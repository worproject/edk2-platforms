/** @file IpmiTransport2Protocol.h
  IpmiTransport2 Protocol Header File.

  @copyright
  Copyright 2016 - 2021 Intel Corporation. <BR>
  Copyright (c) 1985 - 2023, American Megatrends International LLC. <BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _IPMI2_TRANSPORT2_PROTO_H_
#define _IPMI2_TRANSPORT2_PROTO_H_

#include <IpmiTransport2Definitions.h>
#include <Library/BmcCommonInterfaceLib.h>

#define IPMI_TRANSPORT2_PROTOCOL_GUID \
  { \
    0x4a1d0e66, 0x5271, 0x4e22, { 0x83, 0xfe, 0x90, 0x92, 0x1b, 0x74, 0x82, 0x13 } \
  }

#define SMM_IPMI_TRANSPORT2_PROTOCOL_GUID \
  { \
    0x1dbd1503, 0xa60, 0x4230, { 0xaa, 0xa3, 0x80, 0x16, 0xd8, 0xc3, 0xde, 0x2f } \
  }

extern EFI_GUID  gIpmiTransport2ProtocolGuid;
extern EFI_GUID  gSmmIpmiTransport2ProtocolGuid;

#endif
