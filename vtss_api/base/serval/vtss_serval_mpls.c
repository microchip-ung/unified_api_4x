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

#define VTSS_TRACE_GROUP VTSS_TRACE_GROUP_MPLS
#include "vtss_serval_cil.h"

#if defined(VTSS_ARCH_SERVAL) && defined(VTSS_FEATURE_MPLS)

#pragma GCC diagnostic ignored "-Wchar-subscripts"


/* NOTE: The following functions are part of the core MPLS code, but since they are used by
 *       the raw MPLS encapsulation code which is "independent" of MPLS, we need them here
 *       as well.
 */

#define VTSS_MPLS_CHECK(chk)    do { if (!(chk)) { VTSS_DG(VTSS_TRACE_GROUP_MPLS, "Check failed: " #chk); return VTSS_RC_ERROR; } } while (0)
#define VTSS_MPLS_CHECK_RC(chk) do { if (VTSS_RC_OK != (chk)) { VTSS_DG(VTSS_TRACE_GROUP_MPLS, "RC check failed: " #chk); return VTSS_RC_ERROR; } } while (0)

static inline void srvl_mpls_out_encap_set_bit(u32 *bits, u32 offset, u8 val)
{
    u8 *data = (u8*)bits;
    u8 shift = offset % 8;
    data += (320-offset-1)/8;
    *data = (*data & ~VTSS_BIT(shift)) | (val ? VTSS_BIT(shift) : 0);
}

static void srvl_mpls_out_encap_set_bits(u32 *bits, u32 offset, u32 width, u32 value)
{
    while (width > 0) {
        srvl_mpls_out_encap_set_bit(bits, offset, value & 0x01);
        width--;
        offset++;
        value >>= 1;
    }
}

static vtss_es0_mpls_encap_len_t srvl_bytes_to_encap_len(u16 bytes)
{
    switch (bytes) {
    case 0:
        return VTSS_ES0_MPLS_ENCAP_LEN_NONE;
    case 14:
        return VTSS_ES0_MPLS_ENCAP_LEN_14;
    case 18:
        return VTSS_ES0_MPLS_ENCAP_LEN_18;
    case 22:
        return VTSS_ES0_MPLS_ENCAP_LEN_22;
    case 26:
        return VTSS_ES0_MPLS_ENCAP_LEN_26;
    case 30:
        return VTSS_ES0_MPLS_ENCAP_LEN_30;
    case 34:
        return VTSS_ES0_MPLS_ENCAP_LEN_34;
    default:
        VTSS_DG(VTSS_TRACE_GROUP_MPLS, "Unmatched encap byte count: %u", bytes);
        return VTSS_ES0_MPLS_ENCAP_LEN_NONE;
    }
}

/* ----------------------------------------------------------------- *
 * Raw MPLS encapsulation.
 *
 * WARNING: Special use, not used by other MPLS functionality.
 *
 * WARNING: Serval-specific!
 * ----------------------------------------------------------------- */

vtss_rc vtss_srvl_mpls_out_encap_raw_set(vtss_state_t *vtss_state,
                                         const u32 idx,
                                         const vtss_srvl_mpls_out_encap_raw_t *const entry,
                                         vtss_es0_mpls_encap_len_t *length)
{
    const int word_cnt = (entry->length + 6 + 3) / 4;      // The first 6 bytes are for re-marking configuration; all-zero. 3 to round up if necessary.

    u32  bits[word_cnt];    // MSB is bit 31 in [0]
    int  i;
    u32  offset = 272 - entry->length * 8;

    *length = srvl_bytes_to_encap_len(entry->length);

    if (idx == 0  ||  idx > VTSS_MPLS_OUT_ENCAP_CNT) {
        VTSS_DG(VTSS_TRACE_GROUP_MPLS, "Invalid MPLS out-segment encap idx (%u)", idx);
        return VTSS_RC_ERROR;
    }

    memset(bits, 0, sizeof(bits));

    for (i = entry->length - 1; i >= 0; i--) {
        srvl_mpls_out_encap_set_bits(bits, offset, 8, entry->data[i]);
        offset += 8;
    }

    for (i = 0; i < word_cnt; i++) {
        u32 val = VTSS_OS_NTOHL(bits[i]);
        VTSS_DG(VTSS_TRACE_GROUP_MPLS, "%2d   0x%08x   0x%08x", i, val, bits[i]);
        SRVL_WR(VTSS_SYS_ENCAPSULATIONS_ENCAP_DATA(i), val);
    }

    SRVL_WR(VTSS_SYS_ENCAPSULATIONS_ENCAP_CTRL, VTSS_F_SYS_ENCAPSULATIONS_ENCAP_CTRL_ENCAP_ID(idx) | VTSS_F_SYS_ENCAPSULATIONS_ENCAP_CTRL_ENCAP_WR);

    return VTSS_RC_OK;
}

/* - CIL functions ------------------------------------------------- */

#define MPLS_RAL_LABEL_VALUE       (1)
#define MPLS_GAL_LABEL_VALUE       (13)

#define MPLS_CPU_Q                 (3)    // OAM and other frame extractions go via the same CPU queue


/*
 * Internal MPLS utilities
 */

#define VTSS_MPLS_CHECK(chk)    do { if (!(chk)) { VTSS_DG(VTSS_TRACE_GROUP_MPLS, "Check failed: " #chk); return VTSS_RC_ERROR; } } while (0)
#define VTSS_MPLS_CHECK_RC(chk) do { if (VTSS_RC_OK != (chk)) { VTSS_DG(VTSS_TRACE_GROUP_MPLS, "RC check failed: " #chk); return VTSS_RC_ERROR; } } while (0)

#define VTSS_MPLS_TAKE_FROM_FREELIST(freelist, table, idx) \
    do { \
        (idx) = vtss_state->freelist; \
        if (VTSS_MPLS_IDX_IS_UNDEF((idx))) { \
            VTSS_DG(VTSS_TRACE_GROUP_MPLS, "No free " #table " entries"); \
            VTSS_MPLS_IDX_UNDEF((idx)); \
            return VTSS_RC_ERROR; \
        } \
        vtss_state->freelist = vtss_state->table[idx].next_free; \
        VTSS_MPLS_IDX_UNDEF(vtss_state->table[idx].next_free); \
    } while (0)

#define VTSS_MPLS_RETURN_TO_FREELIST(freelist, table, idx) \
    vtss_state->table[idx].next_free = vtss_state->freelist; \
    vtss_state->freelist = (idx);

#if 0
// Not used yet, so disabled due to lint
static BOOL srvl_mpls_hw_avail_mll(u32 cnt)
{
    return (vtss_state->mpls.is0_mll_cnt + cnt <= VTSS_MPLS_L2_CNT)  &&
           (vtss_state->mpls.is0_mll_cnt + cnt + vtss_state->mpls.is0_mlbs_cnt / 2 <= SRVL_IS0_CNT);
}

static BOOL srvl_mpls_hw_avail_mlbs(u32 cnt)
{
    return (vtss_state->mpls.is0_mlbs_cnt + cnt <= VTSS_MPLS_LSP_CNT)  &&
           (vtss_state->mpls.is0_mll_cnt + (vtss_state->mpls.is0_mlbs_cnt + cnt) / 2 <= SRVL_IS0_CNT);
}

static BOOL srvl_mpls_hw_avail_encap(u32 cnt)
{
    return (vtss_state->mpls.encap_cnt + cnt) <= VTSS_MPLS_OUT_ENCAP_CNT;
}

static BOOL srvl_mpls_hw_avail_vprofile(u32 cnt)
{
    return (vtss_state->mpls.vprofile_cnt + cnt) <= VTSS_MPLS_VPROFILE_CNT;
}
#endif

static vtss_rc srvl_mpls_sdx_alloc(vtss_state_t *vtss_state,
                                   BOOL isdx, vtss_port_no_t port_no, vtss_sdx_entry_t **entry)
{
    *entry = isdx ?
        vtss_cmn_isdx_alloc_low(vtss_state, port_no) :
        vtss_cmn_sdx_alloc(vtss_state, port_no, 0, FALSE);

    if (*entry == NULL) {
        VTSS_DG(VTSS_TRACE_GROUP_MPLS, "SDX alloc FAILURE");
        return VTSS_RC_ERROR;
    }

    if (isdx && (*entry)->sdx >= 512) {
        VTSS_EG(VTSS_TRACE_GROUP_MPLS, "ISDX alloc returned value >= 512; not usable for MPLS (got %d)", (*entry)->sdx);
        vtss_cmn_sdx_free(vtss_state, *entry, isdx);
        *entry = NULL;
        return VTSS_RC_ERROR;
    }

    VTSS_DG(VTSS_TRACE_GROUP_MPLS, "SDX alloc success: %d", (*entry)->sdx);
    return VTSS_RC_OK;
}

static vtss_rc srvl_mpls_sdx_free(vtss_state_t *vtss_state, BOOL isdx, vtss_sdx_entry_t *entry)
{
    VTSS_DG(VTSS_TRACE_GROUP_MPLS, "SDX free: %d", entry->sdx);
    vtss_cmn_sdx_free(vtss_state, entry, isdx);
    return VTSS_RC_OK;
}

/* ----------------------------------------------------------------- *
 *  VProfile functions (with regards to MPLS processing)
 * ----------------------------------------------------------------- */

static vtss_rc srvl_mpls_vprofile_hw_update(vtss_state_t *vtss_state, vtss_mpls_vprofile_idx_t idx)
{
    u32                  v, m, i;
    vtss_mpls_vprofile_t *vp;

    if (idx >= VTSS_MPLS_VPROFILE_CNT) {
        return VTSS_RC_ERROR;
    }

    vp = &VP_P(idx);

    VTSS_NG(VTSS_TRACE_GROUP_MPLS, "VProfile update, entry %d (port %d)", idx, vp->port);

    SRVL_WRM(VTSS_ANA_PORT_VCAP_CFG(idx),    vp->s1_s2_enable ? VTSS_F_ANA_PORT_VCAP_CFG_S1_ENA    : 0, VTSS_F_ANA_PORT_VCAP_CFG_S1_ENA);
    SRVL_WRM(VTSS_ANA_PORT_VCAP_S2_CFG(idx), vp->s1_s2_enable ? VTSS_F_ANA_PORT_VCAP_S2_CFG_S2_ENA : 0, VTSS_F_ANA_PORT_VCAP_S2_CFG_S2_ENA);

    v = (vp->recv_enable       ? VTSS_F_ANA_PORT_PORT_CFG_RECV_ENA  : 0)      |
        (vp->learn_enable      ? VTSS_F_ANA_PORT_PORT_CFG_LEARN_ENA : 0)      |
        (vp->src_mirror_enable ? VTSS_F_ANA_PORT_PORT_CFG_SRC_MIRROR_ENA : 0) |
        VTSS_F_ANA_PORT_PORT_CFG_PORTID_VAL(VTSS_CHIP_PORT(vp->port));
    m = VTSS_F_ANA_PORT_PORT_CFG_RECV_ENA       |
        VTSS_F_ANA_PORT_PORT_CFG_LEARN_ENA      |
        VTSS_F_ANA_PORT_PORT_CFG_SRC_MIRROR_ENA |
        VTSS_M_ANA_PORT_PORT_CFG_PORTID_VAL;
    SRVL_WRM(VTSS_ANA_PORT_PORT_CFG(idx), v, m);

    SRVL_WRM(VTSS_ANA_PORT_VLAN_CFG(idx), vp->vlan_aware ? VTSS_F_ANA_PORT_VLAN_CFG_VLAN_AWARE_ENA : 0, VTSS_F_ANA_PORT_VLAN_CFG_VLAN_AWARE_ENA);

    v = (vp->map_dscp_cos_enable ? VTSS_F_ANA_PORT_QOS_CFG_QOS_DSCP_ENA : 0) |
        (vp->map_eth_cos_enable  ? VTSS_F_ANA_PORT_QOS_CFG_QOS_PCP_ENA  : 0);
    m = VTSS_F_ANA_PORT_QOS_CFG_QOS_DSCP_ENA |
        VTSS_F_ANA_PORT_QOS_CFG_QOS_PCP_ENA;
    SRVL_WRM(VTSS_ANA_PORT_QOS_CFG(idx), v, m);

    // Disable port policer for this VProfile (leaves real port policer untouched but unused).
    SRVL_WR(VTSS_ANA_PORT_POL_CFG(idx), 0);

    // Metal fix 4:
    SRVL_WRM(VTSS_ANA_PORT_PTP_DLY1_CFG(idx), vp->ptp_dly1_enable ? VTSS_BIT(31) : 0, VTSS_BIT(31));

    // Copy S1 key lookup config from phys port for all three lookups. The physical port settings may be changed by EVC
    for (i = 0; i < 3; i++) {
        SRVL_RD(VTSS_ANA_PORT_VCAP_S1_KEY_CFG(VTSS_CHIP_PORT(vp->port), i), &v);
        SRVL_WR(VTSS_ANA_PORT_VCAP_S1_KEY_CFG(idx, i), v);
    }

    // Copy S2 key lookup config as well, but maintain the S2_ENA bit from the VProfile
    SRVL_RD(VTSS_ANA_PORT_VCAP_S2_CFG(VTSS_CHIP_PORT(vp->port)), &v);
    SRVL_WRM(VTSS_ANA_PORT_VCAP_S2_CFG(idx), v, ~VTSS_F_ANA_PORT_VCAP_S2_CFG_S2_ENA);

    // And copy VCAP settings for S1, but maintain the S1_ENA bit from the VProfile
    SRVL_RD(VTSS_ANA_PORT_VCAP_CFG(VTSS_CHIP_PORT(vp->port)), &v);
    SRVL_WRM(VTSS_ANA_PORT_VCAP_CFG(idx), v, ~VTSS_F_ANA_PORT_VCAP_CFG_S1_ENA);

    return VTSS_RC_OK;
}

// S1/S2 settings changed for physical port, so PW VProfiles must be sync'd to keep EVCs and EVC OAM working
vtss_rc vtss_srvl_mpls_vprofile_pw_port_sync(vtss_state_t *vtss_state, vtss_port_no_t port)
{
    vtss_mpls_vprofile_idx_t i;
    u32                      v, vprofile_chip_port, chip_port = VTSS_CHIP_PORT(port);

    VTSS_NG(VTSS_TRACE_GROUP_MPLS, "VProfile refresh for port %d", port);

    // We sync for all VProfiles that are configured for the port in question, no matter if
    // they're in use or not; it's safe and simpler

    for (i = VTSS_MPLS_VPROFILE_RESERVED_CNT; i < VTSS_MPLS_VPROFILE_CNT - 1; i++) {
        SRVL_RD(VTSS_ANA_PORT_PORT_CFG(i), &v);
        vprofile_chip_port = VTSS_X_ANA_PORT_PORT_CFG_PORTID_VAL(v);
        if (chip_port == vprofile_chip_port) {
            (void) srvl_mpls_vprofile_hw_update(vtss_state, i);
        }
    }
    return VTSS_RC_OK;
}

static vtss_rc srvl_mpls_vprofile_init(vtss_state_t *vtss_state)
{
    vtss_mpls_vprofile_idx_t i;
    vtss_mpls_vprofile_t     *vp;
    u32                      v;
    u32                      dei, pcp;

    VTSS_DG(VTSS_TRACE_GROUP_MPLS, "Static VProfile config for LSR (entry %u) and OAM (entry %u)", VTSS_MPLS_VPROFILE_LSR_IDX, VTSS_MPLS_VPROFILE_OAM_IDX);

    // TN1135/MPLS, sect. 3.3.3, 3.3.4: Static LSR, OAM entries:

    vp = &VP_P(VTSS_MPLS_VPROFILE_LSR_IDX);
    vp->s1_s2_enable        = FALSE;
    vp->recv_enable         = TRUE;
    vp->ptp_dly1_enable     = FALSE;
    vp->vlan_aware          = FALSE;
    vp->map_dscp_cos_enable = FALSE;
    vp->map_eth_cos_enable  = FALSE;
    (void) srvl_mpls_vprofile_hw_update(vtss_state, VTSS_MPLS_VPROFILE_LSR_IDX);

    vp = &VP_P(VTSS_MPLS_VPROFILE_OAM_IDX);
    vp->s1_s2_enable        = FALSE;
    vp->recv_enable         = FALSE;
    vp->ptp_dly1_enable     = FALSE;
    vp->vlan_aware          = FALSE;
    vp->map_dscp_cos_enable = FALSE;
    vp->map_eth_cos_enable  = FALSE;
    (void) srvl_mpls_vprofile_hw_update(vtss_state, VTSS_MPLS_VPROFILE_OAM_IDX);

    // Set up VProfile free-list. The reserved entries, of course, aren't in:
    for (i = VTSS_MPLS_VPROFILE_RESERVED_CNT; i < VTSS_MPLS_VPROFILE_CNT - 1; i++) {
        vtss_state->mpls.vprofile_conf[i].next_free = i + 1;
    }
    vtss_state->mpls.vprofile_conf[VTSS_MPLS_VPROFILE_CNT - 1].next_free = VTSS_MPLS_IDX_UNDEFINED;
    vtss_state->mpls.vprofile_free_list = VTSS_MPLS_VPROFILE_RESERVED_CNT;

    // TN1135/MPLS, sect. 3.3.1: Use the above OAM VProfile for OAM instead of that from the MLBS action.
    // Same for MLBS misses and MLBS errors.
    v = VTSS_F_ANA_MPLS_CL_EXCP_VPROFILE_CONF_OAM_VPROFILE(VTSS_MPLS_VPROFILE_OAM_IDX) |
        VTSS_F_ANA_MPLS_CL_EXCP_VPROFILE_CONF_MLBS_MISS_VPROFILE(VTSS_MPLS_VPROFILE_OAM_IDX) |
        VTSS_F_ANA_MPLS_CL_EXCP_VPROFILE_CONF_MLBS_ERR_VPROFILE(VTSS_MPLS_VPROFILE_OAM_IDX);
    SRVL_WR(VTSS_ANA_MPLS_CL_EXCP_VPROFILE_CONF, v);

    // If a PW is configured with TC tunnel mode "Short Pipe", then we must use the payload (PCP, DEI) as (QoS-class, DP).
    // In order to do so, we'll enable the per-VProfile ANA:PORT:QOS_CFG.QOS_PCP_ENA option. This takes the classified (PCP, DEI)
    // from basic classification and maps it to (QoS-class, DP). The map default values are all zero so we must initialize them
    // here.

    for (i = VTSS_MPLS_VPROFILE_RESERVED_CNT; i < VTSS_MPLS_VPROFILE_CNT - 1; i++) {
        for (dei = 0; dei <= 1; dei ++) {
            for (pcp = 0; pcp <= 7; pcp++) {
                SRVL_WR(VTSS_ANA_PORT_QOS_PCP_DEI_MAP_CFG(i, 8 *dei + pcp), (dei << 3) | pcp);
            }
        }
    }

    return VTSS_RC_OK;
}

static vtss_rc srvl_mpls_vprofile_alloc(vtss_state_t *vtss_state,
                                        vtss_mpls_vprofile_idx_t * const idx)
{
    VTSS_MPLS_TAKE_FROM_FREELIST(mpls.vprofile_free_list, mpls.vprofile_conf, *idx);
    vtss_state->mpls.vprofile_cnt++;
    VTSS_MPLS_CHECK(vtss_state->mpls.vprofile_cnt <= (VTSS_MPLS_VPROFILE_CNT - VTSS_MPLS_VPROFILE_RESERVED_CNT));

    memset(&vtss_state->mpls.vprofile_conf[*idx].pub, 0, sizeof(vtss_state->mpls.vprofile_conf[*idx].pub));

    VTSS_DG(VTSS_TRACE_GROUP_MPLS, "Allocated VProfile entry %d (%d allocated)", *idx, vtss_state->mpls.vprofile_cnt);
    return VTSS_RC_OK;
}

static vtss_rc srvl_mpls_vprofile_free(vtss_state_t *vtss_state,
                                       const vtss_mpls_vprofile_idx_t idx)
{
    if (idx < VTSS_MPLS_VPROFILE_RESERVED_CNT) {
        return VTSS_RC_ERROR;
    }
    VTSS_DG(VTSS_TRACE_GROUP_MPLS, "Freeing VProfile entry %d (%d allocated afterwards)", idx, vtss_state->mpls.vprofile_cnt - 1);
    VTSS_MPLS_RETURN_TO_FREELIST(mpls.vprofile_free_list, mpls.vprofile_conf, idx);
    VTSS_MPLS_CHECK(vtss_state->mpls.vprofile_cnt > 0);
    vtss_state->mpls.vprofile_cnt--;
    return VTSS_RC_OK;
}

/* ----------------------------------------------------------------- *
 *  MPLS Out-segment (Egress) Encapsulation
 * ----------------------------------------------------------------- */

/* The MPLS egress encapsulation data consists of 320 bits; the first 48 bits
 * contain remarking information, and the subsequent bits consist of the
 * MLL + optional MLBS configuration prepended to the payload.
 */

static void srvl_mpls_out_encap_set_mac(u32 *bits, u32 offset, const vtss_mac_t *mac)
{
    int i;
    for (i = 5; i >= 0; i--) {
        srvl_mpls_out_encap_set_bits(bits, offset, 8, mac->addr[i]);
        offset += 8;
    }
}

typedef struct {
    struct {
        BOOL use_cls_vid;
        BOOL use_cls_dp;
        BOOL use_cls_pcp;
        BOOL use_cls_dei;
        struct {
            BOOL use_ttl;
            BOOL use_s_bit;
            BOOL use_qos_to_tc_map;
            u8   qos_to_tc_map_idx;
        } label[VTSS_MPLS_OUT_ENCAP_LABEL_CNT];
    } remark;
    vtss_mac_t                   dmac;
    vtss_mac_t                   smac;
    vtss_mll_tagtype_t           tag_type;
    vtss_vid_t                   vid;                   /**< C or B VID */
    vtss_tagprio_t               pcp;                   /**< PCP value */
    vtss_dei_t                   dei;                   /**< DEI value */
    vtss_mll_ethertype_t         ether_type;
    u8                           label_stack_depth;     /**< Range [0..VTSS_MPLS_ENCAP_LABEL_CNT] */
    vtss_mpls_label_t            label_stack[VTSS_MPLS_OUT_ENCAP_LABEL_CNT];
    BOOL                         use_cw;
    u32                          cw;
} srvl_mpls_out_encap_t;

static vtss_rc srvl_mpls_out_encap_set(vtss_state_t *vtss_state,
                                       u32 idx, const srvl_mpls_out_encap_t *const entry, u32 *length)
{
    const int word_cnt = 320/32;

    u32  bits[word_cnt];    // MSB is bit 31 in [0]
    int  i;
    u32  offset;
    u32  offset_base;

    if (idx == 0  ||  idx > VTSS_MPLS_OUT_ENCAP_CNT) {
        VTSS_DG(VTSS_TRACE_GROUP_MPLS, "Invalid MPLS out-segment encap idx (%u)", idx);
        return VTSS_RC_ERROR;
    }

    *length = 0;

    memset(bits, 0, sizeof(bits));

#define SET_BOOL(offset, boolean)      srvl_mpls_out_encap_set_bit(bits, (offset), (boolean) ? 1 : 0)
#define SET_INT(offset, width, value)  srvl_mpls_out_encap_set_bits(bits, (offset), (width), (value))
#define SET_INT_O(width, value)        offset -= (width); srvl_mpls_out_encap_set_bits(bits, offset, (width), (value))

    SET_BOOL(298, entry->remark.use_cls_vid);
    SET_BOOL(297, entry->remark.use_cls_dp);
    SET_BOOL(296, entry->remark.use_cls_pcp);
    SET_BOOL(295, entry->remark.use_cls_dei);
    SET_BOOL(294, entry->tag_type != VTSS_MPLS_TAGTYPE_UNTAGGED);
    for (i = 0; i < 3; i++) {
        SET_INT (272 + i*4, 3, entry->remark.label[i].qos_to_tc_map_idx);
        SET_BOOL(275 + i*4,    entry->remark.label[i].use_qos_to_tc_map);
        SET_BOOL(288 + i,      entry->remark.label[i].use_s_bit);
        SET_BOOL(291 + i,      entry->remark.label[i].use_ttl);
    }

    offset_base = offset = 272;

    offset -= 6*8;
    srvl_mpls_out_encap_set_mac(bits, offset, &entry->dmac);
    offset -= 6*8;
    srvl_mpls_out_encap_set_mac(bits, offset, &entry->smac);

    switch (entry->tag_type) {
    case VTSS_MPLS_TAGTYPE_UNTAGGED:
        break;
    case VTSS_MPLS_TAGTYPE_CTAGGED:
        SET_INT_O(16, 0x8100);
        SET_INT_O(3, entry->pcp);
        SET_INT_O(1, entry->dei);
        SET_INT_O(12, entry->vid);
        break;
    case VTSS_MPLS_TAGTYPE_STAGGED:
        SET_INT_O(16, 0x88A8);              // TBD: Use global value if a such is configured?
        SET_INT_O(3, entry->pcp);
        SET_INT_O(1, entry->dei);
        SET_INT_O(12, entry->vid);
        break;
    }

    if (entry->ether_type == VTSS_MLL_ETHERTYPE_DOWNSTREAM_ASSIGNED) {
        SET_INT_O(16, 0x8847);
    }
    else {
        SET_INT_O(16, 0x8848);
    }

    for (i = 0; i < (int)entry->label_stack_depth; i++) {
        SET_INT_O(20, entry->label_stack[i].value);
        SET_INT_O(3,  entry->label_stack[i].tc);
        SET_INT_O(1,  (i == (int)entry->label_stack_depth - 1) ? 1 : 0);
        SET_INT_O(8,  entry->label_stack[i].ttl);
    }
    if (entry->use_cw) {
        SET_INT_O(32, entry->cw);
    }

    *length = (offset_base - offset + 7) / 8;

#undef SET_BOOL
#undef SET_INT
#undef SET_INT_O

    for (i = 0; i < word_cnt; i++) {
        u32 val = VTSS_OS_NTOHL(bits[i]);
        VTSS_DG(VTSS_TRACE_GROUP_MPLS, "%2d   0x%08x   0x%08x", i, val, bits[i]);
        SRVL_WR(VTSS_SYS_ENCAPSULATIONS_ENCAP_DATA(i), val);
    }

    SRVL_WR(VTSS_SYS_ENCAPSULATIONS_ENCAP_CTRL, VTSS_F_SYS_ENCAPSULATIONS_ENCAP_CTRL_ENCAP_ID(idx) | VTSS_F_SYS_ENCAPSULATIONS_ENCAP_CTRL_ENCAP_WR);

    return VTSS_RC_OK;
}

/* ----------------------------------------------------------------- *
 *  Segment utilities
 * ----------------------------------------------------------------- */

static vtss_rc srvl_mpls_segment_state_get(vtss_state_t                      *vtss_state,
                                           const vtss_mpls_segment_idx_t     idx,
                                           vtss_mpls_segment_state_t * const state)
{
    /*lint -save -e506 Disable 'Constant value Boolean' warning */

#define SET(x) *state = VTSS_MPLS_SEGMENT_STATE_##x
#define SET_UP_IF(tst) do { if (!seg->need_hw_alloc && (tst)) SET(UP); } while (0)

    vtss_mpls_segment_internal_t *seg;
    vtss_mpls_xc_internal_t      *xc;
    BOOL                         has_l2, has_xc, has_isdx, has_srv, has_hw;
    BOOL                         is_lsr, is_ler_pw, srv_up, has_esdx;
    vtss_mpls_segment_state_t    srv_state;

    SET(UNCONF);

    VTSS_MPLS_IDX_CHECK_SEGMENT(idx);

    seg        = &SEG_I(idx);
    has_l2     = VTSS_MPLS_IDX_IS_DEF(seg->pub.l2_idx);
    has_xc     = VTSS_MPLS_IDX_IS_DEF(seg->pub.xc_idx);
    has_srv    = VTSS_MPLS_IDX_IS_DEF(seg->pub.server_idx);
    has_hw     = FALSE;
    srv_up     = FALSE;

    /* In all of the below, we cannot enter UP when seg->need_hw_alloc == TRUE.
     *
     * In-segment with L2:
     *
     *     UNCONF  = !XC || !LABEL
     *     CONF    = !UNCONF
     *     UP[LSR] = CONF && xc.ISDX && MLL && MLBS
     *     UP[LER] = if seg.PW
     *                   CONF && xc.ISDX && MLL && MLBS
     *               else
     *                   CONF
     *
     * In-segment without L2:
     *
     *     UNCONF  = !XC || !LABEL
     *     CONF    = !UNCONF  &&  has_server
     *     UP[LSR] = CONF && xc.ISDX && MLL && MLBS && server.UP
     *     UP[LER] = if seg.PW
     *                   CONF && xc.ISDX && MLL && MLBS && server.UP
     *               else
     *                   CONF
     *
     * Out-segment with L2:
     *
     *     UNCONF  = !XC || !LABEL || !TTL
     *     CONF    = !UNCONF
     *     UP[LSR] = CONF && xc.ISDX && ESDX && ENCAP && ES0
     *     UP[LER] = if seg.PW
     *                   CONF && ENCAP
     *               else
     *                   CONF
     *
     * Out-segment without L2:
     *
     *     UNCONF  = !XC || !LABEL || !TTL
     *     CONF    = !UNCONF  &&  has_server
     *     UP[LSR] = CONF && xc.ISDX && ESDX && ENCAP && ES0 && server.UP
     *     UP[LER] = if seg.PW
     *                   CONF && ENCAP && server.UP
     *               else
     *                   CONF && server.UP
     *
     */

    if (!has_xc  ||  (seg->pub.label.value == VTSS_MPLS_LABEL_VALUE_DONTCARE)  ||
        (!seg->pub.is_in  &&  (seg->pub.label.ttl == 0))) {
        *state = VTSS_MPLS_SEGMENT_STATE_UNCONF;
        return VTSS_RC_OK;
    }

    xc         = &XC_I(seg->pub.xc_idx);
    has_isdx   = xc->isdx != NULL;
    is_lsr     = xc->pub.type == VTSS_MPLS_XC_TYPE_LSR;
    is_ler_pw  = xc->pub.type == VTSS_MPLS_XC_TYPE_LER  &&  seg->pub.pw_conf.is_pw;

    VTSS_NG(VTSS_TRACE_GROUP_MPLS, "xc:%d has_isdx:%d is_lsr:%d is_ler_pw:%d oam_type:%d", seg->pub.xc_idx, has_isdx, is_lsr, is_ler_pw, seg->pub.oam_conf.type);

    if (seg->pub.is_in) {
        if (has_l2) {
            SET(CONF);
            if (is_lsr  ||  is_ler_pw) {
                has_hw = has_isdx  &&  seg->u.in.has_mll  &&  seg->u.in.has_mlbs;
                SET_UP_IF(has_hw);
            }
            else {
                SET_UP_IF(TRUE);
            }
        }
        else {
            if (has_srv) {
                SET(CONF);
                (void) srvl_mpls_segment_state_get(vtss_state, seg->pub.server_idx, &srv_state);
                srv_up = srv_state == VTSS_MPLS_SEGMENT_STATE_UP;
                if (is_lsr  ||  is_ler_pw) {
                    has_hw = has_isdx  &&  seg->u.in.has_mlbs;
                    SET_UP_IF(has_hw && srv_up);
                }
                else {
                    SET_UP_IF(TRUE);
                }
            }
        }
    }
    else {  // Out-segment
        if (has_l2) {
            SET(CONF);
            srv_up = TRUE;   // Or so we pretend.
        }
        else if (has_srv) {
            SET(CONF);
            (void) srvl_mpls_segment_state_get(vtss_state, seg->pub.server_idx, &srv_state);
            srv_up = srv_state == VTSS_MPLS_SEGMENT_STATE_UP;
        }
        if (is_lsr) {
            has_esdx = seg->u.out.esdx != NULL;
            has_hw = has_isdx  &&  has_esdx  &&  seg->u.out.has_encap  &&  seg->u.out.has_es0;
            SET_UP_IF(has_hw  && srv_up);
        }
        else if (is_ler_pw) {
            SET_UP_IF(seg->u.out.has_encap && srv_up);
        }
        else {
            SET_UP_IF(srv_up);
        }
    }

    return VTSS_RC_OK;
#undef SET
#undef SET_UP_IF
    /*lint -restore */
}

static const char *srvl_mpls_segment_state_to_str(const vtss_mpls_segment_state_t s)
{
    switch (s) {
    case VTSS_MPLS_SEGMENT_STATE_UNCONF:
        return "UNCONF";
    case VTSS_MPLS_SEGMENT_STATE_CONF:
        return "CONF";
    case VTSS_MPLS_SEGMENT_STATE_UP:
        return "UP";
    default:
        return "(unknown)";
    }
}

static vtss_rc srvl_mpls_segment_status_get(vtss_state_t                       *vtss_state,
                                            const vtss_mpls_segment_idx_t      idx,
                                            vtss_mpls_segment_status_t * const status)
{
    memset(status, 0, sizeof(*status));
    if (srvl_mpls_segment_state_get(vtss_state, idx, &status->state) == VTSS_RC_OK) {
        status->oam_active = SEG_I(idx).oam_active;
        return VTSS_RC_OK;
    }
    return VTSS_RC_ERROR;
}

/* ----------------------------------------------------------------- *
 *  IS0 MLL entries
 * ----------------------------------------------------------------- */

static vtss_rc srvl_mpls_is0_mll_update(vtss_state_t *vtss_state, vtss_mpls_l2_internal_t *l2, BOOL upstream);

static vtss_rc srvl_mpls_is0_mll_alloc (vtss_state_t *vtss_state, vtss_mpls_l2_internal_t *l2, BOOL upstream)
{
    i16                       *ll_idx = upstream ? &l2->ll_idx_upstream : &l2->ll_idx_downstream;
    vtss_mpls_idxchain_iter_t dummy;
    vtss_mpls_idxchain_user_t user;

    VTSS_DG(VTSS_TRACE_GROUP_MPLS, "Allocating new MLL %sstream entry id", upstream ? "up" : "down");
    VTSS_MPLS_CHECK(VTSS_MPLS_IDX_IS_UNDEF(*ll_idx));

    VTSS_MPLS_CHECK(vtss_mpls_idxchain_get_first(vtss_state, &vtss_state->mpls.is0_mll_free_chain, &dummy, &user));
    VTSS_MPLS_CHECK_RC(vtss_mpls_idxchain_remove_first(vtss_state, &vtss_state->mpls.is0_mll_free_chain, user));
    *ll_idx = user;

    vtss_state->mpls.is0_mll_cnt++;
    VTSS_MPLS_CHECK(vtss_state->mpls.is0_mll_cnt <= VTSS_MPLS_L2_CNT  &&
                    vtss_state->mpls.is0_mll_cnt + vtss_state->mpls.is0_mlbs_cnt / 2 <= SRVL_IS0_CNT);

    return srvl_mpls_is0_mll_update(vtss_state, l2, upstream);
}

static vtss_rc srvl_mpls_is0_mll_free(vtss_state_t *vtss_state,
                                      vtss_mpls_l2_internal_t *l2, BOOL upstream)
{
    vtss_rc rc;
    i16 *ll_idx = upstream ? &l2->ll_idx_upstream : &l2->ll_idx_downstream;

    VTSS_DG(VTSS_TRACE_GROUP_MPLS, "Removing IS0 MLL entry %d", *ll_idx);
    VTSS_MPLS_CHECK(VTSS_MPLS_IDX_IS_DEF(*ll_idx));

    // Remove VCAP entry and put its index back on the free chain
    rc = vtss_vcap_del(vtss_state, &vtss_state->vcap.is0.obj, VTSS_IS0_USER_MPLS_LL, *ll_idx);
    VTSS_MPLS_CHECK_RC(vtss_mpls_idxchain_insert_at_head(vtss_state, &vtss_state->mpls.is0_mll_free_chain, *ll_idx));
    VTSS_MPLS_IDX_UNDEF(*ll_idx);

    VTSS_MPLS_CHECK(vtss_state->mpls.is0_mll_cnt > 0);
    vtss_state->mpls.is0_mll_cnt--;

    return rc;
}

static vtss_rc srvl_mpls_is0_mll_update(vtss_state_t *vtss_state,
                                        vtss_mpls_l2_internal_t *l2, BOOL upstream)
{
    vtss_rc                   rc;
    vtss_is0_entry_t          entry;
    vtss_vcap_data_t          data;
    i16                       ll_idx = upstream ? l2->ll_idx_upstream : l2->ll_idx_downstream;

    VTSS_DG(VTSS_TRACE_GROUP_MPLS, "Updating MLL entry id %d", ll_idx);
    VTSS_MPLS_CHECK(VTSS_MPLS_IDX_IS_DEF(ll_idx));

    memset(&entry, 0, sizeof(entry));

    entry.type = VTSS_IS0_TYPE_MLL;

    entry.key.mll.ingress_port = l2->pub.port;
    entry.key.mll.tag_type     = (vtss_is0_tagtype_t) l2->pub.tag_type;
    entry.key.mll.b_vid        = l2->pub.vid;
    entry.key.mll.ether_type   = upstream ? VTSS_MLL_ETHERTYPE_UPSTREAM_ASSIGNED : VTSS_MLL_ETHERTYPE_DOWNSTREAM_ASSIGNED;
    memcpy(&entry.key.mll.smac, &l2->pub.peer_mac, sizeof(entry.key.mll.smac));
    memcpy(&entry.key.mll.dmac, &l2->pub.self_mac, sizeof(entry.key.mll.dmac));

    entry.action.mll.linklayer_index = ll_idx;
    entry.action.mll.mpls_forwarding = TRUE;

    data.key_size    = VTSS_VCAP_KEY_SIZE_FULL;
    data.u.is0.entry = &entry;

    rc = vtss_vcap_add(vtss_state, &vtss_state->vcap.is0.obj, VTSS_IS0_USER_MPLS_LL, ll_idx,
                       VTSS_VCAP_ID_LAST, &data, FALSE);

    if (rc != VTSS_RC_OK) {
        VTSS_DG(VTSS_TRACE_GROUP_MPLS, "MLL entry update FAILED, rc = %d. Attempting to delete entry.", rc);
        VTSS_MPLS_CHECK_RC(srvl_mpls_is0_mll_free(vtss_state, l2, upstream));
    }

    return rc;
 }



/* ----------------------------------------------------------------- *
 *  IS0 MLBS entries
 * ----------------------------------------------------------------- */

static vtss_vcap_user_t srvl_mpls_depth_to_is0_user(u8 depth)
{
#if (VTSS_MPLS_IN_ENCAP_LABEL_CNT != 3)
#error "This code assumes VTSS_MPLS_IN_ENCAP_LABEL_CNT == 3"
#endif
    switch (depth) {
    case 1:  return VTSS_IS0_USER_MPLS_MLBS_1;
    case 2:  return VTSS_IS0_USER_MPLS_MLBS_2;
    case 3:  return VTSS_IS0_USER_MPLS_MLBS_3;
    default:
        VTSS_DG(VTSS_TRACE_GROUP_MPLS, "Label stack too deep: %d", depth);
        return VTSS_IS0_USER_MPLS_MLBS_1;
    }
}

static vtss_rc srvl_mpls_is0_mlbs_free(vtss_state_t *vtss_state, vtss_mpls_segment_idx_t idx)
{
    vtss_mpls_segment_internal_t *seg = &SEG_I(idx);
    VTSS_DG(VTSS_TRACE_GROUP_MPLS, "entry, segment %d", idx);

    VTSS_MPLS_CHECK(seg->pub.is_in);

    if (seg->u.in.has_mlbs) {
        // Remove VCAP entry
        VTSS_DG(VTSS_TRACE_GROUP_MPLS, "Removing IS0 MLBS segment %d (VCAP user %d)", idx, seg->u.in.vcap_user);
        VTSS_MPLS_CHECK_RC(vtss_vcap_del(vtss_state, &vtss_state->vcap.is0.obj, seg->u.in.vcap_user, idx));
        seg->u.in.has_mlbs = FALSE;
        seg->oam_active = FALSE;

        VTSS_MPLS_CHECK(vtss_state->mpls.is0_mlbs_cnt > 0);
        vtss_state->mpls.is0_mlbs_cnt--;
    }
    return VTSS_RC_OK;
}

/* Update IS0 MLBS entry for in-segment. If this segment is itself a server then
 * the clients must be updated as well, but that's outside the scope of this
 * function.
 */
static vtss_rc srvl_mpls_is0_mlbs_update(vtss_state_t *vtss_state, vtss_mpls_segment_idx_t idx)
{
#if (VTSS_MPLS_IN_ENCAP_LABEL_CNT != 3)
#error "This code assumes VTSS_MPLS_IN_ENCAP_LABEL_CNT == 3"
#endif
    vtss_mpls_segment_internal_t *seg        = &SEG_I(idx);
    vtss_mpls_segment_internal_t *srv1       = VTSS_MPLS_IDX_IS_DEF(seg->pub.server_idx) ? &SEG_I(seg->pub.server_idx) : NULL;
    vtss_mpls_segment_internal_t *srv2       = srv1 && VTSS_MPLS_IDX_IS_DEF(srv1->pub.server_idx) ? &SEG_I(srv1->pub.server_idx) : NULL;
    vtss_mpls_xc_internal_t      *xc         = &XC_I(seg->pub.xc_idx);
    vtss_mpls_l2_internal_t      *l2         = NULL;
    vtss_mpls_segment_idx_t      seg_with_l2;
    u8                           depth       = 0;
    i16                          i;
    vtss_rc                      rc;
    vtss_is0_entry_t             entry;
    vtss_vcap_data_t             data;
    BOOL                         already_has_mlbs = seg->u.in.has_mlbs;

    VTSS_DG(VTSS_TRACE_GROUP_MPLS, "entry, segment %d", idx);
    VTSS_MPLS_CHECK(xc->isdx);

    memset(&entry, 0, sizeof(entry));
    entry.type = VTSS_IS0_TYPE_MLBS;

    // Configure label stack for MLBS key

    entry.action.mlbs.pop_count = VTSS_IS0_MLBS_POPCOUNT_18;  // DMAC, SMAC, eth-type, 1 label

    depth = 1 + (srv1 ? 1 : 0) + (srv2 ? 1 : 0);

    seg_with_l2                                      = idx;
    entry.key.mlbs.label_stack[depth - 1].value      = seg->pub.label.value;
    entry.key.mlbs.label_stack[depth - 1].tc         = seg->pub.label.tc;
    entry.key.mlbs.label_stack[depth - 1].value_mask = VTSS_BITMASK(seg->pub.label.value == VTSS_MPLS_LABEL_VALUE_DONTCARE ? 0 : 20);
    entry.key.mlbs.label_stack[depth - 1].tc_mask    = VTSS_BITMASK(seg->pub.label.tc    == VTSS_MPLS_TC_VALUE_DONTCARE    ? 0 :  8);
    entry.action.mlbs.tc_label_index                 = depth - 1;
    entry.action.mlbs.ttl_label_index                = depth - 1;
    entry.action.mlbs.tc_maptable_index              = VTSS_MPLS_IDX_IS_DEF(seg->pub.tc_qos_map_idx) ? seg->pub.tc_qos_map_idx : 0;

    // We only process server segments here at ingress when they're being (LER) terminated

    if (srv1) {
        vtss_mpls_xc_internal_t *srv1_xc                 = &XC_I(srv1->pub.xc_idx);
        seg_with_l2                                      = seg->pub.server_idx;
        entry.action.mlbs.pop_count++;
        entry.key.mlbs.label_stack[depth - 2].value      = srv1->pub.label.value;
        entry.key.mlbs.label_stack[depth - 2].tc         = srv1->pub.label.tc;
        entry.key.mlbs.label_stack[depth - 2].value_mask = VTSS_BITMASK(srv1->pub.label.value == VTSS_MPLS_LABEL_VALUE_DONTCARE ? 0 : 20);
        entry.key.mlbs.label_stack[depth - 2].tc_mask    = VTSS_BITMASK(srv1->pub.label.tc    == VTSS_MPLS_TC_VALUE_DONTCARE    ? 0 :  8);

        if (srv1_xc->pub.tc_tunnel_mode == VTSS_MPLS_TUNNEL_MODE_UNIFORM) {
            entry.action.mlbs.tc_label_index             = depth - 2;
            entry.action.mlbs.tc_maptable_index          = VTSS_MPLS_IDX_IS_DEF(srv1->pub.tc_qos_map_idx) ? srv1->pub.tc_qos_map_idx : 0;
        }
        if (srv1_xc->pub.ttl_tunnel_mode == VTSS_MPLS_TUNNEL_MODE_UNIFORM) {
            entry.action.mlbs.ttl_label_index            = depth - 2;
        }

        if (srv2) {
            vtss_mpls_xc_internal_t *srv2_xc                 = &XC_I(srv2->pub.xc_idx);
            seg_with_l2                                      = srv1->pub.server_idx;
            entry.action.mlbs.pop_count++;
            entry.key.mlbs.label_stack[depth - 3].value      = srv2->pub.label.value;
            entry.key.mlbs.label_stack[depth - 3].tc         = srv2->pub.label.tc;
            entry.key.mlbs.label_stack[depth - 3].value_mask = VTSS_BITMASK(srv2->pub.label.value == VTSS_MPLS_LABEL_VALUE_DONTCARE ? 0 : 20);
            entry.key.mlbs.label_stack[depth - 3].tc_mask    = VTSS_BITMASK(srv2->pub.label.tc    == VTSS_MPLS_TC_VALUE_DONTCARE    ? 0 :  8);

            if ((srv2_xc->pub.tc_tunnel_mode == VTSS_MPLS_TUNNEL_MODE_UNIFORM)  && (srv1_xc->pub.tc_tunnel_mode == VTSS_MPLS_TUNNEL_MODE_UNIFORM)) {
                entry.action.mlbs.tc_label_index             = depth - 3;
                entry.action.mlbs.tc_maptable_index          = VTSS_MPLS_IDX_IS_DEF(srv2->pub.tc_qos_map_idx) ? srv2->pub.tc_qos_map_idx : 0;
            }
            if ((srv2_xc->pub.ttl_tunnel_mode == VTSS_MPLS_TUNNEL_MODE_UNIFORM) && (srv1_xc->pub.ttl_tunnel_mode == VTSS_MPLS_TUNNEL_MODE_UNIFORM)) {
                entry.action.mlbs.ttl_label_index            = depth - 3;
            }
        }
    }


    VTSS_MPLS_CHECK(VTSS_MPLS_IDX_IS_DEF(SEG_P(seg_with_l2).l2_idx));
    l2 = &L2_I(SEG_P(seg_with_l2).l2_idx);

    // Pop VID if L2 has it

    entry.action.mlbs.pop_count += (l2->pub.vid != 0) ? 1 : 0;

    // Get MLL:LL_IDX

    i = seg->pub.upstream ? l2->ll_idx_upstream : l2->ll_idx_downstream;
    VTSS_MPLS_CHECK(VTSS_MPLS_IDX_IS_DEF(i));
    entry.key.mlbs.linklayer_index = i;

    // Choose label for swap. OAM may adjust this later

    entry.action.mlbs.swap_label_index        = depth - 1;
    entry.action.mlbs.swap_is_bottom_of_stack = FALSE;

    // VProfile, classification output setup

    entry.action.mlbs.vprofile_index     = seg->u.in.vprofile_idx;
    entry.action.mlbs.use_service_config = TRUE;
    entry.action.mlbs.classified_vid     = 0;
    entry.action.mlbs.s_tag              = 0;
    entry.action.mlbs.pcp                = 0;
    entry.action.mlbs.dei                = 0;

    // PW + CW termination

    if (seg->pub.pw_conf.is_pw) {
        entry.action.mlbs.terminate_pw =  xc->pub.type == VTSS_MPLS_XC_TYPE_LER || seg->pub.oam_conf.type != VTSS_MPLS_OAM_TYPE_NONE; /* yes, also when stitching */
        entry.action.mlbs.cw_enable    =  seg->pub.pw_conf.process_cw;
        entry.action.mlbs.pop_count    += seg->pub.pw_conf.process_cw ? 1 : 0;
    } else {
        entry.action.mlbs.terminate_pw = FALSE;
        entry.action.mlbs.cw_enable    = FALSE;
    }

    // ISDX

    entry.action.mlbs.isdx              = xc->isdx->sdx;

    // QoS. TC mapping is configured above as it depends on tunnel modes

    entry.action.mlbs.e_lsp             = seg->pub.e_lsp;
    entry.action.mlbs.add_tc_to_isdx    = FALSE;
    entry.action.mlbs.l_lsp_qos_class   = seg->pub.l_lsp_cos;

    // OAM

    // Use same ISDX for both OAM and data: {0, FALSE} = "OAM_ISDX = 0 + ISDX"
    entry.action.mlbs.oam_isdx                           = 0;
    entry.action.mlbs.oam_isdx_add_replace               = FALSE;

    entry.action.mlbs.oam_buried_mip                     = FALSE;
    entry.action.mlbs.oam_reserved_label_value           = 0;
    entry.action.mlbs.oam_reserved_label_bottom_of_stack = FALSE;

    seg->oam_active                                      = FALSE;

    switch (seg->pub.oam_conf.type) {
    case VTSS_MPLS_OAM_TYPE_NONE:
        entry.action.mlbs.oam                                    = VTSS_IS0_MLBS_OAM_NONE;
        entry.action.mlbs.oam_isdx                               = 0;
        break;

    case VTSS_MPLS_OAM_TYPE_VCCV1:
        entry.action.mlbs.oam                                    = VTSS_IS0_MLBS_OAM_VCCV1;
        seg->oam_active                                          = TRUE;
        break;

    case VTSS_MPLS_OAM_TYPE_VCCV2:
        entry.action.mlbs.oam                                    = VTSS_IS0_MLBS_OAM_VCCV2;
        seg->oam_active                                          = TRUE;
        break;

    case VTSS_MPLS_OAM_TYPE_VCCV3:
        entry.action.mlbs.oam                                    = VTSS_IS0_MLBS_OAM_VCCV3;
        seg->oam_active                                          = TRUE;
        break;

    case VTSS_MPLS_OAM_TYPE_VCCV4:
        if (depth < 3) {
            // DS1026, sect. 7.2.5
            // GAL in OAM must be BOS, but that's conflicting with data frames: There the PW is BOS, but we can't check it.
            entry.action.mlbs.oam                                = VTSS_IS0_MLBS_OAM_GAL_MEP;
            entry.action.mlbs.oam_reserved_label_value           = MPLS_GAL_LABEL_VALUE;
            entry.action.mlbs.oam_reserved_label_bottom_of_stack = TRUE;
            entry.action.mlbs.swap_label_index++;                // Swap label must point at GAL
            entry.action.mlbs.swap_is_bottom_of_stack            = FALSE;
            entry.action.mlbs.terminate_pw                       = FALSE;
            seg->oam_active                                      = TRUE;
        } else {
            VTSS_EG(VTSS_TRACE_GROUP_MPLS, "VCCV type 4 on Serval is only supported for label stack depths < 3");
            entry.action.mlbs.oam                                = VTSS_IS0_MLBS_OAM_NONE;
            entry.action.mlbs.oam_isdx                           = 0;
        }
        break;

    case VTSS_MPLS_OAM_TYPE_GAL_MEP:
        if (depth < 3) {
            entry.action.mlbs.oam                                = VTSS_IS0_MLBS_OAM_GAL_MEP;
            entry.action.mlbs.oam_reserved_label_value           = MPLS_GAL_LABEL_VALUE;
            entry.action.mlbs.oam_reserved_label_bottom_of_stack = TRUE;
            entry.action.mlbs.swap_label_index++;                // Swap label must point at GAL
            entry.action.mlbs.swap_is_bottom_of_stack            = TRUE;
            entry.action.mlbs.e_lsp                              = FALSE;
            seg->oam_active                                      = TRUE;
        } else {
            VTSS_EG(VTSS_TRACE_GROUP_MPLS, "GAL MEP OAM on Serval is only supported for label stack depths < 3");
            entry.action.mlbs.oam                                = VTSS_IS0_MLBS_OAM_NONE;
            entry.action.mlbs.oam_isdx                           = 0;
        }
        break;

    case VTSS_MPLS_OAM_TYPE_GAL_MIP:
        entry.action.mlbs.oam                                    = VTSS_IS0_MLBS_OAM_GAL_MIP;
        entry.action.mlbs.oam_reserved_label_value               = MPLS_GAL_LABEL_VALUE;
        entry.action.mlbs.oam_reserved_label_bottom_of_stack     = TRUE;
        seg->oam_active                                          = TRUE;
        break;
    }

#if 0
    /* Currently unused fields with default zero. Just kept here for reference */
    entry.action.mlbs.b_portlist
    entry.action.mlbs.cpu_queue = ;
    entry.action.mlbs.classified_vid = ;
    entry.action.mlbs.s_tag = ;
    entry.action.mlbs.pcp = ;
    entry.action.mlbs.dei = ;
#endif

    // Commit to VCAP

    seg->u.in.vcap_user = srvl_mpls_depth_to_is0_user(depth);

    VTSS_DG(VTSS_TRACE_GROUP_MPLS, "Updating MLBS for segment %d depth %d (VCAP user %d): ISDX %d, label stack  %d/%d  %d/%d  %d/%d",
             idx, depth, seg->u.in.vcap_user, xc->isdx->sdx,
             entry.key.mlbs.label_stack[0].value,
             entry.key.mlbs.label_stack[0].tc,
             entry.key.mlbs.label_stack[1].value,
             entry.key.mlbs.label_stack[1].tc,
             entry.key.mlbs.label_stack[2].value,
             entry.key.mlbs.label_stack[2].tc);

    data.key_size    = VTSS_VCAP_KEY_SIZE_HALF;
    data.u.is0.entry = &entry;

    rc = vtss_vcap_add(vtss_state, &vtss_state->vcap.is0.obj, seg->u.in.vcap_user,
                       idx, VTSS_VCAP_ID_LAST, &data, FALSE);

    VTSS_DG(VTSS_TRACE_GROUP_MPLS, "MLBS Done, rc = %d", rc);

    seg->u.in.has_mlbs = (rc == VTSS_RC_OK);

    if (!seg->u.in.has_mlbs) {
        // VCAP setup failed, thus OAM failed as well
        seg->oam_active = FALSE;
    }

    if (!already_has_mlbs  &&  seg->u.in.has_mlbs) {
        vtss_state->mpls.is0_mlbs_cnt++;
        VTSS_MPLS_CHECK(vtss_state->mpls.is0_mlbs_cnt <= VTSS_MPLS_LSP_CNT &&
                        vtss_state->mpls.is0_mll_cnt + vtss_state->mpls.is0_mlbs_cnt / 2 <= SRVL_IS0_CNT);
    }

    return rc;
}

// Helper: Tear MLBS for segment and all its clients through recursion. 'depth' is only a consistency check to ensure recursion always terminates
static vtss_rc srvl_mpls_is0_mlbs_tear_all_recursive(vtss_state_t *vtss_state,
                                                     vtss_mpls_segment_idx_t idx, u8 depth)
{
    vtss_mpls_idxchain_iter_t    iter;
    vtss_mpls_idxchain_user_t    user;
    BOOL                         more;
    vtss_mpls_segment_internal_t *seg;

    VTSS_DG(VTSS_TRACE_GROUP_MPLS, "entry, segment %d, depth = %u", idx, depth);
    VTSS_MPLS_IDX_CHECK_SEGMENT(idx);
    VTSS_MPLS_CHECK(depth <= VTSS_MPLS_IN_ENCAP_LABEL_CNT);

    seg = &SEG_I(idx);
    VTSS_MPLS_CHECK_RC(srvl_mpls_is0_mlbs_free(vtss_state, idx));

    more = vtss_mpls_idxchain_get_first(vtss_state, &seg->clients, &iter, &user);
    while (more) {
        VTSS_MPLS_CHECK_RC(srvl_mpls_is0_mlbs_tear_all_recursive(vtss_state, user, depth + 1));
        more = vtss_mpls_idxchain_get_next(vtss_state, &iter, &user);
    }
    return VTSS_RC_OK;
}

// Tear MLBS for segment and all its clients
static vtss_rc srvl_mpls_is0_mlbs_tear_all(vtss_state_t *vtss_state, vtss_mpls_segment_idx_t idx)
{
    return srvl_mpls_is0_mlbs_tear_all_recursive(vtss_state, idx, 1);
}



/* ----------------------------------------------------------------- *
 * MPLS Egress encapsulation entries
 * ----------------------------------------------------------------- */

static vtss_rc srvl_mpls_encap_update(vtss_state_t *vtss_state, vtss_mpls_segment_idx_t idx);

static vtss_rc srvl_mpls_encap_alloc(vtss_state_t *vtss_state, vtss_mpls_segment_idx_t idx)
{
    vtss_mpls_segment_internal_t *seg = &SEG_I(idx);
    vtss_mpls_idxchain_iter_t    dummy;
    vtss_mpls_idxchain_user_t    user;

    VTSS_MPLS_CHECK(VTSS_MPLS_IDX_IS_UNDEF(seg->u.out.encap_idx));

    VTSS_DG(VTSS_TRACE_GROUP_MPLS, "Allocating new MPLS encap entry id for out-seg %d", idx);
    VTSS_MPLS_CHECK(vtss_mpls_idxchain_get_first(vtss_state, &vtss_state->mpls.encap_free_chain, &dummy, &user));
    VTSS_MPLS_CHECK_RC(vtss_mpls_idxchain_remove_first(vtss_state, &vtss_state->mpls.encap_free_chain, user));
    vtss_state->mpls.encap_cnt++;
    VTSS_MPLS_CHECK(vtss_state->mpls.encap_cnt <= VTSS_MPLS_OUT_ENCAP_CNT);
    VTSS_MPLS_CHECK(user != 0);   // Index 0 doesn't work for ES0; it won't push the encap then.
    seg->u.out.encap_idx   = user;
    seg->u.out.encap_bytes = 0;

    return srvl_mpls_encap_update(vtss_state, idx);
}

static vtss_rc srvl_mpls_encap_free(vtss_state_t *vtss_state, vtss_mpls_segment_idx_t idx)
{
    vtss_mpls_segment_internal_t *seg = &SEG_I(idx);

    VTSS_DG(VTSS_TRACE_GROUP_MPLS, "Removing MPLS encap entry %d", seg->u.out.encap_idx);
    VTSS_MPLS_CHECK(VTSS_MPLS_IDX_IS_DEF(seg->u.out.encap_idx));
    VTSS_MPLS_CHECK(seg->u.out.has_encap);

    // Put index back on the free chain
    VTSS_MPLS_CHECK_RC(vtss_mpls_idxchain_insert_at_head(vtss_state, &vtss_state->mpls.encap_free_chain, seg->u.out.encap_idx));
    VTSS_MPLS_IDX_UNDEF(seg->u.out.encap_idx);
    seg->u.out.has_encap   = FALSE;
    seg->u.out.encap_bytes = 0;

    VTSS_MPLS_CHECK(vtss_state->mpls.encap_cnt > 0);
    vtss_state->mpls.encap_cnt--;

    /* There may still be an ES0 entry that uses this encap; the caller must
     * clean it up
     */

    return VTSS_RC_OK;
}

/* Fill in segment pointer array. First item must be outer-most label, so we
 * use recursion down the server list to get to the ultimate server before
 * beginning to fill in the array.
 */
static void srvl_mpls_encap_make_seg_array(vtss_state_t *vtss_state,
                                           vtss_mpls_segment_idx_t idx,
                                           vtss_mpls_segment_internal_t *ppseg[],
                                           i8 *depth,
                                           i8 *i)
{
    vtss_mpls_segment_internal_t *s = &SEG_I(idx);
    if (*depth > VTSS_MPLS_IN_ENCAP_LABEL_CNT) {
        VTSS_DG(VTSS_TRACE_GROUP_MPLS, "Recursion level too deep");
        return;
    }
    if (VTSS_MPLS_IDX_IS_DEF(s->pub.server_idx)) {
        (*depth)++;
        srvl_mpls_encap_make_seg_array(vtss_state, s->pub.server_idx, ppseg, depth, i);
    }
    ppseg[(*i)++] = s;
}

static vtss_rc srvl_mpls_encap_update(vtss_state_t *vtss_state, vtss_mpls_segment_idx_t idx)
{
    vtss_mpls_segment_internal_t *seg[VTSS_MPLS_IN_ENCAP_LABEL_CNT] = { 0 };
    vtss_mpls_xc_internal_t      *xc [VTSS_MPLS_IN_ENCAP_LABEL_CNT] = { 0 };
    vtss_mpls_l2_internal_t      *l2;
    i8                           i;
    i8                           depth;
    vtss_rc                      rc;
    srvl_mpls_out_encap_t        encap;
    u32                          length = 0;

    VTSS_DG(VTSS_TRACE_GROUP_MPLS, "MPLS encap table setup of idx %d for segment %d", SEG_I(idx).u.out.encap_idx, idx);
    VTSS_MPLS_CHECK(VTSS_MPLS_IDX_IS_DEF(SEG_I(idx).u.out.encap_idx));

    depth = 1;
    i = 0;
    srvl_mpls_encap_make_seg_array(vtss_state, idx, seg, &depth, &i);

    memset(&encap, 0, sizeof(encap));
    seg[depth - 1]->u.out.encap_bytes = 0;

    l2 = &L2_I(seg[0]->pub.l2_idx);

    for (i = 0; i < depth; i++) {
        xc[i]                = &XC_I(seg[i]->pub.xc_idx);
        encap.label_stack[i] = seg[i]->pub.label;
    }
    encap.label_stack_depth = depth;

    // For LSR, set innermost label's S-bit, TC and TTL from incoming frame's
    // classified values.
    //
    // For MPLS LER, S-bit is set for the innermost label; TC and TTL are
    // taken from the tunneled label in a way depending on the DiffServ
    // tunneling mode:
    //   Pipe + Short Pipe = push new TC / TTL
    //   Uniform           = use inner (tunneled) TC / TTL in tunnel label

    switch (xc[depth - 1]->pub.type) {        // Innermost
    case VTSS_MPLS_XC_TYPE_LSR:
        encap.remark.label[depth - 1].use_ttl       = TRUE;
        encap.remark.label[depth - 1].use_s_bit     = TRUE;
        if (VTSS_MPLS_IDX_IS_DEF(seg[depth - 1]->pub.tc_qos_map_idx)) {
            encap.remark.label[depth - 1].use_qos_to_tc_map = TRUE;
            encap.remark.label[depth - 1].qos_to_tc_map_idx = seg[depth - 1]->pub.tc_qos_map_idx & 0x07;
        } else {
            encap.remark.label[depth - 1].use_qos_to_tc_map = FALSE; // Don't map, and...
            encap.remark.label[depth - 1].qos_to_tc_map_idx = 1;     // ... set magic value: Use classified TC. See DS table 135 (rev 4.0)
        }
        break;
    case VTSS_MPLS_XC_TYPE_LER:
        if (VTSS_MPLS_IDX_IS_DEF(seg[depth - 1]->pub.tc_qos_map_idx)) {
            encap.remark.label[depth - 1].use_qos_to_tc_map = TRUE;
            encap.remark.label[depth - 1].qos_to_tc_map_idx = seg[depth - 1]->pub.tc_qos_map_idx & 0x07;
        }
        break;
    }

    // Handle TC/TTL usage for the (LER) tunnel hierarchy (if any)
    for (i = depth - 2; i >= 0; i--) {
        if (xc[i]->pub.tc_tunnel_mode == VTSS_MPLS_TUNNEL_MODE_UNIFORM) {
            encap.remark.label[i].use_qos_to_tc_map = encap.remark.label[i + 1].use_qos_to_tc_map;
            encap.remark.label[i].qos_to_tc_map_idx = encap.remark.label[i + 1].qos_to_tc_map_idx & 0x07;
            encap.label_stack[i].tc                 = encap.label_stack[i + 1].tc;
        }
        if (xc[i]->pub.ttl_tunnel_mode == VTSS_MPLS_TUNNEL_MODE_UNIFORM) {
            encap.remark.label[i].use_ttl = encap.remark.label[i + 1].use_ttl;
            encap.label_stack[i].ttl      = encap.label_stack[i + 1].ttl;
        }
    }

    for (i = depth - 1; i >= 0; i--) {
        VTSS_DG(VTSS_TRACE_GROUP_MPLS, "L%d: Lbl %d/%d/%d, qos2tc %s/%d, use TTL %s, use S %s", i, encap.label_stack[i].value, encap.label_stack[i].tc, encap.label_stack[i].ttl,
        encap.remark.label[i].use_qos_to_tc_map ? "Y":"N", encap.remark.label[i].qos_to_tc_map_idx, encap.remark.label[i].use_ttl?"Y":"N", encap.remark.label[i].use_s_bit?"Y":"N");
    }

    memcpy(&encap.dmac, &l2->pub.peer_mac, sizeof(encap.dmac));
    memcpy(&encap.smac, &l2->pub.self_mac, sizeof(encap.smac));

    encap.tag_type   = l2->pub.tag_type;
    encap.vid        = l2->pub.vid;
    encap.pcp        = l2->pub.pcp;
    encap.dei        = l2->pub.dei;
    encap.ether_type = seg[depth-1]->pub.upstream ? VTSS_MLL_ETHERTYPE_UPSTREAM_ASSIGNED : VTSS_MLL_ETHERTYPE_DOWNSTREAM_ASSIGNED;
    encap.use_cw     = seg[depth-1]->pub.pw_conf.process_cw;
    encap.cw         = seg[depth-1]->pub.pw_conf.cw;

    rc = srvl_mpls_out_encap_set(vtss_state, seg[depth-1]->u.out.encap_idx, &encap, &length);
    VTSS_DG(VTSS_TRACE_GROUP_MPLS, "Encap entry done, %u bytes, rc=%d", length, rc);

    seg[depth-1]->u.out.encap_bytes = length & 0xffff;
    seg[depth-1]->u.out.has_encap = TRUE;

    if (rc != VTSS_RC_OK) {
        VTSS_DG(VTSS_TRACE_GROUP_MPLS, "Encap entry update FAILED, rc = %d. Attempting to delete entry.", rc);
        VTSS_MPLS_CHECK_RC(srvl_mpls_encap_free(vtss_state, idx));
    }

    return rc;
}



/* ----------------------------------------------------------------- *
 * ES0 entries
 * ----------------------------------------------------------------- */

static vtss_rc srvl_mpls_es0_free(vtss_state_t *vtss_state, vtss_mpls_segment_idx_t idx)
{
    vtss_rc                      rc;
    vtss_mpls_segment_internal_t *seg = &SEG_I(idx);
    vtss_mpls_xc_internal_t      *xc  = &XC_I(seg->pub.xc_idx);

    VTSS_DG(VTSS_TRACE_GROUP_MPLS, "Removing ES0 entry for segment %d", idx);
    VTSS_MPLS_CHECK(seg->u.out.has_es0);
    VTSS_MPLS_CHECK(xc->isdx != NULL);
    rc = vtss_vcap_del(vtss_state, &vtss_state->vcap.es0.obj, VTSS_ES0_USER_MPLS, idx);
    VTSS_MPLS_CHECK_RC(vtss_srvl_isdx_update_es0(vtss_state, FALSE, xc->isdx->sdx, 0));
    seg->u.out.has_es0 = FALSE;
    return rc;
}

static vtss_rc srvl_mpls_es0_update(vtss_state_t *vtss_state, vtss_mpls_segment_idx_t idx)
{
    vtss_mpls_segment_internal_t *seg = &SEG_I(idx);
    vtss_mpls_xc_internal_t      *xc  = &XC_I(seg->pub.xc_idx);
    vtss_rc                      rc;
    vtss_es0_entry_t             entry;
    vtss_vcap_data_t             data;
    u32                          isdx_mask;
    vtss_port_no_t               port;

    VTSS_DG(VTSS_TRACE_GROUP_MPLS, "Updating ES0 entry/segment %d", idx);
    VTSS_MPLS_CHECK(!seg->pub.is_in);
    VTSS_MPLS_CHECK(xc->isdx != NULL);
    VTSS_MPLS_CHECK(seg->u.out.esdx != NULL);
    VTSS_MPLS_CHECK(VTSS_MPLS_IDX_IS_DEF(seg->u.out.encap_idx));
    VTSS_MPLS_CHECK(VTSS_MPLS_IDX_IS_DEF(seg->pub.xc_idx));

    VTSS_RC(vtss_cmn_mpls_port_no_get(vtss_state, idx, &port));

    vtss_vcap_es0_init(&data, &entry);

    entry.key.port_no             = port;

    entry.key.type                = VTSS_ES0_TYPE_ISDX;
    entry.key.isdx_neq0           = VTSS_VCAP_BIT_1;
    entry.key.data.isdx.pcp.value = 0;  // FIXME
    entry.key.data.isdx.pcp.mask  = 0;  // FIXME -- wildcard

    entry.key.data.isdx.isdx = xc->isdx->sdx;

    entry.action.mpls_encap_idx = seg->u.out.encap_idx;
    entry.action.mpls_encap_len = srvl_bytes_to_encap_len(seg->u.out.encap_bytes);
    entry.action.esdx           = seg->u.out.esdx->sdx;

    VTSS_DG(VTSS_TRACE_GROUP_MPLS, "Attempting ES0 config, entry/segment %d, ISDX %d, ESDX %d, encap %d",
            idx, xc->isdx->sdx, seg->u.out.esdx->sdx, seg->u.out.encap_idx);

    data.key_size    = VTSS_VCAP_KEY_SIZE_FULL;
    data.u.es0.flags = 0;
    data.u.es0.entry = &entry;

    rc = vtss_vcap_add(vtss_state, &vtss_state->vcap.es0.obj, VTSS_ES0_USER_MPLS,
                       idx, VTSS_VCAP_ID_LAST, &data, FALSE);

    seg->u.out.has_es0 = TRUE;

    isdx_mask = VTSS_BIT(VTSS_CHIP_PORT(port));
    VTSS_MPLS_CHECK_RC(vtss_srvl_isdx_update_es0(vtss_state, TRUE, xc->isdx->sdx, isdx_mask));

    if (rc != VTSS_RC_OK) {
        VTSS_DG(VTSS_TRACE_GROUP_MPLS, "ES0 update FAILED, rc = %d. Attempting to delete entry.", rc);
        VTSS_MPLS_CHECK_RC(srvl_mpls_es0_free(vtss_state, idx));           // Clears seg->u.out.has_es0
    }

    return rc;
}



/* ----------------------------------------------------------------- *
 * Teardown of both ES0 and MPLS encapsulation entries
 * ----------------------------------------------------------------- */

static vtss_rc srvl_mpls_es0_encap_tear_all_recursive(vtss_state_t *vtss_state,
                                                      vtss_mpls_segment_idx_t idx, u8 depth)
{
    vtss_mpls_idxchain_iter_t    iter;
    vtss_mpls_idxchain_user_t    user;
    BOOL                         more;
    vtss_mpls_segment_internal_t *seg;

    VTSS_DG(VTSS_TRACE_GROUP_MPLS, "entry, segment %d, depth = %u", idx, depth);
    VTSS_MPLS_IDX_CHECK_SEGMENT(idx);
    VTSS_MPLS_CHECK(depth <= VTSS_MPLS_OUT_ENCAP_LABEL_CNT);

    seg = &SEG_I(idx);
    if (seg->u.out.has_es0) {
        VTSS_MPLS_CHECK_RC(srvl_mpls_es0_free(vtss_state, idx));
    }
    if (seg->u.out.has_encap) {
        VTSS_MPLS_CHECK_RC(srvl_mpls_encap_free(vtss_state, idx));
    }
    if (seg->u.out.esdx) {
        VTSS_MPLS_CHECK_RC(srvl_mpls_sdx_free(vtss_state, FALSE, seg->u.out.esdx));
        seg->u.out.esdx = NULL;
    }

    more = vtss_mpls_idxchain_get_first(vtss_state, &seg->clients, &iter, &user);
    while (more) {
        VTSS_MPLS_CHECK_RC(srvl_mpls_es0_encap_tear_all_recursive(vtss_state, user, depth + 1));
        more = vtss_mpls_idxchain_get_next(vtss_state, &iter, &user);
    }
    return VTSS_RC_OK;
}

static vtss_rc srvl_mpls_es0_encap_tear_all(vtss_state_t *vtss_state, vtss_mpls_segment_idx_t idx)
{
    return srvl_mpls_es0_encap_tear_all_recursive(vtss_state, idx, 1);
}



/* ----------------------------------------------------------------- *
 *  Global TC config
 * ----------------------------------------------------------------- */

static vtss_rc srvl_mpls_tc_conf_set(vtss_state_t *vtss_state,
                                     const vtss_mpls_tc_conf_t * const new_map)
{
    u8  map, qos, tc;
    u32 dp0, dp1;

    VTSS_DG(VTSS_TRACE_GROUP_MPLS, "entry");

    // QOS => TC:
    for (map = 0; map < VTSS_MPLS_QOS_TO_TC_MAP_CNT; map++) {
        for (qos = 0; qos < VTSS_MPLS_QOS_TO_TC_ENTRY_CNT; qos++) {
            vtss_state->mpls.tc_conf.qos_to_tc_map[map][qos].dp0_tc = new_map->qos_to_tc_map[map][qos].dp0_tc & 0x07;
            vtss_state->mpls.tc_conf.qos_to_tc_map[map][qos].dp1_tc = new_map->qos_to_tc_map[map][qos].dp1_tc & 0x07;
        }
    }

    // Commit each map to chip
    for (map = 0; map < VTSS_MPLS_QOS_TO_TC_MAP_CNT; map++) {
        dp0 = dp1 = 0;
        for (qos = VTSS_MPLS_QOS_TO_TC_MAP_CNT; qos > 0; qos--) {
            dp0 = (dp0 << 3) | vtss_state->mpls.tc_conf.qos_to_tc_map[map][qos - 1].dp0_tc;
            dp1 = (dp1 << 3) | vtss_state->mpls.tc_conf.qos_to_tc_map[map][qos - 1].dp1_tc;
        }
        SRVL_WR(VTSS_SYS_SYSTEM_MPLS_QOS_MAP_CFG(map),     dp0);
        SRVL_WR(VTSS_SYS_SYSTEM_MPLS_QOS_MAP_CFG(map + 8), dp1);
    }

    // TC => QOS:
    for (map = 0; map < VTSS_MPLS_TC_TO_QOS_MAP_CNT; map++) {
        for (tc = 0; tc < VTSS_MPLS_TC_TO_QOS_ENTRY_CNT; tc++) {
            vtss_state->mpls.tc_conf.tc_to_qos_map[map][tc].qos = new_map->tc_to_qos_map[map][tc].qos & 0x07;
            vtss_state->mpls.tc_conf.tc_to_qos_map[map][tc].dp  = new_map->tc_to_qos_map[map][tc].dp  & 0x01;
            SRVL_WR(VTSS_ANA_MPLS_TC_MAP_TC_MAP_TBL(map, tc),
                    vtss_state->mpls.tc_conf.tc_to_qos_map[map][tc].qos << 1 |
                    vtss_state->mpls.tc_conf.tc_to_qos_map[map][tc].dp);
        }
    }

    VTSS_DG(VTSS_TRACE_GROUP_MPLS, "exit");

    return VTSS_RC_OK;
}


/* ----------------------------------------------------------------- *
 *  L2 entries
 * ----------------------------------------------------------------- */

static void srvl_mpls_segment_hw_update(vtss_state_t *vtss_state, vtss_mpls_segment_idx_t idx);
static vtss_rc srvl_mpls_l2_set(vtss_state_t                 *vtss_state,
                                const vtss_mpls_l2_idx_t     idx,
                                const vtss_mpls_l2_t * const l2);

static vtss_rc srvl_mpls_l2_port_mode_update(vtss_state_t *vtss_state, vtss_port_no_t port)
{
    BOOL enable_section_oam = vtss_state->mpls.port_state[port].l2_section_oam_refcnt > 0;
    BOOL enable_mpls        = vtss_state->mpls.port_state[port].l2_refcnt             > 0  ||  enable_section_oam;

    // If we have > 0 segments, or section OAM is enabled: Make the port an LSR port and enable MPLS; otherwise disable
    SRVL_WRM(VTSS_ANA_PORT_PORT_CFG(VTSS_CHIP_PORT(port)),    enable_mpls ? VTSS_F_ANA_PORT_PORT_CFG_LSR_MODE    : 0, VTSS_F_ANA_PORT_PORT_CFG_LSR_MODE);
    SRVL_WRM(VTSS_SYS_SYSTEM_PORT_MODE(VTSS_CHIP_PORT(port)), enable_mpls ? VTSS_F_SYS_SYSTEM_PORT_MODE_MPLS_ENA : 0, VTSS_F_SYS_SYSTEM_PORT_MODE_MPLS_ENA);

    // If section OAM is enabled for at least one L2 entry, enable it in Serval-1; otherwise disable

    SRVL_WR(VTSS_ANA_MPLS_CL_SEGMENT_OAM(VTSS_CHIP_PORT(port)),
            (enable_section_oam ? VTSS_F_ANA_MPLS_CL_SEGMENT_OAM_SEGMENT_OAM_ENA : 0) |
            VTSS_F_ANA_MPLS_CL_SEGMENT_OAM_SEGMENT_OAM_LBL(MPLS_GAL_LABEL_VALUE)      |
            VTSS_F_ANA_MPLS_CL_SEGMENT_OAM_SEGMENT_OAM_QUEUE(MPLS_CPU_Q)              |
            VTSS_F_ANA_MPLS_CL_SEGMENT_OAM_SEGMENT_OAM_BOS);

    return VTSS_RC_OK;
}

static vtss_rc srvl_mpls_l2_alloc(vtss_state_t *vtss_state, vtss_mpls_l2_idx_t * const idx)
{
    vtss_mpls_l2_internal_t *l2;

    VTSS_MPLS_TAKE_FROM_FREELIST(mpls.l2_free_list, mpls.l2_conf, *idx);
    VTSS_DG(VTSS_TRACE_GROUP_MPLS, "Allocated L2 %d", *idx);

    l2 = &L2_I(*idx);
    memset(&l2->pub, 0, sizeof(l2->pub));
    l2->pub.port = VTSS_PORT_NO_NONE;
    VTSS_MPLS_IDXCHAIN_UNDEF(l2->users);
    VTSS_MPLS_IDX_UNDEF(l2->ll_idx_upstream);
    VTSS_MPLS_IDX_UNDEF(l2->ll_idx_downstream);
    return VTSS_RC_OK;
}

static vtss_rc srvl_mpls_l2_free(vtss_state_t *vtss_state, vtss_mpls_l2_idx_t idx)
{
    vtss_mpls_l2_internal_t *l2;

    VTSS_DG(VTSS_TRACE_GROUP_MPLS, "Freeing L2 %d", idx);

    VTSS_MPLS_IDX_CHECK_L2(idx);

    l2 = &L2_I(idx);

    // No segments must be using this index
    VTSS_MPLS_CHECK(VTSS_MPLS_IDXCHAIN_END(l2->users));

    if (l2->pub.section_oam) {
        VTSS_DG(VTSS_TRACE_GROUP_MPLS, "L2 %d still has section OAM enabled; disabling before free", idx);
        l2->pub.section_oam = FALSE;
        VTSS_MPLS_CHECK_RC(srvl_mpls_l2_set(vtss_state, idx, &l2->pub));
        l2->pub.section_oam = TRUE;
    }

    // Reset port so AIL debug code can see this entry is unused,
    // see base/ail/vtss_mpls_api.c:vtss_mpls_debug_print().
    l2->pub.port = VTSS_PORT_NO_NONE;

    VTSS_MPLS_RETURN_TO_FREELIST(mpls.l2_free_list, mpls.l2_conf, idx);

    return VTSS_RC_OK;
}

static vtss_rc srvl_mpls_l2_set(vtss_state_t                 *vtss_state,
                                const vtss_mpls_l2_idx_t     idx,
                                const vtss_mpls_l2_t * const l2_new)
{
    vtss_mpls_l2_internal_t   *l2_i;
    vtss_mpls_idxchain_iter_t iter;
    vtss_mpls_idxchain_user_t user;
    BOOL                      more;
    vtss_port_no_t            old_port;
    vtss_port_no_t            new_port;
    BOOL                      port_change;
    BOOL                      tear_oam;
    BOOL                      build_oam;
    BOOL                      has_oam;
    BOOL                      want_oam;

    VTSS_DG(VTSS_TRACE_GROUP_MPLS, "Configuring L2 %d", idx);

    VTSS_MPLS_IDX_CHECK_L2(idx);
    l2_i = &L2_I(idx);

    old_port    = l2_i->pub.port;
    new_port    = l2_new->port;
    port_change = (old_port != new_port) && (old_port != VTSS_PORT_NO_NONE);

    has_oam     = l2_i->pub.section_oam;
    want_oam    = l2_new->section_oam;

    memcpy(&l2_i->pub, l2_new, sizeof(l2_i->pub));

    if (has_oam && want_oam && port_change) {
        tear_oam  = TRUE;
        build_oam = TRUE;
    } else if (has_oam && !want_oam) {
        tear_oam  = TRUE;
        build_oam = FALSE;
    } else if (!has_oam && want_oam) {
        tear_oam  = FALSE;
        build_oam = TRUE;
    } else {
        tear_oam  = FALSE;
        build_oam = FALSE;
    }

    if (tear_oam) {
        VTSS_DG(VTSS_TRACE_GROUP_MPLS, "L2 %d has disabled downstream section OAM for port %u. Old OAM refcnt=%d",
                idx, old_port, vtss_state->mpls.port_state[old_port].l2_section_oam_refcnt);
        VTSS_MPLS_CHECK(vtss_state->mpls.port_state[old_port].l2_section_oam_refcnt > 0);
        vtss_state->mpls.port_state[old_port].l2_section_oam_refcnt--;

        // Attempt to free MLL if no downstream users are attached
        BOOL found_one = FALSE;

        more = vtss_mpls_idxchain_get_first(vtss_state, &l2_i->users, &iter, &user);
        while (more  &&  !found_one) {
            found_one = SEG_P(user).is_in  &&  !SEG_P(user).upstream;
            more      = vtss_mpls_idxchain_get_next(vtss_state, &iter, &user);
        }
        if (! found_one) {
            VTSS_MPLS_CHECK_RC(srvl_mpls_is0_mll_free(vtss_state, l2_i, FALSE));
        }
    }

    if (build_oam) {
        VTSS_DG(VTSS_TRACE_GROUP_MPLS, "L2 %d has enabled downstream section OAM for port %u, %s MLL. Old OAM refcnt for port=%d",
                idx, new_port, VTSS_MPLS_IDX_IS_UNDEF(l2_i->ll_idx_downstream) ? "allocating" : "existing",
                vtss_state->mpls.port_state[new_port].l2_section_oam_refcnt);
        if (VTSS_MPLS_IDX_IS_UNDEF(l2_i->ll_idx_downstream)) {
            VTSS_MPLS_CHECK_RC(srvl_mpls_is0_mll_alloc(vtss_state, l2_i, FALSE));
        }
        vtss_state->mpls.port_state[new_port].l2_section_oam_refcnt++;
    }

    if (VTSS_MPLS_IDX_IS_DEF(l2_i->ll_idx_upstream)) {
        VTSS_MPLS_CHECK_RC(srvl_mpls_is0_mll_update(vtss_state, l2_i, TRUE));
    }
    if (VTSS_MPLS_IDX_IS_DEF(l2_i->ll_idx_downstream)) {
        VTSS_MPLS_CHECK_RC(srvl_mpls_is0_mll_update(vtss_state, l2_i, FALSE));
    }

    // Port change    => tear, rebuild client segment HW
    // No port change => update client segment HW
    //
    // We do this for all L2 set operations, even if the configuration hasn't changed.
    // This is primarily a debug tool.

    if (port_change) {
        VTSS_DG(VTSS_TRACE_GROUP_MPLS, "L2 %d has changed port; rebuilding HW setup", idx);
    }
    more = vtss_mpls_idxchain_get_first(vtss_state, &l2_i->users, &iter, &user);
    while (more) {
        if (port_change) {
            vtss_mpls_segment_t *seg = &SEG_P(user);
            VTSS_MPLS_IDX_UNDEF(seg->l2_idx);
            VTSS_MPLS_CHECK(vtss_state->mpls.port_state[old_port].l2_refcnt > 0);
            vtss_state->mpls.port_state[old_port].l2_refcnt--;
            srvl_mpls_segment_hw_update(vtss_state, user);
            seg->l2_idx = idx;
            vtss_state->mpls.port_state[new_port].l2_refcnt++;
        }
        srvl_mpls_segment_hw_update(vtss_state, user);
        more = vtss_mpls_idxchain_get_next(vtss_state, &iter, &user);
    }

    VTSS_MPLS_CHECK_RC(srvl_mpls_l2_port_mode_update(vtss_state, new_port));
    if (port_change) {
        VTSS_MPLS_CHECK_RC(srvl_mpls_l2_port_mode_update(vtss_state, old_port));
    }

    return VTSS_RC_OK;
}

// Return: VTSS_RC_OK if L2 entry's HW allocation is OK
static vtss_rc srvl_mpls_l2_hw_alloc(vtss_state_t                  *vtss_state,
                                     const vtss_mpls_l2_idx_t      l2_idx,
                                     const vtss_mpls_segment_idx_t seg_idx)
{
    vtss_rc                      rc   = VTSS_RC_OK;
    vtss_mpls_l2_internal_t      *l2  = &L2_I(l2_idx);
    vtss_mpls_segment_internal_t *seg = &SEG_I(seg_idx);

    if (seg->pub.is_in) {
        if ( ( seg->pub.upstream  &&  VTSS_MPLS_IDX_IS_UNDEF(l2->ll_idx_upstream))  ||
             (!seg->pub.upstream  &&  VTSS_MPLS_IDX_IS_UNDEF(l2->ll_idx_downstream)) ) {
            rc = srvl_mpls_is0_mll_alloc(vtss_state, l2, seg->pub.upstream);
        }
        seg->u.in.has_mll = (rc == VTSS_RC_OK);
    }

    return rc;
}

/* When first in-segment is attached, an MLL will be allocated (if HW so
 * permits). If section OAM was enabled earlier, that entry is used
 */
static vtss_rc srvl_mpls_l2_segment_attach(vtss_state_t                  *vtss_state,
                                           const vtss_mpls_l2_idx_t      l2_idx,
                                           const vtss_mpls_segment_idx_t seg_idx)
{
    vtss_mpls_l2_internal_t      *l2;
    vtss_mpls_segment_internal_t *seg;

    VTSS_DG(VTSS_TRACE_GROUP_MPLS, "L2 %d: Attaching segment %d", l2_idx, seg_idx);

    // Rules:
    //   - Segment must not already be attached to an L2 entry
    //   - Segment must not be a client

    VTSS_MPLS_IDX_CHECK_L2(l2_idx);
    VTSS_MPLS_IDX_CHECK_SEGMENT(seg_idx);

    l2  = &L2_I(l2_idx);
    seg = &SEG_I(seg_idx);
    VTSS_MPLS_CHECK(VTSS_MPLS_IDX_IS_UNDEF(seg->pub.l2_idx));
    VTSS_MPLS_CHECK(VTSS_MPLS_IDX_IS_UNDEF(seg->pub.server_idx));
    VTSS_MPLS_CHECK(!seg->u.in.has_mll);    // Consistency check

    if (vtss_mpls_idxchain_insert_at_head(vtss_state, &l2->users, seg_idx) != VTSS_RC_OK) {
        VTSS_DG(VTSS_TRACE_GROUP_MPLS, "Couldn't insert segment %u in L2 idx %u user chain", seg_idx, l2_idx);
        return VTSS_RC_ERROR;
    }
    seg->pub.l2_idx = l2_idx;

    VTSS_MPLS_CHECK_RC(srvl_mpls_l2_hw_alloc(vtss_state, l2_idx, seg_idx));

    vtss_state->mpls.port_state[l2->pub.port].l2_refcnt++;

    VTSS_MPLS_CHECK_RC(srvl_mpls_l2_port_mode_update(vtss_state, l2->pub.port));

    srvl_mpls_segment_hw_update(vtss_state, seg_idx);

    VTSS_DG(VTSS_TRACE_GROUP_MPLS, "L2 %d: Attached segment %d", l2_idx, seg_idx);
    return VTSS_RC_OK;
}

static vtss_rc srvl_mpls_l2_segment_detach(vtss_state_t *vtss_state, const vtss_mpls_segment_idx_t seg_idx)
{
    vtss_mpls_l2_idx_t           l2_idx;
    vtss_mpls_l2_internal_t      *l2;
    vtss_mpls_segment_internal_t *seg;

    VTSS_DG(VTSS_TRACE_GROUP_MPLS, "entry, segment %d", seg_idx);

    // Rules:
    //   - Segment must be attached to an L2 entry

    VTSS_MPLS_IDX_CHECK_SEGMENT(seg_idx);
    seg    = &SEG_I(seg_idx);
    l2_idx = seg->pub.l2_idx;
    VTSS_MPLS_IDX_CHECK_L2(l2_idx);
    l2     = &L2_I(l2_idx);

    if (vtss_mpls_idxchain_remove_first(vtss_state, &l2->users, seg_idx) != VTSS_RC_OK) {
        VTSS_DG(VTSS_TRACE_GROUP_MPLS, "Couldn't remove segment %u from L2 idx %u user chain", seg_idx, l2_idx);
        return VTSS_RC_ERROR;
    }
    VTSS_MPLS_IDX_UNDEF(seg->pub.l2_idx);
    VTSS_MPLS_CHECK(vtss_state->mpls.port_state[l2->pub.port].l2_refcnt > 0);

    vtss_state->mpls.port_state[l2->pub.port].l2_refcnt--;

    VTSS_MPLS_CHECK_RC(srvl_mpls_l2_port_mode_update(vtss_state, l2->pub.port));

    if (seg->pub.is_in) {
        seg->u.in.has_mll = FALSE;
    }

    srvl_mpls_segment_hw_update(vtss_state, seg_idx);

    if (seg->pub.is_in  &&  vtss_state->mpls.port_state[l2->pub.port].l2_section_oam_refcnt == 0) {
        // If no section OAM is enabled *and* no other in-segment users exist with the same upstream/downstream
        // setting as the just-removed segment, free the HW resource

        vtss_mpls_idxchain_iter_t    iter;
        vtss_mpls_idxchain_user_t    user;
        BOOL                         more;
        BOOL                         found_one = FALSE;

        more = vtss_mpls_idxchain_get_first(vtss_state, &l2->users, &iter, &user);
        while (more  &&  !found_one) {
            found_one = SEG_P(user).is_in  &&  SEG_P(user).upstream == seg->pub.upstream;
            more      = vtss_mpls_idxchain_get_next(vtss_state, &iter, &user);
        }
        if (! found_one) {
            VTSS_MPLS_CHECK_RC(srvl_mpls_is0_mll_free(vtss_state, l2, seg->pub.upstream));
        }
    }

    return VTSS_RC_OK;
}



/* ----------------------------------------------------------------- *
 *  MPLS Segments
 * ----------------------------------------------------------------- */

static vtss_rc srvl_mpls_segment_alloc(vtss_state_t                    *vtss_state,
                                       BOOL                            in_seg,
                                       vtss_mpls_segment_idx_t * const idx)
{
    vtss_mpls_segment_internal_t *s;
    VTSS_MPLS_TAKE_FROM_FREELIST(mpls.segment_free_list, mpls.segment_conf, *idx);
    VTSS_DG(VTSS_TRACE_GROUP_MPLS, "Allocated %s-segment %d", in_seg ? "in" : "out", *idx);

    s = &vtss_state->mpls.segment_conf[*idx];
    memset(s, 0, sizeof(*s));

    s->pub.is_in       = in_seg;
    s->pub.label.value = VTSS_MPLS_LABEL_VALUE_DONTCARE;
    s->pub.label.tc    = VTSS_MPLS_TC_VALUE_DONTCARE;
    s->pub.label.ttl   = 255;
    s->pub.hqos_id     = VTSS_HQOS_ID_NONE;
    VTSS_MPLS_IDX_UNDEF(s->pub.l2_idx);
    VTSS_MPLS_IDX_UNDEF(s->pub.server_idx);
    VTSS_MPLS_IDX_UNDEF(s->pub.xc_idx);
    VTSS_MPLS_IDXCHAIN_UNDEF(s->clients);

    VTSS_MPLS_IDX_UNDEF(s->next_free);
    s->need_hw_alloc = TRUE;
    s->oam_active    = FALSE;

    if (in_seg) {
        s->u.in.has_mll        = FALSE;
        s->u.in.has_mlbs       = FALSE;
        s->u.in.vcap_user      = VTSS_IS0_USER_MPLS_MLBS_1;
        s->u.in.active_hqos_id = VTSS_HQOS_ID_NONE;
        VTSS_MPLS_IDX_UNDEF(s->u.in.vprofile_idx);
    }
    else {
        s->u.out.has_encap = FALSE;
        s->u.out.has_es0   = FALSE;
        s->u.out.esdx      = NULL;
        VTSS_MPLS_IDX_UNDEF(s->u.out.encap_idx);
        s->u.out.encap_bytes = 0;
    }

    return VTSS_RC_OK;
}

static vtss_rc srvl_mpls_segment_free(vtss_state_t *vtss_state, vtss_mpls_segment_idx_t idx)
{
    vtss_mpls_segment_internal_t *seg;
    VTSS_DG(VTSS_TRACE_GROUP_MPLS, "Freeing segment %d", idx);

    if (VTSS_MPLS_IDX_IS_UNDEF(idx)) {
        return VTSS_RC_OK;
    }
    VTSS_MPLS_IDX_CHECK_SEGMENT(idx);

    // Rules:
    //   - Segment must not be attached to an L2 entry
    //   - Segment must not be attached to an XC
    //   - Segment must not be a server
    //   - Segment must not be attached to a server

    seg = &SEG_I(idx);

    VTSS_MPLS_CHECK(VTSS_MPLS_IDX_IS_UNDEF(seg->pub.l2_idx));
    VTSS_MPLS_CHECK(VTSS_MPLS_IDX_IS_UNDEF(seg->pub.xc_idx));
    VTSS_MPLS_CHECK(VTSS_MPLS_IDXCHAIN_END(seg->clients));
    VTSS_MPLS_CHECK(VTSS_MPLS_IDX_IS_UNDEF(seg->pub.server_idx));

    VTSS_MPLS_RETURN_TO_FREELIST(mpls.segment_free_list, mpls.segment_conf, idx);
    return VTSS_RC_OK;
}

/** \brief Add/update label stacks for segment and any clients it has.
 *
 * Does not affect IS0 VCAP ordering.
 */
static vtss_rc srvl_mpls_segment_hw_label_stack_refresh_recursive(vtss_state_t *vtss_state,
                                                                  vtss_mpls_segment_idx_t idx, u8 depth)
{
    vtss_mpls_segment_internal_t *seg = &SEG_I(idx);
    vtss_mpls_idxchain_iter_t    iter;
    vtss_mpls_idxchain_user_t    user;
    BOOL                         more;

    VTSS_DG(VTSS_TRACE_GROUP_MPLS, "Refreshing %s-segment %d, depth = %u", seg->pub.is_in ? "in" : "out", idx, depth);

    VTSS_MPLS_CHECK(depth <= (seg->pub.is_in ? VTSS_MPLS_IN_ENCAP_LABEL_CNT : VTSS_MPLS_OUT_ENCAP_LABEL_CNT));

    if (VTSS_MPLS_IDX_IS_UNDEF(seg->pub.xc_idx)) {
        VTSS_DG(VTSS_TRACE_GROUP_MPLS, "Segment %d has no XC, cannot update label stack", idx);
        return VTSS_RC_OK;
    }

    // Update labels for this segment

    if (seg->pub.is_in) {
        if (seg->u.in.has_mlbs) {
            VTSS_MPLS_CHECK_RC(srvl_mpls_is0_mlbs_update(vtss_state, idx));
        }
    }
    else {
        if (seg->u.out.has_encap) {
            u16 old_encap_bytes = seg->u.out.encap_bytes;
            VTSS_MPLS_CHECK_RC(srvl_mpls_encap_update(vtss_state, idx));
            if (seg->u.out.has_encap && seg->u.out.encap_bytes != old_encap_bytes && XC_P(seg->pub.xc_idx).type == VTSS_MPLS_XC_TYPE_LER && seg->pub.pw_conf.is_pw) {
                if (vtss_srvl_evc_mpls_xc_update(vtss_state, seg->pub.xc_idx) != VTSS_RC_OK) {
                    VTSS_DG(VTSS_TRACE_GROUP_MPLS, "MPLS segment %d couldn't update client EVCs that use XC %d (encap length change)", idx, seg->pub.xc_idx);
                }
            }
        }
        if (seg->u.out.has_es0) {
            VTSS_MPLS_CHECK_RC(srvl_mpls_es0_update(vtss_state, idx));
        }
    }

    // Update clients

    more = vtss_mpls_idxchain_get_first(vtss_state, &seg->clients, &iter, &user);
    while (more) {
        VTSS_MPLS_CHECK_RC(srvl_mpls_segment_hw_label_stack_refresh_recursive(vtss_state, user, depth + 1));
        more = vtss_mpls_idxchain_get_next(vtss_state, &iter, &user);
    }

    return VTSS_RC_OK;
}

static vtss_rc srvl_mpls_xc_hw_alloc(vtss_state_t *vtss_state, vtss_mpls_xc_idx_t idx)
{
    vtss_mpls_xc_internal_t      *xc  = &XC_I(idx);
    vtss_mpls_segment_internal_t *seg = NULL;
    vtss_mpls_segment_state_t    in_state, out_state;
    BOOL                         need_isdx = FALSE;

    VTSS_DG(VTSS_TRACE_GROUP_MPLS, "XC %d, HW alloc, %s ISDX at entry", idx, xc->isdx ? "has " : "no");

    if (xc->isdx) {
        return VTSS_RC_OK;
    }

    switch (xc->pub.type) {
    case VTSS_MPLS_XC_TYPE_LER:
        if (VTSS_MPLS_IDX_IS_DEF(xc->pub.in_seg_idx)) {
            seg = &SEG_I(xc->pub.in_seg_idx);
            need_isdx = seg->pub.pw_conf.is_pw || (seg->pub.oam_conf.type == VTSS_MPLS_OAM_TYPE_GAL_MEP || seg->pub.oam_conf.type == VTSS_MPLS_OAM_TYPE_GAL_MIP);
        }
        break;
    case VTSS_MPLS_XC_TYPE_LSR:
        if (VTSS_MPLS_IDX_IS_DEF(xc->pub.in_seg_idx)  &&  VTSS_MPLS_IDX_IS_DEF(xc->pub.out_seg_idx)) {
            (void) srvl_mpls_segment_state_get(vtss_state, xc->pub.in_seg_idx,  &in_state);
            (void) srvl_mpls_segment_state_get(vtss_state, xc->pub.out_seg_idx, &out_state);
            need_isdx = in_state == VTSS_MPLS_SEGMENT_STATE_CONF  ||  out_state == VTSS_MPLS_SEGMENT_STATE_CONF;
        }
        break;
    }

    if (need_isdx) {
        VTSS_MPLS_CHECK_RC(srvl_mpls_sdx_alloc(vtss_state, TRUE, VTSS_PORT_NO_NONE, &xc->isdx));
        xc->pub.flow_handle = xc->isdx->sdx;
    }

    return VTSS_RC_OK;
}

static vtss_rc srvl_mpls_xc_hw_free(vtss_state_t *vtss_state, vtss_mpls_xc_idx_t idx)
{
    vtss_mpls_xc_internal_t      *xc = &XC_I(idx);
    vtss_mpls_segment_state_t    in_state, out_state;
    vtss_mpls_segment_internal_t *seg;
    BOOL                         free_isdx = TRUE;

    if (! xc->isdx) {
        return VTSS_RC_OK;
    }

    VTSS_DG(VTSS_TRACE_GROUP_MPLS, "XC %d, HW free. ISDX %d", idx, xc->isdx->sdx);

    switch (xc->pub.type) {
    case VTSS_MPLS_XC_TYPE_LER:
        break;
    case VTSS_MPLS_XC_TYPE_LSR:
        if (VTSS_MPLS_IDX_IS_DEF(xc->pub.in_seg_idx)  &&  VTSS_MPLS_IDX_IS_DEF(xc->pub.out_seg_idx)) {
            (void) srvl_mpls_segment_state_get(vtss_state, xc->pub.in_seg_idx,  &in_state);
            (void) srvl_mpls_segment_state_get(vtss_state, xc->pub.out_seg_idx, &out_state);
            free_isdx = in_state != VTSS_MPLS_SEGMENT_STATE_UP  &&  out_state != VTSS_MPLS_SEGMENT_STATE_UP;
            seg = &SEG_I(xc->pub.in_seg_idx);
            if (free_isdx  &&  seg->u.in.active_hqos_id != VTSS_HQOS_ID_NONE) {
                (void) VTSS_FUNC(hqos.isdx_del, seg->u.in.active_hqos_id, xc->isdx->sdx);
                seg->u.in.active_hqos_id = VTSS_HQOS_ID_NONE;
            }
        }
        break;
    }

    if (free_isdx) {
        VTSS_MPLS_CHECK_RC(srvl_mpls_sdx_free(vtss_state, TRUE, xc->isdx));
        xc->isdx = NULL;
        xc->pub.flow_handle = VTSS_ISDX_NONE;
    } else {
        VTSS_DG(VTSS_TRACE_GROUP_MPLS, "Not freeing ISDX %d; still used", xc->isdx->sdx);
    }

    return VTSS_RC_OK;

}

// Helper function for @srvl_mpls_segment_hw_update() and friends, don't call directly
static vtss_rc srvl_mpls_segment_hw_free_recursive(vtss_state_t *vtss_state, vtss_mpls_segment_idx_t idx)
{
    vtss_mpls_segment_internal_t *seg = &SEG_I(idx);
    vtss_mpls_idxchain_iter_t    iter;
    vtss_mpls_idxchain_user_t    user;
    BOOL                         more;

    VTSS_DG(VTSS_TRACE_GROUP_MPLS, "entry, segment %d", idx);

    if (seg->pub.is_in) {
        (void) srvl_mpls_vprofile_free(vtss_state, seg->u.in.vprofile_idx);
        VTSS_MPLS_IDX_UNDEF(seg->u.in.vprofile_idx);
        seg->u.in.active_hqos_id = VTSS_HQOS_ID_NONE;
    }

    (void) srvl_mpls_xc_hw_free(vtss_state, seg->pub.xc_idx);

    seg->need_hw_alloc = TRUE;

    if (seg->pub.pw_conf.is_pw && XC_P(seg->pub.xc_idx).type == VTSS_MPLS_XC_TYPE_LER) {
        if (vtss_srvl_evc_mpls_xc_update(vtss_state, seg->pub.xc_idx) != VTSS_RC_OK) {
            VTSS_DG(VTSS_TRACE_GROUP_MPLS, "MPLS segment %d couldn't update client EVCs that use XC %d", idx, seg->pub.xc_idx);
        }
    }

    more = vtss_mpls_idxchain_get_first(vtss_state, &seg->clients, &iter, &user);
    while (more) {
        VTSS_MPLS_CHECK_RC(srvl_mpls_segment_hw_free_recursive(vtss_state, user));
        more = vtss_mpls_idxchain_get_next(vtss_state, &iter, &user);
    }

    return VTSS_RC_OK;
}

// Helper function for @srvl_mpls_segment_hw_update() and friends, don't call directly
static vtss_rc srvl_mpls_segment_hw_free(vtss_state_t *vtss_state, vtss_mpls_segment_idx_t idx)
{
    vtss_mpls_segment_internal_t *seg = &SEG_I(idx);

    VTSS_DG(VTSS_TRACE_GROUP_MPLS, "entry, segment %d", idx);

    /* We do the actual tearing of MLBS/ES0/encap here since the tear
     * funcs are recursive, so there's no need to do it (potentially)
     * over and over in srvl_mpls_segment_hw_free_recursive().
     */

    if (seg->pub.is_in) {
        // We never free the MLL since it's owned by the L2 entry
        (void) srvl_mpls_is0_mlbs_tear_all(vtss_state, idx);
    }
    else {
        (void) srvl_mpls_es0_encap_tear_all(vtss_state, idx);
    }

    return srvl_mpls_segment_hw_free_recursive(vtss_state, idx);
}

// Helper function for @srvl_mpls_segment_hw_update(), don't call directly
static vtss_rc srvl_mpls_segment_hw_alloc_recursive(vtss_state_t *vtss_state,
                                                    vtss_mpls_segment_idx_t idx, u8 depth)
{
    vtss_mpls_idxchain_iter_t    iter;
    vtss_mpls_idxchain_user_t    user;
    BOOL                         more;
    vtss_mpls_segment_state_t    state;
    vtss_mpls_segment_internal_t *seg       = &SEG_I(idx);
    vtss_mpls_xc_internal_t      *xc        = &XC_I(seg->pub.xc_idx);
    BOOL                         is_lsr     = xc->pub.type == VTSS_MPLS_XC_TYPE_LSR;
    BOOL                         is_ler_pw  = xc->pub.type == VTSS_MPLS_XC_TYPE_LER  &&  seg->pub.pw_conf.is_pw;
    BOOL                         is_lsp_oam = FALSE;
    vtss_port_no_t               l2_port;

    VTSS_DG(VTSS_TRACE_GROUP_MPLS, "entry: %s-segment %d, depth %d, alloc needed: %s", (seg->pub.is_in ? "In" : "Out"), idx, depth, seg->need_hw_alloc ? "Yes" : "no");
    VTSS_MPLS_CHECK(depth <= VTSS_MPLS_IN_ENCAP_LABEL_CNT);

    (void) srvl_mpls_segment_state_get(vtss_state, idx, &state);
    if (state != VTSS_MPLS_SEGMENT_STATE_CONF) {
        VTSS_DG(VTSS_TRACE_GROUP_MPLS, "Segment %d is not CONF but %s; cannot allocate HW", idx, srvl_mpls_segment_state_to_str(state));
        return VTSS_RC_ERROR;
    }

    if (VTSS_MPLS_IDX_IS_DEF(seg->pub.server_idx)) {
        (void) srvl_mpls_segment_state_get(vtss_state, seg->pub.server_idx, &state);
        if (state != VTSS_MPLS_SEGMENT_STATE_UP) {
            VTSS_DG(VTSS_TRACE_GROUP_MPLS, "Server %d is not UP but %s; cannot allocate HW", seg->pub.server_idx, srvl_mpls_segment_state_to_str(state));
            return VTSS_RC_ERROR;
        }
    }

    VTSS_MPLS_CHECK_RC(vtss_cmn_mpls_port_no_get(vtss_state, idx, &l2_port));

    seg->need_hw_alloc = FALSE;

    // Allocate XC HW resources or bail out
    if (srvl_mpls_xc_hw_alloc(vtss_state, seg->pub.xc_idx) != VTSS_RC_OK) {
        goto err_out;
    }

    if (seg->pub.is_in) {
        is_lsp_oam = seg->pub.oam_conf.type == VTSS_MPLS_OAM_TYPE_GAL_MEP  ||  seg->pub.oam_conf.type == VTSS_MPLS_OAM_TYPE_GAL_MIP;

        if (xc->isdx &&  xc->isdx->port_no == VTSS_PORT_NO_NONE) {
            // The ISDX alloc is with a dummy port in case the alloc happens due to an out segment, so we fix it up here when we know the in-segment L2 port.
            // That dummy port also prevents the counters from being cleared, so we do it here as well.
            xc->isdx->port_no = l2_port;
            (void) VTSS_FUNC(evc.sdx_counters_update, xc->isdx, NULL, NULL, TRUE);
        }

        // For LSR: use fixed VProfile;
        // For LER PW: allocate VProfile or bail out
        if (is_lsr) {
            seg->u.in.vprofile_idx = VTSS_MPLS_VPROFILE_LSR_IDX;
        }
        else if (is_ler_pw) {
            vtss_mpls_vprofile_t *vp;
            BOOL                 is_short_pipe = xc->pub.tc_tunnel_mode == VTSS_MPLS_TUNNEL_MODE_SHORT_PIPE;

            (void) srvl_mpls_vprofile_alloc(vtss_state, &seg->u.in.vprofile_idx);
            if (VTSS_MPLS_IDX_IS_UNDEF(seg->u.in.vprofile_idx)) {
                VTSS_DG(VTSS_TRACE_GROUP_MPLS, "No VProfile");
                goto err_out;
            }
            vp                  = &VP_P(seg->u.in.vprofile_idx);
            vp->s1_s2_enable    = TRUE;
            vp->ptp_dly1_enable = TRUE;
            vp->port            = l2_port;
            vp->recv_enable     = TRUE;
            vp->learn_enable    = TRUE;            // EVC MEPs need this for things like LT to work

            vp->vlan_aware          = is_short_pipe;
            vp->map_eth_cos_enable  = is_short_pipe;
            vp->map_dscp_cos_enable = FALSE;

            (void) srvl_mpls_vprofile_hw_update(vtss_state, seg->u.in.vprofile_idx);
        }

        // LSR, LER PW, LSP OAM: Build label stack for ourself or bail out
        if (is_lsr  ||  is_ler_pw  ||  is_lsp_oam) {
            (void) srvl_mpls_is0_mlbs_update(vtss_state, idx);
            if (!seg->u.in.has_mlbs) {
                VTSS_DG(VTSS_TRACE_GROUP_MPLS, "MLBS update failed");
                goto err_out;
            }
        }

        if (is_lsr  &&  seg->pub.hqos_id != VTSS_HQOS_ID_NONE) {
            if (seg->u.in.active_hqos_id != VTSS_HQOS_ID_NONE) {
                VTSS_E("Seg %d: HQoS ID %d unexpectedly active", idx, seg->u.in.active_hqos_id);
                seg->u.in.active_hqos_id = VTSS_HQOS_ID_NONE;
            }
            if (VTSS_FUNC(hqos.map, seg->pub.hqos_id, TRUE) != VTSS_RC_OK) {      // Set port as NNI
                VTSS_DG(VTSS_TRACE_GROUP_MPLS, "HQoS mapping failed for HQoS ID %u", seg->pub.hqos_id);
            } else if (VTSS_FUNC(hqos.isdx_add, seg->pub.hqos_id, xc->isdx->sdx, FALSE) != VTSS_RC_OK) {
                VTSS_DG(VTSS_TRACE_GROUP_MPLS, "HQoS failed to add ISDX %u to HQoS ID %u", xc->isdx->sdx, seg->pub.hqos_id);
            } else {
                seg->u.in.active_hqos_id = seg->pub.hqos_id;
            }
        }
    }
    else {
        // Allocate entries or bail out.
        // Note order: ES0 depends on encap and ESDX

        VTSS_DG(VTSS_TRACE_GROUP_MPLS, "out-seg: is_lsr: %d   is_ler_pw: %d  has_encap: %d  has_es0: %d  has_esdx: %d",
                is_lsr, is_ler_pw, seg->u.out.has_encap, seg->u.out.has_es0, seg->u.out.esdx != NULL);

        if ((is_lsr || is_ler_pw)  &&  !seg->u.out.has_encap) {
            (void) srvl_mpls_encap_alloc(vtss_state, idx);
            if (!seg->u.out.has_encap) {
                VTSS_DG(VTSS_TRACE_GROUP_MPLS, "MPLS Encap alloc failed");
                goto err_out;
            }
        }

        if (is_lsr  &&  seg->u.out.esdx == NULL) {
            (void) srvl_mpls_sdx_alloc(vtss_state, FALSE, l2_port, &seg->u.out.esdx);
            if (seg->u.out.esdx == NULL) {
                VTSS_DG(VTSS_TRACE_GROUP_MPLS, "MPLS ESDX alloc failed");
                goto err_out;
            }
        }

        if (is_lsr  &&  !seg->u.out.has_es0) {
            (void) srvl_mpls_es0_update(vtss_state, idx);
            if (!seg->u.out.has_es0) {
                VTSS_DG(VTSS_TRACE_GROUP_MPLS, "MPLS ES0 update failed");
                goto err_out;
            }
        }
    }

    if (is_ler_pw) {
        if (vtss_srvl_evc_mpls_xc_update(vtss_state, seg->pub.xc_idx) != VTSS_RC_OK) {
            VTSS_DG(VTSS_TRACE_GROUP_MPLS, "MPLS segment %d couldn't update client EVCs that use XC %d", idx, seg->pub.xc_idx);
            goto err_out;
        }
    }

    // Go through all clients
    more = vtss_mpls_idxchain_get_first(vtss_state, &seg->clients, &iter, &user);
    while (more) {
        VTSS_MPLS_CHECK_RC(srvl_mpls_segment_hw_alloc_recursive(vtss_state, user, depth + 1));
        more = vtss_mpls_idxchain_get_next(vtss_state, &iter, &user);
    }

    VTSS_DG(VTSS_TRACE_GROUP_MPLS, "Segment %d HW alloc succeeded", idx);
    return VTSS_RC_OK;

err_out:
    VTSS_DG(VTSS_TRACE_GROUP_MPLS, "Segment %d HW alloc FAILED. Will free HW resources", idx);
    (void) srvl_mpls_segment_hw_free(vtss_state, idx);
    return VTSS_RC_ERROR;
}

// Helper function for @srvl_mpls_segment_hw_update(), don't call directly
static vtss_rc srvl_mpls_segment_hw_alloc(vtss_state_t *vtss_state, vtss_mpls_segment_idx_t idx)
{
    return srvl_mpls_segment_hw_alloc_recursive(vtss_state, idx, 1);
}

#define SETRC(expr) \
    do { \
        if ((rc2 = (expr)) != VTSS_RC_OK) { \
            rc = rc2; \
            VTSS_DG(VTSS_TRACE_GROUP_MPLS, "Fail, rc=%d", rc); \
        } \
    } while (0)

// Helper function for @srvl_mpls_segment_hw_update(), don't call directly
static vtss_rc srvl_mpls_segment_hw_refresh(vtss_state_t *vtss_state, vtss_mpls_segment_idx_t idx)
{
    /* First check if OAM HW allocation requirements have changed. The rules are:
     *   - Only for in-segments
     *   - Not for PWs
     *   - LSP MEP OAM: Need XC ISDX + MLBS, so alloc or free
     *   - LSP MIP OAM on LER XC: As above
     *   - LSP MIP OAM on LSR XC: Already has ISDX + MLBS due to LSR operation, so don't touch
     */

    vtss_rc                      rc   = VTSS_RC_OK, rc2;
    vtss_mpls_segment_internal_t *seg = &SEG_I(idx);
    vtss_mpls_xc_internal_t      *xc  = &XC_I(seg->pub.xc_idx);

    const BOOL is_lsp_oam    = seg->pub.oam_conf.type == VTSS_MPLS_OAM_TYPE_GAL_MEP  ||  seg->pub.oam_conf.type == VTSS_MPLS_OAM_TYPE_GAL_MIP;
    const BOOL is_ler_in_seg = xc->pub.type == VTSS_MPLS_XC_TYPE_LER && seg->pub.is_in;
    const BOOL is_lsr_in_seg = xc->pub.type == VTSS_MPLS_XC_TYPE_LSR && seg->pub.is_in;

    if (is_ler_in_seg && !seg->pub.pw_conf.is_pw) {
        const BOOL xc_has_hw = xc->isdx != NULL;
        if (is_lsp_oam) {
            if (!xc_has_hw) {
                VTSS_DG(VTSS_TRACE_GROUP_MPLS, "In-segment %d needs XC and MLBS for LSP OAM; allocating", idx);
                seg->oam_active = FALSE;
                if ( (rc = srvl_mpls_xc_hw_alloc(vtss_state, seg->pub.xc_idx)) == VTSS_RC_OK) {
                    seg->u.in.vprofile_idx = VTSS_MPLS_VPROFILE_OAM_IDX;
                    SETRC(srvl_mpls_is0_mlbs_update(vtss_state, idx));
                    seg->oam_active = seg->u.in.has_mlbs;
                }
                VTSS_DG(VTSS_TRACE_GROUP_MPLS, "In-segment %d: Allocation of XC and MLBS for LSP OAM %s", idx, seg->oam_active ? "succeeded" : "FAILED");
            }
        } else if (xc_has_hw) {
            VTSS_DG(VTSS_TRACE_GROUP_MPLS, "In-segment %d has disabled LSP OAM, so no need for XC and MLBS", idx);
            SETRC(srvl_mpls_xc_hw_free(vtss_state, seg->pub.xc_idx));
            SETRC(srvl_mpls_is0_mlbs_free(vtss_state, idx));
            seg->oam_active = FALSE;
        }
    }

    // Then update VProfile for PW in-segment, if relevant, and label stack for ourself and clients

    if (is_ler_in_seg && seg->pub.pw_conf.is_pw) {
        const BOOL            is_short_pipe = xc->pub.tc_tunnel_mode == VTSS_MPLS_TUNNEL_MODE_SHORT_PIPE;
        vtss_mpls_vprofile_t  *vp           = &VP_P(seg->u.in.vprofile_idx);
        vp->vlan_aware                      = is_short_pipe;
        vp->map_eth_cos_enable              = is_short_pipe;

        SETRC(srvl_mpls_vprofile_hw_update(vtss_state, seg->u.in.vprofile_idx));
    }

    SETRC(srvl_mpls_segment_hw_label_stack_refresh_recursive(vtss_state, idx, 1));

    // And finally update HQoS, if changed
    if (is_lsr_in_seg && seg->pub.hqos_id != seg->u.in.active_hqos_id) {
        VTSS_DG(VTSS_TRACE_GROUP_MPLS, "In-segment %d: Attempt HQoS ID %d -> %d", idx, seg->u.in.active_hqos_id, seg->pub.hqos_id);
        if (seg->u.in.active_hqos_id != VTSS_HQOS_ID_NONE) {
            SETRC(VTSS_FUNC(hqos.isdx_del, seg->u.in.active_hqos_id, xc->isdx->sdx));
            seg->u.in.active_hqos_id = VTSS_HQOS_ID_NONE;
        }

        if (seg->pub.hqos_id != VTSS_HQOS_ID_NONE) {
            SETRC(VTSS_FUNC(hqos.map, seg->pub.hqos_id, TRUE));
            if (rc == VTSS_RC_OK) {
                SETRC(VTSS_FUNC(hqos.isdx_add, seg->pub.hqos_id, xc->isdx->sdx, FALSE));
                if (rc == VTSS_RC_OK) {
                    seg->u.in.active_hqos_id = seg->pub.hqos_id;
                }
            }
        }
    }

    return rc;
}

#undef SETRC

static void srvl_mpls_segment_hw_update(vtss_state_t *vtss_state, vtss_mpls_segment_idx_t idx)
{
    vtss_rc                   rc = VTSS_RC_OK;  // Kill silly compiler warning
    vtss_mpls_segment_state_t state;

    (void) srvl_mpls_segment_state_get(vtss_state, idx, &state);
    VTSS_DG(VTSS_TRACE_GROUP_MPLS, "Update of segment %d, entry state %s", idx, srvl_mpls_segment_state_to_str(state));

    switch (state) {
    case VTSS_MPLS_SEGMENT_STATE_UNCONF:
        rc = srvl_mpls_segment_hw_free(vtss_state, idx);
        break;
    case VTSS_MPLS_SEGMENT_STATE_CONF:
        SEG_I(idx).need_hw_alloc = TRUE;
        rc = srvl_mpls_segment_hw_alloc(vtss_state, idx);
        break;
    case VTSS_MPLS_SEGMENT_STATE_UP:
        rc = srvl_mpls_segment_hw_refresh(vtss_state, idx);
        break;
    }

    (void) srvl_mpls_segment_state_get(vtss_state, idx, &state);
    VTSS_DG(VTSS_TRACE_GROUP_MPLS, "Update of segment %d done, exit state %s, subfunction rc = %d", idx, srvl_mpls_segment_state_to_str(state), rc);
}

static vtss_rc srvl_mpls_segment_set_validate(vtss_state_t                      *vtss_state,
                                              const vtss_mpls_segment_idx_t     idx,
                                              const vtss_mpls_segment_t * const seg)
{
    vtss_mpls_segment_internal_t *seg_i;
    u8                           depth;

    VTSS_MPLS_IDX_CHECK_SEGMENT(idx);
    seg_i = &SEG_I(idx);

    // Rules:
    //   - Can't change L2 index once set (but can detach with another func call)
    //   - Can't change server index once set (but can detach with another func call)
    //   - Can't change XC index once set (but can detach with another func call)
    //   - Can't change in/out type
    //   - Can't change upstream flag if attached to L2
    //   - Can't change between PW/LSP type if attached to XC
    //   - Can only enable HQoS for in-segment attached to LSR XC
    //   - Can only enable VCCVx OAM for PW
    //   - Can only enable MEP/MIP OAM for non-PW LSP
    //   - VCCV1 must have Control Word enabled
    //   TBD: - VCCV2 must have Control Word enabled (Serval limitation: Must have ACh for OAM HW detection)
    //   TBD: - VCCV3 must have Control Word enabled (Serval limitation: Must have ACh for OAM HW detection)
    //   - VCCV4 must not have Control Word
    //   - Can't enable VCCV4/LSP MEP/LSP MIP for label depth > 2 (Serval limit.). Check isn't performed unless there's a server or L2 entry.

#define TST_IDX(x)     VTSS_MPLS_CHECK(VTSS_MPLS_IDX_IS_UNDEF(seg_i->pub.x)   || (seg->x == seg_i->pub.x))
#define TST_DEP(idx,x) VTSS_MPLS_CHECK(VTSS_MPLS_IDX_IS_UNDEF(seg_i->pub.idx) || (seg->x == seg_i->pub.x))

    TST_IDX(l2_idx);
    TST_IDX(server_idx);
    TST_IDX(xc_idx);
    VTSS_MPLS_CHECK(seg->is_in == seg_i->pub.is_in);
    TST_DEP(l2_idx, upstream);
    TST_DEP(xc_idx, pw_conf.is_pw);

    if (seg->hqos_id != VTSS_HQOS_ID_NONE  &&  VTSS_MPLS_IDX_IS_DEF(seg->xc_idx)) {
        VTSS_MPLS_CHECK(seg->is_in  &&  XC_P(seg->xc_idx).type == VTSS_MPLS_XC_TYPE_LSR);
    }

    switch (seg->oam_conf.type) {
    case VTSS_MPLS_OAM_TYPE_VCCV1:
        VTSS_MPLS_CHECK(seg->pw_conf.is_pw);
        VTSS_MPLS_CHECK(seg->pw_conf.process_cw);
        break;

    case VTSS_MPLS_OAM_TYPE_VCCV2:
        VTSS_MPLS_CHECK(seg->pw_conf.is_pw);
        // TBD: VTSS_MPLS_CHECK(seg->pw_conf.process_cw);
        break;

    case VTSS_MPLS_OAM_TYPE_VCCV3:
        VTSS_MPLS_CHECK(seg->pw_conf.is_pw);
        // TBD: VTSS_MPLS_CHECK(seg->pw_conf.process_cw);
        break;

    case VTSS_MPLS_OAM_TYPE_VCCV4:
        (void) vtss_cmn_mpls_find_ultimate_server(vtss_state, idx, &depth);
        VTSS_MPLS_CHECK(seg->pw_conf.is_pw);
        VTSS_MPLS_CHECK(!seg->pw_conf.process_cw);
        if (VTSS_MPLS_IDX_IS_DEF(seg->l2_idx) || VTSS_MPLS_IDX_IS_DEF(seg->server_idx)) {
            VTSS_MPLS_CHECK(depth < 3);
        }
        break;

    case VTSS_MPLS_OAM_TYPE_GAL_MEP:
    case VTSS_MPLS_OAM_TYPE_GAL_MIP:
        (void) vtss_cmn_mpls_find_ultimate_server(vtss_state, idx, &depth);
        VTSS_MPLS_CHECK(!seg->pw_conf.is_pw);
        if (VTSS_MPLS_IDX_IS_DEF(seg->l2_idx) || VTSS_MPLS_IDX_IS_DEF(seg->server_idx)) {
            VTSS_MPLS_CHECK(depth < 3);
        }
        break;

    default:
        break;
    }

#undef TST_IDX
#undef TST_DEP
    return VTSS_RC_OK;
}

static vtss_rc srvl_mpls_segment_set(vtss_state_t                      *vtss_state,
                                     const vtss_mpls_segment_idx_t     idx,
                                     const vtss_mpls_segment_t * const seg)
{
    vtss_mpls_segment_internal_t *seg_i;
    BOOL                         update;
    vtss_mpls_segment_state_t    state;

    VTSS_DG(VTSS_TRACE_GROUP_MPLS, "Configuring segment %d", idx);

    VTSS_MPLS_CHECK_RC(srvl_mpls_segment_set_validate(vtss_state, idx, seg));

    seg_i = &SEG_I(idx);

    // Can change label, E-/L-LSP type, TC, HQoS, OAM

    update = (seg->label.value    != seg_i->pub.label.value)    ||
             (seg->label.tc       != seg_i->pub.label.tc)       ||
             (seg->e_lsp          != seg_i->pub.e_lsp)          ||
             (seg->tc_qos_map_idx != seg_i->pub.tc_qos_map_idx) ||
             (seg->oam_conf.type  != seg_i->pub.oam_conf.type);

    if (seg->is_in) {
        update = update ||
            (seg->l_lsp_cos       != seg_i->pub.l_lsp_cos)  ||
            (seg->hqos_id         != seg_i->pub.hqos_id);
    }
    else {
        update = update ||
            (seg->label.ttl       != seg_i->pub.label.ttl);
    }

    if (seg->pw_conf.is_pw) {
        update = update  ||
            (seg->pw_conf.process_cw != seg_i->pub.pw_conf.process_cw)  ||
            (seg->pw_conf.cw         != seg_i->pub.pw_conf.cw);
    }

    seg_i->pub = *seg;

    (void) srvl_mpls_segment_state_get(vtss_state, idx, &state);
    if (state != VTSS_MPLS_SEGMENT_STATE_UP  ||  update) {
        srvl_mpls_segment_hw_update(vtss_state, idx);
    }

    return VTSS_RC_OK;
}

static vtss_rc srvl_mpls_segment_server_attach(vtss_state_t                  *vtss_state,
                                               const vtss_mpls_segment_idx_t idx,
                                               const vtss_mpls_segment_idx_t srv_idx)
{
    vtss_mpls_segment_t *seg;
    vtss_mpls_segment_t *srv_seg;
    u8                  depth;

    VTSS_DG(VTSS_TRACE_GROUP_MPLS, "Attaching segment %d to server segment %d", idx, srv_idx);

    VTSS_MPLS_IDX_CHECK_SEGMENT(idx);
    VTSS_MPLS_IDX_CHECK_SEGMENT(srv_idx);

    seg     = &SEG_P(idx);
    srv_seg = &SEG_P(srv_idx);

    // Rules:
    //   - Segment must not already be attached to a server
    //   - Both segments must be attached to XCs
    //   - The XCs can't be the same
    //   - The segments must be different
    //   - Our HW can handle a limited label stack depth; don't allow more
    //   - We can't use an LSR XC as server
    //  (- The segment can't already be in server's client list. Invariant
    //     check; shouldn't be possible since seg can't be attached to server in
    //     the first place)

    VTSS_MPLS_CHECK(VTSS_MPLS_IDX_IS_UNDEF(seg->server_idx));
    VTSS_MPLS_CHECK(VTSS_MPLS_IDX_IS_DEF(seg->xc_idx));
    VTSS_MPLS_CHECK(VTSS_MPLS_IDX_IS_DEF(srv_seg->xc_idx));
    VTSS_MPLS_CHECK(XC_P(srv_seg->xc_idx).type != VTSS_MPLS_XC_TYPE_LSR);
    VTSS_MPLS_CHECK(seg->xc_idx != srv_seg->xc_idx);
    VTSS_MPLS_CHECK(idx != srv_idx);
    (void) vtss_cmn_mpls_find_ultimate_server(vtss_state, srv_idx, &depth);
    VTSS_MPLS_CHECK(depth <= (seg->is_in ? VTSS_MPLS_IN_ENCAP_LABEL_CNT : VTSS_MPLS_OUT_ENCAP_LABEL_CNT));
    VTSS_MPLS_CHECK(!vtss_mpls_idxchain_find(vtss_state, &SEG_I(srv_idx).clients, idx));

    VTSS_MPLS_CHECK_RC(vtss_mpls_idxchain_append_to_tail(vtss_state, &SEG_I(srv_idx).clients, idx));
    seg->server_idx = srv_idx;

    srvl_mpls_segment_hw_update(vtss_state, idx);

    return VTSS_RC_OK;
}

static vtss_rc srvl_mpls_segment_server_detach(vtss_state_t *vtss_state, const vtss_mpls_segment_idx_t idx)
{
    vtss_mpls_segment_t *seg;

    // Rules:
    //   - Segment must be attached to a server
    //  (- Segment must be in server's client list -- consistency check)

    VTSS_MPLS_IDX_CHECK_SEGMENT(idx);
    seg = &SEG_P(idx);
    VTSS_MPLS_IDX_CHECK_SEGMENT(seg->server_idx);

    VTSS_DG(VTSS_TRACE_GROUP_MPLS, "Detaching segment %d from server segment %d", idx, seg->server_idx);
    VTSS_MPLS_CHECK_RC(vtss_mpls_idxchain_remove_first(vtss_state, &SEG_I(seg->server_idx).clients, idx));
    VTSS_MPLS_IDX_UNDEF(seg->server_idx);

    srvl_mpls_segment_hw_update(vtss_state, idx);

    return VTSS_RC_OK;
}



/* ----------------------------------------------------------------- *
 *  XC entries
 * ----------------------------------------------------------------- */

static vtss_rc srvl_mpls_xc_alloc(vtss_state_t               *vtss_state,
                                  vtss_mpls_xc_type_t        type,
                                  vtss_mpls_xc_idx_t * const idx)
{
    vtss_mpls_xc_internal_t *xc;
    VTSS_DG(VTSS_TRACE_GROUP_MPLS, "entry");

    VTSS_MPLS_TAKE_FROM_FREELIST(mpls.xc_free_list, mpls.xc_conf, *idx);

    xc = &vtss_state->mpls.xc_conf[*idx];
    xc->isdx = NULL;
    VTSS_MPLS_IDX_UNDEF(xc->next_free);
    VTSS_MPLS_IDXCHAIN_UNDEF(xc->mc_chain);

    xc->pub.type = type;
    VTSS_MPLS_IDX_UNDEF(xc->pub.in_seg_idx);
    VTSS_MPLS_IDX_UNDEF(xc->pub.out_seg_idx);

    return VTSS_RC_OK;
}

static vtss_rc srvl_mpls_xc_free(vtss_state_t *vtss_state, vtss_mpls_xc_idx_t idx)
{
    vtss_mpls_xc_internal_t *xc;
    VTSS_DG(VTSS_TRACE_GROUP_MPLS, "entry, XC=%d", idx);

    if (VTSS_MPLS_IDX_IS_UNDEF(idx)) {
        return VTSS_RC_OK;
    }

    VTSS_MPLS_IDX_CHECK_XC(idx);

    xc = &XC_I(idx);
    xc->isdx = NULL;
    VTSS_MPLS_IDX_UNDEF(xc->pub.in_seg_idx);
    VTSS_MPLS_IDX_UNDEF(xc->pub.out_seg_idx);

    VTSS_MPLS_RETURN_TO_FREELIST(mpls.xc_free_list, mpls.xc_conf, idx);

    return VTSS_RC_OK;
}

static vtss_rc srvl_mpls_xc_set(vtss_state_t                 *vtss_state,
                                const vtss_mpls_xc_idx_t     idx,
                                const vtss_mpls_xc_t * const xc)
{
    vtss_mpls_xc_t *old_xc;

    VTSS_DG(VTSS_TRACE_GROUP_MPLS, "entry, XC=%d", idx);
    VTSS_MPLS_IDX_CHECK_XC(idx);
    old_xc = &XC_P(idx);

    // Rules:
    //   - Can't change indices once set
    //   - Can't change type

#define TST_IDX(x)     VTSS_MPLS_CHECK(VTSS_MPLS_IDX_IS_UNDEF(old_xc->x) || (xc->x == old_xc->x))

    VTSS_MPLS_CHECK(old_xc->type == xc->type);
    TST_IDX(in_seg_idx);
    TST_IDX(out_seg_idx);

#undef TST_IDX


    if (xc->tc_tunnel_mode != old_xc->tc_tunnel_mode  ||  xc->ttl_tunnel_mode != old_xc->ttl_tunnel_mode) {
        old_xc->tc_tunnel_mode  = xc->tc_tunnel_mode;
        old_xc->ttl_tunnel_mode = xc->ttl_tunnel_mode;
        if (VTSS_MPLS_IDX_IS_DEF(old_xc->in_seg_idx)) {
            srvl_mpls_segment_hw_update(vtss_state, old_xc->in_seg_idx);
        }
        if (VTSS_MPLS_IDX_IS_DEF(old_xc->out_seg_idx)) {
            srvl_mpls_segment_hw_update(vtss_state, old_xc->out_seg_idx);
        }
    }

    return VTSS_RC_OK;
}

static vtss_rc srvl_mpls_xc_segment_attach(vtss_state_t                  *vtss_state,
                                           const vtss_mpls_xc_idx_t      xc_idx,
                                           const vtss_mpls_segment_idx_t seg_idx)
{
    vtss_mpls_xc_t               *xc;
    vtss_mpls_segment_internal_t *seg;

    VTSS_DG(VTSS_TRACE_GROUP_MPLS, "XC %d: Attaching segment %d", xc_idx, seg_idx);

    VTSS_MPLS_IDX_CHECK_XC(xc_idx);
    VTSS_MPLS_IDX_CHECK_SEGMENT(seg_idx);

    xc  = &XC_P(xc_idx);
    seg = &SEG_I(seg_idx);

    // Rules:
    //   - Segment must not already be attached to an XC
    //   - Can't overwrite an already attached segment of same kind
    //   - Segment must match XC type (e.g. "only one segment for LERs")
    //  (- Segment can't have clients yet. Invariant check)
    //  (- Segment can't be attached to a server segment yet. Invariant check)
    //   - Segment configuration must be internally consistent

    VTSS_MPLS_CHECK(VTSS_MPLS_IDX_IS_UNDEF(seg->pub.xc_idx));
    VTSS_MPLS_CHECK(VTSS_MPLS_IDXCHAIN_END(seg->clients));
    VTSS_MPLS_CHECK(VTSS_MPLS_IDX_IS_UNDEF(seg->pub.server_idx));

    switch (xc->type) {
    case VTSS_MPLS_XC_TYPE_LSR:
        // No check necessary
        break;
    case VTSS_MPLS_XC_TYPE_LER:
        // Either an in- or an out-segment, but not both:
        VTSS_MPLS_CHECK(VTSS_MPLS_IDX_IS_UNDEF(seg->pub.is_in ? xc->out_seg_idx : xc->in_seg_idx));
        break;
    }

    VTSS_MPLS_CHECK_RC(srvl_mpls_segment_set_validate(vtss_state, seg_idx, &seg->pub));

    if (seg->pub.is_in) {
        VTSS_MPLS_CHECK(VTSS_MPLS_IDX_IS_UNDEF(xc->in_seg_idx));
        xc->in_seg_idx     = seg_idx;
        seg->pub.xc_idx    = xc_idx;
    }
    else {
        VTSS_MPLS_CHECK(VTSS_MPLS_IDX_IS_UNDEF(xc->out_seg_idx));
        xc->out_seg_idx     = seg_idx;
        seg->pub.xc_idx     = xc_idx;
    }

    srvl_mpls_segment_hw_update(vtss_state, seg_idx);

    return VTSS_RC_OK;
}

static vtss_rc srvl_mpls_xc_segment_detach(vtss_state_t                  *vtss_state,
                                           const vtss_mpls_segment_idx_t seg_idx)
{
    vtss_mpls_xc_idx_t           xc_idx;
    vtss_mpls_xc_t               *xc;
    vtss_mpls_segment_internal_t *seg;

    VTSS_DG(VTSS_TRACE_GROUP_MPLS, "Detaching segment %d from XC", seg_idx);

    VTSS_MPLS_IDX_CHECK_SEGMENT(seg_idx);
    seg = &SEG_I(seg_idx);
    xc_idx = seg->pub.xc_idx;

    // Rules:
    //   - Segment must be attached to an XC (and XC to segment)
    //   - Segment can't be server to other segments  -- FIXME: Change?
    //   - Segment can't be attached to a server      -- FIXME: Change?

    VTSS_MPLS_IDX_CHECK_XC(xc_idx);
    VTSS_MPLS_CHECK(VTSS_MPLS_IDXCHAIN_END(seg->clients));
    VTSS_MPLS_CHECK(VTSS_MPLS_IDX_IS_UNDEF(seg->pub.server_idx));

    xc = &XC_P(xc_idx);

    VTSS_MPLS_CHECK((seg_idx == xc->in_seg_idx) || (seg_idx == xc->out_seg_idx));  // Consistency check

    if (seg->pub.is_in) {
        VTSS_MPLS_IDX_UNDEF(xc->in_seg_idx);
        VTSS_MPLS_IDX_UNDEF(seg->pub.xc_idx);
    }
    else {
        VTSS_MPLS_IDX_UNDEF(xc->out_seg_idx);
        VTSS_MPLS_IDX_UNDEF(seg->pub.xc_idx);
    }

    srvl_mpls_segment_hw_update(vtss_state, seg_idx);

    return VTSS_RC_OK;
}

static vtss_rc srvl_mpls_xc_mc_segment_attach(vtss_state_t                  *vtss_state,
                                              const vtss_mpls_xc_idx_t      xc_idx,
                                              const vtss_mpls_segment_idx_t seg_idx)
{
    vtss_mpls_xc_internal_t *xc;
    vtss_mpls_segment_t     *seg;

    VTSS_DG(VTSS_TRACE_GROUP_MPLS, "XC %d: Attaching segment %d to MC chain", xc_idx, seg_idx);

    VTSS_MPLS_IDX_CHECK_XC(xc_idx);
    VTSS_MPLS_IDX_CHECK_SEGMENT(seg_idx);

    xc  = &XC_I(xc_idx);
    seg = &SEG_P(seg_idx);

    // Rules:
    //   - Segment must be attached to an XC (and not this one, @xc_idx)
    //   - Must be out-segment
    //   - Must not already be in chain

    VTSS_MPLS_CHECK(VTSS_MPLS_IDX_IS_DEF(seg->xc_idx));
    VTSS_MPLS_CHECK(seg->xc_idx != xc_idx);
    VTSS_MPLS_CHECK(!seg->is_in);
    VTSS_MPLS_CHECK(!vtss_mpls_idxchain_find(vtss_state, &xc->mc_chain, seg_idx));

    // Add to chain

    VTSS_MPLS_CHECK_RC(vtss_mpls_idxchain_insert_at_head(vtss_state, &xc->mc_chain, seg_idx));

    return VTSS_RC_OK;
}

static vtss_rc srvl_mpls_xc_mc_segment_detach(vtss_state_t                  *vtss_state,
                                              const vtss_mpls_xc_idx_t      xc_idx,
                                              const vtss_mpls_segment_idx_t seg_idx)
{
    vtss_mpls_xc_internal_t *xc;
    vtss_mpls_segment_t     *seg;

    VTSS_DG(VTSS_TRACE_GROUP_MPLS, "XC %d: Detaching segment %d from MC chain", xc_idx, seg_idx);

    VTSS_MPLS_IDX_CHECK_XC(xc_idx);
    VTSS_MPLS_IDX_CHECK_SEGMENT(seg_idx);

    xc  = &XC_I(xc_idx);
    seg = &SEG_P(seg_idx);

    VTSS_MPLS_CHECK_RC(vtss_mpls_idxchain_remove_first(vtss_state, &xc->mc_chain, seg_idx));
    VTSS_MPLS_IDX_UNDEF(seg->xc_idx);

    return VTSS_RC_OK;
}

static vtss_rc srvl_mpls_xc_evc_attach_calc(vtss_evc_id_t evc_id, vtss_res_t *res)
{
    return VTSS_RC_OK;
}

static vtss_rc srvl_mpls_xc_evc_detach(vtss_evc_id_t evc_id)
{
    return VTSS_RC_OK;
}

static vtss_rc srvl_mpls_xc_evc_attach(vtss_evc_id_t evc_id)
{
    return VTSS_RC_OK;
}

void vtss_srvl_mpls_ece_is1_update(vtss_state_t     *vtss_state,
                                   vtss_evc_entry_t *evc,  vtss_ece_entry_t *ece,
                                   vtss_sdx_entry_t *isdx, vtss_is1_key_t   *key)
{
    u32 i;

    for (i = 0; i < VTSS_ARRSZ(evc->pw) && evc->pw[i].pw_num; i++) {
        if (VTSS_MPLS_IDX_IS_UNDEF(evc->pw[i].ingress_xc)) {
            continue;
        }

        const vtss_mpls_xc_internal_t *xc     = &XC_I(evc->pw[i].ingress_xc);
        const vtss_mpls_segment_idx_t seg_idx = xc->pub.in_seg_idx;

        if (VTSS_MPLS_IDX_IS_DEF(seg_idx)) {
            vtss_mpls_segment_state_t state;
            vtss_port_no_t            l2_port;

            (void) vtss_cmn_mpls_port_no_get(vtss_state, seg_idx, &l2_port);
            (void) srvl_mpls_segment_state_get(vtss_state, seg_idx, &state);

            if (state == VTSS_MPLS_SEGMENT_STATE_UP  &&  l2_port == isdx->port_no) {
                key->isdx.value[0] = xc->isdx->sdx >> 8;
                key->isdx.value[1] = xc->isdx->sdx & 0xff;
                key->isdx.mask[0]  = 0x01;            // Serval rev B: Only ISDXes < 512 are usable for MPLS
                key->isdx.mask[1]  = 0xff;
                VTSS_DG(VTSS_TRACE_GROUP_MPLS, "MPLS IS1 cfg for evc_id %u: XC %d, seg %d, state %s, PW ISDX %d -> SDX %d",
                        ece->evc_id, evc->pw[i].ingress_xc, seg_idx,
                        srvl_mpls_segment_state_to_str(state),
                        xc->isdx->sdx, isdx->sdx);
            }
        }
    }
}

void vtss_srvl_mpls_ece_es0_update(vtss_state_t     *vtss_state,
                                   vtss_evc_entry_t *evc,  vtss_ece_entry_t  *ece,
                                   vtss_sdx_entry_t *esdx, vtss_es0_action_t *action)
{
    u32 i;

    for (i = 0; i < VTSS_ARRSZ(evc->pw) && evc->pw[i].pw_num; i++) {
        if (VTSS_MPLS_IDX_IS_UNDEF(evc->pw[i].egress_xc)) {
            continue;
        }

        const vtss_mpls_segment_idx_t seg_idx = XC_P(evc->pw[i].egress_xc).out_seg_idx;

        if (VTSS_MPLS_IDX_IS_DEF(seg_idx)) {
            vtss_mpls_segment_state_t    state;
            vtss_mpls_segment_internal_t *seg = &SEG_I(seg_idx);
            vtss_port_no_t               l2_port;

            (void) vtss_cmn_mpls_port_no_get(vtss_state, seg_idx, &l2_port);
            (void) srvl_mpls_segment_state_get(vtss_state, seg_idx, &state);

            if (state == VTSS_MPLS_SEGMENT_STATE_UP  &&  l2_port == esdx->port_no) {
                action->mpls_encap_idx = seg->u.out.encap_idx;
                action->mpls_encap_len = srvl_bytes_to_encap_len(seg->u.out.encap_bytes);
                VTSS_DG(VTSS_TRACE_GROUP_MPLS, "MPLS encap cfg for evc_id %u: XC %d, seg %d, state %s, encap %d/%d",
                        ece->evc_id, evc->pw[i].egress_xc, seg_idx,
                        srvl_mpls_segment_state_to_str(state),
                        action->mpls_encap_idx, action->mpls_encap_len);
            }
        }
    }
}

vtss_rc vtss_srvl_mpls_evc_update(vtss_evc_id_t evc_id, vtss_res_t *res, vtss_res_cmd_t cmd)
{
    if (cmd == VTSS_RES_CMD_CALC) {
        VTSS_RC(srvl_mpls_xc_evc_attach_calc(evc_id, res));
    } else if (cmd == VTSS_RES_CMD_DEL) {
        VTSS_RC(srvl_mpls_xc_evc_detach(evc_id));
    } else if (cmd == VTSS_RES_CMD_ADD) {
        VTSS_RC(srvl_mpls_xc_evc_attach(evc_id));
    }
    return VTSS_RC_OK;
}

void vtss_srvl_mpls_hqos_change_notification(vtss_state_t         *vtss_state,
                                             const vtss_hqos_id_t hqos_id,
                                             const BOOL           active)
{
    u32 i;
    for (i = 0; i < VTSS_MPLS_XC_CNT; i++) {
        vtss_mpls_xc_internal_t       *xc     = &XC_I(i);
        const vtss_mpls_segment_idx_t seg_idx = xc->pub.in_seg_idx;

        if (VTSS_MPLS_IDX_IS_DEF(seg_idx) && xc->isdx != NULL) {     // XC has in-segment and has ISDX: Segment is candidate for also having HQoS
            vtss_mpls_segment_internal_t *seg = &SEG_I(seg_idx);
            if (seg->pub.hqos_id == hqos_id) {
                VTSS_DG(VTSS_TRACE_GROUP_MPLS, "HQoS ID %d changed to %s; updating in-segment %d", hqos_id, (active ? "Enabled" : "Disabled"), seg_idx);
                if (!active) {
                    seg->u.in.active_hqos_id = VTSS_HQOS_ID_NONE;
                }
                srvl_mpls_segment_hw_update(vtss_state, seg_idx);
            }
        }
    }
}

/* - XC counters --------------------------------------------------- */

static vtss_rc srvl_mpls_xc_counters_update(vtss_state_t              *vtss_state,
                                            vtss_mpls_xc_idx_t        xc_idx,
                                            vtss_mpls_xc_counters_t * const counters,
                                            BOOL                      clear)
{
    vtss_mpls_xc_internal_t       *xc;
    vtss_sdx_entry_t              *isdx, *esdx;
    vtss_evc_counters_t           evc_cnt;
    vtss_rc                       rc = VTSS_RC_ERROR;

    VTSS_DG(VTSS_TRACE_GROUP_MPLS, "XC %d: %s statistics", xc_idx, (clear ? "Clearing" : "Getting"));

    VTSS_MPLS_IDX_CHECK_XC(xc_idx);
    xc = &XC_I(xc_idx);

    // Rules:
    //   - Counters only supported for LSR operation
    //   - LSR XC must be operational. We use the presence of ISDX + ESDX for checking that

    VTSS_MPLS_CHECK(xc->pub.type == VTSS_MPLS_XC_TYPE_LSR);

    isdx = xc->isdx;
    esdx = VTSS_MPLS_IDX_IS_DEF(xc->pub.out_seg_idx) ? SEG_I(xc->pub.out_seg_idx).u.out.esdx : NULL;

    if (!isdx || !esdx) {
        return VTSS_RC_ERROR;
    }

    memset(&evc_cnt, 0, sizeof(evc_cnt));       // Yes, even when clearing: Quiets lint...
    rc = VTSS_FUNC(evc.sdx_counters_update, isdx, esdx, &evc_cnt, clear);

    if (counters) {
        counters->rx_green   = evc_cnt.rx_green;
        counters->rx_yellow  = evc_cnt.rx_yellow;
        counters->rx_red     = evc_cnt.rx_red;
        counters->rx_discard = evc_cnt.rx_discard;
        counters->tx_discard = evc_cnt.tx_discard;
        counters->tx_green   = evc_cnt.tx_green;
        counters->tx_yellow  = evc_cnt.tx_yellow;
    }

    return rc;
}

static vtss_rc srvl_mpls_xc_counters_get(vtss_state_t                    *vtss_state,
                                         vtss_mpls_xc_idx_t              xc_idx,
                                         vtss_mpls_xc_counters_t * const counters)
{
    return srvl_mpls_xc_counters_update(vtss_state, xc_idx, counters, FALSE);
}

static vtss_rc srvl_mpls_xc_counters_clear(vtss_state_t *vtss_state, vtss_mpls_xc_idx_t xc_idx)
{
    return srvl_mpls_xc_counters_update(vtss_state, xc_idx, NULL, TRUE);
}

/* - Debug print --------------------------------------------------- */

static vtss_rc srvl_debug_mpls(vtss_state_t              *vtss_state,
                               const vtss_debug_printf_t pr,
                               const vtss_debug_info_t   *const info)
{
    // vtss_debug_print_header(pr, "MPLS Core");
    VTSS_RC(vtss_srvl_debug_is0_all(vtss_state, pr, info));
    VTSS_RC(vtss_srvl_debug_es0_all(vtss_state, pr, info));
    return VTSS_RC_OK;
}

static vtss_rc srvl_debug_mpls_oam(vtss_state_t              *vtss_state,
                                   const vtss_debug_printf_t pr,
                                   const vtss_debug_info_t   *const info)
{
    vtss_debug_print_header(pr, "MPLS OAM");
    return VTSS_RC_OK;
}

vtss_rc vtss_srvl_mpls_debug_print(vtss_state_t              *vtss_state,
                                   const vtss_debug_printf_t pr,
                                   const vtss_debug_info_t   *const info)
{
    VTSS_RC(vtss_debug_print_group(VTSS_DEBUG_GROUP_MPLS, srvl_debug_mpls, vtss_state, pr, info));
    VTSS_RC(vtss_debug_print_group(VTSS_DEBUG_GROUP_MPLS_OAM, srvl_debug_mpls_oam, vtss_state, pr, info));
    return VTSS_RC_OK;
}

/* - Initialization ------------------------------------------------ */

static vtss_rc srvl_mpls_init(vtss_state_t *vtss_state)
{
    u8 map, qos, tc, i;

    // OAM extraction CPU queue
    SRVL_WRM(VTSS_ANA_MPLS_CL_OAM_CONF,
             VTSS_F_ANA_MPLS_CL_OAM_CONF_CPU_OAM_QUEUE(MPLS_CPU_Q),
             VTSS_M_ANA_MPLS_CL_OAM_CONF_CPU_OAM_QUEUE);

    // Error frame extraction state (all start out disabled) + CPU queue
    for (i = 0; i < 7; i++) {
        SRVL_WRM(VTSS_ANA_MPLS_CL_FRAME_EXTRACT(i),
                 VTSS_F_ANA_MPLS_CL_FRAME_EXTRACT_EXTRACT_QUEUE(MPLS_CPU_Q),
                 VTSS_M_ANA_MPLS_CL_FRAME_EXTRACT_EXTRACT_ENA | VTSS_M_ANA_MPLS_CL_FRAME_EXTRACT_EXTRACT_QUEUE);
    }

    // Enable all methods for MPLS frame extraction to CPU
    for (i = 0; i < 7; i++) {
        SRVL_WRM(VTSS_ANA_MPLS_CL_FRAME_EXTRACT(i),
                 VTSS_F_ANA_MPLS_CL_FRAME_EXTRACT_EXTRACT_ENA(0xffff),
                 VTSS_M_ANA_MPLS_CL_FRAME_EXTRACT_EXTRACT_ENA);
    }

    // Set VCCV2 label and make sure OAM TTL = 0/1 aren't treated as TTL expiry (...OAM_TTL_CHECK = 0)

    SRVL_WRM(VTSS_ANA_MPLS_CL_OAM_CONF,
             VTSS_F_ANA_MPLS_CL_OAM_CONF_VCCV2_LABEL(MPLS_RAL_LABEL_VALUE) | VTSS_F_ANA_MPLS_CL_OAM_CONF_VCCV2_ENA,
             VTSS_M_ANA_MPLS_CL_OAM_CONF_VCCV2_LABEL | VTSS_F_ANA_MPLS_CL_OAM_CONF_VCCV2_ENA | VTSS_F_ANA_MPLS_CL_OAM_CONF_OAM_TTL_CHECK);

    // Initialize VProfiles

    (void) srvl_mpls_vprofile_init(vtss_state);

    // TC mapping tables. All start out as identity mappings.

    for (map = 0; map < VTSS_MPLS_QOS_TO_TC_MAP_CNT; map++) {
        for (qos = 0; qos < VTSS_MPLS_QOS_TO_TC_ENTRY_CNT; qos++) {
            vtss_state->mpls.tc_conf.qos_to_tc_map[map][qos].dp0_tc = qos;
            vtss_state->mpls.tc_conf.qos_to_tc_map[map][qos].dp1_tc = qos;
        }
    }

    for (map = 0; map < VTSS_MPLS_TC_TO_QOS_MAP_CNT; map++) {
        for (tc = 0; tc < VTSS_MPLS_TC_TO_QOS_ENTRY_CNT; tc++) {
            vtss_state->mpls.tc_conf.tc_to_qos_map[map][tc].qos = tc;
            vtss_state->mpls.tc_conf.tc_to_qos_map[map][tc].dp  = FALSE;
        }
    }

    (void) srvl_mpls_tc_conf_set(vtss_state, &vtss_state->mpls.tc_conf);

    return VTSS_RC_OK;
}

vtss_rc vtss_srvl_mpls_init(vtss_state_t *vtss_state, vtss_init_cmd_t cmd)
{
    vtss_mpls_state_t *state = &vtss_state->mpls;

    switch (cmd) {
    case VTSS_INIT_CMD_CREATE:
        state->tc_conf_set            = srvl_mpls_tc_conf_set;
        state->l2_alloc               = srvl_mpls_l2_alloc;
        state->l2_free                = srvl_mpls_l2_free;
        state->l2_set                 = srvl_mpls_l2_set;
        state->l2_segment_attach      = srvl_mpls_l2_segment_attach;
        state->l2_segment_detach      = srvl_mpls_l2_segment_detach;
        state->xc_alloc               = srvl_mpls_xc_alloc;
        state->xc_free                = srvl_mpls_xc_free;
        state->xc_set                 = srvl_mpls_xc_set;
        state->xc_segment_attach      = srvl_mpls_xc_segment_attach;
        state->xc_segment_detach      = srvl_mpls_xc_segment_detach;
        state->xc_mc_segment_attach   = srvl_mpls_xc_mc_segment_attach;
        state->xc_mc_segment_detach   = srvl_mpls_xc_mc_segment_detach;
        state->segment_alloc          = srvl_mpls_segment_alloc;
        state->segment_free           = srvl_mpls_segment_free;
        state->segment_set            = srvl_mpls_segment_set;
        state->segment_status_get     = srvl_mpls_segment_status_get;
        state->segment_server_attach  = srvl_mpls_segment_server_attach;
        state->segment_server_detach  = srvl_mpls_segment_server_detach;
        state->xc_counters_get        = srvl_mpls_xc_counters_get;
        state->xc_counters_clear      = srvl_mpls_xc_counters_clear;
        break;
    case VTSS_INIT_CMD_INIT:
        VTSS_RC(srvl_mpls_init(vtss_state));
        break;
    default:
        break;
    }
    return VTSS_RC_OK;
}
#endif /* VTSS_ARCH_SERVAL && VTSS_FEATURE_MPLS */
