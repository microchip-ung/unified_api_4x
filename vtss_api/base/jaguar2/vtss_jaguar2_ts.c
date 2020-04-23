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

#define VTSS_TRACE_GROUP VTSS_TRACE_GROUP_TS
#include "vtss_jaguar2_cil.h"

#if defined(VTSS_ARCH_JAGUAR_2)

#if defined(VTSS_FEATURE_TIMESTAMP)

/* - CIL functions ------------------------------------------------- */

/* GPIO configuration */
static const u8 ptp_gpio[4] = {
    8,  /* PTP_0 */
    9,  /* PTP_1 */
    26, /* PTP_2 */
    27  /* PTP_3 */
};

#define HW_NS_PR_SEC 1000000000L
#define HW_PS_PR_SEC 1000000000000LL
#define HW_PS_PR_NS  1000
#define ADJ_UNITS_PR_NS 10
#define PPS_WIDTH 200 /* Width of 1PPS pulse in ns */

static vtss_rc timestampAddSec(vtss_timestamp_t *ts)
{
    if (ts->seconds == 0xffffffff) {
        ts->sec_msb++;
        ts->seconds = 0;
    } else {
        ts->seconds++;
    }
    return VTSS_RC_OK;
}

static vtss_rc timestampSubSec(vtss_timestamp_t *ts)
{
    if (ts->seconds == 0) {
        ts->sec_msb--;
        ts->seconds = 0xffffffff;
    } else {
        ts->seconds--;
    }
    return VTSS_RC_OK;
}

static vtss_rc timestampAdd(vtss_timestamp_t *ts, const vtss_timestamp_t *ts_add)
{
    ts->nanoseconds += ts_add->nanoseconds;
    if (ts->nanoseconds >= HW_NS_PR_SEC) {
        VTSS_RC(timestampAddSec(ts));
        ts->nanoseconds -= HW_NS_PR_SEC;
    }
    ts->seconds += ts_add->seconds;
    if (ts->seconds < ts_add->seconds) {
        ts->sec_msb++;
    }
    ts->sec_msb += ts_add->sec_msb;
    return VTSS_RC_OK;
}

static vtss_rc timestampSub(vtss_timestamp_t *ts, const vtss_timestamp_t *ts_sub)
{
    ts->sec_msb -= ts_sub->sec_msb;
    if (ts->seconds < ts_sub->seconds) {
        ts->sec_msb--;
    }
    ts->seconds -= ts_sub->seconds;
    if (ts->nanoseconds < ts_sub->nanoseconds) {
        VTSS_RC(timestampSubSec(ts));
        ts->nanoseconds += HW_NS_PR_SEC;
    }
    ts->nanoseconds -= ts_sub->nanoseconds;
    return VTSS_RC_OK;
}

static vtss_rc jr2_ts_timeofday_read(vtss_state_t *vtss_state, vtss_timestamp_t *ts, u32 *tc)
{
    u32 value;
    JR2_WRM(VTSS_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG(EXT_PPS_PIN),
            VTSS_F_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG_PTP_PIN_ACTION(2) | /* 2: SAVE */
            VTSS_F_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG_PTP_PIN_SYNC(0),
            VTSS_M_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG_PTP_PIN_ACTION |
            VTSS_M_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG_PTP_PIN_SYNC);
    JR2_RD(VTSS_DEVCPU_PTP_PTP_PINS_PTP_TOD_SEC_MSB(EXT_PPS_PIN), &value);
    ts->sec_msb = VTSS_X_DEVCPU_PTP_PTP_PINS_PTP_TOD_SEC_MSB_PTP_TOD_SEC_MSB(value);
    JR2_RD(VTSS_DEVCPU_PTP_PTP_PINS_PTP_TOD_SEC_LSB(EXT_PPS_PIN), &ts->seconds);
    JR2_RD(VTSS_DEVCPU_PTP_PTP_PINS_PTP_TOD_NSEC(EXT_PPS_PIN), &value);
    ts->nanoseconds = VTSS_X_DEVCPU_PTP_PTP_PINS_PTP_TOD_NSEC_PTP_TOD_NSEC(value);
    if (ts->nanoseconds >= 0x3ffffff0 && ts->nanoseconds <= 0x3fffffff) { /* -1..-16 = 10^9-1..16 */
        VTSS_RC(timestampSubSec(ts));
        ts->nanoseconds = 999999984 + (ts->nanoseconds & 0xf);
    }
    JR2_RD(VTSS_DEVCPU_PTP_PTP_PINS_PTP_NSF(EXT_PPS_PIN), tc);
    return VTSS_RC_OK;
}

static vtss_rc jr2_ts_timeofday_sample(vtss_state_t *vtss_state)
{
    vtss_ts_ext_clock_one_pps_mode_t one_pps_mode = vtss_state->ts.conf.ext_clock_mode.one_pps_mode;
    if (one_pps_mode == TS_EXT_CLOCK_MODE_ONE_PPS_INPUT) {
        VTSS_RC(jr2_ts_timeofday_read(vtss_state, &vtss_state->ts.conf.sample_ts, &vtss_state->ts.conf.sample_tc));
        JR2_WRM(VTSS_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG(EXT_PPS_PIN),
                VTSS_F_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG_PTP_PIN_ACTION(2) | /* 2: SAVE */
                VTSS_F_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG_PTP_PIN_SYNC(1),
                VTSS_M_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG_PTP_PIN_ACTION |
                VTSS_M_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG_PTP_PIN_SYNC);
    } else if (one_pps_mode == TS_EXT_CLOCK_MODE_ONE_PPS_OUTPUT) {
        (void) vtss_jr2_gpio_mode(vtss_state, 0, ptp_gpio[EXT_PPS_PIN], VTSS_GPIO_OUT);
        VTSS_RC(jr2_ts_timeofday_read(vtss_state, &vtss_state->ts.conf.sample_ts, &vtss_state->ts.conf.sample_tc));
        JR2_WRM(VTSS_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG(EXT_PPS_PIN),
                VTSS_F_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG_PTP_PIN_ACTION(3) | /* 3: CLOCK */
                VTSS_F_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG_PTP_PIN_SYNC(1),
                VTSS_M_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG_PTP_PIN_ACTION |
                VTSS_M_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG_PTP_PIN_SYNC);
        (void) vtss_jr2_gpio_mode(vtss_state, 0, ptp_gpio[EXT_PPS_PIN], VTSS_GPIO_ALT_0);
    }
    return VTSS_RC_OK;
}

static vtss_rc jr2_ts_timeofday_get(vtss_state_t *vtss_state, vtss_timestamp_t *ts, u32 *tc)
{
    if (vtss_state->ts.conf.ext_clock_mode.one_pps_mode == TS_EXT_CLOCK_MODE_ONE_PPS_DISABLE) {
        VTSS_RC(jr2_ts_timeofday_read(vtss_state, ts, tc));
    } else {
        *ts = vtss_state->ts.conf.sample_ts;
        *tc = vtss_state->ts.conf.sample_tc;
    }

    VTSS_D("ts->sec_msb: %u, ts->seconds: %u, ts->nanoseconds: %u", ts->sec_msb, ts->seconds, ts->nanoseconds);
    return VTSS_RC_OK;
}

static vtss_rc jr2_ts_timeofday_next_pps_get(vtss_state_t *vtss_state, vtss_timestamp_t *ts)
{
    if (vtss_state->ts.conf.ext_clock_mode.one_pps_mode == TS_EXT_CLOCK_MODE_ONE_PPS_DISABLE) {
        u32 tc;
        VTSS_RC(jr2_ts_timeofday_read(vtss_state, ts, &tc));
    } else {
        *ts = vtss_state->ts.conf.sample_ts;
        //VTSS_RC(timestampAddSec(ts));
    }
    VTSS_RC(timestampAddSec(ts));
    ts->nanoseconds = 0;

    VTSS_D("ts->sec_msb: %u, ts->seconds: %u, ts->nanoseconds: %u", ts->sec_msb, ts->seconds, ts->nanoseconds);
    return VTSS_RC_OK;
}

static vtss_rc jr2_ts_timeofday_set(vtss_state_t *vtss_state, const vtss_timestamp_t *ts)
{
    /* must be in IDLE mode before the time can be loaded */
    JR2_WRM(VTSS_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG(EXT_PPS_PIN),
            VTSS_F_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG_PTP_PIN_ACTION(0) | /* 0: IDLE */
            VTSS_F_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG_PTP_PIN_SYNC(0),
            VTSS_M_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG_PTP_PIN_ACTION |
            VTSS_M_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG_PTP_PIN_SYNC);
    JR2_WR(VTSS_DEVCPU_PTP_PTP_PINS_PTP_TOD_SEC_MSB(EXT_PPS_PIN),
           VTSS_F_DEVCPU_PTP_PTP_PINS_PTP_TOD_SEC_MSB_PTP_TOD_SEC_MSB(ts->sec_msb));
    JR2_WR(VTSS_DEVCPU_PTP_PTP_PINS_PTP_TOD_SEC_LSB(EXT_PPS_PIN), ts->seconds);
    JR2_WR(VTSS_DEVCPU_PTP_PTP_PINS_PTP_TOD_NSEC(EXT_PPS_PIN),
           VTSS_F_DEVCPU_PTP_PTP_PINS_PTP_TOD_NSEC_PTP_TOD_NSEC(ts->nanoseconds));
    JR2_WRM(VTSS_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG(EXT_PPS_PIN),
            VTSS_F_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG_PTP_PIN_ACTION(1) | /* 4: LOAD */
            VTSS_F_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG_PTP_PIN_SYNC(0),
            VTSS_M_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG_PTP_PIN_ACTION |
            VTSS_M_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG_PTP_PIN_SYNC);
    VTSS_RC(jr2_ts_timeofday_sample(vtss_state));
    VTSS_D("ts->sec_msb: %u, ts->seconds: %u, ts->nanoseconds: %u", ts->sec_msb, ts->seconds, ts->nanoseconds);
    return VTSS_RC_OK;
}

static vtss_rc jr2_ts_timeofday_offset_set(vtss_state_t *vtss_state, i32 offset)
{
    /* must be in IDLE mode before the time can be loaded */
    JR2_WRM(VTSS_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG(EXT_PPS_PIN),
            VTSS_F_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG_PTP_PIN_ACTION(0) | /* 0: IDLE */
            VTSS_F_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG_PTP_PIN_SYNC(0),
            VTSS_M_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG_PTP_PIN_ACTION |
            VTSS_M_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG_PTP_PIN_SYNC);
    JR2_WR(VTSS_DEVCPU_PTP_PTP_PINS_PTP_TOD_NSEC(EXT_PPS_PIN),
           VTSS_F_DEVCPU_PTP_PTP_PINS_PTP_TOD_NSEC_PTP_TOD_NSEC(-offset));
    JR2_WRM(VTSS_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG(EXT_PPS_PIN),
            VTSS_F_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG_PTP_PIN_ACTION(4) | /* 4: DELTA */
            VTSS_F_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG_PTP_PIN_SYNC(0),
            VTSS_M_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG_PTP_PIN_ACTION |
            VTSS_M_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG_PTP_PIN_SYNC);

    VTSS_RC(jr2_ts_timeofday_sample(vtss_state));
    VTSS_D("Time after offset_set: %d, %d, %d",vtss_state->ts.conf.sample_ts.sec_msb,vtss_state->ts.conf.sample_ts.seconds,vtss_state->ts.conf.sample_ts.nanoseconds);
    VTSS_D("offset: %d", offset);
    return VTSS_RC_OK;
}


static vtss_rc jr2_ts_timeofday_set_delta(vtss_state_t *vtss_state, const vtss_timestamp_t *ts, BOOL negative)
{
    if (ts->seconds > 0 || ts->sec_msb > 0 || ts->nanoseconds > HW_NS_PR_SEC/2) {
        vtss_timestamp_t ts_prev;
        u32              tc;

        /* Read current time */
        VTSS_RC(jr2_ts_timeofday_read(vtss_state, &ts_prev, &tc));

        /* Calculate new time */
        if (negative){
            VTSS_RC(timestampSub(&ts_prev, ts));
        } else {
            VTSS_RC(timestampAdd(&ts_prev, ts));
        }

        /* Load the new time */
        JR2_WR(VTSS_DEVCPU_PTP_PTP_PINS_PTP_TOD_SEC_MSB(EXT_PPS_PIN),
                VTSS_F_DEVCPU_PTP_PTP_PINS_PTP_TOD_SEC_MSB_PTP_TOD_SEC_MSB(ts_prev.sec_msb));
        JR2_WR(VTSS_DEVCPU_PTP_PTP_PINS_PTP_TOD_SEC_LSB(EXT_PPS_PIN), ts_prev.seconds);
        JR2_WR(VTSS_DEVCPU_PTP_PTP_PINS_PTP_TOD_NSEC(EXT_PPS_PIN),
                VTSS_F_DEVCPU_PTP_PTP_PINS_PTP_TOD_NSEC_PTP_TOD_NSEC(ts_prev.nanoseconds));
        JR2_WRM(VTSS_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG(EXT_PPS_PIN),
                VTSS_F_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG_PTP_PIN_ACTION(1) | /* 4: LOAD */
                VTSS_F_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG_PTP_PIN_SYNC(0),
                VTSS_M_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG_PTP_PIN_ACTION |
                VTSS_M_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG_PTP_PIN_SYNC);
    } else {
        JR2_WR(VTSS_DEVCPU_PTP_PTP_PINS_PTP_TOD_NSEC(EXT_PPS_PIN),
               VTSS_F_DEVCPU_PTP_PTP_PINS_PTP_TOD_NSEC_PTP_TOD_NSEC(negative ? -(i32)ts->nanoseconds : ts->nanoseconds));
        JR2_WRM(VTSS_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG(EXT_PPS_PIN),
                VTSS_F_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG_PTP_PIN_ACTION(4) | /* 4: DELTA */
                VTSS_F_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG_PTP_PIN_SYNC(0),
                VTSS_M_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG_PTP_PIN_ACTION |
                VTSS_M_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG_PTP_PIN_SYNC);
    }

    VTSS_RC(jr2_ts_timeofday_sample(vtss_state));
    VTSS_D("ts->sec_msb: %u, ts->seconds: %u, ts->nanoseconds: %u", ts->sec_msb, ts->seconds, ts->nanoseconds);
    return VTSS_RC_OK;
}

static vtss_rc jr2_ts_adjtimer_set(vtss_state_t *vtss_state)
{
    i32 adj = vtss_state->ts.conf.adj;

    if (adj != 0) {
        u32 adj_dir = (adj < 0 ? 1 : 0);
        u64 adj_abs = VTSS_LABS(adj);
        u64 adj_val = VTSS_DIV64((u64)HW_PS_PR_SEC*(u64)ADJ_UNITS_PR_NS, adj_abs);
        u32 adj_unit = 0; /* Default to picoseconds unit */
        if (adj_val >= (1L << 30)) {
            /* The adjustment value is too large to be in ps */
            adj_val = VTSS_DIV64(adj_val, (u64)HW_PS_PR_NS);
            adj_unit = 1; /* Use nanosecond uint */
        }
        if (adj_val >= (1L << 30)) { /* if it is still too big then rate is 0 */
            /* The adjustment value is too large to be in ns */
            adj = 0;
        } else {

            /* Configure and enable adjustment */
            JR2_WR(VTSS_DEVCPU_PTP_PTP_CFG_CLK_ADJ_FRQ(0),
                   VTSS_F_DEVCPU_PTP_PTP_CFG_CLK_ADJ_FRQ_CLK_ADJ_UNIT(adj_unit) |
                   VTSS_F_DEVCPU_PTP_PTP_CFG_CLK_ADJ_FRQ_CLK_ADJ(adj_val));
            JR2_WR(VTSS_DEVCPU_PTP_PTP_CFG_CLK_ADJ_CFG(0),
                   VTSS_F_DEVCPU_PTP_PTP_CFG_CLK_ADJ_CFG_CLK_ADJ_DIR(adj_dir) |
                   VTSS_F_DEVCPU_PTP_PTP_CFG_CLK_ADJ_CFG_CLK_ADJ_ENA(1));
        }
    }
    if (adj == 0) {
        /* Disable adjustment */
        JR2_WR(VTSS_DEVCPU_PTP_PTP_CFG_CLK_ADJ_CFG(0), 0);
    }    
    return VTSS_RC_OK;
}

/* Set the clock mode for the external clock */
static vtss_rc jr2_ts_external_clock_mode_set(vtss_state_t *vtss_state)
{
    vtss_ts_ext_clock_mode_t *ext_clock_mode = &vtss_state->ts.conf.ext_clock_mode;

    VTSS_D("one_pps_mode: %u, enable: %u, freq: %u", ext_clock_mode->one_pps_mode, ext_clock_mode->enable, ext_clock_mode->freq);
    JR2_WRM(VTSS_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG(EXT_CLK_PIN),
            VTSS_F_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG_PTP_PIN_ACTION(0), /* 0: IDLE */
            VTSS_M_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG_PTP_PIN_ACTION);
    if (ext_clock_mode->enable) {
        u32 dividers = HW_NS_PR_SEC/ext_clock_mode->freq;
        u32 high_div = dividers/2;
        u32 low_div  = (dividers+1)/2;
        JR2_WR(VTSS_DEVCPU_PTP_PTP_PINS_PIN_WF_HIGH_PERIOD(EXT_CLK_PIN),
               VTSS_F_DEVCPU_PTP_PTP_PINS_PIN_WF_HIGH_PERIOD_PIN_WFH(high_div));
        JR2_WR(VTSS_DEVCPU_PTP_PTP_PINS_PIN_WF_LOW_PERIOD(EXT_CLK_PIN),
               VTSS_F_DEVCPU_PTP_PTP_PINS_PIN_WF_LOW_PERIOD_PIN_WFL(low_div));

        (void) vtss_jr2_gpio_mode(vtss_state, 0, ptp_gpio[EXT_CLK_PIN], VTSS_GPIO_ALT_0);
        JR2_WRM(VTSS_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG(EXT_CLK_PIN),
                VTSS_F_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG_PTP_PIN_ACTION(3) | /* 3: CLOCK */
                VTSS_F_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG_PTP_PIN_SYNC(0),
                VTSS_M_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG_PTP_PIN_ACTION |
                VTSS_M_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG_PTP_PIN_SYNC);

    } else if (ext_clock_mode->one_pps_mode == TS_EXT_CLOCK_MODE_ONE_PPS_OUTPUT) {
        (void) vtss_jr2_gpio_mode(vtss_state, 0, ptp_gpio[EXT_CLK_PIN], VTSS_GPIO_ALT_0);
        JR2_WR(VTSS_DEVCPU_PTP_PTP_PINS_PIN_WF_HIGH_PERIOD(EXT_CLK_PIN),
               VTSS_F_DEVCPU_PTP_PTP_PINS_PIN_WF_HIGH_PERIOD_PIN_WFH(PPS_WIDTH));
        JR2_WR(VTSS_DEVCPU_PTP_PTP_PINS_PIN_WF_LOW_PERIOD(EXT_CLK_PIN), 0);
    
        JR2_WRM(VTSS_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG(EXT_CLK_PIN),
                VTSS_F_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG_PTP_PIN_ACTION(3) | /* 3: CLOCK */
                VTSS_F_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG_PTP_PIN_SYNC(1),
                VTSS_M_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG_PTP_PIN_ACTION |
                VTSS_M_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG_PTP_PIN_SYNC);
    } else {
        (void) vtss_jr2_gpio_mode(vtss_state, 0, ptp_gpio[EXT_CLK_PIN], VTSS_GPIO_IN);
    }
    if (ext_clock_mode->one_pps_mode != TS_EXT_CLOCK_MODE_ONE_PPS_DISABLE) {
        (void) vtss_jr2_gpio_mode(vtss_state, 0, ptp_gpio[EXT_PPS_PIN], VTSS_GPIO_ALT_0);
        JR2_WR(VTSS_DEVCPU_PTP_PTP_PINS_PIN_WF_HIGH_PERIOD(EXT_PPS_PIN),
               VTSS_F_DEVCPU_PTP_PTP_PINS_PIN_WF_HIGH_PERIOD_PIN_WFH(PPS_WIDTH));
        JR2_WR(VTSS_DEVCPU_PTP_PTP_PINS_PIN_WF_LOW_PERIOD(EXT_PPS_PIN), 0);
    } else {
        (void) vtss_jr2_gpio_mode(vtss_state, 0, ptp_gpio[EXT_PPS_PIN], VTSS_GPIO_IN);
    }
    VTSS_RC(jr2_ts_timeofday_sample(vtss_state));

    return VTSS_RC_OK;
}

/* Get the saved nanosec counter */
static vtss_rc jr2_ts_alt_clock_saved_get(vtss_state_t *vtss_state, u32 *const saved)
{
    vtss_ts_alt_clock_mode_t *alt_clock_mode = &vtss_state->ts.conf.alt_clock_mode;
    JR2_RD(VTSS_DEVCPU_PTP_PTP_PINS_PTP_TOD_NSEC(ALT_LDST_PIN), saved);
    *saved = VTSS_X_DEVCPU_PTP_PTP_PINS_PTP_TOD_NSEC_PTP_TOD_NSEC(*saved);
    if (*saved >= 0x3ffffff0 && *saved <= 0x3fffffff) { /* -1..-16 = 10^9-1..16 */
        *saved = 999999984 + (*saved & 0xf);
    }
    if (alt_clock_mode->one_pps_in) {
        if (alt_clock_mode->save && alt_clock_mode->load) {
            VTSS_E("save and load cannot be enabled at the same time");
        } else if (alt_clock_mode->save) {
            JR2_WRM(VTSS_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG(ALT_LDST_PIN),
                    VTSS_F_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG_PTP_PIN_ACTION(2), /* 2: SAVE */
                    VTSS_M_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG_PTP_PIN_ACTION);
        } else if (alt_clock_mode->load) {
            JR2_WRM(VTSS_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG(ALT_LDST_PIN),
                    VTSS_F_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG_PTP_PIN_ACTION(1), /* 1: LOAD */
                    VTSS_M_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG_PTP_PIN_ACTION);
        }
    }
    return VTSS_RC_OK;
}

/* Set the clock mode for the alternative clock
 * the modes are: 
 *    BOOL one_pps_out;        Enable 1pps output
 *    BOOL one_pps_in;         Enable 1pps input
 *    BOOL save;               Save actual time counter at next 1 PPS input
 *    BOOL load;               Load actual time counter at next 1 PPS input
 */
static vtss_rc jr2_ts_alt_clock_mode_set(vtss_state_t *vtss_state)
{
    vtss_ts_alt_clock_mode_t *alt_clock_mode = &vtss_state->ts.conf.alt_clock_mode;
    
    JR2_WRM(VTSS_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG(ALT_PPS_PIN),
            VTSS_F_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG_PTP_PIN_ACTION(0), /* 0: IDLE */
            VTSS_M_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG_PTP_PIN_ACTION);
    if (alt_clock_mode->one_pps_out) {
        JR2_WR(VTSS_DEVCPU_PTP_PTP_PINS_PIN_WF_HIGH_PERIOD(ALT_PPS_PIN),
               VTSS_F_DEVCPU_PTP_PTP_PINS_PIN_WF_HIGH_PERIOD_PIN_WFH(PPS_WIDTH));
        JR2_WR(VTSS_DEVCPU_PTP_PTP_PINS_PIN_WF_LOW_PERIOD(ALT_PPS_PIN), 0);

        (void) vtss_jr2_gpio_mode(vtss_state, 0, ptp_gpio[ALT_PPS_PIN], VTSS_GPIO_ALT_0);
        JR2_WRM(VTSS_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG(ALT_PPS_PIN),
                VTSS_F_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG_PTP_PIN_ACTION(3) | /* 3: CLOCK */
                VTSS_F_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG_PTP_PIN_SYNC(1),
                VTSS_M_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG_PTP_PIN_ACTION |
                VTSS_M_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG_PTP_PIN_SYNC);
    } else {
        (void) vtss_jr2_gpio_mode(vtss_state, 0, ptp_gpio[ALT_PPS_PIN], VTSS_GPIO_IN);
    }

    JR2_WRM(VTSS_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG(ALT_PPS_PIN),
            VTSS_F_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG_PTP_PIN_ACTION(0), /* 0: IDLE */
            VTSS_M_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG_PTP_PIN_ACTION);
    if (alt_clock_mode->one_pps_in) {
        if (alt_clock_mode->save && alt_clock_mode->load) {
            VTSS_E("save and load cannot be enabled at the same time");
            return VTSS_RC_ERROR;
        } else if (alt_clock_mode->save) {
            (void) vtss_jr2_gpio_mode(vtss_state, 0, ptp_gpio[ALT_LDST_PIN], VTSS_GPIO_ALT_0);
            JR2_WRM(VTSS_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG(ALT_LDST_PIN),
                    VTSS_F_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG_PTP_PIN_ACTION(2) | /* 2: SAVE */
                    VTSS_F_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG_PTP_PIN_SYNC(1),
                    VTSS_M_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG_PTP_PIN_ACTION |
                    VTSS_M_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG_PTP_PIN_SYNC);
        } else if (alt_clock_mode->load) {
            (void) vtss_jr2_gpio_mode(vtss_state, 0, ptp_gpio[ALT_LDST_PIN], VTSS_GPIO_ALT_0);
            JR2_WRM(VTSS_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG(ALT_LDST_PIN),
                    VTSS_F_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG_PTP_PIN_ACTION(1) | /* 1: LOAD */
                    VTSS_F_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG_PTP_PIN_SYNC(1),
                    VTSS_M_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG_PTP_PIN_ACTION |
                    VTSS_M_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG_PTP_PIN_SYNC);
        }
    } else {
        (void) vtss_jr2_gpio_mode(vtss_state, 0, ptp_gpio[ALT_LDST_PIN], VTSS_GPIO_IN);
    }
    return VTSS_RC_OK;
}

/* Set the time to be loaded into the PTP timer at the next 1PPS
 * It is assumed that this function is called at the beginning of a sec */
static vtss_rc jr2_ts_timeofday_next_pps_set(vtss_state_t *vtss_state, const vtss_timestamp_t *const ts)
{
    if (vtss_state->ts.conf.alt_clock_mode.one_pps_in) {
        JR2_WR(VTSS_DEVCPU_PTP_PTP_PINS_PTP_TOD_SEC_MSB(ALT_LDST_PIN),
               VTSS_F_DEVCPU_PTP_PTP_PINS_PTP_TOD_SEC_MSB_PTP_TOD_SEC_MSB(ts->sec_msb));
        JR2_WR(VTSS_DEVCPU_PTP_PTP_PINS_PTP_TOD_SEC_LSB(ALT_LDST_PIN), ts->seconds-1);
        JR2_WR(VTSS_DEVCPU_PTP_PTP_PINS_PTP_TOD_NSEC(ALT_LDST_PIN),
               VTSS_F_DEVCPU_PTP_PTP_PINS_PTP_TOD_NSEC_PTP_TOD_NSEC(ts->nanoseconds));
    }

    VTSS_D("time at next pps: sec_msb %u, sec %u, ns %u", ts->sec_msb, ts->seconds, ts->nanoseconds);
    return VTSS_RC_OK;
}

static vtss_rc jr2_ts_ingress_latency_set(vtss_state_t *vtss_state, vtss_port_no_t port_no)
{
    vtss_port_interface_t interface = vtss_state->port.conf[port_no].if_type;
    u32                   port = VTSS_CHIP_PORT(port_no);
    vtss_ts_port_conf_t   *conf = &vtss_state->ts.port_conf[port_no];
    i32                   rx_delay = VTSS_INTERVAL_NS(conf->ingress_latency) + conf->default_igr_latency;
    if ((rx_delay < 0) || (rx_delay > 0x1ff)) {
        VTSS_E("invalid ingress latency %d", rx_delay);
        return VTSS_RC_ERROR;
    }
    VTSS_D("rx_delay %d", rx_delay);

    switch (interface) {
    case VTSS_PORT_INTERFACE_SGMII:
    case VTSS_PORT_INTERFACE_SGMII_CISCO:
    case VTSS_PORT_INTERFACE_SERDES:
    case VTSS_PORT_INTERFACE_VAUI:
    case VTSS_PORT_INTERFACE_100FX:
    case VTSS_PORT_INTERFACE_QSGMII:
        JR2_WRM(VTSS_DEV1G_DEV_CFG_STATUS_PTP_CFG(VTSS_TO_DEV1G(port)),
                VTSS_F_DEV1G_DEV_CFG_STATUS_PTP_CFG_PTP_RX_IO_DLY(rx_delay),
                VTSS_M_DEV1G_DEV_CFG_STATUS_PTP_CFG_PTP_RX_IO_DLY);
        break;
    case VTSS_PORT_INTERFACE_SFI:
    case VTSS_PORT_INTERFACE_XAUI:
    case VTSS_PORT_INTERFACE_RXAUI:
        JR2_WRM(VTSS_DEV10G_DEV_CFG_STATUS_PTP_CFG(VTSS_TO_DEV(port)),
                VTSS_F_DEV10G_DEV_CFG_STATUS_PTP_CFG_PTP_RX_IO_DLY(rx_delay),
                VTSS_M_DEV10G_DEV_CFG_STATUS_PTP_CFG_PTP_RX_IO_DLY);
        break;

    default:
        VTSS_E("unsupported interface: %u", interface);
        return VTSS_RC_ERROR;
    }
    return VTSS_RC_OK;
}

static vtss_rc jr2_ts_p2p_delay_set(vtss_state_t *vtss_state, vtss_port_no_t port_no)
{
    vtss_ts_port_conf_t *conf = &vtss_state->ts.port_conf[port_no];
    i32                 p2p_delay = VTSS_INTERVAL_NS(conf->p2p_delay);
    JR2_WR(VTSS_REW_PORT_PTP_IDLY2_CFG(VTSS_CHIP_PORT(port_no)), p2p_delay + VTSS_INTERVAL_NS(conf->delay_asymmetry));
    return VTSS_RC_OK;
}

static vtss_rc jr2_ts_egress_latency_set(vtss_state_t *vtss_state, vtss_port_no_t port_no)
{
    vtss_port_interface_t interface = vtss_state->port.conf[port_no].if_type;
    u32                   port = VTSS_CHIP_PORT(port_no);
    vtss_ts_port_conf_t   *conf = &vtss_state->ts.port_conf[port_no];
    i32                   tx_delay = VTSS_INTERVAL_NS(conf->egress_latency) + conf->default_egr_latency;
    if ((tx_delay < 0) || (tx_delay > 0x1ff)) {
        VTSS_E("invalid egress latency %d", tx_delay);
        return VTSS_RC_ERROR;
    }
    VTSS_D("tx_delay %d", tx_delay);

    switch (interface) {
    case VTSS_PORT_INTERFACE_SGMII:
    case VTSS_PORT_INTERFACE_SGMII_CISCO:
    case VTSS_PORT_INTERFACE_SERDES:
    case VTSS_PORT_INTERFACE_VAUI:
    case VTSS_PORT_INTERFACE_100FX:
    case VTSS_PORT_INTERFACE_QSGMII:
        JR2_WRM(VTSS_DEV1G_DEV_CFG_STATUS_PTP_CFG(VTSS_TO_DEV1G(port)),
                VTSS_F_DEV1G_DEV_CFG_STATUS_PTP_CFG_PTP_TX_IO_DLY(tx_delay),
                VTSS_M_DEV1G_DEV_CFG_STATUS_PTP_CFG_PTP_TX_IO_DLY);
        break;
    case VTSS_PORT_INTERFACE_SFI:
    case VTSS_PORT_INTERFACE_XAUI:
    case VTSS_PORT_INTERFACE_RXAUI:
        JR2_WRM(VTSS_DEV10G_DEV_CFG_STATUS_PTP_CFG(VTSS_TO_DEV(port)),
                VTSS_F_DEV10G_DEV_CFG_STATUS_PTP_CFG_PTP_TX_IO_DLY(tx_delay),
                VTSS_M_DEV10G_DEV_CFG_STATUS_PTP_CFG_PTP_TX_IO_DLY);
        break;

    default:
        VTSS_E("unsupported interface: %u", interface);
        return VTSS_RC_ERROR;
    }
    return VTSS_RC_OK;
}

static vtss_rc jr2_ts_delay_asymmetry_set(vtss_state_t *vtss_state, vtss_port_no_t port_no)
{
    vtss_ts_port_conf_t *conf = &vtss_state->ts.port_conf[port_no];
    i32 delay = VTSS_INTERVAL_NS(conf->delay_asymmetry);
    /* Used for Ingress asymmetry compensation (Pdelay_Resp) */
    JR2_WR(VTSS_REW_PORT_PTP_IDLY1_CFG(VTSS_CHIP_PORT(port_no)), delay);
    /* Used for Egress asymmetry compensation (on Delay_Req and Pdelay_Req) */
    JR2_WR(VTSS_REW_PORT_PTP_EDLY_CFG(VTSS_CHIP_PORT(port_no)), -delay);
    /* Used for Ingress asymmetry compensation (on Sync, therefore include p2p_delay ) */
    JR2_WR(VTSS_REW_PORT_PTP_IDLY2_CFG(VTSS_CHIP_PORT(port_no)), delay + VTSS_INTERVAL_NS(conf->p2p_delay));
    return VTSS_RC_OK;
}

static vtss_rc jr2_ts_operation_mode_set(vtss_state_t *vtss_state, vtss_port_no_t port_no)
{
    vtss_ts_mode_t         mode = vtss_state->ts.port_conf[port_no].mode.mode;
    vtss_ts_internal_fmt_t fmt = vtss_state->ts.int_mode.int_fmt;
    u32                    mode_val = 0;
    if (mode == TS_MODE_INTERNAL) {
        switch (fmt) {
        case TS_INTERNAL_FMT_RESERVED_LEN_30BIT:
            mode_val = 1;
            break;
        case TS_INTERNAL_FMT_RESERVED_LEN_32BIT:
            mode_val = 2;
            break;
        case TS_INTERNAL_FMT_SUB_ADD_LEN_44BIT_CF62:
            mode_val = 3;
            break;
        case TS_INTERNAL_FMT_RESERVED_LEN_48BIT_CF:
            mode_val = 4;
            break;
        default:
            VTSS_E("unsupported internal timestamp format: %u", fmt);
            return VTSS_RC_ERROR;
        }
    }
    JR2_WRM(VTSS_REW_PORT_PTP_MODE_CFG(VTSS_CHIP_PORT(port_no), 0),
            VTSS_F_REW_PORT_PTP_MODE_CFG_PTP_MODE_VAL(mode_val),
            VTSS_M_REW_PORT_PTP_MODE_CFG_PTP_MODE_VAL);
    JR2_WRM(VTSS_REW_PORT_PTP_MODE_CFG(VTSS_CHIP_PORT(port_no), 1),
            VTSS_F_REW_PORT_PTP_MODE_CFG_PTP_MODE_VAL(mode_val),
            VTSS_M_REW_PORT_PTP_MODE_CFG_PTP_MODE_VAL);
    return VTSS_RC_OK;
}

static vtss_rc jr2_ts_internal_mode_set(vtss_state_t *vtss_state)
{
    vtss_ts_internal_fmt_t fmt = vtss_state->ts.int_mode.int_fmt;
    switch (fmt) {
    case TS_INTERNAL_FMT_RESERVED_LEN_30BIT:
    case TS_INTERNAL_FMT_RESERVED_LEN_32BIT:
    case TS_INTERNAL_FMT_SUB_ADD_LEN_44BIT_CF62:
    case TS_INTERNAL_FMT_RESERVED_LEN_48BIT_CF:
        break;
    default:
        VTSS_E("unsupported internal timestamp format: %u", fmt);
        return VTSS_RC_ERROR;
    }
    return VTSS_RC_OK;
}

static u32 api_port(vtss_state_t *vtss_state, u32 chip_port)
{
    u32 port_no;
    int found = 0;
    /* Map from chip port to API port */
    if (chip_port == VTSS_CHIP_PORT_CPU) {
        port_no = VTSS_CHIP_PORT_CPU;
    } else {
        for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {
            if (VTSS_CHIP_PORT(port_no) == chip_port) {
                found = 1;
                break;
            }
        }
        if (!found) {
            VTSS_E("unknown chip port: %u, port_no: %d", chip_port, port_no);
        }
    }
    return port_no;
}


static vtss_rc jr2_ts_timestamp_get(vtss_state_t *vtss_state)
{
    u32  value;
    u32  delay;
    u32  tx_port;
    u32  mess_id;
    BOOL overflow = FALSE;

    JR2_RD(VTSS_REW_PTP_CTRL_PTP_TWOSTEP_CTRL, &value);
    while (VTSS_X_REW_PTP_CTRL_PTP_TWOSTEP_CTRL_PTP_VLD(value)) {
        /* Read TX timestamp */
        if (!VTSS_X_REW_PTP_CTRL_PTP_TWOSTEP_CTRL_STAMP_TX(value)) {
            VTSS_E("TX timestamp expected but RX timestamp found");
            JR2_WR(VTSS_REW_PTP_CTRL_PTP_TWOSTEP_CTRL, VTSS_F_REW_PTP_CTRL_PTP_TWOSTEP_CTRL_PTP_NXT(1));
            JR2_RD(VTSS_REW_PTP_CTRL_PTP_TWOSTEP_CTRL, &value);
            continue;
        }
        overflow |= VTSS_X_REW_PTP_CTRL_PTP_TWOSTEP_CTRL_PTP_OVFL(value);
        JR2_RD(VTSS_REW_PTP_CTRL_PTP_TWOSTEP_STAMP, &delay);
        tx_port = api_port(vtss_state, VTSS_X_REW_PTP_CTRL_PTP_TWOSTEP_CTRL_STAMP_PORT(value));

        /* Read RX timestamp */
        JR2_WR(VTSS_REW_PTP_CTRL_PTP_TWOSTEP_CTRL, VTSS_F_REW_PTP_CTRL_PTP_TWOSTEP_CTRL_PTP_NXT(1));
        JR2_RD(VTSS_REW_PTP_CTRL_PTP_TWOSTEP_CTRL, &value);
        if (!VTSS_X_REW_PTP_CTRL_PTP_TWOSTEP_CTRL_PTP_VLD(value) ||
            VTSS_X_REW_PTP_CTRL_PTP_TWOSTEP_CTRL_STAMP_TX(value)) {
            VTSS_E("RX timestamp not found");
            JR2_WR(VTSS_REW_PTP_CTRL_PTP_TWOSTEP_CTRL, VTSS_F_REW_PTP_CTRL_PTP_TWOSTEP_CTRL_PTP_NXT(1));
            JR2_RD(VTSS_REW_PTP_CTRL_PTP_TWOSTEP_CTRL, &value);
            continue;
        }
        overflow |= VTSS_X_REW_PTP_CTRL_PTP_TWOSTEP_CTRL_PTP_OVFL(value);
        JR2_RD(VTSS_REW_PTP_CTRL_PTP_TWOSTEP_STAMP, &mess_id);

        if (tx_port < VTSS_PORT_ARRAY_SIZE) {
            vtss_state->ts.status[mess_id].tx_tc[tx_port] = delay;
            vtss_state->ts.status[mess_id].tx_id[tx_port] = mess_id;
            vtss_state->ts.status[mess_id].valid_mask |= 1LL<<tx_port;
        } else {
            VTSS_E("invalid port (%u)", tx_port);
        }

        VTSS_D("value %x, delay %u, tx_port %u, mess_id %u", value, delay, tx_port, mess_id);
        JR2_WR(VTSS_REW_PTP_CTRL_PTP_TWOSTEP_CTRL, VTSS_F_REW_PTP_CTRL_PTP_TWOSTEP_CTRL_PTP_NXT(1));
        JR2_RD(VTSS_REW_PTP_CTRL_PTP_TWOSTEP_CTRL, &value);
    }
    if (overflow) {
        VTSS_E("Timestamp fifo overflow occurred");
    }
    return VTSS_RC_OK;
}

/*
* Signal port status change (used to detect and compensate for the internal ingress and egress latencies)
*/
static vtss_rc jr2_ts_status_change(vtss_state_t *vtss_state, const vtss_port_no_t port_no)
{
    vtss_rc               rc = VTSS_RC_OK, rc2;
    i32                   rx_delay, tx_delay;
    vtss_port_interface_t interface = vtss_state->port.conf[port_no].if_type;
    u32                   port = VTSS_CHIP_PORT(port_no);
    u32                   value;
    VTSS_D("port status change %d", port_no);

    /* I/O delays taken from DSH1061 section 2.31.9 */
    switch (interface) {
    case VTSS_PORT_INTERFACE_SGMII:
    case VTSS_PORT_INTERFACE_SGMII_CISCO:
    case VTSS_PORT_INTERFACE_SERDES:
        /* Single-Lane SerDes at 1 Gbps */
        if (port <= 7 || port == 48) {
            rx_delay = 576;
            tx_delay = 380;
        } else if (port >= 8 && port <= 15) {
            rx_delay = 655;
            tx_delay = 470;
        } else if (port >= 16 && port <= 23) {
            rx_delay = 655;
            tx_delay = 630;
        } else if (port >= 24 && port <= 31) {
            rx_delay = 815;
            tx_delay = 630;
        } else if (port >= 49 && port <= 52) {
            rx_delay = 1103;
            tx_delay = 710;
        } else {
            VTSS_E("unsupported interface %u on port %u", interface, port);
            return VTSS_RC_ERROR;
        }
        JR2_RD(VTSS_DEV1G_PCS1G_CFG_STATUS_PCS1G_LINK_STATUS(VTSS_TO_DEV1G(port)), &value);
        rx_delay += 8 * VTSS_X_DEV1G_PCS1G_CFG_STATUS_PCS1G_LINK_STATUS_DELAY_VAR(value);
        break;
    case VTSS_PORT_INTERFACE_VAUI:
        /* Single-Lane SerDes at 2.5 Gbps */
        if (port >= 8 && port <= 15) {
            rx_delay = 260;
            tx_delay = 180;
        } else if (port >= 16 && port <= 23) {
            rx_delay = 260;
            tx_delay = 240;
        } else if (port >= 24 && port <= 31) {
            rx_delay = 320;
            tx_delay = 240;
        } else if (port >= 49 && port <= 52) {
            rx_delay = 440;
            tx_delay = 280;
        } else {
            VTSS_E("unsupported interface %u on port %u", interface, port);
            return VTSS_RC_ERROR;
        }
        JR2_RD(VTSS_DEV1G_PCS1G_CFG_STATUS_PCS1G_LINK_STATUS(VTSS_TO_DEV1G(port)), &value);
        rx_delay += 32 * VTSS_X_DEV1G_PCS1G_CFG_STATUS_PCS1G_LINK_STATUS_DELAY_VAR(value) / 10;
        break;
    case VTSS_PORT_INTERFACE_100FX:
        /* Single-Lane SerDes at 100 Mbps */
        if (port <= 7 || port == 48) {
            rx_delay = 962;
            tx_delay = 700;
        } else if (port >= 8 && port <= 15) {
            rx_delay = 1039;
            tx_delay = 800;
        } else if (port >= 16 && port <= 23) {
            rx_delay = 1039;
            tx_delay = 960;
        } else if (port >= 24 && port <= 31) {
            rx_delay = 1199;
            tx_delay = 960;
        } else if (port == 49) {
            rx_delay = 1440;
            tx_delay = 1030;
        } else if (port == 50 || port == 51) {
            rx_delay = 1425;
            tx_delay = 1030;
        } else if (port == 52) {
            rx_delay = 1483;
            tx_delay = 1030;
        } else {
            VTSS_E("unsupported interface %u on port %u", interface, port);
            return VTSS_RC_ERROR;
        }
        JR2_RD(VTSS_DEV1G_PCS1G_CFG_STATUS_PCS1G_LINK_STATUS(VTSS_TO_DEV1G(port)), &value);
        rx_delay += 8 * VTSS_X_DEV1G_PCS1G_CFG_STATUS_PCS1G_LINK_STATUS_DELAY_VAR(value);
        break;
    case VTSS_PORT_INTERFACE_SFI:
        /* Single-Lane SerDes at 10 Gbps */
        if (port >= 49 && port <= 52) {
            rx_delay = 1170;
            tx_delay = 1510;
        } else {
            VTSS_E("unsupported interface %u on port %u", interface, port);
            return VTSS_RC_ERROR;
        }
        break;
    case VTSS_PORT_INTERFACE_QSGMII:
        /* Single-Lane SerDes at 4 Gbps (QSGMII) */
        if (port <= 15) {
            rx_delay = 1103;
            tx_delay = 577;
        } else if (port >= 16 && port <= 47) {
            rx_delay = 1100;
            tx_delay = 657;
        } else {
            VTSS_E("unsupported interface %u on port %u", interface, port);
            return VTSS_RC_ERROR;
        }
        JR2_RD(VTSS_HSIO_HW_CFGSTAT_HW_QSGMII_STAT(port / 4), &value);
        rx_delay += 2 * VTSS_X_HSIO_HW_CFGSTAT_HW_QSGMII_STAT_DELAY_VAR_X200PS(value);
        break;
    case VTSS_PORT_INTERFACE_XAUI:
        /* Four-Lane SerDes at 10 Gbps (XAUI) */
        if (port == 49 || port == 50) {
            rx_delay = 868;
            tx_delay = 1170;
        } else if (port == 51 || port == 52) {
            rx_delay = 804;
            tx_delay = 1170;
        } else {
            VTSS_E("unsupported interface %u on port %u", interface, port);
            return VTSS_RC_ERROR;
        }
        JR2_RD(VTSS_DEV10G_PCS_XAUI_STATUS_PCS_XAUI_DESKEW_STATUS(VTSS_TO_DEV(port)), &value);
        rx_delay += 32 * (VTSS_X_DEV10G_PCS_XAUI_STATUS_PCS_XAUI_DESKEW_STATUS_LANE_DESKEW_DELAY(value) % 16);
        JR2_RD(VTSS_DEV10G_PCS_XAUI_STATUS_PCS_XAUI_CGALIGN_STATUS(VTSS_TO_DEV(port)), &value);
        rx_delay += 32 * (VTSS_X_DEV10G_PCS_XAUI_STATUS_PCS_XAUI_CGALIGN_STATUS_LANE_CGALIGN_DELAY(value) % 32) / 10;
        break;
    case VTSS_PORT_INTERFACE_RXAUI:
        /* Two-Lane SerDes at 10 Gbps (RXAUI) */
        if (port == 49 || port == 50) {
            rx_delay = 916;
            tx_delay = 1168;
        } else if (port == 51 || port == 52) {
            rx_delay = 852;
            tx_delay = 1168;
        } else {
            VTSS_E("unsupported interface %u on port %u", interface, port);
            return VTSS_RC_ERROR;
        }
        JR2_RD(VTSS_DEV10G_PCS_XAUI_STATUS_PCS_XAUI_DESKEW_STATUS(VTSS_TO_DEV(port)), &value);
        rx_delay += 32 * (VTSS_X_DEV10G_PCS_XAUI_STATUS_PCS_XAUI_DESKEW_STATUS_LANE_DESKEW_DELAY(value) % 16);
        JR2_RD(VTSS_DEV10G_PCS_XAUI_STATUS_PCS_XAUI_CGALIGN_STATUS(VTSS_TO_DEV(port)), &value);
        rx_delay += 16 * (VTSS_X_DEV10G_PCS_XAUI_STATUS_PCS_XAUI_CGALIGN_STATUS_LANE_CGALIGN_DELAY(value) % 32) / 10;
        break;

    default:
        VTSS_E("unsupported interface: %u", interface);
        return VTSS_RC_ERROR;
    }

    /* Add additional delays found in testing */
    switch (interface) {
    case VTSS_PORT_INTERFACE_SGMII:
    case VTSS_PORT_INTERFACE_SGMII_CISCO:
    case VTSS_PORT_INTERFACE_SERDES:
    case VTSS_PORT_INTERFACE_100FX:
        if ((port >= 8 && port <= 31) || (port >= 48 && port <= 52)) {
            /* DEV2G5 */
            rx_delay += 160;
            tx_delay += 160;
        }
        break;
    case VTSS_PORT_INTERFACE_VAUI:
        rx_delay += 70;
        tx_delay += 70;
        break;
    default:
        /* No additional delays */
        break;
    }

    rx_delay = (rx_delay + 5) / 10;
    tx_delay = (tx_delay + 5) / 10;
    vtss_state->ts.port_conf[port_no].default_igr_latency = rx_delay;
    vtss_state->ts.port_conf[port_no].default_egr_latency = tx_delay;
    VTSS_D("port_no %d, igr_latency %d, egr_latency %d", port_no, rx_delay, tx_delay);
    rc = jr2_ts_ingress_latency_set(vtss_state, port_no);
    rc2 = jr2_ts_egress_latency_set(vtss_state, port_no);
    if (rc == VTSS_RC_OK) {
        rc = rc2;
    }
    return rc;
}

static vtss_rc 
jr2_ts_timestamp_id_release(vtss_state_t *vtss_state, u32 ts_id)
{
    return VTSS_RC_OK;
}


/* - Debug print --------------------------------------------------- */

static vtss_rc jr2_debug_ts(vtss_state_t *vtss_state, const vtss_debug_printf_t pr, const vtss_debug_info_t *const info)
{
    u32            port;
    vtss_port_no_t port_no;
    char           buf[64];
    int            idx;

    /* REW:PORT */
    for (port = 0; port <= VTSS_CHIP_PORTS; port++) {
        sprintf(buf, "REW:PORT[%u]", port);
        vtss_jr2_debug_reg_header(pr, buf);
        vtss_jr2_debug_reg(vtss_state, pr, VTSS_REW_PORT_PTP_MODE_CFG(port, 0), "PTP_MODE_CFG[0]");
        vtss_jr2_debug_reg(vtss_state, pr, VTSS_REW_PORT_PTP_MODE_CFG(port, 1), "PTP_MODE_CFG[1]");
        vtss_jr2_debug_reg(vtss_state, pr, VTSS_REW_PORT_PTP_EDLY_CFG(port), "PTP_EDLY_CFG");
        vtss_jr2_debug_reg(vtss_state, pr, VTSS_REW_PORT_PTP_IDLY1_CFG(port), "PTP_IDLY1_CFG");
        vtss_jr2_debug_reg(vtss_state, pr, VTSS_REW_PORT_PTP_IDLY2_CFG(port), "PTP_IDLY2_CFG");
    }

    /* REW:PTP_CTRL */
    vtss_jr2_debug_reg_header(pr, "REW:PTP_CTRL");
    vtss_jr2_debug_reg(vtss_state, pr, VTSS_REW_PTP_CTRL_PTP_TWOSTEP_CTRL, "PTP_TWOSTEP_CTRL");
    vtss_jr2_debug_reg(vtss_state, pr, VTSS_REW_PTP_CTRL_PTP_TWOSTEP_STAMP, "PTP_TWOSTEP_STAMP");
    
    /* DEVCPU_PTP:PTP_CFG */
    vtss_jr2_debug_reg_header(pr, "DEVCPU_PTP:PTP_CFG");
    vtss_jr2_debug_reg(vtss_state, pr, VTSS_DEVCPU_PTP_PTP_CFG_PTP_MISC_CFG, "PTP_MISC_CFG");
    for (idx = 0; idx <= 2; idx++) {
        sprintf(buf, "CLK_ADJ_CFG[%u]", idx);
        vtss_jr2_debug_reg(vtss_state, pr, VTSS_DEVCPU_PTP_PTP_CFG_CLK_ADJ_CFG(idx), buf);
    }
    for (idx = 0; idx <= 2; idx++) {
        sprintf(buf, "CLK_ADJ_FRQ[%u]", idx);
        vtss_jr2_debug_reg(vtss_state, pr, VTSS_DEVCPU_PTP_PTP_CFG_CLK_ADJ_FRQ(idx), buf);
    }
    vtss_jr2_debug_reg(vtss_state, pr, VTSS_DEVCPU_PTP_PTP_CFG_PTP_PIN_INTR, "PTP_PIN_INTR");
    vtss_jr2_debug_reg(vtss_state, pr, VTSS_DEVCPU_PTP_PTP_CFG_PTP_PIN_INTR_ENA, "PTP_PIN_INTR_ENA");
    vtss_jr2_debug_reg(vtss_state, pr, VTSS_DEVCPU_PTP_PTP_CFG_PTP_INTR_IDENT, "PTP_PIN_INTR_IDENT");

    /* DEVCPU_PTP:PTP_PINS */
    for (idx = 0; idx <= 3; idx++) {
        sprintf(buf, "DEVCPU_PTP:PTP_PINS[%u]", idx);
        vtss_jr2_debug_reg_header(pr, buf);
        vtss_jr2_debug_reg(vtss_state, pr, VTSS_DEVCPU_PTP_PTP_PINS_PTP_PIN_CFG(idx), "PTP_PIN_CFG");
        vtss_jr2_debug_reg(vtss_state, pr, VTSS_DEVCPU_PTP_PTP_PINS_PTP_TOD_SEC_MSB(idx), "PTP_TOD_SEC_MSB");
        vtss_jr2_debug_reg(vtss_state, pr, VTSS_DEVCPU_PTP_PTP_PINS_PTP_TOD_SEC_LSB(idx), "PTP_TOD_SEC_LSB");
        vtss_jr2_debug_reg(vtss_state, pr, VTSS_DEVCPU_PTP_PTP_PINS_PTP_TOD_NSEC(idx), "PTP_TOD_NSEC");
        vtss_jr2_debug_reg(vtss_state, pr, VTSS_DEVCPU_PTP_PTP_PINS_PTP_NSF(idx), "PTP_NSF");
        vtss_jr2_debug_reg(vtss_state, pr, VTSS_DEVCPU_PTP_PTP_PINS_PIN_WF_HIGH_PERIOD(idx), "PIN_WF_HIGH_PERIOD");
        vtss_jr2_debug_reg(vtss_state, pr, VTSS_DEVCPU_PTP_PTP_PINS_PIN_WF_LOW_PERIOD(idx), "PIN_WF_LOW_PERIOD");
    }

    /* DEV1G:DEV_CFG_STATUS / DEV10G:DEV_CFG_STATUS */
    for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {
        port = VTSS_CHIP_PORT(port_no);
        switch (vtss_state->port.conf[port_no].if_type) {
        case VTSS_PORT_INTERFACE_SGMII:
        case VTSS_PORT_INTERFACE_SGMII_CISCO:
        case VTSS_PORT_INTERFACE_SERDES:
        case VTSS_PORT_INTERFACE_VAUI:
        case VTSS_PORT_INTERFACE_100FX:
        case VTSS_PORT_INTERFACE_QSGMII:
            sprintf(buf, "DEV1G (port_no %u):DEV_CFG_STATUS", port_no);
            vtss_jr2_debug_reg_header(pr, buf);
            vtss_jr2_debug_reg(vtss_state, pr, VTSS_DEV1G_DEV_CFG_STATUS_PTP_CFG(VTSS_TO_DEV1G(port)), "PTP_CFG");
            break;
        case VTSS_PORT_INTERFACE_SFI:
        case VTSS_PORT_INTERFACE_XAUI:
        case VTSS_PORT_INTERFACE_RXAUI:
            sprintf(buf, "DEV10G (port_no %u):DEV_CFG_STATUS", port_no);
            vtss_jr2_debug_reg_header(pr, buf);
            vtss_jr2_debug_reg(vtss_state, pr, VTSS_DEV10G_DEV_CFG_STATUS_PTP_CFG(VTSS_TO_DEV(port)), "PTP_CFG");
            break;
        default:
            break;
        }
    }

    pr("\n");

    return VTSS_RC_OK;
}

vtss_rc vtss_jr2_ts_debug_print(vtss_state_t *vtss_state, const vtss_debug_printf_t pr, const vtss_debug_info_t *const info)
{
    return vtss_debug_print_group(VTSS_DEBUG_GROUP_TS, jr2_debug_ts, vtss_state, pr, info);
}

/* - Initialization ------------------------------------------------ */

static vtss_rc jr2_ts_init(vtss_state_t *vtss_state)
{
    /* Enable PTP */
    JR2_WR(VTSS_DEVCPU_PTP_PTP_CFG_PTP_MISC_CFG, VTSS_F_DEVCPU_PTP_PTP_CFG_PTP_MISC_CFG_PTP_ENA(3));

    return VTSS_RC_OK;
}

vtss_rc vtss_jr2_ts_init(vtss_state_t *vtss_state, vtss_init_cmd_t cmd)
{
    vtss_ts_state_t *state = &vtss_state->ts;

    switch (cmd) {
    case VTSS_INIT_CMD_CREATE:
        state->timeofday_get = jr2_ts_timeofday_get;
        state->timeofday_set = jr2_ts_timeofday_set;
        state->timeofday_set_delta = jr2_ts_timeofday_set_delta;
        state->timeofday_next_pps_get = jr2_ts_timeofday_next_pps_get;
        state->timeofday_offset_set = jr2_ts_timeofday_offset_set;
        state->adjtimer_set = jr2_ts_adjtimer_set;
        state->timeofday_one_sec = jr2_ts_timeofday_sample;
        state->external_clock_mode_set = jr2_ts_external_clock_mode_set;
        state->alt_clock_saved_get = jr2_ts_alt_clock_saved_get;
        state->alt_clock_mode_set = jr2_ts_alt_clock_mode_set;
        state->timeofday_next_pps_set = jr2_ts_timeofday_next_pps_set;
        state->ingress_latency_set = jr2_ts_ingress_latency_set;
        state->p2p_delay_set = jr2_ts_p2p_delay_set;
        state->egress_latency_set = jr2_ts_egress_latency_set;
        state->delay_asymmetry_set = jr2_ts_delay_asymmetry_set;
        state->operation_mode_set = jr2_ts_operation_mode_set;
        state->internal_mode_set = jr2_ts_internal_mode_set;
        state->timestamp_get = jr2_ts_timestamp_get;
        state->status_change = jr2_ts_status_change;
        state->timestamp_id_release = jr2_ts_timestamp_id_release;

        break;
    case VTSS_INIT_CMD_INIT:
        VTSS_RC(jr2_ts_init(vtss_state));
        break;
    default:
        break;
    }
    return VTSS_RC_OK;
}

#endif /* VTSS_FEATURE_TIMESTAMP */

#endif /* VTSS_ARCH_JAGUAR_2 */
