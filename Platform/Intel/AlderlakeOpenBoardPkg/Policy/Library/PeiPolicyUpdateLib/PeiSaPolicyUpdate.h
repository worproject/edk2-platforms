/** @file

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef _PEI_SA_POLICY_UPDATE_H_
#define _PEI_SA_POLICY_UPDATE_H_

//
// External include files do NOT need to be explicitly specified in real EDKII
// environment
//
#include <Ppi/SiPolicy.h>
#include <Library/DebugPrintErrorLevelLib.h>
#include <Ppi/Wdt.h>
#include <CpuRegs.h>
#include <Library/CpuPlatformLib.h>
#include "PeiPchPolicyUpdate.h"
#include <Library/PcdLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/SiPolicyLib.h>

#define WDT_TIMEOUT 60

#endif

