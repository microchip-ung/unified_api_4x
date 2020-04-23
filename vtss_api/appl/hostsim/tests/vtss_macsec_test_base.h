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

#ifndef __VTSS_MACSEC_TEST_BASE_H__
#define __VTSS_MACSEC_TEST_BASE_H__

#include "vtss_api.h"
#include "vtss_macsec_api.h"
#include "vtss_macsec_test_base.h"
#include "vtss_macsec_emu_base.h"

void vtss_basic_secy(vtss_inst_t         inst,
                     vtss_port_no_t      macsec_physical_port,
                     vtss_macsec_port_t *_macsec_port,
                     vtss_mac_t         *_port_macaddress
);

void vtss_basic_secy_destroy(
    vtss_inst_t               inst,
    const vtss_macsec_port_t *macsec_port
);

void vtss_single_secy_sample_system_create(
    vtss_inst_t         inst,
    vtss_macsec_port_t *_macsec_port,
    vtss_mac_t         *_port_macaddress,
    vtss_macsec_sci_t  *_peer_sci
);

void vtss_single_secy_sample_system_destroy(
    vtss_inst_t               inst,
    const vtss_macsec_port_t *macsec_port,
    const vtss_mac_t         *port_macaddress,
    const vtss_macsec_sci_t  *peer_sci
);

vtss_rc sak_update_hash_key(vtss_macsec_sak_t * sak);

#endif
