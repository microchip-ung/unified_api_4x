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
 * \file vtss_ae_api.h
 * \brief ae API
 */

#ifndef _VTSS_AE_API_H_
#define _VTSS_AE_API_H_

#include <vtss/api/types.h>

#if defined(VTSS_FEATURE_AE)
#ifdef __cplusplus
extern "C" {
#endif

/* ================================================================= *
 *  Data structures / types
 * ================================================================= */
 
typedef u8 vtss_ae_lane_t;          /**< AE - lane id */
typedef u32 vtss_ae_eye_data_val_t; /**< AE - eye data */
#define VTSS_AE_LANE_CNT 16         /**< Number og AE lanes */



/** \brief  ae protocol info */
typedef enum vtss_ae_eye_data_t {
    AMIN,
    GAIN,
    H1T,
    DATA_EDGE,
    DFE_SAMPLE        
} vtss_ae_eye_data_t;



/** \brief ae config data   */
typedef struct ae_prbs_cfg_t{
		BOOL gen_off;
		BOOL checker_off; 
}ae_prbs_cfg_t;

/** \brief ae config data   */
typedef struct ae_fsm_cfg_t{
	    BOOL training_enable;
	    BOOL disable_coeff_update_fsm;	
}ae_fsm_cfg_t;


 
/** \brief ae config data   */
typedef struct vtss_ae_cfg_t{      
    BOOL MDIO_override;
	BOOL max_wait_timer_disable;
	BOOL manual_rdy;
	u8 manchester_decode;
    u8 peak_amplitude;
    u8 dfe_mode;
    ae_fsm_cfg_t fsm_cfg;
    ae_prbs_cfg_t prbs_cfg;
}vtss_ae_cfg_t;

/** \brief ae protocol data   */
typedef struct vtss_ae_protocol_data{ 
    /* just a cache of some saved registers for later use */
    u32 reg_save[5];
}vtss_ae_protocol_data;

/** \brief ae status   */
typedef struct vtss_ae_prbs_status_t{
	BOOL checker_sync;
	u32 checker_err_cnt;
	u32 sync_cnt;
}vtss_ae_prbs_status_t;

/** \brief ae status   */
typedef struct vtss_ae_status_t{
    BOOL training_ok;
    vtss_ae_prbs_status_t prbs_status;    
}vtss_ae_status_t;

/** \brief ae state - container for vtss_state    */
typedef struct vtss_ae_state_t{
    vtss_ae_cfg_t      ae_cfg;
    vtss_ae_status_t   ae_status;
    vtss_ae_protocol_data  ae_protocol_data;
}vtss_ae_state_t;


/** \brief ae ieee802.3ap config data   */
typedef struct ae_coeff_updt_req_t {
	BOOL preset;
    BOOL init;
    u8 c0_action;
    u8 c1_action;
    u8 c2_action;
}ae_coeff_updt_req_t;

/** \brief ae ieee802.3ap config data   */
typedef struct ae_coeff_updt_stat_t{
	BOOL  rx_rdy;
    u8 c0_action;   
    u8 c1_action;
    u8 c2_action;
}ae_coeff_updt_stat_t;


/** \brief  ae events */
typedef enum vtss_ae_events_t {
        dummy_ae,
} vtss_ae_events_t;

/* ================================================================= *
 *  Defects/Events
 * ================================================================= */

/* ================================================================= *
 *  Dynamic Config
 * ================================================================= */

/**
 * \brief Initialize AE.
 *
 * \param inst [IN]         Target instance reference.
 * \param ae_lane_no [IN]   Lane number.
 *
 * \return Return code.
 **/
vtss_rc vtss_ae_init(vtss_inst_t inst,
                          vtss_ae_lane_t ae_lane_no);
/**
 * \brief Set AE configuration.
 *
 * \param inst [IN]         Target instance reference.
 * \param ae_lane_no [IN]   Lane number.
 * \param cfg [IN]          Lane configuration.
 *
 * \return Return code.
 **/
vtss_rc vtss_ae_set_config(const vtss_inst_t inst,
                          vtss_ae_lane_t ae_lane_no,
                          const vtss_ae_cfg_t *const cfg
                          );


/**
 * \brief Get AE configuration.
 *
 * \param inst [IN]         Target instance reference.
 * \param ae_lane_no [IN]   Lane number.
 * \param cfg [OUT]         Lane configuration.
 *
 * \return Return code.
 **/
vtss_rc vtss_ae_get_config(const vtss_inst_t inst,
                          vtss_ae_lane_t ae_lane_no,
                          vtss_ae_cfg_t *const cfg
                          );
             
             
/**
 * \brief Set AE coefficients.
 *
 * \param inst [IN]         Target instance reference.
 * \param ae_lane_no [IN]   Lane number.
 * \param req [IN]          Lane coefficient.
 *
 * \return Return code.
 **/
vtss_rc vtss_ae_set_coeff_update_req(const vtss_inst_t inst,
                          vtss_ae_lane_t ae_lane_no,const u32 req);
         
                          

             
/**
 * \brief Get AE coefficients.
 *
 * \param inst [IN]         Target instance reference.
 * \param ae_lane_no [IN]   Lane number.
 * \param req [OUT]         Lane coefficient status.
 *
 * \return Return code.
 **/
vtss_rc vtss_ae_get_coeff_update_stat(const vtss_inst_t inst,
                          vtss_ae_lane_t ae_lane_no, u32  *const stat);


/**
 * \brief Get AE Eye Data.
 *
 * \param inst [IN]         Target instance reference.
 * \param ae_lane_no [IN]   Lane number.
 * \param cmd [IN]          Lane eye data command.
 * \param retval [OUT]      Lane eye data value.
 *
 * \return Return code.
 **/
vtss_rc vtss_ae_get_eye_data(const vtss_inst_t inst,
                          vtss_ae_lane_t ae_lane_no, const vtss_ae_eye_data_t  cmd,vtss_ae_eye_data_val_t *const retval);

/**
 * \brief Initialize AE receiver.
 *
 * \param inst [IN]         Target instance reference.
 * \param ae_lane_no [IN]   Lane number.
 *
 * \return Return code.
 **/
vtss_rc vtss_ae_init_rx(const vtss_inst_t inst,
                          vtss_ae_lane_t ae_lane_no );


/**
 * \brief Initialize AE transmitter.
 *
 * \param inst [IN]         Target instance reference.
 * \param ae_lane_no [IN]   Lane number.
 *
 * \return Return code.
 **/
vtss_rc vtss_ae_init_tx(const vtss_inst_t inst,
                          vtss_ae_lane_t ae_lane_no );

/**
 * \brief Return AE receiver config.
 *
 * \param inst [IN]         Target instance reference.
 * \param ae_lane_no [IN]   Lane number.
 *
 * \return Return code.
 **/
vtss_rc vtss_ae_return_rx_cfg(const vtss_inst_t inst,
                          vtss_ae_lane_t ae_lane_no );

                                            
/* ================================================================= *
 *  State Reporting
 * ================================================================= */

/**
 * \brief Get AE events.
 *
 * \param inst [IN]         Target instance reference.
 * \param ae_lane_no [IN]   Lane number.
 * \param events [OUT]      pointer to event status structure.
 *
 * \return Return code.
 **/
vtss_rc vtss_ae_events_get(vtss_inst_t inst,
                          vtss_ae_lane_t ae_lane_no,
                          vtss_ae_events_t *const events);
                          
                          
/**
 * \brief Get AE status.
 *
 * \param inst [IN]         Target instance reference.
 * \param ae_lane_no [IN]   Lane number.
 * \param status [OUT]      pointer to AE lane status structure.
 *
 * \return Return code.
 **/
vtss_rc vtss_ae_get_status(vtss_inst_t inst,
                          vtss_ae_lane_t ae_lane_no,
                          vtss_ae_status_t *const status);
                          

/* ================================================================= *
 *  Performance Primitives
 * ================================================================= */

#ifdef __cplusplus
}
#endif
#endif /* VTSS_FEATURE_AE */
#endif /* _VTSS_AE_API_H_ */
