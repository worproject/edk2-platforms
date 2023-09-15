/** @file
  This file contains routines for GPIO

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#include "GpioLibrary.h"
#include <Register/PchPcrRegs.h>

/**
  This procedure will check if GpioGroup argument is correct and
  supplied DW reg number can be used for this group to access DW registers.
  Function will check below conditions:
   - Valid GpioGroup
   - DwNum is has valid value for this group

  @param[in] Group        GPIO group
  @param[in] DwNum        Register number for current group (parameter applicable in accessing whole register).
                          For group which has less then 32 pads per group DwNum must be 0.

  @retval TRUE             DW Reg number and GpioGroup is valid
  @retval FALSE            DW Reg number and GpioGroup is invalid
**/
STATIC
BOOLEAN
GpioIsGroupAndDwNumValid (
  IN GPIO_GROUP             Group,
  IN UINT32                 DwNum
  )
{
  UINT32                 GroupIndex;
  CONST GPIO_GROUP_INFO  *GpioGroupInfo;
  UINT32                 GpioGroupInfoLength;

  GpioGroupInfo = GpioGetGroupInfoTable (&GpioGroupInfoLength);

  GroupIndex = GpioGetGroupIndexFromGroup (Group);

  if ((Group < GpioGetLowestGroup ()) || (Group > GpioGetHighestGroup ()) || (GroupIndex >= GpioGroupInfoLength)) {
    DEBUG ((DEBUG_ERROR, "GPIO ERROR: Group argument (%d) is not within range of possible groups for this PCH\n", GroupIndex));
    goto Error;
  }

  //
  // Check if DwNum argument does not exceed number of DWord registers
  // resulting from available pads for certain group
  //
  if (DwNum > GPIO_GET_DW_NUM (GpioGroupInfo[GroupIndex].PadPerGroup - 1)){
    goto Error;
  }

  return TRUE;
Error:
  ASSERT (FALSE);
  return FALSE;
}

//
// Possible registers to be accessed using GpioReadReg()/GpioWriteReg() functions
//
typedef enum {
  GpioHostOwnershipRegister = 0,
  GpioGpeEnableRegister,
  GpioGpeStatusRegister,
  GpioSmiEnableRegister,
  GpioSmiStatusRegister,
  GpioNmiEnableRegister,
  GpioPadConfigLockRegister,
  GpioPadLockOutputRegister
} GPIO_REG;

/**
  This procedure will read GPIO register

  @param[in] RegType              GPIO register type
  @param[in] Group                GPIO group
  @param[in] DwNum                Register number for current group (parameter applicable in accessing whole register).
                                  For group which has less then 32 pads per group DwNum must be 0.
  @param[out] ReadVal             Read data

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_UNSUPPORTED         Feature is not supported for this group or pad
**/
STATIC
EFI_STATUS
GpioReadReg (
  IN GPIO_REG               RegType,
  IN GPIO_GROUP             Group,
  IN UINT32                 DwNum,
  OUT UINT32                *ReadVal
  )
{
  UINT32                 RegOffset;
  UINT32                 GroupIndex;
  CONST GPIO_GROUP_INFO  *GpioGroupInfo;
  UINT32                 GpioGroupInfoLength;

  RegOffset = NO_REGISTER_FOR_PROPERTY;
  GroupIndex = GpioGetGroupIndexFromGroup (Group);

  GpioGroupInfo = GpioGetGroupInfoTable (&GpioGroupInfoLength);

  switch (RegType) {
    case GpioHostOwnershipRegister:
      RegOffset = GpioGroupInfo[GroupIndex].HostOwnOffset;
      break;
    case GpioGpeEnableRegister:
      RegOffset = GpioGroupInfo[GroupIndex].GpiGpeEnOffset;
      break;
    case GpioGpeStatusRegister:
      RegOffset = GpioGroupInfo[GroupIndex].GpiGpeStsOffset;
      break;
    case GpioSmiEnableRegister:
      RegOffset = GpioGroupInfo[GroupIndex].SmiEnOffset;
      break;
    case GpioSmiStatusRegister:
      RegOffset = GpioGroupInfo[GroupIndex].SmiStsOffset;
      break;
    case GpioNmiEnableRegister:
      RegOffset = GpioGroupInfo[GroupIndex].NmiEnOffset;
      break;
    case GpioPadConfigLockRegister:
      RegOffset = GpioGroupInfo[GroupIndex].PadCfgLockOffset;
      break;
    case GpioPadLockOutputRegister:
      RegOffset = GpioGroupInfo[GroupIndex].PadCfgLockTxOffset;
      break;
    default:
      break;
  }

  //
  // Check if selected register exists
  //
  if (RegOffset == NO_REGISTER_FOR_PROPERTY) {
    return EFI_UNSUPPORTED;
  }

  //
  // If there are more then 32 pads per group then certain
  // group information would be split into more then one DWord register.
  //
  if ((RegType == GpioPadConfigLockRegister) || (RegType == GpioPadLockOutputRegister)) {
    //
    // PadConfigLock and OutputLock registers when used for group containing more than 32 pads
    // are not placed in a continuous way, e.g:
    // 0x0 - PadConfigLock_DW0
    // 0x4 - OutputLock_DW0
    // 0x8 - PadConfigLock_DW1
    // 0xC - OutputLock_DW1
    //
    RegOffset += DwNum * 0x8;
  } else {
    RegOffset += DwNum * 0x4;
  }

  *ReadVal = GpioRegisterAccessRead32 (&GpioGroupInfo[GroupIndex], RegOffset);

  return EFI_SUCCESS;
}

/**
  This procedure will write GPIO register

  @param[in] RegType              GPIO register type
  @param[in] Group                GPIO group
  @param[in] DwNum                Register number for current group (parameter applicable in accessing whole register).
                                  For group which has less then 32 pads per group DwNum must be 0.
  @param[in] RegAndMask           Mask which will be AND'ed with register value
  @param[in] RegOrMask            Mask which will be OR'ed with register value

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_UNSUPPORTED         Feature is not supported for this group or pad
**/
STATIC
EFI_STATUS
GpioWriteReg (
  IN GPIO_REG               RegType,
  IN GPIO_GROUP             Group,
  IN UINT32                 DwNum,
  IN UINT32                 RegAndMask,
  IN UINT32                 RegOrMask
  )
{
  UINT32                 RegOffset;
  UINT32                 GroupIndex;
  CONST GPIO_GROUP_INFO  *GpioGroupInfo;
  UINT32                 GpioGroupInfoLength;
  UINT32                 PadCfgLock;
  BOOLEAN                Lockable;

  Lockable = FALSE;
  PadCfgLock = 0;
  RegOffset = NO_REGISTER_FOR_PROPERTY;
  GroupIndex = GpioGetGroupIndexFromGroup (Group);

  GpioGroupInfo = GpioGetGroupInfoTable (&GpioGroupInfoLength);

  switch (RegType) {
    case GpioHostOwnershipRegister:
      RegOffset = GpioGroupInfo[GroupIndex].HostOwnOffset;
      break;
    case GpioGpeEnableRegister:
      RegOffset = GpioGroupInfo[GroupIndex].GpiGpeEnOffset;
      Lockable = TRUE;
      break;
    case GpioGpeStatusRegister:
      RegOffset = GpioGroupInfo[GroupIndex].GpiGpeStsOffset;
      break;
    case GpioSmiEnableRegister:
      RegOffset = GpioGroupInfo[GroupIndex].SmiEnOffset;
      Lockable = TRUE;
      break;
    case GpioSmiStatusRegister:
      RegOffset = GpioGroupInfo[GroupIndex].SmiStsOffset;
      break;
    case GpioNmiEnableRegister:
      RegOffset = GpioGroupInfo[GroupIndex].NmiEnOffset;
      Lockable = TRUE;
      break;
    case GpioPadConfigLockRegister:
    case GpioPadLockOutputRegister:
    default:
      break;
  }

  //
  // Check if selected register exists
  //
  if (RegOffset == NO_REGISTER_FOR_PROPERTY) {
    return EFI_UNSUPPORTED;
  }

  if (Lockable) {
    GpioGetPadCfgLockForGroupDw (Group, DwNum, &PadCfgLock);
    if (PadCfgLock) {
      //
      // Check if for pads which are going to be reconfigured lock is set.
      //
      if ((~RegAndMask | RegOrMask) & PadCfgLock) {
        //
        // Unlock all pads for this Group DW reg for simplicity
        // even if not all of those pads will have their settings reprogrammed
        //
        GpioUnlockPadCfgForGroupDw (Group, DwNum, PadCfgLock);
      } else {
        //
        // No need to perform an unlock as pads which are going to be reconfigured
        // are not in locked state
        //
        PadCfgLock = 0;
      }
    }
  }

  //
  // If there are more then 32 pads per group then certain
  // group information would be split into more then one DWord register.
  //
  RegOffset += DwNum * 0x4;

  GpioRegisterAccessAndThenOr32 (&GpioGroupInfo[GroupIndex], RegOffset, RegAndMask,RegOrMask);

  if (Lockable && PadCfgLock) {
    //
    // Lock previously unlocked pads
    //
    GpioLockPadCfgForGroupDw (Group, DwNum, PadCfgLock);
  }

  return EFI_SUCCESS;
}

/**
  This procedure will write GPIO Lock/LockTx register using SBI.

  @param[in] RegType              GPIO register (Lock or LockTx)
  @param[in] Group                GPIO group number
  @param[in] DwNum                Register number for current group.
                                  For group which has less then 32 pads per group DwNum must be 0.
  @param[in] LockRegAndMask       Mask which will be AND'ed with Lock register value
  @param[in] LockRegOrMask        Mask which will be Or'ed with Lock register value

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_UNSUPPORTED         Feature is not supported for this group or pad
**/
STATIC
EFI_STATUS
GpioWriteLockReg (
  IN GPIO_REG                  RegType,
  IN GPIO_GROUP                Group,
  IN UINT32                    DwNum,
  IN UINT32                    LockRegAndMask,
  IN UINT32                    LockRegOrMask
  )
{
  CONST GPIO_GROUP_INFO  *GpioGroupInfo;
  UINT32                 GpioGroupInfoLength;
  UINT32                 RegOffset;
  UINT32                 OldLockVal;
  UINT32                 NewLockVal;
  UINT32                 GroupIndex;

  OldLockVal = 0;
  NewLockVal = 0;

  RegOffset = NO_REGISTER_FOR_PROPERTY;
  GroupIndex = GpioGetGroupIndexFromGroup (Group);

  GpioGroupInfo = GpioGetGroupInfoTable (&GpioGroupInfoLength);

  switch (RegType) {
    case GpioPadConfigLockRegister:
      RegOffset = GpioGroupInfo[GroupIndex].PadCfgLockOffset;
      GpioGetPadCfgLockForGroupDw (Group, DwNum, &OldLockVal);
      break;
    case GpioPadLockOutputRegister:
      RegOffset = GpioGroupInfo[GroupIndex].PadCfgLockTxOffset;
      GpioGetPadCfgLockTxForGroupDw (Group, DwNum, &OldLockVal);
      break;
    default:
      break;
  }

  //
  // Check if selected register exists
  //
  if (RegOffset == NO_REGISTER_FOR_PROPERTY) {
    return EFI_UNSUPPORTED;
  }

  NewLockVal = (OldLockVal & LockRegAndMask) | LockRegOrMask;

  return GpioInternalWriteLockRegister (NewLockVal, RegOffset, DwNum, GpioGroupInfo, GroupIndex);
}

/**
  This procedure will get Gpio Pad Ownership

  @param[in] GpioPad              GPIO pad
  @param[out] PadOwnVal           Value of Pad Ownership

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioGetPadOwnership (
  IN  GPIO_PAD                GpioPad,
  OUT GPIO_PAD_OWN            *PadOwnVal
  )
{
  UINT32                 Mask;
  UINT32                 RegOffset;
  UINT32                 GroupIndex;
  UINT32                 PadNumber;
  CONST GPIO_GROUP_INFO  *GpioGroupInfo;
  UINT32                 GpioGroupInfoLength;
  UINT32                 PadOwnRegValue;

  if (!GpioIsPadValid (GpioPad)) {
    return EFI_INVALID_PARAMETER;
  }

  GroupIndex = GpioGetGroupIndexFromGpioPad (GpioPad);
  PadNumber = GpioGetPadNumberFromGpioPad (GpioPad);

  GpioGroupInfo = GpioGetGroupInfoTable (&GpioGroupInfoLength);

  //
  // Check if selected register exists
  //
  if (GpioGroupInfo[GroupIndex].PadOwnOffset == NO_REGISTER_FOR_PROPERTY) {
    *PadOwnVal = GpioPadOwnHost;
    return EFI_UNSUPPORTED;
  }
  //
  // Calculate RegOffset using Pad Ownership offset and GPIO Pad number.
  // One DWord register contains information for 8 pads.
  //
  RegOffset = GpioGroupInfo[GroupIndex].PadOwnOffset + (PadNumber >> 3) * 0x4;

  //
  // Calculate pad bit position within DWord register
  //
  PadNumber %= 8;
  Mask = (BIT1 | BIT0) << (PadNumber * 4);

  PadOwnRegValue = GpioRegisterAccessRead32 (&GpioGroupInfo[GroupIndex], RegOffset);

  *PadOwnVal = (GPIO_PAD_OWN) ((PadOwnRegValue & Mask) >> (PadNumber * 4));

  return EFI_SUCCESS;
}

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
  )
{
  if (!GpioIsGroupAndDwNumValid (Group, DwNum)) {
    return EFI_INVALID_PARAMETER;
  }

  return GpioReadReg (
           GpioPadConfigLockRegister,
           Group,
           DwNum,
           PadCfgLockRegVal
           );
}
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
  )
{
  if (!GpioIsGroupAndDwNumValid (Group, DwNum)) {
    return EFI_INVALID_PARAMETER;
  }

  return GpioReadReg (
           GpioPadLockOutputRegister,
           Group,
           DwNum,
           PadCfgLockTxRegVal
           );
}
/**
  This procedure will clear PadCfgLock for selected pads within one group.
  This function should be used only inside SMI.

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
  )
{
  if (!GpioIsGroupAndDwNumValid (Group, DwNum)) {
    return EFI_INVALID_PARAMETER;
  }

  return GpioWriteLockReg (
           GpioPadConfigLockRegister,
           Group,
           DwNum,
           ~PadsToUnlock,
           0
           );
}
/**
  This procedure will clear PadCfgLockTx for selected pads within one group.
  This function should be used only inside SMI.

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
  )
{
  if (!GpioIsGroupAndDwNumValid (Group, DwNum)) {
    return EFI_INVALID_PARAMETER;
  }

  return GpioWriteLockReg (
           GpioPadLockOutputRegister,
           Group,
           DwNum,
           ~PadsToUnlockTx,
           0
           );
}
