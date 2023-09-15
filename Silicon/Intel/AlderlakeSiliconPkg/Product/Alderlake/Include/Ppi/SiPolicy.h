/** @file
  Silicon Policy PPI is used for specifying platform
  related Intel silicon information and policy setting.
  This PPI is consumed by the silicon PEI modules and carried
  over to silicon DXE modules.

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _SI_POLICY_PPI_H_
#define _SI_POLICY_PPI_H_

#include <SiPolicyStruct.h>
#include <PchPolicyCommon.h>
#include <PchPreMemPolicyCommon.h>
#include <Uefi.h>
#include <ConfigBlock/CpuConfig.h>
#include <ConfigBlock/CpuConfigLibPreMemConfig.h>
#include <ConfigBlock/CpuSecurityPreMemConfig.h>

#ifndef DISABLED
#define DISABLED  0
#endif
#ifndef ENABLED
#define ENABLED   1
#endif

extern EFI_GUID gSiPreMemPolicyPpiGuid;
extern EFI_GUID gSiPolicyPpiGuid;


#include <GraphicsConfig.h>


#include <CpuPcieConfigGen3.h>
#include <CpuPcieConfig.h>
extern EFI_GUID gCpuPciePeiPreMemConfigGuid;
extern EFI_GUID gCpuPcieRpConfigGuid;

#include <MemoryConfig.h>
extern EFI_GUID gMemoryConfigGuid;
extern EFI_GUID gMemoryConfigNoCrcGuid;

#include <ConfigBlock/SaMiscPeiPreMemConfig.h>
extern EFI_GUID gSaMiscPeiPreMemConfigGuid;

#include <HostBridgeConfig.h>
extern EFI_GUID gHostBridgePeiPreMemConfigGuid;
extern EFI_GUID gHostBridgePeiConfigGuid;

typedef struct _SI_PREMEM_POLICY_STRUCT SI_PREMEM_POLICY_PPI;
typedef struct _SI_POLICY_STRUCT SI_POLICY_PPI;

#endif // _SI_POLICY_PPI_H_
