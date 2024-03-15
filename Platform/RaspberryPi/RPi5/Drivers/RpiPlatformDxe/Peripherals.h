/** @file
 *
 *  Copyright (c) 2023-2024, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#ifndef __RPI_PLATFORM_PERIPHERALS_H__
#define __RPI_PLATFORM_PERIPHERALS_H__

#include <Bcm2712PcieControllerSettings.h>

#define PCIE1_SETTINGS_ENABLED_DEFAULT         TRUE
#define PCIE1_SETTINGS_MAX_LINK_SPEED_DEFAULT  2

#ifndef VFRCOMPILE
  #include <Protocol/Bcm2712PciePlatform.h>

EFI_STATUS
EFIAPI
SetupPeripherals (
  VOID
  );

VOID
EFIAPI
ApplyPeripheralVariables (
  VOID
  );

VOID
EFIAPI
SetupPeripheralVariables (
  VOID
  );

extern BCM2712_PCIE_PLATFORM_PROTOCOL  mPciePlatform;
#endif // VFRCOMPILE

#endif // __RPI_PLATFORM_PERIPHERALS_H__
