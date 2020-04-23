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
