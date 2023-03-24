/** @file
  Secondary System Description Table (SSDT) for IO Virtualization SoC Expansion

  The IO virtualization blocks on Arm Reference Design (RD) platforms allow
  connecting PCIe root bus as well as other non-PCIe SoC peripherals. Each of
  these IO virtualization blocks consists of an instance of SMMUv3, a GIC-ITS
  and a NCI (network chip interconnect) to support traffic flow and address
  mapping, as required. The PCIe root bus or the SoC peripherals connect to the
  IO virtualization block over ports namely x4_0, x4_1, x8 and x16.

  Some of the RD platforms utilize one or more IO virtualization blocks to
  connect non-PCIe devices mapped in the SoC expansion address space. One
  such instance of SoC expansion block consists of a set of non-PCIe devices
  that includes two PL011 UART controllers, two PL330 DMA controllers and
  few additional memory nodes. The devices in this SoC expansion block are
  placed at fixed offsets from a base address in the SoC expansion address
  space and the read/write accesses to these devices are routed by the IO
  virtualization block.

  The table below lists the address offset, address space size and interrupts
  used for the devices present in each instance of this SoC expansion block
  that is connected to the IO Virtualization block.
  +-------------------------------------------------------------------------------+
  | Port |  Peripheral   |             Memory map              | Size | Interrupt |
  |  #   |               |-------------------------------------|      |    ID     |
  |      |               | Start Addr Offset | End Addr Offset |      |           |
  +-------------------------------------------------------------------------------+
  | x4_0 | PL011_UART0   |     0x0000_0000   |    0x0000_FFFF  | 64KB |    492    |
  |-------------------------------------------------------------------------------|
  | x4_1 | PL011_DMA0_NS |     0x1000_0000   |    0x1000_FFFF  | 64KB | 493-501   |
  |-------------------------------------------------------------------------------|
  |  x8  | PL011_UART1   |     0x2000_0000   |    0x2000_FFFF  | 64KB |    502    |
  |-------------------------------------------------------------------------------|
  |  x16 | PL011_DMA1_NS |     0x3000_0000   |    0x3000_FFFF  | 64KB | 503-511   |
  +-------------------------------------------------------------------------------+

  This SSDT ACPI table lists the SoC expansion block devices connected via the
  IO Virtualization block on RD-N2 platform variants and mapped to SoC expansion
  address at an offset of 0x10_8000_0000 from each chip's base address.

  Copyright (c) 2023, Arm Limited. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent

  @par Specification Reference:
    - ACPI 6.4, Chapter 5, Section 5.2.11.2, Secondary System Description Table
**/

#include "IoVirtSoCExp.h"
#include "SgiAcpiHeader.h"

DefinitionBlock ("SsdtIoVirtSocExp.aml", "SSDT", 2, "ARMLTD", "ARMSGI",
                 EFI_ACPI_ARM_OEM_REVISION) {
  Scope (_SB)
  {

    // IO Virtualization SoC Expansion - PL011 UART
#if (FixedPcdGet32 (PcdIoVirtSocExpBlkUartEnable) == 1)
    RD_IOVIRT_SOC_EXP_COM_INIT(2, 0, UART_START(0), 492)
    RD_IOVIRT_SOC_EXP_COM_INIT(3, 0, UART_START(1), 502)

#if (FixedPcdGet32 (PcdChipCount) > 1)
    RD_IOVIRT_SOC_EXP_COM_INIT(4, 1, UART_START(0), 972)
    RD_IOVIRT_SOC_EXP_COM_INIT(5, 1, UART_START(1), 982)
#endif

#if (FixedPcdGet32 (PcdChipCount) > 2)
    RD_IOVIRT_SOC_EXP_COM_INIT(6, 2, UART_START(0), 4556)
    RD_IOVIRT_SOC_EXP_COM_INIT(7, 2, UART_START(1), 4566)
#endif

#if (FixedPcdGet32 (PcdChipCount) > 3)
    RD_IOVIRT_SOC_EXP_COM_INIT(8, 3, UART_START(0), 5036)
    RD_IOVIRT_SOC_EXP_COM_INIT(9, 3, UART_START(1), 5046)
#endif
#endif

    // IO Virtualization SoC Expansion - PL330 DMA
    RD_IOVIRT_SOC_EXP_DMA_INIT(0, 0, DMA_START(0))
    RD_IOVIRT_SOC_EXP_DMA_INIT(1, 0, DMA_START(1))

#if (FixedPcdGet32 (PcdChipCount) > 1)
    RD_IOVIRT_SOC_EXP_DMA_INIT(2, 1, DMA_START(0))
    RD_IOVIRT_SOC_EXP_DMA_INIT(3, 1, DMA_START(1))
#endif

#if (FixedPcdGet32 (PcdChipCount) > 2)
    RD_IOVIRT_SOC_EXP_DMA_INIT(4, 2, DMA_START(0))
    RD_IOVIRT_SOC_EXP_DMA_INIT(5, 2, DMA_START(1))
#endif

#if (FixedPcdGet32 (PcdChipCount) > 3)
    RD_IOVIRT_SOC_EXP_DMA_INIT(6, 3, DMA_START(0))
    RD_IOVIRT_SOC_EXP_DMA_INIT(7, 3, DMA_START(1))
#endif
  } // Scope(_SB)
}
