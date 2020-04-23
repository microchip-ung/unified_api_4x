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
#define VTSS_TRACE_GROUP VTSS_TRACE_GROUP_AFI
#include "vtss_jaguar2_cil.h"

#if defined(VTSS_ARCH_JAGUAR_2) && defined(VTSS_FEATURE_AFI_SWC) && defined(VTSS_AFI_V2)

#define JR2_AFI_CHIP_PORT(port_no) ((port_no) == VTSS_PORT_NO_NONE ? VTSS_CHIP_PORT_VD1 : VTSS_CHIP_PORT(port_no))
#define JR2_AFI_E_RETURN(rc, ...) do {VTSS_E(__VA_ARGS__); return rc;} while(0)

/******************************************************************************/
//
// Internal CIL: Misc. functions
//
/******************************************************************************/

/**
 * jr2_afi_clock_period_get()
 */
static vtss_rc jr2_afi_clock_period_get(vtss_state_t *vtss_state)
{
    u32 res;

#if defined(VTSS_ARCH_JAGUAR_2_B)
    res = 4000;
#elif defined(VTSS_ARCH_JAGUAR_2_C)
    u32 val;
    JR2_RD(VTSS_HSIO_PLL5G_CFG_PLL5G_CFG0(0 /* TBD */), &val);
    val = VTSS_X_HSIO_PLL5G_CFG_PLL5G_CFG0_CORE_CLK_DIV(val);
    switch (val) {
    case 0:
    case 33:
        // 625 MHz
        res = 1600;
        break;

    case 1:
        // 312.5 MHz
        res = 3200;
        break;

    case 2:
    case 4:
        // 500 MHz
        res = 2000;
        break;

    case 3:
        // 277.77 MHz
        res = 3600;
        break;

    case 5:
        // 250 MHz
        res = 4000;
        break;

    case 6:
    case 8:
        // 416.66 MHz
        res = 2400;
        break;

    case 7:
        // 227.27 MHz
        res = 4400;
        break;

    case 9:
        // 208.33 MHz
        res = 4800;
        break;

    case 10:
    case 12:
        // 357.14 MHz
        res = 2800;
        break;

    case 13:
        // 178.57 MHz
        res = 5600;
        break;

    case 14:
        // 312.5 MHz
        res = 3200;
        break;

    case 15:
        // 166.66 MHz
        res = 6000;
        break;

    case 17:
        // 156.25 MHz
        res = 6400;
        break;

    default:
        VTSS_E("Unknown core clock divider (%u). Assuming 4 ns clock period", val);
        res = 4000;
        break;
    }

#elif defined(VTSS_ARCH_SERVAL_T)
    u32 val, div2;

    JR2_RD(VTSS_HSIO_PLL5G_CFG_PLL5G_CFG0(0), &val);
    val = VTSS_X_HSIO_PLL5G_CFG_PLL5G_CFG0_CORE_CLK_DIV(val);

    switch (val) {
    case 17:
        // 156.25 MHz
        res = 6400;
        break;

    case 25:
        // 104.17 MHz
        res = 9600;
        break;

    default:
        VTSS_E("Unknown core clock divider (%u). Assuming 6.4 ns clock period", val);
        res = 6400;
        break;
    }


    JR2_RD(VTSS_HSIO_HW_CFGSTAT_CLK_CFG, &div2);
    if (VTSS_X_HSIO_HW_CFGSTAT_CLK_CFG_SWC_CLK_DIV2(div2)) {
        res *= 2;
    }
#else
#error "Update this piece of code."
#endif

    vtss_state->afi.clk_period_ps = res;
    return VTSS_RC_OK;
}

/**
 * jr2_afi_debug()
 */
static vtss_rc jr2_afi_debug(vtss_state_t *vtss_state, const vtss_debug_printf_t pr, const vtss_debug_info_t *const info)
{
    u64 t_us[8], base_us;
    u32 val, val0, val1, t[8], idx, frm_ptr, next_and_type, part0;

    // Timer Tick Table
    pr("Timer Tick Table\n");
    pr("Clock period: %" PRIu64 " ps\n", vtss_state->afi.clk_period_ps);
    pr("Idx  Value Length [us]\n");
    pr("---- ----- -----------\n");

    JR2_RD(VTSS_AFI_TTI_TICKS_TTI_TICK_LEN_0_3, &val0);
    JR2_RD(VTSS_AFI_TTI_TICKS_TTI_TICK_LEN_4_7, &val1);
    t[0] = VTSS_X_AFI_TTI_TICKS_TTI_TICK_LEN_0_3_LEN0(val0);
    t[1] = VTSS_X_AFI_TTI_TICKS_TTI_TICK_LEN_0_3_LEN1(val0);
    t[2] = VTSS_X_AFI_TTI_TICKS_TTI_TICK_LEN_0_3_LEN2(val0);
    t[3] = VTSS_X_AFI_TTI_TICKS_TTI_TICK_LEN_0_3_LEN3(val0);
    t[4] = VTSS_X_AFI_TTI_TICKS_TTI_TICK_LEN_4_7_LEN4(val1);
    t[5] = VTSS_X_AFI_TTI_TICKS_TTI_TICK_LEN_4_7_LEN5(val1);
    t[6] = VTSS_X_AFI_TTI_TICKS_TTI_TICK_LEN_4_7_LEN6(val1);
    t[7] = VTSS_X_AFI_TTI_TICKS_TTI_TICK_LEN_4_7_LEN7(val1);

    JR2_RD(VTSS_AFI_TTI_TICKS_TTI_TICK_BASE, &val);
    val0 = VTSS_X_AFI_TTI_TICKS_TTI_TICK_BASE_BASE_LEN(val);
    base_us = VTSS_DIV64(val0 * vtss_state->afi.clk_period_ps, 1000000LLU);
    pr("Base %5u %11" PRIu64 "\n", val0, base_us);

    for (idx = 0; idx < VTSS_ARRSZ(t); idx++) {
        t_us[idx] = (idx == 0 ? base_us : t_us[idx - 1]) * t[idx];
        pr("%4u %5u %11" PRIu64 "\n", idx, t[idx], t_us[idx]);
    }

    // TTI Table
    pr("\nTTI Table\n");
    pr("Idx  TickIdx TmrLen Period [us] Jit FrmPtr Type  NextPtr Part0\n");
    pr("---- ------- ------ ----------- --- ------ ----- ------- ----------\n");
    for (idx = 0; idx < VTSS_ARRSZ(vtss_state->afi.tti_tbl); idx++) {
        BOOL enabled;
        u32  tick_idx, tmr_len;

        JR2_RD(VTSS_AFI_TTI_TBL_TTI_TIMER(idx), &val);

#if defined(VTSS_ARCH_JAGUAR_2_B)
        enabled = VTSS_X_AFI_TTI_TBL_TTI_TIMER_TIMER_LEN(val) != 0;
#else
        enabled = VTSS_X_AFI_TTI_TBL_TTI_TIMER_TIMER_ENA(val) && VTSS_X_AFI_TTI_TBL_TTI_TIMER_TIMER_LEN(val) != 0;
#endif /* (!)defined(VTSS_ARCH_JAGUAR_2_B) */

        if (!enabled) {
            continue;
        }

        tick_idx = VTSS_X_AFI_TTI_TBL_TTI_TIMER_TICK_IDX(val);
        tmr_len  = VTSS_X_AFI_TTI_TBL_TTI_TIMER_TIMER_LEN(val);

        // Get pointer to first frame table entry
        JR2_RD(VTSS_AFI_TTI_TBL_TTI_FRM(idx), &frm_ptr);
        frm_ptr = VTSS_X_AFI_TTI_TBL_TTI_FRM_FRM_PTR(frm_ptr);

        // Get what's in the first frame table entry
        JR2_RD(VTSS_AFI_FRM_TBL_FRM_NEXT_AND_TYPE(frm_ptr), &next_and_type);
        JR2_RD(VTSS_AFI_FRM_TBL_FRM_ENTRY_PART0(frm_ptr), &part0);

        pr("%4u %7u %6u %11" PRIu64 " %3u 0x%04x %-5s 0x%05x 0x%08x\n",
           idx,
           tick_idx,
           tmr_len,
           t_us[tick_idx] * tmr_len,
           VTSS_X_AFI_TTI_TBL_TTI_TIMER_JITTER(val),
           frm_ptr,
           VTSS_X_AFI_FRM_TBL_FRM_NEXT_AND_TYPE_ENTRY_TYPE(next_and_type) == 0 ? "Frame" : "Delay",
           VTSS_X_AFI_FRM_TBL_FRM_NEXT_AND_TYPE_NEXT_PTR(next_and_type),
           part0);
    }

    // DTI Table
    pr("\nDTI table\n");
    pr("Idx FrmPtr Type  NextPtr Part0\n");
    pr("--- ------ ----- ------- ----------\n");
    for (idx = 0; idx < VTSS_ARRSZ(vtss_state->afi.dti_tbl); idx++) {
        JR2_RD(VTSS_AFI_DTI_MISC_DTI_CTRL(idx), &val);

        if (!VTSS_X_AFI_DTI_MISC_DTI_CTRL_ENA(val)) {
            // DTI index not in use
            continue;
        }

        // Get pointer to first frame table entry
        JR2_RD(VTSS_AFI_DTI_TBL_DTI_FRM(idx), &frm_ptr);
        frm_ptr = VTSS_X_AFI_TTI_TBL_TTI_FRM_FRM_PTR(frm_ptr);

        // Get what's in the first frame table entry
        JR2_RD(VTSS_AFI_FRM_TBL_FRM_NEXT_AND_TYPE(frm_ptr), &next_and_type);
        JR2_RD(VTSS_AFI_FRM_TBL_FRM_ENTRY_PART0(frm_ptr), &part0);

        pr("%3u 0x%04x %-5s 0x%05x 0x%08x\n",
           idx,
           frm_ptr,
           VTSS_X_AFI_FRM_TBL_FRM_NEXT_AND_TYPE_ENTRY_TYPE(next_and_type) == 0 ? "Frame" : "Delay",
           VTSS_X_AFI_FRM_TBL_FRM_NEXT_AND_TYPE_NEXT_PTR(next_and_type),
           part0);
    }

    pr("\n");

    return VTSS_RC_OK;
}

// Reference to queue into which frame shall be injected.
typedef struct {
    vtss_phys_port_no_t chip_port;

#if defined(VTSS_ARCH_JAGUAR_2_B)
    u32 se_idx; // [0; 4065]
    u32 se_inp; // [0; VTSS_CHIP_PORT_VD1]
#else
    u32 qu_num; // [0; 3357] on ServalT and [0; 272xx] on Jaguar_2_C
#endif /* (!)defined(VTSS_ARCH_JAGUAR_2_B) */
} jr2_afi_qu_ref_t;

/*
 * jr2_afi_port_prio_2_qu_ref()
 */
static void jr2_afi_port_prio_2_qu_ref(vtss_state_t *const vtss_state, vtss_port_no_t port_no, vtss_prio_t prio, jr2_afi_qu_ref_t *qu_ref)
{
    qu_ref->chip_port = JR2_AFI_CHIP_PORT(port_no); // Destination port

    // Prioritized traffic to VD1 is not supported.
    // Auto-pick Super Priority.
    if (qu_ref->chip_port == VTSS_CHIP_PORT_VD1) {
        if (prio < VTSS_PRIO_SUPER) {
            prio = VTSS_PRIO_SUPER;
        }
    }

#if defined(VTSS_ARCH_JAGUAR_2_B)
    if (prio == VTSS_PRIO_SUPER) {
        // Super priority
        qu_ref->se_idx = 4064;
        qu_ref->se_inp = qu_ref->chip_port;
    } else if (prio > VTSS_PRIO_SUPER) {
        // Shaped super priority (no define for this yet).
        qu_ref->se_idx = 4065;
        qu_ref->se_inp = qu_ref->chip_port;
    } else {
        // For other priorities, we pick the CPU port as source.
        qu_ref->se_idx = qu_ref->chip_port * 8 + prio;
        qu_ref->se_inp = VTSS_CHIP_PORT_CPU_0;
    }
#else
    // Other JR2 variants

#if defined(VTSS_ARCH_JAGUAR_2_C)
// Jaguar 2C
#define VTSS_AFI_QU_SP0           27200
#define VTSS_AFI_QU_SP1           27264
#define VTSS_AFI_HSCH_LARGE_RATIO     8
#elif defined(VTSS_ARCH_SERVAL_T)
// Serval-T
#define VTSS_AFI_QU_SP0            3328
#define VTSS_AFI_QU_SP1            3343
#define VTSS_AFI_HSCH_LARGE_RATIO     2
#else
#error "Update this for new chip variants"
#endif /* 2C/ServalT/other */

    if (prio == VTSS_PRIO_SUPER) {
        // Super priority
        qu_ref->qu_num = VTSS_AFI_QU_SP0 + qu_ref->chip_port;
    } else if (prio > VTSS_PRIO_SUPER) {
        qu_ref->qu_num = VTSS_AFI_QU_SP1 + qu_ref->chip_port;
    } else {
        // Assume the destination is not amongst the VDx and CPU ports
        // when we arrive here, because had it been so, the equation
        // would have been quite different.
        // We pick the CPU port as source.
        qu_ref->qu_num = 8 * VTSS_AFI_HSCH_LARGE_RATIO * (qu_ref->chip_port * 8 + prio) + VTSS_CHIP_PORT_CPU_0;
    }
#endif
}

/*
 * jr2_afi_port_tbl_entry()
 *
 * Takes a logical port number and returns a vtss_afi_port_t pointer.
 * The catch is that the function also supports Virtual Device #1,
 * which is indicated by port_no == VTSS_PORT_NO_NONE. In this case,
 * the last entry of the port_tbl[] array (which is one longer than
 * the number of front ports on this platform) is returned.
 */
vtss_afi_port_t *jr2_afi_port_tbl_entry(vtss_state_t *vtss_state, vtss_port_no_t port_no)
{
    return &vtss_state->afi.port_tbl[port_no == VTSS_PORT_NO_NONE ? VTSS_ARRSZ(vtss_state->afi.port_tbl) - 1 : port_no];
}

/*
 * jr2_afi_port_start()
 */
static vtss_rc jr2_afi_port_start(vtss_state_t *const vtss_state, vtss_port_no_t port_no)
{
    vtss_phys_port_no_t chip_port = JR2_AFI_CHIP_PORT(port_no);

#if defined(VTSS_ARCH_JAGUAR_2_B)
    vtss_afi_port_t *port = jr2_afi_port_tbl_entry(vtss_state, port_no);

    // Set FRM_OUT_MAX back to normal
    JR2_WRM(VTSS_AFI_PORT_TBL_PORT_CFG(chip_port),
            VTSS_F_AFI_PORT_TBL_PORT_CFG_FRM_OUT_MAX(port->frm_out_max),
            VTSS_M_AFI_PORT_TBL_PORT_CFG_FRM_OUT_MAX);
#else
    // Set FRM_RM_ONLY back to 0
    JR2_WRM(VTSS_AFI_PORT_TBL_PORT_CFG(chip_port),
            VTSS_F_AFI_PORT_TBL_PORT_CFG_FRM_RM_ONLY(0),
            VTSS_M_AFI_PORT_TBL_PORT_CFG_FRM_RM_ONLY);
#endif /* (!)defined(VTSS_ARCH_JAGUAR_2_B) */
    // Set FC_SKIP_TTI_INJ=0
    JR2_WRM(VTSS_AFI_PORT_TBL_PORT_CFG(chip_port),
            VTSS_F_AFI_PORT_TBL_PORT_CFG_FC_SKIP_TTI_INJ(0),
            VTSS_M_AFI_PORT_TBL_PORT_CFG_FC_SKIP_TTI_INJ);

    return VTSS_RC_OK;
}

/*
 * jr2_afi_chip_port_stop()
 */
static vtss_rc jr2_afi_chip_port_stop(vtss_state_t *const vtss_state, vtss_phys_port_no_t chip_port)
{
#if defined(VTSS_ARCH_JAGUAR_2_B)
    // Use FRM_OUT_MAX=0 to stop port
    JR2_WRM(VTSS_AFI_PORT_TBL_PORT_CFG(chip_port),
            VTSS_F_AFI_PORT_TBL_PORT_CFG_FRM_OUT_MAX(0),
            VTSS_M_AFI_PORT_TBL_PORT_CFG_FRM_OUT_MAX);
#else
    // Use FRM_RM_ONLY=1 to stop port
    JR2_WRM(VTSS_AFI_PORT_TBL_PORT_CFG(chip_port),
            VTSS_F_AFI_PORT_TBL_PORT_CFG_FRM_RM_ONLY(1),
            VTSS_M_AFI_PORT_TBL_PORT_CFG_FRM_RM_ONLY);
#endif /* (!)defined(VTSS_ARCH_JAGUAR_2_B) */

    // Set FC_SKIP_TTI_INJ = 1
    JR2_WRM(VTSS_AFI_PORT_TBL_PORT_CFG(chip_port),
            VTSS_F_AFI_PORT_TBL_PORT_CFG_FC_SKIP_TTI_INJ(1),
            VTSS_M_AFI_PORT_TBL_PORT_CFG_FC_SKIP_TTI_INJ);

    return VTSS_RC_OK;
}

/*
 * jr2_afi_port_stop()
 */
static vtss_rc jr2_afi_port_stop(vtss_state_t *const vtss_state, vtss_port_no_t port_no)
{
    return jr2_afi_chip_port_stop(vtss_state, JR2_AFI_CHIP_PORT(port_no));
}

/*
 * jr2_afi_port_link_up()
 */
static vtss_rc jr2_afi_port_link_up(vtss_state_t *const vtss_state, vtss_port_no_t port_no)
{
    vtss_afi_port_t *port = jr2_afi_port_tbl_entry(vtss_state, port_no);

    VTSS_I("Enter. port = %u, link = %d, started = %d, frm_out_max = %u", port_no, port->link, port->started, port->frm_out_max);

    if (port->link) {
        JR2_AFI_E_RETURN(VTSS_RC_ERROR, "Port %u already has link", port_no);
    }

    if (port->started) {
        VTSS_RC(jr2_afi_port_start(vtss_state, port_no));
    }

    port->link = 1;
    VTSS_I("Exit.  port = %u, link = %d, started = %d, frm_out_max = %u", port_no, port->link, port->started, port->frm_out_max);

    return VTSS_RC_OK;
}

/*
 * jr2_afi_port_link_down()
 */
static vtss_rc jr2_afi_port_link_down(vtss_state_t *const vtss_state, vtss_port_no_t port_no)
{
    vtss_afi_port_t *port = jr2_afi_port_tbl_entry(vtss_state, port_no);

    VTSS_I("Enter. port = %u, link = %d, started = %d, frm_out_max = %u", port_no, port->link, port->started, port->frm_out_max);

    if (!port->link) {
        JR2_AFI_E_RETURN(VTSS_RC_ERROR, "Port %u has no link", port_no);
    }

    VTSS_RC(jr2_afi_port_stop(vtss_state, port_no));
    port->link = 0;

    VTSS_I("Exit.  port = %u, link = %d, started = %d, frm_out_max = %u", port_no, port->link, port->started, port->frm_out_max);

    return VTSS_RC_OK;
}

/*
 * jr2_afi_frm_hijack()
 *
 * Setup frame, hijacked by AFI
 */
static vtss_rc jr2_afi_frm_hijack(vtss_state_t *const vtss_state, i32 frm_idx)
{
    vtss_afi_frm_t *frm = &vtss_state->afi.frm_tbl[frm_idx];
    u32            new_frm_vld = 0;
    u32            frm_info;
    u32            max_poll_cnt = 5;

    // Wait for frame to be hijacked
    while (!new_frm_vld && max_poll_cnt-- > 0) {
        JR2_RD(VTSS_AFI_MISC_NEW_FRM_CTRL, &new_frm_vld);
        new_frm_vld = VTSS_X_AFI_MISC_NEW_FRM_CTRL_VLD(new_frm_vld);
    }

    if (!new_frm_vld) {
        JR2_AFI_E_RETURN(VTSS_RC_ERROR, "Timeout waiting for NEW_FRM_CTRL.VLD");
    }

    // Get frm_info for hijacked frame
    JR2_RD(VTSS_AFI_MISC_NEW_FRM_INFO, &frm_info);
    frm_info = VTSS_X_AFI_MISC_NEW_FRM_INFO_FRM_INFO(frm_info);
    frm->frm_delay.frm.frm_info.fp =
        VTSS_EXTRACT_BITFIELD(frm_info,
                              VTSS_AFI_FRM_INFO_FP_POS,
                              VTSS_AFI_FRM_INFO_FP_WID);
    frm->frm_delay.frm.frm_info.fshort =
        VTSS_EXTRACT_BITFIELD(frm_info,
                              VTSS_AFI_FRM_INFO_SHORT_POS,
                              VTSS_AFI_FRM_INFO_SHORT_WID);
    frm->frm_delay.frm.frm_info.eprio =
        VTSS_EXTRACT_BITFIELD(frm_info,
                              VTSS_AFI_FRM_INFO_EPRIO_POS,
                              VTSS_AFI_FRM_INFO_EPRIO_WID);

    // Setup FRM_TBL entry
    JR2_WRM(VTSS_AFI_FRM_TBL_FRM_NEXT_AND_TYPE(frm_idx),
            VTSS_F_AFI_FRM_TBL_FRM_NEXT_AND_TYPE_ENTRY_TYPE(0),
            VTSS_M_AFI_FRM_TBL_FRM_NEXT_AND_TYPE_ENTRY_TYPE);

    JR2_WR(VTSS_AFI_FRM_TBL_FRM_ENTRY_PART0(frm_idx),
           (frm->frm_delay.frm.frm_info.fp     << (VTSS_AFI_FRM_INFO_FP_POS    + VTSS_AFI_FRM_TBL_PART0_FRM_INFO_POS)) |
           (frm->frm_delay.frm.frm_info.fshort << (VTSS_AFI_FRM_INFO_SHORT_POS + VTSS_AFI_FRM_TBL_PART0_FRM_INFO_POS)) |
           (frm->frm_delay.frm.frm_info.eprio  << (VTSS_AFI_FRM_INFO_EPRIO_POS + VTSS_AFI_FRM_TBL_PART0_FRM_INFO_POS)));

    JR2_WR(VTSS_AFI_MISC_NEW_FRM_CTRL, VTSS_F_AFI_MISC_NEW_FRM_CTRL_VLD(0));

    return VTSS_RC_OK;
}

/*
 * jr2_afi_set_bit_field()
 *
 * Set value of field within u32.
 * offset : Offset of field within val32
 * wid    : Width of field
 * fld_val: Field value
 */
static void jr2_afi_set_bit_field(u32 *const val32, u8 offset, u8 wid, u32  fld_val)
{
    if ((fld_val >> wid) != 0) {
        VTSS_E("Invalid arguments: fld_val = %u, wid = %u", fld_val, wid);
    }

    *val32 = (*val32 & ~(u32)(((1 << wid) - 1) << offset)) | (fld_val << offset);
}

/*
 * jr2_afi_frm_setup()
 *
 * Configure Frame entry in FRM_TBL
 * Note: In PART0 only INJ_CNT is updated.
 */
static vtss_rc jr2_afi_frm_setup(vtss_state_t *const vtss_state, i32 frm_idx)
{
    vtss_afi_frm_t *frm = &vtss_state->afi.frm_tbl[frm_idx];
    u32            frm_tbl_part0;

    JR2_RD(VTSS_AFI_FRM_TBL_FRM_ENTRY_PART0(frm_idx), &frm_tbl_part0);
    jr2_afi_set_bit_field(&frm_tbl_part0,
                          VTSS_AFI_FRM_TBL_PART0_INJ_CNT_POS,
                          VTSS_AFI_FRM_TBL_PART0_INJ_CNT_WID,
                          frm->frm_delay.frm.inj_cnt);
    JR2_WR(VTSS_AFI_FRM_TBL_FRM_ENTRY_PART0(frm_idx), frm_tbl_part0);

    JR2_WR(VTSS_AFI_FRM_TBL_FRM_NEXT_AND_TYPE(frm_idx),
           VTSS_F_AFI_FRM_TBL_FRM_NEXT_AND_TYPE_NEXT_PTR(frm->next_ptr) |
           VTSS_F_AFI_FRM_TBL_FRM_NEXT_AND_TYPE_ENTRY_TYPE(0));

    return VTSS_RC_OK;
}

/*
 * jr2_afi_delay_setup()
 *
 * Configure Delay entry in FRM_TBL
 * Note: In PART0 only DELAY is updated
 */
static vtss_rc jr2_afi_delay_setup(vtss_state_t *const vtss_state, i32 frm_idx)
{
    vtss_afi_frm_t *frm = &vtss_state->afi.frm_tbl[frm_idx];
    u32             frm_tbl_part0;

    JR2_RD(VTSS_AFI_FRM_TBL_FRM_ENTRY_PART0(frm_idx), &frm_tbl_part0);
    jr2_afi_set_bit_field(&frm_tbl_part0,
                          VTSS_AFI_FRM_TBL_PART0_DELAY_POS,
                          VTSS_AFI_FRM_TBL_PART0_DELAY_WID,
                          frm->frm_delay.delay.delay);
    JR2_WR(VTSS_AFI_FRM_TBL_FRM_ENTRY_PART0(frm_idx), frm_tbl_part0);

    JR2_WR(VTSS_AFI_FRM_TBL_FRM_NEXT_AND_TYPE(frm_idx),
           VTSS_F_AFI_FRM_TBL_FRM_NEXT_AND_TYPE_NEXT_PTR(frm->next_ptr) |
           VTSS_F_AFI_FRM_TBL_FRM_NEXT_AND_TYPE_ENTRY_TYPE(1));

    return VTSS_RC_OK;
}

/*
 * jr2_afi_set_frm_rm()
 *
 * Set FRM_RM = 1 for frame
 */
static vtss_rc jr2_afi_set_frm_rm(vtss_state_t *const vtss_state, i32 frm_idx)
{
    u32 frm_tbl_part0;

    VTSS_RC(afi_frm_idx_chk(vtss_state, frm_idx));

    JR2_RD(VTSS_AFI_FRM_TBL_FRM_ENTRY_PART0(frm_idx), &frm_tbl_part0);

    if (VTSS_EXTRACT_BITFIELD(frm_tbl_part0, VTSS_AFI_FRM_TBL_PART0_FRM_RM_POS, 1)) {
        JR2_AFI_E_RETURN(VTSS_RC_ERROR, "frm_rm already set");
    }

    if (VTSS_EXTRACT_BITFIELD(frm_tbl_part0, VTSS_AFI_FRM_TBL_PART0_FRM_GONE_POS, 1)) {
        JR2_AFI_E_RETURN(VTSS_RC_ERROR, "frm_gone already set");
    }

    frm_tbl_part0 |= (1 << VTSS_AFI_FRM_TBL_PART0_FRM_RM_POS);
    JR2_WR(VTSS_AFI_FRM_TBL_FRM_ENTRY_PART0(frm_idx), frm_tbl_part0);

    return VTSS_RC_OK;
}

/*
 * jr2_afi_get_frm_gone()
 */
static vtss_rc jr2_afi_get_frm_gone(vtss_state_t *const vtss_state, u8 *const frm_gone, i32 frm_idx)
{
    u32 frm_tbl_part0;

    JR2_RD(VTSS_AFI_FRM_TBL_FRM_ENTRY_PART0(frm_idx), &frm_tbl_part0);
    *frm_gone = VTSS_EXTRACT_BITFIELD(frm_tbl_part0, VTSS_AFI_FRM_TBL_PART0_FRM_GONE_POS, 1);

    return VTSS_RC_OK;
}

/******************************************************************************/
//
// Internal CIL: TTI functions
//
/******************************************************************************/

/*
 * jr2_afi_tti_cal_init()
 *
 * Initialize TTI calendar
 */
static vtss_rc jr2_afi_tti_cal_init(vtss_state_t *const vtss_state)
{
    u32  max_poll_cnt = 5;
    u32  val;
    BOOL tti_init = 1 /* picky compiler */;

    JR2_WRM(VTSS_AFI_TTI_MISC_TTI_CTRL,
            VTSS_F_AFI_TTI_MISC_TTI_CTRL_TTI_INIT(1),
            VTSS_M_AFI_TTI_MISC_TTI_CTRL_TTI_INIT);

    // Wait for device to clear TTI_INIT
    while (max_poll_cnt-- > 0) {
        JR2_RD(VTSS_AFI_TTI_MISC_TTI_CTRL, &val);
        if ((tti_init = VTSS_X_AFI_TTI_MISC_TTI_CTRL_TTI_INIT(val)) == 0) {
            break;
        }
    }

    if (tti_init == 1) {
        JR2_AFI_E_RETURN(VTSS_RC_ERROR, "Timeout waiting for TTI_CTRL.TTI_INIT == 0");
    }

    return VTSS_RC_OK;
}

/*
 * jr2_afi_tti_tick_init()
 */
static vtss_rc jr2_afi_tti_tick_init(vtss_state_t *const vtss_state)
{
    u32 tick_base_len, val0, val1, idx;
    u64 t_ps[8], tick_base_ps;

    // Find a suitable base tick, given that we want the first tick length to be VTSS_AFI_TTI_TICK_LEN0_US
    tick_base_len = VTSS_DIV64(VTSS_AFI_TTI_TICK_LEN0_US * 1000000LLU, vtss_state->afi.clk_period_ps);
    if (tick_base_len == 0 || tick_base_len > VTSS_M_AFI_TTI_TICKS_TTI_TICK_BASE_BASE_LEN) {
        JR2_AFI_E_RETURN(VTSS_RC_ERROR, "Unable to find a suitable tick base given the first tick index request (%u us)", VTSS_AFI_TTI_TICK_LEN0_US);
    }

    JR2_WRM(VTSS_AFI_TTI_TICKS_TTI_TICK_BASE,
            VTSS_F_AFI_TTI_TICKS_TTI_TICK_BASE_BASE_LEN(tick_base_len),
            VTSS_M_AFI_TTI_TICKS_TTI_TICK_BASE_BASE_LEN);

    // Get true tick base without rounding errors.
    tick_base_ps = tick_base_len * vtss_state->afi.clk_period_ps;

    // Configure tick lengths
    JR2_WR(VTSS_AFI_TTI_TICKS_TTI_TICK_LEN_0_3,
           VTSS_F_AFI_TTI_TICKS_TTI_TICK_LEN_0_3_LEN0(VTSS_DIV64(VTSS_AFI_TTI_TICK_LEN0_US * 1000000LLU, tick_base_ps)) |
           VTSS_F_AFI_TTI_TICKS_TTI_TICK_LEN_0_3_LEN1(VTSS_AFI_TTI_TICK_LEN1_US / VTSS_AFI_TTI_TICK_LEN0_US) |
           VTSS_F_AFI_TTI_TICKS_TTI_TICK_LEN_0_3_LEN2(VTSS_AFI_TTI_TICK_LEN2_US / VTSS_AFI_TTI_TICK_LEN1_US) |
           VTSS_F_AFI_TTI_TICKS_TTI_TICK_LEN_0_3_LEN3(VTSS_AFI_TTI_TICK_LEN3_US / VTSS_AFI_TTI_TICK_LEN2_US));

    JR2_WR(VTSS_AFI_TTI_TICKS_TTI_TICK_LEN_4_7,
           VTSS_F_AFI_TTI_TICKS_TTI_TICK_LEN_4_7_LEN4(VTSS_AFI_TTI_TICK_LEN4_US / VTSS_AFI_TTI_TICK_LEN3_US) |
           VTSS_F_AFI_TTI_TICKS_TTI_TICK_LEN_4_7_LEN5(VTSS_AFI_TTI_TICK_LEN5_US / VTSS_AFI_TTI_TICK_LEN4_US) |
           VTSS_F_AFI_TTI_TICKS_TTI_TICK_LEN_4_7_LEN6(VTSS_AFI_TTI_TICK_LEN6_US / VTSS_AFI_TTI_TICK_LEN5_US) |
           VTSS_F_AFI_TTI_TICKS_TTI_TICK_LEN_4_7_LEN7(VTSS_AFI_TTI_TICK_LEN7_US / VTSS_AFI_TTI_TICK_LEN6_US));

    // Now that we have made the rounding errors that will come from using
    // these constants, update the array that the rest of the code uses.
    JR2_RD(VTSS_AFI_TTI_TICKS_TTI_TICK_LEN_0_3, &val0);
    JR2_RD(VTSS_AFI_TTI_TICKS_TTI_TICK_LEN_4_7, &val1);

    // In order to not accummulate rounding errors, first compute the
    // tick lengths in ps and then found them to microseconds.
    t_ps[0] = VTSS_X_AFI_TTI_TICKS_TTI_TICK_LEN_0_3_LEN0(val0) * tick_base_ps;
    t_ps[1] = VTSS_X_AFI_TTI_TICKS_TTI_TICK_LEN_0_3_LEN1(val0) * t_ps[0];
    t_ps[2] = VTSS_X_AFI_TTI_TICKS_TTI_TICK_LEN_0_3_LEN2(val0) * t_ps[1];
    t_ps[3] = VTSS_X_AFI_TTI_TICKS_TTI_TICK_LEN_0_3_LEN3(val0) * t_ps[2];
    t_ps[4] = VTSS_X_AFI_TTI_TICKS_TTI_TICK_LEN_4_7_LEN4(val1) * t_ps[3];
    t_ps[5] = VTSS_X_AFI_TTI_TICKS_TTI_TICK_LEN_4_7_LEN5(val1) * t_ps[4];
    t_ps[6] = VTSS_X_AFI_TTI_TICKS_TTI_TICK_LEN_4_7_LEN6(val1) * t_ps[5];
    t_ps[7] = VTSS_X_AFI_TTI_TICKS_TTI_TICK_LEN_4_7_LEN7(val1) * t_ps[6];

    for (idx = 0; idx < VTSS_ARRSZ(vtss_state->afi.tick_len_us); idx++) {
        vtss_state->afi.tick_len_us[idx] = VTSS_DIV64(t_ps[idx], 1000000LLU);
    }

    return VTSS_RC_OK;
}

/******************************************************************************/
//
// External CIL: DTI functions
//
/******************************************************************************/

/*
 * jr2_afi_dti_start()
 *
 * do_frm_delay_config: (Re)configure frame-delay sequence before (re)starting
 * do_dti_config:       (Re)configure DTI before (re)starting
 */
static vtss_rc jr2_afi_dti_start(vtss_state_t *const vtss_state, u32 dti_idx, BOOL do_frm_delay_config, BOOL do_dti_config)
{
    vtss_afi_dti_t *dti = &vtss_state->afi.dti_tbl[dti_idx];
    u32            i, frm_inj_cnt;
    i32            frm_idx;

    if (dti->started) {
        JR2_AFI_E_RETURN(VTSS_RC_ERROR, "DTI already started");
    }

    if (do_frm_delay_config) {
        // (Re)write FRM_TBL configuration for DTI
        for (i = 0; i < VTSS_AFI_DTI_MAX_SEQ_LEN; i++) {
            frm_idx = dti->frm_idx[i];
            if (frm_idx != -1) {
                if (vtss_state->afi.frm_tbl[frm_idx].entry_type) {
                    VTSS_RC(jr2_afi_delay_setup(vtss_state, frm_idx));
                } else {
                    VTSS_RC(jr2_afi_frm_setup(vtss_state, frm_idx));
                }
            }
        }
    }

    if (do_dti_config) {
        u32              dti_mode_val, dti_mode_mask;
        jr2_afi_qu_ref_t qu_ref;

        jr2_afi_port_prio_2_qu_ref(vtss_state, dti->port_no, dti->prio, &qu_ref);

        dti_mode_val =
            VTSS_F_AFI_DTI_TBL_DTI_MODE_MODE(dti->mode) |
            VTSS_F_AFI_DTI_TBL_DTI_MODE_TRAILING_DELAY_SEQ_CNT(dti->trailing_delay_seq_cnt) |
            VTSS_F_AFI_DTI_TBL_DTI_MODE_FRM_INJ_CNT(0)
#if !defined(VTSS_ARCH_JAGUAR_2_B)
            | VTSS_F_AFI_DTI_TBL_DTI_MODE_DTI_NEXT(dti->dti_next)
#endif /* !defined(VTSS_ARCH_JAGUAR_2_B) */
            ;

        dti_mode_mask = VTSS_M_AFI_DTI_TBL_DTI_MODE_MODE |
                        VTSS_M_AFI_DTI_TBL_DTI_MODE_TRAILING_DELAY_SEQ_CNT |
                        VTSS_M_AFI_DTI_TBL_DTI_MODE_FRM_INJ_CNT
#if !defined(VTSS_ARCH_JAGUAR_2_B)
                        | VTSS_M_AFI_DTI_TBL_DTI_MODE_DTI_NEXT
#endif /* !defined(VTSS_ARCH_JAGUAR_2_B) */
                        ;

        // (Re)write DTI configuration
        JR2_WRM(VTSS_AFI_DTI_TBL_DTI_MODE(dti_idx), dti_mode_val, dti_mode_mask);

#if defined(VTSS_ARCH_JAGUAR_2_B)
        JR2_WR(VTSS_AFI_DTI_TBL_DTI_PORT_QU(dti_idx),
               VTSS_F_AFI_DTI_TBL_DTI_PORT_QU_SE_INP(qu_ref.se_inp) |
               VTSS_F_AFI_DTI_TBL_DTI_PORT_QU_SE_IDX(qu_ref.se_idx) |
               VTSS_F_AFI_DTI_TBL_DTI_PORT_QU_PORT_NUM(qu_ref.chip_port));
#else
        JR2_WR(VTSS_AFI_DTI_TBL_DTI_PORT_QU(dti_idx),
               VTSS_F_AFI_DTI_TBL_DTI_PORT_QU_QU_NUM(qu_ref.qu_num) |
               VTSS_F_AFI_DTI_TBL_DTI_PORT_QU_PORT_NUM(qu_ref.chip_port));
#endif /* (!)defined(VTSS_ARCH_JAGUAR_2_B) */
    }

    // Configuration settings performed regardless of do_dti_config
    JR2_WRM(VTSS_AFI_DTI_TBL_DTI_FRM(dti_idx),
            VTSS_F_AFI_DTI_TBL_DTI_FRM_NEXT_FRM_PTR(dti->frm_idx[0]),
            VTSS_M_AFI_DTI_TBL_DTI_FRM_NEXT_FRM_PTR);
    if (dti->mode == 0 || dti->mode == 2) {
        frm_inj_cnt = dti->frm_inj_cnt;
    } else {
        frm_inj_cnt = 0;
    }

    JR2_WRM(VTSS_AFI_DTI_TBL_DTI_CNT(dti_idx),
            VTSS_F_AFI_DTI_TBL_DTI_CNT_CNT(frm_inj_cnt),
            VTSS_M_AFI_DTI_TBL_DTI_CNT_CNT);
    JR2_WR(VTSS_AFI_DTI_MISC_DTI_CNT_DOWN(dti_idx), 0);

    // TOE/2014-09-29: ADD THIS FOR JAGUAR2C:
    if (vtss_state->misc.chip_id.revision >= 2) {
        JR2_AFI_E_RETURN(VTSS_RC_ERROR, "Below line must be enabled for Jaguar2c!");
        // JR2_WR(VTSS_AFI_DTI_TBL_DTI_DURATION(dti_idx), 0);
    }

    // Start!
    JR2_WR(VTSS_AFI_DTI_MISC_DTI_CTRL(dti_idx),
           VTSS_F_AFI_DTI_MISC_DTI_CTRL_ENA(1) |
           VTSS_F_AFI_DTI_MISC_DTI_CTRL_BW(dti->bw));

    dti->started = 1;

    return VTSS_RC_OK;
}

/*
 * jr2_afi_dti_stop()
 */
static vtss_rc jr2_afi_dti_stop(vtss_state_t *const vtss_state, u32 dti_idx)
{
    vtss_afi_dti_t *dti = &vtss_state->afi.dti_tbl[dti_idx];

    if (!dti->started) {
        JR2_AFI_E_RETURN(VTSS_RC_ERROR, "DTI not started");
    }

    // Stop!
    JR2_WRM(VTSS_AFI_DTI_MISC_DTI_CTRL(dti_idx),
            VTSS_F_AFI_DTI_MISC_DTI_CTRL_ENA(0),
            VTSS_M_AFI_DTI_MISC_DTI_CTRL_ENA);

    dti->started = 0;

    return VTSS_RC_OK;
}

/*
 * jr2_afi_dti_frm_hijack()
 */
static vtss_rc jr2_afi_dti_frm_hijack(vtss_state_t *const vtss_state, u32 dti_idx, u32 frm_size)
{
    vtss_afi_dti_t *dti    = &vtss_state->afi.dti_tbl[dti_idx];
    i32            frm_idx = dti->frm_idx[dti->frm_cnt * 2];
    vtss_afi_frm_t *frm;

    VTSS_RC(afi_frm_idx_chk(vtss_state, frm_idx));

    if (frm_idx == 0) {
        // Don't use frm_idx == 0 for DTI (cannot link to it with NEXT_PTR)
        JR2_AFI_E_RETURN(VTSS_RC_ERROR, "frm_idx == %d", frm_idx);
    }

    frm = &vtss_state->afi.frm_tbl[frm_idx];

    VTSS_RC(jr2_afi_frm_hijack(vtss_state, frm_idx));
    frm->frm_delay.frm.frm_size = frm_size;

    if (dti->frm_cnt++ == 0) {
        // Setup FIRST_FRM_PTR
        JR2_WRM(VTSS_AFI_DTI_TBL_DTI_FRM(dti_idx),
                VTSS_F_AFI_DTI_TBL_DTI_FRM_FIRST_FRM_PTR(frm_idx),
                VTSS_M_AFI_DTI_TBL_DTI_FRM_FIRST_FRM_PTR);
    }

    return VTSS_RC_OK;
}

/*
 * jr2_afi_dti_frm_rm_inj()
 *
 * TOETBD: What if port has been stopped? (and not ServalT)
 */
static vtss_rc jr2_afi_dti_frm_rm_inj(vtss_state_t *const vtss_state, u32 dti_idx)
{
    vtss_afi_dti_t *dti = &vtss_state->afi.dti_tbl[dti_idx];
    u32             i;
    i32             frm_idx;
    u32             max_poll_cnt = 10;
    BOOL            frm_gone;

    // Disable the DTI by setting DTI_CTRL.ENA = 0
    if (dti->started) {
        JR2_AFI_E_RETURN(VTSS_RC_ERROR, "Injection must be stopped before rm injection");
    }

    // Set the FRM_RM bit for each frame to be removed in the Frame Table.
    for (i = 0; i < dti->frm_cnt; i++) {
        frm_idx = dti->frm_idx[i * 2];
        VTSS_RC(jr2_afi_set_frm_rm(vtss_state, frm_idx));
    }

    // Set DTI_FRM.NEXT_FRM_PTR to DTI_FRM.FIRST_FRM_PTR
    JR2_WRM(VTSS_AFI_DTI_TBL_DTI_FRM(dti_idx),
            VTSS_F_AFI_DTI_TBL_DTI_FRM_NEXT_FRM_PTR(dti->frm_idx[0]),
            VTSS_M_AFI_DTI_TBL_DTI_FRM_NEXT_FRM_PTR);

    // Set DTI_MODE.MODE=0
    // Set DTI_MODE.FRM_INJ_CNT to 0.
    if (dti->mode != 0) {
        JR2_WRM(VTSS_AFI_DTI_TBL_DTI_MODE(dti_idx),
                VTSS_F_AFI_DTI_TBL_DTI_MODE_MODE(0) |
                VTSS_F_AFI_DTI_TBL_DTI_MODE_FRM_INJ_CNT(0),
                VTSS_M_AFI_DTI_TBL_DTI_MODE_MODE |
                VTSS_M_AFI_DTI_TBL_DTI_MODE_FRM_INJ_CNT);
    }

    // Set DTI_CNT.CNT=1
    JR2_WRM(VTSS_AFI_DTI_TBL_DTI_CNT(dti_idx),
            VTSS_F_AFI_DTI_TBL_DTI_CNT_CNT(1),
            VTSS_M_AFI_DTI_TBL_DTI_CNT_CNT);


    // Set all delays in sequence to 0 to speed up the removal procedure
    for (i = 0; i < dti->frm_cnt; i++) {
        frm_idx = dti->frm_idx[i * 2 + 1];
        VTSS_RC(afi_frm_idx_chk(vtss_state, frm_idx));

        JR2_WR(VTSS_AFI_FRM_TBL_FRM_ENTRY_PART0(frm_idx), 0);
    }

    JR2_WR(VTSS_AFI_DTI_MISC_DTI_CNT_DOWN(dti_idx), 0);

    // Start!
    JR2_WR(VTSS_AFI_DTI_MISC_DTI_CTRL(dti_idx),
           VTSS_F_AFI_DTI_MISC_DTI_CTRL_ENA(1) |
           VTSS_F_AFI_DTI_MISC_DTI_CTRL_BW(0));

    // Poll for FRM_GONE=1 for last frame
    frm_idx = dti->frm_idx[(dti->frm_cnt - 1) * 2];
    while (max_poll_cnt-- > 0) {
        VTSS_RC(jr2_afi_get_frm_gone(vtss_state, &frm_gone, frm_idx));
        if (frm_gone) {
            break;
        }
    }

    if (!frm_gone) {
        JR2_AFI_E_RETURN(VTSS_RC_ERROR, "Timeout waiting for FRM_GONE==1");
    }

    VTSS_I("max_poll_cnt left = %u", max_poll_cnt);

    return VTSS_RC_OK;
}

/*
 * jr2_afi_dti_cnt_get()
 */
static vtss_rc jr2_afi_dti_cnt_get(vtss_state_t *const vtss_state, u32 dti_idx, u32 *const cnt)
{
    JR2_RD(VTSS_AFI_DTI_TBL_DTI_CNT(dti_idx), cnt);

    return VTSS_RC_OK;
}

/******************************************************************************/
//
// External CIL: TTI functions
//
/******************************************************************************/

/*
 * jr2_afi_ttis_enable()
 */
static vtss_rc jr2_afi_ttis_enable(vtss_state_t *const vtss_state)
{
    JR2_WRM(VTSS_AFI_TTI_MISC_TTI_CTRL,
            VTSS_F_AFI_TTI_MISC_TTI_CTRL_TTI_ENA(1),
            VTSS_M_AFI_TTI_MISC_TTI_CTRL_TTI_ENA);

    return VTSS_RC_OK;
}

/*
 * jr2_afi_tti_start()
 *
 * do_config: (Re)configure TTI before (re)starting
 */
static vtss_rc jr2_afi_tti_start(vtss_state_t *const vtss_state,  u32 tti_idx, BOOL do_config)
{
    vtss_afi_tti_t *tti = &vtss_state->afi.tti_tbl[tti_idx];
    u32            rand_tick_cnt;

    if (tti->started) {
        JR2_AFI_E_RETURN(VTSS_RC_ERROR, "TTI already started");
    }

    if (do_config) {
        jr2_afi_qu_ref_t qu_ref;

        jr2_afi_port_prio_2_qu_ref(vtss_state, tti->port_no, tti->prio, &qu_ref);

        // (Re)write configuration
#if defined(VTSS_ARCH_JAGUAR_2_B)
        JR2_WR(VTSS_AFI_TTI_TBL_TTI_PORT_QU(tti_idx),
               VTSS_F_AFI_TTI_TBL_TTI_PORT_QU_SE_INP(qu_ref.se_inp) |
               VTSS_F_AFI_TTI_TBL_TTI_PORT_QU_SE_IDX(qu_ref.se_idx) |
               VTSS_F_AFI_TTI_TBL_TTI_PORT_QU_PORT_NUM(qu_ref.chip_port));
#else
        JR2_WR(VTSS_AFI_TTI_TBL_TTI_PORT_QU(tti_idx),
               VTSS_F_AFI_TTI_TBL_TTI_PORT_QU_QU_NUM(qu_ref.qu_num) |
               VTSS_F_AFI_TTI_TBL_TTI_PORT_QU_PORT_NUM(qu_ref.chip_port));
#endif /* (!)defined(VTSS_ARCH_JAGUAR_2_B) */

        JR2_WRM(VTSS_AFI_TTI_TBL_TTI_TIMER(tti_idx),
                VTSS_F_AFI_TTI_TBL_TTI_TIMER_TICK_IDX(tti->tick_idx) |
                VTSS_F_AFI_TTI_TBL_TTI_TIMER_JITTER(tti->jitter),
                VTSS_M_AFI_TTI_TBL_TTI_TIMER_TICK_IDX |
                VTSS_M_AFI_TTI_TBL_TTI_TIMER_JITTER);

#if !defined(VTSS_ARCH_JAGUAR_2_B)
        JR2_WRM(VTSS_AFI_TTI_TBL_TTI_TIMER(tti_idx),
                VTSS_F_AFI_TTI_TBL_TTI_TIMER_TIMER_LEN(tti->timer_len),
                VTSS_M_AFI_TTI_TBL_TTI_TIMER_TIMER_LEN);
#endif /* (!)defined(VTSS_ARCH_JAGUAR_2_B) */

        JR2_WR(VTSS_AFI_TTI_TBL_TTI_FRM(tti_idx),
               VTSS_F_AFI_TTI_TBL_TTI_FRM_FRM_PTR(tti->frm_idx));
    }

    // Set TICK_CNT to a random value in range [1-TIMER_LEN]
    rand_tick_cnt = 1 + rand() % tti->timer_len;
    JR2_WRM(VTSS_AFI_TTI_TBL_TTI_TICKS(tti_idx),
            VTSS_F_AFI_TTI_TBL_TTI_TICKS_TICK_CNT(rand_tick_cnt),
            VTSS_M_AFI_TTI_TBL_TTI_TICKS_TICK_CNT);

    // Start TTI
#if defined(VTSS_ARCH_JAGUAR_2_B)
    // JR2B: Start timer by setting timer_len to non-zero value
    JR2_WRM(VTSS_AFI_TTI_TBL_TTI_TIMER(tti_idx),
            VTSS_F_AFI_TTI_TBL_TTI_TIMER_TIMER_LEN(tti->timer_len),
            VTSS_M_AFI_TTI_TBL_TTI_TIMER_TIMER_LEN);
#else
    // Start timer using timer_ena
    JR2_WRM(VTSS_AFI_TTI_TBL_TTI_TIMER(tti_idx),
            VTSS_F_AFI_TTI_TBL_TTI_TIMER_TIMER_ENA(1),
            VTSS_M_AFI_TTI_TBL_TTI_TIMER_TIMER_ENA);
#endif /* (!)defined(VTSS_ARCH_JAGUAR_2_B) */

    tti->started = 1;

    return VTSS_RC_OK;
}

/*
 * jr2_afi_tti_stop()
 */
static vtss_rc jr2_afi_tti_stop(vtss_state_t *const vtss_state, u32 tti_idx)
{
    vtss_afi_tti_t *tti = &vtss_state->afi.tti_tbl[tti_idx];

    if (!tti->started) {
        JR2_AFI_E_RETURN(VTSS_RC_ERROR, "DTI not started");
    }

#if defined(VTSS_ARCH_JAGUAR_2_B)
    // Jaguar2: Stop timer by setting timer_len to zero
    JR2_WRM(VTSS_AFI_TTI_TBL_TTI_TIMER(tti_idx),
            VTSS_F_AFI_TTI_TBL_TTI_TIMER_TIMER_LEN(0),
            VTSS_M_AFI_TTI_TBL_TTI_TIMER_TIMER_LEN);
#else
    // Stop timer using timer_ena
    JR2_WRM(VTSS_AFI_TTI_TBL_TTI_TIMER(tti_idx),
            VTSS_F_AFI_TTI_TBL_TTI_TIMER_TIMER_ENA(0),
            VTSS_M_AFI_TTI_TBL_TTI_TIMER_TIMER_ENA);
#endif /* (!)defined(VTSS_ARCH_JAGUAR_2_B) */

    tti->started = 0;

    return VTSS_RC_OK;
}

/*
 * jr2_afi_tti_frm_hijack()
 */
static vtss_rc jr2_afi_tti_frm_hijack(vtss_state_t *const vtss_state, u32 tti_idx)
{
    return jr2_afi_frm_hijack(vtss_state, vtss_state->afi.tti_tbl[tti_idx].frm_idx);
}

/*
 * jr2_afi_tti_frm_rm_inj()
 *
 * TOETBD: What if port has been stopped (and not ServalT)?
 */
static vtss_rc jr2_afi_tti_frm_rm_inj(vtss_state_t *const vtss_state, u32 tti_idx)
{
    vtss_afi_tti_t *tti = &vtss_state->afi.tti_tbl[tti_idx];
    BOOL           frm_gone;

    // Assumption:
    // Each poll takes at least 50 clk cycles.
    // One TTI is processed every 4 clk cycles.
    u32            max_poll_cnt = (VTSS_AFI_SLOW_INJ_CNT * 4) / 50;

    if (tti->started) {
        JR2_AFI_E_RETURN(VTSS_RC_ERROR, "Injection must be stopped before rm injection");
    }

    VTSS_RC(jr2_afi_set_frm_rm(vtss_state, tti->frm_idx));

    // Set TIMER_LEN to max value (=> inject ASAP)
    JR2_WRM(VTSS_AFI_TTI_TBL_TTI_TIMER(tti_idx),
            VTSS_F_AFI_TTI_TBL_TTI_TIMER_TIMER_LEN((1 << VTSS_AFI_TTI_TBL_TIMER_LEN_WID) - 1),
            VTSS_M_AFI_TTI_TBL_TTI_TIMER_TIMER_LEN);

#if !defined(VTSS_ARCH_JAGUAR_2_B)
    // Make sure timer is started
    JR2_WRM(VTSS_AFI_TTI_TBL_TTI_TIMER(tti_idx),
            VTSS_F_AFI_TTI_TBL_TTI_TIMER_TIMER_ENA(1),
            VTSS_M_AFI_TTI_TBL_TTI_TIMER_TIMER_ENA);
#endif /* !defined(VTSS_ARCH_JAGUAR_2_B) */

    // Poll for FRM_GONE=1
    while (max_poll_cnt-- > 0) {
        VTSS_RC(jr2_afi_get_frm_gone(vtss_state, &frm_gone, tti->frm_idx));
        if (frm_gone) {
            break;
        }
    }

    if (!frm_gone) {
        JR2_AFI_E_RETURN(VTSS_RC_ERROR, "Timeout waiting for FRM_GONE==1");
    }

    return VTSS_RC_OK;
}

/******************************************************************************/
//
// External CIL: Other functions
//
/******************************************************************************/

/*
 * jr2_afi_pause_resume()
 *
 * Link up/down
 * TOETBD: It seems this function is called with resume = TRUE during startup, despite port unused ... ??
 */
vtss_rc jr2_afi_pause_resume(vtss_state_t *const vtss_state, vtss_port_no_t port_no, BOOL resume)
{
    VTSS_D("Enter. port = %d, resume = %d", port_no, resume);
    if (resume) {
        VTSS_RC(jr2_afi_port_link_up(vtss_state, port_no));
    } else {
        vtss_afi_port_t *port = jr2_afi_port_tbl_entry(vtss_state, port_no);

        if (port->link) {
            VTSS_RC(jr2_afi_port_link_down(vtss_state, port_no));
        }
    }

    return VTSS_RC_OK;
}

/*
 * jr2_afi_enable()
 */
static vtss_rc jr2_afi_enable(vtss_state_t *const vtss_state)
{
#if !defined(VTSS_ARCH_JAGUAR_2_B)
    // Enable AFI first (power saving feature added on ServalT)
    JR2_WRM(VTSS_AFI_MISC_MISC_CTRL,
            VTSS_F_AFI_MISC_MISC_CTRL_AFI_ENA(1),
            VTSS_M_AFI_MISC_MISC_CTRL_AFI_ENA);
#endif /* !defined(VTSS_ARCH_JAGUAR_2_B) */

    VTSS_RC(jr2_afi_tti_cal_init(vtss_state));

    return VTSS_RC_OK;
}

/*
 * jr2_afi_port_admin_start()
 *
 * Administrative port start
 * Upon init, the ports are in started state.
 */
static vtss_rc jr2_afi_port_admin_start(vtss_state_t *const vtss_state, vtss_port_no_t port_no)
{
    vtss_afi_port_t *port = jr2_afi_port_tbl_entry(vtss_state, port_no);

    VTSS_D("Enter. port = %d, link = %d, started = %d", port_no, port->link, port->started);

    if (port->started) {
        JR2_AFI_E_RETURN(VTSS_RC_ERROR, "Port %u already started", port_no);
    }

    if (port->link) {
        VTSS_RC(jr2_afi_port_start(vtss_state, port_no));
    }

    port->started = 1;

    return VTSS_RC_OK;
}

/*
 * jr2_afi_port_admin_stop()
 *
 * Administrative port stop
 * Upon init, the ports are in started state.
 */
static vtss_rc jr2_afi_port_admin_stop(vtss_state_t *const vtss_state, vtss_port_no_t port_no)
{
    vtss_afi_port_t *port = jr2_afi_port_tbl_entry(vtss_state, port_no);

    VTSS_D("Enter. port = %d, link = %d, started = %d", port_no, port->link, port->started);

    if (!port->started) {
        JR2_AFI_E_RETURN(VTSS_RC_ERROR, "Port %u not started", port_no);
    }

    VTSS_RC(jr2_afi_port_stop(vtss_state, port_no));
    port->started = 0;

    return VTSS_RC_OK;
}

/*
 * vtss_jr2_afi_debug_print()
 */
vtss_rc vtss_jr2_afi_debug_print(vtss_state_t *vtss_state, const vtss_debug_printf_t pr, const vtss_debug_info_t *const info)
{
    return vtss_debug_print_group(VTSS_DEBUG_GROUP_AFI, jr2_afi_debug, vtss_state, pr, info);
}

/*
 * vtss_jr2_afi_init()
 */
vtss_rc vtss_jr2_afi_init(vtss_state_t *const vtss_state, const vtss_init_cmd_t cmd)
{
    vtss_afi_state_t    *state = &vtss_state->afi;
    vtss_port_no_t      port_no;
    vtss_phys_port_no_t chip_port;

    switch (cmd) {
    case VTSS_INIT_CMD_CREATE:
        // Assign CIL function pointers
        state->afi_enable       = jr2_afi_enable;
        state->ttis_enable      = jr2_afi_ttis_enable;

        state->tti_start        = jr2_afi_tti_start;
        state->tti_stop         = jr2_afi_tti_stop;
        state->tti_frm_hijack   = jr2_afi_tti_frm_hijack;
        state->tti_frm_rm_inj   = jr2_afi_tti_frm_rm_inj;

        state->dti_start        = jr2_afi_dti_start;
        state->dti_stop         = jr2_afi_dti_stop;
        state->dti_frm_hijack   = jr2_afi_dti_frm_hijack;
        state->dti_frm_rm_inj   = jr2_afi_dti_frm_rm_inj;
        state->dti_cnt_get      = jr2_afi_dti_cnt_get;

        state->port_admin_start = jr2_afi_port_admin_start;
        state->port_admin_stop  = jr2_afi_port_admin_stop;

        // Initialize ports to started = 1. This corresponds to
        // calling jr2_afi_port_admin_start() during boot.
        for (port_no = 0; port_no < VTSS_ARRSZ(vtss_state->afi.port_tbl); port_no++) {
            vtss_state->afi.port_tbl[port_no].started = 1;
        }
        break;

    case VTSS_INIT_CMD_INIT:
        // The switch core clock period differs from JR2 chip-variant to chip-variant,
        // and we rely heavily on it in this module, so get it.
        VTSS_RC(jr2_afi_clock_period_get(vtss_state));

        // Initialize the tick table
        VTSS_RC(jr2_afi_tti_tick_init(vtss_state));

        // Stop injection on all ports, corresponding to link == 0.
        // Will be started upon link up.
        for (chip_port = 0; chip_port < VTSS_CHIP_PORTS_ALL; chip_port++) {
            VTSS_RC(jr2_afi_chip_port_stop(vtss_state, chip_port));
        }

        break;

    case VTSS_INIT_CMD_PORT_MAP:
        // Now that the port map is set, we can use "port_no".
        // VD1 always has "link up". It is indexed with port_no == VTSS_PORT_NO_NONE
        VTSS_RC(jr2_afi_port_link_up(vtss_state, VTSS_PORT_NO_NONE));
        break;

    case VTSS_INIT_CMD_POLL:
        break;

    default:
        break;
    }

    return VTSS_RC_OK;
}

#endif /* defined(VTSS_AFI_V2) && defined(VTSS_FEATURE_AFI_SWC) && defined(VTSS_ARCH_JAGUAR_2) */

