/** @file
  Header file for Cpu Information

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _CPU_GEN_INFO_H_
#define _CPU_GEN_INFO_H_

#include <CpuRegs.h>

//
// Processor Definitions
//
#define CPUID_FULL_FAMILY_MODEL_ALDERLAKE_MOBILE    0x000906A0

///
/// Enums for CPU Stepping IDs
///
typedef enum {

  ///
  /// AlderLake Mobile Steppings(ULT)
  ///
  EnumAdlJ0 = 0,
  EnumAdlK0 = 2,
  EnumAdlL0 = 3,

  ///
  /// AlderLake Mobile Steppings(ULX)
  ///
  EnumAdlQ0 = 1,
  EnumAdlR0 = 4,
  EnumAdlS0 = 5,


  ///
  /// Max Stepping
  ///
  EnumCpuSteppingMax  = CPUID_FULL_STEPPING
} CPU_STEPPING;
#endif
