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

#ifndef _VTSS_PHY_TS_H_
#define _VTSS_PHY_TS_H_

#include <vtss/api/types.h>

#if defined(VTSS_FEATURE_PHY_TIMESTAMP)

typedef enum {
    VTSS_PHY_TS_ANA_BLK_ID_ING_0, /* Order taken from 1G PHY */
    VTSS_PHY_TS_ANA_BLK_ID_EGR_0,
    VTSS_PHY_TS_ANA_BLK_ID_ING_1,
    VTSS_PHY_TS_ANA_BLK_ID_EGR_1,
    VTSS_PHY_TS_ANA_BLK_ID_ING_2,
    VTSS_PHY_TS_ANA_BLK_ID_EGR_2,
    VTSS_PHY_TS_PROC_BLK_ID_0,
    VTSS_PHY_TS_PROC_BLK_ID_1,
    VTSS_PHY_TS_MAX_BLK_ID
} vtss_phy_ts_blk_id_t;

typedef enum {
    PTP_ACTION_CMD_NOP = 0,
    PTP_ACTION_CMD_SUB = 1,
    PTP_ACTION_CMD_ADD = 3,
    PTP_ACTION_CMD_SUB_ADD = 4,
    PTP_ACTION_CMD_WRITE_1588 = 5,
    PTP_ACTION_CMD_WRITE_NS = 7,
    PTP_ACTION_CMD_WRITE_NS_P2P = 8,
    PTP_ACTION_CMD_ADD_2 = 9,
    PTP_ACTION_CMD_SUB_2 = 10,
    PTP_ACTION_CMD_SAVE_IN_TS_FIFO, /* not the 1588 command */
#ifdef VTSS_FEATURE_PTP_DELAY_COMP_ENGINE
    PTP_ACTION_CMD_DCE, /* not the 1588 command */
#endif /* VTSS_FEATURE_PTP_DELAY_COMP_ENGINE */
} vtss_phy_ts_ptp_action_cmd_t;

typedef enum {
    VTSS_PHY_TS_ING_LATENCY_SET,
    VTSS_PHY_TS_EGR_LATENCY_SET,
    VTSS_PHY_TS_PATH_DELAY_SET,
    VTSS_PHY_TS_DELAY_ASYM_SET,
    VTSS_PHY_TS_RATE_ADJ_SET,
    VTSS_PHY_TS_PORT_ENA_SET,
    VTSS_PHY_TS_PORT_EVT_MASK_SET,
    VTSS_PHY_TS_PORT_OPER_MODE_CHANGE_SET,
    VTSS_PHY_TS_PPS_CONF_SET,
    VTSS_PHY_TS_ALT_CLK_SET,
    VTSS_PHY_TS_SERTOD_SET,
    VTSS_PHY_TS_LOAD_PULSE_DLY_SET,
    VTSS_PHY_TS_LTC_FREQ_SYNTH_SET,
#ifdef VTSS_FEATURE_PTP_DELAY_COMP_ENGINE
    VTSS_PHY_TS_ING_DELAY_COMP_SET,
    VTSS_PHY_TS_EGR_DELAY_COMP_SET,
#endif
} vtss_phy_ts_proc_conf_t;

#define VTSS_PHY_TS_DISP_CSR(c,b,a,v) \
                 if ( vtss_phy_ts_read_csr(vtss_state, c, b, a, v) == VTSS_RC_OK ) { printf("0x%08lx ",(long unsigned int)*v); }

#define VTSS_PHY_TS_READ_CSR(p, b, a, v) vtss_phy_ts_read_csr(vtss_state, p, b, a, v)

#define VTSS_PHY_TS_WRITE_CSR(p, b, a, v) vtss_phy_ts_write_csr(vtss_state, p , b, a, v)
#if !defined(VTSS_PHY_TS_CLR_BITS)
#define  VTSS_PHY_TS_CLR_BITS(value, mask)   ((value) & ~(mask))
#endif


#if defined(VTSS_CHIP_CU_PHY) && defined(VTSS_PHY_TS_SPI_CLK_THRU_PPS0)
/* pause_priv is used for private function where no need of vtss_exit */

#if defined (TESLA_ING_TS_ERRFIX) /* Tesla OOS code uses Middle-Man Approach to access CSR to prevent Micro-Hangs */

/* SPI PAUSE not necessary when using Middle-Man Approach and NOT reading TS FIFO */
#define VTSS_PHY_TS_SPI_PAUSE_PRIV(p) \
{ \
    vtss_rc err; \
    if (vtss_state->phy_ts_port_conf[(p)].new_spi_conf.phy_type == VTSS_PHY_TYPE_8574 || vtss_state->phy_ts_port_conf[(p)].new_spi_conf.phy_type == VTSS_PHY_TYPE_8572) { \
        if ((vtss_state->ts_csr_thr_mm) && (!vtss_state->rd_ts_fifo)) { \
        } else { \
            if ((err = vtss_phy_ts_spi_pause_priv(vtss_state, (p))) != VTSS_RC_OK) { \
                VTSS_E("SPI pause fail!, port %u", (p)); \
                return err; \
            } \
        } \
    } \
}

/* SPI PAUSE not necessary when using Middle-Man Approach and NOT reading TS FIFO */
#define VTSS_PHY_TS_SPI_PAUSE(p) \
{ \
    vtss_rc err; \
    if (vtss_state->phy_ts_port_conf[(p)].new_spi_conf.phy_type == VTSS_PHY_TYPE_8574 || vtss_state->phy_ts_port_conf[(p)].new_spi_conf.phy_type == VTSS_PHY_TYPE_8572) { \
        if ((vtss_state->ts_csr_thr_mm) && (!vtss_state->rd_ts_fifo)) { \
        } else { \
            if ((err = vtss_phy_ts_spi_pause_priv(vtss_state, (p))) != VTSS_RC_OK) { \
                VTSS_E("SPI pause fail!, port %u", (p)); \
                VTSS_EXIT(); \
                return err; \
            } \
        } \
    } \
}

#else /* NOT Defined TESLA_ING_TS_ERRFIX */

#define VTSS_PHY_TS_SPI_PAUSE_PRIV(p) \
{ \
    vtss_rc err; \
    if (vtss_state->phy_ts_port_conf[(p)].new_spi_conf.phy_type == VTSS_PHY_TYPE_8574 || vtss_state->phy_ts_port_conf[(p)].new_spi_conf.phy_type == VTSS_PHY_TYPE_8572) { \
        if ((err = vtss_phy_ts_spi_pause_priv(vtss_state, (p))) != VTSS_RC_OK) { \
            VTSS_E("SPI pause fail!, port %u", (p)); \
            return err; \
        } \
    } \
}

#define VTSS_PHY_TS_SPI_PAUSE(p) \
{ \
    vtss_rc err; \
    if (vtss_state->phy_ts_port_conf[(p)].new_spi_conf.phy_type == VTSS_PHY_TYPE_8574 || vtss_state->phy_ts_port_conf[(p)].new_spi_conf.phy_type == VTSS_PHY_TYPE_8572) { \
        if ((err = vtss_phy_ts_spi_pause_priv(vtss_state, (p))) != VTSS_RC_OK) { \
            VTSS_E("SPI pause fail!, port %u", (p)); \
            VTSS_EXIT(); \
            return err; \
        } \
    } \
}

#endif /* TESLA_ING_TS_ERRFIX */

#else /* Not defined(VTSS_CHIP_CU_PHY) && defined(VTSS_PHY_TS_SPI_CLK_THRU_PPS0) */

#define VTSS_PHY_TS_SPI_PAUSE_PRIV(p) {}
#define VTSS_PHY_TS_SPI_PAUSE(p) {}
#endif /* VTSS_CHIP_CU_PHY && VTSS_PHY_TS_SPI_CLK_THRU_PPS0 */

#if defined(VTSS_CHIP_CU_PHY) && defined(VTSS_PHY_TS_SPI_CLK_THRU_PPS0)
/* unpause_priv is used for private function where no need of vtss_exit */
#define VTSS_PHY_TS_SPI_UNPAUSE_PRIV(p) \
{ \
    vtss_rc err; \
    if (vtss_state->phy_ts_port_conf[(p)].new_spi_conf.phy_type == VTSS_PHY_TYPE_8574 || vtss_state->phy_ts_port_conf[(p)].new_spi_conf.phy_type == VTSS_PHY_TYPE_8572) { \
        if ((err = vtss_phy_ts_spi_unpause_priv(vtss_state, (p))) != VTSS_RC_OK) { \
            VTSS_E("SPI Un-pause fail!, port %u", (p)); \
            return err; \
        } \
    } \
}

#define VTSS_PHY_TS_SPI_UNPAUSE(p) \
{ \
    vtss_rc err; \
    if (vtss_state->phy_ts_port_conf[(p)].new_spi_conf.phy_type == VTSS_PHY_TYPE_8574 || vtss_state->phy_ts_port_conf[(p)].new_spi_conf.phy_type == VTSS_PHY_TYPE_8572) { \
        if ((err = vtss_phy_ts_spi_unpause_priv(vtss_state, (p))) != VTSS_RC_OK) { \
            VTSS_E("SPI Un-pause fail!, port %u", (p)); \
            VTSS_EXIT(); \
            return err; \
        } \
    } \
}
#else
#define VTSS_PHY_TS_SPI_UNPAUSE_PRIV(p) {}
#define VTSS_PHY_TS_SPI_UNPAUSE(p) {}
#endif /* VTSS_CHIP_CU_PHY && VTSS_PHY_TS_SPI_CLK_THRU_PPS0 */

#if defined(VTSS_CHIP_CU_PHY) && defined(VTSS_PHY_TS_SPI_CLK_THRU_PPS0)
#define VTSS_PHY_TS_SPI_PAUSE_COLD(p) \
{ \
            if (!vtss_state->warm_start_cur)  VTSS_PHY_TS_SPI_PAUSE(p) \
}
#define VTSS_PHY_TS_SPI_PAUSE_PRIV_COLD(p) \
{ \
    if (!vtss_state->warm_start_cur)  VTSS_PHY_TS_SPI_PAUSE_PRIV(p) \
}
#else
#define VTSS_PHY_TS_SPI_PAUSE_COLD(p) {}
#define VTSS_PHY_TS_SPI_PAUSE_PRIV_COLD(p) {}
#endif /* VTSS_CHIP_CU_PHY && VTSS_PHY_TS_SPI_CLK_THRU_PPS0 */

#if defined(VTSS_CHIP_CU_PHY) && defined(VTSS_PHY_TS_SPI_CLK_THRU_PPS0)
#define VTSS_PHY_TS_SPI_UNPAUSE_COLD(p) \
{ \
            if (!vtss_state->warm_start_cur)  VTSS_PHY_TS_SPI_UNPAUSE(p) \
}
#define VTSS_PHY_TS_SPI_UNPAUSE_PRIV_COLD(p) \
{ \
    if (!vtss_state->warm_start_cur)  VTSS_PHY_TS_SPI_UNPAUSE_PRIV(p) \
}
#else
#define VTSS_PHY_TS_SPI_UNPAUSE_COLD(p) {}
#define VTSS_PHY_TS_SPI_UNPAUSE_PRIV_COLD(p) {}
#endif /* VTSS_CHIP_CU_PHY && VTSS_PHY_TS_SPI_CLK_THRU_PPS0 */

#define VTSS_PHY_TS_ANA_BLK_ID_ING(x) VTSS_PHY_TS_ANA_BLK_ID_ING_##x
#define VTSS_PHY_TS_ANA_BLK_ID_EGR(x) VTSS_PHY_TS_ANA_BLK_ID_EGR_##x
#define VTSS_PHY_TS_PROC_BLK_ID(x)    VTSS_PHY_TS_PROC_BLK_ID_##x


#ifdef VTSS_FEATURE_WARM_START
/**
 * \brief This is used to sync the vtss_state to PHY.
 *
 * \param port_no      [IN]      port number
 *
 * \return Return code.
 **/
extern vtss_rc vtss_phy_ts_sync(vtss_state_t *vtss_state, const vtss_port_no_t port_no);

#endif /* VTSS_FEATURE_WARM_START */

extern void vtss_phy_ts_api_ail_debug_print(vtss_state_t *vtss_state,
                                            const vtss_debug_printf_t pr,
                                            const vtss_debug_info_t   *const info);

extern void vtss_phy_ts_api_cil_debug_print(vtss_state_t *vtss_state,
                                            const vtss_debug_printf_t pr,
                                            const vtss_debug_info_t   *const info);

vtss_rc vtss_phy_ts_macsec_mode_change (vtss_state_t *vtss_state,
                                        const vtss_port_no_t port_no,
                                        const BOOL enable);

vtss_rc vtss_phy_ts_phy_status_change_priv(vtss_state_t *vtss_state,
                                           const vtss_port_no_t       port_no);
vtss_rc vtss_phy_ts_mode_bypass_en_priv(vtss_state_t         *vtss_state,
                                        const vtss_port_no_t port_no,
                                        BOOL                 enable);
vtss_rc vtss_phy_ts_oos_prevention_set(vtss_state_t    *vtss_state,
                                       const vtss_port_no_t   port_no);

vtss_rc vtss_phy_ts_bypass_set(vtss_state_t    *vtss_state,
                               const vtss_port_no_t   port_no, BOOL enable,
                               BOOL force_bypass);
vtss_rc vtss_phy_ts_version_check(vtss_state_t    *vtss_state,
                                  const vtss_port_no_t port_no,
                                  BOOL *prevent);

vtss_rc vtss_phy_ts_one_shot_delays_load(vtss_state_t *vtss_state,
                                         const vtss_port_no_t port_no);

#if defined(VIPER_B_FIFO_RESET)
typedef struct vtss_phy_ts_overflow_info_s {
    u32 egr_intr_status;
    u32 ingr_intr_status;
} vtss_phy_ts_overflow_info_t;
typedef struct vtss_phy_ts_pop_fifo_s {
    u32 egr_pop_fifo;
    u32 ingr_pop_fifo;
} vtss_phy_ts_pop_fifo_t;
#endif

#endif /* VTSS_FEATURE_PHY_TIMESTAMP */

#endif /* _VTSS_PHY_TS_H_ */

