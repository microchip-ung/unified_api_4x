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
 * \file vtss_sfi4_api.h
 * \brief SFI4 API
 */

#ifndef _VTSS_SFI4_API_H_
#define _VTSS_SFI4_API_H_

#include <vtss/api/types.h>

#if defined(VTSS_FEATURE_SFI4)
#ifdef __cplusplus
extern "C" {
#endif

/* ================================================================= *
 *  Data structures / types
 * ================================================================= */
 
 
/** \brief tfi5 config data   */
typedef struct vtss_sfi4_cfg_t{        
    BOOL enable;    
    BOOL dual_rate;
    BOOL rx_to_tx_loopback;    
    BOOL tx_to_rx_loopback;    
}vtss_sfi4_cfg_t;


/** \brief tfi5 status   */
typedef struct vtss_sfi4_status_t{
    BOOL rx_sync;
    BOOL rx_aligned;
}vtss_sfi4_status_t;

/** \brief upi state - container for vtss_state    */
typedef struct vtss_sfi4_state_t{
    vtss_sfi4_cfg_t      sfi4_cfg;
    vtss_sfi4_status_t   sfi4_status;
}vtss_sfi4_state_t;


/** \brief sfi4 loopback */
typedef enum vtss_sfi4_loopback_t {       
    sfi4_rx_to_tx,
    sfi4_tx_to_rx,
} vtss_sfi4_loopback_t;

/** \brief  upi events */
typedef enum vtss_sfi4_events_t {       
        dummy_sfi4,
} vtss_sfi4_events_t;

/* ================================================================= *
 *  Defects/Events
 * ================================================================= */

/* ================================================================= *
 *  Dynamic Config
 * ================================================================= */

/**
 * \brief   .
 *
 * \param inst [IN]      Target instance reference.
 * \param port_no [IN]   Port number.
 *
 * \return Return code.
 **/
vtss_rc vtss_sfi4_set_config(const vtss_inst_t inst,
                          vtss_port_no_t port_no,
                          const vtss_sfi4_cfg_t *const cfg
                          );


/**
 * \brief   .
 *
 * \param inst [IN]      Target instance reference.
 * \param port_no [IN]   Port number.
 *
 * \return Return code.
 **/
vtss_rc vtss_sfi4_get_config(const vtss_inst_t inst,
                          vtss_port_no_t port_no,
                          vtss_sfi4_cfg_t *const cfg
                          );
             
             
         
                          
/**
 * \brief   .
 *
 * \param inst [IN]      Target instance reference.
 * \param port_no [IN]   Port number.
 *
 * \return Return code.
 **/
vtss_rc vtss_sfi4_set_enable(const vtss_inst_t inst,
                          vtss_port_no_t port_no,
                          BOOL enable
                          );                          


/**
 * \brief   .
 *
 * \param inst [IN]      Target instance reference.
 * \param port_no [IN]   Port number.
 *
 * \return Return code.
 **/
vtss_rc vtss_sfi4_set_loopback(const vtss_inst_t inst,
                          vtss_port_no_t port_no,
                          BOOL enable,
                          vtss_sfi4_loopback_t dir
                          ); 
                          

                          
                                            
/* ================================================================= *
 *  State Reporting
 * ================================================================= */

/**
 * \brief .
 *
 * \param inst [IN]      Target instance reference.
 * \param port_no [IN]   Port number.
 * \param def [OUT]      pointer to defect status structure.
 *
 * \return Return code.
 **/
vtss_rc vtss_sfi4_events_get(vtss_inst_t inst,
                          vtss_port_no_t port_no,
                          vtss_sfi4_events_t *const events);
                          
                          
/**
 * \brief .
 *
 * \param inst [IN]      Target instance reference.
 * \param port_no [IN]   Port number.
 * \param def [OUT]      pointer to defect status structure.
 *
 * \return Return code.
 **/
vtss_rc vtss_sfi4_get_status(vtss_inst_t inst,
                          vtss_port_no_t port_no,
                          vtss_sfi4_status_t *const status);
                          

/* ================================================================= *
 *  Performance Primitives
 * ================================================================= */

#ifdef __cplusplus
}
#endif
#endif /* VTSS_FEATURE_SFI4 */
#endif /* _VTSS_SFI4_API_H_ */
