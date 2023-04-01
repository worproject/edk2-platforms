/** @file

  This file defines the manageability transport interface library and functions.

  Copyright (C) 2023 Advanced Micro Devices, Inc. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef MANAGEABILITY_TRANSPORT_HELPER_LIB_H_
#define MANAGEABILITY_TRANSPORT_HELPER_LIB_H_

#include <Library/ManageabilityTransportLib.h>

typedef struct _MANAGEABILITY_PROTOCOL_NAME MANAGEABILITY_PROTOCOL_NAME;

/**
  Helper function returns the human readable name of Manageability specification.

  @param[out]  SpecificationGuid         The Manageability specification GUID

  @retval      !NULL  Human readable name is returned;
  @retval       NULL  No string found, the given Manageability specification is
                      not supported.
**/
CHAR16 *
HelperManageabilitySpecName (
  IN EFI_GUID  *SpecificationGuid
  );

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
  );

/**
  Helper function to acquire the Manageability transport token.

  @param[in]  ManageabilityProtocolSpec   The Manageability protocol specification.
  @param[out] TransportToken              Pointer to receive Manageability transport
                                          token.

  @retval      EFI_SUCCESS            Token is created successfully.
  @retval      EFI_OUT_OF_RESOURCES   Out of resource to create a new transport session.
  @retval      EFI_UNSUPPORTED        Token is created successfully.
  @retval      EFI_DEVICE_ERROR       The transport interface has problems
  @retval      EFI_INVALID_PARAMETER  INput parameter is not valid.
               Otherwise              Other errors.
**/
EFI_STATUS
HelperAcquireManageabilityTransport (
  IN  EFI_GUID                       *ManageabilityProtocolSpec,
  OUT MANAGEABILITY_TRANSPORT_TOKEN  **TransportToken
  );

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
  );

#endif
