/** @file
  PCH configuration based on PCH policy

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _PCH_POLICY_COMMON_H_
#define _PCH_POLICY_COMMON_H_

#include <ConfigBlock.h>

#include "PchLimits.h"
#include "ConfigBlock/PchGeneralConfig.h"
#include <PchPcieRpConfig.h>
#include <PchDmiConfig.h>
#include <PmConfig.h>
#include <SerialIoConfig.h>

#ifndef FORCE_ENABLE
#define FORCE_ENABLE  1
#endif
#ifndef FORCE_DISABLE
#define FORCE_DISABLE 2
#endif
#ifndef PLATFORM_POR
#define PLATFORM_POR  0
#endif


#endif // _PCH_POLICY_COMMON_H_
