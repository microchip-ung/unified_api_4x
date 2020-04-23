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

// Avoid "vtss_api.h not used in module vtss_sf4_api.c"
/*lint --e{766} */

#include "vtss_api.h"
#if defined(VTSS_FEATURE_SFI4)

#include "vtss_state.h"
#include "vtss_sfi4_api.h"

/* ================================================================= *
 *  Defects/Events
 * ================================================================= */

/* ================================================================= *
 *  Dynamic Config
 * ================================================================= */

vtss_rc vtss_sfi4_set_config(vtss_inst_t inst,
                          vtss_port_no_t port_no,
                          const vtss_sfi4_cfg_t *const cfg)
{
    vtss_rc rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, port_no)) == VTSS_RC_OK && cfg) {
        vtss_state->sfi4_state[port_no].sfi4_cfg = *cfg;
        rc = VTSS_FUNC_COLD(sfi4_set_config, port_no);
    }
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_sfi4_get_config(vtss_inst_t inst,
                                   vtss_port_no_t port_no,
                                   vtss_sfi4_cfg_t *const cfg)
{
	vtss_rc rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, port_no)) == VTSS_RC_OK && cfg) {
        *cfg = vtss_state->sfi4_state[port_no].sfi4_cfg;
    }
    VTSS_EXIT();
    return rc;
}


vtss_rc vtss_sfi4_set_enable(const vtss_inst_t inst,
                          vtss_port_no_t port_no,
                          BOOL enable
                          )
{
    vtss_rc rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, port_no)) == VTSS_RC_OK) {
        vtss_state->sfi4_state[port_no].sfi4_cfg.enable = enable;
        rc = VTSS_FUNC_COLD(sfi4_set_enable, port_no);
    }
    VTSS_EXIT();
    return rc;	
}

vtss_rc vtss_sfi4_set_loopback(const vtss_inst_t inst,
                          vtss_port_no_t port_no,
                          BOOL enable,
                          vtss_sfi4_loopback_t dir
                          )
{
    vtss_rc rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, port_no)) == VTSS_RC_OK) {  

        (dir == sfi4_rx_to_tx) ? (vtss_state->sfi4_state[port_no].sfi4_cfg.rx_to_tx_loopback = enable):0;
        (dir == sfi4_tx_to_rx) ? (vtss_state->sfi4_state[port_no].sfi4_cfg.tx_to_rx_loopback = enable):0;

        rc = VTSS_FUNC_COLD(sfi4_set_loopback, port_no);
    }
    VTSS_EXIT();
    return rc;	
}





/* ================================================================= *
 *  State Reporting
 * ================================================================= */

vtss_rc vtss_sfi4_events_get(vtss_inst_t inst,
                          vtss_port_no_t port_no,
                          vtss_sfi4_events_t *const events)
{
	return VTSS_RC_OK;
}

vtss_rc vtss_sfi4_get_status(vtss_inst_t inst,
                          vtss_port_no_t port_no,
                          vtss_sfi4_status_t *const status)
{
	return VTSS_RC_OK;
}


/* ================================================================= *
 *  Performance Primitives
 * ================================================================= */



#endif /*VTSS_FEATURE_SFI4 */