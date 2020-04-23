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

 $Revisions: $

*/

/**
 * \file vtss_upi_api.h
 * \brief  API
 */

#ifndef _VTSS_DAYTONA_UPI_H_
#define _VTSS_DAYTONA_UPI_H_

#if defined(VTSS_ARCH_DAYTONA)

/*
 * \brief Set Default UPI configuration on a port
 *
 * \param port_no [IN]  Port number
 *
 * \return Return code.
 */
vtss_rc vtss_daytona_upi_default_conf_set(vtss_state_t *vtss_state, vtss_port_no_t port_no);

/*
 * Restart_conf_set for UPI layer.
 *
 * \param port_no [IN] Port number
 * \return Return code.
 */
vtss_rc vtss_daytona_upi_restart_conf_set(vtss_state_t *vtss_state, vtss_port_no_t port_no);

/**
 * \brief Create instance (set up function pointers).
 *
 * \param inst [IN]      Target instance reference.
 * \param port_map [IN]  Port map array.
 *
 * \return Return code.
 **/
vtss_rc vtss_daytona_inst_upi_create(vtss_state_t *vtss_state);

/**
 * \brief sync upi conifguration.
 *
 * \param inst [IN]      Target instance reference.
 * \param port_no [IN]   Port number.
 *
 * \return Return code.
 **/
vtss_rc vtss_daytona_upi_sync(vtss_state_t *vtss_state, vtss_port_no_t port_no);

#if defined(VTSS_DAYTONA_OOS_FIX)
vtss_rc vtss_daytona_upi_reconfigure(vtss_state_t *vtss_state, vtss_port_no_t port_no);
#endif //VTSS_DAYTONA_OOS_FIX

#endif /* VTSS_ARCH_DAYTONA */
#endif /* _VTSS_DAYTONA_UPI_H_ */