/** @file IpmiNetFnAppDefinitions.h
  Ipmi NetFn Application additional commands and its structures.

  @copyright
  Copyright 2016 - 2021 Intel Corporation. <BR>
  Copyright (c) 1985 - 2023, American Megatrends International LLC. <BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _IPMI_NETFN_APP_DEFINITIONS_H_
#define _IPMI_NETFN_APP_DEFINITIONS_H_

#include <IndustryStandard/IpmiNetFnApp.h>

#define IPMI_SPEC_VERSION_1_5       0x51
#define IPMI_SPEC_VERSION_2_0       0x02
#define IPMI_APP_SELFTEST_RESERVED  0xFF

#pragma pack(1)

/**
  Get Bmc global enables command response.
*/
typedef struct {
  /// Completion code.
  UINT8    CompletionCode;
  UINT8    ReceiveMsgQueueInterrupt    : 1; ///< Receive Message Queue Interrupt.
  UINT8    EventMsgBufferFullInterrupt : 1; ///< Event Message Buffer Full Interrupt.
  UINT8    EventMsgBuffer              : 1; ///< Event Message Buffer.
  UINT8    SystemEventLogging          : 1; ///< System Event Logging.
  UINT8    Reserved                    : 1; ///< Reserved.
  UINT8    OEM0                        : 1; ///< OEM0 interrupt.
  UINT8    OEM1                        : 1; ///< OEM1 interrupt.
  UINT8    OEM2                        : 1; ///< OEM2 interrupt.
} GET_BMC_GLOBAL_ENABLES_RESPONSE;

/**
  Channel access type.
 */
typedef enum {
  ChannelAccessTypeReserved0,              ///< Reserved0
  ChannelAccessTypeNonVolatile,            ///< NonVolatile
  ChannelAccessTypePresentVolatileSetting, ///< PresentVolatileSetting
  ChannelAccessTypeReserved1               ///< Reserved1
} CHANNEL_ACCESS_TYPE;

/**
  Channel access modes.
*/
typedef enum {
  ChannelAccessModeDisabled,          ///< Disabled Channel Access Mode.
  ChannelAccessModePreBootOnly,       ///< Pre-Boot Only Channel Access Mode.
  ChannelAccessModeAlwaysAvailable,   ///< Always Available Channel Access Mode.
  ChannelAccessModeShared             ///< Shared Channel Access Mode.
} CHANNEL_ACCESS_MODES;

/**
  SSIF read/write support.
*/
typedef enum {
  SsifSinglePartRw,           ///< Single Part read-write.
  SsifMultiPartRw,            ///< Multi Part read-write.
  SsifMultiPartRwWithMiddle,  ///< Multi Part read-write With Middle.
  SsifReserved                ///< Reserved.
} SSIF_READ_WRITE_SUPPORT;

/**
  Channnel states.
*/
typedef enum {
  DisbleChannel = 0,   ///< Disble Channel.
  EnableChannel,       ///< Enable Channel.
  GetChannelState,     ///< Get Channel State.
  ChannelStateReserved ///< Channel State Reserved.
} CHANNEL_STATE;

/**
  Enable message channel command request structure.
*/
typedef struct {
  UINT8    ChannelNumber : 4;         /// Channel Number.
  UINT8    Reserved1     : 4;         ///< Reserved.
  UINT8    ChannelState  : 2;         ///< Channel State.
  UINT8    Reserved2     : 6;         ///< Reserved.
} IPMI_ENABLE_MESSAGE_CHANNEL_REQUEST;

/**
  Enable message channel command response structure.
*/
typedef struct {
  UINT8    CompletionCode;            /// Completion code.
  UINT8    ChannelNumber : 4;         ///< Channel Number.
  UINT8    Reserved1     : 4;         ///< Reserved.
  UINT8    ChannelState  : 1;         ///< Channel State.
  UINT8    Reserved2     : 7;         ///< Reserved.
} IPMI_ENABLE_MESSAGE_CHANNEL_RESPONSE;

/**
  Set System Info Parameters Command.
*/
#define IPMI_APP_SET_SYSTEM_INFO  0x58

/**
  System Info String Encoding.
*/
typedef enum {
  SysInfoAscii,    ///< Ascii
  SysInfoUtf8,     ///< Utf8
  SysInfoUnicode   ///< Unicode
} SYSTEM_INFO_STRING_ENCODING;

/**
  System parameter selector.
*/
typedef enum {
  SysInfoSetInProgress,    ///< SetInProgress.
  SysInfoFirmwareVersion,  ///< FirmwareVersion.
  SysInfoSystemName,       ///< SystemName.
  SysInfoPrimaryOsName,    ///< PrimaryOsName.
  SysInfoPresentOsName,    ///< PresentOsName.
  SysInfoPresentOsVersion, ///< PresentOsVersion.
  SysInfoBmcUrl,           ///< BmcUrl.
  SysInfoHyperviserUrl,    ///< HyperviserUrl.
} SYSTEM_INFO_PARAMETER_SELECTOR;

/**
  System info set state.
*/
typedef enum {
  SysInfoStateSetComplete,     ///< SetComplete.
  SysInfoStateSetInProgress,   ///< SetInProgress.
  SysInfoStateCommitWrite,     ///< StateCommitWrite.
  SysInfoStateReserved,        ///< StateReserved.
} SYSTEM_INFO_SET_STATE;

/**
  Set system info parameter command request Structure.
*/
typedef struct {
  UINT8    ParamSelector;               /// Parameter selector.
  UINT8    SetSelector;                 ///< Data 1
  UINT8    Data[16];                    ///< Data 2:17
} SET_SYSTEM_INFO_REQUEST;

/**
  Get System Info Parameters Command.
*/
#define IPMI_APP_GET_SYSTEM_INFO  0x59

/**
  Get system info Command request Structure.
*/
typedef struct {
  UINT8    Reserved : 7;          /// Reserved.
  UINT8    GetParam : 1;          ///< Get Parameter.
  UINT8    ParamSelector;         ///< Parameter Selector.
  UINT8    SetSelector;           ///< Set selector.
  UINT8    BlockSelector;         ///< Block selector.
} GET_SYSTEM_INFO_REQUEST;

/**
  Get system info command response Structure.
*/
typedef struct {
  UINT8    CompletionCode;            /// Completion code.
  UINT8    ParamRevision;             /// Parameter Revision
  union {
    struct {
      UINT8    State    : 2;          ///< State.
      UINT8    Reserved : 6;          ///< Reserved.
    } Progress;
    UINT8    SetSelector;             ///< Set Selector.
  } Data1;
  UINT8    Data[16];                  ///< Data 2:17.
} GET_SYSTEM_INFO_RESPONSE;

/**
  Get system Guid Command response Structure.
*/
typedef struct {
  UINT8     CompletionCode;      /// Completion code.
  UINT8     Node[6];             ///< Node.
  UINT8     Clock[2];            ///< Clock.
  UINT16    Time_High;           ///< Time High.
  UINT16    Time_Mid;            ///< Time Middle.
  UINT32    Time_Low;            ///< Time Low.
} GET_SYSTEM_GUID_RESPONSE;

/**
  Get Bt interface Capability Command response Structure.
*/
typedef struct {
  UINT8    CompletionCode;        /// Completion code.
  UINT8    OutstaningReq;         ///< Number of outstanding requests supported.
  UINT8    InputBuffSize;         ///< Input (request) buffer message size in bytes.
  UINT8    OutputBuffSize;        ///< Output (response) buffer message size in bytes.
  UINT8    BmcReqToResTime;       ///< BMC Request-to-Response time, in seconds.
  UINT8    RecommandedRetires;    ///< Recommended retries (1 based
} IPMI_BT_INTERFACE_CAPABILITY_RES;

/**
  Get System interface Capability Command request Structure.
*/
typedef struct {
  UINT8    SystemInterfaceType : 4;     /// System Interface Type 0h = SSIF,1h = KCS ,2h = SMIC.
  UINT8    Reserved            : 4;     ///< Reserved.
} IPMI_GET_SYSTEM_INTERFACE_CAPABILITY_REQ;

/**
  Get System interface Capability Command response Structure.
*/
typedef struct {
  UINT8    CompletionCode;            /// Completion code.
  UINT8    Reserved;                  ///< Reserved.
  UINT8    SsifVersion        : 3;    ///< Ssif Version.
  UINT8    PecSupport         : 1;    ///< Pec Support.
  UINT8    Reserved1          : 2;    ///< Reserved.
  UINT8    TransactionSupport : 2;    ///< Transaction Support.
  UINT8    InputMessageSize;          ///< Input Message Size.
  UINT8    OutputMessageSize;         ///< Output Message Size.
} IPMI_GET_SYSTEM_INTERFACE_CAPABILITY_RES;

/**
  Get BMC global Enable Command response Structure.
*/
typedef struct {
  UINT8    CompletionCode;                      /// Completion code.
  UINT8    ReceiveMsgQueueInterrupt    : 1;     ///< Receive Message Queue Interrupt Enable bit.
  UINT8    EventMsgBufferFullInterrupt : 1;     ///< Event Message Buffer Full Interrupt Enable bit.
  UINT8    EventMsgBuffer              : 1;     ///< Event Message Buffer Enable bit.
  UINT8    SystemEventLogging          : 1;     ///< System Event Logging Enable bit.
  UINT8    Reserved                    : 1;     ///< Reserved.
  UINT8    OEM0                        : 1;     ///< oem0 Enable bit.
  UINT8    OEM1                        : 1;     ///< oem1 Enable bit.
  UINT8    OEM2                        : 1;     ///< Oem2 Enable bit.
} IPMI_BMC_GLOBAL_ENABLES_RES;
#pragma pack()

#endif // #ifndef _IPMI_NETFN_APP_DEFINITIONS_H_
