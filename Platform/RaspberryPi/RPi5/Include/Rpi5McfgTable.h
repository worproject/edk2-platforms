/** @file
 *
 *  Copyright (c) 2024, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#ifndef __RPI5_MCFG_TABLE_H__
#define __RPI5_MCFG_TABLE_H__

#include <IndustryStandard/Bcm2712.h>
#include <IndustryStandard/Bcm2712Pcie.h>
#include <IndustryStandard/MemoryMappedConfigurationSpaceAccessTable.h>

#pragma pack(push, 1)
typedef struct {
  EFI_ACPI_MEMORY_MAPPED_CONFIGURATION_BASE_ADDRESS_TABLE_HEADER                           Header;
  EFI_ACPI_MEMORY_MAPPED_ENHANCED_CONFIGURATION_SPACE_BASE_ADDRESS_ALLOCATION_STRUCTURE    Entries[BCM2712_BRCMSTB_PCIE_COUNT];
} RPI5_MCFG_TABLE;
#pragma pack(pop)

#endif // __RPI5_MCFG_TABLE_H__
