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

#ifndef _VTSS_JAGUAR2_CIL_H_
#define _VTSS_JAGUAR2_CIL_H_

/* Use relative DWORD addresses for registers - must be first */
#define VTSS_IOADDR(t,o) ((((t) - VTSS_IO_ORIGIN1_OFFSET) >> 2) + (o))
#define VTSS_IOREG(t,o)  (VTSS_IOADDR(t,o))

// Avoid "vtss_jaguar2_cil.h not used in module vtss_jaguar2.c"
/*lint --e{766} */

#include "vtss_api.h"

#if defined(VTSS_ARCH_JAGUAR_2)
#define VTSS_TRACE_LAYER VTSS_TRACE_LAYER_CIL
#include "../ail/vtss_state.h"
#include "../ail/vtss_common.h"
#include "../ail/vtss_util.h"
#if defined(VTSS_FEATURE_FDMA) && VTSS_OPT_FDMA
#include "vtss_jaguar2_fdma.h"
#endif
#include "vtss_jaguar2.h"
#include "vtss_jaguar2_reg.h"

#if defined(VTSS_ARCH_SERVAL_T)
#define VTSS_CHIP_PORTS           11 /* Port 0-10 are switch ports */
#define VTSS_CHIP_PORT_2_5G_START 5  /* Port 5-6 support 2.5G */
#define VTSS_PORT_10G_START       9  /* Port 9-10 support 10G */
#define VTSS_CHIP_PORT_NPI        4
#else
#define VTSS_CHIP_PORTS           53 /* Port 0-52 are switch ports */
#define VTSS_CHIP_PORT_2_5G_START 8  /* Port 8-31 support 2.5G */
#define VTSS_PORT_10G_START       49 /* Port 49-52 support 10G */
#define VTSS_CHIP_PORT_NPI        48
#endif /* VTSS_ARCH_SERVAL_T */

#define VTSS_CHIP_PORT_CPU   VTSS_CHIP_PORTS          /* Next port is CPU port */
#define VTSS_CHIP_PORT_CPU_0 (VTSS_CHIP_PORT_CPU + 0) /* CPU Port 11/53 */
#define VTSS_CHIP_PORT_CPU_1 (VTSS_CHIP_PORT_CPU + 1) /* CPU Port 12/54 */
#define VTSS_CHIP_PORT_VD0   (VTSS_CHIP_PORT_CPU + 2) /* Port 13/55, used for IPMC */
#define VTSS_CHIP_PORT_VD1   (VTSS_CHIP_PORT_CPU + 3) /* Port 14/56, used for OAM */
#define VTSS_CHIP_PORTS_ALL  (VTSS_CHIP_PORT_CPU + 4) /* Total 15/57 ports, including CPU/VD ports */
#define VTSS_CHIP_PORT_MASK  VTSS_BITMASK(VTSS_CHIP_PORTS) /* Chip port mask */

#define VTSS_PORT_IS_1G(port)  ((port) < VTSS_PORT_10G_START)
#define VTSS_PORT_IS_10G(port) ((port) >= VTSS_PORT_10G_START)

#define VTSS_TO_DEV(port) (VTSS_PORT_IS_10G(port) ? \
                           (VTSS_TO_DEV10G_0 + ((port)-VTSS_PORT_10G_START) * (VTSS_TO_DEV10G_1 - VTSS_TO_DEV10G_0)) :\
                           (port) < 8 ? (VTSS_TO_DEV1G_0 + (port) * (VTSS_TO_DEV1G_1 - VTSS_TO_DEV1G_0)) :\
                           (port) < 32 ? (VTSS_TO_DEV2G5_0 + ((port)-8) * (VTSS_TO_DEV2G5_1 - VTSS_TO_DEV2G5_0)) :\
                           (port) == 48 ? VTSS_TO_DEV2G5_24 : \
                           (VTSS_TO_DEV1G_0 + ((port)-32) * (VTSS_TO_DEV1G_1 - VTSS_TO_DEV1G_0)))

#define VTSS_TO_DEV1G(port) (VTSS_PORT_IS_10G(port) ? \
                             (VTSS_TO_DEV2G5_25 + (port-VTSS_PORT_10G_START) * (VTSS_TO_DEV2G5_1 - VTSS_TO_DEV2G5_0)) :\
                             (VTSS_TO_DEV(port)))

/* Jaguar 2 has 4 PTP PIN configurations that can be used for different purposes, the defines below defines the
 * default usage of the 4 pins.
 */
/* PIN configuration for external clock */
#define EXT_CLK_PIN 0           /* external clock 1 pps input */
#define EXT_PPS_PIN 1           /* 1pps pulse output */

/* PIN configuration for alternative clock */
#define ALT_LDST_PIN 2          /* alternative clock 1 pps input (Load/Store) */
#define ALT_PPS_PIN  3          /* alternative clock 1pps pulse */



/* Policers */
#define JR2_POLICER_PORT    0    /* 0-11    : Port policers (0-10 used, 11 unused) */
#define JR2_POLICER_ACL     12   /* 12-31   : ACL policers (12-27 used, 28-31 unused) */
#define JR2_POLICER_QUEUE   32   /* 32-127  : Queue policers (32-119 used, 120-127 unused) */
#define JR2_POLICER_EVC     129  /* 129-1150: EVC policers (128 unused) */
#define JR2_POLICER_DISCARD 1151 /* 1151    : Discard policer */
#define JR2_POLICER_CNT     1152 /* Total number of policers */

#if defined(VTSS_CHIP_SERVAL_2) || defined(VTSS_CHIP_SERVAL_2_LITE)
/* Buffer constants for Serval2 */
#define JR2_BUFFER_MEMORY    2096688 /* 11913 words * 176 bytes */
#define JR2_BUFFER_REFERENCE 11913   /* Frame references        */
#define JR2_BUFFER_CELL_SZ   176     /* Cell size               */
#else
/* Buffer constants for JR2/ServalT */
#define JR2_BUFFER_MEMORY    4193376 /* 23826 words * 176 bytes */
#define JR2_BUFFER_REFERENCE 23826   /* Frame references        */
#define JR2_BUFFER_CELL_SZ   176     /* Cell size               */
#endif /* VTSS_CHIP_SERVAL_2) ||  defined(VTSS_CHIP_SERVAL_2_LITE */

/* Number of full entries */
//#define JR2_IS0_CNT 384
//#define JR2_IS1_CNT 256
//#define JR2_IS2_CNT 256
//#define JR2_ES0_CNT 1024

#define JR2_ACS          16   /* Number of aggregation masks */
#define JR2_PRIOS        8    /* Number of priorities */
#define JR2_GPIOS        64   /* Number of GPIOs */
#define JR2_SGPIO_GROUPS 3    /* Number of SGPIO groups */
#define JR2_EVC_CNT      1024 /* Number of EVCs */

/* MC PGIDs */
#define PGID_UC_FLOOD     53
#define PGID_MC_FLOOD     54
#define PGID_IPV4_MC_DATA 55
#define PGID_IPV4_MC_CTRL 56
#define PGID_IPV6_MC_DATA 57
#define PGID_IPV6_MC_CTRL 58
#define PGID_DROP         59
#define PGID_FLOOD        60

/* GLAG PGID */
#define PGID_GLAG_START   821

/* Mirror probe usage */
#define JR2_MIRROR_PROBE_RX   0 /* Ingress mirror probe */
#define JR2_MIRROR_PROBE_TX   1 /* Egress mirror probe */
#define JR2_MIRROR_PROBE_VLAN 2 /* VLAN mirror probe */

/* Indices into QFWD:SYSTEM:FRAME_COPY_CFG */
#define QFWD_FRAME_COPY_CFG_CPU_QU(qu)              (qu)
#define QFWD_FRAME_COPY_CFG_LRN_ALL                 8
#define QFWD_FRAME_COPY_CFG_MIRROR_PROBE(probe_idx) (probe_idx + 9)

#define ANA_AC_SRC_GLAG_OFFSET 57 /* Offset of GLAG src masks */

/* sFlow H/W-related min/max */
#define JR2_SFLOW_MIN_SAMPLE_RATE       1 /**< Minimum allowable sampling rate for sFlow */
#define JR2_SFLOW_MAX_SAMPLE_RATE   32767 /**< Maximum allowable sampling rate for sFlow */

/* One bit used to signal ACL hit in IFH */
#define JR2_IFH_CL_RSLT 0x0001

#define JR2_DIV_ROUND_UP(x, base) ((x + base - 1)/base)

typedef struct {
    BOOL frame_rate; /* Enable frame rate policing (always single bucket) */
    BOOL dual;       /* Enable dual leaky bucket mode */
    BOOL data_rate;  /* Enable data rate policing */
    u32  cir;        /* CIR in kbps/fps (ignored in single bucket mode) */
    u32  cbs;        /* CBS in bytes/frames (ignored in single bucket mode) */
    u32  eir;        /* EIR (PIR) in kbps/fps */
    u32  ebs;        /* EBS (PBS) in bytes/frames */
    BOOL cf;         /* Coupling flag (ignored in single bucket mode) */
} vtss_jr2_policer_conf_t;

/* ================================================================= *
 *  Register access
 * ================================================================= */
extern vtss_rc (*vtss_jr2_wr)(vtss_state_t *vtss_state, u32 addr, u32 value);
extern vtss_rc (*vtss_jr2_rd)(vtss_state_t *vtss_state, u32 addr, u32 *value);
vtss_rc vtss_jr2_wrm(vtss_state_t *vtss_state, u32 reg, u32 value, u32 mask);

#define JR2_RD(p, value)                 \
    {                                     \
        vtss_rc __rc = vtss_jr2_rd(vtss_state, p, value);    \
        if (__rc != VTSS_RC_OK)           \
            return __rc;                  \
    }

#define JR2_WR(p, value)                 \
    {                                     \
        vtss_rc __rc = vtss_jr2_wr(vtss_state, p, value);    \
        if (__rc != VTSS_RC_OK)           \
            return __rc;                  \
    }

#define JR2_WRM(p, value, mask)                 \
    {                                            \
        vtss_rc __rc = vtss_jr2_wrm(vtss_state, p, value, mask);        \
        if (__rc != VTSS_RC_OK)                  \
            return __rc;                         \
    }

#define JR2_WRM_SET(p, mask) JR2_WRM(p, mask, mask)
#define JR2_WRM_CLR(p, mask) JR2_WRM(p, 0,    mask)
#define JR2_WRM_CTL(p, _cond_, mask) JR2_WRM(p, (_cond_) ? mask : 0, mask)

/* Decode register bit field */
#define JR2_BF(name, value) ((VTSS_M_##name & value) ? 1 : 0)

// Macros for writing/reading port masks with naming as specified in TN1160, "Multi-Register Bit Vector"
#define JR2_WR_PMASK(tgt_addr, val64)                                   \
    {                                                                   \
        JR2_WR(tgt_addr   , (val64 & 0xffffffff));                      \
        JR2_WR(tgt_addr##1, (val64 >> 32));                             \
    }
#define JR2_WRM_PMASK(tgt_addr, val64, mask64)                            \
    {                                                                   \
        JR2_WRM(tgt_addr   , (val64 & 0xffffffff), mask64 & 0xffffffff);  \
        JR2_WRM(tgt_addr##1, (val64 >> 32),        mask64 >> 32);         \
    }

#define JR2_RD_PMASK(tgt_addr, val64)         \
    {                                         \
        u32 __val32__;                        \
        JR2_RD(tgt_addr, &__val32__);         \
        *val64 = __val32__;                   \
        JR2_RD(tgt_addr##1, &__val32__);      \
        *val64 |= (((u64)__val32__) << 32);   \
    }

// Avoid 'Excessive shift value (precision 0 shifted right by 32)' which may occur if val64 is a constant < 2^32
/*lint -emacro(572,JR2_WRX_PMASK) */
#define JR2_WRX_PMASK(tgt_addr, x, val64)               \
    {                                                   \
        JR2_WR(tgt_addr(x),    ((val64) & 0xffffffff)); \
        JR2_WR(tgt_addr##1(x), ((val64) >> 32));        \
    }

#define JR2_RDX_PMASK(tgt_addr, x, val64)               \
    {                                                   \
        u32 __val32__;                                  \
        JR2_RD(tgt_addr(x),    &__val32__);             \
        *val64 = __val32__;                             \
        JR2_RD(tgt_addr##1(x), &__val32__);             \
        *val64 |= (((u64)__val32__) << 32);             \
    }

#define JR2_WRXM_PMASK(tgt_addr, x, val64, mask64)                          \
    {                                                                       \
        JR2_WRM(tgt_addr(x),    (val64 & 0xffffffff), mask64 & 0xffffffff); \
        JR2_WRM(tgt_addr##1(x), (val64 >> 32),        mask64 >> 32);        \
    }

#define JR2_DEBUG_REG(pr, tgt, addr)                vtss_jr2_debug_reg(vtss_state, pr, VTSS_##tgt##_##addr, #addr)
#define JR2_DEBUG_REG_NAME(pr, tgt, addr, name)     vtss_jr2_debug_reg(vtss_state, pr, VTSS_##tgt##_##addr, name)
#define JR2_DEBUG_REGX_NAME(pr, tgt, addr, x, name) vtss_jr2_debug_reg_inst(vtss_state, pr, VTSS_##tgt##_##addr(x), x, name)

/* Poll bits in mask until all are zero */
#define JR2_POLL_MASK(_p_, _mask_)                                                  \
    {                                                                               \
        u32 _x_, _cnt_ = 0;                                                         \
        do {                                                                        \
            JR2_RD(_p_, &_x_);                                                      \
            VTSS_MSLEEP(1);                                                         \
            if (++_cnt_ == 100) {                                                   \
                VTSS_E("timeout, addr: %s, m: 0x%x, v: 0x%x", #_p_, (_mask_), _x_); \
                return VTSS_RC_ERROR;                                               \
            }                                                                       \
        } while (_x_ & (_mask_));                                                   \
    }

/* Common functions */
vtss_rc vtss_jr2_init_groups(vtss_state_t *vtss_state, vtss_init_cmd_t cmd);
u64 vtss_jr2_pmask(u32 port);
u64 vtss_jr2_port_mask(vtss_state_t *vtss_state, const BOOL member[]);
u32 vtss_jr2_vtss_pgid(const vtss_state_t *const state, u32 pgid);
u32 vtss_jr2_chip_pgid(vtss_state_t *vtss_state, u32 pgid);
// JR2-TBD vtss_rc vtss_jr2_isdx_update_es0(vtss_state_t *vtss_state,
//                                  BOOL isdx_ena, u32 isdx, u32 isdx_mask);
void vtss_jr2_debug_cnt(const vtss_debug_printf_t pr, const char *col1, const char *col2,
                         vtss_chip_counter_t *c1, vtss_chip_counter_t *c2);
void vtss_jr2_debug_reg_header(const vtss_debug_printf_t pr, const char *name);
void vtss_jr2_debug_reg(vtss_state_t *vtss_state,
                         const vtss_debug_printf_t pr, u32 addr, const char *name);
void vtss_jr2_debug_reg_inst(vtss_state_t *vtss_state,
                              const vtss_debug_printf_t pr, u32 addr, u32 i, const char *name);
void vtss_jr2_debug_print_port_header(vtss_state_t *vtss_state,
                                       const vtss_debug_printf_t pr, const char *txt);
void vtss_jr2_debug_print_pmask(const vtss_debug_printf_t pr, u64 pmask);

void vtss_jr2_debug_sticky(vtss_state_t *vtss_state,
                           const vtss_debug_printf_t pr, u32 addr, const char *name);

/* Port functions */
vtss_rc vtss_jr2_port_init(vtss_state_t *vtss_state, vtss_init_cmd_t cmd);
vtss_rc vtss_jr2_port_debug_print(vtss_state_t *vtss_state,
                                   const vtss_debug_printf_t pr,
                                   const vtss_debug_info_t   *const info);
vtss_rc vtss_jr2_port_max_tags_set(vtss_state_t *vtss_state, vtss_port_no_t port_no);
vtss_rc vtss_jr2_counter_update(vtss_state_t *vtss_state,
                                 u32 *addr, vtss_chip_counter_t *counter, BOOL clear);
u32 vtss_jr2_wm_dec(u32 value);
vtss_rc vtss_jr2_wm_update(vtss_state_t *vtss_state);

/* Miscellaneous functions */
vtss_rc vtss_jr2_misc_init(vtss_state_t *vtss_state, vtss_init_cmd_t cmd);
vtss_rc vtss_jr2_chip_id_get(vtss_state_t *vtss_state, vtss_chip_id_t *const chip_id);
vtss_rc vtss_jr2_gpio_mode(vtss_state_t *vtss_state,
                            const vtss_chip_no_t   chip_no,
                            const vtss_gpio_no_t   gpio_no,
                            const vtss_gpio_mode_t mode);
vtss_rc vtss_jr2_misc_debug_print(vtss_state_t *vtss_state,
                                   const vtss_debug_printf_t pr,
                                   const vtss_debug_info_t   *const info);

/* QoS functions */
#if defined(VTSS_FEATURE_QOS)
vtss_rc vtss_jr2_qos_init(vtss_state_t *vtss_state, vtss_init_cmd_t cmd);
vtss_rc vtss_jr2_port_policer_fc_set(vtss_state_t *vtss_state, const vtss_port_no_t port_no, u32 chipport);
vtss_rc vtss_jr2_qos_debug_print(vtss_state_t *vtss_state,
                                  const vtss_debug_printf_t pr,
                                  const vtss_debug_info_t   *const info);
#endif /* VTSS_FEATURE_QOS */

/* L2 functions */
vtss_rc vtss_jr2_l2_init(vtss_state_t *vtss_state, vtss_init_cmd_t cmd);
vtss_rc vtss_jr2_l2_debug_print(vtss_state_t *vtss_state,
                                 const vtss_debug_printf_t pr,
                                 const vtss_debug_info_t   *const info);

/* L3 functions */
vtss_rc vtss_jr2_l3_init(vtss_state_t *vtss_state, vtss_init_cmd_t cmd);
vtss_rc vtss_jr2_l3_debug_print(vtss_state_t *vtss_state,
                                 const vtss_debug_printf_t pr,
                                 const vtss_debug_info_t   *const info);
/* Packet functions */
vtss_rc vtss_jr2_packet_init(vtss_state_t *vtss_state, vtss_init_cmd_t cmd);
vtss_rc vtss_jr2_packet_debug_print(vtss_state_t *vtss_state,
                                     const vtss_debug_printf_t pr,
                                     const vtss_debug_info_t   *const info);

#if defined(VTSS_FEATURE_AFI_SWC)
vtss_rc jr2_afi_pause_resume(vtss_state_t *vtss_state, vtss_port_no_t port_no, BOOL resume);
vtss_rc vtss_jr2_afi_debug_print(vtss_state_t *vtss_state,
                                  const vtss_debug_printf_t pr,
                                  const vtss_debug_info_t   *const info);
#if defined(VTSS_AFI_V2)
vtss_rc vtss_jr2_afi_init(vtss_state_t *const vtss_state, const vtss_init_cmd_t cmd);
#endif /* VTSS_AFI_V2 */
#endif /* VTSS_FEATURE_AFI_SWC */

#if defined(VTSS_FEATURE_MPLS)
/* MPLS functions */
vtss_rc vtss_jr2_mpls_init(vtss_state_t *vtss_state, vtss_init_cmd_t cmd);
vtss_rc vtss_jr2_mpls_debug_print(vtss_state_t *vtss_state,
                                   const vtss_debug_printf_t pr,
                                   const vtss_debug_info_t   *const info);
void vtss_mpls_ece_is1_update(vtss_state_t *vtss_state,
                              vtss_evc_entry_t *evc, vtss_ece_entry_t *ece,
                              vtss_sdx_entry_t *isdx, vtss_is1_key_t *key);
void vtss_mpls_ece_es0_update(vtss_state_t *vtss_state,
                              vtss_evc_entry_t *evc, vtss_ece_entry_t *ece,
                              vtss_sdx_entry_t *esdx, vtss_es0_action_t *action);
vtss_rc vtss_jr2_evc_mpls_update(vtss_evc_id_t evc_id, vtss_res_t *res, vtss_res_cmd_t cmd);
#endif /* VTSS_FEATURE_MPLS */

#if defined(VTSS_FEATURE_OAM)
/* OAM functions */
vtss_rc vtss_jr2_oam_init(vtss_state_t *vtss_state, vtss_init_cmd_t cmd);
vtss_rc vtss_jr2_oam_debug_print(vtss_state_t *vtss_state,
                                  const vtss_debug_printf_t pr,
                                  const vtss_debug_info_t   *const info);
#endif /* VTSS_FEATURE_OAM */

#if defined(VTSS_FEATURE_EVC)
/* EVC functions */
vtss_rc vtss_jr2_evc_init(vtss_state_t *vtss_state, vtss_init_cmd_t cmd);
vtss_vcap_key_size_t vtss_jr2_key_type_to_size(vtss_vcap_key_type_t key_type);
BOOL vtss_jr2_ece_is1_needed(BOOL nni, vtss_ece_dir_t dir, vtss_ece_rule_t rule);
vtss_rc vtss_jr2_ece_update(vtss_state_t *vtss_state,
                             vtss_ece_entry_t *ece, vtss_res_t *res, vtss_res_cmd_t cmd);
vtss_port_no_t vtss_jr2_mce_port_no_get(vtss_state_t *vtss_state,
                                         const BOOL port_list[VTSS_PORT_ARRAY_SIZE],
                                         BOOL port_cpu);
vtss_rc vtss_jr2_mce_is1_add(vtss_state_t *vtss_state, vtss_mce_entry_t *mce);
vtss_rc vtss_jr2_evc_debug_print(vtss_state_t *vtss_state,
                                  const vtss_debug_printf_t pr,
                                  const vtss_debug_info_t   *const info);
#endif /* VTSS_FEATURE_EVC */

/* VCAP functions */
#if defined(VTSS_FEATURE_VCAP)
vtss_rc vtss_jr2_vcap_init(vtss_state_t *vtss_state, vtss_init_cmd_t cmd);
vtss_rc vtss_jr2_vcap_port_key_set(vtss_state_t *vtss_state,
                                    const vtss_port_no_t port_no,
                                    u8 lookup,
                                    vtss_vcap_key_type_t key_new,
                                    vtss_vcap_key_type_t key_old);
vtss_rc vtss_jr2_vcap_debug_print(vtss_state_t *vtss_state,
                                   const vtss_debug_printf_t pr,
                                   const vtss_debug_info_t   *const info);
vtss_rc vtss_jr2_debug_clm_c(vtss_state_t *vtss_state,
                             const vtss_debug_printf_t pr,
                             const vtss_debug_info_t   *const info);
vtss_rc vtss_jr2_debug_es0(vtss_state_t *vtss_state,
                           const vtss_debug_printf_t pr,
                           const vtss_debug_info_t   *const info);
vtss_rc vtss_jr2_debug_lpm(vtss_state_t *vtss_state,
                           const vtss_debug_printf_t pr,
                           const vtss_debug_info_t   *const info);
#endif /* VTSS_FEATURE_VCAP */

#if defined(VTSS_FEATURE_TIMESTAMP)
/* Timestamp functions */
vtss_rc vtss_jr2_ts_init(vtss_state_t *vtss_state, vtss_init_cmd_t cmd);
vtss_rc vtss_jr2_ts_debug_print(vtss_state_t *vtss_state,
                                const vtss_debug_printf_t pr,
                                const vtss_debug_info_t   *const info);
#endif /* VTSS_FEATURE_TIMESTAMP */

#define VTSS_TO_10G_APC_TGT(port)  (VTSS_TO_XGDIG_0 + ((port - VTSS_PORT_10G_START) * (VTSS_TO_XGDIG_1 - VTSS_TO_XGDIG_0)))
#define VTSS_TO_10G_SRD_TGT(port)  (VTSS_TO_XGANA_0 + ((port - VTSS_PORT_10G_START) * (VTSS_TO_XGANA_1 - VTSS_TO_XGANA_0)))
#define VTSS_TO_10G_XFI_TGT(port)  (VTSS_TO_XGXFI_0 + ((port - VTSS_PORT_10G_START) * (VTSS_TO_XGXFI_1 - VTSS_TO_XGXFI_0)))
#define VTSS_TO_10G_PCS_TGT(port)  (VTSS_TO_PCS10G_BR_0 + ((port - VTSS_PORT_10G_START) * (VTSS_TO_PCS10G_BR_1 - VTSS_TO_PCS10G_BR_0)))

/* Serdes functions */
vtss_rc jr2_sd1g_read(vtss_state_t *vtss_state, u32 addr);
vtss_rc jr2_sd1g_write(vtss_state_t *vtss_state, u32 addr, u32 nsec);
vtss_rc jr2_sd6g_read(vtss_state_t *vtss_state, u32 addr);
vtss_rc jr2_sd6g_write(vtss_state_t *vtss_state, u32 addr, u32 nsec);

vtss_rc jr2_sd1g_cfg(vtss_state_t *vtss_state, vtss_serdes_mode_t mode, u32 addr);
vtss_rc jr2_sd6g_cfg(vtss_state_t *vtss_state, vtss_serdes_mode_t mode, u32 addr);
vtss_rc jr2_sd10g_cfg(vtss_state_t *vtss_state, vtss_serdes_mode_t mode, u32 port);

#endif /* VTSS_ARCH_JAGUAR_2 */
#endif /* _VTSS_JAGUAR2_CIL_H_ */
