/** @file
Defines Platform GPIO Configuration Arrary

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Library/GpioLib.h>

#ifndef _PLATFORM_GPIO_CONFIG_H_
#define _PLATFORM_GPIO_CONFIG_H_

typedef struct {
  GPIO_INIT_CONFIG   GpioConfig[0];
} GPIO_INIT_CONFIG_ARRAY;

#endif
