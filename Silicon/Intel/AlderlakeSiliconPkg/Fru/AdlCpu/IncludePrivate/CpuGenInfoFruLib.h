 /** @file
  This file contains Cpu Information for specific generation.

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _CPU_GEN_INFO_FRU_LIB_H_
#define _CPU_GEN_INFO_FRU_LIB_H_
#include <CpuRegs.h>
#include <CpuGenInfo.h>
#include <Register/CommonMsr.h>

///
/// Used to identify the CPU used for programming with the VR override table
///
typedef enum {
  EnumUnknownCpuId              = 0,
  EnumMinCpuId                  = 1,

  ///
  /// ADL P
  ///
  EnumAdlP15Watt282fCpuId       = 0x30,
  EnumAdlP28Watt482fCpuId       = 0x31,
  EnumAdlP28Watt682fCpuId       = 0x32,
  EnumAdlP45Watt682fCpuId       = 0x35,
  EnumAdlP15Watt142fCpuId       = 0x36,
  EnumAdlP15Watt242fCpuId       = 0x37,
  EnumAdlP45Watt482fCpuId       = 0x38,
  EnumAdlP45Watt442fCpuId       = 0x39,
  EnumAdlP28Watt442fCpuId       = 0x3A,
  EnumAdlP28Watt282fCpuId       = 0x3B,
  EnumAdlP28Watt242fCpuId       = 0x3C,
  EnumAdlP28Watt142fCpuId       = 0x3D,
  EnumAdlP45Watt242fCpuId       = 0x3E,
  EnumAdlP28Watt182fCpuId       = 0x3F,
  EnumAdlP28Watt662fCpuId       = 0x40,
  EnumAdlP28Watt642fCpuId       = 0x41,
  EnumAdlP45Watt642fCpuId       = 0x42,
  EnumAdlPMaxCpuId              = EnumAdlP45Watt642fCpuId,

} CPU_IDENTIFIER;

#endif // _CPU_GEN_INFO_FRU_LIB_H_
