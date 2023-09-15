/** @file
  PCH private PMC Library for all PCH generations.
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
#include <Library/BaseMemoryLib.h>
#include <Library/PmcLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/TimerLib.h>
#include <Library/PmcPrivateLib.h>
#include <PchReservedResources.h>
#include <Register/PchRegs.h>
#include <Register/PmcRegs.h>
#include <IndustryStandard/Pci30.h>
#include <Library/PchPciBdfLib.h>


/**
  This function sets SW SMI Rate.

  @param[in] SwSmiRate        Refer to PMC_SWSMI_RATE for possible values
**/
VOID
PmcSetSwSmiRate (
  IN PMC_SWSMI_RATE          SwSmiRate
  )
{
  UINT32        PchPwrmBase;
  STATIC UINT8  SwSmiRateRegVal[4] = {
    V_PMC_PWRM_GEN_PMCON_A_SWSMI_RTSL_1_5MS,
    V_PMC_PWRM_GEN_PMCON_A_SWSMI_RTSL_16MS,
    V_PMC_PWRM_GEN_PMCON_A_SWSMI_RTSL_32MS,
    V_PMC_PWRM_GEN_PMCON_A_SWSMI_RTSL_64MS
  };

  ASSERT (SwSmiRate <= PmcSwSmiRate64ms);

  PchPwrmBase = PmcGetPwrmBase ();

  //
  // SWSMI_RATE_SEL BIT (PWRMBASE offset 1020h[7:6]) bits are in RTC well
  //
  MmioAndThenOr8 (
    PchPwrmBase + R_PMC_PWRM_GEN_PMCON_A,
    (UINT8)~B_PMC_PWRM_GEN_PMCON_A_SWSMI_RTSL,
    SwSmiRateRegVal[SwSmiRate]
    );
}

/**
  This function sets Periodic SMI Rate.

  @param[in] PeriodicSmiRate        Refer to PMC_PERIODIC_SMI_RATE for possible values
**/
VOID
PmcSetPeriodicSmiRate (
  IN PMC_PERIODIC_SMI_RATE    PeriodicSmiRate
  )
{
  UINT32        PchPwrmBase;
  STATIC UINT8  PeriodicSmiRateRegVal[4] = {
    V_PMC_PWRM_GEN_PMCON_A_PER_SMI_8S,
    V_PMC_PWRM_GEN_PMCON_A_PER_SMI_16S,
    V_PMC_PWRM_GEN_PMCON_A_PER_SMI_32S,
    V_PMC_PWRM_GEN_PMCON_A_PER_SMI_64S
  };

  ASSERT (PeriodicSmiRate <= PmcPeriodicSmiRate64s);

  PchPwrmBase = PmcGetPwrmBase ();

  MmioAndThenOr8 (
    PchPwrmBase + R_PMC_PWRM_GEN_PMCON_A,
    (UINT8)~B_PMC_PWRM_GEN_PMCON_A_PER_SMI_SEL,
    PeriodicSmiRateRegVal[PeriodicSmiRate]
    );
}
