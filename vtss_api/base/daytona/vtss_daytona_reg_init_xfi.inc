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
// register structures for mode WORK

#define XFI_TBL_SIZE   1
 static static_cfg_t xfi_work_tbl[1] = { {DAYTONA_BLOCK_XFI, VTSS_XFI_XFI_CONTROL_XFI_MODE(0), (VTSS_XFI_XFI_CONTROL_XFI_MODE_TX_AUTORESYNC_WORK), (VTSS_M_XFI_XFI_CONTROL_XFI_MODE_TX_AUTORESYNC)} };


// register structures for mode PROT

static static_cfg_t xfi_prot_tbl[1] = { {DAYTONA_BLOCK_XFI, VTSS_XFI_XFI_CONTROL_XFI_MODE(0), (VTSS_XFI_XFI_CONTROL_XFI_MODE_TX_AUTORESYNC_PROT), (VTSS_M_XFI_XFI_CONTROL_XFI_MODE_TX_AUTORESYNC)} };


// register structures for mode W_P

static static_cfg_t xfi_w_p_tbl[1] = { {DAYTONA_BLOCK_XFI, VTSS_XFI_XFI_CONTROL_XFI_MODE(0), (VTSS_XFI_XFI_CONTROL_XFI_MODE_TX_AUTORESYNC_W_P), (VTSS_M_XFI_XFI_CONTROL_XFI_MODE_TX_AUTORESYNC)} };


// register structures for mode UNUSED

static static_cfg_t xfi_unused_tbl[1] = { {DAYTONA_BLOCK_XFI, VTSS_XFI_XFI_CONTROL_XFI_MODE(0), (VTSS_XFI_XFI_CONTROL_XFI_MODE_TX_AUTORESYNC_UNUSED), (VTSS_M_XFI_XFI_CONTROL_XFI_MODE_TX_AUTORESYNC)} };


static const static_cfg_t *xfi_config_table[BM_XFI_LAST] = {
    xfi_work_tbl,
    xfi_prot_tbl,
    xfi_w_p_tbl,
    xfi_unused_tbl,
};

