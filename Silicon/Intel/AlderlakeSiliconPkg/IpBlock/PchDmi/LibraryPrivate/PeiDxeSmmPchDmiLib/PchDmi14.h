/** @file
  Internal header file for PCH DMI library for SIP14

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef __PCH_DMI_14_H__
#define __PCH_DMI_14_H__

#include <Library/PchDmiLib.h>

/**
  This function checks if DMI SIP14 Secured Register Lock (SRL) is set

  @retval SRL state
**/
BOOLEAN
IsPchDmi14Locked (
  VOID
  );

#endif
