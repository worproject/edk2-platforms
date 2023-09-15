/** @file
  Header file for PchDmiLib.

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _PCH_DMI_LIB_H_
#define _PCH_DMI_LIB_H_

/**
  This function checks if DMI Secured Register Lock (SRL) is set

  @retval SRL state
**/
BOOLEAN
IsPchDmiLocked (
  VOID
  );

/**
  Get PCH TCO base address.

  @retval Address                   Address of TCO base address.
**/
UINT16
PchDmiGetTcoBase (
  VOID
  );

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
  );

/**
  Set PCH LPC/eSPI IO enable decoding in DMI

  @param[in] LpcIoEnableDecoding        LPC/eSPI IO enable decoding bit settings.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_UNSUPPORTED               DMIC.SRL is set.
**/
EFI_STATUS
PchDmiSetLpcIoEnable (
  IN  UINT16                            LpcIoEnableDecoding
  );
#endif // _PCH_DMI_LIB_H_
