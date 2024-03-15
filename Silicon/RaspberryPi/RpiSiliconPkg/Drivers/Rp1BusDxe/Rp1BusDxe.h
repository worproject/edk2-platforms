/** @file
 *
 *  Copyright (c) 2024, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#ifndef __RP1_BUS_DXE_H__
#define __RP1_BUS_DXE_H__

#include <Protocol/DriverBinding.h>
#include <Protocol/PciIo.h>
#include <Protocol/Rp1Bus.h>

#define RP1_BUS_DATA_SIGNATURE  SIGNATURE_32 ('R','P','1','b')

typedef struct {
  UINT32                         Signature;
  EFI_HANDLE                     ControllerHandle;
  EFI_DRIVER_BINDING_PROTOCOL    *DriverBinding;
  EFI_PCI_IO_PROTOCOL            *PciIo;
  RP1_BUS_PROTOCOL               Rp1Bus;
  EFI_PHYSICAL_ADDRESS           PeripheralBase;
  UINT32                         ChipId;
} RP1_BUS_DATA;

#define RP1_BUS_DATA_FROM_THIS(a)  CR (a, RP1_BUS_DATA, Rp1Bus, RP1_BUS_DATA_SIGNATURE)

extern EFI_DRIVER_BINDING_PROTOCOL   mRp1BusDriverBinding;
extern EFI_COMPONENT_NAME_PROTOCOL   mRp1BusComponentName;
extern EFI_COMPONENT_NAME2_PROTOCOL  mRp1BusComponentName2;

#endif // __RP1_BUS_DXE_H__
