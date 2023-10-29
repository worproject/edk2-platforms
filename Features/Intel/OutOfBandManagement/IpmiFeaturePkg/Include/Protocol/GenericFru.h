/** @file
  Generic FRU Protocol as defined in Tiano
  This code abstracts the generic FRU Protocol

Copyright (c) 2023, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _GENERIC_FRU_H_
#define _GENERIC_FRU_H_

#define EFI_SM_FRU_PROTOCOL_GUID \
  { \
    0xc8eebf0e, 0xe10, 0x47f7, 0x81, 0xbd, 0x39, 0xdb, 0x75, 0xca, 0x93, 0x9f \
  }

typedef struct _EFI_SM_FRU_PROTOCOL EFI_SM_FRU_PROTOCOL;

//
// Common FRU Types
//
#define EFI_CPU_TYPE_FRU_GUID \
  { \
    0xf064c91f, 0x188c, 0x4f56, 0xb7, 0xfd, 0x30, 0xa9, 0xb8, 0x6a, 0x29, 0xf3 \
  }

#define EFI_MEMORY_TYPE_FRU_GUID \
  { \
    0xd50234f4, 0x6f4b, 0x43e8, 0xa0, 0x13, 0x3c, 0x1e, 0x33, 0xd9, 0xb9, 0xb1 \
  }

#define EFI_SYSTEM_TYPE_FRU_GUID \
  { \
    0xaab16018, 0x679d, 0x4461, 0xba, 0x20, 0xe7, 0xc, 0xf7, 0x86, 0x6a, 0x9b \
  }

//
// Common FRU Formats.
//
#define EFI_IPMI_FORMAT_FRU_GUID \
  { \
    0x3531fdc6, 0xeae, 0x4cd2, 0xb0, 0xa6, 0x5f, 0x48, 0xa0, 0xdf, 0xe3, 0x8 \
  }

#define EFI_DMI_FORMAT_FRU_GUID \
  { \
    0x67ef7a73, 0x2594, 0x4a5e, 0x93, 0xa, 0xe1, 0x66, 0xfa, 0xbc, 0xd2, 0xc8 \
  }

#define EFI_INTEL_MFG_FORMAT_FRU_GUID \
  { \
    0x79e8c9c7, 0x1152, 0x4f00, 0xb8, 0x31, 0x14, 0xf1, 0xc4, 0x4, 0x1a, 0xe0 \
  }

//
//  Generic FRU Function Prototypes
//
typedef
EFI_STATUS
(EFIAPI *EFI_GET_FRU_INFO)(
  IN EFI_SM_FRU_PROTOCOL              *This,
  IN EFI_GUID                         *FruTypeGuid,
  IN UINTN                            FruInstance,
  OUT EFI_GUID                        *FruFormatGuid,
  OUT UINTN                           *DataAccessGranularity,
  OUT CHAR16                          **FruInformationString
  );

typedef
EFI_STATUS
(EFIAPI *EFI_GET_FRU_DATA)(
  IN EFI_SM_FRU_PROTOCOL              *This,
  IN EFI_GUID                         *FruTypeGuid,
  IN  UINTN                           FruInstance,
  IN  UINTN                           FruDataOffset,
  IN  UINTN                           FruDataSize,
  IN  UINT8                           *FruData
  );

typedef
EFI_STATUS
(EFIAPI *EFI_SET_FRU_DATA)(
  IN EFI_SM_FRU_PROTOCOL              *This,
  IN EFI_GUID                         *FruTypeGuid,
  IN  UINTN                           FruInstance,
  IN  UINTN                           FruDataOffset,
  IN  UINTN                           FruDataSize,
  IN  UINT8                           *FruData
  );

//
// GENERIC FRU PROTOCOL
//
typedef struct _EFI_SM_FRU_PROTOCOL {
  EFI_GET_FRU_INFO    GetFruInfo;
  EFI_GET_FRU_DATA    GetFruData;
  EFI_SET_FRU_DATA    SetFruData;
} EFI_SM_FRU_PROTOCOL;

extern EFI_GUID  gEfiGenericFruProtocolGuid;

#endif
