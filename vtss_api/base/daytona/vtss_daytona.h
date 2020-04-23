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

#ifndef _VTSS_DAYTONA_H_
#define _VTSS_DAYTONA_H_

#include "vtss_daytona_basics.h"
#include "vtss_daytona_reg_init_block_states.h"

#if defined(VTSS_ARCH_DAYTONA)
typedef struct {
       daytona_block_id_t block;
       u32 offset;
       u32 value;
       u32 mask;
   } static_cfg_t;


vtss_rc vtss_daytona_inst_create(vtss_state_t *vtss_state);

vtss_rc vtss_daytona_sync(vtss_state_t * vtss_state, const vtss_port_no_t port_no);

vtss_rc vtss_daytona_blk_oper_mode_get(vtss_state_t *vtss_state,
                                       const vtss_port_no_t port_no, daytona_channel_t channel, block_name_t blk_name, u8 *blk_mode);

const static_cfg_t ** vtss_daytona_gcb_config_get(daytona_channel_t channel);
const static_cfg_t * vtss_daytona_rab_config_get(u32 mode);

#endif /* VTSS_ARCH_DAYTONA */
#endif /* _VTSS_DAYTONA_H_ */
