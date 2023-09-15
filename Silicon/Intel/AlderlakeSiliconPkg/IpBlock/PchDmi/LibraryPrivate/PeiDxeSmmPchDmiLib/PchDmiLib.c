/** @file
  PCH DMI library.

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
#include <Register/PchPcrRegs.h>
#include <Register/PchDmiRegs.h>
#include <Register/PchRegsLpc.h>

#include "PchDmi14.h"

/**
  This function checks if DMI Secured Register Lock (SRL) is set

  @retval SRL state
**/
BOOLEAN
IsPchDmiLocked (
  VOID
  )
{
    return IsPchDmi14Locked ();
}

/**
  Get PCH TCO base address.

  @retval Address                   Address of TCO base address.
**/
UINT16
PchDmiGetTcoBase (
  VOID
  )
{
  //
  // Read "TCO Base Address" PCR[DMI] + 2778h[15:5]
  //
  return (PchPcrRead16 (PID_DMI, R_PCH_DMI_PCR_TCOBASE) & B_PCH_DMI_PCR_TCOBASE_TCOBA);
}

/**
  Set PCH LPC/eSPI IO decode ranges in DMI
  Please check EDS for detail of LPC/eSPI IO decode ranges bit definition.
  Bit  12: FDD range
  Bit 9:8: LPT range
  Bit 6:4: ComB range
  Bit 2:0: ComA range

  @param[in] LpcIoDecodeRanges          LPC/eSPI IO decode ranges bit settings.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_UNSUPPORTED               DMIC.SRL is set.
**/
EFI_STATUS
PchDmiSetLpcIoDecodeRanges (
  IN  UINT16                            LpcIoDecodeRanges
  )
{
  //
  // This cycle decoding is only allowed to set when DMI is not locked.
  //
  if (IsPchDmiLocked ()) {
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }

  //
  // program LPC I/O Decode Ranges, PCR[DMI] + 2770h[15:0] to the same value programmed in LPC/eSPI PCI offset 80h.
  //
  PchPcrWrite16 (PID_DMI, R_PCH_DMI_PCR_LPCIOD, LpcIoDecodeRanges);
  return EFI_SUCCESS;
}

/**
  Set PCH LPC/eSPI IO enable decoding in DMI

  @param[in] LpcIoEnableDecoding        LPC/eSPI IO enable decoding bit settings.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_UNSUPPORTED               DMIC.SRL is set.
**/
EFI_STATUS
PchDmiSetLpcIoEnable (
  IN  UINT16                            LpcIoEnableDecoding
  )
{
  //
  // This cycle decoding is only allowed to set when DMI is not locked.
  //
  if (IsPchDmiLocked ()) {
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }

  //
  // program LPC I/O Decode Ranges, PCR[DMI] + 2774h[15:0] to the same value programmed in LPC/eSPI PCI offset 82h.
  //
  PchPcrWrite16 (PID_DMI, R_PCH_DMI_PCR_LPCIOE, LpcIoEnableDecoding);
  return EFI_SUCCESS;
}
