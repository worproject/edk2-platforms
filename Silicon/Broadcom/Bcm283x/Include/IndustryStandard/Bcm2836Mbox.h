/** @file
 *
 *  Copyright (c) 2020, Pete Batard <pete@akeo.ie>
 *  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#ifndef __BCM2836_MBOX_H__
#define __BCM2836_MBOX_H__

/* Mailbox registers */
#define BCM2836_MBOX_READ_OFFSET                            0x00000000
#define BCM2836_MBOX_STATUS_OFFSET                          0x00000018
#define BCM2836_MBOX_CONFIG_OFFSET                          0x0000001c
#define BCM2836_MBOX_WRITE_OFFSET                           0x00000020

#define BCM2836_MBOX_STATUS_FULL                            0x1f
#define BCM2836_MBOX_STATUS_EMPTY                           0x1e

#define BCM2836_MBOX_NUM_CHANNELS                           16

#endif /* __BCM2836_MBOX_H__ */
