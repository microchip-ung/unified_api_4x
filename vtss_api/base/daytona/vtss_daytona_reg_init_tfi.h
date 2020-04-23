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
#ifndef _VTSS_DAYTONA_REG_INIT_TFI_H
#define _VTSS_DAYTONA_REG_INIT_TFI_H

// Settings for mode SDR

#define  VTSS_TFI_5_GLOBAL_GLOBAL_CONTROL_RTFI_EN_SDR                                               VTSS_F_TFI_5_GLOBAL_GLOBAL_CONTROL_RTFI_EN(0x0)
#define  VTSS_TFI_5_GLOBAL_GLOBAL_CONTROL_LOOP_EN_SDR                                               VTSS_F_TFI_5_GLOBAL_GLOBAL_CONTROL_LOOP_EN(0x0)
#define  VTSS_TFI_5_RX_RX_CONTROL_SYNC_SEL_SDR                                                      VTSS_F_TFI_5_RX_RX_CONTROL_SYNC_SEL(0x0)


// Settings for mode DDR

#define  VTSS_TFI_5_GLOBAL_GLOBAL_CONTROL_RTFI_EN_DDR                                               VTSS_F_TFI_5_GLOBAL_GLOBAL_CONTROL_RTFI_EN(0x1)
#define  VTSS_TFI_5_GLOBAL_GLOBAL_CONTROL_LOOP_EN_DDR                                               VTSS_F_TFI_5_GLOBAL_GLOBAL_CONTROL_LOOP_EN(0x0)
#define  VTSS_TFI_5_RX_RX_CONTROL_SYNC_SEL_DDR                                                      VTSS_F_TFI_5_RX_RX_CONTROL_SYNC_SEL(0x0)


// Settings for mode UNUSED

#define  VTSS_TFI_5_GLOBAL_GLOBAL_CONTROL_RTFI_EN_UNUSED                                            VTSS_F_TFI_5_GLOBAL_GLOBAL_CONTROL_RTFI_EN(0x0)
#define  VTSS_TFI_5_GLOBAL_GLOBAL_CONTROL_LOOP_EN_UNUSED                                            VTSS_F_TFI_5_GLOBAL_GLOBAL_CONTROL_LOOP_EN(0x0)
#define  VTSS_TFI_5_RX_RX_CONTROL_SYNC_SEL_UNUSED                                                   VTSS_F_TFI_5_RX_RX_CONTROL_SYNC_SEL(0x0)




typedef enum {
    BM_TFI_SDR,
    BM_TFI_DDR,
    BM_TFI_UNUSED,
    BM_TFI_LAST
} block_tfi_mode_t;

#endif /* _VTSS_DAYTONA_REG_INIT_TFI_H */
