/** @file
  Implements SmmControlPei.c

  Copyright (C) 2024 Advanced Micro Devices, Inc. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/PeiServicesLib.h>
#include <Ppi/SmmControl.h>
#include <Include/FchRegistersCommon.h>

/**
  This routine generates an SMI

  @param[in]       PeiServices           Describes the list of possible PEI Services.
  @param[in]       This                  The pointer to this instance of this PPI.
  @param[in, out]  ArgumentBuffer        The buffer of argument
  @param[in, out]  ArgumentBufferSize    The size of the argument buffer
  @param[in]       Periodic              TRUE to indicate a periodical SMI
  @param[in]       ActivationInterval    Interval of periodic SMI

  @retval  EFI_SUCCESS            SMI generated.
  @retval  EFI_INVALID_PARAMETER  Some parameter value passed is not supported
**/
EFI_STATUS
EFIAPI
PeiTrigger (
  IN     EFI_PEI_SERVICES     **PeiServices,
  IN     PEI_SMM_CONTROL_PPI  *This,
  IN OUT INT8                 *ArgumentBuffer OPTIONAL,
  IN OUT UINTN                *ArgumentBufferSize OPTIONAL,
  IN     BOOLEAN              Periodic OPTIONAL,
  IN     UINTN                ActivationInterval OPTIONAL
  );

/**
  Clear SMI related chipset status.

  @param[in]  PeiServices           Describes the list of possible PEI Services.
  @param[in]  This                  The pointer to this instance of this PPI.
  @param[in]  Periodic              TRUE to indicate a periodical SMI.

  @return  Return value from ClearSmi()
**/
EFI_STATUS
EFIAPI
PeiClear (
  IN EFI_PEI_SERVICES     **PeiServices,
  IN PEI_SMM_CONTROL_PPI  *This,
  IN BOOLEAN              Periodic OPTIONAL
  );

STATIC PEI_SMM_CONTROL_PPI  mSmmControlPpi = {
  PeiTrigger,
  PeiClear
};

STATIC EFI_PEI_PPI_DESCRIPTOR  mPpiList = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gPeiSmmControlPpiGuid,
  &mSmmControlPpi
};

/**
 Init related registers

 @param [in]        None

 @retval  EFI_LOAD_ERROR  Get ACPI MMIO base error.
 @retval  EFI_SUCCESS     The function completed successfully..
*/
EFI_STATUS
SmmControlPeiPreInit (
  VOID
  )
{
  UINT16  SmmControlData16;
  UINT16  SmmControlMask16;
  UINT32  SmmControlData32;
  UINT8   SmmControlIndex;
  UINT16  AcpiPmBase;

  //
  // Get ACPI MMIO base and AcpiPm1EvtBlk address
  //
  AcpiPmBase = MmioRead16 (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG60);

  if (0 == AcpiPmBase) {
    return EFI_LOAD_ERROR;
  }

  //
  // Clean up all SMI status and enable bits
  //
  // Clear all SmiControl registers
  SmmControlData32 = 0;
  for (SmmControlIndex = FCH_SMI_REGA0; SmmControlIndex <= FCH_SMI_REGC4; SmmControlIndex += 4) {
    MmioWrite32 (ACPI_MMIO_BASE + SMI_BASE + SmmControlIndex, SmmControlData32);
  }

  // Clear all SmiStatus registers (SmiStatus0-4)
  SmmControlData32 = 0xFFFFFFFF;
  MmioWrite32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG80, SmmControlData32);
  MmioWrite32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG84, SmmControlData32);
  MmioWrite32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG88, SmmControlData32);
  MmioWrite32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG8C, SmmControlData32);
  MmioWrite32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG90, SmmControlData32);

  //
  // If SCI is not enabled, clean up all ACPI PM status/enable registers
  //
  SmmControlData16 = IoRead16 (AcpiPmBase + R_FCH_ACPI_PM_CONTROL);
  if (!(SmmControlData16 & BIT0)) {
    // Clear WAKE_EN, RTC_EN, SLPBTN_EN, GBL_EN and TMR_EN
    SmmControlData16 = 0;
    SmmControlMask16 = (UINT16) ~(BIT15 + BIT10 + BIT9 + BIT5 + BIT0);
    IoAndThenOr16 (AcpiPmBase + R_FCH_ACPI_PM1_ENABLE, SmmControlMask16, SmmControlData16);

    // Clear WAKE_STS, RTC_STS, SLPBTN_STS, GBL_STS and TMR_STS
    SmmControlData16 = BIT15 + BIT10 + BIT9 + BIT5 + BIT0;
    IoWrite16 (AcpiPmBase + R_FCH_ACPI_PM1_STATUS, SmmControlData16);
  }

  //
  // Set the EOS Bit
  // Clear SmiEnB to enable SMI function
  //
  SmmControlData32  = MmioRead32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG98);
  SmmControlData32 |= BIT28;
  SmmControlData32 &= ~BIT31;
  MmioWrite32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG98, SmmControlData32);

  //
  // Enable CmdPort SMI
  //
  SmmControlData32  = MmioRead32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGB0);
  SmmControlData32 &= ~(BIT22 + BIT23);
  SmmControlData32 |= BIT22;
  MmioWrite32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGB0, SmmControlData32);

  return EFI_SUCCESS;
}

/**
  Clear the SMI status


  @retval EFI_SUCCESS             The function completes successfully
**/
EFI_STATUS
ClearSmi (
  VOID
  )
{
  UINT32  SmmControlData32;

  //
  // Clear SmiCmdPort Status Bit
  //
  SmmControlData32 = BIT11;
  MmioWrite32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG88, SmmControlData32);

  //
  // Set the EOS Bit if it is currently cleared so we can get an SMI otherwise
  // leave the register alone
  //
  SmmControlData32 = MmioRead32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG98);
  if ((SmmControlData32 & BIT28) == 0) {
    SmmControlData32 |= BIT28;
    MmioWrite32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG98, SmmControlData32);
  }

  return EFI_SUCCESS;
}

/**
  This routine generates an SMI

  @param[in]       PeiServices           Describes the list of possible PEI Services.
  @param[in]       This                  The pointer to this instance of this PPI.
  @param[in, out]  ArgumentBuffer        The buffer of argument
  @param[in, out]  ArgumentBufferSize    The size of the argument buffer
  @param[in]       Periodic              TRUE to indicate a periodical SMI
  @param[in]       ActivationInterval    Interval of periodic SMI

  @retval  EFI_SUCCESS            SMI generated.
  @retval  EFI_INVALID_PARAMETER  Some parameter value passed is not supported
**/
EFI_STATUS
EFIAPI
PeiTrigger (
  IN     EFI_PEI_SERVICES     **PeiServices,
  IN     PEI_SMM_CONTROL_PPI  *This,
  IN OUT INT8                 *ArgumentBuffer OPTIONAL,
  IN OUT UINTN                *ArgumentBufferSize OPTIONAL,
  IN     BOOLEAN              Periodic OPTIONAL,
  IN     UINTN                ActivationInterval OPTIONAL
  )
{
  UINT8   bIndex;
  UINT8   bData;
  UINT32  SmmControlData32;
  UINT16  SmiCmdPort;

  if (Periodic) {
    return EFI_INVALID_PARAMETER;
  }

  if (NULL == ArgumentBuffer) {
    bIndex = 0xff;
  } else {
    bIndex = *ArgumentBuffer;
  }

  if (NULL == ArgumentBufferSize) {
    bData = 0xff;
  } else {
    bData = (UINT8)*ArgumentBufferSize;
  }

  //
  // Enable CmdPort SMI
  //
  SmmControlData32  = MmioRead32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGB0);
  SmmControlData32 &= ~(BIT22 + BIT23);
  SmmControlData32 |= BIT22;
  MmioWrite32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGB0, SmmControlData32);

  SmiCmdPort = PcdGet16 (PcdAmdFchCfgSmiCmdPortAddr);

  //
  // Issue command port SMI
  //
  IoWrite16 (SmiCmdPort, (bData << 8) + bIndex);
  return EFI_SUCCESS;
}

/**
  Clear SMI related chipset status.

  @param[in]  PeiServices           Describes the list of possible PEI Services.
  @param[in]  This                  The pointer to this instance of this PPI.
  @param[in]  Periodic              TRUE to indicate a periodical SMI.

  @return  Return value from ClearSmi()
**/
EFI_STATUS
EFIAPI
PeiClear (
  IN EFI_PEI_SERVICES     **PeiServices,
  IN PEI_SMM_CONTROL_PPI  *This,
  IN BOOLEAN              Periodic OPTIONAL
  )
{
  if (Periodic) {
    return EFI_INVALID_PARAMETER;
  }

  return ClearSmi ();
}

/**
  This is the constructor for the SMM Control Ppi.

  This function installs PEI_SMM_CONTROL_PPI.

  @param   FileHandle       Handle of the file being invoked.
  @param   PeiServices      Describes the list of possible PEI Services.

  @retval EFI_UNSUPPORTED There's no Intel ICH on this platform
  @return The status returned from PeiServicesInstallPpi().

--*/
EFI_STATUS
SmmControlPeiEntry (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS     Status;
  EFI_BOOT_MODE  BootMode;

  Status = PeiServicesGetBootMode (&BootMode);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "PeiSmmControl Enter\n"));

  if (BootMode != BOOT_ON_S3_RESUME) {
    return EFI_UNSUPPORTED;
  }

  //
  // Initialize EFI library
  //
  Status = SmmControlPeiPreInit ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = PeiServicesInstallPpi (&mPpiList);
  ASSERT_EFI_ERROR (Status);

  return Status;
}
