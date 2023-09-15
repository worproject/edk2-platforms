## @file
#  Component description file for the AlderLake SiPkg PEI libraries.
#
#   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
#   SPDX-License-Identifier: BSD-2-Clause-Patent
##

#
# FRUs
#
!include $(PLATFORM_SI_PACKAGE)/Fru/AdlPch/PeiLib.dsc


 SiPolicyLib|$(PLATFORM_SI_PACKAGE)/Product/Alderlake/Library/PeiSiPolicyLib/PeiSiPolicyLib.inf
 SiConfigBlockLib|$(PLATFORM_SI_PACKAGE)/Library/BaseSiConfigBlockLib/BaseSiConfigBlockLib.inf




