/** @file IpmbInterfaceLib.h
  IPMB interface common function declarations and macros.

  @copyright
  Copyright 2016 - 2021 Intel Corporation. <BR>
  Copyright (c) 1985 - 2023, American Megatrends International LLC. <BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _IPMB_INTERFACE_LIB_H_
#define _IPMB_INTERFACE_LIB_H_

#include <Pi/PiI2c.h>
#include <Protocol/IpmiTransport2Protocol.h>
#include <Include/IpmiNetFnAppDefinitions.h>

#define IPMI_MAX_IPMB_CMD_DATA_SIZE  0xFF
#define IPMI_READ_FLAG               1
#define IPMI_WRITE_FLAG              0
#define IPMI_SEQ_NO                  0    // IPMB Message Sequence Number.

/*++

Routine Description:
  Locate I2c Ppi/Protocol instance and initialize interface pointer.

Arguments:
  IpmiTransport2     - Pointer to IPMI transport2 instance.

Returns:
  Status - Status returned from functions used.
--*/
EFI_STATUS
IpmiGetI2cApiPtr (
  IN OUT IPMI_TRANSPORT2  *IpmiTransport2
  );

/*++

Routine Description:
  Send IPMI command through IPMB interface.

Arguments:
  Interface     - Pointer to System interface.
  SlaveAddress  - I2C device slave address.
  RequestPacket - Pointer to an EFI_I2C_REQUEST_PACKET structure describing the I2C transaction.

Returns:
  Status - Status of the Send I2c command.
--*/
EFI_STATUS
IpmiI2cSendCommand (
  IN IPMI_SYSTEM_INTERFACE   *Interface,
  IN UINTN                   SlaveAddress,
  IN EFI_I2C_REQUEST_PACKET  *RequestPacket
  );

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
  );

#endif // #ifndef _IPMB_INTERFACE_LIB_H
