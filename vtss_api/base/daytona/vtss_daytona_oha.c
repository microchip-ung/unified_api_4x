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

#define VTSS_TRACE_LAYER VTSS_TRACE_LAYER_CIL 

// Avoid "vtss_api.h not used in module vtss_daytona_oha.c"
/*lint --e{766} */

#include "vtss_api.h"
#if defined(VTSS_ARCH_DAYTONA) && defined(VTSS_FEATURE_OHA)
#include "../ail/vtss_state.h"
#include "vtss_daytona.h"
#include "vtss_daytona_regs.h"
#include "vtss_daytona_basics.h"
#include "vtss_daytona_oha.h"
#include "vtss_daytona_regs_devcpu_gcb.h"
#include "vtss_daytona_regs_ewis.h"


/* ================================================================= *
 *  Dynamic Config
 * ================================================================= */

static vtss_rc daytona_oha_config_set(vtss_state_t *vtss_state,
                                      const vtss_port_no_t port_no)
{
    daytona_channel_t channel;
    daytona_side_t    side;
    u32 target_wis;

    VTSS_RC(daytona_port_2_channel_side(vtss_state, port_no, &channel, &side));
    VTSS_RC(daytona_port_2_target(vtss_state, port_no, DAYTONA_BLOCK_EWIS, &target_wis));

    if (vtss_state->oha_state[port_no].oha_cfg.tosi_rosi_otu2_1 == VTSS_OHA_TOSI_ROSI_EWIS2) {
        if(side == 0) {
           return VTSS_RC_ERROR;
        }
        /* Set mux for eWIS */
        if (channel == 0) {
            DAYTONA_WRM(VTSS_DEVCPU_GCB_CHIP_MUX_DATA_MUX_0, 
                        VTSS_F_DEVCPU_GCB_CHIP_MUX_DATA_MUX_0_ROSI_TOSI_MUX_0(0),
                        VTSS_M_DEVCPU_GCB_CHIP_MUX_DATA_MUX_0_ROSI_TOSI_MUX_0);
        } else if (channel == 1) {
            DAYTONA_WRM(VTSS_DEVCPU_GCB_CHIP_MUX_DATA_MUX_1, 
                        VTSS_F_DEVCPU_GCB_CHIP_MUX_DATA_MUX_1_ROSI_TOSI_MUX_1(0),
                        VTSS_M_DEVCPU_GCB_CHIP_MUX_DATA_MUX_1_ROSI_TOSI_MUX_1);
        }
        /* Enable eWIS2 ROSI & TOSI */
         DAYTONA_WRM(VTSS_EWIS_TX_WIS_CTRL_TOSI_CTRL(target_wis), 
                     VTSS_F_EWIS_TX_WIS_CTRL_TOSI_CTRL_TOSI_ENA(1),
                     VTSS_M_EWIS_TX_WIS_CTRL_TOSI_CTRL_TOSI_ENA);

         DAYTONA_WRM(VTSS_EWIS_RX_WIS_CTRL_ROSI_CTRL(target_wis), 
                     VTSS_F_EWIS_RX_WIS_CTRL_ROSI_CTRL_ROSI_ENA(1),
                     VTSS_M_EWIS_RX_WIS_CTRL_ROSI_CTRL_ROSI_ENA);

    } else if (vtss_state->oha_state[port_no].oha_cfg.tosi_rosi_otu2_1 == VTSS_OHA_TOSI_ROSI_OTU2_1) {
        /* Set mux for OTN1 */
        if (channel == 0) {
            DAYTONA_WRM(VTSS_DEVCPU_GCB_CHIP_MUX_DATA_MUX_0, 
                        VTSS_F_DEVCPU_GCB_CHIP_MUX_DATA_MUX_0_ROSI_TOSI_MUX_0(1),
                        VTSS_M_DEVCPU_GCB_CHIP_MUX_DATA_MUX_0_ROSI_TOSI_MUX_0);
        } else if (channel == 1) {
            DAYTONA_WRM(VTSS_DEVCPU_GCB_CHIP_MUX_DATA_MUX_1, 
                        VTSS_F_DEVCPU_GCB_CHIP_MUX_DATA_MUX_1_ROSI_TOSI_MUX_1(1),
                        VTSS_M_DEVCPU_GCB_CHIP_MUX_DATA_MUX_1_ROSI_TOSI_MUX_1);
        }
    }

    if (vtss_state->oha_state[port_no].oha_cfg.otu2_1_otu2_2 == VTSS_OHA_OTU2_OTU2_1) {
        /* Set mux for OTN1 */
        if (channel == 0) {
            DAYTONA_WRM(VTSS_DEVCPU_GCB_CHIP_MUX_DATA_MUX_0, 
                        VTSS_F_DEVCPU_GCB_CHIP_MUX_DATA_MUX_0_OTN_OH_MUX_0(1),
                        VTSS_M_DEVCPU_GCB_CHIP_MUX_DATA_MUX_0_OTN_OH_MUX_0);
        } else if (channel == 1) {
            DAYTONA_WRM(VTSS_DEVCPU_GCB_CHIP_MUX_DATA_MUX_1, 
                        VTSS_F_DEVCPU_GCB_CHIP_MUX_DATA_MUX_1_OTN_OH_MUX_1(1),
                        VTSS_M_DEVCPU_GCB_CHIP_MUX_DATA_MUX_1_OTN_OH_MUX_1);
        }
    } else if (vtss_state->oha_state[port_no].oha_cfg.otu2_1_otu2_2 == VTSS_OHA_OTU2_OTU2_2) {
        /* Set mux for OTN1 */
        if (channel == 0) {
            DAYTONA_WRM(VTSS_DEVCPU_GCB_CHIP_MUX_DATA_MUX_0, 
                        VTSS_F_DEVCPU_GCB_CHIP_MUX_DATA_MUX_0_OTN_OH_MUX_0(0),
                        VTSS_M_DEVCPU_GCB_CHIP_MUX_DATA_MUX_0_OTN_OH_MUX_0);
        } else if (channel == 1) {
            DAYTONA_WRM(VTSS_DEVCPU_GCB_CHIP_MUX_DATA_MUX_1, 
                        VTSS_F_DEVCPU_GCB_CHIP_MUX_DATA_MUX_1_OTN_OH_MUX_1(0),
                        VTSS_M_DEVCPU_GCB_CHIP_MUX_DATA_MUX_1_OTN_OH_MUX_1);
        }
    }

    return VTSS_RC_OK;    
}

/* ================================================================= *
 *  State Reporting
 * ================================================================= */

/* ================================================================= *
 *  Performance Primitives
 * ================================================================= */



/* ================================================================= *
 *  Utilities and internal
 * ================================================================= */

/**
 * \brief Static Initialization of the configuration data base in the vtss_state.
 **/

static vtss_oha_cfg_t cfg_default = {
    .tosi_rosi_otu2_1  = VTSS_OHA_TOSI_ROSI_EWIS2,
    .otu2_1_otu2_2     = VTSS_OHA_OTU2_OTU2_2,
};

vtss_rc vtss_daytona_oha_restart_conf_set(vtss_state_t *vtss_state, vtss_port_no_t port_no)
{
    vtss_rc rc;

    rc = daytona_oha_config_set(vtss_state, port_no);
    VTSS_D("port_no %d, restart conf set, rc = %x", port_no, rc);
    return VTSS_RC_OK;
}

/*
 * \brief Set default oha configuration
 *
 * \param port_no [IN]   Port number
 * 
 * \return Return code.
 **/
vtss_rc vtss_daytona_oha_default_conf_set(vtss_state_t *vtss_state, vtss_port_no_t port_no)
{
    vtss_state->oha_state[port_no].oha_cfg = cfg_default;

    return VTSS_RC_OK;
}
    
/**
 * \brief Create instance (set up function pointers).
 *
 * \param inst [IN]      Target instance reference.
 * \param port_map [IN]  Port map array.
 *
 * \return Return code.
 **/
vtss_rc vtss_daytona_inst_oha_create(vtss_state_t *vtss_state)
{
    vtss_cil_func_t *func = &vtss_state->cil;
    vtss_port_no_t port_no;

    for (port_no = 0 ; port_no < VTSS_PORT_ARRAY_SIZE; port_no++) {
        vtss_state->oha_state[port_no].oha_cfg = cfg_default;
    }

    /* Daytona OHA functions */    
    func->oha_config_set = daytona_oha_config_set;

    return VTSS_RC_OK;
}

/**
 * \brief sync oha conifguration.
 *
 * \param inst [IN]      Target instance reference.
 * \param port_no [IN]   Port number.
 *
 * \return Return code.
 **/
vtss_rc vtss_daytona_oha_sync(vtss_state_t *vtss_state, vtss_port_no_t port_no)
{
    vtss_state->sync_calling_private = TRUE;

    VTSS_SYNC_RC(daytona_oha_config_set(vtss_state, port_no));

    vtss_state->sync_calling_private = FALSE;

    return VTSS_RC_OK;
}
#if defined(VTSS_DAYTONA_OOS_FIX)
vtss_rc vtss_daytona_oha_reconfigure(vtss_state_t *vtss_state, vtss_port_no_t port_no)
{
    u32 channel, side;

    VTSS_DAYTONA_RC(daytona_port_2_channel_side(vtss_state, port_no, &channel, &side));
    if (!((vtss_state->oha_state[port_no].oha_cfg.tosi_rosi_otu2_1 == VTSS_OHA_TOSI_ROSI_EWIS2)
                && (side == 0))) {
        VTSS_RC(daytona_oha_config_set(vtss_state, port_no));
    }

    return VTSS_RC_OK;
}
#endif //VTSS_DAYTONA_OOS_FIX
#endif /* VTSS_ARCH_DAYTONA */

