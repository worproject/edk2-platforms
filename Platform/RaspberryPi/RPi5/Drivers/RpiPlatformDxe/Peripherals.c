/** @file
 *
 *  Copyright (c) 2023-2024, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <Uefi.h>
#include <IndustryStandard/Bcm2712.h>
#include <IndustryStandard/Bcm2712Pinctrl.h>
#include <Library/Bcm2712GpioLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Protocol/BrcmStbSdhciDevice.h>

#include "Peripherals.h"
#include "ConfigTable.h"
#include "RpiPlatformDxe.h"

STATIC
EFI_STATUS
EFIAPI
SdControllerSetSignalingVoltage (
  IN BRCMSTB_SDHCI_DEVICE_PROTOCOL      *This,
  IN SD_MMC_SIGNALING_VOLTAGE           Voltage
  )
{
  // sd_io_1v8_reg
  GpioWrite (BCM2712_GIO_AON, 3, Voltage == SdMmcSignalingVoltage18);

  return EFI_SUCCESS;
}

STATIC BRCMSTB_SDHCI_DEVICE_PROTOCOL mSdController = {
  .HostAddress            = BCM2712_BRCMSTB_SDIO1_HOST_BASE,
  .CfgAddress             = BCM2712_BRCMSTB_SDIO1_CFG_BASE,
  .DmaType                = NonDiscoverableDeviceDmaTypeNonCoherent,
  .IsSlotRemovable        = TRUE,
  .SetSignalingVoltage    = SdControllerSetSignalingVoltage
};

STATIC
EFI_STATUS
EFIAPI
RegisterSdControllers (
  VOID
  )
{
  EFI_STATUS  Status;
  EFI_HANDLE  Handle = NULL;

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &Handle,
                  &gBrcmStbSdhciDeviceProtocolGuid,
                  &mSdController,
                  NULL);
  ASSERT_EFI_ERROR (Status);

  return Status;
}

STATIC
EFI_STATUS
EFIAPI
InitGpioPinctrls (
  VOID
  )
{
  // SD card detect
  GpioSetFunction (BCM2712_GIO_AON, 5, GIO_AON_PIN5_ALT_SD_CARD_G);
  GpioSetPull (BCM2712_GIO_AON, 5, BCM2712_GPIO_PIN_PULL_UP);

  // Route SDIO to Wi-Fi
  GpioSetFunction (BCM2712_GIO, 30, GIO_PIN30_ALT_SD2);
  GpioSetPull (BCM2712_GIO, 30, BCM2712_GPIO_PIN_PULL_NONE);
  GpioSetFunction (BCM2712_GIO, 31, GIO_PIN31_ALT_SD2);
  GpioSetPull (BCM2712_GIO, 31, BCM2712_GPIO_PIN_PULL_UP);
  GpioSetFunction (BCM2712_GIO, 32, GIO_PIN32_ALT_SD2);
  GpioSetPull (BCM2712_GIO, 32, BCM2712_GPIO_PIN_PULL_UP);
  GpioSetFunction (BCM2712_GIO, 33, GIO_PIN33_ALT_SD2);
  GpioSetPull (BCM2712_GIO, 33, BCM2712_GPIO_PIN_PULL_UP);
  GpioSetFunction (BCM2712_GIO, 34, GIO_PIN34_ALT_SD2);
  GpioSetPull (BCM2712_GIO, 34, BCM2712_GPIO_PIN_PULL_UP);
  GpioSetFunction (BCM2712_GIO, 35, GIO_PIN35_ALT_SD2);
  GpioSetPull (BCM2712_GIO, 35, BCM2712_GPIO_PIN_PULL_UP);

  // wl_on_reg
  GpioWrite (BCM2712_GIO, 28, TRUE);
  GpioSetDirection (BCM2712_GIO, 28, BCM2712_GPIO_PIN_OUTPUT);

  return EFI_SUCCESS;
}

BCM2712_PCIE_PLATFORM_PROTOCOL  mPciePlatform = {
  .Mem32BusBase = PCI_RESERVED_MEM32_BASE,
  .Mem32Size    = PCI_RESERVED_MEM32_SIZE,

  .Settings         = {
    [1] = { // Connector (configurable)
      .Enabled      = PCIE1_SETTINGS_ENABLED_DEFAULT,
      .MaxLinkSpeed = PCIE1_SETTINGS_MAX_LINK_SPEED_DEFAULT
    },
    [2] = { // RP1 (fixed)
      .Enabled      = TRUE,
      .MaxLinkSpeed = 2,
      .RcbMatchMps  = TRUE,
      .VdmToQosMap  = 0xbbaa9888
    }
  }
};

STATIC
EFI_STATUS
EFIAPI
RegisterPciePlatform (
  VOID
  )
{
  EFI_STATUS  Status;
  EFI_HANDLE  Handle = NULL;

  ASSERT_PROTOCOL_ALREADY_INSTALLED (NULL, &gBcm2712PciePlatformProtocolGuid);
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &Handle,
                  &gBcm2712PciePlatformProtocolGuid,
                  &mPciePlatform,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

  return Status;
}

EFI_STATUS
EFIAPI
SetupPeripherals (
  VOID
  )
{
  InitGpioPinctrls ();

  RegisterSdControllers ();
  RegisterPciePlatform ();

  return EFI_SUCCESS;
}

VOID
EFIAPI
ApplyPeripheralVariables (
  VOID
  )
{
}

VOID
EFIAPI
SetupPeripheralVariables (
  VOID
  )
{
  EFI_STATUS    Status;
  UINTN         Size;

  Size = sizeof (BCM2712_PCIE_CONTROLLER_SETTINGS);
  Status = gRT->GetVariable (L"Pcie1Settings",
                  &gRpiPlatformFormSetGuid,
                  NULL, &Size, &mPciePlatform.Settings[1]);
  if (EFI_ERROR (Status)) {
    Status = gRT->SetVariable (
                    L"Pcie1Settings",
                    &gRpiPlatformFormSetGuid,
                    EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                    Size,
                    &mPciePlatform.Settings[1]);
    ASSERT_EFI_ERROR (Status);
  }
}
