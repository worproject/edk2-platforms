/** @file
  This driver is responsible for the registration of child drivers
  and the abstraction of the PCH SMI sources.

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#include <Register/TcoRegs.h>

GLOBAL_REMOVE_IF_UNREFERENCED UINT32 mTco1StsClear =
  (
    B_TCO_IO_TCO1_STS_DMISERR |
    B_TCO_IO_TCO1_STS_DMISMI |
    B_TCO_IO_TCO1_STS_DMISCI |
    B_TCO_IO_TCO1_STS_BIOSWR |
    B_TCO_IO_TCO1_STS_NEWCENTURY |
    B_TCO_IO_TCO1_STS_TIMEOUT |
    B_TCO_IO_TCO1_STS_TCO_INT |
    B_TCO_IO_TCO1_STS_SW_TCO_SMI
    );
