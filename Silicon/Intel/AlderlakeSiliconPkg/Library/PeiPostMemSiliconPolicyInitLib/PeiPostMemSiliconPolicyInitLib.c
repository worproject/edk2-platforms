/** @file
  This library initialize Silicon Policy for PostMemory.

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#include <Ppi/SiPolicy.h>
#include <Ppi/PeiSiDefaultPolicy.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/SiPolicyLib.h>

/**
  Performs silicon post-mem policy initialization.

  The returned data must be used as input data for SiliconPolicyDonePostMem (),
  and SiliconPolicyUpdateLib.SiliconPolicyUpdatePostMem ().

  @param[in, out] Policy       Pointer to policy.
  @return the initialized policy.
**/
VOID *
EFIAPI
SiliconPolicyInitPostMem (
  IN OUT VOID *Policy
  )
{
  EFI_STATUS                        Status;
  SI_POLICY_PPI                     *SiPolicyPpi;
  PEI_SI_DEFAULT_POLICY_INIT_PPI    *PeiSiDefaultPolicyInitPpi;

  DEBUG ((DEBUG_INFO, "Silicon PEI Policy Initialization Start in Post-Memory...\n"));

  ASSERT (Policy == NULL);
  SiPolicyPpi = NULL;
  PeiSiDefaultPolicyInitPpi = NULL;

  //
  // Locate Policy init PPI to install default silicon policy
  //
  Status = PeiServicesLocatePpi (
             &gSiDefaultPolicyInitPpiGuid,
             0,
             NULL,
             (VOID **) &PeiSiDefaultPolicyInitPpi
             );
  ASSERT_EFI_ERROR (Status);
  if (PeiSiDefaultPolicyInitPpi != NULL) {
    Status = PeiSiDefaultPolicyInitPpi->PeiPolicyInit ();
    ASSERT_EFI_ERROR (Status);
    if (Status == EFI_SUCCESS) {
      Status = PeiServicesLocatePpi (
                 &gSiPolicyPpiGuid,
                 0,
                 NULL,
                 (VOID **) &SiPolicyPpi
                 );
      ASSERT_EFI_ERROR (Status);
    }
  }

  if (SiPolicyPpi == NULL) {
    DEBUG ((DEBUG_ERROR, "Fail to create default policy!\n"));
    return NULL;
  }


  return SiPolicyPpi;
}

/**
  The silicon post-mem policy is finalized.
  Silicon code can do initialization based upon the policy data.

  The input Policy must be returned by SiliconPolicyInitPostMem().

  @param[in] Policy       Pointer to policy.
  @retval RETURN_SUCCESS The policy is handled consumed by silicon code.
**/
RETURN_STATUS
EFIAPI
SiliconPolicyDonePostMem (
  IN VOID *Policy
  )
{
  EFI_STATUS                   Status;

  Status = SiInstallPolicyReadyPpi ();
  ASSERT_EFI_ERROR (Status);
  DEBUG ((DEBUG_INFO, "Silicon PEI Policy Initialization Done in Post-Memory\n"));

  return Status;
}
