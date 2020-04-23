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
 * \file vtss_daytona_i2c.h
 * \brief  API
 */

#ifndef _VTSS_DAYTONA_I2C_H_
#define _VTSS_DAYTONA_I2C_H_

#if defined(VTSS_ARCH_DAYTONA)
#if defined(VTSS_FEATURE_I2C)

/**
 * \brief Create instance (set up function pointers).
 *
 * \param inst [IN]      Target instance reference.
 *
 * \return Return code.
 **/
vtss_rc vtss_daytona_inst_i2c_create(vtss_state_t *vtss_state);

/**
 * \brief syn the i2c configurations.
 *
 * \return Return code.
 **/
vtss_rc vtss_daytona_i2c_sync(vtss_state_t *vtss_state, vtss_port_no_t port_no);

#endif /* VTSS_FEATURE_I2C  */
#endif /* VTSS_ARCH_DAYTONA */

#endif /* _VTSS_DAYTONA_I2C_H_ */
