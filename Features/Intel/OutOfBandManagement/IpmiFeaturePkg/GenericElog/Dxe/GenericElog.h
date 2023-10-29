/** @file
  Generic Event Log functions.

Copyright (c) 2023, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _EFI_GENELOG_H_
#define _EFI_GENELOG_H_

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>

#include "ServerManagement.h"

#include <Protocol/IpmiTransportProtocol.h>
#include <Protocol/GenericElog.h>

#define EFI_ELOG_PHYSICAL     0
#define EFI_ELOG_VIRTUAL      1
#define MAX_REDIR_DESCRIPTOR  10

///
/// A pointer to a function in IPF points to a plabel.
///
typedef struct {
  UINT64    EntryPoint;
  UINT64    GP;
} EFI_PLABEL;

typedef struct {
  EFI_PLABEL    *Function;
  EFI_PLABEL    Plabel;
} FUNCTION_PTR;

typedef struct {
  EFI_SM_ELOG_REDIR_PROTOCOL    *This;
  FUNCTION_PTR                  SetEventLogData;
  FUNCTION_PTR                  GetEventLogData;
  FUNCTION_PTR                  EraseEventLogData;
  FUNCTION_PTR                  ActivateEventLog;
} REDIR_MODULE_PROC;

typedef struct {
  BOOLEAN              Valid;
  REDIR_MODULE_PROC    Command[2];
} REDIR_MODULES;

typedef struct {
  REDIR_MODULES    Redir[MAX_REDIR_DESCRIPTOR];
  UINTN            MaxDescriptors;
} ELOG_MODULE_GLOBAL;

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
  Elog Service Initialize.

  @param ImageHandle
  @param SystemTable

  @retval EFI_SUCCESS

**/
EFI_STATUS
ElogServiceInitialize (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  );

/**
  Efi Lib Set Elog Data.

  @param ElogData
  @param DataType
  @param AlertEvent
  @param DataSize
  @param RecordId
  @param Global
  @param Virtual

  @retval EFI_SUCCESS

**/
EFI_STATUS
EfiLibSetElogData (
  IN  UINT8             *ElogData,
  IN  EFI_SM_ELOG_TYPE  DataType,
  IN  BOOLEAN           AlertEvent,
  IN  UINTN             DataSize,
  OUT UINT64            *RecordId,
  ELOG_MODULE_GLOBAL    *Global,
  BOOLEAN               Virtual
  );

/**
  Efi Lib Get Elog Data.

  @param ElogData
  @param DataType
  @param DataSize
  @param RecordId
  @param Global
  @param Virtual

  @retval EFI_SUCCESS

**/
EFI_STATUS
EfiLibGetElogData (
  IN UINT8             *ElogData,
  IN EFI_SM_ELOG_TYPE  DataType,
  IN OUT UINTN         *DataSize,
  IN OUT UINT64        *RecordId,
  ELOG_MODULE_GLOBAL   *Global,
  BOOLEAN              Virtual
  );

/**
  Efi Lib Erase Elog Data.

  @param DataType
  @param RecordId
  @param Global
  @param Virtual

  @retval EFI_SUCCESS

**/
EFI_STATUS
EfiLibEraseElogData (
  IN EFI_SM_ELOG_TYPE  DataType,
  IN OUT UINT64        *RecordId,
  ELOG_MODULE_GLOBAL   *Global,
  BOOLEAN              Virtual
  );

/**
  This API enables/Disables Event Log.

  @param DataType              - Type of Elog Data that is being Activated.
  @param EnableElog            - Enables (TRUE) / Disables (FALSE) Event Log. If NULL just returns the
                                 Current ElogStatus.
  @param ElogStatus            - Current (New) Status of Event Log. Enabled (TRUE), Disabled (FALSE).
  @param Global                - The module global variable pointer.
  @param Virtual               - If this function is called in virtual mode or physical mode

  @retval EFI_SUCCESS           - Event-Log was recorded successfully
  @retval EFI_UNSUPPORTED       - The Data Type is unsupported

**/

EFI_STATUS
EfiLibActivateElog (
  IN  EFI_SM_ELOG_TYPE  DataType,
  IN BOOLEAN            *EnableElog,
  OUT BOOLEAN           *ElogStatus,
  ELOG_MODULE_GLOBAL    *Global,
  BOOLEAN               Virtual
  );

#endif //_EFI_GENELOG_H_
