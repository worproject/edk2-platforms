/** @file
  Build time limits of PCH resources.

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _PCH_LIMITS_H_
#define _PCH_LIMITS_H_
/*
 * Defines povided in this file are indended to be used only where static value
 * is needed. They are set to values which allow to accomodate multiple projects
 * needs. Where runtime usage is possible please used dedicated functions from
 * PchInfoLib to retrieve accurate values
 */



//
// PCIe limits
//
#define PCH_MAX_PCIE_ROOT_PORTS             28
#define PCH_MAX_PCIE_CONTROLLERS            7

//
// PCIe clocks limits
//
#define PCH_MAX_PCIE_CLOCKS                 18

//
// DMI lanes
//
#define PCH_MAX_DMI_LANES                   8

//
// SerialIo limits
//
#define PCH_MAX_SERIALIO_I2C_CONTROLLERS      8
#define PCH_MAX_SERIALIO_SPI_CONTROLLERS      7
#define PCH_MAX_SERIALIO_SPI_CHIP_SELECTS     2
#define PCH_MAX_SERIALIO_UART_CONTROLLERS     7

//
// Number of eSPI slaves
//
#define PCH_MAX_ESPI_SLAVES                  2

#endif // _PCH_LIMITS_H_
