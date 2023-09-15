/** @file
Pcie root port policy

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _CPU_PCIE_CONFIG_GEN3_H_
#define _CPU_PCIE_CONFIG_GEN3_H_

#include <Library/GpioLib.h>
#include <Library/CpuPcieInfoFruLib.h>
#include <PcieConfig.h>
#include <ConfigBlock.h>
#include <Register/SaRegsHostBridge.h>

#pragma pack(push, 1)


#define L0_SET                            BIT0
#define L1_SET                            BIT1


///
/// SA GPIO Data Structure
///
typedef struct {
  GPIO_PAD      GpioPad;        ///< Offset 0: GPIO Pad
  UINT8         Value;          ///< Offset 4: GPIO Value
  UINT8         Rsvd0[3];       ///< Offset 5: Reserved for 4 bytes alignment
  UINT32        Active : 1;     ///< Offset 8: 0=Active Low; 1=Active High
  UINT32        RsvdBits0 : 31;
} SA_GPIO_INFO_PCIE;

///
/// SA Board PEG GPIO Info
///
typedef struct {
  SA_GPIO_INFO_PCIE  SaPeg0ResetGpio;    ///< Offset 0:  PEG0 PERST# GPIO assigned, must be a PCH GPIO pin
  SA_GPIO_INFO_PCIE  SaPeg3ResetGpio;    ///< Offset 12: PEG3 PERST# GPIO assigned, must be a PCH GPIO pin
  BOOLEAN            GpioSupport;        ///< Offset 24: 1=Supported; 0=Not Supported
  UINT8              Rsvd0[3];           ///< Offset 25: Reserved for 4 bytes alignment
} PEG_GPIO_DATA;


#pragma pack (pop)

#endif // _CPU_PCIE_CONFIG_GEN3_H_
