/** @file
  Interrupt policy

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _INTERRUPT_CONFIG_H_
#define _INTERRUPT_CONFIG_H_

extern EFI_GUID gInterruptConfigGuid;

#pragma pack (push,1)

//
// --------------------- Interrupts Config ------------------------------
//
typedef enum {
  PchNoInt,        ///< No Interrupt Pin
  PchIntA,
  PchIntB,
  PchIntC,
  PchIntD
} PCH_INT_PIN;

///
/// The PCH_DEVICE_INTERRUPT_CONFIG block describes interrupt pin, IRQ and interrupt mode for PCH device.
///
typedef struct {
  UINT8        Device;                  ///< Device number
  UINT8        Function;                ///< Device function
  UINT8        IntX;                    ///< Interrupt pin: INTA-INTD (see PCH_INT_PIN)
  UINT8        Irq;                     ///< IRQ to be set for device.
} PCH_DEVICE_INTERRUPT_CONFIG;


#pragma pack (pop)

#endif // _INTERRUPT_CONFIG_H_
