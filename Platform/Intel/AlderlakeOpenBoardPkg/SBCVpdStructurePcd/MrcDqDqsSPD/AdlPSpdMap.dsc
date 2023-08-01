## @file
#  ADL P SPD DATA configuration file.
#
#   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
#   SPDX-License-Identifier: BSD-2-Clause-Patent
#
#

[PcdsDynamicExVpd.common.SkuIdAdlPDdr5Rvp]
gBoardModuleTokenSpaceGuid.VpdPcdMrcSpdData|*|{CODE(
{
// DDR5 1Rx16 - 4800 MHz
  1,
 {0x30,                                      ///< 0   1024 SPD bytes total
  0x08,                                      ///< 1   SPD Revision 0.8
  0x12,                                      ///< 2   DRAM Type: DDR5 SDRAM
  0x03,                                      ///< 3   Module Type: Not Hybrid (DRAM only) / SO-DIMM Solution
  0x04,                                      ///< 4   Monolithic SDRAM, 16 Gb SDRAM density
  0x00,                                      ///< 5   16 Rows, 10 Columns
  0x40,                                      ///< 6   x16 SDRAM I/O Width
  0x42,                                      ///< 7   4 Bank Groups, 4 Banks per Bank Group
  0x00,                                      ///< 8   Secondary SDRAM Density and Package
  0x00,                                      ///< 9   Secondary SDRAM Addressing
  0x00,                                      ///< 10  Secondary SDRAM I/O Width
  0x00,                                      ///< 11  Secondary BankGroups and Banks per Bank Group
  0x60,                                      ///< 12  PPR Supported, One row per bank group, Soft PPR Supported
  0x00,                                      ///< 13  Commercial Temperature Grade, 0 to 85 C
  0x00,                                      ///< 14  Reserved
  0x00,                                      ///< 15  Reserved
  0x00,                                      ///< 16  SDRAM Nominal Voltage VDD:  1.1V
  0x00,                                      ///< 17  SDRAM Nominal Voltage VDDQ: 1.1V
  0x00,                                      ///< 18  SDRAM Nominal Voltage VPP:  1.8V
  0x00,                                      ///< 19  Reserved
  0xA1,                                      ///< 20  tCKAVGmin LSB
  0x01,                                      ///< 21  tCKAVGmin MSB
  0xE8,                                      ///< 22  tCKAVGmax LSB
  0x03,                                      ///< 23  tCKAVGmax MSB
  0x72,                                      ///< 24  CAS Latencies supported (First Byte) : 32, 30, 28, 22
  0x15,                                      ///< 25  CAS Latencies supported (Second Byte): 44, 40, 36
  0x00,                                      ///< 26  CAS Latencies supported (Third Byte) :
  0x00,                                      ///< 27  CAS Latencies supported (Fourth Byte):
  0x00,                                      ///< 28  CAS Latencies supported (Fifth Byte) :
  0x00,                                      ///< 29  Reserved
  0x1E,                                      ///< 30  Minimum CAS Latency (tAAmin) LSB
  0x41,                                      ///< 31  Minimum CAS Latency (tAAmin) MSB
  0x1E,                                      ///< 32  Minimum RAS-to-CAS delay (tRCDmin) LSB
  0x41,                                      ///< 33  Minimum RAS-to-CAS delay (tRCDmin) MSB
  0x1E,                                      ///< 34  Minimum Row Precharge delay (tRPmin) LSB
  0x41,                                      ///< 35  Minimum Row Precharge delay (tRPmin) MSB
  0x00,                                      ///< 36  Minimum Active to Precharge delay (tRASmin) LSB
  0x7D,                                      ///< 37  Minimum Active to Precharge delay (tRASmin) MSB
  0x1E,                                      ///< 38  Minimum Active to Active/Refresh delay (tRCmin) LSB
  0xBE,                                      ///< 39  Minimum Active to Active/Refresh delay (tRCmin) MSB
  0x30,                                      ///< 40  Minimum Write Recovery time (tWRmin) LSB
  0x75,                                      ///< 41  Minimum Write Recovery time (tWRmin) MSB
  0x27,                                      ///< 42  Refresh Recovery Delay (tRFC1min) LSB
  0x01,                                      ///< 43  Refresh Recovery Delay (tRFC1min) MSB
  0xA0,                                      ///< 44  Refresh Recovery Delay (tRFC2min) MSB
  0x00,                                      ///< 45  Refresh Recovery Delay (tRFC2min) MSB
  0x82,                                      ///< 46  Refresh Recovery Delay (tRFCsbmin) MSB
  0x00,                                      ///< 47  Refresh Recovery Delay (tRFCsbmin) MSB
  0,  0,                                     ///< 48 - 49
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,              ///< 50 - 59
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,              ///< 60 - 69
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,              ///< 70 - 79
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,              ///< 80 - 89
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,              ///< 90 - 99
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,              ///< 100 - 109
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,              ///< 110 - 119
  0, 0, 0, 0, 0, 0,                          ///< 120 - 125
  0x47,                                      ///< 126 CRC Bytes 0 - 127 LSB
  0xAE,                                      ///< 127 CRC Bytes 0 - 127 MSB
  0, 0,                                      ///< 128 - 129
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,              ///< 130 - 139
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,              ///< 140 - 149
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,              ///< 150 - 159
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,              ///< 160 - 169
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,              ///< 170 - 179
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,              ///< 180 - 189
  0, 0,                                      ///< 190 - 191
  0x08,                                      ///< 192 SPD Revision for Module Information: 0.8
  0x00,                                      ///< 193 Reserved
  0xC2,                                      ///< 194 SPD Manufacturer ID First Byte
  0xC4,                                      ///< 195 SPD Manufacturer ID Second Byte
  0x80,                                      ///< 196 SPD Device Type
  0x00,                                      ///< 197 SPD Device Revision
  0x80,                                      ///< 198 PMIC0 Manufacturer ID First Byte
  0xB3,                                      ///< 199 PMIC0 Manufacturer ID Second Byte
  0x80,                                      ///< 200 PMIC0 Device Type
  0x11,                                      ///< 201 PMIC0 Device Revision
  0, 0, 0, 0,                                ///< 202 - 205 PMIC1
  0, 0, 0, 0,                                ///< 206 - 209 PMIC2
  0x80,                                      ///< 210 Thermal Sensors Manufacturer ID First Byte
  0xB3,                                      ///< 211 Thermal Sensors Manufacturer ID First Byte
  0x80,                                      ///< 212 Thermal Sensors Device Type
  0x11,                                      ///< 213 Thermal Sensors Device Revision
  0, 0, 0, 0, 0, 0,                          ///< 214 - 219
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,              ///< 220 - 229
  0x0F,                                      ///< 230 Module Nominal Height
  0x10,                                      ///< 231 Module Nominal Thickness
  0x00,                                      ///< 232 Reference Raw Card Used
  0x01,                                      ///< 233 1 Row of DRAM on Module
  0x01,                                      ///< 234 1 Rank, 8 bits SDRAM data width per channel
  0x22,                                      ///< 235 2 Channels per DIMM, 32 bits per Channel
  0, 0, 0, 0,                                ///< 236 - 239
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,              ///< 240 - 249
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,              ///< 250 - 259
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,              ///< 260 - 269
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,              ///< 270 - 279
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,              ///< 280 - 289
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,              ///< 290 - 299
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,              ///< 300 - 309
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,              ///< 310 - 319
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,              ///< 320 - 329
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,              ///< 330 - 339
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,              ///< 340 - 349
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,              ///< 350 - 359
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,              ///< 360 - 369
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,              ///< 370 - 379
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,              ///< 380 - 389
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,              ///< 390 - 399
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,              ///< 400 - 409
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,              ///< 410 - 419
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,              ///< 420 - 429
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,              ///< 430 - 439
  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,   ///< 440 - 445
  0x9C,                                      ///< 446 CRC for Bytes 128 - 253 LSB
  0xAD,                                      ///< 447 CRC for Bytes 128 - 253 MSB
  0, 0,                                      ///< 448 - 449
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,              ///< 450 - 459
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,              ///< 460 - 469
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,              ///< 470 - 479
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,              ///< 480 - 489
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,              ///< 490 - 499
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,              ///< 500 - 509
  0, 0                                       ///< 510 - 511
        ///< Ignore bytes 512-1023, @todo_adl: support 1024 bytes SPD array
}})}
