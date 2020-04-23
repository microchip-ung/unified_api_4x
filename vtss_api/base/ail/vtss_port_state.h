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

#ifndef _VTSS_PORT_STATE_H_
#define _VTSS_PORT_STATE_H_

#if defined(VTSS_FEATURE_PORT_CONTROL)
typedef struct {
    u64 value; /**< Accumulated value (64 bit) */
    u64 prev;  /**< Previous value read (32 or 40 bit) */
} vtss_chip_counter_t;

typedef struct {
    /* Rx counters */
    vtss_chip_counter_t rx_octets;
    vtss_chip_counter_t rx_broadcast;
    vtss_chip_counter_t rx_multicast;
    vtss_chip_counter_t rx_unicast;
    vtss_chip_counter_t rx_shorts;
    vtss_chip_counter_t rx_fragments;
    vtss_chip_counter_t rx_jabbers;
    vtss_chip_counter_t rx_crc_align_errors;
    vtss_chip_counter_t rx_symbol_errors;
    vtss_chip_counter_t rx_64;
    vtss_chip_counter_t rx_65_127;
    vtss_chip_counter_t rx_128_255;
    vtss_chip_counter_t rx_256_511;
    vtss_chip_counter_t rx_512_1023;
    vtss_chip_counter_t rx_1024_1526;
    vtss_chip_counter_t rx_1527_max;
    vtss_chip_counter_t rx_pause;
    vtss_chip_counter_t rx_control;
    vtss_chip_counter_t rx_longs;
#if defined(VTSS_FEATURE_QOS)
    vtss_chip_counter_t rx_classified_drops;
    vtss_chip_counter_t rx_red_class[VTSS_PRIOS];
    vtss_chip_counter_t rx_yellow_class[VTSS_PRIOS];
    vtss_chip_counter_t rx_green_class[VTSS_PRIOS];
    /* Drop counters */
    vtss_chip_counter_t dr_local;
    vtss_chip_counter_t dr_tail;
    vtss_chip_counter_t dr_yellow_class[VTSS_PRIOS];
    vtss_chip_counter_t dr_green_class[VTSS_PRIOS];
#endif
    /* Tx counters */
    vtss_chip_counter_t tx_octets;
    vtss_chip_counter_t tx_unicast;
    vtss_chip_counter_t tx_multicast;
    vtss_chip_counter_t tx_broadcast;
    vtss_chip_counter_t tx_collision;
    vtss_chip_counter_t tx_drops;
    vtss_chip_counter_t tx_pause;
    vtss_chip_counter_t tx_64;
    vtss_chip_counter_t tx_65_127;
    vtss_chip_counter_t tx_128_255;
    vtss_chip_counter_t tx_256_511;
    vtss_chip_counter_t tx_512_1023;
    vtss_chip_counter_t tx_1024_1526;
    vtss_chip_counter_t tx_1527_max;
#if defined(VTSS_FEATURE_QOS)
    vtss_chip_counter_t tx_yellow_class[VTSS_PRIOS];
    vtss_chip_counter_t tx_green_class[VTSS_PRIOS];
#endif
    vtss_chip_counter_t tx_aging;
} vtss_port_luton26_counters_t;

typedef struct {
    /* Rx counters */
    vtss_chip_counter_t rx_in_bytes;
    vtss_chip_counter_t rx_symbol_carrier_err;
    vtss_chip_counter_t rx_pause;
    vtss_chip_counter_t rx_unsup_opcode;
    vtss_chip_counter_t rx_ok_bytes;
    vtss_chip_counter_t rx_bad_bytes;
    vtss_chip_counter_t rx_unicast;
    vtss_chip_counter_t rx_multicast;
    vtss_chip_counter_t rx_broadcast;
    vtss_chip_counter_t rx_crc_err;
    vtss_chip_counter_t rx_undersize;
    vtss_chip_counter_t rx_fragments;
    vtss_chip_counter_t rx_in_range_length_err;
    vtss_chip_counter_t rx_out_of_range_length_err;
    vtss_chip_counter_t rx_oversize;
    vtss_chip_counter_t rx_jabbers;
    vtss_chip_counter_t rx_size64;
    vtss_chip_counter_t rx_size65_127;
    vtss_chip_counter_t rx_size128_255;
    vtss_chip_counter_t rx_size256_511;
    vtss_chip_counter_t rx_size512_1023;
    vtss_chip_counter_t rx_size1024_1518;
    vtss_chip_counter_t rx_size1519_max;
    vtss_chip_counter_t rx_filter_drops;
#if defined(VTSS_FEATURE_QOS)
    vtss_chip_counter_t rx_policer_drops[VTSS_PORT_POLICERS];
    vtss_chip_counter_t rx_class[VTSS_PRIOS];
    vtss_chip_counter_t rx_queue_drops[VTSS_PRIOS];
    vtss_chip_counter_t rx_red_drops[VTSS_PRIOS];
    vtss_chip_counter_t rx_error_drops[VTSS_PRIOS];
#endif /* VTSS_FEATURE_QOS */

    /* Tx counters */
    vtss_chip_counter_t tx_out_bytes;
    vtss_chip_counter_t tx_pause;
    vtss_chip_counter_t tx_ok_bytes;
    vtss_chip_counter_t tx_unicast;
    vtss_chip_counter_t tx_multicast;
    vtss_chip_counter_t tx_broadcast;
    vtss_chip_counter_t tx_size64;
    vtss_chip_counter_t tx_size65_127;
    vtss_chip_counter_t tx_size128_255;
    vtss_chip_counter_t tx_size256_511;
    vtss_chip_counter_t tx_size512_1023;
    vtss_chip_counter_t tx_size1024_1518;
    vtss_chip_counter_t tx_size1519_max;
    vtss_chip_counter_t tx_queue_drops;
    vtss_chip_counter_t tx_red_drops;
    vtss_chip_counter_t tx_error_drops;
    vtss_chip_counter_t tx_queue;
    vtss_chip_counter_t tx_queue_bytes;

    /* These fields are only relevant for D4, for D10 these fields remain 0 */
    vtss_chip_counter_t tx_multi_coll;
    vtss_chip_counter_t tx_late_coll;
    vtss_chip_counter_t tx_xcoll;
    vtss_chip_counter_t tx_defer;
    vtss_chip_counter_t tx_xdefer;
    vtss_chip_counter_t tx_csense;
    vtss_chip_counter_t tx_backoff1;
} vtss_port_b2_counters_t;

typedef struct {
    /* Rx counters */
    vtss_chip_counter_t rx_in_bytes;
    vtss_chip_counter_t rx_symbol_err;
    vtss_chip_counter_t rx_pause;
    vtss_chip_counter_t rx_unsup_opcode;
    vtss_chip_counter_t rx_ok_bytes;
    vtss_chip_counter_t rx_bad_bytes;
    vtss_chip_counter_t rx_unicast;
    vtss_chip_counter_t rx_multicast;
    vtss_chip_counter_t rx_broadcast;
    vtss_chip_counter_t rx_crc_err;
    vtss_chip_counter_t rx_undersize;
    vtss_chip_counter_t rx_fragments;
    vtss_chip_counter_t rx_in_range_len_err;
    vtss_chip_counter_t rx_out_of_range_len_err;
    vtss_chip_counter_t rx_oversize;
    vtss_chip_counter_t rx_jabbers;
    vtss_chip_counter_t rx_size64;
    vtss_chip_counter_t rx_size65_127;
    vtss_chip_counter_t rx_size128_255;
    vtss_chip_counter_t rx_size256_511;
    vtss_chip_counter_t rx_size512_1023;
    vtss_chip_counter_t rx_size1024_1518;
    vtss_chip_counter_t rx_size1519_max;
    vtss_chip_counter_t rx_local_drops;
#if defined(VTSS_FEATURE_QOS)
    vtss_chip_counter_t rx_classified_drops;
    vtss_chip_counter_t rx_red_class[VTSS_PRIOS];
    vtss_chip_counter_t rx_yellow_class[VTSS_PRIOS];
    vtss_chip_counter_t rx_green_class[VTSS_PRIOS];
    vtss_chip_counter_t rx_policer_drops[VTSS_PRIOS];
    vtss_chip_counter_t rx_queue_drops[VTSS_PRIOS];
    vtss_chip_counter_t rx_txqueue_drops[VTSS_PRIOS];
#endif

    /* Tx counters */
    vtss_chip_counter_t tx_out_bytes;
    vtss_chip_counter_t tx_pause;
    vtss_chip_counter_t tx_ok_bytes;
    vtss_chip_counter_t tx_unicast;
    vtss_chip_counter_t tx_multicast;
    vtss_chip_counter_t tx_broadcast;
    vtss_chip_counter_t tx_size64;
    vtss_chip_counter_t tx_size65_127;
    vtss_chip_counter_t tx_size128_255;
    vtss_chip_counter_t tx_size256_511;
    vtss_chip_counter_t tx_size512_1023;
    vtss_chip_counter_t tx_size1024_1518;
    vtss_chip_counter_t tx_size1519_max;
#if defined(VTSS_FEATURE_QOS)
    vtss_chip_counter_t tx_yellow_class[VTSS_PRIOS];
    vtss_chip_counter_t tx_green_class[VTSS_PRIOS];
    vtss_chip_counter_t tx_queue_drops[VTSS_PRIOS];
#endif
    vtss_chip_counter_t tx_multi_coll;
    vtss_chip_counter_t tx_late_coll;
    vtss_chip_counter_t tx_xcoll;
    vtss_chip_counter_t tx_defer;
    vtss_chip_counter_t tx_xdefer;
    vtss_chip_counter_t tx_csense;
    vtss_chip_counter_t tx_backoff1;
} vtss_port_jr1_counters_t;

typedef struct {
    /* Rx counters */
    vtss_chip_counter_t rx_in_bytes;
    vtss_chip_counter_t rx_symbol_err;
    vtss_chip_counter_t rx_pause;
    vtss_chip_counter_t rx_unsup_opcode;
    vtss_chip_counter_t rx_ok_bytes;
    vtss_chip_counter_t rx_bad_bytes;
    vtss_chip_counter_t rx_unicast;
    vtss_chip_counter_t rx_multicast;
    vtss_chip_counter_t rx_broadcast;
    vtss_chip_counter_t rx_crc_err;
    vtss_chip_counter_t rx_undersize;
    vtss_chip_counter_t rx_fragments;
    vtss_chip_counter_t rx_in_range_len_err;
    vtss_chip_counter_t rx_out_of_range_len_err;
    vtss_chip_counter_t rx_oversize;
    vtss_chip_counter_t rx_jabbers;
    vtss_chip_counter_t rx_size64;
    vtss_chip_counter_t rx_size65_127;
    vtss_chip_counter_t rx_size128_255;
    vtss_chip_counter_t rx_size256_511;
    vtss_chip_counter_t rx_size512_1023;
    vtss_chip_counter_t rx_size1024_1518;
    vtss_chip_counter_t rx_size1519_max;
    vtss_chip_counter_t rx_local_drops;
#if defined(VTSS_FEATURE_QOS)
    vtss_chip_counter_t rx_green_drops[VTSS_PRIOS];
    vtss_chip_counter_t rx_yellow_drops[VTSS_PRIOS];
    vtss_chip_counter_t rx_class[VTSS_PRIOS];
#endif
    vtss_chip_counter_t rx_policer_drops;

    /* Tx counters */
    vtss_chip_counter_t tx_out_bytes;
    vtss_chip_counter_t tx_pause;
    vtss_chip_counter_t tx_ok_bytes;
    vtss_chip_counter_t tx_unicast;
    vtss_chip_counter_t tx_multicast;
    vtss_chip_counter_t tx_broadcast;
    vtss_chip_counter_t tx_size64;
    vtss_chip_counter_t tx_size65_127;
    vtss_chip_counter_t tx_size128_255;
    vtss_chip_counter_t tx_size256_511;
    vtss_chip_counter_t tx_size512_1023;
    vtss_chip_counter_t tx_size1024_1518;
    vtss_chip_counter_t tx_size1519_max;
#if defined(VTSS_FEATURE_QOS)
    vtss_chip_counter_t tx_yellow_class[VTSS_PRIOS];
    vtss_chip_counter_t tx_green_class[VTSS_PRIOS];
#endif
    vtss_chip_counter_t tx_queue_drops;
    vtss_chip_counter_t tx_multi_coll;
    vtss_chip_counter_t tx_late_coll;
    vtss_chip_counter_t tx_xcoll;
    vtss_chip_counter_t tx_defer;
    vtss_chip_counter_t tx_xdefer;
    vtss_chip_counter_t tx_csense;
    vtss_chip_counter_t tx_backoff1;
} vtss_port_jr2_counters_t;

#if defined(VTSS_FEATURE_MAC10G)
typedef struct {
    /* Rx counters */
    vtss_chip_counter_t rx_in_bytes;

    vtss_chip_counter_t rx_symbol_err;
    vtss_chip_counter_t rx_pause;
    vtss_chip_counter_t rx_unsup_opcode;
    vtss_chip_counter_t rx_ok_bytes;
    vtss_chip_counter_t rx_bad_bytes;
    vtss_chip_counter_t rx_unicast;
    vtss_chip_counter_t rx_multicast;
    vtss_chip_counter_t rx_broadcast;
    vtss_chip_counter_t rx_crc_err;
    vtss_chip_counter_t rx_undersize;
    vtss_chip_counter_t rx_fragments;
    vtss_chip_counter_t rx_in_range_len_err;
    vtss_chip_counter_t rx_out_of_range_len_err;
    vtss_chip_counter_t rx_oversize;
    vtss_chip_counter_t rx_jabbers;
    vtss_chip_counter_t rx_size64;
    vtss_chip_counter_t rx_size65_127;
    vtss_chip_counter_t rx_size128_255;
    vtss_chip_counter_t rx_size256_511;
    vtss_chip_counter_t rx_size512_1023;
    vtss_chip_counter_t rx_size1024_1518;
    vtss_chip_counter_t rx_size1519_max;

    /* Tx counters */
    vtss_chip_counter_t tx_out_bytes;

    vtss_chip_counter_t tx_pause; /* DBG */
    vtss_chip_counter_t tx_ok_bytes;
    vtss_chip_counter_t tx_unicast;
    vtss_chip_counter_t tx_multicast;
    vtss_chip_counter_t tx_broadcast;
    vtss_chip_counter_t tx_size64;
    vtss_chip_counter_t tx_size65_127;
    vtss_chip_counter_t tx_size128_255;
    vtss_chip_counter_t tx_size256_511;
    vtss_chip_counter_t tx_size512_1023;
    vtss_chip_counter_t tx_size1024_1518;
    vtss_chip_counter_t tx_size1519_max;
} vtss_port_mac10g_counters_t;
#endif /* VTSS_FEATURE_MAC10G */

typedef struct {
    union {
        vtss_port_luton26_counters_t luton26;
        vtss_port_b2_counters_t      b2;
        vtss_port_jr1_counters_t     jr1;
        vtss_port_jr2_counters_t     jr2;
#if defined(VTSS_FEATURE_MAC10G)
        vtss_port_mac10g_counters_t  mac10g;
#endif /* VTSS_FEATURE_MAC10G */
    } counter;
} vtss_port_chip_counters_t;

#if defined(VTSS_FEATURE_PCS_10GBASE_R)
/** \brief pcs counters */
typedef struct vtss_pcs_10gbase_r_cnt_s {
	vtss_chip_counter_t ber_count;                 /**< counts each time BER_BAD_SH state is entered */
    vtss_chip_counter_t	rx_errored_block_count;     /**< counts once for each time RX_E state is entered. */
    vtss_chip_counter_t	tx_errored_block_count;     /**< counts once for each time TX_E state is entered. */
    vtss_chip_counter_t	test_pattern_error_count;   /**< When the receiver is in test-pattern mode, the test_pattern_error_count counts
                                                              errors as described in 802.3: 49.2.12. */
} vtss_pcs_10gbase_r_chip_counters_t;
#endif /*VTSS_FEATURE_PCS_10GBASE_R */

#if defined(VTSS_FEATURE_CLAUSE_37)
typedef struct
{
    BOOL link;        /**< FALSE if link has been down since last status read */
    struct
    {
        BOOL                      complete;               /**< Completion status */
        vtss_port_clause_37_adv_t partner_advertisement;  /**< Clause 37 Advertisement control data */
        vtss_port_sgmii_aneg_t    partner_adv_sgmii;      /**< SGMII Advertisement control data */
    } autoneg;                                            /**< Autoneg status */
} vtss_port_clause_37_status_t;
#endif

typedef struct {
    /* CIL function pointers */
    vtss_rc (* miim_read)(struct vtss_state_s *vtss_state,
                          vtss_miim_controller_t miim_controller,
                          u8 miim_addr,
                          u8 addr,
                          u16 *value,
                          BOOL report_errors);
    vtss_rc (* miim_write)(struct vtss_state_s *vtss_state,
                           vtss_miim_controller_t miim_controller,
                           u8 miim_addr,
                           u8 addr,
                           u16 value,
                           BOOL report_errors);
#if defined(VTSS_FEATURE_10G)
    vtss_rc (* mmd_read)(struct vtss_state_s *vtss_state,
                         vtss_miim_controller_t miim_controller, u8 miim_addr, u8 mmd,
                         u16 addr, u16 *value, BOOL report_errors);
    vtss_rc (* mmd_read_inc)(struct vtss_state_s *vtss_state,
                             vtss_miim_controller_t miim_controller, u8 miim_addr, u8 mmd,
                             u16 addr, u16 *buf, u8 count, BOOL report_errors);
    vtss_rc (* mmd_write)(struct vtss_state_s *vtss_state,
                          vtss_miim_controller_t miim_controller, u8 miim_addr, u8 mmd,
                          u16 addr, u16 data, BOOL report_errors);
#endif /* VTSS_FEATURE_10G */
    vtss_rc (* map_set)(struct vtss_state_s *vtss_state);
    vtss_rc (* conf_get)(struct vtss_state_s *vtss_state,
                         const vtss_port_no_t port_no,
                         vtss_port_conf_t *const conf);
    vtss_rc (* conf_set)(struct vtss_state_s *vtss_state, const vtss_port_no_t port_no);

    vtss_rc (* ifh_set)(struct vtss_state_s *vtss_state, const vtss_port_no_t port_no);
#if defined(VTSS_FEATURE_CLAUSE_37)
    vtss_rc (* clause_37_status_get)(struct vtss_state_s *vtss_state,
                                     const vtss_port_no_t port_no,
                                     vtss_port_clause_37_status_t *const status);
    vtss_rc (* clause_37_control_get)(struct vtss_state_s *vtss_state,
                                      const vtss_port_no_t           port_no,
                                      vtss_port_clause_37_control_t  *const control);
    vtss_rc (* clause_37_control_set)(struct vtss_state_s *vtss_state,
                                      const vtss_port_no_t port_no);
#endif
    vtss_rc (* status_get)(struct vtss_state_s *vtss_state,
                           const vtss_port_no_t  port_no,
                           vtss_port_status_t    *const status);
    vtss_rc (* counters_update)(struct vtss_state_s *vtss_state,
                                const vtss_port_no_t port_no);
    vtss_rc (* counters_clear)(struct vtss_state_s *vtss_state,
                               const vtss_port_no_t port_no);
    vtss_rc (* counters_get)(struct vtss_state_s *vtss_state,
                             const vtss_port_no_t port_no,
                             vtss_port_counters_t *const counters);
    vtss_rc (* basic_counters_get)(struct vtss_state_s *vtss_state,
                                   const vtss_port_no_t port_no,
                                   vtss_basic_counters_t *const counters);
    vtss_rc (* forward_set)(struct vtss_state_s *vtss_state,
                                 const vtss_port_no_t port_no);

    /* Configuration/state */
    vtss_port_map_t               map[VTSS_PORT_ARRAY_SIZE];
    vtss_port_conf_t              conf[VTSS_PORT_ARRAY_SIZE];
    BOOL                          conf_set_called[VTSS_PORT_ARRAY_SIZE];
    vtss_serdes_mode_t            serdes_mode[VTSS_PORT_ARRAY_SIZE];
#if defined(VTSS_FEATURE_CLAUSE_37)
    vtss_port_clause_37_control_t clause_37[VTSS_PORT_ARRAY_SIZE];
#endif
    vtss_port_chip_counters_t     counters[VTSS_PORT_ARRAY_SIZE];
    vtss_port_chip_counters_t     cpu_counters;
#if defined(VTSS_FEATURE_PORT_IFH)
    vtss_port_ifh_t               ifh_conf[VTSS_PORT_ARRAY_SIZE];
#endif /* VTSS_FEATURE_PORT_IFH */
#if defined(VTSS_ARCH_SERVAL) || defined(VTSS_ARCH_JAGUAR_2)
    u32                           buf_prio_shr_e[VTSS_PRIOS]; /* Initial values of buf_prio_shr_e sharing watermarks */
#endif /* defined(VTSS_ARCH_SERVAL) || defined(VTSS_ARCH_JAGUAR_2) */
    vtss_port_forward_t           forward[VTSS_PORT_ARRAY_SIZE];
#if defined(VTSS_ARCH_JAGUAR_1)
    u32                           port_int_0;              /* Internal chip port 0 */
    u32                           port_int_1;              /* Internal chip port 1 */
    u32                           mask_int_ports;          /* Internal chip port mask */
    vtss_port_mux_mode_t          mux_mode[VTSS_CHIP_CNT]; /* Port mux modes */
#endif /* VTSS_ARCH_JAGUAR_1 */

#if defined(VTSS_ARCH_JAGUAR_2)
    vtss_port_mux_mode_t          mux_mode[VTSS_CHIP_CNT]; /* Port mux modes */
#endif /* VTSS_ARCH_JAGUAR_2 */
} vtss_port_state_t;

vtss_rc vtss_port_inst_create(struct vtss_state_s *vtss_state);
vtss_rc vtss_port_restart_sync(struct vtss_state_s *vtss_state);

vtss_port_no_t vtss_cmn_first_port_no_get(struct vtss_state_s *vtss_state,
                                          const BOOL port_list[VTSS_PORT_ARRAY_SIZE],
                                          BOOL port_cpu);
vtss_port_no_t vtss_cmn_port2port_no(struct vtss_state_s *vtss_state,
                                     const vtss_debug_info_t *const info, u32 port);
#if defined(VTSS_FEATURE_CLAUSE_37)
vtss_rc vtss_cmn_port_clause_37_adv_get(u32 value, vtss_port_clause_37_adv_t *adv);
vtss_rc vtss_cmn_port_clause_37_adv_set(u32 *value, vtss_port_clause_37_adv_t *adv, BOOL aneg_enable);
#endif
void vtss_port_debug_print(struct vtss_state_s *vtss_state, 
                           const vtss_debug_printf_t pr,
                           const vtss_debug_info_t   *const info);

#endif /* VTSS_FEATURE_PORT_CONTROL */

#endif /* _VTSS_PORT_STATE_H_ */
