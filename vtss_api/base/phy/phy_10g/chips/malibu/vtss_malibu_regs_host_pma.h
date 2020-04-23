#ifndef _VTSS_MALIBU_REGS_HOST_PMA_H_
#define _VTSS_MALIBU_REGS_HOST_PMA_H_

/*


 Copyright (c) 2004-2018 Microsemi Corporation "Microsemi".

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.

*/

#include "vtss_malibu_regs_common.h"

/*********************************************************************** 
 *
 * Target: \a HOST_PMA
 *
 * 
 *
 ***********************************************************************/

/**
 * Register Group: \a HOST_PMA:DEV1_IEEE_PMA_CONTROL
 *
 * Not documented
 */


/** 
 * \brief PMA Control Register 1
 *
 * \details
 * Register: \a HOST_PMA:DEV1_IEEE_PMA_CONTROL:PMA_CONTROL_1
 */
#define VTSS_HOST_PMA_DEV1_IEEE_PMA_CONTROL_PMA_CONTROL_1  VTSS_IOREG(0x09, 0, 0x0000)

/** 
 * \brief
 * MDIO Managable Device (MMD) software reset.	This register resets
 * functions associated exclusively with the host side PMA.  Data path
 * logic and configuration registers are reset. This register is
 * self-clearing.
 *
 * \details 
 * 0: Normal operation
 * 1: Reset
 *
 * Field: VTSS_HOST_PMA_DEV1_IEEE_PMA_CONTROL_PMA_CONTROL_1 . RST
 */
#define  VTSS_F_HOST_PMA_DEV1_IEEE_PMA_CONTROL_PMA_CONTROL_1_RST  VTSS_BIT(15)

/** 
 * \brief
 * Enable PMA Pad Loopback L0
 *
 * \details 
 * 0: Disable
 * 1: Enable
 *
 * Field: VTSS_HOST_PMA_DEV1_IEEE_PMA_CONTROL_PMA_CONTROL_1 . EN_PAD_LOOP
 */
#define  VTSS_F_HOST_PMA_DEV1_IEEE_PMA_CONTROL_PMA_CONTROL_1_EN_PAD_LOOP  VTSS_BIT(0)

/**
 * Register Group: \a HOST_PMA:DEV1_IEEE_PMA_STATUS
 *
 * Not documented
 */


/** 
 * \brief PMA Status Register 1
 *
 * \details
 * Register: \a HOST_PMA:DEV1_IEEE_PMA_STATUS:PMA_STATUS_1
 */
#define VTSS_HOST_PMA_DEV1_IEEE_PMA_STATUS_PMA_STATUS_1  VTSS_IOREG(0x09, 0, 0x0001)

/** 
 * \brief
 * Indicates a fault condition for this interface in either the transmit or
 * the receive paths.

 *
 * \details 
 * 0: Fault condition not detected.  Latch-high alarm status bits
 * TRANSMIT_FAULT=0 AND RECEIVE_FAULT=0.
 * 1: Fault condition detected.  Latch-high alarm status bits
 * TRANSMIT_FAULT=1 OR RECEIVE_FAULT=1.
 *
 * Field: VTSS_HOST_PMA_DEV1_IEEE_PMA_STATUS_PMA_STATUS_1 . FAULT
 */
#define  VTSS_F_HOST_PMA_DEV1_IEEE_PMA_STATUS_PMA_STATUS_1_FAULT  VTSS_BIT(7)

/** 
 * \brief
 * Indicates the receive link status for this interface.  This is a sticky
 * bit that latches the low state. The latch-low bit is cleared when the
 * register is read.
 *
 * \details 
 * 0: PMA/PMD receive link down
 * 1: PMA/PMD receive link up
 *
 * Field: VTSS_HOST_PMA_DEV1_IEEE_PMA_STATUS_PMA_STATUS_1 . RECEIVE_LINK_STATUS
 */
#define  VTSS_F_HOST_PMA_DEV1_IEEE_PMA_STATUS_PMA_STATUS_1_RECEIVE_LINK_STATUS  VTSS_BIT(2)

/**
 * Register Group: \a HOST_PMA:DEV1_IEEE_PMA_PMD_STATUS
 *
 * Not documented
 */


/** 
 * \brief PMA Status 2
 *
 * \details
 * Register: \a HOST_PMA:DEV1_IEEE_PMA_PMD_STATUS:PMA_STATUS_2
 */
#define VTSS_HOST_PMA_DEV1_IEEE_PMA_PMD_STATUS_PMA_STATUS_2  VTSS_IOREG(0x09, 0, 0x0008)

/** 
 * \brief
 * Indicates a fault condition on this interface's transmit path.  This is
 * a sticky bit that latches the high state.  The latch-high bit is cleared
 * when the register is read.
 *
 * \details 
 * 0: No fault condition on transmit path
 * 1: Fault condition on transmit path
 *
 * Field: VTSS_HOST_PMA_DEV1_IEEE_PMA_PMD_STATUS_PMA_STATUS_2 . TRANSMIT_FAULT
 */
#define  VTSS_F_HOST_PMA_DEV1_IEEE_PMA_PMD_STATUS_PMA_STATUS_2_TRANSMIT_FAULT  VTSS_BIT(11)

/** 
 * \brief
 * Indicates a fault condition on this interface's receive path.  This is a
 * sticky bit that latches the high state.  The latch-high bit is cleared
 * when the register is read.
 *
 * \details 
 * 0: No fault condition on receive path
 * 1: Fault condition on receive path
 *
 * Field: VTSS_HOST_PMA_DEV1_IEEE_PMA_PMD_STATUS_PMA_STATUS_2 . RECEIVE_FAULT
 */
#define  VTSS_F_HOST_PMA_DEV1_IEEE_PMA_PMD_STATUS_PMA_STATUS_2_RECEIVE_FAULT  VTSS_BIT(10)

/**
 * Register Group: \a HOST_PMA:DEV1_IEEE_PMD_CONTROL_AND_STATUS
 *
 * Not documented
 */


/** 
 * \brief PMA Status 3
 *
 * \details
 * Register: \a HOST_PMA:DEV1_IEEE_PMD_CONTROL_AND_STATUS:PMA_STATUS_3
 */
#define VTSS_HOST_PMA_DEV1_IEEE_PMD_CONTROL_AND_STATUS_PMA_STATUS_3  VTSS_IOREG(0x09, 0, 0x000a)

/** 
 * \brief
 * Host PMA receiver signal detect
 *
 * \details 
 * 0: Signal not detected by receiver
 * 1: Signal detected by receiver
 *
 * Field: VTSS_HOST_PMA_DEV1_IEEE_PMD_CONTROL_AND_STATUS_PMA_STATUS_3 . GLOBAL_PMD_RECEIVE_SIGNAL_DETECT
 */
#define  VTSS_F_HOST_PMA_DEV1_IEEE_PMD_CONTROL_AND_STATUS_PMA_STATUS_3_GLOBAL_PMD_RECEIVE_SIGNAL_DETECT  VTSS_BIT(0)

/**
 * Register Group: \a HOST_PMA:KR_FEC_ABILITY
 *
 * KR FEC IEEE ABILITY REGISTER
 */


/** 
 * \brief KR FEC ability
 *
 * \details
 * Register: \a HOST_PMA:KR_FEC_ABILITY:KR_FEC_ABILITY
 */
#define VTSS_HOST_PMA_KR_FEC_ABILITY_KR_FEC_ABILITY  VTSS_IOREG(0x09, 0, 0x00aa)

/** 
 * \brief
 * FEC error reporting ability
 *
 * \details 
 * 
 * 0: This PHY device is not able to report FEC decoding errors to the PCS
 * layer.
 * 1: This PHY device is able to report FEC decoding errors to the PCS
 * layer.

 *
 * Field: VTSS_HOST_PMA_KR_FEC_ABILITY_KR_FEC_ABILITY . FEC_error_indication_ability
 */
#define  VTSS_F_HOST_PMA_KR_FEC_ABILITY_KR_FEC_ABILITY_FEC_error_indication_ability  VTSS_BIT(1)

/** 
 * \brief
 * FEC ability
 *
 * \details 
 * 
 * 0: This PHY device does not support FEC.
 * 1: This PHY device supports FEC.

 *
 * Field: VTSS_HOST_PMA_KR_FEC_ABILITY_KR_FEC_ABILITY . FEC_ability
 */
#define  VTSS_F_HOST_PMA_KR_FEC_ABILITY_KR_FEC_ABILITY_FEC_ability  VTSS_BIT(0)

/**
 * Register Group: \a HOST_PMA:KR_FEC_CONTROL_1
 *
 * KR FEC IEEE CONTROL REGISTER
 */


/** 
 * \brief KR FEC control 1
 *
 * \details
 * Register: \a HOST_PMA:KR_FEC_CONTROL_1:KR_FEC_CONTROL_1
 */
#define VTSS_HOST_PMA_KR_FEC_CONTROL_1_KR_FEC_CONTROL_1  VTSS_IOREG(0x09, 0, 0x00ab)

/** 
 * \details 
 * 
 * 0: Decoding errors have no effect on PCS sync bits
 * 1: Enable decoder to indicate errors to PCS sync bits

 *
 * Field: VTSS_HOST_PMA_KR_FEC_CONTROL_1_KR_FEC_CONTROL_1 . FEC_enable_error_indication
 */
#define  VTSS_F_HOST_PMA_KR_FEC_CONTROL_1_KR_FEC_CONTROL_1_FEC_enable_error_indication  VTSS_BIT(1)

/** 
 * \brief
 * FEC enable
 *
 * \details 
 * 
 * 0: Disable FEC
 * 1: Enable FEC

 *
 * Field: VTSS_HOST_PMA_KR_FEC_CONTROL_1_KR_FEC_CONTROL_1 . FEC_enable
 */
#define  VTSS_F_HOST_PMA_KR_FEC_CONTROL_1_KR_FEC_CONTROL_1_FEC_enable  VTSS_BIT(0)

/**
 * Register Group: \a HOST_PMA:KR_FEC_STATUS
 *
 * KR FEC IEEE STATUS REGISTER
 */


/** 
 * \brief KR FEC corrected lower
 *
 * \details
 * Register: \a HOST_PMA:KR_FEC_STATUS:KR_FEC_CORRECTED_LOWER
 */
#define VTSS_HOST_PMA_KR_FEC_STATUS_KR_FEC_CORRECTED_LOWER  VTSS_IOREG(0x09, 0, 0x00ac)

/** 
 * \brief
 * The FEC corrected block count is split across two registers,
 * KR_FEC_corrected_lower and KR_FEC_corrected_upper. 
 * KR_FEC_corrected_lower contains the least significant 16 bits of the
 * count.  KR_FEC_corrected_upper contains the most significant 16 bits of
 * the count.
 * Reading address KR_FEC_corrected_lower latches the 16 most significant
 * bits of the counter in KR_FEC_corrected_upper for future read out.  The
 * block count register is cleared when KR_FEC_corrected_lower is read.
 *
 * \details 
 * Field: VTSS_HOST_PMA_KR_FEC_STATUS_KR_FEC_CORRECTED_LOWER . FEC_CORRECTED_BLOCKS_LOWER
 */
#define  VTSS_F_HOST_PMA_KR_FEC_STATUS_KR_FEC_CORRECTED_LOWER_FEC_CORRECTED_BLOCKS_LOWER(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_HOST_PMA_KR_FEC_STATUS_KR_FEC_CORRECTED_LOWER_FEC_CORRECTED_BLOCKS_LOWER     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_HOST_PMA_KR_FEC_STATUS_KR_FEC_CORRECTED_LOWER_FEC_CORRECTED_BLOCKS_LOWER(x)  VTSS_EXTRACT_BITFIELD(x,0,16)


/** 
 * \brief KR FEC corrected upper
 *
 * \details
 * Register: \a HOST_PMA:KR_FEC_STATUS:KR_FEC_CORRECTED_UPPER
 */
#define VTSS_HOST_PMA_KR_FEC_STATUS_KR_FEC_CORRECTED_UPPER  VTSS_IOREG(0x09, 0, 0x00ad)

/** 
 * \brief
 * The FEC corrected block count is split across two registers,
 * KR_FEC_corrected_lower and KR_FEC_corrected_upper. 
 * KR_FEC_corrected_lower contains the least significant 16 bits of the
 * count.  KR_FEC_corrected_upper contains the most significant 16 bits of
 * the count.
 * Reading address KR_FEC_corrected_lower latches the 16 most significant
 * bits of the counter in KR_FEC_corrected_upper for future read out.  The
 * block count register is cleared when KR_FEC_corrected_lower is read.
 *
 * \details 
 * Field: VTSS_HOST_PMA_KR_FEC_STATUS_KR_FEC_CORRECTED_UPPER . FEC_CORRECTED_BLOCKS_UPPER
 */
#define  VTSS_F_HOST_PMA_KR_FEC_STATUS_KR_FEC_CORRECTED_UPPER_FEC_CORRECTED_BLOCKS_UPPER(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_HOST_PMA_KR_FEC_STATUS_KR_FEC_CORRECTED_UPPER_FEC_CORRECTED_BLOCKS_UPPER     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_HOST_PMA_KR_FEC_STATUS_KR_FEC_CORRECTED_UPPER_FEC_CORRECTED_BLOCKS_UPPER(x)  VTSS_EXTRACT_BITFIELD(x,0,16)


/** 
 * \brief KR FEC uncorrected lower
 *
 * \details
 * Register: \a HOST_PMA:KR_FEC_STATUS:KR_FEC_UNCORRECTED_LOWER
 */
#define VTSS_HOST_PMA_KR_FEC_STATUS_KR_FEC_UNCORRECTED_LOWER  VTSS_IOREG(0x09, 0, 0x00ae)

/** 
 * \brief
 * The FEC uncorrectable block count is split across two registers,
 * KR_FEC_uncorrected_lower and KR_FEC_uncorrected_upper. 
 * KR_FEC_uncorrected_lower contains the least significant 16 bits of the
 * count.  KR_FEC_uncorrected_upper contains the most significant 16 bits
 * of the count.
 * Reading address KR_FEC_uncorrected_lower latches the 16 most significant
 * bits of the counter in KR_FEC_uncorrected_upper for future read out. 
 * The block count register is cleared when KR_FEC_uncorrected_lower is
 * read.
 *
 * \details 
 * Field: VTSS_HOST_PMA_KR_FEC_STATUS_KR_FEC_UNCORRECTED_LOWER . FEC_UNCORRECTED_BLOCKS_LOWER
 */
#define  VTSS_F_HOST_PMA_KR_FEC_STATUS_KR_FEC_UNCORRECTED_LOWER_FEC_UNCORRECTED_BLOCKS_LOWER(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_HOST_PMA_KR_FEC_STATUS_KR_FEC_UNCORRECTED_LOWER_FEC_UNCORRECTED_BLOCKS_LOWER     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_HOST_PMA_KR_FEC_STATUS_KR_FEC_UNCORRECTED_LOWER_FEC_UNCORRECTED_BLOCKS_LOWER(x)  VTSS_EXTRACT_BITFIELD(x,0,16)


/** 
 * \brief KR FEC uncorrected upper
 *
 * \details
 * Register: \a HOST_PMA:KR_FEC_STATUS:KR_FEC_UNCORRECTED_UPPER
 */
#define VTSS_HOST_PMA_KR_FEC_STATUS_KR_FEC_UNCORRECTED_UPPER  VTSS_IOREG(0x09, 0, 0x00af)

/** 
 * \brief
 * The FEC uncorrectable block count is split across two registers,
 * KR_FEC_uncorrected_lower and KR_FEC_uncorrected_upper. 
 * KR_FEC_uncorrected_lower contains the least significant 16 bits of the
 * count.  KR_FEC_uncorrected_upper contains the most significant 16 bits
 * of the count.
 * Reading address KR_FEC_uncorrected_lower latches the 16 most significant
 * bits of the counter in KR_FEC_uncorrected_upper for future read out. 
 * The block count register is cleared when KR_FEC_uncorrected_lower is
 * read.
 *
 * \details 
 * Field: VTSS_HOST_PMA_KR_FEC_STATUS_KR_FEC_UNCORRECTED_UPPER . FEC_UNCORRECTED_BLOCKS_UPPER
 */
#define  VTSS_F_HOST_PMA_KR_FEC_STATUS_KR_FEC_UNCORRECTED_UPPER_FEC_UNCORRECTED_BLOCKS_UPPER(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_HOST_PMA_KR_FEC_STATUS_KR_FEC_UNCORRECTED_UPPER_FEC_UNCORRECTED_BLOCKS_UPPER     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_HOST_PMA_KR_FEC_STATUS_KR_FEC_UNCORRECTED_UPPER_FEC_UNCORRECTED_BLOCKS_UPPER(x)  VTSS_EXTRACT_BITFIELD(x,0,16)

/**
 * Register Group: \a HOST_PMA:KR_FEC_CONTROL_2
 *
 * KR FEC Configuration and Status Vendor Register set
 */


/** 
 * \brief KR_FEC_Control_2
 *
 * \details
 * Register: \a HOST_PMA:KR_FEC_CONTROL_2:KR_FEC_Control_2
 */
#define VTSS_HOST_PMA_KR_FEC_CONTROL_2_KR_FEC_Control_2  VTSS_IOREG(0x09, 0, 0x8300)

/** 
 * \brief
 * FEC in frame lock indication.  This is a sticky bit that latches the low
 * state. The latch-low bit is cleared when the register is read.
 *
 * \details 
 * 
 * 0: FEC has not achieved lock
 * 1: FEC has achieved lock

 *
 * Field: VTSS_HOST_PMA_KR_FEC_CONTROL_2_KR_FEC_Control_2 . fec_inframe
 */
#define  VTSS_F_HOST_PMA_KR_FEC_CONTROL_2_KR_FEC_Control_2_fec_inframe  VTSS_BIT(1)

/** 
 * \brief
 * FEC counters reset
 *
 * \details 
 * 
 * 0: no effect
 * 1: reset FEC counters

 *
 * Field: VTSS_HOST_PMA_KR_FEC_CONTROL_2_KR_FEC_Control_2 . fec_rstmon
 */
#define  VTSS_F_HOST_PMA_KR_FEC_CONTROL_2_KR_FEC_Control_2_fec_rstmon  VTSS_BIT(0)

/**
 * Register Group: \a HOST_PMA:LOOPBACK_CONTROL
 *
 * Control Signal for Loopback
 */


/** 
 * \brief Datapath Loopback Control
 *
 * \details
 * Datapath Loopback Control
 *
 * Register: \a HOST_PMA:LOOPBACK_CONTROL:PMA_LOOPBACK_CTRL
 */
#define VTSS_HOST_PMA_LOOPBACK_CONTROL_PMA_LOOPBACK_CTRL  VTSS_IOREG(0x09, 0, 0xa003)

/** 
 * \brief
 * Loopback L3 Enable
 *
 * \details 
 * 0 = Normal Operation
 * 1 = Enable L3 Loopback 
 *
 * Field: VTSS_HOST_PMA_LOOPBACK_CONTROL_PMA_LOOPBACK_CTRL . L3_CONTROL
 */
#define  VTSS_F_HOST_PMA_LOOPBACK_CONTROL_PMA_LOOPBACK_CTRL_L3_CONTROL  VTSS_BIT(0)

/**
 * Register Group: \a HOST_PMA:ET_SERDES_DATAPATH_CTRL
 *
 * ET Serdes Data Path Control 
 */


/** 
 * \brief Select ET Serdes RPA input
 *
 * \details
 * Register: \a HOST_PMA:ET_SERDES_DATAPATH_CTRL:ET_SERDES_RPAIN_SEL
 */
#define VTSS_HOST_PMA_ET_SERDES_DATAPATH_CTRL_ET_SERDES_RPAIN_SEL  VTSS_IOREG(0x09, 0, 0xa004)

/** 
 * \brief
 * Select ET Serdes RPA input from main deserializer channel or one of the
 * two auxiliary channel
 *
 * \details 
 * 0x = des_data_r	     // from DES
 * 10 = des_vl_data_r	  // from DES-L
 * 11 = des_vh_data_r	 // from DES-H
 *
 * Field: VTSS_HOST_PMA_ET_SERDES_DATAPATH_CTRL_ET_SERDES_RPAIN_SEL . ET_SERDES_RPAIN_SEL
 */
#define  VTSS_F_HOST_PMA_ET_SERDES_DATAPATH_CTRL_ET_SERDES_RPAIN_SEL_ET_SERDES_RPAIN_SEL(x)  VTSS_ENCODE_BITFIELD(x,0,2)
#define  VTSS_M_HOST_PMA_ET_SERDES_DATAPATH_CTRL_ET_SERDES_RPAIN_SEL_ET_SERDES_RPAIN_SEL     VTSS_ENCODE_BITMASK(0,2)
#define  VTSS_X_HOST_PMA_ET_SERDES_DATAPATH_CTRL_ET_SERDES_RPAIN_SEL_ET_SERDES_RPAIN_SEL(x)  VTSS_EXTRACT_BITFIELD(x,0,2)

/**
 * Register Group: \a HOST_PMA:LOCK2REF_CONTROL
 *
 * Control Lock to REF mode in the serdes
 */


/** 
 * \brief Control Lock to REF mode in the serdes
 *
 * \details
 * Control Lock to REF mode in the serdes
 *
 * Register: \a HOST_PMA:LOCK2REF_CONTROL:LOCK2REF_CONTROL
 */
#define VTSS_HOST_PMA_LOCK2REF_CONTROL_LOCK2REF_CONTROL  VTSS_IOREG(0x09, 0, 0xa005)

/** 
 * \brief
 * Control Lock to REF mode in the serdes
 *
 * \details 
 * 0 = Lock to Data
 * 1 = Lock to Ref
 *
 * Field: VTSS_HOST_PMA_LOCK2REF_CONTROL_LOCK2REF_CONTROL . LOCK2REF_CONTROL
 */
#define  VTSS_F_HOST_PMA_LOCK2REF_CONTROL_LOCK2REF_CONTROL_LOCK2REF_CONTROL  VTSS_BIT(0)

/**
 * Register Group: \a HOST_PMA:SD10G65_MISC_DEBUG
 *
 * Miscelanous Debug Registers for SD10G65
 */


/** 
 * \brief SD10G65 Spread Sprectrum Clocking
 *
 * \details
 * SD10G65 Spread Sprectrum Clocking
 *
 * Register: \a HOST_PMA:SD10G65_MISC_DEBUG:SD10G65_SSC_SYNC
 */
#define VTSS_HOST_PMA_SD10G65_MISC_DEBUG_SD10G65_SSC_SYNC  VTSS_IOREG(0x09, 0, 0xa008)

/** 
 * \brief
 * Spread Spectrum Clocking Output
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_MISC_DEBUG_SD10G65_SSC_SYNC . SSC_SYNC_STAT
 */
#define  VTSS_F_HOST_PMA_SD10G65_MISC_DEBUG_SD10G65_SSC_SYNC_SSC_SYNC_STAT  VTSS_BIT(4)

/** 
 * \brief
 * Spread Spectrum Clocking Input
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_MISC_DEBUG_SD10G65_SSC_SYNC . SSC_SYNC_CFG
 */
#define  VTSS_F_HOST_PMA_SD10G65_MISC_DEBUG_SD10G65_SSC_SYNC_SSC_SYNC_CFG  VTSS_BIT(0)


/** 
 * \brief SD10G65 Smooth Cloking Update
 *
 * \details
 * SD10G65 Smooth Cloking Update
 *
 * Register: \a HOST_PMA:SD10G65_MISC_DEBUG:SD10G65_SMOOTH_UPDATE
 */
#define VTSS_HOST_PMA_SD10G65_MISC_DEBUG_SD10G65_SMOOTH_UPDATE  VTSS_IOREG(0x09, 0, 0xa009)

/** 
 * \brief
 * Smooth Update
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_MISC_DEBUG_SD10G65_SMOOTH_UPDATE . SMOOTH_UPDATE
 */
#define  VTSS_F_HOST_PMA_SD10G65_MISC_DEBUG_SD10G65_SMOOTH_UPDATE_SMOOTH_UPDATE  VTSS_BIT(0)


/** 
 * \brief SD10G65 Smooth Cloking Value MSB
 *
 * \details
 * SD10G65 Smooth Cloking Value MSB
 *
 * Register: \a HOST_PMA:SD10G65_MISC_DEBUG:SD10G65_SMOOTH_VALUE_MSB
 */
#define VTSS_HOST_PMA_SD10G65_MISC_DEBUG_SD10G65_SMOOTH_VALUE_MSB  VTSS_IOREG(0x09, 0, 0xa00a)

/** 
 * \brief
 * Smooth Cloking Value MSB bit [22:16]
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_MISC_DEBUG_SD10G65_SMOOTH_VALUE_MSB . SMOOTH_VALUE_MSB
 */
#define  VTSS_F_HOST_PMA_SD10G65_MISC_DEBUG_SD10G65_SMOOTH_VALUE_MSB_SMOOTH_VALUE_MSB(x)  VTSS_ENCODE_BITFIELD(x,0,7)
#define  VTSS_M_HOST_PMA_SD10G65_MISC_DEBUG_SD10G65_SMOOTH_VALUE_MSB_SMOOTH_VALUE_MSB     VTSS_ENCODE_BITMASK(0,7)
#define  VTSS_X_HOST_PMA_SD10G65_MISC_DEBUG_SD10G65_SMOOTH_VALUE_MSB_SMOOTH_VALUE_MSB(x)  VTSS_EXTRACT_BITFIELD(x,0,7)


/** 
 * \brief SD10G65 Smooth Cloking Value LSB
 *
 * \details
 * SD10G65 Smooth Cloking Value LSB
 *
 * Register: \a HOST_PMA:SD10G65_MISC_DEBUG:SD10G65_SMOOTH_VALUE_LSB
 */
#define VTSS_HOST_PMA_SD10G65_MISC_DEBUG_SD10G65_SMOOTH_VALUE_LSB  VTSS_IOREG(0x09, 0, 0xa00b)

/** 
 * \brief
 * Smooth Cloking Value MSB bit [15:0]
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_MISC_DEBUG_SD10G65_SMOOTH_VALUE_LSB . SMOOTH_VALUE_LSB
 */
#define  VTSS_F_HOST_PMA_SD10G65_MISC_DEBUG_SD10G65_SMOOTH_VALUE_LSB_SMOOTH_VALUE_LSB(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_HOST_PMA_SD10G65_MISC_DEBUG_SD10G65_SMOOTH_VALUE_LSB_SMOOTH_VALUE_LSB     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_HOST_PMA_SD10G65_MISC_DEBUG_SD10G65_SMOOTH_VALUE_LSB_SMOOTH_VALUE_LSB(x)  VTSS_EXTRACT_BITFIELD(x,0,16)

/**
 * Register Group: \a HOST_PMA:BYPASS_CONFIG_STAT
 *
 * Not documented
 */


/** 
 * \brief Repeater Mode Configuration
 *
 * \details
 * Register: \a HOST_PMA:BYPASS_CONFIG_STAT:BYPASS_CFG1
 */
#define VTSS_HOST_PMA_BYPASS_CONFIG_STAT_BYPASS_CFG1  VTSS_IOREG(0x09, 0, 0xa020)

/** 
 * \brief
 * Reset the bypass FIFO
 *
 * \details 
 * 0 = Normal operation
 * 1 = RESET
 *
 * Field: VTSS_HOST_PMA_BYPASS_CONFIG_STAT_BYPASS_CFG1 . FIFO_RESET
 */
#define  VTSS_F_HOST_PMA_BYPASS_CONFIG_STAT_BYPASS_CFG1_FIFO_RESET  VTSS_BIT(8)

/** 
 * \brief
 * Enable bypass FIFO
 *
 * \details 
 * 0 = FIFO Disabled
 * 1 = FIFO Enabled
 *
 * Field: VTSS_HOST_PMA_BYPASS_CONFIG_STAT_BYPASS_CFG1 . REPEATER_FIFO_EN
 */
#define  VTSS_F_HOST_PMA_BYPASS_CONFIG_STAT_BYPASS_CFG1_REPEATER_FIFO_EN  VTSS_BIT(1)

/** 
 * \brief
 * Enable repeater mode.  This causes the transmit multiplexor to select
 * bypass data rather than data from the processing core
 *
 * \details 
 * 0 = Standard mode
 * 1 = Repeater Mode
 *
 * Field: VTSS_HOST_PMA_BYPASS_CONFIG_STAT_BYPASS_CFG1 . REPEATER_MODE_EN
 */
#define  VTSS_F_HOST_PMA_BYPASS_CONFIG_STAT_BYPASS_CFG1_REPEATER_MODE_EN  VTSS_BIT(0)


/** 
 * \details
 * Register: \a HOST_PMA:BYPASS_CONFIG_STAT:BYPASS_STATUS
 */
#define VTSS_HOST_PMA_BYPASS_CONFIG_STAT_BYPASS_STATUS  VTSS_IOREG(0x09, 0, 0xa021)

/** 
 * \brief
 * FIFO Underflow state
 *
 * \details 
 * Field: VTSS_HOST_PMA_BYPASS_CONFIG_STAT_BYPASS_STATUS . FIFO_UNDERFLOW_STAT
 */
#define  VTSS_F_HOST_PMA_BYPASS_CONFIG_STAT_BYPASS_STATUS_FIFO_UNDERFLOW_STAT  VTSS_BIT(1)

/** 
 * \brief
 * FIFO Overflow state
 *
 * \details 
 * Field: VTSS_HOST_PMA_BYPASS_CONFIG_STAT_BYPASS_STATUS . FIFO_OVERFLOW_STAT
 */
#define  VTSS_F_HOST_PMA_BYPASS_CONFIG_STAT_BYPASS_STATUS_FIFO_OVERFLOW_STAT  VTSS_BIT(0)


/** 
 * \details
 * Register: \a HOST_PMA:BYPASS_CONFIG_STAT:BYPASS_INTR_EN
 */
#define VTSS_HOST_PMA_BYPASS_CONFIG_STAT_BYPASS_INTR_EN  VTSS_IOREG(0x09, 0, 0xa022)

/** 
 * \brief
 * Allow FIFO_UNDERFLOW_STICKY to propogate to interrupt
 *
 * \details 
 * Field: VTSS_HOST_PMA_BYPASS_CONFIG_STAT_BYPASS_INTR_EN . FIFO_UNDERFLOW_INTR_EN
 */
#define  VTSS_F_HOST_PMA_BYPASS_CONFIG_STAT_BYPASS_INTR_EN_FIFO_UNDERFLOW_INTR_EN  VTSS_BIT(1)

/** 
 * \brief
 * Enable FIFO_OVERFLOW_STICKY to propogate to interrupt
 *
 * \details 
 * Field: VTSS_HOST_PMA_BYPASS_CONFIG_STAT_BYPASS_INTR_EN . FIFO_OVERFLOW_INTR_EN
 */
#define  VTSS_F_HOST_PMA_BYPASS_CONFIG_STAT_BYPASS_INTR_EN_FIFO_OVERFLOW_INTR_EN  VTSS_BIT(0)


/** 
 * \details
 * Register: \a HOST_PMA:BYPASS_CONFIG_STAT:BYPASS_INTR
 */
#define VTSS_HOST_PMA_BYPASS_CONFIG_STAT_BYPASS_INTR  VTSS_IOREG(0x09, 0, 0xa023)

/** 
 * \details 
 * Field: VTSS_HOST_PMA_BYPASS_CONFIG_STAT_BYPASS_INTR . FIFO_UNDERFLOW_STICKY
 */
#define  VTSS_F_HOST_PMA_BYPASS_CONFIG_STAT_BYPASS_INTR_FIFO_UNDERFLOW_STICKY  VTSS_BIT(1)

/** 
 * \brief
 * FIFO overflow occured
 *
 * \details 
 * Field: VTSS_HOST_PMA_BYPASS_CONFIG_STAT_BYPASS_INTR . FIFO_OVERFLOW_STICKY
 */
#define  VTSS_F_HOST_PMA_BYPASS_CONFIG_STAT_BYPASS_INTR_FIFO_OVERFLOW_STICKY  VTSS_BIT(0)

/**
 * Register Group: \a HOST_PMA:Vendor_Specific_PMA_Control_2
 *
 * Not documented
 */


/** 
 * \brief Vendor Specific PMA Control 2
 *
 * \details
 * Register: \a HOST_PMA:Vendor_Specific_PMA_Control_2:Vendor_Specific_PMA_Control_2
 */
#define VTSS_HOST_PMA_Vendor_Specific_PMA_Control_2_Vendor_Specific_PMA_Control_2  VTSS_IOREG(0x09, 0, 0xa100)

/** 
 * \brief
 * LOS circuitry is driven by a signal detection status signal in the
 * line-side input buffer.  The signal detection alarm driving the LOS
 * curcuitry can be squelched with this register bit.
 *
 * \details 
 * LOS detection is
 * 0: Allowed
 * 1: Suppressed
 *
 * Field: VTSS_HOST_PMA_Vendor_Specific_PMA_Control_2_Vendor_Specific_PMA_Control_2 . Suppress_LOS_detection
 */
#define  VTSS_F_HOST_PMA_Vendor_Specific_PMA_Control_2_Vendor_Specific_PMA_Control_2_Suppress_LOS_detection  VTSS_BIT(10)

/** 
 * \brief
 * LOL circuitry is driven by a status signal in the line-side CRU.  The
 * status signal driving the LOL curcuitry can be squelched with this
 * register bit.
 *
 * \details 
 * LOL detection is
 * 0: Allowed
 * 1: Suppressed
 *
 * Field: VTSS_HOST_PMA_Vendor_Specific_PMA_Control_2_Vendor_Specific_PMA_Control_2 . Suppress_LOL_detection
 */
#define  VTSS_F_HOST_PMA_Vendor_Specific_PMA_Control_2_Vendor_Specific_PMA_Control_2_Suppress_LOL_detection  VTSS_BIT(9)

/**
 * Register Group: \a HOST_PMA:Vendor_Specific_PMA_Control_4
 *
 * Not documented
 */


/** 
 * \brief SerDes 10G Data_valid_Det Override
 *
 * \details
 * Register: \a HOST_PMA:Vendor_Specific_PMA_Control_4:Vendor_Specific_PMA_Control_4
 */
#define VTSS_HOST_PMA_Vendor_Specific_PMA_Control_4_Vendor_Specific_PMA_Control_4  VTSS_IOREG(0x09, 0, 0xa103)

/** 
 * \brief
 * Selects the source of the signal driving the data_valid_det input to the
 * PMA's CDR.  The data_valid_det siganl affects the CDR's loop bandwidth.
 *
 * \details 
 * The signal source is
 * 0: PCS block_lock status (10G) / PCS link status (1G) 
 * 1: bit [0] in this register
 *
 * Field: VTSS_HOST_PMA_Vendor_Specific_PMA_Control_4_Vendor_Specific_PMA_Control_4 . SD10G_DATA_VALID_DET_OVERRIDE
 */
#define  VTSS_F_HOST_PMA_Vendor_Specific_PMA_Control_4_Vendor_Specific_PMA_Control_4_SD10G_DATA_VALID_DET_OVERRIDE  VTSS_BIT(1)

/** 
 * \brief
 * This register directly drives the data_valid_det input to the PMA's CDR
 * when bit [1] in this register is 1
 *
 * \details 
 * Field: VTSS_HOST_PMA_Vendor_Specific_PMA_Control_4_Vendor_Specific_PMA_Control_4 . SD10G_DATA_VALID_DET_DATA
 */
#define  VTSS_F_HOST_PMA_Vendor_Specific_PMA_Control_4_Vendor_Specific_PMA_Control_4_SD10G_DATA_VALID_DET_DATA  VTSS_BIT(0)

/**
 * Register Group: \a HOST_PMA:PMA_INTR
 *
 * Not documented
 */


/** 
 * \details
 * Register: \a HOST_PMA:PMA_INTR:PMA_STAT
 */
#define VTSS_HOST_PMA_PMA_INTR_PMA_STAT      VTSS_IOREG(0x09, 0, 0xa202)

/** 
 * \brief
 * Current state of TX LOL from SerDes
 *
 * \details 
 * 0 = TX PLL Locked
 * 1 = TX PLL Not locked
 *
 * Field: VTSS_HOST_PMA_PMA_INTR_PMA_STAT . TX_LOL_STAT
 */
#define  VTSS_F_HOST_PMA_PMA_INTR_PMA_STAT_TX_LOL_STAT  VTSS_BIT(2)

/** 
 * \brief
 * Current state of RX LOL from SerDes
 *
 * \details 
 * 0 = RX PLL Locked
 * 1 = RX PLL Not locked
 *
 * Field: VTSS_HOST_PMA_PMA_INTR_PMA_STAT . RX_LOL_STAT
 */
#define  VTSS_F_HOST_PMA_PMA_INTR_PMA_STAT_RX_LOL_STAT  VTSS_BIT(1)

/** 
 * \brief
 * LOS detected at Serdes
 *
 * \details 
 * 0 = Signal detected at input receiver
 * 1 = No signal detected at input receiver
 *
 * Field: VTSS_HOST_PMA_PMA_INTR_PMA_STAT . RX_LOS_STAT
 */
#define  VTSS_F_HOST_PMA_PMA_INTR_PMA_STAT_RX_LOS_STAT  VTSS_BIT(0)


/** 
 * \details
 * Register: \a HOST_PMA:PMA_INTR:PMA_INTR_MASK
 */
#define VTSS_HOST_PMA_PMA_INTR_PMA_INTR_MASK  VTSS_IOREG(0x09, 0, 0xa203)

/** 
 * \brief
 * Enable interrupt when TX LOL detected
 *
 * \details 
 * 0 = Detected TX LOL condition not propgated to interrupt
 * 1 = Detected TX LOL condition is propogated to interrupt
 *
 * Field: VTSS_HOST_PMA_PMA_INTR_PMA_INTR_MASK . TX_LOL_INTR_EN
 */
#define  VTSS_F_HOST_PMA_PMA_INTR_PMA_INTR_MASK_TX_LOL_INTR_EN  VTSS_BIT(2)

/** 
 * \brief
 * Enable interrupt when RX LOL detected
 *
 * \details 
 * 0 = Detected RX LOL condition not propgated to interrupt
 * 1 = Detected RX LOL condition is propogated to interrupt
 *
 * Field: VTSS_HOST_PMA_PMA_INTR_PMA_INTR_MASK . RX_LOL_INTR_EN
 */
#define  VTSS_F_HOST_PMA_PMA_INTR_PMA_INTR_MASK_RX_LOL_INTR_EN  VTSS_BIT(1)

/** 
 * \brief
 * Enable interrupt when RX LOS detected
 *
 * \details 
 * 0 = Detected LOS condition not propgated to interrupt
 * 1 = Detected LOS condition is propogated to interrupt
 *
 * Field: VTSS_HOST_PMA_PMA_INTR_PMA_INTR_MASK . RX_LOS_INTR_EN
 */
#define  VTSS_F_HOST_PMA_PMA_INTR_PMA_INTR_MASK_RX_LOS_INTR_EN  VTSS_BIT(0)


/** 
 * \details
 * Register: \a HOST_PMA:PMA_INTR:PMA_INTR_STAT
 */
#define VTSS_HOST_PMA_PMA_INTR_PMA_INTR_STAT  VTSS_IOREG(0x09, 0, 0xa204)

/** 
 * \brief
 * TX LOL Detected
 *
 * \details 
 * 0 = No TX LOL detected since last cleared
 * 1 = TX LOL Detected since last cleared
 *
 * Field: VTSS_HOST_PMA_PMA_INTR_PMA_INTR_STAT . TX_LOL_STICKY
 */
#define  VTSS_F_HOST_PMA_PMA_INTR_PMA_INTR_STAT_TX_LOL_STICKY  VTSS_BIT(2)

/** 
 * \brief
 * RX LOL Detected
 *
 * \details 
 * 0 = No RX LOL detected since last cleared
 * 1 = RX LOL Detected since last cleared
 *
 * Field: VTSS_HOST_PMA_PMA_INTR_PMA_INTR_STAT . RX_LOL_STICKY
 */
#define  VTSS_F_HOST_PMA_PMA_INTR_PMA_INTR_STAT_RX_LOL_STICKY  VTSS_BIT(1)

/** 
 * \brief
 * RX LOS Detected
 *
 * \details 
 * 0 = No RX LOS detected since last cleared
 * 1 = RX LOS Detected since last cleared
 *
 * Field: VTSS_HOST_PMA_PMA_INTR_PMA_INTR_STAT . RX_LOS_STICKY
 */
#define  VTSS_F_HOST_PMA_PMA_INTR_PMA_INTR_STAT_RX_LOS_STICKY  VTSS_BIT(0)

/**
 * Register Group: \a HOST_PMA:SPARE_RO_REGISTERS
 *
 * Spare RO Registers
 */


/** 
 * \brief Device1 Spare RO 0
 *
 * \details
 * Register: \a HOST_PMA:SPARE_RO_REGISTERS:DEV1_SPARE_RO0
 */
#define VTSS_HOST_PMA_SPARE_RO_REGISTERS_DEV1_SPARE_RO0  VTSS_IOREG(0x09, 0, 0xaeec)

/** 
 * \brief
 * Spare
 *
 * \details 
 * Field: VTSS_HOST_PMA_SPARE_RO_REGISTERS_DEV1_SPARE_RO0 . dev1_spare_ro0
 */
#define  VTSS_F_HOST_PMA_SPARE_RO_REGISTERS_DEV1_SPARE_RO0_dev1_spare_ro0(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_HOST_PMA_SPARE_RO_REGISTERS_DEV1_SPARE_RO0_dev1_spare_ro0     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_HOST_PMA_SPARE_RO_REGISTERS_DEV1_SPARE_RO0_dev1_spare_ro0(x)  VTSS_EXTRACT_BITFIELD(x,0,16)


/** 
 * \brief Device1 Spare RO 1
 *
 * \details
 * Register: \a HOST_PMA:SPARE_RO_REGISTERS:DEV1_SPARE_RO1
 */
#define VTSS_HOST_PMA_SPARE_RO_REGISTERS_DEV1_SPARE_RO1  VTSS_IOREG(0x09, 0, 0xaeed)

/** 
 * \brief
 * Spare
 *
 * \details 
 * Field: VTSS_HOST_PMA_SPARE_RO_REGISTERS_DEV1_SPARE_RO1 . dev1_spare_ro1
 */
#define  VTSS_F_HOST_PMA_SPARE_RO_REGISTERS_DEV1_SPARE_RO1_dev1_spare_ro1(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_HOST_PMA_SPARE_RO_REGISTERS_DEV1_SPARE_RO1_dev1_spare_ro1     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_HOST_PMA_SPARE_RO_REGISTERS_DEV1_SPARE_RO1_dev1_spare_ro1(x)  VTSS_EXTRACT_BITFIELD(x,0,16)


/** 
 * \brief Device1 Spare RO 2
 *
 * \details
 * Register: \a HOST_PMA:SPARE_RO_REGISTERS:DEV1_SPARE_RO2
 */
#define VTSS_HOST_PMA_SPARE_RO_REGISTERS_DEV1_SPARE_RO2  VTSS_IOREG(0x09, 0, 0xaeee)

/** 
 * \brief
 * Spare
 *
 * \details 
 * Field: VTSS_HOST_PMA_SPARE_RO_REGISTERS_DEV1_SPARE_RO2 . dev1_spare_ro2
 */
#define  VTSS_F_HOST_PMA_SPARE_RO_REGISTERS_DEV1_SPARE_RO2_dev1_spare_ro2(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_HOST_PMA_SPARE_RO_REGISTERS_DEV1_SPARE_RO2_dev1_spare_ro2     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_HOST_PMA_SPARE_RO_REGISTERS_DEV1_SPARE_RO2_dev1_spare_ro2(x)  VTSS_EXTRACT_BITFIELD(x,0,16)


/** 
 * \brief Device1 Spare RO 3
 *
 * \details
 * Register: \a HOST_PMA:SPARE_RO_REGISTERS:DEV1_SPARE_RO3
 */
#define VTSS_HOST_PMA_SPARE_RO_REGISTERS_DEV1_SPARE_RO3  VTSS_IOREG(0x09, 0, 0xaeef)

/** 
 * \brief
 * Spare
 *
 * \details 
 * Field: VTSS_HOST_PMA_SPARE_RO_REGISTERS_DEV1_SPARE_RO3 . dev1_spare_ro3
 */
#define  VTSS_F_HOST_PMA_SPARE_RO_REGISTERS_DEV1_SPARE_RO3_dev1_spare_ro3(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_HOST_PMA_SPARE_RO_REGISTERS_DEV1_SPARE_RO3_dev1_spare_ro3     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_HOST_PMA_SPARE_RO_REGISTERS_DEV1_SPARE_RO3_dev1_spare_ro3(x)  VTSS_EXTRACT_BITFIELD(x,0,16)

/**
 * Register Group: \a HOST_PMA:SPARE_RW_REGISTERS
 *
 * Spare R/W Registers
 */


/** 
 * \brief Device1 Spare R/W 0
 *
 * \details
 * Register: \a HOST_PMA:SPARE_RW_REGISTERS:DEV1_SPARE_RW0
 */
#define VTSS_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW0  VTSS_IOREG(0x09, 0, 0xaef0)

/** 
 * \brief
 * Spare
 *
 * \details 
 * Field: VTSS_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW0 . dev1_spare_rw0
 */
#define  VTSS_F_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW0_dev1_spare_rw0(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW0_dev1_spare_rw0     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW0_dev1_spare_rw0(x)  VTSS_EXTRACT_BITFIELD(x,0,16)


/** 
 * \brief Device1 Spare R/W 1
 *
 * \details
 * Register: \a HOST_PMA:SPARE_RW_REGISTERS:DEV1_SPARE_RW1
 */
#define VTSS_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW1  VTSS_IOREG(0x09, 0, 0xaef1)

/** 
 * \brief
 * Spare
 *
 * \details 
 * Field: VTSS_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW1 . dev1_spare_rw1
 */
#define  VTSS_F_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW1_dev1_spare_rw1(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW1_dev1_spare_rw1     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW1_dev1_spare_rw1(x)  VTSS_EXTRACT_BITFIELD(x,0,16)


/** 
 * \brief Device1 Spare R/W 2
 *
 * \details
 * Register: \a HOST_PMA:SPARE_RW_REGISTERS:DEV1_SPARE_RW2
 */
#define VTSS_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW2  VTSS_IOREG(0x09, 0, 0xaef2)

/** 
 * \brief
 * Spare
 *
 * \details 
 * Field: VTSS_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW2 . dev1_spare_rw2
 */
#define  VTSS_F_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW2_dev1_spare_rw2(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW2_dev1_spare_rw2     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW2_dev1_spare_rw2(x)  VTSS_EXTRACT_BITFIELD(x,0,16)


/** 
 * \brief Device1 Spare R/W 3
 *
 * \details
 * Register: \a HOST_PMA:SPARE_RW_REGISTERS:DEV1_SPARE_RW3
 */
#define VTSS_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW3  VTSS_IOREG(0x09, 0, 0xaef3)

/** 
 * \brief
 * Spare
 *
 * \details 
 * Field: VTSS_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW3 . dev1_spare_rw3
 */
#define  VTSS_F_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW3_dev1_spare_rw3(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW3_dev1_spare_rw3     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW3_dev1_spare_rw3(x)  VTSS_EXTRACT_BITFIELD(x,0,16)


/** 
 * \brief Device1 Spare R/W 4
 *
 * \details
 * Register: \a HOST_PMA:SPARE_RW_REGISTERS:DEV1_SPARE_RW4
 */
#define VTSS_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW4  VTSS_IOREG(0x09, 0, 0xaef4)

/** 
 * \brief
 * Spare
 *
 * \details 
 * Field: VTSS_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW4 . dev1_spare_rw4
 */
#define  VTSS_F_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW4_dev1_spare_rw4(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW4_dev1_spare_rw4     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW4_dev1_spare_rw4(x)  VTSS_EXTRACT_BITFIELD(x,0,16)


/** 
 * \brief Device1 Spare R/W 5
 *
 * \details
 * Register: \a HOST_PMA:SPARE_RW_REGISTERS:DEV1_SPARE_RW5
 */
#define VTSS_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW5  VTSS_IOREG(0x09, 0, 0xaef5)

/** 
 * \brief
 * Spare
 *
 * \details 
 * Field: VTSS_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW5 . dev1_spare_rw5
 */
#define  VTSS_F_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW5_dev1_spare_rw5(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW5_dev1_spare_rw5     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW5_dev1_spare_rw5(x)  VTSS_EXTRACT_BITFIELD(x,0,16)


/** 
 * \brief Device1 Spare R/W 6
 *
 * \details
 * Register: \a HOST_PMA:SPARE_RW_REGISTERS:DEV1_SPARE_RW6
 */
#define VTSS_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW6  VTSS_IOREG(0x09, 0, 0xaef6)

/** 
 * \brief
 * Spare
 *
 * \details 
 * Field: VTSS_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW6 . dev1_spare_rw6
 */
#define  VTSS_F_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW6_dev1_spare_rw6(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW6_dev1_spare_rw6     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW6_dev1_spare_rw6(x)  VTSS_EXTRACT_BITFIELD(x,0,16)


/** 
 * \brief Device1 Spare R/W 7
 *
 * \details
 * Register: \a HOST_PMA:SPARE_RW_REGISTERS:DEV1_SPARE_RW7
 */
#define VTSS_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW7  VTSS_IOREG(0x09, 0, 0xaef7)

/** 
 * \brief
 * Spare
 *
 * \details 
 * Field: VTSS_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW7 . dev1_spare_rw7
 */
#define  VTSS_F_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW7_dev1_spare_rw7(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW7_dev1_spare_rw7     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW7_dev1_spare_rw7(x)  VTSS_EXTRACT_BITFIELD(x,0,16)


/** 
 * \brief Device1 Spare R/W 8
 *
 * \details
 * Register: \a HOST_PMA:SPARE_RW_REGISTERS:DEV1_SPARE_RW8
 */
#define VTSS_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW8  VTSS_IOREG(0x09, 0, 0xaef8)

/** 
 * \brief
 * Spare
 *
 * \details 
 * Field: VTSS_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW8 . dev1_spare_rw8
 */
#define  VTSS_F_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW8_dev1_spare_rw8(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW8_dev1_spare_rw8     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW8_dev1_spare_rw8(x)  VTSS_EXTRACT_BITFIELD(x,0,16)


/** 
 * \brief Device1 Spare R/W 9
 *
 * \details
 * Register: \a HOST_PMA:SPARE_RW_REGISTERS:DEV1_SPARE_RW9
 */
#define VTSS_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW9  VTSS_IOREG(0x09, 0, 0xaef9)

/** 
 * \brief
 * Spare
 *
 * \details 
 * Field: VTSS_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW9 . dev1_spare_rw9
 */
#define  VTSS_F_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW9_dev1_spare_rw9(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW9_dev1_spare_rw9     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW9_dev1_spare_rw9(x)  VTSS_EXTRACT_BITFIELD(x,0,16)


/** 
 * \brief Device1 Spare R/W 10
 *
 * \details
 * Register: \a HOST_PMA:SPARE_RW_REGISTERS:DEV1_SPARE_RW10
 */
#define VTSS_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW10  VTSS_IOREG(0x09, 0, 0xaefa)

/** 
 * \brief
 * Spare
 *
 * \details 
 * Field: VTSS_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW10 . dev1_spare_rw10
 */
#define  VTSS_F_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW10_dev1_spare_rw10(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW10_dev1_spare_rw10     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW10_dev1_spare_rw10(x)  VTSS_EXTRACT_BITFIELD(x,0,16)


/** 
 * \brief Device1 Spare R/W 11
 *
 * \details
 * Register: \a HOST_PMA:SPARE_RW_REGISTERS:DEV1_SPARE_RW11
 */
#define VTSS_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW11  VTSS_IOREG(0x09, 0, 0xaefb)

/** 
 * \brief
 * Spare
 *
 * \details 
 * Field: VTSS_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW11 . dev1_spare_rw11
 */
#define  VTSS_F_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW11_dev1_spare_rw11(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW11_dev1_spare_rw11     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW11_dev1_spare_rw11(x)  VTSS_EXTRACT_BITFIELD(x,0,16)


/** 
 * \brief Device1 Spare R/W 12
 *
 * \details
 * Register: \a HOST_PMA:SPARE_RW_REGISTERS:DEV1_SPARE_RW12
 */
#define VTSS_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW12  VTSS_IOREG(0x09, 0, 0xaefc)

/** 
 * \brief
 * Spare
 *
 * \details 
 * Field: VTSS_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW12 . dev1_spare_rw12
 */
#define  VTSS_F_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW12_dev1_spare_rw12(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW12_dev1_spare_rw12     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW12_dev1_spare_rw12(x)  VTSS_EXTRACT_BITFIELD(x,0,16)


/** 
 * \brief Device1 Spare R/W 13
 *
 * \details
 * Register: \a HOST_PMA:SPARE_RW_REGISTERS:DEV1_SPARE_RW13
 */
#define VTSS_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW13  VTSS_IOREG(0x09, 0, 0xaefd)

/** 
 * \brief
 * Spare
 *
 * \details 
 * Field: VTSS_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW13 . dev1_spare_rw13
 */
#define  VTSS_F_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW13_dev1_spare_rw13(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW13_dev1_spare_rw13     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW13_dev1_spare_rw13(x)  VTSS_EXTRACT_BITFIELD(x,0,16)


/** 
 * \brief Device1 Spare R/W 14
 *
 * \details
 * Register: \a HOST_PMA:SPARE_RW_REGISTERS:DEV1_SPARE_RW14
 */
#define VTSS_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW14  VTSS_IOREG(0x09, 0, 0xaefe)

/** 
 * \brief
 * Spare
 *
 * \details 
 * Field: VTSS_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW14 . dev1_spare_rw14
 */
#define  VTSS_F_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW14_dev1_spare_rw14(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW14_dev1_spare_rw14     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW14_dev1_spare_rw14(x)  VTSS_EXTRACT_BITFIELD(x,0,16)


/** 
 * \brief Device1 Spare R/W 15
 *
 * \details
 * Register: \a HOST_PMA:SPARE_RW_REGISTERS:DEV1_SPARE_RW15
 */
#define VTSS_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW15  VTSS_IOREG(0x09, 0, 0xaeff)

/** 
 * \brief
 * Spare
 *
 * \details 
 * Field: VTSS_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW15 . dev1_spare_rw15
 */
#define  VTSS_F_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW15_dev1_spare_rw15(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW15_dev1_spare_rw15     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_HOST_PMA_SPARE_RW_REGISTERS_DEV1_SPARE_RW15_dev1_spare_rw15(x)  VTSS_EXTRACT_BITFIELD(x,0,16)

/**
 * Register Group: \a HOST_PMA:SD10G65_VSCOPE2
 *
 * SD10G65 VSCOPE Configuration and Status Register set
 */


/** 
 * \brief Vscope main config register A
 *
 * \details
 * Vscope main configuration register A
 *
 * Register: \a HOST_PMA:SD10G65_VSCOPE2:VSCOPE_MAIN_CFG_A
 */
#define VTSS_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_MAIN_CFG_A  VTSS_IOREG(0x09, 0, 0xb000)

/** 
 * \brief
 * Disables writing of synth_phase_aux in synthesizer
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_MAIN_CFG_A . SYN_PHASE_WR_DIS
 */
#define  VTSS_F_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_MAIN_CFG_A_SYN_PHASE_WR_DIS  VTSS_BIT(8)

/** 
 * \brief
 * Disables writing of ib_auxl_offset and ib_auxh_offset in IB
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_MAIN_CFG_A . IB_AUX_OFFS_WR_DIS
 */
#define  VTSS_F_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_MAIN_CFG_A_IB_AUX_OFFS_WR_DIS  VTSS_BIT(7)

/** 
 * \brief
 * Disables writing of ib_jumpl_ena and ib_jumph_ena in IB
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_MAIN_CFG_A . IB_JUMP_ENA_WR_DIS
 */
#define  VTSS_F_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_MAIN_CFG_A_IB_JUMP_ENA_WR_DIS  VTSS_BIT(6)

/** 
 * \brief
 * Counter output selection
 *
 * \details 
 * 0-3: error counter 0-3
 * 4: hit counter
 * 5: clock counter
 * 6: 8 LSBs of error counter 3-1 and hit counter
 * 7: 8 LSBs of error counter 3-0
 *
 * Field: VTSS_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_MAIN_CFG_A . CNT_OUT_SEL
 */
#define  VTSS_F_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_MAIN_CFG_A_CNT_OUT_SEL(x)  VTSS_ENCODE_BITFIELD(x,3,3)
#define  VTSS_M_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_MAIN_CFG_A_CNT_OUT_SEL     VTSS_ENCODE_BITMASK(3,3)
#define  VTSS_X_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_MAIN_CFG_A_CNT_OUT_SEL(x)  VTSS_EXTRACT_BITFIELD(x,3,3)

/** 
 * \brief
 * Comparator input selection
 *
 * \details 
 * [REF] 0
 * 1: auxL
 * 4
 * 5: auxH
 * 2
 * 7: main; [SUB] 5
 * 7: auxL
 * 0
 * 2: auxH
 * 1
 * 4: main (3
 * 6: reserved)
 *
 * Field: VTSS_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_MAIN_CFG_A . COMP_SEL
 */
#define  VTSS_F_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_MAIN_CFG_A_COMP_SEL(x)  VTSS_ENCODE_BITFIELD(x,0,3)
#define  VTSS_M_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_MAIN_CFG_A_COMP_SEL     VTSS_ENCODE_BITMASK(0,3)
#define  VTSS_X_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_MAIN_CFG_A_COMP_SEL(x)  VTSS_EXTRACT_BITFIELD(x,0,3)


/** 
 * \brief Vscope main config register B
 *
 * \details
 * Vscope main configuration register B
 *
 * Register: \a HOST_PMA:SD10G65_VSCOPE2:VSCOPE_MAIN_CFG_B
 */
#define VTSS_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_MAIN_CFG_B  VTSS_IOREG(0x09, 0, 0xb001)

/** 
 * \brief
 * Select GP reg input
 *
 * \details 
 * 0: rx (main)
 *
 * Field: VTSS_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_MAIN_CFG_B . GP_SELECT
 */
#define  VTSS_F_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_MAIN_CFG_B_GP_SELECT(x)  VTSS_ENCODE_BITFIELD(x,8,2)
#define  VTSS_M_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_MAIN_CFG_B_GP_SELECT     VTSS_ENCODE_BITMASK(8,2)
#define  VTSS_X_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_MAIN_CFG_B_GP_SELECT(x)  VTSS_EXTRACT_BITFIELD(x,8,2)

/** 
 * \brief
 * Allows to freeze the GP register value to assure valid reading
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_MAIN_CFG_B . GP_REG_FREEZE
 */
#define  VTSS_F_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_MAIN_CFG_B_GP_REG_FREEZE  VTSS_BIT(7)

/** 
 * \brief
 * Scan limit, selects which counter saturation limits the other counters
 *
 * \details 
 * 0: clock counter
 * 1: hit counter
 * 2: error counters
 * 3: no limit
 *
 * Field: VTSS_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_MAIN_CFG_B . SCAN_LIM
 */
#define  VTSS_F_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_MAIN_CFG_B_SCAN_LIM(x)  VTSS_ENCODE_BITFIELD(x,5,2)
#define  VTSS_M_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_MAIN_CFG_B_SCAN_LIM     VTSS_ENCODE_BITMASK(5,2)
#define  VTSS_X_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_MAIN_CFG_B_SCAN_LIM(x)  VTSS_EXTRACT_BITFIELD(x,5,2)

/** 
 * \brief
 * Preload value for error counter
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_MAIN_CFG_B . PRELOAD_VAL
 */
#define  VTSS_F_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_MAIN_CFG_B_PRELOAD_VAL(x)  VTSS_ENCODE_BITFIELD(x,0,5)
#define  VTSS_M_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_MAIN_CFG_B_PRELOAD_VAL     VTSS_ENCODE_BITMASK(0,5)
#define  VTSS_X_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_MAIN_CFG_B_PRELOAD_VAL(x)  VTSS_EXTRACT_BITFIELD(x,0,5)


/** 
 * \brief Vscope main config register C
 *
 * \details
 * Vscope main configuration register C
 *
 * Register: \a HOST_PMA:SD10G65_VSCOPE2:VSCOPE_MAIN_CFG_C
 */
#define VTSS_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_MAIN_CFG_C  VTSS_IOREG(0x09, 0, 0xb002)

/** 
 * \brief
 * Disable interrupt output
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_MAIN_CFG_C . INTR_DIS
 */
#define  VTSS_F_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_MAIN_CFG_C_INTR_DIS  VTSS_BIT(12)

/** 
 * \brief
 * Enable trigger
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_MAIN_CFG_C . TRIG_ENA
 */
#define  VTSS_F_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_MAIN_CFG_C_TRIG_ENA  VTSS_BIT(11)

/** 
 * \brief
 * Counter enable (bit 4) implicitly done by reading the counter; unused in
 * hw-scan mode
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_MAIN_CFG_C . QUICK_SCAN
 */
#define  VTSS_F_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_MAIN_CFG_C_QUICK_SCAN  VTSS_BIT(10)

/** 
 * \brief
 * Counter period: preload value for clock counter
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_MAIN_CFG_C . COUNT_PER
 */
#define  VTSS_F_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_MAIN_CFG_C_COUNT_PER(x)  VTSS_ENCODE_BITFIELD(x,5,5)
#define  VTSS_M_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_MAIN_CFG_C_COUNT_PER     VTSS_ENCODE_BITMASK(5,5)
#define  VTSS_X_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_MAIN_CFG_C_COUNT_PER(x)  VTSS_EXTRACT_BITFIELD(x,5,5)

/** 
 * \brief
 * Enable Counting; unused in hw-scan mode
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_MAIN_CFG_C . CNT_ENA
 */
#define  VTSS_F_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_MAIN_CFG_C_CNT_ENA  VTSS_BIT(4)

/** 
 * \brief
 * Interface Width
 *
 * \details 
 * 0: 8 bit
 * 1: 10 bit
 * 2: 16 bit
 * 3: 20 bit
 * 4: 32 bit
 * 5: 40 bit
 * others: reserved
 *
 * Field: VTSS_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_MAIN_CFG_C . IF_MODE
 */
#define  VTSS_F_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_MAIN_CFG_C_IF_MODE(x)  VTSS_ENCODE_BITFIELD(x,1,3)
#define  VTSS_M_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_MAIN_CFG_C_IF_MODE     VTSS_ENCODE_BITMASK(1,3)
#define  VTSS_X_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_MAIN_CFG_C_IF_MODE(x)  VTSS_EXTRACT_BITFIELD(x,1,3)

/** 
 * \brief
 * Enable Vscope
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_MAIN_CFG_C . VSCOPE_ENA
 */
#define  VTSS_F_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_MAIN_CFG_C_VSCOPE_ENA  VTSS_BIT(0)


/** 
 * \brief Vscope pattern lock config register A
 *
 * \details
 * Vscope pattern lock configuration register A
 *
 * Register: \a HOST_PMA:SD10G65_VSCOPE2:VSCOPE_PAT_LOCK_CFG_A
 */
#define VTSS_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_PAT_LOCK_CFG_A  VTSS_IOREG(0x09, 0, 0xb003)

/** 
 * \brief
 * Preload value for hit counter
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_PAT_LOCK_CFG_A . PRELOAD_HIT_CNT
 */
#define  VTSS_F_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_PAT_LOCK_CFG_A_PRELOAD_HIT_CNT(x)  VTSS_ENCODE_BITFIELD(x,10,5)
#define  VTSS_M_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_PAT_LOCK_CFG_A_PRELOAD_HIT_CNT     VTSS_ENCODE_BITMASK(10,5)
#define  VTSS_X_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_PAT_LOCK_CFG_A_PRELOAD_HIT_CNT(x)  VTSS_EXTRACT_BITFIELD(x,10,5)

/** 
 * \brief
 * Don't Care mask: Enable history mask usage.
 *
 * \details 
 * 0: enable history mask bit
 * 1: history mask bit is "don't care"
 *
 * Field: VTSS_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_PAT_LOCK_CFG_A . DC_MASK
 */
#define  VTSS_F_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_PAT_LOCK_CFG_A_DC_MASK(x)  VTSS_ENCODE_BITFIELD(x,0,10)
#define  VTSS_M_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_PAT_LOCK_CFG_A_DC_MASK     VTSS_ENCODE_BITMASK(0,10)
#define  VTSS_X_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_PAT_LOCK_CFG_A_DC_MASK(x)  VTSS_EXTRACT_BITFIELD(x,0,10)


/** 
 * \brief Vscope pattern lock config register B
 *
 * \details
 * Vscope pattern lock configuration register B
 *
 * Register: \a HOST_PMA:SD10G65_VSCOPE2:VSCOPE_PAT_LOCK_CFG_B
 */
#define VTSS_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_PAT_LOCK_CFG_B  VTSS_IOREG(0x09, 0, 0xb004)

/** 
 * \brief
 * History mask: Respective sequence is expected in reference input
 * (comp_sel); if enabled (dc_mask) before hit and error counting is
 * enabled
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_PAT_LOCK_CFG_B . HIST_MASK
 */
#define  VTSS_F_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_PAT_LOCK_CFG_B_HIST_MASK(x)  VTSS_ENCODE_BITFIELD(x,0,10)
#define  VTSS_M_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_PAT_LOCK_CFG_B_HIST_MASK     VTSS_ENCODE_BITMASK(0,10)
#define  VTSS_X_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_PAT_LOCK_CFG_B_HIST_MASK(x)  VTSS_EXTRACT_BITFIELD(x,0,10)


/** 
 * \brief Vscope hw scan config register 1A
 *
 * \details
 * Vscope HW scan configuration register 1A
 *
 * Register: \a HOST_PMA:SD10G65_VSCOPE2:VSCOPE_HW_SCAN_CFG_1A
 */
#define VTSS_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_HW_SCAN_CFG_1A  VTSS_IOREG(0x09, 0, 0xb005)

/** 
 * \brief
 * Invert the jumph_ena and jumpl_ena bit in HW scan mode
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_HW_SCAN_CFG_1A . PHASE_JUMP_INV
 */
#define  VTSS_F_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_HW_SCAN_CFG_1A_PHASE_JUMP_INV  VTSS_BIT(13)

/** 
 * \brief
 * Offset between AuxL amplitude (reference) and AuxH amplitude, signed
 * (2s-complement), +- 1/4 amplitude max.
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_HW_SCAN_CFG_1A . AMPL_OFFS_VAL
 */
#define  VTSS_F_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_HW_SCAN_CFG_1A_AMPL_OFFS_VAL(x)  VTSS_ENCODE_BITFIELD(x,8,5)
#define  VTSS_M_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_HW_SCAN_CFG_1A_AMPL_OFFS_VAL     VTSS_ENCODE_BITMASK(8,5)
#define  VTSS_X_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_HW_SCAN_CFG_1A_AMPL_OFFS_VAL(x)  VTSS_EXTRACT_BITFIELD(x,8,5)

/** 
 * \brief
 * Maximum phase increment value before wrapping
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_HW_SCAN_CFG_1A . MAX_PHASE_INCR_VAL
 */
#define  VTSS_F_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_HW_SCAN_CFG_1A_MAX_PHASE_INCR_VAL(x)  VTSS_ENCODE_BITFIELD(x,0,8)
#define  VTSS_M_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_HW_SCAN_CFG_1A_MAX_PHASE_INCR_VAL     VTSS_ENCODE_BITMASK(0,8)
#define  VTSS_X_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_HW_SCAN_CFG_1A_MAX_PHASE_INCR_VAL(x)  VTSS_EXTRACT_BITFIELD(x,0,8)


/** 
 * \brief Vscope hw scan config register 1B
 *
 * \details
 * Vscope HW scan configuration register 1B
 *
 * Register: \a HOST_PMA:SD10G65_VSCOPE2:VSCOPE_HW_SCAN_CFG_1B
 */
#define VTSS_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_HW_SCAN_CFG_1B  VTSS_IOREG(0x09, 0, 0xb006)

/** 
 * \brief
 * Maximum amplitude increment value before wrapping
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_HW_SCAN_CFG_1B . MAX_AMPL_INCR_VAL
 */
#define  VTSS_F_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_HW_SCAN_CFG_1B_MAX_AMPL_INCR_VAL(x)  VTSS_ENCODE_BITFIELD(x,10,6)
#define  VTSS_M_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_HW_SCAN_CFG_1B_MAX_AMPL_INCR_VAL     VTSS_ENCODE_BITMASK(10,6)
#define  VTSS_X_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_HW_SCAN_CFG_1B_MAX_AMPL_INCR_VAL(x)  VTSS_EXTRACT_BITFIELD(x,10,6)

/** 
 * \brief
 * Phase increment per scan step
 *
 * \details 
 * Increment = phase_incr + 1
 *
 * Field: VTSS_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_HW_SCAN_CFG_1B . PHASE_INCR
 */
#define  VTSS_F_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_HW_SCAN_CFG_1B_PHASE_INCR(x)  VTSS_ENCODE_BITFIELD(x,7,3)
#define  VTSS_M_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_HW_SCAN_CFG_1B_PHASE_INCR     VTSS_ENCODE_BITMASK(7,3)
#define  VTSS_X_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_HW_SCAN_CFG_1B_PHASE_INCR(x)  VTSS_EXTRACT_BITFIELD(x,7,3)

/** 
 * \brief
 * Amplitude increment per scan step
 *
 * \details 
 * Increment = ampl_incr + 1
 *
 * Field: VTSS_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_HW_SCAN_CFG_1B . AMPL_INCR
 */
#define  VTSS_F_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_HW_SCAN_CFG_1B_AMPL_INCR(x)  VTSS_ENCODE_BITFIELD(x,4,3)
#define  VTSS_M_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_HW_SCAN_CFG_1B_AMPL_INCR     VTSS_ENCODE_BITMASK(4,3)
#define  VTSS_X_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_HW_SCAN_CFG_1B_AMPL_INCR(x)  VTSS_EXTRACT_BITFIELD(x,4,3)

/** 
 * \brief
 * Number of scans per iteration in N-point-scan mode
 *
 * \details 
 * 0: 1
 * 1: 2
 * 2: 4
 * 3: 8
 *
 * Field: VTSS_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_HW_SCAN_CFG_1B . NUM_SCANS_PER_ITR
 */
#define  VTSS_F_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_HW_SCAN_CFG_1B_NUM_SCANS_PER_ITR(x)  VTSS_ENCODE_BITFIELD(x,2,2)
#define  VTSS_M_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_HW_SCAN_CFG_1B_NUM_SCANS_PER_ITR     VTSS_ENCODE_BITMASK(2,2)
#define  VTSS_X_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_HW_SCAN_CFG_1B_NUM_SCANS_PER_ITR(x)  VTSS_EXTRACT_BITFIELD(x,2,2)

/** 
 * \brief
 * Enables HW scan with N results per scan or fast-scan
 *
 * \details 
 * 0: off
 * 1: N-point scan
 * 2: fast-scan (sq)
 * 3: fast-scan (diag)
 *
 * Field: VTSS_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_HW_SCAN_CFG_1B . HW_SCAN_ENA
 */
#define  VTSS_F_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_HW_SCAN_CFG_1B_HW_SCAN_ENA(x)  VTSS_ENCODE_BITFIELD(x,0,2)
#define  VTSS_M_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_HW_SCAN_CFG_1B_HW_SCAN_ENA     VTSS_ENCODE_BITMASK(0,2)
#define  VTSS_X_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_HW_SCAN_CFG_1B_HW_SCAN_ENA(x)  VTSS_EXTRACT_BITFIELD(x,0,2)


/** 
 * \brief Vscope hw config register 2A
 *
 * \details
 * Vscope HW scan configuration register 2A
 *
 * Register: \a HOST_PMA:SD10G65_VSCOPE2:VSCOPE_HW_SCAN_CFG_2A
 */
#define VTSS_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_HW_SCAN_CFG_2A  VTSS_IOREG(0x09, 0, 0xb007)

/** 
 * \brief
 * Threshold for error_counter in fast-scan mode
 *
 * \details 
 * N+1
 *
 * Field: VTSS_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_HW_SCAN_CFG_2A . FAST_SCAN_THRES
 */
#define  VTSS_F_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_HW_SCAN_CFG_2A_FAST_SCAN_THRES(x)  VTSS_ENCODE_BITFIELD(x,13,3)
#define  VTSS_M_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_HW_SCAN_CFG_2A_FAST_SCAN_THRES     VTSS_ENCODE_BITMASK(13,3)
#define  VTSS_X_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_HW_SCAN_CFG_2A_FAST_SCAN_THRES(x)  VTSS_EXTRACT_BITFIELD(x,13,3)

/** 
 * \brief
 * Left shift for threshold of error_counter in fast-scan mode
 *
 * \details 
 * threshold = (fast_scan_thres+1) shift_left fs_thres_shift
 *
 * Field: VTSS_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_HW_SCAN_CFG_2A . FS_THRES_SHIFT
 */
#define  VTSS_F_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_HW_SCAN_CFG_2A_FS_THRES_SHIFT(x)  VTSS_ENCODE_BITFIELD(x,8,5)
#define  VTSS_M_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_HW_SCAN_CFG_2A_FS_THRES_SHIFT     VTSS_ENCODE_BITMASK(8,5)
#define  VTSS_X_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_HW_SCAN_CFG_2A_FS_THRES_SHIFT(x)  VTSS_EXTRACT_BITFIELD(x,8,5)

/** 
 * \brief
 * Value at which jumpl_ena and jumph_ena in IB must be toggled
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_HW_SCAN_CFG_2A . PHASE_JUMP_VAL
 */
#define  VTSS_F_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_HW_SCAN_CFG_2A_PHASE_JUMP_VAL(x)  VTSS_ENCODE_BITFIELD(x,0,8)
#define  VTSS_M_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_HW_SCAN_CFG_2A_PHASE_JUMP_VAL     VTSS_ENCODE_BITMASK(0,8)
#define  VTSS_X_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_HW_SCAN_CFG_2A_PHASE_JUMP_VAL(x)  VTSS_EXTRACT_BITFIELD(x,0,8)


/** 
 * \brief Vscope hw config register 2B
 *
 * \details
 * Vscope HW scan configuration register 2B
 *
 * Register: \a HOST_PMA:SD10G65_VSCOPE2:VSCOPE_HW_SCAN_CFG_2B
 */
#define VTSS_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_HW_SCAN_CFG_2B  VTSS_IOREG(0x09, 0, 0xb008)

/** 
 * \brief
 * Disable IB amplitude symmetry compensation for AuxH and AuxL
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_HW_SCAN_CFG_2B . AUX_AMPL_SYM_DIS
 */
#define  VTSS_F_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_HW_SCAN_CFG_2B_AUX_AMPL_SYM_DIS  VTSS_BIT(15)

/** 
 * \brief
 * Start value for VScope amplitude in N-point-scan mode and fast-scan mode
 * (before IB amplitude symmetry compensation)
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_HW_SCAN_CFG_2B . AMPL_START_VAL
 */
#define  VTSS_F_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_HW_SCAN_CFG_2B_AMPL_START_VAL(x)  VTSS_ENCODE_BITFIELD(x,8,6)
#define  VTSS_M_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_HW_SCAN_CFG_2B_AMPL_START_VAL     VTSS_ENCODE_BITMASK(8,6)
#define  VTSS_X_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_HW_SCAN_CFG_2B_AMPL_START_VAL(x)  VTSS_EXTRACT_BITFIELD(x,8,6)

/** 
 * \brief
 * Start value for VScope phase in N-point-scan mode and fast-scan mode
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_HW_SCAN_CFG_2B . PHASE_START_VAL
 */
#define  VTSS_F_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_HW_SCAN_CFG_2B_PHASE_START_VAL(x)  VTSS_ENCODE_BITFIELD(x,0,8)
#define  VTSS_M_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_HW_SCAN_CFG_2B_PHASE_START_VAL     VTSS_ENCODE_BITMASK(0,8)
#define  VTSS_X_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_HW_SCAN_CFG_2B_PHASE_START_VAL(x)  VTSS_EXTRACT_BITFIELD(x,0,8)


/** 
 * \brief Vscope status register
 *
 * \details
 * Vscope status register
 *
 * Register: \a HOST_PMA:SD10G65_VSCOPE2:VSCOPE_STAT
 */
#define VTSS_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_STAT  VTSS_IOREG(0x09, 0, 0xb009)

/** 
 * \brief
 * 8 MSBs of general purpose register
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_STAT . GP_REG_MSB
 */
#define  VTSS_F_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_STAT_GP_REG_MSB(x)  VTSS_ENCODE_BITFIELD(x,8,8)
#define  VTSS_M_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_STAT_GP_REG_MSB     VTSS_ENCODE_BITMASK(8,8)
#define  VTSS_X_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_STAT_GP_REG_MSB(x)  VTSS_EXTRACT_BITFIELD(x,8,8)

/** 
 * \brief
 * Fast scan mode: Indicator per cursor position whether threshold was
 * reached
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_STAT . FAST_SCAN_HIT
 */
#define  VTSS_F_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_STAT_FAST_SCAN_HIT(x)  VTSS_ENCODE_BITFIELD(x,4,4)
#define  VTSS_M_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_STAT_FAST_SCAN_HIT     VTSS_ENCODE_BITMASK(4,4)
#define  VTSS_X_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_STAT_FAST_SCAN_HIT(x)  VTSS_EXTRACT_BITFIELD(x,4,4)

/** 
 * \brief
 * Done sticky
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_STAT . DONE_STICKY
 */
#define  VTSS_F_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_STAT_DONE_STICKY  VTSS_BIT(0)


/** 
 * \brief Vscope counter register A
 *
 * \details
 * Vscope counter register A
 *
 * Register: \a HOST_PMA:SD10G65_VSCOPE2:VSCOPE_CNT_A
 */
#define VTSS_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_CNT_A  VTSS_IOREG(0x09, 0, 0xb00a)

/** 
 * \brief
 * Counter value higher 16-bit MSB [31:16]
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_CNT_A . COUNTER_MSB
 */
#define  VTSS_F_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_CNT_A_COUNTER_MSB(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_CNT_A_COUNTER_MSB     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_CNT_A_COUNTER_MSB(x)  VTSS_EXTRACT_BITFIELD(x,0,16)


/** 
 * \brief Vscope counter register B
 *
 * \details
 * Vscope counter register B
 *
 * Register: \a HOST_PMA:SD10G65_VSCOPE2:VSCOPE_CNT_B
 */
#define VTSS_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_CNT_B  VTSS_IOREG(0x09, 0, 0xb00b)

/** 
 * \brief
 * Counter value lower 16-bit LSB [15:0]
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_CNT_B . COUNTER_LSB
 */
#define  VTSS_F_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_CNT_B_COUNTER_LSB(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_CNT_B_COUNTER_LSB     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_CNT_B_COUNTER_LSB(x)  VTSS_EXTRACT_BITFIELD(x,0,16)


/** 
 * \brief Vscope general purpose register A
 *
 * \details
 * Vscope general purpose  register A
 *
 * Register: \a HOST_PMA:SD10G65_VSCOPE2:VSCOPE_DBG_LSB_A
 */
#define VTSS_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_DBG_LSB_A  VTSS_IOREG(0x09, 0, 0xb00c)

/** 
 * \brief
 * 16 bit MSB of a 32 bit general purpose register [31:16]
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_DBG_LSB_A . GP_REG_LSB_A
 */
#define  VTSS_F_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_DBG_LSB_A_GP_REG_LSB_A(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_DBG_LSB_A_GP_REG_LSB_A     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_DBG_LSB_A_GP_REG_LSB_A(x)  VTSS_EXTRACT_BITFIELD(x,0,16)


/** 
 * \brief Vscope general purpose register A
 *
 * \details
 * Vscope general purpose  register B
 *
 * Register: \a HOST_PMA:SD10G65_VSCOPE2:VSCOPE_DBG_LSB_B
 */
#define VTSS_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_DBG_LSB_B  VTSS_IOREG(0x09, 0, 0xb00d)

/** 
 * \brief
 * 16 bit LSB of a 32 bit general purpose register [15:0]
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_DBG_LSB_B . GP_REG_LSB_B
 */
#define  VTSS_F_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_DBG_LSB_B_GP_REG_LSB_B(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_DBG_LSB_B_GP_REG_LSB_B     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_HOST_PMA_SD10G65_VSCOPE2_VSCOPE_DBG_LSB_B_GP_REG_LSB_B(x)  VTSS_EXTRACT_BITFIELD(x,0,16)

/**
 * Register Group: \a HOST_PMA:SD10G65_DFT
 *
 * SD10G65 DFT Configuration and Status Register set
 */


/** 
 * \brief SD10G65 DFT Main configuration register 1
 *
 * \details
 * Main configuration register 1 for SD10G65 DFT.
 *
 * Register: \a HOST_PMA:SD10G65_DFT:DFT_RX_CFG_1
 */
#define VTSS_HOST_PMA_SD10G65_DFT_DFT_RX_CFG_1  VTSS_IOREG(0x09, 0, 0xb100)

/** 
 * \brief
 * Disables error generation based on stuck_at_par errors,
 *
 * \details 
 * 0: stuck_at_par error generates 63 errors per clock cycle (in PRBS mode
 * only)
 * 1: stuck_at_par error does not generate errors
 *
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_RX_CFG_1 . STUCK_AT_PAR_MASK_CFG
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_RX_CFG_1_STUCK_AT_PAR_MASK_CFG  VTSS_BIT(12)

/** 
 * \brief
 * Disables error generation based on stuck_at_01 errors,
 *
 * \details 
 * 0: stuck_at_01 error generates 63 errors per clock cycle (in PRBS mode
 * only)
 * 1: stuck_at_01 error does not generate errors
 *
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_RX_CFG_1 . STUCK_AT_01_MASK_CFG
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_RX_CFG_1_STUCK_AT_01_MASK_CFG  VTSS_BIT(11)

/** 
 * \brief
 * Enables data through from gearbox to gearbox
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_RX_CFG_1 . DIRECT_THROUGH_ENA_CFG
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_RX_CFG_1_DIRECT_THROUGH_ENA_CFG  VTSS_BIT(10)

/** 
 * \brief
 * Captures data from error counter to allow reading of stable data
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_RX_CFG_1 . ERR_CNT_CAPT_CFG
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_RX_CFG_1_ERR_CNT_CAPT_CFG  VTSS_BIT(9)

/** 
 * \brief
 * Data source selection
 *
 * \details 
 * 0: main path
 * 1: vscope high path
 * 2: vscope low path
 *
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_RX_CFG_1 . RX_DATA_SRC_SEL
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_RX_CFG_1_RX_DATA_SRC_SEL(x)  VTSS_ENCODE_BITFIELD(x,7,2)
#define  VTSS_M_HOST_PMA_SD10G65_DFT_DFT_RX_CFG_1_RX_DATA_SRC_SEL     VTSS_ENCODE_BITMASK(7,2)
#define  VTSS_X_HOST_PMA_SD10G65_DFT_DFT_RX_CFG_1_RX_DATA_SRC_SEL(x)  VTSS_EXTRACT_BITFIELD(x,7,2)

/** 
 * \brief
 * States in which error counting is enabled
 *
 * \details 
 * 3:all but IDLE; 2:check 1:stable+check
 * 0:wait_stable+stable+check
 *
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_RX_CFG_1 . BIST_CNT_CFG
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_RX_CFG_1_BIST_CNT_CFG(x)  VTSS_ENCODE_BITFIELD(x,5,2)
#define  VTSS_M_HOST_PMA_SD10G65_DFT_DFT_RX_CFG_1_BIST_CNT_CFG     VTSS_ENCODE_BITMASK(5,2)
#define  VTSS_X_HOST_PMA_SD10G65_DFT_DFT_RX_CFG_1_BIST_CNT_CFG(x)  VTSS_EXTRACT_BITFIELD(x,5,2)

/** 
 * \brief
 * Disable change of stored patterns (e.g. to avoid changes during
 * read-out)
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_RX_CFG_1 . FREEZE_PATTERN_CFG
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_RX_CFG_1_FREEZE_PATTERN_CFG  VTSS_BIT(4)

/** 
 * \brief
 * Selects pattern to check
 *
 * \details 
 * 0: PRBS pattern
 * 1: constant pattern
 *
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_RX_CFG_1 . CHK_MODE_CFG
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_RX_CFG_1_CHK_MODE_CFG  VTSS_BIT(3)

/** 
 * \brief
 * Selects DES interface width
 *
 * \details 
 * 0:8
 * 1:10
 * 2:16
 * 3:20
 * 4:32
 * 5:40 (default)
 *
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_RX_CFG_1 . RX_WID_SEL_CFG
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_RX_CFG_1_RX_WID_SEL_CFG(x)  VTSS_ENCODE_BITFIELD(x,0,3)
#define  VTSS_M_HOST_PMA_SD10G65_DFT_DFT_RX_CFG_1_RX_WID_SEL_CFG     VTSS_ENCODE_BITMASK(0,3)
#define  VTSS_X_HOST_PMA_SD10G65_DFT_DFT_RX_CFG_1_RX_WID_SEL_CFG(x)  VTSS_EXTRACT_BITFIELD(x,0,3)


/** 
 * \brief SD10G65 DFT Main configuration register 2
 *
 * \details
 * Main configuration register 2 for SD10G65 DFT.
 *
 * Register: \a HOST_PMA:SD10G65_DFT:DFT_RX_CFG_2
 */
#define VTSS_HOST_PMA_SD10G65_DFT_DFT_RX_CFG_2  VTSS_IOREG(0x09, 0, 0xb101)

/** 
 * \brief
 * Pattern generator: 0:bytes mode; 1:10-bits word mode
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_RX_CFG_2 . RX_WORD_MODE_CFG
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_RX_CFG_2_RX_WORD_MODE_CFG  VTSS_BIT(14)

/** 
 * \brief
 * Selects PRBS check
 *
 * \details 
 * 0: prbs7
 * 1: prbs15
 * 2: prbs23
 * 3: prbs11
 * 4: prbs31 (default)
 * 5: prbs9
 *
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_RX_CFG_2 . RX_PRBS_SEL_CFG
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_RX_CFG_2_RX_PRBS_SEL_CFG(x)  VTSS_ENCODE_BITFIELD(x,11,3)
#define  VTSS_M_HOST_PMA_SD10G65_DFT_DFT_RX_CFG_2_RX_PRBS_SEL_CFG     VTSS_ENCODE_BITMASK(11,3)
#define  VTSS_X_HOST_PMA_SD10G65_DFT_DFT_RX_CFG_2_RX_PRBS_SEL_CFG(x)  VTSS_EXTRACT_BITFIELD(x,11,3)

/** 
 * \brief
 * Enables PRBS checker input inversion
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_RX_CFG_2 . INV_ENA_CFG
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_RX_CFG_2_INV_ENA_CFG  VTSS_BIT(10)

/** 
 * \brief
 * Selects compare mode
 *
 * \details 
 * 0: compare mode possible
 * 1 learn mode is forced
 *
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_RX_CFG_2 . CMP_MODE_CFG
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_RX_CFG_2_CMP_MODE_CFG  VTSS_BIT(9)

/** 
 * \brief
 * Number of consecutive errors/non-errors before transitioning to
 * respective state
 *
 * \details 
 * value = num-40-bits-words + 1
 *
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_RX_CFG_2 . LRN_CNT_CFG
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_RX_CFG_2_LRN_CNT_CFG(x)  VTSS_ENCODE_BITFIELD(x,6,3)
#define  VTSS_M_HOST_PMA_SD10G65_DFT_DFT_RX_CFG_2_LRN_CNT_CFG     VTSS_ENCODE_BITMASK(6,3)
#define  VTSS_X_HOST_PMA_SD10G65_DFT_DFT_RX_CFG_2_LRN_CNT_CFG(x)  VTSS_EXTRACT_BITFIELD(x,6,3)

/** 
 * \brief
 * SW reset of error counter; rising edge activates reset
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_RX_CFG_2 . CNT_RST
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_RX_CFG_2_CNT_RST  VTSS_BIT(5)

/** 
 * \brief
 * Selects modes in which error counter is active
 *
 * \details 
 * 0:learn and compare mode
 * 1:transition between modes
 * 2:learn mode
 * 3:compare mode
 *
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_RX_CFG_2 . CNT_CFG
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_RX_CFG_2_CNT_CFG(x)  VTSS_ENCODE_BITFIELD(x,3,2)
#define  VTSS_M_HOST_PMA_SD10G65_DFT_DFT_RX_CFG_2_CNT_CFG     VTSS_ENCODE_BITMASK(3,2)
#define  VTSS_X_HOST_PMA_SD10G65_DFT_DFT_RX_CFG_2_CNT_CFG(x)  VTSS_EXTRACT_BITFIELD(x,3,2)

/** 
 * \brief
 * BIST mode
 *
 * \details 
 * 0: off
 * 1: BIST
 * 2: BER
 * 3:CONT (infinite mode)
 *
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_RX_CFG_2 . BIST_MODE_CFG
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_RX_CFG_2_BIST_MODE_CFG(x)  VTSS_ENCODE_BITFIELD(x,1,2)
#define  VTSS_M_HOST_PMA_SD10G65_DFT_DFT_RX_CFG_2_BIST_MODE_CFG     VTSS_ENCODE_BITMASK(1,2)
#define  VTSS_X_HOST_PMA_SD10G65_DFT_DFT_RX_CFG_2_BIST_MODE_CFG(x)  VTSS_EXTRACT_BITFIELD(x,1,2)

/** 
 * \brief
 * Enable RX DFT capability
 *
 * \details 
 * 0: Disable DFT
 * 1: Enable DFT
 *
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_RX_CFG_2 . DFT_RX_ENA
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_RX_CFG_2_DFT_RX_ENA  VTSS_BIT(0)


/** 
 * \brief SD10G65 DFT pattern mask configuration register 1 
 *
 * \details
 * Configuration register 1 for SD10G65 DFT to mask data bits preventing
 * error counting for these bits.
 *
 * Register: \a HOST_PMA:SD10G65_DFT:DFT_RX_MASK_CFG_1
 */
#define VTSS_HOST_PMA_SD10G65_DFT_DFT_RX_MASK_CFG_1  VTSS_IOREG(0x09, 0, 0xb102)

/** 
 * \brief
 * Mask out (active high) errors in 16 bit MSB data bits [31:16]
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_RX_MASK_CFG_1 . LSB_MASK_CFG_1
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_RX_MASK_CFG_1_LSB_MASK_CFG_1(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_HOST_PMA_SD10G65_DFT_DFT_RX_MASK_CFG_1_LSB_MASK_CFG_1     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_HOST_PMA_SD10G65_DFT_DFT_RX_MASK_CFG_1_LSB_MASK_CFG_1(x)  VTSS_EXTRACT_BITFIELD(x,0,16)


/** 
 * \brief SD10G65 DFT pattern mask configuration register 2
 *
 * \details
 * Configuration register 2 for SD10G65 DFT to mask data bits preventing
 * error counting for these bits.
 *
 * Register: \a HOST_PMA:SD10G65_DFT:DFT_RX_MASK_CFG_2
 */
#define VTSS_HOST_PMA_SD10G65_DFT_DFT_RX_MASK_CFG_2  VTSS_IOREG(0x09, 0, 0xb103)

/** 
 * \brief
 * Mask out (active high) errors in 16 LSB data bits [15:0]
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_RX_MASK_CFG_2 . LSB_MASK_CFG_2
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_RX_MASK_CFG_2_LSB_MASK_CFG_2(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_HOST_PMA_SD10G65_DFT_DFT_RX_MASK_CFG_2_LSB_MASK_CFG_2     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_HOST_PMA_SD10G65_DFT_DFT_RX_MASK_CFG_2_LSB_MASK_CFG_2(x)  VTSS_EXTRACT_BITFIELD(x,0,16)


/** 
 * \brief SD10G65 DFT Pattern checker configuration register 1
 *
 * \details
 * Pattern checker configuration register 1 for SD10G65 DFT.
 *
 * Register: \a HOST_PMA:SD10G65_DFT:DFT_RX_PAT_CFG_1
 */
#define VTSS_HOST_PMA_SD10G65_DFT_DFT_RX_PAT_CFG_1  VTSS_IOREG(0x09, 0, 0xb104)

/** 
 * \brief
 * Mask out (active high) errors in 8 MSB data bits
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_RX_PAT_CFG_1 . MSB_MASK_CFG
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_RX_PAT_CFG_1_MSB_MASK_CFG(x)  VTSS_ENCODE_BITFIELD(x,8,8)
#define  VTSS_M_HOST_PMA_SD10G65_DFT_DFT_RX_PAT_CFG_1_MSB_MASK_CFG     VTSS_ENCODE_BITMASK(8,8)
#define  VTSS_X_HOST_PMA_SD10G65_DFT_DFT_RX_PAT_CFG_1_MSB_MASK_CFG(x)  VTSS_EXTRACT_BITFIELD(x,8,8)

/** 
 * \brief
 * Pattern read enable
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_RX_PAT_CFG_1 . PAT_READ_CFG
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_RX_PAT_CFG_1_PAT_READ_CFG  VTSS_BIT(0)


/** 
 * \brief SD10G65 DFT Pattern checker configuration register 2
 *
 * \details
 * Pattern checker configuration register 2 for SD10G65 DFT.
 *
 * Register: \a HOST_PMA:SD10G65_DFT:DFT_RX_PAT_CFG_2
 */
#define VTSS_HOST_PMA_SD10G65_DFT_DFT_RX_PAT_CFG_2  VTSS_IOREG(0x09, 0, 0xb105)

/** 
 * \brief
 * Maximum address in Checker (before continuing with address 0)
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_RX_PAT_CFG_2 . MAX_ADDR_CHK_CFG
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_RX_PAT_CFG_2_MAX_ADDR_CHK_CFG(x)  VTSS_ENCODE_BITFIELD(x,8,4)
#define  VTSS_M_HOST_PMA_SD10G65_DFT_DFT_RX_PAT_CFG_2_MAX_ADDR_CHK_CFG     VTSS_ENCODE_BITMASK(8,4)
#define  VTSS_X_HOST_PMA_SD10G65_DFT_DFT_RX_PAT_CFG_2_MAX_ADDR_CHK_CFG(x)  VTSS_EXTRACT_BITFIELD(x,8,4)

/** 
 * \brief
 * Address to read patterns from used by SW
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_RX_PAT_CFG_2 . READ_ADDR_CFG
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_RX_PAT_CFG_2_READ_ADDR_CFG(x)  VTSS_ENCODE_BITFIELD(x,0,4)
#define  VTSS_M_HOST_PMA_SD10G65_DFT_DFT_RX_PAT_CFG_2_READ_ADDR_CFG     VTSS_ENCODE_BITMASK(0,4)
#define  VTSS_X_HOST_PMA_SD10G65_DFT_DFT_RX_PAT_CFG_2_READ_ADDR_CFG(x)  VTSS_EXTRACT_BITFIELD(x,0,4)


/** 
 * \brief SD10G65 DFT BIST configuration register A
 *
 * \details
 * BIST configuration register A for SD10G65 DFT controlling 'check and
 * wait-stable' mode.
 *
 * Register: \a HOST_PMA:SD10G65_DFT:DFT_BIST_CFG0A
 */
#define VTSS_HOST_PMA_SD10G65_DFT_DFT_BIST_CFG0A  VTSS_IOREG(0x09, 0, 0xb106)

/** 
 * \brief
 * BIST FSM: threshold to leave DOZE state
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_BIST_CFG0A . WAKEUP_DLY_CFG
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_BIST_CFG0A_WAKEUP_DLY_CFG(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_HOST_PMA_SD10G65_DFT_DFT_BIST_CFG0A_WAKEUP_DLY_CFG     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_HOST_PMA_SD10G65_DFT_DFT_BIST_CFG0A_WAKEUP_DLY_CFG(x)  VTSS_EXTRACT_BITFIELD(x,0,16)


/** 
 * \brief SD10G65 DFT BIST configuration register B
 *
 * \details
 * BIST configuration register B for SD10G65 DFT controlling 'check and
 * wait-stable' mode.
 *
 * Register: \a HOST_PMA:SD10G65_DFT:DFT_BIST_CFG0B
 */
#define VTSS_HOST_PMA_SD10G65_DFT_DFT_BIST_CFG0B  VTSS_IOREG(0x09, 0, 0xb107)

/** 
 * \brief
 * BIST FSM: threshold to enter FINISHED state
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_BIST_CFG0B . MAX_BIST_FRAMES_CFG
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_BIST_CFG0B_MAX_BIST_FRAMES_CFG(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_HOST_PMA_SD10G65_DFT_DFT_BIST_CFG0B_MAX_BIST_FRAMES_CFG     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_HOST_PMA_SD10G65_DFT_DFT_BIST_CFG0B_MAX_BIST_FRAMES_CFG(x)  VTSS_EXTRACT_BITFIELD(x,0,16)


/** 
 * \brief SD10G65 DFT BIST configuration register A
 *
 * \details
 * BIST configuration register A for SD10G65 DFT  controlling 'stable'
 * mode.
 *
 * Register: \a HOST_PMA:SD10G65_DFT:DFT_BIST_CFG1A
 */
#define VTSS_HOST_PMA_SD10G65_DFT_DFT_BIST_CFG1A  VTSS_IOREG(0x09, 0, 0xb108)

/** 
 * \brief
 * BIST FSM: threshold to iterate counter for max_stable_attempts
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_BIST_CFG1A . MAX_UNSTABLE_CYC_CFG
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_BIST_CFG1A_MAX_UNSTABLE_CYC_CFG(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_HOST_PMA_SD10G65_DFT_DFT_BIST_CFG1A_MAX_UNSTABLE_CYC_CFG     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_HOST_PMA_SD10G65_DFT_DFT_BIST_CFG1A_MAX_UNSTABLE_CYC_CFG(x)  VTSS_EXTRACT_BITFIELD(x,0,16)


/** 
 * \brief SD10G65 DFT BIST configuration register B
 *
 * \details
 * BIST configuration register B for SD10G65 DFT  controlling 'stable'
 * mode.
 *
 * Register: \a HOST_PMA:SD10G65_DFT:DFT_BIST_CFG1B
 */
#define VTSS_HOST_PMA_SD10G65_DFT_DFT_BIST_CFG1B  VTSS_IOREG(0x09, 0, 0xb109)

/** 
 * \brief
 * BIST FSM: threshold to enter CHECK state
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_BIST_CFG1B . STABLE_THRES_CFG
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_BIST_CFG1B_STABLE_THRES_CFG(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_HOST_PMA_SD10G65_DFT_DFT_BIST_CFG1B_STABLE_THRES_CFG     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_HOST_PMA_SD10G65_DFT_DFT_BIST_CFG1B_STABLE_THRES_CFG(x)  VTSS_EXTRACT_BITFIELD(x,0,16)


/** 
 * \brief SD10G65 DFT BIST configuration register A
 *
 * \details
 * BIST configuration register B for SD10G65 DFT controlling frame length
 * in 'check' mode.
 *
 * Register: \a HOST_PMA:SD10G65_DFT:DFT_BIST_CFG2A
 */
#define VTSS_HOST_PMA_SD10G65_DFT_DFT_BIST_CFG2A  VTSS_IOREG(0x09, 0, 0xb10a)

/** 
 * \brief
 * BIST FSM: threshold to iterate counter for max_bist_frames [31:16]
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_BIST_CFG2A . FRAME_LEN_CFG_MSB
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_BIST_CFG2A_FRAME_LEN_CFG_MSB(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_HOST_PMA_SD10G65_DFT_DFT_BIST_CFG2A_FRAME_LEN_CFG_MSB     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_HOST_PMA_SD10G65_DFT_DFT_BIST_CFG2A_FRAME_LEN_CFG_MSB(x)  VTSS_EXTRACT_BITFIELD(x,0,16)


/** 
 * \brief SD10G65 DFT BIST configuration register B
 *
 * \details
 * BIST configuration register B for SD10G65 DFT controlling frame length
 * in 'check' mode.
 *
 * Register: \a HOST_PMA:SD10G65_DFT:DFT_BIST_CFG2B
 */
#define VTSS_HOST_PMA_SD10G65_DFT_DFT_BIST_CFG2B  VTSS_IOREG(0x09, 0, 0xb10b)

/** 
 * \brief
 * BIST FSM: threshold to iterate counter for max_bist_frames [15:0]
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_BIST_CFG2B . FRAME_LEN_CFG_LSB
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_BIST_CFG2B_FRAME_LEN_CFG_LSB(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_HOST_PMA_SD10G65_DFT_DFT_BIST_CFG2B_FRAME_LEN_CFG_LSB     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_HOST_PMA_SD10G65_DFT_DFT_BIST_CFG2B_FRAME_LEN_CFG_LSB(x)  VTSS_EXTRACT_BITFIELD(x,0,16)


/** 
 * \brief SD10G65 DFT BIST configuration register A
 *
 * \details
 * BIST configuration register A for SD10G65 DFT controlling stable
 * attempts in ' wait-stable' mode.
 *
 * Register: \a HOST_PMA:SD10G65_DFT:DFT_BIST_CFG3A
 */
#define VTSS_HOST_PMA_SD10G65_DFT_DFT_BIST_CFG3A  VTSS_IOREG(0x09, 0, 0xb10c)

/** 
 * \brief
 * BIST FSM: threshold to enter SYNC_ERR state [31:16]
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_BIST_CFG3A . MAX_STABLE_ATTEMPTS_CFG_MSB
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_BIST_CFG3A_MAX_STABLE_ATTEMPTS_CFG_MSB(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_HOST_PMA_SD10G65_DFT_DFT_BIST_CFG3A_MAX_STABLE_ATTEMPTS_CFG_MSB     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_HOST_PMA_SD10G65_DFT_DFT_BIST_CFG3A_MAX_STABLE_ATTEMPTS_CFG_MSB(x)  VTSS_EXTRACT_BITFIELD(x,0,16)


/** 
 * \brief SD10G65 DFT BIST configuration register B
 *
 * \details
 * BIST configuration register B for SD10G65 DFT controlling stable
 * attempts in ' wait-stable' mode.
 *
 * Register: \a HOST_PMA:SD10G65_DFT:DFT_BIST_CFG3B
 */
#define VTSS_HOST_PMA_SD10G65_DFT_DFT_BIST_CFG3B  VTSS_IOREG(0x09, 0, 0xb10d)

/** 
 * \brief
 * BIST FSM: threshold to enter SYNC_ERR state [15:0]
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_BIST_CFG3B . MAX_STABLE_ATTEMPTS_CFG_LSB
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_BIST_CFG3B_MAX_STABLE_ATTEMPTS_CFG_LSB(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_HOST_PMA_SD10G65_DFT_DFT_BIST_CFG3B_MAX_STABLE_ATTEMPTS_CFG_LSB     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_HOST_PMA_SD10G65_DFT_DFT_BIST_CFG3B_MAX_STABLE_ATTEMPTS_CFG_LSB(x)  VTSS_EXTRACT_BITFIELD(x,0,16)


/** 
 * \brief SD10G65 DFT error status register 1
 *
 * \details
 * Status register 1 for SD10G65 DFT containing the error counter value
 *
 * Register: \a HOST_PMA:SD10G65_DFT:DFT_ERR_STAT_1
 */
#define VTSS_HOST_PMA_SD10G65_DFT_DFT_ERR_STAT_1  VTSS_IOREG(0x09, 0, 0xb10e)

/** 
 * \brief
 * Counter output depending on cnt_cfg [31:16]
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_ERR_STAT_1 . ERR_CNT_MSB
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_ERR_STAT_1_ERR_CNT_MSB(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_HOST_PMA_SD10G65_DFT_DFT_ERR_STAT_1_ERR_CNT_MSB     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_HOST_PMA_SD10G65_DFT_DFT_ERR_STAT_1_ERR_CNT_MSB(x)  VTSS_EXTRACT_BITFIELD(x,0,16)


/** 
 * \brief SD10G65 DFT error status register 2
 *
 * \details
 * Status register B2 for SD10G65 DFT containing the error counter value
 *
 * Register: \a HOST_PMA:SD10G65_DFT:DFT_ERR_STAT_2
 */
#define VTSS_HOST_PMA_SD10G65_DFT_DFT_ERR_STAT_2  VTSS_IOREG(0x09, 0, 0xb10f)

/** 
 * \brief
 * Counter output depending on cnt_cfg [15:0]
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_ERR_STAT_2 . ERR_CNT_LSB
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_ERR_STAT_2_ERR_CNT_LSB(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_HOST_PMA_SD10G65_DFT_DFT_ERR_STAT_2_ERR_CNT_LSB     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_HOST_PMA_SD10G65_DFT_DFT_ERR_STAT_2_ERR_CNT_LSB(x)  VTSS_EXTRACT_BITFIELD(x,0,16)


/** 
 * \brief SD10G65 DFT PRBS status register 1
 *
 * \details
 * Status register 1 for SD10G65 DFT containing the PRBS data related to
 * 1st sync lost event
 *
 * Register: \a HOST_PMA:SD10G65_DFT:DFT_PRBS_STAT_1
 */
#define VTSS_HOST_PMA_SD10G65_DFT_DFT_PRBS_STAT_1  VTSS_IOREG(0x09, 0, 0xb110)

/** 
 * \brief
 * PRBS data after first sync lost [31:16]
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_PRBS_STAT_1 . PRBS_DATA_STAT_MSB
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_PRBS_STAT_1_PRBS_DATA_STAT_MSB(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_HOST_PMA_SD10G65_DFT_DFT_PRBS_STAT_1_PRBS_DATA_STAT_MSB     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_HOST_PMA_SD10G65_DFT_DFT_PRBS_STAT_1_PRBS_DATA_STAT_MSB(x)  VTSS_EXTRACT_BITFIELD(x,0,16)


/** 
 * \brief SD10G65 DFT PRBS status register 2
 *
 * \details
 * Status register 2 for SD10G65 DFT containing the PRBS data related to
 * 1st sync lost event
 *
 * Register: \a HOST_PMA:SD10G65_DFT:DFT_PRBS_STAT_2
 */
#define VTSS_HOST_PMA_SD10G65_DFT_DFT_PRBS_STAT_2  VTSS_IOREG(0x09, 0, 0xb111)

/** 
 * \brief
 * PRBS data after first sync lost [15:0]
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_PRBS_STAT_2 . PRBS_DATA_STAT_LSB
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_PRBS_STAT_2_PRBS_DATA_STAT_LSB(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_HOST_PMA_SD10G65_DFT_DFT_PRBS_STAT_2_PRBS_DATA_STAT_LSB     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_HOST_PMA_SD10G65_DFT_DFT_PRBS_STAT_2_PRBS_DATA_STAT_LSB(x)  VTSS_EXTRACT_BITFIELD(x,0,16)


/** 
 * \brief SD10G65 DFT miscellaneous status register 1
 *
 * \details
 * Status register 1 for SD10G65 DFT
 *
 * Register: \a HOST_PMA:SD10G65_DFT:DFT_MAIN_STAT_1
 */
#define VTSS_HOST_PMA_SD10G65_DFT_DFT_MAIN_STAT_1  VTSS_IOREG(0x09, 0, 0xb112)

/** 
 * \brief
 * 10 bits data word at address 'read_addr_cfg' used for further
 * observation by SW
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_MAIN_STAT_1 . CMP_DATA_STAT
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_MAIN_STAT_1_CMP_DATA_STAT(x)  VTSS_ENCODE_BITFIELD(x,0,10)
#define  VTSS_M_HOST_PMA_SD10G65_DFT_DFT_MAIN_STAT_1_CMP_DATA_STAT     VTSS_ENCODE_BITMASK(0,10)
#define  VTSS_X_HOST_PMA_SD10G65_DFT_DFT_MAIN_STAT_1_CMP_DATA_STAT(x)  VTSS_EXTRACT_BITFIELD(x,0,10)


/** 
 * \brief SD10G65 DFT miscellaneous status register 2
 *
 * \details
 * Status register 2 for SD10G65 DFT
 *
 * Register: \a HOST_PMA:SD10G65_DFT:DFT_MAIN_STAT_2
 */
#define VTSS_HOST_PMA_SD10G65_DFT_DFT_MAIN_STAT_2  VTSS_IOREG(0x09, 0, 0xb113)

/** 
 * \brief
 * Data input is unchanged for all 40 parallel bits for at least 7 clock
 * cycles (defined by c_STCK_CNT_THRES)
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_MAIN_STAT_2 . STUCK_AT_PAR
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_MAIN_STAT_2_STUCK_AT_PAR  VTSS_BIT(5)

/** 
 * \brief
 * Data input is constantly 0 or constantly 1 for all 40 parallel bits for
 * at least 7 clock cycles (defined by c_STCK_CNT_THRES)
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_MAIN_STAT_2 . STUCK_AT_01
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_MAIN_STAT_2_STUCK_AT_01  VTSS_BIT(4)

/** 
 * \brief
 * BIST: no sync found since BIST enabled
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_MAIN_STAT_2 . NO_SYNC
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_MAIN_STAT_2_NO_SYNC  VTSS_BIT(3)

/** 
 * \brief
 * BIST: input data not stable
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_MAIN_STAT_2 . INSTABLE
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_MAIN_STAT_2_INSTABLE  VTSS_BIT(2)

/** 
 * \brief
 * BIST not complete (i.e. not reached stable state or following)
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_MAIN_STAT_2 . INCOMPLETE
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_MAIN_STAT_2_INCOMPLETE  VTSS_BIT(1)

/** 
 * \brief
 * BIST is active (i.e. left DOZE but did not enter a final state)
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_MAIN_STAT_2 . ACTIVE
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_MAIN_STAT_2_ACTIVE  VTSS_BIT(0)


/** 
 * \brief SD10G65 DFT Main configuration register
 *
 * \details
 * Main configuration register for SD10G65 DFT.
 *
 * Register: \a HOST_PMA:SD10G65_DFT:DFT_TX_CFG
 */
#define VTSS_HOST_PMA_SD10G65_DFT_DFT_TX_CFG  VTSS_IOREG(0x09, 0, 0xb114)

/** 
 * \brief
 * Enables (1) reset of PRBS generator in case of unchanged data
 * ('stuck-at') for at least 511 clock cycles. Can be disabled (0) e.g. in
 * scrambler mode to avoid the very rare case that input patterns allow to
 * keep the generator's shift register filled with a constant value.
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_TX_CFG . RST_ON_STUCK_AT_CFG
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_TX_CFG_RST_ON_STUCK_AT_CFG  VTSS_BIT(12)

/** 
 * \brief
 * Selects SER interface width
 *
 * \details 
 * 0:8
 * 1:10
 * 2:16
 * 3:20
 * 4:32
 * 5:40 (default)
 *
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_TX_CFG . TX_WID_SEL_CFG
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_TX_CFG_TX_WID_SEL_CFG(x)  VTSS_ENCODE_BITFIELD(x,9,3)
#define  VTSS_M_HOST_PMA_SD10G65_DFT_DFT_TX_CFG_TX_WID_SEL_CFG     VTSS_ENCODE_BITMASK(9,3)
#define  VTSS_X_HOST_PMA_SD10G65_DFT_DFT_TX_CFG_TX_WID_SEL_CFG(x)  VTSS_EXTRACT_BITFIELD(x,9,3)

/** 
 * \brief
 * Selects PRBS generator
 *
 * \details 
 * 0: prbs7
 * 1: prbs15
 * 2: prbs23
 * 3: prbs11
 * 4: prbs31 (default)
 * 5: prbs9
 *
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_TX_CFG . TX_PRBS_SEL_CFG
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_TX_CFG_TX_PRBS_SEL_CFG(x)  VTSS_ENCODE_BITFIELD(x,6,3)
#define  VTSS_M_HOST_PMA_SD10G65_DFT_DFT_TX_CFG_TX_PRBS_SEL_CFG     VTSS_ENCODE_BITMASK(6,3)
#define  VTSS_X_HOST_PMA_SD10G65_DFT_DFT_TX_CFG_TX_PRBS_SEL_CFG(x)  VTSS_EXTRACT_BITFIELD(x,6,3)

/** 
 * \brief
 * Inverts the scrambler output
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_TX_CFG . SCRAM_INV_CFG
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_TX_CFG_SCRAM_INV_CFG  VTSS_BIT(5)

/** 
 * \brief
 * Selects PRBS generator input
 *
 * \details 
 * 0:pat-gen
 * 1:core
 *
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_TX_CFG . IPATH_CFG
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_TX_CFG_IPATH_CFG  VTSS_BIT(4)

/** 
 * \brief
 * Selects DFT-TX output
 *
 * \details 
 * 0:PRBS/scrambler (default)
 * 1:bypass

 *
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_TX_CFG . OPATH_CFG
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_TX_CFG_OPATH_CFG(x)  VTSS_ENCODE_BITFIELD(x,2,2)
#define  VTSS_M_HOST_PMA_SD10G65_DFT_DFT_TX_CFG_OPATH_CFG     VTSS_ENCODE_BITMASK(2,2)
#define  VTSS_X_HOST_PMA_SD10G65_DFT_DFT_TX_CFG_OPATH_CFG(x)  VTSS_EXTRACT_BITFIELD(x,2,2)

/** 
 * \brief
 * Word width of constant pattern generator
 *
 * \details 
 * 0:bytes mode; 1:10-bits word mode
 *
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_TX_CFG . TX_WORD_MODE_CFG
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_TX_CFG_TX_WORD_MODE_CFG  VTSS_BIT(1)

/** 
 * \brief
 * Enable TX DFT capability
 *
 * \details 
 * 0: Disable DFT
 * 1: Enable DFT
 *
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_TX_CFG . DFT_TX_ENA
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_TX_CFG_DFT_TX_ENA  VTSS_BIT(0)


/** 
 * \brief SD10G65 DFT TX Constant pattern configuration register 1
 *
 * \details
 * TX Constant MSB pattern configuration register 1 for SD10G65 DFT.
 *
 * Register: \a HOST_PMA:SD10G65_DFT:DFT_TX_PAT_CFG_1
 */
#define VTSS_HOST_PMA_SD10G65_DFT_DFT_TX_PAT_CFG_1  VTSS_IOREG(0x09, 0, 0xb115)

/** 
 * \brief
 * Constant patterns are valid to store
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_TX_PAT_CFG_1 . PAT_VLD_CFG
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_TX_PAT_CFG_1_PAT_VLD_CFG  VTSS_BIT(4)

/** 
 * \brief
 * Maximum address in generator (before continuing with address 0)
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_TX_PAT_CFG_1 . MAX_ADDR_GEN_CFG
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_TX_PAT_CFG_1_MAX_ADDR_GEN_CFG(x)  VTSS_ENCODE_BITFIELD(x,0,4)
#define  VTSS_M_HOST_PMA_SD10G65_DFT_DFT_TX_PAT_CFG_1_MAX_ADDR_GEN_CFG     VTSS_ENCODE_BITMASK(0,4)
#define  VTSS_X_HOST_PMA_SD10G65_DFT_DFT_TX_PAT_CFG_1_MAX_ADDR_GEN_CFG(x)  VTSS_EXTRACT_BITFIELD(x,0,4)


/** 
 * \brief SD10G65 DFT TX Constant pattern configuration register 2
 *
 * \details
 * TX Constant MSB pattern configuration register 2 for SD10G65 DFT.
 *
 * Register: \a HOST_PMA:SD10G65_DFT:DFT_TX_PAT_CFG_2
 */
#define VTSS_HOST_PMA_SD10G65_DFT_DFT_TX_PAT_CFG_2  VTSS_IOREG(0x09, 0, 0xb116)

/** 
 * \brief
 * Current storage address for patterns in generator
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_TX_PAT_CFG_2 . STORE_ADDR_CFG
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_TX_PAT_CFG_2_STORE_ADDR_CFG(x)  VTSS_ENCODE_BITFIELD(x,10,4)
#define  VTSS_M_HOST_PMA_SD10G65_DFT_DFT_TX_PAT_CFG_2_STORE_ADDR_CFG     VTSS_ENCODE_BITMASK(10,4)
#define  VTSS_X_HOST_PMA_SD10G65_DFT_DFT_TX_PAT_CFG_2_STORE_ADDR_CFG(x)  VTSS_EXTRACT_BITFIELD(x,10,4)

/** 
 * \brief
 * 10 bits word of constant patterns for transmission
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_TX_PAT_CFG_2 . PATTERN_CFG
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_TX_PAT_CFG_2_PATTERN_CFG(x)  VTSS_ENCODE_BITFIELD(x,0,10)
#define  VTSS_M_HOST_PMA_SD10G65_DFT_DFT_TX_PAT_CFG_2_PATTERN_CFG     VTSS_ENCODE_BITMASK(0,10)
#define  VTSS_X_HOST_PMA_SD10G65_DFT_DFT_TX_PAT_CFG_2_PATTERN_CFG(x)  VTSS_EXTRACT_BITFIELD(x,0,10)


/** 
 * \brief SD10G65 DFT TX constant pattern status register
 *
 * \details
 * Status register for SD10G65 DFT containing the constant patterns used
 * for comparison (last in LEARN mode)
 *
 * Register: \a HOST_PMA:SD10G65_DFT:DFT_TX_CMP_DAT_STAT
 */
#define VTSS_HOST_PMA_SD10G65_DFT_DFT_TX_CMP_DAT_STAT  VTSS_IOREG(0x09, 0, 0xb117)

/** 
 * \brief
 * Scrambler/PRBS generator output unchanged for at least 511 clock cycles.
 *  The high state is cleared by writing a 1 to the bit.
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_TX_CMP_DAT_STAT . TX_STUCK_AT_STICKY
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_TX_CMP_DAT_STAT_TX_STUCK_AT_STICKY  VTSS_BIT(12)

/** 
 * \brief
 * 10 bits data word at address 'store_addr_cfg' used for further
 * observation by SW
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_TX_CMP_DAT_STAT . PAT_STAT
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_TX_CMP_DAT_STAT_PAT_STAT(x)  VTSS_ENCODE_BITFIELD(x,0,10)
#define  VTSS_M_HOST_PMA_SD10G65_DFT_DFT_TX_CMP_DAT_STAT_PAT_STAT     VTSS_ENCODE_BITMASK(0,10)
#define  VTSS_X_HOST_PMA_SD10G65_DFT_DFT_TX_CMP_DAT_STAT_PAT_STAT(x)  VTSS_EXTRACT_BITFIELD(x,0,10)


/** 
 * \brief DFT clock compare config
 *
 * \details
 * Configuration register for Clock Compare logic. Compared clocks are
 * always divided by 4 before any further processing. A clock edge on
 * tx_clk increments the counter, a clock edge on rx_clk decrements the
 * counter. If only one clock is selected for clock comparison, the number
 * of clock cycles within a given time can be measured.
 *
 * Register: \a HOST_PMA:SD10G65_DFT:DFT_CLK_CMP_CFG
 */
#define VTSS_HOST_PMA_SD10G65_DFT_DFT_CLK_CMP_CFG  VTSS_IOREG(0x09, 0, 0xb118)

/** 
 * \brief
 * Clock compare value updated toggle bit. Toggles on each update of
 * CLK_CMP_VALUE
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_CLK_CMP_CFG . CLK_CMP_UPDTOG
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_CLK_CMP_CFG_CLK_CMP_UPDTOG  VTSS_BIT(12)

/** 
 * \brief
 * Clock compare selection
 *
 * \details 
 * 0: rx_clk vs. tx_clk
 * 1: rx_clk
 * 2: tx_clk
 * 3: Reserved
 *
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_CLK_CMP_CFG . CLK_CMP_SEL
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_CLK_CMP_CFG_CLK_CMP_SEL(x)  VTSS_ENCODE_BITFIELD(x,2,2)
#define  VTSS_M_HOST_PMA_SD10G65_DFT_DFT_CLK_CMP_CFG_CLK_CMP_SEL     VTSS_ENCODE_BITMASK(2,2)
#define  VTSS_X_HOST_PMA_SD10G65_DFT_DFT_CLK_CMP_CFG_CLK_CMP_SEL(x)  VTSS_EXTRACT_BITFIELD(x,2,2)

/** 
 * \brief
 * Clock comparison mode
 *
 * \details 
 * 0: single shot
 * 1: continuous
 *
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_CLK_CMP_CFG . CLK_CMP_MODE
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_CLK_CMP_CFG_CLK_CMP_MODE  VTSS_BIT(1)

/** 
 * \brief
 * Enable clock comparison (enabling automatically clears comparison
 * counter)
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_CLK_CMP_CFG . CLK_CMP_ENA
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_CLK_CMP_CFG_CLK_CMP_ENA  VTSS_BIT(0)


/** 
 * \brief DFT clock compare timer
 *
 * \details
 * Upper half of clock comparison timer.  After timer has expired, current
 * clock comparison value is stored.  The timer is clocked at 156.25 MHz.
 *
 * Register: \a HOST_PMA:SD10G65_DFT:DFT_CLK_CMP_TIMERA
 */
#define VTSS_HOST_PMA_SD10G65_DFT_DFT_CLK_CMP_TIMERA  VTSS_IOREG(0x09, 0, 0xb119)

/** 
 * \brief
 * Clock comparison timer, bits [31:16]
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_CLK_CMP_TIMERA . CLK_CMP_TIMER_MSB
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_CLK_CMP_TIMERA_CLK_CMP_TIMER_MSB(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_HOST_PMA_SD10G65_DFT_DFT_CLK_CMP_TIMERA_CLK_CMP_TIMER_MSB     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_HOST_PMA_SD10G65_DFT_DFT_CLK_CMP_TIMERA_CLK_CMP_TIMER_MSB(x)  VTSS_EXTRACT_BITFIELD(x,0,16)


/** 
 * \brief DFT clock compare timer
 *
 * \details
 * Lower half of clock comparison timer.  After timer has expired, current
 * clock comparison value is stored.  The timer is clocked at 156.25 MHz.
 *
 * Register: \a HOST_PMA:SD10G65_DFT:DFT_CLK_CMP_TIMERB
 */
#define VTSS_HOST_PMA_SD10G65_DFT_DFT_CLK_CMP_TIMERB  VTSS_IOREG(0x09, 0, 0xb11a)

/** 
 * \brief
 * Clock comparison timer, bits [15:0]
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_CLK_CMP_TIMERB . CLK_CMP_TIMER_LSB
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_CLK_CMP_TIMERB_CLK_CMP_TIMER_LSB(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_HOST_PMA_SD10G65_DFT_DFT_CLK_CMP_TIMERB_CLK_CMP_TIMER_LSB     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_HOST_PMA_SD10G65_DFT_DFT_CLK_CMP_TIMERB_CLK_CMP_TIMER_LSB(x)  VTSS_EXTRACT_BITFIELD(x,0,16)


/** 
 * \brief DFT clock comparison value
 *
 * \details
 * Upper half of clock comparison result. This register is updated after
 * clock comparison timer has expired.	In continuous mode this register is
 * periodically updated.
 *
 * Register: \a HOST_PMA:SD10G65_DFT:DFT_CLK_CMP_VALUEA
 */
#define VTSS_HOST_PMA_SD10G65_DFT_DFT_CLK_CMP_VALUEA  VTSS_IOREG(0x09, 0, 0xb11b)

/** 
 * \brief
 * Clock comparison value (difference between clk0 and clk1), bits [31:16]
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_CLK_CMP_VALUEA . CLK_CMP_VALUE_MSB
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_CLK_CMP_VALUEA_CLK_CMP_VALUE_MSB(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_HOST_PMA_SD10G65_DFT_DFT_CLK_CMP_VALUEA_CLK_CMP_VALUE_MSB     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_HOST_PMA_SD10G65_DFT_DFT_CLK_CMP_VALUEA_CLK_CMP_VALUE_MSB(x)  VTSS_EXTRACT_BITFIELD(x,0,16)


/** 
 * \brief DFT clock comparison value
 *
 * \details
 * Lower half of clock comparison result. This register is updated after
 * clock comparison timer has expired.	In continuous mode this register is
 * periodically updated.
 *
 * Register: \a HOST_PMA:SD10G65_DFT:DFT_CLK_CMP_VALUEB
 */
#define VTSS_HOST_PMA_SD10G65_DFT_DFT_CLK_CMP_VALUEB  VTSS_IOREG(0x09, 0, 0xb11c)

/** 
 * \brief
 * Clock comparison value (difference between clk0 and clk1), bits [15:0]
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_CLK_CMP_VALUEB . CLK_CMP_VALUE_LSB
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_CLK_CMP_VALUEB_CLK_CMP_VALUE_LSB(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_HOST_PMA_SD10G65_DFT_DFT_CLK_CMP_VALUEB_CLK_CMP_VALUE_LSB     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_HOST_PMA_SD10G65_DFT_DFT_CLK_CMP_VALUEB_CLK_CMP_VALUE_LSB(x)  VTSS_EXTRACT_BITFIELD(x,0,16)


/** 
 * \brief DFT clock comparison maximum value
 *
 * \details
 * Upper half of clock comparison max result.  Can be used to judge e.g.
 * SSC clock deviation.  This register is updated after clock comparison
 * timer has expired.  In continuous mode this register is periodically
 * updated.
 *
 * Register: \a HOST_PMA:SD10G65_DFT:DFT_CLK_CMP_MAXVALA
 */
#define VTSS_HOST_PMA_SD10G65_DFT_DFT_CLK_CMP_MAXVALA  VTSS_IOREG(0x09, 0, 0xb11d)

/** 
 * \brief
 * Clock comparison max value (maximum measured difference between clk0 and
 * clk1), bits [31:16]
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_CLK_CMP_MAXVALA . CLK_CMP_MAXVAL_MSB
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_CLK_CMP_MAXVALA_CLK_CMP_MAXVAL_MSB(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_HOST_PMA_SD10G65_DFT_DFT_CLK_CMP_MAXVALA_CLK_CMP_MAXVAL_MSB     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_HOST_PMA_SD10G65_DFT_DFT_CLK_CMP_MAXVALA_CLK_CMP_MAXVAL_MSB(x)  VTSS_EXTRACT_BITFIELD(x,0,16)


/** 
 * \brief DFT clock comparison maximum value
 *
 * \details
 * Lower half of clock comparison max result.  Can be used to judge e.g.
 * SSC clock deviation.  This register is updated after clock comparison
 * timer has expired.  In continuous mode this register is periodically
 * updated.
 *
 * Register: \a HOST_PMA:SD10G65_DFT:DFT_CLK_CMP_MAXVALB
 */
#define VTSS_HOST_PMA_SD10G65_DFT_DFT_CLK_CMP_MAXVALB  VTSS_IOREG(0x09, 0, 0xb11e)

/** 
 * \brief
 * Clock comparison max value (maximum measured difference between clk0 and
 * clk1), bits [15:0]
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_CLK_CMP_MAXVALB . CLK_CMP_MAXVAL_LSB
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_CLK_CMP_MAXVALB_CLK_CMP_MAXVAL_LSB(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_HOST_PMA_SD10G65_DFT_DFT_CLK_CMP_MAXVALB_CLK_CMP_MAXVAL_LSB     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_HOST_PMA_SD10G65_DFT_DFT_CLK_CMP_MAXVALB_CLK_CMP_MAXVAL_LSB(x)  VTSS_EXTRACT_BITFIELD(x,0,16)


/** 
 * \brief DFT TX Error insertion configuration register
 *
 * \details
 * Configuration register for explicit error insertion into DFT driven data
 * stream. Allows to insert expected errors to check e.g. TX/RX
 * connectivity
 *
 * Register: \a HOST_PMA:SD10G65_DFT:DFT_TX_ERR_INSERT_CFG_1
 */
#define VTSS_HOST_PMA_SD10G65_DFT_DFT_TX_ERR_INSERT_CFG_1  VTSS_IOREG(0x09, 0, 0xb11f)

/** 
 * \brief
 * Preload value for clock generator timer
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_TX_ERR_INSERT_CFG_1 . CG_TIMER_CFG
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_TX_ERR_INSERT_CFG_1_CG_TIMER_CFG(x)  VTSS_ENCODE_BITFIELD(x,6,10)
#define  VTSS_M_HOST_PMA_SD10G65_DFT_DFT_TX_ERR_INSERT_CFG_1_CG_TIMER_CFG     VTSS_ENCODE_BITMASK(6,10)
#define  VTSS_X_HOST_PMA_SD10G65_DFT_DFT_TX_ERR_INSERT_CFG_1_CG_TIMER_CFG(x)  VTSS_EXTRACT_BITFIELD(x,6,10)

/** 
 * \brief
 * Trigger a single error or a burst of errors (refer to num_err_cfg)
 *
 * \details 
 * 0 to 1 (edge) activates this function
 *
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_TX_ERR_INSERT_CFG_1 . ERR_TRIG_ONESHOT_CFG
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_TX_ERR_INSERT_CFG_1_ERR_TRIG_ONESHOT_CFG  VTSS_BIT(4)

/** 
 * \brief
 * Frequency of continous/limited error insertion in steps of 40 bits
 *
 * \details 
 * 0: disable continous insertion 1-15: step between 2 errors =
 * 2^(err_freq_cfg + 5) 40 bit words (refer also to err_posit_offs_cfg)
 *
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_TX_ERR_INSERT_CFG_1 . ERR_FREQ_CFG
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_TX_ERR_INSERT_CFG_1_ERR_FREQ_CFG(x)  VTSS_ENCODE_BITFIELD(x,0,4)
#define  VTSS_M_HOST_PMA_SD10G65_DFT_DFT_TX_ERR_INSERT_CFG_1_ERR_FREQ_CFG     VTSS_ENCODE_BITMASK(0,4)
#define  VTSS_X_HOST_PMA_SD10G65_DFT_DFT_TX_ERR_INSERT_CFG_1_ERR_FREQ_CFG(x)  VTSS_EXTRACT_BITFIELD(x,0,4)


/** 
 * \brief DFT TX Error insertion configuration register
 *
 * \details
 * Configuration register for explicit error insertion into DFT driven data
 * stream. Allows to insert expected errors to check e.g. TX/RX
 * connectivity
 *
 * Register: \a HOST_PMA:SD10G65_DFT:DFT_TX_ERR_INSERT_CFG_2
 */
#define VTSS_HOST_PMA_SD10G65_DFT_DFT_TX_ERR_INSERT_CFG_2  VTSS_IOREG(0x09, 0, 0xb120)

/** 
 * \brief
 * Position within 40 bit word where an error is inserted by inverting the
 * bit value
 *
 * \details 
 * 0: LSB
 * 39: MSB
 * 40-63: reserved
 *
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_TX_ERR_INSERT_CFG_2 . ERR_POSIT_CFG
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_TX_ERR_INSERT_CFG_2_ERR_POSIT_CFG(x)  VTSS_ENCODE_BITFIELD(x,10,6)
#define  VTSS_M_HOST_PMA_SD10G65_DFT_DFT_TX_ERR_INSERT_CFG_2_ERR_POSIT_CFG     VTSS_ENCODE_BITMASK(10,6)
#define  VTSS_X_HOST_PMA_SD10G65_DFT_DFT_TX_ERR_INSERT_CFG_2_ERR_POSIT_CFG(x)  VTSS_EXTRACT_BITFIELD(x,10,6)

/** 
 * \brief
 * Offset of bitposition increased per inserted error; allows 'walking'
 * error. Offset is reset when continous/limited error insertion is
 * disabled or burst mode is enabled and burst insertion is finished or
 * err_posit_offs_cfg = 0
 *
 * \details 
 * 0: disabled
 * 1: move 1 bit (from LSB to MSB)
 * ...
 * 39: move 39 bit (from LSB to MSB)
 * 40-63: reserved
 *
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_TX_ERR_INSERT_CFG_2 . ERR_POSIT_OFFS_CFG
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_TX_ERR_INSERT_CFG_2_ERR_POSIT_OFFS_CFG(x)  VTSS_ENCODE_BITFIELD(x,4,6)
#define  VTSS_M_HOST_PMA_SD10G65_DFT_DFT_TX_ERR_INSERT_CFG_2_ERR_POSIT_OFFS_CFG     VTSS_ENCODE_BITMASK(4,6)
#define  VTSS_X_HOST_PMA_SD10G65_DFT_DFT_TX_ERR_INSERT_CFG_2_ERR_POSIT_OFFS_CFG(x)  VTSS_EXTRACT_BITFIELD(x,4,6)

/** 
 * \brief
 * limited error insertion: burst mode (err_freq_cfg must be > 0)
 *
 * \details 
 * 0: burst mode is disabled 1-15: number of errors after each error
 * triggering = 2^(num_err_cfg + 5)
 *
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_TX_ERR_INSERT_CFG_2 . NUM_ERR_CFG
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_TX_ERR_INSERT_CFG_2_NUM_ERR_CFG(x)  VTSS_ENCODE_BITFIELD(x,0,4)
#define  VTSS_M_HOST_PMA_SD10G65_DFT_DFT_TX_ERR_INSERT_CFG_2_NUM_ERR_CFG     VTSS_ENCODE_BITMASK(0,4)
#define  VTSS_X_HOST_PMA_SD10G65_DFT_DFT_TX_ERR_INSERT_CFG_2_NUM_ERR_CFG(x)  VTSS_EXTRACT_BITFIELD(x,0,4)


/** 
 * \brief DFT clock generator configuration register
 *
 * \details
 * Configuration register for clock generator to build a low speed clock
 * signal of variable length and variable duty cycle provided on all data
 * bits simultaniously
 *
 * Register: \a HOST_PMA:SD10G65_DFT:DFT_CLK_GEN_CFG_1
 */
#define VTSS_HOST_PMA_SD10G65_DFT_DFT_CLK_GEN_CFG_1  VTSS_IOREG(0x09, 0, 0xb121)

/** 
 * \brief
 * (Half) clock period cfg in normal mode: high period = cg_per_cfg +
 * cg_dcd_cfg, low period = cg_per_cfg - cg_dcd_cfg
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_CLK_GEN_CFG_1 . CG_PER_CFG
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_CLK_GEN_CFG_1_CG_PER_CFG(x)  VTSS_ENCODE_BITFIELD(x,0,10)
#define  VTSS_M_HOST_PMA_SD10G65_DFT_DFT_CLK_GEN_CFG_1_CG_PER_CFG     VTSS_ENCODE_BITMASK(0,10)
#define  VTSS_X_HOST_PMA_SD10G65_DFT_DFT_CLK_GEN_CFG_1_CG_PER_CFG(x)  VTSS_EXTRACT_BITFIELD(x,0,10)


/** 
 * \brief DFT clock generator configuration register
 *
 * \details
 * Configuration register for clock generator to build a low speed clock
 * signal of variable length and variable duty cycle provided on all data
 * bits simultaniously
 *
 * Register: \a HOST_PMA:SD10G65_DFT:DFT_CLK_GEN_CFG_2
 */
#define VTSS_HOST_PMA_SD10G65_DFT_DFT_CLK_GEN_CFG_2  VTSS_IOREG(0x09, 0, 0xb122)

/** 
 * \brief
 * (Half) clock period cfg in jump mode: high period = cg_per_cfg +
 * cg_dcd_cfg, low period = cg_per_cfg - cg_dcd_cfg
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_CLK_GEN_CFG_2 . CG_PER_JUMP_CFG
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_CLK_GEN_CFG_2_CG_PER_JUMP_CFG(x)  VTSS_ENCODE_BITFIELD(x,0,10)
#define  VTSS_M_HOST_PMA_SD10G65_DFT_DFT_CLK_GEN_CFG_2_CG_PER_JUMP_CFG     VTSS_ENCODE_BITMASK(0,10)
#define  VTSS_X_HOST_PMA_SD10G65_DFT_DFT_CLK_GEN_CFG_2_CG_PER_JUMP_CFG(x)  VTSS_EXTRACT_BITFIELD(x,0,10)


/** 
 * \brief DFT clock generator configuration register
 *
 * \details
 * Configuration register for clock generator to build a low speed clock
 * signal of variable length and variable duty cycle provided on all data
 * bits simultaniously
 *
 * Register: \a HOST_PMA:SD10G65_DFT:DFT_CLK_GEN_CFG_3
 */
#define VTSS_HOST_PMA_SD10G65_DFT_DFT_CLK_GEN_CFG_3  VTSS_IOREG(0x09, 0, 0xb123)

/** 
 * \brief
 * Duty cycle distortion: high period = cg_per_cfg + cg_dcd_cfg, low period
 * = cg_per_cfg - cg_dcd_cfg
 *
 * \details 
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_CLK_GEN_CFG_3 . CG_DCD_CFG
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_CLK_GEN_CFG_3_CG_DCD_CFG(x)  VTSS_ENCODE_BITFIELD(x,2,10)
#define  VTSS_M_HOST_PMA_SD10G65_DFT_DFT_CLK_GEN_CFG_3_CG_DCD_CFG     VTSS_ENCODE_BITMASK(2,10)
#define  VTSS_X_HOST_PMA_SD10G65_DFT_DFT_CLK_GEN_CFG_3_CG_DCD_CFG(x)  VTSS_EXTRACT_BITFIELD(x,2,10)

/** 
 * \brief
 * clock generator mode
 *
 * \details 
 * 0: normal operation
 * cg_per_cfg controls period; 0->1 transition: after current period has
 * finished (only) the next period is controlled by cg_per_jump_cfg
 * afterwards normal operation; 2: every N'th period the high value is
 * replaced by a low value
 * N is defined by cg_timer_cfg; 3: every N'th period the low value is
 * replaced by a high value
 * N is defined by cg_timer_cfg
 *
 * Field: VTSS_HOST_PMA_SD10G65_DFT_DFT_CLK_GEN_CFG_3 . CG_MODE_CFG
 */
#define  VTSS_F_HOST_PMA_SD10G65_DFT_DFT_CLK_GEN_CFG_3_CG_MODE_CFG(x)  VTSS_ENCODE_BITFIELD(x,0,2)
#define  VTSS_M_HOST_PMA_SD10G65_DFT_DFT_CLK_GEN_CFG_3_CG_MODE_CFG     VTSS_ENCODE_BITMASK(0,2)
#define  VTSS_X_HOST_PMA_SD10G65_DFT_DFT_CLK_GEN_CFG_3_CG_MODE_CFG(x)  VTSS_EXTRACT_BITFIELD(x,0,2)

/**
 * Register Group: \a HOST_PMA:ROMENG_1
 *
 * Register Collection for Rom Engine 1
 */


/** 
 * \brief SPI address field of ROM table entry ... replication_count=135
 *
 * \details
 * Register: \a HOST_PMA:ROMENG_1:spi_adr
 *
 * @param ri Register: spi_adr, 0-134
 */
#define VTSS_HOST_PMA_ROMENG_1_spi_adr(ri)   VTSS_IOREG(0x09, 0, 0xb200 | (ri))

/** 
 * \brief
 * SPI address to write
 *
 * \details 
 * Field: VTSS_HOST_PMA_ROMENG_1_spi_adr . spi_adr
 */
#define  VTSS_F_HOST_PMA_ROMENG_1_spi_adr_spi_adr(x)  VTSS_ENCODE_BITFIELD(x,0,7)
#define  VTSS_M_HOST_PMA_ROMENG_1_spi_adr_spi_adr     VTSS_ENCODE_BITMASK(0,7)
#define  VTSS_X_HOST_PMA_ROMENG_1_spi_adr_spi_adr(x)  VTSS_EXTRACT_BITFIELD(x,0,7)


/** 
 * \brief Lower 16 bits of SPI data field of ROM table entry ... replication_count=135
 *
 * \details
 * Register: \a HOST_PMA:ROMENG_1:data_lsw
 *
 * @param ri Register: data_lsw, 0-134
 */
#define VTSS_HOST_PMA_ROMENG_1_data_lsw(ri)  VTSS_IOREG(0x09, 0, 0xb300 | (ri))

/** 
 * \brief
 * SPI data lsw
 *
 * \details 
 * Field: VTSS_HOST_PMA_ROMENG_1_data_lsw . spi_dat_lsw
 */
#define  VTSS_F_HOST_PMA_ROMENG_1_data_lsw_spi_dat_lsw(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_HOST_PMA_ROMENG_1_data_lsw_spi_dat_lsw     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_HOST_PMA_ROMENG_1_data_lsw_spi_dat_lsw(x)  VTSS_EXTRACT_BITFIELD(x,0,16)


/** 
 * \brief Upper 16 bits of SPI data field of ROM table entry ... replication_count=135
 *
 * \details
 * Register: \a HOST_PMA:ROMENG_1:data_msw
 *
 * @param ri Register: data_msw, 0-134
 */
#define VTSS_HOST_PMA_ROMENG_1_data_msw(ri)  VTSS_IOREG(0x09, 0, 0xb400 | (ri))

/** 
 * \brief
 * SPI data msw
 *
 * \details 
 * Field: VTSS_HOST_PMA_ROMENG_1_data_msw . spi_dat_msw
 */
#define  VTSS_F_HOST_PMA_ROMENG_1_data_msw_spi_dat_msw(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_HOST_PMA_ROMENG_1_data_msw_spi_dat_msw     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_HOST_PMA_ROMENG_1_data_msw_spi_dat_msw(x)  VTSS_EXTRACT_BITFIELD(x,0,16)

/**
 * Register Group: \a HOST_PMA:ROMENG_2
 *
 * Register Collection for Rom Engine 2
 */


/** 
 * \brief ROM table start/end addresses of Tx 10G setting routine
 *
 * \details
 * Register: \a HOST_PMA:ROMENG_2:adr_tx10g
 */
#define VTSS_HOST_PMA_ROMENG_2_adr_tx10g     VTSS_IOREG(0x09, 0, 0xb600)

/** 
 * \brief
 * Starting ROM address of Tx 10G routine
 *
 * \details 
 * Field: VTSS_HOST_PMA_ROMENG_2_adr_tx10g . adr_tx10g_start
 */
#define  VTSS_F_HOST_PMA_ROMENG_2_adr_tx10g_adr_tx10g_start(x)  VTSS_ENCODE_BITFIELD(x,8,8)
#define  VTSS_M_HOST_PMA_ROMENG_2_adr_tx10g_adr_tx10g_start     VTSS_ENCODE_BITMASK(8,8)
#define  VTSS_X_HOST_PMA_ROMENG_2_adr_tx10g_adr_tx10g_start(x)  VTSS_EXTRACT_BITFIELD(x,8,8)

/** 
 * \brief
 * Ending   ROM address of Tx 10G routine
 *
 * \details 
 * Field: VTSS_HOST_PMA_ROMENG_2_adr_tx10g . adr_tx10g_end
 */
#define  VTSS_F_HOST_PMA_ROMENG_2_adr_tx10g_adr_tx10g_end(x)  VTSS_ENCODE_BITFIELD(x,0,8)
#define  VTSS_M_HOST_PMA_ROMENG_2_adr_tx10g_adr_tx10g_end     VTSS_ENCODE_BITMASK(0,8)
#define  VTSS_X_HOST_PMA_ROMENG_2_adr_tx10g_adr_tx10g_end(x)  VTSS_EXTRACT_BITFIELD(x,0,8)


/** 
 * \brief ROM table start/end addresses of Rx 10G setting routine
 *
 * \details
 * Register: \a HOST_PMA:ROMENG_2:adr_rx10g
 */
#define VTSS_HOST_PMA_ROMENG_2_adr_rx10g     VTSS_IOREG(0x09, 0, 0xb601)

/** 
 * \brief
 * Starting ROM address of Rx 10G routine
 *
 * \details 
 * Field: VTSS_HOST_PMA_ROMENG_2_adr_rx10g . adr_rx10g_start
 */
#define  VTSS_F_HOST_PMA_ROMENG_2_adr_rx10g_adr_rx10g_start(x)  VTSS_ENCODE_BITFIELD(x,8,8)
#define  VTSS_M_HOST_PMA_ROMENG_2_adr_rx10g_adr_rx10g_start     VTSS_ENCODE_BITMASK(8,8)
#define  VTSS_X_HOST_PMA_ROMENG_2_adr_rx10g_adr_rx10g_start(x)  VTSS_EXTRACT_BITFIELD(x,8,8)

/** 
 * \brief
 * Ending   ROM address of Rx 10G routine
 *
 * \details 
 * Field: VTSS_HOST_PMA_ROMENG_2_adr_rx10g . adr_rx10g_end
 */
#define  VTSS_F_HOST_PMA_ROMENG_2_adr_rx10g_adr_rx10g_end(x)  VTSS_ENCODE_BITFIELD(x,0,8)
#define  VTSS_M_HOST_PMA_ROMENG_2_adr_rx10g_adr_rx10g_end     VTSS_ENCODE_BITMASK(0,8)
#define  VTSS_X_HOST_PMA_ROMENG_2_adr_rx10g_adr_rx10g_end(x)  VTSS_EXTRACT_BITFIELD(x,0,8)


/** 
 * \brief ROM table start/end addresses of Tx 1G setting routine
 *
 * \details
 * Register: \a HOST_PMA:ROMENG_2:adr_tx1g
 */
#define VTSS_HOST_PMA_ROMENG_2_adr_tx1g      VTSS_IOREG(0x09, 0, 0xb602)

/** 
 * \brief
 * Starting ROM address of Tx 1G routine
 *
 * \details 
 * Field: VTSS_HOST_PMA_ROMENG_2_adr_tx1g . adr_tx1g_start
 */
#define  VTSS_F_HOST_PMA_ROMENG_2_adr_tx1g_adr_tx1g_start(x)  VTSS_ENCODE_BITFIELD(x,8,8)
#define  VTSS_M_HOST_PMA_ROMENG_2_adr_tx1g_adr_tx1g_start     VTSS_ENCODE_BITMASK(8,8)
#define  VTSS_X_HOST_PMA_ROMENG_2_adr_tx1g_adr_tx1g_start(x)  VTSS_EXTRACT_BITFIELD(x,8,8)

/** 
 * \brief
 * Ending   ROM address of Tx 1G routine
 *
 * \details 
 * Field: VTSS_HOST_PMA_ROMENG_2_adr_tx1g . adr_tx1g_end
 */
#define  VTSS_F_HOST_PMA_ROMENG_2_adr_tx1g_adr_tx1g_end(x)  VTSS_ENCODE_BITFIELD(x,0,8)
#define  VTSS_M_HOST_PMA_ROMENG_2_adr_tx1g_adr_tx1g_end     VTSS_ENCODE_BITMASK(0,8)
#define  VTSS_X_HOST_PMA_ROMENG_2_adr_tx1g_adr_tx1g_end(x)  VTSS_EXTRACT_BITFIELD(x,0,8)


/** 
 * \brief ROM table start/end addresses of Rx 1G setting routine
 *
 * \details
 * Register: \a HOST_PMA:ROMENG_2:adr_rx1g
 */
#define VTSS_HOST_PMA_ROMENG_2_adr_rx1g      VTSS_IOREG(0x09, 0, 0xb603)

/** 
 * \brief
 * Starting ROM address of Rx 1G routine
 *
 * \details 
 * Field: VTSS_HOST_PMA_ROMENG_2_adr_rx1g . adr_rx1g_start
 */
#define  VTSS_F_HOST_PMA_ROMENG_2_adr_rx1g_adr_rx1g_start(x)  VTSS_ENCODE_BITFIELD(x,8,8)
#define  VTSS_M_HOST_PMA_ROMENG_2_adr_rx1g_adr_rx1g_start     VTSS_ENCODE_BITMASK(8,8)
#define  VTSS_X_HOST_PMA_ROMENG_2_adr_rx1g_adr_rx1g_start(x)  VTSS_EXTRACT_BITFIELD(x,8,8)

/** 
 * \brief
 * Ending   ROM address of Rx 1G routine
 *
 * \details 
 * Field: VTSS_HOST_PMA_ROMENG_2_adr_rx1g . adr_rx1g_end
 */
#define  VTSS_F_HOST_PMA_ROMENG_2_adr_rx1g_adr_rx1g_end(x)  VTSS_ENCODE_BITFIELD(x,0,8)
#define  VTSS_M_HOST_PMA_ROMENG_2_adr_rx1g_adr_rx1g_end     VTSS_ENCODE_BITMASK(0,8)
#define  VTSS_X_HOST_PMA_ROMENG_2_adr_rx1g_adr_rx1g_end(x)  VTSS_EXTRACT_BITFIELD(x,0,8)


/** 
 * \brief ROM table start/end addresses of WAN setting routine
 *
 * \details
 * Register: \a HOST_PMA:ROMENG_2:adr_wan
 */
#define VTSS_HOST_PMA_ROMENG_2_adr_wan       VTSS_IOREG(0x09, 0, 0xb604)

/** 
 * \brief
 * Starting ROM address of WAN routine
 *
 * \details 
 * Field: VTSS_HOST_PMA_ROMENG_2_adr_wan . adr_wan_start
 */
#define  VTSS_F_HOST_PMA_ROMENG_2_adr_wan_adr_wan_start(x)  VTSS_ENCODE_BITFIELD(x,8,8)
#define  VTSS_M_HOST_PMA_ROMENG_2_adr_wan_adr_wan_start     VTSS_ENCODE_BITMASK(8,8)
#define  VTSS_X_HOST_PMA_ROMENG_2_adr_wan_adr_wan_start(x)  VTSS_EXTRACT_BITFIELD(x,8,8)

/** 
 * \brief
 * Ending   ROM address of WAN routine
 *
 * \details 
 * Field: VTSS_HOST_PMA_ROMENG_2_adr_wan . adr_wan_end
 */
#define  VTSS_F_HOST_PMA_ROMENG_2_adr_wan_adr_wan_end(x)  VTSS_ENCODE_BITFIELD(x,0,8)
#define  VTSS_M_HOST_PMA_ROMENG_2_adr_wan_adr_wan_end     VTSS_ENCODE_BITMASK(0,8)
#define  VTSS_X_HOST_PMA_ROMENG_2_adr_wan_adr_wan_end(x)  VTSS_EXTRACT_BITFIELD(x,0,8)

/**
 * Register Group: \a HOST_PMA:ROMENG_STATUS
 *
 * Rom Engine Status
 */


/** 
 * \brief Rom Engine Status
 *
 * \details
 * Rom Engine Status
 *
 * Register: \a HOST_PMA:ROMENG_STATUS:ROMENG_STATUS
 */
#define VTSS_HOST_PMA_ROMENG_STATUS_ROMENG_STATUS  VTSS_IOREG(0x09, 0, 0xb6ff)

/** 
 * \brief
 * Rom Engine last routine executed
 *
 * \details 
 * 00000: 10G	- configured for 10G mode
 * 00001: TX10G - Tx configured for 10G mode
 * 00010: RX10G - Rx configured for 10G mode
 * 00011: 1G	- configured for 1G mode
 * 00100: TX1G	- Rx configured for 1G mode
 * 00101: RX1G	- Rx configured for 1G mode
 * 00110: 3G	- configured for 3G mode
 * 00111: TX3G	- Rx configured for 3G mode
 * 01000: RX3G	- Rx configured for 3G mode
 * 01001: WAN	- configured for WAN mode
 * 01010: RST	- configured to Reset condition
 * 01011: LBON	- configured for Loopback enabled
 * 01100: LBOFF - configured for Loopback disabled
 * 01101: LPON	- LowPower mode enabled
 * 01110: LPOFF - LowPower mode disabled
 * 01111: RC	- RCOMP routine
 * 10000: LRON	- Lock2Ref enabled
 * 10001: LROFF - Lock2Ref disabled
 * others: invalid
 *
 * Field: VTSS_HOST_PMA_ROMENG_STATUS_ROMENG_STATUS . exe_last
 */
#define  VTSS_F_HOST_PMA_ROMENG_STATUS_ROMENG_STATUS_exe_last(x)  VTSS_ENCODE_BITFIELD(x,1,5)
#define  VTSS_M_HOST_PMA_ROMENG_STATUS_ROMENG_STATUS_exe_last     VTSS_ENCODE_BITMASK(1,5)
#define  VTSS_X_HOST_PMA_ROMENG_STATUS_ROMENG_STATUS_exe_last(x)  VTSS_EXTRACT_BITFIELD(x,1,5)

/** 
 * \brief
 * Rom Engine status
 * This is a sticky bit that latches the high state.  The latch-high bit is
 * cleared when the register is read.

 *
 * \details 
 * 0: Rom Engine has not executed a new routine since the last time this
 * bit is read
 * 1: Rom Engine has executed a new routine since the last time this bit is
 * read
 *
 * Field: VTSS_HOST_PMA_ROMENG_STATUS_ROMENG_STATUS . exe_done
 */
#define  VTSS_F_HOST_PMA_ROMENG_STATUS_ROMENG_STATUS_exe_done  VTSS_BIT(0)

/**
 * Register Group: \a HOST_PMA:SD10G65_RX_RCPLL_BIST
 *
 * SD10G65 RX and TX RCPLL BIST Configuration and Status Register set
 */


/** 
 * \brief SD10G65 RX RCPLL BIST Configuration register 0A
 *
 * \details
 * Configuration register 0A for SD10G65 RX RCPLL BIST.
 *
 * Register: \a HOST_PMA:SD10G65_RX_RCPLL_BIST:SD10G65_RX_RCPLL_BIST_CFG0A
 */
#define VTSS_HOST_PMA_SD10G65_RX_RCPLL_BIST_SD10G65_RX_RCPLL_BIST_CFG0A  VTSS_IOREG(0x09, 0, 0xb800)


/** 
 * \brief SD10G65 RX RCPLL BIST Configuration register 0B
 *
 * \details
 * Configuration register 0B for SD10G65 RX RCPLL BIST.
 *
 * Register: \a HOST_PMA:SD10G65_RX_RCPLL_BIST:SD10G65_RX_RCPLL_BIST_CFG0B
 */
#define VTSS_HOST_PMA_SD10G65_RX_RCPLL_BIST_SD10G65_RX_RCPLL_BIST_CFG0B  VTSS_IOREG(0x09, 0, 0xb801)


/** 
 * \brief SD10G65 RX RCPLL BIST Configuration register 1A
 *
 * \details
 * Configuration register 1A for SD10G65 RX RCPLL BIST.
 *
 * Register: \a HOST_PMA:SD10G65_RX_RCPLL_BIST:SD10G65_RX_RCPLL_BIST_CFG1A
 */
#define VTSS_HOST_PMA_SD10G65_RX_RCPLL_BIST_SD10G65_RX_RCPLL_BIST_CFG1A  VTSS_IOREG(0x09, 0, 0xb802)


/** 
 * \brief SD10G65 RX RCPLL BIST Configuration register 1B
 *
 * \details
 * Configuration register 1B for SD10G65 RX RCPLL BIST.
 *
 * Register: \a HOST_PMA:SD10G65_RX_RCPLL_BIST:SD10G65_RX_RCPLL_BIST_CFG1B
 */
#define VTSS_HOST_PMA_SD10G65_RX_RCPLL_BIST_SD10G65_RX_RCPLL_BIST_CFG1B  VTSS_IOREG(0x09, 0, 0xb803)


/** 
 * \brief SD10G65 RX RCPLL BIST Configuration register 2
 *
 * \details
 * Configuration register 2 for SD10G65 RX RCPLL BIST.
 *
 * Register: \a HOST_PMA:SD10G65_RX_RCPLL_BIST:SD10G65_RX_RCPLL_BIST_CFG2
 */
#define VTSS_HOST_PMA_SD10G65_RX_RCPLL_BIST_SD10G65_RX_RCPLL_BIST_CFG2  VTSS_IOREG(0x09, 0, 0xb804)


/** 
 * \brief SD10G65 RX RCPLL BIST Configuration register 3
 *
 * \details
 * Configuration register 3 for SD10G65 RX RCPLL BIST.
 *
 * Register: \a HOST_PMA:SD10G65_RX_RCPLL_BIST:SD10G65_RX_RCPLL_BIST_CFG3
 */
#define VTSS_HOST_PMA_SD10G65_RX_RCPLL_BIST_SD10G65_RX_RCPLL_BIST_CFG3  VTSS_IOREG(0x09, 0, 0xb805)


/** 
 * \brief SD10G65 RX RCPLL BIST Configuration register 4
 *
 * \details
 * Configuration register 4 for SD10G65 RX RCPLL BIST.
 *
 * Register: \a HOST_PMA:SD10G65_RX_RCPLL_BIST:SD10G65_RX_RCPLL_BIST_CFG4
 */
#define VTSS_HOST_PMA_SD10G65_RX_RCPLL_BIST_SD10G65_RX_RCPLL_BIST_CFG4  VTSS_IOREG(0x09, 0, 0xb806)


/** 
 * \brief SD10G65 RX RCPLL BIST Status register 0A
 *
 * \details
 * Status register 0A for SD10G65 RX RCPLL BIST.
 *
 * Register: \a HOST_PMA:SD10G65_RX_RCPLL_BIST:SD10G65_RX_RCPLL_BIST_STAT0A
 */
#define VTSS_HOST_PMA_SD10G65_RX_RCPLL_BIST_SD10G65_RX_RCPLL_BIST_STAT0A  VTSS_IOREG(0x09, 0, 0xb807)


/** 
 * \brief SD10G65 RX RCPLL BIST Status register 0B
 *
 * \details
 * Status register 0B for SD10G65 RX RCPLL BIST.
 *
 * Register: \a HOST_PMA:SD10G65_RX_RCPLL_BIST:SD10G65_RX_RCPLL_BIST_STAT0B
 */
#define VTSS_HOST_PMA_SD10G65_RX_RCPLL_BIST_SD10G65_RX_RCPLL_BIST_STAT0B  VTSS_IOREG(0x09, 0, 0xb808)


/** 
 * \brief SD10G65 RX RCPLL BIST Status register 1
 *
 * \details
 * Status register 1 for SD10G65 RX RCPLL BIST.
 *
 * Register: \a HOST_PMA:SD10G65_RX_RCPLL_BIST:SD10G65_RX_RCPLL_BIST_STAT1
 */
#define VTSS_HOST_PMA_SD10G65_RX_RCPLL_BIST_SD10G65_RX_RCPLL_BIST_STAT1  VTSS_IOREG(0x09, 0, 0xb809)


#endif /* _VTSS_MALIBU_REGS_HOST_PMA_H_ */
