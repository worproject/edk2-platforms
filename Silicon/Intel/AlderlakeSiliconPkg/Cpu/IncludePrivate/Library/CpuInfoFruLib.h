 /** @file
  This file contains Cpu Information for specific generation.

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _CPU_INFO_FRU_LIB_H_
#define _CPU_INFO_FRU_LIB_H_

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <CpuRegs.h>
#include <CpuGenInfo.h>
#include <Register/SaRegsHostBridge.h>
#include <Library/PciSegmentLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PcdLib.h>

///
/// Override table structure for cTDP and non-cTDP skus.
/// Non-cTDP parts would have '0' data for TDP level information.
///
typedef struct {
  UINTN  CpuIdentifier;
  UINT16 SkuPackageTdp;
  UINTN  MsrPowerLimit1;
  UINTN  MsrPowerLimit2;
  UINTN  CtdpUpPowerLimit1;
  UINTN  CtdpUpPowerLimit2;
  UINTN  CtdpNominalPowerLimit1;
  UINTN  CtdpNominalPowerLimit2;
  UINTN  CtdpDownPowerLimit1;
  UINTN  CtdpDownPowerLimit2;
  UINTN  MsrPowerLimit4;             /// PL4 value if FVM is enabled or system does not support FVM
  UINTN  MsrPowerLimit4DisableFvm;   /// PL4 value if FVM is supported but disabled.
} PPM_OVERRIDE_TABLE;

/**
  Return CPU Sku

  @param[in]  UINT32             CpuFamilyModel
  @param[in]  UINT16             CpuDid

  @retval     UINT8              CPU Sku
**/
UINT8
GetCpuSkuInfo (
  IN UINT32    CpuFamilyModel,
  IN UINT16    CpuDid
  );

/**
  This function returns the supported Physical Address Size

  @retval returns the supported Physical Address Size.
**/
UINT8
GetMaxPhysicalAddressSizeFru (
  VOID
  );

#endif // _CPU_INFO_FRU_LIB_H_
