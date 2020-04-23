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

// Avoid "vtss_api.h not used in module vtss_xfi_api.c"
/*lint --e{766} */

#include "vtss_api.h"
#if defined(VTSS_FEATURE_XFI)

#include "vtss_state.h"
#include "vtss_xfi_api.h"

/* ================================================================= *
 *  Defects/Events
 * ================================================================= */

vtss_rc vtss_xfi_event_enable(const vtss_inst_t         inst,
                              const vtss_port_no_t      port_no,
                              const BOOL                enable,
                              const vtss_xfi_event_t    ev_mask)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK ) {
        rc = VTSS_FUNC_COLD(cil.xfi_event_enable, port_no, enable, ev_mask);
    }
    VTSS_EXIT();
    return rc;
}


vtss_rc vtss_xfi_event_poll(const vtss_inst_t     inst,
                            const vtss_port_no_t  port_no,
                            vtss_xfi_event_t      *const status)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK ) {
        rc = VTSS_FUNC_COLD(cil.xfi_event_poll, port_no, status);
    }
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_xfi_event_poll_without_mask(const vtss_inst_t     inst,
                                         const vtss_port_no_t  port_no,
                                         vtss_xfi_event_t      *const status)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK ) {
        rc = VTSS_FUNC_COLD(cil.xfi_event_poll_without_mask, port_no, status);
    }
    VTSS_EXIT();
    return rc;
}


/* ================================================================= *
 *  Dynamic Config
 * ================================================================= */

vtss_rc vtss_xfi_conf_set(const vtss_inst_t inst,
                          const vtss_port_no_t port_no,
                          const vtss_xfi_cfg_t *const cfg)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK && cfg) {
        vtss_state->xfi_state[port_no].xfi_cfg = *cfg;
        rc = VTSS_FUNC_COLD(cil.xfi_config_set, port_no);
    }
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_xfi_conf_get(const vtss_inst_t inst,
                          const vtss_port_no_t port_no,
                          vtss_xfi_cfg_t *const cfg)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK && cfg) {
        *cfg = vtss_state->xfi_state[port_no].xfi_cfg;
    }
    VTSS_EXIT();
    return rc;
}

/* ================================================================= *
 *  State Reporting
 * ================================================================= */
vtss_rc vtss_xfi_status_get(const vtss_inst_t inst,
                            const vtss_port_no_t port_no,
                            vtss_xfi_status_t *const status)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK && status) {
        rc = VTSS_FUNC(cil.xfi_status_get, port_no, status);
    }
    VTSS_EXIT();
    return rc;
}

/* ================================================================= *
 *  Test/Debug Config
 * ================================================================= */

vtss_rc vtss_xfi_test_conf_set(const vtss_inst_t inst,
                               const vtss_port_no_t port_no,
                               const vtss_xfi_test_cfg_t *const cfg)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK && cfg) {
        vtss_state->xfi_state[port_no].xfi_test_cfg = *cfg;
        rc = VTSS_FUNC_COLD(cil.xfi_test_config_set, port_no);
    }
    VTSS_EXIT();
    return rc;
}


vtss_rc vtss_xfi_test_conf_get(const vtss_inst_t inst,
                               const vtss_port_no_t port_no,
                               vtss_xfi_test_cfg_t *const cfg)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK && cfg) {
        *cfg = vtss_state->xfi_state[port_no].xfi_test_cfg;
    }
    VTSS_EXIT();
    return rc;
}


vtss_rc vtss_xfi_test_status_get(const vtss_inst_t inst,
                                 const vtss_port_no_t port_no,
                                 vtss_xfi_test_status_t *const status)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK && status) {
        rc = VTSS_FUNC(cil.xfi_test_status_get, port_no, status);
    }
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_xfi_rec_clock_output_set(const vtss_inst_t inst,
                                      const vtss_port_no_t port_no,
                                      vtss_xfi_rec_clock_output_t const *xfi_rec_clock_output)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;
    BOOL lane_a,lane_b;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK && xfi_rec_clock_output) {
        lane_a = xfi_rec_clock_output->lane_a;
        lane_b = xfi_rec_clock_output->lane_b;
        if ( (lane_a != lane_b) || (lane_a == FALSE && lane_b == FALSE) ) {
        vtss_state->xfi_state[port_no].xfi_cfg.xfi_rec_clock_output = *xfi_rec_clock_output;  
        rc = VTSS_FUNC(cil.xfi_recovered_clock_output_set, port_no);
        } else {
            VTSS_E("lane_a and lane_b can't be enabled at a time");
            rc = VTSS_RC_ERROR;
        }
    }
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_xfi_rec_clock_output_get(const vtss_inst_t inst,
                                      const vtss_port_no_t port_no,
                                      vtss_xfi_rec_clock_output_t *const xfi_rec_clock_output)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK && xfi_rec_clock_output) {
        *xfi_rec_clock_output = vtss_state->xfi_state[port_no].xfi_cfg.xfi_rec_clock_output;  
    }
    VTSS_EXIT();
    return rc;
}


vtss_rc vtss_xfi_txeq_mode_set(const vtss_inst_t inst,
                               const vtss_port_no_t port_no,
                               vtss_xfi_txeq_cfg_t const *cfg)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK && cfg) {
        vtss_state->xfi_state[port_no].xfi_tx_cfg.txeq_cfg = *cfg;  
        rc = VTSS_FUNC(cil.xfi_txeq_mode_set, port_no);
    }
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_xfi_txeq_mode_get(const vtss_inst_t inst,
                               const vtss_port_no_t port_no,
                               vtss_xfi_txeq_cfg_t *const cfg)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK && cfg) {
        *cfg = vtss_state->xfi_state[port_no].xfi_tx_cfg.txeq_cfg;  
    }

    VTSS_EXIT();
    return rc;
}


vtss_rc vtss_xfi_txeq_coef_adjust(const vtss_inst_t inst,
                                  const vtss_port_no_t port_no,
                                  vtss_xfi_ffe_coef_t coef,
                                  u32 step_size,
                                  BOOL incr,
                                  BOOL polarity)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);

    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK) {
        if (((vtss_state->xfi_state[port_no].xfi_tx_cfg.txeq_cfg.mode) != XFI_TXEQ_TRADITIONAL_OVERRIDE) && 
            ((vtss_state->xfi_state[port_no].xfi_tx_cfg.txeq_cfg.mode) != XFI_TXEQ_TRADITIONAL)) {
                rc = VTSS_RC_ERROR;
        }
        if (((vtss_state->xfi_state[port_no].xfi_tx_cfg.txeq_cfg.mode) == XFI_TXEQ_TRADITIONAL_OVERRIDE)  && (incr)){
            if (((coef == txeq_ffe_coef_0) && (vtss_state->xfi_state[port_no].xfi_tx_cfg.coefs[0] + step_size) > 31) ||
                ((coef == txeq_ffe_coef_1) && (vtss_state->xfi_state[port_no].xfi_tx_cfg.coefs[1] + step_size) > 127) ||
                ((coef == txeq_ffe_coef_2) && (vtss_state->xfi_state[port_no].xfi_tx_cfg.coefs[2] + step_size) > 63)) {
                rc = VTSS_RC_ERROR;
            }
            if ((vtss_state->xfi_state[port_no].xfi_tx_cfg.coefs[0] +
                 vtss_state->xfi_state[port_no].xfi_tx_cfg.coefs[1] +
                 vtss_state->xfi_state[port_no].xfi_tx_cfg.coefs[2] + step_size) > 130) {
                rc = VTSS_RC_ERROR;
            }
        }
        if (((vtss_state->xfi_state[port_no].xfi_tx_cfg.txeq_cfg.mode) == XFI_TXEQ_TRADITIONAL)  && (incr)){
            if (((coef == txeq_ffe_coef_0) && (vtss_state->xfi_state[port_no].xfi_tx_cfg.coefs[0] + step_size) > 15) ||
                ((coef == txeq_ffe_coef_1) && (vtss_state->xfi_state[port_no].xfi_tx_cfg.coefs[1] + step_size) > 63) ||
                ((coef == txeq_ffe_coef_2) && (vtss_state->xfi_state[port_no].xfi_tx_cfg.coefs[2] + step_size) > 31)) {
                rc = VTSS_RC_ERROR;
            }
        }
        if(!incr) {
            if (((coef == txeq_ffe_coef_0) && (vtss_state->xfi_state[port_no].xfi_tx_cfg.coefs[0] < step_size)) ||
                ((coef == txeq_ffe_coef_1) && (vtss_state->xfi_state[port_no].xfi_tx_cfg.coefs[1] < step_size)) ||
                ((coef == txeq_ffe_coef_2) && (vtss_state->xfi_state[port_no].xfi_tx_cfg.coefs[2] < step_size))) {
                rc = VTSS_RC_ERROR;
            }
        }

        if (rc != VTSS_RC_ERROR) {
            if (incr) {
                if (coef == txeq_ffe_coef_0) {
                    vtss_state->xfi_state[port_no].xfi_tx_cfg.coefs[0] += step_size;
                    vtss_state->xfi_state[port_no].xfi_tx_cfg.polarity[0] = polarity;
                } else if (coef == txeq_ffe_coef_1) {
                    vtss_state->xfi_state[port_no].xfi_tx_cfg.coefs[1] += step_size;
                    vtss_state->xfi_state[port_no].xfi_tx_cfg.polarity[1] = polarity;
                } else if (coef == txeq_ffe_coef_2) {
                    vtss_state->xfi_state[port_no].xfi_tx_cfg.coefs[2] += step_size;
                    vtss_state->xfi_state[port_no].xfi_tx_cfg.polarity[2] = polarity;
                }
            } else {
               if (coef == txeq_ffe_coef_0) {
                    vtss_state->xfi_state[port_no].xfi_tx_cfg.coefs[0] -= step_size;
                    vtss_state->xfi_state[port_no].xfi_tx_cfg.polarity[0] = polarity;
                } else if (coef == txeq_ffe_coef_1) {
                    vtss_state->xfi_state[port_no].xfi_tx_cfg.coefs[1] -= step_size;
                    vtss_state->xfi_state[port_no].xfi_tx_cfg.polarity[1] = polarity;
                } else if (coef == txeq_ffe_coef_2) {
                    vtss_state->xfi_state[port_no].xfi_tx_cfg.coefs[2] -= step_size;
                    vtss_state->xfi_state[port_no].xfi_tx_cfg.polarity[2] = polarity;
                }
            }
        }
    }
    rc = VTSS_FUNC(cil.xfi_txeq_coef_adjust, port_no, coef, polarity);
    VTSS_EXIT();

    return rc;
}

vtss_rc vtss_xfi_txeq_coef_get(const vtss_inst_t inst,
                               const vtss_port_no_t port_no,
                               u32 coefs[], BOOL polarity[])
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK) {
         coefs[0] = vtss_state->xfi_state[port_no].xfi_tx_cfg.coefs[0];
         coefs[1] = vtss_state->xfi_state[port_no].xfi_tx_cfg.coefs[1];
         coefs[2] = vtss_state->xfi_state[port_no].xfi_tx_cfg.coefs[2];
         polarity[0] = vtss_state->xfi_state[port_no].xfi_tx_cfg.polarity[0];
         polarity[1] = vtss_state->xfi_state[port_no].xfi_tx_cfg.polarity[1];
         polarity[2] = vtss_state->xfi_state[port_no].xfi_tx_cfg.polarity[2];
    }
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_xfi_txamp_set(const vtss_inst_t inst,
                           const vtss_port_no_t port_no,
                           u32 power_mVppd)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);

    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK) {
       if ((vtss_state->xfi_state[port_no].xfi_tx_cfg.txeq_cfg.mode == XFI_TXEQ_TRADITIONAL_OVERRIDE) ||
           (vtss_state->xfi_state[port_no].xfi_tx_cfg.txeq_cfg.mode == XFI_TXEQ_TRADITIONAL)) {
            if ((power_mVppd < 127) && (power_mVppd > 32)) {
                vtss_state->xfi_state[port_no].xfi_tx_cfg.transmit_power = power_mVppd;
                rc = VTSS_FUNC(cil.xfi_txamp_set, port_no);
            }
            else { 
                rc = VTSS_RC_ERROR;
            }
        }
    }
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_xfi_txamp_get(const vtss_inst_t inst,
                           const vtss_port_no_t port_no,
                           u32 *power_mVppd)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK && power_mVppd) {
        *power_mVppd = vtss_state->xfi_state[port_no].xfi_tx_cfg.transmit_power;
    }
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_xfi_txslew_set(const vtss_inst_t inst,
                           const vtss_port_no_t port_no,
                           u32 slew_rate)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);

    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK) {
        if ((slew_rate == 0) || (slew_rate == 3) || (slew_rate == 5) || (slew_rate == 7)){
            vtss_state->xfi_state[port_no].xfi_tx_cfg.slew_rate = slew_rate;
            rc = VTSS_FUNC(cil.xfi_txslew_set, port_no);
        } else {
            rc = VTSS_RC_ERROR;
        }
    }
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_xfi_txslew_get(const vtss_inst_t inst,
                           const vtss_port_no_t port_no,
                           u32 *slew_rate)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK && slew_rate) {
        *slew_rate = vtss_state->xfi_state[port_no].xfi_tx_cfg.slew_rate;
    }
    VTSS_EXIT();
    return rc;
}


vtss_rc vtss_xfi_rxeq_mode_set(const vtss_inst_t inst,
                               const vtss_port_no_t port_no,
                               vtss_xfi_rxeq_cfg_t const *cfg)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);

    VTSS_ENTER();

    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK && cfg) {
        vtss_state->xfi_state[port_no].xfi_rxeq_cfg = *cfg;  
        rc = VTSS_FUNC(cil.xfi_rxeq_mode_set, port_no);
    }

    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_xfi_rxeq_mode_get(const vtss_inst_t inst,
                               const vtss_port_no_t port_no,
                               vtss_xfi_rxeq_cfg_t *const cfg)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);

    VTSS_ENTER();

    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK && cfg) {
       *cfg = vtss_state->xfi_state[port_no].xfi_rxeq_cfg;
    }

    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_xfi_txeq_8023ap_coef_update(const vtss_inst_t inst,
                                         const vtss_port_no_t port_no,
                                         vtss_xfi_lane_t lane,
                                         vtss_xfi_ffe_coef_t coef,
                                         vtss_txeq_8023ap_updreq_t updreq)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);

    VTSS_ENTER();

    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK) {
        rc = VTSS_FUNC(cil.xfi_txeq_8023ap_coef_update, port_no, lane, coef, updreq);
    }

    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_xfi_txeq_8023ap_coef_stat_get(const vtss_inst_t inst,
                                     const vtss_port_no_t port_no, 
                                     vtss_xfi_lane_t lane, vtss_txeq_8023ap_coef_stat_t *ae_status)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);

    VTSS_ENTER();

    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK) {
        rc = VTSS_FUNC(cil.xfi_txeq_8023ap_coef_stat_get, port_no, lane, ae_status);
    }

    VTSS_EXIT();
    return rc;
}


vtss_rc vtss_xfi_txeq_8023ap_fsm_ctl_set(const vtss_inst_t inst,
                                     const vtss_port_no_t port_no, 
                                     vtss_xfi_lane_t lane, u32 val)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);

    VTSS_ENTER();

    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK) {
        rc = VTSS_FUNC(cil.xfi_txeq_8023ap_fsm_ctl_set, port_no, lane, val);
    }

    VTSS_EXIT();
    return rc;
}


vtss_rc vtss_xfi_txeq_8023ap_fsm_stat_get(const vtss_inst_t inst,
                                     const vtss_port_no_t port_no, 
                                     vtss_xfi_lane_t lane, u32 *val)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);

    VTSS_ENTER();

    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK) {
        rc = VTSS_FUNC(cil.xfi_txeq_8023ap_fsm_stat_get, port_no, lane, val);
    }

    VTSS_EXIT();
    return rc;
}

/* ================================================================= *
 *  Performance Primitives
 * ================================================================= */

#endif /*VTSS_FEATURE_xfi */
