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

#define VTSS_TRACE_LAYER VTSS_TRACE_LAYER_CIL
#include "vtss_jaguar2_cil.h"

// Avoid "vtss_api.h not used in module vtss_jaguar2_fdma.c"
/*lint --e{766} */
#if defined(VTSS_OPT_VCORE_III) && VTSS_OPT_FDMA && defined(VTSS_ARCH_JAGUAR_2)
#include "../ail/vtss_fdma.h"
#include "../ail/vtss_fdma_common.h"

/* Use relative addresses for registers - this is a mess! */
// Avoid Lint Warning 572: Excessive shift value (precision 1 shifted right by 2), which occurs
// in this file because (t) - VTSS_IO_ORIGIN1_OFFSET == 0 for t = VTSS_TO_CFG (i.e. ICPU_CFG), and 0 >> 2 gives a lint warning.
#undef VTSS_IOADDR
#undef VTSS_IOREG
#undef VTSS_IO_OFFSET1
#undef VTSS_IO_OFFSET2
#define VTSS_IOADDR(t, o) /*lint -save -e572 */((((t) - VTSS_IO_ORIGIN1_OFFSET) >> 2) + (o)) /*lint -restore */
#define VTSS_IOREG(t,o)  (VTSS_IOADDR(t,o))
#define VTSS_IO_OFFSET1(offset) (VTSS_IO_ORIGIN1_OFFSET + offset)
#define VTSS_IO_OFFSET2(offset) (VTSS_IO_ORIGIN2_OFFSET + offset)

#define VTSS_FDMA_XTR_HDR_SIZE_BYTES 28 // Number of bytes in the extraction header (28 bytes IFH)
#define VTSS_FDMA_INJ_HDR_SIZE_BYTES 32 // Number of bytes in the injection header  (28 bytes IFH + 4 for a VLAN tag if @switch_frm == TRUE)

// We don't operate with assertions in the API, so we print an error and gracefully return.
// Note: This macro is sometimes used with code that MUST be executed - even on non-debug builds -,
// so if you don't like to have VTSS_EG() calls, then rewrite the macro to avoid that call.
#define FDMA_ASSERT(grp, expr, code) do {if (!(expr)) {VTSS_EG((VTSS_TRACE_GROUP_FDMA_ ## grp), "Assert failed: " # expr); code}} while (0);

#define FDMA_INLINE inline

// A field of hw_dcb->datal is free for S/W to use. This field is not part of the
// register list, and so we have to define it ourselves.
// For extraction we use it for the channel number.
#define VTSS_F_ICPU_CFG_FDMA_FDMA_DCB_DATAL_SW(x)  VTSS_ENCODE_BITFIELD(x, 24, 8)
#define VTSS_M_ICPU_CFG_FDMA_FDMA_DCB_DATAL_SW     VTSS_ENCODE_BITMASK(24, 8)
#define VTSS_X_ICPU_CFG_FDMA_FDMA_DCB_DATAL_SW(x)  VTSS_EXTRACT_BITFIELD(x, 24, 8)

/*****************************************************************************/
//
// REGISTER ACCESS
//
//****************************************************************************/
#define J2_WRM_SET(vstate, p, mask)         J2_WRM(vstate, p, mask, mask)
#define J2_WRM_CLR(vstate, p, mask)         J2_WRM(vstate, p, 0,    mask)
#define J2_WRM_CTL(vstate, p, _cond_, mask) J2_WRM(vstate, p, (_cond_) ? mask : 0, mask)

#define VTSS_FDMA_CH_INJ 2

/*****************************************************************************/
// Read target register using current CPU interface
/*****************************************************************************/
static FDMA_INLINE u32 J2_RD(vtss_state_t *const vstate, u32 reg)
{
    u32 value;
    FDMA_ASSERT(IRQ, vstate->init_conf.reg_read(0, reg, &value) == VTSS_RC_OK, return 0;);
    return value;
}

/*****************************************************************************/
// Write target register using current CPU interface
/*****************************************************************************/
static FDMA_INLINE void J2_WR(vtss_state_t *const vstate, u32 reg, u32 value)
{
    FDMA_ASSERT(IRQ, vstate->init_conf.reg_write(0, reg, value) == VTSS_RC_OK, ;);
}

/*****************************************************************************/
// Read-modify-write target register using current CPU interface
/*****************************************************************************/
static FDMA_INLINE void J2_WRM(vtss_state_t *const vstate, u32 reg, u32 value, u32 mask)
{
    u32 val = J2_RD(vstate, reg);
    val = ((val & ~mask) | (value & mask));
    J2_WR(vstate, reg, val);
}

/*****************************************************************************/
//
// PRIVATE FUNCTIONS
//
//****************************************************************************/

/*****************************************************************************/
// disable_chs()
/*****************************************************************************/
static FDMA_INLINE void disable_chs(vtss_state_t *const vstate, u32 ch_mask, BOOL graceful)
{
    if (graceful) {
        u32 val;
        J2_WR(vstate, VTSS_ICPU_CFG_FDMA_FDMA_CH_DISABLE, ch_mask);
        do {
            val = J2_RD(vstate, VTSS_ICPU_CFG_FDMA_FDMA_CH_DISABLE);
        } while (val != 0);
    } else {
        J2_WR(vstate, VTSS_ICPU_CFG_FDMA_FDMA_CH_FORCEDIS, ch_mask);
    }
}

/*****************************************************************************/
// enable_llp_frm_glbl_interrupts()
/*****************************************************************************/
static void enable_llp_frm_glbl_interrupts(vtss_state_t *const vstate, u32 ch_mask)
{
    // Clear and enable FRM interrupts
    J2_WR( vstate, VTSS_ICPU_CFG_FDMA_FDMA_INTR_FRM, ch_mask);
    J2_WRM(vstate, VTSS_ICPU_CFG_FDMA_FDMA_INTR_FRM_ENA, ch_mask, ch_mask);

    // Clear and enable LLP interrupts
    J2_WR( vstate, VTSS_ICPU_CFG_FDMA_FDMA_INTR_LLP, ch_mask);
    J2_WRM(vstate, VTSS_ICPU_CFG_FDMA_FDMA_INTR_LLP_ENA, ch_mask, ch_mask);

    // Enable global interrupts from DMA controller
    J2_WRM(vstate, VTSS_ICPU_CFG_FDMA_FDMA_INTR_ENA, ch_mask, ch_mask);
}

/*****************************************************************************/
// enable_ch()
/*****************************************************************************/
static void enable_ch(vtss_state_t *const vstate, u32 ch)
{
    J2_WRM(vstate, VTSS_ICPU_CFG_FDMA_FDMA_CH_ACTIVATE, VTSS_BIT(ch), VTSS_BIT(ch));
}

/*****************************************************************************/
// jr2_fdma_xtr_dcb_init()
/*****************************************************************************/
static vtss_rc jr2_fdma_xtr_dcb_init(vtss_state_t *const vstate, vtss_fdma_ch_t xtr_ch, fdma_sw_dcb_t *list, u32 cfg_alloc_len)
{
    fdma_hw_dcb_t *hw_dcb = FDMA_HW_DCB(list);

    // Initialize the DCB area
    hw_dcb->datap = CPU_TO_BUS(VTSS_OS_VIRT_TO_PHYS(list->ifh_ptr));
    hw_dcb->datal = CPU_TO_BUS((VTSS_F_ICPU_CFG_FDMA_FDMA_DCB_DATAL_SW(xtr_ch)) | VTSS_F_ICPU_CFG_FDMA_FDMA_DCB_DATAL_DATAL(cfg_alloc_len));
    hw_dcb->llp   = CPU_TO_BUS(FDMA_USER_DCB(list)->next ? VTSS_OS_VIRT_TO_PHYS(FDMA_HW_DCB(FDMA_SW_DCB(FDMA_USER_DCB(list)->next))) : 0);
    hw_dcb->stat  = 0;

    VTSS_OS_REORDER_BARRIER();

    // Flush the dcache, so that the DCB gets written to main memory.
    VTSS_OS_DCACHE_FLUSH(hw_dcb, VTSS_FDMA_DCB_SIZE_BYTES);

    return VTSS_RC_OK;
}

/*****************************************************************************/
// jr2_fdma_xtr_connect()
// This function must be called with interrupts disabled, so that no context
// switches can occur, since it manipulates list pointers that are used in
// IRQ (DSR) context.
/*****************************************************************************/
static void jr2_fdma_xtr_connect(vtss_state_t *const vstate, fdma_hw_dcb_t *tail, fdma_hw_dcb_t *more)
{
    tail->llp = CPU_TO_BUS(VTSS_OS_VIRT_TO_PHYS(more));
    VTSS_OS_REORDER_BARRIER();

    // Flush the dcache, so that the previous tail's DCB's LLP gets written to main memory.
    VTSS_OS_DCACHE_FLUSH(&tail->llp, 4);
}

/*****************************************************************************/
// jr2_fdma_xtr_restart_ch()
// We need to feed the channel with a new list of DCBs (if any).
// This function must be called with interrupts disabled.
/*****************************************************************************/
static BOOL jr2_fdma_xtr_restart_ch(vtss_state_t *const vstate, vtss_fdma_ch_t xtr_ch)
{
    fdma_ch_state_t *ch_state = &vstate->fdma_state.fdma_ch_state[xtr_ch];
    u32             val;

    if (J2_RD(vstate, VTSS_ICPU_CFG_FDMA_FDMA_DCB_LLP(xtr_ch)) != 0) {
        // The FDMA is not yet done.
        return FALSE;
    }

    // Pause the channel gracefully
    J2_WR(vstate, VTSS_ICPU_CFG_FDMA_FDMA_CH_DISABLE, VTSS_BIT(xtr_ch));
    do {
        val = J2_RD(vstate, VTSS_ICPU_CFG_FDMA_FDMA_CH_SAFE);
    } while ((val & VTSS_BIT(xtr_ch)) == 0);

    // Feed the free_head into the H/W.
    J2_WR(vstate, VTSS_ICPU_CFG_FDMA_FDMA_DCB_LLP(xtr_ch), VTSS_OS_VIRT_TO_PHYS(FDMA_HW_DCB(ch_state->free_head)));

    // Re-activate the channel.
    enable_ch(vstate, xtr_ch);

    return TRUE;
}

/*****************************************************************************/
// xtr_start_ch()
/*****************************************************************************/
static void xtr_start_ch(vtss_state_t *const vstate, vtss_fdma_ch_t xtr_ch)
{
    fdma_ch_state_t *ch_state = &vstate->fdma_state.fdma_ch_state[xtr_ch];

    // Configure the extraction group for FDMA-based extraction (default by API-setup is register-based if running on internal CPU, VRAP if running on external).
    J2_WRM(vstate, VTSS_DEVCPU_QS_XTR_XTR_GRP_CFG(ch_state->xtr_grp), VTSS_F_DEVCPU_QS_XTR_XTR_GRP_CFG_MODE(2), VTSS_M_DEVCPU_QS_XTR_XTR_GRP_CFG_MODE);

    // Hand the DCB list to the channel
    J2_WR(vstate, VTSS_ICPU_CFG_FDMA_FDMA_DCB_LLP(xtr_ch), VTSS_OS_VIRT_TO_PHYS(ch_state->cur_head));

    // Set the channel priority
    J2_WRM(vstate, VTSS_ICPU_CFG_FDMA_FDMA_CH_CFG(xtr_ch), VTSS_F_ICPU_CFG_FDMA_FDMA_CH_CFG_CH_PRIO(ch_state->prio), VTSS_M_ICPU_CFG_FDMA_FDMA_CH_CFG_CH_PRIO);

    // Clear and enable LLP, FRM and global interrupts.
    enable_llp_frm_glbl_interrupts(vstate, VTSS_BIT(xtr_ch));

    VTSS_OS_REORDER_BARRIER();

    // Enable the channel
    enable_ch(vstate, xtr_ch);
}

/*****************************************************************************/
// inj_start_ch()
/*****************************************************************************/
static void inj_start_ch(vtss_state_t *const vstate, vtss_fdma_ch_t inj_ch)
{
    fdma_ch_state_t *ch_state = &vstate->fdma_state.fdma_ch_state[inj_ch];

    // Configure the injection group for FDMA-based injection (default by API-setup is register-based if running on internal CPU, VRAP if running on external).
    J2_WRM(vstate, VTSS_DEVCPU_QS_INJ_INJ_GRP_CFG(ch_state->inj_grp), VTSS_F_DEVCPU_QS_INJ_INJ_GRP_CFG_MODE(2), VTSS_M_DEVCPU_QS_INJ_INJ_GRP_CFG_MODE);

    // When using FDMA, set GAP_SIZE to 0
    J2_WRM(vstate, VTSS_DEVCPU_QS_INJ_INJ_CTRL(ch_state->inj_grp), VTSS_F_DEVCPU_QS_INJ_INJ_CTRL_GAP_SIZE(0), VTSS_M_DEVCPU_QS_INJ_INJ_CTRL_GAP_SIZE);

    // Set the channel priority
    J2_WRM(vstate, VTSS_ICPU_CFG_FDMA_FDMA_CH_CFG(inj_ch), VTSS_F_ICPU_CFG_FDMA_FDMA_CH_CFG_CH_PRIO(ch_state->prio), VTSS_M_ICPU_CFG_FDMA_FDMA_CH_CFG_CH_PRIO);

    // Clear and enable LLP, FRM and global interrupts.
    enable_llp_frm_glbl_interrupts(vstate, VTSS_BIT(inj_ch));
}

/*****************************************************************************/
// jr2_fdma_start_ch()
/*****************************************************************************/
static void jr2_fdma_start_ch(vtss_state_t *const vstate, const vtss_fdma_ch_t ch)
{
    if (vstate->fdma_state.fdma_ch_state[ch].usage == VTSS_FDMA_CH_USAGE_XTR) {
        xtr_start_ch(vstate, ch);
    } else {
        inj_start_ch(vstate, ch);
    }
}

/*****************************************************************************/
// disable_and_clear_chs()
/*****************************************************************************/
static void disable_and_clear_chs(vtss_state_t *const vstate, u32 ch_mask, BOOL graceful)
{
    // Disable all channels
    disable_chs(vstate, ch_mask, graceful);

    // Disable global interrupts from DMA controller
    J2_WRM(vstate, VTSS_ICPU_CFG_FDMA_FDMA_INTR_ENA, ~ch_mask, ch_mask);

    // Disable and clear SIG interrupts
    J2_WRM(vstate, VTSS_ICPU_CFG_FDMA_FDMA_INTR_SIG_ENA, ~ch_mask, ch_mask);
    J2_WR( vstate, VTSS_ICPU_CFG_FDMA_FDMA_INTR_SIG, ch_mask);

    // Disable and clear FRM interrupts
    J2_WRM(vstate, VTSS_ICPU_CFG_FDMA_FDMA_INTR_FRM_ENA, ~ch_mask, ch_mask);
    J2_WR( vstate, VTSS_ICPU_CFG_FDMA_FDMA_INTR_FRM, ch_mask);

    // Disable and clear LLP interrupts
    J2_WRM(vstate, VTSS_ICPU_CFG_FDMA_FDMA_INTR_LLP_ENA, ~ch_mask, ch_mask);
    J2_WR( vstate, VTSS_ICPU_CFG_FDMA_FDMA_INTR_LLP, ch_mask);
}

/*****************************************************************************/
// inj_ifh_init()
/*****************************************************************************/
static FDMA_INLINE vtss_rc inj_ifh_init(vtss_state_t *const vstate, fdma_sw_dcb_t *sw_dcb, vtss_packet_tx_info_t *const tx_info, u32 *frm_size)
{
    vtss_fdma_list_t *user_dcb = FDMA_USER_DCB(sw_dcb), *user_dcb_iter;
    u32              bytes_stripped_on_egress = 0, actual_length_bytes = 0;
    u32              bin_hdr_len = 28;
    BOOL             insert_tpid_tag, insert_masq_tag, insert_clas_tag;

    // Frame pointer must be equal to the IFH pointer by now.
    FDMA_ASSERT(NORMAL, sw_dcb->ifh_ptr == user_dcb->frm_ptr, return VTSS_RC_ERROR;);

    insert_tpid_tag = (!tx_info->switch_frm && tx_info->tag.tpid != 0);
    insert_masq_tag = ( tx_info->switch_frm && tx_info->masquerade_port != VTSS_PORT_NO_NONE && tx_info->tag.vid != 0);
    insert_clas_tag = ( tx_info->switch_frm && tx_info->masquerade_port == VTSS_PORT_NO_NONE);

    if (insert_tpid_tag || insert_masq_tag || insert_clas_tag) {
        vtss_vlan_tag_t tag;
        if (insert_masq_tag || insert_clas_tag) {
            // We gotta insert a VLAN tag into the frame when switching the frame.
            // The alternative to this is to use the VStaX header to hold the tag
            // and set FWD.VSTAX_AVAIL, but this will not work in a stacking environment
            // (switched frames will not go across the stack ports then, and TTL cannot
            // be set-up, since we don't know which way the frame goes).
            tag.tpid = 0x8100;
            tag.pcp  = tx_info->cos == 8 ? 7 : tx_info->cos; // Even though there is a PCP-to-CoS conversion.
            tag.dei  = 0;
            tag.vid  = tx_info->tag.vid;
        }

        // Use tag from tx_info when asked to insert a tag (tpid != 0).
        VTSS_RC(vtss_fdma_cmn_insert_vlan_tag(sw_dcb, insert_masq_tag || insert_clas_tag ? &tag : &tx_info->tag, FALSE /* No need to move IFH data because we haven't yet constructed it */, FALSE /* Called with interrupts enabled */));
        bytes_stripped_on_egress += insert_clas_tag ? 4 : 0; // TPID and tags on masquerade ports are not stripped on egress. Only tags that get a switched frame classified to the correct VLAN are.
    }

    // We got to adjust the length of the EOF DCB to accommodate for a
    // minimum sized Ethernet frame.
    user_dcb_iter = user_dcb;
    while (user_dcb_iter) {
        actual_length_bytes += user_dcb_iter->act_len;

        if (user_dcb_iter->next == NULL) {
            // End-of-frame DCB. All lengths are excluding IFH/CMD and
            // excluding FCS, hence the 60 instead of 64.
            // We cannot pad the actual frame data with zeros because we
            // don't know how much data was allocated by the application.
            u32 bytes_on_wire = actual_length_bytes - bytes_stripped_on_egress;
            if (bytes_on_wire < 60) {
                u32 pad_bytes = 60 - bytes_on_wire;
                user_dcb_iter->act_len += pad_bytes;
                actual_length_bytes    += pad_bytes;
            }
        }

        user_dcb_iter = user_dcb_iter->next;
    }

    *frm_size = actual_length_bytes + 4; /* Exclude IFH, but include FCS */

    // And finally time for the IFH.
    // Adjust the IFH pointer.
    sw_dcb->ifh_ptr -= bin_hdr_len;

    VTSS_RC(vstate->packet.tx_hdr_encode(vstate, tx_info, sw_dcb->ifh_ptr, &bin_hdr_len));

    // Check that we haven't inserted too much data.
    FDMA_ASSERT(NORMAL, sw_dcb->ifh_ptr >= sw_dcb->data, return VTSS_RC_ERROR;);

    VTSS_IG(VTSS_TRACE_GROUP_FDMA_NORMAL, "IFH (%u bytes) + a bit frame data (%u bytes incl. FCS):", FDMA_USER_DCB(sw_dcb)->frm_ptr - sw_dcb->ifh_ptr, *frm_size);
    VTSS_IG_HEX(VTSS_TRACE_GROUP_FDMA_NORMAL, sw_dcb->ifh_ptr, 96);

    return VTSS_RC_OK;
}

/*****************************************************************************/
// inj_dcb_init()
/*****************************************************************************/
static vtss_rc inj_dcb_init(vtss_state_t *const vstate, fdma_sw_dcb_t *sw_dcb, fdma_sw_dcb_t **tail, u32 *dcb_cnt, BOOL need_not_be_cache_line_aligned)
{
    u32               *phys_data_ptr, phys_data_off;
    fdma_hw_dcb_t     *hw_dcb;
    int               frag_sz_bytes;
    int               sof = 1, eof;
    vtss_fdma_list_t *user_dcb;

    *dcb_cnt = 0;
    *tail    = sw_dcb;

    while (sw_dcb) {
        user_dcb = FDMA_USER_DCB(sw_dcb);
        eof      = user_dcb->next == NULL;
        // user_dcb->act_len must hold the size of the actual frame data excluding IFH and FCS.
        frag_sz_bytes = user_dcb->act_len + (user_dcb->frm_ptr - sw_dcb->ifh_ptr) + (eof ? 4 /* FCS */ : 0);

        FDMA_ASSERT(NORMAL, sw_dcb->ifh_ptr, return VTSS_RC_ERROR;);
        FDMA_ASSERT(NORMAL,
                    ((frag_sz_bytes >= VTSS_FDMA_MIN_DATA_PER_INJ_SOF_DCB_BYTES && sof == 1)  ||
                     (frag_sz_bytes >= VTSS_FDMA_MIN_DATA_PER_NON_SOF_DCB_BYTES && sof == 0)) &&
                    frag_sz_bytes <= VTSS_FDMA_MAX_DATA_PER_DCB_BYTES, return VTSS_RC_ERROR;);

        phys_data_ptr = (u32 *)VTSS_OS_VIRT_TO_PHYS(sw_dcb->ifh_ptr);
        phys_data_off = (u32)phys_data_ptr & 0x3;
        hw_dcb = FDMA_HW_DCB(sw_dcb);

        // The H/W DCB should be cache-line-size aligned
        FDMA_ASSERT(NORMAL, need_not_be_cache_line_aligned || ((u32)hw_dcb & (VTSS_OS_DCACHE_LINE_SIZE_BYTES - 1)) == 0, return VTSS_RC_ERROR;);

        hw_dcb->llp   = CPU_TO_BUS(user_dcb->next ? VTSS_OS_VIRT_TO_PHYS(FDMA_HW_DCB(FDMA_SW_DCB(user_dcb->next))) : 0); // NULL
        hw_dcb->datap = CPU_TO_BUS((u32)phys_data_ptr & ~0x3);
        hw_dcb->datal = CPU_TO_BUS(VTSS_F_ICPU_CFG_FDMA_FDMA_DCB_DATAL_DATAL(0xFFFF)); // RBNTBD: Can this be zero?
        hw_dcb->stat  = CPU_TO_BUS(VTSS_F_ICPU_CFG_FDMA_FDMA_DCB_STAT_BLOCKO(phys_data_off) |
                                   VTSS_F_ICPU_CFG_FDMA_FDMA_DCB_STAT_SOF(sof)              |
                                   VTSS_F_ICPU_CFG_FDMA_FDMA_DCB_STAT_EOF(eof)              |
                                   VTSS_F_ICPU_CFG_FDMA_FDMA_DCB_STAT_BLOCKL(frag_sz_bytes));

        VTSS_OS_REORDER_BARRIER();

        // Flush the dcache, so that the DCB gets written to main memory.
        VTSS_OS_DCACHE_FLUSH(hw_dcb, VTSS_FDMA_DCB_SIZE_BYTES);

        VTSS_OS_REORDER_BARRIER();

        // Flush the dcache, so that the frame data gets written to main memory.
        VTSS_OS_DCACHE_FLUSH(sw_dcb->ifh_ptr, frag_sz_bytes);

        (*dcb_cnt)++;

        // Prepare for next iteration
        sof = 0;
        if (user_dcb->next) {
            *tail = FDMA_SW_DCB(user_dcb->next);
        }
        sw_dcb = FDMA_SW_DCB(user_dcb->next);
    }
    return VTSS_RC_OK;
}

/*****************************************************************************/
// inj_ch_init()
/*****************************************************************************/
static FDMA_INLINE void inj_ch_init(vtss_state_t *const vstate, fdma_hw_dcb_t *hw_dcb, vtss_fdma_ch_t inj_ch)
{
    // Hand the DCB list to the channel
    J2_WR(vstate, VTSS_ICPU_CFG_FDMA_FDMA_DCB_LLP(inj_ch), VTSS_OS_VIRT_TO_PHYS(hw_dcb));
}

/*****************************************************************************/
// stat_rx_update()
/*****************************************************************************/
static FDMA_INLINE void stat_rx_update(fdma_state_t *state, u32 xtr_qu, BOOL drop)
{
    if (xtr_qu >= VTSS_PACKET_RX_QUEUE_CNT) {
        // What to do if the chip reports an invalid queue?
        xtr_qu = VTSS_PACKET_RX_QUEUE_CNT - 1;
    }

    if (drop) {
        state->fdma_stats.xtr_qu_drops[xtr_qu][0]++;
    } else {
        state->fdma_stats.xtr_qu_packets[xtr_qu][0]++;
    }
}

/*****************************************************************************/
// xtr_qu_suspend_set()
/*****************************************************************************/
static void jr2_fdma_xtr_qu_suspend_set(vtss_state_t *const vstate, vtss_packet_rx_queue_t xtr_qu, BOOL do_suspend)
{
    // On Jaguar2 there are two CPU ports (physical #53 and #54). When running only on
    // the internal CPU, we only use #53. However, some applications may
    // want to divide the extraction queues among them and forward some queues to
    // #53 and others to #54, which can then be read by the external CPU.
    // If enabling throttling, the internal CPU will suspend a given queue by
    // moving that queue from port #53 to port #54, which might have some
    // unexpected side-effects on an external CPU reading port #54.

    // First make sure that port #54 is flushing all data it receives.
    J2_WRM_SET(vstate, VTSS_DEVCPU_QS_XTR_XTR_FLUSH, VTSS_F_DEVCPU_QS_XTR_XTR_FLUSH_FLUSH(VTSS_BIT(1)));

    // Then either redirect the queue to port #53 (ourselves) or port #54 (suspend)
    // We need to call a function that is common to NPI configuration in order to get it updated correctly.
    (void)vstate->packet.fdma_qu_redirect_set(vstate, xtr_qu, do_suspend ? VTSS_CHIP_PORT_CPU_1 : VTSS_CHIP_PORT_CPU_0);
}

/*****************************************************************************/
// drop_check()
/*****************************************************************************/
static FDMA_INLINE BOOL drop_check(vtss_state_t *const vstate, u8 *ifh)
{
    // We discard all frames that are transmitted by the CPU itself.
    // In reality, we should check whether the frame was sFlow
    // marked and only drop those that were, but the sflow_marking
    // bit of the IFH will not be set in case the CPU originally
    // transmitted the frame directly to a port (rather than onto
    // a VLAN, a.k.a. switched).
    // If we didn't do the drop-check here, the call to
    // vtss_fdma_cmn_xtr_hdr_decode() would throw a trace error, because
    // one of its called functions would discover that the source port
    // for the frame is not a port that is in the port map.
    vtss_phys_port_no_t phys_port = ((ifh[23] & 0x1) << 5) | ((ifh[24] & 0xF8) >> 3);

    if (phys_port == VTSS_CHIP_PORT_CPU_0 || phys_port == VTSS_CHIP_PORT_CPU_1) {
        return TRUE; // Drop it
    }

    return FALSE; // Let it through to the application
}

/*****************************************************************************/
// jr2_fdma_inj_restart_ch()
/*****************************************************************************/
static vtss_rc jr2_fdma_inj_restart_ch(vtss_state_t *const vstate, vtss_fdma_ch_t inj_ch, fdma_sw_dcb_t *head, BOOL do_start, BOOL thread_safe)
{
    if (do_start) {
        // Channel must be inactive
        FDMA_CHECK_RC(thread_safe, (J2_RD(vstate, VTSS_ICPU_CFG_FDMA_FDMA_CH_SAFE) & VTSS_BIT(inj_ch)) != 0);

        inj_ch_init(vstate, FDMA_HW_DCB(head), inj_ch);
        enable_ch(vstate, inj_ch);
    }

    return VTSS_RC_OK;
}

/*****************************************************************************/
// xtr_qu_from_mask()
// The IFH contains an extraction queue mask, where each bit in the mask
// implies that the frame was subject to CPU forwarding on the specific port.
// The most significant of these bits indicate the extraction queue that the
// frame was actually sent to.
// This function finds this queue number (0-based) from the mask.
/*****************************************************************************/
static FDMA_INLINE u8 xtr_qu_from_mask(u32 xtr_qu_mask)
{
    int qu; // Must be an integer to be able to terminate this loop if called with 0.
    for (qu = VTSS_PACKET_RX_QUEUE_CNT - 1; qu >= 0; qu--) {
        if (xtr_qu_mask & VTSS_BIT(qu)) {
            break;
        }
    }

    FDMA_ASSERT(IRQ, qu >= 0 && qu < VTSS_PACKET_RX_QUEUE_CNT, return 0;);
    return (u8)qu; // The < 0 case is handled in the FDMA_ASSERT() above.
}

/*****************************************************************************/
// xtr_irq_frm_done()
/*****************************************************************************/
static FDMA_INLINE void xtr_irq_frm_done(vtss_state_t *const vstate, void *const cntxt, vtss_fdma_ch_t xtr_ch)
{
    int                    total_frm_size_bytes_incl_ifh, frag_size_bytes;
    fdma_ch_state_t        *ch_state;
    fdma_hw_dcb_t          *hw_dcb;
    fdma_state_t           *state = &vstate->fdma_state;
    vtss_fdma_list_t       *new_list, *new_head, *temp_list;
    u32                    stat, xtr_qu, xtr_qu_mask;
    u32                    offset_into_data = VTSS_FDMA_HDR_SIZE_BYTES - VTSS_FDMA_XTR_HDR_SIZE_BYTES;
    BOOL                   sof, eof;
    VTSS_OS_TIMESTAMP_TYPE sw_tstamp;

    VTSS_NG(VTSS_TRACE_GROUP_FDMA_IRQ, "xtr_irq_frm_done(ch = %d)\n", xtr_ch);
    ch_state = &state->fdma_ch_state[xtr_ch];
    FDMA_ASSERT(IRQ, ch_state->status != FDMA_CH_STATUS_DISABLED, return;);

    // Iterate over all DCBs and stop when we reach one with the DONE bit cleared.
    while (ch_state->cur_head) {
        hw_dcb = FDMA_HW_DCB(ch_state->cur_head);

        // Before dereferencing the DCB, we must invalidate the cache line(s).
        VTSS_OS_DCACHE_INVALIDATE(hw_dcb, VTSS_FDMA_DCB_SIZE_BYTES);

        stat = BUS_TO_CPU(hw_dcb->stat);

        // If stat.blockl is 0, we've consumed all the frames, and we're done.
        total_frm_size_bytes_incl_ifh = frag_size_bytes = VTSS_X_ICPU_CFG_FDMA_FDMA_DCB_STAT_BLOCKL(stat);
        if (frag_size_bytes == 0) {
            break;
        }

        state->fdma_stats.dcbs_used[xtr_ch]++;

        VTSS_OS_DCACHE_INVALIDATE(ch_state->cur_head->data + offset_into_data, frag_size_bytes);
        VTSS_OS_REORDER_BARRIER();

        // Currently we cannot handle aborted or pruned frames.
        FDMA_ASSERT(IRQ, (stat & (VTSS_M_ICPU_CFG_FDMA_FDMA_DCB_STAT_PD | VTSS_M_ICPU_CFG_FDMA_FDMA_DCB_STAT_ABORT)) == 0, VTSS_EG(VTSS_TRACE_GROUP_FDMA_IRQ, "ch=%d, stat=0x%08x", xtr_ch, stat); return;);

        FDMA_USER_DCB(ch_state->cur_head)->act_len = frag_size_bytes;

        sof = (stat & VTSS_M_ICPU_CFG_FDMA_FDMA_DCB_STAT_SOF) != 0;
        eof = (stat & VTSS_M_ICPU_CFG_FDMA_FDMA_DCB_STAT_EOF) != 0;

        if (sof) {
            ch_state->cur_head->ifh_ptr = ch_state->cur_head->data + offset_into_data;
            FDMA_USER_DCB(ch_state->cur_head)->frm_ptr = ch_state->cur_head->ifh_ptr + VTSS_FDMA_XTR_HDR_SIZE_BYTES;
        } else {
            FDMA_USER_DCB(ch_state->cur_head)->frm_ptr = ch_state->cur_head->ifh_ptr = ch_state->cur_head->data + offset_into_data;
        }

        // If we're receiving a jumbo frame (or a frame that is larger
        // than what we've reserved of space in the data area), we may
        // not have received it all. If the EOF flag in the DCB is not
        // set, this is the case, and we need to receive the remainder of
        // the frame before passing it on.
        if (eof) {
            BOOL drop;

            // EOF reached. Pass the frame on to the callback function.

            // Pass the frame on to the application-level handler, but timestamp it first.
            VTSS_OS_TIMESTAMP(&sw_tstamp);

            // Terminate the current list item before passing it on, but save a reference to the new head.
            new_head = FDMA_USER_DCB(ch_state->cur_head)->next;
            FDMA_USER_DCB(ch_state->cur_head)->next = NULL;

            if (!ch_state->pend_head) {
                // The frame is not part of a jumbo frame, so we expect the SOF flag to be set in the DCB.
                FDMA_ASSERT(IRQ, sof, return;);

                // For further, unified processing, we move the current DCB to the pending list.
                ch_state->pend_head = ch_state->cur_head;
            } else {
                // The frame is part of a jumbo frame. In that case, the SOF flag can't be set, because the SOF is
                // already received and moved to the pending list.
                FDMA_ASSERT(IRQ, !sof, return;);

                // Attach the current list item to the end of the pending list while computing the total length.
                temp_list = FDMA_USER_DCB(ch_state->pend_head);
                total_frm_size_bytes_incl_ifh += temp_list->act_len;
                while (temp_list->next) {
                    temp_list = temp_list->next;
                    total_frm_size_bytes_incl_ifh += temp_list->act_len;
                }
                temp_list->next = FDMA_USER_DCB(ch_state->cur_head);
            }

            state->fdma_stats.packets[xtr_ch]++;
            // Don't include IFH in packet byte counter
            state->fdma_stats.bytes[xtr_ch] += (total_frm_size_bytes_incl_ifh - VTSS_FDMA_XTR_HDR_SIZE_BYTES);

            // In some occassions, frames should be dropped right here.
            drop = drop_check(vstate, &ch_state->pend_head->data[offset_into_data]);

            // Find the physical queue that this frame was forwarded onto. This is the
            // most significant bit of the IFH.CPU_MASK.
            xtr_qu_mask = ((ch_state->pend_head->data[25 + offset_into_data] << 6) | (ch_state->pend_head->data[26 + offset_into_data] >> 2)) & 0xFF;

            // Let the frame pass through unless sFlow is enabled. This will attract the attention
            // due to trace errors if other chip bugs are found.
            if (xtr_qu_mask == 0 && vstate->packet.rx_conf.map.sflow_queue != VTSS_PACKET_RX_QUEUE_NONE) {
                // Bugzilla#17780. Due to a chip-bug, sFlow frames will not give rise to any bits set in the bit mask.
                // We can (in most cases) detect whether it's an sFlow frame and set the queue here.
                BOOL sflow_marked = (ch_state->pend_head->data[23 + offset_into_data] & 0x20) != 0;

                // If it's sflow_marked, we always set it to the sFlow queue.
                // We also need to mark it as an sFlow sample if the frame is originally
                // transmitted by the CPU and got returned to the CPU due to sFlow sampling.
                // This is not as easy as it sounds, because the sflow_marking bit is not always
                // set for such frames (in particular if the CPU transmitted to a particular port
                // or a set of ports, rather than transmitting to a VLAN, the sflow_marking is 0.
                // This last sentence is indicated with #drop == TRUE.
                if (sflow_marked || drop) {
                    xtr_qu_mask = VTSS_BIT(vstate->packet.rx_conf.map.sflow_queue);
                }
            }

            // xtr_qu_from_mask() will generate a tace error if xtr_qu_mask is still 0, which is what we want to attract attention.
            xtr_qu = xtr_qu_from_mask(xtr_qu_mask);

            vtss_fdma_cmn_throttle_suspend_check(vstate, xtr_qu, total_frm_size_bytes_incl_ifh - VTSS_FDMA_XTR_HDR_SIZE_BYTES);

            stat_rx_update(state, xtr_qu, drop);
            if (drop || vtss_fdma_cmn_xtr_hdr_decode(vstate, ch_state->pend_head, 0, xtr_qu, &sw_tstamp) != VTSS_RC_OK) {
                // Re-cycle
                new_list = FDMA_USER_DCB(ch_state->pend_head);
            } else {
                new_list = state->fdma_cfg.rx_cb(cntxt, FDMA_USER_DCB(ch_state->pend_head));
            }

            // NULLify the pending list, since there're no more pending fragments.
            ch_state->pend_head = NULL;

            // Return the DCBs (if any) to the extraction channel.
            (void)vtss_fdma_cmn_dcb_release(vstate, xtr_ch, FDMA_SW_DCB(new_list), TRUE /* thread safe */, TRUE /* connect */, FALSE /* don't restart channel */);
        } else {
            // The EOF is not reached yet, so there wasn't room for the whole frame
            // in the allocated data area of this DCB. Move the DCB to the pending list.

            // Save a reference to the new head.
            new_head = FDMA_USER_DCB(ch_state->cur_head)->next;

            // Clear the next entry, so that we can accommodate the case where the
            // FDMA's DCB list exhausts in the middle of a jumbo frame, causing
            // vtss_fdma_cmn_xtr_restart_ch() to restart the channel with a new list of DCBs.
            // If we didn't always clear the pend_head->next we wouldn't know where
            // to attach the remainder of jumbo frames.
            FDMA_USER_DCB(ch_state->cur_head)->next = NULL;

            if (!ch_state->pend_head) {
                // This is the first part of the frame. The SOF flag must be set. Continue execution if it isn't.
                FDMA_ASSERT(IRQ, sof, VTSS_EG_HEX(VTSS_TRACE_GROUP_FDMA_IRQ, (u8 *)ch_state, sizeof(*ch_state)); VTSS_EG_HEX(VTSS_TRACE_GROUP_FDMA_IRQ, (u8 *)hw_dcb, sizeof(*hw_dcb)));
                ch_state->pend_head = ch_state->cur_head;
            } else {
                // This is not the first and not the last part of the frame. The SOF flag must be cleared. Continue execution if it isn't.
                FDMA_ASSERT(IRQ, !sof, VTSS_EG_HEX(VTSS_TRACE_GROUP_FDMA_IRQ, (u8 *)ch_state, sizeof(*ch_state)); VTSS_EG_HEX(VTSS_TRACE_GROUP_FDMA_IRQ, (u8 *)hw_dcb, sizeof(*hw_dcb)));

                // We need to attach it to the last item in the pend_head list.
                temp_list = FDMA_USER_DCB(ch_state->pend_head);
                while (temp_list->next) {
                    temp_list = temp_list->next;
                }
                temp_list->next = FDMA_USER_DCB(ch_state->cur_head);
            }
        }

        // Advance the head of the "in progress" list
        ch_state->cur_head = FDMA_SW_DCB(new_head);
    }
}

/*****************************************************************************/
// inj_irq_tfr_done()
/*****************************************************************************/
static FDMA_INLINE void inj_irq_tfr_done(vtss_state_t *vstate, void *const cntxt, vtss_fdma_ch_t inj_ch)
{
    fdma_state_t     *state = &vstate->fdma_state;
    fdma_ch_state_t  *ch_state = &state->fdma_ch_state[inj_ch];
    vtss_fdma_list_t *sof_l, *eof_l;
    fdma_hw_dcb_t    *hw_dcb;
    fdma_sw_dcb_t    *sw_dcb;

    VTSS_NG(VTSS_TRACE_GROUP_FDMA_IRQ, "inj_irq_tfr_done(ch = %d)\n", inj_ch);

    FDMA_ASSERT(IRQ, inj_ch >= 0 && inj_ch < VTSS_FDMA_CH_CNT,    return;);
    FDMA_ASSERT(IRQ, ch_state->status != FDMA_CH_STATUS_DISABLED, return;);
    FDMA_ASSERT(IRQ, ch_state->usage  == VTSS_FDMA_CH_USAGE_INJ,  return;);
    FDMA_ASSERT(IRQ, ch_state->cur_head,                          return;);
    sof_l = eof_l = FDMA_USER_DCB(ch_state->cur_head);

    // We need to traverse the DCBs until we find a DCB with the dcb_ptr->llp
    // location being NULL, which is the latest injected frame's EOF. We get called
    // once per frame that has been injected, so it cannot happen that the DMA
    // controller goes beyond that point in the list.
    while (1) {
        FDMA_ASSERT(IRQ, eof_l, return;);

        // Check that the DMA controller indeed has injected the current block.
        // This is conveyed through the dcb_ptr->stat.pruned/done bit.
        // Since the DMA controller writes this bit, we must invalidate the cache
        // for that position before reading it. It is the only part of the DCB
        // that the DMA controller writes back, so it's the only part that we need
        // to invalidate.
        hw_dcb = FDMA_HW_DCB(FDMA_SW_DCB(eof_l));

#if VTSS_OPT_FDMA_DEBUG
        VTSS_OS_DCACHE_INVALIDATE(&hw_dcb->stat, 4);
        VTSS_OS_REORDER_BARRIER();
        FDMA_ASSERT(IRQ, BUS_TO_CPU(hw_dcb->stat) & VTSS_M_ICPU_CFG_FDMA_FDMA_DCB_STAT_PD, return;);
#endif /* VTSS_OPT_FDMA_DEBUG */

        if (hw_dcb->llp) {
            eof_l = eof_l->next;
        } else {
            break;
        }
    }

    VTSS_OS_TIMESTAMP(&sof_l->sw_tstamp);

    // eof_l points to the EOF list item.
    // Terminate it before passing it on to the callback function,
    // but keep a pointer to the next in the list, which is the one
    // we need to reactivate the channel for if non-NULL.
    ch_state->cur_head = FDMA_SW_DCB(eof_l->next);
    eof_l->next = NULL;

    if (ch_state->cur_head) {
        // More frames to go. Re-start the channel.
        VTSS_DG(VTSS_TRACE_GROUP_FDMA_IRQ, "inj_irq_tfr_done(ch = %d). Re-starting channel\n", inj_ch);

        inj_ch_init(vstate, FDMA_HW_DCB(ch_state->cur_head), inj_ch);
        enable_ch(vstate, inj_ch);
    } else {
        // No more frames. Also reset the tail.
        ch_state->cur_tail = NULL;
    }

    state->fdma_stats.packets[inj_ch]++;

    sw_dcb = FDMA_SW_DCB(sof_l);

    vstate->fdma_state.fdma_cfg.tx_done_cb(cntxt, sof_l, VTSS_RC_OK);

    // Release the injection DCBs.
    (void)vtss_fdma_cmn_dcb_release(vstate, 0 /* doesn't matter in FDMA v3+ */, sw_dcb, TRUE /* thread safe */, FALSE /* only used for XTR DCBs */, FALSE /* only used for XTR DCBs */);
}

/*****************************************************************************/
// ch_state_defaults()
/*****************************************************************************/
static FDMA_INLINE void ch_state_defaults(vtss_state_t *const vstate)
{
    fdma_ch_state_t *ch_state;

    // The FDMA API makes its own channel setup in later FDMA driver versions.
    ch_state = &vstate->fdma_state.fdma_ch_state[0];
    ch_state->usage = VTSS_FDMA_CH_USAGE_XTR;
    ch_state->prio  = 3;

    ch_state = &vstate->fdma_state.fdma_ch_state[VTSS_FDMA_CH_INJ];
    ch_state->usage   = VTSS_FDMA_CH_USAGE_INJ;
    ch_state->prio    = 3;
    ch_state->inj_grp = VTSS_FDMA_CH_INJ - 2;
}

/*****************************************************************************/
//
// PUBLIC FUNCTIONS
//
//****************************************************************************/

/*****************************************************************************/
// jr2_fdma_init_conf_set()
/*****************************************************************************/
static vtss_rc jr2_fdma_init_conf_set(vtss_state_t *const vstate)
{
    fdma_state_t *state = &vstate->fdma_state;

    disable_and_clear_chs(vstate, VTSS_BITMASK(VTSS_FDMA_CH_CNT), FALSE);

    // Create new channel state defaults
    ch_state_defaults(vstate);

    state->hdr_size     = VTSS_FDMA_HDR_SIZE_BYTES;
    state->xtr_hdr_size = VTSS_FDMA_XTR_HDR_SIZE_BYTES;
    state->inj_hdr_size = VTSS_FDMA_INJ_HDR_SIZE_BYTES;
    state->xtr_ch_cnt   = 1;

    return VTSS_RC_OK;
}

/*****************************************************************************/
// jr2_fdma_uninit()
/*****************************************************************************/
static vtss_rc jr2_fdma_uninit(vtss_state_t *const vstate)
{
    disable_and_clear_chs(vstate, VTSS_BITMASK(VTSS_FDMA_CH_CNT), TRUE);
    return VTSS_RC_OK;
}

/*****************************************************************************/
// jr2_fdma_tx()
/*****************************************************************************/
static vtss_rc jr2_fdma_tx(vtss_state_t          *const vstate,
                           fdma_sw_dcb_t         *sw_dcb,
                           vtss_fdma_tx_info_t   *const fdma_info,
                           vtss_packet_tx_info_t *const tx_info,
                           BOOL                  afi_cancel)
{
    fdma_sw_dcb_t  *tail;
    u32            dcb_cnt, frm_size;

    VTSS_RC(inj_ifh_init(vstate, sw_dcb, tx_info, &frm_size));
    VTSS_RC(inj_dcb_init(vstate, sw_dcb, &tail, &dcb_cnt, FALSE));
    return vtss_fdma_cmn_inj_attach_frm(vstate, sw_dcb, tail, VTSS_FDMA_CH_INJ, dcb_cnt, frm_size /* for statistics */, fdma_info, FALSE);
}

/*****************************************************************************/
// jr2_fdma_stats_clr()
/*****************************************************************************/
static vtss_rc jr2_fdma_stats_clr(vtss_state_t *const vstate)
{
    fdma_state_t *state = &vstate->fdma_state;
    /*lint --e{529} */ // Avoid "Symbol 'flags' not subsequently referenced" Lint warning
    FDMA_INTERRUPT_FLAGS flags;
    FDMA_INTERRUPT_DISABLE(flags);
    memset(&state->fdma_stats, 0, sizeof(state->fdma_stats));
    FDMA_INTERRUPT_RESTORE(flags);
    return VTSS_RC_OK;
}

/*****************************************************************************/
// jr2_debug_print()
/*****************************************************************************/
static vtss_rc jr2_debug_print(vtss_state_t *const vstate, const vtss_debug_printf_t pr, const vtss_debug_info_t *const info)
{
    fdma_state_t *state = &vstate->fdma_state;
    int          ch, qu;

    pr("Interrupts: %u\n\n", state->fdma_stats.intr_cnt);
    pr("Ch Usage DCBs Used  DCBs Added Packets    Bytes          Interrupts\n");
    pr("-- ----- ---------- ---------- ---------- -------------- ----------\n");
    for (ch = 0; ch < VTSS_FDMA_CH_CNT; ch++) {
        vtss_fdma_ch_usage_t usage = state->fdma_ch_state[ch].usage;
        pr("%2d %-5s %10u %10u %10u %14llu %10u\n",
           ch,
           usage == VTSS_FDMA_CH_USAGE_UNUSED ? "Free" : usage == VTSS_FDMA_CH_USAGE_XTR ? "Xtr" : usage == VTSS_FDMA_CH_USAGE_INJ ? "Inj" : "UNKNOWN",
           state->fdma_stats.dcbs_used[ch],
           state->fdma_stats.dcbs_added[ch],
           state->fdma_stats.packets[ch],
           state->fdma_stats.bytes[ch],
           state->fdma_stats.ch_intr_cnt[ch]);
    }

    pr("\n");
    pr("Xtr Qu Packets    Drops\n");
    pr("------ ---------- ----------\n");
    for (qu = 0; qu < VTSS_PACKET_RX_QUEUE_CNT; qu++) {
        pr("%6d %10u %10u\n", qu, state->fdma_stats.xtr_qu_packets[qu][0], state->fdma_stats.xtr_qu_drops[qu][0]);
    }

    pr("\n");
    return VTSS_RC_OK;
}

/*****************************************************************************/
// jr2_fdma_irq_handler()
/*****************************************************************************/
static vtss_rc jr2_fdma_irq_handler(vtss_state_t *const vstate, void *const cntxt)
{
    fdma_state_t   *state           = &vstate->fdma_state;
    u32            intr_ident       = J2_RD(vstate, VTSS_ICPU_CFG_FDMA_FDMA_INTR_IDENT); // Which enabled channel is interrupting?
    u32            intr_err         = J2_RD(vstate, VTSS_ICPU_CFG_FDMA_FDMA_EVT_ERR);    // Who is signaling error events?
    u32            intr_llp         = J2_RD(vstate, VTSS_ICPU_CFG_FDMA_FDMA_INTR_LLP);   // Who is signaling LLP interrupts?
    u32            intr_frm         = J2_RD(vstate, VTSS_ICPU_CFG_FDMA_FDMA_INTR_FRM);   // Who is signaling FRM interrupts?
    u32            intr_sig         = J2_RD(vstate, VTSS_ICPU_CFG_FDMA_FDMA_INTR_SIG);   // Who is signaling SIG interrupts?
    u32            combined_intr_cause, err_mask;
    vtss_fdma_ch_t ch;

    state->fdma_stats.intr_cnt++;

    // We don't expect any errors. Write a message and clear them if they occur.
    if ((err_mask = (intr_err & intr_ident)) != 0) {
        VTSS_EG(VTSS_TRACE_GROUP_FDMA_IRQ, "Channel error: %u", err_mask);

        // Force the channel to disabled state.
        J2_WR(vstate, VTSS_ICPU_CFG_FDMA_FDMA_CH_FORCEDIS, err_mask);

        // Clear the sticky error
        J2_WR(vstate, VTSS_ICPU_CFG_FDMA_FDMA_EVT_ERR, err_mask);
    }

    // We also don't expect any SIG interrupts (for now). Write a message and clear them if they occur.
    FDMA_ASSERT(IRQ, (intr_sig & intr_ident) == 0, J2_WR(vstate, VTSS_ICPU_CFG_FDMA_FDMA_INTR_SIG, intr_sig & intr_ident););

    // Clear interrupts.
    J2_WR(vstate, VTSS_ICPU_CFG_FDMA_FDMA_INTR_LLP, intr_llp & intr_ident);
    J2_WR(vstate, VTSS_ICPU_CFG_FDMA_FDMA_INTR_FRM, intr_frm & intr_ident);

    VTSS_OS_REORDER_BARRIER();

    combined_intr_cause = (intr_llp | intr_frm | intr_err) & intr_ident;

    for (ch = 0; ch < VTSS_FDMA_CH_CNT; ch++) {
        if (VTSS_BIT(ch) & combined_intr_cause) {
            fdma_ch_state_t *ch_state = &state->fdma_ch_state[ch];

            state->fdma_stats.ch_intr_cnt[ch]++;

            switch (ch_state->usage) {
            case VTSS_FDMA_CH_USAGE_XTR:
                if (intr_err & VTSS_BIT(ch)) {
                    // Gotta restart the channel in case of an error.
                    // The channel is already force-disabled. Also clear the LLP pointer in H/W.
                    J2_WR(vstate, VTSS_ICPU_CFG_FDMA_FDMA_DCB_LLP(ch), 0);

                    if (ch_state->cur_head) {
                        // Re-initialize cur_head DCBs (could be that the FDMA had filled some of them in prior to the error).
                        fdma_sw_dcb_t *temp = ch_state->cur_head;
                        ch_state->cur_head = NULL;
                        (void)vtss_fdma_cmn_dcb_release(vstate, ch, temp, TRUE /* thread safe */, TRUE /* append to to ch_state->free_head */, FALSE /* don't restart channel (we do it below instead) */);
                    }

                    vtss_fdma_cmn_xtr_restart_ch(vstate, ch);
                }

                // Unfortunately, we don't always get FRM interrupts when the very
                // last DCB has been filled with frame data. This means that we
                // can't depend on that interrupt for loading a new list of
                // DCBs to the FDMA.
                // Instead, we have to rely on the LLP interrupt, which fires
                // when the very last DCB of the current chain gets *loaded*.
                // When this event happens, we must feed a new list of DCBs
                // to the channel (if any in ch_state->free_head).
                // The very last DCB of the previous list is already loaded
                // into H/W, and if it receives a frame before we get to
                // load a new LLP pointer into H/W, we will not get a FRM
                // interrupt from the previous list's last DCB, unless it's also EOF-marked.
                // If however, we get to load a new LLP pointer into H/W
                // before the last DCB of the previous list is filled with
                // frame data, we will get a FRM interrupt.
                // To overcome the first situation, we must execute
                // xtr_irq_frm_done() even on LLP interrupts only.
                if ((intr_llp & VTSS_BIT(ch)) || (intr_err & VTSS_BIT(ch))) {
                    vtss_fdma_cmn_xtr_restart_ch(vstate, ch);
                }

                // Whether or not the FRM interrupt is active, we must
                // check for done frames, due to missing FRM interrupt
                // in certain situations (see above).
                xtr_irq_frm_done(vstate, cntxt, ch);
                break;

            case VTSS_FDMA_CH_USAGE_INJ:
                inj_irq_tfr_done(vstate, cntxt, ch);
                break;

            default:
                // Spurious interrupt.
                /*lint -e(506) */ // Avoid "Constant value Boolean"
                FDMA_ASSERT(IRQ, FALSE, return VTSS_RC_ERROR;);
            }
        }
    }

    return VTSS_RC_OK;
}

/*****************************************************************************/
// jr2_fdma_func_init()
// Initialize current global state's function pointers.
// This is the only real public function.
/*****************************************************************************/
void jr2_fdma_func_init(vtss_state_t *vtss_state)
{
    fdma_func_t *func = &vtss_state->fdma_state.fdma_func;

    // Internal functions
    func->fdma_init_conf_set      = jr2_fdma_init_conf_set;
    func->fdma_xtr_qu_suspend_set = jr2_fdma_xtr_qu_suspend_set;
    func->fdma_xtr_dcb_init       = jr2_fdma_xtr_dcb_init;
    func->fdma_xtr_connect        = jr2_fdma_xtr_connect;
    func->fdma_xtr_restart_ch     = jr2_fdma_xtr_restart_ch;
    func->fdma_inj_restart_ch     = jr2_fdma_inj_restart_ch;
    func->fdma_start_ch           = jr2_fdma_start_ch;

    // External functions
    func->fdma_uninit             = jr2_fdma_uninit;
    func->fdma_stats_clr          = jr2_fdma_stats_clr;
    func->fdma_debug_print        = jr2_debug_print;
    func->fdma_irq_handler        = jr2_fdma_irq_handler;
    func->fdma_tx                 = jr2_fdma_tx;
}

#endif /* defined(VTSS_OPT_VCORE_III) && VTSS_OPT_FDMA && defined(VTSS_ARCH_JAGUAR_2) */

/*****************************************************************************/
//
// End of file
//
//****************************************************************************/

