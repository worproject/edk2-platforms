/** @file
  Server Management Driver Lib.

Copyright (c) 2023, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _SERVER_MGMT_LIB_H_
#define _SERVER_MGMT_LIB_H_

#include <Protocol/GenericElog.h>

/**
  ELOG Library Initializer.
**/

/**
  The function will set up a notification on the ELOG protocol.  This function is required to be called prior
  to utilizing the ELOG protocol from within this library.
  @param None

  @retval EFI_SUCCESS  After the notification has been setup.
  @retval Other        Failure in constructor.

**/
EFI_STATUS
EfiInitializeGenericElog (
  VOID
  );

/**

  This function sends event log data to the destination such as LAN, ICMB, BMC etc.
  @param[in] ElogData A pointer to the event log data that needs to be recorded
  @param[in] DataType Type of Elog data that is being recorded.  The Elog is redirected based on this
               parameter.
  @param[in] AlertEvent An indication that the input data type is an alert.  The underlying
                 drivers need to decide if they need to listen to the DataType and send it on
                 an appropriate channel as an alert use of the information.
  @param[in] DataSize The size of the data to be logged
  @param[out] RecordId The array of record IDs sent by the target.  This can be used to retrieve the
               records or erase the records.

  @retval EFI_SUCCESS If the data was logged.
  @retval EFI_INVALID_PARAMETER If the DataType is >= EfiSmElogMax
  @retval EFI_OUT_OF_RESOURCES If the DataSize is larger than the Elog temp buffer and we cannot log the record
  @retval EFI_NOT_FOUND The event log target was not found
  @retval EFI_PROTOCOL_ERROR There was a data formatting error

**/
EFI_STATUS
EfiSmSetEventLogData (
  IN  UINT8             *ElogData,
  IN  EFI_SM_ELOG_TYPE  DataType,
  IN  BOOLEAN           AlertEvent,
  IN  UINTN             DataSize,
  OUT UINT64            *RecordId
  );

/**

  This function gets event log data from the destination dependent on the DataType.  The destination
  can be a remote target such as LAN, ICMB, IPMI, or a FV.  The ELOG redir driver will resolve the
  destination.
  @param[in] ElogData Pointer to the event log data buffer to contain the data to be retrieved.
  @param[in] DataType This is the type of Elog data to be gotten.  Elog is redirected based upon this
             information.
  @param[in, out] DataSize This is the size of the data to be retrieved.
  @param[in, out] RecordId The RecordId of the next record.  If ElogData is NULL, this gives the RecordId of the first
                  record available in the database with the correct DataSize.  A value of 0 on return indicates
                  that it was last record if the Status is EFI_SUCCESS.

  @retval EFI_SUCCESS If the event log was retrieved successfully.
  @retval EFI_NOT_FOUND If the event log target was not found.
  @retval EFI_NO_RESPONSE If the event log target is not responding.  This is done by the redir driver.
  @retval EFI_INVALID_PARAMETER DataType or another parameter was invalid.
  @retval EFI_BUFFER_TOO_SMALL The ElogData buffer is too small to be filled with the requested data.

**/
EFI_STATUS
EfiSmGetEventLogData (
  IN  UINT8             *ElogData,
  IN  EFI_SM_ELOG_TYPE  DataType,
  IN  OUT UINTN         *DataSize,
  IN OUT UINT64         *RecordId
  );

/**
  This function erases the event log data defined by the DataType.  The redir driver associated with
  the DataType resolves the path to the record.

  @param[in] DataType The type of Elog data that is to be erased.
  @param[in, out] RecordId The RecordId of the data to be erased.  If RecordId is NULL, all records in the
                  database are erased if permitted by the target.  RecordId will contain the deleted
                  RecordId on return.

  @retval EFI_SUCCESS The record or collection of records were erased.
  @retval EFI_NOT_FOUND The event log target was not found.
  @retval EFI_NO_RESPONSE The event log target was found but did not respond.
  @retval EFI_INVALID_PARAMETER One of the parameters was invalid.

**/
EFI_STATUS
EfiSmEraseEventlogData (
  IN EFI_SM_ELOG_TYPE  DataType,
  IN OUT UINT64        *RecordId
  );

/**

  This function enables or disables the event log defined by the DataType.
  @param[in] DataType The type of Elog data that is being activated.
  @param[in] EnableElog Enables or disables the event log defined by the DataType.  If it is NULL
             it returns the current status of the DataType log.
  @param[out] ElogStatus Is the current status of the Event log defined by the DataType.  Enabled is
              TRUE and Disabled is FALSE.

  @retval EFI_SUCCESS If the event log was successfully enabled or disabled.
  @retval EFI_NOT_FOUND The event log target was not found.
  @retval EFI_NO_RESPONSE The event log target was found but did not respond.
  @retval EFI_INVALID_PARAMETER One of the parameters was invalid.

**/
EFI_STATUS
EfiSmActivateEventLog (
  IN EFI_SM_ELOG_TYPE  DataType,
  IN BOOLEAN           *EnableElog,
  OUT BOOLEAN          *ElogStatus
  );

/**

  Return Date and Time from RTC in Unix format which fits in 32 bit format.

  @param[out] NumOfSeconds Pointer to return calculated time.

  @retval EFI_SUCCESS
  @retval Other EFI status if error occurred.

**/
EFI_STATUS
EfiSmGetTimeStamp (
  OUT UINT32  *NumOfSeconds
  );

#endif // _SERVER_MGMT_LIB_H_
