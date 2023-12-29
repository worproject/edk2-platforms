/** @file
 *
 *  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#ifndef __BRCMSTB_SDHCI_DEVICE_H__
#define __BRCMSTB_SDHCI_DEVICE_H__

#include <Uefi/UefiBaseType.h>
#include <Protocol/NonDiscoverableDevice.h>
#include <Protocol/SdMmcOverride.h>

#define BRCMSTB_SDHCI_DEVICE_PROTOCOL_GUID \
  { 0xd6c196f9, 0x9c8c, 0x448f, { 0xbd, 0x21, 0xd9, 0x76, 0xa8, 0x3a, 0x82, 0x7f } }

typedef struct _BRCMSTB_SDHCI_DEVICE_PROTOCOL BRCMSTB_SDHCI_DEVICE_PROTOCOL;

typedef
EFI_STATUS
(EFIAPI *BRCMSTB_SDHCI_SET_SIGNALING_VOLTAGE) (
  IN BRCMSTB_SDHCI_DEVICE_PROTOCOL      *This,
  IN SD_MMC_SIGNALING_VOLTAGE           Voltage
  );

struct _BRCMSTB_SDHCI_DEVICE_PROTOCOL {
  //
  // Controller platform info
  //
  EFI_PHYSICAL_ADDRESS                  HostAddress;
  EFI_PHYSICAL_ADDRESS                  CfgAddress;
  NON_DISCOVERABLE_DEVICE_DMA_TYPE      DmaType;

  BOOLEAN                               IsSlotRemovable;

  //
  // Optional callback for setting the signaling voltage via
  // an external regulator.
  //
  BRCMSTB_SDHCI_SET_SIGNALING_VOLTAGE   SetSignalingVoltage;
};

extern EFI_GUID gBrcmStbSdhciDeviceProtocolGuid;

#endif // __BRCMSTB_SDHCI_DEVICE_H__
