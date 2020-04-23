#ifndef _VTSS_MALIBU_REGS_PTP_ANA2_EGR_CFG23_H_
#define _VTSS_MALIBU_REGS_PTP_ANA2_EGR_CFG23_H_

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

#include "vtss_malibu_regs_common.h"

/*********************************************************************** 
 *
 * Target: \a PTP_ANA2_EGR_CFG23
 *
 * Analyzer engine configuration registers
 *
 ***********************************************************************/

/**
 * Register Group: \a PTP_ANA2_EGR_CFG23:EGR2_ETH1_NXT_PROTOCOL_A
 *
 * Ethernet next protocol configuration
 */


/** 
 * \brief Ethernet next protocol register
 *
 * \details
 * Register: \a PTP_ANA2_EGR_CFG23:EGR2_ETH1_NXT_PROTOCOL_A:EGR2_ETH1_NXT_PROTOCOL_A
 */
#define VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH1_NXT_PROTOCOL_A_EGR2_ETH1_NXT_PROTOCOL_A  VTSS_IOREG(0x1e, 1, 0xc800)

/** 
 * \brief
 * Frame signature offset.
 * Points to the start of the byte field in the Ethernet frame that will be
 * used for the frame signature.
 *
 * \details 
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH1_NXT_PROTOCOL_A_EGR2_ETH1_NXT_PROTOCOL_A . ETH1_FRAME_SIG_OFFSET_A
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_ETH1_NXT_PROTOCOL_A_EGR2_ETH1_NXT_PROTOCOL_A_ETH1_FRAME_SIG_OFFSET_A(x)  VTSS_ENCODE_BITFIELD(x,16,5)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_ETH1_NXT_PROTOCOL_A_EGR2_ETH1_NXT_PROTOCOL_A_ETH1_FRAME_SIG_OFFSET_A     VTSS_ENCODE_BITMASK(16,5)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_ETH1_NXT_PROTOCOL_A_EGR2_ETH1_NXT_PROTOCOL_A_ETH1_FRAME_SIG_OFFSET_A(x)  VTSS_EXTRACT_BITFIELD(x,16,5)

/** 
 * \brief
 * Points to the next comparator block after this Ethernet block
 *
 * \details 
 * 0: Reserved
 * 1: Ethernet comparator 2
 * 2: IP/UDP/ACH comparator 1
 * 3: IP/UDP/ACH comparator 2
 * 4: MPLS comparator
 * 5: PTP/OAM comparator
 * 6,7: Reserved
 *
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH1_NXT_PROTOCOL_A_EGR2_ETH1_NXT_PROTOCOL_A . ETH1_NXT_COMPARATOR_A
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_ETH1_NXT_PROTOCOL_A_EGR2_ETH1_NXT_PROTOCOL_A_ETH1_NXT_COMPARATOR_A(x)  VTSS_ENCODE_BITFIELD(x,0,3)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_ETH1_NXT_PROTOCOL_A_EGR2_ETH1_NXT_PROTOCOL_A_ETH1_NXT_COMPARATOR_A     VTSS_ENCODE_BITMASK(0,3)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_ETH1_NXT_PROTOCOL_A_EGR2_ETH1_NXT_PROTOCOL_A_ETH1_NXT_COMPARATOR_A(x)  VTSS_EXTRACT_BITFIELD(x,0,3)


/** 
 * \brief VLAN TPID configuration
 *
 * \details
 * Register: \a PTP_ANA2_EGR_CFG23:EGR2_ETH1_NXT_PROTOCOL_A:EGR2_ETH1_VLAN_TPID_CFG_A
 */
#define VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH1_NXT_PROTOCOL_A_EGR2_ETH1_VLAN_TPID_CFG_A  VTSS_IOREG(0x1e, 1, 0xc801)

/** 
 * \brief
 * Configurable VLAN TPID (S or B-tag)
 *
 * \details 
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH1_NXT_PROTOCOL_A_EGR2_ETH1_VLAN_TPID_CFG_A . ETH1_VLAN_TPID_CFG_A
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_ETH1_NXT_PROTOCOL_A_EGR2_ETH1_VLAN_TPID_CFG_A_ETH1_VLAN_TPID_CFG_A(x)  VTSS_ENCODE_BITFIELD(x,16,16)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_ETH1_NXT_PROTOCOL_A_EGR2_ETH1_VLAN_TPID_CFG_A_ETH1_VLAN_TPID_CFG_A     VTSS_ENCODE_BITMASK(16,16)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_ETH1_NXT_PROTOCOL_A_EGR2_ETH1_VLAN_TPID_CFG_A_ETH1_VLAN_TPID_CFG_A(x)  VTSS_EXTRACT_BITFIELD(x,16,16)


/** 
 * \brief Ethernet tag mode
 *
 * \details
 * Register: \a PTP_ANA2_EGR_CFG23:EGR2_ETH1_NXT_PROTOCOL_A:EGR2_ETH1_TAG_MODE_A
 */
#define VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH1_NXT_PROTOCOL_A_EGR2_ETH1_TAG_MODE_A  VTSS_IOREG(0x1e, 1, 0xc802)

/** 
 * \brief
 * This bit enables the presence of PBB.
 * The I-tag match bits are programmed in the ETH1_VLAN_TAG_RANGE
 * registers. The mask bits are programmed in the ETH1_VLAN_TAG2 registers.
 * A B-tag if present is configured in the ETH1_VLAN_TAG1 registers.
 *
 * \details 
 * 0: PBB not enabled
 * 1: Always expect PBB, last tag is always an I-tag
 *
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH1_NXT_PROTOCOL_A_EGR2_ETH1_TAG_MODE_A . ETH1_PBB_ENA_A
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_ETH1_NXT_PROTOCOL_A_EGR2_ETH1_TAG_MODE_A_ETH1_PBB_ENA_A  VTSS_BIT(0)


/** 
 * \brief Ethertype match register
 *
 * \details
 * Register: \a PTP_ANA2_EGR_CFG23:EGR2_ETH1_NXT_PROTOCOL_A:EGR2_ETH1_ETYPE_MATCH_A
 */
#define VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH1_NXT_PROTOCOL_A_EGR2_ETH1_ETYPE_MATCH_A  VTSS_IOREG(0x1e, 1, 0xc803)

/** 
 * \brief
 * Configure Ethertype verification
 *
 * \details 
 * 0: Disable Ethertype verification
 * 1: Enable Ethertype verification
 *
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH1_NXT_PROTOCOL_A_EGR2_ETH1_ETYPE_MATCH_A . ETH1_ETYPE_MATCH_ENA_A
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_ETH1_NXT_PROTOCOL_A_EGR2_ETH1_ETYPE_MATCH_A_ETH1_ETYPE_MATCH_ENA_A  VTSS_BIT(16)

/** 
 * \brief
 * If the Ethertype/length field is an Ethertype, then this register is
 * compared against the value. If the field is a length, the length value
 * is not checked.
 *
 * \details 
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH1_NXT_PROTOCOL_A_EGR2_ETH1_ETYPE_MATCH_A . ETH1_ETYPE_MATCH_A
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_ETH1_NXT_PROTOCOL_A_EGR2_ETH1_ETYPE_MATCH_A_ETH1_ETYPE_MATCH_A(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_ETH1_NXT_PROTOCOL_A_EGR2_ETH1_ETYPE_MATCH_A_ETH1_ETYPE_MATCH_A     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_ETH1_NXT_PROTOCOL_A_EGR2_ETH1_ETYPE_MATCH_A_ETH1_ETYPE_MATCH_A(x)  VTSS_EXTRACT_BITFIELD(x,0,16)

/**
 * Register Group: \a PTP_ANA2_EGR_CFG23:EGR2_ETH1_NXT_PROTOCOL_B
 *
 * Ethernet next protocol configuration
 */


/** 
 * \brief Ethernet next protocol register
 *
 * \details
 * Register: \a PTP_ANA2_EGR_CFG23:EGR2_ETH1_NXT_PROTOCOL_B:EGR2_ETH1_NXT_PROTOCOL_B
 */
#define VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH1_NXT_PROTOCOL_B_EGR2_ETH1_NXT_PROTOCOL_B  VTSS_IOREG(0x1e, 1, 0xc810)

/** 
 * \brief
 * Frame signature offset.
 * Points to the start of the byte field in the Ethernet frame that will be
 * used for the frame signature.
 *
 * \details 
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH1_NXT_PROTOCOL_B_EGR2_ETH1_NXT_PROTOCOL_B . ETH1_FRAME_SIG_OFFSET_B
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_ETH1_NXT_PROTOCOL_B_EGR2_ETH1_NXT_PROTOCOL_B_ETH1_FRAME_SIG_OFFSET_B(x)  VTSS_ENCODE_BITFIELD(x,16,5)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_ETH1_NXT_PROTOCOL_B_EGR2_ETH1_NXT_PROTOCOL_B_ETH1_FRAME_SIG_OFFSET_B     VTSS_ENCODE_BITMASK(16,5)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_ETH1_NXT_PROTOCOL_B_EGR2_ETH1_NXT_PROTOCOL_B_ETH1_FRAME_SIG_OFFSET_B(x)  VTSS_EXTRACT_BITFIELD(x,16,5)

/** 
 * \brief
 * Points to the next comparator block after this Ethernet block
 *
 * \details 
 * 0: Reserved
 * 1: Ethernet comparator 2
 * 2: IP/UDP/ACH comparator 1
 * 3: IP/UDP/ACH comparator 2
 * 4: MPLS comparator
 * 5: PTP/OAM comparator
 * 6,7: Reserved
 *
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH1_NXT_PROTOCOL_B_EGR2_ETH1_NXT_PROTOCOL_B . ETH1_NXT_COMPARATOR_B
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_ETH1_NXT_PROTOCOL_B_EGR2_ETH1_NXT_PROTOCOL_B_ETH1_NXT_COMPARATOR_B(x)  VTSS_ENCODE_BITFIELD(x,0,3)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_ETH1_NXT_PROTOCOL_B_EGR2_ETH1_NXT_PROTOCOL_B_ETH1_NXT_COMPARATOR_B     VTSS_ENCODE_BITMASK(0,3)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_ETH1_NXT_PROTOCOL_B_EGR2_ETH1_NXT_PROTOCOL_B_ETH1_NXT_COMPARATOR_B(x)  VTSS_EXTRACT_BITFIELD(x,0,3)


/** 
 * \brief VLAN TPID configuration
 *
 * \details
 * Register: \a PTP_ANA2_EGR_CFG23:EGR2_ETH1_NXT_PROTOCOL_B:EGR2_ETH1_VLAN_TPID_CFG_B
 */
#define VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH1_NXT_PROTOCOL_B_EGR2_ETH1_VLAN_TPID_CFG_B  VTSS_IOREG(0x1e, 1, 0xc811)

/** 
 * \brief
 * Configurable VLAN TPID (S or B-tag)
 *
 * \details 
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH1_NXT_PROTOCOL_B_EGR2_ETH1_VLAN_TPID_CFG_B . ETH1_VLAN_TPID_CFG_B
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_ETH1_NXT_PROTOCOL_B_EGR2_ETH1_VLAN_TPID_CFG_B_ETH1_VLAN_TPID_CFG_B(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_ETH1_NXT_PROTOCOL_B_EGR2_ETH1_VLAN_TPID_CFG_B_ETH1_VLAN_TPID_CFG_B     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_ETH1_NXT_PROTOCOL_B_EGR2_ETH1_VLAN_TPID_CFG_B_ETH1_VLAN_TPID_CFG_B(x)  VTSS_EXTRACT_BITFIELD(x,0,16)


/** 
 * \brief Ethernet tag mode
 *
 * \details
 * Register: \a PTP_ANA2_EGR_CFG23:EGR2_ETH1_NXT_PROTOCOL_B:EGR2_ETH1_TAG_MODE_B
 */
#define VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH1_NXT_PROTOCOL_B_EGR2_ETH1_TAG_MODE_B  VTSS_IOREG(0x1e, 1, 0xc812)

/** 
 * \brief
 * This bit enables the presence of PBB.  
 * The I-tag match bits are programmed in the ETH1_VLAN_TAG_RANGE
 * registers. The mask bits are progrogrammed in the ETH1_VLAN_TAG2
 * registers. A B-tag if present is configured in the ETH1_VLAN_TAG1
 * registers.
 *
 * \details 
 * 0: PBB not enabled
 * 1: Always expect PBB, last tag is always an I-tag
 *
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH1_NXT_PROTOCOL_B_EGR2_ETH1_TAG_MODE_B . ETH1_PBB_ENA_B
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_ETH1_NXT_PROTOCOL_B_EGR2_ETH1_TAG_MODE_B_ETH1_PBB_ENA_B  VTSS_BIT(0)


/** 
 * \brief Ethertype match register
 *
 * \details
 * Register: \a PTP_ANA2_EGR_CFG23:EGR2_ETH1_NXT_PROTOCOL_B:EGR2_ETH1_ETYPE_MATCH_B
 */
#define VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH1_NXT_PROTOCOL_B_EGR2_ETH1_ETYPE_MATCH_B  VTSS_IOREG(0x1e, 1, 0xc813)

/** 
 * \brief
 * Configure Ethertype verification
 *
 * \details 
 * 0: Disable Ethertype verification
 * 1: Enable Ethertype verification
 *
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH1_NXT_PROTOCOL_B_EGR2_ETH1_ETYPE_MATCH_B . ETH1_ETYPE_MATCH_ENA_B
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_ETH1_NXT_PROTOCOL_B_EGR2_ETH1_ETYPE_MATCH_B_ETH1_ETYPE_MATCH_ENA_B  VTSS_BIT(16)

/** 
 * \brief
 * If the Ethertype/length field is an Ethertype, then this register is
 * compared against the value. If the field is a length, the length value
 * is not checked.
 *
 * \details 
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH1_NXT_PROTOCOL_B_EGR2_ETH1_ETYPE_MATCH_B . ETH1_ETYPE_MATCH_B
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_ETH1_NXT_PROTOCOL_B_EGR2_ETH1_ETYPE_MATCH_B_ETH1_ETYPE_MATCH_B(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_ETH1_NXT_PROTOCOL_B_EGR2_ETH1_ETYPE_MATCH_B_ETH1_ETYPE_MATCH_B     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_ETH1_NXT_PROTOCOL_B_EGR2_ETH1_ETYPE_MATCH_B_ETH1_ETYPE_MATCH_B(x)  VTSS_EXTRACT_BITFIELD(x,0,16)

/**
 * Register Group: \a PTP_ANA2_EGR_CFG23:EGR2_ETH1_FLOW_CFG
 *
 * Ethernet flow configuration
 */


/** 
 * \brief Ethernet flow enable
 *
 * \details
 * Register: \a PTP_ANA2_EGR_CFG23:EGR2_ETH1_FLOW_CFG:EGR2_ETH1_FLOW_ENABLE
 *
 * @param gi Register: EGR2_ETH1_FLOW_CFG, 0-7
 */
#define VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH1_FLOW_CFG_EGR2_ETH1_FLOW_ENABLE(gi)  VTSS_IOREG_IX(0x1e, 1, 0xc820, gi, 16, 0, 0)

/** 
 * \brief
 * Indicates which next-protocol configuration group is valid with this
 * flow
 *
 * \details 
 * 0: Associate this flow with next-protocol group A
 * 1: Associate this flow with next-protocol group B
 *
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH1_FLOW_CFG_EGR2_ETH1_FLOW_ENABLE . ETH1_NXT_PROT_GRP_SEL
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_ETH1_FLOW_CFG_EGR2_ETH1_FLOW_ENABLE_ETH1_NXT_PROT_GRP_SEL  VTSS_BIT(16)

/** 
 * \brief
 * Channel mask
 *
 * \details 
 * 0: Flow valid for channel 0
 * 1: Flow valid for channel 1
 *
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH1_FLOW_CFG_EGR2_ETH1_FLOW_ENABLE . ETH1_CHANNEL_MASK
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_ETH1_FLOW_CFG_EGR2_ETH1_FLOW_ENABLE_ETH1_CHANNEL_MASK(x)  VTSS_ENCODE_BITFIELD(x,8,2)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_ETH1_FLOW_CFG_EGR2_ETH1_FLOW_ENABLE_ETH1_CHANNEL_MASK     VTSS_ENCODE_BITMASK(8,2)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_ETH1_FLOW_CFG_EGR2_ETH1_FLOW_ENABLE_ETH1_CHANNEL_MASK(x)  VTSS_EXTRACT_BITFIELD(x,8,2)

/** 
 * \brief
 * Flow enable
 *
 * \details 
 * 0: Flow disabled
 * 1: Flow enabled
 *
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH1_FLOW_CFG_EGR2_ETH1_FLOW_ENABLE . ETH1_FLOW_ENABLE
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_ETH1_FLOW_CFG_EGR2_ETH1_FLOW_ENABLE_ETH1_FLOW_ENABLE  VTSS_BIT(0)


/** 
 * \brief Ethernet protocol match mode
 *
 * \details
 * Register: \a PTP_ANA2_EGR_CFG23:EGR2_ETH1_FLOW_CFG:EGR2_ETH1_MATCH_MODE
 *
 * @param gi Register: EGR2_ETH1_FLOW_CFG, 0-7
 */
#define VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH1_FLOW_CFG_EGR2_ETH1_MATCH_MODE(gi)  VTSS_IOREG_IX(0x1e, 1, 0xc820, gi, 16, 0, 1)

/** 
 * \brief
 * VLAN tag mode configuration
 *
 * \details 
 * 0: VLAN range checking disabled
 * 1: VLAN range checking on tag 1
 * 2: VLAN range checking on tag 2 (not supported with PBB)
 * 3: Reserved
 *
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH1_FLOW_CFG_EGR2_ETH1_MATCH_MODE . ETH1_VLAN_TAG_MODE
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_ETH1_FLOW_CFG_EGR2_ETH1_MATCH_MODE_ETH1_VLAN_TAG_MODE(x)  VTSS_ENCODE_BITFIELD(x,12,2)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_ETH1_FLOW_CFG_EGR2_ETH1_MATCH_MODE_ETH1_VLAN_TAG_MODE     VTSS_ENCODE_BITMASK(12,2)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_ETH1_FLOW_CFG_EGR2_ETH1_MATCH_MODE_ETH1_VLAN_TAG_MODE(x)  VTSS_EXTRACT_BITFIELD(x,12,2)

/** 
 * \brief
 * This register is only used if ETH1_VLAN_VERIFY_ENA = 1
 *
 * \details 
 * If PBB not enabled:
 * 0: C tag (TPID of 0x8100)
 * 1: S tag (match to CONF_VLAN_TPID)
 * If PBB enabled:
 * 0,1: I tag (use range registers)
 *
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH1_FLOW_CFG_EGR2_ETH1_MATCH_MODE . ETH1_VLAN_TAG2_TYPE
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_ETH1_FLOW_CFG_EGR2_ETH1_MATCH_MODE_ETH1_VLAN_TAG2_TYPE  VTSS_BIT(9)

/** 
 * \brief
 * This register is only used if ETH1_VLAN_VERIFY_ENA = 1
 *
 * \details 
 * 0: C tag (TPID of 0x8100)
 * 1: S or B tag (match to CONF_VLAN_TPID)
 *
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH1_FLOW_CFG_EGR2_ETH1_MATCH_MODE . ETH1_VLAN_TAG1_TYPE
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_ETH1_FLOW_CFG_EGR2_ETH1_MATCH_MODE_ETH1_VLAN_TAG1_TYPE  VTSS_BIT(8)

/** 
 * \brief
 * This register is only used if ETH1_VLAN_VERIFY_ENA = 1
 *
 * \details 
 * 0: No VLAN tags (not valid for PBB)
 * 1: 1 VLAN tag (for PBB this would be the I-tag)
 * 2: 2 VLAN tags (for PBB expect a B-tag and an I-tag)
 * 3: Reserved
 *
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH1_FLOW_CFG_EGR2_ETH1_MATCH_MODE . ETH1_VLAN_TAGS
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_ETH1_FLOW_CFG_EGR2_ETH1_MATCH_MODE_ETH1_VLAN_TAGS(x)  VTSS_ENCODE_BITFIELD(x,6,2)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_ETH1_FLOW_CFG_EGR2_ETH1_MATCH_MODE_ETH1_VLAN_TAGS     VTSS_ENCODE_BITMASK(6,2)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_ETH1_FLOW_CFG_EGR2_ETH1_MATCH_MODE_ETH1_VLAN_TAGS(x)  VTSS_EXTRACT_BITFIELD(x,6,2)

/** 
 * \brief
 * Verify VLAN tags
 *
 * \details 
 * 0: Parse for VLAN tags; do not check values. For PBB the I-tag is always
 * checked
 * 1: Verify configured VLAN tag configuration
 *
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH1_FLOW_CFG_EGR2_ETH1_MATCH_MODE . ETH1_VLAN_VERIFY_ENA
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_ETH1_FLOW_CFG_EGR2_ETH1_MATCH_MODE_ETH1_VLAN_VERIFY_ENA  VTSS_BIT(4)

/** 
 * \brief
 * When checking for presence of SNAP/LLC based upon ETH1_MATCH_MODE, this
 * field indicates if SNAP and 3-byte LLC is expected to be present.
 *
 * \details 
 * 0: Only Ethernet type II supported, no SNAP/LLC
 * 1: Ethernet type II and Ethernet type I with SNAP/LLC, determine if
 * SNAP/LLC is present or not. Type I always assumes that SNAP/LLC is
 * present.
 *
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH1_FLOW_CFG_EGR2_ETH1_MATCH_MODE . ETH1_ETHERTYPE_MODE
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_ETH1_FLOW_CFG_EGR2_ETH1_MATCH_MODE_ETH1_ETHERTYPE_MODE  VTSS_BIT(0)


/** 
 * \brief Ethernet address match part 1
 *
 * \details
 * Register: \a PTP_ANA2_EGR_CFG23:EGR2_ETH1_FLOW_CFG:EGR2_ETH1_ADDR_MATCH_1
 *
 * @param gi Register: EGR2_ETH1_FLOW_CFG, 0-7
 */
#define VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH1_FLOW_CFG_EGR2_ETH1_ADDR_MATCH_1(gi)  VTSS_IOREG_IX(0x1e, 1, 0xc820, gi, 16, 0, 2)


/** 
 * \brief Ethernet address match part 2
 *
 * \details
 * Register: \a PTP_ANA2_EGR_CFG23:EGR2_ETH1_FLOW_CFG:EGR2_ETH1_ADDR_MATCH_2
 *
 * @param gi Register: EGR2_ETH1_FLOW_CFG, 0-7
 */
#define VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH1_FLOW_CFG_EGR2_ETH1_ADDR_MATCH_2(gi)  VTSS_IOREG_IX(0x1e, 1, 0xc820, gi, 16, 0, 3)

/** 
 * \brief
 * Selects how the addresses are matched. Multiple bits can be set at once.
 *
 * \details 
 * 0: Full 48-bit address match
 * 1: Match any unicast address
 * 2: Match any muliticast address
 *
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH1_FLOW_CFG_EGR2_ETH1_ADDR_MATCH_2 . ETH1_ADDR_MATCH_MODE
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_ETH1_FLOW_CFG_EGR2_ETH1_ADDR_MATCH_2_ETH1_ADDR_MATCH_MODE(x)  VTSS_ENCODE_BITFIELD(x,20,3)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_ETH1_FLOW_CFG_EGR2_ETH1_ADDR_MATCH_2_ETH1_ADDR_MATCH_MODE     VTSS_ENCODE_BITMASK(20,3)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_ETH1_FLOW_CFG_EGR2_ETH1_ADDR_MATCH_2_ETH1_ADDR_MATCH_MODE(x)  VTSS_EXTRACT_BITFIELD(x,20,3)

/** 
 * \brief
 * Selects which address to match
 *
 * \details 
 * 0: Match the destination address
 * 1: Match the source address
 * 2: Match either the source of destination address
 * 3: Reserved
 *
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH1_FLOW_CFG_EGR2_ETH1_ADDR_MATCH_2 . ETH1_ADDR_MATCH_SELECT
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_ETH1_FLOW_CFG_EGR2_ETH1_ADDR_MATCH_2_ETH1_ADDR_MATCH_SELECT(x)  VTSS_ENCODE_BITFIELD(x,16,2)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_ETH1_FLOW_CFG_EGR2_ETH1_ADDR_MATCH_2_ETH1_ADDR_MATCH_SELECT     VTSS_ENCODE_BITMASK(16,2)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_ETH1_FLOW_CFG_EGR2_ETH1_ADDR_MATCH_2_ETH1_ADDR_MATCH_SELECT(x)  VTSS_EXTRACT_BITFIELD(x,16,2)

/** 
 * \brief
 * Last 16 bits of the Ethernet address match field
 *
 * \details 
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH1_FLOW_CFG_EGR2_ETH1_ADDR_MATCH_2 . ETH1_ADDR_MATCH_2
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_ETH1_FLOW_CFG_EGR2_ETH1_ADDR_MATCH_2_ETH1_ADDR_MATCH_2(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_ETH1_FLOW_CFG_EGR2_ETH1_ADDR_MATCH_2_ETH1_ADDR_MATCH_2     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_ETH1_FLOW_CFG_EGR2_ETH1_ADDR_MATCH_2_ETH1_ADDR_MATCH_2(x)  VTSS_EXTRACT_BITFIELD(x,0,16)


/** 
 * \brief Ethernet VLAN tag range match register
 *
 * \details
 * Register: \a PTP_ANA2_EGR_CFG23:EGR2_ETH1_FLOW_CFG:EGR2_ETH1_VLAN_TAG_RANGE_I_TAG
 *
 * @param gi Register: EGR2_ETH1_FLOW_CFG, 0-7
 */
#define VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH1_FLOW_CFG_EGR2_ETH1_VLAN_TAG_RANGE_I_TAG(gi)  VTSS_IOREG_IX(0x1e, 1, 0xc820, gi, 16, 0, 4)

/** 
 * \brief
 * If PBB mode is not enabled, then this register contains the upper range
 * of the VLAN tag range match. 
 * If PBB mode is enabled, then this register contains the upper 12 bits of
 * the I-tag.
 *
 * \details 
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH1_FLOW_CFG_EGR2_ETH1_VLAN_TAG_RANGE_I_TAG . ETH1_VLAN_TAG_RANGE_UPPER
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_ETH1_FLOW_CFG_EGR2_ETH1_VLAN_TAG_RANGE_I_TAG_ETH1_VLAN_TAG_RANGE_UPPER(x)  VTSS_ENCODE_BITFIELD(x,16,12)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_ETH1_FLOW_CFG_EGR2_ETH1_VLAN_TAG_RANGE_I_TAG_ETH1_VLAN_TAG_RANGE_UPPER     VTSS_ENCODE_BITMASK(16,12)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_ETH1_FLOW_CFG_EGR2_ETH1_VLAN_TAG_RANGE_I_TAG_ETH1_VLAN_TAG_RANGE_UPPER(x)  VTSS_EXTRACT_BITFIELD(x,16,12)

/** 
 * \brief
 * If PBB mode is not enabled, then this register contains the lower range
 * of the VLAN tag range match.
 * If PBB mode is enabled, then this register contains the lower 12 bits of
 * the I-tag.
 *
 * \details 
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH1_FLOW_CFG_EGR2_ETH1_VLAN_TAG_RANGE_I_TAG . ETH1_VLAN_TAG_RANGE_LOWER
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_ETH1_FLOW_CFG_EGR2_ETH1_VLAN_TAG_RANGE_I_TAG_ETH1_VLAN_TAG_RANGE_LOWER(x)  VTSS_ENCODE_BITFIELD(x,0,12)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_ETH1_FLOW_CFG_EGR2_ETH1_VLAN_TAG_RANGE_I_TAG_ETH1_VLAN_TAG_RANGE_LOWER     VTSS_ENCODE_BITMASK(0,12)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_ETH1_FLOW_CFG_EGR2_ETH1_VLAN_TAG_RANGE_I_TAG_ETH1_VLAN_TAG_RANGE_LOWER(x)  VTSS_EXTRACT_BITFIELD(x,0,12)


/** 
 * \brief VLAN tag 1 match/mask
 *
 * \details
 * Register: \a PTP_ANA2_EGR_CFG23:EGR2_ETH1_FLOW_CFG:EGR2_ETH1_VLAN_TAG1
 *
 * @param gi Register: EGR2_ETH1_FLOW_CFG, 0-7
 */
#define VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH1_FLOW_CFG_EGR2_ETH1_VLAN_TAG1(gi)  VTSS_IOREG_IX(0x1e, 1, 0xc820, gi, 16, 0, 5)

/** 
 * \brief
 * Mask value for VLAN tag 1
 *
 * \details 
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH1_FLOW_CFG_EGR2_ETH1_VLAN_TAG1 . ETH1_VLAN_TAG1_MASK
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_ETH1_FLOW_CFG_EGR2_ETH1_VLAN_TAG1_ETH1_VLAN_TAG1_MASK(x)  VTSS_ENCODE_BITFIELD(x,16,12)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_ETH1_FLOW_CFG_EGR2_ETH1_VLAN_TAG1_ETH1_VLAN_TAG1_MASK     VTSS_ENCODE_BITMASK(16,12)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_ETH1_FLOW_CFG_EGR2_ETH1_VLAN_TAG1_ETH1_VLAN_TAG1_MASK(x)  VTSS_EXTRACT_BITFIELD(x,16,12)

/** 
 * \brief
 * Match value for the first VLAN tag
 *
 * \details 
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH1_FLOW_CFG_EGR2_ETH1_VLAN_TAG1 . ETH1_VLAN_TAG1_MATCH
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_ETH1_FLOW_CFG_EGR2_ETH1_VLAN_TAG1_ETH1_VLAN_TAG1_MATCH(x)  VTSS_ENCODE_BITFIELD(x,0,12)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_ETH1_FLOW_CFG_EGR2_ETH1_VLAN_TAG1_ETH1_VLAN_TAG1_MATCH     VTSS_ENCODE_BITMASK(0,12)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_ETH1_FLOW_CFG_EGR2_ETH1_VLAN_TAG1_ETH1_VLAN_TAG1_MATCH(x)  VTSS_EXTRACT_BITFIELD(x,0,12)


/** 
 * \brief Match/mask for VLAN tag 2 or I-tag match
 *
 * \details
 * Register: \a PTP_ANA2_EGR_CFG23:EGR2_ETH1_FLOW_CFG:EGR2_ETH1_VLAN_TAG2_I_TAG
 *
 * @param gi Register: EGR2_ETH1_FLOW_CFG, 0-7
 */
#define VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH1_FLOW_CFG_EGR2_ETH1_VLAN_TAG2_I_TAG(gi)  VTSS_IOREG_IX(0x1e, 1, 0xc820, gi, 16, 0, 6)

/** 
 * \brief
 * When PBB is not enabled, the mask field for VLAN tag 2
 * When PBB is enabled, the upper 12 bits of the I-tag mask
 *
 * \details 
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH1_FLOW_CFG_EGR2_ETH1_VLAN_TAG2_I_TAG . ETH1_VLAN_TAG2_MASK
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_ETH1_FLOW_CFG_EGR2_ETH1_VLAN_TAG2_I_TAG_ETH1_VLAN_TAG2_MASK(x)  VTSS_ENCODE_BITFIELD(x,16,12)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_ETH1_FLOW_CFG_EGR2_ETH1_VLAN_TAG2_I_TAG_ETH1_VLAN_TAG2_MASK     VTSS_ENCODE_BITMASK(16,12)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_ETH1_FLOW_CFG_EGR2_ETH1_VLAN_TAG2_I_TAG_ETH1_VLAN_TAG2_MASK(x)  VTSS_EXTRACT_BITFIELD(x,16,12)

/** 
 * \brief
 * When PBB is not enabled, the match field for VLAN Tag 2
 * When PBB is enabled, the lower 12 bits of the I-tag mask field
 *
 * \details 
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH1_FLOW_CFG_EGR2_ETH1_VLAN_TAG2_I_TAG . ETH1_VLAN_TAG2_MATCH
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_ETH1_FLOW_CFG_EGR2_ETH1_VLAN_TAG2_I_TAG_ETH1_VLAN_TAG2_MATCH(x)  VTSS_ENCODE_BITFIELD(x,0,12)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_ETH1_FLOW_CFG_EGR2_ETH1_VLAN_TAG2_I_TAG_ETH1_VLAN_TAG2_MATCH     VTSS_ENCODE_BITMASK(0,12)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_ETH1_FLOW_CFG_EGR2_ETH1_VLAN_TAG2_I_TAG_ETH1_VLAN_TAG2_MATCH(x)  VTSS_EXTRACT_BITFIELD(x,0,12)

/**
 * Register Group: \a PTP_ANA2_EGR_CFG23:EGR2_ETH2_NXT_PROTOCOL_A
 *
 * Ethernet next protocol configuration
 */


/** 
 * \brief Ethernet next protocol register
 *
 * \details
 * Register: \a PTP_ANA2_EGR_CFG23:EGR2_ETH2_NXT_PROTOCOL_A:EGR2_ETH2_NXT_PROTOCOL_A
 */
#define VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH2_NXT_PROTOCOL_A_EGR2_ETH2_NXT_PROTOCOL_A  VTSS_IOREG(0x1e, 1, 0xc8a0)

/** 
 * \brief
 * Frame signature offset.
 * Points to the start of the byte field in the Ethernet frame that will be
 * used for the frame signature.
 *
 * \details 
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH2_NXT_PROTOCOL_A_EGR2_ETH2_NXT_PROTOCOL_A . ETH2_FRAME_SIG_OFFSET_A
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_ETH2_NXT_PROTOCOL_A_EGR2_ETH2_NXT_PROTOCOL_A_ETH2_FRAME_SIG_OFFSET_A(x)  VTSS_ENCODE_BITFIELD(x,16,5)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_ETH2_NXT_PROTOCOL_A_EGR2_ETH2_NXT_PROTOCOL_A_ETH2_FRAME_SIG_OFFSET_A     VTSS_ENCODE_BITMASK(16,5)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_ETH2_NXT_PROTOCOL_A_EGR2_ETH2_NXT_PROTOCOL_A_ETH2_FRAME_SIG_OFFSET_A(x)  VTSS_EXTRACT_BITFIELD(x,16,5)

/** 
 * \brief
 * Points to the next comparator block after this Ethernet block. If this
 * comparator block is not used, this field must be set to 0.
 *
 * \details 
 * 0: Comparator block not used
 * 1: Ethernet comparator 2
 * 2: IP/UDP/ACH comparator 1
 * 3: IP/UDP/ACH comparator 2
 * 4: MPLS comparator
 * 5: PTP/OAM comparator
 * 6,7: Reserved
 *
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH2_NXT_PROTOCOL_A_EGR2_ETH2_NXT_PROTOCOL_A . ETH2_NXT_COMPARATOR_A
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_ETH2_NXT_PROTOCOL_A_EGR2_ETH2_NXT_PROTOCOL_A_ETH2_NXT_COMPARATOR_A(x)  VTSS_ENCODE_BITFIELD(x,0,3)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_ETH2_NXT_PROTOCOL_A_EGR2_ETH2_NXT_PROTOCOL_A_ETH2_NXT_COMPARATOR_A     VTSS_ENCODE_BITMASK(0,3)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_ETH2_NXT_PROTOCOL_A_EGR2_ETH2_NXT_PROTOCOL_A_ETH2_NXT_COMPARATOR_A(x)  VTSS_EXTRACT_BITFIELD(x,0,3)


/** 
 * \brief VLAN TPID configuration
 *
 * \details
 * Register: \a PTP_ANA2_EGR_CFG23:EGR2_ETH2_NXT_PROTOCOL_A:EGR2_ETH2_VLAN_TPID_CFG_A
 */
#define VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH2_NXT_PROTOCOL_A_EGR2_ETH2_VLAN_TPID_CFG_A  VTSS_IOREG(0x1e, 1, 0xc8a1)

/** 
 * \brief
 * Configurable S-tag TPID
 *
 * \details 
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH2_NXT_PROTOCOL_A_EGR2_ETH2_VLAN_TPID_CFG_A . ETH2_VLAN_TPID_CFG_A
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_ETH2_NXT_PROTOCOL_A_EGR2_ETH2_VLAN_TPID_CFG_A_ETH2_VLAN_TPID_CFG_A(x)  VTSS_ENCODE_BITFIELD(x,16,16)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_ETH2_NXT_PROTOCOL_A_EGR2_ETH2_VLAN_TPID_CFG_A_ETH2_VLAN_TPID_CFG_A     VTSS_ENCODE_BITMASK(16,16)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_ETH2_NXT_PROTOCOL_A_EGR2_ETH2_VLAN_TPID_CFG_A_ETH2_VLAN_TPID_CFG_A(x)  VTSS_EXTRACT_BITFIELD(x,16,16)


/** 
 * \brief Ethertype match register
 *
 * \details
 * Register: \a PTP_ANA2_EGR_CFG23:EGR2_ETH2_NXT_PROTOCOL_A:EGR2_ETH2_ETYPE_MATCH_A
 */
#define VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH2_NXT_PROTOCOL_A_EGR2_ETH2_ETYPE_MATCH_A  VTSS_IOREG(0x1e, 1, 0xc8a2)

/** 
 * \brief
 * Configure Ethertype verification
 *
 * \details 
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH2_NXT_PROTOCOL_A_EGR2_ETH2_ETYPE_MATCH_A . ETH2_ETYPE_MATCH_ENA_A
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_ETH2_NXT_PROTOCOL_A_EGR2_ETH2_ETYPE_MATCH_A_ETH2_ETYPE_MATCH_ENA_A  VTSS_BIT(16)

/** 
 * \brief
 * If the Ethertype/length field is an Ethertype, then this register is
 * compared against the value. If the field is a length, the length value
 * is not checked.
 *
 * \details 
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH2_NXT_PROTOCOL_A_EGR2_ETH2_ETYPE_MATCH_A . ETH2_ETYPE_MATCH_A
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_ETH2_NXT_PROTOCOL_A_EGR2_ETH2_ETYPE_MATCH_A_ETH2_ETYPE_MATCH_A(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_ETH2_NXT_PROTOCOL_A_EGR2_ETH2_ETYPE_MATCH_A_ETH2_ETYPE_MATCH_A     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_ETH2_NXT_PROTOCOL_A_EGR2_ETH2_ETYPE_MATCH_A_ETH2_ETYPE_MATCH_A(x)  VTSS_EXTRACT_BITFIELD(x,0,16)

/**
 * Register Group: \a PTP_ANA2_EGR_CFG23:EGR2_ETH2_NXT_PROTOCOL_B
 *
 * Ethernet next protocol configuration
 */


/** 
 * \brief Ethernet next protocol register
 *
 * \details
 * Register: \a PTP_ANA2_EGR_CFG23:EGR2_ETH2_NXT_PROTOCOL_B:EGR2_ETH2_NXT_PROTOCOL_B
 */
#define VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH2_NXT_PROTOCOL_B_EGR2_ETH2_NXT_PROTOCOL_B  VTSS_IOREG(0x1e, 1, 0xc8b0)


/** 
 * \brief VLAN TPID configuration
 *
 * \details
 * Register: \a PTP_ANA2_EGR_CFG23:EGR2_ETH2_NXT_PROTOCOL_B:EGR2_ETH2_VLAN_TPID_CFG_B
 */
#define VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH2_NXT_PROTOCOL_B_EGR2_ETH2_VLAN_TPID_CFG_B  VTSS_IOREG(0x1e, 1, 0xc8b1)


/** 
 * \brief Ethertype match register
 *
 * \details
 * Register: \a PTP_ANA2_EGR_CFG23:EGR2_ETH2_NXT_PROTOCOL_B:EGR2_ETH2_ETYPE_MATCH_B
 */
#define VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH2_NXT_PROTOCOL_B_EGR2_ETH2_ETYPE_MATCH_B  VTSS_IOREG(0x1e, 1, 0xc8b2)

/**
 * Register Group: \a PTP_ANA2_EGR_CFG23:EGR2_ETH2_FLOW_CFG
 *
 * Ethernet flow configuration
 */


/** 
 * \brief Ethernet flow enable
 *
 * \details
 * Register: \a PTP_ANA2_EGR_CFG23:EGR2_ETH2_FLOW_CFG:EGR2_ETH2_FLOW_ENABLE
 *
 * @param gi Register: EGR2_ETH2_FLOW_CFG, 0-7
 */
#define VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH2_FLOW_CFG_EGR2_ETH2_FLOW_ENABLE(gi)  VTSS_IOREG_IX(0x1e, 1, 0xc8c0, gi, 16, 0, 0)

/** 
 * \brief
 * Channel mask
 *
 * \details 
 * 0: Flow valid for channel 0
 * 1: Flow valid for channel 1
 *
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH2_FLOW_CFG_EGR2_ETH2_FLOW_ENABLE . ETH2_CHANNEL_MASK
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_ETH2_FLOW_CFG_EGR2_ETH2_FLOW_ENABLE_ETH2_CHANNEL_MASK(x)  VTSS_ENCODE_BITFIELD(x,8,2)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_ETH2_FLOW_CFG_EGR2_ETH2_FLOW_ENABLE_ETH2_CHANNEL_MASK     VTSS_ENCODE_BITMASK(8,2)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_ETH2_FLOW_CFG_EGR2_ETH2_FLOW_ENABLE_ETH2_CHANNEL_MASK(x)  VTSS_EXTRACT_BITFIELD(x,8,2)

/** 
 * \brief
 * Flow enable
 * If this comparator block is not used, all flow enable bits must be set
 * to 0.
 *
 * \details 
 * 0: Flow disabled
 * 1: Flow enabled
 *
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH2_FLOW_CFG_EGR2_ETH2_FLOW_ENABLE . ETH2_FLOW_ENABLE
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_ETH2_FLOW_CFG_EGR2_ETH2_FLOW_ENABLE_ETH2_FLOW_ENABLE  VTSS_BIT(0)


/** 
 * \brief Ethernet protocol match mode
 *
 * \details
 * Register: \a PTP_ANA2_EGR_CFG23:EGR2_ETH2_FLOW_CFG:EGR2_ETH2_MATCH_MODE
 *
 * @param gi Register: EGR2_ETH2_FLOW_CFG, 0-7
 */
#define VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH2_FLOW_CFG_EGR2_ETH2_MATCH_MODE(gi)  VTSS_IOREG_IX(0x1e, 1, 0xc8c0, gi, 16, 0, 1)

/** 
 * \brief
 * VLAN tag mode configuration
 *
 * \details 
 * 0: VLAN range checking disabled
 * 1: VLAN range checking on tag 1
 * 2: VLAN range checking on tag 2 (not supported with PBB)
 * 3: Reserved
 *
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH2_FLOW_CFG_EGR2_ETH2_MATCH_MODE . ETH2_VLAN_TAG_MODE
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_ETH2_FLOW_CFG_EGR2_ETH2_MATCH_MODE_ETH2_VLAN_TAG_MODE(x)  VTSS_ENCODE_BITFIELD(x,12,2)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_ETH2_FLOW_CFG_EGR2_ETH2_MATCH_MODE_ETH2_VLAN_TAG_MODE     VTSS_ENCODE_BITMASK(12,2)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_ETH2_FLOW_CFG_EGR2_ETH2_MATCH_MODE_ETH2_VLAN_TAG_MODE(x)  VTSS_EXTRACT_BITFIELD(x,12,2)

/** 
 * \brief
 * This register is only used if ETH1_VLAN_VERIFY_ENA = 1
 *
 * \details 
 * 0: C tag (TPID of 0x8100)
 * 1: S tag (match to CONF_VLAN_TPID)
 *
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH2_FLOW_CFG_EGR2_ETH2_MATCH_MODE . ETH2_VLAN_TAG2_TYPE
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_ETH2_FLOW_CFG_EGR2_ETH2_MATCH_MODE_ETH2_VLAN_TAG2_TYPE  VTSS_BIT(9)

/** 
 * \brief
 * This register is only used if ETH1_VLAN_VERIFY_ENA = 1
 *
 * \details 
 * 0: C tag (TPID of 0x8100)
 * 1: S or B tag (match to CONF_VLAN_TPID)
 *
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH2_FLOW_CFG_EGR2_ETH2_MATCH_MODE . ETH2_VLAN_TAG1_TYPE
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_ETH2_FLOW_CFG_EGR2_ETH2_MATCH_MODE_ETH2_VLAN_TAG1_TYPE  VTSS_BIT(8)

/** 
 * \brief
 * This register is only used if ETH2_VLAN_VERIFY_ENA = 1
 *
 * \details 
 * 0: No VLAN tags
 * 1: 1 VLAN tag
 * 2: 2 VLAN tags
 * 3: Reserved
 *
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH2_FLOW_CFG_EGR2_ETH2_MATCH_MODE . ETH2_VLAN_TAGS
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_ETH2_FLOW_CFG_EGR2_ETH2_MATCH_MODE_ETH2_VLAN_TAGS(x)  VTSS_ENCODE_BITFIELD(x,6,2)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_ETH2_FLOW_CFG_EGR2_ETH2_MATCH_MODE_ETH2_VLAN_TAGS     VTSS_ENCODE_BITMASK(6,2)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_ETH2_FLOW_CFG_EGR2_ETH2_MATCH_MODE_ETH2_VLAN_TAGS(x)  VTSS_EXTRACT_BITFIELD(x,6,2)

/** 
 * \brief
 * Verify VLAN tags
 *
 * \details 
 * 0: Parse for VLAN tags, do not check values
 * 1: Verify configured VLAN tag configuration
 *
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH2_FLOW_CFG_EGR2_ETH2_MATCH_MODE . ETH2_VLAN_VERIFY_ENA
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_ETH2_FLOW_CFG_EGR2_ETH2_MATCH_MODE_ETH2_VLAN_VERIFY_ENA  VTSS_BIT(4)

/** 
 * \brief
 * When checking for presence of SNAP/LLC based upon ETH1_MATCH_MODE, this
 * field indicates if SNAP and 3-byte LLC is expected to be present.
 *
 * \details 
 * 0: Only Ethernet type II supported, no SNAP/LLC
 * 1: Ethernet type II and Ethernet type I with SNAP/LLC, determine if
 * SNAP/LLC is present. Type I always assumes that SNAP/LLC is present
 *
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH2_FLOW_CFG_EGR2_ETH2_MATCH_MODE . ETH2_ETHERTYPE_MODE
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_ETH2_FLOW_CFG_EGR2_ETH2_MATCH_MODE_ETH2_ETHERTYPE_MODE  VTSS_BIT(0)


/** 
 * \brief Ethernet address match part 1
 *
 * \details
 * Register: \a PTP_ANA2_EGR_CFG23:EGR2_ETH2_FLOW_CFG:EGR2_ETH2_ADDR_MATCH_1
 *
 * @param gi Register: EGR2_ETH2_FLOW_CFG, 0-7
 */
#define VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH2_FLOW_CFG_EGR2_ETH2_ADDR_MATCH_1(gi)  VTSS_IOREG_IX(0x1e, 1, 0xc8c0, gi, 16, 0, 2)


/** 
 * \brief Ethernet address match part 2
 *
 * \details
 * Register: \a PTP_ANA2_EGR_CFG23:EGR2_ETH2_FLOW_CFG:EGR2_ETH2_ADDR_MATCH_2
 *
 * @param gi Register: EGR2_ETH2_FLOW_CFG, 0-7
 */
#define VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH2_FLOW_CFG_EGR2_ETH2_ADDR_MATCH_2(gi)  VTSS_IOREG_IX(0x1e, 1, 0xc8c0, gi, 16, 0, 3)

/** 
 * \brief
 * Selects how the addresses are matched. Multiple bits can be set at once.
 *
 * \details 
 * 0: Full 48-bit address match
 * 1: Match any unicast address
 * 2: Match any muliticast address
 *
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH2_FLOW_CFG_EGR2_ETH2_ADDR_MATCH_2 . ETH2_ADDR_MATCH_MODE
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_ETH2_FLOW_CFG_EGR2_ETH2_ADDR_MATCH_2_ETH2_ADDR_MATCH_MODE(x)  VTSS_ENCODE_BITFIELD(x,20,3)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_ETH2_FLOW_CFG_EGR2_ETH2_ADDR_MATCH_2_ETH2_ADDR_MATCH_MODE     VTSS_ENCODE_BITMASK(20,3)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_ETH2_FLOW_CFG_EGR2_ETH2_ADDR_MATCH_2_ETH2_ADDR_MATCH_MODE(x)  VTSS_EXTRACT_BITFIELD(x,20,3)

/** 
 * \brief
 * Selects which address to match
 *
 * \details 
 * 0: Match the destination address
 * 1: Match the source address
 * 2: Match either the source of destination address
 * 3: Reserved
 *
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH2_FLOW_CFG_EGR2_ETH2_ADDR_MATCH_2 . ETH2_ADDR_MATCH_SELECT
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_ETH2_FLOW_CFG_EGR2_ETH2_ADDR_MATCH_2_ETH2_ADDR_MATCH_SELECT(x)  VTSS_ENCODE_BITFIELD(x,16,2)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_ETH2_FLOW_CFG_EGR2_ETH2_ADDR_MATCH_2_ETH2_ADDR_MATCH_SELECT     VTSS_ENCODE_BITMASK(16,2)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_ETH2_FLOW_CFG_EGR2_ETH2_ADDR_MATCH_2_ETH2_ADDR_MATCH_SELECT(x)  VTSS_EXTRACT_BITFIELD(x,16,2)

/** 
 * \brief
 * Last 16 bits of the Ethernet address match field
 *
 * \details 
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH2_FLOW_CFG_EGR2_ETH2_ADDR_MATCH_2 . ETH2_ADDR_MATCH_2
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_ETH2_FLOW_CFG_EGR2_ETH2_ADDR_MATCH_2_ETH2_ADDR_MATCH_2(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_ETH2_FLOW_CFG_EGR2_ETH2_ADDR_MATCH_2_ETH2_ADDR_MATCH_2     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_ETH2_FLOW_CFG_EGR2_ETH2_ADDR_MATCH_2_ETH2_ADDR_MATCH_2(x)  VTSS_EXTRACT_BITFIELD(x,0,16)


/** 
 * \brief Ethernet VLAN tag range match register
 *
 * \details
 * Register: \a PTP_ANA2_EGR_CFG23:EGR2_ETH2_FLOW_CFG:EGR2_ETH2_VLAN_TAG_RANGE_I_TAG
 *
 * @param gi Register: EGR2_ETH2_FLOW_CFG, 0-7
 */
#define VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH2_FLOW_CFG_EGR2_ETH2_VLAN_TAG_RANGE_I_TAG(gi)  VTSS_IOREG_IX(0x1e, 1, 0xc8c0, gi, 16, 0, 4)

/** 
 * \brief
 * Contains the upper range of the VLAN tag range match
 *
 * \details 
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH2_FLOW_CFG_EGR2_ETH2_VLAN_TAG_RANGE_I_TAG . ETH2_VLAN_TAG_RANGE_UPPER
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_ETH2_FLOW_CFG_EGR2_ETH2_VLAN_TAG_RANGE_I_TAG_ETH2_VLAN_TAG_RANGE_UPPER(x)  VTSS_ENCODE_BITFIELD(x,16,12)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_ETH2_FLOW_CFG_EGR2_ETH2_VLAN_TAG_RANGE_I_TAG_ETH2_VLAN_TAG_RANGE_UPPER     VTSS_ENCODE_BITMASK(16,12)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_ETH2_FLOW_CFG_EGR2_ETH2_VLAN_TAG_RANGE_I_TAG_ETH2_VLAN_TAG_RANGE_UPPER(x)  VTSS_EXTRACT_BITFIELD(x,16,12)

/** 
 * \brief
 * Contains the lower range of the VLAN tag range match
 *
 * \details 
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH2_FLOW_CFG_EGR2_ETH2_VLAN_TAG_RANGE_I_TAG . ETH2_VLAN_TAG_RANGE_LOWER
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_ETH2_FLOW_CFG_EGR2_ETH2_VLAN_TAG_RANGE_I_TAG_ETH2_VLAN_TAG_RANGE_LOWER(x)  VTSS_ENCODE_BITFIELD(x,0,12)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_ETH2_FLOW_CFG_EGR2_ETH2_VLAN_TAG_RANGE_I_TAG_ETH2_VLAN_TAG_RANGE_LOWER     VTSS_ENCODE_BITMASK(0,12)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_ETH2_FLOW_CFG_EGR2_ETH2_VLAN_TAG_RANGE_I_TAG_ETH2_VLAN_TAG_RANGE_LOWER(x)  VTSS_EXTRACT_BITFIELD(x,0,12)


/** 
 * \brief VLAN tag 1 match/mask
 *
 * \details
 * Register: \a PTP_ANA2_EGR_CFG23:EGR2_ETH2_FLOW_CFG:EGR2_ETH2_VLAN_TAG1
 *
 * @param gi Register: EGR2_ETH2_FLOW_CFG, 0-7
 */
#define VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH2_FLOW_CFG_EGR2_ETH2_VLAN_TAG1(gi)  VTSS_IOREG_IX(0x1e, 1, 0xc8c0, gi, 16, 0, 5)

/** 
 * \brief
 * Mask value for VLAN tag 1
 *
 * \details 
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH2_FLOW_CFG_EGR2_ETH2_VLAN_TAG1 . ETH2_VLAN_TAG1_MASK
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_ETH2_FLOW_CFG_EGR2_ETH2_VLAN_TAG1_ETH2_VLAN_TAG1_MASK(x)  VTSS_ENCODE_BITFIELD(x,16,12)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_ETH2_FLOW_CFG_EGR2_ETH2_VLAN_TAG1_ETH2_VLAN_TAG1_MASK     VTSS_ENCODE_BITMASK(16,12)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_ETH2_FLOW_CFG_EGR2_ETH2_VLAN_TAG1_ETH2_VLAN_TAG1_MASK(x)  VTSS_EXTRACT_BITFIELD(x,16,12)

/** 
 * \brief
 * Match value for the first VLAN tag
 *
 * \details 
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH2_FLOW_CFG_EGR2_ETH2_VLAN_TAG1 . ETH2_VLAN_TAG1_MATCH
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_ETH2_FLOW_CFG_EGR2_ETH2_VLAN_TAG1_ETH2_VLAN_TAG1_MATCH(x)  VTSS_ENCODE_BITFIELD(x,0,12)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_ETH2_FLOW_CFG_EGR2_ETH2_VLAN_TAG1_ETH2_VLAN_TAG1_MATCH     VTSS_ENCODE_BITMASK(0,12)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_ETH2_FLOW_CFG_EGR2_ETH2_VLAN_TAG1_ETH2_VLAN_TAG1_MATCH(x)  VTSS_EXTRACT_BITFIELD(x,0,12)


/** 
 * \brief Match/mask for VLAN tag 2 or I-tag match
 *
 * \details
 * Register: \a PTP_ANA2_EGR_CFG23:EGR2_ETH2_FLOW_CFG:EGR2_ETH2_VLAN_TAG2_I_TAG
 *
 * @param gi Register: EGR2_ETH2_FLOW_CFG, 0-7
 */
#define VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH2_FLOW_CFG_EGR2_ETH2_VLAN_TAG2_I_TAG(gi)  VTSS_IOREG_IX(0x1e, 1, 0xc8c0, gi, 16, 0, 6)

/** 
 * \brief
 * Mask field for VLAN tag 2
 *
 * \details 
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH2_FLOW_CFG_EGR2_ETH2_VLAN_TAG2_I_TAG . ETH2_VLAN_TAG2_MASK
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_ETH2_FLOW_CFG_EGR2_ETH2_VLAN_TAG2_I_TAG_ETH2_VLAN_TAG2_MASK(x)  VTSS_ENCODE_BITFIELD(x,16,12)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_ETH2_FLOW_CFG_EGR2_ETH2_VLAN_TAG2_I_TAG_ETH2_VLAN_TAG2_MASK     VTSS_ENCODE_BITMASK(16,12)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_ETH2_FLOW_CFG_EGR2_ETH2_VLAN_TAG2_I_TAG_ETH2_VLAN_TAG2_MASK(x)  VTSS_EXTRACT_BITFIELD(x,16,12)

/** 
 * \brief
 * Match field for VLAN Tag 2
 *
 * \details 
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_ETH2_FLOW_CFG_EGR2_ETH2_VLAN_TAG2_I_TAG . ETH2_VLAN_TAG2_MATCH
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_ETH2_FLOW_CFG_EGR2_ETH2_VLAN_TAG2_I_TAG_ETH2_VLAN_TAG2_MATCH(x)  VTSS_ENCODE_BITFIELD(x,0,12)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_ETH2_FLOW_CFG_EGR2_ETH2_VLAN_TAG2_I_TAG_ETH2_VLAN_TAG2_MATCH     VTSS_ENCODE_BITMASK(0,12)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_ETH2_FLOW_CFG_EGR2_ETH2_VLAN_TAG2_I_TAG_ETH2_VLAN_TAG2_MATCH(x)  VTSS_EXTRACT_BITFIELD(x,0,12)

/**
 * Register Group: \a PTP_ANA2_EGR_CFG23:EGR2_MPLS_NXT_COMPARATOR_A
 *
 * MPLS next protocol register
 */


/** 
 * \brief MPLS next protocol comparator register
 *
 * \details
 * Register: \a PTP_ANA2_EGR_CFG23:EGR2_MPLS_NXT_COMPARATOR_A:EGR2_MPLS_NXT_COMPARATOR_A
 */
#define VTSS_PTP_ANA2_EGR_CFG23_EGR2_MPLS_NXT_COMPARATOR_A_EGR2_MPLS_NXT_COMPARATOR_A  VTSS_IOREG(0x1e, 1, 0xc940)

/** 
 * \brief
 * Indicates the presence of a control word after the last label
 *
 * \details 
 * 0: No control ward after the last label
 * 1: Control word after the last label
 *
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_MPLS_NXT_COMPARATOR_A_EGR2_MPLS_NXT_COMPARATOR_A . MPLS_CTL_WORD_A
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_MPLS_NXT_COMPARATOR_A_EGR2_MPLS_NXT_COMPARATOR_A_MPLS_CTL_WORD_A  VTSS_BIT(16)

/** 
 * \brief
 * Points to the next comparator stage. If this comparator block is not
 * used, this field must be set to 0.
 *
 * \details 
 * 0: Comparator block not used
 * 1: Ethernet comparator 2
 * 2: IP/UDP/ACH comparator 1
 * 3: IP/UDP/ACH comparator 2
 * 4: Reserved
 * 5: PTP/OAM comparator
 * 6,7: Reserved
 *
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_MPLS_NXT_COMPARATOR_A_EGR2_MPLS_NXT_COMPARATOR_A . MPLS_NXT_COMPARATOR_A
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_MPLS_NXT_COMPARATOR_A_EGR2_MPLS_NXT_COMPARATOR_A_MPLS_NXT_COMPARATOR_A(x)  VTSS_ENCODE_BITFIELD(x,0,3)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_MPLS_NXT_COMPARATOR_A_EGR2_MPLS_NXT_COMPARATOR_A_MPLS_NXT_COMPARATOR_A     VTSS_ENCODE_BITMASK(0,3)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_MPLS_NXT_COMPARATOR_A_EGR2_MPLS_NXT_COMPARATOR_A_MPLS_NXT_COMPARATOR_A(x)  VTSS_EXTRACT_BITFIELD(x,0,3)

/**
 * Register Group: \a PTP_ANA2_EGR_CFG23:EGR2_MPLS_NXT_COMPARATOR_B
 *
 * MPLS Next Protocol Register
 */


/** 
 * \brief MPLS Next Protocol Comparator Register
 *
 * \details
 * Register: \a PTP_ANA2_EGR_CFG23:EGR2_MPLS_NXT_COMPARATOR_B:EGR2_MPLS_NXT_COMPARATOR_B
 */
#define VTSS_PTP_ANA2_EGR_CFG23_EGR2_MPLS_NXT_COMPARATOR_B_EGR2_MPLS_NXT_COMPARATOR_B  VTSS_IOREG(0x1e, 1, 0xc950)

/**
 * Register Group: \a PTP_ANA2_EGR_CFG23:EGR2_MPLS_FLOW_CFG
 *
 * MPLS flow configuration
 */


/** 
 * \brief MPLS flow control register
 *
 * \details
 * Register: \a PTP_ANA2_EGR_CFG23:EGR2_MPLS_FLOW_CFG:EGR2_MPLS_FLOW_CONTROL
 *
 * @param gi Register: EGR2_MPLS_FLOW_CFG, 0-7
 */
#define VTSS_PTP_ANA2_EGR_CFG23_EGR2_MPLS_FLOW_CFG_EGR2_MPLS_FLOW_CONTROL(gi)  VTSS_IOREG_IX(0x1e, 1, 0xc960, gi, 16, 0, 0)

/** 
 * \brief
 * MPLS channel mask selector
 *
 * \details 
 * 0: Flow valid for channel 0
 * 1: Flow valid for channel 1
 *
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_MPLS_FLOW_CFG_EGR2_MPLS_FLOW_CONTROL . MPLS_CHANNEL_MASK
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_MPLS_FLOW_CFG_EGR2_MPLS_FLOW_CONTROL_MPLS_CHANNEL_MASK(x)  VTSS_ENCODE_BITFIELD(x,24,2)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_MPLS_FLOW_CFG_EGR2_MPLS_FLOW_CONTROL_MPLS_CHANNEL_MASK     VTSS_ENCODE_BITMASK(24,2)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_MPLS_FLOW_CFG_EGR2_MPLS_FLOW_CONTROL_MPLS_CHANNEL_MASK(x)  VTSS_EXTRACT_BITFIELD(x,24,2)

/** 
 * \brief
 * Defines the allowable stack depths for searches. The direction that the
 * stack is referenced is determined by the setting of MPLS_REF_PNT. The
 * following table maps bits to stack depths:
 *
 * \details 
 * 0: Stack allowed to be 1 label deep
 * 1: Stack allowed to be 2 labels deep
 * 2: Stack allowed to be 3 labels deep
 * 3: Stack allowed to be 4 labels deep
 *
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_MPLS_FLOW_CFG_EGR2_MPLS_FLOW_CONTROL . MPLS_STACK_DEPTH
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_MPLS_FLOW_CFG_EGR2_MPLS_FLOW_CONTROL_MPLS_STACK_DEPTH(x)  VTSS_ENCODE_BITFIELD(x,16,4)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_MPLS_FLOW_CFG_EGR2_MPLS_FLOW_CONTROL_MPLS_STACK_DEPTH     VTSS_ENCODE_BITMASK(16,4)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_MPLS_FLOW_CFG_EGR2_MPLS_FLOW_CONTROL_MPLS_STACK_DEPTH(x)  VTSS_EXTRACT_BITFIELD(x,16,4)

/** 
 * \brief
 * Defines the search direction for label matching
 *
 * \details 
 * 0: All searching is performed starting from the top of the stack
 * 1: All searching is performed from the end of the stack
 *
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_MPLS_FLOW_CFG_EGR2_MPLS_FLOW_CONTROL . MPLS_REF_PNT
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_MPLS_FLOW_CFG_EGR2_MPLS_FLOW_CONTROL_MPLS_REF_PNT  VTSS_BIT(4)

/** 
 * \brief
 * Flow enable
 * If this comparator block is not used, all flow enable bits must be set
 * to 0.
 *
 * \details 
 * 0: Flow is disabled
 * 1: Flow is enabled
 *
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_MPLS_FLOW_CFG_EGR2_MPLS_FLOW_CONTROL . MPLS_FLOW_ENA
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_MPLS_FLOW_CFG_EGR2_MPLS_FLOW_CONTROL_MPLS_FLOW_ENA  VTSS_BIT(0)


/** 
 * \brief MPLS label 0 match range lower value
 *
 * \details
 * Register: \a PTP_ANA2_EGR_CFG23:EGR2_MPLS_FLOW_CFG:EGR2_MPLS_LABEL_RANGE_LOWER_0
 *
 * @param gi Register: EGR2_MPLS_FLOW_CFG, 0-7
 */
#define VTSS_PTP_ANA2_EGR_CFG23_EGR2_MPLS_FLOW_CFG_EGR2_MPLS_LABEL_RANGE_LOWER_0(gi)  VTSS_IOREG_IX(0x1e, 1, 0xc960, gi, 16, 0, 1)

/** 
 * \brief
 * Lower value for label 0 match range
 *
 * \details 
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_MPLS_FLOW_CFG_EGR2_MPLS_LABEL_RANGE_LOWER_0 . MPLS_LABEL_RANGE_LOWER_0
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_MPLS_FLOW_CFG_EGR2_MPLS_LABEL_RANGE_LOWER_0_MPLS_LABEL_RANGE_LOWER_0(x)  VTSS_ENCODE_BITFIELD(x,0,20)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_MPLS_FLOW_CFG_EGR2_MPLS_LABEL_RANGE_LOWER_0_MPLS_LABEL_RANGE_LOWER_0     VTSS_ENCODE_BITMASK(0,20)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_MPLS_FLOW_CFG_EGR2_MPLS_LABEL_RANGE_LOWER_0_MPLS_LABEL_RANGE_LOWER_0(x)  VTSS_EXTRACT_BITFIELD(x,0,20)


/** 
 * \brief MPLS label 0 match range upper value
 *
 * \details
 * Register: \a PTP_ANA2_EGR_CFG23:EGR2_MPLS_FLOW_CFG:EGR2_MPLS_LABEL_RANGE_UPPER_0
 *
 * @param gi Register: EGR2_MPLS_FLOW_CFG, 0-7
 */
#define VTSS_PTP_ANA2_EGR_CFG23_EGR2_MPLS_FLOW_CFG_EGR2_MPLS_LABEL_RANGE_UPPER_0(gi)  VTSS_IOREG_IX(0x1e, 1, 0xc960, gi, 16, 0, 2)

/** 
 * \brief
 * Upper value for label 0 match range
 *
 * \details 
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_MPLS_FLOW_CFG_EGR2_MPLS_LABEL_RANGE_UPPER_0 . MPLS_LABEL_RANGE_UPPER_0
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_MPLS_FLOW_CFG_EGR2_MPLS_LABEL_RANGE_UPPER_0_MPLS_LABEL_RANGE_UPPER_0(x)  VTSS_ENCODE_BITFIELD(x,0,20)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_MPLS_FLOW_CFG_EGR2_MPLS_LABEL_RANGE_UPPER_0_MPLS_LABEL_RANGE_UPPER_0     VTSS_ENCODE_BITMASK(0,20)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_MPLS_FLOW_CFG_EGR2_MPLS_LABEL_RANGE_UPPER_0_MPLS_LABEL_RANGE_UPPER_0(x)  VTSS_EXTRACT_BITFIELD(x,0,20)


/** 
 * \brief MPLS label 1 match range lower value
 *
 * \details
 * Register: \a PTP_ANA2_EGR_CFG23:EGR2_MPLS_FLOW_CFG:EGR2_MPLS_LABEL_RANGE_LOWER_1
 *
 * @param gi Register: EGR2_MPLS_FLOW_CFG, 0-7
 */
#define VTSS_PTP_ANA2_EGR_CFG23_EGR2_MPLS_FLOW_CFG_EGR2_MPLS_LABEL_RANGE_LOWER_1(gi)  VTSS_IOREG_IX(0x1e, 1, 0xc960, gi, 16, 0, 3)

/** 
 * \brief
 * Lower value for label 1 match range
 *
 * \details 
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_MPLS_FLOW_CFG_EGR2_MPLS_LABEL_RANGE_LOWER_1 . MPLS_LABEL_RANGE_LOWER_1
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_MPLS_FLOW_CFG_EGR2_MPLS_LABEL_RANGE_LOWER_1_MPLS_LABEL_RANGE_LOWER_1(x)  VTSS_ENCODE_BITFIELD(x,0,20)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_MPLS_FLOW_CFG_EGR2_MPLS_LABEL_RANGE_LOWER_1_MPLS_LABEL_RANGE_LOWER_1     VTSS_ENCODE_BITMASK(0,20)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_MPLS_FLOW_CFG_EGR2_MPLS_LABEL_RANGE_LOWER_1_MPLS_LABEL_RANGE_LOWER_1(x)  VTSS_EXTRACT_BITFIELD(x,0,20)


/** 
 * \brief MPLS label 1 match range upper value
 *
 * \details
 * Register: \a PTP_ANA2_EGR_CFG23:EGR2_MPLS_FLOW_CFG:EGR2_MPLS_LABEL_RANGE_UPPER_1
 *
 * @param gi Register: EGR2_MPLS_FLOW_CFG, 0-7
 */
#define VTSS_PTP_ANA2_EGR_CFG23_EGR2_MPLS_FLOW_CFG_EGR2_MPLS_LABEL_RANGE_UPPER_1(gi)  VTSS_IOREG_IX(0x1e, 1, 0xc960, gi, 16, 0, 4)

/** 
 * \brief
 * Upper value for label 1 match range
 *
 * \details 
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_MPLS_FLOW_CFG_EGR2_MPLS_LABEL_RANGE_UPPER_1 . MPLS_LABEL_RANGE_UPPER_1
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_MPLS_FLOW_CFG_EGR2_MPLS_LABEL_RANGE_UPPER_1_MPLS_LABEL_RANGE_UPPER_1(x)  VTSS_ENCODE_BITFIELD(x,0,20)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_MPLS_FLOW_CFG_EGR2_MPLS_LABEL_RANGE_UPPER_1_MPLS_LABEL_RANGE_UPPER_1     VTSS_ENCODE_BITMASK(0,20)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_MPLS_FLOW_CFG_EGR2_MPLS_LABEL_RANGE_UPPER_1_MPLS_LABEL_RANGE_UPPER_1(x)  VTSS_EXTRACT_BITFIELD(x,0,20)


/** 
 * \brief MPLS label 2 match range lower value
 *
 * \details
 * Register: \a PTP_ANA2_EGR_CFG23:EGR2_MPLS_FLOW_CFG:EGR2_MPLS_LABEL_RANGE_LOWER_2
 *
 * @param gi Register: EGR2_MPLS_FLOW_CFG, 0-7
 */
#define VTSS_PTP_ANA2_EGR_CFG23_EGR2_MPLS_FLOW_CFG_EGR2_MPLS_LABEL_RANGE_LOWER_2(gi)  VTSS_IOREG_IX(0x1e, 1, 0xc960, gi, 16, 0, 5)

/** 
 * \brief
 * Lower value for label 2 match range
 *
 * \details 
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_MPLS_FLOW_CFG_EGR2_MPLS_LABEL_RANGE_LOWER_2 . MPLS_LABEL_RANGE_LOWER_2
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_MPLS_FLOW_CFG_EGR2_MPLS_LABEL_RANGE_LOWER_2_MPLS_LABEL_RANGE_LOWER_2(x)  VTSS_ENCODE_BITFIELD(x,0,20)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_MPLS_FLOW_CFG_EGR2_MPLS_LABEL_RANGE_LOWER_2_MPLS_LABEL_RANGE_LOWER_2     VTSS_ENCODE_BITMASK(0,20)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_MPLS_FLOW_CFG_EGR2_MPLS_LABEL_RANGE_LOWER_2_MPLS_LABEL_RANGE_LOWER_2(x)  VTSS_EXTRACT_BITFIELD(x,0,20)


/** 
 * \brief MPLS label 2 match range upper value
 *
 * \details
 * Register: \a PTP_ANA2_EGR_CFG23:EGR2_MPLS_FLOW_CFG:EGR2_MPLS_LABEL_RANGE_UPPER_2
 *
 * @param gi Register: EGR2_MPLS_FLOW_CFG, 0-7
 */
#define VTSS_PTP_ANA2_EGR_CFG23_EGR2_MPLS_FLOW_CFG_EGR2_MPLS_LABEL_RANGE_UPPER_2(gi)  VTSS_IOREG_IX(0x1e, 1, 0xc960, gi, 16, 0, 6)

/** 
 * \brief
 * Upper value for label 2 match range
 *
 * \details 
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_MPLS_FLOW_CFG_EGR2_MPLS_LABEL_RANGE_UPPER_2 . MPLS_LABEL_RANGE_UPPER_2
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_MPLS_FLOW_CFG_EGR2_MPLS_LABEL_RANGE_UPPER_2_MPLS_LABEL_RANGE_UPPER_2(x)  VTSS_ENCODE_BITFIELD(x,0,20)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_MPLS_FLOW_CFG_EGR2_MPLS_LABEL_RANGE_UPPER_2_MPLS_LABEL_RANGE_UPPER_2     VTSS_ENCODE_BITMASK(0,20)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_MPLS_FLOW_CFG_EGR2_MPLS_LABEL_RANGE_UPPER_2_MPLS_LABEL_RANGE_UPPER_2(x)  VTSS_EXTRACT_BITFIELD(x,0,20)


/** 
 * \brief MPLS label 3 match range lower value
 *
 * \details
 * Register: \a PTP_ANA2_EGR_CFG23:EGR2_MPLS_FLOW_CFG:EGR2_MPLS_LABEL_RANGE_LOWER_3
 *
 * @param gi Register: EGR2_MPLS_FLOW_CFG, 0-7
 */
#define VTSS_PTP_ANA2_EGR_CFG23_EGR2_MPLS_FLOW_CFG_EGR2_MPLS_LABEL_RANGE_LOWER_3(gi)  VTSS_IOREG_IX(0x1e, 1, 0xc960, gi, 16, 0, 7)

/** 
 * \brief
 * Lower value for label 3 match range
 *
 * \details 
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_MPLS_FLOW_CFG_EGR2_MPLS_LABEL_RANGE_LOWER_3 . MPLS_LABEL_RANGE_LOWER_3
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_MPLS_FLOW_CFG_EGR2_MPLS_LABEL_RANGE_LOWER_3_MPLS_LABEL_RANGE_LOWER_3(x)  VTSS_ENCODE_BITFIELD(x,0,20)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_MPLS_FLOW_CFG_EGR2_MPLS_LABEL_RANGE_LOWER_3_MPLS_LABEL_RANGE_LOWER_3     VTSS_ENCODE_BITMASK(0,20)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_MPLS_FLOW_CFG_EGR2_MPLS_LABEL_RANGE_LOWER_3_MPLS_LABEL_RANGE_LOWER_3(x)  VTSS_EXTRACT_BITFIELD(x,0,20)


/** 
 * \brief MPLS label 3 match range upper value
 *
 * \details
 * Register: \a PTP_ANA2_EGR_CFG23:EGR2_MPLS_FLOW_CFG:EGR2_MPLS_LABEL_RANGE_UPPER_3
 *
 * @param gi Register: EGR2_MPLS_FLOW_CFG, 0-7
 */
#define VTSS_PTP_ANA2_EGR_CFG23_EGR2_MPLS_FLOW_CFG_EGR2_MPLS_LABEL_RANGE_UPPER_3(gi)  VTSS_IOREG_IX(0x1e, 1, 0xc960, gi, 16, 0, 8)

/** 
 * \brief
 * Upper value for label 3 match range
 *
 * \details 
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_MPLS_FLOW_CFG_EGR2_MPLS_LABEL_RANGE_UPPER_3 . MPLS_LABEL_RANGE_UPPER_3
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_MPLS_FLOW_CFG_EGR2_MPLS_LABEL_RANGE_UPPER_3_MPLS_LABEL_RANGE_UPPER_3(x)  VTSS_ENCODE_BITFIELD(x,0,20)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_MPLS_FLOW_CFG_EGR2_MPLS_LABEL_RANGE_UPPER_3_MPLS_LABEL_RANGE_UPPER_3     VTSS_ENCODE_BITMASK(0,20)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_MPLS_FLOW_CFG_EGR2_MPLS_LABEL_RANGE_UPPER_3_MPLS_LABEL_RANGE_UPPER_3(x)  VTSS_EXTRACT_BITFIELD(x,0,20)

/**
 * Register Group: \a PTP_ANA2_EGR_CFG23:EGR2_PTP_FLOW
 *
 * PTP flow configuration
 */


/** 
 * \brief PTP/OAM flow enable
 *
 * \details
 * Register: \a PTP_ANA2_EGR_CFG23:EGR2_PTP_FLOW:EGR2_PTP_FLOW_ENA
 *
 * @param gi Register: EGR2_PTP_FLOW, 0-5
 */
#define VTSS_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_FLOW_ENA(gi)  VTSS_IOREG_IX(0x1e, 1, 0xc9e0, gi, 16, 0, 0)

/** 
 * \brief
 * Indicates which next protocol groups that this flow is valid for. For
 * each next protocol group, if the bit is 1, then this flow is valid for
 * that goup. If it is 0, then it is not valid for the group.
 *
 * \details 
 * 0: Mask bit for next protocol group A
 * 1: Mask bit for next protocol group B
 *
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_FLOW_ENA . PTP_NXT_PROT_GRP_MASK
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_FLOW_ENA_PTP_NXT_PROT_GRP_MASK(x)  VTSS_ENCODE_BITFIELD(x,16,2)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_FLOW_ENA_PTP_NXT_PROT_GRP_MASK     VTSS_ENCODE_BITMASK(16,2)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_FLOW_ENA_PTP_NXT_PROT_GRP_MASK(x)  VTSS_EXTRACT_BITFIELD(x,16,2)

/** 
 * \brief
 * Channel mask
 *
 * \details 
 * 0: Flow valid for channel 0
 * 1: Flow valid for channel 1
 *
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_FLOW_ENA . PTP_CHANNEL_MASK
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_FLOW_ENA_PTP_CHANNEL_MASK(x)  VTSS_ENCODE_BITFIELD(x,4,2)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_FLOW_ENA_PTP_CHANNEL_MASK     VTSS_ENCODE_BITMASK(4,2)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_FLOW_ENA_PTP_CHANNEL_MASK(x)  VTSS_EXTRACT_BITFIELD(x,4,2)

/** 
 * \brief
 * Flow enable
 *
 * \details 
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_FLOW_ENA . PTP_FLOW_ENA
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_FLOW_ENA_PTP_FLOW_ENA  VTSS_BIT(0)


/** 
 * \brief Upper half of PTP/OAM flow match field
 *
 * \details
 * Register: \a PTP_ANA2_EGR_CFG23:EGR2_PTP_FLOW:EGR2_PTP_FLOW_MATCH_UPPER
 *
 * @param gi Register: EGR2_PTP_FLOW, 0-5
 */
#define VTSS_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_FLOW_MATCH_UPPER(gi)  VTSS_IOREG_IX(0x1e, 1, 0xc9e0, gi, 16, 0, 1)


/** 
 * \brief Lower half of PTP/OAM flow match field
 *
 * \details
 * Register: \a PTP_ANA2_EGR_CFG23:EGR2_PTP_FLOW:EGR2_PTP_FLOW_MATCH_LOWER
 *
 * @param gi Register: EGR2_PTP_FLOW, 0-5
 */
#define VTSS_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_FLOW_MATCH_LOWER(gi)  VTSS_IOREG_IX(0x1e, 1, 0xc9e0, gi, 16, 0, 2)


/** 
 * \brief Upper half of PTP/OAM flow match mask
 *
 * \details
 * Register: \a PTP_ANA2_EGR_CFG23:EGR2_PTP_FLOW:EGR2_PTP_FLOW_MASK_UPPER
 *
 * @param gi Register: EGR2_PTP_FLOW, 0-5
 */
#define VTSS_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_FLOW_MASK_UPPER(gi)  VTSS_IOREG_IX(0x1e, 1, 0xc9e0, gi, 16, 0, 3)


/** 
 * \brief Lower half of PTP/OAM flow match mask
 *
 * \details
 * Register: \a PTP_ANA2_EGR_CFG23:EGR2_PTP_FLOW:EGR2_PTP_FLOW_MASK_LOWER
 *
 * @param gi Register: EGR2_PTP_FLOW, 0-5
 */
#define VTSS_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_FLOW_MASK_LOWER(gi)  VTSS_IOREG_IX(0x1e, 1, 0xc9e0, gi, 16, 0, 4)


/** 
 * \brief PTP/OAM range match register
 *
 * \details
 * Register: \a PTP_ANA2_EGR_CFG23:EGR2_PTP_FLOW:EGR2_PTP_DOMAIN_RANGE
 *
 * @param gi Register: EGR2_PTP_FLOW, 0-5
 */
#define VTSS_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_DOMAIN_RANGE(gi)  VTSS_IOREG_IX(0x1e, 1, 0xc9e0, gi, 16, 0, 5)

/** 
 * \brief
 * PTP domain range offset
 *
 * \details 
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_DOMAIN_RANGE . PTP_DOMAIN_RANGE_OFFSET
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_DOMAIN_RANGE_PTP_DOMAIN_RANGE_OFFSET(x)  VTSS_ENCODE_BITFIELD(x,24,5)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_DOMAIN_RANGE_PTP_DOMAIN_RANGE_OFFSET     VTSS_ENCODE_BITMASK(24,5)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_DOMAIN_RANGE_PTP_DOMAIN_RANGE_OFFSET(x)  VTSS_EXTRACT_BITFIELD(x,24,5)

/** 
 * \brief
 * Upper range of PTP domain field to match
 *
 * \details 
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_DOMAIN_RANGE . PTP_DOMAIN_RANGE_UPPER
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_DOMAIN_RANGE_PTP_DOMAIN_RANGE_UPPER(x)  VTSS_ENCODE_BITFIELD(x,16,8)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_DOMAIN_RANGE_PTP_DOMAIN_RANGE_UPPER     VTSS_ENCODE_BITMASK(16,8)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_DOMAIN_RANGE_PTP_DOMAIN_RANGE_UPPER(x)  VTSS_EXTRACT_BITFIELD(x,16,8)

/** 
 * \brief
 * Lower range of PTP domain field to match
 *
 * \details 
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_DOMAIN_RANGE . PTP_DOMAIN_RANGE_LOWER
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_DOMAIN_RANGE_PTP_DOMAIN_RANGE_LOWER(x)  VTSS_ENCODE_BITFIELD(x,8,8)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_DOMAIN_RANGE_PTP_DOMAIN_RANGE_LOWER     VTSS_ENCODE_BITMASK(8,8)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_DOMAIN_RANGE_PTP_DOMAIN_RANGE_LOWER(x)  VTSS_EXTRACT_BITFIELD(x,8,8)

/** 
 * \brief
 * Enable PTP domain range checking
 *
 * \details 
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_DOMAIN_RANGE . PTP_DOMAIN_RANGE_ENA
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_DOMAIN_RANGE_PTP_DOMAIN_RANGE_ENA  VTSS_BIT(0)


/** 
 * \brief PTP action control register
 *
 * \details
 * Register: \a PTP_ANA2_EGR_CFG23:EGR2_PTP_FLOW:EGR2_PTP_ACTION
 *
 * @param gi Register: EGR2_PTP_FLOW, 0-5
 */
#define VTSS_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_ACTION(gi)  VTSS_IOREG_IX(0x1e, 1, 0xc9e0, gi, 16, 0, 6)

/** 
 * \brief
 * Modified frame status update
 *
 * \details 
 * 0: Do not signal the rewriter to update the value of the Modified Frame
 * Status bit
 * 1: Signal the rewriter to update the value of the Modified Frame Status
 * bit
 *
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_ACTION . PTP_MOD_FRAME_STAT_UPDATE
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_ACTION_PTP_MOD_FRAME_STAT_UPDATE  VTSS_BIT(28)

/** 
 * \brief
 * Indicates the position relative to the start of the PTP frame in bytes
 * where the Modified Frame Status bit resides
 *
 * \details 
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_ACTION . PTP_MOD_FRAME_BYTE_OFFSET
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_ACTION_PTP_MOD_FRAME_BYTE_OFFSET(x)  VTSS_ENCODE_BITFIELD(x,24,3)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_ACTION_PTP_MOD_FRAME_BYTE_OFFSET     VTSS_ENCODE_BITMASK(24,3)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_ACTION_PTP_MOD_FRAME_BYTE_OFFSET(x)  VTSS_EXTRACT_BITFIELD(x,24,3)

/** 
 * \brief
 * Enable subtract delay asymmetry signal
 *
 * \details 
 * 0: Do not signal the timestamp block to subtract the asymmetry delay
 * 1: Signal the timestamp block to subtract the asymmetry delay
 *
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_ACTION . PTP_SUB_DELAY_ASYM_ENA
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_ACTION_PTP_SUB_DELAY_ASYM_ENA  VTSS_BIT(21)

/** 
 * \brief
 * Enable add delay asymmetry signal
 *
 * \details 
 * 0: Do not signal the timestamp block to add the asymmetry delay
 * 1: Signal the timestamp block to add the asymmetry delay
 *
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_ACTION . PTP_ADD_DELAY_ASYM_ENA
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_ACTION_PTP_ADD_DELAY_ASYM_ENA  VTSS_BIT(20)

/** 
 * \brief
 * Time storage field offset
 * The location in a PTP frame where a time value can be stored or read
 *
 * \details 
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_ACTION . PTP_TIME_STRG_FIELD_OFFSET
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_ACTION_PTP_TIME_STRG_FIELD_OFFSET(x)  VTSS_ENCODE_BITFIELD(x,10,6)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_ACTION_PTP_TIME_STRG_FIELD_OFFSET     VTSS_ENCODE_BITMASK(10,6)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_ACTION_PTP_TIME_STRG_FIELD_OFFSET(x)  VTSS_EXTRACT_BITFIELD(x,10,6)

/** 
 * \brief
 * Points to the location of the correction field for updating the
 * timestamp. Location is relative to the first byte of the PTP/OAM header.
 * Note: If this flow is being used to match OAM frames, set this register
 * to 4
 *
 * \details 
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_ACTION . PTP_CORR_FIELD_OFFSET
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_ACTION_PTP_CORR_FIELD_OFFSET(x)  VTSS_ENCODE_BITFIELD(x,5,5)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_ACTION_PTP_CORR_FIELD_OFFSET     VTSS_ENCODE_BITMASK(5,5)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_ACTION_PTP_CORR_FIELD_OFFSET(x)  VTSS_EXTRACT_BITFIELD(x,5,5)

/** 
 * \brief
 * Save local time to the timestamp FIFO
 *
 * \details 
 * 0: Do not save the time to the timestamp FIFO
 * 1: Save the local time to the timestamp FIFO
 *
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_ACTION . PTP_SAVE_LOCAL_TIME
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_ACTION_PTP_SAVE_LOCAL_TIME  VTSS_BIT(4)

/** 
 * \brief
 * PTP action command
 *
 * \details 
 * 0: NOP
 * 1: SUB
 * 2: SUB_P2P
 * 3: ADD
 * 4: SUB_ADD
 * 5: WRITE_1588
 * 6: WRITE_P2P  (deprecated)
 * 7: WRITE_NS
 * 8: WRITE_NS_P2P
 *
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_ACTION . PTP_COMMAND
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_ACTION_PTP_COMMAND(x)  VTSS_ENCODE_BITFIELD(x,0,4)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_ACTION_PTP_COMMAND     VTSS_ENCODE_BITMASK(0,4)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_ACTION_PTP_COMMAND(x)  VTSS_EXTRACT_BITFIELD(x,0,4)


/** 
 * \brief PTP action control register 2
 *
 * \details
 * Register: \a PTP_ANA2_EGR_CFG23:EGR2_PTP_FLOW:EGR2_PTP_ACTION_2
 *
 * @param gi Register: EGR2_PTP_FLOW, 0-5
 */
#define VTSS_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_ACTION_2(gi)  VTSS_IOREG_IX(0x1e, 1, 0xc9e0, gi, 16, 0, 7)

/** 
 * \brief
 * Points to where in the frame relative to the start of the PTP header
 * that the timestamp should be updated
 *
 * \details 
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_ACTION_2 . PTP_REWRITE_OFFSET
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_ACTION_2_PTP_REWRITE_OFFSET(x)  VTSS_ENCODE_BITFIELD(x,8,8)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_ACTION_2_PTP_REWRITE_OFFSET     VTSS_ENCODE_BITMASK(8,8)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_ACTION_2_PTP_REWRITE_OFFSET(x)  VTSS_EXTRACT_BITFIELD(x,8,8)

/** 
 * \brief
 * Number of bytes in the PTP or OAM frame that must be modified by the
 * rewriter for the timestamp
 *
 * \details 
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_ACTION_2 . PTP_REWRITE_BYTES
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_ACTION_2_PTP_REWRITE_BYTES(x)  VTSS_ENCODE_BITFIELD(x,0,4)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_ACTION_2_PTP_REWRITE_BYTES     VTSS_ENCODE_BITMASK(0,4)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_ACTION_2_PTP_REWRITE_BYTES(x)  VTSS_EXTRACT_BITFIELD(x,0,4)


/** 
 * \brief Zero field control register
 *
 * \details
 * Register: \a PTP_ANA2_EGR_CFG23:EGR2_PTP_FLOW:EGR2_PTP_ZERO_FIELD_CTL
 *
 * @param gi Register: EGR2_PTP_FLOW, 0-5
 */
#define VTSS_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_ZERO_FIELD_CTL(gi)  VTSS_IOREG_IX(0x1e, 1, 0xc9e0, gi, 16, 0, 8)

/** 
 * \brief
 * When this bit is set and PTP frame matches to a flow, then the incoming
 * frame is checked for the contents of 4-byte reserved field right after
 * the correction field in the PTP header. If the contents of reserved
 * field are not zero then an interrupt is flagged.
 *
 * \details 
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_ZERO_FIELD_CTL . PTP_RSVD_CHK_EN
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_ZERO_FIELD_CTL_PTP_RSVD_CHK_EN  VTSS_BIT(14)

/** 
 * \brief
 * Points to a location in the PTP/OAM frame relative to the start of the
 * PTP header that will be zeroed if this function is enabled
 *
 * \details 
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_ZERO_FIELD_CTL . PTP_ZERO_FIELD_OFFSET
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_ZERO_FIELD_CTL_PTP_ZERO_FIELD_OFFSET(x)  VTSS_ENCODE_BITFIELD(x,8,6)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_ZERO_FIELD_CTL_PTP_ZERO_FIELD_OFFSET     VTSS_ENCODE_BITMASK(8,6)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_ZERO_FIELD_CTL_PTP_ZERO_FIELD_OFFSET(x)  VTSS_EXTRACT_BITFIELD(x,8,6)

/** 
 * \brief
 * The number of bytes to be zeroed. If this field is 0, then this function
 * is not enabled.
 *
 * \details 
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_ZERO_FIELD_CTL . PTP_ZERO_FIELD_BYTE_CNT
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_ZERO_FIELD_CTL_PTP_ZERO_FIELD_BYTE_CNT(x)  VTSS_ENCODE_BITFIELD(x,0,4)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_ZERO_FIELD_CTL_PTP_ZERO_FIELD_BYTE_CNT     VTSS_ENCODE_BITMASK(0,4)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_PTP_FLOW_EGR2_PTP_ZERO_FIELD_CTL_PTP_ZERO_FIELD_BYTE_CNT(x)  VTSS_EXTRACT_BITFIELD(x,0,4)

/**
 * Register Group: \a PTP_ANA2_EGR_CFG23:EGR2_FRAME_SIG_CFG
 *
 * Frame Signature Builder Configuration
 */


/** 
 * \brief Frame signature builder mode configuration
 *
 * \details
 * Register: \a PTP_ANA2_EGR_CFG23:EGR2_FRAME_SIG_CFG:EGR2_FSB_CFG
 */
#define VTSS_PTP_ANA2_EGR_CFG23_EGR2_FRAME_SIG_CFG_EGR2_FSB_CFG  VTSS_IOREG(0x1e, 1, 0xca40)

/** 
 * \brief
 * Frame signature builder address selection
 *
 * \details 
 * 0: Use the address from Ethernet block 1
 * 1: Use the address from Ethernet block 2
 * 2: Reserved
 * 3: Reserved
 *
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_FRAME_SIG_CFG_EGR2_FSB_CFG . FSB_ADR_SEL
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_FRAME_SIG_CFG_EGR2_FSB_CFG_FSB_ADR_SEL(x)  VTSS_ENCODE_BITFIELD(x,0,2)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_FRAME_SIG_CFG_EGR2_FSB_CFG_FSB_ADR_SEL     VTSS_ENCODE_BITMASK(0,2)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_FRAME_SIG_CFG_EGR2_FSB_CFG_FSB_ADR_SEL(x)  VTSS_EXTRACT_BITFIELD(x,0,2)


/** 
 * \brief Frame Signature Builder Mapping Register 0
 *
 * \details
 * This register selects bytes to pack into the frame signature vector. 
 * The frame signature vector is 16 bytes long.  The source bytes are as
 * follows:
 * 
 * select    source		  select    source	    select   source
 *	      select   source
 * ------------------------------------------------------------------------
 * ----------------------------------------------------------------------
 * 0	    PTP hdr byte 31    1    PTP hdr byte 30   2    PTP hdr byte 29 
 *   3	  PTP hdr byte 28
 * 4	    PTP hdr byte 27    5    PTP hdr byte 26   6    PTP hdr byte 25 
 *   7	  PTP hdr byte 24
 * 8	    PTP hdr byte 23    9    PTP hdr byte 22   10  PTP hdr byte 21  
 *  11	PTP hdr byte 20
 * 12	   PTP hdr byte 19    13  PTP hdr byte 18   14	PTP hdr byte 17   
 * 15  PTP hdr byte 16
 * 16	   PTP hdr byte 15    17  PTP hdr byte 14   18	PTP hdr byte 13   
 * 19  PTP hdr byte 12
 * 20	   PTP hdr byte 11    21  PTP hdr byte 10   22	PTP hdr byte 9	   
 * 23  PTP hdr byte 8
 * 24	   PTP hdr byte 6      25  PTP hdr byte 4     26  PTP hdr byte 0   
 *   27  reserved
 * 28	   address byte 0	29  address byte 1     30  addess byte 2   
 *     31  address byte 3
 * 32	   address byte 4	33  address byte 5     34  addess byte 6   
 *     35  address byte 7
 * 
 * all other select values reserved
 *
 * Register: \a PTP_ANA2_EGR_CFG23:EGR2_FRAME_SIG_CFG:EGR2_FSB_MAP_REG_0
 */
#define VTSS_PTP_ANA2_EGR_CFG23_EGR2_FRAME_SIG_CFG_EGR2_FSB_MAP_REG_0  VTSS_IOREG(0x1e, 1, 0xca41)

/** 
 * \brief
 * Frame signature byte 4 select
 *
 * \details 
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_FRAME_SIG_CFG_EGR2_FSB_MAP_REG_0 . FSB_MAP_4
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_FRAME_SIG_CFG_EGR2_FSB_MAP_REG_0_FSB_MAP_4(x)  VTSS_ENCODE_BITFIELD(x,24,6)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_FRAME_SIG_CFG_EGR2_FSB_MAP_REG_0_FSB_MAP_4     VTSS_ENCODE_BITMASK(24,6)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_FRAME_SIG_CFG_EGR2_FSB_MAP_REG_0_FSB_MAP_4(x)  VTSS_EXTRACT_BITFIELD(x,24,6)

/** 
 * \brief
 * Frame signature byte 3 select
 *
 * \details 
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_FRAME_SIG_CFG_EGR2_FSB_MAP_REG_0 . FSB_MAP_3
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_FRAME_SIG_CFG_EGR2_FSB_MAP_REG_0_FSB_MAP_3(x)  VTSS_ENCODE_BITFIELD(x,18,6)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_FRAME_SIG_CFG_EGR2_FSB_MAP_REG_0_FSB_MAP_3     VTSS_ENCODE_BITMASK(18,6)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_FRAME_SIG_CFG_EGR2_FSB_MAP_REG_0_FSB_MAP_3(x)  VTSS_EXTRACT_BITFIELD(x,18,6)

/** 
 * \brief
 * Frame signature byte 2 select
 *
 * \details 
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_FRAME_SIG_CFG_EGR2_FSB_MAP_REG_0 . FSB_MAP_2
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_FRAME_SIG_CFG_EGR2_FSB_MAP_REG_0_FSB_MAP_2(x)  VTSS_ENCODE_BITFIELD(x,12,6)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_FRAME_SIG_CFG_EGR2_FSB_MAP_REG_0_FSB_MAP_2     VTSS_ENCODE_BITMASK(12,6)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_FRAME_SIG_CFG_EGR2_FSB_MAP_REG_0_FSB_MAP_2(x)  VTSS_EXTRACT_BITFIELD(x,12,6)

/** 
 * \brief
 * Frame signature byte 1 select
 *
 * \details 
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_FRAME_SIG_CFG_EGR2_FSB_MAP_REG_0 . FSB_MAP_1
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_FRAME_SIG_CFG_EGR2_FSB_MAP_REG_0_FSB_MAP_1(x)  VTSS_ENCODE_BITFIELD(x,6,6)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_FRAME_SIG_CFG_EGR2_FSB_MAP_REG_0_FSB_MAP_1     VTSS_ENCODE_BITMASK(6,6)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_FRAME_SIG_CFG_EGR2_FSB_MAP_REG_0_FSB_MAP_1(x)  VTSS_EXTRACT_BITFIELD(x,6,6)

/** 
 * \brief
 * Frame signature byte 0 select
 *
 * \details 
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_FRAME_SIG_CFG_EGR2_FSB_MAP_REG_0 . FSB_MAP_0
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_FRAME_SIG_CFG_EGR2_FSB_MAP_REG_0_FSB_MAP_0(x)  VTSS_ENCODE_BITFIELD(x,0,6)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_FRAME_SIG_CFG_EGR2_FSB_MAP_REG_0_FSB_MAP_0     VTSS_ENCODE_BITMASK(0,6)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_FRAME_SIG_CFG_EGR2_FSB_MAP_REG_0_FSB_MAP_0(x)  VTSS_EXTRACT_BITFIELD(x,0,6)


/** 
 * \brief Frame Signature Builder Mapping Register 1
 *
 * \details
 * Register: \a PTP_ANA2_EGR_CFG23:EGR2_FRAME_SIG_CFG:EGR2_FSB_MAP_REG_1
 */
#define VTSS_PTP_ANA2_EGR_CFG23_EGR2_FRAME_SIG_CFG_EGR2_FSB_MAP_REG_1  VTSS_IOREG(0x1e, 1, 0xca42)

/** 
 * \brief
 * Frame signature byte 9 select
 *
 * \details 
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_FRAME_SIG_CFG_EGR2_FSB_MAP_REG_1 . FSB_MAP_9
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_FRAME_SIG_CFG_EGR2_FSB_MAP_REG_1_FSB_MAP_9(x)  VTSS_ENCODE_BITFIELD(x,24,6)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_FRAME_SIG_CFG_EGR2_FSB_MAP_REG_1_FSB_MAP_9     VTSS_ENCODE_BITMASK(24,6)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_FRAME_SIG_CFG_EGR2_FSB_MAP_REG_1_FSB_MAP_9(x)  VTSS_EXTRACT_BITFIELD(x,24,6)

/** 
 * \brief
 * Frame signature byte 8 select
 *
 * \details 
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_FRAME_SIG_CFG_EGR2_FSB_MAP_REG_1 . FSB_MAP_8
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_FRAME_SIG_CFG_EGR2_FSB_MAP_REG_1_FSB_MAP_8(x)  VTSS_ENCODE_BITFIELD(x,18,6)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_FRAME_SIG_CFG_EGR2_FSB_MAP_REG_1_FSB_MAP_8     VTSS_ENCODE_BITMASK(18,6)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_FRAME_SIG_CFG_EGR2_FSB_MAP_REG_1_FSB_MAP_8(x)  VTSS_EXTRACT_BITFIELD(x,18,6)

/** 
 * \brief
 * Frame signature byte 7 select
 *
 * \details 
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_FRAME_SIG_CFG_EGR2_FSB_MAP_REG_1 . FSB_MAP_7
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_FRAME_SIG_CFG_EGR2_FSB_MAP_REG_1_FSB_MAP_7(x)  VTSS_ENCODE_BITFIELD(x,12,6)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_FRAME_SIG_CFG_EGR2_FSB_MAP_REG_1_FSB_MAP_7     VTSS_ENCODE_BITMASK(12,6)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_FRAME_SIG_CFG_EGR2_FSB_MAP_REG_1_FSB_MAP_7(x)  VTSS_EXTRACT_BITFIELD(x,12,6)

/** 
 * \brief
 * Frame signature byte 6 select
 *
 * \details 
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_FRAME_SIG_CFG_EGR2_FSB_MAP_REG_1 . FSB_MAP_6
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_FRAME_SIG_CFG_EGR2_FSB_MAP_REG_1_FSB_MAP_6(x)  VTSS_ENCODE_BITFIELD(x,6,6)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_FRAME_SIG_CFG_EGR2_FSB_MAP_REG_1_FSB_MAP_6     VTSS_ENCODE_BITMASK(6,6)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_FRAME_SIG_CFG_EGR2_FSB_MAP_REG_1_FSB_MAP_6(x)  VTSS_EXTRACT_BITFIELD(x,6,6)

/** 
 * \brief
 * Frame signature byte 5 select
 *
 * \details 
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_FRAME_SIG_CFG_EGR2_FSB_MAP_REG_1 . FSB_MAP_5
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_FRAME_SIG_CFG_EGR2_FSB_MAP_REG_1_FSB_MAP_5(x)  VTSS_ENCODE_BITFIELD(x,0,6)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_FRAME_SIG_CFG_EGR2_FSB_MAP_REG_1_FSB_MAP_5     VTSS_ENCODE_BITMASK(0,6)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_FRAME_SIG_CFG_EGR2_FSB_MAP_REG_1_FSB_MAP_5(x)  VTSS_EXTRACT_BITFIELD(x,0,6)


/** 
 * \brief Frame Signature Builder Mapping Register 2
 *
 * \details
 * Register: \a PTP_ANA2_EGR_CFG23:EGR2_FRAME_SIG_CFG:EGR2_FSB_MAP_REG_2
 */
#define VTSS_PTP_ANA2_EGR_CFG23_EGR2_FRAME_SIG_CFG_EGR2_FSB_MAP_REG_2  VTSS_IOREG(0x1e, 1, 0xca43)

/** 
 * \brief
 * Frame signature byte 14 select
 *
 * \details 
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_FRAME_SIG_CFG_EGR2_FSB_MAP_REG_2 . FSB_MAP_14
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_FRAME_SIG_CFG_EGR2_FSB_MAP_REG_2_FSB_MAP_14(x)  VTSS_ENCODE_BITFIELD(x,24,6)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_FRAME_SIG_CFG_EGR2_FSB_MAP_REG_2_FSB_MAP_14     VTSS_ENCODE_BITMASK(24,6)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_FRAME_SIG_CFG_EGR2_FSB_MAP_REG_2_FSB_MAP_14(x)  VTSS_EXTRACT_BITFIELD(x,24,6)

/** 
 * \brief
 * Frame signature byte 13 select
 *
 * \details 
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_FRAME_SIG_CFG_EGR2_FSB_MAP_REG_2 . FSB_MAP_13
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_FRAME_SIG_CFG_EGR2_FSB_MAP_REG_2_FSB_MAP_13(x)  VTSS_ENCODE_BITFIELD(x,18,6)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_FRAME_SIG_CFG_EGR2_FSB_MAP_REG_2_FSB_MAP_13     VTSS_ENCODE_BITMASK(18,6)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_FRAME_SIG_CFG_EGR2_FSB_MAP_REG_2_FSB_MAP_13(x)  VTSS_EXTRACT_BITFIELD(x,18,6)

/** 
 * \brief
 * Frame signature byte 12 select
 *
 * \details 
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_FRAME_SIG_CFG_EGR2_FSB_MAP_REG_2 . FSB_MAP_12
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_FRAME_SIG_CFG_EGR2_FSB_MAP_REG_2_FSB_MAP_12(x)  VTSS_ENCODE_BITFIELD(x,12,6)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_FRAME_SIG_CFG_EGR2_FSB_MAP_REG_2_FSB_MAP_12     VTSS_ENCODE_BITMASK(12,6)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_FRAME_SIG_CFG_EGR2_FSB_MAP_REG_2_FSB_MAP_12(x)  VTSS_EXTRACT_BITFIELD(x,12,6)

/** 
 * \brief
 * Frame signature byte 11 select
 *
 * \details 
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_FRAME_SIG_CFG_EGR2_FSB_MAP_REG_2 . FSB_MAP_11
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_FRAME_SIG_CFG_EGR2_FSB_MAP_REG_2_FSB_MAP_11(x)  VTSS_ENCODE_BITFIELD(x,6,6)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_FRAME_SIG_CFG_EGR2_FSB_MAP_REG_2_FSB_MAP_11     VTSS_ENCODE_BITMASK(6,6)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_FRAME_SIG_CFG_EGR2_FSB_MAP_REG_2_FSB_MAP_11(x)  VTSS_EXTRACT_BITFIELD(x,6,6)

/** 
 * \brief
 * Frame signature byte 10 select
 *
 * \details 
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_FRAME_SIG_CFG_EGR2_FSB_MAP_REG_2 . FSB_MAP_10
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_FRAME_SIG_CFG_EGR2_FSB_MAP_REG_2_FSB_MAP_10(x)  VTSS_ENCODE_BITFIELD(x,0,6)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_FRAME_SIG_CFG_EGR2_FSB_MAP_REG_2_FSB_MAP_10     VTSS_ENCODE_BITMASK(0,6)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_FRAME_SIG_CFG_EGR2_FSB_MAP_REG_2_FSB_MAP_10(x)  VTSS_EXTRACT_BITFIELD(x,0,6)


/** 
 * \brief Frame Signature Builder Mapping Register 3
 *
 * \details
 * Register: \a PTP_ANA2_EGR_CFG23:EGR2_FRAME_SIG_CFG:EGR2_FSB_MAP_REG_3
 */
#define VTSS_PTP_ANA2_EGR_CFG23_EGR2_FRAME_SIG_CFG_EGR2_FSB_MAP_REG_3  VTSS_IOREG(0x1e, 1, 0xca44)

/** 
 * \brief
 * Frame signature byte 15 select
 *
 * \details 
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_FRAME_SIG_CFG_EGR2_FSB_MAP_REG_3 . FSB_MAP_15
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_FRAME_SIG_CFG_EGR2_FSB_MAP_REG_3_FSB_MAP_15(x)  VTSS_ENCODE_BITFIELD(x,0,6)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_FRAME_SIG_CFG_EGR2_FSB_MAP_REG_3_FSB_MAP_15     VTSS_ENCODE_BITMASK(0,6)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_FRAME_SIG_CFG_EGR2_FSB_MAP_REG_3_FSB_MAP_15(x)  VTSS_EXTRACT_BITFIELD(x,0,6)

/**
 * Register Group: \a PTP_ANA2_EGR_CFG23:EGR2_ACH_COMPARATOR_A
 *
 * ACH comparator for block A
 */


/** 
 * \brief ACH upper 32-bits match of the header
 *
 * \details
 * ACH upper 32-bits match of the header
 *
 * Register: \a PTP_ANA2_EGR_CFG23:EGR2_ACH_COMPARATOR_A:EGR2_ACH_PROT_MATCH_UPPER_A
 */
#define VTSS_PTP_ANA2_EGR_CFG23_EGR2_ACH_COMPARATOR_A_EGR2_ACH_PROT_MATCH_UPPER_A  VTSS_IOREG(0x1e, 1, 0xca45)


/** 
 * \brief ACH lower 32-bits match of the header
 *
 * \details
 * ACH lower 32-bits match of the header
 *
 * Register: \a PTP_ANA2_EGR_CFG23:EGR2_ACH_COMPARATOR_A:EGR2_ACH_PROT_MATCH_LOWER_A
 */
#define VTSS_PTP_ANA2_EGR_CFG23_EGR2_ACH_COMPARATOR_A_EGR2_ACH_PROT_MATCH_LOWER_A  VTSS_IOREG(0x1e, 1, 0xca46)


/** 
 * \brief ACH mask for upper 32-bits
 *
 * \details
 * ACH mask for upper 32-bits
 *
 * Register: \a PTP_ANA2_EGR_CFG23:EGR2_ACH_COMPARATOR_A:EGR2_ACH_PROT_MASK_UPPER_A
 */
#define VTSS_PTP_ANA2_EGR_CFG23_EGR2_ACH_COMPARATOR_A_EGR2_ACH_PROT_MASK_UPPER_A  VTSS_IOREG(0x1e, 1, 0xca47)


/** 
 * \brief ACH mask for lower 32-bits
 *
 * \details
 * ACH mask for lower 32-bits
 *
 * Register: \a PTP_ANA2_EGR_CFG23:EGR2_ACH_COMPARATOR_A:EGR2_ACH_PROT_MASK_LOWER_A
 */
#define VTSS_PTP_ANA2_EGR_CFG23_EGR2_ACH_COMPARATOR_A_EGR2_ACH_PROT_MASK_LOWER_A  VTSS_IOREG(0x1e, 1, 0xca48)


/** 
 * \brief ACH protocol offset for block A
 *
 * \details
 * ACH protocol offset for block A
 *
 * Register: \a PTP_ANA2_EGR_CFG23:EGR2_ACH_COMPARATOR_A:EGR2_ACH_PROT_OFFSET_A
 */
#define VTSS_PTP_ANA2_EGR_CFG23_EGR2_ACH_COMPARATOR_A_EGR2_ACH_PROT_OFFSET_A  VTSS_IOREG(0x1e, 1, 0xca49)

/** 
 * \brief
 * Points to the start of the next protocol relative to the ACH header.
 *
 * \details 
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_ACH_COMPARATOR_A_EGR2_ACH_PROT_OFFSET_A . ACH_NXT_PROTOCOL_A
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_ACH_COMPARATOR_A_EGR2_ACH_PROT_OFFSET_A_ACH_NXT_PROTOCOL_A(x)  VTSS_ENCODE_BITFIELD(x,0,8)
#define  VTSS_M_PTP_ANA2_EGR_CFG23_EGR2_ACH_COMPARATOR_A_EGR2_ACH_PROT_OFFSET_A_ACH_NXT_PROTOCOL_A     VTSS_ENCODE_BITMASK(0,8)
#define  VTSS_X_PTP_ANA2_EGR_CFG23_EGR2_ACH_COMPARATOR_A_EGR2_ACH_PROT_OFFSET_A_ACH_NXT_PROTOCOL_A(x)  VTSS_EXTRACT_BITFIELD(x,0,8)

/** 
 * \brief
 * Set to scan for ACH control word. This is mutually exclusive with
 * mpls_ctl_word. Both can't be set at the same time
 *
 * \details 
 * Field: VTSS_PTP_ANA2_EGR_CFG23_EGR2_ACH_COMPARATOR_A_EGR2_ACH_PROT_OFFSET_A . ACH_CTL_WORD_A
 */
#define  VTSS_F_PTP_ANA2_EGR_CFG23_EGR2_ACH_COMPARATOR_A_EGR2_ACH_PROT_OFFSET_A_ACH_CTL_WORD_A  VTSS_BIT(8)


#endif /* _VTSS_MALIBU_REGS_PTP_ANA2_EGR_CFG23_H_ */
