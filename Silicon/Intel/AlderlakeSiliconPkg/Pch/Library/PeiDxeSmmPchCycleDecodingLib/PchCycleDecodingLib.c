/** @file
  PCH cycle decoding configuration and query library.

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
#include <Library/PchCycleDecodingLib.h>
#include <Library/PchDmiLib.h>
#include <Library/BaseMemoryLib.h>
#include <Register/PchRegs.h>
#include <Register/PchRegsLpc.h>
#include <Register/SpiRegs.h>
#include <Library/EspiLib.h>
#include <Library/PchPciBdfLib.h>

typedef enum {
  SlaveLpcEspiCS0,
  SlaveEspiCS1,
  SlaveId_Max
} SLAVE_ID_INDEX;

/**
  Get PCH TCO base address.

  @param[out] Address                   Address of TCO base address.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid pointer passed.
**/
EFI_STATUS
PchTcoBaseGet (
  OUT UINT16                            *Address
  )
{
  if (Address == NULL) {
    DEBUG ((DEBUG_ERROR, "PchTcoBaseGet Error. Invalid pointer.\n"));
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  //
  // Read "TCO Base Address" from DMI
  // Don't read TCO base address from SMBUS PCI register since SMBUS might be disabled.
  //
  *Address = PchDmiGetTcoBase ();

  return EFI_SUCCESS;
}

/**
  Set PCH LPC/eSPI IO decode ranges.
  Program LPC/eSPI I/O Decode Ranges in DMI to the same value programmed in LPC/eSPI PCI offset 80h.
  Please check EDS for detail of LPC/eSPI IO decode ranges bit definition.
  Bit  12: FDD range
  Bit 9:8: LPT range
  Bit 6:4: ComB range
  Bit 2:0: ComA range

  @param[in] LpcIoDecodeRanges          LPC/eSPI IO decode ranges bit settings.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_UNSUPPORTED               DMIC.SRL is set.
**/
EFI_STATUS
PchLpcIoDecodeRangesSet (
  IN  UINT16                            LpcIoDecodeRanges
  )
{
  UINT64                                LpcBaseAddr;
  EFI_STATUS                            Status;

  //
  // Note: Inside this function, don't use debug print since it's could used before debug print ready.
  //

  LpcBaseAddr = LpcPciCfgBase ();

  //
  // check if setting is identical
  //
  if (LpcIoDecodeRanges == PciSegmentRead16 (LpcBaseAddr + R_LPC_CFG_IOD)) {
    return EFI_SUCCESS;
  }

  Status = PchDmiSetLpcIoDecodeRanges (LpcIoDecodeRanges);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  //
  // program LPC/eSPI PCI offset 80h.
  //
  PciSegmentWrite16 (LpcBaseAddr + R_LPC_CFG_IOD, LpcIoDecodeRanges);

  return Status;
}

/**
  Set PCH LPC/eSPI and eSPI CS1# IO enable decoding.
  Setup I/O Enables in DMI to the same value program in LPC/eSPI PCI offset 82h (LPC, eSPI CS0#) or A0h (eSPI CS1#).
  Note: Bit[15:10] of the source decode register is Read-Only. The IO range indicated by the Enables field
  in LPC/eSPI PCI offset 82h[13:10] or A0h[13:10] is always forwarded by DMI to subtractive agent for handling.
  Please check EDS for detail of Lpc/eSPI IO decode ranges bit definition.

  @param[in] IoEnableDecoding           LPC/eSPI IO enable decoding bit settings.
  @param[in] SlaveId                    Target ID (refer to SLAVE_ID_INDEX)

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_UNSUPPORTED               DMI configuration is locked
**/
EFI_STATUS
LpcEspiIoEnableDecodingSetHelper (
  IN  UINT16                            IoEnableDecoding,
  IN  SLAVE_ID_INDEX                    SlaveId
  )
{
  UINT64      LpcBaseAddr;
  EFI_STATUS  Status;
  UINT16      Cs1IoEnableDecodingOrg;
  UINT16      Cs0IoEnableDecodingOrg;
  UINT16      IoEnableDecodingMerged;

  LpcBaseAddr = LpcPciCfgBase ();

  Cs0IoEnableDecodingOrg = PciSegmentRead16 (LpcBaseAddr + R_LPC_CFG_IOE);

  if (IsEspiSecondSlaveSupported ()) {
    Cs1IoEnableDecodingOrg = PciSegmentRead16 (LpcBaseAddr + R_ESPI_CFG_CS1IORE);
  } else {
    Cs1IoEnableDecodingOrg = 0;
  }

  if (SlaveId == SlaveEspiCS1) {
    if (IoEnableDecoding == Cs1IoEnableDecodingOrg) {
      return EFI_SUCCESS;
    } else {
      IoEnableDecodingMerged = (Cs0IoEnableDecodingOrg | IoEnableDecoding);
    }
  } else {
    if ((IoEnableDecoding | Cs1IoEnableDecodingOrg) == Cs0IoEnableDecodingOrg) {
      return EFI_SUCCESS;
    } else {
      IoEnableDecodingMerged = (Cs1IoEnableDecodingOrg | IoEnableDecoding);
    }
  }

  Status = PchDmiSetLpcIoEnable (IoEnableDecodingMerged);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  //
  // program PCI offset 82h for LPC/eSPI.
  //
  PciSegmentWrite16 (LpcBaseAddr + R_LPC_CFG_IOE, IoEnableDecodingMerged);

  if (SlaveId == SlaveEspiCS1) {
    //
    // For eSPI CS1# device program eSPI PCI offset A0h.
    //
    PciSegmentWrite16 (LpcBaseAddr + R_ESPI_CFG_CS1IORE, IoEnableDecoding);
  }

  return Status;
}

/**
  Set PCH LPC and eSPI CS0# IO enable decoding.
  Setup I/O Enables in DMI to the same value program in LPC/eSPI PCI offset 82h.
  Note: Bit[15:10] of the source decode register is Read-Only. The IO range indicated by the Enables field
  in LPC/eSPI PCI offset 82h[13:10] is always forwarded by DMI to subtractive agent for handling.
  Please check EDS for detail of LPC/eSPI IO decode ranges bit definition.

  @param[in] LpcIoEnableDecoding        LPC IO enable decoding bit settings.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_UNSUPPORTED               DMIC.SRL is set.
**/
EFI_STATUS
PchLpcIoEnableDecodingSet (
  IN  UINT16                            LpcIoEnableDecoding
  )
{
  return LpcEspiIoEnableDecodingSetHelper (LpcIoEnableDecoding, SlaveLpcEspiCS0);
}
