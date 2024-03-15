/** @file
 *
 *  Copyright (c) 2024, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#ifndef __RP1_BUS_H__
#define __RP1_BUS_H__

#define RP1_BUS_PROTOCOL_GUID \
  { 0xf1417a30, 0x5418, 0x4cd5, { 0x8e, 0x65, 0xf9, 0x02, 0x51, 0x21, 0xb5, 0x7f } }

typedef struct _RP1_BUS_PROTOCOL RP1_BUS_PROTOCOL;

typedef
EFI_PHYSICAL_ADDRESS
(EFIAPI *RP1_BUS_GET_PERIPHERAL_BASE)(
  IN RP1_BUS_PROTOCOL                   *This
  );

struct _RP1_BUS_PROTOCOL {
  RP1_BUS_GET_PERIPHERAL_BASE    GetPeripheralBase;
};

extern EFI_GUID  gRp1BusProtocolGuid;

#endif // __RP1_BUS_H__
