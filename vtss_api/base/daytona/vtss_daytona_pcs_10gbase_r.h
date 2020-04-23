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

 $Revisions: $

*/

/**
 * \file vtss_daytona_pcs_10gbase_r.h
 * \brief  API
 */

#ifndef _VTSS_DAYTONA_PCS_10GBASE_R_H_
#define _VTSS_DAYTONA_PCS_10GBASE_R_H_

#if defined(VTSS_ARCH_DAYTONA)

/*
 * \brief Set 10gbase_r Default configuration
 *
 * \param Port_no [IN]  Port number.
 * \return Return code.
 **/
vtss_rc vtss_daytona_pcs_default_conf_set(vtss_state_t *vtss_state, vtss_port_no_t port_no);

/**
 * \brief Create instance (set up function pointers).
 *
 * \return Return code.
 **/
vtss_rc vtss_daytona_inst_pcs_10gbase_r_create(vtss_state_t *vtss_state);

/**
 * \brief Restart_conf_set for PCS_10G_Base_R layer.
 *
 * \param Port_no [IN] Port number.
 * \return Return code.
 **/
vtss_rc vtss_daytona_10gbase_r_restart_conf_set(vtss_state_t *vtss_state, vtss_port_no_t port_no);

/**
 * \brief sync 10gbase_r config.
 *
 * \param vtss_state [IN] Vitesse PHY instance
 * \param Port_no [IN] Port number.
 * \return Return code.
 **/
vtss_rc vtss_daytona_pcs_10gbase_r_sync(vtss_state_t *vtss_state, vtss_port_no_t port_no);

#if defined(VTSS_DAYTONA_OOS_FIX)
vtss_rc vtss_daytona_10gbase_r_reconfigure(vtss_state_t *vtss_state, vtss_port_no_t port_no);
#endif

#endif /* VTSS_ARCH_DAYTONA */
#endif /* _VTSS_DAYTONA_PCS_10GBASE_R_H_ */
