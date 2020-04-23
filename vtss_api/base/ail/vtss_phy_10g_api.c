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

// Avoid "vtss_api.h not used in module vtss_10g_phy_api.c"
/*lint --e{766} */
#include "vtss_api.h"
#if defined(VTSS_CHIP_10G_PHY)
#include "vtss_state.h"


vtss_rc vtss_phy_10g_csr_read(const vtss_inst_t           inst,
                              const vtss_port_no_t        port_no,
                              const u32                   dev,
                              const u32                   addr,
                              u32                         *const value)
{
    vtss_state_t *vtss_state;
    vtss_rc rc = VTSS_RC_ERROR;
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK) {
        if (vtss_state->phy_10g_state[port_no].family == VTSS_PHY_FAMILY_MALIBU) {
            rc = VTSS_FUNC_COLD(cil.malibu_phy_10g_csr_read, port_no, dev, addr, value);
        } else if (vtss_state->phy_10g_state[port_no].family != VTSS_PHY_FAMILY_10G_NONE) {
            rc = VTSS_FUNC_COLD(cil.phy_10g_csr_read, port_no, dev, addr, value);
        }
    }
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_phy_10g_csr_write(const vtss_inst_t           inst,
                               const vtss_port_no_t        port_no,
                               const u32                   dev,
                               const u32                   addr,
                               const u32                   value)
{
    vtss_state_t *vtss_state;
    vtss_rc rc = VTSS_RC_ERROR;
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK) {
        if (vtss_state->phy_10g_state[port_no].family == VTSS_PHY_FAMILY_MALIBU) {
            rc = VTSS_FUNC_COLD(cil.malibu_phy_10g_csr_write, port_no, dev, addr, value);
        } else if (vtss_state->phy_10g_state[port_no].family != VTSS_PHY_FAMILY_10G_NONE) {
            rc = VTSS_FUNC_COLD(cil.phy_10g_csr_write, port_no, dev, addr, value);
        }
    }
    VTSS_EXIT();
    return rc;
}


#endif /* VTSS_CHIP_10G_PHY */
