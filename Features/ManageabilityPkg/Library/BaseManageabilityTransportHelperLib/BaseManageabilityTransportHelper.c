/** @file
  Null instance of Manageability Transport Helper Library

  Copyright (C) 2023 Advanced Micro Devices, Inc. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Uefi.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/ManageabilityTransportHelperLib.h>

//
// BaseManageabilityTransportHelper is used by PEI, DXE and SMM.
// Make sure the global variables added here should be unchangable.
//
MANAGEABILITY_SPECIFICATION_NAME  ManageabilitySpecNameTable[] = {
  { &gManageabilityTransportKcsGuid,         L"KCS"      },
  { &gManageabilityTransportSmbusI2cGuid,    L"SMBUS I2C"},
  { &gManageabilityTransportPciVdmGuid,      L"PCI VDM"  },
  { &gManageabilityTransportMctpGuid,        L"MCTP"     },
  { &gManageabilityProtocolIpmiGuid,         L"IPMI"     },
  { &gManageabilityProtocolMctpGuid,         L"MCTP"     },
  { &gManageabilityProtocolPldmGuid,         L"PLDM"     }
};

UINT16  mManageabilitySpecNum = sizeof (ManageabilitySpecNameTable)/ sizeof (MANAGEABILITY_SPECIFICATION_NAME);

/**
  Helper function returns the human readable name of Manageability specification.

  @param[in]  SpecificationGuid         The Manageability specification GUID

  @retval      !NULL  Human readable name is returned;
  @retval       NULL  No string found, the given Manageability specification is
                      not supported.
**/
CHAR16 *
HelperManageabilitySpecName (
  IN EFI_GUID  *SpecificationGuid
  )
{
  UINT16                            Index;
  MANAGEABILITY_SPECIFICATION_NAME  *ThisSpec;

  if (mManageabilitySpecNum == 0) {
    return NULL;
  }

  if (SpecificationGuid == NULL || IsZeroGuid (SpecificationGuid)) {
    DEBUG((DEBUG_ERROR, "%a: Improper input GUIDs, could be NULL or zero GUID.\n", __FUNCTION__));
    return NULL;
  }

  ThisSpec = ManageabilitySpecNameTable;
  for (Index = 0; Index < mManageabilitySpecNum; Index++) {
    if (CompareGuid (
          SpecificationGuid,
          ThisSpec->SpecificationGuid
          ))
    {
      return ThisSpec->SpecificationName;
    }

    ThisSpec++;
  }

  return NULL;
}

/**
  Helper function to check if the Manageability specification is supported
  by transport interface or not.

  @param[in]  TransportGuid                         GUID of the transport interface.
  @param[in]  SupportedManageabilityProtocolArray   The Manageability protocols supported
                                                    by the transport interface.
  @param[in]  NumberOfSupportedProtocolInArray      Number of protocols in the array.
  @param[in]  ManageabilityProtocolToCheck          The Manageability specification to check.

  @retval      EFI_SUCCESS            Token is created successfully.
  @retval      EFI_INVALID_PARAMETER  Either NumberOfSupportedProtocolInArray = 0 or
                                      SupportedManageabilityProtocolArray = NULL.
  @retval      EFI_UNSUPPORTED        Out of resource to create a new transport session.
               Otherwise              Other errors.
**/
EFI_STATUS
HelperManageabilityCheckSupportedSpec (
  IN  EFI_GUID  *TransportGuid,
  IN  EFI_GUID  **SupportedManageabilityProtocolArray,
  IN  UINT8     NumberOfSupportedProtocolInArray,
  IN  EFI_GUID  *ManageabilityProtocolToCheck
  )
{
  UINT16    Index;
  EFI_GUID  **ThisSpecGuid;

  if ((NumberOfSupportedProtocolInArray == 0) || (SupportedManageabilityProtocolArray == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  if (TransportGuid == NULL ||
      IsZeroGuid (TransportGuid) ||
      ManageabilityProtocolToCheck == NULL ||
      IsZeroGuid (ManageabilityProtocolToCheck)
      ) {
      DEBUG((DEBUG_ERROR, "%a: Improper input GUIDs, could be NULL or zero GUID.\n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }

  ThisSpecGuid = SupportedManageabilityProtocolArray;
  for (Index = 0; Index < NumberOfSupportedProtocolInArray; Index++) {
    if (CompareGuid (
          *ThisSpecGuid,
          ManageabilityProtocolToCheck
          ))
    {
      DEBUG ((
        DEBUG_VERBOSE,
        "%a: Transport interface %s supports %s manageability specification.\n",
        __FUNCTION__,
        HelperManageabilitySpecName (TransportGuid),
        HelperManageabilitySpecName (ManageabilityProtocolToCheck)
        ));
      return EFI_SUCCESS;
    }

    ThisSpecGuid++;
  }

  DEBUG ((
    DEBUG_ERROR,
    "%a: Transport interface %s doesn't support %s manageability specification.\n",
    __FUNCTION__,
    HelperManageabilitySpecName (TransportGuid),
    HelperManageabilitySpecName (ManageabilityProtocolToCheck)
    ));
  return EFI_UNSUPPORTED;
}

/**
  Helper function to acquire the Manageability transport token.

  @param[in]  ManageabilityProtocolSpec   The Manageability protocol specification.
  @param[out] TransportToken              Pointer to receive Manageability transport
                                          token.

  @retval      EFI_SUCCESS            Token is created successfully.
  @retval      EFI_OUT_OF_RESOURCES   Out of resource to create a new transport session.
  @retval      EFI_UNSUPPORTED        Token is created successfully.
  @retval      EFI_INVALID_PARAMETER  Input parameter is not valid.
               Otherwise              Other errors.
**/
EFI_STATUS
HelperAcquireManageabilityTransport (
  IN  EFI_GUID                       *ManageabilityProtocolSpec,
  OUT MANAGEABILITY_TRANSPORT_TOKEN  **TransportToken
  )
{
  EFI_STATUS  Status;
  CHAR16      *ManageabilityProtocolName;
  CHAR16      *ManageabilityTransportName;

  DEBUG ((DEBUG_INFO, "%a: Entry\n", __FUNCTION__));
  if ((TransportToken == NULL) || (ManageabilityProtocolSpec == NULL)) {
    DEBUG ((DEBUG_ERROR, "%a: One of the required input parameters is NULL.\n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }

  *TransportToken           = NULL;
  ManageabilityProtocolName = HelperManageabilitySpecName (ManageabilityProtocolSpec);
  if (ManageabilityProtocolName == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: Unsupported Manageability Protocol Specification.\n", __FUNCTION__));
    return EFI_UNSUPPORTED;
  }

  DEBUG ((DEBUG_INFO, "  Manageability protocol %s is going to acquire transport interface token...\n", ManageabilityProtocolName));

  Status = AcquireTransportSession (ManageabilityProtocolSpec, TransportToken);
  if (Status == EFI_UNSUPPORTED) {
    DEBUG ((DEBUG_ERROR, "%a: No supported transport interface for %s packet.\n", __FUNCTION__, ManageabilityProtocolName));
    return Status;
  }

  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "%a: Fail to acquire Manageability transport token for %s (%r).\n",
      __FUNCTION__,
      ManageabilityProtocolName,
      Status
      ));
    return Status;
  }

  ManageabilityTransportName = HelperManageabilitySpecName ((*TransportToken)->Transport->ManageabilityTransportSpecification);
  if (ManageabilityTransportName == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: Unsupported Manageability Transport Interface Specification\n", __FUNCTION__));
    return EFI_UNSUPPORTED;
  }

  DEBUG ((DEBUG_INFO, "%a: This is the transfer session for %s over %s\n", __FUNCTION__, ManageabilityProtocolName, ManageabilityTransportName));
  return Status;
}

/**
  Helper function to initial the transport interface.

  @param[in]  TransportToken              Transport token.
  @param[in]  HardwareInfo                Optional hardware information of transport interface.
  @param[out] TransportAdditionalStatus   Transport additional status.

  @retval      EFI_SUCCESS            Transport interface is initiated successfully.
  @retval      EFI_DEVICE_ERROR       The transport interface has problems
  @retval      EFI_INVALID_PARAMETER  INput parameter is not valid.
               Otherwise              Other errors.
**/
EFI_STATUS
HelperInitManageabilityTransport (
  IN  MANAGEABILITY_TRANSPORT_TOKEN                 *TransportToken,
  IN  MANAGEABILITY_TRANSPORT_HARDWARE_INFORMATION  HardwareInfo OPTIONAL,
  OUT MANAGEABILITY_TRANSPORT_ADDITIONAL_STATUS     *TransportAdditionalStatus OPTIONAL
  )
{
  EFI_STATUS  Status;

  if (TransportToken == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: TransportToken is invalid.\n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }

  // Initial transport interface.
  Status = TransportToken->Transport->Function.Version1_0->TransportInit (TransportToken, HardwareInfo);
  if ((Status != EFI_SUCCESS) && (Status != EFI_ALREADY_STARTED)) {
    if (Status == EFI_DEVICE_ERROR) {
      // Try to reset the transport and initialize it again.
      Status = TransportToken->Transport->Function.Version1_0->TransportReset (
                                                                 TransportToken,
                                                                 TransportAdditionalStatus
                                                                 );
      if (EFI_ERROR (Status)) {
        if (Status == EFI_UNSUPPORTED) {
          DEBUG ((DEBUG_ERROR, "%a: Transport interface doesn't have reset capability.\n", __FUNCTION__));
        } else {
          DEBUG ((DEBUG_ERROR, "%a: Fail to reset transport interface (%r).\n", __FUNCTION__, Status));
        }

        Status = EFI_DEVICE_ERROR;
      } else {
        Status = TransportToken->Transport->Function.Version1_0->TransportInit (TransportToken, HardwareInfo);
        if (EFI_ERROR (Status)) {
          DEBUG ((DEBUG_ERROR, "%a: Transport interface is not able to use after the reset (%r).\n", __FUNCTION__, Status));
        }
      }
    } else {
      DEBUG ((DEBUG_ERROR, "%a: Transport interface is not able to use (%r).\n", __FUNCTION__, Status));
    }
  }

  return Status;
}
