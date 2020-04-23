#ifndef _VTSS_SEVILLE_REGS_SYS_H_
#define _VTSS_SEVILLE_REGS_SYS_H_

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

#include "vtss_seville_regs_common.h"

/*********************************************************************** 
 *
 * Target: \a SYS
 *
 * Switching Engine Configuration
 *
 ***********************************************************************/

/**
 * Register Group: \a SYS:SYSTEM
 *
 * Switch configuration
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
 * Register: \a SYS:SYSTEM:RESET_CFG
 */
#define VTSS_SYS_SYSTEM_RESET_CFG            VTSS_IOREG(VTSS_TO_SYS,0xc6)

/** 
 * \brief
 * Switch core is enabled when this field is set.
 *
 * \details 
 * Field: ::VTSS_SYS_SYSTEM_RESET_CFG . CORE_ENA
 */
#define  VTSS_F_SYS_SYSTEM_RESET_CFG_CORE_ENA  VTSS_BIT(7)

/** 
 * \brief
 * Core memory controllers are enabled when this field is set.
 *
 * \details 
 * Field: ::VTSS_SYS_SYSTEM_RESET_CFG . MEM_ENA
 */
#define  VTSS_F_SYS_SYSTEM_RESET_CFG_MEM_ENA  VTSS_BIT(6)

/** 
 * \brief
 * Initialize core memories. Field is automatically cleared when operation
 * is complete ( approx. 40 us).
 *
 * \details 
 * Field: ::VTSS_SYS_SYSTEM_RESET_CFG . MEM_INIT
 */
#define  VTSS_F_SYS_SYSTEM_RESET_CFG_MEM_INIT  VTSS_BIT(5)


/** 
 * \brief S-tag Ethernet Type
 *
 * \details
 * Register: \a SYS:SYSTEM:VLAN_ETYPE_CFG
 */
#define VTSS_SYS_SYSTEM_VLAN_ETYPE_CFG       VTSS_IOREG(VTSS_TO_SYS,0xc8)

/** 
 * \brief
 * Custom Ethernet Type for S-tags. Tags with TPID = 0x88A8 are always
 * recognized as S-tags.
 *
 * \details 
 * Field: ::VTSS_SYS_SYSTEM_VLAN_ETYPE_CFG . VLAN_S_TAG_ETYPE_VAL
 */
#define  VTSS_F_SYS_SYSTEM_VLAN_ETYPE_CFG_VLAN_S_TAG_ETYPE_VAL(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_SYS_SYSTEM_VLAN_ETYPE_CFG_VLAN_S_TAG_ETYPE_VAL     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_SYS_SYSTEM_VLAN_ETYPE_CFG_VLAN_S_TAG_ETYPE_VAL(x)  VTSS_EXTRACT_BITFIELD(x,0,16)


/** 
 * \brief Per device port configuration
 *
 * \details
 * These configurations exists per frontport and for each of the two CPU
 * ports (11+12).
 *
 * Register: \a SYS:SYSTEM:PORT_MODE
 *
 * @param ri Replicator: x_DPORT_CNT (??), 0-11
 */
#define VTSS_SYS_SYSTEM_PORT_MODE(ri)        VTSS_IOREG(VTSS_TO_SYS,0xc9 + (ri))

/** 
 * \brief
 * Set the mode for the formatting of incoming frames. If INCL_INJ_HDR>0,
 * incoming frames are expected to contain the selected prefix followed by
 * the 16-byte CPU injection header as the first part of the frame. Frames
 * are forwarded based on the contents in the CPU injection header instead
 * of normal forwarding.
 * 
 * Three different prefixes are supported:
 * - No prefix.
 * - Short prefix: 0x88800005.
 * - Long prefix: any DMAC, any SMAC, EtherType=0x8880, payload=0x0005.
 * 
 * In modes 2 and 3, if the incoming frame's format does not comply with
 * the prefix, then sticky bit INJ_HDR_PREFIX_ERR is set. A non-complying
 * frame is discarded. 

 *
 * \details 
 * 0: No CPU injection header (normal frames)
 * 1: CPU injection header without prefix
 * 2: CPU injection header with short prefix
 * 3: CPU injection header with long prefix

 *
 * Field: ::VTSS_SYS_SYSTEM_PORT_MODE . INCL_INJ_HDR
 */
#define  VTSS_F_SYS_SYSTEM_PORT_MODE_INCL_INJ_HDR(x)  VTSS_ENCODE_BITFIELD(x,4,2)
#define  VTSS_M_SYS_SYSTEM_PORT_MODE_INCL_INJ_HDR     VTSS_ENCODE_BITMASK(4,2)
#define  VTSS_X_SYS_SYSTEM_PORT_MODE_INCL_INJ_HDR(x)  VTSS_EXTRACT_BITFIELD(x,4,2)

/** 
 * \brief
 * Set the mode for the formatting of outgoing frames. If INCL_XTR_HDR>0,
 * outgoing frames are prepended the selected prefix followed by the
 * 16-byte CPU extraction header as the first part of the frame. Inserting
 * the CPU extraction header prevents other rewriter operations on the same
 * frame.
 * 
 * Three different prefixes are supported:
 * - No prefix.
 * - Short prefix: 0x88800005.
 * - Long prefix: DMAC=0xFFFFFFFFFFFF, SMAC=0xFEFFFFFFFFFF,
 * EtherType=0x8880, payload=0x0005.
 *
 * \details 
 * 0: No CPU extraction header (normal frames)
 * 1: CPU extraction header without prefix
 * 2: CPU extraction header with short prefix
 * 3: CPU extraction header with long prefix

 *
 * Field: ::VTSS_SYS_SYSTEM_PORT_MODE . INCL_XTR_HDR
 */
#define  VTSS_F_SYS_SYSTEM_PORT_MODE_INCL_XTR_HDR(x)  VTSS_ENCODE_BITFIELD(x,2,2)
#define  VTSS_M_SYS_SYSTEM_PORT_MODE_INCL_XTR_HDR     VTSS_ENCODE_BITMASK(2,2)
#define  VTSS_X_SYS_SYSTEM_PORT_MODE_INCL_XTR_HDR(x)  VTSS_EXTRACT_BITFIELD(x,2,2)

/** 
 * \brief
 * If set, a frame has been received with prefix not complying with the
 * setting in INCL_INJ_HDR.
 *
 * \details 
 * Field: ::VTSS_SYS_SYSTEM_PORT_MODE . INJ_HDR_ERR
 */
#define  VTSS_F_SYS_SYSTEM_PORT_MODE_INJ_HDR_ERR  VTSS_BIT(0)


/** 
 * \brief Various Ethernet port configurations per front port
 *
 * \details
 * Register: \a SYS:SYSTEM:FRONT_PORT_MODE
 *
 * @param ri Replicator: x_FPORT_CNT (??), 0-9
 */
#define VTSS_SYS_SYSTEM_FRONT_PORT_MODE(ri)  VTSS_IOREG(VTSS_TO_SYS,0xd5 + (ri))

/** 
 * \brief
 * Enables the queue system to support the half duplex mode. Must be set
 * for a port when enabled for half-duplex mode (MAC_MODE_ENA.FDX_ENA
 * cleared).
 *
 * \details 
 * Field: ::VTSS_SYS_SYSTEM_FRONT_PORT_MODE . HDX_MODE
 */
#define  VTSS_F_SYS_SYSTEM_FRONT_PORT_MODE_HDX_MODE  VTSS_BIT(0)


/** 
 * \brief Frame aging configuration
 *
 * \details
 * Register: \a SYS:SYSTEM:FRM_AGING
 */
#define VTSS_SYS_SYSTEM_FRM_AGING            VTSS_IOREG(VTSS_TO_SYS,0xdf)

/** 
 * \brief
 * If set, frames exceeding the frame aging time while waiting head-of-line
 * in a port due to a link partner signaling pause are allowed to be aged.
 * Note that frames aged in this case are not counted by the normal frame
 * aging counter C_TX_AGED.
 *
 * \details 
 * Field: ::VTSS_SYS_SYSTEM_FRM_AGING . AGE_TX_ENA
 */
#define  VTSS_F_SYS_SYSTEM_FRM_AGING_AGE_TX_ENA  VTSS_BIT(20)

/** 
 * \brief
 * Frames are aged and removed from the queue system when the frame's age
 * timer becomes two. The frame age timer is increased for all frames
 * whenever the configured time, MAX_AGE,  has passed. The unit is 6.5us.
 * Effectively, this means that a frame is aged when the frame has waited
 * in the queue system between one or two times the period specified by
 * MAX_AGE.
 * 
 * A value of zero disables frame aging. 

 *
 * \details 
 * Field: ::VTSS_SYS_SYSTEM_FRM_AGING . MAX_AGE
 */
#define  VTSS_F_SYS_SYSTEM_FRM_AGING_MAX_AGE(x)  VTSS_ENCODE_BITFIELD(x,0,20)
#define  VTSS_M_SYS_SYSTEM_FRM_AGING_MAX_AGE     VTSS_ENCODE_BITMASK(0,20)
#define  VTSS_X_SYS_SYSTEM_FRM_AGING_MAX_AGE(x)  VTSS_EXTRACT_BITFIELD(x,0,20)


/** 
 * \brief Statistics configuration
 *
 * \details
 * Register: \a SYS:SYSTEM:STAT_CFG
 */
#define VTSS_SYS_SYSTEM_STAT_CFG             VTSS_IOREG(VTSS_TO_SYS,0xe0)

/** 
 * \brief
 * Set STAT_CLEAR_SHOT to clear counters in the counter group for the port
 * selected by STAT_VIEW. Auto-cleared when complete (1us). Multiple
 * counter groups can be cleared at the same time by setting multiple bits
 * in STAT_CLEAR_SHOT.
 *
 * \details 
 * Bit 0: Group 0 - Rx counters.
 * Bit 1: Group 1 - Tx counters.
 * Bit 2: Group 2 - Drop counters.
 *
 * Field: ::VTSS_SYS_SYSTEM_STAT_CFG . STAT_CLEAR_SHOT
 */
#define  VTSS_F_SYS_SYSTEM_STAT_CFG_STAT_CLEAR_SHOT(x)  VTSS_ENCODE_BITFIELD(x,10,3)
#define  VTSS_M_SYS_SYSTEM_STAT_CFG_STAT_CLEAR_SHOT     VTSS_ENCODE_BITMASK(10,3)
#define  VTSS_X_SYS_SYSTEM_STAT_CFG_STAT_CLEAR_SHOT(x)  VTSS_EXTRACT_BITFIELD(x,10,3)

/** 
 * \brief
 * Selects the port, which counters are readable through the SYS:STAT:CNT
 * register or can be cleared through STAT_CLEAR_SHOT.
 *
 * \details 
 * Field: ::VTSS_SYS_SYSTEM_STAT_CFG . STAT_VIEW
 */
#define  VTSS_F_SYS_SYSTEM_STAT_CFG_STAT_VIEW(x)  VTSS_ENCODE_BITFIELD(x,0,4)
#define  VTSS_M_SYS_SYSTEM_STAT_CFG_STAT_VIEW     VTSS_ENCODE_BITMASK(0,4)
#define  VTSS_X_SYS_SYSTEM_STAT_CFG_STAT_VIEW(x)  VTSS_EXTRACT_BITFIELD(x,0,4)

/**
 * Register Group: \a SYS:COREMEM
 *
 * Access core memory
 */


/** 
 * \brief Address selection
 *
 * \details
 * Register: \a SYS:COREMEM:CM_ADDR
 */
#define VTSS_SYS_COREMEM_CM_ADDR             VTSS_IOREG(VTSS_TO_SYS,0xc4)

/** 
 * \brief
 * Address selection within selected core memory (CMID register). Address
 * is automatically advanced at every data access.
 *
 * \details 
 * Field: ::VTSS_SYS_COREMEM_CM_ADDR . CM_ADDR
 */
#define  VTSS_F_SYS_COREMEM_CM_ADDR_CM_ADDR(x)  VTSS_ENCODE_BITFIELD(x,0,22)
#define  VTSS_M_SYS_COREMEM_CM_ADDR_CM_ADDR     VTSS_ENCODE_BITMASK(0,22)
#define  VTSS_X_SYS_COREMEM_CM_ADDR_CM_ADDR(x)  VTSS_EXTRACT_BITFIELD(x,0,22)


/** 
 * \brief Data register for core memory access.
 *
 * \details
 * Register: \a SYS:COREMEM:CM_DATA
 */
#define VTSS_SYS_COREMEM_CM_DATA             VTSS_IOREG(VTSS_TO_SYS,0xc5)

/**
 * Register Group: \a SYS:PAUSE_CFG
 *
 * Watermarks for egress queue system
 */


/** 
 * \brief Watermarks for flow control condition per switch port.
 *
 * \details
 * Register: \a SYS:PAUSE_CFG:PAUSE_CFG
 *
 * @param ri Replicator: x_SPORT_CNT (??), 0-10
 */
#define VTSS_SYS_PAUSE_CFG_PAUSE_CFG(ri)     VTSS_IOREG(VTSS_TO_SYS,0x113 + (ri))

/** 
 * \brief
 * Start pausing ingress stream when the amount of memory consumed by the
 * port exceeds this watermark. The TOTPAUSE condition must also be met.
 *
 * \details 
 * See RES_CFG
 *
 * Field: ::VTSS_SYS_PAUSE_CFG_PAUSE_CFG . PAUSE_START
 */
#define  VTSS_F_SYS_PAUSE_CFG_PAUSE_CFG_PAUSE_START(x)  VTSS_ENCODE_BITFIELD(x,11,10)
#define  VTSS_M_SYS_PAUSE_CFG_PAUSE_CFG_PAUSE_START     VTSS_ENCODE_BITMASK(11,10)
#define  VTSS_X_SYS_PAUSE_CFG_PAUSE_CFG_PAUSE_START(x)  VTSS_EXTRACT_BITFIELD(x,11,10)

/** 
 * \brief
 * Stop pausing ingress stream when the amount of memory consumed by the
 * port is below  this watermark.
 *
 * \details 
 * See RES_CFG.
 *
 * Field: ::VTSS_SYS_PAUSE_CFG_PAUSE_CFG . PAUSE_STOP
 */
#define  VTSS_F_SYS_PAUSE_CFG_PAUSE_CFG_PAUSE_STOP(x)  VTSS_ENCODE_BITFIELD(x,1,10)
#define  VTSS_M_SYS_PAUSE_CFG_PAUSE_CFG_PAUSE_STOP     VTSS_ENCODE_BITMASK(1,10)
#define  VTSS_X_SYS_PAUSE_CFG_PAUSE_CFG_PAUSE_STOP(x)  VTSS_EXTRACT_BITFIELD(x,1,10)

/** 
 * \brief
 * Enable pause feedback to the MAC, allowing transmission of pause frames
 * or HDX collisions to limit ingress data rate.
 *
 * \details 
 * Field: ::VTSS_SYS_PAUSE_CFG_PAUSE_CFG . PAUSE_ENA
 */
#define  VTSS_F_SYS_PAUSE_CFG_PAUSE_CFG_PAUSE_ENA  VTSS_BIT(0)


/** 
 * \brief Configure total memory pause condition
 *
 * \details
 * Register: \a SYS:PAUSE_CFG:PAUSE_TOT_CFG
 */
#define VTSS_SYS_PAUSE_CFG_PAUSE_TOT_CFG     VTSS_IOREG(VTSS_TO_SYS,0x11e)

/** 
 * \brief
 * Assert TOTPAUSE condition when total memory allocation is above this
 * watermark.
 *
 * \details 
 * See RES_CFG
 *
 * Field: ::VTSS_SYS_PAUSE_CFG_PAUSE_TOT_CFG . PAUSE_TOT_START
 */
#define  VTSS_F_SYS_PAUSE_CFG_PAUSE_TOT_CFG_PAUSE_TOT_START(x)  VTSS_ENCODE_BITFIELD(x,10,10)
#define  VTSS_M_SYS_PAUSE_CFG_PAUSE_TOT_CFG_PAUSE_TOT_START     VTSS_ENCODE_BITMASK(10,10)
#define  VTSS_X_SYS_PAUSE_CFG_PAUSE_TOT_CFG_PAUSE_TOT_START(x)  VTSS_EXTRACT_BITFIELD(x,10,10)

/** 
 * \brief
 * Deassert TOTPAUSE condition when total memory allocation is below this
 * watermark.
 *
 * \details 
 * See RES_CFG
 *
 * Field: ::VTSS_SYS_PAUSE_CFG_PAUSE_TOT_CFG . PAUSE_TOT_STOP
 */
#define  VTSS_F_SYS_PAUSE_CFG_PAUSE_TOT_CFG_PAUSE_TOT_STOP(x)  VTSS_ENCODE_BITFIELD(x,0,10)
#define  VTSS_M_SYS_PAUSE_CFG_PAUSE_TOT_CFG_PAUSE_TOT_STOP     VTSS_ENCODE_BITMASK(0,10)
#define  VTSS_X_SYS_PAUSE_CFG_PAUSE_TOT_CFG_PAUSE_TOT_STOP(x)  VTSS_EXTRACT_BITFIELD(x,0,10)


/** 
 * \brief Tail dropping level
 *
 * \details
 * Register: \a SYS:PAUSE_CFG:ATOP
 *
 * @param ri Replicator: x_SPORT_CNT (??), 0-10
 */
#define VTSS_SYS_PAUSE_CFG_ATOP(ri)          VTSS_IOREG(VTSS_TO_SYS,0x11f + (ri))

/** 
 * \brief
 * When a source port consumes more than this level in the packet memory,
 * frames are tail dropped, unconditionally of destination. 
 *
 * \details 
 * See RES_CFG
 *
 * Field: ::VTSS_SYS_PAUSE_CFG_ATOP . ATOP
 */
#define  VTSS_F_SYS_PAUSE_CFG_ATOP_ATOP(x)    VTSS_ENCODE_BITFIELD(x,0,10)
#define  VTSS_M_SYS_PAUSE_CFG_ATOP_ATOP       VTSS_ENCODE_BITMASK(0,10)
#define  VTSS_X_SYS_PAUSE_CFG_ATOP_ATOP(x)    VTSS_EXTRACT_BITFIELD(x,0,10)


/** 
 * \brief Total raw memory use before tail dropping is activated
 *
 * \details
 * Register: \a SYS:PAUSE_CFG:ATOP_TOT_CFG
 */
#define VTSS_SYS_PAUSE_CFG_ATOP_TOT_CFG      VTSS_IOREG(VTSS_TO_SYS,0x12a)

/** 
 * \brief
 * Tail dropping is activate on a port when the port use has exceeded the
 * ATOP watermark for the port, and the total memory use has exceeded this
 * watermark.
 *
 * \details 
 * See RES_CFG
 *
 * Field: ::VTSS_SYS_PAUSE_CFG_ATOP_TOT_CFG . ATOP_TOT
 */
#define  VTSS_F_SYS_PAUSE_CFG_ATOP_TOT_CFG_ATOP_TOT(x)  VTSS_ENCODE_BITFIELD(x,0,10)
#define  VTSS_M_SYS_PAUSE_CFG_ATOP_TOT_CFG_ATOP_TOT     VTSS_ENCODE_BITMASK(0,10)
#define  VTSS_X_SYS_PAUSE_CFG_ATOP_TOT_CFG_ATOP_TOT(x)  VTSS_EXTRACT_BITFIELD(x,0,10)


/** 
 * \brief MAC Flow Control Configuration Register
 *
 * \details
 * Register: \a SYS:PAUSE_CFG:MAC_FC_CFG
 *
 * @param ri Replicator: x_FPORT_CNT (??), 0-9
 */
#define VTSS_SYS_PAUSE_CFG_MAC_FC_CFG(ri)    VTSS_IOREG(VTSS_TO_SYS,0x12b + (ri))

/** 
 * \brief
 * Configures the link speed. This is used to evaluate the time
 * specification in incoming pause frames.
 *
 * \details 
 * 0: 2500 Mbps
 * 1: 1000 Mbps
 * 2: 100 Mbps
 * 3: 10 Mbps
 *
 * Field: ::VTSS_SYS_PAUSE_CFG_MAC_FC_CFG . FC_LINK_SPEED
 */
#define  VTSS_F_SYS_PAUSE_CFG_MAC_FC_CFG_FC_LINK_SPEED(x)  VTSS_ENCODE_BITFIELD(x,26,2)
#define  VTSS_M_SYS_PAUSE_CFG_MAC_FC_CFG_FC_LINK_SPEED     VTSS_ENCODE_BITMASK(26,2)
#define  VTSS_X_SYS_PAUSE_CFG_MAC_FC_CFG_FC_LINK_SPEED(x)  VTSS_EXTRACT_BITFIELD(x,26,2)

/** 
 * \brief
 * Accepted reaction time for link partner after the port has transmitted a
 * pause frame. Frames starting after this latency are aborted. Unit is 64
 * byte times.
 *
 * \details 
 * Field: ::VTSS_SYS_PAUSE_CFG_MAC_FC_CFG . FC_LATENCY_CFG
 */
#define  VTSS_F_SYS_PAUSE_CFG_MAC_FC_CFG_FC_LATENCY_CFG(x)  VTSS_ENCODE_BITFIELD(x,20,6)
#define  VTSS_M_SYS_PAUSE_CFG_MAC_FC_CFG_FC_LATENCY_CFG     VTSS_ENCODE_BITMASK(20,6)
#define  VTSS_X_SYS_PAUSE_CFG_MAC_FC_CFG_FC_LATENCY_CFG(x)  VTSS_EXTRACT_BITFIELD(x,20,6)

/** 
 * \brief
 * If set, a zero-delay pause frame is transmitted when flow control is
 * deasserted.
 *
 * \details 
 * '0': Don't send zero pause frame.
 * '1': Send zero pause frame.
 *
 * Field: ::VTSS_SYS_PAUSE_CFG_MAC_FC_CFG . ZERO_PAUSE_ENA
 */
#define  VTSS_F_SYS_PAUSE_CFG_MAC_FC_CFG_ZERO_PAUSE_ENA  VTSS_BIT(18)

/** 
 * \brief
 * When set the MAC will send pause control frames in the Tx direction.
 *
 * \details 
 * '0': Don't send pause control frames
 * '1': Send pause control frames
 *
 * Field: ::VTSS_SYS_PAUSE_CFG_MAC_FC_CFG . TX_FC_ENA
 */
#define  VTSS_F_SYS_PAUSE_CFG_MAC_FC_CFG_TX_FC_ENA  VTSS_BIT(17)

/** 
 * \brief
 * When set the MAC obeys received pause control frames
 *
 * \details 
 * '0': Don't obey received pause control frames
 * '1': Obey received pause control frames.
 *
 * Field: ::VTSS_SYS_PAUSE_CFG_MAC_FC_CFG . RX_FC_ENA
 */
#define  VTSS_F_SYS_PAUSE_CFG_MAC_FC_CFG_RX_FC_ENA  VTSS_BIT(16)

/** 
 * \brief
 * Pause timer value inserted in generated pause frames.
 *
 * \details 
 * 0: Insert timer value 0 in TX pause frame.
 * ...
 * N: Insert timer value N in TX pause frame.
 *
 * Field: ::VTSS_SYS_PAUSE_CFG_MAC_FC_CFG . PAUSE_VAL_CFG
 */
#define  VTSS_F_SYS_PAUSE_CFG_MAC_FC_CFG_PAUSE_VAL_CFG(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_SYS_PAUSE_CFG_MAC_FC_CFG_PAUSE_VAL_CFG     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_SYS_PAUSE_CFG_MAC_FC_CFG_PAUSE_VAL_CFG(x)  VTSS_EXTRACT_BITFIELD(x,0,16)

/**
 * Register Group: \a SYS:MMGT
 *
 * Memory manager status
 */


/** 
 * \brief Packet memory status
 *
 * \details
 * Register: \a SYS:MMGT:MMGT
 */
#define VTSS_SYS_MMGT_MMGT                   VTSS_IOREG(VTSS_TO_SYS,0x135)

/** 
 * \brief
 * Number of 192-byte free memory words.
 *
 * \details 
 * Field: ::VTSS_SYS_MMGT_MMGT . FREECNT
 */
#define  VTSS_F_SYS_MMGT_MMGT_FREECNT(x)      VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_SYS_MMGT_MMGT_FREECNT         VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_SYS_MMGT_MMGT_FREECNT(x)      VTSS_EXTRACT_BITFIELD(x,0,16)


/** 
 * \brief Packet Memory Status
 *
 * \details
 * Register: \a SYS:MMGT:MMGT_FAST
 */
#define VTSS_SYS_MMGT_MMGT_FAST              VTSS_IOREG(VTSS_TO_SYS,0x136)

/**
 * Register Group: \a SYS:MISC
 *
 * Miscellaneous
 */

/**
 * Register Group: \a SYS:STAT
 *
 * Frame statistics
 */


/** 
 * \brief Counter values
 *
 * \details
 * Register: \a SYS:STAT:CNT
 *
 * @param gi Replicator: x_STAT_CNT (??), 0-191
 */
#define VTSS_SYS_STAT_CNT(gi)                VTSS_IOREG_IX(VTSS_TO_SYS,0x0,gi,1,0,0)


#endif /* _VTSS_SEVILLE_REGS_SYS_H_ */
