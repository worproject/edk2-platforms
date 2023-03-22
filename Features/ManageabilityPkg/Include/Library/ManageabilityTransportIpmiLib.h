/** @file

  This file defines the manageability IPMI protocol specific transport data.

  Copyright (C) 2023 Advanced Micro Devices, Inc. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef MANAGEABILITY_TRANSPORT_IPMI_LIB_H_
#define MANAGEABILITY_TRANSPORT_IPMI_LIB_H_

#include <Library/ManageabilityTransportLib.h>

///
/// The IPMI command header which is apart from
/// the payload.
///
typedef struct {
  UINT8    Lun:2;
  UINT8    NetFn:6;
  UINT8    Command;
} MANAGEABILITY_IPMI_TRANSPORT_HEADER;

#endif
