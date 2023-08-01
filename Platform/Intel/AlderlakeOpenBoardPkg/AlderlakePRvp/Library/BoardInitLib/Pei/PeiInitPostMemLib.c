/** @file

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PiPei.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>

#include <Library/BoardConfigLib.h>
#include <Library/GpioLib.h>
#include <Library/IoLib.h>
#include <PlatformBoardId.h>
#include <PlatformBoardConfig.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Library/PchInfoLib.h>
#include <Library/HobLib.h>



/**
  Alderlake P boards configuration init function for PEI post memory phase.

  @retval EFI_SUCCESS             The function completed successfully.
**/
EFI_STATUS
EFIAPI
AdlPInit (
  VOID
  )
{
  UINT16            GpioCount;
  UINTN             Size;
  EFI_STATUS        Status;
  GPIO_INIT_CONFIG  *GpioTable;
  //
  // GPIO Table Init
  //
  Status = EFI_SUCCESS;
  GpioCount = 0;
  Size = 0;
  GpioTable = NULL;
  //
  // GPIO Table Init
  //
  //
  // GPIO Table Init, Update PostMem GPIO table to PcdBoardGpioTable
  //
  GpioTable = (GPIO_INIT_CONFIG *)PcdGetPtr(VpdPcdBoardGpioTable);

  GetGpioTableSize (GpioTable, &GpioCount);
  //
  // Increase GpioCount for the zero terminator.
  //
  GpioCount ++;
  Size = (UINTN) (GpioCount * sizeof (GPIO_INIT_CONFIG));
  Status = PcdSetPtrS (PcdBoardGpioTable, &Size, GpioTable);
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/**
  Configures GPIO

  @param[in]  GpioTable       Point to Platform Gpio table
  @param[in]  GpioTableCount  Number of Gpio table entries

**/
VOID
ConfigureGpio (
  IN GPIO_INIT_CONFIG                 *GpioDefinition,
  IN UINT16                           GpioTableCount
  )
{
  DEBUG ((DEBUG_INFO, "ConfigureGpio() Start\n"));

  GpioConfigurePads (GpioTableCount, GpioDefinition);

  DEBUG ((DEBUG_INFO, "ConfigureGpio() End\n"));
}

/**
  Configure GPIO, TouchPanel, HDA, PMC, TBT etc.

  @retval  EFI_SUCCESS   Operation success.
**/
EFI_STATUS
EFIAPI
AdlPBoardInitBeforeSiliconInit (
  VOID
  )
{
  AdlPInit ();
  GpioInit (PcdGetPtr (PcdBoardGpioTable));

  return EFI_SUCCESS;
}
