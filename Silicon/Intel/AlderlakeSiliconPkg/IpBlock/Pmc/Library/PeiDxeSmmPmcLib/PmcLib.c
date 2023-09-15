/** @file
  PCH PMC Library.
  All function in this library is available for PEI, DXE, and SMM,
  But do not support UEFI RUNTIME environment call.

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PchCycleDecodingLib.h>
#include <Library/PmcLib.h>
#include <Library/PchPcrLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PmcPrivateLib.h>
#include <PchReservedResources.h>
#include <Register/PmcRegs.h>
#include <Register/PchRegs.h>

/**
  Get PCH ACPI base address.

  @retval Address                   Address of PWRM base address.
**/
UINT16
PmcGetAcpiBase (
  VOID
  )
{
  return PcdGet16 (PcdAcpiBaseAddress);
}

/**
  Get PCH PWRM base address.

  @retval Address                   Address of PWRM base address.
**/
UINT32
PmcGetPwrmBase (
  VOID
  )
{
  return PCH_PWRM_BASE_ADDRESS;
}
