/** @file
  VMD PEI policy

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _VMD_PEI_CONFIG_H_
#define _VMD_PEI_CONFIG_H_

#include <ConfigBlock.h>

#pragma pack (push,1)

#define VMD_MAX_DEVICES 31

typedef struct {
    UINT8  RpBus;
    UINT8  RpDevice;
    UINT8  RpFunction;
    UINT8  RpEnable;
} RP_BDF_DATA;

/**
  This configuration block is to configure VMD related variables used in PostMem PEI.
  If VMD Device is not supported, all policies can be ignored.
  <b>Revision 1</b>:
  - Initial version.
  <b>Revision 2</b>:
  - Deprecated VmdPortAEnable, VmdPortBEnable, VmdPortCEnable, VmdPortDEnable.
  - Added VmdPortEnable[VMD_MAX_DEVICES] and structure to hold Vmd EFI Variable details.
    (Added B/D/F fields along with Port Enable for up to max 31 devices).
  <b>Revision 3</b>:
   Added policy to get the Bar values from platform PCD.
  <b>Revision 4</b>: Added VmdGlobalMapping to map all the storage devices under VMD
**/

typedef struct {
  CONFIG_BLOCK_HEADER  Header;                          ///< Offset 0-27 Config Block Header
  UINT8                VmdEnable;                       ///< Offset 28 This field used to enable VMD controller 1=Enable <b>0=Disable(default)</b>
  UINT8                VmdPortAEnable;                  /// Deprecated < Offset 29 This field used to enable VMD portA Support  1=Enable and 0=Disable (default)
  UINT8                VmdPortBEnable;                  /// Deprecated < Offset 30 This field used to enable VMD portB Support  1=Enable and 0=Disable (default)
  UINT8                VmdPortCEnable;                  /// Deprecated < Offset 31 This field used to enable VMD portC Support  1=Enable and 0=Disable (default)
  UINT8                VmdPortDEnable;                  /// Deprecated < Offset 32 This field used to enable VMD portD Support  1=Enable and 0=Disable (default)
  UINT8                VmdCfgBarSize;                   ///< Offset 33 This is used to set the VMD Config Bar Size. <b>25(32MB)</b>
  UINT8                VmdCfgBarAttr;                   ///< Offset 34 This is used to set VMD Config Bar Attributes 0: VMD_32BIT_NONPREFETCH, 1: VMD_64BIT_PREFETCH, <b>2: VMD_64BIT_NONPREFETCH(Default)</b>
  UINT8                VmdMemBarSize1;                  ///< Offset 35 This is used to set the VMD Mem Bar1 size.   <b>25 (32MB)</b>.
  UINT8                VmdMemBar1Attr;                  ///< Offset 36 This is used to set VMD Mem Bar1 Attributes  <b>0: VMD_32BIT_NONPREFETCH(Default) </b> 1: VMD_64BIT_NONPREFETCH, 2: VMD_64BIT_PREFETCH
  UINT8                VmdMemBarSize2;                  ///< Offset 37 This is used to set the VMD Mem Bar2 size.   <b>20(1MB)</b>.
  UINT8                VmdMemBar2Attr;                  ///< Offset 38 This is used to set VMD Mem Bar2 Attributes 0: VMD_32BIT_NONPREFETCH <b>1: VMD_64BIT_NONPREFETCH(Default)</b>, 2: VMD_64BIT_PREFETCH
  UINT8                VmdGlobalMapping;                ///< Offset 39 This field used to enable Global Mapping 1=Enable <b>0=Disable(default)</b>
  RP_BDF_DATA          VmdPortEnable[VMD_MAX_DEVICES];  ///< Offset 40 to 163 This field used to to store b/d/f for each root port along with enable Support  1=Enable <b>0=Disable (default)</b>
  VOID                 *VmdVariablePtr;                 /// This config block will be updated as per the EFI variable.
  UINT32               VmdCfgBarBase;                   /// Temp Address VMD CFG BAR Default is <b>0xA0000000</b>
  UINT32               VmdMemBar1Base;                  /// Temp Address VMD CFG BAR Default is <b>0xA2000000</b>
  UINT32               VmdMemBar2Base;                  /// Temp Address VMD CFG BAR Default is <b>0xA4000000</b>
} VMD_PEI_CONFIG;

#pragma pack (pop)

#endif /* _VMD_PEI_PREMEM_CONFIG_H_ */
