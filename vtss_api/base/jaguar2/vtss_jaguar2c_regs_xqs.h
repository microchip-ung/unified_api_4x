#ifndef _VTSS_JAGUAR2C_REGS_XQS_H_
#define _VTSS_JAGUAR2C_REGS_XQS_H_

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

#include "vtss_jaguar2c_regs_common.h"

/*********************************************************************** 
 *
 * Target: \a XQS
 *
 * Queue Transfer Configuration
 *
 ***********************************************************************/

/**
 * Register Group: \a XQS:SYSTEM
 *
 * Switch configuration
 */


/** 
 * \brief Statistics configuration
 *
 * \details
 * Register: \a XQS:SYSTEM:STAT_CNT_CFG
 */
#define VTSS_XQS_SYSTEM_STAT_CNT_CFG         VTSS_IOREG(VTSS_TO_XQS,0x518)

/** 
 * \brief
 * When set, a frame discarded due to lack of resources is counted on the
 * egress port instead of the ingress. Side effect is a slower processing
 * of multiple drops on the same frame, causing potential head-of-line
 * blocking.
 *
 * \details 
 * Field: ::VTSS_XQS_SYSTEM_STAT_CNT_CFG . DROP_COUNT_EGRESS
 */
#define  VTSS_F_XQS_SYSTEM_STAT_CNT_CFG_DROP_COUNT_EGRESS(x)  VTSS_ENCODE_BITFIELD(!!(x),0,1)
#define  VTSS_M_XQS_SYSTEM_STAT_CNT_CFG_DROP_COUNT_EGRESS  VTSS_BIT(0)
#define  VTSS_X_XQS_SYSTEM_STAT_CNT_CFG_DROP_COUNT_EGRESS(x)  VTSS_EXTRACT_BITFIELD(x,0,1)


/** 
 * \brief Index into large tables
 *
 * \details
 * Register: \a XQS:SYSTEM:MAP_CFG_CFG
 */
#define VTSS_XQS_SYSTEM_MAP_CFG_CFG          VTSS_IOREG(VTSS_TO_XQS,0x519)

/** 
 * \brief
 * This value is used for indexing into the QMAP_QOS_TBL, QMAP_SE_TBL, and
 * QLIMIT_QUEUE tables.
 *
 * \details 
 * Field: ::VTSS_XQS_SYSTEM_MAP_CFG_CFG . MAP_CFG_CFG
 */
#define  VTSS_F_XQS_SYSTEM_MAP_CFG_CFG_MAP_CFG_CFG(x)  (x)
#define  VTSS_M_XQS_SYSTEM_MAP_CFG_CFG_MAP_CFG_CFG     0xffffffff
#define  VTSS_X_XQS_SYSTEM_MAP_CFG_CFG_MAP_CFG_CFG(x)  (x)


/** 
 * \brief Forwarder mischeleaneous configuration
 *
 * \details
 * Register: \a XQS:SYSTEM:FWD_CTRL
 */
#define VTSS_XQS_SYSTEM_FWD_CTRL             VTSS_IOREG(VTSS_TO_XQS,0x51a)

/** 
 * \brief
 * The maximum number of clock cycles between guaranteed CSR access to
 * res_stat counters.
 *
 * \details 
 * Field: ::VTSS_XQS_SYSTEM_FWD_CTRL . FWD_IDLE_CNT
 */
#define  VTSS_F_XQS_SYSTEM_FWD_CTRL_FWD_IDLE_CNT(x)  VTSS_ENCODE_BITFIELD(x,0,13)
#define  VTSS_M_XQS_SYSTEM_FWD_CTRL_FWD_IDLE_CNT     VTSS_ENCODE_BITMASK(0,13)
#define  VTSS_X_XQS_SYSTEM_FWD_CTRL_FWD_IDLE_CNT(x)  VTSS_EXTRACT_BITFIELD(x,0,13)


/** 
 * \brief QS drop events per port and copy type
 *
 * \details
 * Register: \a XQS:SYSTEM:FWD_DROP_EVENTS
 *
 * @param ri Replicator: x_GAZ_CORE_PORT_CNT (??), 0-56
 */
#define VTSS_XQS_SYSTEM_FWD_DROP_EVENTS(ri)  VTSS_IOREG(VTSS_TO_XQS,0x51b + (ri))

/** 
 * \brief
 * A switched frame copy was canceled to one or more egress ports.
 *
 * \details 
 * Field: ::VTSS_XQS_SYSTEM_FWD_DROP_EVENTS . FWD_DROP_NORM_STICKY
 */
#define  VTSS_F_XQS_SYSTEM_FWD_DROP_EVENTS_FWD_DROP_NORM_STICKY(x)  VTSS_ENCODE_BITFIELD(!!(x),4,1)
#define  VTSS_M_XQS_SYSTEM_FWD_DROP_EVENTS_FWD_DROP_NORM_STICKY  VTSS_BIT(4)
#define  VTSS_X_XQS_SYSTEM_FWD_DROP_EVENTS_FWD_DROP_NORM_STICKY(x)  VTSS_EXTRACT_BITFIELD(x,4,1)

/** 
 * \brief
 * A CPU directed frame copy was canceled.
 *
 * \details 
 * Field: ::VTSS_XQS_SYSTEM_FWD_DROP_EVENTS . FWD_DROP_CPU_STICKY
 */
#define  VTSS_F_XQS_SYSTEM_FWD_DROP_EVENTS_FWD_DROP_CPU_STICKY(x)  VTSS_ENCODE_BITFIELD(!!(x),3,1)
#define  VTSS_M_XQS_SYSTEM_FWD_DROP_EVENTS_FWD_DROP_CPU_STICKY  VTSS_BIT(3)
#define  VTSS_X_XQS_SYSTEM_FWD_DROP_EVENTS_FWD_DROP_CPU_STICKY(x)  VTSS_EXTRACT_BITFIELD(x,3,1)

/** 
 * \brief
 * A learnall frame copy was canceled.
 *
 * \details 
 * Field: ::VTSS_XQS_SYSTEM_FWD_DROP_EVENTS . FWD_DROP_LEARN_STICKY
 */
#define  VTSS_F_XQS_SYSTEM_FWD_DROP_EVENTS_FWD_DROP_LEARN_STICKY(x)  VTSS_ENCODE_BITFIELD(!!(x),2,1)
#define  VTSS_M_XQS_SYSTEM_FWD_DROP_EVENTS_FWD_DROP_LEARN_STICKY  VTSS_BIT(2)
#define  VTSS_X_XQS_SYSTEM_FWD_DROP_EVENTS_FWD_DROP_LEARN_STICKY(x)  VTSS_EXTRACT_BITFIELD(x,2,1)

/** 
 * \brief
 * A mirror copy was canceled.
 *
 * \details 
 * Field: ::VTSS_XQS_SYSTEM_FWD_DROP_EVENTS . FWD_DROP_MIRR_STICKY
 */
#define  VTSS_F_XQS_SYSTEM_FWD_DROP_EVENTS_FWD_DROP_MIRR_STICKY(x)  VTSS_ENCODE_BITFIELD(!!(x),1,1)
#define  VTSS_M_XQS_SYSTEM_FWD_DROP_EVENTS_FWD_DROP_MIRR_STICKY  VTSS_BIT(1)
#define  VTSS_X_XQS_SYSTEM_FWD_DROP_EVENTS_FWD_DROP_MIRR_STICKY(x)  VTSS_EXTRACT_BITFIELD(x,1,1)

/** 
 * \brief
 * A frame copy was discarded due to a queuemapping violation.
 *
 * \details 
 * Field: ::VTSS_XQS_SYSTEM_FWD_DROP_EVENTS . FWD_DROP_QMAP_STICKY
 */
#define  VTSS_F_XQS_SYSTEM_FWD_DROP_EVENTS_FWD_DROP_QMAP_STICKY(x)  VTSS_ENCODE_BITFIELD(!!(x),0,1)
#define  VTSS_M_XQS_SYSTEM_FWD_DROP_EVENTS_FWD_DROP_QMAP_STICKY  VTSS_BIT(0)
#define  VTSS_X_XQS_SYSTEM_FWD_DROP_EVENTS_FWD_DROP_QMAP_STICKY(x)  VTSS_EXTRACT_BITFIELD(x,0,1)


/** 
 * \brief Counters showing forwarding runs. Debugging purposes only
 *
 * \details
 * Three counters exists in the forwarder. Replication 0 counts each
 * executed queue head processing. Replication 1 counts each occurance of a
 * delayed processing due to egress queue system pressure, and replication
 * 2 counts occurances of delayed process due to statistics event pressure.
 *
 * Register: \a XQS:SYSTEM:FWD_STAT_CNT
 *
 * @param ri Register: FWD_STAT_CNT (??), 0-2
 */
#define VTSS_XQS_SYSTEM_FWD_STAT_CNT(ri)     VTSS_IOREG(VTSS_TO_XQS,0x554 + (ri))

/** 
 * \brief
 * Counts number of forwarding events since chip reset.
 *
 * \details 
 * Field: ::VTSS_XQS_SYSTEM_FWD_STAT_CNT . FWD_STAT_CNT
 */
#define  VTSS_F_XQS_SYSTEM_FWD_STAT_CNT_FWD_STAT_CNT(x)  (x)
#define  VTSS_M_XQS_SYSTEM_FWD_STAT_CNT_FWD_STAT_CNT     0xffffffff
#define  VTSS_X_XQS_SYSTEM_FWD_STAT_CNT_FWD_STAT_CNT(x)  (x)


/** 
 * \brief Counter for number of frames discarded towards the cpu
 *
 * \details
 * Register: \a XQS:SYSTEM:FWD_CPU_DROP_CNT
 */
#define VTSS_XQS_SYSTEM_FWD_CPU_DROP_CNT     VTSS_IOREG(VTSS_TO_XQS,0x557)

/** 
 * \brief
 * Counts number of frames discarded towards the cpu, since queue system
 * reset.
 *
 * \details 
 * Field: ::VTSS_XQS_SYSTEM_FWD_CPU_DROP_CNT . FWD_CPU_DROP_CNT
 */
#define  VTSS_F_XQS_SYSTEM_FWD_CPU_DROP_CNT_FWD_CPU_DROP_CNT(x)  (x)
#define  VTSS_M_XQS_SYSTEM_FWD_CPU_DROP_CNT_FWD_CPU_DROP_CNT     0xffffffff
#define  VTSS_X_XQS_SYSTEM_FWD_CPU_DROP_CNT_FWD_CPU_DROP_CNT(x)  (x)


/** 
 * \brief Basic port mode for scheduling hierarchy
 *
 * \details
 * All ports have their own scheduling hierarchy defined, consisting of a
 * part for nonservice frames defined as frames having queing group
 * classified to 0, and a part for the rest. The two parts can be 
 * identical if needed, in which case the modes for service and non service
 * must have the same setting.
 *
 * Register: \a XQS:SYSTEM:QMAP_PORT_MODE
 *
 * @param ri Replicator: x_GAZ_CORE_ETHPORT_CNT (??), 0-52
 */
#define VTSS_XQS_SYSTEM_QMAP_PORT_MODE(ri)   VTSS_IOREG(VTSS_TO_XQS,0x558 + (ri))

/** 
 * \brief
 * Scheduling mode for frames classified to queing group /=0 (service
 * frames).
 * 
 * In NORMAL mode, the queue number is looked up in the map tables at index
 * 1024+qos_value.
 * 
 * In GROUP mode, a scheduling element is assigned per queueing group, with
 * 8 queues attached. The queue number is looked up in the tables at index
 * qgrp.
 * 
 * In MBH mode, the qos is looked up at index qgrp, and the scheduling
 * index is looked up at index "(grp and not 0xf) + qos_value".
 * 
 * A minimum hierachy also exist. It corresponds to GROUP mode, but the
 * qgrp is regarded zero in the mapping. 
 *
 * \details 
 * 0: Normal mode
 * 1: Hier mode
 * 2: Mbh mode
 * 3: Mini mode

 *
 * Field: ::VTSS_XQS_SYSTEM_QMAP_PORT_MODE . QMAP_MODE_SERVICE
 */
#define  VTSS_F_XQS_SYSTEM_QMAP_PORT_MODE_QMAP_MODE_SERVICE(x)  VTSS_ENCODE_BITFIELD(x,2,2)
#define  VTSS_M_XQS_SYSTEM_QMAP_PORT_MODE_QMAP_MODE_SERVICE     VTSS_ENCODE_BITMASK(2,2)
#define  VTSS_X_XQS_SYSTEM_QMAP_PORT_MODE_QMAP_MODE_SERVICE(x)  VTSS_EXTRACT_BITFIELD(x,2,2)

/** 
 * \brief
 * Same function as for QMAP_MODE_SERVICE, except this mode is for qgrp=0
 * frames.
 *
 * \details 
 * Field: ::VTSS_XQS_SYSTEM_QMAP_PORT_MODE . QMAP_MODE_NONSERVICE
 */
#define  VTSS_F_XQS_SYSTEM_QMAP_PORT_MODE_QMAP_MODE_NONSERVICE(x)  VTSS_ENCODE_BITFIELD(x,0,2)
#define  VTSS_M_XQS_SYSTEM_QMAP_PORT_MODE_QMAP_MODE_NONSERVICE     VTSS_ENCODE_BITMASK(0,2)
#define  VTSS_X_XQS_SYSTEM_QMAP_PORT_MODE_QMAP_MODE_NONSERVICE(x)  VTSS_EXTRACT_BITFIELD(x,0,2)


/** 
 * \brief Statistics configuration
 *
 * \details
 * Register: \a XQS:SYSTEM:STAT_CFG
 */
#define VTSS_XQS_SYSTEM_STAT_CFG             VTSS_IOREG(VTSS_TO_XQS,0x58d)

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
 * Field: ::VTSS_XQS_SYSTEM_STAT_CFG . STAT_CLEAR_SHOT
 */
#define  VTSS_F_XQS_SYSTEM_STAT_CFG_STAT_CLEAR_SHOT(x)  VTSS_ENCODE_BITFIELD(x,18,4)
#define  VTSS_M_XQS_SYSTEM_STAT_CFG_STAT_CLEAR_SHOT     VTSS_ENCODE_BITMASK(18,4)
#define  VTSS_X_XQS_SYSTEM_STAT_CFG_STAT_CLEAR_SHOT(x)  VTSS_EXTRACT_BITFIELD(x,18,4)

/** 
 * \brief
 * Selects the port or service for which counters can be accessed using
 * QSYS:STAT.
 * 
 * Also used to select the port or service index for which to clear
 * statistics counters, ref. STAT_CFG.STAT_CLEAR_SHOT.

 *
 * \details 
 * Field: ::VTSS_XQS_SYSTEM_STAT_CFG . STAT_VIEW
 */
#define  VTSS_F_XQS_SYSTEM_STAT_CFG_STAT_VIEW(x)  VTSS_ENCODE_BITFIELD(x,5,13)
#define  VTSS_M_XQS_SYSTEM_STAT_CFG_STAT_VIEW     VTSS_ENCODE_BITMASK(5,13)
#define  VTSS_X_XQS_SYSTEM_STAT_CFG_STAT_VIEW(x)  VTSS_EXTRACT_BITFIELD(x,5,13)

/** 
 * \brief
 * Set to enable use of all of the service counter memory for packet
 * counting.

 *
 * \details 
 * Field: ::VTSS_XQS_SYSTEM_STAT_CFG . STAT_SRV_PKT_ONLY
 */
#define  VTSS_F_XQS_SYSTEM_STAT_CFG_STAT_SRV_PKT_ONLY(x)  VTSS_ENCODE_BITFIELD(!!(x),4,1)
#define  VTSS_M_XQS_SYSTEM_STAT_CFG_STAT_SRV_PKT_ONLY  VTSS_BIT(4)
#define  VTSS_X_XQS_SYSTEM_STAT_CFG_STAT_SRV_PKT_ONLY(x)  VTSS_EXTRACT_BITFIELD(x,4,1)

/** 
 * \brief
 * Counters are by default wrapping when exceeding their maximum value, and
 * software must thus do a subtraction with the previous readen value to
 * see how much the total count has changed.
 * If wrapping is disabled, the counters will clear on read, and saturate
 * at their maximum value. Software can thus detect that a counter overflow
 * has happened, and do not need storing the previous read values.
 * The configuration exists replicated per statistics group as the
 * STAT_CLEAR_SHOT describes.
 *
 * \details 
 * Field: ::VTSS_XQS_SYSTEM_STAT_CFG . STAT_WRAP_DIS
 */
#define  VTSS_F_XQS_SYSTEM_STAT_CFG_STAT_WRAP_DIS(x)  VTSS_ENCODE_BITFIELD(x,0,4)
#define  VTSS_M_XQS_SYSTEM_STAT_CFG_STAT_WRAP_DIS     VTSS_ENCODE_BITMASK(0,4)
#define  VTSS_X_XQS_SYSTEM_STAT_CFG_STAT_WRAP_DIS(x)  VTSS_EXTRACT_BITFIELD(x,0,4)

/**
 * Register Group: \a XQS:QMAP_VPORT_TBL
 *
 * Mapping into virtual ports
 */


/** 
 * \brief Mapping into virtual ports
 *
 * \details
 * Register: \a XQS:QMAP_VPORT_TBL:QMAP_VPORT_TBL
 *
 * @param gi Register: QMAP_VPORT_TBL (??), 0-3
 * @param ri Replicator: x_GAZ_CORE_ETHPORT_CNT (??), 0-52
 */
#define VTSS_XQS_QMAP_VPORT_TBL_QMAP_VPORT_TBL(gi,ri)  VTSS_IOREG_IX(VTSS_TO_XQS,0x400,gi,64,ri,0)

/** 
 * \brief
 * Maps the indexed (QGRP,DPORT) set into a virtual port number. This
 * number is used by the queue mapping system when determining target queue
 * through QMAP_SE_TBL and QMAP_QOS_TBL.
 *
 * \details 
 * Field: ::VTSS_XQS_QMAP_VPORT_TBL_QMAP_VPORT_TBL . QMAP_VPORT_VAL
 */
#define  VTSS_F_XQS_QMAP_VPORT_TBL_QMAP_VPORT_TBL_QMAP_VPORT_VAL(x)  VTSS_ENCODE_BITFIELD(x,0,12)
#define  VTSS_M_XQS_QMAP_VPORT_TBL_QMAP_VPORT_TBL_QMAP_VPORT_VAL     VTSS_ENCODE_BITMASK(0,12)
#define  VTSS_X_XQS_QMAP_VPORT_TBL_QMAP_VPORT_TBL_QMAP_VPORT_VAL(x)  VTSS_EXTRACT_BITFIELD(x,0,12)

/**
 * Register Group: \a XQS:QMAP_SE_TBL
 *
 * Select scheduling modes
 */


/** 
 * \brief Per port mapping of qgrp
 *
 * \details
 * Register: \a XQS:QMAP_SE_TBL:QMAP_SE_TBL
 *
 * @param gi Register: QMAP_SE_TBL (??), 0-3
 */
#define VTSS_XQS_QMAP_SE_TBL_QMAP_SE_TBL(gi)  VTSS_IOREG_IX(VTSS_TO_XQS,0x510,gi,1,0,0)

/** 
 * \brief
 * Scheduling element to use for frames going to the specific port with the
 * specific lookup index
 *
 * \details 
 * Field: ::VTSS_XQS_QMAP_SE_TBL_QMAP_SE_TBL . QMAP_SE_VAL
 */
#define  VTSS_F_XQS_QMAP_SE_TBL_QMAP_SE_TBL_QMAP_SE_VAL(x)  VTSS_ENCODE_BITFIELD(x,0,12)
#define  VTSS_M_XQS_QMAP_SE_TBL_QMAP_SE_TBL_QMAP_SE_VAL     VTSS_ENCODE_BITMASK(0,12)
#define  VTSS_X_XQS_QMAP_SE_TBL_QMAP_SE_TBL_QMAP_SE_VAL(x)  VTSS_EXTRACT_BITFIELD(x,0,12)

/**
 * Register Group: \a XQS:QMAP_QOS_TBL
 *
 * Select scheduling modes
 */


/** 
 * \brief Per port mapping of qgrp
 *
 * \details
 * Register: \a XQS:QMAP_QOS_TBL:QMAP_QOS_TBL
 *
 * @param gi Register: QMAP_QOS_TBL (??), 0-3
 */
#define VTSS_XQS_QMAP_QOS_TBL_QMAP_QOS_TBL(gi)  VTSS_IOREG_IX(VTSS_TO_XQS,0x500,gi,2,0,0)

/** 
 * \brief
 * Configures which classified parameter to use when selecting scheduling
 * input.

 *
 * \details 
 * 0: Use IPRIO as input selector (SRCP for normal queue mode)
 * 1: Use COSID as input selector
 * 2: Use TC as input selector
 * 3: Use PCP as input selector

 *
 * Field: ::VTSS_XQS_QMAP_QOS_TBL_QMAP_QOS_TBL . QMAP_QOS_SEL
 */
#define  VTSS_F_XQS_QMAP_QOS_TBL_QMAP_QOS_TBL_QMAP_QOS_SEL(x)  VTSS_ENCODE_BITFIELD(x,0,2)
#define  VTSS_M_XQS_QMAP_QOS_TBL_QMAP_QOS_TBL_QMAP_QOS_SEL     VTSS_ENCODE_BITMASK(0,2)
#define  VTSS_X_XQS_QMAP_QOS_TBL_QMAP_QOS_TBL_QMAP_QOS_SEL(x)  VTSS_EXTRACT_BITFIELD(x,0,2)


/** 
 * \brief Per egress port mapping of qgrp to drop stat index
 *
 * \details
 * Register: \a XQS:QMAP_QOS_TBL:DROP_STAT_CTRL
 *
 * @param gi Register: QMAP_QOS_TBL (??), 0-3
 */
#define VTSS_XQS_QMAP_QOS_TBL_DROP_STAT_CTRL(gi)  VTSS_IOREG_IX(VTSS_TO_XQS,0x500,gi,2,0,1)

/** 
 * \brief
 * Drop counter set base address.
 *
 * \details 
 * Field: ::VTSS_XQS_QMAP_QOS_TBL_DROP_STAT_CTRL . DROP_STAT_BASE_ADDR
 */
#define  VTSS_F_XQS_QMAP_QOS_TBL_DROP_STAT_CTRL_DROP_STAT_BASE_ADDR(x)  VTSS_ENCODE_BITFIELD(x,6,11)
#define  VTSS_M_XQS_QMAP_QOS_TBL_DROP_STAT_CTRL_DROP_STAT_BASE_ADDR     VTSS_ENCODE_BITMASK(6,11)
#define  VTSS_X_XQS_QMAP_QOS_TBL_DROP_STAT_CTRL_DROP_STAT_BASE_ADDR(x)  VTSS_EXTRACT_BITFIELD(x,6,11)

/** 
 * \brief
 * Configures which OAM is counted in the from stat counter.
 * 
 * Ref: ANA_L2:ISDX:MISC_CFG.QGRP_OAM_TYPE
 *
 * \details 
 * bit0: Enable / disable drop count of EVC MEP OAM
 * bit1: Enable / disable drop count of OVC / PW MEP OAM
 * bit2: Enable / disable drop count of DOWN MEP OAM
 *
 * Field: ::VTSS_XQS_QMAP_QOS_TBL_DROP_STAT_CTRL . DROP_STAT_OAM_CNT_SEL
 */
#define  VTSS_F_XQS_QMAP_QOS_TBL_DROP_STAT_CTRL_DROP_STAT_OAM_CNT_SEL(x)  VTSS_ENCODE_BITFIELD(x,3,3)
#define  VTSS_M_XQS_QMAP_QOS_TBL_DROP_STAT_CTRL_DROP_STAT_OAM_CNT_SEL     VTSS_ENCODE_BITMASK(3,3)
#define  VTSS_X_XQS_QMAP_QOS_TBL_DROP_STAT_CTRL_DROP_STAT_OAM_CNT_SEL(x)  VTSS_EXTRACT_BITFIELD(x,3,3)

/** 
 * \brief
 * Configures which classified parameter to use when selecting drop stat
 * index.

 *
 * \details 
 * 0: Use IPRIO as COS input selector
 * 1: Use COSID as COS input selector
 * 2: Use TC as COS input selector
 * 3: Use PCP as COS input selector

 *
 * Field: ::VTSS_XQS_QMAP_QOS_TBL_DROP_STAT_CTRL . DROP_STAT_COS_SEL
 */
#define  VTSS_F_XQS_QMAP_QOS_TBL_DROP_STAT_CTRL_DROP_STAT_COS_SEL(x)  VTSS_ENCODE_BITFIELD(x,1,2)
#define  VTSS_M_XQS_QMAP_QOS_TBL_DROP_STAT_CTRL_DROP_STAT_COS_SEL     VTSS_ENCODE_BITMASK(1,2)
#define  VTSS_X_XQS_QMAP_QOS_TBL_DROP_STAT_CTRL_DROP_STAT_COS_SEL(x)  VTSS_EXTRACT_BITFIELD(x,1,2)

/** 
 * \brief
 * Select the number of counters for this VPORT.
 *
 * \details 
 * 0: 4 counters included. Qos x and x+4 shares counter
 * 1: 8 counters included, and two counter sets will be used
 *
 * Field: ::VTSS_XQS_QMAP_QOS_TBL_DROP_STAT_CTRL . DROP_STAT_COS8_ENA
 */
#define  VTSS_F_XQS_QMAP_QOS_TBL_DROP_STAT_CTRL_DROP_STAT_COS8_ENA(x)  VTSS_ENCODE_BITFIELD(!!(x),0,1)
#define  VTSS_M_XQS_QMAP_QOS_TBL_DROP_STAT_CTRL_DROP_STAT_COS8_ENA  VTSS_BIT(0)
#define  VTSS_X_XQS_QMAP_QOS_TBL_DROP_STAT_CTRL_DROP_STAT_COS8_ENA(x)  VTSS_EXTRACT_BITFIELD(x,0,1)

/**
 * Register Group: \a XQS:QLIMIT_QUEUE
 *
 * Queue Size Table
 */


/** 
 * \brief Current queue size
 *
 * \details
 * Register: \a XQS:QLIMIT_QUEUE:QUEUE_SIZE
 *
 * @param gi Register: QLIMIT_QUEUE (??), 0-3
 */
#define VTSS_XQS_QLIMIT_QUEUE_QUEUE_SIZE(gi)  VTSS_IOREG_IX(VTSS_TO_XQS,0x514,gi,1,0,0)

/** 
 * \brief
 * Current size of queue used for queue size limitation
 *
 * \details 
 * Field: ::VTSS_XQS_QLIMIT_QUEUE_QUEUE_SIZE . QUEUE_SIZE
 */
#define  VTSS_F_XQS_QLIMIT_QUEUE_QUEUE_SIZE_QUEUE_SIZE(x)  VTSS_ENCODE_BITFIELD(x,1,11)
#define  VTSS_M_XQS_QLIMIT_QUEUE_QUEUE_SIZE_QUEUE_SIZE     VTSS_ENCODE_BITMASK(1,11)
#define  VTSS_X_XQS_QLIMIT_QUEUE_QUEUE_SIZE_QUEUE_SIZE(x)  VTSS_EXTRACT_BITFIELD(x,1,11)

/** 
 * \brief
 * This queue was have had one or more discards due to the queue limitation
 * algorithm
 *
 * \details 
 * Field: ::VTSS_XQS_QLIMIT_QUEUE_QUEUE_SIZE . QUEUE_KILLED
 */
#define  VTSS_F_XQS_QLIMIT_QUEUE_QUEUE_SIZE_QUEUE_KILLED(x)  VTSS_ENCODE_BITFIELD(!!(x),0,1)
#define  VTSS_M_XQS_QLIMIT_QUEUE_QUEUE_SIZE_QUEUE_KILLED  VTSS_BIT(0)
#define  VTSS_X_XQS_QLIMIT_QUEUE_QUEUE_SIZE_QUEUE_KILLED(x)  VTSS_EXTRACT_BITFIELD(x,0,1)

/**
 * Register Group: \a XQS:QLIMIT_SE
 *
 * SE Size Table
 */


/** 
 * \brief Current use for the scheduling element
 *
 * \details
 * Register: \a XQS:QLIMIT_SE:QLIMIT_SE_USE
 *
 * @param gi Register: QLIMIT_SE (??), 0-3
 */
#define VTSS_XQS_QLIMIT_SE_QLIMIT_SE_USE(gi)  VTSS_IOREG_IX(VTSS_TO_XQS,0x508,gi,2,0,0)

/** 
 * \brief
 * Total use for all queues on the schedulling element
 *
 * \details 
 * Field: ::VTSS_XQS_QLIMIT_SE_QLIMIT_SE_USE . QLIMIT_SE_USE
 */
#define  VTSS_F_XQS_QLIMIT_SE_QLIMIT_SE_USE_QLIMIT_SE_USE(x)  VTSS_ENCODE_BITFIELD(x,0,15)
#define  VTSS_M_XQS_QLIMIT_SE_QLIMIT_SE_USE_QLIMIT_SE_USE     VTSS_ENCODE_BITMASK(0,15)
#define  VTSS_X_XQS_QLIMIT_SE_QLIMIT_SE_USE_QLIMIT_SE_USE(x)  VTSS_EXTRACT_BITFIELD(x,0,15)


/** 
 * \brief Current congestion count for the scheduling element
 *
 * \details
 * Register: \a XQS:QLIMIT_SE:QLIMIT_CONG_CNT
 *
 * @param gi Register: QLIMIT_SE (??), 0-3
 */
#define VTSS_XQS_QLIMIT_SE_QLIMIT_CONG_CNT(gi)  VTSS_IOREG_IX(VTSS_TO_XQS,0x508,gi,2,0,1)

/** 
 * \brief
 * Number of congested queues on the scheduling element
 *
 * \details 
 * Field: ::VTSS_XQS_QLIMIT_SE_QLIMIT_CONG_CNT . QLIMIT_CONG_CNT
 */
#define  VTSS_F_XQS_QLIMIT_SE_QLIMIT_CONG_CNT_QLIMIT_CONG_CNT(x)  VTSS_ENCODE_BITFIELD(x,0,6)
#define  VTSS_M_XQS_QLIMIT_SE_QLIMIT_CONG_CNT_QLIMIT_CONG_CNT     VTSS_ENCODE_BITMASK(0,6)
#define  VTSS_X_XQS_QLIMIT_SE_QLIMIT_CONG_CNT_QLIMIT_CONG_CNT(x)  VTSS_EXTRACT_BITFIELD(x,0,6)

/**
 * Register Group: \a XQS:QLIMIT_PORT
 *
 * Queue Limitation Configuration
 */


/** 
 * \brief Queue Limitation Configuration
 *
 * \details
 * Register: \a XQS:QLIMIT_PORT:QLIMIT_PORT_CFG
 *
 * @param gi Replicator: x_GAZ_CORE_PORT_CNT (??), 0-56
 */
#define VTSS_XQS_QLIMIT_PORT_QLIMIT_PORT_CFG(gi)  VTSS_IOREG_IX(VTSS_TO_XQS,0x58e,gi,2,0,0)

/** 
 * \brief
 * Index of shared resource to use
 *
 * \details 
 * Field: ::VTSS_XQS_QLIMIT_PORT_QLIMIT_PORT_CFG . QLIMIT_SHR_VAL
 */
#define  VTSS_F_XQS_QLIMIT_PORT_QLIMIT_PORT_CFG_QLIMIT_SHR_VAL(x)  VTSS_ENCODE_BITFIELD(x,1,2)
#define  VTSS_M_XQS_QLIMIT_PORT_QLIMIT_PORT_CFG_QLIMIT_SHR_VAL     VTSS_ENCODE_BITMASK(1,2)
#define  VTSS_X_XQS_QLIMIT_PORT_QLIMIT_PORT_CFG_QLIMIT_SHR_VAL(x)  VTSS_EXTRACT_BITFIELD(x,1,2)

/** 
 * \brief
 * Queue limitation is for this port used in MAX mode, setting upper limits
 * for a queue. Otherwise a minimum guarantee is given by the calculated
 * queue maximum size
 *
 * \details 
 * Field: ::VTSS_XQS_QLIMIT_PORT_QLIMIT_PORT_CFG . QLIMIT_MAX_MODE_ENA
 */
#define  VTSS_F_XQS_QLIMIT_PORT_QLIMIT_PORT_CFG_QLIMIT_MAX_MODE_ENA(x)  VTSS_ENCODE_BITFIELD(!!(x),0,1)
#define  VTSS_M_XQS_QLIMIT_PORT_QLIMIT_PORT_CFG_QLIMIT_MAX_MODE_ENA  VTSS_BIT(0)
#define  VTSS_X_XQS_QLIMIT_PORT_QLIMIT_PORT_CFG_QLIMIT_MAX_MODE_ENA(x)  VTSS_EXTRACT_BITFIELD(x,0,1)


/** 
 * \brief Queue Limitation Configuration
 *
 * \details
 * Disabling queue limitation per port.
 *
 * Register: \a XQS:QLIMIT_PORT:QLIMIT_DIS_CFG
 *
 * @param gi Replicator: x_GAZ_CORE_PORT_CNT (??), 0-56
 */
#define VTSS_XQS_QLIMIT_PORT_QLIMIT_DIS_CFG(gi)  VTSS_IOREG_IX(VTSS_TO_XQS,0x58e,gi,2,0,1)

/** 
 * \brief
 * Disable queue limitation for the port. If either queue limitation is
 * disabled for the source port, or for the destination port, the queue
 * limitation algorithm is bypassed.
 *
 * \details 
 * Field: ::VTSS_XQS_QLIMIT_PORT_QLIMIT_DIS_CFG . QLIMIT_IGR_DIS
 */
#define  VTSS_F_XQS_QLIMIT_PORT_QLIMIT_DIS_CFG_QLIMIT_IGR_DIS(x)  VTSS_ENCODE_BITFIELD(!!(x),1,1)
#define  VTSS_M_XQS_QLIMIT_PORT_QLIMIT_DIS_CFG_QLIMIT_IGR_DIS  VTSS_BIT(1)
#define  VTSS_X_XQS_QLIMIT_PORT_QLIMIT_DIS_CFG_QLIMIT_IGR_DIS(x)  VTSS_EXTRACT_BITFIELD(x,1,1)

/** 
 * \brief
 * Disable queue limitation for the port. If either queue limitation is
 * disabled for the source port, or for the destination port, the queue
 * limitation algorithm is bypassed.
 *
 * \details 
 * Field: ::VTSS_XQS_QLIMIT_PORT_QLIMIT_DIS_CFG . QLIMIT_EGR_DIS
 */
#define  VTSS_F_XQS_QLIMIT_PORT_QLIMIT_DIS_CFG_QLIMIT_EGR_DIS(x)  VTSS_ENCODE_BITFIELD(!!(x),0,1)
#define  VTSS_M_XQS_QLIMIT_PORT_QLIMIT_DIS_CFG_QLIMIT_EGR_DIS  VTSS_BIT(0)
#define  VTSS_X_XQS_QLIMIT_PORT_QLIMIT_DIS_CFG_QLIMIT_EGR_DIS(x)  VTSS_EXTRACT_BITFIELD(x,0,1)

/**
 * Register Group: \a XQS:QLIMIT_SHR
 *
 * Shared memory pool configuration
 */


/** 
 * \brief Maximum size of shared area
 *
 * \details
 * Register: \a XQS:QLIMIT_SHR:QLIMIT_SHR_TOP_CFG
 *
 * @param gi Replicator: x_GAZ_CORE_QLIM_SHR_CNT (??), 0-3
 * @param ri Register: QLIMIT_SHR_TOP_CFG (??), 0-1
 */
#define VTSS_XQS_QLIMIT_SHR_QLIMIT_SHR_TOP_CFG(gi,ri)  VTSS_IOREG_IX(VTSS_TO_XQS,0x600,gi,17,ri,0)

/** 
 * \brief
 * When total consumption of a shared area exceeds this level, all queues
 * belonging to the area start tail dropping
 *
 * \details 
 * Field: ::VTSS_XQS_QLIMIT_SHR_QLIMIT_SHR_TOP_CFG . QLIMIT_SHR_TOP
 */
#define  VTSS_F_XQS_QLIMIT_SHR_QLIMIT_SHR_TOP_CFG_QLIMIT_SHR_TOP(x)  VTSS_ENCODE_BITFIELD(x,0,15)
#define  VTSS_M_XQS_QLIMIT_SHR_QLIMIT_SHR_TOP_CFG_QLIMIT_SHR_TOP     VTSS_ENCODE_BITMASK(0,15)
#define  VTSS_X_XQS_QLIMIT_SHR_QLIMIT_SHR_TOP_CFG_QLIMIT_SHR_TOP(x)  VTSS_EXTRACT_BITFIELD(x,0,15)


/** 
 * \brief Maximum congested size of shared area
 *
 * \details
 * Register: \a XQS:QLIMIT_SHR:QLIMIT_SHR_ATOP_CFG
 *
 * @param gi Replicator: x_GAZ_CORE_QLIM_SHR_CNT (??), 0-3
 * @param ri Register: QLIMIT_SHR_ATOP_CFG (??), 0-1
 */
#define VTSS_XQS_QLIMIT_SHR_QLIMIT_SHR_ATOP_CFG(gi,ri)  VTSS_IOREG_IX(VTSS_TO_XQS,0x600,gi,17,ri,2)

/** 
 * \brief
 * When filling exceeds this level, all active queues start tail dropping.
 *
 * \details 
 * Field: ::VTSS_XQS_QLIMIT_SHR_QLIMIT_SHR_ATOP_CFG . QLIMIT_SHR_ATOP
 */
#define  VTSS_F_XQS_QLIMIT_SHR_QLIMIT_SHR_ATOP_CFG_QLIMIT_SHR_ATOP(x)  VTSS_ENCODE_BITFIELD(x,0,15)
#define  VTSS_M_XQS_QLIMIT_SHR_QLIMIT_SHR_ATOP_CFG_QLIMIT_SHR_ATOP     VTSS_ENCODE_BITMASK(0,15)
#define  VTSS_X_XQS_QLIMIT_SHR_QLIMIT_SHR_ATOP_CFG_QLIMIT_SHR_ATOP(x)  VTSS_EXTRACT_BITFIELD(x,0,15)


/** 
 * \brief Maximum congested size of shared area
 *
 * \details
 * Register: \a XQS:QLIMIT_SHR:QLIMIT_SHR_CTOP_CFG
 *
 * @param gi Replicator: x_GAZ_CORE_QLIM_SHR_CNT (??), 0-3
 * @param ri Register: QLIMIT_SHR_CTOP_CFG (??), 0-1
 */
#define VTSS_XQS_QLIMIT_SHR_QLIMIT_SHR_CTOP_CFG(gi,ri)  VTSS_IOREG_IX(VTSS_TO_XQS,0x600,gi,17,ri,4)

/** 
 * \brief
 * When filling exceeds this level, all congested queues start tail
 * dropping.
 *
 * \details 
 * Field: ::VTSS_XQS_QLIMIT_SHR_QLIMIT_SHR_CTOP_CFG . QLIMIT_SHR_CTOP
 */
#define  VTSS_F_XQS_QLIMIT_SHR_QLIMIT_SHR_CTOP_CFG_QLIMIT_SHR_CTOP(x)  VTSS_ENCODE_BITFIELD(x,0,15)
#define  VTSS_M_XQS_QLIMIT_SHR_QLIMIT_SHR_CTOP_CFG_QLIMIT_SHR_CTOP     VTSS_ENCODE_BITMASK(0,15)
#define  VTSS_X_XQS_QLIMIT_SHR_QLIMIT_SHR_CTOP_CFG_QLIMIT_SHR_CTOP(x)  VTSS_EXTRACT_BITFIELD(x,0,15)


/** 
 * \brief Maximum area use before queue limitation kicks in
 *
 * \details
 * Register: \a XQS:QLIMIT_SHR:QLIMIT_SHR_QLIM_CFG
 *
 * @param gi Replicator: x_GAZ_CORE_QLIM_SHR_CNT (??), 0-3
 * @param ri Register: QLIMIT_SHR_QLIM_CFG (??), 0-1
 */
#define VTSS_XQS_QLIMIT_SHR_QLIMIT_SHR_QLIM_CFG(gi,ri)  VTSS_IOREG_IX(VTSS_TO_XQS,0x600,gi,17,ri,6)

/** 
 * \brief
 * When filling exceeds this level, all queues are limited in size
 * depending on number of congested queues.
 *
 * \details 
 * Field: ::VTSS_XQS_QLIMIT_SHR_QLIMIT_SHR_QLIM_CFG . QLIMIT_SHR_QLIM
 */
#define  VTSS_F_XQS_QLIMIT_SHR_QLIMIT_SHR_QLIM_CFG_QLIMIT_SHR_QLIM(x)  VTSS_ENCODE_BITFIELD(x,0,15)
#define  VTSS_M_XQS_QLIMIT_SHR_QLIMIT_SHR_QLIM_CFG_QLIMIT_SHR_QLIM     VTSS_ENCODE_BITMASK(0,15)
#define  VTSS_X_XQS_QLIMIT_SHR_QLIMIT_SHR_QLIM_CFG_QLIMIT_SHR_QLIM(x)  VTSS_EXTRACT_BITFIELD(x,0,15)


/** 
 * \brief Maximum area to distribute between large SE users
 *
 * \details
 * Register: \a XQS:QLIMIT_SHR:QLIMIT_SHR_QDIV_CFG
 *
 * @param gi Replicator: x_GAZ_CORE_QLIM_SHR_CNT (??), 0-3
 * @param ri Register: QLIMIT_SHR_QDIV_CFG (??), 0-1
 */
#define VTSS_XQS_QLIMIT_SHR_QLIMIT_SHR_QDIV_CFG(gi,ri)  VTSS_IOREG_IX(VTSS_TO_XQS,0x600,gi,17,ri,8)

/** 
 * \brief
 * This amount can be shared between large SEs
 *
 * \details 
 * Field: ::VTSS_XQS_QLIMIT_SHR_QLIMIT_SHR_QDIV_CFG . QLIMIT_SHR_QDIV
 */
#define  VTSS_F_XQS_QLIMIT_SHR_QLIMIT_SHR_QDIV_CFG_QLIMIT_SHR_QDIV(x)  VTSS_ENCODE_BITFIELD(x,0,15)
#define  VTSS_M_XQS_QLIMIT_SHR_QLIMIT_SHR_QDIV_CFG_QLIMIT_SHR_QDIV     VTSS_ENCODE_BITMASK(0,15)
#define  VTSS_X_XQS_QLIMIT_SHR_QLIMIT_SHR_QDIV_CFG_QLIMIT_SHR_QDIV(x)  VTSS_EXTRACT_BITFIELD(x,0,15)


/** 
 * \brief Size of a congested queue
 *
 * \details
 * Register: \a XQS:QLIMIT_SHR:QLIMIT_QUE_CONG_CFG
 *
 * @param gi Replicator: x_GAZ_CORE_QLIM_SHR_CNT (??), 0-3
 */
#define VTSS_XQS_QLIMIT_SHR_QLIMIT_QUE_CONG_CFG(gi)  VTSS_IOREG_IX(VTSS_TO_XQS,0x600,gi,17,0,10)

/** 
 * \details 
 * Field: ::VTSS_XQS_QLIMIT_SHR_QLIMIT_QUE_CONG_CFG . QLIMIT_QUE_CONG
 */
#define  VTSS_F_XQS_QLIMIT_SHR_QLIMIT_QUE_CONG_CFG_QLIMIT_QUE_CONG(x)  VTSS_ENCODE_BITFIELD(x,0,6)
#define  VTSS_M_XQS_QLIMIT_SHR_QLIMIT_QUE_CONG_CFG_QLIMIT_QUE_CONG     VTSS_ENCODE_BITMASK(0,6)
#define  VTSS_X_XQS_QLIMIT_SHR_QLIMIT_QUE_CONG_CFG_QLIMIT_QUE_CONG(x)  VTSS_EXTRACT_BITFIELD(x,0,6)


/** 
 * \brief Size of an active queue
 *
 * \details
 * Register: \a XQS:QLIMIT_SHR:QLIMIT_QUE_ACT_CFG
 *
 * @param gi Replicator: x_GAZ_CORE_QLIM_SHR_CNT (??), 0-3
 */
#define VTSS_XQS_QLIMIT_SHR_QLIMIT_QUE_ACT_CFG(gi)  VTSS_IOREG_IX(VTSS_TO_XQS,0x600,gi,17,0,11)

/** 
 * \details 
 * Field: ::VTSS_XQS_QLIMIT_SHR_QLIMIT_QUE_ACT_CFG . QLIMIT_QUE_ACT
 */
#define  VTSS_F_XQS_QLIMIT_SHR_QLIMIT_QUE_ACT_CFG_QLIMIT_QUE_ACT(x)  VTSS_ENCODE_BITFIELD(x,0,6)
#define  VTSS_M_XQS_QLIMIT_SHR_QLIMIT_QUE_ACT_CFG_QLIMIT_QUE_ACT     VTSS_ENCODE_BITMASK(0,6)
#define  VTSS_X_XQS_QLIMIT_SHR_QLIMIT_QUE_ACT_CFG_QLIMIT_QUE_ACT(x)  VTSS_EXTRACT_BITFIELD(x,0,6)


/** 
 * \brief Size of congested SE
 *
 * \details
 * Register: \a XQS:QLIMIT_SHR:QLIMIT_SE_CONG_CFG
 *
 * @param gi Replicator: x_GAZ_CORE_QLIM_SHR_CNT (??), 0-3
 */
#define VTSS_XQS_QLIMIT_SHR_QLIMIT_SE_CONG_CFG(gi)  VTSS_IOREG_IX(VTSS_TO_XQS,0x600,gi,17,0,12)

/** 
 * \brief
 * An SE is regarded congested when its total queue size exceeds this.
 *
 * \details 
 * Field: ::VTSS_XQS_QLIMIT_SHR_QLIMIT_SE_CONG_CFG . QLIMIT_SE_CONG
 */
#define  VTSS_F_XQS_QLIMIT_SHR_QLIMIT_SE_CONG_CFG_QLIMIT_SE_CONG(x)  VTSS_ENCODE_BITFIELD(x,0,20)
#define  VTSS_M_XQS_QLIMIT_SHR_QLIMIT_SE_CONG_CFG_QLIMIT_SE_CONG     VTSS_ENCODE_BITMASK(0,20)
#define  VTSS_X_XQS_QLIMIT_SHR_QLIMIT_SE_CONG_CFG_QLIMIT_SE_CONG(x)  VTSS_EXTRACT_BITFIELD(x,0,20)


/** 
 * \brief Current amount of congested scheduling elements in the share
 *
 * \details
 * Register: \a XQS:QLIMIT_SHR:QLIMIT_CONG_CNT_STAT
 *
 * @param gi Replicator: x_GAZ_CORE_QLIM_SHR_CNT (??), 0-3
 */
#define VTSS_XQS_QLIMIT_SHR_QLIMIT_CONG_CNT_STAT(gi)  VTSS_IOREG_IX(VTSS_TO_XQS,0x600,gi,17,0,13)

/** 
 * \details 
 * Field: ::VTSS_XQS_QLIMIT_SHR_QLIMIT_CONG_CNT_STAT . QLIMIT_CONG_CNT
 */
#define  VTSS_F_XQS_QLIMIT_SHR_QLIMIT_CONG_CNT_STAT_QLIMIT_CONG_CNT(x)  VTSS_ENCODE_BITFIELD(x,15,12)
#define  VTSS_M_XQS_QLIMIT_SHR_QLIMIT_CONG_CNT_STAT_QLIMIT_CONG_CNT     VTSS_ENCODE_BITMASK(15,12)
#define  VTSS_X_XQS_QLIMIT_SHR_QLIMIT_CONG_CNT_STAT_QLIMIT_CONG_CNT(x)  VTSS_EXTRACT_BITFIELD(x,15,12)

/** 
 * \brief
 * Return the current number of active queues in the share.
 *
 * \details 
 * Field: ::VTSS_XQS_QLIMIT_SHR_QLIMIT_CONG_CNT_STAT . QLIMIT_ACT_CNT
 */
#define  VTSS_F_XQS_QLIMIT_SHR_QLIMIT_CONG_CNT_STAT_QLIMIT_ACT_CNT(x)  VTSS_ENCODE_BITFIELD(x,0,15)
#define  VTSS_M_XQS_QLIMIT_SHR_QLIMIT_CONG_CNT_STAT_QLIMIT_ACT_CNT     VTSS_ENCODE_BITMASK(0,15)
#define  VTSS_X_XQS_QLIMIT_SHR_QLIMIT_CONG_CNT_STAT_QLIMIT_ACT_CNT(x)  VTSS_EXTRACT_BITFIELD(x,0,15)


/** 
 * \brief Current use of the shared area
 *
 * \details
 * Register: \a XQS:QLIMIT_SHR:QLIMIT_SHR_FILL_STAT
 *
 * @param gi Replicator: x_GAZ_CORE_QLIM_SHR_CNT (??), 0-3
 */
#define VTSS_XQS_QLIMIT_SHR_QLIMIT_SHR_FILL_STAT(gi)  VTSS_IOREG_IX(VTSS_TO_XQS,0x600,gi,17,0,14)

/** 
 * \details 
 * Field: ::VTSS_XQS_QLIMIT_SHR_QLIMIT_SHR_FILL_STAT . QLIMIT_SHR_FILL
 */
#define  VTSS_F_XQS_QLIMIT_SHR_QLIMIT_SHR_FILL_STAT_QLIMIT_SHR_FILL(x)  VTSS_ENCODE_BITFIELD(x,0,15)
#define  VTSS_M_XQS_QLIMIT_SHR_QLIMIT_SHR_FILL_STAT_QLIMIT_SHR_FILL     VTSS_ENCODE_BITMASK(0,15)
#define  VTSS_X_XQS_QLIMIT_SHR_QLIMIT_SHR_FILL_STAT_QLIMIT_SHR_FILL(x)  VTSS_EXTRACT_BITFIELD(x,0,15)


/** 
 * \brief Current per SE watermark
 *
 * \details
 * Register: \a XQS:QLIMIT_SHR:QLIMIT_SHR_WM_STAT
 *
 * @param gi Replicator: x_GAZ_CORE_QLIM_SHR_CNT (??), 0-3
 * @param ri Register: QLIMIT_SHR_WM_STAT (??), 0-1
 */
#define VTSS_XQS_QLIMIT_SHR_QLIMIT_SHR_WM_STAT(gi,ri)  VTSS_IOREG_IX(VTSS_TO_XQS,0x600,gi,17,ri,15)

/** 
 * \details 
 * Field: ::VTSS_XQS_QLIMIT_SHR_QLIMIT_SHR_WM_STAT . QLIMIT_SHR_WM
 */
#define  VTSS_F_XQS_QLIMIT_SHR_QLIMIT_SHR_WM_STAT_QLIMIT_SHR_WM(x)  VTSS_ENCODE_BITFIELD(x,0,15)
#define  VTSS_M_XQS_QLIMIT_SHR_QLIMIT_SHR_WM_STAT_QLIMIT_SHR_WM     VTSS_ENCODE_BITMASK(0,15)
#define  VTSS_X_XQS_QLIMIT_SHR_QLIMIT_SHR_WM_STAT_QLIMIT_SHR_WM(x)  VTSS_EXTRACT_BITFIELD(x,0,15)

/**
 * Register Group: \a XQS:QLIMIT_MON
 *
 * Shared memory pool monitoring
 */


/** 
 * \brief Monitor configuration
 *
 * \details
 * Replicated for each classified priority
 *
 * Register: \a XQS:QLIMIT_MON:QLIMIT_MON_CFG
 *
 * @param gi Replicator: x_GAZ_CORE_QLIM_SHR_CNT (??), 0-3
 */
#define VTSS_XQS_QLIMIT_MON_QLIMIT_MON_CFG(gi)  VTSS_IOREG_IX(VTSS_TO_XQS,0x644,gi,3,0,0)

/** 
 * \brief
 * Clear shared memory pool monitoring statistics.
 *
 * \details 
 * Field: ::VTSS_XQS_QLIMIT_MON_QLIMIT_MON_CFG . QLIMIT_SHR_MON_CLR
 */
#define  VTSS_F_XQS_QLIMIT_MON_QLIMIT_MON_CFG_QLIMIT_SHR_MON_CLR(x)  VTSS_ENCODE_BITFIELD(!!(x),0,1)
#define  VTSS_M_XQS_QLIMIT_MON_QLIMIT_MON_CFG_QLIMIT_SHR_MON_CLR  VTSS_BIT(0)
#define  VTSS_X_XQS_QLIMIT_MON_QLIMIT_MON_CFG_QLIMIT_SHR_MON_CLR(x)  VTSS_EXTRACT_BITFIELD(x,0,1)


/** 
 * \brief Maximum amount of congested scheduling elements in the share
 *
 * \details
 * Register: \a XQS:QLIMIT_MON:QLIMIT_CONG_CNT_MAX_STAT
 *
 * @param gi Replicator: x_GAZ_CORE_QLIM_SHR_CNT (??), 0-3
 */
#define VTSS_XQS_QLIMIT_MON_QLIMIT_CONG_CNT_MAX_STAT(gi)  VTSS_IOREG_IX(VTSS_TO_XQS,0x644,gi,3,0,1)

/** 
 * \details 
 * Field: ::VTSS_XQS_QLIMIT_MON_QLIMIT_CONG_CNT_MAX_STAT . QLIMIT_CONG_CNT_MAX
 */
#define  VTSS_F_XQS_QLIMIT_MON_QLIMIT_CONG_CNT_MAX_STAT_QLIMIT_CONG_CNT_MAX(x)  VTSS_ENCODE_BITFIELD(x,0,12)
#define  VTSS_M_XQS_QLIMIT_MON_QLIMIT_CONG_CNT_MAX_STAT_QLIMIT_CONG_CNT_MAX     VTSS_ENCODE_BITMASK(0,12)
#define  VTSS_X_XQS_QLIMIT_MON_QLIMIT_CONG_CNT_MAX_STAT_QLIMIT_CONG_CNT_MAX(x)  VTSS_EXTRACT_BITFIELD(x,0,12)


/** 
 * \brief Maximum use of the shared area
 *
 * \details
 * Register: \a XQS:QLIMIT_MON:QLIMIT_SHR_FILL_MAX_STAT
 *
 * @param gi Replicator: x_GAZ_CORE_QLIM_SHR_CNT (??), 0-3
 */
#define VTSS_XQS_QLIMIT_MON_QLIMIT_SHR_FILL_MAX_STAT(gi)  VTSS_IOREG_IX(VTSS_TO_XQS,0x644,gi,3,0,2)

/** 
 * \details 
 * Field: ::VTSS_XQS_QLIMIT_MON_QLIMIT_SHR_FILL_MAX_STAT . QLIMIT_SHR_FILL_MAX
 */
#define  VTSS_F_XQS_QLIMIT_MON_QLIMIT_SHR_FILL_MAX_STAT_QLIMIT_SHR_FILL_MAX(x)  VTSS_ENCODE_BITFIELD(x,0,15)
#define  VTSS_M_XQS_QLIMIT_MON_QLIMIT_SHR_FILL_MAX_STAT_QLIMIT_SHR_FILL_MAX     VTSS_ENCODE_BITMASK(0,15)
#define  VTSS_X_XQS_QLIMIT_MON_QLIMIT_SHR_FILL_MAX_STAT_QLIMIT_SHR_FILL_MAX(x)  VTSS_EXTRACT_BITFIELD(x,0,15)

/**
 * Register Group: \a XQS:STAT
 *
 * Frame statistics
 */


/** 
 * \brief Counter values
 *
 * \details
 * See description for QSYS:STAT.
 *
 * Register: \a XQS:STAT:CNT
 *
 * @param gi Register: STAT (??), 0-1023
 */
#define VTSS_XQS_STAT_CNT(gi)                VTSS_IOREG_IX(VTSS_TO_XQS,0x0,gi,1,0,0)

/** 
 * \brief
 * Counter value, ref. QSYS:STAT.
 *
 * \details 
 * Field: ::VTSS_XQS_STAT_CNT . CNT
 */
#define  VTSS_F_XQS_STAT_CNT_CNT(x)           (x)
#define  VTSS_M_XQS_STAT_CNT_CNT              0xffffffff
#define  VTSS_X_XQS_STAT_CNT_CNT(x)           (x)


#endif /* _VTSS_JAGUAR2C_REGS_XQS_H_ */
