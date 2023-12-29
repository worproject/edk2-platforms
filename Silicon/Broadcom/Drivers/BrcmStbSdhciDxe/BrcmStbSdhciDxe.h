/** @file
 *
 *  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#ifndef __BRCMSTB_SDHCI_DXE_H__
#define __BRCMSTB_SDHCI_DXE_H__

#define SDIO_HOST_SIZE                                0x260

#define SDIO_CFG_CTRL                                 0x0
#define SDIO_CFG_CTRL_SDCD_N_TEST_EN                  BIT31
#define SDIO_CFG_CTRL_SDCD_N_TEST_LEV                 BIT30

#define SDIO_CFG_SD_PIN_SEL                           0x44
#define SDIO_CFG_SD_PIN_SEL_MASK                      (BIT1 | BIT0)
#define SDIO_CFG_SD_PIN_SEL_CARD                      BIT1

#define SDIO_CFG_MAX_50MHZ_MODE                       0x1ac
#define SDIO_CFG_MAX_50MHZ_MODE_STRAP_OVERRIDE        BIT31
#define SDIO_CFG_MAX_50MHZ_MODE_ENABLE                BIT0

typedef struct {
  UINT32    TimeoutFreq   : 6; // bit 0:5
  UINT32    Reserved      : 1; // bit 6
  UINT32    TimeoutUnit   : 1; // bit 7
  UINT32    BaseClkFreq   : 8; // bit 8:15
  UINT32    MaxBlkLen     : 2; // bit 16:17
  UINT32    BusWidth8     : 1; // bit 18
  UINT32    Adma2         : 1; // bit 19
  UINT32    Reserved2     : 1; // bit 20
  UINT32    HighSpeed     : 1; // bit 21
  UINT32    Sdma          : 1; // bit 22
  UINT32    SuspRes       : 1; // bit 23
  UINT32    Voltage33     : 1; // bit 24
  UINT32    Voltage30     : 1; // bit 25
  UINT32    Voltage18     : 1; // bit 26
  UINT32    SysBus64V4    : 1; // bit 27
  UINT32    SysBus64V3    : 1; // bit 28
  UINT32    AsyncInt      : 1; // bit 29
  UINT32    SlotType      : 2; // bit 30:31
  UINT32    Sdr50         : 1; // bit 32
  UINT32    Sdr104        : 1; // bit 33
  UINT32    Ddr50         : 1; // bit 34
  UINT32    Reserved3     : 1; // bit 35
  UINT32    DriverTypeA   : 1; // bit 36
  UINT32    DriverTypeC   : 1; // bit 37
  UINT32    DriverTypeD   : 1; // bit 38
  UINT32    DriverType4   : 1; // bit 39
  UINT32    TimerCount    : 4; // bit 40:43
  UINT32    Reserved4     : 1; // bit 44
  UINT32    TuningSDR50   : 1; // bit 45
  UINT32    RetuningMod   : 2; // bit 46:47
  UINT32    ClkMultiplier : 8; // bit 48:55
  UINT32    Reserved5     : 7; // bit 56:62
  UINT32    Hs400         : 1; // bit 63
} SD_MMC_HC_SLOT_CAP;

#endif // __BRCMSTB_SDHCI_DXE_H__
