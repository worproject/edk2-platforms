/** @file
 *
 *  Copyright (c) 2023-2024, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#ifndef __RPI_PLATFORM_VARSTORE_DATA_H__
#define __RPI_PLATFORM_VARSTORE_DATA_H__

#define ACPI_SD_COMPAT_MODE_BRCMSTB_BAYTRAIL        0
#define ACPI_SD_COMPAT_MODE_FULL_BAYTRAIL           1
typedef struct {
  UINT8 Value;
} ACPI_SD_COMPAT_MODE_VARSTORE_DATA;

typedef struct {
  BOOLEAN Value;
} ACPI_SD_LIMIT_UHS_VARSTORE_DATA;

#define ACPI_PCIE_ECAM_COMPAT_MODE_DEN0115                  0x00000001
#define ACPI_PCIE_ECAM_COMPAT_MODE_NXPMX6                   0x00000002
#define ACPI_PCIE_ECAM_COMPAT_MODE_GRAVITON                 0x00000004
#define ACPI_PCIE_ECAM_COMPAT_MODE_NXPMX6_DEN0115           0x00000003
#define ACPI_PCIE_ECAM_COMPAT_MODE_NXPMX6_GRAVITON          0x00000006
typedef struct {
  UINT32 Value;
} ACPI_PCIE_ECAM_COMPAT_MODE_VARSTORE_DATA;

typedef struct {
  UINT32 Value;
} ACPI_PCIE_32_BIT_BAR_SPACE_SIZE_MB_VARSTORE_DATA;

#endif // __RPI_PLATFORM_VARSTORE_DATA_H__
