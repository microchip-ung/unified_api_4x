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

#ifndef _VTSS_PHY_TYPES_H_
#define _VTSS_PHY_TYPES_H_

#include <vtss/api/types.h>

/**
 * \file
 * \brief PHY Public API Header
 * \details This header file describes public PHY data-types
 */

#ifdef __cplusplus
extern "C" {
#endif

#define VTSS_PHY_POWER_ACTIPHY_BIT 0 /**< Defines the bit used to signaling that ActiPhy is enabled */
#define VTSS_PHY_POWER_DYNAMIC_BIT 1 /**< Defines the bit used to signaling that PerfectReach is enabled */

/** \brief PHY power reduction modes */
typedef enum {
    VTSS_PHY_POWER_NOMINAL = 0, /**< Default power settings */
    VTSS_PHY_POWER_ACTIPHY = 1 << VTSS_PHY_POWER_ACTIPHY_BIT, /**< ActiPHY - Link down power savings enabled (Bit 0) */
    VTSS_PHY_POWER_DYNAMIC = 1 << VTSS_PHY_POWER_DYNAMIC_BIT, /**< PerfectReach - Link up power savings enabled (Bit 1) */
    VTSS_PHY_POWER_ENABLED = VTSS_PHY_POWER_ACTIPHY +  VTSS_PHY_POWER_DYNAMIC /**< ActiPHY + PerfectReach enabled */
} vtss_phy_power_mode_t;

/** \brief VeriPHY status */
typedef enum {
    VTSS_VERIPHY_STATUS_OK      = 0,  /**< Correctly terminated pair */
    VTSS_VERIPHY_STATUS_OPEN    = 1,  /**< Open pair */
    VTSS_VERIPHY_STATUS_SHORT   = 2,  /**< Short pair */
    VTSS_VERIPHY_STATUS_ABNORM  = 4,  /**< Abnormal termination */
    VTSS_VERIPHY_STATUS_SHORT_A = 8,  /**< Cross-pair short to pair A */
    VTSS_VERIPHY_STATUS_SHORT_B = 9,  /**< Cross-pair short to pair B */
    VTSS_VERIPHY_STATUS_SHORT_C = 10, /**< Cross-pair short to pair C */
    VTSS_VERIPHY_STATUS_SHORT_D = 11, /**< Cross-pair short to pair D */
    VTSS_VERIPHY_STATUS_COUPL_A = 12, /**< Abnormal cross-pair coupling, pair A */
    VTSS_VERIPHY_STATUS_COUPL_B = 13, /**< Abnormal cross-pair coupling, pair B */
    VTSS_VERIPHY_STATUS_COUPL_C = 14, /**< Abnormal cross-pair coupling, pair C */
    VTSS_VERIPHY_STATUS_COUPL_D = 15, /**< Abnormal cross-pair coupling, pair D */
    VTSS_VERIPHY_STATUS_UNKNOWN = 16, /**< Unknown - VeriPhy never started ? */
    VTSS_VERIPHY_STATUS_RUNNING = 17  /**< VeriPhy is still running - No result yet */
} vtss_phy_veriphy_status_t;




#ifdef __cplusplus
}
#endif

#endif /* #define _VTSS_PHY_TYPES_H_ */
