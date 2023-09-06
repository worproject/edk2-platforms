/** @file
  Generic IPMI stack driver

  @copyright
  Copyright 1999 - 2021 Intel Corporation. <BR>
  Copyright (c) 1985 - 2023, American Megatrends International LLC. <BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <IndustryStandard/Ipmi.h>
#include <SmStatusCodes.h>
#include "IpmiHooks.h"
#include "IpmiBmcCommon.h"
#include "IpmiBmc.h"
#include "IpmiPhysicalLayer.h"
#include <Library/TimerLib.h>
#include <Library/BmcCommonInterfaceLib.h>
#ifdef FAST_VIDEO_SUPPORT
  #include <Protocol/VideoPrint.h>
#endif
#include <Library/UefiRuntimeServicesTableLib.h>

/******************************************************************************
 * Local variables
 */
IPMI_BMC_INSTANCE_DATA  *mIpmiInstance = NULL;
EFI_HANDLE              mImageHandle;

//
// Specific test interface
//
VOID
GetDeviceSpecificTestResults (
  IN      IPMI_BMC_INSTANCE_DATA  *IpmiInstance
  )

/*++

Routine Description:

  This is a BMC specific routine to check the device specific self test results as defined
  in the Bensley BMC core specification.

Arguments:

  IpmiInstance  - Data structure describing BMC variables and used for sending commands

Returns:

  VOID

--*/
{
  return;
}

EFI_STATUS
GetSelfTest (
  IN      IPMI_BMC_INSTANCE_DATA  *IpmiInstance,
  IN      EFI_STATUS_CODE_VALUE   StatusCodeValue[],
  IN OUT  UINT8                   *ErrorCount
  )

/*++

Routine Description:

  Execute the Get Self Test results command to determine whether or not the BMC self tests
  have passed

Arguments:

  IpmiInstance    - Data structure describing BMC variables and used for sending commands
  StatusCodeValue - An array used to accumulate error codes for later reporting.
  ErrorCount      - Counter used to keep track of error codes in StatusCodeValue

Returns:

  EFI_SUCCESS       - BMC Self test results are retrieved and saved into BmcStatus
  EFI_DEVICE_ERROR  - BMC failed to return self test results.

--*/
{
  EFI_STATUS  Status;
  UINT32      DataSize;
  UINT8       Index;
  UINT8       *TempPtr;
  UINT32      Retries;
  BOOLEAN     bResultFlag = FALSE;
  UINT8       TempData[MAX_TEMP_DATA];

  IPMI_SELF_TEST_RESULT_RESPONSE  *SelfTestResult;

  //
  // Get the SELF TEST Results.
  //
  //
  // Note: If BMC PcdIpmiBmcReadyDelayTimer < BMC_KCS_TIMEOUT, it need set Retries as 1. Otherwise it will make SELT failure, caused by below condition (EFI_ERROR(Status) || Retries == 0)
  //
  if (PcdGet8 (PcdIpmiBmcReadyDelayTimer) < BMC_KCS_TIMEOUT) {
    Retries = 1;
  } else {
    Retries = PcdGet8 (PcdIpmiBmcReadyDelayTimer);
  }

  DataSize = sizeof (TempData);

  SelfTestResult         = (IPMI_SELF_TEST_RESULT_RESPONSE *)&TempData[0];
  SelfTestResult->Result = 0;

  do {
    Status = IpmiSendCommand (
                              &IpmiInstance->IpmiTransport,
                              IPMI_NETFN_APP,
                              0,
                              IPMI_APP_GET_SELFTEST_RESULTS,
                              NULL,
                              0,
                              TempData,
                              &DataSize
                              );
    if (Status == EFI_SUCCESS) {
      switch (SelfTestResult->Result) {
        case IPMI_APP_SELFTEST_NO_ERROR:
        case IPMI_APP_SELFTEST_NOT_IMPLEMENTED:
        case IPMI_APP_SELFTEST_ERROR:
        case IPMI_APP_SELFTEST_FATAL_HW_ERROR:
          bResultFlag = TRUE;
          break;

        default:
          break;
      } // switch

      if (bResultFlag) {
        break;
      }
    }

    MicroSecondDelay (500 * 1000);
  } while (--Retries > 0);

  //
  // If Status indicates a Device error, then the BMC is not responding, so send an error.
  //
  if (EFI_ERROR (Status) || (Retries == 0)) {
    DEBUG ((DEBUG_ERROR, "\n[IPMI]  BMC self-test does not respond (status: %r)!\n\n", Status));
    if (*ErrorCount < MAX_SOFT_COUNT) {
      StatusCodeValue[*ErrorCount] = EFI_COMPUTING_UNIT_FIRMWARE_PROCESSOR | EFI_CU_FP_EC_COMM_ERROR;
      (*ErrorCount)++;
    }

    IpmiInstance->BmcStatus = BMC_HARDFAIL;
    return Status;
  } else {
    DEBUG ((DEBUG_INFO, "[IPMI] BMC self-test result: %02X-%02X\n", SelfTestResult->Result, SelfTestResult->Param));
    //
    // Copy the Self test results to Error Status.  Data will be copied as long as it
    // does not exceed the size of the ErrorStatus variable.
    //
    for (Index = 0, TempPtr = (UINT8 *)&IpmiInstance->ErrorStatus;
         (Index < DataSize) && (Index < sizeof (IpmiInstance->ErrorStatus));
         Index++, TempPtr++
         )
    {
      *TempPtr = TempData[Index];
    }

    //
    // Check the IPMI defined self test results.
    // Additional Cases are device specific test results.
    //
    switch (SelfTestResult->Result) {
      case IPMI_APP_SELFTEST_NO_ERROR:
      case IPMI_APP_SELFTEST_NOT_IMPLEMENTED:
        IpmiInstance->BmcStatus = BMC_OK;
        break;

      case IPMI_APP_SELFTEST_ERROR:
        //
        // Three of the possible errors result in BMC hard failure; FRU Corruption,
        // BootBlock Firmware corruption, and Operational Firmware Corruption.  All
        // other errors are BMC soft failures.
        //
        if ((SelfTestResult->Param & (IPMI_APP_SELFTEST_FRU_CORRUPT | IPMI_APP_SELFTEST_FW_BOOTBLOCK_CORRUPT | IPMI_APP_SELFTEST_FW_CORRUPT)) != 0) {
          IpmiInstance->BmcStatus = BMC_HARDFAIL;
        } else {
          IpmiInstance->BmcStatus = BMC_SOFTFAIL;
        }

        //
        // Check if SDR repository is empty and report it if it is.
        //
        if ((SelfTestResult->Param & IPMI_APP_SELFTEST_SDR_REPOSITORY_EMPTY) != 0) {
          if (*ErrorCount < MAX_SOFT_COUNT) {
            StatusCodeValue[*ErrorCount] = EFI_COMPUTING_UNIT_FIRMWARE_PROCESSOR | CU_FP_EC_SDR_EMPTY;
            (*ErrorCount)++;
          }
        }

        break;

      case IPMI_APP_SELFTEST_FATAL_HW_ERROR:
        IpmiInstance->BmcStatus = BMC_HARDFAIL;
        break;

      default:
        IpmiInstance->BmcStatus = BMC_HARDFAIL;
        //
        // Call routine to check device specific failures.
        //
        GetDeviceSpecificTestResults (IpmiInstance);
    }

    if (IpmiInstance->BmcStatus == BMC_HARDFAIL) {
      if (*ErrorCount < MAX_SOFT_COUNT) {
        StatusCodeValue[*ErrorCount] = EFI_COMPUTING_UNIT_FIRMWARE_PROCESSOR | EFI_CU_FP_EC_HARD_FAIL;
        (*ErrorCount)++;
      }
    } else if (IpmiInstance->BmcStatus == BMC_SOFTFAIL) {
      if (*ErrorCount < MAX_SOFT_COUNT) {
        StatusCodeValue[*ErrorCount] = EFI_COMPUTING_UNIT_FIRMWARE_PROCESSOR | EFI_CU_FP_EC_SOFT_FAIL;
        (*ErrorCount)++;
      }
    }
  }

  return EFI_SUCCESS;
} // GetSelfTest()

EFI_STATUS
GetDeviceId (
  IN      IPMI_BMC_INSTANCE_DATA  *IpmiInstance,
  IN      EFI_STATUS_CODE_VALUE   StatusCodeValue[],
  IN OUT  UINT8                   *ErrorCount
  )

/*++

Routine Description:
  Execute the Get Device ID command to determine whether or not the BMC is in Force Update
  Mode.  If it is, then report it to the error manager.

Arguments:
  IpmiInstance    - Data structure describing BMC variables and used for sending commands
  StatusCodeValue - An array used to accumulate error codes for later reporting.
  ErrorCount      - Counter used to keep track of error codes in StatusCodeValue

Returns:
  Status

--*/
{
  EFI_STATUS                 Status;
  UINT32                     DataSize;
  SM_CTRL_INFO               *pBmcInfo;
  IPMI_MSG_GET_BMC_EXEC_RSP  *pBmcExecContext;
  UINT32                     Retries;
  UINT8                      TempData[MAX_TEMP_DATA];

 #ifdef FAST_VIDEO_SUPPORT
  EFI_VIDEOPRINT_PROTOCOL  *VideoPrintProtocol;
  EFI_STATUS               VideoPrintStatus;
 #endif

 #ifdef FAST_VIDEO_SUPPORT
  VideoPrintStatus = gBS->LocateProtocol (
                                          &gEfiVideoPrintProtocolGuid,
                                          NULL,
                                          &VideoPrintProtocol
                                          );
 #endif

  //
  // Set up a loop to retry for up to PcdIpmiBmcReadyDelayTimer seconds. Calculate retries not timeout
  // so that in case KCS is not enabled and IpmiSendCommand() returns
  // immediately we will not wait all the PcdIpmiBmcReadyDelayTimer seconds.
  //
  Retries = PcdGet8 (PcdIpmiBmcReadyDelayTimer);
  //
  // Get the device ID information for the BMC.
  //
  DataSize = sizeof (TempData);
  while (EFI_ERROR (
                    Status = IpmiSendCommand (
                                              &IpmiInstance->IpmiTransport,
                                              IPMI_NETFN_APP,
                                              0,
                                              IPMI_APP_GET_DEVICE_ID,
                                              NULL,
                                              0,
                                              TempData,
                                              &DataSize
                                              )
                    )
         )
  {
    DEBUG (
           (DEBUG_ERROR, "[IPMI] BMC does not respond by Get BMC DID (status: %r), %d retries left, ResponseData: 0x%lx\n",
            Status, Retries, TempData)
           );

    if (Retries-- == 0) {
      IpmiInstance->BmcStatus = BMC_HARDFAIL;
      return Status;
    }

    //
    // Handle the case that BMC FW still not enable KCS channel after AC cycle. just stall 1 second
    //
    MicroSecondDelay (1*1000*1000);
  }

  pBmcInfo = (SM_CTRL_INFO *)&TempData[0];
  DEBUG ((DEBUG_INFO, "[IPMI] BMC Device ID: 0x%02X, firmware version: %d.%02X UpdateMode:%x\n", pBmcInfo->DeviceId, pBmcInfo->MajorFirmwareRev, pBmcInfo->MinorFirmwareRev, pBmcInfo->UpdateMode));
  //
  // In OpenBMC, UpdateMode: the bit 7 of byte 4 in get device id command is used for the BMC status:
  // 0 means BMC is ready, 1 means BMC is not ready.
  // At the very beginning of BMC power on, the status is 1 means BMC is in booting process and not ready. It is not the flag for force update mode.
  //
  if (pBmcInfo->UpdateMode == BMC_READY) {
    mIpmiInstance->BmcStatus = BMC_OK;
    return EFI_SUCCESS;
  } else {
    DataSize = sizeof (TempData);
    Status   = IpmiSendCommand (
                                &IpmiInstance->IpmiTransport,
                                IPMI_NETFN_FIRMWARE,
                                0,
                                IPMI_GET_BMC_EXECUTION_CONTEXT,
                                NULL,
                                0,
                                TempData,
                                &DataSize
                                );

    pBmcExecContext = (IPMI_MSG_GET_BMC_EXEC_RSP *)&TempData[0];
    DEBUG ((DEBUG_INFO, "[IPMI] Operational status of BMC: 0x%x\n", pBmcExecContext->CurrentExecutionContext));
    if ((pBmcExecContext->CurrentExecutionContext == IPMI_BMC_IN_FORCED_UPDATE_MODE) &&
        !EFI_ERROR (Status))
    {
      DEBUG ((DEBUG_ERROR, "[IPMI] BMC in Forced Update mode, skip waiting for BMC_READY.\n"));
      IpmiInstance->BmcStatus = BMC_UPDATE_IN_PROGRESS;
    } else {
      //
      // Updatemode = 1 mean BMC is not ready, continue waiting.
      //
      while (Retries-- != 0) {
        MicroSecondDelay (1*1000*1000); // delay 1 seconds
        DEBUG ((DEBUG_INFO, "[IPMI] UpdateMode Retries: %d \n", Retries));
        DataSize = sizeof (TempData);
        Status   = IpmiSendCommand (
                                    &IpmiInstance->IpmiTransport,
                                    IPMI_NETFN_APP,
                                    0,
                                    IPMI_APP_GET_DEVICE_ID,
                                    NULL,
                                    0,
                                    TempData,
                                    &DataSize
                                    );

        if (!EFI_ERROR (Status)) {
          pBmcInfo = (SM_CTRL_INFO *)&TempData[0];
          DEBUG ((DEBUG_ERROR, "[IPMI] UpdateMode Retries: %d   pBmcInfo->UpdateMode:%x, Status: %r, Response Data: 0x%lx\n", Retries, pBmcInfo->UpdateMode, Status, TempData));
          if (pBmcInfo->UpdateMode == BMC_READY) {
            mIpmiInstance->BmcStatus = BMC_OK;
            return EFI_SUCCESS;
          }
        }
      }

      mIpmiInstance->BmcStatus = BMC_HARDFAIL;
    }
  }

  return Status;
} // GetDeviceId()

/*++

Routine Description:
  Initialize the API and parameters for IPMI Transport2 Instance

Arguments:
  IpmiInstance - Pointer to IPMI Instance.

Returns:
  VOID

--*/
VOID
InitIpmiTransport2 (
  IN  IPMI_BMC_INSTANCE_DATA  *IpmiInstance
  )
{
  IpmiInstance->IpmiTransport2.InterfaceType           = FixedPcdGet8 (PcdDefaultSystemInterface);
  IpmiInstance->IpmiTransport2.IpmiTransport2BmcStatus = BmcStatusOk;
  IpmiInstance->IpmiTransport2.IpmiSubmitCommand2      = IpmiSendCommand2;
  IpmiInstance->IpmiTransport2.IpmiSubmitCommand2Ex    = IpmiSendCommand2Ex;

  if (FixedPcdGet8 (PcdBtInterfaceSupport) == 1) {
    InitBtInterfaceData (&IpmiInstance->IpmiTransport2);
  }

  if (FixedPcdGet8 (PcdSsifInterfaceSupport) == 1) {
    InitSsifInterfaceData (&IpmiInstance->IpmiTransport2);
  }

  if (FixedPcdGet8 (PcdIpmbInterfaceSupport) == 1) {
    InitIpmbInterfaceData (&IpmiInstance->IpmiTransport2);
  }
}

/*++

Routine Description:
  Notify call back function.

Arguments:
  Event      - Event which caused this handler.
  Context    - Context passed during Event Handler registration.

Returns:
  VOID

--*/
VOID
EFIAPI
DxeNotifyCallback (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_STATUS            Status;
  IPMI_INTERFACE_STATE  InterfaceState;
  EFI_HANDLE            Handle;

  InterfaceState = IpmiInterfaceNotReady;

  if (FixedPcdGet8 (PcdSsifInterfaceSupport) == 1) {
    InitSsifInterfaceData (&mIpmiInstance->IpmiTransport2);

    if (mIpmiInstance->IpmiTransport2.Interface.Ssif.InterfaceState == IpmiInterfaceInitialized) {
      InterfaceState = IpmiInterfaceInitialized;
    }
  }

  if (FixedPcdGet8 (PcdIpmbInterfaceSupport) == 1) {
    InitIpmbInterfaceData (&mIpmiInstance->IpmiTransport2);

    if (mIpmiInstance->IpmiTransport2.Interface.Ipmb.InterfaceState == IpmiInterfaceInitialized) {
      InterfaceState = IpmiInterfaceInitialized;
    }
  }

  // Default Interface data should be initialized to install Ipmi Transport2 Protocol.
  if (InterfaceState != IpmiInterfaceInitialized) {
    return;
  }

  Handle = NULL;
  Status = gBS->InstallProtocolInterface (
                                          &Handle,
                                          &gIpmiTransport2ProtocolGuid,
                                          EFI_NATIVE_INTERFACE,
                                          &mIpmiInstance->IpmiTransport2
                                          );
  ASSERT_EFI_ERROR (Status);
}

/*++

Routine Description:
  Registers protocol notify call back.

Arguments:
  ProtocolGuid      - Pointer to Protocol Guid to register call back.

Returns:
  Status

--*/
EFI_STATUS
DxeRegisterProtocolCallback (
  IN EFI_GUID  *ProtocolGuid
  )
{
  EFI_STATUS  Status;
  EFI_EVENT   NotifyEvent;
  VOID        *Registration;

  if ((ProtocolGuid == NULL) ||
      ((ProtocolGuid != NULL) && IsZeroBuffer (ProtocolGuid, sizeof (EFI_GUID))))
  {
    return EFI_INVALID_PARAMETER;
  }

  Status = gBS->CreateEvent (
                             EVT_NOTIFY_SIGNAL,
                             TPL_NOTIFY,
                             DxeNotifyCallback,
                             NULL,
                             &NotifyEvent
                             );

  if (!EFI_ERROR (Status)) {
    Status = gBS->RegisterProtocolNotify (
                                          ProtocolGuid,
                                          NotifyEvent,
                                          &Registration
                                          );
  }

  return Status;
}

/**
  This function initializes KCS interface to BMC.

  Setup and initialize the BMC for the DXE phase.  In order to verify the BMC is functioning
  as expected, the BMC Selftest is performed.  The results are then checked and any errors are
  reported to the error manager.  Errors are collected throughout this routine and reported
  just prior to installing the driver.  If there are more errors than MAX_SOFT_COUNT, then they
  will be ignored.

  @param[in] ImageHandle - Handle of this driver image
  @param[in] SystemTable - Table containing standard EFI services

  @retval EFI_SUCCESS - Always success is returned even if KCS does not function
 **/
EFI_STATUS
InitializeIpmiKcsPhysicalLayer (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS             Status;
  UINT8                  ErrorCount;
  EFI_HANDLE             Handle;
  UINT8                  Index;
  IPMI_INTERFACE_STATE   InterfaceState = IpmiInterfaceNotReady;
  EFI_STATUS_CODE_VALUE  StatusCodeValue[MAX_SOFT_COUNT];

  ErrorCount   = 0;
  mImageHandle = ImageHandle;
  ZeroMem (StatusCodeValue, MAX_SOFT_COUNT);

  mIpmiInstance = AllocateZeroPool (sizeof (*mIpmiInstance));
  if (mIpmiInstance == NULL) {
    DEBUG ((DEBUG_ERROR, "ERROR!! Null Pointer returned by AllocateZeroPool ()\n"));
    ASSERT_EFI_ERROR (EFI_OUT_OF_RESOURCES);
    return EFI_OUT_OF_RESOURCES;
  } else {
    //
    // Calibrate TSC Counter.  Stall for 10ms, then multiply the resulting number of
    // ticks in that period by 100 to get the number of ticks in a 1 second timeout
    //

    //
    // Initialize the KCS transaction timeout.
    //
    mIpmiInstance->KcsTimeoutPeriod = (BMC_KCS_TIMEOUT * 1000*1000) / KCS_DELAY_UNIT;
    DEBUG ((DEBUG_INFO, "[IPMI] mIpmiInstance->KcsTimeoutPeriod: 0x%lx\n", mIpmiInstance->KcsTimeoutPeriod));

    //
    // Initialize IPMI IO Base.
    //
    mIpmiInstance->IpmiIoBase   = PcdGet16 (PcdIpmiIoBaseAddress);
    mIpmiInstance->Signature    = SM_IPMI_BMC_SIGNATURE;
    mIpmiInstance->SlaveAddress = BMC_SLAVE_ADDRESS;
    mIpmiInstance->BmcStatus    = BMC_NOTREADY;

    if (FixedPcdGet8 (PcdKcsInterfaceSupport) == 1) {
      mIpmiInstance->IpmiTransport.IpmiSubmitCommand = IpmiSendCommand;
      mIpmiInstance->IpmiTransport.GetBmcStatus      = IpmiGetBmcStatus;

      //
      // Get the Device ID and check if the system is in Force Update mode.
      //
      Status = GetDeviceId (
                            mIpmiInstance,
                            StatusCodeValue,
                            &ErrorCount
                            );
      //
      // Do not continue initialization if the BMC is in Force Update Mode.
      //
      if ((mIpmiInstance->BmcStatus != BMC_UPDATE_IN_PROGRESS) &&
          (mIpmiInstance->BmcStatus != BMC_HARDFAIL))
      {
        //
        // Get the SELF TEST Results.
        //
        Status = GetSelfTest (
                              mIpmiInstance,
                              StatusCodeValue,
                              &ErrorCount
                              );
      }

      //
      // iterate through the errors reporting them to the error manager.
      //
      for (Index = 0; Index < ErrorCount; Index++) {
        ReportStatusCode (
                          EFI_ERROR_CODE | EFI_ERROR_MAJOR,
                          StatusCodeValue[Index]
                          );
      }

      //
      // Now install the Protocol if the BMC is not in a HardFail State and not in Force Update mode
      //
      if ((mIpmiInstance->BmcStatus != BMC_HARDFAIL) && (mIpmiInstance->BmcStatus != BMC_UPDATE_IN_PROGRESS)) {
        Handle = NULL;
        Status = gBS->InstallProtocolInterface (
                                                &Handle,
                                                &gIpmiTransportProtocolGuid,
                                                EFI_NATIVE_INTERFACE,
                                                &mIpmiInstance->IpmiTransport
                                                );
        ASSERT_EFI_ERROR (Status);
      }
    }

    // Initialise the IPMI transport2
    InitIpmiTransport2 (mIpmiInstance);

    // Check interface data initialized successfully else register notify protocol.
    for (Index = SysInterfaceKcs; Index < SysInterfaceMax; Index++) {
      switch (Index) {
        case SysInterfaceKcs:
          if (FixedPcdGet8 (PcdKcsInterfaceSupport) == 1) {
            if ((mIpmiInstance->BmcStatus != BMC_HARDFAIL) && (mIpmiInstance->BmcStatus != BMC_UPDATE_IN_PROGRESS)) {
              BMC_INTERFACE_STATUS  BmcStatus;
              mIpmiInstance->IpmiTransport2.Interface.KcsInterfaceState = IpmiInterfaceInitialized;
              Status                                                    = CheckSelfTestByInterfaceType (
                                                                                                        &mIpmiInstance->IpmiTransport2,
                                                                                                        &BmcStatus,
                                                                                                        SysInterfaceKcs
                                                                                                        );
              if (!EFI_ERROR (Status) && (BmcStatus != BmcStatusHardFail)) {
                InterfaceState = IpmiInterfaceInitialized;
              } else {
                mIpmiInstance->IpmiTransport2.Interface.KcsInterfaceState = IpmiInterfaceInitError;
              }
            }
          }
          break;

        case SysInterfaceBt:
          if (FixedPcdGet8 (PcdBtInterfaceSupport) == 1) {
            if (mIpmiInstance->IpmiTransport2.Interface.Bt.InterfaceState == IpmiInterfaceInitialized) {
              InterfaceState = IpmiInterfaceInitialized;
            }
          }
          break;

        case SysInterfaceSsif:
          if (FixedPcdGet8 (PcdSsifInterfaceSupport) == 1) {
            if (mIpmiInstance->IpmiTransport2.Interface.Ssif.InterfaceState == IpmiInterfaceInitialized) {
              InterfaceState = IpmiInterfaceInitialized;
            } else if (mIpmiInstance->IpmiTransport2.Interface.Ssif.InterfaceState == IpmiInterfaceInitError) {
              // Register protocol notify for SMBUS Protocol.
              Status = DxeRegisterProtocolCallback (
                                                    &mIpmiInstance->IpmiTransport2.Interface.Ssif.SsifInterfaceApiGuid
                                                    );
            }
          }
          break;

        case SysInterfaceIpmb:
          if (FixedPcdGet8 (PcdIpmbInterfaceSupport) == 1) {
            if (mIpmiInstance->IpmiTransport2.Interface.Ipmb.InterfaceState == IpmiInterfaceInitialized) {
              InterfaceState = IpmiInterfaceInitialized;
            } else if (mIpmiInstance->IpmiTransport2.Interface.Ipmb.InterfaceState == IpmiInterfaceInitError) {
              // Register Protocol notify for I2C Protocol.
              Status = DxeRegisterProtocolCallback (
                                                    &mIpmiInstance->IpmiTransport2.Interface.Ipmb.IpmbInterfaceApiGuid
                                                    );
            }
          }
          break;

        default:
          break;
      }
    }

    // Any one of the Interface data should be initialized to install IPMI Transport2 Protocol.
    if (InterfaceState != IpmiInterfaceInitialized) {
      return EFI_SUCCESS;
    }

    Handle = NULL;
    Status = gBS->InstallProtocolInterface (
                                            &Handle,
                                            &gIpmiTransport2ProtocolGuid,
                                            EFI_NATIVE_INTERFACE,
                                            &mIpmiInstance->IpmiTransport2
                                            );
    ASSERT_EFI_ERROR (Status);
    return EFI_SUCCESS;
  }
} // InitializeIpmiKcsPhysicalLayer()
