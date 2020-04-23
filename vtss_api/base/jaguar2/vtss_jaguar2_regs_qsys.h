#ifndef _VTSS_JAGUAR2_REGS_QSYS_H_
#define _VTSS_JAGUAR2_REGS_QSYS_H_

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
 * Target: \a QSYS
 *
 * Queue System Configuration
 *
 ***********************************************************************/

/**
 * Register Group: \a QSYS:SYSTEM
 *
 * Switch configuration
 */


/** 
 * \brief Control Energy Efficient Ethernet operation per front port.
 *
 * \details
 * Register: \a QSYS:SYSTEM:EEE_CFG
 *
 * @param ri Replicator: x_PORT_CNT (??), 0-56
 */
#define VTSS_QSYS_SYSTEM_EEE_CFG(ri)         VTSS_IOREG(VTSS_TO_QSYS,0x402 + (ri))

/** 
 * \brief
 * Queues set in this mask activate the egress port immediately when any of
 * the queues have data available.
 *
 * \details 
 * Field: ::VTSS_QSYS_SYSTEM_EEE_CFG . EEE_FAST_QUEUES
 */
#define  VTSS_F_QSYS_SYSTEM_EEE_CFG_EEE_FAST_QUEUES(x)  VTSS_ENCODE_BITFIELD(x,0,8)
#define  VTSS_M_QSYS_SYSTEM_EEE_CFG_EEE_FAST_QUEUES     VTSS_ENCODE_BITMASK(0,8)
#define  VTSS_X_QSYS_SYSTEM_EEE_CFG_EEE_FAST_QUEUES(x)  VTSS_EXTRACT_BITFIELD(x,0,8)


/** 
 * \brief Thresholds for delayed EEE queues
 *
 * \details
 * Register: \a QSYS:SYSTEM:EEE_THRES
 */
#define VTSS_QSYS_SYSTEM_EEE_THRES           VTSS_IOREG(VTSS_TO_QSYS,0x43b)

/** 
 * \brief
 * Maximum number of bytes in a queue before egress port is activated. Unit
 * is 60 bytes.
 *
 * \details 
 * Field: ::VTSS_QSYS_SYSTEM_EEE_THRES . EEE_HIGH_BYTES
 */
#define  VTSS_F_QSYS_SYSTEM_EEE_THRES_EEE_HIGH_BYTES(x)  VTSS_ENCODE_BITFIELD(x,8,8)
#define  VTSS_M_QSYS_SYSTEM_EEE_THRES_EEE_HIGH_BYTES     VTSS_ENCODE_BITMASK(8,8)
#define  VTSS_X_QSYS_SYSTEM_EEE_THRES_EEE_HIGH_BYTES(x)  VTSS_EXTRACT_BITFIELD(x,8,8)

/** 
 * \brief
 * Maximum number of frames in a queue before the egress port is activated.
 * Unit is 1 frame.
 *
 * \details 
 * Field: ::VTSS_QSYS_SYSTEM_EEE_THRES . EEE_HIGH_FRAMES
 */
#define  VTSS_F_QSYS_SYSTEM_EEE_THRES_EEE_HIGH_FRAMES(x)  VTSS_ENCODE_BITFIELD(x,0,8)
#define  VTSS_M_QSYS_SYSTEM_EEE_THRES_EEE_HIGH_FRAMES     VTSS_ENCODE_BITMASK(0,8)
#define  VTSS_X_QSYS_SYSTEM_EEE_THRES_EEE_HIGH_FRAMES(x)  VTSS_EXTRACT_BITFIELD(x,0,8)


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
 * Register: \a QSYS:SYSTEM:RESET_CFG
 */
#define VTSS_QSYS_SYSTEM_RESET_CFG           VTSS_IOREG(VTSS_TO_QSYS,0x43c)

/** 
 * \brief
 * Switch core is enabled when this field is set.
 *
 * \details 
 * Field: ::VTSS_QSYS_SYSTEM_RESET_CFG . CORE_ENA
 */
#define  VTSS_F_QSYS_SYSTEM_RESET_CFG_CORE_ENA(x)  VTSS_ENCODE_BITFIELD(!!(x),0,1)
#define  VTSS_M_QSYS_SYSTEM_RESET_CFG_CORE_ENA  VTSS_BIT(0)
#define  VTSS_X_QSYS_SYSTEM_RESET_CFG_CORE_ENA(x)  VTSS_EXTRACT_BITFIELD(x,0,1)


/** 
 * \brief Packet memory status
 *
 * \details
 * Register: \a QSYS:SYSTEM:PMEM_SIZE
 */
#define VTSS_QSYS_SYSTEM_PMEM_SIZE           VTSS_IOREG(VTSS_TO_QSYS,0x43d)

/** 
 * \brief
 * Must be set before core reset is released. Unit is  bytes.
 *
 * \details 
 * Field: ::VTSS_QSYS_SYSTEM_PMEM_SIZE . PMEM_SIZE
 */
#define  VTSS_F_QSYS_SYSTEM_PMEM_SIZE_PMEM_SIZE(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_QSYS_SYSTEM_PMEM_SIZE_PMEM_SIZE     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_QSYS_SYSTEM_PMEM_SIZE_PMEM_SIZE(x)  VTSS_EXTRACT_BITFIELD(x,0,16)


/** 
 * \brief Frame aging configuration
 *
 * \details
 * Register: \a QSYS:SYSTEM:FRM_AGING
 */
#define VTSS_QSYS_SYSTEM_FRM_AGING           VTSS_IOREG(VTSS_TO_QSYS,0x43e)

/** 
 * \brief
 * Frames are aged and removed from the queue system when the frame's age
 * timer becomes two. The frame age timer is increased for all frames
 * whenever the configured time, MAX_AGE,  has passed. 
 * 
 * Effectively, this means that a frame is aged when the frame has waited
 * in the queue system between one or two times the period specified by
 * MAX_AGE.
 * 
 * The periodd configured by MAX_AGE is also termed an "age era".
 * 
 * A value of zero disables frame aging. 
 * 
 * Frame aging related parameters:
 * QSYS:SYSTEM:FRM_AGING.MAX_AGE
 * HSCH:HSCH_MISC:PORT_MODE.AGE_DIS
 * DSM:CFG:BUF_CFG.AGING_ENA
 *
 * \details 
 * 0: Disable frame aging.
 * >=1: Era length in unit of 4us.
 *
 * Field: ::VTSS_QSYS_SYSTEM_FRM_AGING . MAX_AGE
 */
#define  VTSS_F_QSYS_SYSTEM_FRM_AGING_MAX_AGE(x)  VTSS_ENCODE_BITFIELD(x,0,20)
#define  VTSS_M_QSYS_SYSTEM_FRM_AGING_MAX_AGE     VTSS_ENCODE_BITMASK(0,20)
#define  VTSS_X_QSYS_SYSTEM_FRM_AGING_MAX_AGE(x)  VTSS_EXTRACT_BITFIELD(x,0,20)


/** 
 * \brief Statistics configuration
 *
 * \details
 * Register: \a QSYS:SYSTEM:STAT_CFG
 */
#define VTSS_QSYS_SYSTEM_STAT_CFG            VTSS_IOREG(VTSS_TO_QSYS,0x43f)

/** 
 * \brief
 * Set STAT_CLEAR_SHOT to clear counters for the port or service index
 * selected by STAT_VIEW. 
 * Auto-cleared when complete (1us). 
 * 
 * Multiple counters can be cleared at the same time by setting multiple
 * bits in STAT_CLEAR_SHOT.

 *
 * \details 
 * Bit 0: Clear Rx port counters (Packet, LS byte and MS byte)
 * Bit 1: Clear Tx port counters (Packet, LS byte and MS byte)
 * Bit 2: Clear ingress service counters (Packet, LS byte and MS byte)
 * Bit 3: Clear egress service counters (Packet, LS byte and MS byte)
 * 
 * When bits 0-1 are used a port number must be configured in STAT_VIEW.
 * When bits 2 is used an ingress service index must be configured in
 * STAT_VIEW.
 * When bits 3 is used an egress service index must be configured in
 * STAT_VIEW.

 *
 * Field: ::VTSS_QSYS_SYSTEM_STAT_CFG . STAT_CLEAR_SHOT
 */
#define  VTSS_F_QSYS_SYSTEM_STAT_CFG_STAT_CLEAR_SHOT(x)  VTSS_ENCODE_BITFIELD(x,13,4)
#define  VTSS_M_QSYS_SYSTEM_STAT_CFG_STAT_CLEAR_SHOT     VTSS_ENCODE_BITMASK(13,4)
#define  VTSS_X_QSYS_SYSTEM_STAT_CFG_STAT_CLEAR_SHOT(x)  VTSS_EXTRACT_BITFIELD(x,13,4)

/** 
 * \brief
 * Selects the port or service for which counters can be accessed using
 * QSYS:STAT.
 * 
 * Also used to select the port or service index for which to clear
 * statistics counters, ref. STAT_CFG.STAT_CLEAR_SHOT.

 *
 * \details 
 * Field: ::VTSS_QSYS_SYSTEM_STAT_CFG . STAT_VIEW
 */
#define  VTSS_F_QSYS_SYSTEM_STAT_CFG_STAT_VIEW(x)  VTSS_ENCODE_BITFIELD(x,0,13)
#define  VTSS_M_QSYS_SYSTEM_STAT_CFG_STAT_VIEW     VTSS_ENCODE_BITMASK(0,13)
#define  VTSS_X_QSYS_SYSTEM_STAT_CFG_STAT_VIEW(x)  VTSS_EXTRACT_BITFIELD(x,0,13)


/** 
 * \brief Map DP values to color
 *
 * \details
 * Register: \a QSYS:SYSTEM:DP_MAP
 */
#define VTSS_QSYS_SYSTEM_DP_MAP              VTSS_IOREG(VTSS_TO_QSYS,0x440)

/** 
 * \brief
 * Drop precedence N uses mapping table given by bit N in this field.
 *
 * \details 
 * xxx0: Map DP value 0 to DE value 0
 * xxx1: Map DP value 0 to DE value 1
 * ...
 * 0xxx: Map DP value 3 to DE value 0
 * 1xxx: Map DP value 3 to DE value 1

 *
 * Field: ::VTSS_QSYS_SYSTEM_DP_MAP . DP
 */
#define  VTSS_F_QSYS_SYSTEM_DP_MAP_DP(x)      VTSS_ENCODE_BITFIELD(x,0,4)
#define  VTSS_M_QSYS_SYSTEM_DP_MAP_DP         VTSS_ENCODE_BITMASK(0,4)
#define  VTSS_X_QSYS_SYSTEM_DP_MAP_DP(x)      VTSS_EXTRACT_BITFIELD(x,0,4)

/**
 * Register Group: \a QSYS:MISC
 *
 * Miscellaneous
 */


/** 
 * \brief Internal events for debugging only
 *
 * \details
 * Core events.
 *
 * Register: \a QSYS:MISC:EVENTS_CORE
 */
#define VTSS_QSYS_MISC_EVENTS_CORE           VTSS_IOREG(VTSS_TO_QSYS,0x441)

/** 
 * \brief
 * Look in the RTL!
 *
 * \details 
 * Field: ::VTSS_QSYS_MISC_EVENTS_CORE . EV_FWR
 */
#define  VTSS_F_QSYS_MISC_EVENTS_CORE_EV_FWR(x)  VTSS_ENCODE_BITFIELD(!!(x),3,1)
#define  VTSS_M_QSYS_MISC_EVENTS_CORE_EV_FWR  VTSS_BIT(3)
#define  VTSS_X_QSYS_MISC_EVENTS_CORE_EV_FWR(x)  VTSS_EXTRACT_BITFIELD(x,3,1)

/** 
 * \brief
 * Look in the RTL!
 *
 * \details 
 * Field: ::VTSS_QSYS_MISC_EVENTS_CORE . EV_FDC
 */
#define  VTSS_F_QSYS_MISC_EVENTS_CORE_EV_FDC(x)  VTSS_ENCODE_BITFIELD(x,0,3)
#define  VTSS_M_QSYS_MISC_EVENTS_CORE_EV_FDC     VTSS_ENCODE_BITMASK(0,3)
#define  VTSS_X_QSYS_MISC_EVENTS_CORE_EV_FDC(x)  VTSS_EXTRACT_BITFIELD(x,0,3)

/**
 * Register Group: \a QSYS:PAUSE_CFG
 *
 * Watermarks for egress queue system
 */


/** 
 * \brief Watermarks for flow control condition per switch port.
 *
 * \details
 * Register: \a QSYS:PAUSE_CFG:PAUSE_CFG
 *
 * @param ri Replicator: x_PORT_CNT (??), 0-56
 */
#define VTSS_QSYS_PAUSE_CFG_PAUSE_CFG(ri)    VTSS_IOREG(VTSS_TO_QSYS,0x442 + (ri))

/** 
 * \brief
 * Start pausing ingress stream when the amount of memory consumed by the
 * port exceeds this watermark. The TOTPAUSE condition must also be met.
 *
 * \details 
 * See RES_CFG
 *
 * Field: ::VTSS_QSYS_PAUSE_CFG_PAUSE_CFG . PAUSE_START
 */
#define  VTSS_F_QSYS_PAUSE_CFG_PAUSE_CFG_PAUSE_START(x)  VTSS_ENCODE_BITFIELD(x,13,12)
#define  VTSS_M_QSYS_PAUSE_CFG_PAUSE_CFG_PAUSE_START     VTSS_ENCODE_BITMASK(13,12)
#define  VTSS_X_QSYS_PAUSE_CFG_PAUSE_CFG_PAUSE_START(x)  VTSS_EXTRACT_BITFIELD(x,13,12)

/** 
 * \brief
 * Stop pausing ingress stream when the amount of memory consumed by the
 * port is below  this watermark.
 *
 * \details 
 * See RES_CFG.
 *
 * Field: ::VTSS_QSYS_PAUSE_CFG_PAUSE_CFG . PAUSE_STOP
 */
#define  VTSS_F_QSYS_PAUSE_CFG_PAUSE_CFG_PAUSE_STOP(x)  VTSS_ENCODE_BITFIELD(x,1,12)
#define  VTSS_M_QSYS_PAUSE_CFG_PAUSE_CFG_PAUSE_STOP     VTSS_ENCODE_BITMASK(1,12)
#define  VTSS_X_QSYS_PAUSE_CFG_PAUSE_CFG_PAUSE_STOP(x)  VTSS_EXTRACT_BITFIELD(x,1,12)

/** 
 * \brief
 * Enable pause feedback to the MAC, allowing transmission of pause frames
 * or HDX collisions to limit ingress data rate.
 *
 * \details 
 * Field: ::VTSS_QSYS_PAUSE_CFG_PAUSE_CFG . PAUSE_ENA
 */
#define  VTSS_F_QSYS_PAUSE_CFG_PAUSE_CFG_PAUSE_ENA(x)  VTSS_ENCODE_BITFIELD(!!(x),0,1)
#define  VTSS_M_QSYS_PAUSE_CFG_PAUSE_CFG_PAUSE_ENA  VTSS_BIT(0)
#define  VTSS_X_QSYS_PAUSE_CFG_PAUSE_CFG_PAUSE_ENA(x)  VTSS_EXTRACT_BITFIELD(x,0,1)


/** 
 * \brief Configure total memory pause condition
 *
 * \details
 * Register: \a QSYS:PAUSE_CFG:PAUSE_TOT_CFG
 */
#define VTSS_QSYS_PAUSE_CFG_PAUSE_TOT_CFG    VTSS_IOREG(VTSS_TO_QSYS,0x47b)

/** 
 * \brief
 * Assert TOTPAUSE condition when total memory allocation is above this
 * watermark.
 *
 * \details 
 * See RES_CFG
 *
 * Field: ::VTSS_QSYS_PAUSE_CFG_PAUSE_TOT_CFG . PAUSE_TOT_START
 */
#define  VTSS_F_QSYS_PAUSE_CFG_PAUSE_TOT_CFG_PAUSE_TOT_START(x)  VTSS_ENCODE_BITFIELD(x,12,12)
#define  VTSS_M_QSYS_PAUSE_CFG_PAUSE_TOT_CFG_PAUSE_TOT_START     VTSS_ENCODE_BITMASK(12,12)
#define  VTSS_X_QSYS_PAUSE_CFG_PAUSE_TOT_CFG_PAUSE_TOT_START(x)  VTSS_EXTRACT_BITFIELD(x,12,12)

/** 
 * \brief
 * Deassert TOTPAUSE condition when total memory allocation is below this
 * watermark.
 *
 * \details 
 * See RES_CFG
 *
 * Field: ::VTSS_QSYS_PAUSE_CFG_PAUSE_TOT_CFG . PAUSE_TOT_STOP
 */
#define  VTSS_F_QSYS_PAUSE_CFG_PAUSE_TOT_CFG_PAUSE_TOT_STOP(x)  VTSS_ENCODE_BITFIELD(x,0,12)
#define  VTSS_M_QSYS_PAUSE_CFG_PAUSE_TOT_CFG_PAUSE_TOT_STOP     VTSS_ENCODE_BITMASK(0,12)
#define  VTSS_X_QSYS_PAUSE_CFG_PAUSE_TOT_CFG_PAUSE_TOT_STOP(x)  VTSS_EXTRACT_BITFIELD(x,0,12)


/** 
 * \brief Tail dropping level
 *
 * \details
 * Register: \a QSYS:PAUSE_CFG:ATOP
 *
 * @param ri Replicator: x_PORT_CNT (??), 0-56
 */
#define VTSS_QSYS_PAUSE_CFG_ATOP(ri)         VTSS_IOREG(VTSS_TO_QSYS,0x47c + (ri))

/** 
 * \brief
 * When a source port consumes more than this level in the packet memory,
 * frames are tail dropped, unconditionally of destination. 
 *
 * \details 
 * See RES_CFG
 *
 * Field: ::VTSS_QSYS_PAUSE_CFG_ATOP . ATOP
 */
#define  VTSS_F_QSYS_PAUSE_CFG_ATOP_ATOP(x)   VTSS_ENCODE_BITFIELD(x,0,12)
#define  VTSS_M_QSYS_PAUSE_CFG_ATOP_ATOP      VTSS_ENCODE_BITMASK(0,12)
#define  VTSS_X_QSYS_PAUSE_CFG_ATOP_ATOP(x)   VTSS_EXTRACT_BITFIELD(x,0,12)


/** 
 * \brief Forward pressure level
 *
 * \details
 * Register: \a QSYS:PAUSE_CFG:FWD_PRESSURE
 *
 * @param ri Replicator: x_PORT_CNT (??), 0-56
 */
#define VTSS_QSYS_PAUSE_CFG_FWD_PRESSURE(ri)  VTSS_IOREG(VTSS_TO_QSYS,0x4b5 + (ri))

/** 
 * \brief
 * When ingress queue request count exceeds this level, maximum number of
 * generated frame copies is limited to the FWD_PRESSURE_COPYCNT value. 
 *
 * \details 
 * See RES_CFG
 *
 * Field: ::VTSS_QSYS_PAUSE_CFG_FWD_PRESSURE . FWD_PRESSURE
 */
#define  VTSS_F_QSYS_PAUSE_CFG_FWD_PRESSURE_FWD_PRESSURE(x)  VTSS_ENCODE_BITFIELD(x,3,12)
#define  VTSS_M_QSYS_PAUSE_CFG_FWD_PRESSURE_FWD_PRESSURE     VTSS_ENCODE_BITMASK(3,12)
#define  VTSS_X_QSYS_PAUSE_CFG_FWD_PRESSURE_FWD_PRESSURE(x)  VTSS_EXTRACT_BITFIELD(x,3,12)

/** 
 * \brief
 * Maximum frame copy count when fwd pressure is activated. 
 *
 * \details 
 * Field: ::VTSS_QSYS_PAUSE_CFG_FWD_PRESSURE . FWD_PRESSURE_COPYCNT
 */
#define  VTSS_F_QSYS_PAUSE_CFG_FWD_PRESSURE_FWD_PRESSURE_COPYCNT(x)  VTSS_ENCODE_BITFIELD(x,0,3)
#define  VTSS_M_QSYS_PAUSE_CFG_FWD_PRESSURE_FWD_PRESSURE_COPYCNT     VTSS_ENCODE_BITMASK(0,3)
#define  VTSS_X_QSYS_PAUSE_CFG_FWD_PRESSURE_FWD_PRESSURE_COPYCNT(x)  VTSS_EXTRACT_BITFIELD(x,0,3)


/** 
 * \brief Total raw memory use before tail dropping is activated
 *
 * \details
 * Register: \a QSYS:PAUSE_CFG:ATOP_TOT_CFG
 */
#define VTSS_QSYS_PAUSE_CFG_ATOP_TOT_CFG     VTSS_IOREG(VTSS_TO_QSYS,0x4ee)

/** 
 * \brief
 * Tail dropping is activate on a port when the port use has exceeded the
 * ATOP watermark for the port, and the total memory use has exceeded this
 * watermark.
 *
 * \details 
 * See RES_CFG
 *
 * Field: ::VTSS_QSYS_PAUSE_CFG_ATOP_TOT_CFG . ATOP_TOT
 */
#define  VTSS_F_QSYS_PAUSE_CFG_ATOP_TOT_CFG_ATOP_TOT(x)  VTSS_ENCODE_BITFIELD(x,0,12)
#define  VTSS_M_QSYS_PAUSE_CFG_ATOP_TOT_CFG_ATOP_TOT     VTSS_ENCODE_BITMASK(0,12)
#define  VTSS_X_QSYS_PAUSE_CFG_ATOP_TOT_CFG_ATOP_TOT(x)  VTSS_EXTRACT_BITFIELD(x,0,12)

/**
 * Register Group: \a QSYS:MMGT
 *
 * Memory manager status
 */


/** 
 * \brief Egress queue status
 *
 * \details
 * Register: \a QSYS:MMGT:EQ_STAT
 */
#define VTSS_QSYS_MMGT_EQ_STAT               VTSS_IOREG(VTSS_TO_QSYS,0x4ef)

/** 
 * \brief
 * Number of free frame references.
 *
 * \details 
 * Field: ::VTSS_QSYS_MMGT_EQ_STAT . FP_FREE_CNT
 */
#define  VTSS_F_QSYS_MMGT_EQ_STAT_FP_FREE_CNT(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_QSYS_MMGT_EQ_STAT_FP_FREE_CNT     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_QSYS_MMGT_EQ_STAT_FP_FREE_CNT(x)  VTSS_EXTRACT_BITFIELD(x,0,16)


/** 
 * \brief Packet memory status
 *
 * \details
 * Register: \a QSYS:MMGT:MMGT
 */
#define VTSS_QSYS_MMGT_MMGT                  VTSS_IOREG(VTSS_TO_QSYS,0x4f0)

/** 
 * \brief
 * Number of 6 frames awaiting releasing.
 *
 * \details 
 * Field: ::VTSS_QSYS_MMGT_MMGT . RELCNT
 */
#define  VTSS_F_QSYS_MMGT_MMGT_RELCNT(x)      VTSS_ENCODE_BITFIELD(x,16,16)
#define  VTSS_M_QSYS_MMGT_MMGT_RELCNT         VTSS_ENCODE_BITMASK(16,16)
#define  VTSS_X_QSYS_MMGT_MMGT_RELCNT(x)      VTSS_EXTRACT_BITFIELD(x,16,16)

/** 
 * \brief
 * Number of 6 free memory words.
 *
 * \details 
 * Field: ::VTSS_QSYS_MMGT_MMGT . FREECNT
 */
#define  VTSS_F_QSYS_MMGT_MMGT_FREECNT(x)     VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_QSYS_MMGT_MMGT_FREECNT        VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_QSYS_MMGT_MMGT_FREECNT(x)     VTSS_EXTRACT_BITFIELD(x,0,16)


/** 
 * \brief Packet Memory Status
 *
 * \details
 * Register: \a QSYS:MMGT:MMGT_FAST
 */
#define VTSS_QSYS_MMGT_MMGT_FAST             VTSS_IOREG(VTSS_TO_QSYS,0x4f1)

/** 
 * \brief
 * Number of words in the fast pool

 *
 * \details 
 * Field: ::VTSS_QSYS_MMGT_MMGT_FAST . FREEVLD
 */
#define  VTSS_F_QSYS_MMGT_MMGT_FAST_FREEVLD(x)  VTSS_ENCODE_BITFIELD(x,4,4)
#define  VTSS_M_QSYS_MMGT_MMGT_FAST_FREEVLD     VTSS_ENCODE_BITMASK(4,4)
#define  VTSS_X_QSYS_MMGT_MMGT_FAST_FREEVLD(x)  VTSS_EXTRACT_BITFIELD(x,4,4)

/** 
 * \brief
 * Number of frames awaiting release in the fast pool
 *
 * \details 
 * Field: ::VTSS_QSYS_MMGT_MMGT_FAST . RELVLD
 */
#define  VTSS_F_QSYS_MMGT_MMGT_FAST_RELVLD(x)  VTSS_ENCODE_BITFIELD(x,0,4)
#define  VTSS_M_QSYS_MMGT_MMGT_FAST_RELVLD     VTSS_ENCODE_BITMASK(0,4)
#define  VTSS_X_QSYS_MMGT_MMGT_FAST_RELVLD(x)  VTSS_EXTRACT_BITFIELD(x,0,4)

/**
 * Register Group: \a QSYS:MMGT_PORT
 *
 * Memory manager status
 */


/** 
 * \brief Port to see values for
 *
 * \details
 * Register: \a QSYS:MMGT_PORT:MMGT_PORT_VIEW
 */
#define VTSS_QSYS_MMGT_PORT_MMGT_PORT_VIEW   VTSS_IOREG(VTSS_TO_QSYS,0x4f2)

/** 
 * \brief
 * Select port to see consumption values for
 *
 * \details 
 * Field: ::VTSS_QSYS_MMGT_PORT_MMGT_PORT_VIEW . MMGT_PORT_VIEW
 */
#define  VTSS_F_QSYS_MMGT_PORT_MMGT_PORT_VIEW_MMGT_PORT_VIEW(x)  VTSS_ENCODE_BITFIELD(x,0,6)
#define  VTSS_M_QSYS_MMGT_PORT_MMGT_PORT_VIEW_MMGT_PORT_VIEW     VTSS_ENCODE_BITMASK(0,6)
#define  VTSS_X_QSYS_MMGT_PORT_MMGT_PORT_VIEW_MMGT_PORT_VIEW(x)  VTSS_EXTRACT_BITFIELD(x,0,6)


/** 
 * \brief Total consumption per ingress port
 *
 * \details
 * Register: \a QSYS:MMGT_PORT:MMGT_PORT_USE
 */
#define VTSS_QSYS_MMGT_PORT_MMGT_PORT_USE    VTSS_IOREG(VTSS_TO_QSYS,0x4f3)

/** 
 * \brief
 * Total consumption per port in the memory manager. Unit is one cell (176
 * bytes).
 *
 * \details 
 * Field: ::VTSS_QSYS_MMGT_PORT_MMGT_PORT_USE . MMGT_PORT_USE
 */
#define  VTSS_F_QSYS_MMGT_PORT_MMGT_PORT_USE_MMGT_PORT_USE(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_QSYS_MMGT_PORT_MMGT_PORT_USE_MMGT_PORT_USE     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_QSYS_MMGT_PORT_MMGT_PORT_USE_MMGT_PORT_USE(x)  VTSS_EXTRACT_BITFIELD(x,0,16)


/** 
 * \brief Ingress queue status
 *
 * \details
 * Register: \a QSYS:MMGT_PORT:MMGT_IQ_STAT
 */
#define VTSS_QSYS_MMGT_PORT_MMGT_IQ_STAT     VTSS_IOREG(VTSS_TO_QSYS,0x4f4)

/** 
 * \brief
 * Number of frame copies pending in the ingress queue
 *
 * \details 
 * Field: ::VTSS_QSYS_MMGT_PORT_MMGT_IQ_STAT . MMGT_IQ_SIZE
 */
#define  VTSS_F_QSYS_MMGT_PORT_MMGT_IQ_STAT_MMGT_IQ_SIZE(x)  VTSS_ENCODE_BITFIELD(x,0,20)
#define  VTSS_M_QSYS_MMGT_PORT_MMGT_IQ_STAT_MMGT_IQ_SIZE     VTSS_ENCODE_BITMASK(0,20)
#define  VTSS_X_QSYS_MMGT_PORT_MMGT_IQ_STAT_MMGT_IQ_SIZE(x)  VTSS_EXTRACT_BITFIELD(x,0,20)

/**
 * Register Group: \a QSYS:STAT
 *
 * Frame statistics
 */


/** 
 * \brief Counter values
 *
 * \details
 * See description for QSYS:STAT.
 *
 * Register: \a QSYS:STAT:CNT
 *
 * @param gi Register: STAT (??), 0-1023
 */
#define VTSS_QSYS_STAT_CNT(gi)               VTSS_IOREG_IX(VTSS_TO_QSYS,0x0,gi,1,0,0)

/**
 * Register Group: \a QSYS:CALCFG
 *
 * Not documented
 */


/** 
 * \brief Automatic calendar setup
 *
 * \details
 * These four registers configures the automaric calendar for port 0-15,
 * 16-31, 32-47 and 48-55.
 *
 * Register: \a QSYS:CALCFG:CAL_AUTO
 *
 * @param ri Register: CAL_AUTO (??), 0-3
 */
#define VTSS_QSYS_CALCFG_CAL_AUTO(ri)        VTSS_IOREG(VTSS_TO_QSYS,0x4f5 + (ri))


/** 
 * \brief Manual calendar setup
 *
 * \details
 * Register: \a QSYS:CALCFG:CAL_SEQ
 */
#define VTSS_QSYS_CALCFG_CAL_SEQ             VTSS_IOREG(VTSS_TO_QSYS,0x4f9)

/** 
 * \brief
 * Last entry in manual sequence. CAL_SEQ only accessible while calendar is
 * halted.
 *
 * \details 
 * Field: ::VTSS_QSYS_CALCFG_CAL_SEQ . CAL_SEQ_LEN
 */
#define  VTSS_F_QSYS_CALCFG_CAL_SEQ_CAL_SEQ_LEN(x)  VTSS_ENCODE_BITFIELD(x,13,9)
#define  VTSS_M_QSYS_CALCFG_CAL_SEQ_CAL_SEQ_LEN     VTSS_ENCODE_BITMASK(13,9)
#define  VTSS_X_QSYS_CALCFG_CAL_SEQ_CAL_SEQ_LEN(x)  VTSS_EXTRACT_BITFIELD(x,13,9)

/** 
 * \brief
 * Port to service in the cycle given by the CAL_SEQ_LEN value.
 *
 * \details 
 * Field: ::VTSS_QSYS_CALCFG_CAL_SEQ . CAL_SEQ_CUR_VAL
 */
#define  VTSS_F_QSYS_CALCFG_CAL_SEQ_CAL_SEQ_CUR_VAL(x)  VTSS_ENCODE_BITFIELD(x,7,6)
#define  VTSS_M_QSYS_CALCFG_CAL_SEQ_CAL_SEQ_CUR_VAL     VTSS_ENCODE_BITMASK(7,6)
#define  VTSS_X_QSYS_CALCFG_CAL_SEQ_CAL_SEQ_CUR_VAL(x)  VTSS_EXTRACT_BITFIELD(x,7,6)

/** 
 * \brief
 * The CAL_SEQ_PGM_VAL is written into the sequence entry CAL_SEQ_LEN.
 *
 * \details 
 * Field: ::VTSS_QSYS_CALCFG_CAL_SEQ . CAL_SEQ_PGM_VAL
 */
#define  VTSS_F_QSYS_CALCFG_CAL_SEQ_CAL_SEQ_PGM_VAL(x)  VTSS_ENCODE_BITFIELD(x,1,6)
#define  VTSS_M_QSYS_CALCFG_CAL_SEQ_CAL_SEQ_PGM_VAL     VTSS_ENCODE_BITMASK(1,6)
#define  VTSS_X_QSYS_CALCFG_CAL_SEQ_CAL_SEQ_PGM_VAL(x)  VTSS_EXTRACT_BITFIELD(x,1,6)

/** 
 * \brief
 * Enable update of the CAL_SEQ.
 *
 * \details 
 * Field: ::VTSS_QSYS_CALCFG_CAL_SEQ . CAL_SEQ_PGM_ENA
 */
#define  VTSS_F_QSYS_CALCFG_CAL_SEQ_CAL_SEQ_PGM_ENA(x)  VTSS_ENCODE_BITFIELD(!!(x),0,1)
#define  VTSS_M_QSYS_CALCFG_CAL_SEQ_CAL_SEQ_PGM_ENA  VTSS_BIT(0)
#define  VTSS_X_QSYS_CALCFG_CAL_SEQ_CAL_SEQ_PGM_ENA(x)  VTSS_EXTRACT_BITFIELD(x,0,1)


/** 
 * \brief Calendar configuration
 *
 * \details
 * Register: \a QSYS:CALCFG:CAL_CTRL
 */
#define VTSS_QSYS_CALCFG_CAL_CTRL            VTSS_IOREG(VTSS_TO_QSYS,0x4fa)

/** 
 * \brief
 * The internal bus calendars can be set to a predefined port mode
 * (settings 0-3), to an automatic sequence generated by the CAL_AUTO
 * registers, or to a specific sequence programmed through the CAL_SEQ
 * fields. 
 *
 * \details 
 * 0: 48x1G + 2x10G
 * 1:  Reserved
 * 2:  52x1G
 * 3:  8x1G + 24x2.5G
 * 4:  8x1G + 8x2.5G + 4x10G
 * 5:  24x1G + 4x10G
 * 8:  CAL_AUTO port setting 
 * 9:  Sequence program
 * 10: Halt
 *
 * Field: ::VTSS_QSYS_CALCFG_CAL_CTRL . CAL_MODE
 */
#define  VTSS_F_QSYS_CALCFG_CAL_CTRL_CAL_MODE(x)  VTSS_ENCODE_BITFIELD(x,9,4)
#define  VTSS_M_QSYS_CALCFG_CAL_CTRL_CAL_MODE     VTSS_ENCODE_BITMASK(9,4)
#define  VTSS_X_QSYS_CALCFG_CAL_CTRL_CAL_MODE(x)  VTSS_EXTRACT_BITFIELD(x,9,4)

/** 
 * \brief
 * Configures the time between bus grants to 10g ports. Other speeds will
 * be adjusted relative to the 10g setting. Default value of 8 matches a
 * 250MHz core clock, granting a cell transfer every 64 ns.
 *
 * \details 
 * Field: ::VTSS_QSYS_CALCFG_CAL_CTRL . CAL_AUTOCYCLES_10G
 */
#define  VTSS_F_QSYS_CALCFG_CAL_CTRL_CAL_AUTOCYCLES_10G(x)  VTSS_ENCODE_BITFIELD(x,2,7)
#define  VTSS_M_QSYS_CALCFG_CAL_CTRL_CAL_AUTOCYCLES_10G     VTSS_ENCODE_BITMASK(2,7)
#define  VTSS_X_QSYS_CALCFG_CAL_CTRL_CAL_AUTOCYCLES_10G(x)  VTSS_EXTRACT_BITFIELD(x,2,7)

/** 
 * \brief
 * The automatic calendar failed to service all ports.
 *
 * \details 
 * Field: ::VTSS_QSYS_CALCFG_CAL_CTRL . CAL_AUTO_ERROR
 */
#define  VTSS_F_QSYS_CALCFG_CAL_CTRL_CAL_AUTO_ERROR(x)  VTSS_ENCODE_BITFIELD(!!(x),1,1)
#define  VTSS_M_QSYS_CALCFG_CAL_CTRL_CAL_AUTO_ERROR  VTSS_BIT(1)
#define  VTSS_X_QSYS_CALCFG_CAL_CTRL_CAL_AUTO_ERROR(x)  VTSS_EXTRACT_BITFIELD(x,1,1)

/** 
 * \brief
 * When enabled, outbound cellbus can service loopback paths when
 * designated port is seen idle.
 *
 * \details 
 * Field: ::VTSS_QSYS_CALCFG_CAL_CTRL . CAL_VD_USE_IDLE_ENA
 */
#define  VTSS_F_QSYS_CALCFG_CAL_CTRL_CAL_VD_USE_IDLE_ENA(x)  VTSS_ENCODE_BITFIELD(!!(x),0,1)
#define  VTSS_M_QSYS_CALCFG_CAL_CTRL_CAL_VD_USE_IDLE_ENA  VTSS_BIT(0)
#define  VTSS_X_QSYS_CALCFG_CAL_CTRL_CAL_VD_USE_IDLE_ENA(x)  VTSS_EXTRACT_BITFIELD(x,0,1)

/**
 * Register Group: \a QSYS:RAM_CTRL
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
 * Register: \a QSYS:RAM_CTRL:RAM_INIT
 */
#define VTSS_QSYS_RAM_CTRL_RAM_INIT          VTSS_IOREG(VTSS_TO_QSYS,0x4fb)

/** 
 * \brief
 * Initialize core memories. Field is automatically cleared when operation
 * is complete ( approx. 40 us).
 *
 * \details 
 * Field: ::VTSS_QSYS_RAM_CTRL_RAM_INIT . RAM_INIT
 */
#define  VTSS_F_QSYS_RAM_CTRL_RAM_INIT_RAM_INIT(x)  VTSS_ENCODE_BITFIELD(!!(x),1,1)
#define  VTSS_M_QSYS_RAM_CTRL_RAM_INIT_RAM_INIT  VTSS_BIT(1)
#define  VTSS_X_QSYS_RAM_CTRL_RAM_INIT_RAM_INIT(x)  VTSS_EXTRACT_BITFIELD(x,1,1)

/** 
 * \brief
 * Core memory controllers are enabled when this field is set.
 *
 * \details 
 * Field: ::VTSS_QSYS_RAM_CTRL_RAM_INIT . RAM_ENA
 */
#define  VTSS_F_QSYS_RAM_CTRL_RAM_INIT_RAM_ENA(x)  VTSS_ENCODE_BITFIELD(!!(x),0,1)
#define  VTSS_M_QSYS_RAM_CTRL_RAM_INIT_RAM_ENA  VTSS_BIT(0)
#define  VTSS_X_QSYS_RAM_CTRL_RAM_INIT_RAM_ENA(x)  VTSS_EXTRACT_BITFIELD(x,0,1)

/**
 * Register Group: \a QSYS:COREMEM
 *
 * Access core memory
 */


/** 
 * \brief Address selection
 *
 * \details
 * Register: \a QSYS:COREMEM:CM_ADDR
 */
#define VTSS_QSYS_COREMEM_CM_ADDR            VTSS_IOREG(VTSS_TO_QSYS,0x400)

/** 
 * \brief
 * Refer to cmid.xls in the AS1000, misc_docs folder.
 *
 * \details 
 * Field: ::VTSS_QSYS_COREMEM_CM_ADDR . CM_ID
 */
#define  VTSS_F_QSYS_COREMEM_CM_ADDR_CM_ID(x)  VTSS_ENCODE_BITFIELD(x,22,8)
#define  VTSS_M_QSYS_COREMEM_CM_ADDR_CM_ID     VTSS_ENCODE_BITMASK(22,8)
#define  VTSS_X_QSYS_COREMEM_CM_ADDR_CM_ID(x)  VTSS_EXTRACT_BITFIELD(x,22,8)

/** 
 * \brief
 * Address selection within selected core memory (CMID register). Address
 * is automatically advanced at every data access.
 *
 * \details 
 * Field: ::VTSS_QSYS_COREMEM_CM_ADDR . CM_ADDR
 */
#define  VTSS_F_QSYS_COREMEM_CM_ADDR_CM_ADDR(x)  VTSS_ENCODE_BITFIELD(x,0,22)
#define  VTSS_M_QSYS_COREMEM_CM_ADDR_CM_ADDR     VTSS_ENCODE_BITMASK(0,22)
#define  VTSS_X_QSYS_COREMEM_CM_ADDR_CM_ADDR(x)  VTSS_EXTRACT_BITFIELD(x,0,22)


/** 
 * \brief Data register for core memory access.
 *
 * \details
 * Register: \a QSYS:COREMEM:CM_DATA
 */
#define VTSS_QSYS_COREMEM_CM_DATA            VTSS_IOREG(VTSS_TO_QSYS,0x401)


#endif /* _VTSS_JAGUAR2_REGS_QSYS_H_ */
