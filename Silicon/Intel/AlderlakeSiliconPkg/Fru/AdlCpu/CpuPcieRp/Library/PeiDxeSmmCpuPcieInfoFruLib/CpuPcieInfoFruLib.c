/** @file
  CPU PCIe information library.

  All function in this library is available for PEI, DXE, and SMM,
  But do not support UEFI RUNTIME environment call.

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#include <Uefi/UefiBaseType.h>
#include <Library/CpuPlatformLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/PciSegmentLib.h>
#include <Register/SaRegsHostBridge.h>
#include <Library/CpuPcieInfoFruLib.h>
#include <CpuPcieInfo.h>

/**
  Get Maximum CPU Pcie Root Port Number

  @retval Maximum CPU Pcie Root Port Number
**/
UINT8
GetMaxCpuPciePortNum (
  VOID
  )
{
  switch (GetCpuSku ()) {
    case EnumCpuUlt:
      return CPU_PCIE_ULT_MAX_ROOT_PORT;
    case EnumCpuUlx:
      return CPU_PCIE_ULX_MAX_ROOT_PORT;
    default:
      return CPU_PCIE_ULT_MAX_ROOT_PORT;
  }
}

/**
  Get CPU Pcie Root Port Device and Function Number by Root Port physical Number

  @param[in]  RpNumber              Root port physical number. (0-based)
  @param[out] RpDev                 Return corresponding root port device number.
  @param[out] RpFun                 Return corresponding root port function number.

  @retval     EFI_SUCCESS           Root port device and function is retrieved
  @retval     EFI_INVALID_PARAMETER RpNumber is invalid
**/
EFI_STATUS
EFIAPI
GetCpuPcieRpDevFun (
  IN  UINTN   RpNumber,
  OUT UINTN   *RpDev,
  OUT UINTN   *RpFun
  )
{
  if (RpNumber > GetMaxCpuPciePortNum ()) {
    DEBUG ((DEBUG_ERROR, "GetCpuPcieRpDevFun invalid RpNumber %x", RpNumber));
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  switch (RpNumber) {
    case 0:
      *RpDev = 6;
      *RpFun = 0;
      break;
    case 1:
      *RpDev = 1;
      *RpFun = 0;
      break;
    case 2:
      if (GetCpuSku () == EnumCpuTrad) {
        *RpDev = 1;
        *RpFun = 1;
      } else {
        *RpDev = 6;
        *RpFun = 2;
      }
      break;
    default:
      *RpDev = 6;
      *RpFun = 0;
      break;
  }
  return EFI_SUCCESS;
}
