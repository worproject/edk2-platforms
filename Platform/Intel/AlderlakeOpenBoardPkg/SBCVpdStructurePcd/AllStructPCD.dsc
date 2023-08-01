## @file
#  Include All Board Gpio configuration file.
#
#   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
#   SPDX-License-Identifier: BSD-2-Clause-Patent
#
##
!include $(PLATFORM_BOARD_PACKAGE)/SBCVpdStructurePcd/GpioTableAdlPPostMem.dsc
!include $(PLATFORM_BOARD_PACKAGE)/SBCVpdStructurePcd/GpioTableAdlPPreMem.dsc


# PCIe clock mapping
!include $(PLATFORM_BOARD_PACKAGE)/SBCVpdStructurePcd/PcieClocks/AdlPPcieClocks.dsc

# MRC DQS DQ and SPD mapping
!include $(PLATFORM_BOARD_PACKAGE)/SBCVpdStructurePcd/MrcDqDqsSPD/AdlPSpdMap.dsc



