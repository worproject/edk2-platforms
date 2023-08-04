/** @file
  eSPI SMI Dispatch header

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PCH_SMI_HELPER_H_
#define _PCH_SMI_HELPER_H_
#include <Uefi/UefiBaseType.h>

/**
  Get CPU or PCH Pcie Root Port Device and Function Number by Root Port physical Number

  @param[in]  RpNumber              Root port physical number. (0-based)
  @param[out] RpDev                 Return corresponding root port device number.
  @param[out] RpFun                 Return corresponding root port function number.
**/
VOID
GetPcieRpDevFun (
  IN  UINTN   RpIndex,
  OUT UINTN   *RpDev,
  OUT UINTN   *RpFun
  );

/**
  Performs update of SmiDispatch descriptors with values that have to be evaluated during runtime.
**/
VOID
PchSmiDispatchUpdateDescriptors (
  VOID
  );

#endif
