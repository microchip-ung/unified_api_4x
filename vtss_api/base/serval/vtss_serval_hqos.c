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

#define VTSS_TRACE_GROUP VTSS_TRACE_GROUP_HQOS
#include "vtss_serval_cil.h"

#if defined(VTSS_ARCH_SERVAL)
#if defined(VTSS_FEATURE_HQOS)

static BOOL srvl_hqos_has_mp_main(vtss_hqos_entry_t *hqos)
{
    return hqos->is_mp && hqos->mp_main != VTSS_HQOS_ID_NONE;
}

static vtss_hqos_id_t srvl_hqos_get_mp_main_id(vtss_hqos_id_t hqos_id, vtss_hqos_entry_t *hqos)
{
    return srvl_hqos_has_mp_main(hqos) ? hqos->mp_main : hqos_id;
}

static u32 srvl_hqos_cost_calc(u32 rate, u32 max)
{
    /* Round half up: Multiply with 16 before division, add 8 and divide result with 16 again */
    u32 c = (((1 << 4) * max / rate) + 8) >> 4;
    return MIN((1 << VTSS_QOS_DWRR_COST_BIT_WIDTH), c); /* Force range to be 1..(1 << VTSS_QOS_DWRR_COST_BIT_WIDTH) */
}

static vtss_rc srvl_hqos_expand_nni_port(vtss_state_t *vtss_state, const vtss_port_no_t port_no)
{
    vtss_hqos_port_info_t *info = &vtss_state->hqos.port_info[port_no];
    u8                    se = info->se_top + 1;
    u8                    se_cnt = se - info->se_base + 1;
    u8                    lvl1_se = info->lvl1_se_base + se_cnt/VTSS_HQOS_SE_INPUTS;
    u8                    lvl1_inp = (se_cnt - 1) % VTSS_HQOS_SE_INPUTS;
    u8                    i;

    if (lvl1_se > info->lvl1_se_top) {
        info->lvl1_se_top = lvl1_se;

        /* Configure level 1 SE */
        SRVL_WR(VTSS_QSYS_HSCH_SE_CONNECT(lvl1_se),
                VTSS_F_QSYS_HSCH_SE_CONNECT_SE_OUTP_IDX(VTSS_HQOS_SES - port_no - 1) |
                VTSS_F_QSYS_HSCH_SE_CONNECT_SE_INP_IDX(se) |
                VTSS_F_QSYS_HSCH_SE_CONNECT_SE_OUTP_CON(lvl1_se - info->lvl1_se_base) |
                VTSS_F_QSYS_HSCH_SE_CONNECT_SE_INP_CNT(VTSS_HQOS_SE_INPUTS));
    }

    info->se_top = se + 1;

    /* Configure level 0 SEs */
    for (i = 0; i < 2; i++) {
        SRVL_WR(VTSS_QSYS_HSCH_SE_CONNECT(se + i),
                VTSS_F_QSYS_HSCH_SE_CONNECT_SE_OUTP_IDX(lvl1_se) |
                VTSS_F_QSYS_HSCH_SE_CONNECT_SE_OUTP_CON(lvl1_inp + i) |
                VTSS_F_QSYS_HSCH_SE_CONNECT_SE_INP_CNT(VTSS_HQOS_SE_INPUTS));
    }

    return VTSS_RC_OK;
}

static vtss_rc srvl_hqos_port_is_expandable(vtss_state_t *vtss_state, const vtss_port_no_t port_no, BOOL *is_expandable)
{
    vtss_hqos_port_info_t *info = &vtss_state->hqos.port_info[port_no];
    vtss_port_no_t        port_no_i;
    u8                    se = info->se_top + 1;
    u8                    se_cnt = se - info->se_base + 1;
    u8                    lvl1_se = info->lvl1_se_base + se_cnt/VTSS_HQOS_SE_INPUTS;

    if (!info->nni_mode) {
        (*is_expandable) = FALSE;
        return VTSS_RC_OK;
    }

    if (se_cnt > VTSS_HQOS_SE_INPUTS * VTSS_HQOS_SE_INPUTS) {
        (*is_expandable) = FALSE;
        return VTSS_RC_OK;
    }

    /* 4 level 0 SEs are used by the CPU ports */
    if (se >= VTSS_HQOS_LVL0_SES - 4) {
        (*is_expandable) = FALSE;
        return VTSS_RC_OK;
    }

    /* Check level 0 SE */
    for (port_no_i = VTSS_PORT_NO_START; port_no_i < vtss_state->port_count; port_no_i++) {
        if (port_no_i == port_no) {
            continue;
        }
        if (se >= vtss_state->hqos.port_info[port_no_i].se_base &&
                se <= vtss_state->hqos.port_info[port_no_i].se_top) {
            (*is_expandable) = FALSE;
            return VTSS_RC_OK;
        }
    }

    /* Check level 1 SE */
    if (lvl1_se > info->lvl1_se_top) {
        for (port_no_i = VTSS_PORT_NO_START; port_no_i < vtss_state->port_count; port_no_i++) {
            if (port_no_i == port_no) {
                continue;
            }
            if (lvl1_se >= vtss_state->hqos.port_info[port_no_i].lvl1_se_base &&
                    lvl1_se <= vtss_state->hqos.port_info[port_no_i].lvl1_se_top) {
                (*is_expandable) = FALSE;
                return VTSS_RC_OK;
            }
        }
    }

    (*is_expandable) = TRUE;
    return VTSS_RC_OK;
}

static vtss_rc srvl_hqos_map_isdx(vtss_state_t *vtss_state, u16 isdx, u16 srv_grp)
{
    SRVL_WR(VTSS_QSYS_SGRP_ISDX_SGRP(isdx),
            VTSS_F_QSYS_SGRP_ISDX_SGRP_ISDX_SGRP(srv_grp));
    return VTSS_RC_OK;
}

static vtss_rc srvl_hqos_isdx_allocate(vtss_state_t *vtss_state, vtss_hqos_id_t hqos_id, u16 isdx)
{
    vtss_hqos_entry_t *hqos = &vtss_state->hqos.info.table[hqos_id];
    vtss_hqos_isdx_t  *free_isdx = vtss_state->hqos.isdx_table.free;

    VTSS_D("hqos_id: %u, isdx: %u", hqos_id, isdx);
    if (free_isdx == NULL) {
        VTSS_E("No available HQoS ISDX resources");
        return VTSS_RC_ERROR;
    }

    vtss_state->hqos.isdx_table.free = free_isdx->next;
    free_isdx->isdx = isdx;
    free_isdx->next = hqos->isdx_list;
    hqos->isdx_list = free_isdx;

    VTSS_RC(srvl_hqos_map_isdx(vtss_state, isdx, hqos->srv_grp));

    return VTSS_RC_OK;
}

static vtss_rc srvl_hqos_isdx_free(vtss_state_t *vtss_state, vtss_hqos_id_t hqos_id, u16 isdx)
{
    vtss_hqos_entry_t *hqos = &vtss_state->hqos.info.table[hqos_id];
    vtss_hqos_isdx_t  *hqos_isdx;

    for (hqos_isdx = hqos->isdx_list; hqos_isdx != NULL; hqos_isdx = hqos_isdx->next) {
        while (hqos_isdx->next != NULL && (isdx == VTSS_ISDX_NONE || hqos_isdx->next->isdx == isdx)) {
            vtss_hqos_isdx_t *first_free = vtss_state->hqos.isdx_table.free;
            VTSS_RC(srvl_hqos_map_isdx(vtss_state, hqos_isdx->next->isdx, 0));
            vtss_state->hqos.isdx_table.free = hqos_isdx->next;
            hqos_isdx->next = hqos_isdx->next->next;
            vtss_state->hqos.isdx_table.free->next = first_free;
        }
    }
    if (hqos->isdx_list != NULL && (isdx == VTSS_ISDX_NONE || hqos->isdx_list->isdx == isdx)) {
        vtss_hqos_isdx_t *first_free = vtss_state->hqos.isdx_table.free;
        VTSS_RC(srvl_hqos_map_isdx(vtss_state, hqos->isdx_list->isdx, 0));
        vtss_state->hqos.isdx_table.free = hqos->isdx_list;
        hqos->isdx_list = hqos->isdx_list->next;
        vtss_state->hqos.isdx_table.free->next = first_free;
    }
    return VTSS_RC_OK;
}

static vtss_rc srvl_hqos_map_srv_grp(vtss_state_t *vtss_state, vtss_hqos_id_t hqos_id)
{
    vtss_hqos_entry_t *hqos = &vtss_state->hqos.info.table[hqos_id];
    vtss_hqos_isdx_t  *isdx;

    for (isdx = hqos->isdx_list; isdx != NULL; isdx = isdx->next) {
        VTSS_RC(srvl_hqos_map_isdx(vtss_state, isdx->isdx, hqos->srv_grp));
    }

    return VTSS_RC_OK;
}

static vtss_rc srvl_hqos_find_mp_srv_grp(vtss_state_t *vtss_state, vtss_hqos_id_t mp_main, vtss_hqos_id_t secondary_id, u16 *new_srv_grp)
{
    vtss_hqos_entry_t *hqos = &vtss_state->hqos.info.table[mp_main];
    u8                ports[VTSS_PORT_BF_SIZE];
    vtss_hqos_id_t    hqos_id;
    u16               srv_grp;
    BOOL              srv_grp_is_valid;

    VTSS_D("mp_main: %u, secondary_id: %u", mp_main, secondary_id);

    (*new_srv_grp) = 0;

    VTSS_PORT_BF_CLR(ports);
    VTSS_PORT_BF_SET(ports, hqos->conf.port_no, 1);
    if (secondary_id != VTSS_HQOS_ID_NONE) {
        VTSS_PORT_BF_SET(ports, vtss_state->hqos.info.table[secondary_id].conf.port_no, 1);
    }
    for (hqos_id = 0; hqos_id < VTSS_HQOS_COUNT; hqos_id++) {
        vtss_hqos_entry_t *hqos_i = &vtss_state->hqos.info.table[hqos_id];
        if (hqos_i->enable && hqos_i->is_mp &&
            (hqos_i->mp_main == mp_main || (secondary_id != VTSS_HQOS_ID_NONE && hqos_i->mp_main == secondary_id))) {
            VTSS_PORT_BF_SET(ports, hqos_i->conf.port_no, 1);
        }
    }

    /* First, try the service group where it is already assigned */
    if (hqos->srv_grp > 0 && hqos->srv_grp <= VTSS_HQOS_MP_SES) {
        srv_grp_is_valid = TRUE;
        for (hqos_id = 0; hqos_id < VTSS_HQOS_COUNT; hqos_id++) {
            vtss_hqos_entry_t *hqos_i = &vtss_state->hqos.info.table[hqos_id];
            if (hqos_i->enable && VTSS_PORT_BF_GET(ports, hqos_i->conf.port_no) && (hqos_i->isdx_list != NULL || srvl_hqos_has_mp_main(hqos_i))) {
                vtss_hqos_id_t mp_main_i = srvl_hqos_get_mp_main_id(hqos_id, hqos_i);
                hqos_i = &vtss_state->hqos.info.table[mp_main_i];
                if (hqos_i->srv_grp == hqos->srv_grp && mp_main_i != mp_main &&
                    (secondary_id == VTSS_HQOS_ID_NONE || mp_main_i != secondary_id)) {
                    srv_grp_is_valid = FALSE;
                    break;
                }
            }
        }
        if (srv_grp_is_valid) {
            (*new_srv_grp) = hqos->srv_grp;
            return VTSS_RC_OK;
        }
    }

    /* If that fails, try all the service groups */
    for (srv_grp = 1; srv_grp <= VTSS_HQOS_MP_SES; srv_grp++) {
        srv_grp_is_valid = TRUE;
        for (hqos_id = 0; hqos_id < VTSS_HQOS_COUNT; hqos_id++) {
            vtss_hqos_entry_t *hqos_i = &vtss_state->hqos.info.table[hqos_id];
            if (hqos_i->enable && VTSS_PORT_BF_GET(ports, hqos_i->conf.port_no) && (hqos_i->isdx_list != NULL || srvl_hqos_has_mp_main(hqos_i))) {
                vtss_hqos_id_t mp_main_i = srvl_hqos_get_mp_main_id(hqos_id, hqos_i);
                hqos_i = &vtss_state->hqos.info.table[mp_main_i];
                if (hqos_i->srv_grp == srv_grp && mp_main_i != mp_main &&
                    (secondary_id == VTSS_HQOS_ID_NONE || mp_main_i != secondary_id)) {
                    srv_grp_is_valid = FALSE;
                    break;
                }
            }
        }
        if (srv_grp_is_valid) {
            (*new_srv_grp) = srv_grp;
            return VTSS_RC_OK;
        }
    }

    return VTSS_RC_OK;
}

static vtss_rc srvl_hqos_find_p2p_srv_grp(vtss_state_t *vtss_state, const vtss_port_no_t port_no, u16 *new_srv_grp, BOOL *need_expansion)
{
    vtss_hqos_port_info_t *info = &vtss_state->hqos.port_info[port_no];
    u8                    se_top = info->se_top - info->se_base;
    vtss_hqos_id_t        hqos_id;
    u16                   srv_grp = VTSS_HQOS_MP_SES;
    BOOL                  srv_grp_is_valid;
    BOOL                  first_run = TRUE;
    BOOL                  is_expandable;

    (*need_expansion) = FALSE;

    do {
        srv_grp_is_valid = TRUE;
        srv_grp++;
        for (hqos_id = 0; hqos_id < VTSS_HQOS_COUNT; hqos_id++) {
            vtss_hqos_entry_t *hqos = &vtss_state->hqos.info.table[hqos_id];
            if (hqos->enable && hqos->conf.port_no == port_no && (hqos->isdx_list != NULL || srvl_hqos_has_mp_main(hqos))) {
                if (srvl_hqos_has_mp_main(hqos)) {
                    hqos = &vtss_state->hqos.info.table[hqos->mp_main];
                }
                if (hqos->srv_grp == srv_grp) {
                    srv_grp_is_valid = FALSE;
                    break;
                }
            }
        }
        if (srv_grp_is_valid) {
            (*new_srv_grp) = srv_grp;
            return VTSS_RC_OK;
        }
        if (first_run && srv_grp == se_top) {
            srv_grp = 0;
            first_run = FALSE;
        }
    } while (first_run || srv_grp < VTSS_HQOS_MP_SES);

    VTSS_RC(srvl_hqos_port_is_expandable(vtss_state, port_no, &is_expandable));
    if (is_expandable) {
        (*need_expansion) = TRUE;
        (*new_srv_grp) = info->se_top - info->se_base;
        return VTSS_RC_OK;
    }

    (*new_srv_grp) = 0;
    return VTSS_RC_OK;
}

static vtss_rc srvl_hqos_clear_p2p_maps(vtss_state_t *vtss_state, const vtss_port_no_t port_no)
{
    vtss_hqos_id_t hqos_id;

    for (hqos_id = 0; hqos_id < VTSS_HQOS_COUNT; hqos_id++) {
        vtss_hqos_entry_t *hqos = &vtss_state->hqos.info.table[hqos_id];
        if (hqos->enable && hqos->conf.port_no == port_no && !hqos->is_mp) {
            hqos->srv_grp = 0;
        }
    }

    return VTSS_RC_OK;
}

static vtss_rc srvl_hqos_connect_normal_port(vtss_state_t *vtss_state, const vtss_port_no_t port_no)
{
    u32                   port = VTSS_CHIP_PORT(port_no);
    vtss_hqos_port_info_t *info = &vtss_state->hqos.port_info[port_no];
    u8                    lvl2_se = VTSS_HQOS_SES - port_no - 1;
    u8                    se;

    /* Configure level 0 SEs */
    for (se = 0; se < 8; se++) {
        SRVL_WR(VTSS_QSYS_HSCH_SE_CONNECT(info->se_base + se),
                VTSS_F_QSYS_HSCH_SE_CONNECT_SE_OUTP_IDX(info->lvl1_se_base) |
                VTSS_F_QSYS_HSCH_SE_CONNECT_SE_OUTP_CON(se) |
                VTSS_F_QSYS_HSCH_SE_CONNECT_SE_INP_CNT(VTSS_HQOS_SE_INPUTS));

        /* Configure for frame-based round robin */
        SRVL_WRM(VTSS_QSYS_HSCH_SE_CFG(info->se_base + se),
                 VTSS_F_QSYS_HSCH_SE_CFG_SE_DWRR_CNT(VTSS_HQOS_SE_INPUTS) |
                 VTSS_F_QSYS_HSCH_SE_CFG_SE_RR_ENA,
                 VTSS_M_QSYS_HSCH_SE_CFG_SE_DWRR_CNT |
                 VTSS_F_QSYS_HSCH_SE_CFG_SE_RR_ENA);
    }

    /* Configure level 1 SE */
    SRVL_WR(VTSS_QSYS_HSCH_SE_CONNECT(info->lvl1_se_base),
            VTSS_F_QSYS_HSCH_SE_CONNECT_SE_OUTP_IDX(lvl2_se) |
            VTSS_F_QSYS_HSCH_SE_CONNECT_SE_OUTP_CON(0) |
            VTSS_F_QSYS_HSCH_SE_CONNECT_SE_INP_IDX(info->se_base) |
            VTSS_F_QSYS_HSCH_SE_CONNECT_SE_INP_CNT(VTSS_HQOS_SE_INPUTS));

    /* Configure level 2 SE */
    SRVL_WR(VTSS_QSYS_HSCH_SE_CONNECT(lvl2_se),
            VTSS_F_QSYS_HSCH_SE_CONNECT_SE_OUTP_IDX(port) |
            VTSS_F_QSYS_HSCH_SE_CONNECT_SE_INP_IDX(info->lvl1_se_base) |
            VTSS_F_QSYS_HSCH_SE_CONNECT_SE_INP_CNT(VTSS_HQOS_SE_INPUTS) |
            VTSS_F_QSYS_HSCH_SE_CONNECT_SE_TERMINAL);

    /* Configure queue mapping */
    SRVL_WR(VTSS_QSYS_QMAP_QMAP(port),
            VTSS_F_QSYS_QMAP_QMAP_SE_BASE(info->se_base) |
            VTSS_F_QSYS_QMAP_QMAP_SE_IDX_SEL(5) | /* 101: Add the frame's QoS class to SE_BASE to select the scheduling element. */
            VTSS_F_QSYS_QMAP_QMAP_SE_INP_SEL(0)); /* 00: Use the frame's ingress port as input port number on the scheduling element. */

    VTSS_RC(srvl_hqos_clear_p2p_maps(vtss_state, port_no));

    return VTSS_RC_OK;
}

static vtss_rc srvl_hqos_connect_basic_port(vtss_state_t *vtss_state, const vtss_port_no_t port_no)
{
    u32                   port = VTSS_CHIP_PORT(port_no);
    vtss_hqos_port_info_t *info = &vtss_state->hqos.port_info[port_no];

    /* Configure level 0 SE */
    SRVL_WR(VTSS_QSYS_HSCH_SE_CONNECT(info->se_base),
            VTSS_F_QSYS_HSCH_SE_CONNECT_SE_OUTP_IDX(port) |
            VTSS_F_QSYS_HSCH_SE_CONNECT_SE_INP_CNT(VTSS_HQOS_SE_INPUTS) |
            VTSS_F_QSYS_HSCH_SE_CONNECT_SE_TERMINAL);

    /* Configure queue mapping */
    SRVL_WR(VTSS_QSYS_QMAP_QMAP(port),
            VTSS_F_QSYS_QMAP_QMAP_SE_BASE(info->se_base) |
            VTSS_F_QSYS_QMAP_QMAP_SE_IDX_SEL(1) | /* 001: One level queuing. Only one scheduling element in use. */
            VTSS_F_QSYS_QMAP_QMAP_SE_INP_SEL(1)); /* 01: Use the frame's QoS class as input port number on the scheduling element. */

    VTSS_RC(srvl_hqos_clear_p2p_maps(vtss_state, port_no));

    return VTSS_RC_OK;
}

static vtss_rc srvl_hqos_connect_hier_port(vtss_state_t *vtss_state, const vtss_port_no_t port_no)
{
    u32                   port = VTSS_CHIP_PORT(port_no);
    vtss_hqos_port_info_t *info = &vtss_state->hqos.port_info[port_no];
    u8                    lvl2_se = VTSS_HQOS_SES - port_no - 1;
    u8                    lvl0_se_cnt = info->se_top - info->se_base + 1;
    u8                    se, lvl1_se_cnt = 0, lvl1_inp_idx = 0;
    vtss_hqos_id_t        hqos_id;
    u16                   p2p_srv_grp = 1 + VTSS_HQOS_MP_SES;
    vtss_shaper_t         default_shaper;

    for (se = 0; se < lvl0_se_cnt; se++) {

        /* Configure level 0 SE */
        SRVL_WR(VTSS_QSYS_HSCH_SE_CONNECT(info->se_base + se),
                VTSS_F_QSYS_HSCH_SE_CONNECT_SE_OUTP_IDX(info->lvl1_se_base + lvl1_se_cnt) |
                VTSS_F_QSYS_HSCH_SE_CONNECT_SE_OUTP_CON(lvl1_inp_idx) |
                VTSS_F_QSYS_HSCH_SE_CONNECT_SE_INP_CNT(VTSS_HQOS_SE_INPUTS));

        if (lvl1_inp_idx == 11 || se == lvl0_se_cnt - 1) {

            /* Configure level 1 SE */
            SRVL_WR(VTSS_QSYS_HSCH_SE_CONNECT(info->lvl1_se_base + lvl1_se_cnt),
                VTSS_F_QSYS_HSCH_SE_CONNECT_SE_OUTP_IDX(lvl2_se) |
                VTSS_F_QSYS_HSCH_SE_CONNECT_SE_INP_IDX(info->se_base + se - lvl1_inp_idx) |
                VTSS_F_QSYS_HSCH_SE_CONNECT_SE_OUTP_CON(lvl1_se_cnt) |
                VTSS_F_QSYS_HSCH_SE_CONNECT_SE_INP_CNT(VTSS_HQOS_SE_INPUTS));

            lvl1_inp_idx = 0;
            lvl1_se_cnt++;
        } else {
            lvl1_inp_idx++;
        }
    }

    /* Configure level 2 SE */
    SRVL_WR(VTSS_QSYS_HSCH_SE_CONNECT(lvl2_se),
            VTSS_F_QSYS_HSCH_SE_CONNECT_SE_OUTP_IDX(port) |
            VTSS_F_QSYS_HSCH_SE_CONNECT_SE_INP_IDX(info->lvl1_se_base) |
            VTSS_F_QSYS_HSCH_SE_CONNECT_SE_INP_CNT(VTSS_HQOS_SE_INPUTS) |
            VTSS_F_QSYS_HSCH_SE_CONNECT_SE_TERMINAL);

    /* Configure queue mapping */
    SRVL_WR(VTSS_QSYS_QMAP_QMAP(port),
            VTSS_F_QSYS_QMAP_QMAP_SE_BASE(info->se_base) |
            VTSS_F_QSYS_QMAP_QMAP_SE_IDX_SEL(6) | /* 110: Add the frame's service group to SE_BASE to select the scheduling element. */
            VTSS_F_QSYS_QMAP_QMAP_SE_INP_SEL(1)); /* 01: Use the frame's QoS class as input port number on the scheduling element. */

    VTSS_RC(srvl_hqos_clear_p2p_maps(vtss_state, port_no));

    /* Map point-to-point HQoS IDs */
    for (hqos_id = 0; hqos_id < VTSS_HQOS_COUNT; hqos_id++) {
        vtss_hqos_entry_t *hqos = &vtss_state->hqos.info.table[hqos_id];
        if (hqos->enable && hqos->conf.port_no == port_no && !hqos->is_mp && hqos->isdx_list != NULL) {
            if (info->se_base + p2p_srv_grp > info->se_top) {
                u16  new_srv_grp;
                BOOL need_expansion;
                VTSS_RC(srvl_hqos_find_p2p_srv_grp(vtss_state, port_no, &new_srv_grp, &need_expansion));
                hqos->srv_grp = new_srv_grp;
            } else {
                hqos->srv_grp = p2p_srv_grp;
                p2p_srv_grp++;
            }
            VTSS_RC(srvl_hqos_map_srv_grp(vtss_state, hqos_id));
        }
    }

    /* Disable shaper for non-service traffic */
    default_shaper.rate = VTSS_BITRATE_DISABLED;
    default_shaper.eir = VTSS_BITRATE_DISABLED;
    default_shaper.mode = VTSS_SHAPER_MODE_LINE;
    VTSS_RC(vtss_srvl_qos_shaper_conf_set(vtss_state, &default_shaper, info->se_base, TRUE, port, 0, NULL));

    return VTSS_RC_OK;
}

static vtss_rc srvl_hqos_port_enable(vtss_state_t *vtss_state, const vtss_port_no_t port_no, BOOL enable)
{
    u32 port = VTSS_CHIP_PORT(port_no);
    u32 value, delay = 0;

    if (enable) {
        /* Enable port for frame transfer */
        SRVL_WRM_SET(VTSS_QSYS_SYSTEM_SWITCH_PORT_MODE(port),
                     VTSS_F_QSYS_SYSTEM_SWITCH_PORT_MODE_PORT_ENA);
    } else {
        /* Disable traffic being sent to or from switch port */
        SRVL_WRM_CLR(VTSS_QSYS_SYSTEM_SWITCH_PORT_MODE(port),
                     VTSS_F_QSYS_SYSTEM_SWITCH_PORT_MODE_PORT_ENA);

        /* Flush the queues associated with the port */
        SRVL_WRM_SET(VTSS_REW_PORT_PORT_CFG(port),
                     VTSS_F_REW_PORT_PORT_CFG_FLUSH_ENA);

        do { /* Wait until flushing is complete */
            SRVL_RD(VTSS_QSYS_SYSTEM_SW_STATUS(port), &value);
            VTSS_MSLEEP(1);
            delay++;
            if (delay == 2000) {
                VTSS_E("Flush timeout port_no %u\n", port_no);
                break;
            }
        } while (value & VTSS_M_QSYS_SYSTEM_SW_STATUS_EQ_AVAIL);

        /* Clear flushing */
        SRVL_WRM_CLR(VTSS_REW_PORT_PORT_CFG(port), VTSS_F_REW_PORT_PORT_CFG_FLUSH_ENA);
    }
    return VTSS_RC_OK;
}

static vtss_rc srvl_hqos_conf_valid(vtss_state_t *vtss_state, const vtss_port_no_t ignore_port_no, u8 se_cnt, BOOL *valid, BOOL *can_be_valid)
{
    u8             se;
    vtss_port_no_t port_no;
    u8             total_vacant = 0, current_vacant_group = 0, largest_vacant_group = 0;

    /* 4 level 0 SEs are used by the CPU ports */
    for (se = 0; se < VTSS_HQOS_LVL0_SES - 4; se++) {
        BOOL se_is_vacant = TRUE;
        for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {
            if (port_no == ignore_port_no) {
                continue;
            }
            if (se >= vtss_state->hqos.port_info[port_no].se_base &&
                se <= vtss_state->hqos.port_info[port_no].se_top) {
                se_is_vacant = FALSE;
                break;
            }
        }
        if (se_is_vacant) {
            total_vacant++;
            current_vacant_group++;
        } else {
            if (current_vacant_group > largest_vacant_group) {
                largest_vacant_group = current_vacant_group;
            }
            current_vacant_group = 0;
            se = vtss_state->hqos.port_info[port_no].se_top;
        }
    }

    if (current_vacant_group > largest_vacant_group) {
        largest_vacant_group = current_vacant_group;
    }
    (*valid) = VTSS_BOOL(largest_vacant_group >= se_cnt);
    (*can_be_valid) = VTSS_BOOL(total_vacant >= se_cnt);

    return VTSS_RC_OK;
}

static vtss_rc srvl_hqos_find_top_vacant_se(vtss_state_t *vtss_state, const vtss_port_no_t port_no, u8 se_cnt, u8 *vacant_se_base, u8 *vacant_se_top)
{
    u8             se;
    vtss_port_no_t port_no_i;

    /* 4 level 0 SEs are used by the CPU ports */
    for (se = VTSS_HQOS_LVL0_SES - se_cnt - 4; ; se--) {
        BOOL se_is_vacant = TRUE;
        for (port_no_i = VTSS_PORT_NO_START; port_no_i < vtss_state->port_count; port_no_i++) {
            if (port_no_i == port_no) {
                continue;
            }
            if (se <= vtss_state->hqos.port_info[port_no_i].se_top &&
                se + se_cnt > vtss_state->hqos.port_info[port_no_i].se_base) {
                se_is_vacant = FALSE;
                if (vtss_state->hqos.port_info[port_no_i].se_base < se_cnt) {
                    se = 0;
                } else {
                    se = vtss_state->hqos.port_info[port_no_i].se_base - se_cnt + 1;
                }
                break;
            }
        }
        if (se_is_vacant) {
            (*vacant_se_base) = se;
            (*vacant_se_top) = se + se_cnt - 1;
            return VTSS_RC_OK;
        } else if (se == 0) {
            break;
        }
    }

    VTSS_E("No available scheduling resources");
    return VTSS_RC_ERROR;
}

static vtss_rc srvl_hqos_find_top_vacant_lvl1_se(vtss_state_t *vtss_state, const vtss_port_no_t port_no, u8 se_cnt, u8 *vacant_lvl1_se_base, u8 *vacant_lvl1_se_top)
{
    u8             se;
    vtss_port_no_t port_no_i;

    /* A level 2 SE is used by each port */
    for (se = VTSS_HQOS_SES - se_cnt - vtss_state->port_count; ; se--) {
        BOOL se_is_vacant = TRUE;
        for (port_no_i = VTSS_PORT_NO_START; port_no_i < vtss_state->port_count; port_no_i++) {
            if (port_no_i == port_no) {
                continue;
            }
            if (se <= vtss_state->hqos.port_info[port_no_i].lvl1_se_top &&
                se + se_cnt > vtss_state->hqos.port_info[port_no_i].lvl1_se_base) {
                se_is_vacant = FALSE;
                if (vtss_state->hqos.port_info[port_no_i].lvl1_se_base - VTSS_HQOS_LVL0_SES < se_cnt) {
                    se = VTSS_HQOS_LVL0_SES;
                } else {
                    se = vtss_state->hqos.port_info[port_no_i].lvl1_se_base - se_cnt + 1;
                }
                break;
            }
        }
        if (se_is_vacant) {
            (*vacant_lvl1_se_base) = se;
            (*vacant_lvl1_se_top) = se + se_cnt - 1;
            return VTSS_RC_OK;
        } else if (se == VTSS_HQOS_LVL0_SES) {
            break;
        }
    }

    VTSS_E("No available scheduling resources");
    return VTSS_RC_ERROR;
}

static vtss_rc srvl_hqos_find_vacant_nni_se(vtss_state_t *vtss_state, const vtss_port_no_t port_no, u8 se_cnt, u8 *vacant_nni_se_base, u8 *vacant_nni_se_top)
{
    u8             se;
    vtss_port_no_t port_no_i;

    /* 4 level 0 SEs are used by the CPU ports */
    for (se = 0; se <= VTSS_HQOS_LVL0_SES - se_cnt - 4; se++) {
        BOOL se_is_vacant = TRUE;
        for (port_no_i = VTSS_PORT_NO_START; port_no_i < vtss_state->port_count; port_no_i++) {
            if (port_no_i == port_no) {
                continue;
            }
            if (se <= vtss_state->hqos.port_info[port_no_i].se_top &&
                se + se_cnt > vtss_state->hqos.port_info[port_no_i].se_base) {
                se_is_vacant = FALSE;
                se = vtss_state->hqos.port_info[port_no_i].se_top;
                break;
            }
        }
        if (se_is_vacant) {
            (*vacant_nni_se_base) = se;
            (*vacant_nni_se_top) = se + se_cnt - 1;
            return VTSS_RC_OK;
        }
    }

    VTSS_E("No available scheduling resources");
    return VTSS_RC_ERROR;
}

static vtss_rc srvl_hqos_find_vacant_nni_lvl1_se(vtss_state_t *vtss_state, const vtss_port_no_t port_no, u8 se_cnt, u8 *vacant_nni_lvl1_se_base, u8 *vacant_nni_lvl1_se_top)
{
    u8             se;
    vtss_port_no_t port_no_i;

    /* A level 2 SE is used by each port */
    for (se = VTSS_HQOS_LVL0_SES; se <= VTSS_HQOS_SES - se_cnt - vtss_state->port_count; se++) {
        BOOL se_is_vacant = TRUE;
        for (port_no_i = VTSS_PORT_NO_START; port_no_i < vtss_state->port_count; port_no_i++) {
            if (port_no_i == port_no) {
                continue;
            }
            if (se <= vtss_state->hqos.port_info[port_no_i].lvl1_se_top &&
                se + se_cnt > vtss_state->hqos.port_info[port_no_i].lvl1_se_base) {
                se_is_vacant = FALSE;
                se = vtss_state->hqos.port_info[port_no_i].lvl1_se_top;
                break;
            }
        }
        if (se_is_vacant) {
            (*vacant_nni_lvl1_se_base) = se;
            (*vacant_nni_lvl1_se_top) = se + se_cnt - 1;
            return VTSS_RC_OK;
        }
    }

    VTSS_E("No available scheduling resources");
    return VTSS_RC_ERROR;
}

static vtss_rc srvl_hqos_get_hqos_cnt(vtss_state_t *vtss_state, const vtss_port_no_t port_no, BOOL map_mp, u32 *hqos_cnt, BOOL *mp_conflict)
{
    vtss_hqos_id_t hqos_id;

    (*hqos_cnt) = 0;
    (*mp_conflict) = FALSE;
    for (hqos_id = 0; hqos_id < VTSS_HQOS_COUNT; hqos_id++) {
        vtss_hqos_entry_t *hqos = &vtss_state->hqos.info.table[hqos_id];
        if (hqos->enable && hqos->conf.port_no == port_no && (hqos->isdx_list != NULL || srvl_hqos_has_mp_main(hqos))) {
            if (hqos->is_mp) {
                u16            new_srv_grp;
                vtss_hqos_id_t mp_main;
                if (srvl_hqos_has_mp_main(hqos)) {
                    mp_main = hqos->mp_main;
                    hqos = &vtss_state->hqos.info.table[mp_main];
                } else {
                    mp_main = hqos_id;
                }
                if (hqos->srv_grp == 0) {
                    VTSS_RC(srvl_hqos_find_mp_srv_grp(vtss_state, mp_main, VTSS_HQOS_ID_NONE, &new_srv_grp));
                    if (new_srv_grp == 0) {
                        (*mp_conflict) = TRUE;
                    } else if (map_mp) {
                        hqos->srv_grp = new_srv_grp;
                        VTSS_RC(srvl_hqos_map_srv_grp(vtss_state, mp_main));
                    }
                }
            }
            (*hqos_cnt)++;
        }
    }

    return VTSS_RC_OK;
}

static vtss_rc srvl_hqos_map_cpu_ports(vtss_state_t *vtss_state)
{
    u32 i;

    /* Map the 2 CPU ports in Basic Mode on the top even level 0 SEs */
    for (i = 0; i < 2; i++) {
        u8 se = VTSS_HQOS_LVL0_SES - 2*i - 2;

        /* Configure level 0 SE */
        SRVL_WR(VTSS_QSYS_HSCH_SE_CONNECT(se),
                VTSS_F_QSYS_HSCH_SE_CONNECT_SE_OUTP_IDX(VTSS_CHIP_PORT_CPU + i) |
                VTSS_F_QSYS_HSCH_SE_CONNECT_SE_INP_CNT(VTSS_HQOS_SE_INPUTS) |
                VTSS_F_QSYS_HSCH_SE_CONNECT_SE_TERMINAL);

        /* Configure queue mapping */
        SRVL_WR(VTSS_QSYS_QMAP_QMAP(VTSS_CHIP_PORT_CPU + i),
                VTSS_F_QSYS_QMAP_QMAP_SE_BASE(se) |
                VTSS_F_QSYS_QMAP_QMAP_SE_IDX_SEL(1) | /* 001: One level queuing. Only one scheduling element in use. */
                VTSS_F_QSYS_QMAP_QMAP_SE_INP_SEL(1)); /* 01: Use the frame's QoS class as input port number on the scheduling element. */

    }

    return VTSS_RC_OK;
}

static vtss_rc srvl_hqos_reallocate(vtss_state_t *vtss_state)
{
    /* 2 SEs are used by the CPU ports */
    u8             top_se_base = VTSS_HQOS_LVL0_SES - 4;
    u8             top_lvl1_se_base = VTSS_HQOS_SES - vtss_state->port_count;
    u8             nni_se_base = 0;
    u8             nni_lvl1_se_base = VTSS_HQOS_LVL0_SES;
    vtss_port_no_t port_no;

    VTSS_D("Reallocating scheduling resources");

    /* Disable all ports */
    for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {
        VTSS_RC(srvl_hqos_port_enable(vtss_state, port_no, FALSE));
    }

    for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {
        vtss_hqos_port_info_t *info = &vtss_state->hqos.port_info[port_no];
        u8                    se_top, lvl1_se_top;
        switch(info->sch_mode) {
        case VTSS_HQOS_SCH_MODE_NORMAL:
            se_top = top_se_base - 1;
            top_se_base = se_top - 7;
            info->se_base = top_se_base;
            info->se_top = se_top;
            top_lvl1_se_base--;
            info->lvl1_se_base = top_lvl1_se_base;
            info->lvl1_se_top = top_lvl1_se_base;
            VTSS_RC(srvl_hqos_connect_normal_port(vtss_state, port_no));
            break;
        case VTSS_HQOS_SCH_MODE_BASIC:
            se_top = top_se_base - 1;
            top_se_base = se_top - 1;
            info->se_base = top_se_base;
            info->se_top = se_top;
            info->lvl1_se_base = 0;
            info->lvl1_se_top = 0;
            VTSS_RC(srvl_hqos_connect_basic_port(vtss_state, port_no));
            break;
        case VTSS_HQOS_SCH_MODE_HIERARCHICAL:
            if (info->nni_mode) {
                u32  hqos_cnt;
                BOOL mp_conflict;
                u8   se_cnt;
                VTSS_RC(srvl_hqos_get_hqos_cnt(vtss_state, port_no, TRUE, &hqos_cnt, &mp_conflict));
                se_cnt = MAX(hqos_cnt + 1, VTSS_HQOS_NNI_SES);
                if (se_cnt % 2 == 1) {
                    /* SE count must be even */
                    se_cnt++;
                }
                se_top = nni_se_base + se_cnt - 1;
                lvl1_se_top = nni_lvl1_se_base + SRVL_DIV_ROUND_UP(se_cnt, VTSS_HQOS_SE_INPUTS) - 1;
                info->se_base = nni_se_base;
                info->se_top = se_top;
                info->lvl1_se_base = nni_lvl1_se_base;
                info->lvl1_se_top = lvl1_se_top;
                nni_se_base = se_top + 1;
                nni_lvl1_se_base = lvl1_se_top + 1;
            } else {
                se_top = top_se_base - 1;
                top_se_base = se_top - VTSS_HQOS_UNI_SES + 1;
                lvl1_se_top = top_lvl1_se_base - 1;
                top_lvl1_se_base = lvl1_se_top - SRVL_DIV_ROUND_UP(VTSS_HQOS_UNI_SES, VTSS_HQOS_SE_INPUTS) + 1;
                info->se_base = top_se_base;
                info->se_top = se_top;
                info->lvl1_se_base = top_lvl1_se_base;
                info->lvl1_se_top = lvl1_se_top;
            }
            VTSS_RC(srvl_hqos_connect_hier_port(vtss_state, port_no));
            break;
        default:
            VTSS_E("Illegal scheduling mode: %d", info->sch_mode);
            return VTSS_RC_ERROR;
        }
        if (info->se_base % 2 == 1) {
            VTSS_E("se_base %u is odd on port_no %u (must be even)", info->se_base, port_no);
        }

        VTSS_FUNC_RC(qos.port_conf_update, port_no);
    }

    /* Enable all ports */
    for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {
        VTSS_RC(srvl_hqos_port_enable(vtss_state, port_no, TRUE));
    }

    return VTSS_RC_OK;
}

static vtss_rc srvl_hqos_sch_mode_set(vtss_state_t *vtss_state, const vtss_port_no_t port_no)
{
    vtss_hqos_port_info_t *info = &vtss_state->hqos.port_info[port_no];
    vtss_hqos_sch_mode_t  sch_mode = vtss_state->hqos.port_conf[port_no].sch_mode;
    u8                    se_cnt = 0, se_base = 0, se_top = 0;
    u8                    lvl1_se_base = 0, lvl1_se_top = 0;
    u32                   hqos_cnt;
    BOOL                  mp_conflict;
    BOOL                  valid, can_be_valid;

    if (sch_mode == info->sch_mode) {
        return VTSS_RC_OK;
    }

    VTSS_D("port_no: %u, sch_mode: %u", port_no, sch_mode);

    switch(sch_mode) {
    case VTSS_HQOS_SCH_MODE_NORMAL:
        se_cnt = 8;
        break;
    case VTSS_HQOS_SCH_MODE_BASIC:
        se_cnt = 2;
        break;
    case VTSS_HQOS_SCH_MODE_HIERARCHICAL:
        VTSS_RC(srvl_hqos_get_hqos_cnt(vtss_state, port_no, TRUE, &hqos_cnt, &mp_conflict));

        if (hqos_cnt == 0) {
            return VTSS_RC_OK;
        } else if (info->nni_mode) {
            se_cnt = MAX(hqos_cnt + 1, VTSS_HQOS_NNI_SES);
            if (se_cnt % 2 == 1) {
                /* SE count must be even */
                se_cnt++;
            }
        } else {
            se_cnt = VTSS_HQOS_UNI_SES;
        }
        break;
    default:
        VTSS_E("Illegal scheduling mode: %d", sch_mode);
        return VTSS_RC_ERROR;
    }

    VTSS_RC(srvl_hqos_conf_valid(vtss_state, port_no, se_cnt, &valid, &can_be_valid));

    if (valid) {
        switch(sch_mode) {
        case VTSS_HQOS_SCH_MODE_NORMAL:
            VTSS_RC(srvl_hqos_find_top_vacant_se(vtss_state, port_no, se_cnt, &se_base, &se_top));
            VTSS_RC(srvl_hqos_find_top_vacant_lvl1_se(vtss_state, port_no, 1, &lvl1_se_base, &lvl1_se_top));
            break;
        case VTSS_HQOS_SCH_MODE_BASIC:
            VTSS_RC(srvl_hqos_find_top_vacant_se(vtss_state, port_no, se_cnt, &se_base, &se_top));
            break;
        case VTSS_HQOS_SCH_MODE_HIERARCHICAL:
            if (info->nni_mode) {
                VTSS_RC(srvl_hqos_find_vacant_nni_se(vtss_state, port_no, se_cnt, &se_base, &se_top));
                VTSS_RC(srvl_hqos_find_vacant_nni_lvl1_se(vtss_state, port_no,
                                                          SRVL_DIV_ROUND_UP(se_cnt, VTSS_HQOS_SE_INPUTS),
                                                          &lvl1_se_base, &lvl1_se_top));
            } else {
                VTSS_RC(srvl_hqos_find_top_vacant_se(vtss_state, port_no, se_cnt, &se_base, &se_top));
                VTSS_RC(srvl_hqos_find_top_vacant_lvl1_se(vtss_state, port_no,
                                                          SRVL_DIV_ROUND_UP(se_cnt, VTSS_HQOS_SE_INPUTS),
                                                          &lvl1_se_base, &lvl1_se_top));
            }
            break;
        }
    }

    VTSS_D("Changing scheduling mode for port_no %u to %s", port_no,
            sch_mode == VTSS_HQOS_SCH_MODE_NORMAL ? "Normal" :
            sch_mode == VTSS_HQOS_SCH_MODE_BASIC ? "Basic" :
            sch_mode == VTSS_HQOS_SCH_MODE_HIERARCHICAL ? "Hierarchical" : "Unknown");

    info->sch_mode = sch_mode;

    if (valid) {
        info->se_base = se_base;
        info->se_top = se_top;
        info->lvl1_se_base = lvl1_se_base;
        info->lvl1_se_top = lvl1_se_top;

        VTSS_RC(srvl_hqos_port_enable(vtss_state, port_no, FALSE));

        switch(sch_mode) {
        case VTSS_HQOS_SCH_MODE_NORMAL:
            VTSS_RC(srvl_hqos_connect_normal_port(vtss_state, port_no));
            break;
        case VTSS_HQOS_SCH_MODE_BASIC:
            VTSS_RC(srvl_hqos_connect_basic_port(vtss_state, port_no));
            break;
        case VTSS_HQOS_SCH_MODE_HIERARCHICAL:
            VTSS_RC(srvl_hqos_connect_hier_port(vtss_state, port_no));
            break;
        }

        if (info->se_base % 2 == 1) {
            VTSS_E("se_base %u is odd on port_no %u (must be even)", info->se_base, port_no);
        }

        VTSS_FUNC_RC(qos.port_conf_update, port_no);
        VTSS_RC(srvl_hqos_port_enable(vtss_state, port_no, TRUE));
    } else {
        if (can_be_valid) {
            VTSS_RC(srvl_hqos_reallocate(vtss_state));
        } else {
            VTSS_E("No available scheduling resources");
            return VTSS_RC_ERROR;
        }
    }

    return VTSS_RC_OK;
}

static vtss_rc srvl_hqos_sch_mode_check(vtss_state_t *vtss_state, const vtss_port_no_t port_no)
{
    vtss_hqos_port_info_t *info = &vtss_state->hqos.port_info[port_no];
    vtss_hqos_sch_mode_t  sch_mode = vtss_state->hqos.port_conf[port_no].sch_mode;
    u8                    se_cnt;
    u32                   hqos_cnt;
    BOOL                  mp_conflict;
    BOOL                  valid, can_be_valid;

    if (sch_mode == info->sch_mode) {
        return VTSS_RC_OK;
    }

    VTSS_D("port_no: %u, sch_mode: %u", port_no, sch_mode);

    switch(sch_mode) {
    case VTSS_HQOS_SCH_MODE_NORMAL:
        se_cnt = 8;
        break;
    case VTSS_HQOS_SCH_MODE_BASIC:
        se_cnt = 2;
        break;
    case VTSS_HQOS_SCH_MODE_HIERARCHICAL:
        VTSS_RC(srvl_hqos_get_hqos_cnt(vtss_state, port_no, FALSE, &hqos_cnt, &mp_conflict));

        if (hqos_cnt == 0) {
            return VTSS_RC_OK;
        } else if (info->nni_mode) {
            if (hqos_cnt + 1 > VTSS_HQOS_LVL0_SES) {
                VTSS_I("Too many HQoS entries on port_no %u for hierarchical scheduling mode", port_no);
                return VTSS_RC_ERROR;
            }
            se_cnt = MAX(hqos_cnt + 1, VTSS_HQOS_NNI_SES);
            if (se_cnt % 2 == 1) {
                /* SE count must be even */
                se_cnt++;
            }
        } else {
            if (hqos_cnt + 1 > VTSS_HQOS_UNI_SES) {
                VTSS_I("Too many HQoS entries on port_no %u for hierarchical scheduling mode", port_no);
                return VTSS_RC_ERROR;
            }
            se_cnt = VTSS_HQOS_UNI_SES;
        }

        if (mp_conflict) {
            VTSS_I("Too many multi point HQoS mappings on port_no %u for hierarchical scheduling mode", port_no);
            return VTSS_RC_ERROR;
        }
        break;
    default:
        VTSS_E("Illegal scheduling mode: %d", sch_mode);
        return VTSS_RC_ERROR;
    }

    VTSS_RC(srvl_hqos_conf_valid(vtss_state, port_no, se_cnt, &valid, &can_be_valid));

    if (!can_be_valid) {
        VTSS_I("No available scheduling resources");
        return VTSS_RC_ERROR;
    }

    return VTSS_RC_OK;
}

/* - CIL functions ------------------------------------------------- */

static vtss_rc srvl_hqos_port_conf_set(struct vtss_state_s *vtss_state, const vtss_port_no_t port_no)
{
    VTSS_D("port_no: %u", port_no);

    /* Check change of scheduling mode */
    VTSS_RC(srvl_hqos_sch_mode_check(vtss_state, port_no));

    /* Apply change of scheduling mode */
    VTSS_RC(srvl_hqos_sch_mode_set(vtss_state, port_no));

    /* Update EVC mapping */
#if defined(VTSS_FEATURE_EVC)
    VTSS_RC(vtss_cmn_evc_hqos_map(vtss_state));
#endif /* VTSS_FEATURE_EVC */

    return VTSS_RC_OK;
}

static vtss_rc srvl_hqos_add(struct vtss_state_s *vtss_state, const vtss_hqos_id_t hqos_id, const vtss_hqos_conf_t *const conf)
{
    vtss_hqos_entry_t *hqos = &vtss_state->hqos.info.table[hqos_id];

    VTSS_D("hqos_id: %u", hqos_id);

    if (hqos->enable) {
        if (hqos->conf.port_no != conf->port_no) {
            VTSS_I("port_no cannot be changed on an existing HQoS entry");
            return VTSS_RC_ERROR;
        }
        hqos->conf = *conf;

        /* Update EVC mapping */
#if defined(VTSS_FEATURE_EVC)
        VTSS_RC(vtss_cmn_evc_hqos_map(vtss_state));
#endif /* VTSS_FEATURE_EVC */

        VTSS_FUNC_RC(qos.port_conf_update, hqos->conf.port_no);
    } else {
        memset(hqos, 0, sizeof(*hqos));
        hqos->conf = *conf;
        hqos->enable = TRUE;
        vtss_state->hqos.info.count++;
#if defined(VTSS_FEATURE_MPLS)
        vtss_srvl_mpls_hqos_change_notification(vtss_state, hqos_id, TRUE);
#endif
    }
    return VTSS_RC_OK;
}

static vtss_rc srvl_hqos_del(struct vtss_state_s *vtss_state, const vtss_hqos_id_t hqos_id)
{
    vtss_hqos_entry_t *hqos = &vtss_state->hqos.info.table[hqos_id];

    VTSS_D("hqos_id: %u", hqos_id);

    if (!hqos->enable) {
        VTSS_I("hqos_id %u does not exist", hqos_id);
        return VTSS_RC_ERROR;
    }
    if (hqos->is_mp) {
        if (hqos->mp_main == VTSS_HQOS_ID_NONE) {
            vtss_hqos_id_t hqos_id_i, new_mp_main = VTSS_HQOS_ID_NONE;
            for (hqos_id_i = 0; hqos_id_i < VTSS_HQOS_COUNT; hqos_id_i++) {
                vtss_hqos_entry_t *hqos_i = &vtss_state->hqos.info.table[hqos_id_i];
                if (hqos_i->enable && hqos_i->is_mp && hqos_i->mp_main == hqos_id) {
                    new_mp_main = hqos_id_i;
                    break;
                }
            }
            if (new_mp_main != VTSS_HQOS_ID_NONE) {
                vtss_state->hqos.info.table[new_mp_main].mp_main = VTSS_HQOS_ID_NONE;
                vtss_state->hqos.info.table[new_mp_main].srv_grp = hqos->srv_grp;
                vtss_state->hqos.info.table[new_mp_main].isdx_list = hqos->isdx_list;
                for (hqos_id_i = 0; hqos_id_i < VTSS_HQOS_COUNT; hqos_id_i++) {
                    vtss_hqos_entry_t *hqos_i = &vtss_state->hqos.info.table[hqos_id_i];
                    if (hqos_i->enable && hqos_i->is_mp && hqos_i->mp_main == hqos_id) {
                        hqos_i->mp_main = new_mp_main;
                    }
                }
            } else {
                VTSS_RC(srvl_hqos_isdx_free(vtss_state, hqos_id, VTSS_ISDX_NONE));
            }
        }
    } else {
        VTSS_RC(srvl_hqos_isdx_free(vtss_state, hqos_id, VTSS_ISDX_NONE));
    }
    hqos->enable = FALSE;
    vtss_state->hqos.info.count--;

    /* Update EVC mapping */
#if defined(VTSS_FEATURE_EVC)
    VTSS_RC(vtss_cmn_evc_hqos_map(vtss_state));
#endif /* VTSS_FEATURE_EVC */

#if defined(VTSS_FEATURE_MPLS)
    vtss_srvl_mpls_hqos_change_notification(vtss_state, hqos_id, FALSE);
#endif

    VTSS_FUNC_RC(qos.port_conf_update, hqos->conf.port_no);

    return VTSS_RC_OK;
}

static vtss_rc srvl_hqos_map(struct vtss_state_s *vtss_state, const vtss_hqos_id_t hqos_id, const BOOL port_is_nni)
{
    vtss_hqos_entry_t *hqos = &vtss_state->hqos.info.table[hqos_id];

    VTSS_D("hqos_id: %u, port_is_nni: %u", hqos_id, port_is_nni);

    if (!hqos->enable) {
        return VTSS_RC_ERROR;
    }
    vtss_state->hqos.port_info[hqos->conf.port_no].nni_mode = port_is_nni;
    return VTSS_RC_OK;
}

static vtss_rc srvl_hqos_isdx_add(struct vtss_state_s *vtss_state, const vtss_hqos_id_t hqos_id, u16 isdx, const BOOL check_only)
{
    vtss_hqos_entry_t *hqos = &vtss_state->hqos.info.table[hqos_id];
    vtss_hqos_id_t    hqos_id_i;
    vtss_hqos_entry_t *hqos_i;
    vtss_hqos_isdx_t  *isdx_i;

    VTSS_D("hqos_id: %u, isdx: %u, check_only: %u", hqos_id, isdx, check_only);

    if (!hqos->enable) {
        VTSS_I("hqos_id %u does not exist", hqos_id);
        return VTSS_RC_ERROR;
    }

    /* Check if it is just a remapping to the same HQoS ID */
    hqos_id_i = srvl_hqos_get_mp_main_id(hqos_id, hqos);
    hqos_i = &vtss_state->hqos.info.table[hqos_id_i];
    for (isdx_i = hqos_i->isdx_list; isdx_i != NULL; isdx_i = isdx_i->next) {
        if (isdx_i->isdx == isdx) {
            return VTSS_RC_OK;
        }
    }

    /* Check if the ISDX has been mapped before */
    for (hqos_id_i = 0; hqos_id_i < VTSS_HQOS_COUNT; hqos_id_i++) {
        hqos_i = &vtss_state->hqos.info.table[hqos_id_i];
        if (!hqos_i->enable) {
            continue;
        }
        for (isdx_i = hqos_i->isdx_list; isdx_i != NULL; isdx_i = isdx_i->next) {
            if (isdx_i->isdx == isdx) {
                vtss_hqos_id_t    mp_main_id, mp_main_id_i, secondary_id = VTSS_HQOS_ID_NONE;
                vtss_hqos_entry_t *mp_main, *mp_main_i;
                u16               new_srv_grp;

                /* Find multi-point main entries */
                if (hqos_i->is_mp || !hqos->is_mp) {
                    mp_main_id = srvl_hqos_get_mp_main_id(hqos_id_i, hqos_i);
                    mp_main_id_i = srvl_hqos_get_mp_main_id(hqos_id, hqos);
                } else {
                    mp_main_id = srvl_hqos_get_mp_main_id(hqos_id, hqos);
                    mp_main_id_i = srvl_hqos_get_mp_main_id(hqos_id_i, hqos_i);
                }
                mp_main = &vtss_state->hqos.info.table[mp_main_id];
                mp_main_i = &vtss_state->hqos.info.table[mp_main_id_i];

                /* Check that the ISDX will not end up twice on the same port */
                vtss_hqos_id_t hqos_id_j, hqos_id_k;
                for (hqos_id_j = 0; hqos_id_j < VTSS_HQOS_COUNT; hqos_id_j++) {
                    vtss_hqos_entry_t *hqos_j = &vtss_state->hqos.info.table[hqos_id_j];
                    if (hqos_j->enable && (hqos_id_j == mp_main_id || (hqos_j->is_mp && hqos_j->mp_main == mp_main_id))) {
                        for (hqos_id_k = 0; hqos_id_k < VTSS_HQOS_COUNT; hqos_id_k++) {
                            vtss_hqos_entry_t *hqos_k = &vtss_state->hqos.info.table[hqos_id_k];
                            if (hqos_k->enable && (hqos_id_k == mp_main_id_i || (hqos_k->is_mp && hqos_k->mp_main == mp_main_id_i)) &&
                                    hqos_j->conf.port_no == hqos_k->conf.port_no) {
                                VTSS_I("ISDX %u is already mapped to hqos_id %u (conflict on port %u)", isdx, hqos_id_k, hqos_k->conf.port_no);
                                return VTSS_RC_ERROR;
                            }
                        }
                    }
                }

                /* Check if this is the first HQoS entry on the port */
                if (vtss_state->hqos.port_conf[hqos->conf.port_no].sch_mode == VTSS_HQOS_SCH_MODE_HIERARCHICAL &&
                    vtss_state->hqos.port_info[hqos->conf.port_no].sch_mode != VTSS_HQOS_SCH_MODE_HIERARCHICAL) {
                    VTSS_RC(srvl_hqos_sch_mode_check(vtss_state, hqos->conf.port_no));
                    if (hqos->is_mp || hqos->isdx_list != NULL) {
                        VTSS_E("Unexpected mapped hqos_id %u on non-hierarchical port_no %u", hqos_id, hqos->conf.port_no);
                        return VTSS_RC_ERROR;
                    }
                } else {
                    secondary_id = mp_main_id_i;
                }

                VTSS_RC(srvl_hqos_find_mp_srv_grp(vtss_state, mp_main_id, secondary_id, &new_srv_grp));
                if (new_srv_grp == 0) {
                    VTSS_I("No available scheduling resources for hqos_id %u on port_no %u", hqos_id, hqos->conf.port_no);
                    return VTSS_RC_ERROR;
                } else if (!check_only) {
                    /* Assign the new main entry */
                    mp_main->srv_grp = new_srv_grp;
                    mp_main->is_mp = TRUE;
                    mp_main->mp_main = VTSS_HQOS_ID_NONE;

                    /* Point entries to new main entry */
                    mp_main_i->srv_grp = 0;
                    mp_main_i->is_mp = TRUE;
                    mp_main_i->mp_main = mp_main_id;
                    for (hqos_id_j = 0; hqos_id_j < VTSS_HQOS_COUNT; hqos_id_j++) {
                        vtss_hqos_entry_t *hqos_j = &vtss_state->hqos.info.table[hqos_id_j];
                        if (hqos_j->enable && hqos_j->is_mp && hqos_j->mp_main == mp_main_id) {
                            hqos_j->mp_main = mp_main_id;
                        }
                    }

                    /* Move existing ISDXs to new main entry */
                    while (mp_main_i->isdx_list != NULL) {
                        vtss_hqos_isdx_t *isdx_next = mp_main->isdx_list;
                        mp_main->isdx_list = mp_main_i->isdx_list;
                        mp_main_i->isdx_list = mp_main_i->isdx_list->next;
                        mp_main->isdx_list->next = isdx_next;
                    }

                    /* Map the new service group */
                    VTSS_RC(srvl_hqos_map_srv_grp(vtss_state, mp_main_id));

                    /* Set port mode if this is the first HQoS entry on the port */
                    if (vtss_state->hqos.port_conf[hqos->conf.port_no].sch_mode == VTSS_HQOS_SCH_MODE_HIERARCHICAL &&
                            vtss_state->hqos.port_info[hqos->conf.port_no].sch_mode != VTSS_HQOS_SCH_MODE_HIERARCHICAL) {
                        VTSS_RC(srvl_hqos_sch_mode_set(vtss_state, hqos->conf.port_no));
                    }

                    VTSS_FUNC_RC(qos.port_conf_update, hqos->conf.port_no);
                    VTSS_FUNC_RC(qos.port_conf_update, hqos_i->conf.port_no);
                }

                return VTSS_RC_OK;
            }
        }
    }

    /* Check if this is the first HQoS entry on the port */
    if (vtss_state->hqos.port_conf[hqos->conf.port_no].sch_mode == VTSS_HQOS_SCH_MODE_HIERARCHICAL &&
        vtss_state->hqos.port_info[hqos->conf.port_no].sch_mode != VTSS_HQOS_SCH_MODE_HIERARCHICAL) {
        VTSS_RC(srvl_hqos_sch_mode_check(vtss_state, hqos->conf.port_no));
        if (hqos->is_mp || hqos->isdx_list != NULL) {
            VTSS_E("Unexpected mapped hqos_id %u on non-hierarchical port_no %u", hqos_id, hqos->conf.port_no);
            return VTSS_RC_ERROR;
        }
        if (!check_only) {
            VTSS_RC(srvl_hqos_isdx_allocate(vtss_state, hqos_id, isdx));
            VTSS_RC(srvl_hqos_sch_mode_set(vtss_state, hqos->conf.port_no));
        }
        return VTSS_RC_OK;
    }

    /* Map the new ISDX */
    if (hqos->is_mp) {
        if (!check_only) {
            vtss_hqos_id_t mp_main = hqos_id;
            if (hqos->mp_main != VTSS_HQOS_ID_NONE) {
                mp_main = hqos->mp_main;
            }
            VTSS_RC(srvl_hqos_isdx_allocate(vtss_state, mp_main, isdx));
        }
    } else {
        if (hqos->isdx_list == NULL &&
            vtss_state->hqos.port_info[hqos->conf.port_no].sch_mode == VTSS_HQOS_SCH_MODE_HIERARCHICAL) {
            u16  new_srv_grp;
            BOOL need_expansion;

            VTSS_RC(srvl_hqos_find_p2p_srv_grp(vtss_state, hqos->conf.port_no, &new_srv_grp, &need_expansion));
            if (new_srv_grp == 0) {
                VTSS_I("No available scheduling resources for hqos_id %u on port_no %u", hqos_id, hqos->conf.port_no);
                return VTSS_RC_ERROR;
            } else if (!check_only) {
                if (need_expansion) {
                    VTSS_RC(srvl_hqos_expand_nni_port(vtss_state, hqos->conf.port_no));
                }
                hqos->srv_grp = new_srv_grp;
                VTSS_RC(srvl_hqos_isdx_allocate(vtss_state, hqos_id, isdx));

                VTSS_FUNC_RC(qos.port_conf_update, hqos->conf.port_no);
            }
        } else if (!check_only) {
            VTSS_RC(srvl_hqos_isdx_allocate(vtss_state, hqos_id, isdx));
        }
    }
    return VTSS_RC_OK;
}

static vtss_rc srvl_hqos_isdx_del(struct vtss_state_s *vtss_state, const vtss_hqos_id_t hqos_id, u16 isdx)
{
    vtss_hqos_entry_t *hqos = &vtss_state->hqos.info.table[hqos_id];

    VTSS_D("hqos_id: %u, isdx: %u", hqos_id, isdx);

    if (!hqos->enable) {
        VTSS_I("hqos_id %u does not exist", hqos_id);
        return VTSS_RC_ERROR;
    }
    if (hqos->is_mp) {
        vtss_hqos_id_t mp_main_id = hqos_id;
        vtss_hqos_entry_t *mp_main = hqos;
        if (hqos->mp_main != VTSS_HQOS_ID_NONE) {
            mp_main_id = hqos->mp_main;
            mp_main = &vtss_state->hqos.info.table[mp_main_id];
        }
        VTSS_RC(srvl_hqos_isdx_free(vtss_state, mp_main_id, isdx));
        if (mp_main->isdx_list == NULL) {
            vtss_hqos_id_t hqos_id_i;
            for (hqos_id_i = 0; hqos_id_i < VTSS_HQOS_COUNT; hqos_id_i++) {
                vtss_hqos_entry_t *hqos_i = &vtss_state->hqos.info.table[hqos_id_i];
                if (hqos_i->enable && hqos_i->is_mp && hqos_i->mp_main == mp_main_id) {
                    hqos_i->srv_grp = 0;
                    hqos_i->is_mp = FALSE;
                    hqos_i->mp_main = VTSS_HQOS_ID_NONE;
                    hqos_i->min_rate_calc = 0;
                    VTSS_FUNC_RC(qos.port_conf_update, hqos_i->conf.port_no);
                }
            }
            mp_main->srv_grp = 0;
            mp_main->is_mp = FALSE;
            mp_main->min_rate_calc = 0;
            VTSS_FUNC_RC(qos.port_conf_update, hqos->conf.port_no);
        }
    } else {
        VTSS_RC(srvl_hqos_isdx_free(vtss_state, hqos_id, isdx));
        if (hqos->isdx_list == NULL) {
            hqos->srv_grp = 0;
            hqos->min_rate_calc = 0;
            VTSS_FUNC_RC(qos.port_conf_update, hqos->conf.port_no);
        }
    }
    return VTSS_RC_OK;
}

static vtss_rc srvl_hqos_update(struct vtss_state_s *vtss_state, const vtss_port_no_t port_no)
{
    vtss_hqos_port_info_t *info = &vtss_state->hqos.port_info[port_no];
    vtss_shaper_t         *port_shaper = &vtss_state->qos.port_conf[port_no].shaper_port;
    vtss_hqos_id_t        hqos_id;
    u32                   port = VTSS_CHIP_PORT(port_no);
    u8                    se, queue, srv_grp;
    u8                    lvl2_se = VTSS_HQOS_SES - port_no - 1;
    vtss_shaper_t         default_shaper;
    u8                    lvl0_cost[VTSS_QUEUE_ARRAY_SIZE];
    vtss_bitrate_t        lvl0_rate[VTSS_HQOS_SE_INPUTS * VTSS_HQOS_SE_INPUTS];
    vtss_bitrate_t        lvl1_rate_sum[VTSS_HQOS_SE_INPUTS];
    vtss_bitrate_t        lvl1_rate_max[VTSS_HQOS_SE_INPUTS];
    vtss_bitrate_t        lvl2_rate_sum = 0;
    vtss_bitrate_t        lvl2_rate_max = 0;
    vtss_bitrate_t        port_speed;
    vtss_bitrate_t        rest_rate = 0;
    BOOL                  hqos_map[VTSS_HQOS_SE_INPUTS * VTSS_HQOS_SE_INPUTS];
    u8                    lvl1_cost[VTSS_HQOS_SE_INPUTS * VTSS_HQOS_SE_INPUTS];
    u64                   lvl1_cost_sum[VTSS_HQOS_SE_INPUTS]; /* Q40.24 fixed point */
    u8                    lvl2_cost[VTSS_HQOS_SE_INPUTS];
    u64                   lvl2_cost_sum = 0;                  /* Q40.24 fixed point */
    u32                   hqos_cnt = 0, dwrr_hqos_cnt = 0;

    switch(vtss_state->port.conf[port_no].speed) {
    case VTSS_SPEED_10M:
        port_speed = 10000;
        break;
    case VTSS_SPEED_100M:
        port_speed = 100000;
        break;
    case VTSS_SPEED_2500M:
        port_speed = 2500000;
        break;
    default: /* 1G */
        port_speed = 1000000;
        break;
    }

    if (port_shaper->rate != VTSS_BITRATE_DISABLED && port_shaper->rate < port_speed) {
        port_speed = port_shaper->rate;
    }

    if (info->sch_mode != VTSS_HQOS_SCH_MODE_HIERARCHICAL) {
        info->min_rate_calc = port_speed;
        return VTSS_RC_OK;
    }

    VTSS_D("port_no: %u", port_no);

    memset(lvl0_rate, 0, sizeof(lvl0_rate));
    memset(lvl1_rate_sum, 0, sizeof(lvl1_rate_sum));
    memset(lvl1_rate_max, 0, sizeof(lvl1_rate_max));
    memset(hqos_map, 0, sizeof(hqos_map));
    memset(lvl1_cost, 0, sizeof(lvl1_cost));
    memset(lvl1_cost_sum, 0, sizeof(lvl1_cost_sum));
    memset(lvl2_cost, 0, sizeof(lvl2_cost));

    /* Setup all service SEs in default configuration */
    default_shaper.rate = VTSS_BITRATE_DISABLED;
    default_shaper.eir = VTSS_BITRATE_DISABLED;
    default_shaper.mode = VTSS_SHAPER_MODE_LINE;
    for (se = info->se_base + 1; se <= info->se_top; se++) {

        /* DWRR configuration */
        SRVL_WRM(VTSS_QSYS_HSCH_SE_CFG(se),
                 VTSS_F_QSYS_HSCH_SE_CFG_SE_DWRR_CNT(0), /* Default all strict */
                 VTSS_M_QSYS_HSCH_SE_CFG_SE_DWRR_CNT |
                 VTSS_F_QSYS_HSCH_SE_CFG_SE_RR_ENA);

        /* HQoS shaper configuration */
        VTSS_RC(vtss_srvl_qos_shaper_conf_set(vtss_state, &default_shaper, se, TRUE, port, 0, NULL));

        /* Egress queue shaper configuration */
        for (queue = 6; queue < 8; queue++) {
            u32 queue_shaper_se = VTSS_HQOS_SES + 2 * se + queue - 6;
            VTSS_RC(vtss_srvl_qos_shaper_conf_set(vtss_state, &default_shaper, queue_shaper_se, FALSE, port, queue, NULL));
        }
    }

    /* Setup configured SEs */
    hqos_map[0] = TRUE;
    for (hqos_id = 0; hqos_id < VTSS_HQOS_COUNT; hqos_id++) {
        vtss_hqos_entry_t *hqos = &vtss_state->hqos.info.table[hqos_id];
        if (hqos->enable && hqos->conf.port_no == port_no && (hqos->isdx_list != NULL || srvl_hqos_has_mp_main(hqos))) {
            if (srvl_hqos_has_mp_main(hqos)) {
                srv_grp = vtss_state->hqos.info.table[hqos->mp_main].srv_grp;
            } else {
                srv_grp = hqos->srv_grp;
            }
            if (srv_grp == 0) {
                continue;
            }
            hqos_map[srv_grp] = TRUE;

            se = info->se_base + srv_grp;

            /* DWRR configuration */
            SRVL_WRM(VTSS_QSYS_HSCH_SE_CFG(se),
                    VTSS_F_QSYS_HSCH_SE_CFG_SE_DWRR_CNT(hqos->conf.dwrr_cnt),
                    VTSS_M_QSYS_HSCH_SE_CFG_SE_DWRR_CNT |
                    VTSS_F_QSYS_HSCH_SE_CFG_SE_RR_ENA);

            VTSS_RC(vtss_cmn_qos_weight2cost(hqos->conf.queue_pct, lvl0_cost, hqos->conf.dwrr_cnt, VTSS_QOS_DWRR_COST_BIT_WIDTH));
            for (queue = 0; queue < hqos->conf.dwrr_cnt; queue++) {
                SRVL_WR(VTSS_QSYS_HSCH_SE_DWRR_CFG(se, queue), lvl0_cost[queue]);
            }

            /* HQoS shaper configuration */
            VTSS_RC(vtss_srvl_qos_shaper_conf_set(vtss_state, &hqos->conf.shaper, se, TRUE, port, 0, NULL));

            /* Egress queue shaper configuration */
            for (queue = 6; queue < 8; queue++) {
                u32 queue_shaper_se = VTSS_HQOS_SES + 2 * se + queue - 6;
                VTSS_RC(vtss_srvl_qos_shaper_conf_set(vtss_state, &hqos->conf.shaper_queue[queue], queue_shaper_se, FALSE, port, queue, NULL));
            }

            if (hqos->conf.min_rate > 0) {
                u8 lvl1_idx = srv_grp / VTSS_HQOS_SE_INPUTS;
                lvl0_rate[srv_grp] = hqos->conf.min_rate;
                lvl1_rate_sum[lvl1_idx] += hqos->conf.min_rate;
                if (hqos->conf.min_rate > lvl1_rate_max[lvl1_idx]) {
                    lvl1_rate_max[lvl1_idx] = hqos->conf.min_rate;
                }
                lvl2_rate_sum += hqos->conf.min_rate;
                dwrr_hqos_cnt++;
            }
        }
    }

    /* Split remaining bandwidth for unconfigured EVCs and non-service traffic */
    for (se = info->se_base; se <= info->se_top; se++) {
        u8  lvl0_idx = se - info->se_base;
        if (hqos_map[lvl0_idx]) {
            hqos_cnt++;
        }
    }
    if (lvl2_rate_sum < port_speed) {
        u32 rest_hqos = hqos_cnt - dwrr_hqos_cnt;
        if (rest_hqos > 0) {
            rest_rate = (port_speed - lvl2_rate_sum) / rest_hqos;
            for (se = info->se_base; se <= info->se_top; se++) {
                u8  lvl0_idx = se - info->se_base;
                if (hqos_map[lvl0_idx] && lvl0_rate[lvl0_idx] == 0) {
                    u8 lvl1_idx = lvl0_idx / VTSS_HQOS_SE_INPUTS;
                    lvl0_rate[lvl0_idx] = rest_rate;
                    lvl1_rate_sum[lvl1_idx] += rest_rate;
                    if (rest_rate > lvl1_rate_max[lvl1_idx]) {
                        lvl1_rate_max[lvl1_idx] = rest_rate;
                    }
                    lvl2_rate_sum += rest_rate;
                }
            }
        }
    }

    /* Configure DWRR for level 1 */
    for (se = info->se_base; se <= info->se_top; se++) {
        u8  lvl0_idx = se - info->se_base;
        u8  lvl1_idx = lvl0_idx / VTSS_HQOS_SE_INPUTS;
        u8  lvl1_inp = lvl0_idx % VTSS_HQOS_SE_INPUTS;
        u8  lvl1_se = info->lvl1_se_base + lvl1_idx;

        /* Enable DWRR */
        SRVL_WRM(VTSS_QSYS_HSCH_SE_CFG(lvl1_se),
                 VTSS_F_QSYS_HSCH_SE_CFG_SE_DWRR_CNT(VTSS_HQOS_SE_INPUTS),
                 VTSS_M_QSYS_HSCH_SE_CFG_SE_DWRR_CNT |
                 VTSS_F_QSYS_HSCH_SE_CFG_SE_RR_ENA);

        if (lvl0_rate[lvl0_idx] > 0) {
            lvl1_cost[lvl0_idx] = srvl_hqos_cost_calc(lvl0_rate[lvl0_idx], lvl1_rate_max[lvl1_idx]);
        } else {
            lvl1_cost[lvl0_idx] = 1 << VTSS_QOS_DWRR_COST_BIT_WIDTH;
        }
        if (hqos_map[lvl0_idx]) {
            lvl1_cost_sum[lvl1_idx] += VTSS_DIV64(((u64)1 << 24), lvl1_cost[lvl0_idx]);
        }
        SRVL_WR(VTSS_QSYS_HSCH_SE_DWRR_CFG(lvl1_se, lvl1_inp), lvl1_cost[lvl0_idx] - 1);
    }

    /* Configure DWRR for level 2 */
    for (se = 0; se < VTSS_HQOS_SE_INPUTS; se++) {
        if (lvl1_rate_sum[se] > lvl2_rate_max) {
            lvl2_rate_max = lvl1_rate_sum[se];
        }
    }
    for (se = info->lvl1_se_base; se <= info->lvl1_se_top; se++) {
        u8 lvl1_idx = se - info->lvl1_se_base;

        /* Enable DWRR */
        SRVL_WRM(VTSS_QSYS_HSCH_SE_CFG(lvl2_se),
                 VTSS_F_QSYS_HSCH_SE_CFG_SE_DWRR_CNT(VTSS_HQOS_SE_INPUTS),
                 VTSS_M_QSYS_HSCH_SE_CFG_SE_DWRR_CNT |
                 VTSS_F_QSYS_HSCH_SE_CFG_SE_RR_ENA);

        if (lvl1_rate_sum[lvl1_idx] > 0) {
            lvl2_cost[lvl1_idx] = srvl_hqos_cost_calc(lvl1_rate_sum[lvl1_idx], lvl2_rate_max);
        } else {
            lvl2_cost[lvl1_idx] = 1 << VTSS_QOS_DWRR_COST_BIT_WIDTH;
        }
        if (lvl1_cost_sum[lvl1_idx] > 0) {
            lvl2_cost_sum += VTSS_DIV64(((u64)1 << 24), lvl2_cost[lvl1_idx]);
        }
        SRVL_WR(VTSS_QSYS_HSCH_SE_DWRR_CFG(lvl2_se, lvl1_idx), lvl2_cost[lvl1_idx] - 1);
    }

    /* Calculate real minimum bandwidths */
    for (hqos_id = 0; hqos_id < VTSS_HQOS_COUNT; hqos_id++) {
        vtss_hqos_entry_t *hqos = &vtss_state->hqos.info.table[hqos_id];
        if (hqos->enable && hqos->conf.port_no == port_no && (hqos->isdx_list != NULL || srvl_hqos_has_mp_main(hqos))) {
            u8 lvl1_idx;

            if (srvl_hqos_has_mp_main(hqos)) {
                srv_grp = vtss_state->hqos.info.table[hqos->mp_main].srv_grp;
            } else {
                srv_grp = hqos->srv_grp;
            }
            if (srv_grp == 0) {
                continue;
            }
            lvl1_idx = srv_grp / VTSS_HQOS_SE_INPUTS;

            if (lvl2_cost[lvl1_idx] != 0 && lvl2_cost_sum != 0 && lvl1_cost[srv_grp] != 0 && lvl1_cost_sum[lvl1_idx] != 0) {
                u64 cost_factor = (u64)lvl2_cost[lvl1_idx] * (u64)lvl1_cost[srv_grp] * ((lvl2_cost_sum * lvl1_cost_sum[lvl1_idx]) >> 24); /* Q40.24 fixed point */
                hqos->min_rate_calc = (vtss_bitrate_t)VTSS_DIV64((u64)port_speed << 24, cost_factor);
            } else {
                hqos->min_rate_calc = 0;
            }
        }
    }
    if (lvl2_cost[0] != 0 && lvl2_cost_sum != 0 && lvl1_cost[0] != 0 && lvl1_cost_sum[0] != 0) {
        u64 cost_factor = (u64)lvl2_cost[0] * (u64)lvl1_cost[0] * ((lvl2_cost_sum * lvl1_cost_sum[0]) >> 24); /* Q40.24 fixed point */
        info->min_rate_calc = (vtss_bitrate_t)VTSS_DIV64((u64)port_speed << 24, cost_factor);
    } else {
        info->min_rate_calc = 0;
    }

    return VTSS_RC_OK;
}

static vtss_rc srvl_hqos_min_rate_calc(struct vtss_state_s *vtss_state, const vtss_hqos_id_t hqos_id, vtss_bitrate_t *const min_rate_calc)
{
    (*min_rate_calc) = vtss_state->hqos.info.table[hqos_id].min_rate_calc;
    return VTSS_RC_OK;
}

static vtss_rc srvl_hqos_port_min_rate_calc(struct vtss_state_s *vtss_state, const vtss_port_no_t port_no, vtss_bitrate_t *const min_rate_calc)
{
    (*min_rate_calc) = vtss_state->hqos.port_info[port_no].min_rate_calc;
    return VTSS_RC_OK;
}

/* - Debug print --------------------------------------------------- */

static vtss_rc srvl_debug_hqos(vtss_state_t *vtss_state, const vtss_debug_printf_t pr, const vtss_debug_info_t *const info)
{
    vtss_port_no_t port_no;
    u8             se;
    u32            lvl1_cost, lvl2_cost;

    /* DWRR debug information */
    for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {
        vtss_hqos_port_info_t *port_info = &vtss_state->hqos.port_info[port_no];
        if (port_info->sch_mode == VTSS_HQOS_SCH_MODE_HIERARCHICAL) {
            u8 lvl2_se = VTSS_HQOS_SES - port_no - 1;
            pr("DWRR costs for port %u (chip_port: %u)\n", port_no, VTSS_CHIP_PORT(port_no));
            for (se = port_info->se_base; se <= port_info->se_top; se++) {
                u8 lvl0_idx = se - port_info->se_base;
                u8 lvl1_idx = lvl0_idx / VTSS_HQOS_SE_INPUTS;
                u8 lvl1_inp = lvl0_idx % VTSS_HQOS_SE_INPUTS;
                u8 lvl1_se = port_info->lvl1_se_base + lvl1_idx;

                SRVL_RD(VTSS_QSYS_HSCH_SE_DWRR_CFG(lvl1_se, lvl1_inp), &lvl1_cost);
                pr("%3u: %2u", lvl0_idx, lvl1_cost + 1);

                if (lvl1_inp == 0) {
                    SRVL_RD(VTSS_QSYS_HSCH_SE_DWRR_CFG(lvl2_se, lvl1_idx), &lvl2_cost);
                    pr(" - %2u", lvl2_cost + 1);
                }
                if (lvl1_inp == VTSS_HQOS_SE_INPUTS - 1) {
                    pr("\n");
                }
                pr("\n");
            }
            pr("\n");
        } else {
            pr("nothing on port %u\n", port_no);
        }
    }

    return VTSS_RC_OK;
}

vtss_rc vtss_srvl_hqos_debug_print(vtss_state_t *vtss_state, const vtss_debug_printf_t pr, const vtss_debug_info_t *const info)
{
    return vtss_debug_print_group(VTSS_DEBUG_GROUP_HQOS, srvl_debug_hqos, vtss_state, pr, info);
}

/* - Initialization ------------------------------------------------ */

vtss_rc vtss_srvl_hqos_init(vtss_state_t *vtss_state, vtss_init_cmd_t cmd)
{
    vtss_hqos_state_t *state = &vtss_state->hqos;

    switch (cmd) {
    case VTSS_INIT_CMD_CREATE:
        state->port_conf_set = srvl_hqos_port_conf_set;
        state->add = srvl_hqos_add;
        state->del = srvl_hqos_del;
        state->map = srvl_hqos_map;
        state->isdx_add = srvl_hqos_isdx_add;
        state->isdx_del = srvl_hqos_isdx_del;
        state->update = srvl_hqos_update;
        state->min_rate_calc = srvl_hqos_min_rate_calc;
        state->port_min_rate_calc = srvl_hqos_port_min_rate_calc;
        state->info.max_count = VTSS_HQOS_COUNT;
        break;
    case VTSS_INIT_CMD_INIT:
        VTSS_RC(srvl_hqos_map_cpu_ports(vtss_state));
        break;
    case VTSS_INIT_CMD_POLL:
        break;
    case VTSS_INIT_CMD_PORT_MAP:
        VTSS_RC(srvl_hqos_reallocate(vtss_state));
        break;
    default:
        break;
    }
    return VTSS_RC_OK;
}
#endif /* VTSS_FEATURE_HQOS */
#endif /* VTSS_ARCH_SERVAL */
