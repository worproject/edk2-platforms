/** @file
  Server Management Generic FRU Driver. Each FRU REDIR driver attaches
  to the Generic FRU driver which is coded in this section. A Runtime
  protocol will bind to this and be able to access all the FRU transports
  as well as their physical layers.

Copyright (c) 2023, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "GenericFruDriver.h"

FRU_MODULE_GLOBAL  *mFruModuleGlobal;

//
// Define module globals used to register for notification of when
// the FRU REDIR protocol has been produced.
//
VOID       *mEfiFruRedirProtocolNotifyReg;
EFI_EVENT  mEfiFruRedirProtocolEvent;

/**
  Efi Lib Get Fru Info.

  @param FruTypeGuid
  @param FruInstance
  @param FruFormatGuid
  @param DataAccessGranularity
  @param FruInformationString
  @param Global
  @param Virtual

  @retval EFI_SUCCESS

**/
EFI_STATUS
EfiLibGetFruInfo (
  IN EFI_GUID           *FruTypeGuid,
  IN UINTN              FruInstance,
  OUT EFI_GUID          *FruFormatGuid,
  OUT UINTN             *DataAccessGranularity,
  OUT CHAR16            **FruInformationString,
  IN FRU_MODULE_GLOBAL  *Global,
  IN BOOLEAN            Virtual
  )
{
  UINTN  Index;
  VOID   *FruRedirCommand;

  //
  // Get the FRU Type string first.
  //
  for (Index = 0; Index < Global->MaxDescriptors; Index++) {
    if (Global->Redir[Index].Valid) {
      if (CompareGuid (&Global->Redir[Index].FruTypeGuid, FruTypeGuid)) {
        //
        // We found our target, Now check the format it supports.
        //
        FruRedirCommand = Global->Redir[Index].Command[Virtual].GetFruRedirInfo.Function;
        return (*((EFI_GET_FRU_REDIR_INFO *)&FruRedirCommand))(
  Global->Redir[Index].Command[Virtual].This,
  FruInstance,
  FruFormatGuid,
  DataAccessGranularity,
  FruInformationString
  );
      }
    }
  }

  return EFI_NOT_FOUND;
}

/**
  Efi Get Fru Info.

  @param This
  @param FruTypeGuid
  @param FruInstance
  @param FruFormatGuid
  @param DataAccessGranularity
  @param FruInformationString

  @retval EFI_SUCCESS

**/
EFI_STATUS
EfiGetFruInfo (
  IN EFI_SM_FRU_PROTOCOL  *This,
  IN EFI_GUID             *FruTypeGuid,
  IN UINTN                FruInstance,
  OUT EFI_GUID            *FruFormatGuid,
  OUT UINTN               *DataAccessGranularity,
  OUT CHAR16              **FruInformationString
  )
{
  return EfiLibGetFruInfo (
           FruTypeGuid,
           FruInstance,
           FruFormatGuid,
           DataAccessGranularity,
           FruInformationString,
           mFruModuleGlobal,
           EfiGoneVirtual ()
           );
}

/**
  Efi Lib Get Fru Data.

  @param FruTypeGuid
  @param FruInstance
  @param FruDataOffset
  @param FruDataSize
  @param FruData
  @param Global
  @param Virtual

  @retval EFI_SUCCESS

**/
EFI_STATUS
EfiLibGetFruData (
  IN EFI_GUID           *FruTypeGuid,
  IN  UINTN             FruInstance,
  IN  UINTN             FruDataOffset,
  IN  UINTN             FruDataSize,
  IN  UINT8             *FruData,
  IN FRU_MODULE_GLOBAL  *Global,
  IN BOOLEAN            Virtual
  )
{
  UINTN  Index;
  VOID   *FruRedirCommand;

  //
  // Get the FRU Type string first.
  //
  for (Index = 0; Index < Global->MaxDescriptors; Index++) {
    if (Global->Redir[Index].Valid) {
      if (CompareGuid (&Global->Redir[Index].FruTypeGuid, FruTypeGuid)) {
        //
        // We found our target, Now check the format it supports.
        //
        FruRedirCommand = Global->Redir[Index].Command[Virtual].GetFruRedirData.Function;
        return (*((EFI_GET_FRU_REDIR_DATA *)&FruRedirCommand))(
  Global->Redir[Index].Command[Virtual].This,
  FruInstance,
  FruDataOffset,
  FruDataSize,
  FruData
  );
      }
    }
  }

  return EFI_NOT_FOUND;
}

/**
  Efi Get Fru Data.

  @param This
  @param FruTypeGuid
  @param FruInstance
  @param FruDataOffset
  @param FruDataSize
  @param FruData

  @retval EFI_SUCCESS

**/
EFI_STATUS
EfiGetFruData (
  IN EFI_SM_FRU_PROTOCOL  *This,
  IN EFI_GUID             *FruTypeGuid,
  IN  UINTN               FruInstance,
  IN  UINTN               FruDataOffset,
  IN  UINTN               FruDataSize,
  IN  UINT8               *FruData
  )
{
  return EfiLibGetFruData (
           FruTypeGuid,
           FruInstance,
           FruDataOffset,
           FruDataSize,
           FruData,
           mFruModuleGlobal,
           EfiGoneVirtual ()
           );
}

/**
  Efi Lib Set Fru Data.

  @param FruTypeGuid
  @param FruInstance
  @param FruDataOffset
  @param FruDataSize
  @param FruData
  @param Global
  @param Virtual

  @retval EFI_SUCCESS

**/
EFI_STATUS
EfiLibSetFruData (
  IN EFI_GUID           *FruTypeGuid,
  IN  UINTN             FruInstance,
  IN  UINTN             FruDataOffset,
  IN  UINTN             FruDataSize,
  IN  UINT8             *FruData,
  IN FRU_MODULE_GLOBAL  *Global,
  IN BOOLEAN            Virtual
  )
{
  UINTN  Index;
  VOID   *FruRedirCommand;

  //
  // Get the FRU Type string first.
  //
  for (Index = 0; Index < Global->MaxDescriptors; Index++) {
    if (Global->Redir[Index].Valid) {
      if (CompareGuid (&Global->Redir[Index].FruTypeGuid, FruTypeGuid)) {
        //
        // We found our target, Now check the format it supports.
        //
        FruRedirCommand = Global->Redir[Index].Command[Virtual].SetFruRedirData.Function;
        return (*((EFI_SET_FRU_REDIR_DATA *)&FruRedirCommand))(
  Global->Redir[Index].Command[Virtual].This,
  FruInstance,
  FruDataOffset,
  FruDataSize,
  FruData
  );
      }
    }
  }

  return EFI_NOT_FOUND;
}

/**
  Efi Set Fru Data.

  @param This
  @param FruTypeGuid
  @param FruInstance
  @param FruDataOffset
  @param FruDataSize
  @param FruData

  @retval EFI_SUCCESS

**/
EFI_STATUS
EfiSetFruData (
  IN EFI_SM_FRU_PROTOCOL  *This,
  IN EFI_GUID             *FruTypeGuid,
  IN  UINTN               FruInstance,
  IN  UINTN               FruDataOffset,
  IN  UINTN               FruDataSize,
  IN  UINT8               *FruData
  )
{
  return EfiLibSetFruData (
           FruTypeGuid,
           FruInstance,
           FruDataOffset,
           FruDataSize,
           FruData,
           mFruModuleGlobal,
           EfiGoneVirtual ()
           );
}

/**
  Sm Fru Address Change Event.

  @param Event
  @param Context
**/
VOID
SmFruAddressChangeEvent (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  UINTN                  Index;
  REDIR_FRU_MODULE_PROC  *RedirProc;

  //
  //  FRU REDIR Pointer Conversion
  //
  for (Index = 0; Index < mFruModuleGlobal->MaxDescriptors; Index++) {
    if (mFruModuleGlobal->Redir[Index].Valid) {
      RedirProc = (REDIR_FRU_MODULE_PROC *)&mFruModuleGlobal->Redir[Index].Command[EFI_FRU_VIRTUAL];
      EfiConvertFunction (&RedirProc->GetFruRedirInfo);
      EfiConvertFunction (&RedirProc->GetFruSlotInfo);
      EfiConvertFunction (&RedirProc->GetFruRedirData);
      EfiConvertFunction (&RedirProc->SetFruRedirData);
      EfiConvertPointer (0x02, (VOID **)&RedirProc->This);
    }
  }
}

/**
  Set Fru Redir Instances.

  @retval EFI_SUCCESS

**/
EFI_STATUS
SetFruRedirInstances (
  VOID
  )
{
  UINTN                      NumHandles;
  UINTN                      Index, Instance, EmptyIndex;
  EFI_HANDLE                 *Buffer;
  EFI_STATUS                 Status;
  EFI_SM_FRU_REDIR_PROTOCOL  *Redir;
  REDIR_FRU_MODULE_PROC      *RedirProc;

  //
  // Check for all IPMI Controllers
  //
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiRedirFruProtocolGuid,
                  NULL,
                  &NumHandles,
                  &Buffer
                  );

  if (EFI_ERROR (Status) || (NumHandles == 0)) {
    return EFI_SUCCESS;
  }

  EmptyIndex = mFruModuleGlobal->MaxDescriptors;

  for (Index = 0; ((Index < NumHandles) && (Index < mFruModuleGlobal->MaxDescriptors)); Index++) {
    Status = gBS->HandleProtocol (
                    Buffer[Index],
                    &gEfiRedirFruProtocolGuid,
                    (VOID *)&Redir
                    );
    if (EFI_ERROR (Status) || (Redir == NULL)) {
      continue;
    }

    for (Instance = 0; Instance < mFruModuleGlobal->MaxDescriptors; Instance++) {
      if (mFruModuleGlobal->Redir[Instance].Valid == FALSE) {
        if (EmptyIndex >= mFruModuleGlobal->MaxDescriptors) {
          EmptyIndex = Instance;
        }
      } else {
        if (Redir == mFruModuleGlobal->Redir[Instance].Command->This) {
          EmptyIndex = mFruModuleGlobal->MaxDescriptors;
          continue;
        }
      }
    }

    if (EmptyIndex < mFruModuleGlobal->MaxDescriptors) {
      Redir->GetFruSlotInfo (
               Redir,
               &mFruModuleGlobal->Redir[EmptyIndex].FruTypeGuid,
               &mFruModuleGlobal->Redir[EmptyIndex].StartSlot,
               &mFruModuleGlobal->Redir[EmptyIndex].EndSlot
               );

      mFruModuleGlobal->Redir[EmptyIndex].EndSlot += mFruModuleGlobal->Redir[EmptyIndex].StartSlot;
      RedirProc                                    = (REDIR_FRU_MODULE_PROC *)mFruModuleGlobal->Redir[EmptyIndex].Command;
      mFruModuleGlobal->Redir[EmptyIndex].Valid    = TRUE;

      EfiSetFunctionEntry (&RedirProc->GetFruRedirInfo, *((VOID **)&Redir->GetFruRedirInfo));
      EfiSetFunctionEntry (&RedirProc->GetFruSlotInfo, *((VOID **)&Redir->GetFruSlotInfo));
      EfiSetFunctionEntry (&RedirProc->GetFruRedirData, *((VOID **)&Redir->GetFruRedirData));
      EfiSetFunctionEntry (&RedirProc->SetFruRedirData, *((VOID **)&Redir->SetFruRedirData));
      RedirProc->This = Redir;

      CopyMem (&RedirProc[EFI_FRU_VIRTUAL], &RedirProc[EFI_FRU_PHYSICAL], sizeof (REDIR_FRU_MODULE_PROC));
    }
  }

  if (Buffer != NULL) {
    gBS->FreePool (Buffer);
  }

  return EFI_SUCCESS;
}

/**
  This notification function is invoked when an instance of the
  FRU REDIR protocol is produced.

  @param Event - The event that occurred
  @param Context - For EFI compatibility.  Not used.

**/
VOID
EFIAPI
NotifyFruRedirEventCallback (
  EFI_EVENT  Event,
  VOID       *Context
  )
{
  SetFruRedirInstances ();
}

/**
  Initialize Sm Fru Layer.

  @param ImageHandle
  @param SystemTable

    @retval EFI_SUCCESS

**/
EFI_STATUS
EFIAPI
InitializeSmFruLayer (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_HANDLE           NewHandle;
  EFI_STATUS           Status;
  EFI_SM_FRU_PROTOCOL  *FruProtocol;

  Status = gBS->AllocatePool (
                  EfiRuntimeServicesData,
                  sizeof (FRU_MODULE_GLOBAL),
                  (VOID **)&mFruModuleGlobal
                  );

  ASSERT_EFI_ERROR (Status);
  if (Status != EFI_SUCCESS) {
    return Status;
  }

  ZeroMem (mFruModuleGlobal, sizeof (FRU_MODULE_GLOBAL));

  SmFruServiceInitialize (ImageHandle, SystemTable);

  mFruModuleGlobal->MaxDescriptors = MAX_REDIR_DESCRIPTOR;

  //
  // Check for all IPMI Controllers
  //
  SetFruRedirInstances ();

  Status = gBS->AllocatePool (
                  EfiRuntimeServicesData,
                  sizeof (EFI_SM_FRU_PROTOCOL),
                  (VOID **)&FruProtocol
                  );
  ASSERT_EFI_ERROR (Status);
  if (Status != EFI_SUCCESS) {
    return Status;
  }

  ZeroMem (FruProtocol, sizeof (EFI_SM_FRU_PROTOCOL));

  FruProtocol->GetFruInfo = (EFI_GET_FRU_INFO)EfiGetFruInfo;
  FruProtocol->GetFruData = (EFI_GET_FRU_DATA)EfiGetFruData;
  FruProtocol->SetFruData = (EFI_SET_FRU_DATA)EfiSetFruData;

  NewHandle = NULL;
  Status    = gBS->InstallProtocolInterface (
                     &NewHandle,
                     &gEfiGenericFruProtocolGuid,
                     EFI_NATIVE_INTERFACE,
                     FruProtocol
                     );

  ASSERT_EFI_ERROR (Status);
  if (Status != EFI_SUCCESS) {
    return Status;
  }

  //
  // Register to be notified when the FRU REDIR protocol has been
  // produced.
  //

  Status = gBS->CreateEvent (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  NotifyFruRedirEventCallback,
                  NULL,
                  &mEfiFruRedirProtocolEvent
                  );

  ASSERT_EFI_ERROR (Status);
  if (Status != EFI_SUCCESS) {
    return Status;
  }

  Status = gBS->RegisterProtocolNotify (
                  &gEfiRedirFruProtocolGuid,
                  mEfiFruRedirProtocolEvent,
                  &mEfiFruRedirProtocolNotifyReg
                  );
  ASSERT_EFI_ERROR (Status);

  return Status;
}
