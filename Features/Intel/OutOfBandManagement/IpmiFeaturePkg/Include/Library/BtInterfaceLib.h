/** @file BtInterfaceLib.h
  BT interface common macros and declarations.

  @copyright
  Copyright 2016 - 2021 Intel Corporation. <BR>
  Copyright (c) 1985 - 2023, American Megatrends International LLC. <BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _BT_INTERFACE_LIB_H_
#define _BT_INTERFACE_LIB_H_

#include <Protocol/IpmiTransport2Protocol.h>
#include <Include/IpmiNetFnAppDefinitions.h>

#define IPMI_MAX_BT_CMD_DATA_SIZE  0xFF
#define IPMI_CLR_WR_PTR_BIT        0x01
#define IPMI_CLR_RD_PTR_BIT        0x02
#define IPMI_H2B_ATN_BIT           0x04
#define IPMI_B2H_ATN_BIT           0x08
#define IPMI_H_BUSY                0x40
#define IPMI_B_BUSY_BIT            0x80

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
  );

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
  );

#endif
