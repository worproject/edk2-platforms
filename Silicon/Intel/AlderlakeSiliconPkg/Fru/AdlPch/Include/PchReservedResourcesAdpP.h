/** @file
  PCH preserved MMIO resource definitions.

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _PCH_PRESERVED_RESOURCES_ADP_P_H_
#define _PCH_PRESERVED_RESOURCES_ADP_P_H_

/**
  Detailed recommended static allocation
  +-------------------------------------------------------------------------+
  | PCH preserved MMIO range, 32 MB, from 0xFC800000 to 0xFE7FFFFF          |
  +-------------------------------------------------------------------------+
  | Size        | Start       | End         | Usage                         |
  | 8 MB        | 0xFC800000  | 0xFCFFFFFF  | TraceHub SW BAR               |
  | 16 MB       | 0xFD000000  | 0xFDFFFFFF  | SBREG                         |
  | 64 KB       | 0xFE000000  | 0xFE00FFFF  | PMC MBAR                      |
  | 4 KB        | 0xFE010000  | 0xFE010FFF  | SPI BAR0                      |
  | 176 KB      | 0xFE020000  | 0xFE04BFFF  | SerialIo BAR in ACPI mode     |
  | 400 KB      | 0xFE04C000  | 0xFE0AFFFF  | Unused                        |
  | 64 KB       | 0xFE0B0000  | 0xFE0BFFFF  | eSPI LGMR BAR                 |
  | 64 KB       | 0xFE0C0000  | 0xFE0CFFFF  | eSPI2 SEGMR BAR               |
  | 192 KB      | 0xFE0D0000  | 0xFE0FFFFF  | Unused                        |
  | 1 MB        | 0xFE100000  | 0xFE1FFFFF  | TraceHub MTB BAR              |
  | 2 MB        | 0xFE200000  | 0xFE3FFFFF  | TraceHub FW BAR               |
  | 2 MB        | 0xFE400000  | 0xFE5FFFFF  | Unused                        |
  | 2 MB        | 0xFE600000  | 0xFE7FFFFF  | Temp address                  |
  +-------------------------------------------------------------------------+
**/
#define PCH_PCR_BASE_ADDRESS            0xFD000000     ///< SBREG MMIO base address
#define PCH_PWRM_BASE_ADDRESS           0xFE000000     ///< PMC MBAR MMIO base address
#define PCH_SPI_BASE_ADDRESS            0xFE010000     ///< SPI BAR0 MMIO base address

#endif // _PCH_PRESERVED_RESOURCES_ADP_P_H_

