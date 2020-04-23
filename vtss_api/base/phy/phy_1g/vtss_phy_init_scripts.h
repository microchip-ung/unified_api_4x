/*


 Copyright (c) 2002-2017 Microsemi Corporation "Microsemi". All Rights Reserved.

 Unpublished rights reserved under the copyright laws of the United States of
 America, other countries and international treaties. Permission to use, copy,
 store and modify, the software and its source code is granted but only in
 connection with products utilizing the Microsemi switch and PHY products.
 Permission is also granted for you to integrate into other products, disclose,
 transmit and distribute the software only in an absolute machine readable format
 (e.g. HEX file) and only in or with products utilizing the Microsemi switch and
 PHY products.  The source code of the software may not be disclosed, transmitted
 or distributed without the prior written permission of Microsemi.

 This copyright notice must appear in any copy, modification, disclosure,
 transmission or distribution of the software.  Microsemi retains all ownership,
 copyright, trade secret and proprietary rights in the software and its source code,
 including all modifications thereto.

 THIS SOFTWARE HAS BEEN PROVIDED "AS IS". MICROSEMI HEREBY DISCLAIMS ALL WARRANTIES
 OF ANY KIND WITH RESPECT TO THE SOFTWARE, WHETHER SUCH WARRANTIES ARE EXPRESS,
 IMPLIED, STATUTORY OR OTHERWISE INCLUDING, WITHOUT LIMITATION, WARRANTIES OF
 MERCHANTABILITY, FITNESS FOR A PARTICULAR USE OR PURPOSE AND NON-INFRINGEMENT.


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

