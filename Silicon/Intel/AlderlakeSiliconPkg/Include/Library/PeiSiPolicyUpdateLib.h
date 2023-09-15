/** @file
  Header file for PEI SiPolicyUpdate Library.

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PEI_SI_POLICY_UPDATE_LIB_H_
#define _PEI_SI_POLICY_UPDATE_LIB_H_

#include <Ppi/SiPolicy.h>

/**
  This function performs CPU PEI Policy initialization in Post-memory.

  @retval EFI_SUCCESS             The PPI is installed and initialized.
  @retval EFI ERRORS              The PPI is not successfully installed.
  @retval EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver
**/
EFI_STATUS
EFIAPI
UpdatePeiCpuPolicy (
  VOID
  );

/**
  This function performs CPU PEI Policy initialization in PreMem.

  @retval EFI_SUCCESS             The PPI is installed and initialized.
  @retval EFI ERRORS              The PPI is not successfully installed.
  @retval EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver
**/
EFI_STATUS
EFIAPI
UpdatePeiCpuPolicyPreMem (
  VOID
  );


/**
  This function performs SA PEI Policy initialization.

  @retval EFI_SUCCESS             The PPI is installed and initialized.
**/
EFI_STATUS
EFIAPI
UpdatePeiSaPolicy (
  VOID
  );

/**
  This function performs SA PEI Policy initialization for PreMem.

  @retval EFI_SUCCESS             Update complete.
**/
EFI_STATUS
EFIAPI
UpdatePeiSaPolicyPreMem (
  VOID
  );

/**
  This function performs PCH PEI Policy initialization.

  @retval EFI_SUCCESS             The PPI is installed and initialized.
  @retval EFI ERRORS              The PPI is not successfully installed.
  @retval EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver
**/
EFI_STATUS
EFIAPI
UpdatePeiPchPolicy (
  VOID
  );

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
  );

/**
  Update the ME Policy Library

  @retval EFI_SUCCESS             Update complete.
  @retval Others                  Update unsuccessful.
**/
EFI_STATUS
EFIAPI
UpdatePeiMePolicy (
  VOID
  );

/**
  Update the ME Policy Library

  @retval EFI_SUCCESS            Update complete.
**/
EFI_STATUS
EFIAPI
UpdatePeiMePolicyPreMem (
  VOID
  );



/**
  Update the TBT Policy Library

  @retval EFI_SUCCESS            Update complete.
**/
EFI_STATUS
EFIAPI
UpdatePeiTbtPolicy (
  VOID
  );

/**
  Update the ME Server Policy Ppi (pre mem)

  @param[in, out] SiPreMemPolicyPpi   PEI Pre Mem Si Policy

  @retval EFI_SUCCESS           Initialization complete.
  @retval EFI_UNSUPPORTED       The chipset is unsupported by this driver.
  @retval EFI_OUT_OF_RESOURCES  Do not have enough resources to initialize the driver.
  @retval EFI_DEVICE_ERROR      Device error, driver exits abnormally.
  @retval EFI_INVALID_PARAMETER Wrong pointer passed to the function
**/
EFI_STATUS
UpdatePeiMeServerPreMemPolicy (
  IN OUT  SI_PREMEM_POLICY_PPI   *SiPreMemPolicy
  );

/**
  Update the ME Server Policy Ppi (post mem)

  @param[in, out] SiPolicyPpi   PEI Si Policy

  @retval EFI_SUCCESS           Initialization complete.
  @retval EFI_UNSUPPORTED       The chipset is unsupported by this driver.
  @retval EFI_DEVICE_ERROR      Device error, driver exits abnormally.
  @retval EFI_INVALID_PARAMETER Wrong pointer passed to the function
**/
 EFI_STATUS
UpdatePeiMeServerPolicy (
  IN OUT  SI_POLICY_PPI *SiPolicyPpi
  );
#endif
