/** @file
* Secondary System Description Table Fields (SSDT) for Virtio-P9 device.
*
* Some of the Arm Reference Design FVP platforms support the Virtio-P9 device
* as part of the RoS subsystem. The Virtio-P9 device implements a subset of the
* Plan 9 file protocol over a virtio transport. It enables accessing a shared
* directory on the host's filesystem from a running FVP platform.
* This file describes the SSDT entry for this Virtio-P9 device
*
* Copyright (c) 2023, Arm Ltd. All rights reserved.
*
* SPDX-License-Identifier: BSD-2-Clause-Patent
*
* @par Specification Reference:
*   - ACPI 6.4, Chapter 5, Section 5.2.11.2, Secondary System Description Table
**/

#include "SgiAcpiHeader.h"
#include "SgiPlatform.h"

DefinitionBlock ("SsdtRosVirtioP9.aml", "SSDT", 2, "ARMLTD", "ARMSGI",
                 EFI_ACPI_ARM_OEM_REVISION) {
  Scope (_SB) {
    // VIRTIO P9 device
    Device (VP90) {
      Name (_HID, "LNRO0005")
      Name (_UID, 2)
      Name (_CCA, 1)    // mark the device coherent

      Name (_CRS, ResourceTemplate() {
        Memory32Fixed (
          ReadWrite,
          FixedPcdGet32 (PcdVirtioP9BaseAddress),
          FixedPcdGet32 (PcdVirtioP9Size)
        )
        Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) {
          FixedPcdGet32 (PcdVirtioP9Interrupt)
        }
      })
    }
  } // Scope(_SB)
}
