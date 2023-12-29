/** @file
 *
 *  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
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
#include <Protocol/BrcmStbSdhciDevice.h>

#include "Peripherals.h"

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

EFI_STATUS
EFIAPI
SetupPeripherals (
  VOID
  )
{
  InitGpioPinctrls ();

  RegisterSdControllers ();

  return EFI_SUCCESS;
}
