#ifndef _VTSS_VENICE_C__API_SD10G65_H_UTE
#define _VTSS_VENICE_C__API_SD10G65_H_UTE

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

/* Generation Tag is  UTE_release_vts_ute_tcllib_20180312_trunk_bjo */

/* ================================================================= *
 *               Note: This code is autogenerated from the
 *                     Universal Tcl Environment (UTE) which provides
 *                     consistent setup functions for
 *       ^             - Verification
 *      / \            - Validation
 *     /   \           - Test Pattern Generation and
 *    /  |  \          - Software
 *   /   !   \         It should not be modified manually.
 *  /_________\        In case there is a need for modifications,
 *                     please contact
 *                       Patrick Urban <patrick.urban@microsemi.com> or
 *                       Alexander Koch <alexander.koch@microsemi.com> or
 *                       Mark Venneboerger <mark.venneboerger@microsemi.com>
 *                     Please use Bugzilla for reporting issues or requesting enhancements:
 *                     Bugzilla: Tools->Victoria
 *                     http://projissuetracker/bugzilla/enter_bug.cgi?product=Victoria
 * ================================================================= */

#include <vtss/api/options.h>  // To get the ARCH define
#if defined(VTSS_ARCH_VENICE_C)

vtss_rc vtss_venice_c_sd10g_setup_tx(vtss_state_t *vtss_state, const vtss_sd10g65_setup_tx_args_t config, vtss_port_no_t port_no);
vtss_rc vtss_venice_c_sd10g_setup_rx(vtss_state_t *vtss_state, const vtss_sd10g65_setup_rx_args_t config, vtss_port_no_t port_no);
vtss_rc vtss_venice_c_sd10g_power_down(vtss_state_t *vtss_state, vtss_port_no_t port_no);
vtss_rc vtss_venice_c_apc10g_setup(vtss_state_t *vtss_state, const vtss_sd10g65_setup_apc_args_t config, vtss_port_no_t port_no);
#endif
#endif
