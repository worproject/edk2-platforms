/** @file
*  OemMiscLib.c
*
*  Copyright (c) 2021 - 2023, Ampere Computing LLC. All rights reserved.
*  Copyright (c) 2021, NUVIA Inc. All rights reserved.
*  Copyright (c) 2018, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2018, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include <PiPei.h>
#include <Uefi.h>
#include <IndustryStandard/ArmCache.h>
#include <Library/AmpereCpuLib.h>
#include <Library/ArmLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/HiiLib.h>
#include <Library/HobLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/OemMiscLib.h>
#include <Library/PrintLib.h>
#include <Guid/PlatformInfoHob.h>

#define PROCESSOR_VERSION_ALTRA       L"Ampere(R) Altra(R) Processor"
#define PROCESSOR_VERSION_ALTRA_MAX   L"Ampere(R) Altra(R) Max Processor"

#define MHZ_SCALE_FACTOR    1000000

#define SCP_VERSION_STRING_MAX_LENGTH 32

#define OEM_DEFAULT_INFORMATION L"To Be Filled By O.E.M."

UINT32
GetCacheConfig (
  IN UINT32  CacheLevel,
  IN BOOLEAN DataCache,
  IN BOOLEAN UnifiedCache
  )
{
  CSSELR_DATA Csselr;
  UINT64      Ccsidr;
  BOOLEAN     SupportWB;
  BOOLEAN     SupportWT;

  Csselr.Data = 0;
  Csselr.Bits.Level = CacheLevel - 1;
  Csselr.Bits.InD = (!DataCache && !UnifiedCache);

  Ccsidr = ReadCCSIDR (Csselr.Data);
  SupportWT = (Ccsidr & (1 << 31)) ? TRUE : FALSE;
  SupportWB = (Ccsidr & (1 << 30)) ? TRUE : FALSE;

  if (SupportWT && SupportWB) {
    return 2; // Varies with Memory Address
  }

  if (SupportWT) {
    return 0; // Write Through
  }

  if (SupportWB) {
    return 1; // Write Back
  }

  return 3; // Unknown
}

/** Gets the CPU frequency of the specified processor.

  @param ProcessorIndex Index of the processor to get the frequency for.

  @return               CPU frequency in Hz
**/
UINTN
EFIAPI
OemGetCpuFreq (
  IN UINT8 ProcessorIndex
  )
{
  return CpuGetCurrentFreq (ProcessorIndex);
}

/** Gets information about the specified processor and stores it in
    the structures provided.

  @param ProcessorIndex  Index of the processor to get the information for.
  @param ProcessorStatus Processor status.
  @param ProcessorCharacteristics Processor characteritics.
  @param MiscProcessorData        Miscellaneous processor information.

  @return  TRUE on success, FALSE on failure.
**/
BOOLEAN
EFIAPI
OemGetProcessorInformation (
  IN UINTN ProcessorIndex,
  IN OUT PROCESSOR_STATUS_DATA *ProcessorStatus,
  IN OUT PROCESSOR_CHARACTERISTIC_FLAGS *ProcessorCharacteristics,
  IN OUT OEM_MISC_PROCESSOR_DATA *MiscProcessorData
  )
{
  UINT16 ProcessorCount;

  ProcessorCount = GetNumberOfActiveSockets ();

  if (ProcessorIndex < ProcessorCount) {
    ProcessorStatus->Bits.CpuStatus       = 1; // CPU enabled
    ProcessorStatus->Bits.Reserved1       = 0;
    ProcessorStatus->Bits.SocketPopulated = 1;
    ProcessorStatus->Bits.Reserved2       = 0;
  } else {
    ProcessorStatus->Bits.CpuStatus       = 0; // CPU disabled
    ProcessorStatus->Bits.Reserved1       = 0;
    ProcessorStatus->Bits.SocketPopulated = 0;
    ProcessorStatus->Bits.Reserved2       = 0;
  }

  ProcessorCharacteristics->ProcessorReserved1      = 0;
  ProcessorCharacteristics->ProcessorUnknown        = 0;
  ProcessorCharacteristics->Processor64BitCapable   = 1;
  ProcessorCharacteristics->ProcessorMultiCore      = 1;
  ProcessorCharacteristics->ProcessorHardwareThread = 0;
  ProcessorCharacteristics->ProcessorExecuteProtection      = 1;
  ProcessorCharacteristics->ProcessorEnhancedVirtualization = 1;
  ProcessorCharacteristics->ProcessorPowerPerformanceCtrl   = 1;
  ProcessorCharacteristics->Processor128BitCapable = 0;
  ProcessorCharacteristics->ProcessorReserved2  = 0;

  MiscProcessorData->Voltage      = CpuGetVoltage (ProcessorIndex);
  MiscProcessorData->CurrentSpeed = (UINT16)(CpuGetCurrentFreq (ProcessorIndex) / MHZ_SCALE_FACTOR);
  MiscProcessorData->MaxSpeed     = (UINT16)(CpuGetMaxFreq (ProcessorIndex) / MHZ_SCALE_FACTOR);
  MiscProcessorData->CoreCount    = GetMaximumNumberOfCores ();
  MiscProcessorData->ThreadCount  = GetMaximumNumberOfCores ();
  MiscProcessorData->CoresEnabled = GetNumberOfActiveCoresPerSocket (ProcessorIndex);

  return TRUE;
}

/** Gets information about the cache at the specified cache level.

  @param ProcessorIndex The processor to get information for.
  @param CacheLevel The cache level to get information for.
  @param DataCache  Whether the cache is a data cache.
  @param UnifiedCache Whether the cache is a unified cache.
  @param SmbiosCacheTable The SMBIOS Type7 cache information structure.

  @return TRUE on success, FALSE on failure.
**/
BOOLEAN
EFIAPI
OemGetCacheInformation (
  IN UINT8   ProcessorIndex,
  IN UINT8   CacheLevel,
  IN BOOLEAN DataCache,
  IN BOOLEAN UnifiedCache,
  IN OUT SMBIOS_TABLE_TYPE7 *SmbiosCacheTable
  )
{
  SmbiosCacheTable->CacheConfiguration = CacheLevel - 1;
  SmbiosCacheTable->CacheConfiguration |= (1 << 7); // Enable
  SmbiosCacheTable->CacheConfiguration |= (GetCacheConfig (CacheLevel, DataCache, UnifiedCache) << 8);

  SmbiosCacheTable->SupportedSRAMType.Unknown = 0;
  SmbiosCacheTable->SupportedSRAMType.Synchronous = 1;
  SmbiosCacheTable->CurrentSRAMType.Unknown = 0;
  SmbiosCacheTable->CurrentSRAMType.Synchronous = 1;

  if (CacheLevel == 1 && !DataCache && !UnifiedCache) {
    SmbiosCacheTable->ErrorCorrectionType = CacheErrorParity;
  } else {
    SmbiosCacheTable->ErrorCorrectionType = CacheErrorSingleBit;
  }

  return TRUE;
}

/** Gets the maximum number of processors supported by the platform.

  @return The maximum number of processors.
**/
UINT8
EFIAPI
OemGetMaxProcessors (
  VOID
  )
{
  return GetNumberOfSupportedSockets ();
}

/** Gets the type of chassis for the system.

  @retval The type of the chassis.
**/
MISC_CHASSIS_TYPE
EFIAPI
OemGetChassisType (
  VOID
  )
{
  return MiscChassisTypeRackMountChassis;
}

/** Returns whether the specified processor is present or not.

  @param ProcessIndex The processor index to check.

  @return TRUE is the processor is present, FALSE otherwise.
**/
BOOLEAN
EFIAPI
OemIsProcessorPresent (
  IN UINTN ProcessorIndex
  )
{
  //
  // Platform only supports 2 sockets: Master and Slave.
  // The master socket is always online.
  //
  if (ProcessorIndex == 0) {
    return TRUE;
  } else if (ProcessorIndex == 1) {
    return IsSlaveSocketAvailable ();
  }

  return FALSE;
}

/**
  Update the firmware version in SMBIOS Type 0.
  This is the combination of UEFI and Ampere system firmware version.

**/
VOID
UpdateFirmwareVersionString (
  OUT CHAR16  *Version
  )
{
  UINT8   UnicodeStrLen;
  UINT8   FirmwareVersionStrLen;
  UINT8   FirmwareVersionStrSize;
  UINT8   *ScpVersion;
  UINT8   *ScpBuild;
  CHAR16  UnicodeStr[SMBIOS_STRING_MAX_LENGTH * sizeof (CHAR16)];
  CHAR16  *FirmwareVersionPcdPtr;

  FirmwareVersionStrLen = 0;
  ZeroMem (UnicodeStr, sizeof (UnicodeStr));
  FirmwareVersionPcdPtr  = (CHAR16 *)FixedPcdGetPtr (PcdFirmwareVersionString);
  FirmwareVersionStrSize = SMBIOS_STRING_MAX_LENGTH * sizeof (CHAR16);

  //
  // Format of PcdFirmwareVersionString is
  // "(MAJOR_VER).(MINOR_VER).(BUILD) Build YYYY.MM.DD", we only need
  // "(MAJOR_VER).(MINOR_VER).(BUILD)" showed in BIOS version. Using
  // space character to determine this string. Another case uses null
  // character to end while loop.
  //
  while (*FirmwareVersionPcdPtr != ' ' && *FirmwareVersionPcdPtr != '\0') {
    FirmwareVersionStrLen++;
    FirmwareVersionPcdPtr++;
  }

  FirmwareVersionPcdPtr = (CHAR16 *)FixedPcdGetPtr (PcdFirmwareVersionString);
  UnicodeStrLen         = FirmwareVersionStrLen * sizeof (CHAR16);
  CopyMem (UnicodeStr, FirmwareVersionPcdPtr, UnicodeStrLen);

  GetScpVersion (&ScpVersion);
  GetScpBuild (&ScpBuild);
  if ((ScpVersion == NULL) || (ScpBuild == NULL)) {
    DEBUG ((
      DEBUG_ERROR,
      "%a:%d: Fail to get SMpro/PMpro information\n",
      __func__,
      __LINE__
      ));
    UnicodeSPrint (
      Version,
      FirmwareVersionStrSize,
      L"TianoCore %.*s (SYS: 0.00.00000000)",
      FirmwareVersionStrLen,
      (UINT16 *)UnicodeStr
      );
  } else {
    UnicodeSPrint (
      Version,
      FirmwareVersionStrSize,
      L"TianoCore %.*s (SYS: %a.%a)",
      FirmwareVersionStrLen,
      (UINT16 *)UnicodeStr,
      ScpVersion,
      ScpBuild
      );
  }
}

/** Updates the HII string for the specified field.

  @param HiiHandle     The HII handle.
  @param TokenToUpdate The string to update.
  @param Field         The field to get information about.
**/
VOID
EFIAPI
OemUpdateSmbiosInfo (
  IN EFI_HII_HANDLE HiiHandle,
  IN EFI_STRING_ID TokenToUpdate,
  IN OEM_MISC_SMBIOS_HII_STRING_FIELD Field
  )
{
  EFI_STRING UnicodeString;
  UINT8      StringLength;

  StringLength = SMBIOS_STRING_MAX_LENGTH * sizeof (CHAR16);
  UnicodeString = AllocatePool (StringLength);
  if (UnicodeString == NULL) {
    DEBUG ((
      DEBUG_ERROR,
      "%a:%d: There is not enough memory remaining to satisfy the request\n",
      __func__,
      __LINE__));

    goto Exit;
  }

  switch (Field) {
    case ProductNameType01:
    case SystemManufacturerType01:
    case VersionType01:
    case SerialNumType01:
    case SkuNumberType01:
      UnicodeSPrint (
        UnicodeString,
        StringLength,
        OEM_DEFAULT_INFORMATION
        );
      break;

    case FamilyType01:
      UnicodeSPrint (
        UnicodeString,
        StringLength,
        IsAc01Processor () ? L"Altra\0" : L"Altra Max\0"
        );
      break;

    case ProductNameType02:
    case AssetTagType02:
    case VersionType02:
    case SerialNumberType02:
    case BoardManufacturerType02:
      UnicodeSPrint (
        UnicodeString,
        StringLength,
        OEM_DEFAULT_INFORMATION
        );
      break;

    case ChassisLocationType02:
      UnicodeSPrint (
        UnicodeString,
        StringLength,
        L"Base of Chassis"
        );
      break;

    case SerialNumberType03:
    case VersionType03:
    case ManufacturerType03:
    case AssetTagType03:
    case SkuNumberType03:
      UnicodeSPrint (
        UnicodeString,
        StringLength,
        OEM_DEFAULT_INFORMATION
        );
      break;

    case BiosVersionType00:
      UpdateFirmwareVersionString (UnicodeString);
      break;

    default:
      UnicodeSPrint (
        UnicodeString,
        StringLength,
        L"Not Specified"
        );
  }

  // Update string value for respective token.
  HiiSetString (HiiHandle, TokenToUpdate, UnicodeString, NULL);

Exit:
  FreePool (UnicodeString);
}

/** Fetches the Type 32 boot information status.

  @return Boot status.
**/
MISC_BOOT_INFORMATION_STATUS_DATA_TYPE
EFIAPI
OemGetBootStatus (
  VOID
  )
{
  return BootInformationStatusNoError;
}

/** Fetches the chassis status when it was last booted.

 @return Chassis status.
**/
MISC_CHASSIS_STATE
EFIAPI
OemGetChassisBootupState (
  VOID
  )
{
  return ChassisStateSafe;
}

/** Fetches the chassis power supply/supplies status when last booted.

 @return Chassis power supply/supplies status.
**/
MISC_CHASSIS_STATE
EFIAPI
OemGetChassisPowerSupplyState (
  VOID
  )
{
  return ChassisStateSafe;
}

/** Fetches the chassis thermal status when last booted.

 @return Chassis thermal status.
**/
MISC_CHASSIS_STATE
EFIAPI
OemGetChassisThermalState (
  VOID
  )
{
  return ChassisStateSafe;
}

/** Fetches the chassis security status when last booted.

 @return Chassis security status.
**/
MISC_CHASSIS_SECURITY_STATE
EFIAPI
OemGetChassisSecurityStatus (
  VOID
  )
{
  return ChassisSecurityStatusNone;
}

/** Fetches the chassis height in RMUs (Rack Mount Units).

  @return The height of the chassis.
**/
UINT8
EFIAPI
OemGetChassisHeight (
  VOID
  )
{
  return 1U;
}

/** Fetches the number of power cords.

  @return The number of power cords.
**/
UINT8
EFIAPI
OemGetChassisNumPowerCords (
  VOID
  )
{
  return 2;
}

/** Fetches the BIOS release.

  @return The BIOS release.
**/
UINT16
EFIAPI
OemGetBiosRelease (
  VOID
  )
{
  UINT16 BiosRelease;

  BiosRelease = (UINT16)(((PcdGet8 (PcdSmbiosTables0MajorVersion)) << 8)
                         | PcdGet8 (PcdSmbiosTables0MinorVersion));

  return BiosRelease;
}

/**
  Fetches the embedded controller firmware release.

  @return   UINT16   The embedded controller firmware release.
**/
UINT16
EFIAPI
OemGetEmbeddedControllerFirmwareRelease (
  VOID
  )
{
  CHAR8  AsciiScpVer[SCP_VERSION_STRING_MAX_LENGTH];
  UINT8  *ScpVer = NULL;
  UINT8  Index;
  UINT16 FirmwareRelease;

  GetScpVersion (&ScpVer);
  if (ScpVer == NULL) {
    DEBUG ((
      DEBUG_ERROR,
      "%a:%d: Fail to get SMpro/PMpro information\n",
      __func__,
      __LINE__));

      return 0xFFFF;
  }

  CopyMem ((VOID *)AsciiScpVer, (VOID *)ScpVer, AsciiStrLen ((CHAR8 *)ScpVer));
  /* The AsciiVersion is formated as "major.minor" */
  for (Index = 0; Index < (UINTN)AsciiStrLen (AsciiScpVer); Index++) {
    if (AsciiScpVer[Index] == '.') {
      AsciiScpVer[Index] = '\0';
      break;
    }
  }

  FirmwareRelease = ((UINT8)AsciiStrDecimalToUintn (AsciiScpVer) << 8)
                    + (UINT8)AsciiStrDecimalToUintn (AsciiScpVer + Index + 1);

  return FirmwareRelease;
}

/**
  Fetches the system UUID.

  @param[out]   SystemUuid   The pointer to the buffer to store the System UUID.
**/
VOID
EFIAPI
OemGetSystemUuid (
  OUT GUID  *SystemUuid
  )
{
  if (SystemUuid == NULL) {
    return;
  }

  CopyGuid (SystemUuid, &gZeroGuid);
}
