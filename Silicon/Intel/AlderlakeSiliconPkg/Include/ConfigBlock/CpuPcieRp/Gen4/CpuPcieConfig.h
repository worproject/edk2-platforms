/** @file
  Pcie root port policy

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _CPU_PCIE_CONFIG_H_
#define _CPU_PCIE_CONFIG_H_

#include <Library/GpioLib.h>
#include <Library/CpuPcieInfoFruLib.h>
#include <PcieConfig.h>
#include <ConfigBlock.h>
#include <Register/SaRegsHostBridge.h>

#pragma pack(push, 1)

#define CPU_PCIE_RP_PREMEM_CONFIG_REVISION  1

/**
 Making any setup structure change after code frozen
 will need to maintain backward compatibility, bump up
 structure revision and update below history table\n
  <b>Revision 1</b>:  - Initial version.
**/

#define CPU_PCIE_CONFIG_REVISION          10

#define L0_SET                            BIT0
#define L1_SET                            BIT1

/**
  CPU PCIe Root Port Pre-Memory Configuration
  Contains Root Port settings and capabilities
  <b>Revision 1</b>:  - Initial version.
  <b>Revision 2</b>:  - Adding Dekel Suqelch Workaround Setup Variable
  <b>Revision 3</b>:  - Deprecate Dekel Suqelch Workaround Setup Variable
  <b>Revision 4</b>:  - Adding New FOM Setup Variable
  <b>Revision 5</b>:  - Add CdrRelock Policy to CPU_PCIE_RP_PREMEM_CONFIG.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;                                ///< Config Block Header
  /**
  Root Port enabling mask.
  Bit0 presents RP1, Bit1 presents RP2, and so on.
  0: Disable; <b>1: Enable</b>.
  **/
  UINT32                RpEnabledMask;
  /**
  Assertion on Link Down GPIOs
  - <b>Disabled</b> (0x0) : Disable assertion on Link Down GPIOs(Default)
  - Enabled         (0x1) : Enable assertion on Link Down GPIOs
  **/
  UINT8                 LinkDownGpios;
  /**
  Enable ClockReq Messaging
  - <b>Disabled</> (0x0) : Disable ClockReq Messaging(Default)
  - Enabled        (0x1) : Enable ClockReq Messaging
  **/
  UINT8                 ClkReqMsgEnable;
  /**
  Dekel Recipe Workaround
  <b>2</b>
  1=Minimal, 9=Maximum,
  **/
  UINT8                 DekelSquelchWa;  // Deprecated variable
  UINT8                 Rsvd0[1];
  /**
  Determines each PCIE Port speed capability.
  <b>0: Auto</b>; 1: Gen1; 2: Gen2; 3: Gen3; 4: Gen4 (see: CPU_PCIE_SPEED)
  **/
  UINT8                 PcieSpeed[CPU_PCIE_MAX_ROOT_PORTS];

  /**
  Enable ClockReq Messaging Policy is for all Rootports
  - <b>Disabled</> (0x0) : Disable ClockReq Messaging(Default)
  - Enabled        (0x1) : Enable ClockReq Messaging
  **/
  UINT8                 ClkReqMsgEnableRp[CPU_PCIE_MAX_ROOT_PORTS];
  /**
  To Enable/Disable New FOM
  <b>0: Disable</b>; 1: Enable
  **/
  UINT8                 NewFom[CPU_PCIE_MAX_ROOT_PORTS];
  /**
  To Enable/Disable CDR Relock
  <b>0: Disable</b>; 1: Enable
  **/
  UINT8                 CdrRelock[CPU_PCIE_MAX_ROOT_PORTS];
} CPU_PCIE_RP_PREMEM_CONFIG;

/**
  Represent lane specific PCIe Gen3 equalization parameters.
**/
typedef struct {
  UINT8                  Cm;                 ///< Coefficient C-1
  UINT8                  Cp;                 ///< Coefficient C+1
  UINT8                  PegGen3RootPortPreset;      ///< <b>(Test)</b> Used for programming PEG Gen3 preset values per lane. Range: 0-9, 8 is default for each lane
  UINT8                  PegGen3EndPointPreset;      ///< <b>(Test)</b> Used for programming PEG Gen3 preset values per lane. Range: 0-9, 7 is default for each lane
  UINT8                  PegGen3EndPointHint;        ///< <b>(Test)</b> Hint value per lane for the PEG Gen3 End Point. Range: 0-6, 2 is default for each lane
  UINT8                  PegGen4RootPortPreset;      ///< <b>(Test)</b> Used for programming PEG Gen4 preset values per lane. Range: 0-9, 8 is default for each lane
  UINT8                  PegGen4EndPointPreset;      ///< <b>(Test)</b> Used for programming PEG Gen4 preset values per lane. Range: 0-9, 7 is default for each lane
  UINT8                  PegGen4EndPointHint;        ///< <b>(Test)</b> Hint value per lane for the PEG Gen4 End Point. Range: 0-6, 2 is default for each lane
} CPU_PCIE_EQ_LANE_PARAM;

/**
  The CPU_PCI_ROOT_PORT_CONFIG describe the feature and capability of each CPU PCIe root port.
**/
typedef struct {

  UINT32  ExtSync                         :  1;   ///< Indicate whether the extended synch is enabled. <b>0: Disable</b>; 1: Enable.
  UINT32  MultiVcEnabled                  :  1;   ///< Multiple Virtual Channel. 0: Disable; <b>1: Enable</b>
  UINT32  RsvdBits0                       : 30;   ///< Reserved bits
  /**
  PCIe Gen4 Equalization Method
  - HwEq           (0x1) : Hardware Equalization (Default)
  - StaticEq       (0x2) : Static Equalization
  **/
  UINT8   Gen4EqPh3Method;
  UINT8   FomsCp;                                 ///< FOM Score Board Control Policy
  UINT8   RsvdBytes0[2];                          ///< Reserved bytes

  //
  // Gen3 Equalization settings
  //
  UINT32  Gen3Uptp            :  4;               ///< <b>(Test)</b> Upstream Port Transmitter Preset used during Gen3 Link Equalization. Used for all lanes.  Default is <b>7</b>.
  UINT32  Gen3Dptp            :  4;               ///< <b>(Test)</b> Downstream Port Transmiter Preset used during Gen3 Link Equalization. Used for all lanes.  Default is <b>7</b>.
  //
  // Gen4 Equalization settings
  //
  UINT32  Gen4Uptp            :  4;               ///< <b>(Test)</b> Upstream Port Transmitter Preset used during Gen4 Link Equalization. Used for all lanes.  Default is <b>7</b>.
  UINT32  Gen4Dptp            :  4;               ///< <b>(Test)</b> Downstream Port Transmiter Preset used during Gen4 Link Equalization. Used for all lanes.  Default is <b>7</b>.
  //
  // Gen5 Equalization settings
  //
  UINT32  Gen5Uptp            :  4;               ///< <b>(Test)</b> Upstream Port Transmitter Preset used during Gen5 Link Equalization. Used for all lanes.  Default is <b>5</b>.
  UINT32  Gen5Dptp            :  4;               ///< <b>(Test)</b> Downstream Port Transmiter Preset used during Gen5 Link Equalization. Used for all lanes.  Default is <b>7</b>.
  UINT32  RsvdBits1           :  8;               ///< Reserved Bits

  PCIE_ROOT_PORT_COMMON_CONFIG                    PcieRpCommonConfig;                       ///< <b>(Test)</b> Includes policies which are common to both SA and PCH RootPort

} CPU_PCIE_ROOT_PORT_CONFIG;

typedef struct {
  UINT8   PcieGen3PresetCoeffSelection;           ///<Gen3 Preset or Coefficient Selection
  UINT8   PcieGen4PresetCoeffSelection;           ///<Gen4 Preset or Coefficient Selection
  UINT8   PcieGen5PresetCoeffSelection;           ///<Gen5 Preset or Coefficient Selection
  UINT8   Func0LinkDisable;                       ///< Disable Func0 Port
} CPU_PCIE_ROOT_PORT_CONFIG2;

/**
  The CPU_PCIE_CONFIG block describes the expected configuration of the CPU PCI Express controllers
  <b>Revision 1< / b>:
  -Initial version.
  <b>Revision 2</b>:
  - SlotSelection policy added
  <b>Revision 3</b>
  - Deprecate PegGen3ProgramStaticEq and PegGen4ProgramStaticEq
  <b>Revision 4</b>:
  - Deprecating SetSecuredRegisterLock
  <b>Revision 5</b>:
  - Moved ClockGating policy to PCIE_ROOT_PORT_COMMON_CONFIG
  - Moved PowerGating policy to PCIE_ROOT_PORT_COMMON_CONFIG
  - Deprecate VcEnabled policy
  <b>Revision 7</b>:
  - Deprecating Gen3PresetCoeffSelection and Gen4PresetCoeffSelection
  <b>Revision 8</b>:
  - Added Serl policy
  <b>Revision 9</b>:
  - Align revision with CPU_PCIE_CONFIG_REVISION value
  <b>Revision 10</b>:
  - Deprecate EqPh3LaneParam.Cm and EqPh3LaneParam.Cp
**/
typedef struct {
  CONFIG_BLOCK_HEADER               Header;                   ///< Config Block Header
  ///
  /// These members describe the configuration of each SA PCIe root port.
  ///
  CPU_PCIE_ROOT_PORT_CONFIG         RootPort[CPU_PCIE_MAX_ROOT_PORTS];
  ///
  /// Gen3 Equalization settings for physical PCIe lane, index 0 represents PCIe lane 1, etc.
  /// Corresponding entries are used when root port EqPh3Method is PchPcieEqStaticCoeff (default).
  ///
  CPU_PCIE_EQ_LANE_PARAM            EqPh3LaneParam[SA_PEG_MAX_LANE];  //@ Deprecated Policy
  ///
  /// List of coefficients used during equalization (applicable to both software and hardware EQ)
  ///
  PCIE_EQ_PARAM                     HwEqGen4CoeffList[PCIE_HWEQ_COEFFS_MAX];  //@ Deprecated Policy

  PCIE_COMMON_CONFIG                PcieCommonConfig;   /// < <b>(Test)</b> Includes policies which are common to both SA and PCH PCIe

  UINT32  FiaProgramming                  :  1;        /// < Skip Fia Configuration and lock if enable
  // Deprecated Policy
  /**
    <b>(Test)</b> Program PEG Gen3 EQ Phase1 Static Presets
  - Disabled        (0x0)  : Disable EQ Phase1 Static Presets Programming
  - <b>Enabled</b>  (0x1)  : Enable  EQ Phase1 Static Presets Programming (Default)
  **/
  UINT32  PegGen3ProgramStaticEq          :  1;

  // Deprecated Policy
  /**
  <b>(Test)</b> Program PEG Gen4 EQ Phase1 Static Presets
  - Disabled        (0x0)  : Disable EQ Phase1 Static Presets Programming
  - <b>Enabled</b>  (0x1)  : Enable  EQ Phase1 Static Presets Programming (Default)
  **/
  UINT32  PegGen4ProgramStaticEq          :  1;
  /**
  <b>(Test)</b> Cpu Pcie Secure Register Lock
  - Disabled        (0x0)
  - <b>Enabled</b>  (0x1)
  **/
  UINT32  SetSecuredRegisterLock          :  1;  // Deprecated Policy
  ///
  /// This member allows to select between the PCI Express M2 or CEMx4 slot <b>1: PCIe M2</b>; 0: CEMx4 slot.
  ///
  UINT32  SlotSelection                   :  1;

  UINT32  Serl                            :  1;

  UINT32  RsvdBits0                       : 26;

  /**
    PCIe device override table
    The PCIe device table is being used to override PCIe device ASPM settings.
    This is a pointer points to a 32bit address. And it's only used in PostMem phase.
    Please refer to PCIE_DEVICE_OVERRIDE structure for the table.
    Last entry VendorId must be 0.
    The prototype of this policy is:
    PCIE_DEVICE_OVERRIDE *PcieDeviceOverrideTablePtr;
  **/
  UINT32  PcieDeviceOverrideTablePtr;
  CPU_PCIE_ROOT_PORT_CONFIG2         RootPort2[CPU_PCIE_MAX_ROOT_PORTS];
  PCIE_COMMON_CONFIG2                PcieCommonConfig2;
  } CPU_PCIE_CONFIG;

#pragma pack (pop)

#endif // _CPU_PCIE_CONFIG_H_
