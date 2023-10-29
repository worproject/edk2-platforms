/** @file
  BMC Event Log Common functions.

Copyright (c) 2023, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "BmcElogCommon.h"

/**
  WaitTillClearSel.

  @param ResvId              - Reserved ID

  @retval EFI_SUCCESS
  @retval EFI_NO_RESPONSE

**/
EFI_STATUS
WaitTillClearSel (
  UINT8  *ResvId
  )
{
  INTN                     Counter;
  UINT32                   ResponseDataSize;
  IPMI_CLEAR_SEL_REQUEST   ClearSelRequest;
  IPMI_CLEAR_SEL_RESPONSE  ClearSelResponse;
  EFI_STATUS               Status;

  Counter = 0x200;
  Status  = EFI_SUCCESS;
  while (TRUE) {
    ClearSelRequest.Reserve[0] = ResvId[0];
    ClearSelRequest.Reserve[1] = ResvId[1];
    ClearSelRequest.AscC       = IPMI_CLEAR_SEL_REQUEST_C_CHAR_ASCII;
    ClearSelRequest.AscL       = IPMI_CLEAR_SEL_REQUEST_L_CHAR_ASCII;
    ClearSelRequest.AscR       = IPMI_CLEAR_SEL_REQUEST_R_CHAR_ASCII;
    ClearSelRequest.Erase      = IPMI_CLEAR_SEL_REQUEST_GET_ERASE_STATUS;
    ResponseDataSize           = sizeof (ClearSelResponse);

    Status = IpmiSubmitCommand (
               IPMI_NETFN_STORAGE,
               IPMI_STORAGE_CLEAR_SEL,
               (UINT8 *)&ClearSelRequest,
               sizeof (ClearSelRequest),
               (UINT8 *)&ClearSelResponse,
               &ResponseDataSize
               );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "IpmiSubmitCommand ClearSelRequest Failed %r\n", Status));
    }

    if ((ClearSelResponse.ErasureProgress & 0xf) == IPMI_CLEAR_SEL_RESPONSE_ERASURE_COMPLETED) {
      return EFI_SUCCESS;
    }

    //
    //  If there is not a response from the BMC controller we need to return and not hang.
    //
    --Counter;
    if (Counter == 0x0) {
      return EFI_NO_RESPONSE;
    }
  }
}

/**
  Set Bmc Elog Data.


  @param ElogData    - Buffer for log storage
  @param DataType    - Event Log type
  @param AlertEvent  - If it is an alert event
  @param Size        - Log data size
  @param RecordId    - Indicate which recorder it is

  @retval EFI_STATUS

**/
EFI_STATUS
SetBmcElogRecord (
  IN  UINT8             *ElogData,
  IN  EFI_SM_ELOG_TYPE  DataType,
  IN  BOOLEAN           AlertEvent,
  IN  UINTN             Size,
  OUT UINT64            *RecordId
  )
{
  EFI_STATUS                                Status;
  UINT32                                    ResponseDataSize;
  UINT8                                     EvntMsgResponse;
  IPMI_PLATFORM_EVENT_MESSAGE_DATA_REQUEST  EvntMsgRequest;
  IPMI_ADD_SEL_ENTRY_REQUEST                AddSelRequest;
  IPMI_ADD_SEL_ENTRY_RESPONSE               AddSelResponse;

  Status = EFI_SUCCESS;
  CopyMem (&AddSelRequest.RecordData, ElogData, Size);

  *RecordId = 0;

  if (AlertEvent) {
    EvntMsgRequest.GeneratorId  = (UINT8)AddSelRequest.RecordData.GeneratorId;
    EvntMsgRequest.EvMRevision  = AddSelRequest.RecordData.EvMRevision;
    EvntMsgRequest.SensorType   = AddSelRequest.RecordData.SensorType;
    EvntMsgRequest.SensorNumber = AddSelRequest.RecordData.SensorNumber;
    EvntMsgRequest.EventDirType = AddSelRequest.RecordData.EventDirType;
    EvntMsgRequest.OEMEvData1   = AddSelRequest.RecordData.OEMEvData1;
    EvntMsgRequest.OEMEvData2   = AddSelRequest.RecordData.OEMEvData2;
    EvntMsgRequest.OEMEvData3   = AddSelRequest.RecordData.OEMEvData3;

    ResponseDataSize = sizeof (EvntMsgResponse);
    Status           = IpmiSubmitCommand (
                         IPMI_NETFN_SENSOR_EVENT,
                         IPMI_SENSOR_PLATFORM_EVENT_MESSAGE,
                         (UINT8 *)&EvntMsgRequest,
                         sizeof (EvntMsgRequest),
                         (UINT8 *)&EvntMsgResponse,
                         &ResponseDataSize
                         );
  } else {
    ResponseDataSize = sizeof (AddSelResponse);
    Status           = IpmiSubmitCommand (
                         IPMI_NETFN_STORAGE,
                         IPMI_STORAGE_ADD_SEL_ENTRY,
                         (UINT8 *)&AddSelRequest,
                         sizeof (AddSelRequest),
                         (UINT8 *)&AddSelResponse,
                         &ResponseDataSize
                         );

    if (Status == EFI_SUCCESS) {
      *RecordId = AddSelResponse.RecordId;
    }
  }

  return Status;
}

/**
  Get Bmc Elog Data.

  @param ElogData    - Buffer for log data store
  @param DataType    - Event log type
  @param Size        - Size of log data
  @param RecordId    - indicate which recorder it is

  @retval EFI_STATUS

**/
EFI_STATUS
GetBmcElogRecord (
  IN UINT8             *ElogData,
  IN EFI_SM_ELOG_TYPE  DataType,
  IN OUT UINTN         *Size,
  IN OUT UINT64        *RecordId
  )
{
  UINT64                       ReceiveKey;
  EFI_STATUS                   Status;
  IPMI_GET_SEL_ENTRY_REQUEST   GetSelEntryRequest;
  IPMI_GET_SEL_ENTRY_RESPONSE  GetSelEntryResponse;
  UINT32                       ResponseDataSize;

  Status                          = EFI_SUCCESS;
  ReceiveKey                      = *RecordId;
  GetSelEntryRequest.ReserveId[0] = 0;
  GetSelEntryRequest.ReserveId[1] = 0;
  GetSelEntryRequest.SelRecID[0]  = (UINT8)ReceiveKey;
  ReceiveKey                      = DivU64x32 (ReceiveKey, (UINT32)(1 << 8));
  GetSelEntryRequest.SelRecID[1]  = (UINT8)ReceiveKey;
  GetSelEntryRequest.Offset       = 0;
  GetSelEntryRequest.BytesToRead  = IPMI_COMPLETE_SEL_RECORD;
  ResponseDataSize                = sizeof (GetSelEntryResponse);

  Status = IpmiSubmitCommand (
             IPMI_NETFN_STORAGE,
             IPMI_STORAGE_GET_SEL_ENTRY,
             (UINT8 *)&GetSelEntryRequest,
             sizeof (GetSelEntryRequest),
             (UINT8 *)&GetSelEntryResponse,
             &ResponseDataSize
             );
  //
  // Per IPMI spec, Entire Record is 16 Bytes
  // If less than 16 bytes pointer is sent return buffer too small
  //
  if (Status == EFI_SUCCESS) {
    if (*Size < (SEL_RECORD_SIZE)) {
      return EFI_BUFFER_TOO_SMALL;
    }

    if (GetSelEntryResponse.NextSelRecordId == 0xFFFF) {
      return EFI_NOT_FOUND;
    }

    *RecordId = GetSelEntryResponse.NextSelRecordId;
    CopyMem (ElogData, &GetSelEntryResponse.RecordData, sizeof (GetSelEntryResponse.RecordData));
    *Size = SEL_RECORD_SIZE;
  }

  return Status;
}

/**
  Erase Bmc Elog Data.

  @param DataType    - Event log type
  @param RecordId    - return which recorder it is

  @retval EFI_STATUS

**/
EFI_STATUS
EraseBmcElogRecord (
  IN EFI_SM_ELOG_TYPE  DataType,
  IN OUT UINT64        *RecordId
  )
{
  EFI_STATUS                      Status;
  UINT64                          ReceiveKey;
  UINT8                           ResvId[2];
  BOOLEAN                         SelReserveIdIsSupported;
  UINT8                           OperationSupport;
  UINT8                           SelReserveIdvalue;
  UINT32                          ResponseDataSize;
  IPMI_GET_SEL_INFO_RESPONSE      GetSelInfoResponse;
  IPMI_RESERVE_SEL_RESPONSE       ReserveSelResponse;
  IPMI_DELETE_SEL_ENTRY_REQUEST   DeleteSelRequest;
  IPMI_DELETE_SEL_ENTRY_RESPONSE  DeleteSelResponse;
  IPMI_CLEAR_SEL_REQUEST          ClearSelRequest;
  IPMI_CLEAR_SEL_RESPONSE         ClearSelResponse;

  Status = EFI_SUCCESS;

  //
  // Before issuing this SEL reservation ID, Check whether this command is supported or not by issuing the
  // GetSelInfoCommand. If it does not support ResvId should be 0000h
  //
  ResponseDataSize = sizeof (GetSelInfoResponse);
  Status           = IpmiSubmitCommand (
                       IPMI_NETFN_STORAGE,
                       IPMI_STORAGE_GET_SEL_INFO,
                       NULL,
                       0,
                       (UINT8 *)&GetSelInfoResponse,
                       &ResponseDataSize
                       );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  OperationSupport  = GetSelInfoResponse.OperationSupport;
  SelReserveIdvalue = (OperationSupport & IPMI_GET_SEL_INFO_OPERATION_SUPPORT_RESERVE_SEL_CMD);
  if (SelReserveIdvalue == IPMI_GET_SEL_INFO_OPERATION_SUPPORT_RESERVE_SEL_CMD) {
    SelReserveIdIsSupported = TRUE;
  } else {
    SelReserveIdIsSupported = FALSE;
  }

  //
  // if SelReserveIdcommand not supported do not issue the RESERVE_SEL_ENTRY command, and set the ResvId value to 0000h
  //

  //
  // Get the SEL reservation ID
  //

  if (SelReserveIdIsSupported) {
    ResponseDataSize = sizeof (ReserveSelResponse);
    Status           = IpmiSubmitCommand (
                         IPMI_NETFN_STORAGE,
                         IPMI_STORAGE_RESERVE_SEL,
                         NULL,
                         0,
                         (UINT8 *)&ReserveSelResponse,
                         &ResponseDataSize
                         );

    if (EFI_ERROR (Status)) {
      return Status;
    }

    ResvId[0] = ReserveSelResponse.ReservationId[0];
    ResvId[1] = ReserveSelResponse.ReservationId[1];
  } else {
    ResvId[0] = 0x00;
    ResvId[1] = 0x00;
  }

  //
  // Clear the SEL
  //
  if (RecordId != NULL) {
    ReceiveKey                         = *RecordId;
    DeleteSelRequest.ReserveId[0]      = ResvId[0];
    DeleteSelRequest.ReserveId[1]      = ResvId[1];
    DeleteSelRequest.RecordToDelete[0] = (UINT8)ReceiveKey;
    ReceiveKey                         = DivU64x32 (ReceiveKey, (UINT32)(1 << 8));
    DeleteSelRequest.RecordToDelete[1] = (UINT8)ReceiveKey;

    ResponseDataSize = sizeof (DeleteSelResponse);
    Status           = IpmiSubmitCommand (
                         IPMI_NETFN_STORAGE,
                         IPMI_STORAGE_DELETE_SEL_ENTRY,
                         (UINT8 *)&DeleteSelRequest,
                         sizeof (DeleteSelRequest),
                         (UINT8 *)&DeleteSelResponse,
                         &ResponseDataSize
                         );
  } else {
    ClearSelRequest.Reserve[0] = ResvId[0];
    ClearSelRequest.Reserve[1] = ResvId[1];
    ClearSelRequest.AscC       = IPMI_CLEAR_SEL_REQUEST_C_CHAR_ASCII;
    ClearSelRequest.AscL       = IPMI_CLEAR_SEL_REQUEST_L_CHAR_ASCII;
    ClearSelRequest.AscR       = IPMI_CLEAR_SEL_REQUEST_R_CHAR_ASCII;
    ClearSelRequest.Erase      = IPMI_CLEAR_SEL_REQUEST_INITIALIZE_ERASE;
    ResponseDataSize           = sizeof (ClearSelResponse);

    Status = IpmiSubmitCommand (
               IPMI_NETFN_STORAGE,
               IPMI_STORAGE_CLEAR_SEL,
               (UINT8 *)&ClearSelRequest,
               sizeof (ClearSelRequest),
               (UINT8 *)&ClearSelResponse,
               &ResponseDataSize
               );
  }

  if (Status == EFI_SUCCESS) {
    if (RecordId == NULL) {
      WaitTillClearSel (ResvId);
    }
  }

  return Status;
}

/**
  Activate Bmc Elog.

  @param DataType    - indicate event log type
  @param EnableElog  - Enable/Disable event log
  @param ElogStatus  - return log status

  @retval EFI_STATUS

**/
EFI_STATUS
ActivateBmcElog (
  IN  EFI_SM_ELOG_TYPE  DataType,
  IN  BOOLEAN           *EnableElog,
  OUT BOOLEAN           *ElogStatus
  )
{
  EFI_STATUS                            Status;
  UINT8                                 ElogStat;
  IPMI_GET_BMC_GLOBAL_ENABLES_RESPONSE  GetBmcGlobalResponse;
  IPMI_SET_BMC_GLOBAL_ENABLES_REQUEST   SetBmcGlobalRequest;
  UINT8                                 SetBmcGlobalResponse;
  UINT32                                ResponseDataSize;

  Status   = EFI_SUCCESS;
  ElogStat = 0;

  ResponseDataSize = sizeof (GetBmcGlobalResponse);
  Status           = IpmiSubmitCommand (
                       IPMI_NETFN_APP,
                       IPMI_APP_GET_BMC_GLOBAL_ENABLES,
                       NULL,
                       0,
                       (UINT8 *)&GetBmcGlobalResponse,
                       &ResponseDataSize
                       );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "IpmiSubmitCommand App Get Bmc Global Enables Failed %r\n", Status));
  }

  if (EnableElog == NULL) {
    *ElogStatus = (UINT8)(GetBmcGlobalResponse.GetEnables.Bits.SystemEventLogging);
  } else {
    if (Status == EFI_SUCCESS) {
      if (*EnableElog) {
        ElogStat = 0x1; // Setting SystemEventLogging
      }

      SetBmcGlobalRequest.SetEnables.Uint8                   = GetBmcGlobalResponse.GetEnables.Uint8;
      SetBmcGlobalRequest.SetEnables.Bits.SystemEventLogging = ElogStat;

      ResponseDataSize = sizeof (SetBmcGlobalResponse);
      Status           = IpmiSubmitCommand (
                           IPMI_NETFN_APP,
                           IPMI_APP_SET_BMC_GLOBAL_ENABLES,
                           (UINT8 *)&SetBmcGlobalRequest,
                           sizeof (SetBmcGlobalRequest),
                           (UINT8 *)&SetBmcGlobalResponse,
                           &ResponseDataSize
                           );

      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "IpmiSubmitCommand App Set Bmc Global Enables Failed %r\n", Status));
      }
    }
  }

  return Status;
}

/**
  This function checks the BMC SEL is full and whether to report the error.

  @retval EFI_SUCCESS
  @retval EFI_DEVICE_ERROR

**/
EFI_STATUS
EFIAPI
CheckIfSelIsFull (
  VOID
  )
{
  EFI_STATUS                  Status;
  UINT32                      ResponseDataSize;
  UINT8                       OperationSupportByte;
  UINT8                       SelIsFull;
  IPMI_GET_SEL_INFO_RESPONSE  GetSelInfoResponse;

  OperationSupportByte = 0;
  SelIsFull            = 0;

  ResponseDataSize = sizeof (GetSelInfoResponse);

  Status = IpmiSubmitCommand (
             IPMI_NETFN_STORAGE,
             IPMI_STORAGE_GET_SEL_INFO,
             NULL,
             0,
             (UINT8 *)&GetSelInfoResponse,
             &ResponseDataSize
             );

  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  OperationSupportByte = GetSelInfoResponse.OperationSupport;

  //
  // Check the Bit7 of the OperationByte if SEL is OverFlow.
  //
  SelIsFull = (OperationSupportByte & IPMI_GET_SEL_INFO_OPERATION_SUPPORT_OVERFLOW_FLAG);

  if (SelIsFull == IPMI_GET_SEL_INFO_OPERATION_SUPPORT_OVERFLOW_FLAG) {
    //
    // Report the Error code that SEL Log is full
    //
    ReportStatusCode (
      (EFI_ERROR_CODE | EFI_ERROR_MINOR),
      (SOFTWARE_EFI_BMC | EFI_SW_EC_EVENT_LOG_FULL)
      );
  }

  return EFI_SUCCESS;
}
