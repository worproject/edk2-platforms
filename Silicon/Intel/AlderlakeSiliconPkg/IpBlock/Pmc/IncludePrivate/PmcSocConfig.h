/** @file
  PMC SoC configuration

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PMC_SOC_CONFIGURATION_H_
#define _PMC_SOC_CONFIGURATION_H_

typedef enum {
  AdrSinglePhase = 0,
  AdrDualPhase
} ADR_PHASE_TYPE;

typedef enum {
  AdrGpioB = 0,
  AdrGpioC
} ADR_GPIO;

typedef enum {
  AdrOverPmSync = 0,
  AdrOverDmi
} ADR_MSG_INTERFACE;

typedef struct {
  BOOLEAN            Supported;
  ADR_PHASE_TYPE     AdrPhaseType;
  ADR_GPIO           AdrGpio;
  ADR_MSG_INTERFACE  AdrMsgInterface;
  //
  // On some designs ADR_GEN_CFG has been moved in the HW.
  // Set this to if ADR_GEN_CFG is located at 0x1908
  //
  BOOLEAN            AdrGenCfgMoved;
} PMC_ADR_SOC_CONFIG;

typedef struct {
  BOOLEAN             CppmCgInterfaceVersion;
  BOOLEAN             LpmSupported;
  UINT8               LpmInterfaceVersion;
  BOOLEAN             OsIdleSupported;
  BOOLEAN             TimedGpioSupported;
  UINT32              CpuIovrRampTime;
  BOOLEAN             PsOnSupported;
  BOOLEAN             ModPhySusPgSupported;
  UINT8               SciIrq;
  BOOLEAN             FabricPowerGatingCppmQualificationEnable;
  BOOLEAN             EspiBoot;
  BOOLEAN             UsbDbcConnected;
  UINT32              Usb3LanesConnectedBitmask;
  BOOLEAN             DisableIosfSbClockGating;
  BOOLEAN             SkipModPhyGatingPolicy;
  PMC_ADR_SOC_CONFIG  AdrSocConfig;
  BOOLEAN             AllSbrIdleQualifierEnable;
  UINT32              LpmPriVal;                            ///< Low Power Mode Priority

} PMC_SOC_CONFIG;

typedef struct {
  BOOLEAN  OverrideFetRampTime;
  UINT8    FetRampTime;
  UINT8    IsFetRampTime;
  UINT16   FuseDownloadDelayUs;
} PMC_FIVR_SOC_CONFIG;

#endif
