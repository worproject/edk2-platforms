/** @file
  Header file for GpioLib.
  All function in this library is available for PEI, DXE, and SMM

  @note: When GPIO pads are owned by ME Firmware, BIOS/host should not
         attempt to access these GPIO Pads registers, registers value
         returned in this case will be 0xFF.

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _GPIO_LIB_H_
#define _GPIO_LIB_H_

#include <Library/GpioConfig.h>

typedef struct {
  GPIO_PAD           GpioPad;
  GPIO_CONFIG        GpioConfig;
} GPIO_INIT_CONFIG;

/**
  This procedure will initialize multiple GPIO pins. Use GPIO_INIT_CONFIG structure.
  Structure contains fields that can be used to configure each pad.
  Pad not configured using GPIO_INIT_CONFIG will be left with hardware default values.
  Separate fields could be set to hardware default if it does not matter, except
  GpioPad and PadMode.
  Function will work in most efficient way if pads which belong to the same group are
  placed in adjacent records of the table.
  Although function can enable pads for Native mode, such programming is done
  by reference code when enabling related silicon feature.

  @param[in] NumberofItem               Number of GPIO pads to be updated
  @param[in] GpioInitTableAddress       GPIO initialization table

  @retval EFI_SUCCESS                   The function completed successfully
  @retval EFI_INVALID_PARAMETER         Invalid group or pad number
**/
EFI_STATUS
GpioConfigurePads (
  IN UINT32                    NumberOfItems,
  IN GPIO_INIT_CONFIG          *GpioInitTableAddress
  );

///
/// Possible values of Pad Ownership
/// If Pad is not under Host ownership then GPIO registers
/// are not accessible by host (e.g. BIOS) and reading them
/// will return 0xFFs.
///
typedef enum {
  GpioPadOwnHost = 0x0,
  GpioPadOwnCsme = 0x1,
  GpioPadOwnIsh  = 0x2,
} GPIO_PAD_OWN;

/**
  This procedure will get Gpio Pad Ownership

  @param[in] GpioPad              GPIO pad
  @param[out] PadOwnVal           Value of Pad Ownership

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid GpioPad
**/
EFI_STATUS
GpioGetPadOwnership (
  IN  GPIO_PAD                GpioPad,
  OUT GPIO_PAD_OWN            *PadOwnVal
  );

/**
  This procedure will check state of Pad Config Lock for pads within one group

  @param[in]  Group               GPIO group
  @param[in]  DwNum               PadCfgLock register number for current group.
                                  For group which has less then 32 pads per group DwNum must be 0.
  @param[out] PadCfgLockRegVal    Value of PadCfgLock register
                                  Bit position - PadNumber
                                  Bit value - 0: NotLocked, 1: Locked

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or DwNum parameter number
**/
EFI_STATUS
GpioGetPadCfgLockForGroupDw (
  IN  GPIO_GROUP                  Group,
  IN  UINT32                      DwNum,
  OUT UINT32                      *PadCfgLockRegVal
  );

/**
  This procedure will check state of Pad Config Tx Lock for pads within one group

  @param[in]  Group               GPIO group
  @param[in]  DwNum               PadCfgLockTx register number for current group.
                                  For group which has less then 32 pads per group DwNum must be 0.
  @param[out] PadCfgLockTxRegVal  Value of PadCfgLockTx register
                                  Bit position - PadNumber
                                  Bit value - 0: NotLockedTx, 1: LockedTx

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or DwNum parameter number
**/
EFI_STATUS
GpioGetPadCfgLockTxForGroupDw (
  IN  GPIO_GROUP                  Group,
  IN  UINT32                      DwNum,
  OUT UINT32                      *PadCfgLockTxRegVal
  );

/**
  This procedure will clear PadCfgLock for selected pads within one group.
  Unlocking a pad will cause an SMI (if enabled)

  @param[in]  Group               GPIO group
  @param[in]  DwNum               PadCfgLock register number for current group.
                                  For group which has less then 32 pads per group DwNum must be 0.
  @param[in]  PadsToUnlock        Bitmask for pads which are going to be unlocked,
                                  Bit position - PadNumber
                                  Bit value - 0: DoNotUnlock, 1: Unlock

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioUnlockPadCfgForGroupDw (
  IN GPIO_GROUP                Group,
  IN UINT32                    DwNum,
  IN UINT32                    PadsToUnlock
  );

/**
  This procedure will set PadCfgLock for selected pads within one group

  @param[in]  Group               GPIO group
  @param[in]  DwNum               PadCfgLock register number for current group.
                                  For group which has less then 32 pads per group DwNum must be 0.
  @param[in]  PadsToLock          Bitmask for pads which are going to be locked,
                                  Bit position - PadNumber
                                  Bit value - 0: DoNotLock, 1: Lock

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or DwNum parameter number
**/
EFI_STATUS
GpioLockPadCfgForGroupDw (
  IN GPIO_GROUP                   Group,
  IN UINT32                       DwNum,
  IN UINT32                       PadsToLock
  );

/**
  This procedure will clear PadCfgLockTx for selected pads within one group.
  Unlocking a pad will cause an SMI (if enabled)

  @param[in]  Group               GPIO group
  @param[in]  DwNum               PadCfgLockTx register number for current group.
                                  For group which has less then 32 pads per group DwNum must be 0.
  @param[in]  PadsToUnlockTx      Bitmask for pads which are going to be unlocked,
                                  Bit position - PadNumber
                                  Bit value - 0: DoNotUnLockTx, 1: LockTx

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioUnlockPadCfgTxForGroupDw (
  IN GPIO_GROUP                Group,
  IN UINT32                    DwNum,
  IN UINT32                    PadsToUnlockTx
  );

#endif // _GPIO_LIB_H_
