/** @file

Copyright (c) 2023, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _ACPI_BMC_STATE_H_
#define _ACPI_BMC_STATE_H_

//
// Statements that include other header files
//
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Guid/EventGroup.h>
#include <Protocol/IpmiTransportProtocol.h>
#include <Include/IpmiNetFnAppDefinitions.h>
#include <IndustryStandard/Ipmi.h>
#include <Library/IpmiBaseLib.h>
#include <Protocol/IpmiProtocol.h>

EFI_EVENT  mExitBootServicesEvent = NULL;

#endif
