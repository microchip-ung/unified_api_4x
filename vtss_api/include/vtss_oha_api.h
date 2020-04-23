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
 * \file vtss_oha_api.h
 * \brief OHA API
 */

#ifndef _VTSS_OHA_API_H_
#define _VTSS_OHA_API_H_

#include <vtss/api/types.h>

#if defined(VTSS_FEATURE_OHA)
#ifdef __cplusplus
extern "C" {
#endif

/* ================================================================= *
 *  Data structures / types
 * ================================================================= */
/** \brief WIS TOSI/ROSI or OTU2 OH A/D mode types */
typedef enum {
    VTSS_OHA_TOSI_ROSI_NONE,     /**< not in use */
    VTSS_OHA_TOSI_ROSI_EWIS2,    /**< select TOSI/ROSI from eWIS2 */
    VTSS_OHA_TOSI_ROSI_OTU2_1,   /**< select OTU2 OH from OTU2-1 */
    VTSS_OHA_TOSI_ROSI_MAX
} vtss_oha_tosi_rosi_mode_t;

/** \brief OTU2 OH A/D mode types */
typedef enum {
    VTSS_OHA_OTU2_NONE,     /**< not in use */
    VTSS_OHA_OTU2_OTU2_1,   /**< select OTU2 OH from OTU2-1 */
    VTSS_OHA_OTU2_OTU2_2,   /**< select OTU2 OH from OTU2-2 */
    VTSS_OHA_OTU2_MAX
} vtss_oha_otu2_mode_t;

/** \brief OTU2 gap_clk_out mode types */
typedef enum {
    VTSS_OHA_GAP_CLK_NONE,     /**< not in use */
    VTSS_OHA_GAP_CLK_OTU2_1,   /**< select OTU2 GAP_CLK from OTU2-1 */
    VTSS_OHA_GAP_CLK_OTU2_2,   /**< select OTU2 GAP_CLK from OTU2-2 */
    VTSS_OHA_GAP_CLK_MAX
} vtss_oha_gap_clk_out_t;

/** \brief oha channels configuration */
typedef struct vtss_oha_cfg_s {
    vtss_oha_tosi_rosi_mode_t tosi_rosi_otu2_1;   /**< Configuration for the WIS TOSI/ROSI or OTU2 OH A/D */
    vtss_oha_otu2_mode_t otu2_1_otu2_2;           /**< Configuration for the OTU2 OH A/D */
    vtss_oha_gap_clk_out_t gap_clk_out;           /**< Configuration for the OTU2 gap_clk_out */
} vtss_oha_cfg_t;

/** \brief oha state - container for vtss_state    */
typedef struct vtss_oha_state_t {
    vtss_oha_cfg_t     oha_cfg;    /**< oha config data */
} vtss_oha_state_t;

/* ================================================================= *
 *  Defects/Events
 * ================================================================= */

/* ================================================================= *
 *  Dynamic Config
 * ================================================================= */


/**
* \brief OHA channels and clock setup.
*  For each channel in Daytona there are 2 OHA serial channels and clock output that can be configured.
* The function sets the multiplexer for the OHA serial channels. Selecting a channel for eWIS will 
* enables the OHA for that block
*
* \param inst [IN]      Target instance reference.
* \param port_no [IN]   Port number.
* \param cfg [IN]       pointer to OHA channels and clock setup.
*
* \return Return code.
**/

vtss_rc vtss_oha_config_set(const vtss_inst_t inst,
                            vtss_port_no_t port_no,
                            const vtss_oha_cfg_t *const cfg
                          );

/**
 * \brief  Get OHA channels configuration.
 *
 * \param inst [IN]      Target instance reference.
 * \param port_no [IN]   Port number.
 * \param cfg [IN]       pointer to OHA channels and clock setup.
 *
 * \return Return code.
 **/
vtss_rc vtss_oha_config_get(const vtss_inst_t inst,
                            vtss_port_no_t port_no,
                            vtss_oha_cfg_t *const cfg
                           );
             
/* ================================================================= *
 *  State Reporting
 * ================================================================= */


/* ================================================================= *
 *  Performance Primitives
 * ================================================================= */

#ifdef __cplusplus
}
#endif
#endif /* VTSS_FEATURE_OHA */
#endif /* _VTSS_OHA_API_H_ */
