/** @file
  This file contains definitions required for creation of
  Memory S3 Save data, Memory Info data and Memory Platform
  data hobs.

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _MEM_INFO_HOB_H_
#define _MEM_INFO_HOB_H_


#pragma pack (push, 1)

extern EFI_GUID gSiMemoryS3DataGuid;
extern EFI_GUID gSiMemoryInfoDataGuid;
extern EFI_GUID gSiMemoryPlatformDataGuid;

#define MAX_NODE        2
#define MAX_CH          4
#define MAX_DIMM        2
#define HOB_MAX_SAGV_POINTS 4

///
/// Defines taken from MRC so avoid having to include MrcInterface.h
///

//
// Matches MAX_SPD_SAVE define in MRC
//
#ifndef MAX_SPD_SAVE
#define MAX_SPD_SAVE 29
#endif

//
// MRC version description.
//
typedef struct {
  UINT8  Major;     ///< Major version number
  UINT8  Minor;     ///< Minor version number
  UINT8  Rev;       ///< Revision number
  UINT8  Build;     ///< Build number
} SiMrcVersion;

//
// Matches MrcChannelSts enum in MRC
//
#ifndef CHANNEL_NOT_PRESENT
#define CHANNEL_NOT_PRESENT     0  // There is no channel present on the controller.
#endif
#ifndef CHANNEL_DISABLED
#define CHANNEL_DISABLED        1  // There is a channel present but it is disabled.
#endif
#ifndef CHANNEL_PRESENT
#define CHANNEL_PRESENT         2  // There is a channel present and it is enabled.
#endif

//
// Matches MrcDimmSts enum in MRC
//
#ifndef DIMM_ENABLED
#define DIMM_ENABLED     0  // DIMM/rank Pair is enabled, presence will be detected.
#endif
#ifndef DIMM_DISABLED
#define DIMM_DISABLED    1  // DIMM/rank Pair is disabled, regardless of presence.
#endif
#ifndef DIMM_PRESENT
#define DIMM_PRESENT     2  // There is a DIMM present in the slot/rank pair and it will be used.
#endif
#ifndef DIMM_NOT_PRESENT
#define DIMM_NOT_PRESENT 3  // There is no DIMM present in the slot/rank pair.
#endif

//
// Matches MrcDdrType enum in MRC
//
#ifndef MRC_DDR_TYPE_DDR5
#define MRC_DDR_TYPE_DDR5     1
#endif
#ifndef MRC_DDR_TYPE_LPDDR5
#define MRC_DDR_TYPE_LPDDR5   2
#endif
#ifndef MRC_DDR_TYPE_LPDDR4
#define MRC_DDR_TYPE_LPDDR4   3
#endif
#ifndef MRC_DDR_TYPE_UNKNOWN
#define MRC_DDR_TYPE_UNKNOWN  4
#endif

#define MAX_PROFILE_NUM     7 // number of memory profiles supported
#define MAX_XMP_PROFILE_NUM 5 // number of XMP profiles supported

//
// DIMM timings
//
typedef struct {
  UINT32 tCK;       ///< Memory cycle time, in femtoseconds.
  UINT16 NMode;     ///< Number of tCK cycles for the channel DIMM's command rate mode.
  UINT16 tCL;       ///< Number of tCK cycles for the channel DIMM's CAS latency.
  UINT16 tCWL;      ///< Number of tCK cycles for the channel DIMM's minimum CAS write latency time.
  UINT16 tFAW;      ///< Number of tCK cycles for the channel DIMM's minimum four activate window delay time.
  UINT16 tRAS;      ///< Number of tCK cycles for the channel DIMM's minimum active to precharge delay time.
  UINT16 tRCDtRP;   ///< Number of tCK cycles for the channel DIMM's minimum RAS# to CAS# delay time and Row Precharge delay time.
  UINT16 tREFI;     ///< Number of tCK cycles for the channel DIMM's minimum Average Periodic Refresh Interval.
  UINT16 tRFC;      ///< Number of tCK cycles for the channel DIMM's minimum refresh recovery delay time.
  UINT16 tRFCpb;    ///< Number of tCK cycles for the channel DIMM's minimum per bank refresh recovery delay time.
  UINT16 tRFC2;     ///< Number of tCK cycles for the channel DIMM's minimum refresh recovery delay time.
  UINT16 tRFC4;     ///< Number of tCK cycles for the channel DIMM's minimum refresh recovery delay time.
  UINT16 tRPab;     ///< Number of tCK cycles for the channel DIMM's minimum row precharge delay time for all banks.
  UINT16 tRRD;      ///< Number of tCK cycles for the channel DIMM's minimum row active to row active delay time.
  UINT16 tRRD_L;    ///< Number of tCK cycles for the channel DIMM's minimum row active to row active delay time for same bank groups.
  UINT16 tRRD_S;    ///< Number of tCK cycles for the channel DIMM's minimum row active to row active delay time for different bank groups.
  UINT16 tRTP;      ///< Number of tCK cycles for the channel DIMM's minimum internal read to precharge command delay time.
  UINT16 tWR;       ///< Number of tCK cycles for the channel DIMM's minimum write recovery time.
  UINT16 tWTR;      ///< Number of tCK cycles for the channel DIMM's minimum internal write to read command delay time.
  UINT16 tWTR_L;    ///< Number of tCK cycles for the channel DIMM's minimum internal write to read command delay time for same bank groups.
  UINT16 tWTR_S;    ///< Number of tCK cycles for the channel DIMM's minimum internal write to read command delay time for different bank groups.
  UINT16 tCCD_L;  ///< Number of tCK cycles for the channel DIMM's minimum CAS-to-CAS delay for same bank group.
} MRC_CH_TIMING;

typedef struct {
  UINT16 tRDPRE;     ///< Read CAS to Precharge cmd delay
} MRC_IP_TIMING;

///
/// Memory SMBIOS & OC Memory Data Hob
///
typedef struct {
  UINT8            Status;                  ///< See MrcDimmStatus for the definition of this field.
  UINT8            DimmId;
  UINT32           DimmCapacity;            ///< DIMM size in MBytes.
  UINT16           MfgId;
  UINT8            ModulePartNum[20];       ///< Module part number for DDR3 is 18 bytes however for DRR4 20 bytes as per JEDEC Spec, so reserving 20 bytes
  UINT8            RankInDimm;              ///< The number of ranks in this DIMM.
  UINT8            SpdDramDeviceType;       ///< Save SPD DramDeviceType information needed for SMBIOS structure creation.
  UINT8            SpdModuleType;           ///< Save SPD ModuleType information needed for SMBIOS structure creation.
  UINT8            SpdModuleMemoryBusWidth; ///< Save SPD ModuleMemoryBusWidth information needed for SMBIOS structure creation.
  UINT8            SpdSave[MAX_SPD_SAVE];   ///< Save SPD Manufacturing information needed for SMBIOS structure creation.
  UINT16           Speed;                   ///< The maximum capable speed of the device, in MHz
  UINT8            MdSocket;                ///< MdSocket: 0 = Memory Down, 1 = Socketed. Needed for SMBIOS structure creation.
} DIMM_INFO;

typedef struct {
  UINT8            Status;                  ///< Indicates whether this channel should be used.
  UINT8            ChannelId;
  UINT8            DimmCount;               ///< Number of valid DIMMs that exist in the channel.
  MRC_CH_TIMING    Timing[MAX_PROFILE_NUM]; ///< The channel timing values.
  DIMM_INFO        DimmInfo[MAX_DIMM];      ///< Save the DIMM output characteristics.
} CHANNEL_INFO;

typedef struct {
  UINT8            Status;                  ///< Indicates whether this controller should be used.
  UINT16           DeviceId;                ///< The PCI device id of this memory controller.
  UINT8            RevisionId;              ///< The PCI revision id of this memory controller.
  UINT8            ChannelCount;            ///< Number of valid channels that exist on the controller.
  CHANNEL_INFO     ChannelInfo[MAX_CH];     ///< The following are channel level definitions.
} CONTROLLER_INFO;

/// This data structure contains per-SaGv timing values that are considered output by the MRC.
typedef struct {
  UINT32        DataRate;    ///< The memory rate for the current SaGv Point in units of MT/s
  MRC_CH_TIMING JedecTiming; ///< Timings used for this entry's corresponding SaGv Point - derived from JEDEC SPD spec
  MRC_IP_TIMING IpTiming;    ///< Timings used for this entry's corresponding SaGv Point - IP specific
} HOB_SAGV_TIMING_OUT;

/// This data structure contains SAGV config values that are considered output by the MRC.
typedef struct {
  UINT32              NumSaGvPointsEnabled; ///< Count of the total number of SAGV Points enabled.
  UINT32              SaGvPointMask;        ///< Bit mask where each bit indicates an enabled SAGV point.
  HOB_SAGV_TIMING_OUT SaGvTiming[HOB_MAX_SAGV_POINTS];
} HOB_SAGV_INFO;

typedef struct {
  UINT8             Revision;
  UINT16            DataWidth;              ///< Data width, in bits, of this memory device
  /** As defined in SMBIOS 3.0 spec
    Section 7.18.2 and Table 75
  **/
  UINT8             MemoryType;             ///< DDR type: DDR3, DDR4, or LPDDR3
  UINT16            MaximumMemoryClockSpeed;///< The maximum capable speed of the device, in megahertz (MHz)
  UINT16            ConfiguredMemoryClockSpeed; ///< The configured clock speed to the memory device, in megahertz (MHz)
  /** As defined in SMBIOS 3.0 spec
    Section 7.17.3 and Table 72
  **/
  UINT8             ErrorCorrectionType;

  SiMrcVersion      Version;
  BOOLEAN           EccSupport;
  UINT8             MemoryProfile;
  UINT8             IsDMBRunning;                      ///< Deprecated.
  UINT32            TotalPhysicalMemorySize;
  UINT32            DefaultXmptCK[MAX_XMP_PROFILE_NUM];///< Stores the tCK value read from SPD XMP profiles if they exist.
  ///
  /// Set of bit flags showing XMP and User Profile capability status for the DIMMs detected in system. For each bit, 1 is supported, 0 is unsupported.
  /// Bit 0: XMP Profile 1 capability status
  /// Bit 1: XMP Profile 2 capability status
  /// Bit 2: XMP Profile 3 capability status
  /// Bit 3: User Profile 4 capability status
  /// Bit 4: User Profile 5 capability status
  ///
  UINT8             XmpProfileEnable;
  UINT8             XmpConfigWarning;                  ///< If XMP capable DIMMs config support only 1DPC, but 2DPC is installed
  UINT8             Ratio;                             ///< DDR Frequency Ratio, Max Value 255
  UINT8             RefClk;
  UINT32            VddVoltage[MAX_PROFILE_NUM];
  UINT32            VddqVoltage[MAX_PROFILE_NUM];
  UINT32            VppVoltage[MAX_PROFILE_NUM];
  CONTROLLER_INFO   Controller[MAX_NODE];
  UINT16            Ratio_UINT16;                      ///< DDR Frequency Ratio, used for programs that require ratios higher then 255
  UINT32            NumPopulatedChannels;              ///< Total number of memory channels populated
  HOB_SAGV_INFO     SagvConfigInfo;                    ///< This data structure contains SAGV config values that are considered output by the MRC.
  UINT16            TotalMemWidth;                     ///< Total Memory Width in bits from all populated channels
  BOOLEAN           MemorySpeedReducedWrongDimmSlot;   ///< Can be used by OEM BIOS to display a warning on the screen that DDR speed was reduced due to wrong DIMM population
  BOOLEAN           MemorySpeedReducedMixedConfig;     ///< Can be used by OEM BIOS to display a warning on the screen that DDR speed was reduced due to mixed DIMM config
  BOOLEAN           DynamicMemoryBoostTrainingFailed;  ///< TRUE if Dynamic Memory Boost failed to train and was force disabled on the last full training boot. FALSE otherwise.
} MEMORY_INFO_DATA_HOB;

#pragma pack (pop)

#endif // _MEM_INFO_HOB_H_
