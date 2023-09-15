/** @file
  Header file for GPIO Private Lib Internal functions.

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _GPIO_NATIVE_PRIVATE_LIB_INTERNAL_H_
#define _GPIO_NATIVE_PRIVATE_LIB_INTERNAL_H_

#include <Library/GpioPrivateLib.h>


/**
  This function provides recommended GPIO IO Standby configuration for a given native function

  @param[in]  PadFunction            PadFunction for a specific native signal. Please refer to GpioNativePads.h
  @param[out] StandbyState           IO Standby State for specified native function
  @param[out] StandbyTerm            IO Standby Termination for specified native function

  @retval Status
**/
EFI_STATUS
GpioGetFunctionIoStandbyConfig (
  IN  UINT32                PadFunction,
  OUT GPIO_IOSTANDBY_STATE  *StandbyState,
  OUT GPIO_IOSTANDBY_TERM   *StandbyTerm
  );

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
  );

#endif // _GPIO_NATIVE_PRIVATE_LIB_INTERNAL_H_
