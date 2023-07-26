/** @file SsifInterfaceLibCommon.c
  SSIF Transport Implementation common functions and variables.

  @copyright
  Copyright 2016 - 2021 Intel Corporation. <BR>
  Copyright (c) 1985 - 2023, American Megatrends International LLC. <BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/DebugLib.h>
#include <Library/TimerLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/BmcCommonInterfaceLib.h>
#include <Library/SsifInterfaceLib.h>

SSIF_ALERT_PIN_CHECK  *gSsifAlertPinCheckHookList[] =
{
  NULL
};

/*++

Routine Description:
  Check the SMBUS alert pin status function.

Arguments:
  VOID  - Nothing.

Returns:
  TRUE  - Alert pin status is set.
  FALSE - Alert pin status is not set.
--*/
BOOLEAN
CheckAlertPinHook (
  VOID
  )
{
  BOOLEAN  CheckAlertSignal = FALSE;
  UINTN    Index;

  for (Index = 0; gSsifAlertPinCheckHookList[Index]; Index++) {
    CheckAlertSignal = gSsifAlertPinCheckHookList[Index]();
  }

  return CheckAlertSignal;
}

/*++

Routine Description:
  Sends the command/data to SSIF interface.

Arguments:
  Interface  - Pointer to System interface..
  Context    - NULL here.
  Data       - Pointer to command data that will be sent to BMC along with Command.
  DataSize   - Size of the command data.

Returns:
  EFI_STATUS  - Status returned from Smbus send command.
--*/
EFI_STATUS
SendDataToSsifBmcPort (
  IN IPMI_SYSTEM_INTERFACE  *Interface,
  IN VOID                   *Context,
  IN UINT8                  *Data,
  IN UINT8                  DataSize
  )
{
  EFI_STATUS                Status;
  EFI_SMBUS_DEVICE_ADDRESS  BmcAddress;
  UINTN                     IpmiWriteCommand;
  UINT8                     IpmiData[IPMI_SMBUS_BLOCK_LENGTH];
  UINTN                     DataLength;
  UINT8                     DataIndex;
  BOOLEAN                   PECSupport;
  UINT8                     RetryCount;
  UINT8                     OriginalDataSize;

  DataLength                    = DataSize;
  DataIndex                     = 0;
  RetryCount                    = 0;
  OriginalDataSize              = DataSize;
  PECSupport                    = Interface->Ssif.PecSupport;
  BmcAddress.SmbusDeviceAddress = FixedPcdGet16 (PcdSsifSlaveAddress);
  ZeroMem (IpmiData, sizeof (IpmiData));

  do {
    if (OriginalDataSize == DataSize) {
      if (DataSize <= IPMI_SMBUS_BLOCK_LENGTH) {
        // Working single writes start.
        DataLength       = DataSize;
        IpmiWriteCommand = IPMI_SMBUS_SINGLE_WRITE_CMD;
        CopyMem (
                 IpmiData,
                 &Data[DataIndex*IPMI_SMBUS_BLOCK_LENGTH],
                 DataLength
                 );
      } else {
        // Working multi-part writes start.
        IpmiWriteCommand = IPMI_SMBUS_MULTI_WRITE_START_CMD;
        DataLength       = IPMI_SMBUS_BLOCK_LENGTH;
        CopyMem (
                 IpmiData,
                 &Data[DataIndex*IPMI_SMBUS_BLOCK_LENGTH],
                 DataLength
                 );
      }
    } else {
      if (DataSize > IPMI_SMBUS_BLOCK_LENGTH) {
        // Working multi-part writes middle.
        IpmiWriteCommand = IPMI_SMBUS_MULTI_WRITE_MIDDLE_CMD;
        DataLength       = IPMI_SMBUS_BLOCK_LENGTH;
        CopyMem (
                 IpmiData,
                 &Data[DataIndex*IPMI_SMBUS_BLOCK_LENGTH],
                 DataLength
                 );
      } else {
        // Working multi-part writes end.
        IpmiWriteCommand = IPMI_SMBUS_MULTI_WRITE_END_CMD;
        DataLength       = DataSize;
        CopyMem (
                 IpmiData,
                 &Data[DataIndex*IPMI_SMBUS_BLOCK_LENGTH],
                 DataLength
                 );
      }
    }

    Status = IpmiSmbusSendCommand (
                                   Interface,
                                   BmcAddress,
                                   IpmiWriteCommand,
                                   EfiSmbusWriteBlock,
                                   PECSupport,
                                   &DataLength,
                                   IpmiData
                                   );
    if (!EFI_ERROR (Status)) {
      if (DataSize >=  IPMI_SMBUS_BLOCK_LENGTH) {
        RetryCount = 0;
        DataSize  -= IPMI_SMBUS_BLOCK_LENGTH;
        DataIndex++;
      } else {
        DataSize = 0;
      }
    } else {
      if (RetryCount == Interface->Ssif.SsifRetryCounter) {
        break;
      } else {
        RetryCount++;
        // Failed retries delay about 60ms to 250ms.
        MicroSecondDelay (FixedPcdGet32 (PcdSsifRequestRetriesDelay));

        /* If the Multi-part write fails, then try to write the
           data from the beginning.*/
        if (IpmiWriteCommand != IPMI_SMBUS_SINGLE_WRITE_CMD) {
          DataSize  = OriginalDataSize;
          DataIndex = 0;
        }
      }
    }
  } while (DataSize);

  return Status;
}

/*++

Routine Description:
  Receives the Data from BMC port.

Arguments:
  Interface  - Pointer to System interface..
  Context    - NULL here.
  Data       - Pointer to response data that is received from BMC.
  DataSize   - Size of the response data.

Returns:
  EFI_STATUS  - Status returned from Smbus send command.
--*/
EFI_STATUS
ReceiveBmcDataFromSsifPort (
  IN  IPMI_SYSTEM_INTERFACE  *Interface,
  IN  VOID                   *Context,
  OUT UINT8                  *Data,
  OUT UINT8                  *DataSize
  )
{
  EFI_STATUS                Status;
  EFI_SMBUS_DEVICE_ADDRESS  BmcAddress;
  UINTN                     IpmiReadCommand;
  UINT8                     IpmiData[IPMI_SMBUS_BLOCK_LENGTH];
  UINTN                     DataLength;
  BOOLEAN                   PECSupport;
  UINT8                     RetryCount;
  UINT8                     OriginalDataSize;

  DataLength                    = *DataSize;
  RetryCount                    = 0;
  OriginalDataSize              = *DataSize;
  PECSupport                    = Interface->Ssif.PecSupport;
  BmcAddress.SmbusDeviceAddress = FixedPcdGet16 (PcdSsifSlaveAddress);
  IpmiReadCommand               = IPMI_SMBUS_SINGLE_READ_CMD;

  while (RetryCount <= Interface->Ssif.SsifRetryCounter) {
    Status = IpmiSmbusSendCommand (
                                   Interface,
                                   BmcAddress,
                                   IpmiReadCommand,
                                   EfiSmbusReadBlock,
                                   PECSupport,
                                   &DataLength,
                                   (VOID *)IpmiData
                                   );
    if (EFI_ERROR (Status)) {
      RetryCount++;
      // Failed retries delay about 60ms to 250ms.
      MicroSecondDelay (FixedPcdGet32 (PcdSsifRequestRetriesDelay));

      /* If the Multi-part Read command fails, then try to read the
         data from the beginning.*/
      if (IpmiReadCommand != IPMI_SMBUS_SINGLE_READ_CMD) {
        IpmiReadCommand = IPMI_SMBUS_SINGLE_READ_CMD;
      }

      DataLength = OriginalDataSize;
      continue;
    }

    if (IpmiReadCommand == IPMI_SMBUS_SINGLE_READ_CMD) {
      if ((IpmiData[0] == IPMI_MULTI_READ_ZEROTH_STRT_BIT) &&
          (IpmiData[1] == IPMI_MULTI_READ_FIRST_STRT_BIT))
      {
        // Working multi-part reads start.
        CopyMem (
                 Data,
                 &IpmiData[2],
                 DataLength-2
                 );
        *DataSize       = (UINT8)DataLength-2;
        IpmiReadCommand = IPMI_SMBUS_MULTI_READ_MIDDLE_CMD;
      } else {
        // Working single reads start.
        CopyMem (
                 Data,
                 IpmiData,
                 DataLength
                 );
        *DataSize = (UINT8)DataLength;
        break;
      }
    } else {
      if (IpmiData[0] == 0xFF) {
        // Working multi-part reads end.
        CopyMem (
                 &Data[*DataSize],
                 &IpmiData[1],
                 DataLength-1
                 );
        *DataSize += (UINT8)DataLength-1;
        break;
      } else {
        // Working multi-part reads middle.
        CopyMem (
                 &Data[*DataSize],
                 &IpmiData[1],
                 DataLength-1
                 );
        *DataSize      += (UINT8)DataLength-1;
        IpmiReadCommand = IPMI_SMBUS_MULTI_READ_MIDDLE_CMD;
      }
    }
  }

  return Status;
}

/*++

Routine Description:
  SSIF interface Ipmi send command Implementation

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
EFIAPI
IpmiSsifSendCommandToBmc (
  IN      IPMI_TRANSPORT2  *This,
  IN      UINT8            NetFunction,
  IN      UINT8            Lun,
  IN      UINT8            Command,
  IN      UINT8            *CommandData,
  IN      UINT8            CommandDataSize,
  IN OUT  UINT8            *ResponseData,
  IN OUT  UINT8            *ResponseDataSize,
  IN      VOID             *Context
  )
{
  EFI_STATUS             Status;
  UINT8                  DataSize;
  UINT8                  CmdDataBuffer[IPMI_MAX_SSIF_CMD_DATA_SIZE];
  IPMI_SYSTEM_INTERFACE  Interface;

  Interface = This->Interface;

  if (Interface.Ssif.InterfaceState != IpmiInterfaceInitialized) {
    return EFI_NOT_READY;
  }

  if (((CommandData == NULL) && (CommandDataSize != 0)) || (This == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  if ((ResponseDataSize == NULL) || ((ResponseData == NULL) && *ResponseDataSize)) {
    return EFI_INVALID_PARAMETER;
  }

  if (IpmiIsIpmiTransportlocked (&Interface.Ssif.SsifTransportLocked)) {
    return EFI_ACCESS_DENIED;
  } else {
    IpmiTransportAcquireLock (&Interface.Ssif.SsifTransportLocked);
  }

  // Check the SSIF interface multi-part reads/writes supported.
  //   Block length include Command data, NetFn, and Command parameter.
  if (((Interface.Ssif.RwSupport == SsifSinglePartRw) &&
       ((CommandDataSize + 2) > IPMI_SMBUS_BLOCK_LENGTH)) ||
      ((Interface.Ssif.RwSupport == SsifMultiPartRw) &&
       ((CommandDataSize + 2) > (2*IPMI_SMBUS_BLOCK_LENGTH))) ||
      ((Interface.Ssif.RwSupport == SsifMultiPartRwWithMiddle) &&
       ((CommandDataSize + 2) > IPMI_MAX_SSIF_CMD_DATA_SIZE)))
  {
    IpmiTransportReleaseLock (&Interface.Ssif.SsifTransportLocked);
    return EFI_INVALID_PARAMETER;
  }

  if (Interface.Ssif.SsifSoftErrorCount >= MAX_BMC_CMD_FAIL_COUNT) {
    IpmiTransportReleaseLock (&Interface.Ssif.SsifTransportLocked);
    return EFI_NOT_READY;
  }

  CmdDataBuffer[0] = (UINT8)((NetFunction << 2) | (Lun & 0x03));
  CmdDataBuffer[1] = Command;

  if (CommandDataSize > 0) {
    if (CommandData == NULL) {
      IpmiTransportReleaseLock (&Interface.Ssif.SsifTransportLocked);
      return EFI_INVALID_PARAMETER;
    }

    if (CommandDataSize <= (IPMI_MAX_SSIF_CMD_DATA_SIZE - 2)) {
      CopyMem (
               &CmdDataBuffer[2],
               CommandData,
               CommandDataSize
               );
    } else {
      IpmiTransportReleaseLock (&Interface.Ssif.SsifTransportLocked);
      return EFI_BAD_BUFFER_SIZE;
    }
  }

  Status = SendDataToSsifBmcPort (
                                  &Interface,
                                  Context,
                                  CmdDataBuffer,
                                  (UINT8)(CommandDataSize + 2)
                                  );

  if (Status != EFI_SUCCESS) {
    Interface.Ssif.SsifSoftErrorCount++;
    IpmiTransportReleaseLock (&Interface.Ssif.SsifTransportLocked);
    return Status;
  }

  // Hook to check smbus alert pin.
  if (Interface.Ssif.SmbAlertSupport) {
    if (!CheckAlertPinHook ()) {
      IpmiTransportReleaseLock (&Interface.Ssif.SsifTransportLocked);
      return EFI_DEVICE_ERROR;
    }
  } else {
    MicroSecondDelay (FixedPcdGet32 (PcdSsifRequestRetriesDelay));
  }

  DataSize = IPMI_SMBUS_BLOCK_LENGTH;

  Status = ReceiveBmcDataFromSsifPort (
                                       &Interface,
                                       Context,
                                       CmdDataBuffer,
                                       &DataSize
                                       );
  if (Status != EFI_SUCCESS) {
    Interface.Ssif.SsifSoftErrorCount++;
    IpmiTransportReleaseLock (&Interface.Ssif.SsifTransportLocked);
    return Status;
  }

  if (IPMI_ERROR_COMPLETION_CODE (CmdDataBuffer[2])) {
    IpmiUpdateSoftErrorCount (
                              CmdDataBuffer[2],
                              &Interface,
                              This->InterfaceType
                              );
    // Write completion code into return buffer if ipmi command returns an error.
    if (*ResponseDataSize) {
      if (ResponseData) {
        *ResponseData = CmdDataBuffer[2];
      }

      *ResponseDataSize = 1;
    }

    IpmiTransportReleaseLock (&Interface.Ssif.SsifTransportLocked);
    return EFI_DEVICE_ERROR;
  }

  if (DataSize < 3) {
    IpmiTransportReleaseLock (&Interface.Ssif.SsifTransportLocked);
    return EFI_DEVICE_ERROR;
  }

  if ((DataSize - 2) > *((UINT8 *)ResponseDataSize)) {
    *ResponseDataSize = (UINT8)(DataSize - 3);
    IpmiTransportReleaseLock (&Interface.Ssif.SsifTransportLocked);
    return EFI_BUFFER_TOO_SMALL;
  }

  // Copying the response data into ResponseData buffer.
  CopyMem (
           ResponseData,
           &CmdDataBuffer[2],
           (DataSize - 2)
           );
  *ResponseDataSize = (UINT8)(DataSize - 2);

  IpmiTransportReleaseLock (&Interface.Ssif.SsifTransportLocked);
  return EFI_SUCCESS;
}

/*++

Routine Description:
  Execute the Get System Interface Capability command and update the RwSupport
  and PecSupport of Ipmi Instance.

Arguments:
  IpmiTransport2     - Pointer to IPMI transport2 instance.

Returns:
  VOID - Nothing.
--*/
VOID
GetSystemInterfaceCapability (
  IN OUT IPMI_TRANSPORT2  *IpmiTransport2
  )
{
  EFI_STATUS                                Status;
  IPMI_GET_SYSTEM_INTERFACE_CAPABILITY_REQ  GetSystemInterfaceCapabilityCmd;
  IPMI_GET_SYSTEM_INTERFACE_CAPABILITY_RES  GetSsifInterfaceCapability;
  UINT32                                    DataSize = sizeof (GetSsifInterfaceCapability);

  GetSystemInterfaceCapabilityCmd.SystemInterfaceType = 0x0; // SSIF
  GetSystemInterfaceCapabilityCmd.Reserved            = 0x0;

  Status = IpmiTransport2->IpmiSubmitCommand2 (
                                               IpmiTransport2,
                                               IPMI_NETFN_APP,
                                               BMC_LUN,
                                               IPMI_APP_GET_SYSTEM_INTERFACE_CAPABILITIES,
                                               (UINT8 *)&GetSystemInterfaceCapabilityCmd,
                                               sizeof (GetSystemInterfaceCapabilityCmd),
                                               (UINT8 *)&GetSsifInterfaceCapability,
                                               &DataSize
                                               );
  if (!EFI_ERROR (Status)) {
    IpmiTransport2->Interface.Ssif.RwSupport  = GetSsifInterfaceCapability.TransactionSupport;
    IpmiTransport2->Interface.Ssif.PecSupport = GetSsifInterfaceCapability.PecSupport;
  }
}

/*++

Routine Description:
  Execute the Get Global Enable command to get receive message queue interrupt.

Arguments:
  IpmiTransport2     - Pointer to IPMI transport2 instance.

Returns:
  VOID - Nothing.
--*/
VOID
GetGlobalEnables (
  IN OUT IPMI_TRANSPORT2  *IpmiTransport2
  )
{
  EFI_STATUS                   Status;
  IPMI_BMC_GLOBAL_ENABLES_RES  BmcGlobalEnables;
  UINT32                       ResponseDataSize = sizeof (BmcGlobalEnables);

  //
  // Get Global Enable Information.
  //
  Status = IpmiTransport2->IpmiSubmitCommand2 (
                                               IpmiTransport2,
                                               IPMI_NETFN_APP,
                                               BMC_LUN,
                                               IPMI_APP_GET_BMC_GLOBAL_ENABLES,
                                               NULL,
                                               0,
                                               (UINT8 *)(&BmcGlobalEnables),
                                               &ResponseDataSize
                                               );
  if (!EFI_ERROR (Status)) {
    //
    // Set Smb alert pin based on ReceiveMsgQueueInterrupt bit
    //
    IpmiTransport2->Interface.Ssif.SmbAlertSupport = BmcGlobalEnables.ReceiveMsgQueueInterrupt;
  }
}

/*++

Routine Description:
  Initialize SSIF interface specific data.

Arguments:
  IpmiTransport2     - Pointer to IPMI transport2 instance.

Returns:
  Status - Return status while initializing interface.
--*/
EFI_STATUS
InitSsifInterfaceData (
  IN OUT IPMI_TRANSPORT2  *IpmiTransport2
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;

  if (IpmiTransport2->Interface.Ssif.InterfaceState == IpmiInterfaceInitialized) {
    return Status;
  }

  IpmiTransport2->Interface.Ssif.SsifRetryCounter = FixedPcdGet16 (PcdSsifCommandtRetryCounter);
  IpmiTransport2->Interface.Ssif.PecSupport       = FALSE;
  IpmiTransport2->Interface.Ssif.RwSupport        = 0x0;      // SSIF multi-part reads/writes support.
  IpmiTransport2->Interface.Ssif.SmbAlertSupport  = FALSE;    // SMB alert pin support.

  Status = IpmiGetSmbusApiPtr (IpmiTransport2);
  if (EFI_ERROR (Status)) {
    IpmiTransport2->Interface.Ssif.InterfaceState = IpmiInterfaceInitError;
    return Status;
  }

  IpmiTransportReleaseLock (&IpmiTransport2->Interface.Ssif.SsifTransportLocked);
  return Status;
}
