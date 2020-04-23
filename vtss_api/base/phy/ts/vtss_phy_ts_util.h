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

#ifndef _VTSS_PHY_TS_UTIL_H_
#define _VTSS_PHY_TS_UTIL_H_

#include <vtss/api/types.h>

#if defined(VTSS_FEATURE_PHY_TIMESTAMP)
#ifdef VTSS_CHIP_CU_PHY
extern vtss_rc vtss_phy_loopback_set_private(vtss_state_t *vtss_state, const vtss_port_no_t port_no);
extern vtss_rc vtss_phy_conf_set_private(vtss_state_t *vtss_state, const vtss_port_no_t port_no);
extern vtss_rc vtss_phy_conf_1g_set_private(vtss_state_t *vtss_state, const vtss_port_no_t port_no);

extern vtss_rc vtss_phy_ts_base_port_get_priv(vtss_state_t *vtss_state,
                                              const vtss_port_no_t port_no,
                                              vtss_port_no_t    *const base_port_no);
extern vtss_rc vtss_phy_id_get_priv(vtss_state_t *vtss_state,
                                    const vtss_port_no_t   port_no,
                                    vtss_phy_type_t *phy_id);
extern vtss_rc vtss_phy_ts_read_csr(vtss_state_t *vtss_state,
                                    const vtss_port_no_t port_no,
                                    const vtss_phy_ts_blk_id_t blk_id,
                                    const u16 csr_address,
                                    u32 *const value);
extern vtss_rc vtss_phy_ts_write_csr(vtss_state_t *vtss_state,
                                     const vtss_port_no_t port_no,
                                     const vtss_phy_ts_blk_id_t blk_id,
                                     const u16 csr_address,
                                     const u32 *const value);
extern vtss_rc vtss_phy_ts_csr_event_poll_priv(vtss_state_t *vtss_state,
                                               const vtss_port_no_t  port_no,
                                               vtss_phy_ts_event_t   *const status);
extern vtss_rc vtss_phy_ts_new_spi_mode_set_priv(vtss_state_t *vtss_state,
                                                 const vtss_port_no_t    port_no,
                                                 const BOOL              enable);
extern vtss_rc vtss_phy_ts_csr_set_priv(vtss_state_t *vtss_state,
                                        const vtss_port_no_t port_no,
                                        const vtss_phy_ts_proc_conf_t conf);

#if defined(VTSS_CHIP_CU_PHY) && defined(VTSS_PHY_TS_SPI_CLK_THRU_PPS0)
extern vtss_rc vtss_phy_ts_channel_id_get_priv(vtss_state_t *vtss_state,
                                               const vtss_port_no_t port_no,
                                               u16     *const channel_id);
extern vtss_rc vtss_phy_ts_spi_pause_priv(vtss_state_t *vtss_state,
                                          const vtss_port_no_t port_no);
extern vtss_rc vtss_phy_ts_spi_unpause_priv(vtss_state_t *vtss_state,
                                            const vtss_port_no_t port_no);

#endif /* VTSS_CHIP_CU_PHY && VTSS_PHY_TS_SPI_CLK_THRU_PPS0 */

#if defined(TESLA_ING_TS_ERRFIX) || defined(VIPER_B_FIFO_RESET)

extern BOOL  vtss_phy_ts_algo_execute_check(vtss_state_t *vtss_state,
                                            const vtss_port_no_t port_no);
#endif

#ifdef TESLA_ING_TS_ERRFIX
typedef enum {
    NO_MATCH = 0,
    NO_MATCH_INGRESS,
    NO_MATCH_EGRESS,
    MATCH_INGRESS = 0x10,
    MATCH_EGRESS,
    MATCH_INGRESS_EGRESS,
    MATCH_ENGINE_2_PLAN_E = 0x20
} vtss_match_type_t;

/* Note: It is not necessary to have a SAV flag for Boolean Values */
typedef struct vtss_phy_ts_fifo_err_recovery {
    vtss_port_no_t                    port_no;
    BOOL                              recovery_required;
    vtss_phy_type_t                   phy_id;
    BOOL                              phy_id_sav;
    vtss_phy_conf_t                   port_conf;
    BOOL                              port_conf_sav;
    vtss_phy_reset_conf_t             port_reset_conf;
    BOOL                              port_reset_conf_sav;
    u32                               ana_mode;
    BOOL                              ana_mode_sav;
    u32                               si_cfg;
    BOOL                              si_cfg_sav;
    vtss_phy_ts_engine_channel_map_t  chmask;
    BOOL                              chmask_sav;
    u16                               eng_id;
    BOOL                              eng_id_sav;
    u16                               phy_control_reg;
    BOOL                              phy_control_reg_sav;
    BOOL                              forced_1000BaseT;
    vtss_timeinterval_t               ingress_latency;
    BOOL                              ingress_latency_sav;
    vtss_timeinterval_t               egress_latency;
    BOOL                              egress_latency_sav;
} vtss_phy_ts_fifo_err_recovery_t;



vtss_rc vtss_phy_ts_tesla_tsp_fifo_sync_private(vtss_state_t *vtss_state,
                                                const vtss_port_no_t            port_no,
                                                const vtss_debug_printf_t       pr,
                                                const vtss_phy_ts_fifo_conf_t   *fifo_conf,
                                                BOOL                            *OOS);


BOOL vtss_phy_ts_is_oos_recovery_enabled_private (vtss_state_t *vtss_state, vtss_port_no_t  port_no);

vtss_rc vtss_phy_ts_tesla_oos_recovery_disable_priv(vtss_state_t *vtss_state, vtss_port_no_t  port_no, const vtss_debug_printf_t  pr);

BOOL vtss_phy_ts_is_oos_recovery_enabled_private (vtss_state_t *vtss_state, vtss_port_no_t  port_no);

vtss_rc vtss_phy_default_fifo_conf_tesla_oos_get(vtss_state_t *vtss_state, vtss_port_no_t port_no, vtss_phy_ts_fifo_conf_t *fifo_conf_tesla);


#endif  /* (TESLA_ING_TS_ERRFIX) */

BOOL  vtss_phy_ts_algo_execute_check(vtss_state_t *vtss_state,
                                     const vtss_port_no_t port_no);


#if defined(VIPER_B_FIFO_RESET)
vtss_rc vtss_phy_1588_oos_mitigation_steps_private(vtss_state_t *vtss_state,
                                                   const vtss_port_no_t port_no,
                                                   const vtss_phy_ts_fifo_conf_t   *fifo_conf);


vtss_rc vtss_phy_ts_isolate_phy(vtss_state_t *vtss_state,
                                const vtss_port_no_t port_no);

#endif  /* (VIPER_B_FIFO_RESET) */


#endif /* VTSS_CHIP_CU_PHY */
#endif /* VTSS_FEATURE_PHY_TIMESTAMP */

#endif /* _VTSS_PHY_TS_UTIL_H_ */

