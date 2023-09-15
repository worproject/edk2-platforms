/** @file
  Pch information library for ADL.

  All function in this library is available for PEI, DXE, and SMM,
  But do not support UEFI RUNTIME environment call.

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#include <Base.h>
#include <PchPcieRpInfo.h>
#include <Uefi/UefiBaseType.h>
#include <Library/PchInfoLib.h>
#include <Library/PmcPrivateLib.h>
#include <Library/PrintLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PcdLib.h>
#include <Library/PchPciBdfLib.h>
#include <Register/PchRegsLpcAdl.h>
#include <Register/PchRegs.h>
#include <Register/PchRegsLpc.h>
#include <IndustryStandard/Pci30.h>
#include "PchInfoLibPrivate.h"


/**
  Return LPC Device Id

  @retval PCH_LPC_DEVICE_ID         PCH Lpc Device ID
**/
UINT16
PchGetLpcDid (
  VOID
  )
{
  UINT64  LpcBaseAddress;

  LpcBaseAddress = LpcPciCfgBase ();

  return PciSegmentRead16 (LpcBaseAddress + PCI_DEVICE_ID_OFFSET);
}

/**
  Return Pch Series

  @retval PCH_SERIES            Pch Series
**/
PCH_SERIES
PchSeries (
  VOID
  )
{
  PCH_SERIES        PchSer;
  static PCH_SERIES PchSeries = PCH_UNKNOWN_SERIES;

  if (PchSeries != PCH_UNKNOWN_SERIES) {
    return PchSeries;
  }

  PchSer = PchSeriesFromLpcDid (PchGetLpcDid ());

  PchSeries = PchSer;

  return PchSer;
}

/**
  Return Pch stepping type

  @retval PCH_STEPPING            Pch stepping type
**/
PCH_STEPPING
PchStepping (
  VOID
  )
{
  UINT8                RevId;
  UINT64               LpcBaseAddress;
  static PCH_STEPPING  PchStepping = PCH_STEPPING_MAX;

  if (PchStepping != PCH_STEPPING_MAX) {
    return PchStepping;
  }

  LpcBaseAddress = LpcPciCfgBase ();
  RevId = PciSegmentRead8 (LpcBaseAddress + PCI_REVISION_ID_OFFSET);

  RevId = PchSteppingFromRevId (RevId);

  PchStepping = RevId;

  return RevId;
}

/**
  Check if this is PCH P series

  @retval TRUE                It's PCH P series
  @retval FALSE               It's not PCH P series
**/
BOOLEAN
IsPchP (
  VOID
  )
{
  return (PchSeries () == PCH_P);
}

/**
  return support status for P2SB PCR 20-bit addressing

  @retval    TRUE
  @retval    FALSE
**/
BOOLEAN
IsP2sb20bPcrSupported (
  VOID
  )
{
  return FALSE;
}

/**
  Determine Pch Series based on Device Id

  @param[in] LpcDeviceId      Lpc Device Id

  @retval PCH_SERIES          Pch Series
**/
PCH_SERIES
PchSeriesFromLpcDid (
  IN UINT16 LpcDeviceId
  )
{
  return PCH_P;
}

/**
  Determine Pch Stepping based on Revision ID

  @param[in] RevId            Pch Revision Id

  @retval PCH_STEPPING        Pch Stepping
**/
PCH_STEPPING
PchSteppingFromRevId (
  IN UINT8 RevId
  )
{
  return RevId;
}


/**
  Check if this is PCH LP series

  @retval TRUE     It's PCH LP series
  @retval FALSE    It's not PCH LP series
**/
BOOLEAN
IsPchLp (
  VOID
  )
{
  return (PchSeries () == PCH_LP || PchSeries () == PCH_P || PchSeries () == PCH_M || PchSeries () == PCH_N );
}
/**
  Get RST mode supported by the silicon

  @retval RST_MODE    RST mode supported by silicon
**/

GLOBAL_REMOVE_IF_UNREFERENCED
struct PCH_SKU_STRING mSkuStrs[] = {
  //
  // ADL PCH LPC Device IDs
  //
  {V_ADL_PCH_P_LPC_CFG_DEVICE_ID_MB_0,   "ADL SKU 0"},
  {V_ADL_PCH_P_LPC_CFG_DEVICE_ID_MB_1,   "P Super SKU (SSKU)"},
  {V_ADL_PCH_P_LPC_CFG_DEVICE_ID_MB_2,   "P Premium"},
  {V_ADL_PCH_P_LPC_CFG_DEVICE_ID_MB_3,   "ADL No UFS"},
  {V_ADL_PCH_P_LPC_CFG_DEVICE_ID_MB_4,   "ADL SKU 4"},
  {V_ADL_PCH_P_LPC_CFG_DEVICE_ID_MB_5,   "ADL SKU 5"},

  {0xFFFF, NULL}
};

/**
  Get Pch Maximum Pcie Root Port Number

  @retval Pch Maximum Pcie Root Port Number
**/
UINT8
GetPchMaxPciePortNum (
  VOID
  )
{
  switch (PchSeries ()) {
    case PCH_P:
    case PCH_N:
      return 12;
    case PCH_S:
      return 28;
    default:
      return 0;
  }
}

/**
  Get Pch Maximum Serial IO I2C controllers number

  @retval Pch Maximum Serial IO I2C controllers number
**/
UINT8
GetPchMaxSerialIoI2cControllersNum (
  VOID
  )
{
  return 8;
}
