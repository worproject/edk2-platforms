/** @file
  PCIe Config Block PreMem

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _PCIE_PREMEM_CONFIG_H_
#define _PCIE_PREMEM_CONFIG_H_


extern EFI_GUID gPciePreMemConfigGuid;

#pragma pack (push,1)

/**
  PCIe IMR Config
**/
typedef struct {
  UINT8     ImrEnabled;                        ///< PCIe IMR. <b>0: Disable</b>; 1: Enable.
  UINT8     ImrRpLocation;                     ///< 0: PCH_PCIe; 1: CPU_PCIe. If PCIeImrEnabled is TRUE then this will use to select the Root port location from PCH PCIe or CPU PCIe.Refer PCIE_IMR_ROOT_PORT_LOCATION above
  UINT16    ImrSize;                           ///< PCIe IMR size in megabytes
  UINT8     ImrRpSelection;                    ///< Index of root port that is selected for PCIe IMR (0 based)
  UINT8     Rsvd0[3];
} PCIE_IMR_CONFIG;

/**
  PCIe Pre-Memory Configuration
  <b>Revision 1</b>:  - Initial version.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;  ///< Offset 0 - 27 Config Block Header
  PCIE_IMR_CONFIG       PcieImr; ///< IMR Configuration
} PCIE_PREMEM_CONFIG;

#pragma pack (pop)
#endif // _PCIE_PREMEM_CONFIG_H_
