## @file
# Manageabilty Package
# This is the package provides edk2 drivers and libraries
# those are related to the platform management.
#
# Copyright (C) 2023 Advanced Micro Devices, Inc. All rights reserved.<BR>
# SPDX-License-Identifier: BSD-2-Clause-Patent
#
##

[Defines]
  PLATFORM_NAME                  = ManageabilityPkg
  PLATFORM_GUID                  = 7A98123A-B194-40B6-A863-A52192F6D65D
  PLATFORM_VERSION               = 1.0
  DSC_SPECIFICATION              = 0x0001001e
  OUTPUT_DIRECTORY               = Build/ManageabilityPkg
  SUPPORTED_ARCHITECTURES        = IA32|X64|ARM|AARCH64|RISCV64
  BUILD_TARGETS                  = DEBUG|RELEASE|NOOPT
  SKUID_IDENTIFIER               = DEFAULT

[Packages]
  MinPlatformPkg/MinPlatformPkg.dec

[PcdsFeatureFlag]
  #
  # MinPlatform common include currently required PCD
  #
  gMinPlatformPkgTokenSpaceGuid.PcdUefiSecureBootEnable                   |FALSE
  gMinPlatformPkgTokenSpaceGuid.PcdPerformanceEnable                      |FALSE

  #
  # Manageability modules
  #
  gManageabilityPkgTokenSpaceGuid.PcdManageabilityDxeIpmiEnable              |TRUE
  gManageabilityPkgTokenSpaceGuid.PcdManageabilitySmmIpmiEnable              |TRUE
  gManageabilityPkgTokenSpaceGuid.PcdManageabilityPeiIpmiEnable              |TRUE
  gManageabilityPkgTokenSpaceGuid.PcdManageabilityDxePldmEnable              |TRUE
  gManageabilityPkgTokenSpaceGuid.PcdManageabilityDxeMctpEnable              |TRUE
  gManageabilityPkgTokenSpaceGuid.PcdManageabilityDxePldmSmbiosTransferEnable|TRUE
  gManageabilityPkgTokenSpaceGuid.PcdManageabilityDxeIpmiFru                 |TRUE
  gManageabilityPkgTokenSpaceGuid.PcdManageabilityDxeIpmiOsWdt               |TRUE
  gManageabilityPkgTokenSpaceGuid.PcdManageabilityDxeIpmiSolStatus           |TRUE
  gManageabilityPkgTokenSpaceGuid.PcdManageabilityDxeIpmiBmcElog             |TRUE
  gManageabilityPkgTokenSpaceGuid.PcdManageabilityDxeIpmiFrb                 |TRUE
  gManageabilityPkgTokenSpaceGuid.PcdManageabilityPeiIpmiFrb                 |TRUE
  gManageabilityPkgTokenSpaceGuid.PcdManageabilityDxeIpmiBmcAcpi             |TRUE

#
# Include common libraries
#
!include MinPlatformPkg/Include/Dsc/CoreCommonLib.dsc
!include MinPlatformPkg/Include/Dsc/CorePeiLib.dsc
!include MinPlatformPkg/Include/Dsc/CoreDxeLib.dsc

[Components]
  ManageabilityPkg/Library/ManageabilityTransportKcsLib/Dxe/DxeManageabilityTransportKcs.inf
  ManageabilityPkg/Library/ManageabilityTransportMctpLib/Dxe/DxeManageabilityTransportMctp.inf
  ManageabilityPkg/Library/PldmProtocolLibrary/Dxe/PldmProtocolLib.inf
  ManageabilityPkg/Library/IpmiCommandLib/IpmiCommandLib.inf

[LibraryClasses]
  ManageabilityTransportLib|ManageabilityPkg/Library/BaseManageabilityTransportNullLib/BaseManageabilityTransportNull.inf
  IpmiLib|MdeModulePkg/Library/BaseIpmiLibNull/BaseIpmiLibNull.inf

!include Include/Manageability.dsc
