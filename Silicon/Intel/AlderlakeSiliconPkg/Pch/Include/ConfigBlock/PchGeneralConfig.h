/** @file
  PCH General policy

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _PCH_GENERAL_CONFIG_H_
#define _PCH_GENERAL_CONFIG_H_


extern EFI_GUID gPchGeneralConfigGuid;
extern EFI_GUID gPchGeneralPreMemConfigGuid;

#pragma pack (push,1)

enum PCH_RESERVED_PAGE_ROUTE {
  PchReservedPageToLpc,                   ///< Port 80h cycles are sent to LPC.
  PchReservedPageToPcie                   ///< Port 80h cycles are sent to PCIe.
};

/**
  PCH General Configuration
  <b>Revision 1</b>:  - Initial version.
  <b>Revision 2</b>:  - Added AcpiL6dPmeHandling
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;                   ///< Config Block Header
  /**
    This member describes whether or not the Compatibility Revision ID (CRID) feature
    of PCH should be enabled. <b>0: Disable</b>; 1: Enable
  **/
  UINT32    Crid            :  1;
  /**
    Set to enable low latency of legacy IO.
    Some systems require lower IO latency irrespective of power.
    This is a tradeoff between power and IO latency.
    @note: Once this is enabled, DmiAspm, Pcie DmiAspm in SystemAgent
    and ITSS Clock Gating are forced to disabled.
    <b>0: Disable</b>, 1: Enable
  **/
  UINT32    LegacyIoLowLatency  :  1;
  /**
  Enables _L6D ACPI handler.
  PME GPE is shared by multiple devices So BIOS must verify the same in the ASL handler by reading offset for PMEENABLE and PMESTATUS bit
  <b>0: Disable</b>, 1: Enable
  **/
  UINT32    AcpiL6dPmeHandling  :  1;
  UINT32    RsvdBits0           : 29;       ///< Reserved bits
} PCH_GENERAL_CONFIG;

/**
  PCH General Pre-Memory Configuration
  <b>Revision 1</b>:  - Initial version.
  <b>Revision 2</b>:  - Added GpioOverride.
  <b>Revision 3</b>:  - Added IoeDebugEn, PmodeClkEn
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;                   ///< Config Block Header
  /**
    Control where the Port 80h cycles are sent, <b>0: LPC</b>; 1: PCI.
  **/
  UINT32    Port80Route     :  1;
  UINT32    IotgPllSscEn    :  1;       ///< Need to disable CPU Side SSC for A0 PO
  /**
    Gpio override Level
    -- <b>0: Disable</b>;
    -  1: Override Level 1 - Skips GPIO configuration in PEI/FSPM/FSPT phase
    -  2: Override Level 2 - Reserved for future use
  **/
  UINT32    GpioOverride    :  3;
  /**
    Enable/Disable IOE Debug. When enabled, IOE D2D Dfx link and clock will keep up for debug
    <b>0: Disable</b>; 1: Enable
  **/
  UINT32    IoeDebugEn      :  1;
  /**
    Enable/Disable PMODE clock. When enabled, Pmode clock will toggle for XDP use
    <b>0: Disable</b>; 1: Enable
  **/
  UINT32    PmodeClkEn      :  1;
  UINT32    RsvdBits0       : 25;       ///< Reserved bits
} PCH_GENERAL_PREMEM_CONFIG;

#pragma pack (pop)

#endif // _PCH_GENERAL_CONFIG_H_
