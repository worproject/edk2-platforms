/** @file
 *
 *  Broadcom BCM2712 PCI Host Bridge Library
 *
 *  This implementation configures the PCIe RCs as close to standard as
 *  possible, to accomodate ACPI OS usage.
 *
 *  The inbound and 64-bit outbound windows are identity mapped, while the
 *  32-bit non-prefetchable one is translated. Since this window must be
 *  located below 4 GB, it inevitably overlaps a part of the inbound window.
 *  This alone is not an issue; however, once the bridge's aperture is
 *  programmed, inbound (DMA) traffic cannot pass through it anymore.
 *
 *  In order to avoid DMA corruption, the platform must ensure that the decoded
 *  aperture (Memory Base - Limit in the root port configuration) is not used
 *  for DMA, either by reserving system RAM or by imposing a limit.
 *
 *  Copyright (c) 2024, Mario Bălănică <mariobalanica02@gmail.com>
 *  Copyright (c) 2017, Linaro Ltd. All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <Uefi.h>
#include <IndustryStandard/Bcm2712.h>
#include <IndustryStandard/Pci.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PciHostBridgeLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/PciRootBridgeIo.h>
#include <Protocol/PciHostBridgeResourceAllocation.h>

#include "Bcm2712PciHostBridge.h"

#pragma pack(1)
typedef struct {
  ACPI_HID_DEVICE_PATH        AcpiDevicePath;
  EFI_DEVICE_PATH_PROTOCOL    EndDevicePath;
} EFI_PCI_ROOT_BRIDGE_DEVICE_PATH;
#pragma pack ()

STATIC CONST EFI_PCI_ROOT_BRIDGE_DEVICE_PATH  mPciDevicePathTemplate[] = {
  {
    {
      {
        ACPI_DEVICE_PATH,
        ACPI_DP,
        {
          (UINT8)(sizeof (ACPI_HID_DEVICE_PATH)),
          (UINT8)(sizeof (ACPI_HID_DEVICE_PATH) >> 8)
        }
      },
      EISA_PNP_ID (0x0A08), // PCI Express
      0
    },

    {
      END_DEVICE_PATH_TYPE,
      END_ENTIRE_DEVICE_PATH_SUBTYPE,
      {
        END_DEVICE_PATH_LENGTH,
        0
      }
    }
  },
};

GLOBAL_REMOVE_IF_UNREFERENCED
CHAR16  *mPciHostBridgeLibAcpiAddressSpaceTypeStr[] = {
  L"Mem", L"I/O", L"Bus"
};

STATIC BCM2712_PCIE_RC  mPcieRcs[BCM2712_BRCMSTB_PCIE_COUNT] = {
  {
    .Base  = BCM2712_BRCMSTB_PCIE0_BASE,
    .Mem32 ={
      .CpuBase = BCM2712_BRCMSTB_PCIE0_CPU_MEM_BASE
    },
    .Mem64 ={
      .CpuBase = BCM2712_BRCMSTB_PCIE0_CPU_MEM64_BASE,
      .BusBase = BCM2712_BRCMSTB_PCIE0_CPU_MEM64_BASE,
      .Size    = BCM2712_BRCMSTB_PCIE_MEM64_SIZE
    }
  },
  {
    .Base  = BCM2712_BRCMSTB_PCIE1_BASE,
    .Mem32 ={
      .CpuBase = BCM2712_BRCMSTB_PCIE1_CPU_MEM_BASE
    },
    .Mem64 ={
      .CpuBase = BCM2712_BRCMSTB_PCIE1_CPU_MEM64_BASE,
      .BusBase = BCM2712_BRCMSTB_PCIE1_CPU_MEM64_BASE,
      .Size    = BCM2712_BRCMSTB_PCIE_MEM64_SIZE
    }
  },
  {
    .Base  = BCM2712_BRCMSTB_PCIE2_BASE,
    .Mem32 ={
      .CpuBase = BCM2712_BRCMSTB_PCIE2_CPU_MEM_BASE
    },
    .Mem64 ={
      .CpuBase = BCM2712_BRCMSTB_PCIE2_CPU_MEM64_BASE,
      .BusBase = BCM2712_BRCMSTB_PCIE2_CPU_MEM64_BASE,
      .Size    = BCM2712_BRCMSTB_PCIE_MEM64_SIZE
    }
  }
};

/**
  Return all the root bridge instances in an array.

  @param Count  Return the count of root bridge instances.

  @return All the root bridge instances in an array.
          The array should be passed into PciHostBridgeFreeRootBridges()
          when it's not used.
**/
PCI_ROOT_BRIDGE *
EFIAPI
PciHostBridgeGetRootBridges (
  OUT UINTN  *Count
  )
{
  EFI_STATUS                      Status;
  UINTN                           Seg;
  UINTN                           Index;
  UINTN                           SegCount;
  BCM2712_PCIE_PLATFORM_PROTOCOL  *PciePlatform;
  PCI_ROOT_BRIDGE                 *RootBridges;

  Status = gBS->LocateProtocol (
                  &gBcm2712PciePlatformProtocolGuid,
                  NULL,
                  (VOID **)&PciePlatform
                  );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    goto Fail;
  }

  SegCount = ARRAY_SIZE (mPcieRcs);

  RootBridges = AllocateZeroPool (SegCount * sizeof (PCI_ROOT_BRIDGE));
  if (RootBridges == NULL) {
    ASSERT (FALSE);
    goto Fail;
  }

  for (Seg = 0, Index = 0; Seg < SegCount; Seg++) {
    if (PciePlatform->Settings[Seg].Enabled == FALSE) {
      continue;
    }

    mPcieRcs[Seg].Inbound.CpuBase = 0;
    mPcieRcs[Seg].Inbound.BusBase = 0;
    mPcieRcs[Seg].Inbound.Size    = SIZE_64GB;

    mPcieRcs[Seg].Mem32.BusBase = PciePlatform->Mem32BusBase;
    mPcieRcs[Seg].Mem32.Size    = PciePlatform->Mem32Size;

    mPcieRcs[Seg].Settings = &PciePlatform->Settings[Seg];

    Status = PcieInitRc (&mPcieRcs[Seg]);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "PCIe: Failed to init segment %d. Status=%r\n", Seg, Status));
      continue;
    }

    RootBridges[Index].Segment               = Seg;
    RootBridges[Index].Supports              = 0;
    RootBridges[Index].Attributes            = RootBridges[Index].Supports;
    RootBridges[Index].DmaAbove4G            = FALSE;
    RootBridges[Index].NoExtendedConfigSpace = FALSE;
    RootBridges[Index].ResourceAssigned      = FALSE;
    RootBridges[Index].AllocationAttributes  = EFI_PCI_HOST_BRIDGE_COMBINE_MEM_PMEM |
                                               EFI_PCI_HOST_BRIDGE_MEM64_DECODE;
    RootBridges[Index].Bus.Base  = 0;
    RootBridges[Index].Bus.Limit = PCI_MAX_BUS;

    //
    // Workaround: pretend to support I/O space even though we don't,
    // because otherwise PciHostBridgeDxe would report a resource conflict
    // on devices with I/O BARs and end up destroying all root bridges.
    // Many such devices don't actually need those BARs to work (e.g. ASM1061,
    // RTL NICs), so ideally, PciBusDxe should skip allocating them when there's
    // no space and leave it up to the device drivers to decide if that's a problem.
    //
    RootBridges[Index].Io.Base        = 0;
    RootBridges[Index].Io.Limit       = RootBridges[Index].Io.Base + SIZE_64KB - 1;
    RootBridges[Index].Io.Translation = MAX_UINT64 - (mPcieRcs[Seg].Mem32.CpuBase + mPcieRcs[Seg].Mem32.Size) + 1;

    RootBridges[Index].Mem.Base        = mPcieRcs[Seg].Mem32.BusBase;
    RootBridges[Index].Mem.Limit       = RootBridges[Index].Mem.Base + mPcieRcs[Seg].Mem32.Size - 1;
    RootBridges[Index].Mem.Translation = MAX_UINT64 - (mPcieRcs[Seg].Mem32.CpuBase - mPcieRcs[Seg].Mem32.BusBase) + 1;

    RootBridges[Index].MemAbove4G.Base        = mPcieRcs[Seg].Mem64.BusBase;
    RootBridges[Index].MemAbove4G.Limit       = RootBridges[Index].MemAbove4G.Base + mPcieRcs[Seg].Mem64.Size - 1;
    RootBridges[Index].MemAbove4G.Translation = MAX_UINT64 - (mPcieRcs[Seg].Mem64.CpuBase - mPcieRcs[Seg].Mem64.BusBase) + 1;

    RootBridges[Index].PMem.Base  = MAX_UINT64;
    RootBridges[Index].PMem.Limit = 0;

    RootBridges[Index].PMemAbove4G.Base  = MAX_UINT64;
    RootBridges[Index].PMemAbove4G.Limit = 0;

    RootBridges[Index].DevicePath = AllocateCopyPool (
                                      sizeof (EFI_PCI_ROOT_BRIDGE_DEVICE_PATH),
                                      &mPciDevicePathTemplate
                                      );
    if (RootBridges[Index].DevicePath == NULL) {
      ASSERT (FALSE);
      PciHostBridgeFreeRootBridges (RootBridges, Index);
      goto Fail;
    }

    ((EFI_PCI_ROOT_BRIDGE_DEVICE_PATH *)RootBridges[Index].DevicePath)->AcpiDevicePath.UID = Seg;

    Index++;
  }

  if (Index == 0) {
    FreePool (RootBridges);
    goto Fail;
  }

  *Count = Index;
  return RootBridges;

Fail:
  *Count = 0;
  return NULL;
}

/**
  Free the root bridge instances array returned from PciHostBridgeGetRootBridges().

  @param Bridges The root bridge instances array.
  @param Count   The count of the array.
**/
VOID
EFIAPI
PciHostBridgeFreeRootBridges (
  PCI_ROOT_BRIDGE  *Bridges,
  UINTN            Count
  )
{
  UINTN  Index;

  if (Bridges == NULL) {
    return;
  }

  for (Index = 0; Index < Count; Index++) {
    FreePool (Bridges[Index].DevicePath);
  }

  FreePool (Bridges);
}

/**
  Inform the platform that the resource conflict happens.

  @param HostBridgeHandle Handle of the Host Bridge.
  @param Configuration    Pointer to PCI I/O and PCI memory resource
                          descriptors. The Configuration contains the resources
                          for all the root bridges. The resource for each root
                          bridge is terminated with END descriptor and an
                          additional END is appended indicating the end of the
                          entire resources. The resource descriptor field
                          values follow the description in
                          EFI_PCI_HOST_BRIDGE_RESOURCE_ALLOCATION_PROTOCOL
                          .SubmitResources().
**/
VOID
EFIAPI
PciHostBridgeResourceConflict (
  EFI_HANDLE  HostBridgeHandle,
  VOID        *Configuration
  )
{
  EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR  *Descriptor;
  UINTN                              RootBridgeIndex;

  DEBUG ((DEBUG_ERROR, "PciHostBridge: Resource conflict happens!\n"));

  RootBridgeIndex = 0;
  Descriptor      = (EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR *)Configuration;
  while (Descriptor->Desc == ACPI_ADDRESS_SPACE_DESCRIPTOR) {
    DEBUG ((DEBUG_ERROR, "RootBridge[%d]:\n", RootBridgeIndex++));
    for ( ; Descriptor->Desc == ACPI_ADDRESS_SPACE_DESCRIPTOR; Descriptor++) {
      ASSERT (
        Descriptor->ResType <
        ARRAY_SIZE (mPciHostBridgeLibAcpiAddressSpaceTypeStr)
        );
      DEBUG ((
        DEBUG_ERROR,
        " %s: Length/Alignment = 0x%lx / 0x%lx\n",
        mPciHostBridgeLibAcpiAddressSpaceTypeStr[Descriptor->ResType],
        Descriptor->AddrLen,
        Descriptor->AddrRangeMax
        ));
      if (Descriptor->ResType == ACPI_ADDRESS_SPACE_TYPE_MEM) {
        DEBUG ((
          DEBUG_ERROR,
          "     Granularity/SpecificFlag = %ld / %02x%s\n",
          Descriptor->AddrSpaceGranularity,
          Descriptor->SpecificFlag,
          ((Descriptor->SpecificFlag &
            EFI_ACPI_MEMORY_RESOURCE_SPECIFIC_FLAG_CACHEABLE_PREFETCHABLE
            ) != 0) ? L" (Prefetchable)" : L""
          ));
      }
    }

    //
    // Skip the END descriptor for root bridge
    //
    ASSERT (Descriptor->Desc == ACPI_END_TAG_DESCRIPTOR);
    Descriptor = (EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR *)(
                                                       (EFI_ACPI_END_TAG_DESCRIPTOR *)Descriptor + 1
                                                       );
  }
}
