/** @file
  Register names for ADL PCH LPC/eSPI device

  Conventions:

  - Prefixes:
    Definitions beginning with "R_" are registers
    Definitions beginning with "B_" are bits within registers
    Definitions beginning with "V_" are meaningful values within the bits
    Definitions beginning with "S_" are register sizes
    Definitions beginning with "N_" are the bit position

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _PCH_REGS_LPC_ADL_H_
#define _PCH_REGS_LPC_ADL_H_

//
// ADL PCH-P/M LPC Device IDs
//
#define V_ADL_PCH_P_LPC_CFG_DEVICE_ID_MB_0         0x5180          ///< LPC/eSPI Controller
#define V_ADL_PCH_P_LPC_CFG_DEVICE_ID_MB_1         0x5181          ///< LPC/eSPI Controller P SuperSKU
#define V_ADL_PCH_P_LPC_CFG_DEVICE_ID_MB_2         0x5182          ///< LPC/eSPI Controller P Premium
#define V_ADL_PCH_P_LPC_CFG_DEVICE_ID_MB_3         0x5183          ///< LPC/eSPI Controller Placeholder
#define V_ADL_PCH_P_LPC_CFG_DEVICE_ID_MB_4         0x5184          ///< LPC/eSPI Controller
#define V_ADL_PCH_P_LPC_CFG_DEVICE_ID_MB_5         0x5185          ///< LPC/eSPI Controller


#endif
