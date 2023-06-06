/** @file
*  FDT client protocol driver for qemu,mach-virt-ahci DT node
*
*  Copyright (c) 2019, Linaro Ltd. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include <Library/ArmSmcLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/NonDiscoverableDeviceRegistrationLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <IndustryStandard/SbsaQemuSmc.h>

#include <Protocol/FdtClient.h>

EFI_STATUS
EFIAPI
InitializeSbsaQemuPlatformDxe (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
  EFI_STATUS                     Status;
  UINTN                          Size;
  VOID*                          Base;
  UINTN                          Arg0;
  UINTN                          Arg1;
  UINTN                          SmcResult;
  RETURN_STATUS                  Result;

  DEBUG ((DEBUG_INFO, "%a: InitializeSbsaQemuPlatformDxe called\n", __FUNCTION__));

  Base = (VOID*)(UINTN)PcdGet64 (PcdPlatformAhciBase);
  ASSERT (Base != NULL);
  Size = (UINTN)PcdGet32 (PcdPlatformAhciSize);
  ASSERT (Size != 0);

  DEBUG ((DEBUG_INFO, "%a: Got platform AHCI %llx %u\n",
          __FUNCTION__, Base, Size));

  Status = RegisterNonDiscoverableMmioDevice (
                   NonDiscoverableDeviceTypeAhci,
                   NonDiscoverableDeviceDmaTypeCoherent,
                   NULL,
                   NULL,
                   1,
                   Base, Size);

  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "%a: NonDiscoverable: Cannot install AHCI device @%p (Staus == %r)\n",
            __FUNCTION__, Base, Status));
    return Status;
  }

  SmcResult = ArmCallSmc0 (SIP_SVC_VERSION, &Arg0, &Arg1, NULL);
  if (SmcResult == SMC_ARCH_CALL_SUCCESS) {
    Result = PcdSet32S (PcdPlatformVersionMajor, Arg0);
    ASSERT_RETURN_ERROR (Result);
    Result = PcdSet32S (PcdPlatformVersionMinor, Arg1);
    ASSERT_RETURN_ERROR (Result);
  }

  Arg0 = PcdGet32 (PcdPlatformVersionMajor);
  Arg1 = PcdGet32 (PcdPlatformVersionMinor);

  DEBUG ((DEBUG_INFO, "Platform version: %d.%d\n", Arg0, Arg1));
  return EFI_SUCCESS;
}
