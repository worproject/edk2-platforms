/** @file
  This file contains routines for eSPI

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/EspiLib.h>
#include <Library/PchPcrLib.h>
#include <Library/PchInfoLib.h>
#include <Library/TimerLib.h>
#include <PchLimits.h>
#include <Register/PchPcrRegs.h>
#include <Register/PchRegsLpc.h>


/**
  Checks if second device capability is enabled

  @retval TRUE      There's second device
  @retval FALSE     There's no second device
**/
BOOLEAN
IsEspiSecondSlaveSupported (
  VOID
  )
{
  return ((PchPcrRead32 (PID_ESPISPI, R_ESPI_PCR_SOFTSTRAPS) & B_ESPI_PCR_SOFTSTRAPS_CS1_EN) != 0);
}


/**
  Is eSPI enabled in strap.

  @retval TRUE          Espi is enabled in strap
  @retval FALSE         Espi is disabled in strap
**/
BOOLEAN
IsEspiEnabled (
  VOID
  )
{
  return (PchPcrRead32 (PID_ESPISPI, R_ESPI_PCR_CFG_VAL) & B_ESPI_PCR_CFG_VAL_ESPI_EN) != 0;
}

typedef enum {
  EspiSlaveOperationConfigRead,
  EspiSlaveOperationConfigWrite,
  EspiSlaveOperationStatusRead,
  EspiSlaveOperationInBandReset
} ESPI_SLAVE_OPERATION;



