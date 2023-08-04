/** @file
  File to contain all the hardware specific stuff for the Smm Sx dispatch protocol.

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#include "PchSmmHelpers.h"
#include <Register/PmcRegs.h>
#include "PchSmiHelper.h"

extern BOOLEAN               mS3SusStart;
#define PROGRESS_CODE_S3_SUSPEND_END  PcdGet32 (PcdProgressCodeS3SuspendEnd)

GLOBAL_REMOVE_IF_UNREFERENCED CONST PCH_SMM_SOURCE_DESC mSxSourceDesc = {
  PCH_SMM_NO_FLAGS,
  {
    {
      {
        ACPI_ADDR_TYPE,
        {R_ACPI_IO_SMI_EN}
      },
      S_ACPI_IO_SMI_EN,
      N_ACPI_IO_SMI_EN_ON_SLP_EN
    },
    NULL_BIT_DESC_INITIALIZER
  },
  {
    {
      {
        ACPI_ADDR_TYPE,
        {R_ACPI_IO_SMI_STS}
      },
      S_ACPI_IO_SMI_STS,
      N_ACPI_IO_SMI_STS_ON_SLP_EN
    }
  },
  {
    {
      ACPI_ADDR_TYPE,
      {R_ACPI_IO_SMI_STS}
    },
    S_ACPI_IO_SMI_STS,
    N_ACPI_IO_SMI_STS_ON_SLP_EN
  }
};

/**
  Get the Sleep type

  @param[in] Record               No use
  @param[out] Context             The context that includes SLP_TYP bits to be filled

**/
VOID
EFIAPI
SxGetContext (
  IN  DATABASE_RECORD    *Record,
  OUT PCH_SMM_CONTEXT    *Context
  )
{
  UINT32  Pm1Cnt;

  Pm1Cnt = IoRead32 ((UINTN) (mAcpiBaseAddr + R_ACPI_IO_PM1_CNT));

  ///
  /// By design, the context phase will always be ENTRY
  ///
  Context->Sx.Phase = SxEntry;

  ///
  /// Map the PM1_CNT register's SLP_TYP bits to the context type
  ///
  switch (Pm1Cnt & B_ACPI_IO_PM1_CNT_SLP_TYP) {
    case V_ACPI_IO_PM1_CNT_S0:
      Context->Sx.Type = SxS0;
      break;

    case V_ACPI_IO_PM1_CNT_S1:
      Context->Sx.Type = SxS1;
      break;

    case V_ACPI_IO_PM1_CNT_S3:
      Context->Sx.Type = SxS3;
      break;

    case V_ACPI_IO_PM1_CNT_S4:
      Context->Sx.Type = SxS4;
      break;

    case V_ACPI_IO_PM1_CNT_S5:
      Context->Sx.Type = SxS5;
      break;

    default:
      ASSERT (FALSE);
      break;
  }
}

/**
  Check whether sleep type of two contexts match

  @param[in] Context1             Context 1 that includes sleep type 1
  @param[in] Context2             Context 2 that includes sleep type 2

  @retval FALSE                   Sleep types match
  @retval TRUE                    Sleep types don't match
**/
BOOLEAN
EFIAPI
SxCmpContext (
  IN PCH_SMM_CONTEXT     *Context1,
  IN PCH_SMM_CONTEXT     *Context2
  )
{
  return (BOOLEAN) (Context1->Sx.Type == Context2->Sx.Type);
}
