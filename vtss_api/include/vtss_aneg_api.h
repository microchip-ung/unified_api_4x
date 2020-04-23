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
 * \file vtss_aneg_api.h
 * \brief  ANEG  API
 */

#ifndef _VTSS_ANEG_API_H_
#define _VTSS_ANEG_API_H_

#include <vtss/api/types.h>

#if defined(VTSS_FEATURE_ANEG)
#ifdef __cplusplus
extern "C" {
#endif

/* ================================================================= *
 *  Data structures / types
 * ================================================================= */
/** \brief aneg config data   */
typedef struct vtss_aneg_cfg_t{
   BOOL  aneg_enable;
   u16   parallel_det_wait_time;
   BOOL  ctrl_ob;
  
}vtss_aneg_cfg_t;


/** \brief aneg status   */
typedef struct vtss_aneg_status_t{
   BOOL aneg_complete;
   BOOL lp_aneg_able;
   BOOL incompatible_link;
}vtss_aneg_status_t;


/** \brief aneg advertisement for ability   */
typedef struct vtss_aneg_ability_t{
  u8     sel_field;
  u8     echoed_nonce;
  u8     pause;
  BOOL   rf;
  BOOL   np;
  u8     tx_nonce;
  BOOL   cap_1gkx;
  BOOL   cap_10gkx4;
  BOOL   cap_10gkr;
  u8     lsb_reserved;
}vtss_aneg_ability_t;


/** \brief aneg advertisement for ability   */
typedef struct vtss_aneg_next_page_t{
   u16    next_page_mlsb;
   u32    next_page_lsb;
}vtss_aneg_next_page_t;


/** \brief aneg state - container for vtss_state    */
typedef struct vtss_aneg_state_t{
    vtss_aneg_cfg_t     aneg_cfg;
    vtss_aneg_status_t  aneg_status;
    vtss_aneg_ability_t aneg_ability;
    /* atleast one next page is supported */   
    vtss_aneg_next_page_t  next_page;
}vtss_aneg_state_t;



/** \brief  aneg events */
typedef enum vtss_aneg_events_t {
    parallel_defect_fault = (1 << 0),
    incompatible_link     = (1 << 1),
    page_recvd            = (1 << 2),
    aneg_complete         = (1 << 3)    
} vtss_aneg_events_t;

/* ================================================================= *
 *  Defects/Events
 * ================================================================= */

/* ================================================================= *
 *  Dynamic Config
 * ================================================================= */

/**
 * \brief Set ANEG configuration.
 *
 * \param inst [IN]      Target instance reference.
 * \param port_no [IN]   Port number.
 * \param cfg [IN]       ANEG configuration.
 *
 * \return Return code.
 **/
vtss_rc vtss_aneg_set_config(const vtss_inst_t inst,
                          vtss_port_no_t port_no,
                          const vtss_aneg_cfg_t *const cfg
                          );


/**
 * \brief Get ANEG configuration.
 *
 * \param inst [IN]      Target instance reference.
 * \param port_no [IN]   Port number.
 * \param cfg [OUT]      ANEG configuration.
 *
 * \return Return code.
 **/
vtss_rc vtss_aneg_get_config(const vtss_inst_t inst,
                          vtss_port_no_t port_no,
                          vtss_aneg_cfg_t *const cfg
                          );


/**
 * \brief Set ANEG ability.
 *
 * \param inst [IN]      Target instance reference.
 * \param port_no [IN]   Port number.
 * \param ability [IN]   ANEG ability.
 *
 * \return Return code.
 **/
vtss_rc vtss_aneg_set_ability(const vtss_inst_t inst,
                          vtss_port_no_t port_no,
                          const vtss_aneg_ability_t *const ability
                          );
                          

/**
 * \brief Get ANEG ability.
 *
 * \param inst [IN]      Target instance reference.
 * \param port_no [IN]   Port number.
 * \param ability [OUT]  ANEG ability.
 *
 * \return Return code.
 **/
vtss_rc vtss_aneg_get_ability(const vtss_inst_t inst,
                          vtss_port_no_t port_no,
                          vtss_aneg_ability_t *const ability
                          );
                          

/**
 * \brief Get ANEG next page.
 *
 * \param inst [IN]         Target instance reference.
 * \param port_no [IN]      Port number.
 * \param next_page [OUT]   ANEG next page value.
 *
 * \return Return code.
 **/
vtss_rc vtss_aneg_get_next_page(const vtss_inst_t inst,
                          vtss_port_no_t port_no,
                          vtss_aneg_next_page_t *const next_page
                          );                          
                          
                          
/**
 * \brief Set ANEG next page.
 *
 * \param inst [IN]         Target instance reference.
 * \param port_no [IN]      Port number.
 * \param next_page [IN]    ANEG next page value.
 *
 * \return Return code.
 **/
vtss_rc vtss_aneg_set_next_page(const vtss_inst_t inst,
                          vtss_port_no_t port_no,
                          const vtss_aneg_next_page_t *const next_page
                          );          
                          

/**
 * \brief Enable ANEG.
 *
 * \param inst [IN]      Target instance reference.
 * \param port_no [IN]   Port number.
 * \param enable [IN]    TRUE => Enable, FALSE => Disable.
 *
 * \return Return code.
 **/
vtss_rc vtss_aneg_set_enable(const vtss_inst_t inst,
                          vtss_port_no_t port_no,
                          BOOL enable
                          );                                          
                          

/**
 * \brief Reset ANEG.
 *
 * \param inst [IN]      Target instance reference.
 * \param port_no [IN]   Port number.
 *
 * \return Return code.
 **/
vtss_rc vtss_aneg_reset(const vtss_inst_t inst,
                          vtss_port_no_t port_no                          
                          );                          
                          
/**
 * \brief Restart ANEG.
 *
 * \param inst [IN]      Target instance reference.
 * \param port_no [IN]   Port number.
 *
 * \return Return code.
 **/
vtss_rc vtss_aneg_restart(const vtss_inst_t inst,
                          vtss_port_no_t port_no                          
                          );                                                    
/* ================================================================= *
 *  State Reporting
 * ================================================================= */

/**
 * \brief Get ANEG events.
 *
 * \param inst [IN]         Target instance reference.
 * \param port_no [IN]      Port number.
 * \param events [OUT]      pointer to event status structure.
 *
 * \return Return code.
 **/
vtss_rc vtss_aneg_events_get(vtss_inst_t inst,
                          vtss_port_no_t port_no,
                          vtss_aneg_events_t *const events);
                          
                          
/**
 * \brief Get ANEG status.
 *
 * \param inst [IN]         Target instance reference.
 * \param port_no [IN]      Port number.
 * \param status [OUT]      pointer to ANEG status structure.
 *
 * \return Return code.
 **/
vtss_rc vtss_aneg_get_status(vtss_inst_t inst,
                          vtss_port_no_t port_no,
                          vtss_aneg_status_t *const status);                          
                          
/**
 * \brief Gt ANEG lp ability.
 *
 * \param inst [IN]         Target instance reference.
 * \param port_no [IN]      Port number.
 * \param lp_ability [OUT]  pointer to ANEG lp ability status structure.
 *
 * \return Return code.
 **/
vtss_rc vtss_aneg_get_lp_ability(vtss_inst_t inst,
                          vtss_port_no_t port_no,
                          vtss_aneg_ability_t *const lp_ability);
/* ================================================================= *
 *  Performance Primitives
 * ================================================================= */

#ifdef __cplusplus
}
#endif
#endif /* VTSS_FEATURE_ANEG */
#endif /* _VTSS_ANEG_API_H_ */
