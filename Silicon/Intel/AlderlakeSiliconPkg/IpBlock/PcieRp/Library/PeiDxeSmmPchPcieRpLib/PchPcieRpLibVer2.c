/** @file
  PCIE root port library.
  All function in this library is available for PEI, DXE, and SMM,
  But do not support UEFI RUNTIME environment call.

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PchPcrLib.h>
#include <Library/PchPcieRpLib.h>
#include <Register/PchRegs.h>
#include <PchBdfAssignment.h>
#include <PchPcieRpInfo.h>
#include <Register/PchPcieRpRegs.h>
#include <Register/PchPcrRegs.h>

#include "PchPcieRpLibInternal.h"

GLOBAL_REMOVE_IF_UNREFERENCED PCH_PCIE_CONTROLLER_INFO mPchPcieControllerInfo[] = {
  { PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_1,  PID_SPA,  0 },
  { PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_5,  PID_SPB,  4 },
  { PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_9,  PID_SPC,  8 },
  { PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_13, PID_SPD, 12 },
  { PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_17, PID_SPE, 16 }, // PCH-H only
  { PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_21, PID_SPF, 20 }  // PCH-H only
};

/**
  Get Pch Pcie Root Port Device and Function Number by Root Port physical Number

  @param[in]  RpNumber              Root port physical number. (0-based)
  @param[out] RpDev                 Return corresponding root port device number.
  @param[out] RpFun                 Return corresponding root port function number.

  @retval     EFI_SUCCESS           Root port device and function is retrieved
  @retval     EFI_INVALID_PARAMETER RpNumber is invalid
**/
EFI_STATUS
EFIAPI
GetPchPcieRpDevFun (
  IN  UINTN   RpNumber,
  OUT UINTN   *RpDev,
  OUT UINTN   *RpFun
  )
{
  UINTN       Index;
  UINTN       FuncIndex;
  UINT32      PciePcd;

  if (RpNumber >= GetPchMaxPciePortNum ()) {
    DEBUG ((DEBUG_ERROR, "GetPchPcieRpDevFun invalid RpNumber %x", RpNumber));
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  Index = RpNumber / PCH_PCIE_CONTROLLER_PORTS;
  FuncIndex = RpNumber - mPchPcieControllerInfo[Index].RpNumBase;
  *RpDev = mPchPcieControllerInfo[Index].DevNum;
  PciePcd = PchPcrRead32 (mPchPcieControllerInfo[Index].Pid, R_SPX_PCR_PCD);
  *RpFun = (PciePcd >> (FuncIndex * S_SPX_PCR_PCD_RP_FIELD)) & B_SPX_PCR_PCD_RP1FN;

  return EFI_SUCCESS;
}
