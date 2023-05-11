/** @file

  Copyright (c) 2023, Ampere Computing LLC. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef PCIE_HOT_PLUG_PORT_MAP_H_
#define PCIE_HOT_PLUG_PORT_MAP_H_

// "PCIe Bifurcation Mapping" spec just reserve 24x48 bytes (48 ports)
#define MAX_NUMBER_PROCESSORS  2
#define NUMBER_OF_PCIE_PORTS   48
#define MAX_PORT_MAP_ENTRY     (NUMBER_OF_PCIE_PORTS * MAX_NUMBER_PROCESSORS)

//
// - Macro to create value for PCIe Hot Plug configuration from PcieHotPlugPortMapEntry structure.
// - Value structure:
//     Bit [0:7]:    Vport
//     Bit [8:11]:   Socket
//     Bit [12:15]:  RcaPort
//     Bit [16:19]:  RcaSubPort
//     Bit [20:23]:  PinPort
//     Bit [24:31]:  I2cAddress
//     Bit [32:39]:  MuxAddress
//     Bit [40:43]:  MuxChannel
//     Bit [44:51]:  GpioResetNumber
//     Bit [52:55]:  Segment
//     Bit [56: 63]: DriveIndexGpioResetNumber
//
#define PCIE_HOT_PLUG_DECODE_VPORT(Value)              ((((UINTN)(Value).Vport)           & 0xFF) << 0)
#define PCIE_HOT_PLUG_DECODE_SOCKET(Value)             ((((UINTN)(Value).Socket)          & 0x0F) << 8)
#define PCIE_HOT_PLUG_DECODE_RCA_PORT(Value)           ((((UINTN)(Value).RcaPort)         & 0x0F) << 12)
#define PCIE_HOT_PLUG_DECODE_RCA_SUB_PORT(Value)       ((((UINTN)(Value).RcaSubPort)      & 0x0F) << 16)
#define PCIE_HOT_PLUG_DECODE_PIN_PORT(Value)           ((((UINTN)(Value).PinPort)         & 0x0F) << 20)
#define PCIE_HOT_PLUG_DECODE_I2C_ADDRESS(Value)        ((((UINTN)(Value).I2cAddress)      & 0xFF) << 24)
#define PCIE_HOT_PLUG_DECODE_MUX_ADDRESS(Value)        ((((UINTN)(Value).MuxAddress)      & 0xFF) << 32)
#define PCIE_HOT_PLUG_DECODE_MUX_CHANNEL(Value)        ((((UINTN)(Value).MuxChannel)      & 0x0F) << 40)
#define PCIE_HOT_PLUG_DECODE_GPIO_RESET_NUMBER(Value)  ((((UINTN)(Value).GpioResetNumber) & 0xFF) << 44)
#define PCIE_HOT_PLUG_DECODE_SEGMENT(Value)            ((((UINTN)(Value).Segment)         & 0x0F) << 52)
#define PCIE_HOT_PLUG_DECODE_DRIVE_INDEX(Value)        ((((UINTN)(Value).DriveIndex)      & 0xFF) << 56)

#define PCIE_HOT_PLUG_GET_CONFIG_VALUE(Value)  (  \
  PCIE_HOT_PLUG_DECODE_VPORT(Value)             | \
  PCIE_HOT_PLUG_DECODE_SOCKET(Value)            | \
  PCIE_HOT_PLUG_DECODE_RCA_PORT(Value)          | \
  PCIE_HOT_PLUG_DECODE_RCA_SUB_PORT(Value)      | \
  PCIE_HOT_PLUG_DECODE_PIN_PORT(Value)          | \
  PCIE_HOT_PLUG_DECODE_I2C_ADDRESS(Value)       | \
  PCIE_HOT_PLUG_DECODE_MUX_ADDRESS(Value)       | \
  PCIE_HOT_PLUG_DECODE_MUX_CHANNEL(Value)       | \
  PCIE_HOT_PLUG_DECODE_GPIO_RESET_NUMBER(Value) | \
  PCIE_HOT_PLUG_DECODE_SEGMENT(Value)           | \
  PCIE_HOT_PLUG_DECODE_DRIVE_INDEX(Value)         \
)

#pragma pack(1)

typedef struct {
  UINT8    Vport;
  UINT8    Socket;
  UINT8    RcaPort;
  UINT8    RcaSubPort;
  UINT8    PinPort;
  UINT8    I2cAddress;
  UINT8    MuxAddress;
  UINT8    MuxChannel;
  UINT8    GpioResetNumber;
  UINT8    Segment;
  UINT8    DriveIndex;
} PCIE_HOT_PLUG_PORT_MAP_ENTRY;

typedef struct {
  BOOLEAN    UseDefaultConfig;
  UINT8      PortMap[MAX_PORT_MAP_ENTRY][sizeof (PCIE_HOT_PLUG_PORT_MAP_ENTRY)];
} PCIE_HOT_PLUG_PORT_MAP_TABLE;

#pragma pack()

#endif
