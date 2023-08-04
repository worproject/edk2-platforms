/** @file
  PCH PCIe Bus Device Function Library.
  All functions from this library are available in PEI, DXE, and SMM,
  But do not support UEFI RUNTIME environment call.

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Base.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PchPcieRpLib.h>
#include <Register/PchRegs.h>
#include <PchBdfAssignment.h>

/**
  Check if a Device is present for PCH FRU
  If the data is defined for PCH RFU return it
  If the data is not defined (Device is NOT present) assert.

  @param[in]  DataToCheck       Device or Function number to check

  @retval Device or Function number value if defined for PCH FRU
          0xFF if not present in PCH FRU
**/
UINT8
CheckAndReturn (
  UINT8 DataToCheck
  )
{
  if (DataToCheck == NOT_PRESENT) {
    ASSERT (FALSE);
  }
  return DataToCheck;
}

/**
  Get P2SB PCI device number

  @retval PCI dev number
**/
UINT8
P2sbDevNumber (
  VOID
  )
{
  return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_P2SB);
}

/**
  Get P2SB PCI function number

  @retval PCI fun number
**/
UINT8
P2sbFuncNumber (
  VOID
  )
{
  return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_P2SB);
}

/**
  Get P2SB controller address that can be passed to the PCI Segment Library functions.

  @retval P2SB controller address in PCI Segment Library representation
**/
UINT64
P2sbPciCfgBase (
  VOID
  )
{
  return PCI_SEGMENT_LIB_ADDRESS (
           DEFAULT_PCI_SEGMENT_NUMBER_PCH,
           DEFAULT_PCI_BUS_NUMBER_PCH,
           P2sbDevNumber (),
           P2sbFuncNumber (),
           0
           );
}



/**
  Returns PCH SPI Device number

  @retval UINT8   PCH SPI Device number
**/
UINT8
SpiDevNumber (
  VOID
  )
{
  return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_SPI);
}

/**
  Returns PCH SPI Function number

  @retval UINT8   PCH SPI Function number
**/
UINT8
SpiFuncNumber (
  VOID
  )
{
  return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_SPI);
}

/**
  Returns PCH SPI PCI Config Space base address

  @retval  UINT64  PCH SPI Config Space base address
**/
UINT64
SpiPciCfgBase (
  VOID
  )
{
  return PCI_SEGMENT_LIB_ADDRESS (
           DEFAULT_PCI_SEGMENT_NUMBER_PCH,
           DEFAULT_PCI_BUS_NUMBER_PCH,
           SpiDevNumber (),
           SpiFuncNumber (),
           0
           );
}

/**
  Get XHCI controller PCIe Device Number

  @retval XHCI controller PCIe Device Number
**/
UINT8
PchXhciDevNumber (
  VOID
  )
{
  return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_XHCI);
}

/**
  Get XHCI controller PCIe Function Number

  @retval XHCI controller PCIe Function Number
**/
UINT8
PchXhciFuncNumber (
  VOID
  )
{
  return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_XHCI);
}

/**
  Get LPC controller PCIe Device Number

  @retval LPC controller PCIe Device Number
**/
UINT8
LpcDevNumber (
  VOID
  )
{
  return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_LPC);
}

/**
  Get LPC controller PCIe Function Number

  @retval LPC controller PCIe Function Number
**/
UINT8
LpcFuncNumber (
  VOID
  )
{
  return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_LPC);
}

/**
  Returns PCH LPC device PCI base address.

  @retval                   PCH LPC PCI base address.
**/
UINT64
LpcPciCfgBase (
  VOID
  )
{
  return PCI_SEGMENT_LIB_ADDRESS (
           DEFAULT_PCI_SEGMENT_NUMBER_PCH,
           DEFAULT_PCI_BUS_NUMBER_PCH,
           LpcDevNumber (),
           LpcFuncNumber (),
           0
           );
}



/**
  Get PCH PCIe controller PCIe Device Number

  @param[in]  RpIndex       Root port physical number. (0-based)

  @retval PCH PCIe controller PCIe Device Number
**/
UINT8
PchPcieRpDevNumber (
  IN  UINTN   RpIndex
  )
{
  switch (RpIndex) {
    case 0:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_1);
    case 1:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_2);
    case 2:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_3);
    case 3:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_4);
    case 4:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_5);
    case 5:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_6);
    case 6:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_7);
    case 7:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_8);
    case 8:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_9);
    case 9:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_10);
    case 10:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_11);
    case 11:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_12);
    case 12:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_13);
    case 13:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_14);
    case 14:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_15);
    case 15:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_16);
    case 16:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_17);
    case 17:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_18);
    case 18:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_19);
    case 19:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_20);
    case 20:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_21);
    case 21:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_22);
    case 22:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_23);
    case 23:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_24);
    case 24:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_25);
    case 25:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_26);
    case 26:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_27);
    case 27:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_28);

    default:
      ASSERT (FALSE);
      return 0xFF;
  }
}

/**
  Get PCH PCIe controller PCIe Function Number
  Note:
  For Client PCH generations Function Number can be various
  depending on "Root Port Function Swapping". For such cases
  Function Number  MUST be obtain from proper register.
  For Server PCHs we have no "Root Port Function Swapping"
  and we can return fixed Function Number.
  To address this difference in this, PCH generation independent,
  library we should call specific function in PchPcieRpLib.

  @param[in]  RpIndex       Root port physical number. (0-based)

  @retval PCH PCIe controller PCIe Function Number
**/
UINT8
PchPcieRpFuncNumber (
  IN  UINTN   RpIndex
  )
{
  UINTN   Device;
  UINTN   Function;

  GetPchPcieRpDevFun (RpIndex, &Device, &Function);

  return (UINT8)Function;
}

