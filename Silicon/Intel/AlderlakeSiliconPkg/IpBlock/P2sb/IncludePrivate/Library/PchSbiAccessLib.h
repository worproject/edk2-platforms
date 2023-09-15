/** @file
  Header file for PchSbiAccessLib.

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _PCH_SBI_ACCESS_LIB_H_
#define _PCH_SBI_ACCESS_LIB_H_

#include <Library/PchPcrLib.h>
#include <Library/P2SbSidebandAccessLib.h>

typedef P2SB_SBI_OPCODE  PCH_SBI_OPCODE ;

typedef P2SB_SBI_RESPONSE  PCH_SBI_RESPONSE;

/**
  Full function for executing PCH SBI message
  Take care of that there is no lock protection when using SBI programming in both POST time and SMI.
  It will clash with POST time SBI programming when SMI happen.
  Programmer MUST do the save and restore opration while using the PchSbiExecution inside SMI
  to prevent from racing condition.
  This function will reveal P2SB and hide P2SB if it's originally hidden. If more than one SBI access
  needed, it's better to unhide the P2SB before calling and hide it back after done.

  When the return value is "EFI_SUCCESS", the "Response" do not need to be checked as it would have been
  SBI_SUCCESS. If the return value is "EFI_DEVICE_ERROR", then this would provide additional information
  when needed.

  @param[in] Pid                        Port ID of the SBI message
  @param[in] Offset                     Offset of the SBI message
  @param[in] Opcode                     Opcode
  @param[in] Posted                     Posted message
  @param[in] Fbe                        First byte enable
  @param[in] Bar                        Bar
  @param[in] Fid                        Function ID
  @param[in, out] Data32                Read/Write data
  @param[out] Response                  Response

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_DEVICE_ERROR              Transaction fail
  @retval EFI_INVALID_PARAMETER         Invalid parameter
  @retval EFI_TIMEOUT                   Timeout while waiting for response
**/
EFI_STATUS
PchSbiExecutionEx (
  IN     PCH_SBI_PID                    Pid,
  IN     UINT64                         Offset,
  IN     PCH_SBI_OPCODE                 Opcode,
  IN     BOOLEAN                        Posted,
  IN     UINT16                         Fbe,
  IN     UINT16                         Bar,
  IN     UINT16                         Fid,
  IN OUT UINT32                         *Data32,
  OUT    UINT8                          *Response
  );

#endif // _PCH_SBI_ACCESS_LIB_H_
