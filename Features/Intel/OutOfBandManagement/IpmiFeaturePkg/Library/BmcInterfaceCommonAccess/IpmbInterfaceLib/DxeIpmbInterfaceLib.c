/** @file DxeIpmbInterfaceLib.c
  IPMB Transport Dxe phase Implementation library functions.

  @copyright
  Copyright 2016 - 2021 Intel Corporation. <BR>
  Copyright (c) 1985 - 2023, American Megatrends International LLC. <BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BmcCommonInterfaceLib.h>
#include <Library/IpmbInterfaceLib.h>
#include <Protocol/IpmiTransport2Protocol.h>
#include <Protocol/I2cMaster.h>

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
  )
{
  EFI_STATUS               Status             = EFI_NOT_FOUND;
  EFI_I2C_MASTER_PROTOCOL  *I2cMasterTransmit = NULL;

  I2cMasterTransmit = (EFI_I2C_MASTER_PROTOCOL *)Interface->Ipmb.IpmbInterfaceApiPtr;

  if (I2cMasterTransmit != NULL) {
    Status = I2cMasterTransmit->StartRequest (
                                              I2cMasterTransmit,
                                              SlaveAddress,
                                              RequestPacket,
                                              NULL,
                                              NULL
                                              );
  }

  DEBUG ((DEBUG_INFO, "I2cMasterTransmit->StartRequest Status = %r\n", Status));
  return Status;
}

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
  )
{
  EFI_STATUS               Status;
  EFI_I2C_MASTER_PROTOCOL  *I2cMasterTransmit = NULL;
  BMC_INTERFACE_STATUS     BmcStatus;

  IpmiTransport2->Interface.Ipmb.IpmbInterfaceApiGuid = gEfiI2cMasterProtocolGuid;

  // Locate the I2C DXE Protocol for Communication.
  Status = gBS->LocateProtocol (
                                &gEfiI2cMasterProtocolGuid,
                                NULL,
                                (VOID **)&I2cMasterTransmit
                                );

  if (EFI_ERROR (Status)) {
    return Status;
  }

  IpmiTransport2->Interface.Ipmb.InterfaceState      = IpmiInterfaceInitialized;
  IpmiTransport2->Interface.Ipmb.IpmbInterfaceApiPtr = (UINTN)I2cMasterTransmit;

  Status = CheckSelfTestByInterfaceType (
                                         IpmiTransport2,
                                         &BmcStatus,
                                         SysInterfaceIpmb
                                         );
  if (EFI_ERROR (Status) || (BmcStatus == BmcStatusHardFail)) {
    IpmiTransport2->Interface.Ipmb.InterfaceState = IpmiInterfaceInitError;
  }

  return Status;
}
