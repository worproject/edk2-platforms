/** @file
 *
 *  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#ifndef __RPI_PLATFORM_CONFIG_TABLE_H__
#define __RPI_PLATFORM_CONFIG_TABLE_H__

#include <RpiPlatformVarStoreData.h>

#define ACPI_SD_COMPAT_MODE_DEFAULT    ACPI_SD_COMPAT_MODE_BRCMSTB_BAYTRAIL
#define ACPI_SD_LIMIT_UHS_DEFAULT      TRUE

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
