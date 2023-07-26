/** @file PeiIpmbInterfaceLib.c
  IPMB Transport Pei phase implementation library functions.

  @copyright
  Copyright 2016 - 2021 Intel Corporation. <BR>
  Copyright (c) 1985 - 2023, American Megatrends International LLC. <BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/HobLib.h>
#include <Library/DebugLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/BmcCommonInterfaceLib.h>
#include <Library/IpmbInterfaceLib.h>
#include <Ppi/I2cMaster.h>

/*++

Routine Description:
  Send IPMI command through IPMB interface.

Arguments:
  Interface    - Pointer to System interface.
  SlaveAddress - I2C device slave address.
  RequestPacket - Pointer to an EFI_I2C_REQUEST_PACKET structure describing the I2C transaction.
Returns:
  EFI_STATUS - Status of the Send I2c command.
--*/
EFI_STATUS
IpmiI2cSendCommand (
  IN IPMI_SYSTEM_INTERFACE   *Interface,
  IN UINTN                   SlaveAddress,
  IN EFI_I2C_REQUEST_PACKET  *RequestPacket
  )
{
  EFI_STATUS              Status             = EFI_NOT_FOUND;
  EFI_PEI_I2C_MASTER_PPI  *I2cMasterTransmit = NULL;

  I2cMasterTransmit = (EFI_PEI_I2C_MASTER_PPI *)Interface->Ipmb.IpmbInterfaceApiPtr;

  if (I2cMasterTransmit != NULL) {
    Status = I2cMasterTransmit->StartRequest (
                                              I2cMasterTransmit,
                                              SlaveAddress,
                                              RequestPacket
                                              );
  }

  DEBUG ((DEBUG_INFO, "%a I2cMasterTransmit->StartRequest Status = %r\n", __func__, Status));

  return Status;
}

/*++

Routine Description:
  Locate I2c Ppi/Protocol instance and initialize interface pointer.

Arguments:
  IpmiTransport2    - Pointer to IPMI transport2 Instance.

Returns:
  EFI_STATUS - Status returned from functions used.
--*/
EFI_STATUS
IpmiGetI2cApiPtr (
  IN OUT IPMI_TRANSPORT2  *IpmiTransport2
  )
{
  EFI_STATUS              Status;
  CONST EFI_PEI_SERVICES  **PeiServices;
  EFI_PEI_I2C_MASTER_PPI  *I2cMasterTransmit = NULL;
  BMC_INTERFACE_STATUS    BmcStatus;

  PeiServices = GetPeiServicesTablePointer ();

  IpmiTransport2->Interface.Ipmb.IpmbInterfaceApiGuid = gEfiPeiI2cMasterPpiGuid;

  // Locate the I2C PPI for Communication.
  Status = (*PeiServices)->LocatePpi (
                                      PeiServices,
                                      &gEfiPeiI2cMasterPpiGuid,
                                      0,
                                      NULL,
                                      (VOID **)&I2cMasterTransmit
                                      );

  DEBUG ((DEBUG_INFO, "%a (*PeiServices)->LocatePpi gEfiPeiI2cMasterPpiGuid Status = %r\n", __func__, Status));

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
