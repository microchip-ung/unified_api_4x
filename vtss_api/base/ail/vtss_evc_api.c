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

#define VTSS_TRACE_GROUP VTSS_TRACE_GROUP_EVC
#include "vtss_api.h"
#include "vtss_state.h"
#include "vtss_common.h"

#if defined(VTSS_FEATURE_EVC)
#if defined(VTSS_ARCH_CARACAL) || defined(VTSS_ARCH_SERVAL)
static const char *vtss_vcap_bit_txt(vtss_vcap_bit_t value)
{
    return (value == VTSS_VCAP_BIT_0 ? "0" : value == VTSS_VCAP_BIT_1 ? "1" : "X");
}
#endif /* VTSS_ARCH_CARACAL/SERVAL */

vtss_rc vtss_evc_port_conf_get(const vtss_inst_t    inst,
                               const vtss_port_no_t port_no,
                               vtss_evc_port_conf_t *const conf)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK)
        *conf = vtss_state->evc.port_conf[port_no];
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_evc_port_conf_set(const vtss_inst_t          inst,
                               const vtss_port_no_t       port_no,
                               const vtss_evc_port_conf_t *const conf)
{
    vtss_state_t          *vtss_state;
    vtss_rc              rc;
    vtss_evc_port_conf_t *port_conf;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK) {
        port_conf = &vtss_state->evc.port_conf[port_no];
        vtss_state->evc.port_conf_old = *port_conf;
        *port_conf = *conf;
        vtss_state->evc.port_conf_update = TRUE;
        if ((rc = VTSS_FUNC_COLD(evc.port_conf_set, port_no)) != VTSS_RC_OK) {
            /* Restore configuration if operation failed */
            *port_conf = vtss_state->evc.port_conf_old;
        }
        vtss_state->evc.port_conf_update = FALSE;
    }
    VTSS_EXIT();
    return rc;
}

static vtss_rc vtss_inst_evc_id_check(const vtss_inst_t   inst,
                                      vtss_state_t        **vtss_state,
                                      const vtss_evc_id_t evc_id,
                                      BOOL                allow_none,
                                      BOOL                check_enabled)
{
    vtss_rc rc;

    if ((rc = vtss_inst_check(inst, vtss_state)) == VTSS_RC_OK) {
        if (allow_none && evc_id == VTSS_EVC_ID_NONE) {
        } else if (evc_id >= VTSS_EVCS) {
            VTSS_E("illegal evc_id: %u", evc_id);
            rc = VTSS_RC_ERROR;
        } else if (check_enabled && !(*vtss_state)->evc.info.table[evc_id].enable) {
            VTSS_E("evc_id %u is disabled", evc_id);
            rc = VTSS_RC_ERROR;
        }
    }
    return rc;
}

vtss_rc vtss_evc_add(const vtss_inst_t     inst,
                     const vtss_evc_id_t   evc_id,
                     const vtss_evc_conf_t *const conf)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("evc_id: %u", evc_id);
#if defined(VTSS_ARCH_JAGUAR_1)
    VTSS_RC(vtss_inst_check(inst, &vtss_state));
    VTSS_RC(vtss_evc_policer_id_check(vtss_state, conf->policer_id, 1));
#endif /* VTSS_ARCH_JAGUAR_1 */

    VTSS_ENTER();
    if ((rc = vtss_inst_evc_id_check(inst, &vtss_state, evc_id, 0, 0)) == VTSS_RC_OK)
        rc = VTSS_FUNC(evc.add, evc_id, conf);
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_evc_del(const vtss_inst_t   inst,
                     const vtss_evc_id_t evc_id)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("evc_id: %u", evc_id);
    VTSS_ENTER();
    if ((rc = vtss_inst_evc_id_check(inst, &vtss_state, evc_id, 0, 1)) == VTSS_RC_OK)
        rc = VTSS_FUNC(evc.del, evc_id);
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_evc_get(const vtss_inst_t   inst,
                     const vtss_evc_id_t evc_id,
                     vtss_evc_conf_t     *const conf)
{
    vtss_state_t            *vtss_state;
    vtss_rc                 rc;

    VTSS_D("evc_id: %u", evc_id);
    VTSS_ENTER();
    if ((rc = vtss_inst_evc_id_check(inst, &vtss_state, evc_id, 0, 1)) == VTSS_RC_OK) {
        rc = vtss_cmn_evc_conf_get(vtss_state, evc_id, conf);
    }
    VTSS_EXIT();
    return rc;
}

#if defined(VTSS_FEATURE_HQOS)

static vtss_rc vtss_evc_hqos_port_is_mapped(vtss_state_t *vtss_state, vtss_evc_id_t evc_id, vtss_port_no_t port_no, BOOL *is_mapped)
{
    vtss_evc_entry_t *evc = &vtss_state->evc.info.table[evc_id];
    BOOL             port_found = VTSS_PORT_BF_GET(evc->ports, port_no);
    if (!port_found) {
        vtss_ece_entry_t *ece;
        for (ece = vtss_state->evc.ece_info.used; ece != NULL; ece = ece->next) {
            if (ece->evc_id == evc_id && VTSS_PORT_BF_GET(ece->ports, port_no)) {
                port_found = TRUE;
                break;
            }
        }
    }

    *is_mapped = port_found && evc->hqos_id[port_no] != VTSS_HQOS_ID_NONE;

    return VTSS_RC_OK;
}

static vtss_rc vtss_evc_hqos_id_check(vtss_state_t *vtss_state, vtss_hqos_id_t hqos_id, vtss_port_no_t port_no)
{
    if (hqos_id == VTSS_HQOS_ID_NONE || (hqos_id < VTSS_HQOS_COUNT && vtss_state->hqos.info.table[hqos_id].enable)) {
        if (hqos_id != VTSS_HQOS_ID_NONE) {
            if (vtss_state->hqos.info.table[hqos_id].conf.port_no != port_no) {
                VTSS_E("hqos_id %u is not on port_no %u", hqos_id, port_no);
                return VTSS_RC_ERROR;
            }
        }
        return VTSS_RC_OK;
    } else {
        VTSS_E("illegal hqos_id: %u", hqos_id);
        return VTSS_RC_ERROR;
    }
}

vtss_rc vtss_evc_hqos_map_get(const vtss_inst_t inst, const vtss_evc_id_t evc_id, const vtss_port_no_t port_no, vtss_hqos_id_t *const hqos_id)
{
    vtss_state_t   *vtss_state;
    vtss_rc        rc;

    VTSS_D("evc_id: %u, port_no: %u", evc_id, port_no);

    *hqos_id = VTSS_HQOS_ID_NONE;

    VTSS_ENTER();

    if ((rc = vtss_inst_evc_id_check(inst, &vtss_state, evc_id, 0, 1)) == VTSS_RC_OK &&
        (rc = vtss_port_no_check(vtss_state, port_no)) == VTSS_RC_OK) {
        *hqos_id = vtss_state->evc.info.table[evc_id].hqos_id[port_no];
    }

    VTSS_EXIT();

    return rc;
}

static vtss_rc vtss_evc_hqos_demap_evc_port(vtss_state_t *vtss_state, vtss_evc_id_t evc_id, vtss_port_no_t port_no)
{
    vtss_evc_entry_t *evc = vtss_cmn_evc_get(vtss_state, evc_id);
    vtss_ece_entry_t *ece;
    vtss_mce_entry_t *mce;
    vtss_sdx_entry_t *isdx;
    BOOL             is_mapped;

    if (evc == NULL || vtss_evc_hqos_port_is_mapped(vtss_state, evc_id, port_no, &is_mapped) != VTSS_RC_OK || !is_mapped) {
        return VTSS_RC_OK;
    }

    for (ece = vtss_state->evc.ece_info.used; ece != NULL; ece = ece->next) {
        if (ece->evc_id == evc_id) {
            for (isdx = ece->isdx_list; isdx != NULL; isdx = isdx->next) {
                VTSS_FUNC_RC(hqos.isdx_del, evc->hqos_id[port_no], isdx->sdx);
            }
        }
    }

    for (mce = vtss_state->evc.mce_info.used; mce != NULL; mce = mce->next) {
        if (mce->conf.action.evc_id == evc_id) {
            for (isdx = mce->isdx_list; isdx != NULL; isdx = isdx->next) {
                VTSS_FUNC_RC(hqos.isdx_del, evc->hqos_id[port_no], isdx->sdx);
            }
        }
    }

    return VTSS_RC_OK;
}

static vtss_rc vtss_evc_hqos_map_evc_port(vtss_state_t *vtss_state, vtss_evc_id_t evc_id, vtss_port_no_t port_no)
{
    vtss_evc_entry_t *evc = vtss_cmn_evc_get(vtss_state, evc_id);
    vtss_ece_entry_t *ece;
    vtss_mce_entry_t *mce;
    vtss_sdx_entry_t *isdx;
    BOOL             is_mapped;

    if (evc == NULL || vtss_evc_hqos_port_is_mapped(vtss_state, evc_id, port_no, &is_mapped) != VTSS_RC_OK || !is_mapped) {
        return VTSS_RC_OK;
    }

    for (ece = vtss_state->evc.ece_info.used; ece != NULL; ece = ece->next) {
        if (ece->evc_id == evc_id) {
            for (isdx = ece->isdx_list; isdx != NULL; isdx = isdx->next) {
                if (port_no != isdx->port_no) {
                    VTSS_FUNC_RC(hqos.map, evc->hqos_id[port_no], VTSS_PORT_BF_GET(evc->ports, port_no));
                    VTSS_FUNC_RC(hqos.isdx_add, evc->hqos_id[port_no], isdx->sdx, FALSE);
                }
            }
        }
    }

    for (mce = vtss_state->evc.mce_info.used; mce != NULL; mce = mce->next) {
        if (mce->conf.action.evc_id == evc_id) {
            for (isdx = mce->isdx_list; isdx != NULL; isdx = isdx->next) {
                if (port_no != isdx->port_no) {
                    VTSS_FUNC_RC(hqos.map, evc->hqos_id[port_no], VTSS_PORT_BF_GET(evc->ports, port_no));
                    VTSS_FUNC_RC(hqos.isdx_add, evc->hqos_id[port_no], isdx->sdx, FALSE);
                }
            }
        }
    }

    return VTSS_RC_OK;
}

vtss_rc vtss_evc_hqos_map_set(const vtss_inst_t    inst,
                              const vtss_evc_id_t  evc_id,
                              const vtss_port_no_t port_no,
                              const vtss_hqos_id_t hqos_id)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("evc_id: %u, port_no: %u, hqos_id: %u", evc_id, port_no, hqos_id);

    VTSS_ENTER();

    if ((rc = vtss_inst_evc_id_check(inst, &vtss_state, evc_id, 0, 1)) == VTSS_RC_OK &&
        (rc = vtss_port_no_check(vtss_state, port_no)) == VTSS_RC_OK &&
        (rc = vtss_evc_hqos_id_check(vtss_state, hqos_id, port_no)) == VTSS_RC_OK) {
            vtss_evc_entry_t *evc = vtss_cmn_evc_get(vtss_state, evc_id);
            if (evc != NULL) {
                evc->hqos_id[port_no] = hqos_id;
                rc = vtss_cmn_evc_hqos_map(vtss_state);
            }
    }

    VTSS_EXIT();

    return rc;
}

#endif /* VTSS_FEATURE_HQOS */

vtss_rc vtss_ece_init(const vtss_inst_t     inst,
                      const vtss_ece_type_t type,
                      vtss_ece_t            *const ece)
{
    VTSS_D("type: %d", type);
    switch (type) {
    case VTSS_ECE_TYPE_ANY:
#if defined(VTSS_ARCH_SERVAL)
    case VTSS_ECE_TYPE_ETYPE:
    case VTSS_ECE_TYPE_LLC:
    case VTSS_ECE_TYPE_SNAP:
#endif /* VTSS_ARCH_SERVAL */
    case VTSS_ECE_TYPE_IPV4:
    case VTSS_ECE_TYPE_IPV6:
        memset(ece, 0, sizeof(*ece));
        ece->key.type = type;
        break;
    default:
        VTSS_E("illegal type: %d", type);
        return VTSS_RC_ERROR;
    }

    return VTSS_RC_OK;
}

vtss_rc vtss_ece_add(const vtss_inst_t   inst,
                     const vtss_ece_id_t ece_id,
                     const vtss_ece_t    *const ece)
{
    vtss_state_t  *vtss_state;
    vtss_rc       rc;
    vtss_evc_id_t evc_id = ece->action.evc_id;

    VTSS_D("ece_id: %u, evc_id: %u", ece_id, evc_id);
    VTSS_ENTER();
    if ((rc = vtss_inst_evc_id_check(inst, &vtss_state, evc_id, 1, 0)) == VTSS_RC_OK)
        rc = VTSS_FUNC(evc.ece_add, ece_id, ece);
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_ece_del(const vtss_inst_t   inst,
                     const vtss_ece_id_t ece_id)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("ece_id: %u", ece_id);
    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK) 
        rc = VTSS_FUNC(evc.ece_del, ece_id);
    VTSS_EXIT();
    return rc;
}

#if defined(VTSS_FEATURE_OAM)
vtss_rc vtss_evc_oam_port_conf_get(const vtss_inst_t        inst,
                                   const vtss_evc_id_t      evc_id,
                                   const vtss_port_no_t     port_no,
                                   vtss_evc_oam_port_conf_t *const conf)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;
    u8           voe_idx;

    VTSS_D("evc_id: %u, port_no: %u", evc_id, port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_evc_id_check(inst, &vtss_state, evc_id, 0, 0)) == VTSS_RC_OK &&
        (rc = vtss_port_no_check(vtss_state, port_no)) == VTSS_RC_OK) {
        voe_idx = vtss_state->evc.info.table[evc_id].voe_idx[port_no];
        conf->voe_idx = (voe_idx == VTSS_EVC_VOE_IDX_NONE ? VTSS_OAM_VOE_IDX_NONE : voe_idx);
    }
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_evc_oam_port_conf_set(const vtss_inst_t              inst,
                                   const vtss_evc_id_t            evc_id,
                                   const vtss_port_no_t           port_no,
                                   const vtss_evc_oam_port_conf_t *const conf)
{
    vtss_state_t     *vtss_state;
    vtss_rc          rc;
    vtss_res_t       res;
    vtss_evc_entry_t *evc;

    VTSS_D("evc_id: %u, port_no: %u", evc_id, port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_evc_id_check(inst, &vtss_state, evc_id, 0, 0)) == VTSS_RC_OK &&
        (rc = vtss_port_no_check(vtss_state, port_no)) == VTSS_RC_OK) {
        evc = &vtss_state->evc.info.table[evc_id];
        evc->voe_idx[port_no] = (conf->voe_idx == VTSS_OAM_VOE_IDX_NONE ? 
                                 VTSS_EVC_VOE_IDX_NONE : conf->voe_idx);
        if (evc->enable) {
            vtss_cmn_res_init(&res);
            rc = VTSS_FUNC_COLD(evc.update, evc_id, &res, VTSS_RES_CMD_ADD);
        }
    }
    VTSS_EXIT();
    return rc;
}
#endif /* VTSS_FEATURE_OAM */

#if defined(VTSS_ARCH_CARACAL) || defined(VTSS_ARCH_SERVAL)
vtss_rc vtss_mce_init(const vtss_inst_t inst,
                      vtss_mce_t        *const mce)
{
    memset(mce, 0, sizeof(*mce));
#if defined(VTSS_ARCH_SERVAL)
    mce->action.evc_counting = FALSE;
    mce->action.evc_etree = FALSE;
    mce->action.evc_id = VTSS_EVC_ID_NONE;
#endif /* VTSS_ARCH_SERVAL */
#if defined(VTSS_ARCH_JAGUAR_1) || defined(VTSS_ARCH_SERVAL)
    mce->action.policer_id = VTSS_EVC_POLICER_ID_NONE;
#endif
    return VTSS_RC_OK;
}

vtss_rc vtss_mce_add(const vtss_inst_t   inst,
                     const vtss_mce_id_t mce_id,
                     const vtss_mce_t    *const mce)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("mce_id: %u", mce_id);
    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK) 
        rc = VTSS_FUNC(evc.mce_add, mce_id, mce);
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_mce_del(const vtss_inst_t   inst,
                     const vtss_mce_id_t mce_id)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("mce_id: %u", mce_id);

    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK) 
        rc = VTSS_FUNC(evc.mce_del, mce_id);
    VTSS_EXIT();
    return rc;
}

#if defined(VTSS_ARCH_SERVAL)
vtss_rc vtss_mce_port_info_get(const vtss_inst_t    inst,
                               const vtss_mce_id_t  mce_id,
                               const vtss_port_no_t port_no,
                               vtss_mce_port_info_t *const info)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("mce_id: %u, port_no: %u", mce_id, port_no);

    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK) 
        rc = VTSS_FUNC(evc.mce_port_get, mce_id, port_no, info);
    VTSS_EXIT();
    return rc;
}
#endif /* VTSS_ARCH_SERVAL */
#endif /* VTSS_ARCH_CARACAL/SERVAL */

#if defined(VTSS_ARCH_JAGUAR_1) || defined(VTSS_ARCH_SERVAL)
vtss_rc vtss_evc_counters_get(const vtss_inst_t    inst,
                              const vtss_evc_id_t  evc_id,
                              const vtss_port_no_t port_no,
                              vtss_evc_counters_t  *const counters)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("evc_id: %u, port_no: %u", evc_id, port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK &&
        (rc = vtss_inst_evc_id_check(inst, &vtss_state, evc_id, 0, 1)) == VTSS_RC_OK)
        rc = VTSS_FUNC(evc.counters_get, evc_id, port_no, counters);
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_evc_counters_clear(const vtss_inst_t    inst,
                                const vtss_evc_id_t  evc_id,
                                const vtss_port_no_t port_no)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("evc_id: %u, port_no: %u", evc_id, port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK &&
        (rc = vtss_inst_evc_id_check(inst, &vtss_state, evc_id, 0, 1)) == VTSS_RC_OK)
        rc = VTSS_FUNC(evc.counters_clear, evc_id, port_no);
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_ece_counters_get(const vtss_inst_t    inst,
                              const vtss_ece_id_t  ece_id,
                              const vtss_port_no_t port_no,
                              vtss_evc_counters_t  *const counters)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("ece_id: %u, port_no: %u", ece_id, port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK)
        rc = VTSS_FUNC(evc.ece_counters_get, ece_id, port_no, counters);
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_ece_counters_clear(const vtss_inst_t    inst,
                                const vtss_ece_id_t  ece_id,
                                const vtss_port_no_t port_no)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("ece_id: %u, port_no: %u", ece_id, port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK)
        rc = VTSS_FUNC(evc.ece_counters_clear, ece_id, port_no);
    VTSS_EXIT();
    return rc;
}

#if defined(VTSS_ARCH_SERVAL)
vtss_rc vtss_mce_counters_get(const vtss_inst_t    inst,
                              const vtss_mce_id_t  mce_id,
                              const vtss_port_no_t port_no,
                              vtss_evc_counters_t  *const counters)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("mce_id: %u, port_no: %u", mce_id, port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK)
        rc = VTSS_FUNC(evc.mce_counters_get, mce_id, port_no, counters);
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_mce_counters_clear(const vtss_inst_t    inst,
                                const vtss_mce_id_t  mce_id,
                                const vtss_port_no_t port_no)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("mce_id: %u, port_no: %u", mce_id, port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK)
        rc = VTSS_FUNC(evc.mce_counters_clear, mce_id, port_no);
    VTSS_EXIT();
    return rc;
}
#endif /* VTSS_ARCH_SERVAL */
#endif /* VTSS_ARCH_JAGUAR_1/SERVAL */

/* - Warm start synchronization ------------------------------------ */

#if defined(VTSS_FEATURE_WARM_START)
vtss_rc vtss_evc_restart_sync(vtss_state_t *vtss_state)
{
    vtss_port_no_t port_no;

    for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {
        vtss_state->evc.port_conf_old = vtss_state->evc.port_conf[port_no];
        VTSS_FUNC_RC(evc.port_conf_set, port_no);
    }
    return VTSS_RC_OK;
}
#endif /* VTSS_FEATURE_WARM_START */

/* - Instance create and initialization ---------------------------- */

vtss_rc vtss_evc_inst_create(vtss_state_t *vtss_state)
{
    vtss_ece_info_t  *ece_info = &vtss_state->evc.ece_info;
    vtss_ece_entry_t *ece;
    u32              i;

#if defined(VTSS_ARCH_JAGUAR_1)
    /* Initialize tunnel policer mappings */
    for (i = 0; i < VTSS_EVC_POLICERS; i++) {
        vtss_state->evc.policer_info[i].tdlb_id = VTSS_TDLB_ID_NONE;
    }
#endif /* VTSS_ARCH_JAGUAR_1 */

#if defined(VTSS_ARCH_SERVAL)
    vtss_evc_info_t   *evc_info = &vtss_state->evc.info;
    vtss_evc_entry_t  *evc;
    vtss_port_no_t    port_no;
    vtss_mce_info_t   *mce_info = &vtss_state->evc.mce_info;
    vtss_mce_entry_t  *mce;
    
    /* Initialize VOE mappings */
    for (i = 0; i < evc_info->max_count; i++) {
        evc = &evc_info->table[i];
        for (port_no = VTSS_PORT_NO_START; port_no < VTSS_PORT_NO_END; port_no++) {
            evc->voe_idx[port_no] = VTSS_EVC_VOE_IDX_NONE;
        }
    }
    
    /* Initialize HQoS mappings */
    for (i = 0; i < evc_info->max_count; i++) {
        evc = &evc_info->table[i];
        for (port_no = VTSS_PORT_NO_START; port_no < VTSS_PORT_NO_END; port_no++) {
            evc->hqos_id[port_no] = VTSS_HQOS_ID_NONE;
        }
    }

    /* Add MCEs to free list */
    for (i = 0; i < mce_info->max_count; i++) {
        mce = &mce_info->table[i];
        mce->next = mce_info->free;
        mce_info->free = mce;
    }
#endif /* VTSS_ARCH_SERVAL */
    
    /* Add ECEs to free list */
    for (i = 0; i < ece_info->max_count; i++) {
        ece = &ece_info->table[i];
        ece->next = ece_info->free;
        ece_info->free = ece;
    }

#if defined(VTSS_SDX_CNT)
    {
        vtss_sdx_info_t  *sdx_info = &vtss_state->evc.sdx_info;
        vtss_sdx_list_t  *isdx_list = &sdx_info->isdx; 
        vtss_sdx_list_t  *esdx_list = &sdx_info->esdx; 
        vtss_sdx_entry_t *sdx;
        
        /* Add SDX entries to free lists */
        for (i = 0; i < sdx_info->max_count; i++) {
            /* ISDX */
            sdx = &isdx_list->table[i];
            sdx->port_no = VTSS_PORT_NO_NONE;
            sdx->sdx = (sdx_info->max_count - i);
#if defined(VTSS_ARCH_SERVAL) && defined(VTSS_FEATURE_MPLS)
            if (sdx->sdx < 512) {
                sdx->next = isdx_list->free_low;
                isdx_list->free_low = sdx;
            } else {
                sdx->next = isdx_list->free;
                isdx_list->free = sdx;
            }
#else
            sdx->next = isdx_list->free;
            isdx_list->free = sdx;
#endif /* VTSS_ARCH_SERVAL && VTSS_FEATURE_MPLS */

            /* ESDX */
            sdx = &esdx_list->table[i];
            sdx->port_no = VTSS_PORT_NO_NONE;
            sdx->sdx = (sdx_info->max_count - i);
            sdx->next = esdx_list->free;
            esdx_list->free = sdx;
        }
    }
#endif /* VTSS_SDX_CNT */

    return VTSS_RC_OK;
}

/* - EVC utilities ------------------------------------------------- */

#if defined(VTSS_FEATURE_HQOS)

static vtss_rc vtss_evc_hqos_detect_mp_conflict(vtss_state_t *vtss_state, vtss_evc_id_t evc_id, BOOL *mp_conflict)
{
    vtss_evc_entry_t *evc = vtss_cmn_evc_get(vtss_state, evc_id);
    vtss_ece_entry_t *ece;
    vtss_port_no_t   port_no;
    BOOL             ports[VTSS_PORT_ARRAY_SIZE];
    u32              port_count = 0;
    BOOL             hqos_found = FALSE, none_hqos_found = FALSE;

    *mp_conflict = FALSE;

    if (evc == NULL) {
        return VTSS_RC_OK;
    }

    memset(ports, 0, sizeof(ports));

    /* Find NNI ports */
    for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {
        if (VTSS_PORT_BF_GET(evc->ports, port_no)) {
            ports[port_no] = TRUE;
        }
    }

    /* Find UNI ports */
    for (ece = vtss_state->evc.ece_info.used; ece != NULL; ece = ece->next) {
        if (ece->evc_id == evc_id) {
            for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {
                if (VTSS_PORT_BF_GET(ece->ports, port_no)) {
                    ports[port_no] = TRUE;
                }
            }
        }
    }

    /* Check for mp conflict */
    for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {
        if (evc->hqos_id[port_no] < VTSS_HQOS_COUNT && !vtss_state->hqos.info.table[evc->hqos_id[port_no]].enable) {
            evc->hqos_id[port_no] = VTSS_HQOS_ID_NONE;
        }
        if (ports[port_no] && vtss_state->hqos.port_conf[port_no].sch_mode == VTSS_HQOS_SCH_MODE_HIERARCHICAL) {
            BOOL is_mapped;
            port_count++;
            if (vtss_evc_hqos_port_is_mapped(vtss_state, evc_id, port_no, &is_mapped) == VTSS_RC_OK && is_mapped) {
                hqos_found = TRUE;
            } else {
                none_hqos_found = TRUE;
            }
        }
    }
    if (port_count > 2 && hqos_found && none_hqos_found) {
        *mp_conflict = TRUE;
    }

    /* Remove all existing mappings */
    if (*mp_conflict) {
        for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {
            VTSS_RC(vtss_evc_hqos_demap_evc_port(vtss_state, evc_id, port_no));
        }
    }

    return VTSS_RC_OK;
}

vtss_rc vtss_cmn_evc_hqos_map(vtss_state_t *vtss_state)
{
    vtss_evc_id_t evc_id;
    vtss_rc       rc = VTSS_RC_OK;

    for (evc_id = 0; evc_id < VTSS_EVCS; evc_id++) {
        vtss_evc_entry_t *evc = vtss_cmn_evc_get(vtss_state, evc_id);
        if (evc != NULL) {
            BOOL mp_conflict;
            if (vtss_evc_hqos_detect_mp_conflict(vtss_state, evc_id, &mp_conflict) == VTSS_RC_OK && !mp_conflict) {
                vtss_port_no_t port_no;
                for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {
                    if (vtss_evc_hqos_map_evc_port(vtss_state, evc_id, port_no) != VTSS_RC_OK) {
                        VTSS_I("HQoS mapping failed for evc_id %u on port_no %u", evc_id, port_no);
                        (void) vtss_evc_hqos_demap_evc_port(vtss_state, evc_id, port_no);
                        evc->hqos_id[port_no] = VTSS_HQOS_ID_NONE;
                        rc = VTSS_RC_ERROR;
                    }
                }
            }
        }
    }

    return rc;
}

#endif /* VTSS_FEATURE_HQOS */

#if defined(VTSS_SDX_CNT)

#if defined(VTSS_ARCH_SERVAL) && defined(VTSS_FEATURE_MPLS)
static vtss_sdx_entry_t *vtss_cmn_sdx_alloc_impl(vtss_state_t *vtss_state, vtss_port_no_t port_no, u16 id, BOOL isdx, BOOL take_isdx_from_low)
{
    vtss_sdx_entry_t *sdx;
    vtss_sdx_list_t  *list = (isdx ? &vtss_state->evc.sdx_info.isdx : &vtss_state->evc.sdx_info.esdx);

    sdx = list->free;

    if (isdx) {
        if (take_isdx_from_low || sdx == NULL) {
            sdx = list->free_low;
            take_isdx_from_low = TRUE;
        }
    }

    if (sdx == NULL) {
        VTSS_E("%sSDX alloc failed, port_no: %u", isdx ? "I" : "E", port_no);
        return NULL;
    }

    /* Take out of free list */

    if (take_isdx_from_low) {
        list->free_low = sdx->next;
    } else {
        list->free = sdx->next;
    }
    list->count++;
    sdx->port_no = port_no;
    sdx->id = id;
    sdx->next = NULL;

    /* Clear counters */
    (void)VTSS_FUNC(evc.sdx_counters_update, isdx ? sdx : NULL, isdx ? NULL : sdx, NULL, 1);

    return sdx;
}

vtss_sdx_entry_t *vtss_cmn_sdx_alloc(vtss_state_t *vtss_state, vtss_port_no_t port_no, u16 id, BOOL isdx)
{
    return vtss_cmn_sdx_alloc_impl(vtss_state, port_no, id, isdx, FALSE);
}

vtss_sdx_entry_t *vtss_cmn_isdx_alloc_low(vtss_state_t *vtss_state, vtss_port_no_t port_no)
{
    return vtss_cmn_sdx_alloc_impl(vtss_state, port_no, 0, TRUE, TRUE);
}

void vtss_cmn_sdx_free(vtss_state_t *vtss_state, vtss_sdx_entry_t *sdx, BOOL isdx)
{
    vtss_sdx_list_t *list = (isdx ? &vtss_state->evc.sdx_info.isdx : &vtss_state->evc.sdx_info.esdx);

    if (isdx && sdx->sdx < 512) {
        sdx->next = list->free_low;
        list->free_low = sdx;
    } else {
        sdx->next = list->free;
        list->free = sdx;
    }
    sdx->port_no = VTSS_PORT_NO_NONE;
    list->count--;
}

#else /* ! (VTSS_ARCH_SERVAL && VTSS_FEATURE_MPLS) */

vtss_sdx_entry_t *vtss_cmn_sdx_alloc(vtss_state_t *vtss_state, vtss_port_no_t port_no, u16 id, BOOL isdx)
{
    vtss_sdx_entry_t *sdx;
    vtss_sdx_list_t  *list = (isdx ? &vtss_state->evc.sdx_info.isdx : &vtss_state->evc.sdx_info.esdx);

    if ((sdx = list->free) == NULL) {
        VTSS_E("%sSDX alloc failed, port_no: %u", isdx ? "I" : "E", port_no);
        return NULL;
    }

    /* Take out of free list */
    list->free = sdx->next;
    list->count++;
    sdx->port_no = port_no;
    sdx->id = id;
    sdx->next = NULL;

    /* Clear counters */
    (void)VTSS_FUNC(evc.sdx_counters_update, isdx ? sdx : NULL, isdx ? NULL : sdx, NULL, 1);

    return sdx;
}

void vtss_cmn_sdx_free(vtss_state_t *vtss_state, vtss_sdx_entry_t *sdx, BOOL isdx)
{
    vtss_sdx_list_t *list = (isdx ? &vtss_state->evc.sdx_info.isdx : &vtss_state->evc.sdx_info.esdx);

    sdx->port_no = VTSS_PORT_NO_NONE;
    sdx->id = 0;
    sdx->next = list->free;
    list->free = sdx;
    list->count--;
}
#endif /* VTSS_ARCH_SERVAL && VTSS_FEATURE_MPLS */

vtss_sdx_entry_t *vtss_cmn_ece_sdx_alloc(vtss_state_t *vtss_state,
                                         vtss_ece_entry_t *ece, vtss_port_no_t port_no, u16 id, BOOL isdx)
{
    vtss_sdx_entry_t *cur, *prev = NULL, **list = (isdx ? &ece->isdx_list : &ece->esdx_list);

    for (cur = *list; cur != NULL; prev = cur, cur = cur->next) {
        if (cur->port_no == port_no) {
            if (cur->id == id) {
                /* Reallocate */
                return cur;
            }
            if (cur->id > id) {
                break;
            }
        } else if (cur->port_no > port_no) {
            /* Found place to insert new entry */
            break;
        }
    }

    if ((cur = vtss_cmn_sdx_alloc(vtss_state, port_no, id, isdx)) == NULL) {
        VTSS_E("SDX alloc failed, ece_id: %u, port_no: %u, id: %u", ece->ece_id, port_no, id);
        return NULL;
    }

    /* Insert in ECE list */
    if (prev == NULL) {
        cur->next = *list;
        *list = cur;
    } else {
        cur->next = prev->next;
        prev->next = cur;
    }

    return cur;
}

vtss_rc vtss_cmn_ece_sdx_free(vtss_state_t *vtss_state,
                              vtss_ece_entry_t *ece, vtss_port_no_t port_no, u16 id, BOOL isdx)
{
    vtss_sdx_entry_t *cur, *prev = NULL, **list = (isdx ? &ece->isdx_list : &ece->esdx_list);
    
    for (cur = *list; cur != NULL; prev = cur, cur = cur->next) {
        if (cur->port_no == port_no && cur->id == id) {
            /* Found entry, move to free list */
            if (prev == NULL)
                *list = cur->next;
            else
                prev->next = cur->next;
            vtss_cmn_sdx_free(vtss_state, cur, isdx);
            return VTSS_RC_OK;
        }
    }

    VTSS_E("%sSDX free failed, ece_id: %u, port_no: %u, id: %u", isdx ? "I" : "E", ece->ece_id, port_no, id);
    return VTSS_RC_ERROR;
}
#endif /* VTSS_SDX_CNT */

BOOL vtss_cmn_evc_port_check(vtss_state_t *vtss_state)
{
    return (vtss_state->arch == VTSS_ARCH_L26 || vtss_state->arch == VTSS_ARCH_JR1 ? 1 : 0);
}

/* EVC port add/delete/update flags */
#define EVC_PORT_NONE 0x00
#define EVC_PORT_OLD  0x01 /* Port was enabled before */
#define EVC_PORT_NEW  0x02 /* Port is enabled now */
#define EVC_PORT_UNI  0x04 /* Port is UNI */
#define EVC_PORT_NNI  0x08 /* Port is NNI */

#define EVC_PORT_CHG  (EVC_PORT_OLD | EVC_PORT_NEW) 

/* Update EVC resource usage for port */
static vtss_rc vtss_cmn_res_evc_port_update(vtss_state_t *vtss_state,
                                            vtss_res_t *res, vtss_port_no_t port_no, u8 flags)
{
    vtss_evc_port_info_t *port_info = &vtss_state->evc.port_info[port_no];
    BOOL                 port_nni = VTSS_BOOL(flags & EVC_PORT_NNI);

    switch (flags & EVC_PORT_CHG) {
    case EVC_PORT_OLD: /* Delete port */
        res->port_del[port_no] = 1;
        break;
    case EVC_PORT_NEW: /* Add port */
        if (vtss_cmn_evc_port_check(vtss_state)) {
            if (port_nni && port_info->uni_count) {
                VTSS_E("port_no: %u, UNI->NNI change not allowed", port_no);
                return VTSS_RC_ERROR;
            }
            if (!port_nni && port_info->nni_count) {
                VTSS_E("port_no: %u, NNI->UNI change not allowed", port_no);
                return VTSS_RC_ERROR;
            }
        }
        res->port_add[port_no] = 1;
        break;
    case EVC_PORT_CHG:
        res->port_chg[port_no] = 1;
        break;
    default:
        break;
    } 
    res->port_nni[port_no] = port_nni;
    return VTSS_RC_OK;
}

static vtss_rc vtss_cmn_evc_port_update(vtss_state_t *vtss_state,
                                        vtss_port_no_t port_no, u8 flags)
{
    u16  *count;
    BOOL update = 0, nni = (flags & EVC_PORT_NNI);

    if (nni) {
        /* Update NNI counter */
        count = &vtss_state->evc.port_info[port_no].nni_count;
    } else {
        /* Update UNI counter */
        count = &vtss_state->evc.port_info[port_no].uni_count;
    }

    if (flags & EVC_PORT_NEW) {
        /* Adding port */
        if (*count == 0)
            update = 1;
        (*count)++;
    } else {
        /* Deleting port */
        if (*count == 0) {
            /* This should never happen */
            VTSS_E("%s port_no: %u count already zero", nni ? "NNI" : "UNI", port_no);
            return VTSS_RC_ERROR;
        }
        (*count)--;
        if (*count == 0)
            update = 1;
    }

    if (update && !vtss_state->warm_start_cur) {
        /* Update VLAN port configuration if UNI/NNI mode changed */
        VTSS_RC(vtss_cmn_vlan_port_conf_set(vtss_state, port_no));

        /* Update EVC port configuration if UNI/NNI mode changed */
        return VTSS_FUNC(evc.port_conf_set, port_no);
    }
    return VTSS_RC_OK;
}

static vtss_rc vtss_cmn_evc_vlan_update(vtss_state_t *vtss_state, vtss_vid_t vid, BOOL learning)
{
    if (vid == 0)
        return VTSS_RC_OK;
    
    vtss_state->l2.vlan_table[vid].evc_learning = learning;
    return VTSS_RC_COLD(vtss_cmn_vlan_members_set(vtss_state, vid));
}

vtss_rc vtss_cmn_evc_conf_get(vtss_state_t        *vtss_state,
                              const vtss_evc_id_t evc_id,
                              vtss_evc_conf_t     *const conf)
{
    vtss_port_no_t          port_no;
    vtss_evc_entry_t        *evc;
    vtss_evc_pb_conf_t      *pb   = &conf->network.pb;
#if defined(VTSS_FEATURE_MPLS)
    vtss_evc_mpls_tp_conf_t *mpls = &conf->network.mpls_tp;
    u32                     i;
#endif

    evc = &vtss_state->evc.info.table[evc_id];
#if defined(VTSS_ARCH_JAGUAR_1) || defined(VTSS_ARCH_SERVAL)
    conf->policer_id = evc->policer_id;
#endif /* VTSS_ARCH_JAGUAR_1/SERVAL */
    conf->learning = evc->learning;
    pb->vid       = evc->vid;
    pb->ivid      = evc->ivid;
#if defined(VTSS_FEATURE_E_TREE)
    pb->leaf_vid  = evc->leaf_vid;
    pb->leaf_ivid = evc->leaf_ivid;
#endif /* VTSS_FEATURE_E_TREE */
#if defined(VTSS_ARCH_CARACAL)
    pb->uvid      = evc->uvid;
    pb->inner_tag = evc->inner_tag;
#endif /* VTSS_ARCH_CARACAL */

    for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {
#if defined(VTSS_FEATURE_MPLS)
        // Calc set of exposed NNI ports: All the NNI ports that aren't also PW residence ports
        pb->nni[port_no] = VTSS_PORT_BF_GET(evc->ports, port_no) && !VTSS_PORT_BF_GET(evc->pw_ports, port_no);
#else
        pb->nni[port_no] = VTSS_PORT_BF_GET(evc->ports, port_no);
#endif /* VTSS_FEATURE_MPLS */
#if defined(VTSS_FEATURE_E_TREE)
        pb->leaf[port_no] = VTSS_PORT_BF_GET(evc->leaf_ports, port_no);
#endif /* VTSS_FEATURE_E_TREE */
    }
#if defined(VTSS_FEATURE_MPLS)
    for (i = 0; i < VTSS_ARRSZ(mpls->pw); i++) {
        if (evc->pw[i].pw_num) {
            mpls->pw[i].ingress_xc    = evc->pw[i].ingress_xc;
            mpls->pw[i].egress_xc     = evc->pw[i].egress_xc;
            mpls->pw[i].split_horizon = evc->pw[i].split_horizon;
            mpls->pw[i].pw_num        = evc->pw[i].pw_num;
        } else {
            mpls->pw[i].pw_num        = 0;
            mpls->pw[i].split_horizon = FALSE;
            mpls->pw[i].ingress_xc    = VTSS_MPLS_IDX_UNDEFINED;
            mpls->pw[i].egress_xc     = VTSS_MPLS_IDX_UNDEFINED;
        }
    }
#endif /* VTSS_FEATURE_MPLS */
    return VTSS_RC_OK;
}

static BOOL vtss_evc_etree(vtss_evc_entry_t *evc)
{
#if defined(VTSS_FEATURE_E_TREE)
    return (evc->leaf_ivid != 0 && evc->leaf_vid != 0);
#else
    return 0;
#endif /* VTSS_FEATURE_E_TREE */
}

/* Add EVC */
vtss_rc vtss_cmn_evc_add(vtss_state_t *vtss_state,
                         const vtss_evc_id_t evc_id, const vtss_evc_conf_t *const conf)
{
    vtss_evc_entry_t              *evc = &vtss_state->evc.info.table[evc_id];
    const vtss_evc_pb_conf_t      *pb = &conf->network.pb;
#if defined(VTSS_FEATURE_MPLS)
    const vtss_evc_mpls_tp_conf_t *mpls_tp = &conf->network.mpls_tp;
    BOOL                          pw_ports[VTSS_PORT_ARRAY_SIZE];
    u32                           i;
#endif
    vtss_res_t                    res;
    vtss_port_no_t                port_no;
    u8                            flags;
    BOOL                          add;
    vtss_vid_t                    old_ivid;
#if defined(VTSS_FEATURE_E_TREE)
    vtss_vid_t                    old_leaf_ivid;
#endif /* VTSS_FEATURE_E_TREE */

    VTSS_D("evc_id: %u", evc_id);

#if defined(VTSS_FEATURE_MPLS)
    memset(pw_ports, 0, sizeof(pw_ports));
    for (i = 0; i < VTSS_ARRSZ(mpls_tp->pw) && mpls_tp->pw[i].pw_num; i++) {
        VTSS_D("Looking at idx %3u: pw_num %4u ingress_xc %4u egress_xc %4u split-horison %u", i, mpls_tp->pw[i].pw_num, mpls_tp->pw[i].ingress_xc, mpls_tp->pw[i].egress_xc, mpls_tp->pw[i].split_horizon);
        if (VTSS_MPLS_IDX_IS_DEF(mpls_tp->pw[i].ingress_xc)) {
            const vtss_mpls_segment_idx_t seg_idx = vtss_state->mpls.xc_conf[mpls_tp->pw[i].ingress_xc].pub.in_seg_idx;
            if (vtss_cmn_mpls_port_no_get(vtss_state, seg_idx, &port_no) == VTSS_RC_OK) {
                VTSS_D("Found ingress port: %u for XC %d, segment %d", port_no, mpls_tp->pw[i].ingress_xc, seg_idx);
                pw_ports[port_no] = 1;
            }
        }
        if (VTSS_MPLS_IDX_IS_DEF(mpls_tp->pw[i].egress_xc)) {
            const vtss_mpls_segment_idx_t seg_idx = vtss_state->mpls.xc_conf[mpls_tp->pw[i].egress_xc].pub.out_seg_idx;
            if (vtss_cmn_mpls_port_no_get(vtss_state, seg_idx, &port_no) == VTSS_RC_OK) {
                VTSS_D("Found egress port: %u for XC %d, segment %d", port_no, mpls_tp->pw[i].egress_xc, seg_idx);
                pw_ports[port_no] = 1;
            }
        }
    }
#endif

    /* Check port roles */
    vtss_cmn_res_init(&res);
#if defined(VTSS_FEATURE_E_TREE)
    res.etree_old = vtss_evc_etree(evc);
    res.etree_new = (pb->leaf_ivid != 0 && pb->leaf_vid != 0);
#endif /* VTSS_FEATURE_E_TREE */
    for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {
        flags = EVC_PORT_NONE;
#if defined(VTSS_FEATURE_E_TREE)
        res.port_leaf_new[port_no] = (res.etree_new && pb->leaf[port_no] && pb->nni[port_no] == 0);
        res.port_leaf_old[port_no] = VTSS_PORT_BF_GET(evc->leaf_ports, port_no);
#endif /* VTSS_FEATURE_E_TREE */
#if defined(VTSS_FEATURE_MPLS)
        if (pb->nni[port_no] || pw_ports[port_no])
#else
        if (pb->nni[port_no])
#endif
            flags |= (EVC_PORT_NEW | EVC_PORT_NNI);
        if (VTSS_PORT_BF_GET(evc->ports, port_no))
            flags |= (EVC_PORT_OLD | EVC_PORT_NNI);
        VTSS_RC(vtss_cmn_res_evc_port_update(vtss_state, &res, port_no, flags));
    }
    res.evc_add = (evc->enable ? 0 : 1);

    /* Calculate resource usage */
    VTSS_FUNC_RC(evc.update, evc_id, &res, VTSS_RES_CMD_CALC);

    /* Check resource availability */
    VTSS_RC(vtss_cmn_res_check(vtss_state, &res));

    /* Save EVC configuration */
    vtss_state->evc.info.count++;
    evc->enable = 1;
    evc->learning = conf->learning;
    evc->vid = pb->vid;
    old_ivid = evc->ivid;
    evc->ivid = pb->ivid;
#if defined(VTSS_FEATURE_E_TREE)
    evc->leaf_vid = pb->leaf_vid;
    old_leaf_ivid = evc->leaf_ivid;
    evc->leaf_ivid = pb->leaf_ivid;
#endif /* VTSS_FEATURE_E_TREE */
#if defined(VTSS_ARCH_CARACAL)
    evc->uvid = pb->uvid;
    evc->inner_tag = pb->inner_tag;
#endif /* VTSS_ARCH_CARACAL */

#if defined(VTSS_FEATURE_MPLS)
    for (i = 0; i < VTSS_ARRSZ(evc->pw); i++) {
        evc->pw[i].ingress_xc    = mpls_tp->pw[i].ingress_xc;
        evc->pw[i].egress_xc     = mpls_tp->pw[i].egress_xc;
        evc->pw[i].split_horizon = mpls_tp->pw[i].split_horizon;
        evc->pw[i].pw_num        = mpls_tp->pw[i].pw_num;
    }
    VTSS_PORT_BF_CLR(evc->pw_ports);
#endif /* VTSS_FEATURE_MPLS */

    for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {
#if defined(VTSS_FEATURE_MPLS)
        if (pw_ports[port_no]) {
            VTSS_PORT_BF_SET(evc->pw_ports, port_no, 1);
        }
#endif /* VTSS_FEATURE_MPLS */
#if defined(VTSS_FEATURE_E_TREE)
        VTSS_PORT_BF_SET(evc->leaf_ports, port_no, res.port_leaf_new[port_no]);
#endif /* VTSS_FEATURE_E_TREE */
        if (res.port_add[port_no]) {
            add = 1;
            flags = EVC_PORT_NEW;
        } else if (res.port_del[port_no]) {
            add = 0;
            flags = EVC_PORT_OLD;
        } else
            continue;
        VTSS_PORT_BF_SET(evc->ports, port_no, add);
        VTSS_RC(vtss_cmn_evc_port_update(vtss_state, port_no, flags | EVC_PORT_NNI));
    }

#if defined(VTSS_ARCH_JAGUAR_1) || defined(VTSS_ARCH_SERVAL)
    evc->policer_id = conf->policer_id;
#endif /* VTSS_ARCH_JAGUAR_1/SERVAL */

    /* Update learning for old and new IVID */
    VTSS_RC(vtss_cmn_evc_vlan_update(vtss_state, old_ivid, 1));
    VTSS_RC(vtss_cmn_evc_vlan_update(vtss_state, evc->ivid, evc->learning));
#if defined(VTSS_FEATURE_E_TREE)
    VTSS_RC(vtss_cmn_evc_vlan_update(vtss_state, old_leaf_ivid, 1));
    VTSS_RC(vtss_cmn_evc_vlan_update(vtss_state, evc->leaf_ivid, evc->learning));
#endif /* VTSS_FEATURE_E_TREE */

    /* Delete resources */
    VTSS_FUNC_RC(evc.update, evc_id, &res, VTSS_RES_CMD_DEL);

    /* Add/update resources */
    VTSS_FUNC_RC(evc.update, evc_id, &res, VTSS_RES_CMD_ADD);
    
#if defined(VTSS_FEATURE_HQOS)
    (void) vtss_cmn_evc_hqos_map(vtss_state);
#endif /* VTSS_FEATURE_HQOS */

    return VTSS_RC_OK;
}

/* Determine if ES0 rules are needed for UNIs */
BOOL vtss_cmn_ece_es0_needed(vtss_ece_entry_t *ece)
{
    /* ES0 entry needed for DIR_NNI if outer tag enabled.
       ES0 entry needed for DIR_BOTH if popping enabled. */
    u32 flags = ece->act_flags;

    return ((flags & VTSS_ECE_ACT_OT_ENA) ||
            ((flags & VTSS_ECE_ACT_DIR_ONE) == 0 && (flags & VTSS_ECE_ACT_POP_1)));
}

vtss_ece_dir_t vtss_cmn_ece_dir_get(vtss_ece_entry_t *ece)
{
    u32 flags = ece->act_flags;

    return ((flags & VTSS_ECE_ACT_DIR_UNI_TO_NNI) ? VTSS_ECE_DIR_UNI_TO_NNI :
            (flags & VTSS_ECE_ACT_DIR_NNI_TO_UNI) ? VTSS_ECE_DIR_NNI_TO_UNI :
            VTSS_ECE_DIR_BOTH);
}

#if defined(VTSS_ARCH_SERVAL)
vtss_ece_rule_t vtss_cmn_ece_rule_get(vtss_ece_entry_t *ece)
{
    u32 flags = ece->act_flags;

    return ((flags & VTSS_ECE_ACT_RULE_RX) ? VTSS_ECE_RULE_RX :
            (flags & VTSS_ECE_ACT_RULE_TX) ? VTSS_ECE_RULE_TX : VTSS_ECE_RULE_BOTH);
}
#endif /* VTSS_ARCH_SERVAL */

/* Delete EVC */
vtss_rc vtss_cmn_evc_del(vtss_state_t *vtss_state, const vtss_evc_id_t evc_id)
{
    vtss_evc_entry_t *evc = &vtss_state->evc.info.table[evc_id], evc_zero;
    vtss_ece_entry_t *ece;
    vtss_res_t       res;
    vtss_port_no_t   port_no;
    BOOL             pop_done[VTSS_PORTS];

    VTSS_D("evc_id: %u", evc_id);

    /* Delete resources */
    memset(pop_done, 0, sizeof(pop_done));
    for (ece = vtss_state->evc.ece_info.used; ece != NULL; ece = ece->next) {
        if (ece->evc_id != evc_id)
            continue;
        /* Find deleted UNI/NNI ports */
        vtss_cmn_res_init(&res);
        res.ece_del = 1;
        res.dir_old = vtss_cmn_ece_dir_get(ece);
        res.etree_old = vtss_evc_etree(evc);
#if defined(VTSS_ARCH_SERVAL)
        res.rule_old = vtss_cmn_ece_rule_get(ece);
#endif /* VTSS_ARCH_SERVAL */
        for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {
            if (VTSS_PORT_BF_GET(evc->ports, port_no)) {
                res.port_nni[port_no] = 1;
                res.port_del[port_no] = 1;
            }
            if (VTSS_PORT_BF_GET(ece->ports, port_no)) {
                res.port_del[port_no] = 1;
                if (pop_done[port_no] == 0 && vtss_cmn_ece_es0_needed(ece)) {
                    pop_done[port_no] = 1;
                    res.es0_del[port_no] = 1;
                }
            }
#if defined(VTSS_FEATURE_E_TREE)
            res.port_leaf_old[port_no] = VTSS_PORT_BF_GET(evc->leaf_ports, port_no);
#endif /* VTSS_FEATURE_E_TREE */
        }
        VTSS_FUNC_RC(evc.ece_update, ece, &res, VTSS_RES_CMD_DEL);
    }

    /* Update NNI information */
    vtss_cmn_res_init(&res);
    for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {
        if (VTSS_PORT_BF_GET(evc->ports, port_no)) {
            VTSS_RC(vtss_cmn_evc_port_update(vtss_state, port_no, EVC_PORT_OLD | EVC_PORT_NNI));
            res.port_del[port_no] = 1;
        }
    }
    
    /* Delete EVC */
    res.evc_del = 1;
    VTSS_FUNC_RC(evc.update, evc_id, &res, VTSS_RES_CMD_DEL);
    
    /* Update learning for IVID */
    VTSS_RC(vtss_cmn_evc_vlan_update(vtss_state, evc->ivid, 1));

    /* Save EVC configuration */
    vtss_state->evc.info.count--;
    memset(&evc_zero, 0, sizeof(evc_zero));
#if defined(VTSS_ARCH_SERVAL)
    for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {
        /* Preserve VOE mappings */
        evc_zero.voe_idx[port_no] = evc->voe_idx[port_no];
    }
#endif /* VTSS_ARCH_SERVAL */
#if defined(VTSS_FEATURE_HQOS)
    for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {
        evc_zero.hqos_id[port_no] = VTSS_HQOS_ID_NONE;
    }
#endif /* VTSS_FEATURE_HQOS */

    *evc = evc_zero;

#if defined(VTSS_FEATURE_HQOS)
    (void) vtss_cmn_evc_hqos_map(vtss_state);
#endif /* VTSS_FEATURE_HQOS */

    return VTSS_RC_OK;
}

static void vtss_cmn_ece_flags_set(vtss_ece_entry_t *ece,
                                   vtss_vcap_bit_t fld, u32 mask_vld, u32 mask_1)
{
    if (fld == VTSS_VCAP_BIT_0)
        ece->key_flags |= mask_vld;
    else if (fld == VTSS_VCAP_BIT_1)
        ece->key_flags |= (mask_vld | mask_1);
}

vtss_vcap_bit_t vtss_cmn_ece_bit_get(vtss_ece_entry_t *ece, u32 mask_vld, u32 mask_1)
{
    return ((ece->key_flags & mask_vld) ?
            ((ece->key_flags & mask_1) ? VTSS_VCAP_BIT_1 : VTSS_VCAP_BIT_0) : VTSS_VCAP_BIT_ANY);
}

static vtss_rc vtss_cmn_ece_range_check(vtss_state_t *vtss_state,
                                        vtss_vcap_vr_t *vr, vtss_vcap_range_chk_type_t type, 
                                        const char *name, BOOL no_range)
{
    vtss_vcap_range_chk_table_t range_new;
    u32                         range;

    if (vtss_state->arch == VTSS_ARCH_JR1 || no_range) {
        /* Convert range to value/mask pair */
        memset(&range_new, 0, sizeof(range_new));
        VTSS_RC(vtss_vcap_vr_alloc(&range_new, &range, type, vr));
        if (range != VTSS_VCAP_RANGE_CHK_NONE) {
            VTSS_E("illegal %s range", name);
            return VTSS_RC_ERROR;
        }
    }    
    return VTSS_RC_OK;
}

/* Save ECE in API memory */
static vtss_rc vtss_cmn_ece_save(vtss_state_t *vtss_state,
                                 const vtss_ece_t *const ece, vtss_ece_entry_t *new, 
                                 BOOL port_update)
{
    const vtss_ece_key_t       *key = &ece->key;
    const vtss_ece_tag_t       *tag = &key->tag;
    const vtss_ece_action_t    *action = &ece->action;
    const vtss_ece_outer_tag_t *ot = &action->outer_tag;
#if defined(VTSS_ARCH_JAGUAR_1) || defined(VTSS_ARCH_SERVAL)
    const vtss_ece_inner_tag_t *it = &action->inner_tag;
#endif /* VTSS_ARCH_JAGUAR_1/SERVAL */
    vtss_port_no_t             port_no;
    BOOL                       uni;

    new->ece_id = ece->id;
    new->key_flags = 0;
    new->act_flags = 0;
 
    /* UNI ports */
    for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {
        uni = (key->port_list[port_no] == VTSS_ECE_PORT_NONE ? 0 : 1);
        if (VTSS_PORT_BF_GET(new->ports, port_no)) {
            if (!uni && port_update) {
                VTSS_RC(vtss_cmn_evc_port_update(vtss_state, port_no, EVC_PORT_OLD | EVC_PORT_UNI));
            }
        } else if (uni && port_update) {
            VTSS_RC(vtss_cmn_evc_port_update(vtss_state, port_no, EVC_PORT_NEW | EVC_PORT_UNI));
        }
        VTSS_PORT_BF_SET(new->ports, port_no, uni);
    }

    /* MAC header matching */
#if defined(VTSS_ARCH_CARACAL) || defined(VTSS_ARCH_SERVAL)
    vtss_cmn_ece_flags_set(new, key->mac.dmac_mc,
                           VTSS_ECE_KEY_DMAC_MC_VLD, VTSS_ECE_KEY_DMAC_MC_1);
    
    vtss_cmn_ece_flags_set(new, key->mac.dmac_bc,
                           VTSS_ECE_KEY_DMAC_BC_VLD, VTSS_ECE_KEY_DMAC_BC_1);
    new->smac = key->mac.smac;
#endif /* VTSS_ARCH_CARACAL/SERVAL */
#if defined(VTSS_ARCH_JAGUAR_1) || defined(VTSS_ARCH_SERVAL)
    new->dmac = key->mac.dmac;
#endif /* VTSS_ARCH_JAGUAR_1/SERVAL */
#if defined(VTSS_ARCH_SERVAL)
    if (key->lookup)
        new->key_flags |= VTSS_ECE_KEY_LOOKUP;
#endif /* VTSS_ARCH_SERVAL */

    /* Tag matching */
    new->vid = tag->vid;
    VTSS_RC(vtss_cmn_ece_range_check(vtss_state, &new->vid,
                                     VTSS_VCAP_RANGE_TYPE_VID, "outer VID", 0));
    new->pcp = tag->pcp;
    vtss_cmn_ece_flags_set(new, tag->dei, VTSS_ECE_KEY_TAG_DEI_VLD, VTSS_ECE_KEY_TAG_DEI_1);
    vtss_cmn_ece_flags_set(new, tag->tagged,
                           VTSS_ECE_KEY_TAG_TAGGED_VLD, VTSS_ECE_KEY_TAG_TAGGED_1);
    vtss_cmn_ece_flags_set(new, tag->s_tagged,
                           VTSS_ECE_KEY_TAG_S_TAGGED_VLD, VTSS_ECE_KEY_TAG_S_TAGGED_1);

#if defined(VTSS_ARCH_JAGUAR_1) || defined(VTSS_ARCH_SERVAL)
    /* Inner tag matching */
    tag = &key->inner_tag;
    new->in_vid = tag->vid;
    VTSS_RC(vtss_cmn_ece_range_check(vtss_state, &new->in_vid,
                                     VTSS_VCAP_RANGE_TYPE_VID, "inner VID", 1));
    new->in_pcp = tag->pcp;
    vtss_cmn_ece_flags_set(new, tag->dei,
                           VTSS_ECE_KEY_IN_TAG_DEI_VLD, VTSS_ECE_KEY_IN_TAG_DEI_1);
    vtss_cmn_ece_flags_set(new, tag->tagged,
                           VTSS_ECE_KEY_IN_TAG_TAGGED_VLD, VTSS_ECE_KEY_IN_TAG_TAGGED_1);
    vtss_cmn_ece_flags_set(new, tag->s_tagged,
                           VTSS_ECE_KEY_IN_TAG_S_TAGGED_VLD, VTSS_ECE_KEY_IN_TAG_S_TAGGED_1);
#endif /* VTSS_ARCH_JAGUAR_1/SERVAL */

    /* IP header matching */
    switch (key->type) {
#if defined(VTSS_ARCH_SERVAL)
    case VTSS_ECE_TYPE_ETYPE:
        new->key_flags |= VTSS_ECE_KEY_PROT_ETYPE;
        new->frame.etype = key->frame.etype;
        break;
    case VTSS_ECE_TYPE_LLC:
        new->key_flags |= VTSS_ECE_KEY_PROT_LLC;
        new->frame.llc = key->frame.llc;
        break;
    case VTSS_ECE_TYPE_SNAP:
        new->key_flags |= VTSS_ECE_KEY_PROT_SNAP;
        new->frame.snap = key->frame.snap;
        break;
#endif
    case VTSS_ECE_TYPE_IPV4:
        new->key_flags |= VTSS_ECE_KEY_PROT_IPV4;
        new->frame.ipv4 = key->frame.ipv4;
        VTSS_RC(vtss_cmn_ece_range_check(vtss_state, &new->frame.ipv4.dscp,
                                         VTSS_VCAP_RANGE_TYPE_DSCP, "IPv4 DSCP", 0));
        break;
    case VTSS_ECE_TYPE_IPV6:
        new->key_flags |= VTSS_ECE_KEY_PROT_IPV6;
        new->frame.ipv6 = key->frame.ipv6;
        VTSS_RC(vtss_cmn_ece_range_check(vtss_state, &new->frame.ipv6.dscp,
                                         VTSS_VCAP_RANGE_TYPE_DSCP, "IPv6 DSCP", 0));
        break;
    default:
        break;
    }

    /* Direction and outer tag */
    if (action->dir == VTSS_ECE_DIR_UNI_TO_NNI)
        new->act_flags |= VTSS_ECE_ACT_DIR_UNI_TO_NNI;
    if (action->dir == VTSS_ECE_DIR_NNI_TO_UNI) {
        new->act_flags |= VTSS_ECE_ACT_DIR_NNI_TO_UNI;
        if (ot->enable)
            new->act_flags |= VTSS_ECE_ACT_OT_ENA;
    }
    new->ot_pcp = ot->pcp;
#if defined(VTSS_ARCH_SERVAL)
    if (action->rule == VTSS_ECE_RULE_RX)
        new->act_flags |= VTSS_ECE_ACT_RULE_RX;
    else if (action->rule == VTSS_ECE_RULE_TX)
        new->act_flags |= VTSS_ECE_ACT_RULE_TX;
    if (action->tx_lookup == VTSS_ECE_TX_LOOKUP_VID_PCP)
        new->act_flags |= VTSS_ECE_ACT_TX_LOOKUP_VID_PCP;
    else if (action->tx_lookup == VTSS_ECE_TX_LOOKUP_ISDX)
        new->act_flags |= VTSS_ECE_ACT_TX_LOOKUP_ISDX;
    if (ot->pcp_mode == VTSS_ECE_PCP_MODE_FIXED)
        new->act_flags |= VTSS_ECE_ACT_OT_PCP_MODE_FIXED;
    else if (ot->pcp_mode == VTSS_ECE_PCP_MODE_MAPPED)
        new->act_flags |= VTSS_ECE_ACT_OT_PCP_MODE_MAPPED;
#endif /* VTSS_ARCH_SERVAL */
#if defined(VTSS_ARCH_JAGUAR_1) || defined(VTSS_ARCH_CARACAL)
    if (!ot->pcp_dei_preserve)
        new->act_flags |= VTSS_ECE_ACT_OT_PCP_MODE_FIXED;
#endif /* VTSS_ARCH_JAGUAR_1/CARACAL */
    if (ot->dei)
        new->act_flags |= VTSS_ECE_ACT_OT_DEI;
#if defined(VTSS_ARCH_SERVAL)
    if (ot->dei_mode == VTSS_ECE_DEI_MODE_FIXED)
        new->act_flags |= VTSS_ECE_ACT_OT_DEI_MODE_FIXED;
    else if (ot->dei_mode == VTSS_ECE_DEI_MODE_DP)
        new->act_flags |= VTSS_ECE_ACT_OT_DEI_MODE_DP;
#endif /* VTSS_ARCH_SERVAL */
#if defined(VTSS_ARCH_JAGUAR_1) || defined(VTSS_ARCH_SERVAL)
    new->ot_vid = ot->vid;
#endif /* VTSS_ARCH_JAGUAR_1/SERVAL */

    /* Tag popping */
    if (action->pop_tag == VTSS_ECE_POP_TAG_1)
        new->act_flags |= VTSS_ECE_ACT_POP_1;
    else if (action->pop_tag == VTSS_ECE_POP_TAG_2)
        new->act_flags |= VTSS_ECE_ACT_POP_2;

    /* Inner tag */
#if defined(VTSS_ARCH_JAGUAR_1) || defined(VTSS_ARCH_SERVAL)
    switch (it->type) {
    case VTSS_ECE_INNER_TAG_C:
        new->act_flags |= VTSS_ECE_ACT_IT_TYPE_C;
        break;
    case VTSS_ECE_INNER_TAG_S:
        new->act_flags |= VTSS_ECE_ACT_IT_TYPE_S;
        break;
    case VTSS_ECE_INNER_TAG_S_CUSTOM:
        new->act_flags |= VTSS_ECE_ACT_IT_TYPE_S_CUSTOM;
        break;
    case VTSS_ECE_INNER_TAG_NONE:
    default:
        break;
    }
#if defined(VTSS_ARCH_SERVAL)
    if (it->pcp_mode == VTSS_ECE_PCP_MODE_FIXED)
        new->act_flags |= VTSS_ECE_ACT_IT_PCP_MODE_FIXED;
    else if (it->pcp_mode == VTSS_ECE_PCP_MODE_MAPPED)
        new->act_flags |= VTSS_ECE_ACT_IT_PCP_MODE_MAPPED;
#endif /* VTSS_ARCH_SERVAL */
#if defined(VTSS_ARCH_JAGUAR_1) || defined(VTSS_ARCH_CARACAL)
    if (!it->pcp_dei_preserve)
        new->act_flags |= VTSS_ECE_ACT_IT_PCP_MODE_FIXED;
#endif /* VTSS_ARCH_JAGUAR_1/CARACAL */
    if (it->dei)
        new->act_flags |= VTSS_ECE_ACT_IT_DEI;
#if defined(VTSS_ARCH_SERVAL)
    if (it->dei_mode == VTSS_ECE_DEI_MODE_FIXED)
        new->act_flags |= VTSS_ECE_ACT_IT_DEI_MODE_FIXED;
    else if (it->dei_mode == VTSS_ECE_DEI_MODE_DP)
        new->act_flags |= VTSS_ECE_ACT_IT_DEI_MODE_DP;
#endif /* VTSS_ARCH_SERVAL */
    new->it_vid = it->vid;
    new->it_pcp = it->pcp;

    /* Policer, EVC, policy and priority */
    new->policer_id = action->policer_id;
#endif /* VTSS_ARCH_JAGUAR_1/SERVAL */
    new->evc_id = action->evc_id;
    if (action->policy_no == VTSS_ACL_POLICY_NO_NONE)
        new->act_flags |= VTSS_ECE_ACT_POLICY_NONE;
    new->policy_no = action->policy_no;
#if defined(VTSS_ARCH_CARACAL) || defined(VTSS_ARCH_SERVAL)
    if (action->prio_enable)
        new->act_flags |= VTSS_ECE_ACT_PRIO_ENA;
    new->prio = action->prio;
#endif /* VTSS_ARCH_CARACAL/SERVAL */
#if defined(VTSS_ARCH_SERVAL)
    if (action->dp_enable)
        new->act_flags |= VTSS_ECE_ACT_DP_ENA;
    new->dp = action->dp;
#endif /* VTSS_ARCH_CARACAL/SERVAL */
    
    return VTSS_RC_OK;
}

/* Get EVC, if valid */
vtss_evc_entry_t *vtss_cmn_evc_get(vtss_state_t *vtss_state, vtss_evc_id_t evc_id)
{
    vtss_evc_entry_t *evc = NULL;
    
    if (evc_id != VTSS_EVC_ID_NONE) {
        evc = &vtss_state->evc.info.table[evc_id];
        if (!evc->enable)
            evc = NULL;
    }
    return evc;
}

typedef struct {
    u16 old_cnt[VTSS_PORTS];
    u16 new_cnt[VTSS_PORTS];
} vtss_evc_es0_info_t;

static void vtss_cmn_es0_info_get(vtss_state_t *vtss_state, vtss_evc_es0_info_t *info, 
                                  vtss_evc_id_t new_evc_id, vtss_evc_id_t old_evc_id)
{
    vtss_ece_entry_t *ece;
    vtss_port_no_t   port_no;
    vtss_evc_id_t    evc_id;

    memset(info, 0, sizeof(*info));
    for (ece = vtss_state->evc.ece_info.used; ece != NULL; ece = ece->next) {
        if ((evc_id = ece->evc_id) != VTSS_EVC_ID_NONE &&
            (evc_id == old_evc_id || evc_id == new_evc_id) && 
            vtss_cmn_ece_es0_needed(ece)) {
            for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {
                if (VTSS_PORT_BF_GET(ece->ports, port_no)) {
                    if (evc_id == old_evc_id)
                        info->old_cnt[port_no]++;
                    if (evc_id == new_evc_id)
                        info->new_cnt[port_no]++;
                }
            }
        }
    }
}

/* Add ECE */
vtss_rc vtss_cmn_ece_add(vtss_state_t *vtss_state,
                         const vtss_ece_id_t ece_id, const vtss_ece_t *const ece)
{
    vtss_ece_info_t     *ece_info = &vtss_state->evc.ece_info;
    vtss_evc_id_t       evc_id = ece->action.evc_id, old_evc_id;
    vtss_evc_entry_t    *new_evc, *old_evc;
    vtss_ece_entry_t    *cur, *prev = NULL, new;
    vtss_ece_entry_t    *old = NULL, *old_prev = NULL, *ins = NULL, *ins_prev = NULL;
    vtss_res_t          res;
    vtss_port_no_t      port_no;
    u8                  flags;
    vtss_evc_es0_info_t es0_info;
    
    VTSS_D("ece_id: %u", ece_id);

    /* Check ECE ID */
    if (ece->id == VTSS_ECE_ID_LAST || ece->id == ece_id) {
        VTSS_E("illegal ece id: %u", ece->id);
        return VTSS_RC_ERROR;
    }
    
    /* Search for existing entry and place to add */
    for (cur = ece_info->used; cur != NULL; prev = cur, cur = cur->next) {
        if (cur->ece_id == ece->id) {
            /* Entry already exists */
            old_prev = prev;
            old = cur;
        }
        
        if (cur->ece_id == ece_id) {
            /* Found insertion point */
            ins_prev = prev;
            ins = cur;
        }
    }
    if (ece_id == VTSS_ECE_ID_LAST)
        ins_prev = prev;

    /* Check if the place to insert was found */
    if (ins == NULL && ece_id != VTSS_ECE_ID_LAST) {
        VTSS_E("could not find ece ID: %u", ece_id);
        return VTSS_RC_ERROR;
    }
    
    /* Find and check UNI/NNI port changes */
    vtss_cmn_res_init(&res);

    /* New EVC, if valid */
    if ((new_evc = vtss_cmn_evc_get(vtss_state, evc_id)) != NULL) {
        res.ece_add = 1;
        res.etree_new = vtss_evc_etree(new_evc);
    }
    
    /* Old EVC, if valid */
    old_evc_id = (old == NULL ? VTSS_EVC_ID_NONE : old->evc_id);
    if ((old_evc = vtss_cmn_evc_get(vtss_state, old_evc_id)) != NULL) {
        res.ece_del = 1;
        res.etree_old = vtss_evc_etree(old_evc);
    }
    
    /* Old and new direction */
    if (old != NULL) 
        res.dir_old = vtss_cmn_ece_dir_get(old);
    res.dir_new = ece->action.dir;

#if defined(VTSS_ARCH_SERVAL)
    /* Old and new rule type */
    if (old != NULL)
        res.rule_old = vtss_cmn_ece_rule_get(old);
    res.rule_new = ece->action.rule;
#endif /* VTSS_ARCH_SERVAL */
    
    /* Get POP information */
    vtss_cmn_es0_info_get(vtss_state, &es0_info, evc_id, old_evc_id);

    for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {
        flags = EVC_PORT_NONE;
        if (new_evc != NULL) {
            /* New ECE is using a valid EVC */
            if (ece->key.port_list[port_no] != VTSS_ECE_PORT_NONE) {
                flags |= (EVC_PORT_NEW | EVC_PORT_UNI);
                if (((ece->action.dir == VTSS_ECE_DIR_BOTH && 
                     ece->action.pop_tag == VTSS_ECE_POP_TAG_1) ||
                     (ece->action.dir == VTSS_ECE_DIR_NNI_TO_UNI && 
                      ece->action.outer_tag.enable))
                    && es0_info.new_cnt[port_no] == 0)
                    res.es0_add[port_no] = 1;
            }
            if (VTSS_PORT_BF_GET(new_evc->ports, port_no))
                flags |= (EVC_PORT_NEW | EVC_PORT_NNI);
#if defined(VTSS_FEATURE_E_TREE)
            res.port_leaf_new[port_no] = VTSS_PORT_BF_GET(new_evc->leaf_ports, port_no);
#endif /* VTSS_FEATURE_E_TREE */
        }
        if (old != NULL && old_evc != NULL) {
            /* Old ECE was using a valid EVC */
            if (VTSS_PORT_BF_GET(old->ports, port_no)) {
                flags |= (EVC_PORT_OLD | EVC_PORT_UNI);
                if (vtss_cmn_ece_es0_needed(old) && es0_info.old_cnt[port_no] == 1)
                    res.es0_del[port_no] = 1;
            }
            if (VTSS_PORT_BF_GET(old_evc->ports, port_no))
                flags |= (EVC_PORT_OLD | EVC_PORT_NNI);
#if defined(VTSS_FEATURE_E_TREE)
            res.port_leaf_old[port_no] = VTSS_PORT_BF_GET(old_evc->leaf_ports, port_no);
#endif /* VTSS_FEATURE_E_TREE */
        }
        VTSS_RC(vtss_cmn_res_evc_port_update(vtss_state, &res, port_no, flags));
    }

    /* Calculate resource usage changes */
    VTSS_RC(vtss_cmn_ece_save(vtss_state, ece, &new, 0));
    VTSS_FUNC_RC(evc.ece_update, &new, &res, VTSS_RES_CMD_CALC);
    
    /* Check resource availability */
    VTSS_RC(vtss_cmn_res_check(vtss_state, &res));

    if (old == NULL) {
        /* Take entry from free list */
        if ((old = ece_info->free) == NULL) {
            VTSS_E("no more ECEs");
            return VTSS_RC_ERROR;
        }
        ece_info->free = old->next;
        ece_info->count++;
    } else {
        /* Take existing entry out of list */
        if (ins_prev == old)
            ins_prev = old_prev;
        if (old_prev == NULL)
            ece_info->used = old->next;
        else
            old_prev->next = old->next;
    }

    /* Insert new entry in list */
    if (ins_prev == NULL) {
        old->next = ece_info->used;
        ece_info->used = old;
    } else {
        old->next = ins_prev->next;
        ins_prev->next = old;
    }

    /* Save ECE configuration */
    VTSS_RC(vtss_cmn_ece_save(vtss_state, ece, old, 1));
    
    /* Delete resources */
    VTSS_FUNC_RC(evc.ece_update, old, &res, VTSS_RES_CMD_DEL);

    /* Add/update resources */
    VTSS_FUNC_RC(evc.ece_update, old, &res, VTSS_RES_CMD_ADD);

#if defined(VTSS_FEATURE_HQOS)
    (void) vtss_cmn_evc_hqos_map(vtss_state);
#endif /* VTSS_FEATURE_HQOS */

    return VTSS_RC_OK;
}

/* Delete ECE */
vtss_rc vtss_cmn_ece_del(vtss_state_t *vtss_state, const vtss_ece_id_t ece_id)
{
    vtss_ece_info_t      *ece_info = &vtss_state->evc.ece_info;
    vtss_evc_entry_t     *evc;
    vtss_ece_entry_t     *ece, *prev = NULL;
    vtss_res_t           res;
    vtss_port_no_t       port_no;
    vtss_evc_es0_info_t  es0_info;
    
    VTSS_D("ece_id: %u", ece_id);

    /* Find ECE */
    for (ece = ece_info->used; ece != NULL; prev = ece, ece = ece->next) {
        if (ece->ece_id == ece_id) {
            break;
        }
    }

    /* Check if ECE was found */
    if (ece == NULL) {
        VTSS_E("could not find ece ID: %u", ece_id);
        return VTSS_RC_ERROR;
    }

    /* Delete resources */
    if ((evc = vtss_cmn_evc_get(vtss_state, ece->evc_id)) != NULL) {
        /* Get POP information */
        vtss_cmn_es0_info_get(vtss_state, &es0_info, ece->evc_id, VTSS_EVC_ID_NONE);

        vtss_cmn_res_init(&res);
        res.ece_del = 1;
        res.dir_old = vtss_cmn_ece_dir_get(ece);
#if defined(VTSS_ARCH_SERVAL)
        res.rule_old = vtss_cmn_ece_rule_get(ece);
#endif /* VTSS_ARCH_SERVAL */
        res.etree_old = vtss_evc_etree(evc);
        for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {
            if (VTSS_PORT_BF_GET(evc->ports, port_no)) {
                res.port_del[port_no] = 1;
                res.port_nni[port_no] = 1;
            }
            if (VTSS_PORT_BF_GET(ece->ports, port_no)) {
                res.port_del[port_no] = 1;
                if (vtss_cmn_ece_es0_needed(ece) && es0_info.new_cnt[port_no] == 1)
                    res.es0_del[port_no] = 1;
            }
#if defined(VTSS_FEATURE_E_TREE)
            res.port_leaf_old[port_no] = VTSS_PORT_BF_GET(evc->leaf_ports, port_no);
#endif /* VTSS_FEATURE_E_TREE */
        }
        VTSS_FUNC_RC(evc.ece_update, ece, &res, VTSS_RES_CMD_DEL);
    }
    
    /* Update UNI information */
    for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {
        if (VTSS_PORT_BF_GET(ece->ports, port_no)) {
            VTSS_RC(vtss_cmn_evc_port_update(vtss_state, port_no, EVC_PORT_OLD | EVC_PORT_UNI));
        }
    }
    
    /* Save ECE configuration and move entry from used list to free list */
    ece_info->count--;
    if (prev == NULL)
        ece_info->used = ece->next;
    else
        prev->next = ece->next;
    memset(ece, 0, sizeof(*ece));
    ece->next = ece_info->free;
    ece_info->free = ece;

#if defined(VTSS_FEATURE_HQOS)
    (void) vtss_cmn_evc_hqos_map(vtss_state);
#endif /* VTSS_FEATURE_HQOS */

    return VTSS_RC_OK;
}

#if defined(VTSS_SDX_CNT)
static vtss_rc vtss_cmn_ece_counters_update(vtss_state_t *vtss_state,
                                            const vtss_ece_id_t ece_id, 
                                            const vtss_evc_id_t evc_id,
                                            vtss_port_no_t port_no,
                                            vtss_evc_counters_t *const counters, 
                                            BOOL clear)
{
    vtss_ece_entry_t *ece;
    vtss_sdx_entry_t *sdx;

    /* Search for matching ECE ID or EVC ID */
    for (ece = vtss_state->evc.ece_info.used; ece != NULL; ece = ece->next) {
        if (ece->ece_id == ece_id || (evc_id != VTSS_EVC_ID_NONE && ece->evc_id == evc_id)) {
            /* Found ECE matching ECE ID or EVC ID */
            for (sdx = ece->isdx_list; sdx != NULL; sdx = sdx->next) {
                if (sdx->port_no == port_no) {
                    VTSS_FUNC_RC(evc.sdx_counters_update, sdx, NULL, counters, clear);
                }
            }
            for (sdx = ece->esdx_list; sdx != NULL; sdx = sdx->next) {
                if (sdx->port_no == port_no) {
                    VTSS_FUNC_RC(evc.sdx_counters_update, NULL, sdx, counters, clear);
                }
            }
            if (evc_id == VTSS_EVC_ID_NONE)
                break; /* Found ECE ID */
        }
    }
    if (evc_id != VTSS_EVC_ID_NONE) {
#if defined(VTSS_ARCH_SERVAL)
    vtss_mce_entry_t *mce;
    vtss_port_no_t   port;
    u32              count;

    /* Include ISDX counters for MCEs mapping to EVC */
    for (mce = vtss_state->evc.mce_info.used; mce != NULL; mce = mce->next) {
        if (mce->conf.action.evc_counting && mce->conf.action.evc_id == evc_id) {
            /* We only include statistics if the MCE includes one ingress port.
               Currently, if there are multiple ports, these will share IS1/ISDX. */
            count = 0;
            for (port = VTSS_PORT_NO_START; port < vtss_state->port_count; port++) {
                if (mce->conf.key.port_list[port]) {
                    count++;
                }
            }
            for (sdx = mce->isdx_list; count == 1 && sdx != NULL; sdx = sdx->next) {
                if (sdx->port_no == port_no) {
                    VTSS_FUNC_RC(evc.sdx_counters_update, sdx, NULL, counters, clear);
                    break;
                }
            }
        }
    }
#endif /* VTSS_ARCH_SERVAL */
    } else if (ece == NULL) {
        VTSS_E("ece_id: %u not found", ece_id);
        return VTSS_RC_ERROR;
    }
    return VTSS_RC_OK;
}

vtss_rc vtss_cmn_ece_counters_get(vtss_state_t *vtss_state,
                                  const vtss_ece_id_t  ece_id,
                                  const vtss_port_no_t port_no,
                                  vtss_evc_counters_t  *const counters)
{
    memset(counters, 0, sizeof(*counters));
    return vtss_cmn_ece_counters_update(vtss_state, ece_id, VTSS_EVC_ID_NONE, port_no, counters, 0);
}

vtss_rc vtss_cmn_ece_counters_clear(vtss_state_t *vtss_state,
                                    const vtss_ece_id_t  ece_id,
                                    const vtss_port_no_t port_no)
{
    return vtss_cmn_ece_counters_update(vtss_state, ece_id, VTSS_EVC_ID_NONE, port_no, NULL, 1);
}

vtss_rc vtss_cmn_evc_counters_get(vtss_state_t *vtss_state,
                                  const vtss_evc_id_t  evc_id,
                                  const vtss_port_no_t port_no,
                                  vtss_evc_counters_t  *const counters)
{
    memset(counters, 0, sizeof(*counters));
    return vtss_cmn_ece_counters_update(vtss_state, VTSS_ECE_ID_LAST, evc_id, port_no, counters, 0);
}

vtss_rc vtss_cmn_evc_counters_clear(vtss_state_t *vtss_state,
                                    const vtss_evc_id_t  evc_id,
                                    const vtss_port_no_t port_no)
{
    return vtss_cmn_ece_counters_update(vtss_state, VTSS_ECE_ID_LAST, evc_id, port_no, NULL, 1);
}

#if defined(VTSS_ARCH_SERVAL)
static vtss_rc vtss_cmn_mce_counters_update(vtss_state_t        *vtss_state,
                                            const vtss_mce_id_t mce_id, 
                                            vtss_port_no_t      port_no,
                                            vtss_evc_counters_t *const counters, 
                                            BOOL                clear)
{
    vtss_mce_entry_t *mce;
    vtss_sdx_entry_t *sdx;
    
    /* Search for matching MCE ID or EVC ID */
    for (mce = vtss_state->evc.mce_info.used; mce != NULL; mce = mce->next) {
        if (mce->conf.id == mce_id) {
            /* Found MCE matching MCE ID */
            for (sdx = mce->isdx_list; sdx != NULL; sdx = sdx->next) {
                if (sdx->port_no == port_no) {
                    VTSS_FUNC_RC(evc.sdx_counters_update, sdx, NULL, counters, clear);
                    break;
                }
            }
            for (sdx = mce->esdx_list; sdx != NULL; sdx = sdx->next) {
                if (sdx->port_no == port_no) {
                    VTSS_FUNC_RC(evc.sdx_counters_update, NULL, sdx, counters, clear);
                    break;
                }
            }
        }
    }
    return VTSS_RC_OK;
}

vtss_rc vtss_cmn_mce_counters_get(vtss_state_t         *vtss_state,
                                  const vtss_mce_id_t  mce_id,
                                  const vtss_port_no_t port_no,
                                  vtss_evc_counters_t  *const counters)
{
    memset(counters, 0, sizeof(*counters));
    return vtss_cmn_mce_counters_update(vtss_state, mce_id, port_no, counters, 0);
}

vtss_rc vtss_cmn_mce_counters_clear(vtss_state_t         *vtss_state,
                                    const vtss_mce_id_t  mce_id,
                                    const vtss_port_no_t port_no)
{
    return vtss_cmn_mce_counters_update(vtss_state, mce_id, port_no, NULL, 1);
}
#endif /* VTSS_ARCH_SERVAL */

#endif /* VTSS_SDX_CNT */

#if defined(VTSS_ARCH_CARACAL)
/* Add MCE */
vtss_rc vtss_cmn_mce_add(vtss_state_t *vtss_state,
                         const vtss_mce_id_t mce_id, const vtss_mce_t *const mce)
{
    vtss_vcap_obj_t             *is1_obj = &vtss_state->vcap.is1.obj;
    vtss_vcap_user_t            is1_user = VTSS_IS1_USER_MEP;
    vtss_vcap_data_t            data;
    vtss_is1_data_t             *is1 = &data.u.is1;
    vtss_is1_entry_t            entry;
    vtss_is1_action_t           *action = &entry.action;
    vtss_is1_key_t              *key = &entry.key;
    u32                         old = 0;

    VTSS_D("mce_id: %u", mce_id);

    /* Check MCE ID */
    if (mce->id == VTSS_MCE_ID_LAST || mce->id == mce_id) {
        VTSS_E("illegal mce id: %u", mce->id);
        return VTSS_RC_ERROR;
    }

    /* Initialize entry data */
    vtss_vcap_is1_init(&data, &entry);

    /* First Lookup */
    is1->lookup = 0;

    /* Check if main entry exists */
    if (vtss_vcap_lookup(vtss_state, is1_obj, is1_user, mce->id, &data, NULL) == VTSS_RC_OK) {
        is1->entry = &entry; /* NOTE: Restore entry pointer which was overwritten by vtss_vcap_lookup() */
        old = 1;
    }

    if (is1_obj->count >= (is1_obj->max_count + old)) {
        VTSS_I("IS1 is full");
        return VTSS_RC_ERROR;
    }

    /* Copy action data */
    action->prio_enable = mce->action.prio_enable;
    action->prio        = mce->action.prio;
    action->pag_enable  = TRUE;
    action->pag         = mce->action.policy_no;
    action->pop_enable  = TRUE;
    action->pop         = mce->action.pop_cnt;
    action->vid         = mce->action.vid;

    /* Copy key data */
    memcpy(key->port_list, mce->key.port_list, sizeof(key->port_list));

    key->tag.vid.type              = VTSS_VCAP_VR_TYPE_VALUE_MASK;
    key->tag.vid.vr.v.value        = mce->key.vid.value;
    key->tag.vid.vr.v.mask         = mce->key.vid.mask;

    key->type                       = VTSS_IS1_TYPE_ETYPE;
    key->frame.etype.etype.value[0] = 0x89; /* OAM PDU */
    key->frame.etype.etype.value[1] = 0x02; /* OAM PDU */
    key->frame.etype.etype.mask[0]  = 0xFF;
    key->frame.etype.etype.mask[1]  = 0xFF;
    key->frame.etype.data.value[0]  = mce->key.data.value[0];
    key->frame.etype.data.value[1]  = mce->key.data.value[1];
    key->frame.etype.data.mask[0]   = mce->key.data.mask[0];
    key->frame.etype.data.mask[1]   = mce->key.data.mask[1];

    /* Add main entry */
    VTSS_RC(vtss_vcap_add(vtss_state, is1_obj, is1_user, mce->id, mce_id, &data, 0));
    return VTSS_RC_OK;
}

/* Delete MCE */
vtss_rc vtss_cmn_mce_del(vtss_state_t *vtss_state, const vtss_mce_id_t mce_id)
{
    vtss_vcap_obj_t *obj = &vtss_state->vcap.is1.obj;

    VTSS_D("mce_id: %u", mce_id);

    if (vtss_vcap_lookup(vtss_state, obj, VTSS_IS1_USER_MEP, mce_id, NULL, NULL) != VTSS_RC_OK) {
        VTSS_E("mce_id: %u not found", mce_id);
        return VTSS_RC_ERROR;
    }
    
    /* Delete entry */
    VTSS_RC(vtss_vcap_del(vtss_state, obj, VTSS_IS1_USER_MEP, mce_id));
    
    return VTSS_RC_OK;
}
#endif /* VTSS_ARCH_CARACAL */

/* - Debug print --------------------------------------------------- */

static const char *vtss_debug_ece_flags(vtss_ece_entry_t *ece, u32 mask_vld, u32 mask_1)
{
    return ((ece->key_flags & mask_1) ? "1" : (ece->key_flags & mask_vld) ? "0" : "X");
}

#if defined(VTSS_ARCH_JAGUAR_1) || defined(VTSS_ARCH_CARACAL)
static const char *vtss_debug_ece_pcp_dei(vtss_ece_entry_t *ece, u32 mask_fixed)
{
    return ((ece->act_flags & mask_fixed) ? "Fixed" : "Preserved");
}
#endif /* VTSS_ARCH_JAGUAR_1/CARACAL */

#if defined(VTSS_ARCH_SERVAL)
static const char *vtss_debug_ece_dei_mode(vtss_ece_entry_t *ece, u32 mask_fixed, u32 mask_dp)
{
    u32 flags = ece->act_flags;

    return ((flags & mask_fixed) ? "Fixed" : (flags & mask_dp) ? "DP" : "Classified");
}

static const char *vtss_debug_ece_pcp_mode(vtss_ece_entry_t *ece, u32 mask_fixed, u32 mask_mapped)
{
    u32 flags = ece->act_flags;

    return ((flags & mask_fixed) ? "Fixed" : (flags & mask_mapped) ? "Mapped" : "Classified");
}
#endif /* VTSS_ARCH_SERVAL */

static void vtss_debug_print_port_list(vtss_state_t *vtss_state,
                                       const vtss_debug_printf_t pr, u8 *ports, BOOL nl)
{
    vtss_port_no_t port_no;
    u32            count = 0, max = (vtss_state->port_count - 1);
    BOOL           member, first = 1;

    for (port_no = VTSS_PORT_NO_START; port_no <= max; port_no++) {
        member = VTSS_PORT_BF_GET(ports, port_no);
        if ((member && (count == 0 || port_no == max)) || (!member && count > 1)) {
            pr("%s%u",
               first ? "" : count > (member ? 1 : 2) ? "-" : ",",
               member ? port_no : (port_no - 1));
            first = 0;
        }
        if (member)
            count++;
        else
            count=0;
    }
    if (first)
        pr("None");
    if (nl)
        pr("\n");
}

static void vtss_debug_print_vr(const vtss_debug_printf_t pr, vtss_vcap_vr_t *vr, u32 bits)
{
    vtss_vcap_vr_value_t value, mask, i, zeros = 0, range = 1;

    if (vr->type == VTSS_VCAP_VR_TYPE_VALUE_MASK) {
        value = vr->vr.v.value;
        mask = vr->vr.v.mask;
        pr("0x%x/0x%x", value, mask);

        /* Check if value/mask is a contiguous range */
        for (i = 0; i < bits; i++) {
            if (mask & (1 << (bits - i - 1))) {
                if (zeros)
                    range = 0;
            } else {
                zeros++;
            }
        }
        if (range) {
            pr(" (%u-%u)", value, value + (1 << zeros) - 1);
        }
    } else 
        pr("%u-%u (%s)", vr->vr.r.low, vr->vr.r.high, 
           vr->type == VTSS_VCAP_VR_TYPE_RANGE_INCLUSIVE ? "Incl" : "Excl");
    pr("\n");
}

static void vtss_debug_print_ece_u48(const vtss_debug_printf_t pr, vtss_vcap_u48_t *data)
{
    int i;

    for (i = 0; i < 12; i++) {
        pr("%02x%s",
           i < 6 ? data->value[i] : data->mask[i - 6],
           i == 5 ? "/" : i == 11 ? "\n" : "-");
    }
}

#if defined(VTSS_ARCH_CARACAL) || defined(VTSS_ARCH_SERVAL)
static void vtss_debug_print_ece_ipv6_addr(const vtss_debug_printf_t pr, u8 *p)
{
    int i;

    for (i = 0; i < 16; i++) {
        pr("%02x%s", p[i], i == 15 ? "\n" : (i & 1) ? ":" : "");
    }
}
#endif /* VTSS_ARCH_CARACAL/SERVAL */

#if defined(VTSS_SDX_CNT)
static void vtss_debug_print_sdx_list(vtss_state_t *vtss_state,
                                      const vtss_debug_printf_t pr, 
                                      vtss_sdx_entry_t *isdx_list,
                                      vtss_sdx_entry_t *esdx_list)
{
    vtss_port_no_t   port_no;
    vtss_sdx_entry_t *isdx, *isdx_found;
    vtss_sdx_entry_t *esdx, *esdx_found;
    u32              id, id_min;
    BOOL             first = 1;
    
    /* Traverse in (port, ID) order */
    for (port_no = VTSS_PORT_NO_START; port_no <= vtss_state->port_count; port_no++) {
        if (port_no == vtss_state->port_count)
            port_no = VTSS_PORT_NO_CPU;

        id_min = 0;
        while (1) {
            id = 0xfffff;
            for (isdx = isdx_list, isdx_found = NULL; isdx != NULL; isdx = isdx->next) {
                if (isdx->port_no == port_no && isdx->id >= id_min && isdx->id <= id) {
                    id = isdx->id;
                    isdx_found = isdx;
                }
            }
            for (esdx = esdx_list, esdx_found = NULL; esdx != NULL; esdx = esdx->next) {
                if (esdx->port_no == port_no && esdx->id >= id_min && esdx->id <= id) {
                    id = esdx->id;
                    esdx_found = esdx;
                }
            }
            id_min = (id + 1);
            if (isdx_found == NULL && esdx_found == NULL)
                break;

            pr("%s", first ? "" : ",");
            if (port_no == VTSS_PORT_NO_CPU)
                pr("CPU");
            else
                pr("%u", port_no);
            pr("/%u", id);

            if (isdx_found == NULL)
                pr("/-");
            else
                pr("/%u", isdx_found->sdx);
            if (esdx_found == NULL)
                pr("/-");
            else
                pr("/%u", esdx_found->sdx);
            first = 0;
        }
    }
    pr("%s\n", first ? "-" : "");
}
#endif /* VTSS_SDX_CNT */

#if defined(VTSS_ARCH_SERVAL)
static void vtss_debug_print_mce_tag(const vtss_debug_printf_t pr, vtss_mce_tag_t *tag)
{
    pr("%s %s 0x%03x/%03x 0x%x/%x %s\n", 
       vtss_vcap_bit_txt(tag->tagged), 
       vtss_vcap_bit_txt(tag->s_tagged), 
       tag->vid.value, tag->vid.mask, 
       tag->pcp.value, tag->pcp.mask, 
       vtss_vcap_bit_txt(tag->dei));
}

static void vtss_debug_print_mce_ports(vtss_state_t *vtss_state, const vtss_debug_printf_t pr,
                                       BOOL port_list[VTSS_PORT_ARRAY_SIZE], BOOL port_cpu)
{
    BOOL           found = 0;
    vtss_port_no_t port_no;
    
    for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {
        if (port_list[port_no]) {
            pr("%s%u", found ? "," : "", port_no);
            found = 1;
        }
    }
    if (port_cpu)
        pr("%s%s", found ? "," : "", "CPU");
    else if (!found)
        pr("None");
    pr("\n");
}

#endif /* VTSS_ARCH_SERVAL */

void vtss_evc_debug_print(vtss_state_t *vtss_state,
                          const vtss_debug_printf_t pr,
                          const vtss_debug_info_t   *const info)
{
    u32                   i;
    vtss_port_no_t        port_no;
    vtss_evc_port_info_t  *port_info;
    vtss_evc_port_conf_t  *port_conf;
#if defined(VTSS_SDX_CNT)
    vtss_sdx_info_t       *sdx_info = &vtss_state->evc.sdx_info;
#endif /* VTSS_SDX_CNT */
    vtss_evc_info_t       *evc_info = &vtss_state->evc.info;
    vtss_evc_entry_t      *evc;
    vtss_ece_info_t       *ece_info = &vtss_state->evc.ece_info;
    vtss_ece_entry_t      *ece;
    int                   w = 15;
    BOOL                  header = 1, is_ipv6;
    vtss_ece_frame_ipv4_t *ipv4;
    vtss_ece_frame_ipv6_t *ipv6;
    vtss_vcap_vr_t        vr;
    char                  buf[16];
    
#if defined(VTSS_ARCH_JAGUAR_1)
    vtss_vcap_debug_print_is0(vtss_state, pr, info);
#endif /* VTSS_ARCH_JAGUAR_1 */

#if defined(VTSS_ARCH_CARACAL) || defined(VTSS_ARCH_SERVAL)
    vtss_vcap_debug_print_is1(vtss_state, pr, info);
#endif /* VTSS_ARCH_CARACAL/SERVAL */

    vtss_vcap_debug_print_es0(vtss_state, pr, info);
    
    vtss_vcap_debug_print_range_checkers(vtss_state, pr, info);

    /* Port table */
    for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {
        if (!info->port_list[port_no])
            continue;
        if (header) {
            header = 0;
            vtss_debug_print_header(pr, "Ports");
            pr("Port  UNI   NNI   ");
#if defined(VTSS_ARCH_JAGUAR_1) || defined(VTSS_ARCH_CARACAL)
            pr("DEI Colouring  ");
#endif /* VTSS_ARCH_JAGUAR_1/CARACAL */
#if defined(VTSS_ARCH_CARACAL)
            pr("Tag    ");
#endif /* VTSS_ARCH_CARACAL */
#if defined(VTSS_ARCH_CARACAL) || defined(VTSS_ARCH_SERVAL)
            pr("DMAC/DIP  ");
#endif /* VTSS_ARCH_CARACAL/SERVAL */
#if defined(VTSS_ARCH_SERVAL)
            pr("Key Type     ");
#endif /* VTSS_ARCH_SERVAL */
            pr("\n");
        }
        port_info = &vtss_state->evc.port_info[port_no];
        port_conf = &vtss_state->evc.port_conf[port_no];
        pr("%-6u%-6u%-6u", port_no, port_info->uni_count, port_info->nni_count);
#if defined(VTSS_ARCH_JAGUAR_1) || defined(VTSS_ARCH_CARACAL)
        pr("%-15s", vtss_bool_txt(port_conf->dei_colouring));
#endif /* VTSS_ARCH_JAGUAR_1/CARACAL */
#if defined(VTSS_ARCH_CARACAL)
        pr("%-7s", port_conf->inner_tag ? "Inner" : "Outer");
#endif /* VTSS_ARCH_CARACAL */
#if defined(VTSS_ARCH_CARACAL) || defined(VTSS_ARCH_SERVAL)
        pr("%-10s", vtss_bool_txt(port_conf->dmac_dip));
#endif /* VTSS_ARCH_CARACAL/SERVAL */
#if defined(VTSS_ARCH_SERVAL)
        pr("%-13s", vtss_vcap_key_type2txt(port_conf->key_type));
#endif /* VTSS_ARCH_SERVAL */
        pr("\n");
    }
    if (!header)
        pr("\n");
    header = 1;

    /* Policer table */
    vtss_qos_debug_print_dlb(vtss_state, pr, info);

#if defined(VTSS_SDX_CNT)
    /* SDX table */
    vtss_debug_print_header(pr, "SDX");
    pr("state size: %zu\n", sizeof(*sdx_info));
    pr("max_count : %u\n", sdx_info->max_count);
    pr("ISDX count: %u\n", sdx_info->isdx.count);
    pr("ESDX count: %u\n", sdx_info->esdx.count);
    pr("poll_idx  : %u\n\n", sdx_info->poll_idx);
#endif /* VTSS_SDX_CNT */
    
#if defined(VTSS_ARCH_SERVAL)
    /* VCAP Port table */
    for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {
        if (!info->port_list[port_no])
            continue;
        if (header) {
            header = 0;
            vtss_debug_print_header(pr, "VCAP Port");
            pr("Port  Key Type     Dmac Dip_1\n");
        }
        pr("%-6u%-13s%s\n", port_no,
           vtss_vcap_key_type2txt(vtss_state->vcap.port_conf[port_no].key_type_is1_1),
           (vtss_state->vcap.port_conf[port_no].dmac_dip_1) ? "TRUE" : "FALSE");
    }
    if (!header)
        pr("\n");
    header = 1;
    
    /* MCEs */
    {
        vtss_mce_info_t      *mce_info = &vtss_state->evc.mce_info;
        vtss_mce_entry_t     *mce;
        vtss_mce_key_t       *key;
        vtss_mce_action_t    *action;
        vtss_mce_outer_tag_t *ot;

        vtss_debug_print_header(pr, "MCEs");
        pr("state size: %zu\n", sizeof(*mce_info));
        pr("entry size: %zu\n", sizeof(*mce));
        pr("max_count : %u\n", mce_info->max_count);
        pr("count     : %u\n\n", mce_info->count);

        for (mce = mce_info->used; mce != NULL; mce = mce->next) {
            pr("%-*s: %u\n", w, "MCE ID", mce->conf.id);
            
            /* Key fields */
            key = &mce->conf.key;
            pr("%-*s: ", w, "Rx Port");
            vtss_debug_print_mce_ports(vtss_state, pr, key->port_list, key->port_cpu);
            pr("%-*s: ", w, "Outer Tag");
            vtss_debug_print_mce_tag(pr, &key->tag);
            pr("%-*s: ", w, "Inner Tag");
            vtss_debug_print_mce_tag(pr, &key->inner_tag);
            pr("%-*s: 0x%02x/%02x\n", w, "MEL", key->mel.value, key->mel.mask);
            pr("%-*s: %s\n", w, "Injected", vtss_vcap_bit_txt(key->injected));
            pr("%-*s: %u\n", w, "Lookup", key->lookup);
            pr("%-*s: %s\n", w, "DMAC MC", vtss_vcap_bit_txt(key->dmac_mc));
            pr("%-*s: ", w, "DMAC");
            for (i = 0; i < 12; i++) {
                pr("%02x%s", 
                i < 6 ? key->dmac.value[i] : key->dmac.mask[i - 6],
                i == 5 ? "/" : i == 11 ? "\n" : "-");
            }
            
            /* Action fields */
            action = &mce->conf.action;
            pr("%-*s: ", w, "Tx Port");
            vtss_debug_print_mce_ports(vtss_state, pr, action->port_list, 0);
            pr("%-*s: ", w, "EVC ID");
            if (action->evc_id == VTSS_EVC_ID_NONE)
                pr("-");
            else
                pr("%u", action->evc_id);
            pr("\n");
            pr("%-*s: ", w, "VOE");
            if (action->voe_idx == VTSS_OAM_VOE_IDX_NONE)
                pr("-");
            else
                pr("%u", action->voe_idx);
            pr("\n");
            ot = &action->outer_tag;
            pr("%-*s: %s\n", w, "Egress Tag", vtss_bool_txt(ot->enable));
            pr("%-*s: %u\n", w, "VID", ot->vid);
            pr("%-*s: ", w, "PCP");
            if (ot->pcp_mode == VTSS_MCE_PCP_MODE_FIXED)
                pr("%u", ot->pcp);
            else
                pr("Mapped");
            pr("\n");
            pr("%-*s: ", w, "DEI");
            if (ot->dei_mode == VTSS_MCE_DEI_MODE_FIXED)
                pr("%u", ot->dei);
            else
                pr("DP"); 
            pr("\n");
            pr("%-*s: %s\n", w, "Tx Lookup",
               action->tx_lookup == VTSS_MCE_TX_LOOKUP_VID ? "VID" :
               action->tx_lookup == VTSS_MCE_TX_LOOKUP_ISDX ? "ISDX" : "ISDX_PCP");
            pr("%-*s: %s\n", w, "OAM Detect",
               action->oam_detect == VTSS_MCE_OAM_DETECT_UNTAGGED ? "Untagged" :
               action->oam_detect == VTSS_MCE_OAM_DETECT_SINGLE_TAGGED ? "Single Tagged" :
               action->oam_detect == VTSS_MCE_OAM_DETECT_DOUBLE_TAGGED ? "Double Tagged" : "None");
            pr("%-*s: ", w, "ISDX");
            if (action->isdx == VTSS_MCE_ISDX_NONE)
                pr("NONE");
            else if (action->isdx == VTSS_MCE_ISDX_NEW)
                pr("NEW");
            else
                pr("%u", action->isdx);
            pr("\n");
            pr("%-*s: ", w, "Port/ID/ISDX/ESDX");
            vtss_debug_print_sdx_list(vtss_state, pr, mce->isdx_list, mce->esdx_list);
            pr("%-*s: %u\n", w, "POP Count", action->pop_cnt);
            pr("%-*s: %s\n", w, "Policy", vtss_acl_policy_no_txt(action->policy_no, buf));
            pr("%-*s: ", w, "Priority");
            if (action->prio_enable)
                pr("%u", action->prio);
            else
                pr("-");
            pr("\n");
            pr("%-*s: %u\n", w, "VID", action->vid);
            pr("\n");
        }
    }
#endif /* VTSS_ARCH_SERVAL */

    /* EVCs */
    vtss_debug_print_header(pr, "EVCs");
    pr("state size: %zu\n", sizeof(*evc_info));
    pr("entry size: %zu\n", sizeof(*evc));
    pr("max_count : %u\n", evc_info->max_count);
    pr("count     : %u\n\n", evc_info->count);
    for (i = 0; i < VTSS_EVCS; i++) {
        evc = &evc_info->table[i];
        if (evc->enable == 0)
            continue;
        if (header) {
            header = 0;
            pr("EVC ID  VID/IVID  ");
#if defined(VTSS_FEATURE_E_TREE)
            pr("LVID/LIVID  ");
#endif /* VTSS_FEATURE_E_TREE */
            pr("Learning  ");
#if defined(VTSS_ARCH_CARACAL)
            pr("UVID  Inner Tag           ");
#endif /* VTSS_ARCH_CARACAL */
#if defined(VTSS_ARCH_JAGUAR_1) || defined(VTSS_ARCH_SERVAL)
            pr("Policer  ");
#endif /* VTSS_ARCH_JAGUAR_1/SERVAL */
            pr("NNI Ports");
#if defined(VTSS_FEATURE_E_TREE)
            pr("/Leaf Ports");
#endif /* VTSS_FEATURE_E_TREE */
            pr("\n");
        }
        sprintf(buf, "%u/%u", evc->vid, evc->ivid);
        pr("%-8u%-10s", i, buf);
#if defined(VTSS_FEATURE_E_TREE)
        sprintf(buf, "%u/%u", evc->leaf_vid, evc->leaf_ivid);
        pr("%-12s", buf);
#endif /* VTSS_FEATURE_E_TREE */
        pr("%-10s", vtss_bool_txt(evc->learning));
#if defined(VTSS_ARCH_CARACAL)
        {
            vtss_evc_inner_tag_t *it = &evc->inner_tag;
            pr("%-6u%-4s:%s/0x%03x/%s/%u/%u  ",
               evc->uvid,
               it->type == VTSS_EVC_INNER_TAG_NONE ? "None" :
               it->type == VTSS_EVC_INNER_TAG_C ? "C" :
               it->type == VTSS_EVC_INNER_TAG_S ? "S" :
               it->type == VTSS_EVC_INNER_TAG_S_CUSTOM ? "S-Cu" : "?",
               it->vid_mode == VTSS_EVC_VID_MODE_TUNNEL ? "T" : "N",
               it->vid, it->pcp_dei_preserve ? "P" : "F", it->pcp, it->dei);
        }
#endif /* VTSS_ARCH_CARACAL */
#if defined(VTSS_ARCH_JAGUAR_1) || defined(VTSS_ARCH_SERVAL)
        pr("%-9u", evc->policer_id);
#endif /* VTSS_ARCH_JAGUAR_1/SERVAL */
        vtss_debug_print_port_list(vtss_state, pr, evc->ports, 0);
#if defined(VTSS_FEATURE_E_TREE)
        pr("/");
        vtss_debug_print_port_list(vtss_state, pr, evc->leaf_ports, 0);
#endif /* VTSS_FEATURE_E_TREE */
        pr("\n");
    }
    if (!header)
        pr("\n");

#if defined(VTSS_ARCH_SERVAL)
    /* EVC VOE mappings */
    header = 1;
    for (i = 0; i < VTSS_EVCS; i++) {
        evc = &evc_info->table[i];
        if (evc->enable == 0)
            continue;
        if (header)
            pr("EVC ID  VOEs\n");
        header = 1;
        pr("%-8u", i);
        for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {
            if (evc->voe_idx[port_no] == VTSS_EVC_VOE_IDX_NONE)
                continue;
            pr("%s%u/%u", header ? "" : ",", port_no, evc->voe_idx[port_no]);
            header = 0;
        }
        pr("%s\n", header ? "None" : "");
        header = 0;
    }
    if (!header)
        pr("\n");
#endif /* VTSS_ARCH_SERVAL */

#if defined(VTSS_FEATURE_HQOS)
    /* EVC HQoS mappings */
    header = 1;
    for (i = 0; i < VTSS_EVCS; i++) {
        evc = &evc_info->table[i];
        if (evc->enable == 0)
            continue;
        if (header)
            pr("EVC ID  HQoS IDs\n");
        header = 1;
        pr("%-8u", i);
        for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {
            if (evc->hqos_id[port_no] == VTSS_HQOS_ID_NONE)
                continue;
            pr("%s%u/%u", header ? "" : ",", port_no, evc->hqos_id[port_no]);
            header = 0;
        }
        pr("%s\n", header ? "None" : "");
        header = 0;
    }
    if (!header)
        pr("\n");
#endif /* VTSS_FEATURE_HQOS */

#if defined(VTSS_FEATURE_MPLS)
    /* EVC MPLS pseudo wires */
    header = 1;
    for (i = 0; i < VTSS_EVCS; i++) {
        u32 pw_idx;
        evc = &evc_info->table[i];
        if (evc->enable == 0)
            continue;
        if (header)
            pr("EVC ID  PW/InXC/OutXC/SplitHorizon\n");
        header = 1;
        pr("%-8u", i);
        for (pw_idx = 0; pw_idx < VTSS_ARRSZ(evc->pw) && evc->pw[pw_idx].pw_num; pw_idx++) {
            pr("%s%u/%d/%d/%d", header ? "" : ",",
               evc->pw[pw_idx].pw_num,
               evc->pw[pw_idx].ingress_xc,
               evc->pw[pw_idx].egress_xc,
               evc->pw[pw_idx].split_horizon);
            header = 0;
        }
        pr("%s\n", header ? "None" : "");
        header = 0;
    }
    if (!header)
        pr("\n");
#endif

    /* ECEs */
    vtss_debug_print_header(pr, "ECEs");
    pr("state size: %zu\n", sizeof(*ece_info));
    pr("entry size: %zu\n", sizeof(*ece));
    pr("max_count : %u\n", ece_info->max_count);
    pr("count     : %u\n\n", ece_info->count);
    for (ece = vtss_state->evc.ece_info.used; ece != NULL; ece = ece->next) {
        pr("%-*s: 0x%08x (%u)\n", w, "ECE ID", ece->ece_id, ece->ece_id);

        /* Key fields */
        pr("%-*s: ", w, "UNI Ports");
        vtss_debug_print_port_list(vtss_state, pr, ece->ports, 1);
#if defined(VTSS_ARCH_SERVAL)
        pr("%-*s: %u\n", w, "Lookup", ece->key_flags & VTSS_ECE_KEY_LOOKUP ? 1 : 0);
#endif /* VTSS_ARCH_SERVAL */
#if defined(VTSS_ARCH_CARACAL) || defined(VTSS_ARCH_SERVAL)
        pr("%-*s: %s\n", w, "DMAC MC", 
           vtss_debug_ece_flags(ece, VTSS_ECE_KEY_DMAC_MC_VLD, VTSS_ECE_KEY_DMAC_MC_1));
        pr("%-*s: %s\n", w, "DMAC BC", 
           vtss_debug_ece_flags(ece, VTSS_ECE_KEY_DMAC_BC_VLD, VTSS_ECE_KEY_DMAC_BC_1));
        pr("%-*s: ", w, "SMAC");
        vtss_debug_print_ece_u48(pr, &ece->smac);
#endif /* VTSS_ARCH_CARACAL/SERVAL */
#if defined(VTSS_ARCH_JAGUAR_1) || defined(VTSS_ARCH_SERVAL)
        pr("%-*s: ", w, "DMAC");
        vtss_debug_print_ece_u48(pr, &ece->dmac);
#endif /* VTSS_ARCH_JAGUAR_1/SERVAL */
        pr("%-*s: %s\n", w, "Tagged",
           vtss_debug_ece_flags(ece, VTSS_ECE_KEY_TAG_TAGGED_VLD, VTSS_ECE_KEY_TAG_TAGGED_1));
        pr("%-*s: %s\n", w, "S-Tagged",
           vtss_debug_ece_flags(ece, VTSS_ECE_KEY_TAG_S_TAGGED_VLD, VTSS_ECE_KEY_TAG_S_TAGGED_1));
        pr("%-*s: ", w, "VID");
        vtss_debug_print_vr(pr, &ece->vid, 12);
        pr("%-*s: ", w, "PCP");
        vr.type = VTSS_VCAP_VR_TYPE_VALUE_MASK;
        vr.vr.v.value = ece->pcp.value;
        vr.vr.v.mask = ece->pcp.mask;
        vtss_debug_print_vr(pr, &vr, 3);
        pr("%-*s: %s\n", w, "DEI", 
           vtss_debug_ece_flags(ece, VTSS_ECE_KEY_TAG_DEI_VLD, VTSS_ECE_KEY_TAG_DEI_1));
#if defined(VTSS_ARCH_JAGUAR_1) || defined(VTSS_ARCH_SERVAL)
        pr("%-*s: %s\n", w, "In-Tagged",
           vtss_debug_ece_flags(ece, VTSS_ECE_KEY_IN_TAG_TAGGED_VLD,
                                VTSS_ECE_KEY_IN_TAG_TAGGED_1));
        pr("%-*s: %s\n", w, "In-S-Tagged",
           vtss_debug_ece_flags(ece, VTSS_ECE_KEY_IN_TAG_S_TAGGED_VLD,
                                VTSS_ECE_KEY_IN_TAG_S_TAGGED_1));
        pr("%-*s: ", w, "In-VID");
        vtss_debug_print_vr(pr, &ece->in_vid, 12);
        pr("%-*s: ", w, "In-PCP");
        vr.vr.v.value = ece->in_pcp.value;
        vr.vr.v.mask = ece->in_pcp.mask;
        vtss_debug_print_vr(pr, &vr, 3);
        pr("%-*s: %s\n", w, "In-DEI", 
           vtss_debug_ece_flags(ece, VTSS_ECE_KEY_IN_TAG_DEI_VLD, VTSS_ECE_KEY_IN_TAG_DEI_1));
#endif /* VTSS_ARCH_JAGUAR_1/SERVAL */
        ipv4 = ((ece->key_flags & VTSS_ECE_KEY_PROT_IPV4) ? &ece->frame.ipv4 : NULL);
        is_ipv6 = VTSS_BOOL(ece->key_flags & VTSS_ECE_KEY_PROT_IPV6);
        ipv6 = &ece->frame.ipv6;
        pr("%-*s: %s\n", w, "Frame",
           ece->key_flags & VTSS_ECE_KEY_PROT_ETYPE ? "EType" :
           ece->key_flags & VTSS_ECE_KEY_PROT_LLC ? "LLC" :
           ece->key_flags & VTSS_ECE_KEY_PROT_SNAP ? "SNAP" :
           ipv4 ? "IPv4" :
           is_ipv6 ? "IPv6" :
           "Any");
#if defined(VTSS_ARCH_SERVAL)
        if (ece->key_flags & VTSS_ECE_KEY_PROT_ETYPE) {
            vtss_vcap_u48_t etype;

            pr("%-*s: ", w, "EType");
            for (i = 0; i < 6; i++) {
                etype.value[i] = (i < 2 ? ece->frame.etype.etype.value[i] : ece->frame.etype.data.value[i - 2]);
                etype.mask[i] = (i < 2 ? ece->frame.etype.etype.mask[i] : ece->frame.etype.data.mask[i - 2]);
            }
            vtss_debug_print_ece_u48(pr, &etype);
        } else if (ece->key_flags & VTSS_ECE_KEY_PROT_LLC) {
            pr("%-*s: ", w, "LLC");
            vtss_debug_print_ece_u48(pr, &ece->frame.llc.data);
        } else if (ece->key_flags & VTSS_ECE_KEY_PROT_SNAP){
            pr("%-*s: ", w, "SNAP");
            vtss_debug_print_ece_u48(pr, &ece->frame.snap.data);
        }
#endif /* VTSS_ARCH_SERVAL */
        if (ipv4 || is_ipv6) { /* The is_ipv6 variable is used only to please Lint */
            vtss_vcap_vr_t  *dscp = (ipv4 ? &ipv4->dscp : &ipv6->dscp);
#if defined(VTSS_ARCH_CARACAL) || defined(VTSS_ARCH_SERVAL)
            vtss_vcap_u8_t  *proto = (ipv4 ? &ipv4->proto : &ipv6->proto);
            
            pr("%-*s: 0x%02x/0x%02x\n", w, "Protocol", proto->value, proto->mask);
#endif /* VTSS_ARCH_CARACAL/SERVAL */
            pr("%-*s: ", w, "DSCP");
            vtss_debug_print_vr(pr, dscp, 6);
#if defined(VTSS_ARCH_CARACAL) || defined(VTSS_ARCH_SERVAL)
            if (ipv4) {
                pr("%-*s: %s\n", w, "Fragment", vtss_vcap_bit_txt(ipv4->fragment));
                pr("%-*s: 0x%08x/0x%08x\n", w, "SIP", ipv4->sip.value, ipv4->sip.mask);
#if defined(VTSS_ARCH_SERVAL)
                pr("%-*s: 0x%08x/0x%08x\n", w, "DIP", ipv4->dip.value, ipv4->dip.mask);
#endif /* VTSS_ARCH_SERVAL */
            } else {
                pr("%-*s: ", w, "SIP Value");
                vtss_debug_print_ece_ipv6_addr(pr, ipv6->sip.value);
                pr("%-*s: ", w, "SIP Mask");
                vtss_debug_print_ece_ipv6_addr(pr, ipv6->sip.mask);
#if defined(VTSS_ARCH_SERVAL)
                pr("%-*s: ", w, "DIP Value");
                vtss_debug_print_ece_ipv6_addr(pr, ipv6->dip.value);
                pr("%-*s: ", w, "DIP Mask");
                vtss_debug_print_ece_ipv6_addr(pr, ipv6->dip.mask);
#endif /* VTSS_ARCH_SERVAL */
            }
            for (i = 0; i < 2; i++) {
                vtss_vcap_vr_t  *l4port = (i == 0 ? (ipv4 ? &ipv4->sport : &ipv6->sport) :
                                           (ipv4 ? &ipv4->dport : &ipv6->dport));
                pr("%-*s: ", w, i == 0 ? "SPort" : "DPort");
                vtss_debug_print_vr(pr, l4port, 16);
            }
#endif /* VTSS_ARCH_CARACAL/SERVAL */
        }
        
        /* Action fields */
        pr("%-*s: %s\n", w, "Direction", 
           (ece->act_flags & VTSS_ECE_ACT_DIR_UNI_TO_NNI) ? "UNI_TO_NNI" : 
           (ece->act_flags & VTSS_ECE_ACT_DIR_NNI_TO_UNI) ? "NNI_TO_UNI" : "BOTH");
#if defined(VTSS_ARCH_SERVAL)
        pr("%-*s: %s\n", w, "Rule Type", 
           (ece->act_flags & VTSS_ECE_ACT_RULE_RX) ? "RX" : 
           (ece->act_flags & VTSS_ECE_ACT_RULE_TX) ? "TX" : "BOTH");
        pr("%-*s: %s\n", w, "Tx Lookup", 
           (ece->act_flags & VTSS_ECE_ACT_TX_LOOKUP_VID_PCP) ? "VID_PCP" : 
           (ece->act_flags & VTSS_ECE_ACT_TX_LOOKUP_ISDX) ? "ISDX" : "VID");
#endif /* VTSS_ARCH_SERVCAL */
        pr("%-*s: %u\n", w, "EVC ID", ece->evc_id);
        pr("%-*s: %s\n", w, "Policy",
           vtss_acl_policy_no_txt(ece->act_flags & VTSS_ECE_ACT_POLICY_NONE ?
                                  VTSS_ACL_POLICY_NO_NONE : ece->policy_no, buf));
#if defined(VTSS_ARCH_JAGUAR_1) || defined(VTSS_ARCH_SERVAL)
        pr("%-*s: %u %s\n", w, "Policer",
           ece->policer_id,
           ece->policer_id == VTSS_EVC_POLICER_ID_DISCARD ? "(Discard)" :
           ece->policer_id == VTSS_EVC_POLICER_ID_NONE ? "(None)" :
           ece->policer_id == VTSS_EVC_POLICER_ID_EVC ? "(EVC)" : "");
#endif /* VTSS_ARCH_JAGUAR_1/SERVAL */
#if defined(VTSS_ARCH_CARACAL) || defined(VTSS_ARCH_SERVAL)
        pr("%-*s: ", w, "Priority");
        if (ece->act_flags & VTSS_ECE_ACT_PRIO_ENA)
            pr("%u", ece->prio);
        else
            pr("-");
        pr("\n");
#endif /* VTSS_ARCH_CARACAL/SERVAL */
#if defined(VTSS_ARCH_SERVAL)
        pr("%-*s: ", w, "DP");
        if (ece->act_flags & VTSS_ECE_ACT_DP_ENA)
            pr("%u", ece->dp);
        else
            pr("-");
        pr("\n");
#endif /* VTSS_ARCH_SERVCAL */
        pr("%-*s: %u\n", w, "Pop Count", 
           (ece->act_flags & VTSS_ECE_ACT_POP_2) ? 2 : 
           (ece->act_flags & VTSS_ECE_ACT_POP_1) ? 1 : 0);
        pr("%-*s: %s\n", w, "Outer Tag", 
           vtss_bool_txt(ece->act_flags & VTSS_ECE_ACT_OT_ENA ? 1 : 0)); 
#if defined(VTSS_ARCH_JAGUAR_1) || defined(VTSS_ARCH_SERVAL)
        pr("%-*s: %u\n", w, "Outer VID", ece->ot_vid);
#endif /* VTSS_ARCH_JAGUAR_1/SERVAL */
#if defined(VTSS_ARCH_SERVAL)
        pr("%-*s: %s\n", w, "Outer PCP Mode", 
           vtss_debug_ece_pcp_mode(ece, VTSS_ECE_ACT_OT_PCP_MODE_FIXED,
                                   VTSS_ECE_ACT_OT_PCP_MODE_MAPPED));
#endif /* VTSS_ARCH_SERVAL */
#if defined(VTSS_ARCH_JAGUAR_1) || defined(VTSS_ARCH_CARACAL)
        pr("%-*s: %s\n", w, "Outer PCP/DEI",
           vtss_debug_ece_pcp_dei(ece, VTSS_ECE_ACT_OT_PCP_MODE_FIXED));
#endif /* VTSS_ARCH_JAGUAR_1/CARACAL */
        pr("%-*s: %u\n", w, "Outer PCP", ece->ot_pcp);
#if defined(VTSS_ARCH_SERVAL)
        pr("%-*s: %s\n", w, "Outer DEI Mode", 
           vtss_debug_ece_dei_mode(ece, VTSS_ECE_ACT_OT_DEI_MODE_FIXED,
                                   VTSS_ECE_ACT_OT_DEI_MODE_DP));
#endif /* VTSS_ARCH_SERVAL */
        pr("%-*s: %u\n", w, "Outer DEI", ece->act_flags & VTSS_ECE_ACT_OT_DEI ? 1 : 0);
#if defined(VTSS_ARCH_JAGUAR_1) || defined(VTSS_ARCH_SERVAL)
        pr("%-*s: %s\n", w, "Inner Tag", 
           ece->act_flags & VTSS_ECE_ACT_IT_TYPE_C ? "C" :
           ece->act_flags & VTSS_ECE_ACT_IT_TYPE_S ? "S" :
           ece->act_flags & VTSS_ECE_ACT_IT_TYPE_S_CUSTOM ? "S-Custom" : "None");
        pr("%-*s: %u\n", w, "Inner VID", ece->it_vid);
#if defined(VTSS_ARCH_SERVAL)
        pr("%-*s: %s\n", w, "Inner PCP Mode", 
           vtss_debug_ece_pcp_mode(ece, VTSS_ECE_ACT_IT_PCP_MODE_FIXED,
                                   VTSS_ECE_ACT_IT_PCP_MODE_MAPPED));
#endif /* VTSS_ARCH_SERVAL */
#if defined(VTSS_ARCH_JAGUAR_1) || defined(VTSS_ARCH_CARACAL)
        pr("%-*s: %s\n", w, "Inner PCP/DEI",
           vtss_debug_ece_pcp_dei(ece, VTSS_ECE_ACT_IT_PCP_MODE_FIXED));
#endif /* VTSS_ARCH_JAGUAR_1/CARACAL */
        pr("%-*s: %u\n", w, "Inner PCP", ece->it_pcp);
#if defined(VTSS_ARCH_SERVAL)
        pr("%-*s: %s\n", w, "Inner DEI Mode", 
           vtss_debug_ece_dei_mode(ece, VTSS_ECE_ACT_IT_DEI_MODE_FIXED,
                                   VTSS_ECE_ACT_IT_DEI_MODE_DP));
#endif /* VTSS_ARCH_SERVAL */
        pr("%-*s: %u\n", w, "Inner DEI", ece->act_flags & VTSS_ECE_ACT_IT_DEI ? 1 : 0);
        pr("%-*s: ", w, "Port/ID/ISDX/ESDX");
        vtss_debug_print_sdx_list(vtss_state, pr, ece->isdx_list, ece->esdx_list);
#endif /* VTSS_ARCH_JAGUAR_1/SERVAL */
        pr("\n");
    }
}
#endif /* VTSS_FEATURE_EVC */
