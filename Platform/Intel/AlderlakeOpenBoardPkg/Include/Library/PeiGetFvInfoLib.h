/** @file
  Header file for PeiGetFvInfoLib..

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _PEI_GET_FV_INFO_LIB_H_
#define _PEI_GET_FV_INFO_LIB_H_

/**
  PeiGetSectionFromFv finds the file in FV and gets file Address and Size

  @param[in] NameGuid              - File GUID
  @param[out] Address              - Pointer to the File Address
  @param[out] Size                 - Pointer to File Size

  @retval EFI_SUCCESS                Successfull in reading the section from FV
**/
EFI_STATUS
EFIAPI
PeiGetSectionFromFv (
  IN CONST  EFI_GUID        NameGuid,
  OUT VOID                  **Address,
  OUT UINT32                *Size
  );

#endif
