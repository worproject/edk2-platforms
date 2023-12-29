/** @file
 *
 *  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
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

#endif // __RPI_PLATFORM_VARSTORE_DATA_H__
