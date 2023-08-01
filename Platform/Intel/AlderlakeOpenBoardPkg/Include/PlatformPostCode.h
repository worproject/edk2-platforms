/** @file

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef _PLATFORMPOSTCODE_H_
#define _PLATFORMPOSTCODE_H_

//
// GENERAL USAGE GUIDELINES
//

/**
[definition]
PostCode = XYZZ
X - "D"=premem, "9"=postmem, "8"=SMM, "7"=DXE
Y - "6"=platform driver, "5"=board driver
ZZ - "00"=entry, "7F"=exit
 - 1 - board init premem: entry/exit (0xD500/0xD57F)
 - 2 - platform init premem: entry/exit (0xD600/0xD67F)
 - 3 - board init postmem: entry/exit (0x9500/0x957F)
 - 4 - platform init postmem: entry/exit (0x9600/0x967F)
 - 5 - board init DXE: entry/exit (0x7500/0x757F)
 - 6 - platform init DXE: entry/exit (0x7600/0x767F)
 - 7 - platform SMM init: entry/exit (0x8600/0x867F)
 - 8 - BIOS S3 entry (0xB503) means BioS PC 03, to differentiate with ACPI _PTS PC
 - 9 - BIOS S4 entry (0xB504) means BioS PC 04, to differentiate with ACPI _PTS PC
 - 10 - BIOS S5 entry (0B505) means BioS PC 05, to differentiate with ACPI _PTS PC
*/

#define PLATFORM_INIT_PREMEM_ENTRY  0xD600
#define PLATFORM_INIT_PREMEM_EXIT   0xD67F
#define PLATFORM_INIT_POSTMEM_ENTRY 0x9600
#define PLATFORM_INIT_POSTMEM_EXIT  0x967F
#define PLATFORM_SMM_INIT_ENTRY     0x8600
#define PLATFORM_SMM_INIT_EXIT      0x867F

#endif
