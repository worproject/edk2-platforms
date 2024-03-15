/** @file
 *
 *  Copyright (c) 2024, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#ifndef __BCM2712_PCI_HOST_BRIDGE_H__
#define __BCM2712_PCI_HOST_BRIDGE_H__

#include <Protocol/Bcm2712PciePlatform.h>

typedef struct {
  EFI_PHYSICAL_ADDRESS    CpuBase;
  EFI_PHYSICAL_ADDRESS    BusBase;
  UINTN                   Size;
} BCM_PCIE_RC_WINDOW;

typedef struct {
  EFI_PHYSICAL_ADDRESS                Base;
  BCM_PCIE_RC_WINDOW                  Inbound;
  BCM_PCIE_RC_WINDOW                  Mem32;
  BCM_PCIE_RC_WINDOW                  Mem64;
  BCM2712_PCIE_CONTROLLER_SETTINGS    *Settings;
} BCM2712_PCIE_RC;

EFI_STATUS
PcieInitRc (
  IN BCM2712_PCIE_RC  *Pcie
  );

#endif // __BCM2712_PCI_HOST_BRIDGE_H__
