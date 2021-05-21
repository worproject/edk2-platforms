/** @file

  Copyright (c) 2020 - 2021, Ampere Computing LLC. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef PLATFORM_AC01_H_
#define PLATFORM_AC01_H_

//
// Number of supported sockets in the platform
//
#define PLATFORM_CPU_MAX_SOCKET           2

//
// Maximum number of CPMs in the chip.
//
#define PLATFORM_CPU_MAX_CPM              (FixedPcdGet32 (PcdClusterCount))

//
// Number of cores per CPM.
//
#define PLATFORM_CPU_NUM_CORES_PER_CPM    (FixedPcdGet32 (PcdCoreCount) / PLATFORM_CPU_MAX_CPM)

//
// Maximum number of cores supported.
//
#define PLATFORM_CPU_MAX_NUM_CORES        (PLATFORM_CPU_MAX_SOCKET * PLATFORM_CPU_MAX_CPM * PLATFORM_CPU_NUM_CORES_PER_CPM)

//
// Maximum number of memory region
//
#define PLATFORM_DRAM_INFO_MAX_REGION     16

//
// Maximum number of DDR slots supported
//
#define PLATFORM_DIMM_INFO_MAX_SLOT       32

//
// CSR Address base for slave socket
//
#define SLAVE_SOCKET_BASE_ADDRESS_OFFSET  0x400000000000

//
// SMpro EFUSE Shadow register
//
#define SMPRO_EFUSE_SHADOW0               (FixedPcdGet64 (PcdSmproEfuseShadow0))

//
// 2P Configuration Register
//
#define CFG2P_OFFSET                      0x200

//
// Slave socket present
//
#define SLAVE_PRESENT_N                   BIT1

//
// The maximum number of I2C bus
//
#define AC01_I2C_MAX_BUS_NUM              2

//
// The base address of DW I2C
//
#define AC01_I2C_BASE_ADDRESS_LIST        0x1000026B0000ULL, 0x100002750000ULL

//
// The Array of Soc Gpio Base Address
//
#define AC01_GPIO_BASE_ADDRESS_LIST       0x1000026f0000, 0x1000026e0000, 0x1000027b0000, 0x1000026d0000, 0x5000026f0000, 0x5000026e0000, 0x5000027b0000, 0x5000026d0000

//
// The Array of Soc Gpi Base Address
//
#define AC01_GPI_BASE_ADDRESS_LIST        0x1000026d0000, 0x5000026d0000

//
// Number of Pins Per Each Contoller
//
#define AC01_GPIO_PINS_PER_CONTROLLER     8

//
// Number of Pins Each Socket
//
#define AC01_GPIO_PINS_PER_SOCKET         32

#endif /* PLATFORM_AC01_H_ */
