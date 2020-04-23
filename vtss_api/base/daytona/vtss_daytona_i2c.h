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
