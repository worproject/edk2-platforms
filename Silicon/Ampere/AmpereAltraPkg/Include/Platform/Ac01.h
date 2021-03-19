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

#endif /* PLATFORM_AC01_H_ */
