/** @file
  This file is PeiSiPolicyLib library creates default settings of RC
  Policy and installs RC Policy PPI.

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#include "PeiSiPolicyLibrary.h"
#include <Library/PcdLib.h>
#include <Library/PchInfoLib.h>


/**
  SiInstallPolicyReadyPpi installs SiPolicyReadyPpi.
  While installed, RC assumes the Policy is ready and finalized. So please update and override
  any setting before calling this function.

  @retval EFI_SUCCESS            The policy is installed.
  @retval EFI_OUT_OF_RESOURCES   Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
SiInstallPolicyReadyPpi (
  VOID
  )
{
  EFI_STATUS             Status;
  EFI_PEI_PPI_DESCRIPTOR *SiPolicyReadyPpiDesc;

  SiPolicyReadyPpiDesc = (EFI_PEI_PPI_DESCRIPTOR *) AllocateZeroPool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
  if (SiPolicyReadyPpiDesc == NULL) {
    ASSERT (FALSE);
    return EFI_OUT_OF_RESOURCES;
  }

  SiPolicyReadyPpiDesc->Flags = EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
  SiPolicyReadyPpiDesc->Guid  = &gSiPolicyReadyPpiGuid;
  SiPolicyReadyPpiDesc->Ppi   = NULL;

  //
  // Install Silicon Policy Ready PPI
  //
  Status = PeiServicesInstallPpi (SiPolicyReadyPpiDesc);
  ASSERT_EFI_ERROR (Status);
  return Status;
}

