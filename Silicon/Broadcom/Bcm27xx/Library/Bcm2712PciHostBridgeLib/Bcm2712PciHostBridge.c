/** @file
 *
 *  Copyright (c) 2024, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <Uefi.h>
#include <IndustryStandard/Bcm2712.h>
#include <IndustryStandard/Bcm2712Pcie.h>
#include <IndustryStandard/Pci.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include "Bcm2712PciHostBridge.h"

EFI_STATUS
PciePhyRead (
  IN BCM2712_PCIE_RC  *Pcie,
  IN UINT8            Register,
  OUT UINT16          *Value
  )
{
  UINT32  Retry;
  UINT32  Data;

  MmioWrite32 (
    Pcie->Base + PCIE_RC_DL_MDIO_ADDR,
    PCIE_RC_DL_MDIO_PACKET (0, Register, PCIE_RC_DL_MDIO_CMD_READ)
    );
  MmioRead32 (Pcie->Base + PCIE_RC_DL_MDIO_ADDR);

  for (Retry = 10; Retry > 0; Retry--) {
    Data = MmioRead32 (Pcie->Base + PCIE_RC_DL_MDIO_RD_DATA);
    if ((Data & PCIE_RC_DL_MDIO_DATA_DONE_MASK) != 0) {
      *Value = (Data & PCIE_RC_DL_MDIO_DATA_MASK) >> PCIE_RC_DL_MDIO_DATA_SHIFT;
      return EFI_SUCCESS;
    }

    gBS->Stall (10);
  }

  return EFI_TIMEOUT;
}

STATIC
EFI_STATUS
PciePhyWrite (
  IN BCM2712_PCIE_RC  *Pcie,
  IN UINT8            Register,
  IN UINT16           Value
  )
{
  UINT32  Retry;
  UINT32  Data;

  MmioWrite32 (
    Pcie->Base + PCIE_RC_DL_MDIO_ADDR,
    PCIE_RC_DL_MDIO_PACKET (0, Register, PCIE_RC_DL_MDIO_CMD_WRITE)
    );
  MmioRead32 (Pcie->Base + PCIE_RC_DL_MDIO_ADDR);
  MmioWrite32 (Pcie->Base + PCIE_RC_DL_MDIO_WR_DATA, PCIE_RC_DL_MDIO_DATA_DONE_MASK | Value);

  for (Retry = 10; Retry > 0; Retry--) {
    Data = MmioRead32 (Pcie->Base + PCIE_RC_DL_MDIO_WR_DATA);
    if ((Data & PCIE_RC_DL_MDIO_DATA_DONE_MASK) == 0) {
      return EFI_SUCCESS;
    }

    gBS->Stall (10);
  }

  return EFI_TIMEOUT;
}

STATIC
VOID
PcieSetupPhy (
  IN BCM2712_PCIE_RC  *Pcie
  )
{
  // Enable PHY SerDes
  MmioAnd32 (
    Pcie->Base + PCIE_MISC_HARD_PCIE_HARD_DEBUG,
    ~PCIE_MISC_HARD_PCIE_HARD_DEBUG_SERDES_IDDQ_MASK
    );
  gBS->Stall (100);

  // Allow a 54 MHz reference clock
  PciePhyWrite (Pcie, MDIO_PHY_SET_ADDR_OFFSET, 0x1600);
  PciePhyWrite (Pcie, 0x16, 0x50b9);
  PciePhyWrite (Pcie, 0x17, 0xbda1);
  PciePhyWrite (Pcie, 0x18, 0x0094);
  PciePhyWrite (Pcie, 0x19, 0x97b4);
  PciePhyWrite (Pcie, 0x1b, 0x5030);
  PciePhyWrite (Pcie, 0x1c, 0x5030);
  PciePhyWrite (Pcie, 0x1e, 0x0007);
  gBS->Stall (100);

  // Tweak PM period for 54 MHz clock
  MmioAndThenOr32 (
    Pcie->Base + PCIE_RC_PL_PHY_CTL_15,
    ~PCIE_RC_PL_PHY_CTL_15_PM_CLK_PERIOD_MASK,
    18 // ns
    );
}

STATIC
UINTN
PcieEncodeInboundSize (
  IN UINT64  Size
  )
{
  UINTN  Log2Size;

  Log2Size = HighBitSet64 (Size);

  if ((Log2Size >= 12) && (Log2Size <= 15)) {
    // 4KB - 32KB
    return (Log2Size - 12) + 0x1c;
  } else if ((Log2Size >= 16) && (Log2Size <= 36)) {
    // 64KB - 64GB
    return Log2Size - 15;
  }

  return 0;
}

STATIC
VOID
PcieSetupInboundWindow (
  IN BCM2712_PCIE_RC       *Pcie,
  IN UINT32                Index,
  IN EFI_PHYSICAL_ADDRESS  CpuBase,
  IN EFI_PHYSICAL_ADDRESS  BusBase,
  IN UINTN                 Size
  )
{
  UINT32  InboundSize;

  InboundSize = PcieEncodeInboundSize (Size);

  MmioWrite32 (
    Pcie->Base + PCIE_IB_WIN_LO (Index),
    ((UINT32)BusBase & ~PCIE_MISC_RC_BAR_CONFIG_LO_SIZE_MASK) | InboundSize
    );
  MmioWrite32 (Pcie->Base + PCIE_IB_WIN_HI (Index), BusBase >> 32);

  MmioWrite32 (
    Pcie->Base + PCIE_IB_WIN_REMAP_LO (Index),
    ((UINT32)CpuBase & PCIE_MISC_UBUS_BAR_CONFIG_REMAP_LO_MASK) |
    PCIE_MISC_UBUS_BAR_CONFIG_REMAP_ENABLE
    );
  MmioWrite32 (
    Pcie->Base + PCIE_IB_WIN_REMAP_HI (Index),
    (CpuBase >> 32) & PCIE_MISC_UBUS_BAR_CONFIG_REMAP_HI_MASK
    );

  if (Index == 0) {
    // Set memory controller size based on the primary window.
    MmioAndThenOr32 (
      Pcie->Base + PCIE_MISC_MISC_CTRL,
      ~PCIE_MISC_MISC_CTRL_SCB0_SIZE_MASK,
      InboundSize << PCIE_MISC_MISC_CTRL_SCB0_SIZE_SHIFT
      );
  }
}

STATIC
VOID
PcieSetupOutboundWindow (
  IN BCM2712_PCIE_RC       *Pcie,
  IN UINT32                Index,
  IN EFI_PHYSICAL_ADDRESS  CpuBase,
  IN EFI_PHYSICAL_ADDRESS  BusBase,
  IN UINTN                 Size
  )
{
  EFI_PHYSICAL_ADDRESS  CpuBaseMB;
  EFI_PHYSICAL_ADDRESS  CpuLimitMB;

  CpuBaseMB  = CpuBase / SIZE_1MB;
  CpuLimitMB = (CpuBase + Size - 1) / SIZE_1MB;

  MmioAndThenOr32 (
    Pcie->Base + PCIE_MEM_WIN_BASE_LIMIT (Index),
    ~(PCIE_MISC_CPU_2_PCIE_MEM_WIN_BASE_LIMIT_BASE_MASK |
      PCIE_MISC_CPU_2_PCIE_MEM_WIN_BASE_LIMIT_LIMIT_MASK),
    CpuBaseMB << PCIE_MISC_CPU_2_PCIE_MEM_WIN_BASE_LIMIT_BASE_SHIFT |
    CpuLimitMB << PCIE_MISC_CPU_2_PCIE_MEM_WIN_BASE_LIMIT_LIMIT_SHIFT
    );

  MmioAndThenOr32 (
    Pcie->Base + PCIE_MEM_WIN_BASE_HI (Index),
    ~PCIE_MISC_CPU_2_PCIE_MEM_WIN_BASE_HI_BASE_MASK,
    CpuBaseMB >> PCIE_MISC_CPU_2_PCIE_MEM_WIN_BASE_LIMIT_MASK_BITS
    );

  MmioAndThenOr32 (
    Pcie->Base + PCIE_MEM_WIN_LIMIT_HI (Index),
    ~PCIE_MISC_CPU_2_PCIE_MEM_WIN_LIMIT_HI_LIMIT_MASK,
    CpuLimitMB >> PCIE_MISC_CPU_2_PCIE_MEM_WIN_BASE_LIMIT_MASK_BITS
    );

  MmioWrite32 (Pcie->Base + PCIE_MEM_WIN_LO (Index), (UINT32)BusBase);
  MmioWrite32 (Pcie->Base + PCIE_MEM_WIN_HI (Index), BusBase >> 32);
}

STATIC
VOID
PcieSetupAxiQosPriority (
  IN BCM2712_PCIE_RC  *Pcie
  )
{
  MmioAnd32 (Pcie->Base + PCIE_MISC_AXI_INTF_CTRL, ~AXI_REQFIFO_EN_QOS_PROPAGATION);

  if (Pcie->Settings->VdmToQosMap == 0) {
    MmioAnd32 (Pcie->Base + PCIE_MISC_CTRL_1, ~PCIE_MISC_CTRL_1_EN_VDM_QOS_CONTROL_MASK);
    return;
  }

  MmioOr32 (Pcie->Base + PCIE_MISC_CTRL_1, PCIE_MISC_CTRL_1_EN_VDM_QOS_CONTROL_MASK);

  MmioWrite32 (Pcie->Base + PCIE_MISC_VDM_PRIORITY_TO_QOS_MAP_LO, Pcie->Settings->VdmToQosMap);
  MmioWrite32 (Pcie->Base + PCIE_MISC_VDM_PRIORITY_TO_QOS_MAP_HI, Pcie->Settings->VdmToQosMap);

  MmioWrite32 (Pcie->Base + PCIE_RC_TL_VDM_CTL1, 0);

  MmioOr32 (
    Pcie->Base + PCIE_RC_TL_VDM_CTL0,
    PCIE_RC_TL_VDM_CTL0_VDM_ENABLED_MASK |
    PCIE_RC_TL_VDM_CTL0_VDM_IGNORETAG_MASK |
    PCIE_RC_TL_VDM_CTL0_VDM_IGNOREVNDRID_MASK
    );
}

STATIC
VOID
PcieSetupAspm (
  IN BCM2712_PCIE_RC  *Pcie
  )
{
  UINT32  Data;
  UINT32  AspmCaps;

  Data  = MmioRead32 (Pcie->Base + PCIE_MISC_HARD_PCIE_HARD_DEBUG);
  Data &= ~PCIE_MISC_HARD_PCIE_HARD_DEBUG_CLKREQ_DEBUG_ENABLE_MASK;
  Data &= ~PCIE_MISC_HARD_PCIE_HARD_DEBUG_CLKREQ_L1SS_ENABLE_MASK;
  //
  // TODO: BCM2712 cannot support both L1 and L1SS at the same time.
  // Only allow L1 for now, but we should read the capabilities of
  // the endpoint and pick L1SS if possible.
  //
  Data |= PCIE_MISC_HARD_PCIE_HARD_DEBUG_CLKREQ_DEBUG_ENABLE_MASK;
  MmioWrite32 (Pcie->Base + PCIE_MISC_HARD_PCIE_HARD_DEBUG, Data);

  AspmCaps = 0;

  if (Pcie->Settings->AspmSupportL0s) {
    AspmCaps |= PCIE_LINK_STATE_L0S;
  }

  if (Pcie->Settings->AspmSupportL1) {
    AspmCaps |= PCIE_LINK_STATE_L1;
  }

  MmioAndThenOr32 (
    Pcie->Base + PCIE_RC_CFG_PRIV1_LINK_CAPABILITY,
    ~PCIE_RC_CFG_PRIV1_LINK_CAPABILITY_ASPM_SUPPORT_MASK,
    AspmCaps << PCIE_RC_CFG_PRIV1_LINK_CAPABILITY_ASPM_SUPPORT_SHIFT
    );
}

STATIC
VOID
PcieSetupLinkSpeed (
  IN BCM2712_PCIE_RC  *Pcie,
  IN UINT8            Speed
  )
{
  if (Speed == 0) {
    return;
  }

  MmioAndThenOr16 (
    Pcie->Base + BRCM_PCIE_CAP_REGS + PCIE_LINK_CONTROL_2,
    ~PCIE_LINK_CONTROL_2_TARGET_LINK_SPEED_MASK,
    Speed
    );

  MmioAndThenOr32 (
    Pcie->Base + BRCM_PCIE_CAP_REGS + PCIE_LINK_CAPABILITIES,
    ~PCIE_LINK_CAPABILITIES_SUPPORTED_LINK_SPEED_MASK,
    Speed
    );
}

STATIC
VOID
PcieAssertPerst (
  IN BCM2712_PCIE_RC  *Pcie,
  IN BOOLEAN          Value
  )
{
  MmioAndThenOr32 (
    Pcie->Base + PCIE_MISC_PCIE_CTRL,
    ~PCIE_MISC_PCIE_CTRL_PCIE_PERSTB_MASK,
    Value ? 0 : PCIE_MISC_PCIE_CTRL_PCIE_PERSTB_MASK
    );
}

STATIC
BOOLEAN
PcieIsLinkUp (
  IN BCM2712_PCIE_RC  *Pcie
  )
{
  UINT32  Status;

  Status = MmioRead32 (Pcie->Base + PCIE_MISC_PCIE_STATUS);

  return (Status & (PCIE_MISC_PCIE_STATUS_PCIE_DL_ACTIVE_MASK |
                    PCIE_MISC_PCIE_STATUS_PCIE_PHYLINKUP_MASK)) != 0;
}

STATIC
EFI_STATUS
PcieWaitForLinkUp (
  IN BCM2712_PCIE_RC  *Pcie
  )
{
  UINT32  Retry;
  UINT16  LinkStatus;
  UINT8   LinkSpeed;
  UINT8   LinkWidth;

  // Wait up to 100 ms
  for (Retry = 20; Retry > 0; Retry--) {
    if (PcieIsLinkUp (Pcie)) {
      break;
    }

    gBS->Stall (5000);
  }

  if (Retry == 0) {
    DEBUG ((DEBUG_ERROR, "PCIe: Link down (timeout)\n"));
    return EFI_TIMEOUT;
  }

  LinkStatus = MmioRead16 (Pcie->Base + BRCM_PCIE_CAP_REGS + PCIE_LINK_STATUS);
  LinkSpeed  = LinkStatus & PCIE_LINK_STATUS_LINK_SPEED_MASK;
  LinkWidth  = (LinkStatus & PCIE_LINK_STATUS_LINK_WIDTH_MASK) >> PCIE_LINK_STATUS_LINK_WIDTH_SHIFT;

  DEBUG ((DEBUG_INFO, "PCIe: Link up (Gen %d x%d)\n", LinkSpeed, LinkWidth));

  return EFI_SUCCESS;
}

EFI_STATUS
PcieInitRc (
  IN BCM2712_PCIE_RC  *Pcie
  )
{
  DEBUG ((DEBUG_INIT, "PCIe: Base: 0x%lx\n", Pcie->Base));
  DEBUG ((DEBUG_INIT, "PCIe: Settings->MaxLinkSpeed: %d\n", Pcie->Settings->MaxLinkSpeed));
  DEBUG ((DEBUG_INIT, "PCIe: Settings->AspmSupportL0s: %d\n", Pcie->Settings->AspmSupportL0s));
  DEBUG ((DEBUG_INIT, "PCIe: Settings->AspmSupportL1: %d\n", Pcie->Settings->AspmSupportL1));
  DEBUG ((DEBUG_INIT, "PCIe: Settings->RcbMatchMps: %d\n", Pcie->Settings->RcbMatchMps));
  DEBUG ((DEBUG_INIT, "PCIe: Settings->VdmToQosMap: %x\n", Pcie->Settings->VdmToQosMap));

  //
  // The VPU firmware has already deasserted the bridge resets.
  // Only touch PERST.
  //
  PcieAssertPerst (Pcie, TRUE);
  gBS->Stall (100);

  PcieSetupPhy (Pcie);

  MmioAndThenOr32 (
    Pcie->Base + PCIE_MISC_MISC_CTRL,
    ~PCIE_MISC_MISC_CTRL_MAX_BURST_SIZE_MASK,
    PCIE_MISC_MISC_CTRL_MAX_BURST_SIZE_128 << PCIE_MISC_MISC_CTRL_MAX_BURST_SIZE_SHIFT |
    PCIE_MISC_MISC_CTRL_SCB_ACCESS_EN_MASK |
    PCIE_MISC_MISC_CTRL_CFG_READ_UR_MODE_MASK |
    Pcie->Settings->RcbMatchMps ? PCIE_MISC_MISC_CTRL_PCIE_RCB_MPS_MODE_MASK : 0
    );

  //
  // Disable AXI bus errors to avoid Arm SError exceptions
  // when the link is down.
  //
  MmioOr32 (
    Pcie->Base + PCIE_MISC_UBUS_CTRL,
    PCIE_MISC_UBUS_CTRL_UBUS_PCIE_REPLY_ERR_DIS_MASK |
    PCIE_MISC_UBUS_CTRL_UBUS_PCIE_REPLY_DECERR_DIS_MASK
    );
  MmioWrite32 (Pcie->Base + PCIE_MISC_AXI_READ_ERROR_DATA, PCI_INVALID_ADDRESS);

  //
  // Set UBUS timeout to ~250ms and CRS timeout to ~240ms.
  //
  MmioWrite32 (Pcie->Base + PCIE_MISC_UBUS_TIMEOUT, 0xb2d0000);
  MmioWrite32 (Pcie->Base + PCIE_MISC_RC_CONFIG_RETRY_TIMEOUT, 0xaba0000);

  PcieSetupAxiQosPriority (Pcie);

  //
  // Disable GISB & SCB windows
  //
  MmioAnd32 (Pcie->Base + PCIE_MISC_RC_BAR1_CONFIG_LO, ~PCIE_MISC_RC_BAR_CONFIG_LO_SIZE_MASK);
  MmioAnd32 (Pcie->Base + PCIE_MISC_RC_BAR3_CONFIG_LO, ~PCIE_MISC_RC_BAR_CONFIG_LO_SIZE_MASK);

  //
  // Map inbound windows
  //
  PcieSetupInboundWindow (
    Pcie,
    0,
    Pcie->Inbound.CpuBase,
    Pcie->Inbound.BusBase,
    Pcie->Inbound.Size
    );

  //
  // Map outbound windows
  //
  PcieSetupOutboundWindow (
    Pcie,
    0,
    Pcie->Mem32.CpuBase,
    Pcie->Mem32.BusBase,
    Pcie->Mem32.Size
    );

  PcieSetupOutboundWindow (
    Pcie,
    1,
    Pcie->Mem64.CpuBase,
    Pcie->Mem64.BusBase,
    Pcie->Mem64.Size
    );

  // Clear and mask interrupts
  MmioWrite32 (Pcie->Base + PCIE_INTR2_CPU_BASE + PCIE_INTR2_CPU_MASK_CLR, 0xffffffff);
  MmioWrite32 (Pcie->Base + PCIE_INTR2_CPU_BASE + PCIE_INTR2_CPU_MASK_SET, 0xffffffff);

  // Program PCI to PCI bridge class into the root port
  MmioAndThenOr32 (
    Pcie->Base + PCIE_RC_CFG_PRIV1_ID_VAL3,
    ~PCIE_RC_CFG_PRIV1_ID_VAL3_CLASS_CODE_MASK,
    ((PCI_CLASS_BRIDGE << 8) | PCI_CLASS_BRIDGE_P2P) << 8
    );

  // PCIe->SCB little-endian mode for BAR
  MmioAndThenOr32 (
    Pcie->Base + PCIE_RC_CFG_VENDOR_VENDOR_SPECIFIC_REG1,
    ~PCIE_RC_CFG_VENDOR_VENDOR_SPECIFIC_REG1_ENDIAN_MODE_BAR2_MASK,
    PCIE_RC_CFG_VENDOR_SPCIFIC_REG1_LITTLE_ENDIAN
    << PCIE_RC_CFG_VENDOR_VENDOR_SPECIFIC_REG1_ENDIAN_MODE_BAR2_SHIFT
    );

  PcieSetupLinkSpeed (Pcie, Pcie->Settings->MaxLinkSpeed);

  PcieSetupAspm (Pcie);

  // Start link-up
  PcieAssertPerst (Pcie, FALSE);
  gBS->Stall (100000);

  PcieWaitForLinkUp (Pcie);

  return EFI_SUCCESS;
}
