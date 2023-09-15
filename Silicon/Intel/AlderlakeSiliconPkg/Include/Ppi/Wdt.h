/** @file
  Watchdog Timer PPI

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _PEI_WDT_H_
#define _PEI_WDT_H_

#include <Protocol/Wdt.h>

//
// Extern the GUID for PPI users.
//
extern EFI_GUID       gWdtPpiGuid;

///
/// Reuse WDT_PROTOCOL definition
///
typedef WDT_PROTOCOL  WDT_PPI;

#endif
