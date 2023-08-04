/** @file
  CPU Platform Lib implementation.

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#include "CpuPlatformLibrary.h"
#include <Library/PciSegmentLib.h>
#include <Register/SaRegsHostBridge.h>
#include <CpuRegs.h>
#include <Register/IgdRegs.h>
#include <Library/CpuInfoFruLib.h>
#include <Register/CommonMsr.h>
#include <CpuGenInfoFruLib.h>
#include <Pi/PiStatusCode.h>
#include <Library/ReportStatusCodeLib.h>

/**
  Return CPU Sku

  @retval UINT8              CPU Sku
**/
UINT8
EFIAPI
GetCpuSku (
  VOID
  )
{
  UINT16                  CpuDid;
  UINT32                  CpuFamilyModel;
  CPUID_VERSION_INFO_EAX  Eax;

  ///
  /// Read the CPUID & DID information
  ///
  AsmCpuid (CPUID_VERSION_INFO, &Eax.Uint32, NULL, NULL, NULL);
  CpuFamilyModel = Eax.Uint32 & CPUID_FULL_FAMILY_MODEL;
  CpuDid = PciSegmentRead16 (PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, R_SA_MC_DEVICE_ID));

  return GetCpuSkuInfo (CpuFamilyModel, CpuDid);

}

/**
  This function returns the supported Physical Address Size

  @retval supported Physical Address Size.
**/
UINT8
GetMaxPhysicalAddressSize (
  VOID
  )
{
  return GetMaxPhysicalAddressSizeFru ();
}
