/** @file
  PEI/DXE/SMM library for reserved 64-bit MMIO space.

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PEI_DXE_SMM_RESERVE_MMIO_64_SIZE_LIB_H_
#define _PEI_DXE_SMM_RESERVE_MMIO_64_SIZE_LIB_H_

/**
  The function return the 64 bit MMIO size to reserve.

  @retval The 64-bit MMIO size
**/
UINT64
ReserveMmio64Size (
  VOID
  );

#endif // _PEI_DXE_SMM_RESERVE_MMIO_64_SIZE_LIB_H_
