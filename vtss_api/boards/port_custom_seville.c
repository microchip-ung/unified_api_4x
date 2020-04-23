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

// Avoid "*.h not used in module port_custom_seville.c"
/*lint --e{766} */
#include "port_custom_api.h"
#include "board_probe.h"
#if defined(VTSS_SW_OPTION_PHY)
#include "phy_api.h" // For PHY_INST
#else
#define PHY_INST NULL
#endif

#if defined(VTSS_ARCH_SEVILLE)

#ifdef __cplusplus
extern "C" {
#endif

#include "../base/serval/vtss_serval_cil.h"

#ifdef __cplusplus
}
#endif

static port_custom_entry_t seville_port_table[VTSS_PORT_ARRAY_SIZE];

static vtss_miim_read_t seville_miim_read = NULL;

static void seville_init(void)
{
    vtss_state_t     vtss_state;
    if (vtss_init_conf_get(NULL, &vtss_state.init_conf) == VTSS_RC_OK) {
        u32 i;
        vtss_rc __rc;
        /* Need to change some registers to use /dev/npi */
        for (i = 0; i < VTSS_PACKET_RX_GRP_CNT; i++) {
            __rc = vtss_srvl_wrm(&vtss_state,
                                 VTSS_DEVCPU_QS_XTR_XTR_GRP_CFG(i),
                                 VTSS_F_DEVCPU_QS_XTR_XTR_GRP_CFG_STATUS_WORD_POS,
                                 VTSS_F_DEVCPU_QS_XTR_XTR_GRP_CFG_STATUS_WORD_POS);
            if (__rc != VTSS_RC_OK)
                return;

            __rc = vtss_srvl_wrm(&vtss_state,
                                 VTSS_DEVCPU_QS_XTR_XTR_GRP_CFG(i),
                                 VTSS_F_DEVCPU_QS_XTR_XTR_GRP_CFG_BYTE_SWAP,
                                 VTSS_F_DEVCPU_QS_XTR_XTR_GRP_CFG_BYTE_SWAP);
            if (__rc != VTSS_RC_OK)
                return;
        }
        for (i = 0; i < VTSS_PACKET_TX_GRP_CNT; i++) {

            __rc = vtss_srvl_wrm(&vtss_state,
                                 VTSS_DEVCPU_QS_INJ_INJ_GRP_CFG(i),
                                 VTSS_F_DEVCPU_QS_INJ_INJ_GRP_CFG_BYTE_SWAP,
                                 VTSS_F_DEVCPU_QS_INJ_INJ_GRP_CFG_BYTE_SWAP);
            if (__rc != VTSS_RC_OK)
                return;
      }
    }
}

static vtss_rc seville_reset(void)
{
    return VTSS_RC_OK;
}

static vtss_rc seville_post_reset(void)
{
    return VTSS_RC_OK;
}

static void seville_pre_reset(void)
{
}

static void seville_port_conf(vtss_port_no_t port_no, 
                             port_custom_conf_t *port_conf, 
                             vtss_port_status_t *port_status)
{
}

static vtss_rc phy_probe(vtss_port_no_t port_no)
{
    vtss_rc rc = VTSS_RC_ERROR;
    u16 id;

    if (seville_miim_read != NULL) {
        rc = seville_miim_read(NULL, port_no, 1, &id);
        if (id == 0xFFFF) {
            rc = VTSS_RC_ERROR;
        }
    }

    return rc;
}

BOOL vtss_board_probe_seville(vtss_board_t *board, 
                              vtss_board_info_t *board_info)
{
    vtss_init_conf_t conf;
    vtss_port_no_t   port_no;
    
    memset(board, 0, sizeof(*board));

    //
    // Setup Board info
    //
    board_info->board_type = board->type = VTSS_BOARD_SEVILLE_T1040QDS;
    board->name = "T1040QDS";
    board->custom_port_table = seville_port_table;
    board->init = seville_init;
    board->reset = seville_reset;
    board->pre_reset = seville_pre_reset;
    board->post_reset = seville_post_reset;
    board->port_conf = seville_port_conf;

    if (vtss_init_conf_get(NULL, &conf) == VTSS_RC_OK) {
        seville_miim_read = conf.miim_read;
    }
    for (port_no = 0; port_no < VTSS_PORTS; port_no++) {
        port_custom_entry_t *entry = &seville_port_table[port_no];
        vtss_port_map_t *map = &entry->map;
        map->miim_controller = VTSS_MIIM_CONTROLLER_NONE;
        if (port_no < 8) {
            if (phy_probe(port_no) == VTSS_RC_OK) {
                entry->cap = PORT_CAP_TRI_SPEED_COPPER;
                /* Copper ports, chip port 0-7  */
                map->chip_port = port_no;
                entry->mac_if = VTSS_PORT_INTERFACE_QSGMII;
            } else {
                entry->cap = PORT_CAP_NONE;
                /* Copper ports, chip port 0-7  */
                map->chip_port = port_no;
                entry->mac_if = VTSS_PORT_INTERFACE_NO_CONNECTION;
            }
        } else {
            /* CPU ports, chip port 8-9 */
            entry->cap = PORT_CAP_2_5G_FDX;
            map->chip_port = (8 + (port_no & 1));
            entry->mac_if = VTSS_PORT_INTERFACE_INTERNAL;
        }
    }

    if (VTSS_PORTS >= 10) {
        BOOL members[VTSS_PORT_ARRAY_SIZE];

        /* Aggregate the 2 2.5G CPU ports */
        memset(members, 0, sizeof(members));
        members[8] = members[9] = TRUE;
        vtss_aggr_port_members_set(NULL, VTSS_AGGR_NO_START, members);

        /* Reserve the two aggregated CPU ports */
        //        board_info->port_count -= 2;
    }

    return TRUE;
}

#endif /* defined(VTSS_ARCH_SEVILLE) */

/****************************************************************************/
/*                                                                          */
/*  End of file.                                                            */
/*                                                                          */
/****************************************************************************/


