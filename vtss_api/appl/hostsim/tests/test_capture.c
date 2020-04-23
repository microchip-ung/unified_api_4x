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
#include "vtss_api.h"
#include "vtss_macsec_api.h"
#include "vtss_macsec_emu_base.h"
#include "vtss_phy_api.h" 
#include "vtss_phy_10g_api.h" 
int main()
{
    // Create a PHY instance
    vtss_inst_t inst = instance_phy_new();

    
    //
    // Capture encrypted frame
    //
    vtss_port_no_t port_no = 0; // Internal port which we are going to test.

    // Setup capturing of egress frame
    vtss_macsec_frame_capture_set(inst, port_no, VTSS_MACSEC_FRAME_CAPTURE_EGRESS); 
    
    // *** Start Your frame transmission - and wait until it is transmitted ****

    // Stop capturing of egress frame
    vtss_macsec_frame_capture_set(inst, port_no, VTSS_MACSEC_FRAME_CAPTURE_DISABLE); 

    // Get the captured frame
    u32 frm_len;                                        // Length of the frame captured
    u8 frm_buffer[VTSS_MACSEC_FRAME_CAPTURE_SIZE_MAX]; // Buffer for storing the frame 
    vtss_macsec_frame_get(inst, port_no, VTSS_MACSEC_FRAME_CAPTURE_SIZE_MAX, &frm_len, &frm_buffer[0]);
    
    // *** Do your checks *** 

    
    //
    // Capture decrypted frame
    //
    
    // Determine PHY type
    vtss_phy_type_t phy_1g_id;
    BOOL is_phy_1g  = (vtss_phy_id_get(inst, port_no, &phy_1g_id) == VTSS_RC_OK)  && (phy_1g_id.part_number != VTSS_PHY_TYPE_NONE);

#ifdef VTSS_CHIP_10G_PHY
    vtss_phy_10g_id_t phy_10g_id;
    BOOL is_phy_10g = (vtss_phy_10g_id_get(inst, port_no, &phy_10g_id) == VTSS_RC_OK) && (phy_10g_id.part_number != VTSS_PHY_TYPE_10G_NONE);
    
    // Setup loopback 
    vtss_phy_10g_loopback_t lb_10g;
#endif
    vtss_phy_loopback_t lb_1g;

    if (is_phy_1g) {
        lb_1g.far_end_enable  = TRUE;  // Enable loop back at the CU side
        lb_1g.near_end_enable = FALSE; // don't enable loop back at teh MAC side.
        vtss_phy_loopback_set(inst, port_no, lb_1g);
#ifdef VTSS_CHIP_10G_PHY
    } else if (is_phy_10g) {
        lb_10g.enable  = TRUE;
        lb_10g.lb_type = VTSS_LB_SYSTEM_XS_SHALLOW;
        vtss_phy_10g_loopback_set(inst, port_no, &lb_10g);
#endif
    } else {
        T_E("Unknown PHY type");
        return 1;
    }

    // Setup capturing of ingress frame
    vtss_macsec_frame_capture_set(inst, port_no, VTSS_MACSEC_FRAME_CAPTURE_INGRESS); 
    
    // *** Start Your frame transmission - and wait until it is transmitted ****

    // Stop capturing of frame
    vtss_macsec_frame_capture_set(inst, port_no, VTSS_MACSEC_FRAME_CAPTURE_DISABLE); 

    // Get the captured frame
    vtss_macsec_frame_get(inst, port_no, VTSS_MACSEC_FRAME_CAPTURE_SIZE_MAX, &frm_len, &frm_buffer[0]);
    
    // *** Do your checks *** 
    
    //
    // Remove the loopbacks  and Delete the PHY instance.
    //
    
    if (is_phy_1g) {        
        lb_1g.far_end_enable  = FALSE;  
        vtss_phy_loopback_set(inst, port_no, lb_1g);
#ifdef VTSS_CHIP_10G_PHY
    } else if (is_phy_10g) {
        lb_10g.enable  = FALSE;
        vtss_phy_10g_loopback_set(inst, port_no, &lb_10g);
#endif
    }
    
    instance_phy_delete(inst);
    
    return 0;
}

