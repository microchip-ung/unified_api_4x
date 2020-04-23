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
