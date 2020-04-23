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
 * \brief Security API
 * \details This header file describes security functions
 */

#ifndef _VTSS_SECURITY_API_H_
#define _VTSS_SECURITY_API_H_

#include <vtss/api/types.h>

#ifdef __cplusplus
extern "C" {
#endif
#if defined(VTSS_FEATURE_LAYER2)

/** \page security Security

    The Security functions are used to control Port Authentication and Access Control List.

    \section port_auth Port Authentication (802.1X)

    The the 802.1X forwarding state for a port can be setup.
    - vtss_auth_port_state_get() is used to get the authentication state for a port.
    - vtss_auth_port_state_set() is used to set the authentication state for a port.

    By default, all ports are authenticated in both directions.
*/

/* - Port Based Network Access Control, 802.1X --------------------- */

/** \brief Authentication state */
typedef enum
{
    VTSS_AUTH_STATE_NONE,   /**< Not authenticated */
    VTSS_AUTH_STATE_EGRESS, /**< Authenticated in egress direction */
    VTSS_AUTH_STATE_BOTH    /**< Authenticated in both directions */
} vtss_auth_state_t;

/**
 * \brief Get 802.1X Authentication state for a port.
 *
 * \param inst [IN]     Target instance reference.
 * \param port_no [IN]  Port number.
 * \param state [OUT]   Authentication state.
 *
 * \return Return code.
 **/
vtss_rc vtss_auth_port_state_get(const vtss_inst_t     inst,
                                 const vtss_port_no_t  port_no,
                                 vtss_auth_state_t     *const state);


/**
 * \brief Set 802.1X Authentication state for a port.
 *
 * \param inst [IN]     Target instance reference.
 * \param port_no [IN]  Port number.
 * \param state [IN]    Authentication state.
 *
 * \return Return code.
 **/
vtss_rc vtss_auth_port_state_set(const vtss_inst_t        inst,
                                 const vtss_port_no_t     port_no,
                                 const vtss_auth_state_t  state);
                                          
#endif /* VTSS_FEATURE_LAYER2 */

#if defined(VTSS_FEATURE_ACL)

/** \page security

    \section acl Access Control List

    Advanced frame processing can be setup using the Access Control List (ACL) of the switch.
    Each rule in the list is called an Access Control Entry (ACE). For each frame received on an ingress port,
    the ACL is searched until an ACE matching the ingress port and frame properties is found.
    The action of the first matching ACE determines the forwarding of the frame. If no matching ACE is found,
    the default action of the ingress port is used.

    By default, the ACL is empty and the default port actions allow forwarding of frames.

    \subsection ace Access Control Entry

    Each ACE is idenfied by an ACE ID used for identification and ordering of ACEs.
    The following ACE functions are available:
    - vtss_ace_init() is used to initialize an ACE to default values.
    - vtss_ace_add() is used to add or modify an ACE.
    - vtss_ace_del() is used to delete an ACE.
    - vtss_ace_counter_get() is used to get the hit counter of an ACE.
    - vtss_ace_counter_clear() is used to clear the hit counter of an ACE.

    The ::vtss_ace_t structure used when adding an ACE can be divided into three parts:
    - ACE ID (::vtss_ace_id_t) used for identifying the rule.
    - ACE action field (::vtss_acl_action_t) used to describe the forwarding of matching frames, including:
      - Filtering (e.g. permit/deny frames)
      - Policing (rate limiting)
      - CPU copy (e.g. for protocol processing)
    - ACE key fields used to match against incoming frames, including:
      - Ingress ports
      - ACL policy number
      - Frame type and frame type specific fields

    \subsection acl_port Port Configuration

    The following ACL functions are available per ingress port:
    - vtss_acl_port_conf_get() is used to get the ACL port configuration.
    - vtss_acl_port_conf_set() is used to set the ACL port configuration.
    - vtss_acl_port_counter_get() is used to get the default hit counter.
    - vtss_acl_port_counter_clear() is used to clear the default hit counter.

    The port configuration includes the default ACL action and the ACL policy number (::vtss_acl_policy_no_t)
    which can be used to form groups of ports matching the same ACEs.
    ACEs matching an ACL policy number can be added to match frames from ports with the same policy number.
    The value #VTSS_ACL_POLICY_NO_NONE is used to disable ACL processing on a port.

    \subsection acl_policer Policer Configuration

    Each policer is identified by an ACL policer ID (::vtss_acl_policer_no_t) which can be used when
    mapping an ACL action to a policer. The following functions are available per ACL policer:
    - vtss_acl_policer_conf_get() is used to get the ACL policer configuration.
    - vtss_acl_policer_conf_set() is used to set the ACL policer configuration.
*/


/* - Access Control Lists ------------------------------------------ */

/** \brief ACL policer configuration */
typedef struct {
#if defined(VTSS_FEATURE_ACL_V2)
    BOOL               bit_rate_enable; /**< Use bit rate policing instead of packet rate */
    vtss_bitrate_t     bit_rate;        /**< Bit rate */
#endif /* VTSS_FEATURE_ACL_V2 */
    vtss_packet_rate_t rate;            /**< Packet rate */
} vtss_acl_policer_conf_t;


/**
 * \brief Get ACL policer configuration.
 *
 * \param inst [IN]        Target instance reference.
 * \param policer_no [IN]  ACL policer number.
 * \param conf [OUT]       ACL policer configuration.
 *
 * \return Return code.
 **/
vtss_rc vtss_acl_policer_conf_get(const vtss_inst_t            inst,
                                  const vtss_acl_policer_no_t  policer_no,
                                  vtss_acl_policer_conf_t      *const conf);


/**
 * \brief Set ACL policer configuration.
 *
 * \param inst [IN]        Target instance reference.
 * \param policer_no [IN]  ACL policer number.
 * \param conf [IN]        ACL policer configuration.
 *
 * \return Return code.
 **/
vtss_rc vtss_acl_policer_conf_set(const vtss_inst_t              inst,
                                  const vtss_acl_policer_no_t    policer_no,
                                  const vtss_acl_policer_conf_t  *const conf);


#if defined(VTSS_FEATURE_ACL_V2)
/** \brief ACL port action */
typedef enum
{
    VTSS_ACL_PORT_ACTION_NONE,   /**< No action from port list */
    VTSS_ACL_PORT_ACTION_FILTER, /**< Port list filter is used */
    VTSS_ACL_PORT_ACTION_REDIR   /**< Port list redirect is used */
} vtss_acl_port_action_t;

/** \brief ACL PTP action */
typedef enum
{
    VTSS_ACL_PTP_ACTION_NONE,                 /**< No PTP action */
    VTSS_ACL_PTP_ACTION_ONE_STEP,             /**< PTP one-step time-stamping */
#if defined(VTSS_ARCH_SERVAL) || defined(VTSS_ARCH_JAGUAR_2)
    VTSS_ACL_PTP_ACTION_ONE_STEP_ADD_DELAY,   /**< PTP one-step time-stamping, Serval: add delay, Jr2: Add EDLY */
    VTSS_ACL_PTP_ACTION_ONE_STEP_SUB_DELAY_1, /**< PTP one-step time-stamping, Serval: subtract delay 1, Jr2: Add IDLY1 */
    VTSS_ACL_PTP_ACTION_ONE_STEP_SUB_DELAY_2, /**< PTP one-step time-stamping, Serval: subtract delay 2, Jr2: Add IDLY2 */
#endif /* VTSS_ARCH_SERVAL/JAGUAR_2 */
#if defined(VTSS_ARCH_LUTON26)
    VTSS_ACL_PTP_ACTION_ONE_AND_TWO_STEP,     /**< PTP one-step and two-step time-stamping */
#endif /* VTSS_ARCH_LUTON26 */
    VTSS_ACL_PTP_ACTION_TWO_STEP              /**< PTP two-step time-stamping */
} vtss_acl_ptp_action_t;
#endif /* VTSS_FEATURE_ACL_V2 */

#if defined(VTSS_ARCH_JAGUAR_2)
/** \brief ACL PTP response action */
typedef enum
{
    VTSS_ACL_PTP_RSP_NONE,               /**< No response action */
    VTSS_ACL_PTP_RSP_DLY_REQ_RSP_TS_UPD, /**< Auto response to Delay_Req, includes receiveTimestamp update */
    VTSS_ACL_PTP_RSP_DLY_REQ_RSP_NO_TS   /**< Auto response to Delay_Req, exludes receiveTimestamp update */
} vtss_acl_ptp_rsp_t;

/** \brief ACL PTP action configuration */
typedef struct {
    vtss_acl_ptp_rsp_t response;             /**< PTP Delay_Req/Response action */
    i8                 log_message_interval; /**< PTP logMessageInterval [-8,7] returned in the Delay_Resp message */
    BOOL               copy_smac_to_dmac;    /**< PTP DMAC operation */
    BOOL               set_smac_to_port_mac; /**< PTP SMAC operation */
    u8                 dom_sel;              /**< PTP domain selector. PTP_DOM_SEL indexes the PTP configuration */
} vtss_acl_ptp_action_conf_t;
#endif /* VTSS_ARCH_JAGUAR_2 */

/** \brief ACL Action */
typedef struct
{
    BOOL                       cpu;            /**< Forward to CPU */
    BOOL                       cpu_once;       /**< Only first frame forwarded to CPU */
    vtss_packet_rx_queue_t     cpu_queue;      /**< CPU queue */
    BOOL                       police;         /**< Enable policer */
    vtss_acl_policer_no_t      policer_no;     /**< Policer number */
#if defined(VTSS_ARCH_LUTON26) || defined(VTSS_ARCH_SERVAL)
#if defined(VTSS_FEATURE_QOS_POLICER_DLB)
    BOOL                       evc_police;     /**< Enable EVC policer */
    vtss_evc_policer_id_t      evc_policer_id; /**< EVC policer ID */
#endif /* VTSS_FEATURE_QOS_POLICER_DLB */
#endif /* VTSS_ARCH_LUTON26/SERVAL */
    BOOL                       learn;          /**< Allow learning */
#if defined(VTSS_FEATURE_ACL_V1)
    BOOL                       forward;        /**< Allow forwarding */
    BOOL                       port_forward;   /**< Forward to specific port */
    vtss_port_no_t             port_no;        /**< Specified port */
#endif /* VTSS_FEATURE_ACL_V1 */
#if defined(VTSS_FEATURE_ACL_V2)
    vtss_acl_port_action_t     port_action;    /**< Port action */
    BOOL                       port_list[VTSS_PORT_ARRAY_SIZE]; /**< Egress port list */
    BOOL                       mirror;         /**< Enable mirroring */
    vtss_acl_ptp_action_t      ptp_action;     /**< PTP action */
#endif /* VTSS_FEATURE_ACL_V2 */
#if defined(VTSS_ARCH_JAGUAR_2)
    vtss_acl_ptp_action_conf_t ptp;            /**< PTP configuration */
#endif /* VTSS_ARCH_JAGUAR_2 */
#if defined(VTSS_ARCH_JAGUAR_1)
    BOOL                       irq_trigger;    /**< Trigger interrupt against CPU. */
#endif
#if defined(VTSS_ARCH_SERVAL)
    BOOL                       lm_cnt_disable; /**< Disable OAM LM Tx counting */
    BOOL                       mac_swap;       /**< Swap SMAC and DMAC */
#endif /* VTSS_ARCH_SERVAL */
} vtss_acl_action_t;

/** \brief ACL port configuration */
typedef struct
{
    vtss_acl_policy_no_t policy_no; /**< Policy number */
    vtss_acl_action_t    action;    /**< Action */
} vtss_acl_port_conf_t;

/**
 * \brief Get ACL configuration for port.
 *
 * \param inst [IN]     Target instance reference.
 * \param port_no [IN]  Port number.
 * \param conf [OUT]    Port configuration.
 *
 * \return Return code.
 **/
vtss_rc vtss_acl_port_conf_get(const vtss_inst_t     inst,
                               const vtss_port_no_t  port_no,
                               vtss_acl_port_conf_t  *const conf);

/**
 * \brief Set ACL configuration for port.
 *
 * \param inst [IN]     Target instance reference.
 * \param port_no [IN]  Port number.
 * \param conf [IN]     Port configuration.
 *
 * \return Return code.
 **/
vtss_rc vtss_acl_port_conf_set(const vtss_inst_t           inst,
                               const vtss_port_no_t        port_no,
                               const vtss_acl_port_conf_t  *const conf);


/** \brief ACL port counter */
typedef u32 vtss_acl_port_counter_t; 

/**
 * \brief Get default action counter for port.
 *
 * \param inst [IN]      Target instance reference.
 * \param port_no [IN]   Port number.
 * \param counter [OUT]  Default action counter for port.
 *
 * \return Return code.
 **/
vtss_rc vtss_acl_port_counter_get(const vtss_inst_t        inst,
                                  const vtss_port_no_t     port_no,
                                  vtss_acl_port_counter_t  *const counter);

/**
 * \brief Clear default action counter for port.
 *
 * \param inst [IN]     Target instance reference.
 * \param port_no [IN]  Port number.
 *
 * \return Return code.
 **/
vtss_rc vtss_acl_port_counter_clear(const vtss_inst_t     inst,
                                    const vtss_port_no_t  port_no);


/** \brief ACE frame type */
typedef enum
{
    VTSS_ACE_TYPE_ANY,     /**< Any frame type */
    VTSS_ACE_TYPE_ETYPE,   /**< Ethernet Type */
    VTSS_ACE_TYPE_LLC,     /**< LLC */
    VTSS_ACE_TYPE_SNAP,    /**< SNAP */
    VTSS_ACE_TYPE_ARP,     /**< ARP/RARP */
    VTSS_ACE_TYPE_IPV4,    /**< IPv4 */
    VTSS_ACE_TYPE_IPV6     /**< IPv6 */
} vtss_ace_type_t;

/** \brief ACE ID type */
typedef u32 vtss_ace_id_t;

#define VTSS_ACE_ID_LAST 0 /**< Special value used to add last in list */

/** \brief ACE 1 bit */
typedef enum
{
    VTSS_ACE_BIT_ANY, /**< Value 0 or 1 */
    VTSS_ACE_BIT_0,   /**< Value 0 */
    VTSS_ACE_BIT_1    /**< Value 1 */
} vtss_ace_bit_t;

/** \brief ACE 8 bit value and mask */
typedef vtss_vcap_u8_t vtss_ace_u8_t;

/** \brief ACE 16 bit value and mask */
typedef vtss_vcap_u16_t vtss_ace_u16_t;

/** \brief ACE 32 bit value and mask */
typedef vtss_vcap_u32_t vtss_ace_u32_t;

/** \brief ACE 40 bit value and mask */
typedef vtss_vcap_u40_t vtss_ace_u40_t;

/** \brief ACE 48 bit value and mask */
typedef vtss_vcap_u48_t vtss_ace_u48_t;

/** \brief ACE 128 bit value and mask */
typedef vtss_vcap_u128_t vtss_ace_u128_t;

/** \brief ACE VLAN ID value and mask */
typedef vtss_vcap_vid_t vtss_ace_vid_t;

/** \brief ACE IP address value and mask */
typedef vtss_vcap_ip_t vtss_ace_ip_t; 

/** \brief ACE UDP/TCP port range */
typedef vtss_vcap_udp_tcp_t vtss_ace_udp_tcp_t;

#if defined(VTSS_FEATURE_ACL_V2)
/** \brief PTP header filtering */
typedef struct
{
    BOOL           enable; /**< Enable PTP header filtering */
    vtss_ace_u32_t header; /**< PTP header byte 0, 1, 4 and 6 */  
} vtss_ace_ptp_t;

/** \brief SIP/SMAC filtering */
typedef struct
{
    BOOL       enable; /**< Enable SIP/SMAC filtering */
    vtss_ip_t  sip;    /**< SIP */  
    vtss_mac_t smac;   /**< SMAC */
} vtss_ace_sip_smac_t;
#endif /* VTSS_FEATURE_ACL_V2 */

#if defined(VTSS_FEATURE_ACL_V1)
#define VTSS_PORT_NO_ANY VTSS_PORT_NO_NONE /**< Any port number */
#endif /* VTSS_FEATURE_ACL_V1 */

/** \brief ACE VLAN information */
typedef struct
{
    vtss_ace_vid_t   vid;      /**< VLAN ID (12 bit) */
    vtss_ace_u8_t    usr_prio; /**< User priority/PCP (3 bit) */
    vtss_ace_bit_t   cfi;      /**< CFI/DEI */
#if defined(VTSS_FEATURE_ACL_V2)
    vtss_ace_bit_t   tagged;   /**< Tagged/untagged frame */
#endif /* VTSS_FEATURE_ACL_V2 */
} vtss_ace_vlan_t;

/** \brief Frame data for VTSS_ACE_TYPE_ETYPE */
typedef struct
{
    vtss_ace_u48_t dmac;  /**< DMAC */
    vtss_ace_u48_t smac;  /**< SMAC */
    vtss_ace_u16_t etype; /**< Ethernet Type value */
    vtss_ace_u16_t data;  /**< MAC data */ 
#if defined(VTSS_FEATURE_ACL_V2)
    vtss_ace_ptp_t ptp;   /**< PTP header filtering (overrides smac byte 2,4 and data fields) */
#endif /* VTSS_FEATURE_ACL_V2 */
} vtss_ace_frame_etype_t;

/** \brief Frame data for VTSS_ACE_TYPE_LLC */
typedef struct                                 
{                                      
    vtss_ace_u48_t dmac; /**< DMAC */
    vtss_ace_u48_t smac; /**< SMAC */
    vtss_ace_u32_t llc;  /**< LLC header: DSAP at byte 0, SSAP at byte 1, Control at byte 2 */
} vtss_ace_frame_llc_t;

/** \brief Frame data for VTSS_ACE_TYPE_SNAP */
typedef struct                                 
{                                      
    vtss_ace_u48_t dmac; /**< DMAC */
    vtss_ace_u48_t smac; /**< SMAC */
    vtss_ace_u40_t snap; /**< SNAP header: Organization Code at byte 0, Type at byte 3 */
} vtss_ace_frame_snap_t;

/** \brief Frame data for VTSS_ACE_TYPE_ARP */
typedef struct                                 
{                                      
    vtss_ace_u48_t smac;       /**< SMAC */
    vtss_ace_bit_t arp;        /**< Opcode ARP/RARP */
    vtss_ace_bit_t req;        /**< Opcode request/reply */
    vtss_ace_bit_t unknown;    /**< Opcode unknown */
    vtss_ace_bit_t smac_match; /**< Sender MAC matches SMAC */
    vtss_ace_bit_t dmac_match; /**< Target MAC matches DMAC */
    vtss_ace_bit_t length;     /**< Protocol addr. length 4, hardware length 6 */
    vtss_ace_bit_t ip;         /**< Protocol address type IP */
    vtss_ace_bit_t ethernet;   /**< Hardware address type Ethernet */
    vtss_ace_ip_t  sip;        /**< Sender IP address */
    vtss_ace_ip_t  dip;        /**< Target IP address */
} vtss_ace_frame_arp_t;
        
/** \brief Frame data for VTSS_ACE_TYPE_IPV4 */
typedef struct
{
    vtss_ace_bit_t      ttl;            /**< TTL zero */
    vtss_ace_bit_t      fragment;       /**< Fragment */
    vtss_ace_bit_t      options;        /**< Header options */
    vtss_ace_u8_t       ds;             /**< DS field */
    vtss_ace_u8_t       proto;          /**< Protocol */
    vtss_ace_ip_t       sip;            /**< Source IP address */
    vtss_ace_ip_t       dip;            /**< Destination IP address */
    vtss_ace_u48_t      data;           /**< Not UDP/TCP: IP data */
    vtss_ace_udp_tcp_t  sport;          /**< UDP/TCP: Source port */
    vtss_ace_udp_tcp_t  dport;          /**< UDP/TCP: Destination port */
    vtss_ace_bit_t      tcp_fin;        /**< TCP FIN */
    vtss_ace_bit_t      tcp_syn;        /**< TCP SYN */
    vtss_ace_bit_t      tcp_rst;        /**< TCP RST */
    vtss_ace_bit_t      tcp_psh;        /**< TCP PSH */
    vtss_ace_bit_t      tcp_ack;        /**< TCP ACK */
    vtss_ace_bit_t      tcp_urg;        /**< TCP URG */
    vtss_ace_bit_t      sip_eq_dip;     /**< SIP equals DIP  */
    vtss_ace_bit_t      sport_eq_dport; /**< SPORT equals DPORT  */
    vtss_ace_bit_t      seq_zero;       /**< TCP sequence number is zero */
#if defined(VTSS_FEATURE_ACL_V2)
    vtss_ace_ptp_t      ptp;            /**< PTP filtering (overrides sip field) */
    vtss_ace_sip_smac_t sip_smac;       /**< SIP/SMAC matching (overrides sip field) */
#endif /* VTSS_FEATURE_ACL_V2 */
} vtss_ace_frame_ipv4_t;
        
/** \brief Frame data for VTSS_ACE_TYPE_IPV6 */
typedef struct
{
    vtss_ace_u8_t      proto;          /**< IPv6 protocol */
    vtss_ace_u128_t    sip;            /**< IPv6 source address (byte 0-7 ignored for ACL_V2) */
    vtss_ace_bit_t     ttl;            /**< TTL zero */
    vtss_ace_u8_t      ds;             /**< DS field */
    vtss_ace_u48_t     data;           /**< Not UDP/TCP: IP data */
    vtss_ace_udp_tcp_t sport;          /**< UDP/TCP: Source port */
    vtss_ace_udp_tcp_t dport;          /**< UDP/TCP: Destination port */
    vtss_ace_bit_t     tcp_fin;        /**< TCP FIN */
    vtss_ace_bit_t     tcp_syn;        /**< TCP SYN */
    vtss_ace_bit_t     tcp_rst;        /**< TCP RST */
    vtss_ace_bit_t     tcp_psh;        /**< TCP PSH */
    vtss_ace_bit_t     tcp_ack;        /**< TCP ACK */
    vtss_ace_bit_t     tcp_urg;        /**< TCP URG */
    vtss_ace_bit_t     sip_eq_dip;     /**< SIP equals DIP  */
    vtss_ace_bit_t     sport_eq_dport; /**< SPORT equals DPORT  */
    vtss_ace_bit_t     seq_zero;       /**< TCP sequence number is zero */
#if defined(VTSS_FEATURE_ACL_V2)
    vtss_ace_ptp_t     ptp;            /**< PTP filtering (overrides sip byte 0-3) */
#endif /* VTSS_FEATURE_ACL_V2 */
} vtss_ace_frame_ipv6_t;

/** \brief Access Control Entry */
typedef struct
{
    vtss_ace_id_t        id;           /**< ACE ID, must be different from VTSS_ACE_ID_LAST */
#if defined(VTSS_ARCH_SERVAL)
    u8                   lookup;       /**< Lookup, any non-zero value means second lookup */
    BOOL                 isdx_enable;  /**< Use VID value for ISDX value */
    BOOL                 isdx_disable; /**< Match only frames with ISDX zero */
#endif /* VTSS_ARCH_SERVAL */
#if defined(VTSS_FEATURE_ACL_V1)
    vtss_port_no_t       port_no;      /**< Port number or VTSS_PORT_NO_ANY */
#endif /* VTSS_FEATURE_ACL_V1 */
#if defined(VTSS_FEATURE_ACL_V2)
    BOOL                 port_list[VTSS_PORT_ARRAY_SIZE]; /**< Port list */ 
#endif /* VTSS_FEATURE_ACL_V2 */
    vtss_ace_u8_t        policy;       /**< Policy number */
    vtss_ace_type_t      type;         /**< ACE frame type */
    vtss_acl_action_t    action;       /**< ACE action */
                                   
    vtss_ace_bit_t       dmac_mc;      /**< Multicast DMAC */
    vtss_ace_bit_t       dmac_bc;      /**< Broadcast DMAC */

    vtss_ace_vlan_t      vlan;         /**< VLAN Tag */

    union
    {
        /* VTSS_ACE_TYPE_ANY: No specific fields */
        vtss_ace_frame_etype_t etype;  /**< VTSS_ACE_TYPE_ETYPE */
        vtss_ace_frame_llc_t   llc;    /**< VTSS_ACE_TYPE_LLC */
        vtss_ace_frame_snap_t  snap;   /**< VTSS_ACE_TYPE_SNAP */
        vtss_ace_frame_arp_t   arp;    /**< VTSS_ACE_TYPE_ARP */
        vtss_ace_frame_ipv4_t  ipv4;   /**< VTSS_ACE_TYPE_IPV4 */
        vtss_ace_frame_ipv6_t  ipv6;   /**< VTSS_ACE_TYPE_IPV6 */
    } frame; /**< Frame type specific data */
} vtss_ace_t;

/**
 * \brief Initialize ACE to default values.
 *
 * \param inst [IN]  Target instance reference.
 * \param type [IN]  ACE type.
 * \param ace [OUT]  ACE structure.
 *
 * \return Return code.
 **/
vtss_rc vtss_ace_init(const vtss_inst_t      inst,
                      const vtss_ace_type_t  type,
                      vtss_ace_t             *const ace);


/**
 * \brief Add/modify ACE.
 *
 * \param inst [IN]         Target instance reference.
 * \param ace_id_next [IN]  ACE ID of next entry.
 *                          The ACE will be added before the entry with this ID.
 *                          VTSS_ACE_ID_LAST is reserved for inserting last.
 * \param ace [IN]          ACE structure.
 *
 * \return Return code.
 **/
vtss_rc vtss_ace_add(const vtss_inst_t    inst,
                     const vtss_ace_id_t  ace_id_next,
                     const vtss_ace_t     *const ace);



/**
 * \brief Delete ACE.
 *
 * \param inst [IN]    Target instance reference.
 * \param ace_id [IN]  ACE ID.
 *
 * \return Return code.
 **/
vtss_rc vtss_ace_del(const vtss_inst_t    inst,
                     const vtss_ace_id_t  ace_id);



/** \brief ACE hit counter */
typedef u32 vtss_ace_counter_t; 

/**
 * \brief Get ACE counter.
 *
 * \param inst [IN]      Target instance reference.
 * \param ace_id [IN]    ACE ID.
 * \param counter [OUT]  ACE counter.
 *
 * \return Return code.
 **/
vtss_rc vtss_ace_counter_get(const vtss_inst_t    inst,
                             const vtss_ace_id_t  ace_id,
                             vtss_ace_counter_t   *const counter);


/**
 * \brief Clear ACE counter.
 *
 * \param inst [IN]    Target instance reference.
 * \param ace_id [IN]  ACE ID.
 *
 * \return Return code.
 **/
vtss_rc vtss_ace_counter_clear(const vtss_inst_t    inst,
                               const vtss_ace_id_t  ace_id);

#endif /* VTSS_FEATURE_ACL */
#ifdef __cplusplus
}
#endif
#endif /* _VTSS_SECURITY_API_H_ */
