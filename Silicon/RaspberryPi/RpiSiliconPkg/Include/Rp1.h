/** @file
 *
 *  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#ifndef __RP1_H__
#define __RP1_H__

//
// BAR1 Peripherals
//
#define RP1_SYSINFO_BASE                            0x00000000
#define RP1_SYSCFG_BASE                             0x00008000
#define RP1_OTP_BASE                                0x0000c000
#define RP1_POWER_BASE                              0x00010000
#define RP1_RESETS_BASE                             0x00014000
#define RP1_CLOCKS_MAIN_BASE                        0x00018000
#define RP1_CLOCKS_VIDEO_BASE                       0x0001c000
#define RP1_PLL_SYS_BASE                            0x00020000
#define RP1_PLL_AUDIO_BASE                          0x00024000
#define RP1_PLL_VIDEO_BASE                          0x00028000
#define RP1_UART0_BASE                              0x00030000
#define RP1_UART1_BASE                              0x00034000
#define RP1_UART2_BASE                              0x00038000
#define RP1_UART3_BASE                              0x0003c000
#define RP1_UART4_BASE                              0x00040000
#define RP1_UART5_BASE                              0x00044000
#define RP1_SPI8_BASE                               0x0004c000
#define RP1_SPI0_BASE                               0x00050000
#define RP1_SPI1_BASE                               0x00054000
#define RP1_SPI2_BASE                               0x00058000
#define RP1_SPI3_BASE                               0x0005c000
#define RP1_SPI4_BASE                               0x00060000
#define RP1_SPI5_BASE                               0x00064000
#define RP1_SPI6_BASE                               0x00068000
#define RP1_SPI7_BASE                               0x0006c000
#define RP1_I2C0_BASE                               0x00070000
#define RP1_I2C1_BASE                               0x00074000
#define RP1_I2C2_BASE                               0x00078000
#define RP1_I2C3_BASE                               0x0007c000
#define RP1_I2C4_BASE                               0x00080000
#define RP1_I2C5_BASE                               0x00084000
#define RP1_I2C6_BASE                               0x00088000
#define RP1_AUDIO_IN_BASE                           0x00090000
#define RP1_AUDIO_OUT_BASE                          0x00094000
#define RP1_PWM0_BASE                               0x00098000
#define RP1_PWM1_BASE                               0x0009c000
#define RP1_I2S0_BASE                               0x000a0000
#define RP1_I2S1_BASE                               0x000a4000
#define RP1_I2S2_BASE                               0x000a8000
#define RP1_TIMER_BASE                              0x000ac000
#define RP1_SDIO0_CFG_BASE                          0x000b0000
#define RP1_SDIO1_CFG_BASE                          0x000b4000
#define RP1_BUSFABRIC_MONITOR_BASE                  0x000c0000
#define RP1_BUSFABRIC_AXISHIM_BASE                  0x000c4000
#define RP1_ADC_BASE                                0x000c8000
#define RP1_IO_BANK0_BASE                           0x000d0000
#define RP1_IO_BANK1_BASE                           0x000d4000
#define RP1_IO_BANK2_BASE                           0x000d8000
#define RP1_SYS_RIO0_BASE                           0x000e0000
#define RP1_SYS_RIO1_BASE                           0x000e4000
#define RP1_SYS_RIO2_BASE                           0x000e8000
#define RP1_PADS_BANK0_BASE                         0x000f0000
#define RP1_PADS_BANK1_BASE                         0x000f4000
#define RP1_PADS_BANK2_BASE                         0x000f8000
#define RP1_PADS_ETH_BASE                           0x000fc000
#define RP1_ETH_BASE                                0x00100000
#define RP1_ETH_CFG_BASE                            0x00104000
#define RP1_PCIE_BASE                               0x00108000
#define RP1_MIPI0_CSIDMA_BASE                       0x00110000
#define RP1_MIPI0_CSIHOST_BASE                      0x00114000
#define RP1_MIPI0_DSIDMA_BASE                       0x00118000
#define RP1_MIPI0_DSIHOST_BASE                      0x0011c000
#define RP1_MIPI0_CFG_BASE                          0x00120000
#define RP1_MIPI0_ISP_BASE                          0x00124000
#define RP1_MIPI1_CSIDMA_BASE                       0x00128000
#define RP1_MIPI1_CSIHOST_BASE                      0x0012c000
#define RP1_MIPI1_DSIDMA_BASE                       0x00130000
#define RP1_MIPI1_DSIHOST_BASE                      0x00134000
#define RP1_MIPI1_CFG_BASE                          0x00138000
#define RP1_MIPI1_ISP_BASE                          0x0013c000
#define RP1_VIDEO_OUT_CFG_BASE                      0x00140000
#define RP1_VIDEO_OUT_VEC_BASE                      0x00144000
#define RP1_VIDEO_OUT_DPI_BASE                      0x00148000
#define RP1_XOSC_BASE                               0x00150000
#define RP1_WATCHDOG_BASE                           0x00154000
#define RP1_DMA_TICK_BASE                           0x00158000
#define RP1_USBHOST0_CFG_BASE                       0x00160000
#define RP1_USBHOST1_CFG_BASE                       0x00164000
#define RP1_ROSC0_BASE                              0x00168000
#define RP1_ROSC1_BASE                              0x0016c000
#define RP1_VBUSCTRL_BASE                           0x00170000
#define RP1_TICKS_BASE                              0x00174000
#define RP1_PIO_BASE                                0x00178000
#define RP1_SDIO0_BASE                              0x00180000
#define RP1_SDIO1_BASE                              0x00184000
#define RP1_DMA_BASE                                0x00188000
#define RP1_USBHOST0_BASE                           0x00200000
#define RP1_USBHOST1_BASE                           0x00300000
#define RP1_EXAC_BASE                               0x00400000

#define RP1_USBHOST_SIZE                            0x00100000

//
// Local MSI-X vectors
//
#define RP1_INT_IO_BANK0                            0
#define RP1_INT_IO_BANK1                            1
#define RP1_INT_IO_BANK2                            2
#define RP1_INT_AUDIO_IN                            3
#define RP1_INT_AUDIO_OUT                           4
#define RP1_INT_PWM0                                5
#define RP1_INT_ETH                                 6
#define RP1_INT_I2C0                                7
#define RP1_INT_I2C1                                8
#define RP1_INT_I2C2                                9
#define RP1_INT_I2C3                                10
#define RP1_INT_I2C4                                11
#define RP1_INT_I2C5                                12
#define RP1_INT_I2C6                                13
#define RP1_INT_I2S0                                14
#define RP1_INT_I2S1                                15
#define RP1_INT_I2S2                                16
#define RP1_INT_SDIO0                               17
#define RP1_INT_SDIO1                               18
#define RP1_INT_SPI0                                19
#define RP1_INT_SPI1                                20
#define RP1_INT_SPI2                                21
#define RP1_INT_SPI3                                22
#define RP1_INT_SPI4                                23
#define RP1_INT_SPI5                                24
#define RP1_INT_UART0                               25
#define RP1_INT_TIMER_0                             26
#define RP1_INT_TIMER_1                             27
#define RP1_INT_TIMER_2                             28
#define RP1_INT_TIMER_3                             29
#define RP1_INT_USBHOST0                            30
#define RP1_INT_USBHOST0_0                          31
#define RP1_INT_USBHOST0_1                          32
#define RP1_INT_USBHOST0_2                          33
#define RP1_INT_USBHOST0_3                          34
#define RP1_INT_USBHOST1                            35
#define RP1_INT_USBHOST1_0                          36
#define RP1_INT_USBHOST1_1                          37
#define RP1_INT_USBHOST1_2                          38
#define RP1_INT_USBHOST1_3                          39
#define RP1_INT_DMA                                 40
#define RP1_INT_PWM1                                41
#define RP1_INT_UART1                               42
#define RP1_INT_UART2                               43
#define RP1_INT_UART3                               44
#define RP1_INT_UART4                               45
#define RP1_INT_UART5                               46
#define RP1_INT_MIPI0                               47
#define RP1_INT_MIPI1                               48
#define RP1_INT_VIDEO_OUT                           49
#define RP1_INT_PIO_0                               50
#define RP1_INT_PIO_1                               51
#define RP1_INT_ADC_FIFO                            52
#define RP1_INT_PCIE_OUT                            53
#define RP1_INT_SPI6                                54
#define RP1_INT_SPI7                                55
#define RP1_INT_SPI8                                56
#define RP1_INT_PROC_MISC                           57
#define RP1_INT_SYSCFG                              58
#define RP1_INT_CLOCKS_DEFAULT                      59
#define RP1_INT_VBUSCTRL                            60

//
// System information registers
//
#define RP1_SYSINFO_CHIP_ID                         0x0

//
// PCIe endpoint configuration registers
//
#define RP1_PCIE_REG_RW                             (RP1_PCIE_BASE + 0x000)
#define RP1_PCIE_REG_SET                            (RP1_PCIE_BASE + 0x800)
#define RP1_PCIE_REG_CLR                            (RP1_PCIE_BASE + 0xc00)

// MSI-X vectors configuration
#define RP1_PCIE_MSIX_CFG(Irq)                      (0x008 + ((Irq) * 4))
#define RP1_PCIE_MSIX_CFG_IACK_EN                   BIT3
#define RP1_PCIE_MSIX_CFG_IACK                      BIT2
#define RP1_PCIE_MSIX_CFG_TEST                      BIT1
#define RP1_PCIE_MSIX_CFG_ENABLE                    BIT0

#endif // __RP1_H__
