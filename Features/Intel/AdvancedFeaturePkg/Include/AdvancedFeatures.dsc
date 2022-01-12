## @file
#  DSC file for advanced features.
#
#  This file is intended to be included into another package so advanced features
#  can be conditionally built by enabling the respective feature via its FeaturePCD.
#
# Copyright (c) 2019 - 2020, Intel Corporation. All rights reserved.<BR>
#
# SPDX-License-Identifier: BSD-2-Clause-Patent
#
##

#
# Debug Advanced Features
#
!if gAcpiDebugFeaturePkgTokenSpaceGuid.PcdAcpiDebugFeatureEnable == TRUE
  !include AcpiDebugFeaturePkg/Include/AcpiDebugFeature.dsc
!endif

!if gUsb3DebugFeaturePkgTokenSpaceGuid.PcdUsb3DebugFeatureEnable == TRUE
  !include Usb3DebugFeaturePkg/Include/Usb3DebugFeature.dsc
!endif

#
# Network Advanced Features
#
!if gNetworkFeaturePkgTokenSpaceGuid.PcdNetworkFeatureEnable == TRUE
  !include NetworkFeaturePkg/Include/NetworkFeature.dsc
!endif

#
# Out-of-Band Management Advanced Features
#
!if gIpmiFeaturePkgTokenSpaceGuid.PcdIpmiFeatureEnable == TRUE
  !include IpmiFeaturePkg/Include/IpmiFeature.dsc
!endif

#
# Power Management Advanced Features
#
!if gS3FeaturePkgTokenSpaceGuid.PcdS3FeatureEnable == TRUE
  !include S3FeaturePkg/Include/S3Feature.dsc
!endif

#
# System Information Advanced Features
#
!if gSmbiosFeaturePkgTokenSpaceGuid.PcdSmbiosFeatureEnable == TRUE
  !include SmbiosFeaturePkg/Include/SmbiosFeature.dsc
!endif

#
# User Interface Advanced Features
#
!if gUserAuthFeaturePkgTokenSpaceGuid.PcdUserAuthenticationFeatureEnable == TRUE
  !include UserAuthFeaturePkg/Include/UserAuthFeature.dsc
!endif

!if gLogoFeaturePkgTokenSpaceGuid.PcdLogoFeatureEnable == TRUE
  !include LogoFeaturePkg/Include/LogoFeature.dsc
!endif
