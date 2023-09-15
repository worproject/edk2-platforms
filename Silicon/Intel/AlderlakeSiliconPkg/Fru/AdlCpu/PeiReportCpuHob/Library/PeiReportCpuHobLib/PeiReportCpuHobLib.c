/** @file

  Source code file for Report CPU HOB library.

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Base.h>
#include <Library/HobLib.h>
#include <Library/CpuPlatformLib.h>

VOID
ReportCpuHob (
  VOID
  )
{
  ///
  /// Create a CPU hand-off information
  ///
  BuildCpuHob (GetMaxPhysicalAddressSize (), 16);
}
