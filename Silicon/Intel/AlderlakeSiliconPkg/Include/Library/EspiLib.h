/** @file
  Header file for PchEspiLib.
  All function in this library is available for PEI, DXE, and SMM,

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _ESPI_LIB_H_
#define _ESPI_LIB_H_

/**
  Checks if there's second device connected under CS#1

  @retval TRUE      There's second device
  @retval FALSE     There's no second device
**/
BOOLEAN
IsEspiSecondSlaveSupported (
  VOID
  );

/**
  Is eSPI enabled in strap.

  @retval TRUE          Espi is enabled in strap
  @retval FALSE         Espi is disabled in strap
**/
BOOLEAN
IsEspiEnabled (
  VOID
  );

#endif // _ESPI_LIB_H_
