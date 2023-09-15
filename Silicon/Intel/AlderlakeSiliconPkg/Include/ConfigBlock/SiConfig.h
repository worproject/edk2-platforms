/** @file
  Si Config Block

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _SI_CONFIG_H_
#define _SI_CONFIG_H_

#define SI_CONFIG_REVISION  1

extern EFI_GUID gSiConfigGuid;


#pragma pack (push,1)

/**
  The Silicon Policy allows the platform code to publish a set of configuration
  information that the RC drivers will use to configure the silicon hardware.

  <b>Revision 1</b>:  - Initial version.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;  ///< Offset 0 - 27 Config Block Header
  //
  // Platform specific common policies that used by several silicon components.
  //
  UINT8  CsmFlag;                ///< offset 28 CSM status flag.@deprecated.
  /**
    This is used to skip the SSID programming in silicon code.
    When set to TRUE, silicon code will not do any SSID programming and platform code
    needs to handle that by itself properly.
    <b>0: FALSE</b>, 1: TRUE
  **/
  UINT8  SkipSsidProgramming;   ///< offset 29
  UINT8  RsvdBytes0[2];         ///< offset 30 - 31
  /**
    When SkipSsidProgramming is FALSE, silicon code will use this as default value
    to program the SVID for all internal devices.
    <b>0: use silicon default SVID 0x8086 </b>, Non-zero: use customized SVID.
  **/
  UINT16 CustomizedSvid;        ///< offset 32 - 33
  /**
    When SkipSsidProgramming is FALSE, silicon code will use this as default value
    to program the Sid for all internal devices.
    <b>0: use silicon default SSID 0x7270 </b>, Non-zero: use customized SSID.
  **/
  UINT16 CustomizedSsid;        ///< offset 34 - 35
  UINT32 *SsidTablePtr;             ///< offset 36 - 39
  /**
    Number of valid enties in SsidTablePtr.
    This is valid when SkipSsidProgramming is FALSE;
    <b>Default is 0.</b>
  **/
  UINT16 NumberOfSsidTableEntry;    ///< offset 40 - 41
  UINT8  RsvdBytes1[2];             ///< offset 42 - 43
  /**
    This is used to skip setting BIOS_DONE MSR during firmware update boot mode.
    When set to TRUE and boot mode is BOOT_ON_FLASH_UPDATE,
    skip setting BIOS_DONE MSR at EndofPei.
    <b>0: FALSE</b>, 1: TRUE
  **/
  UINT8  SkipBiosDoneWhenFwUpdate;  ///< Offset 44
  UINT8  RsvdBytes2[3];             ///< Offset 45 - 47
} SI_CONFIG;

#pragma pack (pop)


///
/// Subsystem Vendor ID / Subsystem ID
///
typedef struct {
  UINT16         SubSystemVendorId;
  UINT16         SubSystemId;
} SVID_SID_VALUE;

//
// Below is to match PCI_SEGMENT_LIB_ADDRESS () which can directly send to PciSegmentRead/Write functions.
//
typedef struct {
  union {
    struct {
      UINT32  Register:12;
      UINT32  Function:3;
      UINT32  Device:5;
      UINT32  Bus:8;
      UINT32  Reserved1:4;
      UINT32  Segment:16;
      UINT32  Reserved2:16;
    } Bits;
    UINT64    SegBusDevFuncRegister;
  } Address;
  SVID_SID_VALUE SvidSidValue;
  UINT32 Reserved;
} SVID_SID_INIT_ENTRY;


typedef struct {
  UINT32  SkipBus;
  UINT32  SkipDevice;
  UINT32  SkipFunction;
} SVID_SID_SKIP_TABLE;

#endif // _SI_CONFIG_H_
