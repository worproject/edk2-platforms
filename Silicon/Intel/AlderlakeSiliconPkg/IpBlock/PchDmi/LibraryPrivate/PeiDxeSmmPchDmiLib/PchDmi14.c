/** @file
  This file contains functions for PCH DMI SIP14

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PchPcrLib.h>
#include <Library/PchDmiLib.h>
#include <Library/PchCycleDecodingLib.h>
#include <Library/PchPcrLib.h>
#include <Library/PchInfoLib.h>
#include <Register/PchDmiRegs.h>
#include <Register/PchDmi14Regs.h>
#include <Register/PchPcrRegs.h>

/**
  This function checks if DMI SIP14 Secured Register Lock (SRL) is set

  @retval SRL state
**/
BOOLEAN
IsPchDmi14Locked (
  VOID
  )
{
  return ((PchPcrRead32 (PID_DMI, R_PCH_DMI14_PCR_DMIC) & B_PCH_DMI14_PCR_DMIC_SRL) != 0);
}
