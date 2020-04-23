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

#ifndef _VTSS_OAM_STATE_H_
#define _VTSS_OAM_STATE_H_

#if defined(VTSS_FEATURE_OAM)
typedef struct {
    vtss_chip_counter_t rx;
    vtss_chip_counter_t tx;
} vtss_oam_voe_internal_rx_tx_counter_t;

typedef struct {
    struct {
        vtss_chip_counter_t                   rx_valid_count;
        vtss_chip_counter_t                   rx_invalid_count;
        vtss_chip_counter_t                   rx_invalid_seq_no;
    } ccm;
    struct {
        vtss_oam_voe_internal_rx_tx_counter_t lm_count[VTSS_PRIO_ARRAY_SIZE];
    } lm;
    struct {
        vtss_chip_counter_t                   rx_lbr;
        vtss_chip_counter_t                   tx_lbm;
        vtss_chip_counter_t                   rx_lbr_trans_id_err;
    } lb;
    struct {
        vtss_chip_counter_t                   rx_tst;
        vtss_chip_counter_t                   tx_tst;
        vtss_chip_counter_t                   rx_tst_trans_id_err;
    } tst;
    struct {
        vtss_oam_voe_internal_rx_tx_counter_t selected_frames;
        vtss_oam_voe_internal_rx_tx_counter_t nonselected_frames;
    } sel;
} vtss_oam_voe_internal_counters_t;

typedef struct {
    BOOL                             allocated;      /* TRUE => VOE is allocated by API (vtss_oam_voe_alloc()); FALSE => free */
    vtss_oam_voe_internal_counters_t counters;       /* Chip counters for a VOE */
} vtss_oam_voe_internal_t;

typedef struct {
    /* CIL function pointers */
    vtss_rc (* vop_cfg_set)(struct vtss_state_s *vtss_state);

    vtss_rc (* event_poll)(struct vtss_state_s *vtss_state,
                           vtss_oam_event_mask_t *const mask);
    vtss_rc (* voe_event_enable)(struct vtss_state_s *vtss_state,
                                 const vtss_oam_voe_idx_t, const vtss_oam_voe_event_mask_t mask, const BOOL enable);
    vtss_rc (* voe_event_poll)(struct vtss_state_s *vtss_state,
                               const vtss_oam_voe_idx_t, vtss_oam_voe_event_mask_t *const mask);

    vtss_rc (* voe_alloc)(struct vtss_state_s *vtss_state,
                          const vtss_oam_voe_type_t type, const vtss_oam_voe_alloc_cfg_t *data, vtss_oam_voe_idx_t *voe_idx);
    vtss_rc (* voe_free)(struct vtss_state_s *vtss_state,
                         const vtss_oam_voe_idx_t);

    vtss_rc (* voe_cfg_set)(struct vtss_state_s *vtss_state,
                            const vtss_oam_voe_idx_t);

    vtss_rc (* voe_ccm_arm_hitme)(struct vtss_state_s *vtss_state,
                                  const vtss_oam_voe_idx_t, const BOOL enable);
    vtss_rc (* voe_ccm_set_rdi_flag)(struct vtss_state_s *vtss_state,
                                     const vtss_oam_voe_idx_t, const BOOL flag);

    vtss_rc (* ccm_status_get)(struct vtss_state_s *vtss_state,
                               const vtss_oam_voe_idx_t, vtss_oam_ccm_status_t *value);

    vtss_rc (* pdu_seen_status_get)(struct vtss_state_s *vtss_state,
                                    const vtss_oam_voe_idx_t, vtss_oam_pdu_seen_status_t *value);
    vtss_rc (* proc_status_get)(struct vtss_state_s *vtss_state,
                                const vtss_oam_voe_idx_t, vtss_oam_proc_status_t *value);

    vtss_rc (* voe_counter_update)(struct vtss_state_s *vtss_state,
                                   const vtss_oam_voe_idx_t);
    vtss_rc (* voe_counter_get)(struct vtss_state_s *vtss_state,
                                const vtss_oam_voe_idx_t, vtss_oam_voe_counter_t *value);
    vtss_rc (* voe_counter_clear)(struct vtss_state_s *vtss_state,
                                  const vtss_oam_voe_idx_t, const u32 mask);
#if defined(VTSS_ARCH_SERVAL)
    vtss_rc (* voe_counter_update_serval)(struct vtss_state_s *vtss_state,
                                          const vtss_oam_voe_idx_t);
#endif
    
    vtss_rc (* ipt_conf_get)(struct vtss_state_s *vtss_state,
                             u32 isdx, vtss_oam_ipt_conf_t *cfg);
    vtss_rc (* ipt_conf_set)(struct vtss_state_s *vtss_state,
                             u32 isdx, const vtss_oam_ipt_conf_t *const cfg);

    /* Configuration/state */
    vtss_oam_vop_conf_t     vop_conf;
    vtss_oam_voe_conf_t     voe_conf[VTSS_OAM_VOE_CNT];
    vtss_oam_voe_internal_t voe_internal[VTSS_OAM_VOE_CNT];
    vtss_oam_voe_idx_t      voe_poll_idx;
} vtss_oam_state_t;

vtss_rc vtss_oam_inst_create(struct vtss_state_s *vtss_state);
void vtss_oam_debug_print(struct vtss_state_s *vtss_state,
                          const vtss_debug_printf_t pr,
                          const vtss_debug_info_t   *const info);

#endif /* VTSS_FEATURE_OAM */

#endif /* _VTSS_OAM_STATE_H_ */
