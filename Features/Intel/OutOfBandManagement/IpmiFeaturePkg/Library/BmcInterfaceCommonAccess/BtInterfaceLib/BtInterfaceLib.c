/** @file BtInterfaceLib.c
  BT Transport implementation library functions.

  @copyright
  Copyright 2016 - 2021 Intel Corporation. <BR>
  Copyright (c) 1985 - 2023, American Megatrends International LLC. <BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Uefi/UefiBaseType.h>
#include <Library/DebugLib.h>
#include <Library/TimerLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/BmcCommonInterfaceLib.h>
#include <Library/BtInterfaceLib.h>

#define IPMI_BT_DELAY_PER_RETRY  FixedPcdGet32 (PcdBtDelayPerRetry)

/*++

Routine Description:
  Get the BT interface port addresses based on access type.

Arguments:
  Interface       - Pointer to System interface.
  BtCtrlPort      - Pointer to BT control port.
  BtComBufferPort - Pointer to BT communication buffer port.

Returns:
  VOID - Nothing.

--*/
VOID
GetBtPortAddresses (
  IN  IPMI_SYSTEM_INTERFACE  *Interface,
  OUT UINTN                  *BtCtrlPort,
  OUT UINTN                  *BtComBufferPort
  )
{
  // Update Bt Ports based on Interface AccessType.
  if (Interface->Bt.AccessType == IpmiIoAccess) {
    *BtCtrlPort      = (UINTN)Interface->Bt.CtrlPort;
    *BtComBufferPort = (UINTN)Interface->Bt.ComBuffer;
  } else {
    *BtCtrlPort      = Interface->Bt.MmioBaseAddress;
    *BtComBufferPort = *BtCtrlPort + Interface->Bt.BaseAddressRange;
  }
}

/*++

Routine Description:
  Sends the command to BT interface BMC port.

Arguments:
  Interface - Pointer to System interface.
  Context   - NULL here.
  Data      - Pointer to command data that will be sent to BMC along with Command.
  DataSize  - Size of the command data.

Returns:
  EFI_NOT_READY - Interface is not ready to send data.
  EFI_SUCCESS   - Command sent to BMC successfully.
--*/
EFI_STATUS
SendDataToBtBmcPort (
  IN IPMI_SYSTEM_INTERFACE  *Interface,
  IN VOID                   *Context,
  IN UINT8                  *Data,
  IN UINT8                  DataSize
  )
{
  UINT8             BtCntlData;
  UINT8             Index;
  UINT32            Retry;
  UINTN             BtCtrlPort;
  UINTN             BtComBufferPort;
  IPMI_ACCESS_TYPE  AccessType;
  UINT8             TempDataSize;
  BOOLEAN           MultipleDataSend;
  UINT32            BtRetryCount;

  MultipleDataSend = FALSE;
  BtRetryCount     = Interface->Bt.BtRetryCount;
  AccessType       = Interface->Bt.AccessType;

  // Get Bt Ports addresses.
  GetBtPortAddresses (
                      Interface,
                      &BtCtrlPort,
                      &BtComBufferPort
                      );

  do {
    /* Wait for B_BUSY bit to clear (BMC ready to accept a request).
       Default delay for each retry is 15 micro seconds.*/
    for (Retry = 0; Retry < BtRetryCount; Retry++) {
      BtCntlData = IpmiBmcRead8 (
                                 AccessType,
                                 BtCtrlPort
                                 );
      if (!(BtCntlData & IPMI_B_BUSY_BIT)) {
        break;
      }

      MicroSecondDelay (IPMI_BT_DELAY_PER_RETRY);
    }

    if (Retry == BtRetryCount) {
      return EFI_TIMEOUT;
    }

    // Wait for H2B_ATN bit to clear (Acknowledgment of previous commands).
    for (Retry = 0; Retry < BtRetryCount; Retry++) {
      BtCntlData = IpmiBmcRead8 (
                                 AccessType,
                                 BtCtrlPort
                                 );
      if (!(BtCntlData & IPMI_H2B_ATN_BIT)) {
        break;
      }

      MicroSecondDelay (IPMI_BT_DELAY_PER_RETRY);
    }

    if (Retry == BtRetryCount) {
      return EFI_TIMEOUT;
    }

    // Set CLR_WR_PTR.
    BtCntlData = IPMI_CLR_WR_PTR_BIT;
    IpmiBmcWrite8 (
                   AccessType,
                   BtCtrlPort,
                   BtCntlData
                   );

    if (DataSize > Interface->Bt.HosttoBmcBufferSize ) {
      TempDataSize     = Interface->Bt.HosttoBmcBufferSize;
      MultipleDataSend = TRUE;
    } else {
      TempDataSize     = DataSize;
      MultipleDataSend = FALSE;
    }

    // Send each message byte out (write data to HOST2BMC buffer).
    for (Index = 0; Index < TempDataSize; Index++) {
      IpmiBmcWrite8 (
                     AccessType,
                     BtComBufferPort,
                     *(Data + Index)
                     );
    }

    // Set H2B_ATN bit to inform BMC that data is available.
    BtCntlData = IPMI_H2B_ATN_BIT;
    IpmiBmcWrite8 (
                   AccessType,
                   BtCtrlPort,
                   BtCntlData
                   );

    // Command data size greater than available Input buffer size.
    if (MultipleDataSend) {
      Data      = Data + TempDataSize;
      DataSize -= TempDataSize;

      for (Retry = 0; Retry < BtRetryCount; Retry++) {
        BtCntlData = IpmiBmcRead8 (
                                   AccessType,
                                   BtCtrlPort
                                   );
        if ((BtCntlData & IPMI_B_BUSY_BIT)) {
          break;
        }

        MicroSecondDelay (IPMI_BT_DELAY_PER_RETRY);
      }

      if (Retry == BtRetryCount) {
        return EFI_TIMEOUT;
      }
    }
  } while (MultipleDataSend);

  return EFI_SUCCESS;
}

/*++

Routine Description:
  Receives the Data from BT interface BMC port.

Arguments:
  Interface - Pointer to System interface.
  Context   - NULL here.
  Data      - Pointer to response data that is received from BMC.
  DataSize  - Size of the response data.

Returns:
  EFI_NOT_READY         - Interface is not ready to receive data.
  EFI_SUCCESS           - Data received from BMC successfully.
  EFI_INVALID_PARAMETER - Invalid parameter.
--*/
EFI_STATUS
ReceiveBmcDataFromBtPort (
  IN  IPMI_SYSTEM_INTERFACE  *Interface,
  IN  VOID                   *Context,
  OUT UINT8                  *Data,
  OUT UINT8                  *DataSize
  )
{
  UINT8             BtCntlData;
  UINT8             Length;
  UINT8             TempDataSize;
  UINT8             Index;
  UINT32            Retry;
  UINTN             BtCtrlPort;
  UINTN             BtComBufferPort;
  IPMI_ACCESS_TYPE  AccessType;
  BOOLEAN           MultipleDataReceive;
  UINT32            BtRetryCount;

  Length              = 0;
  MultipleDataReceive = FALSE;
  BtRetryCount        = Interface->Bt.BtRetryCount;
  AccessType          = Interface->Bt.AccessType;

  // Get Bt Ports addresses.
  GetBtPortAddresses (
                      Interface,
                      &BtCtrlPort,
                      &BtComBufferPort
                      );
  do {
    /* Wait for B2H_ATN bit to be set,signaling data is available for host.
    Default delay for each retry is 15 micro seconds.*/
    for (Retry = 0; Retry < BtRetryCount; Retry++) {
      BtCntlData = IpmiBmcRead8 (
                                 AccessType,
                                 BtCtrlPort
                                 );
      if (BtCntlData & IPMI_B2H_ATN_BIT) {
        break;
      }

      MicroSecondDelay (IPMI_BT_DELAY_PER_RETRY);
    }

    if (Retry == BtRetryCount) {
      return EFI_TIMEOUT;
    }

    // Set H_BUSY bit, indicating host is in process of reading data from interface.
    BtCntlData = IpmiBmcRead8 (
                               AccessType,
                               BtCtrlPort
                               );
    if (!(BtCntlData & IPMI_H_BUSY)) {
      BtCntlData = IPMI_H_BUSY;            // most bits are rw1c, so clear them.
      IpmiBmcWrite8 (
                     AccessType,
                     BtCtrlPort,
                     BtCntlData
                     );
    }

    // Clear B2H_ATN bit,to acknowledge receipt of message response.
    BtCntlData = IPMI_B2H_ATN_BIT;       // Most bits are rw1c, so clear them.
    IpmiBmcWrite8 (
                   AccessType,
                   BtCtrlPort,
                   BtCntlData
                   );

    // Set CLR_RD_PTR bit.
    BtCntlData = IPMI_CLR_RD_PTR_BIT;    // Most bits are rw1c, so clear them.
    IpmiBmcWrite8 (
                   AccessType,
                   BtCtrlPort,
                   BtCntlData
                   );

    if (!Length) {
      // Read the data bytes from BMC.
      Length = IpmiBmcRead8 (
                             AccessType,
                             BtComBufferPort
                             );
      if (Length == 0x00) {
        return EFI_INVALID_PARAMETER;
      }

      IpmiBmcWrite8 (
                     AccessType,
                     BtCtrlPort,
                     BtCntlData
                     );

      *DataSize = Length;
      // Increment Length to include length field
      Length++;
    }

    if (Length > Interface->Bt.BmctoHostBufferSize) {
      TempDataSize        = Interface->Bt.BmctoHostBufferSize;
      MultipleDataReceive = TRUE;
    } else {
      TempDataSize        = Length;
      MultipleDataReceive = FALSE;
    }

    for (Index = 0; Index < TempDataSize; Index++) {
      *(Data + Index) = IpmiBmcRead8 (
                                      AccessType,
                                      BtComBufferPort
                                      );
    }

    // Clear H_BUSY bit indicating host is done reading data from BMC.
    BtCntlData = IpmiBmcRead8 (
                               AccessType,
                               BtCtrlPort
                               );
    if (BtCntlData & IPMI_H_BUSY) {
      BtCntlData = IPMI_H_BUSY;            // Most bits are rw1c, so clear them.
      IpmiBmcWrite8 (
                     AccessType,
                     BtCtrlPort,
                     BtCntlData
                     );
    }

    if (MultipleDataReceive) {
      Data    = Data + TempDataSize;
      Length -= TempDataSize;
    }
  } while (MultipleDataReceive);

  return EFI_SUCCESS;
}

/*++

Routine Description:
  BT interface send command implementation.

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
IpmiBtSendCommandToBmc (
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
  UINT8                  DataSize;
  EFI_STATUS             Status;
  UINT8                  Seq;
  UINT8                  CmdDataBuffer[IPMI_MAX_BT_CMD_DATA_SIZE];
  IPMI_SYSTEM_INTERFACE  Interface;

  Seq       = 0;
  Interface = This->Interface;

  if (Interface.Bt.InterfaceState != IpmiInterfaceInitialized) {
    return EFI_NOT_READY;
  }

  if (((CommandData == NULL) && (CommandDataSize != 0)) || (This == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  if ((ResponseDataSize == NULL) || ((ResponseData == NULL) && *ResponseDataSize)) {
    return EFI_INVALID_PARAMETER;
  }

  if (IpmiIsIpmiTransportlocked (&Interface.Bt.BtTransportLocked)) {
    return EFI_ACCESS_DENIED;
  } else {
    IpmiTransportAcquireLock (&Interface.Bt.BtTransportLocked);
  }

  CmdDataBuffer[0] = (UINT8)CommandDataSize + 0x03;
  CmdDataBuffer[1] = (UINT8)((NetFunction << 2) | (Lun & 0xfc));
  CmdDataBuffer[2] = Seq;
  CmdDataBuffer[3] = Command;

  if (CommandDataSize > 0) {
    if (CommandData == NULL) {
      IpmiTransportReleaseLock (&Interface.Bt.BtTransportLocked);
      return EFI_INVALID_PARAMETER;
    }

    if (CommandDataSize <= (IPMI_MAX_BT_CMD_DATA_SIZE - 4)) {
      CopyMem (
               &CmdDataBuffer[4],
               CommandData,
               CommandDataSize
               );
    } else {
      IpmiTransportReleaseLock (&Interface.Bt.BtTransportLocked);
      return EFI_BAD_BUFFER_SIZE;
    }
  }

  Status = SendDataToBtBmcPort (
                                &Interface,
                                Context,
                                CmdDataBuffer,
                                (UINT8)(CommandDataSize + 4)
                                );

  if (Status != EFI_SUCCESS) {
    Interface.Bt.BtSoftErrorCount++;
    IpmiTransportReleaseLock (&Interface.Bt.BtTransportLocked);
    return Status;
  }

  DataSize = IPMI_MAX_BT_CMD_DATA_SIZE;

  Status = ReceiveBmcDataFromBtPort (
                                     &Interface,
                                     Context,
                                     CmdDataBuffer,
                                     &DataSize
                                     );

  if (Status != EFI_SUCCESS) {
    Interface.Bt.BtSoftErrorCount++;
    IpmiTransportReleaseLock (&Interface.Bt.BtTransportLocked);
    return Status;
  }

  if (IPMI_ERROR_COMPLETION_CODE (CmdDataBuffer[4])) {
    IpmiUpdateSoftErrorCount (
                              CmdDataBuffer[4],
                              &Interface,
                              This->InterfaceType
                              );

    // Write completion code into return buffer if ipmi command returns an error.
    if (*ResponseDataSize) {
      if (ResponseData) {
        *ResponseData = CmdDataBuffer[4];
      }

      *ResponseDataSize = 1;
    }

    IpmiTransportReleaseLock (&Interface.Bt.BtTransportLocked);
    return EFI_DEVICE_ERROR;
  }

  if (DataSize < 4) {
    IpmiTransportReleaseLock (&Interface.Bt.BtTransportLocked);
    return EFI_DEVICE_ERROR;
  }

  if ((DataSize - 3) > *((UINT8 *)ResponseDataSize)) {
    *ResponseDataSize = (UINT8)(DataSize - 3);
    IpmiTransportReleaseLock (&Interface.Bt.BtTransportLocked);
    return EFI_BUFFER_TOO_SMALL;
  }

  // Copying the response data into ResponseData buffer.
  CopyMem (
           ResponseData,
           &CmdDataBuffer[4],
           (DataSize - 3)
           );
  *ResponseDataSize = (UINT8)(DataSize - 3);

  IpmiTransportReleaseLock (&Interface.Bt.BtTransportLocked);

  return EFI_SUCCESS;
}

/*++

Routine Description:
  Execute the Get BT Interface Capability command and update the input
  and output buffer value of IPMI transport2.

Arguments:
  IpmiTransport2 - IPMI transport2 protocol Instance.

Returns:
  VOID - Nothing.
--*/
VOID
GetBtInterfaceCapability (
  IN OUT IPMI_TRANSPORT2  *IpmiTransport2
  )
{
  EFI_STATUS                        Status;
  IPMI_BT_INTERFACE_CAPABILITY_RES  Responsedata;
  UINT32                            ResponseSize;

  ResponseSize = sizeof (IPMI_BT_INTERFACE_CAPABILITY_RES);

  Status = IpmiTransport2->IpmiSubmitCommand2 (
                                               IpmiTransport2,
                                               IPMI_NETFN_APP,
                                               BMC_LUN,
                                               IPMI_APP_GET_BT_INTERFACE_CAPABILITY,
                                               NULL,
                                               0,
                                               (UINT8 *)&Responsedata,
                                               &ResponseSize
                                               );

  if (EFI_ERROR (Status) || Responsedata.CompletionCode) {
    DEBUG ((DEBUG_ERROR, " IPMI_APP_GET_BT_INTERFACE_CAPABILITY Status: %r Completion code: %x\n", Status, Responsedata.CompletionCode));
    return;
  }

  IpmiTransport2->Interface.Bt.HosttoBmcBufferSize = Responsedata.InputBuffSize;
  IpmiTransport2->Interface.Bt.BmctoHostBufferSize = Responsedata.OutputBuffSize;

  DEBUG ((DEBUG_ERROR, " InputBuffSize:%x OutBuffSize%x  BtRetry %x Status %r \n", IpmiTransport2->Interface.Bt.HosttoBmcBufferSize, IpmiTransport2->Interface.Bt.BmctoHostBufferSize, Responsedata.RecommandedRetires, Status));

  return;
}

/*++

Routine Description:
  Initialize BT interface specific data.

Arguments:
  IpmiTransport2 - IPMI transport2 protocol pointer.

Returns:
  Status.
--*/
EFI_STATUS
InitBtInterfaceData (
  IN OUT IPMI_TRANSPORT2  *IpmiTransport2
  )
{
  BMC_INTERFACE_STATUS  BmcStatus;
  EFI_STATUS            Status;

  if (IpmiTransport2->Interface.Bt.InterfaceState == IpmiInterfaceInitialized) {
    return EFI_SUCCESS;
  }

  IpmiTransport2->Interface.Bt.CtrlPort            = FixedPcdGet16 (PcdBtControlPort);           // BT Control Port
  IpmiTransport2->Interface.Bt.ComBuffer           = FixedPcdGet16 (PcdBtBufferPort);            // BT Buffer Port
  IpmiTransport2->Interface.Bt.IntMaskPort         = FixedPcdGet16 (PcdBtInterruptMaskPort);     // BT IntMask Port
  IpmiTransport2->Interface.Bt.BtRetryCount        = FixedPcdGet32 (PcdBtCommandRetryCounter);   // BT retry count
  IpmiTransport2->Interface.Bt.HosttoBmcBufferSize = FixedPcdGet8 (PcdBtBufferSize);             // Host to Bmc Buffer Size.
  IpmiTransport2->Interface.Bt.BmctoHostBufferSize = FixedPcdGet8 (PcdBtBufferSize);             // Bmc to Host Buffer Size.

  if (FixedPcdGet8 (PcdIpmiDefaultAccessType)) {
    IpmiTransport2->Interface.Bt.AccessType       = IpmiIoAccess;
    IpmiTransport2->Interface.Bt.MmioBaseAddress  = 0;
    IpmiTransport2->Interface.Bt.BaseAddressRange = 0;
  } else {
    IpmiTransport2->Interface.Bt.AccessType       = IpmiMmioAccess;
    IpmiTransport2->Interface.Bt.MmioBaseAddress  = FixedPcdGet64 (PcdMmioBaseAddress);
    IpmiTransport2->Interface.Bt.BaseAddressRange = FixedPcdGet64 (PcdBaseAddressRange);
  }

  IpmiTransportReleaseLock (&IpmiTransport2->Interface.Bt.BtTransportLocked);
  IpmiTransport2->Interface.Bt.InterfaceState = IpmiInterfaceInitialized;

  Status = CheckSelfTestByInterfaceType (
                                         IpmiTransport2,
                                         &BmcStatus,
                                         SysInterfaceBt
                                         );
  if (EFI_ERROR (Status) || (BmcStatus == BmcStatusHardFail)) {
    IpmiTransport2->Interface.Bt.InterfaceState = IpmiInterfaceInitError;
    return Status;
  }

  GetBtInterfaceCapability (IpmiTransport2);

  return Status;
}
