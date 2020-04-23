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

/**
 * \file vtss_rab_api.h
 * \brief  RAB  API
 */

#ifndef _VTSS_RAB_API_H_
#define _VTSS_RAB_API_H_

#include <vtss/api/types.h>

#if defined(VTSS_FEATURE_RAB)
#ifdef __cplusplus
extern "C" {
#endif

/* ================================================================= *
 *  Data structures / types
 * ================================================================= */

/** \brief RAB config data   */
typedef struct vtss_rab_cfg {
	u16  fifo_threshold_high_rx;      /**< High Threshold for generation of PAUSE flow control on MAC2 TX */
	u16  fifo_threshold_low_rx;       /**< Low  Threshold for generation of PAUSE flow control on MAC2 TX */
	u16  fifo_threshold_adapt_rx;     /**< Threshold for asertion of rate adaption request to MAC1 TX */
	u16  fifo_threshold_high_tx;      /**< High Threshold for generation of PAUSE flow control on MAC1 TX */
	u16  fifo_threshold_low_tx;       /**< Low  Threshold for generation of PAUSE flow control on MAC1 TX */
	u16  fifo_threshold_adapt_tx;     /**< Threshold for asertion of rate adaption request to MAC2 TX */
    BOOL cut_thru_rx;                 /**< RAB does not store any frames for processing in cut_thru mode
                                           (i.e) abort marked frames will not be discarded */
    BOOL cut_thru_tx;                 /**< RAB does not store any frames for processing in cut_thru mode
                                           (i.e) abort marked frames will not be discarded */
} vtss_rab_cfg_t;

/** \brief RAB counters */
typedef struct vtss_rab_counters {
	u32 pkt_drop_cnt_rx;    /**< Rx packet drop counter */
	u32 pkt_drop_cnt_tx;    /**< Tx packet drop counter */
} vtss_rab_counters_t;


/** \brief RAB vtss_state structure definitions */
typedef struct vtss_rab_cfg_state {
        vtss_rab_cfg_t    rab_cfg; /**< RAB configuration */
} vtss_rab_state_t;

/* ================================================================= *
 *  Dynamic Config
 * ================================================================= */
/**
 * \brief  Set RAB configuration parameters.
 *
 * \param inst [IN]     Target instance reference.
 * \param port_no [IN]  Port number.
 * \param cfg [IN]      pointer to configuration data.
 *
 * \return Return code.
 **/
vtss_rc vtss_rab_config_set(const vtss_inst_t inst,
                            const vtss_port_no_t port_no,
                            const vtss_rab_cfg_t *const cfg);

/**
 * \brief Get RAB configuration parameters.
 *
 * \param inst [IN]     Target instance reference.
 * \param port_no [IN]  Port number.
 * \param cfg [OUT]     pointer to configuration data.
 *
 * \return Return code.
 **/
vtss_rc vtss_rab_config_get(const vtss_inst_t inst,
                            const vtss_port_no_t port_no,
                            vtss_rab_cfg_t * const cfg);

/* ================================================================= *
 *  State Reporting
 * ================================================================= */


/* ================================================================= *
 *  Performance Primitives
 * ================================================================= */
/**
 * \brief Get actual RAB counters.
 *
 * \param inst [IN]     Target instance reference.
 * \param port_no [IN]  Port number.
 * \param counters [OUT]     pointer to counter structure.
 *
 * \return Return code.
 **/
vtss_rc vtss_rab_counters_get(const vtss_inst_t inst,
                              const vtss_port_no_t port_no,
                              vtss_rab_counters_t *const counters);

#ifdef __cplusplus
}
#endif
#endif /* VTSS_FEATURE_RAB */
#endif /* _VTSS_RAB_API_H_ */
