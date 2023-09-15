/** @file
  Register names for PCH LPC/eSPI device

Conventions:

  - Register definition format:
    Prefix_[GenerationName]_[ComponentName]_SubsystemName_RegisterSpace_RegisterName
  - Prefix:
    Definitions beginning with "R_" are registers
    Definitions beginning with "B_" are bits within registers
    Definitions beginning with "V_" are meaningful values within the bits
    Definitions beginning with "S_" are register size
    Definitions beginning with "N_" are the bit position
  - [GenerationName]:
    Three letter acronym of the generation is used (e.g. SKL,KBL,CNL etc.).
    Register name without GenerationName applies to all generations.
  - [ComponentName]:
    This field indicates the component name that the register belongs to (e.g. PCH, SA etc.)
    Register name without ComponentName applies to all components.
    Register that is specific to -H denoted by "_PCH_H_" in component name.
    Register that is specific to -LP denoted by "_PCH_LP_" in component name.
  - SubsystemName:
    This field indicates the subsystem name of the component that the register belongs to
    (e.g. PCIE, USB, SATA, GPIO, PMC etc.).
  - RegisterSpace:
    MEM - MMIO space register of subsystem.
    IO  - IO space register of subsystem.
    PCR - Private configuration register of subsystem.
    CFG - PCI configuration space register of subsystem.
  - RegisterName:
    Full register name.

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _PCH_REGS_LPC_H_
#define _PCH_REGS_LPC_H_

//
// PCI to LPC Bridge Registers
//

#define R_LPC_CFG_IOD                             0x80
#define R_LPC_CFG_IOE                             0x82
#define R_ESPI_CFG_CS1IORE                        0xA0

#define R_LPC_CFG_ULKMC                           0x94
#define B_LPC_CFG_ULKMC_A20PASSEN                 BIT5
#define B_LPC_CFG_ULKMC_64WEN                     BIT3
#define B_LPC_CFG_ULKMC_64REN                     BIT2
#define B_LPC_CFG_ULKMC_60WEN                     BIT1
#define B_LPC_CFG_ULKMC_60REN                     BIT0

//
// APM Registers
//
#define R_PCH_IO_APM_CNT                             0xB2
#define R_PCH_IO_APM_STS                             0xB3
#define R_LPC_CFG_BC                              0xDC            ///< Bios Control
#define S_LPC_CFG_BC                              1
#define N_LPC_CFG_BC_LE                           1

//
// Reset Generator I/O Port
//
#define R_PCH_IO_RST_CNT                             0xCF9
#define V_PCH_IO_RST_CNT_FULLRESET                   0x0E
#define V_PCH_IO_RST_CNT_HARDRESET                   0x06
//
// eSPI PCR Registers
//
#define R_ESPI_PCR_CFG_VAL                    0xC00C          ///< ESPI Enabled Strap
#define B_ESPI_PCR_CFG_VAL_ESPI_EN            BIT0            ///< ESPI Enabled Strap bit position
#define R_ESPI_PCR_SOFTSTRAPS                 0xC210          ///< eSPI Sofstraps Register 0
#define B_ESPI_PCR_SOFTSTRAPS_CS1_EN          BIT12           ///< CS1# Enable

#endif
