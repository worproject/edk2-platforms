## @file
#  Alderlake P RVP GPIO definition table for Pre-Memory Initialization
#
#   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
#   SPDX-License-Identifier: BSD-2-Clause-Patent
#
##

###
### !!! GPIOs designated to Native Functions shall not be configured by Platform Code.
### Native Pins shall be configured by Silicon Code (based on BIOS policies setting) or soft straps(set by CSME in FITc).
###
###

# mGpioTablePreMemAdlPDdr5Rvp
[PcdsDynamicExVpd.common.SkuIdAdlPDdr5Rvp]
gBoardModuleTokenSpaceGuid.VpdPcdBoardGpioTablePreMem|*|{CODE({
  // CPU M.2 SSD
  { GPIO_VER2_LP_GPP_F20, { GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut, GpioOutHigh, GpioIntDis, GpioPlatformReset, GpioTermNone } },  //CPU SSD RESET

  // CPU M.2 SSD2
  {GPIO_VER2_LP_GPP_C2, {GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut,  GpioOutHigh,   GpioIntDis,GpioPlatformReset,  GpioTermNone}},  //CPU SSD2 PWREN
  {GPIO_VER2_LP_GPP_H1, {GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut,  GpioOutHigh,   GpioIntDis,GpioPlatformReset,  GpioTermNone}},  //CPU SSD2 RESET

  // X4 Pcie Slot for Gen3 and Gen 4
  {GPIO_VER2_LP_GPP_F10, {GpioPadModeGpio, GpioHostOwnAcpi, GpioDirOut,  GpioOutHigh,   GpioIntDis,GpioPlatformReset,  GpioTermNone}},//ONBOARD_X4_PCIE_SLOT1_RESET_N

  {0x0}  // terminator
})}
