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
 * \file vtss_tfi5_api.h
 * \brief  TFI5 API
 */

#ifndef _VTSS_TFI5_API_H_
#define _VTSS_TFI5_API_H_

#include <vtss/api/types.h>

#if defined(VTSS_FEATURE_TFI5)
#ifdef __cplusplus
extern "C" {
#endif

/* ================================================================= *
 *  Data structures / types
 * ================================================================= */
#define TFI5_NUM_LANES 4 

 
/** \brief tfi5 config data   */
typedef struct vtss_tfi5_cfg_t{
    BOOL enable;    
    BOOL dual_rate;
    BOOL rx_to_tx_loopback;

    struct {
        BOOL b1_passthru;
        BOOL fixed_j0z0;        
    }tx_config;     

}vtss_tfi5_cfg_t;


/** \brief tfi5 status   */
typedef struct vtss_tfi5_status_t{    
    BOOL OOA[TFI5_NUM_LANES];
    BOOL OOF[TFI5_NUM_LANES];
    BOOL LOS[TFI5_NUM_LANES];
    BOOL LOF;    
}vtss_tfi5_status_t;

/** \brief tfi5 state - container for vtss_state    */
typedef struct vtss_tfi5_state_t{
    vtss_tfi5_cfg_t     tfi5_cfg;
    vtss_tfi5_status_t   tfi5_status;    
}vtss_tfi5_state_t;


/** \brief  tfi5 events */
typedef enum vtss_tfi5_events_t {       
       dummy_tfi5,
} vtss_tfi5_events_t;

/** \brief  tfi5 loopback */
typedef enum vtss_tfi5_loopback_t {       
    tfi5_rx_to_tx,
} vtss_tfi5_loopback_t;

/** \brief  tfi5 error insertion */
typedef enum vtss_tfi5_err_ins_t{
    tfi5_ais,
    tfi5_b1,
    tfi5_a1a2
}vtss_tfi5_err_ins_t;


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
vtss_rc vtss_tfi5_set_config(const vtss_inst_t inst,
                          vtss_port_no_t port_no,
                          const vtss_tfi5_cfg_t *const cfg
                          );


/**
 * \brief   .
 *
 * \param inst [IN]      Target instance reference.
 * \param port_no [IN]   Port number.
 *
 * \return Return code.
 **/
vtss_rc vtss_tfi5_get_config(const vtss_inst_t inst,
                          vtss_port_no_t port_no,
                          vtss_tfi5_cfg_t *const cfg
                          );
             
             
         
                          
/**
 * \brief   .
 *
 * \param inst [IN]      Target instance reference.
 * \param port_no [IN]   Port number.
 *
 * \return Return code.
 **/
vtss_rc vtss_tfi5_set_enable(const vtss_inst_t inst,
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
vtss_rc vtss_tfi5_set_reset(const vtss_inst_t inst,
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
vtss_rc vtss_tfi5_set_loopback(const vtss_inst_t inst,
                          vtss_port_no_t port_no,
                          BOOL enable,
                          vtss_tfi5_loopback_t dir
                          );                          


/**
 * \brief   .
 *
 * \param inst [IN]      Target instance reference.
 * \param port_no [IN]   Port number.
 *
 * \return Return code.
 **/
vtss_rc vtss_tfi5_error_insert(const vtss_inst_t inst,
                          vtss_port_no_t port_no,
                          BOOL enable,
                          vtss_tfi5_err_ins_t err
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
vtss_rc vtss_tfi5_events_get(vtss_inst_t inst,
                          vtss_port_no_t port_no,
                          vtss_tfi5_events_t *const events);
                          
                          
/**
 * \brief .
 *
 * \param inst [IN]      Target instance reference.
 * \param port_no [IN]   Port number.
 * \param def [OUT]      pointer to defect status structure.
 *
 * \return Return code.
 **/
vtss_rc vtss_tfi5_get_status(vtss_inst_t inst,
                          vtss_port_no_t port_no,
                          vtss_tfi5_status_t *const status);
                          

/* ================================================================= *
 *  Performance Primitives
 * ================================================================= */

#ifdef __cplusplus
}
#endif
#endif /* VTSS_FEATURE_TFI5 */
#endif /* _VTSS_TFI5_API_H_ */
