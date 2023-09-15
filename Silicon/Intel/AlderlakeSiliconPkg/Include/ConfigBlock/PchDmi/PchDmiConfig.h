/** @file
  DMI policy

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _PCH_DMI_CONFIG_H_
#define _PCH_DMI_CONFIG_H_

extern EFI_GUID gPchDmiConfigGuid;

/*
<b>Revision 1</b>:  - Initial version.
*/
#define PCH_DMI_PREMEM_CONFIG_REVISION 1
extern EFI_GUID gPchDmiPreMemConfigGuid;

#pragma pack (push,1)

#define PCH_DMI_HWEQ_COEFFS_MAX    8
/**
  Lane specific Dmi Gen3, Gen4 equalization parameters.
**/
typedef struct {
  UINT8   Cm;                 ///< Coefficient C-1
  UINT8   Cp;                 ///< Coefficient C+1
  UINT8   Rsvd0[2];           ///< Reserved bytes
} PCH_DMI_EQ_PARAM;

typedef struct {
  CONFIG_BLOCK_HEADER   Header;                   ///< Config Block
  PCH_DMI_EQ_PARAM  DmiHwEqGen3CoeffList[PCH_DMI_HWEQ_COEFFS_MAX];
  UINT8             DmiHweq;
  UINT8             Reserved[3];
} PCH_DMI_PREMEM_CONFIG;


/**
 The PCH_DMI_CONFIG block describes the expected configuration of the PCH for DMI.
   <b>Revision 1</b>:
  - Initial version.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;         ///< Config Block Header

  UINT32     PwrOptEnable      :  1;    ///< <b>0: Disable</b>; 1: Enable DMI Power Optimizer on PCH side.
  UINT32     DmiAspmCtrl       :  8;    ///< ASPM configuration on the PCH side of the DMI/OPI Link. Default is <b>PchPcieAspmAutoConfig</b>
  UINT32     CwbEnable         :  1;    ///< 0: Disable; <b>1: Enable</b> Central Write Buffer feature configurable and enabled by default
  UINT32     L1RpCtl           :  1;    ///< 0: Disable; <b>1: Enable</b> Allow DMI enter L1 when all root ports are in L1, L0s or link down. Disabled by default.
  /**
   When set to TRUE turns on:
     - L1 State Controller Power Gating
     - L1 State PHY Data Lane Power Gating
     - PHY Common Lane Power Gating
     - Hardware Autonomous Enable
     - PMC Request Enable and Sleep Enable
  **/
  UINT32     DmiPowerReduction :  1;
  UINT32     ClockGating       :  1;    ///< 0: Disable; 1: Enable clock gating.
  UINT32     Rsvdbits          : 19;    ///< Reserved bits
} PCH_DMI_CONFIG;

#pragma pack (pop)

#endif // _PCH_DMI_CONFIG_H_
