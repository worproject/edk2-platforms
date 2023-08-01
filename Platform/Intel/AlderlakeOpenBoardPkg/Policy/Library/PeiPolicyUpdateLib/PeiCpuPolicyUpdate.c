/** @file
  CPU PEI Policy Update & initialization.

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent


**/
#include "PeiCpuPolicyUpdate.h"
#include <Library/ConfigBlockLib.h>
#include <Library/CpuPlatformLib.h>
#include <Library/HobLib.h>
#include <Library/PeiSiPolicyUpdateLib.h>
#include <Library/SiPolicyLib.h>
#include <Library/Tpm12CommandLib.h>
#include <Library/Tpm2CommandLib.h>
#include <PolicyUpdateMacro.h>

/*
  Get the uCode region from PCD settings, and copy the patches to memory.
  This function is used to replace CpuLocateMicrocodePatch due to that function can not works
  with uCode update new design.
  In current uCode update solution, there are some padding data between uCode patches,
  the algorithm in CpuLocateMicrocodePatch can not handle this.
  Besides that, in CpuLocateMicrocodePatch function, the scan algorithm just find the first
  correct uCode patch which is not the highest version uCode.
  This function just copy the uCode region to memory, and in later, the CpuMpInit driver
  will load the correct patch for CPU.

  @param[out] RegionAddress     Pointer to the uCode array.
  @param[out] RegionSize        Size of the microcode FV.

  @retval EFI_SUCCESS           Find uCode patch region and success copy the data to memory.
  @retval EFI_NOT_FOUND         Something wrong with uCode region.
  @retval EFI_OUT_OF_RESOUCES   Memory allocation fail.
  @retval EFI_INVALID_PARAMETER RegionAddres or RegionSize is NULL.

*/
EFI_STATUS
SearchMicrocodeRegion (
  OUT UINTN                *RegionAddress,
  OUT UINTN                *RegionSize
  )
{
  UINTN                MicrocodeStart;
  UINTN                MicrocodeEnd;
  UINT8                *MemoryBuffer;

  if (RegionAddress == NULL || RegionSize == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  *RegionAddress = 0;
  *RegionSize = 0;

  if ((FixedPcdGet32 (PcdFlashFvMicrocodeBase) == 0) || (FixedPcdGet32 (PcdFlashFvMicrocodeSize) == 0)) {
    return EFI_NOT_FOUND;
  }

  MicrocodeStart = (UINTN) FixedPcdGet32 (PcdFlashFvMicrocodeBase) + (UINTN) FixedPcdGet32 (PcdMicrocodeOffsetInFv);
  MicrocodeEnd = (UINTN) FixedPcdGet32 (PcdFlashFvMicrocodeBase) + (UINTN) FixedPcdGet32 (PcdFlashFvMicrocodeSize);
  *RegionSize = MicrocodeEnd - MicrocodeStart;

  DEBUG ((DEBUG_INFO, "[SearchMicrocodeRegion]: Microcode Region Address = %x, Size = %d\n", MicrocodeStart, *RegionSize));

  MemoryBuffer = AllocatePages (EFI_SIZE_TO_PAGES (*RegionSize));
  ASSERT (MemoryBuffer != NULL);
  if (MemoryBuffer == NULL) {
    DEBUG ((DEBUG_ERROR, "Failed to allocate enough memory for Microcode Patch.\n"));
    return EFI_OUT_OF_RESOURCES;
  } else {
    CopyMem (MemoryBuffer, (UINT8 *)MicrocodeStart, *RegionSize);
    *RegionAddress = (UINTN)MemoryBuffer;
    DEBUG ((DEBUG_INFO, "Copy whole uCode region to memory, address = %x, size = %d\n", RegionAddress, *RegionSize));
  }

  return EFI_SUCCESS;
}

/**
  This function performs CPU PEI Policy initialization.

  @retval EFI_SUCCESS              The PPI is installed and initialized.
  @retval EFI ERRORS               The PPI is not successfully installed.
  @retval EFI_OUT_OF_RESOURCES     Do not have enough resources to initialize the driver
**/
EFI_STATUS
EFIAPI
UpdatePeiCpuPolicy (
  VOID
  )
{
  EFI_STATUS                       Status;
  SI_POLICY_PPI                    *SiPolicyPpi;
  CPU_CONFIG                       *CpuConfig;

  DEBUG ((DEBUG_INFO, "Update PeiCpuPolicyUpdate Pos-Mem Start\n"));

  SiPolicyPpi             = NULL;
  CpuConfig               = NULL;

  Status = PeiServicesLocatePpi (&gSiPolicyPpiGuid, 0, NULL, (VOID **) &SiPolicyPpi);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gCpuConfigGuid, (VOID *) &CpuConfig);
  ASSERT_EFI_ERROR (Status);

  Status = SearchMicrocodeRegion (
             (UINTN *)&CpuConfig->MicrocodePatchAddress,
             (UINTN *)&CpuConfig->MicrocodePatchRegionSize
             );

  return EFI_SUCCESS;
}
