/** @file
 *
 *  Copyright (c) 2019, Pete Batard <pete@akeo.ie>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#ifndef BCM2838_RNG_H__
#define BCM2838_RNG_H__

#define RNG_CTRL                            0x0
#define RNG_STATUS                          0x4
#define RNG_DATA                            0x8
#define RNG_BIT_COUNT                       0xc
#define RNG_BIT_COUNT_THRESHOLD             0x10
#define RNG_INT_STATUS                      0x18
#define RNG_INT_ENABLE                      0x1c
#define RNG_FIFO_DATA                       0x20
#define RNG_FIFO_COUNT                      0x24

#define RNG_CTRL_ENABLE_MASK                0x1fff
#define RNG_CTRL_SAMPLE_RATE_DIVISOR_SHIFT  13      // Unmasked bits from above
#define RNG_FIFO_DATA_AVAIL_MASK            0xff

#endif /* BCM2838_RNG_H__ */
