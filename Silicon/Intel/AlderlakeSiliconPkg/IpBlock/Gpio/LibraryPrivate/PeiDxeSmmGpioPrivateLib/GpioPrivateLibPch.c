/** @file
  This file contains hard/physical/local (not virtual) GPIO information

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#include <Uefi/UefiBaseType.h>
#include <Library/GpioLib.h>
#include <Library/GpioPrivateLib.h>
#include <Library/GpioNativeLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/PchSbiAccessLib.h>
#include <Register/GpioRegs.h>
#include "GpioNativePrivateLibInternal.h"

/**
  This procedure calculates Pad Configuration Register DW offset

  @param[in] GpioPad                 GPIO pad
  @param[in] DwReg                   Index of the configuration register

  @retval DW Register offset
**/
UINT32
GpioGetGpioPadCfgAddressFromGpioPad (
  IN  GPIO_PAD                GpioPad,
  IN  UINT32                  DwReg
  )
{
  UINT32                 PadCfgRegAddress;
  CONST GPIO_GROUP_INFO  *GpioGroupInfo;
  UINT32                 GpioGroupInfoLength;
  UINT32                 GroupIndex;
  UINT32                 PadNumber;

  GroupIndex = GpioGetGroupIndexFromGpioPad (GpioPad);
  PadNumber = GpioGetPadNumberFromGpioPad (GpioPad);

  GpioGroupInfo = GpioGetGroupInfoTable (&GpioGroupInfoLength);

  //
  // Create Pad Configuration register offset
  //
  PadCfgRegAddress = GpioGroupInfo[GroupIndex].PadCfgOffset + DwReg * 4 + S_GPIO_PCR_PADCFG * PadNumber;

  return PadCfgRegAddress;
}

/**
  This procedure reads GPIO register

  @param[in] GpioGroupInfo           Pointer to GPIO group table info
  @param[in] Register                Register offset

  @retval Register value or "F"s in case of errors
**/
UINT32
GpioRegisterAccessRead32 (
  IN  CONST GPIO_GROUP_INFO   *GpioGroupInfo,
  IN  UINT32                  Register
  )
{
  return MmioRead32 (PCH_PCR_ADDRESS (GpioGroupInfo->Community, Register));
}

/**
  This procedure writes GPIO register

  @param[in] GpioGroupInfo           Pointer to GPIO group table info
  @param[in] Register                Register offset
  @param[in] AndValue                And value
  @param[in] OrValue                 Or value

  @retval EFI_SUCCESS                Operation completed successfully
**/
EFI_STATUS
GpioRegisterAccessAndThenOr32 (
  IN  CONST GPIO_GROUP_INFO   *GpioGroupInfo,
  IN  UINT32                  Register,
  IN  UINT32                  AndValue,
  IN  UINT32                  OrValue
  )
{
  MmioAndThenOr32 (
    PCH_PCR_ADDRESS (GpioGroupInfo->Community, Register),
    AndValue,
    OrValue
    );
  return EFI_SUCCESS;
}

/**
  This procedure will calculate PADCFG register value based on GpioConfig data

  @param[in]  GpioPad                   GPIO Pad
  @param[in]  GpioConfig                GPIO Configuration data
  @param[out] PadCfgDwReg               PADCFG DWx register value
  @param[out] PadCfgDwRegMask           Mask with PADCFG DWx register bits to be modified

  @retval Status
**/
EFI_STATUS
GpioPadCfgRegValueFromGpioConfig (
  IN  GPIO_PAD           GpioPad,
  IN  CONST GPIO_CONFIG  *GpioConfig,
  OUT UINT32             *PadCfgDwReg,
  OUT UINT32             *PadCfgDwRegMask
  )
{
  return GpioPadCfgRegValueFromGpioConfigHardGpio (GpioPad, GpioConfig, PadCfgDwReg, PadCfgDwRegMask);
}

/**
  This procedure will write GPIO Lock/LockTx register
  - For PCH SBI message is used.

  @param[in] RegValue             GPIO register (Lock or LockTx) value
  @param[in] RegOffset            GPIO register (Lock or LockTx) base offset
  @param[in] DwNum                Register number for current group.
                                  For group which has less then 32 pads per group DwNum must be 0.
  @param[in] GpioGroupInfo        Pointer to GPIO group table info
  @param[in] GroupIndex           GPIO group index in the GpioGroupInfo table

  @retval EFI_SUCCESS             The function completed successfully
          EFI_UNSUPPORTED         Feature is not supported for this group or pad
**/
EFI_STATUS
GpioInternalWriteLockRegister (
  IN UINT32                 RegValue,
  IN UINT32                 RegOffset,
  IN UINT32                 DwNum,
  IN CONST GPIO_GROUP_INFO  *GpioGroupInfo,
  IN UINT32                 GroupIndex
  )
{
  EFI_STATUS             Status;
  PCH_SBI_OPCODE         Opcode;
  UINT8                  Response;

  //
  // If there are more then 32 pads per group then certain
  // group information would be split into more then one DWord register.
  // PadConfigLock and OutputLock registers when used for group containing more than 32 pads
  // are not placed in a continuous way, e.g:
  // 0x0 - PadConfigLock_DW0
  // 0x4 - OutputLock_DW0
  // 0x8 - PadConfigLock_DW1
  // 0xC - OutputLock_DW1
  //
  RegOffset += DwNum * 0x8;

  if (IsGpioLockOpcodeSupported ()) {
    Opcode = GpioLockUnlock;
  } else {
    Opcode = PrivateControlWrite;
  }

  Status = PchSbiExecutionEx (
             GpioGroupInfo[GroupIndex].Community,
             RegOffset,
             Opcode,
             FALSE,
             0x000F,
             0x0000,
             0x0000,
             &RegValue,
             &Response
             );
  ASSERT_EFI_ERROR (Status);
  return Status;
}
