/** @file
  This file provide services for DXE phase policy default initialization

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "DxeSaPolicyLibrary.h"
#include <Library/DxeGraphicsPolicyLib.h>
#include <Register/SaRegsHostBridge.h>
#include "MemoryConfig.h"

extern EFI_GUID gMemoryDxeConfigGuid;

/**
  This function prints the SA DXE phase policy.

  @param[in] SaPolicy - SA DXE Policy protocol
**/
VOID
SaPrintPolicyProtocol (
  IN  SA_POLICY_PROTOCOL      *SaPolicy
  )
{
  UINT8                       ControllerIndex;
  UINT8                       ChannelIndex;
  EFI_STATUS                  Status;
  MEMORY_DXE_CONFIG           *MemoryDxeConfig;

  Status = GetConfigBlock ((VOID *) SaPolicy, &gMemoryDxeConfigGuid, (VOID *)&MemoryDxeConfig);
  ASSERT_EFI_ERROR (Status);


  DEBUG_CODE_BEGIN ();
  INTN  i;

  DEBUG ((DEBUG_INFO, "\n------------------------ SA Policy (DXE) print BEGIN -----------------\n"));
  DEBUG ((DEBUG_INFO, "Revision : %x\n", SaPolicy->TableHeader.Header.Revision));
  ASSERT (SaPolicy->TableHeader.Header.Revision == SA_POLICY_PROTOCOL_REVISION);

  DEBUG ((DEBUG_INFO, "------------------------ SA_MEMORY_CONFIGURATION -----------------\n"));

  DEBUG ((DEBUG_INFO, " SpdAddressTable[%d] :", 4));
  for (i = 0; i < 4; i++) {
    DEBUG ((DEBUG_INFO, " %x", MemoryDxeConfig->SpdAddressTable[i]));
  }
  DEBUG ((DEBUG_INFO, "\n"));

  for (ControllerIndex = 0; ControllerIndex < MEM_CFG_MAX_CONTROLLERS; ControllerIndex++) {
    for (ChannelIndex = 0; ChannelIndex < MEM_CFG_MAX_CHANNELS; ChannelIndex++) {
      DEBUG ((DEBUG_INFO, " SlotMap[%d][%d] : 0x%x\n", ControllerIndex, ChannelIndex, MemoryDxeConfig->SlotMap[ControllerIndex][ChannelIndex]));
    }
  }
  DEBUG ((DEBUG_INFO, " MrcTimeMeasure  : %x\n", MemoryDxeConfig->MrcTimeMeasure));
  DEBUG ((DEBUG_INFO, " MrcFastBoot     : %x\n", MemoryDxeConfig->MrcFastBoot));

  DEBUG ((DEBUG_INFO, "------------------------ CPU_PCIE_CONFIGURATION -----------------\n"));
  DEBUG ((DEBUG_INFO, " PegAspm[%d] :", SA_PEG_MAX_FUN));
  DEBUG ((DEBUG_INFO, " PegRootPortHPE[%d] :", SA_PEG_MAX_FUN));
  DEBUG ((DEBUG_INFO, "\n"));


  DEBUG ((DEBUG_INFO, "\n------------------------ SA Policy (DXE) print END -----------------\n"));
  DEBUG_CODE_END ();

  return;
}

/**
  Load DXE Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadMemoryDxeDefault (
  IN VOID    *ConfigBlockPointer
  )
{
  UINT8                    ControllerIndex;
  UINT8                    ChannelIndex;
  MEMORY_DXE_CONFIG        *MemoryDxeConfig;

  MemoryDxeConfig = ConfigBlockPointer;
  ///
  /// Initialize the Memory Configuration
  ///
  ///
  /// DIMM SMBus addresses info
  /// Refer to the SpdAddressTable[] mapping rule in DxeSaPolicyLibrary.h
  ///
  MemoryDxeConfig->SpdAddressTable = AllocateZeroPool (sizeof (UINT8) * 4);
  ASSERT (MemoryDxeConfig->SpdAddressTable != NULL);
  if (MemoryDxeConfig->SpdAddressTable != NULL) {
    MemoryDxeConfig->SpdAddressTable[0] = DIMM_SMB_SPD_P0C0D0;
    MemoryDxeConfig->SpdAddressTable[1] = DIMM_SMB_SPD_P0C0D1;
    MemoryDxeConfig->SpdAddressTable[2] = DIMM_SMB_SPD_P0C1D0;
    MemoryDxeConfig->SpdAddressTable[3] = DIMM_SMB_SPD_P0C1D1;
  }
  MemoryDxeConfig->SlotMap = (UINT8**)AllocateZeroPool (sizeof (UINT8*) * MEM_CFG_MAX_CONTROLLERS);
  ASSERT (MemoryDxeConfig->SlotMap != NULL);
  if (MemoryDxeConfig->SlotMap != NULL) {
    for (ControllerIndex = 0; ControllerIndex < MEM_CFG_MAX_CONTROLLERS; ControllerIndex++) {
      MemoryDxeConfig->SlotMap[ControllerIndex] = (UINT8*)AllocateZeroPool (sizeof (UINT8) * MEM_CFG_MAX_CHANNELS);
      ASSERT (MemoryDxeConfig->SlotMap[ControllerIndex] != NULL);
      if (MemoryDxeConfig->SlotMap[ControllerIndex] != NULL) {
        for (ChannelIndex = 0; ChannelIndex < MEM_CFG_MAX_CHANNELS; ChannelIndex++) {
          MemoryDxeConfig->SlotMap[ControllerIndex][ChannelIndex] = 0x01;
        }
      }
    }
  }
}

/**
  LoadSaDxeConfigBlockDefault - Initialize default settings for each SA Config block

  @param[in] ConfigBlockPointer         The buffer pointer that will be initialized as specific config block
  @param[in] BlockId                    Request to initialize defaults of specified config block by given Block ID

  @retval EFI_SUCCESS                   The given buffer has contained the defaults of requested config block
  @retval EFI_NOT_FOUND                 Block ID is not defined so no default Config block will be initialized
**/

GLOBAL_REMOVE_IF_UNREFERENCED COMPONENT_BLOCK_ENTRY  mSaDxeIpBlocks [] = {
  {&gMemoryDxeConfigGuid,   sizeof (MEMORY_DXE_CONFIG),   MEMORY_DXE_CONFIG_REVISION,    LoadMemoryDxeDefault}
};


/**
  CreateSaDxeConfigBlocks generates the config blocksg of SA DXE Policy.
  It allocates and zero out buffer, and fills in the Intel default settings.

  @param[out] SaPolicy               The pointer to get SA  DXE Protocol instance

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
CreateSaDxeConfigBlocks (
  IN OUT  SA_POLICY_PROTOCOL      **SaPolicy
  )
{
  UINT16              TotalBlockSize;
  EFI_STATUS          Status;
  SA_POLICY_PROTOCOL  *SaInitPolicy;
  UINT16              RequiredSize;

  DEBUG ((DEBUG_INFO, "SA Create Dxe Config Blocks\n"));

  SaInitPolicy = NULL;

  TotalBlockSize = GetComponentConfigBlockTotalSize (&mSaDxeIpBlocks[0], sizeof (mSaDxeIpBlocks) / sizeof (COMPONENT_BLOCK_ENTRY));
  TotalBlockSize += GraphicsGetConfigBlockTotalSizeDxe ();
  DEBUG ((DEBUG_INFO, "TotalBlockSize = 0x%x\n", TotalBlockSize));

  RequiredSize = sizeof (CONFIG_BLOCK_TABLE_HEADER) + TotalBlockSize;

  Status = CreateConfigBlockTable (RequiredSize, (VOID *) &SaInitPolicy);
  ASSERT_EFI_ERROR (Status);

  //
  // Initialize Policy Revision
  //
  SaInitPolicy->TableHeader.Header.Revision = SA_POLICY_PROTOCOL_REVISION;
  //
  // Add config blocks.
  //
  Status =  AddComponentConfigBlocks ((VOID *) SaInitPolicy, &mSaDxeIpBlocks[0], sizeof (mSaDxeIpBlocks) / sizeof (COMPONENT_BLOCK_ENTRY));
  ASSERT_EFI_ERROR (Status);


  // Gfx
  Status = GraphicsAddConfigBlocksDxe ((VOID *) SaInitPolicy);
  ASSERT_EFI_ERROR (Status);

  //
  // Assignment for returning SaInitPolicy config block base address
  //
  *SaPolicy = SaInitPolicy;
  return Status;
}


/**
  SaInstallPolicyProtocol installs SA Policy.
  While installed, RC assumes the Policy is ready and finalized. So please update and override
  any setting before calling this function.

  @param[in] ImageHandle                Image handle of this driver.
  @param[in] SaPolicy                   The pointer to SA Policy Protocol instance

  @retval EFI_SUCCESS                   The policy is installed.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer

**/
EFI_STATUS
EFIAPI
SaInstallPolicyProtocol (
  IN  EFI_HANDLE                  ImageHandle,
  IN  SA_POLICY_PROTOCOL         *SaPolicy
  )
{
  EFI_STATUS            Status;

  ///
  /// Print SA DXE Policy
  ///
  SaPrintPolicyProtocol (SaPolicy);
  GraphicsDxePolicyPrint (SaPolicy);

  ///
  /// Install protocol to to allow access to this Policy.
  ///
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gSaPolicyProtocolGuid,
                  SaPolicy,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

  return Status;
}

