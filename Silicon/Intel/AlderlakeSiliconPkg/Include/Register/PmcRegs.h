/** @file
  Register names for PCH PMC device

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
#ifndef _PCH_REGS_PMC_H_
#define _PCH_REGS_PMC_H_

//
// ACPI and legacy I/O register offsets from ACPIBASE
//
#define R_ACPI_IO_PM1_STS                        0x00
#define S_ACPI_IO_PM1_STS                        2
#define B_ACPI_IO_PM1_STS_PRBTNOR                BIT11
#define B_ACPI_IO_PM1_STS_PWRBTN                 BIT8
#define N_ACPI_IO_PM1_STS_RTC                    10
#define N_ACPI_IO_PM1_STS_PWRBTN                 8
#define N_ACPI_IO_PM1_STS_TMROF                  0

#define R_ACPI_IO_PM1_EN                         0x02
#define S_ACPI_IO_PM1_EN                         2
#define B_ACPI_IO_PM1_EN_PWRBTN                  BIT8
#define N_ACPI_IO_PM1_EN_RTC                     10
#define N_ACPI_IO_PM1_EN_PWRBTN                  8
#define N_ACPI_IO_PM1_EN_TMROF                   0

#define R_ACPI_IO_PM1_CNT                        0x04                                   ///< Power Management 1 Control
#define B_ACPI_IO_PM1_CNT_SLP_EN                 BIT13                                  ///< Sleep Enable
#define B_ACPI_IO_PM1_CNT_SLP_TYP                (BIT12 | BIT11 | BIT10)                ///< Sleep Type
#define V_ACPI_IO_PM1_CNT_S0                     0
#define V_ACPI_IO_PM1_CNT_S1                     BIT10
#define V_ACPI_IO_PM1_CNT_S3                     (BIT12 | BIT10)
#define V_ACPI_IO_PM1_CNT_S4                     (BIT12 | BIT11)
#define V_ACPI_IO_PM1_CNT_S5                     (BIT12 | BIT11 | BIT10)
#define B_ACPI_IO_PM1_CNT_SCI_EN                 BIT0                                   ///< SCI Enable

#define R_ACPI_IO_PM1_TMR                        0x08                                   ///< Power Management 1 Timer
#define B_ACPI_IO_PM1_TMR_TMR_VAL                0xFFFFFF                               ///< Timer Value
#define V_ACPI_IO_PM1_TMR_MAX_VAL                0x1000000       ///< The timer is 24 bit overflow

#define R_ACPI_IO_SMI_EN                              0x30                              ///< SMI Control and Enable
#define S_ACPI_IO_SMI_EN                              4
#define B_ACPI_IO_SMI_EN_LEGACY_USB2                  BIT17
#define B_ACPI_IO_SMI_EN_TCO                          BIT13
#define B_ACPI_IO_SMI_EN_SWSMI_TMR                    BIT6
#define B_ACPI_IO_SMI_EN_APMC                         BIT5
#define B_ACPI_IO_SMI_EN_LEGACY_USB                   BIT3
#define B_ACPI_IO_SMI_EN_EOS                          BIT1                              ///< End of SMI
#define B_ACPI_IO_SMI_EN_GBL_SMI                      BIT0
#define N_ACPI_IO_SMI_EN_LEGACY_USB3                  31
#define N_ACPI_IO_SMI_EN_PERIODIC                     14
#define N_ACPI_IO_SMI_EN_TCO                          13
#define N_ACPI_IO_SMI_EN_MCSMI                        11
#define N_ACPI_IO_SMI_EN_SWSMI_TMR                    6
#define N_ACPI_IO_SMI_EN_APMC                         5
#define N_ACPI_IO_SMI_EN_ON_SLP_EN                    4
#define N_ACPI_IO_SMI_EN_LEGACY_USB                   3

#define R_ACPI_IO_SMI_STS                             0x34                              ///< SMI Status Register
#define S_ACPI_IO_SMI_STS                             4
#define B_ACPI_IO_SMI_STS_GPIO_UNLOCK                 BIT27
#define B_ACPI_IO_SMI_STS_TCO                         BIT13
#define B_ACPI_IO_SMI_STS_APM                         BIT5
#define N_ACPI_IO_SMI_STS_LEGACY_USB3                 31
#define N_ACPI_IO_SMI_STS_SPI                         26
#define N_ACPI_IO_SMI_STS_SMBUS                       16
#define N_ACPI_IO_SMI_STS_SERIRQ                      15
#define N_ACPI_IO_SMI_STS_PERIODIC                    14
#define N_ACPI_IO_SMI_STS_TCO                         13
#define N_ACPI_IO_SMI_STS_MCSMI                       11
#define N_ACPI_IO_SMI_STS_GPIO_SMI                    10
#define N_ACPI_IO_SMI_STS_GPE0                        9
#define N_ACPI_IO_SMI_STS_PM1_STS_REG                 8
#define N_ACPI_IO_SMI_STS_SWSMI_TMR                   6
#define N_ACPI_IO_SMI_STS_APM                         5
#define N_ACPI_IO_SMI_STS_ON_SLP_EN                   4
#define N_ACPI_IO_SMI_STS_LEGACY_USB                  3

#define R_ACPI_IO_GPE0_STS_127_96                  0x6C                                 ///< General Purpose Event 0 Status [127:96]
#define S_ACPI_IO_GPE0_STS_127_96                  4
#define B_ACPI_IO_GPE0_STS_127_96_WADT             BIT18
#define N_ACPI_IO_GPE0_STS_127_96_PME_B0           13
#define N_ACPI_IO_GPE0_STS_127_96_PME              11
#define R_ACPI_IO_GPE0_EN_127_96                   0x7C                                 ///< General Purpose Event 0 Enable [127:96]
#define S_ACPI_IO_GPE0_EN_127_96                   4
#define B_ACPI_IO_GPE0_EN_127_96_WADT              BIT18
#define N_ACPI_IO_GPE0_EN_127_96_PME_B0            13
#define N_ACPI_IO_GPE0_EN_127_96_PME               11

//
// PWRM Registers
//
#define R_PMC_PWRM_GEN_PMCON_A                              0x1020                        ///< General PM Configuration A
#define B_PMC_PWRM_GEN_PMCON_A_SWSMI_RTSL                   0xC0
#define V_PMC_PWRM_GEN_PMCON_A_SWSMI_RTSL_64MS              0xC0
#define V_PMC_PWRM_GEN_PMCON_A_SWSMI_RTSL_32MS              0x80
#define V_PMC_PWRM_GEN_PMCON_A_SWSMI_RTSL_16MS              0x40
#define V_PMC_PWRM_GEN_PMCON_A_SWSMI_RTSL_1_5MS             0x00
#define B_PMC_PWRM_GEN_PMCON_A_PER_SMI_SEL                  (BIT2|BIT1)                 ///< Period SMI Select
#define V_PMC_PWRM_GEN_PMCON_A_PER_SMI_64S                  0x0000
#define V_PMC_PWRM_GEN_PMCON_A_PER_SMI_32S                  0x0002
#define V_PMC_PWRM_GEN_PMCON_A_PER_SMI_16S                  0x0004
#define V_PMC_PWRM_GEN_PMCON_A_PER_SMI_8S                   0x0006
#endif
