/** @file
  Lightweight lib to support EFI Server Management drivers.

Copyright (c) 2023, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PiDxe.h>
#include <Library/DebugLib.h>
#include <Library/ServerMgmtRtLib.h>

/**
  The function will set up a notification on the ELOG protocol.  This function is required to be called prior
  to utilizing the ELOG protocol from within this library.

  @retval EFI_SUCCESS          after the notification has been setup.
**/
EFI_STATUS
EfiInitializeGenericElog (
  VOID
  )
{
  return EFI_UNSUPPORTED;
}

/**
  This function sends event log data to the destination such as LAN, ICMB, BMC etc.

  @param ElogData     is a pointer to the event log data that needs to be recorded
  @param DataType     type of Elog data that is being recorded.  The Elog is redirected based on this parameter.
  @param AlertEvent   is an indication that the input data type is an alert.  The underlying
                      drivers need to decide if they need to listen to the DataType and send it on
                      an appropriate channel as an alert use of the information.
  @param DataSize     is the size of the data to be logged
  @param RecordId     is the array of record IDs sent by the target.  This can be used to retrieve the
                      records or erase the records.
  @retval EFI_NOT_FOUND - the event log target was not found

**/
EFI_STATUS
EfiSmSetEventLogData (
  IN  UINT8             *ElogData,
  IN  EFI_SM_ELOG_TYPE  DataType,
  IN  BOOLEAN           AlertEvent,
  IN  UINTN             DataSize,
  OUT UINT64            *RecordId
  )
{
  return EFI_NOT_FOUND;
}

/**
  This function gets event log data from the destination dependent on the DataType.  The destination
  can be a remote target such as LAN, ICMB, IPMI, or a FV.  The ELOG redir driver will resolve the
  destination.

  @param ElogData - pointer to the event log data buffer to contain the data to be retrieved.
  @param DataType - this is the type of Elog data to be gotten.  Elog is redirected based upon this
                    information.
  @param DataSize - this is the size of the data to be retrieved.
  @param RecordId - the RecordId of the next record.  If ElogData is NULL, this gives the RecordId of the first
                    record available in the database with the correct DataSize.  A value of 0 on return indicates
                    that it was last record if the Status is EFI_SUCCESS.

  @retval EFI_NOT_FOUND - if the event log target was not found.

**/
EFI_STATUS
EfiSmGetEventLogData (
  IN  UINT8             *ElogData,
  IN  EFI_SM_ELOG_TYPE  DataType,
  IN  OUT UINTN         *DataSize,
  IN OUT UINT64         *RecordId
  )
{
  return EFI_NOT_FOUND;
}

/**
  This function erases the event log data defined by the DataType.  The redir driver associated with
  the DataType resolves the path to the record.

  @param DataType - the type of Elog data that is to be erased.
  @param RecordId - the RecordId of the data to be erased.  If RecordId is NULL, all records in the
                    database are erased if permitted by the target.  RecordId will contain the deleted
                    RecordId on return.

  @retval EFI_SUCCESS - the record or collection of records were erased.
  @retval EFI_NOT_FOUND - the event log target was not found.
  @retval EFI_NO_RESPONSE - the event log target was found but did not respond.
  @retval EFI_INVALID_PARAMETER - one of the parameters was invalid.

**/
EFI_STATUS
EfiSmEraseEventlogData (
  IN EFI_SM_ELOG_TYPE  DataType,
  IN OUT UINT64        *RecordId
  )
{
  return EFI_NOT_FOUND;
}

/**
  This function enables or disables the event log defined by the DataType.


  @param DataType   - the type of Elog data that is being activated.
  @param EnableElog - enables or disables the event log defined by the DataType.  If it is NULL
                      it returns the current status of the DataType log.
  @param ElogStatus - is the current status of the Event log defined by the DataType.  Enabled is
                      TRUE and Disabled is FALSE.

  @retval EFI_NOT_FOUND - the event log target was not found.

**/
EFI_STATUS
EfiSmActivateEventLog (
  IN EFI_SM_ELOG_TYPE  DataType,
  IN BOOLEAN           *EnableElog,
  OUT BOOLEAN          *ElogStatus
  )
{
  return EFI_NOT_FOUND;
}

/**
  Return Date and Time from RTC in Unix format which fits in 32 bit format.

  @param NumOfSeconds - pointer to return calculated time

  @retval EFI_SUCCESS
  @retval EFI status if error occurred

**/
EFI_STATUS
EfiSmGetTimeStamp (
  OUT UINT32  *NumOfSeconds
  )
{
  *NumOfSeconds = 0;

  return EFI_NOT_FOUND;
}
