#ifndef _VTSS_LUTON26_REGS_REW_H_
#define _VTSS_LUTON26_REGS_REW_H_

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

#include "vtss_luton26_regs_common.h"

/*********************************************************************** 
 *
 * Target: \a REW
 *
 * Rewriter Configuration
 *
 ***********************************************************************/

/**
 * Register Group: \a REW:PORT
 *
 * Per port configurations for Rewriter
 */


/** 
 * \brief Port VLAN configuration
 *
 * \details
 * Register: \a REW:PORT:PORT_VLAN_CFG
 *
 * @param gi Register: PORT (??), 0-27
 */
#define VTSS_REW_PORT_PORT_VLAN_CFG(gi)      VTSS_IOREG_IX(VTSS_TO_REW,0x0,gi,32,0,0)

/** 
 * \brief
 * Tag Protocol Identifier for port.
 *
 * \details 
 * Field: ::VTSS_REW_PORT_PORT_VLAN_CFG . PORT_TPID
 */
#define  VTSS_F_REW_PORT_PORT_VLAN_CFG_PORT_TPID(x)  VTSS_ENCODE_BITFIELD(x,16,16)
#define  VTSS_M_REW_PORT_PORT_VLAN_CFG_PORT_TPID     VTSS_ENCODE_BITMASK(16,16)
#define  VTSS_X_REW_PORT_PORT_VLAN_CFG_PORT_TPID(x)  VTSS_EXTRACT_BITFIELD(x,16,16)

/** 
 * \brief
 * DEI value for port.
 *
 * \details 
 * Field: ::VTSS_REW_PORT_PORT_VLAN_CFG . PORT_DEI
 */
#define  VTSS_F_REW_PORT_PORT_VLAN_CFG_PORT_DEI  VTSS_BIT(15)

/** 
 * \brief
 * PCP value for port.
 *
 * \details 
 * Field: ::VTSS_REW_PORT_PORT_VLAN_CFG . PORT_PCP
 */
#define  VTSS_F_REW_PORT_PORT_VLAN_CFG_PORT_PCP(x)  VTSS_ENCODE_BITFIELD(x,12,3)
#define  VTSS_M_REW_PORT_PORT_VLAN_CFG_PORT_PCP     VTSS_ENCODE_BITMASK(12,3)
#define  VTSS_X_REW_PORT_PORT_VLAN_CFG_PORT_PCP(x)  VTSS_EXTRACT_BITFIELD(x,12,3)

/** 
 * \brief
 * VID value for port.
 *
 * \details 
 * Field: ::VTSS_REW_PORT_PORT_VLAN_CFG . PORT_VID
 */
#define  VTSS_F_REW_PORT_PORT_VLAN_CFG_PORT_VID(x)  VTSS_ENCODE_BITFIELD(x,0,12)
#define  VTSS_M_REW_PORT_PORT_VLAN_CFG_PORT_VID     VTSS_ENCODE_BITMASK(0,12)
#define  VTSS_X_REW_PORT_PORT_VLAN_CFG_PORT_VID(x)  VTSS_EXTRACT_BITFIELD(x,0,12)


/** 
 * \brief Tagging configuration
 *
 * \details
 * Register: \a REW:PORT:TAG_CFG
 *
 * @param gi Register: PORT (??), 0-27
 */
#define VTSS_REW_PORT_TAG_CFG(gi)            VTSS_IOREG_IX(VTSS_TO_REW,0x0,gi,32,0,1)

/** 
 * \brief
 * Enable VLAN port tagging.
 *
 * \details 
 * 0: Port tagging disabled.
 * 1: Tag all frames, except when VID=PORT_VLAN_CFG.PORT_VID or VID=0.
 * 2: Tag all frames, except when VID=0.
 * 3: Tag all frames.

 *
 * Field: ::VTSS_REW_PORT_TAG_CFG . TAG_CFG
 */
#define  VTSS_F_REW_PORT_TAG_CFG_TAG_CFG(x)   VTSS_ENCODE_BITFIELD(x,5,2)
#define  VTSS_M_REW_PORT_TAG_CFG_TAG_CFG      VTSS_ENCODE_BITMASK(5,2)
#define  VTSS_X_REW_PORT_TAG_CFG_TAG_CFG(x)   VTSS_EXTRACT_BITFIELD(x,5,2)

/** 
 * \brief
 * Select TPID EtherType in port tag.
 *
 * \details 
 * 0: Use 0x8100.
 * 1: Use 0x88A8.
 * 2: Use custom value from PORT_VLAN_CFG.PORT_TPID.
 * 3: Use PORT_VLAN_CFG.PORT_TPID, unless ingress tag was a C-tag
 * (EtherType = 0x8100)

 *
 * Field: ::VTSS_REW_PORT_TAG_CFG . TAG_TPID_CFG
 */
#define  VTSS_F_REW_PORT_TAG_CFG_TAG_TPID_CFG(x)  VTSS_ENCODE_BITFIELD(x,3,2)
#define  VTSS_M_REW_PORT_TAG_CFG_TAG_TPID_CFG     VTSS_ENCODE_BITMASK(3,2)
#define  VTSS_X_REW_PORT_TAG_CFG_TAG_TPID_CFG(x)  VTSS_EXTRACT_BITFIELD(x,3,2)

/** 
 * \brief
 * Select VID in port tag. It can be set to either the classified VID or
 * VID_A_VAL from the ES0 service action.
 *
 * \details 
 * 0: Use classified VID.
 * 1: Use VID_A_VAL from ES0 action if hit, otherwise use classified VID.
 *
 * Field: ::VTSS_REW_PORT_TAG_CFG . TAG_VID_CFG
 */
#define  VTSS_F_REW_PORT_TAG_CFG_TAG_VID_CFG  VTSS_BIT(2)

/** 
 * \brief
 * Select PCP/DEI fields in port tag.
 *
 * \details 
 * 0: Use classified PCP/DEI values.
 * 1: Use PCP/DEI values from ES0 action if hit, otherwise classified
 * values.
 * 2: Use PCP/DEI values from port VLAN tag in PORT_VLAN_CFG.
 * 3: Use DP level and QoS class mapped to PCP/DEI values
 * (PCP_DEI_QOS_MAP_CFG).

 *
 * Field: ::VTSS_REW_PORT_TAG_CFG . TAG_QOS_CFG
 */
#define  VTSS_F_REW_PORT_TAG_CFG_TAG_QOS_CFG(x)  VTSS_ENCODE_BITFIELD(x,0,2)
#define  VTSS_M_REW_PORT_TAG_CFG_TAG_QOS_CFG     VTSS_ENCODE_BITMASK(0,2)
#define  VTSS_X_REW_PORT_TAG_CFG_TAG_QOS_CFG(x)  VTSS_EXTRACT_BITFIELD(x,0,2)


/** 
 * \brief Special port configuration
 *
 * \details
 * Register: \a REW:PORT:PORT_CFG
 *
 * @param gi Register: PORT (??), 0-27
 */
#define VTSS_REW_PORT_PORT_CFG(gi)           VTSS_IOREG_IX(VTSS_TO_REW,0x0,gi,32,0,2)

/** 
 * \brief
 * Enable ES0 lookup.
 *
 * \details 
 * Field: ::VTSS_REW_PORT_PORT_CFG . ES0_ENA
 */
#define  VTSS_F_REW_PORT_PORT_CFG_ES0_ENA     VTSS_BIT(8)

/** 
 * \brief
 * Insert IFH into frame (mainly for CPU ports)
 *
 * \details 
 * Field: ::VTSS_REW_PORT_PORT_CFG . IFH_INSERT_ENA
 */
#define  VTSS_F_REW_PORT_PORT_CFG_IFH_INSERT_ENA  VTSS_BIT(7)

/** 
 * \brief
 * Select the position of IFH in the generated frames when IFH_INSERT_ENA
 * is set
 *
 * \details 
 * 0: IFH written before DMAC.
 * 1: IFH written after SMAC.

 *
 * Field: ::VTSS_REW_PORT_PORT_CFG . IFH_INSERT_MODE
 */
#define  VTSS_F_REW_PORT_PORT_CFG_IFH_INSERT_MODE  VTSS_BIT(6)

/** 
 * \brief
 * FCS update mode for frames not received on the CPU port.
 *
 * \details 
 * 0: Update FCS if frame data has changed
 * 1: Never update FCS
 * 2: Always update FCS
 *
 * Field: ::VTSS_REW_PORT_PORT_CFG . FCS_UPDATE_NONCPU_CFG
 */
#define  VTSS_F_REW_PORT_PORT_CFG_FCS_UPDATE_NONCPU_CFG(x)  VTSS_ENCODE_BITFIELD(x,4,2)
#define  VTSS_M_REW_PORT_PORT_CFG_FCS_UPDATE_NONCPU_CFG     VTSS_ENCODE_BITMASK(4,2)
#define  VTSS_X_REW_PORT_PORT_CFG_FCS_UPDATE_NONCPU_CFG(x)  VTSS_EXTRACT_BITFIELD(x,4,2)

/** 
 * \brief
 * If set, update FCS for all frames injected by the CPU. If cleared, never
 * update the FCS.
 *
 * \details 
 * Field: ::VTSS_REW_PORT_PORT_CFG . FCS_UPDATE_CPU_ENA
 */
#define  VTSS_F_REW_PORT_PORT_CFG_FCS_UPDATE_CPU_ENA  VTSS_BIT(3)

/** 
 * \brief
 * If set, all frames destined for the egress port are discarded. 
 *
 * \details 
 * Field: ::VTSS_REW_PORT_PORT_CFG . FLUSH_ENA
 */
#define  VTSS_F_REW_PORT_PORT_CFG_FLUSH_ENA   VTSS_BIT(2)

/** 
 * \brief
 * Disable frame ageing for this egress port
 *
 * \details 
 * Field: ::VTSS_REW_PORT_PORT_CFG . AGE_DIS
 */
#define  VTSS_F_REW_PORT_PORT_CFG_AGE_DIS     VTSS_BIT(1)


/** 
 * \brief DSCP updates
 *
 * \details
 * Register: \a REW:PORT:DSCP_CFG
 *
 * @param gi Register: PORT (??), 0-27
 */
#define VTSS_REW_PORT_DSCP_CFG(gi)           VTSS_IOREG_IX(VTSS_TO_REW,0x0,gi,32,0,3)

/** 
 * \brief
 * Egress DSCP rewrite. 

 *
 * \details 
 * 0: No update of DSCP value in frame.
 * 1: Update with DSCP value from analyzer.
 * 2: Update with DSCP value from analyzer remapped through DSCP_REMAP_CFG.
 * 3. Update with DSCP value from analyzer remapped based on drop
 * precedence level through DSCP_REMAP_CFG or DSCP_REMAP_DP1_CFG.
 *
 * Field: ::VTSS_REW_PORT_DSCP_CFG . DSCP_REWR_CFG
 */
#define  VTSS_F_REW_PORT_DSCP_CFG_DSCP_REWR_CFG(x)  VTSS_ENCODE_BITFIELD(x,0,2)
#define  VTSS_M_REW_PORT_DSCP_CFG_DSCP_REWR_CFG     VTSS_ENCODE_BITMASK(0,2)
#define  VTSS_X_REW_PORT_DSCP_CFG_DSCP_REWR_CFG(x)  VTSS_EXTRACT_BITFIELD(x,0,2)


/** 
 * \brief Mapping of DP level and QoS class to PCP and DEI values.
 *
 * \details
 * Register: \a REW:PORT:PCP_DEI_QOS_MAP_CFG
 *
 * @param gi Register: PORT (??), 0-27
 * @param ri Register: PCP_DEI_QOS_MAP_CFG (??), 0-15
 */
#define VTSS_REW_PORT_PCP_DEI_QOS_MAP_CFG(gi,ri)  VTSS_IOREG_IX(VTSS_TO_REW,0x0,gi,32,ri,4)

/** 
 * \brief
 * Map the frame's DP level and QoS class to a DEI value. DEI =
 * PCP_DEI_QOS_MAP_CFG[8*DP level + QoS class].DEI_QOS_VAL. This must be
 * enabled in VLAN_CFG.QOS_CFG.
 *
 * \details 
 * Field: ::VTSS_REW_PORT_PCP_DEI_QOS_MAP_CFG . DEI_QOS_VAL
 */
#define  VTSS_F_REW_PORT_PCP_DEI_QOS_MAP_CFG_DEI_QOS_VAL  VTSS_BIT(3)

/** 
 * \brief
 * Map the frame's DP level and QoS class to a PCP value. PCP =
 * PCP_DEI_QOS_MAP_CFG[8*DP level + QoS class].PCP_QOS_VAL. This must be
 * enabled in VLAN_CFG.QOS_CFG.
 *
 * \details 
 * Field: ::VTSS_REW_PORT_PCP_DEI_QOS_MAP_CFG . PCP_QOS_VAL
 */
#define  VTSS_F_REW_PORT_PCP_DEI_QOS_MAP_CFG_PCP_QOS_VAL(x)  VTSS_ENCODE_BITFIELD(x,0,3)
#define  VTSS_M_REW_PORT_PCP_DEI_QOS_MAP_CFG_PCP_QOS_VAL     VTSS_ENCODE_BITMASK(0,3)
#define  VTSS_X_REW_PORT_PCP_DEI_QOS_MAP_CFG_PCP_QOS_VAL(x)  VTSS_EXTRACT_BITFIELD(x,0,3)

/**
 * Register Group: \a REW:COMMON
 *
 * Common configurations for Rewriter
 */


/** 
 * \brief Remap table of DSCP values for frames with drop precedence set
 *
 * \details
 * Register: \a REW:COMMON:DSCP_REMAP_DP1_CFG
 *
 * @param ri Register: DSCP_REMAP_DP1_CFG (??), 0-63
 */
#define VTSS_REW_COMMON_DSCP_REMAP_DP1_CFG(ri)  VTSS_IOREG(VTSS_TO_REW,0x380 + (ri))

/** 
 * \brief
 * One to one DSCP remapping table common for all ports. This table is used
 * if DSCP_CFG.DSCP_REWR_ENA=3 and DP=1.
 *
 * \details 
 * Field: ::VTSS_REW_COMMON_DSCP_REMAP_DP1_CFG . DSCP_REMAP_DP1_VAL
 */
#define  VTSS_F_REW_COMMON_DSCP_REMAP_DP1_CFG_DSCP_REMAP_DP1_VAL(x)  VTSS_ENCODE_BITFIELD(x,0,6)
#define  VTSS_M_REW_COMMON_DSCP_REMAP_DP1_CFG_DSCP_REMAP_DP1_VAL     VTSS_ENCODE_BITMASK(0,6)
#define  VTSS_X_REW_COMMON_DSCP_REMAP_DP1_CFG_DSCP_REMAP_DP1_VAL(x)  VTSS_EXTRACT_BITFIELD(x,0,6)


/** 
 * \brief Remap table of DSCP values.
 *
 * \details
 * Register: \a REW:COMMON:DSCP_REMAP_CFG
 *
 * @param ri Register: DSCP_REMAP_CFG (??), 0-63
 */
#define VTSS_REW_COMMON_DSCP_REMAP_CFG(ri)   VTSS_IOREG(VTSS_TO_REW,0x3c0 + (ri))

/** 
 * \brief
 * One to one DSCP remapping table common for all ports. This table is used
 * if DSCP_CFG.DSCP_REWR_ENA=2 or if DSCP_CFG.DSCP_REWR_ENA=3 and DP=0.
 *
 * \details 
 * Field: ::VTSS_REW_COMMON_DSCP_REMAP_CFG . DSCP_REMAP_VAL
 */
#define  VTSS_F_REW_COMMON_DSCP_REMAP_CFG_DSCP_REMAP_VAL(x)  VTSS_ENCODE_BITFIELD(x,0,6)
#define  VTSS_M_REW_COMMON_DSCP_REMAP_CFG_DSCP_REMAP_VAL     VTSS_ENCODE_BITMASK(0,6)
#define  VTSS_X_REW_COMMON_DSCP_REMAP_CFG_DSCP_REMAP_VAL(x)  VTSS_EXTRACT_BITFIELD(x,0,6)


#endif /* _VTSS_LUTON26_REGS_REW_H_ */
