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

// Avoid "vtss_api.h not used in module vtss_aneg_api.c"
/*lint --e{766} */
#include "vtss_api.h"
#if defined(VTSS_FEATURE_ANEG)

#include "vtss_state.h"
#include "vtss_aneg_api.h"

/* ================================================================= *
 *  Defects/Events
 * ================================================================= */

/* ================================================================= *
 *  Dynamic Config
 * ================================================================= */

vtss_rc vtss_aneg_set_config(vtss_inst_t inst,
                          vtss_port_no_t port_no,
                          const vtss_aneg_cfg_t *const cfg)
{
    vtss_rc rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, port_no)) == VTSS_RC_OK && cfg) {
        vtss_state->aneg_state[port_no].aneg_cfg = *cfg;
        rc = VTSS_FUNC_COLD(aneg_set_config, port_no);
    }
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_aneg_get_config(vtss_inst_t inst,
                                   vtss_port_no_t port_no,
                                   vtss_aneg_cfg_t *const cfg)
{
	vtss_rc rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, port_no)) == VTSS_RC_OK && cfg) {
        *cfg = vtss_state->aneg_state[port_no].aneg_cfg;        
    }
    VTSS_EXIT();
    return rc;
}



vtss_rc vtss_aneg_set_ability(vtss_inst_t inst,
                                       vtss_port_no_t port_no,
                                       const vtss_aneg_ability_t *const ability)
{
	vtss_rc rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, port_no)) == VTSS_RC_OK && ability) {
        vtss_state->aneg_state[port_no].aneg_ability = *ability;
        rc = VTSS_FUNC_COLD(aneg_set_ability, port_no);
    }
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_aneg_get_ability(vtss_inst_t inst,
                              vtss_port_no_t port_no,
                              vtss_aneg_ability_t *const ability)
{
	vtss_rc rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, port_no)) == VTSS_RC_OK && ability) {
        *ability=vtss_state->aneg_state[port_no].aneg_ability;
        rc = VTSS_FUNC_COLD(aneg_set_ability, port_no);
    }
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_aneg_get_next_page(vtss_inst_t inst,
                              vtss_port_no_t port_no,
                              vtss_aneg_next_page_t *const next_page)
{
	return VTSS_RC_OK;
}

vtss_rc vtss_aneg_set_next_page(vtss_inst_t inst,
                              vtss_port_no_t port_no,
                              const vtss_aneg_next_page_t *const next_page)
{
	return VTSS_RC_OK;
}

vtss_rc vtss_aneg_set_enable(const vtss_inst_t inst,
                          vtss_port_no_t port_no,
                          BOOL enable
                          )
{
    vtss_rc rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, port_no)) == VTSS_RC_OK) {
        vtss_state->aneg_state[port_no].aneg_cfg.aneg_enable = enable;
        rc = VTSS_FUNC_COLD(aneg_set_enable, port_no);
    }
    VTSS_EXIT();
    return rc;	
}


vtss_rc vtss_aneg_reset(const vtss_inst_t inst,
                          vtss_port_no_t port_no
                          )
{
	vtss_rc rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, port_no)) == VTSS_RC_OK) {        
        rc = VTSS_FUNC_COLD(aneg_reset, port_no);
    }
    VTSS_EXIT();
    return rc;
}



vtss_rc vtss_aneg_restart(const vtss_inst_t inst,
                          vtss_port_no_t port_no
                          )
{
	vtss_rc rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, port_no)) == VTSS_RC_OK) {        
        rc = VTSS_FUNC_COLD(aneg_restart, port_no);
    }
    VTSS_EXIT();
    return rc;
}


/* ================================================================= *
 *  State Reporting
 * ================================================================= */

vtss_rc vtss_aneg_events_get(vtss_inst_t inst,
                          vtss_port_no_t port_no,
                          vtss_aneg_events_t *const events)
{
	return VTSS_RC_OK;
}

vtss_rc vtss_aneg_get_status(vtss_inst_t inst,
                          vtss_port_no_t port_no,
                          vtss_aneg_status_t *const status)
{
	return VTSS_RC_OK;
}

vtss_rc vtss_aneg_get_lp_ability(vtss_inst_t inst,
                          vtss_port_no_t port_no,
                          vtss_aneg_ability_t *const lp_ability)
{
	return VTSS_RC_OK;
}

/* ================================================================= *
 *  Performance Primitives
 * ================================================================= */



#endif /*VTSS_FEATURE_ANEG */
