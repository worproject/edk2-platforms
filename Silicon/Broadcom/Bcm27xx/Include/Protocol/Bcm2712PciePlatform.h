/** @file
 *
 *  Copyright (c) 2024, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#ifndef __BCM2712_PCIE_PLATFORM_H__
#define __BCM2712_PCIE_PLATFORM_H__

#include <IndustryStandard/Bcm2712.h>
#include <Bcm2712PcieControllerSettings.h>

#define BCM2712_PCIE_PLATFORM_PROTOCOL_GUID \
  { 0xf411a098, 0xc82b, 0x4920, { 0x90, 0x07, 0xca, 0x70, 0xb3, 0x65, 0x33, 0x89 } }

typedef struct _BCM2712_PCIE_PLATFORM_PROTOCOL BCM2712_PCIE_PLATFORM_PROTOCOL;

struct _BCM2712_PCIE_PLATFORM_PROTOCOL {
  EFI_PHYSICAL_ADDRESS                Mem32BusBase;
  UINTN                               Mem32Size;
  BCM2712_PCIE_CONTROLLER_SETTINGS    Settings[BCM2712_BRCMSTB_PCIE_COUNT];
};

extern EFI_GUID  gBcm2712PciePlatformProtocolGuid;

#endif // __BCM2712_PCIE_PLATFORM_H__
