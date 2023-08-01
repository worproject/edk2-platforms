/** @file
  Macros for platform to update different types of policy.

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _POLICY_UPDATE_MACRO_H_
#define _POLICY_UPDATE_MACRO_H_

#ifdef UPDATE_POLICY
#undef UPDATE_POLICY
#endif

#ifdef COPY_POLICY
#undef COPY_POLICY
#endif

#ifdef GET_POLICY
#undef GET_POLICY
#endif

#ifdef AND_POLICY
#undef AND_POLICY
#endif

#ifdef OR_POLICY
#undef OR_POLICY
#endif

#define UPDATE_POLICY(UpdField, ConfigField, Value)  ConfigField = Value;
#define COPY_POLICY(UpdField, ConfigField, Value, Size)  CopyMem (ConfigField, Value, Size);
#define GET_POLICY(UpdField, ConfigField, Value)  Value = ConfigField;
#define AND_POLICY(UpdField, ConfigField, Value)  ConfigField &= Value;
#define OR_POLICY(UpdField, ConfigField, Value)  ConfigField |= Value;
//
// Compare Policy Default and Setup Default when FirstBoot and RvpSupport
//

#define COMPARE_AND_UPDATE_POLICY(UpdField, ConfigField, Value) {\
  UPDATE_POLICY(UpdField, ConfigField, Value);\
}
#define COMPARE_UPDATE_POLICY_ARRAY(UpdField, ConfigField, Value, ArrayIndex) {\
  UPDATE_POLICY(UpdField, ConfigField, Value);\
}

#endif //_POLICY_UPDATE_MACRO_H_
