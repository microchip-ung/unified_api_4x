#ifndef _VTSS_LUTON26_REGS_MEMCTRL_H_
#define _VTSS_LUTON26_REGS_MEMCTRL_H_

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
 * Target: \a MEMCTRL
 *
 * 
 *
 ***********************************************************************/

/**
 * Register Group: \a MEMCTRL::MEMCTRL
 *
 * DDR memory controller registers
 */


/** 
 * \brief SDRAM Config Register
 *
 * \details
 * Register: \a MEMCTRL::MEMCTRL::SCONR
 */
#define VTSS_MEMCTRL_MEMCTRL_SCONR           VTSS_IOREG(VTSS_TO_MEMCTRL,0x0)

/** 
 * \brief
 * Number of address bits for column address.
 *
 * \details 
 * 7-12: Correspond to 8-13 bits
 * Unused values are reserved.
 *
 * Field: ::VTSS_MEMCTRL_MEMCTRL_SCONR . S_COL_ADDR_WIDTH
 */
#define  VTSS_F_MEMCTRL_MEMCTRL_SCONR_S_COL_ADDR_WIDTH(x)  VTSS_ENCODE_BITFIELD(x,9,4)

/** 
 * \brief
 * Number of address bits for row address.
 *
 * \details 
 * 10-13: correspond to 11-14 bits
 * Unused values are reserved.
 *
 * Field: ::VTSS_MEMCTRL_MEMCTRL_SCONR . S_ROW_ADDR_WIDTH
 */
#define  VTSS_F_MEMCTRL_MEMCTRL_SCONR_S_ROW_ADDR_WIDTH(x)  VTSS_ENCODE_BITFIELD(x,5,4)


/** 
 * \brief SDRAM Timing Register0
 *
 * \details
 * Register: \a MEMCTRL::MEMCTRL::STMG0R
 */
#define VTSS_MEMCTRL_MEMCTRL_STMG0R          VTSS_IOREG(VTSS_TO_MEMCTRL,0x1)

/** 
 * \brief
 * See CAS_LATENCY.

 *
 * \details 
 * Field: ::VTSS_MEMCTRL_MEMCTRL_STMG0R . EXTENDED_CAS_LATENCY
 */
#define  VTSS_F_MEMCTRL_MEMCTRL_STMG0R_EXTENDED_CAS_LATENCY  VTSS_BIT(26)

/** 
 * \brief
 * Active-to-active command period.
 *
 * \details 
 * Values of 0-15 correspond to t_rc of 1-16 clocks.
 *
 * Field: ::VTSS_MEMCTRL_MEMCTRL_STMG0R . T_RC
 */
#define  VTSS_F_MEMCTRL_MEMCTRL_STMG0R_T_RC(x)  VTSS_ENCODE_BITFIELD(x,22,4)

/** 
 * \brief
 * Auto-refresh period; minimum time between two auto-refresh commands.
 *
 * \details 
 * Values 0-15 correspond to t_rcar of 1-16 clocks.
 *
 * Field: ::VTSS_MEMCTRL_MEMCTRL_STMG0R . T_RCAR
 */
#define  VTSS_F_MEMCTRL_MEMCTRL_STMG0R_T_RCAR(x)  VTSS_ENCODE_BITFIELD(x,14,4)

/** 
 * \brief
 * For writes, delay from last data in to next precharge command.
 *
 * \details 
 * Values 0-3 correspond to t_wr of 1-4 clocks.
 *
 * Field: ::VTSS_MEMCTRL_MEMCTRL_STMG0R . T_WR
 */
#define  VTSS_F_MEMCTRL_MEMCTRL_STMG0R_T_WR(x)  VTSS_ENCODE_BITFIELD(x,12,2)

/** 
 * \brief
 * Precharge period.
 *
 * \details 
 * Values of 0-7 correspond to t_rp of 1-8 clocks.
 *
 * Field: ::VTSS_MEMCTRL_MEMCTRL_STMG0R . T_RP
 */
#define  VTSS_F_MEMCTRL_MEMCTRL_STMG0R_T_RP(x)  VTSS_ENCODE_BITFIELD(x,9,3)

/** 
 * \brief
 * Minimum delay between active and read/write commands.
 *
 * \details 
 * Values 0-7 correspond to t_rcd values of 1-8 clocks.
 *
 * Field: ::VTSS_MEMCTRL_MEMCTRL_STMG0R . T_RCD
 */
#define  VTSS_F_MEMCTRL_MEMCTRL_STMG0R_T_RCD(x)  VTSS_ENCODE_BITFIELD(x,6,3)

/** 
 * \brief
 * Minimum delay between active and precharge commands.
 *
 * \details 
 * Values of 0-15 correspond to T_RAS_MIN of 1-16 clocks.
 *
 * Field: ::VTSS_MEMCTRL_MEMCTRL_STMG0R . T_RAS_MIN
 */
#define  VTSS_F_MEMCTRL_MEMCTRL_STMG0R_T_RAS_MIN(x)  VTSS_ENCODE_BITFIELD(x,2,4)

/** 
 * \brief
 * This field is extended with the EXTENDED_CAS_LATENCY field.
 * Delay in clock cycles between read command and availability of first
 * data.
 *
 * \details 
 * 0: 1 clock
 * 1: 2 clocks
 * 2: 3 clocks
 * 3: 4 clocks
 * 4: 1.5 clocks
 * 5: 2.5 clocks
 * 6, 7: reserved
 *
 * Field: ::VTSS_MEMCTRL_MEMCTRL_STMG0R . CAS_LATENCY
 */
#define  VTSS_F_MEMCTRL_MEMCTRL_STMG0R_CAS_LATENCY(x)  VTSS_ENCODE_BITFIELD(x,0,2)


/** 
 * \brief SDRAM Timing Register1
 *
 * \details
 * Register: \a MEMCTRL::MEMCTRL::STMG1R
 */
#define VTSS_MEMCTRL_MEMCTRL_STMG1R          VTSS_IOREG(VTSS_TO_MEMCTRL,0x2)

/** 
 * \brief
 * Internal write-to-read delay for DDR-SDRAMs.
 *
 * \details 
 * Values of 0-3 represent 1- 4 clocks delay.
 *
 * Field: ::VTSS_MEMCTRL_MEMCTRL_STMG1R . T_WTR
 */
#define  VTSS_F_MEMCTRL_MEMCTRL_STMG1R_T_WTR(x)  VTSS_ENCODE_BITFIELD(x,20,2)


/** 
 * \brief SDRAM Control Register
 *
 * \details
 * Register: \a MEMCTRL::MEMCTRL::SCTLR
 */
#define VTSS_MEMCTRL_MEMCTRL_SCTLR           VTSS_IOREG(VTSS_TO_MEMCTRL,0x3)

/** 
 * \brief
 * When set to 1, forces controller to do update of SDRAM extended mode
 * register. The bit is cleared by controller after it finishes extended
 * mode register update.
 *
 * \details 
 * Field: ::VTSS_MEMCTRL_MEMCTRL_SCTLR . SET_EXN_MODE_REG
 */
#define  VTSS_F_MEMCTRL_MEMCTRL_SCTLR_SET_EXN_MODE_REG  VTSS_BIT(18)

/** 
 * \brief
 * When set to 1, forces controller to do update of SDRAM mode register.
 * The bit is cleared by controller once it finishes mode register update.
 *
 * \details 
 * Field: ::VTSS_MEMCTRL_MEMCTRL_SCTLR . SET_MODE_REG
 */
#define  VTSS_F_MEMCTRL_MEMCTRL_SCTLR_SET_MODE_REG  VTSS_BIT(9)

/** 
 * \brief
 * When reading from the memory, this indicates a delay in clock-cycles
 * before data from SDRAM is ready inside controller. DDR400 requires 2
 * cycles.
 *
 * \details 
 * Values 0-7 indicate 0-7 clock-cycles.
 *
 * Field: ::VTSS_MEMCTRL_MEMCTRL_SCTLR . READ_PIPE
 */
#define  VTSS_F_MEMCTRL_MEMCTRL_SCTLR_READ_PIPE(x)  VTSS_ENCODE_BITFIELD(x,6,3)

/** 
 * \brief
 * Forces SDRAM controller to initialize SDRAM. This field is automatically
 * reset to 0 once initialization sequence is complete.
 *
 * \details 
 * Field: ::VTSS_MEMCTRL_MEMCTRL_SCTLR . INITIALIZE
 */
#define  VTSS_F_MEMCTRL_MEMCTRL_SCTLR_INITIALIZE  VTSS_BIT(0)


/** 
 * \brief SDRAM Refresh Interval Register
 *
 * \details
 * Register: \a MEMCTRL::MEMCTRL::SREFR
 */
#define VTSS_MEMCTRL_MEMCTRL_SREFR           VTSS_IOREG(VTSS_TO_MEMCTRL,0x4)

/** 
 * \brief
 * Number of clock cycles between consecutive refresh cycles.
 *
 * \details 
 * Field: ::VTSS_MEMCTRL_MEMCTRL_SREFR . T_REF
 */
#define  VTSS_F_MEMCTRL_MEMCTRL_SREFR_T_REF(x)  VTSS_ENCODE_BITFIELD(x,0,16)


/** 
 * \brief Extended Mode Register
 *
 * \details
 * Register: \a MEMCTRL::MEMCTRL::EXN_MODE_REG
 */
#define VTSS_MEMCTRL_MEMCTRL_EXN_MODE_REG    VTSS_IOREG(VTSS_TO_MEMCTRL,0x2b)

/** 
 * \brief
 * See datasheet for the selected SDRAM.
 *
 * \details 
 * 0: Normal operation
 * Others: Reserved
 *
 * Field: ::VTSS_MEMCTRL_MEMCTRL_EXN_MODE_REG . OPERATION_MODE
 */
#define  VTSS_F_MEMCTRL_MEMCTRL_EXN_MODE_REG_OPERATION_MODE(x)  VTSS_ENCODE_BITFIELD(x,3,10)

/** 
 * \brief
 * See datasheet for the selected SDRAM.
 *
 * \details 
 * 0: Disable
 * 1: Enable (optional)
 *
 * Field: ::VTSS_MEMCTRL_MEMCTRL_EXN_MODE_REG . QFC
 */
#define  VTSS_F_MEMCTRL_MEMCTRL_EXN_MODE_REG_QFC  VTSS_BIT(2)

/** 
 * \brief
 * See datasheet for the selected SDRAM.
 *
 * \details 
 * 0: Normal
 * 1: Weak (optional)
 *
 * Field: ::VTSS_MEMCTRL_MEMCTRL_EXN_MODE_REG . DRIVE_STRENGTH
 */
#define  VTSS_F_MEMCTRL_MEMCTRL_EXN_MODE_REG_DRIVE_STRENGTH  VTSS_BIT(1)

/** 
 * \brief
 * See datasheet for the selected SDRAM.
 *
 * \details 
 * 0: DLL disable
 * 1: DDL enable
 *
 * Field: ::VTSS_MEMCTRL_MEMCTRL_EXN_MODE_REG . DLL
 */
#define  VTSS_F_MEMCTRL_MEMCTRL_EXN_MODE_REG_DLL  VTSS_BIT(0)


#endif /* _VTSS_LUTON26_REGS_MEMCTRL_H_ */
