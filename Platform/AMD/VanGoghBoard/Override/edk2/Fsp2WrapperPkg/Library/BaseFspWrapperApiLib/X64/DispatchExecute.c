/** @file
  Execute 64-bit code in Long Mode.
  Provide a thunk function to transition from long mode to compatibility mode to execute 32-bit code and then transit
  back to long mode.

  Copyright (C) 2024 Advanced Micro Devices, Inc. All rights reserved.<BR>
  Copyright (c) 2014 - 2022, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <FspEas.h>
#include <Library/DebugLib.h>

UINT64
ReadRsp (
  VOID
  );

/**
  FSP API functions.

  @param[in] Param1       The first parameter to pass to 64bit code.
  @param[in] Param2       The second parameter to pass to 64bit code.

  @return EFI_STATUS.
**/
typedef
EFI_STATUS
(EFIAPI *FSP_FUNCTION)(
  IN VOID *Param1,
  IN VOID *Param2
  );

#pragma pack(1)
typedef union {
  struct {
    UINT32    LimitLow    : 16;
    UINT32    BaseLow     : 16;
    UINT32    BaseMid     : 8;
    UINT32    Type        : 4;
    UINT32    System      : 1;
    UINT32    Dpl         : 2;
    UINT32    Present     : 1;
    UINT32    LimitHigh   : 4;
    UINT32    Software    : 1;
    UINT32    Reserved    : 1;
    UINT32    DefaultSize : 1;
    UINT32    Granularity : 1;
    UINT32    BaseHigh    : 8;
  } Bits;
  UINT64    Uint64;
} IA32_GDT;
#pragma pack()

GLOBAL_REMOVE_IF_UNREFERENCED IA32_GDT  mGdtEntries[] = {
  {
    { 0,      0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0, 0 }
  },                                                            /* 0x0:  reserve */
  {
    { 0xFFFF, 0, 0, 0xB, 1, 0, 1, 0xF, 0, 0, 1, 1, 0 }
  },                                                            /* 0x8:  compatibility mode */
  {
    { 0xFFFF, 0, 0, 0xB, 1, 0, 1, 0xF, 0, 1, 0, 1, 0 }
  },                                                            /* 0x10: for long mode */
  {
    { 0xFFFF, 0, 0, 0x3, 1, 0, 1, 0xF, 0, 0, 1, 1, 0 }
  },                                                            /* 0x18: data */
  {
    { 0,      0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0, 0 }
  },                                                            /* 0x20: reserve */
};

//
// IA32 Gdt register
//
GLOBAL_REMOVE_IF_UNREFERENCED IA32_DESCRIPTOR  mGdt = {
  sizeof (mGdtEntries) - 1,
  (UINTN)mGdtEntries
};

/**
  Assembly function to transition from long mode to compatibility mode to execute 32-bit code and then transit back to
  long mode.

  @param[in] Function     The 32bit code entry to be executed.
  @param[in] Param1       The first parameter to pass to 32bit code
  @param[in] Param2       The second parameter to pass to 32bit code
  @param[in] InternalGdtr The GDT and GDT descriptor used by this library

  @return status.
**/
UINT32
EFIAPI
AsmExecute32BitCode (
  IN UINT64           Function,
  IN UINT64           Param1,
  IN UINT64           Param2,
  IN IA32_DESCRIPTOR  *InternalGdtr
  );

/**
  Wrapper for a thunk to transition from long mode to compatibility mode to execute 32-bit code and then transit back to
  long mode.

  @param[in] Function     The 32bit code entry to be executed.
  @param[in] Param1       The first parameter to pass to 32bit code.
  @param[in] Param2       The second parameter to pass to 32bit code.

  @return EFI_STATUS.
**/
IA32_DESCRIPTOR  MemoryIdtr;
EFI_STATUS
Execute32BitCode (
  IN UINT64  Function,
  IN UINT64  Param1,
  IN UINT64  Param2
  )
{
  EFI_STATUS       Status;
  IA32_DESCRIPTOR  Idtr;

  // Idtr might be changed inside of FSP. 32bit FSP only knows the <4G address.
  // If IDTR.Base is >4G, FSP can not handle. So we need save/restore IDTR here for X64 only.
  // Interrupt is already disabled here, so it is safety to update IDTR.
  //
  AsmReadIdtr (&Idtr);
  MemoryIdtr = Idtr;
  DEBUG ((DEBUG_ERROR, "Before FSP:%llx\n", ReadRsp ()));
  Status = AsmExecute32BitCode (Function, Param1, Param2, &mGdt);
  DEBUG ((DEBUG_ERROR, "After FSP:%llx\n", ReadRsp ()));
  ASSERT (Idtr.Limit == MemoryIdtr.Limit && Idtr.Base == MemoryIdtr.Base);
  //
  // Convert FSP Status code from 32bit to 64bit to match caller expectation.
  //
  Status = (Status & ~(BIT31 + BIT30)) | LShiftU64 (Status & (BIT31 + BIT30), 32);
  AsmWriteIdtr (&Idtr);

  return Status;
}

/**
  Wrapper to execute 64-bit code directly from long mode.

  @param[in] Function     The 64bit code entry to be executed.
  @param[in] Param1       The first parameter to pass to 64bit code.
  @param[in] Param2       The second parameter to pass to 64bit code.

  @return EFI_STATUS.
**/
EFI_STATUS
Execute64BitCode (
  IN UINT64  Function,
  IN UINT64  Param1,
  IN UINT64  Param2
  )
{
  FSP_FUNCTION  EntryFunc;
  EFI_STATUS    Status;

  EntryFunc = (FSP_FUNCTION)(UINTN)(Function);
  Status    = EntryFunc ((VOID *)(UINTN)Param1, (VOID *)(UINTN)Param2);

  return Status;
}
