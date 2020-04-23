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

//#include <cyg/infra/diag.h>
#define VTSS_TRACE_GROUP VTSS_TRACE_GROUP_PORT
#include "vtss_jaguar2_cil.h"
#if defined(VTSS_ARCH_JAGUAR_2)

#define JR2_SERDES_TYPE_1G  1
#define JR2_SERDES_TYPE_6G  6
#define JR2_SERDES_TYPE_10G 10

static vtss_rc jr2_port_inst_get(vtss_state_t *vtss_state, vtss_port_no_t port_no, u32 *tgt, u32 *serdes_inst, u32 *serdes_type)
{
    u32 t=0, t0=0, t1=0, si=0, st=0, port = VTSS_CHIP_PORT(port_no);

    if ((serdes_inst == NULL) && (serdes_type == NULL) && (vtss_state->port.conf[port_no].if_type == VTSS_PORT_INTERFACE_NO_CONNECTION)) {
        *tgt = VTSS_TO_DEV(port);
        return VTSS_RC_OK;
    }

    if (vtss_state->port.conf[port_no].if_type == VTSS_PORT_INTERFACE_QSGMII) {
        st = JR2_SERDES_TYPE_6G;
        if (port < 8) {
            // DEV1G, QSGMII on hw pin S13-S14. 6G macro 4-5
            si = ((port >> 2) + 4);  
            t0 = VTSS_TO_DEV1G_0;
            t1 = VTSS_TO_DEV1G_1;
            t  = port;
        } else if (port >= 8 && port < 32) {
            // DEV2G5, QSGMII on hw pin S15-S20. 6G macro 6-11
            si = ((port >> 2) + 4);  
            t0 = VTSS_TO_DEV2G5_0;
            t1 = VTSS_TO_DEV2G5_1;
            t  = (port - 8);
        } else if (port >= 32 && port < 48) {
            // DEV1G on QSGMII only, hw pin S21-S24. 6G macro 12-15
            si = ((port >> 2) + 4);  
            t0 = VTSS_TO_DEV1G_0;
            t1 = VTSS_TO_DEV1G_1;
            t  = (port - 24);
        } else {
            VTSS_E("Illegal port: %u", port);
            return VTSS_RC_ERROR;
        } 
    } else {
        if (port < 8) {
            // DEV1G
            st = JR2_SERDES_TYPE_1G;
            si = (port + 1);
            t0 = VTSS_TO_DEV1G_0;
            t1 = VTSS_TO_DEV1G_1;
            t  = port;
        } else if (port >= 8 && port < 32) {
            // DEV2G5
            st = JR2_SERDES_TYPE_6G;
            si = (port - 8);
            t0 = VTSS_TO_DEV2G5_0;
            t1 = VTSS_TO_DEV2G5_1;
            t  = (port - 8);
        } else if (port == VTSS_CHIP_PORT_NPI) {
            // NPI
            st = JR2_SERDES_TYPE_1G;
            si = 0;
            t0 = VTSS_TO_DEV2G5_0;
            t1 = VTSS_TO_DEV2G5_1;
            t  = 24;
        } else if (port >= VTSS_PORT_10G_START && port < VTSS_CHIP_PORTS) {
            if (vtss_state->port.conf[port_no].if_type == VTSS_PORT_INTERFACE_SFI) {
                // SFP+ 
                st = JR2_SERDES_TYPE_10G;
                si = (port - VTSS_PORT_10G_START);
                t0 = VTSS_TO_DEV10G_0;
                t1 = VTSS_TO_DEV10G_1;
                t  = (port - VTSS_PORT_10G_START);
            } else if (vtss_state->port.conf[port_no].if_type == VTSS_PORT_INTERFACE_XAUI) { 
                // XAUI/RXAUI 
                st = JR2_SERDES_TYPE_6G;
                si = (port == 49) ? 0xF0000 : (port == 50) ? 0xF00000 : (port == 51) ? 0xF00 : 0xF000;
                t0 = VTSS_TO_DEV10G_0;
                t1 = VTSS_TO_DEV10G_1;
                t  = (port - VTSS_PORT_10G_START);
            } else {
                // Dev2G5_25-28 connected to SD10G 
                st = JR2_SERDES_TYPE_10G;
                si = (port - VTSS_PORT_10G_START);
                t0 = VTSS_TO_DEV2G5_0;
                t1 = VTSS_TO_DEV2G5_1;
                t = 25 + (port - VTSS_PORT_10G_START);
            }
        } else {
            VTSS_E("Illegal port: %u", port);
            return VTSS_RC_ERROR;
        }
    }

    *tgt = (t*(t1 - t0) + t0);
    if (serdes_inst != NULL) {
        *serdes_inst = si;
    }
    if (serdes_type != NULL) {
        *serdes_type = st;
    }

    return VTSS_RC_OK;
}

/* - CIL functions ------------------------------------------------- */

#if defined(VTSS_FEATURE_SYNCE)
static vtss_rc jr2_synce_clock_out_set(vtss_state_t *vtss_state, const vtss_synce_clk_port_t clk_port)
{
    u32 div_mask = 0;
    VTSS_D("clk_port %X  enable %u\n", clk_port, vtss_state->synce.out_conf[clk_port].enable);

    switch (vtss_state->synce.out_conf[clk_port].divider) {
        case VTSS_SYNCE_DIVIDER_1: div_mask = 0; break;
        case VTSS_SYNCE_DIVIDER_4: div_mask = 2; break;
        case VTSS_SYNCE_DIVIDER_5: div_mask = 1; break;
    }
    JR2_WRM(VTSS_HSIO_SYNC_ETH_CFG_SYNC_ETH_CFG(clk_port), 
            VTSS_F_HSIO_SYNC_ETH_CFG_SYNC_ETH_CFG_SEL_RECO_CLK_DIV(div_mask) |
            VTSS_F_HSIO_SYNC_ETH_CFG_SYNC_ETH_CFG_RECO_CLK_ENA(vtss_state->synce.out_conf[clk_port].enable), 
            VTSS_M_HSIO_SYNC_ETH_CFG_SYNC_ETH_CFG_SEL_RECO_CLK_DIV |
            VTSS_M_HSIO_SYNC_ETH_CFG_SYNC_ETH_CFG_RECO_CLK_ENA);
    
    return VTSS_RC_OK;
}

static vtss_rc jr2_synce_clock_in_set(vtss_state_t *vtss_state, const vtss_synce_clk_port_t clk_port)
{
    vtss_synce_clock_in_t *conf = &vtss_state->synce.in_conf[clk_port];
    u32         serdes_type;
    u32         tgt, serdes_instance, mask;
    i32         new_chip_port = vtss_state->port.map[conf->port_no].chip_port;
    i32         clk_src;
    
    if (new_chip_port <= 23) {
        clk_src = new_chip_port + 1;
    } else if (new_chip_port >= 24 && new_chip_port <= 31) {
        clk_src = new_chip_port + 5;
    } else if (new_chip_port >= 49 && new_chip_port <= 52) {
        clk_src = new_chip_port - 24;
    } else {
        VTSS_E("SyncE not supported for port_no %u, chip port %u", conf->port_no, new_chip_port);
        return VTSS_RC_ERROR;
    }
    VTSS_D("clk_port %X, port_no %u, chip port %u enable %u\n", clk_port, conf->port_no, new_chip_port, vtss_state->synce.in_conf[clk_port].enable);
    JR2_WRM(VTSS_HSIO_SYNC_ETH_CFG_SYNC_ETH_CFG(clk_port), 
            VTSS_F_HSIO_SYNC_ETH_CFG_SYNC_ETH_CFG_SEL_RECO_CLK_SRC(clk_src) |
            VTSS_F_HSIO_SYNC_ETH_CFG_SYNC_ETH_CFG_RECO_CLK_ENA(conf->enable), 
            VTSS_M_HSIO_SYNC_ETH_CFG_SYNC_ETH_CFG_SEL_RECO_CLK_SRC |
            VTSS_M_HSIO_SYNC_ETH_CFG_SYNC_ETH_CFG_RECO_CLK_ENA);

    if (conf->enable) {
        /* Enable input clock configuration - now configuring the new (or maybe the same) input port */
        if (VTSS_RC_OK == jr2_port_inst_get(vtss_state, conf->port_no, &tgt, &serdes_instance, &serdes_type)) {
            mask = (1<<serdes_instance);
            if (serdes_type == JR2_SERDES_TYPE_1G) {
                VTSS_RC(jr2_sd1g_read(vtss_state, mask)); /* Readback the 1G common config register */
                JR2_WRM(VTSS_HSIO_SERDES1G_ANA_CFG_SERDES1G_COMMON_CFG, 
                        VTSS_F_HSIO_SERDES1G_ANA_CFG_SERDES1G_COMMON_CFG_SE_AUTO_SQUELCH_ENA(conf->enable),
                        VTSS_M_HSIO_SERDES1G_ANA_CFG_SERDES1G_COMMON_CFG_SE_AUTO_SQUELCH_ENA);
                VTSS_RC(jr2_sd1g_write(vtss_state, mask, 0)); /* transfer 1G common config register */
            }
            if (serdes_type == JR2_SERDES_TYPE_6G) {
                VTSS_RC(jr2_sd6g_read(vtss_state, mask));                /* Readback the 1G common config register */
                JR2_WRM(VTSS_HSIO_SERDES6G_ANA_CFG_SERDES6G_COMMON_CFG, 
                        VTSS_F_HSIO_SERDES6G_ANA_CFG_SERDES6G_COMMON_CFG_SE_AUTO_SQUELCH_ENA(conf->enable),
                        VTSS_M_HSIO_SERDES6G_ANA_CFG_SERDES6G_COMMON_CFG_SE_AUTO_SQUELCH_ENA);
                VTSS_RC(jr2_sd6g_write(vtss_state, mask, 0));     /* transfer 6G common config register */
            }
            if (serdes_type == JR2_SERDES_TYPE_10G) {
                JR2_WRM(VTSS_HSIO_SYNC_ETH_CFG_SYNC_ETH_SD10G_CFG(serdes_instance), 
                        VTSS_F_HSIO_SYNC_ETH_CFG_SYNC_ETH_SD10G_CFG_SD10G_RECO_CLK_DIV(1) |
                        VTSS_F_HSIO_SYNC_ETH_CFG_SYNC_ETH_SD10G_CFG_SD10G_AUTO_SQUELCH_ENA(conf->enable),
                        VTSS_M_HSIO_SYNC_ETH_CFG_SYNC_ETH_SD10G_CFG_SD10G_RECO_CLK_DIV |
                        VTSS_M_HSIO_SYNC_ETH_CFG_SYNC_ETH_SD10G_CFG_SD10G_AUTO_SQUELCH_ENA);
            }
        }
    }
    
    return VTSS_RC_OK;
}
#endif /* VTSS_FEATURE_SYNCE */

/* ================================================================= *
 *  Port control
 * ================================================================= */

static vtss_rc vtss_jr2_vaui_lane_alignement(vtss_state_t *vtss_state, u32 p, BOOL ena) {
    // VTSS_TO_VAUI0, Instance 0 maps to Serdes6G 16-19
    // VTSS_TO_VAUI0, Instance 1 maps to Serdes6G 20-23
    // VTSS_TO_VAUI1, Instance 0 maps to Serdes6G 8-11
    // VTSS_TO_VAUI2, Instance 1 maps to Serdes6G 12-15
    if (VTSS_PORT_IS_10G(p)) {
        /* Disable/enable LANE alignment */
        JR2_WRM(VTSS_VAUI_CHANNEL_VAUI_CHANNEL_CFG_VAUI_CHANNEL_CFG(p < 51 ? VTSS_TO_VAUI0 : VTSS_TO_VAUI1, !(p % 2)),
                VTSS_F_VAUI_CHANNEL_VAUI_CHANNEL_CFG_VAUI_CHANNEL_CFG_LANE_SYNC_ENA(ena ? 0xf : 0),
                VTSS_M_VAUI_CHANNEL_VAUI_CHANNEL_CFG_VAUI_CHANNEL_CFG_LANE_SYNC_ENA);
    }
    return VTSS_RC_OK;
}

static BOOL port_is_in_10g_mode(vtss_state_t *vtss_state, vtss_port_no_t port_no)
{
    if (vtss_state->port.conf[port_no].if_type == VTSS_PORT_INTERFACE_SFI ||
        vtss_state->port.conf[port_no].if_type == VTSS_PORT_INTERFACE_XAUI ||
        vtss_state->port.conf[port_no].if_type == VTSS_PORT_INTERFACE_RXAUI) {
        return 1;
    } else {
        return 0;
    }
}

vtss_rc vtss_jr2_port_max_tags_set(vtss_state_t *vtss_state, vtss_port_no_t port_no)
{
    vtss_port_max_tags_t  max_tags = vtss_state->port.conf[port_no].max_tags;
    vtss_vlan_port_type_t vlan_type = vtss_state->l2.vlan_port_conf[port_no].port_type;
    u32                   etype, port = VTSS_CHIP_PORT(port_no);

    /* S-ports and VLAN unaware ports both support 0x88a8 (in addition to 0x8100) */
    etype = (vlan_type == VTSS_VLAN_PORT_TYPE_S_CUSTOM ? vtss_state->l2.vlan_conf.s_etype :
             vlan_type == VTSS_VLAN_PORT_TYPE_C ? VTSS_ETYPE_TAG_C : VTSS_ETYPE_TAG_S);

    if (VTSS_PORT_IS_10G(port)) {
        /* As 10G and 1G devices come in pairs, we must update both devices with max tags */

        JR2_WR(VTSS_DEV10G_MAC_CFG_STATUS_MAC_TAGS_CFG(VTSS_TO_DEV(port), 0),
                VTSS_F_DEV10G_MAC_CFG_STATUS_MAC_TAGS_CFG_TAG_ID(etype));

        // Currently only supporting one user defined TAG (besides 0x8100 and 0x88A8)
        JR2_WRM(VTSS_DEV10G_MAC_CFG_STATUS_MAC_TAGS_CFG(VTSS_TO_DEV(port), 0),
                VTSS_F_DEV10G_MAC_CFG_STATUS_MAC_TAGS_CFG_TAG_ID(etype) |
                VTSS_F_DEV10G_MAC_CFG_STATUS_MAC_TAGS_CFG_TAG_ENA(max_tags == VTSS_PORT_MAX_TAGS_NONE ? 0 : 1),
                VTSS_M_DEV10G_MAC_CFG_STATUS_MAC_TAGS_CFG_TAG_ID |
                VTSS_M_DEV10G_MAC_CFG_STATUS_MAC_TAGS_CFG_TAG_ENA);

        JR2_WRM(VTSS_DEV10G_MAC_CFG_STATUS_MAC_NUM_TAGS_CFG(VTSS_TO_DEV(port)), // Triple tags not currently supported
                VTSS_F_DEV10G_MAC_CFG_STATUS_MAC_NUM_TAGS_CFG_NUM_TAGS(max_tags == VTSS_PORT_MAX_TAGS_ONE?1 : max_tags == VTSS_PORT_MAX_TAGS_TWO?2 : 0),
                VTSS_M_DEV10G_MAC_CFG_STATUS_MAC_NUM_TAGS_CFG_NUM_TAGS);
        
        JR2_WRM(VTSS_DEV10G_MAC_CFG_STATUS_MAC_MAXLEN_CFG(VTSS_TO_DEV(port)), 
                VTSS_F_DEV10G_MAC_CFG_STATUS_MAC_MAXLEN_CFG_MAX_LEN_TAG_CHK(max_tags == VTSS_PORT_MAX_TAGS_NONE ? 0 : 1),
                VTSS_M_DEV10G_MAC_CFG_STATUS_MAC_MAXLEN_CFG_MAX_LEN_TAG_CHK);
    }

    JR2_WR(VTSS_DEV1G_MAC_CFG_STATUS_MAC_TAGS_CFG(VTSS_TO_DEV1G(port)),
           VTSS_F_DEV1G_MAC_CFG_STATUS_MAC_TAGS_CFG_TAG_ID(etype) |
           VTSS_F_DEV1G_MAC_CFG_STATUS_MAC_TAGS_CFG_PB_ENA(max_tags == VTSS_PORT_MAX_TAGS_TWO ? 1 : 0) | // Triple tags not currently supported
           VTSS_F_DEV1G_MAC_CFG_STATUS_MAC_TAGS_CFG_VLAN_AWR_ENA(max_tags == VTSS_PORT_MAX_TAGS_NONE ? 0 : 1) |
           VTSS_F_DEV1G_MAC_CFG_STATUS_MAC_TAGS_CFG_VLAN_LEN_AWR_ENA(max_tags == VTSS_PORT_MAX_TAGS_NONE ? 0 : 1));

    return VTSS_RC_OK;
}


/* static vtss_rc jr2_miim_read_write(vtss_state_t *vtss_state, */
/*                                     BOOL read,  */
/*                                     u32 miim_controller,  */
/*                                     u8 miim_addr,  */
/*                                     u8 addr,  */
/*                                     u16 *value,  */
/*                                     BOOL report_errors) */
/* { */
/*     // JR2-TBD: Stub */
/*     return VTSS_RC_ERROR; */
/* } */

/* PHY commands */
#define PHY_CMD_ADDRESS  0 /* 10G: Address */
#define PHY_CMD_WRITE    1 /* 1G/10G: Write */
#define PHY_CMD_READ_INC 2 /* 1G: Read, 10G: Read and increment */
#define PHY_CMD_READ     3 /* 10G: Read */

static vtss_rc jr2_miim_cmd(vtss_state_t *vtss_state,
                            u32 cmd, u32 sof, vtss_miim_controller_t miim_controller,
                            u8 miim_addr, u8 addr, u16 *data, BOOL report_errors)
{
    u32 i, n;
    u32 value;

    switch (miim_controller) {
    case VTSS_MIIM_CONTROLLER_0:
        i = 0;
        break;
    case VTSS_MIIM_CONTROLLER_1:
        i = 1;
        break;
    case VTSS_MIIM_CONTROLLER_2:
        i = 2;
        break;
    default:
        VTSS_E("illegal miim_controller: %d", miim_controller);
        return VTSS_RC_ERROR;
    }

    /* Set Start of frame field */
    JR2_WR(VTSS_DEVCPU_GCB_MIIM_MII_CFG(i),
           VTSS_F_DEVCPU_GCB_MIIM_MII_CFG_MIIM_CFG_PRESCALE(0x32) |
           VTSS_F_DEVCPU_GCB_MIIM_MII_CFG_MIIM_ST_CFG_FIELD(sof));

    /* Read command is different for Clause 22 */
    if (sof == 1 && cmd == PHY_CMD_READ) {
        cmd = PHY_CMD_READ_INC;
    }

    /* Start command */
    JR2_WR(VTSS_DEVCPU_GCB_MIIM_MII_CMD(i),
           VTSS_M_DEVCPU_GCB_MIIM_MII_CMD_MIIM_CMD_VLD |
           VTSS_F_DEVCPU_GCB_MIIM_MII_CMD_MIIM_CMD_PHYAD(miim_addr) |
           VTSS_F_DEVCPU_GCB_MIIM_MII_CMD_MIIM_CMD_REGAD(addr) |
           VTSS_F_DEVCPU_GCB_MIIM_MII_CMD_MIIM_CMD_WRDATA(*data) |
           VTSS_F_DEVCPU_GCB_MIIM_MII_CMD_MIIM_CMD_OPR_FIELD(cmd));

    /* Wait for access to complete */
    for (n = 0; ; n++) {
        JR2_RD(VTSS_DEVCPU_GCB_MIIM_MII_STATUS(i), &value);
        if ((value & (VTSS_M_DEVCPU_GCB_MIIM_MII_STATUS_MIIM_STAT_PENDING_RD |
                      VTSS_M_DEVCPU_GCB_MIIM_MII_STATUS_MIIM_STAT_PENDING_WR)) == 0) {
            break;
        }
        if (n == 1000) {
            goto mmd_error;
        }
    }

    /* Read data */
    if (cmd == PHY_CMD_READ || cmd == PHY_CMD_READ_INC) {
        JR2_RD(VTSS_DEVCPU_GCB_MIIM_MII_DATA(i), &value);
        if (value & VTSS_M_DEVCPU_GCB_MIIM_MII_DATA_MIIM_DATA_SUCCESS) {
            goto mmd_error;
        }
        *data = VTSS_X_DEVCPU_GCB_MIIM_MII_DATA_MIIM_DATA_RDDATA(value);
    }
    return VTSS_RC_OK;

mmd_error:
    if(report_errors) {
        VTSS_E("miim failed, cmd: %s, miim_addr: %u, addr: %u, miim_controller:%u, sof:%u",
               cmd == PHY_CMD_READ ? "PHY_CMD_READ" :
               cmd == PHY_CMD_WRITE ? "PHY_CMD_WRITE" :
               cmd == PHY_CMD_ADDRESS ? "PHY_CMD_ADDRESS" : "PHY_CMD_READ_INC",
               miim_addr, addr, miim_controller, sof);
    }

    return VTSS_RC_ERROR;
}

static vtss_rc jr2_miim_read(vtss_state_t *vtss_state,
                              vtss_miim_controller_t miim_controller, 
                              u8 miim_addr, 
                              u8 addr, 
                              u16 *value, 
                              BOOL report_errors)
{
    return jr2_miim_cmd(vtss_state, PHY_CMD_READ, 1, miim_controller, miim_addr, addr, value, report_errors);
//    return jr2_miim_read_write(vtss_state, TRUE, miim_controller, miim_addr, addr, value, report_errors);
}

static vtss_rc jr2_miim_write(vtss_state_t *vtss_state,
                               vtss_miim_controller_t miim_controller, 
                               u8 miim_addr, 
                               u8 addr, 
                               u16 value, 
                               BOOL report_errors)
{
    return jr2_miim_cmd(vtss_state, PHY_CMD_WRITE, 1, miim_controller, miim_addr, addr, &value, report_errors);
//    return jr2_miim_read_write(vtss_state, FALSE, miim_controller, miim_addr, addr, &value, report_errors);
}
#if defined(VTSS_FEATURE_10G)
static vtss_rc jr2_mmd_read(vtss_state_t *vtss_state,
                            vtss_miim_controller_t miim_controller, u8 miim_addr, u8 mmd,
                            u16 addr, u16 *value, BOOL report_errors)
{

    VTSS_RC(jr2_miim_cmd(vtss_state, PHY_CMD_ADDRESS, 0, miim_controller, miim_addr, mmd,
                         &addr, report_errors));
    if (jr2_miim_cmd(vtss_state, PHY_CMD_READ, 0, miim_controller, miim_addr, mmd,
                     value, 0) != VTSS_RC_OK) {
        if (report_errors) {
            VTSS_E("jr2_mmd_read failed, miim_controller:%d miim_addr:%x, mmd:%x, addr:%x",miim_controller, miim_addr, mmd, addr);
        }
        return VTSS_RC_ERROR;
    }
    return VTSS_RC_OK;
}

/* MMD (MDIO Management Devices (10G)) read-inc */
static vtss_rc jr2_mmd_read_inc(vtss_state_t *vtss_state,
                                vtss_miim_controller_t miim_controller, u8 miim_addr, u8 mmd,
                                u16 addr, u16 *buf, u8 count, BOOL report_errors)
{
    
    VTSS_RC(jr2_miim_cmd(vtss_state, PHY_CMD_ADDRESS, 0, miim_controller, miim_addr, mmd,
                         &addr, report_errors));
    while (count > 1) {
        VTSS_RC(jr2_miim_cmd(vtss_state, PHY_CMD_READ_INC, 0, miim_controller, miim_addr, mmd,
                             buf, report_errors));
        buf++;
        count--;
    }
    VTSS_RC(jr2_miim_cmd(vtss_state, PHY_CMD_READ, 0, miim_controller, miim_addr, mmd,
                         buf, report_errors));
    return VTSS_RC_OK;
}


/* MMD (MDIO Management Devices (10G)) write */
static vtss_rc jr2_mmd_write(vtss_state_t *vtss_state,
                             vtss_miim_controller_t miim_controller,
                             u8 miim_addr, u8 mmd, u16 addr, u16 data,  BOOL report_errors)
{
    VTSS_RC(jr2_miim_cmd(vtss_state, PHY_CMD_ADDRESS, 0, miim_controller, miim_addr, mmd,
                         &addr, report_errors));
    return jr2_miim_cmd(vtss_state, PHY_CMD_WRITE, 0, miim_controller, miim_addr, mmd,
                        &data, report_errors);
}
#endif //VTSS_FEATURE_10G

static BOOL vrfy_spd_iface(vtss_port_no_t port_no, vtss_port_interface_t if_type, vtss_port_speed_t speed, BOOL *port_10g)
{
    *port_10g = 0;
    switch (if_type) {
    case VTSS_PORT_INTERFACE_SERDES:
        if (speed != VTSS_SPEED_1G) {
            VTSS_E("Serdes port interface only supports 1000M speed (port:%u speed:%d)",port_no, speed);
            return 0;
        }
        break;
    case VTSS_PORT_INTERFACE_SGMII:
    case VTSS_PORT_INTERFACE_QSGMII:
    case VTSS_PORT_INTERFACE_SGMII_CISCO:
#if defined(AQR_CHIP_CU_PHY)
        if (speed != VTSS_SPEED_1G && speed != VTSS_SPEED_100M && speed != VTSS_SPEED_10M && speed != VTSS_SPEED_2500M) {
            VTSS_E("SGMII/QSGMII port interface only supports 10/100/1000M/2.5G speeds (port:%u)",port_no);
            return 0;
        }
#else
        if (speed != VTSS_SPEED_1G && speed != VTSS_SPEED_100M && speed != VTSS_SPEED_10M) {
            VTSS_E("SGMII/QSGMII port interface only supports 10/100/1000M speeds (port:%u)",port_no);
            return 0;
        }
#endif
        break;
    case VTSS_PORT_INTERFACE_100FX:
        if (speed != VTSS_SPEED_100M) {
            VTSS_E("100FX port interface only supports 100M speed (port:%u)",port_no);
            return 0;
        }
        break;
    case VTSS_PORT_INTERFACE_SFI:
    case VTSS_PORT_INTERFACE_RXAUI:
    case VTSS_PORT_INTERFACE_XAUI:
        if (speed != VTSS_SPEED_10G) {
            VTSS_E("SFI/XAUI/RXAUI port interface only supports 10G speeds (port:%u)",port_no);
            return 0;
        }
        *port_10g = 1;
        break;
    case VTSS_PORT_INTERFACE_VAUI:
        if (speed != VTSS_SPEED_2500M) {
            VTSS_E("VAUI port interface only supports 2.5G speed (port:%u)",port_no);
            return 0;
        }
        break;
    default:
        VTSS_E("Illegal interface type (%d)",if_type);
        return 0;
    }  
    return 1;
}

static vtss_rc jr2_port_conf_get(vtss_state_t *vtss_state,
                                  const vtss_port_no_t port_no, vtss_port_conf_t *const conf)
{
    // JR2-TBD: Stub
    return VTSS_RC_ERROR;
}

/* Serdes1G: Read/write data */

/*
 * Watermark encode/decode for QSYS:RES_CTRL:RES_CFG.WM_HIGH
 * Bit 11:  Unit; 0:1, 1:16
 * Bit 10-0: Value to be multiplied with unit
 */
#define MULTIPLIER_BIT 2048

static u16 wm_enc(u16 value)
{
    if (value >= MULTIPLIER_BIT) {
        return MULTIPLIER_BIT + value / 16;
    }
    return value;
}

u32 vtss_jr2_wm_dec(u32 value)
{
    if (value >= MULTIPLIER_BIT) { 
        return (value - MULTIPLIER_BIT) * 16;
    }        
    return value;
}


static vtss_rc jr2_port_pfc(vtss_state_t *vtss_state, u32 port, vtss_port_conf_t *conf)
{
    u32 q, pfc_mask = 0;
    u32 spd = (conf->speed == VTSS_SPEED_10G) ? 1 :
              (conf->speed == VTSS_SPEED_2500M) ? 2 :
              (conf->speed == VTSS_SPEED_1G) ? 3 :
              (conf->speed == VTSS_SPEED_100M) ? 4 : 5;

    /*  ASM / Rx enable */
    for (q = 0; q < VTSS_PRIOS; q++) {
        pfc_mask |= conf->flow_control.pfc[q] ? (1 << q) : 0;
    }
    JR2_WRM(VTSS_ASM_PFC_PFC_CFG(port), 
            VTSS_F_ASM_PFC_PFC_CFG_RX_PFC_ENA(pfc_mask) |
            VTSS_F_ASM_PFC_PFC_CFG_FC_LINK_SPEED(spd),
            VTSS_M_ASM_PFC_PFC_CFG_RX_PFC_ENA |
            VTSS_M_ASM_PFC_PFC_CFG_FC_LINK_SPEED);
    
    /*  ASM must not drop PFC frames as the PFC detection is done on the cellbus interface */
    JR2_WRM(VTSS_ASM_CFG_PAUSE_CFG(port),
            VTSS_F_ASM_CFG_PAUSE_CFG_ABORT_CTRL_ENA(!VTSS_BOOL(pfc_mask)),
            VTSS_M_ASM_CFG_PAUSE_CFG_ABORT_CTRL_ENA);
  
    /*  DSM / Tx enable */
    JR2_WRM(VTSS_QRES_RES_QOS_ADV_PFC_CFG(port),
            VTSS_F_QRES_RES_QOS_ADV_PFC_CFG_TX_PFC_ENA(pfc_mask),
            VTSS_M_QRES_RES_QOS_ADV_PFC_CFG_TX_PFC_ENA);
    JR2_WRM(VTSS_DSM_CFG_ETH_PFC_CFG(port),
            VTSS_F_DSM_CFG_ETH_PFC_CFG_PFC_ENA(VTSS_BOOL(pfc_mask)),
            VTSS_M_DSM_CFG_ETH_PFC_CFG_PFC_ENA);

    /* When enabled let the queue system generate Pause frames to avoid HOL */
    JR2_WRM(VTSS_DSM_CFG_RX_PAUSE_CFG(port),
            VTSS_F_DSM_CFG_RX_PAUSE_CFG_FC_OBEY_LOCAL(VTSS_BOOL(pfc_mask)),
            VTSS_M_DSM_CFG_RX_PAUSE_CFG_RX_PAUSE_EN);

    /* // Disable Port memory */
    JR2_WR(VTSS_QRES_RES_CTRL_RES_CFG((4096 + port + 512)), 0);

    /* // Disable Shared Prio memory */
    for (q = 0; q < VTSS_PRIOS; q++) {
        JR2_WR(VTSS_QRES_RES_CTRL_RES_CFG((4096 + q + 496)), 0);
    }

    /* // Disable Shared DP memory */
    for (q = 0; q < 4; q++) {
        JR2_WR(VTSS_QRES_RES_CTRL_RES_CFG((4096 + q + 508)), 0);
    }
 
    return VTSS_RC_OK;
}

static vtss_rc jr2_port_fc_setup(vtss_state_t *vtss_state, u32 port, vtss_port_conf_t *conf)
{
    u8                *smac = &conf->flow_control.smac.addr[0];
    u32               q, pause_start = 0x77F00, pause_stop = 0x77F00, rsrv_raw = 0x77F00, atop_tot = 0x77F00;
    BOOL              pfc = 0, fc_gen = conf->flow_control.generate, fc_obey = conf->flow_control.obey;
    vtss_port_no_t    port_no;

    for (q = 0; q < VTSS_PRIOS; q++) {
        if (conf->flow_control.pfc[q]) {
            pfc = 1;
            if (fc_gen || fc_obey) {
                VTSS_E("802.3X FC and 802.1Qbb PFC cannot both be enabled, chip port %u",port);
                return VTSS_RC_ERROR;
            }
        }
    }
    /* Configure 802.1Qbb PFC */
    VTSS_RC(jr2_port_pfc(vtss_state, port, conf));

    /* Configure Standard 802.3X FC */
    for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {
        if (vtss_state->port.conf[port_no].flow_control.generate || pfc) {
            atop_tot = 0; //  Generate FC early if any port is in FC mode
        }
    }
 
    if (pfc || fc_gen || fc_obey) {
        pause_start = 8 * (VTSS_MAX_FRAME_LENGTH_STANDARD / JR2_BUFFER_CELL_SZ);
        pause_stop = 4 * (VTSS_MAX_FRAME_LENGTH_STANDARD / JR2_BUFFER_CELL_SZ);
        if (conf->max_frame_length > VTSS_MAX_FRAME_LENGTH_STANDARD) {
            rsrv_raw = (20 * VTSS_MAX_FRAME_LENGTH_STANDARD) / JR2_BUFFER_CELL_SZ;
        } else {
            rsrv_raw = 40000 / JR2_BUFFER_CELL_SZ;
        }

    }

    /* Set Pause WM hysteresis*/
    JR2_WR(VTSS_QSYS_PAUSE_CFG_PAUSE_CFG(port),
           VTSS_F_QSYS_PAUSE_CFG_PAUSE_CFG_PAUSE_START(pause_start) |
           VTSS_F_QSYS_PAUSE_CFG_PAUSE_CFG_PAUSE_STOP(pause_stop) |
           VTSS_F_QSYS_PAUSE_CFG_PAUSE_CFG_PAUSE_ENA(0)); // enabled after reset
    
    /* Set SMAC of Pause frame */
    JR2_WR(VTSS_DSM_CFG_MAC_ADDR_BASE_HIGH_CFG(port), (smac[0]<<16) | (smac[1]<<8) | smac[2]);
    JR2_WR(VTSS_DSM_CFG_MAC_ADDR_BASE_LOW_CFG(port),  (smac[3]<<16) | (smac[4]<<8) | smac[5]);

    /* Set HDX flowcontrol */
    JR2_WRM(VTSS_DSM_CFG_MAC_CFG(port), 
            VTSS_F_DSM_CFG_MAC_CFG_HDX_BACKPRESSURE(!conf->fdx),
            VTSS_M_DSM_CFG_MAC_CFG_HDX_BACKPRESSURE);

    /* Obey flowcontrol  */
    JR2_WRM(VTSS_DSM_CFG_RX_PAUSE_CFG(port),
            VTSS_F_DSM_CFG_RX_PAUSE_CFG_RX_PAUSE_EN(fc_obey),
            VTSS_M_DSM_CFG_RX_PAUSE_CFG_RX_PAUSE_EN);

    /* No ingress dropping in flowontrol  */
    JR2_WRM(VTSS_QFWD_SYSTEM_SWITCH_PORT_MODE(port),
           VTSS_F_QFWD_SYSTEM_SWITCH_PORT_MODE_INGRESS_DROP_MODE(!fc_gen),
           VTSS_M_QFWD_SYSTEM_SWITCH_PORT_MODE_INGRESS_DROP_MODE);

    JR2_WR(VTSS_QSYS_PAUSE_CFG_ATOP(port),
           VTSS_F_QSYS_PAUSE_CFG_ATOP_ATOP(wm_enc(rsrv_raw)));
  
    /*  When 'port ATOP' and 'common ATOP_TOT' are exceeded, tail dropping is activated on port */
    JR2_WR(VTSS_QSYS_PAUSE_CFG_ATOP_TOT_CFG,
           VTSS_F_QSYS_PAUSE_CFG_ATOP_TOT_CFG_ATOP_TOT(wm_enc(atop_tot)));


    return VTSS_RC_OK;
}

static vtss_rc jr2_sd10g_xfi_mode(vtss_state_t *vtss_state, vtss_serdes_mode_t mode, u32 port)
{
    u32 tgt = VTSS_TO_10G_XFI_TGT(port);

    // Set XFI to default
    JR2_WR(VTSS_XFI_SHELL_XFI_CONTROL_XFI_MODE(tgt), 0x5);
 
    if (mode == VTSS_SERDES_MODE_100FX || 
        mode == VTSS_SERDES_MODE_SGMII || 
        mode == VTSS_SERDES_MODE_1000BaseX || 
        mode == VTSS_SERDES_MODE_2G5) {
        JR2_WRM(VTSS_XFI_SHELL_XFI_CONTROL_XFI_MODE(tgt),
                VTSS_F_XFI_SHELL_XFI_CONTROL_XFI_MODE_PORT_SEL(1),
                VTSS_M_XFI_SHELL_XFI_CONTROL_XFI_MODE_PORT_SEL);
    } else {
        JR2_WRM(VTSS_XFI_SHELL_XFI_CONTROL_XFI_MODE(tgt),
                VTSS_F_XFI_SHELL_XFI_CONTROL_XFI_MODE_PORT_SEL(0),
                VTSS_M_XFI_SHELL_XFI_CONTROL_XFI_MODE_PORT_SEL);
    }

    JR2_WRM(VTSS_XFI_SHELL_XFI_CONTROL_XFI_MODE(tgt),
            VTSS_F_XFI_SHELL_XFI_CONTROL_XFI_MODE_SW_RST(0),
            VTSS_M_XFI_SHELL_XFI_CONTROL_XFI_MODE_SW_RST);

    JR2_WRM(VTSS_XFI_SHELL_XFI_CONTROL_XFI_MODE(tgt),
            VTSS_F_XFI_SHELL_XFI_CONTROL_XFI_MODE_SW_ENA(1),
            VTSS_M_XFI_SHELL_XFI_CONTROL_XFI_MODE_SW_ENA);

    JR2_WRM(VTSS_XFI_SHELL_XFI_CONTROL_XFI_MODE(tgt),
            VTSS_F_XFI_SHELL_XFI_CONTROL_XFI_MODE_ENDIAN(1),
            VTSS_M_XFI_SHELL_XFI_CONTROL_XFI_MODE_ENDIAN);

    JR2_WRM(VTSS_XFI_SHELL_XFI_CONTROL_XFI_MODE(tgt),
            VTSS_F_XFI_SHELL_XFI_CONTROL_XFI_MODE_TX_RESYNC_SHOT(1),
            VTSS_M_XFI_SHELL_XFI_CONTROL_XFI_MODE_TX_RESYNC_SHOT);
   
    return VTSS_RC_OK;
}

static vtss_rc jr2_serdes_cfg(vtss_state_t *vtss_state, const vtss_port_no_t port_no, vtss_serdes_mode_t mode)
{
    u32  serdes_inst, serdes_type, tgt, addr, port = VTSS_CHIP_PORT(port_no);

    if (mode == vtss_state->port.serdes_mode[port_no]) {
        return VTSS_RC_OK; // No change of Serdes
    }
    VTSS_RC(jr2_port_inst_get(vtss_state, port_no, &tgt, &serdes_inst, &serdes_type));

    if (!VTSS_PORT_IS_10G(port) || mode == VTSS_SERDES_MODE_1000BaseX) {
        addr = (1 << serdes_inst);
    } else {
        addr = serdes_inst;
    }

    if (mode == VTSS_SERDES_MODE_XAUI) {
        /* Enable Lane alignment */
        VTSS_RC(vtss_jr2_vaui_lane_alignement(vtss_state, port, 1));
    }

    switch (serdes_type) {
    case JR2_SERDES_TYPE_1G:
        VTSS_RC(jr2_sd1g_cfg(vtss_state, mode, addr));
        break;

    case JR2_SERDES_TYPE_6G:
        if ((mode != VTSS_SERDES_MODE_QSGMII) || (mode == VTSS_SERDES_MODE_QSGMII && ((port % 4) == 0))) {
            VTSS_RC(jr2_sd6g_cfg(vtss_state, mode, addr));
        }
        break;

    case JR2_SERDES_TYPE_10G:
        VTSS_RC(jr2_sd10g_xfi_mode(vtss_state, mode, port));
        VTSS_RC(jr2_sd10g_cfg(vtss_state, mode, port));

        /* Change to 100FX after Serdes config */
        if (mode == VTSS_SERDES_MODE_100FX) {
            JR2_WRM(VTSS_XFI_SHELL_XFI_CONTROL_XFI_MODE(VTSS_TO_10G_XFI_TGT(port)),
                    VTSS_F_XFI_SHELL_XFI_CONTROL_XFI_MODE_FX100_ENA(1),
                    VTSS_M_XFI_SHELL_XFI_CONTROL_XFI_MODE_FX100_ENA);

            JR2_WRM(VTSS_XFI_SHELL_XFI_CONTROL_XFI_MODE(VTSS_TO_10G_XFI_TGT(port)),
                    VTSS_F_XFI_SHELL_XFI_CONTROL_XFI_MODE_TX_RESYNC_SHOT(1),
                    VTSS_M_XFI_SHELL_XFI_CONTROL_XFI_MODE_TX_RESYNC_SHOT);
        }      

        break;
    }

    vtss_state->port.serdes_mode[port_no] = mode;
    return VTSS_RC_OK;
}

/* Port disable and flush procedure */
static vtss_rc jr2_port_flush(vtss_state_t *vtss_state, const vtss_port_no_t port_no, BOOL is_10g)
{
    u32 delay = 0, value, empty = 0, prio, port = VTSS_CHIP_PORT(port_no);
    u32 tgt = is_10g ? VTSS_TO_DEV(port) : VTSS_TO_DEV1G(port);

    if (vtss_state->port.conf[port_no].if_type == VTSS_PORT_INTERFACE_QSGMII) {
        VTSS_RC(jr2_port_inst_get(vtss_state, port_no, &tgt, NULL, NULL));
    }

    if (is_10g) {   
        /* 1: Reset the PCS Rx clock domain  */
        JR2_WRM_SET(VTSS_DEV10G_DEV_CFG_STATUS_DEV_RST_CTRL(tgt), 
                    VTSS_M_DEV10G_DEV_CFG_STATUS_DEV_RST_CTRL_PCS_RX_RST);

        /* 2: Disable MAC frame reception */
        JR2_WRM_CLR(VTSS_DEV10G_MAC_CFG_STATUS_MAC_ENA_CFG(tgt),
                    VTSS_M_DEV10G_MAC_CFG_STATUS_MAC_ENA_CFG_RX_ENA);
    } else {
        /* 1: Reset the PCS Rx clock domain  */
        JR2_WRM_SET(VTSS_DEV1G_DEV_CFG_STATUS_DEV_RST_CTRL(tgt),                 
                    VTSS_M_DEV1G_DEV_CFG_STATUS_DEV_RST_CTRL_PCS_RX_RST);

        /* 2: Disable MAC frame reception */
        JR2_WRM_CLR(VTSS_DEV1G_MAC_CFG_STATUS_MAC_ENA_CFG(tgt),
                    VTSS_M_DEV1G_MAC_CFG_STATUS_MAC_ENA_CFG_RX_ENA);
    }

#if defined(VTSS_FEATURE_AFI_SWC)
    /* Pause all AFI-generated frames to this port. */
    VTSS_RC(jr2_afi_pause_resume(vtss_state, port_no, FALSE));
#endif /* VTSS_FEATURE_AFI_SWC */

    /* 3: Disable traffic being sent to or from switch port */
    JR2_WRM_CLR(VTSS_QFWD_SYSTEM_SWITCH_PORT_MODE(port),
                 VTSS_M_QFWD_SYSTEM_SWITCH_PORT_MODE_PORT_ENA);

    /* 4: Disable dequeuing from the egress queues  */
    JR2_WRM_SET(VTSS_HSCH_HSCH_MISC_PORT_MODE(port),
                VTSS_M_HSCH_HSCH_MISC_PORT_MODE_DEQUEUE_DIS);

    /* 5: Disable Flowcontrol */
    JR2_WRM_CLR(VTSS_QSYS_PAUSE_CFG_PAUSE_CFG(port), VTSS_M_QSYS_PAUSE_CFG_PAUSE_CFG_PAUSE_ENA);

    /* 5.1: Disable PFC */
    JR2_WRM(VTSS_QRES_RES_QOS_ADV_PFC_CFG(port),
            VTSS_F_QRES_RES_QOS_ADV_PFC_CFG_TX_PFC_ENA(0),
            VTSS_M_QRES_RES_QOS_ADV_PFC_CFG_TX_PFC_ENA);

    /* 6: Wait a worst case time 8ms (jumbo/10Mbit) *\/ */
    VTSS_MSLEEP(8);
   
    /* 7: Flush the queues accociated with the port */
#if defined(VTSS_ARCH_JAGUAR_2_B)
    JR2_WRM_SET(VTSS_HSCH_HSCH_MISC_PORT_MODE(port), 
                 VTSS_M_HSCH_HSCH_MISC_PORT_MODE_FLUSH_ENA);
#else // VTSS_ARCH_JAGUAR_2_C
    JR2_WRM(VTSS_HSCH_HSCH_MISC_FLUSH_CTRL,
            VTSS_F_HSCH_HSCH_MISC_FLUSH_CTRL_FLUSH_PORT(port) |
            VTSS_F_HSCH_HSCH_MISC_FLUSH_CTRL_FLUSH_DST(1) |
            VTSS_F_HSCH_HSCH_MISC_FLUSH_CTRL_FLUSH_ENA(1),
            VTSS_M_HSCH_HSCH_MISC_FLUSH_CTRL_FLUSH_PORT |
            VTSS_M_HSCH_HSCH_MISC_FLUSH_CTRL_FLUSH_DST |
            VTSS_M_HSCH_HSCH_MISC_FLUSH_CTRL_FLUSH_ENA);
#endif

    /* 8: Enable dequeuing from the egress queues */
    JR2_WRM_CLR(VTSS_HSCH_HSCH_MISC_PORT_MODE(port),
                VTSS_M_HSCH_HSCH_MISC_PORT_MODE_DEQUEUE_DIS);

    /* 9: Wait until flushing is complete */
    do { 
        for (prio = 0; prio < 8; prio++) {
            JR2_RD(VTSS_QRES_RES_CTRL_RES_STAT(2048 + 8 * port + prio), &value);
            if (value == 0) {
                empty++;
            } else {
                empty = 0;
            }
        }
        VTSS_MSLEEP(1);            
        delay++;
        if (delay == 2000) {
            VTSS_E("Flush timeout chip port %u",port);
            break;
        }
    } while (empty < 16);      

    /* 10: Reset the  MAC clock domain */
    if (is_10g) {           
        JR2_WRM(VTSS_DEV10G_DEV_CFG_STATUS_DEV_RST_CTRL(tgt), 
                VTSS_F_DEV10G_DEV_CFG_STATUS_DEV_RST_CTRL_PCS_TX_RST(1) |
                VTSS_F_DEV10G_DEV_CFG_STATUS_DEV_RST_CTRL_MAC_RX_RST(1) |
                VTSS_F_DEV10G_DEV_CFG_STATUS_DEV_RST_CTRL_MAC_TX_RST(1) |
                VTSS_F_DEV10G_DEV_CFG_STATUS_DEV_RST_CTRL_SPEED_SEL(6),
                VTSS_M_DEV10G_DEV_CFG_STATUS_DEV_RST_CTRL_PCS_TX_RST |
                VTSS_M_DEV10G_DEV_CFG_STATUS_DEV_RST_CTRL_MAC_RX_RST |
                VTSS_M_DEV10G_DEV_CFG_STATUS_DEV_RST_CTRL_MAC_TX_RST |
                VTSS_M_DEV10G_DEV_CFG_STATUS_DEV_RST_CTRL_SPEED_SEL);    
    } else {
        JR2_WR(VTSS_DEV1G_DEV_CFG_STATUS_DEV_RST_CTRL(tgt), 
               VTSS_F_DEV1G_DEV_CFG_STATUS_DEV_RST_CTRL_PCS_RX_RST(0)|
               VTSS_F_DEV1G_DEV_CFG_STATUS_DEV_RST_CTRL_PCS_TX_RST(0)|
               VTSS_F_DEV1G_DEV_CFG_STATUS_DEV_RST_CTRL_MAC_RX_RST(1)|
               VTSS_F_DEV1G_DEV_CFG_STATUS_DEV_RST_CTRL_MAC_TX_RST(1)|
               VTSS_F_DEV1G_DEV_CFG_STATUS_DEV_RST_CTRL_SPEED_SEL(3));
    }
    /* 11: Clear flushing */
#if defined(VTSS_ARCH_JAGUAR_2_B)
    JR2_WRM_CLR(VTSS_HSCH_HSCH_MISC_PORT_MODE(port), 
                 VTSS_M_HSCH_HSCH_MISC_PORT_MODE_FLUSH_ENA);
#else
    JR2_WRM(VTSS_HSCH_HSCH_MISC_FLUSH_CTRL,
            VTSS_F_HSCH_HSCH_MISC_FLUSH_CTRL_FLUSH_PORT(port) |
            VTSS_F_HSCH_HSCH_MISC_FLUSH_CTRL_FLUSH_ENA(0),
            VTSS_M_HSCH_HSCH_MISC_FLUSH_CTRL_FLUSH_PORT |
            VTSS_M_HSCH_HSCH_MISC_FLUSH_CTRL_FLUSH_ENA);
#endif

    if (is_10g) {           
        /* Disable XAUI PCS */
        JR2_WRM_CLR(VTSS_DEV10G_PCS_XAUI_CONFIGURATION_PCS_XAUI_CFG(tgt), 
                    VTSS_M_DEV10G_PCS_XAUI_CONFIGURATION_PCS_XAUI_CFG_PCS_ENA);

        /* Disable RXAUI PCS */
        JR2_WRM_CLR(VTSS_DEV10G_PCS2X6G_CONFIGURATION_PCS2X6G_CFG(tgt), 
                    VTSS_M_DEV10G_PCS2X6G_CONFIGURATION_PCS2X6G_CFG_PCS_ENA);

        /* Disable SFI PCS */
        JR2_WRM_CLR(VTSS_PCS_10GBASE_R_PCS_10GBR_CFG_PCS_CFG(VTSS_TO_10G_PCS_TGT(port)), 
                    VTSS_M_PCS_10GBASE_R_PCS_10GBR_CFG_PCS_CFG_PCS_ENA);
    }

    /* The port is disabled and flushed */

    return VTSS_RC_OK;
}


static vtss_rc jr2_port_conf_1g_set(vtss_state_t *vtss_state, const vtss_port_no_t port_no)
{
    vtss_port_conf_t       *conf  = &vtss_state->port.conf[port_no];
    u32                    port   = VTSS_CHIP_PORT(port_no);
    u32                    tgt, tx_gap, hdx_gap_1 = 0, hdx_gap_2 = 0;
    vtss_port_speed_t      speed = conf->speed;
    u32                    value;
    BOOL                   fdx = conf->fdx, disable = conf->power_down;
    BOOL                   sgmii = 0, if_100fx = 0;
    vtss_serdes_mode_t     serdes_mode = VTSS_SERDES_MODE_SGMII;   

    // Find device target and serdes macro for this chip port
    VTSS_RC(jr2_port_inst_get(vtss_state, port_no, &tgt, NULL, NULL));

    // Find serdes mode
    switch (conf->if_type) {
    case VTSS_PORT_INTERFACE_NO_CONNECTION:
        disable = 1;
        break;
    case VTSS_PORT_INTERFACE_INTERNAL:
    case VTSS_PORT_INTERFACE_SGMII:
#if defined(AQR_CHIP_CU_PHY)
        serdes_mode = (speed == VTSS_SPEED_2500M ? VTSS_SERDES_MODE_2G5 : VTSS_SERDES_MODE_SGMII);
        sgmii = (serdes_mode == VTSS_SERDES_MODE_SGMII ? 1: 0);
#else
        serdes_mode = VTSS_SERDES_MODE_SGMII;
        sgmii = 1;
#endif
        break;
    case VTSS_PORT_INTERFACE_QSGMII:
        serdes_mode = VTSS_SERDES_MODE_QSGMII;
        sgmii = 1;
        JR2_WRM(VTSS_HSIO_HW_CFGSTAT_HW_CFG, VTSS_BIT(port/4), VTSS_BIT(port/4));
        break;
    case VTSS_PORT_INTERFACE_SERDES:
        serdes_mode = VTSS_SERDES_MODE_1000BaseX;
        break;
    case VTSS_PORT_INTERFACE_100FX:
        serdes_mode = VTSS_SERDES_MODE_100FX;
        if_100fx = 1;      
        break;
    case VTSS_PORT_INTERFACE_SGMII_CISCO:
        serdes_mode = VTSS_SERDES_MODE_1000BaseX;
        sgmii = 1;        
        break;
    case VTSS_PORT_INTERFACE_VAUI:
        serdes_mode = (speed == VTSS_SPEED_2500M ? VTSS_SERDES_MODE_2G5 : VTSS_SERDES_MODE_1000BaseX);
        break; 
    default:
        VTSS_E("illegal interface, port %u", port_no);
        return VTSS_RC_ERROR;
    }

    if (VTSS_PORT_IS_10G(port)) {
        u32  bt_fld = (port == 49) ? 12 : (port == 50) ? 14 : (port == 51) ? 16 : 18;
        /* Configure the 10G Mux mode to DEV2G5 */
        JR2_WRM(VTSS_HSIO_HW_CFGSTAT_HW_CFG,
                VTSS_ENCODE_BITFIELD(3, bt_fld, 2),
                VTSS_ENCODE_BITMASK(bt_fld, 2));

        /* Must be 1 when the 10G port is attached to DEV2G5*/
        JR2_WRM(VTSS_DSM_CFG_DEV_TX_STOP_WM_CFG(port),
                VTSS_F_DSM_CFG_DEV_TX_STOP_WM_CFG_DEV10G_SHADOW_ENA(1),
                VTSS_M_DSM_CFG_DEV_TX_STOP_WM_CFG_DEV10G_SHADOW_ENA);

    }

    /* Enable the Serdes if disabled */
    if (vtss_state->port.serdes_mode[port_no] == VTSS_SERDES_MODE_DISABLE) {
        VTSS_RC(jr2_serdes_cfg(vtss_state, port_no, serdes_mode));
    }

   /* Port disable and flush procedure: */
    VTSS_RC(jr2_port_flush(vtss_state, port_no, FALSE));

    /* Configure the Serdes Macro to 'serdes_mode' */
    if (serdes_mode != vtss_state->port.serdes_mode[port_no]) {
        VTSS_RC(jr2_serdes_cfg(vtss_state, port_no, serdes_mode));
    }

    /* Bugzilla 4388: disabling frame aging when in HDX */
    JR2_WRM_CTL(VTSS_HSCH_HSCH_MISC_PORT_MODE(port), fdx, VTSS_M_HSCH_HSCH_MISC_PORT_MODE_AGE_DIS);

    /* GIG/FDX mode */
    if (fdx) {
        value = VTSS_M_DEV1G_MAC_CFG_STATUS_MAC_MODE_CFG_FDX_ENA;
        if (speed == VTSS_SPEED_1G || speed == VTSS_SPEED_2500M) {
            value |= VTSS_M_DEV1G_MAC_CFG_STATUS_MAC_MODE_CFG_GIGA_MODE_ENA;
        }
    } else {        
        value = 0;
    }
    JR2_WR(VTSS_DEV1G_MAC_CFG_STATUS_MAC_MODE_CFG(tgt), value);

    /* Default gaps */
    tx_gap = (speed == VTSS_SPEED_1G) ? 4 : fdx ? 6 : 5;
    if (speed == VTSS_SPEED_100M) {
        hdx_gap_1 = 1;
        hdx_gap_2 = 4;
    } else if (speed == VTSS_SPEED_10M) {
        hdx_gap_1 = 2;
        hdx_gap_2 = 1;
    }

    /* Non default gaps */
    if (conf->frame_gaps.hdx_gap_1 != VTSS_FRAME_GAP_DEFAULT) {
        hdx_gap_1 = conf->frame_gaps.hdx_gap_1;
    }
    if (conf->frame_gaps.hdx_gap_2 != VTSS_FRAME_GAP_DEFAULT) {
        hdx_gap_2 = conf->frame_gaps.hdx_gap_2;
    }
    if (conf->frame_gaps.fdx_gap != VTSS_FRAME_GAP_DEFAULT) {
        tx_gap = conf->frame_gaps.fdx_gap;
    }
    
    /* Set MAC IFG Gaps */
    JR2_WR(VTSS_DEV1G_MAC_CFG_STATUS_MAC_IFG_CFG(tgt), 
            VTSS_F_DEV1G_MAC_CFG_STATUS_MAC_IFG_CFG_TX_IFG(tx_gap) |
            VTSS_F_DEV1G_MAC_CFG_STATUS_MAC_IFG_CFG_RX_IFG1(hdx_gap_1) |
            VTSS_F_DEV1G_MAC_CFG_STATUS_MAC_IFG_CFG_RX_IFG2(hdx_gap_2));
    
    /* Set MAC HDX late collision */
    JR2_WRM(VTSS_DEV1G_MAC_CFG_STATUS_MAC_HDX_CFG(tgt),
            VTSS_F_DEV1G_MAC_CFG_STATUS_MAC_HDX_CFG_LATE_COL_POS(67) |
            VTSS_F_DEV1G_MAC_CFG_STATUS_MAC_HDX_CFG_RETRY_AFTER_EXC_COL_ENA(conf->exc_col_cont),
            VTSS_M_DEV1G_MAC_CFG_STATUS_MAC_HDX_CFG_LATE_COL_POS |
            VTSS_M_DEV1G_MAC_CFG_STATUS_MAC_HDX_CFG_RETRY_AFTER_EXC_COL_ENA);

    /* PCS settings for 100fx/SGMII/SERDES */
    if (if_100fx) {

        /* 100FX PCS */                    
        JR2_WRM(VTSS_DEV1G_PCS_FX100_CONFIGURATION_PCS_FX100_CFG(tgt),
                VTSS_F_DEV1G_PCS_FX100_CONFIGURATION_PCS_FX100_CFG_PCS_ENA(!disable) |
                VTSS_F_DEV1G_PCS_FX100_CONFIGURATION_PCS_FX100_CFG_SD_SEL(!conf->sd_internal) |
                VTSS_F_DEV1G_PCS_FX100_CONFIGURATION_PCS_FX100_CFG_SD_POL(conf->sd_active_high) |
                VTSS_F_DEV1G_PCS_FX100_CONFIGURATION_PCS_FX100_CFG_SD_ENA(conf->sd_enable),
                VTSS_M_DEV1G_PCS_FX100_CONFIGURATION_PCS_FX100_CFG_PCS_ENA |
                VTSS_M_DEV1G_PCS_FX100_CONFIGURATION_PCS_FX100_CFG_SD_SEL |
                VTSS_M_DEV1G_PCS_FX100_CONFIGURATION_PCS_FX100_CFG_SD_POL |
                VTSS_M_DEV1G_PCS_FX100_CONFIGURATION_PCS_FX100_CFG_SD_ENA);
    } else {
        /* Disable 100FX */
        JR2_WRM_CLR(VTSS_DEV1G_PCS_FX100_CONFIGURATION_PCS_FX100_CFG(tgt),
                     VTSS_M_DEV1G_PCS_FX100_CONFIGURATION_PCS_FX100_CFG_PCS_ENA);

        /* Choose SGMII or Serdes PCS mode */
        JR2_WR(VTSS_DEV1G_PCS1G_CFG_STATUS_PCS1G_MODE_CFG(tgt), 
               VTSS_F_DEV1G_PCS1G_CFG_STATUS_PCS1G_MODE_CFG_SGMII_MODE_ENA(sgmii));
        
        if (sgmii) {
            /* Set whole register */
            JR2_WR(VTSS_DEV1G_PCS1G_CFG_STATUS_PCS1G_ANEG_CFG(tgt),
                   VTSS_F_DEV1G_PCS1G_CFG_STATUS_PCS1G_ANEG_CFG_SW_RESOLVE_ENA(1));
        } else {
            /* Clear specific bit only */
            JR2_WRM_CLR(VTSS_DEV1G_PCS1G_CFG_STATUS_PCS1G_ANEG_CFG(tgt),
                         VTSS_M_DEV1G_PCS1G_CFG_STATUS_PCS1G_ANEG_CFG_SW_RESOLVE_ENA);
        }

        JR2_WR(VTSS_DEV1G_PCS1G_CFG_STATUS_PCS1G_SD_CFG(tgt),
               VTSS_F_DEV1G_PCS1G_CFG_STATUS_PCS1G_SD_CFG_SD_POL(conf->sd_active_high) |
               VTSS_F_DEV1G_PCS1G_CFG_STATUS_PCS1G_SD_CFG_SD_ENA(conf->sd_enable) |
               VTSS_F_DEV1G_PCS1G_CFG_STATUS_PCS1G_SD_CFG_SD_SEL(!conf->sd_internal));

        /* Enable/disable PCS */
        JR2_WR(VTSS_DEV1G_PCS1G_CFG_STATUS_PCS1G_CFG(tgt), 
               VTSS_F_DEV1G_PCS1G_CFG_STATUS_PCS1G_CFG_PCS_ENA(!disable));
        
        if (conf->if_type == VTSS_PORT_INTERFACE_SGMII) {
            JR2_WR(VTSS_DEV1G_PCS1G_CFG_STATUS_PCS1G_ANEG_CFG(tgt), 0);
        } else if (conf->if_type == VTSS_PORT_INTERFACE_SGMII_CISCO) {
            /* Complete SGMII aneg */
            JR2_WR(VTSS_DEV1G_PCS1G_CFG_STATUS_PCS1G_ANEG_CFG(tgt),
                   VTSS_F_DEV1G_PCS1G_CFG_STATUS_PCS1G_ANEG_CFG_ADV_ABILITY(0x0001) |
                   VTSS_F_DEV1G_PCS1G_CFG_STATUS_PCS1G_ANEG_CFG_SW_RESOLVE_ENA(1) |
                   VTSS_F_DEV1G_PCS1G_CFG_STATUS_PCS1G_ANEG_CFG_ANEG_ENA(1) |
                   VTSS_F_DEV1G_PCS1G_CFG_STATUS_PCS1G_ANEG_CFG_ANEG_RESTART_ONE_SHOT(1));

            /* Clear the sticky bits */
            JR2_RD(VTSS_DEV1G_PCS1G_CFG_STATUS_PCS1G_STICKY(tgt), &value);
            JR2_WR(VTSS_DEV1G_PCS1G_CFG_STATUS_PCS1G_STICKY(tgt), value);
        }           
    }

    JR2_WRM_CTL(VTSS_DEV1G_PCS1G_CFG_STATUS_PCS1G_LB_CFG(tgt), 
                 conf->loop == VTSS_PORT_LOOP_PCS_HOST, 
                VTSS_F_DEV1G_PCS1G_CFG_STATUS_PCS1G_LB_CFG_TBI_HOST_LB_ENA(1));

    /* Set Max Length */
    JR2_WRM(VTSS_DEV1G_MAC_CFG_STATUS_MAC_MAXLEN_CFG(tgt), 
            VTSS_F_DEV1G_MAC_CFG_STATUS_MAC_MAXLEN_CFG_MAX_LEN(conf->max_frame_length),
            VTSS_M_DEV1G_MAC_CFG_STATUS_MAC_MAXLEN_CFG_MAX_LEN);

    /* Configure frame length check (from ethertype / length field) */
    JR2_WRM(VTSS_DEV1G_MAC_CFG_STATUS_MAC_ADV_CHK_CFG(tgt), 
            VTSS_F_DEV1G_MAC_CFG_STATUS_MAC_ADV_CHK_CFG_LEN_DROP_ENA(conf->frame_length_chk),
            VTSS_M_DEV1G_MAC_CFG_STATUS_MAC_ADV_CHK_CFG_LEN_DROP_ENA);

    /* Must be 1 for 10/100 */
    JR2_WRM(VTSS_DSM_CFG_DEV_TX_STOP_WM_CFG(port), 
            VTSS_F_DSM_CFG_DEV_TX_STOP_WM_CFG_DEV_TX_STOP_WM((speed == VTSS_SPEED_10M || speed == VTSS_SPEED_100M) ? 1 : 0),
            VTSS_M_DSM_CFG_DEV_TX_STOP_WM_CFG_DEV_TX_STOP_WM);

    if (!disable) {
        /* Configure flow control */
        if (jr2_port_fc_setup(vtss_state, port, conf) != VTSS_RC_OK) {
            VTSS_E("Could not configure FC port: %u", port);
        }
        /* Update policer flow control configuration */
        VTSS_RC(vtss_jr2_port_policer_fc_set(vtss_state, port_no, port));

        /* Enable MAC module */
        JR2_WR(VTSS_DEV1G_MAC_CFG_STATUS_MAC_ENA_CFG(tgt), 
               VTSS_M_DEV1G_MAC_CFG_STATUS_MAC_ENA_CFG_RX_ENA |
               VTSS_M_DEV1G_MAC_CFG_STATUS_MAC_ENA_CFG_TX_ENA);
        
        /* Take MAC, Port, Phy (intern) and PCS (SGMII/Serdes) clock out of reset */
        JR2_WR(VTSS_DEV1G_DEV_CFG_STATUS_DEV_RST_CTRL(tgt),
               VTSS_F_DEV1G_DEV_CFG_STATUS_DEV_RST_CTRL_SPEED_SEL(speed == VTSS_SPEED_10M ? 0 : speed == VTSS_SPEED_100M ? 1 : 2));

        /* Clear the PCS stickys */
        JR2_WR(VTSS_DEV1G_PCS1G_CFG_STATUS_PCS1G_STICKY(tgt),
                VTSS_M_DEV1G_PCS1G_CFG_STATUS_PCS1G_STICKY_LINK_DOWN_STICKY |
                VTSS_M_DEV1G_PCS1G_CFG_STATUS_PCS1G_STICKY_OUT_OF_SYNC_STICKY);


        /* Core: This is a 1G/2G5 port (fwd_speed) and enable port for frame transfer */
#if defined(VTSS_ARCH_JAGUAR_2_B)
        JR2_WRM(VTSS_QFWD_SYSTEM_SWITCH_PORT_MODE(port),
                VTSS_F_QFWD_SYSTEM_SWITCH_PORT_MODE_FWD_SPEED(speed == VTSS_SPEED_2500M ? 1 : 0) |
                VTSS_F_QFWD_SYSTEM_SWITCH_PORT_MODE_PORT_ENA(1),
                VTSS_M_QFWD_SYSTEM_SWITCH_PORT_MODE_FWD_SPEED |
                VTSS_M_QFWD_SYSTEM_SWITCH_PORT_MODE_PORT_ENA);
#else
        JR2_WRM(VTSS_QFWD_SYSTEM_SWITCH_PORT_MODE(port),
                VTSS_F_QFWD_SYSTEM_SWITCH_PORT_MODE_PORT_ENA(1) |
                VTSS_F_QFWD_SYSTEM_SWITCH_PORT_MODE_FWD_URGENCY(speed == VTSS_SPEED_2500M ? 41 : 104), //( 270000/6400 - 1) : (672000/6400 - 1)
                VTSS_M_QFWD_SYSTEM_SWITCH_PORT_MODE_PORT_ENA |
                VTSS_M_QFWD_SYSTEM_SWITCH_PORT_MODE_FWD_URGENCY);
#endif

#if defined(VTSS_FEATURE_AFI_SWC)
        /* Resume any AFI-generated frames to this port. This will only happen if using the
         * FDMA Driver to inject frames. An external CPU will have to make sure that
         * the frame is re-injected once link-up event is seen.
         */
        VTSS_RC(jr2_afi_pause_resume(vtss_state, port_no, TRUE));
#endif /*V TSS_FEATURE_AFI */

        /* Enable flowcontrol - must be done after the port is enabled */
        if (conf->flow_control.generate) {
            JR2_WRM_SET(VTSS_QSYS_PAUSE_CFG_PAUSE_CFG(port), VTSS_M_QSYS_PAUSE_CFG_PAUSE_CFG_PAUSE_ENA);
        }
    } 

    VTSS_D("chip port: %u (1G),is configured", port);
    return VTSS_RC_OK;
}

static vtss_rc jr2_port_conf_10g_set(vtss_state_t *vtss_state, const vtss_port_no_t port_no)
{
    vtss_port_conf_t   *conf = &vtss_state->port.conf[port_no];
    u32                port = VTSS_CHIP_PORT(port_no);
    u32                tgt;
    vtss_port_speed_t  speed = conf->speed;
    BOOL               rx_flip = conf->xaui_rx_lane_flip;
    BOOL               tx_flip = conf->xaui_tx_lane_flip;
    u32                mode_10g = 0;
    u32                link_speed = 4;
    u32                bt_fld = (port == 49) ? 12 : (port == 50) ? 14 : (port == 51) ? 16 : 18;
    vtss_serdes_mode_t serdes_mode;

    // Find device target and serdes macro for this chip port
    VTSS_RC(jr2_port_inst_get(vtss_state, port_no, &tgt, NULL, NULL));
   
    /* Verify speed */
    switch (speed) {
    case VTSS_SPEED_10G:
        break;
    default:
        VTSS_E("Illegal speed: %d", speed);
        return VTSS_RC_ERROR;
    }

    VTSS_D("chip port: %u interface:%d", port,conf->if_type);
    switch (conf->if_type) {
    case VTSS_PORT_INTERFACE_XAUI:
        serdes_mode = VTSS_SERDES_MODE_XAUI;
        mode_10g = 1;
        break;
    case VTSS_PORT_INTERFACE_RXAUI:
        serdes_mode = VTSS_SERDES_MODE_RXAUI;
        mode_10g = 2;
        break;
    case VTSS_PORT_INTERFACE_SFI:
        serdes_mode = conf->serdes.sfp_dac ? VTSS_SERDES_MODE_SFI_DAC : VTSS_SERDES_MODE_SFI;
        mode_10g = 0;
        link_speed = 7;
        break;
    case VTSS_PORT_INTERFACE_LOOPBACK:
        serdes_mode = VTSS_SERDES_MODE_1000BaseX;
        mode_10g = 3;
        break;
    default:
        VTSS_E("Illegal interface type");
        return VTSS_RC_ERROR;
    }  

    /* Configure the 10G Mux mode  */
    JR2_WRM(VTSS_HSIO_HW_CFGSTAT_HW_CFG,
            VTSS_ENCODE_BITFIELD(mode_10g, bt_fld,2),
            VTSS_ENCODE_BITMASK(bt_fld,2));

   /* This is a 10G port only */
    JR2_WRM(VTSS_DSM_CFG_DEV_TX_STOP_WM_CFG(port),
            VTSS_F_DSM_CFG_DEV_TX_STOP_WM_CFG_DEV_TX_STOP_WM(0) |
            VTSS_F_DSM_CFG_DEV_TX_STOP_WM_CFG_DEV10G_SHADOW_ENA(0),
            VTSS_M_DSM_CFG_DEV_TX_STOP_WM_CFG_DEV_TX_STOP_WM |
            VTSS_M_DSM_CFG_DEV_TX_STOP_WM_CFG_DEV10G_SHADOW_ENA);

    /* Enable the Serdes if disabled */
    if (vtss_state->port.serdes_mode[port_no] == VTSS_SERDES_MODE_DISABLE) {
        VTSS_RC(jr2_serdes_cfg(vtss_state, port_no, serdes_mode));
    }

   /* Port disable and flush procedure: */
    VTSS_RC(jr2_port_flush(vtss_state, port_no, TRUE));

   /* Re-configure Serdes if needed */
    if (serdes_mode != vtss_state->port.serdes_mode[port_no]) {
        VTSS_RC(jr2_serdes_cfg(vtss_state, port_no, serdes_mode));
    }

    if (conf->if_type == VTSS_PORT_INTERFACE_XAUI) {
        /* XAUI: Handle Signal Detect */
        JR2_WR(VTSS_DEV10G_PCS_XAUI_CONFIGURATION_PCS_XAUI_SD_CFG(tgt), 
               VTSS_F_DEV10G_PCS_XAUI_CONFIGURATION_PCS_XAUI_SD_CFG_SD_POL(conf->sd_active_high) |
               VTSS_F_DEV10G_PCS_XAUI_CONFIGURATION_PCS_XAUI_SD_CFG_SD_SEL(!conf->sd_internal) |
               VTSS_F_DEV10G_PCS_XAUI_CONFIGURATION_PCS_XAUI_SD_CFG_SD_ENA(conf->sd_enable));
                
        /* Enable XAUI PCS */
        JR2_WRM_SET(VTSS_DEV10G_PCS_XAUI_CONFIGURATION_PCS_XAUI_CFG(tgt), 
                    VTSS_M_DEV10G_PCS_XAUI_CONFIGURATION_PCS_XAUI_CFG_PCS_ENA);

        /* XAUI Lane flipping  */
        JR2_WRM(VTSS_DEV10G_PCS_XAUI_CONFIGURATION_PCS_XAUI_EXT_CFG(tgt),
                VTSS_F_DEV10G_PCS_XAUI_CONFIGURATION_PCS_XAUI_EXT_CFG_RX_FLIP_HMBUS(rx_flip) |
                VTSS_F_DEV10G_PCS_XAUI_CONFIGURATION_PCS_XAUI_EXT_CFG_TX_FLIP_HMBUS(tx_flip),
                VTSS_M_DEV10G_PCS_XAUI_CONFIGURATION_PCS_XAUI_EXT_CFG_RX_FLIP_HMBUS |
                VTSS_M_DEV10G_PCS_XAUI_CONFIGURATION_PCS_XAUI_EXT_CFG_TX_FLIP_HMBUS);

    } else if (conf->if_type == VTSS_PORT_INTERFACE_RXAUI) {
        /*  RXAUI: Enable RXAUI PCS and Handle Signal Detect */
        JR2_WRM(VTSS_DEV10G_PCS2X6G_CONFIGURATION_PCS2X6G_CFG(tgt), 
                VTSS_F_DEV10G_PCS2X6G_CONFIGURATION_PCS2X6G_CFG_PCS_ENA(1) |
                VTSS_F_DEV10G_PCS2X6G_CONFIGURATION_PCS2X6G_CFG_SD_POL(conf->sd_active_high) |
                VTSS_F_DEV10G_PCS2X6G_CONFIGURATION_PCS2X6G_CFG_SD_SEL(!conf->sd_internal) |
                VTSS_F_DEV10G_PCS2X6G_CONFIGURATION_PCS2X6G_CFG_SD_ENA(conf->sd_enable),
                VTSS_M_DEV10G_PCS2X6G_CONFIGURATION_PCS2X6G_CFG_PCS_ENA |
                VTSS_M_DEV10G_PCS2X6G_CONFIGURATION_PCS2X6G_CFG_SD_POL |
                VTSS_M_DEV10G_PCS2X6G_CONFIGURATION_PCS2X6G_CFG_SD_SEL |
                VTSS_M_DEV10G_PCS2X6G_CONFIGURATION_PCS2X6G_CFG_SD_ENA);
    } else if (conf->if_type == VTSS_PORT_INTERFACE_SFI) {
        /* SFI: Handle Signal Detect */
        JR2_WR(VTSS_PCS_10GBASE_R_PCS_10GBR_CFG_PCS_SD_CFG(VTSS_TO_10G_PCS_TGT(port)),
               VTSS_F_PCS_10GBASE_R_PCS_10GBR_CFG_PCS_SD_CFG_SD_POL(conf->sd_active_high) |
               VTSS_F_PCS_10GBASE_R_PCS_10GBR_CFG_PCS_SD_CFG_SD_SEL(!conf->sd_internal) |
               VTSS_F_PCS_10GBASE_R_PCS_10GBR_CFG_PCS_SD_CFG_SD_ENA(conf->sd_enable));

        /* Enable SFI PCS */
        JR2_WRM_SET(VTSS_PCS_10GBASE_R_PCS_10GBR_CFG_PCS_CFG(VTSS_TO_10G_PCS_TGT(port)),
                    VTSS_M_PCS_10GBASE_R_PCS_10GBR_CFG_PCS_CFG_PCS_ENA);

    } else if (conf->if_type == VTSS_PORT_INTERFACE_LOOPBACK) {

    } else {
        VTSS_E("Interface config not done yet");
        return VTSS_RC_ERROR;
    }

    /* Set Max Length and maximum tags allowed */
    JR2_WRM(VTSS_DEV10G_MAC_CFG_STATUS_MAC_MAXLEN_CFG(tgt), 
            VTSS_F_DEV10G_MAC_CFG_STATUS_MAC_MAXLEN_CFG_MAX_LEN(conf->max_frame_length),
            VTSS_M_DEV10G_MAC_CFG_STATUS_MAC_MAXLEN_CFG_MAX_LEN);

    /* Configure framelength check (from ethertype / length field) */
    JR2_WRM(VTSS_DEV10G_MAC_CFG_STATUS_MAC_ADV_CHK_CFG(tgt), 
            VTSS_F_DEV10G_MAC_CFG_STATUS_MAC_ADV_CHK_CFG_INR_ERR_ENA(conf->frame_length_chk),
            VTSS_M_DEV10G_MAC_CFG_STATUS_MAC_ADV_CHK_CFG_INR_ERR_ENA);

    if (!conf->power_down) {
        /* Configure flow control */
        if (jr2_port_fc_setup(vtss_state, port, conf) != VTSS_RC_OK) {
            VTSS_E("Could not configure FC port: %u", port);
        }
        /* Enable MAC module */
        JR2_WR(VTSS_DEV10G_MAC_CFG_STATUS_MAC_ENA_CFG(tgt), 
               VTSS_M_DEV10G_MAC_CFG_STATUS_MAC_ENA_CFG_RX_ENA |
               VTSS_M_DEV10G_MAC_CFG_STATUS_MAC_ENA_CFG_TX_ENA);
        
        JR2_WRM(VTSS_DEV10G_DEV_CFG_STATUS_DEV_RST_CTRL(tgt), 
                VTSS_F_DEV10G_DEV_CFG_STATUS_DEV_RST_CTRL_PCS_RX_RST(0) |
                VTSS_F_DEV10G_DEV_CFG_STATUS_DEV_RST_CTRL_PCS_TX_RST(0) |
                VTSS_F_DEV10G_DEV_CFG_STATUS_DEV_RST_CTRL_MAC_RX_RST(0) |
                VTSS_F_DEV10G_DEV_CFG_STATUS_DEV_RST_CTRL_MAC_TX_RST(0) |
                VTSS_F_DEV10G_DEV_CFG_STATUS_DEV_RST_CTRL_SPEED_SEL(link_speed),
                VTSS_M_DEV10G_DEV_CFG_STATUS_DEV_RST_CTRL_PCS_RX_RST |
                VTSS_M_DEV10G_DEV_CFG_STATUS_DEV_RST_CTRL_PCS_TX_RST |
                VTSS_M_DEV10G_DEV_CFG_STATUS_DEV_RST_CTRL_MAC_RX_RST |
                VTSS_M_DEV10G_DEV_CFG_STATUS_DEV_RST_CTRL_MAC_TX_RST |
                VTSS_M_DEV10G_DEV_CFG_STATUS_DEV_RST_CTRL_SPEED_SEL);
        	
        /* Core: This is a 10G port (fwd_speed) and enable port for frame transfer */
#if defined(VTSS_ARCH_JAGUAR_2_B)
        JR2_WRM(VTSS_QFWD_SYSTEM_SWITCH_PORT_MODE(port),
                VTSS_F_QFWD_SYSTEM_SWITCH_PORT_MODE_FWD_SPEED(2) |
                VTSS_F_QFWD_SYSTEM_SWITCH_PORT_MODE_PORT_ENA(1),
                VTSS_M_QFWD_SYSTEM_SWITCH_PORT_MODE_FWD_SPEED |
                VTSS_M_QFWD_SYSTEM_SWITCH_PORT_MODE_PORT_ENA);
#else /* VTSS_ARCH_JAGUAR_2_C */
        JR2_WRM(VTSS_QFWD_SYSTEM_SWITCH_PORT_MODE(port),
                VTSS_F_QFWD_SYSTEM_SWITCH_PORT_MODE_PORT_ENA(1) |
                VTSS_F_QFWD_SYSTEM_SWITCH_PORT_MODE_FWD_URGENCY(9),
                VTSS_M_QFWD_SYSTEM_SWITCH_PORT_MODE_PORT_ENA |
                VTSS_M_QFWD_SYSTEM_SWITCH_PORT_MODE_FWD_URGENCY);
#endif /* VTSS_ARCH_JAGUAR_2_B */

#if defined(VTSS_FEATURE_AFI_SWC)
        VTSS_RC(jr2_afi_pause_resume(vtss_state, port_no, TRUE));
#endif /* VTSS_FEATURE_AFI_SWC */

        /* Enable flowcontrol - must be done after the port is enabled */
        if (conf->flow_control.generate) {
            JR2_WRM_SET(VTSS_QSYS_PAUSE_CFG_PAUSE_CFG(port), VTSS_M_QSYS_PAUSE_CFG_PAUSE_CFG_PAUSE_ENA);
        }
    } else {
        /* Disable the power hungry serdes */
        VTSS_RC(jr2_serdes_cfg(vtss_state, port_no, VTSS_SERDES_MODE_DISABLE));
    }

    VTSS_D("chip port: %u (10G),is configured", port);

    return VTSS_RC_OK;
}


static vtss_rc jr2_port_conf_set(vtss_state_t *vtss_state, const vtss_port_no_t port_no)
{
    vtss_port_conf_t *conf = &vtss_state->port.conf[port_no];
    BOOL             port_10g;
    u32              port = VTSS_CHIP_PORT(port_no);
    
    if (!vrfy_spd_iface(port_no, conf->if_type, conf->speed, &port_10g)) {
        return VTSS_RC_ERROR;
    }    
    /* Chip ports 49-52 can be either DEV10G or DEV2G5 */
    /* Only one of them can be attached to the switch core at a time. */
    /* A shut down must be performed on the DEV that is not active: */
    if (VTSS_PORT_IS_10G(port)) {
        u32  bt_fld = (port == 49) ? 12 : (port == 50) ? 14 : (port == 51) ? 16 : 18;
        u32 value, mode;
        JR2_RD(VTSS_HSIO_HW_CFGSTAT_HW_CFG, &value);
        mode = VTSS_EXTRACT_BITFIELD(value, bt_fld, 2);
        if ((mode == 3) && port_10g) {
            VTSS_RC(jr2_port_flush(vtss_state, port_no, FALSE)); // Shutdown the 2G5 device
        } else if ((mode != 3) && !port_10g) {
            VTSS_RC(jr2_port_flush(vtss_state, port_no, TRUE));  // Shutdown the 10G device
        }
    }

    VTSS_D("port_no:%d (port:%d, dev%s) if:%s, spd:%d/%s, shutdown:%d\n",
           port_no, port, VTSS_PORT_IS_10G(port) ? "10G":"1G", vtss_port_if_txt(conf->if_type),
           conf->speed, conf->fdx ? "FDX" : "HDX", conf->power_down);

    if (port_10g) {
        return jr2_port_conf_10g_set(vtss_state, port_no);
    } else {
        return jr2_port_conf_1g_set(vtss_state, port_no);
    }
}

//----------------------------------------------------------------------------------------------------


static vtss_rc jr2_port_clause_37_control_get(vtss_state_t *vtss_state,
                                               const vtss_port_no_t port_no,
                                               vtss_port_clause_37_control_t *const control)
{
    u32 value, port = VTSS_CHIP_PORT(port_no);

    JR2_RD(VTSS_DEV1G_PCS1G_CFG_STATUS_PCS1G_ANEG_CFG(VTSS_TO_DEV(port)), &value);
    control->enable = VTSS_BOOL(VTSS_X_DEV1G_PCS1G_CFG_STATUS_PCS1G_ANEG_CFG_ANEG_ENA(value));
    value = VTSS_X_DEV1G_PCS1G_CFG_STATUS_PCS1G_ANEG_CFG_ADV_ABILITY(control->enable ? value : 0);
    VTSS_RC(vtss_cmn_port_clause_37_adv_get(value, &control->advertisement));

    return VTSS_RC_OK;
}

static vtss_rc jr2_port_clause_37_control_set(vtss_state_t *vtss_state,
                                               const vtss_port_no_t port_no)
{
    vtss_port_clause_37_control_t *control = &vtss_state->port.clause_37[port_no];
    u32 value, tgt;

    VTSS_RC(jr2_port_inst_get(vtss_state, port_no, &tgt, NULL, NULL));

    /* Aneg capabilities for this port */
    VTSS_RC(vtss_cmn_port_clause_37_adv_set(&value, &control->advertisement, control->enable));

    /* Set aneg capabilities, enable neg and restart */
    JR2_WR(VTSS_DEV1G_PCS1G_CFG_STATUS_PCS1G_ANEG_CFG(tgt),
           VTSS_F_DEV1G_PCS1G_CFG_STATUS_PCS1G_ANEG_CFG_ADV_ABILITY(value) |
           VTSS_F_DEV1G_PCS1G_CFG_STATUS_PCS1G_ANEG_CFG_ANEG_ENA(1) |
           VTSS_F_DEV1G_PCS1G_CFG_STATUS_PCS1G_ANEG_CFG_ANEG_RESTART_ONE_SHOT(1));

    if (!control->enable) {
        /* Disable Aneg */
        JR2_WR(VTSS_DEV1G_PCS1G_CFG_STATUS_PCS1G_ANEG_CFG(tgt),
               VTSS_F_DEV1G_PCS1G_CFG_STATUS_PCS1G_ANEG_CFG_ADV_ABILITY(0) |
               VTSS_F_DEV1G_PCS1G_CFG_STATUS_PCS1G_ANEG_CFG_ANEG_ENA(0) |
               VTSS_F_DEV1G_PCS1G_CFG_STATUS_PCS1G_ANEG_CFG_ANEG_RESTART_ONE_SHOT(1));
    }
 
    return VTSS_RC_OK;
}


static vtss_rc jr2_port_clause_37_status_get(vtss_state_t *vtss_state,
                                              const vtss_port_no_t         port_no, 
                                              vtss_port_clause_37_status_t *const status)
    
{
    u32                    value;
    vtss_port_sgmii_aneg_t *sgmii_adv = &status->autoneg.partner_adv_sgmii;
    u32                    tgt;

    // Find device target and serdes macro for this chip port
    VTSS_RC(jr2_port_inst_get(vtss_state, port_no, &tgt, NULL, NULL));
    
    if (vtss_state->port.conf[port_no].power_down) {
        status->link = 0;
        return VTSS_RC_OK;
    }

    /* Get the link state 'down' sticky bit  */
    JR2_RD(VTSS_DEV1G_PCS1G_CFG_STATUS_PCS1G_STICKY(tgt), &value);
    status->link = (JR2_BF(DEV1G_PCS1G_CFG_STATUS_PCS1G_STICKY_LINK_DOWN_STICKY, value) ? 0 : 1);
    if (status->link == 0) {
        /* The link has been down. Clear the sticky bit and return the 'down' value  */
        JR2_WR(VTSS_DEV1G_PCS1G_CFG_STATUS_PCS1G_STICKY(tgt), 
                VTSS_M_DEV1G_PCS1G_CFG_STATUS_PCS1G_STICKY_LINK_DOWN_STICKY);        
    } else {
        JR2_RD(VTSS_DEV1G_PCS1G_CFG_STATUS_PCS1G_LINK_STATUS(tgt), &value);
        status->link = JR2_BF(DEV1G_PCS1G_CFG_STATUS_PCS1G_LINK_STATUS_LINK_STATUS, value) &&
                       JR2_BF(DEV1G_PCS1G_CFG_STATUS_PCS1G_LINK_STATUS_SYNC_STATUS, value);
    }

    /* Get PCS ANEG status register */
    JR2_RD(VTSS_DEV1G_PCS1G_CFG_STATUS_PCS1G_ANEG_STATUS(tgt), &value);

    /* Get 'Aneg complete'   */
    status->autoneg.complete = JR2_BF(DEV1G_PCS1G_CFG_STATUS_PCS1G_ANEG_STATUS_ANEG_COMPLETE, value);

   /* Workaround for a Serdes issue, when aneg completes with FDX capability=0 */
    if (vtss_state->port.conf[port_no].if_type == VTSS_PORT_INTERFACE_SERDES) {
        if (status->autoneg.complete) {
            if (((value >> 21) & 0x1) == 0) { 
                JR2_WRM_CLR(VTSS_DEV1G_PCS1G_CFG_STATUS_PCS1G_CFG(tgt), VTSS_M_DEV1G_PCS1G_CFG_STATUS_PCS1G_CFG_PCS_ENA);
                JR2_WRM_SET(VTSS_DEV1G_PCS1G_CFG_STATUS_PCS1G_CFG(tgt), VTSS_M_DEV1G_PCS1G_CFG_STATUS_PCS1G_CFG_PCS_ENA);
                (void)jr2_port_clause_37_control_set(vtss_state, port_no); /* Restart Aneg */
                VTSS_MSLEEP(50);
                JR2_RD(VTSS_DEV1G_PCS1G_CFG_STATUS_PCS1G_ANEG_STATUS(tgt), &value);
                status->autoneg.complete = JR2_BF(DEV1G_PCS1G_CFG_STATUS_PCS1G_ANEG_STATUS_ANEG_COMPLETE, value);
            }
        }
    }

    /* Return partner advertisement ability */
    value = VTSS_X_DEV1G_PCS1G_CFG_STATUS_PCS1G_ANEG_STATUS_LP_ADV_ABILITY(value);

    if (vtss_state->port.conf[port_no].if_type == VTSS_PORT_INTERFACE_SGMII_CISCO) {
        sgmii_adv->link = ((value >> 15) == 1) ? 1 : 0;
        sgmii_adv->fdx = (((value >> 12) & 0x1) == 1) ? 1 : 0;
        value = ((value >> 10) & 3);
        sgmii_adv->speed_10M = (value == 0 ? 1 : 0);
        sgmii_adv->speed_100M = (value == 1 ? 1 : 0);
        sgmii_adv->speed_1G = (value == 2 ? 1 : 0);
        sgmii_adv->hdx = (status->autoneg.partner_advertisement.fdx ? 0 : 1);
        if (status->link) {
            /* If the SFP module does not have a link then the port does not have link */
            status->link = sgmii_adv->link;
        }
    } else {
        VTSS_RC(vtss_cmn_port_clause_37_adv_get(value, &status->autoneg.partner_advertisement));
    }

    return VTSS_RC_OK;
}




/* Get status of the XAUI, VAUI or 100FX ports. */
/* Status for SERDES and SGMII ports is handled elsewhere (through autonegotiation) */
static vtss_rc jr2_port_status_get(vtss_state_t *vtss_state,
                                  const vtss_port_no_t  port_no,
                                  vtss_port_status_t    *const status)
{
    u32              value, sd;
    u32              tgt;
    vtss_port_conf_t *conf = &vtss_state->port.conf[port_no];

    VTSS_RC(jr2_port_inst_get(vtss_state, port_no, &tgt, NULL, NULL));
    
    if (conf->power_down) {
        /* Disabled port is considered down */
        return VTSS_RC_OK;
    }

    switch (vtss_state->port.conf[port_no].if_type) {
    case VTSS_PORT_INTERFACE_100FX:
        /* Get the PCS status  */
        JR2_RD(VTSS_DEV1G_PCS_FX100_STATUS_PCS_FX100_STATUS(tgt), &value);
        
        /* Link has been down if the are any error stickies */
        status->link_down = VTSS_X_DEV1G_PCS_FX100_STATUS_PCS_FX100_STATUS_SYNC_LOST_STICKY(value) ||
                            VTSS_X_DEV1G_PCS_FX100_STATUS_PCS_FX100_STATUS_SSD_ERROR_STICKY(value) ||
                            VTSS_X_DEV1G_PCS_FX100_STATUS_PCS_FX100_STATUS_FEF_FOUND_STICKY(value) ||
                            VTSS_X_DEV1G_PCS_FX100_STATUS_PCS_FX100_STATUS_PCS_ERROR_STICKY(value) ||
                            VTSS_X_DEV1G_PCS_FX100_STATUS_PCS_FX100_STATUS_FEF_STATUS(value);

        if (status->link_down) {
            /* Clear the stickies and re-read */
            JR2_WR(VTSS_DEV1G_PCS_FX100_STATUS_PCS_FX100_STATUS(tgt), value);
            VTSS_MSLEEP(1);
            JR2_RD(VTSS_DEV1G_PCS_FX100_STATUS_PCS_FX100_STATUS(tgt), &value);
        }
        /* Link=1 if sync status=1 and no error stickies after a clear */
        status->link = VTSS_X_DEV1G_PCS_FX100_STATUS_PCS_FX100_STATUS_SYNC_STATUS(value) &&
                     (!VTSS_X_DEV1G_PCS_FX100_STATUS_PCS_FX100_STATUS_SYNC_LOST_STICKY(value) &&
                      !VTSS_X_DEV1G_PCS_FX100_STATUS_PCS_FX100_STATUS_SSD_ERROR_STICKY(value) &&
                      !VTSS_X_DEV1G_PCS_FX100_STATUS_PCS_FX100_STATUS_FEF_FOUND_STICKY(value) &&
                      !VTSS_X_DEV1G_PCS_FX100_STATUS_PCS_FX100_STATUS_PCS_ERROR_STICKY(value) &&
                      !VTSS_X_DEV1G_PCS_FX100_STATUS_PCS_FX100_STATUS_FEF_STATUS(value));
        status->speed = VTSS_SPEED_100M; 
        break;
    case VTSS_PORT_INTERFACE_VAUI:
        /* Get the PCS status */
        JR2_RD(VTSS_DEV1G_PCS1G_CFG_STATUS_PCS1G_LINK_STATUS(tgt), &value);
        status->link = JR2_BF(DEV1G_PCS1G_CFG_STATUS_PCS1G_LINK_STATUS_LINK_STATUS, value) &&
                       JR2_BF(DEV1G_PCS1G_CFG_STATUS_PCS1G_LINK_STATUS_SYNC_STATUS, value);
        JR2_RD(VTSS_DEV1G_PCS1G_CFG_STATUS_PCS1G_STICKY(tgt), &value);
        status->link_down = JR2_BF(DEV1G_PCS1G_CFG_STATUS_PCS1G_STICKY_LINK_DOWN_STICKY, value);
        if (status->link_down) {
            /* The link has been down. Clear the sticky bit */
            JR2_WRM_SET(VTSS_DEV1G_PCS1G_CFG_STATUS_PCS1G_STICKY(tgt),
                        VTSS_M_DEV1G_PCS1G_CFG_STATUS_PCS1G_STICKY_LINK_DOWN_STICKY);
        }
        status->speed = VTSS_SPEED_2500M; 
        break;
    case VTSS_PORT_INTERFACE_XAUI:
    case VTSS_PORT_INTERFACE_RXAUI:
    case VTSS_PORT_INTERFACE_SFI:
        /* MAC10G Tx Monitor Sticky bit Register */
        JR2_RD(VTSS_DEV10G_MAC_CFG_STATUS_MAC_TX_MONITOR_STICKY(tgt), &value);
        /* Signal detect from the 10G Macro */
        JR2_RD(VTSS_SD10G65_SD10G65_IB_SD10G65_IB_CFG10(VTSS_TO_10G_SRD_TGT(VTSS_CHIP_PORT(port_no))), &sd);
        if (value != 2) {
            /* The link is or has been down. Clear the sticky bit */
            status->link_down = 1;
            JR2_WR(VTSS_DEV10G_MAC_CFG_STATUS_MAC_TX_MONITOR_STICKY(tgt), 0xFFFFFFFF);
            JR2_RD(VTSS_DEV10G_MAC_CFG_STATUS_MAC_TX_MONITOR_STICKY(tgt), &value);
        }
        if (vtss_state->port.conf[port_no].if_type == VTSS_PORT_INTERFACE_SFI) {
            /* IDLE_STATE_STICKY = 1 and no others stickys --> link on */
            status->link = (value == 2 ? 1 : 0) && VTSS_X_SD10G65_SD10G65_IB_SD10G65_IB_CFG10_IB_IA_SDET(sd);
        } else {
            status->link = (value == 2 ? 1 : 0);
        }        
        status->speed = conf->speed;
        break;
    case VTSS_PORT_INTERFACE_NO_CONNECTION:
        status->link = 0;
        status->link_down = 0;
        break;
    default:
        VTSS_E("Status not supported for port: %u", port_no);
        return VTSS_RC_ERROR;
    }
    status->fdx = conf->fdx;
    return VTSS_RC_OK;
}

#define JR2_CNT_1G(name, i, cnt, clr)                         \
{                                                            \
    u32 value;                                               \
    JR2_RD(VTSS_ASM_DEV_STATISTICS_##name##_CNT(i), &value); \
    vtss_cmn_counter_32_update(value, cnt, clr);             \
}

#define JR2_CNT_10G(name, i, cnt, clr)                           \
{                                                               \
    u32 value;                                                  \
    JR2_RD(VTSS_DEV10G_DEV_STATISTICS_##name##_CNT(i), &value); \
    vtss_cmn_counter_32_update(value, cnt, clr);                \
}

#define JR2_CNT_ANA_AC(name, cnt, clr)               \
{                                                    \
    u32 value;                                       \
    JR2_RD(VTSS_ANA_AC_STAT_CNT_CFG_##name, &value); \
    vtss_cmn_counter_32_update(value, cnt, clr);     \
}

static vtss_rc vtss_jr2_qsys_counter_update(vtss_state_t *vtss_state,
                                            u32 *addr, vtss_chip_counter_t *counter, BOOL clear)
{
    u32 value;

#if defined(VTSS_ARCH_JAGUAR_2_C)
    JR2_RD(VTSS_XQS_STAT_CNT(*addr), &value);
#else
    JR2_RD(VTSS_QSYS_STAT_CNT(*addr), &value);
#endif /* VTSS_ARCH_JAGUAR_2_C */

    *addr = (*addr + 1); /* Next counter address */
    vtss_cmn_counter_32_update(value, counter, clear);

    return VTSS_RC_OK;
}

/* Index of ANA_AC port counters */
#define JR2_CNT_ANA_AC_PORT_FILTER        0
#define JR2_CNT_ANA_AC_PORT_POLICER_DROPS 1

/* Index of ANA_AC queue counters */
#define JR2_CNT_ANA_AC_QUEUE_PRIO 0

static vtss_rc jr2_port_counters_chip(vtss_state_t                 *vtss_state,
                                      vtss_port_no_t               port_no,
                                      vtss_port_jr2_counters_t     *c,
                                      vtss_port_counters_t *const  counters, 
                                      BOOL                         clr)
{
    u32                                i, addr, port;
    vtss_port_counter_t                rx_errors;
    vtss_port_rmon_counters_t          *rmon;
    vtss_port_if_group_counters_t      *if_group;
    vtss_port_ethernet_like_counters_t *elike;
    vtss_port_proprietary_counters_t   *prop;

    if (port_no >= vtss_state->port_count) {
        /* CPU/virtual port */
        port = (VTSS_CHIP_PORT_CPU + port_no - vtss_state->port_count);
    } else if (!port_is_in_10g_mode(vtss_state, port_no)) {
        /* ASM counters */
        port = VTSS_CHIP_PORT(port_no);
        i = port;
        JR2_CNT_1G(RX_IN_BYTES, i, &c->rx_in_bytes, clr);
        JR2_CNT_1G(RX_SYMBOL_ERR, i, &c->rx_symbol_err, clr);
        JR2_CNT_1G(RX_PAUSE, i, &c->rx_pause, clr);
        JR2_CNT_1G(RX_UNSUP_OPCODE, i, &c->rx_unsup_opcode, clr);
        JR2_CNT_1G(RX_OK_BYTES, i, &c->rx_ok_bytes, clr);
        JR2_CNT_1G(RX_BAD_BYTES, i, &c->rx_bad_bytes, clr);
        JR2_CNT_1G(RX_UC, i, &c->rx_unicast, clr);
        JR2_CNT_1G(RX_MC, i, &c->rx_multicast, clr);
        JR2_CNT_1G(RX_BC, i, &c->rx_broadcast, clr);
        JR2_CNT_1G(RX_CRC_ERR, i, &c->rx_crc_err, clr);
        JR2_CNT_1G(RX_UNDERSIZE, i, &c->rx_undersize, clr);
        JR2_CNT_1G(RX_FRAGMENTS, i, &c->rx_fragments, clr);
        JR2_CNT_1G(RX_IN_RANGE_LEN_ERR, i, &c->rx_in_range_len_err, clr);
        JR2_CNT_1G(RX_OUT_OF_RANGE_LEN_ERR, i, &c->rx_out_of_range_len_err, clr);
        JR2_CNT_1G(RX_OVERSIZE, i, &c->rx_oversize, clr);
        JR2_CNT_1G(RX_JABBERS, i, &c->rx_jabbers, clr);
        JR2_CNT_1G(RX_SIZE64, i, &c->rx_size64, clr);
        JR2_CNT_1G(RX_SIZE65TO127, i, &c->rx_size65_127, clr);
        JR2_CNT_1G(RX_SIZE128TO255, i, &c->rx_size128_255, clr);
        JR2_CNT_1G(RX_SIZE256TO511, i, &c->rx_size256_511, clr);
        JR2_CNT_1G(RX_SIZE512TO1023, i, &c->rx_size512_1023, clr);
        JR2_CNT_1G(RX_SIZE1024TO1518, i, &c->rx_size1024_1518, clr);
        JR2_CNT_1G(RX_SIZE1519TOMAX, i, &c->rx_size1519_max, clr);

        JR2_CNT_1G(TX_OUT_BYTES, i, &c->tx_out_bytes, clr);
        JR2_CNT_1G(TX_PAUSE, i, &c->tx_pause, clr);
        JR2_CNT_1G(TX_OK_BYTES, i, &c->tx_ok_bytes, clr);
        JR2_CNT_1G(TX_UC, i, &c->tx_unicast, clr);
        JR2_CNT_1G(TX_MC, i, &c->tx_multicast, clr);
        JR2_CNT_1G(TX_BC, i, &c->tx_broadcast, clr);
        JR2_CNT_1G(TX_SIZE64, i, &c->tx_size64, clr);
        JR2_CNT_1G(TX_SIZE65TO127, i, &c->tx_size65_127, clr);
        JR2_CNT_1G(TX_SIZE128TO255, i, &c->tx_size128_255, clr);
        JR2_CNT_1G(TX_SIZE256TO511, i, &c->tx_size256_511, clr);
        JR2_CNT_1G(TX_SIZE512TO1023, i, &c->tx_size512_1023, clr);
        JR2_CNT_1G(TX_SIZE1024TO1518, i, &c->tx_size1024_1518, clr);
        JR2_CNT_1G(TX_SIZE1519TOMAX, i, &c->tx_size1519_max, clr);
        JR2_CNT_1G(TX_MULTI_COLL, i, &c->tx_multi_coll, clr);
        JR2_CNT_1G(TX_LATE_COLL, i, &c->tx_late_coll, clr);
        JR2_CNT_1G(TX_XCOLL, i, &c->tx_xcoll, clr);
        JR2_CNT_1G(TX_DEFER, i, &c->tx_defer, clr);
        JR2_CNT_1G(TX_XDEFER, i, &c->tx_xdefer, clr);
        JR2_CNT_1G(TX_BACKOFF1, i, &c->tx_backoff1, clr);
    } else {
        /* DEV10G counters */
        port = VTSS_CHIP_PORT(port_no);
        VTSS_RC(jr2_port_inst_get(vtss_state, port_no, &i, NULL, NULL));
        JR2_CNT_10G(40BIT_RX_IN_BYTES, i, &c->rx_in_bytes, clr);
        JR2_CNT_10G(32BIT_RX_SYMBOL_ERR, i, &c->rx_symbol_err, clr);
        JR2_CNT_10G(32BIT_RX_PAUSE, i, &c->rx_pause, clr);
        JR2_CNT_10G(32BIT_RX_UNSUP_OPCODE, i, &c->rx_unsup_opcode, clr);
        JR2_CNT_10G(40BIT_RX_OK_BYTES, i, &c->rx_ok_bytes, clr);
        JR2_CNT_10G(40BIT_RX_BAD_BYTES, i, &c->rx_bad_bytes, clr);
        JR2_CNT_10G(32BIT_RX_UC, i, &c->rx_unicast, clr);
        JR2_CNT_10G(32BIT_RX_MC, i, &c->rx_multicast, clr);
        JR2_CNT_10G(32BIT_RX_BC, i, &c->rx_broadcast, clr);
        JR2_CNT_10G(32BIT_RX_CRC_ERR, i, &c->rx_crc_err, clr);
        JR2_CNT_10G(32BIT_RX_UNDERSIZE, i, &c->rx_undersize, clr);
        JR2_CNT_10G(32BIT_RX_FRAGMENTS, i, &c->rx_fragments, clr);
        JR2_CNT_10G(32BIT_RX_IN_RANGE_LEN_ERR, i, &c->rx_in_range_len_err, clr);
        JR2_CNT_10G(32BIT_RX_OUT_OF_RANGE_LEN_ERR, i, &c->rx_out_of_range_len_err, clr);
        JR2_CNT_10G(32BIT_RX_OVERSIZE, i, &c->rx_oversize, clr);
        JR2_CNT_10G(32BIT_RX_JABBERS, i, &c->rx_jabbers, clr);
        JR2_CNT_10G(32BIT_RX_SIZE64, i, &c->rx_size64, clr);
        JR2_CNT_10G(32BIT_RX_SIZE65TO127, i, &c->rx_size65_127, clr);
        JR2_CNT_10G(32BIT_RX_SIZE128TO255, i, &c->rx_size128_255, clr);
        JR2_CNT_10G(32BIT_RX_SIZE256TO511, i, &c->rx_size256_511, clr);
        JR2_CNT_10G(32BIT_RX_SIZE512TO1023, i, &c->rx_size512_1023, clr);
        JR2_CNT_10G(32BIT_RX_SIZE1024TO1518, i, &c->rx_size1024_1518, clr);
        JR2_CNT_10G(32BIT_RX_SIZE1519TOMAX, i, &c->rx_size1519_max, clr);

        JR2_CNT_10G(40BIT_TX_OUT_BYTES, i, &c->tx_out_bytes, clr);
        JR2_CNT_10G(32BIT_TX_PAUSE, i, &c->tx_pause, clr);
        JR2_CNT_10G(40BIT_TX_OK_BYTES, i, &c->tx_ok_bytes, clr);
        JR2_CNT_10G(32BIT_TX_UC, i, &c->tx_unicast, clr);
        JR2_CNT_10G(32BIT_TX_MC, i, &c->tx_multicast, clr);
        JR2_CNT_10G(32BIT_TX_BC, i, &c->tx_broadcast, clr);
        JR2_CNT_10G(32BIT_TX_SIZE64, i, &c->tx_size64, clr);
        JR2_CNT_10G(32BIT_TX_SIZE65TO127, i, &c->tx_size65_127, clr);
        JR2_CNT_10G(32BIT_TX_SIZE128TO255, i, &c->tx_size128_255, clr);
        JR2_CNT_10G(32BIT_TX_SIZE256TO511, i, &c->tx_size256_511, clr);
        JR2_CNT_10G(32BIT_TX_SIZE512TO1023, i, &c->tx_size512_1023, clr);
        JR2_CNT_10G(32BIT_TX_SIZE1024TO1518, i, &c->tx_size1024_1518, clr);
        JR2_CNT_10G(32BIT_TX_SIZE1519TOMAX, i, &c->tx_size1519_max, clr);
    }

    /* QSYS counters */
#if defined(VTSS_ARCH_JAGUAR_2_C)
    JR2_WR(VTSS_XQS_SYSTEM_STAT_CFG, VTSS_F_XQS_SYSTEM_STAT_CFG_STAT_VIEW(port));
#else
    JR2_WR(VTSS_QSYS_SYSTEM_STAT_CFG, VTSS_F_QSYS_SYSTEM_STAT_CFG_STAT_VIEW(port));
#endif /* VTSS_ARCH_JAGUAR_2_C */
    addr = 0;
    for (i = 0; i < VTSS_PRIOS; i++) {
        VTSS_RC(vtss_jr2_qsys_counter_update(vtss_state, &addr, &c->rx_green_drops[i], clr));
    }
    for (i = 0; i < VTSS_PRIOS; i++) {
        VTSS_RC(vtss_jr2_qsys_counter_update(vtss_state, &addr, &c->rx_yellow_drops[i], clr));
    }
    addr = 256;
    for (i = 0; i < VTSS_PRIOS; i++) {
        VTSS_RC(vtss_jr2_qsys_counter_update(vtss_state, &addr, &c->tx_green_class[i], clr));
    }
    for (i = 0; i < VTSS_PRIOS; i++) {
        VTSS_RC(vtss_jr2_qsys_counter_update(vtss_state, &addr, &c->tx_yellow_class[i], clr));
    }
    VTSS_RC(vtss_jr2_qsys_counter_update(vtss_state, &addr, &c->tx_queue_drops, clr));

    /* ANA_AC counters */
    JR2_CNT_ANA_AC(PORT_STAT_LSB_CNT(port, JR2_CNT_ANA_AC_PORT_FILTER), &c->rx_local_drops, clr);
    JR2_CNT_ANA_AC(PORT_STAT_LSB_CNT(port, JR2_CNT_ANA_AC_PORT_POLICER_DROPS), &c->rx_policer_drops, clr);
    for (i = 0; i < VTSS_PRIOS; i++) {
        JR2_CNT_ANA_AC(QUEUE_STAT_LSB_CNT(port*8 + i, JR2_CNT_ANA_AC_QUEUE_PRIO), &c->rx_class[i], clr);
    }

    if (counters == NULL) {
        return VTSS_RC_OK;
    }

    /* Proprietary counters */
    prop = &counters->prop;
    for (i = 0; i < VTSS_PRIOS; i++) {
        prop->rx_prio[i] = c->rx_class[i].value;
        prop->tx_prio[i] = (c->tx_yellow_class[i].value + c->tx_green_class[i].value);
    }

    /* RMON Rx counters */
    rmon = &counters->rmon;
    rmon->rx_etherStatsDropEvents = c->rx_policer_drops.value;
    for (i = 0; i < VTSS_PRIOS; i++) {
        rmon->rx_etherStatsDropEvents += (c->rx_green_drops[i].value + c->rx_yellow_drops[i].value);
    }
    rmon->rx_etherStatsOctets = (c->rx_ok_bytes.value + c->rx_bad_bytes.value);
    rx_errors = (c->rx_crc_err.value +  c->rx_undersize.value + c->rx_oversize.value +
                 c->rx_out_of_range_len_err.value + c->rx_symbol_err.value +
                 c->rx_jabbers.value + c->rx_fragments.value);
    rmon->rx_etherStatsPkts = (c->rx_unicast.value + c->rx_multicast.value +
                               c->rx_broadcast.value + rx_errors);
    rmon->rx_etherStatsBroadcastPkts = c->rx_broadcast.value;
    rmon->rx_etherStatsMulticastPkts = c->rx_multicast.value;
    rmon->rx_etherStatsCRCAlignErrors = c->rx_crc_err.value;
    rmon->rx_etherStatsUndersizePkts = c->rx_undersize.value;
    rmon->rx_etherStatsOversizePkts = c->rx_oversize.value;
    rmon->rx_etherStatsFragments = c->rx_fragments.value;
    rmon->rx_etherStatsJabbers = c->rx_jabbers.value;
    rmon->rx_etherStatsPkts64Octets = c->rx_size64.value;
    rmon->rx_etherStatsPkts65to127Octets = c->rx_size65_127.value;
    rmon->rx_etherStatsPkts128to255Octets = c->rx_size128_255.value;
    rmon->rx_etherStatsPkts256to511Octets = c->rx_size256_511.value;
    rmon->rx_etherStatsPkts512to1023Octets = c->rx_size512_1023.value;
    rmon->rx_etherStatsPkts1024to1518Octets = c->rx_size1024_1518.value;
    rmon->rx_etherStatsPkts1519toMaxOctets = c->rx_size1519_max.value;

    /* RMON Tx counters */
    rmon->tx_etherStatsDropEvents = c->tx_queue_drops.value;
    rmon->tx_etherStatsPkts = (c->tx_unicast.value + c->tx_multicast.value +
                               c->tx_broadcast.value + c->tx_late_coll.value);
    rmon->tx_etherStatsOctets = c->tx_ok_bytes.value;
    rmon->tx_etherStatsBroadcastPkts = c->tx_broadcast.value;
    rmon->tx_etherStatsMulticastPkts = c->tx_multicast.value;
    rmon->tx_etherStatsCollisions = (c->tx_multi_coll.value + c->tx_backoff1.value +
                                     c->tx_late_coll.value + c->tx_xcoll.value);
    rmon->tx_etherStatsPkts64Octets = c->tx_size64.value;
    rmon->tx_etherStatsPkts65to127Octets = c->tx_size65_127.value;
    rmon->tx_etherStatsPkts128to255Octets = c->tx_size128_255.value;
    rmon->tx_etherStatsPkts256to511Octets = c->tx_size256_511.value;
    rmon->tx_etherStatsPkts512to1023Octets = c->tx_size512_1023.value;
    rmon->tx_etherStatsPkts1024to1518Octets = c->tx_size1024_1518.value;
    rmon->tx_etherStatsPkts1519toMaxOctets = c->tx_size1519_max.value;

    /* Interfaces Group Rx counters */
    if_group = &counters->if_group;
    if_group->ifInOctets = rmon->rx_etherStatsOctets;
    if_group->ifInUcastPkts = c->rx_unicast.value;
    if_group->ifInMulticastPkts = c->rx_multicast.value;
    if_group->ifInBroadcastPkts = c->rx_broadcast.value;
    if_group->ifInNUcastPkts = (c->rx_multicast.value + c->rx_broadcast.value);
    if_group->ifInDiscards = rmon->rx_etherStatsDropEvents;
    if_group->ifInErrors = rx_errors;

    /* Interfaces Group Tx counters */
    if_group->ifOutOctets = rmon->tx_etherStatsOctets;
    if_group->ifOutUcastPkts = c->tx_unicast.value;
    if_group->ifOutMulticastPkts = c->tx_multicast.value;
    if_group->ifOutBroadcastPkts = c->tx_broadcast.value;
    if_group->ifOutNUcastPkts = (c->tx_multicast.value + c->tx_broadcast.value);
    if_group->ifOutDiscards = rmon->tx_etherStatsDropEvents;
    if_group->ifOutErrors = (c->tx_late_coll.value + c->tx_csense.value + c->tx_xcoll.value);

    /* Ethernet-like Rx counters */
    elike = &counters->ethernet_like;
    elike->dot3StatsAlignmentErrors = 0; /* Not supported */
    elike->dot3StatsFCSErrors = c->rx_crc_err.value;
    elike->dot3StatsFrameTooLongs = c->rx_oversize.value;
    elike->dot3StatsSymbolErrors = c->rx_symbol_err.value;
    elike->dot3ControlInUnknownOpcodes = c->rx_unsup_opcode.value;
    elike->dot3InPauseFrames = c->rx_pause.value;

    /* Ethernet-like Tx counters */
    elike->dot3StatsSingleCollisionFrames = c->tx_backoff1.value;
    elike->dot3StatsMultipleCollisionFrames = c->tx_multi_coll.value;
    elike->dot3StatsDeferredTransmissions = c->tx_defer.value;
    elike->dot3StatsLateCollisions = c->tx_late_coll.value;
    elike->dot3StatsExcessiveCollisions = c->tx_xcoll.value;
    elike->dot3StatsCarrierSenseErrors = c->tx_csense.value;
    elike->dot3OutPauseFrames = c->tx_pause.value;

    /* Bridge counters, including filtered frames with and without CRC error */
    counters->bridge.dot1dTpPortInDiscards = c->rx_local_drops.value;

    return VTSS_RC_OK;
}

static vtss_rc jr2_port_basic_counters_get(vtss_state_t *vtss_state,
                                            const vtss_port_no_t port_no,
                                            vtss_basic_counters_t *const counters)
{
    // JR2-TBD: Stub
    return VTSS_RC_ERROR;
}

static vtss_rc jr2_port_counters(vtss_state_t                *vtss_state,
                                 const vtss_port_no_t        port_no, 
                                 vtss_port_counters_t *const counters, 
                                 BOOL                        clear)
{
    return jr2_port_counters_chip(vtss_state,
                                  port_no,
                                  &vtss_state->port.counters[port_no].counter.jr2,
                                  counters,
                                  clear);
}

static vtss_rc jr2_port_counters_update(vtss_state_t *vtss_state, const vtss_port_no_t port_no)
{
    return jr2_port_counters(vtss_state, port_no, NULL, 0);
}

static vtss_rc jr2_port_counters_clear(vtss_state_t *vtss_state, const vtss_port_no_t port_no)
{
    return jr2_port_counters(vtss_state, port_no, NULL, 1);
}

static vtss_rc jr2_port_counters_get(vtss_state_t *vtss_state,
                                      const vtss_port_no_t port_no,
                                      vtss_port_counters_t *const counters)
{
    memset(counters, 0, sizeof(*counters)); 
    return jr2_port_counters(vtss_state, port_no, counters, 0);
}

static vtss_rc jr2_port_forward_set(vtss_state_t *vtss_state, const vtss_port_no_t port_no)
{
    return VTSS_RC_OK;
}

static vtss_rc jr2_port_ifh_set(vtss_state_t *vtss_state, const vtss_port_no_t port_no)
{
    u32 port = VTSS_CHIP_PORT(port_no);
    vtss_port_ifh_t *ifh = &vtss_state->port.ifh_conf[port_no];
    BOOL extr =  ifh->ena_xtr_header || ifh->ena_ifh_header;

     /* Enable/Disable IFH parsing at ingress DMAC:SMAC:0x8880:0x0007:IFH:Frame */
    JR2_WRM(VTSS_ASM_CFG_PORT_CFG(port), 
            VTSS_F_ASM_CFG_PORT_CFG_INJ_FORMAT_CFG(ifh->ena_inj_header ? 2 : 0) |
            VTSS_F_ASM_CFG_PORT_CFG_SKIP_PREAMBLE_ENA(ifh->ena_inj_header),
            VTSS_M_ASM_CFG_PORT_CFG_SKIP_PREAMBLE_ENA |
            VTSS_M_ASM_CFG_PORT_CFG_INJ_FORMAT_CFG);
    
     /* Enable/Disable IFH prepend at egress DMAC:SMAC:0x8880:0x0007:IFH:Frame */
    JR2_WRM(VTSS_REW_COMMON_PORT_CTRL(port), 
             VTSS_F_REW_COMMON_PORT_CTRL_KEEP_IFH_SEL(extr ? 2 : 0),
             VTSS_M_REW_COMMON_PORT_CTRL_KEEP_IFH_SEL);
 
    return VTSS_RC_OK;
}

vtss_rc vtss_jr2_wm_update(vtss_state_t *vtss_state)
{
    int  group, queue, dpl;
    BOOL enabled[VTSS_PRIOS] = {0};

    for (group = 0; group < VTSS_WRED_GROUP_CNT; group++) {
        for (queue = VTSS_QUEUE_START; queue < VTSS_QUEUE_END; queue++) {
            for (dpl = 0; dpl < VTSS_WRED_DPL_CNT; dpl++) {
                if (vtss_state->qos.conf.red_v3[queue][dpl][group].enable) {
                    enabled[queue] = TRUE;
                }
            }
        }
    }

    /* Update BUF_PRIO_SHR_E sharing watermarks according to AN1121 Section 7 WRED Operation */
    for (queue = VTSS_QUEUE_START; queue < VTSS_QUEUE_END; queue++) {
        if (enabled[queue]) {
            JR2_WR(VTSS_QRES_RES_CTRL_RES_CFG((queue + 496 + 2048)), 0); /* WRED is enabled somewhere for this QoS class - set watermark to zero */
        } else {
            JR2_WR(VTSS_QRES_RES_CTRL_RES_CFG((queue + 496 + 2048)), vtss_state->port.buf_prio_shr_e[queue]); /* Restore initial value */
        }
    }

    return VTSS_RC_OK;
}

#define BUF_OVERSUBSRIPTION_FACTOR 200 // (factor 100 = no oversubsription)
static vtss_rc jr2_port_buf_conf_set(vtss_state_t *vtss_state)
{
    int q;
    u32 port_no, port, dp;
    u32 buf_q_rsrv_i, buf_q_rsrv_e, ref_q_rsrv_i, ref_q_rsrv_e, buf_prio_shr_i[8], buf_prio_shr_e[8], ref_prio_shr_i[8], ref_prio_shr_e[8];
    u32 buf_p_rsrv_i, buf_p_rsrv_e, ref_p_rsrv_i, ref_p_rsrv_e, buf_col_shr_i, buf_col_shr_e, ref_col_shr_i, ref_col_shr_e;
    u32 buf_prio_rsrv, ref_prio_rsrv, guaranteed, q_rsrv_mask, prio_mem;

    /* Mode: Strict priority sharing. Higher priorities have right to use all shared before lower */
    
    /* Reserved space. Buffer values are in BYTES */
    buf_q_rsrv_i  = 4000;                 /* Ingress Queue reserved space (all prios) */
    buf_p_rsrv_i  = 15000;                /* Ingress Port reserved space */
    buf_q_rsrv_e  = 4000;                 /* Egress Queue reserved space (all prios)  */
    buf_p_rsrv_e  = 15000;                /* Egress Port reserved space  */
    buf_col_shr_i = JR2_BUFFER_MEMORY;    /* Coloring - disabled for now */
    buf_col_shr_e = JR2_BUFFER_MEMORY;    /* Coloring - disabled for now */
    buf_prio_rsrv = 6000;                 /* Distance between priority stops in the shared memory */

    /* Reserved frame reference. Values are NUMBER of FRAMES */
    ref_q_rsrv_e  = 4;                    /* Number of frames that can be pending at each egress queue   */
    ref_q_rsrv_i  = 4;                    /* Number of frames that can be pending at each ingress queue  */
    ref_p_rsrv_e  = 10;                   /* Number of frames that can be pending shared between the QoS classes at egress */
    ref_p_rsrv_i  = 10;                   /* Number of frames that can be pending shared between the QoS classes at ingress */
    ref_col_shr_i = JR2_BUFFER_REFERENCE; /* Coloring - disabled for now */
    ref_col_shr_e = JR2_BUFFER_REFERENCE; /* Coloring - disabled for now */
    ref_prio_rsrv = 1;                    /* Difference of frames that can be pending for each class (prio = highest) */


    /* The number of supported queues is given through the state structure                           */
    q_rsrv_mask = 0xff >> (8 - vtss_state->qos.conf.prios); 

    /* The memory is oversubsrcribed by BUF_OVERSUBSRIPTION_FACTOR (factor 100 = no oversubsription) */

    /* **************************************************  */
    /* BELOW, everything is calculated based on the above. */
    /* **************************************************  */

    /* Find the amount of guaranteeed space per port */
    guaranteed = buf_p_rsrv_i+buf_p_rsrv_e;
    for (q = 0; q < VTSS_PRIOS; q++) {
        if (q_rsrv_mask & (1 << q)) 
            guaranteed += (buf_q_rsrv_i+buf_q_rsrv_e);
    }

    if (((vtss_state->port_count + 1) * guaranteed * 100 / BUF_OVERSUBSRIPTION_FACTOR) > JR2_BUFFER_MEMORY) {
        VTSS_E("Memory is oversubscribed");
    }
    /* Find the total amount of shared memory incl. oversubscription */
    prio_mem = JR2_BUFFER_MEMORY - (vtss_state->port_count + 1) * guaranteed * 100 / BUF_OVERSUBSRIPTION_FACTOR;

    /* Find the prio watermarks */
    for (q = vtss_state->qos.conf.prios - 1; q >= 0; q--) {
        buf_prio_shr_i[q] = prio_mem;
        ref_prio_shr_i[q] = ref_prio_rsrv;
        buf_prio_shr_e[q] = prio_mem;
        ref_prio_shr_e[q] = ref_prio_rsrv;

        prio_mem -= buf_prio_rsrv;
    }

    /* Configure reserved space for all QoS classes per port */
    for (port_no = 0; port_no <= vtss_state->port_count; port_no++) {
        if (port_no == vtss_state->port_count) {
            port = VTSS_CHIP_PORT_CPU;
        } else {
            port = VTSS_CHIP_PORT(port_no);
        }
        for (q = 0; q < VTSS_PRIOS; q++) {
            if (q_rsrv_mask&(1 << q)) {
                JR2_WR(VTSS_QRES_RES_CTRL_RES_CFG(port * VTSS_PRIOS + q + 0),   wm_enc(buf_q_rsrv_i / JR2_BUFFER_CELL_SZ));
                JR2_WR(VTSS_QRES_RES_CTRL_RES_CFG(port * VTSS_PRIOS + q + 1024), wm_enc(ref_q_rsrv_i));
                JR2_WR(VTSS_QRES_RES_CTRL_RES_CFG(port * VTSS_PRIOS + q + 2048), wm_enc(buf_q_rsrv_e / JR2_BUFFER_CELL_SZ));
                JR2_WR(VTSS_QRES_RES_CTRL_RES_CFG(port * VTSS_PRIOS + q + 3072), wm_enc(ref_q_rsrv_e));
            }
        }
    }

    /* Configure shared space for all QoS classes */
    for (q = 0; q < VTSS_PRIOS; q++) {
        JR2_WR(VTSS_QRES_RES_CTRL_RES_CFG((q + 496 + 0)),   wm_enc(buf_prio_shr_i[q] / JR2_BUFFER_CELL_SZ));
//        JR2_WR(VTSS_QRES_RES_CTRL_RES_CFG((q + 496 + 1024)), wm_enc(ref_prio_shr_i[q])); // Leave at default
        /* Save initial encoded value of shared area for later use (WRED) */
        vtss_state->port.buf_prio_shr_e[q] = wm_enc(buf_prio_shr_e[q] / JR2_BUFFER_CELL_SZ);
        JR2_WR(VTSS_QRES_RES_CTRL_RES_CFG((q + 496 + 2048)), wm_enc(buf_prio_shr_e[q] / JR2_BUFFER_CELL_SZ));
//        JR2_WR(VTSS_QRES_RES_CTRL_RES_CFG((q + 496 + 3072)), wm_enc(ref_prio_shr_e[q])); // Leave at default
    }

    /* Configure reserved space for all ports */
    for (port_no = 0; port_no <= vtss_state->port_count; port_no++) {
        if (port_no == vtss_state->port_count) {
            port = VTSS_CHIP_PORT_CPU;
        } else {
            port = VTSS_CHIP_PORT(port_no);
        }
        JR2_WR(VTSS_QRES_RES_CTRL_RES_CFG(port + 512 +   0), wm_enc(buf_p_rsrv_i / JR2_BUFFER_CELL_SZ));
        JR2_WR(VTSS_QRES_RES_CTRL_RES_CFG(port + 512 + 1024), wm_enc(ref_p_rsrv_i));
        JR2_WR(VTSS_QRES_RES_CTRL_RES_CFG(port + 512 + 2048), wm_enc(buf_p_rsrv_e / JR2_BUFFER_CELL_SZ));
        JR2_WR(VTSS_QRES_RES_CTRL_RES_CFG(port + 512 + 3072), wm_enc(ref_p_rsrv_e));
    }

    /* Configure shared space for  both DP levels (green:0 yellow:1) */
    for (dp = 0; dp < 4; dp++) {
        JR2_WR(VTSS_QRES_RES_CTRL_RES_CFG(dp + 508 +   0), wm_enc(buf_col_shr_i / JR2_BUFFER_CELL_SZ));
        JR2_WR(VTSS_QRES_RES_CTRL_RES_CFG(dp + 508 + 1024), ref_col_shr_i);
        JR2_WR(VTSS_QRES_RES_CTRL_RES_CFG(dp + 508 + 2048), wm_enc(buf_col_shr_e / JR2_BUFFER_CELL_SZ));
        JR2_WR(VTSS_QRES_RES_CTRL_RES_CFG(dp + 508 + 3072), ref_col_shr_e);
    }

    return VTSS_RC_OK;
}

/* - Debug print --------------------------------------------------- */

#define JR_DEBUG_MAC(pr, addr, i, name) vtss_jr2_debug_reg_inst(vtss_state, pr, VTSS_DEV1G_MAC_CFG_STATUS_MAC_##addr, i, "MAC_"name)
#define JR_DEBUG_PCS(pr, addr, i, name) vtss_jr2_debug_reg_inst(vtss_state, pr, VTSS_DEV1G_PCS1G_CFG_STATUS_PCS1G_##addr, i, "PCS_"name)
#define JR_DEBUG_PCS_XAUI(pr, addr, i, name) vtss_jr2_debug_reg_inst(vtss_state, pr, VTSS_DEV10G_PCS_XAUI_##addr, i, "PCS_"name)
#define JR_DEBUG_10G_MAC(pr, addr, i, name) vtss_jr2_debug_reg_inst(vtss_state, pr, VTSS_DEV10G_MAC_CFG_STATUS_MAC_##addr, i, "MAC_"name)
#define JR_DEBUG_FX100(pr, addr, i, name) vtss_jr2_debug_reg_inst(vtss_state, pr, VTSS_DEV1G_PCS_FX100_##addr, i, name)
#define JR_DEBUG_HSIO(pr, addr, name) JR2_DEBUG_REG_NAME(pr, HSIO, addr, name)

static vtss_rc jr2_debug_chip_port(vtss_state_t *vtss_state,
                                   const vtss_debug_printf_t pr,
                                   const vtss_debug_info_t   *const info,
                                   vtss_port_no_t port_no)
{
    u32  tgt, i, inst, type, port = VTSS_CHIP_PORT(port_no);
    char buf[32];

    VTSS_RC(jr2_port_inst_get(vtss_state, port_no, &tgt, &inst, &type));
    if (VTSS_PORT_IS_1G(port)) {
        vtss_jr2_debug_reg_inst(vtss_state, pr, VTSS_DEV1G_DEV_CFG_STATUS_DEV_RST_CTRL(tgt), port, "DEV_RST_CTRL");
        JR_DEBUG_MAC(pr, ENA_CFG(tgt), port, "ENA_CFG");
        JR_DEBUG_MAC(pr, MODE_CFG(tgt), port, "MODE_CFG");
        JR_DEBUG_MAC(pr, MAXLEN_CFG(tgt), port, "MAXLEN_CFG");
        JR_DEBUG_MAC(pr, TAGS_CFG(tgt), port, "TAGS_CFG");
        JR_DEBUG_PCS(pr, CFG(tgt), port, "CFG");
        JR_DEBUG_PCS(pr, MODE_CFG(tgt), port, "MODE_CFG");
        JR_DEBUG_PCS(pr, SD_CFG(tgt), port, "SD_CFG");
        JR_DEBUG_PCS(pr, ANEG_CFG(tgt), port, "ANEG_CFG");
        JR_DEBUG_PCS(pr, ANEG_STATUS(tgt), port, "ANEG_STATUS");
        JR_DEBUG_PCS(pr, LINK_STATUS(tgt), port, "LINK_STATUS");
        JR_DEBUG_FX100(pr, CONFIGURATION_PCS_FX100_CFG(tgt), port, "PCS_FX100_CFG");
        JR_DEBUG_FX100(pr, STATUS_PCS_FX100_STATUS(tgt), port, "FX100_STATUS");
    } else {
        if (port_is_in_10g_mode(vtss_state, port_no)) {
            JR_DEBUG_PCS_XAUI(pr, CONFIGURATION_PCS_XAUI_EXT_CFG(tgt), port, "EXT_CFG");
            JR_DEBUG_PCS_XAUI(pr, CONFIGURATION_PCS_XAUI_CFG(tgt), port, "CFG");
            JR_DEBUG_PCS_XAUI(pr, CONFIGURATION_PCS_XAUI_SD_CFG(tgt), port, "SD_CFG");
            JR_DEBUG_PCS_XAUI(pr, STATUS_PCS_XAUI_RX_STATUS(tgt), port,  "XAUI_RX_STATUS");
            JR_DEBUG_PCS_XAUI(pr, STATUS_PCS_XAUI_RX_ERROR_STATUS(tgt), port, "XAUI_RX_ERROR_STATUS");
            JR_DEBUG_10G_MAC(pr, TX_MONITOR_STICKY(tgt), port, "TX_MONITOR_STICKY");
            JR_DEBUG_10G_MAC(pr, ENA_CFG(tgt), port, "ENA_CFG");
            JR_DEBUG_10G_MAC(pr, MODE_CFG(tgt), port, "MODE_CFG");
        } else {
            vtss_jr2_debug_reg_inst(vtss_state, pr, VTSS_DEV1G_DEV_CFG_STATUS_DEV_RST_CTRL(tgt), port, "DEV_RST_CTRL");
            JR_DEBUG_MAC(pr, ENA_CFG(tgt), port, "ENA_CFG");
            JR_DEBUG_MAC(pr, MODE_CFG(tgt), port, "MODE_CFG");
            JR_DEBUG_MAC(pr, MAXLEN_CFG(tgt), port, "MAXLEN_CFG");
            JR_DEBUG_MAC(pr, TAGS_CFG(tgt), port, "TAGS_CFG");
            JR_DEBUG_PCS(pr, CFG(tgt), port, "CFG");
            JR_DEBUG_PCS(pr, MODE_CFG(tgt), port, "MODE_CFG");
            JR_DEBUG_PCS(pr, SD_CFG(tgt), port, "SD_CFG");
            JR_DEBUG_PCS(pr, ANEG_CFG(tgt), port, "ANEG_CFG");
            JR_DEBUG_PCS(pr, ANEG_STATUS(tgt), port, "ANEG_STATUS");
            JR_DEBUG_PCS(pr, LINK_STATUS(tgt), port, "LINK_STATUS");
            JR_DEBUG_FX100(pr, CONFIGURATION_PCS_FX100_CFG(tgt), port, "PCS_FX100_CFG");
            JR_DEBUG_FX100(pr, STATUS_PCS_FX100_STATUS(tgt), port, "FX100_STATUS");
        }
    }
    JR2_DEBUG_REGX_NAME(pr, DSM, CFG_RX_PAUSE_CFG, port, "RX_PAUSE_CFG");
    JR2_DEBUG_REGX_NAME(pr, DSM, CFG_ETH_FC_CFG, port, "ETH_FC_CFG");
    pr("\n");

    if (VTSS_PORT_IS_10G(port)) {
        // APC status readouts:
        u32 value, p, sd, pcs;        
        pr("APC status:\n");
        pr("port    gain    g.adj   ldlev   offs    agc     c       l       dfe1    dfe2    dfe3    dfe4\n");
        for (p = VTSS_PORT_10G_START; p < VTSS_CHIP_PORTS; p++) {
            JR2_RD(VTSS_SD10G65_DIG_SD10G65_APC_APC_EQZ_CTRL_STATUS(VTSS_TO_10G_APC_TGT(p)), &value);
            u32 eqz_gain = VTSS_X_SD10G65_DIG_SD10G65_APC_APC_EQZ_CTRL_STATUS_EQZ_GAIN_ACTVAL(value);
            u32 eqz_gain_adj = VTSS_X_SD10G65_DIG_SD10G65_APC_APC_EQZ_CTRL_STATUS_EQZ_GAIN_ADJ_ACTVAL(value);
            JR2_RD(VTSS_SD10G65_DIG_SD10G65_APC_APC_EQZ_CTRL_STATUS(VTSS_TO_10G_APC_TGT(p)), &value);
            u32 ld_lev = VTSS_X_SD10G65_DIG_SD10G65_APC_APC_EQZ_CTRL_STATUS_LD_LEV_ACTVAL(value);
            JR2_RD(VTSS_SD10G65_DIG_SD10G65_APC_APC_EQZ_OFFS_CTRL(VTSS_TO_10G_APC_TGT(p)), &value);
            u32 eqz_offs = VTSS_X_SD10G65_DIG_SD10G65_APC_APC_EQZ_OFFS_CTRL_EQZ_OFFS_ACTVAL(value);
            JR2_RD(VTSS_SD10G65_DIG_SD10G65_APC_APC_EQZ_AGC_CTRL(VTSS_TO_10G_APC_TGT(p)), &value);
            u32 eqz_agc = VTSS_X_SD10G65_DIG_SD10G65_APC_APC_EQZ_AGC_CTRL_EQZ_AGC_ACTVAL(value);
            JR2_RD(VTSS_SD10G65_DIG_SD10G65_APC_APC_EQZ_C_CTRL(VTSS_TO_10G_APC_TGT(p)), &value);
            u32 eqz_c = VTSS_X_SD10G65_DIG_SD10G65_APC_APC_EQZ_C_CTRL_EQZ_C_ACTVAL(value);
            JR2_RD(VTSS_SD10G65_DIG_SD10G65_APC_APC_EQZ_L_CTRL(VTSS_TO_10G_APC_TGT(p)), &value);
            u32 eqz_l = VTSS_X_SD10G65_DIG_SD10G65_APC_APC_EQZ_L_CTRL_EQZ_L_ACTVAL(value);
            JR2_RD(VTSS_SD10G65_DIG_SD10G65_APC_APC_DFE1_CTRL(VTSS_TO_10G_APC_TGT(p)), &value);
            u32 dfe1 = VTSS_X_SD10G65_DIG_SD10G65_APC_APC_DFE1_CTRL_DFE1_ACTVAL(value);
            JR2_RD(VTSS_SD10G65_DIG_SD10G65_APC_APC_DFE2_CTRL(VTSS_TO_10G_APC_TGT(p)), &value);
            u32 dfe2 = VTSS_X_SD10G65_DIG_SD10G65_APC_APC_DFE2_CTRL_DFE2_ACTVAL(value);
            JR2_RD(VTSS_SD10G65_DIG_SD10G65_APC_APC_DFE3_CTRL(VTSS_TO_10G_APC_TGT(p)), &value);
            u32 dfe3 = VTSS_X_SD10G65_DIG_SD10G65_APC_APC_DFE3_CTRL_DFE3_ACTVAL(value);
            JR2_RD(VTSS_SD10G65_DIG_SD10G65_APC_APC_DFE4_CTRL(VTSS_TO_10G_APC_TGT(p)), &value);
            u32 dfe4 = VTSS_X_SD10G65_DIG_SD10G65_APC_APC_DFE4_CTRL_DFE4_ACTVAL(value);
            pr("%-7d %-7d %-7d %-7d %-7d %-7d %-7d %-7d %-7d %-7d %-7d %-7d\n", 
               p, eqz_gain, eqz_gain_adj, ld_lev, eqz_offs, eqz_agc, eqz_c, eqz_l, dfe1, dfe2, dfe3, dfe4);
        }

        pr("\nLink status (MAC/SD/PCS):\n");
        pr("port          local_fault   remote_fault  idle_state    SD            rx_blk_lock   rx_hi_ber\n");
        for (p = VTSS_PORT_10G_START; p < VTSS_CHIP_PORTS; p++) {
            JR2_RD(VTSS_DEV10G_MAC_CFG_STATUS_MAC_TX_MONITOR_STICKY(VTSS_TO_DEV(p)), &value);
            JR2_RD(VTSS_SD10G65_SD10G65_IB_SD10G65_IB_CFG10(VTSS_TO_10G_SRD_TGT(p)), &sd);
            JR2_RD(VTSS_PCS_10GBASE_R_PCS_10GBR_STATUS_PCS_STATUS(VTSS_TO_10G_PCS_TGT(p)), &pcs);
            pr("%-13d %-13d %-13d %-13d %-13d %-13d %-13d\n", 
               p, 
               VTSS_X_DEV10G_MAC_CFG_STATUS_MAC_TX_MONITOR_STICKY_LOCAL_ERR_STATE_STICKY(value),
               VTSS_X_DEV10G_MAC_CFG_STATUS_MAC_TX_MONITOR_STICKY_REMOTE_ERR_STATE_STICKY(value),
               VTSS_X_DEV10G_MAC_CFG_STATUS_MAC_TX_MONITOR_STICKY_IDLE_STATE_STICKY(value),
               VTSS_X_SD10G65_SD10G65_IB_SD10G65_IB_CFG10_IB_IA_SDET(sd),
               VTSS_X_PCS_10GBASE_R_PCS_10GBR_STATUS_PCS_STATUS_RX_BLOCK_LOCK(pcs),
               VTSS_X_PCS_10GBASE_R_PCS_10GBR_STATUS_PCS_STATUS_RX_HI_BER(pcs));
            // Clear the stickies
            JR2_WR(VTSS_PCS_10GBASE_R_PCS_10GBR_STATUS_PCS_STATUS(VTSS_TO_10G_PCS_TGT(p)), 0xFFFFFFFF);
        }
    }

    /* Show SerDes setup if full configuration requested */
    if (!info->full) {
        return VTSS_RC_OK;
    }

    for (i = inst; i < (inst + (type == JR2_SERDES_TYPE_10G ? 4 : 1)); i++) {
        if (type == JR2_SERDES_TYPE_1G) {
            sprintf(buf, "SerDes1G_%u", i);
            vtss_jr2_debug_reg_header(pr, buf);
            VTSS_RC(jr2_sd1g_read(vtss_state, 1 << i));
            JR_DEBUG_HSIO(pr, SERDES1G_ANA_CFG_SERDES1G_DES_CFG, "DES_CFG");
            JR_DEBUG_HSIO(pr, SERDES1G_ANA_CFG_SERDES1G_IB_CFG, "IB_CFG");
            JR_DEBUG_HSIO(pr, SERDES1G_ANA_CFG_SERDES1G_OB_CFG, "OB_CFG");
            JR_DEBUG_HSIO(pr, SERDES1G_ANA_CFG_SERDES1G_SER_CFG, "SER_CFG");
            JR_DEBUG_HSIO(pr, SERDES1G_ANA_CFG_SERDES1G_COMMON_CFG, "COMMON_CFG");
            JR_DEBUG_HSIO(pr, SERDES1G_ANA_CFG_SERDES1G_PLL_CFG, "PLL_CFG");
            JR_DEBUG_HSIO(pr, SERDES1G_DIG_CFG_SERDES1G_MISC_CFG, "100FX_MISC");
            JR_DEBUG_HSIO(pr, SERDES1G_ANA_STATUS_SERDES1G_PLL_STATUS, "PLL_STATUS");
            JR_DEBUG_HSIO(pr, SERDES1G_DIG_CFG_SERDES1G_DFT_CFG0, "DFT_CFG0");
            JR_DEBUG_HSIO(pr, SERDES1G_DIG_CFG_SERDES1G_DFT_CFG1, "DFT_CFG1");
            JR_DEBUG_HSIO(pr, SERDES1G_DIG_CFG_SERDES1G_DFT_CFG2, "DFT_CFG2");
            JR_DEBUG_HSIO(pr, SERDES1G_DIG_CFG_SERDES1G_TP_CFG, "TP_CFG");
            JR_DEBUG_HSIO(pr, SERDES1G_DIG_CFG_SERDES1G_MISC_CFG, "MISC_CFG");
            JR_DEBUG_HSIO(pr, SERDES1G_DIG_STATUS_SERDES1G_DFT_STATUS, "DFT_STATUS");
        } else if (type == JR2_SERDES_TYPE_6G) {
            sprintf(buf, "SerDes6G_%u", i);
            vtss_jr2_debug_reg_header(pr, buf);
            VTSS_RC(jr2_sd6g_read(vtss_state, 1 << i));
            JR_DEBUG_HSIO(pr, SERDES6G_ANA_CFG_SERDES6G_IB_CFG, "IB_CFG");
            JR_DEBUG_HSIO(pr, SERDES6G_ANA_CFG_SERDES6G_IB_CFG1, "IB_CFG1");
            JR_DEBUG_HSIO(pr, SERDES6G_ANA_CFG_SERDES6G_OB_CFG, "OB_CFG");
            JR_DEBUG_HSIO(pr, SERDES6G_ANA_CFG_SERDES6G_OB_CFG1, "OB_CFG1");
            JR_DEBUG_HSIO(pr, SERDES6G_ANA_CFG_SERDES6G_SER_CFG, "SER_CFG");
            JR_DEBUG_HSIO(pr, SERDES6G_ANA_CFG_SERDES6G_COMMON_CFG, "COMMON_CFG");
            JR_DEBUG_HSIO(pr, SERDES6G_ANA_CFG_SERDES6G_PLL_CFG, "PLL_CFG");
            JR_DEBUG_HSIO(pr, SERDES6G_ANA_STATUS_SERDES6G_IB_STATUS0, "IB_STATUS0");
            JR_DEBUG_HSIO(pr, SERDES6G_ANA_STATUS_SERDES6G_IB_STATUS1, "IB_STATUS1");
            JR_DEBUG_HSIO(pr, SERDES6G_ANA_STATUS_SERDES6G_PLL_STATUS, "PLL_STATUS");
        } else {
        }
        pr("\n");
    }
    return VTSS_RC_OK;
}

static vtss_rc jr2_debug_port(vtss_state_t *vtss_state,
                              const vtss_debug_printf_t pr,
                              const vtss_debug_info_t   *const info)
    
{
    vtss_port_no_t port_no;
    u32            port;
    char           buf[32];

    for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {
        if (info->port_list[port_no] == 0)
            continue;
        port = VTSS_CHIP_PORT(port_no);
        sprintf(buf, "Port %u (%u)", port, port_no);
        vtss_jr2_debug_reg_header(pr, buf);
        VTSS_RC(jr2_debug_chip_port(vtss_state, pr, info, port_no));
    } /* Port loop */
    return VTSS_RC_OK;
}

static vtss_rc jr2_debug_port_counters(vtss_state_t *vtss_state,
                                       const vtss_debug_printf_t pr, 
                                       const vtss_debug_info_t   *const info, vtss_port_no_t port_no)
{
    u32                      i;
    char                     rx_buf[32], tx_buf[32];
    vtss_port_jr2_counters_t cnt;

    memset(&cnt, 0, sizeof(vtss_port_jr2_counters_t));
    VTSS_RC(jr2_port_counters_chip(vtss_state, port_no, &cnt, NULL, 0));

    if (port_no < vtss_state->port_count) {
        vtss_jr2_debug_cnt(pr, "ok_bytes", "out_bytes", &cnt.rx_ok_bytes, &cnt.tx_out_bytes);
        vtss_jr2_debug_cnt(pr, "uc", "", &cnt.rx_unicast, &cnt.tx_unicast);
        vtss_jr2_debug_cnt(pr, "mc", "", &cnt.rx_multicast, &cnt.tx_multicast);
        vtss_jr2_debug_cnt(pr, "bc", "", &cnt.rx_broadcast, &cnt.tx_broadcast);
    }

    if (!info->full) {
        pr("\n");
        return VTSS_RC_OK;
    }
    
    if (port_no < vtss_state->port_count) {
        vtss_jr2_debug_cnt(pr, "pause", "", &cnt.rx_pause, &cnt.tx_pause);
        vtss_jr2_debug_cnt(pr, "64", "", &cnt.rx_size64, &cnt.tx_size64);
        vtss_jr2_debug_cnt(pr, "65_127", "", &cnt.rx_size65_127, &cnt.tx_size65_127);
        vtss_jr2_debug_cnt(pr, "128_255", "", &cnt.rx_size128_255, &cnt.tx_size128_255);
        vtss_jr2_debug_cnt(pr, "256_511", "", &cnt.rx_size256_511, &cnt.tx_size256_511);
        vtss_jr2_debug_cnt(pr, "512_1023", "", &cnt.rx_size512_1023, &cnt.tx_size512_1023);
        vtss_jr2_debug_cnt(pr, "1024_1526", "", &cnt.rx_size1024_1518, &cnt.tx_size1024_1518);
        vtss_jr2_debug_cnt(pr, "jumbo", "", &cnt.rx_size1519_max, &cnt.tx_size1519_max);
        vtss_jr2_debug_cnt(pr, "crc", NULL, &cnt.rx_crc_err, NULL);
    }
    vtss_jr2_debug_cnt(pr, "local_drops", NULL, &cnt.rx_local_drops, NULL);
    vtss_jr2_debug_cnt(pr, "policer_drops", "queue_drops", &cnt.rx_policer_drops, &cnt.tx_queue_drops);
    
    for (i = 0; i < VTSS_PRIOS; i++) {
        sprintf(rx_buf, "class_%u", i);
        vtss_jr2_debug_cnt(pr, rx_buf, NULL, &cnt.rx_class[i], NULL);
    }
    for (i = 0; i < VTSS_PRIOS; i++) {
        sprintf(rx_buf, "green_drops_%u", i);
        sprintf(tx_buf, "green_%u", i);
        vtss_jr2_debug_cnt(pr, rx_buf, tx_buf, &cnt.rx_green_drops[i], &cnt.tx_green_class[i]);
    }
    for (i = 0; i < VTSS_PRIOS; i++) {
        sprintf(rx_buf, "yellow_drops_%u", i);
        sprintf(tx_buf, "yellow_%u", i);
        vtss_jr2_debug_cnt(pr, rx_buf, tx_buf, &cnt.rx_yellow_drops[i], &cnt.tx_yellow_class[i]);
    }
    pr("\n");

    return VTSS_RC_OK;
}

static vtss_rc jr2_debug_port_cnt(vtss_state_t *vtss_state,
                                  const vtss_debug_printf_t pr,
                                  const vtss_debug_info_t   *const info)
{
    /*lint --e{454, 455} */ // Due to VTSS_EXIT_ENTER
    vtss_port_no_t port_no;
    
    for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count + 4; port_no++) {
        if (port_no < vtss_state->port_count) {
            if (info->port_list[port_no] == 0)
                continue;
            pr("Counters for port: %u (chip_port: %u):\n\n", port_no, VTSS_CHIP_PORT(port_no));
        } else {
            if (!info->full)
                continue;
            pr("Counters for chip_port: %u:\n\n", VTSS_CHIP_PORT_CPU + port_no - vtss_state->port_count);
        }
        VTSS_EXIT_ENTER();
        (void)jr2_debug_port_counters(vtss_state, pr, info, port_no);
    }
    pr("\n");
    
    return VTSS_RC_OK;
}


static void jr2_debug_wm_dump(const vtss_debug_printf_t pr, const char *reg_name, u32 *value, u32 i, BOOL decode)
{
    u32 q;
    pr("%-25s", reg_name);
    for (q = 0; q < i; q++) {
        pr("%10u ", decode ? vtss_jr2_wm_dec(value[q]) * JR2_BUFFER_CELL_SZ : value[q]);
    }
    pr("\n");
}

static vtss_rc jr2_debug_wm(vtss_state_t *vtss_state,
                            const vtss_debug_printf_t pr,
                            const vtss_debug_info_t  *const info)

{
    u32 port_no, value, q, dp, cpu_port, port = 0;
    u32 id[8];
    u32 p_val1 = 0, p_val2 = 0, p_val3 = 0, p_val4 = 0, p_val5 = 0;
    u32 q_val1[8], q_val2[8], q_val3[8], q_val4[8], q_val5[8];
    u32 c_val1[8], c_val2[8], c_val3[8], c_val4[8], c_val5[8];

    for (q = 0; q < VTSS_PRIOS; q++) {
        q_val1[q] = q_val2[q] = q_val3[q] = q_val4[q] = q_val5[q] = 0;
        c_val1[q] = c_val2[q] = c_val3[q] = c_val4[q] = c_val5[q] = 0;
    }
   
    for (port_no = VTSS_PORT_NO_START; port_no <= vtss_state->port_count; port_no++) {
        cpu_port = (port_no == vtss_state->port_count);
        if (cpu_port) {
            /* CPU port */
            if (!info->full)
                continue;
            port = VTSS_CHIP_PORT_CPU;
        } else {
            if (!info->port_list[port_no]) {
                continue;
            }
            port = VTSS_CHIP_PORT(port_no);
        }
        if (cpu_port) {
            pr("CPU_Port                : %u\n",port_no);
        } else {
            pr("API Port (chip port:%d) : %u\n",port, port_no);
        }
        pr("============================\n\n");
        pr("General:\n");
        pr("------------\n");
        pr("Total Buffer (bytes)   : %d\n",JR2_BUFFER_MEMORY);
        pr("Total Frame References : %d\n",JR2_BUFFER_REFERENCE);
        pr("Cell size (bytes)      : %d\n",JR2_BUFFER_CELL_SZ);
        pr("Oversubsription factor : %d%%\n",BUF_OVERSUBSRIPTION_FACTOR/100);
        pr("Num of ports configured: %d\n",vtss_state->port_count+1);
        pr("\n");   
        pr("Flow Control:\n");
        pr("------------\n");
        JR2_RD(VTSS_DSM_CFG_ETH_PFC_CFG(port), &value);
        pr("PFC ena             : %d\n", VTSS_X_DSM_CFG_ETH_PFC_CFG_PFC_ENA(value));
        JR2_RD(VTSS_DSM_CFG_RX_PAUSE_CFG(port), &value);
        pr("FC Rx ena (standard): %d\n", (VTSS_X_DSM_CFG_RX_PAUSE_CFG_RX_PAUSE_EN(value)));
        JR2_RD(VTSS_QSYS_PAUSE_CFG_PAUSE_CFG(port), &value);
        pr("FC Tx ena (standard): %d\n", VTSS_X_QSYS_PAUSE_CFG_PAUSE_CFG_PAUSE_ENA(value));
        JR2_RD(VTSS_QSYS_PAUSE_CFG_PAUSE_CFG(port), &value);
        pr("FC Start    (Bytes) : %d\n", vtss_jr2_wm_dec(VTSS_X_QSYS_PAUSE_CFG_PAUSE_CFG_PAUSE_START(value)) * JR2_BUFFER_CELL_SZ);
        pr("FC Stop     (Bytes) : %d\n", vtss_jr2_wm_dec(VTSS_X_QSYS_PAUSE_CFG_PAUSE_CFG_PAUSE_STOP(value)) * JR2_BUFFER_CELL_SZ);
        JR2_RD(VTSS_QSYS_PAUSE_CFG_ATOP(port), &value);
        pr("FC Atop     (Bytes) : %d\n", vtss_jr2_wm_dec(VTSS_X_QSYS_PAUSE_CFG_ATOP_ATOP(value)) * JR2_BUFFER_CELL_SZ);
        JR2_RD(VTSS_QSYS_PAUSE_CFG_ATOP_TOT_CFG, &value);
        pr("FC ATOP_TOT (Bytes) : %d\n", vtss_jr2_wm_dec(VTSS_X_QSYS_PAUSE_CFG_ATOP_TOT_CFG_ATOP_TOT(value)) * JR2_BUFFER_CELL_SZ);
        pr("\n");   
        
        for (q = 0; q < VTSS_PRIOS; q++) {
            id[q] = q;
            JR2_RD(VTSS_QRES_RES_CTRL_RES_CFG((port * VTSS_PRIOS + q +    0)), &q_val1[q]);
            JR2_RD(VTSS_QRES_RES_CTRL_RES_CFG((port * VTSS_PRIOS + q + 1024)), &q_val2[q]);
            JR2_RD(VTSS_QRES_RES_CTRL_RES_CFG((port * VTSS_PRIOS + q + 2048)), &q_val3[q]);
            JR2_RD(VTSS_QRES_RES_CTRL_RES_CFG((port * VTSS_PRIOS + q + 3072)), &q_val4[q]);
            JR2_RD(VTSS_QRES_RES_CTRL_RES_CFG((port * VTSS_PRIOS + q + 4096)), &q_val5[q]);
        }
        
        jr2_debug_wm_dump(pr, "Queue level:", id, 8, 0);
        pr("-----------\n");
        jr2_debug_wm_dump(pr, "Qu Ingr Buf Rsrv (Bytes) :", q_val1, 8, 1);
        jr2_debug_wm_dump(pr, "Qu Ingr Ref Rsrv (Frames):", q_val2, 8, 0);
        jr2_debug_wm_dump(pr, "Qu Egr Buf Rsrv  (Bytes) :", q_val3, 8, 1);
        jr2_debug_wm_dump(pr, "Qu Egr Ref Rsrv  (Frames):", q_val4, 8, 0);
        jr2_debug_wm_dump(pr, "Qu Ingr PFC      (Bytes) :", q_val5, 8, 1);
        pr("\n");
            
        /* Configure reserved space for port */
        JR2_RD(VTSS_QRES_RES_CTRL_RES_CFG((port + 512 +    0)), &p_val1);
        JR2_RD(VTSS_QRES_RES_CTRL_RES_CFG((port + 512 + 1024)), &p_val2);
        JR2_RD(VTSS_QRES_RES_CTRL_RES_CFG((port + 512 + 2048)), &p_val3);
        JR2_RD(VTSS_QRES_RES_CTRL_RES_CFG((port + 512 + 3072)), &p_val4);
        JR2_RD(VTSS_QRES_RES_CTRL_RES_CFG((port + 512 + 4096)), &p_val5);
        pr("Port level (%u):\n",port_no);
        pr("-----------\n");
        pr("Port Ingress Buf Rsrv       (Bytes) : %u\n", vtss_jr2_wm_dec(p_val1)*JR2_BUFFER_CELL_SZ);
        pr("Port Ingress Ref Rsrv       (Frames): %u\n", p_val2);
        pr("Port Egress  Buf Rsrv       (Bytes) : %u\n", vtss_jr2_wm_dec(p_val3)*JR2_BUFFER_CELL_SZ);
        pr("Port Egress  Ref Rsrv       (Frames): %u\n", p_val4);
        pr("Port Ingress Buf Rsrv - PFC (Bytes) : %u\n", vtss_jr2_wm_dec(p_val5)*JR2_BUFFER_CELL_SZ);
        pr("\n");
    }

    /* Shared space for all QoS classes */
    for (q = 0; q < VTSS_PRIOS; q++) {
        JR2_RD(VTSS_QRES_RES_CTRL_RES_CFG((q + 496 +    0)), &c_val1[q]);
        JR2_RD(VTSS_QRES_RES_CTRL_RES_CFG((q + 496 + 1024)), &c_val2[q]);
        JR2_RD(VTSS_QRES_RES_CTRL_RES_CFG((q + 496 + 2048)), &c_val3[q]);
        JR2_RD(VTSS_QRES_RES_CTRL_RES_CFG((q + 496 + 3072)), &c_val4[q]);
        JR2_RD(VTSS_QRES_RES_CTRL_RES_CFG((q + 496 + 4096)), &c_val5[q]);
    }
    jr2_debug_wm_dump(pr, "QoS level:", id, 8, 0);
    pr("-----------\n");
    jr2_debug_wm_dump(pr, "QoS Ingr Buf       (Bytes) :", c_val1, 8, 1);
    jr2_debug_wm_dump(pr, "QoS Ingr Ref       (Frames):", c_val2, 8, 0);
    jr2_debug_wm_dump(pr, "QoS Egr Buf        (Bytes) :", c_val3, 8, 1);
    jr2_debug_wm_dump(pr, "QoS Egr Ref        (Frames):", c_val4, 8, 0);
    jr2_debug_wm_dump(pr, "QoS Ingr BUF - PFC (Bytes) :", c_val5, 8, 1);
    pr("\n");
        
    pr("Color level:\n");
    pr("-----------\n");
    /* Configure shared space for  both DP levels (green:0 yellow:1) */
    for (dp = 0; dp < 2; dp++) {
        JR2_RD(VTSS_QRES_RES_CTRL_RES_CFG((dp + 508 +    0)), &p_val1);
        JR2_RD(VTSS_QRES_RES_CTRL_RES_CFG((dp + 508 + 1024)), &p_val2);
        JR2_RD(VTSS_QRES_RES_CTRL_RES_CFG((dp + 508 + 2048)), &p_val3);
        JR2_RD(VTSS_QRES_RES_CTRL_RES_CFG((dp + 508 + 3072)), &p_val4);
        JR2_RD(VTSS_QRES_RES_CTRL_RES_CFG((dp + 508 + 4096)), &p_val5);
        pr("Port DP:%6s Ingress Buf (Bytes) :%u \n",dp?"Green":"Yellow",vtss_jr2_wm_dec(p_val1)*JR2_BUFFER_CELL_SZ);
        pr("Port DP:%6s Ingress Ref         :%u \n",dp?"Green":"Yellow",p_val2);
        pr("Port DP:%6s Egress  Buf (Bytes) :%u \n",dp?"Green":"Yellow",vtss_jr2_wm_dec(p_val3)*JR2_BUFFER_CELL_SZ);
        pr("Port DP:%6s Egress  Ref         :%u \n",dp?"Green":"Yellow",p_val4);
        pr("Port DP:%6s Ingress Buf  - PFC  :%u \n",dp?"Green":"Yellow",vtss_jr2_wm_dec(p_val5)*JR2_BUFFER_CELL_SZ);
    }
    pr("\n");

    pr("Burst capacity expected for one port (prio 7): %d\n",
       vtss_jr2_wm_dec(c_val1[7]) * JR2_BUFFER_CELL_SZ  + vtss_jr2_wm_dec(q_val1[7]) * JR2_BUFFER_CELL_SZ + 
       vtss_jr2_wm_dec(q_val3[7]) * JR2_BUFFER_CELL_SZ  + vtss_jr2_wm_dec(p_val1) * JR2_BUFFER_CELL_SZ + 
       vtss_jr2_wm_dec(p_val3) * JR2_BUFFER_CELL_SZ);

    for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {
        for (q = 0; q < VTSS_PRIOS; q++) {
            JR2_RD(VTSS_QRES_RES_CTRL_RES_STAT((VTSS_CHIP_PORT(port_no) * VTSS_PRIOS + q + 0)), &q_val1[q]);
            if (q_val1[q] > 0) {
                pr("API port %u (%u), ingress qu %u has used %u bytes of memory since last read\n",
                   port_no, VTSS_CHIP_PORT(port_no), q, q_val1[q]);
            }
        }
    }

    for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {
        port = VTSS_CHIP_PORT(port_no);
#if defined(VTSS_ARCH_JAGUAR_2_C)
        JR2_RD(VTSS_XQS_SYSTEM_FWD_DROP_EVENTS(port), &value);
#else
        JR2_RD(VTSS_QFWD_SYSTEM_FWD_DROP_EVENTS(port), &value);
#endif /* VTSS_ARCH_JAGUAR_2_C */
        if (value == 0) {
            continue;
        }
        pr("Port %u (%d) have dropped frame(s)\n",port_no, VTSS_CHIP_PORT(port_no));
#if defined(VTSS_ARCH_JAGUAR_2_C)
        JR2_WR(VTSS_XQS_SYSTEM_FWD_DROP_EVENTS(port), 0xFFFFFFFF); // Clear stickies
#else
        JR2_WR(VTSS_QFWD_SYSTEM_FWD_DROP_EVENTS(port), 0xFFFFFFFF); // Clear stickies
#endif /* VTSS_ARCH_JAGUAR_2_C */
    }

    return VTSS_RC_OK;
}

vtss_rc vtss_jr2_port_debug_print(vtss_state_t *vtss_state,
                                  const vtss_debug_printf_t pr,
                                  const vtss_debug_info_t   *const info)
{
    VTSS_RC(vtss_debug_print_group(VTSS_DEBUG_GROUP_PORT, jr2_debug_port, vtss_state, pr, info));
    VTSS_RC(vtss_debug_print_group(VTSS_DEBUG_GROUP_PORT_CNT, jr2_debug_port_cnt, vtss_state, pr, info));
    VTSS_RC(vtss_debug_print_group(VTSS_DEBUG_GROUP_WM, jr2_debug_wm, vtss_state, pr, info));
    return VTSS_RC_OK;
}

/* - Initialization ------------------------------------------------ */

static vtss_rc jr2_init_ana(vtss_state_t *vtss_state)
{
    u32 port, i, value;

    /* Initialize policers */
    JR2_WRM(VTSS_ANA_AC_POL_POL_ALL_CFG_POL_ALL_CFG,
            VTSS_F_ANA_AC_POL_POL_ALL_CFG_POL_ALL_CFG_STORM_GAP_VALUE(20)  | /* GAP_VALUE = 20 : Use line-rate measurement */
            VTSS_F_ANA_AC_POL_POL_ALL_CFG_POL_ALL_CFG_STORM_FORCE_INIT(1)  |
            VTSS_F_ANA_AC_POL_POL_ALL_CFG_POL_ALL_CFG_PRIO_FORCE_INIT(1)   |
            VTSS_F_ANA_AC_POL_POL_ALL_CFG_POL_ALL_CFG_ACL_FORCE_INIT(1)    |
            VTSS_F_ANA_AC_POL_POL_ALL_CFG_POL_ALL_CFG_FORCE_INIT(1),
            VTSS_M_ANA_AC_POL_POL_ALL_CFG_POL_ALL_CFG_STORM_GAP_VALUE      |
            VTSS_M_ANA_AC_POL_POL_ALL_CFG_POL_ALL_CFG_STORM_FORCE_INIT     |
            VTSS_M_ANA_AC_POL_POL_ALL_CFG_POL_ALL_CFG_PRIO_FORCE_INIT      |
            VTSS_M_ANA_AC_POL_POL_ALL_CFG_POL_ALL_CFG_ACL_FORCE_INIT       |
            VTSS_M_ANA_AC_POL_POL_ALL_CFG_POL_ALL_CFG_FORCE_INIT);

    /* Setup ANA_AC to count local drops and policer drops per port */
    JR2_WR(VTSS_ANA_AC_PS_STICKY_MASK_STICKY_MASK(0),
           VTSS_M_ANA_AC_PS_STICKY_MASK_STICKY_MASK_ZERO_DST_STICKY_MASK);
    JR2_WR(VTSS_ANA_AC_STAT_GLOBAL_CFG_PORT_STAT_GLOBAL_EVENT_MASK(JR2_CNT_ANA_AC_PORT_FILTER),
           VTSS_F_ANA_AC_STAT_GLOBAL_CFG_PORT_STAT_GLOBAL_EVENT_MASK_GLOBAL_EVENT_MASK(1<<0));
    JR2_WR(VTSS_ANA_AC_STAT_GLOBAL_CFG_PORT_STAT_GLOBAL_EVENT_MASK(JR2_CNT_ANA_AC_PORT_POLICER_DROPS),
           VTSS_F_ANA_AC_STAT_GLOBAL_CFG_PORT_STAT_GLOBAL_EVENT_MASK_GLOBAL_EVENT_MASK(0x7f0));
    for (port = 0; port < VTSS_CHIP_PORTS; port++) {
        JR2_WR(VTSS_ANA_AC_STAT_CNT_CFG_PORT_STAT_CFG(port, JR2_CNT_ANA_AC_PORT_FILTER),
               VTSS_F_ANA_AC_STAT_CNT_CFG_PORT_STAT_CFG_CFG_PRIO_MASK(0xff) |
               VTSS_F_ANA_AC_STAT_CNT_CFG_PORT_STAT_CFG_CFG_CNT_FRM_TYPE(1));
        JR2_WR(VTSS_ANA_AC_STAT_CNT_CFG_PORT_STAT_CFG(port, JR2_CNT_ANA_AC_PORT_POLICER_DROPS),
               VTSS_F_ANA_AC_STAT_CNT_CFG_PORT_STAT_CFG_CFG_PRIO_MASK(0xff) |
               VTSS_F_ANA_AC_STAT_CNT_CFG_PORT_STAT_CFG_CFG_CNT_FRM_TYPE(1));
    }

    /* Enable overall signaling of flow control from the port policers to DSM */
    JR2_WRM(VTSS_ANA_AC_POL_POL_ALL_CFG_POL_ALL_CFG,
            VTSS_F_ANA_AC_POL_POL_ALL_CFG_POL_ALL_CFG_PORT_FC_ENA(1),
            VTSS_M_ANA_AC_POL_POL_ALL_CFG_POL_ALL_CFG_PORT_FC_ENA);

    /* Setup ANA_AC to count non-FCS errors per queue */
    JR2_WR(VTSS_ANA_AC_STAT_GLOBAL_CFG_QUEUE_GLOBAL_CNT_FRM_TYPE_CFG(JR2_CNT_ANA_AC_QUEUE_PRIO),
           VTSS_F_ANA_AC_STAT_GLOBAL_CFG_QUEUE_GLOBAL_CNT_FRM_TYPE_CFG_GLOBAL_CFG_CNT_FRM_TYPE(0));

#if defined(VTSS_FEATURE_VLAN_COUNTERS)
    /* Enable VLAN counters */
    JR2_WRM_SET(VTSS_ANA_AC_PS_COMMON_MISC_CTRL, VTSS_M_ANA_AC_PS_COMMON_MISC_CTRL_USE_VID_AS_ISDX_ENA);
#endif /* VTSS_FEATURE_VLAN_COUNTERS */

    /* Setup ANA_AC SDX/VLAN statistics:
       - Even counters (0,2,4) are byte counters
       - Odd counters (1,3,5) are frame counters */
    for (i = 0; i < 6; i++) {
#if defined(VTSS_FEATURE_VLAN_COUNTERS)
        value = (i < 2 ? 0x08 : i < 4 ? 0x10 : 0x40); /* UC/MC/BC */
#else
        value = (1<<(i/2));
#endif /* VTSS_FEATURE_VLAN_COUNTERS */
        JR2_WR(VTSS_ANA_AC_STAT_GLOBAL_CFG_ISDX_STAT_GLOBAL_CFG(i),
               VTSS_F_ANA_AC_STAT_GLOBAL_CFG_ISDX_STAT_GLOBAL_CFG_GLOBAL_CFG_CNT_BYTE(i & 1 ? 0 : 1));
        JR2_WR(VTSS_ANA_AC_STAT_GLOBAL_CFG_ISDX_STAT_GLOBAL_EVENT_MASK(i),
               VTSS_F_ANA_AC_STAT_GLOBAL_CFG_ISDX_STAT_GLOBAL_EVENT_MASK_GLOBAL_EVENT_MASK(value));
    }

    /* Now, wait until initilization is complete */

    /* Await policer init */
    JR2_POLL_MASK(VTSS_ANA_AC_POL_POL_ALL_CFG_POL_ALL_CFG,
                  VTSS_M_ANA_AC_POL_POL_ALL_CFG_POL_ALL_CFG_STORM_FORCE_INIT |
#if defined(VTSS_ARCH_JAGUAR_2_B)
                   VTSS_M_ANA_AC_POL_POL_ALL_CFG_POL_ALL_CFG_PRIO_FORCE_INIT |
#endif
                   VTSS_M_ANA_AC_POL_POL_ALL_CFG_POL_ALL_CFG_ACL_FORCE_INIT  |
                   VTSS_M_ANA_AC_POL_POL_ALL_CFG_POL_ALL_CFG_FORCE_INIT);

    return VTSS_RC_OK;
}

static vtss_rc jr2_port_init(vtss_state_t *vtss_state)
{
    u32 port;

    VTSS_RC(jr2_init_ana(vtss_state));

    /* Disable LANE alignment per default  */
    for (port = VTSS_PORT_10G_START; port < VTSS_CHIP_PORTS; port++) {
        VTSS_RC(vtss_jr2_vaui_lane_alignement(vtss_state, port, 0));
    }

    /* Disable all Serdes instances (to save power) */
    for (port = VTSS_PORT_10G_START; port < VTSS_CHIP_PORTS; port++) {
        VTSS_RC(jr2_sd10g_cfg(vtss_state, VTSS_SERDES_MODE_DISABLE, port));
    }
    VTSS_RC(jr2_sd6g_cfg(vtss_state, VTSS_SERDES_MODE_DISABLE, 0xFFFFFF));
    VTSS_RC(jr2_sd1g_cfg(vtss_state, VTSS_SERDES_MODE_DISABLE, 0xFF));

    return VTSS_RC_OK;
}

vtss_rc vtss_jr2_port_init(vtss_state_t *vtss_state, vtss_init_cmd_t cmd)
{
    vtss_port_state_t *state = &vtss_state->port;

    switch (cmd) {
    case VTSS_INIT_CMD_CREATE:
        state->miim_read = jr2_miim_read;
        state->miim_write = jr2_miim_write;
#if defined(VTSS_FEATURE_10G)
        state->mmd_read = jr2_mmd_read;
        state->mmd_read_inc = jr2_mmd_read_inc;
        state->mmd_write = jr2_mmd_write;
#endif
        state->conf_get = jr2_port_conf_get;
        state->conf_set = jr2_port_conf_set;
        state->clause_37_status_get = jr2_port_clause_37_status_get;
        state->clause_37_control_get = jr2_port_clause_37_control_get;
        state->clause_37_control_set = jr2_port_clause_37_control_set;
        state->status_get = jr2_port_status_get;
        state->counters_update = jr2_port_counters_update;
        state->counters_clear = jr2_port_counters_clear;
        state->counters_get = jr2_port_counters_get;
        state->basic_counters_get = jr2_port_basic_counters_get;
        state->ifh_set = jr2_port_ifh_set;
        state->forward_set = jr2_port_forward_set;
        
        /* SYNCE features */
#if defined(VTSS_FEATURE_SYNCE)
        vtss_state->synce.clock_out_set = jr2_synce_clock_out_set;
        vtss_state->synce.clock_in_set = jr2_synce_clock_in_set;
#endif /* VTSS_FEATURE_SYNCE */
        break;

    case VTSS_INIT_CMD_INIT:
        VTSS_RC(jr2_port_init(vtss_state));
        break;

    case VTSS_INIT_CMD_PORT_MAP:
        if (!vtss_state->warm_start_cur) {
            VTSS_RC(jr2_port_buf_conf_set(vtss_state));
        }
        break;

    default:
        break;
    }
    return VTSS_RC_OK;
}

#endif /* VTSS_ARCH_JAGUAR_2 */

