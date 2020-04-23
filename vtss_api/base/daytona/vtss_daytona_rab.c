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

// Avoid "vtss_api.h not used in module vtss_daytona_rab.c"
/*lint --e{766} */

#include "vtss_api.h"
#if defined(VTSS_ARCH_DAYTONA)
#include "../ail/vtss_state.h"
#include "vtss_daytona.h"
#include "vtss_daytona_regs.h"
#include "vtss_daytona_basics.h"
#include "vtss_rab_api.h"
#include "vtss_daytona_rab.h"
#include "vtss_daytona_regs_rab.h"


#define VTSS_DAYTONA_RAB_CUT_THRU_ENA (u16)1
#define VTSS_DAYTONA_RAB_CUT_THRU_DIS (u16)0

/* ================================================================= *
 *  Dynamic Config
 * ================================================================= */

static vtss_rc vtss_daytona_rab_config_set(vtss_state_t *vtss_state,
                                           const vtss_port_no_t port_no)
{
    u32 target;
    u32 value;
    vtss_rab_cfg_t *cfg;

    VTSS_RC(daytona_port_2_target(vtss_state, port_no, DAYTONA_BLOCK_RAB, &target));
    cfg = &(vtss_state->rab_state[port_no].rab_cfg);

    value = 0;
    value = VTSS_F_RAB_RX_FIFO_RX_FIFO_HIGH_THRESH_RX_FIFO_HIGH_THRESH(cfg->fifo_threshold_high_rx);
    DAYTONA_WR(VTSS_RAB_RX_FIFO_RX_FIFO_HIGH_THRESH(target), value);
    value = 0;
    value = VTSS_F_RAB_RX_FIFO_RX_FIFO_LOW_THRESH_RX_FIFO_LOW_THRESH(cfg->fifo_threshold_low_rx);
    DAYTONA_WR(VTSS_RAB_RX_FIFO_RX_FIFO_LOW_THRESH(target), value);
    value = 0;
    value = VTSS_F_RAB_RX_FIFO_RX_FIFO_ADAPT_THRESH_RX_FIFO_ADAPT_THRESH(cfg->fifo_threshold_adapt_rx);
    DAYTONA_WR(VTSS_RAB_RX_FIFO_RX_FIFO_ADAPT_THRESH(target), value);
    value = 0;
    value = VTSS_F_RAB_TX_FIFO_TX_FIFO_HIGH_THRESH_TX_FIFO_HIGH_THRESH(cfg->fifo_threshold_high_tx);
    DAYTONA_WR(VTSS_RAB_TX_FIFO_TX_FIFO_HIGH_THRESH(target), value);
    value = 0;
    value = VTSS_F_RAB_TX_FIFO_TX_FIFO_LOW_THRESH_TX_FIFO_LOW_THRESH(cfg->fifo_threshold_low_tx);
    DAYTONA_WR(VTSS_RAB_TX_FIFO_TX_FIFO_LOW_THRESH(target), value);
    value = 0;
    value = VTSS_F_RAB_TX_FIFO_TX_FIFO_ADAPT_THRESH_TX_FIFO_ADAPT_THRESH(cfg->fifo_threshold_adapt_tx);
    DAYTONA_WR(VTSS_RAB_TX_FIFO_TX_FIFO_ADAPT_THRESH(target), value);
    value = 0;
    value = VTSS_F_RAB_RX_FIFO_RX_FIFO_CONTROL_RX_FIFO_CUT_THRU((cfg->cut_thru_rx != FALSE) ? VTSS_DAYTONA_RAB_CUT_THRU_ENA:VTSS_DAYTONA_RAB_CUT_THRU_DIS );
    DAYTONA_WRM(VTSS_RAB_RX_FIFO_RX_FIFO_CONTROL(target), value, VTSS_M_RAB_RX_FIFO_RX_FIFO_CONTROL_RX_FIFO_CUT_THRU);
    value = 0;
    value = VTSS_F_RAB_TX_FIFO_TX_FIFO_CONTROL_TX_FIFO_CUT_THRU((cfg->cut_thru_tx != FALSE) ? VTSS_DAYTONA_RAB_CUT_THRU_ENA : VTSS_DAYTONA_RAB_CUT_THRU_DIS);
    DAYTONA_WRM(VTSS_RAB_TX_FIFO_TX_FIFO_CONTROL(target), value, VTSS_M_RAB_TX_FIFO_TX_FIFO_CONTROL_TX_FIFO_CUT_THRU);

    /*Work around for TGO-2 Sub, TGO2-Sub small, PGO2-Sub, PGO2-Sub small modes */ 
    if (vtss_state->sync_calling_private == FALSE) 
    {
        value = 0;
        value = VTSS_F_RAB_RAB_BASE_RAB_CONTROL_TX_SW_RST(1);
        DAYTONA_WRM(VTSS_RAB_RAB_BASE_RAB_CONTROL(target), value, VTSS_M_RAB_RAB_BASE_RAB_CONTROL_TX_SW_RST);    
        value = VTSS_F_RAB_RAB_BASE_RAB_CONTROL_TX_SW_RST(0);
        DAYTONA_WRM(VTSS_RAB_RAB_BASE_RAB_CONTROL(target), value, VTSS_M_RAB_RAB_BASE_RAB_CONTROL_TX_SW_RST);    
    }

   return VTSS_RC_OK;
}

static vtss_rc vtss_daytona_rab_counters_get(vtss_state_t *vtss_state,
                                             const vtss_port_no_t port_no,
                                             vtss_rab_counters_t *const counters)
{
    u32 target;
    u32 value;

    VTSS_RC(daytona_port_2_target(vtss_state, port_no, DAYTONA_BLOCK_RAB, &target));

    value = 0;
    DAYTONA_RD(VTSS_RAB_RX_FIFO_RX_PKT_DROP_CNT(target),&value);
    counters->pkt_drop_cnt_rx = value;
    value = 0;
    DAYTONA_RD(VTSS_RAB_TX_FIFO_TX_PKT_DROP_CNT(target),&value);
    counters->pkt_drop_cnt_tx = value;

    return VTSS_RC_OK;
}

/* ================================================================= *
 *  Utilities and internal
 * ================================================================= */

static vtss_rab_cfg_t  rab_default =
{
    .fifo_threshold_high_rx  = 0x1FFF,
    .fifo_threshold_low_rx   = 0x0500,
    .fifo_threshold_adapt_rx = 0x1F80,
    .fifo_threshold_high_tx  = 0x0FFF,
    .fifo_threshold_low_tx   = 0x0500,
    .fifo_threshold_adapt_tx = 0x0F80,
    .cut_thru_rx             = TRUE,
    .cut_thru_tx             = TRUE,
};

/*
 * \brief Restart rab configuration on a port
 *
 * \param port_no [IN] Port number
 *
 * \return Return code
 */
vtss_rc vtss_daytona_rab_restart_conf_set(vtss_state_t *vtss_state, vtss_port_no_t port_no)
{
    vtss_rc rc;
    u32 mode;
    const static_cfg_t *rab_conf_table;

    /* work-around for not overriding the static config values */
    VTSS_RC(daytona_port_2_mode(vtss_state, port_no, &mode));
    rab_conf_table = vtss_daytona_rab_config_get(mode);
    vtss_state->rab_state[port_no].rab_cfg.fifo_threshold_adapt_rx = rab_conf_table[7].value;
    vtss_state->rab_state[port_no].rab_cfg.fifo_threshold_adapt_tx = rab_conf_table[8].value;
    rc = vtss_daytona_rab_config_set(vtss_state, port_no);
    VTSS_D("port_no %d, restart conf set, rc = %x", port_no, rc);

    return VTSS_RC_OK;
}

/*
 * \brief Set Rab default configuration
 *
 * \param port_no [IN] Port number
 *
 * \return Return code
 */
vtss_rc vtss_daytona_rab_default_conf_set(vtss_state_t *vtss_state, vtss_port_no_t port_no)
{
    vtss_state->rab_state[port_no].rab_cfg = rab_default;

    return VTSS_RC_OK;
}
    
/**
 * \brief Create instance (set up function pointers).
 *
 * \return Return code.
 **/
vtss_rc vtss_daytona_inst_rab_create(vtss_state_t *vtss_state)
{
    vtss_port_no_t port_no;
    vtss_cil_func_t *func = &vtss_state->cil;

    for (port_no = 0 ; port_no < VTSS_PORT_ARRAY_SIZE; port_no++) {
        vtss_state->rab_state[port_no].rab_cfg = rab_default;
    }
    /* Daytona RAB functions */        
    func->rab_config_set        = vtss_daytona_rab_config_set;
    func->rab_counters_get      = vtss_daytona_rab_counters_get;
        
    return VTSS_RC_OK;
}

/**
 * \brief sync rab conifguration.
 *
 * \param inst [IN]      Target instance reference.
 * \param port_no [IN]   Port number.
 *
 * \return Return code.
 **/
vtss_rc vtss_daytona_rab_sync(vtss_state_t *vtss_state, vtss_port_no_t port_no)
{
    vtss_state->sync_calling_private = TRUE;

    VTSS_SYNC_RC(vtss_daytona_rab_config_set(vtss_state, port_no));

    vtss_state->sync_calling_private = FALSE;

    return VTSS_RC_OK;
}
#if defined(VTSS_DAYTONA_OOS_FIX)
vtss_rc vtss_daytona_rab_reconfigure(vtss_state_t *vtss_state, vtss_port_no_t port_no)
{
        VTSS_RC(vtss_daytona_rab_config_set(vtss_state, port_no));

            return VTSS_RC_OK;
}
#endif // VTSS_DAYTONA_OOS_FIX

#endif /* VTSS_ARCH_DAYTONA */
