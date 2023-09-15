/** @file
  The PEI_GRAPHICS_PLATFORM_POLICY_PPI provides platform information to PEI Graphics PEIM.

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PEI_GRAPHICS_PLATFORM_POLICY_PPI_H_
#define _PEI_GRAPHICS_PLATFORM_POLICY_PPI_H_

/**
  Globally unique identifier for PEI platform policy PPI.
**/
#define PEI_GRAPHICS_PLATFORM_POLICY_PPI_GUID \
{ \
  0x4eabcd09, 0x43d3, 0x4b4d, { 0xb7, 0x3d, 0x43, 0xc8, 0xd9, 0x89, 0x99, 0x5 } \
}

#define PEI_GRAPHICS_PLATFORM_POLICY_REVISION         1

/**
Pre-declaration of PEI graphics platform policy PPI.
**/
typedef struct _PEI_GRAPHICS_PLATFORM_POLICY_PPI PEI_GRAPHICS_PLATFORM_POLICY_PPI;

/**
  Enum defining the different lid status values
**/
typedef enum {
  LidClosed,
  LidOpen,
  LidStatusMax
} LID_STATUS;

/**
  This function gets the platform lid status for LFP displays.

  @param[out] CurrentLidStatus  Output variable to store the lid status.

  @retval EFI_SUCCESS           Correct lid status is returned.
  @retval EFI_UNSUPPORTED       Platform does not support lid.
**/
typedef
EFI_STATUS
(EFIAPI *GET_PLATFORM_LID_STATUS) (
  OUT LID_STATUS  *CurrentLidStatus
  );

/**
  This function gets the base address of loaded VBT.

  @param[out] VbtAddress    Starting address of the VBT is returned in this parameter.
  @param[out] VbtSize       Size of the VBT is returned in this parameter.

  @retval     EFI_SUCCESS   If the VBT is loaded and parameters contain valid values.
  @return     Other error codes meaning VBT is not loaded and parameters contain invalid values.
**/
typedef
EFI_STATUS
(EFIAPI *GET_VBT_DATA) (
  OUT EFI_PHYSICAL_ADDRESS *VbtAddress,
  OUT UINT32               *VbtSize
  );

/**
  This defines the PEI Graphics Platform Policy PPI structure.
**/
struct _PEI_GRAPHICS_PLATFORM_POLICY_PPI {
  UINT32                        Revision;               ///< Revision of current implementation.
  GET_PLATFORM_LID_STATUS       GetPlatformLidStatus;   ///< Function Pointer for get platform lid status.
  GET_VBT_DATA                  GetVbtData;             ///< Function pointer for get vbt data.
};

extern EFI_GUID gPeiGraphicsPlatformPpiGuid;

#endif
