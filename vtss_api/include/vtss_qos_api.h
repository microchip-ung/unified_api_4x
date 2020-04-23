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
 * \file
 * \brief QoS API
 * \details This header file describes Quality of Service functions
 */

#ifndef _VTSS_QOS_API_H_
#define _VTSS_QOS_API_H_

#include <vtss/api/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined(VTSS_FEATURE_QOS)

#if defined(VTSS_FEATURE_QOS_WRED)
/**
 * \brief Random Early Detection configuration struct version 1 (per port, per queue)
 **/
typedef struct
{
    BOOL       enable;          /**< Enable/disable RED */
#if defined(VTSS_ARCH_JAGUAR_1)
    vtss_pct_t max_th;          /**< Maximum threshold */
    vtss_pct_t min_th;          /**< Minimum threshold */
#endif
    vtss_pct_t max_prob_1;      /**< Drop probability at max_th for drop precedence level 1 */
    vtss_pct_t max_prob_2;      /**< Drop probability at max_th for drop precedence level 2 */
    vtss_pct_t max_prob_3;      /**< Drop probability at max_th for drop precedence level 3 */
} vtss_red_t;
#endif /* defined(VTSS_FEATURE_QOS_WRED) */

#if defined(VTSS_FEATURE_QOS_WRED_V2) || defined(VTSS_FEATURE_QOS_WRED_V3)
/**
 * \brief Random Early Detection version 2. Select if max means max drop probability or max fill level
 **/
typedef enum {
    VTSS_WRED_V2_MAX_DP, /**< Unit for max is drop probability */
    VTSS_WRED_V2_MAX_FL  /**< Unit for max is fill level */
} vtss_wred_v2_max_t;

/**
 * \brief Random Early Detection configuration struct version 2 (per queue, per dpl - switch global)
 **/
typedef struct
{
    BOOL               enable;   /**< Enable/disable RED */
    vtss_pct_t         min_fl;   /**< Minimum fill level */
    vtss_pct_t         max;      /**< Maximum drop probability or fill level - selected by max_unit */
    vtss_wred_v2_max_t max_unit; /**< Selects the unit for max */
} vtss_red_v2_t;
#endif /* defined(VTSS_FEATURE_QOS_WRED_V2) || defined(VTSS_FEATURE_QOS_WRED_V3) */

#if defined(VTSS_FEATURE_QOS_WRED_V3)
#define VTSS_WRED_DPL_CNT    3           /**< Number of dpl's. There are no profile for dpl 0, only for dpl 1 to 3 */
#define VTSS_WRED_GROUP_CNT  3           /**< Number of groups */
    typedef vtss_red_v2_t vtss_red_v3_t; /**< Version 3 uses the v2 struct (per queue, per dpl, per group - switch global */
#endif /* defined(VTSS_FEATURE_QOS_WRED_V3) */

/**
 * \brief All parameters below are defined per chip
 **/
typedef struct
{
    vtss_prio_t          prios;                                   /**< Number of priorities (1/2/4/8) */

#if defined(VTSS_FEATURE_QOS_CLASSIFICATION_V2)
    BOOL                 dscp_trust[64];                          /**< Ingress: Only trusted DSCP values are used for QOS class and DP level classification  */
    vtss_prio_t          dscp_qos_class_map[64];                  /**< Ingress: Mapping from DSCP value to QOS class  */
    vtss_dp_level_t      dscp_dp_level_map[64];                   /**< Ingress: Mapping from DSCP value to DP level */

    vtss_dscp_t          dscp_qos_map[VTSS_PRIO_ARRAY_SIZE];      /**< Ingress: Mapping from QoS class to DSCP (DP unaware or DP level = 0) */
#if defined(VTSS_FEATURE_QOS_DSCP_CLASS_DP_AWARE)
    vtss_dscp_t          dscp_qos_map_dp1[VTSS_PRIO_ARRAY_SIZE];  /**< Ingress: Mapping from QoS class to DSCP (DP aware and DP level = 1) */
#if VTSS_DPLS == 4
    vtss_dscp_t          dscp_qos_map_dp2[VTSS_PRIO_ARRAY_SIZE];  /**< Ingress: Mapping from QoS class to DSCP (DP aware and DP level = 2) */
    vtss_dscp_t          dscp_qos_map_dp3[VTSS_PRIO_ARRAY_SIZE];  /**< Ingress: Mapping from QoS class to DSCP (DP aware and DP level = 3) */
#endif /* VTSS_DPLS == 4 */
#endif /* defined(VTSS_FEATURE_QOS_DSCP_CLASS_DP_AWARE) */
#endif /* defined(VTSS_FEATURE_QOS_CLASSIFICATION_V2) */

#if defined(VTSS_FEATURE_QOS_DSCP_REMARK)
    BOOL                 dscp_remark[64];                         /**< Ingress: DSCP remarking enable. Used when port.dscp_mode = VTSS_DSCP_MODE_SEL */
#if defined(VTSS_FEATURE_QOS_DSCP_REMARK_V2)
    vtss_dscp_t          dscp_translate_map[64];                  /**< Ingress: Translated DSCP value. Used when port.dscp_translate = TRUE) */
    vtss_dscp_t          dscp_remap[64];                          /**< Egress: Remap one DSCP to another (DP unaware or DP level = 0) */
#if defined(VTSS_FEATURE_QOS_DSCP_REMARK_DP_AWARE)
    vtss_dscp_t          dscp_remap_dp1[64];                      /**< Egress: Remap one DSCP to another (DP aware and DP level = 1) */
#endif /* defined(VTSS_FEATURE_QOS_DSCP_REMARK_DP_AWARE) */
#endif /* defined(VTSS_FEATURE_QOS_DSCP_REMARK_V2) */
#endif /* defined(VTSS_FEATURE_QOS_DSCP_REMARK) */

#if defined(VTSS_FEATURE_QOS_POLICER_CPU_SWITCH)
    vtss_packet_rate_t   policer_mac;                             /**< MAC table CPU policer */
    vtss_packet_rate_t   policer_cat;                             /**< BPDU, GARP, IGMP, IP MC and MLD CPU policer */
    vtss_packet_rate_t   policer_learn;                           /**< Learn frame policer */
#endif /* defined(VTSS_FEATURE_QOS_POLICER_CPU_SWITCH) */
#if defined(VTSS_FEATURE_QOS_POLICER_UC_SWITCH)
    vtss_packet_rate_t        policer_uc;                         /**< Unicast packet storm policer */
    BOOL                      policer_uc_frame_rate;              /**< FALSE: Unit is kbps. TRUE: Unit is fps */
    vtss_storm_policer_mode_t policer_uc_mode;                    /**< Unicast packet storm policer mode */
#endif /* defined(VTSS_FEATURE_QOS_POLICER_UC_SWITCH) */
#if defined(VTSS_FEATURE_QOS_POLICER_MC_SWITCH)
    vtss_packet_rate_t        policer_mc;                         /**< Multicast packet storm policer */
    BOOL                      policer_mc_frame_rate;              /**< FALSE: Unit is kbps. TRUE: Unit is fps */
    vtss_storm_policer_mode_t policer_mc_mode;                    /**< Multicast packet storm policer mode */
#endif /* defined(VTSS_FEATURE_QOS_POLICER_MC_SWITCH) */
#if defined(VTSS_FEATURE_QOS_POLICER_BC_SWITCH)
    vtss_packet_rate_t        policer_bc;                         /**< Broadcast packet storm policer */
    BOOL                      policer_bc_frame_rate;              /**< FALSE: Unit is kbps. TRUE: Unit is fps */
    vtss_storm_policer_mode_t policer_bc_mode;                    /**< Broadcast packet storm policer mode */
#endif /* defined(VTSS_FEATURE_QOS_POLICER_BC_SWITCH) */

#if defined(VTSS_FEATURE_QOS_WRED_V2)
    vtss_red_v2_t        red_v2[VTSS_QUEUE_ARRAY_SIZE][2];        /**< Random Early Detection - per queue (0..7), per dpl (0..1) */
#endif /* defined(VTSS_FEATURE_QOS_WRED_V2) */

#if defined(VTSS_FEATURE_QOS_WRED_V3)
    vtss_red_v3_t        red_v3[VTSS_QUEUE_ARRAY_SIZE]            /**< Random Early Detection - per queue (0..7), per dpl (1..3), per group (0..2) */
                               [VTSS_WRED_DPL_CNT]
                               [VTSS_WRED_GROUP_CNT];
#endif /* defined(VTSS_FEATURE_QOS_WRED_V3) */
} vtss_qos_conf_t;

/**
 * \brief Get QoS setup for switch.
 *
 * \param inst [IN]   Target instance reference.
 * \param conf [OUT]  QoS setup structure.
 *
 * \return Return code.
 **/
vtss_rc vtss_qos_conf_get(const vtss_inst_t  inst,
                          vtss_qos_conf_t    *const conf);

/**
 * \brief Set QoS setup for switch.
 *
 * \param inst [IN]  Target instance reference.
 * \param conf [IN]  QoS setup structure.
 *
 * \return Return code.
 **/
vtss_rc vtss_qos_conf_set(const vtss_inst_t      inst,
                          const vtss_qos_conf_t  *const conf);

/* Port policers */
#if defined(VTSS_ARCH_JAGUAR_1) || defined(VTSS_ARCH_JAGUAR_2)
#define VTSS_PORT_POLICERS 4 /**< Number of port policers */
#else
#define VTSS_PORT_POLICERS 1 /**< Number of port policers */
#endif /* VTSS_ARCH_JAGUAR_1/JAGUAR_2 */

/**
 * \brief Policer
 **/
typedef struct
{
    vtss_burst_level_t level;       /**< Burst level */
    vtss_bitrate_t     rate;        /**< Maximum rate */
} vtss_policer_t;

#if defined(VTSS_FEATURE_QOS_PORT_POLICER_EXT)

#define VTSS_PORT_POLICER_CPU_QUEUES 8 /**< Number of cpu queues pr port policer */

/**
 * \brief Policer Extensions
 **/
typedef struct
{
#if defined(VTSS_FEATURE_QOS_PORT_POLICER_EXT_FPS)
    BOOL               frame_rate;           /**< Measure rates in frames per seconds instead of bits per second */
#endif /* VTSS_FEATURE_QOS_PORT_POLICER_EXT_FPS */
#if defined(VTSS_FEATURE_QOS_PORT_POLICER_EXT_DPBL)
    vtss_dp_level_t    dp_bypass_level;      /**< Drop Predence bypass level */
#endif /* VTSS_FEATURE_QOS_PORT_POLICER_EXT_DPBL */
#if defined(VTSS_FEATURE_QOS_PORT_POLICER_EXT_TTM)
    BOOL               unicast;              /**< Unicast frames are policed */
    BOOL               multicast;            /**< Multicast frames are policed */
    BOOL               broadcast;            /**< Broadcast frames are policed */
    BOOL               uc_no_flood;          /**< Exclude flooding unicast frames (if unicast is set) */
    BOOL               mc_no_flood;          /**< Exclude flooding multicast frames (if multicast is set) */
    BOOL               flooded;              /**< Flooded frames are policed */
#endif /* VTSS_FEATURE_QOS_PORT_POLICER_EXT_TTM */
#if defined(VTSS_FEATURE_QOS_PORT_POLICER_EXT_TTM_V2)
    BOOL               known_unicast;        /**< Known unicast frames are policed */
    BOOL               known_multicast;      /**< Known multicast frames are policed */
    BOOL               known_broadcast;      /**< Known broadcast frames are policed */
    BOOL               unknown_unicast;      /**< Unknown unicast frames are policed */
    BOOL               unknown_multicast;    /**< Unknown multicast frames are policed */
    BOOL               unknown_broadcast;    /**< Unknown broadcast frames are policed */
#endif /* VTSS_FEATURE_QOS_PORT_POLICER_EXT_TTM */
#if defined(VTSS_FEATURE_QOS_PORT_POLICER_EXT_TTM) || defined(VTSS_FEATURE_QOS_PORT_POLICER_EXT_TTM_V2)
    BOOL               learning;             /**< Learning frames are policed */
    BOOL               to_cpu;               /**< Frames to the CPU are policed */
    BOOL               cpu_queue[VTSS_PORT_POLICER_CPU_QUEUES]; /**< Enable each individual CPU queue (if to_cpu is set) */
    BOOL               limit_noncpu_traffic; /**< Remove the front ports from the destination set for a policed frame */
    BOOL               limit_cpu_traffic;    /**< Remove the CPU ports from the destination set for a policed frame */
#endif /* VTSS_FEATURE_QOS_PORT_POLICER_EXT_TTM || VTSS_FEATURE_QOS_PORT_POLICER_EXT_TTM_V2 */
#if defined(VTSS_FEATURE_QOS_PORT_POLICER_EXT_FC)
    BOOL               flow_control;         /**< Flow control is enabled */
#endif /* VTSS_FEATURE_QOS_PORT_POLICER_EXT_FC */
} vtss_policer_ext_t;
#endif /* VTSS_FEATURE_QOS_PORT_POLICER_EXT */

#if defined(VTSS_FEATURE_QOS_POLICER_DLB)
/** \brief Dual leaky buckets policer configuration */
typedef struct {
    vtss_policer_type_t type;      /**< Policer type */
    BOOL                enable;    /**< Enable/disable policer */
#if defined(VTSS_ARCH_JAGUAR_1) || defined(VTSS_ARCH_JAGUAR_2)
    BOOL                cm;        /**< Colour Mode (TRUE means colour aware) */
#endif /* VTSS_ARCH_JAGUAR_1/JAGUAR_2 */
    BOOL                cf;        /**< Coupling Flag */
    BOOL                line_rate; /**< Line rate policing (default is data rate policing) */
    vtss_bitrate_t      cir;       /**< Committed Information Rate */
    vtss_burst_level_t  cbs;       /**< Committed Burst Size */
    vtss_bitrate_t      eir;       /**< Excess Information Rate */
    vtss_burst_level_t  ebs;       /**< Excess Burst Size */
} vtss_dlb_policer_conf_t;
#endif /* VTSS_FEATURE_QOS_POLICER_DLB */

#if defined(VTSS_ARCH_CARACAL)
/**
 * \brief Get MEP policer configuration.
 *
 * \param inst [IN]        Target instance reference.
 * \param port_no [IN]     Ingress port number.
 * \param prio [IN]        Selected priority (QoS class).
 * \param conf [OUT]       Policer configuration.
 *
 * \return Return code.
 **/
vtss_rc vtss_mep_policer_conf_get(const vtss_inst_t       inst,
                                  const vtss_port_no_t    port_no,
                                  const vtss_prio_t       prio,
                                  vtss_dlb_policer_conf_t *const conf);

/**
 * \brief Set MEP policer configuration.
 *
 * \param inst [IN]        Target instance reference.
 * \param port_no [IN]     Ingress port number.
 * \param prio [IN]        Selected priority (QoS class).
 * \param conf [IN]        Policer configuration.
 *
 * \return Return code.
 **/
vtss_rc vtss_mep_policer_conf_set(const vtss_inst_t             inst,
                                  const vtss_port_no_t          port_no,
                                  const vtss_prio_t             prio,
                                  const vtss_dlb_policer_conf_t *const conf);
#endif /* defined(VTSS_ARCH_CARACAL) */

#if defined(VTSS_FEATURE_QOS_EGRESS_SHAPERS_RT)
/**
 * \brief Shaper Accounting Mode
 **/
typedef enum
{
    VTSS_SHAPER_MODE_LINE, /**< Use line-rate for the shaper */
    VTSS_SHAPER_MODE_DATA  /**< Use data-rate for the shaper */
} vtss_shaper_mode_t;
#endif

/**
 * \brief Shaper
 **/
typedef struct
{
    vtss_burst_level_t level;          /**< CBS (Committed Burst Size).                 Unit: bytes */
    vtss_bitrate_t     rate;           /**< CIR (Committed Information Rate).           Unit: kbps. Use VTSS_BITRATE_DISABLED to disable shaper */
#if defined(VTSS_FEATURE_QOS_EGRESS_SHAPERS_DLB)
    vtss_burst_level_t ebs;            /**< EBS (Excess Burst Size).                    Unit: bytes */
    vtss_bitrate_t     eir;            /**< EIR (Excess Information Rate).              Unit: kbps. Use VTSS_BITRATE_DISABLED to disable DLB */
#endif /* VTSS_FEATURE_QOS_EGRESS_SHAPERS_DLB */
#if defined(VTSS_FEATURE_QOS_EGRESS_SHAPERS_RT)
    vtss_shaper_mode_t mode;           /**< RT (Rate type). Shaper rate type configuration: 0 = Line-rate, 1 = Data-rate */
#endif
} vtss_shaper_t;

/** \brief Tag Remark Mode */
typedef enum
{
    VTSS_TAG_REMARK_MODE_CLASSIFIED = 0, /**< Use classified PCP/DEI values */
    VTSS_TAG_REMARK_MODE_DEFAULT    = 2, /**< Use default (configured) PCP/DEI values */
    VTSS_TAG_REMARK_MODE_MAPPED     = 3  /**< Use mapped versions of classified QOS class and DP level */
} vtss_tag_remark_mode_t;

/**
 * \brief DSCP mode for ingress port
 **/
typedef enum
{
    VTSS_DSCP_MODE_NONE,   /**< DSCP not remarked */
    VTSS_DSCP_MODE_ZERO,   /**< DSCP value zero remarked */
    VTSS_DSCP_MODE_SEL,    /**< DSCP values selected above (dscp_remark) are remarked */
    VTSS_DSCP_MODE_ALL     /**< DSCP remarked for all values */
} vtss_dscp_mode_t;

/**
 * \brief DSCP mode for egress port
 **/
typedef enum
{
    VTSS_DSCP_EMODE_DISABLE,   /**< DSCP not remarked */
    VTSS_DSCP_EMODE_REMARK,    /**< DSCP remarked with DSCP value from analyzer */
    VTSS_DSCP_EMODE_REMAP,     /**< DSCP remarked with DSCP value from analyzer remapped through global remap table  */
#if defined(VTSS_FEATURE_QOS_DSCP_REMARK_DP_AWARE)
    VTSS_DSCP_EMODE_REMAP_DPA  /**< DSCP remarked with DSCP value from analyzer remapped through global remap dp aware tables */
#endif /* VTSS_FEATURE_QOS_DSCP_REMARK_DP_AWARE */
} vtss_dscp_emode_t;

/** \brief QoS setup per port **/
typedef struct
{
#if defined(VTSS_FEATURE_QOS_WRED)
    vtss_red_t     red[VTSS_QUEUE_ARRAY_SIZE];                   /**< Random Early Detection */
#endif /* VTSS_FEATURE_QOS_WRED */

    vtss_policer_t policer_port[VTSS_PORT_POLICERS];             /**< Ingress port policers */

#if defined(VTSS_FEATURE_QOS_PORT_POLICER_EXT)
    vtss_policer_ext_t policer_ext_port[VTSS_PORT_POLICERS];     /**< Ingress port policers extensions */
#endif /* VTSS_FEATURE_QOS_PORT_POLICER_EXT */

#if defined(VTSS_FEATURE_QOS_QUEUE_POLICER)
    vtss_policer_t     policer_queue[VTSS_QUEUE_ARRAY_SIZE];     /**< Ingress queue policers */
#endif /* VTSS_FEATURE_QOS_QUEUE_POLICER */

    vtss_shaper_t  shaper_port;                                  /**< Egress port shaper */

#if defined(VTSS_FEATURE_QOS_EGRESS_QUEUE_SHAPERS)
    vtss_shaper_t shaper_queue[VTSS_QUEUE_ARRAY_SIZE];           /**< Egress queue shapers */
#if defined(VTSS_FEATURE_QOS_EGRESS_QUEUE_SHAPERS_EB)
    BOOL          excess_enable[VTSS_QUEUE_ARRAY_SIZE];          /**< Allow this queue to use excess bandwidth */
#endif /* VTSS_FEATURE_QOS_EGRESS_QUEUE_SHAPERS_EB */
#endif  /* VTSS_FEATURE_QOS_EGRESS_QUEUE_SHAPERS */

#if defined(VTSS_FEATURE_LAYER2)         
    vtss_prio_t    default_prio;                                 /**< Default port priority (QoS class) */
    vtss_tagprio_t usr_prio;                                     /**< Default Ingress VLAN tag priority (PCP) */
#endif /* VTSS_FEATURE_LAYER2 */

#if defined(VTSS_FEATURE_QOS_CLASSIFICATION_V2)
    vtss_dp_level_t   default_dpl;                                             /**< Default Ingress Drop Precedence level */
    vtss_dei_t        default_dei;                                             /**< Default Ingress DEI value  */
    BOOL              tag_class_enable;                                        /**< Ingress classification of QoS class and DP level based PCP and DEI */
    vtss_prio_t       qos_class_map[VTSS_PCP_ARRAY_SIZE][VTSS_DEI_ARRAY_SIZE]; /**< Ingress mapping for tagged frames from PCP and DEI to QOS class  */
    vtss_dp_level_t   dp_level_map[VTSS_PCP_ARRAY_SIZE][VTSS_DEI_ARRAY_SIZE];  /**< Ingress mapping for tagged frames from PCP and DEI to DP level */
    BOOL              dscp_class_enable;                                       /**< Ingress classification of QoS class and DP level based on DSCP */
#endif /* VTSS_FEATURE_QOS_CLASSIFICATION_V2 */
                                        
#if defined(VTSS_FEATURE_QOS_DSCP_REMARK)
    vtss_dscp_mode_t dscp_mode;                       /**< Ingress DSCP mode */
#if defined(VTSS_FEATURE_QOS_DSCP_REMARK_V2)
    vtss_dscp_emode_t dscp_emode;                     /**< Egress DSCP mode */
    BOOL              dscp_translate;                 /**< Ingress: Translate DSCP value via dscp_translate_map[DSCP] before use */
#endif /* VTSS_FEATURE_QOS_DSCP_REMARK_V2 */
#endif /* VTSS_FEATURE_QOS_DSCP_REMARK */

#if defined(VTSS_FEATURE_QOS_TAG_REMARK_V2)
    vtss_tag_remark_mode_t tag_remark_mode;                      /**< Egress tag remark mode */
    vtss_tagprio_t         tag_default_pcp;                      /**< Default PCP value for Egress port */
    vtss_dei_t             tag_default_dei;                      /**< Default DEI value for Egress port */
    vtss_tagprio_t         tag_pcp_map[VTSS_PRIO_ARRAY_SIZE][2]; /**< Egress mapping from QOS class and (1 bit) DP level to PCP */
    vtss_dei_t             tag_dei_map[VTSS_PRIO_ARRAY_SIZE][2]; /**< Egress mapping from QOS class and (1 bit) DP level to DEI */
#endif /* VTSS_FEATURE_QOS_TAG_REMARK_V2 */

#if defined(VTSS_FEATURE_QOS_SCHEDULER_V2)
    BOOL       dwrr_enable;                                      /**< Enable Weighted fairness queueing */
#if defined(VTSS_FEATURE_QOS_SCHEDULER_DWRR_CNT)
    u8         dwrr_cnt;                                         /**< Number of queues, starting from queue 0, running in DWRR mode */
#endif /* VTSS_FEATURE_QOS_SCHEDULER_DWRR_CNT */
    vtss_pct_t queue_pct[VTSS_QUEUE_ARRAY_SIZE];                 /**< Queue percentages */
#endif /* VTSS_FEATURE_QOS_SCHEDULER_V2 */

#if defined(VTSS_FEATURE_QCL_DMAC_DIP)
    BOOL       dmac_dip;                                         /**< Enable DMAC/DIP matching in QCLs (default SMAC/SIP) */
#endif /* VTSS_FEATURE_QCL_DMAC_DIP */

#if defined(VTSS_FEATURE_QCL_KEY_TYPE)
    vtss_vcap_key_type_t key_type;                               /**< Key type for received frames */
#endif /* defined(VTSS_FEATURE_QCL_KEY_TYPE) */

#if defined(VTSS_FEATURE_QOS_WRED_V3)
    vtss_wred_group_t wred_group;                                /**< WRED group number - zero based */
#endif /* defined(VTSS_FEATURE_QOS_WRED_V3) */
} vtss_qos_port_conf_t;

/**
 * \brief Get QoS setup for port.
 *
 * \param inst [IN]     Target instance reference.
 * \param port_no [IN]  Port number.
 * \param conf [OUT]    QoS setup structure.
 *
 * \return Return code.
 **/
vtss_rc vtss_qos_port_conf_get(const vtss_inst_t     inst,
                               const vtss_port_no_t  port_no,
                               vtss_qos_port_conf_t  *const conf);

/**
 * \brief Set QoS setup for port.
 *
 * \param inst [IN]    Target instance reference.
 * \param port_no [IN] Port number.
 * \param conf [IN]    QoS setup structure.
 *
 * \return Return code.
 **/
vtss_rc vtss_qos_port_conf_set(const vtss_inst_t           inst,
                               const vtss_port_no_t        port_no,
                               const vtss_qos_port_conf_t  *const conf);

#if defined(VTSS_FEATURE_QCL)
/**
 * \brief QCL ID type
 **/
typedef u32 vtss_qcl_id_t; 

#define VTSS_QCL_IDS        1                                  /**< Number of QCLs */
#define VTSS_QCL_ID_START   0                                  /**< QCL ID start number */
#define VTSS_QCL_ID_END     (VTSS_QCL_ID_START + VTSS_QCL_IDS) /**< QCL ID end number */
#define VTSS_QCL_ARRAY_SIZE VTSS_QCL_ID_END                    /**< QCL ID array size */

#define VTSS_QCE_ID_LAST 0 /**< Special value used to add last in list */

/** \brief QoS Control Entry type */
typedef enum
{
    VTSS_QCE_TYPE_ANY,     /**< Any frame type */
    VTSS_QCE_TYPE_ETYPE,   /**< Ethernet Type */
    VTSS_QCE_TYPE_LLC,     /**< LLC */
    VTSS_QCE_TYPE_SNAP,    /**< SNAP */
    VTSS_QCE_TYPE_IPV4,    /**< IPv4 */
    VTSS_QCE_TYPE_IPV6     /**< IPv6 */
} vtss_qce_type_t;

/** \brief QCE MAC information */
typedef struct 
{
    vtss_vcap_bit_t dmac_mc; /**< Multicast DMAC */
    vtss_vcap_bit_t dmac_bc; /**< Broadcast DMAC */
#if defined(VTSS_FEATURE_QCL_KEY_DMAC)
    vtss_vcap_u48_t dmac;    /**< DMAC - Serval: key_type = mac_ip_addr */
#endif /* defined(VTSS_FEATURE_QCL_KEY_DMAC) */
    vtss_vcap_u48_t smac;    /**< SMAC - Only the 24 most significant bits (OUI) are supported on Jaguar1, rest are wildcards */
} vtss_qce_mac_t;

/** \brief QCE tag information */
typedef struct 
{
    vtss_vcap_vr_t  vid;    /**< VLAN ID (12 bit) */
    vtss_vcap_u8_t  pcp;    /**< PCP (3 bit) */
    vtss_vcap_bit_t dei;    /**< DEI */
    vtss_vcap_bit_t tagged; /**< Tagged/untagged frame */
#if defined(VTSS_FEATURE_QCL_KEY_S_TAG)
    vtss_vcap_bit_t s_tag;  /**< S-tagged/C-tagged frame */
#endif /* defined(VTSS_FEATURE_QCL_KEY_S_TAG) */
} vtss_qce_tag_t;

/** \brief Frame data for VTSS_QCE_TYPE_ETYPE */
typedef struct 
{
    vtss_vcap_u16_t etype; /**< Ethernet Type value */
    vtss_vcap_u32_t data;  /**< MAC data */ 
} vtss_qce_frame_etype_t;

/** \brief Frame data for VTSS_QCE_TYPE_LLC */
typedef struct 
{
    vtss_vcap_u48_t data; /**< Data */
} vtss_qce_frame_llc_t;

/** \brief Frame data for VTSS_QCE_TYPE_SNAP */
typedef struct 
{
    vtss_vcap_u48_t data; /**< Data */
} vtss_qce_frame_snap_t;

/** \brief Frame data for VTSS_QCE_TYPE_IPV4 */
typedef struct 
{
    vtss_vcap_bit_t fragment; /**< Fragment */
    vtss_vcap_vr_t  dscp;     /**< DSCP field (6 bit) */
    vtss_vcap_u8_t  proto;    /**< Protocol */
    vtss_vcap_ip_t  sip;      /**< Source IP address - Serval: key_type = normal, ip_addr and mac_ip_addr */
#if defined(VTSS_FEATURE_QCL_KEY_DIP)
    vtss_vcap_ip_t  dip;      /**< Destination IP address - Serval: key_type = ip_addr and mac_ip_addr */
#endif /* defined(VTSS_FEATURE_QCL_KEY_DIP) */
    vtss_vcap_vr_t  sport;    /**< UDP/TCP: Source port - Serval: key_type = normal, ip_addr and mac_ip_addr */
    vtss_vcap_vr_t  dport;    /**< UDP/TCP: Destination port - Serval: key_type = double_tag, ip_addr and mac_ip_addr */
} vtss_qce_frame_ipv4_t;

/** \brief Frame data for VTSS_QCE_TYPE_IPV6 */
typedef struct 
{
    vtss_vcap_vr_t   dscp;    /**< DSCP field (6 bit) */
    vtss_vcap_u8_t   proto;   /**< Protocol */
    vtss_vcap_u128_t sip;     /**< Source IP address (32 LSB on L26 and J1, 64 LSB on Serval when key_type = mac_ip_addr) */
#if defined(VTSS_FEATURE_QCL_KEY_DIP)
    vtss_vcap_u128_t dip;     /**< Destination IP address - 64 LSB on Serval when key_type = mac_ip_addr */
#endif /* defined(VTSS_FEATURE_QCL_KEY_DIP) */
    vtss_vcap_vr_t   sport;   /**< UDP/TCP: Source port - Serval: key_type = normal, ip_addr and mac_ip_addr */
    vtss_vcap_vr_t   dport;   /**< UDP/TCP: Destination port - Serval: key_type = double_tag, ip_addr and mac_ip_addr */
} vtss_qce_frame_ipv6_t;

/**
 * \brief QCE key 
 **/
typedef struct
{
    BOOL            port_list[VTSS_PORT_ARRAY_SIZE]; /**< Port list */ 
    vtss_qce_mac_t  mac;                             /**< MAC */
    vtss_qce_tag_t  tag;                             /**< Tag */
#if defined(VTSS_FEATURE_QCL_KEY_INNER_TAG)
    vtss_qce_tag_t  inner_tag;                       /**< Inner tag */
#endif /* defined(VTSS_FEATURE_QCL_KEY_INNER_TAG) */
    vtss_qce_type_t type;                            /**< Frame type */

    union
    {
        /* VTSS_QCE_TYPE_ANY: No specific fields */
        vtss_qce_frame_etype_t etype; /**< VTSS_QCE_TYPE_ETYPE */
        vtss_qce_frame_llc_t   llc;   /**< VTSS_QCE_TYPE_LLC */
        vtss_qce_frame_snap_t  snap;  /**< VTSS_QCE_TYPE_SNAP */
        vtss_qce_frame_ipv4_t  ipv4;  /**< VTSS_QCE_TYPE_IPV4 */
        vtss_qce_frame_ipv6_t  ipv6;  /**< VTSS_QCE_TYPE_IPV6 */
    } frame; /**< Frame type specific data */
} vtss_qce_key_t;

/**
 * \brief QCE action 
 **/
typedef struct
{
    BOOL                 prio_enable;      /**< Enable priority classification */
    vtss_prio_t          prio;             /**< Priority value */
    BOOL                 dp_enable;        /**< Enable DP classification */
    vtss_dp_level_t      dp;               /**< DP value */
    BOOL                 dscp_enable;      /**< Enable DSCP classification */
    vtss_dscp_t          dscp;             /**< DSCP value */
#if (defined VTSS_FEATURE_QCL_PCP_DEI_ACTION)
    BOOL                 pcp_dei_enable;   /**< Enable PCP and DEI classification */
    vtss_tagprio_t       pcp;              /**< PCP value */
    vtss_dei_t           dei;              /**< DEI value */
#endif /* (defined VTSS_FEATURE_QCL_PCP_DEI_ACTION) */
#if (defined VTSS_FEATURE_QCL_POLICY_ACTION)
    BOOL                 policy_no_enable; /**< Enable ACL policy classification */
    vtss_acl_policy_no_t policy_no;        /**< ACL policy number */
#endif /* VTSS_FEATURE_QCL_POLICY_ACTION */
} vtss_qce_action_t;

/**
 * \brief QoS Control Entry
 **/
typedef struct
{
    vtss_qce_id_t     id;         /**< Entry ID */
    vtss_qce_key_t    key;        /**< QCE key */      
    vtss_qce_action_t action;     /**< QCE action */      
} vtss_qce_t;

/**
 * \brief Initialize QCE to default values.
 *
 * \param inst [IN]  Target instance reference.
 * \param type [IN]  QCE type.
 * \param qce [OUT]  QCE structure.
 *
 * \return Return code.
 **/
vtss_rc vtss_qce_init(const vtss_inst_t      inst,
                      const vtss_qce_type_t  type,
                      vtss_qce_t             *const qce);

/**
 * \brief Add QCE to QCL.
 *
 * \param inst [IN]    Target instance reference.
 * \param qcl_id [IN]  QCL ID. 
 * \param qce_id [IN]  QCE ID. The QCE will be added before the entry with this ID. 
 *                     VTSS_QCE_ID_LAST is reserved for inserting last.
 * \param qce [IN]     QCE structure.
 *
 * \return Return code.
 **/
vtss_rc vtss_qce_add(const vtss_inst_t    inst,
                     const vtss_qcl_id_t  qcl_id,
                     const vtss_qce_id_t  qce_id,
                     const vtss_qce_t     *const qce);

/**
 * \brief Delete QCE from QCL.
 *
 * \param inst [IN]    Target instance reference.
 * \param qcl_id [IN]  QCL ID.
 * \param qce_id [IN]  QCE ID.
 *
 * \return Return code.
 **/
vtss_rc vtss_qce_del(const vtss_inst_t    inst,
                     const vtss_qcl_id_t  qcl_id,
                     const vtss_qce_id_t  qce_id);

#endif /* VTSS_FEATURE_QCL */

#if defined(VTSS_ARCH_SERVAL)
/**
 * \brief Calibrate egress shaper rates.
 *
 * This function calibrates the egress port and queue shapers on VTSS_ARCH_SERVAL.
 *
 * Calling this function periodically will enhance the accuracy of the egress
 * port and queue shapers.
 *
 * Recommended rate is between 1 to 50 times per second.
 *
 * \param inst [IN] Target instance reference.
 *
 * \return Return code.
 **/
vtss_rc vtss_qos_shaper_calibrate(const vtss_inst_t inst);
#endif /* defined(VTSS_ARCH_SERVAL) */

#endif /* VTSS_FEATURE_QOS */

#ifdef __cplusplus
}
#endif
#endif /* _VTSS_QOS_API_H_ */
