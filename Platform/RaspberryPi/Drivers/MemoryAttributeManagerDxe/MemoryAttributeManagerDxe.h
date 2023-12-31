/** @file
 *
 *  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#ifndef __MEMORY_ATTRIBUTE_MANAGER_DXE_H__
#define __MEMORY_ATTRIBUTE_MANAGER_DXE_H__

#include <Guid/MemoryAttributeManagerFormSet.h>

#define PROTOCOL_ENABLED_DEFAULT  FixedPcdGetBool(PcdMemoryAttributeEnabledDefault)

#define MEMORY_ATTRIBUTE_MANAGER_DATA_VAR_NAME  L"MemoryAttributeManagerData"

typedef struct {
  BOOLEAN    Enabled;
} MEMORY_ATTRIBUTE_MANAGER_VARSTORE_DATA;

#endif // __MEMORY_ATTRIBUTE_MANAGER_DXE_H__
