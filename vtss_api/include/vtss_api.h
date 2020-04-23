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

/**
 * \file
 * \brief Vitesse API main header file
 * \details This is the only header file which must be included by the application
 */

#ifndef _VTSS_API_H_
#define _VTSS_API_H_

#include <vtss/api/options.h>

#include <vtss_os.h>

#include <vtss/api/types.h>
#include <vtss_init_api.h>

#ifdef VTSS_FEATURE_MISC
#include <vtss_misc_api.h>
#endif 

#ifdef VTSS_FEATURE_PORT_CONTROL
#include <vtss_port_api.h>
#endif

#ifdef VTSS_CHIP_CU_PHY
#include <vtss_phy_api.h>
#endif

#ifdef VTSS_CHIP_10G_PHY
#include <vtss_phy_10g_api.h>
#endif

#ifdef VTSS_FEATURE_QOS
#include <vtss_qos_api.h>
#endif

#ifdef VTSS_FEATURE_PACKET
#include <vtss_packet_api.h>
#endif 

#ifdef VTSS_FEATURE_AFI_SWC
#include <vtss_afi_api.h>
#endif 

#ifdef VTSS_FEATURE_LAYER2
#include <vtss_security_api.h>
#include <vtss_l2_api.h>
#endif 

#if defined(VTSS_FEATURE_LAYER3)
#include <vtss_l3_api.h>
#endif


#ifdef VTSS_FEATURE_OAM
#include <vtss_oam_api.h>
#endif 

#if defined (VTSS_FEATURE_EVC) || defined(VTSS_ARCH_SERVAL) || defined(VTSS_ARCH_LUTON26)
#include <vtss_evc_api.h>
#endif 

#ifdef VTSS_FEATURE_FDMA
#include <vtss_fdma_api.h>
#endif

#ifdef VTSS_FEATURE_MPLS
#include <vtss_mpls_api.h>
#endif

#ifdef VTSS_FEATURE_HQOS
#include <vtss_hqos_api.h>
#endif

#ifdef VTSS_FEATURE_SYNCE
#include <vtss_sync_api.h>
#endif 

#if defined(VTSS_FEATURE_PHY_TIMESTAMP) || defined(VTSS_FEATURE_TIMESTAMP)
#include <vtss_phy_ts_api.h>
#include <vtss_ts_api.h>
#endif

#ifdef VTSS_FEATURE_WIS
#include <vtss_wis_api.h>
#endif

#ifdef VTSS_FEATURE_AE
#include <vtss_ae_api.h>
#endif

#ifdef VTSS_FEATURE_ANEG
#include <vtss_aneg_api.h>
#endif 

#ifdef VTSS_FEATURE_GFP
#include <vtss_gfp_api.h>
#endif 

#ifdef VTSS_FEATURE_I2C
#include <vtss_i2c_api.h>
#endif 

#ifdef VTSS_FEATURE_MAC10G
#include <vtss_mac10g_api.h>
#endif 

#ifdef VTSS_FEATURE_OHA
#include <vtss_oha_api.h>
#endif 

#ifdef VTSS_FEATURE_OTN
#include <vtss_otn_api.h>
#endif

#ifdef VTSS_FEATURE_RAB
#include <vtss_rab_api.h>
#endif 

#ifdef VTSS_FEATURE_SFI4
#include <vtss_sfi4_api.h>
#endif 

#ifdef VTSS_FEATURE_TFI5
#include <vtss_tfi5_api.h>
#endif 

#ifdef VTSS_FEATURE_UPI
#include <vtss_upi_api.h>
#endif 

#ifdef VTSS_FEATURE_XAUI
#include <vtss_xaui_api.h>
#endif 

#ifdef VTSS_FEATURE_XFI
#include <vtss_xfi_api.h>
#endif 

#ifdef VTSS_FEATURE_PCS_10GBASE_R
#include <vtss_pcs_10gbase_r_api.h>
#endif

#ifdef VTSS_FEATURE_MACSEC
#include <vtss_macsec_api.h>
#endif

#endif /* _VTSS_API_H_ */
