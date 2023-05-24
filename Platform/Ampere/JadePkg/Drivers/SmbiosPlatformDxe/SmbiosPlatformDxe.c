/** @file

  Copyright (c) 2020 - 2023, Ampere Computing LLC. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>

#include <Guid/SmBios.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/Smbios.h>

#define CHASSIS_VERSION_TEMPLATE    "None               \0"
#define CHASSIS_SERIAL_TEMPLATE     "Serial Not Set     \0"
#define CHASSIS_ASSET_TAG_TEMPLATE  "Asset Tag Not Set  \0"

#define TYPE8_ADDITIONAL_STRINGS      \
  "VGA1 - Rear VGA Connector\0"       \
  "DB-15 Male (VGA)         \0"

#define TYPE9_ADDITIONAL_STRINGS       \
  "Socket 0 Riser 1 x32 - Slot 1\0"

#define TYPE11_ADDITIONAL_STRINGS       \
  "www.amperecomputing.com\0"

#define TYPE41_ADDITIONAL_STRINGS       \
  "Onboard VGA\0"

#define ADDITIONAL_STR_INDEX_1    0x01
#define ADDITIONAL_STR_INDEX_2    0x02
#define ADDITIONAL_STR_INDEX_3    0x03
#define ADDITIONAL_STR_INDEX_4    0x04
#define ADDITIONAL_STR_INDEX_5    0x05
#define ADDITIONAL_STR_INDEX_6    0x06

//
// Type definition and contents of the default SMBIOS table.
// This table covers only the minimum structures required by
// the SMBIOS specification (section 6.2, version 3.0)
//
#pragma pack(1)
typedef struct {
  SMBIOS_TABLE_TYPE8 Base;
  CHAR8              Strings[sizeof (TYPE8_ADDITIONAL_STRINGS)];
} ARM_TYPE8;

typedef struct {
  SMBIOS_TABLE_TYPE9 Base;
  CHAR8              Strings[sizeof (TYPE9_ADDITIONAL_STRINGS)];
} ARM_TYPE9;

typedef struct {
  SMBIOS_TABLE_TYPE11 Base;
  CHAR8               Strings[sizeof (TYPE11_ADDITIONAL_STRINGS)];
} ARM_TYPE11;

typedef struct {
  SMBIOS_TABLE_TYPE41 Base;
  CHAR8               Strings[sizeof (TYPE41_ADDITIONAL_STRINGS)];
} ARM_TYPE41;

#pragma pack()

// Type 8 Port Connector Information
STATIC CONST ARM_TYPE8 mArmDefaultType8Vga = {
  {
    {                                             // SMBIOS_STRUCTURE Hdr
      EFI_SMBIOS_TYPE_PORT_CONNECTOR_INFORMATION, // UINT8 Type
      sizeof (SMBIOS_TABLE_TYPE8),                // UINT8 Length
      SMBIOS_HANDLE_PI_RESERVED,
    },
    ADDITIONAL_STR_INDEX_1,       // InternalReferenceDesignator String
    PortConnectorTypeDB15Female,  // InternalConnectorType;
    ADDITIONAL_STR_INDEX_2,       // ExternalReferenceDesignator String
    PortTypeOther,                // ExternalConnectorType;
    PortTypeVideoPort,            // PortType;
  },
  "VGA1 - Rear VGA Connector\0" \
  "DB-15 Male (VGA)\0"
};

// Type 8 Port Connector Information
STATIC CONST ARM_TYPE8 mArmDefaultType8USBFront = {
  {
    {                                             // SMBIOS_STRUCTURE Hdr
      EFI_SMBIOS_TYPE_PORT_CONNECTOR_INFORMATION, // UINT8 Type
      sizeof (SMBIOS_TABLE_TYPE8),                // UINT8 Length
      SMBIOS_HANDLE_PI_RESERVED,
    },
    ADDITIONAL_STR_INDEX_1,       // InternalReferenceDesignator String
    PortConnectorTypeUsb,         // InternalConnectorType;
    ADDITIONAL_STR_INDEX_2,       // ExternalReferenceDesignator String
    PortTypeOther,                // ExternalConnectorType;
    PortTypeUsb,                  // PortType;
  },
  "Front Panel USB 3.0\0"  \
  "USB\0"
};

// Type 8 Port Connector Information
STATIC CONST ARM_TYPE8 mArmDefaultType8USBRear = {
  {
    {                                             // SMBIOS_STRUCTURE Hdr
      EFI_SMBIOS_TYPE_PORT_CONNECTOR_INFORMATION, // UINT8 Type
      sizeof (SMBIOS_TABLE_TYPE8),                // UINT8 Length
      SMBIOS_HANDLE_PI_RESERVED,
    },
    ADDITIONAL_STR_INDEX_1,       // InternalReferenceDesignator String
    PortConnectorTypeUsb,         // InternalConnectorType;
    ADDITIONAL_STR_INDEX_2,       // ExternalReferenceDesignator String
    PortTypeOther,                // ExternalConnectorType;
    PortTypeUsb,                  // PortType;
  },
  "Rear Panel USB 3.0\0"   \
  "USB\0"
};

// Type 8 Port Connector Information
STATIC CONST ARM_TYPE8 mArmDefaultType8NetRJ45 = {
  {
    {                                             // SMBIOS_STRUCTURE Hdr
      EFI_SMBIOS_TYPE_PORT_CONNECTOR_INFORMATION, // UINT8 Type
      sizeof (SMBIOS_TABLE_TYPE8),                // UINT8 Length
      SMBIOS_HANDLE_PI_RESERVED,
    },
    ADDITIONAL_STR_INDEX_1,       // InternalReferenceDesignator String
    PortConnectorTypeRJ45,        // InternalConnectorType;
    ADDITIONAL_STR_INDEX_2,       // ExternalReferenceDesignator String
    PortConnectorTypeRJ45,        // ExternalConnectorType;
    PortTypeNetworkPort,          // PortType;
  },
  "RJ1 - BMC RJ45 Port\0" \
  "RJ45 Connector\0"
};

// Type 8 Port Connector Information
STATIC CONST ARM_TYPE8 mArmDefaultType8NetOcp = {
  {
    {                                             // SMBIOS_STRUCTURE Hdr
      EFI_SMBIOS_TYPE_PORT_CONNECTOR_INFORMATION, // UINT8 Type
      sizeof (SMBIOS_TABLE_TYPE8),                // UINT8 Length
      SMBIOS_HANDLE_PI_RESERVED,
    },
    ADDITIONAL_STR_INDEX_1,       // InternalReferenceDesignator String
    PortTypeOther,                // InternalConnectorType;
    ADDITIONAL_STR_INDEX_2,       // ExternalReferenceDesignator String
    PortTypeOther,                // ExternalConnectorType;
    PortTypeNetworkPort,          // PortType;
  },
  "OCP1 - OCP NIC 3.0 Connector\0"  \
  "OCP NIC 3.0\0"
};

// Type 8 Port Connector Information
STATIC CONST ARM_TYPE8 mArmDefaultType8Uart = {
  {
    {                                             // SMBIOS_STRUCTURE Hdr
      EFI_SMBIOS_TYPE_PORT_CONNECTOR_INFORMATION, // UINT8 Type
      sizeof (SMBIOS_TABLE_TYPE8),                // UINT8 Length
      SMBIOS_HANDLE_PI_RESERVED,
    },
    ADDITIONAL_STR_INDEX_1,        // InternalReferenceDesignator String
    PortTypeOther,                 // InternalConnectorType;
    ADDITIONAL_STR_INDEX_2,        // ExternalReferenceDesignator String
    PortConnectorTypeDB9Female,    // ExternalConnectorType;
    PortTypeSerial16550Compatible, // PortType;
  },
  "UART1 - BMC UART5 Connector\0"  \
  "DB-9 female\0"
};

// Type 9 System Slots
STATIC ARM_TYPE9 mArmDefaultType9Sk0RiserX32Slot1 = {
  {
    {                               // SMBIOS_STRUCTURE Hdr
      EFI_SMBIOS_TYPE_SYSTEM_SLOTS, // UINT8 Type
      sizeof (SMBIOS_TABLE_TYPE9),  // UINT8 Length
      SMBIOS_HANDLE_PI_RESERVED,
    },
    ADDITIONAL_STR_INDEX_1,
    SlotTypePciExpressGen3,
    SlotDataBusWidth16X,
    SlotUsageAvailable,
    SlotLengthLong,
    0,
    {0, 0, 1}, // Provides 3.3 Volts
    {1},       // PME
    0,
    0,
    0,
  },
  "S0 Riser 1 x32 - Slot 1\0"
};

// Type 9 System Slots
STATIC ARM_TYPE9 mArmDefaultType9Sk0RiserX32Slot2 = {
  {
    {                               // SMBIOS_STRUCTURE Hdr
      EFI_SMBIOS_TYPE_SYSTEM_SLOTS, // UINT8 Type
      sizeof (SMBIOS_TABLE_TYPE9),  // UINT8 Length
      SMBIOS_HANDLE_PI_RESERVED,
    },
    ADDITIONAL_STR_INDEX_1,
    SlotTypePciExpressGen3,
    SlotDataBusWidth8X,
    SlotUsageAvailable,
    SlotLengthLong,
    0,
    {0, 0, 1}, // Provides 3.3 Volts
    {1},       // PME
    4,
    0,
    0,
  },
  "S0 Riser x32 - Slot 2\0"
};

// Type 9 System Slots
STATIC ARM_TYPE9 mArmDefaultType9Sk0RiserX32Slot3 = {
  {
    {                               // SMBIOS_STRUCTURE Hdr
      EFI_SMBIOS_TYPE_SYSTEM_SLOTS, // UINT8 Type
      sizeof (SMBIOS_TABLE_TYPE9),  // UINT8 Length
      SMBIOS_HANDLE_PI_RESERVED,
    },
    ADDITIONAL_STR_INDEX_1,
    SlotTypePciExpressGen3,
    SlotDataBusWidth8X,
    SlotUsageAvailable,
    SlotLengthLong,
    0,
    {0, 0, 1}, // Provides 3.3 Volts
    {1},       // PME
    5,
    0,
    0,
  },
  "S0 Riser x32 - Slot 3\0"
};

// Type 9 System Slots
STATIC ARM_TYPE9 mArmDefaultType9Sk1RiserX24Slot1 = {
  {
    {                               // SMBIOS_STRUCTURE Hdr
      EFI_SMBIOS_TYPE_SYSTEM_SLOTS, // UINT8 Type
      sizeof (SMBIOS_TABLE_TYPE9),  // UINT8 Length
      SMBIOS_HANDLE_PI_RESERVED,
    },
    ADDITIONAL_STR_INDEX_1,
    SlotTypePciExpressGen3,
    SlotDataBusWidth8X,
    SlotUsageAvailable,
    SlotLengthLong,
    0,
    {0, 0, 1}, // Provides 3.3 Volts
    {1},       // PME
    7,
    0,
    0,
  },
  "S1 Riser x24 - Slot 1\0"
};

// Type 9 System Slots
STATIC ARM_TYPE9 mArmDefaultType9Sk1RiserX24Slot2 = {
  {
    {                               // SMBIOS_STRUCTURE Hdr
      EFI_SMBIOS_TYPE_SYSTEM_SLOTS, // UINT8 Type
      sizeof (SMBIOS_TABLE_TYPE9),  // UINT8 Length
      SMBIOS_HANDLE_PI_RESERVED,
    },
    ADDITIONAL_STR_INDEX_1,
    SlotTypePciExpressGen3,
    SlotDataBusWidth8X,
    SlotUsageAvailable,
    SlotLengthLong,
    0,
    {0, 0, 1}, // Provides 3.3 Volts
    {1},       // PME
    8,
    0,
    0,
  },
  "S1 Riser x24 - Slot 2\0"
};

// Type 9 System Slots
STATIC ARM_TYPE9 mArmDefaultType9Sk1RiserX24Slot3 = {
  {
    {                               // SMBIOS_STRUCTURE Hdr
      EFI_SMBIOS_TYPE_SYSTEM_SLOTS, // UINT8 Type
      sizeof (SMBIOS_TABLE_TYPE9),  // UINT8 Length
      SMBIOS_HANDLE_PI_RESERVED,
    },
    ADDITIONAL_STR_INDEX_1,
    SlotTypePciExpressGen3,
    SlotDataBusWidth8X,
    SlotUsageAvailable,
    SlotLengthLong,
    0,
    {0, 0, 1}, // Provides 3.3 Volts
    {1},       // PME
    9,
    0,
    0,
  },
  "S1 Riser x24 - Slot 3\0"
};

// Type 9 System Slots
STATIC ARM_TYPE9 mArmDefaultType9Sk1RiserX8Slot1 = {
  {
    {                               // SMBIOS_STRUCTURE Hdr
      EFI_SMBIOS_TYPE_SYSTEM_SLOTS, // UINT8 Type
      sizeof (SMBIOS_TABLE_TYPE9),  // UINT8 Length
      SMBIOS_HANDLE_PI_RESERVED,
    },
    ADDITIONAL_STR_INDEX_1,
    SlotTypePciExpressGen3,
    SlotDataBusWidth8X,
    SlotUsageAvailable,
    SlotLengthLong,
    0,
    {0, 0, 1}, // Provides 3.3 Volts
    {1},       // PME
    8,
    0,
    0,
  },
  "S1 Riser x8 - Slot 1\0"
};

// Type 9 System Slots
STATIC ARM_TYPE9 mArmDefaultType9Sk0OcpNic = {
  {
    {                               // SMBIOS_STRUCTURE Hdr
      EFI_SMBIOS_TYPE_SYSTEM_SLOTS, // UINT8 Type
      sizeof (SMBIOS_TABLE_TYPE9),  // UINT8 Length
      SMBIOS_HANDLE_PI_RESERVED,
    },
    ADDITIONAL_STR_INDEX_1,
    SlotTypePciExpressGen3,
    SlotDataBusWidth16X,
    SlotUsageAvailable,
    SlotLengthLong,
    0,
    {0, 0, 1}, // Provides 3.3 Volts
    {1},       // PME
    1,
    0,
    0,
  },
  "S0 OCP NIC 3.0\0"
};

// Type 9 System Slots
STATIC ARM_TYPE9 mArmDefaultType9Sk1NvmeM2Slot1 = {
  {
    {                               // SMBIOS_STRUCTURE Hdr
      EFI_SMBIOS_TYPE_SYSTEM_SLOTS, // UINT8 Type
      sizeof (SMBIOS_TABLE_TYPE9),  // UINT8 Length
      SMBIOS_HANDLE_PI_RESERVED,
    },
    ADDITIONAL_STR_INDEX_1,
    SlotTypePciExpressGen3,
    SlotDataBusWidth4X,
    SlotUsageAvailable,
    SlotLengthLong,
    0,
    {0, 0, 1}, // Provides 3.3 Volts
    {1},       // PME
    5,
    0,
    0,
  },
  "S1 NVMe M.2 - Slot 1\0"
};

// Type 9 System Slots
STATIC ARM_TYPE9 mArmDefaultType9Sk1NvmeM2Slot2 = {
  {
    {                               // SMBIOS_STRUCTURE Hdr
      EFI_SMBIOS_TYPE_SYSTEM_SLOTS, // UINT8 Type
      sizeof (SMBIOS_TABLE_TYPE9),  // UINT8 Length
      SMBIOS_HANDLE_PI_RESERVED,
    },
    ADDITIONAL_STR_INDEX_1,
    SlotTypePciExpressGen3,
    SlotDataBusWidth4X,
    SlotUsageAvailable,
    SlotLengthLong,
    0,
    {0, 0, 1}, // Provides 3.3 Volts
    {1},       // PME
    5,
    0,
    0,
  },
  "S1 NVMe M.2 - Slot 2\0"
};

// Type 11 OEM Strings
STATIC ARM_TYPE11 mArmDefaultType11 = {
  {
    {                               // SMBIOS_STRUCTURE Hdr
      EFI_SMBIOS_TYPE_OEM_STRINGS,  // UINT8 Type
      sizeof (SMBIOS_TABLE_TYPE11), // UINT8 Length
      SMBIOS_HANDLE_PI_RESERVED,
    },
    ADDITIONAL_STR_INDEX_1
  },
  TYPE11_ADDITIONAL_STRINGS
};

// Type 24 Hardware Security
STATIC SMBIOS_TABLE_TYPE24 mArmDefaultType24 = {
  {                                    // SMBIOS_STRUCTURE Hdr
    EFI_SMBIOS_TYPE_HARDWARE_SECURITY, // UINT8 Type
    sizeof (SMBIOS_TABLE_TYPE24),      // UINT8 Length
    SMBIOS_HANDLE_PI_RESERVED,
  },
  0
};

// Type 38 IPMI Device Information
STATIC SMBIOS_TABLE_TYPE38 mArmDefaultType38 = {
  {                                          // SMBIOS_STRUCTURE Hdr
    EFI_SMBIOS_TYPE_IPMI_DEVICE_INFORMATION, // UINT8 Type
    sizeof (SMBIOS_TABLE_TYPE38),            // UINT8 Length
    SMBIOS_HANDLE_PI_RESERVED,
  },
  IPMIDeviceInfoInterfaceTypeSSIF,
  0x20,
  0x20,
  0xFF,
  0x20
};

// Type 41 Onboard Devices Extended Information
STATIC ARM_TYPE41 mArmDefaultType41 = {
  {
    { // SMBIOS_STRUCTURE Hdr
      EFI_SMBIOS_TYPE_ONBOARD_DEVICES_EXTENDED_INFORMATION,
      sizeof (SMBIOS_TABLE_TYPE41),
      SMBIOS_HANDLE_PI_RESERVED,
    },
    1,
    0x83,  // OnBoardDeviceExtendedTypeVideo, Enabled
    1,
    4,
    2,
    0,
  },
  TYPE41_ADDITIONAL_STRINGS
};

// Type 42 System Boot Information
STATIC SMBIOS_TABLE_TYPE42 mArmDefaultType42 = {
  { // SMBIOS_STRUCTURE Hdr
    EFI_SMBIOS_TYPE_MANAGEMENT_CONTROLLER_HOST_INTERFACE,
    sizeof (SMBIOS_TABLE_TYPE42),
    SMBIOS_HANDLE_PI_RESERVED,
  },
  MCHostInterfaceTypeOemDefined,
  4,
  {0xFF, 0, 0, 0}
};

STATIC CONST VOID *DefaultCommonTables[] =
{
  &mArmDefaultType8Vga,
  &mArmDefaultType8USBFront,
  &mArmDefaultType8USBRear,
  &mArmDefaultType8NetRJ45,
  &mArmDefaultType8NetOcp,
  &mArmDefaultType8Uart,
  &mArmDefaultType9Sk0RiserX32Slot1,
  &mArmDefaultType9Sk0RiserX32Slot2,
  &mArmDefaultType9Sk0RiserX32Slot3,
  &mArmDefaultType9Sk1RiserX24Slot1,
  &mArmDefaultType9Sk1RiserX24Slot2,
  &mArmDefaultType9Sk1RiserX24Slot3,
  &mArmDefaultType9Sk1RiserX8Slot1,
  &mArmDefaultType9Sk0OcpNic,
  &mArmDefaultType9Sk1NvmeM2Slot1,
  &mArmDefaultType9Sk1NvmeM2Slot2,
  &mArmDefaultType11,
  &mArmDefaultType24,
  &mArmDefaultType38,
  &mArmDefaultType41,
  &mArmDefaultType42,
  NULL
};

typedef struct {
  CHAR8 MonthNameStr[4]; // example "Jan", Compiler build date, month
  CHAR8 DigitStr[3];     // example "01", Smbios date format, month
} MonthStringDig;

STATIC
UINTN
GetStringPackSize (
  CHAR8 *StringPack
  )
{
  UINTN StrCount;
  CHAR8 *StrStart;

  if ((*StringPack == 0) && (*(StringPack + 1) == 0)) {
    return 0;
  }

  // String section ends in double-null (0000h)
  for (StrCount = 0, StrStart = StringPack;
       ((*StrStart != 0) || (*(StrStart + 1) != 0)); StrStart++, StrCount++)
  {
  }

  return StrCount + 2; // Included the double NULL
}

// Update String at String number to String Pack
EFI_STATUS
UpdateStringPack (
  CHAR8 *StringPack,
  CHAR8 *String,
  UINTN StringNumber
  )
{
  CHAR8 *StrStart;
  UINTN StrIndex;
  UINTN InputStrLen;
  UINTN TargetStrLen;
  UINTN BufferSize;
  CHAR8 *Buffer;

  StrStart = StringPack;
  for (StrIndex = 1; StrIndex < StringNumber; StrStart++) {
    // A string ends in 00h
    if (*StrStart == 0) {
      StrIndex++;
    }
    // String section ends in double-null (0000h)
    if ((*StrStart == 0) && (*(StrStart + 1) == 0)) {
      return EFI_NOT_FOUND;
    }
  }

  if (*StrStart == 0) {
    StrStart++;
  }

  InputStrLen = AsciiStrLen (String);
  TargetStrLen = AsciiStrLen (StrStart);
  BufferSize = GetStringPackSize (StrStart + TargetStrLen + 1);

  // Replace the String if length matched
  // OR this is the last string
  if (InputStrLen == TargetStrLen || (BufferSize == 0)) {
    CopyMem (StrStart, String, InputStrLen);
  }
  // Otherwise, buffer is needed to apply new string
  else {
    Buffer = AllocateZeroPool (BufferSize);
    if (Buffer == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }

    CopyMem (Buffer, StrStart + TargetStrLen + 1, BufferSize);
    CopyMem (StrStart, String, InputStrLen + 1);
    CopyMem (StrStart + InputStrLen + 1, Buffer, BufferSize);

    FreePool (Buffer);
  }

  return EFI_SUCCESS;
}

/**
   Install a whole table worth of structures

   @param  Smbios               SMBIOS protocol.
   @param  DefaultTables        A pointer to the default SMBIOS table structure.
**/
EFI_STATUS
InstallStructures (
  IN       EFI_SMBIOS_PROTOCOL *Smbios,
  IN CONST VOID                *DefaultTables[]
  )
{
  EFI_STATUS        Status = EFI_SUCCESS;
  EFI_SMBIOS_HANDLE SmbiosHandle;
  UINTN             TableIndex;

  ASSERT (Smbios != NULL);

  for (TableIndex = 0; DefaultTables[TableIndex] != NULL; TableIndex++) {
    SmbiosHandle = ((EFI_SMBIOS_TABLE_HEADER *)DefaultTables[TableIndex])->Handle;
    Status = Smbios->Add (
                       Smbios,
                       NULL,
                       &SmbiosHandle,
                       (EFI_SMBIOS_TABLE_HEADER *)DefaultTables[TableIndex]
                       );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a: adding %d failed\n", __FUNCTION__, TableIndex));

      // stop adding rather than continuing
      return Status;
    }
  }

  return EFI_SUCCESS;
}

/**
   Install all structures from the DefaultTables structure

   @param  Smbios               SMBIOS protocol

**/
EFI_STATUS
InstallAllStructures (
  IN EFI_SMBIOS_PROTOCOL *Smbios
  )
{
  EFI_STATUS Status = EFI_SUCCESS;

  ASSERT (Smbios != NULL);

  // Install Tables
  Status = InstallStructures (Smbios, DefaultCommonTables);
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/**
   Installs SMBIOS information for ARM platforms

   @param ImageHandle     Module's image handle
   @param SystemTable     Pointer of EFI_SYSTEM_TABLE

   @retval EFI_SUCCESS    Smbios data successfully installed
   @retval Other          Smbios data was not installed

**/
EFI_STATUS
EFIAPI
SmbiosPlatformDxeEntry (
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  EFI_STATUS          Status;
  EFI_SMBIOS_PROTOCOL *Smbios;

  //
  // Find the SMBIOS protocol
  //
  Status = gBS->LocateProtocol (
                  &gEfiSmbiosProtocolGuid,
                  NULL,
                  (VOID **)&Smbios
                  );

  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = InstallAllStructures (Smbios);
  DEBUG ((DEBUG_ERROR, "SmbiosPlatform install - %r\n", Status));

  return Status;
}
