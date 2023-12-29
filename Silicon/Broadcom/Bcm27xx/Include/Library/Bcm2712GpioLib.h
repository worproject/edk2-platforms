/** @file
 *
 *  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/


#ifndef __BCM2712_GPIO_LIB_H__
#define __BCM2712_GPIO_LIB_H__

typedef enum {
  BCM2712_GIO = 0,
  BCM2712_GIO_AON,
  BCM2712_GIO_COUNT
} BCM2712_GPIO_TYPE;

typedef enum {
  BCM2712_GPIO_ALT_IO = 0,
  BCM2712_GPIO_ALT_1,
  BCM2712_GPIO_ALT_2,
  BCM2712_GPIO_ALT_3,
  BCM2712_GPIO_ALT_4,
  BCM2712_GPIO_ALT_5,
  BCM2712_GPIO_ALT_6,
  BCM2712_GPIO_ALT_7,
  BCM2712_GPIO_ALT_8,
  BCM2712_GPIO_ALT_COUNT
} BCM2712_GPIO_ALT;

typedef enum {
  BCM2712_GPIO_PIN_PULL_NONE  = 0,
  BCM2712_GPIO_PIN_PULL_DOWN  = 1,
  BCM2712_GPIO_PIN_PULL_UP    = 2
} BCM2712_GPIO_PIN_PULL;

typedef enum {
  BCM2712_GPIO_PIN_OUTPUT = 0,
  BCM2712_GPIO_PIN_INPUT  = 1
} BCM2712_GPIO_PIN_DIRECTION;

UINT8
EFIAPI
GpioGetFunction (
  IN  BCM2712_GPIO_TYPE               Type,
  IN  UINT8                           Pin
  );

VOID
EFIAPI
GpioSetFunction (
  IN  BCM2712_GPIO_TYPE               Type,
  IN  UINT8                           Pin,
  IN  UINT8                           Function
  );

BCM2712_GPIO_PIN_PULL
EFIAPI
GpioGetPull (
  IN  BCM2712_GPIO_TYPE               Type,
  IN  UINT8                           Pin
  );

VOID
EFIAPI
GpioSetPull (
  IN  BCM2712_GPIO_TYPE               Type,
  IN  UINT8                           Pin,
  IN  BCM2712_GPIO_PIN_PULL           Pull
  );

BOOLEAN
EFIAPI
GpioRead (
  IN  BCM2712_GPIO_TYPE               Type,
  IN  UINT8                           Pin
  );

VOID
EFIAPI
GpioWrite (
  IN  BCM2712_GPIO_TYPE               Type,
  IN  UINT8                           Pin,
  IN  BOOLEAN                         Value
  );

BCM2712_GPIO_PIN_DIRECTION
EFIAPI
GpioGetDirection (
  IN  BCM2712_GPIO_TYPE               Type,
  IN  UINT8                           Pin
  );

VOID
EFIAPI
GpioSetDirection (
  IN  BCM2712_GPIO_TYPE               Type,
  IN  UINT8                           Pin,
  IN  BCM2712_GPIO_PIN_DIRECTION      Direction
  );

#endif // __BCM2712_GPIO_LIB_H__
