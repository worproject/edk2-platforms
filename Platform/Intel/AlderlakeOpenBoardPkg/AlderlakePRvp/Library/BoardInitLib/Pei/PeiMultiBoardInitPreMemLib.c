/** @file
  PEI Multi-Board Initialization in Pre-Memory PEI Library

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PiPei.h>
#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/BoardInitLib.h>
#include <Library/MultiBoardInitSupportLib.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/BoardConfigLib.h>
#include <PlatformBoardId.h>

EFI_STATUS
EFIAPI
AdlPBoardDetect (
  VOID
  );

EFI_STATUS
EFIAPI
AdlPMultiBoardDetect (
  VOID
  );

EFI_BOOT_MODE
EFIAPI
AdlPBoardBootModeDetect (
  VOID
  );

EFI_STATUS
EFIAPI
AdlPBoardDebugInit (
  VOID
  );

EFI_STATUS
EFIAPI
AdlPBoardInitBeforeMemoryInit (
  VOID
  );

BOARD_DETECT_FUNC  mAdlPBoardDetectFunc = {
  AdlPMultiBoardDetect
};

BOARD_PRE_MEM_INIT_FUNC  mAdlPBoardPreMemInitFunc = {
  AdlPBoardDebugInit,
  AdlPBoardBootModeDetect,
  AdlPBoardInitBeforeMemoryInit,
  NULL, // BoardInitAfterMemoryInit
  NULL, // BoardInitBeforeTempRamExit
  NULL, // BoardInitAfterTempRamExit
};

EFI_STATUS
EFIAPI
AdlPMultiBoardDetect (
  VOID
  )
{
  UINT8  SkuType;
  DEBUG ((DEBUG_INFO, " In AdlPMultiBoardDetect \n"));

  AdlPBoardDetect ();

  SkuType = PcdGet8 (PcdSkuType);
  if (SkuType==AdlPSkuType) {
    RegisterBoardPreMemInit (&mAdlPBoardPreMemInitFunc);
  } else {
    DEBUG ((DEBUG_WARN,"Not a Valid Alderlake P Board\n"));
  }
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
PeiAdlPMultiBoardInitPreMemLibConstructor (
  VOID
  )
{
  return RegisterBoardDetect (&mAdlPBoardDetectFunc);
}
