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
#include "vtss_api.h"
#include "vtss_state.h"
#include "vtss_common.h"

#if defined(VTSS_FEATURE_HQOS)

static vtss_rc vtss_hqos_id_check(vtss_state_t         *vtss_state,
                                  const vtss_hqos_id_t hqos_id)
{
    vtss_rc rc = VTSS_RC_OK;

    if (hqos_id >= VTSS_HQOS_COUNT || !vtss_state->hqos.info.table[hqos_id].enable) {
        VTSS_E("hqos_id %u does not exist", hqos_id);
        rc = VTSS_RC_ERROR;
    }
    return rc;
}

/* - API functions ------------------------------------------------- */

vtss_rc vtss_hqos_port_conf_get(const vtss_inst_t inst, const vtss_port_no_t port_no, vtss_hqos_port_conf_t *const conf)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK) {
        *conf = vtss_state->hqos.port_conf[port_no];
    }
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_hqos_port_conf_set(const vtss_inst_t inst, const vtss_port_no_t port_no, const vtss_hqos_port_conf_t *const conf)
{
    vtss_state_t          *vtss_state;
    vtss_rc               rc;
    vtss_hqos_port_conf_t *port_conf;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK) {
        port_conf = &vtss_state->hqos.port_conf[port_no];
        vtss_state->hqos.port_conf_old = *port_conf;
        *port_conf = *conf;
        if ((rc = VTSS_FUNC_COLD(hqos.port_conf_set, port_no)) != VTSS_RC_OK) {
            /* Restore configuration if operation failed */
            *port_conf = vtss_state->hqos.port_conf_old;
        }
    }
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_hqos_add(const vtss_inst_t inst, const vtss_hqos_id_t hqos_id, const vtss_hqos_conf_t *const conf)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("hqos_id: %u", hqos_id);
    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK) {
        if (hqos_id >= VTSS_HQOS_COUNT) {
            VTSS_E("Illegal hqos_id: %u", hqos_id);
            rc = VTSS_RC_ERROR;
        } else {
            rc = VTSS_FUNC(hqos.add, hqos_id, conf);
        }
    }
    VTSS_EXIT();

    return rc;
}

vtss_rc vtss_hqos_del(const vtss_inst_t inst, const vtss_hqos_id_t hqos_id)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("hqos_id: %u", hqos_id);
    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK &&
        (rc = vtss_hqos_id_check(vtss_state, hqos_id)) == VTSS_RC_OK) {
        rc = VTSS_FUNC(hqos.del, hqos_id);
    }
    VTSS_EXIT();

    return rc;
}

vtss_rc vtss_hqos_get(const vtss_inst_t inst, const vtss_hqos_id_t hqos_id, vtss_hqos_conf_t *const conf)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("hqos_id: %u", hqos_id);
    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK &&
        (rc = vtss_hqos_id_check(vtss_state, hqos_id)) == VTSS_RC_OK) {
        (*conf) = vtss_state->hqos.info.table[hqos_id].conf;
    }
    VTSS_EXIT();

    return rc;
}

vtss_rc vtss_hqos_min_rate_calc(const vtss_inst_t inst, const vtss_hqos_id_t hqos_id, vtss_bitrate_t *const min_rate_calc)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("hqos_id: %u", hqos_id);
    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK &&
        (rc = vtss_hqos_id_check(vtss_state, hqos_id)) == VTSS_RC_OK) {
        rc = VTSS_FUNC(hqos.min_rate_calc, hqos_id, min_rate_calc);
    }
    VTSS_EXIT();

    return rc;
}

vtss_rc vtss_hqos_port_min_rate_calc(const vtss_inst_t inst, const vtss_port_no_t port_no, vtss_bitrate_t *const min_rate_calc)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK) {
        rc = VTSS_FUNC(hqos.port_min_rate_calc, port_no, min_rate_calc);
    }
    VTSS_EXIT();

    return rc;
}

/* - Warm start synchronization ------------------------------------ */

#if defined(VTSS_FEATURE_WARM_START)
vtss_rc vtss_hqos_restart_sync(vtss_state_t *vtss_state)
{
    vtss_port_no_t port_no;

    for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {
        vtss_state->hqos.port_conf_old = vtss_state->hqos.port_conf[port_no];
        VTSS_FUNC_RC(hqos.port_conf_set, port_no);
    }

    return VTSS_RC_OK;
}
#endif /* VTSS_FEATURE_WARM_START */

/* - Instance create and initialization ---------------------------- */

vtss_rc vtss_hqos_inst_create(vtss_state_t *vtss_state)
{
    vtss_hqos_isdx_list_t *isdx_table = &vtss_state->hqos.isdx_table;
    vtss_hqos_isdx_t      *isdx;
    u32                   i;

    /* Add ISDX entries to free list */
    for (i = 0; i < VTSS_SDX_CNT; i++) {
        isdx = &isdx_table->table[i];
        isdx->next = isdx_table->free;
        isdx_table->free = isdx;
    }

    return VTSS_RC_OK;
}

/* - Debug print --------------------------------------------------- */

void vtss_hqos_debug_print(vtss_state_t *vtss_state,
                          const vtss_debug_printf_t pr,
                          const vtss_debug_info_t   *const info)
{
    u32                    i;
    vtss_port_no_t         port_no;
    vtss_hqos_port_info_t  *port_info;
    vtss_hqos_port_conf_t  *port_conf;
    vtss_hqos_entry_t      *hqos;
    vtss_hqos_info_t       *hqos_info = &vtss_state->hqos.info;
    BOOL                   header = 1;

    if (vtss_debug_group_enabled(pr, info, VTSS_DEBUG_GROUP_HQOS)) {
        /* Port table */
        for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {
            if (!info->port_list[port_no]) {
                continue;
            }
            if (header) {
                header = 0;
                vtss_debug_print_header(pr, "Ports");
                pr("Port  Scheduling mode  Expandable  SE0 Base  SE0 Top  SE1 Base  SE1 Top\n");
            }
            port_info = &vtss_state->hqos.port_info[port_no];
            port_conf = &vtss_state->hqos.port_conf[port_no];
            pr("%-6u%-17s", port_no, port_conf->sch_mode == VTSS_HQOS_SCH_MODE_NORMAL       ? "Normal" :
                                     port_conf->sch_mode == VTSS_HQOS_SCH_MODE_BASIC        ? "Basic" :
                                     port_conf->sch_mode == VTSS_HQOS_SCH_MODE_HIERARCHICAL ? "Hierarchical" :
                                                                                              "?");
            pr("%-12s%-10u%-9u%-10u%u\n", vtss_bool_txt(port_info->nni_mode),
                    port_info->se_base, port_info->se_top,
                    port_info->lvl1_se_base, port_info->lvl1_se_top);
        }
        if (!header) {
            pr("\n");
        }
        header = 1;

        /* HQoS table */
        vtss_debug_print_header(pr, "HQoS Entries");
        pr("state size: %zu\n", sizeof(*info));
        pr("entry size: %zu\n", sizeof(*hqos));
        pr("max_count : %u\n", hqos_info->max_count);
        pr("count     : %u\n\n", hqos_info->count);
        for (i = 0; i < VTSS_HQOS_COUNT; i++) {
            hqos = &hqos_info->table[i];
            if (hqos->enable == 0) {
                continue;
            }
            if (header) {
                header = 0;
                pr("HQoS ID  Port  Service Group  Multi-Point  Main MP ID\n");
            }
            pr("%-9u%-6u%-15u%-13s",
                    i, hqos->conf.port_no, hqos->srv_grp, vtss_bool_txt(hqos->is_mp));
            if (hqos->is_mp && hqos->mp_main != VTSS_HQOS_ID_NONE) {
                pr("%u\n", hqos->mp_main);
            } else {
                pr("-\n");
            }
        }
        if (!header) {
            pr("\n");
        }
        header = 1;

        /* HQoS mappings */
        for (i = 0; i < VTSS_HQOS_COUNT; i++) {
            vtss_hqos_isdx_t *isdx;
            int              j = 0;
            hqos = &hqos_info->table[i];
            if (hqos->enable == 0 || hqos->isdx_list == NULL) {
                continue;
            }
            if (hqos->isdx_list == NULL) {
                continue;
            }
            if (header) {
                header = 0;
                pr("HQoS ID  ISDXs\n");
            }
            pr("%-9u", i);
            for (isdx = hqos->isdx_list; isdx != NULL; isdx = isdx->next) {
                pr("%u", isdx->isdx);
                if (isdx->next != NULL) {
                    if (++j >= 5) {
                        pr(",\n         ");
                        j = 0;
                    } else {
                        pr(", ");
                    }
                }
            }
            pr("\n");
        }
        if (!header) {
            pr("\n");
        }
        header = 1;

        /* Minimum bandwidth */
        for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {
            if (!info->port_list[port_no])
                continue;
            port_info = &vtss_state->hqos.port_info[port_no];
            if (port_info->sch_mode != VTSS_HQOS_SCH_MODE_HIERARCHICAL) {
                continue;
            }
            if (header) {
                header = 0;
                pr("Port  HQoS ID      Calculated Minimum Bandwidth (kbps)\n");
            }
            for (i = 0; i < VTSS_HQOS_COUNT; i++) {
                hqos = &hqos_info->table[i];
                if (hqos->enable == 0 || hqos->conf.port_no != port_no) {
                    continue;
                }
                pr("%-6u%-13u%u\n", port_no, i, hqos->min_rate_calc);
            }
            pr("%-6uNon-service  %u\n", port_no, port_info->min_rate_calc);
        }
        if (!header)
            pr("\n");
    }
}
#endif /* VTSS_FEATURE_HQOS */
