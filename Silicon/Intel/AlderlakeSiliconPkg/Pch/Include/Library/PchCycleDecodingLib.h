/** @file
  Header file for PchCycleDecodingLib.

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _PCH_CYCLE_DECODING_LIB_H_
#define _PCH_CYCLE_DECODING_LIB_H_

/**
  Get PCH TCO base address.

  @param[out] Address                   Address of TCO base address.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid pointer passed.
**/
EFI_STATUS
PchTcoBaseGet (
  OUT UINT16                            *Address
  );

/**
  Set PCH LPC IO decode ranges.
  Program LPC I/O Decode Ranges, PCR[DMI] + 2770h[15:0] to the same value programmed in LPC offset 80h.
  Please check EDS for detail of Lpc IO decode ranges bit definition.
  Bit  12: FDD range
  Bit 9:8: LPT range
  Bit 6:4: ComB range
  Bit 2:0: ComA range

  @param[in] LpcIoDecodeRanges          Lpc IO decode ranges bit settings.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_UNSUPPORTED               DMIC.SRL is set.
**/
EFI_STATUS
PchLpcIoDecodeRangesSet (
  IN  UINT16                            LpcIoDecodeRanges
  );

/**
  Set PCH LPC and eSPI CS0# IO enable decoding.
  Setup I/O Enables in DMI to the same value program in LPC/eSPI PCI offset 82h.
  Note: Bit[15:10] of the source decode register is Read-Only. The IO range indicated by the Enables field
  in LPC/eSPI PCI offset 82h[13:10] is always forwarded by DMI to subtractive agent for handling.
  Please check EDS for detail of LPC/eSPI IO decode ranges bit definition.

  @param[in] LpcIoEnableDecoding        LPC IO enable decoding bit settings.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_UNSUPPORTED               DMIC.SRL is set.
**/
EFI_STATUS
PchLpcIoEnableDecodingSet (
  IN  UINT16                            LpcIoEnableDecoding
  );

#endif // _PCH_CYCLE_DECODING_LIB_H_
