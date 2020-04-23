#ifndef _VTSS_LUTON26_REGS_DEV_CMN_H_
#define _VTSS_LUTON26_REGS_DEV_CMN_H_

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

#include "vtss_luton26_regs_dev_gmii.h"
#include "vtss_luton26_regs_dev.h"

/*
 * Abstraction macros for functionally identical registers 
 * in the DEV and DEV_GMII targets.
 *
 * Caution: These macros may not work a lvalues, depending
 * on compiler and platform.
 *
 */

#define VTSS_DEV_CMN_MAC_CFG_STATUS_MAC_MODE_CFG(t) ( (t) >= VTSS_TO_DEV_10 ? VTSS_DEV_MAC_CFG_STATUS_MAC_MODE_CFG(t) : VTSS_DEV_GMII_MAC_CFG_STATUS_MAC_MODE_CFG(t) )

#define VTSS_DEV_CMN_MAC_CFG_STATUS_MAC_IFG_CFG(t) ( (t) >= VTSS_TO_DEV_10 ? VTSS_DEV_MAC_CFG_STATUS_MAC_IFG_CFG(t) : VTSS_DEV_GMII_MAC_CFG_STATUS_MAC_IFG_CFG(t) )

#define VTSS_DEV_CMN_MAC_CFG_STATUS_MAC_ADV_CHK_CFG(t) ( (t) >= VTSS_TO_DEV_10 ? VTSS_DEV_MAC_CFG_STATUS_MAC_ADV_CHK_CFG(t) : VTSS_DEV_GMII_MAC_CFG_STATUS_MAC_ADV_CHK_CFG(t) )

#define VTSS_DEV_CMN_MAC_CFG_STATUS_MAC_TAGS_CFG(t) ( (t) >= VTSS_TO_DEV_10 ? VTSS_DEV_MAC_CFG_STATUS_MAC_TAGS_CFG(t) : VTSS_DEV_GMII_MAC_CFG_STATUS_MAC_TAGS_CFG(t) )

#define VTSS_DEV_CMN_MAC_CFG_STATUS_MAC_HDX_CFG(t) ( (t) >= VTSS_TO_DEV_10 ? VTSS_DEV_MAC_CFG_STATUS_MAC_HDX_CFG(t) : VTSS_DEV_GMII_MAC_CFG_STATUS_MAC_HDX_CFG(t) )

#define VTSS_DEV_CMN_MAC_CFG_STATUS_MAC_ENA_CFG(t) ( (t) >= VTSS_TO_DEV_10 ? VTSS_DEV_MAC_CFG_STATUS_MAC_ENA_CFG(t) : VTSS_DEV_GMII_MAC_CFG_STATUS_MAC_ENA_CFG(t) )

#define VTSS_DEV_CMN_MAC_CFG_STATUS_MAC_FC_CFG(t) ( (t) >= VTSS_TO_DEV_10 ? VTSS_DEV_MAC_CFG_STATUS_MAC_FC_CFG(t) : VTSS_DEV_GMII_MAC_CFG_STATUS_MAC_FC_CFG(t) )

#define VTSS_DEV_CMN_MAC_CFG_STATUS_MAC_FC_MAC_HIGH_CFG(t) ( (t) >= VTSS_TO_DEV_10 ? VTSS_DEV_MAC_CFG_STATUS_MAC_FC_MAC_HIGH_CFG(t) : VTSS_DEV_GMII_MAC_CFG_STATUS_MAC_FC_MAC_HIGH_CFG(t) )

#define VTSS_DEV_CMN_MAC_CFG_STATUS_MAC_MAXLEN_CFG(t) ( (t) >= VTSS_TO_DEV_10 ? VTSS_DEV_MAC_CFG_STATUS_MAC_MAXLEN_CFG(t) : VTSS_DEV_GMII_MAC_CFG_STATUS_MAC_MAXLEN_CFG(t) )

#define VTSS_DEV_CMN_MAC_CFG_STATUS_MAC_STICKY(t) ( (t) >= VTSS_TO_DEV_10 ? VTSS_DEV_MAC_CFG_STATUS_MAC_STICKY(t) : VTSS_DEV_GMII_MAC_CFG_STATUS_MAC_STICKY(t) )

#define VTSS_DEV_CMN_MAC_CFG_STATUS_MAC_FC_MAC_LOW_CFG(t) ( (t) >= VTSS_TO_DEV_10 ? VTSS_DEV_MAC_CFG_STATUS_MAC_FC_MAC_LOW_CFG(t) : VTSS_DEV_GMII_MAC_CFG_STATUS_MAC_FC_MAC_LOW_CFG(t) )


#endif /* _VTSS_LUTON26_REGS_DEV_CMN_H_ */
