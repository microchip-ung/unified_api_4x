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

#define VTSS_TRACE_GROUP VTSS_TRACE_GROUP_PHY

// Avoid "vtss_api.h not used in module vtss_phy_ts_api.c"
/*lint --e{766} */

#include "vtss_api.h"
#ifndef VIPER_B_FIFO_RESET
#define VIPER_B_FIFO_RESET
#endif

#if defined(VTSS_FEATURE_PHY_TIMESTAMP)

#if defined(VTSS_CHIP_CU_PHY)
#include "../../ail/vtss_state.h"
#include "../common/vtss_phy_common.h"
#include "vtss_phy_ts_api.h"
#include "vtss_phy_ts.h"
#ifdef VTSS_ARCH_DAYTONA
#include "../../daytona/vtss_daytona_basics.h"
#endif

# if defined(VTSS_FIFO_SYNC_DEBUG)
#include <sys/time.h>
#endif

#if defined(VTSS_FEATURE_MACSEC)
#include "vtss_macsec_api.h"
#endif /* VTSS_FEATURE_MACSEC */

#if defined(VTSS_CHIP_CU_PHY)
#include "../phy_1g/vtss_phy.h"
#include "../phy_1g/vtss_phy_init_scripts.h"
#endif

#if defined(VTSS_IOADDR)
#undef VTSS_IOADDR
#endif

#if defined(VTSS_IOREG)
#undef VTSS_IOREG
#endif

#if defined(VTSS_IOREG_IX)
#undef VTSS_IOREG_IX
#endif

#include "vtss_phy_ts_regs_common.h"
#include "vtss_phy_ts_regs.h"
#include "vtss_phy_ts_util.h"
/*lint -sem( vtss_callout_lock, thread_lock ) */
/*lint -sem( vtss_callout_unlock, thread_unlock ) */



// calculate the packet-to-packet delta for the given PHY's EPG settings
// INPUTS: port_no
//         Packet_Size Supported by EPG
//         InterPacket-Gap supported by EPG
// Return:
//         rtn-delta = Nsecs between two packets SOF
//

#ifdef ENABLE_1588_DEBUG_REG_READ
extern vtss_rc vtss_phy_1588_debug_reg_read_private(vtss_state_t *vtss_state, vtss_port_no_t port_no, u32 blk_id);
#endif


#ifdef TESLA_ING_TS_ERRFIX

/* *****************  Start of Lab Testing ONLY Definitions *********************************************** */
/* Simulation of WARMSTART */
/* This flag will simulate OOS Recovery being active and then the Processor restarts in the middle of OOS */
/* This means this is an Early Exit, but there is no recovery mechanism to reconfig the PHY */
//#define TESLA_FIFO_SYNC_OOS_WS_ERROR_TESTING

/* Simulation of EARLY EXIT OF OOS Recovery: */
/* Similar to CSR Access Error, MDIO Error, or other Error */
/* Lab Testing purposes ONLY, Forces Tesla OOS Early Exit Error every TESLA_FIFO_SYNC_OOS_EARLY_EXIT_COUNT iterations */
//#define TESLA_FIFO_SYNC_OOS_EARLY_EXIT_ERROR_TESTING

/* Having TESLA_FIFO_SYNC_OOS_WS_ERROR_TESTING Flag set will NOT exercise the OOS Reconfig of the PHY to return */
/* the PHY config to an Operational State so traffic can pass.  This would happen in the restart part of WARMSTART */
/* The TESLA_FIFO_SYNC_OOS_WS_ERROR_TESTING flag simulates OOS Recovery being active and then the Processor restarts in the middle of OOS */
#ifdef  TESLA_FIFO_SYNC_OOS_WS_ERROR_TESTING
/* Forces Tesla OOS Early Exit Error every TESLA_FIFO_SYNC_OOS_EARLY_EXIT_COUNT iterations */
#define TESLA_FIFO_SYNC_OOS_EARLY_EXIT_ERROR_TESTING
#endif

/* Lab Testing purposes ONLY, Forces Tesla OOS Early Exit Error every TESLA_FIFO_SYNC_OOS_EARLY_EXIT_COUNT iterations */
/* Having this Flag set will exercise the OOS Reconfig of the PHY to return it to Operational State so traffic can pass */
#ifdef TESLA_FIFO_SYNC_OOS_EARLY_EXIT_ERROR_TESTING
u32    tesla_fifo_sync_counter = 0;
#ifndef TESLA_FIFO_SYNC_OOS_EARLY_EXIT_COUNT
#define TESLA_FIFO_SYNC_OOS_EARLY_EXIT_COUNT  3   /* Forces Tesla OOS Early Exit Error every TESLA_FIFO_SYNC_OOS_EARLY_EXIT_COUNT iterations */
#endif
#endif

/* *****************  END of Lab Testing ONLY Definitions *********************************************** */

static u32     oos_counter[VTSS_PORTS];

static vtss_rc vtss_phy_ts_tesla_tsp_fifo_epg_done(const vtss_port_no_t  port_no, int pkt_count, BOOL ingress);
static vtss_rc vtss_phy_ts_tesla_8051_timer_start(vtss_state_t *vtss_state, vtss_port_no_t port_no,
                                                  u16 channel, BOOL ingress_fifo, u16 pkt_cnt, BOOL match);

vtss_rc vtss_phy_default_fifo_conf_tesla_oos_get(vtss_state_t *vtss_state, vtss_port_no_t port_no, vtss_phy_ts_fifo_conf_t *fifo_conf_tesla)
{
    memset(fifo_conf_tesla, 0, sizeof(vtss_phy_ts_fifo_conf_t));
    fifo_conf_tesla->detect_only = FALSE;
    fifo_conf_tesla->eng_recov = VTSS_PHY_TS_PTP_ENGINE_ID_0;
    fifo_conf_tesla->eng_minE = VTSS_PHY_TS_OAM_ENGINE_ID_2B;
    return (VTSS_RC_OK);
}

#undef CALC_PACKET_TO_PACKET_DELTA
#ifdef CALC_PACKET_TO_PACKET_DELTA

static u32  vtss_phy_ts_tesla_calc_pkt_to_pkt_delta_epg(vtss_state_t *vtss_state, vtss_port_no_t   port_no, u16 pkt_sz, u16 ipg)
{
    u32                rtn_delta = 0;
    vtss_miim_read_t   miim_read_func = vtss_state->init_conf.miim_read;
    vtss_miim_write_t  miim_write_func = vtss_state->init_conf.miim_write;
    u16                value = 0;

    miim_write_func(vtss_state, port_no, 0x1f, VTSS_PHY_PAGE_EXTENDED);
    miim_read_func(vtss_state, port_no, 0x1d, &value);
    VTSS_I("Reading Current EPG Setting: Reg29E1: 0x%x", value);
    miim_write_func(vtss_state, port_no, 0x1f, VTSS_PHY_PAGE_STANDARD);

    // NOTE: Current EPG Settings are **NOT** Used because the EPG could have been RESET!!!
    switch (pkt_sz) {
    case 64:
    case 125:
    case 1518:
    case 10000:
        if (ipg == 8192) {    // 8192ns InterPacket Gap
            rtn_delta = (8 + pkt_sz + 1024) * 8;
        } else {              // 96ns InterPacket Gap
            rtn_delta = (8 + pkt_sz + 12) * 8;
        }
        VTSS_I("  pkt_sz: %d    ipg: %d    Delta: %d   ", pkt_sz, ipg, rtn_delta);
        break;
    default:
        VTSS_E("INVALID pkt_sz selected!  pkt_sz: %d    ipg: %d    Not Supported!! ", pkt_sz, ipg);
    }

    return (rtn_delta);
}
#endif

static vtss_rc vtss_phy_ts_tesla_serdes_test_mode_get(vtss_state_t  *vtss_state, vtss_port_no_t   port_no, BOOL *is_mac_tst_ena, BOOL *is_media_tst_ena)
{
    vtss_rc            rc = VTSS_RC_OK;
    vtss_miim_write_t  miim_write_func = vtss_state->init_conf.miim_write;
    vtss_miim_read_t   miim_read_func = vtss_state->init_conf.miim_read;
    u16                reg17 = 0;
    u16                reg18 = 0;

    miim_write_func(vtss_state, port_no, 0x1f, VTSS_PHY_PAGE_TR);

    // Check MAC i/f TEST MODE Enabled - Trigger TR Read Request
    miim_write_func(vtss_state, port_no, 0x10, 0xac82);
    miim_read_func(vtss_state, port_no, 0x12, &reg18);
    *is_mac_tst_ena = ((reg18 & 0x0010) == 0x0010) ? 1 : 0;

    // Check Media i/f TEST MODE Enabled - Trigger TR Read Request
    miim_write_func(vtss_state, port_no, 0x10, 0xbe80);
    miim_read_func(vtss_state, port_no, 0x11, &reg17);
    *is_media_tst_ena = ((reg17 & 0x0040) == 0x0040) ? 1 : 0;

    VTSS_I("Checking SerDes Test Mode for port_no: %d,  is_mac_tst_ena?: 0x%x,  is_media_tst_ena?: 0x%x\n", port_no, *is_mac_tst_ena, *is_media_tst_ena);
    miim_write_func(vtss_state, port_no, 0x1f, VTSS_PHY_PAGE_STANDARD);

    return (rc);
}

static vtss_rc vtss_phy_ts_tesla_serdes_test_mode_set(vtss_state_t  *vtss_state, vtss_port_no_t   port_no, BOOL is_mac_if, BOOL enable)
{
    vtss_rc            rc = VTSS_RC_OK;
    vtss_miim_write_t  miim_write_func = vtss_state->init_conf.miim_write;

    VTSS_I("Configure SerDes Test Mode for port_no: %d,  Is_Mac?: 0x%x,  enable?: 0x%x\n", port_no, is_mac_if, enable);
    miim_write_func(vtss_state, port_no, 0x1f, VTSS_PHY_PAGE_TR);

    if (is_mac_if) {    // MAC
        if (enable) {      // Enable
            miim_write_func(vtss_state, port_no, 0x12, 0x001f);
            miim_write_func(vtss_state, port_no, 0x11, 0x83e0);
            miim_write_func(vtss_state, port_no, 0x10, 0x8c82);
        } else {           // Disable
            miim_write_func(vtss_state, port_no, 0x12, 0x000f);
            miim_write_func(vtss_state, port_no, 0x11, 0x83e0);
            miim_write_func(vtss_state, port_no, 0x10, 0x8c82);
        }
    } else {            // MEDIA
        if (enable) {      // Enable
            /* Set the Pattern */
            /* Note: This makes it ORDER Dependant!! */
            /* If MAC is called first to enable, this code will disable MAC */
            miim_write_func(vtss_state, port_no, 0x12, 0x000f);
            miim_write_func(vtss_state, port_no, 0x11, 0x83e0);
            miim_write_func(vtss_state, port_no, 0x10, 0x8c82);

            miim_write_func(vtss_state, port_no, 0x12, 0x0001);
            miim_write_func(vtss_state, port_no, 0x11, 0x1041);
            miim_write_func(vtss_state, port_no, 0x10, 0x9e80);
        } else {           // Disable
            miim_write_func(vtss_state, port_no, 0x12, 0x0001);
            miim_write_func(vtss_state, port_no, 0x11, 0x1001);
            miim_write_func(vtss_state, port_no, 0x10, 0x9e80);
        }
    }

    miim_write_func(vtss_state, port_no, 0x1f, VTSS_PHY_PAGE_STANDARD);

    return (rc);
}

/* *********************************************************************************** */
/* The following code checks Config to see if there was an Early Exit previously       */
/* *********************************************************************************** */
/* For Tesla PHY, Reg30, bit 13 (Reg30.13) is used to store in HW when OOS Recovery has been activated */
BOOL vtss_phy_ts_is_oos_recovery_enabled_private (vtss_state_t *vtss_state, vtss_port_no_t  port_no)
{
    vtss_rc              rc = 0;
    vtss_phy_type_t      phy_id;
    u16                  value = 0;
    BOOL                 oos_recov_enabled = FALSE;

    memset(&phy_id, 0, sizeof(vtss_phy_type_t));

    if ((rc = PHY_WR_PAGE(vtss_state, port_no, VTSS_PHY_PAGE, VTSS_PHY_PAGE_STANDARD)) != VTSS_RC_OK) {
        VTSS_E("TESLA_OOS: Failed to set page to VTSS_PHY_PAGE_STANDARD, port-no: %d\n", port_no);
    }

    if ((rc = vtss_phy_id_get_priv(vtss_state, port_no, &phy_id)) != VTSS_RC_OK) {
        VTSS_E("TESLA_OOS: Failed to get PHY_ID, port-no: %d\n", port_no);
    }

    if ((phy_id.part_number == VTSS_PHY_TYPE_8574) || (phy_id.part_number == VTSS_PHY_TYPE_8572)) {
        if ((rc = PHY_RD_PAGE(vtss_state, port_no, VTSS_PHY_LED_BEHAVIOR, &value)) != VTSS_RC_OK) {
            VTSS_E("TESLA_OOS: Failed to read VTSS_F_PHY_LED_BEHAVIOR_OOS_RECOVERY_ENABLE, port-no: %d\n", port_no);
        }
        if (value & VTSS_F_PHY_LED_BEHAVIOR_OOS_RECOVERY_ENABLE) {
            VTSS_D("TESLA_OOS: Detecting Reg30.13 Set, Prev OOS Recovery had Premature-Exit - Must Re-Run OOS! port: %d", port_no);
            oos_recov_enabled = TRUE;
        }
    }

    return (oos_recov_enabled);
}

/* For Tesla PHY, Reg30, bit 13 (Reg30.13) is used to store in HW when OOS Recovery has been activated */
/* Any one of the following settings will cause Packet Loss and must be Removed so that traffic can flow again */
/* If OOS Recovery Bit in Reg30.13 is set, that means OOS Recovery exited prematurely and likely has one or all of the following settings:  */
/* If the EPG is enabled, the PHY is configured so that MAC TX data will Not be sent out the PHY */
/* If the MAC Isolate is enabled, the PHY is configured so that MAC RX data will Not be sent from the PHY */
/* If the MAC SerDes TEST mode is enabled, the PHY is configured so that MAC SerDes i/f to MAC is DOWN */
/* If the MEDIA SerDes TEST mode is enabled, the PHY is configured so that MEDIA SerDes i/f to Link Partner is DOWN */
/* If the NE and FE Loopbacks are both enabled, the PHY is configured in an invalid operational mode */
/* This function checks ALL of the above settings.  If settings prevent traffic flow, they are changes to allow traffic */
/* Return Values:  VTSS_RC_OK = Disable was successful,  */
/*                 VTSS_RC_ERROR = Disable was not successful and traffic flow likely not possible */
vtss_rc vtss_phy_ts_tesla_oos_recovery_disable_priv(vtss_state_t *vtss_state, vtss_port_no_t  port_no, const vtss_debug_printf_t  pr)
{
    vtss_rc          rc = VTSS_RC_OK;
    vtss_phy_type_t  phy_id;
    u16              value = 0;
    u16  media_operating_mode = 0;
    BOOL oos_recov_enabled = FALSE;
    BOOL fe_loopback = FALSE;
    BOOL ne_loopback = FALSE;
    BOOL mac_isolate = FALSE;
    BOOL epg_enabled = FALSE;
    BOOL is_mac_tst_ena   = FALSE;  /* MAC SerDes enabled or in TEST Mode ? */
    BOOL is_media_tst_ena = FALSE;  /* Media SerDes enabled or in TEST Mode ? */
    vtss_miim_read_t        miim_read_func;
    vtss_miim_write_t       miim_write_func;

    miim_read_func = vtss_state->init_conf.miim_read;
    miim_write_func = vtss_state->init_conf.miim_write;

    memset(&phy_id, 0, sizeof(vtss_phy_type_t));

    if ((rc = vtss_phy_id_get_priv(vtss_state, port_no, &phy_id)) != VTSS_RC_OK) {
        VTSS_E("TESLA_OOS: Failed to get PHY_ID, port-no: %d\n", port_no);
    }

    VTSS_I("PHY Identified:PortID:%d PHY: %d Rev:%d Channel:%d  Channel count:%d Baseport:%d\n",
           port_no, phy_id.part_number, phy_id.revision, phy_id.channel_id, phy_id.port_cnt, phy_id.base_port_no);

    if ((phy_id.part_number == VTSS_PHY_TYPE_8574) || (phy_id.part_number == VTSS_PHY_TYPE_8572)) {
        oos_recov_enabled = vtss_phy_ts_is_oos_recovery_enabled_private(vtss_state, port_no);

        if ((rc = vtss_phy_ts_tesla_serdes_test_mode_get(vtss_state, port_no, &is_mac_tst_ena, &is_media_tst_ena)) != VTSS_RC_OK) {
            VTSS_I("TESLA_OOS: Failure occurred when checking MAC/MEDIA SerDes TEST Mode config on port: %d", port_no);
            //pr("Failure occurred when checking MAC/MEDIA SerDes TEST Mode config on port: %d\n", port_no);
        }

        if ((rc = PHY_RD_PAGE(vtss_state, port_no, VTSS_PHY_EXTENDED_PHY_CONTROL, &value)) != VTSS_RC_OK) {
            VTSS_I("TESLA_OOS: Failed to get Media_Operating_Mode, port-no: %d\n", port_no);
        }
        media_operating_mode = (value & VTSS_M_PHY_EXTENDED_PHY_CONTROL_MEDIA_OPERATING_MODE) >> 8;

        if (value & VTSS_F_PHY_EXTENDED_PHY_CONTROL_FAR_END_LOOPBACK_MODE) {
            VTSS_I("Entering OOS Recovery: Far-End Loopback is Enabled! port: %d", port_no);
            //pr("Entering OOS Recovery: Far-End Loopback is Enabled! port: %d\n", port_no);
            fe_loopback = TRUE;
        }

        if (media_operating_mode == 2) {
            VTSS_I("Entering OOS Recovery: Media Operating Mode is already configured for 1000BaseX Fiber/SFP Media! port: %d", port_no);
            //pr("Entering OOS Recovery: Media Operating Mode is already configured for 1000BaseX Fiber/SFP Media! port: %d\n", port_no);
        }

        value = 0;
        if ((rc = PHY_RD_PAGE(vtss_state, port_no, VTSS_PHY_MODE_CONTROL, &value)) != VTSS_RC_OK) {
            VTSS_I("TESLA_OOS: Failed to get Mode_Control, port-no: %d\n", port_no);
        }

        if (value & VTSS_F_PHY_MODE_CONTROL_LOOP) {
            VTSS_I("Entering OOS Recovery: Near-End Loopback is Enabled! port: %d", port_no);
            //pr("Entering OOS Recovery: Near-End Loopback is Enabled! port: %d\n", port_no);
            ne_loopback = TRUE;
        }

        if (value & VTSS_F_PHY_MODE_CONTROL_MAC_ISOLATE) {
            VTSS_I("Entering OOS Recovery: PHY MAC Isolate Enabled - MAC Rx Disabled - Total Packet Loss - MAC Rx!!  port: %d", port_no);
            //pr("Entering OOS Recovery: PHY MAC Isolate Enabled - MAC Rx Disabled - Total Packet Loss - MAC Rx!!  port: %d\n", port_no);
            mac_isolate = TRUE;
        }

        if (value & VTSS_F_PHY_MODE_CONTROL_AUTO_NEG_ENA) {
            VTSS_I("Entering OOS Recovery: ANEG Disabled!!  port: %d", port_no);
            //pr("Entering OOS Recovery: ANEG Disabled!!  port: %d\n", port_no);
        }

        /* Switch to extended page 1 */
        if ((rc = PHY_WR_PAGE(vtss_state, port_no, VTSS_PHY_PAGE, VTSS_PHY_PAGE_EXTENDED)) != VTSS_RC_OK) {
            VTSS_E("TESLA_OOS: Failed to set page to VTSS_PHY_PAGE_EXTENDED, port-no: %d\n", port_no);
        }

        value = 0;
        if ((rc = PHY_RD_PAGE(vtss_state, port_no, EPG_CTRL_REG_1, &value)) != VTSS_RC_OK) {
            VTSS_E("TESLA_OOS: Failed to read page to EPG_CTRL_REG_1, port-no: %d\n", port_no);
        }

        if (value & VTSS_F_EPG_CTRL_REG_1_EPG_ENABLE) {
            VTSS_I("Entering OOS Recovery: EPG Enabled - EPG Mux Enabled - MAC Tx Disabled - Total Packet Loss - MAC Tx!!  port: %d", port_no);
            //pr("Entering OOS Recovery: EPG Enabled - EPG Mux Enabled - MAC Tx Disabled - Total Packet Loss - MAC Tx!!  port: %d\n", port_no);
            epg_enabled = TRUE;
        }

        if ((rc = PHY_WR_PAGE(vtss_state, port_no, VTSS_PHY_PAGE, VTSS_PHY_PAGE_STANDARD)) != VTSS_RC_OK) {
            VTSS_E("TESLA_OOS: Failed to set page to VTSS_PHY_PAGE_STANDARD, port-no: %d\n", port_no);
        }

        /* For Tesla PHY, Reg30, bit 13 (Reg30.13) is used to store in HW when OOS Recovery has been activated */
        /* If OOS Recovery Bit in Reg30.13 is set, */
        /*    that means OOS Recovery exited prematurely and likely has one or all of the following settings:  */
        /* During the Tesla OOS Recovery, the PHY is completely isolated, thus this isolation must be undone for normal operation */
        /* If the EPG is enabled, the PHY is configured so that MAC TX data path is muxed and data will Not be sent out the PHY */
        /* If the MAC Isolate is enabled, the PHY is configured so that MAC RX data will Not be sent to the MAC from the PHY */
        /* If the MAC SerDes TEST mode is enabled, the PHY is configured so that MAC SerDes i/f to MAC is DOWN */
        /* If the MEDIA SerDes TEST mode is enabled, the PHY is configured so that MEDIA SerDes i/f to Link Partner is DOWN */
        /* If the NE and FE Loopbacks are both enabled, the PHY is configured in an invalid operational mode */
        /*    the NE and FE Loopbacks are setup so that packets can circulate within the isolated PHY */

        /* In the event there was a failure, Ensure EPG and Loopbacks are Disabled */
        /* Disable EPG */
        /* Make Sure that the EPG is Disabled Before Analyzer Reconfig !! */
        if (epg_enabled) {
            miim_write_func(vtss_state, port_no, 0x1f, VTSS_PHY_PAGE_EXTENDED);
            value = 0x0;
            miim_write_func(vtss_state, port_no, 0x1d, value);
            miim_read_func(vtss_state, port_no, 0x1d, &value);
            miim_write_func(vtss_state, port_no, 0x1f, VTSS_PHY_PAGE_STANDARD);
            VTSS_I("Port_No: %d, Disabling EPG, Turning OFF: Reading Ext:%d Reg:%02d: 0x%04X \n", port_no, VTSS_PHY_PAGE_EXTENDED, 0x1d, value);
        }

        if (fe_loopback || ne_loopback) {
            /* Disable ALL LoopBack configurations       */
            vtss_state->phy_state[port_no].loopback.far_end_enable = FALSE;
            vtss_state->phy_state[port_no].loopback.near_end_enable = FALSE;
            vtss_state->phy_state[port_no].loopback.connector_enable = FALSE;
            vtss_state->phy_state[port_no].loopback.mac_serdes_input_enable = FALSE;
            vtss_state->phy_state[port_no].loopback.mac_serdes_facility_enable = FALSE;
            vtss_state->phy_state[port_no].loopback.mac_serdes_equipment_enable = FALSE;
            vtss_state->phy_state[port_no].loopback.media_serdes_input_enable = FALSE;
            vtss_state->phy_state[port_no].loopback.media_serdes_facility_enable = FALSE;
            vtss_state->phy_state[port_no].loopback.media_serdes_equipment_enable = FALSE;
            if ((rc = vtss_phy_loopback_set_private( vtss_state, port_no)) != VTSS_RC_OK) {
                VTSS_E("ERROR during Disable ALL Loopbacks");
                return (VTSS_RC_ERROR);
            }
            VTSS_I("Disabling ALL Loopbacks");
        }

        if (mac_isolate) {
            /* Clear the MAC Isolate bit to Enable MAC i/f  */
            miim_write_func(vtss_state, port_no, 0x1f, VTSS_PHY_PAGE_STANDARD);
            miim_read_func(vtss_state, port_no, 0x0, &value);
            value &= ~(1 << 10);    // Enable MAC i/f - Clearing MAC ISOLATE BIT
            miim_write_func(vtss_state, port_no, 0x0, value);
            miim_read_func(vtss_state, port_no, 0x0, &value);
            VTSS_I("Cleared MAC Isolate, Reg 0: 0x%x", value);
        }

        // MEDIA i/f
        if (is_media_tst_ena) {
            VTSS_I("Disable of SerDes Test Mode MEDIA i/f and MAC i/f\n");
            if ((rc = vtss_phy_ts_tesla_serdes_test_mode_set(vtss_state, port_no, FALSE, FALSE)) != VTSS_RC_OK) {
                VTSS_E("Disable of SerDes Test Mode on MEDIA i/f failed");
                return (VTSS_RC_ERROR);
            }
        }


        // MAC i/f
        if (is_mac_tst_ena) {
            if ((rc = vtss_phy_ts_tesla_serdes_test_mode_set(vtss_state, port_no, TRUE, FALSE)) != VTSS_RC_OK) {
                VTSS_E("Disable of SerDes Test Mode on MAC i/f failed\n");
                return (VTSS_RC_ERROR);
            }
        }

        if (oos_recov_enabled || epg_enabled || mac_isolate || is_mac_tst_ena || is_media_tst_ena || (fe_loopback && ne_loopback)) {
            VTSS_I("WARNING: PHY WAS CONFIGURED FOR OOS RECOVERY! NOW DISABLED - Must Re-Run OOS Recovery <<<< port: %d", port_no);
            //pr("WARNING: PHY WAS CONFIGURED FOR OOS RECOVERY! NOW DISABLED - Must Re-Run OOS Rcovery <<<< port: %d \n", port_no);
        }
    }
    return (VTSS_RC_OK);
}


static vtss_rc vtss_phy_ts_tesla_epg_da_sa_get(vtss_match_type_t match_mode, u16  *value)
{
    vtss_rc            rc = VTSS_RC_OK;

    /* In analyzer, SA: FF:FF:FF:FF:FF:F0  DA: FF:FF:FF:FF:FF:F1 would be configured.
       DA is used for matching on ingress, SA is used for matching on egress.
       Rest of the addresses are non matching */
    // Clear the SA and DA
    *value &= 0xfc03; // SA clear FF:FF:FF:FF:FF:F0   DA clear FF:FF:FF:FF:FF:F0

    switch (match_mode) {
    case MATCH_INGRESS:
        *value |= (1 << 6); //match DA on FF:FF:FF:FF:FF:F1
        //match SA on FF:FF:FF:FF:FF:F0
        break;
    case MATCH_EGRESS:
        *value |= (5 << 6); //match DA on FF:FF:FF:FF:FF:F5
        //match SA on FF:FF:FF:FF:FF:F0
        break;
    case MATCH_INGRESS_EGRESS:
        *value |= (1 << 6); //match DA on FF:FF:FF:FF:FF:F1
        //match SA on FF:FF:FF:FF:FF:F0
        break;
    case MATCH_ENGINE_2_PLAN_E:
        *value |= (4 << 6); //match DA on FF:FF:FF:FF:FF:F4
        *value |= (3 << 2); //match SA on FF:FF:FF:FF:FF:F3
        break;
    case NO_MATCH_INGRESS:
        *value |= (5 << 6); //match DA on FF:FF:FF:FF:FF:F5
        //match SA on FF:FF:FF:FF:FF:F0
        break;
    case NO_MATCH_EGRESS:
        *value |= (1 << 6); //match DA on FF:FF:FF:FF:FF:F1
        *value |= (8 << 2); //match SA on FF:FF:FF:FF:FF:F8
        break;
    case NO_MATCH:
    default:
        *value |= (8 << 6); //match DA on FF:FF:FF:FF:FF:F8
        *value |= (8 << 2); //match SA on FF:FF:FF:FF:FF:F8
        break;
    }
    return (rc);
}

#define TESLA_RECOVERY_EPG_FRAME_PATTERN    (0x013F)
#define TESLA_EPG_300_PACKETS               (300)
// Configure EPG to send Recovery Frames - Ttl = 300Pkts; (64 or 125 byte) Pkts (UDP) w/SOF=INT - Pattern = 0x013F - Not Using 8051 to shutoff EPG
static vtss_rc vtss_phy_ts_tesla_epg_send_300_packets(vtss_state_t *vtss_state, vtss_port_no_t   port_no, const vtss_debug_printf_t pr, u16  pkt_sz, vtss_match_type_t match_mode, u32 *ing_delta, u32 *egr_delta )
{
    vtss_rc            rc = VTSS_RC_OK;
    vtss_miim_read_t   miim_read_func = vtss_state->init_conf.miim_read;
    vtss_miim_write_t  miim_write_func = vtss_state->init_conf.miim_write;
    u16                counter1 = 0;
    u16                value = 0;
    u8                 loop_cnt = 0;
    u16                val = 0;
    vtss_phy_ts_blk_id_t  blk_id = 0;
    vtss_phy_type_t       phy_id;
    u32                v = 0;
    u32                val_before_ing = 0;
    u32                val_after_ing = 0;
    u32                val_before_egr = 0;
    u32                val_after_egr = 0;
    vtss_port_no_t     cfg_port = port_no;
    BOOL               match = FALSE;


    VTSS_I("Configure EPG to send 300 Packets  \n");
    /* Clear Counters */
    if ((rc = vtss_phy_ts_base_port_get_priv(vtss_state, port_no, &cfg_port)) != VTSS_RC_OK) {
        VTSS_E("Base port get failed: port: %#x", port_no);
    }

    rc = vtss_phy_id_get_priv(vtss_state, port_no, &phy_id);
    if (rc != VTSS_RC_OK) {
        VTSS_E("EPG Error : Failed to get PHY_ID, port-no: %d\n", port_no);
        return VTSS_RC_ERROR;
    }

    if (phy_id.channel_id > 1) {
        blk_id = VTSS_PHY_TS_PROC_BLK_ID_1;
    } else {
        blk_id = VTSS_PHY_TS_PROC_BLK_ID_0;
    }

    switch (match_mode) {
    case MATCH_INGRESS:
    case MATCH_EGRESS:
    case MATCH_INGRESS_EGRESS:
    case MATCH_ENGINE_2_PLAN_E:
        match = TRUE;
        break;

    case NO_MATCH:
    case NO_MATCH_EGRESS:
    case NO_MATCH_INGRESS:
    default:
        match = FALSE;
        break;
    }

    VTSS_I("INGR_RW_MODFRM_CNT before timestamping 300 %s frames -- match_mode: 0x%x \n", (match ? "match" : "Non-Match"), match_mode );
    v = 0;
    //VTSS_PHY_TS_DISP_CSR(port_no,cfg_port, "INGR_RW_MODFRM_CNT", blk_id, 0x45, &v);
    VTSS_PHY_TS_DISP_CSR(port_no, blk_id, 0x45, &v);
    val_before_ing = v;
    VTSS_I("EGR_RW_MODFRM_CNT before timestamping 300 %s frames -- match_mode: 0x%x \n", (match ? "match" : "Non-Match"), match_mode );
    v = 0;
    //VTSS_PHY_TS_DISP_CSR(port_no,cfg_port, "EGR_RW_MODFRM_CNT", blk_id, 0x65, &v);
    VTSS_PHY_TS_DISP_CSR(port_no, blk_id, 0x65, &v);
    val_before_egr = v;

    loop_cnt = 5;   // Default
    /* Clear Counters */
    miim_write_func(vtss_state, port_no, 0x1f, VTSS_PHY_PAGE_EXTENDED_3);
    miim_read_func(vtss_state, port_no, 0x15, &value);
    VTSS_I("Configure EPG to send 300 small Packets (UDP)-Pattern:0x013F-  Reg21E3: 0x%x \n", value);

    switch (match_mode) {
    case MATCH_INGRESS:
    case MATCH_EGRESS:
    case MATCH_INGRESS_EGRESS:
    case MATCH_ENGINE_2_PLAN_E:
    case NO_MATCH_INGRESS:
        miim_write_func(vtss_state, port_no, 0x1f, VTSS_PHY_PAGE_TR);
        //miim_write_func(vtss_state, port_no, 0x10, 0xbe92); // read
        miim_write_func(vtss_state, port_no, 0x12, 0x000b);   // ETH-IP Frame
        /* Adding for UDP packet */
        miim_write_func(vtss_state, port_no, 0x11, 0x1110); // setting bits 15:8 to UDP protocol (0x11)
        miim_write_func(vtss_state, port_no, 0x10, 0x9e92); // writeback to TR

        miim_write_func(vtss_state, port_no, 0x10, 0xbe92); // read
        miim_read_func(vtss_state, port_no, 0x12, &value);
        miim_read_func(vtss_state, port_no, 0x11, &val);
        VTSS_I("TR Page (Match) Readback Value: Read at 0x12:%x Read at 0x11:%x\n", value, val);
        break;

    case NO_MATCH:
    case NO_MATCH_EGRESS:
        miim_write_func(vtss_state, port_no, 0x1f, VTSS_PHY_PAGE_TR);
        miim_write_func(vtss_state, port_no, 0x10, 0xbe92); // read
        miim_write_func(vtss_state, port_no, 0x12, 0x000a); // Ethernet frame
        /* Adding for UDP packet */
        miim_write_func(vtss_state, port_no, 0x11, 0x1110); // setting bits 15:8 to UDP protocol (0x11)
        miim_write_func(vtss_state, port_no, 0x10, 0x9e92); // writeback to TR

        miim_write_func(vtss_state, port_no, 0x10, 0xbe92); // read
        miim_read_func(vtss_state, port_no, 0x12, &value);
        miim_read_func(vtss_state, port_no, 0x11, &val);
        VTSS_I("TR Page (non-match) Readback Value: Read at 0x12:%x Read at 0x11:%x\n", value,  val);
        break;

    default:
        VTSS_I("ERROR: MATCH_MODE: 0x%x  ---  Not Supported!! \n", match_mode);
        break;
    }

    miim_write_func(vtss_state, port_no, 0x1f, VTSS_PHY_PAGE_EXTENDED);
    value = TESLA_RECOVERY_EPG_FRAME_PATTERN;
    miim_write_func(vtss_state, port_no, 0x1e, value);
    /*  When using 8051 toggling, 8051 is setting bit 14, the RUN bit.  This is done for Jumbo Frames */
    /*  0x9C40 = EPG Enable, Stop, 10,000 Byte Frames, IPG=8192nsec, Dst=0001, Src=0000, Fixed Payload Pattern, Good FCS */
    /*  0x9840 = EPG Enable, Stop, 10,000 Byte Frames, IPG=96nsec, Dst=0001, Src=0000, Fixed Payload Pattern, Good FCS */
    /*  0xC040 = EPG Enable, Run, 125 Byte Frames, IPG=96nsec, Dst=0001, Src=0000, Fixed Payload Pattern, Good FCS */
    /*  0x8040 = EPG Enable, Stop, 125 Byte Frames, IPG=96nsec, Dst=0001, Src=0000, Fixed Payload Pattern, Good FCS */
    /*  0xE840 = EPG Enable, Run, 64 Byte Frames, IPG=96nsec, Dst=0001, Src=0000, Fixed Payload Pattern, Good FCS */
    if (pkt_sz == 64) {
        VTSS_I("Configure EPG to send 300 x 64-byte packets, IPG=96ns (UDP) - Pattern = 0x013F - Not Using 8051 \n");
        value = 0x8840;  // Configure, but don't Run 64 byte
    } else if (pkt_sz == 125) {
        VTSS_I("Configure EPG to send 300 x 125-byte packets, IPG=96ns (UDP) - Pattern = 0x013F - Not Using 8051 \n");
        value = 0x8040;  // Configure, but don't Run 125 byte
    } else if (pkt_sz == 1518) {
        VTSS_I("Configure EPG to send 300 x 1518-byte packets, IPG=96ns (UDP) - Pattern = 0x013F - Not Using 8051 \n");
        value = 0x9040;  // Configure, but don't Run 125 byte
        loop_cnt = 8;
    } else if (pkt_sz == 10000) {
        VTSS_I("Configure EPG to send 300 x 10000-byte packets, IPG=96ns (UDP) - Pattern = 0x013F - Not Using 8051 \n");
        value = 0x9840;  // Configure, but don't Run 125 byte
        loop_cnt = 30;
    } else {   // Default to 125 byte packets
        VTSS_I("Configure EPG to send 300 x 125-byte packets, IPG=96ns (UDP) - Pattern = 0x013F - Not Using 8051 \n");
        value = 0x8040;  // Configure, but don't Run 125 byte
    }

    /*  Set the DA and SA based upon the Match Mode for the EPG */
    if ((rc = vtss_phy_ts_tesla_epg_da_sa_get(match_mode, &value)) != VTSS_RC_OK) {
        VTSS_E("FAILURE:  Retrieving DA and SA using Match_Mode \n");
    }
    VTSS_I("MATCH-MODE: 0x%x;    value:%x\n", match_mode, value);

    miim_write_func(vtss_state, port_no, 0x1d, value);
    miim_read_func(vtss_state, port_no, 0x1d, &value);
    value |= (1 << 14);   // Run!!
    VTSS_I("Running EPG (Setting bit 14): Reg29E1: 0x%x\n", value);
    miim_write_func(vtss_state, port_no, 0x1d, value);
    miim_read_func(vtss_state, port_no, 0x1d, &value);

    /* STOPPING EPG is Not necessary - It stops on it's own after 300 packets and changes the Run/Stop bit to Stop */
    /* However, we need to verify the Run bit is cleared */
    do {
        miim_read_func(vtss_state, port_no, 0x1d, &value);
        VTSS_I("port_no: %d, Checking EPG for Completion, Reg29E1:0x%04X \n", port_no, value);
        if (!(value & (1 << 14))) {   // Bit 14: 1=Run, 0=Stop
            break;
        }
        VTSS_MSLEEP(1);
        loop_cnt--;
    } while (loop_cnt > 0);

    miim_write_func(vtss_state, port_no, 0x1f, VTSS_PHY_PAGE_EXTENDED);
    miim_read_func(vtss_state, port_no, 0x1d, &value);
    value &= ~(1 << 14);   // STOP!!  It should already be Stopped, but just in case, Force Stop!
    miim_write_func(vtss_state, port_no, 0x1d, value);
    miim_read_func(vtss_state, port_no, 0x1d, &value);
    VTSS_I("Ensuring EPG Stopped (Clearing bit 14): Reg29E1: 0x%x\n", value);

    miim_write_func(vtss_state, port_no, 0x1f, VTSS_PHY_PAGE_EXTENDED_3);
    miim_read_func(vtss_state, port_no, 0x15, &counter1);
    miim_read_func(vtss_state, port_no, 0x15, &value);
    VTSS_I("Tx Counters Reg_21E3   1st Read RegVal: 0x%x,  Counter: %d    2nd Read RegVal: 0x%x,   Counter:%d \n", counter1, (counter1 & 0x3fff), value, (value & 0x3fff));

    miim_write_func(vtss_state, port_no, 0x1f, VTSS_PHY_PAGE_STANDARD);

    VTSS_I("INGR_RW_MODFRM_CNT after timestamping 300 %s frames -- match_mode: 0x%x \n", (match ? "match" : "Non-Match"), match_mode );
    v = 0;
    //VTSS_PHY_TS_DISP_CSR(port_no,cfg_port, "INGR_RW_MODFRM_CNT", blk_id, 0x45, &v);
    VTSS_PHY_TS_DISP_CSR(port_no, blk_id, 0x45, &v);
    val_after_ing = v;
    VTSS_I("EGR_RW_MODFRM_CNT after timestamping 300 %s frames -- match_mode: 0x%x \n", (match ? "match" : "Non-Match"), match_mode );
    v = 0;
    //VTSS_PHY_TS_DISP_CSR(port_no,cfg_port, "EGR_RW_MODFRM_CNT", blk_id, 0x65, &v);
    VTSS_PHY_TS_DISP_CSR(port_no, blk_id, 0x65, &v);
    val_after_egr = v;

    if (val_before_ing > val_after_ing) {  // Rollover ??
        val_after_ing += (0xffffffff + 1);
    }
    if (val_before_egr > val_after_egr) {  // Rollover ??
        val_after_egr += (0xffffffff + 1);
    }

    *ing_delta = val_after_ing - val_before_ing;
    *egr_delta = val_after_egr - val_before_egr;
    pr("300 %s frames-No of packets timestampped Ingress:%d\n", (match ? "match" : "Non-Match"), (*ing_delta));
    pr("300 %s frames-No of packets timestampped Egress:%d\n",  (match ? "match" : "Non-Match"), (*egr_delta));

    return (rc);
}

// Configure EPG to send Recovery Frames-Ttl=10Pkts; 9 Jumbo (10000 byte) Pkts (UDP) w/SOF=EXT, 1Pkt w/SOF=INT-Pattern=0x013F-Using 8051 to stop EPG after 10 Frames
/* configure timer for 9 packets */
/* Using the 8051 Timer, the SOF is toggled at packet 9  */
// Send 10 Jumbo Frames with max IPG and Clear the Ext SOF Pin @ an Odd Boundary, ie. @ 1,3,5,7, or 9 - We preferred 9
static vtss_rc vtss_phy_ts_tesla_epg_send_jumbo_packets_8051(vtss_state_t *vtss_state, vtss_port_no_t   port_no, const vtss_debug_printf_t pr, u16   channel, BOOL  ingress, u16 sof_toggle_pkt, vtss_match_type_t match_mode, u32 *ing_delta, u32 *egr_delta)
{
    vtss_rc            rc = VTSS_RC_OK;
    vtss_miim_read_t   miim_read_func = vtss_state->init_conf.miim_read;
    vtss_miim_write_t  miim_write_func = vtss_state->init_conf.miim_write;
    vtss_phy_ts_blk_id_t  blk_id = 0;
    vtss_phy_type_t       phy_id;
    u16                value = 0;
    u8                 loop_cnt = 0;
    u16                counter1 = 0;
    u32                v = 0;
    long unsigned int                val_before_ing = 0;
    long unsigned int                val_after_ing = 0;
    long unsigned int                val_before_egr = 0;
    long unsigned int                val_after_egr = 0;
    vtss_port_no_t     cfg_port = port_no;
    BOOL               match = FALSE;

    VTSS_I("Configuring EPG to send 10 JUMBO Packets: Port_no: %d,  ING/EGR= %s \n", port_no, (ingress ? "INGRESS" : "EGRESS"));

    /* Clear Counters */
    if ((rc = vtss_phy_ts_base_port_get_priv(vtss_state, port_no, &cfg_port)) != VTSS_RC_OK) {
        VTSS_E("Base port get failed: port: %#x", port_no);
    }

    rc = vtss_phy_id_get_priv(vtss_state, port_no, &phy_id);
    if (rc != VTSS_RC_OK) {
        VTSS_E("EPG Error : Failed to get PHY_ID, port-no: %d", port_no);
        return VTSS_RC_ERROR;
    }

    if (phy_id.channel_id > 1) {
        blk_id = VTSS_PHY_TS_PROC_BLK_ID_1;
    } else {
        blk_id = VTSS_PHY_TS_PROC_BLK_ID_0;
    }

    switch (match_mode) {
    case MATCH_INGRESS:
    case MATCH_EGRESS:
    case MATCH_INGRESS_EGRESS:
    case MATCH_ENGINE_2_PLAN_E:
        match = TRUE;
        break;

    case NO_MATCH:
    case NO_MATCH_EGRESS:
    case NO_MATCH_INGRESS:
    default:
        match = FALSE;
        break;
    }

    VTSS_I("INGR_RW_MODFRM_CNT before timestamping 10 %s JUMBO frames  -- match_mode: 0x%x \n", (match ? "match" : "Non-Match"), match_mode);
    v = 0;
    //VTSS_PHY_TS_DISP_CSR(port_no,cfg_port, "INGR_RW_MODFRM_CNT", blk_id, 0x45, &v);
    VTSS_PHY_TS_DISP_CSR(port_no, blk_id, 0x45, &v);
    val_before_ing = v;
    VTSS_I("EGR_RW_MODFRM_CNT before timestamping 10 %s JUMBO frames -- match_mode: 0x%x \n", (match ? "match" : "Non-Match"), match_mode);
    v = 0;
    //VTSS_PHY_TS_DISP_CSR(port_no,cfg_port, "EGR_RW_MODFRM_CNT", blk_id, 0x65, &v);
    VTSS_PHY_TS_DISP_CSR(port_no, blk_id, 0x65, &v);
    val_before_egr = v;

    /* Clear Counters */
    miim_write_func(vtss_state, port_no, 0x1f, VTSS_PHY_PAGE_EXTENDED_3);
    miim_read_func(vtss_state, port_no, 0x15, &value);

    VTSS_I("Configure EPG to send 10 Jumbo Packets (UDP)-Pattern:0x013F-8051 toggles SOF to INT at packet: %d  Reg21E3: 0x%x \n", sof_toggle_pkt, value);
    //VTSS_I("Configure EPG to send 10 Jumbo Packets (UDP)-Pattern:0x013F-8051 toggles SOF to INT at packet: %d  Reg21E3: 0x%x", sof_toggle_pkt, value);

    miim_write_func(vtss_state, port_no, 0x1f, VTSS_PHY_PAGE_TR);

    switch (match_mode) {
    case MATCH_INGRESS:
    case MATCH_EGRESS:
    case MATCH_INGRESS_EGRESS:
    case MATCH_ENGINE_2_PLAN_E:
    case NO_MATCH_INGRESS:
        miim_write_func(vtss_state, port_no, 0x10, 0xbe92); // read
        miim_write_func(vtss_state, port_no, 0x12, 0x000b); // ETH-IP Frame
        /* Adding for UDP packet */
        miim_write_func(vtss_state, port_no, 0x11, 0x1110); // setting bits 15:8 to UDP protocol (0x11)
        miim_write_func(vtss_state, port_no, 0x10, 0x9e92); // writeback to TR
        break;

    case NO_MATCH:
    case NO_MATCH_EGRESS:
        miim_write_func(vtss_state, port_no, 0x10, 0xbe92); // read
        miim_write_func(vtss_state, port_no, 0x12, 0x000a); // Ethernet Frame
        /* Adding for UDP packet */
        miim_write_func(vtss_state, port_no, 0x11, 0x1110); // setting bits 15:8 to UDP protocol (0x11)
        miim_write_func(vtss_state, port_no, 0x10, 0x9e92); // writeback to TR
        break;

    default:
        VTSS_I("ERROR: MATCH_MODE: 0x%x  ---  Not Supported!! \n", match_mode);
        break;
    }

    miim_write_func(vtss_state, port_no, 0x1f, VTSS_PHY_PAGE_EXTENDED);
    value = 0x13F;
    miim_write_func(vtss_state, port_no, 0x1e, value);
    /*  When using 8051 toggling, 8051 is setting bit 14, the RUN bit.  This is done for Jumbo Frames */
    /*  0x9C40 = EPG Enable, Stop, 10,000 Byte Frames, IPG=8192nsec, Dst=0001, Src=0000, Fixed Payload Pattern, Good FCS */
    /*  0x9840 = EPG Enable, Stop, 10,000 Byte Frames, IPG=96nsec, Dst=0001, Src=0000, Fixed Payload Pattern, Good FCS */
    /*  0x9440 = EPG Enable, Stop, 1518 Byte Frames, IPG=96nsec, Dst=0001, Src=0000, Fixed Payload Pattern, Good FCS */
    /*  0xC040 = EPG Enable, Run, 125 Byte Frames, IPG=96nsec, Dst=0001, Src=0000, Fixed Payload Pattern, Good FCS */
    /*  0xE840 = EPG Enable, Run, 64 Byte Frames, IPG=96nsec, Dst=0001, Src=0000, Fixed Payload Pattern, Good FCS */
    value = 0x9440;   /*  Jumbo Frames = 0x9440,1518 Byte size Using 8051 to Set/Clear bit 14, RUN bit */

    /*  Set the DA and SA based upon the Match Mode for the EPG */
    if ((rc = vtss_phy_ts_tesla_epg_da_sa_get(match_mode, &value)) != VTSS_RC_OK) {
        VTSS_E("FAILURE:  Retrieving DA and SA using Match_Mode \n");
    }
    VTSS_I("MATCH-MODE: 0x%x;    value:%x\n", match_mode, value);

    miim_write_func(vtss_state, port_no, 0x1d, value);
    miim_read_func(vtss_state, port_no, 0x1d, &value);
    VTSS_I("Port_No: %d, EPG ON: Reading Ext:%d Reg:%02d: 0x%04X \n", port_no, VTSS_PHY_PAGE_EXTENDED, 0x1d, value);

    /* configure timer for to go off @ sof_toggle # of packets, which toggles SOF to INT at that packet */
    /* Using the 8051 Timer, Automatically flips us from EXT PCS to INT PCS at an Odd State, If SOF already INT, then this is a No-Op */
    // Send 10 Jumbo Frames with max IPG and Clear the Ext SOF Pin @ an Odd Boundary, ie. @ 1,3,5,7, or 9 - We preferred 9
    miim_write_func(vtss_state, port_no, 0x1f, VTSS_PHY_PAGE_STANDARD);
    if (VTSS_RC_OK != vtss_phy_ts_tesla_8051_timer_start(vtss_state, port_no, channel, ingress, sof_toggle_pkt, match )) {
        VTSS_I("Error in starting 8051 timer\n");
    }

    //VTSS_RC(vtss_phy_ts_tesla_tsp_fifo_epg_done(port_no, 1, ingress)); // This function just waits via MSLEEP

    /* STOPPING EPG is Not necessary - It stops on it's own after 300 packets and changes the Run/Stop bit to Stop */
    /* However, we need to verify the Run bit is cleared */
    miim_write_func(vtss_state, port_no, 0x1f, VTSS_PHY_PAGE_EXTENDED);
    loop_cnt = 8;
    do {
        miim_read_func(vtss_state, port_no, 0x1d, &value);
        VTSS_I("port_no: %d, Checking EPG for Completion, Reg29E1:0x%04X \n", port_no, value);
        if (!(value & (1 << 14))) {   // Bit 14: 1=Run, 0=Stop
            break;
        }
        VTSS_MSLEEP(1);
        loop_cnt--;
    } while (loop_cnt > 0);

    miim_write_func(vtss_state, port_no, 0x1f, VTSS_PHY_PAGE_EXTENDED);
    miim_read_func(vtss_state, port_no, 0x1d, &value);
    value &= ~(1 << 14);   // STOP!!  It should already be Stopped, but just in case, Force Stop!
    miim_write_func(vtss_state, port_no, 0x1d, value);
    miim_read_func(vtss_state, port_no, 0x1d, &value);
    VTSS_I("Ensuring EPG Stopped (Clearing bit 14): Reg29E1: 0x%x  \n", value);

    miim_write_func(vtss_state, port_no, 0x1f, VTSS_PHY_PAGE_EXTENDED_3);
    miim_read_func(vtss_state, port_no, 0x15, &counter1);
    miim_read_func(vtss_state, port_no, 0x15, &value);
    VTSS_I("Tx Counters Reg_21E3   1st Read RegVal: 0x%x,  Counter: %d    2nd Read RegVal: 0x%x,   Counter:%d \n", counter1, (counter1 & 0x3fff), value, (value & 0x3fff));

    VTSS_I("EPG Done - 8051 Set SOF to Internal PCS at Packet: %d ", sof_toggle_pkt);

    miim_write_func(vtss_state, port_no, 0x1f, VTSS_PHY_PAGE_STANDARD);

    VTSS_I("INGR_RW_MODFRM_CNT after timestamping 10 %s JUMBO frames -- match_mode: 0x%x \n", (match ? "match" : "Non-Match"), match_mode);
    v = 0;
    //VTSS_PHY_TS_DISP_CSR(port_no,cfg_port, "INGR_RW_MODFRM_CNT", blk_id, 0x45, &v);
    VTSS_PHY_TS_DISP_CSR(port_no, blk_id, 0x45, &v);
    val_after_ing = v;
    VTSS_I("EGR_RW_MODFRM_CNT after timestamping 10 %s JUMBO frames -- match_mode: 0x%x \n", (match ? "match" : "Non-Match"), match_mode);
    v = 0;
    //VTSS_PHY_TS_DISP_CSR(port_no,cfg_port, "EGR_RW_MODFRM_CNT", blk_id, 0x65, &v);
    VTSS_PHY_TS_DISP_CSR(port_no, blk_id, 0x65, &v);
    val_after_egr = v;

    if (val_before_ing > val_after_ing) {  // Rollover ??
        val_after_ing += (0xffffffff + 1);
    }
    if (val_before_egr > val_after_egr) {  // Rollover ??
        val_after_egr += (0xffffffff + 1);
    }

    *ing_delta = val_after_ing - val_before_ing;
    *egr_delta = val_after_egr - val_before_egr;

    pr("%d Jumbo %s frames with Ext SOF-No of packets timestampped Ingress:%d\n", sof_toggle_pkt, (match ? "match" : "Non-Match"), (*ing_delta));
    pr("%d Jumo %s frames with Ext SOF-No of packets timestampped Egress:%d\n", sof_toggle_pkt, (match ? "match" : "Non-Match"), (*egr_delta));
    return (rc);
}

static vtss_rc vtss_phy_ts_tesla_ingress_tsp_fifo_sync_flow_add(vtss_state_t *vtss_state, vtss_port_no_t   port_no, vtss_phy_ts_engine_channel_map_t channel, vtss_phy_ts_engine_t eng_id, BOOL ing)
{

    vtss_rc                         rc = VTSS_RC_OK;
    vtss_port_no_t                  base_port_no, cfgport;
    vtss_phy_ts_blk_id_t            blk_id = VTSS_PHY_TS_ANA_BLK_ID_ING_0;
    vtss_phy_ts_eng_conf_t          *eng_conf;
    vtss_phy_ts_eth_conf_t          *eth_conf;
    vtss_phy_ts_ip_conf_t           *ip_conf;
    u8                              flow_id = 7; /* Using last flow for Recovery process */
    u32                             value, temp;
    u32                             i;

    if ((rc = vtss_phy_ts_base_port_get_priv(vtss_state, port_no, &base_port_no)) != VTSS_RC_OK) {
        VTSS_E(" Baseport get failed for Port-no: %d\n", port_no);
        return VTSS_RC_ERROR;
    }

    cfgport = base_port_no;
    if (ing) {
        blk_id = (eng_id == VTSS_PHY_TS_PTP_ENGINE_ID_0) ? VTSS_PHY_TS_ANA_BLK_ID_ING_0 : VTSS_PHY_TS_ANA_BLK_ID_ING_1;
        eng_conf = &vtss_state->phy_ts_port_conf[base_port_no].ingress_eng_conf[eng_id];
        VTSS_I("ING-Flow Add: Port-no: %d Base Port No: %d ing:0x%x, Channel Mask: %d blk_id: x%x  Engine :%d \n", port_no,
               base_port_no, ing, channel, blk_id, eng_id);
    } else {
        blk_id = (eng_id == VTSS_PHY_TS_PTP_ENGINE_ID_0) ? VTSS_PHY_TS_ANA_BLK_ID_EGR_0 : VTSS_PHY_TS_ANA_BLK_ID_EGR_1;
        eng_conf = &vtss_state->phy_ts_port_conf[base_port_no].egress_eng_conf[eng_id];
        VTSS_I("EGR-Flow Add: Port-no: %d Base Port No: %d ing:0x%x, Channel Mask: %d blk_id: x%x  Engine :%d \n", port_no,
               base_port_no, ing, channel, blk_id, eng_id);
    }

    eth_conf = &eng_conf->flow_conf.flow_conf.ptp.eth1_opt;
    ip_conf = &eng_conf->flow_conf.flow_conf.ptp.ip1_opt;

    if (!(eng_conf->eng_used)) {
        VTSS_E("Port-no: %d Engine :%d not configured\n", port_no, eng_id);
        return VTSS_RC_ERROR;
    }

    for (i = 0; i < 7; i++) {
        value = 0;
        VTSS_RC(VTSS_PHY_TS_READ_CSR(cfgport, blk_id, VTSS_ANA_ETH1_FLOW_CFG_ETH1_FLOW_ENABLE(i), &value));
        vtss_state->chmap[ing].eth1_chmap[i] = value;
        VTSS_I("Port-no: %d  blk_id: 0x%x,  Saving: VTSS_ANA_ETH1_FLOW_CFG_ETH1_FLOW_ENABLE(%d):: 0x%x :: 0x%x\n",
               port_no, blk_id, i, VTSS_ANA_ETH1_FLOW_CFG_ETH1_FLOW_ENABLE(i), value);
        value = VTSS_PHY_TS_CLR_BITS(value, (channel << 8));
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(cfgport, blk_id, VTSS_ANA_ETH1_FLOW_CFG_ETH1_FLOW_ENABLE(i), &value));
    }

    /* Eth Flow 7 Enable, common for all encapsulation types */
    value = 0;
    VTSS_RC(VTSS_PHY_TS_READ_CSR(cfgport, blk_id, VTSS_ANA_ETH1_FLOW_CFG_ETH1_FLOW_ENABLE(flow_id), &value));
    value = VTSS_PHY_TS_CLR_BITS(value, VTSS_M_ANA_ETH1_FLOW_CFG_ETH1_FLOW_ENABLE_ETH1_CHANNEL_MASK);
    value |= VTSS_F_ANA_ETH1_FLOW_CFG_ETH1_FLOW_ENABLE_ETH1_CHANNEL_MASK(channel);
    value |=  VTSS_F_ANA_ETH1_FLOW_CFG_ETH1_FLOW_ENABLE_ETH1_FLOW_ENABLE;
    VTSS_RC(VTSS_PHY_TS_WRITE_CSR(cfgport, blk_id, VTSS_ANA_ETH1_FLOW_CFG_ETH1_FLOW_ENABLE(flow_id), &value));

    /* MATCH MODE */
    value = 0;
    VTSS_RC(VTSS_PHY_TS_WRITE_CSR(cfgport, blk_id, VTSS_ANA_ETH1_FLOW_CFG_ETH1_MATCH_MODE(flow_id), &value));

    /* Match Address, Match any Multicast Address, either at source or dest*/
    value = 0xFFFFFFFF;
    VTSS_RC(VTSS_PHY_TS_WRITE_CSR(cfgport, blk_id, VTSS_ANA_ETH1_FLOW_CFG_ETH1_ADDR_MATCH_1(flow_id), &value));

    value = VTSS_F_ANA_ETH1_FLOW_CFG_ETH1_ADDR_MATCH_2_ETH1_ADDR_MATCH_MODE(VTSS_PHY_TS_ETH_ADDR_MATCH_48BIT);
    if (ing) {
        value |= VTSS_F_ANA_ETH1_FLOW_CFG_ETH1_ADDR_MATCH_2_ETH1_ADDR_MATCH_SELECT(VTSS_PHY_TS_ETH_MATCH_DEST_ADDR) |
                 VTSS_F_ANA_ETH1_FLOW_CFG_ETH1_ADDR_MATCH_2_ETH1_ADDR_MATCH_2(0xFFF1);
    } else {
        value |= VTSS_F_ANA_ETH1_FLOW_CFG_ETH1_ADDR_MATCH_2_ETH1_ADDR_MATCH_SELECT(VTSS_PHY_TS_ETH_MATCH_SRC_ADDR) |
                 VTSS_F_ANA_ETH1_FLOW_CFG_ETH1_ADDR_MATCH_2_ETH1_ADDR_MATCH_2(0xFFF0);
    }

    VTSS_RC(VTSS_PHY_TS_WRITE_CSR(cfgport, blk_id, VTSS_ANA_ETH1_FLOW_CFG_ETH1_ADDR_MATCH_2(flow_id), &value));

    value = VTSS_F_ANA_ETH1_FLOW_CFG_ETH1_VLAN_TAG_RANGE_I_TAG_ETH1_VLAN_TAG_RANGE_UPPER(0);
    VTSS_RC(VTSS_PHY_TS_WRITE_CSR(cfgport, blk_id, VTSS_ANA_ETH1_FLOW_CFG_ETH1_VLAN_TAG_RANGE_I_TAG(flow_id), &value));
    value = VTSS_F_ANA_ETH1_FLOW_CFG_ETH1_VLAN_TAG1_ETH1_VLAN_TAG1_MASK(0);
    VTSS_RC(VTSS_PHY_TS_WRITE_CSR(cfgport, blk_id, VTSS_ANA_ETH1_FLOW_CFG_ETH1_VLAN_TAG1(flow_id), &value));
    value = VTSS_F_ANA_ETH1_FLOW_CFG_ETH1_VLAN_TAG2_I_TAG_ETH1_VLAN_TAG2_MASK(0);
    VTSS_RC(VTSS_PHY_TS_WRITE_CSR(cfgport, blk_id, VTSS_ANA_ETH1_FLOW_CFG_ETH1_VLAN_TAG2_I_TAG(flow_id), &value));


#undef PLAN_E_CHG
#ifdef PLAN_E_CHG
    /* sriharsha */
    /* Configure next comparator as PTP*/
    VTSS_RC(VTSS_PHY_TS_READ_CSR(cfgport, blk_id, VTSS_ANA_ETH1_NXT_PROTOCOL_ETH1_NXT_PROTOCOL, &value));
    temp = VTSS_F_ANA_ETH1_NXT_PROTOCOL_ETH1_NXT_PROTOCOL_ETH1_NXT_COMPARATOR(5);
    value = VTSS_PHY_TS_CLR_BITS(value, VTSS_M_ANA_ETH1_NXT_PROTOCOL_ETH1_NXT_PROTOCOL_ETH1_NXT_COMPARATOR) | temp;
    VTSS_RC(VTSS_PHY_TS_WRITE_CSR(cfgport, blk_id, VTSS_ANA_ETH1_NXT_PROTOCOL_ETH1_NXT_PROTOCOL, &value));
#endif

    switch (eng_conf->encap_type) {
    case VTSS_PHY_TS_ENCAP_ETH_PTP:
        /* configured */
        break;
    case VTSS_PHY_TS_ENCAP_ETH_IP_PTP:
#if 0
        if (ip_conf->flow_opt[flow_id].flow_en) {
            VTSS_E("Port-no: %d IP Flow :%d already enabled \n", port_no, flow_id);
            rc = VTSS_RC_ERROR;
            break;
        }
#endif
        for (i = 0; i < 7; i++) {
            value = 0;
            VTSS_RC(VTSS_PHY_TS_READ_CSR(cfgport, blk_id, VTSS_ANA_IP1_FLOW_CFG_IP1_FLOW_ENA(i), &value));
            vtss_state->chmap[ing].ip1_chmap[i] = value;
            VTSS_I("Port-no: %d  blk_id: 0x%x,  Saving: VTSS_ANA_IP1_FLOW_CFG_IP1_FLOW_ENABLE(%d):: 0x%x :: 0x%x\n",
                   port_no, blk_id, i, VTSS_ANA_IP1_FLOW_CFG_IP1_FLOW_ENA(i), value);
#ifdef PLAN_E_CHG // ETH_PTP
            /* sriharsha */
            value = 0;
#else // ETH_IP_PTP
            value = VTSS_PHY_TS_CLR_BITS(value, (channel << 4));
#endif

            VTSS_RC(VTSS_PHY_TS_WRITE_CSR(cfgport, blk_id, VTSS_ANA_IP1_FLOW_CFG_IP1_FLOW_ENA(i), &value));
        }

        /* Change the Common conf for protocol, source port and dest part */
        if (ip_conf->comm_opt.ip_mode == VTSS_PHY_TS_IP_VER_4) {
            value = 0;
            VTSS_RC(VTSS_PHY_TS_WRITE_CSR(cfgport, blk_id, VTSS_ANA_IP1_FLOW_CFG_IP1_FLOW_MATCH_UPPER(flow_id), &value));
            value = 0;
            VTSS_RC(VTSS_PHY_TS_WRITE_CSR(cfgport, blk_id, VTSS_ANA_IP1_FLOW_CFG_IP1_FLOW_MASK_UPPER(flow_id), &value));
            /* clear the other mask register */
            value = 0;
            VTSS_RC(VTSS_PHY_TS_WRITE_CSR(cfgport, blk_id, VTSS_ANA_IP1_FLOW_CFG_IP1_FLOW_MASK_UPPER_MID(flow_id), &value));
            VTSS_RC(VTSS_PHY_TS_WRITE_CSR(cfgport, blk_id, VTSS_ANA_IP1_FLOW_CFG_IP1_FLOW_MASK_LOWER_MID(flow_id), &value));
            VTSS_RC(VTSS_PHY_TS_WRITE_CSR(cfgport, blk_id, VTSS_ANA_IP1_FLOW_CFG_IP1_FLOW_MASK_LOWER(flow_id), &value));
        } else {
            value = 0;
            VTSS_RC(VTSS_PHY_TS_WRITE_CSR(cfgport, blk_id, VTSS_ANA_IP1_FLOW_CFG_IP1_FLOW_MATCH_UPPER(flow_id), &value));
            value = 0;
            VTSS_RC(VTSS_PHY_TS_WRITE_CSR(cfgport, blk_id, VTSS_ANA_IP1_FLOW_CFG_IP1_FLOW_MASK_UPPER(flow_id), &value));
            /* clear the other mask register */
            value = 0;
            VTSS_RC(VTSS_PHY_TS_WRITE_CSR(cfgport, blk_id, VTSS_ANA_IP1_FLOW_CFG_IP1_FLOW_MASK_UPPER_MID(flow_id), &value));
            VTSS_RC(VTSS_PHY_TS_WRITE_CSR(cfgport, blk_id, VTSS_ANA_IP1_FLOW_CFG_IP1_FLOW_MASK_LOWER_MID(flow_id), &value));
            VTSS_RC(VTSS_PHY_TS_WRITE_CSR(cfgport, blk_id, VTSS_ANA_IP1_FLOW_CFG_IP1_FLOW_MASK_LOWER(flow_id), &value));
        }
        /* ********* sriharsha ****** */
#ifdef PLAN_E_CHG // ETH_PTP
        /* For PLAN_E_CHG ... Nothing to do */
#else   // ETH_IP_PTP

        VTSS_RC(VTSS_PHY_TS_READ_CSR(cfgport, blk_id, VTSS_ANA_IP1_FLOW_CFG_IP1_FLOW_ENA(flow_id), &value));
        temp = VTSS_F_ANA_IP1_FLOW_CFG_IP1_FLOW_ENA_IP1_CHANNEL_MASK(channel);
        value = VTSS_PHY_TS_CLR_BITS(value, VTSS_M_ANA_IP1_FLOW_CFG_IP1_FLOW_ENA_IP1_CHANNEL_MASK) | temp;
        value |=  VTSS_F_ANA_IP1_FLOW_CFG_IP1_FLOW_ENA_IP1_FLOW_ENA;

        temp = VTSS_F_ANA_IP1_FLOW_CFG_IP1_FLOW_ENA_IP1_FLOW_MATCH_MODE(VTSS_PHY_TS_IP_MATCH_SRC_OR_DEST);
        value = VTSS_PHY_TS_CLR_BITS(value, VTSS_M_ANA_IP1_FLOW_CFG_IP1_FLOW_ENA_IP1_FLOW_MATCH_MODE) | temp;
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(cfgport, blk_id, VTSS_ANA_IP1_FLOW_CFG_IP1_FLOW_ENA(flow_id), &value));
#endif
        break;

    case VTSS_PHY_TS_ENCAP_ETH_IP_IP_PTP:
    case VTSS_PHY_TS_ENCAP_ETH_ETH_PTP:
    case VTSS_PHY_TS_ENCAP_ETH_ETH_IP_PTP:
    case VTSS_PHY_TS_ENCAP_ETH_MPLS_IP_PTP:
    case VTSS_PHY_TS_ENCAP_ETH_MPLS_ETH_PTP:
    case VTSS_PHY_TS_ENCAP_ETH_MPLS_ETH_IP_PTP:
    case VTSS_PHY_TS_ENCAP_ETH_MPLS_ACH_PTP:
    case VTSS_PHY_TS_ENCAP_ETH_OAM:
    case VTSS_PHY_TS_ENCAP_ETH_ETH_OAM:
    case VTSS_PHY_TS_ENCAP_ETH_MPLS_ETH_OAM:
    case VTSS_PHY_TS_ENCAP_ETH_MPLS_ACH_OAM:
        VTSS_E("Port %d:: Encap type not supported", port_no);
        rc = VTSS_RC_ERROR;
        break;
    default:
        VTSS_E("Port %d:: Invalid encap type", port_no);
        rc = VTSS_RC_ERROR;
        break;
    }

    return rc;
}
static vtss_rc vtss_phy_ts_tesla_ingress_tsp_fifo_sync_ing_act_add(vtss_state_t *vtss_state, vtss_port_no_t  port_no,
                                                                   vtss_phy_ts_engine_t eng_id,
                                                                   vtss_phy_ts_engine_channel_map_t channel)
{

    vtss_rc                         rc = VTSS_RC_OK;
    vtss_port_no_t                  base_port_no, cfgport;
    vtss_phy_ts_blk_id_t            blk_id = VTSS_PHY_TS_ANA_BLK_ID_ING_0;
    u8                              flow_id = 5;
    u32                             value = 0;
#ifdef PLAN_E_CHG // ETH_PTP
    u8                              cf_offset = 20;
#else
    u8                              rx_ts_pos = 22;
#endif
    vtss_phy_ts_fifo_sig_mask_t     sig_mask;
    u32                             i;

    if ((rc = vtss_phy_ts_base_port_get_priv(vtss_state, port_no, &base_port_no)) != VTSS_RC_OK) {
        VTSS_E("Port %d:: Base Port get failed", port_no);
        return VTSS_RC_ERROR;
    }

    VTSS_I("Ing_Action_Add: Port-no: %d Base Port No: %d Channel Mask: %d  \n", port_no, base_port_no, channel);
    sig_mask = vtss_state->phy_ts_port_conf[base_port_no].sig_mask;
    cfgport = port_no;
    blk_id = (eng_id == VTSS_PHY_TS_PTP_ENGINE_ID_0) ? VTSS_PHY_TS_ANA_BLK_ID_ING_0 : VTSS_PHY_TS_ANA_BLK_ID_ING_1;

    /* sriharsha */
#ifdef PLAN_E_CHG // ETH_PTP
    if ((sig_mask & VTSS_PHY_TS_FIFO_SIG_SEQ_ID) && (sig_mask & VTSS_PHY_TS_FIFO_SIG_SOURCE_PORT_ID)) {
        cf_offset = 20;
        VTSS_I("Port %d:: Signature OFFSET: %d ", port_no, cf_offset);
    } else {
        VTSS_E("Port %d:: ERROR !! Recovery  method need to add for other signature types\n", port_no);
        return VTSS_RC_ERROR;
    }
#else   // ETH_IP_PTP
    if (sig_mask & VTSS_PHY_TS_FIFO_SIG_SEQ_ID) {
        rx_ts_pos = 22;
        VTSS_I("Port %d:: Signature OFFSET: %d ", port_no, rx_ts_pos);
    } else {
        VTSS_E("Port %d:: ERROR!! Recovery  method need to add for other signature types\n", port_no);
        return VTSS_RC_ERROR;
    }
#endif
    for (i = 0; i < 5; i++) {
        value = 0;
        VTSS_RC(VTSS_PHY_TS_READ_CSR(cfgport, blk_id, VTSS_ANA_PTP_FLOW_PTP_FLOW_ENA(i), &value));
        vtss_state->chmap[1].ptp_chmap[i] = value;
        VTSS_I("Port-no: %d  blk_id: 0x%x,  Saving VTSS_ANA_PTP_FLOW_PTP_FLOW_ENA(%d):: 0x%x :: 0x%x\n",
               port_no, blk_id, i, VTSS_ANA_PTP_FLOW_PTP_FLOW_ENA(i), value);
        value = VTSS_PHY_TS_CLR_BITS(value, (channel << 4));
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(cfgport, blk_id, VTSS_ANA_PTP_FLOW_PTP_FLOW_ENA(i), &value));
    }
    value = VTSS_F_ANA_PTP_FLOW_PTP_FLOW_ENA_PTP_FLOW_ENA;
    value |= VTSS_F_ANA_PTP_FLOW_PTP_FLOW_ENA_PTP_CHANNEL_MASK(channel);
    VTSS_RC(VTSS_PHY_TS_WRITE_CSR(cfgport, blk_id, VTSS_ANA_PTP_FLOW_PTP_FLOW_ENA(flow_id), &value));
    value = 0;
    VTSS_RC(VTSS_PHY_TS_WRITE_CSR(cfgport, blk_id, VTSS_ANA_PTP_FLOW_PTP_DOMAIN_RANGE(flow_id), &value));
    value = 0;
    VTSS_RC(VTSS_PHY_TS_WRITE_CSR(cfgport, blk_id, VTSS_ANA_PTP_FLOW_PTP_FLOW_MASK_LOWER(flow_id), &value));
    VTSS_RC(VTSS_PHY_TS_WRITE_CSR(cfgport, blk_id, VTSS_ANA_PTP_FLOW_PTP_FLOW_MASK_UPPER(flow_id), &value));
    VTSS_RC(VTSS_PHY_TS_WRITE_CSR(cfgport, blk_id, VTSS_ANA_PTP_FLOW_PTP_FLOW_MATCH_LOWER(flow_id), &value));
    VTSS_RC(VTSS_PHY_TS_WRITE_CSR(cfgport, blk_id, VTSS_ANA_PTP_FLOW_PTP_FLOW_MATCH_UPPER(flow_id), &value));

#ifdef PLAN_E_CHG // ETH_PTP
    /* Action Add */
    value = 0;
    value = VTSS_F_ANA_PTP_FLOW_PTP_ACTION_PTP_TIME_STRG_FIELD_OFFSET(8);
    value |= VTSS_F_ANA_PTP_FLOW_PTP_ACTION_PTP_COMMAND(PTP_ACTION_CMD_WRITE_NS); /* write nano sec */
    value |= VTSS_F_ANA_PTP_FLOW_PTP_ACTION_PTP_SAVE_LOCAL_TIME; /* save in FIFO */
    value |= VTSS_F_ANA_PTP_FLOW_PTP_ACTION_PTP_CORR_FIELD_OFFSET(cf_offset);
    VTSS_RC(VTSS_PHY_TS_WRITE_CSR(cfgport, blk_id, VTSS_ANA_PTP_FLOW_PTP_ACTION(flow_id), &value));

    /* action_2 setting */
    value = 0;
    value = VTSS_F_ANA_PTP_FLOW_PTP_ACTION_2_PTP_REWRITE_OFFSET(cf_offset + 8); /*  */
    value |= VTSS_F_ANA_PTP_FLOW_PTP_ACTION_2_PTP_REWRITE_BYTES(4); /* nano sec timestamp */
    value |= VTSS_F_ANA_PTP_FLOW_PTP_ACTION_2_PTP_NEW_CF_LOC(cf_offset); /* correction field offset */
    VTSS_RC(VTSS_PHY_TS_WRITE_CSR(cfgport, blk_id, VTSS_ANA_PTP_FLOW_PTP_ACTION_2(flow_id), &value));

#else   // ETH_IP_PTP
    /* Action Add */
    value = 0;
    value = VTSS_F_ANA_PTP_FLOW_PTP_ACTION_PTP_TIME_STRG_FIELD_OFFSET(8);
    value |= VTSS_F_ANA_PTP_FLOW_PTP_ACTION_PTP_COMMAND(PTP_ACTION_CMD_WRITE_1588); /* write_1588 */
    value |= VTSS_F_ANA_PTP_FLOW_PTP_ACTION_PTP_SAVE_LOCAL_TIME; /* save in FIFO */
    value |= VTSS_F_ANA_PTP_FLOW_PTP_ACTION_PTP_CORR_FIELD_OFFSET(8);
    VTSS_RC(VTSS_PHY_TS_WRITE_CSR(cfgport, blk_id, VTSS_ANA_PTP_FLOW_PTP_ACTION(flow_id), &value));

    /* action_2 setting */
    value = 0;
    value = VTSS_F_ANA_PTP_FLOW_PTP_ACTION_2_PTP_REWRITE_OFFSET(rx_ts_pos); /* TODO, origintimestamp offset */
    value |= VTSS_F_ANA_PTP_FLOW_PTP_ACTION_2_PTP_REWRITE_BYTES(10); /* full timestamp */
    value |= VTSS_F_ANA_PTP_FLOW_PTP_ACTION_2_PTP_NEW_CF_LOC(0); /* correction field offset */
    VTSS_RC(VTSS_PHY_TS_WRITE_CSR(cfgport, blk_id, VTSS_ANA_PTP_FLOW_PTP_ACTION_2(flow_id), &value));
#endif

#undef _MADHU_DEBUG_
#ifdef _MADHU_DEBUG_
    /* Debug code until final patch */
    VTSS_I("Ingress Registers Dump port:%d\n", cfgport);

    vtss_phy_ts_read_ing_ana_reg_priv(port_no, cfgport, 0);
    vtss_phy_ts_read_proc_reg_priv(port_no, cfgport, 6);
    vtss_phy_ts_read_proc_reg_priv(port_no, cfgport, 7);
#endif
    return rc;

}

static vtss_rc vtss_phy_ts_tesla_ingress_tsp_fifo_sync_egr_act_add(vtss_state_t *vtss_state, vtss_port_no_t   port_no,
                                                                   vtss_phy_ts_engine_t eng_id,
                                                                   vtss_phy_ts_engine_channel_map_t channel
                                                                  )
{

    vtss_rc                         rc = VTSS_RC_OK;
    u8                              flow_id = 5;
    u32                             value;
    vtss_port_no_t                  base_port_no, cfgport;
    vtss_phy_ts_blk_id_t            blk_id = VTSS_PHY_TS_ANA_BLK_ID_EGR_0;
    u32                             i;
#ifdef PLAN_E_CHG // ETH_PTP
    u8                              cf_offset = 20;
#endif

    if ((rc = vtss_phy_ts_base_port_get_priv(vtss_state, port_no, &base_port_no)) != VTSS_RC_OK) {
        VTSS_E("Port %d:: Base Port get failed", port_no);
        return VTSS_RC_ERROR;
    }

    VTSS_I("Egr_Action_Add: Port-no: %d Base Port No: %d Channel Mask: %d  \n", port_no, base_port_no, channel);

    cfgport = port_no;
    blk_id = (eng_id == VTSS_PHY_TS_PTP_ENGINE_ID_0) ? VTSS_PHY_TS_ANA_BLK_ID_EGR_0 : VTSS_PHY_TS_ANA_BLK_ID_EGR_1;

    for (i = 0; i < 5; i++) {
        value = 0;
        VTSS_RC(VTSS_PHY_TS_READ_CSR(cfgport, blk_id, VTSS_ANA_PTP_FLOW_PTP_FLOW_ENA(i), &value));
        vtss_state->chmap[0].ptp_chmap[i] = value;
        VTSS_I("Port-no: %d  blk_id: 0x%x,  Saving VTSS_ANA_PTP_FLOW_PTP_FLOW_ENA(%d):: 0x%x :: 0x%x\n",
               port_no, blk_id, i, VTSS_ANA_PTP_FLOW_PTP_FLOW_ENA(i), value);
        value = VTSS_PHY_TS_CLR_BITS(value, (channel << 4));
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(cfgport, blk_id, VTSS_ANA_PTP_FLOW_PTP_FLOW_ENA(i), &value));
    }

    /* Flow Enable */
    value = VTSS_F_ANA_PTP_FLOW_PTP_FLOW_ENA_PTP_FLOW_ENA;
    value |= VTSS_F_ANA_PTP_FLOW_PTP_FLOW_ENA_PTP_CHANNEL_MASK(channel);
    VTSS_RC(VTSS_PHY_TS_WRITE_CSR(cfgport, blk_id, VTSS_ANA_PTP_FLOW_PTP_FLOW_ENA(flow_id), &value));

    value = 0;
    VTSS_RC(VTSS_PHY_TS_WRITE_CSR(cfgport, blk_id, VTSS_ANA_PTP_FLOW_PTP_DOMAIN_RANGE(flow_id), &value));

    value = 0;
    VTSS_RC(VTSS_PHY_TS_WRITE_CSR(cfgport, blk_id, VTSS_ANA_PTP_FLOW_PTP_FLOW_MASK_LOWER(flow_id), &value));
    VTSS_RC(VTSS_PHY_TS_WRITE_CSR(cfgport, blk_id, VTSS_ANA_PTP_FLOW_PTP_FLOW_MASK_UPPER(flow_id), &value));
    VTSS_RC(VTSS_PHY_TS_WRITE_CSR(cfgport, blk_id, VTSS_ANA_PTP_FLOW_PTP_FLOW_MATCH_LOWER(flow_id), &value));
    VTSS_RC(VTSS_PHY_TS_WRITE_CSR(cfgport, blk_id, VTSS_ANA_PTP_FLOW_PTP_FLOW_MATCH_UPPER(flow_id), &value));

#ifdef PLAN_E_CHG // ETH_PTP
    /* Action Add */
    value = 0;
    value = VTSS_F_ANA_PTP_FLOW_PTP_ACTION_PTP_TIME_STRG_FIELD_OFFSET(8);
    value |= VTSS_F_ANA_PTP_FLOW_PTP_ACTION_PTP_COMMAND(PTP_ACTION_CMD_WRITE_NS); /* write nano sec */
    value |= VTSS_F_ANA_PTP_FLOW_PTP_ACTION_PTP_SAVE_LOCAL_TIME; /* save in FIFO */
    value |= VTSS_F_ANA_PTP_FLOW_PTP_ACTION_PTP_CORR_FIELD_OFFSET(cf_offset);
    VTSS_RC(VTSS_PHY_TS_WRITE_CSR(cfgport, blk_id, VTSS_ANA_PTP_FLOW_PTP_ACTION(flow_id), &value));

    /* action_2 setting */
    value = 0;
    value = VTSS_F_ANA_PTP_FLOW_PTP_ACTION_2_PTP_REWRITE_OFFSET(cf_offset + 8); /*  */
    value |= VTSS_F_ANA_PTP_FLOW_PTP_ACTION_2_PTP_REWRITE_BYTES(4); /* nano sec timestamp */
    value |= VTSS_F_ANA_PTP_FLOW_PTP_ACTION_2_PTP_NEW_CF_LOC(cf_offset); /* correction field offset */
    VTSS_RC(VTSS_PHY_TS_WRITE_CSR(cfgport, blk_id, VTSS_ANA_PTP_FLOW_PTP_ACTION_2(flow_id), &value));

#else
    /* Action Add */
    value = 0;
    value = VTSS_F_ANA_PTP_FLOW_PTP_ACTION_PTP_TIME_STRG_FIELD_OFFSET(8);
    value |= VTSS_F_ANA_PTP_FLOW_PTP_ACTION_PTP_COMMAND(PTP_ACTION_CMD_WRITE_1588); /* write_1588 */
    value |= VTSS_F_ANA_PTP_FLOW_PTP_ACTION_PTP_SAVE_LOCAL_TIME; /* save in FIFO */
    value |= VTSS_F_ANA_PTP_FLOW_PTP_ACTION_PTP_CORR_FIELD_OFFSET(8);
    VTSS_RC(VTSS_PHY_TS_WRITE_CSR(cfgport, blk_id, VTSS_ANA_PTP_FLOW_PTP_ACTION(flow_id), &value));

    /* action_2 setting */
    value = 0;
    value = VTSS_F_ANA_PTP_FLOW_PTP_ACTION_2_PTP_REWRITE_OFFSET(34); /* origintimestamp offset */
    value |= VTSS_F_ANA_PTP_FLOW_PTP_ACTION_2_PTP_REWRITE_BYTES(10); /* full timestamp */
    value |= VTSS_F_ANA_PTP_FLOW_PTP_ACTION_2_PTP_NEW_CF_LOC(0); /* correction field offset */
    VTSS_RC(VTSS_PHY_TS_WRITE_CSR(cfgport, blk_id, VTSS_ANA_PTP_FLOW_PTP_ACTION_2(flow_id), &value));
#endif

#if 0

    VTSS_I("Ingress Registers Dump port:%d Engine0\n", cfgport);
    vtss_phy_ts_read_ing_ana_reg_priv(vtss_state, port_no, cfgport, 0);
    VTSS_I("Egress Registers Dump port:%d Engine0\n", cfgport);
    vtss_phy_ts_read_egr_ana_reg_priv(vtss_state, port_no, cfgport, 1);
    VTSS_I("Ingress Registers Dump port:%d Engine1\n", cfgport);
    vtss_phy_ts_read_ing_ana_reg_priv(vtss_state, port_no, cfgport, 2);
    VTSS_I("Egress Registers Dump port:%d Engine1\n", cfgport);
    vtss_phy_ts_read_egr_ana_reg_priv(vtss_state, port_no, cfgport, 3);
    VTSS_I("Proc registers  Dump port:%d\n", cfgport);
    vtss_phy_ts_read_proc_reg_priv(vtss_state, port_no, cfgport, 6);
    vtss_phy_ts_read_proc_reg_priv(vtss_state, port_no, cfgport, 7);
#endif

    return rc;
}


static vtss_rc vtss_phy_ts_tesla_ingress_tsp_fifo_sync_restore_analyzer_config(vtss_state_t *vtss_state, vtss_port_no_t  port_no,
                                                                               vtss_phy_ts_engine_t eng_id,
                                                                               vtss_phy_ts_engine_channel_map_t channel)
{
    vtss_rc                         rc = VTSS_RC_OK;
    vtss_port_no_t                  base_port_no, cfgport;
    vtss_phy_ts_blk_id_t            ing_blk_id = VTSS_PHY_TS_ANA_BLK_ID_ING_0;
    vtss_phy_ts_blk_id_t            egr_blk_id = VTSS_PHY_TS_ANA_BLK_ID_EGR_0;
    u8                              flow_id = 7;
    u32                             value, i;
#ifdef PLAN_E_CHG // ETH_PTP
    u32                             temp = 0;
#endif


    vtss_phy_ts_eng_conf_t          *ing_eng_conf, *egr_eng_conf;

    if ((rc = vtss_phy_ts_base_port_get_priv(vtss_state, port_no, &base_port_no)) != VTSS_RC_OK) {
        VTSS_E("Port %d:: Base Port get failed\n", port_no);
        return VTSS_RC_ERROR;
    }
    cfgport = base_port_no;

    egr_blk_id = (eng_id == VTSS_PHY_TS_PTP_ENGINE_ID_0) ? VTSS_PHY_TS_ANA_BLK_ID_EGR_0 : VTSS_PHY_TS_ANA_BLK_ID_EGR_1;
    ing_blk_id = (eng_id == VTSS_PHY_TS_PTP_ENGINE_ID_0) ? VTSS_PHY_TS_ANA_BLK_ID_ING_0 : VTSS_PHY_TS_ANA_BLK_ID_ING_1;

    ing_eng_conf = &vtss_state->phy_ts_port_conf[base_port_no].ingress_eng_conf[eng_id];
    egr_eng_conf = &vtss_state->phy_ts_port_conf[base_port_no].egress_eng_conf[eng_id];

    VTSS_I("Restore_Analyzer_Config Port-no: %d Base Port No: %d Channel Mask: %d  \n", port_no, base_port_no, channel);

    //VTSS_PHY_TS_SPI_PAUSE(port_no);
#ifdef PLAN_E_CHG // ETH_PTP
    /* sriharsha */
    /* On ingress Configure next comparator as IP */
    VTSS_RC(VTSS_PHY_TS_READ_CSR(cfgport, ing_blk_id, VTSS_ANA_ETH1_NXT_PROTOCOL_ETH1_NXT_PROTOCOL, &value));
    temp = VTSS_F_ANA_ETH1_NXT_PROTOCOL_ETH1_NXT_PROTOCOL_ETH1_NXT_COMPARATOR(2);
    value = VTSS_PHY_TS_CLR_BITS(value, VTSS_M_ANA_ETH1_NXT_PROTOCOL_ETH1_NXT_PROTOCOL_ETH1_NXT_COMPARATOR) | temp;
    VTSS_RC(VTSS_PHY_TS_WRITE_CSR(cfgport, ing_blk_id, VTSS_ANA_ETH1_NXT_PROTOCOL_ETH1_NXT_PROTOCOL, &value));

    /* On Egress Configure next comparator as IP */
    VTSS_RC(VTSS_PHY_TS_READ_CSR(cfgport, egr_blk_id, VTSS_ANA_ETH1_NXT_PROTOCOL_ETH1_NXT_PROTOCOL, &value));
    temp = VTSS_F_ANA_ETH1_NXT_PROTOCOL_ETH1_NXT_PROTOCOL_ETH1_NXT_COMPARATOR(2);
    value = VTSS_PHY_TS_CLR_BITS(value, VTSS_M_ANA_ETH1_NXT_PROTOCOL_ETH1_NXT_PROTOCOL_ETH1_NXT_COMPARATOR) | temp;
    VTSS_RC(VTSS_PHY_TS_WRITE_CSR(cfgport, egr_blk_id, VTSS_ANA_ETH1_NXT_PROTOCOL_ETH1_NXT_PROTOCOL, &value));
#endif

    for (i = 0; i < flow_id; i++) {
        value = vtss_state->chmap[1].eth1_chmap[i];
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(cfgport, ing_blk_id, VTSS_ANA_ETH1_FLOW_CFG_ETH1_FLOW_ENABLE(i), &value));
        VTSS_I("Port-no: %d  ing_blk_id: 0x%x, Restoring: VTSS_ANA_ETH1_FLOW_CFG_ETH1_FLOW_ENABLE(%d):: 0x%x :: 0x%x\n",
               port_no, ing_blk_id, i, VTSS_ANA_ETH1_FLOW_CFG_ETH1_FLOW_ENABLE(i), value);
    }
    switch (ing_eng_conf->encap_type) {
    case VTSS_PHY_TS_ENCAP_ETH_PTP:
        /* configured */
        break;
    case VTSS_PHY_TS_ENCAP_ETH_IP_PTP:
        for (i = 0; i < flow_id; i++) {
            value = vtss_state->chmap[1].ip1_chmap[i];
            VTSS_RC(VTSS_PHY_TS_WRITE_CSR(cfgport, ing_blk_id, VTSS_ANA_IP1_FLOW_CFG_IP1_FLOW_ENA(i), &value));
            VTSS_I("Port-no: %d  ing_blk_id: 0x%x, Restoring: VTSS_ANA_IP1_FLOW_CFG_IP1_FLOW_ENA(%d):: 0x%x :: 0x%x\n",
                   port_no, ing_blk_id, i, VTSS_ANA_IP1_FLOW_CFG_IP1_FLOW_ENA(i), value);
        }
        break;

    case VTSS_PHY_TS_ENCAP_ETH_IP_IP_PTP:
    case VTSS_PHY_TS_ENCAP_ETH_ETH_PTP:
    case VTSS_PHY_TS_ENCAP_ETH_ETH_IP_PTP:
    case VTSS_PHY_TS_ENCAP_ETH_MPLS_IP_PTP:
    case VTSS_PHY_TS_ENCAP_ETH_MPLS_ETH_PTP:
    case VTSS_PHY_TS_ENCAP_ETH_MPLS_ETH_IP_PTP:
    case VTSS_PHY_TS_ENCAP_ETH_MPLS_ACH_PTP:
    case VTSS_PHY_TS_ENCAP_ETH_OAM:
    case VTSS_PHY_TS_ENCAP_ETH_ETH_OAM:
    case VTSS_PHY_TS_ENCAP_ETH_MPLS_ETH_OAM:
    case VTSS_PHY_TS_ENCAP_ETH_MPLS_ACH_OAM:
        VTSS_E("Port %d:: Encap type not supported", port_no);
        rc = VTSS_RC_ERROR;
        break;
    default:
        VTSS_E("Port %d:: Invalid encap type", port_no);
        rc = VTSS_RC_ERROR;
        break;
    }

    for (i = 0; i < 5; i++) {
        value = vtss_state->chmap[1].ptp_chmap[i];
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(cfgport, ing_blk_id, VTSS_ANA_PTP_FLOW_PTP_FLOW_ENA(i), &value));
        VTSS_I("Port-no: %d  Ing_Blk_id: 0x%x, Restoring: VTSS_ANA_PTP_FLOW_PTP_FLOW_ENA(%d):: 0x%x :: 0x%x\n",
               port_no, ing_blk_id, i, VTSS_ANA_PTP_FLOW_PTP_FLOW_ENA(i), value);
    }

    /* Egress */

    for (i = 0; i < flow_id; i++) {
        value = vtss_state->chmap[0].eth1_chmap[i];
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(cfgport, egr_blk_id, VTSS_ANA_ETH1_FLOW_CFG_ETH1_FLOW_ENABLE(i), &value));
        VTSS_I("Port-no: %d  Egr_Blk_id: 0x%x, Restoring: VTSS_ANA_ETH1_FLOW_CFG_ETH1_FLOW_ENABLE(%d):: 0x%x :: 0x%x\n",
               port_no, egr_blk_id, i, VTSS_ANA_ETH1_FLOW_CFG_ETH1_FLOW_ENABLE(i), value);
    }

    switch (egr_eng_conf->encap_type) {
    case VTSS_PHY_TS_ENCAP_ETH_PTP:
        /* configured */
        break;
    case VTSS_PHY_TS_ENCAP_ETH_IP_PTP:
        for (i = 0; i < flow_id; i++) {
            value = vtss_state->chmap[0].ip1_chmap[i];
            VTSS_RC(VTSS_PHY_TS_WRITE_CSR(cfgport, egr_blk_id, VTSS_ANA_IP1_FLOW_CFG_IP1_FLOW_ENA(i), &value));
            VTSS_I("Port-no: %d  Egr_Blk_id: 0x%x,  Restoring: VTSS_ANA_IP1_FLOW_CFG_IP1_FLOW_ENA(%d):: 0x%x :: 0x%x\n",
                   cfgport, egr_blk_id, i, VTSS_ANA_IP1_FLOW_CFG_IP1_FLOW_ENA(i), value);
        }

        break;

    case VTSS_PHY_TS_ENCAP_ETH_IP_IP_PTP:
    case VTSS_PHY_TS_ENCAP_ETH_ETH_PTP:
    case VTSS_PHY_TS_ENCAP_ETH_ETH_IP_PTP:
    case VTSS_PHY_TS_ENCAP_ETH_MPLS_IP_PTP:
    case VTSS_PHY_TS_ENCAP_ETH_MPLS_ETH_PTP:
    case VTSS_PHY_TS_ENCAP_ETH_MPLS_ETH_IP_PTP:
    case VTSS_PHY_TS_ENCAP_ETH_MPLS_ACH_PTP:
    case VTSS_PHY_TS_ENCAP_ETH_OAM:
    case VTSS_PHY_TS_ENCAP_ETH_ETH_OAM:
    case VTSS_PHY_TS_ENCAP_ETH_MPLS_ETH_OAM:
    case VTSS_PHY_TS_ENCAP_ETH_MPLS_ACH_OAM:
        VTSS_E("Port %d:: Encap type not supported", port_no);
        rc = VTSS_RC_ERROR;
        break;
    default:
        VTSS_E("Port %d:: Invalid encap type", port_no);
        rc = VTSS_RC_ERROR;
        break;
    }

    for (i = 0; i < 5; i++) {
        value = vtss_state->chmap[0].ptp_chmap[i];
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(cfgport, egr_blk_id, VTSS_ANA_PTP_FLOW_PTP_FLOW_ENA(i), &value));
        VTSS_I("Port-no: %d  Egr_Blk_id: 0x%x, Restoring: VTSS_ANA_PTP_FLOW_PTP_FLOW_ENA(%d):: 0x%x :: 0x%x\n",
               cfgport, egr_blk_id, i, VTSS_ANA_PTP_FLOW_PTP_FLOW_ENA(i), value);
    }


    /* Disable Ingress Flows */
    value = 0x300; /* Default value */
    VTSS_RC(VTSS_PHY_TS_WRITE_CSR(cfgport, ing_blk_id, VTSS_ANA_ETH1_FLOW_CFG_ETH1_FLOW_ENABLE(flow_id), &value));
    VTSS_I("Port-no: %d  Ing_Blk_id: 0x%x, Resetting to Default: VTSS_ANA_ETH1_FLOW_CFG_ETH1_FLOW_ENABLE(%d):: 0x%x :: 0x%x\n",
           cfgport, ing_blk_id, flow_id, VTSS_ANA_ETH1_FLOW_CFG_ETH1_FLOW_ENABLE(flow_id), value);
    value = 0x30; /* Hardware default */
    VTSS_RC(VTSS_PHY_TS_WRITE_CSR(cfgport, ing_blk_id, VTSS_ANA_IP1_FLOW_CFG_IP1_FLOW_ENA(flow_id), &value));
    VTSS_I("Port-no: %d  Ing_Blk_id: 0x%x, Resetting to Default: VTSS_ANA_IP1_FLOW_CFG_IP1_FLOW_ENA(%d):: 0x%x :: 0x%x\n",
           cfgport, ing_blk_id, flow_id, VTSS_ANA_IP1_FLOW_CFG_IP1_FLOW_ENA(flow_id), value);
    /* Disable Egress Flows */
    value = 0x300; /* Default value */
    VTSS_RC(VTSS_PHY_TS_WRITE_CSR(cfgport, egr_blk_id, VTSS_ANA_ETH1_FLOW_CFG_ETH1_FLOW_ENABLE(flow_id), &value));
    VTSS_I("Port-no: %d  Egr_Blk_id: 0x%x, Resetting to Default: VTSS_ANA_ETH1_FLOW_CFG_ETH1_FLOW_ENABLE(%d):: 0x%x :: 0x%x\n",
           cfgport, egr_blk_id, flow_id, VTSS_ANA_ETH1_FLOW_CFG_ETH1_FLOW_ENABLE(flow_id), value);
    value = 0x30; /* Hardware default */
    VTSS_RC(VTSS_PHY_TS_WRITE_CSR(cfgport, egr_blk_id, VTSS_ANA_IP1_FLOW_CFG_IP1_FLOW_ENA(flow_id), &value));
    VTSS_I("Port-no: %d  Egr_Blk_id: 0x%x, Resetting to Default: VTSS_ANA_IP1_FLOW_CFG_IP1_FLOW_ENA(%d):: 0x%x :: 0x%x\n",
           cfgport, egr_blk_id, flow_id, VTSS_ANA_IP1_FLOW_CFG_IP1_FLOW_ENA(flow_id), value);

    /* Disable PTP flows */
    value = 0x30;
    VTSS_RC(VTSS_PHY_TS_WRITE_CSR(cfgport, ing_blk_id, VTSS_ANA_PTP_FLOW_PTP_FLOW_ENA(5), &value));
    VTSS_I("Port-no: %d  Ing_Blk_id: 0x%x, Resetting to Default: VTSS_ANA_PTP_FLOW_PTP_FLOW_ENA(%d):: 0x%x :: 0x%x\n",
           cfgport, ing_blk_id, 5, VTSS_ANA_PTP_FLOW_PTP_FLOW_ENA(5), value);
    VTSS_RC(VTSS_PHY_TS_WRITE_CSR(cfgport, egr_blk_id, VTSS_ANA_PTP_FLOW_PTP_FLOW_ENA(5), &value));
    VTSS_I("Port-no: %d  Egr_Blk_id: 0x%x, Resetting to Default: VTSS_ANA_PTP_FLOW_PTP_FLOW_ENA(%d):: 0x%x :: 0x%x\n",
           cfgport, egr_blk_id, 5, VTSS_ANA_PTP_FLOW_PTP_FLOW_ENA(5), value);

    //VTSS_PHY_TS_SPI_UNPAUSE(port_no);

#ifdef _MADHU_DEBUG_
    /* debug code until final release */
    VTSS_I("\n\n\nIngress Registers Dump port:%d\n", cfgport);

    vtss_phy_ts_read_ing_ana_reg_priv(port_no, cfgport, 0);

    VTSS_I("Egress Registers Dump port:%d\n", cfgport);

    vtss_phy_ts_read_egr_ana_reg_priv(port_no, cfgport, 1);
    vtss_phy_ts_read_proc_reg_priv(port_no, cfgport, 6);
    vtss_phy_ts_read_proc_reg_priv(port_no, cfgport, 7);
#endif

    /* Configure SOF to External PCS */
    VTSS_RC(VTSS_PHY_TS_READ_CSR(port_no, VTSS_PHY_TS_PROC_BLK_ID(0), VTSS_PTP_INGR_IP_1588_TSP_INGR_TSP_CTRL, &value));
    VTSS_I("Port-no: %d  Reading: PROC_BLK_ID: 0x%x; VTSS_PTP_INGR_IP_1588_TSP_INGR_TSP_CTRL: 0x%x\n", port_no, VTSS_PHY_TS_PROC_BLK_ID(0), value);

    if (value & 2) {
        VTSS_E("Port Id: %d Ingress SoF is not cleared by 8051\n", port_no);
        value &= ~(VTSS_F_PTP_INGR_IP_1588_TSP_INGR_TSP_CTRL_INGR_SEL_EXT_SOF_IND);
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(port_no, VTSS_PHY_TS_PROC_BLK_ID(0), VTSS_PTP_INGR_IP_1588_TSP_INGR_TSP_CTRL, &value));
    }

    VTSS_RC(VTSS_PHY_TS_READ_CSR(port_no, VTSS_PHY_TS_PROC_BLK_ID(0), VTSS_PTP_EGR_IP_1588_TSP_EGR_TSP_CTRL, &value));
    VTSS_I("Port-no: %d  Reading: PROC_BLK_ID: 0x%x; VTSS_PTP_EGR_IP_1588_TSP_EGR_TSP_CTRL: 0x%x\n", port_no, VTSS_PHY_TS_PROC_BLK_ID(0), value);
    if (value & 2) {
        VTSS_E("Port Id: %d Egress SoF is not cleared by 8051\n", port_no);
        value &= ~(VTSS_F_PTP_EGR_IP_1588_TSP_EGR_TSP_CTRL_EGR_SEL_EXT_SOF_IND);
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(port_no, VTSS_PHY_TS_PROC_BLK_ID(0),
                                      VTSS_PTP_EGR_IP_1588_TSP_EGR_TSP_CTRL, &value));
    }

    return rc;
}

#define VTSS_TS_MAX_FIFO_ENTRY                     (7)
#define VTSS_MAX_LOOPS_READING_TS_FIFO_EMPTY       (5)
#define VTSS_MAX_NUM_VALID_ENTRIES_TS_FIFO_EMPTY   (10) // This is correlated to the max number of Jumbo Packets to be time stamped, sent by EPG = 9
#define VTSS_TS_FIFO_NANO_SEC_PER_1SEC_ROLLOVER    (0x3B9ACA00)  // NanoSeconds per Second = 1billion
#define VTSS_TS_FIFO_16_BIT_ROLLOVER               (0x10000)  // 16-bit Rollover
#define VTSS_TS_FIFO_16_BIT_1SEC_ROLLOVER          (0xCA00)   // Remainder of: (0x3B9ACA00)/(0x10000), ie. if 1Sec Rollover of enough 16-bit rollovers for 1 second
#define VTSS_TS_DEVIATION_OOS_LIMIT                (32)       // MAX TimeStamp Deviation before declaring OOS!
#define VTSS_TS_SOF_NS_125BYTE_PKT_TO_PKT_TIME     (1160)     // (125+8)*8 + 96ns=1160ns; SOF-to-SOF nsec between two 125byte frames w/96nsec IPG
#define VTSS_TS_SOF_NS_1518BYTE_PKT_TO_PKT_TIME    (20400)    // (1518+8)*8 + 8192ns=20400ns; SOF-to-SOF nsec between two 1518byte frames w/8192ns IPG
#define VTSS_TS_SOF_NS_10KBYTE_PKT_TO_PKT_TIME     (88256)    // (10k+8)*8 + 8192=88256ns from SOF-to-SOF of two 10kbyte frames w/8192nsec IPG


static vtss_rc vtss_phy_ts_tesla_tsp_fifo_is_entry_found(vtss_state_t *vtss_state, vtss_port_no_t   port_no, const vtss_debug_printf_t pr, BOOL *entry_found)
{
    vtss_rc   rc = VTSS_RC_OK;
    u32       loop_cnt;
    u32       val_1st = 0;
    u32       val_2nd = 0;

    *entry_found = FALSE;

    /* Loop reading the TSFIFO_0 register, until TS_EMPTY bit = 0 */
    loop_cnt = VTSS_MAX_LOOPS_READING_TS_FIFO_EMPTY;
    do {
        VTSS_RC(VTSS_PHY_TS_READ_CSR(port_no, VTSS_PHY_TS_PROC_BLK_ID(0),
                                     VTSS_PTP_EGR_IP_1588_TSFIFO_EGR_TSFIFO_0, &val_1st));
        VTSS_RC(VTSS_PHY_TS_READ_CSR(port_no, VTSS_PHY_TS_PROC_BLK_ID(0),
                                     VTSS_PTP_EGR_IP_1588_TSFIFO_EGR_TSFIFO_0, &val_2nd));
        if ((!(val_1st & VTSS_F_PTP_EGR_IP_1588_TSFIFO_EGR_TSFIFO_0_EGR_TS_EMPTY)) &&
            (!(val_2nd & VTSS_F_PTP_EGR_IP_1588_TSFIFO_EGR_TSFIFO_0_EGR_TS_EMPTY))) {
            /* Entries found */
            VTSS_I("Port_No: %d, FIFO entry found - Breaking out of loop, LoopCnt: %d \n", port_no, loop_cnt);
            *entry_found = TRUE;
            break;
        }

        loop_cnt--;
    } while (loop_cnt > 0);

    VTSS_D("Port_No: %d, Is Entry Found in FIFO? : 0x%x \n", port_no, *entry_found);
    return (rc);
}


static vtss_rc vtss_phy_ts_tesla_tsp_fifo_read_ts_fifo(vtss_state_t *vtss_state, vtss_port_no_t   port_no, const vtss_debug_printf_t pr, u32 fifo_val[], u32 *depth, BOOL *fifo_empty)
{
    vtss_rc                         rc = VTSS_RC_OK;
    u32   loop_cnt;
    u32   loop_cnt2;
    u32   val_1st = 0;
    u32   val_2nd = 0;
    u32   value = 0;

    *depth = 0;

    for (loop_cnt = 0; loop_cnt < VTSS_TS_MAX_FIFO_ENTRY; loop_cnt++) {
        fifo_val[loop_cnt] = 0;
    }

    loop_cnt2 = VTSS_MAX_LOOPS_READING_TS_FIFO_EMPTY;
    do {
        VTSS_RC(VTSS_PHY_TS_READ_CSR(port_no, VTSS_PHY_TS_PROC_BLK_ID(0),
                                     VTSS_PTP_EGR_IP_1588_TSFIFO_EGR_TSFIFO_0, &val_1st));

        VTSS_RC(VTSS_PHY_TS_READ_CSR(port_no, VTSS_PHY_TS_PROC_BLK_ID(0),
                                     VTSS_PTP_EGR_IP_1588_TSFIFO_EGR_TSFIFO_0, &val_2nd));

        fifo_val[0] = val_2nd;  // Always update fifo_val[0]
        if ((val_1st & VTSS_F_PTP_EGR_IP_1588_TSFIFO_EGR_TSFIFO_0_EGR_TS_EMPTY) &&
            (val_2nd & VTSS_F_PTP_EGR_IP_1588_TSFIFO_EGR_TSFIFO_0_EGR_TS_EMPTY)) {
            VTSS_D("Port_No: %d, 2-Consecutive Valid EGR_TSFIFO_0 - loop_cnt: %d  - Exit Loop \n", port_no, loop_cnt2);
            break;
        }
        loop_cnt2--;
    } while ((loop_cnt2 > 0));  /* Prevent run-away loop, We are sending 9 Jumbo Packets by EPG, We should always break out based on TS_EMPTY Flag */

    VTSS_RC(VTSS_PHY_TS_READ_CSR(port_no, VTSS_PHY_TS_PROC_BLK_ID(0),
                                 VTSS_PTP_EGR_IP_1588_TSFIFO_EGR_TSFIFO_1, &fifo_val[1]));

    VTSS_RC(VTSS_PHY_TS_READ_CSR(port_no, VTSS_PHY_TS_PROC_BLK_ID(0),
                                 VTSS_PTP_EGR_IP_1588_TSFIFO_EGR_TSFIFO_2, &fifo_val[2]));

    VTSS_RC(VTSS_PHY_TS_READ_CSR(port_no, VTSS_PHY_TS_PROC_BLK_ID(0),
                                 VTSS_PTP_EGR_IP_1588_TSFIFO_EGR_TSFIFO_3, &fifo_val[3]));
    VTSS_RC(VTSS_PHY_TS_READ_CSR(port_no, VTSS_PHY_TS_PROC_BLK_ID(0),
                                 VTSS_PTP_EGR_IP_1588_TSFIFO_EGR_TSFIFO_4, &fifo_val[4]));
    VTSS_RC(VTSS_PHY_TS_READ_CSR(port_no, VTSS_PHY_TS_PROC_BLK_ID(0),
                                 VTSS_PTP_EGR_IP_1588_TSFIFO_EGR_TSFIFO_5, &fifo_val[5]));
    VTSS_RC(VTSS_PHY_TS_READ_CSR(port_no, VTSS_PHY_TS_PROC_BLK_ID(0),
                                 VTSS_PTP_EGR_IP_1588_TSFIFO_EGR_TSFIFO_6, &fifo_val[6]));

    /* Read the TSFIFO_CSR register and check FIFO level associated with the last read of the TS_EMPTY status field of the TSFIFO_0 register */
    VTSS_RC(VTSS_PHY_TS_READ_CSR(port_no, VTSS_PHY_TS_PROC_BLK_ID(0),
                                 VTSS_PTP_EGR_IP_1588_TSFIFO_EGR_TSFIFO_CSR, &value));

    *depth = VTSS_X_PTP_EGR_IP_1588_TSFIFO_EGR_TSFIFO_CSR_EGR_TS_LEVEL(value);

    pr("Port_No: %d, Sig&TS :%d::%08X%08X%08X%08X%08X%08X%08X \n", port_no, *depth, fifo_val[0], fifo_val[1], fifo_val[2], fifo_val[3],
       fifo_val[4], fifo_val[5], fifo_val[6]);

    if (fifo_val[0] & VTSS_F_PTP_EGR_IP_1588_TSFIFO_EGR_TSFIFO_0_EGR_TS_EMPTY) {
        *fifo_empty = TRUE;
        VTSS_D ("Port_no: %d;  FIFO EMPTY!! TSFIFO_0_EGR_TS_EMPTY: Depth: %d \n", port_no, *depth);
    }

    return (rc);

}



static vtss_rc vtss_phy_ts_tesla_tsp_fifo_sync_empty_ts_fifo(vtss_state_t *vtss_state, vtss_port_no_t   port_no, const vtss_debug_printf_t pr, BOOL ingress, u32 *entry)
{
    vtss_rc                         rc = VTSS_RC_OK;
    u32   loop_cnt;
    BOOL  entry_found = FALSE;
    BOOL  fifo_empty = FALSE;
    u32   depth = 0;
    u32 diff_0;
    u32 diff_1;
    u32   fifo_val[VTSS_TS_MAX_FIFO_ENTRY] = {0, 0, 0, 0, 0, 0, 0};
    u32   last_fifo_val[VTSS_TS_MAX_FIFO_ENTRY] = {0, 0, 0, 0x013F013f, 0x013F013F, 0, 0}; /* ensure we don't fail during dummy test of TSFIFO0 */

    VTSS_I("Port_No: %d, Empty  FIFO  \n", port_no);
    *entry = 0;

    /* Loop reading the TSFIFO_0 register, until TS_EMPTY bit = 0 */
    if ((rc = vtss_phy_ts_tesla_tsp_fifo_is_entry_found(vtss_state, port_no, pr, &entry_found)) != VTSS_RC_OK) {
        VTSS_E("Port_no: %d;  Unable to Read FIFO - Entry Found? - Exit! \n", port_no);
        return VTSS_RC_ERROR;
    }

    if (entry_found) {
        if (ingress) {
            loop_cnt = VTSS_MAX_NUM_VALID_ENTRIES_TS_FIFO_EMPTY;
            VTSS_I("Port_No: %d, Valid FIFO entry found - Starting Loop of %d loops  entry: %d \n", port_no, loop_cnt, *entry);
            do {
                if ((rc = vtss_phy_ts_tesla_tsp_fifo_read_ts_fifo(vtss_state, port_no, pr, fifo_val, &depth, &fifo_empty)) != VTSS_RC_OK) {
                    VTSS_E("Port_no: %d;  Unable to read FIFO!: Depth: %d  loop_cnt: %d \n", port_no, depth, loop_cnt);
                    return VTSS_RC_ERROR;
                }

                // We don't want to break out until we've read ALL the TSFIFO_0->6
                if (fifo_empty) {
                    VTSS_I("Port_no: %d;  TSFIFO_0_EGR_TS_EMPTY: Depth: %d  loop_cnt: %d \n", port_no, depth, loop_cnt);
                    /* confirm signature entries are identical to deduce no ingress TS occurring */
                    break;
                }
                diff_0 = fifo_val[3] - last_fifo_val[3];
                diff_1 = fifo_val[4] - last_fifo_val[4];
                VTSS_I("last_ts_fifo[3] : %04X, last_ts_fifo[4] : %04X,\n", last_fifo_val[3] , last_fifo_val[4]);
                VTSS_I("ts_fifo[3] : %04X, ts_fifo[4] : %04X,\n", fifo_val[3] , fifo_val[4]);

                if ((diff_0 != 0) || (diff_1 != 0)) {
                    /* confirm signature entries are identical to deduce no ingress TS occurring */
                    VTSS_E("Port_no: %d;  TSFIFO signature mismatch, check that igr timestamping is occurring: Depth: %d  loop_cnt: %d \n", port_no, depth, loop_cnt);
                    VTSS_E("last_ts_fifo[3] : %04X, last_ts_fifo[4] : %04X,\n", last_fifo_val[3] , last_fifo_val[4]);
                    VTSS_E("ts_fifo[3] : %04X, ts_fifo[4] : %04X,\n", fifo_val[3] , fifo_val[4]);

                    (*entry)++;
                }

                loop_cnt--;
            } while ((loop_cnt > 0));

        } else {   // EGRESS

            loop_cnt = VTSS_MAX_NUM_VALID_ENTRIES_TS_FIFO_EMPTY;
            VTSS_I("Port_No: %d, Valid FIFO entry found - Starting Loop of %d loops  entry: %d \n", port_no, loop_cnt, *entry);
            do {
                if ((rc = vtss_phy_ts_tesla_tsp_fifo_read_ts_fifo(vtss_state, port_no, pr, fifo_val, &depth, &fifo_empty)) != VTSS_RC_OK) {
                    VTSS_E("Port_no: %d;  Unable to read FIFO!: Depth: %d  loop_cnt: %d \n", port_no, depth, loop_cnt);
                    return VTSS_RC_ERROR;
                }

                // We don't want to break out until we've read ALL the TSFIFO_0->6
                if (fifo_empty) {
                    VTSS_I("Port_no: %d;  TSFIFO_0_EGR_TS_EMPTY: Depth: %d  loop_cnt: %d \n", port_no, depth, loop_cnt);
                    break;
                }

                loop_cnt--;
                (*entry)++;
            } while ((loop_cnt > 0));  /* Prevent run-away loop, We are sending 9 Jumbo Packets by EPG, We should always break out based on TS_EMPTY Flag */
        }
    }

    VTSS_I("Port_no: %d;  TSFIFO_0_EGR_TS_EMPTY: Depth: %d  entries: %d \n", port_no, depth, *entry);
    return rc;
}



static vtss_rc vtss_phy_ts_tesla_tsp_fifo_sync_read_egress_ts_fifo(vtss_state_t *vtss_state, vtss_port_no_t   port_no, const vtss_debug_printf_t pr, BOOL *egr_OOS, u16 *fifodepth, BOOL smallframe, u16 *discIndex)
{
    vtss_rc                         rc = VTSS_RC_OK;
    vtss_port_no_t                  base_port_no;
    u32   loop_cnt = 0;
    u32   fifo_val[VTSS_TS_MAX_FIFO_ENTRY] = {0, 0, 0, 0, 0, 0, 0};
    u32   depth = 10000; // max depth is 8, this ensures timestamp is checked on first pass through ts check loop
    BOOL  entry_found = FALSE;
    BOOL  first_time = TRUE;
    BOOL  fifo_empty = FALSE;
    u32   egress_offset = 0;
    u32   egr_32ns = 0, egr_32ns_last = 0;
    u32   egr_ts_dev = 0; /* Egress timestamp deviation from its expected value */
    u32   egr_dis_ref = 0;          // 1160ns from SOF of one 125byte frame to the SOF another 125byte frame
    i32   egr_ts_dis = 0;           /* This Value MUST be Signed!! Difference between 2 successive egr timestamps */
    //u64   egr_ts_dis = 0; /* Difference between 2 successive egr timestamps */
    u8    max_fifo_read = 0;
    u16   idx = 0;

    VTSS_I("Port_No: %d, EGRESS Read FIFO \n", port_no);
    *discIndex = 0;

    if (smallframe) {
        egr_dis_ref = VTSS_TS_SOF_NS_125BYTE_PKT_TO_PKT_TIME;    // 1160ns from SOF of one 125byte frame to the SOF another 125byte frame
    } else {
        //egr_dis_ref = VTSS_TS_SOF_NS_10KBYTE_PKT_TO_PKT_TIME;    // 88256ns from SOF of one Jumbo to the SOF of another packet
        egr_dis_ref = VTSS_TS_SOF_NS_1518BYTE_PKT_TO_PKT_TIME;   // 22400ns from SOF of one 1518Byte Pkt to the SOF of another packet
    }
    VTSS_I("Port_No: %d, EGRESS_Read_FIFO, EGR_Ref Time dist: %d nsec SOF-to-SOF: (0x%x) nsec\n", port_no, egr_dis_ref, egr_dis_ref);

    if ((rc = vtss_phy_ts_base_port_get_priv(vtss_state, port_no, &base_port_no)) != VTSS_RC_OK) {
        VTSS_E("Port %d:: Base Port get failed \n", port_no);
        return VTSS_RC_ERROR;
    }

    /* Loop reading the TSFIFO_0 register, until TS_EMPTY bit = 0 */
    if ((rc = vtss_phy_ts_tesla_tsp_fifo_is_entry_found(vtss_state, port_no, pr, &entry_found)) != VTSS_RC_OK) {
        VTSS_E("Port_no: %d;  INGRESS:: Unable to Read FIFO - Entry Found? - Exit! \n", port_no);
        return VTSS_RC_ERROR;
    }

    if (entry_found) {
        loop_cnt = VTSS_MAX_NUM_VALID_ENTRIES_TS_FIFO_EMPTY;
        VTSS_I("Port_No: %d, Valid FIFO entry found - Starting Loop of %d loops \n", port_no, loop_cnt);
        idx = 0;
        do {
            if ((rc = vtss_phy_ts_tesla_tsp_fifo_read_ts_fifo(vtss_state, port_no, pr, fifo_val, &depth, &fifo_empty)) != VTSS_RC_OK) {
                VTSS_E("Port_no: %d;  EGRESS:: Unable to read FIFO!: Depth: %d  loop_cnt: %d \n", port_no, depth, loop_cnt);
                return VTSS_RC_ERROR;
            }

            // We don't want to break out until we've read ALL the TSFIFO_0->6
            if (fifo_empty) {
                VTSS_I("Port_no: %d;  EGRESS:: TSFIFO_0_EGR_TS_EMPTY: Depth: %d  loop_cnt: %d \n", port_no, depth, loop_cnt);
                break;

            } else {

                egr_32ns = (fifo_val[0] & 0xFFFF) | ((fifo_val[1] & 0xFFFF) << 16);

                /* Calculate distance b/w every set of 2 consequetive egress timestamps */
                /* Handle the 1second Rollover - Done */
                /* Handle the 1e-16 Rollover ??? */
                /* Handle the value of 0 ??? */
                //if (depth != 10000)
                if (!first_time) {
                    /* This calculation is ommitted only for first entry - because it is compared to the 2nd entry */
                    if (egr_32ns_last < egr_32ns) {
                        egr_ts_dis = egr_32ns - egr_32ns_last;  // Normal Operation
                    } else {
                        // Rollover Event for 1 Second Rollover; 1 billion NanoSecs = 1Sec
                        egr_ts_dis = (VTSS_TS_FIFO_NANO_SEC_PER_1SEC_ROLLOVER + egr_32ns) - egr_32ns_last;
                        VTSS_I("1-Sec Rollover or Out-of-Sync Pkt Event has occured - distance egr_ts_dis 0x%x  egr_32ns: 0x%x;  egr_32ns_last: 0x%x \n", egr_ts_dis, egr_32ns, egr_32ns_last);
                    }
                    // This is indicating a deviation of more than 10nsec
                    egr_ts_dev = ((egr_ts_dis > egr_dis_ref) ? (egr_ts_dis - egr_dis_ref) : (egr_dis_ref - egr_ts_dis));
                    egress_offset = 7 - depth;

                    // 10 Nsec deviation to handle clock variations
                    if ((egr_ts_dis > (egr_dis_ref + VTSS_TS_DEVIATION_OOS_LIMIT)) || (egr_ts_dis < (egr_dis_ref - VTSS_TS_DEVIATION_OOS_LIMIT))) {
                        *egr_OOS = TRUE;
                        *discIndex = idx;
                        VTSS_I("Egress OUT-OF-SYNC, Detected at depth %d offset %d distance 0x%x deviation 0x%x, egr_32ns: 0x%x, egr_32ns_last 0x%x \n", depth, egress_offset, egr_ts_dis, egr_ts_dev, egr_32ns, egr_32ns_last );
                        //break;
                    } else {
                        VTSS_I("Egress info at depth %d offset %d distance 0x%x deviation 0x%x egr_32ns:0x%x egr_32ns_last:0x%x \n", depth, egress_offset, egr_ts_dis, egr_ts_dev, egr_32ns, egr_32ns_last);
                    }

                } else {
                    VTSS_I("Egress First_Time: depth x0%x \n", depth);
                    first_time = FALSE;
                }

                egr_32ns_last = egr_32ns;
            }

            VTSS_I ("Depth to be read : %d fifo_read:%d   loop_cnt: %d \n", depth, max_fifo_read, loop_cnt);
            idx++;   // The 0 case is always skipped over because of the first time flag
            loop_cnt--;
        } while ((loop_cnt > 0) && (max_fifo_read++ < 20));  /* completely empty the FIFO to ensure no stale entries whenever recovery subsequently called */

        *fifodepth = depth;

    } else {
        VTSS_I("Port_No: %d, NO Valid FIFO entry found - Exitting \n", port_no);
    }

    VTSS_I("Exit: Port_No: %d, EGRESS Read FIFO;   fifo_depth: 0x%x;    egr_OOS: 0x%x \n", port_no, *fifodepth, *egr_OOS);

    return rc;
}


static vtss_rc vtss_phy_ts_tesla_tsp_fifo_sync_read_ingress_ts_fifo(vtss_state_t *vtss_state, vtss_port_no_t   port_no, const vtss_debug_printf_t pr, BOOL *ing_OOS, u16 *fifodepth, BOOL smallframe, u16 *discIndex)
{

    vtss_rc                         rc = VTSS_RC_OK;
    vtss_port_no_t                  base_port_no;
    u32   loop_cnt = 0;
    u32   fifo_val[VTSS_TS_MAX_FIFO_ENTRY] = {0, 0, 0, 0, 0, 0, 0};
    u32   depth = 10000; // max depth is 8, this ensures timestamp is checked on first pass through ts check loop
    BOOL  entry_found = FALSE;
    BOOL  first_time = TRUE;
    BOOL  fifo_empty = FALSE;
    vtss_phy_ts_fifo_sig_mask_t sig_mask;
    u32   ingress_offset = 0;
    u32   ing_32ns = 0; /**< 4 bytes nano-sec part of Timestamp */
    u32   ing_32ns_last = 0;
    u32   ing_ts_dev = 0; /* Ingress timestamp deviation from its expected value */
    u32   ing_ts_dis = 0; /* Difference between 2 successive egr timestamps */
    //u32   ing_dis_ref = 0;          // 1160ns from SOF of one 125byte frame to the SOF another 125byte frame
    i32   ing_dis_ref = 0;          // This MUST be Signed!!! 1160ns from SOF of one 125byte frame to the SOF another 125byte frame
    u8    max_fifo_read = 0;
    u16   idx = 0;

    VTSS_I("Port_No: %d, INGRESS Read FIFO  \n", port_no);

    if (smallframe) {
        ing_dis_ref = VTSS_TS_SOF_NS_125BYTE_PKT_TO_PKT_TIME;    // 1160ns from SOF of one 125byte frame to the SOF another 125byte frame
    } else {
        //ing_dis_ref = VTSS_TS_SOF_NS_10KBYTE_PKT_TO_PKT_TIME;    // 88256ns from SOF of one Jumbo to the SOF of another packet
        ing_dis_ref = VTSS_TS_SOF_NS_1518BYTE_PKT_TO_PKT_TIME;   // 22400ns from SOF of one 1518Byte Pkt to the SOF of another packet
    }
    VTSS_I("Port_No: %d, INGRESS_Read_FIFO, ING_Ref Time dist: %d nsec SOF-to-SOF: (0x%x) nsec\n", port_no, ing_dis_ref, ing_dis_ref);

    if ((rc = vtss_phy_ts_base_port_get_priv(vtss_state, port_no, &base_port_no)) != VTSS_RC_OK) {
        VTSS_E("Port %d:: Base Port get failed  \n", port_no);
        return VTSS_RC_ERROR;
    }

    sig_mask = vtss_state->phy_ts_port_conf[base_port_no].sig_mask;


    /* Loop reading the TSFIFO_0 register, until TS_EMPTY bit = 0 */
    if ((rc = vtss_phy_ts_tesla_tsp_fifo_is_entry_found(vtss_state, port_no, pr, &entry_found)) != VTSS_RC_OK) {
        VTSS_E("Port_no: %d;  INGRESS:: Unable to Read FIFO - Entry Found? - Exit!  \n", port_no);
        return VTSS_RC_ERROR;
    }

    if (entry_found) {
        VTSS_I("Port_No: %d, Valid FIFO entry found \n", port_no);
        loop_cnt = VTSS_MAX_NUM_VALID_ENTRIES_TS_FIFO_EMPTY;
        idx = 0;
        do {
            if ((rc = vtss_phy_ts_tesla_tsp_fifo_read_ts_fifo(vtss_state, port_no, pr, fifo_val, &depth, &fifo_empty)) != VTSS_RC_OK) {
                VTSS_E("Port_no: %d;  INGRESS:: Unable to read FIFO!: Depth: %d fifo_read:%d   loop_cnt: %d \n", port_no, depth, max_fifo_read, loop_cnt);
                return VTSS_RC_ERROR;
            }

            // We don't want to break out until we've read ALL the TSFIFO_0->6
            if (fifo_empty) {
                VTSS_I("Port_no: %d;  INGRESS:: TSFIFO_0_EGR_TS_EMPTY: Depth: %d fifo_read:%d   loop_cnt: %d \n", port_no, depth, max_fifo_read, loop_cnt);
                break;
            } else {
                if (sig_mask & VTSS_PHY_TS_FIFO_SIG_SEQ_ID) {
                    //ing_ns = (fifo_val[3] & 0xFFFF);
                    ing_32ns = fifo_val[3]; // Use the Whole 32bit field
                }

                /* Calculate distance b/w every set of 2 consequetive egress timestamps */
                /* Handle the 1second Rollover - Done */
                /* Handle the 1e-16 Rollover ??? */
                /* Handle the value of 0 ??? */
                //if (depth != 10000) {/* This calculation is ommitted only for first entry */
                if (!first_time) {/* This calculation is ommitted only for first entry */
                    if (ing_32ns_last < ing_32ns) {
                        ing_ts_dis = ing_32ns - ing_32ns_last;  // Normal Operation
                    } else {
                        // Need to determine if this was a Rollover event or if this was just a Frame out-of-order event
                        // Rollover Event for 1 Second Rollover; 1 billion NanoSecs = 1Sec
                        ing_ts_dis = (VTSS_TS_FIFO_NANO_SEC_PER_1SEC_ROLLOVER + ing_32ns) - ing_32ns_last;
                        VTSS_I("1-Sec Rollover or Out-of-Sync Pkt Event has occured - distance ing_ts_dis 0x%x  ing_32ns: 0x%x;  ing_32ns_last: 0x%x \n",
                               ing_ts_dis, ing_32ns, ing_32ns_last);
                    }

                    /* The following calculation is for all other deviations */
                    // Compute the Delta between the current packet-to-packet Deviation and the reference deviation,
                    // Note: The Reference Deviation is based on packet size
                    ing_ts_dev = ((ing_ts_dis > ing_dis_ref) ? (ing_ts_dis - ing_dis_ref) : (ing_dis_ref - ing_ts_dis));

                    if ((ing_ts_dis > (ing_dis_ref + VTSS_TS_DEVIATION_OOS_LIMIT)) || (ing_ts_dis < (ing_dis_ref - VTSS_TS_DEVIATION_OOS_LIMIT))) {
                        *ing_OOS = TRUE;
                        *discIndex = idx;
                        ingress_offset = 7 - depth;
                        VTSS_I("Ingress OUT-OF-SYNC, Detected at depth %d offset %d distance 0x%x deviation 0x%x, ing_32ns:0x%x ing_32ns_last:0x%x \n", depth, ingress_offset, ing_ts_dis, ing_ts_dev, ing_32ns, ing_32ns_last);
                        //break;
                    } else {
                        VTSS_I("Ingress: Depth %d offset %d distance 0x%x deviation 0x%x ing_32ns:0x%x ing_32ns_last:0x%x  \n", depth, ingress_offset, ing_ts_dis, ing_ts_dev, ing_32ns, ing_32ns_last);
                    }
                } else {
                    VTSS_I("Ingress: Depth = 10000  -  First Time! \n");
                    first_time = FALSE;
                }

                ing_32ns_last = ing_32ns;
            }

            VTSS_I ("Depth to be read : %d fifo_read:%d   loop_cnt: %d \n", depth, max_fifo_read, loop_cnt);
            idx++;   // The 0 case is always skipped over because of the first time flag
            loop_cnt--;
        } while ((loop_cnt > 0) && (max_fifo_read++ < 20));  /* completely empty the FIFO to ensure no stale entries whenever recovery subsequently called */

        *fifodepth = depth;

    } else {
        VTSS_I("Port_No: %d, NO Valid FIFO entry found - Exitting \n", port_no);
    }

    VTSS_I("Exit: Port_No: %d, INGRESS Read FIFO;   fifo_depth: 0x%x;    ing_OOS: 0x%x \n", port_no, *fifodepth, *ing_OOS);
    return rc;

}

static vtss_rc vtss_phy_ts_tesla_tsp_fifo_epg_done(const vtss_port_no_t                  port_no,
                                                   int pkt_count, BOOL ingress)
{
    // NOTE:  there are two state-based solutions:  a) poll EPG start/stop bit
    // 14 until it auto-clears (TBD this works for an interrupted test-mode burst)
    // b) poll SerDes good pkt counter until proper count observed at PCS output.
    // IF THE FOLLOWING COUNTER-BASED METHOD DOES NOT WORK SYSTEM-LEVEL TEST THEN
    // PLEASE REPLACE WITH GUARD-BANDED VTSS_MSLEEP TIMER CONSTANTS AS FOLLOWS:
    // 10/1 recover jumbo PKTS, 8.192us IPG ==> 2.65 milliseconds
    // 300 detection 125 byte PKTS, 96ns IPG ==> 1 milliseconds

    vtss_rc                 rc = VTSS_RC_OK;

    if (ingress) {
        switch (pkt_count) {
        case 1:
        case 10: {
            VTSS_MSLEEP(3);
            break;
        }
        case 300: {
            VTSS_MSLEEP(1);
            break;
        }
        default:  {
            VTSS_I("DEBUG:  need to enter a delay for this pkt count\n");
        }
        }
    } else {
        /* TODO : Add appropriate delay for egress */
        switch (pkt_count) {
        case 1:
        case 10: {
            VTSS_MSLEEP(3);
            break;
        }
        case 300: {
            VTSS_MSLEEP(1);
            break;
        }
        default:
            break;
        }
    }
    return rc;
}


static vtss_rc vtss_phy_ts_tesla_ingress_tsp_fifo_sync_conf_analyzer(vtss_state_t *vtss_state, vtss_port_no_t  port_no,
                                                                     vtss_phy_ts_engine_t eng_id,
                                                                     vtss_phy_ts_engine_channel_map_t channel)
{
    vtss_rc        rc = VTSS_RC_OK;

    do {

        /*  Configure ingress Analyzer VTSS_PHY_TS_ANA_BLK_ID_ING_0  */
        rc = vtss_phy_ts_tesla_ingress_tsp_fifo_sync_flow_add(vtss_state, port_no, channel, eng_id, TRUE);
        if (rc != VTSS_RC_OK) {
            VTSS_E("Ingress TS Error Detect : Ing Flow add Failed, port-no: %d\n", port_no);
            break;
        }
        rc = vtss_phy_ts_tesla_ingress_tsp_fifo_sync_ing_act_add(vtss_state, port_no, eng_id, channel);
        if (rc != VTSS_RC_OK) {
            VTSS_E("Ingress TS Error Detect : Ing Action add Failed, port-no: %d\n", port_no);
            break;
        }
        /*  Configure egress Analyzer(VTSS_PHY_TS_ANA_BLK_ID_EGR_0) to save egress timestamp in FIFO and extract ingress timestamps saved in the FIFO  */
        rc = vtss_phy_ts_tesla_ingress_tsp_fifo_sync_flow_add(vtss_state, port_no, channel, eng_id, FALSE);
        if (rc != VTSS_RC_OK) {
            VTSS_E("Ingress TS Error Detect : Egr Flow Add Failed, port-no: %d\n", port_no);
            break;
        }
        rc = vtss_phy_ts_tesla_ingress_tsp_fifo_sync_egr_act_add(vtss_state, port_no, eng_id, channel);
        if (rc != VTSS_RC_OK) {
            VTSS_E("Ingress TS Error Detect : Egr Action Add Failed, port-no: %d\n", port_no);
            break;
        }
    } while (0);

    return rc;

}



static vtss_rc vtss_phy_ts_tesla_tsp_fifo_toggle_spi(vtss_state_t *vtss_state,
                                                     vtss_port_no_t  port_no,
                                                     u32             *si_cfg,
                                                     BOOL            togglespi_en)
{
    u32                     value = 0;
    vtss_rc                 rc = VTSS_RC_OK;

    VTSS_PHY_TS_SPI_PAUSE_PRIV(port_no);
    if (togglespi_en) {
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(port_no, VTSS_PHY_TS_PROC_BLK_ID(0),
                                      VTSS_PTP_TS_FIFO_SI_TS_FIFO_SI_CFG, si_cfg));
        VTSS_I("SPI SI_CFG Mode: %d! port %d \n", togglespi_en, port_no);
    }
    VTSS_PHY_TS_SPI_UNPAUSE_PRIV(port_no);

#if defined(VTSS_CHIP_CU_PHY) && defined(VTSS_PHY_TS_SPI_CLK_THRU_PPS0)
    /* Disable New SPI mode */
    if ( vtss_state->phy_ts_port_conf[port_no].new_spi_conf.enable ) {
        VTSS_I("New SPI Mode disabled! port %d \n", port_no);
        if ((rc = VTSS_RC_COLD(vtss_phy_ts_new_spi_mode_set_priv(vtss_state, port_no, togglespi_en))) != VTSS_RC_OK) {
            VTSS_E("New SPI Mode Disable failed! port %d \n", port_no);
            rc = VTSS_RC_ERROR;
        }
        VTSS_I("New SPI Mode: %d! port %d \n", togglespi_en, port_no);
    }
#endif // #if defined(VTSS_CHIP_CU_PHY) && defined(VTSS_PHY_TS_SPI_CLK_THRU_PPS0)
    VTSS_PHY_TS_SPI_PAUSE_PRIV(port_no);
    if (!togglespi_en) {
        /* TSFIFO read mode change to MDIO */
        value = 0;
        VTSS_RC(VTSS_PHY_TS_READ_CSR(port_no, VTSS_PHY_TS_PROC_BLK_ID(0),
                                     VTSS_PTP_TS_FIFO_SI_TS_FIFO_SI_CFG, &value));
        *si_cfg = value;
        /* Clear the serial FIFO mode, configure in MDIO*/
        value = VTSS_PHY_TS_CLR_BITS(value, VTSS_F_PTP_TS_FIFO_SI_TS_FIFO_SI_CFG_TS_FIFO_SI_ENA);
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(port_no, VTSS_PHY_TS_PROC_BLK_ID(0),
                                      VTSS_PTP_TS_FIFO_SI_TS_FIFO_SI_CFG, &value));
    }
    VTSS_PHY_TS_SPI_UNPAUSE_PRIV(port_no);
    return rc;

}

static vtss_rc vtss_phy_ts_tesla_8051_timer_start(vtss_state_t *vtss_state, vtss_port_no_t port_no, u16 channel, BOOL ingress_fifo, u16 pkt_cnt, BOOL match)
{
    vtss_miim_read_t   miim_read_func = vtss_state->init_conf.miim_read;
    vtss_miim_write_t  miim_write_func = vtss_state->init_conf.miim_write;
    vtss_rc            rc = VTSS_RC_OK;
    vtss_phy_type_t     phy_id;
    u16 value, readval;

    rc = vtss_phy_id_get_priv(vtss_state, port_no, &phy_id);
    if (rc != VTSS_RC_OK) {
        VTSS_E("Base Port get Failed  %d \n", port_no);
        return rc;
    }
    // configure 8051 timer for corresponding pkt count
    if (ingress_fifo == TRUE) {
        if (pkt_cnt == 10) {     // Note: Tests show that is unreliable - the EPG may send 20 packets not 10
            value = 0xf831;         // Burst 10 Packets, but Flip the SOF bit to INT at the 10th packet, but 2nd burst of 10 follows
        } else if (pkt_cnt == 9) {
            value = 0xf906;         // Burst 10 Packets, but Flip the SOF bit to INT at the 9th packet
        } else if (pkt_cnt == 8) {
            value = 0xf9da;         // Burst 10 Packets, but Flip the SOF bit to INT at the 8th packet
        } else if (pkt_cnt == 7) {
            value = 0xfaaf;
        } else if (pkt_cnt == 6) {
            value = 0xfb83;
        } else if (pkt_cnt == 5) {
            value = 0xfc58;
        } else if (pkt_cnt == 4) {
            value = 0xfd2c;
        } else if (pkt_cnt == 3) {
            value = 0xfe01;         // Burst 10 Packets, but Flip the SOF bit to INT at the 1st packet
        } else if (pkt_cnt == 2) {
            value = 0xfed5;         // Burst 10 Packets, but Flip the SOF bit to INT at the 1st packet
        } else if (pkt_cnt == 1) {
            value = 0xffaa;         // Burst 10 Packets, but Flip the SOF bit to INT at the 1st packet
        } else {
            value = 0xf9da;  // Default: Burst 10 Packets, but Flip the SOF bit to INT at the 9th packet, ensuring only 1 burst of 10 packets
        }
    } else {
        if (pkt_cnt == 10) {     // Note: Tests show that is unreliable - the EPG may send 20 packets not 10
            value = 0xf850;          // Burst 10 Packets, but Flip the SOF bit to INT at the 10th packet, but 2nd burst of 10 follows
        } else if (pkt_cnt == 9) {
            value = 0xf925;         // Burst 10 Packets, but Flip the SOF bit to INT at the 9th packet
        } else if (pkt_cnt == 8) {
            value = 0xf9f9;          // Burst 10 Packets, but Flip the SOF bit to INT at the 8th packet
        } else if (pkt_cnt == 7) {
            value = 0xface;
        } else if (pkt_cnt == 6) {
            value = 0xfba2;
        } else if (pkt_cnt == 5) {
            value = 0xfc77;
        } else if (pkt_cnt == 4) {
            value = 0xfd4b;
        } else if (pkt_cnt == 3) {
            value = 0xfe20;         // Burst 10 Packets, but Flip the SOF bit to INT at the 1st packet
        } else if (pkt_cnt == 2) {
            value = 0xfef4;         // Burst 10 Packets, but Flip the SOF bit to INT at the 1st packet
        } else if (pkt_cnt == 1) {
            value = 0xffc9;         // Burst 10 Packets, but Flip the SOF bit to INT at the 1st packet
        } else {
            value = 0xf9f9;  // Default: Burst 10 Packets, but Flip the SOF bit to INT at the 9th packet, ensuring only 1 burst of 10 packets
        }
    }

    miim_write_func(vtss_state, port_no, 0x1f, VTSS_PHY_PAGE_EXTENDED);
    miim_read_func(vtss_state, port_no, 0x19, &readval);
    VTSS_I("Port_No: %d, Setting Timer for SOF Switch: Reading Ext:%d Reg:%02d: 0x%04X \n", port_no, VTSS_PHY_PAGE_EXTENDED, 0x19, readval);
    miim_write_func(vtss_state, port_no, 0x19, value);
    miim_read_func(vtss_state, port_no, 0x19, &readval);
    VTSS_I("Setting timer for SOF Switch of %s fifo at packet count %d\n", ingress_fifo ? "ingress" : "egress", pkt_cnt);
    VTSS_I("Port_No: %d, Set SOF timer: Reading Ext:%d Reg:%02d: 0x%04X \n", port_no, VTSS_PHY_PAGE_EXTENDED, 0x19, readval);
    miim_write_func(vtss_state, port_no, 0x1f, VTSS_PHY_PAGE_STANDARD);

    // trigger the 8051 patch on base port
    //miim_write_func(vtss_state, VTSS_PORT_NO_START, 0x1f, VTSS_PHY_PAGE_GPIO);
    miim_write_func(vtss_state, phy_id.base_port_no, 0x1f, VTSS_PHY_PAGE_GPIO);
    value = 0x0;
    value = (0x800b | (channel << 8));
    miim_write_func(vtss_state, phy_id.base_port_no, 0x12, value);
    value = 0;
    miim_read_func(vtss_state, phy_id.base_port_no, 0x12, &value);
    VTSS_I("Base_Port_no: %d channel:%d Port_No: %d, trig 8051 microcommand: Reading GP:%d Reg:%02d: 0x%04X \n", channel, phy_id.base_port_no, port_no, VTSS_PHY_PAGE_GPIO, 0x12, value);

    miim_write_func(vtss_state, phy_id.base_port_no, 0x1f, VTSS_PHY_PAGE_STANDARD);
    rc = vtss_phy_wait_for_micro_complete(vtss_state, port_no);
    if (rc == VTSS_RC_OK) {
        if ((value & 0xf000) == 0x4000) { // error code returned from 8051
            VTSS_I("8051 EPG micropatch errored out!\n");
        } else {
            VTSS_I("8051 EPG microcommand Success\n");
        }
    } else {
        VTSS_I("8051 EPG microcommand Failed!\n");
    }
    return VTSS_RC_OK;
}

static vtss_rc vtss_phy_ts_tesla_tsp_sof_conf(vtss_state_t *vtss_state, vtss_port_no_t port_no, BOOL ingress, BOOL sof_ext_pcs)
{
    u32 regval;
    /* Configure SOF to External PCS */
    regval = 0;
    if (ingress == TRUE) {
        VTSS_RC(VTSS_PHY_TS_READ_CSR(port_no, VTSS_PHY_TS_PROC_BLK_ID(0),
                                     VTSS_PTP_INGR_IP_1588_TSP_INGR_TSP_CTRL, &regval));
        if (sof_ext_pcs) {
            regval |= VTSS_F_PTP_INGR_IP_1588_TSP_INGR_TSP_CTRL_INGR_SEL_EXT_SOF_IND;
            VTSS_I("INGRESS:: Setting SOF - Setting SOF to EXT \n");
        } else {
            regval &= ~(VTSS_F_PTP_INGR_IP_1588_TSP_INGR_TSP_CTRL_INGR_SEL_EXT_SOF_IND);
            VTSS_I("INGRESS:: Clearing SOF - Setting SOF to INT \n");
        }

        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(port_no, VTSS_PHY_TS_PROC_BLK_ID(0),
                                      VTSS_PTP_INGR_IP_1588_TSP_INGR_TSP_CTRL, &regval));
    } else {
        VTSS_RC(VTSS_PHY_TS_READ_CSR(port_no, VTSS_PHY_TS_PROC_BLK_ID(0),
                                     VTSS_PTP_EGR_IP_1588_TSP_EGR_TSP_CTRL, &regval));
        if (sof_ext_pcs) {
            regval |= VTSS_F_PTP_EGR_IP_1588_TSP_EGR_TSP_CTRL_EGR_SEL_EXT_SOF_IND;
            VTSS_I("EGRESS:: Setting SOF - Setting SOF to EXT \n");
        } else {
            regval &= ~(VTSS_F_PTP_EGR_IP_1588_TSP_EGR_TSP_CTRL_EGR_SEL_EXT_SOF_IND);
            VTSS_I("EGRESS:: Clearing SOF - Setting SOF to INT \n");
        }
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(port_no, VTSS_PHY_TS_PROC_BLK_ID(0),
                                      VTSS_PTP_EGR_IP_1588_TSP_EGR_TSP_CTRL, &regval));
    }

    // Egress
    VTSS_RC(VTSS_PHY_TS_READ_CSR(port_no, VTSS_PHY_TS_PROC_BLK_ID(0),
                                 VTSS_PTP_EGR_IP_1588_TSP_EGR_TSP_CTRL, &regval));
    VTSS_I("EGRESS:: SOF Setting - %04X \n", regval);
    // Ingress
    VTSS_RC(VTSS_PHY_TS_READ_CSR(port_no, VTSS_PHY_TS_PROC_BLK_ID(0),
                                 VTSS_PTP_INGR_IP_1588_TSP_INGR_TSP_CTRL, &regval));
    VTSS_I("INGRESS:: SOF Setting - %04X \n", regval);

    return VTSS_RC_OK;
}

#define TSP_CMD_OOS_RECOVER 1
#ifdef TSP_CMD_OOS_RECOVER

#undef TSP_CMD_OOS_RECOVER_TESTING
#ifdef TSP_CMD_OOS_RECOVER_TESTING
static vtss_rc vtss_phy_read_1588_rw_mod_frames(const vtss_port_no_t port_no, u32 *ing_ctr, u32 *egr_ctr)
{
    u32 value;

    VTSS_RC(VTSS_PHY_TS_READ_CSR(port_no, VTSS_PHY_TS_PROC_BLK_ID(0),
                                 VTSS_PTP_INGR_IP_1588_RW_INGR_RW_MODFRM_CNT, &value));
    *ing_ctr = value;
    VTSS_RC(VTSS_PHY_TS_READ_CSR(port_no, VTSS_PHY_TS_PROC_BLK_ID(0),
                                 VTSS_PTP_EGR_IP_1588_RW_EGR_RW_MODFRM_CNT, &value));
    *egr_ctr = value;
    return VTSS_RC_OK;
}
#endif

#ifdef TSP_CMD_OOS_RECOVER_TESTING
static void vtss_phy_ts_generate_tsp_cmd_oos(vtss_state_t *vtss_state, const vtss_port_no_t port_no, u16 channel, BOOL ingress)
{
    u32 entry = 0;
    u32 ing_delta = 0;
    u32 egr_delta = 0;
    BOOL sof_ext_pcs;
    vtss_match_type_t match_mode = MATCH_ENGINE_2_PLAN_E;

    VTSS_I("Inside %s\n", __FUNCTION__);
    sof_ext_pcs = TRUE;
    if (vtss_phy_ts_tesla_tsp_fifo_sync_empty_ts_fifo(vtss_state, port_no, pr, ingress, &entry) != VTSS_RC_OK) {
        VTSS_I("Error Clearing FIFO ");
    }
    if (vtss_phy_ts_tesla_tsp_sof_conf(vtss_state, port_no, ingress, sof_ext_pcs) != VTSS_RC_OK) {
        VTSS_E("Error setting SOF");
    }

    //match_mode = MATCH_ENGINE_2_PLAN_E;
    match_mode = ingress ? MATCH_INGRESS : MATCH_EGRESS;
    if (vtss_phy_ts_tesla_epg_send_300_packets(vtss_state, port_no, pr, 125, match_mode, &ing_delta, &egr_delta) != VTSS_RC_OK) {


        VTSS_E("Error Configuring EPG for 300 Matching Frames");
    }
    if (vtss_phy_ts_tesla_tsp_fifo_sync_empty_ts_fifo(vtss_state, port_no, pr, ingress, &entry) != VTSS_RC_OK) {
        VTSS_I("Error Clearing FIFO ");
    }

    //match_mode = MATCH_ENGINE_2_PLAN_E;
    match_mode = ingress ? MATCH_INGRESS : MATCH_EGRESS;
    vtss_phy_ts_tesla_epg_send_jumbo_packets_8051(vtss_state, port_no, pr, channel, ingress, 9, match_mode, &ing_delta, &egr_delta);
    if (vtss_phy_ts_tesla_tsp_fifo_sync_empty_ts_fifo(vtss_state, port_no, pr, ingress, &entry) != VTSS_RC_OK) {
        VTSS_I("Error Clearing FIFO ");
    }

    return;
}
#endif

#ifdef TSP_CMD_OOS_RECOVER_TESTING
static vtss_rc vtss_phy_ts_detect_tsp_cmd_oos(vtss_state_t *vtss_state, const vtss_port_no_t port_no, BOOL ingress, BOOL *cmd_oos)
{
    u32 entry = 0;
    u32 ing_delta = 0;
    u32 egr_delta = 0;
    u32 ing_ctr, egr_ctr, temp_ctr, ctr_diff;
    vtss_match_type_t match_mode = NO_MATCH;

    VTSS_I("Inside %s\n", __FUNCTION__);
    ing_ctr = egr_ctr = 0;
    if (vtss_phy_ts_tesla_tsp_fifo_sync_empty_ts_fifo(vtss_state, port_no, pr, ingress, &entry) != VTSS_RC_OK) {
        VTSS_I("Error Clearing FIFO ");
    }
    vtss_phy_read_1588_rw_mod_frames(port_no, &ing_ctr, &egr_ctr);
    VTSS_I("Before sending non matching frames port=%d RW ing_ctr=%d egr_ctr=%d\n", port_no, ing_ctr, egr_ctr);
    match_mode = ingress ? NO_MATCH_INGRESS : NO_MATCH_EGRESS;
    if (vtss_phy_ts_tesla_epg_send_300_packets(vtss_state, port_no, pr, 125, match_mode, &ing_delta, &egr_delta) != VTSS_RC_OK) {
        VTSS_E("Error Configuring EPG for 300 Matching Frames");
    }

    vtss_phy_read_1588_rw_mod_frames(port_no, &ing_ctr, &egr_ctr);
    VTSS_I("After sending non matching frames port=%d RW ing_ctr=%d egr_ctr=%d\n", port_no, ing_ctr, egr_ctr);
    if (vtss_phy_ts_tesla_tsp_fifo_sync_empty_ts_fifo(vtss_state, port_no, pr, ingress, &entry) != VTSS_RC_OK) {
        VTSS_I("Error Clearing FIFO ");
    }

    vtss_phy_read_1588_rw_mod_frames(port_no, &ing_ctr, &egr_ctr);
    temp_ctr = ingress ? ing_ctr : egr_ctr;
    VTSS_I("Before sending matching frames port=%d RW ing_ctr=%d egr_ctr=%d\n", port_no, ing_ctr, egr_ctr);
    match_mode = ingress ? MATCH_INGRESS : MATCH_EGRESS;
    if (vtss_phy_ts_tesla_epg_send_300_packets(vtss_state, port_no, pr, 125, match_mode, &ing_delta, &egr_delta) != VTSS_RC_OK) {
        VTSS_E("Error Configuring EPG for 300 Matching Frames");
    }

    vtss_phy_read_1588_rw_mod_frames(port_no, &ing_ctr, &egr_ctr);
    ctr_diff = ingress ? (ing_ctr - temp_ctr) : (egr_ctr - temp_ctr);
    VTSS_I("After sending matching frames port=%d RW ing_ctr=%d egr_ctr=%d counter diff = %d\n", port_no, ing_ctr, egr_ctr, ctr_diff);

    *cmd_oos = (ctr_diff < 300) ? TRUE : FALSE;

    return VTSS_RC_OK;
}
#endif

// See TN1327 for full Algorithm
//6.    Min_PlanE_recovery:
//a.    Disable all IP flows, all ETH2 flows. - For both ingress and egress
//b.    Set ETH_NXT_COMPARATOR = 5. ETH_ETYPE_MATCH= 0x0800,
//c.    Set PTP_MASK_LOWER = 0x0, PTP_MASK_UPPER= 0x0, PTP_FLOW_EN(5) = 1
//d.    Send 300 matching 64B packets with min IPG.
//e.    Restore analyzer config to IPv4. ETH_NXT_COMPARATOR=2, Set PTP_MASK_LOWER = previous, PTP_MASK_UPPER= previous
//f.    Flush FIFO.
//g.    If first time running Min_PlanE_recovery: Goto Prep0 and start over
//h.    Else (run before) Issue error: HW RESET REQUIRED, cannot recover.
//
#ifdef TSP_CMD_OOS_RECOVER_TESTING
static vtss_rc vtss_phy_ts_resolve_tsp_cmd_oos_plan_e(vtss_state_t *vtss_state, const vtss_port_no_t port_no)
{
    vtss_port_no_t base_port;
    vtss_phy_ts_blk_id_t blk_id = VTSS_PHY_TS_ANA_BLK_ID_ING_0;
    u32 ip1_chmap[2][8], eth1_chmap[2][8], eth1_next_comp = 0;
    u32 value, i, temp;
    u16 alt_chmask;
    u32 ing_delta = 0;
    u32 egr_delta = 0;
    vtss_rc rc;
    BOOL ingress = FALSE;
    vtss_match_type_t match_mode = MATCH_ENGINE_2_PLAN_E;

    VTSS_I("Entering Mini-PLAN_E Recovery: Port_no: %d \n", port_no);

    VTSS_I("Inside %s\n", __FUNCTION__);
    if ((rc = vtss_phy_ts_base_port_get_priv(vtss_state, port_no, &base_port)) != VTSS_RC_OK) {
        VTSS_I("Error in getting base port\n");
    }
    if (port_no == base_port) {
        alt_chmask = VTSS_PHY_TS_ENG_FLOW_VALID_FOR_CH1;
    } else {
        alt_chmask = VTSS_PHY_TS_ENG_FLOW_VALID_FOR_CH0;
    }

    for (blk_id = VTSS_PHY_TS_ANA_BLK_ID_ING_0; blk_id <= VTSS_PHY_TS_ANA_BLK_ID_EGR_0; blk_id++) {
        /* Disable eth1 flow configuration for shared port */
        for (i = 0; i < 8; i++) {
            value = 0;
            VTSS_RC(VTSS_PHY_TS_READ_CSR(base_port, blk_id, VTSS_ANA_ETH1_FLOW_CFG_ETH1_FLOW_ENABLE(i), &value));
            eth1_chmap[blk_id][i] = value;
            VTSS_I("TSP OOS Recovery Port-no: %d  blk_id: 0x%x,  Saving: VTSS_ANA_ETH1_FLOW_CFG_ETH1_FLOW_ENABLE(%d):: 0x%x :: 0x%x \n",
                   port_no, blk_id, i, VTSS_ANA_ETH1_FLOW_CFG_ETH1_FLOW_ENABLE(i), value);
            value = VTSS_PHY_TS_CLR_BITS(value, (alt_chmask << 8));
            VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_ETH1_FLOW_CFG_ETH1_FLOW_ENABLE(i), &value));
        }

        /* Configure next comparator as PTP in ETH comparator */
        VTSS_RC(VTSS_PHY_TS_READ_CSR(base_port, blk_id, VTSS_ANA_ETH1_NXT_PROTOCOL_ETH1_NXT_PROTOCOL, &value));
        eth1_next_comp = value;
        temp = VTSS_F_ANA_ETH1_NXT_PROTOCOL_ETH1_NXT_PROTOCOL_ETH1_NXT_COMPARATOR(5);
        value = VTSS_PHY_TS_CLR_BITS(value, VTSS_M_ANA_ETH1_NXT_PROTOCOL_ETH1_NXT_PROTOCOL_ETH1_NXT_COMPARATOR) | temp;
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_ETH1_NXT_PROTOCOL_ETH1_NXT_PROTOCOL, &value));

        /* Disable all the IP flows */
        for (i = 0; i < 8; i++) {
            value = 0;
            VTSS_RC(VTSS_PHY_TS_READ_CSR(base_port, blk_id, VTSS_ANA_IP1_FLOW_CFG_IP1_FLOW_ENA(i), &value));
            ip1_chmap[blk_id][i] = value;
            VTSS_I(" TSP OOS recovery Port-no: %d  blk_id: 0x%x, Saving: VTSS_ANA_IP1_FLOW_CFG_IP1_FLOW_ENABLE(%d):: 0x%x :: 0x%x \n",
                   port_no, blk_id, i, VTSS_ANA_IP1_FLOW_CFG_IP1_FLOW_ENA(i), value);
            value = 0;
            VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_IP1_FLOW_CFG_IP1_FLOW_ENA(i), &value));
        }
    }

    /* Flush fifo */
    if (vtss_phy_ts_tesla_tsp_fifo_sync_empty_ts_fifo(vtss_state, port_no, pr, ingress, &temp) != VTSS_RC_OK) {
        VTSS_I("Error Clearing FIFO \n");
    }
    /* Send 300 125 byte matching frames */
    //match_mode = ingress ? MATCH_INGRESS : MATCH_EGRESS;
    match_mode = MATCH_ENGINE_2_PLAN_E;
    if (vtss_phy_ts_tesla_epg_send_300_packets(vtss_state, port_no, pr, 125, match_mode, &ing_delta, &egr_delta) != VTSS_RC_OK) {
        VTSS_I("Error sending 300 matching 125 byte frames \n");
    }
    /* Restore analyzer conf related to IP1 and ETH1 comparators */
    for (blk_id = VTSS_PHY_TS_ANA_BLK_ID_ING_0; blk_id <= VTSS_PHY_TS_ANA_BLK_ID_EGR_0; blk_id++) {
        /* Enable all the IP flows */
        for (i = 0; i < 8; i++) {
            value = ip1_chmap[blk_id][i];
            rc = VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_IP1_FLOW_CFG_IP1_FLOW_ENA(i), &value);
        }
        /* Restore ETH1 next comparator config */
        rc = VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_ETH1_NXT_PROTOCOL_ETH1_NXT_PROTOCOL, &eth1_next_comp);
        /* Restore ETH1 flow mask config */
        for (i = 0; i < 8; i++) {
            value = eth1_chmap[blk_id][i];
            VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_ETH1_FLOW_CFG_ETH1_FLOW_ENABLE(i), &value));
        }
    }
    /* Flush fifo */
    if (vtss_phy_ts_tesla_tsp_fifo_sync_empty_ts_fifo(vtss_state, port_no, pr, ingress, &temp) != VTSS_RC_OK) {
        VTSS_I("Error Clearing FIFO \n");
    }

    VTSS_I("Exiting TSP OOS recovery using only ETH1 and PTP comparators\n");
    return rc;
}

#endif

static vtss_rc vtss_phy_ts_resolve_tsp_cmd_oos_eng_2B_plan_e(vtss_state_t *vtss_state, const vtss_port_no_t port_no,
                                                             vtss_phy_ts_engine_t  eng_minE, const vtss_debug_printf_t pr)
{
    vtss_phy_ts_blk_id_t blk_id = VTSS_PHY_TS_ANA_BLK_ID_ING_2;
    vtss_port_no_t base_port;
    vtss_match_type_t match_mode = MATCH_ENGINE_2_PLAN_E;
    u32 value;
    u16 chmask;
    u16 pkt_sz = 0;
    vtss_rc rc;
    u8  flow_id = 7, i, dir = 0;
    u32 ing_delta = 0, egr_delta = 0;
    u32 eth1_next_proto[2], eth1_tag_mode[2], eth1_type[2], eth1_chmask[2][7];
    u32 eth2_chmask[2][7], mpls_chmask[2][7];

    pr("Port_no: %d --- <<<<<  Entering Mini-PLAN_E Eng_2B Recovery to reset TSP CMD FIFO >>>>>  ---- \n", port_no);
    /* Enable engine 2B in proc block */
    VTSS_RC(VTSS_PHY_TS_READ_CSR(port_no, VTSS_PHY_TS_PROC_BLK_ID_0,
                                 VTSS_PTP_IP_1588_TOP_CFG_STAT_ANALYZER_MODE, &value));
    value = value | VTSS_F_PTP_IP_1588_TOP_CFG_STAT_ANALYZER_MODE_EGR_ENCAP_ENGINE_ENA(4) |
            VTSS_F_PTP_IP_1588_TOP_CFG_STAT_ANALYZER_MODE_INGR_ENCAP_ENGINE_ENA(4);
    VTSS_RC(VTSS_PHY_TS_WRITE_CSR(port_no, VTSS_PHY_TS_PROC_BLK_ID_0,
                                  VTSS_PTP_IP_1588_TOP_CFG_STAT_ANALYZER_MODE, &value));
    value = 0;
    VTSS_RC(VTSS_PHY_TS_READ_CSR(port_no, VTSS_PHY_TS_PROC_BLK_ID_0,
                                 VTSS_PTP_IP_1588_TOP_CFG_STAT_ANALYZER_MODE, &value));
    pr("VTSS_PTP_IP_1588_TOP_CFG_STAT_ANALYZER_MODE:%x port_no:%d\n", value, port_no);

    if ((rc = vtss_phy_ts_base_port_get_priv(vtss_state, port_no, &base_port)) != VTSS_RC_OK) {
        VTSS_I("Error in getting base port\n");
    }
    if (port_no == base_port) {
        chmask = VTSS_PHY_TS_ENG_FLOW_VALID_FOR_CH0;
    } else {
        chmask = VTSS_PHY_TS_ENG_FLOW_VALID_FOR_CH1;
    }

#if 0
    for (blk_id = VTSS_PHY_TS_ANA_BLK_ID_ING_2; blk_id <= VTSS_PHY_TS_ANA_BLK_ID_EGR_2; blk_id++) {
        value = VTSS_F_ANA_OAM_ETH1_NXT_PROTOCOL_A_ETH1_NXT_PROTOCOL_A_ETH1_NXT_COMPARATOR_A(5);
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_ETH2_NXT_PROTOCOL_A_ETH2_NXT_PROTOCOL_A, &value));
        value = VTSS_F_ANA_OAM_ETH1_NXT_PROTOCOL_B_ETH1_NXT_PROTOCOL_B_ETH1_NXT_COMPARATOR_B(5);
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_ETH1_NXT_PROTOCOL_B_ETH1_NXT_PROTOCOL_B, &value));
        value = VTSS_F_ANA_OAM_ETH1_NXT_PROTOCOL_B_ETH1_ETYPE_MATCH_B_ETH1_ETYPE_MATCH_B(0x8902);
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_ETH1_NXT_PROTOCOL_B_ETH1_ETYPE_MATCH_B, &value));
        value = 0x301;
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_ETH2_FLOW_CFG_ETH2_FLOW_ENABLE(0), &value));
        value = 0x10301;
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_ETH1_FLOW_CFG_ETH1_FLOW_ENABLE(4), &value));
        value = 0x10001;
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_MPLS_NXT_COMPARATOR_A_MPLS_NXT_COMPARATOR_A, &value));
        value = 0x3070001;
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_MPLS_FLOW_CFG_MPLS_FLOW_CONTROL(0), &value));
        value = 0x3070001;
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_MPLS_FLOW_CFG_MPLS_FLOW_CONTROL(1), &value));
    }

    VTSS_I("Engine 2A dump before saving the config\n");
    vtss_phy_ts_read_ing_ana_eng_2_reg_priv(vtss_state, port_no, base_port);
    vtss_phy_ts_read_egr_ana_eng_2_reg_priv(vtss_state, port_no, base_port);
    pr("Engine 2B dump before saving the config\n");
    vtss_phy_ts_read_ing_ana_eng_2_reg_priv(vtss_state, port_no, base_port);
    vtss_phy_ts_read_egr_ana_eng_2_reg_priv(vtss_state, port_no, base_port);
#endif

    // Loop over INGRESS(4) and EGRESS(5) for Eng_2
    for (blk_id = VTSS_PHY_TS_ANA_BLK_ID_ING_2; blk_id <= VTSS_PHY_TS_ANA_BLK_ID_EGR_2; blk_id++) {
        /* Save analyzer config */
        dir = blk_id - VTSS_PHY_TS_ANA_BLK_ID_ING_2;
        VTSS_RC(VTSS_PHY_TS_READ_CSR(base_port, blk_id, VTSS_ANA_OAM_ETH1_NXT_PROTOCOL_B_ETH1_NXT_PROTOCOL_B, &eth1_next_proto[dir]));
#if 0
        VTSS_RC(VTSS_PHY_TS_READ_CSR(base_port, blk_id, VTSS_ANA_OAM_ETH2_NXT_PROTOCOL_B_ETH2_NXT_PROTOCOL_B, &eth2_next_proto[dir]));
        VTSS_RC(VTSS_PHY_TS_READ_CSR(base_port, blk_id, VTSS_ANA_OAM_MPLS_NXT_COMPARATOR_B_MPLS_NXT_COMPARATOR_B, &mpls_next_proto[dir]));
#endif
        VTSS_RC(VTSS_PHY_TS_READ_CSR(base_port, blk_id, VTSS_ANA_OAM_ETH1_NXT_PROTOCOL_B_ETH1_TAG_MODE_B, &eth1_tag_mode[dir]));
        VTSS_RC(VTSS_PHY_TS_READ_CSR(base_port, blk_id, VTSS_ANA_OAM_ETH1_NXT_PROTOCOL_B_ETH1_ETYPE_MATCH_B, &eth1_type[dir]));
        for (i = 0; i < 7; i++) {
            VTSS_RC(VTSS_PHY_TS_READ_CSR(base_port, blk_id, VTSS_ANA_OAM_ETH1_FLOW_CFG_ETH1_FLOW_ENABLE(i), &eth1_chmask[dir][i]));
            if (!(eth1_chmask[dir][i] & 0x10000)) {
                pr("ETH1 Flow%d configured for Engine-2A blk_id:%d\n", i, blk_id);
                value = eth1_chmask[dir][i];
                value = VTSS_PHY_TS_CLR_BITS(value, (chmask << 8));
                VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_ETH1_FLOW_CFG_ETH1_FLOW_ENABLE(i), &value));
            } else {
                pr("ETH1 Flow%d configured for Engine-2B blk_id:%d\n", i, blk_id);
                value = 0;
                VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_ETH1_FLOW_CFG_ETH1_FLOW_ENABLE(i), &value));
            }
            /* Save & DIsable Eth2 Flow */
            VTSS_RC(VTSS_PHY_TS_READ_CSR(base_port, blk_id, VTSS_ANA_OAM_ETH2_FLOW_CFG_ETH2_FLOW_ENABLE(i), &eth2_chmask[dir][i]));
            if (!(eth2_chmask[dir][i] & 0x10000)) {
                VTSS_I("ETH2 Flow%d configured for Engine-2A blk_id:%d\n", i, blk_id);
                value = eth2_chmask[dir][i];
                value = VTSS_PHY_TS_CLR_BITS(value, (chmask << 8));
                VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_ETH2_FLOW_CFG_ETH2_FLOW_ENABLE(i), &value));
            } else {
                pr("ETH2 Flow%d configured for Engine-2B blk_id:%d\n", i, blk_id);
                value = 0;
                VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_ETH2_FLOW_CFG_ETH2_FLOW_ENABLE(i), &value));
            }
            /* Save & DIsable MPLS Flow */
            VTSS_RC(VTSS_PHY_TS_READ_CSR(base_port, blk_id, VTSS_ANA_OAM_MPLS_FLOW_CFG_MPLS_FLOW_CONTROL(i), &mpls_chmask[dir][i]));
            value = 0;
            VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_MPLS_FLOW_CFG_MPLS_FLOW_CONTROL(i), &value));
        }
#if 0
        /* Disable ETH2 & MPLS Comparator */
        value = 0;
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_ETH2_NXT_PROTOCOL_B_ETH2_NXT_PROTOCOL_B, &value));
        value = 0;
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_MPLS_NXT_COMPARATOR_B_MPLS_NXT_COMPARATOR_B, &value));
#endif
        /* Configure engine 2B eth1 comparator */
        value = VTSS_F_ANA_OAM_ETH1_NXT_PROTOCOL_B_ETH1_NXT_PROTOCOL_B_ETH1_NXT_COMPARATOR_B(5);
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_ETH1_NXT_PROTOCOL_B_ETH1_NXT_PROTOCOL_B, &value));
        value = 0;
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_ETH1_NXT_PROTOCOL_B_ETH1_TAG_MODE_B, &value));
        value = VTSS_F_ANA_OAM_ETH1_NXT_PROTOCOL_B_ETH1_ETYPE_MATCH_B_ETH1_ETYPE_MATCH_B(0x800);
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_ETH1_NXT_PROTOCOL_B_ETH1_ETYPE_MATCH_B, &value));
        /* configure flow 7 for engine 2B */
        flow_id = 7;
        value = 0;
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_ETH1_FLOW_CFG_ETH1_MATCH_MODE(flow_id), &value));
        /* Configure address to be matched */
        value = 0xffffffff;
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_ETH1_FLOW_CFG_ETH1_ADDR_MATCH_1(flow_id), &value));
        value = VTSS_F_ANA_OAM_ETH1_FLOW_CFG_ETH1_ADDR_MATCH_2_ETH1_ADDR_MATCH_MODE(VTSS_PHY_TS_ETH_ADDR_MATCH_48BIT);
        if (blk_id == VTSS_PHY_TS_ANA_BLK_ID_ING_2) {
            value |= VTSS_F_ANA_OAM_ETH1_FLOW_CFG_ETH1_ADDR_MATCH_2_ETH1_ADDR_MATCH_SELECT(VTSS_PHY_TS_ETH_MATCH_DEST_ADDR) |
                     VTSS_F_ANA_OAM_ETH1_FLOW_CFG_ETH1_ADDR_MATCH_2_ETH1_ADDR_MATCH_2(0xfff4);
        } else {
            value |= VTSS_F_ANA_OAM_ETH1_FLOW_CFG_ETH1_ADDR_MATCH_2_ETH1_ADDR_MATCH_SELECT(VTSS_PHY_TS_ETH_MATCH_SRC_ADDR) |
                     VTSS_F_ANA_OAM_ETH1_FLOW_CFG_ETH1_ADDR_MATCH_2_ETH1_ADDR_MATCH_2(0xfff3);
        }
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_ETH1_FLOW_CFG_ETH1_ADDR_MATCH_2(flow_id), &value));
        value = 0;
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_ETH1_FLOW_CFG_ETH1_VLAN_TAG1(flow_id), &value));
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_ETH1_FLOW_CFG_ETH1_VLAN_TAG2_I_TAG(flow_id), &value));
        /* Enable the flow */
        //value = VTSS_F_ANA_OAM_ETH1_FLOW_CFG_ETH1_FLOW_ENABLE_ETH1_FLOW_ENABLE | VTSS_F_ANA_OAM_ETH1_FLOW_CFG_ETH1_FLOW_ENABLE_ETH1_CHANNEL_MASK(chmask);
        //VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_ETH1_FLOW_CFG_ETH1_FLOW_ENABLE(flow_id), &value));

        /* Delay Enable of the flow until PTP comparitor has been configured */
        /* Delay Actually Enabling Flow_7 until all has been configured */

        /* Configure PTP comparator */
        flow_id = 5;
        value = 0;
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_PTP_FLOW_PTP_FLOW_MATCH_UPPER(flow_id), &value));
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_PTP_FLOW_PTP_FLOW_MATCH_LOWER(flow_id), &value));
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_PTP_FLOW_PTP_FLOW_MASK_UPPER(flow_id), &value));
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_PTP_FLOW_PTP_FLOW_MASK_LOWER(flow_id), &value));
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_PTP_FLOW_PTP_DOMAIN_RANGE(flow_id), &value));

        value = VTSS_F_ANA_OAM_PTP_FLOW_PTP_ACTION_PTP_COMMAND(PTP_ACTION_CMD_WRITE_NS);
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_PTP_FLOW_PTP_ACTION(flow_id), &value));
        if (blk_id == VTSS_PHY_TS_ANA_BLK_ID_ING_2) {
            value = VTSS_F_ANA_PTP_FLOW_PTP_ACTION_2_PTP_REWRITE_OFFSET(28);
        } else {
            value = VTSS_F_ANA_PTP_FLOW_PTP_ACTION_2_PTP_REWRITE_OFFSET(20);
        }
        value |= VTSS_F_ANA_PTP_FLOW_PTP_ACTION_2_PTP_REWRITE_BYTES(4);
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_PTP_FLOW_PTP_ACTION_2(flow_id), &value));

        /* Now Actually Enable the Flows:  Delay Actually Enabling Flow_7 and PTP Flow_5 until all has been configured */
        /* Enable the flow 7 for engine 2b */
        flow_id = 7;
        value = 0;
        value = VTSS_F_ANA_OAM_ETH1_FLOW_CFG_ETH1_FLOW_ENABLE_ETH1_FLOW_ENABLE | VTSS_F_ANA_OAM_ETH1_FLOW_CFG_ETH1_FLOW_ENABLE_ETH1_CHANNEL_MASK(chmask) | VTSS_F_ANA_OAM_ETH1_FLOW_CFG_ETH1_FLOW_ENABLE_ETH1_NXT_PROT_GRP_SEL;
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_ETH1_FLOW_CFG_ETH1_FLOW_ENABLE(flow_id), &value));

        /* Enable PTP flow for PTP comparator */
        flow_id = 5;
        value = 0;
        value = VTSS_F_ANA_OAM_PTP_FLOW_PTP_FLOW_ENA_PTP_NXT_PROT_GRP_MASK(2) |
                VTSS_F_ANA_OAM_PTP_FLOW_PTP_FLOW_ENA_PTP_CHANNEL_MASK(chmask) |
                VTSS_F_ANA_OAM_PTP_FLOW_PTP_FLOW_ENA_PTP_FLOW_ENA;
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_PTP_FLOW_PTP_FLOW_ENA(flow_id), &value));
    }

#if 0
    VTSS_I("Engine 2B dump after saving the config\n");
    vtss_phy_ts_read_ing_ana_eng_2_reg_priv(vtss_state, port_no, base_port);
    vtss_phy_ts_read_egr_ana_eng_2_reg_priv(vtss_state, port_no, base_port);
#endif
    ing_delta = egr_delta = 0;
    pkt_sz = 64;
    /* Send 300 64 byte matching frames of Plan E type*/
    match_mode = MATCH_ENGINE_2_PLAN_E;
    VTSS_I(" Send 300 - %d byte matching frames for plan_E: MATCH_MODE = 0x%x (MATCH_ENGINE_2_PLAN_E) \n", pkt_sz, match_mode);
    if (vtss_phy_ts_tesla_epg_send_300_packets(vtss_state, port_no, pr, pkt_sz, match_mode, &ing_delta, &egr_delta) != VTSS_RC_OK) {
        VTSS_I("Error sending 300 matching %d byte frames \n", pkt_sz);
    }

    /* Restore analyzer config */
    for (blk_id = VTSS_PHY_TS_ANA_BLK_ID_ING_2; blk_id <= VTSS_PHY_TS_ANA_BLK_ID_EGR_2; blk_id++) {
        /* Disable flow 7 in ETH1 & flow 5 in PTP comparators respectively */
        flow_id = 7;
        value = 0x300;
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_ETH1_FLOW_CFG_ETH1_FLOW_ENABLE(flow_id), &value));
        flow_id = 5;
        value = 0x30030;
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_PTP_FLOW_PTP_FLOW_ENA(flow_id), &value));

        dir = blk_id - VTSS_PHY_TS_ANA_BLK_ID_ING_2;
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_ETH1_NXT_PROTOCOL_B_ETH1_NXT_PROTOCOL_B, &eth1_next_proto[dir]));
#if 0
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_ETH2_NXT_PROTOCOL_B_ETH2_NXT_PROTOCOL_B, &eth2_next_proto[dir]));
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_MPLS_NXT_COMPARATOR_B_MPLS_NXT_COMPARATOR_B, &mpls_next_proto[dir]));
#endif
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_ETH1_NXT_PROTOCOL_B_ETH1_TAG_MODE_B, &eth1_tag_mode[dir]));
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_ETH1_NXT_PROTOCOL_B_ETH1_ETYPE_MATCH_B, &eth1_type[dir]));
        for (i = 0; i < 7; i++) {
            VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_ETH1_FLOW_CFG_ETH1_FLOW_ENABLE(i), &eth1_chmask[dir][i]));
            VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_ETH2_FLOW_CFG_ETH2_FLOW_ENABLE(i), &eth2_chmask[dir][i]));
            VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_MPLS_FLOW_CFG_MPLS_FLOW_CONTROL(i), &mpls_chmask[dir][i]));
        }
    }
#if 0
    pr("Engine 2B dump after restoring the config\n");
    vtss_phy_ts_read_ing_ana_eng_2_reg_priv(vtss_state, port_no, base_port);
    vtss_phy_ts_read_egr_ana_eng_2_reg_priv(vtss_state, port_no, base_port);
#endif
    VTSS_I("Port_no: %d --- <<<<<  Exit Mini-PLAN_E Eng_2B Recovery to reset TSP CMD FIFO >>>>>  ---- \n", port_no);
    pr("Executed Mini-planE recovery\n");
    return VTSS_RC_OK;
}

static vtss_rc vtss_phy_ts_resolve_tsp_cmd_oos_eng_2A_plan_e(vtss_state_t *vtss_state, const vtss_port_no_t port_no,
                                                             vtss_phy_ts_engine_t  eng_minE, const vtss_debug_printf_t pr)
{
    vtss_phy_ts_blk_id_t blk_id = VTSS_PHY_TS_ANA_BLK_ID_ING_2;
    vtss_port_no_t base_port;
    vtss_match_type_t match_mode = MATCH_ENGINE_2_PLAN_E;
    u32 value;
    u16 chmask;
    u16 pkt_sz = 0;
    vtss_rc rc;
    u8  flow_id = 7, i, dir = 0;
    u32 ing_delta = 0, egr_delta = 0;
    u32 eth1_next_proto[2], eth1_tag_mode[2], eth1_type[2], eth1_chmask[2][7];
    u32 eth2_next_proto[2], mpls_next_proto[2], eth2_chmask[2][7], mpls_chmask[2][7];

    VTSS_I("Port_no: %d --- <<<<<  Entering Mini-PLAN_E Eng_2A Recovery to reset TSP CMD FIFO >>>>>  ---- \n", port_no);
    /* Enable engine 2A in proc block */
    VTSS_RC(VTSS_PHY_TS_READ_CSR(port_no, VTSS_PHY_TS_PROC_BLK_ID_0,
                                 VTSS_PTP_IP_1588_TOP_CFG_STAT_ANALYZER_MODE, &value));
    value = value | VTSS_F_PTP_IP_1588_TOP_CFG_STAT_ANALYZER_MODE_EGR_ENCAP_ENGINE_ENA(4) |
            VTSS_F_PTP_IP_1588_TOP_CFG_STAT_ANALYZER_MODE_INGR_ENCAP_ENGINE_ENA(4);
    VTSS_RC(VTSS_PHY_TS_WRITE_CSR(port_no, VTSS_PHY_TS_PROC_BLK_ID_0,
                                  VTSS_PTP_IP_1588_TOP_CFG_STAT_ANALYZER_MODE, &value));
    value = 0;
    VTSS_RC(VTSS_PHY_TS_READ_CSR(port_no, VTSS_PHY_TS_PROC_BLK_ID_0,
                                 VTSS_PTP_IP_1588_TOP_CFG_STAT_ANALYZER_MODE, &value));
    pr("VTSS_PTP_IP_1588_TOP_CFG_STAT_ANALYZER_MODE:%x port_no:%d\n", value, port_no);

    if ((rc = vtss_phy_ts_base_port_get_priv(vtss_state, port_no, &base_port)) != VTSS_RC_OK) {
        VTSS_I("Error in getting base port\n");
    }
    if (port_no == base_port) {
        chmask = VTSS_PHY_TS_ENG_FLOW_VALID_FOR_CH0;
    } else {
        chmask = VTSS_PHY_TS_ENG_FLOW_VALID_FOR_CH1;
    }

#if 0
    for (blk_id = VTSS_PHY_TS_ANA_BLK_ID_ING_2; blk_id <= VTSS_PHY_TS_ANA_BLK_ID_EGR_2; blk_id++) {
        value = VTSS_F_ANA_OAM_ETH1_NXT_PROTOCOL_A_ETH1_NXT_PROTOCOL_A_ETH1_NXT_COMPARATOR_A(5);
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_ETH2_NXT_PROTOCOL_A_ETH2_NXT_PROTOCOL_A, &value));
        value = VTSS_F_ANA_OAM_ETH1_NXT_PROTOCOL_B_ETH1_NXT_PROTOCOL_B_ETH1_NXT_COMPARATOR_B(5);
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_ETH1_NXT_PROTOCOL_B_ETH1_NXT_PROTOCOL_B, &value));
        value = VTSS_F_ANA_OAM_ETH1_NXT_PROTOCOL_B_ETH1_ETYPE_MATCH_B_ETH1_ETYPE_MATCH_B(0x8902);
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_ETH1_NXT_PROTOCOL_B_ETH1_ETYPE_MATCH_B, &value));
        value = 0x301;
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_ETH2_FLOW_CFG_ETH2_FLOW_ENABLE(0), &value));
        value = 0x10301;
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_ETH1_FLOW_CFG_ETH1_FLOW_ENABLE(4), &value));
        value = 0x10001;
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_MPLS_NXT_COMPARATOR_A_MPLS_NXT_COMPARATOR_A, &value));
        value = 0x3070001;
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_MPLS_FLOW_CFG_MPLS_FLOW_CONTROL(0), &value));
        value = 0x3070001;
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_MPLS_FLOW_CFG_MPLS_FLOW_CONTROL(1), &value));
    }

    VTSS_I("Engine 2A dump before saving the config\n");
    vtss_phy_ts_read_ing_ana_eng_2_reg_priv(vtss_state, port_no, base_port);
    vtss_phy_ts_read_egr_ana_eng_2_reg_priv(vtss_state, port_no, base_port);
    pr("Engine 2A dump before saving the config\n");
    vtss_phy_ts_read_ing_ana_eng_2_reg_priv(vtss_state, port_no, base_port);
    vtss_phy_ts_read_egr_ana_eng_2_reg_priv(vtss_state, port_no, base_port);
#endif

    // Loop over INGRESS(4) and EGRESS(5) for Eng_2
    for (blk_id = VTSS_PHY_TS_ANA_BLK_ID_ING_2; blk_id <= VTSS_PHY_TS_ANA_BLK_ID_EGR_2; blk_id++) {
        /* Save analyzer config */
        dir = blk_id - VTSS_PHY_TS_ANA_BLK_ID_ING_2;
        VTSS_RC(VTSS_PHY_TS_READ_CSR(base_port, blk_id, VTSS_ANA_OAM_ETH1_NXT_PROTOCOL_A_ETH1_NXT_PROTOCOL_A, &eth1_next_proto[dir]));
        VTSS_RC(VTSS_PHY_TS_READ_CSR(base_port, blk_id, VTSS_ANA_OAM_ETH2_NXT_PROTOCOL_A_ETH2_NXT_PROTOCOL_A, &eth2_next_proto[dir]));
        VTSS_RC(VTSS_PHY_TS_READ_CSR(base_port, blk_id, VTSS_ANA_OAM_MPLS_NXT_COMPARATOR_A_MPLS_NXT_COMPARATOR_A, &mpls_next_proto[dir]));
        VTSS_RC(VTSS_PHY_TS_READ_CSR(base_port, blk_id, VTSS_ANA_OAM_ETH1_NXT_PROTOCOL_A_ETH1_TAG_MODE_A, &eth1_tag_mode[dir]));
        VTSS_RC(VTSS_PHY_TS_READ_CSR(base_port, blk_id, VTSS_ANA_OAM_ETH1_NXT_PROTOCOL_A_ETH1_ETYPE_MATCH_A, &eth1_type[dir]));
        for (i = 0; i < 7; i++) {
            VTSS_RC(VTSS_PHY_TS_READ_CSR(base_port, blk_id, VTSS_ANA_OAM_ETH1_FLOW_CFG_ETH1_FLOW_ENABLE(i), &eth1_chmask[dir][i]));
            if (eth1_chmask[dir][i] & 0x10000) {
                VTSS_I("ETH1 Flow%d configured for Engine-2B blk_id:%d\n", i, blk_id);
                value = eth1_chmask[dir][i];
                value = VTSS_PHY_TS_CLR_BITS(value, (chmask << 8));
                VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_ETH1_FLOW_CFG_ETH1_FLOW_ENABLE(i), &value));
            } else {
                value = 0;
                VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_ETH1_FLOW_CFG_ETH1_FLOW_ENABLE(i), &value));
            }
            /* Save & DIsable Eth2 Flow */
            VTSS_RC(VTSS_PHY_TS_READ_CSR(base_port, blk_id, VTSS_ANA_OAM_ETH2_FLOW_CFG_ETH2_FLOW_ENABLE(i), &eth2_chmask[dir][i]));
            if (eth2_chmask[dir][i] & 0x10000) {
                VTSS_I("ETH2 Flow%d configured for Engine-2B blk_id:%d\n", i, blk_id);
                value = eth2_chmask[dir][i];
                value = VTSS_PHY_TS_CLR_BITS(value, (chmask << 8));
                VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_ETH2_FLOW_CFG_ETH2_FLOW_ENABLE(i), &value));
            } else {
                value = 0;
                VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_ETH2_FLOW_CFG_ETH2_FLOW_ENABLE(i), &value));
            }
            /* Save & DIsable MPLS Flow */
            VTSS_RC(VTSS_PHY_TS_READ_CSR(base_port, blk_id, VTSS_ANA_OAM_MPLS_FLOW_CFG_MPLS_FLOW_CONTROL(i), &mpls_chmask[dir][i]));
            value = 0;
            VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_MPLS_FLOW_CFG_MPLS_FLOW_CONTROL(i), &value));
        }
        /* Disable ETH2 & MPLS Comparator */
        value = 0;
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_ETH2_NXT_PROTOCOL_A_ETH2_NXT_PROTOCOL_A, &value));
        value = 0;
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_MPLS_NXT_COMPARATOR_A_MPLS_NXT_COMPARATOR_A, &value));

        /* Configure engine 2A eth1 comparator */
        value = VTSS_F_ANA_OAM_ETH1_NXT_PROTOCOL_A_ETH1_NXT_PROTOCOL_A_ETH1_NXT_COMPARATOR_A(5);
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_ETH1_NXT_PROTOCOL_A_ETH1_NXT_PROTOCOL_A, &value));
        value = 0;
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_ETH1_NXT_PROTOCOL_A_ETH1_TAG_MODE_A, &value));
        value = VTSS_F_ANA_OAM_ETH1_NXT_PROTOCOL_A_ETH1_ETYPE_MATCH_A_ETH1_ETYPE_MATCH_A(0x800);
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_ETH1_NXT_PROTOCOL_A_ETH1_ETYPE_MATCH_A, &value));
        /* configure flow 7 for engine 2A */
        flow_id = 7;
        value = 0;
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_ETH1_FLOW_CFG_ETH1_MATCH_MODE(flow_id), &value));
        /* Configure address to be matched */
        value = 0xffffffff;
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_ETH1_FLOW_CFG_ETH1_ADDR_MATCH_1(flow_id), &value));
        value = VTSS_F_ANA_OAM_ETH1_FLOW_CFG_ETH1_ADDR_MATCH_2_ETH1_ADDR_MATCH_MODE(VTSS_PHY_TS_ETH_ADDR_MATCH_48BIT);
        if (blk_id == VTSS_PHY_TS_ANA_BLK_ID_ING_2) {
            value |= VTSS_F_ANA_OAM_ETH1_FLOW_CFG_ETH1_ADDR_MATCH_2_ETH1_ADDR_MATCH_SELECT(VTSS_PHY_TS_ETH_MATCH_DEST_ADDR) |
                     VTSS_F_ANA_OAM_ETH1_FLOW_CFG_ETH1_ADDR_MATCH_2_ETH1_ADDR_MATCH_2(0xfff4);
        } else {
            value |= VTSS_F_ANA_OAM_ETH1_FLOW_CFG_ETH1_ADDR_MATCH_2_ETH1_ADDR_MATCH_SELECT(VTSS_PHY_TS_ETH_MATCH_SRC_ADDR) |
                     VTSS_F_ANA_OAM_ETH1_FLOW_CFG_ETH1_ADDR_MATCH_2_ETH1_ADDR_MATCH_2(0xfff3);
        }
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_ETH1_FLOW_CFG_ETH1_ADDR_MATCH_2(flow_id), &value));
        value = 0;
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_ETH1_FLOW_CFG_ETH1_VLAN_TAG1(flow_id), &value));
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_ETH1_FLOW_CFG_ETH1_VLAN_TAG2_I_TAG(flow_id), &value));
        /* Enable the flow */
        //value = VTSS_F_ANA_OAM_ETH1_FLOW_CFG_ETH1_FLOW_ENABLE_ETH1_FLOW_ENABLE | VTSS_F_ANA_OAM_ETH1_FLOW_CFG_ETH1_FLOW_ENABLE_ETH1_CHANNEL_MASK(chmask);
        //VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_ETH1_FLOW_CFG_ETH1_FLOW_ENABLE(flow_id), &value));

        /* Delay Enable of the flow until PTP comparitor has been configured */
        /* Delay Actually Enabling Flow_7 until all has been configured */

        /* Configure PTP comparator */
        flow_id = 5;
        value = 0;
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_PTP_FLOW_PTP_FLOW_MATCH_UPPER(flow_id), &value));
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_PTP_FLOW_PTP_FLOW_MATCH_LOWER(flow_id), &value));
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_PTP_FLOW_PTP_FLOW_MASK_UPPER(flow_id), &value));
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_PTP_FLOW_PTP_FLOW_MASK_LOWER(flow_id), &value));
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_PTP_FLOW_PTP_DOMAIN_RANGE(flow_id), &value));

        value = VTSS_F_ANA_OAM_PTP_FLOW_PTP_ACTION_PTP_COMMAND(PTP_ACTION_CMD_WRITE_NS);
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_PTP_FLOW_PTP_ACTION(flow_id), &value));
        if (blk_id == VTSS_PHY_TS_ANA_BLK_ID_ING_2) {
            value = VTSS_F_ANA_PTP_FLOW_PTP_ACTION_2_PTP_REWRITE_OFFSET(28);
        } else {
            value = VTSS_F_ANA_PTP_FLOW_PTP_ACTION_2_PTP_REWRITE_OFFSET(20);
        }
        value |= VTSS_F_ANA_PTP_FLOW_PTP_ACTION_2_PTP_REWRITE_BYTES(4);
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_PTP_FLOW_PTP_ACTION_2(flow_id), &value));

        /* Now Actually Enable the Flows:  Delay Actually Enabling Flow_7 and PTP Flow_5 until all has been configured */
        /* Enable the flow 7 for engine 2A */
        flow_id = 7;
        value = 0;
        value = VTSS_F_ANA_OAM_ETH1_FLOW_CFG_ETH1_FLOW_ENABLE_ETH1_FLOW_ENABLE | VTSS_F_ANA_OAM_ETH1_FLOW_CFG_ETH1_FLOW_ENABLE_ETH1_CHANNEL_MASK(chmask);
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_ETH1_FLOW_CFG_ETH1_FLOW_ENABLE(flow_id), &value));

        /* Enable PTP flow for PTP comparator */
        flow_id = 5;
        value = 0;
        value = VTSS_F_ANA_OAM_PTP_FLOW_PTP_FLOW_ENA_PTP_NXT_PROT_GRP_MASK(1) |
                VTSS_F_ANA_OAM_PTP_FLOW_PTP_FLOW_ENA_PTP_CHANNEL_MASK(chmask) |
                VTSS_F_ANA_OAM_PTP_FLOW_PTP_FLOW_ENA_PTP_FLOW_ENA;
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_PTP_FLOW_PTP_FLOW_ENA(flow_id), &value));
    }

#if 0
    VTSS_I("Engine 2A dump after saving the config\n");
    vtss_phy_ts_read_ing_ana_eng_2_reg_priv(vtss_state, port_no, base_port);
    vtss_phy_ts_read_egr_ana_eng_2_reg_priv(vtss_state, port_no, base_port);
#endif
    ing_delta = egr_delta = 0;
    pkt_sz = 64;
    /* Send 300 64 byte matching frames of Plan E type*/
    match_mode = MATCH_ENGINE_2_PLAN_E;
    VTSS_I(" Send 300 - %d byte matching frames for plan_E: MATCH_MODE = 0x%x (MATCH_ENGINE_2_PLAN_E) \n", pkt_sz, match_mode);
    if (vtss_phy_ts_tesla_epg_send_300_packets(vtss_state, port_no, pr, pkt_sz, match_mode, &ing_delta, &egr_delta) != VTSS_RC_OK) {
        VTSS_I("Error sending 300 matching %d byte frames \n", pkt_sz);
    }

    /* Restore analyzer config */
    for (blk_id = VTSS_PHY_TS_ANA_BLK_ID_ING_2; blk_id <= VTSS_PHY_TS_ANA_BLK_ID_EGR_2; blk_id++) {
        /* Disable flow 7 in ETH1 & flow 5 in PTP comparators respectively */
        flow_id = 7;
        value = 0x300;
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_ETH1_FLOW_CFG_ETH1_FLOW_ENABLE(flow_id), &value));
        flow_id = 5;
        value = 0x30030;
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_PTP_FLOW_PTP_FLOW_ENA(flow_id), &value));

        dir = blk_id - VTSS_PHY_TS_ANA_BLK_ID_ING_2;
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_ETH1_NXT_PROTOCOL_A_ETH1_NXT_PROTOCOL_A, &eth1_next_proto[dir]));
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_ETH2_NXT_PROTOCOL_A_ETH2_NXT_PROTOCOL_A, &eth2_next_proto[dir]));
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_MPLS_NXT_COMPARATOR_A_MPLS_NXT_COMPARATOR_A, &mpls_next_proto[dir]));
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_ETH1_NXT_PROTOCOL_A_ETH1_TAG_MODE_A, &eth1_tag_mode[dir]));
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_ETH1_NXT_PROTOCOL_A_ETH1_ETYPE_MATCH_A, &eth1_type[dir]));
        for (i = 0; i < 7; i++) {
            VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_ETH1_FLOW_CFG_ETH1_FLOW_ENABLE(i), &eth1_chmask[dir][i]));
            VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_ETH2_FLOW_CFG_ETH2_FLOW_ENABLE(i), &eth2_chmask[dir][i]));
            VTSS_RC(VTSS_PHY_TS_WRITE_CSR(base_port, blk_id, VTSS_ANA_OAM_MPLS_FLOW_CFG_MPLS_FLOW_CONTROL(i), &mpls_chmask[dir][i]));
        }
    }
#if 0
    pr("Engine 2A dump after restoring the config\n");
    vtss_phy_ts_read_ing_ana_eng_2_reg_priv(vtss_state, port_no, base_port);
    vtss_phy_ts_read_egr_ana_eng_2_reg_priv(vtss_state, port_no, base_port);
#endif
    VTSS_I("Port_no: %d --- <<<<<  Exit Mini-PLAN_E Eng_2A Recovery to reset TSP CMD FIFO >>>>>  ---- \n", port_no);
    pr("Executed Mini-planE recovery\n");
    return VTSS_RC_OK;
}

static vtss_rc vtss_phy_ts_resolve_tsp_cmd_oos_eng_2_plan_e(vtss_state_t *vtss_state,
                                                            const vtss_port_no_t port_no,
                                                            vtss_phy_ts_engine_t  eng_minE,
                                                            const vtss_debug_printf_t pr)
{
    vtss_rc rc;

    if (eng_minE == VTSS_PHY_TS_OAM_ENGINE_ID_2B) {
        rc = vtss_phy_ts_resolve_tsp_cmd_oos_eng_2B_plan_e(vtss_state, port_no,
                                                           VTSS_PHY_TS_OAM_ENGINE_ID_2B, pr);
    } else {
        rc = vtss_phy_ts_resolve_tsp_cmd_oos_eng_2A_plan_e(vtss_state, port_no,
                                                           VTSS_PHY_TS_OAM_ENGINE_ID_2A, pr);

    }
    return rc;
}
#endif

//2.    Prep_0:
//a.    Use EPG to send 300 non matching Ethernet-only frames of size 125B min IPG.
//b.    Flush fifo
//c.    If Ingress and FIFO is non empty (CMD FIFO OOS) - goto Min_PlanE_recovery
//  The following Algorythm was developed/implemented in the Engress and Ingress Recovery
static vtss_rc vtss_phy_ts_tesla_tsp_fifo_sync_prep_stage_0(vtss_state_t *vtss_state, vtss_port_no_t  port_no, const vtss_debug_printf_t pr, BOOL ingress, u32 *entry)
{
    vtss_rc            rc = VTSS_RC_OK;
    u16                pkt_sz = 125;
    u32                ing_delta = 0;
    u32                egr_delta = 0;
    vtss_match_type_t match_mode = NO_MATCH;

    VTSS_I("Port_no: %d, Enter Prep Stage_0  ING/EGR= %s \n", port_no, (ingress ? "INGRESS" : "EGRESS"));
    *entry = 0;

    pkt_sz = 125;
    VTSS_I("Prep0_Step_1: Config EPG to send 300 Non-Matching Frames-(%d byte) INT SOF Packets (UDP) - Pattern = 0x013F \n", pkt_sz);
    match_mode = ingress ? NO_MATCH_INGRESS : NO_MATCH;
    if ((rc = vtss_phy_ts_tesla_epg_send_300_packets(vtss_state, port_no, pr, pkt_sz, match_mode, &ing_delta, &egr_delta)) != VTSS_RC_OK) {
        VTSS_E("Error - FAILURE detected sending 300 small Non-Matching Frames \n");
    }

    VTSS_I("Prep0_Step_2: Flush FIFO  \n");
    if ((rc = vtss_phy_ts_tesla_tsp_fifo_sync_empty_ts_fifo(vtss_state, port_no, pr, ingress, entry)) != VTSS_RC_OK) {
        VTSS_E("Error Clearing FIFO\n");
    }

    return (rc);
}

// See TN1327 for full Algorithm
//3.    Detect:
//a.    Use EPG to send 300 matching IP frames of size 125B min IPG.
//b.    Check fifo and compute TS distance. Check off by 0,1,2,3..
//c.    Read old_count before and new_count after RW_MODFRM_CNT (take care of rollover of RW_MODFRM_CNT at 2^32).
//        Difference should be 300 (If RW_MOD_FRM_CNT != 300 then TSP_CMD_FIFO is OOS go to TSP_ODD_recovery
//d.    If OFF_TS: If TS distance are OK go to next step else go to Recovery
//e.    If RW_MODFRM_CNT=300 exit (all working), else, go to Odd recovery in 2nd iteration.
static vtss_rc vtss_phy_ts_tesla_tsp_fifo_sync_detect_stage_1(vtss_state_t *vtss_state, vtss_port_no_t  port_no, const vtss_debug_printf_t       pr, u16 channel, BOOL ingress, BOOL *oos, BOOL *tsp_odd_recovery, u16 *discIndex)
{
    u16                fifodepth = 0;
    BOOL               ing_OOS = FALSE;
    BOOL               egr_OOS = FALSE;
    vtss_rc            rc = VTSS_RC_OK;
    //u16                pkt_cnt = 0;
    u32                ing_delta = 0;
    u32                egr_delta = 0;
    vtss_match_type_t match_mode = NO_MATCH;

    *oos = FALSE;
    *tsp_odd_recovery = FALSE;
    *discIndex = 0;

    VTSS_I("Entering Detect_Stage_1: Port_no: %d,  ING/EGR= %s \n", port_no, (ingress ? "INGRESS" : "EGRESS"));
    //pkt_cnt = 8;
    //VTSS_I("Step 4: Config EPG to send 10 Frames-(10000 byte) Matching Packets (UDP) SOF=Int - Pattern = 0x013F ");
    // vtss_phy_ts_tesla_epg_send_jumbo_packets_8051(vtss_port_no_t   port_no, u16   channel, BOOL  ingress, u16 sof_toggle_pkt, BOOL match)
    //VTSS_RC(vtss_phy_ts_tesla_epg_send_jumbo_packets_8051(vtss_state, port_no,pr, channel, ingress, pkt_cnt, TRUE));
    VTSS_I("Detect1_Step_1: Config EPG to send 300 Frames-(125 byte) Matching Packets (UDP) SOF=Int - Pattern = 0x013F \n");
    match_mode = ingress ? MATCH_INGRESS : MATCH_EGRESS;
    if ((rc = vtss_phy_ts_tesla_epg_send_300_packets(vtss_state, port_no, pr, 125, match_mode, &ing_delta, &egr_delta)) != VTSS_RC_OK) {
        VTSS_E("Error in Detect1_Step1  %d \n", port_no);
        return (VTSS_RC_ERROR);
    }

    VTSS_I("Detect1_Step_2: 1588_TS_Packet_Counters:: ing_delta: %d,  egr_delta: %d  \n", ing_delta, egr_delta);
    if (ingress) {
        if (ing_delta == TESLA_EPG_300_PACKETS) {
            *tsp_odd_recovery = FALSE;
        } else {
            *tsp_odd_recovery = TRUE;
        }

        VTSS_I("Detect1_Step_2a:INGRESS:: 1588_TS_Packet_Counters: %d: Reading Timestamps, Compare Packet-to-Packet Gaps  \n", ing_delta);
        if ((rc = vtss_phy_ts_tesla_tsp_fifo_sync_read_ingress_ts_fifo(vtss_state, port_no, pr, &ing_OOS, &fifodepth, TRUE, discIndex)) != VTSS_RC_OK) {
            VTSS_E("Error in INGRESS Detect1_Step_2a  %d \n", port_no);
            return (VTSS_RC_ERROR);
        }
        *oos = ing_OOS;
    } else {
        if (egr_delta == TESLA_EPG_300_PACKETS) {
            *tsp_odd_recovery = FALSE;
        } else {
            *tsp_odd_recovery = TRUE;
        }

        VTSS_I("Detect1_Step_2a:EGRESS:: 1588_TS_Packet_Counters: %d: Reading Timestamps, Compare Packet-to-Packet Gaps  \n", ing_delta);
        if ((rc = vtss_phy_ts_tesla_tsp_fifo_sync_read_egress_ts_fifo(vtss_state, port_no, pr, &egr_OOS, &fifodepth, TRUE, discIndex)) != VTSS_RC_OK) {
            VTSS_E("Error in EGRESS Detect1_Step_2a  %d \n", port_no);
            return (VTSS_RC_ERROR);
        }
        *oos = egr_OOS;
    }

    pr("Detect-: Port_no: %d,  tsp_odd_recovery: %x;  ING/EGR= %s; OOS: 0x%x, DiscIndex: %d \n", port_no, *tsp_odd_recovery, (ingress ? "INGRESS" : "EGRESS"), *oos, *discIndex);

    return VTSS_RC_OK;
}

// See TN1327 for full Algorithm
//4.    Recovery:
//a.    Use EPG to send 300 non matching Ethernet-only frames of size 125B min IPG.
//b.    Use Micro to send exact number of non matching jumbo frames with max IPG
//          computed in OFF_TS step with external SOF and transition to internal and send 10-n frames.
//
//      The following 3 steps are Detect Steps performed by: vtss_phy_ts_tesla_tsp_fifo_sync_detect_stage_1
//c.    Send 300 matching frames 125B min IPG with EPG. Check fifo and compute TS difference.
//d.    If OFF by 1 && RW_MODFRM_CNT == 300 (take care of rollover 2^32): Loop back to Prep 0. If already looped once and ended  here, go to 'run odd recovery'
//e.    OR RW_MODFRM_CNT < 300 go to 'run odd recovery'
//f.    OR If in sync and RW_MODFRM_CNT == 300 then exit.
static vtss_rc vtss_phy_ts_tesla_tsp_fifo_sync_recover_stage_1(vtss_state_t *vtss_state, vtss_port_no_t  port_no, const vtss_debug_printf_t       pr, u16 channel, BOOL ingress, u16 pkt_cnt)
{
    vtss_rc            rc = VTSS_RC_OK;
    BOOL               sof_ext_pcs = TRUE;
    u32                entry = 0;
    u32                ing_delta = 0;
    u32                egr_delta = 0;
    vtss_match_type_t match_mode = NO_MATCH;
    u32 regval;

    VTSS_I("Entering Recovery_Stage_1: Port_no: %d,  ING/EGR= %s \n", port_no, (ingress ? "INGRESS" : "EGRESS"));

    /* Step 1: configure EPG to send Recovery frames - 300 Packets for now */
    VTSS_I("Recov1_Step_1: Configure EPG to send 300 small Non-Match (64 or 125byte) Packets (UDP) - Pattern = 0x013F - Not Using 8051 \n");
    match_mode = ingress ? NO_MATCH_INGRESS : NO_MATCH;
    if ((rc = vtss_phy_ts_tesla_epg_send_300_packets(vtss_state, port_no, pr, 125, match_mode, &ing_delta, &egr_delta)) != VTSS_RC_OK) {
        VTSS_E("Error Detected when sending 300 Non-Match packets via EPG \n");
    }

    /* Step 2: Configure SOF to External PCS via API, ie. not using 8051 */
    VTSS_I("Recov1_Step_2: Configure SOF to External PCS Pin  ");
    sof_ext_pcs = TRUE;
    if ((rc = vtss_phy_ts_tesla_tsp_sof_conf(vtss_state, port_no, ingress, sof_ext_pcs)) != VTSS_RC_OK) {
        VTSS_E("Error setting SOF to EXT");
    }

    ing_delta = 0;
    egr_delta = 0;
    VTSS_I("Recov1_Step_3: Config EPG to send 10 Frames-(10000 byte) Non-Match Pkts SOF-Chg (UDP) %d-pkt:SOF=Ext, %d-pkt:SOF=Int - Pattern = 0x013F \n", pkt_cnt, (10 - pkt_cnt));
    // vtss_phy_ts_tesla_epg_send_jumbo_packets_8051(vtss_port_no_t   port_no, u16   channel, BOOL  ingress, u16 sof_toggle_pkt, BOOL match)
    match_mode = ingress ? NO_MATCH_INGRESS : NO_MATCH;
    if ((rc = vtss_phy_ts_tesla_epg_send_jumbo_packets_8051(vtss_state, port_no, pr, channel, ingress, pkt_cnt, match_mode, &ing_delta, &egr_delta)) != VTSS_RC_OK) {
        VTSS_E("Error Configuring EPG for 10 Large Non-Match Frames, Toggling SOF from EXT to INT at packet: %d \n", pkt_cnt);
    }

    // Egress
    VTSS_RC(VTSS_PHY_TS_READ_CSR(port_no, VTSS_PHY_TS_PROC_BLK_ID(0),
                                 VTSS_PTP_EGR_IP_1588_TSP_EGR_TSP_CTRL, &regval));
    VTSS_I("EGRESS:: SOF After Jumbo frames send - %04X \n", regval);
    // Ingress
    VTSS_RC(VTSS_PHY_TS_READ_CSR(port_no, VTSS_PHY_TS_PROC_BLK_ID(0),
                                 VTSS_PTP_INGR_IP_1588_TSP_INGR_TSP_CTRL, &regval));
    VTSS_I("INGRESS:: SOF After Jumbo frames send- %04X \n", regval);

    // Flush FIFO
    VTSS_I("Recov1_Step_4: Flush FIFO \n");
    if (VTSS_RC_OK != vtss_phy_ts_tesla_tsp_fifo_sync_empty_ts_fifo(vtss_state, port_no, pr, ingress, &entry)) {
        VTSS_I("Error Clearing FIFO\n");
    }

    VTSS_I("Exit Recovery_Stage_1: Port_no: %d,  ING/EGR= %s \n", port_no, (ingress ? "INGRESS" : "EGRESS"));

    return (rc);
}

// **********************************************************************************/
// See TN1327 for full Algorithm
//      Steps 5a -> 5f are done in function: vtss_phy_ts_tesla_tsp_fifo_sync_odd_recover
//5.    Odd recovery. This step is entered if TSP FIFO was not in Sync. (set n = 1)
//      if (tsp_odd_recovery=TRUE) Skip down to Step 5b. "Set SOF to Ext"
//a.    Set internal SOF.
//      i. Send 300 non-matching frames of size 125B min IPG.
//b.    Set SOF indicator to external.
//c.    Send 300 non-matching frames of size 125 min IPG
//d.    Flush FIFO
//e.    Send n non-matching jumbo packets using 8051, setting SOF to internal after frame n
//f.    Empty TS FIFO
//
//      The following 3 steps are Detect Steps performed by: vtss_phy_ts_tesla_tsp_fifo_sync_detect_stage_1
//g.    Send 300 matching frames of size 125B min IPG.
//            Read old_count before frame and new_count after reading RW_MODFRM_CNT.
//            New_count -old_count should be 300.
//h.    If 300 then confirm by checking distance between timestamps
//
//i.    If timestamps are OK
//            continue
//      else
//            go back to Recovery (recover_stage_1)
//
//i.    Set internal SOF.
//j.    Send 300 non matching frames of size 125 min IPG.
//k.    Check fifo contents.
//      i.    If entry exists, increment n. (n=n+1) (This is because TSP PROC is still off by 1).
//              Repeat 'Odd recovery'.
//      ii.     If n = 2 and fifo is not empty --- goto min_PlanE_recovery.
//            ELSE
//      iii.    (FIFO is empty): Exit
// **********************************************************************************/
static vtss_rc vtss_phy_ts_tesla_tsp_fifo_sync_odd_recover(vtss_state_t *vtss_state,
                                                           vtss_port_no_t  port_no,
                                                           const vtss_debug_printf_t       pr,
                                                           u16 channel, BOOL ingress, BOOL tsp_odd_recovery, u16 pkt_cnt)
{
    vtss_rc            rc = VTSS_RC_OK;
    BOOL               sof_ext_pcs = TRUE;
    u32                entry = 0;
    u32                ing_delta = 0;
    u32                egr_delta = 0;
    vtss_match_type_t match_mode = NO_MATCH;

    VTSS_I("Entering ODD_Recovery: Port_no: %d,  tsp_odd_recovery: %x;  ING/EGR= %s \n", port_no, tsp_odd_recovery, (ingress ? "INGRESS" : "EGRESS"));

    if (tsp_odd_recovery) {
        VTSS_I("Odd_Recov_Step_1_and_2 Skipped: tsp_odd_recovery=TRUE   port_no:%d  \n", port_no);
    } else {
        VTSS_I("Odd_Recov_Step_1: Ensure Configuration is SOF=INT PCS - tsp_odd_recovery=FALSE ");
        sof_ext_pcs = FALSE;
        if ((rc = vtss_phy_ts_tesla_tsp_sof_conf(vtss_state, port_no, ingress, sof_ext_pcs)) != VTSS_RC_OK) {
            VTSS_E("Error setting SOF");
        }

        VTSS_I("Odd_Recov_Step_2: Configure EPG to send 300 small Non-Match (64 or 125byte) Packets (UDP) - Pattern = 0x013F - Not Using 8051 \n");
        match_mode = ingress ? NO_MATCH_INGRESS : NO_MATCH;
        if ((rc = vtss_phy_ts_tesla_epg_send_300_packets(vtss_state, port_no, pr, 125, match_mode, &ing_delta, &egr_delta)) != VTSS_RC_OK) {
            VTSS_E("Error Configuring EPG for 300 small Non-Match Frames\n");
        }
    }

    VTSS_I("Odd_Recov_Step_3: Configure SOF to External PCS Pin  \n");
    sof_ext_pcs = TRUE;
    if ((rc = vtss_phy_ts_tesla_tsp_sof_conf(vtss_state, port_no, ingress, sof_ext_pcs)) != VTSS_RC_OK) {
        VTSS_E("Error setting SOF");
    }

    ing_delta = 0;
    egr_delta = 0;

    VTSS_I("Odd_Recov_Step_4: Configure EPG to send 300 small Non-Match (64 or 125byte) Packets (UDP) - Pattern = 0x013F - Not Using 8051 \n");
    match_mode = ingress ? NO_MATCH_INGRESS : NO_MATCH;
    if ((rc = vtss_phy_ts_tesla_epg_send_300_packets(vtss_state, port_no, pr, 125, match_mode, &ing_delta, &egr_delta)) != VTSS_RC_OK) {
        VTSS_E("Error Configuring EPG for 300 small Non-Match Frames\n");
    }

    VTSS_I("Odd_Recov_Step_5: Flush FIFO  ");
    if ((rc = vtss_phy_ts_tesla_tsp_fifo_sync_empty_ts_fifo(vtss_state, port_no, pr, ingress, &entry)) != VTSS_RC_OK) {
        VTSS_I("Error Clearing FIFO (1)");
    }
    VTSS_I("Odd_Recov_Step_5: FIFO Flushed, Entry_Cnt: %d  ", entry);

    VTSS_I("Odd_Recov_Step_6: Config EPG to send 10 Frames-(10000 byte) Non-Match Pkts SOF-Chg (UDP) %d-pkt:SOF=Ext, %d-pkt:SOF=Int - Pattern = 0x013F \n", pkt_cnt, (10 - pkt_cnt));
    // vtss_phy_ts_tesla_epg_send_jumbo_packets_8051(vtss_port_no_t   port_no, u16   channel, BOOL  ingress, u16 sof_toggle_pkt, BOOL match)
    match_mode = ingress ? NO_MATCH_INGRESS : NO_MATCH;
    if ((rc = vtss_phy_ts_tesla_epg_send_jumbo_packets_8051(vtss_state, port_no, pr, channel, ingress, pkt_cnt, match_mode, &ing_delta, &egr_delta)) != VTSS_RC_OK) {
        VTSS_E("Error Configuring EPG for 10 Large Non-Match Frames, Toggling SOF from EXT to INT at packet: %d \n", pkt_cnt);
    }

    VTSS_I("Odd_Recov_Step_7: Flush FIFO (After 8051 has switch SOF=INT) \n");
    entry = 0;
    if ((rc = vtss_phy_ts_tesla_tsp_fifo_sync_empty_ts_fifo(vtss_state, port_no, pr, ingress, &entry)) != VTSS_RC_OK) {
        VTSS_E("Error Clearing FIFO (2)");
    }

    VTSS_I("Exit ODD_Recovery: Port_no: %d,  tsp_odd_recovery: %x; FIFO_Flushed - Entry_Cnt: %d;  ING/EGR= %s \n", port_no, tsp_odd_recovery, entry, (ingress ? "INGRESS" : "EGRESS"));

    return (rc);
}

#define  VTSS_SYNC_RECOVERY_LOOP_COUNT            (3)  /* Original Value was 8 */
#define  VTSS_SYNC_ODD_RECOVERY_INNER_LOOP_COUNT  (2)
#define  VTSS_SYNC_ODD_RECOVERY_START_CNT         (1)
#define  VTSS_SYNC_ODD_RECOVERY_MAX_CNT           (2)

// See TN1327 for full Algorithm
//  The following Algorythm was developed/implemented in the Engress and Ingress Recovery
static vtss_rc vtss_phy_ts_tesla_egress_tsp_fifo_sync_recover(vtss_state_t *vtss_state,
                                                              vtss_port_no_t  port_no,
                                                              const vtss_debug_printf_t       pr,
                                                              u16 channel,
                                                              vtss_phy_ts_engine_t            eng_minE,
                                                              BOOL *hw_reset_required)
{
    BOOL               ingress = FALSE;
    BOOL               egr_OOS = FALSE;
    BOOL               recov_stage1 = FALSE;
    BOOL               first_time = TRUE;
    BOOL               recovery_confirmed = FALSE;
    BOOL               min_PlanE_recovery = FALSE;
    BOOL               odd_recovery = FALSE;
    BOOL               tsp_odd_recovery = TRUE;
    BOOL               even_recovery = TRUE; //Used for even recovery case encountered during odd_recov
    u16                odd_recovery_cnt = VTSS_SYNC_ODD_RECOVERY_START_CNT;
    u32                entry = 0;
    vtss_port_no_t     base_port_no;
    i8                 count = 0;
    u16                inner_loop_cnt;
    u16                discIndex = 0;
    vtss_rc            rc = VTSS_RC_OK;

    VTSS_I("Entering EGRESS Recovery: Port_no: %d \n", port_no);

    if ((rc = vtss_phy_ts_base_port_get_priv(vtss_state, port_no, &base_port_no)) != VTSS_RC_OK) {
        VTSS_E("Port %d:: Base Port get failed", port_no);
        return VTSS_RC_ERROR;
    }

    // See TN1327 for full Algorithm
    //1.If egress recovery:
    //  disable ingress analyzer by clearing the ingress analyzer enables
    //  (not touching any registers to other channel).
    /* Disable Ingress Analyzer by clearing the Ingress Analyzer Enable bits for this channel */
    *hw_reset_required = FALSE;

    //1.    Step_1:
    //a.    Execute MinPlanE to fix TSP_CMD_FIFO if Out-of-Sync
    //b.    Flush the FIFO
    VTSS_I("Step 1a: EGRESS: Port_no: %d -- Executing Mini-PLAN_E Recovery, \n", port_no);
    if ((rc = vtss_phy_ts_resolve_tsp_cmd_oos_eng_2_plan_e(vtss_state, port_no, eng_minE, pr)) != VTSS_RC_OK) {
        VTSS_E("EGRESS Port_no: %d -- ERROR in Mini-PLAN_E Recovery \n", port_no);
    }

    VTSS_I("Step 1b: Flush FIFO  \n");
    if ((rc = vtss_phy_ts_tesla_tsp_fifo_sync_empty_ts_fifo(vtss_state, port_no, pr, ingress, &entry)) != VTSS_RC_OK) {
        VTSS_E("Error Clearing FIFO\n");
    }

    /*
     * 3a. Prep_0:
     * i. Use EPG to send 300 non matching Ethernet-only frames of size 125B min IPG.
     * ii.    Flush fifo
     */
    VTSS_I("Step 3a: Run prep_0\n");
    if ((rc = vtss_phy_ts_tesla_tsp_fifo_sync_prep_stage_0(vtss_state, port_no, pr, ingress, &entry)) != VTSS_RC_OK) {
        VTSS_E("Unable to Setup Stage_0 Recovery, port_no: %d \n", port_no);
        return (VTSS_RC_ERROR);
    }

    if (entry > 0) {
        VTSS_E("EGRESS -- FIFO should be empty, but FIFO_NOT_EMPTY!! Entry: %d -- Going to Mini-PLAN_E Recovery loop_cnt: %d \n!!", entry, count);
    } else {
        VTSS_I("EGRESS: Setup of Stage_0 Recovery for port_no: %d, entry: %d, Forcing tsp_odd_recovery \n", port_no, entry);
    }

    //4.    Recovery:
    //a.    Use EPG to send 300 non matching Ethernet-only frames of size 125B min IPG.
    //b.    Use Micro to send exact number of non matching jumbo frames (discIndex) with max IPG computed
    //          in OFF_TS step with external SOF and transition to internal and send 10-n frames.
    //c.    Detect Step:
    //          Send 300 matching frames 125B min IPG with EPG.
    //          Check fifo and compute TS difference.
    //d.    If OFF by 1 && RW_MODFRM_CNT == 300 (take care of rollover 2^32):
    //         - Loop back to Prep 0.
    //         - If already looped once and ended  here, go to 'run odd recovery'
    //e.    OR RW_MODFRM_CNT < 300 go to 'run odd recovery'
    //f.    OR If in sync and RW_MODFRM_CNT == 300 then exit
    count = 0;
    do {
        if (even_recovery) {      /* even_recovery = TRUE at entry on First time thru */
            inner_loop_cnt = 0;
            do {
                count++;
                tsp_odd_recovery = FALSE;
                odd_recovery = FALSE;
                even_recovery = FALSE;
                if (recov_stage1 == TRUE) {   /* recov_stage1 = FALSE at entry on First time thru, but gets set to TRUE on inner_loop_cnt=1 */
                    if (first_time) {
                        VTSS_I("TSP ODD Recovery was set..Going to TSP ODD recovery\n");
                        tsp_odd_recovery = TRUE;
                        first_time = FALSE;
                        break;
                    }

                    if ((rc = vtss_phy_ts_tesla_tsp_fifo_sync_recover_stage_1(vtss_state, port_no, pr, channel, ingress, discIndex)) != VTSS_RC_OK) {
                        VTSS_E("Port_no: %d; Egress: ERROR Returned from Recovery Stage_1! \n", port_no);
                        return (VTSS_RC_ERROR);
                    }

                    discIndex = 0;
                    if ((rc = vtss_phy_ts_tesla_tsp_fifo_sync_detect_stage_1(vtss_state, port_no, pr, channel, ingress, &egr_OOS, &tsp_odd_recovery, &discIndex)) != VTSS_RC_OK) {
                        VTSS_E("Egress: ERROR Returned from SYNC-Detect Stage_1! \n");
                    }

                    if (tsp_odd_recovery) {
                        VTSS_I("EGRESS -- TSP_ODD_RECOVERY(2) Flag is Set -- Setting Odd_Recovery Flag!! \n");
                        odd_recovery = TRUE;
                        break;
                    } else {
                        VTSS_I("EGRESS -- TSP_ODD_RECOVERY(2) Flag is Clear - Checking FIFO Empty!! \n");
                        if ((rc = vtss_phy_ts_tesla_tsp_fifo_sync_prep_stage_0(vtss_state, port_no, pr, ingress, &entry)) != VTSS_RC_OK) {
                            VTSS_E("Unable to Setup Stage_0 Recovery, port_no: %d \n", port_no);
                            return (VTSS_RC_ERROR);
                        }

                        if (entry > 0) {
                            VTSS_E("EGRESS -- FIFO should be empty, but FIFO_NOT_EMPTY!! Entry: %d -- \n", entry);
                            min_PlanE_recovery = TRUE;
                            break; // break out!! Elevate to Higher Level of Recovery!!
                        }
                    }

                    if ((egr_OOS == FALSE) && (odd_recovery == FALSE)) {
                        VTSS_I("***********************************************************\n");
                        VTSS_I("----------- EGRESS -- IN-SYNC! ----- Exitting Recovery ----\n");
                        VTSS_I("***********************************************************\n");
                        recovery_confirmed = TRUE;
                        break;
                    } else if ((egr_OOS == TRUE) && (odd_recovery == FALSE)) {
                        VTSS_I("***********************************************************\n");
                        VTSS_I("----------- EGRESS -- OUT-OF-SYNC -- (EGR_OOS Only) -------\n");
                        VTSS_I("***********************************************************\n");
                    } else {
                        VTSS_I("****************************************************************\n");
                        VTSS_I("----------- EGRESS -- OUT-OF_SYNC ---(EGR_OOS & ODD_RECOV) -----\n");
                        VTSS_I("****************************************************************\n");
                    }
                }

                // If OFF by 1 (ie. egr_OOS=TRUE) && RW_MODFRM_CNT == 300 (ie. odd_recovery = FALSE)
                // Loop back to Prep 0.
                // However, If already looped once and ended  here, go to 'run odd recovery'
                if ((egr_OOS == TRUE) && (odd_recovery == FALSE)) { //If OFF by 1 && RW_MODFRM_CNT == 300
                    if (inner_loop_cnt > 0) { // Already been thru once!
                        odd_recovery = TRUE;
                        VTSS_I("EGRESS --Recovery Twice : Setting ODD_RECOVERY Flag!! \n");
                        break;
                    }
                }
                inner_loop_cnt++;
                recov_stage1 = TRUE;
            } while (inner_loop_cnt < VTSS_SYNC_ODD_RECOVERY_INNER_LOOP_COUNT);

        }  /* end of condition - if (even_recovery)  */

        if (odd_recovery && !tsp_odd_recovery && (!recovery_confirmed && !min_PlanE_recovery)) {    // Two cases: Both have ODD_RECOVERY = TRUE!

            // Odd_Recovery: Must execute detect and Even Recovery afterwards
            // **********************************************************************************/
            // See TN1327 for full Algorithm
            //      Steps 5a -> 5f are done in function: vtss_phy_ts_tesla_tsp_fifo_sync_odd_recover
            //5.    Odd recovery. This step is entered if TSP FIFO was not in Sync. (set n = 1)
            //a.    Set internal SOF. Send 300 non-matching frames of size 125B min IPG.
            //b.    Set SOF indicator to external.
            //c.    Send 300 non-matching frames of size 125 min IPG
            //d.    Flush FIFO
            //e.    Send n non-matching jumbo packets using 8051, setting SOF to internal after frame n
            //f.    Empty TS FIFO
            //
            //      The following 3 steps are Detect Steps performed by: vtss_phy_ts_tesla_tsp_fifo_sync_detect_stage_1
            //g.    Send 300 matching frames of size 125B min IPG.
            //            Read old_count before frame and new_count after reading RW_MODFRM_CNT.
            //            New_count -old_count should be 300.
            //h.    If 300 (odd_recovery = FALSE) then confirm by checking distance between timestamps
            //i.    If timestamps are OK (egr_OOS = FALSE)
            //            continue
            //      else
            //            go back to Recovery (recover_stage_1)
            //
            //      The following 2 Steps are performed by: vtss_phy_ts_tesla_tsp_fifo_sync_prep_stage_0
            //i.    Set internal SOF.
            //j.    Send 300 non matching frames of size 125 min IPG.
            //
            //k.    Check fifo contents.
            //i.    If entry exists, increment n. (n=n+1) (This is because TSP PROC is still off by 1).
            //              Repeat 'Odd recovery'.
            //              If n = 2 and fifo is not empty --- goto min_PlanE_recovery.
            //ii.   ELSE
            //              (FIFO is empty): Exit
            // **********************************************************************************/

            inner_loop_cnt = 0;


            do {
                count++;
                VTSS_I("Egress: ODD_RECOVERY: EGR_OOS: %s;  Odd_recovery_Cnt: %d, inner_lp: %d, outer_lp: %d \n", (egr_OOS ? "TRUE" : "FALSE"), odd_recovery_cnt, inner_loop_cnt, count);

                if ((rc = vtss_phy_ts_tesla_tsp_fifo_sync_odd_recover(vtss_state, port_no, pr, channel, ingress, tsp_odd_recovery, odd_recovery_cnt)) != VTSS_RC_OK) {
                    VTSS_E("Port_no: %d; Egress: ERROR Returned from Odd_Recovery! \n", port_no);
                }

                discIndex = 0;
                if ((rc = vtss_phy_ts_tesla_tsp_fifo_sync_detect_stage_1(vtss_state, port_no, pr, channel, ingress, &egr_OOS, &tsp_odd_recovery, &discIndex)) != VTSS_RC_OK) {
                    VTSS_E("Egress: ERROR Returned from SYNC-Detect Stage_1! \n");
                }

                if (tsp_odd_recovery) {
                    VTSS_I("EGRESS -- TSP ODD RECOVERY REQUIRED\n");
                    break;
                }

                if (egr_OOS) {

                    odd_recovery_cnt++;
                    if (inner_loop_cnt == 0) {
                        odd_recovery = TRUE;
                        even_recovery = FALSE;
                    } else {
                        // second count. So go to start of algorithm
                        odd_recovery = FALSE;
                        even_recovery = TRUE;
                    }

                    VTSS_I("Egress OOS SEEN, exiting Odd recovery and starting even recovery");
                    if (odd_recovery == FALSE) {
                        VTSS_I("***********************************************************\n");
                        VTSS_I("----------- EGRESS -- OUT-OF-SYNC -- (EGR_OOS Only) -------\n");
                        VTSS_I("***********************************************************\n");
                        break;
                    } else {// if ((odd_recovery == TRUE))
                        VTSS_I("***********************************************************\n");
                        VTSS_I("----------- EGRESS -- OUT-OF-SYNC -- (ODD_RECOV Only)------\n");
                        VTSS_I("***********************************************************\n");
                    }
                } else {
                    if ((rc = vtss_phy_ts_tesla_tsp_fifo_sync_prep_stage_0(vtss_state, port_no, pr, ingress, &entry)) != VTSS_RC_OK) {
                        VTSS_E("Unable to Setup Stage_0 Recovery, port_no: %d \n", port_no);
                        return (VTSS_RC_ERROR);
                    }

                    if (entry > 0) {
                        VTSS_E("EGRESS -- FIFO_NOT_EMPTY!! Entry: %d -- odd_recovery_cnt: %d \n", entry, odd_recovery_cnt);
                        min_PlanE_recovery = TRUE;
                        VTSS_I("EGRESS -- Going to Mini-PLAN_E Recovery!! Entry: %d, ODD_RECOVERY_CNT: %d,  inner_lp: %d   outer_lp: %d \n", entry, odd_recovery_cnt, inner_loop_cnt, count);
                    } else {
                        recovery_confirmed = TRUE;
                        VTSS_I("EGRESS -- Recovery-Confirmed!! Entry: %d, ODD_RECOVERY_CNT: %d,  inner_lp: %d   outer_lp: %d \n", entry, odd_recovery_cnt, inner_loop_cnt, count);
                    }
                    break;
                }

                VTSS_I("EGRESS -- Entry: %d, ODD_RECOVERY_CNT: %d,  inner_lp: %d   outer_lp: %d \n", entry, odd_recovery_cnt, inner_loop_cnt, count);

                inner_loop_cnt++;
            } while (inner_loop_cnt < VTSS_SYNC_ODD_RECOVERY_INNER_LOOP_COUNT);
        }

        if (tsp_odd_recovery && (!min_PlanE_recovery && !recovery_confirmed)) {
            //tsp ODD recovery has higher priority to be resolved
            // Odd_Recovery: Must execute detect and Even Recovery afterwards
            // **********************************************************************************/
            // See TN1327 for full Algorithm
            //      Steps 5a -> 5f are done in function: vtss_phy_ts_tesla_tsp_fifo_sync_odd_recover
            //5.    Odd recovery. This step is entered if TSP FIFO was not in Sync. (set n = 1)
            //a.    Set internal SOF. Send 300 non-matching frames of size 125B min IPG.
            //b.    Set SOF indicator to external.
            //c.    Send 300 non-matching frames of size 125 min IPG
            //d.    Flush FIFO
            //e.    Send n non-matching jumbo packets using 8051, setting SOF to internal after frame n
            //f.    Empty TS FIFO
            //
            //      The following 3 steps are Detect Steps performed by: vtss_phy_ts_tesla_tsp_fifo_sync_detect_stage_1
            //g.    Send 300 matching frames of size 125B min IPG.
            //            Read old_count before frame and new_count after reading RW_MODFRM_CNT.
            //            New_count -old_count should be 300.
            //h.    If 300 (odd_recovery = FALSE) then confirm by checking distance between timestamps
            //i.    If timestamps are OK (egr_OOS = FALSE)
            //            continue
            //      else
            //            go back to Recovery (recover_stage_1)
            //
            //      The following 2 Steps are performed by: vtss_phy_ts_tesla_tsp_fifo_sync_prep_stage_0
            //i.    Set internal SOF.
            //j.    Send 300 non matching frames of size 125 min IPG.
            //
            //k.    Check fifo contents.
            //i.    If entry exists, increment n. (n=n+1) (This is because TSP PROC is still off by 1).
            //              Repeat 'Odd recovery'.
            //              If n = 2 and fifo is not empty --- goto min_PlanE_recovery.
            //ii.   ELSE
            //              (FIFO is empty): Exit
            // **********************************************************************************/

            inner_loop_cnt = 0;

            do {
                count++;
                VTSS_I("EGRESS TSP_ODD_RECOVERY: Port_no: %d -- Executing Mini-PLAN_E Recovery, EGR_OOS: %s;  odd_recovery_Cnt: %d\n",
                       port_no, (egr_OOS ? "TRUE" : "FALSE"), inner_loop_cnt);
                if ((rc = vtss_phy_ts_resolve_tsp_cmd_oos_eng_2_plan_e(vtss_state, port_no, eng_minE, pr)) != VTSS_RC_OK) {
                    VTSS_E("EGRESS Port_no: %d -- ERROR in Mini-PLAN_E Recovery \n", port_no);
                }
                VTSS_I("Egress: ODD_RECOVERY: Odd_recovery_Cnt: %d, inner_lp: %d, outer_lp: %d \n",
                       odd_recovery_cnt, inner_loop_cnt, count);
                if ((rc = vtss_phy_ts_tesla_tsp_fifo_sync_odd_recover(vtss_state, port_no, pr, channel, ingress, tsp_odd_recovery, odd_recovery_cnt)) != VTSS_RC_OK) {
                    VTSS_E("Port_no: %d; Egress: ERROR Returned from Odd_Recovery! \n", port_no);
                }

                discIndex = 0;
                if ((rc = vtss_phy_ts_tesla_tsp_fifo_sync_detect_stage_1(vtss_state, port_no, pr, channel, ingress, &egr_OOS, &tsp_odd_recovery, &discIndex)) != VTSS_RC_OK) {
                    VTSS_E("Egress: ERROR Returned from SYNC-Detect Stage_1! \n");
                }

                if (tsp_odd_recovery) {

                    if (inner_loop_cnt == 0) {
                        odd_recovery_cnt++;

                    } else {
                        min_PlanE_recovery = TRUE;
                        VTSS_I("EGRESS -- Going to Mini-PLAN_E Recovery!! Entry: %d, ODD_RECOVERY_CNT: %d,  inner_lp: %d   outer_lp: %d \n", entry, odd_recovery_cnt, inner_loop_cnt, count);
                        break;
                    }
                } else if ((egr_OOS) && (!tsp_odd_recovery)) {
                    // If egr_OOS after two times thru, then we just do recovery here and fall out of loop
                    odd_recovery = FALSE;
                    even_recovery = TRUE;
                    VTSS_I("Egress OOS SEEN, exiting Odd recovery and starting even recovery");

                    VTSS_I("***********************************************************\n");
                    VTSS_I("----------- EGRESS -- OUT-OF-SYNC -- (EGR_OOS Only) -------\n");
                    VTSS_I("***********************************************************\n");
                    break;
                } else {
                    if ((rc = vtss_phy_ts_tesla_tsp_fifo_sync_prep_stage_0(vtss_state, port_no, pr, ingress, &entry)) != VTSS_RC_OK) {
                        VTSS_E("Unable to Setup Stage_0 Recovery, port_no: %d \n", port_no);
                        return (VTSS_RC_ERROR);
                    }

                    if (entry > 0) {
                        VTSS_E("EGRESS -- FIFO_NOT_EMPTY!! Entry: %d -- odd_recovery_cnt: %d \n", entry, odd_recovery_cnt);
                        min_PlanE_recovery = TRUE;
                        VTSS_I("EGRESS -- Going to Mini-PLAN_E Recovery!! Entry: %d, ODD_RECOVERY_CNT: %d,  inner_lp: %d   outer_lp: %d \n", entry, odd_recovery_cnt, inner_loop_cnt, count);
                    } else {
                        recovery_confirmed = TRUE;
                        VTSS_I("EGRESS -- Recovery-Confirmed!! Entry: %d, ODD_RECOVERY_CNT: %d,  inner_lp: %d   outer_lp: %d \n", entry, odd_recovery_cnt, inner_loop_cnt, count);
                    }
                    break;
                }

                VTSS_I("EGRESS -- Entry: %d, ODD_RECOVERY_CNT: %d,  inner_lp: %d   outer_lp: %d \n", entry, odd_recovery_cnt, inner_loop_cnt, count);

                inner_loop_cnt++;

            } while (inner_loop_cnt < VTSS_SYNC_ODD_RECOVERY_INNER_LOOP_COUNT);
        }

        if ((recovery_confirmed) || (min_PlanE_recovery)) {
            VTSS_I("EGRESS:: <<<<<<<< Recv_Confirmed: 0x%x, min_PlanE: 0x%x >>>>>>>>> \n", recovery_confirmed, min_PlanE_recovery);
            break;
        }

        VTSS_I("EGRESS::Looping for complete Algorithm Count:%d egr_OOS:%s \n", count, egr_OOS ? "TRUE" : "FALSE" );
        recov_stage1 = TRUE;

    } while (count < VTSS_SYNC_RECOVERY_LOOP_COUNT);


    if ((!recovery_confirmed) || (min_PlanE_recovery)) {
        VTSS_I("******************************************************************\n");
        VTSS_I("----------- EGRESS -- OUT-OF-SYNC - Executing Mini-PLAN_E  ----- -\n");
        VTSS_I("******************************************************************\n");
        VTSS_I("EGRESS Port_no: %d -- Executing Mini-PLAN_E Recovery \n", port_no);

        //TODO Enable ingress timestamp
        if ((rc = vtss_phy_ts_resolve_tsp_cmd_oos_eng_2_plan_e(vtss_state, port_no, eng_minE, pr)) != VTSS_RC_OK) {
            VTSS_E("EGRESS Port_no: %d -- ERROR in Mini-PLAN_E Recovery \n", port_no);
        }
        // TODO disable ingress timestamp

        VTSS_I("******************************************************************\n");
        VTSS_I("----------- EGRESS -- Mini-PLAN_E Recovery Complete  ----------- -\n");
        VTSS_I("******************************************************************\n");
    }

    if (recovery_confirmed) {
        *hw_reset_required = FALSE;
    } else {
        *hw_reset_required = TRUE;
    }

    return (VTSS_RC_OK);
}

// See TN1327 for full Algorithm
static vtss_rc vtss_phy_ts_tesla_ingress_tsp_fifo_sync_recover(vtss_state_t *vtss_state,
                                                               vtss_port_no_t  port_no,
                                                               const vtss_debug_printf_t       pr,
                                                               u16 channel,
                                                               vtss_phy_ts_engine_t     eng_minE,
                                                               BOOL *hw_reset_required)
{
    BOOL               ingress = TRUE;
    BOOL               ing_OOS = FALSE;
    BOOL               recov_stage1 = FALSE;
    BOOL               first_time = TRUE;
    BOOL               recovery_confirmed = FALSE;
    BOOL               odd_recovery = FALSE;
    BOOL               min_PlanE_recovery = FALSE;
    BOOL               tsp_odd_recovery = TRUE;
    BOOL               even_recovery = TRUE; //Used for even recovery case encountered during odd_recov
    u32                entry = 0;
    vtss_rc            rc = VTSS_RC_OK;
    i8                 count = 2;
    u16                discIndex = 0;
    u16                odd_recovery_cnt = VTSS_SYNC_ODD_RECOVERY_START_CNT;
    u16                inner_loop_cnt;
    BOOL               mini_plane_done;

    VTSS_I("Entering INGRESS Recovery: Port_no: %d\n", port_no);

    *hw_reset_required = FALSE;

    //1.    Step_1:
    //a.    Execute MinPlanE to fix TSP_CMD_FIFO if Out-of-Sync
    //b.    Flush the FIFO
    VTSS_I("Step 1a: INRESS: Port_no: %d -- Executing Mini-PLAN_E Recovery \n", port_no);
    if ((rc = vtss_phy_ts_resolve_tsp_cmd_oos_eng_2_plan_e(vtss_state, port_no, eng_minE, pr)) != VTSS_RC_OK) {
        VTSS_E("INGRESS Port_no: %d -- ERROR in Mini-PLAN_E Recovery \n", port_no);
    }

    VTSS_I("Step_1b: Flush FIFO  \n");
    if ((rc = vtss_phy_ts_tesla_tsp_fifo_sync_empty_ts_fifo(vtss_state, port_no, pr, ingress, &entry)) != VTSS_RC_OK) {
        VTSS_E("Error Clearing FIFO\n");
    }

    //2.    Prep_0:
    //a.    Use EPG to send 300 non matching Ethernet-only frames of size 125B min IPG.
    //b.    Flush fifo
    //c.    If Ingress and FIFO is non empty (CMD FIFO OOS) - goto Min_PlanE_recovery
    if ((rc = vtss_phy_ts_tesla_tsp_fifo_sync_prep_stage_0(vtss_state, port_no, pr, ingress, &entry)) != VTSS_RC_OK) {
        VTSS_E("Unable to Setup Stage_0 Recovery, port_no: %d ", port_no);
        return (VTSS_RC_ERROR);
    }

    if (entry > 0) {
        VTSS_E("INGRESS -- FIFO should be empty, but FIFO_NOT_EMPTY!! Entry: %d -- Going to Mini-PLAN_E Recovery loop_cnt: %d \n!!", entry, count);
    } else {
        VTSS_I("INRESS: Setup of Stage_0 Recovery for port_no: %d, entry: %d, Forcing tsp_odd_recovery \n", port_no, entry);
    }

    //4.    Recovery:
    //a.    Use EPG to send 300 non matching Ethernet-only frames of size 125B min IPG.
    //b.    Use Micro to send exact number of non matching jumbo frames (discIndex) with max IPG computed
    //          in OFF_TS step with external SOF and transition to internal and send 10-n frames.
    //c.    Detect Step:
    //          Send 300 matching frames 125B min IPG with EPG.
    //          Check fifo and compute TS difference.
    //d.    If OFF by 1 && RW_MODFRM_CNT == 300 (take care of rollover 2^32):
    //         - Loop back to Prep 0.
    //         - If already looped once and ended  here, go to 'run odd recovery'
    //e.    OR RW_MODFRM_CNT < 300 go to 'run odd recovery'
    //f.    OR If in sync and RW_MODFRM_CNT == 300 then exit.
    count = 0;
    do {
        if (even_recovery) {      /* even_recovery = TRUE at entry on First time thru */
            inner_loop_cnt = 0;
            do {
                count++;
                even_recovery = FALSE;
                tsp_odd_recovery = FALSE;
                odd_recovery = FALSE;
                if (recov_stage1 == TRUE) {   /* recov_stage1=FALSE at entry on First time thru, but gets set to TRUE on inner_loop_cnt=1 */
                    if (first_time) {
                        VTSS_I("TSP ODD Recovery was set..Going to TSP ODD recovery\n");
                        tsp_odd_recovery = TRUE;
                        first_time = FALSE;
                        break;
                    }

                    if ((rc = vtss_phy_ts_tesla_tsp_fifo_sync_recover_stage_1(vtss_state, port_no, pr, channel, ingress, discIndex)) != VTSS_RC_OK) {
                        VTSS_E("Port_no: %d; Ingress: ERROR Returned from Recovery Stage_1! \n", port_no);
                        return (VTSS_RC_ERROR);
                    }

                    discIndex = 0;
                    if ((rc = vtss_phy_ts_tesla_tsp_fifo_sync_detect_stage_1(vtss_state, port_no, pr, channel, ingress, &ing_OOS, &tsp_odd_recovery, &discIndex)) != VTSS_RC_OK) {
                        VTSS_E("Ingress: ERROR Returned from SYNC-Detect Stage_1! \n");
                    }

                    if (tsp_odd_recovery) {
                        VTSS_I("INGRESS -- TSP_ODD_RECOVERY(2) Flag is Set -- Setting Odd_Recovery Flag!! \n");
                        odd_recovery = TRUE;
                        break;
                    } else {
                        VTSS_I("INGRESS -- TSP_ODD_RECOVERY(2) Flag is Clear - Checking FIFO Empty!! \n");
                        if ((rc = vtss_phy_ts_tesla_tsp_fifo_sync_prep_stage_0(vtss_state, port_no, pr, ingress, &entry)) != VTSS_RC_OK) {
                            VTSS_E("Unable to Setup Stage_0 Recovery, port_no: %d ", port_no);
                            return (VTSS_RC_ERROR);
                        }

                        if (entry > 0) {
                            VTSS_E("INGRESS -- FIFO should be empty, but FIFO_NOT_EMPTY!! Entry: %d -- \n", entry);
                            min_PlanE_recovery = TRUE;
                            break;
                        }
                    }

                    if ((ing_OOS == FALSE) && (odd_recovery == FALSE)) {
                        VTSS_I("***********************************************************\n");
                        VTSS_I("----------- INGRESS -- IN-SYNC! ----- Exitting Recovery ----\n");
                        VTSS_I("***********************************************************\n");
                        recovery_confirmed = TRUE;
                        break;
                    } else if ((ing_OOS == TRUE) && (odd_recovery == FALSE)) {
                        VTSS_I("***********************************************************\n");
                        VTSS_I("----------- INGRESS -- OUT-OF-SYNC -- (ING_OOS Only) -------\n");
                        VTSS_I("***********************************************************\n");
                    } else {
                        VTSS_I("****************************************************************\n");
                        VTSS_I("----------- INGRESS -- OUT-OF_SYNC ---(ING_OOS & ODD_RECOV) -----\n");
                        VTSS_I("****************************************************************\n");
                    }
                    VTSS_I("Port_no: %d, -- INGRESS -- ING_OOS: 0x%x; ODD_RECOV: 0x%x; \n", port_no, ing_OOS, odd_recovery);
                }

                // If OFF by 1 (ie. egr_OOS=TRUE) && RW_MODFRM_CNT == 300 (ie. odd_recovery = FALSE)
                // Loop back to Prep 0.
                // However, If already looped once and ended  here, go to 'run odd recovery'
                if ((ing_OOS == TRUE) && (odd_recovery == FALSE)) { //If OFF by 1 && RW_MODFRM_CNT == 300
                    if (inner_loop_cnt > 0) { // Already been thru once!
                        odd_recovery = TRUE;
                        break;
                    }
                }
                inner_loop_cnt++;
                recov_stage1 = TRUE;

            } while (inner_loop_cnt < VTSS_SYNC_ODD_RECOVERY_INNER_LOOP_COUNT);

        } /* end of condition - if (even_recovery) */


        if ((odd_recovery) && (!tsp_odd_recovery) && (!recovery_confirmed && !min_PlanE_recovery)) {
            // Two cases: Both have ODD_RECOVERY = TRUE!
            // Odd_Recovery: Must execute detect and Even Recovery afterwards
            // **********************************************************************************/
            // See TN1327 for full Algorithm
            //      Steps 5a -> 5f are done in function: vtss_phy_ts_tesla_tsp_fifo_sync_odd_recover
            //5.    Odd recovery. This step is entered if TSP FIFO was not in Sync. (set n = 1)
            //a.    Set internal SOF. Send 300 non-matching frames of size 125B min IPG.
            //b.    Set SOF indicator to external.
            //c.    Send 300 non-matching frames of size 125 min IPG
            //d.    Flush FIFO
            //e.    Send n non-matching jumbo packets using 8051, setting SOF to internal after frame n
            //f.    Empty TS FIFO
            //
            //      The following 3 steps are Detect Steps performed by: vtss_phy_ts_tesla_tsp_fifo_sync_detect_stage_1
            //g.    Send 300 matching frames of size 125B min IPG.
            //            Read old_count before frame and new_count after reading RW_MODFRM_CNT.
            //            New_count -old_count should be 300.
            //h.    If 300 (odd_recovery = FALSE) then confirm by checking distance between timestamps
            //i.    If timestamps are OK (egr_OOS = FALSE)
            //            continue
            //      else
            //            go back to Recovery (recover_stage_1)
            //
            //      The following 2 Steps are performed by: vtss_phy_ts_tesla_tsp_fifo_sync_prep_stage_0
            //i.    Set internal SOF.
            //j.    Send 300 non matching frames of size 125 min IPG.
            //
            //k.    Check fifo contents.
            //i.    If entry exists, increment n. (n=n+1) (This is because TSP PROC is still off by 1).
            //              Repeat 'Odd recovery'.
            //              If n = 2 and fifo is not empty --- goto min_PlanE_recovery.
            //ii.   ELSE
            //              (FIFO is empty): Exit
            // **********************************************************************************/

            inner_loop_cnt = 0;

            do {
                count++;

                VTSS_I("Ingress: ODD_RECOVERY: Odd_recovery_Cnt: %d, inner_lp: %d, outer_lp: %d \n",
                       odd_recovery_cnt, inner_loop_cnt, count);

                if ((rc = vtss_phy_ts_tesla_tsp_fifo_sync_odd_recover(vtss_state, port_no, pr, channel, ingress, tsp_odd_recovery, odd_recovery_cnt)) != VTSS_RC_OK) {
                    VTSS_E("Port_no: %d; Ingress: ERROR Returned from Odd_Recovery! \n", port_no);
                }

                discIndex = 0;
                if ((rc = vtss_phy_ts_tesla_tsp_fifo_sync_detect_stage_1(vtss_state, port_no, pr, channel, ingress, &ing_OOS, &tsp_odd_recovery, &discIndex)) != VTSS_RC_OK) {
                    VTSS_E("Ingress: ERROR Returned from SYNC-Detect Stage_1! \n");
                }

                // If ing_OOS after two times thru, then we just do recovery here and fall out of loop
                if (tsp_odd_recovery) {
                    VTSS_I("EGRESS -- TSP ODD RECOVERY REQUIRED\n");
                    break;
                }
                if (ing_OOS) {
                    odd_recovery_cnt++;
                    if (inner_loop_cnt == 0) {
                        odd_recovery = TRUE;
                        even_recovery = FALSE;
                    } else {
                        // second count. So go to start of algorithm
                        odd_recovery = FALSE;
                        even_recovery = TRUE;
                    }

                    VTSS_I("INGRESS OOS SEEN, exiting Odd recovery and starting even recovery");

                    if (odd_recovery == FALSE) {
                        VTSS_I("***********************************************************\n");
                        VTSS_I("----------- INGRESS -- OUT-OF-SYNC -- (Ingr OOS Only)------\n");
                        VTSS_I("***********************************************************\n");
                        break;
                    } else  {
                        VTSS_I("***********************************************************\n");
                        VTSS_I("----------- INGRESS -- OUT-OF-SYNC -- (ODD_RECOV Only)------\n");
                        VTSS_I("***********************************************************\n");
                    }
                } else {
                    VTSS_I("INGRESS -- TSP_ODD_RECOVERY(3) Flag is Clear - Checking FIFO Empty!! \n");
                    if ((rc = vtss_phy_ts_tesla_tsp_fifo_sync_prep_stage_0(vtss_state, port_no, pr, ingress, &entry)) != VTSS_RC_OK) {
                        VTSS_E("Unable to Setup Stage_0 Recovery, port_no: %d \n", port_no);
                        return (VTSS_RC_ERROR);
                    }

                    if (entry > 0) {
                        VTSS_E("INGRESS -- FIFO_NOT_EMPTY!! Entry: %d -- odd_recovery_cnt: %d \n", entry, odd_recovery_cnt);
                        min_PlanE_recovery = TRUE;
                        VTSS_I("INGRESS -- Going to Mini-PLAN_E Recovery!! Entry: %d, ODD_RECOVERY_CNT: %d,  inner_lp: %d   outer_lp: %d \n", entry, odd_recovery_cnt, inner_loop_cnt, count);
                    } else {
                        recovery_confirmed = TRUE;
                        VTSS_I("INGRESS -- Recovery-Confirmed!! Entry: %d, ODD_RECOVERY_CNT: %d,  inner_lp: %d   outer_lp: %d \n", entry, odd_recovery_cnt, inner_loop_cnt, count);
                    }
                    break;
                }

                VTSS_I("INGRESS -- Entry: %d, ODD_RECOVERY_CNT: %d,  inner_lp: %d   outer_lp: %d \n", entry, odd_recovery_cnt, inner_loop_cnt, count);

                inner_loop_cnt++;

            } while (inner_loop_cnt < VTSS_SYNC_ODD_RECOVERY_INNER_LOOP_COUNT);
        } // ODD recovery


        if (!min_PlanE_recovery && !recovery_confirmed && (tsp_odd_recovery)) { // Two cases: Both have ODD_RECOVERY = TRUE!
            // Odd_Recovery: Must execute detect and Even Recovery afterwards
            // **********************************************************************************/
            // See TN1327 for full Algorithm
            //      Steps 5a -> 5f are done in function: vtss_phy_ts_tesla_tsp_fifo_sync_odd_recover
            //5.    Odd recovery. This step is entered if TSP FIFO was not in Sync. (set n = 1)
            //a.    Set internal SOF. Send 300 non-matching frames of size 125B min IPG.
            //b.    Set SOF indicator to external.
            //c.    Send 300 non-matching frames of size 125 min IPG
            //d.    Flush FIFO
            //e.    Send n non-matching jumbo packets using 8051, setting SOF to internal after frame n
            //f.    Empty TS FIFO
            //
            //      The following 3 steps are Detect Steps performed by: vtss_phy_ts_tesla_tsp_fifo_sync_detect_stage_1
            //g.    Send 300 matching frames of size 125B min IPG.
            //            Read old_count before frame and new_count after reading RW_MODFRM_CNT.
            //            New_count -old_count should be 300.
            //h.    If 300 (odd_recovery = FALSE) then confirm by checking distance between timestamps
            //i.    If timestamps are OK (egr_OOS = FALSE)
            //            continue
            //      else
            //            go back to Recovery (recover_stage_1)
            //
            //      The following 2 Steps are performed by: vtss_phy_ts_tesla_tsp_fifo_sync_prep_stage_0
            //i.    Set internal SOF.
            //j.    Send 300 non matching frames of size 125 min IPG.
            //
            //k.    Check fifo contents.
            //i.    If entry exists, increment n. (n=n+1) (This is because TSP PROC is still off by 1).
            //              Repeat 'Odd recovery'.
            //              If n = 2 and fifo is not empty --- goto min_PlanE_recovery.
            //ii.   ELSE
            //              (FIFO is empty): Exit
            // **********************************************************************************/

            inner_loop_cnt = 0;

            do {
                count++;
                VTSS_I("INGRESS TSP_ODD_RECOVERY: Port_no: %d -- Executing Mini-PLAN_E Recovery, ING_OOS: %s;  odd_recovery_Cnt: %d\n",
                       port_no, (ing_OOS ? "TRUE" : "FALSE"), inner_loop_cnt);
                VTSS_I("INGRESS Port_no: %d -- Executing Mini-PLAN_E Recovery to reset TSP CMD FIFO \n", port_no);
                if ((rc = vtss_phy_ts_resolve_tsp_cmd_oos_eng_2_plan_e(vtss_state, port_no, eng_minE, pr)) != VTSS_RC_OK) {
                    VTSS_E("INGRESS Port_no: %d -- ERROR in Mini-PLAN_E Recovery \n", port_no);
                }
                VTSS_I("Ingress:TSP ODD_RECOVERY: Odd_recovery_Cnt: %d, inner_lp: %d, outer_lp: %d \n",
                       odd_recovery_cnt, inner_loop_cnt, count);

                if ((rc = vtss_phy_ts_tesla_tsp_fifo_sync_odd_recover(vtss_state, port_no, pr, channel, ingress, tsp_odd_recovery, odd_recovery_cnt)) != VTSS_RC_OK) {
                    VTSS_E("Port_no: %d; Ingress: ERROR Returned from Odd_Recovery! \n", port_no);
                }

                discIndex = 0;
                if ((rc = vtss_phy_ts_tesla_tsp_fifo_sync_detect_stage_1(vtss_state, port_no, pr, channel, ingress, &ing_OOS, &tsp_odd_recovery, &discIndex)) != VTSS_RC_OK) {
                    VTSS_E("Ingress: ERROR Returned from SYNC-Detect Stage_1! \n");
                }

                // If ing_OOS after two times thru, then we just do recovery here and fall out of loop
                if (tsp_odd_recovery) {
                    if (inner_loop_cnt == 0) {
                        odd_recovery_cnt++;
                    } else {
                        min_PlanE_recovery = TRUE;
                        VTSS_I("INGRESS -- Going to Mini-PLAN_E Recovery!! Entry: %d, ODD_RECOVERY_CNT: %d,  inner_lp: %d   outer_lp: %d \n", entry, odd_recovery_cnt, inner_loop_cnt, count);
                        break;
                    }
                } else if ((ing_OOS) && (!tsp_odd_recovery)) {
                    odd_recovery = FALSE;
                    even_recovery = TRUE;
                    VTSS_I("INGRESS OOS SEEN, exiting Odd recovery and starting even recovery");

                    VTSS_I("***********************************************************\n");
                    VTSS_I("----------- INGRESS -- OUT-OF-SYNC ----- Even recovery ----\n");
                    VTSS_I("***********************************************************\n");
                    break;
                } else {
                    VTSS_I("INGRESS -- TSP_ODD_RECOVERY(3) Flag is Clear - Checking FIFO Empty!! \n");
                    if ((rc = vtss_phy_ts_tesla_tsp_fifo_sync_prep_stage_0(vtss_state, port_no, pr, ingress, &entry)) != VTSS_RC_OK) {
                        VTSS_E("Unable to Setup Stage_0 Recovery, port_no: %d \n", port_no);
                        return (VTSS_RC_ERROR);
                    }

                    if (entry > 0) {
                        VTSS_E("INGRESS -- FIFO_NOT_EMPTY!! Entry: %d -- odd_recovery_cnt: %d \n", entry, odd_recovery_cnt);
                        min_PlanE_recovery = TRUE;
                        VTSS_I("INGRESS -- Going to Mini-PLAN_E Recovery!! Entry: %d, ODD_RECOVERY_CNT: %d,  inner_lp: %d   outer_lp: %d \n", entry, odd_recovery_cnt, inner_loop_cnt, count);
                    } else {
                        recovery_confirmed = TRUE;
                        VTSS_I("INGRESS -- Recovery-Confirmed!! Entry: %d, ODD_RECOVERY_CNT: %d,  inner_lp: %d   outer_lp: %d \n", entry, odd_recovery_cnt, inner_loop_cnt, count);
                    }
                    break;
                }

                VTSS_I("INGRESS -- Entry: %d, ODD_RECOVERY_CNT: %d,  inner_lp: %d   outer_lp: %d \n", entry, odd_recovery_cnt, inner_loop_cnt, count);

                inner_loop_cnt++;

            } while (inner_loop_cnt < VTSS_SYNC_ODD_RECOVERY_INNER_LOOP_COUNT);
        }  //TSP ODD recovery

        if ((recovery_confirmed) || (min_PlanE_recovery)) {
            VTSS_I("INGRESS:: <<<<<<<< Recv_Confirmed: 0x%x, min_PlanE: 0x%x >>>>>>>>> \n", recovery_confirmed, min_PlanE_recovery);
            break;
        }
        VTSS_I("INGRESS::Looping for complete Algorithm Count:%d ing_OOS:%s \n", count, ing_OOS ? "TRUE" : "FALSE" );

        recov_stage1 = TRUE;
    } while (count < VTSS_SYNC_RECOVERY_LOOP_COUNT);

    if ((!recovery_confirmed) || (min_PlanE_recovery)) {
        VTSS_I("******************************************************************\n");
        VTSS_I("----------- INGRESS -- OUT-OF-SYNC - Executing Mini-PLAN_E  ----- -\n");
        VTSS_I("******************************************************************\n");
        VTSS_I("INGRESS Port_no: %d -- Executing Mini-PLAN_E Recovery \n", port_no);
        if ((rc = vtss_phy_ts_resolve_tsp_cmd_oos_eng_2_plan_e(vtss_state, port_no, eng_minE, pr)) != VTSS_RC_OK) {
            VTSS_E("INGRESS Port_no: %d -- ERROR in Mini-PLAN_E Recovery \n", port_no);
        }
        mini_plane_done = TRUE;
        VTSS_I("******************************************************************\n");
        VTSS_I("----------- INGRESS -- Mini-PLAN_E Recovery Complete  ----------- -\n");
        VTSS_I("******************************************************************\n");
    }

    if (recovery_confirmed) {
        *hw_reset_required = FALSE;
    } else {
        *hw_reset_required = TRUE;
    }

    return (VTSS_RC_OK);
}

static vtss_rc vtss_phy_ts_tesla_ingress_tsp_fifo_sync_conf_epg(vtss_state_t *vtss_state, vtss_port_no_t   port_no, BOOL en)
{

    vtss_rc                         rc = VTSS_RC_OK;
    u16                             value = 0, value1 = 0;
    vtss_phy_ts_eng_conf_t          *eng_conf;
    vtss_port_no_t                  base_port_no;
    vtss_phy_ts_engine_t            eng_id = VTSS_PHY_TS_PTP_ENGINE_ID_0;
    u32                             pkt_cnt = 0;
    BOOL                            ingress = FALSE;

    vtss_miim_read_t   miim_read_func = vtss_state->init_conf.miim_read;
    vtss_miim_write_t  miim_write_func = vtss_state->init_conf.miim_write;

    if ((rc = vtss_phy_ts_base_port_get_priv(vtss_state, port_no, &base_port_no)) != VTSS_RC_OK) {
        VTSS_E(" Baseport get failed for Port-no: %d\n", port_no);
        return VTSS_RC_ERROR;
    }
    eng_conf = &vtss_state->phy_ts_port_conf[base_port_no].ingress_eng_conf[eng_id];
    switch (eng_conf->encap_type) {
    case VTSS_PHY_TS_ENCAP_ETH_PTP:
        VTSS_E("Port %d:: Encap type not supported", port_no);
        rc = VTSS_RC_ERROR;

        /* Need to generate Ethtype: 0x88F7 */
        break;
    case VTSS_PHY_TS_ENCAP_ETH_IP_PTP:
        if (eng_conf->flow_conf.flow_conf.ptp.ip1_opt.comm_opt.ip_mode == VTSS_PHY_TS_IP_VER_4) {
            miim_write_func(vtss_state, port_no, 0x1f, VTSS_PHY_PAGE_TR);
            miim_write_func(vtss_state, port_no, 0x10, 0xbe92); // read
            miim_write_func(vtss_state, port_no, 0x12, 0x000b);
            /* Adding for UDP packet */
            miim_write_func(vtss_state, port_no, 0x11, 0x1110); // setting bits 15:8 to UDP protocol (0x11)
            miim_write_func(vtss_state, port_no, 0x10, 0x9e92); // writeback to TR
            miim_write_func(vtss_state, port_no, 0x1f, VTSS_PHY_PAGE_STANDARD);
        } else {
            /* IPv6 Confing */
            VTSS_E("Port %d:: Encap type not supported", port_no);
            rc = VTSS_RC_ERROR;
        }
        break;
    case VTSS_PHY_TS_ENCAP_ETH_IP_IP_PTP:
    case VTSS_PHY_TS_ENCAP_ETH_ETH_PTP:
    case VTSS_PHY_TS_ENCAP_ETH_ETH_IP_PTP:
    case VTSS_PHY_TS_ENCAP_ETH_MPLS_IP_PTP:
    case VTSS_PHY_TS_ENCAP_ETH_MPLS_ETH_PTP:
    case VTSS_PHY_TS_ENCAP_ETH_MPLS_ETH_IP_PTP:
    case VTSS_PHY_TS_ENCAP_ETH_MPLS_ACH_PTP:
    case VTSS_PHY_TS_ENCAP_ETH_OAM:
    case VTSS_PHY_TS_ENCAP_ETH_ETH_OAM:
    case VTSS_PHY_TS_ENCAP_ETH_MPLS_ETH_OAM:
    case VTSS_PHY_TS_ENCAP_ETH_MPLS_ACH_OAM:
        VTSS_E("Port %d:: Encap type not supported \n", port_no);
        rc = VTSS_RC_ERROR;
        break;
    default:
        VTSS_E("Port %d:: Invalid encap type", port_no);
        rc = VTSS_RC_ERROR;
        break;
    }

    miim_write_func(vtss_state, port_no, 0x1f, VTSS_PHY_PAGE_EXTENDED);
    /* EPG Pattern to UDP port number for PTP packet */
    value = 0x13F;
    miim_write_func(vtss_state, port_no, 0x1e, value);
    if (en) {
        value = 0xC040;   /*  Jumbo Frames = 0x9C40, 125byte frames = 0xC040 - 125byte Packets w/ 0.096 us IPG, single burst */
        // value = 0xC440;  /* 125byte Packets w/ 8.192 us IPG, single burst*/
    }   else {
        value = 0x0;
        pkt_cnt = 300;
        vtss_phy_ts_tesla_tsp_fifo_epg_done(port_no, pkt_cnt, ingress); // This function just waits via MSLEEP
    }
    miim_write_func(vtss_state, port_no, 0x1d, value);

    miim_read_func(vtss_state, port_no, 0x1e, &value);
    miim_read_func(vtss_state, port_no, 0x1d, &value1);
    miim_write_func(vtss_state, port_no, 0x1f, VTSS_PHY_PAGE_STANDARD);
    VTSS_I("\nPort_No: %d, EPG Conf: Reading Ext:%d Reg:%02d: 0x1E : 0x%04X 0x1D : %04x en%d   \n",
           port_no, VTSS_PHY_PAGE_EXTENDED, 0x1e, value, value1, en );
    VTSS_MSLEEP(1);

    return rc;
}

static vtss_rc vtss_phy_ts_tesla_tsp_fifo_sync_det_read_ts_fifo(vtss_state_t *vtss_state,
                                                                vtss_port_no_t   port_no,
                                                                const vtss_debug_printf_t       pr,
                                                                BOOL *ing_OOS, BOOL *egr_OOS, u16 *fifodepth)
{

    vtss_rc                         rc = VTSS_RC_OK;
    vtss_port_no_t                  base_port_no = 0;
    vtss_port_no_t                  cfgport = 0;
    u32   loop_cnt = 5;
    u32   fifo_val[7] = {0, 0, 0, 0, 0, 0, 0}, value = 0;
    u32   depth = 10000; // max depth is 8, this ensures timestamp is checked on first pass through ts check loop
    BOOL  entry_found = FALSE, just_empty_fifo = FALSE;
    vtss_phy_ts_fifo_sig_mask_t sig_mask;
    u32   val_1st = 0, val_2nd = 0, egress_offset = 0;
    u32   egr_ns = 0, ing_ns = 0; /**< 4 bytes nano-sec part of Timestamp */
    u32   egr_32ns = 0, egr_32ns_last = 0;
    u32   egr_ts_dev = 0; /* Egress timestamp deviation from its expected value */
    u64   egr_ts_dis = 0; /* Difference between 2 successive egr timestamps */
    u32   egr_dis_ref = 1160;
    u32   ts_diff = 0;

    VTSS_I("\nPort_No: %d, Read FIFO\n", port_no);
    if ((rc = vtss_phy_ts_base_port_get_priv(vtss_state, port_no, &base_port_no)) != VTSS_RC_OK) {
        VTSS_E("Port %d:: Base Port get failed", port_no);
        return VTSS_RC_ERROR;
    }
    cfgport = port_no;
    sig_mask = vtss_state->phy_ts_port_conf[base_port_no].sig_mask;

    /* Loop reading the TSFIFO_0 register, until TS_EMPTY bit = 0 */
    do {
        VTSS_RC(VTSS_PHY_TS_READ_CSR(cfgport, VTSS_PHY_TS_PROC_BLK_ID(0),
                                     VTSS_PTP_EGR_IP_1588_TSFIFO_EGR_TSFIFO_0, &val_1st));
        VTSS_RC(VTSS_PHY_TS_READ_CSR(cfgport, VTSS_PHY_TS_PROC_BLK_ID(0),
                                     VTSS_PTP_EGR_IP_1588_TSFIFO_EGR_TSFIFO_0, &val_2nd));
        if ((!(val_1st & VTSS_F_PTP_EGR_IP_1588_TSFIFO_EGR_TSFIFO_0_EGR_TS_EMPTY)) &&
            (!(val_2nd & VTSS_F_PTP_EGR_IP_1588_TSFIFO_EGR_TSFIFO_0_EGR_TS_EMPTY))) {
            /* Entries found */
            entry_found = TRUE;
            break;
        }
        loop_cnt--;
    } while (loop_cnt > 0);


    if (entry_found) {
        VTSS_I("\nPort_No: %d, Valid FIFO entry found\n", port_no);
        do {
            VTSS_RC(VTSS_PHY_TS_READ_CSR(cfgport, VTSS_PHY_TS_PROC_BLK_ID(0),
                                         VTSS_PTP_EGR_IP_1588_TSFIFO_EGR_TSFIFO_0, &fifo_val[0]));
            if ((fifo_val[0] & VTSS_F_PTP_EGR_IP_1588_TSFIFO_EGR_TSFIFO_0_EGR_TS_EMPTY)) {
                break;
            }
            VTSS_RC(VTSS_PHY_TS_READ_CSR(cfgport, VTSS_PHY_TS_PROC_BLK_ID(0),
                                         VTSS_PTP_EGR_IP_1588_TSFIFO_EGR_TSFIFO_1, &fifo_val[1]));

            VTSS_RC(VTSS_PHY_TS_READ_CSR(cfgport, VTSS_PHY_TS_PROC_BLK_ID(0),
                                         VTSS_PTP_EGR_IP_1588_TSFIFO_EGR_TSFIFO_2, &fifo_val[2]));

            VTSS_RC(VTSS_PHY_TS_READ_CSR(cfgport, VTSS_PHY_TS_PROC_BLK_ID(0),
                                         VTSS_PTP_EGR_IP_1588_TSFIFO_EGR_TSFIFO_3, &fifo_val[3]));
            VTSS_RC(VTSS_PHY_TS_READ_CSR(cfgport, VTSS_PHY_TS_PROC_BLK_ID(0),
                                         VTSS_PTP_EGR_IP_1588_TSFIFO_EGR_TSFIFO_4, &fifo_val[4]));
            VTSS_RC(VTSS_PHY_TS_READ_CSR(cfgport, VTSS_PHY_TS_PROC_BLK_ID(0),
                                         VTSS_PTP_EGR_IP_1588_TSFIFO_EGR_TSFIFO_5, &fifo_val[5]));
            VTSS_RC(VTSS_PHY_TS_READ_CSR(cfgport, VTSS_PHY_TS_PROC_BLK_ID(0),
                                         VTSS_PTP_EGR_IP_1588_TSFIFO_EGR_TSFIFO_6, &fifo_val[6]));

            VTSS_I("\nPort_No: %d, Sig&TS:%08X%08X%08X%08X%08X%08X%08X\n", port_no, fifo_val[0], fifo_val[1], fifo_val[2], fifo_val[3],
                   fifo_val[4], fifo_val[5], fifo_val[6]);
            if (!just_empty_fifo) {
                egr_32ns = (fifo_val[0] & 0xFFFF) | ((fifo_val[1] & 0xFFFF) << 16);
                egr_ns = (egr_32ns & 0xFFFF); /* Retrieve lower 2 bytes for ing-ns comparison */

                if (sig_mask & VTSS_PHY_TS_FIFO_SIG_SEQ_ID) {
                    ing_ns = (fifo_val[3] & 0xFFFF);
                }
                VTSS_I("\nPort_no:%d, ing TS %08x Egr TS %08X fifo depth:%d signature:%x\n", port_no, ing_ns, egr_ns, depth, sig_mask);

                /* Calculate distance b/w 2 successive egress timestamps */
                if (depth != 10000) {/* This calculation is ommitted only for first entry */
                    if (egr_32ns_last < egr_32ns) {
                        egr_ts_dis = egr_32ns - egr_32ns_last;
                    } else {
                        egr_ts_dis = (VTSS_TS_FIFO_NANO_SEC_PER_1SEC_ROLLOVER + egr_32ns) - egr_32ns_last;
                    }
                    egr_ts_dev = ((egr_ts_dis > egr_dis_ref) ?
                                  (egr_ts_dis - egr_dis_ref) :
                                  (egr_dis_ref - egr_ts_dis));
                    if (egr_ts_dev > 10) {
                        *egr_OOS = TRUE;
                        egress_offset = 7 - depth;
                        VTSS_I("\negress out of sync detected at depth %d offset %d distance 0x%llx deviation 0x%x egr_32ns:0x%x egr_32ns_last:0x%x\n", depth, egress_offset, (long long unsigned int)egr_ts_dis, egr_ts_dev, egr_32ns, egr_32ns_last);
                    }
                }
                egr_32ns_last = egr_32ns;
                /* Ignore NS or 1 second Overflow, and ensure at least first 3 entries in TSFIFO skipped in case tsp FIFO out of sync */
                if (depth <= 5) { // first 3 entries in TSFIFO skipped
                    if (ing_ns < egr_ns) { // valid, no Overflow occurred
                        ts_diff =  egr_ns - ing_ns;
                        if ((ts_diff > 3000) || (ts_diff < 2000)) {
                            *ing_OOS = TRUE;
                        }
                        just_empty_fifo = TRUE;
                        VTSS_I("\nPort_No: %d, Ing TS:0x%04X Egr TS:0x%04X ts_diff: %d\n", port_no, ing_ns, egr_ns, ts_diff);
                    }
                    // else some kind of rollover occurred, so read next entry
                }
            }

            /* Read the TSFIFO_CSR register and check FIFO level associated with the last read of the TS_EMPTY status field of the TSFIFO_0 register */
            VTSS_RC(VTSS_PHY_TS_READ_CSR(cfgport, VTSS_PHY_TS_PROC_BLK_ID(0),
                                         VTSS_PTP_EGR_IP_1588_TSFIFO_EGR_TSFIFO_CSR, &value));
            depth = VTSS_X_PTP_EGR_IP_1588_TSFIFO_EGR_TSFIFO_CSR_EGR_TS_LEVEL(value);

        } while (depth > 0);  /* completely empty the FIFO to ensure no stale entries whenever recovery subsequently called */
        *fifodepth = depth;
    }
    return rc;
}


static vtss_rc vtss_phy_ts_tesla_tsp_fifo_sync_detect(vtss_state_t *vtss_state,
                                                      vtss_port_no_t  port_no,
                                                      const vtss_debug_printf_t       pr,
                                                      vtss_phy_ts_engine_channel_map_t   channel,
                                                      BOOL                             *ing_OOS,
                                                      BOOL                             *egr_OOS)
{
    vtss_rc                 rc = VTSS_RC_OK;
    u32                     value = 0;
    u16                     fifodepth = 0, loop = 3;

    do {

        /* Reset the FIFO before sending packets */
        value = 0;
        VTSS_RC(VTSS_PHY_TS_READ_CSR(port_no, VTSS_PHY_TS_PROC_BLK_ID(0),
                                     VTSS_PTP_EGR_IP_1588_TSFIFO_EGR_TSFIFO_CSR, &value));
        value |= VTSS_F_PTP_EGR_IP_1588_TSFIFO_EGR_TSFIFO_CSR_EGR_TS_FIFO_RESET;
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(port_no, VTSS_PHY_TS_PROC_BLK_ID(0),
                                      VTSS_PTP_EGR_IP_1588_TSFIFO_EGR_TSFIFO_CSR, &value));
        /* clear the FIFO reset*/
        value =  VTSS_PHY_TS_CLR_BITS(value,
                                      VTSS_F_PTP_EGR_IP_1588_TSFIFO_EGR_TSFIFO_CSR_EGR_TS_FIFO_RESET);

        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(port_no, VTSS_PHY_TS_PROC_BLK_ID(0),
                                      VTSS_PTP_EGR_IP_1588_TSFIFO_EGR_TSFIFO_CSR, &value));
        do {
            /*  Start EPG with 125 bytes frames, 0.096ns IPG, test mode UDP protocol burst */
            rc = vtss_phy_ts_tesla_ingress_tsp_fifo_sync_conf_epg(vtss_state, port_no, TRUE);
            if (rc != VTSS_RC_OK) {
                VTSS_E("Ingress TS Error Detect : EPG Config for Detect Failed, port-no: %d\n", port_no);
                break;
            }

            /*  Stop EPG  */
            rc = vtss_phy_ts_tesla_ingress_tsp_fifo_sync_conf_epg(vtss_state, port_no, FALSE);
            if (rc != VTSS_RC_OK) {
                VTSS_E("Ingress TS Error Detect : EPG Stop for Detect Failed, port-no: %d\n", port_no);
                break;
            }

            /*  Read last timestamp set in the timestamp FIFO and compare ingress and egress timestamp */
            *ing_OOS = FALSE;
            *egr_OOS = FALSE;
            fifodepth = 0;
            rc = vtss_phy_ts_tesla_tsp_fifo_sync_det_read_ts_fifo(vtss_state, port_no, pr, ing_OOS, egr_OOS, &fifodepth);
            if (rc != VTSS_RC_OK) {
                VTSS_E("Error in reading FIFO  %d \n", port_no);
                break;
            }
            loop--;
        } while ((fifodepth > 1) && (loop > 0));



        /*  If (egress - ingress) > internal PHY loop delay (~ 3us) the FIFO is out of sync */
        /*  irrespective of IPG choice */
        if ((*egr_OOS == TRUE) || (*ing_OOS == TRUE)) {
            if (*egr_OOS == TRUE) {
                VTSS_I("\nEgress TS FIFO Out of sync Error detected\n");
            } else if (*ing_OOS == TRUE) {
                VTSS_I("\nIngress TS FIFO out of sync Error detected\n");
            }
        } else {
            VTSS_I("\nFIFO's are in Sync on Port %d \n", port_no);
        }
    } while (0);
    return rc;

}


static vtss_rc vtss_phy_ts_tesla_fifo_sync_port_reset_priv(vtss_state_t                          *vtss_state,
                                                           const vtss_port_no_t                   port_no,
                                                           const vtss_debug_printf_t              pr,
                                                           const vtss_phy_ts_fifo_err_recovery_t *recov)
{
    vtss_rc                rc = VTSS_RC_OK;
    u16                    value = 0;
    u16                    channel_id = 0;
    u16                    reg = 0;
    vtss_mtimer_t          timer;
    u16                    micro_cmd_100fx = 0;

    /* Define the MIIM Read/Write functions to use from PHY Instance */
    vtss_miim_read_t       miim_read_func = vtss_state->init_conf.miim_read;
    vtss_miim_write_t      miim_write_func = vtss_state->init_conf.miim_write;

    /* Setup the Chip Channel_id which was saved in vtss_phy_ts_tesla_tsp_fifo_sync_priv()  */
    if (recov->phy_id_sav) {
        channel_id = recov->phy_id.channel_id;
    }

    if (vtss_state->phy_state[port_no].reset.media_if == VTSS_PHY_MEDIA_IF_CU) {
        VTSS_I("Cu mode-setting up Micro\n");
        // Setup media in micro program.
        miim_write_func(vtss_state, port_no, 0x1f, VTSS_PHY_PAGE_GPIO);
        // Turn off SerDes for 100Base-FX.
        miim_write_func(vtss_state, port_no, 0x12, 0x80f1 | ((1 << (channel_id)) << 8));
        if ((rc = vtss_phy_wait_for_micro_complete(vtss_state, port_no)) != VTSS_RC_OK) {
            VTSS_E("ERROR: Waiting for Micro-Complete during Media mode switch to Cu-setting - Turning OFF 100Base-FX SerDes\n");
        }
        // Turn off SerDes for 1000Base-X.
        miim_write_func(vtss_state, port_no, 0x1f, VTSS_PHY_PAGE_GPIO);
        miim_write_func(vtss_state, port_no, 0x12, 0x80e1 | ((1 << (channel_id)) << 8));
        if ((rc = vtss_phy_wait_for_micro_complete(vtss_state, port_no)) != VTSS_RC_OK) {
            VTSS_E("ERROR: Waiting for Micro-Complete during Media mode switch to Cu-setting - Turning OFF 1000Base-X SerDes\n");
        }
    } else {
        if ((vtss_state->phy_state[port_no].reset.media_if == VTSS_PHY_MEDIA_IF_FI_100FX) ||
            (vtss_state->phy_state[port_no].reset.media_if == VTSS_PHY_MEDIA_IF_AMS_CU_100FX) ||
            (vtss_state->phy_state[port_no].reset.media_if == VTSS_PHY_MEDIA_IF_AMS_FI_100FX)) {
            // Use to signal to micro program if the fiber is 100FX (Bit 4). Default is 1000BASE-x
            micro_cmd_100fx = 1 << 4;
        }
        // Setup media in micro program. Bit 8-11 is bit for the corresponding port (See TN1080)
        miim_write_func(vtss_state, port_no, 0x1f, VTSS_PHY_PAGE_GPIO);
        reg = (0x80C1 | ((1 << (channel_id)) << 8) | micro_cmd_100fx);
        miim_write_func(vtss_state, port_no, 0x12, reg);
        if ((rc = vtss_phy_wait_for_micro_complete(vtss_state, port_no)) != VTSS_RC_OK) {
            VTSS_E("Waiting for Micro Complete Tesla port:%d, Writing RegValue = 0x%x\n", port_no, reg);
        }
    }

    VTSS_MSLEEP(10);

    miim_write_func(vtss_state, port_no, 0x1f, VTSS_PHY_PAGE_STANDARD);
    miim_read_func(vtss_state, port_no, 0x17, &value);      // Extended PHY Control, Reg23
    value &= ~(VTSS_M_PHY_EXTENDED_PHY_CONTROL_MEDIA_OPERATING_MODE);   // Clear bits 8-10
    value &= ~(VTSS_F_PHY_EXTENDED_PHY_CONTROL_AMS_PREFERENCE);         // Clear bit 11
    value &= ~(VTSS_M_PHY_EXTENDED_PHY_CONTROL_AMS_OVERRIDE);           // Clear bits 7:6

    if (vtss_state->phy_state[port_no].reset.media_if == VTSS_PHY_MEDIA_IF_CU) {
        value |= VTSS_F_PHY_EXTENDED_PHY_CONTROL_MEDIA_OPERATING_MODE(0x0); // VTSS_PHY_MEDIA_IF_CU
        value |= VTSS_F_PHY_EXTENDED_PHY_CONTROL_AMS_PREFERENCE;            // AMS Preference = Cat5 Copper, but not in AMS mode
    } else if (vtss_state->phy_state[port_no].reset.media_if == VTSS_PHY_MEDIA_IF_SFP_PASSTHRU) {
        value |= VTSS_F_PHY_EXTENDED_PHY_CONTROL_MEDIA_OPERATING_MODE(0x1); // Protcol Transfer Mode = CuSFP
    } else if (vtss_state->phy_state[port_no].reset.media_if == VTSS_PHY_MEDIA_IF_FI_1000BX) {
        value |= VTSS_F_PHY_EXTENDED_PHY_CONTROL_MEDIA_OPERATING_MODE(0x2);
    } else if (vtss_state->phy_state[port_no].reset.media_if == VTSS_PHY_MEDIA_IF_FI_100FX) {
        value |= VTSS_F_PHY_EXTENDED_PHY_CONTROL_MEDIA_OPERATING_MODE(0x3); // VTSS_PHY_MEDIA_IF_FI_1000BX
    } else if (vtss_state->phy_state[port_no].reset.media_if == VTSS_PHY_MEDIA_IF_AMS_CU_PASSTHRU) {
        value |= VTSS_F_PHY_EXTENDED_PHY_CONTROL_MEDIA_OPERATING_MODE(0x5);
        value |= VTSS_F_PHY_EXTENDED_PHY_CONTROL_AMS_PREFERENCE;            // AMS Preference = Cat5 Copper
    } else if (vtss_state->phy_state[port_no].reset.media_if == VTSS_PHY_MEDIA_IF_AMS_FI_PASSTHRU) {
        value |= VTSS_F_PHY_EXTENDED_PHY_CONTROL_MEDIA_OPERATING_MODE(0x5); // AMS Preference = SerDes SFP Prtocol Transfer Mode
    } else if (vtss_state->phy_state[port_no].reset.media_if == VTSS_PHY_MEDIA_IF_AMS_CU_1000BX) {
        value |= VTSS_F_PHY_EXTENDED_PHY_CONTROL_MEDIA_OPERATING_MODE(0x6);
        value |= VTSS_F_PHY_EXTENDED_PHY_CONTROL_AMS_PREFERENCE;            // AMS Preference = Cat5 Copper
    } else if (vtss_state->phy_state[port_no].reset.media_if == VTSS_PHY_MEDIA_IF_AMS_FI_1000BX) {
        value |= VTSS_F_PHY_EXTENDED_PHY_CONTROL_MEDIA_OPERATING_MODE(0x6); // AMS Preference = SerDes SFP 1000BaseX Fiber
    } else if (vtss_state->phy_state[port_no].reset.media_if == VTSS_PHY_MEDIA_IF_AMS_CU_100FX) {
        value |= VTSS_F_PHY_EXTENDED_PHY_CONTROL_MEDIA_OPERATING_MODE(0x7);
        value |= VTSS_F_PHY_EXTENDED_PHY_CONTROL_AMS_PREFERENCE;            // AMS Preference = Cat5 Copper
    } else if (vtss_state->phy_state[port_no].reset.media_if == VTSS_PHY_MEDIA_IF_AMS_FI_100FX) {
        value |= VTSS_F_PHY_EXTENDED_PHY_CONTROL_MEDIA_OPERATING_MODE(0x7); // AMS Preference = SerDes SFP 100FX Fiber
    } else {
        value |= VTSS_F_PHY_EXTENDED_PHY_CONTROL_MEDIA_OPERATING_MODE(0x2); // VTSS_PHY_MEDIA_IF_FI_1000BX
    }

    /* Restore any Forced AMS Values */
    if (recov->phy_control_reg_sav) {
        u16  ams_force;
        ams_force = (recov->phy_control_reg & VTSS_M_PHY_EXTENDED_PHY_CONTROL_AMS_OVERRIDE) >> 6;
        if ((ams_force > 0) && (ams_force < 3)) {
            value |= VTSS_F_PHY_EXTENDED_PHY_CONTROL_AMS_OVERRIDE(ams_force);
        }
    }

    miim_write_func(vtss_state, port_no, 0x17, value);      // Extended PHY Control, Reg23

    reg = VTSS_F_PHY_MODE_CONTROL_SW_RESET | VTSS_F_PHY_MODE_CONTROL_LOOP | (1 << 6);
    VTSS_I("Soft resetting Tesla port:%d, Writing RegValue = 0x%x\n", port_no, reg);
    PHY_WR_PAGE(vtss_state, port_no, VTSS_PHY_MODE_CONTROL, reg);  // Tesla PHY Only - Writing 0xc040
    VTSS_MSLEEP(1);/* pause after reset */
    VTSS_MTIMER_START(&timer, 5000); /* Wait up to 5 seconds */
    while (1) {
        if (PHY_RD_PAGE(vtss_state, port_no, VTSS_PHY_MODE_CONTROL, &reg) == VTSS_RC_OK && (reg & VTSS_F_PHY_MODE_CONTROL_SW_RESET) == 0) {
            break;
        }
        VTSS_MSLEEP(1);
        if (VTSS_MTIMER_TIMEOUT(&timer)) {
            VTSS_E("port_no %u, reset timeout, reg = 0x%X\n", port_no, reg);
        }
    }
    VTSS_MTIMER_CANCEL(&timer);

    return rc;
}

// The purpose is to force the Link down to Isolate this port while the recovery is running without generating RUNT frames
// during the transition into and out of the recovery algorithm

#define VTSS_TS_FIFO_SYNC_MAX_LOOPS  (2)   // #define the Max Number of loops Egress and Ingress Recovery will be tried
#define VTSS_TS_ENG0_ENABLED 0x11
#define VTSS_TS_ENG1_ENABLED 0x22
#define VTSS_TS_ENG2_ENABLED 0x44

static vtss_rc vtss_phy_ts_tesla_tsp_fifo_sync_priv(vtss_state_t *vtss_state,
                                                    const vtss_port_no_t            port_no,
                                                    const vtss_debug_printf_t       pr,
                                                    const vtss_phy_ts_fifo_conf_t   *fifo_conf,
                                                    BOOL                           *OOS,
                                                    vtss_phy_ts_fifo_err_recovery_t *recov)
{
    vtss_phy_ts_engine_channel_map_t chmask = VTSS_PHY_TS_ENG_FLOW_VALID_FOR_CH0;

    vtss_rc                 rc = VTSS_RC_OK;
    vtss_miim_read_t        miim_read_func;
    vtss_miim_write_t       miim_write_func;
    vtss_phy_conf_t         tmp_port_conf;
    vtss_phy_ts_event_t     event_status = 0;
    vtss_phy_type_t         phy_id;
    vtss_phy_ts_blk_id_t    blk_id = VTSS_PHY_TS_ANA_BLK_ID_ING_0;
    vtss_port_no_t          base_port_no, cfgport;
    u32                     temp32 = 0;
    u32                     si_cfg = 0;
    u32                     value32 = 0;
    u32                     sport_mask = 0;
    u32                     sport_val = 0;
    u32                     dport_mask = 0;
    u32                     dport_val = 0;
    u16                     channel_id = 0;
    u16                     counter1 = 0;
    u16                     counter2 = 0;
    u16                     counter3 = 0;
    u16                     value = 0;
    u16                     media_operating_mode = 0;
    u8                      flow_id = 7; /* Using last flow for Recovery process */
    BOOL                    hw_reset = FALSE;
    BOOL                    *hw_reset_required = &hw_reset;
    BOOL                    ing_reset = FALSE;
    BOOL                    egr_reset = FALSE;
    BOOL                    is_mac_if = FALSE;
    BOOL                    enable_tst_mode = FALSE;
    BOOL                    ing_OOS = FALSE, egr_OOS = FALSE;
    vtss_phy_ts_engine_t    eng_id, eng_minE;
    BOOL                    detect_only = FALSE;

    /* Set OOS Flag to FALSE until we start recovery and set it otherwise */
    *OOS = FALSE;
    recov->recovery_required = FALSE;

    memset(&phy_id, 0, sizeof(vtss_phy_type_t));

    miim_read_func = vtss_state->init_conf.miim_read;
    miim_write_func = vtss_state->init_conf.miim_write;

    if ((rc = vtss_phy_id_get_priv(vtss_state, port_no, &phy_id)) != VTSS_RC_OK) {
        VTSS_E("Ingress TS Error Detect : Failed to get channel ID, port-no: %d\n", port_no);
        return VTSS_RC_ERROR;
    }

    VTSS_I("PHY Identified:PortID:%d PHY: %d Rev:%d Channel:%d  Channel count:%d Baseport:%d\n",
           port_no, phy_id.part_number, phy_id.revision, phy_id.channel_id, phy_id.port_cnt, phy_id.base_port_no);

    channel_id = phy_id.channel_id;
    if (!((phy_id.part_number == VTSS_PHY_TYPE_8574) || (phy_id.part_number == VTSS_PHY_TYPE_8572))) {
        //Return silently as we don't want to throw the error for other PHYs
        VTSS_I("Not a Tesla PHY! - Tesla OOS Recovery not applicable - Returning... ");
        return VTSS_RC_OK;
    }


    VTSS_I("Entering Tesla OOS Recovery Sequence: FIFO Re-Sync, port_no: %d\n", port_no);

    recov->port_no = port_no; /* Save for Recovery in event of ERROR-Exit */
    recov->phy_id = phy_id;   /* Save for Recovery in event of ERROR-Exit */
    recov->phy_id_sav = TRUE; /* Save for Recovery in event of ERROR-Exit */

    /* Check the Tesla Chip Rev. Don't run the resync code for revE*/
    value32 = 0;
    VTSS_PHY_TS_SPI_PAUSE_PRIV(port_no);
    if ((rc = VTSS_PHY_TS_READ_CSR(port_no, VTSS_PHY_TS_PROC_BLK_ID(0),
                                   VTSS_PTP_IP_1588_TOP_CFG_STAT_VERSION_CODE, &value32)) != VTSS_RC_OK) {
        pr("CSR Access failed: VTSS_PTP_IP_1588_TOP_CFG_STAT_VERSION_CODE \n");
        VTSS_PHY_TS_SPI_UNPAUSE_PRIV(port_no);
        return VTSS_RC_ERROR;
    }
    VTSS_PHY_TS_SPI_UNPAUSE_PRIV(port_no);

    /* In API 4.20 and API 4.49, the 4th param passed is a BOOLEAN instead of a pointer to fifo_conf */
    /* Therefore, the Application written for API 4.20 and 4.49 would have the 4th Param to be 0=FALSE */
    /* A 0=NULL Ptr if passed as fifo_conf and be considered the same as a Boolean FALSE, as both evaluate to 0 */
    if (fifo_conf) {
        if (fifo_conf->skip_rev_check) {
            pr("Skipping Tesla Rev_Check \n");
        } else {
            if ((value32 & 0xff) >= VTSS_PTP_IP_1588_VERSION_2_1) {
                pr("Tesla Rev-E: FIFO Re-Sync not needed \n");
                return VTSS_RC_OK;
            } else {
                pr("Tesla Rev-C/D: Running FIFO Re-Sync..\n");
            }
        }
    } else {
        if ((value32 & 0xff) >= VTSS_PTP_IP_1588_VERSION_2_1) {
            pr("Tesla Rev-E: FIFO Re-Sync not needed \n");
            return VTSS_RC_OK;
        } else {
            pr("Tesla Rev-C/D: fifo_conf=NULL; Running FIFO Re-Sync..\n");
        }
    }

    /* *********************************************************************************** */
    /* The following code checks Config to see if there was an Early Exit previously       */
    /* *********************************************************************************** */
    if ((rc = vtss_phy_ts_tesla_oos_recovery_disable_priv(vtss_state, port_no, pr)) != VTSS_RC_OK) {
        VTSS_E("ERROR: PHY WAS CONFIGURED FOR OOS RECOVERY! UNABLE to fully DISABLED - HW Reset may be Required <<<< port: %d", port_no);
    }

    /* *********************************************************************************** */
    /* The following code checks the MEDIA I/f Config / Media Operating Mode in SW and HW  */
    /* *********************************************************************************** */
    value = 0;
    VTSS_RC(PHY_RD_PAGE(vtss_state, port_no, VTSS_PHY_EXTENDED_PHY_CONTROL, &value));
    recov->phy_control_reg = value; /* Save the Operating mode for Recovery in event of ERROR-Exit */
    recov->phy_control_reg_sav = TRUE; /* Save the Operating mode for Recovery in event of ERROR-Exit */
    media_operating_mode = (value & VTSS_M_PHY_EXTENDED_PHY_CONTROL_MEDIA_OPERATING_MODE) >> 8;
    VTSS_I("OOS Entry: Media i/f Config in PHY_Inst:0x%x, in HW_Reg23:0x%04x OOS Patch port-no: %d \n", vtss_state->phy_state[port_no].reset.media_if, value, port_no);
    pr("OOS Entry: Media i/f Config in PHY_Inst:0x%x, in HW_Reg23:0x%04x OOS Patch port-no: %d \n", vtss_state->phy_state[port_no].reset.media_if, value, port_no);

    /* If this is defined, then Media i/f check is preformed, CuSFP is not supported */
    /* This is configurable, based upon the board layout, so the User may set or clear this option */
//#define _PROTO_TRANSFER_MEDIA_IF_MODE_NOT_SUPPORT  /* If Defined, CuSFP Media is not supported */
#undef _PROTO_TRANSFER_MEDIA_IF_MODE_NOT_SUPPORT     /* If UnDefined, CuSFP Media is supported */

#ifdef _PROTO_TRANSFER_MEDIA_IF_MODE_NOT_SUPPORT
    /* Only Execute this code if this port if Fiber Media i/f */
    if ((vtss_state->phy_state[port_no].reset.media_if == VTSS_PHY_MEDIA_IF_FI_100FX) ||
        (vtss_state->phy_state[port_no].reset.media_if == VTSS_PHY_MEDIA_IF_AMS_CU_100FX) ||
        (vtss_state->phy_state[port_no].reset.media_if == VTSS_PHY_MEDIA_IF_AMS_FI_100FX) ||
        (vtss_state->phy_state[port_no].reset.media_if == VTSS_PHY_MEDIA_IF_FI_1000BX) ||
        (vtss_state->phy_state[port_no].reset.media_if == VTSS_PHY_MEDIA_IF_AMS_CU_1000BX) ||
        (vtss_state->phy_state[port_no].reset.media_if == VTSS_PHY_MEDIA_IF_AMS_FI_1000BX) ||
        (vtss_state->phy_state[port_no].reset.media_if == VTSS_PHY_MEDIA_IF_CU)) {
        VTSS_I("OOS Check Supported for Media i/f: 0x%x on port-no: %d ",
               vtss_state->phy_state[port_no].reset.media_if, port_no);
    } else {
        VTSS_I("INVALID Request!  OOS Check not Supported for Media i/f: 0x%x on port-no: %d ",
               vtss_state->phy_state[port_no].reset.media_if, port_no);
        return VTSS_RC_INV_STATE;
    }

    if ((media_operating_mode == 0x1) || (media_operating_mode == 0x5)) {
        VTSS_E("INVALID Request!  OOS Check - Media Op Mode is NOT Supported (Protocol-Transfer) Media i/f: 0x%x  MediaOpMode_Reg23:0x%x  on port-no: %d ", vtss_state->phy_state[port_no].reset.media_if, media_operating_mode, port_no);
        pr("INVALID Request!  OOS Check - Media Op Mode is NOT Supported (Protocol-Transfer) Media i/f: 0x%x  MediaOpMode_Reg23:0x%x  on port-no: %d ", vtss_state->phy_state[port_no].reset.media_if, media_operating_mode, port_no);
        return VTSS_RC_INV_STATE;
    }
#endif  // End of _CHECK_MEDIA_IF_MODE
    /* End of Media i/f check, CuSFP -> protocol transfer mode is not supported */

    if (vtss_state->phy_ts_port_conf[port_no].port_ts_init_done == FALSE) {
        VTSS_I("Port ts not initialized! port: %d", port_no);
        return VTSS_RC_OK;
    }

    if ((rc = vtss_phy_ts_base_port_get_priv(vtss_state, port_no, &base_port_no)) != VTSS_RC_OK) {
        VTSS_E(" Baseport get failed for Port-no: %d\n", port_no);
        return VTSS_RC_ERROR;
    }
    cfgport = base_port_no;

    /* Check whether PTP-IP-ETH encapsulation is enabled on the engine */
    /* Note: If fifo_conf == NULL, that appears to be the same as a Boolean "FALSE" */
    if (fifo_conf == NULL) {
        eng_id = VTSS_PHY_TS_PTP_ENGINE_ID_0;
    } else {
        eng_id = (fifo_conf->eng_recov > VTSS_PHY_TS_PTP_ENGINE_ID_1) ?
                 VTSS_PHY_TS_PTP_ENGINE_ID_0 : fifo_conf->eng_recov;
    }

    if ((vtss_state->phy_ts_port_conf[base_port_no].ingress_eng_conf[eng_id].eng_used == FALSE) ||
        (vtss_state->phy_ts_port_conf[base_port_no].ingress_eng_conf[eng_id].encap_type
         != VTSS_PHY_TS_ENCAP_ETH_IP_PTP)) {
        VTSS_I(" PTP-IP-ETH encapsulation not enabled to run OOS check on engine:%d\n", eng_id);
        return VTSS_RC_OK;
    }

    if (!(VTSS_PHY_TS_FIFO_SIG_SEQ_ID & vtss_state->phy_ts_port_conf[base_port_no].sig_mask) ||
        !(VTSS_PHY_TS_FIFO_SIG_SOURCE_PORT_ID & vtss_state->phy_ts_port_conf[base_port_no].sig_mask)) {
        VTSS_I(" Signature for Sequence-id and Source port-id need to be set for running OOS API\n");
        return VTSS_RC_INV_STATE;
    }

    /* Check if 1588 clock is enabled or not */
    VTSS_PHY_TS_SPI_PAUSE_PRIV(port_no);
    value32 = 0;
    if ((rc = VTSS_PHY_TS_READ_CSR(port_no, VTSS_PHY_TS_PROC_BLK_ID(0),
                                   VTSS_PTP_IP_1588_TOP_CFG_STAT_INTERFACE_CTL, &value32)) != VTSS_RC_OK) {
        pr("CSR Access failed: VTSS_PTP_IP_1588_TOP_CFG_STAT_INTERFACE_CTL \n");
        VTSS_PHY_TS_SPI_UNPAUSE_PRIV(port_no);
        return VTSS_RC_ERROR;
    }

    if (value32 & VTSS_F_PTP_IP_1588_TOP_CFG_STAT_INTERFACE_CTL_CLK_ENA) {
        VTSS_I("1588 Clk is enabled - OOS Check Supported ");
    } else {
        VTSS_E("1588 Clk is Disabled - OOS Check NOT Supported - Returning!");
        VTSS_PHY_TS_SPI_UNPAUSE_PRIV(port_no);
        return VTSS_RC_OK;
    }

    /* Check if Engine-0 or Engine-1 are enabled or not */
    value32 = 0;
    if ((rc = VTSS_PHY_TS_READ_CSR(port_no, VTSS_PHY_TS_PROC_BLK_ID_0,
                                   VTSS_PTP_IP_1588_TOP_CFG_STAT_ANALYZER_MODE, &value32)) != VTSS_RC_OK) {
        pr("CSR Access failed: Initial Reading VTSS_PTP_IP_1588_TOP_CFG_STAT_ANALYZER_MODE \n");
        VTSS_PHY_TS_SPI_UNPAUSE_PRIV(port_no);
        return VTSS_RC_ERROR;
    }
    recov->ana_mode = value32;
    recov->ana_mode_sav = TRUE;
    VTSS_I("VTSS_PTP_IP_1588_TOP_CFG_STAT_ANALYZER_MODE:%x port_no:%d\n", value32, port_no);

    if (fifo_conf == NULL) {
        /* Fixed Config */
        detect_only = FALSE;
        eng_id = VTSS_PHY_TS_PTP_ENGINE_ID_0;
        eng_minE = VTSS_PHY_TS_OAM_ENGINE_ID_2A;
        VTSS_I("FIFO Configuration for OOS_Recovery is NULL: Recov_Eng:Eng_0  Mini_E_Eng:Eng_2A port_no: %d\n", port_no);
    } else {
        detect_only = fifo_conf->detect_only;
        eng_id = fifo_conf->eng_recov;
        eng_minE = fifo_conf->eng_minE;
        VTSS_I("FIFO OOS Recovery Detect_only:%s Recovery Engine:%d Recovery Mini-Engine:%d\n",
               detect_only ? "TRUE" : "FALSE", eng_id, eng_minE);
    }

    /* Check if Engine-0 or Engine-1 are enabled or not */
    /* Check if the Designated Eng for Primary OOS Recovery is configured for ETH-IPv4-PTP */
    switch (eng_id) {
    case VTSS_PHY_TS_PTP_ENGINE_ID_0:
        if ((value32 & VTSS_TS_ENG0_ENABLED) == VTSS_TS_ENG0_ENABLED) {
            pr("Engine Id:%d, Designated for Primary OOS Recovery (eng_recov) is Enabled! \n", eng_id);
        } else {
            pr("Engine Id:%d, Designated for Primary OOS Recovery (eng_recov) is Disabled! FORCE-Enabling NOW! \n", eng_id);
            VTSS_I("Engine Id:%d, Designated for Primary OOS Recovery (eng_recov) is Disabled! FORCE-Enabling NOW! \n", eng_id);
            value32 |= VTSS_TS_ENG0_ENABLED;
            if ((rc = VTSS_PHY_TS_WRITE_CSR(port_no, VTSS_PHY_TS_PROC_BLK_ID_0,
                                            VTSS_PTP_IP_1588_TOP_CFG_STAT_ANALYZER_MODE, &value32)) != VTSS_RC_OK) {
                pr("CSR Access failed: Initial Reading VTSS_PTP_IP_1588_TOP_CFG_STAT_ANALYZER_MODE \n");
                VTSS_PHY_TS_SPI_UNPAUSE_PRIV(port_no);
                return VTSS_RC_ERROR;
            }
        }

        if ((rc = VTSS_PHY_TS_READ_CSR(port_no, VTSS_PHY_TS_ANA_BLK_ID_ING_0,
                                       VTSS_ANA_ETH1_NXT_PROTOCOL_ETH1_ETYPE_MATCH, &value32)) != VTSS_RC_OK) {
            pr("CSR Access failed: Initial Reading VTSS_ANA_ETH1_NXT_PROTOCOL_ETH1_ETYPE_MATCH \n");
            VTSS_PHY_TS_SPI_UNPAUSE_PRIV(port_no);
            return VTSS_RC_ERROR;
        }

        if (value32 == 0x0800) {   /* IPv4 */
            pr("Engine Id:%d, Eng_0 designated for Primary OOS Recovery, ENCAP is ETH-IP-PTP (0x%x)! \n", eng_id, value32);
            VTSS_I("Engine Id:%d, Eng_0 designated for Primary OOS Recovery, ENCAP is ETH-IP-PTP (0x%x)! \n", eng_id, value32);
        } else {
            pr("INVALID CONFIG: Engine Id:%d, Eng designated for Primary OOS Recovery, ENCAP is NOT ETH-IP-PTP (0x%x)! \n", eng_id, value32);
            VTSS_E("INVALID CONFIG: Engine Id:%d, Eng designated for Primary OOS Recovery, ENCAP is NOT ETH-IP-PTP (0x%x)! \n", eng_id, value32);
            VTSS_PHY_TS_SPI_UNPAUSE_PRIV(port_no);
            return VTSS_RC_ERROR;
        }

        /* Ingress - If UDP Sport_MASK is Non-Zero, We have to check the Value, it should be 319 or 0x013F  */
        if ((rc = VTSS_PHY_TS_READ_CSR(port_no, VTSS_PHY_TS_ANA_BLK_ID_ING_0,
                                       VTSS_ANA_IP1_NXT_PROTOCOL_IP1_PROT_MATCH_2_UPPER,
                                       &value32)) != VTSS_RC_OK) {
            pr("CSR Access failed: Initial Reading VTSS_ANA_IP1_NXT_PROTOCOL_IP1_PROT_MATCH_2_UPPER \n");
            VTSS_PHY_TS_SPI_UNPAUSE_PRIV(port_no);
            return VTSS_RC_ERROR;
        }
        sport_val = (value32 & 0xffff0000) >> 16;
        dport_val = (value32 & 0x0000ffff);

        if ((rc = VTSS_PHY_TS_READ_CSR(port_no, VTSS_PHY_TS_ANA_BLK_ID_ING_0,
                                       VTSS_ANA_IP1_NXT_PROTOCOL_IP1_PROT_MASK_2_UPPER,
                                       &value32)) != VTSS_RC_OK) {
            pr("CSR Access failed: Initial Reading VTSS_ANA_IP1_NXT_PROTOCOL_IP1_PROT_MASK_2_UPPER \n");
            VTSS_PHY_TS_SPI_UNPAUSE_PRIV(port_no);
            return VTSS_RC_ERROR;
        }
        sport_mask = (value32 & 0xffff0000) >> 16;
        dport_mask = (value32 & 0x0000ffff);

        VTSS_I("Engine Id:%d, INGRESS: OOS Recovery, ETH-IP-PTP UDP Sport_M: 0x%04x, Sport_Val:0x%04x, Dport_M:0x%04x, Dport_Val:0x%04x",
               eng_id, sport_mask, sport_val, dport_mask, dport_val);
        pr("Engine Id:%d, INGRESS: OOS Recovery, ETH-IP-PTP UDP Sport_M: 0x%04x, Sport_Val:0x%04x, Dport_M:0x%04x, Dport_Val:0x%04x\n",
           eng_id, sport_mask, sport_val, dport_mask, dport_val);

        /* If the UDP Sport_Mask is set, the only value that can be used for OOS Recovery to work is 319 or 0x013F */
        if ((sport_mask)) {
            /* Must check for UDP Value, but also Apply the Mask to ensure Both are correct */
            if ((sport_val & sport_mask) == 319) {
                VTSS_I("Engine Id:%d, ING: Primary OOS Recovery, ENCAP is ETH-IP-PTP,  UDP Sport_Val: 0x%04x ", eng_id, sport_val);
            } else {
                pr("ERROR in Config: Engine Id:%d, ING: Primary OOS Recovery, ENCAP is ETH-IP-PTP, UDP Sport_Val != 0x013F Sport_Val: 0x%04x \n", eng_id, sport_val);
                VTSS_E("ERROR in Config: Engine Id:%d, ING: Primary OOS Recovery, ENCAP is ETH-IP-PTP, UDP Sport_Val != 0x013F Sport_Val: 0x%04x ", eng_id, sport_val);
                VTSS_PHY_TS_SPI_UNPAUSE_PRIV(port_no);
                return VTSS_RC_ERROR;
            }
        } else {
            pr("Engine Id:%d, ING: Primary OOS Recovery, ENCAP is ETH-IP-PTP,  UDP Sport_MASK: 0x%04x \n", eng_id, sport_mask);
            VTSS_I("Engine Id:%d, ING: Primary OOS Recovery, ENCAP is ETH-IP-PTP,  UDP Sport_MASK: 0x%04x ", eng_id, sport_mask);
        }

        /* Check UDP Dest Port, For PTP Setup, It is required to be 319 or 0x013F */
        if ((dport_val & dport_mask) != 319) {
            pr("ERROR in Config: Engine Id:%d, ING: Primary OOS Recovery, ENCAP is ETH-IP-PTP, UDP Dport_Val != 0x013F Dport_Val: 0x%04x \n", eng_id, dport_val);
            VTSS_E("ERROR in Config: Engine Id:%d, ING: Primary OOS Recovery, ENCAP is ETH-IP-PTP, UDP Dport_Val != 0x013F Dport_Val: 0x%04x ", eng_id, dport_val);
            VTSS_PHY_TS_SPI_UNPAUSE_PRIV(port_no);
            return VTSS_RC_ERROR;
        }

        /* Egress - If UDP Sport_MASK is Non-Zero, We have to check the Value, it should be 319 or 0x013f  */
        if ((rc = VTSS_PHY_TS_READ_CSR(port_no, VTSS_PHY_TS_ANA_BLK_ID_EGR_0,
                                       VTSS_ANA_IP1_NXT_PROTOCOL_IP1_PROT_MATCH_2_UPPER,
                                       &value32)) != VTSS_RC_OK) {
            pr("CSR Access failed: Initial Reading VTSS_ANA_IP1_NXT_PROTOCOL_IP1_PROT_MATCH_2_UPPER \n");
            VTSS_PHY_TS_SPI_UNPAUSE_PRIV(port_no);
            return VTSS_RC_ERROR;
        }
        sport_val = (value32 & 0xffff0000) >> 16;
        dport_val = (value32 & 0x0000ffff);

        if ((rc = VTSS_PHY_TS_READ_CSR(port_no, VTSS_PHY_TS_ANA_BLK_ID_EGR_0,
                                       VTSS_ANA_IP1_NXT_PROTOCOL_IP1_PROT_MASK_2_UPPER,
                                       &value32)) != VTSS_RC_OK) {
            pr("CSR Access failed: Initial Reading VTSS_ANA_IP1_NXT_PROTOCOL_IP1_PROT_MASK_2_UPPER \n");
            VTSS_PHY_TS_SPI_UNPAUSE_PRIV(port_no);
            return VTSS_RC_ERROR;
        }
        sport_mask = (value32 & 0xffff0000) >> 16;
        dport_mask = (value32 & 0x0000ffff);

        VTSS_I("Engine Id:%d, EGRESS: OOS Recovery, ETH-IP-PTP UDP Sport_M: 0x%04x, Sport_Val:0x%04x, Dport_M:0x%04x, Dport_Val:0x%04x",
               eng_id, sport_mask, sport_val, dport_mask, dport_val);
        pr("Engine Id:%d, EGRESS: OOS Recovery, ETH-IP-PTP UDP Sport_M: 0x%04x, Sport_Val:0x%04x, Dport_M:0x%04x, Dport_Val:0x%04x\n",
           eng_id, sport_mask, sport_val, dport_mask, dport_val);

        /* If the UDP Sport_Mask is set, the only value that can be used for OOS Recovery to work is 319 or 0x013F */
        if ((sport_mask)) {
            /* Must check for UDP Value, but also Apply the Mask to ensure Both are correct */
            if ((sport_val & sport_mask) == 319) {
                VTSS_I("Engine Id:%d, EGR: Primary OOS Recovery, ENCAP is ETH-IP-PTP,  UDP Sport_Val: 0x%04x ", eng_id, sport_val);
            } else {
                pr("ERROR in Config: Engine Id:%d, EGR: Primary OOS Recovery, ENCAP is ETH-IP-PTP, UDP Sport_Val != 0x013F Sport_Val: 0x%04x \n", eng_id, sport_val);
                VTSS_E("ERROR in Config: Engine Id:%d, EGR: Primary OOS Recovery, ENCAP is ETH-IP-PTP, UDP Sport_Val != 0x013F Sport_Val: 0x%04x ", eng_id, sport_val);
                VTSS_PHY_TS_SPI_UNPAUSE_PRIV(port_no);
                return VTSS_RC_ERROR;
            }
        } else {
            pr("Engine Id:%d, EGR: Primary OOS Recovery, ENCAP is ETH-IP-PTP,  UDP Sport_MASK: 0x%04x \n", eng_id, sport_mask);
            VTSS_I("Engine Id:%d, EGR: Primary OOS Recovery, ENCAP is ETH-IP-PTP,  UDP Sport_MASK: 0x%04x ", eng_id, sport_mask);
        }

        /* Check UDP Dest Port, For PTP Setup, It is required to be 319 or 0x013F */
        if ((dport_val & dport_mask) != 319) {
            pr("ERROR in Config: Engine Id:%d, EGR: Primary OOS Recovery, ENCAP is ETH-IP-PTP, UDP Dport_Val != 0x013F Dport_Val: 0x%04x \n", eng_id, dport_val);
            VTSS_E("ERROR in Config: Engine Id:%d, EGR: Primary OOS Recovery, ENCAP is ETH-IP-PTP, UDP Dport_Val != 0x013F Dport_Val: 0x%04x ", eng_id, dport_val);
            VTSS_PHY_TS_SPI_UNPAUSE_PRIV(port_no);
            return VTSS_RC_ERROR;
        }
        break;

    case VTSS_PHY_TS_PTP_ENGINE_ID_1:
        if ((value32 & VTSS_TS_ENG1_ENABLED) == VTSS_TS_ENG1_ENABLED) {
            pr("Engine Id:%d, Designated for Primary OOS Recovery (eng_recov) is Enabled! \n", eng_id);
            VTSS_I("Engine Id:%d, Designated for Primary OOS Recovery (eng_recov) is Enabled! \n", eng_id);
        } else {
            pr("Engine Id:%d, Designated for Primary OOS Recovery (eng_recov) is Disabled! FORCE-Enabling NOW! \n", eng_id);
            VTSS_I("Engine Id:%d, Designated for Primary OOS Recovery (eng_recov) is Disabled! FORCE-Enabling NOW! \n", eng_id);
            value32 |= VTSS_TS_ENG1_ENABLED;
            if ((rc = VTSS_PHY_TS_WRITE_CSR(port_no, VTSS_PHY_TS_PROC_BLK_ID_0,
                                            VTSS_PTP_IP_1588_TOP_CFG_STAT_ANALYZER_MODE, &value32)) != VTSS_RC_OK) {
                pr("CSR Access failed: Initial Reading VTSS_PTP_IP_1588_TOP_CFG_STAT_ANALYZER_MODE \n");
                VTSS_PHY_TS_SPI_UNPAUSE_PRIV(port_no);
                return VTSS_RC_ERROR;
            }
        }

        if ((rc = VTSS_PHY_TS_READ_CSR(port_no, VTSS_PHY_TS_ANA_BLK_ID_ING_1,
                                       VTSS_ANA_ETH1_NXT_PROTOCOL_ETH1_ETYPE_MATCH, &value32)) != VTSS_RC_OK) {
            pr("CSR Access failed: Initial Reading VTSS_PTP_IP_1588_TOP_CFG_STAT_ANALYZER_MODE \n");
            VTSS_PHY_TS_SPI_UNPAUSE_PRIV(port_no);
            return VTSS_RC_ERROR;
        }

        if (value32 == 0x0800) {   /* IPv4 */
            pr("Engine Id:%d, Eng_1 designated for Primary OOS Recovery, ENCAP is ETH-IP-PTP (0x%x)! \n", eng_id, value32);
            VTSS_I("Engine Id:%d, Eng_1 designated for Primary OOS Recovery, ENCAP is ETH-IP-PTP (0x%x)! \n", eng_id, value32);
        } else {
            pr("INVALID CONFIG: Engine Id:%d, Eng_1 designated for Primary OOS Recovery, ENCAP is NOT ETH-IP-PTP ! \n", eng_id);
            VTSS_E("INVALID CONFIG: Engine Id:%d, Eng_1 designated for Primary OOS Recovery, ENCAP is NOT ETH-IP-PTP ! ", eng_id);
            VTSS_PHY_TS_SPI_UNPAUSE_PRIV(port_no);
            return VTSS_RC_ERROR;
        }

        /* Ingress - If UDP Sport_MASK is Non-Zero, We have to check the Value, it should be 319 or 0x013F  */
        if ((rc = VTSS_PHY_TS_READ_CSR(port_no, VTSS_PHY_TS_ANA_BLK_ID_ING_1,
                                       VTSS_ANA_IP1_NXT_PROTOCOL_IP1_PROT_MATCH_2_UPPER,
                                       &value32)) != VTSS_RC_OK) {
            pr("CSR Access failed: Initial Reading VTSS_ANA_IP1_NXT_PROTOCOL_IP1_PROT_MATCH_2_UPPER \n");
            VTSS_PHY_TS_SPI_UNPAUSE_PRIV(port_no);
            return VTSS_RC_ERROR;
        }
        sport_val = (value32 & 0xffff0000) >> 16;
        dport_val = (value32 & 0x0000ffff);

        if ((rc = VTSS_PHY_TS_READ_CSR(port_no, VTSS_PHY_TS_ANA_BLK_ID_ING_1,
                                       VTSS_ANA_IP1_NXT_PROTOCOL_IP1_PROT_MASK_2_UPPER,
                                       &value32)) != VTSS_RC_OK) {
            pr("CSR Access failed: Initial Reading VTSS_ANA_IP1_NXT_PROTOCOL_IP1_PROT_MASK_2_UPPER \n");
            VTSS_PHY_TS_SPI_UNPAUSE_PRIV(port_no);
            return VTSS_RC_ERROR;
        }
        sport_mask = (value32 & 0xffff0000) >> 16;
        dport_mask = (value32 & 0x0000ffff);

        VTSS_I("Engine Id:%d, INGRESS: OOS Recovery, ETH-IP-PTP UDP Sport_M: 0x%04x, Sport_Val:0x%04x, Dport_M:0x%04x, Dport_Val:0x%04x",
               eng_id, sport_mask, sport_val, dport_mask, dport_val);
        pr("Engine Id:%d, INGRESS: OOS Recovery, ETH-IP-PTP UDP Sport_M: 0x%04x, Sport_Val:0x%04x, Dport_M:0x%04x, Dport_Val:0x%04x\n",
           eng_id, sport_mask, sport_val, dport_mask, dport_val);

        /* If the UDP Sport_Mask is set, the only value that can be used for OOS Recovery to work is 319 or 0x013F */
        if ((sport_mask)) {
            /* Must check for UDP Value, but also Apply the Mask to ensure Both are correct */
            if ((sport_val & sport_mask) == 319) {
                VTSS_I("Engine Id:%d, ING: Primary OOS Recovery, ENCAP is ETH-IP-PTP,  UDP Sport_Val: 0x%04x ", eng_id, sport_val);
            } else {
                pr("ERROR in Config: Engine Id:%d, ING: Primary OOS Recovery, ENCAP is ETH-IP-PTP, UDP Sport_Val != 0x013F Sport_Val: 0x%04x \n", eng_id, sport_val);
                VTSS_E("ERROR in Config: Engine Id:%d, ING: Primary OOS Recovery, ENCAP is ETH-IP-PTP, UDP Sport_Val != 0x013F Sport_Val: 0x%04x ", eng_id, sport_val);
                VTSS_PHY_TS_SPI_UNPAUSE_PRIV(port_no);
                return VTSS_RC_ERROR;
            }
        } else {
            pr("Engine Id:%d, ING: Primary OOS Recovery, ENCAP is ETH-IP-PTP,  UDP Sport_MASK: 0x%04x \n", eng_id, sport_mask);
            VTSS_I("Engine Id:%d, ING: Primary OOS Recovery, ENCAP is ETH-IP-PTP,  UDP Sport_MASK: 0x%04x ", eng_id, sport_mask);
        }

        /* Check UDP Dest Port, For PTP Setup, It is required to be 319 or 0x013F */
        if ((dport_val & dport_mask) != 319) {
            pr("ERROR in Config: Engine Id:%d, ING: Primary OOS Recovery, ENCAP is ETH-IP-PTP, UDP Dport_Val != 0x013F Dport_Val: 0x%04x \n", eng_id, dport_val);
            VTSS_E("ERROR in Config: Engine Id:%d, ING: Primary OOS Recovery, ENCAP is ETH-IP-PTP, UDP Dport_Val != 0x013F Dport_Val: 0x%04x ", eng_id, dport_val);
            VTSS_PHY_TS_SPI_UNPAUSE_PRIV(port_no);
            return VTSS_RC_ERROR;
        }

        /* Egress - If UDP Sport_MASK is Non-Zero, We have to check the Value, it should be 319 or 0x013f  */
        if ((rc = VTSS_PHY_TS_READ_CSR(port_no, VTSS_PHY_TS_ANA_BLK_ID_EGR_1,
                                       VTSS_ANA_IP1_NXT_PROTOCOL_IP1_PROT_MATCH_2_UPPER,
                                       &value32)) != VTSS_RC_OK) {
            pr("CSR Access failed: Initial Reading VTSS_ANA_IP1_NXT_PROTOCOL_IP1_PROT_MATCH_2_UPPER \n");
            VTSS_PHY_TS_SPI_UNPAUSE_PRIV(port_no);
            return VTSS_RC_ERROR;
        }
        sport_val = (value32 & 0xffff0000) >> 16;
        dport_val = (value32 & 0x0000ffff);

        if ((rc = VTSS_PHY_TS_READ_CSR(port_no, VTSS_PHY_TS_ANA_BLK_ID_EGR_1,
                                       VTSS_ANA_IP1_NXT_PROTOCOL_IP1_PROT_MASK_2_UPPER,
                                       &value32)) != VTSS_RC_OK) {
            pr("CSR Access failed: Initial Reading VTSS_ANA_IP1_NXT_PROTOCOL_IP1_PROT_MASK_2_UPPER \n");
            VTSS_PHY_TS_SPI_UNPAUSE_PRIV(port_no);
            return VTSS_RC_ERROR;
        }
        sport_mask = (value32 & 0xffff0000) >> 16;
        dport_mask = (value32 & 0x0000ffff);

        VTSS_I("Engine Id:%d, EGRESS: OOS Recovery, ETH-IP-PTP UDP Sport_M: 0x%04x, Sport_Val:0x%04x, Dport_M:0x%04x, Dport_Val:0x%04x",
               eng_id, sport_mask, sport_val, dport_mask, dport_val);
        pr("Engine Id:%d, EGRESS: OOS Recovery, ETH-IP-PTP UDP Sport_M: 0x%04x, Sport_Val:0x%04x, Dport_M:0x%04x, Dport_Val:0x%04x\n",
           eng_id, sport_mask, sport_val, dport_mask, dport_val);

        /* If the UDP Sport_Mask is set, the only value that can be used for OOS Recovery to work is 319 or 0x013F */
        if ((sport_mask)) {
            /* Must check for UDP Value, but also Apply the Mask to ensure Both are correct */
            if ((sport_val & sport_mask) == 319) {
                VTSS_I("Engine Id:%d, EGR: Primary OOS Recovery, ENCAP is ETH-IP-PTP,  UDP Sport_Val: 0x%04x ", eng_id, sport_val);
            } else {
                pr("ERROR in Config: Engine Id:%d, EGR: Primary OOS Recovery, ENCAP is ETH-IP-PTP, UDP Sport_Val != 0x013F Sport_Val: 0x%04x \n", eng_id, sport_val);
                VTSS_E("ERROR in Config: Engine Id:%d, EGR: Primary OOS Recovery, ENCAP is ETH-IP-PTP, UDP Sport_Val != 0x013F Sport_Val: 0x%04x ", eng_id, sport_val);
                VTSS_PHY_TS_SPI_UNPAUSE_PRIV(port_no);
                return VTSS_RC_ERROR;
            }
        } else {
            pr("Engine Id:%d, EGR: Primary OOS Recovery, ENCAP is ETH-IP-PTP,  UDP Sport_MASK: 0x%04x \n", eng_id, sport_mask);
            VTSS_I("Engine Id:%d, EGR: Primary OOS Recovery, ENCAP is ETH-IP-PTP,  UDP Sport_MASK: 0x%04x ", eng_id, sport_mask);
        }

        /* Check UDP Dest Port, For PTP Setup, It is required to be 319 or 0x013F */
        if ((dport_val & dport_mask) != 319) {
            pr("ERROR in Config: Engine Id:%d, EGR: Primary OOS Recovery, ENCAP is ETH-IP-PTP, UDP Dport_Val != 0x013F Dport_Val: 0x%04x \n", eng_id, dport_val);
            VTSS_E("ERROR in Config: Engine Id:%d, EGR: Primary OOS Recovery, ENCAP is ETH-IP-PTP, UDP Dport_Val != 0x013F Dport_Val: 0x%04x ", eng_id, dport_val);
            VTSS_PHY_TS_SPI_UNPAUSE_PRIV(port_no);
            return VTSS_RC_ERROR;
        }
        break;
    default:
        pr("INVALID CONFIG: Engine Id:%d, Engine designated for Primary OOS Recovery (eng_recov) is INVALID! \n", eng_id);
        VTSS_E("INVALID CONFIG: Engine Id:%d, Engine designated for Primary OOS Recovery (eng_recov) is INVALID! ", eng_id);
        VTSS_PHY_TS_SPI_UNPAUSE_PRIV(port_no);
        return VTSS_RC_ERROR;
    }

    value32 = recov->ana_mode;
    /* Check if Engine-2A or Engine-2B are enabled or not - INFO Only */
    switch (eng_minE) {
    case VTSS_PHY_TS_OAM_ENGINE_ID_2A:
    case VTSS_PHY_TS_OAM_ENGINE_ID_2B:
        if ((value32 & VTSS_TS_ENG2_ENABLED) == VTSS_TS_ENG2_ENABLED) {
            pr("Engine Id:%d, Designated for Secondary OOS Recovery (eng_minE) is Enabled! \n", eng_minE);
            VTSS_I("Engine Id:%d, Designated for Secondary OOS Recovery (eng_minE) is Enabled! ", eng_minE);
        } else {
            pr("Engine Id:%d, Designated for Secondary OOS Recovery (eng_minE) is NOT Enabled! \n", eng_minE);
            VTSS_I("Engine Id:%d, Designated for Secondary OOS Recovery (eng_minE) is NOT Enabled! ", eng_minE);
        }
        break;
    default:
        pr("INVALID CONFIG: Engine Id:%d, Engine designated for Secondary OOS Recovery (eng_minE) is INVALID! \n", eng_id);
        VTSS_E("INVALID CONFIG: Engine Id:%d, Engine designated for Secondary OOS Recovery (eng_minE) is INVALID! \n", eng_id);
        VTSS_PHY_TS_SPI_UNPAUSE_PRIV(port_no);
        return VTSS_RC_ERROR;
    }

    VTSS_PHY_TS_SPI_UNPAUSE_PRIV(port_no);

    VTSS_I("Getting port configuration\n");
    /* Get Port Conf */
    recov->eng_id = eng_id;
    recov->eng_id_sav = TRUE;
    recov->port_conf = vtss_state->phy_state[port_no].setup;
    recov->port_conf_sav = TRUE;

    VTSS_I("waiting traffic to stop\n");
#if 1
    // Wait for Traffic to stop
    miim_write_func(vtss_state, port_no, 0x1f, VTSS_PHY_PAGE_EXTENDED_3);
    counter1 = 0;
    counter2 = 0;
    counter3 = 0;
    miim_read_func(vtss_state, port_no, 0x1d, &counter1);
    VTSS_I("Port_No: %d; >>> Entry::Media i/f: 0x%x   Fiber Media CRC Error Reg_29E3: 0x%x", port_no, vtss_state->phy_state[port_no].reset.media_if, counter1);

    do {
        miim_read_func(vtss_state, port_no, 0x1c, &counter1);
        VTSS_MSLEEP(1);
        miim_read_func(vtss_state, port_no, 0x1c, &counter2);
        VTSS_MSLEEP(1);
        counter3++;
        if (counter3 > 10) {  // limits to 10 loops or about 2secs
            break;
        }
    } while (((counter1 & 0x3fff) > 0) || ((counter2 & 0x3fff) > 0));

    VTSS_I("Tx Counters Reg_28E3   1st Read RegVal: 0x%x,  Counter: %d    2nd Read RegVal: 0x%x,   Counter:%d \n",
           counter1, (counter1 & 0x3fff), counter2, (counter2 & 0x3fff));

    miim_write_func(vtss_state, port_no, 0x1f, VTSS_PHY_PAGE_STANDARD);

    /* *************************************************************** */

#endif

    oos_counter[port_no]++;

    /* ********************************************************************************* */
    /* The Following Counters MUST be Read/Cleared before the OOS Algorithm runs         */
    /* ********************************************************************************* */
    counter3 = 0;
    do {
        counter1 = 0;
        counter2 = 0;
        miim_write_func(vtss_state, port_no, 0x1f, VTSS_PHY_PAGE_EXTENDED_3);
        miim_read_func(vtss_state, port_no, 0x15, &counter1);
        miim_read_func(vtss_state, port_no, 0x16, &counter2);
        VTSS_I("Info:Port: %d, >>> Entry: Good_CRC Counters Reg_21E3: 0x%x;  BAD_CRC Counters Reg_22E3: 0x%x ", port_no, counter1, counter2);
        counter1 = 0;
        counter2 = 0;
        miim_read_func(vtss_state, port_no, 0x11, &counter1);
        miim_read_func(vtss_state, port_no, 0x18, &counter2);
        VTSS_I("Info: >>> Entry: Port: %d,   OOS_Cnt: %d;  MAC SerDes PCS Status Reg_17E3: 0x%x;  Media SerDes PCS Status Reg_24E3: 0x%x ", port_no, oos_counter[port_no], counter1, counter2);
        counter1 = 0;
        miim_read_func(vtss_state, port_no, 0x1d, &counter1);
        VTSS_I("OOS Port_No: %d;>>>Entry Media i/f: 0x%x   Fiber Media CRC Error Reg_29E3: 0x%x", port_no, vtss_state->phy_state[port_no].reset.media_if, counter1);
        VTSS_MSLEEP(5);
        counter3++;
    } while (counter3 < 2);

    miim_write_func(vtss_state, port_no, 0x1f, VTSS_PHY_PAGE_STANDARD);

    counter1 = 0;
    counter2 = 0;
    miim_read_func(vtss_state, port_no, 0x01, &counter1);
    miim_read_func(vtss_state, port_no, 0x01, &counter2);
    VTSS_I ("Info: Entry - Just-Before-Enable-Port-BYPASS Mode.   Link Status: 1st Read:Reg01: 0x%x;   2nd Read:Reg01: 0x%x    port_no: %d, OOS_Cnt:%d ",
            counter1, counter2, port_no, oos_counter[port_no]);


    /* ********************************************************************************* */
    /* Failures from this point onward require special recovery                          */
    /* ********************************************************************************* */
    recov->recovery_required = TRUE;
    vtss_state->phy_ts_port_conf[port_no].oos_recovery_active = TRUE;

    /* Set HW Bit to indicate OOS has been enabled - Saved across Warm-Start Cycles  */
    if ((rc = PHY_WR_MASKED_PAGE(vtss_state, port_no, VTSS_PHY_LED_BEHAVIOR, 0x2000, 0x2000)) != VTSS_RC_OK) {
        VTSS_E("TESLA_OOS: Failed to SET VTSS_F_PHY_LED_BEHAVIOR_OOS_RECOVERY_ENABLE, port-no: %d\n", port_no);
    }

    /* *********************************************************************************** */
    /* *********************************************************************************** */

#if 1
    /* Set bypass mode */
    VTSS_PHY_TS_SPI_PAUSE_PRIV(port_no);
    vtss_state->phy_ts_port_conf[port_no].port_ena = FALSE;
    if (VTSS_RC_OK != vtss_phy_ts_csr_set_priv(vtss_state, port_no, VTSS_PHY_TS_PORT_ENA_SET)) {
        /* Should never happen */
        VTSS_E("Disable Port - BYPASS Mode set failed\n");
        VTSS_PHY_TS_SPI_UNPAUSE_PRIV(port_no);
        return VTSS_RC_ERROR;
    }
    VTSS_PHY_TS_SPI_UNPAUSE_PRIV(port_no);
    VTSS_I("1588 BYPASS Enabled");
#endif

    VTSS_MSLEEP(1);
    /* ****************************************************************************************** */
    /* NOTE: The ENABLE is Order Dependant!  The MEDIA i/f test mode must be enabled BEFORE MAC!! */
    /* ****************************************************************************************** */
    VTSS_I("SerDes Test Mode Enabled - Media & MAC");
    // MEDIA i/f
    is_mac_if = FALSE;
    enable_tst_mode = TRUE;
    if ((rc = vtss_phy_ts_tesla_serdes_test_mode_set(vtss_state, port_no, is_mac_if, enable_tst_mode)) != VTSS_RC_OK) {
        VTSS_E("Enable of SerDes Test Mode on MEDIA i/f failed\n");
        return VTSS_RC_ERROR;
    }

    VTSS_MSLEEP(1);
    // MAC i/f
    is_mac_if = TRUE;
    enable_tst_mode = TRUE;
    if ((rc = vtss_phy_ts_tesla_serdes_test_mode_set(vtss_state, port_no, is_mac_if, enable_tst_mode)) != VTSS_RC_OK) {
        VTSS_E("Enable of SerDes Test Mode on MAC i/f failed\n");
    }

// ** Register definitions -  EXT2 page**
//#define VTSS_PHY_EEE_CONTROL VTSS_PHY_PAGE_EXTENDED_2, 17
//VTSS_F_PHY_EEE_CONTROL_ENABLE_1000BASE_T_FORCE_MODE   VTSS_PHY_BIT(5)

    /* Setting 17E2.5 will prevent 1000BaseX Link to NOT come up, Therefore, Must ensure it is clear */
    miim_write_func(vtss_state, port_no, 0x1f, VTSS_PHY_PAGE_EXTENDED_2);
    miim_read_func(vtss_state, port_no, 0x11, &value);      // Extended Page_2 EEE Control, Reg17E2.5
    if (value & VTSS_F_PHY_EEE_CONTROL_ENABLE_1000BASE_T_FORCE_MODE) {
        value &= ~VTSS_F_PHY_EEE_CONTROL_ENABLE_1000BASE_T_FORCE_MODE;  // clear bit 5
        miim_write_func(vtss_state, port_no, 0x11, value);
        recov->forced_1000BaseT = TRUE; /* Save for Recovery in event of ERROR-Exit */
        VTSS_I("Clearing 1000BaseT Forced Mode: 17E2.5\n");
    }
    miim_write_func(vtss_state, port_no, 0x1f, VTSS_PHY_PAGE_STANDARD);

    VTSS_MSLEEP(1);

    /* Start: port reset for 100FX TODO: Need to add any delay after reset or check for reset bit cleared??*/
    recov->port_reset_conf = vtss_state->phy_state[port_no].reset;
    recov->port_reset_conf_sav = TRUE;
    VTSS_I("Entry PHY_Reset_Media_if mode: 0x%x,  Media_Op_Mode: 0x%x, Sav_Media_if: 0x%x, Port_no: %d\n",
           vtss_state->phy_state[port_no].reset.media_if, media_operating_mode, recov->port_reset_conf.media_if, port_no);

    /* Must Check port Media Operating Mode - In the PHY Instance - Must be in 1000Base-X Mode */
    /* Must Check port Media Operating Mode - In the HW also - Must be in 1000Base-X */
    if ((vtss_state->phy_state[port_no].reset.media_if != VTSS_PHY_MEDIA_IF_FI_1000BX) ||
        (media_operating_mode != 0x2)) {   // If current Operating Mode programmed in Chip is Not 1000Base-X Fiber
        u16 reg;
        vtss_mtimer_t         timer;
        u16                   micro_cmd_100fx = 0; // Use to signal to micro program if the fiber is 100FX (Bit 4). Default is 1000BASE-x

        VTSS_I("FIFO-SYNC: Current Media i/f: 0x%x  HW_media_operating_mode:0x%x chg to VTSS_PHY_MEDIA_IF_FI_1000BX(0x%x)\n",
               vtss_state->phy_state[port_no].reset.media_if, media_operating_mode, VTSS_PHY_MEDIA_IF_FI_1000BX);
        // Setup media in micro program. Bit 8-11 is bit for the corresponding port (See TN1080)
        miim_write_func(vtss_state, port_no, 0x1f, VTSS_PHY_PAGE_GPIO);
        miim_write_func(vtss_state, port_no, 0x12, (0x80C1 | ((1 << (channel_id)) << 8) | micro_cmd_100fx));
        if ((rc = vtss_phy_wait_for_micro_complete(vtss_state, port_no)) != VTSS_RC_OK) {
            VTSS_E("Micro-Timeout: FIFO-SYNC Setup of Media i/f failed\n");
            return VTSS_RC_ERROR;
        }
        VTSS_MSLEEP(10);
        miim_write_func(vtss_state, port_no, 0x1f, VTSS_PHY_PAGE_STANDARD);
        miim_read_func(vtss_state, port_no, 0x17, &value);      // Extended PHY Control, Reg23
        value &= ~(VTSS_M_PHY_EXTENDED_PHY_CONTROL_MEDIA_OPERATING_MODE);   // Clear bits 8-10
        value &= ~(VTSS_F_PHY_EXTENDED_PHY_CONTROL_AMS_PREFERENCE);   // Clear bit 11
        value |= VTSS_F_PHY_EXTENDED_PHY_CONTROL_MEDIA_OPERATING_MODE(0x2); // VTSS_PHY_MEDIA_IF_FI_1000BX
        miim_write_func(vtss_state, port_no, 0x17, value);      // Extended PHY Control, Reg23
        /* Update the Media i/f */
//        vtss_state->phy_state[port_no].reset.media_if = VTSS_PHY_MEDIA_IF_FI_1000BX; // Overwrite the reset config to match the OOS Patch for conf_set code

        /*  Reg0: Bits 15,14,6 Set: SW_Reset; NE_Lpbk ENA; Forced_Speed-1000m */
        reg = VTSS_F_PHY_MODE_CONTROL_SW_RESET | VTSS_F_PHY_MODE_CONTROL_LOOP | (1 << 6);
        VTSS_I("Soft resetting Tesla port:%d, Placing Media in Fiber-1000BX; FORCED-1000m mode; Writing Reg23:0x%04x; Reg0:0x%x\n", port_no, value, reg);
        PHY_WR_PAGE(vtss_state, port_no, VTSS_PHY_MODE_CONTROL, reg);  // Tesla PHY Only - Writing 0xc040
        VTSS_MSLEEP(1);/* pause after reset */
        VTSS_MTIMER_START(&timer, 5000); /* Wait up to 5 seconds */
        while (1) {
            if (PHY_RD_PAGE(vtss_state, port_no, VTSS_PHY_MODE_CONTROL, &reg) == VTSS_RC_OK && (reg & VTSS_F_PHY_MODE_CONTROL_SW_RESET) == 0) {
                break;
            }
            VTSS_MSLEEP(1);
            if (VTSS_MTIMER_TIMEOUT(&timer)) {
                VTSS_E("port_no %u, reset timeout, reg = 0x%X", port_no, reg);
            }
        }
        VTSS_MTIMER_CANCEL(&timer);
    }
    /* End port reset for 100FX */

    tmp_port_conf = recov->port_conf;
    tmp_port_conf.mode = VTSS_PHY_MODE_FORCED;
    tmp_port_conf.forced.speed = VTSS_SPEED_1G;
    tmp_port_conf.forced.fdx = 1;

    vtss_state->phy_state[port_no].setup = tmp_port_conf;
    if ((rc = vtss_phy_conf_set_private(vtss_state, port_no)) != VTSS_RC_OK) {
        VTSS_E("ERROR during FIFO-SYNC Config Update!!");
        return VTSS_RC_ERROR;
    }
    VTSS_I("PHY HW Config Updated - Forced");
    vtss_state->phy_state[port_no].setup = recov->port_conf;
    VTSS_I("PHY_INST Config Reverted - Not applying to HW at this time");

    /* MAC ISOLATE */
    /* NOTE: This must occur AFTER the switch from 100FX because the Media chg does a SW Reset, which clears the isolate bit! */
    /* Set the MAC Isolate bit to Disable MAC i/f - This should keep the MAC from possibly seeing strange traffic !! */
    miim_write_func(vtss_state, port_no, 0x1f, VTSS_PHY_PAGE_STANDARD);
    miim_read_func(vtss_state, port_no, 0x0, &value);
    value |= (1 << 10);    // Disable MAC i/f - Setting MAC ISOLATE BIT
    miim_write_func(vtss_state, port_no, 0x0, value);
    miim_read_func(vtss_state, port_no, 0x0, &value);
    VTSS_I("Setting MAC Isolate, Reg 0: 0x%x", value);

    VTSS_MSLEEP(1);

#if 1
    /* Enable EPG, but do not run */
    miim_write_func(vtss_state, port_no, 0x1f, VTSS_PHY_PAGE_EXTENDED);
    value = TESLA_RECOVERY_EPG_FRAME_PATTERN;
    miim_write_func(vtss_state, port_no, 0x1e, value);
    value = 0x8040;  // Configure EPG, but don't Run 125 byte, This ensure the in-line MUX is switched to proper setting
    miim_write_func(vtss_state, port_no, 0x1d, value);
    miim_read_func(vtss_state, port_no, 0x1d, &value);
    VTSS_I("Port_No: %d, EPG Enabled but OFF: Reading Ext:%d Reg:%02d: 0x%04X \n", port_no, VTSS_PHY_PAGE_EXTENDED, 0x1d, value);
    miim_write_func(vtss_state, port_no, 0x1f, VTSS_PHY_PAGE_STANDARD);
#endif

    VTSS_MSLEEP(1);

    /* Set NE loopback */
    vtss_state->phy_state[port_no].loopback.far_end_enable = FALSE;
    vtss_state->phy_state[port_no].loopback.near_end_enable = TRUE;
    vtss_state->phy_state[port_no].loopback.connector_enable = FALSE;
    vtss_state->phy_state[port_no].loopback.mac_serdes_input_enable = FALSE;
    vtss_state->phy_state[port_no].loopback.mac_serdes_facility_enable = FALSE;
    vtss_state->phy_state[port_no].loopback.mac_serdes_equipment_enable = FALSE;
    vtss_state->phy_state[port_no].loopback.media_serdes_input_enable = FALSE;
    vtss_state->phy_state[port_no].loopback.media_serdes_facility_enable = FALSE;
    vtss_state->phy_state[port_no].loopback.media_serdes_equipment_enable = TRUE;

    if ((rc = vtss_phy_loopback_set_private(vtss_state, port_no)) != VTSS_RC_OK) {
        VTSS_E("ERROR during FIFO-SYNC NEAR-END Loopback and Media Equip Loopback set!!");
        return VTSS_RC_ERROR;
    }
    VTSS_I("Enable NE Loopback");
    VTSS_MSLEEP(1);
#if 1
    /* Disable bypass mode */
    VTSS_PHY_TS_SPI_PAUSE_PRIV(port_no);
    vtss_state->phy_ts_port_conf[port_no].port_ena = TRUE;
    if (VTSS_RC_OK != vtss_phy_ts_csr_set_priv(vtss_state, port_no, VTSS_PHY_TS_PORT_ENA_SET)) {
        VTSS_E("Enable Port - BYPASS Mode set failed");
        VTSS_PHY_TS_SPI_UNPAUSE_PRIV(port_no);
        return VTSS_RC_ERROR;
    }
    VTSS_PHY_TS_SPI_UNPAUSE_PRIV(port_no);
    VTSS_I("1588 BYPASS Disabled");
#endif

    VTSS_MSLEEP(1);


    /* Set FE loopback */
    vtss_state->phy_state[port_no].loopback.far_end_enable = TRUE;
    vtss_state->phy_state[port_no].loopback.near_end_enable = TRUE;
    vtss_state->phy_state[port_no].loopback.connector_enable = FALSE;
    vtss_state->phy_state[port_no].loopback.mac_serdes_input_enable = FALSE;
    vtss_state->phy_state[port_no].loopback.mac_serdes_facility_enable = FALSE;
    vtss_state->phy_state[port_no].loopback.mac_serdes_equipment_enable = FALSE;
    vtss_state->phy_state[port_no].loopback.media_serdes_input_enable = FALSE;
    vtss_state->phy_state[port_no].loopback.media_serdes_facility_enable = FALSE;
    vtss_state->phy_state[port_no].loopback.media_serdes_equipment_enable = TRUE;

    if ((rc = vtss_phy_loopback_set_private(vtss_state, port_no)) != VTSS_RC_OK) {
        VTSS_E("ERROR during FIFO-SYNC FAR-END Loopback and Media Equip Loopback set!!");
        return VTSS_RC_ERROR;
    }

    VTSS_I("Enable FE Loopback");

    VTSS_MSLEEP(1);

    /* ********************************************************************************* */
    /* SAVE ANY Application Latency Values and Reset to Default                          */
    /* ********************************************************************************* */

    recov->ingress_latency     = vtss_state->phy_ts_port_conf[port_no].ingress_latency;
    recov->ingress_latency_sav = TRUE;
    recov->egress_latency      = vtss_state->phy_ts_port_conf[port_no].egress_latency;
    recov->egress_latency_sav  = TRUE;

    vtss_state->phy_ts_port_conf[port_no].ingress_latency = 12; // Default Value for 125MHz Clock
    vtss_state->phy_ts_port_conf[port_no].egress_latency = 0;   // Default Value for 125MHz Clock

    VTSS_PHY_TS_SPI_PAUSE_PRIV(port_no);
    if ((rc = vtss_phy_ts_csr_set_priv(vtss_state, port_no, VTSS_PHY_TS_ING_LATENCY_SET)) != VTSS_RC_OK) {
        VTSS_E("VTSS_OOS_RECOVERY: VTSS_PHY_TS_ING_LATENCY_SET failure at OOS Entry, port %u", port_no);
    }
    if ((rc = vtss_phy_ts_csr_set_priv(vtss_state, port_no, VTSS_PHY_TS_EGR_LATENCY_SET)) != VTSS_RC_OK) {
        VTSS_E("VTSS_OOS_RECOVERY: VTSS_PHY_TS_EGR_LATENCY_SET failure at OOS Entry, port %u", port_no);
    }

    value32 = 0;
    VTSS_RC(VTSS_PHY_TS_READ_CSR(port_no, VTSS_PHY_TS_PROC_BLK_ID_0,
                                 VTSS_PTP_INGR_IP_1588_RW_INGR_RW_CTRL, &value32));
    pr("Entry: VTSS_PTP_INGR_IP_1588_RW_INGR_RW_CTRL: 0x%08x \n", value32);
    value32 = 0;
    VTSS_RC(VTSS_PHY_TS_READ_CSR(port_no, VTSS_PHY_TS_PROC_BLK_ID_0,
                                 VTSS_PTP_INGR_IP_1588_RW_INGR_RW_MODFRM_CNT, &value32));
    pr("Entry: VTSS_PTP_INGR_IP_1588_RW_INGR_RW_MODFRM_CNT: 0x%08x \n", value32);

    VTSS_PHY_TS_SPI_UNPAUSE_PRIV(port_no);

    do {

        if (phy_id.part_number == VTSS_PHY_TYPE_8574) {
            switch (channel_id) {
            case 0:
            case 1:
                chmask = VTSS_PHY_TS_ENG_FLOW_VALID_FOR_CH0;
                break;
            case 2:
            case 3:
                chmask = VTSS_PHY_TS_ENG_FLOW_VALID_FOR_CH1;
                break;
            default:
                break;
            }
        } else { /* Two Channel PHY */
            switch (channel_id) {
            case 0:
                chmask = VTSS_PHY_TS_ENG_FLOW_VALID_FOR_CH0;
                break;
            case 1:
                chmask = VTSS_PHY_TS_ENG_FLOW_VALID_FOR_CH0;
                break;
            default:
                break;
            }
        }

        recov->chmask = chmask;
        recov->chmask_sav = TRUE;
        VTSS_PHY_TS_SPI_PAUSE_PRIV(port_no);

        /*  Configure Analyzer */
        VTSS_I("Ingress TS FIFO - Configuring Analyzer port-no: %d", port_no);
        if ((rc = vtss_phy_ts_tesla_ingress_tsp_fifo_sync_conf_analyzer(vtss_state, port_no, eng_id, chmask)) != VTSS_RC_OK) {
            VTSS_E("Ingress TS FIFO Sync Detect : Analyzer Configuration Failed, port-no: %d\n", port_no);
            VTSS_PHY_TS_SPI_UNPAUSE_PRIV(port_no);
            break;
        }
        VTSS_PHY_TS_SPI_UNPAUSE_PRIV(port_no);
        /* Entry Step : Saving to Previous Mode - Whichever was enabled - SPI or MDIO */
        VTSS_I("Entry Step: Saving tx_fifo_mode, Configure for MDIO  ");
        if (vtss_state->phy_ts_port_conf[port_no].tx_fifo_mode == VTSS_PHY_TS_FIFO_MODE_SPI) {
            if ((rc = vtss_phy_ts_tesla_tsp_fifo_toggle_spi(vtss_state, port_no, &si_cfg, FALSE)) != VTSS_RC_OK) {
                VTSS_E("Error in Disabling New SPI Mode  %d \n", port_no);
                break;
            }
            recov->si_cfg = si_cfg;
            recov->si_cfg_sav = TRUE;
#if defined(VTSS_CHIP_CU_PHY) && defined(VTSS_PHY_TS_SPI_CLK_THRU_PPS0)
            VTSS_I("Entry Step: port_no: %d; Saving tx_fifo_mode: 0x%x;  Chg Cfg to MDIO; New_SPI_enable: 0x%x;  SI_CFG:0x%x  \n", port_no, vtss_state->phy_ts_port_conf[port_no].tx_fifo_mode, vtss_state->phy_ts_port_conf[port_no].new_spi_conf.enable, si_cfg);
#else
            VTSS_I("Entry Step: port_no: %d; Saving tx_fifo_mode: 0x%x;  Chg Cfg to MDIO; New_SPI_enable: 0x%x;  SI_CFG:0x%x  \n", port_no, vtss_state->phy_ts_port_conf[port_no].tx_fifo_mode, FALSE, si_cfg);
#endif /* VTSS_CHIP_CU_PHY && VTSS_PHY_TS_SPI_CLK_THRU_PPS0 */

        }
        VTSS_PHY_TS_SPI_PAUSE_PRIV(port_no);

        ing_OOS = FALSE;
        egr_OOS = FALSE;
        *OOS = FALSE;

        if (detect_only) {
            VTSS_I("FIFO_SYNC_RECOVER: Executing detect only\n");
            vtss_phy_ts_tesla_tsp_fifo_sync_detect(vtss_state, port_no, pr, chmask, &ing_OOS, &egr_OOS);
            if (egr_OOS == TRUE || ing_OOS == TRUE) {
                *OOS = TRUE;
                VTSS_I("\nOut of sync detected\n");
            }
        } else {
            VTSS_I("FIFO_SYNC_RECOVER: Executing complete recovery\n");

            // We want to loop at least twice in case there is a PLAN_E case so that we can run recovery afterwards.
            // Plan_E impacts the shared port
            do {
                counter1 = 0;
                do {
                    egr_reset = FALSE;
                    *hw_reset_required = FALSE;
                    /*  Egress FIFO Recovery   */
                    VTSS_I("Engress TS FIFO - Egress TSP FIFO_SYNC Recovery port-no: %d", port_no);

                    if ((rc = vtss_phy_ts_tesla_egress_tsp_fifo_sync_recover(vtss_state, port_no, pr, channel_id, eng_id, &egr_reset)) != VTSS_RC_OK) {
                        VTSS_E("Egress TSP FIFO Recovery Failed port_no %d", port_no);
                        rc = VTSS_RC_ERROR;
                        break;
                    }
                    // VTSS_I("Engress TS FIFO - SKIPPING Egress TSP FIFO_SYNC Recovery port-no: %d", port_no);
                    if (egr_reset) {
                        if (counter1 < 1) {
                            VTSS_E("*****************************************************************************: ");
                            VTSS_E("Egress TS CMD FIFO - !!!!! PLAN_E TSP_CMD_OOS_RECOVERY Completed !!!  port-no: %d", port_no);
                            VTSS_E("*****************************************************************************: ");
                            *hw_reset_required = FALSE;
                        } else {
                            VTSS_E("*****************************************************************************: ");
                            VTSS_E("Egress TS CMD FIFO - !!!!! OUT-OF-SYNC Detected !!!!! HW_RESET_IS_REQUIRED !!!  port-no: %d", port_no);
                            VTSS_E("*****************************************************************************: ");
                            *hw_reset_required = TRUE;
                        }
                    } else {
                        VTSS_I("!!!!! port-no: %d", port_no);
                        VTSS_I("*****************************************************************************: ");
                        VTSS_I("Egress TS CMD FIFO - !!!!! IN-SYNC Detected !!!!! port-no: %d", port_no);
                        VTSS_I("*****************************************************************************: ");
                        VTSS_I("!!!!! port-no: %d", port_no);
                        *hw_reset_required = FALSE;
                        break;
                    }
                    counter1++;
                } while (counter1 < VTSS_TS_FIFO_SYNC_MAX_LOOPS);

                //Start of ingress
                /************************************************************** */
                // Change Ingress to match on 48 bit DA/SA, i.e. not timestamp
                // value = VTSS_F_ANA_OAM_ETH1_FLOW_CFG_ETH1_ADDR_MATCH_2_ETH1_ADDR_MATCH_MODE(VTSS_PHY_TS_ETH_ADDR_MATCH_48BIT);
                /* Match Address, Match any Multicast Address, either at source or dest*/
                /************************************************************** */
                blk_id = (eng_id == VTSS_PHY_TS_PTP_ENGINE_ID_0) ? VTSS_PHY_TS_ANA_BLK_ID_ING_0 : VTSS_PHY_TS_ANA_BLK_ID_ING_1;; //ingress
                value32 = 0xFFFFFFFF;
                if ((rc = VTSS_PHY_TS_WRITE_CSR(cfgport, blk_id, VTSS_ANA_ETH1_FLOW_CFG_ETH1_ADDR_MATCH_1(flow_id), &value32)) != VTSS_RC_OK) {
                    VTSS_E("ERROR::Writing Analyzer Config - VTSS_ANA_ETH1_FLOW_CFG_ETH1_ADDR_MATCH_1 - Failed, port-no: %d\n", port_no);
                    *hw_reset_required = TRUE;
                    break;
                }

                value32 = 0;
                if ((rc = VTSS_PHY_TS_READ_CSR(cfgport, blk_id, VTSS_ANA_ETH1_FLOW_CFG_ETH1_ADDR_MATCH_2(flow_id), &value32)) != VTSS_RC_OK) {
                    VTSS_E("ERROR::Reading Analyzer Config - VTSS_ANA_ETH1_FLOW_CFG_ETH1_ADDR_MATCH_2 - Failed, port-no: %d\n", port_no);
                    *hw_reset_required = TRUE;
                    break;
                }
                temp32 = VTSS_F_ANA_ETH1_FLOW_CFG_ETH1_ADDR_MATCH_2_ETH1_ADDR_MATCH_MODE(VTSS_PHY_TS_ETH_ADDR_MATCH_48BIT);
                value32 =  VTSS_PHY_TS_CLR_BITS(value, VTSS_M_ANA_ETH1_FLOW_CFG_ETH1_ADDR_MATCH_2_ETH1_ADDR_MATCH_MODE) | temp32;

                temp32 = VTSS_F_ANA_ETH1_FLOW_CFG_ETH1_ADDR_MATCH_2_ETH1_ADDR_MATCH_SELECT(VTSS_PHY_TS_ETH_MATCH_DEST_ADDR);
                value = VTSS_PHY_TS_CLR_BITS(value, VTSS_M_ANA_ETH1_FLOW_CFG_ETH1_ADDR_MATCH_2_ETH1_ADDR_MATCH_SELECT) | temp32;
                temp32 = 0xFFF1;
                value32 |= VTSS_F_ANA_ETH1_FLOW_CFG_ETH1_ADDR_MATCH_2_ETH1_ADDR_MATCH_2(temp32);

                if ((rc = VTSS_PHY_TS_WRITE_CSR(cfgport, blk_id, VTSS_ANA_ETH1_FLOW_CFG_ETH1_ADDR_MATCH_2(flow_id), &value32)) != VTSS_RC_OK) {
                    VTSS_E("ERROR::Writing Analyzer Config - VTSS_ANA_ETH1_FLOW_CFG_ETH1_ADDR_MATCH_2 - Failed, port-no: %d\n", port_no);
                    *hw_reset_required = TRUE;
                    break;
                }
                /************************************************************** */

                /************************************************************** */

                if (*hw_reset_required == FALSE) {
                    counter1 = 0;
                    do {
                        ing_reset = FALSE;
                        /*  Ingress FIFO Recovery   */
                        VTSS_I("Ingress TS FIFO - Ingress TSP FIFO_SYNC Recovery port-no: %d\n", port_no);
                        if ((rc = vtss_phy_ts_tesla_ingress_tsp_fifo_sync_recover(vtss_state, port_no, pr, channel_id, eng_id, &ing_reset)) != VTSS_RC_OK) {
                            VTSS_E("Ingress TSP FIFO Recovery Failed port_no %d", port_no);
                            rc = VTSS_RC_ERROR;
                            break;
                        }
#ifdef TESLA_FIFO_SYNC_OOS_EARLY_EXIT_ERROR_TESTING
                        /************************************************************** */
                        /************************************************************** */
                        tesla_fifo_sync_counter++;
                        if ((tesla_fifo_sync_counter > 0) &&
                            (tesla_fifo_sync_counter % TESLA_FIFO_SYNC_OOS_EARLY_EXIT_COUNT) == 0) {
                            VTSS_E("TESLA_FIFO_SYNC_OOS_EARLY_EXIT_ERROR_TESTING:  Executing EARLY_EXIT fail port_no %d,  tesla_fifo_sync_counter: %d",
                                   port_no, tesla_fifo_sync_counter);
                            printf("TESLA_FIFO_SYNC_OOS_EARLY_EXIT_ERROR_TESTING:  Executing EARLY_EXIT fail port_no %d,  tesla_fifo_sync_counter: %d\n",
                                   port_no, tesla_fifo_sync_counter);
                            return (VTSS_RC_ERROR); // Error Testing
                        }
                        /************************************************************** */
                        /************************************************************** */
#endif


                        if (ing_reset) {
                            if (counter1 < 1) {
                                VTSS_E("Ingress TS CMD FIFO - !!!!! PLAN_E TSP_CMD_OOS_RECOVERY Completed !!!  port-no: %d\n", port_no);
                                *hw_reset_required = FALSE;

                                VTSS_I("Ingress Found HW Reset : Try Engress TS FIFO -  port-no: %d", port_no);
                                if ((rc = vtss_phy_ts_tesla_egress_tsp_fifo_sync_recover(vtss_state, port_no, pr, channel_id, eng_id, &egr_reset)) != VTSS_RC_OK) {
                                    VTSS_E("Egress TSP FIFO Recovery Failed port_no %d", port_no);
                                    rc = VTSS_RC_ERROR;
                                    break;
                                }
                                if (egr_reset) {
                                    *hw_reset_required = TRUE;
                                    VTSS_E("Executed two times for Both Ing and Egr : -  !!!  port-no: %d\n", port_no);
                                    break;
                                }

                            } else {
                                VTSS_E("Ingress TS CMD FIFO - !!!!! OUT-OF-SYNC Detected !!!!! HW_RESET_IS_REQUIRED !!!  port-no: %d\n", port_no);
                                *hw_reset_required = TRUE;
                            }
                        } else {
                            VTSS_I("!!!!! port-no: %d", port_no);
                            VTSS_I("Ingress TS CMD FIFO - !!!!! IN-SYNC Detected !!!!! port-no: %d\n", port_no);
                            VTSS_I("!!!!! port-no: %d", port_no);
                            *hw_reset_required = FALSE;
                            break;
                        }
                        counter1++;
                    } while (counter1 < VTSS_TS_FIFO_SYNC_MAX_LOOPS);
                }
            } while (0);

            if (*hw_reset_required) {
                VTSS_E("TS FIFO - OUT-OF-SYNC Detected !!!!! HW_RESET_REQUIRED !!!  port-no: %d\n", port_no);
                *OOS = TRUE;
#ifdef VTSS_TS_FIFO_SYNC_MINIMIZE_OUTPUT
                printf("TS FIFO - OUT-OF-SYNC Detected !!!!! HW_RESET_REQUIRED !!!  port-no: %d\n", port_no);
#endif
            } else {
#ifdef VTSS_TS_FIFO_SYNC_MINIMIZE_OUTPUT
                printf("TS FIFO - IN-SYNC Detected !!!!! INGRESS_AND_EGRESS_RECOVERY_CONFIRMED !!!  port-no: %d\n", port_no);
#else
                pr("TS FIFO - IN-SYNC Detected !!!!! INGRESS_AND_EGRESS_RECOVERY_CONFIRMED !!!  port-no: %d\n", port_no);
#endif
            }
        }

        VTSS_PHY_TS_SPI_UNPAUSE_PRIV(port_no);
        /*Restore FIFO depths while exiting */
        /* Returning to Previous Mode - Whichever was enabled - SPI or MDIO */
        VTSS_I("Exit Step: Restoring tx_fifo_mode, Re-Configure port_no: %d  for MDIO/SPI as previously configured  \n", port_no);
        if (vtss_state->phy_ts_port_conf[port_no].tx_fifo_mode == VTSS_PHY_TS_FIFO_MODE_SPI) {
            if ((rc = vtss_phy_ts_tesla_tsp_fifo_toggle_spi(vtss_state, port_no, &si_cfg, TRUE)) != VTSS_RC_OK) {
                VTSS_E("Error in Enable New SPI Mode  %d \n", port_no);
            }
#if defined(VTSS_CHIP_CU_PHY) && defined(VTSS_PHY_TS_SPI_CLK_THRU_PPS0)
            VTSS_I("Exit Step: port_no: %d; Restoring tx_fifo_mode: 0x%x;  Chg Cfg to SPI; New_SPI_enable: 0x%x;  SI_CFG:0x%x  \n", port_no,
                   vtss_state->phy_ts_port_conf[port_no].tx_fifo_mode, vtss_state->phy_ts_port_conf[port_no].new_spi_conf.enable, si_cfg);
#endif /* VTSS_CHIP_CU_PHY && VTSS_PHY_TS_SPI_CLK_THRU_PPS0 */
        }
        VTSS_PHY_TS_SPI_PAUSE_PRIV(port_no);
        VTSS_I("TS FIFO Sync : Disable flows configured in analyzer, port-no: %d\n", port_no);
        /*  Disable flows configured in analyzer */
        if ((rc = vtss_phy_ts_tesla_ingress_tsp_fifo_sync_restore_analyzer_config(vtss_state, port_no, eng_id, chmask)) != VTSS_RC_OK) {
            VTSS_E("Disabling analyzer configurations Failed, port-no: %d\n", port_no);
        }
        VTSS_PHY_TS_SPI_UNPAUSE_PRIV(port_no);

    } while (0);

    VTSS_PHY_TS_SPI_PAUSE_PRIV(port_no);
    if ((rc = VTSS_PHY_TS_WRITE_CSR(port_no, VTSS_PHY_TS_PROC_BLK_ID_0,
                                    VTSS_PTP_IP_1588_TOP_CFG_STAT_ANALYZER_MODE, &recov->ana_mode)) != VTSS_RC_OK) {
        VTSS_E("ERROR::Unable to Re-Store PTP Engine after recovery..Failure in Restoring eng,  port_no: %d;   rc= 0x%x\n", port_no, rc);
    } else {
        VTSS_I("PTP Engine Re-Stored after recovery.. Restoring: 0x%08x,  port_no: %d;   rc= 0x%x\n", recov->ana_mode, port_no, rc);
    }

    if (fifo_conf) {
        if (fifo_conf->eng_recov == VTSS_PHY_TS_PTP_ENGINE_ID_0) {
            if ((recov->ana_mode & VTSS_TS_ENG0_ENABLED) == VTSS_TS_ENG0_ENABLED) {
                VTSS_I("Restoring PTP Engine_0 to Enabled, port_no: %d;   ana_mode: 0x%x\n", port_no, recov->ana_mode);
            } else {
                VTSS_I("Restoring PTP Engine_0 to Disabled, port_no: %d;   ana_mode: 0x%x\n", port_no, recov->ana_mode);
            }
        } else if (fifo_conf->eng_recov == VTSS_PHY_TS_PTP_ENGINE_ID_1) {
            if ((recov->ana_mode & VTSS_TS_ENG1_ENABLED) == VTSS_TS_ENG1_ENABLED) {
                VTSS_I("Restoring PTP Engine_1 to Enabled, port_no: %d;   ana_mode: 0x%x\n", port_no, recov->ana_mode);
            } else {
                VTSS_I("Restoring PTP Engine_1 to Disabled, port_no: %d;   ana_mode: 0x%x\n", port_no, recov->ana_mode);
            }
        }

        if ((fifo_conf->eng_minE == VTSS_PHY_TS_OAM_ENGINE_ID_2A) ||
            (fifo_conf->eng_minE == VTSS_PHY_TS_OAM_ENGINE_ID_2B)) {
            if ((recov->ana_mode & VTSS_TS_ENG2_ENABLED) == VTSS_TS_ENG2_ENABLED) {
                VTSS_I("Restoring PTP Engine_2 to Enabled, port_no: %d;   ana_mode: 0x%x\n", port_no, recov->ana_mode);
            } else {
                VTSS_I("Restoring PTP Engine_2 to Disabled, port_no: %d;   ana_mode: 0x%x\n", port_no, recov->ana_mode);
            }
        }
    } else {
        VTSS_I("FIFO_CONF is NULL, Unable to Restore PTP Engine port_no: %d;   ana_mode: 0x%x\n", port_no, recov->ana_mode);
    }

    value32 = 0;
    if ((rc = VTSS_PHY_TS_READ_CSR(port_no, VTSS_PHY_TS_PROC_BLK_ID_0,
                                   VTSS_PTP_IP_1588_TOP_CFG_STAT_ANALYZER_MODE, &value32)) != VTSS_RC_OK) {
        VTSS_E("ERROR::After Restoring eng0-VTSS_PTP_IP_1588_TOP_CFG_STAT_ANALYZER_MODE:%x port_no:%d;  rc= 0x%x\n",
               value32, port_no, rc);
    } else {
        VTSS_I("After Restoring eng0-VTSS_PTP_IP_1588_TOP_CFG_STAT_ANALYZER_MODE:%x port_no:%d;  rc= 0x%x\n", value32, port_no, rc);
    }
    VTSS_PHY_TS_SPI_UNPAUSE_PRIV(port_no);

    VTSS_PHY_TS_SPI_PAUSE_PRIV(port_no);
    if ((rc = vtss_phy_ts_csr_event_poll_priv(vtss_state, port_no, &event_status)) != VTSS_RC_OK) {
        VTSS_E("Clearing Interrupts upon Exit of OOS Detect/Recovery Failure, port_no: %d \n", port_no);
    }
    VTSS_PHY_TS_SPI_UNPAUSE_PRIV(port_no);
    VTSS_I("port_no: %d:   ----  1588 Events Cleared: 0x%x ", port_no, event_status);


    /* Enable 1588 BYPASS */
    VTSS_I("1588 BYPASS MODE - Enabled");
    /* Set bypass mode */
    VTSS_PHY_TS_SPI_PAUSE_PRIV(port_no);
    vtss_state->phy_ts_port_conf[port_no].port_ena = FALSE;
    if (VTSS_RC_OK != vtss_phy_ts_csr_set_priv(vtss_state, port_no, VTSS_PHY_TS_PORT_ENA_SET)) {
        VTSS_E("Bypass Mode set failed");
    }

    /* ********************************************************************************* */
    /* Restore ANY Application Latency Values                           */
    /* ********************************************************************************* */

    if (recov->ingress_latency_sav) {
        vtss_state->phy_ts_port_conf[port_no].ingress_latency = recov->ingress_latency;
        if ((rc = vtss_phy_ts_csr_set_priv(vtss_state, port_no, VTSS_PHY_TS_ING_LATENCY_SET)) != VTSS_RC_OK) {
            VTSS_E("VTSS_OOS_RECOVERY: VTSS_PHY_TS_ING_LATENCY_SET failure at OOS Entry, port %u", port_no);
        }
    }

    if (recov->egress_latency_sav) {
        vtss_state->phy_ts_port_conf[port_no].egress_latency = recov->egress_latency;
        if ((rc = vtss_phy_ts_csr_set_priv(vtss_state, port_no, VTSS_PHY_TS_EGR_LATENCY_SET)) != VTSS_RC_OK) {
            VTSS_E("VTSS_OOS_RECOVERY: VTSS_PHY_TS_EGR_LATENCY_SET failure at OOS Entry, port %u", port_no);
        }
    }

    VTSS_PHY_TS_SPI_UNPAUSE_PRIV(port_no);

    /* Disable EPG */
    /* Make Sure that the EPG is Disabled Before Analyzer Reconfig !! */
    miim_write_func(vtss_state, port_no, 0x1f, VTSS_PHY_PAGE_EXTENDED);
    value = 0x0;
    miim_write_func(vtss_state, port_no, 0x1d, value);
    miim_read_func(vtss_state, port_no, 0x1d, &value);
    miim_write_func(vtss_state, port_no, 0x1f, VTSS_PHY_PAGE_STANDARD);
    VTSS_I("Port_No: %d, EPG OFF: Reading Ext:%d Reg:%02d: 0x%04X \n", port_no, VTSS_PHY_PAGE_EXTENDED, 0x1d, value);

    VTSS_MSLEEP(1);  // This delay is to make sure all Ingress/Egress Activity has settled, May be able to delete later

    /* Disable LoopBack configurations - Diable FE first */
    vtss_state->phy_state[port_no].loopback.far_end_enable = FALSE;
    vtss_state->phy_state[port_no].loopback.near_end_enable = TRUE;
    vtss_state->phy_state[port_no].loopback.connector_enable = FALSE;
    vtss_state->phy_state[port_no].loopback.mac_serdes_input_enable = FALSE;
    vtss_state->phy_state[port_no].loopback.mac_serdes_facility_enable = FALSE;
    vtss_state->phy_state[port_no].loopback.mac_serdes_equipment_enable = FALSE;
    vtss_state->phy_state[port_no].loopback.media_serdes_input_enable = FALSE;
    vtss_state->phy_state[port_no].loopback.media_serdes_facility_enable = FALSE;
    vtss_state->phy_state[port_no].loopback.media_serdes_equipment_enable = FALSE;
    if ((rc = vtss_phy_loopback_set_private(vtss_state, port_no)) != VTSS_RC_OK) {
        VTSS_E("ERROR during Disable FE Loopback");
    }
    VTSS_I("Disable FE Loopback");

    VTSS_MSLEEP(1);

    /* Disable LoopBack configurations - Diable NE       */
    vtss_state->phy_state[port_no].loopback.far_end_enable = FALSE;
    vtss_state->phy_state[port_no].loopback.near_end_enable = FALSE;
    vtss_state->phy_state[port_no].loopback.connector_enable = FALSE;
    vtss_state->phy_state[port_no].loopback.mac_serdes_input_enable = FALSE;
    vtss_state->phy_state[port_no].loopback.mac_serdes_facility_enable = FALSE;
    vtss_state->phy_state[port_no].loopback.mac_serdes_equipment_enable = FALSE;
    vtss_state->phy_state[port_no].loopback.media_serdes_input_enable = FALSE;
    vtss_state->phy_state[port_no].loopback.media_serdes_facility_enable = FALSE;
    vtss_state->phy_state[port_no].loopback.media_serdes_equipment_enable = FALSE;
    if ((rc = vtss_phy_loopback_set_private( vtss_state, port_no)) != VTSS_RC_OK) {
        VTSS_E("ERROR during Disable NE Loopback");
    }
    VTSS_I("Disable NE Loopback");

    VTSS_MSLEEP(1);

    VTSS_MSLEEP(1);

    if (recov->port_reset_conf_sav) {
        vtss_state->phy_state[port_no].reset = recov->port_reset_conf;  // Restore the port_reset config to what it was prior to entry.
        VTSS_I("FIFO_SYNC: Exit PHY_Reset_Media_if mode: 0x%x,  Port_no: %d\n", vtss_state->phy_state[port_no].reset.media_if, port_no);
    }

    /* Start of Restoring Media i/f to port */
    if (vtss_state->phy_state[port_no].reset.media_if != VTSS_PHY_MEDIA_IF_FI_1000BX) {
        if ((rc = vtss_phy_ts_tesla_fifo_sync_port_reset_priv(vtss_state, port_no, pr, recov)) != VTSS_RC_OK) {
            VTSS_E("TS FIFO Sync: Failure to Revert PHY Port MEDIA_IF configuration, port-no: %d\n", port_no);
        }
    }
    /* End of Restoring Media i/f to port, after reset */

    if (recov->forced_1000BaseT) {
        miim_write_func(vtss_state, port_no, 0x1f, VTSS_PHY_PAGE_EXTENDED_2);
        miim_read_func(vtss_state, port_no, 0x11, &value);      // Extended Page_2 EEE Control, Reg17E2.5
        value |= VTSS_F_PHY_EEE_CONTROL_ENABLE_1000BASE_T_FORCE_MODE;
        miim_write_func(vtss_state, port_no, 0x11, value);
        miim_write_func(vtss_state, port_no, 0x1f, VTSS_PHY_PAGE_STANDARD);
        VTSS_I("Setting 1000BaseT Forced Mode: 17E2.5\n");
    }

    /* Reapply Port Conf */
    VTSS_I("TS FIFO Sync: Revert PHY Port configuration, port-no: %d\n", port_no);
    vtss_state->phy_state[port_no].setup = recov->port_conf;
    /* The config should be applied before the link comes up, therefore, there should not be a need to re-aneg. */
    /* After reset of a port, we need to re-configure it */
    VTSS_I("TS FIFO Sync: Reverting PHY Port configuration, Includes Master/Slave Configuration port-no: %d\n", port_no);
    if ((rc = vtss_phy_conf_1g_set_private(vtss_state, port_no)) != VTSS_RC_OK) {
        VTSS_E("ERROR during FIFO-SYNC Reverting PHY Config_1g Back to Original Config!!");
    }
    if ((rc = vtss_phy_conf_set_private(vtss_state, port_no)) != VTSS_RC_OK) {
        VTSS_E("ERROR during FIFO-SYNC Reverting PHY Config Back to Original Config!!");
    }

    VTSS_MSLEEP(2);

    /* Clear the MAC Isolate bit to Enable MAC i/f  */
    miim_write_func(vtss_state, port_no, 0x1f, VTSS_PHY_PAGE_STANDARD);
    miim_read_func(vtss_state, port_no, 0x0, &value);
    value &= ~(1 << 10);    // Enable MAC i/f - Clearing MAC ISOLATE BIT
    miim_write_func(vtss_state, port_no, 0x0, value);
    miim_read_func(vtss_state, port_no, 0x0, &value);
    VTSS_I("Clearing MAC Isolate, Reg 0: 0x%x", value);

    VTSS_I("1588 BYPASS MODE - Disabled");
    /* Disable bypass mode */
    VTSS_PHY_TS_SPI_PAUSE_PRIV(port_no);
    vtss_state->phy_ts_port_conf[port_no].port_ena = TRUE;
    if (VTSS_RC_OK != vtss_phy_ts_csr_set_priv(vtss_state, port_no, VTSS_PHY_TS_PORT_ENA_SET)) {
        VTSS_E("Bypass Mode Disable failed");
    }
    VTSS_PHY_TS_SPI_UNPAUSE_PRIV(port_no);

    // MEDIA i/f
    VTSS_I("Disable of SerDes Test Mode MEDIA i/f and MAC i/f\n");
    is_mac_if = FALSE;
    enable_tst_mode = FALSE;
    if ((rc = vtss_phy_ts_tesla_serdes_test_mode_set(vtss_state, port_no, is_mac_if, enable_tst_mode)) != VTSS_RC_OK) {
        VTSS_E("Disable of SerDes Test Mode on MEDIA i/f failed");
    }

    VTSS_MSLEEP(1);
    // MAC i/f
    is_mac_if = TRUE;
    enable_tst_mode = FALSE;
    if ((rc = vtss_phy_ts_tesla_serdes_test_mode_set(vtss_state, port_no, is_mac_if, enable_tst_mode)) != VTSS_RC_OK) {
        VTSS_E("Disable of SerDes Test Mode on MAC i/f failed\n");
    }

    VTSS_MSLEEP(1);

    /* Clear HW Bit to indicate OOS has been disabled - Saved across Warm-Start Cycles  */
    VTSS_I("TESLA_OOS: CLEARING - VTSS_F_PHY_LED_BEHAVIOR_OOS_RECOVERY_ENABLE, port-no: %d\n", port_no);
    if ((rc = PHY_WR_MASKED_PAGE(vtss_state, port_no, VTSS_PHY_LED_BEHAVIOR, 0x0000, 0x2000)) != VTSS_RC_OK) {
        VTSS_E("TESLA_OOS: Failed to CLEAR - VTSS_F_PHY_LED_BEHAVIOR_OOS_RECOVERY_ENABLE, port-no: %d\n", port_no);
    }
    vtss_state->phy_ts_port_conf[port_no].oos_recovery_active = FALSE;

    value = 0;
    VTSS_RC(PHY_RD_PAGE(vtss_state, port_no, VTSS_PHY_EXTENDED_PHY_CONTROL, &value));
    media_operating_mode = (value >> 8) & 0x7;
    pr("OOS Exit: Media i/f Config: PHYInst:0x%x, Reg23:0x%04x OOS Patch port-no: %d \n", vtss_state->phy_state[port_no].reset.media_if, value, port_no);
    VTSS_I("Loopback and Bypass mode disabled\n");
    pr("FIFO_SYNC: Exiting ALL Loopbacks Disabled, EPG Disabled, Port_no: %d\n", port_no);
    /* Switch over back from 8051 middle man approach */

    return rc;
}

static vtss_rc vtss_phy_ts_tesla_error_recover_priv(vtss_state_t                          *vtss_state,
                                                    const vtss_port_no_t                   port_no,
                                                    const vtss_debug_printf_t              pr,
                                                    const vtss_phy_ts_fifo_err_recovery_t *recov)
{
    vtss_rc                rc = VTSS_RC_OK;
    vtss_phy_ts_event_t    event_status = 0;
    u32                    value32 = 0;
    u32                    si_cfg = 0;
    u16                    value = 0;
    u16                    channel_id = 0;
    BOOL                   is_mac_if = 0;
    BOOL                   enable_tst_mode = 0;

    /* Define the MIIM Read/Write functions to use from PHY Instance */
    vtss_miim_read_t       miim_read_func = vtss_state->init_conf.miim_read;
    vtss_miim_write_t      miim_write_func = vtss_state->init_conf.miim_write;

    if (vtss_state->phy_ts_port_conf[port_no].port_ts_init_done == FALSE) {
        VTSS_I("Port ts not initialized! port: %d", port_no);
        return VTSS_RC_ERROR;
    }

    if (!recov->recovery_required) {
        VTSS_I("Port Does not require Special OOS Recovery!  port: %d", port_no);
        return VTSS_RC_ERROR;
    }

    pr("ERROR has occurred in FIFO_SYNC: vtss_phy_ts_tesla_error_recover_priv entry, Port_no: %d\n", port_no);
    if (recov->phy_id_sav) {
        channel_id = recov->phy_id.channel_id;
    }

    if (recov->si_cfg_sav) {
        si_cfg = recov->si_cfg;
    }

    /* Returning to Previous Mode - Whichever was enabled - SPI or MDIO */
    if (recov->si_cfg_sav) {
        VTSS_I("Exit Step: Restoring tx_fifo_mode, Re-Configure port_no: %d  for MDIO/SPI as previously configured  \n", port_no);
        if (vtss_state->phy_ts_port_conf[port_no].tx_fifo_mode == VTSS_PHY_TS_FIFO_MODE_SPI) {
            if ((rc = vtss_phy_ts_tesla_tsp_fifo_toggle_spi(vtss_state, port_no, &si_cfg, TRUE)) != VTSS_RC_OK) {
                VTSS_E("Error in Enable New SPI Mode  %d \n", port_no);
            }
#if defined(VTSS_CHIP_CU_PHY) && defined(VTSS_PHY_TS_SPI_CLK_THRU_PPS0)
            VTSS_I("Exit Step: port_no: %d; Restoring tx_fifo_mode: 0x%x;  Chg Cfg to SPI; New_SPI_enable: 0x%x;  SI_CFG:0x%x  \n", port_no,
                   vtss_state->phy_ts_port_conf[port_no].tx_fifo_mode, vtss_state->phy_ts_port_conf[port_no].new_spi_conf.enable, recov->si_cfg);
#endif
        }
    }

    if (recov->chmask_sav) {
        VTSS_PHY_TS_SPI_PAUSE_PRIV(port_no);
        VTSS_I("TS FIFO Sync : Disable flows configured in analyzer, port-no: %d\n", port_no);
        /*  Disable flows configured in analyzer */
        if ((rc = vtss_phy_ts_tesla_ingress_tsp_fifo_sync_restore_analyzer_config(vtss_state, port_no, recov->eng_id, recov->chmask)) != VTSS_RC_OK) {
            VTSS_E("Disabling analyzer configurations Failed, port-no: %d\n", port_no);
        }
        VTSS_PHY_TS_SPI_UNPAUSE_PRIV(port_no);
    }

    if (recov->ana_mode_sav) {
        VTSS_I("PTP Engine-0 was enabled for recovery..Restoring eng-0\n");
        VTSS_PHY_TS_SPI_PAUSE_PRIV(port_no);
        if ((rc = VTSS_PHY_TS_WRITE_CSR(port_no, VTSS_PHY_TS_PROC_BLK_ID_0,
                                        VTSS_PTP_IP_1588_TOP_CFG_STAT_ANALYZER_MODE, &recov->ana_mode)) != VTSS_RC_OK) {
            VTSS_E("ERROR::PTP Engine-0 was enabled for recovery..Failure in Restoring eng-0,  port_no: %d;   rc= 0x%x\n", port_no, rc);
        } else {
            VTSS_I("PTP Engine-0 was enabled for recovery..Restoring eng-0,  port_no: %d;   rc= 0x%x\n", port_no, rc);
        }

        value32 = 0;
        if ((rc = VTSS_PHY_TS_READ_CSR(port_no, VTSS_PHY_TS_PROC_BLK_ID_0,
                                       VTSS_PTP_IP_1588_TOP_CFG_STAT_ANALYZER_MODE, &value32)) != VTSS_RC_OK) {
            VTSS_E("ERROR::After Restoring eng0-VTSS_PTP_IP_1588_TOP_CFG_STAT_ANALYZER_MODE:%x port_no:%d;  rc= 0x%x\n",
                   value32, port_no, rc);
        } else {
            VTSS_I("After Restoring eng0-VTSS_PTP_IP_1588_TOP_CFG_STAT_ANALYZER_MODE:%x port_no:%d;  rc= 0x%x\n", value32, port_no, rc);
        }
        VTSS_PHY_TS_SPI_UNPAUSE_PRIV(port_no);
    }

    VTSS_PHY_TS_SPI_PAUSE_PRIV(port_no);
    if ((rc = vtss_phy_ts_csr_event_poll_priv(vtss_state, port_no, &event_status)) != VTSS_RC_OK) {
        VTSS_E("Clearing Interrupts upon Exit of OOS Detect/Recovery Failure, port_no: %d \n", port_no);
    }
    VTSS_PHY_TS_SPI_UNPAUSE_PRIV(port_no);
    VTSS_I("port_no: %d:   ----  1588 Events Cleared: 0x%x ", port_no, event_status);

    /* Enable 1588 BYPASS */
    VTSS_I("1588 BYPASS MODE - Enabled");
    /* Set bypass mode */
    VTSS_PHY_TS_SPI_PAUSE_PRIV(port_no);
    vtss_state->phy_ts_port_conf[port_no].port_ena = FALSE;
    if (VTSS_RC_OK != vtss_phy_ts_csr_set_priv(vtss_state, port_no, VTSS_PHY_TS_PORT_ENA_SET)) {
        VTSS_E("Bypass Mode set failed");
    }

    if (recov->ingress_latency_sav) {
        vtss_state->phy_ts_port_conf[port_no].ingress_latency = recov->ingress_latency;
        if ((rc = vtss_phy_ts_csr_set_priv(vtss_state, port_no, VTSS_PHY_TS_ING_LATENCY_SET)) != VTSS_RC_OK) {
            VTSS_E("VTSS_OOS_RECOVERY: VTSS_PHY_TS_ING_LATENCY_SET failure at OOS Entry, port %u", port_no);
        }
    }

    if (recov->egress_latency_sav) {
        vtss_state->phy_ts_port_conf[port_no].egress_latency = recov->egress_latency;
        if ((rc = vtss_phy_ts_csr_set_priv(vtss_state, port_no, VTSS_PHY_TS_EGR_LATENCY_SET)) != VTSS_RC_OK) {
            VTSS_E("VTSS_OOS_RECOVERY: VTSS_PHY_TS_EGR_LATENCY_SET failure at OOS Entry, port %u", port_no);
        }
    }
    VTSS_PHY_TS_SPI_UNPAUSE_PRIV(port_no);

    /* In the event there was a failure, Ensure EPG and Loopbacks are Disabled */
    /* Disable EPG */
    /* Make Sure that the EPG is Disabled Before Analyzer Reconfig !! */
    miim_write_func(vtss_state, port_no, 0x1f, VTSS_PHY_PAGE_EXTENDED);
    value = 0x0;
    miim_write_func(vtss_state, port_no, 0x1d, value);
    miim_read_func(vtss_state, port_no, 0x1d, &value);
    miim_write_func(vtss_state, port_no, 0x1f, VTSS_PHY_PAGE_STANDARD);
    VTSS_I("Port_No: %d, EPG OFF: Reading Ext:%d Reg:%02d: 0x%04X \n", port_no, VTSS_PHY_PAGE_EXTENDED, 0x1d, value);

    /* Disable ALL LoopBack configurations       */
    vtss_state->phy_state[port_no].loopback.far_end_enable = FALSE;
    vtss_state->phy_state[port_no].loopback.near_end_enable = FALSE;
    vtss_state->phy_state[port_no].loopback.connector_enable = FALSE;
    vtss_state->phy_state[port_no].loopback.mac_serdes_input_enable = FALSE;
    vtss_state->phy_state[port_no].loopback.mac_serdes_facility_enable = FALSE;
    vtss_state->phy_state[port_no].loopback.mac_serdes_equipment_enable = FALSE;
    vtss_state->phy_state[port_no].loopback.media_serdes_input_enable = FALSE;
    vtss_state->phy_state[port_no].loopback.media_serdes_facility_enable = FALSE;
    vtss_state->phy_state[port_no].loopback.media_serdes_equipment_enable = FALSE;
    if ((rc = vtss_phy_loopback_set_private( vtss_state, port_no)) != VTSS_RC_OK) {
        VTSS_E("ERROR during Disable ALL Loopbacks");
    }
    VTSS_I("Disable ALL Loopbacks");

    pr("FIFO_SYNC: vtss_phy_ts_tesla_error_recover_priv - ALL Loopbacks Disabled, EPG Disabled, Port_no: %d\n", port_no);

    if (recov->port_reset_conf_sav) {
        vtss_state->phy_state[port_no].reset = recov->port_reset_conf;  // Restore the port_reset config to what it was prior to entry.
        VTSS_I("FIFO_SYNC: Error_Recover_Exit PHY_Reset_Media_if mode: 0x%x,  Port_no: %d\n", vtss_state->phy_state[port_no].reset.media_if, port_no);
    }

    /* Start of Restoring Media i/f to port */
    if (vtss_state->phy_state[port_no].reset.media_if != VTSS_PHY_MEDIA_IF_FI_1000BX) {
        if ((rc = vtss_phy_ts_tesla_fifo_sync_port_reset_priv(vtss_state, port_no, pr, recov)) != VTSS_RC_OK) {
            VTSS_E("TS FIFO Sync: Failure to Revert PHY Port MEDIA_IF configuration, port-no: %d\n", port_no);
        }
    }
    /* End port reset for 100FX */

    if (recov->forced_1000BaseT) {
        miim_write_func(vtss_state, port_no, 0x1f, VTSS_PHY_PAGE_EXTENDED_2);
        miim_read_func(vtss_state, port_no, 0x11, &value);      // Extended Page_2 EEE Control, Reg17E2.5
        value |= VTSS_F_PHY_EEE_CONTROL_ENABLE_1000BASE_T_FORCE_MODE;
        miim_write_func(vtss_state, port_no, 0x11, value);
        miim_write_func(vtss_state, port_no, 0x1f, VTSS_PHY_PAGE_STANDARD);
        VTSS_I("Setting 1000BaseT Forced Mode: 17E2.5\n");
    }

    /* Only Re-apply Port Conf if it was saved previously */
    if (recov->port_conf_sav) {
        VTSS_I("TS FIFO Sync: Revert PHY Port configuration, port-no: %d\n", port_no);
        vtss_state->phy_state[port_no].setup = recov->port_conf;
    }
    /* The config should be applied before the link comes up, therefore, there should not be a need to re-aneg. */
    /* After reset of a port, we need to re-configure it */
    VTSS_I("TS FIFO Sync: Reverting PHY Port configuration, Includes Master/Slave Configuration port-no: %d\n", port_no);
    if ((rc = vtss_phy_conf_1g_set_private(vtss_state, port_no)) != VTSS_RC_OK) {
        VTSS_E("ERROR during FIFO-SYNC Reverting PHY Config_1g Back to Original Config!!");
    }
    if ((rc = vtss_phy_conf_set_private(vtss_state, port_no)) != VTSS_RC_OK) {
        VTSS_E("ERROR during FIFO-SYNC Reverting PHY Config Back to Original Config!!");
    }

    VTSS_MSLEEP(2);

    /* Clear the MAC Isolate bit to Enable MAC i/f  */
    miim_write_func(vtss_state, port_no, 0x1f, VTSS_PHY_PAGE_STANDARD);
    miim_read_func(vtss_state, port_no, 0x0, &value);
    value &= ~(1 << 10);    // Enable MAC i/f - Clearing MAC ISOLATE BIT
    miim_write_func(vtss_state, port_no, 0x0, value);
    miim_read_func(vtss_state, port_no, 0x0, &value);
    VTSS_I("Cleared MAC Isolate, Reg 0: 0x%x", value);

    VTSS_I("1588 BYPASS MODE - Disabled");
    /* Disable bypass mode */
    VTSS_PHY_TS_SPI_PAUSE_PRIV(port_no);
    vtss_state->phy_ts_port_conf[port_no].port_ena = TRUE;
    if (VTSS_RC_OK != vtss_phy_ts_csr_set_priv(vtss_state, port_no, VTSS_PHY_TS_PORT_ENA_SET)) {
        VTSS_E("Bypass Mode Disable failed");
    }
    VTSS_PHY_TS_SPI_UNPAUSE_PRIV(port_no);

    // MEDIA i/f
    VTSS_I("Disable of SerDes Test Mode MEDIA i/f and MAC i/f\n");
    is_mac_if = FALSE;
    enable_tst_mode = FALSE;
    if ((rc = vtss_phy_ts_tesla_serdes_test_mode_set(vtss_state, port_no, is_mac_if, enable_tst_mode)) != VTSS_RC_OK) {
        VTSS_E("Disable of SerDes Test Mode on MEDIA i/f failed");
    }

    VTSS_MSLEEP(1);
    // MAC i/f
    is_mac_if = TRUE;
    enable_tst_mode = FALSE;
    if ((rc = vtss_phy_ts_tesla_serdes_test_mode_set(vtss_state, port_no, is_mac_if, enable_tst_mode)) != VTSS_RC_OK) {
        VTSS_E("Disable of SerDes Test Mode on MAC i/f failed\n");
    }

    VTSS_MSLEEP(1);


    value = 0;
    VTSS_RC(PHY_RD_PAGE(vtss_state, port_no, VTSS_PHY_EXTENDED_PHY_CONTROL, &value));
    pr("FIFO_SYNC_ABNORMAL_Exit: Media i/f Config: PHYInst:0x%x, Reg23:0x%04x OOS Patch port-no: %d \n", vtss_state->phy_state[port_no].reset.media_if, ((value >> 8) & 0x7), port_no);

    vtss_state->phy_ts_port_conf[port_no].oos_recovery_active = FALSE;

    pr("FIFO_SYNC: vtss_phy_ts_tesla_error_recover_priv Exit, Port_no: %d\n", port_no);
    /* Switch over back from 8051 middle man approach */

    return rc;
}

#ifdef TESLA_FIFO_SYNC_OOS_RE_CHECK_FIFO

#define VTSS_OOS_PATCH_MAX_RETRY       (2)
static vtss_rc vtss_phy_ts_tesla_tsp_recheck_fifo(vtss_state_t                 *vtss_state,
                                                  const vtss_port_no_t             port_no,
                                                  const vtss_debug_printf_t        pr,
                                                  const vtss_phy_ts_fifo_conf_t   *fifo_conf,
                                                  BOOL                            *OOS,
                                                  vtss_phy_ts_fifo_err_recovery_t *recov)
{
    vtss_rc                 rc = VTSS_RC_OK;
    u16                     counter1 = 0;
    u16                     counter2 = 0;
    u16                     counter3 = 0;
    vtss_phy_type_t         phy_id;

    if ((rc = vtss_phy_id_get_priv(vtss_state, port_no, &phy_id)) != VTSS_RC_OK) {
        VTSS_E("Ingress TS Error Detect : Failed to get channel ID, port-no: %d\n", port_no);
        return VTSS_RC_ERROR;
    }

    VTSS_I("PHY Identified:PortID:%d PHY: %d Rev:%d Channel:%d  Channel count:%d Baseport:%d\n",
           port_no, phy_id.part_number, phy_id.revision, phy_id.channel_id, phy_id.port_cnt, phy_id.base_port_no);

    if (!((phy_id.part_number == VTSS_PHY_TYPE_8574) || (phy_id.part_number == VTSS_PHY_TYPE_8572))) {
        //Return silently as we don't want to throw the error for other PHYs
        VTSS_I("Not a Tesla PHY! - Tesla OOS Recovery not applicable - Returning... ");
        return VTSS_RC_OK;
    }

    if (vtss_state->phy_ts_port_conf[port_no].port_ts_init_done == FALSE) {
        VTSS_I("Port ts not initialized! port: %d", port_no);
        return VTSS_RC_ERROR;
    }
    if (fifo_conf->detect_only) {
        VTSS_I("TS FIFO Re-Check Not Performed, Detection Only. port_no: %d", port_no);
    } else {

        do {
            counter1 = 0;
            counter2 = 0;
            vtss_phy_page_ext3(vtss_state, port_no);
            VTSS_RC(PHY_RD_PAGE(vtss_state, port_no, VTSS_PHY_MEDIA_SERDES_TX_GOOD_PACKET_COUNTER, &counter1));
            VTSS_RC(PHY_RD_PAGE(vtss_state, port_no, VTSS_PHY_MEDIA_SERDES_TX_CRC_ERROR_COUNTER, &counter2));
            VTSS_I("TSP_FIFO_SYNC_RE_CHECK: Checking:Port: %d,  Good_CRC Counters Reg_21E3: 0x%x;  BAD_CRC Counters Reg_22E3: 0x%x ", port_no, counter1, counter2);
            vtss_phy_page_std(vtss_state, port_no);

            if (counter2 > 0) {
                VTSS_I("<<< TSP_FIFO_SYNC_RE_CHECK: >>> Port: %d;   OOS_Cnt: %d;  Counter in Reg_22E3 is Non-Zero, Fragments possible!! >>_RUNNING_OOS_AGAIN_<< ", port_no, oos_counter[port_no]);
                VTSS_I("Info: >>> TSP_FIO_SYNC_RE_CHECK: Re-Entry <<< - TOP_LEVEL_ENTRY: TESLA TSP_FIFO SYNC ");

                if ((rc = vtss_phy_ts_tesla_tsp_fifo_sync_priv(vtss_state, port_no, pr, fifo_conf, OOS, recov)) != VTSS_RC_OK) {
                    vtss_rc   rc1 = 0;
                    VTSS_E("TESLA TSP_FIFO SYNC Failed. port_no: %d\n", port_no);
                    /* In the event there was a failure, Ensure EPG and Loopbacks are Disabled */
                    /* Disable EPG */
                    /* Make Sure that the EPG is Disabled Before Analyzer Reconfig !! */
                    rc1 = vtss_phy_ts_tesla_error_recover_priv(vtss_state, port_no, pr, recov);
                    if (rc1 != VTSS_RC_OK) {
                        VTSS_E("TESLA TSP_FIFO SYNC Failed AND FAILED RECONFIG of PHY. port_no: %d\n", port_no);
                        VTSS_E(" TESLA TSP_FIFO SYNC Failed AND FAILED RECONFIG of PHY. port_no: %d\n", port_no);
                    } else {
                        VTSS_I("TESLA TSP_FIFO SYNC Failed AND RECONFIG of PHY Successful. port_no: %d\n", port_no);
                    }
                }
            } else {
                VTSS_I("Info: >>> TSP_FIO_SYNC_RE_CHECK:  BAD_CRC Counter Reg_22E3 = 0; ");
                break;
            }
            counter3++;
        } while (counter3 < VTSS_OOS_PATCH_MAX_RETRY);

        counter1 = 0;
        counter2 = 0;
        vtss_phy_page_ext3(vtss_state, port_no);
        VTSS_RC(PHY_RD_PAGE(vtss_state, port_no, VTSS_PHY_MEDIA_SERDES_TX_GOOD_PACKET_COUNTER, &counter1));
        VTSS_RC(PHY_RD_PAGE(vtss_state, port_no, VTSS_PHY_MEDIA_SERDES_TX_CRC_ERROR_COUNTER, &counter2));
        VTSS_I("Exit TSP_FIFO_SYNC_RE_CHECK: Info:Port: %d,  --DONE--: Media i/f: Good_CRC Counters Reg_21E3: 0x%x;  BAD_CRC Counters Reg_22E3: 0x%x ", port_no, counter1, counter2);
        counter1 = 0;
        VTSS_RC(PHY_RD_PAGE(vtss_state, port_no, VTSS_PHY_MEDIA_MAC_SERDES_RX_CRC_CRC_ERR_COUNTER, &counter1));
        VTSS_I("<<< Exit TSP_FIFO_SYNC_RE_CHECK: >>> - TESLA: Port_No: %d;  OOS_Cnt: %d;  --DONE-- Fiber Media CRC Error Reg_29E3: 0x%x", port_no, oos_counter[port_no], counter1);

        vtss_phy_page_std(vtss_state, port_no);
    }

    return rc;

}

#endif /* TESLA_FIFO_SYNC_OOS_RE_CHECK_FIFO */

vtss_rc vtss_phy_ts_tesla_tsp_fifo_sync_private(vtss_state_t                    *vtss_state,
                                                const vtss_port_no_t            port_no,
                                                const vtss_debug_printf_t       pr,
                                                const vtss_phy_ts_fifo_conf_t   *fifo_conf,
                                                BOOL                            *OOS)
{
    vtss_rc                 rc = VTSS_RC_OK;
    u16                    value = 0;
    vtss_miim_read_t        miim_read_func;
    vtss_miim_write_t       miim_write_func;
    vtss_phy_ts_fifo_err_recovery_t    recov;

#ifdef VTSS_FIFO_SYNC_DEBUG
    struct timeval   tv;
#endif

    memset(&recov, 0, sizeof(vtss_phy_ts_fifo_err_recovery_t));

#ifdef VTSS_FIFO_SYNC_DEBUG
    gettimeofday(&tv, NULL);
    pr("Entry:Time Sec %ld, Usec  %ld\n", tv.tv_sec, tv.tv_usec);
#endif
    /* turn on enable flag for "middleman" accesses throughout fifo_sync API */
    vtss_state->rd_ts_fifo = TRUE;
    miim_read_func = vtss_state->init_conf.miim_read;
    miim_write_func = vtss_state->init_conf.miim_write;

    //Save EPG Reg 29E1
    miim_write_func(vtss_state, port_no, 0x1f, VTSS_PHY_PAGE_EXTENDED);
    miim_read_func(vtss_state, port_no, 0x1d, &value);
    miim_write_func(vtss_state, port_no, 0x1f, VTSS_PHY_PAGE_STANDARD);

    if ((vtss_phy_ts_algo_execute_check(vtss_state, port_no)) == FALSE) {
        VTSS_I("PTP is not configured on the port exiting quietly\n");
        return VTSS_RC_OK;
    }

    if ((rc = vtss_phy_ts_tesla_tsp_fifo_sync_priv(vtss_state, port_no, pr, fifo_conf, OOS, &recov)) != VTSS_RC_OK) {
#ifdef TESLA_FIFO_SYNC_OOS_WS_ERROR_TESTING
        if ((tesla_fifo_sync_counter > 0) &&
            (tesla_fifo_sync_counter % TESLA_FIFO_SYNC_OOS_EARLY_EXIT_COUNT) == 0) {
            VTSS_E("TESLA_FIFO_SYNC_OOS_WS_ERROR_TESTING: TESLA TSP_FIFO SYNC Skipping Early-Exit-Error-Recovery! port_no: %d\n", port_no);
        } else
#endif
        {
            vtss_rc   rc1 = 0;
            VTSS_E("TESLA TSP_FIFO SYNC Failed. port_no: %d\n", port_no);
            /* In the event there was a failure, Ensure EPG and Loopbacks are Disabled */
            /* Disable EPG */
            /* Make Sure that the EPG is Disabled Before Analyzer Reconfig !! */
            rc1 = vtss_phy_ts_tesla_error_recover_priv(vtss_state, port_no, pr, &recov);
            if (rc1 != VTSS_RC_OK) {
                VTSS_E("TESLA TSP_FIFO SYNC Failed AND FAILED RECONFIG of PHY. port_no: %d\n", port_no);
            } else {
                VTSS_I("TESLA TSP_FIFO SYNC Failed AND RECONFIG of PHY Successful. port_no: %d\n", port_no);
            }
        }
    }
    /* Only do a Re-Check if there was no ERROR Returned */
#ifdef TESLA_FIFO_SYNC_OOS_RE_CHECK_FIFO
    /* Only do a Re-Check if there was no ERROR Returned */
    VTSS_I("TSP_FIFO_SYNC_RE_CHECKING Port_no: %d, Previous OOS State: 0x%x ", port_no, *OOS);
    if (rc == VTSS_RC_OK) {
        if ((rc = vtss_phy_ts_tesla_tsp_recheck_fifo(vtss_state, port_no, pr, fifo_conf, OOS, &recov)) != VTSS_RC_OK) {
            VTSS_E("TESLA TSP_FIFO SYNC Failed. port_no: %d", port_no);
        }
    }
#endif

    //Restore EPG reg 29E1
    miim_write_func(vtss_state, port_no, 0x1f, VTSS_PHY_PAGE_EXTENDED);
    miim_write_func(vtss_state, port_no, 0x1d, value);
    miim_write_func(vtss_state, port_no, 0x1f, VTSS_PHY_PAGE_STANDARD);
    /* Switch over back from 8051 middle man approach */
    vtss_state->rd_ts_fifo = FALSE;

#ifdef VTSS_FIFO_SYNC_DEBUG
    gettimeofday(&tv, NULL);
    pr("Exit: Time Sec %ld, Usec  %ld\n", tv.tv_sec, tv.tv_usec);
#endif
    return rc;

}


#else
vtss_rc vtss_phy_ts_tesla_tsp_fifo_sync_private(const vtss_state_t   *vtss_state,
                                                const vtss_port_no_t port_no,
                                                const vtss_debug_printf_t       pr,
                                                const vtss_phy_ts_fifo_conf_t   *fifo_conf,
                                                BOOL *OOS)
{
    vtss_rc                 rc = VTSS_RC_OK;
    return rc;
}
#endif //TESLA_ING_TS_ERRFIX

/* ******************************************************************************************** */

// Function that returns the chip type/id for a given port.
// In     : port_no  - Internal port (starting from 0)
// In/Out : phy_id - Pointer to phy_id to be returned.
static vtss_rc vtss_phy_id_get_private(vtss_state_t *vtss_state, const vtss_port_no_t port_no, vtss_phy_type_t *phy_id)
{
    vtss_phy_port_state_t *ps = &vtss_state->phy_state[port_no];
    *phy_id = ps->type;
    VTSS_N("channel_id:%d, port_no:%d, phy_api_base_no:%d", phy_id->channel_id, port_no, phy_id->phy_api_base_no);
    return VTSS_RC_OK;
}

#if defined(VIPER_B_FIFO_RESET)
#define VTSS_PTP_EGR_IP_1588_DEBUG_REGISTERS_EGR_SW_POP_FIFO VTSS_IOREG(0xC0)
#define VTSS_PTP_INGR_IP_1588_DEBUG_REGISTERS_INGR_SW_POP_FIFO VTSS_IOREG(0x9f)
#define VTSS_F_PTP_INGR_IP_1588_DEBUG_REGISTERS_INGR_SW_POP_FIFO_AUTO_CLR VTSS_BIT(1)
#define VTSS_F_PTP_EGR_IP_1588_DEBUG_REGISTERS_EGR_SW_POP_FIFO_AUTO_CLR VTSS_BIT(1)

#define VTSS_F_PTP_INGR_IP_1588_DEBUG_REGISTERS_INGR_SW_POP_FIFO_INGR_SW_POP_FIFO VTSS_BIT(0)
#define VTSS_F_PTP_EGR_IP_1588_DEBUG_REGISTERS_EGR_SW_POP_FIFO_EGR_SW_POP_FIFO VTSS_BIT(0)
#define VTSS_PTP_EGR_IP_1588_DEBUG_REGISTERS_EGR_INTR_STATUS VTSS_IOREG(0x4D)
#define VTSS_PTP_INGR_IP_1588_DEBUG_REGISTERS_INGR_INTR_STATUS VTSS_IOREG(0x2D)

BOOL vtss_phy_ts_bypass_get(vtss_state_t  *vtss_state,
                            const vtss_port_no_t   port_no)
{
    u32 value = 0;

    VTSS_RC(VTSS_PHY_TS_READ_CSR(port_no, VTSS_PHY_TS_PROC_BLK_ID(0),
                                 VTSS_PTP_IP_1588_TOP_CFG_STAT_INTERFACE_CTL, &value));

    if ( value & VTSS_F_PTP_IP_1588_TOP_CFG_STAT_INTERFACE_CTL_EGR_BYPASS) {
        return TRUE;
    }

    return FALSE;
}

vtss_rc vtss_phy_ts_sw_pop_fifo(vtss_state_t *vtss_state,
                                const vtss_port_no_t port_no,
                                BOOL val)
{
    u32 value = 0;
    //set ingress sw_pop_fifo
    if (val) {
        VTSS_I("2.1 set ingress sw_pop_fifo on port %u\n", port_no);
        VTSS_RC(VTSS_PHY_TS_READ_CSR(port_no, VTSS_PHY_TS_PROC_BLK_ID(0),
                                     VTSS_PTP_INGR_IP_1588_DEBUG_REGISTERS_INGR_SW_POP_FIFO,
                                     &value));

        value = VTSS_PHY_TS_CLR_BITS(value, VTSS_F_PTP_INGR_IP_1588_DEBUG_REGISTERS_INGR_SW_POP_FIFO_INGR_SW_POP_FIFO);

        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(port_no, VTSS_PHY_TS_PROC_BLK_ID(0),
                                      VTSS_PTP_INGR_IP_1588_DEBUG_REGISTERS_INGR_SW_POP_FIFO,
                                      &value));
        //    VTSS_MSLEEP(1);

        value = 0;
        VTSS_RC(VTSS_PHY_TS_READ_CSR(port_no, VTSS_PHY_TS_PROC_BLK_ID(0),
                                     VTSS_PTP_INGR_IP_1588_DEBUG_REGISTERS_INGR_SW_POP_FIFO, &value));

        VTSS_I("INGR_SW_POP_FIFO port_no %u value 0x%x\n", port_no, value);

        //set egress sw_pop_fifo
        VTSS_I("2.1 set egress sw_pop_fifo on port %u\n", port_no);
        VTSS_RC(VTSS_PHY_TS_READ_CSR(port_no, VTSS_PHY_TS_PROC_BLK_ID(0),
                                     VTSS_PTP_EGR_IP_1588_DEBUG_REGISTERS_EGR_SW_POP_FIFO,
                                     &value));

        value = VTSS_PHY_TS_CLR_BITS(value, VTSS_F_PTP_EGR_IP_1588_DEBUG_REGISTERS_EGR_SW_POP_FIFO_EGR_SW_POP_FIFO);

        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(port_no, VTSS_PHY_TS_PROC_BLK_ID(0),
                                      VTSS_PTP_EGR_IP_1588_DEBUG_REGISTERS_EGR_SW_POP_FIFO,
                                      &value));
        //   VTSS_MSLEEP(1);

        value = 0;

        VTSS_RC(VTSS_PHY_TS_READ_CSR(port_no, VTSS_PHY_TS_PROC_BLK_ID(0),
                                     VTSS_PTP_EGR_IP_1588_DEBUG_REGISTERS_EGR_SW_POP_FIFO, &value));
        VTSS_I("EGR_SW_POP_FIFO port_no %u value 0x%x\n", port_no, value);

    } else {
        VTSS_I("2.1 set ingress sw_pop_fifo on port %u\n", port_no);
        VTSS_RC(VTSS_PHY_TS_READ_CSR(port_no, VTSS_PHY_TS_PROC_BLK_ID(0),
                                     VTSS_PTP_INGR_IP_1588_DEBUG_REGISTERS_INGR_SW_POP_FIFO,
                                     &value));
        value |= VTSS_F_PTP_INGR_IP_1588_DEBUG_REGISTERS_INGR_SW_POP_FIFO_INGR_SW_POP_FIFO;
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(port_no, VTSS_PHY_TS_PROC_BLK_ID(0),
                                      VTSS_PTP_INGR_IP_1588_DEBUG_REGISTERS_INGR_SW_POP_FIFO,
                                      &value));
        //    VTSS_MSLEEP(1);

        value = 0;
        VTSS_RC(VTSS_PHY_TS_READ_CSR(port_no, VTSS_PHY_TS_PROC_BLK_ID(0),
                                     VTSS_PTP_INGR_IP_1588_DEBUG_REGISTERS_INGR_SW_POP_FIFO, &value));

        VTSS_I("INGR_SW_POP_FIFO port_no %u value 0x%x\n", port_no, value);

        //set egress sw_pop_fifo
        VTSS_I("2.1 set egress sw_pop_fifo on port %u\n", port_no);
        VTSS_RC(VTSS_PHY_TS_READ_CSR(port_no, VTSS_PHY_TS_PROC_BLK_ID(0),
                                     VTSS_PTP_EGR_IP_1588_DEBUG_REGISTERS_EGR_SW_POP_FIFO,
                                     &value));
        value |= VTSS_F_PTP_EGR_IP_1588_DEBUG_REGISTERS_EGR_SW_POP_FIFO_EGR_SW_POP_FIFO;
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(port_no, VTSS_PHY_TS_PROC_BLK_ID(0),
                                      VTSS_PTP_EGR_IP_1588_DEBUG_REGISTERS_EGR_SW_POP_FIFO,
                                      &value));
        //   VTSS_MSLEEP(1);

        value = 0;
        VTSS_RC(VTSS_PHY_TS_READ_CSR(port_no, VTSS_PHY_TS_PROC_BLK_ID(0),
                                     VTSS_PTP_EGR_IP_1588_DEBUG_REGISTERS_EGR_SW_POP_FIFO, &value));
        VTSS_I("EGR_SW_POP_FIFO port_no %u value 0x%x\n", port_no, value);
    }
    return VTSS_RC_OK;

}

vtss_rc vtss_phy_ts_en_pkt_mode(vtss_state_t *vtss_state,
                                const vtss_port_no_t port_no,
                                BOOL enable)
{
    u32 value = 0;
    if (enable) {
        value = 0x4;
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(port_no, VTSS_PHY_TS_PROC_BLK_ID(0),
                                      VTSS_PTP_IP_1588_TOP_CFG_STAT_MODE_CTL,
                                      &value));
    } else {
        /*disable packet mode */
        value = 0x0;
        VTSS_RC(VTSS_PHY_TS_WRITE_CSR(port_no, VTSS_PHY_TS_PROC_BLK_ID(0),
                                      VTSS_PTP_IP_1588_TOP_CFG_STAT_MODE_CTL,
                                      &value));
    }
    VTSS_RC(VTSS_PHY_TS_READ_CSR(port_no, VTSS_PHY_TS_PROC_BLK_ID(0),
                                 VTSS_PTP_IP_1588_TOP_CFG_STAT_MODE_CTL, &value));

    VTSS_I("CFG_STAT_MODE_CTL  port_no %u value 0x%x\n", port_no, value);

    return VTSS_RC_OK;
}

vtss_rc vtss_phy_intr_status(vtss_state_t *vtss_state,
                             const vtss_port_no_t port_no,
                             vtss_phy_ts_overflow_info_t *overflow_conf)
{

    u32 value = 0;

    VTSS_RC(VTSS_PHY_TS_READ_CSR(port_no, VTSS_PHY_TS_PROC_BLK_ID(0),
                                 VTSS_PTP_EGR_IP_1588_DEBUG_REGISTERS_EGR_INTR_STATUS, &value));

    overflow_conf->egr_intr_status = value;

    VTSS_RC(VTSS_PHY_TS_READ_CSR(port_no, VTSS_PHY_TS_PROC_BLK_ID(0),
                                 VTSS_PTP_INGR_IP_1588_DEBUG_REGISTERS_INGR_INTR_STATUS, &value));

    overflow_conf->ingr_intr_status = value;

    //write the interrupt status back.
    value = overflow_conf->egr_intr_status;
    VTSS_RC(VTSS_PHY_TS_WRITE_CSR(port_no, VTSS_PHY_TS_PROC_BLK_ID(0),
                                  VTSS_PTP_EGR_IP_1588_DEBUG_REGISTERS_EGR_INTR_STATUS,
                                  &value));

    value = overflow_conf->ingr_intr_status;
    VTSS_RC(VTSS_PHY_TS_WRITE_CSR(port_no, VTSS_PHY_TS_PROC_BLK_ID(0),
                                  VTSS_PTP_INGR_IP_1588_DEBUG_REGISTERS_INGR_INTR_STATUS,
                                  &value));

    return VTSS_RC_OK;
}

vtss_rc vtss_phy_sw_pop_fifo_get(vtss_state_t *vtss_state,
                                 const vtss_port_no_t port_no,
                                 vtss_phy_ts_pop_fifo_t *pop_fifo)
{
    u32 value = 0;

    VTSS_RC(VTSS_PHY_TS_READ_CSR(port_no, VTSS_PHY_TS_PROC_BLK_ID(0),
                                 VTSS_PTP_EGR_IP_1588_DEBUG_REGISTERS_EGR_SW_POP_FIFO, &value));

    pop_fifo->egr_pop_fifo = value;

    VTSS_RC(VTSS_PHY_TS_READ_CSR(port_no, VTSS_PHY_TS_PROC_BLK_ID(0),
                                 VTSS_PTP_INGR_IP_1588_DEBUG_REGISTERS_INGR_SW_POP_FIFO , &value));

    pop_fifo->ingr_pop_fifo = value;

    return VTSS_RC_OK;
}

BOOL  vtss_phy_ts_algo_execute_check(vtss_state_t *vtss_state,
                                     const vtss_port_no_t port_no)
{
    u32 value = 0;
    u32 ingr_eng, egr_eng;
    if (vtss_state->phy_ts_port_conf[port_no].port_ts_init_done == FALSE) {
        VTSS_I("Port ts not initialized! port: %d", port_no);
        return FALSE;
    }
    /* Check if 1588 clock is enabled or not */
    value = 0;
    VTSS_RC(VTSS_PHY_TS_READ_CSR(port_no, VTSS_PHY_TS_PROC_BLK_ID(0),
                                 VTSS_PTP_IP_1588_TOP_CFG_STAT_INTERFACE_CTL, &value));

    if (value & VTSS_F_PTP_IP_1588_TOP_CFG_STAT_INTERFACE_CTL_CLK_ENA) {
        VTSS_I("1588 Clk is enabled - OOS Check Supported ");
    } else {
        VTSS_I("1588 Clk is Disabled - OOS Check NOT Supported - Returning!");
        //        VTSS_PHY_TS_SPI_UNPAUSE_PRIV(port_no);
        return FALSE;
    }
    value = 0;
    VTSS_RC(VTSS_PHY_TS_READ_CSR(port_no, VTSS_PHY_TS_PROC_BLK_ID_0,
                                 VTSS_PTP_IP_1588_TOP_CFG_STAT_ANALYZER_MODE, &value));

    ingr_eng = value & 0x7;
    egr_eng = value & ( 0x7 << 4);

    VTSS_I("ingr_eng %u egr_eng %u\n", ingr_eng, egr_eng);
    if (!(ingr_eng | egr_eng)) {
        return FALSE;
    }


    return TRUE;
}


vtss_rc vtss_phy_1588_oos_mitigation_steps_private(vtss_state_t *vtss_state,
                                                   const vtss_port_no_t port_no,
                                                   const vtss_phy_ts_fifo_conf_t *fifo_conf);


static vtss_rc vtss_phy_1588_oos_mitigation_steps_execute(vtss_state_t *vtss_state,
                                                          const vtss_port_no_t port_no,
                                                          BOOL copper,
                                                          BOOL ams_mode,
                                                          BOOL cu_sfp);

//proc core_patch {port proc_id} {

static vtss_rc vtss_phy_10g_oos_core_patch( vtss_state_t *vtss_state,
                                            const vtss_port_no_t port_no)
{
    u16 reg;
#if defined(VTSS_FEATURE_MACSEC)
    BOOL macsec_enable = FALSE;
    macsec_enable = vtss_state->macsec_conf[port_no].glb.init.enable;
#endif

    VTSS_I("Executing core patch in pkt mode to clear fifo reset bit ox69\n");

    //TeslaCsrWrite $port $proc_id 0x02 0x4;  #Mode ctrl, enable pkt mode
#if defined(VTSS_FEATURE_MACSEC)
    if (!macsec_enable)
#endif
    {
        VTSS_RC(vtss_phy_ts_en_pkt_mode(vtss_state, port_no, TRUE));
    }
    //TeslaCsrWrite $port $proc_id 0x00 0x46;#Enable 1588 Bypass
    vtss_phy_ts_bypass_set(vtss_state, port_no, TRUE, TRUE);

    //Ext4MiiWriteBits 0 21 15 15 1;#1588 reset

    VTSS_RC(vtss_phy_page_ext4(vtss_state, port_no));
    VTSS_RC(vtss_phy_rd(vtss_state, port_no, 31, &reg));
    VTSS_I("Current Page Value Port_no %u Value 0x%x\n", port_no, reg);

    VTSS_RC(PHY_WR_MASKED_PAGE(vtss_state, port_no, VTSS_PHY_1588_PPS_0_MUX_CTRL , VTSS_F_PHY_1588_PPS_0_MUX_CTRL_1588_SOFT_RESET,
                               VTSS_F_PHY_1588_PPS_0_MUX_CTRL_1588_SOFT_RESET));
    VTSS_RC(PHY_RD_PAGE(vtss_state, port_no, VTSS_PHY_1588_PPS_0_MUX_CTRL , &reg));

    VTSS_I("VTSS_PHY_1588_PPS_0_MUX_CTRL Port_no %u Value 0x%x\n", port_no, reg);

    //after 1
    VTSS_MSLEEP(1);

    //TeslaCsrWrite $port $proc_id 0x00 0x42;#Disable 1588 bypass
    vtss_phy_ts_bypass_set(vtss_state, port_no, FALSE, TRUE);

    //TeslaCsrWrite $port $proc_id 0x9F 0x69;#SW_POP_FIFO
    //TeslaCsrWrite $port $proc_id 0xC0 0x69
    VTSS_RC(vtss_phy_ts_sw_pop_fifo(vtss_state, port_no, FALSE));

    //after 1
    VTSS_MSLEEP(1);

    //ExtMiiWrite $port 29 0x8042;#Enable EPG
    VTSS_RC(vtss_phy_page_ext(vtss_state, port_no));        // Switch to extended register-page 1
    VTSS_RC(vtss_phy_rd(vtss_state, port_no, 31, &reg));
    VTSS_I("Current Page Value Port_no %u Value 0x%x\n", port_no, reg);

    VTSS_RC(PHY_WR_PAGE(vtss_state, port_no, EPG_CTRL_REG_1, 0x8042));
    VTSS_RC(PHY_RD_PAGE(vtss_state, port_no, EPG_CTRL_REG_1, &reg));
    VTSS_I("EPG_CTRL_REG_1 Port_no %u Value 0x%x\n", port_no, reg);

    //ExtMiiWriteBits $port 29 14 14 1;#Trigger 300 EPG frames
    VTSS_RC(PHY_RD_PAGE(vtss_state, port_no, EPG_CTRL_REG_1, &reg));
    reg = reg | 0x4000;
    VTSS_RC(PHY_WR_PAGE(vtss_state, port_no, EPG_CTRL_REG_1, reg));

    //after 10
    VTSS_MSLEEP(10);

    //TeslaCsrWrite $port $proc_id 0x00 0x46;#Enable 1588 Bypass
    vtss_phy_ts_bypass_set(vtss_state, port_no, TRUE, TRUE);

    //Ext4MiiWriteBits 0 21 15 15 1;#1588 reset
    VTSS_RC(vtss_phy_page_ext4(vtss_state, port_no));
    VTSS_RC(vtss_phy_rd(vtss_state, port_no, 31, &reg));
    VTSS_I("Current Page Value Port_no %u Value 0x%x\n", port_no, reg);

    VTSS_RC(PHY_WR_MASKED_PAGE(vtss_state, port_no, VTSS_PHY_1588_PPS_0_MUX_CTRL , VTSS_F_PHY_1588_PPS_0_MUX_CTRL_1588_SOFT_RESET,
                               VTSS_F_PHY_1588_PPS_0_MUX_CTRL_1588_SOFT_RESET));
    VTSS_RC(PHY_RD_PAGE(vtss_state, port_no, VTSS_PHY_1588_PPS_0_MUX_CTRL , &reg));

    VTSS_I("VTSS_PHY_1588_PPS_0_MUX_CTRL Port_no %u Value 0x%x\n", port_no, reg);

    //after 1
    VTSS_MSLEEP(1);

    //TeslaCsrWrite $port $proc_id 0x00 0x42;#Disable 1588 bypass
    vtss_phy_ts_bypass_set(vtss_state, port_no, FALSE, TRUE);

    //TeslaCsrWrite $port $proc_id 0x9F 0x69;
    //TeslaCsrWrite $port $proc_id 0xC0 0x69
    VTSS_RC(vtss_phy_ts_sw_pop_fifo(vtss_state, port_no, FALSE));

    //TeslaCsrWrite $port $proc_id 0x02 0x0;  #Mode ctrl pkt mode disable
#if defined(VTSS_FEATURE_MACSEC)
    if (!macsec_enable)
#endif
    {
        VTSS_RC(vtss_phy_ts_en_pkt_mode(vtss_state, port_no, FALSE));
    }

    return VTSS_RC_OK;

}
vtss_rc vtss_phy_ts_viper_fifo_reset(const vtss_inst_t               inst,
                                     const vtss_port_no_t            port_no,
                                     const vtss_phy_ts_fifo_conf_t *fifo_conf)
{
    vtss_rc    rc = VTSS_RC_ERROR;
    vtss_state_t   *vtss_state;

    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst,  &vtss_state, port_no)) == VTSS_RC_OK) {
        rc = vtss_phy_1588_oos_mitigation_steps_private(vtss_state, port_no, fifo_conf);
    }
    VTSS_EXIT();
    return rc;
}


static vtss_rc token_reg_write_func(vtss_state_t *vtss_state, const vtss_port_no_t port_no, u32 register_val, u16 value1, u16 mask1, u16 value2, u16 mask2)
{
    u16 reg;
    u32 address;

    VTSS_RC(vtss_phy_page_tr(vtss_state, port_no));    // Go to Token Ring Page 0x52b5
    VTSS_RC(vtss_phy_rd(vtss_state, port_no, 31, &reg));
    VTSS_I("Current Page Value Port_no %u Value 0x%x\n", port_no, reg);

    address = 0xa000 | register_val;

    VTSS_RC(PHY_WR_PAGE(vtss_state, port_no, VTSS_PHY_PAGE_TR_16, address));   // Setup Address, Reg16 for READING Address 0xc80
    VTSS_RC(PHY_RD_PAGE(vtss_state, port_no, VTSS_PHY_PAGE_TR_16, &reg));
    VTSS_I("VTSS_PHY_PAGE_TR_16 Port_no %u Value 0x%x\n", port_no, reg);


    VTSS_RC(PHY_WR_MASKED_PAGE(vtss_state, port_no, VTSS_PHY_PAGE_TR_18, value1, mask1)); /*- Read-Modify-Write of Reg18 to have a value of 0x0045 */
    VTSS_RC(PHY_RD_PAGE(vtss_state, port_no, VTSS_PHY_PAGE_TR_18, &reg));
    VTSS_I("VTSS_PHY_PAGE_TR_18 Port_no %u Value 0x%x\n", port_no, reg);

    VTSS_RC(PHY_WR_MASKED_PAGE(vtss_state, port_no, VTSS_PHY_PAGE_TR_17, value2, mask2)); /*- Read-Modify-Write of Reg17 to have a value of 0x1000 */
    VTSS_RC(PHY_RD_PAGE(vtss_state, port_no, VTSS_PHY_PAGE_TR_17, &reg));
    VTSS_I("VTSS_PHY_PAGE_TR_17 Port_no %u Value 0x%x\n", port_no, reg);

    address = 0x8000 | register_val;

    VTSS_RC(PHY_WR_PAGE(vtss_state, port_no, VTSS_PHY_PAGE_TR_16, address));   // Setup Address, Reg16 for WRITING Address 0xc80
    VTSS_RC(PHY_RD_PAGE(vtss_state, port_no, VTSS_PHY_PAGE_TR_16, &reg));
    VTSS_I("VTSS_PHY_PAGE_TR_16 Port_no %u Value 0x%x\n", port_no, reg);

    VTSS_RC(vtss_phy_page_std(vtss_state, port_no));   // Go back to Std Page
    VTSS_RC(vtss_phy_rd(vtss_state, port_no, 31, &reg));
    VTSS_I("Current Page Value Port_no %u Value 0x%x\n", port_no, reg);

    return VTSS_RC_OK;
}


//FOR 100BT and 10BT speeds.
static vtss_rc vtss_phy_1588_oos_mitigation_save_cfg(vtss_state_t *vtss_state,
                                                     const vtss_port_no_t port_no,
                                                     u16 *reg0,
                                                     u16 *reg9,
                                                     u16 *extreg29,
                                                     u16 *reg23)
{
    u16 reg;
    //Save Reg 0 and Reg 9 and register 29E1
    // set reg0 [MiiRead $port 0]
    VTSS_RC(vtss_phy_page_std(vtss_state, port_no));   // Go back to Std Page
    VTSS_RC(vtss_phy_rd(vtss_state, port_no, 31, &reg));
    VTSS_I("Current Page Value Port_no %u Value 0x%x\n", port_no, reg);

    //read register 0
    VTSS_RC(PHY_RD_PAGE(vtss_state, port_no, VTSS_PHY_MODE_CONTROL , reg0));
    //set reg9 [MiiRead $port 9]
    //Save register 9
    VTSS_RC(PHY_RD_PAGE(vtss_state, port_no,  VTSS_PHY_1000BASE_T_CONTROL , reg9));

    VTSS_RC(PHY_RD_PAGE(vtss_state, port_no,  VTSS_PHY_EXTENDED_PHY_CONTROL , reg23));

    VTSS_RC(vtss_phy_page_ext(vtss_state, port_no));
    VTSS_RC(PHY_RD_PAGE(vtss_state, port_no, EPG_CTRL_REG_1, extreg29));

    VTSS_RC(vtss_phy_page_std(vtss_state, port_no));

    return VTSS_RC_OK;
}

vtss_rc vtss_phy_ts_isolate_phy(vtss_state_t *vtss_state,
                                const vtss_port_no_t port_no)
{
    vtss_phy_type_t                phy_id;
    //Only Perform the Operation for Viper B.

    VTSS_RC(vtss_phy_id_get_private(vtss_state, port_no, &phy_id));

    if (!((phy_id.part_number == VTSS_PHY_TYPE_8582) || (phy_id.part_number == VTSS_PHY_TYPE_8584) || (phy_id.part_number == VTSS_PHY_TYPE_8575))) {
        VTSS_I("Not A Viper PHY, returning port_no %u\n", port_no);
        return VTSS_RC_OK;
    }

    if (!(phy_id.revision >= 1)) {
        VTSS_I("Viper Rev A, OOS algorithm should not be executed\n");
        return VTSS_RC_OK;
    }

    if ((vtss_phy_ts_algo_execute_check(vtss_state, port_no)) == FALSE) {
        VTSS_I("PTP is not configured on the port exiting quietly\n");
        return VTSS_RC_OK;
    }


    VTSS_RC(token_reg_write_func(vtss_state, port_no, 0x0cc0, 0x2, 0x2, 0x610, 0x610));

    //TeslaCsrWrite $port $proc_id 0x00 0x46;#Enable 1588 Bypass
    VTSS_RC(vtss_phy_ts_bypass_set(vtss_state, port_no, TRUE, TRUE));

    //after 1
    VTSS_MSLEEP(1);

    //TrWrite $port mac_isolate_rx -1 1;#Prevent mac side traffic
    VTSS_I("3.Configure mac_isolate_rx and wait for 1m (to prevent media side traffic) \n");
    VTSS_RC(token_reg_write_func(vtss_state, port_no, 0x0c80, 0x0045, 0x0045, 0x1000, 0x1000));

    //TrWrite $port mac_isolate_tx -1 1
    VTSS_I("3.Configure mac_isolate_tx and wait for 1m (to prevent media side traffic) \n");
    VTSS_RC(token_reg_write_func(vtss_state, port_no, 0x0c80, 0x0085, 0x0085, 0x1000, 0x1000));

    return VTSS_RC_OK;

}
vtss_rc vtss_phy_1588_oos_mitigation_isolate_phy(vtss_state_t *vtss_state,
                                                 const vtss_port_no_t port_no,
                                                 BOOL copper)
{
    vtss_rc rc = VTSS_RC_OK;

    //TrWrite $port epg_testmode -1 1

    VTSS_I("Enabling EPG in test mode\n");
    rc = token_reg_write_func(vtss_state, port_no, 0x0cc0, 0x2, 0x2, 0x610, 0x610);

    //TeslaCsrWrite $port $proc_id 0x00 0x46;#Enable 1588 Bypass
    rc = vtss_phy_ts_bypass_set(vtss_state, port_no, TRUE, TRUE);

    //after 1
    VTSS_MSLEEP(1);

    //TrWrite $port mac_isolate_rx -1 1;#Prevent mac side traffic
    VTSS_I("3.Configure mac_isolate_rx and wait for 1m (to prevent media side traffic) \n");
    rc = token_reg_write_func(vtss_state, port_no, 0x0c80, 0x0045, 0x0045, 0x1000, 0x1000);

    //TrWrite $port mac_isolate_tx -1 1
    VTSS_I("3.Configure mac_isolate_tx and wait for 1m (to prevent media side traffic) \n");
    rc = token_reg_write_func(vtss_state, port_no, 0x0c80, 0x0085, 0x0085, 0x1000, 0x1000);

    if (copper) {
        //MiiWrite $port 9 0x1F00;#Force Master
        VTSS_RC(PHY_WR_PAGE(vtss_state, port_no, VTSS_PHY_1000BASE_T_CONTROL, 0x1F00));

        //MiiWriteBits $port 0 9 9 1;#Restart ANEG
        VTSS_RC(PHY_WR_MASKED_PAGE(vtss_state, port_no, VTSS_PHY_MODE_CONTROL,
                                   VTSS_F_PHY_MODE_CONTROL_RESTART_AUTO_NEG,
                                   VTSS_F_PHY_MODE_CONTROL_RESTART_AUTO_NEG));

        //MiiWrite $port 0 0x4140; #Forced 1G mode and Near-End loopback
        VTSS_RC(PHY_WR_PAGE(vtss_state, port_no, VTSS_PHY_MODE_CONTROL, 0x4140));
    } else {
        //set Near-end loopback
        VTSS_RC(PHY_WR_MASKED_PAGE(vtss_state, port_no, VTSS_PHY_MODE_CONTROL,
                                   VTSS_F_PHY_MODE_CONTROL_LOOP,
                                   VTSS_F_PHY_MODE_CONTROL_LOOP));
    }

    return VTSS_RC_OK;
}
static vtss_rc  vtss_phy_1588_oos_mitigation_media_interface_link_status(vtss_state_t *vtss_state,
                                                                         const vtss_port_no_t port_no)
{
    u16 reg;
    u32 count;
    reg = count = 0;

    VTSS_I("vtss_phy_1588_oos_mitigation_media_interface_link_status  Media connected is Cu-SFP checking media interface link status\n");
    VTSS_RC(vtss_phy_page_ext3(vtss_state, port_no));    // Go to Extended page 3
    VTSS_RC(vtss_phy_rd(vtss_state, port_no, 31, &reg));
    VTSS_I("Current Page Value Port_no %u Value 0x%x\n", port_no, reg);

    VTSS_RC(PHY_RD_PAGE(vtss_state, port_no, VTSS_PHY_MEDIA_SERDES_PCS_STATUS, &reg));

    count = 0;

    while (!(reg & VTSS_F_PHY_MEDIA_SERDES_PCS_STATUS_MEDIA_LINK_STATUS)) {
        count++;

        if (count >= 10) {
            VTSS_E("LOCAL REMOTE STATUS GOOD POLL took too much time\n");
            break;
        }
        VTSS_I("VTSS_PHY_1000BASE_T_STATUS 14:12 still not set\n");
        VTSS_RC(PHY_RD_PAGE(vtss_state, port_no, VTSS_PHY_MEDIA_SERDES_PCS_STATUS, &reg));
        VTSS_I("VTSS_PHY_1000BASE_T_STATUS value %x", reg);
        VTSS_MSLEEP(1);
    }

    VTSS_RC(vtss_phy_page_std(vtss_state, port_no));    // Go to standard Page

    return VTSS_RC_OK;

}
static vtss_rc vtss_phy_1588_oos_mitigation_check_frames_remote_local_good(vtss_state_t *vtss_state,
                                                                           const vtss_port_no_t port_no)
{
    u16 reg, rec_cnt, rx_er, tx_cnt;
    u32 count;
    BOOL check_for_no_frames;
    //while {![MiiReadBits $port 10 14 12] == "0x7"} {};#Waiting to receive remote&local status good
    VTSS_RC(PHY_RD_PAGE(vtss_state, port_no, VTSS_PHY_1000BASE_T_STATUS, &reg));

    count = 0;

    while (!((reg & 0x7000) == 0x7000)) {
        count++;

        if (count >= 10) {
            VTSS_E("LOCAL REMOTE STATUS GOOD POLL took too much time\n");
            break;
        }
        VTSS_I("VTSS_PHY_1000BASE_T_STATUS 14:12 still not set\n");
        VTSS_RC(PHY_RD_PAGE(vtss_state, port_no, VTSS_PHY_1000BASE_T_STATUS, &reg));
        VTSS_I("VTSS_PHY_1000BASE_T_STATUS value %x", reg);
        VTSS_MSLEEP(1);
    }

    //after 1
    VTSS_MSLEEP(1);


    VTSS_RC(vtss_phy_page_ext(vtss_state, port_no));
    check_for_no_frames = 1;
    count = 0;
    while (check_for_no_frames) {

        count++;
        if (count > 10) {
            VTSS_E("No of times this loop has run is greater than 10 Exiting\n");
            break;
        }

        VTSS_RC(vtss_phy_page_ext(vtss_state, port_no));
        VTSS_RC(vtss_phy_rd(vtss_state, port_no, 18, &rec_cnt));
        VTSS_RC(vtss_phy_rd(vtss_state, port_no, 19, &rx_er));
        rec_cnt &= 0x3FFF;
        rx_er &= 0xFF;

        tx_cnt = 0;
        //tx_cnt read using the register reads provided in image
        VTSS_RC(vtss_phy_page_tr(vtss_state, port_no));  // Set page to token ring.
        VTSS_RC(vtss_phy_rd(vtss_state, port_no, 31, &reg));
        VTSS_I("Current Page Value Port_no %u Value 0x%x\n", port_no, reg);

        VTSS_RC(PHY_WR_PAGE(vtss_state, port_no, VTSS_PHY_PAGE_TR_16, 0xbf88));   // Setup Address, Reg16 for READING Address 0xc80
        VTSS_RC(PHY_RD_PAGE(vtss_state, port_no, VTSS_PHY_PAGE_TR_16, &reg));
        VTSS_I("VTSS_PHY_PAGE_TR_16 Port_no %u Value 0x%x\n", port_no, reg);

        VTSS_RC(PHY_RD_PAGE(vtss_state, port_no, VTSS_PHY_PAGE_TR_18, &reg));
        tx_cnt |= reg << 16;
        VTSS_RC(PHY_RD_PAGE(vtss_state, port_no, VTSS_PHY_PAGE_TR_17, &reg));
        tx_cnt |= reg;

        //        VTSS_RC(vtss_phy_page_std(vtss_state, port_no));   // Go back to Std Page
        //        VTSS_RC(vtss_phy_rd(vtss_state, port_no, 31, &reg));
        //        VTSS_I("Current Page Value Port_no %u Value 0x%x\n", port_no, reg);

        VTSS_I("tx_cnt %u, rec_cnt %u rx_er %u\n", tx_cnt, rec_cnt, rx_er);

        if ((rec_cnt == 0) && (rx_er == 0) && (tx_cnt == 0)) {
            check_for_no_frames = 0;
        }
        VTSS_MSLEEP(1);
    }
    VTSS_RC(vtss_phy_page_std(vtss_state, port_no));

    return VTSS_RC_OK;
}
static vtss_rc vtss_phy_1588_oos_mitigation_soft_reset_pop_fifo(vtss_state_t *vtss_state,
                                                                const vtss_port_no_t port_no)
{
    u16 reg;
    vtss_phy_ts_overflow_info_t overflow_conf;
    //Ext4MiiWriteBits 0 21 15 15 1;#1588 reset
    VTSS_I("7.Give 1588 soft reset\n");
    VTSS_RC(vtss_phy_page_ext4(vtss_state, port_no));
    VTSS_RC(vtss_phy_rd(vtss_state, port_no, 31, &reg));
    VTSS_I("Current Page Value Port_no %u Value 0x%x\n", port_no, reg);

    VTSS_RC(PHY_WR_MASKED_PAGE(vtss_state, port_no, VTSS_PHY_1588_PPS_0_MUX_CTRL , VTSS_F_PHY_1588_PPS_0_MUX_CTRL_1588_SOFT_RESET,
                               VTSS_F_PHY_1588_PPS_0_MUX_CTRL_1588_SOFT_RESET));
    VTSS_RC(PHY_RD_PAGE(vtss_state, port_no, VTSS_PHY_1588_PPS_0_MUX_CTRL , &reg));

    VTSS_I("VTSS_PHY_1588_PPS_0_MUX_CTRL Port_no %u Value 0x%x\n", port_no, reg);

    //after 1
    VTSS_MSLEEP(1);

    //TeslaCsrWrite $port $proc_id 0x00 0x42;#Disable 1588 bypass
    vtss_phy_ts_bypass_set(vtss_state, port_no, FALSE, TRUE);

    //after 1
    VTSS_MSLEEP(1);
    //TeslaCsrWrite $port $proc_id 0x9F 0x69;#SW_POP_FIFO
    //TeslaCsrWrite $port $proc_id 0xC0 0x69
    VTSS_RC(vtss_phy_ts_sw_pop_fifo(vtss_state, port_no, FALSE));

    //TeslaCsrWrite $port $proc_id 0x9F 0x68;
    //TeslaCsrWrite $port $proc_id 0xC0 0x68
    VTSS_RC(vtss_phy_ts_sw_pop_fifo(vtss_state, port_no, TRUE));
    // set d2 [TeslaCsrRead $port $proc_id 0x2D]
    // set d4 [TeslaCsrRead $port $proc_id 0x4D]
    // TeslaCsrWrite $port $proc_id 0x2D $d2;
    //     TeslaCsrWrite $port $proc_id 0x4D $d4
    VTSS_RC(vtss_phy_intr_status(vtss_state, port_no, &overflow_conf));

    return VTSS_RC_OK;
}
static vtss_rc vtss_phy_1588_oos_mitigation_epg_transmit_frames(vtss_state_t *vtss_state,
                                                                const vtss_port_no_t port_no,
                                                                BOOL copper)
{
    u16 reg;
    u32 count;
    u16 rec_cnt;

    //ExtMiiRead $port 18
    VTSS_I("Read Good CRC counter\n");
    VTSS_RC(vtss_phy_page_ext(vtss_state, port_no));        // Switch to extended register-page 1
    VTSS_RC(vtss_phy_rd(vtss_state, port_no, 31, &reg));
    VTSS_I("Current Page Value Port_no %u Value 0x%x\n", port_no, reg);
    VTSS_RC(PHY_RD_PAGE(vtss_state, port_no, VTSS_PHY_CU_MEDIA_CRC_GOOD_COUNTER, &reg));

    VTSS_I("Value of register VTSS_PHY_CU_MEDIA_CRC_GOOD_COUNTER is 0x%x\n", reg);

    //ExtMiiWrite $port 29 0x8042;#Enable EPG
    VTSS_I("9.Enable EPG. Just enable, don't start it\n");
    VTSS_RC(vtss_phy_page_ext(vtss_state, port_no));        // Switch to extended register-page 1
    VTSS_RC(vtss_phy_rd(vtss_state, port_no, 31, &reg));
    VTSS_I("Current Page Value Port_no %u Value 0x%x\n", port_no, reg);

    VTSS_RC(PHY_WR_PAGE(vtss_state, port_no, EPG_CTRL_REG_1, 0x8042));
    VTSS_RC(PHY_RD_PAGE(vtss_state, port_no, EPG_CTRL_REG_1, &reg));
    VTSS_I("EPG_CTRL_REG_1 Port_no %u Value 0x%x\n", port_no, reg);


    //ExtMiiWriteBits $port 29 14 14 1;#Trigger 300 EPG frames
    VTSS_RC(PHY_RD_PAGE(vtss_state, port_no, EPG_CTRL_REG_1, &reg));
    reg = reg | 0x4000;
    VTSS_RC(PHY_WR_PAGE(vtss_state, port_no, EPG_CTRL_REG_1, reg));

    if (copper) {
        count = 0;
        VTSS_RC(PHY_RD_PAGE(vtss_state, port_no, EPG_CTRL_REG_1, &reg));

        while (reg & 0x4000) {
            if (count >= 10) {
                VTSS_E("EPG taking way too long to transmit packets\n");
            }
            VTSS_I("Waiting to transmit EPG frames\n");
            VTSS_MSLEEP(1);
            VTSS_RC(PHY_RD_PAGE(vtss_state, port_no, EPG_CTRL_REG_1, &reg));
            count++;
        }
    }
    //after 10
    VTSS_MSLEEP(10);

    //set rec_cnt [hex2dec [ExtMiiReadBits $port 18 13 0]]
    VTSS_RC(vtss_phy_page_ext(vtss_state, port_no));

    VTSS_RC(vtss_phy_rd(vtss_state, port_no, 18, &rec_cnt));
    //htEcho 1 "----Received $rec_cnt"
    rec_cnt &= 0x3FFF;
    VTSS_I("Recieved %u\n", rec_cnt);

    rec_cnt = 0;
    //[Ext3MiiReadBits $port 28 13 0]
    VTSS_RC(vtss_phy_page_ext3(vtss_state, port_no));
    VTSS_RC(vtss_phy_rd(vtss_state, port_no, 28, &rec_cnt));
    VTSS_I("Media-MAC SerDes Receive CRC Good Counter rec_cnt %u\n", rec_cnt);


    return VTSS_RC_OK;
}
static vtss_rc vtss_phy_1588_oos_mitigation_execute_core_patch_execute(vtss_state_t *vtss_state,
                                                                       const vtss_port_no_t port_no)
{
    vtss_phy_ts_overflow_info_t overflow_conf;
    vtss_phy_ts_pop_fifo_t pop_fifo;
    u32 core_patch_cnt;

    //htEcho 1 "2D:[TeslaCsrRead $port $proc_id 0x2D]"
    //htEcho 1 "4D:[TeslaCsrRead $port $proc_id 0x4D]"
    //htEcho 1 "9F:[TeslaCsrRead $port $proc_id 0x9F]"
    //htEcho 1 "C0:[TeslaCsrRead $port $proc_id 0xC0]"

    VTSS_I("Value of register 0x2D INGR_INT_STATUS is 0x%x\n", overflow_conf.ingr_intr_status);
    VTSS_I("Value of register 0x4D EGR_INT_STATUS is 0x%x\n", overflow_conf.egr_intr_status);
    VTSS_RC(vtss_phy_sw_pop_fifo_get(vtss_state, port_no, &pop_fifo));
    VTSS_I("Value of register 0x9F INGR_SW_POP_FIFO is 0x%x\n", pop_fifo.egr_pop_fifo);
    VTSS_I("Value of register 0xC0 EGR_SW_POP_FIFO is 0x%x\n", pop_fifo.ingr_pop_fifo);

    //set core_patch_cnt 0
    core_patch_cnt = 0;


    while ((pop_fifo.ingr_pop_fifo  == 0x69) || (pop_fifo.egr_pop_fifo  == 0x69)) {

        core_patch_cnt++;

        if (core_patch_cnt > 10) {
            VTSS_I("Core patch executing way too many times, exiting\n");
            break;

        }
        //check if the core patch is the same for 100BT and normal Cu-Fiber Operations => It is same
        //core_patch $port $proc_id
        VTSS_RC(vtss_phy_10g_oos_core_patch(vtss_state, port_no));

        //htEcho 1 "2D:[TeslaCsrRead $port $proc_id 0x2D]"
        //htEcho 1 "4D:[TeslaCsrRead $port $proc_id 0x4D]"
        //htEcho 1 "9F:[TeslaCsrRead $port $proc_id 0x9F]"
        //htEcho 1 "C0:[TeslaCsrRead $port $proc_id 0xC0]"
        VTSS_RC(vtss_phy_intr_status(vtss_state, port_no, &overflow_conf));
        VTSS_I("Value of register 0x2D INGR_INT_STATUS is 0x%x\n", overflow_conf.ingr_intr_status);
        VTSS_I("Value of register 0x4D EGR_INT_STATUS is 0x%x\n", overflow_conf.egr_intr_status);
        VTSS_RC(vtss_phy_sw_pop_fifo_get(vtss_state, port_no, &pop_fifo));
        VTSS_I("Value of register 0x9F INGR_SW_POP_FIFO is 0x%x\n", pop_fifo.egr_pop_fifo);
        VTSS_I("Value of register 0xC0 EGR_SW_POP_FIFO is 0x%x\n", pop_fifo.ingr_pop_fifo);

    }
    return VTSS_RC_OK;
}
static vtss_rc vtss_phy_1588_oos_mitigation_restore_setup(vtss_state_t *vtss_state, const vtss_port_no_t port_no, u16 *reg0, u16 *reg9, u16 *extreg29, u16 *reg23)
{
    u16 reg;
    vtss_rc rc = VTSS_RC_OK;
    //ExtMiiWriteBits $port 29 15 15 0;#Disable EPG to enable datapath
    VTSS_I("11.   Disable EPG bit. (This will enable normal datapath.)\n");
    VTSS_RC(vtss_phy_page_ext(vtss_state, port_no));        // Switch to extended register-page 1
    VTSS_RC(vtss_phy_rd(vtss_state, port_no, 31, &reg));
    VTSS_I("Current Page Value Port_no %u Value 0x%x\n", port_no, reg);

    VTSS_RC(PHY_WR_PAGE(vtss_state, port_no, EPG_CTRL_REG_1, *extreg29));
    VTSS_RC(PHY_RD_PAGE(vtss_state, port_no, EPG_CTRL_REG_1, &reg));
    VTSS_I("EPG_CTRL_REG_1 Port_no %u Value 0x%x\n", port_no, reg);

    //TrWrite $port epg_testmode -1 0
    VTSS_I("Disable EPG Test mode\n");

    rc = token_reg_write_func(vtss_state, port_no, 0x0cc0, 0x0, 0x2, 0x610, 0x610);

    //after 1
    VTSS_MSLEEP(1);

    VTSS_RC(PHY_WR_PAGE(vtss_state, port_no, VTSS_PHY_EXTENDED_PHY_CONTROL, *reg23));

    //TrWrite $port mac_isolate_rx -1 0
    VTSS_I("16.Disable mac_isolate_rx and wait for 1ms\n");
    rc = token_reg_write_func(vtss_state, port_no, 0x0c80, 0x0005, 0x0045, 0x1000, 0x1000);

    //TrWrite $port mac_isolate_tx -1 0
    VTSS_I("16.5.Disable mac_isolate_tx and wait for 1ms\n");
    rc = token_reg_write_func(vtss_state, port_no, 0x0c80, 0x0005, 0x0085, 0x1000, 0x1000);

    //after 1
    VTSS_MSLEEP(1);

    VTSS_I("Normal Operation being restored\n");
    //MiiWrite $port 9 $reg9
    VTSS_RC(PHY_WR_PAGE(vtss_state, port_no, VTSS_PHY_1000BASE_T_CONTROL, *reg9));

    //MiiWrite $port 0 $reg0;#This will take care of disabling Near-End loopback
    VTSS_RC(PHY_WR_PAGE(vtss_state, port_no, VTSS_PHY_MODE_CONTROL, *reg0));

    return VTSS_RC_OK;
}
vtss_rc vtss_phy_1588_oos_mitigation_steps_private(vtss_state_t *vtss_state,
                                                   const vtss_port_no_t port_no, const vtss_phy_ts_fifo_conf_t *fifo_conf)
{
    vtss_phy_port_state_t          *ps = &vtss_state->phy_state[port_no];
    vtss_phy_media_interface_t     media_type = ps->reset.media_if;
    vtss_port_speed_t              speed = ps->status.speed;
    vtss_phy_type_t                phy_id;
    vtss_rc                        rc = VTSS_RC_OK;
    BOOL ts_new_rev = TRUE;
    BOOL cu_mode;   /*If the bit is set it is copper interface, otherwise it is fiber interface */
    BOOL ams_mode = FALSE;
    BOOL macsec_enable = FALSE;
    BOOL cu_sfp = FALSE;
    u16 is_power_down = 0;

#if defined(VTSS_FEATURE_MACSEC)
    macsec_enable =  vtss_state->macsec_conf[port_no].glb.init.enable;
#endif

#ifdef VTSS_FIFO_SYNC_DEBUG
    struct timeval   tv;
    gettimeofday(&tv, NULL);
    VTSS_I("Time At the entry of Algoritm: Sec %ld, Usec  %ld\n", tv.tv_sec, tv.tv_usec);
#endif

    VTSS_I("Speed configured in the previous Run is %u\n", speed);

    VTSS_I("Media type selected is %u\n", media_type);


    if (vtss_state->phy_ts_port_conf[port_no].port_ts_init_done == FALSE) {
        VTSS_I("Port ts not initialized! exiting quietly port: %d", port_no);
        return VTSS_RC_OK;
    }
    VTSS_RC(vtss_phy_id_get_private(vtss_state, port_no, &phy_id));

    if (!((phy_id.part_number == VTSS_PHY_TYPE_8582) || (phy_id.part_number == VTSS_PHY_TYPE_8584) || (phy_id.part_number == VTSS_PHY_TYPE_8575))) {
        //OOS Algorithm is supported on only 1588 Supported SKU's
        //i.e PHY ID 8584, 8582, 8575
        return VTSS_RC_OK;
    }

    if (!(phy_id.revision >= 1)) {
        VTSS_I("Viper Rev A, OOS algorithm should not be executed\n");
        return VTSS_RC_OK;
    }

    //Check if PHY is in power down mode, in that case do not execute the algorithm.
    VTSS_RC(PHY_RD_PAGE(vtss_state, port_no, VTSS_PHY_MODE_CONTROL, &is_power_down));
    if (is_power_down & VTSS_F_PHY_MODE_CONTROL_POWER_DOWN) {
        VTSS_I("PHY is in Power Down mode, Algorithm not supported. Exiting Silently\n");
        return VTSS_RC_OK;
    }


    if ((vtss_phy_ts_algo_execute_check(vtss_state, port_no)) == FALSE) {
        VTSS_I("PTP is not configured on the port exiting quietly\n");
        return VTSS_RC_OK;
    }

    if (!fifo_conf->skip_rev_check) {
        vtss_phy_ts_version_check(vtss_state, port_no, &ts_new_rev);
        if (ts_new_rev == TRUE) {
            VTSS_I("Not running oos alog port %u\n", port_no);
            return VTSS_RC_OK;
        }
    }

    //Select mode of execution depending on the media_type configured on the device.
    switch (media_type) {
    case VTSS_PHY_MEDIA_IF_CU:
        cu_mode = TRUE;
        break;
    case VTSS_PHY_MEDIA_IF_SFP_PASSTHRU:
        cu_sfp = TRUE;
        cu_mode = TRUE;
        break;
    case VTSS_PHY_MEDIA_IF_FI_1000BX:
        cu_mode = FALSE;
        break;
    case VTSS_PHY_MEDIA_IF_FI_100FX:
        cu_mode = FALSE;
        break;
    case VTSS_PHY_MEDIA_IF_AMS_CU_PASSTHRU:
        cu_mode = TRUE;
        ams_mode = TRUE;
        break;
    case VTSS_PHY_MEDIA_IF_AMS_FI_PASSTHRU:
        cu_mode = FALSE;
        ams_mode = TRUE;
        break;
    case VTSS_PHY_MEDIA_IF_AMS_CU_1000BX:
        cu_mode = TRUE;
        ams_mode = TRUE;
        break;
    case VTSS_PHY_MEDIA_IF_AMS_FI_1000BX:
        cu_mode = FALSE;
        ams_mode = TRUE;
        break;
    case VTSS_PHY_MEDIA_IF_AMS_CU_100FX:
        cu_mode = TRUE;
        ams_mode = TRUE;
        break;
    case VTSS_PHY_MEDIA_IF_AMS_FI_100FX:
        cu_mode = FALSE;
        ams_mode = TRUE;
        break;
    default:
        VTSS_E("No Media Type selected\n");
        return VTSS_RC_OK;
        break;
    }

    if (ams_mode == TRUE) {
        cu_mode = TRUE;    //Need to run fiber patch in AMS cases
    }

    VTSS_I("Calling OOS Alog ams_mode : %s  cu_mode : %s port_no : %u\n", ams_mode ? "TRUE " : "FALSE ", cu_mode ? "TRUE " : "FALSE ", port_no);

    if (!macsec_enable) {
        rc = vtss_phy_1588_oos_mitigation_steps_execute(vtss_state, port_no, cu_mode, ams_mode, cu_sfp);
    } else {
        VTSS_I("MACsec is enabled, Current 1588 OOS Algorithm does not support MACsec modes");
    }

#ifdef VTSS_FIFO_SYNC_DEBUG
    gettimeofday(&tv, NULL);
    VTSS_I("Time At the exit of Algoritm: Sec %ld, Usec  %ld\n", tv.tv_sec, tv.tv_usec);
#endif
    return VTSS_RC_OK;
}
static vtss_rc vtss_phy_1588_oos_mitigation_steps_execute(vtss_state_t *vtss_state,
                                                          const vtss_port_no_t port_no,
                                                          BOOL copper,
                                                          BOOL ams_mode,
                                                          BOOL cu_sfp)
{
    vtss_rc rc = VTSS_RC_OK;
    vtss_phy_ts_overflow_info_t overflow_conf;
    vtss_phy_ts_pop_fifo_t pop_fifo;
    u16 reg0, reg9, extreg29, reg23;
    VTSS_I("Enter vtss_phy_1588_oos_mitigation_steps_execute port_no %u mode of operation %s\n", port_no, copper ? "Copper" : "Fiber");

    rc = vtss_phy_1588_oos_mitigation_save_cfg(vtss_state, port_no, &reg0, &reg9, &extreg29, &reg23);

    if (rc != VTSS_RC_OK) {
        VTSS_E("Not Able to save configuration, returning");
        return VTSS_RC_ERROR;
    }

    if (ams_mode) {
        rc = PHY_WR_MASKED_PAGE(vtss_state, port_no, VTSS_PHY_EXTENDED_PHY_CONTROL,
                                VTSS_F_PHY_EXTENDED_PHY_CONTROL_AMS_OVERRIDE(2),
                                VTSS_M_PHY_EXTENDED_PHY_CONTROL_AMS_OVERRIDE);
    }

    if (rc == VTSS_RC_OK) {
        rc = vtss_phy_1588_oos_mitigation_isolate_phy(vtss_state, port_no, copper);
    }

    if (rc == VTSS_RC_OK) {
        if (copper && !cu_sfp) {
            rc = vtss_phy_1588_oos_mitigation_check_frames_remote_local_good(vtss_state, port_no);
        } else if (cu_sfp) {
            rc = vtss_phy_1588_oos_mitigation_media_interface_link_status(vtss_state, port_no);
        }

    }

    //TeslaCsrWrite $port $proc_id 0x00 0x42;#Disable 1588 bypass
    if (rc == VTSS_RC_OK) {
        rc = vtss_phy_ts_bypass_set(vtss_state, port_no, FALSE, TRUE);
    }

    //after 1
    VTSS_MSLEEP(1);
    if (rc == VTSS_RC_OK) {
        rc = vtss_phy_intr_status(vtss_state, port_no, &overflow_conf);
    }

    VTSS_I("Value of register 0x2D INGR_INT_STATUS is 0x%x\n", overflow_conf.ingr_intr_status);
    VTSS_I("Value of register 0x4D EGR_INT_STATUS is 0x%x\n", overflow_conf.egr_intr_status);

    if (rc == VTSS_RC_OK) {
        rc = vtss_phy_sw_pop_fifo_get(vtss_state, port_no, &pop_fifo);
    }

    VTSS_I("Value of register 0x9F INGR_SW_POP_FIFO is 0x%x\n", pop_fifo.egr_pop_fifo);
    VTSS_I("Value of register 0xC0 EGR_SW_POP_FIFO is 0x%x\n", pop_fifo.ingr_pop_fifo);

    if (rc == VTSS_RC_OK) {
        rc = vtss_phy_ts_bypass_set(vtss_state, port_no, TRUE, TRUE);
    }

    //after 1
    VTSS_MSLEEP(1);

    if (rc == VTSS_RC_OK) {
        rc = vtss_phy_1588_oos_mitigation_soft_reset_pop_fifo(vtss_state, port_no);
    }

    //after 1
    VTSS_MSLEEP(1);

    if (rc == VTSS_RC_OK) {
        rc = vtss_phy_1588_oos_mitigation_epg_transmit_frames(vtss_state, port_no, copper);
    }

    if (rc == VTSS_RC_OK) {
        rc = vtss_phy_ts_bypass_set(vtss_state, port_no, TRUE, TRUE);
    }

    if (rc == VTSS_RC_OK) {
        rc = vtss_phy_1588_oos_mitigation_soft_reset_pop_fifo(vtss_state, port_no);
    }

    VTSS_MSLEEP(1);

    if (rc == VTSS_RC_OK) {
        rc = vtss_phy_1588_oos_mitigation_execute_core_patch_execute(vtss_state, port_no);
    }

    //After 1
    VTSS_MSLEEP(1);

    //Enable 1588 Bypass

    if (rc == VTSS_RC_OK) {
        rc = vtss_phy_ts_bypass_set(vtss_state, port_no, TRUE, TRUE);
    }

    if (rc == VTSS_RC_OK) {
        rc = vtss_phy_1588_oos_mitigation_restore_setup(vtss_state, port_no, &reg0, &reg9, &extreg29, &reg23);
    }

    VTSS_MSLEEP(1);

    if (rc == VTSS_RC_OK) {
        rc = vtss_phy_ts_bypass_set(vtss_state, port_no, FALSE, TRUE);
    }

    if (rc != VTSS_RC_OK) {
        VTSS_E("Failure in the execution of Algorithm, Restoring PHY operation");
        rc = vtss_phy_ts_bypass_set(vtss_state, port_no, FALSE, TRUE);
        rc = vtss_phy_1588_oos_mitigation_restore_setup(vtss_state, port_no, &reg0, &reg9, &extreg29, &reg23);
        return VTSS_RC_ERROR;
    }
    VTSS_I("*******************************************************************************\n");
    VTSS_I("********************ALGORITHM EXECUTION COMPLETED******************************\n");
    VTSS_I("*******************************************************************************\n");

    return VTSS_RC_OK;
}
#else
vtss_rc vtss_phy_ts_viper_fifo_reset(const vtss_inst_t               inst,
                                     const vtss_port_no_t            port_no,
                                     const vtss_phy_ts_fifo_conf_t *fifo_conf)
{
    return VTSS_RC_OK;
}


vtss_rc vtss_phy_1588_oos_mitigation_steps_private(vtss_state_t *vtss_state,
                                                   const vtss_port_no_t port_no,
                                                   const vtss_phy_ts_fifo_conf_t *fifo_conf)
{
    return VTSS_RC_OK;
}
#endif
vtss_rc vtss_phy_ts_tesla_tsp_fifo_sync(const vtss_inst_t               inst,
                                        const vtss_port_no_t            port_no,
                                        const vtss_debug_printf_t       pr,
                                        const vtss_phy_ts_fifo_conf_t   *fifo_conf,
                                        BOOL                            *OOS)
{
    vtss_rc             rc = VTSS_RC_OK;
    vtss_state_t        *vtss_state;

    VTSS_ENTER();

    if ((rc = vtss_inst_port_no_check(inst,  &vtss_state, port_no)) == VTSS_RC_OK) {
        rc = vtss_phy_ts_tesla_tsp_fifo_sync_private(vtss_state, port_no, pr, fifo_conf, OOS);
    }

    VTSS_EXIT();
    return rc;
}
static vtss_rc vtss_phy_1g_ts_fifo_sync_private(vtss_state_t               *vtss_state,
                                                const vtss_port_no_t       port_no,
                                                const vtss_debug_printf_t  pr,
                                                const vtss_phy_ts_fifo_conf_t *fifo_conf,
                                                BOOL                          *OOS)
{
    vtss_rc        rc = VTSS_RC_OK;
    vtss_phy_type_t         phy_id;

    VTSS_RC(vtss_phy_id_get_private(vtss_state, port_no, &phy_id));
    VTSS_I("Enter vtss_phy_1g_ts_fifo_sync_private port_no %u PHY_ID 0x%x\n", port_no, phy_id.part_number);

    if ((phy_id.part_number == VTSS_PHY_TYPE_8582) || (phy_id.part_number == VTSS_PHY_TYPE_8584) || (phy_id.part_number == VTSS_PHY_TYPE_8575)) {

        rc = vtss_phy_1588_oos_mitigation_steps_private(vtss_state, port_no, fifo_conf);

    } else if (( phy_id.part_number == VTSS_PHY_TYPE_8574 ) || ( phy_id.part_number == VTSS_PHY_TYPE_8572)) {

        rc = vtss_phy_ts_tesla_tsp_fifo_sync_private(vtss_state, port_no, pr, fifo_conf, OOS);

    }
    return rc;
}
vtss_rc vtss_phy_1g_ts_fifo_sync(const vtss_inst_t  inst,
                                 const vtss_port_no_t port_no,
                                 const vtss_debug_printf_t pr,
                                 const vtss_phy_ts_fifo_conf_t *fifo_conf,
                                 BOOL *OOS)
{
    vtss_rc        rc = VTSS_RC_OK;
    vtss_state_t   *vtss_state;

    VTSS_ENTER();

    if ((rc = vtss_inst_port_no_check(inst,  &vtss_state, port_no)) == VTSS_RC_OK) {
        rc = vtss_phy_1g_ts_fifo_sync_private(vtss_state, port_no, pr, fifo_conf, OOS);
    }

    VTSS_EXIT();
    return rc;
}




#endif /* #if defined(VTSS_CHIP_CU_PHY) */
#endif /* VTSS_FEATURE_PHY_TIMESTAMP*/
