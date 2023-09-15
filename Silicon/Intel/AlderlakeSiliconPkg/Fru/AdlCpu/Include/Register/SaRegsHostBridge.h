/** @file
  Register names for Host Bridge block
  <b>Conventions</b>:
  - Prefixes:
    - Definitions beginning with "R_" are registers
    - Definitions beginning with "B_" are bits within registers
    - Definitions beginning with "V_" are meaningful values of bits within the registers
    - Definitions beginning with "S_" are register sizes
    - Definitions beginning with "N_" are the bit position
  - In general, SA registers are denoted by "_SA_" in register names
  - Registers / bits that are different between SA generations are denoted by
    "_SA_[generation_name]_" in register/bit names. e.g., "_SA_HSW_"
  - Registers / bits that are different between SKUs are denoted by "_[SKU_name]"
    at the end of the register/bit names
  - Registers / bits of new devices introduced in a SA generation will be just named
    as "_SA_" without [generation_name] inserted.

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _SA_REGS_HOST_BRIDGE_H_
#define _SA_REGS_HOST_BRIDGE_H_

#define SA_SEG_NUM              0x00
//
// DEVICE 0 (Memory Controller Hub)
//
#define SA_MC_BUS          0x00
#define SA_MC_DEV          0x00
#define SA_MC_FUN          0x00
#define R_SA_MC_DEVICE_ID  0x02

//
// AlderLake CPU Mobile SA Device IDs B0:D0:F0
//
#define V_SA_DEVICE_ID_MB_ULT_1    0x4641   ///< AlderLake P (6+8+GT) SA DID
#define V_SA_DEVICE_ID_MB_ULT_2    0x4649   ///< AlderLake P (6+4(f)+GT) SA DID
#define V_SA_DEVICE_ID_MB_ULT_3    0x4621   ///< AlderLake P (4(f)+8+GT) SA DID
#define V_SA_DEVICE_ID_MB_ULT_4    0x4609   ///< AlderLake P (2(f)+4(f)+GT) SA DID
#define V_SA_DEVICE_ID_MB_ULT_5    0x4601   ///< AlderLake P (2+8+GT) SA DID
#define V_SA_DEVICE_ID_MB_ULT_6    0x4661   ///< AlderLake P (6+8+2) SA DID
#define V_SA_DEVICE_ID_MB_ULT_7    0x4629   ///< AlderLake P (4+4+1) SA DID
#define V_SA_DEVICE_ID_MB_ULT_8    0x4619   ///< AlderLake P (1+4+GT) SA DID
#define V_SA_DEVICE_ID_MB_ULT_9    0x4659   ///< AlderLake P (1+8+GT) SA DID
#define V_SA_DEVICE_ID_MB_ULT_10   0x4645   ///< AlderLake P (6+6+GT) SA DID
///
/// Description:
///  The SMRAMC register controls how accesses to Compatible SMRAM spaces are treated.  The Open, Close and Lock bits function only when G_SMRAME bit is set to 1.  Also, the Open bit must be reset before the Lock bit is set.
///
#define R_SA_SMRAMC  (0x88)

///
/// Description:
///  This register contains the Top of low memory address.
///
#define R_SA_TOLUD (0xbc)
///
/// Description of TOLUD (20:31)
///  This register contains bits 31 to 20 of an address one byte above the maximum DRAM memory below 4G that is usable by the operating system. Address bits 31 down to 20 programmed to 01h implies a minimum memory size of 1MB. Configuration software must set this value to the smaller of the following 2 choices: maximum amount memory in the system minus ME stolen memory plus one byte or the minimum address allocated for PCI memory. Address bits 19:0 are assumed to be 0_0000h for the purposes of address comparison. The Host interface positively decodes an address towards DRAM if the incoming address is less than the value programmed in this register.
///  The Top of Low Usable DRAM is the lowest address above both Graphics Stolen memory and Tseg. BIOS determines the base of Graphics Stolen Memory by subtracting the Graphics Stolen Memory Size from TOLUD and further decrements by Tseg size to determine base of Tseg. All the Bits in this register are locked in LT mode.
///  This register must be 1MB aligned when reclaim is enabled.
///
#define B_SA_TOLUD_TOLUD_MASK      (0xfff00000)
#endif
