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

// Avoid "vtss_api.h not used in module vtss_10g_phy_api.c"
/*lint --e{766} */
#include "vtss_api.h"
#if defined(VTSS_CHIP_10G_PHY)
#include "vtss_state.h"


vtss_rc vtss_phy_10g_csr_read(const vtss_inst_t           inst,
                              const vtss_port_no_t        port_no,
                              const u32                   dev,
                              const u32                   addr,
                              u32                         *const value)
{
    vtss_state_t *vtss_state;
    vtss_rc rc = VTSS_RC_ERROR;
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK) {
        if (vtss_state->phy_10g_state[port_no].family == VTSS_PHY_FAMILY_MALIBU) {
            rc = VTSS_FUNC_COLD(cil.malibu_phy_10g_csr_read, port_no, dev, addr, value);
        } else if (vtss_state->phy_10g_state[port_no].family != VTSS_PHY_FAMILY_10G_NONE) {
            rc = VTSS_FUNC_COLD(cil.phy_10g_csr_read, port_no, dev, addr, value);
        }
    }
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_phy_10g_csr_write(const vtss_inst_t           inst,
                               const vtss_port_no_t        port_no,
                               const u32                   dev,
                               const u32                   addr,
                               const u32                   value)
{
    vtss_state_t *vtss_state;
    vtss_rc rc = VTSS_RC_ERROR;
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK) {
        if (vtss_state->phy_10g_state[port_no].family == VTSS_PHY_FAMILY_MALIBU) {
            rc = VTSS_FUNC_COLD(cil.malibu_phy_10g_csr_write, port_no, dev, addr, value);
        } else if (vtss_state->phy_10g_state[port_no].family != VTSS_PHY_FAMILY_10G_NONE) {
            rc = VTSS_FUNC_COLD(cil.phy_10g_csr_write, port_no, dev, addr, value);
        }
    }
    VTSS_EXIT();
    return rc;
}


#endif /* VTSS_CHIP_10G_PHY */
