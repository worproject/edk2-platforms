/** @file
  Header file for GpioPrivateLib.
  All function in this library is available for PEI, DXE, and SMM,

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _GPIO_PRIVATE_LIB_H_
#define _GPIO_PRIVATE_LIB_H_

#include <Uefi/UefiBaseType.h>
#include <Library/GpioConfig.h>
#include <Library/PchPcrLib.h>

/**
  GPIO Standby State configuration
  Standby State options for GPIO Pads
**/
typedef enum {
  GpioIosStateDefault         = 0x0,
  GpioIosStateLatchLastValue  = (0x0 << 1) | 0x01,  ///< Latch last value driven on TX, TX Enable and RX Enable
  GpioIosStateTx0Rx0RxDis     = (0x1 << 1) | 0x01,  ///< TX: 0, RX: 0 (internally), RX disabled
  GpioIosStateTx0Rx1RxDis     = (0x2 << 1) | 0x01,  ///< TX: 0, RX: 1 (internally), RX disabled
  GpioIosStateTx1Rx0RxDis     = (0x3 << 1) | 0x01,  ///< TX: 1, RX: 0 (internally), RX disabled
  GpioIosStateTx1Rx1RxDis     = (0x4 << 1) | 0x01,  ///< TX: 1, RX: 1 (internally), RX disabled
  GpioIosStateTx0RxEn         = (0x5 << 1) | 0x01,  ///< TX: 0, RX enabled
  GpioIosStateTx1RxEn         = (0x6 << 1) | 0x01,  ///< TX: 1, RX enabled
  GpioIosStateHizRx0          = (0x7 << 1) | 0x01,  ///< Hi-Z, RX: 0 (internally)
  GpioIosStateHizRx1          = (0x8 << 1) | 0x01,  ///< Hi-Z, RX: 1 (internally)
  GpioIosStateTxDisRxEn       = (0x9 << 1) | 0x01,  ///< TX Disabled and RX Enabled (i.e. wake or interrupt)
  GpioIosStateMasked          = (0xF << 1) | 0x01   ///< IO Standby signal is masked for this pad. In this mode, a pad operates as if IOStandby has not been asserted.
} GPIO_IOSTANDBY_STATE;

/**
  GPIO Standby Term configuration
  Standby Termination options for GPIO Pads
**/
typedef enum {
  GpioIosTermDefault         = 0x00,
  GpioIosTermSame            = (0x00 << 1) | 0x01, ///< Same as state specified in Term
  GpioIosTermPuDisPdDis      = (0x01 << 1) | 0x01, ///< Disable Pullup and Pulldown
  GpioIosTermPuDisPdEn       = (0x02 << 1) | 0x01, ///< Enable Pulldown
  GpioIosTermPuEnPdDis       = (0x03 << 1) | 0x01  ///< Enable Pullup
} GPIO_IOSTANDBY_TERM;

//
// Structure for native pin data
//
typedef struct {
  GPIO_PAD              Pad;
  GPIO_PAD_MODE         Mode;
  GPIO_IOSTANDBY_STATE  IosState;
  GPIO_IOSTANDBY_TERM   IosTerm;
} GPIO_PAD_NATIVE_FUNCTION;

//
// Structure for Serial GPIO pin definition
//
typedef struct {
  GPIO_PAD_NATIVE_FUNCTION  Sclock;
  GPIO_PAD_NATIVE_FUNCTION  Sload;
  GPIO_PAD_NATIVE_FUNCTION  Sdataout;
} SGPIO_PINS;

//
// Structure for USB Virtual Wire OverCurrent Pad Mode group
//
typedef struct {
  GPIO_PAD       OcRxPad;
  GPIO_PAD       OcTxPad;
} GPIO_VWOC_FUNCTION;

//
// Below defines are based on GPIO_CONFIG structure fields
//
#define B_GPIO_PAD_MODE_MASK                            0xF
#define N_GPIO_PAD_MODE_BIT_POS                         0
#define B_GPIO_DIRECTION_DIR_MASK                       0x7
#define N_GPIO_DIRECTION_DIR_BIT_POS                    0
#define B_GPIO_DIRECTION_INV_MASK                       0x18
#define N_GPIO_DIRECTION_INV_BIT_POS                    3
#define B_GPIO_OUTPUT_MASK                              0x3
#define N_GPIO_OUTPUT_BIT_POS                           0
#define N_GPIO_INT_CONFIG_INT_SOURCE_BIT_POS            0
#define N_GPIO_INT_CONFIG_INT_TYPE_BIT_POS              5
#define N_GPIO_ELECTRICAL_CONFIG_TERMINATION_BIT_POS    0
#define N_GPIO_OTHER_CONFIG_RXRAW_BIT_POS               0

//
// Structure for storing information about registers offset, community,
// maximal pad number for available groups
//
typedef struct {
  PCH_SBI_PID  Community;
  UINT16       PadOwnOffset;
  UINT16       HostOwnOffset;
  UINT16       GpiIsOffset;
  UINT16       GpiIeOffset;
  UINT16       GpiGpeStsOffset;
  UINT16       GpiGpeEnOffset;
  UINT16       SmiStsOffset;
  UINT16       SmiEnOffset;
  UINT16       NmiStsOffset;
  UINT16       NmiEnOffset;
  UINT16       PadCfgLockOffset;
  UINT16       PadCfgLockTxOffset;
  UINT16       PadCfgOffset;
  UINT16       PadPerGroup;
} GPIO_GROUP_INFO;

//
// If in GPIO_GROUP_INFO structure certain register doesn't exist
// it will have value equal to NO_REGISTER_FOR_PROPERTY
//
#define NO_REGISTER_FOR_PROPERTY 0xFFFF

#define GPIO_PAD_DEF(Group,Pad)                (UINT32)(((Group) << 16) + (Pad))
#define GPIO_GROUP_DEF(GroupIndex,ChipsetId)   ((GroupIndex) | ((ChipsetId) << 8))
#define GPIO_GET_GROUP_INDEX(Group)            ((Group) & 0x1F)
#define GPIO_GET_GROUP_FROM_PAD(GpioPad)       (((GpioPad) & 0x0F1F0000) >> 16)
#define GPIO_GET_GROUP_INDEX_FROM_PAD(GpioPad) GPIO_GET_GROUP_INDEX (GPIO_GET_GROUP_FROM_PAD(GpioPad))
#define GPIO_GET_PAD_NUMBER(GpioPad)           ((GpioPad) & 0x1FF)
#define GPIO_GET_CHIPSET_ID(GpioPad)           (((GpioPad) >> 24) & 0xF)

#define GPIO_GET_PAD_POSITION(PadNumber)       ((PadNumber) % 32)
#define GPIO_GET_DW_NUM(PadNumber)             ((PadNumber) / 32u)

/**
  This procedure will retrieve address and length of GPIO info table

  @param[out]  GpioGroupInfoTableLength   Length of GPIO group table

  @retval Pointer to GPIO group table
**/
CONST GPIO_GROUP_INFO*
GpioGetGroupInfoTable (
  OUT UINT32              *GpioGroupInfoTableLength
  );

typedef struct {
  CONST CHAR8*    GpioGroupPrefix;
  CONST GPIO_PAD  FirstUniqueGpio;
  CONST CHAR8**   GroupUniqueNames;
  CONST UINT32    UniqueNamesTableSize;
} GPIO_GROUP_NAME_INFO;

//
// Helper macros for initializing GPIO_GROUP_NAME_INFO structures
//
#define GPIO_GROUP_NAME(GroupName,FirstUniqueGpio,GroupUniqueNamesTable) \
  {GroupName, FirstUniqueGpio, GroupUniqueNamesTable, ARRAY_SIZE (GroupUniqueNamesTable)}

#define GPIO_GROUP_NAME_BASIC(GroupName) \
  {GroupName, 0, NULL, 0}

/**
  Get GPIO Chipset ID specific to PCH generation and series
**/
UINT32
GpioGetThisChipsetId (
  VOID
  );

/**
  This procedure is used to check if GpioPad is valid for certain chipset

  @param[in]  GpioPad             GPIO pad

  @retval TRUE                    This pin is valid on this chipset
          FALSE                   Incorrect pin
**/
BOOLEAN
GpioIsCorrectPadForThisChipset (
  IN  GPIO_PAD        GpioPad
  );


/**
  This procedure is used by PchSmiDispatcher and will return information
  needed to register GPI SMI.

  @param[in]  Index                   GPI SMI number
  @param[out] GpioPin                 GPIO pin
  @param[out] GpiSmiBitOffset         GPI SMI bit position within GpiSmi Registers
  @param[out] GpiHostSwOwnRegAddress  Address of HOSTSW_OWN register
  @param[out] GpiSmiStsRegAddress     Address of GPI SMI status register

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioGetPadAndSmiRegs (
  IN UINT32            Index,
  OUT GPIO_PAD         *GpioPin,
  OUT UINT8            *GpiSmiBitOffset,
  OUT UINT32           *GpiHostSwOwnRegAddress,
  OUT UINT32           *GpiSmiStsRegAddress
  );

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
  );


/**
  This procedure will check if GpioPad argument is valid.
  Function will check below conditions:
   - GpioPad represents a pad for current PCH
   - GpioPad belongs to valid GpioGroup
   - GPIO PadNumber is not greater than number of pads for this group

  @param[in] GpioPad       GPIO pad

  @retval TRUE             GPIO pad is valid and can be used with GPIO lib API
  @retval FALSE            GPIO pad is invalid and cannot be used with GPIO lib API
**/
BOOLEAN
GpioIsPadValid (
  IN GPIO_PAD             GpioPad
  );

/**
  This procedure will read GPIO Pad Configuration register

  @param[in] GpioPad          GPIO pad
  @param[in] DwReg            Choose PADCFG register: 0:DW0, 1:DW1

  @retval PadCfgRegValue      PADCFG_DWx value
**/
UINT32
GpioReadPadCfgReg (
  IN GPIO_PAD             GpioPad,
  IN UINT8                DwReg
  );

/**
  Check if 0x13 opcode supported for writing to GPIO lock unlock register

  @retval TRUE                It's supported
  @retval FALSE               It's not supported
**/
BOOLEAN
IsGpioLockOpcodeSupported (
  VOID
  );

/**
  Gpio Minimum Set

  Set of Gpio Minimum function to use in Pre Mem phase.
  To optimise execution and reduce memory footprint thse minimum version
  of 'full' functions are stripped from:
    - GpioPad PCH validation
    - GpioPad Group belonging validation
    - GpioPad Host ownership validation
    - IoStandbyState configuration
  The use of below functions has to be careful and with full
  understanding of all pros and cons. Please refer to GpioPrivateLib.c
  to familiarize with details of implementation.
**/

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
  );

/**
  This procedure writes GPIO register

  @param[in] GpioGroupInfo           Pointer to GPIO group table info
  @param[in] Register                Register offset
  @param[in] AndValue                And value
  @param[in] OrValue                 Or value

  @retval EFI_DEVICE_ERROR           vGPIO BAR not programmed
          EFI_SUCCESS                Operation completed successfully
**/
EFI_STATUS
GpioRegisterAccessAndThenOr32 (
  IN  CONST GPIO_GROUP_INFO   *GpioGroupInfo,
  IN  UINT32                  Register,
  IN  UINT32                  AndValue,
  IN  UINT32                  OrValue
  );

/**
  This procedure will calculate PADCFG register value based on GpioConfig data
  The procedure can be various depending on chipset generation.
  Available configuration options and corresponding registers fields
  can distributed in different way in configuration registers.

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
  );

/**
  This procedure will write GPIO Lock/LockTx register
  - For PCH SBI message is used.
  - For IBL MMIO access is used.

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
  );

#endif // _GPIO_PRIVATE_LIB_H_
