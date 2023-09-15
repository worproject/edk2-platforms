/** @file
  Prototype of the SiPolicyLib library.

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _SI_POLICY_LIB_H_
#define _SI_POLICY_LIB_H_

#include <Ppi/SiPolicy.h>

/**
  SiPreMemInstallPolicyReadyPpi installs SiPreMemPolicyReadyPpi.
  While installed, RC assumes the Policy is ready and finalized. So please update and override
  any setting before calling this function.

  @retval EFI_SUCCESS            The policy is installed.
  @retval EFI_OUT_OF_RESOURCES   Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
SiPreMemInstallPolicyReadyPpi (
  VOID
  );

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
  );
#endif // _SI_POLICY_LIB_H_
