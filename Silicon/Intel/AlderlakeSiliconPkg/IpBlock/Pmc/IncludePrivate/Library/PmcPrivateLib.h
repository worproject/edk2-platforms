/** @file
  Header file for private PmcLib.

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _PMC_PRIVATE_LIB_H_
#define _PMC_PRIVATE_LIB_H_

#include <Library/PmcLib.h>
#include "Register/PmcRegs.h"

typedef enum {
  PmcSwSmiRate1p5ms = 0,
  PmcSwSmiRate16ms,
  PmcSwSmiRate32ms,
  PmcSwSmiRate64ms
} PMC_SWSMI_RATE;

/**
  This function sets SW SMI Rate.

  @param[in] SwSmiRate        Refer to PMC_SWSMI_RATE for possible values
**/
VOID
PmcSetSwSmiRate (
  IN PMC_SWSMI_RATE          SwSmiRate
  );

typedef enum {
  PmcPeriodicSmiRate8s = 0,
  PmcPeriodicSmiRate16s,
  PmcPeriodicSmiRate32s,
  PmcPeriodicSmiRate64s
} PMC_PERIODIC_SMI_RATE;

/**
  This function sets Periodic SMI Rate.

  @param[in] PeriodicSmiRate        Refer to PMC_PERIODIC_SMI_RATE for possible values
**/
VOID
PmcSetPeriodicSmiRate (
  IN PMC_PERIODIC_SMI_RATE    PeriodicSmiRate
  );

#endif // _PMC_PRIVATE_LIB_H_
