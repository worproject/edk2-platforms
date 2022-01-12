## @file
# This is a build description file for the Intelligent Platform Management Interface (IPMI) advanced feature.
# This file should be included into another package DSC file to build this feature.
#
# The DEC files are used by the utilities that parse DSC and
# INF files to generate AutoGen.c and AutoGen.h files
# for the build infrastructure.
#
# Copyright (c) 2019 - 2021, Intel Corporation. All rights reserved.<BR>
#
# SPDX-License-Identifier: BSD-2-Clause-Patent
#
##

################################################################################
#
# Defines Section - statements that will be processed to create a Makefile.
#
################################################################################
[Defines]
!ifndef $(PEI_ARCH)
  !error "PEI_ARCH must be specified to build this feature!"
!endif
!ifndef $(DXE_ARCH)
  !error "DXE_ARCH must be specified to build this feature!"
!endif

################################################################################
#
# Library Class section - list of all Library Classes needed by this feature.
#
################################################################################

!include MdePkg/MdeLibs.dsc.inc

[LibraryClasses]
  IpmiLib|MdeModulePkg/Library/BaseIpmiLibNull/BaseIpmiLibNull.inf

  IpmiCommandLib|OutOfBandManagement/IpmiFeaturePkg/Library/IpmiCommandLib/IpmiCommandLib.inf
  IpmiPlatformHookLib|OutOfBandManagement/IpmiFeaturePkg/Library/IpmiPlatformHookLibNull/IpmiPlatformHookLibNull.inf

[LibraryClasses.common.PEI_CORE,LibraryClasses.common.PEIM]
  IpmiBaseLib|OutOfBandManagement/IpmiFeaturePkg/Library/PeiIpmiBaseLib/PeiIpmiBaseLib.inf

[LibraryClasses.common.DXE_DRIVER,LibraryClasses.common.UEFI_DRIVER]
  IpmiBaseLib|OutOfBandManagement/IpmiFeaturePkg/Library/IpmiBaseLib/IpmiBaseLib.inf

################################################################################
#
# Component section - list of all components that need built for this feature.
#
# Note: The EDK II DSC file is not used to specify how compiled binary images get placed
#       into firmware volume images. This section is just a list of modules to compile from
#       source into UEFI-compliant binaries.
#       It is the FDF file that contains information on combining binary files into firmware
#       volume images, whose concept is beyond UEFI and is described in PI specification.
#       There may also be modules listed in this section that are not required in the FDF file,
#       When a module listed here is excluded from FDF file, then UEFI-compliant binary will be
#       generated for it, but the binary will not be put into any firmware volume.
#
################################################################################
#
# Feature PEI Components
#

# @todo: Change below line to [Components.$(PEI_ARCH)] after https://bugzilla.tianocore.org/show_bug.cgi?id=2308
#        is completed.
[Components.IA32]
  #####################################
  # IPMI Feature Package
  #####################################

  # Add library instances here that are not included in package components and should be tested
  # in the package build.

  OutOfBandManagement/IpmiFeaturePkg/Library/IpmiPlatformHookLibNull/IpmiPlatformHookLibNull.inf

  #
  # Add components here that should be included in the package build.
  #
  OutOfBandManagement/IpmiFeaturePkg/GenericIpmi/Pei/PeiGenericIpmi.inf
  OutOfBandManagement/IpmiFeaturePkg/Frb/FrbPei.inf
  OutOfBandManagement/IpmiFeaturePkg/IpmiInit/PeiIpmiInit.inf

#
# Feature DXE Components
#

# @todo: Change below line to [Components.$(DXE_ARCH)] after https://bugzilla.tianocore.org/show_bug.cgi?id=2308
#        is completed.
[Components.X64]
  #####################################
  # IPMI Feature Package
  #####################################

  # Add library instances here that are not included in package components and should be tested
  # in the package build.

  OutOfBandManagement/IpmiFeaturePkg/Library/IpmiPlatformHookLibNull/IpmiPlatformHookLibNull.inf

  #
  # Add components here that should be included in the package build.
  #
  OutOfBandManagement/IpmiFeaturePkg/GenericIpmi/Dxe/GenericIpmi.inf
  OutOfBandManagement/IpmiFeaturePkg/Library/SmmIpmiBaseLib/SmmIpmiBaseLib.inf
  OutOfBandManagement/IpmiFeaturePkg/BmcAcpi/BmcAcpi.inf
  OutOfBandManagement/IpmiFeaturePkg/BmcElog/BmcElog.inf
  OutOfBandManagement/IpmiFeaturePkg/Frb/FrbDxe.inf
  OutOfBandManagement/IpmiFeaturePkg/IpmiFru/IpmiFru.inf
  OutOfBandManagement/IpmiFeaturePkg/IpmiInit/DxeIpmiInit.inf
  OutOfBandManagement/IpmiFeaturePkg/OsWdt/OsWdt.inf
  OutOfBandManagement/IpmiFeaturePkg/SolStatus/SolStatus.inf
