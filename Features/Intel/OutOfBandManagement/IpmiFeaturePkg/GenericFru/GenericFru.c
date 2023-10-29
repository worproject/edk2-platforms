/** @file
  Hooks for Generic FRU.

Copyright (c) 2023, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "GenericFruDriver.h"

//
// Don't use module globals after the SetVirtualAddress map is signaled
//
extern FRU_MODULE_GLOBAL  *mFruModuleGlobal;

/**
  Efi Convert Function.

  @param Function

  @retval EFI_SUCCESS

**/
EFI_STATUS
EfiConvertFunction (
  IN  FUNCTION_PTR  *Function
  )
{
  gRT->ConvertPointer (0x02, (VOID **)&Function->Function);
  return EFI_SUCCESS;
}

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
  )
{
  FunctionPointer->Function = (EFI_PLABEL *)Function;
  return EFI_SUCCESS;
}

/**
  SmFru Service Initialize.

  @param ImageHandle
  @param SystemTable

  @retval EFI_SUCCESS

**/
EFI_STATUS
SmFruServiceInitialize (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  return EFI_SUCCESS;
}
