## @file
#  Component description file for the AlderLake SiPkg DXE drivers.
#
#   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
#   SPDX-License-Identifier: BSD-2-Clause-Patent
##

#
# FRUs
#
!include $(PLATFORM_SI_PACKAGE)/Fru/AdlPch/Dxe.dsc
#
# Common
#

#
# Pch
#
  $(PLATFORM_SI_PACKAGE)/Pch/SmmControl/RuntimeDxe/SmmControl.inf

  $(PLATFORM_SI_PACKAGE)/Pch/PchSmiDispatcher/Smm/PchSmiDispatcher.inf{
    <LibraryClasses>
      SmiHandlerProfileLib|MdePkg/Library/SmiHandlerProfileLibNull/SmiHandlerProfileLibNull.inf
  }

#
# SystemAgent
#
  $(PLATFORM_SI_PACKAGE)/SystemAgent/SaInit/Dxe/SaInitDxe.inf



