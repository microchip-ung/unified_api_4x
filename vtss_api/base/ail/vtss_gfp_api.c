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

// Avoid "vtss_api.h not used in module vtss_gfp_api.c"
/*lint --e{766} */

#include "vtss_api.h"
#if defined(VTSS_FEATURE_GFP)

#include "vtss_state.h"
#include "vtss_gfp_api.h"

/* ================================================================= *
 *  Dynamic Config
 * ================================================================= */

vtss_rc vtss_gfp_single_err_corr_set(const vtss_inst_t           inst,
                                     const vtss_port_no_t        port_no,
                                     const vtss_gfp_err_corr_t   *const cfg)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK && cfg) {
        rc = VTSS_RC_ERROR;
        vtss_state->gfp_state[port_no].err_corr = *cfg;
        rc = VTSS_FUNC_COLD(cil.gfp_single_err_corr_set, port_no);
    }
    VTSS_EXIT();
    return rc;
}


vtss_rc vtss_gfp_single_err_corr_get(const vtss_inst_t        inst,
                                     const vtss_port_no_t     port_no,
                                     vtss_gfp_err_corr_t      *const cfg)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK && cfg) {
        *cfg = vtss_state->gfp_state[port_no].err_corr;
    }
    VTSS_EXIT();
    return rc;
}


vtss_rc vtss_gfp_cmf_auto_sf_gen_set(const vtss_inst_t              inst,
                                     const vtss_port_no_t           port_no,
                                     const vtss_gfp_cmf_auto_sf_t   *const cfg)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK && cfg) {
        rc = VTSS_RC_ERROR;
        if (((u16)cfg->los_sf < (u16)VTSS_GFP_CMF_SF_MAX) && ((u16)cfg->lsync_sf < (u16)VTSS_GFP_CMF_SF_MAX)) {
            vtss_state->gfp_state[port_no].cmf_auto_sf = *cfg;
            rc = VTSS_FUNC_COLD(cil.gfp_cmf_auto_sf_gen_set, port_no);
        }
    }
    VTSS_EXIT();
    return rc;
}


vtss_rc vtss_gfp_cmf_auto_sf_gen_get(const vtss_inst_t         inst,
                                     const vtss_port_no_t      port_no,
                                     vtss_gfp_cmf_auto_sf_t    *const cfg)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK && cfg) {
        *cfg = vtss_state->gfp_state[port_no].cmf_auto_sf;
    }
    VTSS_EXIT();
    return rc;
}


vtss_rc vtss_gfp_cmf_forced_gen_set(const vtss_inst_t              inst,
                                    const vtss_port_no_t           port_no,
                                    const vtss_gfp_cmf_forced_t    *const cfg)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK && cfg) {
        rc = VTSS_RC_ERROR;
        if (cfg->frame_len <= 65527) {
            vtss_state->gfp_state[port_no].cmf_forced = *cfg;
            rc = VTSS_FUNC_COLD(cil.gfp_cmf_forced_gen_set, port_no);
        }
        else
            VTSS_E(" Frame length > 65527 not accepted");
    }
    VTSS_EXIT();
    return rc;
}


vtss_rc vtss_gfp_cmf_forced_gen_get(const vtss_inst_t       inst,
                                    const vtss_port_no_t    port_no,
                                    vtss_gfp_cmf_forced_t   *const cfg)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK && cfg) {
        *cfg = vtss_state->gfp_state[port_no].cmf_forced;
    }
    VTSS_EXIT();
    return rc;
}


vtss_rc vtss_gfp_cmf_acc_upi_pti_get(const  vtss_inst_t            inst,
                                 const  vtss_port_no_t         port_no,
                                 u32    *const                 upi,
                                 u32    *const                 pti)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK && upi) {
        rc = VTSS_FUNC_COLD(cil.gfp_cmf_acc_upi_pti_get, port_no, upi, pti);
    }
    VTSS_EXIT();
    return rc;
}


vtss_rc vtss_gfp_cdf_fcs_insert_set(const vtss_inst_t                 inst,
                                    const vtss_port_no_t              port_no,
                                    const vtss_gfp_cdf_fcs_insert_t   *const cfg)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK && cfg) {
        rc = VTSS_RC_ERROR;
        vtss_state->gfp_state[port_no].fcs_insert = *cfg;
        rc = VTSS_FUNC_COLD(cil.gfp_cdf_fcs_insert_set, port_no);
    }
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_gfp_cdf_fcs_insert_get(const vtss_inst_t            inst,
                                    const vtss_port_no_t         port_no,
                                    vtss_gfp_cdf_fcs_insert_t    *const cfg)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK && cfg) {
        *cfg = vtss_state->gfp_state[port_no].fcs_insert;
    }
    VTSS_EXIT();
    return rc;
}


vtss_rc vtss_gfp_cdf_upi_set(const vtss_inst_t           inst,
                             const vtss_port_no_t        port_no,
                             const vtss_gfp_cdf_upi_t    *const cfg)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK && cfg) {
        rc = VTSS_RC_ERROR;
        vtss_state->gfp_state[port_no].cdf_upi = *cfg;
        rc = VTSS_FUNC_COLD(cil.gfp_cdf_upi_set, port_no);
    }
    VTSS_EXIT();
    return rc;
}


vtss_rc vtss_gfp_cdf_upi_get(const vtss_inst_t      inst,
                             const vtss_port_no_t   port_no,
                             vtss_gfp_cdf_upi_t     *const cfg)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK && cfg) {
        *cfg = vtss_state->gfp_state[port_no].cdf_upi;
    }
    VTSS_EXIT();
    return rc;
}


vtss_rc vtss_gfp_cdf_acc_upi_get(const vtss_inst_t       inst,
                                 const vtss_port_no_t    port_no,
                                 u32                     *const upi)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK && upi) {
        rc = VTSS_FUNC_COLD(cil.gfp_cdf_acc_upi_get, port_no, upi);
    }
    VTSS_EXIT();
    return rc;
}


vtss_rc vtss_gfp_8b10b_control_code_set(const vtss_inst_t                     inst,
                                        const vtss_port_no_t                  port_no,
                                        const vtss_gfp_8b10b_control_code_t   *const cfg)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK && cfg) {
        rc = VTSS_RC_ERROR;
        if ((u16)cfg->err_code < (u16)VTSS_GFP_K30_7_MAX) {
            vtss_state->gfp_state[port_no].control_code = *cfg;
            rc = VTSS_FUNC_COLD(cil.gfp_8b10b_control_code_set, port_no);
        }
    }
    VTSS_EXIT();
    return rc;
}


vtss_rc vtss_gfp_8b10b_control_code_get(const vtss_inst_t                inst,
                                        const vtss_port_no_t             port_no,
                                        vtss_gfp_8b10b_control_code_t    *const cfg)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK && cfg) {
        *cfg = vtss_state->gfp_state[port_no].control_code;
    }
    VTSS_EXIT();
    return rc;
}




/* ================================================================= *
 *  State Reporting
 * ================================================================= */

vtss_rc vtss_gfp_defects_get(const vtss_inst_t       inst,
                             const vtss_port_no_t    port_no,
                             vtss_gfp_defects_t      *const defects)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK && defects) {
        rc = VTSS_FUNC_COLD(cil.gfp_defects_get, port_no, defects);
    }
    VTSS_EXIT();
    return rc;
}




/* ================================================================= *
 *  Performance Primitives
 * ================================================================= */
vtss_rc vtss_gfp_perf_get(const vtss_inst_t      inst,
                          const vtss_port_no_t   port_no,
                          vtss_gfp_perf_t        *const perf)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK && perf) {
        rc = VTSS_FUNC_COLD(cil.gfp_perf_get, port_no, perf);
    }
    VTSS_EXIT();
    return rc;
}




/* ================================================================= *
 *  Events
 * ================================================================= */
vtss_rc vtss_gfp_event_enable(const vtss_inst_t        inst,
                              const vtss_port_no_t     port_no,
                              const BOOL               enable,
                              const vtss_gfp_event_t   ev_mask)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK) {
        rc = VTSS_FUNC_COLD(cil.gfp_event_enable, port_no, enable, ev_mask);
    }
    VTSS_EXIT();
    return rc;
}


vtss_rc vtss_gfp_event_poll(const vtss_inst_t      inst,
                            const vtss_port_no_t   port_no,
                            vtss_gfp_event_t       *const status)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK && status) {
        rc = VTSS_FUNC_COLD(cil.gfp_event_poll, port_no, status);
    }
    VTSS_EXIT();
    return rc;
}


vtss_rc vtss_gfp_event_poll_without_mask(const vtss_inst_t      inst,
                                         const vtss_port_no_t   port_no,
                                         vtss_gfp_event_t       *const status)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK && status) {
        rc = VTSS_FUNC_COLD(cil.gfp_event_poll_without_mask, port_no, status);
    }
    VTSS_EXIT();
    return rc;
}


/* ================================================================= *
 *  Test/Debug Config
 * ================================================================= */


#endif /*VTSS_FEATURE_GFP */
