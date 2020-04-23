#ifndef _VTSS_MALIBU_REGS_GLOBAL_RESET_H_
#define _VTSS_MALIBU_REGS_GLOBAL_RESET_H_

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
 * Target: \a GLOBAL_RESET
 *
 * 
 *
 ***********************************************************************/

/**
 * Register Group: \a GLOBAL_RESET:FAST_ACCESS_RESET
 *
 * FAST RESET REGISTERS ARE NOT ON A CSR RING
 */


/** 
 * \brief FAST RESET REGISTERS ARE NOT ON A CSR RING
 *
 * \details
 * Fast access reset registers are not on a CSR ring
 *
 * Register: \a GLOBAL_RESET:FAST_ACCESS_RESET:GLOBAL_FAST_RESET
 */
#define VTSS_GLOBAL_RESET_FAST_ACCESS_RESET_GLOBAL_FAST_RESET  VTSS_IOREG(0x1e, 0, 0x8000)

/** 
 * \brief
 * Self-clearing software reset register
 *
 * \details 
 * 0: Normal operation
 * 1: Reset CSR ring 2

 *
 * Field: VTSS_GLOBAL_RESET_FAST_ACCESS_RESET_GLOBAL_FAST_RESET . CSR_RING_5_FAST_RESET
 */
#define  VTSS_F_GLOBAL_RESET_FAST_ACCESS_RESET_GLOBAL_FAST_RESET_CSR_RING_5_FAST_RESET  VTSS_BIT(13)

/** 
 * \brief
 * Self-clearing software reset register
 *
 * \details 
 * 0: Normal operation
 * 1: Reset CSR ring 2

 *
 * Field: VTSS_GLOBAL_RESET_FAST_ACCESS_RESET_GLOBAL_FAST_RESET . CSR_RING_4_FAST_RESET
 */
#define  VTSS_F_GLOBAL_RESET_FAST_ACCESS_RESET_GLOBAL_FAST_RESET_CSR_RING_4_FAST_RESET  VTSS_BIT(12)

/** 
 * \brief
 * Self-clearing software reset register
 *
 * \details 
 * 0: Normal operation
 * 1: Reset CSR ring 2

 *
 * Field: VTSS_GLOBAL_RESET_FAST_ACCESS_RESET_GLOBAL_FAST_RESET . CSR_RING_3_FAST_RESET
 */
#define  VTSS_F_GLOBAL_RESET_FAST_ACCESS_RESET_GLOBAL_FAST_RESET_CSR_RING_3_FAST_RESET  VTSS_BIT(11)

/** 
 * \brief
 * Self-clearing software reset register
 *
 * \details 
 * 0: Normal operation
 * 1: Reset CSR ring 2

 *
 * Field: VTSS_GLOBAL_RESET_FAST_ACCESS_RESET_GLOBAL_FAST_RESET . CSR_RING_2_FAST_RESET
 */
#define  VTSS_F_GLOBAL_RESET_FAST_ACCESS_RESET_GLOBAL_FAST_RESET_CSR_RING_2_FAST_RESET  VTSS_BIT(10)

/** 
 * \brief
 * Self-clearing software reset register
 *
 * \details 
 * 0: Normal operation
 * 1: Reset CSR ring 1

 *
 * Field: VTSS_GLOBAL_RESET_FAST_ACCESS_RESET_GLOBAL_FAST_RESET . CSR_RING_1_FAST_RESET
 */
#define  VTSS_F_GLOBAL_RESET_FAST_ACCESS_RESET_GLOBAL_FAST_RESET_CSR_RING_1_FAST_RESET  VTSS_BIT(9)

/** 
 * \brief
 * Self-clearing software reset register
 *
 * \details 
 * 0: Normal operation
 * 1: Reset CSR ring 0

 *
 * Field: VTSS_GLOBAL_RESET_FAST_ACCESS_RESET_GLOBAL_FAST_RESET . CSR_RING_0_FAST_RESET
 */
#define  VTSS_F_GLOBAL_RESET_FAST_ACCESS_RESET_GLOBAL_FAST_RESET_CSR_RING_0_FAST_RESET  VTSS_BIT(8)

/** 
 * \brief
 * Self-clearing software reset register
 *
 * \details 
 * 0: Normal operation
 * 1: Reset channel 1 and CSR ring 1

 *
 * Field: VTSS_GLOBAL_RESET_FAST_ACCESS_RESET_GLOBAL_FAST_RESET . CHANNEL_3_FAST_RESET
 */
#define  VTSS_F_GLOBAL_RESET_FAST_ACCESS_RESET_GLOBAL_FAST_RESET_CHANNEL_3_FAST_RESET  VTSS_BIT(7)

/** 
 * \brief
 * Self-clearing software reset register
 *
 * \details 
 * 0: Normal operation
 * 1: Reset channel 1 and CSR ring 1

 *
 * Field: VTSS_GLOBAL_RESET_FAST_ACCESS_RESET_GLOBAL_FAST_RESET . CHANNEL_2_FAST_RESET
 */
#define  VTSS_F_GLOBAL_RESET_FAST_ACCESS_RESET_GLOBAL_FAST_RESET_CHANNEL_2_FAST_RESET  VTSS_BIT(6)

/** 
 * \brief
 * Self-clearing software reset register
 *
 * \details 
 * 0: Normal operation
 * 1: Reset channel 1 and CSR ring 1

 *
 * Field: VTSS_GLOBAL_RESET_FAST_ACCESS_RESET_GLOBAL_FAST_RESET . CHANNEL_1_FAST_RESET
 */
#define  VTSS_F_GLOBAL_RESET_FAST_ACCESS_RESET_GLOBAL_FAST_RESET_CHANNEL_1_FAST_RESET  VTSS_BIT(5)

/** 
 * \brief
 * Self-clearing software reset register
 *
 * \details 
 * 0: Normal operation
 * 1: Reset channel 0 and CSR ring 0

 *
 * Field: VTSS_GLOBAL_RESET_FAST_ACCESS_RESET_GLOBAL_FAST_RESET . CHANNEL_0_FAST_RESET
 */
#define  VTSS_F_GLOBAL_RESET_FAST_ACCESS_RESET_GLOBAL_FAST_RESET_CHANNEL_0_FAST_RESET  VTSS_BIT(4)

/** 
 * \brief
 * Reset the datapath in both channels and all configuration registers
 * except those used for global configuration.
 *
 * \details 
 * 0: Normal operation
 * 1: Reset

 *
 * Field: VTSS_GLOBAL_RESET_FAST_ACCESS_RESET_GLOBAL_FAST_RESET . CHIP_FAST_RESET
 */
#define  VTSS_F_GLOBAL_RESET_FAST_ACCESS_RESET_GLOBAL_FAST_RESET_CHIP_FAST_RESET  VTSS_BIT(0)


#endif /* _VTSS_MALIBU_REGS_GLOBAL_RESET_H_ */
