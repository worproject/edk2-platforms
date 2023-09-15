/** @file
  Power Management policy

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _PM_CONFIG_H_
#define _PM_CONFIG_H_

#include <ConfigBlock.h>

extern EFI_GUID gPmConfigGuid;

#pragma pack (push,1)

/**
  Description of Global Reset Trigger/Event Mask register
**/
typedef union {
  struct {
    UINT32 Reserved1     : 1;
    UINT32 Pbo           : 1;
    UINT32 PmcUncErr     : 1;
    UINT32 PchThrm       : 1;
    UINT32 MePbo         : 1;
    UINT32 CpuThrm       : 1;
    UINT32 Megbl         : 1;
    UINT32 LtReset       : 1;
    UINT32 PmcWdt        : 1;
    UINT32 MeWdt         : 1;
    UINT32 PmcFw         : 1;
    UINT32 PchpwrFlr     : 1;
    UINT32 SyspwrFlr     : 1;
    UINT32 Reserved2     : 1;
    UINT32 MiaUxsErr     : 1;
    UINT32 MiaUxErr      : 1;
    UINT32 CpuThrmWdt    : 1;
    UINT32 MeUncErr      : 1;
    UINT32 AdrGpio       : 1;
    UINT32 OcwdtNoicc    : 1;
    UINT32 OcwdtIcc      : 1;
    UINT32 CseHecUncErr  : 1;
    UINT32 PmcSramUncErr : 1;
    UINT32 PmcIromParity : 1;
    UINT32 PmcRfFusaErr  : 1;
    UINT32 Reserved3     : 4;
    UINT32 PpbrParityErr : 1;
    UINT32 Reserved4     : 2;
  } Field;
  UINT32 Value;
} PMC_GLOBAL_RESET_MASK;

typedef union {
  struct {
    UINT32  HostResetTimeout : 1;
    UINT32  SxEntryTimeout : 1;
    UINT32  HostRstProm : 1;
    UINT32  HsmbMsg : 1;
    UINT32  Pmc3Strike : 1;
    UINT32  FwGblrstScratch5 : 1;
    UINT32  PmcDmaTimeout : 1;
    UINT32  EspiType7 : 1;
    UINT32  EspiType8 : 1;
    UINT32  FwGblRstScratch10 : 1;
    UINT32  FwGblRstScratch11 : 1;
    UINT32  LpmFwErr : 1;
    UINT32  BscanMode : 1;
    UINT32  SlpLvlRspErr : 1;
    UINT32  FwGblrstScratch15 : 1;
    UINT32  FwGblrstScratch16 : 1;
    UINT32  FwGblrstScratch17 : 1;
    UINT32  FwGblrstScratch18 : 1;
    UINT32  FwGblrstScratch19 : 1;
    UINT32  FwGblrstScratch20 : 1;
    UINT32  FwGblrstScratch21 : 1;
    UINT32  FwGblrstScratch22 : 1;
    UINT32  FwGblrstScratch23 : 1;
    UINT32  Rsvd : 9;
  } Field;
  UINT32 Value;
} PMC_GLOBAL_RESET_MASK1;


#pragma pack (pop)

#endif // _PM_CONFIG_H_
