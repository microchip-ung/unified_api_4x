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
 * \file
 * \brief HQoS API
 * \details This header file describes Hierarchical Quality of Service (HQoS) functions
 *
 * HQoS is enabled on a port when the port scheduling mode is set to hierarchical.
 *
 * HQoS parameters are configured using HQoS IDs.
 *
 * Traffic can be mapped to HQoS IDs by using the corresponding modules.
 */

#ifndef _VTSS_HQOS_API_H_
#define _VTSS_HQOS_API_H_

#include <vtss/api/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined(VTSS_FEATURE_HQOS)

/* - Hierarchical Quality of Service ---------------------------------- */

/** \brief HQoS port configuration */
typedef struct {
    vtss_hqos_sch_mode_t sch_mode; /**< Scheduling mode */
} vtss_hqos_port_conf_t;

/**
 * \brief Get HQoS port configuration.
 *
 * \param inst [IN]     Target instance reference.
 * \param port_no [IN]  Port number.
 * \param conf [OUT]    HQoS port configuration structure.
 *
 * \return Return code.
 **/
vtss_rc vtss_hqos_port_conf_get(const vtss_inst_t     inst,
                                const vtss_port_no_t  port_no,
                                vtss_hqos_port_conf_t *const conf);

/**
 * \brief Set HQoS port configuration.
 *
 * \param inst [IN]     Target instance reference.
 * \param port_no [IN]  Port number.
 * \param conf [IN]     HQoS port configuration structure.
 *
 * \return Return code.
 **/
vtss_rc vtss_hqos_port_conf_set(const vtss_inst_t           inst,
                                const vtss_port_no_t        port_no,
                                const vtss_hqos_port_conf_t *const conf);

/** \brief Egress QoS setup per HQoS ID **/
typedef struct {
    vtss_port_no_t port_no;                             /**< Port number */
    vtss_shaper_t  shaper;                              /**< Egress shaper */
    vtss_shaper_t  shaper_queue[VTSS_QUEUE_ARRAY_SIZE]; /**< Egress queue shapers */
    u8             dwrr_cnt;                            /**< Number of queues running Deficit Weighted Round Robin scheduling */
    vtss_pct_t     queue_pct[VTSS_QUEUE_ARRAY_SIZE];    /**< Queue percentages */
    vtss_bitrate_t min_rate;                            /**< Configured minimum bandwidth. Unit: kbps. */
} vtss_hqos_conf_t;

/**
 * \brief Add or set HQoS entry.
 *
 * \param inst [IN]     Target instance reference.
 * \param hqos_id [IN]  HQoS ID.
 * \param conf [IN]     HQoS ID configuration.
 *
 * \return Return code.
 **/
vtss_rc vtss_hqos_add(const vtss_inst_t      inst,
                      const vtss_hqos_id_t   hqos_id,
                      const vtss_hqos_conf_t *const conf);


/**
 * \brief Delete HQoS entry.
 *
 * \param inst [IN]     Target instance reference.
 * \param hqos_id [IN]  HQoS ID.
 *
 * \return Return code.
 **/
vtss_rc vtss_hqos_del(const vtss_inst_t    inst,
                      const vtss_hqos_id_t hqos_id);


/**
 * \brief Get HQoS ID configuration.
 *
 * \param inst [IN]     Target instance reference.
 * \param hqos_id [IN]  HQoS ID.
 * \param conf [OUT]    HQoS ID configuration.
 *
 * \return Return code.
 **/
vtss_rc vtss_hqos_get(const vtss_inst_t    inst,
                      const vtss_hqos_id_t hqos_id,
                      vtss_hqos_conf_t     *const conf);

/**
 * \brief Calculate guaranteed bandwidth for an HQoS ID
 *
 * \param inst [IN]            Target instance reference.
 * \param hqos_id [IN]         HQoS ID.
 * \param min_rate_calc [OUT]  Calculated guaranteed bandwidth.
 *
 * \return Return code.
 **/
vtss_rc vtss_hqos_min_rate_calc(const vtss_inst_t    inst,
                                const vtss_hqos_id_t hqos_id,
                                vtss_bitrate_t       *const min_rate_calc);

/**
 * \brief Calculate non-service guaranteed bandwidth
 *
 * \param inst [IN]            Target instance reference.
 * \param port_no [IN]         Port number.
 * \param min_rate_calc [OUT]  Calculated guaranteed bandwidth.
 *
 * \return Return code.
 **/
vtss_rc vtss_hqos_port_min_rate_calc(const vtss_inst_t    inst,
                                     const vtss_port_no_t port_no,
                                     vtss_bitrate_t       *const min_rate_calc);

#endif /* VTSS_FEATURE_HQOS */

#ifdef __cplusplus
}
#endif
#endif /* _VTSS_HQOS_API_H_ */
