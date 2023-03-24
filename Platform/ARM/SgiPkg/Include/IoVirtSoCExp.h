/** @file
*
*  Copyright (c) 2023, Arm Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include "SgiPlatform.h"

#define IO_VIRT_BLK_BASE     FixedPcdGet64 (PcdIoVirtSocExpBlk0Base)
#define DEV_OFFSET           0x10000000
#define RESOURCE_SIZE        0x10000

/** Macros to calculate base addresses of UART and DMA devices within IO
    virtualization SoC expansion block address space.

  @param [in] n         Index of UART or DMA device within SoC expansion block.
                        Should be either 0 or 1.

  The base address offsets of UART and DMA devices within a SoC expansion block
  are shown below. The UARTs are at offset (2 * index * offset), while the DMAs
  are at offsets ((2 * index + 1) * offset).
  +----------------------------------------------+
  | Port # |  Peripheral   | Base address offset |
  |--------|---------------|---------------------|
  |  x4_0  | PL011_UART0   |     0x0000_0000     |
  |--------|---------------|---------------------|
  |  x4_1  | PL011_DMA0_NS |     0x1000_0000     |
  |--------|---------------|---------------------|
  |   x8   | PL011_UART1   |     0x2000_0000     |
  |--------|---------------|---------------------|
  |   x16  | PL011_DMA1_NS |     0x3000_0000     |
  +----------------------------------------------+
**/
#define UART_START(n)        IO_VIRT_BLK_BASE + (2 * n * DEV_OFFSET)
#define DMA_START(n)         IO_VIRT_BLK_BASE + (((2 * n) + 1) * DEV_OFFSET)

// Interrupt numbers of PL330 DMA-0 and DMA-1 devices in the SoC expansion
// connected to the IO Virtualization block. Each DMA PL330 controller uses
// eight data channel interrupts and one instruction channel interrupt to
// notify aborts.
#define RD_IOVIRT_SOC_EXP_DMA0_INTERRUPTS_INIT                                 \
  Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) {                 \
    493, 494, 495, 496, 497, 498, 499, 500, 501                                \
  }
#define RD_IOVIRT_SOC_EXP_DMA1_INTERRUPTS_INIT                                 \
  Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) {                 \
    503, 504, 505, 506, 507, 508, 509, 510, 511                                \
  }

#define RD_IOVIRT_SOC_EXP_DMA2_INTERRUPTS_INIT                                 \
  Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) {                 \
    973, 974, 975, 976, 977, 978, 979, 980, 981                                \
  }

#define RD_IOVIRT_SOC_EXP_DMA3_INTERRUPTS_INIT                                 \
  Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) {                 \
    983, 984, 985, 986, 987, 988, 989, 990, 991                                \
  }

#define RD_IOVIRT_SOC_EXP_DMA4_INTERRUPTS_INIT                                 \
  Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) {                 \
    4557, 4558, 4559, 4560, 4561, 4562, 4563, 4564, 4565                       \
  }

#define RD_IOVIRT_SOC_EXP_DMA5_INTERRUPTS_INIT                                 \
  Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) {                 \
    4567, 4568, 4569, 4570, 4571, 4572, 4573, 4574, 4575                       \
  }

#define RD_IOVIRT_SOC_EXP_DMA6_INTERRUPTS_INIT                                 \
  Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) {                 \
    5037, 5038, 5039, 5040, 5041, 5042, 5043, 5044, 5045                       \
  }

#define RD_IOVIRT_SOC_EXP_DMA7_INTERRUPTS_INIT                                 \
  Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) {                 \
    5047, 5048, 5049, 5050, 5051, 5052, 5053, 5054, 5055                       \
  }

/** Macro for PL011 UART controller node instantiation in SSDT table.

  See section 5.2.11.2 of ACPI specification v6.4 for the definition of SSDT
  table.

  @param [in] ComIdx          Index of Com device to be initializaed;
                              to be passed as 2-digit index, such as 01 to
                              support multichip platforms as well.
  @param [in] ChipIdx         Index of chip to which this DMA device belongs
  @param [in] StartOff        Starting offset of this device within IO
                              virtualization block memory map
  @param [in] IrqNum          Interrupt ID used for the device
**/
#define RD_IOVIRT_SOC_EXP_COM_INIT(ComIdx, ChipIdx, StartOff, IrqNum)          \
  Device (COM ##ComIdx) {                                                      \
    Name (_HID, "ARMH0011")                                                    \
    Name (_UID, ComIdx)                                                        \
    Name (_STA, 0xF)                                                           \
                                                                               \
    Method (_CRS, 0, Serialized) {                                             \
      Name (RBUF, ResourceTemplate () {                                        \
        QWordMemory (                                                          \
          ResourceProducer,                                                    \
          PosDecode,                                                           \
          MinFixed,                                                            \
          MaxFixed,                                                            \
          NonCacheable,                                                        \
          ReadWrite,                                                           \
          0x0,                                                                 \
          0,                                                                   \
          1,                                                                   \
          0x0,                                                                 \
          2,                                                                   \
          ,                                                                    \
          ,                                                                    \
          MMI1,                                                                \
          AddressRangeMemory,                                                  \
          TypeStatic                                                           \
        )                                                                      \
                                                                               \
        Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) {           \
          IrqNum                                                               \
        }                                                                      \
      }) /* end Name(RBUF) */                                                  \
      /* Work around ASL's inability to add in a resource definition */        \
      CreateQwordField (RBUF, MMI1._MIN, MIN1)                                 \
      CreateQwordField (RBUF, MMI1._MAX, MAX1)                                 \
      CreateQwordField (RBUF, MMI1._LEN, LEN1)                                 \
      Add (SGI_REMOTE_CHIP_MEM_OFFSET(ChipIdx), StartOff, MIN1)                \
      Add (MIN1, RESOURCE_SIZE - 1, MAX1)                                      \
      Add (RESOURCE_SIZE, 0, LEN1)                                             \
                                                                               \
      Return (RBUF)                                                            \
    } /* end Method(_CRS) */                                                   \
  }

/** Macro for PL330 DMA controller node instantiation in SSDT table.

  See section 5.2.11.2 of ACPI specification v6.4 for the definition of SSDT
  table.

  @param [in] DmaIdx          Index of DMA device to be initializaed
  @param [in] ChipIdx         Index of chip to which this DMA device belongs
  @param [in] StartOff        Starting offset of this device within IO
                              virtualization block memory map
**/
#define RD_IOVIRT_SOC_EXP_DMA_INIT(DmaIdx, ChipIdx, StartOff)                  \
  Device (\_SB.DMA ##DmaIdx) {                                                 \
    Name (_HID, "ARMH0330")                                                    \
    Name (_UID, DmaIdx)                                                        \
    Name (_CCA, 1)                                                             \
    Name (_STA, 0xF)                                                           \
                                                                               \
    Method (_CRS, 0, Serialized) {                                             \
      Name (RBUF, ResourceTemplate () {                                        \
        QWordMemory (                                                          \
          ResourceProducer,                                                    \
          PosDecode,                                                           \
          MinFixed,                                                            \
          MaxFixed,                                                            \
          NonCacheable,                                                        \
          ReadWrite,                                                           \
          0x0,                                                                 \
          0,                                                                   \
          1,                                                                   \
          0x0,                                                                 \
          2,                                                                   \
          ,                                                                    \
          ,                                                                    \
          MMI2,                                                                \
          AddressRangeMemory,                                                  \
          TypeStatic                                                           \
        )                                                                      \
                                                                               \
        RD_IOVIRT_SOC_EXP_DMA ##DmaIdx## _INTERRUPTS_INIT                      \
      }) /* end Name(RBUF) */                                                  \
      /* Work around ASL's inability to add in a resource definition */        \
      CreateQwordField (RBUF, MMI2._MIN, MIN2)                                 \
      CreateQwordField (RBUF, MMI2._MAX, MAX2)                                 \
      CreateQwordField (RBUF, MMI2._LEN, LEN2)                                 \
      Add (SGI_REMOTE_CHIP_MEM_OFFSET(ChipIdx), StartOff, MIN2)                \
      Add (MIN2, RESOURCE_SIZE - 1, MAX2)                                      \
      Add (RESOURCE_SIZE, 0, LEN2)                                             \
                                                                               \
      Return (RBUF)                                                            \
    } /* end Method(_CRS) */                                                   \
  }
