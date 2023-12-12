/** @file
 *
 *  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <Uefi.h>
#include <Library/BoardRevisionHelperLib.h>

//
// https://www.raspberrypi.com/documentation/computers/raspberry-pi.html#new-style-revision-codes
//
#define RPI_MEMORY_SIZE(Rev)      ((Rev >> 20) & 0x07)
#define RPI_MANUFACTURER(Rev)     ((Rev >> 16) & 0x0F)
#define RPI_PROCESSOR(Rev)        ((Rev >> 12) & 0x0F)
#define RPI_TYPE(Rev)             ((Rev >> 4) & 0xFF)

UINT64
EFIAPI
BoardRevisionGetMemorySize (
  IN  UINT32  RevisionCode
  )
{
  if (RevisionCode != 0) {
      return SIZE_256MB * 1ULL << RPI_MEMORY_SIZE (RevisionCode);
  }
  return SIZE_256MB; // Smallest possible size
}

UINT32
EFIAPI
BoardRevisionGetModelFamily (
  IN  UINT32  RevisionCode
  )
{
  if (RevisionCode != 0) {
    switch (RPI_TYPE (RevisionCode)) {
      case 0x00:          // Raspberry Pi Model A
      case 0x01:          // Raspberry Pi Model B
      case 0x02:          // Raspberry Pi Model A+
      case 0x03:          // Raspberry Pi Model B+
      case 0x06:          // Raspberry Pi Compute Module 1
      case 0x09:          // Raspberry Pi Zero
      case 0x0C:          // Raspberry Pi Zero W
        return 1;
      case 0x04:          // Raspberry Pi 2 Model B
        return 2;
      case 0x08:          // Raspberry Pi 3 Model B
      case 0x0A:          // Raspberry Pi Compute Module 3
      case 0x0D:          // Raspberry Pi 3 Model B+
      case 0x0E:          // Raspberry Pi 3 Model A+
      case 0x10:          // Raspberry Pi Compute Module 3+
        return 3;
      case 0x11:          // Raspberry Pi 4 Model B
      case 0x13:          // Raspberry Pi 400
      case 0x14:          // Raspberry Pi Computer Module 4
        return 4;
      case 0x17:          // Raspberry Pi 5 Model B
        return 5;
    }
  }
  return 0;
}

CHAR8 *
EFIAPI
BoardRevisionGetModelName (
  IN  UINT32  RevisionCode
  )
{
  if (RevisionCode != 0) {
    switch (RPI_TYPE (RevisionCode)) {
      case 0x00:
        return "Raspberry Pi Model A";
      case 0x01:
        return "Raspberry Pi Model B";
      case 0x02:
        return "Raspberry Pi Model A+";
      case 0x03:
        return "Raspberry Pi Model B+";
      case 0x04:
        return "Raspberry Pi 2 Model B";
      case 0x06:
        return "Raspberry Pi Compute Module 1";
      case 0x08:
        return "Raspberry Pi 3 Model B";
      case 0x09:
        return "Raspberry Pi Zero";
      case 0x0A:
        return "Raspberry Pi Compute Module 3";
      case 0x0C:
        return "Raspberry Pi Zero W";
      case 0x0D:
        return "Raspberry Pi 3 Model B+";
      case 0x0E:
        return "Raspberry Pi 3 Model A+";
      case 0x10:
        return "Raspberry Pi Compute Module 3+";
      case 0x11:
        return "Raspberry Pi 4 Model B";
      case 0x13:
        return "Raspberry Pi 400";
      case 0x14:
        return "Raspberry Pi Compute Module 4";
      case 0x17:
        return "Raspberry Pi 5 Model B";
    }
  }
  return "Unknown Raspberry Pi Model";
}

CHAR8 *
EFIAPI
BoardRevisionGetManufacturerName (
  IN  UINT32  RevisionCode
  )
{
  if (RevisionCode != 0) {
    switch (RPI_MANUFACTURER (RevisionCode)) {
      case 0x00:
        return "Sony UK";
      case 0x01:
        return "Egoman";
      case 0x02:
      case 0x04:
        return "Embest";
      case 0x03:
        return "Sony Japan";
      case 0x05:
        return "Stadium";
    }
  }
  return "Unknown Manufacturer";
}

CHAR8 *
EFIAPI
BoardRevisionGetProcessorName (
  IN  UINT32  RevisionCode
  )
{
  if (RevisionCode != 0) {
    switch (RPI_PROCESSOR (RevisionCode)) {
      case 0x00:
        return "BCM2835 (ARM11)";
      case 0x01:
        return "BCM2836 (Arm Cortex-A7)";
      case 0x02:
        return "BCM2837 (Arm Cortex-A53)";
      case 0x03:
        return "BCM2711 (Arm Cortex-A72)";
      case 0x04:
        return "BCM2712 (Arm Cortex-A76)";
    }
  }
  return "Unknown CPU Model";
}
