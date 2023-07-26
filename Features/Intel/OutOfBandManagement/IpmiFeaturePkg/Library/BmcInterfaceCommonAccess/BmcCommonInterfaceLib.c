/** @file BmcCommonInterfaceLib.c
  BmcCommonInterfaceLib generic functions for all interfaces.

  @copyright
  Copyright 2016 - 2021 Intel Corporation. <BR>
  Copyright (c) 1985 - 2023, American Megatrends International LLC. <BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/BmcCommonInterfaceLib.h>

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
  )
{
  if (AccessType == IpmiIoAccess) {
    return IoRead8 (Address);
  } else {
    return MmioRead8 (Address);
  }
}

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
  )
{
  if (AccessType == IpmiIoAccess) {
    return IoWrite8 (
                     Address,
                     Data
                     );
  } else {
    return MmioWrite8 (
                       Address,
                       Data
                       );
  }
}

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
  )
{
  *Lock = TRUE;
}

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
  )
{
  *Lock = FALSE;
}

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
  )
{
  return *Lock;
}

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
  )
{
  UINT8  Errors[] = { 0xC0, 0xC3, 0xC4, 0xC9, 0xCE, 0xCF, 0xFF, 0x00 };
  UINT8  Index    = 0;

  if ((Interface == NULL) || (InterfaceType <= SysInterfaceUnknown) ||
      (InterfaceType >= SysInterfaceMax))
  {
    return EFI_INVALID_PARAMETER;
  }

  while (Errors[Index] != 0) {
    if (Errors[Index] == BmcError) {
      switch (InterfaceType) {
        case SysInterfaceBt:
          Interface->Bt.BtSoftErrorCount++;
          break;

        case SysInterfaceSsif:
          Interface->Ssif.SsifSoftErrorCount++;
          break;

        default:
          break;
      }
    }

    Index++;
  }

  return EFI_SUCCESS;
}

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
  )
{
  EFI_STATUS                      Status;
  IPMI_SELF_TEST_RESULT_RESPONSE  BstStatus;
  UINT32                          ResponseDataSize;

  if ((IpmiTransport2 == NULL) || (BmcStatus == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  ResponseDataSize = sizeof (IPMI_SELF_TEST_RESULT_RESPONSE);

  Status = IpmiTransport2->IpmiSubmitCommand2Ex (
                                                 IpmiTransport2,
                                                 IPMI_NETFN_APP,
                                                 BMC_LUN,
                                                 IPMI_APP_GET_SELFTEST_RESULTS,
                                                 NULL,
                                                 0,
                                                 (UINT8 *)&BstStatus,
                                                 &ResponseDataSize,
                                                 InterfaceType
                                                 );
  if (EFI_ERROR (Status)) {
    *BmcStatus = BmcStatusHardFail;
    return Status;
  }

  if (BstStatus.CompletionCode == IPMI_COMPLETION_CODE_SUCCESS) {
    /* Check the self test results.  Cases 55h - 58h are Ipmi defined
       test results. Additional Cases are device specific test results.*/
    switch (BstStatus.Result) {
      case IPMI_APP_SELFTEST_NO_ERROR:            // 0x55
      case IPMI_APP_SELFTEST_NOT_IMPLEMENTED:     // 0x56
      case IPMI_APP_SELFTEST_RESERVED:            // 0xFF
        *BmcStatus = BmcStatusOk;
        break;

      case IPMI_APP_SELFTEST_ERROR:     // 0x57
        *BmcStatus = BmcStatusSoftFail;
        break;

      default:     // 0x58 and Other Device Specific Hardware Error.
        *BmcStatus = BmcStatusHardFail;
        break;
    }
  }

  return Status;
}
