/** @file
 *
 *  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <PiDxe.h>

#include <Library/DebugLib.h>
#include <Library/RealTimeClockLib.h>
#include <Library/TimeBaseLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeLib.h>

#include <Protocol/RpiFirmware.h>

STATIC RASPBERRY_PI_FIRMWARE_PROTOCOL *mFwProtocol;

STATIC
VOID
EFIAPI
OffsetTimeZoneEpoch (
  IN      EFI_TIME    *Time,
  IN OUT  UINT32      *EpochSeconds,
  IN      BOOLEAN     Add
  )
{
  //
  // Adjust for the correct time zone
  // The timezone setting also reflects the DST setting of the clock
  //
  if (Time->TimeZone != EFI_UNSPECIFIED_TIMEZONE) {
    *EpochSeconds += (Add ? 1 : -1) * Time->TimeZone * SEC_PER_MIN;
  } else if ((Time->Daylight & EFI_TIME_IN_DAYLIGHT) == EFI_TIME_IN_DAYLIGHT) {
    // Convert to adjusted time, i.e. spring forwards one hour
    *EpochSeconds += (Add ? 1 : -1) * SEC_PER_HOUR;
  }
}

/**
  Returns the current time and date information, and the time-keeping capabilities
  of the hardware platform.

  @param  Time                   A pointer to storage to receive a snapshot of the current time.
  @param  Capabilities           An optional pointer to a buffer to receive the real time clock
                                 device's capabilities.

  @retval EFI_SUCCESS            The operation completed successfully.
  @retval EFI_INVALID_PARAMETER  Time is NULL.
  @retval EFI_DEVICE_ERROR       The time could not be retrieved due to hardware error.
  @retval EFI_SECURITY_VIOLATION The time could not be retrieved due to an authentication failure.

**/
EFI_STATUS
EFIAPI
LibGetTime (
  OUT EFI_TIME               *Time,
  OUT EFI_TIME_CAPABILITIES  *Capabilities
  )
{
  EFI_STATUS  Status;
  UINT32      EpochSeconds;

  if (Time == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = mFwProtocol->GetRtc (RpiRtcTime, &EpochSeconds);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  OffsetTimeZoneEpoch (Time, &EpochSeconds, TRUE);

  EpochToEfiTime (EpochSeconds, Time);

  return EFI_SUCCESS;
}

/**
  Sets the current local time and date information.

  @param  Time                  A pointer to the current time.

  @retval EFI_SUCCESS           The operation completed successfully.
  @retval EFI_INVALID_PARAMETER A time field is out of range.
  @retval EFI_DEVICE_ERROR      The time could not be set due to hardware error.

**/
EFI_STATUS
EFIAPI
LibSetTime (
  IN  EFI_TIME  *Time
  )
{
  EFI_STATUS  Status;
  UINT32      EpochSeconds;

  if (Time == NULL || !IsTimeValid (Time)) {
    return EFI_UNSUPPORTED;
  }

  EpochSeconds = (UINT32)EfiTimeToEpoch (Time);

  OffsetTimeZoneEpoch (Time, &EpochSeconds, FALSE);

  Status = mFwProtocol->SetRtc (RpiRtcTime, EpochSeconds);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}

/**
  Returns the current wakeup alarm clock setting.

  @param  Enabled               Indicates if the alarm is currently enabled or disabled.
  @param  Pending               Indicates if the alarm signal is pending and requires acknowledgement.
  @param  Time                  The current alarm setting.

  @retval EFI_SUCCESS           The alarm settings were returned.
  @retval EFI_INVALID_PARAMETER Any parameter is NULL.
  @retval EFI_DEVICE_ERROR      The wakeup time could not be retrieved due to a hardware error.

**/
EFI_STATUS
EFIAPI
LibGetWakeupTime (
  OUT BOOLEAN   *Enabled,
  OUT BOOLEAN   *Pending,
  OUT EFI_TIME  *Time
  )
{
  EFI_STATUS  Status;
  UINT32      EpochSeconds;
  UINT32      EnableVal;
  UINT32      PendingVal;

  if (Time == NULL || Enabled == NULL || Pending == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = mFwProtocol->GetRtc (RpiRtcAlarmEnable, &EnableVal);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = mFwProtocol->GetRtc (RpiRtcAlarmPending, &PendingVal);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  *Enabled = EnableVal;
  *Pending = PendingVal;

  if (*Pending) {
    // Acknowledge alarm
    Status = mFwProtocol->SetRtc (RpiRtcAlarmPending, TRUE);
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }

  Status = mFwProtocol->GetRtc (RpiRtcAlarm, &EpochSeconds);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  OffsetTimeZoneEpoch (Time, &EpochSeconds, TRUE);

  EpochToEfiTime (EpochSeconds, Time);

  return EFI_SUCCESS;
}

/**
  Sets the system wakeup alarm clock time.

  @param  Enabled               Enable or disable the wakeup alarm.
  @param  Time                  If Enable is TRUE, the time to set the wakeup alarm for.

  @retval EFI_SUCCESS           If Enable is TRUE, then the wakeup alarm was enabled. If
                                Enable is FALSE, then the wakeup alarm was disabled.
  @retval EFI_INVALID_PARAMETER A time field is out of range.
  @retval EFI_DEVICE_ERROR      The wakeup time could not be set due to a hardware error.
  @retval EFI_UNSUPPORTED       A wakeup timer is not supported on this platform.

**/
EFI_STATUS
EFIAPI
LibSetWakeupTime (
  IN BOOLEAN    Enabled,
  OUT EFI_TIME  *Time
  )
{
  EFI_STATUS  Status;
  UINT32      EpochSeconds;

  if (Enabled) {
    if (Time == NULL || !IsTimeValid (Time)) {
      return EFI_INVALID_PARAMETER;
    }

    EpochSeconds = (UINT32)EfiTimeToEpoch (Time);

    OffsetTimeZoneEpoch (Time, &EpochSeconds, FALSE);

    Status = mFwProtocol->SetRtc (RpiRtcAlarm, EpochSeconds);
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }

  Status = mFwProtocol->SetRtc (RpiRtcAlarmEnable, Enabled);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}

/**
  Fixup internal data so that EFI can be call in virtual mode.
  Call the passed in Child Notify event and convert any pointers in
  lib to virtual mode.

  @param[in]    Event   The Event that is being processed
  @param[in]    Context Event Context

**/
STATIC
VOID
EFIAPI
VirtualAddressChangeNotify (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
{
  EfiConvertPointer (0x0, (VOID **)&mFwProtocol);
}

/**
  This is the declaration of an EFI image entry point. This can be the entry point to an application
  written to this specification, an EFI boot service driver, or an EFI runtime driver.

  @param  ImageHandle           Handle that identifies the loaded image.
  @param  SystemTable           System Table for this image.

  @retval EFI_SUCCESS           The operation completed successfully.

**/
EFI_STATUS
EFIAPI
LibRtcInitialize (
  IN EFI_HANDLE                 ImageHandle,
  IN EFI_SYSTEM_TABLE           *SystemTable
  )
{
  EFI_STATUS  Status;
  EFI_TIME    Time;
  EFI_EVENT   VirtualAddressChangeEvent;

  Status = gBS->LocateProtocol (
                  &gRaspberryPiFirmwareProtocolGuid,
                  NULL,
                  (VOID **)&mFwProtocol);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_NOTIFY,
                  VirtualAddressChangeNotify,
                  NULL,
                  &gEfiEventVirtualAddressChangeGuid,
                  &VirtualAddressChangeEvent);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Initial RTC time starts off at Epoch = 0, which is out
  // of UEFI's bounds. Update it to the firmware build time.
  //
  Status = LibGetTime (&Time, NULL);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  if (!IsTimeValid (&Time)) {
    EpochToEfiTime (BUILD_EPOCH, &Time);
    Status = LibSetTime (&Time);
    if (EFI_ERROR(Status)) {
      return Status;
    }
  }

  return EFI_SUCCESS;
}
