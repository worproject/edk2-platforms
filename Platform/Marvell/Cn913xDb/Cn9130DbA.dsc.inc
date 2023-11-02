## @file
#  Component description file for the CN9130 Development Board (variant A)
#
#  Copyright (c) 2019 Marvell International Ltd.<BR>
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
  gMarvellSiliconTokenSpaceGuid.PcdChip1MppSel1|{ 0x3, 0x3, 0x0, 0x3, 0x3, 0x3, 0x3, 0x1, 0x1, 0x1 }
  gMarvellSiliconTokenSpaceGuid.PcdChip1MppSel2|{ 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x3, 0xA }
  gMarvellSiliconTokenSpaceGuid.PcdChip1MppSel3|{ 0xA, 0x3, 0x7, 0x6, 0x7, 0x2, 0x2, 0x2, 0x2, 0x1 }
  gMarvellSiliconTokenSpaceGuid.PcdChip1MppSel4|{ 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1 }
  gMarvellSiliconTokenSpaceGuid.PcdChip1MppSel5|{ 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0xE, 0xE, 0xE, 0xE }
  gMarvellSiliconTokenSpaceGuid.PcdChip1MppSel6|{ 0xE, 0xE, 0xE, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 }

  # I2C
  gMarvellSiliconTokenSpaceGuid.PcdI2cSlaveAddresses|{ 0x21 }
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

  # ComPhy
  gMarvellSiliconTokenSpaceGuid.PcdComPhyDevices|{ 0x1 }
  # ComPhy0
  # 0: PCIE0         5 Gbps
  # 1: PCIE0         5 Gbps
  # 2: PCIE0         5 Gbps
  # 3: PCIE0         5 Gbps
  # 4: SFI           10.31 Gbps
  # 5: SATA1         5 Gbps
  gMarvellSiliconTokenSpaceGuid.PcdChip0ComPhyTypes|{ $(CP_PCIE0), $(CP_PCIE0), $(CP_PCIE0), $(CP_PCIE0), $(CP_SFI), $(CP_SATA1)}
  gMarvellSiliconTokenSpaceGuid.PcdChip0ComPhySpeeds|{ $(CP_5G), $(CP_5G), $(CP_5G), $(CP_5G), $(CP_10_3125G), $(CP_5G) }

  # UtmiPhy
  gMarvellSiliconTokenSpaceGuid.PcdUtmiControllersEnabled|{ 0x1, 0x1 }
  gMarvellSiliconTokenSpaceGuid.PcdUtmiPortType|{ $(UTMI_USB_HOST0), $(UTMI_USB_HOST1) }

  # MDIO
  gMarvellSiliconTokenSpaceGuid.PcdMdioControllersEnabled|{ 0x1, 0x0 }

  # PHY
  gMarvellSiliconTokenSpaceGuid.PcdPhy2MdioController|{ 0x0, 0x0 }
  gMarvellSiliconTokenSpaceGuid.PcdPhyDeviceIds|{ 0x0, 0x0 }
  gMarvellSiliconTokenSpaceGuid.PcdPhySmiAddresses|{ 0x0, 0x1 }
  gMarvellSiliconTokenSpaceGuid.PcdPhyStartupAutoneg|FALSE

  # NET
  gMarvellSiliconTokenSpaceGuid.PcdPp2GopIndexes|{ 0x0, 0x2, 0x3 }
  gMarvellSiliconTokenSpaceGuid.PcdPp2InterfaceAlwaysUp|{ 0x0, 0x0, 0x0 }
  gMarvellSiliconTokenSpaceGuid.PcdPp2InterfaceSpeed|{ $(PHY_SPEED_10000), $(PHY_SPEED_1000), $(PHY_SPEED_1000) }
  gMarvellSiliconTokenSpaceGuid.PcdPp2PhyConnectionTypes|{ $(PHY_SFI), $(PHY_RGMII), $(PHY_RGMII) }
  gMarvellSiliconTokenSpaceGuid.PcdPp2PhyIndexes|{ 0xFF, 0x0, 0x1 }
  gMarvellSiliconTokenSpaceGuid.PcdPp2Port2Controller|{ 0x0, 0x0, 0x0 }
  gMarvellSiliconTokenSpaceGuid.PcdPp2PortIds|{ 0x0, 0x1, 0x2 }
  gMarvellSiliconTokenSpaceGuid.PcdPp2Controllers|{ 0x1 }

  # NonDiscoverableDevices
  gMarvellSiliconTokenSpaceGuid.PcdPciEXhci|{ 0x1, 0x1 }
  gMarvellSiliconTokenSpaceGuid.PcdPciEAhci|{ 0x1 }
  gMarvellSiliconTokenSpaceGuid.PcdPciESdhci|{ 0x1, 0x1 }

  # RTC
  gMarvellSiliconTokenSpaceGuid.PcdRtcBaseAddress|0xF2284000

  # Variable store
  gMarvellSiliconTokenSpaceGuid.PcdSpiMemoryMapped|FALSE
