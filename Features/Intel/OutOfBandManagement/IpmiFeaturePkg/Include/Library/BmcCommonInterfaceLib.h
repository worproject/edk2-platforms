/** @file BmcCommonInterfaceLib.h
  Bmc Common interface library functions.

  @copyright
  Copyright 2016 - 2021 Intel Corporation. <BR>
  Copyright (c) 1985 - 2023, American Megatrends International LLC. <BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _BMC_COMMON_INTERFACE_LIB_H_
#define _BMC_COMMON_INTERFACE_LIB_H_

#include <IndustryStandard/IpmiNetFnApp.h>
#include <IpmiTransport2Definitions.h>
#include <ServerManagement.h>

#define IPMI_APP_SELFTEST_RESERVED  0xFF

#define IPMI_GET_SET_IN_PROGRESS_RETRY_COUNT  10
#define IPMI_BIT_CLEAR                        0
#define IPMI_SELECTOR_NONE                    0
#define IPMI_CLEAR_FLAG                       0
#define IPMI_SET_FLAG                         1
#define IPMI_STALL                            1000
#define MIN_TO_100MS                          60 * 10
#define MAX_BMC_CMD_FAIL_COUNT                10

// Completion code macros.
#define IPMI_COMPLETION_CODE_SUCCESS                 0x00
#define IPMI_COMPLETION_CODE_DEVICE_SPECIFIC_START   0x01
#define IPMI_COMPLETION_CODE_DEVICE_SPECIFIC_END     0x7E
#define IPMI_COMPLETION_CODE_COMMAND_SPECIFIC_START  0x80
#define IPMI_COMPLETION_CODE_COMMAND_SPECIFIC_END    0xBE
#define IPMI_MAX_SOFT_COUNT                          10

#define IPMI_MAX_BT_CMD_DATA_SIZE  0xFF

#define IPMI_ERROR_COMPLETION_CODE(a)  !((a == IPMI_COMPLETION_CODE_SUCCESS) ||   \
                                             ((a >= IPMI_COMPLETION_CODE_DEVICE_SPECIFIC_START) && \
                                              (a <= IPMI_COMPLETION_CODE_DEVICE_SPECIFIC_END)) || \
                                             ((a >= IPMI_COMPLETION_CODE_COMMAND_SPECIFIC_START) && \
                                              (a <= IPMI_COMPLETION_CODE_COMMAND_SPECIFIC_END)) \
                                             )

/*++

Routine Description:
  Read 8 bit data from BMC port based on access type.

Arguments:
  AccessType - Specifies MMIO or IO access.
  Address    - Specifies Address to read.

Returns:
  UINT8 - Data read.

--*/
UINT8
IpmiBmcRead8 (
  IN UINT8  AccessType,
  IN UINTN  Address
  );

/*++

Routine Description:
  Write 8 bit data to BMC port based on access type.

Arguments:
  AccessType - Specifies MMIO or IO access.
  Address    - Specifies Address to write.
  Data       - Specifies data to be written.

Returns:
  UINT8 - Data written.

--*/
UINT8
IpmiBmcWrite8 (
  IN UINT8  AccessType,
  IN UINTN  Address,
  IN UINT8  Data
  );

/*++

Routine Description:
  Acquire the lock to use the IPMI transport.

Arguments:
  Lock - Pointer to Lock.

Returns:
  VOID - Returns nothing.

--*/
VOID
IpmiTransportAcquireLock (
  OUT BOOLEAN  *Lock
  );

/*++

Routine Description:
  Release the lock of IPMI transport.

Arguments:
  Lock - Pointer to Lock.

Returns:
  VOID - Returns nothing.

--*/
VOID
IpmiTransportReleaseLock (
  OUT BOOLEAN  *Lock
  );

/*++

Routine Description:
  Returns the Lock state of IPMI transport.

Arguments:
  Lock - Pointer to Lock.

Returns:
  TRUE  - IPMI transport is in lock state.
  FALSE - IPMI transport is in release state.
--*/
BOOLEAN
IpmiIsIpmiTransportlocked (
  IN BOOLEAN  *Lock
  );

/*++

Routine Description:
  Updates the SoftErrorCount of specific interface based on the BMC Error input.

Arguments:
  BmcError - BMC Error.
  Interface - Interface pointer to update soft error count.
  InterfaceType - Interface type to communicate.

Returns:
  EFI_SUCCESS           - Updated SoftErrorCount of specific interface.
  EFI_INVALID_PARAMETER - Invalid Interface pointer or Interface type.
--*/
EFI_STATUS
IpmiUpdateSoftErrorCount (
  IN     UINT8                  BmcError,
  IN OUT IPMI_SYSTEM_INTERFACE  *Interface,
  IN     SYSTEM_INTERFACE_TYPE  InterfaceType
  );

/*++

Routine Description:
  Check the BMC Interface self test for the specified Interface.

Arguments:
  IpmiTransport2 - IPMI Interface pointer.
  BmcStatus      - BMC Interface status.
  InterfaceType  - Interface type to communicate.

Returns:
  EFI_SUCCESS           - BMC self test command success.
  EFI_INVALID_PARAMETER - Invalid Interface pointer or Interface type.
--*/
EFI_STATUS
CheckSelfTestByInterfaceType (
  IN OUT  IPMI_TRANSPORT2        *IpmiTransport2,
  IN OUT  BMC_INTERFACE_STATUS   *BmcStatus,
  IN      SYSTEM_INTERFACE_TYPE  InterfaceType
  );

/*++

Routine Description:
  Initialize BT interface specific data.

Arguments:
  IpmiTransport2 - IPMI transport2 protocol pointer.

Returns:
  Status.
--*/
EFI_STATUS
InitBtInterfaceData (
  IN OUT IPMI_TRANSPORT2  *IpmiTransport2
  );

/*++

Routine Description:
  Initialize SSIF interface specific data.

Arguments:
  IpmiTransport2     - Pointer to IPMI transport2 instance.

Returns:
  Status - Return status while initializing interface.
--*/
EFI_STATUS
InitSsifInterfaceData (
  IN OUT IPMI_TRANSPORT2  *IpmiTransport2
  );

/*++

Routine Description:
  Initialize IPMB interface specific data.

Arguments:
  IpmiTransport2    - Pointer to IPMI Transport2 instance.

Returns:
  EFI_SUCCESS - Interface is successfully initialized.
  Others      - Error status while initializing interface.
--*/
EFI_STATUS
InitIpmbInterfaceData (
  IN OUT IPMI_TRANSPORT2  *IpmiTransport2
  );

#endif
