/** @file
  The GUID definition for SaDataHob

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _SA_DATA_HOB_H_
#define _SA_DATA_HOB_H_

#include <Base.h>
#include <CpuPcieInfo.h>
#include <Library/PcdLib.h>

extern EFI_GUID gSaDataHobGuid;
#pragma pack (push,1)

///
/// System Agent Data Hob
///
typedef struct {
  EFI_HOB_GUID_TYPE        EfiHobGuidType;                       ///< GUID Hob type structure for gSaDataHobGuid
  UINT8                    PrimaryDisplay;
  BOOLEAN                  ResizableBarSupport;                  ///< Resizable BAR Support
  UINT8                    Rsvd1[2];                             ///< Reserved for future use
} SA_DATA_HOB;

#pragma pack (pop)
#endif
