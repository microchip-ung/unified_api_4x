/*


 Copyright (c) 2002-2016 Microsemi Corporation "Microsemi". All Rights Reserved.

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
// register structures for mode SDR

#define TFI_TBL_SIZE   2
 static static_cfg_t tfi_sdr_tbl[2] = { {DAYTONA_BLOCK_TFI_5, VTSS_TFI_5_GLOBAL_GLOBAL_CONTROL(0), (VTSS_TFI_5_GLOBAL_GLOBAL_CONTROL_RTFI_EN_SDR | VTSS_TFI_5_GLOBAL_GLOBAL_CONTROL_LOOP_EN_SDR), (VTSS_M_TFI_5_GLOBAL_GLOBAL_CONTROL_RTFI_EN | VTSS_M_TFI_5_GLOBAL_GLOBAL_CONTROL_LOOP_EN)},
				{DAYTONA_BLOCK_TFI_5, VTSS_TFI_5_RX_RX_CONTROL(0), (VTSS_TFI_5_RX_RX_CONTROL_SYNC_SEL_SDR), (VTSS_M_TFI_5_RX_RX_CONTROL_SYNC_SEL)} };


// register structures for mode DDR

static static_cfg_t tfi_ddr_tbl[2] = { {DAYTONA_BLOCK_TFI_5, VTSS_TFI_5_GLOBAL_GLOBAL_CONTROL(0), (VTSS_TFI_5_GLOBAL_GLOBAL_CONTROL_RTFI_EN_DDR | VTSS_TFI_5_GLOBAL_GLOBAL_CONTROL_LOOP_EN_DDR), (VTSS_M_TFI_5_GLOBAL_GLOBAL_CONTROL_RTFI_EN | VTSS_M_TFI_5_GLOBAL_GLOBAL_CONTROL_LOOP_EN)},
				{DAYTONA_BLOCK_TFI_5, VTSS_TFI_5_RX_RX_CONTROL(0), (VTSS_TFI_5_RX_RX_CONTROL_SYNC_SEL_DDR), (VTSS_M_TFI_5_RX_RX_CONTROL_SYNC_SEL)} };


// register structures for mode UNUSED

static static_cfg_t tfi_unused_tbl[2] = { {DAYTONA_BLOCK_TFI_5, VTSS_TFI_5_GLOBAL_GLOBAL_CONTROL(0), (VTSS_TFI_5_GLOBAL_GLOBAL_CONTROL_RTFI_EN_UNUSED | VTSS_TFI_5_GLOBAL_GLOBAL_CONTROL_LOOP_EN_UNUSED), (VTSS_M_TFI_5_GLOBAL_GLOBAL_CONTROL_RTFI_EN | VTSS_M_TFI_5_GLOBAL_GLOBAL_CONTROL_LOOP_EN)},
				{DAYTONA_BLOCK_TFI_5, VTSS_TFI_5_RX_RX_CONTROL(0), (VTSS_TFI_5_RX_RX_CONTROL_SYNC_SEL_UNUSED), (VTSS_M_TFI_5_RX_RX_CONTROL_SYNC_SEL)} };


static const static_cfg_t *tfi_config_table[BM_TFI_LAST] = {
    tfi_sdr_tbl,
    tfi_ddr_tbl,
    tfi_unused_tbl,
};

