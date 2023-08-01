/** @file
  Implementation of PeiBoardConfigLib.

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PiPei.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/PostCodeLib.h>
#include <Library/TimerLib.h>
#include <Library/GpioLib.h>
#include <PlatformBoardId.h>
#include <PlatformBoardConfig.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Library/PeiServicesLib.h>
#include <Library/PmcLib.h>
#include <Library/BoardConfigLib.h>



VOID
EFIAPI
InternalUpdateRvpBoardConfig (
  IN OUT UINT16         BoardId
  )
{
  //
  // Update Board Type/Platform Type/Platform Flavor
  //
  switch (BoardId) {
    case BoardIdAdlPDdr5Rvp:
      if(PcdSet64S (PcdAcpiDefaultOemTableId, ACPI_OEM_TABLE_ID_ADL_P_M) != EFI_SUCCESS)
      {
         DEBUG ((DEBUG_INFO, "Set PcdAcpiDefaultOemTableId error!!!\n"));
      }
      break;
  }
  DEBUG ((DEBUG_INFO, "PcdAcpiDefaultOemTableId is 0x%llX\n", PcdGet64 (PcdAcpiDefaultOemTableId)));
}

/**
  Procedure to detect current board HW configuration.

**/
VOID
GetBoardConfig (
  VOID
  )
{
  UINT16          BoardId;

  //
  // Get Platform Info and fill the PCD
  //
  BoardId   = BoardIdAdlPDdr5Rvp;
  PcdSet16S (PcdBoardId, BoardId);
  //
  // update RVP board config
  //
  InternalUpdateRvpBoardConfig (BoardId);

  DEBUG ((DEBUG_INFO, "Platform Information:\n"));
  DEBUG ((DEBUG_INFO, "BoardID: 0x%x\n", BoardId));

}

/**
  Count the number of GPIO settings in the Table.

  @param[in]  GpioTable   The pointer of GPIO config table
  @param[out] GpioCount   The number of GPIO config entries
**/
VOID
GetGpioTableSize (
  GPIO_INIT_CONFIG   *GpioTable,
  OUT UINT16         *GpioCount
  )
{
  *GpioCount = 0;
  if(GpioTable != NULL) {
    while (GpioTable[*GpioCount].GpioPad != 0 && *GpioCount < MAX_GPIO_PINS) {
      DEBUG ((DEBUG_INFO, "GpioTable[%d]->GpioPad = %x \n", *GpioCount, GpioTable[*GpioCount].GpioPad));
      (*GpioCount) ++;
    }
  } else {
    DEBUG ((DEBUG_INFO, "GpioTable is NULL\n"));
  }
  DEBUG ((DEBUG_INFO, "GetGpioTableSize() GpioCount = %d\n", *GpioCount));
}

/**
  Configures GPIO

  @param[in]  GpioTable       Point to Platform Gpio table
  @param[in]  GpioTableCount  Number of Gpio table entries
**/
STATIC
VOID
ConfigureGpio (
  IN GPIO_INIT_CONFIG                 *GpioTable,
  IN UINT16                           GpioTableCount
  )
{
  EFI_STATUS          Status;

  DEBUG ((DEBUG_INFO, "ConfigureGpio() Start\n"));

  Status = GpioConfigurePads (GpioTableCount, GpioTable);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "ConfigureGpio() End\n"));
}

/**
  Configure GPIO Before Memory is initialized.

  @param[in]  GpioTable  Pointer to Gpio table
**/
VOID
GpioInit (
  IN GPIO_INIT_CONFIG *GpioTable
  )
{
  UINT16             GpioCount;

  if (GpioTable != 0) {
    GpioCount = 0;
    GetGpioTableSize (GpioTable, &GpioCount);
    if (GpioCount != 0) {
      ConfigureGpio ((VOID *) GpioTable, (UINTN) GpioCount);
    }
  }
}
