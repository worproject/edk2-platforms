## @file PlatformPkg.dsc
# BoardPortTemplate reference platform single board build target.
#
#
# @copyright
# Copyright 2008 - 2020 Intel Corporation. <BR>
#
# SPDX-License-Identifier: BSD-2-Clause-Patent
##

[Defines]
  PLATFORM_NAME                       = WhitleyOpenBoardPkg
  PEI_ARCH                            = IA32
  DXE_ARCH                            = X64

!include $(RP_PKG)/PlatformPkg.dsc

[Defines]
  DEFINE BOARD_NAME                   = BoardPortTemplate
  PLATFORM_GUID                       = f3518dd4-5dec-4d2c-9ac9-561121e2628b
  OUTPUT_DIRECTORY                    = Build/$(RP_PKG)
  SUPPORTED_ARCHITECTURES             = IA32|X64
  BUILD_TARGETS                       = DEBUG|RELEASE
  FLASH_DEFINITION                    = $(RP_PKG)/$(BOARD_NAME)/PlatformPkg.fdf

#
# Advanced feature selection/enablement
#

[PcdsFixedAtBuild]
  gPlatformTokenSpaceGuid.PcdBoardId|0x26 # TypeBoardPortTemplate

#
# Increase debug message levels
# Several options are provided, last uncommented one will take effect
#
#!include $(RP_PKG)/Include/Dsc/EnableRichDebugMessages.dsc
#!include $(RP_PKG)/Include/Dsc/EnableAllDebugMessages.dsc

!include $(RP_PKG)/$(BOARD_NAME)/Include/Dsc/UbaSingleBoardPei.dsc
