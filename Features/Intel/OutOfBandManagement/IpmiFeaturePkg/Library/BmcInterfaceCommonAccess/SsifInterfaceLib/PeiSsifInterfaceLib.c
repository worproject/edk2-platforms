/** @file PeiSsifInterfaceLib.c
  SSIF Transport Pei phase Implementation library functions.

  @copyright
  Copyright 2016 - 2021 Intel Corporation. <BR>
  Copyright (c) 1985 - 2023, American Megatrends International LLC. <BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/HobLib.h>
#include <Library/DebugLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/BmcCommonInterfaceLib.h>
#include <Library/SsifInterfaceLib.h>
#include <Ppi/Smbus2.h>

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
  )
{
  EFI_STATUS          Status;
  EFI_PEI_SMBUS2_PPI  *EfiPeiSmbus2Ppi;

  Status          = EFI_NOT_FOUND;
  EfiPeiSmbus2Ppi = (EFI_PEI_SMBUS2_PPI *)Interface->Ssif.SsifInterfaceApiPtr;

  if (EfiPeiSmbus2Ppi != NULL) {
    Status = EfiPeiSmbus2Ppi->Execute (
                                       EfiPeiSmbus2Ppi,
                                       SlaveAddress,
                                       Command,
                                       Operation,
                                       PecCheck,
                                       Length,
                                       Buffer
                                       );
  }

  DEBUG ((DEBUG_INFO, "%a EfiPeiSmbus2Ppi->Execute Status = %r\n", __func__, Status));
  return Status;
}

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
  )
{
  EFI_STATUS              Status;
  UINTN                   Instance;
  CONST EFI_PEI_SERVICES  **PeiServices;
  EFI_PEI_SMBUS2_PPI      *EfiPeiSmbus2Ppi;
  BMC_INTERFACE_STATUS    BmcStatus;

  PeiServices = GetPeiServicesTablePointer ();

  IpmiTransport2->Interface.Ssif.SsifInterfaceApiGuid = gEfiPeiSmbus2PpiGuid;

  // Traverse all Smbus2 PPI instances and find the right instance for SSIF.
  for (Instance = 0; ; Instance++) {
    // Locate the Smbus Ppi.
    Status = (*PeiServices)->LocatePpi (
                                        PeiServices,
                                        &gEfiPeiSmbus2PpiGuid,
                                        Instance,
                                        NULL,
                                        (VOID **)&EfiPeiSmbus2Ppi
                                        );
    if (EFI_ERROR (Status)) {
      break;
    }

    IpmiTransport2->Interface.Ssif.InterfaceState      = IpmiInterfaceInitialized;
    IpmiTransport2->Interface.Ssif.SsifInterfaceApiPtr = (UINTN)EfiPeiSmbus2Ppi;

    Status = CheckSelfTestByInterfaceType (
                                           IpmiTransport2,
                                           &BmcStatus,
                                           SysInterfaceSsif
                                           );
    if (EFI_ERROR (Status) || (BmcStatus == BmcStatusHardFail)) {
      IpmiTransport2->Interface.Ssif.InterfaceState = IpmiInterfaceInitError;
      continue;
    }

    GetSystemInterfaceCapability (IpmiTransport2);
    GetGlobalEnables (IpmiTransport2);
    break;
  }

  return Status;
}
