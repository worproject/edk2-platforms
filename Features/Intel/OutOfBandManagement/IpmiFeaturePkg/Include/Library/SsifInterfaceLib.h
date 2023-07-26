/** @file SsifInterfaceLib.h
  SSIF interface common function declarations and macros.

  @copyright
  Copyright 2016 - 2021 Intel Corporation. <BR>
  Copyright (c) 1985 - 2023, American Megatrends International LLC. <BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _SSIF_INTERFACE_LIB_H
#define _SSIF_INTERFACE_LIB_H

#include <Protocol/SmbusHc.h>
#include <Library/BmcCommonInterfaceLib.h>
#include <Protocol/IpmiTransport2Protocol.h>
#include <Include/IpmiNetFnAppDefinitions.h>

#define IPMI_MAX_SSIF_CMD_DATA_SIZE  0xFF
#define IPMI_SMBUS_BLOCK_LENGTH      0x20

// Smbus Write Commands.
#define IPMI_SMBUS_SINGLE_WRITE_CMD        0x2
#define IPMI_SMBUS_MULTI_WRITE_START_CMD   0x6
#define IPMI_SMBUS_MULTI_WRITE_MIDDLE_CMD  0x7
#define IPMI_SMBUS_MULTI_WRITE_END_CMD     0x8

// Smbus Read Commands.
#define IPMI_SMBUS_SINGLE_READ_CMD        0x3
#define IPMI_SMBUS_MULTI_READ_START_CMD   SMBUS_SINGLE_READ_CMD
#define IPMI_SMBUS_MULTI_READ_MIDDLE_CMD  0x9
#define IPMI_SMBUS_MULTI_READ_END_CMD     0x9
#define IPMI_SMBUS_MULTI_READ_RETRY_CMD   0xA

#define IPMI_MULTI_READ_ZEROTH_STRT_BIT  0x0
#define IPMI_MULTI_READ_FIRST_STRT_BIT   0x1

/*++

Routine Description:
  Check the SMBUS alert pin status function.

Arguments:
  VOID  - Nothing.

Returns:
  TRUE  - Alert pin status is set.
  FALSE - Alert pin status is not set.
--*/
typedef BOOLEAN (SSIF_ALERT_PIN_CHECK) (
  VOID
  );

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
  );

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
  );

/*++

Routine Description:
  Locate Smbus instance and initialize interface pointer.

Arguments:
  IpmiTransport2     - Pointer to IPMI transport2 instance.

Returns:
  Status - Status returned while locating SMBUS instance.
--*/
EFI_STATUS
IpmiGetSmbusApiPtr (
  IN OUT IPMI_TRANSPORT2  *IpmiTransport2
  );

/*++

Routine Description:
  Send IPMI command through SMBUS instance.

Arguments:
  Interface     - Pointer to System interface.
  SlaveAddress  - The SMBUS hardware address.
  Command       - This command is transmitted by the SMBus host controller to the SMBus slave device..
  Operation     - Operation to be performed.
  PecCheck      - Defines if Packet Error Code (PEC) checking is required for this operation.
  Length        - Signifies the number of bytes that this operation will do.
  Buffer        - Contains the value of data to execute to
                  the SMBus slave device. The length of this buffer is identified by Length.

Returns:
  EFI_NOT_FOUND - SMBUS instance is not found.
  Others        - Return status of the SMBUS Execute operation.
--*/
EFI_STATUS
IpmiSmbusSendCommand (
  IN     IPMI_SYSTEM_INTERFACE     *Interface,
  IN     EFI_SMBUS_DEVICE_ADDRESS  SlaveAddress,
  IN     EFI_SMBUS_DEVICE_COMMAND  Command,
  IN     EFI_SMBUS_OPERATION       Operation,
  IN     BOOLEAN                   PecCheck,
  IN OUT UINTN                     *Length,
  IN OUT VOID                      *Buffer
  );

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
  );

#endif
