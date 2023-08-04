/** @file
  This function handle the register/unregister of PCH PCIe specific SMI events.

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#include "PchSmmHelpers.h"
#include <Register/PmcRegs.h>
#include <Register/PchPcieRpRegs.h>
#include <Library/CpuPcieInfoFruLib.h>
#include <CpuPcieInfo.h>
#include <Library/PchPcieRpLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PchPciBdfLib.h>

extern UINT32  mNumOfRootPorts;

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
  )
{
  if (RpIndex >= CpuRpIndex0 && RpIndex <= CpuRpIndex3) {
    GetCpuPcieRpDevFun ((RpIndex - CpuRpIndex0), RpDev, RpFun);
  } else {
    *RpDev = PchPcieRpDevNumber (RpIndex);
    *RpFun = PchPcieRpFuncNumber (RpIndex);
  }
}
