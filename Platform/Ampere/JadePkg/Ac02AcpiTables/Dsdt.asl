/** @file

  Copyright (c) 2023, Ampere Computing LLC. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

DefinitionBlock("Dsdt.aml", "DSDT", 0x02, "Ampere", "Jade", 1) {
  //
  // Board Model
  Name(\BDMD, "Altra Max Jade Board")
  Name(AERF, 0)  // PCIe AER Firmware-First

  Scope(\_SB) {
    Include ("CommonDevices.asi")
    Include ("PCI-S0.Rca01.asi")
    Include ("PCI-S0.asi")
    Include ("PCI-S1.asi")
    Include ("PCI-PDRC.asi")
  }

  Include ("CPU.asi")
  Include ("PMU.asi")

} // DSDT
