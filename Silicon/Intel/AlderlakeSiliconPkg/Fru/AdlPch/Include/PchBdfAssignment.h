/** @file
  Header file for AlderLake PCH devices PCI Bus Device Function map.

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _PCH_BDF_ASSIGNMENT_H_
#define _PCH_BDF_ASSIGNMENT_H_

#define NOT_PRESENT                     0xFF

//
// PCH PCIe Controllers
//
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_1          28
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_2          28
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_3          28
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_4          28
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_5          28
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_6          28
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_7          28
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_8          28
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_9          29
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_10         29
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_11         29
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_12         29
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_13         29
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_14         29
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_15         29
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_16         29
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_17         27
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_18         27
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_19         27
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_20         27
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_21         27
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_22         27
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_23         27
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_24         27
#ifdef PCH_ADPP
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_25         NOT_PRESENT
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_26         NOT_PRESENT
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_27         NOT_PRESENT
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_28         NOT_PRESENT
#else
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_25         26
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_26         26
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_27         26
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_28         26
#endif

//
// USB3 (XHCI) Controller PCI config
//
#define PCI_DEVICE_NUMBER_PCH_XHCI                    20
#define PCI_FUNCTION_NUMBER_PCH_XHCI                  0




//
// LPC Controller (D31:F0)
//
#define PCI_DEVICE_NUMBER_PCH_LPC                     31
#define PCI_FUNCTION_NUMBER_PCH_LPC                   0

//
// Primary to Sideband (P2SB) Bridge (D31:F1)
//
#define PCI_DEVICE_NUMBER_PCH_P2SB                    31
#define PCI_FUNCTION_NUMBER_PCH_P2SB                  1



//
// SPI Controller (D31:F5)
//
#define PCI_DEVICE_NUMBER_PCH_SPI                     31
#define PCI_FUNCTION_NUMBER_PCH_SPI                   5


#endif // _PCH_BDF_ASSIGNMENT_H_
