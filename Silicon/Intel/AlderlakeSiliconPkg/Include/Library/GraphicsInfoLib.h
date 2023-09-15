/** @file
  Header file for Graphics Private Info Lib implementation.

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _GRAPHICS_INFO_LIB_H_
#define _GRAPHICS_INFO_LIB_H_

#include <Library/BaseLib.h>
#include <Library/PciSegmentLib.h>
#include <IndustryStandard/Pci22.h>
#include <Register/SaRegsHostBridge.h>

/**
  GetIgdBusNumber: Get IGD Bus Number

  @retval PCI bus number for IGD
**/
UINT8
GetIgdBusNumber (
  VOID
  );

/**
  GetIgdDevNumber: Get IGD Dev Number

  @retval PCI dev number for IGD
**/
UINT8
GetIgdDevNumber (
  VOID
  );

/**
  GetIgdFunNumber: Get IGD Fun Number

  @retval PCI fun number for IGD
**/
UINT8
GetIgdFuncNumber (
  VOID
  );

#endif
