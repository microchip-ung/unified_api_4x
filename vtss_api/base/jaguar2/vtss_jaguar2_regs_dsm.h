#ifndef _VTSS_JAGUAR2_REGS_DSM_H_
#define _VTSS_JAGUAR2_REGS_DSM_H_

/*


 Copyright (c) 2002-2017 Microsemi Corporation "Microsemi". All Rights Reserved.

 Unpublished rights reserved under the copyright laws of the United States of
 America, other countries and international treaties. Permission to use, copy,
 store and modify, the software and its source code is granted but only in
 connection with products utilizing the Microsemi switch and PHY products.
 Permission is also granted for you to integrate into other products, disclose,
 transmit and distribute the software only in an absolute machine readable format
 (e.g. HEX file) and only in or with products utilizing the Microsemi switch and
 PHY products.  The source code of the software may not be disclosed, transmitted
 or distributed without the prior written permission of Microsemi.

 This copyright notice must appear in any copy, modification, disclosure,
 transmission or distribution of the software.  Microsemi retains all ownership,
 copyright, trade secret and proprietary rights in the software and its source code,
 including all modifications thereto.

 THIS SOFTWARE HAS BEEN PROVIDED "AS IS". MICROSEMI HEREBY DISCLAIMS ALL WARRANTIES
 OF ANY KIND WITH RESPECT TO THE SOFTWARE, WHETHER SUCH WARRANTIES ARE EXPRESS,
 IMPLIED, STATUTORY OR OTHERWISE INCLUDING, WITHOUT LIMITATION, WARRANTIES OF
 MERCHANTABILITY, FITNESS FOR A PARTICULAR USE OR PURPOSE AND NON-INFRINGEMENT.

*/

#include "vtss_jaguar2_regs_common.h"

/*********************************************************************** 
 *
 * Target: \a DSM
 *
 * Disassembler
 *
 ***********************************************************************/

/**
 * Register Group: \a DSM:RAM_CTRL
 *
 * Access core memory
 */


/** 
 * \brief Core reset control
 *
 * \details
 * Controls reset and initialization of the switching core. Proper startup
 * sequence is:
 * - Enable memories
 * - Initialize memories
 * - Enable core
 *
 * Register: \a DSM:RAM_CTRL:RAM_INIT
 */
#define VTSS_DSM_RAM_CTRL_RAM_INIT           VTSS_IOREG(VTSS_TO_DSM,0x2)

/** 
 * \brief
 * Initialize core memories. Field is automatically cleared when operation
 * is complete ( approx. 40 us).
 *
 * \details 
 * Field: ::VTSS_DSM_RAM_CTRL_RAM_INIT . RAM_INIT
 */
#define  VTSS_F_DSM_RAM_CTRL_RAM_INIT_RAM_INIT(x)  VTSS_ENCODE_BITFIELD(!!(x),1,1)
#define  VTSS_M_DSM_RAM_CTRL_RAM_INIT_RAM_INIT  VTSS_BIT(1)
#define  VTSS_X_DSM_RAM_CTRL_RAM_INIT_RAM_INIT(x)  VTSS_EXTRACT_BITFIELD(x,1,1)

/** 
 * \brief
 * Core memory controllers are enabled when this field is set.
 *
 * \details 
 * Field: ::VTSS_DSM_RAM_CTRL_RAM_INIT . RAM_ENA
 */
#define  VTSS_F_DSM_RAM_CTRL_RAM_INIT_RAM_ENA(x)  VTSS_ENCODE_BITFIELD(!!(x),0,1)
#define  VTSS_M_DSM_RAM_CTRL_RAM_INIT_RAM_ENA  VTSS_BIT(0)
#define  VTSS_X_DSM_RAM_CTRL_RAM_INIT_RAM_ENA(x)  VTSS_EXTRACT_BITFIELD(x,0,1)

/**
 * Register Group: \a DSM:COREMEM
 *
 * Access core memory
 */


/** 
 * \brief Address selection
 *
 * \details
 * Register: \a DSM:COREMEM:CM_ADDR
 */
#define VTSS_DSM_COREMEM_CM_ADDR             VTSS_IOREG(VTSS_TO_DSM,0x0)

/** 
 * \brief
 * Refer to cmid.xls in the AS1000, misc_docs folder.
 *
 * \details 
 * Field: ::VTSS_DSM_COREMEM_CM_ADDR . CM_ID
 */
#define  VTSS_F_DSM_COREMEM_CM_ADDR_CM_ID(x)  VTSS_ENCODE_BITFIELD(x,22,8)
#define  VTSS_M_DSM_COREMEM_CM_ADDR_CM_ID     VTSS_ENCODE_BITMASK(22,8)
#define  VTSS_X_DSM_COREMEM_CM_ADDR_CM_ID(x)  VTSS_EXTRACT_BITFIELD(x,22,8)

/** 
 * \brief
 * Address selection within selected core memory (CMID register). Address
 * is automatically advanced at every data access.
 *
 * \details 
 * Field: ::VTSS_DSM_COREMEM_CM_ADDR . CM_ADDR
 */
#define  VTSS_F_DSM_COREMEM_CM_ADDR_CM_ADDR(x)  VTSS_ENCODE_BITFIELD(x,0,22)
#define  VTSS_M_DSM_COREMEM_CM_ADDR_CM_ADDR     VTSS_ENCODE_BITMASK(0,22)
#define  VTSS_X_DSM_COREMEM_CM_ADDR_CM_ADDR(x)  VTSS_EXTRACT_BITFIELD(x,0,22)


/** 
 * \brief Data register for core memory access.
 *
 * \details
 * Register: \a DSM:COREMEM:CM_DATA
 */
#define VTSS_DSM_COREMEM_CM_DATA             VTSS_IOREG(VTSS_TO_DSM,0x1)

/**
 * Register Group: \a DSM:CFG
 *
 * Configuration registers
 */


/** 
 * \brief Configuration per device buffer.
 *
 * \details
 * Miscellaneous configurations per device buffer.
 *
 * Register: \a DSM:CFG:BUF_CFG
 *
 * @param ri Replicator: x_DSM_PORTS (??), 0-54
 */
#define VTSS_DSM_CFG_BUF_CFG(ri)             VTSS_IOREG(VTSS_TO_DSM,0x3 + (ri))

/** 
 * \brief
 * Disables updates of the DSM related counter for the common statistics
 * counter system that is used for DEV1G/DEV2G5 devices
 * (ASM:DEV_STATISTICS:TX_PAUSE_CNT).
 * Set this when the port utilizes a DEV10G device as it handles the
 * statistics locally in the device (Statistics counters in
 * DEV10G:DEV_STATISTICS_*:*).
 *
 * \details 
 * Field: ::VTSS_DSM_CFG_BUF_CFG . CSC_STAT_DIS
 */
#define  VTSS_F_DSM_CFG_BUF_CFG_CSC_STAT_DIS(x)  VTSS_ENCODE_BITFIELD(!!(x),1,1)
#define  VTSS_M_DSM_CFG_BUF_CFG_CSC_STAT_DIS  VTSS_BIT(1)
#define  VTSS_X_DSM_CFG_BUF_CFG_CSC_STAT_DIS(x)  VTSS_EXTRACT_BITFIELD(x,1,1)

/** 
 * \brief
 * Enable aging of frames stuck in the DSM buffer system for long periods.
 * Aging is done by comparing the age era stamped into the frame with the
 * current age era. This check is only performed at SOF. If the current age
 * era is more than one higher than the era stamped into the frame, then
 * the frame is discarded.
 * 
 * Frame aging related parameters:
 * QSYS:SYSTEM:FRM_AGING.MAX_AGE
 * HSCH:HSCH_MISC:PORT_MODE.AGE_DIS
 * DSM:CFG:BUF_CFG.AGING_ENA
 *
 * \details 
 * '0': Aging disabled.
 * '1': Aging enabled.
 *
 * Field: ::VTSS_DSM_CFG_BUF_CFG . AGING_ENA
 */
#define  VTSS_F_DSM_CFG_BUF_CFG_AGING_ENA(x)  VTSS_ENCODE_BITFIELD(!!(x),0,1)
#define  VTSS_M_DSM_CFG_BUF_CFG_AGING_ENA     VTSS_BIT(0)
#define  VTSS_X_DSM_CFG_BUF_CFG_AGING_ENA(x)  VTSS_EXTRACT_BITFIELD(x,0,1)


/** 
 * \brief txAdditionalFrameOverhead configuration
 *
 * \details
 * Configuration of the txAdditionalFrameOverhead in RateLimit
 *
 * Register: \a DSM:CFG:RATE_CTRL
 *
 * @param ri Replicator: x_DSM_PORTS (??), 0-54
 */
#define VTSS_DSM_CFG_RATE_CTRL(ri)           VTSS_IOREG(VTSS_TO_DSM,0x3a + (ri))

/** 
 * \brief
 * If Rate Limit mode Frame Rate Overhead is enabled this bitgroup is used
 * for configuration of txAdditionalFrameOverhead.
 *
 * \details 
 * Field: ::VTSS_DSM_CFG_RATE_CTRL . FRM_GAP_COMP
 */
#define  VTSS_F_DSM_CFG_RATE_CTRL_FRM_GAP_COMP(x)  VTSS_ENCODE_BITFIELD(x,24,8)
#define  VTSS_M_DSM_CFG_RATE_CTRL_FRM_GAP_COMP     VTSS_ENCODE_BITMASK(24,8)
#define  VTSS_X_DSM_CFG_RATE_CTRL_FRM_GAP_COMP(x)  VTSS_EXTRACT_BITFIELD(x,24,8)


/** 
 * \brief Configuration register for IPG shrink mode
 *
 * \details
 * Configuration register for IPG shrink mode
 *
 * Register: \a DSM:CFG:IPG_SHRINK_CFG
 *
 * @param ri Replicator: x_DSM_PORTS (??), 0-54
 */
#define VTSS_DSM_CFG_IPG_SHRINK_CFG(ri)      VTSS_IOREG(VTSS_TO_DSM,0x71 + (ri))

/** 
 * \brief
 * Enable for preamble shrink in IPG shrink mode of DEV10G, DEV24G.
 * Preamble shrink is only allowed when IPG shrink mode is enabled.

 *
 * \details 
 * Field: ::VTSS_DSM_CFG_IPG_SHRINK_CFG . IPG_PREAM_SHRINK_ENA
 */
#define  VTSS_F_DSM_CFG_IPG_SHRINK_CFG_IPG_PREAM_SHRINK_ENA(x)  VTSS_ENCODE_BITFIELD(!!(x),1,1)
#define  VTSS_M_DSM_CFG_IPG_SHRINK_CFG_IPG_PREAM_SHRINK_ENA  VTSS_BIT(1)
#define  VTSS_X_DSM_CFG_IPG_SHRINK_CFG_IPG_PREAM_SHRINK_ENA(x)  VTSS_EXTRACT_BITFIELD(x,1,1)

/** 
 * \brief
 * Enable for IPG shrink mode of DEV10G, DEV24G.
 * In shrink mode frame gap compensation is depending on frame size.
 *
 * \details 
 * Field: ::VTSS_DSM_CFG_IPG_SHRINK_CFG . IPG_SHRINK_ENA
 */
#define  VTSS_F_DSM_CFG_IPG_SHRINK_CFG_IPG_SHRINK_ENA(x)  VTSS_ENCODE_BITFIELD(!!(x),0,1)
#define  VTSS_M_DSM_CFG_IPG_SHRINK_CFG_IPG_SHRINK_ENA  VTSS_BIT(0)
#define  VTSS_X_DSM_CFG_IPG_SHRINK_CFG_IPG_SHRINK_ENA(x)  VTSS_EXTRACT_BITFIELD(x,0,1)


/** 
 * \brief Clear a single buffer in the DSM.
 *
 * \details
 * This register controls clearing of buffers in the DSM.
 *
 * Register: \a DSM:CFG:CLR_BUF
 *
 * @param ri Replicator: x_DSM_PORTS_DIV32_CEIL (??), 0-1
 */
#define VTSS_DSM_CFG_CLR_BUF(ri)             VTSS_IOREG(VTSS_TO_DSM,0xa8 + (ri))


/** 
 * \brief SCH stop fill level
 *
 * \details
 * SCH stop fill level
 *
 * Register: \a DSM:CFG:SCH_STOP_WM_CFG
 *
 * @param ri Replicator: x_DSM_PORTS (??), 0-54
 */
#define VTSS_DSM_CFG_SCH_STOP_WM_CFG(ri)     VTSS_IOREG(VTSS_TO_DSM,0xaa + (ri))

/** 
 * \brief
 * DSM buffer fill level at which SCH is stopped to send to this device.
 *
 * \details 
 * The SCH is stopped when the fill level is above the configured value.
 * When set to 0 the hardware will use a default watermark set to the mid
 * of the buffer.
 *
 * Field: ::VTSS_DSM_CFG_SCH_STOP_WM_CFG . SCH_STOP_WM
 */
#define  VTSS_F_DSM_CFG_SCH_STOP_WM_CFG_SCH_STOP_WM(x)  VTSS_ENCODE_BITFIELD(x,0,9)
#define  VTSS_M_DSM_CFG_SCH_STOP_WM_CFG_SCH_STOP_WM     VTSS_ENCODE_BITMASK(0,9)
#define  VTSS_X_DSM_CFG_SCH_STOP_WM_CFG_SCH_STOP_WM(x)  VTSS_EXTRACT_BITFIELD(x,0,9)


/** 
 * \brief Transmit start fill level
 *
 * \details
 * Transmit start fill level
 *
 * Register: \a DSM:CFG:TX_START_WM_CFG
 *
 * @param ri Replicator: x_DSM_PORTS (??), 0-54
 */
#define VTSS_DSM_CFG_TX_START_WM_CFG(ri)     VTSS_IOREG(VTSS_TO_DSM,0xe1 + (ri))

/** 
 * \brief
 * The number of 4-bytes words in DSM buffer required before the DSM will
 * start to transmit on the taxi bus. The fill level must above a certain
 * threshold to avoid underflow in the devices. Single cell frames will be
 * transfered immediately.

 *
 * \details 
 * The DSM will not begin transmission until the fill level is above this
 * watermark or one complete frame is present in the buffer.
 *
 * Field: ::VTSS_DSM_CFG_TX_START_WM_CFG . TX_START_WM
 */
#define  VTSS_F_DSM_CFG_TX_START_WM_CFG_TX_START_WM(x)  VTSS_ENCODE_BITFIELD(x,0,9)
#define  VTSS_M_DSM_CFG_TX_START_WM_CFG_TX_START_WM     VTSS_ENCODE_BITMASK(0,9)
#define  VTSS_X_DSM_CFG_TX_START_WM_CFG_TX_START_WM(x)  VTSS_EXTRACT_BITFIELD(x,0,9)


/** 
 * \brief Token count tx stop watermark
 *
 * \details
 * Token count tx stop watermark
 *
 * Register: \a DSM:CFG:DEV_TX_STOP_WM_CFG
 *
 * @param ri Replicator: x_DSM_PORTS (??), 0-54
 */
#define VTSS_DSM_CFG_DEV_TX_STOP_WM_CFG(ri)  VTSS_IOREG(VTSS_TO_DSM,0x118 + (ri))

/** 
 * \brief
 * Set this to 0 to disable the fast startup of frames in the token system.
 *
 * \details 
 * Field: ::VTSS_DSM_CFG_DEV_TX_STOP_WM_CFG . FAST_STARTUP_ENA
 */
#define  VTSS_F_DSM_CFG_DEV_TX_STOP_WM_CFG_FAST_STARTUP_ENA(x)  VTSS_ENCODE_BITFIELD(!!(x),7,1)
#define  VTSS_M_DSM_CFG_DEV_TX_STOP_WM_CFG_FAST_STARTUP_ENA  VTSS_BIT(7)
#define  VTSS_X_DSM_CFG_DEV_TX_STOP_WM_CFG_FAST_STARTUP_ENA(x)  VTSS_EXTRACT_BITFIELD(x,7,1)

/** 
 * \brief
 * Set this to '1' when a 10G capable port runs at speeds below 10G.
 *
 * \details 
 * Field: ::VTSS_DSM_CFG_DEV_TX_STOP_WM_CFG . DEV10G_SHADOW_ENA
 */
#define  VTSS_F_DSM_CFG_DEV_TX_STOP_WM_CFG_DEV10G_SHADOW_ENA(x)  VTSS_ENCODE_BITFIELD(!!(x),6,1)
#define  VTSS_M_DSM_CFG_DEV_TX_STOP_WM_CFG_DEV10G_SHADOW_ENA  VTSS_BIT(6)
#define  VTSS_X_DSM_CFG_DEV_TX_STOP_WM_CFG_DEV10G_SHADOW_ENA(x)  VTSS_EXTRACT_BITFIELD(x,6,1)

/** 
 * \brief
 * When the token counter value for the device exceeds this value the DSM
 * will stop transmission to the device. When set to 0 a hardware
 * calculated default value is used.
 * When a port is configured for HDX this WM must be set to 1.
 *
 * \details 
 * Field: ::VTSS_DSM_CFG_DEV_TX_STOP_WM_CFG . DEV_TX_STOP_WM
 */
#define  VTSS_F_DSM_CFG_DEV_TX_STOP_WM_CFG_DEV_TX_STOP_WM(x)  VTSS_ENCODE_BITFIELD(x,0,6)
#define  VTSS_M_DSM_CFG_DEV_TX_STOP_WM_CFG_DEV_TX_STOP_WM     VTSS_ENCODE_BITMASK(0,6)
#define  VTSS_X_DSM_CFG_DEV_TX_STOP_WM_CFG_DEV_TX_STOP_WM(x)  VTSS_EXTRACT_BITFIELD(x,0,6)


/** 
 * \brief Pause frame handling in RX direction
 *
 * \details
 * Pause frame handling in RX direction
 *
 * Register: \a DSM:CFG:RX_PAUSE_CFG
 *
 * @param ri Replicator: x_DSM_PORTS (??), 0-54
 */
#define VTSS_DSM_CFG_RX_PAUSE_CFG(ri)        VTSS_IOREG(VTSS_TO_DSM,0x14f + (ri))

/** 
 * \brief
 * Rx Pause Enable: Enables Flow Control in Rx direction:
 *
 * \details 
 * '0': Flow Control Disabled
 * '1': Flow Control Enabled.
 *
 * Field: ::VTSS_DSM_CFG_RX_PAUSE_CFG . RX_PAUSE_EN
 */
#define  VTSS_F_DSM_CFG_RX_PAUSE_CFG_RX_PAUSE_EN(x)  VTSS_ENCODE_BITFIELD(!!(x),1,1)
#define  VTSS_M_DSM_CFG_RX_PAUSE_CFG_RX_PAUSE_EN  VTSS_BIT(1)
#define  VTSS_X_DSM_CFG_RX_PAUSE_CFG_RX_PAUSE_EN(x)  VTSS_EXTRACT_BITFIELD(x,1,1)

/** 
 * \brief
 * Configures for each Ethernet port whether flow control is obeyed locally
 * in the DSM or if the flow_control information is sent to the SCH.
 * If not all priorities should obey flow control for this port, then the
 * DSM should not stop the data stream in a flow control case, but let the
 * SCH stop the traffic to avoid head of line blocking.
 * If all priorities should obey flow control for this port, then the DSM
 * should stop the data stream locally to be able to pass 802.3 conformance
 * testing. If the data stream is stopped by the SCH frames in the OQS can
 * not be aged, thus the Allocation Bitmaps in the QS must not recover lost
 * elements.
 *
 * \details 
 * '0': This Ethernet port obeys flow control locally in the DSM. 
 * '1': This Ethernet port sends flow control information to the SCH and
 * does not stop traffic in the DSM.
 *
 * Field: ::VTSS_DSM_CFG_RX_PAUSE_CFG . FC_OBEY_LOCAL
 */
#define  VTSS_F_DSM_CFG_RX_PAUSE_CFG_FC_OBEY_LOCAL(x)  VTSS_ENCODE_BITFIELD(!!(x),0,1)
#define  VTSS_M_DSM_CFG_RX_PAUSE_CFG_FC_OBEY_LOCAL  VTSS_BIT(0)
#define  VTSS_X_DSM_CFG_RX_PAUSE_CFG_FC_OBEY_LOCAL(x)  VTSS_EXTRACT_BITFIELD(x,0,1)


/** 
 * \brief FC configuration for Ethernet ports.
 *
 * \details
 * FC configuration for Ethernet ports.
 *
 * Register: \a DSM:CFG:ETH_FC_CFG
 *
 * @param ri Replicator: x_DSM_PORTS (??), 0-54
 */
#define VTSS_DSM_CFG_ETH_FC_CFG(ri)          VTSS_IOREG(VTSS_TO_DSM,0x186 + (ri))

/** 
 * \brief
 * Obey FC status received from ANA
 *
 * \details 
 * Field: ::VTSS_DSM_CFG_ETH_FC_CFG . FC_ANA_ENA
 */
#define  VTSS_F_DSM_CFG_ETH_FC_CFG_FC_ANA_ENA(x)  VTSS_ENCODE_BITFIELD(!!(x),1,1)
#define  VTSS_M_DSM_CFG_ETH_FC_CFG_FC_ANA_ENA  VTSS_BIT(1)
#define  VTSS_X_DSM_CFG_ETH_FC_CFG_FC_ANA_ENA(x)  VTSS_EXTRACT_BITFIELD(x,1,1)

/** 
 * \brief
 * Obey FC status received from QSYS
 *
 * \details 
 * Field: ::VTSS_DSM_CFG_ETH_FC_CFG . FC_QS_ENA
 */
#define  VTSS_F_DSM_CFG_ETH_FC_CFG_FC_QS_ENA(x)  VTSS_ENCODE_BITFIELD(!!(x),0,1)
#define  VTSS_M_DSM_CFG_ETH_FC_CFG_FC_QS_ENA  VTSS_BIT(0)
#define  VTSS_X_DSM_CFG_ETH_FC_CFG_FC_QS_ENA(x)  VTSS_EXTRACT_BITFIELD(x,0,1)


/** 
 * \brief PFC configuration for Ethernet ports.
 *
 * \details
 * PFC configuration for Ethernet ports.
 *
 * Register: \a DSM:CFG:ETH_PFC_CFG
 *
 * @param ri Replicator: x_DSM_PORTS (??), 0-54
 */
#define VTSS_DSM_CFG_ETH_PFC_CFG(ri)         VTSS_IOREG(VTSS_TO_DSM,0x1bd + (ri))

/** 
 * \brief
 * Minimum time between two PFC PDUs when PFC state changes after
 * transmission of PFC PDU.
 *
 * \details 
 * Field: ::VTSS_DSM_CFG_ETH_PFC_CFG . PFC_MIN_UPDATE_TIME
 */
#define  VTSS_F_DSM_CFG_ETH_PFC_CFG_PFC_MIN_UPDATE_TIME(x)  VTSS_ENCODE_BITFIELD(x,2,15)
#define  VTSS_M_DSM_CFG_ETH_PFC_CFG_PFC_MIN_UPDATE_TIME     VTSS_ENCODE_BITMASK(2,15)
#define  VTSS_X_DSM_CFG_ETH_PFC_CFG_PFC_MIN_UPDATE_TIME(x)  VTSS_EXTRACT_BITFIELD(x,2,15)

/** 
 * \brief
 * Upon sending  PFC PDU with flow control deasserted for all priorities,
 * enforce a PFC_MIN_UPDATE_TIME delay before allowing transmission of next
 * PFC PDU.
 *
 * \details 
 * Field: ::VTSS_DSM_CFG_ETH_PFC_CFG . PFC_XOFF_MIN_UPDATE_ENA
 */
#define  VTSS_F_DSM_CFG_ETH_PFC_CFG_PFC_XOFF_MIN_UPDATE_ENA(x)  VTSS_ENCODE_BITFIELD(!!(x),1,1)
#define  VTSS_M_DSM_CFG_ETH_PFC_CFG_PFC_XOFF_MIN_UPDATE_ENA  VTSS_BIT(1)
#define  VTSS_X_DSM_CFG_ETH_PFC_CFG_PFC_XOFF_MIN_UPDATE_ENA(x)  VTSS_EXTRACT_BITFIELD(x,1,1)

/** 
 * \brief
 * Enable PFC operation for the port.
 *
 * \details 
 * Field: ::VTSS_DSM_CFG_ETH_PFC_CFG . PFC_ENA
 */
#define  VTSS_F_DSM_CFG_ETH_PFC_CFG_PFC_ENA(x)  VTSS_ENCODE_BITFIELD(!!(x),0,1)
#define  VTSS_M_DSM_CFG_ETH_PFC_CFG_PFC_ENA   VTSS_BIT(0)
#define  VTSS_X_DSM_CFG_ETH_PFC_CFG_PFC_ENA(x)  VTSS_EXTRACT_BITFIELD(x,0,1)


/** 
 * \brief MAC Configuration Register.
 *
 * \details
 * Contains configuration for flowcontrol and operation in FDX or HDX for
 * Ethernet ports connected to the DSM.
 *
 * Register: \a DSM:CFG:MAC_CFG
 *
 * @param ri Replicator: x_DSM_PORTS (??), 0-54
 */
#define VTSS_DSM_CFG_MAC_CFG(ri)             VTSS_IOREG(VTSS_TO_DSM,0x1f4 + (ri))

/** 
 * \brief
 * Tx Pause Value: The pause value inserted in each Pause Control Frame in
 * the Tx direction. It is also used to schedule the transmission of Pause
 * Control Frames when Flow Control is enabled and active. If flow control
 * is enabled, the pause value must be set to a value different from 0,
 * based on the link characteristics.
 *
 * \details 
 * 0x0000: 0 quanta (512-bit times) 
 * 0x0001: 1 quanta
 * ...
 * 0xFFFF: 64K-1 quanta.
 *
 * Field: ::VTSS_DSM_CFG_MAC_CFG . TX_PAUSE_VAL
 */
#define  VTSS_F_DSM_CFG_MAC_CFG_TX_PAUSE_VAL(x)  VTSS_ENCODE_BITFIELD(x,16,16)
#define  VTSS_M_DSM_CFG_MAC_CFG_TX_PAUSE_VAL     VTSS_ENCODE_BITMASK(16,16)
#define  VTSS_X_DSM_CFG_MAC_CFG_TX_PAUSE_VAL(x)  VTSS_EXTRACT_BITFIELD(x,16,16)

/** 
 * \brief
 * Enables HDX backpressure instead of FDX FC when FC is generated.
 * Note: 10G and 24G ports can not run HDX, so for those ports this field
 * must always be set to '0'.
 *
 * \details 
 * '0': FDX FC is used.
 * '1': HDX backpreassure is used.
 *
 * Field: ::VTSS_DSM_CFG_MAC_CFG . HDX_BACKPRESSURE
 */
#define  VTSS_F_DSM_CFG_MAC_CFG_HDX_BACKPRESSURE(x)  VTSS_ENCODE_BITFIELD(!!(x),2,1)
#define  VTSS_M_DSM_CFG_MAC_CFG_HDX_BACKPRESSURE  VTSS_BIT(2)
#define  VTSS_X_DSM_CFG_MAC_CFG_HDX_BACKPRESSURE(x)  VTSS_EXTRACT_BITFIELD(x,2,1)

/** 
 * \brief
 * Defines whether or not each pause frame will be sent twice.
 *
 * \details 
 * '0': Send one pause frame two times per pause period.
 * '1': Send two pause frames back to back two times per pause period.
 *
 * Field: ::VTSS_DSM_CFG_MAC_CFG . SEND_PAUSE_FRM_TWICE
 */
#define  VTSS_F_DSM_CFG_MAC_CFG_SEND_PAUSE_FRM_TWICE(x)  VTSS_ENCODE_BITFIELD(!!(x),1,1)
#define  VTSS_M_DSM_CFG_MAC_CFG_SEND_PAUSE_FRM_TWICE  VTSS_BIT(1)
#define  VTSS_X_DSM_CFG_MAC_CFG_SEND_PAUSE_FRM_TWICE(x)  VTSS_EXTRACT_BITFIELD(x,1,1)

/** 
 * \brief
 * TX pause zero on deassert. Determines whether or not a pause control
 * frame with pause value zero is transmitted when flow control is
 * deasserted.
 *
 * \details 
 * '0': No pause frame with value zero is transmitted.
 * '1': Pause frame with value zero is transmitted when flow control
 * becomes inactive.
 *
 * Field: ::VTSS_DSM_CFG_MAC_CFG . TX_PAUSE_XON_XOFF
 */
#define  VTSS_F_DSM_CFG_MAC_CFG_TX_PAUSE_XON_XOFF(x)  VTSS_ENCODE_BITFIELD(!!(x),0,1)
#define  VTSS_M_DSM_CFG_MAC_CFG_TX_PAUSE_XON_XOFF  VTSS_BIT(0)
#define  VTSS_X_DSM_CFG_MAC_CFG_TX_PAUSE_XON_XOFF(x)  VTSS_EXTRACT_BITFIELD(x,0,1)


/** 
 * \brief MAC Address Base Configuration Register - bits 47-24.
 *
 * \details
 * MAC base address.
 * Used when generating Pause Control Frames with the specified MAC
 * address.
 *
 * Register: \a DSM:CFG:MAC_ADDR_BASE_HIGH_CFG
 *
 * @param ri Replicator: x_DSM_NUM_PAUSE_FRM_MAC_ADDR (??), 0-52
 */
#define VTSS_DSM_CFG_MAC_ADDR_BASE_HIGH_CFG(ri)  VTSS_IOREG(VTSS_TO_DSM,0x22b + (ri))

/** 
 * \brief
 * Bits 47-24 of MAC base address.
 * Used when generating Pause Control Frames with the specified MAC
 * address.
 *
 * \details 
 * Field: ::VTSS_DSM_CFG_MAC_ADDR_BASE_HIGH_CFG . MAC_ADDR_HIGH
 */
#define  VTSS_F_DSM_CFG_MAC_ADDR_BASE_HIGH_CFG_MAC_ADDR_HIGH(x)  VTSS_ENCODE_BITFIELD(x,0,24)
#define  VTSS_M_DSM_CFG_MAC_ADDR_BASE_HIGH_CFG_MAC_ADDR_HIGH     VTSS_ENCODE_BITMASK(0,24)
#define  VTSS_X_DSM_CFG_MAC_ADDR_BASE_HIGH_CFG_MAC_ADDR_HIGH(x)  VTSS_EXTRACT_BITFIELD(x,0,24)


/** 
 * \brief MAC Address Base Configuration Register - bits 23-0.
 *
 * \details
 * MAC base address.
 * Used when generating Pause Control Frames with the specified MAC
 * address.
 *
 * Register: \a DSM:CFG:MAC_ADDR_BASE_LOW_CFG
 *
 * @param ri Replicator: x_DSM_NUM_PAUSE_FRM_MAC_ADDR (??), 0-52
 */
#define VTSS_DSM_CFG_MAC_ADDR_BASE_LOW_CFG(ri)  VTSS_IOREG(VTSS_TO_DSM,0x260 + (ri))

/** 
 * \brief
 * Bits 23-0 of MAC base address.
 * Used when generating Pause Control Frames with the specified MAC
 * address.
 *
 * \details 
 * Field: ::VTSS_DSM_CFG_MAC_ADDR_BASE_LOW_CFG . MAC_ADDR_LOW
 */
#define  VTSS_F_DSM_CFG_MAC_ADDR_BASE_LOW_CFG_MAC_ADDR_LOW(x)  VTSS_ENCODE_BITFIELD(x,0,24)
#define  VTSS_M_DSM_CFG_MAC_ADDR_BASE_LOW_CFG_MAC_ADDR_LOW     VTSS_ENCODE_BITMASK(0,24)
#define  VTSS_X_DSM_CFG_MAC_ADDR_BASE_LOW_CFG_MAC_ADDR_LOW(x)  VTSS_EXTRACT_BITFIELD(x,0,24)


/** 
 * \brief Debug control
 *
 * \details
 * Configures which events are counted in the ageing counter.
 *
 * Register: \a DSM:CFG:DBG_CTRL
 */
#define VTSS_DSM_CFG_DBG_CTRL                VTSS_IOREG(VTSS_TO_DSM,0x295)

/** 
 * \brief
 * Controls which event the AGE counter counts. This setting is common for
 * all aging counters.
 *
 * \details 
 * 0: Number of aged frames
 * 1: Number of SOF transmitted on taxi bus
 * 2: Number of EOF transmitted on taxi bus
 * 3: Number of ABORT transmitted on taxi bus
 * 4: Reserved
 * 5: Number of retransmits requests received from port status
 * 6: Reserved
 * 7: Reserved
 *
 * Field: ::VTSS_DSM_CFG_DBG_CTRL . DBG_EVENT_CTRL
 */
#define  VTSS_F_DSM_CFG_DBG_CTRL_DBG_EVENT_CTRL(x)  VTSS_ENCODE_BITFIELD(x,0,3)
#define  VTSS_M_DSM_CFG_DBG_CTRL_DBG_EVENT_CTRL     VTSS_ENCODE_BITMASK(0,3)
#define  VTSS_X_DSM_CFG_DBG_CTRL_DBG_EVENT_CTRL(x)  VTSS_EXTRACT_BITFIELD(x,0,3)

/**
 * Register Group: \a DSM:STATUS
 *
 * Status registers
 */


/** 
 * \brief Counts number of aged frames.
 *
 * \details
 * This counter holds the number of aged frames from this port.
 *
 * Register: \a DSM:STATUS:AGED_FRMS
 *
 * @param ri Replicator: x_DSM_NUM_AGED_FRM_CNTRS (??), 0-54
 */
#define VTSS_DSM_STATUS_AGED_FRMS(ri)        VTSS_IOREG(VTSS_TO_DSM,0x296 + (ri))


/** 
 * \brief Sticky bits for the DSM cell bus interface.
 *
 * \details
 * This register holds sticky bit information for the cell bus interface of
 * the DSM.
 *
 * Register: \a DSM:STATUS:CELL_BUS_STICKY
 */
#define VTSS_DSM_STATUS_CELL_BUS_STICKY      VTSS_IOREG(VTSS_TO_DSM,0x2cd)

/** 
 * \brief
 * This bit is set if cells are received after an EOF and before the
 * following SOF.
 *
 * \details 
 * '0': Missing SOF has not been detected.
 * '1': Missing SOF has been detected.
 * Bit is cleared by writing a '1' to this position.
 *
 * Field: ::VTSS_DSM_STATUS_CELL_BUS_STICKY . CELL_BUS_MISSING_SOF_STICKY
 */
#define  VTSS_F_DSM_STATUS_CELL_BUS_STICKY_CELL_BUS_MISSING_SOF_STICKY(x)  VTSS_ENCODE_BITFIELD(!!(x),1,1)
#define  VTSS_M_DSM_STATUS_CELL_BUS_STICKY_CELL_BUS_MISSING_SOF_STICKY  VTSS_BIT(1)
#define  VTSS_X_DSM_STATUS_CELL_BUS_STICKY_CELL_BUS_MISSING_SOF_STICKY(x)  VTSS_EXTRACT_BITFIELD(x,1,1)

/** 
 * \brief
 * This bit is set if data marked as SOF is received without the current
 * frame has been terminated with EOF. The current frame will be terminated
 * by setting the abort bit and the next frame will be discarded.
 *
 * \details 
 * '0': Missing EOF has not been detected.
 * '1': Missing EOF has been detected.
 * Bit is cleared by writing a '1' to this position.
 *
 * Field: ::VTSS_DSM_STATUS_CELL_BUS_STICKY . CELL_BUS_MISSING_EOF_STICKY
 */
#define  VTSS_F_DSM_STATUS_CELL_BUS_STICKY_CELL_BUS_MISSING_EOF_STICKY(x)  VTSS_ENCODE_BITFIELD(!!(x),0,1)
#define  VTSS_M_DSM_STATUS_CELL_BUS_STICKY_CELL_BUS_MISSING_EOF_STICKY  VTSS_BIT(0)
#define  VTSS_X_DSM_STATUS_CELL_BUS_STICKY_CELL_BUS_MISSING_EOF_STICKY(x)  VTSS_EXTRACT_BITFIELD(x,0,1)


/** 
 * \brief Buffer Overflow
 *
 * \details
 * Buffer Overflow
 *
 * Register: \a DSM:STATUS:BUF_OFLW_STICKY
 *
 * @param ri Replicator: x_DSM_PORTS_DIV32_CEIL (??), 0-1
 */
#define VTSS_DSM_STATUS_BUF_OFLW_STICKY(ri)  VTSS_IOREG(VTSS_TO_DSM,0x2ce + (ri))


/** 
 * \brief Buffer Underrun
 *
 * \details
 * Buffer Underrun
 *
 * Register: \a DSM:STATUS:BUF_UFLW_STICKY
 *
 * @param ri Replicator: x_DSM_PORTS_DIV32_CEIL (??), 0-1
 */
#define VTSS_DSM_STATUS_BUF_UFLW_STICKY(ri)  VTSS_IOREG(VTSS_TO_DSM,0x2d0 + (ri))


/** 
 * \brief Pause Frame pre-counter overflow sticky register
 *
 * \details
 * Pause Frame pre-counter overflow sticky register
 *
 * Register: \a DSM:STATUS:PRE_CNT_OFLW_STICKY
 */
#define VTSS_DSM_STATUS_PRE_CNT_OFLW_STICKY  VTSS_IOREG(VTSS_TO_DSM,0x2d2)

/** 
 * \brief
 * Will be set if one of the statistics pre counter have an overflow.
 *
 * \details 
 * '0': Pre counter overflow has not occurred
 * '1': Pre counter overflow has occurred
 * Bit is cleared by writing a '1' to this position.
 *
 * Field: ::VTSS_DSM_STATUS_PRE_CNT_OFLW_STICKY . PRE_CNT_OFLW_STICKY
 */
#define  VTSS_F_DSM_STATUS_PRE_CNT_OFLW_STICKY_PRE_CNT_OFLW_STICKY(x)  VTSS_ENCODE_BITFIELD(!!(x),8,1)
#define  VTSS_M_DSM_STATUS_PRE_CNT_OFLW_STICKY_PRE_CNT_OFLW_STICKY  VTSS_BIT(8)
#define  VTSS_X_DSM_STATUS_PRE_CNT_OFLW_STICKY_PRE_CNT_OFLW_STICKY(x)  VTSS_EXTRACT_BITFIELD(x,8,1)

/**
 * Register Group: \a DSM:RATE_LIMIT_CFG
 *
 * Configuration registers for rate limit modes
 */


/** 
 * \brief Configuration register for rate limit modes
 *
 * \details
 * Configuration register for rate limit modes
 *
 * Register: \a DSM:RATE_LIMIT_CFG:TX_RATE_LIMIT_MODE
 *
 * @param ri Replicator: x_DSM_PORTS (??), 0-54
 */
#define VTSS_DSM_RATE_LIMIT_CFG_TX_RATE_LIMIT_MODE(ri)  VTSS_IOREG(VTSS_TO_DSM,0x2d3 + (ri))

/** 
 * \brief
 * Scale the IPG calculated by txRateLimitFrameOverhead and/or
 * txRateLimitPayloadRate by a power of 2 to enable shaping down to lower
 * BW.
 * 
 * Note: For txRateLimitPayloadRate only the additional overhead excluding
 * the standard 12 byte IPG is scaled whereas for txRateLimitFrameOverhead
 * the complete value is scaled.
 *
 * \details 
 * 0: Don't scale
 * 1: Multiply by 2
 * 2: Multiply by 4
 * (...)
 * 10: Multiply by 1024
 * 11-15: Reserved
 *
 * Field: ::VTSS_DSM_RATE_LIMIT_CFG_TX_RATE_LIMIT_MODE . IPG_SCALE_VAL
 */
#define  VTSS_F_DSM_RATE_LIMIT_CFG_TX_RATE_LIMIT_MODE_IPG_SCALE_VAL(x)  VTSS_ENCODE_BITFIELD(x,16,4)
#define  VTSS_M_DSM_RATE_LIMIT_CFG_TX_RATE_LIMIT_MODE_IPG_SCALE_VAL     VTSS_ENCODE_BITMASK(16,4)
#define  VTSS_X_DSM_RATE_LIMIT_CFG_TX_RATE_LIMIT_MODE_IPG_SCALE_VAL(x)  VTSS_EXTRACT_BITFIELD(x,16,4)

/** 
 * \brief
 * Defines if what is configured as header size in
 * TX_RATE_LIMIT_HDR_CFG.TX_RATE_LIMIT_HDR_SIZE is subtracted form the
 * payload in txRateLimitPayloadRate and txRateLimitFrameRate mode.
 *
 * \details 
 * 0: Nothing is subtracted from payload
 * 1: TX_RATE_LIMIT_HDR_CFG.TX_RATE_LIMIT_HDR_SIZE is subtracted from
 * payload
 *
 * Field: ::VTSS_DSM_RATE_LIMIT_CFG_TX_RATE_LIMIT_MODE . PAYLOAD_CFG
 */
#define  VTSS_F_DSM_RATE_LIMIT_CFG_TX_RATE_LIMIT_MODE_PAYLOAD_CFG(x)  VTSS_ENCODE_BITFIELD(!!(x),9,1)
#define  VTSS_M_DSM_RATE_LIMIT_CFG_TX_RATE_LIMIT_MODE_PAYLOAD_CFG  VTSS_BIT(9)
#define  VTSS_X_DSM_RATE_LIMIT_CFG_TX_RATE_LIMIT_MODE_PAYLOAD_CFG(x)  VTSS_EXTRACT_BITFIELD(x,9,1)

/** 
 * \brief
 * This bitgroup defines whether the preamble is counted as payload in
 * txRateLimitPayloadRate and txRateLimitFrameRate mode.
 *
 * \details 
 * '0': Preamble is not counted as payload
 * '1': Preamble is counted as payload
 *
 * Field: ::VTSS_DSM_RATE_LIMIT_CFG_TX_RATE_LIMIT_MODE . PAYLOAD_PREAM_CFG
 */
#define  VTSS_F_DSM_RATE_LIMIT_CFG_TX_RATE_LIMIT_MODE_PAYLOAD_PREAM_CFG(x)  VTSS_ENCODE_BITFIELD(!!(x),8,1)
#define  VTSS_M_DSM_RATE_LIMIT_CFG_TX_RATE_LIMIT_MODE_PAYLOAD_PREAM_CFG  VTSS_BIT(8)
#define  VTSS_X_DSM_RATE_LIMIT_CFG_TX_RATE_LIMIT_MODE_PAYLOAD_PREAM_CFG(x)  VTSS_EXTRACT_BITFIELD(x,8,1)

/** 
 * \brief
 * Enable for accumulated rate limit mode.
 * Instead of taking the maximum  IPG of txRateLimitFrameOverhead,
 * txRateLimitPayloadRate and txRateLimitFrameRate. The resulting IPG is
 * the sum of txRateLimitFrameOverhead and txRateLimitPayloadRate.
 * txRateLimitFrameRate must be disabled.
 *
 * \details 
 * '0': Accumulated rate limit mode is disabled
 * '1': Accumulated rate limit mode is enabled
 *
 * Field: ::VTSS_DSM_RATE_LIMIT_CFG_TX_RATE_LIMIT_MODE . TX_RATE_LIMIT_ACCUM_MODE_ENA
 */
#define  VTSS_F_DSM_RATE_LIMIT_CFG_TX_RATE_LIMIT_MODE_TX_RATE_LIMIT_ACCUM_MODE_ENA(x)  VTSS_ENCODE_BITFIELD(!!(x),4,1)
#define  VTSS_M_DSM_RATE_LIMIT_CFG_TX_RATE_LIMIT_MODE_TX_RATE_LIMIT_ACCUM_MODE_ENA  VTSS_BIT(4)
#define  VTSS_X_DSM_RATE_LIMIT_CFG_TX_RATE_LIMIT_MODE_TX_RATE_LIMIT_ACCUM_MODE_ENA(x)  VTSS_EXTRACT_BITFIELD(x,4,1)

/** 
 * \brief
 * Enable for IPG reduction to allow adaption to clock frequency offsets.
 * When this mode is enabled, all other TX_RATE_LIMIT modes must be
 * disabled.
 *
 * \details 
 * '0': tx IPG reduction test feature disabled
 * '1': tx IPG reduction test feature enabled
 *
 * Field: ::VTSS_DSM_RATE_LIMIT_CFG_TX_RATE_LIMIT_MODE . TX_RATE_IPG_PPM_ADAPT_ENA
 */
#define  VTSS_F_DSM_RATE_LIMIT_CFG_TX_RATE_LIMIT_MODE_TX_RATE_IPG_PPM_ADAPT_ENA(x)  VTSS_ENCODE_BITFIELD(!!(x),3,1)
#define  VTSS_M_DSM_RATE_LIMIT_CFG_TX_RATE_LIMIT_MODE_TX_RATE_IPG_PPM_ADAPT_ENA  VTSS_BIT(3)
#define  VTSS_X_DSM_RATE_LIMIT_CFG_TX_RATE_LIMIT_MODE_TX_RATE_IPG_PPM_ADAPT_ENA(x)  VTSS_EXTRACT_BITFIELD(x,3,1)

/** 
 * \brief
 * Enable txRateLimitFrameRate mode.
 * IPG is extended so that frame_size + IPG result in the value configured
 * in TX_FRAME_RATE_START_CFG.TX_FRAME_RATE_START
 *
 * \details 
 * '0': txRateLimitFrameRate disabled
 * '1': txRateLimitFrameRate enabled
 *
 * Field: ::VTSS_DSM_RATE_LIMIT_CFG_TX_RATE_LIMIT_MODE . TX_RATE_LIMIT_FRAME_RATE_ENA
 */
#define  VTSS_F_DSM_RATE_LIMIT_CFG_TX_RATE_LIMIT_MODE_TX_RATE_LIMIT_FRAME_RATE_ENA(x)  VTSS_ENCODE_BITFIELD(!!(x),2,1)
#define  VTSS_M_DSM_RATE_LIMIT_CFG_TX_RATE_LIMIT_MODE_TX_RATE_LIMIT_FRAME_RATE_ENA  VTSS_BIT(2)
#define  VTSS_X_DSM_RATE_LIMIT_CFG_TX_RATE_LIMIT_MODE_TX_RATE_LIMIT_FRAME_RATE_ENA(x)  VTSS_EXTRACT_BITFIELD(x,2,1)

/** 
 * \brief
 * Enable txRateLimitPayloadRate mode.
 * IPG is increased relative to payload size.
 *
 * \details 
 * '0': txRateLimitPayloadRate disabled
 * '1': txRateLimitPayloadRate enable
 *
 * Field: ::VTSS_DSM_RATE_LIMIT_CFG_TX_RATE_LIMIT_MODE . TX_RATE_LIMIT_PAYLOAD_RATE_ENA
 */
#define  VTSS_F_DSM_RATE_LIMIT_CFG_TX_RATE_LIMIT_MODE_TX_RATE_LIMIT_PAYLOAD_RATE_ENA(x)  VTSS_ENCODE_BITFIELD(!!(x),1,1)
#define  VTSS_M_DSM_RATE_LIMIT_CFG_TX_RATE_LIMIT_MODE_TX_RATE_LIMIT_PAYLOAD_RATE_ENA  VTSS_BIT(1)
#define  VTSS_X_DSM_RATE_LIMIT_CFG_TX_RATE_LIMIT_MODE_TX_RATE_LIMIT_PAYLOAD_RATE_ENA(x)  VTSS_EXTRACT_BITFIELD(x,1,1)

/** 
 * \brief
 * Enable txRateLimitFrameOverhead mode.
 * IPG is increased by a fixed value specified in 
 * RATE_CTRL.FRM_GAP_COMP
 *
 * \details 
 * '0': txRateLimitFrameOverhead disabled
 * '1': txRateLimitFrameOverhead enabled
 *
 * Field: ::VTSS_DSM_RATE_LIMIT_CFG_TX_RATE_LIMIT_MODE . TX_RATE_LIMIT_FRAME_OVERHEAD_ENA
 */
#define  VTSS_F_DSM_RATE_LIMIT_CFG_TX_RATE_LIMIT_MODE_TX_RATE_LIMIT_FRAME_OVERHEAD_ENA(x)  VTSS_ENCODE_BITFIELD(!!(x),0,1)
#define  VTSS_M_DSM_RATE_LIMIT_CFG_TX_RATE_LIMIT_MODE_TX_RATE_LIMIT_FRAME_OVERHEAD_ENA  VTSS_BIT(0)
#define  VTSS_X_DSM_RATE_LIMIT_CFG_TX_RATE_LIMIT_MODE_TX_RATE_LIMIT_FRAME_OVERHEAD_ENA(x)  VTSS_EXTRACT_BITFIELD(x,0,1)


/** 
 * \brief Configuration register for txFineIpgStretchRatio
 *
 * \details
 * Configuration register for txFineIpgStretchRatio
 *
 * Register: \a DSM:RATE_LIMIT_CFG:TX_IPG_STRETCH_RATIO_CFG
 *
 * @param ri Replicator: x_DSM_PORTS (??), 0-54
 */
#define VTSS_DSM_RATE_LIMIT_CFG_TX_IPG_STRETCH_RATIO_CFG(ri)  VTSS_IOREG(VTSS_TO_DSM,0x30a + (ri))

/** 
 * \brief
 * Determines the extent of the interPacketGap extension, when
 * txRateLimitPayloadRate mode is enabled. It may be considered as the
 * number of bits in a packet that would require 32 octets of
 * interPacketGap extension. Note that each interPacketGap is increased by
 * an integer number of octets, the average increase is ( (frame length in
 * bits) * 32) / txFineIpgStretchRatio.
 * 
 * Note: fractions left at frame border will be carried on to the next
 * frame.
 * 
 * Example:
 * Packet length = 64 bytes + 8 bytes preamble 
 * value = 2048
 * IPG increase = 9 bytes
 * 
 * Maximum IPG increase is 255 bytes.
 *
 * \details 
 * IPG increase = frame length in bits * 32 / value.
 * 
 * values below 2048 and above 518143 are reserved for arithmetic
 * processing  reasons.
 *
 * Field: ::VTSS_DSM_RATE_LIMIT_CFG_TX_IPG_STRETCH_RATIO_CFG . TX_FINE_IPG_STRETCH_RATIO
 */
#define  VTSS_F_DSM_RATE_LIMIT_CFG_TX_IPG_STRETCH_RATIO_CFG_TX_FINE_IPG_STRETCH_RATIO(x)  VTSS_ENCODE_BITFIELD(x,0,19)
#define  VTSS_M_DSM_RATE_LIMIT_CFG_TX_IPG_STRETCH_RATIO_CFG_TX_FINE_IPG_STRETCH_RATIO     VTSS_ENCODE_BITMASK(0,19)
#define  VTSS_X_DSM_RATE_LIMIT_CFG_TX_IPG_STRETCH_RATIO_CFG_TX_FINE_IPG_STRETCH_RATIO(x)  VTSS_EXTRACT_BITFIELD(x,0,19)


/** 
 * \brief Configuration register for txFrameRateStart
 *
 * \details
 * Configuration register for txFrameRateStart
 *
 * Register: \a DSM:RATE_LIMIT_CFG:TX_FRAME_RATE_START_CFG
 *
 * @param ri Replicator: x_DSM_PORTS (??), 0-54
 */
#define VTSS_DSM_RATE_LIMIT_CFG_TX_FRAME_RATE_START_CFG(ri)  VTSS_IOREG(VTSS_TO_DSM,0x341 + (ri))

/** 
 * \brief
 * Value loaded into the txFrameRateTimer at the start of each packet, when
 * TX_RATE_LIMIT_FRAME_RATE mode is enabled.
 * Timer is decremented with each byte transmitted.
 * IPG is set to timer value at EOF.
 * Note: If resulting IPG is smaller than the minimum IPG of 12 bytes, it
 * will be set to 12 bytes.
 *
 * \details 
 * Timer start value in bytes.
 * To take effect, value must be above 84. (64byte min frame size, 8 byte
 * preamble, 12 byte minimum IFG).
 *
 * Field: ::VTSS_DSM_RATE_LIMIT_CFG_TX_FRAME_RATE_START_CFG . TX_FRAME_RATE_START
 */
#define  VTSS_F_DSM_RATE_LIMIT_CFG_TX_FRAME_RATE_START_CFG_TX_FRAME_RATE_START(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_DSM_RATE_LIMIT_CFG_TX_FRAME_RATE_START_CFG_TX_FRAME_RATE_START     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_DSM_RATE_LIMIT_CFG_TX_FRAME_RATE_START_CFG_TX_FRAME_RATE_START(x)  VTSS_EXTRACT_BITFIELD(x,0,16)


/** 
 * \brief Header size configuration register for txRateLimitPayloadRate mode
 *
 * \details
 * Header size configuration register for txRateLimitPayloadRate mode
 *
 * Register: \a DSM:RATE_LIMIT_CFG:TX_RATE_LIMIT_HDR_CFG
 */
#define VTSS_DSM_RATE_LIMIT_CFG_TX_RATE_LIMIT_HDR_CFG  VTSS_IOREG(VTSS_TO_DSM,0x378)

/** 
 * \brief
 * Defines how much of the frame is seen as header and not counted as
 * payload in txRateLimitPayloadRate and txRateLimitFrameRate mode when
 * TX_RATE_LIMIT_MODE.PAYLOAD_CFG is set to 1.
 *
 * \details 
 * 0: 32 bytes are counted as header
 * 1-31: 1-31 bytes are counted as header
 *
 * Field: ::VTSS_DSM_RATE_LIMIT_CFG_TX_RATE_LIMIT_HDR_CFG . TX_RATE_LIMIT_HDR_SIZE
 */
#define  VTSS_F_DSM_RATE_LIMIT_CFG_TX_RATE_LIMIT_HDR_CFG_TX_RATE_LIMIT_HDR_SIZE(x)  VTSS_ENCODE_BITFIELD(x,0,5)
#define  VTSS_M_DSM_RATE_LIMIT_CFG_TX_RATE_LIMIT_HDR_CFG_TX_RATE_LIMIT_HDR_SIZE     VTSS_ENCODE_BITMASK(0,5)
#define  VTSS_X_DSM_RATE_LIMIT_CFG_TX_RATE_LIMIT_HDR_CFG_TX_RATE_LIMIT_HDR_SIZE(x)  VTSS_EXTRACT_BITFIELD(x,0,5)

/**
 * Register Group: \a DSM:RATE_LIMIT_STATUS
 *
 * Status registers for rate limit modes
 */


/** 
 * \brief Sticky bit register for rate limit modes
 *
 * \details
 * Sticky bit register for rate limit modes
 *
 * Register: \a DSM:RATE_LIMIT_STATUS:TX_RATE_LIMIT_STICKY
 *
 * @param ri Replicator: x_DSM_PORTS_DIV32_CEIL (??), 0-1
 */
#define VTSS_DSM_RATE_LIMIT_STATUS_TX_RATE_LIMIT_STICKY(ri)  VTSS_IOREG(VTSS_TO_DSM,0x379 + (ri))


#endif /* _VTSS_JAGUAR2_REGS_DSM_H_ */
