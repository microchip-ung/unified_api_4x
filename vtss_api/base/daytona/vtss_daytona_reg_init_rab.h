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
#ifndef _VTSS_DAYTONA_REG_INIT_RAB_H
#define _VTSS_DAYTONA_REG_INIT_RAB_H

// Settings for mode MAC

#define  VTSS_RAB_RAB_BASE_RAB_CONTROL_RX_CLK_ENA_MAC                                               VTSS_F_RAB_RAB_BASE_RAB_CONTROL_RX_CLK_ENA(0x1)
#define  VTSS_RAB_RAB_BASE_RAB_CONTROL_RX_BYPASS_SELECT_MAC                                         VTSS_F_RAB_RAB_BASE_RAB_CONTROL_RX_BYPASS_SELECT(0x0)
#define  VTSS_RAB_RAB_BASE_RAB_CONTROL_TX_CLK_ENA_MAC                                               VTSS_F_RAB_RAB_BASE_RAB_CONTROL_TX_CLK_ENA(0x1)
#define  VTSS_RAB_RAB_BASE_RAB_CONTROL_TX_BYPASS_SELECT_MAC                                         VTSS_F_RAB_RAB_BASE_RAB_CONTROL_TX_BYPASS_SELECT(0x0)
#define  VTSS_RAB_RX_FIFO_RX_FIFO_CONTROL_RX_FIFO_INPUT_SELECT_MAC                                  VTSS_F_RAB_RX_FIFO_RX_FIFO_CONTROL_RX_FIFO_INPUT_SELECT(0x0)
#define  VTSS_RAB_RX_FIFO_RX_FIFO_CONTROL_RX_FIFO_OUTPUT_SELECT_MAC                                 VTSS_F_RAB_RX_FIFO_RX_FIFO_CONTROL_RX_FIFO_OUTPUT_SELECT(0x0)
#define  VTSS_RAB_RX_FIFO_RX_FIFO_CONTROL_RX_FIFO_CUT_THRU_MAC                                      VTSS_F_RAB_RX_FIFO_RX_FIFO_CONTROL_RX_FIFO_CUT_THRU(0x1)
#define  VTSS_RAB_TX_FIFO_TX_FIFO_CONTROL_TX_FIFO_INPUT_SELECT_MAC                                  VTSS_F_RAB_TX_FIFO_TX_FIFO_CONTROL_TX_FIFO_INPUT_SELECT(0x0)
#define  VTSS_RAB_TX_FIFO_TX_FIFO_CONTROL_TX_FIFO_OUTPUT_SELECT_MAC                                 VTSS_F_RAB_TX_FIFO_TX_FIFO_CONTROL_TX_FIFO_OUTPUT_SELECT(0x0)
#define  VTSS_RAB_TX_FIFO_TX_FIFO_CONTROL_TX_FIFO_CUT_THRU_MAC                                      VTSS_F_RAB_TX_FIFO_TX_FIFO_CONTROL_TX_FIFO_CUT_THRU(0x1)
#define  VTSS_RAB_RX_FIFO_RX_FIFO_FLUSH_RX_FIFO_FLUSH_MAC                                           VTSS_F_RAB_RX_FIFO_RX_FIFO_FLUSH_RX_FIFO_FLUSH(0x1)
#define  VTSS_RAB_TX_FIFO_TX_FIFO_FLUSH_TX_FIFO_FLUSH_MAC                                           VTSS_F_RAB_TX_FIFO_TX_FIFO_FLUSH_TX_FIFO_FLUSH(0x1)
#define  VTSS_RAB_RX_EXTRACT_RX_EXTRACT_STATUS_RX_EXTRACT_FLUSH_MAC                                 VTSS_F_RAB_RX_EXTRACT_RX_EXTRACT_STATUS_RX_EXTRACT_FLUSH(0x1)
#define  VTSS_RAB_TX_EXTRACT_TX_EXTRACT_STATUS_TX_EXTRACT_FLUSH_MAC                                 VTSS_F_RAB_TX_EXTRACT_TX_EXTRACT_STATUS_TX_EXTRACT_FLUSH(0x1)
#define  VTSS_RAB_RX_FIFO_RX_FIFO_ADAPT_THRESH_RX_FIFO_ADAPT_THRESH_MAC                             VTSS_F_RAB_RX_FIFO_RX_FIFO_ADAPT_THRESH_RX_FIFO_ADAPT_THRESH(0x0)
#define  VTSS_RAB_TX_FIFO_TX_FIFO_ADAPT_THRESH_TX_FIFO_ADAPT_THRESH_MAC                             VTSS_F_RAB_TX_FIFO_TX_FIFO_ADAPT_THRESH_TX_FIFO_ADAPT_THRESH(0x0)
#define  VTSS_RAB_TX_FIFO_TX_FIFO_LOW_THRESH_TX_FIFO_LOW_THRESH_MAC                                 VTSS_F_RAB_TX_FIFO_TX_FIFO_LOW_THRESH_TX_FIFO_LOW_THRESH(0x500)
#define  VTSS_RAB_TX_FIFO_TX_FIFO_HIGH_THRESH_TX_FIFO_HIGH_THRESH_MAC                               VTSS_F_RAB_TX_FIFO_TX_FIFO_HIGH_THRESH_TX_FIFO_HIGH_THRESH(0xfff)


// Settings for mode MAC_FC_EN

#define  VTSS_RAB_RAB_BASE_RAB_CONTROL_RX_CLK_ENA_MAC_FC_EN                                         VTSS_F_RAB_RAB_BASE_RAB_CONTROL_RX_CLK_ENA(0x1)
#define  VTSS_RAB_RAB_BASE_RAB_CONTROL_RX_BYPASS_SELECT_MAC_FC_EN                                   VTSS_F_RAB_RAB_BASE_RAB_CONTROL_RX_BYPASS_SELECT(0x0)
#define  VTSS_RAB_RAB_BASE_RAB_CONTROL_TX_CLK_ENA_MAC_FC_EN                                         VTSS_F_RAB_RAB_BASE_RAB_CONTROL_TX_CLK_ENA(0x1)
#define  VTSS_RAB_RAB_BASE_RAB_CONTROL_TX_BYPASS_SELECT_MAC_FC_EN                                   VTSS_F_RAB_RAB_BASE_RAB_CONTROL_TX_BYPASS_SELECT(0x0)
#define  VTSS_RAB_RX_FIFO_RX_FIFO_CONTROL_RX_FIFO_INPUT_SELECT_MAC_FC_EN                            VTSS_F_RAB_RX_FIFO_RX_FIFO_CONTROL_RX_FIFO_INPUT_SELECT(0x0)
#define  VTSS_RAB_RX_FIFO_RX_FIFO_CONTROL_RX_FIFO_OUTPUT_SELECT_MAC_FC_EN                           VTSS_F_RAB_RX_FIFO_RX_FIFO_CONTROL_RX_FIFO_OUTPUT_SELECT(0x0)
#define  VTSS_RAB_RX_FIFO_RX_FIFO_CONTROL_RX_FIFO_CUT_THRU_MAC_FC_EN                                VTSS_F_RAB_RX_FIFO_RX_FIFO_CONTROL_RX_FIFO_CUT_THRU(0x1)
#define  VTSS_RAB_TX_FIFO_TX_FIFO_CONTROL_TX_FIFO_INPUT_SELECT_MAC_FC_EN                            VTSS_F_RAB_TX_FIFO_TX_FIFO_CONTROL_TX_FIFO_INPUT_SELECT(0x0)
#define  VTSS_RAB_TX_FIFO_TX_FIFO_CONTROL_TX_FIFO_OUTPUT_SELECT_MAC_FC_EN                           VTSS_F_RAB_TX_FIFO_TX_FIFO_CONTROL_TX_FIFO_OUTPUT_SELECT(0x0)
#define  VTSS_RAB_TX_FIFO_TX_FIFO_CONTROL_TX_FIFO_CUT_THRU_MAC_FC_EN                                VTSS_F_RAB_TX_FIFO_TX_FIFO_CONTROL_TX_FIFO_CUT_THRU(0x1)
#define  VTSS_RAB_RX_FIFO_RX_FIFO_FLUSH_RX_FIFO_FLUSH_MAC_FC_EN                                     VTSS_F_RAB_RX_FIFO_RX_FIFO_FLUSH_RX_FIFO_FLUSH(0x1)
#define  VTSS_RAB_TX_FIFO_TX_FIFO_FLUSH_TX_FIFO_FLUSH_MAC_FC_EN                                     VTSS_F_RAB_TX_FIFO_TX_FIFO_FLUSH_TX_FIFO_FLUSH(0x1)
#define  VTSS_RAB_RX_EXTRACT_RX_EXTRACT_STATUS_RX_EXTRACT_FLUSH_MAC_FC_EN                           VTSS_F_RAB_RX_EXTRACT_RX_EXTRACT_STATUS_RX_EXTRACT_FLUSH(0x1)
#define  VTSS_RAB_TX_EXTRACT_TX_EXTRACT_STATUS_TX_EXTRACT_FLUSH_MAC_FC_EN                           VTSS_F_RAB_TX_EXTRACT_TX_EXTRACT_STATUS_TX_EXTRACT_FLUSH(0x1)
#define  VTSS_RAB_RX_FIFO_RX_FIFO_ADAPT_THRESH_RX_FIFO_ADAPT_THRESH_MAC_FC_EN                       VTSS_F_RAB_RX_FIFO_RX_FIFO_ADAPT_THRESH_RX_FIFO_ADAPT_THRESH(0x0)
#define  VTSS_RAB_TX_FIFO_TX_FIFO_ADAPT_THRESH_TX_FIFO_ADAPT_THRESH_MAC_FC_EN                       VTSS_F_RAB_TX_FIFO_TX_FIFO_ADAPT_THRESH_TX_FIFO_ADAPT_THRESH(0xfff)
#define  VTSS_RAB_TX_FIFO_TX_FIFO_LOW_THRESH_TX_FIFO_LOW_THRESH_MAC_FC_EN                           VTSS_F_RAB_TX_FIFO_TX_FIFO_LOW_THRESH_TX_FIFO_LOW_THRESH(0x820)
#define  VTSS_RAB_TX_FIFO_TX_FIFO_HIGH_THRESH_TX_FIFO_HIGH_THRESH_MAC_FC_EN                         VTSS_F_RAB_TX_FIFO_TX_FIFO_HIGH_THRESH_TX_FIFO_HIGH_THRESH(0xed8)


// Settings for mode GFP2

#define  VTSS_RAB_RAB_BASE_RAB_CONTROL_RX_CLK_ENA_GFP2                                              VTSS_F_RAB_RAB_BASE_RAB_CONTROL_RX_CLK_ENA(0x1)
#define  VTSS_RAB_RAB_BASE_RAB_CONTROL_RX_BYPASS_SELECT_GFP2                                        VTSS_F_RAB_RAB_BASE_RAB_CONTROL_RX_BYPASS_SELECT(0x0)
#define  VTSS_RAB_RAB_BASE_RAB_CONTROL_TX_CLK_ENA_GFP2                                              VTSS_F_RAB_RAB_BASE_RAB_CONTROL_TX_CLK_ENA(0x1)
#define  VTSS_RAB_RAB_BASE_RAB_CONTROL_TX_BYPASS_SELECT_GFP2                                        VTSS_F_RAB_RAB_BASE_RAB_CONTROL_TX_BYPASS_SELECT(0x0)
#define  VTSS_RAB_RX_FIFO_RX_FIFO_CONTROL_RX_FIFO_INPUT_SELECT_GFP2                                 VTSS_F_RAB_RX_FIFO_RX_FIFO_CONTROL_RX_FIFO_INPUT_SELECT(0x2)
#define  VTSS_RAB_RX_FIFO_RX_FIFO_CONTROL_RX_FIFO_OUTPUT_SELECT_GFP2                                VTSS_F_RAB_RX_FIFO_RX_FIFO_CONTROL_RX_FIFO_OUTPUT_SELECT(0x0)
#define  VTSS_RAB_RX_FIFO_RX_FIFO_CONTROL_RX_FIFO_CUT_THRU_GFP2                                     VTSS_F_RAB_RX_FIFO_RX_FIFO_CONTROL_RX_FIFO_CUT_THRU(0x1)
#define  VTSS_RAB_TX_FIFO_TX_FIFO_CONTROL_TX_FIFO_INPUT_SELECT_GFP2                                 VTSS_F_RAB_TX_FIFO_TX_FIFO_CONTROL_TX_FIFO_INPUT_SELECT(0x0)
#define  VTSS_RAB_TX_FIFO_TX_FIFO_CONTROL_TX_FIFO_OUTPUT_SELECT_GFP2                                VTSS_F_RAB_TX_FIFO_TX_FIFO_CONTROL_TX_FIFO_OUTPUT_SELECT(0x1)
#define  VTSS_RAB_TX_FIFO_TX_FIFO_CONTROL_TX_FIFO_CUT_THRU_GFP2                                     VTSS_F_RAB_TX_FIFO_TX_FIFO_CONTROL_TX_FIFO_CUT_THRU(0x0)
#define  VTSS_RAB_RX_FIFO_RX_FIFO_FLUSH_RX_FIFO_FLUSH_GFP2                                          VTSS_F_RAB_RX_FIFO_RX_FIFO_FLUSH_RX_FIFO_FLUSH(0x1)
#define  VTSS_RAB_TX_FIFO_TX_FIFO_FLUSH_TX_FIFO_FLUSH_GFP2                                          VTSS_F_RAB_TX_FIFO_TX_FIFO_FLUSH_TX_FIFO_FLUSH(0x1)
#define  VTSS_RAB_RX_EXTRACT_RX_EXTRACT_STATUS_RX_EXTRACT_FLUSH_GFP2                                VTSS_F_RAB_RX_EXTRACT_RX_EXTRACT_STATUS_RX_EXTRACT_FLUSH(0x1)
#define  VTSS_RAB_TX_EXTRACT_TX_EXTRACT_STATUS_TX_EXTRACT_FLUSH_GFP2                                VTSS_F_RAB_TX_EXTRACT_TX_EXTRACT_STATUS_TX_EXTRACT_FLUSH(0x1)
#define  VTSS_RAB_RX_FIFO_RX_FIFO_ADAPT_THRESH_RX_FIFO_ADAPT_THRESH_GFP2                            VTSS_F_RAB_RX_FIFO_RX_FIFO_ADAPT_THRESH_RX_FIFO_ADAPT_THRESH(0x0)
#define  VTSS_RAB_TX_FIFO_TX_FIFO_ADAPT_THRESH_TX_FIFO_ADAPT_THRESH_GFP2                            VTSS_F_RAB_TX_FIFO_TX_FIFO_ADAPT_THRESH_TX_FIFO_ADAPT_THRESH(0xf80)
#define  VTSS_RAB_TX_FIFO_TX_FIFO_LOW_THRESH_TX_FIFO_LOW_THRESH_GFP2                                VTSS_F_RAB_TX_FIFO_TX_FIFO_LOW_THRESH_TX_FIFO_LOW_THRESH(0x500)
#define  VTSS_RAB_TX_FIFO_TX_FIFO_HIGH_THRESH_TX_FIFO_HIGH_THRESH_GFP2                              VTSS_F_RAB_TX_FIFO_TX_FIFO_HIGH_THRESH_TX_FIFO_HIGH_THRESH(0xfff)


// Settings for mode GFP2_PPOS

#define  VTSS_RAB_RAB_BASE_RAB_CONTROL_RX_CLK_ENA_GFP2_PPOS                                         VTSS_F_RAB_RAB_BASE_RAB_CONTROL_RX_CLK_ENA(0x0)
#define  VTSS_RAB_RAB_BASE_RAB_CONTROL_RX_BYPASS_SELECT_GFP2_PPOS                                   VTSS_F_RAB_RAB_BASE_RAB_CONTROL_RX_BYPASS_SELECT(0x1)
#define  VTSS_RAB_RAB_BASE_RAB_CONTROL_TX_CLK_ENA_GFP2_PPOS                                         VTSS_F_RAB_RAB_BASE_RAB_CONTROL_TX_CLK_ENA(0x1)
#define  VTSS_RAB_RAB_BASE_RAB_CONTROL_TX_BYPASS_SELECT_GFP2_PPOS                                   VTSS_F_RAB_RAB_BASE_RAB_CONTROL_TX_BYPASS_SELECT(0x0)
#define  VTSS_RAB_RX_FIFO_RX_FIFO_CONTROL_RX_FIFO_INPUT_SELECT_GFP2_PPOS                            VTSS_F_RAB_RX_FIFO_RX_FIFO_CONTROL_RX_FIFO_INPUT_SELECT(0x0)
#define  VTSS_RAB_RX_FIFO_RX_FIFO_CONTROL_RX_FIFO_OUTPUT_SELECT_GFP2_PPOS                           VTSS_F_RAB_RX_FIFO_RX_FIFO_CONTROL_RX_FIFO_OUTPUT_SELECT(0x0)
#define  VTSS_RAB_RX_FIFO_RX_FIFO_CONTROL_RX_FIFO_CUT_THRU_GFP2_PPOS                                VTSS_F_RAB_RX_FIFO_RX_FIFO_CONTROL_RX_FIFO_CUT_THRU(0x1)
#define  VTSS_RAB_TX_FIFO_TX_FIFO_CONTROL_TX_FIFO_INPUT_SELECT_GFP2_PPOS                            VTSS_F_RAB_TX_FIFO_TX_FIFO_CONTROL_TX_FIFO_INPUT_SELECT(0x1)
#define  VTSS_RAB_TX_FIFO_TX_FIFO_CONTROL_TX_FIFO_OUTPUT_SELECT_GFP2_PPOS                           VTSS_F_RAB_TX_FIFO_TX_FIFO_CONTROL_TX_FIFO_OUTPUT_SELECT(0x1)
#define  VTSS_RAB_TX_FIFO_TX_FIFO_CONTROL_TX_FIFO_CUT_THRU_GFP2_PPOS                                VTSS_F_RAB_TX_FIFO_TX_FIFO_CONTROL_TX_FIFO_CUT_THRU(0x0)
#define  VTSS_RAB_RX_FIFO_RX_FIFO_FLUSH_RX_FIFO_FLUSH_GFP2_PPOS                                     VTSS_F_RAB_RX_FIFO_RX_FIFO_FLUSH_RX_FIFO_FLUSH(0x1)
#define  VTSS_RAB_TX_FIFO_TX_FIFO_FLUSH_TX_FIFO_FLUSH_GFP2_PPOS                                     VTSS_F_RAB_TX_FIFO_TX_FIFO_FLUSH_TX_FIFO_FLUSH(0x1)
#define  VTSS_RAB_RX_EXTRACT_RX_EXTRACT_STATUS_RX_EXTRACT_FLUSH_GFP2_PPOS                           VTSS_F_RAB_RX_EXTRACT_RX_EXTRACT_STATUS_RX_EXTRACT_FLUSH(0x1)
#define  VTSS_RAB_TX_EXTRACT_TX_EXTRACT_STATUS_TX_EXTRACT_FLUSH_GFP2_PPOS                           VTSS_F_RAB_TX_EXTRACT_TX_EXTRACT_STATUS_TX_EXTRACT_FLUSH(0x1)
#define  VTSS_RAB_RX_FIFO_RX_FIFO_ADAPT_THRESH_RX_FIFO_ADAPT_THRESH_GFP2_PPOS                       VTSS_F_RAB_RX_FIFO_RX_FIFO_ADAPT_THRESH_RX_FIFO_ADAPT_THRESH(0x0)
#define  VTSS_RAB_TX_FIFO_TX_FIFO_ADAPT_THRESH_TX_FIFO_ADAPT_THRESH_GFP2_PPOS                       VTSS_F_RAB_TX_FIFO_TX_FIFO_ADAPT_THRESH_TX_FIFO_ADAPT_THRESH(0xf80)
#define  VTSS_RAB_TX_FIFO_TX_FIFO_LOW_THRESH_TX_FIFO_LOW_THRESH_GFP2_PPOS                           VTSS_F_RAB_TX_FIFO_TX_FIFO_LOW_THRESH_TX_FIFO_LOW_THRESH(0x500)
#define  VTSS_RAB_TX_FIFO_TX_FIFO_HIGH_THRESH_TX_FIFO_HIGH_THRESH_GFP2_PPOS                         VTSS_F_RAB_TX_FIFO_TX_FIFO_HIGH_THRESH_TX_FIFO_HIGH_THRESH(0xfff)


// Settings for mode GFP1

#define  VTSS_RAB_RAB_BASE_RAB_CONTROL_RX_CLK_ENA_GFP1                                              VTSS_F_RAB_RAB_BASE_RAB_CONTROL_RX_CLK_ENA(0x1)
#define  VTSS_RAB_RAB_BASE_RAB_CONTROL_RX_BYPASS_SELECT_GFP1                                        VTSS_F_RAB_RAB_BASE_RAB_CONTROL_RX_BYPASS_SELECT(0x0)
#define  VTSS_RAB_RAB_BASE_RAB_CONTROL_TX_CLK_ENA_GFP1                                              VTSS_F_RAB_RAB_BASE_RAB_CONTROL_TX_CLK_ENA(0x1)
#define  VTSS_RAB_RAB_BASE_RAB_CONTROL_TX_BYPASS_SELECT_GFP1                                        VTSS_F_RAB_RAB_BASE_RAB_CONTROL_TX_BYPASS_SELECT(0x0)
#define  VTSS_RAB_RX_FIFO_RX_FIFO_CONTROL_RX_FIFO_INPUT_SELECT_GFP1                                 VTSS_F_RAB_RX_FIFO_RX_FIFO_CONTROL_RX_FIFO_INPUT_SELECT(0x0)
#define  VTSS_RAB_RX_FIFO_RX_FIFO_CONTROL_RX_FIFO_OUTPUT_SELECT_GFP1                                VTSS_F_RAB_RX_FIFO_RX_FIFO_CONTROL_RX_FIFO_OUTPUT_SELECT(0x1)
#define  VTSS_RAB_RX_FIFO_RX_FIFO_CONTROL_RX_FIFO_CUT_THRU_GFP1                                     VTSS_F_RAB_RX_FIFO_RX_FIFO_CONTROL_RX_FIFO_CUT_THRU(0x0)
#define  VTSS_RAB_TX_FIFO_TX_FIFO_CONTROL_TX_FIFO_INPUT_SELECT_GFP1                                 VTSS_F_RAB_TX_FIFO_TX_FIFO_CONTROL_TX_FIFO_INPUT_SELECT(0x2)
#define  VTSS_RAB_TX_FIFO_TX_FIFO_CONTROL_TX_FIFO_OUTPUT_SELECT_GFP1                                VTSS_F_RAB_TX_FIFO_TX_FIFO_CONTROL_TX_FIFO_OUTPUT_SELECT(0x0)
#define  VTSS_RAB_TX_FIFO_TX_FIFO_CONTROL_TX_FIFO_CUT_THRU_GFP1                                     VTSS_F_RAB_TX_FIFO_TX_FIFO_CONTROL_TX_FIFO_CUT_THRU(0x1)
#define  VTSS_RAB_RX_FIFO_RX_FIFO_FLUSH_RX_FIFO_FLUSH_GFP1                                          VTSS_F_RAB_RX_FIFO_RX_FIFO_FLUSH_RX_FIFO_FLUSH(0x1)
#define  VTSS_RAB_TX_FIFO_TX_FIFO_FLUSH_TX_FIFO_FLUSH_GFP1                                          VTSS_F_RAB_TX_FIFO_TX_FIFO_FLUSH_TX_FIFO_FLUSH(0x1)
#define  VTSS_RAB_RX_EXTRACT_RX_EXTRACT_STATUS_RX_EXTRACT_FLUSH_GFP1                                VTSS_F_RAB_RX_EXTRACT_RX_EXTRACT_STATUS_RX_EXTRACT_FLUSH(0x1)
#define  VTSS_RAB_TX_EXTRACT_TX_EXTRACT_STATUS_TX_EXTRACT_FLUSH_GFP1                                VTSS_F_RAB_TX_EXTRACT_TX_EXTRACT_STATUS_TX_EXTRACT_FLUSH(0x1)
#define  VTSS_RAB_RX_FIFO_RX_FIFO_ADAPT_THRESH_RX_FIFO_ADAPT_THRESH_GFP1                            VTSS_F_RAB_RX_FIFO_RX_FIFO_ADAPT_THRESH_RX_FIFO_ADAPT_THRESH(0x1f80)
#define  VTSS_RAB_TX_FIFO_TX_FIFO_ADAPT_THRESH_TX_FIFO_ADAPT_THRESH_GFP1                            VTSS_F_RAB_TX_FIFO_TX_FIFO_ADAPT_THRESH_TX_FIFO_ADAPT_THRESH(0x0)
#define  VTSS_RAB_TX_FIFO_TX_FIFO_LOW_THRESH_TX_FIFO_LOW_THRESH_GFP1                                VTSS_F_RAB_TX_FIFO_TX_FIFO_LOW_THRESH_TX_FIFO_LOW_THRESH(0x500)
#define  VTSS_RAB_TX_FIFO_TX_FIFO_HIGH_THRESH_TX_FIFO_HIGH_THRESH_GFP1                              VTSS_F_RAB_TX_FIFO_TX_FIFO_HIGH_THRESH_TX_FIFO_HIGH_THRESH(0xfff)


// Settings for mode GFP1_PPOS

#define  VTSS_RAB_RAB_BASE_RAB_CONTROL_RX_CLK_ENA_GFP1_PPOS                                         VTSS_F_RAB_RAB_BASE_RAB_CONTROL_RX_CLK_ENA(0x1)
#define  VTSS_RAB_RAB_BASE_RAB_CONTROL_RX_BYPASS_SELECT_GFP1_PPOS                                   VTSS_F_RAB_RAB_BASE_RAB_CONTROL_RX_BYPASS_SELECT(0x0)
#define  VTSS_RAB_RAB_BASE_RAB_CONTROL_TX_CLK_ENA_GFP1_PPOS                                         VTSS_F_RAB_RAB_BASE_RAB_CONTROL_TX_CLK_ENA(0x0)
#define  VTSS_RAB_RAB_BASE_RAB_CONTROL_TX_BYPASS_SELECT_GFP1_PPOS                                   VTSS_F_RAB_RAB_BASE_RAB_CONTROL_TX_BYPASS_SELECT(0x1)
#define  VTSS_RAB_RX_FIFO_RX_FIFO_CONTROL_RX_FIFO_INPUT_SELECT_GFP1_PPOS                            VTSS_F_RAB_RX_FIFO_RX_FIFO_CONTROL_RX_FIFO_INPUT_SELECT(0x1)
#define  VTSS_RAB_RX_FIFO_RX_FIFO_CONTROL_RX_FIFO_OUTPUT_SELECT_GFP1_PPOS                           VTSS_F_RAB_RX_FIFO_RX_FIFO_CONTROL_RX_FIFO_OUTPUT_SELECT(0x1)
#define  VTSS_RAB_RX_FIFO_RX_FIFO_CONTROL_RX_FIFO_CUT_THRU_GFP1_PPOS                                VTSS_F_RAB_RX_FIFO_RX_FIFO_CONTROL_RX_FIFO_CUT_THRU(0x0)
#define  VTSS_RAB_TX_FIFO_TX_FIFO_CONTROL_TX_FIFO_INPUT_SELECT_GFP1_PPOS                            VTSS_F_RAB_TX_FIFO_TX_FIFO_CONTROL_TX_FIFO_INPUT_SELECT(0x0)
#define  VTSS_RAB_TX_FIFO_TX_FIFO_CONTROL_TX_FIFO_OUTPUT_SELECT_GFP1_PPOS                           VTSS_F_RAB_TX_FIFO_TX_FIFO_CONTROL_TX_FIFO_OUTPUT_SELECT(0x0)
#define  VTSS_RAB_TX_FIFO_TX_FIFO_CONTROL_TX_FIFO_CUT_THRU_GFP1_PPOS                                VTSS_F_RAB_TX_FIFO_TX_FIFO_CONTROL_TX_FIFO_CUT_THRU(0x1)
#define  VTSS_RAB_RX_FIFO_RX_FIFO_FLUSH_RX_FIFO_FLUSH_GFP1_PPOS                                     VTSS_F_RAB_RX_FIFO_RX_FIFO_FLUSH_RX_FIFO_FLUSH(0x1)
#define  VTSS_RAB_TX_FIFO_TX_FIFO_FLUSH_TX_FIFO_FLUSH_GFP1_PPOS                                     VTSS_F_RAB_TX_FIFO_TX_FIFO_FLUSH_TX_FIFO_FLUSH(0x1)
#define  VTSS_RAB_RX_EXTRACT_RX_EXTRACT_STATUS_RX_EXTRACT_FLUSH_GFP1_PPOS                           VTSS_F_RAB_RX_EXTRACT_RX_EXTRACT_STATUS_RX_EXTRACT_FLUSH(0x1)
#define  VTSS_RAB_TX_EXTRACT_TX_EXTRACT_STATUS_TX_EXTRACT_FLUSH_GFP1_PPOS                           VTSS_F_RAB_TX_EXTRACT_TX_EXTRACT_STATUS_TX_EXTRACT_FLUSH(0x1)
#define  VTSS_RAB_RX_FIFO_RX_FIFO_ADAPT_THRESH_RX_FIFO_ADAPT_THRESH_GFP1_PPOS                       VTSS_F_RAB_RX_FIFO_RX_FIFO_ADAPT_THRESH_RX_FIFO_ADAPT_THRESH(0x1f80)
#define  VTSS_RAB_TX_FIFO_TX_FIFO_ADAPT_THRESH_TX_FIFO_ADAPT_THRESH_GFP1_PPOS                       VTSS_F_RAB_TX_FIFO_TX_FIFO_ADAPT_THRESH_TX_FIFO_ADAPT_THRESH(0x0)
#define  VTSS_RAB_TX_FIFO_TX_FIFO_LOW_THRESH_TX_FIFO_LOW_THRESH_GFP1_PPOS                           VTSS_F_RAB_TX_FIFO_TX_FIFO_LOW_THRESH_TX_FIFO_LOW_THRESH(0x500)
#define  VTSS_RAB_TX_FIFO_TX_FIFO_HIGH_THRESH_TX_FIFO_HIGH_THRESH_GFP1_PPOS                         VTSS_F_RAB_TX_FIFO_TX_FIFO_HIGH_THRESH_TX_FIFO_HIGH_THRESH(0xfff)


// Settings for mode BYP

#define  VTSS_RAB_RAB_BASE_RAB_CONTROL_RX_CLK_ENA_BYP                                               VTSS_F_RAB_RAB_BASE_RAB_CONTROL_RX_CLK_ENA(0x0)
#define  VTSS_RAB_RAB_BASE_RAB_CONTROL_RX_BYPASS_SELECT_BYP                                         VTSS_F_RAB_RAB_BASE_RAB_CONTROL_RX_BYPASS_SELECT(0x0)
#define  VTSS_RAB_RAB_BASE_RAB_CONTROL_TX_CLK_ENA_BYP                                               VTSS_F_RAB_RAB_BASE_RAB_CONTROL_TX_CLK_ENA(0x0)
#define  VTSS_RAB_RAB_BASE_RAB_CONTROL_TX_BYPASS_SELECT_BYP                                         VTSS_F_RAB_RAB_BASE_RAB_CONTROL_TX_BYPASS_SELECT(0x0)
#define  VTSS_RAB_RX_FIFO_RX_FIFO_CONTROL_RX_FIFO_INPUT_SELECT_BYP                                  VTSS_F_RAB_RX_FIFO_RX_FIFO_CONTROL_RX_FIFO_INPUT_SELECT(0x0)
#define  VTSS_RAB_RX_FIFO_RX_FIFO_CONTROL_RX_FIFO_OUTPUT_SELECT_BYP                                 VTSS_F_RAB_RX_FIFO_RX_FIFO_CONTROL_RX_FIFO_OUTPUT_SELECT(0x0)
#define  VTSS_RAB_RX_FIFO_RX_FIFO_CONTROL_RX_FIFO_CUT_THRU_BYP                                      VTSS_F_RAB_RX_FIFO_RX_FIFO_CONTROL_RX_FIFO_CUT_THRU(0x1)
#define  VTSS_RAB_TX_FIFO_TX_FIFO_CONTROL_TX_FIFO_INPUT_SELECT_BYP                                  VTSS_F_RAB_TX_FIFO_TX_FIFO_CONTROL_TX_FIFO_INPUT_SELECT(0x0)
#define  VTSS_RAB_TX_FIFO_TX_FIFO_CONTROL_TX_FIFO_OUTPUT_SELECT_BYP                                 VTSS_F_RAB_TX_FIFO_TX_FIFO_CONTROL_TX_FIFO_OUTPUT_SELECT(0x0)
#define  VTSS_RAB_TX_FIFO_TX_FIFO_CONTROL_TX_FIFO_CUT_THRU_BYP                                      VTSS_F_RAB_TX_FIFO_TX_FIFO_CONTROL_TX_FIFO_CUT_THRU(0x1)
#define  VTSS_RAB_RX_FIFO_RX_FIFO_FLUSH_RX_FIFO_FLUSH_BYP                                           VTSS_F_RAB_RX_FIFO_RX_FIFO_FLUSH_RX_FIFO_FLUSH(0x1)
#define  VTSS_RAB_TX_FIFO_TX_FIFO_FLUSH_TX_FIFO_FLUSH_BYP                                           VTSS_F_RAB_TX_FIFO_TX_FIFO_FLUSH_TX_FIFO_FLUSH(0x1)
#define  VTSS_RAB_RX_EXTRACT_RX_EXTRACT_STATUS_RX_EXTRACT_FLUSH_BYP                                 VTSS_F_RAB_RX_EXTRACT_RX_EXTRACT_STATUS_RX_EXTRACT_FLUSH(0x1)
#define  VTSS_RAB_TX_EXTRACT_TX_EXTRACT_STATUS_TX_EXTRACT_FLUSH_BYP                                 VTSS_F_RAB_TX_EXTRACT_TX_EXTRACT_STATUS_TX_EXTRACT_FLUSH(0x1)
#define  VTSS_RAB_RX_FIFO_RX_FIFO_ADAPT_THRESH_RX_FIFO_ADAPT_THRESH_BYP                             VTSS_F_RAB_RX_FIFO_RX_FIFO_ADAPT_THRESH_RX_FIFO_ADAPT_THRESH(0x1f80)
#define  VTSS_RAB_TX_FIFO_TX_FIFO_ADAPT_THRESH_TX_FIFO_ADAPT_THRESH_BYP                             VTSS_F_RAB_TX_FIFO_TX_FIFO_ADAPT_THRESH_TX_FIFO_ADAPT_THRESH(0xf80)
#define  VTSS_RAB_TX_FIFO_TX_FIFO_LOW_THRESH_TX_FIFO_LOW_THRESH_BYP                                 VTSS_F_RAB_TX_FIFO_TX_FIFO_LOW_THRESH_TX_FIFO_LOW_THRESH(0x500)
#define  VTSS_RAB_TX_FIFO_TX_FIFO_HIGH_THRESH_TX_FIFO_HIGH_THRESH_BYP                               VTSS_F_RAB_TX_FIFO_TX_FIFO_HIGH_THRESH_TX_FIFO_HIGH_THRESH(0xfff)




typedef enum {
    BM_RAB_MAC,
    BM_RAB_MAC_FC_EN,
    BM_RAB_GFP2,
    BM_RAB_GFP2_PPOS,
    BM_RAB_GFP1,
    BM_RAB_GFP1_PPOS,
    BM_RAB_BYP,
    BM_RAB_LAST
} block_rab_mode_t;

#endif /* _VTSS_DAYTONA_REG_INIT_RAB_H */
