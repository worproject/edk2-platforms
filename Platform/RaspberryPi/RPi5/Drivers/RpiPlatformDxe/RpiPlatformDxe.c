/** @file
 *
 *  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <Uefi.h>
#include <Guid/RpiPlatformFormSetGuid.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/HiiLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include "ConfigTable.h"
#include "Peripherals.h"

extern UINT8 RpiPlatformDxeHiiBin[];
extern UINT8 RpiPlatformDxeStrings[];

typedef struct {
  VENDOR_DEVICE_PATH VendorDevicePath;
  EFI_DEVICE_PATH_PROTOCOL End;
} HII_VENDOR_DEVICE_PATH;

STATIC HII_VENDOR_DEVICE_PATH mVendorDevicePath = {
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_VENDOR_DP,
      {
        (UINT8)(sizeof (VENDOR_DEVICE_PATH)),
        (UINT8)((sizeof (VENDOR_DEVICE_PATH)) >> 8)
      }
    },
    RPI_PLATFORM_FORMSET_GUID
  },
  {
    END_DEVICE_PATH_TYPE,
    END_ENTIRE_DEVICE_PATH_SUBTYPE,
    {
      (UINT8)(END_DEVICE_PATH_LENGTH),
      (UINT8)((END_DEVICE_PATH_LENGTH) >> 8)
    }
  }
};

STATIC
EFI_STATUS
EFIAPI
InstallHiiPages (
  VOID
  )
{
  EFI_STATUS        Status;
  EFI_HII_HANDLE    HiiHandle;
  EFI_HANDLE        DriverHandle;

  DriverHandle = NULL;
  Status = gBS->InstallMultipleProtocolInterfaces (&DriverHandle,
                  &gEfiDevicePathProtocolGuid,
                  &mVendorDevicePath,
                  NULL);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  HiiHandle = HiiAddPackages (&gRpiPlatformFormSetGuid,
                DriverHandle,
                RpiPlatformDxeStrings,
                RpiPlatformDxeHiiBin,
                NULL);

  if (HiiHandle == NULL) {
    gBS->UninstallMultipleProtocolInterfaces (DriverHandle,
           &gEfiDevicePathProtocolGuid,
           &mVendorDevicePath,
           NULL);
    return EFI_OUT_OF_RESOURCES;
  }
  return EFI_SUCCESS;
}

STATIC
VOID
EFIAPI
SetupVariables (
  VOID
  )
{
  SetupConfigTableVariables ();
}

STATIC
VOID
EFIAPI
ApplyVariables (
  VOID
  )
{
  ApplyConfigTableVariables ();
}

EFI_STATUS
EFIAPI
RpiPlatformDxeEntryPoint (
  IN  EFI_HANDLE          ImageHandle,
  IN  EFI_SYSTEM_TABLE    *SystemTable
  )
{
  EFI_STATUS Status;

  SetupVariables ();
  ApplyVariables ();

  SetupPeripherals ();

  Status = InstallHiiPages ();
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Couldn't install HII pages. Status=%r\n", __func__, Status));
  }

  return EFI_SUCCESS;
}
