/** @file
 *
 *  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <Guid/RpiPlatformFormSetGuid.h>
#include <IndustryStandard/Acpi.h>
#include <Library/AcpiLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Protocol/AcpiSystemDescriptionTable.h>
#include <RpiPlatformVarStoreData.h>
#include <ConfigVars.h>

#include "ConfigTable.h"

//
// AcpiTables.inf
//
STATIC CONST EFI_GUID mAcpiTableFile = {
  0x7E374E25, 0x8E01, 0x4FEE, { 0x87, 0xf2, 0x39, 0x0C, 0x23, 0xC6, 0x06, 0xCD }
};

STATIC ACPI_SD_COMPAT_MODE_VARSTORE_DATA    AcpiSdCompatMode;
STATIC ACPI_SD_LIMIT_UHS_VARSTORE_DATA      AcpiSdLimitUhs;

STATIC
VOID
EFIAPI
DsdtFixupSd (
  IN EFI_ACPI_SDT_PROTOCOL    *AcpiSdtProtocol,
  IN EFI_ACPI_HANDLE          TableHandle
  )
{
  EFI_STATUS Status;

  Status = AcpiAmlObjectUpdateInteger (AcpiSdtProtocol, TableHandle,
                "\\_SB.SDCM", AcpiSdCompatMode.Value);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Failed to patch AcpiSdCompatMode.\n", __func__));
  }

  Status = AcpiAmlObjectUpdateInteger (AcpiSdtProtocol, TableHandle,
                "\\_SB.SDLU", AcpiSdLimitUhs.Value);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Failed to patch AcpiSdLimitUhs.\n", __func__));
  }
}

STATIC
EFI_STATUS
EFIAPI
ApplyDsdtFixups (
  VOID
  )
{
  EFI_STATUS                         Status;
  EFI_ACPI_SDT_PROTOCOL              *AcpiSdtProtocol;
  EFI_ACPI_DESCRIPTION_HEADER        *Table;
  UINTN                              TableKey;
  UINTN                              TableIndex;
  EFI_ACPI_HANDLE                    TableHandle;

  Status = gBS->LocateProtocol (
                  &gEfiAcpiSdtProtocolGuid,
                  NULL,
                  (VOID **)&AcpiSdtProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Couldn't locate gEfiAcpiSdtProtocolGuid!\n", __func__));
    return Status;
  }

  TableIndex = 0;
  Status = AcpiLocateTableBySignature (
             AcpiSdtProtocol,
             EFI_ACPI_6_3_DIFFERENTIATED_SYSTEM_DESCRIPTION_TABLE_SIGNATURE,
             &TableIndex,
             &Table,
             &TableKey);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Couldn't locate ACPI DSDT table!\n", __func__));
    return Status;
  }

  Status = AcpiSdtProtocol->OpenSdt (TableKey, &TableHandle);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Couldn't open ACPI DSDT table!\n", __func__));
    AcpiSdtProtocol->Close (TableHandle);
    return Status;
  }

  DsdtFixupSd (AcpiSdtProtocol, TableHandle);

  AcpiSdtProtocol->Close (TableHandle);
  AcpiUpdateChecksum ((UINT8 *)Table, Table->Length);

  return EFI_SUCCESS;
}

VOID
EFIAPI
ApplyConfigTableVariables (
  VOID
  )
{
  EFI_STATUS Status;

  if (PcdGet32 (PcdSystemTableMode) != SYSTEM_TABLE_MODE_ACPI
      && PcdGet32 (PcdSystemTableMode) != SYSTEM_TABLE_MODE_BOTH) {
    // FDT is taken care of by FdtDxe.
    return;
  }

  Status = LocateAndInstallAcpiFromFvConditional (&mAcpiTableFile, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Failed to install ACPI tables!\n"));
    return;
  }

  Status = ApplyDsdtFixups ();
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Failed to apply ACPI DSDT fixups!\n"));
  }
}

VOID
EFIAPI
SetupConfigTableVariables (
  VOID
  )
{
  EFI_STATUS    Status;
  UINTN         Size;
  UINT32        Var32;

  AcpiSdCompatMode.Value = ACPI_SD_COMPAT_MODE_DEFAULT;
  AcpiSdLimitUhs.Value = ACPI_SD_LIMIT_UHS_DEFAULT;

  Size = sizeof (ACPI_SD_COMPAT_MODE_VARSTORE_DATA);
  Status = gRT->GetVariable (L"AcpiSdCompatMode",
                  &gRpiPlatformFormSetGuid,
                  NULL, &Size, &AcpiSdCompatMode);
  if (EFI_ERROR (Status)) {
    Status = gRT->SetVariable (
                    L"AcpiSdCompatMode",
                    &gRpiPlatformFormSetGuid,
                    EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                    Size,
                    &AcpiSdCompatMode);
    ASSERT_EFI_ERROR (Status);
  }

  Size = sizeof (ACPI_SD_LIMIT_UHS_VARSTORE_DATA);
  Status = gRT->GetVariable (L"AcpiSdLimitUhs",
                  &gRpiPlatformFormSetGuid,
                  NULL, &Size, &AcpiSdLimitUhs);
  if (EFI_ERROR (Status)) {
    Status = gRT->SetVariable (
                    L"AcpiSdLimitUhs",
                    &gRpiPlatformFormSetGuid,
                    EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                    Size,
                    &AcpiSdLimitUhs);
    ASSERT_EFI_ERROR (Status);
  }

  Size = sizeof (UINT32);
  Status = gRT->GetVariable (L"SystemTableMode",
                  &gRpiPlatformFormSetGuid,
                  NULL, &Size, &Var32);
  if (EFI_ERROR (Status)) {
    Status = PcdSet32S (PcdSystemTableMode, PcdGet32 (PcdSystemTableMode));
    ASSERT_EFI_ERROR (Status);
  }
}
