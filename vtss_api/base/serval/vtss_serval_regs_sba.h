#ifndef _VTSS_SERVAL_REGS_SBA_H_
#define _VTSS_SERVAL_REGS_SBA_H_

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

#include "vtss_serval_regs_common.h"

/*********************************************************************** 
 *
 * Target: \a SBA
 *
 * Shared bus arbiter
 *
 ***********************************************************************/

/**
 * Register Group: \a SBA:SBA
 *
 * Shared bus arbiter registers
 */


/** 
 * \brief Master 1 arbitration priority
 *
 * \details
 * Register: \a SBA:SBA:PL_CPU
 */
#define VTSS_SBA_SBA_PL_CPU                  VTSS_IOREG(VTSS_TO_SBA,0x0)

/** 
 * \brief
 * Arbitration priority for the master. When multiple masters request the
 * bus at the same time, the one with the highest priority is ganted bus
 * access.
 *
 * \details 
 * Values 0x1 through 0xF, higher values are prioritized over lower values.
 *
 * Field: ::VTSS_SBA_SBA_PL_CPU . PL1
 */
#define  VTSS_F_SBA_SBA_PL_CPU_PL1(x)         VTSS_ENCODE_BITFIELD(x,0,4)
#define  VTSS_M_SBA_SBA_PL_CPU_PL1            VTSS_ENCODE_BITMASK(0,4)
#define  VTSS_X_SBA_SBA_PL_CPU_PL1(x)         VTSS_EXTRACT_BITFIELD(x,0,4)


/** 
 * \brief Master 2 arbitration priority
 *
 * \details
 * Register: \a SBA:SBA:PL_PCIE
 */
#define VTSS_SBA_SBA_PL_PCIE                 VTSS_IOREG(VTSS_TO_SBA,0x1)

/** 
 * \brief
 * See SBA::PL1 for description.
 *
 * \details 
 * Field: ::VTSS_SBA_SBA_PL_PCIE . PL2
 */
#define  VTSS_F_SBA_SBA_PL_PCIE_PL2(x)        VTSS_ENCODE_BITFIELD(x,0,4)
#define  VTSS_M_SBA_SBA_PL_PCIE_PL2           VTSS_ENCODE_BITMASK(0,4)
#define  VTSS_X_SBA_SBA_PL_PCIE_PL2(x)        VTSS_EXTRACT_BITFIELD(x,0,4)


/** 
 * \brief Master 3 arbitration priority
 *
 * \details
 * Register: \a SBA:SBA:PL_CSR
 */
#define VTSS_SBA_SBA_PL_CSR                  VTSS_IOREG(VTSS_TO_SBA,0x2)

/** 
 * \brief
 * See SBA::PL1 for description.
 *
 * \details 
 * Field: ::VTSS_SBA_SBA_PL_CSR . PL3
 */
#define  VTSS_F_SBA_SBA_PL_CSR_PL3(x)         VTSS_ENCODE_BITFIELD(x,0,4)
#define  VTSS_M_SBA_SBA_PL_CSR_PL3            VTSS_ENCODE_BITMASK(0,4)
#define  VTSS_X_SBA_SBA_PL_CSR_PL3(x)         VTSS_EXTRACT_BITFIELD(x,0,4)


/** 
 * \brief Weighted-token arbitration scheme enable
 *
 * \details
 * When weighted token arbitration is enabled, each master on the shared
 * bus is granted a configurable number of tokens at the start of each
 * refresh period. The length of each refresh period is configurable. In
 * each clock-cycle that a master uses the bus, the token counter for that
 * master decreases. Once all tokens are spent, the master is forced to a
 * low priority. A master with tokens remaining, always takes priority over
 * masters with no tokens remaining.
 *
 * Register: \a SBA:SBA:WT_EN
 */
#define VTSS_SBA_SBA_WT_EN                   VTSS_IOREG(VTSS_TO_SBA,0x13)

/** 
 * \brief
 * Set this field to enable weighted-token arbitration scheme.
 *
 * \details 
 * Field: ::VTSS_SBA_SBA_WT_EN . WT_EN
 */
#define  VTSS_F_SBA_SBA_WT_EN_WT_EN           VTSS_BIT(0)


/** 
 * \brief Clock tokens refresh period
 *
 * \details
 * Register: \a SBA:SBA:WT_TCL
 */
#define VTSS_SBA_SBA_WT_TCL                  VTSS_IOREG(VTSS_TO_SBA,0x14)

/** 
 * \brief
 * Refresh period length for the weighted-token arbitration scheme.
 *
 * \details 
 * Field: ::VTSS_SBA_SBA_WT_TCL . WT_TCL
 */
#define  VTSS_F_SBA_SBA_WT_TCL_WT_TCL(x)      VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_SBA_SBA_WT_TCL_WT_TCL         VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_SBA_SBA_WT_TCL_WT_TCL(x)      VTSS_EXTRACT_BITFIELD(x,0,16)


/** 
 * \brief Master 1 clock tokens
 *
 * \details
 * Register: \a SBA:SBA:WT_CPU
 */
#define VTSS_SBA_SBA_WT_CPU                  VTSS_IOREG(VTSS_TO_SBA,0x15)

/** 
 * \brief
 * Number of tokens to grant the master at the start of each refresh period
 * for weighted-token arbitration scheme. If configured with a value of
 * zero, the master is considered to have infinite tokens.
 *
 * \details 
 * Field: ::VTSS_SBA_SBA_WT_CPU . WT_CL1
 */
#define  VTSS_F_SBA_SBA_WT_CPU_WT_CL1(x)      VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_SBA_SBA_WT_CPU_WT_CL1         VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_SBA_SBA_WT_CPU_WT_CL1(x)      VTSS_EXTRACT_BITFIELD(x,0,16)


/** 
 * \brief Master 2 clock tokens
 *
 * \details
 * Register: \a SBA:SBA:WT_PCIE
 */
#define VTSS_SBA_SBA_WT_PCIE                 VTSS_IOREG(VTSS_TO_SBA,0x16)

/** 
 * \brief
 * See SBA::WT_CL1 for description.
 *
 * \details 
 * Field: ::VTSS_SBA_SBA_WT_PCIE . WT_CL2
 */
#define  VTSS_F_SBA_SBA_WT_PCIE_WT_CL2(x)     VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_SBA_SBA_WT_PCIE_WT_CL2        VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_SBA_SBA_WT_PCIE_WT_CL2(x)     VTSS_EXTRACT_BITFIELD(x,0,16)


/** 
 * \brief Master 3 clock tokens
 *
 * \details
 * Register: \a SBA:SBA:WT_CSR
 */
#define VTSS_SBA_SBA_WT_CSR                  VTSS_IOREG(VTSS_TO_SBA,0x17)

/** 
 * \brief
 * See SBA::WT_CL1 for description.
 *
 * \details 
 * Field: ::VTSS_SBA_SBA_WT_CSR . WT_CL3
 */
#define  VTSS_F_SBA_SBA_WT_CSR_WT_CL3(x)      VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_SBA_SBA_WT_CSR_WT_CL3         VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_SBA_SBA_WT_CSR_WT_CL3(x)      VTSS_EXTRACT_BITFIELD(x,0,16)


#endif /* _VTSS_SERVAL_REGS_SBA_H_ */
