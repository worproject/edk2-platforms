/** @file
  Header for P2SbSidebandAccessLib

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _P2SB_SIDEBAND_ACCESS_LIB_H_
#define _P2SB_SIDEBAND_ACCESS_LIB_H_

#include <RegisterAccess.h>
#include <P2SbController.h>
#include <Library/PchPcrLib.h> // For PCH_SBI_PID definition

typedef PCH_SBI_PID  P2SB_PID;

typedef enum {
  P2SbMemory = 0,
  P2SbPciConfig,
  P2SbPrivateConfig
} P2SB_REGISTER_SPACE;

typedef enum {
  MemoryRead             = 0x0,
  MemoryWrite            = 0x1,
  PciConfigRead          = 0x4,
  PciConfigWrite         = 0x5,
  PrivateControlRead     = 0x6,
  PrivateControlWrite    = 0x7,
  GpioLockUnlock         = 0x13
} P2SB_SBI_OPCODE;

typedef enum {
  SBI_SUCCESSFUL          = 0,
  SBI_UNSUCCESSFUL        = 1,
  SBI_POWERDOWN           = 2,
  SBI_MIXED               = 3,
  SBI_INVALID_RESPONSE
} P2SB_SBI_RESPONSE;

typedef enum {
  P2SbMmioAccess = 0,
  P2SbMsgAccess
} P2SB_SIDEBAND_ACCESS_METHOD;

/**
  REGISTER_ACCESS for P2SB device to support access to sideband registers.
  Be sure to keep first member of this structure as REGISTER_ACCESS to allow
  for correct casting between caller who sees this structure as REGISTER_ACCESS
  and calle who will cast it to P2SB_SIDEBAND_REGISTER_ACCESS.
**/
typedef struct {
  REGISTER_ACCESS              Access;
  P2SB_SIDEBAND_ACCESS_METHOD  AccessMethod;
  P2SB_PID                     P2SbPid;
  UINT16                       Fid;
  P2SB_REGISTER_SPACE          RegisterSpace;
  BOOLEAN                      PostedWrites;
  P2SB_CONTROLLER              *P2SbCtrl;
} P2SB_SIDEBAND_REGISTER_ACCESS;

/**
  Full function for executing P2SB SBI message
  Take care of that there is no lock protection when using SBI programming in both POST time and SMI.
  It will clash with POST time SBI programming when SMI happen.
  Programmer MUST do the save and restore opration while using the PchSbiExecution inside SMI
  to prevent from racing condition.
  This function will reveal P2SB and hide P2SB if it's originally hidden. If more than one SBI access
  needed, it's better to unhide the P2SB before calling and hide it back after done.

  When the return value is "EFI_SUCCESS", the "Response" do not need to be checked as it would have been
  SBI_SUCCESS. If the return value is "EFI_DEVICE_ERROR", then this would provide additional information
  when needed.

  @param[in] P2sbBase                   P2SB PCI config base
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
P2SbSbiExecutionEx (
  IN     UINT64           P2sbBase,
  IN     P2SB_PID         Pid,
  IN     UINT64           Offset,
  IN     P2SB_SBI_OPCODE  Opcode,
  IN     BOOLEAN          Posted,
  IN     UINT16           Fbe,
  IN     UINT16           Bar,
  IN     UINT16           Fid,
  IN OUT UINT32           *Data32,
  OUT    UINT8            *Response
  );

#endif
