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
#ifndef _BASE_H_
#define _BASE_H_

#ifdef __cplusplus
extern "C" {
#endif

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



vtss_rc miim_read(const vtss_port_no_t port_no,
                  const u8             addr,
                  u16                  *const value);

vtss_rc miim_write(const vtss_port_no_t port_no,
                   const u8             addr,
                   const u16            value);

vtss_rc _mmd_read(const vtss_port_no_t port_no,
                  const u8             mmd,
                  const u16            addr,
                  u16                  *const value);

vtss_rc _mmd_write(const vtss_port_no_t port_no,
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
