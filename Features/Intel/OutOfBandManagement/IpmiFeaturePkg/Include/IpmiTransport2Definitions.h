/** @file IpmiTransport2Definitions.h
  Bmc Common interface library functions.

  @copyright
  Copyright 2016 - 2021 Intel Corporation. <BR>
  Copyright (c) 1985 - 2023, American Megatrends International LLC. <BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _IPMI_TRANSPORT2_DEFINITIONS_H_
#define _IPMI_TRANSPORT2_DEFINITIONS_H_

typedef struct _IPMI_TRANSPORT2 IPMI_TRANSPORT2;

/** @internal
  BT Interface.
*/
typedef struct {
  UINT8      InterfaceState;       /// Interface state.
  UINT16     CtrlPort;             ///< Control port.
  UINT16     ComBuffer;            ///< Communication buffer port.
  UINT16     IntMaskPort;          ///< Interrupt mask port.
  UINTN      MmioBaseAddress;      ///< Mmio base address.
  UINTN      BaseAddressRange;     ///< Mmio base address range to
                                   ///< differentiate port address.
  UINT8      AccessType;           ///< Access type - IO or MMIO.
  UINT32     BtRetryCount;         ///< Delay counter for retry.
  UINT8      BtSoftErrorCount;     ///< Soft error count.
  BOOLEAN    BtTransportLocked;    ///< Interface lock.
  UINT8      HosttoBmcBufferSize;  ///< Host to Bmc Buffer Size.
  UINT8      BmctoHostBufferSize;  ///< Bmc to Host Buffer Size.
} BT_SYSTEM_INTERFACE;

/** @internal
  SSIF Interface.
*/
typedef struct {
  UINT8       InterfaceState;       /// Interface state.
  EFI_GUID    SsifInterfaceApiGuid; ///< Smbus instance guid.
  UINTN       SsifInterfaceApiPtr;  ///< Smbus instance pointer.
  UINT8       RwSupport;            ///< Read-write support.
  UINT16      SsifRetryCounter;     ///< Retry counter.
  BOOLEAN     PecSupport;           ///< Packet Error Check support.
  BOOLEAN     SmbAlertSupport;      ///< Smbus alert support.
  UINT8       SsifSoftErrorCount;   ///< Soft error count.
  BOOLEAN     SsifTransportLocked;  ///< Interface lock.
} SSIF_SYSTEM_INTERFACE;

/** @internal
  IPMB Interface.
*/
typedef struct {
  /// Interface state.
  UINT8       InterfaceState;
  EFI_GUID    IpmbInterfaceApiGuid; ///< Ipmb instance guid.
  UINTN       IpmbInterfaceApiPtr;  ///< Ipmb instance pointer.
  UINT8       IpmbSoftErrorCount;   ///< Soft error count.
  BOOLEAN     IpmbTransportLocked;  ///< Interface lock.
} IPMB_SYSTEM_INTERFACE;

/** @internal
  System Interface.
*/
typedef struct {
  UINT8                    KcsInterfaceState;
  BT_SYSTEM_INTERFACE      Bt;       ///< Bt interface.
  SSIF_SYSTEM_INTERFACE    Ssif;     ///< Ssif interface.
  IPMB_SYSTEM_INTERFACE    Ipmb;
} IPMI_SYSTEM_INTERFACE;

/** @inrernal
  Ipmi Interface Access Type.
*/
typedef enum {
  IpmiMmioAccess,  ///< Mmio Access.
  IpmiIoAccess     ///< Io Access.
} IPMI_ACCESS_TYPE;

/** @internal
  Host to BMC Interface Type.
*/
typedef enum {
  SysInterfaceUnknown,    ///< Unknown interface type.
  SysInterfaceKcs,        ///< Kcs interface.
  SysInterfaceSmic,       ///< Smic interface.
  SysInterfaceBt,         ///< Bt interface.
  SysInterfaceSsif,       ///< Ssif interface.
  SysInterfaceIpmb,       ///< Ipmb interface.
  SysInterfaceMax         ///< Maximum interface type.
} SYSTEM_INTERFACE_TYPE;

/** @internal
  BMC Interface status.
*/
typedef enum {
  BmcStatusOk,                 ///< Bmc status Ok.
  BmcStatusSoftFail,           ///< Bmc status Soft fail.
  BmcStatusHardFail,           ///< Bmc status Hard fail.
  BmcStatusUpdateInProgress    ///< Bmc status Update in progress.
} BMC_INTERFACE_STATUS;

/** @internal
  Ipmi Interface state.
*/
typedef enum {
  IpmiInterfaceNotReady,       ///< Interface Not Ready.
  IpmiInterfaceInitialized,    ///< Interface Initialized.
  IpmiInterfaceInitError,      ///< Interface Initialization Error.
} IPMI_INTERFACE_STATE;

//
//  IPMI Function Prototypes
//
typedef
  EFI_STATUS
(EFIAPI *IPMI_SEND_COMMAND2)(
                             IN IPMI_TRANSPORT2                   *This,
                             IN UINT8                             NetFunction,
                             IN UINT8                             Lun,
                             IN UINT8                             Command,
                             IN UINT8                             *CommandData,
                             IN UINT32                            CommandDataSize,
                             OUT UINT8                            *ResponseData,
                             OUT UINT32                           *ResponseDataSize
                             );

typedef
  EFI_STATUS
(EFIAPI *IPMI_SEND_COMMAND2Ex)(
                               IN IPMI_TRANSPORT2                   *This,
                               IN UINT8                             NetFunction,
                               IN UINT8                             Lun,
                               IN UINT8                             Command,
                               IN UINT8                             *CommandData,
                               IN UINT32                            CommandDataSize,
                               OUT UINT8                            *ResponseData,
                               OUT UINT32                           *ResponseDataSize,
                               IN SYSTEM_INTERFACE_TYPE             InterfaceType
                               );

struct _IPMI_TRANSPORT2 {
  UINT64                   Revision;
  IPMI_SEND_COMMAND2       IpmiSubmitCommand2;
  IPMI_SEND_COMMAND2Ex     IpmiSubmitCommand2Ex;
  IPMI_SYSTEM_INTERFACE    Interface;                ///< System interface.
  SYSTEM_INTERFACE_TYPE    InterfaceType;            ///< Bmc Interface Type.
  BMC_INTERFACE_STATUS     IpmiTransport2BmcStatus;
  EFI_HANDLE               IpmiHandle;
  UINT8                    CompletionCode;
};

#endif
