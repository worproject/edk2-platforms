/** @file
 *
 *  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#ifndef __ACPITABLES_H__
#define __ACPITABLES_H__

#include <IndustryStandard/Acpi.h>
#include <IndustryStandard/Bcm2712.h>
#include <Library/PcdLib.h>

#define EFI_ACPI_OEM_ID                       {'R','P','I','F','D','N'}
#define EFI_ACPI_OEM_TABLE_ID                 SIGNATURE_64 ('R','P','I','5',' ',' ',' ',' ')
#define EFI_ACPI_OEM_REVISION                 0x00000200
#define EFI_ACPI_CREATOR_ID                   SIGNATURE_32 ('E','D','K','2')
#define EFI_ACPI_CREATOR_REVISION             0x00000300

//
// A macro to initialise the common header part of EFI ACPI tables as defined by
// EFI_ACPI_DESCRIPTION_HEADER structure.
//
#define ACPI_HEADER(Signature, Type, Revision) {                  \
    Signature,                      /* UINT32  Signature */       \
    sizeof (Type),                  /* UINT32  Length */          \
    Revision,                       /* UINT8   Revision */        \
    0,                              /* UINT8   Checksum */        \
    EFI_ACPI_OEM_ID,                /* UINT8   OemId[6] */        \
    EFI_ACPI_OEM_TABLE_ID,          /* UINT64  OemTableId */      \
    EFI_ACPI_OEM_REVISION,          /* UINT32  OemRevision */     \
    EFI_ACPI_CREATOR_ID,            /* UINT32  CreatorId */       \
    EFI_ACPI_CREATOR_REVISION       /* UINT32  CreatorRevision */ \
  }

//
// Device resource helpers
//
#define QWORDMEMORY_BUF(Index, ResourceType)                    \
  QWordMemory (ResourceType,,                                   \
    MinFixed, MaxFixed, NonCacheable, ReadWrite,                \
    0x0, 0x0, 0x0, 0x0, 0x1,,, RB ## Index)

#define QWORDMEMORY_SET(Index, Minimum, Length)                 \
  CreateQwordField (RBUF, RB ## Index._MIN, MI ## Index)        \
  CreateQwordField (RBUF, RB ## Index._MAX, MA ## Index)        \
  CreateQwordField (RBUF, RB ## Index._LEN, LE ## Index)        \
  LE ## Index = Length                                          \
  MI ## Index = Minimum                                         \
  MA ## Index = MI ## Index + LE ## Index - 1

//
// PL011 Debug UART Port
//
#define PL011_DEBUG_BASE_ADDRESS                FixedPcdGet64 (PcdSerialRegisterBase)
#define PL011_DEBUG_INTERRUPT                   FixedPcdGet32 (PL011UartInterrupt)
#define PL011_DEBUG_LENGTH                      BCM2712_PL011_LENGTH
#define PL011_DEBUG_CLOCK_FREQUENCY             FixedPcdGet32 (PL011UartClkInHz)

#endif // __ACPITABLES_H__
