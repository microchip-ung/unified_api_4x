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

#ifndef _VTSS_QOS_STATE_H_
#define _VTSS_QOS_STATE_H_

#if defined(VTSS_FEATURE_QOS)
#if defined(VTSS_FEATURE_QCL)
#define VTSS_QCL_LIST_SIZE 24

typedef struct vtss_qcl_entry_t {
    struct vtss_qcl_entry_t *next;  /* Next in list */
    vtss_qce_t               qce;   /* This entry */
} vtss_qcl_entry_t;

typedef struct {
    vtss_qcl_entry_t         *qcl_list_used;               /* Free entries for QCL usage */
    vtss_qcl_entry_t         *qcl_list_free;               /* Entries in QCL List */
    vtss_qcl_entry_t         qcl_list[VTSS_QCL_LIST_SIZE]; /* Actual storage for list members */
} vtss_qcl_t;
#endif /* VTSS_FEATURE_QCL */

#if defined(VTSS_ARCH_SERVAL) || defined(VTSS_ARCH_JAGUAR_2)

#define VTSS_QOS_DWRR_COST_BIT_WIDTH 5

#endif /* VTSS_ARCH_SERVAL || VTSS_ARCH_JAGUAR_2 */

#if defined(VTSS_ARCH_LUTON26)
#define VTSS_L26_POLICER_CNT 256

/** \brief Shared policer users */
typedef enum
{
    VTSS_POLICER_USER_NONE,    /**< Policer is free */
    VTSS_POLICER_USER_DISCARD, /**< Discard Policer */
    VTSS_POLICER_USER_PORT,    /**< Port Policer */
    VTSS_POLICER_USER_QUEUE,   /**< Queue Policer */
    VTSS_POLICER_USER_ACL,     /**< ACL Policer */
    VTSS_POLICER_USER_EVC,     /**< EVC Policer */
    VTSS_POLICER_USER_MEP,     /**< Up-MEP Policer */

    /* Number of users, must be last field */
    VTSS_POLICER_USER_CNT
} vtss_policer_user_t;

/* Policer allocation */
typedef struct {
    u16 count;   /* Reference count */
    u16 policer; /* Policer index */
} vtss_policer_alloc_t;

/* Policer types used for IS2 entries */
#define VTSS_L26_POLICER_NONE 0
#define VTSS_L26_POLICER_ACL  1
#define VTSS_L26_POLICER_EVC  2

#endif /* VTSS_ARCH_LUTON26 */

#if defined(VTSS_ARCH_SERVAL)
typedef struct {
    u32 rate_pwm;       /**< Target relative rate of CIR PWM Serval fix. Unit: kbps. Valid values are 0, 100, 200, and 300. Use 0 to disable the CIR PWM Serval fix */
    u32 rate_pwm_high;  /**< High rate of CIR PWM Serval fix.            Unit: 100 kbps */
    i32 rate_pwm_error; /**< Current rate error of CIR PWM Serval fix. */
    u32 level_pwm;      /**< CBS used by CIR PWM Serval fix.             Unit: 4096 bytes */
    u32 eir_pwm;        /**< Target relative rate of EIR PWM Serval fix. Unit: kbps. Valid values are 0, 100, 200, and 300. Use 0 to disable the EIR PWM Serval fix */
    u32 eir_pwm_high;   /**< High rate of EIR PWM Serval fix.            Unit: 100 kbps */
    i32 eir_pwm_error;  /**< Current rate error of EIR PWM Serval fix. */
    u32 ebs_pwm;        /**< EBS used by EIR PWM Serval fix.             Unit: 4096 bytes */
} vtss_shaper_calibrate_t;
#endif /* defined(VTSS_ARCH_SERVAL) */

#if defined(VTSS_ARCH_JAGUAR_2)
#define VTSS_QUEUE_POL_IDX(port, queue) (4096 + (port * 8) + queue)
#endif /* VTSS_ARCH_JAGUAR_2 */

typedef struct {
    /* CIL function pointers */
    vtss_rc (* conf_set)(struct vtss_state_s *vtss_state, BOOL changed);
    vtss_rc (* port_conf_set)(struct vtss_state_s *vtss_state,
                              const vtss_port_no_t port_no);
    vtss_rc (* port_conf_update)(struct vtss_state_s *vtss_state,
                                 const vtss_port_no_t port_no);
#if defined(VTSS_FEATURE_QCL)
    vtss_rc (* qce_add)(struct vtss_state_s *vtss_state,
                        const vtss_qcl_id_t  qcl_id,
                        const vtss_qce_id_t  qce_id,
                        const vtss_qce_t     *const qce);
    vtss_rc (* qce_del)(struct vtss_state_s *vtss_state,
                        const vtss_qcl_id_t  qcl_id,
                        const vtss_qce_id_t  qce_id);
#endif /* VTSS_FEATURE_QCL */
#if defined(VTSS_FEATURE_QOS_POLICER_DLB)
    vtss_rc (* evc_policer_conf_set)(struct vtss_state_s *vtss_state,
                                     const vtss_evc_policer_id_t policer_id);
#endif /* VTSS_FEATURE_QOS_POLICER_DLB */
#if defined(VTSS_ARCH_SERVAL)
    vtss_rc (* shaper_calibrate)(struct vtss_state_s *vtss_state);
#endif /* defined(VTSS_ARCH_SERVAL) */

#if defined(VTSS_FEATURE_QOS_CPU_PORT_SHAPER)
    vtss_rc (* cpu_port_shaper_set)(struct vtss_state_s *vtss_state, const vtss_bitrate_t rate);
#endif /* defined(VTSS_FEATURE_QOS_CPU_PORT_SHAPER) */

    /* Configuration/state */
    BOOL                    wfq;
    vtss_prio_t             prio_count; /* Maximum number of supported priorities. Must be a power of two. Read-only! */
    vtss_qos_conf_t         conf;
    vtss_qos_port_conf_t    port_conf_old;
    vtss_qos_port_conf_t    port_conf[VTSS_PORT_ARRAY_SIZE];
#if defined(VTSS_FEATURE_QCL)
    vtss_qcl_t              qcl[VTSS_QCL_ARRAY_SIZE];
#endif /* VTSS_FEATURE_QCL */
#if defined(VTSS_ARCH_LUTON26)
    vtss_policer_user_t     policer_user[VTSS_L26_POLICER_CNT];
#endif /* VTSS_ARCH_LUTON26 */

#if defined(VTSS_EVC_POLICERS)
    u32                     evc_policer_max;
    vtss_evc_policer_conf_t evc_policer_conf[VTSS_EVC_POLICERS];
#if defined(VTSS_ARCH_LUTON26)
    vtss_policer_alloc_t    evc_policer_alloc[VTSS_EVC_POLICERS];
#endif /* VTSS_ARCH_LUTON26 */
#endif /* VTSS_EVC_POLICERS */

#if defined(VTSS_ARCH_SERVAL)
    vtss_shaper_calibrate_t port_shaper[VTSS_PORT_ARRAY_SIZE];
    vtss_shaper_calibrate_t queue_shaper[VTSS_PORT_ARRAY_SIZE][VTSS_QUEUE_ARRAY_SIZE];
#endif /* defined(VTSS_ARCH_SERVAL) */
} vtss_qos_state_t;

vtss_rc vtss_qos_inst_create(struct vtss_state_s *vtss_state);
vtss_rc vtss_qos_restart_sync(struct vtss_state_s *vtss_state);

u32 vtss_cmn_qos_chip_prio(struct vtss_state_s *vtss_state, const vtss_prio_t prio);
vtss_rc vtss_cmn_qos_port_conf_set(struct vtss_state_s *vtss_state, const vtss_port_no_t port_no);
vtss_rc vtss_cmn_qos_weight2cost(const vtss_pct_t * weight, u8 * cost, size_t num, u8 bit_width);
#if defined(VTSS_FEATURE_QCL_V2)
vtss_rc vtss_cmn_qce_add(struct vtss_state_s *vtss_state,
                         const vtss_qcl_id_t qcl_id, const vtss_qce_id_t qce_id, const vtss_qce_t *const qce);
vtss_rc vtss_cmn_qce_del(struct vtss_state_s *vtss_state,
                         const vtss_qcl_id_t qcl_id, const vtss_qce_id_t qce_id);
#endif /* VTSS_FEATURE_QCL_V2 */

#if defined(VTSS_FEATURE_QOS_POLICER_DLB)
vtss_rc vtss_evc_policer_id_check(struct vtss_state_s *vtss_state,
                                  const vtss_evc_policer_id_t policer_id, BOOL resv);
#endif /* VTSS_FEATURE_QOS_POLICER_DLB */
void vtss_qos_debug_print_dlb(struct vtss_state_s *vtss_state,
                              const vtss_debug_printf_t pr,
                              const vtss_debug_info_t   *const info);
void vtss_qos_debug_print(struct vtss_state_s *vtss_state,
                          const vtss_debug_printf_t pr,
                          const vtss_debug_info_t   *const info);

#endif /* VTSS_FEATURE_QOS */

#endif /* _VTSS_QOS_STATE_H_ */
