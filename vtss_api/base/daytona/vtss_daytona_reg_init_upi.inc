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
// register structures for mode XAUI_10GE_SDR

#define UPI_TBL_SIZE   3
 static static_cfg_t upi_xaui_10ge_sdr_tbl[3] = { {DAYTONA_BLOCK_UPI, VTSS_UPI_UPI_CONFIG_UPI_RX_CONFIG(0), (VTSS_UPI_UPI_CONFIG_UPI_RX_CONFIG_RX_ENDIAN_XAUI_10GE_SDR | VTSS_UPI_UPI_CONFIG_UPI_RX_CONFIG_RX_DDR_MODE_XAUI_10GE_SDR | VTSS_UPI_UPI_CONFIG_UPI_RX_CONFIG_RX_PROTO_SEL_XAUI_10GE_SDR), (VTSS_M_UPI_UPI_CONFIG_UPI_RX_CONFIG_RX_ENDIAN | VTSS_M_UPI_UPI_CONFIG_UPI_RX_CONFIG_RX_DDR_MODE | VTSS_M_UPI_UPI_CONFIG_UPI_RX_CONFIG_RX_PROTO_SEL)},
				{DAYTONA_BLOCK_UPI, VTSS_UPI_UPI_CONFIG_UPI_TX_CONFIG(0), (VTSS_UPI_UPI_CONFIG_UPI_TX_CONFIG_TX_ENDIAN_XAUI_10GE_SDR | VTSS_UPI_UPI_CONFIG_UPI_TX_CONFIG_TX_DDR_MODE_XAUI_10GE_SDR | VTSS_UPI_UPI_CONFIG_UPI_TX_CONFIG_TX_PROTO_SEL_XAUI_10GE_SDR), (VTSS_M_UPI_UPI_CONFIG_UPI_TX_CONFIG_TX_ENDIAN | VTSS_M_UPI_UPI_CONFIG_UPI_TX_CONFIG_TX_DDR_MODE | VTSS_M_UPI_UPI_CONFIG_UPI_TX_CONFIG_TX_PROTO_SEL)},
				{DAYTONA_BLOCK_UPI, VTSS_UPI_UPI_RX_OTN_FRMR_UPI_RX_OTN_FRMR_ENA(0), (VTSS_UPI_UPI_RX_OTN_FRMR_UPI_RX_OTN_FRMR_ENA_FRMR_ENA_XAUI_10GE_SDR), (VTSS_M_UPI_UPI_RX_OTN_FRMR_UPI_RX_OTN_FRMR_ENA_FRMR_ENA)} };


// register structures for mode XAUI_10GE_DDR

static static_cfg_t upi_xaui_10ge_ddr_tbl[3] = { {DAYTONA_BLOCK_UPI, VTSS_UPI_UPI_CONFIG_UPI_RX_CONFIG(0), (VTSS_UPI_UPI_CONFIG_UPI_RX_CONFIG_RX_ENDIAN_XAUI_10GE_DDR | VTSS_UPI_UPI_CONFIG_UPI_RX_CONFIG_RX_DDR_MODE_XAUI_10GE_DDR | VTSS_UPI_UPI_CONFIG_UPI_RX_CONFIG_RX_PROTO_SEL_XAUI_10GE_DDR), (VTSS_M_UPI_UPI_CONFIG_UPI_RX_CONFIG_RX_ENDIAN | VTSS_M_UPI_UPI_CONFIG_UPI_RX_CONFIG_RX_DDR_MODE | VTSS_M_UPI_UPI_CONFIG_UPI_RX_CONFIG_RX_PROTO_SEL)},
				{DAYTONA_BLOCK_UPI, VTSS_UPI_UPI_CONFIG_UPI_TX_CONFIG(0), (VTSS_UPI_UPI_CONFIG_UPI_TX_CONFIG_TX_ENDIAN_XAUI_10GE_DDR | VTSS_UPI_UPI_CONFIG_UPI_TX_CONFIG_TX_DDR_MODE_XAUI_10GE_DDR | VTSS_UPI_UPI_CONFIG_UPI_TX_CONFIG_TX_PROTO_SEL_XAUI_10GE_DDR), (VTSS_M_UPI_UPI_CONFIG_UPI_TX_CONFIG_TX_ENDIAN | VTSS_M_UPI_UPI_CONFIG_UPI_TX_CONFIG_TX_DDR_MODE | VTSS_M_UPI_UPI_CONFIG_UPI_TX_CONFIG_TX_PROTO_SEL)},
				{DAYTONA_BLOCK_UPI, VTSS_UPI_UPI_RX_OTN_FRMR_UPI_RX_OTN_FRMR_ENA(0), (VTSS_UPI_UPI_RX_OTN_FRMR_UPI_RX_OTN_FRMR_ENA_FRMR_ENA_XAUI_10GE_DDR), (VTSS_M_UPI_UPI_RX_OTN_FRMR_UPI_RX_OTN_FRMR_ENA_FRMR_ENA)} };


// register structures for mode XAUI_10GFC_SDR

static static_cfg_t upi_xaui_10gfc_sdr_tbl[3] = { {DAYTONA_BLOCK_UPI, VTSS_UPI_UPI_CONFIG_UPI_RX_CONFIG(0), (VTSS_UPI_UPI_CONFIG_UPI_RX_CONFIG_RX_ENDIAN_XAUI_10GFC_SDR | VTSS_UPI_UPI_CONFIG_UPI_RX_CONFIG_RX_DDR_MODE_XAUI_10GFC_SDR | VTSS_UPI_UPI_CONFIG_UPI_RX_CONFIG_RX_PROTO_SEL_XAUI_10GFC_SDR), (VTSS_M_UPI_UPI_CONFIG_UPI_RX_CONFIG_RX_ENDIAN | VTSS_M_UPI_UPI_CONFIG_UPI_RX_CONFIG_RX_DDR_MODE | VTSS_M_UPI_UPI_CONFIG_UPI_RX_CONFIG_RX_PROTO_SEL)},
				{DAYTONA_BLOCK_UPI, VTSS_UPI_UPI_CONFIG_UPI_TX_CONFIG(0), (VTSS_UPI_UPI_CONFIG_UPI_TX_CONFIG_TX_ENDIAN_XAUI_10GFC_SDR | VTSS_UPI_UPI_CONFIG_UPI_TX_CONFIG_TX_DDR_MODE_XAUI_10GFC_SDR | VTSS_UPI_UPI_CONFIG_UPI_TX_CONFIG_TX_PROTO_SEL_XAUI_10GFC_SDR), (VTSS_M_UPI_UPI_CONFIG_UPI_TX_CONFIG_TX_ENDIAN | VTSS_M_UPI_UPI_CONFIG_UPI_TX_CONFIG_TX_DDR_MODE | VTSS_M_UPI_UPI_CONFIG_UPI_TX_CONFIG_TX_PROTO_SEL)},
				{DAYTONA_BLOCK_UPI, VTSS_UPI_UPI_RX_OTN_FRMR_UPI_RX_OTN_FRMR_ENA(0), (VTSS_UPI_UPI_RX_OTN_FRMR_UPI_RX_OTN_FRMR_ENA_FRMR_ENA_XAUI_10GFC_SDR), (VTSS_M_UPI_UPI_RX_OTN_FRMR_UPI_RX_OTN_FRMR_ENA_FRMR_ENA)} };


// register structures for mode XAUI_10GFC_DDR

static static_cfg_t upi_xaui_10gfc_ddr_tbl[3] = { {DAYTONA_BLOCK_UPI, VTSS_UPI_UPI_CONFIG_UPI_RX_CONFIG(0), (VTSS_UPI_UPI_CONFIG_UPI_RX_CONFIG_RX_ENDIAN_XAUI_10GFC_DDR | VTSS_UPI_UPI_CONFIG_UPI_RX_CONFIG_RX_DDR_MODE_XAUI_10GFC_DDR | VTSS_UPI_UPI_CONFIG_UPI_RX_CONFIG_RX_PROTO_SEL_XAUI_10GFC_DDR), (VTSS_M_UPI_UPI_CONFIG_UPI_RX_CONFIG_RX_ENDIAN | VTSS_M_UPI_UPI_CONFIG_UPI_RX_CONFIG_RX_DDR_MODE | VTSS_M_UPI_UPI_CONFIG_UPI_RX_CONFIG_RX_PROTO_SEL)},
				{DAYTONA_BLOCK_UPI, VTSS_UPI_UPI_CONFIG_UPI_TX_CONFIG(0), (VTSS_UPI_UPI_CONFIG_UPI_TX_CONFIG_TX_ENDIAN_XAUI_10GFC_DDR | VTSS_UPI_UPI_CONFIG_UPI_TX_CONFIG_TX_DDR_MODE_XAUI_10GFC_DDR | VTSS_UPI_UPI_CONFIG_UPI_TX_CONFIG_TX_PROTO_SEL_XAUI_10GFC_DDR), (VTSS_M_UPI_UPI_CONFIG_UPI_TX_CONFIG_TX_ENDIAN | VTSS_M_UPI_UPI_CONFIG_UPI_TX_CONFIG_TX_DDR_MODE | VTSS_M_UPI_UPI_CONFIG_UPI_TX_CONFIG_TX_PROTO_SEL)},
				{DAYTONA_BLOCK_UPI, VTSS_UPI_UPI_RX_OTN_FRMR_UPI_RX_OTN_FRMR_ENA(0), (VTSS_UPI_UPI_RX_OTN_FRMR_UPI_RX_OTN_FRMR_ENA_FRMR_ENA_XAUI_10GFC_DDR), (VTSS_M_UPI_UPI_RX_OTN_FRMR_UPI_RX_OTN_FRMR_ENA_FRMR_ENA)} };


// register structures for mode TFI5_SDR

static static_cfg_t upi_tfi5_sdr_tbl[3] = { {DAYTONA_BLOCK_UPI, VTSS_UPI_UPI_CONFIG_UPI_RX_CONFIG(0), (VTSS_UPI_UPI_CONFIG_UPI_RX_CONFIG_RX_ENDIAN_TFI5_SDR | VTSS_UPI_UPI_CONFIG_UPI_RX_CONFIG_RX_DDR_MODE_TFI5_SDR | VTSS_UPI_UPI_CONFIG_UPI_RX_CONFIG_RX_PROTO_SEL_TFI5_SDR), (VTSS_M_UPI_UPI_CONFIG_UPI_RX_CONFIG_RX_ENDIAN | VTSS_M_UPI_UPI_CONFIG_UPI_RX_CONFIG_RX_DDR_MODE | VTSS_M_UPI_UPI_CONFIG_UPI_RX_CONFIG_RX_PROTO_SEL)},
				{DAYTONA_BLOCK_UPI, VTSS_UPI_UPI_CONFIG_UPI_TX_CONFIG(0), (VTSS_UPI_UPI_CONFIG_UPI_TX_CONFIG_TX_ENDIAN_TFI5_SDR | VTSS_UPI_UPI_CONFIG_UPI_TX_CONFIG_TX_DDR_MODE_TFI5_SDR | VTSS_UPI_UPI_CONFIG_UPI_TX_CONFIG_TX_PROTO_SEL_TFI5_SDR), (VTSS_M_UPI_UPI_CONFIG_UPI_TX_CONFIG_TX_ENDIAN | VTSS_M_UPI_UPI_CONFIG_UPI_TX_CONFIG_TX_DDR_MODE | VTSS_M_UPI_UPI_CONFIG_UPI_TX_CONFIG_TX_PROTO_SEL)},
				{DAYTONA_BLOCK_UPI, VTSS_UPI_UPI_RX_OTN_FRMR_UPI_RX_OTN_FRMR_ENA(0), (VTSS_UPI_UPI_RX_OTN_FRMR_UPI_RX_OTN_FRMR_ENA_FRMR_ENA_TFI5_SDR), (VTSS_M_UPI_UPI_RX_OTN_FRMR_UPI_RX_OTN_FRMR_ENA_FRMR_ENA)} };


// register structures for mode TFI5_DDR

static static_cfg_t upi_tfi5_ddr_tbl[3] = { {DAYTONA_BLOCK_UPI, VTSS_UPI_UPI_CONFIG_UPI_RX_CONFIG(0), (VTSS_UPI_UPI_CONFIG_UPI_RX_CONFIG_RX_ENDIAN_TFI5_DDR | VTSS_UPI_UPI_CONFIG_UPI_RX_CONFIG_RX_DDR_MODE_TFI5_DDR | VTSS_UPI_UPI_CONFIG_UPI_RX_CONFIG_RX_PROTO_SEL_TFI5_DDR), (VTSS_M_UPI_UPI_CONFIG_UPI_RX_CONFIG_RX_ENDIAN | VTSS_M_UPI_UPI_CONFIG_UPI_RX_CONFIG_RX_DDR_MODE | VTSS_M_UPI_UPI_CONFIG_UPI_RX_CONFIG_RX_PROTO_SEL)},
				{DAYTONA_BLOCK_UPI, VTSS_UPI_UPI_CONFIG_UPI_TX_CONFIG(0), (VTSS_UPI_UPI_CONFIG_UPI_TX_CONFIG_TX_ENDIAN_TFI5_DDR | VTSS_UPI_UPI_CONFIG_UPI_TX_CONFIG_TX_DDR_MODE_TFI5_DDR | VTSS_UPI_UPI_CONFIG_UPI_TX_CONFIG_TX_PROTO_SEL_TFI5_DDR), (VTSS_M_UPI_UPI_CONFIG_UPI_TX_CONFIG_TX_ENDIAN | VTSS_M_UPI_UPI_CONFIG_UPI_TX_CONFIG_TX_DDR_MODE | VTSS_M_UPI_UPI_CONFIG_UPI_TX_CONFIG_TX_PROTO_SEL)},
				{DAYTONA_BLOCK_UPI, VTSS_UPI_UPI_RX_OTN_FRMR_UPI_RX_OTN_FRMR_ENA(0), (VTSS_UPI_UPI_RX_OTN_FRMR_UPI_RX_OTN_FRMR_ENA_FRMR_ENA_TFI5_DDR), (VTSS_M_UPI_UPI_RX_OTN_FRMR_UPI_RX_OTN_FRMR_ENA_FRMR_ENA)} };


// register structures for mode SFI42_SDR

static static_cfg_t upi_sfi42_sdr_tbl[3] = { {DAYTONA_BLOCK_UPI, VTSS_UPI_UPI_CONFIG_UPI_RX_CONFIG(0), (VTSS_UPI_UPI_CONFIG_UPI_RX_CONFIG_RX_ENDIAN_SFI42_SDR | VTSS_UPI_UPI_CONFIG_UPI_RX_CONFIG_RX_DDR_MODE_SFI42_SDR | VTSS_UPI_UPI_CONFIG_UPI_RX_CONFIG_RX_PROTO_SEL_SFI42_SDR), (VTSS_M_UPI_UPI_CONFIG_UPI_RX_CONFIG_RX_ENDIAN | VTSS_M_UPI_UPI_CONFIG_UPI_RX_CONFIG_RX_DDR_MODE | VTSS_M_UPI_UPI_CONFIG_UPI_RX_CONFIG_RX_PROTO_SEL)},
				{DAYTONA_BLOCK_UPI, VTSS_UPI_UPI_CONFIG_UPI_TX_CONFIG(0), (VTSS_UPI_UPI_CONFIG_UPI_TX_CONFIG_TX_ENDIAN_SFI42_SDR | VTSS_UPI_UPI_CONFIG_UPI_TX_CONFIG_TX_DDR_MODE_SFI42_SDR | VTSS_UPI_UPI_CONFIG_UPI_TX_CONFIG_TX_PROTO_SEL_SFI42_SDR), (VTSS_M_UPI_UPI_CONFIG_UPI_TX_CONFIG_TX_ENDIAN | VTSS_M_UPI_UPI_CONFIG_UPI_TX_CONFIG_TX_DDR_MODE | VTSS_M_UPI_UPI_CONFIG_UPI_TX_CONFIG_TX_PROTO_SEL)},
				{DAYTONA_BLOCK_UPI, VTSS_UPI_UPI_RX_OTN_FRMR_UPI_RX_OTN_FRMR_ENA(0), (VTSS_UPI_UPI_RX_OTN_FRMR_UPI_RX_OTN_FRMR_ENA_FRMR_ENA_SFI42_SDR), (VTSS_M_UPI_UPI_RX_OTN_FRMR_UPI_RX_OTN_FRMR_ENA_FRMR_ENA)} };


// register structures for mode SFI42_DDR

static static_cfg_t upi_sfi42_ddr_tbl[3] = { {DAYTONA_BLOCK_UPI, VTSS_UPI_UPI_CONFIG_UPI_RX_CONFIG(0), (VTSS_UPI_UPI_CONFIG_UPI_RX_CONFIG_RX_ENDIAN_SFI42_DDR | VTSS_UPI_UPI_CONFIG_UPI_RX_CONFIG_RX_DDR_MODE_SFI42_DDR | VTSS_UPI_UPI_CONFIG_UPI_RX_CONFIG_RX_PROTO_SEL_SFI42_DDR), (VTSS_M_UPI_UPI_CONFIG_UPI_RX_CONFIG_RX_ENDIAN | VTSS_M_UPI_UPI_CONFIG_UPI_RX_CONFIG_RX_DDR_MODE | VTSS_M_UPI_UPI_CONFIG_UPI_RX_CONFIG_RX_PROTO_SEL)},
				{DAYTONA_BLOCK_UPI, VTSS_UPI_UPI_CONFIG_UPI_TX_CONFIG(0), (VTSS_UPI_UPI_CONFIG_UPI_TX_CONFIG_TX_ENDIAN_SFI42_DDR | VTSS_UPI_UPI_CONFIG_UPI_TX_CONFIG_TX_DDR_MODE_SFI42_DDR | VTSS_UPI_UPI_CONFIG_UPI_TX_CONFIG_TX_PROTO_SEL_SFI42_DDR), (VTSS_M_UPI_UPI_CONFIG_UPI_TX_CONFIG_TX_ENDIAN | VTSS_M_UPI_UPI_CONFIG_UPI_TX_CONFIG_TX_DDR_MODE | VTSS_M_UPI_UPI_CONFIG_UPI_TX_CONFIG_TX_PROTO_SEL)},
				{DAYTONA_BLOCK_UPI, VTSS_UPI_UPI_RX_OTN_FRMR_UPI_RX_OTN_FRMR_ENA(0), (VTSS_UPI_UPI_RX_OTN_FRMR_UPI_RX_OTN_FRMR_ENA_FRMR_ENA_SFI42_DDR), (VTSS_M_UPI_UPI_RX_OTN_FRMR_UPI_RX_OTN_FRMR_ENA_FRMR_ENA)} };


// register structures for mode SFI42_SDR_FRMR

static static_cfg_t upi_sfi42_sdr_frmr_tbl[3] = { {DAYTONA_BLOCK_UPI, VTSS_UPI_UPI_CONFIG_UPI_RX_CONFIG(0), (VTSS_UPI_UPI_CONFIG_UPI_RX_CONFIG_RX_ENDIAN_SFI42_SDR_FRMR | VTSS_UPI_UPI_CONFIG_UPI_RX_CONFIG_RX_DDR_MODE_SFI42_SDR_FRMR | VTSS_UPI_UPI_CONFIG_UPI_RX_CONFIG_RX_PROTO_SEL_SFI42_SDR_FRMR), (VTSS_M_UPI_UPI_CONFIG_UPI_RX_CONFIG_RX_ENDIAN | VTSS_M_UPI_UPI_CONFIG_UPI_RX_CONFIG_RX_DDR_MODE | VTSS_M_UPI_UPI_CONFIG_UPI_RX_CONFIG_RX_PROTO_SEL)},
				{DAYTONA_BLOCK_UPI, VTSS_UPI_UPI_CONFIG_UPI_TX_CONFIG(0), (VTSS_UPI_UPI_CONFIG_UPI_TX_CONFIG_TX_ENDIAN_SFI42_SDR_FRMR | VTSS_UPI_UPI_CONFIG_UPI_TX_CONFIG_TX_DDR_MODE_SFI42_SDR_FRMR | VTSS_UPI_UPI_CONFIG_UPI_TX_CONFIG_TX_PROTO_SEL_SFI42_SDR_FRMR), (VTSS_M_UPI_UPI_CONFIG_UPI_TX_CONFIG_TX_ENDIAN | VTSS_M_UPI_UPI_CONFIG_UPI_TX_CONFIG_TX_DDR_MODE | VTSS_M_UPI_UPI_CONFIG_UPI_TX_CONFIG_TX_PROTO_SEL)},
				{DAYTONA_BLOCK_UPI, VTSS_UPI_UPI_RX_OTN_FRMR_UPI_RX_OTN_FRMR_ENA(0), (VTSS_UPI_UPI_RX_OTN_FRMR_UPI_RX_OTN_FRMR_ENA_FRMR_ENA_SFI42_SDR_FRMR), (VTSS_M_UPI_UPI_RX_OTN_FRMR_UPI_RX_OTN_FRMR_ENA_FRMR_ENA)} };


// register structures for mode SFI42_DDR_FRMR

static static_cfg_t upi_sfi42_ddr_frmr_tbl[3] = { {DAYTONA_BLOCK_UPI, VTSS_UPI_UPI_CONFIG_UPI_RX_CONFIG(0), (VTSS_UPI_UPI_CONFIG_UPI_RX_CONFIG_RX_ENDIAN_SFI42_DDR_FRMR | VTSS_UPI_UPI_CONFIG_UPI_RX_CONFIG_RX_DDR_MODE_SFI42_DDR_FRMR | VTSS_UPI_UPI_CONFIG_UPI_RX_CONFIG_RX_PROTO_SEL_SFI42_DDR_FRMR), (VTSS_M_UPI_UPI_CONFIG_UPI_RX_CONFIG_RX_ENDIAN | VTSS_M_UPI_UPI_CONFIG_UPI_RX_CONFIG_RX_DDR_MODE | VTSS_M_UPI_UPI_CONFIG_UPI_RX_CONFIG_RX_PROTO_SEL)},
				{DAYTONA_BLOCK_UPI, VTSS_UPI_UPI_CONFIG_UPI_TX_CONFIG(0), (VTSS_UPI_UPI_CONFIG_UPI_TX_CONFIG_TX_ENDIAN_SFI42_DDR_FRMR | VTSS_UPI_UPI_CONFIG_UPI_TX_CONFIG_TX_DDR_MODE_SFI42_DDR_FRMR | VTSS_UPI_UPI_CONFIG_UPI_TX_CONFIG_TX_PROTO_SEL_SFI42_DDR_FRMR), (VTSS_M_UPI_UPI_CONFIG_UPI_TX_CONFIG_TX_ENDIAN | VTSS_M_UPI_UPI_CONFIG_UPI_TX_CONFIG_TX_DDR_MODE | VTSS_M_UPI_UPI_CONFIG_UPI_TX_CONFIG_TX_PROTO_SEL)},
				{DAYTONA_BLOCK_UPI, VTSS_UPI_UPI_RX_OTN_FRMR_UPI_RX_OTN_FRMR_ENA(0), (VTSS_UPI_UPI_RX_OTN_FRMR_UPI_RX_OTN_FRMR_ENA_FRMR_ENA_SFI42_DDR_FRMR), (VTSS_M_UPI_UPI_RX_OTN_FRMR_UPI_RX_OTN_FRMR_ENA_FRMR_ENA)} };


// register structures for mode UNUSED

static static_cfg_t upi_unused_tbl[3] = { {DAYTONA_BLOCK_UPI, VTSS_UPI_UPI_CONFIG_UPI_RX_CONFIG(0), (VTSS_UPI_UPI_CONFIG_UPI_RX_CONFIG_RX_ENDIAN_UNUSED | VTSS_UPI_UPI_CONFIG_UPI_RX_CONFIG_RX_DDR_MODE_UNUSED | VTSS_UPI_UPI_CONFIG_UPI_RX_CONFIG_RX_PROTO_SEL_UNUSED), (VTSS_M_UPI_UPI_CONFIG_UPI_RX_CONFIG_RX_ENDIAN | VTSS_M_UPI_UPI_CONFIG_UPI_RX_CONFIG_RX_DDR_MODE | VTSS_M_UPI_UPI_CONFIG_UPI_RX_CONFIG_RX_PROTO_SEL)},
				{DAYTONA_BLOCK_UPI, VTSS_UPI_UPI_CONFIG_UPI_TX_CONFIG(0), (VTSS_UPI_UPI_CONFIG_UPI_TX_CONFIG_TX_ENDIAN_UNUSED | VTSS_UPI_UPI_CONFIG_UPI_TX_CONFIG_TX_DDR_MODE_UNUSED | VTSS_UPI_UPI_CONFIG_UPI_TX_CONFIG_TX_PROTO_SEL_UNUSED), (VTSS_M_UPI_UPI_CONFIG_UPI_TX_CONFIG_TX_ENDIAN | VTSS_M_UPI_UPI_CONFIG_UPI_TX_CONFIG_TX_DDR_MODE | VTSS_M_UPI_UPI_CONFIG_UPI_TX_CONFIG_TX_PROTO_SEL)},
				{DAYTONA_BLOCK_UPI, VTSS_UPI_UPI_RX_OTN_FRMR_UPI_RX_OTN_FRMR_ENA(0), (VTSS_UPI_UPI_RX_OTN_FRMR_UPI_RX_OTN_FRMR_ENA_FRMR_ENA_UNUSED), (VTSS_M_UPI_UPI_RX_OTN_FRMR_UPI_RX_OTN_FRMR_ENA_FRMR_ENA)} };


static const static_cfg_t *upi_config_table[BM_UPI_LAST] = {
    upi_xaui_10ge_sdr_tbl,
    upi_xaui_10ge_ddr_tbl,
    upi_xaui_10gfc_sdr_tbl,
    upi_xaui_10gfc_ddr_tbl,
    upi_tfi5_sdr_tbl,
    upi_tfi5_ddr_tbl,
    upi_sfi42_sdr_tbl,
    upi_sfi42_ddr_tbl,
    upi_sfi42_sdr_frmr_tbl,
    upi_sfi42_ddr_frmr_tbl,
    upi_unused_tbl,
};

