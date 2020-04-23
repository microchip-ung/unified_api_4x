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

#ifndef _VTSS_HQOS_STATE_H_
#define _VTSS_HQOS_STATE_H_

#if defined(VTSS_FEATURE_HQOS)

#define VTSS_HQOS_SES           256
#define VTSS_HQOS_LVL0_SES      218
#define VTSS_HQOS_UNI_SES        30
#define VTSS_HQOS_NNI_SES        42
#define VTSS_HQOS_MAX_NNI_PORTS   2
#define VTSS_HQOS_MP_SES          5
#define VTSS_HQOS_SE_INPUTS      12

/* HQoS port configuration */
typedef struct {
    vtss_hqos_sch_mode_t sch_mode;      /* Scheduling mode */
    BOOL                 nni_mode;      /* Scheduling system is mapped as NNI */
    u8                   se_base;       /* Base index of level 0 scheduling elements */
    u8                   se_top;        /* Top index of level 0 scheduling elements */
    u8                   lvl1_se_base;  /* Base index of level 1 scheduling elements */
    u8                   lvl1_se_top;   /* Top index of level 1 scheduling elements */
    vtss_bitrate_t       min_rate_calc; /* Calculated non-service minimum bandwidth. Unit: kbps. */
} vtss_hqos_port_info_t;

/* ISDX entry */
typedef struct vtss_hqos_isdx_t {
    struct vtss_hqos_isdx_t *next;   /* Next in list */
    u16                     isdx;    /* ISDX value */
} vtss_hqos_isdx_t;

/* ISDX list */
typedef struct {
    vtss_hqos_isdx_t *free;               /* List of free entries */
    vtss_hqos_isdx_t table[VTSS_SDX_CNT]; /* Table of entries */
} vtss_hqos_isdx_list_t;

/* HQoS entry */
typedef struct vtss_hqos_entry_t {
    BOOL                     enable;        /* HQoS entry valid indication */
    u16                      srv_grp;       /* HQoS service group */
    BOOL                     is_mp;         /* Multi-point indication */
    vtss_hqos_id_t           mp_main;       /* Main multi-point HQoS ID */
    vtss_hqos_isdx_t         *isdx_list;    /* List of mapped ISDXs */
    vtss_hqos_conf_t         conf;          /* HQoS configuration */
    vtss_bitrate_t           min_rate_calc; /* Calculated minimum bandwidth. Unit: kbps. */
} vtss_hqos_entry_t;

/* HQoS information */
typedef struct {
    u32               max_count;              /* Maximum number of rules */
    u32               count;                  /* Actual number of rules */
    vtss_hqos_entry_t table[VTSS_HQOS_COUNT]; /* Entries */
} vtss_hqos_info_t;

typedef struct {
    /* CIL function pointers */
    vtss_rc (* port_conf_set)(struct vtss_state_s *vtss_state,
                              const vtss_port_no_t port_no);
    vtss_rc (* add)(struct vtss_state_s *vtss_state,
                    const vtss_hqos_id_t hqos_id,
                    const vtss_hqos_conf_t *const conf);
    vtss_rc (* del)(struct vtss_state_s *vtss_state,
                    const vtss_hqos_id_t hqos_id);
    vtss_rc (* map)(struct vtss_state_s *vtss_state,
                    const vtss_hqos_id_t hqos_id,
                    const BOOL port_is_nni);
    vtss_rc (* isdx_add)(struct vtss_state_s *vtss_state,
                         const vtss_hqos_id_t hqos_id,
                         const u16 isdx,
                         const BOOL check_only);
    vtss_rc (* isdx_del)(struct vtss_state_s *vtss_state,
                         const vtss_hqos_id_t hqos_id,
                         const u16 isdx);
    vtss_rc (* update)(struct vtss_state_s *vtss_state,
                       const vtss_port_no_t port_no);
    vtss_rc (* min_rate_calc)(struct vtss_state_s *vtss_state,
                              const vtss_hqos_id_t hqos_id,
                              vtss_bitrate_t *const min_rate_calc);
    vtss_rc (* port_min_rate_calc)(struct vtss_state_s *vtss_state,
                                   const vtss_port_no_t port_no,
                                   vtss_bitrate_t *const min_rate_calc);

    /* Configuration/state */
    vtss_hqos_port_conf_t port_conf[VTSS_PORT_ARRAY_SIZE];
    vtss_hqos_port_conf_t port_conf_old;
    vtss_hqos_port_info_t port_info[VTSS_PORT_ARRAY_SIZE];
    vtss_hqos_info_t      info;
    vtss_hqos_isdx_list_t isdx_table;
} vtss_hqos_state_t;

vtss_rc vtss_hqos_inst_create(struct vtss_state_s *vtss_state);
vtss_rc vtss_hqos_restart_sync(struct vtss_state_s *vtss_state);
void vtss_hqos_debug_print(struct vtss_state_s *vtss_state, const vtss_debug_printf_t pr, const vtss_debug_info_t *const info);

#endif /* VTSS_FEATURE_HQOS */

#endif /* _VTSS_HQOS_STATE_H_ */
