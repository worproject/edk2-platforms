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
  # ComPhy
  gMarvellSiliconTokenSpaceGuid.PcdComPhyDevices|{ 0x1 }
  # ComPhy0
  # 0: PCIE0         5 Gbps
  # 1: PCIE0         5 Gbps
  # 2: PCIE0         5 Gbps
  # 3: PCIE0         5 Gbps
  # 4: SFI           10.31 Gbps
  # 5: SGMII2        3.125 Gbps
  gMarvellSiliconTokenSpaceGuid.PcdChip0ComPhyTypes|{ $(CP_PCIE0), $(CP_PCIE0), $(CP_PCIE0), $(CP_PCIE0), $(CP_SFI), $(CP_SGMII2)}
  gMarvellSiliconTokenSpaceGuid.PcdChip0ComPhySpeeds|{ $(CP_5G), $(CP_5G), $(CP_5G), $(CP_5G), $(CP_10_3125G), $(CP_3_125G) }

  # UtmiPhy
  gMarvellSiliconTokenSpaceGuid.PcdUtmiControllersEnabled|{ 0x1, 0x1 }
  gMarvellSiliconTokenSpaceGuid.PcdUtmiPortType|{ $(UTMI_USB_HOST0), $(UTMI_USB_HOST1) }

  # MDIO
  gMarvellSiliconTokenSpaceGuid.PcdMdioControllersEnabled|{ 0x1 }

  # PHY
  gMarvellSiliconTokenSpaceGuid.PcdPhy2MdioController|{ 0x0 }
  gMarvellSiliconTokenSpaceGuid.PcdPhyDeviceIds|{ 0x0 }
  gMarvellSiliconTokenSpaceGuid.PcdPhySmiAddresses|{ 0x0 }
  gMarvellSiliconTokenSpaceGuid.PcdPhyStartupAutoneg|FALSE

  # NET
  gMarvellSiliconTokenSpaceGuid.PcdPp2GopIndexes|{ 0x0, 0x2, 0x3 }
  gMarvellSiliconTokenSpaceGuid.PcdPp2InterfaceAlwaysUp|{ 0x0, 0x0, 0x1 }
  gMarvellSiliconTokenSpaceGuid.PcdPp2InterfaceSpeed|{ $(PHY_SPEED_10000), $(PHY_SPEED_1000), $(PHY_SPEED_2500) }
  gMarvellSiliconTokenSpaceGuid.PcdPp2PhyConnectionTypes|{ $(PHY_SFI), $(PHY_RGMII), $(PHY_SGMII) }
  gMarvellSiliconTokenSpaceGuid.PcdPp2PhyIndexes|{ 0xFF, 0x0, 0xFF }
  gMarvellSiliconTokenSpaceGuid.PcdPp2Port2Controller|{ 0x0, 0x0, 0x0 }
  gMarvellSiliconTokenSpaceGuid.PcdPp2PortIds|{ 0x0, 0x1, 0x2 }
  gMarvellSiliconTokenSpaceGuid.PcdPp2Controllers|{ 0x1 }

  # NonDiscoverableDevices
  gMarvellSiliconTokenSpaceGuid.PcdPciEXhci|{ 0x1, 0x1 }
  gMarvellSiliconTokenSpaceGuid.PcdPciESdhci|{ 0x1, 0x1 }
