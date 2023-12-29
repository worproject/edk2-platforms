/** @file
 *
 *  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/NonDiscoverableDeviceRegistrationLib.h>

#include <Rp1.h>

typedef struct {
  EFI_PHYSICAL_ADDRESS    Bar;
  UINT32                  ChipId;
} RP1_DEVICE;

STATIC
EFI_STATUS
EFIAPI
Rp1RegisterDwc3Controllers (
  IN  RP1_DEVICE  *This
  )
{
  EFI_STATUS              Status;
  UINTN                   Index;
  EFI_PHYSICAL_ADDRESS    FullBase;

  EFI_PHYSICAL_ADDRESS Dwc3Addresses[] = {
    RP1_USBHOST0_BASE, RP1_USBHOST1_BASE
  };

  for (Index = 0; Index < ARRAY_SIZE (Dwc3Addresses); Index++) {
    FullBase = This->Bar + Dwc3Addresses[Index];
    Status = RegisterNonDiscoverableMmioDevice (
                NonDiscoverableDeviceTypeXhci,
                NonDiscoverableDeviceDmaTypeNonCoherent,
                NULL,
                NULL,
                1,
                FullBase,
                RP1_USBHOST_SIZE
                );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR,
        "%a: Failed to register DWC3 controller at 0x%lx. Status=%r\n",
        __func__, FullBase, Status));
      return Status;
    }
  }
  return EFI_SUCCESS;
}

STATIC
VOID
EFIAPI
Rp1RegisterDevices (
  IN  RP1_DEVICE  *This
  )
{
  Rp1RegisterDwc3Controllers (This);
}

STATIC
VOID
EFIAPI
Rp1EnableInterrupts (
  IN  RP1_DEVICE  *This
  )
{
  MmioWrite32 (This->Bar + RP1_PCIE_REG_SET + RP1_PCIE_MSIX_CFG (RP1_INT_USBHOST0_0),
               RP1_PCIE_MSIX_CFG_ENABLE);
  MmioWrite32 (This->Bar + RP1_PCIE_REG_SET + RP1_PCIE_MSIX_CFG (RP1_INT_USBHOST1_0),
               RP1_PCIE_MSIX_CFG_ENABLE);
}

EFI_STATUS
EFIAPI
Rp1BusDxeEntryPoint (
  IN  EFI_HANDLE          ImageHandle,
  IN  EFI_SYSTEM_TABLE    *SystemTable
  )
{
  RP1_DEVICE  Dev;
  Dev.Bar = PcdGet64 (Rp1PciPeripheralsBar);
  Dev.ChipId = MmioRead32 (Dev.Bar + RP1_SYSINFO_BASE);

  DEBUG ((DEBUG_INFO, "RP1 chip id: %x, peripheral BAR at CPU address 0x%lx\n",
          Dev.ChipId, Dev.Bar));

  Rp1RegisterDevices (&Dev);
  Rp1EnableInterrupts (&Dev);

  return EFI_SUCCESS;
}
