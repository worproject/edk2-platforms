/** @file
  This file provides SMBIOS Type.

  Based on files under Nt32Pkg/MiscSubClassPlatformDxe/

  Copyright (c) 2023, Ampere Computing LLC. All rights reserved.<BR>
  Copyright (c) 2021, NUVIA Inc. All rights reserved.<BR>
  Copyright (c) 2006 - 2011, Intel Corporation. All rights reserved.<BR>
  Copyright (c) 2015, Hisilicon Limited. All rights reserved.<BR>
  Copyright (c) 2015, Linaro Limited. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "SmbiosPlatformDxe.h"

SMBIOS_PLATFORM_DXE_TABLE_EXTERNS (
  SMBIOS_TABLE_TYPE8,
  PlatformPortConnector
  )
SMBIOS_PLATFORM_DXE_TABLE_EXTERNS (
  SMBIOS_TABLE_TYPE9,
  PlatformSystemSlot
  )
SMBIOS_PLATFORM_DXE_TABLE_EXTERNS (
  SMBIOS_TABLE_TYPE9,
  PlatformOemString
  )
SMBIOS_PLATFORM_DXE_TABLE_EXTERNS (
  SMBIOS_TABLE_TYPE24,
  PlatformHardwareSecurity
  )
SMBIOS_PLATFORM_DXE_TABLE_EXTERNS (
  SMBIOS_TABLE_TYPE38,
  PlatformIpmiDevice
  )
SMBIOS_PLATFORM_DXE_TABLE_EXTERNS (
  SMBIOS_TABLE_TYPE41,
  PlatformOnboardDevicesExtended
  )

SMBIOS_PLATFORM_DXE_DATA_TABLE mSmbiosPlatformDxeDataTable[] = {
  // Type8
  SMBIOS_PLATFORM_DXE_TABLE_ENTRY_DATA_AND_FUNCTION (
    PlatformPortConnector
  ),
  // Type9
  SMBIOS_PLATFORM_DXE_TABLE_ENTRY_DATA_AND_FUNCTION (
    PlatformSystemSlot
  ),
  // Type11
  SMBIOS_PLATFORM_DXE_TABLE_ENTRY_DATA_AND_FUNCTION (
    PlatformOemString
  ),
  // Type24
  SMBIOS_PLATFORM_DXE_TABLE_ENTRY_DATA_AND_FUNCTION (
    PlatformHardwareSecurity
  ),
  // Type38
  SMBIOS_PLATFORM_DXE_TABLE_ENTRY_DATA_AND_FUNCTION (
    PlatformIpmiDevice
  ),
  // Type41
  SMBIOS_PLATFORM_DXE_TABLE_ENTRY_DATA_AND_FUNCTION (
    PlatformOnboardDevicesExtended
  )
};

//
// Number of Data Table entries.
//
UINTN mSmbiosPlatformDxeDataTableEntries = ARRAY_SIZE (mSmbiosPlatformDxeDataTable);
