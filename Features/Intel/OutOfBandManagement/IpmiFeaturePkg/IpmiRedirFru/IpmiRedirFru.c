/** @file
  Server Management IPMI Redir FRU Driver. This REDIR FRU driver attaches
  to the Generic FRU driver.

Copyright (c) 2023, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "IpmiRedirFru.h"
#include <Library/DebugLib.h>

EFI_IPMI_FRU_GLOBAL  *mIpmiFruGlobal;

/**
  Get Empty Fru Slot.

  @retval UINT8

**/
UINT8
GetEmptyFruSlot (
  VOID
  )
{
  UINT8  Num;

  for (Num = 1; Num < mIpmiFruGlobal->MaxFruSlots; Num++) {
    if (!mIpmiFruGlobal->FruDeviceInfo[Num].Valid) {
      return Num;
    }
  }

  return 0xFF;
}

/**
  Get Fru Redir Info.

  @param This
  @param FruSlotNumber
  @param FruFormatGuid
  @param DataAccessGranularity
  @param FruInformationString

  @retval EFI_NO_MAPPING
  @retval EFI_SUCCESS

**/
EFI_STATUS
EFIAPI
EfiGetFruRedirInfo (
  IN  EFI_SM_FRU_REDIR_PROTOCOL  *This,
  IN  UINTN                      FruSlotNumber,
  OUT EFI_GUID                   *FruFormatGuid,
  OUT UINTN                      *DataAccessGranularity,
  OUT CHAR16                     **FruInformationString
  )
{
  EFI_IPMI_FRU_GLOBAL  *FruPrivate;

  FruPrivate = INSTANCE_FROM_EFI_SM_IPMI_FRU_THIS (This);

  if ((FruSlotNumber + 1) > FruPrivate->NumSlots) {
    return EFI_NO_MAPPING;
  }

  CopyMem (FruFormatGuid, &gEfiIpmiFormatFruGuid, sizeof (EFI_GUID));

  *DataAccessGranularity = 0x1;
  *FruInformationString  = NULL;

  return EFI_SUCCESS;
}

/**
  Get Fru Slot Info.

  @param This
  @param FruTypeGuid
  @param StartFruSlotNumber
  @param NumSlots

  @retval EFI_SUCCESS

**/
EFI_STATUS
EFIAPI
EfiGetFruSlotInfo (
  IN  EFI_SM_FRU_REDIR_PROTOCOL  *This,
  OUT EFI_GUID                   *FruTypeGuid,
  OUT UINTN                      *StartFruSlotNumber,
  OUT UINTN                      *NumSlots
  )
{
  EFI_IPMI_FRU_GLOBAL  *FruPrivate;

  FruPrivate = NULL;

  FruPrivate = INSTANCE_FROM_EFI_SM_IPMI_FRU_THIS (This);

  CopyMem (FruTypeGuid, &gEfiSystemTypeFruGuid, sizeof (EFI_GUID));
  *StartFruSlotNumber = 0;
  *NumSlots           = FruPrivate->NumSlots;
  return EFI_SUCCESS;
}

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
  )
{
  EFI_IPMI_FRU_GLOBAL          *FruPrivate;
  UINT32                       ResponseDataSize;
  UINT8                        BackupCount;
  UINT8                        PointerOffset;
  UINT8                        DataToCopySize;
  EFI_STATUS                   Status = EFI_SUCCESS;
  IPMI_READ_FRU_DATA_REQUEST   ReadFruDataRequest;
  IPMI_READ_FRU_DATA_RESPONSE  *ReadFruDataResponse;

  FruPrivate    = NULL;
  PointerOffset = 0;

  FruPrivate = INSTANCE_FROM_EFI_SM_IPMI_FRU_THIS (This);

  if ((FruSlotNumber + 1) > FruPrivate->NumSlots) {
    Status = EFI_NO_MAPPING;
    return Status;
  }

  if (FruSlotNumber >= sizeof (FruPrivate->FruDeviceInfo) / sizeof (EFI_FRU_DEVICE_INFO)) {
    Status = EFI_INVALID_PARAMETER;
    return Status;
  }

  if (FruPrivate->FruDeviceInfo[FruSlotNumber].FruDevice.Bits.LogicalFruDevice) {
    //
    // Create the FRU Read Command for the logical FRU Device.
    //
    ReadFruDataRequest.DeviceId        = FruPrivate->FruDeviceInfo[FruSlotNumber].FruDevice.Bits.FruDeviceId;
    ReadFruDataRequest.InventoryOffset = (UINT16)FruDataOffset;
    ReadFruDataRequest.CountToRead     = (UINT8)FruDataSize;

    ReadFruDataResponse = AllocateZeroPool (sizeof (ReadFruDataResponse) + IPMI_RDWR_FRU_FRAGMENT_SIZE);

    if (ReadFruDataResponse == NULL) {
      DEBUG ((DEBUG_ERROR, " Null Pointer returned by AllocateZeroPool to Read Fru data\n"));
      return EFI_OUT_OF_RESOURCES;
    }

    //
    // Collect the data till it is completely retrieved.
    //
    while (ReadFruDataRequest.CountToRead != 0) {
      //
      // Backup the count, since we are going to perform fragmented reads
      //
      BackupCount = ReadFruDataRequest.CountToRead;
      if (ReadFruDataRequest.CountToRead > IPMI_RDWR_FRU_FRAGMENT_SIZE) {
        ReadFruDataRequest.CountToRead = IPMI_RDWR_FRU_FRAGMENT_SIZE;
      }

      ResponseDataSize = sizeof (ReadFruDataResponse) + ReadFruDataRequest.CountToRead;

      Status = IpmiSubmitCommand (
                 IPMI_NETFN_STORAGE,
                 IPMI_STORAGE_READ_FRU_DATA,
                 (UINT8 *)&ReadFruDataRequest,
                 sizeof (ReadFruDataRequest),
                 (UINT8 *)ReadFruDataResponse,
                 &ResponseDataSize
                 );

      if (Status == EFI_BUFFER_TOO_SMALL) {
        DEBUG ((DEBUG_WARN, "%a: WARNING:: IpmiSubmitCommand returned EFI_BUFFER_TOO_SMALL \n", __func__));
      }

      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "%a: IpmiSubmitCommand returned status %r\n", __func__, Status));
        FreePool (ReadFruDataResponse);
        return Status;
      }

      //
      // If the read FRU command returns a count of 0, then no FRU data was found, so exit.
      //
      if (ReadFruDataResponse->CountReturned == 0x00) {
        Status = EFI_NOT_FOUND;
        DEBUG ((DEBUG_ERROR, "%a: IpmiSubmitCommand Response data size is 0x0\n", __func__));
        FreePool (ReadFruDataResponse);
        return Status;
      }

      ReadFruDataRequest.CountToRead = BackupCount;

      //
      // In case of partial retrieval; Data[0] contains the retrieved data size;
      //
      if (ReadFruDataRequest.CountToRead >= ReadFruDataResponse->CountReturned) {
        DataToCopySize                 = ReadFruDataResponse->CountReturned;
        ReadFruDataRequest.CountToRead = (UINT8)(ReadFruDataRequest.CountToRead - ReadFruDataResponse->CountReturned);   // Remaining Count
      } else {
        DEBUG ((
          DEBUG_WARN,
          "%a: WARNING Command.Count (%d) is less than response data size (%d) received\n",
          __func__,
          ReadFruDataRequest.CountToRead,
          ReadFruDataResponse->CountReturned
          ));
        DataToCopySize                 = ReadFruDataRequest.CountToRead;
        ReadFruDataRequest.CountToRead = 0;  // Remaining Count
      }

      ReadFruDataRequest.InventoryOffset = (UINT16)(ReadFruDataRequest.InventoryOffset + DataToCopySize);   // Next Offset to retrieve

      if (PointerOffset + DataToCopySize > FruDataSize) {
        DEBUG ((
          DEBUG_ERROR,
          "Insufficient storage supplied to %a, need more than % bytes\n",
          __func__,
          PointerOffset + DataToCopySize
          ));
        Status = EFI_BUFFER_TOO_SMALL;
        FreePool (ReadFruDataResponse);
        return Status;
      }

      ASSERT (PointerOffset < FruDataSize);
      ASSERT (PointerOffset + DataToCopySize <= FruDataSize);

      CopyMem (&FruData[PointerOffset], &ReadFruDataResponse->Data[0], DataToCopySize); // Copy the partial data
      PointerOffset = (UINT8)(PointerOffset + DataToCopySize);                          // Next offset to the iput pointer.
    }

    FreePool (ReadFruDataResponse);
  } else {
    Status = EFI_UNSUPPORTED;
    return Status;
  }

  return Status;
}

/**
  Set Fru Redir Data.

  @retval This
  @retval FruSlotNumber
  @retval FruDataOffset
  @retval FruDataSize
  @retval FruData

  @retval EFI_STATUS

**/
EFI_STATUS
EFIAPI
EfiSetFruRedirData (
  IN EFI_SM_FRU_REDIR_PROTOCOL  *This,
  IN UINTN                      FruSlotNumber,
  IN UINTN                      FruDataOffset,
  IN UINTN                      FruDataSize,
  IN UINT8                      *FruData
  )
{
  EFI_IPMI_FRU_GLOBAL           *FruPrivate;
  UINT8                         Count;
  UINT8                         BackupCount;
  UINT32                        ResponseDataSize;
  UINT8                         PointerOffset;
  UINT8                         DataToCopySize;
  EFI_STATUS                    Status;
  IPMI_WRITE_FRU_DATA_REQUEST   *WriteFruDataRequest;
  IPMI_WRITE_FRU_DATA_RESPONSE  WriteFruDataResponse;

  FruPrivate    = NULL;
  PointerOffset = 0;

  FruPrivate = INSTANCE_FROM_EFI_SM_IPMI_FRU_THIS (This);

  if ((FruSlotNumber + 1) > FruPrivate->NumSlots) {
    return EFI_NO_MAPPING;
  }

  if (FruSlotNumber >= sizeof (FruPrivate->FruDeviceInfo) / sizeof (EFI_FRU_DEVICE_INFO)) {
    return EFI_INVALID_PARAMETER;
  }

  if (FruPrivate->FruDeviceInfo[FruSlotNumber].FruDevice.Bits.LogicalFruDevice) {
    WriteFruDataRequest = AllocateZeroPool (sizeof (IPMI_WRITE_FRU_DATA_REQUEST) + IPMI_RDWR_FRU_FRAGMENT_SIZE);

    if (WriteFruDataRequest == NULL) {
      DEBUG ((DEBUG_ERROR, " Null Pointer returned by AllocateZeroPool to Write Fru data\n"));
      return EFI_OUT_OF_RESOURCES;
    }

    //
    // Create the FRU Write Command for the logical FRU Device.
    //
    WriteFruDataRequest->DeviceId        = FruPrivate->FruDeviceInfo[FruSlotNumber].FruDevice.Bits.FruDeviceId;
    WriteFruDataRequest->InventoryOffset = (UINT16)FruDataOffset;
    Count                                = (UINT8)FruDataSize;
    PointerOffset                        = 0;

    //
    // Collect the data till it is completely retrieved.
    //
    while (Count != 0) {
      //
      // Backup the count, since we are going to perform fragmented reads
      //
      BackupCount = Count;
      if (Count > IPMI_RDWR_FRU_FRAGMENT_SIZE) {
        Count = IPMI_RDWR_FRU_FRAGMENT_SIZE;
      }

      CopyMem (&WriteFruDataRequest->Data[0], &FruData[PointerOffset], Count);

      ResponseDataSize = sizeof (WriteFruDataResponse);
      Status           = IpmiSubmitCommand (
                           IPMI_NETFN_STORAGE,
                           IPMI_STORAGE_WRITE_FRU_DATA,
                           (UINT8 *)WriteFruDataRequest,
                           (sizeof (WriteFruDataRequest) + Count),
                           (UINT8 *)&WriteFruDataResponse,
                           &ResponseDataSize
                           );

      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "%a: IpmiSubmitCommand returned status %r\n", __func__, Status));
        FreePool (WriteFruDataRequest);
        return Status;
      }

      Count = BackupCount;

      if (Count >= WriteFruDataResponse.CountWritten) {
        DataToCopySize = Count - WriteFruDataResponse.CountWritten;
        Count          = (UINT8)(Count - WriteFruDataResponse.CountWritten); // Remaining Count
      } else {
        DEBUG ((
          DEBUG_WARN,
          "%a: WARNING Count (%d) is less than response data size (%d) received\n",
          __func__,
          Count,
          WriteFruDataResponse.CountWritten
          ));
        DataToCopySize =  Count;
        Count          = 0; // Remaining Count
      }

      //
      // In case of partial retrieval; Data[0] contains the retrieved data size;
      //
      WriteFruDataRequest->InventoryOffset = (UINT16)(WriteFruDataRequest->InventoryOffset + DataToCopySize); // Next Offset to set
      PointerOffset                        = (UINT8)(PointerOffset + DataToCopySize);                         // Next offset to the iput pointer.
    }

    FreePool (WriteFruDataRequest);
  } else {
    return EFI_UNSUPPORTED;
  }

  return EFI_SUCCESS;
}

/**
  Initialize SM Redirection Fru Layer.

  @param ImageHandle - ImageHandle of the loaded driver
  @param SystemTable - Pointer to the System Table

  @retval EFI_STATUS

**/
EFI_STATUS
EFIAPI
InitializeSmRedirFruLayer (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_HANDLE                   NewHandle;
  EFI_STATUS                   Status;
  IPMI_GET_DEVICE_ID_RESPONSE  GetDeviceIdResponse;
  UINT32                       ResponseDataSize;

  gST = SystemTable;
  gBS = gST->BootServices;

  //
  // Initialize Global memory
  //
  mIpmiFruGlobal = AllocateRuntimePool (sizeof (EFI_IPMI_FRU_GLOBAL));
  ASSERT (mIpmiFruGlobal != NULL);
  if (mIpmiFruGlobal == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  mIpmiFruGlobal->NumSlots                             = 0;
  mIpmiFruGlobal->IpmiRedirFruProtocol.GetFruRedirInfo = (EFI_GET_FRU_REDIR_INFO)EfiGetFruRedirInfo;
  mIpmiFruGlobal->IpmiRedirFruProtocol.GetFruSlotInfo  = (EFI_GET_FRU_SLOT_INFO)EfiGetFruSlotInfo;
  mIpmiFruGlobal->IpmiRedirFruProtocol.GetFruRedirData = (EFI_GET_FRU_REDIR_DATA)EfiGetFruRedirData;
  mIpmiFruGlobal->IpmiRedirFruProtocol.SetFruRedirData = (EFI_SET_FRU_REDIR_DATA)EfiSetFruRedirData;
  mIpmiFruGlobal->Signature                            = EFI_SM_FRU_REDIR_SIGNATURE;
  mIpmiFruGlobal->MaxFruSlots                          = MAX_FRU_SLOT;
  //
  //  Get all the SDR Records from BMC and retrieve the Record ID from the structure for future use.
  //
  ResponseDataSize = sizeof (GetDeviceIdResponse);
  Status           = IpmiSubmitCommand (
                       IPMI_NETFN_APP,
                       IPMI_APP_GET_DEVICE_ID,
                       (UINT8 *)NULL,
                       0,
                       (UINT8 *)&GetDeviceIdResponse,
                       &ResponseDataSize
                       );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "IpmiSubmitCommand Ipmi App Get DeviceId Failed %r\n", Status));
    return Status;
  }

  if (GetDeviceIdResponse.DeviceSupport.Bits.FruInventorySupport) {
    //
    // Initialize all FRU slots
    // Add a mandatory FRU Inventory device attached to the controller.
    //
    for (mIpmiFruGlobal->NumSlots = 0; mIpmiFruGlobal->NumSlots < mIpmiFruGlobal->MaxFruSlots; mIpmiFruGlobal->NumSlots++) {
      mIpmiFruGlobal->FruDeviceInfo[mIpmiFruGlobal->NumSlots].Valid = TRUE;
      ZeroMem (&mIpmiFruGlobal->FruDeviceInfo[mIpmiFruGlobal->NumSlots].FruDevice, sizeof (IPMI_FRU_DATA_INFO));
      mIpmiFruGlobal->FruDeviceInfo[mIpmiFruGlobal->NumSlots].FruDevice.Bits.LogicalFruDevice = 1;
      mIpmiFruGlobal->FruDeviceInfo[mIpmiFruGlobal->NumSlots].FruDevice.Bits.FruDeviceId      = mIpmiFruGlobal->NumSlots;
    }
  }

  //
  // Install callback for ReadyToBoot event to generate FRU SMBIOS Data
  //

  GenerateFruSmbiosData (&mIpmiFruGlobal->IpmiRedirFruProtocol);

  //
  // Install the FRU Ipmi Redir protocol.
  //
  NewHandle = NULL;
  Status    = gBS->InstallProtocolInterface (
                     &NewHandle,
                     &gEfiRedirFruProtocolGuid,
                     EFI_NATIVE_INTERFACE,
                     &mIpmiFruGlobal->IpmiRedirFruProtocol
                     );

  ASSERT_EFI_ERROR (Status);
  if (Status != EFI_SUCCESS) {
    return Status;
  }

  return EFI_SUCCESS;
}
