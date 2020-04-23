#ifndef _VTSS_MALIBU_REGS_PTP_3_H_
#define _VTSS_MALIBU_REGS_PTP_3_H_

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
 * Target: \a PTP_3
 *
 * 1588 IP block configuration and status registers
 *
 ***********************************************************************/

/**
 * Register Group: \a PTP_3:IP_1588_TOP_CFG_STAT
 *
 * 1588 IP control and status registers
 */


/** 
 * \brief Interface control register
 *
 * \details
 * Register: \a PTP_3:IP_1588_TOP_CFG_STAT:INTERFACE_CTL
 */
#define VTSS_PTP_3_IP_1588_TOP_CFG_STAT_INTERFACE_CTL  VTSS_IOREG(0x1e, 1, 0x9200)

/** 
 * \brief
 * If this bit is 0, then EGR_BYPASS is treated as a single bypass control
 * for both egress and ingress directions. 
 * If this bit is 1, then bypass for individual direction is controlled by
 * EGR_BYPASS and INGR_BYPASS.
 *
 * \details 
 * 0: Single bypass control
 * 1: Separate bypass control
 *
 * Field: VTSS_PTP_3_IP_1588_TOP_CFG_STAT_INTERFACE_CTL . SPLIT_BYPASS
 */
#define  VTSS_F_PTP_3_IP_1588_TOP_CFG_STAT_INTERFACE_CTL_SPLIT_BYPASS  VTSS_BIT(7)

/** 
 * \brief
 * Clock enable for the 1588 IP block. 
 * This bit is an output of the IP and can be used externally to gate the
 * clocks to the block off when the block is disabled. This bit is not used
 * inside the IP block.
 *
 * \details 
 * 0: Clocks disabled
 * 1: Clocks enabled
 *
 * Field: VTSS_PTP_3_IP_1588_TOP_CFG_STAT_INTERFACE_CTL . CLK_ENA
 */
#define  VTSS_F_PTP_3_IP_1588_TOP_CFG_STAT_INTERFACE_CTL_CLK_ENA  VTSS_BIT(6)

/** 
 * \brief
 * When 1, the 1588 IP block in the ingress direction is bypassed. Changing
 * this bit to 0 allows 1588 processed data to flow out of the block. This
 * bit is internally registered so that it only takes effect during an IDLE
 * period in the data stream. This allows for a more seamless transition
 * from bypass to data passing modes.
 *
 * \details 
 * 0 = Data mode
 * 1 = Bypass mode (Default)
 *
 * Field: VTSS_PTP_3_IP_1588_TOP_CFG_STAT_INTERFACE_CTL . INGR_BYPASS
 */
#define  VTSS_F_PTP_3_IP_1588_TOP_CFG_STAT_INTERFACE_CTL_INGR_BYPASS  VTSS_BIT(3)

/** 
 * \brief
 * When 1, the 1588 IP block in the egress direction is bypassed. Changing
 * this bit to 0 allows 1588 processed data to flow out of the block. This
 * bit is internally registered so that it only takes effect during an IDLE
 * period in the data stream. This allows for a more seamless transition
 * from bypass to data passing modes.
 *
 * \details 
 * 0 = Data mode
 * 1 = Bypass mode (Default)
 *
 * Field: VTSS_PTP_3_IP_1588_TOP_CFG_STAT_INTERFACE_CTL . EGR_BYPASS
 */
#define  VTSS_F_PTP_3_IP_1588_TOP_CFG_STAT_INTERFACE_CTL_EGR_BYPASS  VTSS_BIT(2)

/** 
 * \brief
 * Defines the operating mode in which the attached PCS block operates
 *
 * \details 
 * 0: XGMII-64
 * 1: Reserved
 * 2: GMII
 * 3: MII
 *
 * Field: VTSS_PTP_3_IP_1588_TOP_CFG_STAT_INTERFACE_CTL . MII_PROTOCOL
 */
#define  VTSS_F_PTP_3_IP_1588_TOP_CFG_STAT_INTERFACE_CTL_MII_PROTOCOL(x)  VTSS_ENCODE_BITFIELD(x,0,2)
#define  VTSS_M_PTP_3_IP_1588_TOP_CFG_STAT_INTERFACE_CTL_MII_PROTOCOL     VTSS_ENCODE_BITMASK(0,2)
#define  VTSS_X_PTP_3_IP_1588_TOP_CFG_STAT_INTERFACE_CTL_MII_PROTOCOL(x)  VTSS_EXTRACT_BITFIELD(x,0,2)


/** 
 * \brief Analyzer mode register
 *
 * \details
 * Register: \a PTP_3:IP_1588_TOP_CFG_STAT:ANALYZER_MODE
 */
#define VTSS_PTP_3_IP_1588_TOP_CFG_STAT_ANALYZER_MODE  VTSS_IOREG(0x1e, 1, 0x9201)

/** 
 * \brief
 * If this bit is 0, then INGR_ENCAP_FLOW_ENA controls both ingress and
 * egress side of encapsulations. 
 * If this bit is 1, then flows for the respective direction are set by
 * INGR_ENCAP_FLOW_ENA and EGR_ENCAP_FLOW_ENA.
 *
 * \details 
 * 0: Single encap flow for both directions
 * 1: Individual encap flow enables
 *
 * Field: VTSS_PTP_3_IP_1588_TOP_CFG_STAT_ANALYZER_MODE . SPLIT_ENCAP_FLOW
 */
#define  VTSS_F_PTP_3_IP_1588_TOP_CFG_STAT_ANALYZER_MODE_SPLIT_ENCAP_FLOW  VTSS_BIT(24)

/** 
 * \brief
 * Defines how flow matching is performed in each encapsulation engine for
 * egress side.  Only valid values are: 111 : Strict flow for all engines.
 * 000 : Non-strict flow for all engines. 
 *
 * \details 
 * 0: Match any flow
 * 1: Strict matching
 *
 * Field: VTSS_PTP_3_IP_1588_TOP_CFG_STAT_ANALYZER_MODE . EGR_ENCAP_FLOW_MODE
 */
#define  VTSS_F_PTP_3_IP_1588_TOP_CFG_STAT_ANALYZER_MODE_EGR_ENCAP_FLOW_MODE(x)  VTSS_ENCODE_BITFIELD(x,20,3)
#define  VTSS_M_PTP_3_IP_1588_TOP_CFG_STAT_ANALYZER_MODE_EGR_ENCAP_FLOW_MODE     VTSS_ENCODE_BITMASK(20,3)
#define  VTSS_X_PTP_3_IP_1588_TOP_CFG_STAT_ANALYZER_MODE_EGR_ENCAP_FLOW_MODE(x)  VTSS_EXTRACT_BITFIELD(x,20,3)

/** 
 * \brief
 * Defines how flow matching is performed in each encapsulation engine for
 * ingress side. Only valid values are:
 * 111: Strict flow for all engines
 * 000: Non-strict flow for all engines
 *
 * \details 
 * 0: Match any flow
 * 1: Strict matching
 *
 * Field: VTSS_PTP_3_IP_1588_TOP_CFG_STAT_ANALYZER_MODE . INGR_ENCAP_FLOW_MODE
 */
#define  VTSS_F_PTP_3_IP_1588_TOP_CFG_STAT_ANALYZER_MODE_INGR_ENCAP_FLOW_MODE(x)  VTSS_ENCODE_BITFIELD(x,16,3)
#define  VTSS_M_PTP_3_IP_1588_TOP_CFG_STAT_ANALYZER_MODE_INGR_ENCAP_FLOW_MODE     VTSS_ENCODE_BITMASK(16,3)
#define  VTSS_X_PTP_3_IP_1588_TOP_CFG_STAT_ANALYZER_MODE_INGR_ENCAP_FLOW_MODE(x)  VTSS_EXTRACT_BITFIELD(x,16,3)

/** 
 * \brief
 * Enables for the egress encapsulation engines. Enable bit 0 and 1 are for
 * the PTP engines; bit 2 is for the OAM engine.
 *
 * \details 
 * 0: Disabled
 * 1: Enabled
 *
 * Field: VTSS_PTP_3_IP_1588_TOP_CFG_STAT_ANALYZER_MODE . EGR_ENCAP_ENGINE_ENA
 */
#define  VTSS_F_PTP_3_IP_1588_TOP_CFG_STAT_ANALYZER_MODE_EGR_ENCAP_ENGINE_ENA(x)  VTSS_ENCODE_BITFIELD(x,4,3)
#define  VTSS_M_PTP_3_IP_1588_TOP_CFG_STAT_ANALYZER_MODE_EGR_ENCAP_ENGINE_ENA     VTSS_ENCODE_BITMASK(4,3)
#define  VTSS_X_PTP_3_IP_1588_TOP_CFG_STAT_ANALYZER_MODE_EGR_ENCAP_ENGINE_ENA(x)  VTSS_EXTRACT_BITFIELD(x,4,3)

/** 
 * \brief
 * Enables for the ingress encapsulation engines. Enable bit 0 & 1 are for
 * the PTP engines; bit 2 is for the OAM engine.
 *
 * \details 
 * 0: Disabled
 * 1: Enabled
 *
 * Field: VTSS_PTP_3_IP_1588_TOP_CFG_STAT_ANALYZER_MODE . INGR_ENCAP_ENGINE_ENA
 */
#define  VTSS_F_PTP_3_IP_1588_TOP_CFG_STAT_ANALYZER_MODE_INGR_ENCAP_ENGINE_ENA(x)  VTSS_ENCODE_BITFIELD(x,0,3)
#define  VTSS_M_PTP_3_IP_1588_TOP_CFG_STAT_ANALYZER_MODE_INGR_ENCAP_ENGINE_ENA     VTSS_ENCODE_BITMASK(0,3)
#define  VTSS_X_PTP_3_IP_1588_TOP_CFG_STAT_ANALYZER_MODE_INGR_ENCAP_ENGINE_ENA(x)  VTSS_EXTRACT_BITFIELD(x,0,3)


/** 
 * \brief Mode of the 1588
 *
 * \details
 * This register has the duplicate information for MII_MODE of
 * INTERFACE_CTL. Final mode for 1588 will be this 3-bit register ORed with
 * 2-bit MII_MODE of INTERFACE_CTL
 *
 * Register: \a PTP_3:IP_1588_TOP_CFG_STAT:MODE_CTL
 */
#define VTSS_PTP_3_IP_1588_TOP_CFG_STAT_MODE_CTL  VTSS_IOREG(0x1e, 1, 0x9202)

/** 
 * \brief
 * This register has the duplicate information for MII_MODE of
 * INTERFACE_CTL. Final mode for 1588 will be this 3-bit register ORed with
 * 2-bit MII_MODE of INTERFACE_CTL
 *
 * \details 
 * 0: XGMII-64
 * 1: XGMII-32
 * 2: GMII
 * 3: MII
 * 4: PKT_MODE
 *
 * Field: VTSS_PTP_3_IP_1588_TOP_CFG_STAT_MODE_CTL . PROTOCOL_MODE
 */
#define  VTSS_F_PTP_3_IP_1588_TOP_CFG_STAT_MODE_CTL_PROTOCOL_MODE(x)  VTSS_ENCODE_BITFIELD(x,0,3)
#define  VTSS_M_PTP_3_IP_1588_TOP_CFG_STAT_MODE_CTL_PROTOCOL_MODE     VTSS_ENCODE_BITMASK(0,3)
#define  VTSS_X_PTP_3_IP_1588_TOP_CFG_STAT_MODE_CTL_PROTOCOL_MODE(x)  VTSS_EXTRACT_BITFIELD(x,0,3)

/**
 * Register Group: \a PTP_3:IP_1588_LTC
 *
 * 1588 IP local time counter
 */


/** 
 * \brief LTC control
 *
 * \details
 * LTC control
 *
 * Register: \a PTP_3:IP_1588_LTC:LTC_CTRL
 */
#define VTSS_PTP_3_IP_1588_LTC_LTC_CTRL      VTSS_IOREG(0x1e, 1, 0x9210)

/** 
 * \brief
 * When this bit is set to high, then LTC_AUTO_ADJUST_UPDATE and
 * LTC_AUTO_ADJ_M_UPDATE will update the adjustment only when a rising edge
 * is detected on load_save external pin. 
 *
 * \details 
 * Field: VTSS_PTP_3_IP_1588_LTC_LTC_CTRL . LTC_AUTO_ADJ_UPD_SYNC
 */
#define  VTSS_F_PTP_3_IP_1588_LTC_LTC_CTRL_LTC_AUTO_ADJ_UPD_SYNC  VTSS_BIT(15)

/** 
 * \brief
 * Select the clock source for the LTC block. The actual clock mux is
 * external to the IP block, this field merely provides the select lines to
 * the clock mux. These three select lines are outputs of the IP block and
 * are not used internally. The single clock signal is then fed to the LTC
 * input.The three bits allow for one of up to eight possible clock sources
 * to be selected.
 *
 * \details 
 * 0: External pin is source of clock
 * 1: Client Rx clock
 * 2: Client Tx clock
 * 3: Line Rx clock
 * 4: Line Tx clock
 * 5-7: INVALID
 *
 * Field: VTSS_PTP_3_IP_1588_LTC_LTC_CTRL . LTC_CLK_SEL
 */
#define  VTSS_F_PTP_3_IP_1588_LTC_LTC_CTRL_LTC_CLK_SEL(x)  VTSS_ENCODE_BITFIELD(x,12,3)
#define  VTSS_M_PTP_3_IP_1588_LTC_LTC_CTRL_LTC_CLK_SEL     VTSS_ENCODE_BITMASK(12,3)
#define  VTSS_X_PTP_3_IP_1588_LTC_LTC_CTRL_LTC_CLK_SEL(x)  VTSS_EXTRACT_BITFIELD(x,12,3)

/** 
 * \brief
 * Selects one bit of the LTC internal nanosecond counter (0-29) to drive
 * to the PPS pin when in the alternate mode (LTC_ALT_MODE register bit
 * set).
 *
 * \details 
 * Field: VTSS_PTP_3_IP_1588_LTC_LTC_CTRL . LTC_ALT_MODE_PPS_BIT
 */
#define  VTSS_F_PTP_3_IP_1588_LTC_LTC_CTRL_LTC_ALT_MODE_PPS_BIT(x)  VTSS_ENCODE_BITFIELD(x,6,5)
#define  VTSS_M_PTP_3_IP_1588_LTC_LTC_CTRL_LTC_ALT_MODE_PPS_BIT     VTSS_ENCODE_BITMASK(6,5)
#define  VTSS_X_PTP_3_IP_1588_LTC_LTC_CTRL_LTC_ALT_MODE_PPS_BIT(x)  VTSS_EXTRACT_BITFIELD(x,6,5)

/** 
 * \brief
 * Enable an alternate mode that modifies the default PPS output. See the
 * LTC_ALT_MODE_PPS_BIT field description.
 *
 * \details 
 * 0: Normal mode
 * 1: Alternate mode
 *
 * Field: VTSS_PTP_3_IP_1588_LTC_LTC_CTRL . LTC_ALT_MODE
 */
#define  VTSS_F_PTP_3_IP_1588_LTC_LTC_CTRL_LTC_ALT_MODE  VTSS_BIT(5)

/** 
 * \brief
 * When written to a 1, AUTO_ADJUST_NS_COUNTER uses programmed
 * LTC_AUTO_ADJUST_NS and LTC_AUTO_ADD_SUB_1NS values.
 * Automatically cleared.
 *
 * \details 
 * 0: No change to any previous updates (write), or update has completed
 * (read)
 * 1: Use new values from LTC_AUTO_ADJUST register
 *
 * Field: VTSS_PTP_3_IP_1588_LTC_LTC_CTRL . LTC_AUTO_ADJUST_UPDATE
 */
#define  VTSS_F_PTP_3_IP_1588_LTC_LTC_CTRL_LTC_AUTO_ADJUST_UPDATE  VTSS_BIT(4)

/** 
 * \brief
 * When written to a 1 causes a request for 1 ns to be added or subtracted
 * (depending upon the LTC_ADD_SUB_1NS field) from the Local time. This bit
 * is automatically cleared.
 *
 * \details 
 * 0: No add/subtract from local time (write), bit has auto cleared (read)
 * 1: Add/subtract 1 ns from the local time
 *
 * Field: VTSS_PTP_3_IP_1588_LTC_LTC_CTRL . LTC_ADD_SUB_1NS_REQ
 */
#define  VTSS_F_PTP_3_IP_1588_LTC_LTC_CTRL_LTC_ADD_SUB_1NS_REQ  VTSS_BIT(3)

/** 
 * \brief
 * This bit selects whether a write to the LTC_ADD_SUB_1NS_REQ register
 * causes an add or subtract
 *
 * \details 
 * 0: Subtract 1 ns
 * 1: Add 1 ns to the local time
 *
 * Field: VTSS_PTP_3_IP_1588_LTC_LTC_CTRL . LTC_ADD_SUB_1NS
 */
#define  VTSS_F_PTP_3_IP_1588_LTC_LTC_CTRL_LTC_ADD_SUB_1NS  VTSS_BIT(2)

/** 
 * \brief
 * LTC save enable
 * Enables the load/save for channel 0 and 1 pin (GPIO 1) to save the
 * LTC_SAVE seconds/nanoseconds registers.
 *
 * \details 
 * Field: VTSS_PTP_3_IP_1588_LTC_LTC_CTRL . LTC_SAVE_ENA
 */
#define  VTSS_F_PTP_3_IP_1588_LTC_LTC_CTRL_LTC_SAVE_ENA  VTSS_BIT(1)

/** 
 * \brief
 * LTC load enable
 * Enables load/save for channel 0 and 1 pin (GPIO 1) to load the LTC_LOAD
 * seconds/nanoseconds registers.
 *
 * \details 
 * Field: VTSS_PTP_3_IP_1588_LTC_LTC_CTRL . LTC_LOAD_ENA
 */
#define  VTSS_F_PTP_3_IP_1588_LTC_LTC_CTRL_LTC_LOAD_ENA  VTSS_BIT(0)


/** 
 * \brief LTC load seconds (high)
 *
 * \details
 * LTC load seconds (high)
 *
 * Register: \a PTP_3:IP_1588_LTC:LTC_LOAD_SEC_H
 */
#define VTSS_PTP_3_IP_1588_LTC_LTC_LOAD_SEC_H  VTSS_IOREG(0x1e, 1, 0x9211)

/** 
 * \brief
 * LTC load seconds (high)
 *
 * \details 
 * Field: VTSS_PTP_3_IP_1588_LTC_LTC_LOAD_SEC_H . LTC_LOAD_SEC_H
 */
#define  VTSS_F_PTP_3_IP_1588_LTC_LTC_LOAD_SEC_H_LTC_LOAD_SEC_H(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_PTP_3_IP_1588_LTC_LTC_LOAD_SEC_H_LTC_LOAD_SEC_H     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_PTP_3_IP_1588_LTC_LTC_LOAD_SEC_H_LTC_LOAD_SEC_H(x)  VTSS_EXTRACT_BITFIELD(x,0,16)


/** 
 * \brief LTC load seconds (low)
 *
 * \details
 * LTC load seconds (low)
 *
 * Register: \a PTP_3:IP_1588_LTC:LTC_LOAD_SEC_L
 */
#define VTSS_PTP_3_IP_1588_LTC_LTC_LOAD_SEC_L  VTSS_IOREG(0x1e, 1, 0x9212)


/** 
 * \brief LTC load nanoseconds
 *
 * \details
 * LTC load nanoseconds
 *
 * Register: \a PTP_3:IP_1588_LTC:LTC_LOAD_NS
 */
#define VTSS_PTP_3_IP_1588_LTC_LTC_LOAD_NS   VTSS_IOREG(0x1e, 1, 0x9213)


/** 
 * \brief LTC saved seconds (high)
 *
 * \details
 * LTC saved seconds (high)
 *
 * Register: \a PTP_3:IP_1588_LTC:LTC_SAVED_SEC_H
 */
#define VTSS_PTP_3_IP_1588_LTC_LTC_SAVED_SEC_H  VTSS_IOREG(0x1e, 1, 0x9214)

/** 
 * \brief
 * LTC saved seconds (high)
 *
 * \details 
 * Field: VTSS_PTP_3_IP_1588_LTC_LTC_SAVED_SEC_H . LTC_SAVED_SEC_H
 */
#define  VTSS_F_PTP_3_IP_1588_LTC_LTC_SAVED_SEC_H_LTC_SAVED_SEC_H(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_PTP_3_IP_1588_LTC_LTC_SAVED_SEC_H_LTC_SAVED_SEC_H     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_PTP_3_IP_1588_LTC_LTC_SAVED_SEC_H_LTC_SAVED_SEC_H(x)  VTSS_EXTRACT_BITFIELD(x,0,16)


/** 
 * \brief LTC saved seconds (low)
 *
 * \details
 * LTC saved seconds (low)
 *
 * Register: \a PTP_3:IP_1588_LTC:LTC_SAVED_SEC_L
 */
#define VTSS_PTP_3_IP_1588_LTC_LTC_SAVED_SEC_L  VTSS_IOREG(0x1e, 1, 0x9215)


/** 
 * \brief LTC saved nanoseconds
 *
 * \details
 * LTC saved nanoseconds
 *
 * Register: \a PTP_3:IP_1588_LTC:LTC_SAVED_NS
 */
#define VTSS_PTP_3_IP_1588_LTC_LTC_SAVED_NS  VTSS_IOREG(0x1e, 1, 0x9216)


/** 
 * \brief LTC sequence configuration
 *
 * \details
 * LTC sequence configuration
 *
 * Register: \a PTP_3:IP_1588_LTC:LTC_SEQUENCE
 */
#define VTSS_PTP_3_IP_1588_LTC_LTC_SEQUENCE  VTSS_IOREG(0x1e, 1, 0x9217)

/** 
 * \brief
 * LTC sequence of increments (nanoseconds).
 *
 * \details 
 * Field: VTSS_PTP_3_IP_1588_LTC_LTC_SEQUENCE . LTC_SEQUENCE_A
 */
#define  VTSS_F_PTP_3_IP_1588_LTC_LTC_SEQUENCE_LTC_SEQUENCE_A(x)  VTSS_ENCODE_BITFIELD(x,0,4)
#define  VTSS_M_PTP_3_IP_1588_LTC_LTC_SEQUENCE_LTC_SEQUENCE_A     VTSS_ENCODE_BITMASK(0,4)
#define  VTSS_X_PTP_3_IP_1588_LTC_LTC_SEQUENCE_LTC_SEQUENCE_A(x)  VTSS_EXTRACT_BITFIELD(x,0,4)


/** 
 * \brief LTC sequence configuration
 *
 * \details
 * LTC sequence configuration
 *
 * Register: \a PTP_3:IP_1588_LTC:LTC_SEQ
 */
#define VTSS_PTP_3_IP_1588_LTC_LTC_SEQ       VTSS_IOREG(0x1e, 1, 0x9218)

/** 
 * \brief
 * LTC sequence correction sign
 *
 * \details 
 * 0: Subtract 1 ns adjustment
 * 1: Add 1 ns adjustment
 *
 * Field: VTSS_PTP_3_IP_1588_LTC_LTC_SEQ . LTC_SEQ_ADD_SUB
 */
#define  VTSS_F_PTP_3_IP_1588_LTC_LTC_SEQ_LTC_SEQ_ADD_SUB  VTSS_BIT(19)

/** 
 * \brief
 * LTC sequence correction (nanoseconds * 1 million)
 * Example for 6.4 ns period (156.25 MHz):
 * LTC_SEQUENCE.LTC_SEQUENCE_A = 6 (6 ns)
 * LTC_SEQ.LTC_SEQ_ADD_SUB = 1 (add 1 ns)
 * LTC_SEQ.LTC_SEQ_E = 400000 (0.4 ns * 1,000,000)
 *
 * \details 
 * Field: VTSS_PTP_3_IP_1588_LTC_LTC_SEQ . LTC_SEQ_E
 */
#define  VTSS_F_PTP_3_IP_1588_LTC_LTC_SEQ_LTC_SEQ_E(x)  VTSS_ENCODE_BITFIELD(x,0,19)
#define  VTSS_M_PTP_3_IP_1588_LTC_LTC_SEQ_LTC_SEQ_E     VTSS_ENCODE_BITMASK(0,19)
#define  VTSS_X_PTP_3_IP_1588_LTC_LTC_SEQ_LTC_SEQ_E(x)  VTSS_EXTRACT_BITFIELD(x,0,19)


/** 
 * \brief LTC sequence of increments (nanoseconds)
 *
 * \details
 * LTC sequence of increments (nanoseconds)
 *
 * Register: \a PTP_3:IP_1588_LTC:LTC_SEQUENCE_INCR
 */
#define VTSS_PTP_3_IP_1588_LTC_LTC_SEQUENCE_INCR  VTSS_IOREG(0x1e, 1, 0x9219)


/** 
 * \brief LTC auto adjustment
 *
 * \details
 * LTC auto adjustment
 *
 * Register: \a PTP_3:IP_1588_LTC:LTC_AUTO_ADJUST
 */
#define VTSS_PTP_3_IP_1588_LTC_LTC_AUTO_ADJUST  VTSS_IOREG(0x1e, 1, 0x921a)

/** 
 * \brief
 * LTC auto adjustment add/subtract 1 ns
 *
 * \details 
 * 0,3: No adjustment
 * 1: Adjust by adding 1 ns upon rollover
 * 2: Adjust by subtracting 1 ns upon rollover
 *
 * Field: VTSS_PTP_3_IP_1588_LTC_LTC_AUTO_ADJUST . LTC_AUTO_ADD_SUB_1NS
 */
#define  VTSS_F_PTP_3_IP_1588_LTC_LTC_AUTO_ADJUST_LTC_AUTO_ADD_SUB_1NS(x)  VTSS_ENCODE_BITFIELD(x,30,2)
#define  VTSS_M_PTP_3_IP_1588_LTC_LTC_AUTO_ADJUST_LTC_AUTO_ADD_SUB_1NS     VTSS_ENCODE_BITMASK(30,2)
#define  VTSS_X_PTP_3_IP_1588_LTC_LTC_AUTO_ADJUST_LTC_AUTO_ADD_SUB_1NS(x)  VTSS_EXTRACT_BITFIELD(x,30,2)

/** 
 * \brief
 * LTC auto adjustment rollover (nanoseconds)
 *
 * \details 
 * Field: VTSS_PTP_3_IP_1588_LTC_LTC_AUTO_ADJUST . LTC_AUTO_ADJUST_NS
 */
#define  VTSS_F_PTP_3_IP_1588_LTC_LTC_AUTO_ADJUST_LTC_AUTO_ADJUST_NS(x)  VTSS_ENCODE_BITFIELD(x,0,30)
#define  VTSS_M_PTP_3_IP_1588_LTC_LTC_AUTO_ADJUST_LTC_AUTO_ADJUST_NS     VTSS_ENCODE_BITMASK(0,30)
#define  VTSS_X_PTP_3_IP_1588_LTC_LTC_AUTO_ADJUST_LTC_AUTO_ADJUST_NS(x)  VTSS_EXTRACT_BITFIELD(x,0,30)


/** 
 * \brief LTC 1 pulse per second width adjustment
 *
 * \details
 * LTC 1 pulse per second width adjustment
 *
 * Register: \a PTP_3:IP_1588_LTC:LTC_1PPS_WIDTH_ADJ
 */
#define VTSS_PTP_3_IP_1588_LTC_LTC_1PPS_WIDTH_ADJ  VTSS_IOREG(0x1e, 1, 0x921b)

/** 
 * \brief
 * The 1 pulse per second is high for the programmed number of nanoseconds
 * within +/- the sequence increment value
 *
 * \details 
 * Field: VTSS_PTP_3_IP_1588_LTC_LTC_1PPS_WIDTH_ADJ . LTC_1PPS_WIDTH_ADJ
 */
#define  VTSS_F_PTP_3_IP_1588_LTC_LTC_1PPS_WIDTH_ADJ_LTC_1PPS_WIDTH_ADJ(x)  VTSS_ENCODE_BITFIELD(x,0,30)
#define  VTSS_M_PTP_3_IP_1588_LTC_LTC_1PPS_WIDTH_ADJ_LTC_1PPS_WIDTH_ADJ     VTSS_ENCODE_BITMASK(0,30)
#define  VTSS_X_PTP_3_IP_1588_LTC_LTC_1PPS_WIDTH_ADJ_LTC_1PPS_WIDTH_ADJ(x)  VTSS_EXTRACT_BITFIELD(x,0,30)


/** 
 * \brief LTC state machine states
 *
 * \details
 * Register: \a PTP_3:IP_1588_LTC:LTC_STATE_MACHINES
 */
#define VTSS_PTP_3_IP_1588_LTC_LTC_STATE_MACHINES  VTSS_IOREG(0x1e, 1, 0x921c)


/** 
 * \brief LTC one shot load 
 *
 * \details
 * LTC one shot load 
 *
 * Register: \a PTP_3:IP_1588_LTC:LTC_ONE_SHOT_LOAD
 */
#define VTSS_PTP_3_IP_1588_LTC_LTC_ONE_SHOT_LOAD  VTSS_IOREG(0x1e, 1, 0x921d)

/**
 * Register Group: \a PTP_3:TS_FIFO_SI
 *
 * Timestamp FIFO serial interface registers
 */


/** 
 * \brief Timestamp FIFO serial interface configuration register
 *
 * \details
 * Polarity and cycle counts are configurable from port 0 only
 *
 * Register: \a PTP_3:TS_FIFO_SI:TS_FIFO_SI_CFG
 */
#define VTSS_PTP_3_TS_FIFO_SI_TS_FIFO_SI_CFG  VTSS_IOREG(0x1e, 1, 0x9220)

/** 
 * \brief
 * SI clock phase control
 *
 * \details 
 * 0: SI_CLK falling edge changes output data 
 * 1: SI_CLK rising edge changes output data 
 *
 * Field: VTSS_PTP_3_TS_FIFO_SI_TS_FIFO_SI_CFG . SI_CLK_PHA
 */
#define  VTSS_F_PTP_3_TS_FIFO_SI_TS_FIFO_SI_CFG_SI_CLK_PHA  VTSS_BIT(25)

/** 
 * \brief
 * SI clock polarity control
 *
 * \details 
 * 0: SI_CLK starts and ends (idles) low
 * 1: SI_CLK starts and ends (idles) high
 *
 * Field: VTSS_PTP_3_TS_FIFO_SI_TS_FIFO_SI_CFG . SI_CLK_POL
 */
#define  VTSS_F_PTP_3_TS_FIFO_SI_TS_FIFO_SI_CFG_SI_CLK_POL  VTSS_BIT(24)

/** 
 * \brief
 * Number of CSR clock periods SI_EN negates between writes (deselected).
 * The CSR clock period is one-half the REFCLK pin's period.
 *
 * \details 
 * Field: VTSS_PTP_3_TS_FIFO_SI_TS_FIFO_SI_CFG . SI_EN_DES_CYCS
 */
#define  VTSS_F_PTP_3_TS_FIFO_SI_TS_FIFO_SI_CFG_SI_EN_DES_CYCS(x)  VTSS_ENCODE_BITFIELD(x,20,4)
#define  VTSS_M_PTP_3_TS_FIFO_SI_TS_FIFO_SI_CFG_SI_EN_DES_CYCS     VTSS_ENCODE_BITMASK(20,4)
#define  VTSS_X_PTP_3_TS_FIFO_SI_TS_FIFO_SI_CFG_SI_EN_DES_CYCS(x)  VTSS_EXTRACT_BITFIELD(x,20,4)

/** 
 * \brief
 * Number of CSR clock periods that the SI_CLK is high. The CSR clock
 * period is one-half the REFCLK pin's period.
 *
 * \details 
 * Field: VTSS_PTP_3_TS_FIFO_SI_TS_FIFO_SI_CFG . SI_CLK_HI_CYCS
 */
#define  VTSS_F_PTP_3_TS_FIFO_SI_TS_FIFO_SI_CFG_SI_CLK_HI_CYCS(x)  VTSS_ENCODE_BITFIELD(x,6,5)
#define  VTSS_M_PTP_3_TS_FIFO_SI_TS_FIFO_SI_CFG_SI_CLK_HI_CYCS     VTSS_ENCODE_BITMASK(6,5)
#define  VTSS_X_PTP_3_TS_FIFO_SI_TS_FIFO_SI_CFG_SI_CLK_HI_CYCS(x)  VTSS_EXTRACT_BITFIELD(x,6,5)

/** 
 * \brief
 * Number of CSR clock periods that the SI_CLK is low. The CSR clock period
 * is one-half the REFCLK pin's period.
 *
 * \details 
 * Field: VTSS_PTP_3_TS_FIFO_SI_TS_FIFO_SI_CFG . SI_CLK_LO_CYCS
 */
#define  VTSS_F_PTP_3_TS_FIFO_SI_TS_FIFO_SI_CFG_SI_CLK_LO_CYCS(x)  VTSS_ENCODE_BITFIELD(x,1,5)
#define  VTSS_M_PTP_3_TS_FIFO_SI_TS_FIFO_SI_CFG_SI_CLK_LO_CYCS     VTSS_ENCODE_BITMASK(1,5)
#define  VTSS_X_PTP_3_TS_FIFO_SI_TS_FIFO_SI_CFG_SI_CLK_LO_CYCS(x)  VTSS_EXTRACT_BITFIELD(x,1,5)

/** 
 * \brief
 * Timestamp FIFO serial interface block control
 *
 * \details 
 * 0: Disable Timestamp FIFO serial interface block
 * 1: Enable Timestamp FIFO serial interface block
 *
 * Field: VTSS_PTP_3_TS_FIFO_SI_TS_FIFO_SI_CFG . TS_FIFO_SI_ENA
 */
#define  VTSS_F_PTP_3_TS_FIFO_SI_TS_FIFO_SI_CFG_TS_FIFO_SI_ENA  VTSS_BIT(0)


/** 
 * \brief Transmitted timestamp count
 *
 * \details
 * Counter for the number of timestamps transmitted to the interface
 *
 * Register: \a PTP_3:TS_FIFO_SI:TS_FIFO_SI_TX_CNT
 */
#define VTSS_PTP_3_TS_FIFO_SI_TS_FIFO_SI_TX_CNT  VTSS_IOREG(0x1e, 1, 0x9221)

/**
 * Register Group: \a PTP_3:INGR_PREDICTOR
 *
 * Ingress (Rx) registers
 */


/** 
 * \brief Ingress configuration register
 *
 * \details
 * Register: \a PTP_3:INGR_PREDICTOR:IG_CFG
 */
#define VTSS_PTP_3_INGR_PREDICTOR_IG_CFG     VTSS_IOREG(0x1e, 1, 0x9222)

/** 
 * \brief
 * WIS advanced (fixed) value
 * This is the number of register stages in the pipeline from the SFD
 * detection logic through the 1588 IP, the PCS stages until the Rx gearbox
 * and after the gearbox through the WIS until the frame overhead
 * generation logic.
 *
 * \details 
 * Field: VTSS_PTP_3_INGR_PREDICTOR_IG_CFG . WAF
 */
#define  VTSS_F_PTP_3_INGR_PREDICTOR_IG_CFG_WAF(x)  VTSS_ENCODE_BITFIELD(x,16,6)
#define  VTSS_M_PTP_3_INGR_PREDICTOR_IG_CFG_WAF     VTSS_ENCODE_BITMASK(16,6)
#define  VTSS_X_PTP_3_INGR_PREDICTOR_IG_CFG_WAF(x)  VTSS_EXTRACT_BITFIELD(x,16,6)

/** 
 * \brief
 * PCS advanced (fixed) value
 * This is the number of register stages in the pipeline from the SFD
 * detection logic through the 1588 IP, the PCS stages until the Rx
 * gearbox.
 *
 * \details 
 * Field: VTSS_PTP_3_INGR_PREDICTOR_IG_CFG . PAF
 */
#define  VTSS_F_PTP_3_INGR_PREDICTOR_IG_CFG_PAF(x)  VTSS_ENCODE_BITFIELD(x,8,6)
#define  VTSS_M_PTP_3_INGR_PREDICTOR_IG_CFG_PAF     VTSS_ENCODE_BITMASK(8,6)
#define  VTSS_X_PTP_3_INGR_PREDICTOR_IG_CFG_PAF(x)  VTSS_EXTRACT_BITFIELD(x,8,6)

/** 
 * \brief
 * When 1, the Ingress prediction block is enabled.
 *
 * \details 
 * 0: Disabled
 * 1: Enabled
 *
 * Field: VTSS_PTP_3_INGR_PREDICTOR_IG_CFG . IG_ENABLE
 */
#define  VTSS_F_PTP_3_INGR_PREDICTOR_IG_CFG_IG_ENABLE  VTSS_BIT(0)


/** 
 * \brief Period of PMA clock in fractional nanoseconds
 *
 * \details
 * Register: \a PTP_3:INGR_PREDICTOR:IG_PMA
 */
#define VTSS_PTP_3_INGR_PREDICTOR_IG_PMA     VTSS_IOREG(0x1e, 1, 0x9223)

/** 
 * \brief
 * Period in fractional ns of the PMA clock. The binary number is in 9.7
 * format with 9 significant ns bits and 7 fractional ns bits.
 *
 * \details 
 * 9.7 format unsigned fractional binary number
 *
 * Field: VTSS_PTP_3_INGR_PREDICTOR_IG_PMA . TPMA
 */
#define  VTSS_F_PTP_3_INGR_PREDICTOR_IG_PMA_TPMA(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_PTP_3_INGR_PREDICTOR_IG_PMA_TPMA     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_PTP_3_INGR_PREDICTOR_IG_PMA_TPMA(x)  VTSS_EXTRACT_BITFIELD(x,0,16)


/** 
 * \brief XFI delays in nanoseconds
 *
 * \details
 * Register: \a PTP_3:INGR_PREDICTOR:IG_XFI
 */
#define VTSS_PTP_3_INGR_PREDICTOR_IG_XFI     VTSS_IOREG(0x1e, 1, 0x9224)

/** 
 * \brief
 * The time taken for the 32 most significant bits of a 64-bit transaction
 * to pass through the XFI logic. This is because the XFI processes the LS
 * and MS bits separately on a 2x clock.
 *
 * \details 
 * 9.7 format unsigned fractional binary number
 *
 * Field: VTSS_PTP_3_INGR_PREDICTOR_IG_XFI . XFI_MSB
 */
#define  VTSS_F_PTP_3_INGR_PREDICTOR_IG_XFI_XFI_MSB(x)  VTSS_ENCODE_BITFIELD(x,16,16)
#define  VTSS_M_PTP_3_INGR_PREDICTOR_IG_XFI_XFI_MSB     VTSS_ENCODE_BITMASK(16,16)
#define  VTSS_X_PTP_3_INGR_PREDICTOR_IG_XFI_XFI_MSB(x)  VTSS_EXTRACT_BITFIELD(x,16,16)

/** 
 * \brief
 * The time taken for the 32 least significant bits of a 64-bit transaction
 * to pass through the XFI logic. This is because the XFI processes the LS
 * and MS bits separately on a 2x clock.
 *
 * \details 
 * 9.7 format unsigned fractional binary number
 *
 * Field: VTSS_PTP_3_INGR_PREDICTOR_IG_XFI . XFI_LSB
 */
#define  VTSS_F_PTP_3_INGR_PREDICTOR_IG_XFI_XFI_LSB(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_PTP_3_INGR_PREDICTOR_IG_XFI_XFI_LSB     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_PTP_3_INGR_PREDICTOR_IG_XFI_XFI_LSB(x)  VTSS_EXTRACT_BITFIELD(x,0,16)


/** 
 * \brief OTN configuration
 *
 * \details
 * Register: \a PTP_3:INGR_PREDICTOR:IG_OTN
 */
#define VTSS_PTP_3_INGR_PREDICTOR_IG_OTN     VTSS_IOREG(0x1e, 1, 0x9225)

/** 
 * \brief
 * OTN clock gapping and virtual CBR pipe width information
 *
 * \details 
 * GAP_PERIOD[6:2]
 * 0: Virtual CBR pipe, 64 bits wide
 * 1: Virtual CBR pipe, 32 bits wide	   
 * 2: Virtual CBR pipe, 1 bit wide
 * others: Undefined	   
 * GAP_PERIOD[1:0]
 * 1: 1/15 OTN gapped clock.
 * 2: 6/85 OTN gapped clock
 * others: No gapping
 *
 * Field: VTSS_PTP_3_INGR_PREDICTOR_IG_OTN . GAP_PERIOD
 */
#define  VTSS_F_PTP_3_INGR_PREDICTOR_IG_OTN_GAP_PERIOD(x)  VTSS_ENCODE_BITFIELD(x,0,7)
#define  VTSS_M_PTP_3_INGR_PREDICTOR_IG_OTN_GAP_PERIOD     VTSS_ENCODE_BITMASK(0,7)
#define  VTSS_X_PTP_3_INGR_PREDICTOR_IG_OTN_GAP_PERIOD(x)  VTSS_EXTRACT_BITFIELD(x,0,7)

/**
 * Register Group: \a PTP_3:EGR_PREDICTOR
 *
 * Egress (Tx) registers
 */


/** 
 * \brief Egress configuration register
 *
 * \details
 * Register: \a PTP_3:EGR_PREDICTOR:EG_CFG
 */
#define VTSS_PTP_3_EGR_PREDICTOR_EG_CFG      VTSS_IOREG(0x1e, 1, 0x9226)

/** 
 * \brief
 * WIS advanced (fixed) value
 * This is the number of register stages in the pipeline from the SFD
 * detection logic through the 1588 IP, the PCS stages until the Tx gearbox
 * and after the gearbox through the WIS until the frame overhead
 * generation logic.
 *
 * \details 
 * Field: VTSS_PTP_3_EGR_PREDICTOR_EG_CFG . WAF
 */
#define  VTSS_F_PTP_3_EGR_PREDICTOR_EG_CFG_WAF(x)  VTSS_ENCODE_BITFIELD(x,16,6)
#define  VTSS_M_PTP_3_EGR_PREDICTOR_EG_CFG_WAF     VTSS_ENCODE_BITMASK(16,6)
#define  VTSS_X_PTP_3_EGR_PREDICTOR_EG_CFG_WAF(x)  VTSS_EXTRACT_BITFIELD(x,16,6)

/** 
 * \brief
 * PCS advanced (fixed) value
 * This is the number of register stages in the pipeline from the SFD
 * detection logic through the 1588 IP, the PCS stages until the Tx
 * gearbox.
 *
 * \details 
 * Field: VTSS_PTP_3_EGR_PREDICTOR_EG_CFG . PAF
 */
#define  VTSS_F_PTP_3_EGR_PREDICTOR_EG_CFG_PAF(x)  VTSS_ENCODE_BITFIELD(x,8,6)
#define  VTSS_M_PTP_3_EGR_PREDICTOR_EG_CFG_PAF     VTSS_ENCODE_BITMASK(8,6)
#define  VTSS_X_PTP_3_EGR_PREDICTOR_EG_CFG_PAF(x)  VTSS_EXTRACT_BITFIELD(x,8,6)

/** 
 * \brief
 * When 1, the Egress prediction block is enabled.
 *
 * \details 
 * 0: Disabled
 * 1: Enabled
 *
 * Field: VTSS_PTP_3_EGR_PREDICTOR_EG_CFG . EG_ENABLE
 */
#define  VTSS_F_PTP_3_EGR_PREDICTOR_EG_CFG_EG_ENABLE  VTSS_BIT(0)


/** 
 * \brief Egress WIS frame characteristics in clocks
 *
 * \details
 * Register: \a PTP_3:EGR_PREDICTOR:EG_WIS_FRAME
 */
#define VTSS_PTP_3_EGR_PREDICTOR_EG_WIS_FRAME  VTSS_IOREG(0x1e, 1, 0x9227)

/** 
 * \brief
 * WIS overhead time in clock cycles
 * This is the number of clocks in the Egress WIS overhead time. Typically
 * 80.
 *
 * \details 
 * Field: VTSS_PTP_3_EGR_PREDICTOR_EG_WIS_FRAME . W_OH
 */
#define  VTSS_F_PTP_3_EGR_PREDICTOR_EG_WIS_FRAME_W_OH(x)  VTSS_ENCODE_BITFIELD(x,16,8)
#define  VTSS_M_PTP_3_EGR_PREDICTOR_EG_WIS_FRAME_W_OH     VTSS_ENCODE_BITMASK(16,8)
#define  VTSS_X_PTP_3_EGR_PREDICTOR_EG_WIS_FRAME_W_OH(x)  VTSS_EXTRACT_BITFIELD(x,16,8)

/** 
 * \brief
 * Size of the WIS frame in clocks. Typically 2160.
 *
 * \details 
 * Field: VTSS_PTP_3_EGR_PREDICTOR_EG_WIS_FRAME . W_FSIZE
 */
#define  VTSS_F_PTP_3_EGR_PREDICTOR_EG_WIS_FRAME_W_FSIZE(x)  VTSS_ENCODE_BITFIELD(x,0,12)
#define  VTSS_M_PTP_3_EGR_PREDICTOR_EG_WIS_FRAME_W_FSIZE     VTSS_ENCODE_BITMASK(0,12)
#define  VTSS_X_PTP_3_EGR_PREDICTOR_EG_WIS_FRAME_W_FSIZE(x)  VTSS_EXTRACT_BITFIELD(x,0,12)


/** 
 * \brief Egress WIS delays in nanoseconds
 *
 * \details
 * Register: \a PTP_3:EGR_PREDICTOR:EG_WIS_DELAYS
 */
#define VTSS_PTP_3_EGR_PREDICTOR_EG_WIS_DELAYS  VTSS_IOREG(0x1e, 1, 0x9228)

/** 
 * \brief
 * Duration of the WIS overhead in fractional nanoseconds
 * The number is in 12.4 format with 12 nanosecond bits and 4 fractional
 * nanosecond bits.
 *
 * \details 
 * 12.4 format unsigned fractional binary number
 *
 * Field: VTSS_PTP_3_EGR_PREDICTOR_EG_WIS_DELAYS . W_OH_NS
 */
#define  VTSS_F_PTP_3_EGR_PREDICTOR_EG_WIS_DELAYS_W_OH_NS(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_PTP_3_EGR_PREDICTOR_EG_WIS_DELAYS_W_OH_NS     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_PTP_3_EGR_PREDICTOR_EG_WIS_DELAYS_W_OH_NS(x)  VTSS_EXTRACT_BITFIELD(x,0,16)


/** 
 * \brief Egress PMA clock delay
 *
 * \details
 * Register: \a PTP_3:EGR_PREDICTOR:EG_PMA
 */
#define VTSS_PTP_3_EGR_PREDICTOR_EG_PMA      VTSS_IOREG(0x1e, 1, 0x9229)

/** 
 * \brief
 * PMA clock period in fractional nanoseconds
 * This period is for the clock that drives the PMA, PCS and WIS and is in
 * 9.7 format with 9 significant ns bits and 7 fractional ns bits.
 *
 * \details 
 * 9.7 format unsigned fractional binary number
 *
 * Field: VTSS_PTP_3_EGR_PREDICTOR_EG_PMA . TPMA
 */
#define  VTSS_F_PTP_3_EGR_PREDICTOR_EG_PMA_TPMA(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_PTP_3_EGR_PREDICTOR_EG_PMA_TPMA     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_PTP_3_EGR_PREDICTOR_EG_PMA_TPMA(x)  VTSS_EXTRACT_BITFIELD(x,0,16)


/** 
 * \brief XFI delays in nanoseconds
 *
 * \details
 * Register: \a PTP_3:EGR_PREDICTOR:EG_XFI
 */
#define VTSS_PTP_3_EGR_PREDICTOR_EG_XFI      VTSS_IOREG(0x1e, 1, 0x922a)

/** 
 * \brief
 * The time taken for the 32 most significant bits of a 64-bit transaction
 * to pass through the XFI logic. This is because the XFI processes the LS
 * and MS bits separately on a 2x clock.
 *
 * \details 
 * 9.7 format unsigned fractional binary number
 *
 * Field: VTSS_PTP_3_EGR_PREDICTOR_EG_XFI . XFI_MSB
 */
#define  VTSS_F_PTP_3_EGR_PREDICTOR_EG_XFI_XFI_MSB(x)  VTSS_ENCODE_BITFIELD(x,16,16)
#define  VTSS_M_PTP_3_EGR_PREDICTOR_EG_XFI_XFI_MSB     VTSS_ENCODE_BITMASK(16,16)
#define  VTSS_X_PTP_3_EGR_PREDICTOR_EG_XFI_XFI_MSB(x)  VTSS_EXTRACT_BITFIELD(x,16,16)

/** 
 * \brief
 * The time taken for the 32 least significant bits of a 64-bit transaction
 * to pass through the XFI logic. This is because the XFI processes the LS
 * and MS bits separately on a 2x clock.
 *
 * \details 
 * 9.7 format unsigned fractional binary number
 *
 * Field: VTSS_PTP_3_EGR_PREDICTOR_EG_XFI . XFI_LSB
 */
#define  VTSS_F_PTP_3_EGR_PREDICTOR_EG_XFI_XFI_LSB(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_PTP_3_EGR_PREDICTOR_EG_XFI_XFI_LSB     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_PTP_3_EGR_PREDICTOR_EG_XFI_XFI_LSB(x)  VTSS_EXTRACT_BITFIELD(x,0,16)


/** 
 * \brief OTN configuration
 *
 * \details
 * Register: \a PTP_3:EGR_PREDICTOR:EG_OTN
 */
#define VTSS_PTP_3_EGR_PREDICTOR_EG_OTN      VTSS_IOREG(0x1e, 1, 0x922b)

/** 
 * \brief
 * OTN clock gapping and virtual CBR pipe width information 
 *
 * \details 
 * GAP_PERIOD[6:2]
 * 0: Virtual CBR pipe, 64 bits wide
 * 1: Virtual CBR pipe, 32 bits wide	   
 * 2: Virtual CBR pipe, 1 bit wide
 * others: Undefined	   
 * GAP_PERIOD[1:0]
 * 1: 1/15 OTN gapped clock.
 * 2: 6/85 OTN gapped clock
 * others: PMA clock not gapped
 *
 * Field: VTSS_PTP_3_EGR_PREDICTOR_EG_OTN . GAP_PERIOD
 */
#define  VTSS_F_PTP_3_EGR_PREDICTOR_EG_OTN_GAP_PERIOD(x)  VTSS_ENCODE_BITFIELD(x,0,7)
#define  VTSS_M_PTP_3_EGR_PREDICTOR_EG_OTN_GAP_PERIOD     VTSS_ENCODE_BITMASK(0,7)
#define  VTSS_X_PTP_3_EGR_PREDICTOR_EG_OTN_GAP_PERIOD(x)  VTSS_EXTRACT_BITFIELD(x,0,7)

/**
 * Register Group: \a PTP_3:MISC_CFG
 *
 * Miscellaneous chip-specific configuration and control signals
 */


/** 
 * \brief Miscellaneous configuration and control signals
 *
 * \details
 * Register: \a PTP_3:MISC_CFG:CFG
 */
#define VTSS_PTP_3_MISC_CFG_CFG              VTSS_IOREG(0x1e, 1, 0x922c)

/** 
 * \brief
 * Controls an external mux to select the SOF detection pulses for output
 * instead of the 1 pulse-per-second (1 pps) signals. Either the ingress or
 * egress SOF detection pulses can be selected instead of the 1 pps signal
 * for output. Typically used for debugging.
 *
 * \details 
 * 0: 1 PPS output
 * 1: Ingress SOF Detect output
 * 2: Egress SOF Detect output
 * 3: INVALID
 *
 * Field: VTSS_PTP_3_MISC_CFG_CFG . SOF_OUT_SEL
 */
#define  VTSS_F_PTP_3_MISC_CFG_CFG_SOF_OUT_SEL(x)  VTSS_ENCODE_BITFIELD(x,0,2)
#define  VTSS_M_PTP_3_MISC_CFG_CFG_SOF_OUT_SEL     VTSS_ENCODE_BITMASK(0,2)
#define  VTSS_X_PTP_3_MISC_CFG_CFG_SOF_OUT_SEL(x)  VTSS_EXTRACT_BITFIELD(x,0,2)

/**
 * Register Group: \a PTP_3:INGR_IP_1588_CFG_STAT
 *
 * 1588 IP control and status registers
 */


/** 
 * \brief 1588 IP interrupt status register
 *
 * \details
 * Status sticky conditions for the 1588 IP
 *
 * Register: \a PTP_3:INGR_IP_1588_CFG_STAT:INGR_INT_STATUS
 */
#define VTSS_PTP_3_INGR_IP_1588_CFG_STAT_INGR_INT_STATUS  VTSS_IOREG(0x1e, 1, 0x922d)

/** 
 * \brief
 * When set, indicates an underflow in the bypass logic. The sticky bit
 * should be reset by writing it to 1.
 *
 * \details 
 * Field: VTSS_PTP_3_INGR_IP_1588_CFG_STAT_INGR_INT_STATUS . INGR_BYPASS_UNDERFLOW_STICKY
 */
#define  VTSS_F_PTP_3_INGR_IP_1588_CFG_STAT_INGR_INT_STATUS_INGR_BYPASS_UNDERFLOW_STICKY  VTSS_BIT(15)

/** 
 * \brief
 * When set, indicates an overflow in the bypass logic. The sticky bit
 * should be reset by writing it to 1.
 *
 * \details 
 * Field: VTSS_PTP_3_INGR_IP_1588_CFG_STAT_INGR_INT_STATUS . INGR_BYPASS_OVERFLOW_STICKY
 */
#define  VTSS_F_PTP_3_INGR_IP_1588_CFG_STAT_INGR_INT_STATUS_INGR_BYPASS_OVERFLOW_STICKY  VTSS_BIT(14)

/** 
 * \brief
 * When set, indicates an underflow in the analyzer command processor. The
 * sticky bit should be reset by writing it to 1.
 *
 * \details 
 * Field: VTSS_PTP_3_INGR_IP_1588_CFG_STAT_INGR_INT_STATUS . INGR_ANA_CMD_UNDERFLOW_STICKY
 */
#define  VTSS_F_PTP_3_INGR_IP_1588_CFG_STAT_INGR_INT_STATUS_INGR_ANA_CMD_UNDERFLOW_STICKY  VTSS_BIT(13)

/** 
 * \brief
 * When set, indicates an overflow in the analyzer command processor. The
 * sticky bit should be reset by writing it to 1.
 *
 * \details 
 * Field: VTSS_PTP_3_INGR_IP_1588_CFG_STAT_INGR_INT_STATUS . INGR_ANA_CMD_OVERFLOW_STICKY
 */
#define  VTSS_F_PTP_3_INGR_IP_1588_CFG_STAT_INGR_INT_STATUS_INGR_ANA_CMD_OVERFLOW_STICKY  VTSS_BIT(12)

/** 
 * \brief
 * When set, indicates an a frame was processed by rewriter command before
 * SOF arrives. Increase the fifo depth. The sticky bit should be reset by
 * writing it to 1.
 *
 * \details 
 * Field: VTSS_PTP_3_INGR_IP_1588_CFG_STAT_INGR_INT_STATUS . INGR_TSP_CMD_UNDERFLOW_STICKY
 */
#define  VTSS_F_PTP_3_INGR_IP_1588_CFG_STAT_INGR_INT_STATUS_INGR_TSP_CMD_UNDERFLOW_STICKY  VTSS_BIT(11)

/** 
 * \brief
 * When set, indicates an overflow in the Timestamp command processor. The
 * sticky bit should be reset by writing it to 1.
 *
 * \details 
 * Field: VTSS_PTP_3_INGR_IP_1588_CFG_STAT_INGR_INT_STATUS . INGR_TSP_CMD_OVERFLOW_STICKY
 */
#define  VTSS_F_PTP_3_INGR_IP_1588_CFG_STAT_INGR_INT_STATUS_INGR_TSP_CMD_OVERFLOW_STICKY  VTSS_BIT(10)

/** 
 * \brief
 * When set, indicates an a frame was processed by analyzer command before
 * a timestamp was taken. Underflow in the Timestamp. The sticky bit should
 * be reset by writing it to 1.
 *
 * \details 
 * Field: VTSS_PTP_3_INGR_IP_1588_CFG_STAT_INGR_INT_STATUS . INGR_TSP_FIFO_UNDERFLOW_STICKY
 */
#define  VTSS_F_PTP_3_INGR_IP_1588_CFG_STAT_INGR_INT_STATUS_INGR_TSP_FIFO_UNDERFLOW_STICKY  VTSS_BIT(9)

/** 
 * \brief
 * When set, indicates an overflow in the Timestamp processor. More than
 * four frames within 1588. The sticky bit should be reset by writing it to
 * 1.
 *
 * \details 
 * Field: VTSS_PTP_3_INGR_IP_1588_CFG_STAT_INGR_INT_STATUS . INGR_TSP_FIFO_OVERFLOW_STICKY
 */
#define  VTSS_F_PTP_3_INGR_IP_1588_CFG_STAT_INGR_INT_STATUS_INGR_TSP_FIFO_OVERFLOW_STICKY  VTSS_BIT(8)

/** 
 * \brief
 * When set indicates that a frame was received with non-zero data in the
 * reserved field. The sticky bit should be reset by writing it to 1.
 *
 * \details 
 * 0 = No data in reserved field
 * 1 = Data in reserved field
 *
 * Field: VTSS_PTP_3_INGR_IP_1588_CFG_STAT_INGR_INT_STATUS . INGR_RSVD_NONZERO_STICKY
 */
#define  VTSS_F_PTP_3_INGR_IP_1588_CFG_STAT_INGR_INT_STATUS_INGR_RSVD_NONZERO_STICKY  VTSS_BIT(7)

/** 
 * \brief
 * Indicates that more than one engine has produced a match. The sticky bit
 * should be reset by writing it to 1.
 *
 * \details 
 * 0: No error found
 * 1: Duplicate match found
 *
 * Field: VTSS_PTP_3_INGR_IP_1588_CFG_STAT_INGR_INT_STATUS . INGR_ANALYZER_ERROR_STICKY
 */
#define  VTSS_F_PTP_3_INGR_IP_1588_CFG_STAT_INGR_INT_STATUS_INGR_ANALYZER_ERROR_STICKY  VTSS_BIT(6)

/** 
 * \brief
 * When set, indicates that a preamble that was too short to modify was
 * detected in a PTP frame. Write to 1 to clear. This occurs when the
 * rewriter needs to shrink the preamble to append a timestamp but cannot
 * because the preamble is too short. A short preamble is any preamble that
 * is less than eight characters long including the XGMII /S/ character and
 * the ending SFD of 0xD5. Other preamble values are not checked, only the
 * GMII length.
 *
 * \details 
 * 0: No error
 * 1: Preamble too short error
 *
 * Field: VTSS_PTP_3_INGR_IP_1588_CFG_STAT_INGR_INT_STATUS . INGR_RW_PREAMBLE_ERR_STICKY
 */
#define  VTSS_F_PTP_3_INGR_IP_1588_CFG_STAT_INGR_INT_STATUS_INGR_RW_PREAMBLE_ERR_STICKY  VTSS_BIT(5)

/** 
 * \brief
 * When set, indicates that an FCS error was detected in a PTP/OAM frame.
 * Write to 1 to clear.
 *
 * \details 
 * 0: No error
 * 1: FCS error
 *
 * Field: VTSS_PTP_3_INGR_IP_1588_CFG_STAT_INGR_INT_STATUS . INGR_RW_FCS_ERR_STICKY
 */
#define  VTSS_F_PTP_3_INGR_IP_1588_CFG_STAT_INGR_INT_STATUS_INGR_RW_FCS_ERR_STICKY  VTSS_BIT(4)

/** 
 * \brief
 * When set, indicates that the level in the Timestamp FIFO has reached the
 * threshold TS_THRESH. The sticky bit should be reset by writing it to 1.
 *
 * \details 
 * 0: No overflow
 * 1: Overflow
 *
 * Field: VTSS_PTP_3_INGR_IP_1588_CFG_STAT_INGR_INT_STATUS . INGR_TS_LEVEL_STICKY
 */
#define  VTSS_F_PTP_3_INGR_IP_1588_CFG_STAT_INGR_INT_STATUS_INGR_TS_LEVEL_STICKY  VTSS_BIT(3)

/** 
 * \brief
 * When set, indicates a timestamp was captured in the Timestamp FIFO. The
 * sticky bit should be reset by writing it to 1.
 *
 * \details 
 * 0: No overflow
 * 1: Overflow
 *
 * Field: VTSS_PTP_3_INGR_IP_1588_CFG_STAT_INGR_INT_STATUS . INGR_TS_LOADED_STICKY
 */
#define  VTSS_F_PTP_3_INGR_IP_1588_CFG_STAT_INGR_INT_STATUS_INGR_TS_LOADED_STICKY  VTSS_BIT(2)

/** 
 * \brief
 * When set, indicates an underflow in the Timestamp FIFO. The sticky bit
 * should be reset by writing it to 1.
 *
 * \details 
 * 0: No overflow
 * 1: Overflow
 *
 * Field: VTSS_PTP_3_INGR_IP_1588_CFG_STAT_INGR_INT_STATUS . INGR_TS_UNDERFLOW_STICKY
 */
#define  VTSS_F_PTP_3_INGR_IP_1588_CFG_STAT_INGR_INT_STATUS_INGR_TS_UNDERFLOW_STICKY  VTSS_BIT(1)

/** 
 * \brief
 * When set, indicates an overflow in the Timestamp FIFO. The sticky bit
 * should be reset by writing it to 1.
 *
 * \details 
 * 0: No overflow
 * 1: Overflow
 *
 * Field: VTSS_PTP_3_INGR_IP_1588_CFG_STAT_INGR_INT_STATUS . INGR_TS_OVERFLOW_STICKY
 */
#define  VTSS_F_PTP_3_INGR_IP_1588_CFG_STAT_INGR_INT_STATUS_INGR_TS_OVERFLOW_STICKY  VTSS_BIT(0)


/** 
 * \brief 1588 IP interrupt mask register
 *
 * \details
 * Masks that enable and disable the interrupts
 *
 * Register: \a PTP_3:INGR_IP_1588_CFG_STAT:INGR_INT_MASK
 */
#define VTSS_PTP_3_INGR_IP_1588_CFG_STAT_INGR_INT_MASK  VTSS_IOREG(0x1e, 1, 0x922e)

/** 
 * \brief
 * Mask for BYPASS_UNDERFLOW_STICKY bit
 *
 * \details 
 * 0: Interrupt disabled
 * 1: Interrupt enabled
 *
 * Field: VTSS_PTP_3_INGR_IP_1588_CFG_STAT_INGR_INT_MASK . INGR_BYPASS_UNDERFLOW_MASK
 */
#define  VTSS_F_PTP_3_INGR_IP_1588_CFG_STAT_INGR_INT_MASK_INGR_BYPASS_UNDERFLOW_MASK  VTSS_BIT(15)

/** 
 * \brief
 * Mask for BYPASS_OVERFLOW_STICKY bit. This mask is also shared for
 * TSP_PUSH_FLAG interrupt.
 *
 * \details 
 * 0: Interrupt disabled
 * 1: Interrupt enabled
 *
 * Field: VTSS_PTP_3_INGR_IP_1588_CFG_STAT_INGR_INT_MASK . INGR_BYPASS_OVERFLOW_MASK
 */
#define  VTSS_F_PTP_3_INGR_IP_1588_CFG_STAT_INGR_INT_MASK_INGR_BYPASS_OVERFLOW_MASK  VTSS_BIT(14)

/** 
 * \brief
 * Mask for ANA_CMD_UNDERFLOW_STICKY bit
 *
 * \details 
 * 0: Interrupt disabled
 * 1: Interrupt enabled
 *
 * Field: VTSS_PTP_3_INGR_IP_1588_CFG_STAT_INGR_INT_MASK . INGR_ANA_CMD_UNDERFLOW_MASK
 */
#define  VTSS_F_PTP_3_INGR_IP_1588_CFG_STAT_INGR_INT_MASK_INGR_ANA_CMD_UNDERFLOW_MASK  VTSS_BIT(13)

/** 
 * \brief
 * Mask for ANA_CMD_OVERFLOW_STICKY bit
 *
 * \details 
 * 0: Interrupt disabled
 * 1: Interrupt enabled
 *
 * Field: VTSS_PTP_3_INGR_IP_1588_CFG_STAT_INGR_INT_MASK . INGR_ANA_CMD_OVERFLOW_MASK
 */
#define  VTSS_F_PTP_3_INGR_IP_1588_CFG_STAT_INGR_INT_MASK_INGR_ANA_CMD_OVERFLOW_MASK  VTSS_BIT(12)

/** 
 * \brief
 * Mask bit for TSP_CMD_UNDERFLOW_STICKY bit
 *
 * \details 
 * 0: Interrupt disabled
 * 1: Interrupt enabled
 *
 * Field: VTSS_PTP_3_INGR_IP_1588_CFG_STAT_INGR_INT_MASK . INGR_TSP_CMD_UNDERFLOW_MASK
 */
#define  VTSS_F_PTP_3_INGR_IP_1588_CFG_STAT_INGR_INT_MASK_INGR_TSP_CMD_UNDERFLOW_MASK  VTSS_BIT(11)

/** 
 * \brief
 * Mask bit for TSP_CMD_OVERFLOW_STICKY bit
 *
 * \details 
 * 0: Interrupt disabled
 * 1: Interrupt enabled
 *
 * Field: VTSS_PTP_3_INGR_IP_1588_CFG_STAT_INGR_INT_MASK . INGR_TSP_CMD_OVERFLOW_MASK
 */
#define  VTSS_F_PTP_3_INGR_IP_1588_CFG_STAT_INGR_INT_MASK_INGR_TSP_CMD_OVERFLOW_MASK  VTSS_BIT(10)

/** 
 * \brief
 * Mask for TSP_FIFO_UNDERFLOW_STICKY bit
 *
 * \details 
 * 0: Interrupt disabled
 * 1: Interrupt enabled
 *
 * Field: VTSS_PTP_3_INGR_IP_1588_CFG_STAT_INGR_INT_MASK . INGR_TSP_FIFO_UNDERFLOW_MASK
 */
#define  VTSS_F_PTP_3_INGR_IP_1588_CFG_STAT_INGR_INT_MASK_INGR_TSP_FIFO_UNDERFLOW_MASK  VTSS_BIT(9)

/** 
 * \brief
 * Mask bit for TSP_FIFO_OVERFLOW_STICKY bit
 *
 * \details 
 * 0: Interrupt disabled
 * 1: Interrupt enabled
 *
 * Field: VTSS_PTP_3_INGR_IP_1588_CFG_STAT_INGR_INT_MASK . INGR_TSP_FIFO_OVERFLOW_MASK
 */
#define  VTSS_F_PTP_3_INGR_IP_1588_CFG_STAT_INGR_INT_MASK_INGR_TSP_FIFO_OVERFLOW_MASK  VTSS_BIT(8)

/** 
 * \brief
 * Mask bit for RSVD_NONZERO_STICKY bit
 *
 * \details 
 * 0: Interrupt disabled
 * 1: Interrupt enabled
 *
 * Field: VTSS_PTP_3_INGR_IP_1588_CFG_STAT_INGR_INT_MASK . INGR_RSVD_NONZERO_MASK
 */
#define  VTSS_F_PTP_3_INGR_IP_1588_CFG_STAT_INGR_INT_MASK_INGR_RSVD_NONZERO_MASK  VTSS_BIT(7)

/** 
 * \brief
 * Mask bit for ANALYZER_ERROR_STICKY bit
 *
 * \details 
 * 0: Interrupt disabled
 * 1: Interrupt enabled
 *
 * Field: VTSS_PTP_3_INGR_IP_1588_CFG_STAT_INGR_INT_MASK . INGR_ANALYZER_ERROR_MASK
 */
#define  VTSS_F_PTP_3_INGR_IP_1588_CFG_STAT_INGR_INT_MASK_INGR_ANALYZER_ERROR_MASK  VTSS_BIT(6)

/** 
 * \brief
 * Mask for the RW_PREAMBLE_ERR_STICKY bit
 *
 * \details 
 * 0: Interrupt disabled
 * 1: Interrupt enabled
 *
 * Field: VTSS_PTP_3_INGR_IP_1588_CFG_STAT_INGR_INT_MASK . INGR_RW_PREAMBLE_ERR_MASK
 */
#define  VTSS_F_PTP_3_INGR_IP_1588_CFG_STAT_INGR_INT_MASK_INGR_RW_PREAMBLE_ERR_MASK  VTSS_BIT(5)

/** 
 * \brief
 * Mask for the RW_FCS_ERR_STICKY bit
 *
 * \details 
 * 0: Interrupt disabled
 * 1: Interrupt enabled
 *
 * Field: VTSS_PTP_3_INGR_IP_1588_CFG_STAT_INGR_INT_MASK . INGR_RW_FCS_ERR_MASK
 */
#define  VTSS_F_PTP_3_INGR_IP_1588_CFG_STAT_INGR_INT_MASK_INGR_RW_FCS_ERR_MASK  VTSS_BIT(4)

/** 
 * \brief
 * Mask bit for TS_LEVEL_STICKY. When 1, the interrupt is enabled.
 *
 * \details 
 * 0: Interrupt disabled
 * 1: Interrupt enabled
 *
 * Field: VTSS_PTP_3_INGR_IP_1588_CFG_STAT_INGR_INT_MASK . INGR_TS_LEVEL_MASK
 */
#define  VTSS_F_PTP_3_INGR_IP_1588_CFG_STAT_INGR_INT_MASK_INGR_TS_LEVEL_MASK  VTSS_BIT(3)

/** 
 * \brief
 * Mask bit for TS_LOADED_STICKY. When 1, the interrupt is enabled.
 *
 * \details 
 * 0: Interrupt disabled
 * 1: Interrupt enabled
 *
 * Field: VTSS_PTP_3_INGR_IP_1588_CFG_STAT_INGR_INT_MASK . INGR_TS_LOADED_MASK
 */
#define  VTSS_F_PTP_3_INGR_IP_1588_CFG_STAT_INGR_INT_MASK_INGR_TS_LOADED_MASK  VTSS_BIT(2)

/** 
 * \brief
 * Mask bit for TS_UNDERFLOW_STICKY. When 1, the interrupt is enabled.
 *
 * \details 
 * 0: Interrupt disabled
 * 1: Interrupt enabled
 *
 * Field: VTSS_PTP_3_INGR_IP_1588_CFG_STAT_INGR_INT_MASK . INGR_TS_UNDERFLOW_MASK
 */
#define  VTSS_F_PTP_3_INGR_IP_1588_CFG_STAT_INGR_INT_MASK_INGR_TS_UNDERFLOW_MASK  VTSS_BIT(1)

/** 
 * \brief
 * Mask bit for TS_OVERFLOW_STICKY. When 1, the interrupt is enabled.
 *
 * \details 
 * 0: Interrupt disabled
 * 1: Interrupt enabled
 *
 * Field: VTSS_PTP_3_INGR_IP_1588_CFG_STAT_INGR_INT_MASK . INGR_TS_OVERFLOW_MASK
 */
#define  VTSS_F_PTP_3_INGR_IP_1588_CFG_STAT_INGR_INT_MASK_INGR_TS_OVERFLOW_MASK  VTSS_BIT(0)


/** 
 * \brief Egress side stall latency
 *
 * \details
 * Register: \a PTP_3:INGR_IP_1588_CFG_STAT:INGR_CFG_STALL_LATENCY
 */
#define VTSS_PTP_3_INGR_IP_1588_CFG_STAT_INGR_CFG_STALL_LATENCY  VTSS_IOREG(0x1e, 1, 0x922f)

/**
 * Register Group: \a PTP_3:INGR_IP_1588_TSP
 *
 * 1588 IP timestamp processor
 */


/** 
 * \brief TSP control
 *
 * \details
 * Register: \a PTP_3:INGR_IP_1588_TSP:INGR_TSP_CTRL
 */
#define VTSS_PTP_3_INGR_IP_1588_TSP_INGR_TSP_CTRL  VTSS_IOREG(0x1e, 1, 0x9235)

/** 
 * \brief
 * Selects a mode in which the fractional portion of a second (in units of
 * nanoseconds) is used for timestamping. Only the operation of the
 * WRITE_NS, WRITE_NS_P2P, and SUB_ADD PTP commands are affected by the
 * setting of this mode bit.
 *
 * \details 
 * 0: Select the total (summed) nanoseconds for timestamping
 * 1: Select the fractional portion in nanoseconds for timestamping
 *
 * Field: VTSS_PTP_3_INGR_IP_1588_TSP_INGR_TSP_CTRL . INGR_FRACT_NS_MODE
 */
#define  VTSS_F_PTP_3_INGR_IP_1588_TSP_INGR_TSP_CTRL_INGR_FRACT_NS_MODE  VTSS_BIT(2)

/** 
 * \brief
 * Select external pin start of frame indicator
 *
 * \details 
 * 0: Select internal PCS as the source of SOF
 * 1: Select external pin as the source of SOF
 *
 * Field: VTSS_PTP_3_INGR_IP_1588_TSP_INGR_TSP_CTRL . INGR_SEL_EXT_SOF_IND
 */
#define  VTSS_F_PTP_3_INGR_IP_1588_TSP_INGR_TSP_CTRL_INGR_SEL_EXT_SOF_IND  VTSS_BIT(1)

/** 
 * \brief
 * One-shot loads local latency, path delay, and DelayAsymmetry values into
 * the timestamp processor
 *
 * \details 
 * Field: VTSS_PTP_3_INGR_IP_1588_TSP_INGR_TSP_CTRL . INGR_LOAD_DELAYS
 */
#define  VTSS_F_PTP_3_INGR_IP_1588_TSP_INGR_TSP_CTRL_INGR_LOAD_DELAYS  VTSS_BIT(0)


/** 
 * \brief TSP status
 *
 * \details
 * Register: \a PTP_3:INGR_IP_1588_TSP:INGR_TSP_STAT
 */
#define VTSS_PTP_3_INGR_IP_1588_TSP_INGR_TSP_STAT  VTSS_IOREG(0x1e, 1, 0x9236)

/** 
 * \brief
 * Timestamp processor marked a calculated correction field as too big
 *
 * \details 
 * 0: A calculated correction field that was too big did occur
 * 1: A calculated correction field that was too big did not occur
 *
 * Field: VTSS_PTP_3_INGR_IP_1588_TSP_INGR_TSP_STAT . INGR_CF_TOO_BIG_STICKY
 */
#define  VTSS_F_PTP_3_INGR_IP_1588_TSP_INGR_TSP_STAT_INGR_CF_TOO_BIG_STICKY  VTSS_BIT(0)


/** 
 * \brief Local latency
 *
 * \details
 * Register: \a PTP_3:INGR_IP_1588_TSP:INGR_LOCAL_LATENCY
 */
#define VTSS_PTP_3_INGR_IP_1588_TSP_INGR_LOCAL_LATENCY  VTSS_IOREG(0x1e, 1, 0x9237)

/** 
 * \brief
 * Local latency (nanoseconds)
 *
 * \details 
 * Field: VTSS_PTP_3_INGR_IP_1588_TSP_INGR_LOCAL_LATENCY . INGR_LOCAL_LATENCY
 */
#define  VTSS_F_PTP_3_INGR_IP_1588_TSP_INGR_LOCAL_LATENCY_INGR_LOCAL_LATENCY(x)  VTSS_ENCODE_BITFIELD(x,0,23)
#define  VTSS_M_PTP_3_INGR_IP_1588_TSP_INGR_LOCAL_LATENCY_INGR_LOCAL_LATENCY     VTSS_ENCODE_BITMASK(0,23)
#define  VTSS_X_PTP_3_INGR_IP_1588_TSP_INGR_LOCAL_LATENCY_INGR_LOCAL_LATENCY(x)  VTSS_EXTRACT_BITFIELD(x,0,23)


/** 
 * \brief Path delay
 *
 * \details
 * Register: \a PTP_3:INGR_IP_1588_TSP:INGR_PATH_DELAY
 */
#define VTSS_PTP_3_INGR_IP_1588_TSP_INGR_PATH_DELAY  VTSS_IOREG(0x1e, 1, 0x9238)


/** 
 * \brief Delay asymmetry
 *
 * \details
 * Register: \a PTP_3:INGR_IP_1588_TSP:INGR_DELAY_ASYMMETRY
 */
#define VTSS_PTP_3_INGR_IP_1588_TSP_INGR_DELAY_ASYMMETRY  VTSS_IOREG(0x1e, 1, 0x9239)

/**
 * Register Group: \a PTP_3:INGR_IP_1588_DF
 *
 * 1588 IP delay FIFO
 */


/** 
 * \brief Configuration and control register for the delay FIFO
 *
 * \details
 * Register: \a PTP_3:INGR_IP_1588_DF:INGR_DF_CTRL
 */
#define VTSS_PTP_3_INGR_IP_1588_DF_INGR_DF_CTRL  VTSS_IOREG(0x1e, 1, 0x923a)

/** 
 * \brief
 * The index of the register stage in the delay FIFO that is used for
 * output. The actual delay through the block is one more than the depth.
 * If depth is set to 2, then the delay is 3 clocks as data is taken from
 * stage 2. The depth MUST be greater than 0 (depth of 0 is not allowed)
 * and MUST be less than 20 (depth of 20 not allowed).
 *
 * \details 
 * Binary number >= 1
 *
 * Field: VTSS_PTP_3_INGR_IP_1588_DF_INGR_DF_CTRL . INGR_DF_DEPTH
 */
#define  VTSS_F_PTP_3_INGR_IP_1588_DF_INGR_DF_CTRL_INGR_DF_DEPTH(x)  VTSS_ENCODE_BITFIELD(x,0,5)
#define  VTSS_M_PTP_3_INGR_IP_1588_DF_INGR_DF_CTRL_INGR_DF_DEPTH     VTSS_ENCODE_BITMASK(0,5)
#define  VTSS_X_PTP_3_INGR_IP_1588_DF_INGR_DF_CTRL_INGR_DF_DEPTH(x)  VTSS_EXTRACT_BITFIELD(x,0,5)

/**
 * Register Group: \a PTP_3:INGR_IP_1588_TSFIFO
 *
 * 1588 IP timestamp FIFO
 */


/** 
 * \brief Timestamp FIFO configuration and status
 *
 * \details
 * Configuration and status register for the timestamp FIFO
 *
 * Register: \a PTP_3:INGR_IP_1588_TSFIFO:INGR_TSFIFO_CSR
 */
#define VTSS_PTP_3_INGR_IP_1588_TSFIFO_INGR_TSFIFO_CSR  VTSS_IOREG(0x1e, 1, 0x923b)

/** 
 * \brief
 * Selects a smaller timestamp size to be stored in the timestamp FIFO (4
 * bytes vs. the default 10 bytes)
 *
 * \details 
 * 0: Full 10 byte timestamps are stored
 * 1: Only 4 bytes of each timestamp are stored
 *
 * Field: VTSS_PTP_3_INGR_IP_1588_TSFIFO_INGR_TSFIFO_CSR . INGR_TS_4BYTES
 */
#define  VTSS_F_PTP_3_INGR_IP_1588_TSFIFO_INGR_TSFIFO_CSR_INGR_TS_4BYTES  VTSS_BIT(17)

/** 
 * \brief
 * Forces the TS_FIFO into the reset state
 *
 * \details 
 * Field: VTSS_PTP_3_INGR_IP_1588_TSFIFO_INGR_TSFIFO_CSR . INGR_TS_FIFO_RESET
 */
#define  VTSS_F_PTP_3_INGR_IP_1588_TSFIFO_INGR_TSFIFO_CSR_INGR_TS_FIFO_RESET  VTSS_BIT(16)

/** 
 * \brief
 * The FIFO level associated with the last read of the TS_EMPTY status
 * field of the TSFIFO_0 register
 *
 * \details 
 * Binary number (0-8)
 *
 * Field: VTSS_PTP_3_INGR_IP_1588_TSFIFO_INGR_TSFIFO_CSR . INGR_TS_LEVEL
 */
#define  VTSS_F_PTP_3_INGR_IP_1588_TSFIFO_INGR_TSFIFO_CSR_INGR_TS_LEVEL(x)  VTSS_ENCODE_BITFIELD(x,12,4)
#define  VTSS_M_PTP_3_INGR_IP_1588_TSFIFO_INGR_TSFIFO_CSR_INGR_TS_LEVEL     VTSS_ENCODE_BITMASK(12,4)
#define  VTSS_X_PTP_3_INGR_IP_1588_TSFIFO_INGR_TSFIFO_CSR_INGR_TS_LEVEL(x)  VTSS_EXTRACT_BITFIELD(x,12,4)

/** 
 * \brief
 * The threshold at which the timestamp FIFO interrupt TS_LEVEL_STICKY will
 * be set. If the FIFO level reaches the threshold, the sticky bit
 * TS_LEVEL_STICKY will be set.
 *
 * \details 
 * Binary number (1-8)
 *
 * Field: VTSS_PTP_3_INGR_IP_1588_TSFIFO_INGR_TSFIFO_CSR . INGR_TS_THRESH
 */
#define  VTSS_F_PTP_3_INGR_IP_1588_TSFIFO_INGR_TSFIFO_CSR_INGR_TS_THRESH(x)  VTSS_ENCODE_BITFIELD(x,8,4)
#define  VTSS_M_PTP_3_INGR_IP_1588_TSFIFO_INGR_TSFIFO_CSR_INGR_TS_THRESH     VTSS_ENCODE_BITMASK(8,4)
#define  VTSS_X_PTP_3_INGR_IP_1588_TSFIFO_INGR_TSFIFO_CSR_INGR_TS_THRESH(x)  VTSS_EXTRACT_BITFIELD(x,8,4)

/** 
 * \brief
 * Indicates the number of signature bytes used for timestamps in the
 * timestamp FIFO (0-16).
 *
 * \details 
 * Field: VTSS_PTP_3_INGR_IP_1588_TSFIFO_INGR_TSFIFO_CSR . INGR_TS_SIGNAT_BYTES
 */
#define  VTSS_F_PTP_3_INGR_IP_1588_TSFIFO_INGR_TSFIFO_CSR_INGR_TS_SIGNAT_BYTES(x)  VTSS_ENCODE_BITFIELD(x,0,5)
#define  VTSS_M_PTP_3_INGR_IP_1588_TSFIFO_INGR_TSFIFO_CSR_INGR_TS_SIGNAT_BYTES     VTSS_ENCODE_BITMASK(0,5)
#define  VTSS_X_PTP_3_INGR_IP_1588_TSFIFO_INGR_TSFIFO_CSR_INGR_TS_SIGNAT_BYTES(x)  VTSS_EXTRACT_BITFIELD(x,0,5)


/** 
 * \brief Data value from the timestamp FIFO
 *
 * \details
 * Read the data from the timestamp FIFO along with the FIFO empty flag in
 * the MSB
 *
 * Register: \a PTP_3:INGR_IP_1588_TSFIFO:INGR_TSFIFO_0
 */
#define VTSS_PTP_3_INGR_IP_1588_TSFIFO_INGR_TSFIFO_0  VTSS_IOREG(0x1e, 1, 0x923c)

/** 
 * \brief
 * The FIFO empty flag from the Timestamp FIFO. If this bit is set, there
 * is no FIFO data to be read from the FIFO. The data in the TSFIFO_x
 * registers is not valid and should be discarded. When 0, the FIFO has
 * data and the TSFIFO_x has a valid set of data. This register can be
 * polled and when the bit is cleared, the other registers should be read
 * to get a full timestamp. When 1, the last data has already been read out
 * and the current read data should be discarded.
 * Timestamp/Frame signature bytes are packed such that the 10 or 4 byte
 * timestamp resides in the LEAST significant bytes while the frame
 * signature (0 to 16 bytes) resides in the MOST significant bytes. The
 * order of the bytes within each timestamp/frame signature field is also
 * most significant to least significant.
 * For example, 26 byte timestamp/frame signature pairs are packed with the
 * 10 byte timestamp field ([79:0]) corresponding to Bits 79:0 in the
 * following registers, and a 16 byte frame signature field ([127:0])
 * corresponding to Bits 207:80 in the following registers.
 *
 * \details 
 * 0: FIFO not empty, data valid
 * 1: FIFO empty, data invalid
 *
 * Field: VTSS_PTP_3_INGR_IP_1588_TSFIFO_INGR_TSFIFO_0 . INGR_TS_EMPTY
 */
#define  VTSS_F_PTP_3_INGR_IP_1588_TSFIFO_INGR_TSFIFO_0_INGR_TS_EMPTY  VTSS_BIT(31)

/** 
 * \brief
 * FIFO flags from the timestamp FIFO. These bits indicate how many
 * timestamps are valid in the current (not empty) 26 byte FIFO entry.
 *
 * \details 
 * 000: Only the end of a partial timestamp is valid in the current FIFO
 * entry (any remaining data is invalid)
 * 001: 1 valid timestamp begins in the current FIFO entry (any remaining
 * data is invalid)
 * 010: 2 valid timestamps begin in the current FIFO entry (any remaining
 * data is invalid)
 * 011: 3 valid timestamps begin in the current FIFO entry (any remaining
 * data is invalid)
 * 100: 4 valid timestamps begin in the current FIFO entry (any remaining
 * data is invalid)
 * 101: 5 valid timestamps begin in the current FIFO entry (any remaining
 * data is invalid)
 * 110: 6 valid timestamps begin in the current FIFO entry (any remaining
 * data is invalid)
 * 111: The current FIFO entry is fully packed with timestamps (all data is
 * valid)
 *
 * Field: VTSS_PTP_3_INGR_IP_1588_TSFIFO_INGR_TSFIFO_0 . INGR_TS_FLAGS
 */
#define  VTSS_F_PTP_3_INGR_IP_1588_TSFIFO_INGR_TSFIFO_0_INGR_TS_FLAGS(x)  VTSS_ENCODE_BITFIELD(x,28,3)
#define  VTSS_M_PTP_3_INGR_IP_1588_TSFIFO_INGR_TSFIFO_0_INGR_TS_FLAGS     VTSS_ENCODE_BITMASK(28,3)
#define  VTSS_X_PTP_3_INGR_IP_1588_TSFIFO_INGR_TSFIFO_0_INGR_TS_FLAGS(x)  VTSS_EXTRACT_BITFIELD(x,28,3)

/** 
 * \brief
 * 16 bits from the timestamp FIFO. Bits 15:0
 *
 * \details 
 * Field: VTSS_PTP_3_INGR_IP_1588_TSFIFO_INGR_TSFIFO_0 . INGR_TSFIFO_0
 */
#define  VTSS_F_PTP_3_INGR_IP_1588_TSFIFO_INGR_TSFIFO_0_INGR_TSFIFO_0(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_PTP_3_INGR_IP_1588_TSFIFO_INGR_TSFIFO_0_INGR_TSFIFO_0     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_PTP_3_INGR_IP_1588_TSFIFO_INGR_TSFIFO_0_INGR_TSFIFO_0(x)  VTSS_EXTRACT_BITFIELD(x,0,16)


/** 
 * \brief Data value from the timestamp FIFO
 *
 * \details
 * Read the data from the timestamp FIFO
 *
 * Register: \a PTP_3:INGR_IP_1588_TSFIFO:INGR_TSFIFO_1
 */
#define VTSS_PTP_3_INGR_IP_1588_TSFIFO_INGR_TSFIFO_1  VTSS_IOREG(0x1e, 1, 0x923d)


/** 
 * \brief Data value from the timestamp FIFO
 *
 * \details
 * Read the data from the timestamp FIFO
 *
 * Register: \a PTP_3:INGR_IP_1588_TSFIFO:INGR_TSFIFO_2
 */
#define VTSS_PTP_3_INGR_IP_1588_TSFIFO_INGR_TSFIFO_2  VTSS_IOREG(0x1e, 1, 0x923e)


/** 
 * \brief Data value from the timestamp FIFO
 *
 * \details
 * Read the data from the timestamp FIFO
 *
 * Register: \a PTP_3:INGR_IP_1588_TSFIFO:INGR_TSFIFO_3
 */
#define VTSS_PTP_3_INGR_IP_1588_TSFIFO_INGR_TSFIFO_3  VTSS_IOREG(0x1e, 1, 0x923f)


/** 
 * \brief Data value from the timestamp FIFO
 *
 * \details
 * Read the data from the timestamp FIFO
 *
 * Register: \a PTP_3:INGR_IP_1588_TSFIFO:INGR_TSFIFO_4
 */
#define VTSS_PTP_3_INGR_IP_1588_TSFIFO_INGR_TSFIFO_4  VTSS_IOREG(0x1e, 1, 0x9240)


/** 
 * \brief Data value from the timestamp FIFO
 *
 * \details
 * Read the data from the timestamp FIFO
 *
 * Register: \a PTP_3:INGR_IP_1588_TSFIFO:INGR_TSFIFO_5
 */
#define VTSS_PTP_3_INGR_IP_1588_TSFIFO_INGR_TSFIFO_5  VTSS_IOREG(0x1e, 1, 0x9241)


/** 
 * \brief Data value from the timestamp FIFO
 *
 * \details
 * Read the data from the timestamp FIFO
 *
 * Register: \a PTP_3:INGR_IP_1588_TSFIFO:INGR_TSFIFO_6
 */
#define VTSS_PTP_3_INGR_IP_1588_TSFIFO_INGR_TSFIFO_6  VTSS_IOREG(0x1e, 1, 0x9242)


/** 
 * \brief Count of dropped timestamps
 *
 * \details
 * Count of dropped timestamps not enqueued to the TS FIFO
 *
 * Register: \a PTP_3:INGR_IP_1588_TSFIFO:INGR_TSFIFO_DROP_CNT
 */
#define VTSS_PTP_3_INGR_IP_1588_TSFIFO_INGR_TSFIFO_DROP_CNT  VTSS_IOREG(0x1e, 1, 0x9243)

/**
 * Register Group: \a PTP_3:INGR_IP_1588_RW
 *
 * 1588 IP rewriter
 */


/** 
 * \brief Rewriter configuration and control
 *
 * \details
 * Configuration for the rewriter
 *
 * Register: \a PTP_3:INGR_IP_1588_RW:INGR_RW_CTRL
 */
#define VTSS_PTP_3_INGR_IP_1588_RW_INGR_RW_CTRL  VTSS_IOREG(0x1e, 1, 0x9244)

/** 
 * \brief
 * When set, the 1588 IP will reduce the preamble of ALL incoming frames by
 * 4 bytes to allow a timestamp to be appended to the ingress data frames.
 * This bit must be set along with the proper configuration of the Analyzer
 * to ensure proper operation. Note: Valid in ingress direction only
 *
 * \details 
 * 0: No preamble modification
 * 1: Reduce preamble by 4 bytes
 *
 * Field: VTSS_PTP_3_INGR_IP_1588_RW_INGR_RW_CTRL . INGR_RW_REDUCE_PREAMBLE
 */
#define  VTSS_F_PTP_3_INGR_IP_1588_RW_INGR_RW_CTRL_INGR_RW_REDUCE_PREAMBLE  VTSS_BIT(4)

/** 
 * \brief
 * Value to write to the flag bit when it is overwritten
 *
 * \details 
 * 0: 0 will be written to the flag bit
 * 1: 1 will be written to the flag bit
 *
 * Field: VTSS_PTP_3_INGR_IP_1588_RW_INGR_RW_CTRL . INGR_RW_FLAG_VAL
 */
#define  VTSS_F_PTP_3_INGR_IP_1588_RW_INGR_RW_CTRL_INGR_RW_FLAG_VAL  VTSS_BIT(3)

/** 
 * \brief
 * Bit offset within a byte of the flag bit that indicates if the frame has
 * been modified
 *
 * \details 
 * Binary number
 *
 * Field: VTSS_PTP_3_INGR_IP_1588_RW_INGR_RW_CTRL . INGR_RW_FLAG_BIT
 */
#define  VTSS_F_PTP_3_INGR_IP_1588_RW_INGR_RW_CTRL_INGR_RW_FLAG_BIT(x)  VTSS_ENCODE_BITFIELD(x,0,3)
#define  VTSS_M_PTP_3_INGR_IP_1588_RW_INGR_RW_CTRL_INGR_RW_FLAG_BIT     VTSS_ENCODE_BITMASK(0,3)
#define  VTSS_X_PTP_3_INGR_IP_1588_RW_INGR_RW_CTRL_INGR_RW_FLAG_BIT(x)  VTSS_EXTRACT_BITFIELD(x,0,3)


/** 
 * \brief Count of modified frames
 *
 * \details
 * Register: \a PTP_3:INGR_IP_1588_RW:INGR_RW_MODFRM_CNT
 */
#define VTSS_PTP_3_INGR_IP_1588_RW_INGR_RW_MODFRM_CNT  VTSS_IOREG(0x1e, 1, 0x9245)


/** 
 * \brief Count of FCS errors
 *
 * \details
 * Register: \a PTP_3:INGR_IP_1588_RW:INGR_RW_FCS_ERR_CNT
 */
#define VTSS_PTP_3_INGR_IP_1588_RW_INGR_RW_FCS_ERR_CNT  VTSS_IOREG(0x1e, 1, 0x9246)


/** 
 * \brief Count of the number of preamble errors
 *
 * \details
 * Register: \a PTP_3:INGR_IP_1588_RW:INGR_RW_PREAMBLE_ERR_CNT
 */
#define VTSS_PTP_3_INGR_IP_1588_RW_INGR_RW_PREAMBLE_ERR_CNT  VTSS_IOREG(0x1e, 1, 0x9247)

/**
 * Register Group: \a PTP_3:INGR_IP_ANALYZER_STAT
 *
 * Analyzer Counters and Errors
 */


/** 
 * \brief Analyzer Modified Frames Counter
 *
 * \details
 * Register: \a PTP_3:INGR_IP_ANALYZER_STAT:INGR_ANALYZER_MOD_FRAMES_CNT
 */
#define VTSS_PTP_3_INGR_IP_ANALYZER_STAT_INGR_ANALYZER_MOD_FRAMES_CNT  VTSS_IOREG(0x1e, 1, 0x924c)

/**
 * Register Group: \a PTP_3:EGR_IP_1588_CFG_STAT
 *
 * 1588 IP control and status registers
 */


/** 
 * \brief 1588 IP interrupt status register
 *
 * \details
 * Status sticky conditions for the 1588 IP
 *
 * Register: \a PTP_3:EGR_IP_1588_CFG_STAT:EGR_INT_STATUS
 */
#define VTSS_PTP_3_EGR_IP_1588_CFG_STAT_EGR_INT_STATUS  VTSS_IOREG(0x1e, 1, 0x924d)

/** 
 * \brief
 * When set, indicates an underflow in the bypass logic. The sticky bit
 * should be reset by writing it to 1.
 *
 * \details 
 * Field: VTSS_PTP_3_EGR_IP_1588_CFG_STAT_EGR_INT_STATUS . EGR_BYPASS_UNDERFLOW_STICKY
 */
#define  VTSS_F_PTP_3_EGR_IP_1588_CFG_STAT_EGR_INT_STATUS_EGR_BYPASS_UNDERFLOW_STICKY  VTSS_BIT(15)

/** 
 * \brief
 * When set, indicates an overflow in the bypass logic. The sticky bit
 * should be reset by writing it to 1.
 *
 * \details 
 * Field: VTSS_PTP_3_EGR_IP_1588_CFG_STAT_EGR_INT_STATUS . EGR_BYPASS_OVERFLOW_STICKY
 */
#define  VTSS_F_PTP_3_EGR_IP_1588_CFG_STAT_EGR_INT_STATUS_EGR_BYPASS_OVERFLOW_STICKY  VTSS_BIT(14)

/** 
 * \brief
 * When set, indicates an underflow in the analyzer command processor. The
 * sticky bit should be reset by writing it to 1.
 *
 * \details 
 * Field: VTSS_PTP_3_EGR_IP_1588_CFG_STAT_EGR_INT_STATUS . EGR_ANA_CMD_UNDERFLOW_STICKY
 */
#define  VTSS_F_PTP_3_EGR_IP_1588_CFG_STAT_EGR_INT_STATUS_EGR_ANA_CMD_UNDERFLOW_STICKY  VTSS_BIT(13)

/** 
 * \brief
 * When set, indicates an overflow in the analyzer command processor. The
 * sticky bit should be reset by writing it to 1.
 *
 * \details 
 * Field: VTSS_PTP_3_EGR_IP_1588_CFG_STAT_EGR_INT_STATUS . EGR_ANA_CMD_OVERFLOW_STICKY
 */
#define  VTSS_F_PTP_3_EGR_IP_1588_CFG_STAT_EGR_INT_STATUS_EGR_ANA_CMD_OVERFLOW_STICKY  VTSS_BIT(12)

/** 
 * \brief
 * When set, indicates an a frame was processed by rewriter command before
 * SOF arrives. Increase the fifo depth. The sticky bit should be reset by
 * writing it to 1.
 *
 * \details 
 * Field: VTSS_PTP_3_EGR_IP_1588_CFG_STAT_EGR_INT_STATUS . EGR_TSP_CMD_UNDERFLOW_STICKY
 */
#define  VTSS_F_PTP_3_EGR_IP_1588_CFG_STAT_EGR_INT_STATUS_EGR_TSP_CMD_UNDERFLOW_STICKY  VTSS_BIT(11)

/** 
 * \brief
 * When set, indicates an overflow in the Timestamp command processor. The
 * sticky bit should be reset by writing it to 1.
 *
 * \details 
 * Field: VTSS_PTP_3_EGR_IP_1588_CFG_STAT_EGR_INT_STATUS . EGR_TSP_CMD_OVERFLOW_STICKY
 */
#define  VTSS_F_PTP_3_EGR_IP_1588_CFG_STAT_EGR_INT_STATUS_EGR_TSP_CMD_OVERFLOW_STICKY  VTSS_BIT(10)

/** 
 * \brief
 * When set, indicates an a frame was processed by analyzer command before
 * a timestamp was taken. Underflow in the Timestamp. The sticky bit should
 * be reset by writing it to 1.
 *
 * \details 
 * Field: VTSS_PTP_3_EGR_IP_1588_CFG_STAT_EGR_INT_STATUS . EGR_TSP_FIFO_UNDERFLOW_STICKY
 */
#define  VTSS_F_PTP_3_EGR_IP_1588_CFG_STAT_EGR_INT_STATUS_EGR_TSP_FIFO_UNDERFLOW_STICKY  VTSS_BIT(9)

/** 
 * \brief
 * When set, indicates an overflow in the Timestamp processor. More than
 * four frames within 1588. The sticky bit should be reset by writing it to
 * 1.
 *
 * \details 
 * Field: VTSS_PTP_3_EGR_IP_1588_CFG_STAT_EGR_INT_STATUS . EGR_TSP_FIFO_OVERFLOW_STICKY
 */
#define  VTSS_F_PTP_3_EGR_IP_1588_CFG_STAT_EGR_INT_STATUS_EGR_TSP_FIFO_OVERFLOW_STICKY  VTSS_BIT(8)

/** 
 * \brief
 * When set indicates that a frame was received with non-zero data in the
 * reserved field. The sticky bit should be reset by writing it to 1.
 *
 * \details 
 * 0 = No data in reserved field
 * 1 = Data in reserved field
 *
 * Field: VTSS_PTP_3_EGR_IP_1588_CFG_STAT_EGR_INT_STATUS . EGR_RSVD_NONZERO_STICKY
 */
#define  VTSS_F_PTP_3_EGR_IP_1588_CFG_STAT_EGR_INT_STATUS_EGR_RSVD_NONZERO_STICKY  VTSS_BIT(7)

/** 
 * \brief
 * Indicates that more than one engine has produced a match. The sticky bit
 * should be reset by writing it to 1.
 *
 * \details 
 * 0: No error found
 * 1: Duplicate match found
 *
 * Field: VTSS_PTP_3_EGR_IP_1588_CFG_STAT_EGR_INT_STATUS . EGR_ANALYZER_ERROR_STICKY
 */
#define  VTSS_F_PTP_3_EGR_IP_1588_CFG_STAT_EGR_INT_STATUS_EGR_ANALYZER_ERROR_STICKY  VTSS_BIT(6)

/** 
 * \brief
 * When set, indicates that a preamble that was too short to modify was
 * detected in a PTP frame. Write to 1 to clear. This occurs when the
 * rewriter needs to shrink the preamble to append a timestamp but cannot
 * because the preamble is too short. A short preamble is any preamble that
 * is less than eight characters long including the XGMII /S/ character and
 * the ending SFD of 0xD5. Other preamble values are not checked, only the
 * GMII length.
 *
 * \details 
 * 0: No error
 * 1: Preamble too short error
 *
 * Field: VTSS_PTP_3_EGR_IP_1588_CFG_STAT_EGR_INT_STATUS . EGR_RW_PREAMBLE_ERR_STICKY
 */
#define  VTSS_F_PTP_3_EGR_IP_1588_CFG_STAT_EGR_INT_STATUS_EGR_RW_PREAMBLE_ERR_STICKY  VTSS_BIT(5)

/** 
 * \brief
 * When set, indicates that an FCS error was detected in a PTP/OAM frame.
 * Write to 1 to clear.
 *
 * \details 
 * 0: No error
 * 1: FCS error
 *
 * Field: VTSS_PTP_3_EGR_IP_1588_CFG_STAT_EGR_INT_STATUS . EGR_RW_FCS_ERR_STICKY
 */
#define  VTSS_F_PTP_3_EGR_IP_1588_CFG_STAT_EGR_INT_STATUS_EGR_RW_FCS_ERR_STICKY  VTSS_BIT(4)

/** 
 * \brief
 * When set, indicates that the level in the Timestamp FIFO has reached the
 * threshold TS_THRESH. The sticky bit should be reset by writing it to 1.
 *
 * \details 
 * 0: No overflow
 * 1: Overflow
 *
 * Field: VTSS_PTP_3_EGR_IP_1588_CFG_STAT_EGR_INT_STATUS . EGR_TS_LEVEL_STICKY
 */
#define  VTSS_F_PTP_3_EGR_IP_1588_CFG_STAT_EGR_INT_STATUS_EGR_TS_LEVEL_STICKY  VTSS_BIT(3)

/** 
 * \brief
 * When set, indicates a timestamp was captured in the Timestamp FIFO. The
 * sticky bit should be reset by writing it to 1.
 *
 * \details 
 * 0: No overflow
 * 1: Overflow
 *
 * Field: VTSS_PTP_3_EGR_IP_1588_CFG_STAT_EGR_INT_STATUS . EGR_TS_LOADED_STICKY
 */
#define  VTSS_F_PTP_3_EGR_IP_1588_CFG_STAT_EGR_INT_STATUS_EGR_TS_LOADED_STICKY  VTSS_BIT(2)

/** 
 * \brief
 * When set, indicates an underflow in the Timestamp FIFO. The sticky bit
 * should be reset by writing it to 1.
 *
 * \details 
 * 0: No overflow
 * 1: Overflow
 *
 * Field: VTSS_PTP_3_EGR_IP_1588_CFG_STAT_EGR_INT_STATUS . EGR_TS_UNDERFLOW_STICKY
 */
#define  VTSS_F_PTP_3_EGR_IP_1588_CFG_STAT_EGR_INT_STATUS_EGR_TS_UNDERFLOW_STICKY  VTSS_BIT(1)

/** 
 * \brief
 * When set, indicates an overflow in the Timestamp FIFO. The sticky bit
 * should be reset by writing it to 1.
 *
 * \details 
 * 0: No overflow
 * 1: Overflow
 *
 * Field: VTSS_PTP_3_EGR_IP_1588_CFG_STAT_EGR_INT_STATUS . EGR_TS_OVERFLOW_STICKY
 */
#define  VTSS_F_PTP_3_EGR_IP_1588_CFG_STAT_EGR_INT_STATUS_EGR_TS_OVERFLOW_STICKY  VTSS_BIT(0)


/** 
 * \brief 1588 IP interrupt mask register
 *
 * \details
 * Masks that enable and disable the interrupts
 *
 * Register: \a PTP_3:EGR_IP_1588_CFG_STAT:EGR_INT_MASK
 */
#define VTSS_PTP_3_EGR_IP_1588_CFG_STAT_EGR_INT_MASK  VTSS_IOREG(0x1e, 1, 0x924e)

/** 
 * \brief
 * Mask for BYPASS_UNDERFLOW_STICKY bit
 *
 * \details 
 * 0: Interrupt disabled
 * 1: Interrupt enabled
 *
 * Field: VTSS_PTP_3_EGR_IP_1588_CFG_STAT_EGR_INT_MASK . EGR_BYPASS_UNDERFLOW_MASK
 */
#define  VTSS_F_PTP_3_EGR_IP_1588_CFG_STAT_EGR_INT_MASK_EGR_BYPASS_UNDERFLOW_MASK  VTSS_BIT(15)

/** 
 * \brief
 * Mask for BYPASS_OVERFLOW_STICKY bit. This mask is also shared for
 * TSP_PUSH_FLAG interrupt.
 *
 * \details 
 * 0: Interrupt disabled
 * 1: Interrupt enabled
 *
 * Field: VTSS_PTP_3_EGR_IP_1588_CFG_STAT_EGR_INT_MASK . EGR_BYPASS_OVERFLOW_MASK
 */
#define  VTSS_F_PTP_3_EGR_IP_1588_CFG_STAT_EGR_INT_MASK_EGR_BYPASS_OVERFLOW_MASK  VTSS_BIT(14)

/** 
 * \brief
 * Mask for ANA_CMD_UNDERFLOW_STICKY bit
 *
 * \details 
 * 0: Interrupt disabled
 * 1: Interrupt enabled
 *
 * Field: VTSS_PTP_3_EGR_IP_1588_CFG_STAT_EGR_INT_MASK . EGR_ANA_CMD_UNDERFLOW_MASK
 */
#define  VTSS_F_PTP_3_EGR_IP_1588_CFG_STAT_EGR_INT_MASK_EGR_ANA_CMD_UNDERFLOW_MASK  VTSS_BIT(13)

/** 
 * \brief
 * Mask for ANA_CMD_OVERFLOW_STICKY bit
 *
 * \details 
 * 0: Interrupt disabled
 * 1: Interrupt enabled
 *
 * Field: VTSS_PTP_3_EGR_IP_1588_CFG_STAT_EGR_INT_MASK . EGR_ANA_CMD_OVERFLOW_MASK
 */
#define  VTSS_F_PTP_3_EGR_IP_1588_CFG_STAT_EGR_INT_MASK_EGR_ANA_CMD_OVERFLOW_MASK  VTSS_BIT(12)

/** 
 * \brief
 * Mask bit for TSP_CMD_UNDERFLOW_STICKY bit
 *
 * \details 
 * 0: Interrupt disabled
 * 1: Interrupt enabled
 *
 * Field: VTSS_PTP_3_EGR_IP_1588_CFG_STAT_EGR_INT_MASK . EGR_TSP_CMD_UNDERFLOW_MASK
 */
#define  VTSS_F_PTP_3_EGR_IP_1588_CFG_STAT_EGR_INT_MASK_EGR_TSP_CMD_UNDERFLOW_MASK  VTSS_BIT(11)

/** 
 * \brief
 * Mask bit for TSP_CMD_OVERFLOW_STICKY bit
 *
 * \details 
 * 0: Interrupt disabled
 * 1: Interrupt enabled
 *
 * Field: VTSS_PTP_3_EGR_IP_1588_CFG_STAT_EGR_INT_MASK . EGR_TSP_CMD_OVERFLOW_MASK
 */
#define  VTSS_F_PTP_3_EGR_IP_1588_CFG_STAT_EGR_INT_MASK_EGR_TSP_CMD_OVERFLOW_MASK  VTSS_BIT(10)

/** 
 * \brief
 * Mask for TSP_FIFO_UNDERFLOW_STICKY bit
 *
 * \details 
 * 0: Interrupt disabled
 * 1: Interrupt enabled
 *
 * Field: VTSS_PTP_3_EGR_IP_1588_CFG_STAT_EGR_INT_MASK . EGR_TSP_FIFO_UNDERFLOW_MASK
 */
#define  VTSS_F_PTP_3_EGR_IP_1588_CFG_STAT_EGR_INT_MASK_EGR_TSP_FIFO_UNDERFLOW_MASK  VTSS_BIT(9)

/** 
 * \brief
 * Mask bit for TSP_FIFO_OVERFLOW_STICKY bit
 *
 * \details 
 * 0: Interrupt disabled
 * 1: Interrupt enabled
 *
 * Field: VTSS_PTP_3_EGR_IP_1588_CFG_STAT_EGR_INT_MASK . EGR_TSP_FIFO_OVERFLOW_MASK
 */
#define  VTSS_F_PTP_3_EGR_IP_1588_CFG_STAT_EGR_INT_MASK_EGR_TSP_FIFO_OVERFLOW_MASK  VTSS_BIT(8)

/** 
 * \brief
 * Mask bit for RSVD_NONZERO_STICKY bit
 *
 * \details 
 * 0: Interrupt disabled
 * 1: Interrupt enabled
 *
 * Field: VTSS_PTP_3_EGR_IP_1588_CFG_STAT_EGR_INT_MASK . EGR_RSVD_NONZERO_MASK
 */
#define  VTSS_F_PTP_3_EGR_IP_1588_CFG_STAT_EGR_INT_MASK_EGR_RSVD_NONZERO_MASK  VTSS_BIT(7)

/** 
 * \brief
 * Mask bit for ANALYZER_ERROR_STICKY bit
 *
 * \details 
 * 0: Interrupt disabled
 * 1: Interrupt enabled
 *
 * Field: VTSS_PTP_3_EGR_IP_1588_CFG_STAT_EGR_INT_MASK . EGR_ANALYZER_ERROR_MASK
 */
#define  VTSS_F_PTP_3_EGR_IP_1588_CFG_STAT_EGR_INT_MASK_EGR_ANALYZER_ERROR_MASK  VTSS_BIT(6)

/** 
 * \brief
 * Mask for the RW_PREAMBLE_ERR_STICKY bit
 *
 * \details 
 * 0: Interrupt disabled
 * 1: Interrupt enabled
 *
 * Field: VTSS_PTP_3_EGR_IP_1588_CFG_STAT_EGR_INT_MASK . EGR_RW_PREAMBLE_ERR_MASK
 */
#define  VTSS_F_PTP_3_EGR_IP_1588_CFG_STAT_EGR_INT_MASK_EGR_RW_PREAMBLE_ERR_MASK  VTSS_BIT(5)

/** 
 * \brief
 * Mask for the RW_FCS_ERR_STICKY bit
 *
 * \details 
 * 0: Interrupt disabled
 * 1: Interrupt enabled
 *
 * Field: VTSS_PTP_3_EGR_IP_1588_CFG_STAT_EGR_INT_MASK . EGR_RW_FCS_ERR_MASK
 */
#define  VTSS_F_PTP_3_EGR_IP_1588_CFG_STAT_EGR_INT_MASK_EGR_RW_FCS_ERR_MASK  VTSS_BIT(4)

/** 
 * \brief
 * Mask bit for TS_LEVEL_STICKY. When 1, the interrupt is enabled.
 *
 * \details 
 * 0: Interrupt disabled
 * 1: Interrupt enabled
 *
 * Field: VTSS_PTP_3_EGR_IP_1588_CFG_STAT_EGR_INT_MASK . EGR_TS_LEVEL_MASK
 */
#define  VTSS_F_PTP_3_EGR_IP_1588_CFG_STAT_EGR_INT_MASK_EGR_TS_LEVEL_MASK  VTSS_BIT(3)

/** 
 * \brief
 * Mask bit for TS_LOADED_STICKY. When 1, the interrupt is enabled.
 *
 * \details 
 * 0: Interrupt disabled
 * 1: Interrupt enabled
 *
 * Field: VTSS_PTP_3_EGR_IP_1588_CFG_STAT_EGR_INT_MASK . EGR_TS_LOADED_MASK
 */
#define  VTSS_F_PTP_3_EGR_IP_1588_CFG_STAT_EGR_INT_MASK_EGR_TS_LOADED_MASK  VTSS_BIT(2)

/** 
 * \brief
 * Mask bit for TS_UNDERFLOW_STICKY. When 1, the interrupt is enabled.
 *
 * \details 
 * 0: Interrupt disabled
 * 1: Interrupt enabled
 *
 * Field: VTSS_PTP_3_EGR_IP_1588_CFG_STAT_EGR_INT_MASK . EGR_TS_UNDERFLOW_MASK
 */
#define  VTSS_F_PTP_3_EGR_IP_1588_CFG_STAT_EGR_INT_MASK_EGR_TS_UNDERFLOW_MASK  VTSS_BIT(1)

/** 
 * \brief
 * Mask bit for TS_OVERFLOW_STICKY. When 1, the interrupt is enabled.
 *
 * \details 
 * 0: Interrupt disabled
 * 1: Interrupt enabled
 *
 * Field: VTSS_PTP_3_EGR_IP_1588_CFG_STAT_EGR_INT_MASK . EGR_TS_OVERFLOW_MASK
 */
#define  VTSS_F_PTP_3_EGR_IP_1588_CFG_STAT_EGR_INT_MASK_EGR_TS_OVERFLOW_MASK  VTSS_BIT(0)


/** 
 * \brief Egress side stall latency
 *
 * \details
 * Register: \a PTP_3:EGR_IP_1588_CFG_STAT:EGR_CFG_STALL_LATENCY
 */
#define VTSS_PTP_3_EGR_IP_1588_CFG_STAT_EGR_CFG_STALL_LATENCY  VTSS_IOREG(0x1e, 1, 0x924f)

/**
 * Register Group: \a PTP_3:EGR_IP_1588_TSP
 *
 * 1588 IP timestamp processor
 */


/** 
 * \brief TSP control
 *
 * \details
 * Register: \a PTP_3:EGR_IP_1588_TSP:EGR_TSP_CTRL
 */
#define VTSS_PTP_3_EGR_IP_1588_TSP_EGR_TSP_CTRL  VTSS_IOREG(0x1e, 1, 0x9255)

/** 
 * \brief
 * Selects a mode in which the fractional portion of a second (in units of
 * nanoseconds) is used for timestamping. Only the operation of the
 * WRITE_NS, WRITE_NS_P2P, and SUB_ADD PTP commands are affected by the
 * setting of this mode bit.
 *
 * \details 
 * 0: Select the total (summed) nanoseconds for timestamping
 * 1: Select the fractional portion in nanoseconds for timestamping
 *
 * Field: VTSS_PTP_3_EGR_IP_1588_TSP_EGR_TSP_CTRL . EGR_FRACT_NS_MODE
 */
#define  VTSS_F_PTP_3_EGR_IP_1588_TSP_EGR_TSP_CTRL_EGR_FRACT_NS_MODE  VTSS_BIT(2)

/** 
 * \brief
 * Select external pin start of frame indicator
 *
 * \details 
 * 0: Select internal PCS as the source of SOF
 * 1: Select external pin as the source of SOF
 *
 * Field: VTSS_PTP_3_EGR_IP_1588_TSP_EGR_TSP_CTRL . EGR_SEL_EXT_SOF_IND
 */
#define  VTSS_F_PTP_3_EGR_IP_1588_TSP_EGR_TSP_CTRL_EGR_SEL_EXT_SOF_IND  VTSS_BIT(1)

/** 
 * \brief
 * One-shot loads local latency, path delay, and DelayAsymmetry values into
 * the timestamp processor
 *
 * \details 
 * Field: VTSS_PTP_3_EGR_IP_1588_TSP_EGR_TSP_CTRL . EGR_LOAD_DELAYS
 */
#define  VTSS_F_PTP_3_EGR_IP_1588_TSP_EGR_TSP_CTRL_EGR_LOAD_DELAYS  VTSS_BIT(0)


/** 
 * \brief TSP status
 *
 * \details
 * Register: \a PTP_3:EGR_IP_1588_TSP:EGR_TSP_STAT
 */
#define VTSS_PTP_3_EGR_IP_1588_TSP_EGR_TSP_STAT  VTSS_IOREG(0x1e, 1, 0x9256)

/** 
 * \brief
 * Timestamp processor marked a calculated correction field as too big
 *
 * \details 
 * 0: A calculated correction field that was too big did occur
 * 1: A calculated correction field that was too big did not occur
 *
 * Field: VTSS_PTP_3_EGR_IP_1588_TSP_EGR_TSP_STAT . EGR_CF_TOO_BIG_STICKY
 */
#define  VTSS_F_PTP_3_EGR_IP_1588_TSP_EGR_TSP_STAT_EGR_CF_TOO_BIG_STICKY  VTSS_BIT(0)


/** 
 * \brief Local latency
 *
 * \details
 * Register: \a PTP_3:EGR_IP_1588_TSP:EGR_LOCAL_LATENCY
 */
#define VTSS_PTP_3_EGR_IP_1588_TSP_EGR_LOCAL_LATENCY  VTSS_IOREG(0x1e, 1, 0x9257)

/** 
 * \brief
 * Local latency (nanoseconds)
 *
 * \details 
 * Field: VTSS_PTP_3_EGR_IP_1588_TSP_EGR_LOCAL_LATENCY . EGR_LOCAL_LATENCY
 */
#define  VTSS_F_PTP_3_EGR_IP_1588_TSP_EGR_LOCAL_LATENCY_EGR_LOCAL_LATENCY(x)  VTSS_ENCODE_BITFIELD(x,0,23)
#define  VTSS_M_PTP_3_EGR_IP_1588_TSP_EGR_LOCAL_LATENCY_EGR_LOCAL_LATENCY     VTSS_ENCODE_BITMASK(0,23)
#define  VTSS_X_PTP_3_EGR_IP_1588_TSP_EGR_LOCAL_LATENCY_EGR_LOCAL_LATENCY(x)  VTSS_EXTRACT_BITFIELD(x,0,23)


/** 
 * \brief Path delay
 *
 * \details
 * Register: \a PTP_3:EGR_IP_1588_TSP:EGR_PATH_DELAY
 */
#define VTSS_PTP_3_EGR_IP_1588_TSP_EGR_PATH_DELAY  VTSS_IOREG(0x1e, 1, 0x9258)


/** 
 * \brief Delay asymmetry
 *
 * \details
 * Register: \a PTP_3:EGR_IP_1588_TSP:EGR_DELAY_ASYMMETRY
 */
#define VTSS_PTP_3_EGR_IP_1588_TSP_EGR_DELAY_ASYMMETRY  VTSS_IOREG(0x1e, 1, 0x9259)

/**
 * Register Group: \a PTP_3:EGR_IP_1588_DF
 *
 * 1588 IP delay FIFO
 */


/** 
 * \brief Configuration and control register for the delay FIFO
 *
 * \details
 * Register: \a PTP_3:EGR_IP_1588_DF:EGR_DF_CTRL
 */
#define VTSS_PTP_3_EGR_IP_1588_DF_EGR_DF_CTRL  VTSS_IOREG(0x1e, 1, 0x925a)

/** 
 * \brief
 * The index of the register stage in the delay FIFO that is used for
 * output. The actual delay through the block is one more than the depth.
 * If depth is set to 2, then the delay is 3 clocks as data is taken from
 * stage 2. The depth MUST be greater than 0 (depth of 0 is not allowed)
 * and MUST be less than 20 (depth of 20 not allowed).
 *
 * \details 
 * Binary number >= 1
 *
 * Field: VTSS_PTP_3_EGR_IP_1588_DF_EGR_DF_CTRL . EGR_DF_DEPTH
 */
#define  VTSS_F_PTP_3_EGR_IP_1588_DF_EGR_DF_CTRL_EGR_DF_DEPTH(x)  VTSS_ENCODE_BITFIELD(x,0,5)
#define  VTSS_M_PTP_3_EGR_IP_1588_DF_EGR_DF_CTRL_EGR_DF_DEPTH     VTSS_ENCODE_BITMASK(0,5)
#define  VTSS_X_PTP_3_EGR_IP_1588_DF_EGR_DF_CTRL_EGR_DF_DEPTH(x)  VTSS_EXTRACT_BITFIELD(x,0,5)

/**
 * Register Group: \a PTP_3:EGR_IP_1588_TSFIFO
 *
 * 1588 IP timestamp FIFO
 */


/** 
 * \brief Timestamp FIFO configuration and status
 *
 * \details
 * Configuration and status register for the timestamp FIFO
 *
 * Register: \a PTP_3:EGR_IP_1588_TSFIFO:EGR_TSFIFO_CSR
 */
#define VTSS_PTP_3_EGR_IP_1588_TSFIFO_EGR_TSFIFO_CSR  VTSS_IOREG(0x1e, 1, 0x925b)

/** 
 * \brief
 * Selects a smaller timestamp size to be stored in the timestamp FIFO (4
 * bytes vs. the default 10 bytes)
 *
 * \details 
 * 0: Full 10 byte timestamps are stored
 * 1: Only 4 bytes of each timestamp are stored
 *
 * Field: VTSS_PTP_3_EGR_IP_1588_TSFIFO_EGR_TSFIFO_CSR . EGR_TS_4BYTES
 */
#define  VTSS_F_PTP_3_EGR_IP_1588_TSFIFO_EGR_TSFIFO_CSR_EGR_TS_4BYTES  VTSS_BIT(17)

/** 
 * \brief
 * Forces the TS_FIFO into the reset state
 *
 * \details 
 * Field: VTSS_PTP_3_EGR_IP_1588_TSFIFO_EGR_TSFIFO_CSR . EGR_TS_FIFO_RESET
 */
#define  VTSS_F_PTP_3_EGR_IP_1588_TSFIFO_EGR_TSFIFO_CSR_EGR_TS_FIFO_RESET  VTSS_BIT(16)

/** 
 * \brief
 * The FIFO level associated with the last read of the TS_EMPTY status
 * field of the TSFIFO_0 register
 *
 * \details 
 * Binary number (0-8)
 *
 * Field: VTSS_PTP_3_EGR_IP_1588_TSFIFO_EGR_TSFIFO_CSR . EGR_TS_LEVEL
 */
#define  VTSS_F_PTP_3_EGR_IP_1588_TSFIFO_EGR_TSFIFO_CSR_EGR_TS_LEVEL(x)  VTSS_ENCODE_BITFIELD(x,12,4)
#define  VTSS_M_PTP_3_EGR_IP_1588_TSFIFO_EGR_TSFIFO_CSR_EGR_TS_LEVEL     VTSS_ENCODE_BITMASK(12,4)
#define  VTSS_X_PTP_3_EGR_IP_1588_TSFIFO_EGR_TSFIFO_CSR_EGR_TS_LEVEL(x)  VTSS_EXTRACT_BITFIELD(x,12,4)

/** 
 * \brief
 * The threshold at which the timestamp FIFO interrupt TS_LEVEL_STICKY will
 * be set. If the FIFO level reaches the threshold, the sticky bit
 * TS_LEVEL_STICKY will be set.
 *
 * \details 
 * Binary number (1-8)
 *
 * Field: VTSS_PTP_3_EGR_IP_1588_TSFIFO_EGR_TSFIFO_CSR . EGR_TS_THRESH
 */
#define  VTSS_F_PTP_3_EGR_IP_1588_TSFIFO_EGR_TSFIFO_CSR_EGR_TS_THRESH(x)  VTSS_ENCODE_BITFIELD(x,8,4)
#define  VTSS_M_PTP_3_EGR_IP_1588_TSFIFO_EGR_TSFIFO_CSR_EGR_TS_THRESH     VTSS_ENCODE_BITMASK(8,4)
#define  VTSS_X_PTP_3_EGR_IP_1588_TSFIFO_EGR_TSFIFO_CSR_EGR_TS_THRESH(x)  VTSS_EXTRACT_BITFIELD(x,8,4)

/** 
 * \brief
 * Indicates the number of signature bytes used for timestamps in the
 * timestamp FIFO (0-16).
 *
 * \details 
 * Field: VTSS_PTP_3_EGR_IP_1588_TSFIFO_EGR_TSFIFO_CSR . EGR_TS_SIGNAT_BYTES
 */
#define  VTSS_F_PTP_3_EGR_IP_1588_TSFIFO_EGR_TSFIFO_CSR_EGR_TS_SIGNAT_BYTES(x)  VTSS_ENCODE_BITFIELD(x,0,5)
#define  VTSS_M_PTP_3_EGR_IP_1588_TSFIFO_EGR_TSFIFO_CSR_EGR_TS_SIGNAT_BYTES     VTSS_ENCODE_BITMASK(0,5)
#define  VTSS_X_PTP_3_EGR_IP_1588_TSFIFO_EGR_TSFIFO_CSR_EGR_TS_SIGNAT_BYTES(x)  VTSS_EXTRACT_BITFIELD(x,0,5)


/** 
 * \brief Data value from the timestamp FIFO
 *
 * \details
 * Read the data from the timestamp FIFO along with the FIFO empty flag in
 * the MSB
 *
 * Register: \a PTP_3:EGR_IP_1588_TSFIFO:EGR_TSFIFO_0
 */
#define VTSS_PTP_3_EGR_IP_1588_TSFIFO_EGR_TSFIFO_0  VTSS_IOREG(0x1e, 1, 0x925c)

/** 
 * \brief
 * The FIFO empty flag from the Timestamp FIFO. If this bit is set, there
 * is no FIFO data to be read from the FIFO. The data in the TSFIFO_x
 * registers is not valid and should be discarded. When 0, the FIFO has
 * data and the TSFIFO_x has a valid set of data. This register can be
 * polled and when the bit is cleared, the other registers should be read
 * to get a full timestamp. When 1, the last data has already been read out
 * and the current read data should be discarded.
 * Timestamp/Frame signature bytes are packed such that the 10 or 4 byte
 * timestamp resides in the LEAST significant bytes while the frame
 * signature (0 to 16 bytes) resides in the MOST significant bytes. The
 * order of the bytes within each timestamp/frame signature field is also
 * most significant to least significant.
 * For example, 26 byte timestamp/frame signature pairs are packed with the
 * 10 byte timestamp field ([79:0]) corresponding to Bits 79:0 in the
 * following registers, and a 16 byte frame signature field ([127:0])
 * corresponding to Bits 207:80 in the following registers.
 *
 * \details 
 * 0: FIFO not empty, data valid
 * 1: FIFO empty, data invalid
 *
 * Field: VTSS_PTP_3_EGR_IP_1588_TSFIFO_EGR_TSFIFO_0 . EGR_TS_EMPTY
 */
#define  VTSS_F_PTP_3_EGR_IP_1588_TSFIFO_EGR_TSFIFO_0_EGR_TS_EMPTY  VTSS_BIT(31)

/** 
 * \brief
 * FIFO flags from the timestamp FIFO. These bits indicate how many
 * timestamps are valid in the current (not empty) 26 byte FIFO entry.
 *
 * \details 
 * 000: Only the end of a partial timestamp is valid in the current FIFO
 * entry (any remaining data is invalid)
 * 001: 1 valid timestamp begins in the current FIFO entry (any remaining
 * data is invalid)
 * 010: 2 valid timestamps begin in the current FIFO entry (any remaining
 * data is invalid)
 * 011: 3 valid timestamps begin in the current FIFO entry (any remaining
 * data is invalid)
 * 100: 4 valid timestamps begin in the current FIFO entry (any remaining
 * data is invalid)
 * 101: 5 valid timestamps begin in the current FIFO entry (any remaining
 * data is invalid)
 * 110: 6 valid timestamps begin in the current FIFO entry (any remaining
 * data is invalid)
 * 111: The current FIFO entry is fully packed with timestamps (all data is
 * valid)
 *
 * Field: VTSS_PTP_3_EGR_IP_1588_TSFIFO_EGR_TSFIFO_0 . EGR_TS_FLAGS
 */
#define  VTSS_F_PTP_3_EGR_IP_1588_TSFIFO_EGR_TSFIFO_0_EGR_TS_FLAGS(x)  VTSS_ENCODE_BITFIELD(x,28,3)
#define  VTSS_M_PTP_3_EGR_IP_1588_TSFIFO_EGR_TSFIFO_0_EGR_TS_FLAGS     VTSS_ENCODE_BITMASK(28,3)
#define  VTSS_X_PTP_3_EGR_IP_1588_TSFIFO_EGR_TSFIFO_0_EGR_TS_FLAGS(x)  VTSS_EXTRACT_BITFIELD(x,28,3)

/** 
 * \brief
 * 16 bits from the timestamp FIFO. Bits 15:0
 *
 * \details 
 * Field: VTSS_PTP_3_EGR_IP_1588_TSFIFO_EGR_TSFIFO_0 . EGR_TSFIFO_0
 */
#define  VTSS_F_PTP_3_EGR_IP_1588_TSFIFO_EGR_TSFIFO_0_EGR_TSFIFO_0(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_PTP_3_EGR_IP_1588_TSFIFO_EGR_TSFIFO_0_EGR_TSFIFO_0     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_PTP_3_EGR_IP_1588_TSFIFO_EGR_TSFIFO_0_EGR_TSFIFO_0(x)  VTSS_EXTRACT_BITFIELD(x,0,16)


/** 
 * \brief Data value from the timestamp FIFO
 *
 * \details
 * Read the data from the timestamp FIFO
 *
 * Register: \a PTP_3:EGR_IP_1588_TSFIFO:EGR_TSFIFO_1
 */
#define VTSS_PTP_3_EGR_IP_1588_TSFIFO_EGR_TSFIFO_1  VTSS_IOREG(0x1e, 1, 0x925d)


/** 
 * \brief Data value from the timestamp FIFO
 *
 * \details
 * Read the data from the timestamp FIFO
 *
 * Register: \a PTP_3:EGR_IP_1588_TSFIFO:EGR_TSFIFO_2
 */
#define VTSS_PTP_3_EGR_IP_1588_TSFIFO_EGR_TSFIFO_2  VTSS_IOREG(0x1e, 1, 0x925e)


/** 
 * \brief Data value from the timestamp FIFO
 *
 * \details
 * Read the data from the timestamp FIFO
 *
 * Register: \a PTP_3:EGR_IP_1588_TSFIFO:EGR_TSFIFO_3
 */
#define VTSS_PTP_3_EGR_IP_1588_TSFIFO_EGR_TSFIFO_3  VTSS_IOREG(0x1e, 1, 0x925f)


/** 
 * \brief Data value from the timestamp FIFO
 *
 * \details
 * Read the data from the timestamp FIFO
 *
 * Register: \a PTP_3:EGR_IP_1588_TSFIFO:EGR_TSFIFO_4
 */
#define VTSS_PTP_3_EGR_IP_1588_TSFIFO_EGR_TSFIFO_4  VTSS_IOREG(0x1e, 1, 0x9260)


/** 
 * \brief Data value from the timestamp FIFO
 *
 * \details
 * Read the data from the timestamp FIFO
 *
 * Register: \a PTP_3:EGR_IP_1588_TSFIFO:EGR_TSFIFO_5
 */
#define VTSS_PTP_3_EGR_IP_1588_TSFIFO_EGR_TSFIFO_5  VTSS_IOREG(0x1e, 1, 0x9261)


/** 
 * \brief Data value from the timestamp FIFO
 *
 * \details
 * Read the data from the timestamp FIFO
 *
 * Register: \a PTP_3:EGR_IP_1588_TSFIFO:EGR_TSFIFO_6
 */
#define VTSS_PTP_3_EGR_IP_1588_TSFIFO_EGR_TSFIFO_6  VTSS_IOREG(0x1e, 1, 0x9262)


/** 
 * \brief Count of dropped timestamps
 *
 * \details
 * Count of dropped timestamps not enqueued to the TS FIFO
 *
 * Register: \a PTP_3:EGR_IP_1588_TSFIFO:EGR_TSFIFO_DROP_CNT
 */
#define VTSS_PTP_3_EGR_IP_1588_TSFIFO_EGR_TSFIFO_DROP_CNT  VTSS_IOREG(0x1e, 1, 0x9263)

/**
 * Register Group: \a PTP_3:EGR_IP_1588_RW
 *
 * 1588 IP rewriter
 */


/** 
 * \brief Rewriter configuration and control
 *
 * \details
 * Configuration for the rewriter
 *
 * Register: \a PTP_3:EGR_IP_1588_RW:EGR_RW_CTRL
 */
#define VTSS_PTP_3_EGR_IP_1588_RW_EGR_RW_CTRL  VTSS_IOREG(0x1e, 1, 0x9264)

/** 
 * \brief
 * When set, the 1588 IP will reduce the preamble of ALL incoming frames by
 * 4 bytes to allow a timestamp to be appended to the ingress data frames.
 * This bit must be set along with the proper configuration of the Analyzer
 * to ensure proper operation. Note: Valid in ingress direction only
 *
 * \details 
 * 0: No preamble modification
 * 1: Reduce preamble by 4 bytes
 *
 * Field: VTSS_PTP_3_EGR_IP_1588_RW_EGR_RW_CTRL . EGR_RW_REDUCE_PREAMBLE
 */
#define  VTSS_F_PTP_3_EGR_IP_1588_RW_EGR_RW_CTRL_EGR_RW_REDUCE_PREAMBLE  VTSS_BIT(4)

/** 
 * \brief
 * Value to write to the flag bit when it is overwritten
 *
 * \details 
 * 0: 0 will be written to the flag bit
 * 1: 1 will be written to the flag bit
 *
 * Field: VTSS_PTP_3_EGR_IP_1588_RW_EGR_RW_CTRL . EGR_RW_FLAG_VAL
 */
#define  VTSS_F_PTP_3_EGR_IP_1588_RW_EGR_RW_CTRL_EGR_RW_FLAG_VAL  VTSS_BIT(3)

/** 
 * \brief
 * Bit offset within a byte of the flag bit that indicates if the frame has
 * been modified
 *
 * \details 
 * Binary number
 *
 * Field: VTSS_PTP_3_EGR_IP_1588_RW_EGR_RW_CTRL . EGR_RW_FLAG_BIT
 */
#define  VTSS_F_PTP_3_EGR_IP_1588_RW_EGR_RW_CTRL_EGR_RW_FLAG_BIT(x)  VTSS_ENCODE_BITFIELD(x,0,3)
#define  VTSS_M_PTP_3_EGR_IP_1588_RW_EGR_RW_CTRL_EGR_RW_FLAG_BIT     VTSS_ENCODE_BITMASK(0,3)
#define  VTSS_X_PTP_3_EGR_IP_1588_RW_EGR_RW_CTRL_EGR_RW_FLAG_BIT(x)  VTSS_EXTRACT_BITFIELD(x,0,3)


/** 
 * \brief Count of modified frames
 *
 * \details
 * Register: \a PTP_3:EGR_IP_1588_RW:EGR_RW_MODFRM_CNT
 */
#define VTSS_PTP_3_EGR_IP_1588_RW_EGR_RW_MODFRM_CNT  VTSS_IOREG(0x1e, 1, 0x9265)


/** 
 * \brief Count of FCS errors
 *
 * \details
 * Register: \a PTP_3:EGR_IP_1588_RW:EGR_RW_FCS_ERR_CNT
 */
#define VTSS_PTP_3_EGR_IP_1588_RW_EGR_RW_FCS_ERR_CNT  VTSS_IOREG(0x1e, 1, 0x9266)


/** 
 * \brief Count of the number of preamble errors
 *
 * \details
 * Register: \a PTP_3:EGR_IP_1588_RW:EGR_RW_PREAMBLE_ERR_CNT
 */
#define VTSS_PTP_3_EGR_IP_1588_RW_EGR_RW_PREAMBLE_ERR_CNT  VTSS_IOREG(0x1e, 1, 0x9267)

/**
 * Register Group: \a PTP_3:EGR_IP_ANALYZER_STAT
 *
 * Analyzer Counters and Errors
 */


/** 
 * \brief Analyzer Modified Frames Counter
 *
 * \details
 * Register: \a PTP_3:EGR_IP_ANALYZER_STAT:EGR_ANALYZER_MOD_FRAMES_CNT
 */
#define VTSS_PTP_3_EGR_IP_ANALYZER_STAT_EGR_ANALYZER_MOD_FRAMES_CNT  VTSS_IOREG(0x1e, 1, 0x926c)

/**
 * Register Group: \a PTP_3:IP_1588_LTC_CFG
 *
 * 1588 IP local time counter
 */


/** 
 * \brief LTC PPS generation
 *
 * \details
 * LTC PPS generation
 *
 * Register: \a PTP_3:IP_1588_LTC_CFG:LTC_PPS_GEN
 */
#define VTSS_PTP_3_IP_1588_LTC_CFG_LTC_PPS_GEN  VTSS_IOREG(0x1e, 1, 0x926d)

/** 
 * \brief
 * PPS pulse is generated when LTC crosses PPS_GEN_CNT value. Only values 0
 * to 0x3B9AC9FF are relevant.
 *
 * \details 
 * Field: VTSS_PTP_3_IP_1588_LTC_CFG_LTC_PPS_GEN . PPS_GEN_CNT
 */
#define  VTSS_F_PTP_3_IP_1588_LTC_CFG_LTC_PPS_GEN_PPS_GEN_CNT(x)  VTSS_ENCODE_BITFIELD(x,0,30)
#define  VTSS_M_PTP_3_IP_1588_LTC_CFG_LTC_PPS_GEN_PPS_GEN_CNT     VTSS_ENCODE_BITMASK(0,30)
#define  VTSS_X_PTP_3_IP_1588_LTC_CFG_LTC_PPS_GEN_PPS_GEN_CNT(x)  VTSS_EXTRACT_BITFIELD(x,0,30)


/** 
 * \brief Serial ToD Interface
 *
 * \details
 * Serial ToD Interface
 *
 * Register: \a PTP_3:IP_1588_LTC_CFG:SER_TOD_INTF
 */
#define VTSS_PTP_3_IP_1588_LTC_CFG_SER_TOD_INTF  VTSS_IOREG(0x1e, 1, 0x926e)

/** 
 * \brief
 * This value will be added to LTC timer whenever a new value is loaded
 * into LTC timer either by serial or parallel interface.
 *
 * \details 
 * Field: VTSS_PTP_3_IP_1588_LTC_CFG_SER_TOD_INTF . LOAD_PULSE_DLY
 */
#define  VTSS_F_PTP_3_IP_1588_LTC_CFG_SER_TOD_INTF_LOAD_PULSE_DLY(x)  VTSS_ENCODE_BITFIELD(x,8,10)
#define  VTSS_M_PTP_3_IP_1588_LTC_CFG_SER_TOD_INTF_LOAD_PULSE_DLY     VTSS_ENCODE_BITMASK(8,10)
#define  VTSS_X_PTP_3_IP_1588_LTC_CFG_SER_TOD_INTF_LOAD_PULSE_DLY(x)  VTSS_EXTRACT_BITFIELD(x,8,10)

/** 
 * \brief
 * When enabled will loopback the PPS signal at the pad will be looped back
 * onto load_save
 *
 * \details 
 * Field: VTSS_PTP_3_IP_1588_LTC_CFG_SER_TOD_INTF . PPS_PAD_LPBK_EN
 */
#define  VTSS_F_PTP_3_IP_1588_LTC_CFG_SER_TOD_INTF_PPS_PAD_LPBK_EN  VTSS_BIT(7)

/** 
 * \brief
 * When enabled will loopback the PPS signal onto load_save input pin
 *
 * \details 
 * Field: VTSS_PTP_3_IP_1588_LTC_CFG_SER_TOD_INTF . PPS_LOAD_SAVE_LPBK_EN
 */
#define  VTSS_F_PTP_3_IP_1588_LTC_CFG_SER_TOD_INTF_PPS_LOAD_SAVE_LPBK_EN  VTSS_BIT(6)

/** 
 * \brief
 * Once enabled, load_save_pin will become a output pin for a three clocks
 * after LOAD_SAVE_LPBK_TIMER value. This is used to diagnose the cable
 * delay.
 *
 * \details 
 * Field: VTSS_PTP_3_IP_1588_LTC_CFG_SER_TOD_INTF . LOAD_SAVE_LPBK_EN
 */
#define  VTSS_F_PTP_3_IP_1588_LTC_CFG_SER_TOD_INTF_LOAD_SAVE_LPBK_EN  VTSS_BIT(5)

/** 
 * \brief
 * When enabled the asynchronous load_save_i input is looped back to the
 * pps_o output pin.
 *
 * \details 
 * Field: VTSS_PTP_3_IP_1588_LTC_CFG_SER_TOD_INTF . LOAD_SAVE_PPS_LPBK_EN
 */
#define  VTSS_F_PTP_3_IP_1588_LTC_CFG_SER_TOD_INTF_LOAD_SAVE_PPS_LPBK_EN  VTSS_BIT(4)

/** 
 * \brief
 * Load save input inversion
 *
 * \details 
 * 0: Not inverted
 * 1: Inverted
 *
 * Field: VTSS_PTP_3_IP_1588_LTC_CFG_SER_TOD_INTF . LOAD_SAVE_INV
 */
#define  VTSS_F_PTP_3_IP_1588_LTC_CFG_SER_TOD_INTF_LOAD_SAVE_INV  VTSS_BIT(3)

/** 
 * \brief
 * When enabled LTC_SAVE_ENA and LTC_LOAD_ENA will be cleared as soon as
 * the LOAD/SAVE operation is done.
 *
 * \details 
 * 0: Manually clear bits
 * 1: Auto clear when one-shot operation is done
 *
 * Field: VTSS_PTP_3_IP_1588_LTC_CFG_SER_TOD_INTF . LOAD_SAVE_AUTO_CLR
 */
#define  VTSS_F_PTP_3_IP_1588_LTC_CFG_SER_TOD_INTF_LOAD_SAVE_AUTO_CLR  VTSS_BIT(2)

/** 
 * \brief
 * Enable the generation of serial ToD through serial ToD output interface
 *
 * \details 
 * Field: VTSS_PTP_3_IP_1588_LTC_CFG_SER_TOD_INTF . SER_TOD_OUTPUT_EN
 */
#define  VTSS_F_PTP_3_IP_1588_LTC_CFG_SER_TOD_INTF_SER_TOD_OUTPUT_EN  VTSS_BIT(1)

/** 
 * \brief
 * Enable the load/save of serial ToD through serial ToD input interface
 *
 * \details 
 * Field: VTSS_PTP_3_IP_1588_LTC_CFG_SER_TOD_INTF . SER_TOD_INPUT_EN
 */
#define  VTSS_F_PTP_3_IP_1588_LTC_CFG_SER_TOD_INTF_SER_TOD_INPUT_EN  VTSS_BIT(0)


/** 
 * \brief LTC PPS Return Input
 *
 * \details
 * LTC PPS Return Input
 *
 * Register: \a PTP_3:IP_1588_LTC_CFG:LTC_PPS_RI
 */
#define VTSS_PTP_3_IP_1588_LTC_CFG_LTC_PPS_RI  VTSS_IOREG(0x1e, 1, 0x926f)

/** 
 * \brief
 * Set by hardware when a new value is load into PPS_RI_TIME
 * 
 * This bit is cleared by writing a '1'.

 *
 * \details 
 * Field: VTSS_PTP_3_IP_1588_LTC_CFG_LTC_PPS_RI . PPS_RI_VALID
 */
#define  VTSS_F_PTP_3_IP_1588_LTC_CFG_LTC_PPS_RI_PPS_RI_VALID  VTSS_BIT(30)

/** 
 * \brief
 * This register shows the ns part of the LTC timer value captured on the
 * detection of the rising edge of the pps_ri signal 
 *
 * \details 
 * Field: VTSS_PTP_3_IP_1588_LTC_CFG_LTC_PPS_RI . PPS_RI_TIME
 */
#define  VTSS_F_PTP_3_IP_1588_LTC_CFG_LTC_PPS_RI_PPS_RI_TIME(x)  VTSS_ENCODE_BITFIELD(x,0,30)
#define  VTSS_M_PTP_3_IP_1588_LTC_CFG_LTC_PPS_RI_PPS_RI_TIME     VTSS_ENCODE_BITMASK(0,30)
#define  VTSS_X_PTP_3_IP_1588_LTC_CFG_LTC_PPS_RI_PPS_RI_TIME(x)  VTSS_EXTRACT_BITFIELD(x,0,30)


/** 
 * \brief LTC Timer Offset Adjustment
 *
 * \details
 * LTC Timer Offset Adjustment
 *
 * Register: \a PTP_3:IP_1588_LTC_CFG:LTC_OFFSET_ADJ
 */
#define VTSS_PTP_3_IP_1588_LTC_CFG_LTC_OFFSET_ADJ  VTSS_IOREG(0x1e, 1, 0x9270)

/** 
 * \brief
 * When this bit is set to 1'b1, the offset value LTC_OFFSET_VAL is
 * added/subtracted to the LTC timer, which is free running. This bit is
 * cleared by hardware after the operation is complete and status of this
 * operation is reflected in LTC_OFFSET_ADJ_STAT register. 
 *
 * \details 
 * Field: VTSS_PTP_3_IP_1588_LTC_CFG_LTC_OFFSET_ADJ . LTC_OFFSET_ADJ
 */
#define  VTSS_F_PTP_3_IP_1588_LTC_CFG_LTC_OFFSET_ADJ_LTC_OFFSET_ADJ  VTSS_BIT(31)

/** 
 * \brief
 * The LTC load offset is added to or subtracted from/to the LTC value when
 * LTC_OFFSET_ADJ is asserted.
 *
 * \details 
 * 1'b1: Add
 * 1'b0: Subtract
 *
 * Field: VTSS_PTP_3_IP_1588_LTC_CFG_LTC_OFFSET_ADJ . LTC_ADD_SUB_OFFSET
 */
#define  VTSS_F_PTP_3_IP_1588_LTC_CFG_LTC_OFFSET_ADJ_LTC_ADD_SUB_OFFSET  VTSS_BIT(30)

/** 
 * \brief
 * This offset is added to/subtracted from the LTC value whenever
 * LTC_OFFSET_ADJ transitions from 0->1. The maximum value of the field in
 * nano seconds can be [(10^9) - 1].
 *
 * \details 
 * Field: VTSS_PTP_3_IP_1588_LTC_CFG_LTC_OFFSET_ADJ . LTC_OFFSET_VAL
 */
#define  VTSS_F_PTP_3_IP_1588_LTC_CFG_LTC_OFFSET_ADJ_LTC_OFFSET_VAL(x)  VTSS_ENCODE_BITFIELD(x,0,30)
#define  VTSS_M_PTP_3_IP_1588_LTC_CFG_LTC_OFFSET_ADJ_LTC_OFFSET_VAL     VTSS_ENCODE_BITMASK(0,30)
#define  VTSS_X_PTP_3_IP_1588_LTC_CFG_LTC_OFFSET_ADJ_LTC_OFFSET_VAL(x)  VTSS_EXTRACT_BITFIELD(x,0,30)


/** 
 * \brief Control and status for LTC_OFFSET_ADJ operation
 *
 * \details
 * Control and status for LTC_OFFSET_ADJ operation
 *
 * Register: \a PTP_3:IP_1588_LTC_CFG:LTC_OFFSET_ADJ_STAT
 */
#define VTSS_PTP_3_IP_1588_LTC_CFG_LTC_OFFSET_ADJ_STAT  VTSS_IOREG(0x1e, 1, 0x9271)

/** 
 * \brief
 * Indicates the type of error when PPS_RI_CALIB_ERR is set.
 *
 * \details 
 * Field: VTSS_PTP_3_IP_1588_LTC_CFG_LTC_OFFSET_ADJ_STAT . PPS_RI_CALIB_CODE_ERR
 */
#define  VTSS_F_PTP_3_IP_1588_LTC_CFG_LTC_OFFSET_ADJ_STAT_PPS_RI_CALIB_CODE_ERR(x)  VTSS_ENCODE_BITFIELD(x,28,2)
#define  VTSS_M_PTP_3_IP_1588_LTC_CFG_LTC_OFFSET_ADJ_STAT_PPS_RI_CALIB_CODE_ERR     VTSS_ENCODE_BITMASK(28,2)
#define  VTSS_X_PTP_3_IP_1588_LTC_CFG_LTC_OFFSET_ADJ_STAT_PPS_RI_CALIB_CODE_ERR(x)  VTSS_EXTRACT_BITFIELD(x,28,2)

/** 
 * \brief
 * Indicates the type of error when PPS_CALIB_ERR is set.
 *
 * \details 
 * Field: VTSS_PTP_3_IP_1588_LTC_CFG_LTC_OFFSET_ADJ_STAT . PPS_CALIB_CODE_ERR
 */
#define  VTSS_F_PTP_3_IP_1588_LTC_CFG_LTC_OFFSET_ADJ_STAT_PPS_CALIB_CODE_ERR(x)  VTSS_ENCODE_BITFIELD(x,24,2)
#define  VTSS_M_PTP_3_IP_1588_LTC_CFG_LTC_OFFSET_ADJ_STAT_PPS_CALIB_CODE_ERR     VTSS_ENCODE_BITMASK(24,2)
#define  VTSS_X_PTP_3_IP_1588_LTC_CFG_LTC_OFFSET_ADJ_STAT_PPS_CALIB_CODE_ERR(x)  VTSS_EXTRACT_BITFIELD(x,24,2)

/** 
 * \brief
 * Indicates the type of error when EGR_CALIB_ERR is set.
 *
 * \details 
 * Field: VTSS_PTP_3_IP_1588_LTC_CFG_LTC_OFFSET_ADJ_STAT . EGR_CALIB_CODE_ERR
 */
#define  VTSS_F_PTP_3_IP_1588_LTC_CFG_LTC_OFFSET_ADJ_STAT_EGR_CALIB_CODE_ERR(x)  VTSS_ENCODE_BITFIELD(x,20,2)
#define  VTSS_M_PTP_3_IP_1588_LTC_CFG_LTC_OFFSET_ADJ_STAT_EGR_CALIB_CODE_ERR     VTSS_ENCODE_BITMASK(20,2)
#define  VTSS_X_PTP_3_IP_1588_LTC_CFG_LTC_OFFSET_ADJ_STAT_EGR_CALIB_CODE_ERR(x)  VTSS_EXTRACT_BITFIELD(x,20,2)

/** 
 * \brief
 * Indicates the type of error when IGR_CALIB_ERR is set.
 *
 * \details 
 * Field: VTSS_PTP_3_IP_1588_LTC_CFG_LTC_OFFSET_ADJ_STAT . IGR_CALIB_CODE_ERR
 */
#define  VTSS_F_PTP_3_IP_1588_LTC_CFG_LTC_OFFSET_ADJ_STAT_IGR_CALIB_CODE_ERR(x)  VTSS_ENCODE_BITFIELD(x,16,2)
#define  VTSS_M_PTP_3_IP_1588_LTC_CFG_LTC_OFFSET_ADJ_STAT_IGR_CALIB_CODE_ERR     VTSS_ENCODE_BITMASK(16,2)
#define  VTSS_X_PTP_3_IP_1588_LTC_CFG_LTC_OFFSET_ADJ_STAT_IGR_CALIB_CODE_ERR(x)  VTSS_EXTRACT_BITFIELD(x,16,2)

/** 
 * \brief
 * Indicates the type of error when LS_CALIB_ERR is set.
 *
 * \details 
 * Field: VTSS_PTP_3_IP_1588_LTC_CFG_LTC_OFFSET_ADJ_STAT . LS_CALIB_CODE_ERR
 */
#define  VTSS_F_PTP_3_IP_1588_LTC_CFG_LTC_OFFSET_ADJ_STAT_LS_CALIB_CODE_ERR(x)  VTSS_ENCODE_BITFIELD(x,12,2)
#define  VTSS_M_PTP_3_IP_1588_LTC_CFG_LTC_OFFSET_ADJ_STAT_LS_CALIB_CODE_ERR     VTSS_ENCODE_BITMASK(12,2)
#define  VTSS_X_PTP_3_IP_1588_LTC_CFG_LTC_OFFSET_ADJ_STAT_LS_CALIB_CODE_ERR(x)  VTSS_EXTRACT_BITFIELD(x,12,2)

/** 
 * \brief
 * Set by the hardware when it encounters an error while trying to offset
 * the LTC value. For example, the load offset command happens at the same
 * time when 1 pps occurs. The software is advised to retry the load offset
 * command instead of waiting on the status bit, LTC_OFFSET_DONE.
 *
 * \details 
 * Field: VTSS_PTP_3_IP_1588_LTC_CFG_LTC_OFFSET_ADJ_STAT . LTC_OFFSET_LOAD_ERR
 */
#define  VTSS_F_PTP_3_IP_1588_LTC_CFG_LTC_OFFSET_ADJ_STAT_LTC_OFFSET_LOAD_ERR  VTSS_BIT(1)

/** 
 * \brief
 * Set by the hardware when the LTC value is offset as configured.
 * Reset by software to clear the load offset control and status or to give
 * a new offset command.
 *
 * \details 
 * Field: VTSS_PTP_3_IP_1588_LTC_CFG_LTC_OFFSET_ADJ_STAT . LTC_OFFSET_DONE
 */
#define  VTSS_F_PTP_3_IP_1588_LTC_CFG_LTC_OFFSET_ADJ_STAT_LTC_OFFSET_DONE  VTSS_BIT(0)


/** 
 * \brief LTC auto adjustment M counter
 *
 * \details
 * LTC auto adjustment M counter
 *
 * Register: \a PTP_3:IP_1588_LTC_CFG:LTC_AUTO_ADJUST_M
 */
#define VTSS_PTP_3_IP_1588_LTC_CFG_LTC_AUTO_ADJUST_M  VTSS_IOREG(0x1e, 1, 0x9272)

/** 
 * \brief
 * When written to a '1',  AUTO_ADJ_M_COUNTER uses programmed
 * LTC_AUTO_ADJ_M_NS and LTC_AUTO_ADJ_M values.
 * Automatically cleared.
 * 0 = No change to any previous updates (write), or
 * update has completed (read)
 * 1 = Use new values. 

 *
 * \details 
 * Field: VTSS_PTP_3_IP_1588_LTC_CFG_LTC_AUTO_ADJUST_M . LTC_AUTO_ADJ_M_UPDATE
 */
#define  VTSS_F_PTP_3_IP_1588_LTC_CFG_LTC_AUTO_ADJUST_M_LTC_AUTO_ADJ_M_UPDATE  VTSS_BIT(22)

/** 
 * \brief
 * LTC auto adjustment M add/subtract 1 ns
 *
 * \details 
 * 0,3: No adjustment
 * 1: Adjust by adding 1 ns upon rollover
 * 2: Adjust by subtracting 1 ns upon rollover
 *
 * Field: VTSS_PTP_3_IP_1588_LTC_CFG_LTC_AUTO_ADJUST_M . LTC_AUTO_ADJ_M
 */
#define  VTSS_F_PTP_3_IP_1588_LTC_CFG_LTC_AUTO_ADJUST_M_LTC_AUTO_ADJ_M(x)  VTSS_ENCODE_BITFIELD(x,20,2)
#define  VTSS_M_PTP_3_IP_1588_LTC_CFG_LTC_AUTO_ADJUST_M_LTC_AUTO_ADJ_M     VTSS_ENCODE_BITMASK(20,2)
#define  VTSS_X_PTP_3_IP_1588_LTC_CFG_LTC_AUTO_ADJUST_M_LTC_AUTO_ADJ_M(x)  VTSS_EXTRACT_BITFIELD(x,20,2)

/** 
 * \brief
 * LTC auto adjustment rollover 2 (nanoseconds)
 *
 * \details 
 * Field: VTSS_PTP_3_IP_1588_LTC_CFG_LTC_AUTO_ADJUST_M . LTC_AUTO_ADJ_M_NS
 */
#define  VTSS_F_PTP_3_IP_1588_LTC_CFG_LTC_AUTO_ADJUST_M_LTC_AUTO_ADJ_M_NS(x)  VTSS_ENCODE_BITFIELD(x,0,20)
#define  VTSS_M_PTP_3_IP_1588_LTC_CFG_LTC_AUTO_ADJUST_M_LTC_AUTO_ADJ_M_NS     VTSS_ENCODE_BITMASK(0,20)
#define  VTSS_X_PTP_3_IP_1588_LTC_CFG_LTC_AUTO_ADJUST_M_LTC_AUTO_ADJ_M_NS(x)  VTSS_EXTRACT_BITFIELD(x,0,20)


/** 
 * \brief LTC Frequency Synthesis pulse generation
 *
 * \details
 * LTC Frequency Synthesis pulse generation
 *
 * Register: \a PTP_3:IP_1588_LTC_CFG:LTC_FREQ_SYNTH
 */
#define VTSS_PTP_3_IP_1588_LTC_CFG_LTC_FREQ_SYNTH  VTSS_IOREG(0x1e, 1, 0x9273)

/** 
 * \brief
 * When set, LS_CALIBA_IN and LS_CALIBB_IN values will overridde PPS RI
 * calibration values.
 *
 * \details 
 * Field: VTSS_PTP_3_IP_1588_LTC_CFG_LTC_FREQ_SYNTH . PPS_RI_CALIB_OVERRIDE
 */
#define  VTSS_F_PTP_3_IP_1588_LTC_CFG_LTC_FREQ_SYNTH_PPS_RI_CALIB_OVERRIDE  VTSS_BIT(22)

/** 
 * \brief
 * When set, LS_CALIBA_IN and LS_CALIBB_IN values will overridde egress
 * calibration values.
 *
 * \details 
 * Field: VTSS_PTP_3_IP_1588_LTC_CFG_LTC_FREQ_SYNTH . EGR_CALIB_OVERRIDE
 */
#define  VTSS_F_PTP_3_IP_1588_LTC_CFG_LTC_FREQ_SYNTH_EGR_CALIB_OVERRIDE  VTSS_BIT(21)

/** 
 * \brief
 * When set, LS_CALIBA_IN and LS_CALIBB_IN values will overridde ingress
 * calibration values.
 *
 * \details 
 * Field: VTSS_PTP_3_IP_1588_LTC_CFG_LTC_FREQ_SYNTH . INGR_CALIB_OVERRIDE
 */
#define  VTSS_F_PTP_3_IP_1588_LTC_CFG_LTC_FREQ_SYNTH_INGR_CALIB_OVERRIDE  VTSS_BIT(20)

/** 
 * \brief
 * When enabled, this will drive data that toggles with duty cycle
 * specified in LTC_FREQ_DUTY_CYC_NS
 *
 * \details 
 * Field: VTSS_PTP_3_IP_1588_LTC_CFG_LTC_FREQ_SYNTH . LTC_FREQ_SYNTH_EN
 */
#define  VTSS_F_PTP_3_IP_1588_LTC_CFG_LTC_FREQ_SYNTH_LTC_FREQ_SYNTH_EN  VTSS_BIT(16)

/** 
 * \brief
 * Denotes the Low time of Frequency Synthesis pulse.
 *
 * \details 
 * Field: VTSS_PTP_3_IP_1588_LTC_CFG_LTC_FREQ_SYNTH . FREQ_HI_DUTY_CYC_NS
 */
#define  VTSS_F_PTP_3_IP_1588_LTC_CFG_LTC_FREQ_SYNTH_FREQ_HI_DUTY_CYC_NS(x)  VTSS_ENCODE_BITFIELD(x,8,8)
#define  VTSS_M_PTP_3_IP_1588_LTC_CFG_LTC_FREQ_SYNTH_FREQ_HI_DUTY_CYC_NS     VTSS_ENCODE_BITMASK(8,8)
#define  VTSS_X_PTP_3_IP_1588_LTC_CFG_LTC_FREQ_SYNTH_FREQ_HI_DUTY_CYC_NS(x)  VTSS_EXTRACT_BITFIELD(x,8,8)

/** 
 * \brief
 * Denotes the High time of Frequency Synthesis pulse.
 *
 * \details 
 * Field: VTSS_PTP_3_IP_1588_LTC_CFG_LTC_FREQ_SYNTH . FREQ_LO_DUTY_CYC_NS
 */
#define  VTSS_F_PTP_3_IP_1588_LTC_CFG_LTC_FREQ_SYNTH_FREQ_LO_DUTY_CYC_NS(x)  VTSS_ENCODE_BITFIELD(x,0,8)
#define  VTSS_M_PTP_3_IP_1588_LTC_CFG_LTC_FREQ_SYNTH_FREQ_LO_DUTY_CYC_NS     VTSS_ENCODE_BITMASK(0,8)
#define  VTSS_X_PTP_3_IP_1588_LTC_CFG_LTC_FREQ_SYNTH_FREQ_LO_DUTY_CYC_NS(x)  VTSS_EXTRACT_BITFIELD(x,0,8)

/**
 * Register Group: \a PTP_3:IP_1588_ACC_CFG
 *
 * 1588 higher accuracy
 */


/** 
 * \brief Configuration for Higher accuracy
 *
 * \details
 * Configuration for Higher accuracy
 *
 * Register: \a PTP_3:IP_1588_ACC_CFG:ACC_CFG_STATUS
 */
#define VTSS_PTP_3_IP_1588_ACC_CFG_ACC_CFG_STATUS  VTSS_IOREG(0x1e, 1, 0x9274)

/** 
 * \brief
 * When set indicates the error in the calibration.
 *
 * \details 
 * Field: VTSS_PTP_3_IP_1588_ACC_CFG_ACC_CFG_STATUS . PPS_O_CALIB_ERR
 */
#define  VTSS_F_PTP_3_IP_1588_ACC_CFG_ACC_CFG_STATUS_PPS_O_CALIB_ERR  VTSS_BIT(17)

/** 
 * \brief
 * Calibration done status signal for PPS_O 
 *
 * \details 
 * Field: VTSS_PTP_3_IP_1588_ACC_CFG_ACC_CFG_STATUS . PPS_O_CALIB_DONE
 */
#define  VTSS_F_PTP_3_IP_1588_ACC_CFG_ACC_CFG_STATUS_PPS_O_CALIB_DONE  VTSS_BIT(16)

/** 
 * \brief
 * When set, PPS_CALIBA_IN and PPS_CALIBB_IN values will be overridden with
 * the Offset sampler calibration values.
 *
 * \details 
 * Field: VTSS_PTP_3_IP_1588_ACC_CFG_ACC_CFG_STATUS . PPS_O_CALIB_OVERRIDE
 */
#define  VTSS_F_PTP_3_IP_1588_ACC_CFG_ACC_CFG_STATUS_PPS_O_CALIB_OVERRIDE  VTSS_BIT(15)

/** 
 * \brief
 * When set indicates the error in the calibration.
 *
 * \details 
 * Field: VTSS_PTP_3_IP_1588_ACC_CFG_ACC_CFG_STATUS . PPS_RI_CALIB_ERR
 */
#define  VTSS_F_PTP_3_IP_1588_ACC_CFG_ACC_CFG_STATUS_PPS_RI_CALIB_ERR  VTSS_BIT(14)

/** 
 * \brief
 * Calibration done status signal for PPS_RI 
 *
 * \details 
 * Field: VTSS_PTP_3_IP_1588_ACC_CFG_ACC_CFG_STATUS . PPS_RI_CALIB_DONE
 */
#define  VTSS_F_PTP_3_IP_1588_ACC_CFG_ACC_CFG_STATUS_PPS_RI_CALIB_DONE  VTSS_BIT(13)

/** 
 * \brief
 * When set indicates the error in the calibration.
 *
 * \details 
 * Field: VTSS_PTP_3_IP_1588_ACC_CFG_ACC_CFG_STATUS . EGR_SOF_CALIB_ERR
 */
#define  VTSS_F_PTP_3_IP_1588_ACC_CFG_ACC_CFG_STATUS_EGR_SOF_CALIB_ERR  VTSS_BIT(12)

/** 
 * \brief
 * Calibration done status signal for Egress SOF
 *
 * \details 
 * Field: VTSS_PTP_3_IP_1588_ACC_CFG_ACC_CFG_STATUS . EGR_SOF_CALIB_DONE
 */
#define  VTSS_F_PTP_3_IP_1588_ACC_CFG_ACC_CFG_STATUS_EGR_SOF_CALIB_DONE  VTSS_BIT(11)

/** 
 * \brief
 * When set indicates the error in the calibration.
 *
 * \details 
 * Field: VTSS_PTP_3_IP_1588_ACC_CFG_ACC_CFG_STATUS . IGR_SOF_CALIB_ERR
 */
#define  VTSS_F_PTP_3_IP_1588_ACC_CFG_ACC_CFG_STATUS_IGR_SOF_CALIB_ERR  VTSS_BIT(10)

/** 
 * \brief
 * Calibration done status signal for Ingress SOF
 *
 * \details 
 * Field: VTSS_PTP_3_IP_1588_ACC_CFG_ACC_CFG_STATUS . IGR_SOF_CALIB_DONE
 */
#define  VTSS_F_PTP_3_IP_1588_ACC_CFG_ACC_CFG_STATUS_IGR_SOF_CALIB_DONE  VTSS_BIT(9)

/** 
 * \brief
 * When set indicates the error in the calibration.
 *
 * \details 
 * Field: VTSS_PTP_3_IP_1588_ACC_CFG_ACC_CFG_STATUS . LS_CALIB_ERR
 */
#define  VTSS_F_PTP_3_IP_1588_ACC_CFG_ACC_CFG_STATUS_LS_CALIB_ERR  VTSS_BIT(8)

/** 
 * \brief
 * Calibration done status signal for Load Save
 *
 * \details 
 * Field: VTSS_PTP_3_IP_1588_ACC_CFG_ACC_CFG_STATUS . LS_CALIB_DONE
 */
#define  VTSS_F_PTP_3_IP_1588_ACC_CFG_ACC_CFG_STATUS_LS_CALIB_DONE  VTSS_BIT(7)

/** 
 * \brief
 * When set, calibration will start from minimum delays
 *
 * \details 
 * Field: VTSS_PTP_3_IP_1588_ACC_CFG_ACC_CFG_STATUS . LS_CALIB_TRIG
 */
#define  VTSS_F_PTP_3_IP_1588_ACC_CFG_ACC_CFG_STATUS_LS_CALIB_TRIG  VTSS_BIT(6)

/** 
 * \brief
 * When set, LS_CALIBA_IN and LS_CALIBB_IN values will be overridden with
 * the Offset sampler calibration values.
 *
 * \details 
 * Field: VTSS_PTP_3_IP_1588_ACC_CFG_ACC_CFG_STATUS . LS_CALIB_OVERRIDE
 */
#define  VTSS_F_PTP_3_IP_1588_ACC_CFG_ACC_CFG_STATUS_LS_CALIB_OVERRIDE  VTSS_BIT(5)

/** 
 * \brief
 * If enabled, this will bypass the accuracy block for pulse per sec output
 * pin
 *
 * \details 
 * Field: VTSS_PTP_3_IP_1588_ACC_CFG_ACC_CFG_STATUS . PPS_O_ACC_BYPASS
 */
#define  VTSS_F_PTP_3_IP_1588_ACC_CFG_ACC_CFG_STATUS_PPS_O_ACC_BYPASS  VTSS_BIT(4)

/** 
 * \brief
 * If enabled, this will bypass the accuracy block for pulse per sec return
 * input pin
 *
 * \details 
 * Field: VTSS_PTP_3_IP_1588_ACC_CFG_ACC_CFG_STATUS . PPS_RI_ACC_BYPASS
 */
#define  VTSS_F_PTP_3_IP_1588_ACC_CFG_ACC_CFG_STATUS_PPS_RI_ACC_BYPASS  VTSS_BIT(3)

/** 
 * \brief
 * If enabled, this will bypass the accuracy block for SOF in egress
 * direction
 *
 * \details 
 * Field: VTSS_PTP_3_IP_1588_ACC_CFG_ACC_CFG_STATUS . EGR_SOF_ACC_BYPASS
 */
#define  VTSS_F_PTP_3_IP_1588_ACC_CFG_ACC_CFG_STATUS_EGR_SOF_ACC_BYPASS  VTSS_BIT(2)

/** 
 * \brief
 * If enabled, this will bypass the accuracy block for SOF in ingress
 * direction
 *
 * \details 
 * Field: VTSS_PTP_3_IP_1588_ACC_CFG_ACC_CFG_STATUS . INGR_SOF_ACC_BYPASS
 */
#define  VTSS_F_PTP_3_IP_1588_ACC_CFG_ACC_CFG_STATUS_INGR_SOF_ACC_BYPASS  VTSS_BIT(1)

/** 
 * \brief
 * If enabled, this will bypass the accuracy block for load_save pin
 *
 * \details 
 * Field: VTSS_PTP_3_IP_1588_ACC_CFG_ACC_CFG_STATUS . LOAD_SAVE_ACC_BYPASS
 */
#define  VTSS_F_PTP_3_IP_1588_ACC_CFG_ACC_CFG_STATUS_LOAD_SAVE_ACC_BYPASS  VTSS_BIT(0)


/** 
 * \brief Calibration A value for LOAD_SAVE Offset sampler
 *
 * \details
 * Calibration A value for LOAD_SAVE Offset sampler
 *
 * Register: \a PTP_3:IP_1588_ACC_CFG:LS_CALIBA_IN
 */
#define VTSS_PTP_3_IP_1588_ACC_CFG_LS_CALIBA_IN  VTSS_IOREG(0x1e, 1, 0x9275)


/** 
 * \brief Calibration B value for LOAD_SAVE Offset sampler
 *
 * \details
 * Calibration B value for LOAD_SAVE Offset sampler
 *
 * Register: \a PTP_3:IP_1588_ACC_CFG:LS_CALIBB_IN
 */
#define VTSS_PTP_3_IP_1588_ACC_CFG_LS_CALIBB_IN  VTSS_IOREG(0x1e, 1, 0x9276)


/** 
 * \brief Calibration A value for PPS Offset sampler
 *
 * \details
 * Calibration A value for PPS Offset sampler
 *
 * Register: \a PTP_3:IP_1588_ACC_CFG:PPS_CALIBA_IN
 */
#define VTSS_PTP_3_IP_1588_ACC_CFG_PPS_CALIBA_IN  VTSS_IOREG(0x1e, 1, 0x9277)


/** 
 * \brief Calibration B value for PPS Offset sampler
 *
 * \details
 * Calibration B value for PPS Offset sampler
 *
 * Register: \a PTP_3:IP_1588_ACC_CFG:PPS_CALIBB_IN
 */
#define VTSS_PTP_3_IP_1588_ACC_CFG_PPS_CALIBB_IN  VTSS_IOREG(0x1e, 1, 0x9278)


/** 
 * \brief Calibration for Offset calculation
 *
 * \details
 * Calibration for Offset calculation
 *
 * Register: \a PTP_3:IP_1588_ACC_CFG:CALIB_OFFSET
 */
#define VTSS_PTP_3_IP_1588_ACC_CFG_CALIB_OFFSET  VTSS_IOREG(0x1e, 1, 0x9279)

/** 
 * \brief
 * Indicates the offset value from ingress SOF
 *
 * \details 
 * Field: VTSS_PTP_3_IP_1588_ACC_CFG_CALIB_OFFSET . PPS_RI_OFFSET
 */
#define  VTSS_F_PTP_3_IP_1588_ACC_CFG_CALIB_OFFSET_PPS_RI_OFFSET(x)  VTSS_ENCODE_BITFIELD(x,17,4)
#define  VTSS_M_PTP_3_IP_1588_ACC_CFG_CALIB_OFFSET_PPS_RI_OFFSET     VTSS_ENCODE_BITMASK(17,4)
#define  VTSS_X_PTP_3_IP_1588_ACC_CFG_CALIB_OFFSET_PPS_RI_OFFSET(x)  VTSS_EXTRACT_BITFIELD(x,17,4)

/** 
 * \brief
 * Indicates the offset value from ingress SOF
 *
 * \details 
 * Field: VTSS_PTP_3_IP_1588_ACC_CFG_CALIB_OFFSET . IGR_SOF_OFFSET
 */
#define  VTSS_F_PTP_3_IP_1588_ACC_CFG_CALIB_OFFSET_IGR_SOF_OFFSET(x)  VTSS_ENCODE_BITFIELD(x,13,4)
#define  VTSS_M_PTP_3_IP_1588_ACC_CFG_CALIB_OFFSET_IGR_SOF_OFFSET     VTSS_ENCODE_BITMASK(13,4)
#define  VTSS_X_PTP_3_IP_1588_ACC_CFG_CALIB_OFFSET_IGR_SOF_OFFSET(x)  VTSS_EXTRACT_BITFIELD(x,13,4)

/** 
 * \brief
 * Indicates the offset value from egress SOF
 *
 * \details 
 * Field: VTSS_PTP_3_IP_1588_ACC_CFG_CALIB_OFFSET . EGR_SOF_OFFSET
 */
#define  VTSS_F_PTP_3_IP_1588_ACC_CFG_CALIB_OFFSET_EGR_SOF_OFFSET(x)  VTSS_ENCODE_BITFIELD(x,9,4)
#define  VTSS_M_PTP_3_IP_1588_ACC_CFG_CALIB_OFFSET_EGR_SOF_OFFSET     VTSS_ENCODE_BITMASK(9,4)
#define  VTSS_X_PTP_3_IP_1588_ACC_CFG_CALIB_OFFSET_EGR_SOF_OFFSET(x)  VTSS_EXTRACT_BITFIELD(x,9,4)

/** 
 * \brief
 * Indicates the offset value from load_save Offset
 *
 * \details 
 * Field: VTSS_PTP_3_IP_1588_ACC_CFG_CALIB_OFFSET . LS_OFFSET
 */
#define  VTSS_F_PTP_3_IP_1588_ACC_CFG_CALIB_OFFSET_LS_OFFSET(x)  VTSS_ENCODE_BITFIELD(x,5,4)
#define  VTSS_M_PTP_3_IP_1588_ACC_CFG_CALIB_OFFSET_LS_OFFSET     VTSS_ENCODE_BITMASK(5,4)
#define  VTSS_X_PTP_3_IP_1588_ACC_CFG_CALIB_OFFSET_LS_OFFSET(x)  VTSS_EXTRACT_BITFIELD(x,5,4)

/** 
 * \brief
 * Indicates a valid PPS_RI_OFFSET value to be read
 *
 * \details 
 * Field: VTSS_PTP_3_IP_1588_ACC_CFG_CALIB_OFFSET . PPS_RI_OFFSET_VLD
 */
#define  VTSS_F_PTP_3_IP_1588_ACC_CFG_CALIB_OFFSET_PPS_RI_OFFSET_VLD  VTSS_BIT(3)

/** 
 * \brief
 * Indicates a valid IGR_SOF_OFFSET value to be read
 *
 * \details 
 * Field: VTSS_PTP_3_IP_1588_ACC_CFG_CALIB_OFFSET . IGR_SOF_OFFSET_VLD
 */
#define  VTSS_F_PTP_3_IP_1588_ACC_CFG_CALIB_OFFSET_IGR_SOF_OFFSET_VLD  VTSS_BIT(2)

/** 
 * \brief
 * Indicates a valid EGR_SOF_OFFSET value to be read
 *
 * \details 
 * Field: VTSS_PTP_3_IP_1588_ACC_CFG_CALIB_OFFSET . EGR_SOF_OFFSET_VLD
 */
#define  VTSS_F_PTP_3_IP_1588_ACC_CFG_CALIB_OFFSET_EGR_SOF_OFFSET_VLD  VTSS_BIT(1)

/** 
 * \brief
 * Indicates a valid LS_OFFSET value to be read
 *
 * \details 
 * Field: VTSS_PTP_3_IP_1588_ACC_CFG_CALIB_OFFSET . LS_OFFSET_VLD
 */
#define  VTSS_F_PTP_3_IP_1588_ACC_CFG_CALIB_OFFSET_LS_OFFSET_VLD  VTSS_BIT(0)


/** 
 * \brief Calibration delay chain A status value
 *
 * \details
 * Calibration delay chain A status value
 *
 * Register: \a PTP_3:IP_1588_ACC_CFG:LS_STATUS_CALIBA
 */
#define VTSS_PTP_3_IP_1588_ACC_CFG_LS_STATUS_CALIBA  VTSS_IOREG(0x1e, 1, 0x927a)


/** 
 * \brief Calibration delay chain B status value
 *
 * \details
 * Calibration delay chain B status value
 *
 * Register: \a PTP_3:IP_1588_ACC_CFG:LS_STATUS_CALIBB
 */
#define VTSS_PTP_3_IP_1588_ACC_CFG_LS_STATUS_CALIBB  VTSS_IOREG(0x1e, 1, 0x927b)


/** 
 * \brief Calibration delay chain A status value for Egress SOF
 *
 * \details
 * Calibration delay chain A status value for Egress SOF
 *
 * Register: \a PTP_3:IP_1588_ACC_CFG:EGR_STATUS_CALIBA
 */
#define VTSS_PTP_3_IP_1588_ACC_CFG_EGR_STATUS_CALIBA  VTSS_IOREG(0x1e, 1, 0x927c)


/** 
 * \brief Calibration delay chain B status value for Egress SOF
 *
 * \details
 * Calibration delay chain B status value for Egress SOF
 *
 * Register: \a PTP_3:IP_1588_ACC_CFG:EGR_STATUS_CALIBB
 */
#define VTSS_PTP_3_IP_1588_ACC_CFG_EGR_STATUS_CALIBB  VTSS_IOREG(0x1e, 1, 0x927d)


/** 
 * \brief Calibration delay chain A status value for ingress SOF
 *
 * \details
 * Calibration delay chain A status value for ingress SOF
 *
 * Register: \a PTP_3:IP_1588_ACC_CFG:IGR_STATUS_CALIBA
 */
#define VTSS_PTP_3_IP_1588_ACC_CFG_IGR_STATUS_CALIBA  VTSS_IOREG(0x1e, 1, 0x927e)


/** 
 * \brief Calibration delay chain B status value for ingress SOF
 *
 * \details
 * Calibration delay chain B status value for ingress SOF
 *
 * Register: \a PTP_3:IP_1588_ACC_CFG:IGR_STATUS_CALIBB
 */
#define VTSS_PTP_3_IP_1588_ACC_CFG_IGR_STATUS_CALIBB  VTSS_IOREG(0x1e, 1, 0x927f)


/** 
 * \brief Calibration delay chain A status value for pps_ri 
 *
 * \details
 * Calibration delay chain A status value for pps_ri 
 *
 * Register: \a PTP_3:IP_1588_ACC_CFG:PPS_RI_STATUS_CALIBA
 */
#define VTSS_PTP_3_IP_1588_ACC_CFG_PPS_RI_STATUS_CALIBA  VTSS_IOREG(0x1e, 1, 0x9280)


/** 
 * \brief Calibration delay chain B status value for pps_ri 
 *
 * \details
 * Calibration delay chain B status value for pps_ri 
 *
 * Register: \a PTP_3:IP_1588_ACC_CFG:PPS_RI_STATUS_CALIBB
 */
#define VTSS_PTP_3_IP_1588_ACC_CFG_PPS_RI_STATUS_CALIBB  VTSS_IOREG(0x1e, 1, 0x9281)


/** 
 * \brief Calibration delay chain A status value for PPS_O 
 *
 * \details
 * Calibration delay chain A status value for PPS_O 
 *
 * Register: \a PTP_3:IP_1588_ACC_CFG:PPS_O_STATUS_CALIBA
 */
#define VTSS_PTP_3_IP_1588_ACC_CFG_PPS_O_STATUS_CALIBA  VTSS_IOREG(0x1e, 1, 0x9282)


/** 
 * \brief Calibration delay chain B status value for PPS_O
 *
 * \details
 * Calibration delay chain B status value for PPS_O
 *
 * Register: \a PTP_3:IP_1588_ACC_CFG:PPS_O_STATUS_CALIBB
 */
#define VTSS_PTP_3_IP_1588_ACC_CFG_PPS_O_STATUS_CALIBB  VTSS_IOREG(0x1e, 1, 0x9283)

/**
 * Register Group: \a PTP_3:EGR_STALL_PREDICTOR
 *
 * Egress stall (Tx) registers
 */


/** 
 * \brief Egress stall configuration register
 *
 * \details
 * Register: \a PTP_3:EGR_STALL_PREDICTOR:EG_STALL_CFG
 */
#define VTSS_PTP_3_EGR_STALL_PREDICTOR_EG_STALL_CFG  VTSS_IOREG(0x1e, 1, 0x9284)

/** 
 * \brief
 * WIS advanced (fixed) value
 * This is the number of register stages in the pipeline from the SFD
 * detection logic through the 1588 IP, the PCS stages until the Tx gearbox
 * and after the gearbox through the WIS until the frame overhead
 * generation logic. This will be when three PTP frames in between SFD
 * detection logic and Tx gearbox.
 *
 * \details 
 * Field: VTSS_PTP_3_EGR_STALL_PREDICTOR_EG_STALL_CFG . WAF3
 */
#define  VTSS_F_PTP_3_EGR_STALL_PREDICTOR_EG_STALL_CFG_WAF3(x)  VTSS_ENCODE_BITFIELD(x,24,6)
#define  VTSS_M_PTP_3_EGR_STALL_PREDICTOR_EG_STALL_CFG_WAF3     VTSS_ENCODE_BITMASK(24,6)
#define  VTSS_X_PTP_3_EGR_STALL_PREDICTOR_EG_STALL_CFG_WAF3(x)  VTSS_EXTRACT_BITFIELD(x,24,6)

/** 
 * \brief
 * PCS advanced (fixed) value
 * This is the number of register stages in the pipeline from the SFD
 * detection logic through the 1588 IP, the PCS stages until the Tx gearbox
 * when three PTP frames are in between 1588 IP and SFD detection logic.
 *
 * \details 
 * Field: VTSS_PTP_3_EGR_STALL_PREDICTOR_EG_STALL_CFG . PAF3
 */
#define  VTSS_F_PTP_3_EGR_STALL_PREDICTOR_EG_STALL_CFG_PAF3(x)  VTSS_ENCODE_BITFIELD(x,16,6)
#define  VTSS_M_PTP_3_EGR_STALL_PREDICTOR_EG_STALL_CFG_PAF3     VTSS_ENCODE_BITMASK(16,6)
#define  VTSS_X_PTP_3_EGR_STALL_PREDICTOR_EG_STALL_CFG_PAF3(x)  VTSS_EXTRACT_BITFIELD(x,16,6)

/** 
 * \brief
 * WIS advanced (fixed) value
 * This is the number of register stages in the pipeline from the SFD
 * detection logic through the 1588 IP, the PCS stages until the Tx gearbox
 * and after the gearbox through the WIS until the frame overhead
 * generation logic. This will be when two PTP frames in between SFD
 * detection logic and Tx gearbox.
 *
 * \details 
 * Field: VTSS_PTP_3_EGR_STALL_PREDICTOR_EG_STALL_CFG . WAF2
 */
#define  VTSS_F_PTP_3_EGR_STALL_PREDICTOR_EG_STALL_CFG_WAF2(x)  VTSS_ENCODE_BITFIELD(x,8,6)
#define  VTSS_M_PTP_3_EGR_STALL_PREDICTOR_EG_STALL_CFG_WAF2     VTSS_ENCODE_BITMASK(8,6)
#define  VTSS_X_PTP_3_EGR_STALL_PREDICTOR_EG_STALL_CFG_WAF2(x)  VTSS_EXTRACT_BITFIELD(x,8,6)

/** 
 * \brief
 * PCS advanced (fixed) value
 * This is the number of register stages in the pipeline from the SFD
 * detection logic through the 1588 IP, the PCS stages until the Tx gearbox
 * when two PTP frames are in between 1588 IP and SFD detection logic.
 *
 * \details 
 * Field: VTSS_PTP_3_EGR_STALL_PREDICTOR_EG_STALL_CFG . PAF2
 */
#define  VTSS_F_PTP_3_EGR_STALL_PREDICTOR_EG_STALL_CFG_PAF2(x)  VTSS_ENCODE_BITFIELD(x,0,6)
#define  VTSS_M_PTP_3_EGR_STALL_PREDICTOR_EG_STALL_CFG_PAF2     VTSS_ENCODE_BITMASK(0,6)
#define  VTSS_X_PTP_3_EGR_STALL_PREDICTOR_EG_STALL_CFG_PAF2(x)  VTSS_EXTRACT_BITFIELD(x,0,6)

/**
 * Register Group: \a PTP_3:LTC_INTR
 *
 * LTC Interrupts
 */


/** 
 * \brief LTC_INTR
 *
 * \details
 * LTC interrupts
 *
 * Register: \a PTP_3:LTC_INTR:LTC_INTR
 */
#define VTSS_PTP_3_LTC_INTR_LTC_INTR         VTSS_IOREG(0x1e, 1, 0x9285)

/** 
 * \brief
 * When '1' indicates that a new PPS signal is pushed onto external PPS
 * pin. This interrupt is not set when PPS is configured for loopbacks,
 * serial Tod or in debug mode.
 *
 * \details 
 * Field: VTSS_PTP_3_LTC_INTR_LTC_INTR . PPS_INTR
 */
#define  VTSS_F_PTP_3_LTC_INTR_LTC_INTR_PPS_INTR  VTSS_BIT(1)

/** 
 * \brief
 * When '1' indicates that a new LTC value is either loaded in to HW or
 * saved into registers. Clear by writing a '1'.
 *
 * \details 
 * Field: VTSS_PTP_3_LTC_INTR_LTC_INTR . LTC_LOAD_SAVE_INTR
 */
#define  VTSS_F_PTP_3_LTC_INTR_LTC_INTR_LTC_LOAD_SAVE_INTR  VTSS_BIT(0)


/** 
 * \brief LTC_MASK
 *
 * \details
 * LTC interrupts
 *
 * Register: \a PTP_3:LTC_INTR:LTC_MASK
 */
#define VTSS_PTP_3_LTC_INTR_LTC_MASK         VTSS_IOREG(0x1e, 1, 0x9286)

/** 
 * \brief
 * Mask for PPS_INTR.
 *
 * \details 
 * Field: VTSS_PTP_3_LTC_INTR_LTC_MASK . PPS_MASK
 */
#define  VTSS_F_PTP_3_LTC_INTR_LTC_MASK_PPS_MASK  VTSS_BIT(1)

/** 
 * \brief
 * Mask for LTC_LOAD_SAVE_INTR.
 *
 * \details 
 * Field: VTSS_PTP_3_LTC_INTR_LTC_MASK . LTC_LOAD_SAVE_MASK
 */
#define  VTSS_F_PTP_3_LTC_INTR_LTC_MASK_LTC_LOAD_SAVE_MASK  VTSS_BIT(0)

/**
 * Register Group: \a PTP_3:INGR_IP_1588_DEBUG_REGISTERS
 *
 * Provides the ability to read the IP1588 internal registers
 */


/** 
 * \brief Part1 of command data generated by analyzer
 *
 * \details
 * This will provide the ability to check the command generated by analyzer
 * to the rewriter
 *
 * Register: \a PTP_3:INGR_IP_1588_DEBUG_REGISTERS:INGR_ANA_CMD_FIFO_DATA1
 */
#define VTSS_PTP_3_INGR_IP_1588_DEBUG_REGISTERS_INGR_ANA_CMD_FIFO_DATA1  VTSS_IOREG(0x1e, 1, 0x9287)


/** 
 * \brief Part2 of command data generated by analyzer
 *
 * \details
 * This will provide the ability to check the command generated by analyzer
 * to the rewriter
 *
 * Register: \a PTP_3:INGR_IP_1588_DEBUG_REGISTERS:INGR_ANA_CMD_FIFO_DATA2
 */
#define VTSS_PTP_3_INGR_IP_1588_DEBUG_REGISTERS_INGR_ANA_CMD_FIFO_DATA2  VTSS_IOREG(0x1e, 1, 0x9288)


/** 
 * \brief Status signals of Analyzer
 *
 * \details
 * Register: \a PTP_3:INGR_IP_1588_DEBUG_REGISTERS:INGR_ANALYZER_STATUS
 */
#define VTSS_PTP_3_INGR_IP_1588_DEBUG_REGISTERS_INGR_ANALYZER_STATUS  VTSS_IOREG(0x1e, 1, 0x9289)


/** 
 * \brief Predictor variable delay1
 *
 * \details
 * Predictor variable stall latency 1.
 *
 * Register: \a PTP_3:INGR_IP_1588_DEBUG_REGISTERS:INGR_PREDICTOR_VAR_DELAY1
 */
#define VTSS_PTP_3_INGR_IP_1588_DEBUG_REGISTERS_INGR_PREDICTOR_VAR_DELAY1  VTSS_IOREG(0x1e, 1, 0x928a)


/** 
 * \brief Predictor variable delay 2
 *
 * \details
 * Predictor variable stall latency 2
 *
 * Register: \a PTP_3:INGR_IP_1588_DEBUG_REGISTERS:INGR_PREDICTOR_VAR_DELAY2
 */
#define VTSS_PTP_3_INGR_IP_1588_DEBUG_REGISTERS_INGR_PREDICTOR_VAR_DELAY2  VTSS_IOREG(0x1e, 1, 0x928b)


/** 
 * \brief Predictor variable delay 3
 *
 * \details
 * Predictor variable stall latency 3
 *
 * Register: \a PTP_3:INGR_IP_1588_DEBUG_REGISTERS:INGR_PREDICTOR_VAR_DELAY3
 */
#define VTSS_PTP_3_INGR_IP_1588_DEBUG_REGISTERS_INGR_PREDICTOR_VAR_DELAY3  VTSS_IOREG(0x1e, 1, 0x928c)


/** 
 * \brief Gives rewriter modifications on packet 
 *
 * \details
 * Register: \a PTP_3:INGR_IP_1588_DEBUG_REGISTERS:INGR_REWRITER_OPMODE
 */
#define VTSS_PTP_3_INGR_IP_1588_DEBUG_REGISTERS_INGR_REWRITER_OPMODE  VTSS_IOREG(0x1e, 1, 0x928d)


/** 
 * \brief Indicates valid cf if asserted.
 *
 * \details
 * 
 * Indicates valid cf if asserted.

 *
 * Register: \a PTP_3:INGR_IP_1588_DEBUG_REGISTERS:INGR_CORRECTION_FIELD_VLD
 */
#define VTSS_PTP_3_INGR_IP_1588_DEBUG_REGISTERS_INGR_CORRECTION_FIELD_VLD  VTSS_IOREG(0x1e, 1, 0x928e)


/** 
 * \brief correction field value lsb [31:0]  of new_cf[63:0] from rewriter fifo.
 *
 * \details
 * correction field value lsb [31:0]  of new_cf[63:0] from rewriter fifo.
 *
 * Register: \a PTP_3:INGR_IP_1588_DEBUG_REGISTERS:INGR_CORRECTION_FIELD_VALUE1
 */
#define VTSS_PTP_3_INGR_IP_1588_DEBUG_REGISTERS_INGR_CORRECTION_FIELD_VALUE1  VTSS_IOREG(0x1e, 1, 0x928f)


/** 
 * \brief correction field value lsb [63:32]  of new_cf[63:0] from rewriter fifo
 *
 * \details
 * correction field value lsb [63:32]  of new_cf[63:0] from rewriter fifo
 *
 * Register: \a PTP_3:INGR_IP_1588_DEBUG_REGISTERS:INGR_CORRECTION_FIELD_VALUE2
 */
#define VTSS_PTP_3_INGR_IP_1588_DEBUG_REGISTERS_INGR_CORRECTION_FIELD_VALUE2  VTSS_IOREG(0x1e, 1, 0x9290)


/** 
 * \brief lsb (31:0] of new_field_o[79:0] from tsp.v
 *
 * \details
 * lsb (31:0] of new_field_o[79:0] from tsp.v
 *
 * Register: \a PTP_3:INGR_IP_1588_DEBUG_REGISTERS:INGR_TIMESTAMP_VALUE1
 */
#define VTSS_PTP_3_INGR_IP_1588_DEBUG_REGISTERS_INGR_TIMESTAMP_VALUE1  VTSS_IOREG(0x1e, 1, 0x9291)


/** 
 * \brief lsb (63:32] of new_field_o[79:0] from rewriter fifo
 *
 * \details
 * lsb (63:32] of new_field_o[79:0] from rewriter fifo
 *
 * Register: \a PTP_3:INGR_IP_1588_DEBUG_REGISTERS:INGR_TIMESTAMP_VALUE2
 */
#define VTSS_PTP_3_INGR_IP_1588_DEBUG_REGISTERS_INGR_TIMESTAMP_VALUE2  VTSS_IOREG(0x1e, 1, 0x9292)


/** 
 * \brief Msb (79:64] of new_field_o[79:0] from rewriter fifo
 *
 * \details
 * Msb (79:64] of new_field_o[79:0] from rewriter fifo
 *
 * Register: \a PTP_3:INGR_IP_1588_DEBUG_REGISTERS:INGR_TIMESTAMP_VALUE3
 */
#define VTSS_PTP_3_INGR_IP_1588_DEBUG_REGISTERS_INGR_TIMESTAMP_VALUE3  VTSS_IOREG(0x1e, 1, 0x9293)


/** 
 * \brief Configure LTC read type
 *
 * \details
 * Register: \a PTP_3:INGR_IP_1588_DEBUG_REGISTERS:INGR_LTC_READ
 */
#define VTSS_PTP_3_INGR_IP_1588_DEBUG_REGISTERS_INGR_LTC_READ  VTSS_IOREG(0x1e, 1, 0x9294)


/** 
 * \brief LTC seconds (high)
 *
 * \details
 * LTC seconds (high)
 *
 * Register: \a PTP_3:INGR_IP_1588_DEBUG_REGISTERS:INGR_LTC_SEC_H
 */
#define VTSS_PTP_3_INGR_IP_1588_DEBUG_REGISTERS_INGR_LTC_SEC_H  VTSS_IOREG(0x1e, 1, 0x9295)


/** 
 * \brief LTC seconds (low)
 *
 * \details
 * LTC seconds (low)
 *
 * Register: \a PTP_3:INGR_IP_1588_DEBUG_REGISTERS:INGR_LTC_SEC_L
 */
#define VTSS_PTP_3_INGR_IP_1588_DEBUG_REGISTERS_INGR_LTC_SEC_L  VTSS_IOREG(0x1e, 1, 0x9296)


/** 
 * \brief LTC nanoseconds
 *
 * \details
 * LTC nanoseconds
 *
 * Register: \a PTP_3:INGR_IP_1588_DEBUG_REGISTERS:INGR_LTC_NS
 */
#define VTSS_PTP_3_INGR_IP_1588_DEBUG_REGISTERS_INGR_LTC_NS  VTSS_IOREG(0x1e, 1, 0x9297)


/** 
 * \brief Extracted correction field LSB
 *
 * \details
 * Register: \a PTP_3:INGR_IP_1588_DEBUG_REGISTERS:INGR_EXTRACTED_CF1
 */
#define VTSS_PTP_3_INGR_IP_1588_DEBUG_REGISTERS_INGR_EXTRACTED_CF1  VTSS_IOREG(0x1e, 1, 0x9298)


/** 
 * \brief Extracted correction field MSB
 *
 * \details
 * Register: \a PTP_3:INGR_IP_1588_DEBUG_REGISTERS:INGR_EXTRACTED_CF2
 */
#define VTSS_PTP_3_INGR_IP_1588_DEBUG_REGISTERS_INGR_EXTRACTED_CF2  VTSS_IOREG(0x1e, 1, 0x9299)


/** 
 * \brief Stored Timestamp
 *
 * \details
 * Register: \a PTP_3:INGR_IP_1588_DEBUG_REGISTERS:INGR_STORED_TIMESTAMP
 */
#define VTSS_PTP_3_INGR_IP_1588_DEBUG_REGISTERS_INGR_STORED_TIMESTAMP  VTSS_IOREG(0x1e, 1, 0x929a)


/** 
 * \brief Active time stamp LSB
 *
 * \details
 * Register: \a PTP_3:INGR_IP_1588_DEBUG_REGISTERS:INGR_ACTIVE_TIMESTAMP1
 */
#define VTSS_PTP_3_INGR_IP_1588_DEBUG_REGISTERS_INGR_ACTIVE_TIMESTAMP1  VTSS_IOREG(0x1e, 1, 0x929b)


/** 
 * \brief Active time stamp MSB
 *
 * \details
 * Register: \a PTP_3:INGR_IP_1588_DEBUG_REGISTERS:INGR_ACTIVE_TIMESTAMP2
 */
#define VTSS_PTP_3_INGR_IP_1588_DEBUG_REGISTERS_INGR_ACTIVE_TIMESTAMP2  VTSS_IOREG(0x1e, 1, 0x929c)


/** 
 * \brief Predictor delay used and the number of PTP frames matched
 *
 * \details
 * Register: \a PTP_3:INGR_IP_1588_DEBUG_REGISTERS:INGR_PREDICTOR_VALUES
 */
#define VTSS_PTP_3_INGR_IP_1588_DEBUG_REGISTERS_INGR_PREDICTOR_VALUES  VTSS_IOREG(0x1e, 1, 0x929d)


/** 
 * \brief Current and Next States of FSMs in rw_cmd_write[0:3], rw_cmd_cfwrite[7:4] and rw_cmd_clear[11:8]
 *
 * \details
 * Register: \a PTP_3:INGR_IP_1588_DEBUG_REGISTERS:INGR_REWRITER_FSM_STATES
 */
#define VTSS_PTP_3_INGR_IP_1588_DEBUG_REGISTERS_INGR_REWRITER_FSM_STATES  VTSS_IOREG(0x1e, 1, 0x929e)


/** 
 * \brief Software pop fifo
 *
 * \details
 * Software pop fifo
 *
 * Register: \a PTP_3:INGR_IP_1588_DEBUG_REGISTERS:INGR_SW_POP_FIFO
 */
#define VTSS_PTP_3_INGR_IP_1588_DEBUG_REGISTERS_INGR_SW_POP_FIFO  VTSS_IOREG(0x1e, 1, 0x929f)

/** 
 * \brief
 * Reset all the fifos. This bit will be cleared after the reset operation
 * is completed. This should only be used if an error pushes the fifo to
 * out of sync.
 *
 * \details 
 * Field: VTSS_PTP_3_INGR_IP_1588_DEBUG_REGISTERS_INGR_SW_POP_FIFO . INGR_SW_POP_FIFO
 */
#define  VTSS_F_PTP_3_INGR_IP_1588_DEBUG_REGISTERS_INGR_SW_POP_FIFO_INGR_SW_POP_FIFO  VTSS_BIT(0)


/** 
 * \brief lsb (31:0] of ANA_CMD_OUT_S from Rewriter
 *
 * \details
 * Register: \a PTP_3:INGR_IP_1588_DEBUG_REGISTERS:INGR_ANA_CMD_FIFO_DATA_OUT1
 */
#define VTSS_PTP_3_INGR_IP_1588_DEBUG_REGISTERS_INGR_ANA_CMD_FIFO_DATA_OUT1  VTSS_IOREG(0x1e, 1, 0x92a0)


/** 
 * \brief msb [42:32] of ANA_CMD_OUT_S from Rewriter
 *
 * \details
 * Register: \a PTP_3:INGR_IP_1588_DEBUG_REGISTERS:INGR_ANA_CMD_FIFO_DATA_OUT2
 */
#define VTSS_PTP_3_INGR_IP_1588_DEBUG_REGISTERS_INGR_ANA_CMD_FIFO_DATA_OUT2  VTSS_IOREG(0x1e, 1, 0x92a1)


/** 
 * \brief lsb (31:0] of new_field_i[79:0] from tsp.v to Rewriter
 *
 * \details
 * Register: \a PTP_3:INGR_IP_1588_DEBUG_REGISTERS:INGR_TIMESTAMP_VALUE_FIFO_INPUT1
 */
#define VTSS_PTP_3_INGR_IP_1588_DEBUG_REGISTERS_INGR_TIMESTAMP_VALUE_FIFO_INPUT1  VTSS_IOREG(0x1e, 1, 0x92a2)


/** 
 * \brief lsb (63:32] of new_field_i[79:0] from tsp.v to Rewriter
 *
 * \details
 * lsb (63:32] of new_field_i[79:0] from tsp.v to Rewriter
 *
 * Register: \a PTP_3:INGR_IP_1588_DEBUG_REGISTERS:INGR_TIMESTAMP_VALUE_FIFO_INPUT2
 */
#define VTSS_PTP_3_INGR_IP_1588_DEBUG_REGISTERS_INGR_TIMESTAMP_VALUE_FIFO_INPUT2  VTSS_IOREG(0x1e, 1, 0x92a3)


/** 
 * \brief lsb (79:63] of new_field_i[79:0] from tsp.v to Rewriter
 *
 * \details
 * lsb (79:63] of new_field_i[79:0] from tsp.v to Rewriter
 *
 * Register: \a PTP_3:INGR_IP_1588_DEBUG_REGISTERS:INGR_TIMESTAMP_VALUE_FIFO_INPUT3
 */
#define VTSS_PTP_3_INGR_IP_1588_DEBUG_REGISTERS_INGR_TIMESTAMP_VALUE_FIFO_INPUT3  VTSS_IOREG(0x1e, 1, 0x92a4)


/** 
 * \brief correction field valid of new_cf_valid from tsp to rewriter.
 *
 * \details
 * correction field valid of new_cf_valid from tsp to rewriter.
 *
 * Register: \a PTP_3:INGR_IP_1588_DEBUG_REGISTERS:INGR_CORRECTION_FIELD_VLD_FIFO_INPUT
 */
#define VTSS_PTP_3_INGR_IP_1588_DEBUG_REGISTERS_INGR_CORRECTION_FIELD_VLD_FIFO_INPUT  VTSS_IOREG(0x1e, 1, 0x92a5)


/** 
 * \brief correction field value lsb [31:0]  of new_cf[63:0] from tsp to rewriter
 *
 * \details
 * correction field value lsb [31:0]  of new_cf[63:0] from tsp to rewriter
 *
 * Register: \a PTP_3:INGR_IP_1588_DEBUG_REGISTERS:INGR_CORRECTION_FIELD_VALUE_FIFO_INPUT1
 */
#define VTSS_PTP_3_INGR_IP_1588_DEBUG_REGISTERS_INGR_CORRECTION_FIELD_VALUE_FIFO_INPUT1  VTSS_IOREG(0x1e, 1, 0x92a6)


/** 
 * \brief correction field value lsb [63:32]  of new_cf[63:0] from tsp to rewriter
 *
 * \details
 * correction field value lsb [63:32]  of new_cf[63:0] from tsp to rewriter
 *
 * Register: \a PTP_3:INGR_IP_1588_DEBUG_REGISTERS:INGR_CORRECTION_FIELD_VALUE_FIFO_INPUT2
 */
#define VTSS_PTP_3_INGR_IP_1588_DEBUG_REGISTERS_INGR_CORRECTION_FIELD_VALUE_FIFO_INPUT2  VTSS_IOREG(0x1e, 1, 0x92a7)

/**
 * Register Group: \a PTP_3:EGR_IP_1588_DEBUG_REGISTERS
 *
 * Provides the ability to read the IP1588 internal registers
 */


/** 
 * \brief Part1 of command data generated by analyzer
 *
 * \details
 * This will provide the ability to check the command generated by analyzer
 * to the rewriter
 *
 * Register: \a PTP_3:EGR_IP_1588_DEBUG_REGISTERS:EGR_ANA_CMD_FIFO_DATA1
 */
#define VTSS_PTP_3_EGR_IP_1588_DEBUG_REGISTERS_EGR_ANA_CMD_FIFO_DATA1  VTSS_IOREG(0x1e, 1, 0x92a8)


/** 
 * \brief Part2 of command data generated by analyzer
 *
 * \details
 * This will provide the ability to check the command generated by analyzer
 * to the rewriter
 *
 * Register: \a PTP_3:EGR_IP_1588_DEBUG_REGISTERS:EGR_ANA_CMD_FIFO_DATA2
 */
#define VTSS_PTP_3_EGR_IP_1588_DEBUG_REGISTERS_EGR_ANA_CMD_FIFO_DATA2  VTSS_IOREG(0x1e, 1, 0x92a9)


/** 
 * \brief Status signals of Analyzer
 *
 * \details
 * Register: \a PTP_3:EGR_IP_1588_DEBUG_REGISTERS:EGR_ANALYZER_STATUS
 */
#define VTSS_PTP_3_EGR_IP_1588_DEBUG_REGISTERS_EGR_ANALYZER_STATUS  VTSS_IOREG(0x1e, 1, 0x92aa)


/** 
 * \brief Predictor variable delay1
 *
 * \details
 * Predictor variable stall latency 1.
 *
 * Register: \a PTP_3:EGR_IP_1588_DEBUG_REGISTERS:EGR_PREDICTOR_VAR_DELAY1
 */
#define VTSS_PTP_3_EGR_IP_1588_DEBUG_REGISTERS_EGR_PREDICTOR_VAR_DELAY1  VTSS_IOREG(0x1e, 1, 0x92ab)


/** 
 * \brief Predictor variable delay 2
 *
 * \details
 * Predictor variable stall latency 2
 *
 * Register: \a PTP_3:EGR_IP_1588_DEBUG_REGISTERS:EGR_PREDICTOR_VAR_DELAY2
 */
#define VTSS_PTP_3_EGR_IP_1588_DEBUG_REGISTERS_EGR_PREDICTOR_VAR_DELAY2  VTSS_IOREG(0x1e, 1, 0x92ac)


/** 
 * \brief Predictor variable delay 3
 *
 * \details
 * Predictor variable stall latency 3
 *
 * Register: \a PTP_3:EGR_IP_1588_DEBUG_REGISTERS:EGR_PREDICTOR_VAR_DELAY3
 */
#define VTSS_PTP_3_EGR_IP_1588_DEBUG_REGISTERS_EGR_PREDICTOR_VAR_DELAY3  VTSS_IOREG(0x1e, 1, 0x92ad)


/** 
 * \brief Gives rewriter modifications on packet 
 *
 * \details
 * Register: \a PTP_3:EGR_IP_1588_DEBUG_REGISTERS:EGR_REWRITER_OPMODE
 */
#define VTSS_PTP_3_EGR_IP_1588_DEBUG_REGISTERS_EGR_REWRITER_OPMODE  VTSS_IOREG(0x1e, 1, 0x92ae)


/** 
 * \brief Indicates valid cf if asserted.
 *
 * \details
 * 
 * Indicates valid cf if asserted.

 *
 * Register: \a PTP_3:EGR_IP_1588_DEBUG_REGISTERS:EGR_CORRECTION_FIELD_VLD
 */
#define VTSS_PTP_3_EGR_IP_1588_DEBUG_REGISTERS_EGR_CORRECTION_FIELD_VLD  VTSS_IOREG(0x1e, 1, 0x92af)


/** 
 * \brief correction field value lsb [31:0]  of new_cf[63:0] from rewriter fifo.
 *
 * \details
 * correction field value lsb [31:0]  of new_cf[63:0] from rewriter fifo.
 *
 * Register: \a PTP_3:EGR_IP_1588_DEBUG_REGISTERS:EGR_CORRECTION_FIELD_VALUE1
 */
#define VTSS_PTP_3_EGR_IP_1588_DEBUG_REGISTERS_EGR_CORRECTION_FIELD_VALUE1  VTSS_IOREG(0x1e, 1, 0x92b0)


/** 
 * \brief correction field value lsb [63:32]  of new_cf[63:0] from rewriter fifo
 *
 * \details
 * correction field value lsb [63:32]  of new_cf[63:0] from rewriter fifo
 *
 * Register: \a PTP_3:EGR_IP_1588_DEBUG_REGISTERS:EGR_CORRECTION_FIELD_VALUE2
 */
#define VTSS_PTP_3_EGR_IP_1588_DEBUG_REGISTERS_EGR_CORRECTION_FIELD_VALUE2  VTSS_IOREG(0x1e, 1, 0x92b1)


/** 
 * \brief lsb (31:0] of new_field_o[79:0] from tsp.v
 *
 * \details
 * lsb (31:0] of new_field_o[79:0] from tsp.v
 *
 * Register: \a PTP_3:EGR_IP_1588_DEBUG_REGISTERS:EGR_TIMESTAMP_VALUE1
 */
#define VTSS_PTP_3_EGR_IP_1588_DEBUG_REGISTERS_EGR_TIMESTAMP_VALUE1  VTSS_IOREG(0x1e, 1, 0x92b2)


/** 
 * \brief lsb (63:32] of new_field_o[79:0] from rewriter fifo
 *
 * \details
 * lsb (63:32] of new_field_o[79:0] from rewriter fifo
 *
 * Register: \a PTP_3:EGR_IP_1588_DEBUG_REGISTERS:EGR_TIMESTAMP_VALUE2
 */
#define VTSS_PTP_3_EGR_IP_1588_DEBUG_REGISTERS_EGR_TIMESTAMP_VALUE2  VTSS_IOREG(0x1e, 1, 0x92b3)


/** 
 * \brief Msb (79:64] of new_field_o[79:0] from rewriter fifo
 *
 * \details
 * Msb (79:64] of new_field_o[79:0] from rewriter fifo
 *
 * Register: \a PTP_3:EGR_IP_1588_DEBUG_REGISTERS:EGR_TIMESTAMP_VALUE3
 */
#define VTSS_PTP_3_EGR_IP_1588_DEBUG_REGISTERS_EGR_TIMESTAMP_VALUE3  VTSS_IOREG(0x1e, 1, 0x92b4)


/** 
 * \brief Configure LTC read type
 *
 * \details
 * Register: \a PTP_3:EGR_IP_1588_DEBUG_REGISTERS:EGR_LTC_READ
 */
#define VTSS_PTP_3_EGR_IP_1588_DEBUG_REGISTERS_EGR_LTC_READ  VTSS_IOREG(0x1e, 1, 0x92b5)


/** 
 * \brief LTC seconds (high)
 *
 * \details
 * LTC seconds (high)
 *
 * Register: \a PTP_3:EGR_IP_1588_DEBUG_REGISTERS:EGR_LTC_SEC_H
 */
#define VTSS_PTP_3_EGR_IP_1588_DEBUG_REGISTERS_EGR_LTC_SEC_H  VTSS_IOREG(0x1e, 1, 0x92b6)


/** 
 * \brief LTC seconds (low)
 *
 * \details
 * LTC seconds (low)
 *
 * Register: \a PTP_3:EGR_IP_1588_DEBUG_REGISTERS:EGR_LTC_SEC_L
 */
#define VTSS_PTP_3_EGR_IP_1588_DEBUG_REGISTERS_EGR_LTC_SEC_L  VTSS_IOREG(0x1e, 1, 0x92b7)


/** 
 * \brief LTC nanoseconds
 *
 * \details
 * LTC nanoseconds
 *
 * Register: \a PTP_3:EGR_IP_1588_DEBUG_REGISTERS:EGR_LTC_NS
 */
#define VTSS_PTP_3_EGR_IP_1588_DEBUG_REGISTERS_EGR_LTC_NS  VTSS_IOREG(0x1e, 1, 0x92b8)


/** 
 * \brief Extracted correction field LSB
 *
 * \details
 * Register: \a PTP_3:EGR_IP_1588_DEBUG_REGISTERS:EGR_EXTRACTED_CF1
 */
#define VTSS_PTP_3_EGR_IP_1588_DEBUG_REGISTERS_EGR_EXTRACTED_CF1  VTSS_IOREG(0x1e, 1, 0x92b9)


/** 
 * \brief Extracted correction field MSB
 *
 * \details
 * Register: \a PTP_3:EGR_IP_1588_DEBUG_REGISTERS:EGR_EXTRACTED_CF2
 */
#define VTSS_PTP_3_EGR_IP_1588_DEBUG_REGISTERS_EGR_EXTRACTED_CF2  VTSS_IOREG(0x1e, 1, 0x92ba)


/** 
 * \brief Stored Timestamp
 *
 * \details
 * Register: \a PTP_3:EGR_IP_1588_DEBUG_REGISTERS:EGR_STORED_TIMESTAMP
 */
#define VTSS_PTP_3_EGR_IP_1588_DEBUG_REGISTERS_EGR_STORED_TIMESTAMP  VTSS_IOREG(0x1e, 1, 0x92bb)


/** 
 * \brief Active time stamp LSB
 *
 * \details
 * Register: \a PTP_3:EGR_IP_1588_DEBUG_REGISTERS:EGR_ACTIVE_TIMESTAMP1
 */
#define VTSS_PTP_3_EGR_IP_1588_DEBUG_REGISTERS_EGR_ACTIVE_TIMESTAMP1  VTSS_IOREG(0x1e, 1, 0x92bc)


/** 
 * \brief Active time stamp MSB
 *
 * \details
 * Register: \a PTP_3:EGR_IP_1588_DEBUG_REGISTERS:EGR_ACTIVE_TIMESTAMP2
 */
#define VTSS_PTP_3_EGR_IP_1588_DEBUG_REGISTERS_EGR_ACTIVE_TIMESTAMP2  VTSS_IOREG(0x1e, 1, 0x92bd)


/** 
 * \brief Predictor delay used and the number of PTP frames matched
 *
 * \details
 * Register: \a PTP_3:EGR_IP_1588_DEBUG_REGISTERS:EGR_PREDICTOR_VALUES
 */
#define VTSS_PTP_3_EGR_IP_1588_DEBUG_REGISTERS_EGR_PREDICTOR_VALUES  VTSS_IOREG(0x1e, 1, 0x92be)


/** 
 * \brief Current and Next States of FSMs in rw_cmd_write[0:3], rw_cmd_cfwrite[7:4] and rw_cmd_clear[11:8]
 *
 * \details
 * Register: \a PTP_3:EGR_IP_1588_DEBUG_REGISTERS:EGR_REWRITER_FSM_STATES
 */
#define VTSS_PTP_3_EGR_IP_1588_DEBUG_REGISTERS_EGR_REWRITER_FSM_STATES  VTSS_IOREG(0x1e, 1, 0x92bf)


/** 
 * \brief Software pop fifo
 *
 * \details
 * Software pop fifo
 *
 * Register: \a PTP_3:EGR_IP_1588_DEBUG_REGISTERS:EGR_SW_POP_FIFO
 */
#define VTSS_PTP_3_EGR_IP_1588_DEBUG_REGISTERS_EGR_SW_POP_FIFO  VTSS_IOREG(0x1e, 1, 0x92c0)

/** 
 * \brief
 * Reset all the fifos. This bit will be cleared after the reset operation
 * is completed. This should only be used if an error pushes the fifo to
 * out of sync.
 *
 * \details 
 * Field: VTSS_PTP_3_EGR_IP_1588_DEBUG_REGISTERS_EGR_SW_POP_FIFO . EGR_SW_POP_FIFO
 */
#define  VTSS_F_PTP_3_EGR_IP_1588_DEBUG_REGISTERS_EGR_SW_POP_FIFO_EGR_SW_POP_FIFO  VTSS_BIT(0)


/** 
 * \brief lsb (31:0] of ANA_CMD_OUT_S from Rewriter
 *
 * \details
 * Register: \a PTP_3:EGR_IP_1588_DEBUG_REGISTERS:EGR_ANA_CMD_FIFO_DATA_OUT1
 */
#define VTSS_PTP_3_EGR_IP_1588_DEBUG_REGISTERS_EGR_ANA_CMD_FIFO_DATA_OUT1  VTSS_IOREG(0x1e, 1, 0x92c1)


/** 
 * \brief msb [42:32] of ANA_CMD_OUT_S from Rewriter
 *
 * \details
 * Register: \a PTP_3:EGR_IP_1588_DEBUG_REGISTERS:EGR_ANA_CMD_FIFO_DATA_OUT2
 */
#define VTSS_PTP_3_EGR_IP_1588_DEBUG_REGISTERS_EGR_ANA_CMD_FIFO_DATA_OUT2  VTSS_IOREG(0x1e, 1, 0x92c2)


/** 
 * \brief lsb (31:0] of new_field_i[79:0] from tsp.v to Rewriter
 *
 * \details
 * Register: \a PTP_3:EGR_IP_1588_DEBUG_REGISTERS:EGR_TIMESTAMP_VALUE_FIFO_INPUT1
 */
#define VTSS_PTP_3_EGR_IP_1588_DEBUG_REGISTERS_EGR_TIMESTAMP_VALUE_FIFO_INPUT1  VTSS_IOREG(0x1e, 1, 0x92c3)


/** 
 * \brief lsb (63:32] of new_field_i[79:0] from tsp.v to Rewriter
 *
 * \details
 * lsb (63:32] of new_field_i[79:0] from tsp.v to Rewriter
 *
 * Register: \a PTP_3:EGR_IP_1588_DEBUG_REGISTERS:EGR_TIMESTAMP_VALUE_FIFO_INPUT2
 */
#define VTSS_PTP_3_EGR_IP_1588_DEBUG_REGISTERS_EGR_TIMESTAMP_VALUE_FIFO_INPUT2  VTSS_IOREG(0x1e, 1, 0x92c4)


/** 
 * \brief lsb (79:63] of new_field_i[79:0] from tsp.v to Rewriter
 *
 * \details
 * lsb (79:63] of new_field_i[79:0] from tsp.v to Rewriter
 *
 * Register: \a PTP_3:EGR_IP_1588_DEBUG_REGISTERS:EGR_TIMESTAMP_VALUE_FIFO_INPUT3
 */
#define VTSS_PTP_3_EGR_IP_1588_DEBUG_REGISTERS_EGR_TIMESTAMP_VALUE_FIFO_INPUT3  VTSS_IOREG(0x1e, 1, 0x92c5)


/** 
 * \brief correction field valid of new_cf_valid from tsp to rewriter.
 *
 * \details
 * correction field valid of new_cf_valid from tsp to rewriter.
 *
 * Register: \a PTP_3:EGR_IP_1588_DEBUG_REGISTERS:EGR_CORRECTION_FIELD_VLD_FIFO_INPUT
 */
#define VTSS_PTP_3_EGR_IP_1588_DEBUG_REGISTERS_EGR_CORRECTION_FIELD_VLD_FIFO_INPUT  VTSS_IOREG(0x1e, 1, 0x92c6)


/** 
 * \brief correction field value lsb [31:0]  of new_cf[63:0] from tsp to rewriter
 *
 * \details
 * correction field value lsb [31:0]  of new_cf[63:0] from tsp to rewriter
 *
 * Register: \a PTP_3:EGR_IP_1588_DEBUG_REGISTERS:EGR_CORRECTION_FIELD_VALUE_FIFO_INPUT1
 */
#define VTSS_PTP_3_EGR_IP_1588_DEBUG_REGISTERS_EGR_CORRECTION_FIELD_VALUE_FIFO_INPUT1  VTSS_IOREG(0x1e, 1, 0x92c7)


/** 
 * \brief correction field value lsb [63:32]  of new_cf[63:0] from tsp to rewriter
 *
 * \details
 * correction field value lsb [63:32]  of new_cf[63:0] from tsp to rewriter
 *
 * Register: \a PTP_3:EGR_IP_1588_DEBUG_REGISTERS:EGR_CORRECTION_FIELD_VALUE_FIFO_INPUT2
 */
#define VTSS_PTP_3_EGR_IP_1588_DEBUG_REGISTERS_EGR_CORRECTION_FIELD_VALUE_FIFO_INPUT2  VTSS_IOREG(0x1e, 1, 0x92c8)


#endif /* _VTSS_MALIBU_REGS_PTP_3_H_ */
