/** @file
  This file is SampleCode for Intel PEI Platform Policy initialization in pre-memory.

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Library/PeiSiPolicyUpdateLib.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiServicesTablePointerLib.h>

VOID
EFIAPI
SiliconPolicyUpdatePreMemFirmwareConfig (
  VOID
  )
{
    UpdatePeiPchPolicyPreMem ();
    UpdatePeiSaPolicyPreMem ();
    UpdatePeiCpuPolicyPreMem ();
}


VOID *
EFIAPI
SiliconPolicyUpdatePreMem (
  IN OUT VOID *Policy
  )
{
  Policy = NULL;

  SiliconPolicyUpdatePreMemFirmwareConfig ();

  return Policy;
}
