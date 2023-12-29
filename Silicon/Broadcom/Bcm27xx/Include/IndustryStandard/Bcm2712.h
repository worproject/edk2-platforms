/** @file
 *
 *  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#ifndef __BCM2712_H__
#define __BCM2712_H__

#define BCM2712_BRCMSTB_GIO_BASE                          0x107d508500
#define BCM2712_BRCMSTB_GIO_LENGTH                        0x40
#define BCM2712_BRCMSTB_GIO_AON_BASE                      0x107d517c00
#define BCM2712_BRCMSTB_GIO_AON_LENGTH                    0x40

#define BCM2712_PINCTRL_BASE                              0x107d504100
#define BCM2712_PINCTRL_LENGTH                            0x30
#define BCM2712_PINCTRL_AON_BASE                          0x107d510700
#define BCM2712_PINCTRL_AON_LENGTH                        0x20

#define BCM2712_BRCMSTB_SDIO1_HOST_BASE                   0x1000fff000
#define BCM2712_BRCMSTB_SDIO1_CFG_BASE                    0x1000fff400
#define BCM2712_BRCMSTB_SDIO2_HOST_BASE                   0x1001100000
#define BCM2712_BRCMSTB_SDIO2_CFG_BASE                    0x1001100400
#define BCM2712_BRCMSTB_SDIO_HOST_LENGTH                  0x260
#define BCM2712_BRCMSTB_SDIO_CFG_LENGTH                   0x200

#endif // __BCM2712_H__
