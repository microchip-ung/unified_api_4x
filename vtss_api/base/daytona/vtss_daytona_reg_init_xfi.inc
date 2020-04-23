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

