/** @file IpmbInterfaceLibCommon.c
  IPMB Transport implementation common library functions.

  @copyright
  Copyright 2016 - 2021 Intel Corporation. <BR>
  Copyright (c) 1985 - 2023, American Megatrends International LLC. <BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/BmcCommonInterfaceLib.h>
#include <Library/IpmbInterfaceLib.h>

#define  IPMI_BMC_SLAVE_ADDRESS  FixedPcdGet32 (PcdBmcSlaveAddress)

/*++

Routine Description:
  Verify the data integrity using checksum of BMC response data.

Arguments:
  ResponseData  - Response data from BMC.
  ResponseSize  - Data size of response data.

Returns:
  EFI_SUCCESS           - Data integrity is valid.
  EFI_INVALID_PARAMETER - Invalid parameter.
--*/
EFI_STATUS
CheckDataValidity (
  IN UINT8  *ResponseData,
  IN UINT8  ResponseSize
  )
{
  UINT8  Index;
  UINT8  CheckSum = 0;
  UINT8  DataSum  = 0;

  // Calculate header checksum.
  for (Index = 0; Index < 2; Index++) {
    DataSum += ResponseData[Index];
  }

  // Verify header checksum.
  CheckSum = (UINT8)(0x100 - DataSum);
  if (CheckSum != ResponseData[2]) {
    return EFI_INVALID_PARAMETER;
  }

  DataSum = 0;

  // Calculate information checksum.
  for (Index = 3; Index < (ResponseSize - 1); Index++) {
    DataSum += ResponseData[Index];
  }

  // Verify information checksum.
  CheckSum = (UINT8)(0x100 - DataSum);
  if (CheckSum != ResponseData[ResponseSize - 1]) {
    return EFI_INVALID_PARAMETER;
  }

  return EFI_SUCCESS;
}

/*++

Routine Description:
  Sends the command/data to IPMB interface.

Arguments:
  Interface  - Pointer to System interface.
  Context    - NULL here.
  Data       - Pointer to command data that will be sent to BMC along with Command.
  DataSize   - Size of the command data.

Returns:
  EFI_STATUS           - Status returned from I2C send command.
--*/
EFI_STATUS
SendDataToIpmbBmcPort (
  IN IPMI_SYSTEM_INTERFACE  *Interface,
  IN VOID                   *Context,
  IN UINT8                  *Data,
  IN UINT8                  DataSize
  )
{
  EFI_STATUS              Status;
  EFI_I2C_REQUEST_PACKET  Packet;

  // Pack command data.
  Packet.Operation[0].Buffer        = Data;
  Packet.Operation[0].LengthInBytes = DataSize;
  Packet.Operation[0].Flags         = IPMI_WRITE_FLAG;

  // Call the StartRequest function.
  Status = IpmiI2cSendCommand (
                               Interface,
                               IPMI_BMC_SLAVE_ADDRESS,
                               &Packet
                               );

  return Status;
}

/*++

Routine Description:
  Receives the data from IPMB interface.

Arguments:
  Interface  - Pointer to System interface.
  Context    - NULL here.
  Data       - Pointer to response data that is received from BMC.
  DataSize   - Size of the response data.

Returns:
  EFI_STATUS           - Status returned from I2C send command.
--*/
EFI_STATUS
ReceiveBmcDataFromIpmbPort (
  IN  IPMI_SYSTEM_INTERFACE  *Interface,
  IN  VOID                   *Context,
  OUT UINT8                  *Data,
  OUT UINT8                  *DataSize
  )
{
  EFI_STATUS              Status;
  EFI_I2C_REQUEST_PACKET  Packet;

  // Pack command data.
  Packet.Operation[0].Buffer = Data;
  Packet.Operation[0].Flags  = IPMI_READ_FLAG;

  // Call the StartRequest function.
  Status = IpmiI2cSendCommand (
                               Interface,
                               IPMI_BMC_SLAVE_ADDRESS,
                               &Packet
                               );

  if (!EFI_ERROR (Status)) {
    *DataSize = (UINT8)Packet.Operation[0].LengthInBytes;
  }

  return Status;
}

/*++

Routine Description:
  IPMB interface send command implementation.

Arguments:
  This              - Pointer to IPMI protocol instance.
  NetFunction       - Net Function of command to send.
  Lun               - LUN of command to send.
  Command           - IPMI command to send.
  CommandData       - Pointer to command data buffer, if needed.
  CommandDataSize   - Size of command data buffer.
  ResponseData      - Pointer to response data buffer.
  ResponseDataSize  - Pointer to response data buffer size.
  Context           - NULL here.

Returns:
    EFI_INVALID_PARAMETER - One of the input values is bad.
  EFI_DEVICE_ERROR      - IPMI command failed.
  EFI_BUFFER_TOO_SMALL  - Response buffer is too small.
  EFI_UNSUPPORTED       - Command is not supported by BMC.
  EFI_SUCCESS           - Command completed successfully.
--*/
EFI_STATUS
IpmiIpmbSendCommandToBmc (
  IN     IPMI_TRANSPORT2  *This,
  IN     UINT8            NetFunction,
  IN     UINT8            Lun,
  IN     UINT8            Command,
  IN     UINT8            *CommandData,
  IN     UINT8            CommandDataSize,
  OUT    UINT8            *ResponseData,
  IN OUT UINT8            *ResponseDataSize,
  IN     VOID             *Context
  )
{
  EFI_STATUS             Status;
  UINT8                  DataSize;
  UINT8                  DataSum  = 0;
  UINT8                  CheckSum = 0;
  UINT8                  Index;
  UINT8                  CmdDataBuffer[IPMI_MAX_IPMB_CMD_DATA_SIZE];
  IPMI_SYSTEM_INTERFACE  Interface;

  Interface = This->Interface;

  if (Interface.Ipmb.InterfaceState != IpmiInterfaceInitialized) {
    return EFI_NOT_READY;
  }

  if (!ResponseDataSize || (!ResponseData && *ResponseDataSize)) {
    return EFI_INVALID_PARAMETER;
  }

  if (IpmiIsIpmiTransportlocked (&Interface.Ipmb.IpmbTransportLocked)) {
    return EFI_ACCESS_DENIED;
  } else {
    IpmiTransportAcquireLock (&Interface.Ipmb.IpmbTransportLocked);
  }

  if (Interface.Ipmb.IpmbSoftErrorCount >= MAX_BMC_CMD_FAIL_COUNT) {
    IpmiTransportReleaseLock (&Interface.Ipmb.IpmbTransportLocked);
    return EFI_NOT_READY;
  }

  /* Request Packet format.
      | Slave Address | Netfun/Lun | CheckSum (Check Sum of previous data)
      | Slave Address | Seq No | Command | Data 1..N
      | CheckSum (Check Sum of previous data).*/
  CmdDataBuffer[0] = IPMI_BMC_SLAVE_ADDRESS;
  CmdDataBuffer[1] = (UINT8)((NetFunction << 2) | (Lun & 0x03));

  DataSum += CmdDataBuffer[1] + IPMI_BMC_SLAVE_ADDRESS;
  CheckSum = (UINT8)(0x100 - DataSum);
  DataSum  = 0;

  CmdDataBuffer[2] = CheckSum;

  CmdDataBuffer[3] = IPMI_BMC_SLAVE_ADDRESS;
  DataSum         += IPMI_BMC_SLAVE_ADDRESS;

  CmdDataBuffer[4] = IPMI_SEQ_NO;
  DataSum         += IPMI_SEQ_NO;

  CmdDataBuffer[5] = Command;
  DataSum         += Command;

  if (CommandDataSize > 0) {
    if (CommandData == NULL) {
      IpmiTransportReleaseLock (&Interface.Ipmb.IpmbTransportLocked);
      return EFI_INVALID_PARAMETER;
    }

    // Last data will be Checksum so limiting the Max command data to < IPMI_MAX_IPMB_CMD_DATA_SIZE - 6
    if (CommandDataSize < (IPMI_MAX_IPMB_CMD_DATA_SIZE - 6)) {
      CopyMem (
               &CmdDataBuffer[6],
               CommandData,
               CommandDataSize
               );
    } else {
      IpmiTransportReleaseLock (&Interface.Ipmb.IpmbTransportLocked);
      return EFI_BAD_BUFFER_SIZE;
    }

    for (Index = 0; Index < CommandDataSize; Index++) {
      DataSum += CmdDataBuffer[6 + Index];
    }
  }

  CheckSum         = (UINT8)(0x100 - DataSum); // Find the checksum for the packing data.
  CmdDataBuffer[6] = CheckSum;                 // Update the checksum.

  if ((Status = SendDataToIpmbBmcPort (
                                       &Interface,
                                       Context,
                                       CmdDataBuffer,
                                       (UINT8)(CommandDataSize + 7)
                                       )) != EFI_SUCCESS)
  {
    Interface.Ipmb.IpmbSoftErrorCount++;
    IpmiTransportReleaseLock (&Interface.Ipmb.IpmbTransportLocked);
    return Status;
  }

  DataSize = IPMI_MAX_IPMB_CMD_DATA_SIZE;
  if ((Status = ReceiveBmcDataFromIpmbPort (
                                            &Interface,
                                            Context,
                                            CmdDataBuffer,
                                            &DataSize
                                            )) != EFI_SUCCESS)
  {
    Interface.Ipmb.IpmbSoftErrorCount++;
    IpmiTransportReleaseLock (&Interface.Ipmb.IpmbTransportLocked);
    return Status;
  }

  /* Response Packet format.
     | Slave Address | Netfun/Lun | CheckSum (Check Sum of previous data)
     | Slave Address | Seq No | Command | Completion code| Data 1..N
     | CheckSum (Check Sum of previous data).*/

  // Calculate and verify checksum.
  Status = CheckDataValidity (
                              CmdDataBuffer,
                              DataSize
                              );
  if (EFI_ERROR (Status)) {
    IpmiTransportReleaseLock (&Interface.Ipmb.IpmbTransportLocked);
    return Status;
  }

  if (IPMI_ERROR_COMPLETION_CODE (CmdDataBuffer[6])) {
    IpmiUpdateSoftErrorCount (
                              CmdDataBuffer[6],
                              &Interface,
                              This->InterfaceType
                              );
    // Write completion code into return buffer if an IPMI command returns an error
    if (*ResponseDataSize) {
      if (ResponseData) {
        *ResponseData = CmdDataBuffer[6];
      }

      *ResponseDataSize = 1;
    }

    IpmiTransportReleaseLock (&Interface.Ipmb.IpmbTransportLocked);
    return EFI_DEVICE_ERROR;
  }

  if (DataSize < 8) {
    IpmiTransportReleaseLock (&Interface.Ipmb.IpmbTransportLocked);
    return EFI_DEVICE_ERROR;
  }

  if ((DataSize - 7) > *((UINT8 *)ResponseDataSize)) {
    *ResponseDataSize = (UINT8)(DataSize - 7);
    IpmiTransportReleaseLock (&Interface.Ipmb.IpmbTransportLocked);
    return EFI_BUFFER_TOO_SMALL;
  }

  // Copying the response data into ResponseData buffer.
  CopyMem (
           ResponseData,
           &CmdDataBuffer[6],
           (DataSize - 7)
           );
  *ResponseDataSize = (UINT8)(DataSize - 7);

  IpmiTransportReleaseLock (&Interface.Ipmb.IpmbTransportLocked);
  return EFI_SUCCESS;
}

/*++

Routine Description:
  Initialize IPMB interface specific data.

Arguments:
  IpmiTransport2    - Pointer to IPMI Transport2 instance.

Returns:
  EFI_SUCCESS - Interface is successfully initialized.
  Others      - Error status while initializing interface.
--*/
EFI_STATUS
InitIpmbInterfaceData (
  IN OUT IPMI_TRANSPORT2  *IpmiTransport2
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;

  if (IpmiTransport2->Interface.Ipmb.InterfaceState == IpmiInterfaceInitialized) {
    return Status;
  }

  Status = IpmiGetI2cApiPtr (IpmiTransport2);

  if (EFI_ERROR (Status)) {
    IpmiTransport2->Interface.Ipmb.InterfaceState = IpmiInterfaceInitError;
    return Status;
  }

  IpmiTransportReleaseLock (&IpmiTransport2->Interface.Ipmb.IpmbTransportLocked);
  IpmiTransport2->Interface.Ipmb.InterfaceState = IpmiInterfaceInitialized;
  return Status;
}
