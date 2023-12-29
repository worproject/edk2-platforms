/** @file
 *
 *  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/NonDiscoverableDeviceRegistrationLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Protocol/BrcmStbSdhciDevice.h>
#include <Protocol/NonDiscoverableDevice.h>
#include <Protocol/SdMmcOverride.h>

#include "BrcmStbSdhciDxe.h"

STATIC
EFI_STATUS
EFIAPI
SdMmcCapability (
  IN      EFI_HANDLE                      ControllerHandle,
  IN      UINT8                           Slot,
  IN OUT  VOID                            *SdMmcHcSlotCapability,
  IN OUT  UINT32                          *BaseClkFreq
  )
{
  SD_MMC_HC_SLOT_CAP     *Capability;

  if (Slot != 0) {
    return EFI_UNSUPPORTED;
  }
  if (SdMmcHcSlotCapability == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Capability = SdMmcHcSlotCapability;

  // Hardware retuning is not supported.
  Capability->RetuningMod = 0;

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
EFIAPI
SdMmcNotifyPhase (
  IN      EFI_HANDLE                      ControllerHandle,
  IN      UINT8                           Slot,
  IN      EDKII_SD_MMC_PHASE_TYPE         PhaseType,
  IN OUT  VOID                            *PhaseData
  )
{
  EFI_STATUS                        Status;
  BRCMSTB_SDHCI_DEVICE_PROTOCOL     *Device;

  if (Slot != 0) {
    return EFI_UNSUPPORTED;
  }

  Status = gBS->HandleProtocol (
                  ControllerHandle,
                  &gBrcmStbSdhciDeviceProtocolGuid,
                  (VOID **)&Device);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Failed to get protocol. Status=%r\n",
            __func__, Status));
    return EFI_UNSUPPORTED;
  }

  switch (PhaseType) {
    case EdkiiSdMmcSetSignalingVoltage:
      if (PhaseData == NULL) {
        return EFI_INVALID_PARAMETER;
      }
      if (Device->SetSignalingVoltage != NULL) {
        return Device->SetSignalingVoltage (Device, *(SD_MMC_SIGNALING_VOLTAGE *)PhaseData);
      }
      break;

    default:
      break;
  }

  return EFI_SUCCESS;
}

STATIC EDKII_SD_MMC_OVERRIDE mSdMmcOverride = {
  EDKII_SD_MMC_OVERRIDE_PROTOCOL_VERSION,
  SdMmcCapability,
  SdMmcNotifyPhase,
};

STATIC
EFI_STATUS
EFIAPI
StartDevice (
  IN BRCMSTB_SDHCI_DEVICE_PROTOCOL    *This,
  IN EFI_HANDLE                       ControllerHandle
  )
{
  EFI_STATUS Status;

  //
  // Set the PHY DLL as clock source to support higher speed modes
  // reliably.
  //
  MmioAndThenOr32 (This->CfgAddress + SDIO_CFG_MAX_50MHZ_MODE,
                   ~SDIO_CFG_MAX_50MHZ_MODE_ENABLE,
                   SDIO_CFG_MAX_50MHZ_MODE_STRAP_OVERRIDE);

  if (This->IsSlotRemovable) {
    MmioAndThenOr32 (This->CfgAddress + SDIO_CFG_SD_PIN_SEL,
                     ~SDIO_CFG_SD_PIN_SEL_MASK,
                     SDIO_CFG_SD_PIN_SEL_CARD);
  } else {
    MmioAndThenOr32 (This->CfgAddress + SDIO_CFG_CTRL,
                     ~SDIO_CFG_CTRL_SDCD_N_TEST_LEV,
                     SDIO_CFG_CTRL_SDCD_N_TEST_EN);
  }

  Status = RegisterNonDiscoverableMmioDevice (
              NonDiscoverableDeviceTypeSdhci,
              This->DmaType,
              NULL,
              &ControllerHandle,
              1,
              This->HostAddress, SDIO_HOST_SIZE);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR,
      "%a: Failed to register Broadcom STB SDHCI controller at 0x%lx. Status=%r\n",
      __func__, This->HostAddress, Status));
    return Status;
  }

  return Status;
}

STATIC VOID  *mProtocolInstallEventRegistration;

STATIC
VOID
EFIAPI
NotifyProtocolInstall (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
  EFI_STATUS                          Status;
  EFI_HANDLE                          Handle;
  UINTN                               BufferSize;
  BRCMSTB_SDHCI_DEVICE_PROTOCOL       *Device;

  while (TRUE) {
    BufferSize = sizeof (EFI_HANDLE);
    Status = gBS->LocateHandle (
                    ByRegisterNotify,
                    NULL,
                    mProtocolInstallEventRegistration,
                    &BufferSize,
                    &Handle);
    if (EFI_ERROR (Status)) {
      if (Status != EFI_NOT_FOUND) {
        DEBUG ((DEBUG_ERROR, "%a: Failed to locate protocol. Status=%r\n",
                __func__, Status));
      }
      break;
    }

    Status = gBS->HandleProtocol (
                    Handle,
                    &gBrcmStbSdhciDeviceProtocolGuid,
                    (VOID **)&Device);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a: Failed to get protocol. Status=%r\n",
              __func__, Status));
      break;
    }

    Status = StartDevice (Device, Handle);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a: Failed to start device. Status=%r\n",
              __func__, Status));
      break;
    }
  }
}

EFI_STATUS
EFIAPI
BrcmStbSdhciDxeInitialize (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS     Status;
  EFI_HANDLE     Handle;
  EFI_EVENT      ProtocolInstallEvent;

  ProtocolInstallEvent = EfiCreateProtocolNotifyEvent (
        &gBrcmStbSdhciDeviceProtocolGuid,
        TPL_CALLBACK,
        NotifyProtocolInstall,
        NULL,
        &mProtocolInstallEventRegistration);
  if (ProtocolInstallEvent == NULL) {
    ASSERT (FALSE);
    return EFI_OUT_OF_RESOURCES;
  }

  Handle = NULL;
  Status = gBS->InstallMultipleProtocolInterfaces (
              &Handle,
              &gEdkiiSdMmcOverrideProtocolGuid,
              &mSdMmcOverride,
              NULL);
  ASSERT_EFI_ERROR (Status);

  return Status;
}
