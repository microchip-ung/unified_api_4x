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

#ifndef _VTSS_APPL_CU_PHY_H_
#define _VTSS_APPL_CU_PHY_H_

int atom12_board_init(int argc, const char **argv, vtss_appl_board_t *board);
int tesla_board_init(int argc, const char **argv, vtss_appl_board_t *board);
int elise_board_init(int argc, const char **argv, vtss_appl_board_t *board);
int nano_board_init(int argc, const char **argv, vtss_appl_board_t *board);
int viper_board_init(int argc, const char **argv, vtss_appl_board_t *board);

void vtss_board_phy_init(vtss_appl_board_t *board);

vtss_rc miim_read(const vtss_inst_t    inst,
                  const vtss_port_no_t port_no,
                  const u8             addr,
                  u16                  *const value);

vtss_rc miim_write(const vtss_inst_t    inst,
                   const vtss_port_no_t port_no,
                   const u8             addr,
                   const u16            value);

#if defined(VTSS_CHIP_10G_PHY)
vtss_rc mmd_read(const vtss_inst_t     inst,
                 const vtss_port_no_t  port_no,
                 const u8              mmd,
                 const u16             addr,
                 u16                   *const value);

vtss_rc mmd_write(const vtss_inst_t     inst,
                  const vtss_port_no_t  port_no,
                  const u8              mmd,
                  const u16             addr,
                  const u16             value);

vtss_rc mmd_read_inc(const vtss_inst_t     inst,
                     const vtss_port_no_t  port_no,
                     const u8              mmd,
                     const u16             addr,
                     u16                   *buf,
                     u8                    count);
#endif /* VTSS_CHIP_10G_PHY */
#endif /* _VTSS_APPL_CU_PHY_H_ */
