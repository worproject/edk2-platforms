## @file
#  Alderlake P Pcie Clock configuration file.
#
#   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
#   SPDX-License-Identifier: BSD-2-Clause-Patent
#
##


[PcdsDynamicExVpd.common.SkuIdAdlPDdr5Rvp]
gBoardModuleTokenSpaceGuid.VpdPcdPcieClkUsageMap|*|{CODE(
{{
  PCIE_PEG,              // CPU M.2 SSD 1
  PCIE_PCH + 8,          // PCH M.2 SSD
  PCIE_PCH + 4,
  PCIE_PEG + 1,          // X8 DG/DG2
  PCIE_PEG + 2,          // CPU M.2 SSD 2
  PCIE_PCH + 5,          // M.2 KEY B WWAN - PCIe P6
  LAN_CLOCK,
  // Default Case:
  // - PCIe P7 mapped to GBELAN
  // - PCIe P8 mapped to x4 PCIe DT Slot (Pair 1)
  PCIE_PCH + 7,          // x4 PCIe DT Slot (x1)
  // Reworked Case: with rework and soft strap changes
  // - PCIe P7 mapped to x4 PCIe DT Slot (Pair 2)
  // - PCIe P8 mapped to x4 PCIe DT Slot (Pair 1)
  // PCIE_PCH + 6,       // x4 PCIe DT Slot (x2)
  NOT_USED,
  NOT_USED
}}
)}
