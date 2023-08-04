/** @file
  This library initialize Silicon Policy for PreMemory.

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#include <Ppi/SiPolicy.h>
#include <Ppi/PeiPreMemSiDefaultPolicy.h>
#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/SiPolicyLib.h>


/**
  Performs silicon pre-mem policy initialization.

  The returned data must be used as input data for SiliconPolicyDonePreMem (),
  and SiliconPolicyUpdateLib.SiliconPolicyUpdatePreMem ().

  @param[in, out] Policy       Pointer to policy.
  @return the initialized policy.
**/
VOID *
EFIAPI
SiliconPolicyInitPreMem (
  IN OUT VOID *Policy
  )
{
  EFI_STATUS                             Status;
  SI_PREMEM_POLICY_PPI                   *SiPreMemPolicyPpi;
  PEI_PREMEM_SI_DEFAULT_POLICY_INIT_PPI  *PeiPreMemSiDefaultPolicyInitPpi;

  DEBUG ((DEBUG_INFO, "Silicon PEI Policy Initialization Start in Pre-Memory...\n"));

  ASSERT (Policy == NULL);
  SiPreMemPolicyPpi = NULL;
  PeiPreMemSiDefaultPolicyInitPpi = NULL;

  //
  // Locate Policy init PPI to install default silicon policy
  //
  Status = PeiServicesLocatePpi (
             &gSiPreMemDefaultPolicyInitPpiGuid,
             0,
             NULL,
             (VOID **) &PeiPreMemSiDefaultPolicyInitPpi
             );
  ASSERT_EFI_ERROR (Status);
  if (PeiPreMemSiDefaultPolicyInitPpi != NULL) {
    Status = PeiPreMemSiDefaultPolicyInitPpi->PeiPreMemPolicyInit ();
    ASSERT_EFI_ERROR (Status);
    if (Status == EFI_SUCCESS) {
      Status = PeiServicesLocatePpi (
                 &gSiPreMemPolicyPpiGuid,
                 0,
                 NULL,
                 (VOID **) &SiPreMemPolicyPpi
                 );
      ASSERT_EFI_ERROR (Status);
    }
  }

  if (SiPreMemPolicyPpi == NULL) {
    DEBUG ((DEBUG_ERROR, "Fail to create default policy!\n"));
    return NULL;
  }

  return SiPreMemPolicyPpi;
}

/**
  The silicon pre-mem policy is finalized.
  Silicon code can do initialization based upon the policy data.

  The input Policy must be returned by SiliconPolicyInitPreMem().

  @param[in] Policy       Pointer to policy.
  @retval RETURN_SUCCESS The policy is handled consumed by silicon code.
**/
RETURN_STATUS
EFIAPI
SiliconPolicyDonePreMem (
  IN VOID *Policy
  )
{
  EFI_STATUS                   Status;
  //
  // Install PreMem Policy Ready PPI
  // While installs PreMemPolicyReadyPpi, RC assumes the Policy is ready and finalized. So please
  // update and override any setting before calling this function.
  //
  Status = SiPreMemInstallPolicyReadyPpi ();
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "Silicon PEI Policy Initialization Done in Pre-Memory\n"));

  return Status;
}
