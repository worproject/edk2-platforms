/** @file
  Function prototype of BoardConfigLib.

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _BOARD_CONFIG_LIB_H_
#define _BOARD_CONFIG_LIB_H_

#include <Library/GpioLib.h>
#include <PlatformBoardConfig.h>

#define SIZE_OF_FIELD(TYPE, Field) (sizeof (((TYPE *)0)->Field))

#define SIZE_OF_TABLE(TABLE, TYPE) (sizeof (TABLE) / sizeof (TYPE))

#define BOARD_CONFIG PLATFORM_INFO

#define PRE_MEM        0
#define POST_MEM       1
#define EARLY_PRE_MEM  2

/**
  Procedure to detect current board HW configuration.

**/
VOID
EFIAPI
GetBoardConfig (
  VOID
  );


/**
  Count the number of GPIO settings in the Table.

  @param[in]  GpioTable   The pointer of GPIO config table
  @param[out] GpioCount   The number of GPIO config entries
**/
VOID
GetGpioTableSize (
  GPIO_INIT_CONFIG   *GpioTable,
  OUT UINT16         *GpioCount
  );


/**
  Configure GPIO pads in PEI phase.

  @param[in]  GpioTable  Pointer to Gpio table
**/
VOID
GpioInit (
  IN GPIO_INIT_CONFIG *GpioTable
  );

#endif // _BOARD_CONFIG_LIB_H_
