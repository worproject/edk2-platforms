## @file
#  Component description file for the AlderLake SiPkg both Pei and Dxe libraries DSC file.
#
#   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
#   SPDX-License-Identifier: BSD-2-Clause-Patent
##
#
#
# FRUs
#
!include $(PLATFORM_SI_PACKAGE)/Fru/AdlCpu/CommonLib.dsc
!include $(PLATFORM_SI_PACKAGE)/Fru/AdlPch/CommonLib.dsc
#
# Common
#
 PciSegmentLib|$(PLATFORM_SI_PACKAGE)/Library/BasePciSegmentMultiSegLibPci/BasePciSegmentMultiSegLibPci.inf

#
# Cpu
#
 CpuPlatformLib|$(PLATFORM_SI_PACKAGE)/Cpu/Library/PeiDxeSmmCpuPlatformLib/PeiDxeSmmCpuPlatformLib.inf

#
# Pch
#
 PchCycleDecodingLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/PeiDxeSmmPchCycleDecodingLib/PeiDxeSmmPchCycleDecodingLib.inf

 ResetSystemLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/BaseResetSystemLib/BaseResetSystemLib.inf
#private
 GpioPrivateLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Gpio/LibraryPrivate/PeiDxeSmmGpioPrivateLib/PeiDxeSmmGpioPrivateLibVer2.inf
 PchPciBdfLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/BasePchPciBdfLib/BasePchPciBdfLib.inf

#
# SA
#
 GraphicsInfoLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Graphics/Library/PeiDxeSmmGraphicsInfoLib/GraphicsInfoLibVer1.inf
