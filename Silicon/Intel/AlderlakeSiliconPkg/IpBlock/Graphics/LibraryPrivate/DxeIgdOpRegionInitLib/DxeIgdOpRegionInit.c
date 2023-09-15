/** @file
  This is part of the implementation of an Intel Graphics drivers OpRegion /
  Software SCI interface between system BIOS, ASL code, and Graphics drivers.
  The code in this file will load the driver and initialize the interface

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/DxeIgdOpRegionInitLib.h>



GLOBAL_REMOVE_IF_UNREFERENCED IGD_OPREGION_PROTOCOL           mIgdOpRegion;


/**
  Update Graphics OpRegion after PCI enumeration.

  @param[in] void         - None
  @retval EFI_SUCCESS     - The function completed successfully.
**/
EFI_STATUS
UpdateIgdOpRegionEndOfDxe (
  VOID
)
{
  EFI_STATUS                    Status;
  UINTN                         HandleCount;
  EFI_HANDLE                    *HandleBuffer;
  UINTN                         Index;
  EFI_PCI_IO_PROTOCOL           *PciIo;
  PCI_TYPE00                    Pci;
  UINTN                         Segment;
  UINTN                         Bus;
  UINTN                         Device;
  UINTN                         Function;

  Bus      = 0;
  Device   = 0;
  Function = 0;

  DEBUG ((DEBUG_INFO, "UpdateIgdOpRegionEndOfDxe\n"));

  mIgdOpRegion.OpRegion->Header.PCON |= BIT8; //Set External Gfx Adapter field is valid
  mIgdOpRegion.OpRegion->Header.PCON &= (UINT32) (~BIT7); //Assume No External Gfx Adapter

  ///
  /// Get all PCI IO protocols handles
  ///
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiPciIoProtocolGuid,
                  NULL,
                  &HandleCount,
                  &HandleBuffer
                  );

  if (!EFI_ERROR (Status)) {
    for (Index = 0; Index < HandleCount; Index++) {
      ///
      /// Get the PCI IO Protocol Interface corresponding to each handle
      ///
      Status = gBS->HandleProtocol (
                      HandleBuffer[Index],
                      &gEfiPciIoProtocolGuid,
                      (VOID **) &PciIo
                      );

      if (!EFI_ERROR (Status)) {
        ///
        /// Read the PCI configuration space
        ///
        Status = PciIo->Pci.Read (
                              PciIo,
                              EfiPciIoWidthUint32,
                              0,
                              sizeof (Pci) / sizeof (UINT32),
                              &Pci
                              );

        ///
        /// Find the display controllers devices
        ///
        if (!EFI_ERROR (Status) && IS_PCI_DISPLAY (&Pci)) {
          Status = PciIo->GetLocation (
                            PciIo,
                            &Segment,
                            &Bus,
                            &Device,
                            &Function
                            );

          //
          // Assumption: Onboard devices will be sits on Bus no 0, while external devices will be sits on Bus no > 0
          //
          if (!EFI_ERROR (Status) && (Bus > 0)) {
            //External Gfx Adapter Detected and Available
            DEBUG ((DEBUG_INFO, "PCON - External Gfx Adapter Detected and Available\n"));
            mIgdOpRegion.OpRegion->Header.PCON |= BIT7;
            break;
          }
        }
      }
    }
  }

  ///
  /// Free any allocated buffers
  ///
  if (HandleBuffer != NULL) {
    FreePool (HandleBuffer);
  }

  ///
  /// Return final status
  ///
  return Status;
}
