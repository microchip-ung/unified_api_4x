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

#define VTSS_TRACE_GROUP VTSS_TRACE_GROUP_QOS
#include "vtss_jaguar2_cil.h"

#if defined(VTSS_ARCH_JAGUAR_2)

/* - CIL functions ------------------------------------------------- */

/* Calculate 19 bit policer rate

   If input rate is kbps (frame_rate = FALSE) then output rate is calculated like this:
   output rate = ((input rate * 1000) + 25039) / 25040
   This will round up the rate to the nearest possible value:
     0      -> 0 (Open until burst capacity is used, then closed)
     1..25  -> 1 ( 25040 bps)
    26..50  -> 2 ( 50080 bps)
    51..75  -> 3 ( 75120 bps)
    76..100 -> 4 (100160 bps)
   ...

   If input rate is frames per second (frame_rate = TRUE) then output rate is calculated like this:
   output rate = ((input rate) + 9) / 10
   This will round up the rate to the nearest possible value:
     0     -> 0 (Open until burst capacity is used, then closed)
     1..10 -> 1 (10 fps)
    11..20 -> 2 (20 fps)
    21..30 -> 3 (30 fps)
    31..40 -> 4 (40 fps)
   ...

   In both cases the maximum rate returned is limited to 0x7FFFF (13.128.146.480 bps or 5.242.870 fps).
*/
static u32 jr2_calc_policer_rate(u32 rate, BOOL frame_rate)
{
    if (!frame_rate) { /* input rate is measured in kbps */
        rate = VTSS_DIV64(((u64)rate * 1000) + 25039, 25040);
    } else { /* input rate is measured in fps */
        rate = VTSS_DIV64(((u64)rate) + 9, 10);
    }
    return MIN(0x7ffff, rate);
}

/* Calculate 6 bit policer burst level

   If input rate is kbps (frame_rate = FALSE) then output burst level is calculated like this:
   output burst level = (input burst level + 8191) / 8192
   This will round up the burst level to the nearest possible value:
       0        -> 0 (Always closed)
       1.. 8192 -> 1 ( 8.192 bytes)
    8193..16384 -> 2 (16.384 bytes)
   16385..24576 -> 3 (24.576 bytes)
   ...

   If input rate is frames per second (frame_rate = TRUE) then output burst level is set to max.

   In both cases the maximum level returned is limited to 0x3F (516.096 bytes or 206 frames).
*/
static u32 jr2_calc_policer_level(u32 level, u32 rate, BOOL frame_rate)
{
    if (rate == 0) {
        return 0;                                /* Always closed */
    } else if ((rate == VTSS_BITRATE_DISABLED) || frame_rate) {
        return 0x3f;                             /* Maximum burst level */
    } else {
        return MIN(0x3f, (level + 8191) / 8192); /* Calculated value 0..0x3f ~ 0..516.096 bytes in steps of 8.192 bytes */
    }
}

vtss_rc vtss_jr2_port_policer_fc_set(vtss_state_t *vtss_state, const vtss_port_no_t port_no, u32 chipport)
{
    u32 i, fc_ena = 0;
    vtss_port_conf_t *port_conf = &vtss_state->port.conf[port_no];
    vtss_qos_port_conf_t *qos_conf = &vtss_state->qos.port_conf[port_no];

    if (port_conf->flow_control.generate) {
        for (i = 0; i < 4; i++) {
            if ((qos_conf->policer_port[i].rate != VTSS_BITRATE_DISABLED) && qos_conf->policer_ext_port[i].flow_control) {
                fc_ena |= 1 << i;
            }
        }
    }
    JR2_WR(VTSS_ANA_AC_POL_POL_ALL_CFG_POL_PORT_FC_CFG(chipport), fc_ena);
    JR2_WRM(VTSS_DSM_CFG_ETH_FC_CFG(chipport),
            VTSS_F_DSM_CFG_ETH_FC_CFG_FC_ANA_ENA(!!fc_ena),
            VTSS_M_DSM_CFG_ETH_FC_CFG_FC_ANA_ENA);

    return VTSS_RC_OK;
}

static vtss_rc jr2_port_policer_set(vtss_state_t *vtss_state,
                                    u32 port, u32 idx, vtss_policer_t *conf, vtss_policer_ext_t *conf_ext)
{
    u32 pol_idx = (port * 4 + idx);
    u32 cpu_qu_mask = 0, q, traffic_type_mask = 0;

    /* Burst size and rate */
    JR2_WR(VTSS_ANA_AC_POL_POL_PORT_CFG_POL_PORT_THRES_CFG_0(pol_idx), jr2_calc_policer_level(conf->level, conf->rate, conf_ext->frame_rate));
    JR2_WR(VTSS_ANA_AC_POL_POL_PORT_CFG_POL_PORT_THRES_CFG_1(pol_idx), conf_ext->flow_control ? 1 : 0); /* 8196 bytes flow control deassert threshold */
    JR2_WR(VTSS_ANA_AC_POL_POL_PORT_CFG_POL_PORT_RATE_CFG(pol_idx), jr2_calc_policer_rate(conf->rate, conf_ext->frame_rate));

    /* GAP */
    JR2_WR(VTSS_ANA_AC_POL_POL_PORT_CTRL_POL_PORT_GAP(port), 20); /* GAP_VALUE = 20 : Use line-rate measurement */

    /* CPU queue mask */
    for (q = 0; q < 8; q++) {
        if (conf_ext->cpu_queue[q]) {
            cpu_qu_mask |= VTSS_BIT(q);
        }
    }

    /* Traffic type mask */
    if (conf->rate != VTSS_BITRATE_DISABLED) {
        if (conf_ext->known_multicast) {
            traffic_type_mask |= VTSS_BIT(0);
        }
        if (conf_ext->known_broadcast) {
            traffic_type_mask |= VTSS_BIT(1);
        }
        if (conf_ext->known_unicast) {
            traffic_type_mask |= VTSS_BIT(2);
        }
        if (conf_ext->unknown_multicast) {
            traffic_type_mask |= VTSS_BIT(3);
        }
        if (conf_ext->unknown_broadcast) {
            traffic_type_mask |= VTSS_BIT(4);
        }
        if (conf_ext->unknown_unicast) {
            traffic_type_mask |= VTSS_BIT(5);
        }
        if (conf_ext->to_cpu) {
            traffic_type_mask |= VTSS_BIT(6);
        }
        if (conf_ext->learning) {
            traffic_type_mask |= VTSS_BIT(7);
        }
    }

    JR2_WR(VTSS_ANA_AC_POL_POL_PORT_CTRL_POL_PORT_CFG(port, idx),
           VTSS_F_ANA_AC_POL_POL_PORT_CTRL_POL_PORT_CFG_CPU_QU_MASK(cpu_qu_mask)                                 |
           VTSS_F_ANA_AC_POL_POL_PORT_CTRL_POL_PORT_CFG_DP_BYPASS_LVL(conf_ext->dp_bypass_level)                 |
           VTSS_F_ANA_AC_POL_POL_PORT_CTRL_POL_PORT_CFG_FRAME_RATE_ENA(conf_ext->frame_rate)                     |
           VTSS_F_ANA_AC_POL_POL_PORT_CTRL_POL_PORT_CFG_LIMIT_NONCPU_TRAFFIC_ENA(conf_ext->limit_noncpu_traffic) |
           VTSS_F_ANA_AC_POL_POL_PORT_CTRL_POL_PORT_CFG_LIMIT_CPU_TRAFFIC_ENA(conf_ext->limit_cpu_traffic)       |
           VTSS_F_ANA_AC_POL_POL_PORT_CTRL_POL_PORT_CFG_TRAFFIC_TYPE_MASK(traffic_type_mask));

    return VTSS_RC_OK;
}

vtss_rc vtss_jr2_policer_conf_set(vtss_state_t *vtss_state, u32 idx, vtss_dlb_policer_conf_t *dlb_conf)
{
    vtss_dlb_policer_conf_t conf = *dlb_conf;
    u64                     rate, interval;
    u32                     scale, cf, i, level;

    switch (conf.type) {
    case VTSS_POLICER_TYPE_MEF:
        break;
    case VTSS_POLICER_TYPE_SINGLE:
    default:
        /* Single bucket, disable EIR */
        conf.eir = 0;
        conf.ebs = 0;
        conf.cm = 0;
        conf.cf = 0;
        break;
    }

    /* Calculate scale based on maximum rate */
    if ((cf = (conf.cm && conf.cf ? 1 : 0)) != 0) {
        /* Coupling mode, PIR = EIR + CIR */
        conf.eir += conf.cir;
    }
    rate = MAX(conf.cir, conf.eir);
    if (rate > 2000000) {
        /* More than 2 Gbps */
        scale = 0;
        interval = 8196721;
    } else if (rate > 260000) {
        /* More than 260 Mbps */
        scale = 1;
        interval = 1024590;
    } else if (rate > 32000) {
        /* More than 32 Mbps */
        scale = 2;
        interval = 128074;
    } else {
        /* Less than 32 Mbps */
        scale = 3;
        interval = 16009;
    }

    JR2_WR(VTSS_ANA_AC_POL_SDLB_DLB_CFG(idx),
           VTSS_F_ANA_AC_POL_SDLB_DLB_CFG_COUPLING_MODE(cf) |
           VTSS_F_ANA_AC_POL_SDLB_DLB_CFG_COLOR_AWARE_LVL(conf.cm ? 0 : 3) |
           VTSS_F_ANA_AC_POL_SDLB_DLB_CFG_CIR_INC_DP_VAL(1) |
           VTSS_F_ANA_AC_POL_SDLB_DLB_CFG_TRAFFIC_TYPE_MASK(3) |
           VTSS_F_ANA_AC_POL_SDLB_DLB_CFG_GAP_VAL(conf.line_rate ? 20 : 0) |
           VTSS_F_ANA_AC_POL_SDLB_DLB_CFG_TIMESCALE_VAL(scale));

    for (i = 0; i < 2; i++) {
        if (i == 0) {
            rate = conf.cir;
            level = conf.cbs;
        } else {
            rate = conf.eir;
            level = conf.ebs;
        }
        rate = VTSS_DIV64((rate * 1000), interval);
        if (rate > 0x7ff || conf.enable == 0) {
            rate = 0x7ff;
        }
        level /= 2048;
        if (level > 0x7f) {
            level = 0x7f;
        } else if (level == 0) {
            level = 1;
        }
        if (rate == 0) {
            /* No burst allowed if rate is zero */
            level = 0;
        }
        JR2_WR(VTSS_ANA_AC_POL_SDLB_LB_CFG(idx, i),
               VTSS_F_ANA_AC_POL_SDLB_LB_CFG_THRES_VAL(level) |
               VTSS_F_ANA_AC_POL_SDLB_LB_CFG_RATE_VAL(rate));
    }
    return VTSS_RC_OK;
}

static vtss_rc jr2_queue_policer_set(vtss_state_t *vtss_state,
                                     u32 port, u32 queue, vtss_policer_t *conf)
{
#if defined(VTSS_ARCH_JAGUAR_2_B)
    u32 pol_idx = (port * 8 + queue);
    u32 traffic_type_mask = 0;

    if (conf->rate != VTSS_BITRATE_DISABLED) {
        traffic_type_mask = 1; /* Only front ports - not CPU */
    }

    JR2_WR(VTSS_ANA_AC_POL_POL_PRIO_CFG_POL_PRIO_RATE_CFG(pol_idx), jr2_calc_policer_rate(conf->rate, FALSE));
    JR2_WR(VTSS_ANA_AC_POL_POL_PRIO_CFG_POL_PRIO_THRES_CFG(pol_idx), jr2_calc_policer_level(conf->level, conf->rate, FALSE));
    JR2_WR(VTSS_ANA_AC_POL_POL_PRIO_CTRL_POL_PRIO_GAP(pol_idx), 20); /* GAP_VALUE = 20 : Use line-rate measurement */
    JR2_WR(VTSS_ANA_AC_POL_POL_PRIO_CTRL_POL_PRIO_CFG(pol_idx),
           VTSS_F_ANA_AC_POL_POL_PRIO_CTRL_POL_PRIO_CFG_TRAFFIC_TYPE_MASK(traffic_type_mask) |
           VTSS_F_ANA_AC_POL_POL_PRIO_CTRL_POL_PRIO_CFG_DP_BYPASS_LVL(0)); /* 0: Police on all dp levels */
#else
    if (port < VTSS_CHIP_PORTS) {
        u32                     pol_idx = VTSS_QUEUE_POL_IDX(port, queue);
        vtss_dlb_policer_conf_t dlb_conf;

        memset(&dlb_conf, 0, sizeof(dlb_conf));
        dlb_conf.type = VTSS_POLICER_TYPE_SINGLE;
        dlb_conf.enable = 1;
        dlb_conf.line_rate = 1;
        dlb_conf.cir = conf->rate;
        dlb_conf.cbs = conf->level;
        return vtss_jr2_policer_conf_set(vtss_state, pol_idx, &dlb_conf);
    }
#endif /* VTSS_ARCH_JAGUAR_2_B */

    return VTSS_RC_OK;
}

static vtss_rc jr2_qos_shaper_conf_set(vtss_state_t *vtss_state, vtss_shaper_t *shaper, u32 se, u32 layer, u32 dlb_sense_port, u32 dlb_sense_qos)
{
    /* Shaper rate configuration.
     * Unit is defined to 100 kbps by the leak list period the shaper is attached to (see HSCH_LEAK_CFG.LEAK_TIME).
     * The value (in kbps) is rounded up to the next possible value:
     *        0 -> 0 (Open until burst capacity is used, then closed)
     *   1..100 -> 1 (100 kbps)
     * 101..200 -> 2 (200 kbps)
     * 201..300 -> 3 (300 kbps)
     */

    /* Shaper burst level configuration
     * The value is rounded up to the next possible value:
     *           0 -> 0 (Shaper disabled)
     *    1.. 4096 -> 1 ( 4 KB)
     * 4097.. 8192 -> 2 ( 8 KB)
     * 8193..12288 -> 3 (12 KB)
     */

    /* Select layer */
    JR2_WRM(VTSS_HSCH_HSCH_MISC_HSCH_CFG_CFG,
            VTSS_F_HSCH_HSCH_MISC_HSCH_CFG_CFG_HSCH_LAYER(layer),
            VTSS_M_HSCH_HSCH_MISC_HSCH_CFG_CFG_HSCH_LAYER);

    if (shaper->rate != VTSS_BITRATE_DISABLED) {
        u32 cir = MIN(VTSS_BITMASK(17), JR2_DIV_ROUND_UP(shaper->rate,   100));
        u32 cbs = MIN(VTSS_BITMASK(6),  JR2_DIV_ROUND_UP(shaper->level, 4096));

        JR2_WR(VTSS_HSCH_HSCH_CFG_CIR_CFG(se),
               VTSS_F_HSCH_HSCH_CFG_CIR_CFG_CIR_RATE(cir) |
               VTSS_F_HSCH_HSCH_CFG_CIR_CFG_CIR_BURST(cbs));

        if (shaper->eir != VTSS_BITRATE_DISABLED) {
            u32 eir = MIN(VTSS_BITMASK(17), JR2_DIV_ROUND_UP(shaper->eir,  100));
            u32 ebs = MIN(VTSS_BITMASK(6),  JR2_DIV_ROUND_UP(shaper->ebs, 4096));

            JR2_WR(VTSS_HSCH_HSCH_CFG_EIR_CFG(se),
                   VTSS_F_HSCH_HSCH_CFG_EIR_CFG_EIR_RATE(eir) |
                   VTSS_F_HSCH_HSCH_CFG_EIR_CFG_EIR_BURST(ebs));

            JR2_WR(VTSS_HSCH_HSCH_CFG_SE_DLB_SENSE(se),
                   VTSS_F_HSCH_HSCH_CFG_SE_DLB_SENSE_SE_DLB_PRIO(dlb_sense_qos)   |
                   VTSS_F_HSCH_HSCH_CFG_SE_DLB_SENSE_SE_DLB_DPORT(dlb_sense_port) |
                   VTSS_F_HSCH_HSCH_CFG_SE_DLB_SENSE_SE_DLB_PRIO_ENA(1)           |
                   VTSS_F_HSCH_HSCH_CFG_SE_DLB_SENSE_SE_DLB_DPORT_ENA(1));
        } else {
            JR2_WR(VTSS_HSCH_HSCH_CFG_EIR_CFG(se),      0); /* Disable EIR */
            JR2_WR(VTSS_HSCH_HSCH_CFG_SE_DLB_SENSE(se), 0); /* Disable DLB */
        }
    } else {
        JR2_WR(VTSS_HSCH_HSCH_CFG_CIR_CFG(se),      0); /* Disable CIR */
        JR2_WR(VTSS_HSCH_HSCH_CFG_EIR_CFG(se),      0); /* Disable EIR */
        JR2_WR(VTSS_HSCH_HSCH_CFG_SE_DLB_SENSE(se), 0); /* Disable DLB */
    }

    return VTSS_RC_OK;
}

static vtss_rc jr2_qos_port_conf_set(vtss_state_t *vtss_state, const vtss_port_no_t port_no)
{
    vtss_qos_port_conf_t     *conf = &vtss_state->qos.port_conf[port_no];
    u32                      port = VTSS_CHIP_PORT(port_no);
    int                      pcp, dei, class, queue, policer;
    BOOL                     tag_default_dei;
    u32                      tag_pcp_cfg, tag_dei_cfg;
    u8                       dwrr_cost[8] = {0};
    u32                      dwrr_cnt;

    /* Port default PCP and DEI configuration */
    JR2_WRM(VTSS_ANA_CL_PORT_VLAN_CTRL(port),
            VTSS_F_ANA_CL_PORT_VLAN_CTRL_PORT_PCP(conf->usr_prio)     |
            VTSS_F_ANA_CL_PORT_VLAN_CTRL_PORT_DEI(conf->default_dei),
            VTSS_M_ANA_CL_PORT_VLAN_CTRL_PORT_PCP                     |
            VTSS_M_ANA_CL_PORT_VLAN_CTRL_PORT_DEI);

    /* Port default QoS class, DP level, tagged frames mode, DSCP mode and DSCP remarking configuration */
    JR2_WRM(VTSS_ANA_CL_PORT_QOS_CFG(port),
            VTSS_F_ANA_CL_PORT_QOS_CFG_DSCP_REWR_MODE_SEL(conf->dscp_mode)                                      |
            VTSS_F_ANA_CL_PORT_QOS_CFG_DSCP_TRANSLATE_ENA(conf->dscp_translate)                                 |
            VTSS_F_ANA_CL_PORT_QOS_CFG_PCP_DEI_DP_ENA(conf->tag_class_enable)                                   |
            VTSS_F_ANA_CL_PORT_QOS_CFG_PCP_DEI_QOS_ENA(conf->tag_class_enable)                                  |
            VTSS_F_ANA_CL_PORT_QOS_CFG_DSCP_DP_ENA(conf->dscp_class_enable)                                     |
            VTSS_F_ANA_CL_PORT_QOS_CFG_DSCP_QOS_ENA(conf->dscp_class_enable)                                    |
            VTSS_F_ANA_CL_PORT_QOS_CFG_DEFAULT_DP_VAL(conf->default_dpl)                                        |
            VTSS_F_ANA_CL_PORT_QOS_CFG_DEFAULT_QOS_VAL(vtss_cmn_qos_chip_prio(vtss_state, conf->default_prio)),
            VTSS_M_ANA_CL_PORT_QOS_CFG_DSCP_REWR_MODE_SEL                                                       |
            VTSS_M_ANA_CL_PORT_QOS_CFG_DSCP_TRANSLATE_ENA                                                       |
            VTSS_M_ANA_CL_PORT_QOS_CFG_PCP_DEI_DP_ENA                                                           |
            VTSS_M_ANA_CL_PORT_QOS_CFG_PCP_DEI_QOS_ENA                                                          |
            VTSS_M_ANA_CL_PORT_QOS_CFG_DSCP_DP_ENA                                                              |
            VTSS_M_ANA_CL_PORT_QOS_CFG_DSCP_QOS_ENA                                                             |
            VTSS_M_ANA_CL_PORT_QOS_CFG_DEFAULT_DP_VAL                                                           |
            VTSS_M_ANA_CL_PORT_QOS_CFG_DEFAULT_QOS_VAL);

    /* Egress DSCP remarking configuration */
    JR2_WR(VTSS_REW_PORT_DSCP_MAP(port),
           VTSS_F_REW_PORT_DSCP_MAP_DSCP_UPDATE_ENA((conf->dscp_emode > VTSS_DSCP_EMODE_DISABLE)) |
           VTSS_F_REW_PORT_DSCP_MAP_DSCP_REMAP_ENA((conf->dscp_emode > VTSS_DSCP_EMODE_REMARK)));

    /* Map for (PCP and DEI) to (QoS class and DP level */
    for (pcp = VTSS_PCP_START; pcp < VTSS_PCP_END; pcp++) {
        for (dei = VTSS_DEI_START; dei < VTSS_DEI_END; dei++) {
            JR2_WR(VTSS_ANA_CL_PORT_PCP_DEI_MAP_CFG(port, (8 * dei + pcp)),
                   VTSS_F_ANA_CL_PORT_PCP_DEI_MAP_CFG_PCP_DEI_DP_VAL(conf->dp_level_map[pcp][dei]) |
                   VTSS_F_ANA_CL_PORT_PCP_DEI_MAP_CFG_PCP_DEI_QOS_VAL(vtss_cmn_qos_chip_prio(vtss_state, conf->qos_class_map[pcp][dei])));
        }
    }

    /* DWRR configuration */
    if (conf->dwrr_enable) {
#if defined(VTSS_FEATURE_QOS_SCHEDULER_DWRR_CNT)
        dwrr_cnt = conf->dwrr_cnt;
        /* Number of allowed queues in DWRR mode is 1..8 */
        if (dwrr_cnt < 1) {
            dwrr_cnt = 1;
        } else if (dwrr_cnt > 8) {
            dwrr_cnt = 8;
        }
#else
        dwrr_cnt = 6;
#endif /* VTSS_FEATURE_QOS_SCHEDULER_DWRR_CNT */
    } else {
        dwrr_cnt = 0; /* All queues are running in strict mode */
    }

    /* Select layer 2 and port */
#if defined(VTSS_ARCH_JAGUAR_2_B)
    JR2_WR(VTSS_HSCH_HSCH_MISC_HSCH_CFG_CFG,
           VTSS_F_HSCH_HSCH_MISC_HSCH_CFG_CFG_HSCH_LAYER(2) |
           VTSS_F_HSCH_HSCH_MISC_HSCH_CFG_CFG_DWRR_IDX(port));
#else
    JR2_WR(VTSS_HSCH_HSCH_MISC_HSCH_CFG_CFG,
           VTSS_F_HSCH_HSCH_MISC_HSCH_CFG_CFG_HSCH_LAYER(2)   |
           VTSS_F_HSCH_HSCH_MISC_HSCH_CFG_CFG_CFG_SE_IDX(port));
#endif /* VTSS_ARCH_JAGUAR_2_B */

    JR2_WRM(VTSS_HSCH_HSCH_CFG_SE_CFG(port),
            VTSS_F_HSCH_HSCH_CFG_SE_CFG_SE_DWRR_CNT(dwrr_cnt),
            VTSS_M_HSCH_HSCH_CFG_SE_CFG_SE_DWRR_CNT);

    VTSS_RC(vtss_cmn_qos_weight2cost(conf->queue_pct, dwrr_cost, dwrr_cnt, VTSS_QOS_DWRR_COST_BIT_WIDTH));
    for (queue = 0; queue < 8; queue++) {
        JR2_WRM(VTSS_HSCH_HSCH_DWRR_DWRR_ENTRY(queue),
                VTSS_F_HSCH_HSCH_DWRR_DWRR_ENTRY_DWRR_COST(dwrr_cost[queue]),
                VTSS_M_HSCH_HSCH_DWRR_DWRR_ENTRY_DWRR_COST);
    }

    /* Port shaper configuration. Use scheduler element in layer 2 indexed by port. */
    VTSS_RC(jr2_qos_shaper_conf_set(vtss_state, &conf->shaper_port, port, 2, port, 0));

    /* Queue shaper configuration. Use scheduler element in layer 0 indexed by (port * 8) + queue */
    for (queue = 0; queue < 8; queue++) {
        VTSS_RC(jr2_qos_shaper_conf_set(vtss_state, &conf->shaper_queue[queue], (port * 8) + queue, 0, port, queue));
    }

    /* Tag remarking configuration */
    tag_default_dei = (conf->tag_remark_mode == VTSS_TAG_REMARK_MODE_DEFAULT ? conf->tag_default_dei : 0); // JR2-TBD: Is this necessary on JR2?

    JR2_WRM(VTSS_REW_PORT_PORT_VLAN_CFG(port),
            VTSS_F_REW_PORT_PORT_VLAN_CFG_PORT_PCP(conf->tag_default_pcp) |
            VTSS_F_REW_PORT_PORT_VLAN_CFG_PORT_DEI(tag_default_dei),                                       // Is this necessary on JR2?
            VTSS_M_REW_PORT_PORT_VLAN_CFG_PORT_PCP                        |
            VTSS_M_REW_PORT_PORT_VLAN_CFG_PORT_DEI);

    switch (conf->tag_remark_mode) {
    case VTSS_TAG_REMARK_MODE_DEFAULT:
        tag_pcp_cfg = 1; /* PORT_PCP */
        tag_dei_cfg = 1; /* PORT_DEI */
        break;
    case VTSS_TAG_REMARK_MODE_MAPPED:
        tag_pcp_cfg = 2; /* DE and QoS mapped to PCP (PCP_MAP_DEx */
        tag_dei_cfg = 2; /* DE and QoS mapped to DEI (DEI_MAP_DEx */
        break;
    default:
        tag_pcp_cfg = 0; /* Classified PCP */
        tag_dei_cfg = 0; /* Classified PCP/DEI */
        break;
    }

    JR2_WRM(VTSS_REW_PORT_TAG_CTRL(port),
            VTSS_F_REW_PORT_TAG_CTRL_TAG_PCP_CFG(tag_pcp_cfg) |
            VTSS_F_REW_PORT_TAG_CTRL_TAG_DEI_CFG(tag_dei_cfg),
            VTSS_M_REW_PORT_TAG_CTRL_TAG_PCP_CFG              |
            VTSS_M_REW_PORT_TAG_CTRL_TAG_DEI_CFG);

    /* Map for (QoS class and DP level) to (PCP and DEI) */
    for (class = VTSS_QUEUE_START; class < VTSS_QUEUE_END; class++) {
        JR2_WR(VTSS_REW_PORT_PCP_MAP_DE0(port, class),
               VTSS_F_REW_PORT_PCP_MAP_DE0_PCP_DE0(conf->tag_pcp_map[class][0]));
        JR2_WR(VTSS_REW_PORT_PCP_MAP_DE1(port, class),
               VTSS_F_REW_PORT_PCP_MAP_DE1_PCP_DE1(conf->tag_pcp_map[class][1]));
        JR2_WR(VTSS_REW_PORT_DEI_MAP_DE0(port, class),
               VTSS_F_REW_PORT_DEI_MAP_DE0_DEI_DE0(conf->tag_dei_map[class][0]));
        JR2_WR(VTSS_REW_PORT_DEI_MAP_DE1(port, class),
               VTSS_F_REW_PORT_DEI_MAP_DE1_DEI_DE1(conf->tag_dei_map[class][1]));
    }

    /* Port policer configuration */
    for (policer = 0; policer < 4; policer++) {
        VTSS_RC(jr2_port_policer_set(vtss_state, port, policer, &conf->policer_port[policer], &conf->policer_ext_port[policer]));
    }

    /* Queue policer configuration */
    for (queue = 0; queue < 8; queue++) {
        VTSS_RC(jr2_queue_policer_set(vtss_state, port, queue, &conf->policer_queue[queue]));
    }

    /* Update policer flow control configuration */
    VTSS_RC(vtss_jr2_port_policer_fc_set(vtss_state, port_no, port));

    /* Update WRED group configuration */
    if (conf->wred_group > 2) {
        VTSS_E("Invalid WRED group: %u!", conf->wred_group);
        return VTSS_RC_ERROR;
    }
    JR2_WR(VTSS_QRES_RES_QOS_ADV_WRED_GROUP(port), conf->wred_group);

    /* JR2-TBD: Update QCL port configuration */

    /* JR2-TBD: HQoS update */
    return VTSS_RC_OK;
}

static vtss_rc jr2_storm_policer_set(vtss_state_t             *vtss_state,
                                     u32                       idx,
                                     vtss_packet_rate_t        rate,
                                     BOOL                      frame_rate,
                                     vtss_storm_policer_mode_t mode)
{
    BOOL noncpu_traffic, cpu_traffic;
    u32  traffic_type_mask;

    if (idx > 2) {
        VTSS_E("Invalid storm policer index: %u!", idx);
        return VTSS_RC_ERROR;
    }

    if (rate == VTSS_PACKET_RATE_DISABLED) {
        /* Apply default values */
        JR2_WR(VTSS_ANA_AC_POL_POL_ALL_CFG_POL_STORM_RATE_CFG(idx), 0);
        JR2_WR(VTSS_ANA_AC_POL_POL_ALL_CFG_POL_STORM_THRES_CFG(idx), 0);
        JR2_WR(VTSS_ANA_AC_POL_POL_ALL_CFG_POL_STORM_CTRL(idx),
               VTSS_F_ANA_AC_POL_POL_ALL_CFG_POL_STORM_CTRL_STORM_LIMIT_NONCPU_TRAFFIC_ENA(1));
    } else {
        if (mode == VTSS_STORM_POLICER_MODE_PORTS_ONLY) {
            noncpu_traffic = TRUE;
            cpu_traffic    = FALSE;
        } else if (mode == VTSS_STORM_POLICER_MODE_CPU_ONLY) {
            noncpu_traffic = FALSE;
            cpu_traffic    = TRUE;
        } else {
            noncpu_traffic = TRUE;
            cpu_traffic    = TRUE;
        }

        if (idx == 0) {
            traffic_type_mask = 0x20; /* Unknown unicast: 0b0010.0000 */
        } else if (idx == 1) {
            traffic_type_mask = 0x08; /* Unknown multicast: 0b0000.1000 */
        } else {
            traffic_type_mask = 0x12; /* Known and unknown broadcast: 0b0001.0010 */
        }

        JR2_WR(VTSS_ANA_AC_POL_POL_ALL_CFG_POL_STORM_RATE_CFG(idx),
               jr2_calc_policer_rate(rate, frame_rate));
        JR2_WR(VTSS_ANA_AC_POL_POL_ALL_CFG_POL_STORM_THRES_CFG(idx),
               jr2_calc_policer_level(8192 * 2, rate, frame_rate));
        JR2_WR(VTSS_ANA_AC_POL_POL_ALL_CFG_POL_STORM_CTRL(idx),
               VTSS_F_ANA_AC_POL_POL_ALL_CFG_POL_STORM_CTRL_STORM_FRAME_RATE_ENA(frame_rate)               |
               VTSS_F_ANA_AC_POL_POL_ALL_CFG_POL_STORM_CTRL_STORM_CPU_QU_MASK(0)                           |
               VTSS_F_ANA_AC_POL_POL_ALL_CFG_POL_STORM_CTRL_STORM_LIMIT_NONCPU_TRAFFIC_ENA(noncpu_traffic) |
               VTSS_F_ANA_AC_POL_POL_ALL_CFG_POL_STORM_CTRL_STORM_LIMIT_CPU_TRAFFIC_ENA(cpu_traffic)       |
               VTSS_F_ANA_AC_POL_POL_ALL_CFG_POL_STORM_CTRL_STORM_TRAFFIC_TYPE_MASK(traffic_type_mask));
    }

    return VTSS_RC_OK;
}

static vtss_rc jr2_qos_wred_conf_set(vtss_state_t *vtss_state)
{
    vtss_qos_conf_t *conf = &vtss_state->qos.conf;
    int              group, queue, dpl;

    for (group = 0; group < VTSS_WRED_GROUP_CNT; group++) {
        for (queue = VTSS_QUEUE_START; queue < VTSS_QUEUE_END; queue++) {
            u32 wm_high, wm_red_low, wm_red_high;
            JR2_RD(VTSS_QRES_RES_CTRL_RES_CFG((queue + 496)), &wm_high); /* Shared ingress high watermark for queue - common for all dpls */
            wm_high = vtss_jr2_wm_dec(wm_high) * JR2_BUFFER_CELL_SZ; /* Convert from 176 byte chunks to bytes */
            for (dpl = 0; dpl < VTSS_WRED_DPL_CNT; dpl++) {
                vtss_red_v3_t *red = &conf->red_v3[queue][dpl][group];
                vtss_pct_t     max_dp = 100;
                vtss_pct_t     max_fl = 100;

                /* Sanity check */
                if (red->min_fl > 100) {
                    VTSS_E("illegal min_fl (%u) on group %d, queue %d, dpl %d", red->min_fl, group, queue, dpl);
                    return VTSS_RC_ERROR;
                }
                if ((red->max < 1) || (red->max > 100)) {
                    VTSS_E("illegal max (%u) on group %d, queue %d, dpl %d", red->max, group, queue, dpl);
                    return VTSS_RC_ERROR;
                }
                if ((red->max_unit != VTSS_WRED_V2_MAX_DP) && (red->max_unit != VTSS_WRED_V2_MAX_FL)) {
                    VTSS_E("illegal max_unit (%u) on group %d, queue %d, dpl %d", red->max_unit, group, queue, dpl);
                    return VTSS_RC_ERROR;
                }
                if (red->max_unit == VTSS_WRED_V2_MAX_DP) {
                    max_dp = red->max; /* Unit is drop probability - save specified value */
                } else {
                    if (red->min_fl >= red->max) {
                        VTSS_E("min_fl (%u) >= max fl (%u) on group %d, queue %d, dpl %d", red->min_fl, red->max, group, queue, dpl);
                        return VTSS_RC_ERROR;
                    } else {
                        max_fl = red->max; /* Unit is fill level - save specified value */
                    }
                }
                if (red->enable) {
                    wm_red_low  = wm_high * red->min_fl / 100;                              /* Convert from % to actual value in bytes */
                    wm_red_high = wm_high * max_fl / 100;                                   /* Convert from % to actual value in bytes */
                    wm_red_high = ((wm_red_high - wm_red_low) * 100 / max_dp) + wm_red_low; /* Adjust wm_red_high to represent 100% drop probability */
                    wm_red_low  = MIN(wm_red_low / 2816, VTSS_BITMASK(11));                 /* Convert from bytes to 2816 byte chunks and prevent overflow */
                    wm_red_high = MIN(wm_red_high / 2816, VTSS_BITMASK(11));                /* Convert from bytes to 2816 byte chunks and prevent overflow */
                } else {
                    wm_red_low = wm_red_high = VTSS_BITMASK(11);                            /* Disable red by setting both fields to max */
                }

                JR2_WR(VTSS_QRES_RES_WRED_WRED_PROFILE((24 * group) + (8 * dpl) + queue),   /* Red profile for qroup, queue, dpl */
                       VTSS_F_QRES_RES_WRED_WRED_PROFILE_WM_RED_LOW(wm_red_low) |
                       VTSS_F_QRES_RES_WRED_WRED_PROFILE_WM_RED_HIGH(wm_red_high));
            }
        }
    }

    VTSS_RC(vtss_jr2_wm_update(vtss_state)); /* Update watermarks */

    return VTSS_RC_OK;
}

static vtss_rc jr2_qos_conf_set(vtss_state_t *vtss_state, BOOL changed)
{
    vtss_qos_conf_t    *conf = &vtss_state->qos.conf;
    vtss_port_no_t     port_no;
    u32                i;

    if (changed) {
        /* Number of priorities changed, update QoS setup for all ports */
        for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {
            VTSS_RC(jr2_qos_port_conf_set(vtss_state, port_no));
        }
    }

    /* Storm control:
     *
     * Jaguar 2 has eight identical configurable storm policers.
     *
     * Allocation:
     *  Policer 0:   Unicast (unknown unicast frames).
     *  Policer 1:   Multicast (unknown multicast frames).
     *  Policer 2:   Broadcast (known and unknown broadcast frames).
     *  Policer 3-7: Not used.
     */
    VTSS_RC(jr2_storm_policer_set(vtss_state, 0, conf->policer_uc, conf->policer_uc_frame_rate, conf->policer_uc_mode));
    VTSS_RC(jr2_storm_policer_set(vtss_state, 1, conf->policer_mc, conf->policer_mc_frame_rate, conf->policer_mc_mode));
    VTSS_RC(jr2_storm_policer_set(vtss_state, 2, conf->policer_bc, conf->policer_bc_frame_rate, conf->policer_bc_mode));

    /* DSCP classification and remarking configuration: */
    for (i = 0; i < 64; i++) {
        JR2_WR(VTSS_ANA_CL_COMMON_DSCP_CFG(i),
               VTSS_F_ANA_CL_COMMON_DSCP_CFG_DSCP_TRANSLATE_VAL(conf->dscp_translate_map[i])                               |
               VTSS_F_ANA_CL_COMMON_DSCP_CFG_DSCP_QOS_VAL(vtss_cmn_qos_chip_prio(vtss_state, conf->dscp_qos_class_map[i])) |
               VTSS_F_ANA_CL_COMMON_DSCP_CFG_DSCP_DP_VAL(conf->dscp_dp_level_map[i])                                       |
               VTSS_F_ANA_CL_COMMON_DSCP_CFG_DSCP_REWR_ENA(conf->dscp_remark[i])                                           |
               VTSS_F_ANA_CL_COMMON_DSCP_CFG_DSCP_TRUST_ENA(conf->dscp_trust[i]));
        JR2_WR(VTSS_REW_COMMON_DSCP_REMAP(i),
               VTSS_F_REW_COMMON_DSCP_REMAP_DSCP_REMAP(conf->dscp_remap[i]));
    }

    /* DSCP classification from QoS configuration: */
    for (i = 0; i < 8; i++) {
        JR2_WR(VTSS_ANA_CL_COMMON_QOS_MAP_CFG(i),
               VTSS_F_ANA_CL_COMMON_QOS_MAP_CFG_DSCP_REWR_VAL(conf->dscp_qos_map[i]));
        JR2_WR(VTSS_ANA_CL_COMMON_QOS_MAP_CFG(i +  8),
               VTSS_F_ANA_CL_COMMON_QOS_MAP_CFG_DSCP_REWR_VAL(conf->dscp_qos_map_dp1[i]));
        JR2_WR(VTSS_ANA_CL_COMMON_QOS_MAP_CFG(i + 16),
               VTSS_F_ANA_CL_COMMON_QOS_MAP_CFG_DSCP_REWR_VAL(conf->dscp_qos_map_dp2[i]));
        JR2_WR(VTSS_ANA_CL_COMMON_QOS_MAP_CFG(i + 24),
               VTSS_F_ANA_CL_COMMON_QOS_MAP_CFG_DSCP_REWR_VAL(conf->dscp_qos_map_dp3[i]));

    }

    /* WRED configuration: */
    VTSS_RC(jr2_qos_wred_conf_set(vtss_state));

    return VTSS_RC_OK;
}

#if defined(VTSS_FEATURE_EVC)
static vtss_rc jr2_evc_policer_conf_set(vtss_state_t *vtss_state,
                                        const vtss_evc_policer_id_t policer_id)
{
    // JR2-TBD: Stub
    return VTSS_RC_ERROR;
}
#endif

/* - Debug print --------------------------------------------------- */

static vtss_rc jr2_debug_qos(vtss_state_t *vtss_state,
                             const vtss_debug_printf_t pr,
                             const vtss_debug_info_t   *const info)
{
    u32            i, port, queue, value, pol_idx;
    vtss_port_no_t port_no;
    char           buf[32];

    /* Global configuration starts here */

    vtss_debug_print_header(pr, "QoS DSCP Config");

    pr("DSCP Trans CLS DPL Rewr Trust Remap\n");
    for (i = 0; i < 64; i++) {
        u32 dscp_cfg, dscp_remap;
        JR2_RD(VTSS_ANA_CL_COMMON_DSCP_CFG(i), &dscp_cfg);
        JR2_RD(VTSS_REW_COMMON_DSCP_REMAP(i), &dscp_remap);
        pr("%4u %5u %3u %3u %4u %5u %5u\n",
           i,
           VTSS_X_ANA_CL_COMMON_DSCP_CFG_DSCP_TRANSLATE_VAL(dscp_cfg),
           VTSS_X_ANA_CL_COMMON_DSCP_CFG_DSCP_QOS_VAL(dscp_cfg),
           VTSS_X_ANA_CL_COMMON_DSCP_CFG_DSCP_DP_VAL(dscp_cfg),
           VTSS_X_ANA_CL_COMMON_DSCP_CFG_DSCP_REWR_ENA(dscp_cfg),
           VTSS_X_ANA_CL_COMMON_DSCP_CFG_DSCP_TRUST_ENA(dscp_cfg),
           VTSS_X_REW_COMMON_DSCP_REMAP_DSCP_REMAP(dscp_remap));
    }
    pr("\n");

    vtss_debug_print_header(pr, "QoS DSCP Classification from QoS Config");

    pr("QoS DSCP_DP0 DSCP_DP1 DSCP_DP2 DSCP_DP3\n");
    for (i = 0; i < 8; i++) {
        u32 qos_dp0, qos_dp1, qos_dp2, qos_dp3;
        JR2_RD(VTSS_ANA_CL_COMMON_QOS_MAP_CFG(i),      &qos_dp0);
        JR2_RD(VTSS_ANA_CL_COMMON_QOS_MAP_CFG(i +  8), &qos_dp1);
        JR2_RD(VTSS_ANA_CL_COMMON_QOS_MAP_CFG(i + 16), &qos_dp2);
        JR2_RD(VTSS_ANA_CL_COMMON_QOS_MAP_CFG(i + 24), &qos_dp3);
        pr("%3u %4u     %4u     %4u     %4u\n",
           i,
           VTSS_X_ANA_CL_COMMON_QOS_MAP_CFG_DSCP_REWR_VAL(qos_dp0),
           VTSS_X_ANA_CL_COMMON_QOS_MAP_CFG_DSCP_REWR_VAL(qos_dp1),
           VTSS_X_ANA_CL_COMMON_QOS_MAP_CFG_DSCP_REWR_VAL(qos_dp2),
           VTSS_X_ANA_CL_COMMON_QOS_MAP_CFG_DSCP_REWR_VAL(qos_dp3));
    }
    pr("\n");

    vtss_debug_print_header(pr, "QoS Storm Policers");
    vtss_jr2_debug_reg_header(pr, "Storm Policers");
    for (i = 0; i < 8; i++) {
        vtss_jr2_debug_reg_inst(vtss_state, pr, VTSS_ANA_AC_POL_POL_ALL_CFG_POL_STORM_RATE_CFG(i) ,  i, "POL_STORM_RATE_CFG");
        vtss_jr2_debug_reg_inst(vtss_state, pr, VTSS_ANA_AC_POL_POL_ALL_CFG_POL_STORM_THRES_CFG(i) , i, "POL_STORM_THRES_CFG");
        vtss_jr2_debug_reg_inst(vtss_state, pr, VTSS_ANA_AC_POL_POL_ALL_CFG_POL_STORM_CTRL(i) ,      i, "POL_STORM_CTRL");
    }
    vtss_jr2_debug_reg(vtss_state, pr, VTSS_ANA_AC_POL_POL_ALL_CFG_POL_ALL_CFG, "POL_ALL_CFG");
    pr("\n");

    vtss_debug_print_header(pr, "QoS WRED Config");
    vtss_jr2_debug_reg_header(pr, "RES_WRED GRP*24 + (DP-1)*8 + QOS");
    for (i = 0; i < 72; i++) {
        vtss_jr2_debug_reg_inst(vtss_state, pr, VTSS_QRES_RES_WRED_WRED_PROFILE(i) ,  i, "WRED_PROFILE");
    }
    pr("\n");

    /* Per port configuration starts here */

    vtss_debug_print_header(pr, "QoS Port Classification Config");

    pr("LP CP PCP CLS DEI DPL TC_CLS TC_DPL DC_CLS DC_DPL W_GRP\n");
    for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {
        u32 vlan, qos, wred_group;
        if (!info->port_list[port_no]) {
            continue;
        }
        port = VTSS_CHIP_PORT(port_no);
        JR2_RD(VTSS_ANA_CL_PORT_VLAN_CTRL(port), &vlan);
        JR2_RD(VTSS_ANA_CL_PORT_QOS_CFG(port), &qos);
        JR2_RD(VTSS_QRES_RES_QOS_ADV_WRED_GROUP(port), &wred_group);
        pr("%2u %2u %3u %3u %3u %3u %6u %6u %6u %6u %5u\n",
           port_no, // Logical port
           port,    // Chip port
           VTSS_X_ANA_CL_PORT_VLAN_CTRL_PORT_PCP(vlan),
           VTSS_X_ANA_CL_PORT_QOS_CFG_DEFAULT_QOS_VAL(qos),
           VTSS_X_ANA_CL_PORT_VLAN_CTRL_PORT_DEI(vlan),
           VTSS_X_ANA_CL_PORT_QOS_CFG_DEFAULT_DP_VAL(qos),
           VTSS_X_ANA_CL_PORT_QOS_CFG_PCP_DEI_QOS_ENA(qos),
           VTSS_X_ANA_CL_PORT_QOS_CFG_PCP_DEI_DP_ENA(qos),
           VTSS_X_ANA_CL_PORT_QOS_CFG_DSCP_QOS_ENA(qos),
           VTSS_X_ANA_CL_PORT_QOS_CFG_DSCP_DP_ENA(qos),
           wred_group);
    }

    pr("\n");

    vtss_debug_print_header(pr, "QoS Port Classification PCP, DEI to QoS class, DP level Mapping");

    pr("LP CP QoS class (8*DEI+PCP)           DP level (8*DEI+PCP)\n");
    for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {
        int pcp, dei, class_ct = 0, dpl_ct = 0;
        char class_buf[40], dpl_buf[40];
        if (!info->port_list[port_no]) {
            continue;
        }
        port = VTSS_CHIP_PORT(port_no);
        for (dei = VTSS_DEI_START; dei < VTSS_DEI_END; dei++) {
            for (pcp = VTSS_PCP_START; pcp < VTSS_PCP_END; pcp++) {
                const char *delim = ((pcp == VTSS_PCP_START) && (dei == VTSS_DEI_START)) ? "" : ",";
                JR2_RD(VTSS_ANA_CL_PORT_PCP_DEI_MAP_CFG(port, (8 * dei + pcp)), &value);
                class_ct += snprintf(class_buf + class_ct, sizeof(class_buf) - class_ct, "%s%u", delim,
                                     VTSS_X_ANA_CL_PORT_PCP_DEI_MAP_CFG_PCP_DEI_QOS_VAL(value));
                dpl_ct   += snprintf(dpl_buf   + dpl_ct,   sizeof(dpl_buf)   - dpl_ct,   "%s%u",  delim,
                                     VTSS_X_ANA_CL_PORT_PCP_DEI_MAP_CFG_PCP_DEI_DP_VAL(value));
            }
        }
        pr("%2u %2u %s %s\n", port_no, port, class_buf, dpl_buf);
    }
    pr("\n");

    vtss_debug_print_header(pr, "QoS Port Tag Remarking Config");

    pr("LP CP MPCP MDEI PCP DEI\n");
    for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {
        u32 tag_ctrl, tag_default;
        if (!info->port_list[port_no]) {
            continue;
        }
        port = VTSS_CHIP_PORT(port_no);
        JR2_RD(VTSS_REW_PORT_TAG_CTRL(port), &tag_ctrl);
        JR2_RD(VTSS_REW_PORT_PORT_VLAN_CFG(port), &tag_default);
        pr("%2u %2u %4x %4x %3d %3d\n",
           port_no,
           port,
           VTSS_X_REW_PORT_TAG_CTRL_TAG_PCP_CFG(tag_ctrl),
           VTSS_X_REW_PORT_TAG_CTRL_TAG_DEI_CFG(tag_ctrl),
           VTSS_X_REW_PORT_PORT_VLAN_CFG_PORT_PCP(tag_default),
           VTSS_X_REW_PORT_PORT_VLAN_CFG_PORT_DEI(tag_default));
    }
    pr("\n");

    vtss_debug_print_header(pr, "QoS Port Tag Remarking Map");

    pr("LP CP PCP (2*QoS class+DPL)           DEI (2*QoS class+DPL)\n");
    for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {
        int class, pcp_ct = 0, dei_ct = 0;
        u32 pcp_dp0, pcp_dp1, dei_dp0, dei_dp1;
        char pcp_buf[40], dei_buf[40];
        if (info->port_list[port_no] == 0) {
            continue;
        }
        port = VTSS_CHIP_PORT(port_no);
        for (class = VTSS_QUEUE_START; class < VTSS_QUEUE_END; class++) {
            const char *delim = (class == VTSS_QUEUE_START) ? "" : ",";
            JR2_RD(VTSS_REW_PORT_PCP_MAP_DE0(port, class), &pcp_dp0);
            JR2_RD(VTSS_REW_PORT_PCP_MAP_DE1(port, class), &pcp_dp1);
            JR2_RD(VTSS_REW_PORT_DEI_MAP_DE0(port, class), &dei_dp0);
            JR2_RD(VTSS_REW_PORT_DEI_MAP_DE1(port, class), &dei_dp1);
            pcp_ct += snprintf(pcp_buf + pcp_ct, sizeof(pcp_buf) - pcp_ct, "%s%u,%u",
                               delim,
                               VTSS_X_REW_PORT_PCP_MAP_DE0_PCP_DE0(pcp_dp0),
                               VTSS_X_REW_PORT_PCP_MAP_DE1_PCP_DE1(pcp_dp1));
            dei_ct += snprintf(dei_buf + dei_ct, sizeof(dei_buf) - dei_ct, "%s%u,%u",
                               delim,
                               VTSS_X_REW_PORT_DEI_MAP_DE0_DEI_DE0(dei_dp0),
                               VTSS_X_REW_PORT_DEI_MAP_DE1_DEI_DE1(dei_dp1));
        }
        pr("%2u %2u %s %s\n", port_no, port, pcp_buf, dei_buf);
    }
    pr("\n");

    vtss_debug_print_header(pr, "QoS Port DSCP Remarking Config");

    pr("LP CP I_Mode Keep Trans Update Remap\n");
    for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {
        u32 qos_cfg, dscp_map;
        if (info->port_list[port_no] == 0) {
            continue;
        }
        port = VTSS_CHIP_PORT(port_no);
        JR2_RD(VTSS_ANA_CL_PORT_QOS_CFG(port), &qos_cfg);
        JR2_RD(VTSS_REW_PORT_DSCP_MAP(port), &dscp_map);
        pr("%2u %2u %6u %4u %5u %6u %5u\n",
           port_no,
           port,
           VTSS_X_ANA_CL_PORT_QOS_CFG_DSCP_REWR_MODE_SEL(qos_cfg),
           VTSS_X_ANA_CL_PORT_QOS_CFG_DSCP_KEEP_ENA(qos_cfg),
           VTSS_X_ANA_CL_PORT_QOS_CFG_DSCP_TRANSLATE_ENA(qos_cfg),
           VTSS_X_REW_PORT_DSCP_MAP_DSCP_UPDATE_ENA(dscp_map),
           VTSS_X_REW_PORT_DSCP_MAP_DSCP_REMAP_ENA(dscp_map));
    }
    pr("\n");

    vtss_debug_print_header(pr, "QoS Port Policers");
    for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {
        int policer;
        if (info->port_list[port_no] == 0) {
            continue;
        }
        port = VTSS_CHIP_PORT(port_no);
        sprintf(buf, "Port %u (%u)", port, port_no);
        vtss_jr2_debug_reg_header(pr, buf);
        for (policer = 0; policer < 4; policer++) {
            pol_idx = (port * 4 + policer);
            vtss_jr2_debug_reg_inst(vtss_state, pr, VTSS_ANA_AC_POL_POL_PORT_CFG_POL_PORT_THRES_CFG_0(pol_idx) , pol_idx, "THRES_CFG_0");
            vtss_jr2_debug_reg_inst(vtss_state, pr, VTSS_ANA_AC_POL_POL_PORT_CFG_POL_PORT_THRES_CFG_1(pol_idx) , pol_idx, "THRES_CFG_1");
            vtss_jr2_debug_reg_inst(vtss_state, pr, VTSS_ANA_AC_POL_POL_PORT_CFG_POL_PORT_RATE_CFG(pol_idx) , pol_idx, "RATE_CFG");
            vtss_jr2_debug_reg_inst(vtss_state, pr, VTSS_ANA_AC_POL_POL_PORT_CTRL_POL_PORT_CFG(port, policer) , pol_idx, "POL_PORT_CFG");
        }
        vtss_jr2_debug_reg_inst(vtss_state, pr, VTSS_ANA_AC_POL_POL_PORT_CTRL_POL_PORT_GAP(port) , port, "GAP");
        vtss_jr2_debug_reg_inst(vtss_state, pr, VTSS_ANA_AC_POL_POL_ALL_CFG_POL_PORT_FC_CFG(port) , port, "POL_PORT_FC_CFG");
    }
    pr("\n");

    vtss_debug_print_header(pr, "QoS Queue Policers");
    for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {
        if (info->port_list[port_no] == 0) {
            continue;
        }
        port = VTSS_CHIP_PORT(port_no);
        sprintf(buf, "Port %u (%u)", port, port_no);
        vtss_jr2_debug_reg_header(pr, buf);
        for (queue = 0; queue < 8; queue++) {
#if defined(VTSS_ARCH_JAGUAR_2_B)
            pol_idx = (port * 8 + queue);
            vtss_jr2_debug_reg_inst(vtss_state, pr, VTSS_ANA_AC_POL_POL_PRIO_CFG_POL_PRIO_THRES_CFG(pol_idx) , pol_idx, "THRES_CFG");
            vtss_jr2_debug_reg_inst(vtss_state, pr, VTSS_ANA_AC_POL_POL_PRIO_CFG_POL_PRIO_RATE_CFG(pol_idx) , pol_idx, "RATE_CFG");
            vtss_jr2_debug_reg_inst(vtss_state, pr, VTSS_ANA_AC_POL_POL_PRIO_CTRL_POL_PRIO_GAP(pol_idx) , pol_idx, "PRIO_GAP");
            vtss_jr2_debug_reg_inst(vtss_state, pr, VTSS_ANA_AC_POL_POL_PRIO_CTRL_POL_PRIO_CFG(pol_idx) , pol_idx, "PRIO_CFG");
#else
            pol_idx = VTSS_QUEUE_POL_IDX(port, queue);
            vtss_jr2_debug_reg_inst(vtss_state, pr, VTSS_ANA_AC_POL_SDLB_MISC_CFG(pol_idx), pol_idx, "MISC_CFG");
            vtss_jr2_debug_reg_inst(vtss_state, pr, VTSS_ANA_AC_POL_SDLB_DLB_CFG(pol_idx), pol_idx, "DLB_CFG");
            vtss_jr2_debug_reg_inst(vtss_state, pr, VTSS_ANA_AC_POL_SDLB_LB_CFG(pol_idx, 0), pol_idx, "LB_CFG_0");
            vtss_jr2_debug_reg_inst(vtss_state, pr, VTSS_ANA_AC_POL_SDLB_LB_CFG(pol_idx, 1), pol_idx, "LB_CFG_1");
#endif /* VTSS_ARCH_JAGUAR_2_B */
        }
    }
    pr("\n");

    vtss_debug_print_header(pr, "QoS Hsch leak lists");
    vtss_jr2_debug_reg_header(pr, "Hsch leak lists");
    for (i = 0; i <= 2; i++) {
        u32 leak_group;
        for (leak_group = 0; leak_group <= 3; leak_group++) {
            sprintf(buf, "HSCH_LEAK_CFG_%u", i);
            vtss_jr2_debug_reg_inst(vtss_state, pr, VTSS_HSCH_HSCH_LEAK_LISTS_HSCH_LEAK_CFG(i, leak_group), leak_group, buf);
        }
    }
    pr("\n");

    vtss_debug_print_header(pr, "QoS Scheduler Config");

    pr("LP CP DWRR C0 C1 C2 C3 C4 C5 C6 C7\n");
    for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {

        if (!info->port_list[port_no]) {
            continue;
        }
        port = VTSS_CHIP_PORT(port_no);

#if defined(VTSS_ARCH_JAGUAR_2_B)
        JR2_WR(VTSS_HSCH_HSCH_MISC_HSCH_CFG_CFG,
               VTSS_F_HSCH_HSCH_MISC_HSCH_CFG_CFG_HSCH_LAYER(2) |
               VTSS_F_HSCH_HSCH_MISC_HSCH_CFG_CFG_DWRR_IDX(port));
#else
        JR2_WR(VTSS_HSCH_HSCH_MISC_HSCH_CFG_CFG,
               VTSS_F_HSCH_HSCH_MISC_HSCH_CFG_CFG_HSCH_LAYER(2) |
               VTSS_F_HSCH_HSCH_MISC_HSCH_CFG_CFG_CFG_SE_IDX(port));
#endif /* VTSS_ARCH_JAGUAR_2_B */

        JR2_RD(VTSS_HSCH_HSCH_CFG_SE_CFG(port), &value);
        pr("%2u %2u %4u",
           port_no, // Logical port
           port,    // Chip port
           VTSS_X_HSCH_HSCH_CFG_SE_CFG_SE_DWRR_CNT(value));
        for (queue = 0; queue < 8; queue++) {
            JR2_RD(VTSS_HSCH_HSCH_DWRR_DWRR_ENTRY(queue), &value);
            pr(" %2u", VTSS_X_HSCH_HSCH_DWRR_DWRR_ENTRY_DWRR_COST(value));
        }
        pr("\n");
    }
    pr("\n");

    vtss_debug_print_header(pr, "QoS Port Shapers (Uses elements from layer 2 indexed by chip port)");
    /* Select layer 2 */
    JR2_WRM(VTSS_HSCH_HSCH_MISC_HSCH_CFG_CFG,
            VTSS_F_HSCH_HSCH_MISC_HSCH_CFG_CFG_HSCH_LAYER(2),
            VTSS_M_HSCH_HSCH_MISC_HSCH_CFG_CFG_HSCH_LAYER);

    for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {
        if (info->port_list[port_no] == 0) {
            continue;
        }
        port = VTSS_CHIP_PORT(port_no);
        sprintf(buf, "Port %u (%u)", port, port_no);
        vtss_jr2_debug_reg_header(pr, buf);
        vtss_jr2_debug_reg_inst(vtss_state, pr, VTSS_HSCH_HSCH_CFG_CIR_CFG(port),      port, "HSC_CFG_CIR_CFG");
        vtss_jr2_debug_reg_inst(vtss_state, pr, VTSS_HSCH_HSCH_CFG_EIR_CFG(port),      port, "HSC_CFG_EIR_CFG");
        vtss_jr2_debug_reg_inst(vtss_state, pr, VTSS_HSCH_HSCH_CFG_SE_CFG(port),       port, "HSC_CFG_SE_CFG");
        vtss_jr2_debug_reg_inst(vtss_state, pr, VTSS_HSCH_HSCH_CFG_SE_CONNECT(port),   port, "HSC_CFG_SE_CONNECT");
        vtss_jr2_debug_reg_inst(vtss_state, pr, VTSS_HSCH_HSCH_CFG_SE_DLB_SENSE(port), port, "HSC_CFG_SE_DLB_SENSE");
    }
    pr("\n");

    vtss_debug_print_header(pr, "QoS Queue Shapers (Uses elements from layer 0 indexed by (chip port * 8) + queue)");
    /* Select layer 0 */
    JR2_WRM(VTSS_HSCH_HSCH_MISC_HSCH_CFG_CFG,
            VTSS_F_HSCH_HSCH_MISC_HSCH_CFG_CFG_HSCH_LAYER(0),
            VTSS_M_HSCH_HSCH_MISC_HSCH_CFG_CFG_HSCH_LAYER);

    for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {
        if (info->port_list[port_no] == 0) {
            continue;
        }
        port = VTSS_CHIP_PORT(port_no);
        for (queue = 0; queue < 8; queue++) {
            u32 se = (port * 8) + queue;
            sprintf(buf, "Port %u (%u), queue %u", port, port_no, queue);
            vtss_jr2_debug_reg_header(pr, buf);
            vtss_jr2_debug_reg_inst(vtss_state, pr, VTSS_HSCH_HSCH_CFG_CIR_CFG(se),      se, "HSC_CFG_CIR_CFG");
            vtss_jr2_debug_reg_inst(vtss_state, pr, VTSS_HSCH_HSCH_CFG_EIR_CFG(se),      se, "HSC_CFG_EIR_CFG");
            vtss_jr2_debug_reg_inst(vtss_state, pr, VTSS_HSCH_HSCH_CFG_SE_CFG(se),       se, "HSC_CFG_SE_CFG");
            vtss_jr2_debug_reg_inst(vtss_state, pr, VTSS_HSCH_HSCH_CFG_SE_CONNECT(se),   se, "HSC_CFG_SE_CONNECT");
            vtss_jr2_debug_reg_inst(vtss_state, pr, VTSS_HSCH_HSCH_CFG_SE_DLB_SENSE(se), se, "HSC_CFG_SE_DLB_SENSE");
        }
    }
    pr("\n");

    VTSS_RC(vtss_jr2_debug_clm_c(vtss_state, pr, info));
    return VTSS_RC_ERROR;
}

vtss_rc vtss_jr2_qos_debug_print(vtss_state_t *vtss_state,
                                 const vtss_debug_printf_t pr,
                                 const vtss_debug_info_t   *const info)
{
    return vtss_debug_print_group(VTSS_DEBUG_GROUP_QOS, jr2_debug_qos, vtss_state, pr, info);
}

/* - Initialization ------------------------------------------------ */

static vtss_rc jr2_leak_set(vtss_state_t *vtss_state, u32 layer, u32 chain, u32 min, u32 max)
{
    u32 se, se_next, time;

    /* Select layer */
    JR2_WR(VTSS_HSCH_HSCH_MISC_HSCH_CFG_CFG, VTSS_F_HSCH_HSCH_MISC_HSCH_CFG_CFG_HSCH_LAYER(layer));

    /* Setup chain */
    for (se = min; se < max; se++) {
        se_next = (se + (se < (max - 1) ? 1 : 0)); /* Last element points to itself (terminates chain) */
#if defined(VTSS_ARCH_JAGUAR_2_B)
        if (layer == 0) {
            /* Use SE_INP_IDX instead of SE_LEAK_IDX in layer 0 */
            JR2_WRM(VTSS_HSCH_HSCH_CFG_SE_CONNECT(se),
                    VTSS_F_HSCH_HSCH_CFG_SE_CONNECT_SE_INP_IDX(se_next),
                    VTSS_M_HSCH_HSCH_CFG_SE_CONNECT_SE_INP_IDX);
        } else {
            JR2_WRM(VTSS_HSCH_HSCH_CFG_SE_CONNECT(se),
                    VTSS_F_HSCH_HSCH_CFG_SE_CONNECT_SE_LEAK_IDX(se_next),
                    VTSS_M_HSCH_HSCH_CFG_SE_CONNECT_SE_LEAK_IDX);
        }
#else
        JR2_WRM(VTSS_HSCH_HSCH_CFG_SE_CONNECT(se),
                VTSS_F_HSCH_HSCH_CFG_SE_CONNECT_SE_LEAK_LINK(se_next),
                VTSS_M_HSCH_HSCH_CFG_SE_CONNECT_SE_LEAK_LINK);
#endif /* VTSS_ARCH_JAGUAR_2_B */
    }

    /* Setup for 100 kbps resolution */
#if defined(VTSS_ARCH_JAGUAR_2_B)
    time = 2500;
#else
    time = 10000;
#endif /* VTSS_ARCH_JAGUAR_2_B */

#if defined(VTSS_ARCH_JAGUAR_2_C)
    JR2_WRM(VTSS_HSCH_HSCH_LEAK_LISTS_HSCH_LEAK_CFG(layer, chain),
            VTSS_F_HSCH_HSCH_LEAK_LISTS_HSCH_LEAK_CFG_LEAK_FIRST(min),
            VTSS_M_HSCH_HSCH_LEAK_LISTS_HSCH_LEAK_CFG_LEAK_FIRST);
    JR2_WR(VTSS_HSCH_HSCH_LEAK_LISTS_HSCH_TIMER_CFG(layer, chain), time);
#else
    JR2_WRM(VTSS_HSCH_HSCH_LEAK_LISTS_HSCH_LEAK_CFG(layer, chain),
            VTSS_F_HSCH_HSCH_LEAK_LISTS_HSCH_LEAK_CFG_LEAK_TIME(time) |
            VTSS_F_HSCH_HSCH_LEAK_LISTS_HSCH_LEAK_CFG_LEAK_FIRST(min),
            VTSS_M_HSCH_HSCH_LEAK_LISTS_HSCH_LEAK_CFG_LEAK_TIME       |
            VTSS_M_HSCH_HSCH_LEAK_LISTS_HSCH_LEAK_CFG_LEAK_FIRST);
#endif /* VTSS_ARCH_JAGUAR_2_C */

    return VTSS_RC_OK;
}

static vtss_rc jr2_qos_init(vtss_state_t *vtss_state)
{
    vtss_policer_t     pol_conf;
    vtss_policer_ext_t pol_ext_conf;
    u32                port, i, half, whole;

#if defined(VTSS_ARCH_SERVAL_T) || defined(VTSS_ARCH_JAGUAR_2_C)
    /* Setup queue policer indexes */
    for (port = 0; port < VTSS_CHIP_PORTS; port++) {
        JR2_WRM(VTSS_ANA_L2_COMMON_PORT_DLB_CFG(port),
                VTSS_F_ANA_L2_COMMON_PORT_DLB_CFG_QUEUE_DLB_IDX(VTSS_QUEUE_POL_IDX(port, 0)),
                VTSS_M_ANA_L2_COMMON_PORT_DLB_CFG_QUEUE_DLB_IDX);
    }
#endif /* defined(VTSS_ARCH_SERVAL_T) || defined(VTSS_ARCH_JAGUAR_2_C) */

    memset(&pol_conf, 0, sizeof(pol_conf));
    memset(&pol_ext_conf, 0, sizeof(pol_ext_conf));
    pol_conf.rate = VTSS_BITRATE_DISABLED;
    /* Disable port and queue policers */
    for (port = 0; port <= VTSS_CHIP_PORT_CPU_1; port++) {
        for (i = 0; i < 8; i++) {
            if (i < 4) {
                VTSS_RC(jr2_port_policer_set(vtss_state, port, i, &pol_conf, &pol_ext_conf));
            }
            VTSS_RC(jr2_queue_policer_set(vtss_state, port, i, &pol_conf));
        }
    }

    /* Queue mapping: We use the defaults for QFWD:QMAP */

    /* Scheduling configuration: */

    /* Configure leak chain for scheduler elements in layer 0.
     * There is one scheduler element per queue per front port (8 * 53 = 424).
     * The scheduler elements are separated in 2 leak chains (chain 0 and 1).
     * Using less than 250 scheduler elements in a leak chain makes it possible
     * to achieve a shaper resolution of 100 kbps.
     */

    whole = VTSS_CHIP_PORTS * 8;
    half = whole / 2;

    /* Layer 0, leak chain 0, element [0; half[ */
    VTSS_RC(jr2_leak_set(vtss_state, 0, 0, 0, half));

    /* Layer 0, leaf chain 1, element [half; whole[ */
    VTSS_RC(jr2_leak_set(vtss_state, 0, 1, half, whole));

    /* Configure leak chain for scheduler elements in layer 2.
     * There is one scheduler element per front port and all
     * scheduler elements are in the same leak chain (chain 0).
     */

    /* Layer 2, leak chain 0, element [0; VTSS_CHIP_PORTS[ */
    VTSS_RC(jr2_leak_set(vtss_state, 2, 0, 0, VTSS_CHIP_PORTS));

    return VTSS_RC_OK;
}

vtss_rc vtss_jr2_qos_init(vtss_state_t *vtss_state, vtss_init_cmd_t cmd)
{
    vtss_qos_state_t   *state = &vtss_state->qos;

    switch (cmd) {
    case VTSS_INIT_CMD_CREATE:
        state->conf_set = jr2_qos_conf_set;
        state->port_conf_set = vtss_cmn_qos_port_conf_set;
        state->port_conf_update = jr2_qos_port_conf_set;
#if defined(VTSS_FEATURE_QCL)
        state->qce_add = vtss_cmn_qce_add;
        state->qce_del = vtss_cmn_qce_del;
#endif
        state->prio_count = JR2_PRIOS;
        break;
    case VTSS_INIT_CMD_INIT:
        VTSS_RC(jr2_qos_init(vtss_state));
        break;
    default:
        break;
    }
    return VTSS_RC_OK;
}

#endif /* VTSS_ARCH_JAGUAR_2 */
