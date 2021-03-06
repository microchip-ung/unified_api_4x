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
// register structures for mode EFECI7

#define OTU_FEC_TBL_SIZE   5
 static static_cfg_t otu_fec_efeci7_tbl[5] = { {DAYTONA_BLOCK_OTN_WRAPPER, VTSS_OTN_WRAPPER_XCO2_TX_OTN_XCO2_TX_OTN_GLOBAL_CONTROL(0), (VTSS_OTN_WRAPPER_XCO2_TX_OTN_XCO2_TX_OTN_GLOBAL_CONTROL_FEC_BLANKING_EFECI7 | VTSS_OTN_WRAPPER_XCO2_TX_OTN_XCO2_TX_OTN_GLOBAL_CONTROL_EXTERNAL_FEC_EFECI7), (VTSS_M_OTN_WRAPPER_XCO2_TX_OTN_XCO2_TX_OTN_GLOBAL_CONTROL_FEC_BLANKING | VTSS_M_OTN_WRAPPER_XCO2_TX_OTN_XCO2_TX_OTN_GLOBAL_CONTROL_EXTERNAL_FEC)},
				{DAYTONA_BLOCK_OTN_WRAPPER, VTSS_OTN_WRAPPER_XCO2_RX_OTN_XCO2_RX_OTN_GLOBAL_CONTROL(0), (VTSS_OTN_WRAPPER_XCO2_RX_OTN_XCO2_RX_OTN_GLOBAL_CONTROL_FEC_ENABLE_EFECI7 | VTSS_OTN_WRAPPER_XCO2_RX_OTN_XCO2_RX_OTN_GLOBAL_CONTROL_EXTERNAL_FEC_EFECI7), (VTSS_M_OTN_WRAPPER_XCO2_RX_OTN_XCO2_RX_OTN_GLOBAL_CONTROL_FEC_ENABLE | VTSS_M_OTN_WRAPPER_XCO2_RX_OTN_XCO2_RX_OTN_GLOBAL_CONTROL_EXTERNAL_FEC)},
				{DAYTONA_BLOCK_OTN_WRAPPER, VTSS_OTN_WRAPPER_WRAPPER_GROUP_CONTROL_REG(0), (VTSS_OTN_WRAPPER_WRAPPER_GROUP_CONTROL_REG_FEC_SEL_EFECI7), (VTSS_M_OTN_WRAPPER_WRAPPER_GROUP_CONTROL_REG_FEC_SEL)},
				{DAYTONA_BLOCK_EFECI4, VTSS_EFECI4_EFECI4_DECODER_CFG_EFECI4_DECODER_CONFIG1_CFG(0), (VTSS_EFECI4_EFECI4_DECODER_CFG_EFECI4_DECODER_CONFIG1_CFG_DECODER_LATENCY_EFECI7 | VTSS_EFECI4_EFECI4_DECODER_CFG_EFECI4_DECODER_CONFIG1_CFG_DECODER_DI_BYPASS_EFECI7 | VTSS_EFECI4_EFECI4_DECODER_CFG_EFECI4_DECODER_CONFIG1_CFG_DECODER_PMTICK_EN_EFECI7 | VTSS_EFECI4_EFECI4_DECODER_CFG_EFECI4_DECODER_CONFIG1_CFG_DECODER_LOAD_COUNTERS_EFECI7 | VTSS_EFECI4_EFECI4_DECODER_CFG_EFECI4_DECODER_CONFIG1_CFG_DECODER_SCRAM_EN_EFECI7 | VTSS_EFECI4_EFECI4_DECODER_CFG_EFECI4_DECODER_CONFIG1_CFG_DECODER_SW_ENA_EFECI7 | VTSS_EFECI4_EFECI4_DECODER_CFG_EFECI4_DECODER_CONFIG1_CFG_DECODER_SW_RST_EFECI7), (VTSS_M_EFECI4_EFECI4_DECODER_CFG_EFECI4_DECODER_CONFIG1_CFG_DECODER_LATENCY | VTSS_M_EFECI4_EFECI4_DECODER_CFG_EFECI4_DECODER_CONFIG1_CFG_DECODER_DI_BYPASS | VTSS_M_EFECI4_EFECI4_DECODER_CFG_EFECI4_DECODER_CONFIG1_CFG_DECODER_PMTICK_EN | VTSS_M_EFECI4_EFECI4_DECODER_CFG_EFECI4_DECODER_CONFIG1_CFG_DECODER_LOAD_COUNTERS | VTSS_M_EFECI4_EFECI4_DECODER_CFG_EFECI4_DECODER_CONFIG1_CFG_DECODER_SCRAM_EN | VTSS_M_EFECI4_EFECI4_DECODER_CFG_EFECI4_DECODER_CONFIG1_CFG_DECODER_SW_ENA | VTSS_M_EFECI4_EFECI4_DECODER_CFG_EFECI4_DECODER_CONFIG1_CFG_DECODER_SW_RST)},
				{DAYTONA_BLOCK_EFECI4, VTSS_EFECI4_EFECI4_ENCODER_CFG_EFECI4_ENCODER_CONFIG1_CFG(0), (VTSS_EFECI4_EFECI4_ENCODER_CFG_EFECI4_ENCODER_CONFIG1_CFG_ENCODER_BYPASS_EFECI7 | VTSS_EFECI4_EFECI4_ENCODER_CFG_EFECI4_ENCODER_CONFIG1_CFG_ENCODER_SW_ENA_EFECI7), (VTSS_M_EFECI4_EFECI4_ENCODER_CFG_EFECI4_ENCODER_CONFIG1_CFG_ENCODER_BYPASS | VTSS_M_EFECI4_EFECI4_ENCODER_CFG_EFECI4_ENCODER_CONFIG1_CFG_ENCODER_SW_ENA)} };


// register structures for mode EFECI4

static static_cfg_t otu_fec_efeci4_tbl[5] = { {DAYTONA_BLOCK_OTN_WRAPPER, VTSS_OTN_WRAPPER_XCO2_TX_OTN_XCO2_TX_OTN_GLOBAL_CONTROL(0), (VTSS_OTN_WRAPPER_XCO2_TX_OTN_XCO2_TX_OTN_GLOBAL_CONTROL_FEC_BLANKING_EFECI4 | VTSS_OTN_WRAPPER_XCO2_TX_OTN_XCO2_TX_OTN_GLOBAL_CONTROL_EXTERNAL_FEC_EFECI4), (VTSS_M_OTN_WRAPPER_XCO2_TX_OTN_XCO2_TX_OTN_GLOBAL_CONTROL_FEC_BLANKING | VTSS_M_OTN_WRAPPER_XCO2_TX_OTN_XCO2_TX_OTN_GLOBAL_CONTROL_EXTERNAL_FEC)},
				{DAYTONA_BLOCK_OTN_WRAPPER, VTSS_OTN_WRAPPER_XCO2_RX_OTN_XCO2_RX_OTN_GLOBAL_CONTROL(0), (VTSS_OTN_WRAPPER_XCO2_RX_OTN_XCO2_RX_OTN_GLOBAL_CONTROL_FEC_ENABLE_EFECI4 | VTSS_OTN_WRAPPER_XCO2_RX_OTN_XCO2_RX_OTN_GLOBAL_CONTROL_EXTERNAL_FEC_EFECI4), (VTSS_M_OTN_WRAPPER_XCO2_RX_OTN_XCO2_RX_OTN_GLOBAL_CONTROL_FEC_ENABLE | VTSS_M_OTN_WRAPPER_XCO2_RX_OTN_XCO2_RX_OTN_GLOBAL_CONTROL_EXTERNAL_FEC)},
				{DAYTONA_BLOCK_OTN_WRAPPER, VTSS_OTN_WRAPPER_WRAPPER_GROUP_CONTROL_REG(0), (VTSS_OTN_WRAPPER_WRAPPER_GROUP_CONTROL_REG_FEC_SEL_EFECI4), (VTSS_M_OTN_WRAPPER_WRAPPER_GROUP_CONTROL_REG_FEC_SEL)},
				{DAYTONA_BLOCK_EFECI4, VTSS_EFECI4_EFECI4_DECODER_CFG_EFECI4_DECODER_CONFIG1_CFG(0), (VTSS_EFECI4_EFECI4_DECODER_CFG_EFECI4_DECODER_CONFIG1_CFG_DECODER_LATENCY_EFECI4 | VTSS_EFECI4_EFECI4_DECODER_CFG_EFECI4_DECODER_CONFIG1_CFG_DECODER_DI_BYPASS_EFECI4 | VTSS_EFECI4_EFECI4_DECODER_CFG_EFECI4_DECODER_CONFIG1_CFG_DECODER_PMTICK_EN_EFECI4 | VTSS_EFECI4_EFECI4_DECODER_CFG_EFECI4_DECODER_CONFIG1_CFG_DECODER_LOAD_COUNTERS_EFECI4 | VTSS_EFECI4_EFECI4_DECODER_CFG_EFECI4_DECODER_CONFIG1_CFG_DECODER_SCRAM_EN_EFECI4 | VTSS_EFECI4_EFECI4_DECODER_CFG_EFECI4_DECODER_CONFIG1_CFG_DECODER_SW_ENA_EFECI4 | VTSS_EFECI4_EFECI4_DECODER_CFG_EFECI4_DECODER_CONFIG1_CFG_DECODER_SW_RST_EFECI4), (VTSS_M_EFECI4_EFECI4_DECODER_CFG_EFECI4_DECODER_CONFIG1_CFG_DECODER_LATENCY | VTSS_M_EFECI4_EFECI4_DECODER_CFG_EFECI4_DECODER_CONFIG1_CFG_DECODER_DI_BYPASS | VTSS_M_EFECI4_EFECI4_DECODER_CFG_EFECI4_DECODER_CONFIG1_CFG_DECODER_PMTICK_EN | VTSS_M_EFECI4_EFECI4_DECODER_CFG_EFECI4_DECODER_CONFIG1_CFG_DECODER_LOAD_COUNTERS | VTSS_M_EFECI4_EFECI4_DECODER_CFG_EFECI4_DECODER_CONFIG1_CFG_DECODER_SCRAM_EN | VTSS_M_EFECI4_EFECI4_DECODER_CFG_EFECI4_DECODER_CONFIG1_CFG_DECODER_SW_ENA | VTSS_M_EFECI4_EFECI4_DECODER_CFG_EFECI4_DECODER_CONFIG1_CFG_DECODER_SW_RST)},
				{DAYTONA_BLOCK_EFECI4, VTSS_EFECI4_EFECI4_ENCODER_CFG_EFECI4_ENCODER_CONFIG1_CFG(0), (VTSS_EFECI4_EFECI4_ENCODER_CFG_EFECI4_ENCODER_CONFIG1_CFG_ENCODER_BYPASS_EFECI4 | VTSS_EFECI4_EFECI4_ENCODER_CFG_EFECI4_ENCODER_CONFIG1_CFG_ENCODER_SW_ENA_EFECI4), (VTSS_M_EFECI4_EFECI4_ENCODER_CFG_EFECI4_ENCODER_CONFIG1_CFG_ENCODER_BYPASS | VTSS_M_EFECI4_EFECI4_ENCODER_CFG_EFECI4_ENCODER_CONFIG1_CFG_ENCODER_SW_ENA)} };


// register structures for mode RSFEC

static static_cfg_t otu_fec_rsfec_tbl[5] = { {DAYTONA_BLOCK_OTN_WRAPPER, VTSS_OTN_WRAPPER_XCO2_TX_OTN_XCO2_TX_OTN_GLOBAL_CONTROL(0), (VTSS_OTN_WRAPPER_XCO2_TX_OTN_XCO2_TX_OTN_GLOBAL_CONTROL_FEC_BLANKING_RSFEC | VTSS_OTN_WRAPPER_XCO2_TX_OTN_XCO2_TX_OTN_GLOBAL_CONTROL_EXTERNAL_FEC_RSFEC), (VTSS_M_OTN_WRAPPER_XCO2_TX_OTN_XCO2_TX_OTN_GLOBAL_CONTROL_FEC_BLANKING | VTSS_M_OTN_WRAPPER_XCO2_TX_OTN_XCO2_TX_OTN_GLOBAL_CONTROL_EXTERNAL_FEC)},
				{DAYTONA_BLOCK_OTN_WRAPPER, VTSS_OTN_WRAPPER_XCO2_RX_OTN_XCO2_RX_OTN_GLOBAL_CONTROL(0), (VTSS_OTN_WRAPPER_XCO2_RX_OTN_XCO2_RX_OTN_GLOBAL_CONTROL_FEC_ENABLE_RSFEC | VTSS_OTN_WRAPPER_XCO2_RX_OTN_XCO2_RX_OTN_GLOBAL_CONTROL_EXTERNAL_FEC_RSFEC), (VTSS_M_OTN_WRAPPER_XCO2_RX_OTN_XCO2_RX_OTN_GLOBAL_CONTROL_FEC_ENABLE | VTSS_M_OTN_WRAPPER_XCO2_RX_OTN_XCO2_RX_OTN_GLOBAL_CONTROL_EXTERNAL_FEC)},
				{DAYTONA_BLOCK_OTN_WRAPPER, VTSS_OTN_WRAPPER_WRAPPER_GROUP_CONTROL_REG(0), (VTSS_OTN_WRAPPER_WRAPPER_GROUP_CONTROL_REG_FEC_SEL_RSFEC), (VTSS_M_OTN_WRAPPER_WRAPPER_GROUP_CONTROL_REG_FEC_SEL)},
				{DAYTONA_BLOCK_EFECI4, VTSS_EFECI4_EFECI4_DECODER_CFG_EFECI4_DECODER_CONFIG1_CFG(0), (VTSS_EFECI4_EFECI4_DECODER_CFG_EFECI4_DECODER_CONFIG1_CFG_DECODER_LATENCY_RSFEC | VTSS_EFECI4_EFECI4_DECODER_CFG_EFECI4_DECODER_CONFIG1_CFG_DECODER_DI_BYPASS_RSFEC | VTSS_EFECI4_EFECI4_DECODER_CFG_EFECI4_DECODER_CONFIG1_CFG_DECODER_PMTICK_EN_RSFEC | VTSS_EFECI4_EFECI4_DECODER_CFG_EFECI4_DECODER_CONFIG1_CFG_DECODER_LOAD_COUNTERS_RSFEC | VTSS_EFECI4_EFECI4_DECODER_CFG_EFECI4_DECODER_CONFIG1_CFG_DECODER_SCRAM_EN_RSFEC | VTSS_EFECI4_EFECI4_DECODER_CFG_EFECI4_DECODER_CONFIG1_CFG_DECODER_SW_ENA_RSFEC | VTSS_EFECI4_EFECI4_DECODER_CFG_EFECI4_DECODER_CONFIG1_CFG_DECODER_SW_RST_RSFEC), (VTSS_M_EFECI4_EFECI4_DECODER_CFG_EFECI4_DECODER_CONFIG1_CFG_DECODER_LATENCY | VTSS_M_EFECI4_EFECI4_DECODER_CFG_EFECI4_DECODER_CONFIG1_CFG_DECODER_DI_BYPASS | VTSS_M_EFECI4_EFECI4_DECODER_CFG_EFECI4_DECODER_CONFIG1_CFG_DECODER_PMTICK_EN | VTSS_M_EFECI4_EFECI4_DECODER_CFG_EFECI4_DECODER_CONFIG1_CFG_DECODER_LOAD_COUNTERS | VTSS_M_EFECI4_EFECI4_DECODER_CFG_EFECI4_DECODER_CONFIG1_CFG_DECODER_SCRAM_EN | VTSS_M_EFECI4_EFECI4_DECODER_CFG_EFECI4_DECODER_CONFIG1_CFG_DECODER_SW_ENA | VTSS_M_EFECI4_EFECI4_DECODER_CFG_EFECI4_DECODER_CONFIG1_CFG_DECODER_SW_RST)},
				{DAYTONA_BLOCK_EFECI4, VTSS_EFECI4_EFECI4_ENCODER_CFG_EFECI4_ENCODER_CONFIG1_CFG(0), (VTSS_EFECI4_EFECI4_ENCODER_CFG_EFECI4_ENCODER_CONFIG1_CFG_ENCODER_BYPASS_RSFEC | VTSS_EFECI4_EFECI4_ENCODER_CFG_EFECI4_ENCODER_CONFIG1_CFG_ENCODER_SW_ENA_RSFEC), (VTSS_M_EFECI4_EFECI4_ENCODER_CFG_EFECI4_ENCODER_CONFIG1_CFG_ENCODER_BYPASS | VTSS_M_EFECI4_EFECI4_ENCODER_CFG_EFECI4_ENCODER_CONFIG1_CFG_ENCODER_SW_ENA)} };


// register structures for mode UNUSED

static static_cfg_t otu_fec_unused_tbl[5] = { {DAYTONA_BLOCK_OTN_WRAPPER, VTSS_OTN_WRAPPER_XCO2_TX_OTN_XCO2_TX_OTN_GLOBAL_CONTROL(0), (VTSS_OTN_WRAPPER_XCO2_TX_OTN_XCO2_TX_OTN_GLOBAL_CONTROL_FEC_BLANKING_UNUSED | VTSS_OTN_WRAPPER_XCO2_TX_OTN_XCO2_TX_OTN_GLOBAL_CONTROL_EXTERNAL_FEC_UNUSED), (VTSS_M_OTN_WRAPPER_XCO2_TX_OTN_XCO2_TX_OTN_GLOBAL_CONTROL_FEC_BLANKING | VTSS_M_OTN_WRAPPER_XCO2_TX_OTN_XCO2_TX_OTN_GLOBAL_CONTROL_EXTERNAL_FEC)},
				{DAYTONA_BLOCK_OTN_WRAPPER, VTSS_OTN_WRAPPER_XCO2_RX_OTN_XCO2_RX_OTN_GLOBAL_CONTROL(0), (VTSS_OTN_WRAPPER_XCO2_RX_OTN_XCO2_RX_OTN_GLOBAL_CONTROL_FEC_ENABLE_UNUSED | VTSS_OTN_WRAPPER_XCO2_RX_OTN_XCO2_RX_OTN_GLOBAL_CONTROL_EXTERNAL_FEC_UNUSED), (VTSS_M_OTN_WRAPPER_XCO2_RX_OTN_XCO2_RX_OTN_GLOBAL_CONTROL_FEC_ENABLE | VTSS_M_OTN_WRAPPER_XCO2_RX_OTN_XCO2_RX_OTN_GLOBAL_CONTROL_EXTERNAL_FEC)},
				{DAYTONA_BLOCK_OTN_WRAPPER, VTSS_OTN_WRAPPER_WRAPPER_GROUP_CONTROL_REG(0), (VTSS_OTN_WRAPPER_WRAPPER_GROUP_CONTROL_REG_FEC_SEL_UNUSED), (VTSS_M_OTN_WRAPPER_WRAPPER_GROUP_CONTROL_REG_FEC_SEL)},
				{DAYTONA_BLOCK_EFECI4, VTSS_EFECI4_EFECI4_DECODER_CFG_EFECI4_DECODER_CONFIG1_CFG(0), (VTSS_EFECI4_EFECI4_DECODER_CFG_EFECI4_DECODER_CONFIG1_CFG_DECODER_LATENCY_UNUSED | VTSS_EFECI4_EFECI4_DECODER_CFG_EFECI4_DECODER_CONFIG1_CFG_DECODER_DI_BYPASS_UNUSED | VTSS_EFECI4_EFECI4_DECODER_CFG_EFECI4_DECODER_CONFIG1_CFG_DECODER_PMTICK_EN_UNUSED | VTSS_EFECI4_EFECI4_DECODER_CFG_EFECI4_DECODER_CONFIG1_CFG_DECODER_LOAD_COUNTERS_UNUSED | VTSS_EFECI4_EFECI4_DECODER_CFG_EFECI4_DECODER_CONFIG1_CFG_DECODER_SCRAM_EN_UNUSED | VTSS_EFECI4_EFECI4_DECODER_CFG_EFECI4_DECODER_CONFIG1_CFG_DECODER_SW_ENA_UNUSED | VTSS_EFECI4_EFECI4_DECODER_CFG_EFECI4_DECODER_CONFIG1_CFG_DECODER_SW_RST_UNUSED), (VTSS_M_EFECI4_EFECI4_DECODER_CFG_EFECI4_DECODER_CONFIG1_CFG_DECODER_LATENCY | VTSS_M_EFECI4_EFECI4_DECODER_CFG_EFECI4_DECODER_CONFIG1_CFG_DECODER_DI_BYPASS | VTSS_M_EFECI4_EFECI4_DECODER_CFG_EFECI4_DECODER_CONFIG1_CFG_DECODER_PMTICK_EN | VTSS_M_EFECI4_EFECI4_DECODER_CFG_EFECI4_DECODER_CONFIG1_CFG_DECODER_LOAD_COUNTERS | VTSS_M_EFECI4_EFECI4_DECODER_CFG_EFECI4_DECODER_CONFIG1_CFG_DECODER_SCRAM_EN | VTSS_M_EFECI4_EFECI4_DECODER_CFG_EFECI4_DECODER_CONFIG1_CFG_DECODER_SW_ENA | VTSS_M_EFECI4_EFECI4_DECODER_CFG_EFECI4_DECODER_CONFIG1_CFG_DECODER_SW_RST)},
				{DAYTONA_BLOCK_EFECI4, VTSS_EFECI4_EFECI4_ENCODER_CFG_EFECI4_ENCODER_CONFIG1_CFG(0), (VTSS_EFECI4_EFECI4_ENCODER_CFG_EFECI4_ENCODER_CONFIG1_CFG_ENCODER_BYPASS_UNUSED | VTSS_EFECI4_EFECI4_ENCODER_CFG_EFECI4_ENCODER_CONFIG1_CFG_ENCODER_SW_ENA_UNUSED), (VTSS_M_EFECI4_EFECI4_ENCODER_CFG_EFECI4_ENCODER_CONFIG1_CFG_ENCODER_BYPASS | VTSS_M_EFECI4_EFECI4_ENCODER_CFG_EFECI4_ENCODER_CONFIG1_CFG_ENCODER_SW_ENA)} };


static const static_cfg_t *otu_fec_config_table[BM_OTU_FEC_LAST] = {
    otu_fec_efeci7_tbl,
    otu_fec_efeci4_tbl,
    otu_fec_rsfec_tbl,
    otu_fec_unused_tbl,
};

