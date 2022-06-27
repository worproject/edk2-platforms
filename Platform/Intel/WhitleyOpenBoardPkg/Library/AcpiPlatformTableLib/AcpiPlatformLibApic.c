/** @file
  ACPI Platform Driver Hooks

  @copyright
  Copyright 1996 - 2020 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

//
// Statements that include other files
//
#include "AcpiPlatformLibLocal.h"

extern EFI_PLATFORM_INFO            *mPlatformInfo;
extern BIOS_ACPI_PARAM              *mAcpiParameter;
extern EFI_ACPI_TABLE_PROTOCOL      *mAcpiTable;
EFI_IIO_UDS_PROTOCOL                *mIioUds = NULL;

extern UINT32                       mNumOfBitShift;

extern BOOLEAN                      mX2ApicEnabled;
BOOLEAN                             mCpuOrderSorted = FALSE;

extern CPU_CSR_ACCESS_VAR           *mCpuCsrAccessVarPtr;
EFI_MP_SERVICES_PROTOCOL            *mMpService;
CPU_ID_ORDER_MAP                    mCpuApicIdOrderTable[MAX_CPU_NUM];
UINTN                               mNumberOfCPUs = 0;
UINTN                               mNumberOfEnabledCPUs = 0;

UINT32                              mEnabledProcessor[MAX_SOCKET];

extern UINT32 mCpuPCPSInfo[MAX_SOCKET];
extern CPU_LOGICAL_THREAD_ID_TABLE  mCpuThreadIdMsrTable[MAX_CPU_NUM];


UINT32 mApicIdMap[MAX_SOCKET][MAX_CORE * MAX_THREAD];

UINT32 mThreadCount[MAX_SOCKET] = {0};

typedef struct {
  UINT64 EnableMask;
  UINT8  Id;
  UINT32 GsiBase;
} IO_APIC_DESCRIPTOR;

STATIC CONST IO_APIC_DESCRIPTOR IoApicDescList[] = {
  {PCH_IOAPIC,  PCH_IOAPIC_ID,  PCH_INTERRUPT_BASE},
  {PC00_IOAPIC, PC00_IOAPIC_ID, PC00_INTERRUPT_BASE},
  {PC01_IOAPIC, PC01_IOAPIC_ID, PC01_INTERRUPT_BASE},
  {PC02_IOAPIC, PC02_IOAPIC_ID, PC02_INTERRUPT_BASE},
  {PC03_IOAPIC, PC03_IOAPIC_ID, PC03_INTERRUPT_BASE},
  {PC04_IOAPIC, PC04_IOAPIC_ID, PC04_INTERRUPT_BASE},
  {PC05_IOAPIC, PC05_IOAPIC_ID, PC05_INTERRUPT_BASE},
  {PC06_IOAPIC, PC06_IOAPIC_ID, PC06_INTERRUPT_BASE},
  {PC07_IOAPIC, PC07_IOAPIC_ID, PC07_INTERRUPT_BASE},
  {PC08_IOAPIC, PC08_IOAPIC_ID, PC08_INTERRUPT_BASE},
  {PC09_IOAPIC, PC09_IOAPIC_ID, PC09_INTERRUPT_BASE},
  {PC10_IOAPIC, PC10_IOAPIC_ID, PC10_INTERRUPT_BASE},
  {PC11_IOAPIC, PC11_IOAPIC_ID, PC11_INTERRUPT_BASE},
  {PC12_IOAPIC, PC12_IOAPIC_ID, PC12_INTERRUPT_BASE},
  {PC13_IOAPIC, PC13_IOAPIC_ID, PC13_INTERRUPT_BASE},
  {PC14_IOAPIC, PC14_IOAPIC_ID, PC14_INTERRUPT_BASE},
  {PC15_IOAPIC, PC15_IOAPIC_ID, PC15_INTERRUPT_BASE},
  {PC16_IOAPIC, PC16_IOAPIC_ID, PC16_INTERRUPT_BASE},
  {PC17_IOAPIC, PC17_IOAPIC_ID, PC17_INTERRUPT_BASE},
  {PC18_IOAPIC, PC18_IOAPIC_ID, PC18_INTERRUPT_BASE},
  {PC19_IOAPIC, PC19_IOAPIC_ID, PC19_INTERRUPT_BASE},
  {PC20_IOAPIC, PC20_IOAPIC_ID, PC20_INTERRUPT_BASE},
  {PC21_IOAPIC, PC21_IOAPIC_ID, PC21_INTERRUPT_BASE},
  {PC22_IOAPIC, PC22_IOAPIC_ID, PC22_INTERRUPT_BASE},
  {PC23_IOAPIC, PC23_IOAPIC_ID, PC23_INTERRUPT_BASE},
  {PC24_IOAPIC, PC24_IOAPIC_ID, PC24_INTERRUPT_BASE},
  {PC25_IOAPIC, PC25_IOAPIC_ID, PC25_INTERRUPT_BASE},
  {PC26_IOAPIC, PC26_IOAPIC_ID, PC26_INTERRUPT_BASE},
  {PC27_IOAPIC, PC27_IOAPIC_ID, PC27_INTERRUPT_BASE},
  {PC28_IOAPIC, PC28_IOAPIC_ID, PC28_INTERRUPT_BASE},
  {PC29_IOAPIC, PC29_IOAPIC_ID, PC29_INTERRUPT_BASE},
  {PC30_IOAPIC, PC30_IOAPIC_ID, PC30_INTERRUPT_BASE},
  {PC31_IOAPIC, PC31_IOAPIC_ID, PC31_INTERRUPT_BASE},
  {PC32_IOAPIC, PC32_IOAPIC_ID, PC32_INTERRUPT_BASE},
  {PC33_IOAPIC, PC33_IOAPIC_ID, PC33_INTERRUPT_BASE},
  {PC34_IOAPIC, PC34_IOAPIC_ID, PC34_INTERRUPT_BASE},
  {PC35_IOAPIC, PC35_IOAPIC_ID, PC35_INTERRUPT_BASE},
  {PC36_IOAPIC, PC36_IOAPIC_ID, PC36_INTERRUPT_BASE},
  {PC37_IOAPIC, PC37_IOAPIC_ID, PC37_INTERRUPT_BASE},
  {PC38_IOAPIC, PC38_IOAPIC_ID, PC38_INTERRUPT_BASE},
  {PC39_IOAPIC, PC39_IOAPIC_ID, PC39_INTERRUPT_BASE},
  {PC40_IOAPIC, PC40_IOAPIC_ID, PC40_INTERRUPT_BASE},
  {PC41_IOAPIC, PC41_IOAPIC_ID, PC41_INTERRUPT_BASE},
  {PC42_IOAPIC, PC42_IOAPIC_ID, PC42_INTERRUPT_BASE},
  {PC43_IOAPIC, PC43_IOAPIC_ID, PC43_INTERRUPT_BASE},
  {PC44_IOAPIC, PC44_IOAPIC_ID, PC44_INTERRUPT_BASE},
  {PC45_IOAPIC, PC45_IOAPIC_ID, PC45_INTERRUPT_BASE},
  {PC46_IOAPIC, PC46_IOAPIC_ID, PC46_INTERRUPT_BASE},
  {PC47_IOAPIC, PC47_IOAPIC_ID, PC47_INTERRUPT_BASE}
};

/**
  Find OrderTable index which is matching input ApicId

  @param ApicId - input ApicId

  @retval Index       - OrderTable index
  @retval (UINT32) -1 - Not found
**/
UINT32
ApicId2OrderTableIndex (
  UINT32 ApicId
  )
{
  UINT32 Index;

  for (Index = 0; Index < MAX_CPU_NUM; Index++) {
    if ((mCpuApicIdOrderTable[Index].Flags == 1) && (mCpuApicIdOrderTable[Index].ApicId == ApicId)) {
      return Index;
    }
  }

  return (UINT32) -1;
}

/**
  Display reordered Apic table for detected CPUs.

  @param None

  @retval None
**/
VOID
DebugDisplayReOrderTable (
  VOID
  )
{
  UINT32 Index;
  UINT32 TotalEnabledThreads = 0;

  DEBUG ((DEBUG_INFO, "Index  AcpiProcId  ApicId  Flags  Skt\n"));
  for (Index = 0; Index < MAX_CPU_NUM; Index++) {
    if (mCpuApicIdOrderTable[Index].ApicId == (UINT32) -1) {
      break;
    }
    TotalEnabledThreads++;
    DEBUG ((DEBUG_INFO, " %02d       0x%02X      0x%02X      %d      %d\n",
                           Index, mCpuApicIdOrderTable[Index].AcpiProcessorId,
                           mCpuApicIdOrderTable[Index].ApicId,
                           mCpuApicIdOrderTable[Index].Flags,
                           mCpuApicIdOrderTable[Index].SocketNum));
  }
  DEBUG ((DEBUG_INFO, "\n:ACPI: Total Enabled Threads = %d\n\n", TotalEnabledThreads));
}

/**
  Consolidate APIC ID order for all populated socket in mApicIdMap table

  @param None

  @retval None
**/
VOID
UpdateApicIdMap (
  VOID
  )
{
  UINT32 SocketId;
  UINT32 ThreadIndex;
  UINT32 CurrProcessor;

  //
  // init global mApicIdMap variable
  //
  SetMem32 ((VOID *)mApicIdMap, sizeof(mApicIdMap), 0xFFFFFFFF);

  for (SocketId = 0; SocketId < MAX_SOCKET; SocketId++) {
    if ((mCpuCsrAccessVarPtr->socketPresentBitMap & (1 << SocketId)) == 0) {
      continue;
    }

    ThreadIndex = 0;
    for (CurrProcessor = 0; CurrProcessor < MAX_CPU_NUM; CurrProcessor++) {
      if (mCpuApicIdOrderTable[CurrProcessor].ApicId == (UINT32) -1) {
        break;
      }

      if ((mCpuApicIdOrderTable[CurrProcessor].SocketNum == SocketId) && (ThreadIndex < (MAX_CORE * MAX_THREAD))) {
        mApicIdMap[SocketId][ThreadIndex] = (UINT32) mCpuApicIdOrderTable[CurrProcessor].ApicId & (UINT32) ~(SocketId<< mNumOfBitShift);
        ThreadIndex++;
      }
    }

    if (ThreadIndex != mThreadCount[SocketId]) {
      DEBUG((DEBUG_ERROR, ":: Skt: %d - Enabled ThreadCount is incorrect!!!\n"));
      break;
    }
  }

  return;
}

/**
  Find the processor index of the thread running and obtain MSR 53 and ApicId data
  to populate mCpuThreadIdMsrTable array.

  @param None

  @retval None
**/
VOID
GetThreadIdMsrValue (
  VOID
  )
{
  UINTN     ProcessorNumber;
  UINT64    LogicalIdMsrValue;

  mMpService->WhoAmI (mMpService, &ProcessorNumber);
  LogicalIdMsrValue = AsmReadMsr64 (0x00000053);
  mCpuThreadIdMsrTable[ProcessorNumber].ThreadIdValue = (UINT32) LogicalIdMsrValue;
  mCpuThreadIdMsrTable[ProcessorNumber].CollocatedChaId = (UINT32) RShiftU64(LogicalIdMsrValue, 32);
  mCpuThreadIdMsrTable[ProcessorNumber].ApicId = mCpuConfigLibConfigContextBuffer->CollectedDataBuffer[ProcessorNumber].CpuMiscData.ApicID;
}

/**
  Sort CPU Local APIC Information.

  This function gets the CPU local APIC information from the MP service
  protocol into the local table structure, and sorts it based on APIC ID.

  @retval EFI_SUCCESS   Local APIC information was successfully sorted.
**/
EFI_STATUS
SortCpuLocalApicInTable (
  VOID
  )
{
  EFI_STATUS                                Status;
  EFI_PROCESSOR_INFORMATION                 ProcessorInfoBuffer;
  UINTN                                     BufferSize;
  UINT32                                    Index;
  UINT32                                    Socket;
  UINT32                                    CurrProcessor;
  CPU_ID_ORDER_MAP                          *CpuIdMapPtr;
  UINT32                                    CoreThreadMask;
  UINT32                                    CoreThreadCount;
  UINT32                                    BspApicId;
  UINT32                                    LowestApicId;
  UINT32                                    CurrentIndex;
  UINT32                                    TargetIndex;
  CPU_ID_ORDER_MAP                          TempEntry;
  BOOLEAN                                   SecondThreadExistent;

  BufferSize = 0;
  Index      = 0;
  Status     = EFI_SUCCESS;

  CoreThreadMask = (UINT32) ((1 << mNumOfBitShift) - 1);
  CoreThreadCount = 0;

  if (!mCpuOrderSorted) {
    //
    // Init ProcessorBitMask table and EnabledProcessor table
    //
    for (Index = 0; Index < MAX_SOCKET; Index++) {
      mAcpiParameter->ProcessorBitMask[Index] = 0;
      mAcpiParameter->ProcessorBitMaskHi[Index] = 0;
      mEnabledProcessor[Index] = 0;
      mCpuPCPSInfo[Index] = 0;
    }

    Index  = 0;
    SecondThreadExistent = FALSE;
    for (CurrProcessor = 0; CurrProcessor < mNumberOfCPUs; CurrProcessor++) {
      Status = mMpService->GetProcessorInfo (
        mMpService,
        CurrProcessor,
        &ProcessorInfoBuffer
        );
      ASSERT_EFI_ERROR (Status);
      if (EFI_ERROR (Status)) {
        continue;
      }


      if (ProcessorInfoBuffer.ProcessorId & 1) {  // secondary thread
        CpuIdMapPtr = &mCpuApicIdOrderTable[(Index - 1) + MAX_CPU_NUM / 2];
        SecondThreadExistent= TRUE;
      } else {                                    // primary thread
        CpuIdMapPtr = &mCpuApicIdOrderTable[Index];
        Index++;
      }

      CpuIdMapPtr->ApicId  = (UINT32) ProcessorInfoBuffer.ProcessorId;
      CpuIdMapPtr->Flags   = ((ProcessorInfoBuffer.StatusFlag & PROCESSOR_ENABLED_BIT) != 0);
      CpuIdMapPtr->SocketNum = (UINT32) ProcessorInfoBuffer.Location.Package;
      CpuIdMapPtr->AcpiProcessorId = (CpuIdMapPtr->SocketNum << mNumOfBitShift) + mThreadCount[CpuIdMapPtr->SocketNum];

      mThreadCount[CpuIdMapPtr->SocketNum]++;

      if (CpuIdMapPtr->Flags == 1) {
        //
        // Update EnabledProcessor table
        //
        mEnabledProcessor[CpuIdMapPtr->SocketNum]++;
        mCpuPCPSInfo[CpuIdMapPtr->SocketNum]++; // count enabled processor in current socket

        //
        // Update processorbitMask
        //
        if (ProcessorInfoBuffer.Location.Core < 64) {
          mAcpiParameter->ProcessorBitMask[CpuIdMapPtr->SocketNum] |=
            LShiftU64 (1, ProcessorInfoBuffer.Location.Core);
        } else {
          mAcpiParameter->ProcessorBitMaskHi[CpuIdMapPtr->SocketNum] |=
            LShiftU64 (1, ProcessorInfoBuffer.Location.Core - 64);
        }

        if (ProcessorInfoBuffer.Location.Thread >= CoreThreadCount) {
          CoreThreadCount = ProcessorInfoBuffer.Location.Thread + 1;
        }
      }
    } //end for CurrentProcessor

    DEBUG ((
      DEBUG_INFO,
      "::ACPI::  APIC ID Order Table Init.   CoreThreadMask = 0x%x,   mNumOfBitShift = %d,   CoreThreadCount = %d\n",
      CoreThreadMask,
      mNumOfBitShift,
      CoreThreadCount
      ));

    DEBUG ((DEBUG_INFO, "::ACPI::  Socket  ProcessorBitMaskHi ProcessorBitMask   ProcessorApicIdBase\n"));
    for (Index = 0; Index < MAX_SOCKET; Index++) {
      DEBUG ((
        DEBUG_INFO,
        "::ACPI::    %d     0x%016lx %016lx          0x%x\n",
        Index,
        mAcpiParameter->ProcessorBitMaskHi[Index],
        mAcpiParameter->ProcessorBitMask[Index],
        mAcpiParameter->ProcessorApicIdBase[Index]
        ));
    }
    AsmCpuidEx (CPUID_EXTENDED_TOPOLOGY, 1, NULL, NULL, NULL, &BspApicId);

    if (mCpuApicIdOrderTable[0].ApicId != BspApicId) {
      //
      // check to see if 1st entry is BSP, if not swap it
      //
      CurrentIndex = ApicId2OrderTableIndex (BspApicId);

      if (CurrentIndex >= MAX_CPU_NUM) {
        DEBUG ((DEBUG_ERROR, "BSP index is out of range\n"));
        ASSERT (CurrentIndex < MAX_CPU_NUM);
      } else {
        LowestApicId = mCpuApicIdOrderTable[0].ApicId;
        TargetIndex = 0;
        CopyMem (&TempEntry, &mCpuApicIdOrderTable[CurrentIndex], sizeof(TempEntry));
        CopyMem (&mCpuApicIdOrderTable[CurrentIndex], &mCpuApicIdOrderTable[TargetIndex], sizeof(TempEntry));
        CopyMem (&mCpuApicIdOrderTable[TargetIndex], &TempEntry, sizeof(TempEntry));

        if (SecondThreadExistent) {
          //
          // Also swap BSP's companion thread (the second thread in same core of BSP)
          //
          CurrentIndex = ApicId2OrderTableIndex (BspApicId + 1);
          TargetIndex = ApicId2OrderTableIndex (LowestApicId + 1);
          if ((CurrentIndex < MAX_CPU_NUM) && (TargetIndex < MAX_CPU_NUM)) {
            CopyMem (&TempEntry, &mCpuApicIdOrderTable[CurrentIndex], sizeof(TempEntry));
            CopyMem (&mCpuApicIdOrderTable[CurrentIndex], &mCpuApicIdOrderTable[TargetIndex], sizeof(TempEntry));
            CopyMem (&mCpuApicIdOrderTable[TargetIndex], &TempEntry, sizeof(TempEntry));
          }
        }
      }
    }

    //
    // Make sure no holes between enabled threads
    //
    for (CurrProcessor = 0; CurrProcessor < MAX_CPU_NUM; CurrProcessor++) {

      if (mCpuApicIdOrderTable[CurrProcessor].Flags == 0) {
        //
        // make sure disabled entry has ProcId set to FFs
        //
        mCpuApicIdOrderTable[CurrProcessor].ApicId = (UINT32) -1;
        mCpuApicIdOrderTable[CurrProcessor].AcpiProcessorId = (UINT32) -1;

        for (Index = CurrProcessor + 1; Index < MAX_CPU_NUM; Index++) {
          if (mCpuApicIdOrderTable[Index].Flags == 1) {
            //
            // move enabled entry up
            //
            mCpuApicIdOrderTable[CurrProcessor].Flags = 1;
            mCpuApicIdOrderTable[CurrProcessor].ApicId = mCpuApicIdOrderTable[Index].ApicId;
            mCpuApicIdOrderTable[CurrProcessor].AcpiProcessorId = mCpuApicIdOrderTable[Index].AcpiProcessorId;
            mCpuApicIdOrderTable[CurrProcessor].SocketNum = mCpuApicIdOrderTable[Index].SocketNum;
            //
            // disable moved entry
            //
            mCpuApicIdOrderTable[Index].Flags = 0;
            mCpuApicIdOrderTable[Index].ApicId = (UINT32) -1;
            mCpuApicIdOrderTable[Index].AcpiProcessorId = (UINT32) -1;
            break;
          }
        }
      }
    }

    //
    // keep for debug purpose
    //
    DEBUG ((DEBUG_INFO, "APIC ID Order Table ReOrdered\n"));
    DebugDisplayReOrderTable ();

    //
    // Re-sort AcpiProcessorId for all sockets
    //
    for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
      Index  = 0;

      for (CurrProcessor = 0; CurrProcessor < MAX_CPU_NUM; CurrProcessor++) {
        if (mCpuApicIdOrderTable[CurrProcessor].Flags && (mCpuApicIdOrderTable[CurrProcessor].SocketNum == Socket)) {
          //
          // re-assign AcpiProcessorId to match MADT (socket, thread)
          //
          mCpuApicIdOrderTable[CurrProcessor].AcpiProcessorId = (Socket << mNumOfBitShift) + Index;
          Index++;
        }
      }
    }

    //
    // keep for debug purpose
    //
    DEBUG ((DEBUG_INFO, "APIC ID Ord Tbl ReOrd aft Re-sort AcpiProcessorId\n"));
    DebugDisplayReOrderTable ();

    //
    // Update mApicIdMap according the final mCpuApicIdOrderTable
    //
    UpdateApicIdMap ();

    for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    for (Index=0; Index < (MAX_THREAD * MAX_CORE); Index++) {
        if (mApicIdMap[Socket][Index] != (UINT32) -1) {
          DEBUG ((DEBUG_INFO, "mApicIdMap[%d][%d] = 0x%x\n", Socket, Index, mApicIdMap[Socket][Index]));
        }
      }
    }

    //
    // Initialize with safe defaults
    //
    for(CurrProcessor = 0; CurrProcessor < MAX_CPU_NUM; CurrProcessor++) {
      mCpuThreadIdMsrTable[CurrProcessor].ApicId = (UINT32)-1;
      mCpuThreadIdMsrTable[CurrProcessor].ThreadIdValue = 0xFF;
      mCpuThreadIdMsrTable[CurrProcessor].CollocatedChaId = 0xFF;
      mCpuThreadIdMsrTable[CurrProcessor].SNCProximityDomain = 0;
    }
    //
    // Collect MSR 53 value and ApicId for each thread
    //
    mMpService->StartupAllAPs (mMpService, (EFI_AP_PROCEDURE)GetThreadIdMsrValue, FALSE, NULL, 0, NULL, NULL);
    GetThreadIdMsrValue ();

    mCpuOrderSorted = TRUE;
  }

  return Status;
}

/**
  Build from scratch and install the MADT.

  @retval EFI_SUCCESS           The MADT was installed successfully.
  @retval EFI_OUT_OF_RESOURCES  Could not allocate required structures.
**/
EFI_STATUS
InstallMadtFromScratch (
  VOID
  )
{
  EFI_STATUS                                          Status;
  UINTN                                               Index;
  EFI_ACPI_6_2_MULTIPLE_APIC_DESCRIPTION_TABLE_HEADER *NewMadtTable;
  UINTN                                               TableHandle;
  EFI_ACPI_6_2_MULTIPLE_APIC_DESCRIPTION_TABLE_HEADER MadtTableHeader;
  EFI_ACPI_6_2_PROCESSOR_LOCAL_APIC_STRUCTURE         ProcLocalApicStruct;
  EFI_ACPI_6_2_IO_APIC_STRUCTURE                      IoApicStruct;
  EFI_ACPI_6_2_INTERRUPT_SOURCE_OVERRIDE_STRUCTURE    IntSrcOverrideStruct;
  EFI_ACPI_6_2_LOCAL_APIC_NMI_STRUCTURE               LocalApciNmiStruct;
  EFI_ACPI_6_2_PROCESSOR_LOCAL_X2APIC_STRUCTURE       ProcLocalX2ApicStruct;
  EFI_ACPI_6_2_LOCAL_X2APIC_NMI_STRUCTURE             LocalX2ApicNmiStruct;
  STRUCTURE_HEADER                                    **MadtStructs;
  UINTN                                               MaxMadtStructCount;
  UINTN                                               MadtStructsIndex;

  NewMadtTable = NULL;

  MaxMadtStructCount = (UINT32) (
    MAX_CPU_NUM +         // processor local APIC structures
    MAX_CPU_NUM +         // processor local x2APIC structures
    MAX_IO_APICS_10NM +   // IOAPIC structures
    2 +                   // interrupt source override structures
    1 +                   // local APIC NMI structures
    1                     // local x2APIC NMI structures
  );                      // other structures are not used

  MadtStructs = (STRUCTURE_HEADER **) AllocateZeroPool (MaxMadtStructCount * sizeof (STRUCTURE_HEADER *));
  if (MadtStructs == NULL) {
    DEBUG ((DEBUG_ERROR, "[ACPI](MADT) Could not allocate MADT structure pointer array\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Initialize the next index into the structure pointer array. It is
  // incremented every time a structure of any type is copied to the array.
  //
  MadtStructsIndex = 0;

  //
  // Initialize MADT Header Structure
  //
  Status = InitializeMadtHeader (&MadtTableHeader);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[ACPI](MADT) InitializeMadtHeader failed: %r\n", Status));
    goto Done;
  }

  DEBUG ((DEBUG_INFO, "[ACPI](MADT) Number of CPUs detected = %d \n", mNumberOfCPUs));

  //
  // Build Processor Local APIC Structures and Processor Local X2APIC Structures
  //
  ProcLocalApicStruct.Type = EFI_ACPI_6_2_PROCESSOR_LOCAL_APIC;
  ProcLocalApicStruct.Length = sizeof (EFI_ACPI_6_2_PROCESSOR_LOCAL_APIC_STRUCTURE);

  ProcLocalX2ApicStruct.Type = EFI_ACPI_6_2_PROCESSOR_LOCAL_X2APIC;
  ProcLocalX2ApicStruct.Length = sizeof (EFI_ACPI_6_2_PROCESSOR_LOCAL_X2APIC_STRUCTURE);
  ProcLocalX2ApicStruct.Reserved[0] = 0;
  ProcLocalX2ApicStruct.Reserved[1] = 0;

  for (Index = 0; Index < mNumberOfCPUs; Index++) {
    //
    // If x2APIC mode is not enabled, and if it is possible to express the
    // APIC ID as a UINT8, use a processor local APIC structure. Otherwise,
    // use a processor local x2APIC structure.
    //
    if (!mX2ApicEnabled && mCpuApicIdOrderTable[Index].ApicId < MAX_UINT8) {
      ProcLocalApicStruct.Flags           = (UINT8) mCpuApicIdOrderTable[Index].Flags;
      ProcLocalApicStruct.ApicId          = (UINT8) mCpuApicIdOrderTable[Index].ApicId;
      ProcLocalApicStruct.AcpiProcessorUid = (UINT8) mCpuApicIdOrderTable[Index].AcpiProcessorId;

      ASSERT (MadtStructsIndex < MaxMadtStructCount);
      Status = CopyStructure (
        &MadtTableHeader.Header,
        (STRUCTURE_HEADER *) &ProcLocalApicStruct,
        &MadtStructs[MadtStructsIndex++]
        );
    } else {
      ProcLocalX2ApicStruct.Flags            = (UINT8) mCpuApicIdOrderTable[Index].Flags;
      ProcLocalX2ApicStruct.X2ApicId         = mCpuApicIdOrderTable[Index].ApicId;
      ProcLocalX2ApicStruct.AcpiProcessorUid = mCpuApicIdOrderTable[Index].AcpiProcessorId;

      ASSERT (MadtStructsIndex < MaxMadtStructCount);
      Status = CopyStructure (
        &MadtTableHeader.Header,
        (STRUCTURE_HEADER *) &ProcLocalX2ApicStruct,
        &MadtStructs[MadtStructsIndex++]
        );
    }
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "[ACPI](MADT) CopyMadtStructure (local APIC/x2APIC) failed: %r\n", Status));
      goto Done;
    }
  }

  //
  // Build IOAPIC Structures
  //
  if (mIioUds == NULL) {
    Status = gBS->LocateProtocol (&gEfiIioUdsProtocolGuid, NULL, (VOID **) &mIioUds);
    ASSERT_EFI_ERROR (Status);
    if (EFI_ERROR (Status)) {
      goto Done;
    }
  }

  IoApicStruct.Type = EFI_ACPI_6_2_IO_APIC;
  IoApicStruct.Length = sizeof (EFI_ACPI_6_2_IO_APIC_STRUCTURE);
  IoApicStruct.Reserved = 0;
  IoApicStruct.IoApicId = PCH_IOAPIC_ID;
  IoApicStruct.IoApicAddress =  mIioUds->IioUdsPtr->PlatformData.IIO_resource[0].StackRes[0].IoApicBase;
  IoApicStruct.GlobalSystemInterruptBase = PCH_INTERRUPT_BASE;
  ASSERT (MadtStructsIndex < MaxMadtStructCount);
  Status = CopyStructure (
    &MadtTableHeader.Header,
    (STRUCTURE_HEADER *) &IoApicStruct,
    &MadtStructs[MadtStructsIndex++]
    );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[ACPI](MADT) CopyMadtStructure (IOAPIC) failed: %r\n", Status));
    goto Done;
  }
  DEBUG ((DEBUG_INFO, "[ACPI](MADT) Add IOAPIC id %d, addr 0x%x, Interrupt base %d for PCH\n", IoApicStruct.IoApicId, IoApicStruct.IoApicAddress, IoApicStruct.GlobalSystemInterruptBase));

  //
  // Build Interrupt Source Override Structures
  //
  IntSrcOverrideStruct.Type = EFI_ACPI_6_2_INTERRUPT_SOURCE_OVERRIDE;
  IntSrcOverrideStruct.Length = sizeof (EFI_ACPI_6_2_INTERRUPT_SOURCE_OVERRIDE_STRUCTURE);

  //
  // IRQ0=>IRQ2 Interrupt Source Override Structure
  //
  IntSrcOverrideStruct.Bus = 0x0;                   // Bus - ISA
  IntSrcOverrideStruct.Source = 0x0;                // Source - IRQ0
  IntSrcOverrideStruct.GlobalSystemInterrupt = 0x2; // Global System Interrupt - IRQ2
  IntSrcOverrideStruct.Flags = 0x0;                 // Flags - Conforms to specifications of the bus

  ASSERT (MadtStructsIndex < MaxMadtStructCount);
  Status = CopyStructure (
    &MadtTableHeader.Header,
    (STRUCTURE_HEADER *) &IntSrcOverrideStruct,
    &MadtStructs[MadtStructsIndex++]
    );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[ACPI](MADT) CopyMadtStructure (IRQ2 source override) failed: %r\n", Status));
    goto Done;
  }

  //
  // IRQ9 (SCI Active High) Interrupt Source Override Structure
  //
  IntSrcOverrideStruct.Bus = 0x0;                   // Bus - ISA
  IntSrcOverrideStruct.Source = 0x9;                // Source - IRQ9
  IntSrcOverrideStruct.GlobalSystemInterrupt = 0x9; // Global System Interrupt - IRQ9
  IntSrcOverrideStruct.Flags = 0xD;                 // Flags - Level-tiggered, Active High

  ASSERT (MadtStructsIndex < MaxMadtStructCount);
  Status = CopyStructure (
    &MadtTableHeader.Header,
    (STRUCTURE_HEADER *) &IntSrcOverrideStruct,
    &MadtStructs[MadtStructsIndex++]
    );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[ACPI](MADT) CopyMadtStructure (IRQ9 source override) failed: %r\n", Status));
    goto Done;
  }

  //
  // Build Local APIC NMI Structures
  //
  LocalApciNmiStruct.Type   = EFI_ACPI_6_2_LOCAL_APIC_NMI;
  LocalApciNmiStruct.Length = sizeof (EFI_ACPI_6_2_LOCAL_APIC_NMI_STRUCTURE);
  LocalApciNmiStruct.AcpiProcessorUid = 0xFF;      // Applies to all processors
  LocalApciNmiStruct.Flags  = POLARITY_ACTIVE_HIGH | TRIGGERMODE_EDGE; // Flags - Edge-tiggered, Active High
  LocalApciNmiStruct.LocalApicLint   = 0x1;

  ASSERT (MadtStructsIndex < MaxMadtStructCount);
  Status = CopyStructure (
    &MadtTableHeader.Header,
    (STRUCTURE_HEADER *) &LocalApciNmiStruct,
    &MadtStructs[MadtStructsIndex++]
    );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[ACPI](MADT) CopyMadtStructure (APIC NMI) failed: %r\n", Status));
    goto Done;
  }

  //
  // Build Local x2APIC NMI Structure
  //
  LocalX2ApicNmiStruct.Type   = EFI_ACPI_6_2_LOCAL_X2APIC_NMI;
  LocalX2ApicNmiStruct.Length = sizeof (EFI_ACPI_6_2_LOCAL_X2APIC_NMI_STRUCTURE);
  LocalX2ApicNmiStruct.Flags  = POLARITY_ACTIVE_HIGH | TRIGGERMODE_EDGE; // Flags - Edge-tiggered, Active High
  LocalX2ApicNmiStruct.AcpiProcessorUid = 0xFFFFFFFF;  // Applies to all processors
  LocalX2ApicNmiStruct.LocalX2ApicLint  = 0x01;
  LocalX2ApicNmiStruct.Reserved[0] = 0x00;
  LocalX2ApicNmiStruct.Reserved[1] = 0x00;
  LocalX2ApicNmiStruct.Reserved[2] = 0x00;

  ASSERT (MadtStructsIndex < MaxMadtStructCount);
  Status = CopyStructure (
    &MadtTableHeader.Header,
    (STRUCTURE_HEADER *) &LocalX2ApicNmiStruct,
    &MadtStructs[MadtStructsIndex++]
    );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[ACPI](MADT) CopyMadtStructure (x2APIC NMI) failed: %r\n", Status));
    goto Done;
  }

  //
  // Build Madt Structure from the Madt Header and collection of pointers in MadtStructs[]
  //
  Status = BuildAcpiTable (
    (EFI_ACPI_DESCRIPTION_HEADER *) &MadtTableHeader,
    sizeof (EFI_ACPI_6_2_MULTIPLE_APIC_DESCRIPTION_TABLE_HEADER),
    MadtStructs,
    MadtStructsIndex,
    (UINT8 **)&NewMadtTable
    );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[ACPI](MADT) BuildAcpiTable failed: %r\n", Status));
    goto Done;
  }

  //
  // Publish Madt Structure to ACPI
  //
  Status = mAcpiTable->InstallAcpiTable (
    mAcpiTable,
    NewMadtTable,
    NewMadtTable->Header.Length,
    &TableHandle
    );

Done:
  //
  // Free memory
  //
  for (MadtStructsIndex = 0; MadtStructsIndex < MaxMadtStructCount; MadtStructsIndex++) {
    if (MadtStructs[MadtStructsIndex] != NULL) {
      FreePool (MadtStructs[MadtStructsIndex]);
    }
  }

  FreePool (MadtStructs);

  if (NewMadtTable != NULL) {
    FreePool (NewMadtTable);
  }

  return Status;
}
