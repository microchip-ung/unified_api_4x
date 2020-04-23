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
