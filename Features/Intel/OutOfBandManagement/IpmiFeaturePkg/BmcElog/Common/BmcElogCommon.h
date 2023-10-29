/** @file
  BMC Event Log functions.

Copyright (c) 2023, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _BMCELOG_COMMON_H_
#define _BMCELOG_COMMON_H_

//
// Statements that include other files
//
#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/ReportStatusCodeLib.h>
#include "ServerManagement.h"
#include <SmStatusCodes.h>
#include <IndustryStandard/Ipmi.h>
#include <Library/IpmiBaseLib.h>

#define MAX_TEMP_DATA       160
#define CLEAR_SEL_COUNTER   0x200
#define SEL_RECORD_SIZE     0x10       // 16 bytes : Entire SEL Record size
#define SEL_RECORD_ID_SIZE  0x2        // 2 bytes  : SEL Record-ID size

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

/**
  WaitTillClearSel.

  @param ResvId               Reserved ID

  @retval EFI_SUCCESS
  @retval EFI_NO_RESPONSE

**/
EFI_STATUS
WaitTillClearSel (
  UINT8  *ResvId
  );

/**
  Set Bmc Elog Data.


  @param ElogData      Buffer for log storage
  @param DataType      Event Log type
  @param AlertEvent    If it is an alert event
  @param Size          Log data size
  @param RecordId      Indicate which recorder it is

  @retval EFI_STATUS

**/
EFI_STATUS
SetBmcElogRecord (
  IN  UINT8             *ElogData,
  IN  EFI_SM_ELOG_TYPE  DataType,
  IN  BOOLEAN           AlertEvent,
  IN  UINTN             Size,
  OUT UINT64            *RecordId
  );

/**
  Get Bmc Elog Data.

  @param ElogData      Buffer for log data store
  @param DataType      Event log type
  @param Size          Size of log data
  @param RecordId      indicate which recorder it is

  @retval EFI_STATUS

**/
EFI_STATUS
GetBmcElogRecord (
  IN UINT8             *ElogData,
  IN EFI_SM_ELOG_TYPE  DataType,
  IN OUT UINTN         *Size,
  IN OUT UINT64        *RecordId
  );

/**
  Erase Bmc Elog Data.

  @param This          Protocol pointer
  @param DataType      Event log type
  @param RecordId      return which recorder it is

  @retval EFI_STATUS

**/
EFI_STATUS
EraseBmcElogRecord (
  IN EFI_SM_ELOG_TYPE  DataType,
  IN OUT UINT64        *RecordId
  );

/**
  Activate Bmc Elog.

  @param DataType      indicate event log type
  @param EnableElog    Enable/Disable event log
  @param ElogStatus    return log status

  @retval EFI_STATUS

**/
EFI_STATUS
ActivateBmcElog (
  IN  EFI_SM_ELOG_TYPE  DataType,
  IN  BOOLEAN           *EnableElog,
  OUT BOOLEAN           *ElogStatus
  );

/**
  This function verifies the BMC SEL is full and When it is reports the error to the Error Manager.

  @retval EFI_SUCCESS
  @retval EFI_DEVICE_ERROR

**/
EFI_STATUS
EFIAPI
CheckIfSelIsFull (
  VOID
  );

#endif
