/** @file
  This code reads the FRU and publishes the data to SMBIOS Type 1,2,3 tables.

Copyright (c) 2023, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef FRUMAXSTRING
#define FRUMAXSTRING  128
#endif

#include "IpmiRedirFru.h"

EFI_SM_FRU_REDIR_PROTOCOL  *mFruRedirProtocol;
EFI_SMBIOS_PROTOCOL        *mSmbiosProtocol;

extern EFI_GUID  gEfiSmbiosProtocolGuid;

/**
  Fru Str Len.

  @param Str.

  @retval Length.

**/
UINT8
FruStrLen (
  IN CHAR8  *Str
  )
{
  UINT8  Length;
  CHAR8  *Ptr;

  Length = 0;

  if (Str != NULL) {
    Ptr = Str;
    while (*Ptr != '\0') {
      Length++;
      Ptr++;
    }
  }

  return Length;
}

/**
  This routine attempts to get a string out of the FRU at the designated offset in the
  buffer pointed to by TempPtr.  String type is ASCII.

  @param Offset    - Offset of string in buffer pointed to by TempPtr, this is updated to the next
                     offset.
  @param TempPtr   - Pointer to a buffer containing the FRU.
  @param StrPtr    - the pointer to a buffer for retrieve the string get from FRU.

**/
VOID
GetStringFromFru (
  IN OUT  UINTN  *Offset,
  IN      UINT8  *TempPtr,
  IN OUT  UINT8  *StrPtr
  )
{
  UINTN  Length;
  UINT8  *SrcStrPtr;

  if ((Offset == NULL) || (TempPtr == NULL)) {
    return;
  }

  Length    = 0x3F & TempPtr[*Offset];
  SrcStrPtr = &TempPtr[*Offset + 1];

  ASSERT (Length < FRUMAXSTRING);
  if (Length >= FRUMAXSTRING) {
    return;
  }

  if (StrPtr != NULL) {
    if (Length > 0) {
      CopyMem (StrPtr, SrcStrPtr, Length);
      StrPtr[Length] = '\0';
    } else {
      StrPtr[0] = '\0';
    }
  }

  *Offset = *Offset + Length + 1;
  return;
}

/**
  This routine gets the FRU info area specified by the offset and returns it in
  an allocated buffer.  It is the caller's responsibility to free the buffer.

  @param This      - SM Fru Redir protocol.
  @param Offset    - Info Area starting offset in multiples of 8 bytes.

  @retval Buffer with FruInfo data or NULL if not found.

**/
UINT8 *
GetFruInfoArea (
  IN  EFI_SM_FRU_REDIR_PROTOCOL  *This,
  IN  UINTN                      Offset
  )
{
  EFI_STATUS  Status;
  UINT8       *TempPtr;
  UINTN       Length;

  TempPtr = NULL;

  Offset = Offset * 8;
  if (Offset > 0) {
    //
    // Get Info area length, which is in multiples of 8 bytes
    //
    Length = 0;
    Status = EfiGetFruRedirData (This, 0, (Offset + 1), 1, (UINT8 *)&Length);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "EfiGetFruRedirData returned status %r\n", Status));
      return NULL;
    }

    Length = Length * 8;

    if (Length > 0) {
      TempPtr = AllocateRuntimePool (Length);
      ASSERT (TempPtr != NULL);
      if (TempPtr == NULL) {
        return NULL;
      }

      Status = EfiGetFruRedirData (This, 0, Offset, Length, TempPtr);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "EfiGetFruRedirData returned status %r\n", Status));
        return NULL;
      }
    }
  }

  return TempPtr;
}

/**
  Type1,2,3 only has one instance in SMBIOS tables for each.

  @param TypeNo    - The number of SMBIOS TYPE.

  @retval Record the pointer of SMBIOS TYPE structure.

**/
UINT8 *
GetStructureByTypeNo (
  IN  UINT16  TypeNo
  )
{
  EFI_STATUS               Status;
  EFI_SMBIOS_HANDLE        SmbiosHandle;
  EFI_SMBIOS_TABLE_HEADER  *Record;

  SmbiosHandle = 0;

  Status = mSmbiosProtocol->GetNext (
                              mSmbiosProtocol,
                              &SmbiosHandle,
                              (EFI_SMBIOS_TYPE *)&TypeNo,
                              &Record,
                              NULL
                              );
  if (EFI_ERROR (Status)) {
    return NULL;
  }

  return (UINT8 *)Record;
}

/**
  Smbios Check Sum.

  @param ChecksumSrc.
  @param length.

  @retval Record the pointer of SMBIOS TYPE structure.

**/
UINT8
SmbiosCheckSum (
  IN UINT8  *ChecksumSrc,
  IN UINT8  length
  )
{
  UINT8  Checksum;
  UINT8  Num;

  Checksum = 0;

  for (Num = 0; Num < length; Num++) {
    Checksum = Checksum + *ChecksumSrc++;
  }

  return (0 - Checksum);
}

/**
  Dynamic Update Type.

  @param TypeNo.
  @param StringNo.
  @param Data.

**/
VOID
DynamicUpdateType (
  IN  UINT16  TypeNo,
  IN  UINTN   StringNo,
  IN  UINT8   *Data
  )
{
  EFI_STATUS               Status;
  EFI_SMBIOS_HANDLE        SmbiosHandle;
  EFI_SMBIOS_TABLE_HEADER  *Record;

  SmbiosHandle = 0;

  Status = mSmbiosProtocol->GetNext (
                              mSmbiosProtocol,
                              &SmbiosHandle,
                              (EFI_SMBIOS_TYPE *)&TypeNo,
                              &Record,
                              NULL
                              );
  if (EFI_ERROR (Status)) {
    return;
  }

  mSmbiosProtocol->UpdateString (
                     mSmbiosProtocol,
                     &SmbiosHandle,
                     &StringNo,
                     (CHAR8 *)Data
                     );
}

/**
  This routine is notified by protocol gEfiEventReadyToBootGuid, and reads
  strings out of the FRU and populates them into the appropriate Smbios tables.

  @param Event.
  @param Context.

**/
VOID
EFIAPI
GenerateFruSmbiosType123DataNotified (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_STATUS  Status;

  UINT8                   *FruHdrPtr;
  UINT8                   FruHdrChksum;
  IPMI_FRU_COMMON_HEADER  FruCommonHeader;
  UINT8                   Num;
  UINTN                   Offset;
  UINT8                   *TempPtr;
  UINT8                   TempStr[FRUMAXSTRING];

  UINT8  *TablePtr;

  DEBUG ((DEBUG_INFO, "[FRU SMBIOS]: Generate Fru Smbios Type 1,2,3 Data Notified.\n"));
  gBS->CloseEvent (Event);

  SetMem ((UINT8 *)(&FruCommonHeader), sizeof (IPMI_FRU_COMMON_HEADER), 0);
  SetMem (TempStr, FRUMAXSTRING, 0);

  Status = gBS->LocateProtocol (&gEfiSmbiosProtocolGuid, NULL, (VOID **)&mSmbiosProtocol);
  ASSERT_EFI_ERROR (Status);
  if (Status != EFI_SUCCESS) {
    return;
  }

  Status = EfiGetFruRedirData (mFruRedirProtocol, 0, 0, sizeof (IPMI_FRU_COMMON_HEADER), (UINT8 *)&FruCommonHeader);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "EfiGetFruRedirData returned status %r\n", Status));
    return;
  } else {
    //
    // Do a validity check on the FRU header, since it may be all 0xFF(s) if
    // there is no FRU programmed on the system.
    //
    FruHdrPtr = (UINT8 *)&FruCommonHeader;
    for (Num = 0, FruHdrChksum = 0; Num < sizeof (FruCommonHeader); Num++) {
      FruHdrChksum = (UINT8)(FruHdrChksum +*FruHdrPtr++);
    }

    if (FruHdrChksum != 0) {
      DEBUG ((DEBUG_ERROR, "FRU header invalid.\n"));
      //
      //  The FRU information is bad so nothing need to do.
      //
      return;
    }
  }

  //
  // SMBIOS Type 1, Product data
  //
  TempPtr = GetFruInfoArea (mFruRedirProtocol, FruCommonHeader.ProductInfoStartingOffset);
  if (TempPtr != NULL) {
    //
    // Get the following fields in the specified order.  DO NOT change this order unless the FRU file definition
    // changes.  Offset is initialized and then is incremented to the next field offset in GetStringFromFru.
    //
    // Product Manufacturer
    // Product Name
    // Product Model Number / System Part Number
    // Product Version
    // Product Serial Number
    // Product Asset Tag
    //
    Offset = PRODUCT_MFG_OFFSET;
    GetStringFromFru (&Offset, TempPtr, TempStr);    // MiscSystemManufacturer.SystemManufacturer
    if (FruStrLen ((CHAR8 *)(TempStr)) != 0) {
      DynamicUpdateType (SMBIOSTYPE1, STRING1, TempStr);
    }

    GetStringFromFru (&Offset, TempPtr, TempStr);    // MiscSystemManufacturer.SystemProductName
    if (FruStrLen ((CHAR8 *)(TempStr)) != 0) {
      DynamicUpdateType (SMBIOSTYPE1, STRING2, TempStr);
    }

    GetStringFromFru (&Offset, TempPtr, TempStr);    // ***********************SystemPartNum

    GetStringFromFru (&Offset, TempPtr, TempStr);    // MiscSystemManufacturer.SystemVersion
    if (FruStrLen ((CHAR8 *)(TempStr)) != 0) {
      DynamicUpdateType (SMBIOSTYPE1, STRING3, TempStr);
    }

    GetStringFromFru (&Offset, TempPtr, TempStr);    // MiscSystemManufacturer.SystemSerialNumber
    if (FruStrLen ((CHAR8 *)(TempStr)) != 0) {
      DynamicUpdateType (SMBIOSTYPE1, STRING4, TempStr);
    }

    GetStringFromFru (&Offset, TempPtr, TempStr);    // ***********************AssetTag
    if (FruStrLen ((CHAR8 *)(TempStr)) != 0) {
      DynamicUpdateType (SMBIOSTYPE3, STRING4, TempStr); // NOTICE: this Asset Tag can be used by type 3 table
    }

    FreePool (TempPtr);
  }

  //
  // SMBIOS Type 2, Base Board data
  //
  TempPtr = GetFruInfoArea (mFruRedirProtocol, FruCommonHeader.BoardAreaStartingOffset);
  if (TempPtr != NULL) {
    //
    // Get the following fields in the specified order.  DO NOT change this order unless the FRU file definition
    // changes.  Offset is initialized and then is incremented to the next field offset in GetStringFromFru.
    //
    // Board Manufacturer
    // Board Product Name
    // Board Serial Number
    // Board Part Number
    // FRU Version Number
    //
    Offset = BOARD_MFG_OFFSET;
    GetStringFromFru (&Offset, TempPtr, TempStr);  // MiscBaseBoardManufacturer.BaseBoardManufacturer
    if (FruStrLen ((CHAR8 *)(TempStr)) != 0) {
      DynamicUpdateType (SMBIOSTYPE2, STRING1, TempStr);
    }

    GetStringFromFru (&Offset, TempPtr, TempStr);  // MiscBaseBoardManufacturer.BaseBoardProductName
    if (FruStrLen ((CHAR8 *)(TempStr)) != 0) {
      DynamicUpdateType (SMBIOSTYPE2, STRING2, TempStr);
    }

    GetStringFromFru (&Offset, TempPtr, TempStr);  // MiscBaseBoardManufacturer.BaseBoardSerialNumber
    if (FruStrLen ((CHAR8 *)(TempStr)) != 0) {
      DynamicUpdateType (SMBIOSTYPE2, STRING4, TempStr);
    }

    GetStringFromFru (&Offset, TempPtr, TempStr);  // MiscBaseBoardManufacturer.BoardPartNumber
    if (FruStrLen ((CHAR8 *)(TempStr)) != 0) {
      DynamicUpdateType (SMBIOSTYPE2, STRING3, TempStr);
    }

    GetStringFromFru (&Offset, TempPtr, TempStr);  // **************************FRU Version Number
    if (FruStrLen ((CHAR8 *)(TempStr)) != 0) {
      DynamicUpdateType (SMBIOSTYPE2, STRING3, TempStr);
    }

    FreePool (TempPtr);
  }

  //
  // SMBIOS Type 3, Chassis data
  //
  TempPtr = GetFruInfoArea (mFruRedirProtocol, FruCommonHeader.ChassisInfoStartingOffset);
  if (TempPtr != NULL) {
    // special process:
    TablePtr = GetStructureByTypeNo (SMBIOSTYPE3);
    ASSERT (TablePtr != NULL);
    if (TablePtr == NULL) {
      return;
    }

    ((SMBIOS_TABLE_TYPE3 *)TablePtr)->Type = TempPtr[CHASSIS_TYPE_OFFSET];
    //
    // Get the following fields in the specified order.  DO NOT change this order unless the FRU file definition
    // changes.  Offset is initialized and then is incremented to the next field offset in GetStringFromFru.
    //
    Offset = CHASSIS_PART_NUMBER;
    GetStringFromFru (&Offset, TempPtr, TempStr);  // MiscChassisManufacturer.ChassisVersion
    if (FruStrLen ((CHAR8 *)(TempStr)) != 0) {
      DynamicUpdateType (SMBIOSTYPE3, STRING2, TempStr);
    }

    GetStringFromFru (&Offset, TempPtr, TempStr);  // MiscChassisManufacturer.ChassisSerialNumber
    if (FruStrLen ((CHAR8 *)(TempStr)) != 0) {
      DynamicUpdateType (SMBIOSTYPE3, STRING3, TempStr);
    }

    GetStringFromFru (&Offset, TempPtr, TempStr);  // MiscChassisManufacturer.ChassisManufacturer
    if (FruStrLen ((CHAR8 *)(TempStr)) != 0) {
      DynamicUpdateType (SMBIOSTYPE3, STRING1, TempStr);
    }

    FreePool (TempPtr);
  }

  return;
}

/**
  This routine install a notify function listen to ReadyToBoot Event.

  @param This                        - SM Fru Redir protocol.

**/
VOID
GenerateFruSmbiosData (
  IN EFI_SM_FRU_REDIR_PROTOCOL  *This
  )
{
  EFI_EVENT   Event;
  EFI_STATUS  Status;

  mFruRedirProtocol = This;

  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  GenerateFruSmbiosType123DataNotified,
                  NULL,
                  &gBdsEventAfterConsoleReadyBeforeBootOptionGuid,
                  &Event
                  );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "GenerateFruSmbiosData(): Create AfterConsole event failed with return value: %r\n", Status));
  }

  return;
}
