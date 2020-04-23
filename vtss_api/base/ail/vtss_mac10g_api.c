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

/**
 * \file vtss_mac10g_api.c
 * \brief  MAC10G  API
 */

#ifndef _VTSS_MAC10G_API_
#define _VTSS_MAC10G_API_

#include "vtss_api.h"
#include "vtss_state.h"
#include "vtss_common.h"

#if defined(VTSS_FEATURE_MAC10G)
#include <vtss/api/types.h>
#include <vtss_port_api.h>
#include <vtss_mac10g_api.h>

/* ================================================================= *
 *  Defects/Events
 * ================================================================= */

vtss_rc vtss_mac10g_event_enable(const vtss_inst_t           inst,
                                 const vtss_port_no_t        port_no,
                                 const BOOL                  enable,
                                 const vtss_mac10g_event_t   ev_mask)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK ) {
        rc = VTSS_FUNC_COLD(cil.mac10g_event_enable, port_no, enable, ev_mask);
    }
    VTSS_EXIT();
    return rc;
}


vtss_rc vtss_mac10g_event_poll(const vtss_inst_t       inst,
                               const vtss_port_no_t    port_no,
                               vtss_mac10g_event_t     *const status)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK ) {
        rc = VTSS_FUNC_COLD(cil.mac10g_event_poll, port_no, status);
    }
    VTSS_EXIT();
    return rc;
}

#if defined(VTSS_ARCH_DAYTONA)
vtss_rc vtss_mac10g_event_poll_without_mask(const vtss_inst_t       inst,
                                            const vtss_port_no_t    port_no,
                                            vtss_mac10g_event_t     *const status)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK ) {
        rc = VTSS_FUNC_COLD(cil.mac10g_event_poll_without_mask, port_no, status);
    }
    VTSS_EXIT();
    return rc;
}
#endif /* VTSS_ARCH_DAYTONA */

/* ================================================================= *
 *  Dynamic Config
 * ================================================================= */
vtss_rc vtss_mac10g_config_set(const vtss_inst_t inst,
                               const vtss_port_no_t port_no,
                               const vtss_mac10g_cfg_t *const cfg)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK && cfg) {
        vtss_state->mac10g_state[port_no].mac10g_cfg = *cfg;
        rc = VTSS_FUNC_COLD(cil.mac10g_config_set, port_no);
    }
    VTSS_EXIT();
    return rc;    
}

vtss_rc vtss_mac10g_config_get(const vtss_inst_t inst,
                               const vtss_port_no_t port_no,
                               vtss_mac10g_cfg_t * const cfg)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK && cfg) {
        *cfg = vtss_state->mac10g_state[port_no].mac10g_cfg;
    }
    VTSS_EXIT();
    return rc;
}

/* ================================================================= *
 *  State Reporting
 * ================================================================= */
vtss_rc vtss_mac10g_status_get(const vtss_inst_t inst,
                               const vtss_port_no_t port_no,
                               vtss_mac10g_status_t *const status)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK && status) {
        rc = VTSS_FUNC_COLD(cil.mac10g_status_get, port_no, status);
    }
    VTSS_EXIT();
    return rc;
}

/* ================================================================= *
 *  Performance Primitives
 * ================================================================= */
vtss_rc vtss_mac10g_counters_update(const vtss_inst_t inst,
                                    const vtss_port_no_t port_no)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK) {
        rc = VTSS_FUNC_COLD(cil.mac10g_counters_update, port_no);
    }
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_mac10g_counters_clear(const vtss_inst_t inst,
                                   const vtss_port_no_t port_no)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK) {
        rc = VTSS_FUNC_COLD(cil.mac10g_counters_clear, port_no);
    }
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_mac10g_counters_get(const vtss_inst_t inst,
                                 const vtss_port_no_t port_no,
                                 vtss_port_counters_t *const counters)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK && counters) {
        rc = VTSS_FUNC_COLD(cil.mac10g_counters_get, port_no, counters);
    }
    VTSS_EXIT();
    return rc;
}

#endif /* VTSS_FEATURE_MAC10G */
#endif /* _VTSS_MAC10G_API_ */
