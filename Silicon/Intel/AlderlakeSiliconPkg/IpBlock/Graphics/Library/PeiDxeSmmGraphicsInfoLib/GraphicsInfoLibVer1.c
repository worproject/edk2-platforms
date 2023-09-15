/** @file
  Source file for common Graphics Info Lib.

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#include <Library/GraphicsInfoLib.h>
#include <Register/IgdRegs.h>
#include <Library/TimerLib.h>
#include <Base.h>

/**
  GetIgdBusNumber: Get IGD Bus Number

  @retval PCI bus number for IGD
**/
UINT8
GetIgdBusNumber (
  VOID
  )
{
  return (UINT8) IGD_BUS_NUM;
}

/**
  GetIgdDevNumber: Get IGD Dev Number

  @retval PCI dev number for IGD
**/
UINT8
GetIgdDevNumber (
  VOID
  )
{
  return (UINT8) IGD_DEV_NUM;
}

/**
  GetIgdFunNumber: Get IGD Fun Number

  @retval PCI fun number for IGD
**/
UINT8
GetIgdFuncNumber (
  VOID
  )
{
  return (UINT8) IGD_FUN_NUM;
}



