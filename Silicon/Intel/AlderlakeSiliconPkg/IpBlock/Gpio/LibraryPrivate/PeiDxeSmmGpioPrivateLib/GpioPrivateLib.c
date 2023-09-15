/** @file
  This file contains GPIO routines for RC usage

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/GpioLib.h>
#include <Library/GpioNativeLib.h>
#include <Library/GpioPrivateLib.h>
#include <Library/GpioNativePads.h>
#include <Register/GpioRegs.h>
#include "GpioNativePrivateLibInternal.h"

/**
  This procedure is used to check if GpioPad is valid for certain chipset

  @param[in]  GpioPad             GPIO pad

  @retval TRUE                    This pin is valid on this chipset
          FALSE                   Incorrect pin
**/
BOOLEAN
GpioIsCorrectPadForThisChipset (
  IN  GPIO_PAD        GpioPad
  )
{
  return ((GPIO_GET_CHIPSET_ID (GpioPad) == GpioGetThisChipsetId ()) &&
         (GpioGetGroupIndexFromGpioPad (GpioPad) < GpioGetNumberOfGroups ()));
}

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
  )
{
  UINT32                 GroupIndex;
  UINT32                 PadNumber;
  CONST GPIO_GROUP_INFO  *GpioGroupInfo;
  GPIO_GROUP             GpioGroup;
  UINT32                 GpioGroupInfoLength;
  UINT32                 SmiStsRegOffset;
  UINT32                 HostSwOwnRegOffset;
  GPIO_PAD_OWN           PadOwnVal;

  GpioGroupInfo = GpioGetGroupInfoTable (&GpioGroupInfoLength);

  PadNumber = 0;
  GroupIndex = 0;
  for (GroupIndex = 0; GroupIndex < GpioGroupInfoLength; GroupIndex++) {
    PadNumber = Index;
    if (PadNumber < GpioGroupInfo[GroupIndex].PadPerGroup) {
      //
      // Found group and pad number
      //
      break;
    }
    Index = Index - GpioGroupInfo[GroupIndex].PadPerGroup;
  }

  //
  // Check if legal pad number
  //
  if (PadNumber >= GpioGroupInfo[GroupIndex].PadPerGroup){
    return EFI_INVALID_PARAMETER;
  }

  //
  // Check if selected group has GPI SMI Enable and Status registers
  //
  if (GpioGroupInfo[GroupIndex].SmiEnOffset == NO_REGISTER_FOR_PROPERTY) {
    return EFI_INVALID_PARAMETER;
  }

  GpioGroup = GpioGetGroupFromGroupIndex (GroupIndex);
  *GpioPin = GpioGetGpioPadFromGroupAndPadNumber (GpioGroup, PadNumber);

  DEBUG_CODE_BEGIN ();
  //
  // Check if selected GPIO Pad is not owned by CSME/ISH/IE
  //
  GpioGetPadOwnership (*GpioPin, &PadOwnVal);
  if (PadOwnVal != GpioPadOwnHost) {
    DEBUG ((DEBUG_ERROR, "GPIO ERROR: GPIO not owned by host!\n"));
    return EFI_INVALID_PARAMETER;
  }
  DEBUG_CODE_END ();

  *GpiSmiBitOffset = (UINT8)(PadNumber % 32);

  HostSwOwnRegOffset = GpioGroupInfo[GroupIndex].HostOwnOffset + (PadNumber / 32) * 0x4;
  *GpiHostSwOwnRegAddress = PCH_PCR_ADDRESS (GpioGroupInfo[GroupIndex].Community, HostSwOwnRegOffset);

  SmiStsRegOffset = GpioGroupInfo[GroupIndex].SmiStsOffset + (PadNumber / 32) * 0x4;
  *GpiSmiStsRegAddress = PCH_PCR_ADDRESS (GpioGroupInfo[GroupIndex].Community, SmiStsRegOffset);

  return EFI_SUCCESS;
}

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
  )
{
  CONST GPIO_GROUP_INFO  *GpioGroupInfo;
  UINT32                 GpioGroupInfoLength;
  UINT32                 PadNumber;
  UINT32                 GroupIndex;

  if (!GpioIsCorrectPadForThisChipset (GpioPad)) {
    DEBUG ((DEBUG_ERROR, "GPIO ERROR: Incorrect GpioPad (0x%08x) used on this chipset!\n", GpioPad));
    goto Error;
  }

  GpioGroupInfo = GpioGetGroupInfoTable (&GpioGroupInfoLength);

  //
  // Check if legal pin number
  //
  GroupIndex = GpioGetGroupIndexFromGpioPad (GpioPad);
  PadNumber = GpioGetPadNumberFromGpioPad (GpioPad);
  if (PadNumber >= GpioGroupInfo[GroupIndex].PadPerGroup) {
    DEBUG ((DEBUG_ERROR, "GPIO ERROR: Pin number (%d) exceeds range of group (max: %d)\n",
                                                                                   PadNumber,
                                                                                   GpioGroupInfo[GroupIndex].PadPerGroup));
    goto Error;
  }

  return TRUE;
Error:
  ASSERT (FALSE);
  return FALSE;
}

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
  )
{
  UINT32                 PadCfgReg;
  CONST GPIO_GROUP_INFO  *GpioGroupInfo;
  UINT32                 GpioGroupInfoLength;
  UINT32                 GroupIndex;

  GroupIndex = GpioGetGroupIndexFromGpioPad (GpioPad);
  GpioGroupInfo = GpioGetGroupInfoTable (&GpioGroupInfoLength);

  //
  // Create Pad Configuration register offset
  //
  PadCfgReg = GpioGetGpioPadCfgAddressFromGpioPad (GpioPad, DwReg);
  return GpioRegisterAccessRead32 (&GpioGroupInfo[GroupIndex], PadCfgReg);
}


/**
  This procedure will calculate PADCFG register value based on GpioConfig data
  For physical/local/hard (not virtual) GPIO pads

  @param[in]  GpioPad                   GPIO Pad
  @param[in]  GpioConfig                GPIO Configuration data
  @param[out] PadCfgDwReg               PADCFG DWx register value
  @param[out] PadCfgDwRegMask           Mask with PADCFG DWx register bits to be modified

  @retval Status
**/
EFI_STATUS
GpioPadCfgRegValueFromGpioConfigHardGpio (
  IN  GPIO_PAD           GpioPad,
  IN  CONST GPIO_CONFIG  *GpioConfig,
  OUT UINT32             *PadCfgDwReg,
  OUT UINT32             *PadCfgDwRegMask
  )
{
  //
  // Configure how interrupt is triggered (RxEvCfg)
  //
  PadCfgDwRegMask[0] |= ((((GpioConfig->InterruptConfig & B_GPIO_INT_CONFIG_INT_TYPE_MASK) >> N_GPIO_INT_CONFIG_INT_TYPE_BIT_POS) == GpioHardwareDefault) ? 0x0 : B_GPIO_PCR_RX_LVL_EDG);
  PadCfgDwReg[0] |= (((GpioConfig->InterruptConfig & B_GPIO_INT_CONFIG_INT_TYPE_MASK) >> (N_GPIO_INT_CONFIG_INT_TYPE_BIT_POS + 1)) << N_GPIO_PCR_RX_LVL_EDG);

  //
  // Configure interrupt generation (GPIRoutIOxAPIC/SCI/SMI/NMI)
  //
  PadCfgDwRegMask[0] |= ((((GpioConfig->InterruptConfig & B_GPIO_INT_CONFIG_INT_SOURCE_MASK) >> N_GPIO_INT_CONFIG_INT_SOURCE_BIT_POS) == GpioHardwareDefault)  ? 0x0 : (B_GPIO_PCR_RX_NMI_ROUTE | B_GPIO_PCR_RX_SCI_ROUTE | B_GPIO_PCR_RX_SMI_ROUTE | B_GPIO_PCR_RX_APIC_ROUTE));
  PadCfgDwReg[0] |= (((GpioConfig->InterruptConfig & B_GPIO_INT_CONFIG_INT_SOURCE_MASK) >> (N_GPIO_INT_CONFIG_INT_SOURCE_BIT_POS + 1)) << N_GPIO_PCR_RX_NMI_ROUTE);

  //
  // Configure GPIO direction (GPIORxDis and GPIOTxDis)
  //
  PadCfgDwRegMask[0] |= ((((GpioConfig->Direction & B_GPIO_DIRECTION_DIR_MASK) >> N_GPIO_DIRECTION_DIR_BIT_POS) == GpioHardwareDefault) ? 0x0 : (B_GPIO_PCR_RXDIS | B_GPIO_PCR_TXDIS));
  PadCfgDwReg[0] |= (((GpioConfig->Direction & B_GPIO_DIRECTION_DIR_MASK) >> (N_GPIO_DIRECTION_DIR_BIT_POS + 1)) << N_GPIO_PCR_TXDIS);

  //
  // Configure GPIO input inversion (RXINV)
  //
  PadCfgDwRegMask[0] |= ((((GpioConfig->Direction & B_GPIO_DIRECTION_INV_MASK) >> N_GPIO_DIRECTION_INV_BIT_POS) == GpioHardwareDefault) ?  0x0 : B_GPIO_PCR_RXINV);
  PadCfgDwReg[0] |= (((GpioConfig->Direction & B_GPIO_DIRECTION_INV_MASK) >> (N_GPIO_DIRECTION_INV_BIT_POS + 1)) << N_GPIO_PCR_RXINV);

  //
  // Configure GPIO output state (GPIOTxState)
  //
  PadCfgDwRegMask[0] |= ((((GpioConfig->OutputState & B_GPIO_OUTPUT_MASK) >> N_GPIO_OUTPUT_BIT_POS) == GpioHardwareDefault) ? 0x0 : B_GPIO_PCR_TX_STATE);
  PadCfgDwReg[0] |= (((GpioConfig->OutputState & B_GPIO_OUTPUT_MASK) >> (N_GPIO_OUTPUT_BIT_POS + 1)) << N_GPIO_PCR_TX_STATE);

  //
  // Configure GPIO RX raw override to '1' (RXRAW1)
  //
  PadCfgDwRegMask[0] |= ((((GpioConfig->OtherSettings & B_GPIO_OTHER_CONFIG_RXRAW_MASK) >> N_GPIO_OTHER_CONFIG_RXRAW_BIT_POS) == GpioHardwareDefault) ? 0x0 : B_GPIO_PCR_RX_RAW1);
  PadCfgDwReg[0] |= (((GpioConfig->OtherSettings & B_GPIO_OTHER_CONFIG_RXRAW_MASK) >> (N_GPIO_OTHER_CONFIG_RXRAW_BIT_POS + 1)) << N_GPIO_PCR_RX_RAW1);

  //
  // Configure GPIO Pad Mode (PMode)
  //
  PadCfgDwRegMask[0] |= ((((GpioConfig->PadMode & B_GPIO_PAD_MODE_MASK) >> N_GPIO_PAD_MODE_BIT_POS) == GpioHardwareDefault) ? 0x0 : B_GPIO_PCR_PAD_MODE);
  PadCfgDwReg[0] |= (((GpioConfig->PadMode & B_GPIO_PAD_MODE_MASK) >> (N_GPIO_PAD_MODE_BIT_POS + 1)) << N_GPIO_PCR_PAD_MODE);

  //
  // Configure GPIO termination (Term)
  //
  PadCfgDwRegMask[1] |= ((((GpioConfig->ElectricalConfig & B_GPIO_ELECTRICAL_CONFIG_TERMINATION_MASK) >> N_GPIO_ELECTRICAL_CONFIG_TERMINATION_BIT_POS) == GpioHardwareDefault) ? 0x0 : B_GPIO_PCR_TERM);
  PadCfgDwReg[1] |= (((GpioConfig->ElectricalConfig & B_GPIO_ELECTRICAL_CONFIG_TERMINATION_MASK) >> (N_GPIO_ELECTRICAL_CONFIG_TERMINATION_BIT_POS + 1)) << N_GPIO_PCR_TERM);

  return EFI_SUCCESS;
}

