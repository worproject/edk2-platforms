/** @file
  Register names for CPU registers

  <b>Conventions</b>
  - Definitions beginning with "MSR_" are MSRs
  - Definitions beginning with "R_" are registers
  - Definitions beginning with "B_" are bits within registers
  - Definitions beginning with "V_" are meaningful values of bits within the registers
  - Definitions beginning with "S_" are register sizes
  - Definitions beginning with "N_" are the bit position

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _CPU_REGS_H_
#define _CPU_REGS_H_

///
/// Other defines
///
#ifndef CPU_FEATURE_DISABLE
#define CPU_FEATURE_DISABLE  0
#endif
#ifndef CPU_FEATURE_ENABLE
#define CPU_FEATURE_ENABLE   1
#endif

//
// Processor Definitions
//
#define CPUID_FULL_STEPPING                        0x0000000F
#define CPUID_FULL_FAMILY_MODEL                    0x0FFF0FF0

#define BITS(x) (1 << (x))

/**
Notes :
  1.  Bit position always starts at 0.
  2.  Following macros are applicable only for Word aligned integers.
**/
#define BIT(Pos, Value)               (1 << (Pos) & (Value))

typedef UINT32       CPU_FAMILY;

///
/// Enums for CPU SKU IDs
///
typedef enum {
  EnumCpuUlt        = 0,
  EnumCpuTrad,
  EnumCpuUlx,
  EnumCpuHalo,
  EnumCpuUnknown
} CPU_SKU;

///
/// Enums for CPU Generation
///
typedef enum {
  EnumAdlCpu = 0,
  EnumCpuUnknownGeneration = 255
} CPU_GENERATION;
#endif
