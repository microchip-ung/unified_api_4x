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

#ifndef _VTSS_COMMON_H_
#define _VTSS_COMMON_H_

#include "vtss_api.h"
#include <vtss_os.h>            /* This defines the *runtime* OS environment */

u32 vtss_cmn_pcp2qos(u32 pcp);
vtss_rc vtss_port_no_check(vtss_state_t *vtss_state, const vtss_port_no_t port_no);
vtss_rc vtss_port_no_none_check(vtss_state_t *vtss_state, const vtss_port_no_t port_no);
vtss_rc vtss_cmn_restart_update(vtss_state_t *vtss_state, u32 value);
u32 vtss_cmn_restart_value_get(vtss_state_t *vtss_state);

#if defined(VTSS_FEATURE_PORT_CONTROL)  ||  defined(VTSS_FEATURE_OAM)
void vtss_cmn_counter_32_rebase(u32 new_base_value, vtss_chip_counter_t *counter);
void vtss_cmn_counter_32_update(u32 value, vtss_chip_counter_t *counter, BOOL clear);
void vtss_cmn_counter_40_rebase(u32 new_lsb, u32 new_msb, vtss_chip_counter_t *counter);
void vtss_cmn_counter_40_update(u32 lsb, u32 msb, vtss_chip_counter_t *counter, BOOL clear);
const char *vtss_serdes_mode_txt(vtss_serdes_mode_t mode);
#endif

const char *vtss_bool_txt(BOOL enabled);
vtss_rc vtss_cmn_debug_info_print(vtss_state_t *vtss_state,
                                  const vtss_debug_printf_t pr,
                                  const vtss_debug_info_t   *const info);
void vtss_debug_print_header_underlined(const vtss_debug_printf_t pr,
                                        const char                *header,
                                        BOOL                      layer);
void vtss_debug_print_header(const vtss_debug_printf_t pr,
                             const char                *header);
void vtss_debug_print_port_header(vtss_state_t *vtss_state,
                                  const vtss_debug_printf_t pr,
                                  const char *txt, u32 count, BOOL nl);
void vtss_debug_print_ports(vtss_state_t *vtss_state,
                            const vtss_debug_printf_t pr, u8 *member, BOOL nl);
void vtss_debug_print_port_members(vtss_state_t *vtss_state,
                                   const vtss_debug_printf_t pr,
                                   BOOL port_member[VTSS_PORT_ARRAY_SIZE],
                                   BOOL nl);
BOOL vtss_debug_group_enabled(const vtss_debug_printf_t pr,
                              const vtss_debug_info_t *const info,
                              const vtss_debug_group_t group);
vtss_rc vtss_debug_print_group(const vtss_debug_group_t group,
                               vtss_rc (* dbg)(vtss_state_t *vtss_state,
                                               const vtss_debug_printf_t pr, 
                                               const vtss_debug_info_t   *const info),
                               vtss_state_t *vtss_state,
                               const vtss_debug_printf_t pr,
                               const vtss_debug_info_t *const info);
void vtss_debug_print_sticky(const vtss_debug_printf_t pr, 
                             const char *name, u32 value, u32 offset);
void vtss_debug_print_value(const vtss_debug_printf_t pr, const char *name, u32 value);
void vtss_debug_print_reg_header(const vtss_debug_printf_t pr, const char *name);
void vtss_debug_print_reg(const vtss_debug_printf_t pr, const char *name, u32 value);

vtss_rc vtss_cmn_debug_reg_check_set(vtss_state_t *state, BOOL enable);

vtss_rc vtss_cmn_bit_from_one_hot_mask64(u64 mask, u32 *bit_pos);

#endif /* _VTSS_COMMON_H_ */

