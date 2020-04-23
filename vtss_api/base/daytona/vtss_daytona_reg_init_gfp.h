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
