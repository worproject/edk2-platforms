/** @file
  Header file for CpuPlatform Lib.

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _CPU_PLATFORM_LIB_H_
#define _CPU_PLATFORM_LIB_H_

#include <Uefi.h>
#include <CpuRegs.h>
#include <CpuGenInfo.h>

///
/// Table to convert Seconds into equivalent MSR values
/// This table is used for PL1, Pl2 and RATL TDP Time Window programming
///
extern GLOBAL_REMOVE_IF_UNREFERENCED UINT16 mSecondsToMsrValueMapTable[][2];

/**
  Return CPU Sku

  @retval UINT8              CPU Sku
**/
UINT8
EFIAPI
GetCpuSku (
  VOID
  );

/**
  This function returns the supported Physical Address Size

  @retval supported Physical Address Size.
**/
UINT8
GetMaxPhysicalAddressSize (
  VOID
  );

#endif
