/** @file
  This is the driver that initializes the Intel System Agent.

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#include "SaInitDxe.h"
#include "SaInit.h"
#include <MemInfoHob.h>
#include <Protocol/PciEnumerationComplete.h>
#include <Library/CpuPlatformLib.h>
#include <Library/CpuPcieInfoFruLib.h>

GLOBAL_REMOVE_IF_UNREFERENCED UINT16                   mPcieIoTrapAddress;

/**
  SystemAgent Dxe Initialization.

  @param[in] ImageHandle             Handle for the image of this driver
  @param[in] SystemTable             Pointer to the EFI System Table

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_OUT_OF_RESOURCES    No enough buffer to allocate
**/
EFI_STATUS
EFIAPI
SaInitEntryPointDxe (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  VOID                      *Registration;
#if FixedPcdGetBool(PcdFspWrapperEnable) == 0
  EFI_EVENT                 Event;
#endif

  DEBUG ((DEBUG_INFO, "SaInitDxe Start\n"));

  SaInitEntryPoint ();

  ///
  /// Create PCI Enumeration Completed callback for CPU PCIe
  ///
  EfiCreateProtocolNotifyEvent (
    &gEfiPciEnumerationCompleteProtocolGuid,
    TPL_CALLBACK,
    CpuPciEnumCompleteCallback,
    NULL,
    &Registration
    );

  DEBUG ((DEBUG_INFO, "SaInitDxe End\n"));

  return EFI_SUCCESS;
}

/**
  This function gets registered as a callback to perform CPU PCIe initialization before EndOfDxe

  @param[in] Event     - A pointer to the Event that triggered the callback.
  @param[in] Context   - A pointer to private data registered with the callback function.
**/
VOID
EFIAPI
CpuPciEnumCompleteCallback (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
  EFI_STATUS          Status;
  VOID                *ProtocolPointer;

  DEBUG ((DEBUG_INFO, "CpuPciEnumCompleteCallback Start\n"));
  ///
  /// Check if this is first time called by EfiCreateProtocolNotifyEvent() or not,
  /// if it is, we will skip it until real event is triggered
  ///
  Status = gBS->LocateProtocol (&gEfiPciEnumerationCompleteProtocolGuid, NULL, (VOID **) &ProtocolPointer);
  if (EFI_SUCCESS != Status) {
    return;
  }

  gBS->CloseEvent (Event);

  DEBUG ((DEBUG_INFO, "CpuPciEnumCompleteCallback End\n"));
  return;
}
