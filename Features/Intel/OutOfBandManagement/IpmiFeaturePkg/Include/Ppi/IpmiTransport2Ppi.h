/** @file
  IPMI Ttransport2 PPI Header File.

  @copyright
  Copyright 2016 - 2021 Intel Corporation. <BR>
  Copyright (c) 1985 - 2023, American Megatrends International LLC. <BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _IPMI_TRANSPORT2_PPI_H_
#define _IPMI_TRANSPORT2_PPI_H_

#include <IpmiTransport2Definitions.h>
#include <Library/BmcCommonInterfaceLib.h>

#define PEI_IPMI_TRANSPORT2_PPI_GUID \
  { \
    0x8122CEBD, 0xF4FD, 0x4EA8, 0x97, 0x6C, 0xF0, 0x30, 0xAD, 0xDC, 0x4C, 0xB4 \
  }

extern EFI_GUID  gPeiIpmiTransport2PpiGuid;

#endif
