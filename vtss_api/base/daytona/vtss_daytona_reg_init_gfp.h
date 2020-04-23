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
#ifndef _VTSS_DAYTONA_REG_INIT_GFP_H
#define _VTSS_DAYTONA_REG_INIT_GFP_H

// Settings for mode GFPF

#define  VTSS_GFP_GFP_BASE_GFP_CONTROL_CLK_ENA_GFPF                                                 VTSS_F_GFP_GFP_BASE_GFP_CONTROL_CLK_ENA(0x1)
#define  VTSS_GFP_GFP_BASE_GFP_CONTROL_CLIENT_MODE_SEL_GFPF                                         VTSS_F_GFP_GFP_BASE_GFP_CONTROL_CLIENT_MODE_SEL(0x2)
#define  VTSS_GFP_GFPM_TX_GLOBAL_TX_GLOBAL_CONTROL_MAPPER_MODE_GFPF                                 VTSS_F_GFP_GFPM_TX_GLOBAL_TX_GLOBAL_CONTROL_MAPPER_MODE(0x2)
#define  VTSS_GFP_GFPM_RX_GLOBAL_RX_GLOBAL_CONTROL_MAPPER_MODE_GFPF                                 VTSS_F_GFP_GFPM_RX_GLOBAL_RX_GLOBAL_CONTROL_MAPPER_MODE(0x2)
#define  VTSS_GFP_GFPM_TX_GLOBAL_TX_GLOBAL_CONTROL_FIFO_RESET_GFPF                                  VTSS_F_GFP_GFPM_TX_GLOBAL_TX_GLOBAL_CONTROL_FIFO_RESET(0x1)
#define  VTSS_GFP_GFPM_RX_GLOBAL_RX_GLOBAL_CONTROL_FIFO_RESET_GFPF                                  VTSS_F_GFP_GFPM_RX_GLOBAL_RX_GLOBAL_CONTROL_FIFO_RESET(0x1)


// Settings for mode PPOS

#define  VTSS_GFP_GFP_BASE_GFP_CONTROL_CLK_ENA_PPOS                                                 VTSS_F_GFP_GFP_BASE_GFP_CONTROL_CLK_ENA(0x1)
#define  VTSS_GFP_GFP_BASE_GFP_CONTROL_CLIENT_MODE_SEL_PPOS                                         VTSS_F_GFP_GFP_BASE_GFP_CONTROL_CLIENT_MODE_SEL(0x3)
#define  VTSS_GFP_GFPM_TX_GLOBAL_TX_GLOBAL_CONTROL_MAPPER_MODE_PPOS                                 VTSS_F_GFP_GFPM_TX_GLOBAL_TX_GLOBAL_CONTROL_MAPPER_MODE(0x3)
#define  VTSS_GFP_GFPM_RX_GLOBAL_RX_GLOBAL_CONTROL_MAPPER_MODE_PPOS                                 VTSS_F_GFP_GFPM_RX_GLOBAL_RX_GLOBAL_CONTROL_MAPPER_MODE(0x3)
#define  VTSS_GFP_GFPM_TX_GLOBAL_TX_GLOBAL_CONTROL_FIFO_RESET_PPOS                                  VTSS_F_GFP_GFPM_TX_GLOBAL_TX_GLOBAL_CONTROL_FIFO_RESET(0x1)
#define  VTSS_GFP_GFPM_RX_GLOBAL_RX_GLOBAL_CONTROL_FIFO_RESET_PPOS                                  VTSS_F_GFP_GFPM_RX_GLOBAL_RX_GLOBAL_CONTROL_FIFO_RESET(0x1)


// Settings for mode GFPT

#define  VTSS_GFP_GFP_BASE_GFP_CONTROL_CLK_ENA_GFPT                                                 VTSS_F_GFP_GFP_BASE_GFP_CONTROL_CLK_ENA(0x1)
#define  VTSS_GFP_GFP_BASE_GFP_CONTROL_CLIENT_MODE_SEL_GFPT                                         VTSS_F_GFP_GFP_BASE_GFP_CONTROL_CLIENT_MODE_SEL(0x0)
#define  VTSS_GFP_GFPM_TX_GLOBAL_TX_GLOBAL_CONTROL_MAPPER_MODE_GFPT                                 VTSS_F_GFP_GFPM_TX_GLOBAL_TX_GLOBAL_CONTROL_MAPPER_MODE(0x0)
#define  VTSS_GFP_GFPM_RX_GLOBAL_RX_GLOBAL_CONTROL_MAPPER_MODE_GFPT                                 VTSS_F_GFP_GFPM_RX_GLOBAL_RX_GLOBAL_CONTROL_MAPPER_MODE(0x0)
#define  VTSS_GFP_GFPM_TX_GLOBAL_TX_GLOBAL_CONTROL_FIFO_RESET_GFPT                                  VTSS_F_GFP_GFPM_TX_GLOBAL_TX_GLOBAL_CONTROL_FIFO_RESET(0x1)
#define  VTSS_GFP_GFPM_RX_GLOBAL_RX_GLOBAL_CONTROL_FIFO_RESET_GFPT                                  VTSS_F_GFP_GFPM_RX_GLOBAL_RX_GLOBAL_CONTROL_FIFO_RESET(0x1)


// Settings for mode BYP

#define  VTSS_GFP_GFP_BASE_GFP_CONTROL_CLK_ENA_BYP                                                  VTSS_F_GFP_GFP_BASE_GFP_CONTROL_CLK_ENA(0x0)
#define  VTSS_GFP_GFP_BASE_GFP_CONTROL_CLIENT_MODE_SEL_BYP                                          VTSS_F_GFP_GFP_BASE_GFP_CONTROL_CLIENT_MODE_SEL(0x0)
#define  VTSS_GFP_GFPM_TX_GLOBAL_TX_GLOBAL_CONTROL_MAPPER_MODE_BYP                                  VTSS_F_GFP_GFPM_TX_GLOBAL_TX_GLOBAL_CONTROL_MAPPER_MODE(0x0)
#define  VTSS_GFP_GFPM_RX_GLOBAL_RX_GLOBAL_CONTROL_MAPPER_MODE_BYP                                  VTSS_F_GFP_GFPM_RX_GLOBAL_RX_GLOBAL_CONTROL_MAPPER_MODE(0x0)
#define  VTSS_GFP_GFPM_TX_GLOBAL_TX_GLOBAL_CONTROL_FIFO_RESET_BYP                                   VTSS_F_GFP_GFPM_TX_GLOBAL_TX_GLOBAL_CONTROL_FIFO_RESET(0x1)
#define  VTSS_GFP_GFPM_RX_GLOBAL_RX_GLOBAL_CONTROL_FIFO_RESET_BYP                                   VTSS_F_GFP_GFPM_RX_GLOBAL_RX_GLOBAL_CONTROL_FIFO_RESET(0x1)




typedef enum {
    BM_GFP_GFPF,
    BM_GFP_PPOS,
    BM_GFP_GFPT,
    BM_GFP_BYP,
    BM_GFP_LAST
} block_gfp_mode_t;

#endif /* _VTSS_DAYTONA_REG_INIT_GFP_H */
