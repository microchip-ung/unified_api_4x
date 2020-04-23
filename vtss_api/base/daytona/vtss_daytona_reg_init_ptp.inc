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
// register structures for mode BYP

#define PTP_TBL_SIZE   1
 static static_cfg_t ptp_byp_tbl[1] = { {DAYTONA_BLOCK_IP_1588, VTSS_IP_1588_IP_1588_TOP_CFG_STAT_INTERFACE_CTL(0), (VTSS_IP_1588_IP_1588_TOP_CFG_STAT_INTERFACE_CTL_BYPASS_BYP | VTSS_IP_1588_IP_1588_TOP_CFG_STAT_INTERFACE_CTL_MII_PROTOCOL_BYP), (VTSS_M_IP_1588_IP_1588_TOP_CFG_STAT_INTERFACE_CTL_BYPASS | VTSS_M_IP_1588_IP_1588_TOP_CFG_STAT_INTERFACE_CTL_MII_PROTOCOL)} };


// register structures for mode UNUSED

static static_cfg_t ptp_unused_tbl[1] = { {DAYTONA_BLOCK_IP_1588, VTSS_IP_1588_IP_1588_TOP_CFG_STAT_INTERFACE_CTL(0), (VTSS_IP_1588_IP_1588_TOP_CFG_STAT_INTERFACE_CTL_BYPASS_UNUSED | VTSS_IP_1588_IP_1588_TOP_CFG_STAT_INTERFACE_CTL_MII_PROTOCOL_UNUSED), (VTSS_M_IP_1588_IP_1588_TOP_CFG_STAT_INTERFACE_CTL_BYPASS | VTSS_M_IP_1588_IP_1588_TOP_CFG_STAT_INTERFACE_CTL_MII_PROTOCOL)} };


static const static_cfg_t *ptp_config_table[BM_PTP_LAST] = {
    ptp_byp_tbl,
    ptp_unused_tbl,
};

