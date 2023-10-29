/** @file
  This code abstracts the generic ELOG Protocol.

Copyright (c) 2023, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _GENERIC_ELOG_H_
#define _GENERIC_ELOG_H_

#include "ServerManagement.h"

typedef struct _EFI_SM_ELOG_PPI EFI_SM_ELOG_REDIR_PPI;

//
// Common Defines
//
#ifndef _EFI_SM_ELOG_TYPE
#define _EFI_SM_ELOG_TYPE
typedef enum {
  EfiElogSmSMBIOS,
  EfiElogSmIPMI,
  EfiElogSmMachineCritical,
  EfiElogSmASF,
  EfiElogSmOEM,
  EfiSmElogMax
} EFI_SM_ELOG_TYPE;
#endif

//
//  Generic ELOG Function Prototypes
//
typedef
EFI_STATUS
(EFIAPI *EFI_SET_ELOG_DATA)(
  IN  EFI_SM_ELOG_REDIR_PPI             *This,
  IN  UINT8                             *ElogData,
  IN  EFI_SM_ELOG_TYPE                  DataType,
  IN  BOOLEAN                           AlertEvent,
  IN  UINTN                             DataSize,
  OUT UINT64                            *RecordId
  );

typedef
EFI_STATUS
(EFIAPI *EFI_GET_ELOG_DATA)(
  IN  EFI_SM_ELOG_REDIR_PPI             *This,
  IN  OUT UINT8                         *ElogData,
  IN  EFI_SM_ELOG_TYPE                  DataType,
  IN  OUT UINTN                         *DataSize,
  IN  OUT UINT64                        *RecordId
  );

typedef
EFI_STATUS
(EFIAPI *EFI_ERASE_ELOG_DATA)(
  IN EFI_SM_ELOG_REDIR_PPI              *This,
  IN EFI_SM_ELOG_TYPE                   DataType,
  IN OUT UINT64                         *RecordId
  );

typedef
EFI_STATUS
(EFIAPI *EFI_ACTIVATE_ELOG)(
  IN EFI_SM_ELOG_REDIR_PPI              *This,
  IN EFI_SM_ELOG_TYPE                   DataType,
  IN BOOLEAN                            *EnableElog,
  OUT BOOLEAN                           *ElogStatus
  );

//
// IPMI TRANSPORT PPI
//
struct _EFI_SM_ELOG_PPI {
  EFI_SET_ELOG_DATA      SetEventLogData;
  EFI_GET_ELOG_DATA      GetEventLogData;
  EFI_ERASE_ELOG_DATA    EraseEventlogData;
  EFI_ACTIVATE_ELOG      ActivateEventLog;
};

extern EFI_GUID  gPeiRedirElogPpiGuid;

#endif
