/** @file
  This file is SampleCode of the library for Intel PCH PEI Policy initialization.

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "PeiPchPolicyUpdate.h"
#include <Guid/GlobalVariable.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PchPcieRpLib.h>
#include <Library/PeiSiPolicyUpdateLib.h>
#include <Library/SiPolicyLib.h>
#include <PolicyUpdateMacro.h>
#include <Pins/GpioPinsVer2Lp.h>
#include <PchDmiConfig.h>

VOID
UpdatePcieClockInfo (
  PCH_PCIE_RP_PREMEM_CONFIG  *PcieRpPreMemConfig,
  IN VOID                    *FspmUpd,
  UINTN                      Index,
  UINT64                     Data
  )
{
  PCD64_BLOB Pcd64;

  Pcd64.Blob = Data;
  DEBUG ((DEBUG_INFO, "UpdatePcieClockInfo ClkIndex %x ClkUsage %x, Supported %x\n", Index, Pcd64.PcieClock.ClockUsage, Pcd64.PcieClock.ClkReqSupported));

  UPDATE_POLICY (((FSPM_UPD *)FspmUpd)->FspmConfig.PcieClkSrcUsage[Index], PcieRpPreMemConfig->PcieClock[Index].Usage, (UINT8)Pcd64.PcieClock.ClockUsage);
  UPDATE_POLICY (((FSPM_UPD *)FspmUpd)->FspmConfig.PcieClkSrcClkReq[Index], PcieRpPreMemConfig->PcieClock[Index].ClkReq, Pcd64.PcieClock.ClkReqSupported ? (UINT8)Index : 0xFF);
}

/**
  Update PcieRp pre mem policies.

  @param[in] SiPreMemPolicy Pointer to SI_PREMEM_POLICY_PPI
  @param[in] FspsUpm        Pointer to FSPM_UPD
  @param[in] PchSetup       Pointer to PCH_SETUP
**/
STATIC
VOID
UpdatePcieRpPreMemPolicy (
  IN  SI_PREMEM_POLICY_PPI     *SiPreMemPolicy,
  IN  VOID                     *FspmUpd
  )
{
  UINT32                          RpIndex;
  UINT32                          RpEnabledMask;
  PCH_PCIE_RP_PREMEM_CONFIG       *PcieRpPreMemConfig;
  EFI_STATUS                      Status;

  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gPcieRpPreMemConfigGuid, (VOID *) &PcieRpPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  GET_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PcieRpEnableMask, PcieRpPreMemConfig->RpEnabledMask, RpEnabledMask);

  for (RpIndex = 0; RpIndex < GetPchMaxPciePortNum (); RpIndex ++) {
      RpEnabledMask |=  (UINT32) (1 << RpIndex);
  }
  // RpEnabledMask value is related with Setup value, Need to check Policy Default
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PcieRpEnableMask, PcieRpPreMemConfig->RpEnabledMask, RpEnabledMask);

  UpdatePcieClockInfo (PcieRpPreMemConfig, FspmUpd, 0, PcdGet64(PcdPcieClock0));
  UpdatePcieClockInfo (PcieRpPreMemConfig, FspmUpd, 1, PcdGet64(PcdPcieClock1));
  UpdatePcieClockInfo (PcieRpPreMemConfig, FspmUpd, 2, PcdGet64(PcdPcieClock2));
  UpdatePcieClockInfo (PcieRpPreMemConfig, FspmUpd, 3, PcdGet64(PcdPcieClock3));
  UpdatePcieClockInfo (PcieRpPreMemConfig, FspmUpd, 4, PcdGet64(PcdPcieClock4));
  UpdatePcieClockInfo (PcieRpPreMemConfig, FspmUpd, 5, PcdGet64(PcdPcieClock5));
  UpdatePcieClockInfo (PcieRpPreMemConfig, FspmUpd, 6, PcdGet64(PcdPcieClock6));
  UpdatePcieClockInfo (PcieRpPreMemConfig, FspmUpd, 7, PcdGet64(PcdPcieClock7));
  UpdatePcieClockInfo (PcieRpPreMemConfig, FspmUpd, 8, PcdGet64(PcdPcieClock8));
  UpdatePcieClockInfo (PcieRpPreMemConfig, FspmUpd, 9, PcdGet64(PcdPcieClock9));
  UpdatePcieClockInfo (PcieRpPreMemConfig, FspmUpd, 10, PcdGet64(PcdPcieClock10));
  UpdatePcieClockInfo (PcieRpPreMemConfig, FspmUpd, 11, PcdGet64(PcdPcieClock11));
  UpdatePcieClockInfo (PcieRpPreMemConfig, FspmUpd, 12, PcdGet64(PcdPcieClock12));
  UpdatePcieClockInfo (PcieRpPreMemConfig, FspmUpd, 13, PcdGet64(PcdPcieClock13));
  UpdatePcieClockInfo (PcieRpPreMemConfig, FspmUpd, 14, PcdGet64(PcdPcieClock14));
  UpdatePcieClockInfo (PcieRpPreMemConfig, FspmUpd, 15, PcdGet64(PcdPcieClock15));
  UpdatePcieClockInfo (PcieRpPreMemConfig, FspmUpd, 16, PcdGet64(PcdPcieClock16));
  UpdatePcieClockInfo (PcieRpPreMemConfig, FspmUpd, 17, PcdGet64(PcdPcieClock17));

}

/**
  This function performs PCH PEI Policy initialization.

  @retval EFI_SUCCESS             The PPI is installed and initialized.
  @retval EFI ERRORS              The PPI is not successfully installed.
  @retval EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver
**/
EFI_STATUS
EFIAPI
UpdatePeiPchPolicyPreMem (
  VOID
  )
{
  EFI_STATUS                      Status;
  VOID                            *FspmUpd;
  SI_PREMEM_POLICY_PPI            *SiPreMemPolicy;

  DEBUG ((DEBUG_INFO, "Update PeiPchPolicyUpdate Pre-Mem Start\n"));

  FspmUpd        = NULL;
  SiPreMemPolicy = NULL;

  Status = PeiServicesLocatePpi (&gSiPreMemPolicyPpiGuid, 0, NULL, (VOID **) &SiPreMemPolicy);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  UpdatePcieRpPreMemPolicy (SiPreMemPolicy, FspmUpd);
  return EFI_SUCCESS;
}
