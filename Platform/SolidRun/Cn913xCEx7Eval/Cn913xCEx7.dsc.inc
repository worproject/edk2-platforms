## @file
#  Component description file for the CN9130 Development Board (variant A)
#
#  Copyright (c) 2019 Marvell International Ltd.<BR>
#  Copyright (c) 2021 Semihalf.<BR>
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
#
##

################################################################################
#
# Pcd Section - list of all EDK II PCD Entries defined by this Platform
#
################################################################################
[PcdsFixedAtBuild.common]
  # CP115 count
  gMarvellSiliconTokenSpaceGuid.PcdMaxCpCount|1

  # MPP
  gMarvellSiliconTokenSpaceGuid.PcdMppChipCount|2

  # APN807 MPP
  gMarvellSiliconTokenSpaceGuid.PcdChip0MppReverseFlag|FALSE
  gMarvellSiliconTokenSpaceGuid.PcdChip0MppBaseAddress|0xF06F4000
  gMarvellSiliconTokenSpaceGuid.PcdChip0MppPinCount|20
  gMarvellSiliconTokenSpaceGuid.PcdChip0MppSel0|{ 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1 }
  gMarvellSiliconTokenSpaceGuid.PcdChip0MppSel1|{ 0x1, 0x3, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3 }

  # CP115 #0 MPP
  gMarvellSiliconTokenSpaceGuid.PcdChip1MppReverseFlag|FALSE
  gMarvellSiliconTokenSpaceGuid.PcdChip1MppBaseAddress|0xF2440000
  gMarvellSiliconTokenSpaceGuid.PcdChip1MppPinCount|64
  gMarvellSiliconTokenSpaceGuid.PcdChip1MppSel0|{ 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3 }
  gMarvellSiliconTokenSpaceGuid.PcdChip1MppSel1|{ 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x0, 0x0, 0x0 }
  gMarvellSiliconTokenSpaceGuid.PcdChip1MppSel2|{ 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x7, 0x7 }
  gMarvellSiliconTokenSpaceGuid.PcdChip1MppSel3|{ 0x7, 0x0, 0x0, 0x0, 0x0, 0x2, 0x2, 0x2, 0x2, 0x0 }
  gMarvellSiliconTokenSpaceGuid.PcdChip1MppSel4|{ 0x8, 0x8, 0x8, 0x8, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 }
  gMarvellSiliconTokenSpaceGuid.PcdChip1MppSel5|{ 0x6, 0x6, 0x2, 0x0, 0x2, 0xB, 0xE, 0xE, 0xE, 0xE }
  gMarvellSiliconTokenSpaceGuid.PcdChip1MppSel6|{ 0xE, 0xE, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 }

  # I2C
  gMarvellSiliconTokenSpaceGuid.PcdI2cSlaveAddresses|{ 0x50 }
  gMarvellSiliconTokenSpaceGuid.PcdI2cSlaveBuses|{ 0x0 }
  gMarvellSiliconTokenSpaceGuid.PcdI2cControllersEnabled|{ 0x0, 0x1 }
  gMarvellSiliconTokenSpaceGuid.PcdI2cClockFrequency|250000000
  gMarvellSiliconTokenSpaceGuid.PcdI2cBaudRate|100000

  # SPI
  gMarvellSiliconTokenSpaceGuid.PcdSpiRegBase|0xF2700680
  gMarvellSiliconTokenSpaceGuid.PcdSpiMaxFrequency|10000000
  gMarvellSiliconTokenSpaceGuid.PcdSpiClockFrequency|200000000

  gMarvellSiliconTokenSpaceGuid.PcdSpiFlashMode|3
  gMarvellSiliconTokenSpaceGuid.PcdSpiFlashCs|0

  # NonDiscoverableDevices
  gMarvellSiliconTokenSpaceGuid.PcdPciESdhci|{ 0x1 }

  # RTC
  gMarvellSiliconTokenSpaceGuid.PcdRtcBaseAddress|0xF2284000

  # Variable store
  gMarvellSiliconTokenSpaceGuid.PcdSpiMemoryBase|0xEF000000
[PcdsDynamicDefault.common]
  gEfiMdeModulePkgTokenSpaceGuid.PcdFlashNvStorageVariableBase64|0xEF3C0000
  gEfiMdeModulePkgTokenSpaceGuid.PcdFlashNvStorageFtwSpareBase64|0xEF3E0000
  gEfiMdeModulePkgTokenSpaceGuid.PcdFlashNvStorageFtwWorkingBase64|0xEF3D0000
