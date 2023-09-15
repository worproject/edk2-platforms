/** @file
  GPIO pins for TGL-PCH-LP,

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _GPIO_PINS_VER2_LP_H_
#define _GPIO_PINS_VER2_LP_H_
///
/// This header file should be used together with
/// PCH GPIO lib in C and ASL. All defines used
/// must match both ASL/C syntax
///

///
/// Unique ID used in GpioPad defines
///
#define GPIO_VER2_LP_CHIPSET_ID     0x9

///
/// TGL LP GPIO pins
/// Use below for functions from PCH GPIO Lib which
/// require GpioPad as argument. Encoding used here
/// has all information required by library functions
///
#define GPIO_VER2_LP_GPP_H0                  0x09070000
#define GPIO_VER2_LP_GPP_H1                  0x09070001
#define GPIO_VER2_LP_GPP_D16                 0x09080010
#define GPIO_VER2_LP_GPP_C2                  0x090B0002

#define GPIO_VER2_LP_GPP_F9                  0x090C0009
#define GPIO_VER2_LP_GPP_F10                 0x090C000A
#define GPIO_VER2_LP_GPP_F20                 0x090C0014

#define GPIO_VER2_LP_GPP_E7                  0x090E0007
#endif // _GPIO_PINS_VER2_LP_H_
