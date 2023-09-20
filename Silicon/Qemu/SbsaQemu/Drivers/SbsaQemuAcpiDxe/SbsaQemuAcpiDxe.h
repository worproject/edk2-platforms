/** @file
*  This file is an ACPI driver for the Qemu SBSA platform.
*
*  Copyright (c) Linaro Ltd. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#ifndef SBSAQEMU_ACPI_DXE_H
#define SBSAQEMU_ACPI_DXE_H

typedef struct {
  EFI_ACPI_6_0_IO_REMAPPING_ITS_NODE        Node;
  UINT32                                    Identifiers;
} SBSA_EFI_ACPI_6_0_IO_REMAPPING_ITS_NODE;

typedef struct
{
  EFI_ACPI_6_0_IO_REMAPPING_SMMU3_NODE     SmmuNode;
  EFI_ACPI_6_0_IO_REMAPPING_ID_TABLE       SmmuIdMap;
} SBSA_EFI_ACPI_6_0_IO_REMAPPING_SMMU3_NODE;

typedef struct
{
  EFI_ACPI_6_0_IO_REMAPPING_RC_NODE        RcNode;
  EFI_ACPI_6_0_IO_REMAPPING_ID_TABLE       RcIdMap;
} SBSA_EFI_ACPI_6_0_IO_REMAPPING_RC_NODE;

typedef struct {
  EFI_ACPI_6_0_IO_REMAPPING_TABLE           Iort;
  SBSA_EFI_ACPI_6_0_IO_REMAPPING_ITS_NODE   ItsNode;
  SBSA_EFI_ACPI_6_0_IO_REMAPPING_SMMU3_NODE SmmuNode;
  SBSA_EFI_ACPI_6_0_IO_REMAPPING_RC_NODE    RcNode;
} SBSA_IO_REMAPPING_STRUCTURE;

#endif
