/** @file
  Header file for GPIO Lib implementation.

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _GPIO_LIBRARY_H_
#define _GPIO_LIBRARY_H_

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/GpioLib.h>
#include <Library/GpioNativeLib.h>
#include <Library/GpioPrivateLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PchCycleDecodingLib.h>
#include <Library/PmcPrivateLib.h>
#include <Library/GpioHelpersLib.h>
#include <Register/GpioRegs.h>

//
// Number of PADCFG_DW registers
//
#define GPIO_PADCFG_DW_REG_NUMBER  4

#endif // _GPIO_LIBRARY_H_
