/** @file
  This file is SampleCode for Intel PEI Platform Policy initialization in post-memory.

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Library/PeiSiPolicyUpdateLib.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiServicesTablePointerLib.h>

VOID
EFIAPI
SiliconPolicyUpdatePostMemFirmwareConfig (
  VOID
  )
{
    //
    // Update and override all platform related and customized settings below.
    //
    UpdatePeiPchPolicy ();
    UpdatePeiSaPolicy ();
    UpdatePeiCpuPolicy ();
}

VOID *
EFIAPI
SiliconPolicyUpdatePostMem (
  IN OUT VOID *Policy
  )
{
  Policy = NULL;

  SiliconPolicyUpdatePostMemFirmwareConfig ();

  return Policy;
}
