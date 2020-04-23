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
#ifndef _BASE_H_
#define _BASE_H_

#ifdef __cplusplus
extern "C" {
#endif

// byte order is handled by the API
#define ETHERTYPE_IEEE_802_1_X 0x888E

#include <vtss/api/options.h>
#include "vtss_api.h"

#define VTSS_OPT_TRACE 1
#ifdef VTSS_OPT_TRACE

extern vtss_trace_conf_t vtss_appl_trace_conf;

/* Application trace group */
#define VTSS_APPL_TRACE_GROUP VTSS_TRACE_GROUP_COUNT
#define VTSS_APPL_TRACE_LAYER VTSS_TRACE_LAYER_COUNT

/* Application trace macros */
#define T_E(...) { if (vtss_appl_trace_conf.level[VTSS_TRACE_LAYER_AIL] >= VTSS_TRACE_LEVEL_ERROR) vtss_callout_trace_printf(VTSS_APPL_TRACE_LAYER, VTSS_APPL_TRACE_GROUP, VTSS_TRACE_LEVEL_ERROR, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__); }
#define T_I(...) { if (vtss_appl_trace_conf.level[VTSS_TRACE_LAYER_AIL] >= VTSS_TRACE_LEVEL_INFO) vtss_callout_trace_printf(VTSS_APPL_TRACE_LAYER, VTSS_APPL_TRACE_GROUP, VTSS_TRACE_LEVEL_INFO, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__); }
#define T_D(...) { if (vtss_appl_trace_conf.level[VTSS_TRACE_LAYER_AIL] >= VTSS_TRACE_LEVEL_DEBUG) vtss_callout_trace_printf(VTSS_APPL_TRACE_LAYER, VTSS_APPL_TRACE_GROUP, VTSS_TRACE_LEVEL_DEBUG, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__); }
#define T_N(...) { if (vtss_appl_trace_conf.level[VTSS_TRACE_LAYER_AIL] >= VTSS_TRACE_LEVEL_NOISE) vtss_callout_trace_printf(VTSS_APPL_TRACE_LAYER, VTSS_APPL_TRACE_GROUP, VTSS_TRACE_LEVEL_NOISE, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__); }
#else

/* No trace */
#define T_E(...)
#define T_I(...)
#define T_D(...)
#define T_N(...)

#endif

#define VTSS_RC_TEST(X)           \
{                                 \
    if ((X) != VTSS_RC_OK) {      \
        printf("Failed: "#X"\n"); \
        abort();                  \
    }                             \
}

#define VTSS_RC_NTEST(X)          \
{                                 \
    if ((X) == VTSS_RC_OK) {      \
        printf("Failed: "#X"\n"); \
        abort();                  \
    }                             \
}

#define VTSS_INT_HEX_TEST(A, B)   \
{                                 \
    if (A != B) {                 \
        printf("Failed: "#A" != "#B" (0x%08x != 0x%08x)\n", (u32)A, (u32)B); \
        abort();                  \
    }                             \
}

#define VTSS_COUNTER_CHECK(A, B)                              \
{                                                             \
    if (A != B) {                                             \
        printf("Failed: "#A" != "#B" (%llu != 0x%llu)\n",     \
               (unsigned long long)A, (unsigned long long)B); \
        abort();                                              \
    }                                                         \
}



vtss_rc miim_read(const vtss_inst_t    inst,
                  const vtss_port_no_t port_no,
                  const u8             addr,
                  u16                  *const value);

vtss_rc miim_write(const vtss_inst_t    inst,
                   const vtss_port_no_t port_no,
                   const u8             addr,
                   const u16            value);

vtss_rc _mmd_read(const vtss_inst_t    inst,
                  const vtss_port_no_t port_no,
                  const u8             mmd,
                  const u16            addr,
                  u16                  *const value);

vtss_rc _mmd_write(const vtss_inst_t    inst,
                   const vtss_port_no_t port_no,
                   const u8             mmd,
                   const u16            addr,
                   const u16            value);

vtss_rc raw_hw_read(const char *reg, u32 *val);
vtss_rc raw_hw_write(const char *reg, u32 val);

vtss_inst_t instance_phy_new();
void instance_phy_delete(vtss_inst_t inst);

#ifdef __cplusplus
}
#endif

#endif /* _BASE_H_ */
