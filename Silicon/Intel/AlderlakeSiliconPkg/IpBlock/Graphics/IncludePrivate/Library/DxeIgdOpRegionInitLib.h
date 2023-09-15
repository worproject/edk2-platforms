/** @file
  This is part of the implementation of an Intel Graphics drivers OpRegion /
  Software SCI interface between system BIOS, ASL code, and Graphics drivers.

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _DXE_IGD_OPREGION_INIT_LIB_H_
#define _DXE_IGD_OPREGION_INIT_LIB_H_

///
/// Statements that include other header files.
///
#include <Uefi.h>
#include <Uefi/UefiBaseType.h>
#include <Library/UefiBootServicesTableLib.h>
#include <IndustryStandard/Pci.h>
#include <Library/ConfigBlockLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/UefiLib.h>
#include <Library/S3BootScriptLib.h>
#include <Register/IgdRegs.h>
#include <SiConfigHob.h>
#include <Register/SaRegsHostBridge.h>
///
/// Driver Consumed Protocol Prototypes
///
#include <Protocol/PciIo.h>
#include <Protocol/PciRootBridgeIo.h>
#include <Protocol/SaPolicy.h>
///
/// Driver Produced Protocol Prototypes
///
#include <Protocol/IgdOpRegion.h>

#pragma pack(push, 1)
///
///
/// OpRegion (Miscellaneous) defines.
///
/// OpRegion Header defines.
///
typedef UINT16  STRING_REF;
///
/// Typedef stuctures
///
typedef struct {
  UINT16  Signature;  /// 0xAA55
  UINT8   Size512;
  UINT8   Reserved[21];
  UINT16  PcirOffset;
  UINT16  VbtOffset;
} INTEL_VBIOS_OPTION_ROM_HEADER;

typedef struct {
  UINT32  Signature;  /// "PCIR"
  UINT16  VendorId;   /// 0x8086
  UINT16  DeviceId;
  UINT16  Reserved0;
  UINT16  Length;
  UINT8   Revision;
  UINT8   ClassCode[3];
  UINT16  ImageLength;
  UINT16  CodeRevision;
  UINT8   CodeType;
  UINT8   Indicator;
  UINT16  Reserved1;
} INTEL_VBIOS_PCIR_STRUCTURE;

typedef struct {
  UINT8   HeaderSignature[20];
  UINT16  HeaderVersion;
  UINT16  HeaderSize;
  UINT16  HeaderVbtSize;
  UINT8   HeaderVbtCheckSum;
  UINT8   HeaderReserved;
  UINT32  HeaderOffsetVbtDataBlock;
  UINT32  HeaderOffsetAim1;
  UINT32  HeaderOffsetAim2;
  UINT32  HeaderOffsetAim3;
  UINT32  HeaderOffsetAim4;
  UINT8   DataHeaderSignature[16];
  UINT16  DataHeaderVersion;
  UINT16  DataHeaderSize;
  UINT16  DataHeaderDataBlockSize;
  UINT8   CoreBlockId;
  UINT16  CoreBlockSize;
  UINT16  CoreBlockBiosSize;
  UINT8   CoreBlockBiosType;
  UINT8   CoreBlockReleaseStatus;
  UINT8   CoreBlockHWSupported;
  UINT8   CoreBlockIntegratedHW;
  UINT8   CoreBlockBiosBuild[4];
  UINT8   CoreBlockBiosSignOn[155];
} VBIOS_VBT_STRUCTURE;
#pragma pack(pop)
///
/// Driver Private Function definitions
///

/**
  Update Graphics OpRegion after PCI enumeration.

  @retval EFI_SUCCESS     - The function completed successfully.
**/
EFI_STATUS
UpdateIgdOpRegionEndOfDxe (
  VOID
  );
#endif
