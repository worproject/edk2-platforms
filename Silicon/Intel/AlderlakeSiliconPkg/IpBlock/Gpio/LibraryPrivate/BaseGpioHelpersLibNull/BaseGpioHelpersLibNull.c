/** @file
  This file contains NULL implementation for GPIO Helpers Lib

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#include <Uefi/UefiBaseType.h>
#include <Library/GpioConfig.h>

/**
  This procedure stores GPIO group data about pads which PadConfig needs to be unlocked.

  @param[in]  GroupIndex          GPIO group index
  @param[in]  DwNum               DWORD index for a group.
                                  For group which has less then 32 pads per group DwNum must be 0.
  @param[in]  UnlockedPads        DWORD bitmask for pads which are going to be left unlocked
                                  Bit position - PadNumber
                                  Bit value - 0: Skip, 1: Leave unlocked

  @retval Status
**/
EFI_STATUS
GpioStoreGroupDwUnlockPadConfigData (
  IN UINT32                       GroupIndex,
  IN UINT32                       DwNum,
  IN UINT32                       UnlockedPads
  )
{
  return EFI_SUCCESS;
}

/**
  This procedure stores GPIO group data about pads which Output state needs to be unlocked.

  @param[in]  GroupIndex          GPIO group index
  @param[in]  DwNum               DWORD index for a group.
                                  For group which has less then 32 pads per group DwNum must be 0.
  @param[in]  UnlockedPads        DWORD bitmask for pads which are going to be left unlocked
                                  Bit position - PadNumber
                                  Bit value - 0: Skip, 1: Leave unlocked
  @retval Status
**/
EFI_STATUS
GpioStoreGroupDwUnlockOutputData (
  IN UINT32                       GroupIndex,
  IN UINT32                       DwNum,
  IN UINT32                       UnlockedPads
  )
{
  return EFI_SUCCESS;
}
