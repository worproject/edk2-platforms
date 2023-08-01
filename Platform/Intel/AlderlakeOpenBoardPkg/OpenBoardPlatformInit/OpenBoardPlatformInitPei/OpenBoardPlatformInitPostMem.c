/** @file
  Source code file for OpenBoard Platform Init PEI module

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Register/Msr.h>
#include <CpuRegs.h>
#include <Library/IoLib.h>
#include <Library/HobLib.h>
#include <Library/DebugLib.h>
#include <Library/PchInfoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PeiServicesLib.h>
#include <Ppi/EndOfPeiPhase.h>
#include <Library/MtrrLib.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Guid/SmramMemoryReserve.h>
#include <Library/ConfigBlockLib.h>
#include <Ppi/SiPolicy.h>
#include <PchPolicyCommon.h>
#include <Library/SiPolicyLib.h>
#include <Guid/FirmwareFileSystem2.h>
#include <Protocol/FirmwareVolumeBlock.h>
#include <Library/PostCodeLib.h>
#include <PlatformPostCode.h>
#include <Ppi/Spi.h>
#include <Library/MtrrLib.h>
#include <Library/PciSegmentLib.h>
#include <Register/PchRegs.h>
#include <PlatformBoardId.h>
#include <Core/Pei/PeiMain.h>
#include <Library/PchPciBdfLib.h>
#include <Ppi/GraphicsPlatformPolicyPpi.h>
#include <Library/PeiGetFvInfoLib.h>


EFI_STATUS
EFIAPI
OpenBoardPlatformInitEndOfPei (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  );

static EFI_PEI_NOTIFY_DESCRIPTOR  mEndOfPeiNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiEndOfPeiSignalPpiGuid,
  (EFI_PEIM_NOTIFY_ENTRY_POINT) OpenBoardPlatformInitEndOfPei
};

EFI_STATUS
EFIAPI
GetPeiPlatformLidStatus (
  OUT LID_STATUS  *CurrentLidStatus
  );

EFI_STATUS
EFIAPI
GetVbtData (
  OUT EFI_PHYSICAL_ADDRESS *VbtAddress,
  OUT UINT32               *VbtSize
  );

PEI_GRAPHICS_PLATFORM_POLICY_PPI PeiGraphicsPlatform = {
  PEI_GRAPHICS_PLATFORM_POLICY_REVISION,
  GetPeiPlatformLidStatus,
  GetVbtData
};

EFI_PEI_PPI_DESCRIPTOR  mPeiGraphicsPlatformPpi = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gPeiGraphicsPlatformPpiGuid,
  &PeiGraphicsPlatform
};

EFI_STATUS
EFIAPI
GetVbtData (
  OUT EFI_PHYSICAL_ADDRESS *VbtAddress,
  OUT UINT32               *VbtSize
  )
{
  EFI_GUID                        FileGuid;
  EFI_GUID                        BmpImageGuid;
  VOID                            *Buffer;
  UINT32                          Size;

  Size    = 0;
  Buffer  = NULL;


  DEBUG((DEBUG_INFO, "GetVbtData Entry\n"));

  CopyMem (&BmpImageGuid, PcdGetPtr(PcdIntelGraphicsVbtFileGuid), sizeof(BmpImageGuid));

  CopyMem(&FileGuid, &BmpImageGuid, sizeof(FileGuid));
  PeiGetSectionFromFv(FileGuid, &Buffer, &Size);
  if (Buffer == NULL) {
    DEBUG((DEBUG_ERROR, "Could not locate VBT\n"));
  } else {
    DEBUG ((DEBUG_INFO, "GetVbtData Buffer is 0x%x\n", Buffer));
    DEBUG ((DEBUG_INFO, "GetVbtData Size is 0x%x\n", Size));
    *VbtAddress = (EFI_PHYSICAL_ADDRESS)(UINTN)Buffer;
    *VbtSize    = Size;
  }
  DEBUG((DEBUG_INFO, "GetVbtData exit\n"));

  return EFI_SUCCESS;
}


/**
  This function will return Lid Status in PEI phase.

  @param[out] CurrentLidStatus

  @retval     EFI_SUCCESS
  @retval     EFI_UNSUPPORTED
**/

EFI_STATUS
EFIAPI
GetPeiPlatformLidStatus (
  OUT LID_STATUS  *CurrentLidStatus
  )
{
  DEBUG ((DEBUG_INFO, "LidStatus Unsupported\n"));
  return EFI_UNSUPPORTED;
}

/**
  Configure PciHostBridge related PCDs
**/
VOID
ConfigurePciHostBridgePcds (
  VOID
  )
{
  //
  // Provide 256GB available above 4GB MMIO resource
  // limited to use single variable MTRR to cover this above 4GB MMIO region.
  //
  PcdSet64S (PcdPciReservedMemAbove4GBBase, BASE_256GB);
  PcdSet64S (PcdPciReservedMemAbove4GBLimit, BASE_256GB + SIZE_256GB - 1);
  if (PcdGet64 (PcdPciReservedMemAbove4GBBase) < PcdGet64 (PcdPciReservedMemAbove4GBLimit)) {
    DEBUG ((DEBUG_INFO, " PCI space that above 4GB MMIO is from 0x%lX", PcdGet64 (PcdPciReservedMemAbove4GBBase)));
    DEBUG ((DEBUG_INFO, " to 0x%lX\n", PcdGet64 (PcdPciReservedMemAbove4GBLimit)));
  }
}

/**
  This function handles PlatformInit task at the end of PEI

  @param[in]  PeiServices  Pointer to PEI Services Table.
  @param[in]  NotifyDesc   Pointer to the descriptor for the Notification event that
                           caused this function to execute.
  @param[in]  Ppi          Pointer to the PPI data associated with this function.

  @retval     EFI_SUCCESS  The function completes successfully
  @retval     others
**/
EFI_STATUS
EFIAPI
OpenBoardPlatformInitEndOfPei (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  //
  // Configure PciHostBridge related PCDs before DXE phase
  //
  ConfigurePciHostBridgePcds ();

  return EFI_SUCCESS;
}


/**
  Platform Init PEI module entry point

  @param[in]  FileHandle           Not used.
  @param[in]  PeiServices          General purpose services available to every PEIM.

  @retval     EFI_SUCCESS          The function completes successfully
  @retval     EFI_OUT_OF_RESOURCES Insufficient resources to create database
**/
EFI_STATUS
EFIAPI
OpenBoardPlatformInitPostMemEntryPoint (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS                       Status;
  PEI_CORE_INSTANCE                *PrivateData;
  UINTN                            CurrentFv;
  PEI_CORE_FV_HANDLE               *CoreFvHandle;
  VOID                             *HobData;

  PostCode (PLATFORM_INIT_POSTMEM_ENTRY);

  //
  // Build a HOB to show current FV location for SA policy update code to consume.
  //
  PrivateData = PEI_CORE_INSTANCE_FROM_PS_THIS(PeiServices);
  CurrentFv = PrivateData->CurrentPeimFvCount;
  CoreFvHandle = &(PrivateData->Fv[CurrentFv]);

  HobData = BuildGuidHob (
             &gPlatformInitFvLocationGuid,
             sizeof (VOID *)
             );
  ASSERT (HobData != NULL);
  CopyMem (HobData, (VOID *) &CoreFvHandle, sizeof (VOID *));

  //
  // Install mPeiGraphicsPlatformPpi
  //
  DEBUG ((DEBUG_INFO, "Install mPeiGraphicsPlatformPpi \n"));
  Status = PeiServicesInstallPpi (&mPeiGraphicsPlatformPpi);

  //
  // Performing PlatformInitEndOfPei after EndOfPei PPI produced
  //
  Status = PeiServicesNotifyPpi (&mEndOfPeiNotifyList);
  PostCode (PLATFORM_INIT_POSTMEM_EXIT);

  return Status;
}
