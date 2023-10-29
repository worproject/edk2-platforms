/** @file

Copyright (c) 2023, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <BmcAcpiState.h>

/**
  Notification function of Exit Boot Services event group.

  Send a command to BMC to set power state to S0.

  @param  Event         Event whose notification function is being invoked.
  @param  Context        Pointer to the notification function's context.

**/
VOID
EFIAPI
BmcAcpiPowerStateS0Notify (
  EFI_EVENT  Event,
  VOID       *Context
  )
{
  EFI_STATUS                         Status = EFI_SUCCESS;
  IPMI_SET_ACPI_POWER_STATE_REQUEST  AcpiPowerStateRequest;
  UINT8                              AcpiPowerStateResponse;
  UINT32                             ResponseDataSize = 0;

  AcpiPowerStateRequest.SystemPowerState.Bits.PowerState  = IPMI_SYSTEM_POWER_STATE_S0_G0;   // System Power State  S0
  AcpiPowerStateRequest.SystemPowerState.Bits.StateChange = 1;
  AcpiPowerStateRequest.DevicePowerState.Bits.PowerState  = IPMI_DEVICE_POWER_STATE_D0;   // Device State State  S0
  AcpiPowerStateRequest.DevicePowerState.Bits.StateChange = 1;

  ResponseDataSize = sizeof (AcpiPowerStateResponse);

  //
  // Send a command to BMC to set power state to S0.
  //
  Status = IpmiSubmitCommand (
             IPMI_NETFN_APP,
             IPMI_APP_SET_ACPI_POWERSTATE,
             (UINT8 *)&AcpiPowerStateRequest,
             sizeof (IPMI_SET_ACPI_POWER_STATE_REQUEST),
             (UINT8 *)&AcpiPowerStateResponse,
             &ResponseDataSize
             );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "IpmiSubmitCommand App Set Acpi Power State Failed %r\n", Status));
  }
}

/**

  Entry point for the Bmc Acpi State Dxe driver.
  Use this function to replace previous ACPI Enable SMM handler to set BMC ACPI power state.

  @param ImageHandle  -  Image Handle.
  @param SystemTable  -  EFI System Table.

  @retval EFI_SUCCESS  -  Function has completed successfully.

**/
EFI_STATUS
EFIAPI
BmcAcpiStateEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status;

  //
  // Create an Exit Boot Service to Send a command to BMC to set the present power state
  //
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_NOTIFY,
                  BmcAcpiPowerStateS0Notify,
                  NULL,
                  &gEfiEventExitBootServicesGuid,
                  &mExitBootServicesEvent
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Create Exit Boot Services Event Failed\n"));
  } else {
    return EFI_SUCCESS;
  }

  return EFI_UNSUPPORTED;
}
