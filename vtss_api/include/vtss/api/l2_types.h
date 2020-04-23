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

#ifndef _VTSS_L2_TYPES_H_
#define _VTSS_L2_TYPES_H_

#include <vtss/api/types.h>

/**
 * \file
 * \brief Layer 2 Public API Header for l2
 * \details This header file describes public Layer 2 datatypes
 */

/** \brief Aggregation traffic distribution mode */
typedef struct
{
    BOOL smac_enable;            /**< Source MAC address */
    BOOL dmac_enable;            /**< Destination MAC address */
    BOOL sip_dip_enable;         /**< Source and destination IP address */
    BOOL sport_dport_enable;     /**< Source and destination UDP/TCP port */
} vtss_aggr_mode_t;

/**
 * \brief sFlow sampler type.
 *
 * The API supports sampling ingress and egress separately, as well
 * as simultaneously.
 */
typedef enum {
    VTSS_SFLOW_TYPE_NONE = 0, /**< Sampler is not enabled on the port.                         */
    VTSS_SFLOW_TYPE_RX,       /**< Sampler is enabled for ingress on the port.                 */
    VTSS_SFLOW_TYPE_TX,       /**< Sampler is enabled for egress on the port.                  */
    VTSS_SFLOW_TYPE_ALL       /**< Sampler is enabled for both ingress and egress on the port. */
} vtss_sflow_type_t;

#endif /* #define _VTSS_L2_TYPES_H_ */
