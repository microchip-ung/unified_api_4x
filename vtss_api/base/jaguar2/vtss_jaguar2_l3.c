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

#define VTSS_TRACE_GROUP VTSS_TRACE_GROUP_L3
#include "vtss_jaguar2_cil.h"

#if defined(VTSS_ARCH_JAGUAR_2) && defined(VTSS_FEATURE_LAYER3)

/* - CIL functions ------------------------------------------------- */

/* L3 counter indices */
#define JR2_L3_CNT_IP_UC_PACKETS 0
#define JR2_L3_CNT_IP_UC_BYTES   1

static vtss_rc jr2_l3_common_set(vtss_state_t *vtss_state,
                                 const vtss_l3_common_conf_t * const conf)
{
    const u8 *addr = conf->base_address.addr;
    u32      type_sel = 2;
    u32      msb = ((addr[0] << 16) | (addr[1] << 8) | addr[2]);
    u32      lsb = ((addr[3] << 16) | (addr[4] << 8) | addr[5]);
    u64      port_mask = 0xffffffffffffffff;
    
    /* Setup router leg MAC address and type */
    JR2_WR(VTSS_ANA_L3_COMMON_RLEG_CFG_0, VTSS_F_ANA_L3_COMMON_RLEG_CFG_0_RLEG_MAC_LSB(lsb));
    JR2_WR(VTSS_ANA_L3_COMMON_RLEG_CFG_1,
           VTSS_F_ANA_L3_COMMON_RLEG_CFG_1_RLEG_MAC_TYPE_SEL(type_sel) |
           VTSS_F_ANA_L3_COMMON_RLEG_CFG_1_RLEG_MAC_MSB(msb));
    JR2_WR(VTSS_REW_COMMON_RLEG_CFG_0, VTSS_F_REW_COMMON_RLEG_CFG_0_RLEG_MAC_LSB(lsb));
    JR2_WR(VTSS_REW_COMMON_RLEG_CFG_1,
           VTSS_F_REW_COMMON_RLEG_CFG_1_RLEG_MAC_TYPE_SEL(type_sel) |
           VTSS_F_REW_COMMON_RLEG_CFG_1_RLEG_MAC_MSB(msb));

    /* Enable/disable routing */
    JR2_WR_PMASK(VTSS_ANA_L3_COMMON_L3_UC_ENA, port_mask);

    return VTSS_RC_OK;
}

static vtss_rc jr2_l3_rleg_counter_update(vtss_state_t *vtss_state,
                                          BOOL rx, u32 i, u32 j, u64 *prev, u64 *counter)
{
    vtss_chip_counter_t chip_counter;
    u32                 lsb, msb;

    /* Copy counter state */
    chip_counter.prev = *prev;
    chip_counter.value = *counter;

    if (rx) {
        /* Read IRLEG counter */
        JR2_RD(VTSS_ANA_AC_STAT_CNT_CFG_IRLEG_STAT_LSB_CNT(i, j), &lsb);
        JR2_RD(VTSS_ANA_AC_STAT_CNT_CFG_IRLEG_STAT_MSB_CNT(i, j), &msb);
    } else {
        /* Read ERLEG counter */
        JR2_RD(VTSS_ANA_AC_STAT_CNT_CFG_ERLEG_STAT_LSB_CNT(i, j), &lsb);
        JR2_RD(VTSS_ANA_AC_STAT_CNT_CFG_ERLEG_STAT_MSB_CNT(i, j), &msb);
    }

    /* Update counter */
    vtss_cmn_counter_40_update(lsb, msb, &chip_counter, FALSE);
    *prev = chip_counter.prev;
    *counter = chip_counter.value;

    return VTSS_RC_OK;
}

static vtss_rc jr2_l3_rleg_hw_stat_poll(vtss_state_t *vtss_state,
                                        vtss_l3_rleg_id_t rleg)
{
    vtss_l3_counters_t *prev = &vtss_state->l3.statistics.interface_shadow_counter[rleg];
    vtss_l3_counters_t *counter = &vtss_state->l3.statistics.interface_counter[rleg];

    /* IPv4 counters */
    VTSS_RC(jr2_l3_rleg_counter_update(vtss_state, TRUE, rleg, JR2_L3_CNT_IP_UC_PACKETS,
                                       &prev->ipv4uc_received_frames, &counter->ipv4uc_received_frames));
    VTSS_RC(jr2_l3_rleg_counter_update(vtss_state, TRUE, rleg, JR2_L3_CNT_IP_UC_BYTES,
                                       &prev->ipv4uc_received_octets, &counter->ipv4uc_received_octets));
    VTSS_RC(jr2_l3_rleg_counter_update(vtss_state, FALSE, rleg, JR2_L3_CNT_IP_UC_PACKETS,
                                       &prev->ipv4uc_transmitted_frames, &counter->ipv4uc_transmitted_frames));
    VTSS_RC(jr2_l3_rleg_counter_update(vtss_state, FALSE, rleg, JR2_L3_CNT_IP_UC_BYTES,
                                       &prev->ipv4uc_transmitted_octets, &counter->ipv4uc_transmitted_octets));

    /* IPv6 counters */
    rleg += VTSS_RLEG_CNT;
    VTSS_RC(jr2_l3_rleg_counter_update(vtss_state, TRUE, rleg, JR2_L3_CNT_IP_UC_PACKETS,
                                       &prev->ipv6uc_received_frames, &counter->ipv6uc_received_frames));
    VTSS_RC(jr2_l3_rleg_counter_update(vtss_state, TRUE, rleg, JR2_L3_CNT_IP_UC_BYTES,
                                       &prev->ipv6uc_received_octets, &counter->ipv6uc_received_octets));
    VTSS_RC(jr2_l3_rleg_counter_update(vtss_state, FALSE, rleg, JR2_L3_CNT_IP_UC_PACKETS,
                                       &prev->ipv6uc_transmitted_frames, &counter->ipv6uc_transmitted_frames));
    VTSS_RC(jr2_l3_rleg_counter_update(vtss_state, FALSE, rleg, JR2_L3_CNT_IP_UC_BYTES,
                                       &prev->ipv6uc_transmitted_octets, &counter->ipv6uc_transmitted_octets));

    return VTSS_RC_OK;
}

static vtss_rc jr2_l3_rleg_stat_reset(vtss_state_t *vtss_state)
{
    u32 i, j;

    for (i = 0; i < (2*VTSS_RLEG_CNT); i++) {
        for (j = 0; j < 2; j++) {
            JR2_WR(VTSS_ANA_AC_STAT_CNT_CFG_IRLEG_STAT_MSB_CNT(i, j), 0);
            JR2_WR(VTSS_ANA_AC_STAT_CNT_CFG_IRLEG_STAT_LSB_CNT(i, j), 0);
            JR2_WR(VTSS_ANA_AC_STAT_CNT_CFG_ERLEG_STAT_MSB_CNT(i, j), 0);
            JR2_WR(VTSS_ANA_AC_STAT_CNT_CFG_ERLEG_STAT_LSB_CNT(i, j), 0);
        }
    }
    return VTSS_RC_OK;
}

static vtss_rc jr2_l3_rleg_set(vtss_state_t *vtss_state,
                               const vtss_l3_rleg_id_t   rleg,
                               const vtss_l3_rleg_conf_t *const conf)
{
    BOOL vrid_enable = (conf->vrid0_enable || conf->vrid1_enable);
    u32  i, vrid;
    
    JR2_WR(VTSS_ANA_L3_VMID_RLEG_CTRL(rleg),
           VTSS_F_ANA_L3_VMID_RLEG_CTRL_RLEG_EVID(conf->vlan) |
           VTSS_F_ANA_L3_VMID_RLEG_CTRL_RLEG_IP6_UC_ENA(conf->ipv6_unicast_enable) |
           VTSS_F_ANA_L3_VMID_RLEG_CTRL_RLEG_IP4_UC_ENA(conf->ipv4_unicast_enable) |
           VTSS_F_ANA_L3_VMID_RLEG_CTRL_RLEG_IP6_ICMP_REDIR_ENA(conf->ipv6_icmp_redirect_enable) |
           VTSS_F_ANA_L3_VMID_RLEG_CTRL_RLEG_IP4_ICMP_REDIR_ENA(conf->ipv4_icmp_redirect_enable) |
           VTSS_F_ANA_L3_VMID_RLEG_CTRL_RLEG_IP6_VRID_ENA(vrid_enable) |
           VTSS_F_ANA_L3_VMID_RLEG_CTRL_RLEG_IP4_VRID_ENA(vrid_enable));

    /* If only one VRID is enabled, both are set to the same value */
    for (i = 0; i < 2; i++) {
        vrid = (i == 0 ? (conf->vrid0_enable ? conf->vrid0 : conf->vrid1) :
                (conf->vrid1_enable ? conf->vrid1 : conf->vrid0));
        JR2_WR(VTSS_ANA_L3_VMID_VRRP_CFG(rleg, i),
               VTSS_F_ANA_L3_VMID_VRRP_CFG_RLEG_IP6_VRID(vrid) |
               VTSS_F_ANA_L3_VMID_VRRP_CFG_RLEG_IP4_VRID(vrid));
    }

    JR2_WR(VTSS_REW_VMID_RLEG_CTRL(rleg), VTSS_F_REW_VMID_RLEG_CTRL_RLEG_EVID(conf->vlan));

    return VTSS_RC_OK;
}

static vtss_rc jr2_l3_vlan_set(vtss_state_t *vtss_state,
                               const vtss_l3_rleg_id_t rleg_id,
                               const vtss_vid_t        vid,
                               const BOOL              enable)
{
    JR2_WRM(VTSS_ANA_L3_VLAN_VLAN_CFG(vid), 
            VTSS_F_ANA_L3_VLAN_VLAN_CFG_VLAN_RLEG_ENA(enable),
            VTSS_M_ANA_L3_VLAN_VLAN_CFG_VLAN_RLEG_ENA);
    JR2_WR(VTSS_ANA_L3_VLAN_VMID_CFG(vid), VTSS_F_ANA_L3_VLAN_VMID_CFG_VMID(rleg_id));

    return VTSS_RC_OK;
}

static void jr2_l3_lpm_entry_init(vtss_vcap_data_t *data, vtss_lpm_entry_t *entry, u32 remap_ptr, BOOL ecmp)
{
    vtss_lpm_action_t *action = &entry->action;

    /* Initialize LPM entry */
    memset(data, 0, sizeof(*data));
    memset(entry, 0, sizeof(*entry));
    data->u.lpm.entry = entry;
    action->type = LPM_TYPE_ARP_PTR;
    action->data.arp_ptr.arp_ptr = remap_ptr;
    action->data.arp_ptr.ecmp_cnt = (ecmp ? 1 : 0);
}

static vtss_rc jr2_l3_lpm_entry_add(vtss_state_t *vtss_state, vtss_vcap_data_t *data, u32 row)
{
    vtss_vcap_obj_t     *obj = &vtss_state->vcap.lpm.obj;
    int                 user = VTSS_LPM_USER_L3;
    u32                 id_next = VTSS_VCAP_ID_LAST;
    vtss_l3_lpm_entry_t *lpm;

    /* Find next VCAP ID */
    if (row != 0) {
        /* The LPM table in the API state is sorted with shortest prefix first,
           so we reverse the ordering by using the previous LPM entry as next ID */
        lpm = &vtss_state->l3.lpm.e[row - 1];
        if (lpm->valid && (vtss_vcap_lookup(vtss_state, obj, user, lpm->vcap_id, NULL, NULL) == VTSS_RC_OK)) {
            id_next = lpm->vcap_id;
        }
    }
    lpm = &vtss_state->l3.lpm.e[row];
    VTSS_D("idx: %u, vcap_id: %u", row, lpm->vcap_id);
    return vtss_vcap_add(vtss_state, obj, user, lpm->vcap_id, id_next, data, 0);
}

static vtss_rc jr2_l3_lpm_entry_del(vtss_state_t *vtss_state, u32 row)
{
    vtss_l3_lpm_entry_t *lpm = &vtss_state->l3.lpm.e[row];

    if (lpm->valid) {
        VTSS_D("idx: %u, vcap_id: %u", row, lpm->vcap_id);
        return vtss_vcap_del(vtss_state, &vtss_state->vcap.lpm.obj, VTSS_LPM_USER_L3, lpm->vcap_id);
    }
    VTSS_N("idx: %u invalid, ignored", row);
    return VTSS_RC_OK;
}

static vtss_rc jr2_l3_ipv4_uc_set(vtss_state_t *vtss_state,
                                  const u32 row, const u32 remap_ptr,
                                  const BOOL ecmp, const u32 addr, const u32 mask)
{
    vtss_vcap_data_t data;
    vtss_lpm_entry_t entry;
    vtss_lpm_key_t   *key = &entry.key;

    /* Add/update VCAP entry */
    jr2_l3_lpm_entry_init(&data, &entry, remap_ptr, ecmp);
    data.key_size = VTSS_VCAP_KEY_SIZE_SIXTEENTH;
    key->type = VTSS_LPM_KEY_SGL_IP4;
    key->data.sgl_ip4.dst_ena = 1;
    key->data.sgl_ip4.xip.value = addr;
    key->data.sgl_ip4.xip.mask = mask;
    VTSS_D("row: %u, addr: 0x%08x, mask: 0x%08x", row, addr, mask);
    return jr2_l3_lpm_entry_add(vtss_state, &data, row);
}

static vtss_rc jr2_l3_ipv4_uc_clear(vtss_state_t *vtss_state, const u32 row)
{
    return jr2_l3_lpm_entry_del(vtss_state, row);
}

static vtss_rc jr2_l3_ipv6_uc_set(vtss_state_t *vtss_state, 
                                  const u32 row, const u32 remap_ptr,
                                  const BOOL ecmp, const u32 a0, const u32 a1, const u32 a2,
                                  const u32 a3, const u32 m0, const u32 m1, const u32 m2, const u32 m3)
{
    vtss_vcap_data_t data;
    vtss_lpm_entry_t entry;
    vtss_lpm_key_t   *key = &entry.key;
    u32              i, j, a, m;

    /* Add/update VCAP entry */
    jr2_l3_lpm_entry_init(&data, &entry, remap_ptr, ecmp);
    data.key_size = VTSS_VCAP_KEY_SIZE_QUARTER;
    key->type = VTSS_LPM_KEY_SGL_IP6;
    key->data.sgl_ip6.dst_ena = 1;
    for (i = 0; i < 16; i++) {
        if (i < 4) {
            a = a0;
            m = m0;
        } else if (i < 8) {
            a = a1;
            m = m1;
        } else if (i < 12) {
            a = a2;
            m = m2;
        } else {
            a = a3;
            m = m3;
        }
        j = (24 - (i & 3)*8);
        key->data.sgl_ip6.xip.value[i] = (a >> j);
        key->data.sgl_ip6.xip.mask[i] = (m >> j);
    }
    VTSS_D("row: %u, addr: 0x%08x%08x%08x%08x, mask: 0x%08x%08x%08x%08x", row, a0, a1, a2, a3, m0, m1, m2, m3);
    return jr2_l3_lpm_entry_add(vtss_state, &data, row);
}

static vtss_rc jr2_l3_ipv6_uc_clear(vtss_state_t *vtss_state, const u32 row)
{
    return jr2_l3_lpm_entry_del(vtss_state, row);
}

static vtss_rc jr2_l3_ipv4_mc_clear(vtss_state_t *vtss_state, const u32 row)
{
    return VTSS_RC_OK;
}

static vtss_rc jr2_l3_lpm_move(vtss_state_t *vtss_state,
                               const u32 idx, const u32 size,
                               const u32 distance, const BOOL down)
{
    vtss_l3_lpm_entry_t *lpm;

    if (down) {
        /* Ignore move-down operations related to insertions as this is handled by VCAP library */
        VTSS_D("DOWN, ignored");
        return VTSS_RC_OK;
    }

    /* Move-up operation related to deletions */
    if (idx == 0) {
        VTSS_D("UP, idx zero ignored");
        return VTSS_RC_OK;
    }

    lpm = &vtss_state->l3.lpm.e[idx - 1];
    if (lpm->valid == 0) {
        VTSS_D("UP, invalid ignored");
        return VTSS_RC_OK;
    }

    VTSS_D("UP, idx: %u, size: %u, dist: %u, valid: %u, vcap_id: %u", idx, size, distance, lpm->valid, lpm->vcap_id);

    return vtss_vcap_del(vtss_state, &vtss_state->vcap.lpm.obj, VTSS_LPM_USER_L3, lpm->vcap_id);
}

static vtss_rc jr2_l3_arp_set(vtss_state_t *vtss_state,
                              const u32 row, const vtss_mac_t * const mac,
                              const vtss_vid_t rleg)
{
    const u8 *addr = mac->addr;
    u32      msb = ((addr[0] << 8) | addr[1]);
    u32      lsb = ((addr[2] << 24) | (addr[3] << 16) | (addr[4] << 8) | addr[5]);

    JR2_WR(VTSS_ANA_L3_ARP_ARP_CFG_0(row),
           VTSS_F_ANA_L3_ARP_ARP_CFG_0_MAC_MSB(msb) |
           VTSS_F_ANA_L3_ARP_ARP_CFG_0_ARP_VMID(rleg) |
           VTSS_F_ANA_L3_ARP_ARP_CFG_0_ARP_ENA(1));
    JR2_WR(VTSS_ANA_L3_ARP_ARP_CFG_1(row), lsb);

    return VTSS_RC_OK;
}

static vtss_rc jr2_l3_arp_clear(vtss_state_t *vtss_state, const u32 row)
{
    JR2_WR(VTSS_ANA_L3_ARP_ARP_CFG_0(row), 0);
    JR2_WR(VTSS_ANA_L3_ARP_ARP_CFG_1(row), 0);

    return VTSS_RC_OK;
}

static vtss_rc jr2_l3_debug_sticky_clear(vtss_state_t *vtss_state)
{
    u32 value = 0xffffffff;

    JR2_WR(VTSS_ANA_L3_LPM_REMAP_STICKY_L3_LPM_REMAP_STICKY, value);
    JR2_WR(VTSS_ANA_L3_VLAN_ARP_L3MC_STICKY_VLAN_STICKY, value);
    JR2_WR(VTSS_ANA_L3_VLAN_ARP_L3MC_STICKY_L3_ARP_IPMC_STICKY, value);

    return VTSS_RC_OK;
}

/* - Debug print --------------------------------------------------- */

static void jr2_l3_debug_cnt(const vtss_debug_printf_t pr, const char *name, u64 rx_cnt, u64 tx_cnt)
{
    vtss_chip_counter_t c1, c2;

    c1.value = rx_cnt;
    c2.value = tx_cnt;

    vtss_jr2_debug_cnt(pr, name, "", &c1, &c2);
}

vtss_rc vtss_jr2_l3_debug_print(vtss_state_t *vtss_state,
                                const vtss_debug_printf_t pr,
                                const vtss_debug_info_t   *const info)
{
    BOOL               header = 1;
    vtss_vid_t         vid;
    u32                i, value, enable, cfg0, cfg1;
    char               buf0[16], buf1[16];
    vtss_l3_counters_t *cnt;

    if (!vtss_debug_group_enabled(pr, info, VTSS_DEBUG_GROUP_L3)) {
        return VTSS_RC_OK;
    }

    JR2_RD(VTSS_ANA_L3_COMMON_RLEG_CFG_0, &cfg0);
    JR2_RD(VTSS_ANA_L3_COMMON_RLEG_CFG_1, &cfg1);
    pr("Router MAC: %06x-%06x (Type %u)\n\n",
       VTSS_X_ANA_L3_COMMON_RLEG_CFG_1_RLEG_MAC_MSB(cfg1),
       VTSS_X_ANA_L3_COMMON_RLEG_CFG_0_RLEG_MAC_LSB(cfg0),
       VTSS_X_ANA_L3_COMMON_RLEG_CFG_1_RLEG_MAC_TYPE_SEL(cfg1));
    
    for (i = 0; i < VTSS_RLEG_CNT; i++) {
        if (vtss_state->l3.rleg_conf[i].vlan == 0 && !info->full) {
            continue;
        }
        
        if (header) {
            pr("RLEG  VID   IP4_UC  IP6_UC  IP4_REDIR  IP6_REDIR  IP4_VRID     IP6_VRID\n");
            header = 0;
        }
        JR2_RD(VTSS_ANA_L3_VMID_RLEG_CTRL(i), &value);
        JR2_RD(VTSS_ANA_L3_VMID_VRRP_CFG(i, 0), &cfg0);
        JR2_RD(VTSS_ANA_L3_VMID_VRRP_CFG(i, 1), &cfg1);
        sprintf(buf0, "%u (%u/%u)",
                VTSS_X_ANA_L3_VMID_RLEG_CTRL_RLEG_IP4_VRID_ENA(value),
                VTSS_X_ANA_L3_VMID_VRRP_CFG_RLEG_IP4_VRID(cfg0),
                VTSS_X_ANA_L3_VMID_VRRP_CFG_RLEG_IP4_VRID(cfg1));
        sprintf(buf1, "%u (%u/%u)",
                VTSS_X_ANA_L3_VMID_RLEG_CTRL_RLEG_IP6_VRID_ENA(value),
                VTSS_X_ANA_L3_VMID_VRRP_CFG_RLEG_IP6_VRID(cfg0),
                VTSS_X_ANA_L3_VMID_VRRP_CFG_RLEG_IP6_VRID(cfg1));
        pr("%-6u%-6u%-8u%-8u%-11u%-11u%-13s%-13s\n",
           i,
           VTSS_X_ANA_L3_VMID_RLEG_CTRL_RLEG_EVID(value),
           VTSS_X_ANA_L3_VMID_RLEG_CTRL_RLEG_IP4_UC_ENA(value),
           VTSS_X_ANA_L3_VMID_RLEG_CTRL_RLEG_IP6_UC_ENA(value),
           VTSS_X_ANA_L3_VMID_RLEG_CTRL_RLEG_IP4_ICMP_REDIR_ENA(value),
           VTSS_X_ANA_L3_VMID_RLEG_CTRL_RLEG_IP6_ICMP_REDIR_ENA(value),
           buf0,
           buf1);
    }
    if (!header) {
        pr("\n");
        header = 1;
    }

    for (vid = VTSS_VID_NULL; vid < VTSS_VIDS; vid++) {
        if (!vtss_state->l2.vlan_table[vid].enabled && !info->full) {
            continue;
        }
        
        if (header) {
            pr("VID   RLEG_ENA  VMID\n");
            header = 0;
        }
        JR2_RD(VTSS_ANA_L3_VLAN_VLAN_CFG(vid), &value);
        enable = VTSS_X_ANA_L3_VLAN_VLAN_CFG_VLAN_RLEG_ENA(value);
        JR2_RD(VTSS_ANA_L3_VLAN_VMID_CFG(vid), &value);
        pr("%-6u%-10u%-4u\n", vid, enable, VTSS_X_ANA_L3_VLAN_VMID_CFG_VMID(value));
        
        /* Leave critical region briefly */
        VTSS_EXIT_ENTER();
    }
    if (!header) {
        pr("\n");
        header = 1;
    }

    VTSS_RC(vtss_jr2_debug_lpm(vtss_state, pr, info));

    for (i = 0; i < VTSS_ARP_CNT; i++) {
        JR2_RD(VTSS_ANA_L3_ARP_ARP_CFG_0(i), &cfg0);
        if (VTSS_X_ANA_L3_ARP_ARP_CFG_0_ARP_ENA(cfg0) == 0 && !info->full) {
            continue;
        }

        JR2_RD(VTSS_ANA_L3_ARP_ARP_CFG_1(i), &cfg1);
        if (header) {
            pr("IDX   MAC           VMID\n");
            header = 0;
        }
        pr("%-6u%04x%08x  %u\n",
           i,
           VTSS_X_ANA_L3_ARP_ARP_CFG_0_MAC_MSB(cfg0),
           cfg1,
           VTSS_X_ANA_L3_ARP_ARP_CFG_0_ARP_VMID(cfg0));
    }
    if (!header) {
        pr("\n");
    }

    for (i = 0; i < VTSS_RLEG_CNT; i++) {
        if ((vtss_state->l3.rleg_conf[i].vlan == 0 && !info->full) ||
            jr2_l3_rleg_hw_stat_poll(vtss_state, i) != VTSS_RC_OK) {
            continue;
        }

        pr("RLEG %u counters:\n\n", i);
        cnt = &vtss_state->l3.statistics.interface_counter[i];
        jr2_l3_debug_cnt(pr, "ipv4_uc_packets", cnt->ipv4uc_received_frames, cnt->ipv4uc_transmitted_frames);
        jr2_l3_debug_cnt(pr, "ipv4_uc_bytes", cnt->ipv4uc_received_octets, cnt->ipv4uc_transmitted_octets);
        jr2_l3_debug_cnt(pr, "ipv6_uc_packets", cnt->ipv6uc_received_frames, cnt->ipv6uc_transmitted_frames);
        jr2_l3_debug_cnt(pr, "ipv6_uc_bytes", cnt->ipv6uc_received_octets, cnt->ipv6uc_transmitted_octets);
        pr("\n");
    }

    if (info->clear || info->full) {
        /* Read and clear sticky bits */
        vtss_jr2_debug_reg_header(pr, "ANA_L3:STICKY");
        vtss_jr2_debug_sticky(vtss_state, pr, VTSS_ANA_L3_LPM_REMAP_STICKY_L3_LPM_REMAP_STICKY, "LPM_REMAP_STICKY");
        vtss_jr2_debug_sticky(vtss_state, pr, VTSS_ANA_L3_VLAN_ARP_L3MC_STICKY_VLAN_STICKY, "VLAN_STICKY");
        vtss_jr2_debug_sticky(vtss_state, pr, VTSS_ANA_L3_VLAN_ARP_L3MC_STICKY_L3_ARP_IPMC_STICKY, "ARP_IPMC_STICKY");
    }

    return VTSS_RC_OK;
}

/* - Initialization ------------------------------------------------ */

static vtss_rc jr2_l3_init_counter(vtss_state_t *vtss_state, u32 idx, u32 cnt_byte)
{
    u32 frame_type = 1;     /* Events with no FCS errors */

    /* IRLEG counters */
    JR2_WR(VTSS_ANA_AC_STAT_GLOBAL_CFG_IRLEG_GLOBAL_CNT_FRM_TYPE_CFG(idx),
           VTSS_F_ANA_AC_STAT_GLOBAL_CFG_IRLEG_GLOBAL_CNT_FRM_TYPE_CFG_GLOBAL_CFG_CNT_FRM_TYPE(frame_type));
    JR2_WR(VTSS_ANA_AC_STAT_GLOBAL_CFG_IRLEG_STAT_GLOBAL_CFG(idx),
           VTSS_F_ANA_AC_STAT_GLOBAL_CFG_IRLEG_STAT_GLOBAL_CFG_GLOBAL_CFG_CNT_BYTE(cnt_byte));
    JR2_WR(VTSS_ANA_AC_STAT_GLOBAL_CFG_IRLEG_STAT_GLOBAL_EVENT_MASK(idx),
           VTSS_F_ANA_AC_STAT_GLOBAL_CFG_IRLEG_STAT_GLOBAL_EVENT_MASK_GLOBAL_EVENT_MASK(1 << 3)); /* IP unicast */

    /* ERLEG counters */
    JR2_WR(VTSS_ANA_AC_STAT_GLOBAL_CFG_ERLEG_GLOBAL_CNT_FRM_TYPE_CFG(idx),
           VTSS_F_ANA_AC_STAT_GLOBAL_CFG_ERLEG_GLOBAL_CNT_FRM_TYPE_CFG_GLOBAL_CFG_CNT_FRM_TYPE(frame_type));
    JR2_WR(VTSS_ANA_AC_STAT_GLOBAL_CFG_ERLEG_STAT_GLOBAL_CFG(idx),
           VTSS_F_ANA_AC_STAT_GLOBAL_CFG_ERLEG_STAT_GLOBAL_CFG_GLOBAL_CFG_CNT_BYTE(cnt_byte));
    JR2_WR(VTSS_ANA_AC_STAT_GLOBAL_CFG_ERLEG_STAT_GLOBAL_EVENT_MASK(idx),
           VTSS_F_ANA_AC_STAT_GLOBAL_CFG_ERLEG_STAT_GLOBAL_EVENT_MASK_GLOBAL_EVENT_MASK(1 << 1)); /* IP unicast */

    return VTSS_RC_OK;
}

static vtss_rc jr2_l3_init(vtss_state_t *vtss_state)
{
    VTSS_RC(jr2_l3_init_counter(vtss_state, JR2_L3_CNT_IP_UC_PACKETS, 0));
    VTSS_RC(jr2_l3_init_counter(vtss_state, JR2_L3_CNT_IP_UC_BYTES, 1));

    return VTSS_RC_OK;
}

static vtss_rc jr2_l3_poll(vtss_state_t *vtss_state)
{
    /* Poll counters for one router leg every second to avoid counter wrapping.
       The worst case is a 40-bit byte counter, which would wrap in about 900 seconds at 10 Gbps */
    VTSS_RC(jr2_l3_rleg_hw_stat_poll(vtss_state, vtss_state->l3.statistics.rleg));
    vtss_state->l3.statistics.rleg++;
    if (vtss_state->l3.statistics.rleg >= VTSS_RLEG_CNT) {
        vtss_state->l3.statistics.rleg = 0;
    }
    return VTSS_RC_OK;
}

vtss_rc vtss_jr2_l3_init(vtss_state_t *vtss_state, vtss_init_cmd_t cmd)
{
    vtss_l3_state_t *state = &vtss_state->l3;
    
    switch (cmd) {
    case VTSS_INIT_CMD_CREATE:
        state->common_set = jr2_l3_common_set;
        state->rleg_counters_get = jr2_l3_rleg_hw_stat_poll;
        state->rleg_counters_reset = jr2_l3_rleg_stat_reset;
        state->rleg_set = jr2_l3_rleg_set;
        state->vlan_set = jr2_l3_vlan_set;
        state->arp_set = jr2_l3_arp_set;
        state->arp_clear = jr2_l3_arp_clear;
        state->ipv4_uc_set = jr2_l3_ipv4_uc_set;
        state->ipv6_uc_set = jr2_l3_ipv6_uc_set;
        state->ipv4_uc_clear = jr2_l3_ipv4_uc_clear;
        state->ipv4_mc_clear = jr2_l3_ipv4_mc_clear;
        state->ipv6_uc_clear = jr2_l3_ipv6_uc_clear;
        state->lpm_move = jr2_l3_lpm_move;
        state->debug_sticky_clear = jr2_l3_debug_sticky_clear;
        vtss_l3_integrity_update(vtss_state);
        break;
    case VTSS_INIT_CMD_INIT:
        vtss_l3_integrity_check(vtss_state, __FILE__, __LINE__);
        VTSS_RC(jr2_l3_init(vtss_state));
        vtss_l3_integrity_update(vtss_state);
        break;
    case VTSS_INIT_CMD_POLL:
        vtss_l3_integrity_check(vtss_state, __FILE__, __LINE__);
        VTSS_RC(jr2_l3_poll(vtss_state));
        vtss_l3_integrity_update(vtss_state);
        break;
    default:
        break;
    }
    return VTSS_RC_OK;
}

#endif /* VTSS_ARCH_JAGUAR_2 && VTSS_FEATURE_LAYER3 */
