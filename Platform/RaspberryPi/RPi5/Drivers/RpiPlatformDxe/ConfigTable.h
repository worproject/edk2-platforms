/** @file
 *
 *  Copyright (c) 2023-2024, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#ifndef __RPI_PLATFORM_CONFIG_TABLE_H__
#define __RPI_PLATFORM_CONFIG_TABLE_H__

#include <RpiPlatformVarStoreData.h>

//
// Must match the reserved memory in PlatformLib/RaspberryPiMem.c
//
#define PCI_RESERVED_MEM32_BASE  0xC0000000  // 3 GB
#define PCI_RESERVED_MEM32_SIZE  0x40000000  // 1 GB

#define ACPI_SD_COMPAT_MODE_DEFAULT    ACPI_SD_COMPAT_MODE_BRCMSTB_BAYTRAIL
#define ACPI_SD_LIMIT_UHS_DEFAULT      TRUE

#define ACPI_PCIE_ECAM_COMPAT_MODE_DEFAULT             ACPI_PCIE_ECAM_COMPAT_MODE_NXPMX6_DEN0115
#define ACPI_PCIE_32_BIT_BAR_SPACE_SIZE_MB_MINIMUM     0
#define ACPI_PCIE_32_BIT_BAR_SPACE_SIZE_MB_MAXIMUM     1024 // (PCI_RESERVED_MEM32_SIZE / 1024 / 1024)
#define ACPI_PCIE_32_BIT_BAR_SPACE_SIZE_MB_STEP        1
#define ACPI_PCIE_32_BIT_BAR_SPACE_SIZE_MB_DEFAULT     ACPI_PCIE_32_BIT_BAR_SPACE_SIZE_MB_MINIMUM

#ifndef VFRCOMPILE
VOID
EFIAPI
ApplyConfigTableVariables (
  VOID
  );

VOID
EFIAPI
SetupConfigTableVariables (
  VOID
  );
#endif // VFRCOMPILE

#endif // __RPI_PLATFORM_CONFIG_TABLE_H__
