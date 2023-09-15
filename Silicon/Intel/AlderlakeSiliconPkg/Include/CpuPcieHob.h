/** @file
  The GUID definition for CpuPcieHob

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _CPU_PCIE_HOB_H_
#define _CPU_PCIE_HOB_H_

#include <Base.h>
#include <CpuPcieInfo.h>
#include <CpuPcieConfig.h>

extern EFI_GUID gCpuPcieHobGuid;
#pragma pack (push,1)


/**
  The CPU_PCIE_HOB block describes the expected configuration of the CpuPcie controllers
**/
typedef struct {
  ///
  /// These members describe the configuration of each CPU PCIe root port.
  ///
  EFI_HOB_GUID_TYPE           EfiHobGuidType;                           ///< Offset 0 - 23: GUID Hob type structure for gCpuPcieHobGuid
  CPU_PCIE_ROOT_PORT_CONFIG   RootPort[CPU_PCIE_MAX_ROOT_PORTS];
  UINT8                       L1SubStates[CPU_PCIE_MAX_ROOT_PORTS];  ///< The L1 Substates configuration of the root port

  UINT32                      DekelFwVersionMinor;                      ///< Dekel Firmware Minor Version
  UINT32                      DekelFwVersionMajor;                      ///< Dekel Firmware Major Version
  BOOLEAN                     InitPcieAspmAfterOprom;                   ///< 1=initialize PCIe ASPM after Oprom; 0=before (This will be set basing on policy)
  UINT32                      RpEnabledMask;                            ///< Rootport enabled mask based on DEVEN register
  UINT32                      RpEnMaskFromDevEn;                        ///< Rootport enabled mask based on Device Id
  UINT8                       DisableClkReqMsg[CPU_PCIE_MAX_ROOT_PORTS];     ///< 1=ClkReqMsg disabled, 0=ClkReqMsg enabled
  UINT8                       SlotSelection;                            ///< 1=M2 slot, 0=CEMx4 slot
  BOOLEAN                     ComplianceTest;                           ///< Compliance Test based on policy
  UINT32                      HsPhyRecipeVersionMajor;                  ///< HS-Phy Recipe Major Version
  UINT32                      HsPhyRecipeVersionMinor;                  ///< HS-Phy Recipe Minor Version
  UINT32                      HsPhyFwProdMajor;                         ///< HS-Phy Firmware Product Major Verison
  UINT32                      HsPhyFwProdMinor;                         ///< HS-Phy Firmware Product Minor Verison
  UINT32                      HsPhyFwHotFix;                            ///< HS-Phy Firmware Hot Fix Version
  UINT32                      HsPhyFwBuild;                             ///< HS-Phy Firmware Build version
  UINT32                      HsPhyFwEvBitProgMajor;                    ///< HS-Phy Firmware EV Bit Prog Major
  UINT32                      HsPhyFwEvBitProgMinor;                    ///< HS-Phy Firmware EV Bit Prog Minor
  UINT32                      HsPhyMap;                                 ///< HS-Phy Mapping Based on HS-Py supported ports
} CPU_PCIE_HOB;
#pragma pack (pop)
#endif
