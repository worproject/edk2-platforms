/** @file
  This library abstract how to send/receive IPMI command.

Copyright (c) 2018-2021, Intel Corporation. All rights reserved.<BR>
Copyright (C) 2023 Advanced Micro Devices, Inc. All rights reserved.<BR>

SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef IPMI_COMMAND_LIB_H_
#define IPMI_COMMAND_LIB_H_

#include <Uefi.h>
#include <IndustryStandard/Ipmi.h>

///
/// Functions for IPMI NetFnApp commands
///

/**
  This function is used to retrieve device ID.

  @param [out]  DeviceId  The pointer to receive IPMI_GET_DEVICE_ID_RESPONSE.

  @retval EFI_STATUS   See the return values of IpmiSubmitCommand () function.

**/
EFI_STATUS
EFIAPI
IpmiGetDeviceId (
  OUT IPMI_GET_DEVICE_ID_RESPONSE  *DeviceId
  );

/**
  This function returns device self test results

  @param [out]  SelfTestResult  The pointer to receive IPMI_SELF_TEST_RESULT_RESPONSE.

  @retval EFI_STATUS   See the return values of IpmiSubmitCommand () function.

**/
EFI_STATUS
EFIAPI
IpmiGetSelfTestResult (
  OUT IPMI_SELF_TEST_RESULT_RESPONSE  *SelfTestResult
  );

/**
  This function is used for starting and restarting the Watchdog
  Timer from the initial countdown value that was specified in
  the Set Watchdog Timer command the watchdog timer.

  @param [out]  CompletionCode  IPMI completetion code, refer to Ipmi.h.

  @retval EFI_STATUS   See the return values of IpmiSubmitCommand () function.

**/
EFI_STATUS
EFIAPI
IpmiResetWatchdogTimer (
  OUT UINT8  *CompletionCode
  );

/**
  This function  is used for initializing and configuring
  the watchdog timer.

  @param [in]   SetWatchdogTimer  Pointer to receive IPMI_SET_WATCHDOG_TIMER_REQUEST.
  @param [out]  CompletionCode    IPMI completetion code, refer to Ipmi.h.

  @retval EFI_STATUS   See the return values of IpmiSubmitCommand () function.

**/
EFI_STATUS
EFIAPI
IpmiSetWatchdogTimer (
  IN  IPMI_SET_WATCHDOG_TIMER_REQUEST  *SetWatchdogTimer,
  OUT UINT8                            *CompletionCode
  );

/**
  This function retrieves the current settings and present
  countdown of the watchdog timer.

  @param [out]  GetWatchdogTimer  Pointer to receive IPMI_GET_WATCHDOG_TIMER_RESPONSE.

  @retval EFI_STATUS   See the return values of IpmiSubmitCommand () function.

**/
EFI_STATUS
EFIAPI
IpmiGetWatchdogTimer (
  OUT IPMI_GET_WATCHDOG_TIMER_RESPONSE  *GetWatchdogTimer
  );

/**
  This function enables message reception into Message Buffers,
  and any interrupt associated with that buffer getting full.

  @param [in]   SetBmcGlobalEnables  Pointer receive to IPMI_SET_BMC_GLOBAL_ENABLES_REQUEST.
  @param [out]  CompletionCode       IPMI completetion code, refer to Ipmi.h.

  @retval EFI_STATUS   See the return values of IpmiSubmitCommand () function.

**/
EFI_STATUS
EFIAPI
IpmiSetBmcGlobalEnables (
  IN  IPMI_SET_BMC_GLOBAL_ENABLES_REQUEST  *SetBmcGlobalEnables,
  OUT UINT8                                *CompletionCode
  );

/**
  This function retrieves the present setting of the Global Enables

  @param [out]  GetBmcGlobalEnables  Pointer to receive IPMI_GET_BMC_GLOBAL_ENABLES_RESPONSE.

  @retval EFI_STATUS   See the return values of IpmiSubmitCommand () function.

**/
EFI_STATUS
EFIAPI
IpmiGetBmcGlobalEnables (
  OUT IPMI_GET_BMC_GLOBAL_ENABLES_RESPONSE  *GetBmcGlobalEnables
  );

/**
  This function is used to flush unread data from the Receive
  Message Queue or Event Message Buffer

  @param [in]   ClearMessageFlagsRequest IPMI_CLEAR_MESSAGE_FLAGS_REQUEST
  @param [out]  CompletionCode           IPMI completetion code, refer to Ipmi.h.

  @retval EFI_STATUS   See the return values of IpmiSubmitCommand () function.

**/
EFI_STATUS
EFIAPI
IpmiClearMessageFlags (
  IN  IPMI_CLEAR_MESSAGE_FLAGS_REQUEST  *ClearMessageFlagsRequest,
  OUT UINT8                             *CompletionCode
  );

/**
  This function is used to retrieve the present message available states.

  @param [out]  GetMessageFlagsResponse  Pointer to receive IPMI_GET_MESSAGE_FLAGS_RESPONSE

  @retval EFI_STATUS   See the return values of IpmiSubmitCommand () function.

**/
EFI_STATUS
EFIAPI
IpmiGetMessageFlags (
  OUT IPMI_GET_MESSAGE_FLAGS_RESPONSE  *GetMessageFlagsResponse
  );

/**
  This function is used to get data from the Receive Message Queue.

  @param [out]      GetMessageResponse      Pointer to receive IPMI_GET_MESSAGE_RESPONSE.
  @param [in, out]  GetMessageResponseSize  When in, which is the expected size of
                                            response. When out, which is the actual
                                            size returned.

  @retval EFI_STATUS   See the return values of IpmiSubmitCommand () function.

**/
EFI_STATUS
EFIAPI
IpmiGetMessage (
  OUT IPMI_GET_MESSAGE_RESPONSE  *GetMessageResponse,
  IN OUT UINT32                  *GetMessageResponseSize
  );

/**
  This function is used for bridging IPMI messages between channels,
  and between the system management software (SMS) and a given channel

  @param [in]   SendMessageRequest       Pointer to IPMI_SEND_MESSAGE_REQUEST.
  @param [in]   SendMessageRequestSize   Size of entire SendMessageRequestSize.
  @param [out]  SendMessageResponse      Pointer to receive IPMI_SEND_MESSAGE_RESPONSE.
  @param [in]   SendMessageResponseSize  When in, which is the expected size of
                                         response. When out, which is the actual
                                         size returned.

  @retval EFI_STATUS   See the return values of IpmiSubmitCommand () function.

**/
EFI_STATUS
EFIAPI
IpmiSendMessage (
  IN  IPMI_SEND_MESSAGE_REQUEST   *SendMessageRequest,
  IN  UINT32                      SendMessageRequestSize,
  OUT IPMI_SEND_MESSAGE_RESPONSE  *SendMessageResponse,
  IN OUT UINT32                   *SendMessageResponseSize
  );

/**
  This function gets the system UUID.

  @param [out]  SystemGuid   The pointer to retrieve system UUID.

  @retval EFI_SUCCESS               UUID is returned.
  @retval EFI_INVALID_PARAMETER     SystemGuid is a NULL pointer.
  @retval Others                    See return value of IpmiSubmitCommand () function.
**/
EFI_STATUS
EFIAPI
IpmiGetSystemUuid (
  OUT EFI_GUID  *SystemGuid
  );

/**
  This function gets the channel information.

  @param [in]   GetChannelInfoRequest       The get channel information request.
  @param [out]  GetChannelInfoResponse      The get channel information response.
  @param [out]  GetChannelInfoResponseSize  When input, the expected size of response.
                                            When output, the exact size of the returned
                                            response.

  @retval EFI_SUCCESS            Get channel information successfully.
  @retval EFI_INVALID_PARAMETER  One of the given input parameters is invalid.
  @retval Others                 See return value of IpmiSubmitCommand () function.

**/
EFI_STATUS
EFIAPI
IpmiGetChannelInfo (
  IN  IPMI_GET_CHANNEL_INFO_REQUEST   *GetChannelInfoRequest,
  OUT IPMI_GET_CHANNEL_INFO_RESPONSE  *GetChannelInfoResponse,
  OUT UINT32                          *GetChannelInfoResponseSize
  );

///
/// Functions for IPMI NetFnTransport commands.
///

/**
  This function sends command to BMC to notify a remote application
  that a SOL payload is activating on another channel.

  @param [in]   SolActivatingRequest  Pointer to IPMI_SOL_ACTIVATING_REQUEST.
  @param [out]  CompletionCode        IPMI completetion code, refer to Ipmi.h.

  @retval EFI_STATUS   See the return values of IpmiSubmitCommand () function.

**/
EFI_STATUS
EFIAPI
IpmiSolActivating (
  IN  IPMI_SOL_ACTIVATING_REQUEST  *SolActivatingRequest,
  OUT UINT8                        *CompletionCode
  );

/**
  This function is used to set parameters such as the network addressing
  information required for SOL payload operation.

  @param [in]  SetConfigurationParametersRequest      Pointer to IPMI_SET_SOL_CONFIGURATION_PARAMETERS_REQUEST.
  @param [in]  SetConfigurationParametersRequestSize  Size of entire SetConfigurationParametersRequestSize.
  @param [out] CompletionCode                         IPMI completetion code, refer to Ipmi.h.

  @retval EFI_STATUS   See the return values of IpmiSubmitCommand () function.

**/
EFI_STATUS
EFIAPI
IpmiSetSolConfigurationParameters (
  IN  IPMI_SET_SOL_CONFIGURATION_PARAMETERS_REQUEST  *SetConfigurationParametersRequest,
  IN  UINT32                                         SetConfigurationParametersRequestSize,
  OUT UINT8                                          *CompletionCode
  );

/**
  This function is used to retrieve the configuration parameters from the
  Set SOL Configuration Parameters.

  @param [in]       GetConfigurationParametersRequest       Pointer to IPMI_SET_SOL_CONFIGURATION_PARAMETERS_REQUEST.
  @param [out]      GetConfigurationParametersResponse      Pointer to receive IPMI_GET_SOL_CONFIGURATION_PARAMETERS_RESPONSE.
  @param [in, out]  GetConfigurationParametersResponseSize  When in, which is the expected size of
                                                            response. When out, which is the actual
                                                            size returned.

  @retval EFI_STATUS   See the return values of IpmiSubmitCommand () function.

**/
EFI_STATUS
EFIAPI
IpmiGetSolConfigurationParameters (
  IN  IPMI_GET_SOL_CONFIGURATION_PARAMETERS_REQUEST   *GetConfigurationParametersRequest,
  OUT IPMI_GET_SOL_CONFIGURATION_PARAMETERS_RESPONSE  *GetConfigurationParametersResponse,
  IN OUT UINT32                                       *GetConfigurationParametersResponseSize
  );

/**
  This function gets the LAN configuration parameter.

  @param[in]      GetLanConfigurationParametersRequest   Request data
  @param[out]     GetLanConfigurationParametersResponse  Response data
  @param[in,out]  GetLanConfigurationParametersSize      When input, the expected size of response data.
                                                         When out, the exact size of response data.

  @retval EFI_SUCCESS            Lan configuration parameter is returned in the response.
  @retval EFI_INVALID_PARAMETER  One of the given input parameters is invalid.
  @retval Others                 Other errors.

**/
EFI_STATUS
EFIAPI
IpmiGetLanConfigurationParameters (
  IN     IPMI_GET_LAN_CONFIGURATION_PARAMETERS_REQUEST   *GetLanConfigurationParametersRequest,
  OUT    IPMI_GET_LAN_CONFIGURATION_PARAMETERS_RESPONSE  *GetLanConfigurationParametersResponse,
  IN OUT UINT32                                          *GetLanConfigurationParametersSize
  );

///
/// Functions for IPMI NetFnChasis commands
///

/**
  This function returns information about which main chassis management functions are
  present and  what addresses are used to access those functions.

  @param [out]  GetChassisCapabilitiesResponse  Pointer to IPMI_GET_CHASSIS_CAPABILITIES_RESPONSE.

  @retval EFI_STATUS   See the return values of IpmiSubmitCommand () function.

**/
EFI_STATUS
EFIAPI
IpmiGetChassisCapabilities (
  OUT IPMI_GET_CHASSIS_CAPABILITIES_RESPONSE  *GetChassisCapabilitiesResponse
  );

/**
  This function gets  information regarding the high-level status of the system
  chassis and main power subsystem.

  @param [out]  GetChassisStatusResponse  Pointer to IPMI_GET_CHASSIS_STATUS_RESPONSE.

  @retval EFI_STATUS   See the return values of IpmiSubmitCommand () function.

**/
EFI_STATUS
EFIAPI
IpmiGetChassisStatus (
  OUT IPMI_GET_CHASSIS_STATUS_RESPONSE  *GetChassisStatusResponse
  );

/**
  This function sends command to control power up, power down, and reset.

  @param [in]   ChassisControlRequest  Pointer to IPMI_CHASSIS_CONTROL_REQUEST.
  @param [out]  CompletionCode         IPMI completetion code, refer to Ipmi.h.

  @retval EFI_STATUS   See the return values of IpmiSubmitCommand () function.

**/
EFI_STATUS
EFIAPI
IpmiChassisControl (
  IN IPMI_CHASSIS_CONTROL_REQUEST  *ChassisControlRequest,
  OUT UINT8                        *CompletionCode
  );

/**
  This function is used to configure the power restore policy.

  @param [in]   ChassisControlRequest   Pointer to IPMI_SET_POWER_RESTORE_POLICY_REQUEST.
  @param [out]  ChassisControlResponse  Pointer to IPMI_SET_POWER_RESTORE_POLICY_RESPONSE.

  @retval EFI_STATUS   See the return values of IpmiSubmitCommand () function.

**/
EFI_STATUS
EFIAPI
IpmiSetPowerRestorePolicy (
  IN  IPMI_SET_POWER_RESTORE_POLICY_REQUEST   *ChassisControlRequest,
  OUT IPMI_SET_POWER_RESTORE_POLICY_RESPONSE  *ChassisControlResponse
  );

/**
  This function is used to set parameters that direct the system boot
  following a system power up or reset.

  @param [in]   BootOptionsRequest   Pointer to IPMI_SET_BOOT_OPTIONS_REQUEST.
  @param [out]  BootOptionsResponse  Pointer to IPMI_SET_BOOT_OPTIONS_RESPONSE.

  @retval EFI_STATUS   See the return values of IpmiSubmitCommand () function.

**/
EFI_STATUS
EFIAPI
IpmiSetSystemBootOptions (
  IN  IPMI_SET_BOOT_OPTIONS_REQUEST   *BootOptionsRequest,
  OUT IPMI_SET_BOOT_OPTIONS_RESPONSE  *BootOptionsResponse
  );

/**
  This function is used to retrieve the boot options set by the
  Set System Boot Options command.

  @param [in]   BootOptionsRequest   Pointer to IPMI_GET_BOOT_OPTIONS_REQUEST.
  @param [out]  BootOptionsResponse  Pointer to IPMI_GET_BOOT_OPTIONS_RESPONSE.

  @retval EFI_STATUS   See the return values of IpmiSubmitCommand () function.

**/
EFI_STATUS
EFIAPI
IpmiGetSystemBootOptions (
  IN  IPMI_GET_BOOT_OPTIONS_REQUEST   *BootOptionsRequest,
  OUT IPMI_GET_BOOT_OPTIONS_RESPONSE  *BootOptionsResponse
  );

///
/// Functions for IPMI NetFnStorage commands
///

/**
  This function is used to retrieve FRU Inventory Area

  @param [in]   GetFruInventoryAreaInfoRequest   Pointer to IPMI_GET_FRU_INVENTORY_AREA_INFO_REQUEST.
  @param [out]  GetFruInventoryAreaInfoResponse  Pointer to IPMI_GET_FRU_INVENTORY_AREA_INFO_RESPONSE.

  @retval EFI_STATUS   See the return values of IpmiSubmitCommand () function.

**/
EFI_STATUS
EFIAPI
IpmiGetFruInventoryAreaInfo (
  IN  IPMI_GET_FRU_INVENTORY_AREA_INFO_REQUEST   *GetFruInventoryAreaInfoRequest,
  OUT IPMI_GET_FRU_INVENTORY_AREA_INFO_RESPONSE  *GetFruInventoryAreaInfoResponse
  );

/**
  This function returns specified data from the FRU Inventory Info area.

  @param [in]       ReadFruDataRequest       Pointer to IPMI_READ_FRU_DATA_REQUEST.
  @param [out]      ReadFruDataResponse      Pointer to IPMI_READ_FRU_DATA_RESPONSE.
  @param [in, out]  ReadFruDataResponseSize  Returns the size of ReadFruDataResponse.

  @retval EFI_STATUS   See the return values of IpmiSubmitCommand () function.

**/
EFI_STATUS
EFIAPI
IpmiReadFruData (
  IN  IPMI_READ_FRU_DATA_REQUEST   *ReadFruDataRequest,
  OUT IPMI_READ_FRU_DATA_RESPONSE  *ReadFruDataResponse,
  IN OUT UINT32                    *ReadFruDataResponseSize
  );

/**
  This function writes specified data from the FRU Inventory Info area.

  @param [in]   WriteFruDataRequest      Pointer to IPMI_WRITE_FRU_DATA_REQUEST.
  @param [in]   WriteFruDataRequestSize  Size of WriteFruDataRequest.
  @param [out]  WriteFruDataResponse     Pointer to receive IPMI_WRITE_FRU_DATA_RESPONSE.

  @retval EFI_STATUS   See the return values of IpmiSubmitCommand () function.

**/
EFI_STATUS
EFIAPI
IpmiWriteFruData (
  IN  IPMI_WRITE_FRU_DATA_REQUEST   *WriteFruDataRequest,
  IN  UINT32                        WriteFruDataRequestSize,
  OUT IPMI_WRITE_FRU_DATA_RESPONSE  *WriteFruDataResponse
  );

/**
  This function returns the number of entries in the SEL

  @param [out] GetSelInfoResponse     Pointer to receive IPMI_GET_SEL_INFO_RESPONSE.

  @retval EFI_STATUS   See the return values of IpmiSubmitCommand () function.

**/
EFI_STATUS
EFIAPI
IpmiGetSelInfo (
  OUT IPMI_GET_SEL_INFO_RESPONSE  *GetSelInfoResponse
  );

/**
  This function retrieves entries from the SEL

  @param [in]   GetSelEntryRequest       Pointer to IPMI_GET_SEL_ENTRY_REQUEST.
  @param [out]  GetSelEntryResponse      Pointer to receive IPMI_GET_SEL_ENTRY_RESPONSE.
  @param [in]   GetSelEntryResponseSize  Size of entire GetSelEntryResponse.

  @retval EFI_STATUS   See the return values of IpmiSubmitCommand () function.

**/
EFI_STATUS
EFIAPI
IpmiGetSelEntry (
  IN IPMI_GET_SEL_ENTRY_REQUEST    *GetSelEntryRequest,
  OUT IPMI_GET_SEL_ENTRY_RESPONSE  *GetSelEntryResponse,
  IN OUT UINT32                    *GetSelEntryResponseSize
  );

/**
  This function adds an entry in the SEL

  @param [in]   AddSelEntryRequest   Pointer to IPMI_ADD_SEL_ENTRY_REQUEST.
  @param [out]  AddSelEntryResponse  Pointer to receive IPMI_ADD_SEL_ENTRY_RESPONSE.

  @retval EFI_STATUS   See the return values of IpmiSubmitCommand () function.

**/
EFI_STATUS
EFIAPI
IpmiAddSelEntry (
  IN IPMI_ADD_SEL_ENTRY_REQUEST    *AddSelEntryRequest,
  OUT IPMI_ADD_SEL_ENTRY_RESPONSE  *AddSelEntryResponse
  );

/**
  This function adds SEL Entry command that allows the record to be incrementally
  added to the SEL.

  @param [in]  PartialAddSelEntryRequest      Pointer to IPMI_PARTIAL_ADD_SEL_ENTRY_REQUEST.
  @param [in]  PartialAddSelEntryRequestSize  Size of entire PartialAddSelEntryRequest.
  @param [out] PartialAddSelEntryResponse     Pointer to receive IPMI_PARTIAL_ADD_SEL_ENTRY_RESPONSE.

  @retval EFI_STATUS   See return value of IpmiSubmitCommand () function.

**/
EFI_STATUS
EFIAPI
IpmiPartialAddSelEntry (
  IN IPMI_PARTIAL_ADD_SEL_ENTRY_REQUEST    *PartialAddSelEntryRequest,
  IN UINT32                                PartialAddSelEntryRequestSize,
  OUT IPMI_PARTIAL_ADD_SEL_ENTRY_RESPONSE  *PartialAddSelEntryResponse
  );

/**
  This function erases all contents of the System Event Log.

  @param [in]   ClearSelRequest   Pointer to IPMI_CLEAR_SEL_REQUEST.
  @param [out]  ClearSelResponse  Pointer to receive IPMI_CLEAR_SEL_RESPONSE.

  @retval EFI_STATUS   See the return values of IpmiSubmitCommand () function.

**/
EFI_STATUS
EFIAPI
IpmiClearSel (
  IN IPMI_CLEAR_SEL_REQUEST    *ClearSelRequest,
  OUT IPMI_CLEAR_SEL_RESPONSE  *ClearSelResponse
  );

/**
  This function returns the time from the SEL Device.

  @param [out]  GetSelTimeResponse  Pointer to IPMI_GET_SEL_TIME_RESPONSE.

  @retval EFI_STATUS   See the return values of IpmiSubmitCommand () function.

**/
EFI_STATUS
EFIAPI
IpmiGetSelTime (
  OUT IPMI_GET_SEL_TIME_RESPONSE  *GetSelTimeResponse
  );

/**
  This function set the time in the SEL Device.

  @param [in]   SetSelTimeRequest  Pointer to IPMI_SET_SEL_TIME_REQUEST.
  @param [out]  CompletionCode     IPMI completetion code, refer to Ipmi.h.

  @retval EFI_STATUS   See the return values of IpmiSubmitCommand () function.

**/
EFI_STATUS
EFIAPI
IpmiSetSelTime (
  IN IPMI_SET_SEL_TIME_REQUEST  *SetSelTimeRequest,
  OUT UINT8                     *CompletionCode
  );

/**
  This function returns the SDR command version for the SDR Repository.

  @param [out]  ClearSelResponse  Pointer to receive IPMI_GET_SDR_REPOSITORY_INFO_RESPONSE.

  @retval EFI_STATUS   See the return values of IpmiSubmitCommand () function.

**/
EFI_STATUS
EFIAPI
IpmiGetSdrRepositoryInfo (
  OUT IPMI_GET_SDR_REPOSITORY_INFO_RESPONSE  *GetSdrRepositoryInfoResp
  );

/**
  This function returns the sensor record specified by Record ID.

  @param [in]       GetSdrRequest       Pointer to IPMI_GET_SDR_REQUEST.
  @param [out]      GetSdrResponse      Pointer to receive IPMI_GET_SDR_RESPONSE.
  @param [in, out]  GetSdrResponseSize  Size of entire GetSdrResponse.

  @retval EFI_STATUS   See the return values of IpmiSubmitCommand () function.

**/
EFI_STATUS
EFIAPI
IpmiGetSdr (
  IN  IPMI_GET_SDR_REQUEST   *GetSdrRequest,
  OUT IPMI_GET_SDR_RESPONSE  *GetSdrResponse,
  IN OUT UINT32              *GetSdrResponseSize
  );

#endif // IPMI_COMMAND_LIB_H_
