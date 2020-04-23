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

// Avoid "vtss_api.h not used in module vtss_i2c_api.c"
/*lint --e{766} */

#include "vtss_api.h"
#if defined (VTSS_ARCH_DAYTONA) && defined(VTSS_FEATURE_I2C)

#include "vtss_state.h"
#include "vtss_i2c_api.h"

/* ================================================================= *
 *  Defects/Events
 * ================================================================= */

/* ================================================================= *
 *  Dynamic Config
 * ================================================================= */

vtss_rc vtss_i2c_conf_set(const vtss_inst_t                  inst,
                          const vtss_i2c_controller_t *const controller_cfg,
                          const vtss_i2c_cfg_t        *const cfg)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    if (controller_cfg == NULL || cfg == NULL) {
        return VTSS_RC_ERROR;
    }

    VTSS_D("controller_no: %u", controller_cfg->controller_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK) {
        rc = VTSS_FUNC_COLD(cil.i2c_set_config, controller_cfg, cfg);
    }
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_i2c_conf_get(const vtss_inst_t                  inst,
                          const vtss_i2c_controller_t *const controller_cfg,
                                vtss_i2c_cfg_t        *const cfg)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    if (controller_cfg == NULL || cfg == NULL) {
        return VTSS_RC_ERROR;
    }

    VTSS_D("controller_no: %u", controller_cfg->controller_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK) {
        rc = VTSS_FUNC_COLD(cil.i2c_get_config, controller_cfg, cfg);
    }
    VTSS_EXIT();
    return rc;

}

vtss_rc vtss_i2c_rx(const vtss_inst_t                  inst,
                    const vtss_i2c_controller_t *const controller_cfg,
                          u8                    *const data)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    if (controller_cfg == NULL || data == NULL) {
        return VTSS_RC_ERROR;
    }

    VTSS_D("controller_no: %u", controller_cfg->controller_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK) {
        rc = VTSS_FUNC_COLD(cil.i2c_rx, controller_cfg, data);
    }
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_i2c_tx(const vtss_inst_t                  inst,
                    const vtss_i2c_controller_t *const controller_cfg,
                    const u8                    *const addr_data)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    if (controller_cfg == NULL || addr_data == NULL) {
        return VTSS_RC_ERROR;
    }

    VTSS_D("controller_no: %u", controller_cfg->controller_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK) {
        rc = VTSS_FUNC_COLD(cil.i2c_tx, controller_cfg, addr_data);
    }
    VTSS_EXIT();
    return rc;

}

vtss_rc vtss_i2c_cmd(const vtss_inst_t                  inst,
                     const vtss_i2c_controller_t *const controller_cfg,
                     const u16                          cmd_flags)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    if (controller_cfg == NULL) {
        return VTSS_RC_ERROR;
    }

    VTSS_D("controller_no: %u", controller_cfg->controller_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK) {
        rc = VTSS_FUNC_COLD(cil.i2c_cmd, controller_cfg, cmd_flags);
    }
    VTSS_EXIT();
    return rc;

}

vtss_rc vtss_i2c_stop(const vtss_inst_t                  inst,
                      const vtss_i2c_controller_t *const controller_cfg)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    if (controller_cfg == NULL) {
        return VTSS_RC_ERROR;
    }

    VTSS_D("controller_no: %u", controller_cfg->controller_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK) {
        rc = VTSS_FUNC_COLD(cil.i2c_stop, controller_cfg);
    }
    VTSS_EXIT();
    return rc;


}

/* ================================================================= *
 *  State Reporting
 * ================================================================= */
vtss_rc vtss_i2c_status_get(const vtss_inst_t                  inst,
                            const vtss_i2c_controller_t *const controller_cfg,
                                  u16                   *const status)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    if (controller_cfg == NULL || status == NULL) {
        return VTSS_RC_ERROR;
    }

    VTSS_D("controller_no: %u", controller_cfg->controller_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK) {
        rc = VTSS_FUNC_COLD(cil.i2c_get_status, controller_cfg, status);
    }
    VTSS_EXIT();
    return rc;

}

/* ================================================================= *
 *  Performance Primitives
 * ================================================================= */



#endif /*VTSS_FEATURE_I2C */
