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
 * \file
 * \brief EVC API
 * \details This header file describes EVC functions
 */

#ifndef _VTSS_EVC_API_H_
#define _VTSS_EVC_API_H_

#include <vtss/api/types.h>

#if defined(VTSS_FEATURE_MPLS)
#include "vtss_mpls_api.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if defined(VTSS_FEATURE_EVC)

/** \page evc Ethernet Virtual Connections

    Ethernet Virtual Connections (EVCs) are based on Provider Bridging. It is possible to 
    setup MEF compliant EVCs including Ingress Bandwidth Profiles.
    The normal procedure for configuring EVCs is:

    1) Setup VLAN port types using vtss_vlan_port_conf_set().\n
    2) Setup VLAN membership using vtss_vlan_port_members_set().\n
    3) Add EVCs using vtss_evc_add(), specifying the NNI ports and VID of the outer tag.\n
    4) Add ECEs using vtss_ece_add(), specifying the UNI ports and frames mapping to the EVCs.\n
    5) Setup EVC policers using vtss_evc_policer_conf_set().\n

    \section port Port Configuration

    The following EVC functions are available per port.
    - vtss_evc_port_conf_get() is used to get the EVC port configuration.
    - vtss_evc_port_conf_set() is used to set the EVC port configuration.

    \section policer Policer Configuration
    EVC policers are Ingress Bandwidth Profiles applied to frames classified to an EVC.
    Each EVC policer is identified by a policer ID (::vtss_evc_policer_id_t).
    The following EVC policer functions are available:
    - vtss_evc_policer_conf_get() is used to get the EVC policer configuration.
    - vtss_evc_policer_conf_set() is used to set the EVC policer configuration.
*/

#if defined(VTSS_ARCH_JAGUAR_1) || defined(VTSS_ARCH_SERVAL)
/** \page evc

    The following policer IDs are reserved for special purposes and can not be changed:
    - #VTSS_EVC_POLICER_ID_DISCARD used for an EVC/ECE indicates that all frames are discarded.
    - #VTSS_EVC_POLICER_ID_NONE used for an EVC/ECE indicates that all frames are forwarded.
    - #VTSS_EVC_POLICER_ID_EVC used for an ECE indicates that the policer of the EVC is used.
*/
#endif /* VTSS_ARCH_JAGUAR_1/SERVAL */

/** \page evc

    By default, all EVC policers are disabled.

    \section evcs EVCs
    Each EVC rule is identified by an EVC ID (::vtss_evc_id_t). The following functions are available:
    - vtss_evc_add() is used to add an EVC rule.
    - vtss_evc_del() is used to delete an EVC rule.
    - vtss_evc_get() is used to get an EVC rule.

    The ::vtss_evc_conf_t structure used when adding an EVC rule includes the following:
    - NNI port list.
    - VLAN ID used in outer tag on NNI ports.
    - Internal/classified VLAN ID used for forwarding and learning.

    By default, no EVCs are setup.

    \section eces ECEs
    Each EVC Control Entry (ECE) is identified by an ECE ID used for identification and ordering of ECEs.
    The following functions are available:
    - vtss_ece_add() is used to add an ECE.
    - vtss_ece_del() us used to delete an ECE.

    The ::vtss_ece_t structure used when adding an ECE includes the following fields:
    - ECE ID (::vtss_ece_id_t) used for identifying the rule.
    - ECE key fields (::vtss_ece_key_t), including:
      - UNI port list.
      - Frame type and frame specific fields.
    - ECE action fields (::vtss_ece_action_t), including:
      - EVC ID (::vtss_evc_id_t) to which the ECE is mapping.
      - Direction (::vtss_ece_dir_t) determining if UNI-to-NNI, NNI-to-UNI or bidirectional processing is done.
      - Tag pop count (::vtss_ece_pop_tag_t).
      - Outer tag properties (::vtss_ece_outer_tag_t) determining the PCP and DEI values.
      - ACL policy number (::vtss_acl_policy_no_t) for ACL rule processing.

    By default, no ECEs are setup.
*/
#if defined(VTSS_ARCH_JAGUAR_1) || defined(VTSS_ARCH_SERVAL)
/** \page evc
   
    \section evc_stats EVC Statistics
    EVC statistics are available per EVC ID and UNI/NNI port.
    - vtss_evc_counters_get() is used to get the counters for an EVC and port.
    - vtss_evc_counters_clear() is used to clear the counters for an EVC and port.

    \section ece_stats ECE Statistics
    ECE statistics are available per ECE ID and UNI/NNI port.
    - vtss_ece_counters_get() is used to get the counters for an EVC and port.
    - vtss_ece_counters_clear() is used to clear the counters for an EVC and port.
*/
#endif /* VTSS_ARCH_JAGUAR_1/SERVAL */

#if defined(VTSS_ARCH_CARACAL)
/** \page evc
   
    \section evc_stats EVC Port Statistics
    EVC statistics include green/yellow/red/discarded frames. 
    These counters are available per port and priority.
    - vtss_port_counters_get() is used to get port counters.
    - vtss_port_counters_clear() is used to clear port counters.
*/
#endif /* VTSS_ARCH_CARACAL */

/* - Ethernet Virtual Connections ---------------------------------- */

/** \brief EVC port configuration */
typedef struct {
#if defined(VTSS_ARCH_JAGUAR_1) || defined(VTSS_ARCH_CARACAL)
    BOOL                 dei_colouring; /**< NNI: Enable colouring of DEI for received frames */
#endif /* VTSS_ARCH_JAGUAR_1/CARACAL */
#if defined(VTSS_ARCH_CARACAL)
    BOOL                 inner_tag;     /**< NNI: Enable inner tag (default outer tag) */
#endif /* VTSS_ARCH_CARACAL */
#if defined(VTSS_ARCH_CARACAL) || defined(VTSS_ARCH_SERVAL)
    BOOL                 dmac_dip;      /**< UNI: Enable DMAC/DIP matching (default SMAC/SIP) */
#endif /* VTSS_ARCH_CARACAL/SERVAL */
#if defined(VTSS_ARCH_SERVAL)
    vtss_vcap_key_type_t key_type;      /**< Key type for received frames (default NORMAL) */
#endif /* VTSS_ARCH_SERVAL */
} vtss_evc_port_conf_t;

/**
 * \brief Get EVC port configuration.
 *
 * \param inst [IN]     Target instance reference.
 * \param port_no [IN]  Port number.
 * \param conf [OUT]    EVC port configuration structure.
 *
 * \return Return code.
 **/
vtss_rc vtss_evc_port_conf_get(const vtss_inst_t    inst,
                               const vtss_port_no_t port_no,
                               vtss_evc_port_conf_t *const conf);

/**
 * \brief Set EVC port configuration.
 *
 * \param inst [IN]     Target instance reference.
 * \param port_no [IN]  Port number.
 * \param conf [IN]     EVC port configuration structure.
 *
 * \return Return code.
 **/
vtss_rc vtss_evc_port_conf_set(const vtss_inst_t          inst,
                               const vtss_port_no_t       port_no,
                               const vtss_evc_port_conf_t *const conf);
#endif /* VTSS_FEATURE_EVC */

#if defined(VTSS_FEATURE_QOS_POLICER_DLB)

#if defined(VTSS_ARCH_JAGUAR_1)
#define VTSS_EVC_POLICERS           2048 /**< Maximum number of EVC policers */
#elif defined(VTSS_CHIP_SERVAL)
#define VTSS_EVC_POLICERS           1022 /**< Maximum number of EVC policers */
#elif defined(VTSS_ARCH_LUTON26) || defined(VTSS_CHIP_SERVAL_LITE)
#define VTSS_EVC_POLICERS           256  /**< Maximum number of EVC policers */
#elif defined(VTSS_ARCH_SERVAL)
#define VTSS_EVC_POLICERS           64   /**< Maximum number of EVC policers */
#endif

#if defined(VTSS_ARCH_JAGUAR_1) || defined(VTSS_ARCH_SERVAL)
#define VTSS_EVC_POLICER_ID_DISCARD 4094 /**< EVC/ECE: Policer discards all frames */
#define VTSS_EVC_POLICER_ID_NONE    4095 /**< EVC/ECE: Policer forwards all frames */
#define VTSS_EVC_POLICER_ID_EVC     4096 /**< ECE only: Use EVC policer */
#endif /* VTSS_ARCH_JAGUAR_1/SERVAL */

/** \brief EVC policer configuration */
typedef vtss_dlb_policer_conf_t vtss_evc_policer_conf_t;

/**
 * \brief Get EVC policer configuration.
 *
 * \param inst [IN]        Target instance reference.
 * \param policer_id [IN]  Policer ID.
 * \param conf [OUT]       Policer configuration.
 *
 * \return Return code.
 **/
vtss_rc vtss_evc_policer_conf_get(const vtss_inst_t           inst,
                                  const vtss_evc_policer_id_t policer_id,
                                  vtss_evc_policer_conf_t     *const conf);

/**
 * \brief Set EVC policer configuration.
 *
 * \param inst [IN]        Target instance reference.
 * \param policer_id [IN]  Policer ID.
 * \param conf [IN]        Policer configuration.
 *
 * \return Return code.
 **/
vtss_rc vtss_evc_policer_conf_set(const vtss_inst_t             inst,
                                  const vtss_evc_policer_id_t   policer_id,
                                  const vtss_evc_policer_conf_t *const conf);

#endif /* VTSS_FEATURE_QOS_POLICER_DLB */

#if defined(VTSS_FEATURE_EVC)

#define VTSS_EVC_ID_NONE 0xffff /**< Special EVC ID value */

#if defined(VTSS_ARCH_CARACAL)
/** \brief EVC VID mode */
typedef enum
{
    VTSS_EVC_VID_MODE_NORMAL, /**< Outer VID identifies EVC */
    VTSS_EVC_VID_MODE_TUNNEL  /**< Inner VID identifies EVC */
} vtss_evc_vid_mode_t;

/** \brief EVC inner tag type */
typedef enum 
{
    VTSS_EVC_INNER_TAG_NONE,    /**< No inner tag */
    VTSS_EVC_INNER_TAG_C,       /**< Inner tag is C-tag */
    VTSS_EVC_INNER_TAG_S,       /**< Inner tag is S-tag */
    VTSS_EVC_INNER_TAG_S_CUSTOM /**< Inner tag is S-custom tag */
} vtss_evc_inner_tag_type_t;

/** \brief EVC inner tag */
typedef struct 
{
    vtss_evc_inner_tag_type_t type;             /**< Tag type */
    vtss_evc_vid_mode_t       vid_mode;         /**< VLAN ID mode */
    vtss_vid_t                vid;              /**< VLAN ID  */
    BOOL                      pcp_dei_preserve; /**< Preserved or explicit PCP/DEI values */
    vtss_tagprio_t            pcp;              /**< PCP value */
    vtss_dei_t                dei;              /**< DEI value */
} vtss_evc_inner_tag_t;
#endif /* VTSS_ARCH_CARACAL */

/** \brief PB specific EVC configuration */
typedef struct {
    BOOL                 nni[VTSS_PORT_ARRAY_SIZE];  /**< NNI configuration */
    vtss_vid_t           ivid;                       /**< Internal VID */
    vtss_vid_t           vid;                        /**< NNI VID of outer tag */
#if defined(VTSS_FEATURE_E_TREE)
    vtss_vid_t           leaf_ivid;                  /**< Leaf internal VID */
    vtss_vid_t           leaf_vid;                   /**< Leaf NNI VID of outer tag */
    BOOL                 leaf[VTSS_PORT_ARRAY_SIZE]; /**< UNI leaf */
#endif /* VTSS_FEATURE_E_TREE */
#if defined(VTSS_ARCH_CARACAL)
    vtss_vid_t           uvid;                       /**< UNI VID of outer tag (VTSS_ECE_DIR_NNI_TO_UNI only) */
    vtss_evc_inner_tag_t inner_tag;                  /**< Inner tag (optional) */
#endif /* VTSS_ARCH_CARACAL */
} vtss_evc_pb_conf_t;

#if defined(VTSS_FEATURE_MPLS)

#define VTSS_EVC_MPLS_PW_CNT    VTSS_PORTS          /**< Maximum number of PWs that can be attached to one EVC */

/** \brief MPLS Pseudo Wire configuration, used when attaching a PW to an EVC */
typedef struct {
    BOOL                split_horizon;              /**< TRUE == this PW obeys split horizon; FALSE == it does not */
    vtss_mpls_xc_idx_t  ingress_xc;                 /**< XC for ingress unidirectional pseudo-wire. VTSS_MPLS_IDX_UNDEFINED == unused */
    vtss_mpls_xc_idx_t  egress_xc;                  /**< XC for egress unidirectional pseudo-wire.  VTSS_MPLS_IDX_UNDEFINED == unused */
    u32                 pw_num;                     /**< Pseudo-wire number. Zero == unused entry */
} vtss_evc_mpls_pw_info_t;

/** \brief MPLS specific EVC configuration */
typedef struct {
    vtss_evc_mpls_pw_info_t     pw[VTSS_EVC_MPLS_PW_CNT];   /**< MPLS pseudo-wire configuration. NOTE: All unused entries must be at the end of the array; no "holes" are allowed */
} vtss_evc_mpls_tp_conf_t;
#endif /* VTSS_FEATURE_MPLS */

/** \brief EVC configuration (excluding UNIs) */
typedef struct {
#if defined(VTSS_ARCH_JAGUAR_1) || defined(VTSS_ARCH_SERVAL)
    vtss_evc_policer_id_t   policer_id;                    /**< Policer ID */
#endif /* VTSS_ARCH_JAGUAR_1/SERVAL */
    BOOL                    learning;                      /**< Enable/disable learning */

    struct {
        vtss_evc_pb_conf_t      pb;                        /**< PB specific configuration */
#if defined(VTSS_FEATURE_MPLS)
        vtss_evc_mpls_tp_conf_t mpls_tp;                   /**< MPLS-TP specific configuration */
#endif /* VTSS_FEATURE_MPLS */
    } network; /**< Network specific configuration */
} vtss_evc_conf_t;

/**
 * \brief Add EVC.
 *
 * \param inst [IN]    Target instance reference.
 * \param evc_id [IN]  EVC ID.
 * \param conf [IN]    EVC configuration.
 *
 * \return Return code.
 **/
vtss_rc vtss_evc_add(const vtss_inst_t     inst,
                     const vtss_evc_id_t   evc_id,
                     const vtss_evc_conf_t *const conf);


/**
 * \brief Delete EVC.
 *
 * \param inst [IN]    Target instance reference.
 * \param evc_id [IN]  EVC ID.
 *
 * \return Return code.
 **/
vtss_rc vtss_evc_del(const vtss_inst_t   inst,
                     const vtss_evc_id_t evc_id);


/**
 * \brief Get EVC configuration.
 *
 * \param inst [IN]    Target instance reference.
 * \param evc_id [IN]  EVC ID.
 * \param conf [OUT]   EVC configuration.
 *
 * \return Return code.
 **/
vtss_rc vtss_evc_get(const vtss_inst_t   inst,
                     const vtss_evc_id_t evc_id,
                     vtss_evc_conf_t     *const conf);


#define VTSS_ECE_ID_LAST 0 /**< Special value used to add last in list */

#if defined(VTSS_FEATURE_HQOS)

/**
 * \brief Get the HQoS ID mapping of a port on an EVC.
 *
 * \param inst [IN]      Target instance reference.
 * \param evc_id [IN]    EVC ID.
 * \param port_no [IN]   Port number.
 * \param hqos_id [OUT]  HQoS ID.
 *
 * \return Return code.
 **/
vtss_rc vtss_evc_hqos_map_get(const vtss_inst_t    inst,
                              const vtss_evc_id_t  evc_id,
                              const vtss_port_no_t port_no,
                              vtss_hqos_id_t *const hqos_id);

/**
 * \brief Map a port on an EVC to an HQoS ID.
 *
 * \param inst [IN]     Target instance reference.
 * \param evc_id [IN]   EVC ID.
 * \param port_no [IN]  Port number.
 * \param hqos_id [IN]  HQoS ID.
 *
 * \return Return code.
 **/
vtss_rc vtss_evc_hqos_map_set(const vtss_inst_t    inst,
                              const vtss_evc_id_t  evc_id,
                              const vtss_port_no_t port_no,
                              const vtss_hqos_id_t hqos_id);

#endif /* VTSS_FEATURE_HQOS */

/** \brief ECE frame type */
typedef enum
{
    VTSS_ECE_TYPE_ANY,   /**< Any frame type */
#if defined(VTSS_ARCH_SERVAL)
    VTSS_ECE_TYPE_ETYPE, /**< Ethernet Type */
    VTSS_ECE_TYPE_LLC,   /**< LLC */
    VTSS_ECE_TYPE_SNAP,  /**< SNAP */
#endif /* VTSS_ARCH_SERVAL */
    VTSS_ECE_TYPE_IPV4,  /**< IPv4 */
    VTSS_ECE_TYPE_IPV6   /**< IPv6 */
} vtss_ece_type_t;

/** \brief ECE port type */
typedef enum
{
    VTSS_ECE_PORT_NONE, /**< Port not included */
    VTSS_ECE_PORT_ROOT  /**< Root UNI port */
} vtss_ece_port_t;

/** \brief ECE MAC information */
typedef struct 
{
#if defined(VTSS_ARCH_JAGUAR_1) || defined(VTSS_ARCH_SERVAL)
    vtss_vcap_u48_t dmac;    /**< DMAC */
#endif /* VTSS_ARCH_JAGUAR_1/SERVAL */
#if defined(VTSS_ARCH_CARACAL) || defined(VTSS_ARCH_SERVAL)
    vtss_vcap_bit_t dmac_mc; /**< Multicast DMAC */
    vtss_vcap_bit_t dmac_bc; /**< Broadcast DMAC */
    vtss_vcap_u48_t smac;    /**< SMAC */
#endif /* VTSS_ARCH_CARACAL/SERVAL */
} vtss_ece_mac_t;

/** \brief ECE tag information */
typedef struct 
{
    vtss_vcap_vr_t  vid;      /**< VLAN ID (12 bit) */
    vtss_vcap_u8_t  pcp;      /**< PCP (3 bit) */
    vtss_vcap_bit_t dei;      /**< DEI */
    vtss_vcap_bit_t tagged;   /**< Tagged/untagged frame */
    vtss_vcap_bit_t s_tagged; /**< S-tagged/C-tagged frame */
} vtss_ece_tag_t;

#if defined(VTSS_ARCH_SERVAL)
/** \brief ECE Ethernet Type information */
typedef struct {
    vtss_vcap_u16_t etype; /**< Ethernet Type value */
    vtss_vcap_u32_t data;  /**< MAC data */
} vtss_ece_frame_etype_t;

/** \brief ECE LLC information */
typedef struct {
    vtss_vcap_u48_t data; /**< Data */
} vtss_ece_frame_llc_t;

/** \brief ECE SNAP information */
typedef struct {
    vtss_vcap_u48_t data; /**< Data */
} vtss_ece_frame_snap_t;
#endif /* VTSS_ARCH_SERVAL */

/** \brief ECE IPv4 information */
typedef struct {
    vtss_vcap_vr_t  dscp;     /**< DSCP field (6 bit) */
#if defined(VTSS_ARCH_CARACAL) || defined(VTSS_ARCH_SERVAL)
    vtss_vcap_bit_t fragment; /**< Fragment */
    vtss_vcap_u8_t  proto;    /**< Protocol */
    vtss_vcap_ip_t  sip;      /**< Source IP address */
#if defined(VTSS_ARCH_SERVAL)
    vtss_vcap_ip_t  dip;      /**< Destination IP address */
#endif /* VTSS_ARCH_SERVAL */
    vtss_vcap_vr_t  sport;    /**< UDP/TCP: Source port */
    vtss_vcap_vr_t  dport;    /**< UDP/TCP: Destination port */
#endif /* VTSS_ARCH_CARACAL/SERVAL */
} vtss_ece_frame_ipv4_t;

/** \brief ECE IPv6 information */
typedef struct {
    vtss_vcap_vr_t   dscp;  /**< DSCP field (6 bit) */
#if defined(VTSS_ARCH_CARACAL) || defined(VTSS_ARCH_SERVAL)
    vtss_vcap_u8_t   proto; /**< Protocol */
    vtss_vcap_u128_t sip;   /**< Source IP address (32 LSB) */
#if defined(VTSS_ARCH_SERVAL)
    vtss_vcap_u128_t dip;   /**< Destination IP address (32 LSB) */
#endif /* VTSS_ARCH_SERVAL */
    vtss_vcap_vr_t   sport; /**< UDP/TCP: Source port */
    vtss_vcap_vr_t   dport; /**< UDP/TCP: Destination port */
#endif /* VTSS_ARCH_CARACAL/SERVAL */
} vtss_ece_frame_ipv6_t;

/** \brief ECE key */
typedef struct 
{
    vtss_ece_port_t      port_list[VTSS_PORT_ARRAY_SIZE]; /**< UNI port list */ 
    vtss_ece_mac_t       mac;                             /**< MAC header */
    vtss_ece_tag_t       tag;                             /**< Tag */
#if defined(VTSS_ARCH_JAGUAR_1) || defined(VTSS_ARCH_SERVAL)
    vtss_ece_tag_t       inner_tag;                       /**< Inner tag */
#endif /* VTSS_ARCH_JAGUAR_1/SERVAL */
    vtss_ece_type_t      type;                            /**< Frame type */
#if defined(VTSS_ARCH_SERVAL)
    u8                   lookup;                          /**< Lookup, any non-zero value means second lookup */
#endif /* VTSS_ARCH_SERVAL */

    union 
    {
        /* VTSS_VCE_TYPE_ANY: No specific fields */
#if defined(VTSS_ARCH_SERVAL)
        vtss_ece_frame_etype_t etype; /**< VTSS_ECE_TYPE_ETYPE */
        vtss_ece_frame_llc_t   llc;   /**< VTSS_ECE_TYPE_LLC */
        vtss_ece_frame_snap_t  snap;  /**< VTSS_ECE_TYPE_SNAP */
#endif /* VTSS_ARCH_SERVAL */
        vtss_ece_frame_ipv4_t  ipv4;  /**< VTSS_ECE_TYPE_IPV4 */
        vtss_ece_frame_ipv6_t  ipv6;  /**< VTSS_ECE_TYPE_IPV6 */
    } frame;  /**< Frame type specific data */
} vtss_ece_key_t;

/** \brief ECE outer tag */
typedef struct 
{
    BOOL                enable;           /**< Enable tag (VTSS_ECE_DIR_NNI_TO_UNI only) */
#if defined(VTSS_ARCH_JAGUAR_1) || defined(VTSS_ARCH_SERVAL)
    vtss_vid_t          vid;              /**< VLAN ID (VTSS_ECE_DIR_NNI_TO_UNI only) */
#endif /* VTSS_ARCH_JAGUAR_1/SERVAL */
#if defined(VTSS_ARCH_SERVAL)
    vtss_ece_pcp_mode_t pcp_mode;         /**< PCP mode */
#endif /* VTSS_ARCH_SERVAL */
#if defined(VTSS_ARCH_JAGUAR_1) || defined(VTSS_ARCH_CARACAL)
    BOOL                pcp_dei_preserve; /**< Preserved or explicit PCP/DEI values */
#endif /* VTSS_ARCH_JAGUAR_1/CARACAL */
    vtss_tagprio_t      pcp;              /**< PCP value */
#if defined(VTSS_ARCH_SERVAL)
    vtss_ece_dei_mode_t dei_mode;         /**< DEI mode */
#endif /* VTSS_ARCH_SERVAL */
    vtss_dei_t          dei;              /**< DEI value (ignored if colouring enabled) */
} vtss_ece_outer_tag_t;

#if defined(VTSS_ARCH_JAGUAR_1) || defined(VTSS_ARCH_SERVAL)
/** \brief ECE inner tag */
typedef struct 
{
    vtss_ece_inner_tag_type_t type;             /**< Tag type */
    vtss_vid_t                vid;              /**< VLAN ID  */
#if defined(VTSS_ARCH_SERVAL)
    vtss_ece_pcp_mode_t       pcp_mode;         /**< PCP mode */
#endif /* VTSS_ARCH_SERVAL */
#if defined(VTSS_ARCH_JAGUAR_1)
    BOOL                      pcp_dei_preserve; /**< Preserved or explicit PCP/DEI values */
#endif /* VTSS_ARCH_JAGUAR_1 */
    vtss_tagprio_t            pcp;              /**< PCP value */
#if defined(VTSS_ARCH_SERVAL)
    vtss_ece_dei_mode_t       dei_mode;         /**< DEI mode */
#endif /* VTSS_ARCH_SERVAL */
    vtss_dei_t                dei;              /**< DEI value */
} vtss_ece_inner_tag_t;
#endif /* VTSS_ARCH_JAGUAR_1/SERVAL */

/** \brief ECE action */
typedef struct {
    vtss_ece_dir_t        dir;         /**< Traffic direction */
#if defined(VTSS_ARCH_SERVAL)
    vtss_ece_rule_t       rule;        /**< Rule type */
    vtss_ece_tx_lookup_t  tx_lookup;   /**< Egress lookup type */
#endif /* VTSS_ARCH_SERVAL */
    vtss_ece_pop_tag_t    pop_tag;     /**< Ingress VLAN popping */
    vtss_ece_outer_tag_t  outer_tag;   /**< Egress outer VLAN tag (always present) */
#if defined(VTSS_ARCH_JAGUAR_1) || defined(VTSS_ARCH_SERVAL)
    vtss_ece_inner_tag_t  inner_tag;   /**< Egress inner VLAN tag (optional) */
    vtss_evc_policer_id_t policer_id;  /**< Policer ID */
#endif /* VTSS_ARCH_JAGUAR_1/SERVAL */
    vtss_evc_id_t         evc_id;      /**< EVC ID */
    vtss_acl_policy_no_t  policy_no;   /**< ACL policy number */
#if defined(VTSS_ARCH_CARACAL) || defined(VTSS_ARCH_SERVAL)
    BOOL                  prio_enable; /**< Enable priority classification */
    vtss_prio_t           prio;        /**< Priority (QoS class) */
#endif /* VTSS_ARCH_CARACAL/SERVAL */
#if defined(VTSS_ARCH_SERVAL)
    BOOL                  dp_enable;   /**< Enable DP classification */
    vtss_dp_level_t       dp;          /**< Drop precedence */
#endif /* VTSS_ARCH_SERVAL */
} vtss_ece_action_t;

/** \brief EVC Control Entry */
typedef struct {
    vtss_ece_id_t     id;     /**< Entry ID */
    vtss_ece_key_t    key;    /**< ECE key */
    vtss_ece_action_t action; /**< ECE action */
} vtss_ece_t;

/**
 * \brief Initialize ECE to default values.
 *
 * \param inst [IN]  Target instance reference.
 * \param type [IN]  ECE type.
 * \param ece [OUT]  ECE structure.
 *
 * \return Return code.
 **/
vtss_rc vtss_ece_init(const vtss_inst_t     inst,
                      const vtss_ece_type_t type,
                      vtss_ece_t            *const ece);

/**
 * \brief Add/modify ECE.
 *
 * \param inst [IN]    Target instance reference.
 * \param ece_id [IN]  ECE ID. The ECE will be added before the entry with this ID. 
 *                     VTSS_ECE_ID_LAST is reserved for inserting last.
 * \param ece [IN]     ECE structure.
 *
 * \return Return code.
 **/
vtss_rc vtss_ece_add(const vtss_inst_t   inst,
                     const vtss_ece_id_t ece_id,
                     const vtss_ece_t    *const ece);

/**
 * \brief Delete ECE.
 *
 * \param inst [IN]    Target instance reference.
 * \param ece_id [IN]  ECE ID.
 *
 * \return Return code.
 **/
vtss_rc vtss_ece_del(const vtss_inst_t   inst,
                     const vtss_ece_id_t ece_id);

#if defined(VTSS_FEATURE_OAM)
/** \brief EVC OAM port configuration */
typedef struct {
    vtss_oam_voe_idx_t voe_idx; /**< VOE index or VTSS_OAM_VOE_IDX_NONE */
} vtss_evc_oam_port_conf_t;

/**
 * \brief Get OAM port configuration for (EVC, port)
 *
 * \param inst [IN]    Target instance reference.
 * \param evc_id [IN]  EVC ID.
 * \param port_no [IN] Port number.
 * \param conf [OUT]   OAM port configuration.
 *
 * \return Return code.
 **/
vtss_rc vtss_evc_oam_port_conf_get(const vtss_inst_t        inst,
                                   const vtss_evc_id_t      evc_id,
                                   const vtss_port_no_t     port_no,
                                   vtss_evc_oam_port_conf_t *const conf);

/**
 * \brief Set OAM port configuration for (EVC, port)
 *
 * \param inst [IN]    Target instance reference.
 * \param evc_id [IN]  EVC ID.
 * \param port_no [IN] Port number.
 * \param conf [IN]    OAM port configuration.
 *
 * \return Return code.
 **/
vtss_rc vtss_evc_oam_port_conf_set(const vtss_inst_t              inst,
                                   const vtss_evc_id_t            evc_id,
                                   const vtss_port_no_t           port_no,
                                   const vtss_evc_oam_port_conf_t *const conf);
#endif /* VTSS_FEATURE_OAM */

#if defined(VTSS_ARCH_CARACAL) || defined(VTSS_ARCH_SERVAL)
/** \brief MEP Control Entry (MCE) ID */
typedef u32 vtss_mce_id_t;

#define VTSS_MCE_ID_LAST 0 /**< Special value used to add last in list */

#if defined(VTSS_ARCH_SERVAL)
/** \brief MCE tag information */
typedef struct 
{
    vtss_vcap_bit_t tagged;   /**< Tagged/untagged frame */
    vtss_vcap_bit_t s_tagged; /**< S-tagged/C-tagged frame */
    vtss_vcap_vid_t vid;      /**< VLAN ID (12 bit) */
    vtss_vcap_u8_t  pcp;      /**< PCP (3 bit) */
    vtss_vcap_bit_t dei;      /**< DEI */
} vtss_mce_tag_t;
#endif /* VTSS_ARCH_SERVAL */

/** \brief MCE key */
typedef struct
{
    BOOL            port_list[VTSS_PORT_ARRAY_SIZE]; /**< Ingress port list */
#if defined(VTSS_ARCH_SERVAL)
    BOOL            port_cpu;  /**< CPU is the only ingress port */
    vtss_mce_tag_t  tag;       /**< Outer tag */
    vtss_mce_tag_t  inner_tag; /**< Inner tag */
    vtss_vcap_u8_t  mel;       /**< MEG level (7 bit) */
    vtss_vcap_bit_t injected;  /**< CPU injected */
    u8              lookup;    /**< Lookup, any non-zero value means second ingress lookup */
    vtss_vcap_u48_t dmac;      /**< DMAC */
    vtss_vcap_bit_t dmac_mc;   /**< Multicast DMAC */
    BOOL            service_detect; /**< Detection of OAM or Service frames */
#endif /* VTSS_ARCH_SERVAL */
#if defined(VTSS_ARCH_CARACAL)
    vtss_vcap_vid_t vid;       /**< Classified VID */
    vtss_vcap_u16_t data;      /**< Two first data bytes after Ethertype */
#endif /* VTSS_ARCH_CARACAL */
} vtss_mce_key_t;

#if defined(VTSS_ARCH_SERVAL)
/** \brief MCE PCP mode */
typedef enum
{
    VTSS_MCE_PCP_MODE_FIXED, /* Fixed PCP */
    VTSS_MCE_PCP_MODE_MAPPED /* PCP based on mapped (QOS, DP) */
} vtss_mce_pcp_mode_t;

/** \brief MCE PCP mode */
typedef enum
{
    VTSS_MCE_DEI_MODE_FIXED, /* Fixed DEI */
    VTSS_MCE_DEI_MODE_DP     /* DP-based DEI */
} vtss_mce_dei_mode_t;

/** \brief ECE outer tag */
typedef struct 
{
    BOOL                enable;   /**< Enable tag */
    vtss_vid_t          vid;      /**< VLAN ID */
    vtss_mce_pcp_mode_t pcp_mode; /**< PCP mode */
    vtss_tagprio_t      pcp;      /**< PCP value */
    vtss_mce_dei_mode_t dei_mode; /**< DEI mode */
    vtss_dei_t          dei;      /**< DEI value */
} vtss_mce_outer_tag_t;

/** \brief MCE egress lookup types */
typedef enum {
    VTSS_MCE_TX_LOOKUP_VID,     /**< VID lookup */
    VTSS_MCE_TX_LOOKUP_ISDX,    /**< ISDX lookup */
    VTSS_MCE_TX_LOOKUP_ISDX_PCP /**< (ISDX, PCP) lookup */
} vtss_mce_tx_lookup_t;

/** \brief MCE OAM detection signalled to VOE */
typedef enum {
    VTSS_MCE_OAM_DETECT_NONE,          /**< No OAM detection */
    VTSS_MCE_OAM_DETECT_UNTAGGED,      /**< Untagged OAM detection */
    VTSS_MCE_OAM_DETECT_SINGLE_TAGGED, /**< Single tagged OAM detection */
    VTSS_MCE_OAM_DETECT_DOUBLE_TAGGED  /**< Double tagged OAM detection */
} vtss_mce_oam_detect_t;

/** \brief MCE rule types */
typedef enum {
    VTSS_MCE_RULE_BOTH, /**< Ingress and egress rules */
    VTSS_MCE_RULE_RX,   /**< Ingress rules */
} vtss_mce_rule_t;

#endif /* VTSS_ARCH_SERVAL */

#if defined(VTSS_ARCH_SERVAL)
#define VTSS_MCE_ISDX_NONE 0xFFFFFFFF /**< Allocate no ISDX */
#define VTSS_MCE_ISDX_NEW  0xFFFFFFFE /**< Allocate new ISDX */
#define VTSS_ISDX_CPU_TX   1023       /**< ISDX used for CPU transmissions */
#endif /* VTSS_ARCH_SERVAL */
#define VTSS_MCE_POP_NONE 0xFF /**< Special value used to indicate pop_cnt not enabled */

/** \brief MCE action */
typedef struct
{
#if defined(VTSS_ARCH_SERVAL)
    BOOL                  port_list[VTSS_PORT_ARRAY_SIZE]; /**< Egress port list */
    BOOL                  evc_counting;/**< OAM frames hitting this rule must be counting as EVC frames */
    BOOL                  evc_etree;   /**< OAM frames hitting this rule must be forwarded as in an E-TREE. ISDX mask must be used restricting forwarding and no ES0 created */
    vtss_evc_id_t         evc_id;      /**< EVC ID */
    vtss_oam_voe_idx_t    voe_idx;     /**< VOE index or VTSS_OAM_VOE_IDX_NONE */
    vtss_mce_outer_tag_t  outer_tag;   /**< Egress outer tag */
    vtss_isdx_t           isdx;        /**< ISDX or VTSS_MCE_ISDX_NONE or VTSS_MCE_ISDX_NEW */
    vtss_mce_rule_t       rule;        /**< Rule type */
    vtss_mce_tx_lookup_t  tx_lookup;   /**< Egress lookup type */
    vtss_mce_oam_detect_t oam_detect;  /**< OAM detection */
#endif /* VTSS_ARCH_SERVAL */
#if defined(VTSS_ARCH_JAGUAR_1) || defined(VTSS_ARCH_SERVAL)
    vtss_evc_policer_id_t policer_id;  /**< Policer ID */
#endif /* VTSS_ARCH_JAGUAR_1/SERVAL */
    vtss_acl_policy_no_t  policy_no;   /**< ACL policy number */
    BOOL                  prio_enable; /**< Enable priority control */
    vtss_prio_t           prio;        /**< Selected priority */
    vtss_vid_t            vid;         /**< Replace VID */
    u8                    pop_cnt;     /**< Pop count */
} vtss_mce_action_t;

/** \brief MEP Control Entry */
typedef struct
{
    BOOL              tt_loop;/**< This is a TT_LOOP entry. The TT_LOOP VCAP user is used when creating entry */
    vtss_mce_id_t     id;     /**< Entry ID */
    vtss_mce_key_t    key;    /**< MCE key */
    vtss_mce_action_t action; /**< MCE action */
} vtss_mce_t;

/**
 * \brief Initialize MCE to default values.
 *
 * \param inst [IN]  Target instance reference.
 * \param mce [OUT]  MCE structure.
 *
 * \return Return code.
 **/
vtss_rc vtss_mce_init(const vtss_inst_t inst,
                      vtss_mce_t        *const mce);

/**
 * \brief Add/modify MCE.
 *
 * \param inst [IN]    Target instance reference.
 * \param mce_id [IN]  MCE ID. The MCE will be added before the entry with this ID. 
 *                     VTSS_MCE_ID_LAST is reserved for inserting last.
 * \param mce [IN]     MCE structure.
 *
 * \return Return code.
 **/
vtss_rc vtss_mce_add(const vtss_inst_t   inst,
                     const vtss_mce_id_t mce_id,
                     const vtss_mce_t    *const mce);

/**
 * \brief Delete MCE.
 *
 * \param inst [IN]    Target instance reference.
 * \param mce_id [IN]  MCE ID.
 *
 * \return Return code.
 **/
vtss_rc vtss_mce_del(const vtss_inst_t   inst,
                     const vtss_mce_id_t mce_id);

#if defined(VTSS_ARCH_SERVAL)
/** \brief MEP Control Entry port information */
typedef struct
{
    vtss_isdx_t isdx; /**< Ingress service index */
} vtss_mce_port_info_t;

/**
 * \brief Get MCE info.
 *
 * \param inst [IN]    Target instance reference.
 * \param mce_id [IN]  MCE ID.
 * \param port_no [IN] Port number.
 * \param info [OUT]   MCE information.
 *
 * \return Return code.
 **/
vtss_rc vtss_mce_port_info_get(const vtss_inst_t    inst,
                               const vtss_mce_id_t  mce_id,
                               const vtss_port_no_t port_no,
                               vtss_mce_port_info_t *const info);
#endif /* VTSS_ARCH_SERVAL */
#endif /* VTSS_ARCH_CARACAL/SERVAL */

#if defined(VTSS_ARCH_JAGUAR_1) || defined(VTSS_ARCH_SERVAL)
/**
 * \brief Get EVC counters for a port.
 *
 * \param inst [IN]       Target instance reference.
 * \param evc_id [IN]     EVC ID.
 * \param port_no [IN]    Port number.
 * \param counters [OUT]  EVC counters.
 *
 * \return Return code.
 **/
vtss_rc vtss_evc_counters_get(const vtss_inst_t    inst,
                              const vtss_evc_id_t  evc_id,
                              const vtss_port_no_t port_no,
                              vtss_evc_counters_t  *const counters);

/**
 * \brief Clear EVC counters for a port.
 *
 * \param inst [IN]     Target instance reference.
 * \param evc_id [IN]   EVC ID.
 * \param port_no [IN]  Port number.
 *
 * \return Return code.
 **/
vtss_rc vtss_evc_counters_clear(const vtss_inst_t    inst,
                                const vtss_evc_id_t  evc_id,
                                const vtss_port_no_t port_no);

/**
 * \brief Get ECE counters for a port.
 *
 * \param inst [IN]       Target instance reference.
 * \param ece_id [IN]     ECE ID.
 * \param port_no [IN]    Port number.
 * \param counters [OUT]  ECE counters.
 *
 * \return Return code.
 **/
vtss_rc vtss_ece_counters_get(const vtss_inst_t    inst,
                              const vtss_ece_id_t  ece_id,
                              const vtss_port_no_t port_no,
                              vtss_evc_counters_t  *const counters);

/**
 * \brief Clear ECE counters for a port.
 *
 * \param inst [IN]     Target instance reference.
 * \param ece_id [IN]   ECE ID.
 * \param port_no [IN]  Port number.
 *
 * \return Return code.
 **/
vtss_rc vtss_ece_counters_clear(const vtss_inst_t    inst,
                                const vtss_ece_id_t  ece_id,
                                const vtss_port_no_t port_no);

#if defined(VTSS_ARCH_SERVAL)
/**
 * \brief Get MCE counters for a port.
 *
 * \param inst [IN]       Target instance reference.
 * \param mce_id [IN]     MCE ID.
 * \param port_no [IN]    Port number.
 * \param counters [OUT]  MCE counters.
 *
 * \return Return code.
 **/
vtss_rc vtss_mce_counters_get(const vtss_inst_t    inst,
                              const vtss_mce_id_t  mce_id,
                              const vtss_port_no_t port_no,
                              vtss_evc_counters_t  *const counters);

/**
 * \brief Clear MCE counters for a port.
 *
 * \param inst [IN]     Target instance reference.
 * \param mce_id [IN]   MCE ID.
 * \param port_no [IN]  Port number.
 *
 * \return Return code.
 **/
vtss_rc vtss_mce_counters_clear(const vtss_inst_t    inst,
                                const vtss_mce_id_t  mce_id,
                                const vtss_port_no_t port_no);
#endif /* VTSS_ARCH_SERVAL */

#endif /* VTSS_ARCH_JAGUAR_1/SERVAL */

#endif /* VTSS_FEATURE_EVC */

#ifdef __cplusplus
}
#endif
#endif /* _VTSS_EVC_API_H_ */
