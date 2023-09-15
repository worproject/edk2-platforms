/** @file
  Header file for PmcLib.

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _PMC_LIB_H_
#define _PMC_LIB_H_

#include <PmConfig.h>


typedef struct {
  UINT32    Buf0;
  UINT32    Buf1;
  UINT32    Buf2;
  UINT32    Buf3;
} PMC_IPC_COMMAND_BUFFER;

/**
  Get PCH ACPI base address.

  @retval Address                   Address of PWRM base address.
**/
UINT16
PmcGetAcpiBase (
  VOID
  );

/**
  Get PCH PWRM base address.

  @retval Address                   Address of PWRM base address.
**/
UINT32
PmcGetPwrmBase (
  VOID
  );

#endif // _PMC_LIB_H_
