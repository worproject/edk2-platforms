/** @file
  BMC Event Log functions.

Copyright (c) 2023, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _EFI_BMCELOG_H_
#define _EFI_BMCELOG_H_

//
// Statements that include other files
//

#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Protocol/IpmiTransportProtocol.h>
#include <Protocol/GenericElog.h>
#include "BmcElogCommon.h"

//
// BMC Elog instance data
//
typedef struct {
  UINTN                         Signature;
  SM_COM_ADDRESS                ControllerAddress;
  SM_COM_ADDRESS                TargetAddress;
  UINT16                        Instance;
  EFI_SM_ELOG_TYPE              DataType;
  UINT8                         TempData[MAX_TEMP_DATA + 1];
  EFI_SM_ELOG_REDIR_PROTOCOL    BmcElog;
} EFI_BMC_ELOG_INSTANCE_DATA;

//
// BMC Elog Instance signature
//
#define EFI_ELOG_REDIR_SIGNATURE  SIGNATURE_32 ('e', 'e', 'l', 'g')

#define INSTANCE_FROM_EFI_ELOG_REDIR_THIS(a)  CR (a, EFI_BMC_ELOG_INSTANCE_DATA, BmcElog, EFI_ELOG_REDIR_SIGNATURE)

#endif
