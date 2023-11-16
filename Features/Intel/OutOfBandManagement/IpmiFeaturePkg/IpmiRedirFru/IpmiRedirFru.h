/** @file
  IPMI Redir Sensor functions.

Copyright (c) 2023, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _EFI_IPMI_REDIR_FRU_H_
#define _EFI_IPMI_REDIR_FRU_H_

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>

#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Protocol/RedirFru.h>
#include <Protocol/GenericFru.h>
#include <Protocol/HiiDatabase.h>
#include <Protocol/HiiString.h>
#include <IndustryStandard/SmBios.h>
#include <Protocol/Smbios.h>
#include <IndustryStandard/Ipmi.h>
#include <Library/IpmiBaseLib.h>
#include <Protocol/IpmiTransportProtocol.h>

#define MAX_FRU_SLOT  20

#define IPMI_RDWR_FRU_FRAGMENT_SIZE  0x10

#define CHASSIS_TYPE_LENGTH  1
#define CHASSIS_TYPE_OFFSET  2
#define CHASSIS_PART_NUMBER  3

#define PRODUCT_MFG_OFFSET  3
#define BOARD_MFG_OFFSET    6

#define SMBIOSTYPE1  1
#define SMBIOSTYPE2  2
#define SMBIOSTYPE3  3

#define OFFSET0  0
#define OFFSET1  1
#define OFFSET2  2
#define OFFSET3  3
#define OFFSET4  4
#define OFFSET5  5
#define OFFSET6  6
#define OFFSET7  7
#define OFFSET8  8
#define OFFSET9  9

#define STRING1  1
#define STRING2  2
#define STRING3  3
#define STRING4  4
#define STRING5  5
#define STRING6  6
#define STRING7  7
#define STRING8  8
#define STRING9  9

typedef struct {
  BOOLEAN               Valid;
  IPMI_FRU_DATA_INFO    FruDevice;
} EFI_FRU_DEVICE_INFO;

typedef struct {
  UINTN                        Signature;
  UINT8                        MaxFruSlots;
  UINT8                        NumSlots;
  EFI_FRU_DEVICE_INFO          FruDeviceInfo[MAX_FRU_SLOT];
  EFI_SM_FRU_REDIR_PROTOCOL    IpmiRedirFruProtocol;
} EFI_IPMI_FRU_GLOBAL;

/**
  Get Fru Redir Data.

  @param This
  @param FruSlotNumber
  @param FruDataOffset
  @param FruDataSize
  @param FruData

  EFI_STATUS

**/
EFI_STATUS
EFIAPI
EfiGetFruRedirData (
  IN EFI_SM_FRU_REDIR_PROTOCOL  *This,
  IN UINTN                      FruSlotNumber,
  IN UINTN                      FruDataOffset,
  IN UINTN                      FruDataSize,
  IN UINT8                      *FruData
  );

/**
  This routine install a notify function listen to gEfiEventReadyToBootGuid.

  @param This                        - SM Fru Redir protocol

**/
VOID
GenerateFruSmbiosData (
  IN EFI_SM_FRU_REDIR_PROTOCOL  *This
  );

#define INSTANCE_FROM_EFI_SM_IPMI_FRU_THIS(a) \
  CR (a, \
      EFI_IPMI_FRU_GLOBAL, \
      IpmiRedirFruProtocol, \
      EFI_SM_FRU_REDIR_SIGNATURE \
      )

/**
  Do a one byte IO write.

  @param Address - IO address to write
  @param Data    - Data to write to Address

Returns:
  NONE

**/
VOID
IoWrite8 (
  IN  UINT64  Address,
  IN  UINT8   Data
  );

/**
  Do a one byte IO read.

  @param Address - IO address to read

  @retval Data read

**/
UINT8
IoRead8 (
  IN  UINT64  Address
  );

#endif
