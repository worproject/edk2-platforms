/** @file
  TCSS PEI policy

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _TCSS_PEI_CONFIG_H_
#define _TCSS_PEI_CONFIG_H_

#include <ConfigBlock.h>

extern EFI_GUID gTcssPeiConfigGuid;

#pragma pack (push,1)


#define MAX_IOM_AUX_BIAS_COUNT 4

///
/// The IOM_AUX_ORI_PAD_CONFIG describes IOM TypeC port map GPIO pin.
/// Those GPIO setting for DP Aux Orientation Bias Control when the TypeC port didn't have re-timer.
/// IOM needs know Pull-Up and Pull-Down pin for Bias control
///
typedef struct {
  UINT32     GpioPullN; ///< GPIO Pull Up Ping number that is for IOM indecate the pull up pin from TypeC port.
  UINT32     GpioPullP; ///< GPIO Pull Down Ping number that is for IOM indecate the pull down pin from TypeC port.
} IOM_AUX_ORI_PAD_CONFIG;

///
/// The IOM_EC_INTERFACE_CONFIG block describes interaction between BIOS and IOM-EC.
///

typedef struct {
  UINT32     VccSt;         ///< IOM VCCST request. (Not equal to actual VCCST value)
  UINT32     UsbOverride;   ///< IOM to override USB connection.
  UINT32     D3ColdEnable;  ///< Enable/disable D3 Cold support in TCSS
  UINT32     D3HotEnable;   ///< Enable/disable D3 Hot support in TCSS
} IOM_INTERFACE_CONFIG;

///
/// The PMC_INTERFACE_CONFIG block describes interaction between BIOS and PMC
///
typedef struct {
  UINT8      PmcPdEnable;    ///< PMC PD Solution Enable
  UINT8      Rsvd[3];
} PMC_INTERFACE_CONFIG;

///
/// The TCSS_IOM_PEI_CONFIG block describes IOM Aux/HSL override settings for TCSS.
///
typedef struct {
  UINT16    AuxOri;     ///< Bits defining value for IOM Aux Orientation Register
  UINT16    HslOri;     ///< Bits defining value for IOM HSL Orientation Register
} TCSS_IOM_ORI_OVERRIDE;

///
/// The TCSS_IOM_PEI_CONFIG block describes IOM settings for TCSS.
///
typedef struct {
  IOM_AUX_ORI_PAD_CONFIG    IomAuxPortPad[MAX_IOM_AUX_BIAS_COUNT];   ///< The IOM_AUX_ORI_BIAS_CTRL port config setting.
  TCSS_IOM_ORI_OVERRIDE     IomOverrides;
  IOM_INTERFACE_CONFIG      IomInterface;                            ///< Config settings are BIOS <-> IOM interface.
  PMC_INTERFACE_CONFIG      PmcInterface;                            ///< Config settings for BIOS <-> PMC interface
  UINT8                     TcStateLimit;                            ///< Tcss C-State deep stage
  UINT8                     Reserved[3];                             ///< Reserved bytes for future use
} TCSS_IOM_PEI_CONFIG;


#pragma pack (pop)

#endif /* _TCSS_PEI_CONFIG_H_ */
