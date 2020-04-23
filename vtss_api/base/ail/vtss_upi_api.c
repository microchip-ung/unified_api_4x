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

// Avoid "vtss_api.h not used in module vtss_upi_api.c"
/*lint --e{766} */

#include "vtss_api.h"
#if defined(VTSS_FEATURE_UPI)

#include "vtss_state.h"
#include "vtss_upi_api.h"

/* ================================================================= *
 *  Defects/Events
 * ================================================================= */

vtss_rc vtss_upi_event_enable(const vtss_inst_t         inst,
                              const vtss_port_no_t      port_no,
                              const BOOL                enable,
                              const vtss_upi_event_t    ev_mask)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK ) {
        rc = VTSS_FUNC_COLD(cil.upi_event_enable, port_no, enable, ev_mask);
    }
    VTSS_EXIT();
    return rc;
}


vtss_rc vtss_upi_event_poll(const vtss_inst_t     inst,
                            const vtss_port_no_t  port_no,
                            vtss_upi_event_t      *const status)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK ) {
        rc = VTSS_FUNC_COLD(cil.upi_event_poll, port_no, status);
    }
    VTSS_EXIT();
    return rc;
}


vtss_rc vtss_upi_event_poll_without_mask(const vtss_inst_t     inst,
                                         const vtss_port_no_t  port_no,
                                         vtss_upi_event_t      *const status)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK ) {
        rc = VTSS_FUNC_COLD(cil.upi_event_poll_without_mask, port_no, status);
    }
    VTSS_EXIT();
    return rc;
}


/* ================================================================= *
 *  Dynamic Config
 * ================================================================= */

vtss_rc vtss_upi_conf_set(const vtss_inst_t inst,
                          const vtss_port_no_t port_no,
                          const vtss_upi_cfg_t *const cfg)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK && cfg) {
        vtss_state->upi_state[port_no].upi_cfg = *cfg;
        rc = VTSS_FUNC_COLD(cil.upi_config_set, port_no);
    }
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_upi_conf_get(const vtss_inst_t inst,
                          const vtss_port_no_t port_no,
                          vtss_upi_cfg_t *const cfg)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK && cfg) {
        *cfg = vtss_state->upi_state[port_no].upi_cfg;
    }
    VTSS_EXIT();
    return rc;
}


/* ================================================================= *
 *  State Reporting
 * ================================================================= */


vtss_rc vtss_upi_status_get(const vtss_inst_t inst,
                            const vtss_port_no_t port_no,
                            vtss_upi_status_t *const status)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK && status) {
        rc = VTSS_FUNC(cil.upi_status_get, port_no, status);
    }
    VTSS_EXIT();
    return rc;
}

/* ================================================================= *
 *  Test/Debug Config
 * ================================================================= */

vtss_rc vtss_upi_test_conf_set(const vtss_inst_t inst,
                               const vtss_port_no_t port_no,
                               const vtss_upi_test_cfg_t *const cfg)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK && cfg) {
        vtss_state->upi_state[port_no].upi_test_cfg = *cfg;
        rc = VTSS_FUNC_COLD(cil.upi_test_config_set, port_no);
    }
    VTSS_EXIT();
    return rc;
}


vtss_rc vtss_upi_test_conf_get(const vtss_inst_t inst,
                               const vtss_port_no_t port_no,
                               vtss_upi_test_cfg_t *const cfg)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK && cfg) {
        *cfg = vtss_state->upi_state[port_no].upi_test_cfg;
    }
    VTSS_EXIT();
    return rc;
}


vtss_rc vtss_upi_test_status_get(const vtss_inst_t inst,
                                 const vtss_port_no_t port_no,
                                 vtss_upi_test_status_t *const status)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK && status) {
        rc = VTSS_FUNC(cil.upi_test_status_get, port_no, status);
    }
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_upi_txeq_mode_set(const vtss_inst_t inst,
                               const vtss_port_no_t port_no,
                               vtss_upi_txeq_cfg_t const *cfg)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if (((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK)
               && cfg) {
        ((vtss_state->upi_state[port_no]).upi_tx_cfg).txeq_cfg = *cfg;  
        rc = VTSS_FUNC(cil.upi_txeq_mode_set, port_no);
    }
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_upi_txeq_mode_get(const vtss_inst_t inst,
                               const vtss_port_no_t port_no,
                               vtss_upi_txeq_cfg_t *const cfg)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;
    
    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    rc = vtss_inst_port_no_check(inst, &vtss_state, port_no);
    if (rc == VTSS_RC_OK && cfg) {
        *cfg = vtss_state->upi_state[port_no].upi_tx_cfg.txeq_cfg;
    }
    VTSS_EXIT();

    return rc;
}

vtss_rc vtss_upi_txeq_coef_adjust(const vtss_inst_t inst,
                                  const vtss_port_no_t port_no,
                                  vtss_upi_ffe_coef_t coef,
                                  u32 step_size,
                                  BOOL incr,
                                  BOOL polarity)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
   
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK) {
        if ((vtss_state->upi_state[port_no].upi_tx_cfg.txeq_cfg.mode == UPI_TXEQ_OVERRIDE) && (incr)) {
			if (((coef == upi_txeq_coef_0) && (((vtss_state->upi_state[port_no]).upi_tx_cfg.coefs[0]) + step_size) > 63) ||
				((coef == upi_txeq_coef_1) && ((vtss_state->upi_state[port_no].upi_tx_cfg.coefs[1]) + step_size) > 255) ||
				((coef == upi_txeq_coef_2) && ((vtss_state->upi_state[port_no].upi_tx_cfg.coefs[2]) + step_size) > 127) ||
				((coef == upi_txeq_coef_3) && ((vtss_state->upi_state[port_no].upi_tx_cfg.coefs[3]) + step_size) > 63)) {
				rc = VTSS_RC_ERROR;
			}
			if ((vtss_state->upi_state[port_no].upi_tx_cfg.coefs[0] +
				vtss_state->upi_state[port_no].upi_tx_cfg.coefs[1] +
				vtss_state->upi_state[port_no].upi_tx_cfg.coefs[2] +
				vtss_state->upi_state[port_no].upi_tx_cfg.coefs[3] + 
				step_size) > 264) {
				rc = VTSS_RC_ERROR;
			}
		}
		else if (((vtss_state->upi_state[port_no].upi_tx_cfg.txeq_cfg.mode) == UPI_TXEQ_NORMAL) && (incr)) {
			if (((coef == upi_txeq_coef_0) && ((vtss_state->upi_state[port_no].upi_tx_cfg.coefs[0]) + step_size) > 15) ||
				((coef == upi_txeq_coef_1) && ((vtss_state->upi_state[port_no].upi_tx_cfg.coefs[1]) + step_size) > 63) ||
				((coef == upi_txeq_coef_2) && ((vtss_state->upi_state[port_no].upi_tx_cfg.coefs[2]) + step_size) > 31) ||
				((coef == upi_txeq_coef_3) && ((vtss_state->upi_state[port_no].upi_tx_cfg.coefs[3]) + step_size) > 15)) {
				rc = VTSS_RC_ERROR;
			}
		}

        if ((rc != VTSS_RC_ERROR) && !incr) {
            if ((coef == upi_txeq_coef_0 && (vtss_state->upi_state[port_no].upi_tx_cfg.coefs[0] < step_size)) ||
                (coef == upi_txeq_coef_1 && (vtss_state->upi_state[port_no].upi_tx_cfg.coefs[1] < step_size)) ||
                (coef == upi_txeq_coef_2 && (vtss_state->upi_state[port_no].upi_tx_cfg.coefs[2] < step_size)) ||
                (coef == upi_txeq_coef_3 && (vtss_state->upi_state[port_no].upi_tx_cfg.coefs[3] < step_size))) { 
                rc = VTSS_RC_ERROR;
            }
        }
                              
        if (rc != VTSS_RC_ERROR) {
            if (incr) {
                if (coef == upi_txeq_coef_0) {
                    vtss_state->upi_state[port_no].upi_tx_cfg.coefs[0] += step_size;
                    vtss_state->upi_state[port_no].upi_tx_cfg.polarity[0] = polarity;
                } else if (coef == upi_txeq_coef_1) {
                    vtss_state->upi_state[port_no].upi_tx_cfg.coefs[1] += step_size;
                    vtss_state->upi_state[port_no].upi_tx_cfg.polarity[1] = polarity;
                } else if (coef == upi_txeq_coef_2) {
                    vtss_state->upi_state[port_no].upi_tx_cfg.coefs[2] += step_size;
                    vtss_state->upi_state[port_no].upi_tx_cfg.polarity[2] = polarity;
                } else if (coef == upi_txeq_coef_3) {
                    vtss_state->upi_state[port_no].upi_tx_cfg.coefs[3] += step_size;
                    vtss_state->upi_state[port_no].upi_tx_cfg.polarity[3] = polarity;
                }
            } else {
                if (coef == upi_txeq_coef_0) {
                    vtss_state->upi_state[port_no].upi_tx_cfg.coefs[0] -= step_size;
                    vtss_state->upi_state[port_no].upi_tx_cfg.polarity[0] = polarity;
                } else if (coef == upi_txeq_coef_1) {
                    vtss_state->upi_state[port_no].upi_tx_cfg.coefs[1] -= step_size;
                    vtss_state->upi_state[port_no].upi_tx_cfg.polarity[1] = polarity;
                } else if (coef == upi_txeq_coef_2) {
                    vtss_state->upi_state[port_no].upi_tx_cfg.coefs[2] -= step_size;
                    vtss_state->upi_state[port_no].upi_tx_cfg.polarity[2] = polarity;
                } else if (coef == upi_txeq_coef_3) {
                    vtss_state->upi_state[port_no].upi_tx_cfg.coefs[3] -= step_size;
                    vtss_state->upi_state[port_no].upi_tx_cfg.polarity[3] = polarity;
                }
            }
        }

        rc = VTSS_FUNC(cil.upi_txeq_coef_adjust, port_no, coef, polarity);
    }
    VTSS_EXIT();

    return rc;
}

vtss_rc vtss_upi_txeq_coef_get(const vtss_inst_t inst,
                               const vtss_port_no_t port_no,
                               u32 coefs[], BOOL polarity[])
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK) {
        coefs[0] = vtss_state->upi_state[port_no].upi_tx_cfg.coefs[0];
        coefs[1] = vtss_state->upi_state[port_no].upi_tx_cfg.coefs[1];
        coefs[2] = vtss_state->upi_state[port_no].upi_tx_cfg.coefs[2];
        coefs[3] = vtss_state->upi_state[port_no].upi_tx_cfg.coefs[3];
        polarity[0] = vtss_state->upi_state[port_no].upi_tx_cfg.polarity[0];
        polarity[1] = vtss_state->upi_state[port_no].upi_tx_cfg.polarity[1];
        polarity[2] = vtss_state->upi_state[port_no].upi_tx_cfg.polarity[2];
        polarity[3] = vtss_state->upi_state[port_no].upi_tx_cfg.polarity[3];
    }
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_upi_txamp_set(const vtss_inst_t inst,
                           const vtss_port_no_t port_no,
                           u32 power_mvppd)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK){
        if ((power_mvppd < 127) && (power_mvppd > 20)) {
            vtss_state->upi_state[port_no].upi_tx_cfg.transmit_power = 
															power_mvppd;
            rc = VTSS_FUNC(cil.upi_txamp_set, port_no);
    	}
        else {
            rc = VTSS_RC_ERROR;
        }
    }
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_upi_txamp_get(const vtss_inst_t inst,
                           const vtss_port_no_t port_no,
                           u32 *power_mvppd)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK && power_mvppd) {
        *power_mvppd = vtss_state->upi_state[port_no].upi_tx_cfg.transmit_power;
    }
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_upi_txslew_set(const vtss_inst_t inst,
                            const vtss_port_no_t port_no,
                            u32 slew_rate)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK) {
        vtss_state->upi_state[port_no].upi_tx_cfg.slew_rate = slew_rate;
        rc = VTSS_FUNC(cil.upi_txslew_set, port_no);
    } else {
        rc = VTSS_RC_ERROR;
    }

    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_upi_txslew_get(const vtss_inst_t inst,
                            const vtss_port_no_t port_no,
                            u32 *slew_rate)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK && slew_rate) {
        *slew_rate = vtss_state->upi_state[port_no].upi_tx_cfg.slew_rate;
    }
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_upi_rxeq_mode_set(const vtss_inst_t inst,
                               const vtss_port_no_t port_no,
                               vtss_upi_rxeq_cfg_t const *cfg)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK 
         && cfg != NULL) {
        vtss_state->upi_state[port_no].upi_rxeq_cfg = *cfg;
        rc = VTSS_FUNC(cil.upi_rxeq_mode_set, port_no);
    }
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_upi_rxeq_mode_get(const vtss_inst_t inst,
                               const vtss_port_no_t port_no,
                               vtss_upi_rxeq_cfg_t *const cfg)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK 
         && cfg != NULL) {
        *cfg = vtss_state->upi_state[port_no].upi_rxeq_cfg;
    }
    VTSS_EXIT();
    return rc;
}
/* ================================================================= *
 *  Performance Primitives
 * ================================================================= */



#endif /*VTSS_FEATURE_UPI */
