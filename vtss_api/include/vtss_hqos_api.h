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
