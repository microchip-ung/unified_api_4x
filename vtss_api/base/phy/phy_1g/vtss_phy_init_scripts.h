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

#ifndef _VTSS_PHY_INIT_SCRIPTS_H_
#define _VTSS_PHY_INIT_SCRIPTS_H_


#include <vtss/api/types.h>

vtss_rc vtss_phy_post_reset_private(struct vtss_state_s *vtss_state, const vtss_port_no_t port_no);



vtss_rc vtss_phy_init_seq_atom(struct vtss_state_s *vtss_state,
                               vtss_phy_port_state_t *ps,
                               vtss_port_no_t        port_no,
                               BOOL                  luton26_mode);

vtss_rc vtss_phy_pre_reset_private(struct vtss_state_s *vtss_state, const vtss_port_no_t port_no);
vtss_rc vtss_phy_wait_for_micro_complete(struct vtss_state_s *vtss_state, vtss_port_no_t port_no);

vtss_rc vtss_phy_is_8051_crc_ok_private (struct vtss_state_s *vtss_state,
                                         vtss_port_no_t port_no,
                                         u16 start_addr,
                                         u16 code_length,
                                         u16 expected_crc,
                                         BOOL first_time);

const char *vtss_phy_family2txt(vtss_phy_family_t family);

vtss_rc vtss_phy_init_seq_blazer(struct vtss_state_s *vtss_state,
                                 vtss_phy_port_state_t *ps,
                                 vtss_port_no_t        port_no);

// The address at where the first byte of the internal 8051 firmware is placed.
#define FIRMWARE_START_ADDR 0x4000

#endif /* _VTSS_PHY_INIT_SCRIPTS_H_ */

