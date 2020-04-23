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

#define VTSS_TRACE_GROUP VTSS_TRACE_GROUP_AFI
#include "vtss_api.h"
#include "vtss_state.h"
#include "vtss_common.h"
#include "vtss_util.h"

#if defined(VTSS_FEATURE_AFI_SWC) && defined(VTSS_AFI_V2)

/******************************************************************************/
//
// Internal AIL: Generic resource control
//
/******************************************************************************/

/*
 * afi_res_is_free()
 *
 * Returns true if specific resource is free, FALSE otherwise.
 */
static BOOL afi_res_is_free(u32 *const alloc_table, u32 res_idx)
{
    u32 word_idx = res_idx / 32;
    u8  bit_idx  = res_idx - word_idx * 32;

    if ((alloc_table[word_idx] & (1 << bit_idx)) == 0) {
        // Not allocated!
        return TRUE;
    } else {
        return FALSE;
    }
}

/*
 * afi_res_alloc()
 *
 * Generic function for FRM/DTI/TTI allocation
 */
static vtss_rc afi_res_alloc(vtss_state_t *const vtss_state,
                             u32          *const alloc_table,
                             u32                 res_cnt,
                             u32          *const alloced_res_idx, // res_idx of allocated resource
                             u32                 min_res_idx,     // Minimum res_idx
                             u32                 max_res_idx,     // Maximum res_idx
                             BOOL                rand_mode)       // Randomize resource allocation
{
    u32 res_idx, start_res_idx;

    if (!(min_res_idx < res_cnt && max_res_idx < res_cnt)) {
        VTSS_E("Unexpected min/max: %u/%u, res_cnt=%u", min_res_idx, max_res_idx, res_cnt);
        return VTSS_RC_ERROR;
    }

    if (rand_mode) {
        /*
         * Randomize resource allocation.
         * This is intended for TTI allocation, where spreading the allocated TTIs
         * throughout the TTI table will help reduce burstiness for many real-life
         * configurations.
         */
        start_res_idx = min_res_idx + VTSS_OS_RAND() % (max_res_idx - min_res_idx + 1);
    } else {
        start_res_idx = min_res_idx;
    }

    res_idx = start_res_idx;

    do {
        if (afi_res_is_free(alloc_table, res_idx)) {
            u32 word_idx = res_idx / 32;
            u8  bit_idx  = res_idx - word_idx * 32;

            alloc_table[word_idx] |= (1 << bit_idx);
            *alloced_res_idx = res_idx;
            return VTSS_RC_OK;
        }

        res_idx = (res_idx + 1);
        if (res_idx > max_res_idx) {
            res_idx = min_res_idx;
        }
    } while (res_idx != start_res_idx);

    return VTSS_RC_ERROR; // Out of resources
}

/*
 * afi_res_free()
 *
 * Generic function for FRM/DTI/TTI freeing
 */
static vtss_rc afi_res_free(vtss_state_t *const vtss_state,
                            u32          *const alloc_table,
                            u32                 res_idx)
{
    u32     word_idx = res_idx / 32;
    u8      bit_idx = res_idx - word_idx * 32;
    vtss_rc rc = VTSS_RC_OK;

    if (afi_res_is_free(alloc_table, res_idx)) {
        // Not alloced!
        VTSS_E("res_idx=%u not alloced", res_idx);
        rc = VTSS_RC_ERROR;
    }

    alloc_table[word_idx] &= ~(u32)(1 << bit_idx);

    return rc;
}

/******************************************************************************/
//
// Internal AIL: DTI resource control
//
/******************************************************************************/

/*
 * afi_dti_init()
 */
static void afi_dti_init(vtss_afi_dti_t *dti)
{
    u32 i;

    memset(dti, 0, sizeof(vtss_afi_dti_t));
    for (i = 0; i < VTSS_AFI_DTI_MAX_SEQ_LEN; i++) {
        // frm_idx == -1 <=> No FRM allocated.
        dti->frm_idx[i] = -1;
    }
}

/*
 * afi_dti_alloc()
 */
static vtss_rc afi_dti_alloc(vtss_state_t *const vtss_state, u32 *const dti_idx)
{
    vtss_rc rc;

    if ((rc = afi_res_alloc(vtss_state, vtss_state->afi.dtis_alloced, VTSS_AFI_FAST_INJ_CNT, dti_idx /* min_res_idx+max_res_idx (full range allowed) */, 0, VTSS_AFI_FAST_INJ_CNT - 1, FALSE)) != VTSS_RC_OK) {
        VTSS_E("Out of DTIs");
        return rc;
    }

    afi_dti_init(&vtss_state->afi.dti_tbl[*dti_idx]);

    return VTSS_RC_OK;
}

/*
 * afi_dti_free()
 */
static vtss_rc afi_dti_free(vtss_state_t *const vtss_state, u32 dti_idx)
{
    if (dti_idx >= VTSS_AFI_FAST_INJ_CNT) {
        VTSS_E("dti_idx=%u > %u", dti_idx, VTSS_AFI_FAST_INJ_CNT);
        return VTSS_RC_ERROR;
    }

    return afi_res_free(vtss_state, vtss_state->afi.dtis_alloced, dti_idx);
}

/******************************************************************************/
//
// Internal AIL: TTI resource control
//
/******************************************************************************/

/*
 * afi_tti_init()
 */
static void afi_tti_init(vtss_afi_tti_t *tti)
{
    memset(tti, 0, sizeof(vtss_afi_tti_t));
    tti->frm_idx = -1; // frm_idx == -1 <=> No FRM allocated.
}

/*
 * afi_tti_alloc()
 */
static vtss_rc afi_tti_alloc(vtss_state_t *const vtss_state,
                             u32          *const tti_idx,
                             u32                 min_tti_idx,
                             u32                 max_tti_idx)
{
    vtss_rc rc;

    if ((rc = afi_res_alloc(vtss_state, vtss_state->afi.ttis_alloced, VTSS_AFI_SLOW_INJ_CNT, tti_idx, min_tti_idx, max_tti_idx, TRUE)) != VTSS_RC_OK) {
        VTSS_E("Out of TTIs");
        return rc;
    }

    afi_tti_init(&vtss_state->afi.tti_tbl[*tti_idx]);

    return VTSS_RC_OK;
}

/*
 * afi_tti_free()
 */
static vtss_rc afi_tti_free(vtss_state_t *const vtss_state, const u32 tti_idx)
{
    if (tti_idx >= VTSS_AFI_SLOW_INJ_CNT) {
        VTSS_E("tti_idx=%u > %u", tti_idx, VTSS_AFI_SLOW_INJ_CNT);
        return VTSS_RC_ERROR;
    }

    return afi_res_free(vtss_state, vtss_state->afi.ttis_alloced, tti_idx);
}

/******************************************************************************/
//
// FRM resource control
//
/******************************************************************************/

/*
 * afi_frm_init()
 */
static void afi_frm_init(vtss_afi_frm_t *frm)
{
    memset(frm, 0, sizeof(*frm));
}

/*
 * afi_frm_alloc()
 */
static vtss_rc afi_frm_alloc(vtss_state_t *const vtss_state, i32 *const frm_idx, i32 min_frm_idx)
{
    vtss_rc rc;

    if ((rc = afi_res_alloc(vtss_state, vtss_state->afi.frms_alloced, VTSS_AFI_FRM_CNT, (u32 *)frm_idx, min_frm_idx, VTSS_AFI_FRM_CNT - 1, FALSE)) != VTSS_RC_OK) {
        VTSS_E("Out of FRMs");
        return rc;
    }

    afi_frm_init(&vtss_state->afi.frm_tbl[*frm_idx]);

    return VTSS_RC_OK;
}

/*
 * afi_frm_free()
 */
static vtss_rc afi_frm_free(vtss_state_t *const vtss_state, i32 frm_idx)
{
    if (frm_idx >= VTSS_AFI_FRM_CNT) {
        VTSS_E("frm_idx=%u > %u", frm_idx, VTSS_AFI_FRM_CNT);
        return VTSS_RC_ERROR;
    }

    return afi_res_free(vtss_state, vtss_state->afi.frms_alloced, frm_idx);
}

/******************************************************************************/
//
// Internal AIL: Argument checking
//
/******************************************************************************/

/*
 * afi_frm_idx_chk()
 */
vtss_rc afi_frm_idx_chk(struct vtss_state_s *const vtss_state, i32 frm_idx)
{
    if (frm_idx < 0 || frm_idx >= VTSS_AFI_FRM_CNT) {
        VTSS_E("frm_idx == %d illegal", frm_idx);
        return VTSS_RC_ERROR;
    }

    if (afi_res_is_free(vtss_state->afi.frms_alloced, frm_idx)) {
        VTSS_E("frm_idx == %d not alloced", frm_idx);
        return VTSS_RC_ERROR;
    }

    return VTSS_RC_OK;
}

/*
 * afi_dti_idx_chk()
 */
vtss_rc afi_dti_idx_chk(struct vtss_state_s *const vtss_state, u32 dti_idx)
{
    if (dti_idx >= VTSS_AFI_FAST_INJ_CNT) {
        VTSS_E("dti_idx == %u illegal", dti_idx);
        return VTSS_RC_ERROR;
    }

    if (afi_res_is_free(vtss_state->afi.dtis_alloced, dti_idx)) {
        VTSS_E("dti_idx == %u not alloced", dti_idx);
        return VTSS_RC_ERROR;
    }

    return VTSS_RC_OK;
}

/*
 * afi_tti_idx_chk()
 */
vtss_rc afi_tti_idx_chk(struct vtss_state_s *const vtss_state, u32 tti_idx)
{
    if (tti_idx >= VTSS_AFI_SLOW_INJ_CNT) {
        VTSS_E("tti_idx == %u illegal", tti_idx)
        return VTSS_RC_ERROR;
    }

    if (afi_res_is_free(vtss_state->afi.ttis_alloced, tti_idx)) {
        VTSS_E("tti_idx == %u not alloced", tti_idx);
        return VTSS_RC_ERROR;
    }

    return VTSS_RC_OK;
}

/******************************************************************************/
//
// Internal AIL: Other functions
//
/******************************************************************************/

/*
 * afi_tti_start_cfg_cmp()
 *
 * Compare TTI start_cfgs. 0 => Identical
 */
static int afi_tti_start_cfg_cmp(const vtss_afi_slow_inj_start_cfg_t *const cfg1, const vtss_afi_slow_inj_start_cfg_t *const cfg2)
{
    return memcmp(cfg1, cfg2, sizeof(vtss_afi_slow_inj_start_cfg_t));
}

/*
 * afi_tti_start_cfg_cp()
 *
 * Copy cfg2 to cfg1
 */
static void afi_tti_start_cfg_cp(vtss_afi_slow_inj_start_cfg_t *const cfg1, const vtss_afi_slow_inj_start_cfg_t *const cfg2)
{
    memcpy(cfg1, cfg2, sizeof(vtss_afi_slow_inj_start_cfg_t));
}

/*
 * afi_div_round32()
 */
static u32 afi_div_round32(u32 dividend, u32 divisor)
{
    return (dividend * 1000 / divisor + 500) / 1000;
}

/*
 * afi_timer_prec_ok()
 *
 * Check that actual timer_len is with +/- prec_pct of requested value
 */
static BOOL afi_timer_prec_ok(u32 timer_len_us_requested, u32 timer_len_us_actual, u32 prec_pct)
{
    if (timer_len_us_actual * 1000 >= timer_len_us_requested * (1000 - prec_pct * 10) &&
        timer_len_us_actual * 1000 <= timer_len_us_requested * (1000 + prec_pct * 10)) {
        return TRUE;
    }

    return FALSE;
}

/*
 * afi_max64()
 */
static u64 afi_max64(u64 a, u64 b)
{
    return (a > b) ? a : b;
}

/*
 * afi_dti_link_trailing_delay()
 *
 * Link trailing delay into Frame-Delay sequence of DTI
 */
static vtss_rc afi_dti_link_trailing_delay(vtss_state_t *const vtss_state, u32 dti_idx)
{
    vtss_afi_dti_t *dti = &vtss_state->afi.dti_tbl[dti_idx];
    i32            frm_idx, frm_idx_next;

    VTSS_RC(afi_frm_idx_chk(vtss_state, frm_idx      = dti->frm_idx[dti->frm_cnt * 2 - 1]));
    VTSS_RC(afi_frm_idx_chk(vtss_state, frm_idx_next = dti->frm_idx[dti->frm_cnt * 2 - 0]));

    vtss_state->afi.frm_tbl[frm_idx].next_ptr = frm_idx_next;

    return VTSS_RC_OK;
}

/******************************************************************************/
//
// Internal AIL: DTI bandwidth calculation
//
/******************************************************************************/

/*
 * afi_dti_delay_calc_single_frm()
 *
 * Single frame sequence: Update Frame-Delay sequence for DTI to match requested cfg
 * INJ_CNT in FRM_TBL is used to fine-tune BW
 */
static vtss_rc afi_dti_delay_calc_single_frm(vtss_state_t *const vtss_state, u32 dti_idx, const vtss_afi_fast_inj_start_cfg_t *const cfg)
{
    vtss_afi_dti_t  *dti = &vtss_state->afi.dti_tbl[dti_idx];
    u32             frm_size, delay;
    u64             delay_xm = 0; // Delay, multiplied by 1e6
    i32             carry_xm = 0; // Carry delay, multiplied by 1e6
    frm_tbl_frm_t   *frm_tbl_frm;
    frm_tbl_delay_t *frm_tbl_delay, *frm_tbl_delay_last;

    // Algorithm
    // ---------
    // 1) Assign delay to frame, proportional to frame size, and resulting in a
    //    rate equal to or higher than requested rate.
    // 2) Calculate "excess delay", i.e. the number of delay clock cycles beyond the 12 cycles
    //    it takes to process a DTI.
    // 3) If actual BW > requested BW:
    //    Fine-tune BW using INJ_CNT and an additional delay.
    //    Make sure INJ_CNT is high enough to have excess delay >=12 cycles.
    //
    // Variables named with _xm have been multiplied by 1e6 to improve accuracy.

    frm_tbl_frm        = &vtss_state->afi.frm_tbl[dti->frm_idx[0]].frm_delay.frm;
    frm_tbl_delay      = &vtss_state->afi.frm_tbl[dti->frm_idx[1]].frm_delay.delay;
    frm_tbl_delay_last = &vtss_state->afi.frm_tbl[dti->frm_idx[2]].frm_delay.delay;

    frm_size = frm_tbl_frm->frm_size;

    // Calculate delay required to achive bps
    delay_xm =
        VTSS_DIV64(
            (u64)frm_size * 8LLU *
            VTSS_DIV64((u64)1e18, cfg->bps),
            vtss_state->afi.clk_period_ps);
    // Set frame delay to truncation of delay_xm, though minimum 12
    frm_tbl_delay->delay = afi_max64(VTSS_DIV64(delay_xm, (u64)1e6), (u64)12);
    carry_xm = (u32)(delay_xm - (u64)(frm_tbl_delay->delay * 1e6));

    if (carry_xm) {
        u32 delay_best         = 0;
        u32 inj_cnt_best       = 0;
        i32 carry_xm_diff_best = (i32)1e6;
        u32 inj_cnt;

        // Loop through possible Delay values and search for closest match for carry
        // Use same max value for inj_cnt as for TRAILING_DELAY_SEQ_CNT
        for (inj_cnt = 1; inj_cnt <= VTSS_AFI_TRAILING_DELAY_SEQ_CNT_MAX; inj_cnt++) {
            for (delay = 1; delay <= inj_cnt - 1; delay++) {
                i32 carry_xm_diff = (i32)((u64)(delay * 1e6 / inj_cnt) - carry_xm);

                // Delay must be bigger than the carry (otherwise BW will be above requested rate)
                if (carry_xm_diff < 0) {
                    continue;
                }

                // Make sure there is sufficient excess delay to process
                // the additional delay entry
                if ((frm_tbl_delay->delay - 12) * inj_cnt < 12) {
                    continue;
                }

                if (carry_xm_diff < carry_xm_diff_best) {
                    // Found new best
                    carry_xm_diff_best = carry_xm_diff;
                    inj_cnt_best       = inj_cnt;
                    delay_best         = delay;
                }
            }
        }

        if (delay_best != 0) {
            frm_tbl_frm->inj_cnt      = inj_cnt_best;
            frm_tbl_delay_last->delay = delay_best;

            // Link in trailing delay.
            VTSS_RC(afi_dti_link_trailing_delay(vtss_state, dti_idx));
        }
    }

    return VTSS_RC_OK;
}

/*
 * afi_dti_delay_calc_multi_frm()
 *
 * Multi frame sequence: Update Frame-Delay sequence for DTI to match requested cfg
 * TRAILING_DELAY_SEQ_CNT is used to fine-tune BW
 */
static vtss_rc afi_dti_delay_calc_multi_frm(vtss_state_t *const vtss_state, u32 dti_idx, const vtss_afi_fast_inj_start_cfg_t *const cfg)
{
    vtss_afi_dti_t  *dti = &vtss_state->afi.dti_tbl[dti_idx];
    u32             exs_delay, delay;
    u64             delay_xm = 0; // Delay, multiplied by 1e6
    i32             carry_xm = 0; // Carry delay, multiplied by 1e6
    frm_tbl_frm_t   *frm_tbl_frm;
    frm_tbl_delay_t *frm_tbl_delay;
    u32             i;
    i32             frm_idx;

    // Algorithm
    // ---------
    // 1) Assign delay to each frame, proportional to frame size, and resulting in a
    //    rate equal to or higher than requested rate.
    // 2) Calculate "excess delay", i.e. the number of delay clock cycles beyond the 12 cycles
    //    it takes to process each pair of Frame-Delay entries.
    // 3) If actual BW > requested BW:
    //    If excess >= 12 cycles: Fine-tune BW using trailing delay.
    //    If excess <12 cycles:   Add one to the last delay to ensure actual BW gets below requested BW.
    //
    // Variables named with _xm have been multiplied by 1e6 to improve accuracy.

    carry_xm  = 0; // Carry from previous frame(s)
    exs_delay = 0; // Acculated delay beyond the minimum 12 cycles it takes to process a DTI
    for (i = 0; i < dti->frm_cnt; i++) {
        frm_tbl_frm   = &vtss_state->afi.frm_tbl[dti->frm_idx[i * 2]].frm_delay.frm;
        frm_tbl_delay = &vtss_state->afi.frm_tbl[dti->frm_idx[i * 2 + 1]].frm_delay.delay;

        // Calculate delay for frame required to achive bps
        delay_xm      =
            VTSS_DIV64(
                (u64)frm_tbl_frm->frm_size * (u64)8 *
                VTSS_DIV64((u64)1e18, cfg->bps),
                vtss_state->afi.clk_period_ps)
            +
            carry_xm;

        // Set frame delay to truncation of delay_xm, though minimum 12
        delay         = afi_max64(VTSS_DIV64(delay_xm, (u64)1e6), (u64)12);
        carry_xm      = delay_xm - (u64)(delay * 1e6);
        exs_delay    += (delay - 12);
        frm_tbl_delay->delay  = delay;
    }

    // BW fine tuning using Trailing Delay (TD)
    if (carry_xm) {
        // TD can only be used if there is "excess delay" of at least 12 cycles in base sequence
        // since a TD entry takes 12 cycles to process
        if (exs_delay >= 12) {
            // Best value for TRAILING_DELAY_SEQ_CNT + Delay
            // - and corresponding diff against carry_xm
            u32 td_seq_cnt_best    = 0;
            u32 td_delay_best      = 0;
            i32 carry_xm_diff_best = (i32)1e6;
            u32 tds;

            // Loop through possible TDS values and search for closest match for carry
            for (tds = 1; tds <= VTSS_AFI_TRAILING_DELAY_SEQ_CNT_MAX; tds++) {
                for (delay = 1; delay < tds; delay++) {
                    i32 carry_xm_diff = (i32)((u64)(delay * 1e6 / tds) - carry_xm);

                    // Delay must be bigger than the carry (otherwise BW gets above requested rate)
                    if (carry_xm_diff < 0) {
                        continue;
                    }

                    if (carry_xm_diff < carry_xm_diff_best) {
                        // Found new best
                        carry_xm_diff_best = carry_xm_diff;
                        td_seq_cnt_best    = tds;
                        td_delay_best      = delay;
                    }
                }
            }

            if (td_seq_cnt_best != 0) {
                // Found TD adjustment
                dti->trailing_delay_seq_cnt = td_seq_cnt_best;
                VTSS_RC(afi_frm_idx_chk(vtss_state, frm_idx = dti->frm_idx[dti->frm_cnt * 2]));
                vtss_state->afi.frm_tbl[frm_idx].frm_delay.delay.delay = td_delay_best;

                // Link in trailing delay
                VTSS_RC(afi_dti_link_trailing_delay(vtss_state, dti_idx));
            }
        } else {
            // Cannot use TDS. Add 1 to last delay to avoid being above requested BW
            VTSS_RC(afi_frm_idx_chk(vtss_state, frm_idx = dti->frm_idx[(dti->frm_cnt - 1) * 2 + 1]));
            vtss_state->afi.frm_tbl[frm_idx].frm_delay.delay.delay++;
        }
    } else {
        // Requested BW matches actual BW
    }

    return VTSS_RC_OK;
}

/*
 * afi_dti_bps_actual_calc()
 *
 * Calculate cfg->bps_actual for DTI's Frame-Delay configuration
 */
static vtss_rc afi_dti_bps_actual_calc(vtss_state_t *const vtss_state, u32 dti_idx, vtss_afi_fast_inj_start_cfg_t *const cfg)
{
    vtss_afi_dti_t *dti = &vtss_state->afi.dti_tbl[dti_idx];
    i32            frm_idx;
    u32            i;
    u32            frm_size_sum = 0;
    u64            delay_sum_xm = 0;
    u32            entry_cnt    = 0; // Number of FRM_TBL entries in Frame-Delay sequence
    vtss_afi_frm_t *frm_prev;

    VTSS_RC(afi_frm_idx_chk(vtss_state, frm_idx = dti->frm_idx[0]));

    i = 0;
    frm_prev = NULL;

    // Walk-through all FRM_TBL entries in Frame-Delay sequence and calculate
    // number of bytes and number of delay clock cycles in entire sequence
    while (frm_idx != 0) {
        vtss_afi_frm_t *frm = &vtss_state->afi.frm_tbl[frm_idx];
        entry_cnt++;

        if (entry_cnt > VTSS_AFI_DTI_MAX_SEQ_LEN) {
            VTSS_E("entry_cnt=%u > VTSS_AFI_DTI_MAX_SEQ_LEN", entry_cnt);
            return VTSS_RC_ERROR;
        }

        // Check that dti->frm_idx matches
        if (dti->frm_idx[i] == frm_idx) {
            i++;
        } else if (dti->frm_idx[i + 1] == frm_idx) {
            // Entry skipped
            i += 2;
        } else {
            VTSS_E("frm_idx=%d not found in dti->frm_idx[]", frm_idx);
            return VTSS_RC_ERROR;
        }

        if (frm->entry_type == 0) {
            // Frame
            frm_size_sum +=
                frm->frm_delay.frm.frm_size * frm->frm_delay.frm.inj_cnt;
        } else {
            // Delay
            if (frm_prev && frm_prev->entry_type == 0) {
                // Previous entry was a frame, so multiply delay with inj_cnt
                delay_sum_xm += (u64)(frm->frm_delay.delay.delay * (u64)frm_prev->frm_delay.frm.inj_cnt * (u64)1e6);
            } else {
                if (frm->next_ptr == 0) {
                    if (dti->trailing_delay_seq_cnt > 0) {
                        // Trailing delay
                        delay_sum_xm += VTSS_DIV64((u64)frm->frm_delay.delay.delay * (u64)1e6,
                                                   (u64)dti->trailing_delay_seq_cnt);
                    } else {
                        delay_sum_xm += (u64)frm->frm_delay.delay.delay * (u64)1e6;
                    }
                } else {
                    delay_sum_xm += (u64)frm->frm_delay.delay.delay * (u64)1e6;
                }
            }
        }

        frm_prev = frm;
        frm_idx  = frm->next_ptr;
    }

    if (frm_size_sum == 0) {
        VTSS_E("frm_size_sum=%u", frm_size_sum);
        return VTSS_RC_ERROR;
    }

    if (delay_sum_xm) {
        // Calculate bps_actual
        // To avoid overflow in 64 bit calculations different expressions and to
        // increase accuracy depending on delay_sum_xm.
        if (delay_sum_xm > 1e12) {
            cfg->bps_actual =
                VTSS_DIV64(
                    VTSS_DIV64(
                        (u64)frm_size_sum * (u64)8 *
                        VTSS_DIV64((u64)1e18, VTSS_DIV64(delay_sum_xm, (u64)1e6)),
                        vtss_state->afi.clk_period_ps) +
                    (u64)5e5,
                    (u64)1e6);
        } else {
            if (delay_sum_xm > 1e10) {
                cfg->bps_actual =
                    VTSS_DIV64(
                        (u64)frm_size_sum * (u64)8 *
                        VTSS_DIV64(
                            (u64)1e6 *
                            VTSS_DIV64((u64)1e18, delay_sum_xm),
                            vtss_state->afi.clk_period_ps) +
                        (u64)5e5,
                        (u64)1e6);
            } else {
                cfg->bps_actual =
                    VTSS_DIV64(
                        (u64)frm_size_sum * (u64)8 * (u64)1e6 *
                        VTSS_DIV64(
                            VTSS_DIV64((u64)1e18, delay_sum_xm),
                            vtss_state->afi.clk_period_ps) +
                        (u64)5e5,
                        (u64)1e6);
            }
        }
    } else {
        VTSS_E("delay_sum_xm=%" PRIu64, delay_sum_xm);
        return VTSS_RC_ERROR;
    }

    return VTSS_RC_OK;
}

/*
 * afi_dti_delay_calc()
 *
 * Update Frame-Delay sequence for DTI to match requested configuration
 */
static vtss_rc afi_dti_delay_calc(vtss_state_t *const vtss_state, u32 dti_idx, vtss_afi_fast_inj_start_cfg_t *const cfg)
{
    vtss_afi_dti_t *dti = &vtss_state->afi.dti_tbl[dti_idx];
    u32            i;
    i32            frm_idx, frm_idx_next;

    VTSS_RC(afi_dti_idx_chk(vtss_state, dti_idx));

    dti->trailing_delay_seq_cnt = 0;

    if (cfg->bps == 0) {
        // No delays, just inject as fast as possible, relying on FRM_OUT_MAX
        // to throttle bandwidth down to port's bandwidth.
        for (i = 0; i < dti->frm_cnt; i++) {
            VTSS_RC(afi_frm_idx_chk(vtss_state, frm_idx = dti->frm_idx[i * 2]));

            vtss_state->afi.frm_tbl[frm_idx].frm_delay.frm.inj_cnt = 1;

            if (i == dti->frm_cnt - 1) {
                // Last entry
                vtss_state->afi.frm_tbl[frm_idx].next_ptr = 0;
            } else {
                VTSS_RC(afi_frm_idx_chk(vtss_state, frm_idx_next = dti->frm_idx[i * 2 + 2]));
                vtss_state->afi.frm_tbl[frm_idx].next_ptr = frm_idx_next;
            }
        }

        cfg->bps_actual = 0;
    } else {
        // Link frames and delays and set inj_cnt=1
        for (i = 0; i < dti->frm_cnt * 2; i++) {
            VTSS_RC(afi_frm_idx_chk(vtss_state, frm_idx      = dti->frm_idx[i]));
            VTSS_RC(afi_frm_idx_chk(vtss_state, frm_idx_next = dti->frm_idx[i + 1]));

            vtss_state->afi.frm_tbl[frm_idx].frm_delay.frm.inj_cnt = 1;

            if (i == dti->frm_cnt * 2 - 1) {
                // Last entry (though a trailing delay may be appended later)
                vtss_state->afi.frm_tbl[frm_idx].next_ptr = 0;
            } else {
                vtss_state->afi.frm_tbl[frm_idx].next_ptr = frm_idx_next;
            }
        }

        // Calculate delays
        if (dti->frm_cnt == 1) {
            VTSS_RC(afi_dti_delay_calc_single_frm(vtss_state, dti_idx, cfg));
        } else {
            VTSS_RC(afi_dti_delay_calc_multi_frm(vtss_state, dti_idx, cfg));
        }

        // Calculate cfg->actual_bps
        VTSS_RC(afi_dti_bps_actual_calc(vtss_state, dti_idx, cfg));
    }

    return VTSS_RC_OK;
}

/******************************************************************************/
//
// External AIL: Fast injections
//
/******************************************************************************/

/*
 * vtss_afi_fast_inj_alloc()
 */
vtss_rc vtss_afi_fast_inj_alloc(const vtss_inst_t inst, const vtss_afi_fast_inj_alloc_cfg_t *const cfg, vtss_afi_fastid_t *const fastid)
{
    vtss_state_t   *vtss_state;
    u32            dti_idx;
    i32            frm_idx;
    i32            i;
    vtss_rc        rc = VTSS_RC_ERROR;
    vtss_afi_dti_t *dti;

    // Argument checks
    if (cfg == NULL || fastid == NULL) {
        VTSS_E("cfg or fastid is NULL");
        return VTSS_RC_ERROR;
    }

    *fastid = 0;

    VTSS_ENTER();

    if ((rc = vtss_inst_port_no_none_check(inst, &vtss_state, cfg->port_no)) != VTSS_RC_OK) {
        goto do_exit;
    }

    if (cfg->frm_cnt == 0 || cfg->frm_cnt > VTSS_AFI_FAST_INJ_FRM_CNT_MAX) {
        VTSS_E("Illegal frm_cnt (%u)", cfg->frm_cnt);
        rc  = VTSS_RC_ERROR;
        goto do_exit;
    }

    if (cfg->prio > VTSS_PRIO_SUPER + 1) {
        VTSS_E("Illegal prio (%u)", cfg->prio);
        rc = VTSS_RC_ERROR;
        goto do_exit;
    }

    // On first alloc, enable AFI (if not already done)
    if (!vtss_state->afi.afi_ena) {
        if ((rc = vtss_state->afi.afi_enable(vtss_state)) != VTSS_RC_OK) {
            goto do_exit;
        }

        vtss_state->afi.afi_ena = 1;
    }

    if ((rc = afi_dti_alloc(vtss_state, &dti_idx)) != VTSS_RC_OK) {
        goto do_exit;
    }

    *fastid = dti_idx;
    dti = &vtss_state->afi.dti_tbl[dti_idx];

    // Allocate FRMs: Two for each frame (frame + delay), plus one for Trailing Delay
    for (i = 0; (u32)i < cfg->frm_cnt * 2 + 1; i++) {
        if ((rc = afi_frm_alloc(vtss_state, &frm_idx, 1)) != VTSS_RC_OK) {
            // Free DTI and allocated FRMs
            i--;
            while (i >= 0) {
                (void)afi_frm_free(vtss_state, dti->frm_idx[i]);
                i--;
            }

            (void)afi_dti_free(vtss_state, dti_idx);
            goto do_exit;
        }

        dti->frm_idx[i] = frm_idx;

        if (i == cfg->frm_cnt * 2) {
            // Trailing delay
            vtss_state->afi.frm_tbl[frm_idx].entry_type = 1;
        } else {
            if (i % 2 == 0) {
                vtss_state->afi.frm_tbl[frm_idx].entry_type = 0; // Frame
            } else {
                vtss_state->afi.frm_tbl[frm_idx].entry_type = 1; // Delay
            }
        }
    }

    dti->port_no = cfg->port_no;
    dti->prio    = cfg->prio;

do_exit:
    VTSS_D("Exit, ID = %u, rc = %u", *fastid, rc);
    VTSS_EXIT();
    return rc;
}

/*
 * vtss_afi_fast_inj_free()
 */
vtss_rc vtss_afi_fast_inj_free(const vtss_inst_t inst, vtss_afi_fastid_t fastid)
{
    vtss_state_t   *vtss_state;
    vtss_afi_dti_t *dti;
    u32            i;
    vtss_rc        rc;

    VTSS_D("Enter, ID = %u", fastid);

    VTSS_ENTER();

    if ((rc = vtss_inst_check(inst, &vtss_state)) != VTSS_RC_OK) {
        goto do_exit;
    }

    if ((rc = afi_dti_idx_chk(vtss_state, fastid)) != VTSS_RC_OK) {
        goto do_exit;
    }

    dti = &vtss_state->afi.dti_tbl[fastid];

    if (dti->started) {
        VTSS_E("Injection must be stopped before freeing");
        rc = VTSS_RC_ERROR;
        goto do_exit;
    }

    // Inject frames for removal
    if ((rc = vtss_state->afi.dti_frm_rm_inj(vtss_state, fastid)) != VTSS_RC_OK) {
        goto do_exit;
    }

    // Free resources
    for (i = 0; i < dti->frm_cnt * 2 + 1; i++) {
        if ((rc = afi_frm_free(vtss_state, dti->frm_idx[i])) != VTSS_RC_OK) {
            goto do_exit;
        }
    }

    rc = afi_dti_free(vtss_state, fastid);

do_exit:
    VTSS_EXIT();
    return rc;
}

/*
 * vtss_afi_fast_inj_frm_hijack()
 */
vtss_rc vtss_afi_fast_inj_frm_hijack(const vtss_inst_t inst, vtss_afi_fastid_t fastid, const vtss_afi_fast_inj_frm_cfg_t *const cfg)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    if (cfg == NULL) {
        VTSS_E("cfg is NULL");
        return VTSS_RC_ERROR;
    }

    VTSS_D("Enter, ID = %u", fastid);

    VTSS_ENTER();

    if ((rc = vtss_inst_check(inst, &vtss_state)) != VTSS_RC_OK) {
        goto do_exit;
    }

    if ((rc = afi_dti_idx_chk(vtss_state, fastid)) != VTSS_RC_OK) {
        goto do_exit;
    }

    rc = vtss_state->afi.dti_frm_hijack(vtss_state, fastid, cfg->frm_size);

do_exit:
    VTSS_EXIT();
    return rc;
}

/*
 * vtss_afi_fast_inj_start()
 */
vtss_rc vtss_afi_fast_inj_start(const vtss_inst_t inst, vtss_afi_fastid_t fastid, vtss_afi_fast_inj_start_cfg_t *const cfg)
{
    vtss_state_t   *vtss_state;
    vtss_afi_dti_t *dti;
    BOOL           do_frm_delay_config = FALSE;
    vtss_rc        rc;

    if (cfg == NULL) {
        VTSS_E("cfg is NULL");
        return VTSS_RC_ERROR;
    }

    VTSS_D("Enter, ID = %u", fastid);

    VTSS_ENTER();

    if ((rc = vtss_inst_check(inst, &vtss_state)) != VTSS_RC_OK) {
        goto do_exit;
    }

    if ((rc = afi_dti_idx_chk(vtss_state, fastid)) != VTSS_RC_OK) {
        goto do_exit;
    }

    dti = &vtss_state->afi.dti_tbl[fastid];

    if (dti->started) {
        VTSS_E("DTI already started");
        rc = VTSS_RC_ERROR;
        goto do_exit;
    }

    if (dti->frm_cnt == 0) {
        VTSS_E("frm_cnt=%u", dti->frm_cnt);
        rc = VTSS_RC_ERROR;
        goto do_exit;
    }

    if (cfg->bps > (u64)VTSS_AFI_FAST_INJ_BPS_MAX) {
        VTSS_E("bps=%" PRIu64 " too high", cfg->bps);
        rc = VTSS_RC_ERROR;
        goto do_exit;
    }

    if (cfg->bps != dti->start_cfg.bps) {
        // Bandwidth has changed => Recalculate delays
        if ((rc = afi_dti_delay_calc(vtss_state, fastid, cfg)) != VTSS_RC_OK) {
            goto do_exit;
        }

        dti->bw = (cfg->bps >= VTSS_AFI_DTI_BW1_THRES_BPS);

        do_frm_delay_config = TRUE;
    }

    dti->mode = (cfg->seq_cnt == 0);

    rc = vtss_state->afi.dti_start(vtss_state, fastid, do_frm_delay_config, TRUE /* do_dti_config */);

do_exit:
    VTSS_EXIT();
    return rc;
}

/*
 * vtss_afi_fast_inj_stop()
 */
vtss_rc vtss_afi_fast_inj_stop(const vtss_inst_t inst, vtss_afi_fastid_t fastid)
{
    vtss_state_t   *vtss_state;
    vtss_afi_dti_t *dti;
    vtss_rc        rc;

    VTSS_D("Enter, ID = %u", fastid);

    VTSS_ENTER();

    if ((rc = vtss_inst_check(inst, &vtss_state)) != VTSS_RC_OK) {
        goto do_exit;
    }

    if ((rc = afi_dti_idx_chk(vtss_state, fastid)) != VTSS_RC_OK) {
        goto do_exit;
    }

    dti = &vtss_state->afi.dti_tbl[fastid];

    if (!dti->started) {
        VTSS_E("DTI not started");
        rc = VTSS_RC_ERROR;
        goto do_exit;
    }

    rc = vtss_state->afi.dti_stop(vtss_state, fastid);

do_exit:
    VTSS_EXIT();
    return rc;
}

/*
 * vtss_afi_fast_inj_seq_cnt_get()
 */
vtss_rc vtss_afi_fast_inj_seq_cnt_get(const vtss_inst_t inst, vtss_afi_fastid_t fastid, u32 *const seq_cnt)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    if (seq_cnt == NULL) {
        VTSS_E("seq_cnt is NULL");
        return VTSS_RC_ERROR;
    }

    VTSS_D("Enter, ID = %u", fastid);

    VTSS_ENTER();

    if ((rc = vtss_inst_check(inst, &vtss_state)) != VTSS_RC_OK) {
        goto do_exit;
    }

    if ((rc = afi_dti_idx_chk(vtss_state, fastid)) != VTSS_RC_OK) {
        goto do_exit;
    }

    rc = vtss_state->afi.dti_cnt_get(vtss_state, fastid, seq_cnt);

do_exit:
    VTSS_EXIT();
    return rc;
}

/******************************************************************************/
//
// External AIL: Slow injections
//
/******************************************************************************/

/*
 * vtss_afi_slow_inj_alloc()
 */
vtss_rc vtss_afi_slow_inj_alloc(const vtss_inst_t                          inst,
                                const vtss_afi_slow_inj_alloc_cfg_t *const cfg,
                                vtss_afi_slowid_t                   *const slowid)
{
    vtss_state_t   *vtss_state;
    u32            tti_idx;
    i32            frm_idx;
    vtss_afi_tti_t *tti;
    vtss_rc        rc;

    // Argument checks
    if (cfg == NULL || slowid == NULL) {
        VTSS_E("cfg or slowid is NULL");
        return VTSS_RC_ERROR;
    }

    *slowid = 0;

    VTSS_ENTER();

    if ((rc = vtss_inst_port_no_none_check(inst, &vtss_state, cfg->port_no)) != VTSS_RC_OK) {
        goto do_exit;
    }

    if (cfg->prio > VTSS_PRIO_SUPER + 1) {
        VTSS_E("Illegal prio (%u)", cfg->prio);
        rc = VTSS_RC_ERROR;
        goto do_exit;
    }

    // On first alloc, enable AFI and TTIs (if not already done)
    if (!vtss_state->afi.afi_ena) {
        if ((rc = vtss_state->afi.afi_enable(vtss_state)) != VTSS_RC_OK) {
            goto do_exit;
        }

        vtss_state->afi.afi_ena = 1;
    }

    if (!vtss_state->afi.tti_ena) {
        if ((rc = vtss_state->afi.ttis_enable(vtss_state)) != VTSS_RC_OK) {
            goto do_exit;
        }

        vtss_state->afi.tti_ena = 1;
    }

    // Allocate a TTI
    if ((rc = afi_tti_alloc(vtss_state, &tti_idx, 0 /* min_tti_idx */, VTSS_AFI_SLOW_INJ_CNT - 1 /* max_tti_idx */)) != VTSS_RC_OK) {
        goto do_exit;
    }

    *slowid = tti_idx;
    tti = &vtss_state->afi.tti_tbl[tti_idx];

    // Allocate a FRM
    if ((rc = afi_frm_alloc(vtss_state, &frm_idx, 0)) != VTSS_RC_OK) {
        (void)afi_tti_free(vtss_state, tti_idx);
        goto do_exit;
    }

    tti->frm_idx = frm_idx;
    tti->port_no = cfg->port_no;
    tti->prio    = cfg->prio;

do_exit:
    VTSS_D("Exit. ID = %u, rc = %u", *slowid, rc);
    VTSS_EXIT();
    return rc;
}

/*
 * vtss_afi_slow_inj_free()
 */
vtss_rc vtss_afi_slow_inj_free(const vtss_inst_t inst, vtss_afi_slowid_t slowid)
{
    vtss_state_t   *vtss_state;
    vtss_afi_tti_t *tti;
    vtss_rc        rc;

    VTSS_D("Enter. ID = %u", slowid);

    VTSS_ENTER();

    if ((rc = vtss_inst_check(inst, &vtss_state)) != VTSS_RC_OK) {
        goto do_exit;
    }

    if ((rc = afi_tti_idx_chk(vtss_state, slowid)) != VTSS_RC_OK) {
        goto do_exit;
    }

    tti = &vtss_state->afi.tti_tbl[slowid];

    if (tti->started) {
        VTSS_E("Injection must be stopped before freeing");
        return VTSS_RC_ERROR;
    }

    // Inject frame for removal
    if ((rc = vtss_state->afi.tti_frm_rm_inj(vtss_state, slowid)) != VTSS_RC_OK) {
        goto do_exit;
    }

    // Free resources
    if ((rc = afi_frm_free(vtss_state, vtss_state->afi.tti_tbl[slowid].frm_idx)) != VTSS_RC_OK) {
        goto do_exit;
    }

    rc = afi_tti_free(vtss_state, slowid);

do_exit:
    VTSS_EXIT();
    return rc;
}

/*
 * vtss_afi_slow_inj_frm_hijack()
 */
vtss_rc vtss_afi_slow_inj_frm_hijack(const vtss_inst_t inst, vtss_afi_slowid_t slowid)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("Enter. ID = %u", slowid);

    VTSS_ENTER();

    if ((rc = vtss_inst_check(inst, &vtss_state)) != VTSS_RC_OK) {
        goto do_exit;
    }

    if ((rc = afi_tti_idx_chk(vtss_state, slowid)) != VTSS_RC_OK) {
        goto do_exit;
    }

    rc = vtss_state->afi.tti_frm_hijack(vtss_state, slowid);

do_exit:
    VTSS_EXIT();
    return rc;
}

/*
 * vtss_afi_slow_inj_start()
 */
vtss_rc vtss_afi_slow_inj_start(const vtss_inst_t inst, vtss_afi_slowid_t slowid, const vtss_afi_slow_inj_start_cfg_t *const cfg)
{
    vtss_state_t   *vtss_state;
    vtss_afi_tti_t *tti;
    BOOL           do_config = 0;
    int            tick_idx;
    const u32      TIMER_LEN_MAX = (1 << VTSS_AFI_TTI_TBL_TIMER_LEN_WID) - 2;
    u32            timer_len_us;
    u32            timer_len_ticks;
    vtss_rc        rc;

    VTSS_D("Enter. ID = %u", slowid);

    VTSS_ENTER();

    // Argument checking
    if (cfg == NULL) {
        VTSS_E("cfg is NULL");
        rc = VTSS_RC_ERROR;
        goto do_exit;
    }

    if ((rc = vtss_inst_check(inst, &vtss_state)) != VTSS_RC_OK) {
        goto do_exit;
    }

    if ((rc = afi_tti_idx_chk(vtss_state, slowid)) != VTSS_RC_OK) {
        goto do_exit;
    }

    if (cfg->fpm == 0) {
        VTSS_E("cfg->fpm == 0");
        rc = VTSS_RC_ERROR;
        goto do_exit;
    }

    if (cfg->jitter_mode > 3) {
        VTSS_E("cfg->jitter_mode=%u", cfg->jitter_mode);
        rc = VTSS_RC_ERROR;
        goto do_exit;
    }

    tti = &vtss_state->afi.tti_tbl[slowid];

    if (tti->started) {
        VTSS_E("TTI already started");
        rc = VTSS_RC_ERROR;
        goto do_exit;
    }

    timer_len_us = (60LLU * 1E6) / cfg->fpm;

    if (afi_tti_start_cfg_cmp(&tti->start_cfg, cfg) != 0) {
        afi_tti_start_cfg_cp(&tti->start_cfg, cfg);

        // Calculate tick_idx and timer_len based on cfg->fpm
        if (cfg->jitter_mode != 0) {
            // Choose fastest possible tick to obtain largest set of values to randomize among
            BOOL tick_found = 0;
            for (tick_idx = 0; tick_idx < 8; tick_idx++) {
                u32 tick_len_us = vtss_state->afi.tick_len_us[tick_idx];
                timer_len_ticks = afi_div_round32(timer_len_us, tick_len_us);
                if (timer_len_ticks <= TIMER_LEN_MAX) {
                    vtss_state->afi.tti_tbl[slowid].timer_len = timer_len_ticks;
                    vtss_state->afi.tti_tbl[slowid].tick_idx  = tick_idx;
                    tick_found = 1;
                    break;
                }
            }

            if (!tick_found) {
                VTSS_E("No tick found for fpm = %" PRIu64, cfg->fpm);
                rc = VTSS_RC_ERROR;
                goto do_exit;
            }
        } else {
            // Choose slowest possible tick resulting in timer_len_ticks >= 8.
            // This reduces the frequency with which TICK_CNT shall be decremented
            // (thus making the walk-through of TTI_TBL as fast as possible)
            // while ensuring some room for randomization of time to first injection.
            BOOL tick_found = 0;

            for (tick_idx = 7; tick_idx >= 0; tick_idx--) {
                u32 tick_len_us = vtss_state->afi.tick_len_us[tick_idx];
                timer_len_ticks = afi_div_round32(timer_len_us, tick_len_us);
                if (timer_len_ticks >= 8 &&
                    // Check that resulting timer is correct within 5%
                    // If not within 5% then a faster tick must be used.
                    afi_timer_prec_ok(timer_len_us, timer_len_ticks * tick_len_us, 5)) {
                    vtss_state->afi.tti_tbl[slowid].timer_len = timer_len_ticks;
                    vtss_state->afi.tti_tbl[slowid].tick_idx  = tick_idx;
                    tick_found = 1;
                    break;
                }
            }

            if (!tick_found) {
                VTSS_E("No tick found for fpm = %" PRIu64, cfg->fpm);
                rc = VTSS_RC_ERROR;
                goto do_exit;
            }
        }

        do_config = 1;
    } else {
        // Configuration unchanged => Just start TTI
        do_config = 0;
    }

    rc = vtss_state->afi.tti_start(vtss_state, slowid, do_config);

do_exit:
    VTSS_EXIT();
    return rc;
}

/*
 * vtss_afi_slow_inj_stop()
 */
vtss_rc vtss_afi_slow_inj_stop(const vtss_inst_t inst, vtss_afi_slowid_t slowid)
{
    vtss_state_t   *vtss_state;
    vtss_afi_tti_t *tti;
    vtss_rc        rc;

    VTSS_D("Enter. ID = %u", slowid);

    VTSS_ENTER();

    if ((rc = vtss_inst_check(inst, &vtss_state)) != VTSS_RC_OK) {
        goto do_exit;
    }

    if ((rc = afi_tti_idx_chk(vtss_state, slowid)) != VTSS_RC_OK) {
        goto do_exit;
    }

    tti = &vtss_state->afi.tti_tbl[slowid];

    if (!tti->started) {
        VTSS_E("TTI not started");
        rc = VTSS_RC_ERROR;
        goto do_exit;
    }

    rc = vtss_state->afi.tti_stop(vtss_state, slowid);

do_exit:
    VTSS_EXIT();
    return rc;
}

/******************************************************************************/
//
// External AIL: Other functions
//
/******************************************************************************/

/*
 * vtss_afi_port_start()
 */
vtss_rc vtss_afi_port_start(const vtss_inst_t inst, vtss_port_no_t port_no)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_ENTER();

    if ((rc = vtss_inst_port_no_none_check(inst, &vtss_state, port_no)) != VTSS_RC_OK) {
        goto do_exit;
    }

    rc = vtss_state->afi.port_admin_start(vtss_state, port_no);

do_exit:
    VTSS_EXIT();
    return rc;
}

/*
 * vtss_afi_port_stop()
 */
vtss_rc vtss_afi_port_stop(const vtss_inst_t inst, vtss_port_no_t port_no)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    if ((rc = vtss_inst_port_no_none_check(inst, &vtss_state, port_no)) != VTSS_RC_OK) {
        goto do_exit;
    }

    rc = vtss_state->afi.port_admin_stop(vtss_state, port_no);

do_exit:
    VTSS_EXIT();
    return rc;
}

/*
 * vtss_afi_debug_print()
 */
void vtss_afi_debug_print(vtss_state_t *vtss_state, const vtss_debug_printf_t pr, const vtss_debug_info_t *const info)
{
    vtss_afi_state_t *afi = &vtss_state->afi;
    vtss_afi_frm_t   *frm;
    vtss_afi_dti_t   *dti;
    vtss_afi_tti_t   *tti;
    u32              res_idx, i;

    if (!vtss_debug_group_enabled(pr, info, VTSS_DEBUG_GROUP_AFI)) {
        return;
    }

    pr("Frame Table\n");
    pr("#    Type  FrmSz InjCnt Delay     \n");
    pr("---- ----- ----- ------ ----------\n");

    for (res_idx = 0; res_idx < VTSS_ARRSZ(afi->frm_tbl); res_idx++) {
        if (afi_res_is_free(afi->frms_alloced, res_idx)) {
            continue;
        }

        frm = &afi->frm_tbl[res_idx];
        if (frm->entry_type) {
            pr("%4u Delay %5s %6s %10u\n", res_idx, "N/A",                      "N/A",                       frm->frm_delay.delay.delay);
        } else {
            pr("%4u Frame %5u %6u %10s\n", res_idx, frm->frm_delay.frm.frm_size, frm->frm_delay.frm.inj_cnt, "N/A");
        }
    }


    pr("\nDTI Table\n");
    pr("#  Port Prio FrmInjCnt  Indices into Frame Table\n");
    pr("-- ---- ---- ---------- ------------------------\n");

    for (res_idx = 0; res_idx < VTSS_ARRSZ(afi->dti_tbl); res_idx++) {
        if (afi_res_is_free(afi->dtis_alloced, res_idx)) {
            continue;
        }

        dti = &afi->dti_tbl[res_idx];
        pr("%2u %4u %4u %10u", res_idx, dti->port_no, dti->prio, dti->frm_inj_cnt);
        for (i = 0; i < 2 * dti->frm_cnt + 1; i++) {
            pr(" %u", dti->frm_idx[i]);
        }

        pr("\n");
    }

    pr("\nTTI Ticks\n");
    pr("#   TickLen [us]\n");
    pr("--- ------------\n");
    for (res_idx = 0; res_idx < VTSS_ARRSZ(afi->tick_len_us); res_idx++) {
        pr("%3u %12u\n", res_idx, afi->tick_len_us[res_idx]);
    }

    pr("\nTTI Table\n");
    pr("#  Port Prio Frames/min  TickIdx TmrLen Jit FrmTblIdx TickCnt\n");
    pr("-- ---- ---- ----------- ------- ------ --- --------- -------\n");

    for (res_idx = 0; res_idx < VTSS_ARRSZ(afi->tti_tbl); res_idx++) {
        if (afi_res_is_free(afi->ttis_alloced, res_idx)) {
            continue;
        }

        tti = &afi->tti_tbl[res_idx];
        pr("%2u %4u %4u %11llu %7u %6u %3u %9d %7u\n", res_idx, tti->port_no, tti->prio, tti->start_cfg.fpm, tti->tick_idx, tti->timer_len, tti->jitter, tti->frm_idx, tti->tick_cnt);
    }

    pr("\n");
}

/*
 * vtss_afi_inst_create()
 *
 * Instance creation and initialization
 */
vtss_rc vtss_afi_inst_create(vtss_state_t *vtss_state)
{
    vtss_afi_state_t *state = &vtss_state->afi;
    vtss_port_no_t   port_no;

    // Set chip default values
    for (port_no = 0; port_no < VTSS_ARRSZ(state->port_tbl); port_no++) {
        state->port_tbl[port_no].frm_out_max = VTSS_AFI_FRM_OUT_MAX_DEF;
    }

    return VTSS_RC_OK;
}

#endif /* defined(VTSS_FEATURE_AFI_SWC) && defined(VTSS_AFI_V2) */

