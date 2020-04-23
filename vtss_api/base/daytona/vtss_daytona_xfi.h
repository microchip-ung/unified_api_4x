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

 $ $

*/

/**
 * \file vtss_xxx_api.h
 * \brief  API
 */

#ifndef _VTSS_DAYTONA_XFI_H_
#define _VTSS_DAYTONA_XFI_H_

#if defined(VTSS_ARCH_DAYTONA)

/*
 * Set default xfi configuration on a port
 *
 * \param port_no [IN]  Port number
 *
 * \return Return code.
 */
vtss_rc vtss_daytona_xfi_default_conf_set(vtss_state_t *vtss_state, vtss_port_no_t port_no);
/*
 * Restart_conf_set for XFI layer.
 *
 * \param port_no [IN]   Port number
 * \return Return code.
 */
vtss_rc vtss_daytona_xfi_restart_conf_set(vtss_state_t *vtss_state, vtss_port_no_t port_no);

/**
 * \brief Create instance (set up function pointers).
 *
 * \param inst [IN]      Target instance reference.
 * \param port_map [IN]  Port map array.
 *
 * \return Return code.
 **/
vtss_rc vtss_daytona_inst_xfi_create(vtss_state_t *vtss_state);

/**
 * \brief sync xfi conifguration.
 *
 * \param inst [IN]      Target instance reference.
 * \param port_no [IN]   Port number.
 *
 * \return Return code.
 **/
vtss_rc vtss_daytona_xfi_sync(vtss_state_t *vtss_state, vtss_port_no_t port_no);

#if defined(VTSS_DAYTONA_OOS_FIX)
vtss_rc vtss_daytona_xfi_reconfigure(vtss_state_t *vtss_state, vtss_port_no_t port_no);
#endif //VTSS_DAYTONA_OOS_FIX
#endif /* VTSS_ARCH_DAYTONA */
#endif /* _VTSS_DAYTONA_XFI_H_ */
