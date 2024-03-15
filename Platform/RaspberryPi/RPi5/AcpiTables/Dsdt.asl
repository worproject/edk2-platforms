/** @file
 *
 *  Differentiated System Definition Table (DSDT)
 *
 *  Copyright (c) 2023-2024, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <IndustryStandard/Bcm2712.h>
#include <RpiPlatformVarStoreData.h>

#include "AcpiTables.h"

DefinitionBlock ("Dsdt.aml", "DSDT", 2, "RPIFDN", "RPI5    ", 2)
{
  Scope (\_SB_)
  {
    Device (CPU0) {
      Name (_HID, "ACPI0007")
      Name (_UID, 0x0)
      Name (_STA, 0xf)
    }

    Device (CPU1) {
      Name (_HID, "ACPI0007")
      Name (_UID, 0x1)
      Name (_STA, 0xf)
    }

    Device (CPU2) {
      Name (_HID, "ACPI0007")
      Name (_UID, 0x2)
      Name (_STA, 0xf)
    }

    Device (CPU3) {
      Name (_HID, "ACPI0007")
      Name (_UID, 0x3)
      Name (_STA, 0xf)
    }

    //
    // Legacy SOC bus
    //
    Device (SOCB) {
      Name (_HID, "ACPI0004")
      Name (_UID, 0x0)
      Name (_CCA, 0x0)

      Method (_CRS, 0, Serialized) {
        //
        // Container devices with _DMA must have _CRS.
        // TO-DO: Is describing the entire MMIO range in a single resource
        // enough, or do we need to list each individual resource consumed
        // by the child devices?
        //
        Name (RBUF, ResourceTemplate () {
          QWORDMEMORY_BUF (00, ResourceProducer)
        })
        QWORD_SET (00, BCM2712_LEGACY_BUS_BASE, BCM2712_LEGACY_BUS_LENGTH, 0)
        Return (RBUF)
      }

      Name (_DMA, ResourceTemplate () {
        //
        // Only the first GB is available.
        // Bus 0xC0000000 -> CPU 0x00000000.
        //
        QWordMemory (ResourceProducer,
          PosDecode,
          MinFixed,
          MaxFixed,
          NonCacheable,
          ReadWrite,
          0x0,
          0x00000000C0000000, // MIN
          0x00000000FFFFFFFF, // MAX
          0xFFFFFFFF40000000, // TRA
          0x0000000040000000, // LEN
          ,
          ,
          )
      })

      //
      // PL011 Debug UART Port
      //
      Device (URT0) {
        Name (_HID, "ARMH0011")
        Name (_UID, 0x0)
        Name (_CCA, 0x0)

        Method (_CRS, 0x0, Serialized) {
          Name (RBUF, ResourceTemplate () {
            QWORDMEMORY_BUF (00, ResourceConsumer)
            Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { PL011_DEBUG_INTERRUPT }
          })
          QWORD_SET (00, PL011_DEBUG_BASE_ADDRESS, PL011_DEBUG_LENGTH, 0)
          Return (RBUF)
        }

        Name (_DSD, Package () {
          ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
          Package () {
            Package () { "clock-frequency", PL011_DEBUG_CLOCK_FREQUENCY }
          }
        })
      }
    } // Device (SOCB)

    //
    // PCIe Root Complexes
    //
    // These (and _STA) are patched by the platform driver:
    //
    Name (PBMA, ACPI_PATCH_BYTE_VALUE)
    Name (BB32, ACPI_PATCH_QWORD_VALUE)
    Name (MS32, ACPI_PATCH_QWORD_VALUE)

    Device (PCI0) {
      Name (_SEG, 0)
      Name (_STA, 0xF)

      Name (CFGB, BCM2712_BRCMSTB_PCIE0_BASE)
      Name (CFGS, BCM2712_BRCMSTB_PCIE_LENGTH)
      Name (MB32, BCM2712_BRCMSTB_PCIE0_CPU_MEM_BASE)
      Name (MB64, BCM2712_BRCMSTB_PCIE0_CPU_MEM64_BASE)
      Name (MS64, BCM2712_BRCMSTB_PCIE_MEM64_SIZE)

      Name (_PRT, Package () {
        Package (4) { 0x0FFFF, 0, 0, 241 },
        Package (4) { 0x0FFFF, 1, 0, 242 },
        Package (4) { 0x0FFFF, 2, 0, 243 },
        Package (4) { 0x0FFFF, 3, 0, 244 }
      })

      Include ("PcieCommon.asi")
    }

    Device (PCI1) {
      Name (_SEG, 1)
      Name (_STA, 0xF)

      Name (CFGB, BCM2712_BRCMSTB_PCIE1_BASE)
      Name (CFGS, BCM2712_BRCMSTB_PCIE_LENGTH)
      Name (MB32, BCM2712_BRCMSTB_PCIE1_CPU_MEM_BASE)
      Name (MB64, BCM2712_BRCMSTB_PCIE1_CPU_MEM64_BASE)
      Name (MS64, BCM2712_BRCMSTB_PCIE_MEM64_SIZE)

      Name (_PRT, Package () {
        Package (4) { 0x0FFFF, 0, 0, 251 },
        Package (4) { 0x0FFFF, 1, 0, 252 },
        Package (4) { 0x0FFFF, 2, 0, 253 },
        Package (4) { 0x0FFFF, 3, 0, 254 }
      })

      Include ("PcieCommon.asi")
    }

    Device (PCI2) {
      Name (_SEG, 2)
      Name (_STA, 0xF)

      Name (CFGB, BCM2712_BRCMSTB_PCIE2_BASE)
      Name (CFGS, BCM2712_BRCMSTB_PCIE_LENGTH)
      Name (MB32, BCM2712_BRCMSTB_PCIE2_CPU_MEM_BASE)
      Name (MB64, BCM2712_BRCMSTB_PCIE2_CPU_MEM64_BASE)
      Name (MS64, BCM2712_BRCMSTB_PCIE_MEM64_SIZE)

      Name (_PRT, Package () {
        Package (4) { 0x0FFFF, 0, 0, 261 },
        Package (4) { 0x0FFFF, 1, 0, 262 },
        Package (4) { 0x0FFFF, 2, 0, 263 },
        Package (4) { 0x0FFFF, 3, 0, 264 }
      })

      Include ("PcieCommon.asi")
    }

    //
    // RP1 I/O Bridge
    //
    Device (RP1B) {
      Name (_HID, "ACPI0004")
      Name (_UID, 0x1)

      // Parent bus is non-coherent
      Name (_CCA, 0x0)

      // Firmware mapped BAR - patched by platform driver
      Name (PBAR, ACPI_PATCH_QWORD_VALUE)

      // Shared level interrupt - PCIE2 INTA# SPI
      Name (PINT, 261)

      Method (_STA) {
        If (PBAR == ACPI_PATCH_QWORD_VALUE) {
          Return (0x0)
        }
        Return (0xF)
      }

      Include ("Rp1.asi")
    }

    //
    // Broadcom STB SDHCI controllers (Arasan IP)
    //
    // There are 2 notable quirks with these controllers:
    // 1) Broken 1.8v signaling switch: instead it's changed via an external
    //    regulator. Thankfully, Intel Bay Trail had the same issue, so we
    //    can pretend to be one of their affected HCs and reuse the _DSM
    //    workaround.
    //
    // 2) Capability claims hardware retuning is supported, but it causes issues.
    //    Windows will crash when switching to SDR50/SDR104. Linux does not appear
    //    to care, but we still override the "sdhci-caps-mask" property just in case.
    //
    // Supposedly there's a 32-bit bus access limitation too (inherited from BCM283x),
    // but no issues have actually been observed under stress test in both Windows
    // and Linux. Chances are this was fixed in the production BCM2712C0 stepping.
    //
    // We provide two compatibility modes:
    // 1) BRCMSTB _HID + Bay Trail _CID:
    //    - Windows binds to "VEN_8086&DEV_0F14" and has DDR50 with _DSM working.
    //      SDR104/50 modes can be enabled by a sdbus driver override.
    //
    //    - Linux recognizes "80860F16" but treats the controller as plain SDHCI and
    //      no _DSM, we limit the speed to HS via "sdhci-caps-mask".
    //
    //    - FreeBSD binds to "80860F16" but does not implement the _DSM nor the _DSD
    //      for caps override, fortunately it just falls back to HS.
    //
    // 2) Full Bay Trail _HID: this enables Linux to see the device as proper Bay Trail
    //    and use the _DSM. DDR50 is also enabled by relaxing the caps mask.
    //
    // The "Limit UHS-I" option is enabled by default in case OSes are not aware of
    // the broken retuning (i.e. Windows does not parse _DSD). It disables SDR104/50
    // since these modes depend on tuning.
    //
    // These will be patched in by the platform driver.
    //
    Name (SDCM, 0x0) // Compatibility Mode
    Name (SDLU, 0x0) // Limit UHS-I

    Device (SDC0) {
      Method (_HID) {
        If (SDCM == ACPI_SD_COMPAT_MODE_FULL_BAYTRAIL) {
          Return ("80860F16")
        } Else {
          Return ("BRCM5D12")
        }
      }
      Name (_CID, Package () { "80860F16", "VEN_8086&DEV_0F14" })
      Name (_UID, 0x0)
      Name (_CCA, 0x0)

      Method (_CRS, 0x0, Serialized) {
        Name (RBUF, ResourceTemplate () {
          QWORDMEMORY_BUF (00, ResourceConsumer)
          Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 305 }
        })
        QWORD_SET (00, BCM2712_BRCMSTB_SDIO1_HOST_BASE, BCM2712_BRCMSTB_SDIO_HOST_LENGTH, 0)
        Return (RBUF)
      }

      OperationRegion (GPIO, SystemMemory, BCM2712_BRCMSTB_GIO_AON_BASE, BCM2712_BRCMSTB_GIO_AON_LENGTH)
      Field (GPIO, DWordAcc, NoLock, Preserve) {
        Offset (0x4),
        DATA, 32,     // BIT3 = GPIO 3, 1.8v switch
      }

      Method (_INI, 0, Serialized) {
        DATA &= ~(1 << 3)
      }

      Method (_DSM, 4, Serialized) {
        // Check the UUID
        If (Arg0 == ToUUID ("f6c13ea5-65cd-461f-ab7a-29f7e8d5bd61")) {
          // Check the revision
          If (Arg1 >= 0) {
            // Check the function index
            Switch (ToInteger (Arg2)) {
              //
              // Supported functions:
              // Bit 0 - Indicates support for functions other than 0
              // Bit 3 - Indicates support to set 1.8V signalling
              // Bit 4 - Indicates support to set 3.3V signalling
              // Bit 8 - Indicates support for UHS-I modes
              //
              Case (0) {
                Return (Buffer () { 0x19, 0x01 }) // 0x119
              }

              // Function Index 3: Set 1.8v signalling
              Case (3) {
                DATA |= (1 << 3)
                Return (Buffer () { 0x00 })
              }

              // Function Index 4: Set 3.3v signalling
              Case (4) {
                DATA &= ~(1 << 3)
                Return (Buffer () { 0x00 })
              }

              //
              // Function Index 8: Supported UHS-I modes
              // Bit 0 - SDR25
              // Bit 1 - DDR50
              // Bit 2 - SDR50
              // Bit 3 - SDR104
              //
              Case (8) {
                // Limit UHS-I modes?
                If (SDLU == 1) {
                  Return (Buffer () { 0x02 }) // DDR50
                } Else {
                  Return (Buffer () { 0x0F }) // All
                }
              }
            } // Function index check
          } // Revision check
        } // UUID check
        Return (Buffer () { 0x0 })
      } // _DSM

      Method (_DSD, 0, Serialized) {
        // Capabilities mask
        Name (CAPM, 0x0000000000000000)

        // Start by disabling hardware retuning
        CAPM |= (1 << 47) | (1 << 46)

        // Limit UHS-I modes?
        If (SDLU == 1) {
          // Disable SDR104, SDR50
          CAPM |= (1 << 33) | (1 << 32)

          If (SDCM != ACPI_SD_COMPAT_MODE_FULL_BAYTRAIL) {
            // Additionally disable DDR50, Linux can't use
            // the _DSM for changing voltage in this case.
            CAPM |= (1 << 34)
          }
        }

        Return (Package () {
          ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
          Package () {
            Package () { "sdhci-caps-mask", CAPM }
          }
        })
      } // _DSD

      //
      // Removable SD card
      //
      Device (SDMM) {
        Name (_ADR, 0x0)

        Method (_RMV) {
          Return (1)
        }
      }
    } // Device (SDC0)

    //
    // This controller drives the SDIO Wi-Fi.
    // It can only run at DDR50 with fixed signaling voltage, so there's no
    // need to apply most of the workarounds above.
    //
    Device (SDC1) {
      Name (_HID, "BRCM5D12")
      Name (_CID, Package () { "80860F16", "VEN_8086&DEV_0F14" })
      Name (_UID, 0x1)
      Name (_CCA, 0x0)

      Method (_CRS, 0x0, Serialized) {
        Name (RBUF, ResourceTemplate () {
          QWORDMEMORY_BUF (00, ResourceConsumer)
          Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 306 }
        })
        QWORD_SET (00, BCM2712_BRCMSTB_SDIO2_HOST_BASE, BCM2712_BRCMSTB_SDIO_HOST_LENGTH, 0)
        Return (RBUF)
      }

      //
      // Only needed by Windows.
      //
      Method (_DSM, 4, Serialized) {
        // Check the UUID
        If (Arg0 == ToUUID ("f6c13ea5-65cd-461f-ab7a-29f7e8d5bd61")) {
          // Check the revision
          If (Arg1 >= 0) {
            // Check the function index
            Switch (ToInteger (Arg2)) {
              //
              // Supported functions:
              // Bit 0 - Indicates support for functions other than 0
              // Bit 8 - Indicates support for UHS-I modes
              //
              Case (0) {
                Return (Buffer () { 0x01, 0x01 }) // 0x101
              }

              //
              // Function Index 8: Supported UHS-I modes
              // Bit 0 - SDR25
              // Bit 1 - DDR50
              // Bit 2 - SDR50
              // Bit 3 - SDR104
              //
              Case (8) {
                Return (Buffer () { 0x02 }) // DDR50
              }
            } // Function index check
          } // Revision check
        } // UUID check
        Return (Buffer () { 0x0 })
      } // _DSM

      Method (_DSD, 0x0, Serialized) {
        Return (Package () {
          ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
          Package () {
            // Disable hardware retuning, SDR104, SDR50.
            Package () { "sdhci-caps-mask", (1 << 47) | (1 << 46) | (1 << 33) | (1 << 32) },
          }
        })
      } // _DSD

      //
      // Fixed CYW43455 SDIO Wi-Fi
      //
      Device (WLAN) {
        Name (_ADR, 0x1)

        Method (_RMV) {
          Return (0)
        }
      }
    } // Device (SDC1)

  } // Scope (\_SB_)
} // DefinitionBlock
