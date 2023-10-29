/** @file
  BMC Event Log functions.

Copyright (c) 2023, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _EFI_PEIBMCELOG_H_
#define _EFI_PEIBMCELOG_H_

//
// Statements that include other files
//

#include <Ppi/GenericElog.h>
#include <Ppi/IpmiTransportPpi.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <IndustryStandard/Ipmi.h>
#include "BmcElogCommon.h"

//
// BMC Elog instance data
//
typedef struct {
  UINTN                     Signature;
  SM_COM_ADDRESS            ControllerAddress;
  SM_COM_ADDRESS            TargetAddress;
  UINT16                    Instance;
  EFI_SM_ELOG_TYPE          DataType;
  UINT8                     TempData[MAX_TEMP_DATA + 1];
  EFI_SM_ELOG_REDIR_PPI     BmcElogPpi;
  EFI_PEI_PPI_DESCRIPTOR    BmcElog;
} EFI_PEI_BMC_ELOG_INSTANCE_DATA;

//
// BMC Elog Instance signature
//
#define EFI_PEI_ELOG_REDIR_SIGNATURE  SIGNATURE_32 ('e', 'p', 'l', 'g')

#define INSTANCE_FROM_EFI_PEI_ELOG_REDIR_THIS(a)  CR (a, EFI_PEI_BMC_ELOG_INSTANCE_DATA, BmcElogPpi, EFI_PEI_ELOG_REDIR_SIGNATURE)

#endif
