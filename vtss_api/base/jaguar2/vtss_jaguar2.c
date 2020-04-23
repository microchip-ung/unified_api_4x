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
//#include <cyg/infra/diag.h>
#include "vtss_jaguar2_cil.h"
// Avoid Lint Warning 572: Excessive shift value (precision 1 shifted right by 2), which occurs
// in this file because (t) - VTSS_IO_ORIGIN1_OFFSET == 0 for t = VTSS_TO_CFG (i.e. ICPU_CFG), and 0 >> 2 gives a lint warning.
/*lint --e{572} */
#if defined(VTSS_ARCH_JAGUAR_2)

static vtss_rc jr2_wr_indirect(vtss_state_t *vtss_state, u32 addr, u32 value);
static vtss_rc jr2_rd_indirect(vtss_state_t *vtss_state, u32 addr, u32 *value);

vtss_rc (*vtss_jr2_wr)(vtss_state_t *vtss_state, u32 addr, u32 value) = jr2_wr_indirect;
vtss_rc (*vtss_jr2_rd)(vtss_state_t *vtss_state, u32 addr, u32 *value) = jr2_rd_indirect;

/* Read target register using current CPU interface */
static inline vtss_rc jr2_rd_direct(vtss_state_t *vtss_state, u32 reg, u32 *value)
{
    return vtss_state->init_conf.reg_read(0, reg, value);
}

/* Write target register using current CPU interface */
static inline vtss_rc jr2_wr_direct(vtss_state_t *vtss_state, u32 reg, u32 value)
{
    return vtss_state->init_conf.reg_write(0, reg, value);
}

static inline BOOL jr2_reg_directly_accessible(u32 addr)
{
#if VTSS_OPT_VCORE_III
    // Running on internal CPU. All registers are directly accessible.
    return TRUE;
#else
    /* Running on external CPU. VCoreIII registers require indirect access. */
    /* On internal CPU, all registers are always directly accessible. */
    return (addr >= ((VTSS_IO_ORIGIN2_OFFSET - VTSS_IO_ORIGIN1_OFFSET) >> 2));
#endif
}

/* Read or write register indirectly */
static vtss_rc jr2_reg_indirect_access(vtss_state_t *vtss_state, u32 addr, u32 *value, BOOL is_write)
{
    /* The following access must be executed atomically, and since this function may be called
     * without the API lock taken, we have to disable the scheduler */
    /*lint --e{529} */ // Avoid "Symbol 'flags' not subsequently referenced" Lint warning
    VTSS_OS_SCHEDULER_FLAGS flags = 0;
    u32 ctrl;
    vtss_rc result;

    /* The @addr is an address suitable for the read or write callout function installed by
     * the application, i.e. it's a 32-bit address suitable for presentation on a PI
     * address bus, i.e. it's not suitable for presentation on the VCore-III shared bus.
     * In order to make it suitable for presentation on the VCore-III shared bus, it must
     * be made an 8-bit address, so we multiply by 4, and it must be offset by the base
     * address of the switch core registers, so we add VTSS_IO_ORIGIN1_OFFSET.
     */
    addr <<= 2;
    addr += VTSS_IO_ORIGIN1_OFFSET;

    VTSS_OS_SCHEDULER_LOCK(flags);

    if ((result = vtss_jr2_wr(vtss_state, VTSS_DEVCPU_GCB_VCORE_ACCESS_VA_ADDR, addr)) != VTSS_RC_OK) {
        goto do_exit;
    }
    if (is_write) {
        if ((result = vtss_jr2_wr(vtss_state, VTSS_DEVCPU_GCB_VCORE_ACCESS_VA_DATA, *value)) != VTSS_RC_OK) {
            goto do_exit;
        }
        // Wait for operation to complete
        do {
            if ((result = vtss_jr2_rd(vtss_state, VTSS_DEVCPU_GCB_VCORE_ACCESS_VA_CTRL, &ctrl)) != VTSS_RC_OK) {
                goto do_exit;
            }
        } while (ctrl & VTSS_M_DEVCPU_GCB_VCORE_ACCESS_VA_CTRL_VA_BUSY);
    } else {
        // Dummy read to initiate access
        if ((result = vtss_jr2_rd(vtss_state, VTSS_DEVCPU_GCB_VCORE_ACCESS_VA_DATA, value)) != VTSS_RC_OK) {
            goto do_exit;
        }
        // Wait for operation to complete
        do {
            if ((result = vtss_jr2_rd(vtss_state, VTSS_DEVCPU_GCB_VCORE_ACCESS_VA_CTRL, &ctrl)) != VTSS_RC_OK) {
                goto do_exit;
            }
        } while (ctrl & VTSS_M_DEVCPU_GCB_VCORE_ACCESS_VA_CTRL_VA_BUSY);
        if ((result = vtss_jr2_rd(vtss_state, VTSS_DEVCPU_GCB_VCORE_ACCESS_VA_DATA, value)) != VTSS_RC_OK) {
            goto do_exit;
        }
    }

do_exit:
    VTSS_OS_SCHEDULER_UNLOCK(flags);
    return result;
}

/* Read target register using current CPU interface */
static vtss_rc jr2_rd_indirect(vtss_state_t *vtss_state, u32 reg, u32 *value)
{
    if (jr2_reg_directly_accessible(reg)) {
        return vtss_state->init_conf.reg_read(0, reg, value);
    } else {
        return jr2_reg_indirect_access(vtss_state, reg, value, FALSE);
    }
}

/* Write target register using current CPU interface */
static vtss_rc jr2_wr_indirect(vtss_state_t *vtss_state, u32 reg, u32 value)
{
    if (jr2_reg_directly_accessible(reg)) {
        return vtss_state->init_conf.reg_write(0, reg, value);
    } else {
        return jr2_reg_indirect_access(vtss_state, reg, &value, TRUE);
    }
}

/* Read-modify-write target register using current CPU interface */
vtss_rc vtss_jr2_wrm(vtss_state_t *vtss_state, u32 reg, u32 value, u32 mask)
{
    vtss_rc rc;
    u32     val;

    if ((rc = vtss_jr2_rd(vtss_state, reg, &val)) == VTSS_RC_OK) {
        val = ((val & ~mask) | (value & mask));
        rc = vtss_jr2_wr(vtss_state, reg, val);
    }
    return rc;
}

/* ================================================================= *
 *  Utility functions
 * ================================================================= */

u64 vtss_jr2_pmask(u32 port)
{
    u64 mask = 1;
    
    return (mask << port);
}

u64 vtss_jr2_port_mask(vtss_state_t *vtss_state,
                       const BOOL member[])
{
    vtss_port_no_t port_no;
    u64            pmask = 0;
    
    for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {
        if (member[port_no]) {
            pmask |= vtss_jr2_pmask(VTSS_CHIP_PORT(port_no));
        }
    }
    return pmask;
} // vtss_jr2_port_mask

/* Convert to PGID in chip */
u32 vtss_jr2_chip_pgid(vtss_state_t *vtss_state, u32 pgid)
{
    if (pgid < vtss_state->port_count) {
        return VTSS_CHIP_PORT(pgid);
    } else {
        return (pgid + VTSS_CHIP_PORTS - vtss_state->port_count);
    }
} // vtss_jr2_chip_pgid

/* Convert from PGID in chip */
u32 vtss_jr2_vtss_pgid(const vtss_state_t *const state, u32 pgid)
{
    vtss_port_no_t        port_no;
    const vtss_port_map_t *pmap;

    if (pgid < VTSS_CHIP_PORTS) {
        for (port_no = VTSS_PORT_NO_START; port_no < state->port_count; port_no++) {
            pmap = &state->port.map[port_no];
            if (pmap->chip_port == pgid)
                break;
        }
        return port_no;
    } else {
        return (state->port_count + pgid - VTSS_CHIP_PORTS);
    }
} // vtss_jr2_vtss_pgid

/* ================================================================= *
 *  Debug print utility functions
 * ================================================================= */

void vtss_jr2_debug_print_port_header(vtss_state_t *vtss_state,
                                       const vtss_debug_printf_t pr, const char *txt)
{
    vtss_debug_print_port_header(vtss_state, pr, txt, VTSS_CHIP_PORTS + 1, 1);
}

void vtss_jr2_debug_print_pmask(const vtss_debug_printf_t pr, u64 pmask)
{
    u32 port;

    for (port = 0; port <= VTSS_CHIP_PORTS; port++) {
        pr("%s%s", port == 0 || (port & 7) ? "" : ".", (vtss_jr2_pmask(port) & pmask) ? "1" : "0");
    }
    pr("\n");
}

void vtss_jr2_debug_reg_header(const vtss_debug_printf_t pr, const char *name)
{
    char buf[64];
    
    sprintf(buf, "%-32s  Tgt   Addr  ", name);
    vtss_debug_print_reg_header(pr, buf);
}

void vtss_jr2_debug_reg(vtss_state_t *vtss_state,
                        const vtss_debug_printf_t pr, u32 addr, const char *name)
{
    u32 value;
    char buf[64];

    if (vtss_jr2_rd(vtss_state, addr, &value) == VTSS_RC_OK) {
        sprintf(buf, "%-32s  0x%02x  0x%04x", name, (addr >> 14) & 0x3f, addr & 0x3fff);
        vtss_debug_print_reg(pr, buf, value);
    }
}

void vtss_jr2_debug_reg_inst(vtss_state_t *vtss_state,
                             const vtss_debug_printf_t pr, u32 addr, u32 i, const char *name)
{
    char buf[64];

    sprintf(buf, "%s_%u", name, i);
    vtss_jr2_debug_reg(vtss_state, pr, addr, buf);
}

void vtss_jr2_debug_cnt(const vtss_debug_printf_t pr, const char *col1, const char *col2,
                        vtss_chip_counter_t *c1, vtss_chip_counter_t *c2)
{
    char buf[80];
    
    sprintf(buf, "rx_%s:", col1);
    pr("%-28s%10" PRIu64 "   ", buf, c1->value);
    if (col2 != NULL) {
        sprintf(buf, "tx_%s:", strlen(col2) ? col2 : col1);
        pr("%-28s%10" PRIu64, buf, c2->value);
    }
    pr("\n");
}


static void jr2_debug_reg_clr(vtss_state_t *vtss_state,
                              const vtss_debug_printf_t pr, u32 addr, const char *name, BOOL clr)
{
    u32  value, tgt;
    char buf[64];

    if (vtss_jr2_rd(vtss_state, addr, &value) == VTSS_RC_OK &&
        (clr == 0 || vtss_jr2_wr(vtss_state, addr, value) == VTSS_RC_OK)) {
        tgt = ((addr >> 14) & 0xff);
        if (tgt < 128) {
            /* 14 bit address */
            addr &= 0x3fff;
        } else if (tgt < 192) {
            /* 17 bit address */
            tgt &= 0xf8;
            addr &= 0x1ffff;
        } else {
            /* 18 bit address */
            tgt &= 0xf0;
            addr &= 0x3ffff;
        }
        sprintf(buf, "%-32s  0x%02x 0x%05x", name, tgt, addr);
        vtss_debug_print_reg(pr, buf, value);
    }
} // jr2_debug_reg_clr


void vtss_jr2_debug_sticky(vtss_state_t *vtss_state,
                           const vtss_debug_printf_t pr, u32 addr, const char *name)
{
    jr2_debug_reg_clr(vtss_state, pr, addr, name, 1);
} // vtss_jr2_debug_sticky

static vtss_rc jr2_debug_info_print(vtss_state_t *vtss_state,
                                     const vtss_debug_printf_t pr,
                                     const vtss_debug_info_t   *const info)
{
    VTSS_RC(vtss_jr2_misc_debug_print(vtss_state, pr, info));
    VTSS_RC(vtss_jr2_port_debug_print(vtss_state, pr, info));
    VTSS_RC(vtss_jr2_l2_debug_print(vtss_state, pr, info));
#if defined(VTSS_FEATURE_LAYER3)
    VTSS_RC(vtss_jr2_l3_debug_print(vtss_state, pr, info));
#endif /* VTSS_FEATURE_LAYER3 */
    VTSS_RC(vtss_jr2_vcap_debug_print(vtss_state, pr, info));
#if defined(VTSS_FEATURE_QOS)
    VTSS_RC(vtss_jr2_qos_debug_print(vtss_state, pr, info));
#endif /* VTSS_FEATURE_QOS */
#if defined(VTSS_FEATURE_EVC)
    VTSS_RC(vtss_jr2_evc_debug_print(vtss_state, pr, info));
#endif /* VTSS_FEATURE_EVC */
    VTSS_RC(vtss_jr2_packet_debug_print(vtss_state, pr, info));
#if defined(VTSS_FEATURE_AFI_SWC)
    VTSS_RC(vtss_jr2_afi_debug_print(vtss_state, pr, info));
#endif /* VTSS_FEATURE_AFI_SWC */
#if defined(VTSS_FEATURE_TIMESTAMP)
    VTSS_RC(vtss_jr2_ts_debug_print(vtss_state, pr, info));
#endif /* VTSS_FEATURE_TIMESTAMP */
#if defined(VTSS_FEATURE_OAM)
    VTSS_RC(vtss_jr2_oam_debug_print(vtss_state, pr, info));
#endif /* VTSS_FEATURE_OAM */
#if defined (VTSS_FEATURE_MPLS)
    VTSS_RC(vtss_jr2_mpls_debug_print(vtss_state, pr, info));
#endif /* VTSS_FEATURE_MPLS */
#if defined(VTSS_FEATURE_FDMA) && VTSS_OPT_FDMA
    if (vtss_debug_group_enabled(pr, info, VTSS_DEBUG_GROUP_FDMA)) {
        if (vtss_state->fdma_state.fdma_func.fdma_debug_print != NULL) {
            return vtss_state->fdma_state.fdma_func.fdma_debug_print(vtss_state, pr, info);
        } else {
            return VTSS_RC_ERROR;
        }
    }
#endif 
    return VTSS_RC_OK;
}

vtss_rc vtss_jr2_init_groups(vtss_state_t *vtss_state, vtss_init_cmd_t cmd)
{
    /* Initialize ports */
    VTSS_RC(vtss_jr2_port_init(vtss_state, cmd));

    /* Initialize miscellaneous */
    VTSS_RC(vtss_jr2_misc_init(vtss_state, cmd));

    /* Initialize packet before L2 to ensure that VLAN table clear does not break VRAP access */
    VTSS_RC(vtss_jr2_packet_init(vtss_state, cmd));

#if defined(VTSS_FEATURE_AFI_SWC) && defined(VTSS_AFI_V2)
    VTSS_RC(vtss_jr2_afi_init(vtss_state, cmd));
#endif /* VTSS_FEATURE_AFI_SWC&& VTSS_AFI_V2 */

    /* Initialize L2 */
    VTSS_RC(vtss_jr2_l2_init(vtss_state, cmd));

#if defined(VTSS_FEATURE_LAYER3)
    /* Initialize L3 */
    VTSS_RC(vtss_jr2_l3_init(vtss_state, cmd));
#endif /* VTSS_FEATURE_LAYER3 */

#if defined(VTSS_FEATURE_EVC)
    VTSS_RC(vtss_jr2_evc_init(vtss_state, cmd));
#endif /* VTSS_FEATURE_EVC */

#if defined(VTSS_FEATURE_VCAP)
    VTSS_RC(vtss_jr2_vcap_init(vtss_state, cmd));
#endif

#if defined(VTSS_FEATURE_QOS)
    VTSS_RC(vtss_jr2_qos_init(vtss_state, cmd));
#endif /* VTSS_FEATURE_QOS */

#if defined(VTSS_FEATURE_TIMESTAMP)
    VTSS_RC(vtss_jr2_ts_init(vtss_state, cmd));
#endif /* VTSS_FEATURE_TIMESTAMP */

#if defined(VTSS_FEATURE_OAM)
    VTSS_RC(vtss_jr2_oam_init(vtss_state, cmd));
#endif

#if defined(VTSS_FEATURE_MPLS)
    VTSS_RC(vtss_jr2_mpls_init(vtss_state, cmd));
#endif

    return VTSS_RC_OK;
}

#define JR2_CAL_SPEED_NONE 0
#define JR2_CAL_SPEED_1G   1
#define JR2_CAL_SPEED_2_5G 2
#define JR2_CAL_SPEED_10G  3

// Auto configure the calender based on port-map
static vtss_rc jr2_calender_auto(vtss_state_t *vtss_state)
{
    u32                cal[4], value, bw = 0, spd;
#if !defined(VTSS_ARCH_SERVAL_T)
    u32                i;
#endif /* VTSS_ARCH_SERVAL_T */
    vtss_port_no_t     port_no;
    vtss_internal_bw_t max_bw;
    i32                port;

    VTSS_I("Using Auto calender");

    // Setup the calender, i.e. the BW to each device
    memset(cal, 0, sizeof(cal));
    for (port_no = 0; port_no < (VTSS_PORTS + 4); port_no++) {
        /* Assume 1G by default */
        spd = JR2_CAL_SPEED_1G;
        if (port_no >= VTSS_PORTS) {
            /* CPU/virtual port, 1G */
            port = (VTSS_CHIP_PORT_CPU + port_no - VTSS_PORTS);
        } else {
            /* Switch port, use port map */
            if ((port = VTSS_CHIP_PORT(port_no)) == CHIP_PORT_UNUSED) {
                continue;
            }
            max_bw = vtss_state->port.map[port_no].max_bw;
            if (port == VTSS_CHIP_PORT_NPI) {
                /* NPI port, 1G */
            } else if (VTSS_PORT_IS_10G(port)) {
                /* 10G port */
                if (max_bw != VTSS_BW_1G) {
                    spd = (max_bw == VTSS_BW_2500M ? JR2_CAL_SPEED_2_5G : JR2_CAL_SPEED_10G);
                }
            } else if (port >= VTSS_CHIP_PORT_2_5G_START) {
                /* 2.5G port */
                if (VTSS_PORTS < 38 && max_bw != VTSS_BW_1G) {
                    /* Assuming not 48 QSGMII ports */
                    spd = JR2_CAL_SPEED_2_5G;
                }
            } else {
                /* 1G port */
            }
        }
        bw += (spd == JR2_CAL_SPEED_1G ? 1000 : spd == JR2_CAL_SPEED_2_5G ? 2500 : 10000);
        cal[port/16] += (spd << ((port % 16)*2));
    }

    if (vtss_state->create.target == VTSS_TARGET_SPARX_IV_90) {
        if (bw > 94000) {
            VTSS_E("The configured BW (%d) is above limit",bw);
        }
    } else {
        if (bw > 84000) {
            VTSS_E("The configured BW (%d) is above limit",bw);
        }
    }

    /* Use auto calender mode for calender calculation */
#if defined(VTSS_ARCH_SERVAL_T)
    JR2_WR(VTSS_QSYS_CALCFG_CAL_AUTO, cal[0]);
#else
    for (i = 0; i < 4; i++) {
        JR2_WR(VTSS_QSYS_CALCFG_CAL_AUTO(i), cal[i]);
    }
#endif /* VTSS_ARCH_SERVAL_T */

#if defined(VTSS_ARCH_JAGUAR_2_C) || defined(VTSS_ARCH_SERVAL_T)
    JR2_WRM(VTSS_QSYS_CALCFG_CAL_CTRL,
            VTSS_F_QSYS_CALCFG_CAL_CTRL_CAL_AUTO_GRANT_RATE(671), /* 672->671, BZ19678 */
            VTSS_M_QSYS_CALCFG_CAL_CTRL_CAL_AUTO_GRANT_RATE);
#endif /* (VTSS_ARCH_JAGUAR_2_C) || (VTSS_ARCH_SERVAL_T) */

    JR2_WRM(VTSS_QSYS_CALCFG_CAL_CTRL,
            VTSS_F_QSYS_CALCFG_CAL_CTRL_CAL_MODE(8),
            VTSS_M_QSYS_CALCFG_CAL_CTRL_CAL_MODE);

    /* Verify successful calender config */
    JR2_RD(VTSS_QSYS_CALCFG_CAL_CTRL, &value);
    if (VTSS_X_QSYS_CALCFG_CAL_CTRL_CAL_AUTO_ERROR(value)) {
        VTSS_E("Calender auto error");
    }              
    return VTSS_RC_OK;
 }

#if defined(VTSS_ARCH_JAGUAR_2_B)
// Auto calender does not supports rates close to 80Gb.  Those must be mannually configured.
static vtss_rc jr2_calender_set(vtss_state_t *vtss_state)
{
    u32 cal_length = 335, i;
    u8 *cal_p;
   
    /* Calendar of length 335 found for configuration "TOP_CFG=10" (4x10G 16x2.5G 1x1G): (from TOE) */    
    u8 cal_1[] = { 
        63,
        51,49,50,52,15,19,14,23, 51,49,50,52,13,22,12,21,    51,49,50,52,18,11,17,10,20, 51,49,50,52,9,16,8,48,
        51,49,50,52,15,19,14,23, 51,49,50,52,13,22,12,21,56, 51,49,50,52,18,11,17,10,    51,49,50,52,9,16,20,8,54,
        51,49,50,52,15,19,14,23, 51,49,50,52,13,22,12,63,    51,49,50,52,18,11,21,10,20, 51,49,50,52,17,9,16,8,
        51,49,50,52,15,19,14,23, 51,49,50,52,13,22,12,21,48, 51,49,50,52,18,11,17,10,    51,49,50,52,9,16,20,8,53,
        51,49,50,52,15,19,14,23, 51,49,50,52,13,22,12,21,    51,49,50,52,18,11,17,10,20, 51,49,50,52,9,16,8,63,
        51,49,50,52,15,19,14,23, 51,49,50,52,13,22,12,21,56, 51,49,50,52,18,11,17,10,    51,49,50,52,9,16,20,8,48,
        51,49,50,52,15,19,14,23, 51,49,50,52,13,22,12,21,    51,49,50,52,18,11,17,10,20, 51,49,50,52,9,16,8,54,
        51,49,50,52,15,19,14,23, 51,49,50,52,13,22,12,21,63, 51,49,50,52,18,11,17,10,    51,49,50,52,9,16,20,8,48,
        51,49,50,52,15,19,14,23, 51,49,50,52,13,22,12,21,    51,49,50,52,18,11,17,10,20, 51,49,50,52,9,16,8,56,
        51,49,50,52,15,19,14,23, 51,49,50,52,13,22,12,21,53, 51,49,50,52,18,11,17,10,    51,49,50,52,9,16,20,8};

#if !defined(AQR_CHIP_CU_PHY)
    /* Calendar of length 335 found for configuration "TOP_CFG=11" (2x10G 24x2.5G 1x1G): (from TOE) */
    u8 cal_2[] = {
        63,
        49,50,15,19,14,31,13,23, 49,50,27,12,30,11,18,10,    49,50,26,22,25,17,9,21,29, 49,50,24,20,28,16,8,48,
        49,50,15,19,14,31,13,23, 49,50,27,12,30,11,18,10,56, 49,50,26,22,25,17,9,21,    49,50,24,29,20,28,16,8,54,
        49,50,15,19,14,31,13,23, 49,50,27,12,30,11,18,63,    49,50,10,26,22,25,17,9,21, 49,50,24,29,20,28,16,8,
        49,50,15,19,14,31,13,23, 49,50,27,12,30,11,18,10,48, 49,50,26,22,25,17,9,21,    49,50,24,29,20,28,16,8,53,
        49,50,15,19,14,31,13,23, 49,50,27,12,30,11,18,10,    49,50,26,22,25,17,9,21,29, 49,50,24,20,28,16,8,63,
        49,50,15,19,14,31,13,23, 49,50,27,12,30,11,18,10,56, 49,50,26,22,25,17,9,21,    49,50,24,29,20,28,16,8,48,
        49,50,15,19,14,31,13,23, 49,50,27,12,30,11,18,10,    49,50,26,22,25,17,9,21,29, 49,50,24,20,28,16,8,54,
        49,50,15,19,14,31,13,23, 49,50,27,12,30,11,18,10,63, 49,50,26,22,25,17,9,21,    49,50,24,29,20,28,16,8,48,
        49,50,15,19,14,31,13,23, 49,50,27,12,30,11,18,10,    49,50,26,22,25,17,9,21,29, 49,50,24,20,28,16,8,56,
        49,50,15,19,14,31,13,23, 49,50,27,12,30,11,18,10,53, 49,50,26,22,25,17,9,21,    49,50,24,29,20,28,16,8};
#else
    u8 cal_2[] = {
        63,
        51,52,15,19,14,31,13,23, 51,52,27,12,30,11,18,10,    51,52,26,22,25,17,9,21,29, 51,52,24,20,28,16,8,48,
        51,52,15,19,14,31,13,23, 51,52,27,12,30,11,18,10,56, 51,52,26,22,25,17,9,21,    51,52,24,29,20,28,16,8,54,
        51,52,15,19,14,31,13,23, 51,52,27,12,30,11,18,63,    51,52,10,26,22,25,17,9,21, 51,52,24,29,20,28,16,8,
        51,52,15,19,14,31,13,23, 51,52,27,12,30,11,18,10,48, 51,52,26,22,25,17,9,21,    51,52,24,29,20,28,16,8,53,
        51,52,15,19,14,31,13,23, 51,52,27,12,30,11,18,10,    51,52,26,22,25,17,9,21,29, 51,52,24,20,28,16,8,63,
        51,52,15,19,14,31,13,23, 51,52,27,12,30,11,18,10,56, 51,52,26,22,25,17,9,21,    51,52,24,29,20,28,16,8,48,
        51,52,15,19,14,31,13,23, 51,52,27,12,30,11,18,10,    51,52,26,22,25,17,9,21,29, 51,52,24,20,28,16,8,54,
        51,52,15,19,14,31,13,23, 51,52,27,12,30,11,18,10,63, 51,52,26,22,25,17,9,21,    51,52,24,29,20,28,16,8,48,
        51,52,15,19,14,31,13,23, 51,52,27,12,30,11,18,10,    51,52,26,22,25,17,9,21,29, 51,52,24,20,28,16,8,56,
        51,52,15,19,14,31,13,23, 51,52,27,12,30,11,18,10,53, 51,52,26,22,25,17,9,21,    51,52,24,29,20,28,16,8};
#endif

    if (vtss_state->init_conf.mux_mode == VTSS_PORT_MUX_MODE_0) {
        VTSS_I("4x10 + 16x2.5 + NPI is configured for calender (VTSS_PORT_MUX_MODE_0)");
        cal_p = cal_1; // VTSS_PORT_MUX_MODE_0, 4x10 + 16x2.5 + NPI
    } else {
        VTSS_I("2x10 + 24x2.5 + NPI is configured for calender (VTSS_PORT_MUX_MODE_1)");
        cal_p = cal_2; // VTSS_PORT_MUX_MODE_1, 2x10 + 24x2.5 + NPI
    }

    /* Halt the calender + allow idle usage to Virtual ports */
    JR2_WRM(VTSS_QSYS_CALCFG_CAL_CTRL,
            VTSS_F_QSYS_CALCFG_CAL_CTRL_CAL_MODE(10) |
            VTSS_F_QSYS_CALCFG_CAL_CTRL_CAL_VD_USE_IDLE_ENA(1),
            VTSS_M_QSYS_CALCFG_CAL_CTRL_CAL_MODE |
            VTSS_M_QSYS_CALCFG_CAL_CTRL_CAL_VD_USE_IDLE_ENA);

    /* Enable calender update */
    JR2_WRM(VTSS_QSYS_CALCFG_CAL_SEQ,
            VTSS_F_QSYS_CALCFG_CAL_SEQ_CAL_SEQ_PGM_ENA(1),
            VTSS_M_QSYS_CALCFG_CAL_SEQ_CAL_SEQ_PGM_ENA);

    VTSS_NSLEEP(100);

    /* Update according to the above table */
    for (i = 0; i < cal_length; i++) {
        JR2_WRM(VTSS_QSYS_CALCFG_CAL_SEQ,
                VTSS_F_QSYS_CALCFG_CAL_SEQ_CAL_SEQ_LEN(i) |
                VTSS_F_QSYS_CALCFG_CAL_SEQ_CAL_SEQ_PGM_VAL(cal_p[i]),
                VTSS_M_QSYS_CALCFG_CAL_SEQ_CAL_SEQ_LEN |
                VTSS_M_QSYS_CALCFG_CAL_SEQ_CAL_SEQ_PGM_VAL);

        VTSS_NSLEEP(100);
    }

     /* Press start */
    JR2_WRM(VTSS_QSYS_CALCFG_CAL_CTRL,
            VTSS_F_QSYS_CALCFG_CAL_CTRL_CAL_MODE(9),
            VTSS_M_QSYS_CALCFG_CAL_CTRL_CAL_MODE);

    return VTSS_RC_OK;
}
#endif /* VTSS_ARCH_JAGUAR_2_B */

#if defined(VTSS_ARCH_JAGUAR_2_C)

static vtss_rc jr2_calender_set(vtss_state_t *vtss_state)
{
    u32 cal_length = 373, i;
    u8 *cal_p;

    /* Calendar (from TOE) of length 337 for 4x10G + 48x1G + NPI + CPU0(53)=750Mb + CPU1(54)=250Mb + VD0(55)/VD1(56)=1G.  */
    u8 cal_3[] = {63,
                  51,49,50,52,15,35,39,47,38,51,49,50,52,46,34,14,33,37,51,49,50,52,13,45,48,36,12,
                  51,49,50,52,44,32,43,23,27,7,51,49,50,52,3,19,31,11,22,51,49,50,52,42,6,26,2,30,
                  51,49,50,52,18,10,5,25,17,9,51,49,50,52,29,21,41,1,8,51,49,50,52,55,4,0,28,56,
                  51,49,50,52,40,16,20,53,24,63,51,49,50,52,15,35,39,47,38,51,49,50,52,46,34,14,33,37,
                  51,49,50,52,13,45,48,36,12,54,51,49,50,52,44,32,43,23,27,51,49,50,52,7,3,19,31,11,
                  51,49,50,52,42,22,6,26,2,30,51,49,50,52,18,10,5,25,9,51,49,50,52,17,21,29,41,1,
                  51,49,50,52,8,55,4,0,28,56,51,49,50,52,40,16,20,53,63,51,49,50,52,15,35,39,47,38,
                  51,49,50,52,46,34,14,33,37,24,51,49,50,52,13,45,48,36,12,51,49,50,52,44,32,43,23,27,
                  51,49,50,52,7,3,19,31,11,22,51,49,50,52,42,6,26,2,30,51,49,50,52,18,10,5,25,9,
                  51,49,50,52,17,21,29,41,1,8,51,49,50,52,55,4,0,28,56,51,49,50,52,40,16,20,53,63,
                  51,49,50,52,15,35,39,47,38,24,51,49,50,52,46,34,14,33,37,51,49,50,52,13,45,48,36,12,
                  51,49,50,52,44,32,43,23,27,7,51,49,50,52,3,19,31,11,22,51,49,50,52,42,6,26,2,30,
                  51,49,50,52,18,10,5,25,17,9,51,49,50,52,29,21,41,1,8,51,49,50,52,55,4,0,28,
                  56,51,49,50,52,40,16,20,53,24};

    if (vtss_state->init_conf.mux_mode == VTSS_PORT_MUX_MODE_0) {
        VTSS_E("Mux mode 0 not supported");
        return VTSS_RC_ERROR;
    } else if (vtss_state->init_conf.mux_mode == VTSS_PORT_MUX_MODE_1) {
        VTSS_E("Mux mode 1 not supported");
        return VTSS_RC_ERROR;
    } else {
        VTSS_I("4x10 + 48x1 + NPI is configured for calender (VTSS_PORT_MUX_MODE_2)");
        cal_p = cal_3; // VTSS_PORT_MUX_MODE_2, 4x10 + 48x1 + NPI
    }

    /* Halt the calender + allow idle usage to Virtual ports */
    JR2_WRM(VTSS_QSYS_CALCFG_CAL_CTRL,
            VTSS_F_QSYS_CALCFG_CAL_CTRL_CAL_MODE(10),
            VTSS_M_QSYS_CALCFG_CAL_CTRL_CAL_MODE);
    /* Enable calender update */
    JR2_WRM(VTSS_QSYS_CALCFG_CAL_SEQ,
            VTSS_F_QSYS_CALCFG_CAL_SEQ_CAL_SEQ_PGM_ENA(1),
            VTSS_M_QSYS_CALCFG_CAL_SEQ_CAL_SEQ_PGM_ENA);

    VTSS_NSLEEP(100);

    /* Update according to the above table */
    for (i = 0; i < cal_length; i++) {
        JR2_WRM(VTSS_QSYS_CALCFG_CAL_SEQ,
                VTSS_F_QSYS_CALCFG_CAL_SEQ_CAL_SEQ_LEN(i) |
                VTSS_F_QSYS_CALCFG_CAL_SEQ_CAL_SEQ_PGM_VAL(cal_p[i]),
                VTSS_M_QSYS_CALCFG_CAL_SEQ_CAL_SEQ_LEN |
                VTSS_M_QSYS_CALCFG_CAL_SEQ_CAL_SEQ_PGM_VAL);

        VTSS_NSLEEP(100);
    }

     /* Press start */
    JR2_WRM(VTSS_QSYS_CALCFG_CAL_CTRL,
            VTSS_F_QSYS_CALCFG_CAL_CTRL_CAL_MODE(9),
            VTSS_M_QSYS_CALCFG_CAL_CTRL_CAL_MODE);

    return VTSS_RC_OK;
}
#endif /* VTSS_ARCH_JAGUAR_2_C */


static vtss_rc jr2_port_map_set(vtss_state_t *vtss_state)
{
    if (vtss_state->init_conf.mux_mode == VTSS_PORT_MUX_MODE_AUTO) {
        VTSS_RC(jr2_calender_auto(vtss_state));
    } else {
#if defined(VTSS_ARCH_JAGUAR_2_B) || defined(VTSS_ARCH_JAGUAR_2_C)
        VTSS_RC(jr2_calender_set(vtss_state));
#endif /* VTSS_ARCH_JAGUAR_2_B || VTSS_ARCH_JAGUAR_2_C*/
    }
    return vtss_jr2_init_groups(vtss_state, VTSS_INIT_CMD_PORT_MAP);
}   

#define JR2_API_VERSION 1

static vtss_rc jr2_restart_conf_set(vtss_state_t *vtss_state)
{
    // JR2-TBD: Stub
    return VTSS_RC_OK;
}

static vtss_rc jr2_init_conf_set(vtss_state_t *vtss_state)
{
    /*lint -esym(459, vtss_jr2_rd, vtss_jr2_wr) */
    u32 value, pending, j, i;

    // Note; by design - all gazwrap init registers have the same field layout
    struct {
        BOOL gazwrap;
        u32  init_reg;
        u32  init_val;
    } r, 
              ram_init_list[] = {
            {FALSE, VTSS_ANA_AC_STAT_GLOBAL_CFG_PORT_STAT_RESET, VTSS_M_ANA_AC_STAT_GLOBAL_CFG_PORT_STAT_RESET_RESET},
            {FALSE, VTSS_ASM_CFG_STAT_CFG, VTSS_M_ASM_CFG_STAT_CFG_STAT_CNT_CLR_SHOT},
            {TRUE,  VTSS_QSYS_RAM_CTRL_RAM_INIT, 0},
            {TRUE,  VTSS_REW_RAM_CTRL_RAM_INIT, 0},
            {TRUE,  VTSS_VOP_RAM_CTRL_RAM_INIT, 0},
            {TRUE,  VTSS_ANA_AC_RAM_CTRL_RAM_INIT, 0},
            {TRUE,  VTSS_ASM_RAM_CTRL_RAM_INIT, 0},
            {TRUE,  VTSS_DSM_RAM_CTRL_RAM_INIT, 0}};

#define INIT_CNT (sizeof(ram_init_list)/sizeof(ram_init_list[0]))
    
    vtss_jr2_rd = jr2_rd_direct;
    vtss_jr2_wr = jr2_wr_direct;
    /* Reset switch core only */
    VTSS_I("resetting switch core");
    JR2_WR(VTSS_ICPU_CFG_CPU_SYSTEM_CTRL_RESET, VTSS_M_ICPU_CFG_CPU_SYSTEM_CTRL_RESET_CORE_RST_PROTECT);
    JR2_WR(VTSS_DEVCPU_GCB_CHIP_REGS_SOFT_RST, VTSS_M_DEVCPU_GCB_CHIP_REGS_SOFT_RST_SOFT_SWC_RST);
    for (i=0; ; i++) {
        JR2_RD(VTSS_DEVCPU_GCB_CHIP_REGS_SOFT_RST, &value);
        if (value & VTSS_M_DEVCPU_GCB_CHIP_REGS_SOFT_RST_SOFT_SWC_RST) {
            VTSS_MSLEEP(1);
        }
        else {
            break;
        }
        if (i == 100) {
            VTSS_E("switch core reset failed");
            return VTSS_RC_ERROR;
        }
    }
    VTSS_I("reset switch core done");

    /* Read chip ID to check CPU interface */
    VTSS_RC(vtss_jr2_chip_id_get(vtss_state, &vtss_state->misc.chip_id));
    VTSS_I("chip_id: 0x%04x, revision: 0x%04x",
           vtss_state->misc.chip_id.part_number, vtss_state->misc.chip_id.revision);
#if !defined(VTSS_ARCH_SERVAL_T)
    vtss_state->misc.jr2_a = (vtss_state->misc.chip_id.revision == 0 ? 1 : 0);
#endif /* VTSS_ARCH_SERVAL_T */

#if defined(VTSS_ARCH_JAGUAR_2_C)
    if (vtss_state->misc.chip_id.revision != 2) {
        VTSS_E("The chip architechture (JR-RevC) does not match with the chip revision (rev %d)",vtss_state->misc.chip_id.revision);
        return VTSS_RC_ERROR;
    }
#endif /* VTSS_ARCH_JAGUAR_2_C */

#if defined(VTSS_ARCH_JAGUAR_2_B)
    if (vtss_state->misc.chip_id.revision > 1) {
        VTSS_E("The chip architechture (JR-RevB) does not match with the chip revision (rev %d)",vtss_state->misc.chip_id.revision);
        return VTSS_RC_ERROR;
    }
#endif /* VTSS_ARCH_JAGUAR_2_B */


    /* Initialize memories, if not done already */
    JR2_RD(VTSS_QSYS_SYSTEM_RESET_CFG, &value);

    if (!(value & VTSS_M_QSYS_SYSTEM_RESET_CFG_CORE_ENA)) {
        for (i=0; i<10; i++) {
            pending=INIT_CNT;
            for (j=0; j<INIT_CNT; j++) {
                r = ram_init_list[j];
                if (r.gazwrap) r.init_val=VTSS_M_QSYS_RAM_CTRL_RAM_INIT_RAM_INIT | VTSS_M_QSYS_RAM_CTRL_RAM_INIT_RAM_ENA;
            
                if (i==0) {
                    JR2_WRM_SET(r.init_reg, r.init_val);
                } else {
                    JR2_RD(r.init_reg, &value);
                    if ((value & r.init_val) != r.init_val) {
                        pending--;
                    }
                }
            }
            if (!pending)
                break;
            VTSS_MSLEEP(1);
        }
        
        if (pending>0) {
            /* Still initializing, should be complete in less than 1ms */
            VTSS_E("Memory initialization error");
            return VTSS_RC_ERROR;
        }
    }
   
    JR2_WRM(VTSS_HSIO_PLL5G_CFG_PLL5G_CFG4(0), 
            VTSS_F_HSIO_PLL5G_CFG_PLL5G_CFG4_IB_CTRL(0x7600),
            VTSS_M_HSIO_PLL5G_CFG_PLL5G_CFG4_IB_CTRL);

    JR2_WRM(VTSS_HSIO_PLL5G_CFG_PLL5G_CFG4(1), 
            VTSS_F_HSIO_PLL5G_CFG_PLL5G_CFG4_IB_CTRL(0x7600),
            VTSS_M_HSIO_PLL5G_CFG_PLL5G_CFG4_IB_CTRL);

    // As according to TN1052 update 13/5/2014
    JR2_WRM(VTSS_HSIO_PLL5G_CFG_PLL5G_CFG0(0), 
            VTSS_F_HSIO_PLL5G_CFG_PLL5G_CFG0_ENA_VCO_CONTRH(0),
            VTSS_M_HSIO_PLL5G_CFG_PLL5G_CFG0_ENA_VCO_CONTRH);

    JR2_WRM(VTSS_HSIO_PLL5G_CFG_PLL5G_CFG0(1), 
            VTSS_F_HSIO_PLL5G_CFG_PLL5G_CFG0_ENA_VCO_CONTRH(0),
            VTSS_M_HSIO_PLL5G_CFG_PLL5G_CFG0_ENA_VCO_CONTRH);

#if defined(VTSS_ARCH_JAGUAR_2_C)
    if (vtss_state->create.target == VTSS_TARGET_SPARX_IV_90) {
        // Configuring core clock to run 278MHz
        for(i = 0;i < 2; i++) {
            JR2_WRM(VTSS_HSIO_PLL5G_CFG_PLL5G_CFG0(i),
                    VTSS_F_HSIO_PLL5G_CFG_PLL5G_CFG0_CORE_CLK_DIV(3),
                    VTSS_M_HSIO_PLL5G_CFG_PLL5G_CFG0_CORE_CLK_DIV);
        }

        // Adapt other blocks to the new core speed (TN1267, Chapter 2.2)
        JR2_WRM(VTSS_ANA_AC_POL_COMMON_SDLB_DLB_CTRL,
                VTSS_F_ANA_AC_POL_COMMON_SDLB_DLB_CTRL_CLK_PERIOD_01NS(36),
                VTSS_M_ANA_AC_POL_COMMON_SDLB_DLB_CTRL_CLK_PERIOD_01NS);

        JR2_WRM(VTSS_ANA_AC_POL_COMMON_BDLB_DLB_CTRL,
                VTSS_F_ANA_AC_POL_COMMON_BDLB_DLB_CTRL_CLK_PERIOD_01NS(36),
                VTSS_M_ANA_AC_POL_COMMON_BDLB_DLB_CTRL_CLK_PERIOD_01NS);

        JR2_WRM(VTSS_ANA_AC_POL_COMMON_BUM_SLB_DLB_CTRL,
                VTSS_F_ANA_AC_POL_COMMON_BUM_SLB_DLB_CTRL_CLK_PERIOD_01NS(36),
                VTSS_M_ANA_AC_POL_COMMON_BUM_SLB_DLB_CTRL_CLK_PERIOD_01NS);

        JR2_WRM(VTSS_ANA_AC_POL_POL_ALL_CFG_POL_UPD_INT_CFG,
                VTSS_F_ANA_AC_POL_POL_ALL_CFG_POL_UPD_INT_CFG_POL_UPD_INT(693),
                VTSS_M_ANA_AC_POL_POL_ALL_CFG_POL_UPD_INT_CFG_POL_UPD_INT);

        JR2_WRM(VTSS_LRN_COMMON_AUTOAGE_CFG_1,
                VTSS_F_LRN_COMMON_AUTOAGE_CFG_1_CLK_PERIOD_01NS(36),
                VTSS_M_LRN_COMMON_AUTOAGE_CFG_1_CLK_PERIOD_01NS);

        for(i = 0;i < 2; i++) {
            JR2_WRM(VTSS_DEVCPU_GCB_SIO_CTRL_SIO_CLOCK(i),
                    VTSS_F_DEVCPU_GCB_SIO_CTRL_SIO_CLOCK_SYS_CLK_PERIOD(36),
                    VTSS_M_DEVCPU_GCB_SIO_CTRL_SIO_CLOCK_SYS_CLK_PERIOD);
        }
        
        // VTSS_DEVCPU_GCB_FAN_CTRL_FAN_CFG::CLK_PERIOD stays at default

        JR2_WRM(VTSS_DEVCPU_PTP_PTP_CFG_PTP_SYS_CLK_CFG,
                VTSS_F_DEVCPU_PTP_PTP_CFG_PTP_SYS_CLK_CFG_PTP_SYS_CLK_PER_NS(3),
                VTSS_M_DEVCPU_PTP_PTP_CFG_PTP_SYS_CLK_CFG_PTP_SYS_CLK_PER_NS);

        JR2_WRM(VTSS_DEVCPU_PTP_PTP_CFG_PTP_SYS_CLK_CFG,
                VTSS_F_DEVCPU_PTP_PTP_CFG_PTP_SYS_CLK_CFG_PTP_SYS_CLK_PER_PS100(6),
                VTSS_M_DEVCPU_PTP_PTP_CFG_PTP_SYS_CLK_CFG_PTP_SYS_CLK_PER_PS100);

        JR2_WRM(VTSS_HSCH_HSCH_MISC_SYS_CLK_PER,
                VTSS_F_HSCH_HSCH_MISC_SYS_CLK_PER_SYS_CLK_PER_100PS(36),
                VTSS_M_HSCH_HSCH_MISC_SYS_CLK_PER_SYS_CLK_PER_100PS);

        JR2_WRM(VTSS_VOP_COMMON_LOC_CTRL,
                VTSS_F_VOP_COMMON_LOC_CTRL_LOC_BASE_TICK_CNT(28), // 100/3.6
                VTSS_M_VOP_COMMON_LOC_CTRL_LOC_BASE_TICK_CNT);

        JR2_WRM(VTSS_AFI_TTI_TICKS_TTI_TICK_BASE,
                VTSS_F_AFI_TTI_TICKS_TTI_TICK_BASE_BASE_LEN(14444), // 52us/3.6ns
                VTSS_M_AFI_TTI_TICKS_TTI_TICK_BASE_BASE_LEN);
    }
#endif /*  VTSS_ARCH_JAGUAR_2_C */
                
    // Enable 250 MHz CLKOUT2 for 1588
    JR2_WRM(VTSS_HSIO_PLL5G_CFG_PLL5G_CFG3(1), 
            VTSS_F_HSIO_PLL5G_CFG_PLL5G_CFG3_CLKOUT2_SEL(3),
            VTSS_M_HSIO_PLL5G_CFG_PLL5G_CFG3_CLKOUT2_SEL);

    /* Enable queue system */
    JR2_WR(VTSS_QSYS_SYSTEM_RESET_CFG, VTSS_F_QSYS_SYSTEM_RESET_CFG_CORE_ENA(1));
    for (i=VTSS_CHIP_PORTS; i<VTSS_CHIP_PORTS_ALL; i++)
        JR2_WRM_SET(VTSS_QFWD_SYSTEM_SWITCH_PORT_MODE(i),VTSS_M_QFWD_SYSTEM_SWITCH_PORT_MODE_PORT_ENA);


    /* Set ASM/DSM watermarks for cpu traffic */
    JR2_WRM(VTSS_DSM_CFG_SCH_STOP_WM_CFG(VTSS_CHIP_PORT_CPU_0),
            VTSS_F_DSM_CFG_SCH_STOP_WM_CFG_SCH_STOP_WM(64),
            VTSS_M_DSM_CFG_SCH_STOP_WM_CFG_SCH_STOP_WM);

    JR2_WRM(VTSS_DSM_CFG_SCH_STOP_WM_CFG(VTSS_CHIP_PORT_CPU_1),
            VTSS_F_DSM_CFG_SCH_STOP_WM_CFG_SCH_STOP_WM(64),
            VTSS_M_DSM_CFG_SCH_STOP_WM_CFG_SCH_STOP_WM);

    JR2_WRM(VTSS_ASM_CFG_CPU_FC_CFG,
            VTSS_F_ASM_CFG_CPU_FC_CFG_CPU_FC_WM(2)| VTSS_F_ASM_CFG_CPU_FC_CFG_CPU_FC_ENA(1),
            VTSS_M_ASM_CFG_CPU_FC_CFG_CPU_FC_WM   | VTSS_M_ASM_CFG_CPU_FC_CFG_CPU_FC_ENA);

    JR2_RD(VTSS_ICPU_CFG_CPU_SYSTEM_CTRL_GENERAL_STAT, &value);
    vtss_state->sys_config.vcore_cfg = VTSS_X_ICPU_CFG_CPU_SYSTEM_CTRL_GENERAL_STAT_VCORE_CFG(value);

    /* See datasheet, Table 253 VCore-III Configurations,
       vcore_cfg == 14 added as a special case */

    vtss_state->sys_config.using_vcoreiii = 
        (vtss_state->sys_config.vcore_cfg == 0 || vtss_state->sys_config.vcore_cfg == 12);
#if defined(VTSS_OPT_VRAP_ACCESS)
    vtss_state->sys_config.using_vrap = (vtss_state->sys_config.vcore_cfg >= 1 &&
                                         vtss_state->sys_config.vcore_cfg <= 8);
#else
    vtss_state->sys_config.using_vrap = 0;
#endif //VTSS_OPT_VRAP_ACCESS
#if defined(VTSS_OPT_PCIE_ACCESS)
    vtss_state->sys_config.using_pcie = ((vtss_state->sys_config.vcore_cfg >= 1 &&
                                          vtss_state->sys_config.vcore_cfg <= 7) ||
                                          vtss_state->sys_config.vcore_cfg == 9 ||
                                          vtss_state->sys_config.vcore_cfg == 14);
#else
    vtss_state->sys_config.using_pcie = 0;
#endif //VTSS_OPT_PCIe_ACCESS
    VTSS_I("Vcore_cfg: 0x%04x, VCOREIII: %d, VRAP: %d, PCIe: %d", 
           vtss_state->sys_config.vcore_cfg, 
           vtss_state->sys_config.using_vcoreiii,
           vtss_state->sys_config.using_vrap,
           vtss_state->sys_config.using_pcie);

    /* Initialize function groups */
    VTSS_RC(vtss_jr2_init_groups(vtss_state, VTSS_INIT_CMD_INIT));

    return VTSS_RC_OK;
}

vtss_rc vtss_jaguar2_inst_create(vtss_state_t *vtss_state)
{
    /* Initialization */
    vtss_state->cil.init_conf_set = jr2_init_conf_set;
    vtss_state->cil.restart_conf_set = jr2_restart_conf_set;
    vtss_state->cil.debug_info_print = jr2_debug_info_print;
    vtss_state->port.map_set = jr2_port_map_set;

#if defined(VTSS_FEATURE_WIS)
    if (vtss_phy_inst_ewis_create(vtss_state) != VTSS_RC_OK) {
        VTSS_E("vtss_phy_inst_ewis_create() failed");
    }
#endif

    /* Create function groups */
    return vtss_jr2_init_groups(vtss_state, VTSS_INIT_CMD_CREATE);
}

#endif /* VTSS_ARCH_JAGUAR_2 */
