/** @file
  Generic FRU functions.

Copyright (c) 2023, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _EFI_GENEFRU_H_
#define _EFI_GENEFRU_H_

#include <PiDxe.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiRuntimeLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>

#include "Protocol/GenericFru.h"
#include "Protocol/RedirFru.h"

#define  EFI_FRU_PHYSICAL      0
#define  EFI_FRU_VIRTUAL       1
#define  MAX_REDIR_DESCRIPTOR  10
//
// A pointer to a function in IPF points to a plabel.
//
typedef struct {
  UINT64    EntryPoint;
  UINT64    GP;
} EFI_PLABEL;

typedef struct {
  EFI_PLABEL    *Function;
  EFI_PLABEL    Plabel;
} FUNCTION_PTR;

typedef struct {
  EFI_SM_FRU_REDIR_PROTOCOL    *This;
  FUNCTION_PTR                 GetFruRedirInfo;
  FUNCTION_PTR                 GetFruSlotInfo;
  FUNCTION_PTR                 GetFruRedirData;
  FUNCTION_PTR                 SetFruRedirData;
} REDIR_FRU_MODULE_PROC;

typedef struct {
  BOOLEAN                  Valid;
  EFI_GUID                 FruTypeGuid;
  UINTN                    StartSlot;
  UINTN                    EndSlot;
  REDIR_FRU_MODULE_PROC    Command[2];
} FRU_REDIR_MODULES;

typedef struct {
  FRU_REDIR_MODULES    Redir[MAX_REDIR_DESCRIPTOR];
  UINTN                MaxDescriptors;
} FRU_MODULE_GLOBAL;

/**
  Efi Convert Function.

  @param Function

  @retval EFI_SUCCESS

**/
EFI_STATUS
EfiConvertFunction (
  IN  FUNCTION_PTR  *Function
  );

/**
  Efi Set Function Entry.

  @param FunctionPointer
  @param Function

  @retval EFI_SUCCESS

**/
EFI_STATUS
EfiSetFunctionEntry (
  IN  FUNCTION_PTR  *FunctionPointer,
  IN  VOID          *Function
  );

/**
  Sm Fru Service Initialize.

  @param ImageHandle
  @param SystemTable

  @retval EFI_SUCCESS

**/
EFI_STATUS
SmFruServiceInitialize (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  );

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
  );

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
  );

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
  );

#endif
