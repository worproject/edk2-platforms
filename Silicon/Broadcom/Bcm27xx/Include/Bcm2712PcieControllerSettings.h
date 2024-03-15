/** @file
 *
 *  Copyright (c) 2024, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#ifndef __BCM2712_PCIE_CONTROLLER_SETTINGS_H__
#define __BCM2712_PCIE_CONTROLLER_SETTINGS_H__

//
// This may be used in VFR forms and variable store.
//

#pragma pack (1)
typedef struct {
  BOOLEAN    Enabled;
  UINT8      MaxLinkSpeed;
  BOOLEAN    AspmSupportL0s;
  BOOLEAN    AspmSupportL1;
  BOOLEAN    RcbMatchMps;
  UINT32     VdmToQosMap;
} BCM2712_PCIE_CONTROLLER_SETTINGS;
#pragma pack()

#endif // __BCM2712_PCIE_CONTROLLER_SETTINGS_H__
