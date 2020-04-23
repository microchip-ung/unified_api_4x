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

#define VTSS_TRACE_GROUP VTSS_TRACE_GROUP_TS
#include "vtss_serval_cil.h"

#if defined(VTSS_ARCH_SERVAL)

#if defined(VTSS_FEATURE_TIMESTAMP)

/* - CIL functions ------------------------------------------------- */

#define HW_NS_PR_SEC 1000000000L
//#define HW_CLK_CNT_PR_SEC 250000000L
#define HW_CLK_CNT_PR_SEC 156250000L

#define HW_CLK_50_MS (HW_NS_PR_SEC/20)
#define HW_CLK_M950_MS (-HW_NS_PR_SEC + HW_CLK_50_MS)
#define EXT_SYNC_INPUT_LATCH_LATENCY 5 //(1*HW_NS_PR_SEC/HW_CLK_CNT_PR_SEC)  /* 1 clock cycle added to EXT_SYNC_CURRENT TIME */
#define ADJ_UNITS_PR_NS 10
/* 5 clock cycle compensation in ingress and egress latency compenstation, because the timestamped clock is 
 * 5 cycles behind the master timer */
#define TIMESTAMPER_SKEW 32

#define SPEED_INDEX_10M   0
#define SPEED_INDEX_100M  1
#define SPEED_INDEX_1000M 2
#define SPEED_INDEX_2500M 3
#define SPEED_INDEX_MAX   4

/*
 * Note: The values i have added to the table below ensures the correct path delay, byt the asymmetry may not be correct.
 */
static const i32 latency_igr[SPEED_INDEX_MAX] = {
    [SPEED_INDEX_10M]   = 71+100,
    [SPEED_INDEX_100M]  = 71+100, /* I have added 100 ns compared to the values given by HW in order to obtain the correct path delay */
    [SPEED_INDEX_1000M] = 71+16,  /* I have added 16 ns compared to the values given by HW in order to obtain the correct path delay */
    [SPEED_INDEX_2500M] = 23+8   /* I have added 8 ns compared to the values given by HW in order to obtain the correct path delay */
};

static const i32 latency_egr[SPEED_INDEX_MAX] = {
    [SPEED_INDEX_10M]   = 793+100, /* the correct value is 7273, but this is outside the latency value range */
    [SPEED_INDEX_100M]  = 793+100, /* I have added 100 ns compared to the values given by HW in order to obtain the correct path delay */
    [SPEED_INDEX_1000M] = 153+16,  /* I have added 16 ns compared to the values given by HW in order to obtain the correct path delay */
    [SPEED_INDEX_2500M] = 67+8    /* I have added 8 ns compared to the values given by HW in order to obtain the correct path delay */
};

static vtss_rc subTimestamp(i32 *r, const vtss_timestamp_t *x, const vtss_timestamp_t *y)
{
    i64 temp;
	temp = (x->sec_msb - y->sec_msb)*0x100000000LL;
	temp += x->seconds - y->seconds;
    if (temp < -1 || temp > 1) return VTSS_RC_ERROR;
    *r = temp*VTSS_ONE_MIA;
	*r += x->nanoseconds - y->nanoseconds;
    return VTSS_RC_OK;
}

static vtss_rc srvl_ts_timeofday_read(vtss_state_t *vtss_state, vtss_timestamp_t *ts)
{
    vtss_timestamp_t r1, r2;
    u32 value;
    /* read timestamp registers twice, and verify that no wrapping occurred between two register reads.
       return the value that did not wrap */
    SRVL_RD(VTSS_SYS_PTP_TOD_PTP_TOD_NSEC, &r1.nanoseconds);
    SRVL_RD(VTSS_SYS_PTP_TOD_PTP_TOD_LSB, &r1.seconds);
    SRVL_RD(VTSS_SYS_PTP_TOD_PTP_TOD_MSB, &value);
    r1.sec_msb = VTSS_X_SYS_PTP_TOD_PTP_TOD_MSB_PTP_TOD_MSB(value);

    SRVL_RD(VTSS_SYS_PTP_TOD_PTP_TOD_NSEC, &r2.nanoseconds);
    if (r1.nanoseconds < r2.nanoseconds) {
        *ts = r1; /* nothing wrapped */
    } else {
        SRVL_RD(VTSS_SYS_PTP_TOD_PTP_TOD_LSB, &r2.seconds);
        SRVL_RD(VTSS_SYS_PTP_TOD_PTP_TOD_MSB, &value);
        r2.sec_msb = VTSS_X_SYS_PTP_TOD_PTP_TOD_MSB_PTP_TOD_MSB(value);
        *ts = r2; /* nanoseconds wrapped and sec incremented*/
    }
    VTSS_D("ts->sec_msb: %u, ts->seconds: %u, ts->nanoseconds: %u, delta ns = %u", ts->sec_msb, ts->seconds, ts->nanoseconds, r2.nanoseconds - r1.nanoseconds);
    /* ns value may be >= 1 sec */
    if (ts->nanoseconds >= VTSS_ONE_MIA) {
        VTSS_D("ns >= one mia: ts->sec_msb: %u, ts->seconds: %u, ts->nanoseconds: %u", ts->sec_msb, ts->seconds, ts->nanoseconds);
        ts->nanoseconds -= VTSS_ONE_MIA;
        if (++ts->seconds == 0) ++ts->sec_msb;
    }
    return VTSS_RC_OK;
}

static vtss_rc srvl_ts_timeofday_get(vtss_state_t *vtss_state,
                                     vtss_timestamp_t *ts,
                                     u32              *tc)
{
    i32 x;
    VTSS_RC(srvl_ts_timeofday_read(vtss_state, ts));
    /* tc is returned as 32 bit value, i.e. (seconds*10**9 + nanoseconds) mod 2*32 */
    /* Any value in sec_msb will not change this value, therefore it is omitted */
    *tc = ts->nanoseconds + ts->seconds*VTSS_ONE_MIA;
    x = ts->nanoseconds;
    /* compensate for outstanding adjustment */
    if (vtss_state->ts.conf.awaiting_adjustment) {
        x = ts->nanoseconds - vtss_state->ts.conf.outstanding_corr;
        if (x < 0) {
            ts->seconds--;
            x += HW_NS_PR_SEC;
        } else if (x >= HW_NS_PR_SEC) {
            ts->seconds++;
            x -= HW_NS_PR_SEC;
        }
        ts->nanoseconds = x;
    }
    VTSS_D("ts->seconds: %u, ts->nanoseconds: %u", ts->seconds, ts->nanoseconds);
    return VTSS_RC_OK;
}

static vtss_rc srvl_ts_timeofday_next_pps_get(vtss_state_t *vtss_state,
                                              vtss_timestamp_t *ts)
{
    VTSS_RC(srvl_ts_timeofday_read(vtss_state, ts));
    ++ts->seconds; /* sec counter is maintained in SW */
    ts->nanoseconds = 0;
    VTSS_D("ts->seconds: %u, ts->nanoseconds: %u", ts->seconds, ts->nanoseconds);
    return VTSS_RC_OK;
}


/*
 * Subtract the offset from the actual time. 
 * offset is in the range [-1..+1] sec.
 * alignment algorithm:
 *  offset > 50 ms => 
 *    PTP_UPPER_LIMIT_1_TIME_ADJ = TOD_NANOSECS - ts.nanoseconds
 *    (TBD if this works, because the ADJ is < actual TOD_NANOSECS ??)
 *  50 ms > offset > -950 ms => 
 *    PTP_UPPER_LIMIT_1_TIME_ADJ = TOD_NANOSECS - ts.nanoseconds + 1 sec
 *  -950 ms > offset => 
 *    PTP_UPPER_LIMIT_1_TIME_ADJ = TOD_NANOSECS - ts.nanoseconds + 2 sec
 * The second offset is adjusted accordingly.
 *
 * In serval the ts_conf.sec_offset is only used for time adjustment. i.e if sec_offset != 0 
 *   then the sec counter is adjusted in the next onesec call: 
 *     1 => sec counter is incremented;
 *     2 => sec counter is decremented.
 */
static vtss_rc srvl_ts_timeofday_offset_set(vtss_state_t *vtss_state, i32 offset)
{
    vtss_ts_conf_t *conf = &vtss_state->ts.conf;

    if (conf->awaiting_adjustment) {
        return VTSS_RC_ERROR;
    }
    conf->awaiting_adjustment = TRUE;
    conf->outstanding_adjustment = 0;

    if (offset > HW_CLK_50_MS) {
        conf->sec_offset = 2; 
        conf->outstanding_adjustment = offset;
        conf->outstanding_corr = offset - HW_NS_PR_SEC;
    } else if (offset < HW_CLK_M950_MS) {
        conf->sec_offset = 1;
        conf->outstanding_adjustment = offset + 2*HW_NS_PR_SEC;
        conf->outstanding_corr = offset + HW_NS_PR_SEC;
    } else {
        conf->sec_offset = 0;
        conf->outstanding_corr = offset;
        conf->outstanding_adjustment = offset + HW_NS_PR_SEC;
    }
    /* we wait until TOD_NANOSECS wraps */
    VTSS_D("adjust = %u, offset: %d", conf->outstanding_adjustment, conf->outstanding_corr);
    return VTSS_RC_OK;
}
/*
 * When the time is set, the HW sec  counters are written. The HW clock is adjusted so that the timestamp nanoseconds
 * and the hw nanoseconds counters are aligned. This cannot be done with nanosecond precision, so the final alignment
 * is done by the clock rate adjustment. The hw nanoseconds counter is aligned by setting the
 * PTP_UPPER_LIMIT_1_TIME_ADJ. This register sets the next onesec period to [0.. 1.073.741.823] nanosec. 
 * A value of 0 makes no sense as this means two clock pulses with no time distance, therefore the algorithm sets the
 * period in the range [0,050..1,050] Sec.
 */
static vtss_rc srvl_ts_timeofday_set(vtss_state_t *vtss_state, const vtss_timestamp_t *ts)
{
    i32 corr;
    vtss_timestamp_t ts_hw;
    if (vtss_state->ts.conf.awaiting_adjustment) {
        return VTSS_RC_ERROR;
    }
    SRVL_WR(VTSS_SYS_PTP_TOD_PTP_TOD_MSB, VTSS_F_SYS_PTP_TOD_PTP_TOD_MSB_PTP_TOD_MSB(ts->sec_msb));
    SRVL_WR(VTSS_SYS_PTP_TOD_PTP_TOD_LSB, ts->seconds);
    VTSS_RC(srvl_ts_timeofday_read(vtss_state, &ts_hw));
    VTSS_RC(subTimestamp(&corr, &ts_hw, ts));
    VTSS_RC(srvl_ts_timeofday_offset_set(vtss_state, corr));
    VTSS_D("ts->sec_msb: %u, ts->seconds: %u, ts->nanoseconds: %u", ts->sec_msb, ts->seconds, ts->nanoseconds);
    return VTSS_RC_OK;
}

/*
 * When the time is set, the HW sec  counters are written. The HW clock is adjusted so that the timestamp nanoseconds
 * and the hw nanoseconds counters are aligned. This cannot be done with nanosecond precision, so the final alignment
 * is done by the clock rate adjustment. The hw nanoseconds counter is aligned by setting the
 * PTP_UPPER_LIMIT_1_TIME_ADJ. This register sets the next onesec period to [0.. 1.073.741.823] nanosec. 
 * A value of 0 makes no sense as this means two clock pulses with no time distance, therefore the algorithm sets the
 * period in the range [0,050..1,050] Sec.
 */
static vtss_rc srvl_ts_timeofday_set_delta(vtss_state_t *vtss_state,
                                           const vtss_timestamp_t *ts, BOOL negative)
{
    vtss_ts_conf_t *conf = &vtss_state->ts.conf;
    i32 corr;

    if (conf->awaiting_adjustment) {
        return VTSS_RC_ERROR;
    }
    if (ts->nanoseconds != 0) {
        if (negative) {
            corr = -(i32)ts->nanoseconds;
        } else {
            corr = ts->nanoseconds;
        }        
        VTSS_RC(srvl_ts_timeofday_offset_set(vtss_state, corr));
    }
    conf->delta_sec = ts->seconds;
    conf->delta_sec_msb = ts->sec_msb;
    conf->delta_sign = negative ? 1 : 2;
    
    VTSS_D("ts->sec_msb: %u, ts->seconds: %u, ts->nanoseconds: %u", ts->sec_msb, ts->seconds, ts->nanoseconds);
    return VTSS_RC_OK;
}

/*
 * This function is called from interrupt, therefore the macros using vtss_state cannot be used.
 */
static u32 srvl_ts_ns_cnt_get(vtss_inst_t inst)
{
    /* this code is a copy of the srvl_ts_timeofday_read(ts) function , but without using the MACROS */
    vtss_timestamp_t r1, r2, ts;
    u32 value;
    /* read timestamp registers twice, and verify that no wrapping occurred between two register reads.
       return the value that did not wrap */
    (void)inst->init_conf.reg_read(0, VTSS_DEVCPU_GCB_PTP_STAT_PTP_CURRENT_TIME_STAT, &value);

    (void)inst->init_conf.reg_read(0, VTSS_SYS_PTP_TOD_PTP_TOD_NSEC, &r1.nanoseconds);
    (void)inst->init_conf.reg_read(0, VTSS_SYS_PTP_TOD_PTP_TOD_LSB, &r1.seconds);
    (void)inst->init_conf.reg_read(0, VTSS_SYS_PTP_TOD_PTP_TOD_MSB, &value);
    r1.sec_msb = VTSS_X_SYS_PTP_TOD_PTP_TOD_MSB_PTP_TOD_MSB(value);
    
    (void)inst->init_conf.reg_read(0, VTSS_SYS_PTP_TOD_PTP_TOD_NSEC, &r2.nanoseconds);
    if (r1.nanoseconds < r2.nanoseconds) {
        ts = r1; /* nothing wrapped */
    } else {
        (void)inst->init_conf.reg_read(0, VTSS_SYS_PTP_TOD_PTP_TOD_LSB, &r2.seconds);
        (void)inst->init_conf.reg_read(0, VTSS_SYS_PTP_TOD_PTP_TOD_MSB, &value);
        r2.sec_msb = VTSS_X_SYS_PTP_TOD_PTP_TOD_MSB_PTP_TOD_MSB(value);
        ts = r2; /* nanoseconds wrapped and sec incremented*/
    }
    /* ns value may be >= 1 sec */
    if (ts.nanoseconds >= VTSS_ONE_MIA) {
        ts.nanoseconds -= VTSS_ONE_MIA;
        if (++ts.seconds == 0) ++ts.sec_msb;
    }
    /* tc is returned as 32 bit value, i.e. (seconds*10**9 + nanoseconds) mod 2*32 */
    /* Any value in sec_msb will not change this value, therefore it is omitted */
    value = ts.nanoseconds + ts.seconds*VTSS_ONE_MIA ;
    return value;
}

static vtss_rc srvl_ts_timeofday_one_sec(vtss_state_t *vtss_state)
{
    vtss_ts_conf_t *conf = &vtss_state->ts.conf;
    u32 value, result, value_msb;
    i32 delta_msb = 0;
    u32 reg;

    /* update Delta Sec counter */
    if (conf->delta_sign != 0) {
        SRVL_RD(VTSS_SYS_PTP_TOD_PTP_TOD_LSB, &value);
        SRVL_RD(VTSS_SYS_PTP_TOD_PTP_TOD_MSB, &value_msb);
        
        if (conf->delta_sign == 1) {
            if (value < conf->delta_sec) {
                value_msb--;
            }
            result = value - conf->delta_sec;
            value_msb -= conf->delta_sec_msb;
        } else {
            result = value + conf->delta_sec;
            if (result < value) {
                value_msb++;
            }
            value_msb += conf->delta_sec_msb;
        }
        SRVL_WR(VTSS_SYS_PTP_TOD_PTP_TOD_LSB, result);
        SRVL_WR(VTSS_SYS_PTP_TOD_PTP_TOD_MSB, value_msb);
        VTSS_D("delta sec %d, sign %d", conf->delta_sec, conf->delta_sign);
        
        conf->delta_sec = 0;
        conf->delta_sec_msb = 0;
        conf->delta_sign = 0;
    }
    if (conf->awaiting_adjustment) {
        /* For debug: read the internal NS counter */
        SRVL_RD(VTSS_SYS_PTP_TOD_PTP_TOD_NSEC, &value);
        VTSS_D("TOD_NANOSECS %u", value);

        if (conf->outstanding_adjustment != 0) {
            if (value > conf->outstanding_adjustment) {
                VTSS_D("Too large interrupt latency to adjust %u (ns)", value);
            } else {
                /* update Sec counter */
                if (conf->sec_offset != 0) {
                    SRVL_RD(VTSS_SYS_PTP_TOD_PTP_TOD_LSB, &value);
                    if (conf->sec_offset == 2) {
                        if (value == 0) {
                            delta_msb = -1;
                        }
                        --value;
                    }
                    if (conf->sec_offset == 1) {
                        ++value;
                        if (value == 0) {
                            delta_msb = 1;
                        }
                    }
                    SRVL_WR(VTSS_SYS_PTP_TOD_PTP_TOD_LSB, value);
                    if (delta_msb != 0) {
                        SRVL_RD(VTSS_SYS_PTP_TOD_PTP_TOD_MSB, &value);
                        value += delta_msb;
                        SRVL_WR(VTSS_SYS_PTP_TOD_PTP_TOD_MSB, value);
                    }
                    conf->sec_offset = 0;
                }
                /*PTP_UPPER_LIMIT_1_TIME_ADJ = x */
                SRVL_WR(VTSS_DEVCPU_GCB_PTP_CFG_PTP_UPPER_LIMIT_1_TIME_ADJ_CFG, 
                        conf->outstanding_adjustment |
                        VTSS_F_DEVCPU_GCB_PTP_CFG_PTP_UPPER_LIMIT_1_TIME_ADJ_CFG_PTP_UPPER_LIMIT_1_TIME_ADJ_SHOT);
                VTSS_D("onetime adjustment done %u", conf->outstanding_adjustment);
                SRVL_RD(VTSS_DEVCPU_GCB_PTP_CFG_PTP_UPPER_LIMIT_1_TIME_ADJ_CFG,&reg);
                VTSS_D("1_TIME_ADJ_CFG reg %x", reg);
                if (conf->outstanding_adjustment < 0x20000000) {
                    /* sec counter in serval does not increment if ns counter wraps before bit 29 is set */
                    conf->delta_sec = 1;
                    conf->delta_sign = 2;
                }
                conf->outstanding_adjustment = 0;
            }
        } else {
            conf->awaiting_adjustment = FALSE;
            VTSS_D("awaiting_adjustment cleared");
        }
    }
    return VTSS_RC_OK;
}

static vtss_rc srvl_ts_adjtimer_set(vtss_state_t *vtss_state)
{
    /* adj in units of 0,1 pbb */
    vtss_ts_conf_t *conf = &vtss_state->ts.conf;
    i32 adj = conf->adj;
    u32 clk_adj = HW_CLK_CNT_PR_SEC+1;
    u32 clk_adj_cfg = 0;
    i64 divisor = 0;
    
    /* CLK_ADJ = clock_rate/|adj|
    * CLK_ADJ_DIR = adj>0 ? Positive : Negative
    */
    if (adj != 0) {
        divisor =  VTSS_DIV64((i64)HW_CLK_CNT_PR_SEC*(i64)ADJ_UNITS_PR_NS, (i64)conf->adj);
        if (divisor > (i64)HW_CLK_CNT_PR_SEC || divisor < -(i64)HW_CLK_CNT_PR_SEC) divisor = (i64)HW_CLK_CNT_PR_SEC+1;
        clk_adj = VTSS_LABS((i32)divisor);
    }
    conf->adj_divisor = (i32)divisor;

    if ((adj != 0)) {
        clk_adj_cfg |= VTSS_F_DEVCPU_GCB_PTP_CFG_CLK_ADJ_CFG_CLK_ADJ_ENA;
        SRVL_WRM_SET(VTSS_DEVCPU_GCB_PTP_CFG_GEN_EXT_CLK_CFG(0), VTSS_F_DEVCPU_GCB_PTP_CFG_GEN_EXT_CLK_CFG_GEN_EXT_CLK_ADJ_ENA);
        SRVL_WRM_SET(VTSS_DEVCPU_GCB_PTP_CFG_GEN_EXT_CLK_CFG(1), VTSS_F_DEVCPU_GCB_PTP_CFG_GEN_EXT_CLK_CFG_GEN_EXT_CLK_ADJ_ENA);
    } else {
        SRVL_WRM_CLR(VTSS_DEVCPU_GCB_PTP_CFG_GEN_EXT_CLK_CFG(0), VTSS_F_DEVCPU_GCB_PTP_CFG_GEN_EXT_CLK_CFG_GEN_EXT_CLK_ADJ_ENA);
        SRVL_WRM_CLR(VTSS_DEVCPU_GCB_PTP_CFG_GEN_EXT_CLK_CFG(1), VTSS_F_DEVCPU_GCB_PTP_CFG_GEN_EXT_CLK_CFG_GEN_EXT_CLK_ADJ_ENA);
    }
    if (adj < 0) {
        clk_adj_cfg |= VTSS_F_DEVCPU_GCB_PTP_CFG_CLK_ADJ_CFG_CLK_ADJ_DIR;
    } 
    SRVL_WR(VTSS_DEVCPU_GCB_PTP_CFG_CLK_ADJ_CFG, clk_adj_cfg);
    SRVL_WR(VTSS_DEVCPU_GCB_PTP_CFG_CLK_ADJ_FRQ, clk_adj);
    
    VTSS_D("adj: %d, CLK_ACJ_CFG: %x", adj, clk_adj);
    return VTSS_RC_OK;
}

/* This function assumes that instance 0 of the EXT_SYNC_CURRENT_TIME_STAT is used*/
static vtss_rc srvl_ts_freq_offset_get(vtss_state_t *vtss_state, i32 *const adj)
{
    i32 offset; /* frequency offset in clock counts pr sec */
    u32 cu;
    u32 lv;
    SRVL_RD(VTSS_DEVCPU_GCB_PTP_STAT_EXT_SYNC_CURRENT_TIME_STAT(0),&cu);
    cu = VTSS_X_DEVCPU_GCB_PTP_STAT_EXT_SYNC_CURRENT_TIME_STAT_EXT_SYNC_CURRENT_TIME(cu);
    /* the saved value is 3 clock cycles after the l/s, and the load value seems to be 4 cycles delayed, 
       i.e. the saved value is expected to be load value + 10^9 + a clock cycle */
    cu += EXT_SYNC_INPUT_LATCH_LATENCY;
    /* compensate for load value. i.e. the ns counter is expected to run from 'lv' to 10^9+'lv' - 1clock cycle*/
    SRVL_RD(VTSS_DEVCPU_GCB_PTP_CFG_PTP_EXT_SYNC_TIME_CFG(0), &lv);
    cu -= lv;
    offset = (i32)(cu) - (i32)HW_NS_PR_SEC + 1; /* frequency offset in ppb */
    /* convert to units of 0,1 ppb */
    VTSS_D("cu: %u adj: %d", cu, *adj);
    *adj = ADJ_UNITS_PR_NS*offset;
    VTSS_D("cu: %u adj: %d, lv: %u", cu, *adj, lv);
    return VTSS_RC_OK;
}


/* Set the clock mode for either idx 0 (GPIO pin 30,31) or idx 1 (GPIO pin 15,16) */
/* idx 0 = the alternative clock instance (PPS0), on the Ref board called RS422_1PPS
 * idx 1 = the default clock instance (PPS1), on the Ref board called SyncE1PPS, and also connected to the Tesla PHY's
 */
static vtss_rc srvl_ts_external_clock_mode_set(vtss_state_t *vtss_state, int idx)
{
    u32 dividers;
    u32 high_div;
    u32 low_div;
    u32 cfg_bit_field, ri;
    u16 gpio_ld_st, gpio_ext_clk_out;
    vtss_ts_ext_clock_mode_t *ext_clock_mode;
    vtss_gpio_mode_t gpio_alternate_mode;
    if (idx < 0 || idx > 1) {
        VTSS_E("Invalid external clock index: %d", idx);
        return VTSS_RC_ERROR;
    }
    if (idx == 0) {
        ext_clock_mode = &vtss_state->ts.conf.ext_clock_mode_alt;
        cfg_bit_field = 0x1;
        ri = 0;
        gpio_ld_st = 30;
        gpio_ext_clk_out = 31;
        gpio_alternate_mode = VTSS_GPIO_ALT_0;
    } else {
        ext_clock_mode = &vtss_state->ts.conf.ext_clock_mode;
        cfg_bit_field = 0x2;
        ri = 1;
        gpio_ld_st = 15;
        gpio_ext_clk_out = 16;
        gpio_alternate_mode = VTSS_GPIO_ALT_1;
    }
    if (!ext_clock_mode->enable && 
            (ext_clock_mode->one_pps_mode == TS_EXT_CLOCK_MODE_ONE_PPS_DISABLE)) {
        SRVL_WRM_CLR(VTSS_DEVCPU_GCB_PTP_CFG_PTP_MISC_CFG,
                    VTSS_F_DEVCPU_GCB_PTP_CFG_PTP_MISC_CFG_EXT_SYNC_INP_ENA(cfg_bit_field));
        /* disable clock output, disable 1PPS */
        SRVL_WRM_CLR(VTSS_DEVCPU_GCB_PTP_CFG_GEN_EXT_CLK_CFG(ri), 
                     VTSS_F_DEVCPU_GCB_PTP_CFG_GEN_EXT_CLK_CFG_GEN_EXT_CLK_ENA |
                     VTSS_F_DEVCPU_GCB_PTP_CFG_GEN_EXT_CLK_CFG_GEN_EXT_CLK_SYNC_ENA);
        SRVL_WRM_CLR(VTSS_DEVCPU_GCB_PTP_CFG_PTP_MISC_CFG,
                     VTSS_F_DEVCPU_GCB_PTP_CFG_PTP_MISC_CFG_EXT_SYNC_OUTP_SEL(cfg_bit_field) |
                     VTSS_F_DEVCPU_GCB_PTP_CFG_PTP_MISC_CFG_EXT_SYNC_OUTP_ENA(cfg_bit_field) |
                     VTSS_F_DEVCPU_GCB_PTP_CFG_PTP_MISC_CFG_EXT_SYNC_INP_ENA(cfg_bit_field) |
                     VTSS_F_DEVCPU_GCB_PTP_CFG_PTP_MISC_CFG_EXT_SYNC_CAP_ENA(cfg_bit_field) |
                     VTSS_F_DEVCPU_GCB_PTP_CFG_PTP_MISC_CFG_EXT_SYNC_ENA(cfg_bit_field));
        /* both GPIO pins are unused: disable alternate  for both pins. */
        (void) vtss_srvl_gpio_mode(vtss_state, 0, gpio_ld_st, VTSS_GPIO_IN);
        (void) vtss_srvl_gpio_mode(vtss_state, 0, gpio_ext_clk_out, VTSS_GPIO_IN);
    } else if (ext_clock_mode->enable && 
                (ext_clock_mode->one_pps_mode == TS_EXT_CLOCK_MODE_ONE_PPS_DISABLE)) {
        /* enable clock output, disable 1PPS */
        SRVL_WRM_SET(VTSS_DEVCPU_GCB_PTP_CFG_GEN_EXT_CLK_CFG(ri), 
                     VTSS_F_DEVCPU_GCB_PTP_CFG_GEN_EXT_CLK_CFG_GEN_EXT_CLK_ENA |
                     VTSS_F_DEVCPU_GCB_PTP_CFG_GEN_EXT_CLK_CFG_GEN_EXT_CLK_SYNC_ENA);
        SRVL_WRM_SET(VTSS_DEVCPU_GCB_PTP_CFG_PTP_MISC_CFG,
                     VTSS_F_DEVCPU_GCB_PTP_CFG_PTP_MISC_CFG_EXT_SYNC_OUTP_ENA(cfg_bit_field));
        SRVL_WRM_CLR(VTSS_DEVCPU_GCB_PTP_CFG_PTP_MISC_CFG,
                     VTSS_F_DEVCPU_GCB_PTP_CFG_PTP_MISC_CFG_EXT_SYNC_OUTP_SEL(cfg_bit_field) |
                     VTSS_F_DEVCPU_GCB_PTP_CFG_PTP_MISC_CFG_EXT_SYNC_INP_ENA(cfg_bit_field) |
                     VTSS_F_DEVCPU_GCB_PTP_CFG_PTP_MISC_CFG_EXT_SYNC_CAP_ENA(cfg_bit_field) |
                     VTSS_F_DEVCPU_GCB_PTP_CFG_PTP_MISC_CFG_EXT_SYNC_ENA(cfg_bit_field));
        (void) vtss_srvl_gpio_mode(vtss_state, 0, gpio_ld_st, VTSS_GPIO_IN);
        (void) vtss_srvl_gpio_mode(vtss_state, 0, gpio_ext_clk_out, gpio_alternate_mode);
    } else if (ext_clock_mode->one_pps_mode == TS_EXT_CLOCK_MODE_ONE_PPS_OUTPUT) {
        /* disable clock output, 1 pps output enabled */
        SRVL_WRM_CLR(VTSS_DEVCPU_GCB_PTP_CFG_GEN_EXT_CLK_CFG(ri), 
                     VTSS_F_DEVCPU_GCB_PTP_CFG_GEN_EXT_CLK_CFG_GEN_EXT_CLK_ENA |
                     VTSS_F_DEVCPU_GCB_PTP_CFG_GEN_EXT_CLK_CFG_GEN_EXT_CLK_SYNC_ENA);
        SRVL_WRM_SET(VTSS_DEVCPU_GCB_PTP_CFG_PTP_MISC_CFG,
                     VTSS_F_DEVCPU_GCB_PTP_CFG_PTP_MISC_CFG_EXT_SYNC_OUTP_SEL(cfg_bit_field) |
                     VTSS_F_DEVCPU_GCB_PTP_CFG_PTP_MISC_CFG_EXT_SYNC_OUTP_ENA(cfg_bit_field));
        SRVL_WRM_CLR(VTSS_DEVCPU_GCB_PTP_CFG_PTP_MISC_CFG,
                     VTSS_F_DEVCPU_GCB_PTP_CFG_PTP_MISC_CFG_EXT_SYNC_INP_ENA(cfg_bit_field) |
                     VTSS_F_DEVCPU_GCB_PTP_CFG_PTP_MISC_CFG_EXT_SYNC_CAP_ENA(cfg_bit_field) |
                     VTSS_F_DEVCPU_GCB_PTP_CFG_PTP_MISC_CFG_EXT_SYNC_ENA(cfg_bit_field));
        /* enable alternate mode for clock out. */
        (void) vtss_srvl_gpio_mode(vtss_state, 0, gpio_ld_st, VTSS_GPIO_IN);
        (void) vtss_srvl_gpio_mode(vtss_state, 0, gpio_ext_clk_out, gpio_alternate_mode);
    } else if (!ext_clock_mode->enable && 
               (ext_clock_mode->one_pps_mode == TS_EXT_CLOCK_MODE_ONE_PPS_INPUT)) {
        /* disable clock output, 1 pps input enabled */
        SRVL_WRM_CLR(VTSS_DEVCPU_GCB_PTP_CFG_GEN_EXT_CLK_CFG(ri), 
                     VTSS_F_DEVCPU_GCB_PTP_CFG_GEN_EXT_CLK_CFG_GEN_EXT_CLK_ENA |
                     VTSS_F_DEVCPU_GCB_PTP_CFG_GEN_EXT_CLK_CFG_GEN_EXT_CLK_SYNC_ENA);
        SRVL_WRM_SET(VTSS_DEVCPU_GCB_PTP_CFG_PTP_MISC_CFG,
                     VTSS_F_DEVCPU_GCB_PTP_CFG_PTP_MISC_CFG_EXT_SYNC_INP_ENA(cfg_bit_field) |
                     VTSS_F_DEVCPU_GCB_PTP_CFG_PTP_MISC_CFG_EXT_SYNC_CAP_ENA(cfg_bit_field) |
                     VTSS_F_DEVCPU_GCB_PTP_CFG_PTP_MISC_CFG_EXT_SYNC_ENA(cfg_bit_field));
        SRVL_WRM_CLR(VTSS_DEVCPU_GCB_PTP_CFG_PTP_MISC_CFG,
                     VTSS_F_DEVCPU_GCB_PTP_CFG_PTP_MISC_CFG_EXT_SYNC_OUTP_SEL(cfg_bit_field) |
                     VTSS_F_DEVCPU_GCB_PTP_CFG_PTP_MISC_CFG_EXT_SYNC_OUTP_ENA(cfg_bit_field));
        /* enable alternate mode for load/store */
        (void) vtss_srvl_gpio_mode(vtss_state, 0, gpio_ld_st, gpio_alternate_mode);
        (void) vtss_srvl_gpio_mode(vtss_state, 0, gpio_ext_clk_out, VTSS_GPIO_IN);
    } else if (ext_clock_mode->enable && 
                (ext_clock_mode->one_pps_mode != TS_EXT_CLOCK_MODE_ONE_PPS_INPUT)) {
        /* enable clock output, 1 pps input enabled */
        SRVL_WRM_SET(VTSS_DEVCPU_GCB_PTP_CFG_GEN_EXT_CLK_CFG(ri), 
                     VTSS_F_DEVCPU_GCB_PTP_CFG_GEN_EXT_CLK_CFG_GEN_EXT_CLK_ENA |
                     VTSS_F_DEVCPU_GCB_PTP_CFG_GEN_EXT_CLK_CFG_GEN_EXT_CLK_SYNC_ENA);
        SRVL_WRM_SET(VTSS_DEVCPU_GCB_PTP_CFG_PTP_MISC_CFG,
                     VTSS_F_DEVCPU_GCB_PTP_CFG_PTP_MISC_CFG_EXT_SYNC_INP_ENA(cfg_bit_field) |
                     VTSS_F_DEVCPU_GCB_PTP_CFG_PTP_MISC_CFG_EXT_SYNC_CAP_ENA(cfg_bit_field) |
                     VTSS_F_DEVCPU_GCB_PTP_CFG_PTP_MISC_CFG_EXT_SYNC_ENA(cfg_bit_field));
        SRVL_WRM_CLR(VTSS_DEVCPU_GCB_PTP_CFG_PTP_MISC_CFG,
                     VTSS_F_DEVCPU_GCB_PTP_CFG_PTP_MISC_CFG_EXT_SYNC_OUTP_SEL(cfg_bit_field) |
                     VTSS_F_DEVCPU_GCB_PTP_CFG_PTP_MISC_CFG_EXT_SYNC_OUTP_ENA(cfg_bit_field));
            /* enable alternate mode for clock out. */
        (void) vtss_srvl_gpio_mode(vtss_state, 0, gpio_ld_st, gpio_alternate_mode);
        (void) vtss_srvl_gpio_mode(vtss_state, 0, gpio_ext_clk_out, gpio_alternate_mode);
    }
    if (ext_clock_mode->enable) {
        /* set dividers; enable clock output. */
        dividers = (HW_CLK_CNT_PR_SEC/(ext_clock_mode->freq));
        high_div = (dividers/2)-1;
        low_div =  ((dividers+1)/2)-1;
        SRVL_WR(VTSS_DEVCPU_GCB_PTP_CFG_GEN_EXT_CLK_HIGH_PERIOD_CFG(ri), high_div  &
               VTSS_M_DEVCPU_GCB_PTP_CFG_GEN_EXT_CLK_HIGH_PERIOD_CFG_GEN_EXT_CLK_HIGH_PERIOD);
        SRVL_WR(VTSS_DEVCPU_GCB_PTP_CFG_GEN_EXT_CLK_LOW_PERIOD_CFG(ri), low_div  &
               VTSS_M_DEVCPU_GCB_PTP_CFG_GEN_EXT_CLK_LOW_PERIOD_CFG_GEN_EXT_CLK_LOW_PERIOD);
    }
    return VTSS_RC_OK;
}

/* Get the saved nanosec conuter from counter */
static vtss_rc srvl_ts_alt_clock_saved_get(vtss_state_t *vtss_state, u32 *const saved)
{
    SRVL_RD(VTSS_DEVCPU_GCB_PTP_STAT_EXT_SYNC_CURRENT_TIME_STAT(0), saved);
    *saved = VTSS_X_DEVCPU_GCB_PTP_STAT_EXT_SYNC_CURRENT_TIME_STAT_EXT_SYNC_CURRENT_TIME(*saved);
    return VTSS_RC_OK;
}

/* Set the clock mode for external clock idx 0 (GPIO pin 30,31) */
/* idx 0 = the alternative clock instance (PPS0), on the Ref board called RS422_1PPS
 * the modes are: 
 *    BOOL one_pps_out;        Enable 1pps output
 *    BOOL one_pps_in;         Enable 1pps input
 *    BOOL save;               Save actual time counter at next 1 PPS input
 *    BOOL load;               Load actual time counter at next 1 PPS input
 * Register setting in PTP_MISC_CFG:
 *                     one_pps_out | one_pps_in |   save   |   load
 *                        T    F   |   T    F   |  T    F  |  T    F  |
 * EXT_SYNC_OUTP_SEL      1    0
 * EXT_SYNC_OUTP_ENA      1    0
 * EXT_SYNC_INP_ENA                    1    0
 * EXT_SYNC_CAP_ENA                                1    0
 * EXT_SYNC_ENA                                               1    0
 */
static vtss_rc srvl_ts_alt_clock_mode_set(vtss_state_t *vtss_state)
{
    u32 cfg_bit_field, ri;
    u16 gpio_ld_st, gpio_ext_clk_out;
    vtss_ts_alt_clock_mode_t       *alt_clock_mode;
    vtss_gpio_mode_t gpio_alternate_mode;
    
    alt_clock_mode = &vtss_state->ts.conf.alt_clock_mode;
    cfg_bit_field = 0x1;
    ri = 0;
    gpio_ld_st = 30;
    gpio_ext_clk_out = 31;
    gpio_alternate_mode = VTSS_GPIO_ALT_0;
    
    SRVL_WRM_CLR(VTSS_DEVCPU_GCB_PTP_CFG_GEN_EXT_CLK_CFG(ri), 
                 VTSS_F_DEVCPU_GCB_PTP_CFG_GEN_EXT_CLK_CFG_GEN_EXT_CLK_ENA |
                 VTSS_F_DEVCPU_GCB_PTP_CFG_GEN_EXT_CLK_CFG_GEN_EXT_CLK_SYNC_ENA);
    if (alt_clock_mode->one_pps_out) {
        /*  1 pps output enabled */
        SRVL_WRM_SET(VTSS_DEVCPU_GCB_PTP_CFG_PTP_MISC_CFG,
                     VTSS_F_DEVCPU_GCB_PTP_CFG_PTP_MISC_CFG_EXT_SYNC_OUTP_SEL(cfg_bit_field) |
                     VTSS_F_DEVCPU_GCB_PTP_CFG_PTP_MISC_CFG_EXT_SYNC_OUTP_ENA(cfg_bit_field));
        SRVL_WRM_CLR(VTSS_DEVCPU_GCB_PTP_CFG_PTP_MISC_CFG,
                     VTSS_F_DEVCPU_GCB_PTP_CFG_PTP_MISC_CFG_EXT_SYNC_ENA(cfg_bit_field));
        /* enable alternate mode for clock out. */
        (void) vtss_srvl_gpio_mode(vtss_state, 0, gpio_ext_clk_out, gpio_alternate_mode);
    } else {
        SRVL_WRM_CLR(VTSS_DEVCPU_GCB_PTP_CFG_PTP_MISC_CFG,
                     VTSS_F_DEVCPU_GCB_PTP_CFG_PTP_MISC_CFG_EXT_SYNC_OUTP_SEL(cfg_bit_field) |
                     VTSS_F_DEVCPU_GCB_PTP_CFG_PTP_MISC_CFG_EXT_SYNC_OUTP_ENA(cfg_bit_field));
        SRVL_WRM_SET(VTSS_DEVCPU_GCB_PTP_CFG_PTP_MISC_CFG,
                     VTSS_F_DEVCPU_GCB_PTP_CFG_PTP_MISC_CFG_EXT_SYNC_ENA(cfg_bit_field));
        /* disable alternate mode for clock out. */
        (void) vtss_srvl_gpio_mode(vtss_state, 0, gpio_ext_clk_out, VTSS_GPIO_IN);
    }
        
    if (alt_clock_mode->one_pps_in) {
        /* L/S input enabled */
        SRVL_WRM_SET(VTSS_DEVCPU_GCB_PTP_CFG_PTP_MISC_CFG,
                     VTSS_F_DEVCPU_GCB_PTP_CFG_PTP_MISC_CFG_EXT_SYNC_INP_ENA(cfg_bit_field));
        /* enable alternate mode for load/store */
        (void) vtss_srvl_gpio_mode(vtss_state, 0, gpio_ld_st, gpio_alternate_mode);
    } else {
        /* disable clock L/S input */
        SRVL_WRM_CLR(VTSS_DEVCPU_GCB_PTP_CFG_PTP_MISC_CFG,
                     VTSS_F_DEVCPU_GCB_PTP_CFG_PTP_MISC_CFG_EXT_SYNC_INP_ENA(cfg_bit_field));
        /* enable alternate mode for clock out. */
        (void) vtss_srvl_gpio_mode(vtss_state, 0, gpio_ld_st, VTSS_GPIO_IN);
    }
    if (alt_clock_mode->save) {
        /* save actual time at L/S input */
        SRVL_WRM_SET(VTSS_DEVCPU_GCB_PTP_CFG_PTP_MISC_CFG,
                     VTSS_F_DEVCPU_GCB_PTP_CFG_PTP_MISC_CFG_EXT_SYNC_CAP_ENA(cfg_bit_field));
    } else {
        SRVL_WRM_CLR(VTSS_DEVCPU_GCB_PTP_CFG_PTP_MISC_CFG,
                     VTSS_F_DEVCPU_GCB_PTP_CFG_PTP_MISC_CFG_EXT_SYNC_CAP_ENA(cfg_bit_field));
    }
    if (alt_clock_mode->load) {
        /*  load enabled */
        SRVL_WRM_SET(VTSS_DEVCPU_GCB_PTP_CFG_PTP_MISC_CFG,
                     VTSS_F_DEVCPU_GCB_PTP_CFG_PTP_MISC_CFG_EXT_SYNC_ENA(cfg_bit_field));
    } else {
        SRVL_WRM_CLR(VTSS_DEVCPU_GCB_PTP_CFG_PTP_MISC_CFG,
                     VTSS_F_DEVCPU_GCB_PTP_CFG_PTP_MISC_CFG_EXT_SYNC_ENA(cfg_bit_field));
    }
    return VTSS_RC_OK;
}

/* Set the time to be loaded into the Serval timer at the next 1PPS
 * It is assumed that this function is called at the beginning of a sec */
static vtss_rc srvl_ts_timeofday_next_pps_set(vtss_state_t *vtss_state,
                                              const vtss_timestamp_t *const ts)
{
    /* warn that the time is being set, awaiting_adjustment is cleared at the next 1PPS */
    vtss_state->ts.conf.awaiting_adjustment = TRUE;
    vtss_state->ts.conf.outstanding_adjustment = 0;
    SRVL_WR(VTSS_SYS_PTP_TOD_PTP_TOD_MSB, VTSS_F_SYS_PTP_TOD_PTP_TOD_MSB_PTP_TOD_MSB(ts->sec_msb));
    SRVL_WR(VTSS_SYS_PTP_TOD_PTP_TOD_LSB, ts->seconds-1);
    SRVL_WR(VTSS_DEVCPU_GCB_PTP_CFG_PTP_EXT_SYNC_TIME_CFG(0), ts->nanoseconds);
    VTSS_D("time at next pps: sec_msb %u, sec %u, ns %u", ts->sec_msb, ts->seconds, ts->nanoseconds);
    return VTSS_RC_OK;
}

static vtss_rc srvl_ts_ingress_latency_set(vtss_state_t *vtss_state, vtss_port_no_t port_no)
{
    vtss_ts_port_conf_t *conf = &vtss_state->ts.port_conf[port_no];
    i32 rx_delay = 0;

    if (vtss_state->port.conf[port_no].loop != VTSS_PORT_LOOP_PCS_HOST) {   /* When loop is enabled on the port the latency must be zero */
        rx_delay = VTSS_INTERVAL_NS(conf->ingress_latency);
        rx_delay += -TIMESTAMPER_SKEW + conf->default_igr_latency;
        if ((rx_delay < -TIMESTAMPER_SKEW) || (rx_delay > 0x7ff)) {
            VTSS_D("Invalid ingress latency %d", rx_delay);
            return VTSS_RC_ERROR;
        }
        /* the RX_PATH_DELAY is added to the rx timestamp, therefore it is -ingress_latency */
        rx_delay = -rx_delay;
    }

    VTSS_D("rx_delay %d", rx_delay);
    SRVL_WRM(VTSS_SYS_SYSTEM_IO_PATH_DELAY(VTSS_CHIP_PORT(port_no)), 
            VTSS_F_SYS_SYSTEM_IO_PATH_DELAY_RX_PATH_DELAY(rx_delay), 
            VTSS_M_SYS_SYSTEM_IO_PATH_DELAY_RX_PATH_DELAY);
    return VTSS_RC_OK;
}

static vtss_rc srvl_ts_p2p_delay_set(vtss_state_t *vtss_state, vtss_port_no_t port_no)
{
    vtss_ts_port_conf_t *conf = &vtss_state->ts.port_conf[port_no];
    i32 p2p_delay = VTSS_INTERVAL_NS(conf->p2p_delay);
    p2p_delay += VTSS_INTERVAL_NS(conf->delay_asymmetry);
    SRVL_WR(VTSS_ANA_PORT_PTP_DLY2_CFG(VTSS_CHIP_PORT(port_no)), 
             p2p_delay);
    return VTSS_RC_OK;
}

static vtss_rc srvl_ts_egress_latency_set(vtss_state_t *vtss_state, vtss_port_no_t port_no)
{
    vtss_ts_port_conf_t *conf = &vtss_state->ts.port_conf[port_no];
    i32 tx_delay = 0;

    if (vtss_state->port.conf[port_no].loop != VTSS_PORT_LOOP_PCS_HOST) {   /* When loop is enabled on the port the latency must be zero */
        tx_delay = VTSS_INTERVAL_NS(conf->egress_latency);
        tx_delay += TIMESTAMPER_SKEW + conf->default_egr_latency;
        if ((tx_delay < 0) || (tx_delay > 0x7ff)) {
            VTSS_D("Invalid egress latency %d", tx_delay);
            return VTSS_RC_ERROR;
        }
    }

    VTSS_D("tx_delay %d", tx_delay);
    SRVL_WRM(VTSS_SYS_SYSTEM_IO_PATH_DELAY(VTSS_CHIP_PORT(port_no)), 
             VTSS_F_SYS_SYSTEM_IO_PATH_DELAY_TX_PATH_DELAY(tx_delay), 
             VTSS_M_SYS_SYSTEM_IO_PATH_DELAY_TX_PATH_DELAY);
    return VTSS_RC_OK;
}

static vtss_rc srvl_ts_delay_asymmetry_set(vtss_state_t *vtss_state, vtss_port_no_t port_no)
{
    vtss_ts_port_conf_t *conf = &vtss_state->ts.port_conf[port_no];
    i32 delay = VTSS_INTERVAL_NS(conf->delay_asymmetry);
    /* Used for Ingress asymmetry compensation (Pdelay_Resp) */
    SRVL_WR(VTSS_ANA_PORT_PTP_DLY1_CFG(VTSS_CHIP_PORT(port_no)), 
            delay);
    /* Used for Egress asymmetry compensation (on Delay_Req and Pdelay_Req) */
    SRVL_WR(VTSS_REW_PORT_PTP_DLY1_CFG(VTSS_CHIP_PORT(port_no)), 
            -delay);
    /* Used for Ingress asymmetry compensation (on Sync, therefore include p2p_delay ) */
    delay += VTSS_INTERVAL_NS(conf->p2p_delay);
    SRVL_WR(VTSS_ANA_PORT_PTP_DLY2_CFG(VTSS_CHIP_PORT(port_no)), 
            delay);
    return VTSS_RC_OK;
}

static vtss_rc srvl_ts_operation_mode_set(vtss_state_t *vtss_state, vtss_port_no_t port_no)
{
    vtss_ts_mode_t mode = vtss_state->ts.port_conf[port_no].mode.mode;
    u32 ana_mode = 0;
    u32 rew_mode = 0;
    if (mode == TS_MODE_INTERNAL) {
        ana_mode = VTSS_F_ANA_PORT_PTP_CFG_PTP_BACKPLANE_MODE;
        rew_mode = VTSS_F_REW_PORT_PTP_CFG_PTP_BACKPLANE_MODE;
    }
    SRVL_WRM(VTSS_ANA_PORT_PTP_CFG(VTSS_CHIP_PORT(port_no)),
             ana_mode,
             VTSS_F_ANA_PORT_PTP_CFG_PTP_BACKPLANE_MODE);
    SRVL_WRM(VTSS_REW_PORT_PTP_CFG(VTSS_CHIP_PORT(port_no)), 
             rew_mode, 
             VTSS_F_REW_PORT_PTP_CFG_PTP_BACKPLANE_MODE);
    return VTSS_RC_OK;
}

static vtss_rc srvl_ts_internal_mode_set(vtss_state_t *vtss_state)
{
    vtss_ts_internal_fmt_t fmt = vtss_state->ts.int_mode.int_fmt;
    u32 stamp_wid = 30;
    u32 roll_mode = 0;
    switch (fmt) {
        case TS_INTERNAL_FMT_RESERVED_LEN_30BIT:
            stamp_wid = 30;
            roll_mode = 0;
            break;
        case TS_INTERNAL_FMT_RESERVED_LEN_32BIT:
            stamp_wid = 32;
            roll_mode = 0;
            break;
        case TS_INTERNAL_FMT_SUB_ADD_LEN_44BIT_CF62:
            stamp_wid = 44;
            roll_mode = 2;
            break;
        case TS_INTERNAL_FMT_RESERVED_LEN_48BIT_CF_3_0:
            stamp_wid = 48;
            roll_mode = 0;
            break;
        case TS_INTERNAL_FMT_RESERVED_LEN_48BIT_CF_0:
            stamp_wid = 48;
            roll_mode = 1;
            break;
        default:
            VTSS_E("unsupported internal timestamp format: %u", fmt);
            return VTSS_RC_ERROR;
    }
    VTSS_D("timestamp_width %d, roll_mode %d",stamp_wid, roll_mode);
    if (stamp_wid >=44) {
        vtss_state->ts.add_sub_option = TRUE;
    } else {
        vtss_state->ts.add_sub_option = FALSE;
    }
    SRVL_WRM(VTSS_SYS_PTP_PTP_CFG, 
             VTSS_F_SYS_PTP_PTP_CFG_PTP_STAMP_WID(stamp_wid) | VTSS_F_SYS_PTP_PTP_CFG_PTP_CF_ROLL_MODE(roll_mode), 
             VTSS_M_SYS_PTP_PTP_CFG_PTP_STAMP_WID | VTSS_M_SYS_PTP_PTP_CFG_PTP_CF_ROLL_MODE);
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


static vtss_rc srvl_ts_timestamp_get(vtss_state_t *vtss_state)
{
    u32 value;
    u32 delay;
    u32 tx_port;
    u32 mess_id;
    u32 seq_no;
    u32 sec;
    BOOL overflow = FALSE;
    u32 ts_sec;

    /* PTP_STATUS */
    SRVL_RD(VTSS_SYS_PTP_PTP_STATUS, &value);
    while (value & VTSS_F_SYS_PTP_PTP_STATUS_PTP_MESS_VLD) {
        overflow |= (value & VTSS_F_SYS_PTP_PTP_STATUS_PTP_OVFL) ? TRUE : FALSE;
        SRVL_RD(VTSS_SYS_PTP_PTP_TXSTAMP, &delay);
        ts_sec = (delay & VTSS_F_SYS_PTP_PTP_TXSTAMP_PTP_TXSTAMP_SEC) ? 1 : 0;
        delay = VTSS_X_SYS_PTP_PTP_TXSTAMP_PTP_TXSTAMP(delay); /* extract nanoseconds */
        /* Convert the tx timestamp to 32 bit value to make it comparable to the rx timestamp */
        SRVL_RD(VTSS_SYS_PTP_TOD_PTP_TOD_LSB, &sec);
        if ((sec & 0x1) != ts_sec) { // sec has incremented since timestamp was registered
            --sec;
        }
        delay += sec*1000000000L;
        tx_port = api_port(vtss_state, VTSS_X_SYS_PTP_PTP_STATUS_PTP_MESS_TXPORT(value));
        mess_id = VTSS_X_SYS_PTP_PTP_STATUS_PTP_MESS_ID(value);
        seq_no  = VTSS_X_SYS_PTP_PTP_STATUS_PTP_MESS_SEQ_ID(value);
        if (value & VTSS_F_SYS_PTP_PTP_STATUS_PTP_TXSTAMP_OAM) {
#if defined(VTSS_ARCH_SERVAL_CE)
            if (mess_id >= VTSS_OAM_PATH_SERVICE_VOE_CNT) {
                VTSS_E("invalid mess_id (%u)", mess_id);
            } else {
                VTSS_I("OAM timestamps received:");
                if (++vtss_state->ts.oam_ts_status[mess_id].last >= VTSS_SERVAL_MAX_OAM_ENTRIES) vtss_state->ts.oam_ts_status[mess_id].last = 0;
                vtss_state->ts.oam_ts_status[mess_id].entry[vtss_state->ts.oam_ts_status[mess_id].last].tc = delay;
                vtss_state->ts.oam_ts_status[mess_id].entry[vtss_state->ts.oam_ts_status[mess_id].last].id = mess_id;
                vtss_state->ts.oam_ts_status[mess_id].entry[vtss_state->ts.oam_ts_status[mess_id].last].sq = seq_no;
                vtss_state->ts.oam_ts_status[mess_id].entry[vtss_state->ts.oam_ts_status[mess_id].last].port = tx_port;
                vtss_state->ts.oam_ts_status[mess_id].entry[vtss_state->ts.oam_ts_status[mess_id].last].valid = TRUE;
                VTSS_I(" port %d, id %d, tc %u, sq %u", tx_port, mess_id, delay, seq_no);
            }
#endif /* VTSS_ARCH_SERVAL_CE */
        } else {
            if (mess_id >= VTSS_TS_ID_SIZE) {
                VTSS_E("invalid mess_id (%u)", mess_id);
            } else {
                if (tx_port < VTSS_PORT_ARRAY_SIZE) {
                    vtss_state->ts.status[mess_id].tx_tc[tx_port] = delay;
                    vtss_state->ts.status[mess_id].tx_id[tx_port] = mess_id;
                    vtss_state->ts.status[mess_id].tx_sq[tx_port] = seq_no;
                    vtss_state->ts.status[mess_id].valid_mask |= 1LL<<tx_port;
                } else {
                    VTSS_E("invalid port (%u)", tx_port);
                }
            }
        }
        VTSS_D("value %x, delay %u, tx_port %u, mess_id %u, seq_no %u", value, delay, tx_port, mess_id, seq_no);
        SRVL_WR(VTSS_SYS_PTP_PTP_NXT, VTSS_F_SYS_PTP_PTP_NXT_PTP_NXT);
        SRVL_RD(VTSS_SYS_PTP_PTP_STATUS, &value);
    }
    if (overflow ) {
        VTSS_E("Timestamp fifo overflow occurred");
    }
    return VTSS_RC_OK;
}

static vtss_rc srvl_ts_timestamp_id_release(vtss_state_t *vtss_state, u32 ts_id)
{
    /* Clear bit corresponding to ts_id */
    if (ts_id <TS_IDS_RESERVED_FOR_SW) {
        vtss_state->ts.conf.sw_ts_id_res[ts_id] = 0;
    } else if (ts_id <=31) {
        SRVL_WR(VTSS_ANA_ANA_TABLES_PTP_ID_LOW, VTSS_BIT(ts_id));
    } else if (ts_id <=63) {
        SRVL_WR(VTSS_ANA_ANA_TABLES_PTP_ID_HIGH, VTSS_BIT(ts_id-32));
    } else {
        VTSS_D("invalid ts_id %d", ts_id);
    }
    return VTSS_RC_OK;
}

/*
* Signal port status change (used to detect and compensate for the internal ingress and egress latencies)
*/
static vtss_rc srvl_ts_status_change(vtss_state_t         *vtss_state,
                                     const vtss_port_no_t port_no)
{
    vtss_rc rc = VTSS_RC_OK, rc2;
    VTSS_D("port status change %d", port_no);
    vtss_port_speed_t speed = vtss_state->port.conf[port_no].speed;
    u32 speed_index = speed == VTSS_SPEED_10M ? SPEED_INDEX_10M : speed == VTSS_SPEED_100M ? SPEED_INDEX_100M :
                      speed == VTSS_SPEED_1G ? SPEED_INDEX_1000M : speed == VTSS_SPEED_2500M ? SPEED_INDEX_2500M : SPEED_INDEX_MAX;
    if (speed == VTSS_SPEED_UNDEFINED) {
        vtss_state->ts.port_conf[port_no].default_igr_latency = 0;
        vtss_state->ts.port_conf[port_no].default_egr_latency = 0;
    } else if (speed_index < SPEED_INDEX_MAX) {
        vtss_state->ts.port_conf[port_no].default_igr_latency = latency_igr[speed_index];
        vtss_state->ts.port_conf[port_no].default_egr_latency = latency_egr[speed_index];
    } else {
        VTSS_E("invalid port speed %u on port_no %u", speed, port_no);
        return VTSS_RC_ERROR;
    }
    VTSS_D("port_no %d, igr_latency %d, egr_latency %d", port_no, vtss_state->ts.port_conf[port_no].default_igr_latency, vtss_state->ts.port_conf[port_no].default_egr_latency);
    rc = srvl_ts_ingress_latency_set(vtss_state, port_no);
    rc2 = srvl_ts_egress_latency_set(vtss_state, port_no);
    if (rc == VTSS_RC_OK) {
        rc = rc2;
    }
    return rc;
}

/* - Debug print --------------------------------------------------- */

static vtss_rc srvl_debug_ts(vtss_state_t *vtss_state,
                             const vtss_debug_printf_t pr,
                             const vtss_debug_info_t   *const info)
{
    u32  value;
    u32  port;
    int ix;
    char buf[32];

    /* SYS:PTP_TOD */
    vtss_srvl_debug_reg_header(pr, "SYS:PTP_TOD");
    vtss_srvl_debug_reg(vtss_state, pr, VTSS_SYS_PTP_TOD_PTP_TOD_MSB, "PTP_TOD_MSB");
    vtss_srvl_debug_reg(vtss_state, pr, VTSS_SYS_PTP_TOD_PTP_TOD_LSB, "PTP_TOD_LSB");
    vtss_srvl_debug_reg(vtss_state, pr, VTSS_SYS_PTP_TOD_PTP_TOD_NSEC, "PTP_TOD_NSEC");
    /* SYS:PTP */
    vtss_srvl_debug_reg_header(pr, "SYS:PTP");
    vtss_srvl_debug_reg(vtss_state, pr, VTSS_SYS_PTP_PTP_STATUS, "PTP_STATUS");
    vtss_srvl_debug_reg(vtss_state, pr, VTSS_SYS_PTP_PTP_TXSTAMP, "PTP_TXSTAMP");
    vtss_srvl_debug_reg(vtss_state, pr, VTSS_SYS_PTP_PTP_NXT, "PTP_NXT");
    vtss_srvl_debug_reg(vtss_state, pr, VTSS_SYS_PTP_PTP_CFG, "PTP_CFG");
    
    /* DEVCPU_GCB: PTP_CFG */
    vtss_srvl_debug_reg_header(pr, "GCB:PTP_CFG");
    vtss_srvl_debug_reg(vtss_state, pr, VTSS_DEVCPU_GCB_PTP_CFG_PTP_MISC_CFG, "PTP_MISC_CFG");
    vtss_srvl_debug_reg(vtss_state, pr, VTSS_DEVCPU_GCB_PTP_CFG_PTP_UPPER_LIMIT_CFG, "PTP_UPPER_LIMIT_CFG");
    vtss_srvl_debug_reg(vtss_state, pr, VTSS_DEVCPU_GCB_PTP_CFG_PTP_UPPER_LIMIT_1_TIME_ADJ_CFG, "PTP_UPPER_LIMIT_1_TIME_ADJ_CFG");
    vtss_srvl_debug_reg(vtss_state, pr, VTSS_DEVCPU_GCB_PTP_CFG_PTP_SYNC_INTR_ENA_CFG, "PTP_SYNC_INTR_ENA_CFG");
    vtss_srvl_debug_reg(vtss_state, pr, VTSS_DEVCPU_GCB_PTP_CFG_CLK_ADJ_CFG, "CLK_ADJ_CFG");
    vtss_srvl_debug_reg(vtss_state, pr, VTSS_DEVCPU_GCB_PTP_CFG_CLK_ADJ_FRQ, "CLK_ADJ_FRQ");
    for (ix = 0; ix <= 1; ix++) {
        sprintf(buf, "GEN_EXT_CLK_HIGH_PERIOD_CFG_%u", ix);
        vtss_srvl_debug_reg(vtss_state, pr, VTSS_DEVCPU_GCB_PTP_CFG_GEN_EXT_CLK_HIGH_PERIOD_CFG(ix), buf);
        sprintf(buf, "GEN_EXT_CLK_LOW_PERIOD_CFG_%u", ix);
        vtss_srvl_debug_reg(vtss_state, pr, VTSS_DEVCPU_GCB_PTP_CFG_GEN_EXT_CLK_LOW_PERIOD_CFG(ix), buf);
        sprintf(buf, "GEN_EXT_CLK_CFG_%u", ix);
        vtss_srvl_debug_reg(vtss_state, pr, VTSS_DEVCPU_GCB_PTP_CFG_GEN_EXT_CLK_CFG(ix), buf);
        sprintf(buf, "PTP_EXT_SYNC_TIME_CFG_%u", ix);
        vtss_srvl_debug_reg(vtss_state, pr, VTSS_DEVCPU_GCB_PTP_CFG_PTP_EXT_SYNC_TIME_CFG(ix), buf);
    }    
    /* DEVCPU_GCB: PTP_STAT */
    vtss_srvl_debug_reg_header(pr, "GCB:PTP_STAT");
    vtss_srvl_debug_reg(vtss_state, pr, VTSS_DEVCPU_GCB_PTP_STAT_PTP_CURRENT_TIME_STAT, "PTP_CURRENT_TIME_STAT");
    vtss_srvl_debug_reg(vtss_state, pr, VTSS_DEVCPU_GCB_PTP_STAT_EXT_SYNC_CURRENT_TIME_STAT(0), "EXT_SYNC_CURRENT_TIME_STAT_0");
    vtss_srvl_debug_reg(vtss_state, pr, VTSS_DEVCPU_GCB_PTP_STAT_EXT_SYNC_CURRENT_TIME_STAT(1), "EXT_SYNC_CURRENT_TIME_STAT_1");
    vtss_srvl_debug_reg(vtss_state, pr, VTSS_DEVCPU_GCB_PTP_STAT_PTP_EVT_STAT, "PTP_EVT_STAT");

    SRVL_RD(VTSS_DEVCPU_GCB_PTP_STAT_PTP_EVT_STAT, &value);
    SRVL_WR(VTSS_DEVCPU_GCB_PTP_STAT_PTP_EVT_STAT, value);
    vtss_debug_print_sticky(pr, "CLK_ADJ_UPD_STICKY", value, VTSS_F_DEVCPU_GCB_PTP_STAT_PTP_EVT_STAT_CLK_ADJ_UPD_STICKY);
    vtss_debug_print_sticky(pr, "EXT_SYNC_CURRENT_TIME_STICKY_0", value, VTSS_F_DEVCPU_GCB_PTP_STAT_PTP_EVT_STAT_EXT_SYNC_CURRENT_TIME_STICKY(0x1));
    vtss_debug_print_sticky(pr, "EXT_SYNC_CURRENT_TIME_STICKY_1", value, VTSS_F_DEVCPU_GCB_PTP_STAT_PTP_EVT_STAT_EXT_SYNC_CURRENT_TIME_STICKY(0x2));
    vtss_debug_print_sticky(pr, "SYNC_STAT", value, VTSS_F_DEVCPU_GCB_PTP_STAT_PTP_EVT_STAT_SYNC_STAT);
    
    /* SYS_PORT:PTP */
    vtss_srvl_debug_reg_header(pr, "REW:PORT");
    for (port = 0; port <= VTSS_CHIP_PORTS; port++) {
        sprintf(buf, "PTP_CFG_%u", port);
        vtss_srvl_debug_reg(vtss_state, pr, VTSS_REW_PORT_PTP_CFG(port), buf);
        sprintf(buf, "PTP_DLY1_CFG_%u", port);
        vtss_srvl_debug_reg(vtss_state, pr, VTSS_REW_PORT_PTP_DLY1_CFG(port), buf);
    }
    /* SYS_SWITCH:PTP */
    vtss_srvl_debug_reg_header(pr, "SWITCH:PORT");
    for (port = 0; port <= VTSS_CHIP_PORTS; port++) {
        sprintf(buf, "IO_PATH_DELAY_%u", port);
        vtss_srvl_debug_reg(vtss_state, pr, VTSS_SYS_SYSTEM_IO_PATH_DELAY(port), buf);
    }
    /* ANA_PORT:PTP */
    vtss_srvl_debug_reg_header(pr, "ANA:PORT");
    for (port = 0; port <= VTSS_CHIP_PORTS; port++) {
        sprintf(buf, "PTP_CFG_%u", port);
        vtss_srvl_debug_reg(vtss_state, pr, VTSS_ANA_PORT_PTP_CFG(port), buf);
        sprintf(buf, "PTP_DLY1_CFG_%u", port);
        vtss_srvl_debug_reg(vtss_state, pr, VTSS_ANA_PORT_PTP_DLY1_CFG(port), buf);
        sprintf(buf, "PTP_DLY2_CFG_%u", port);
        vtss_srvl_debug_reg(vtss_state, pr, VTSS_ANA_PORT_PTP_DLY2_CFG(port), buf);
    }

    /* ANA:: */
    vtss_srvl_debug_reg_header(pr, "ANA::PTP");
    vtss_srvl_debug_reg(vtss_state, pr, VTSS_ANA_ANA_TABLES_PTP_ID_HIGH, "PTP_ID_HIGH");
    vtss_srvl_debug_reg(vtss_state, pr, VTSS_ANA_ANA_TABLES_PTP_ID_LOW, "PTP_ID_LOW");
    
    
    pr("\n");

    return VTSS_RC_OK;
}

vtss_rc vtss_srvl_ts_debug_print(vtss_state_t *vtss_state,
                                 const vtss_debug_printf_t pr,
                                 const vtss_debug_info_t   *const info)
{
    return vtss_debug_print_group(VTSS_DEBUG_GROUP_TS, srvl_debug_ts, vtss_state, pr, info);
}

/* - Initialization ------------------------------------------------ */

static vtss_rc srvl_ts_init(vtss_state_t *vtss_state)
{
    SRVL_WR(VTSS_DEVCPU_GCB_PTP_CFG_PTP_MISC_CFG, VTSS_F_DEVCPU_GCB_PTP_CFG_PTP_MISC_CFG_PTP_ENA);
    /* release all timestamp id's except those that are reserved for SW*/
    SRVL_WR(VTSS_ANA_ANA_TABLES_PTP_ID_LOW, VTSS_ENCODE_BITMASK(TS_IDS_RESERVED_FOR_SW,32-TS_IDS_RESERVED_FOR_SW));
    SRVL_WR(VTSS_ANA_ANA_TABLES_PTP_ID_HIGH, 0xffffffff); /* assuming 32 are reserved for HW */
    /* pr default use 30 bit timestamping in the reserved field in the PTP packets (backplane mode) */
    SRVL_WRM(VTSS_SYS_PTP_PTP_CFG, VTSS_F_SYS_PTP_PTP_CFG_PTP_STAMP_WID(30), VTSS_M_SYS_PTP_PTP_CFG_PTP_STAMP_WID);

    return VTSS_RC_OK;
}

vtss_rc vtss_srvl_ts_init(vtss_state_t *vtss_state, vtss_init_cmd_t cmd)
{
    vtss_ts_state_t *state = &vtss_state->ts;

    switch (cmd) {
    case VTSS_INIT_CMD_CREATE:
        state->timeofday_get = srvl_ts_timeofday_get;
        state->timeofday_set = srvl_ts_timeofday_set;
        state->timeofday_set_delta = srvl_ts_timeofday_set_delta;
        state->timeofday_next_pps_get = srvl_ts_timeofday_next_pps_get;
        state->timeofday_offset_set = srvl_ts_timeofday_offset_set;
        state->ns_cnt_get = srvl_ts_ns_cnt_get;
        state->timeofday_one_sec = srvl_ts_timeofday_one_sec;
        state->adjtimer_set = srvl_ts_adjtimer_set;
        state->freq_offset_get = srvl_ts_freq_offset_get;
        state->external_clock_mode_set = srvl_ts_external_clock_mode_set;
        state->alt_clock_saved_get = srvl_ts_alt_clock_saved_get;
        state->alt_clock_mode_set = srvl_ts_alt_clock_mode_set;
        state->timeofday_next_pps_set = srvl_ts_timeofday_next_pps_set;
        state->ingress_latency_set = srvl_ts_ingress_latency_set;
        state->p2p_delay_set = srvl_ts_p2p_delay_set;
        state->egress_latency_set = srvl_ts_egress_latency_set;
        state->delay_asymmetry_set = srvl_ts_delay_asymmetry_set;
        state->operation_mode_set = srvl_ts_operation_mode_set;
        state->internal_mode_set = srvl_ts_internal_mode_set;
        state->timestamp_get = srvl_ts_timestamp_get;
        state->timestamp_id_release = srvl_ts_timestamp_id_release;
        state->status_change = srvl_ts_status_change;

        break;
    case VTSS_INIT_CMD_INIT:
        VTSS_RC(srvl_ts_init(vtss_state));
        break;
    default:
        break;
    }
    return VTSS_RC_OK;
}

#endif /* VTSS_FEATURE_TIMESTAMP */

#endif /* VTSS_ARCH_SERVAL */
