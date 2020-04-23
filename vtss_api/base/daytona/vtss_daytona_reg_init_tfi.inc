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

