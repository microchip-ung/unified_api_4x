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

#ifndef _VTSS_VCAP_STATE_H_
#define _VTSS_VCAP_STATE_H_


#if defined(VTSS_FEATURE_VCAP)

#if defined(VTSS_ARCH_LUTON26)
#define VTSS_FEATURE_IS1  /* VCAP IS1 */
#define VTSS_FEATURE_IS2  /* VCAP IS2 */
#define VTSS_FEATURE_ES0  /* VCAP ES0 */
#endif /* VTSS_ARCH_LUTON26 */

#if defined(VTSS_ARCH_SERVAL)
#if defined(VTSS_ARCH_SERVAL_CPU)
#define VTSS_FEATURE_IS0  /* VCAP IS0 */
#endif /* VTSS_ARCH_SERVAL_CPU */
#define VTSS_FEATURE_IS1  /* VCAP IS1 */
#define VTSS_FEATURE_IS2  /* VCAP IS2 */
#define VTSS_FEATURE_ES0  /* VCAP ES0 */
#endif /* VTSS_ARCH_SERVAL */

#if defined(VTSS_ARCH_JAGUAR_1)
#define VTSS_FEATURE_IS0  /* VCAP IS0 */
#define VTSS_FEATURE_IS1  /* VCAP IS1 */
#define VTSS_FEATURE_IS2  /* VCAP IS2 */
#define VTSS_FEATURE_ES0  /* VCAP ES0 */
#endif /* VTSS_ARCH_JAGUAR_1 */

#if defined(VTSS_ARCH_JAGUAR_2)
#define VTSS_FEATURE_IS2            /* VCAP IS2 */
#define VTSS_FEATURE_ES0            /* VCAP ES0 */
#define VTSS_FEATURE_CLM            /* VCAP CLM */
#define VTSS_FEATURE_LPM            /* VCAP LPM */
#define VTSS_FEATURE_VCAP_SUPER     /* VCAP_SUPER pool */
#if defined(VTSS_CHIP_SPARX_IV_52)
#define VTSS_VCAP_SUPER_BLK_CNT  4   /* Number of VCAP_SUPER blocks */
#elif defined(VTSS_CHIP_SERVAL_2)
#define VTSS_VCAP_SUPER_BLK_CNT  5   /* Number of VCAP_SUPER blocks */
#elif defined(VTSS_CHIP_SPARX_IV_44)
#define VTSS_VCAP_SUPER_BLK_CNT  6   /* Number of VCAP_SUPER blocks */
#else
#define VTSS_VCAP_SUPER_BLK_CNT  8   /* Number of VCAP_SUPER blocks */
#endif
#define VTSS_VCAP_SUPER_ROW_CNT  256 /* Number of rows in one VCAP super block (full rules) */
#define VTSS_VCAP_SUPER_RULE_CNT (VTSS_VCAP_SUPER_BLK_CNT * VTSS_VCAP_SUPER_ROW_CNT * 2) /* Two rules per row */
#endif /* VTSS_ARCH_JAGUAR_2 */

/** \brief VCAP key size */
typedef enum
{
    VTSS_VCAP_KEY_SIZE_FULL,     /**< Full (1/1) key */
    VTSS_VCAP_KEY_SIZE_HALF,     /**< Half (1/2) key */
    VTSS_VCAP_KEY_SIZE_QUARTER,  /**< Quarter (1/4) key */
    VTSS_VCAP_KEY_SIZE_EIGHTH,   /**< Eighth (1/8) key */
    VTSS_VCAP_KEY_SIZE_SIXTEENTH /**< Sixteeth (1/16) key */
} vtss_vcap_key_size_t;

#define VTSS_VCAP_KEY_SIZE_LAST VTSS_VCAP_KEY_SIZE_SIXTEENTH
#define VTSS_VCAP_KEY_SIZE_MAX  (VTSS_VCAP_KEY_SIZE_LAST + 1)

/* Resource change data */
typedef struct {
    u32 add;                             /* Number of added items */
    u32 del;                             /* Number of deleted items */
    u32 add_key[VTSS_VCAP_KEY_SIZE_MAX]; /* Added rules for each key size */
    u32 del_key[VTSS_VCAP_KEY_SIZE_MAX]; /* Deleted rules for each key size */
} vtss_res_chg_t;

/* Resource change information */
typedef struct {
#if defined(VTSS_FEATURE_EVC)
    BOOL            port_add[VTSS_PORT_ARRAY_SIZE];
    BOOL            port_del[VTSS_PORT_ARRAY_SIZE];
    BOOL            port_chg[VTSS_PORT_ARRAY_SIZE];
    BOOL            port_nni[VTSS_PORT_ARRAY_SIZE];
    BOOL            evc_add;
    BOOL            evc_del;
    BOOL            ece_add;
    BOOL            ece_del;
    BOOL            es0_add[VTSS_PORT_ARRAY_SIZE];
    BOOL            es0_del[VTSS_PORT_ARRAY_SIZE];
    vtss_ece_dir_t  dir_old;
    vtss_ece_dir_t  dir_new;
#if defined(VTSS_ARCH_SERVAL)
    vtss_ece_rule_t rule_old;
    vtss_ece_rule_t rule_new;
#endif /* VTSS_ARCH_SERVAL */
    BOOL            port_leaf_old[VTSS_PORT_ARRAY_SIZE];
    BOOL            port_leaf_new[VTSS_PORT_ARRAY_SIZE];
    BOOL            etree_old;
    BOOL            etree_new;
#endif /* VTSS_FEATURE_EVC */
    vtss_res_chg_t  is0;
    vtss_res_chg_t  is1;
    vtss_res_chg_t  clm_a;
    vtss_res_chg_t  clm_b;
    vtss_res_chg_t  clm_c;
    vtss_res_chg_t  lpm;
    vtss_res_chg_t  is2;
    vtss_res_chg_t  es0;
    vtss_res_chg_t  isdx;
    vtss_res_chg_t  esdx;
} vtss_res_t;

typedef enum {
    VTSS_VCAP_TYPE_NONE,
    VTSS_VCAP_TYPE_IS0,
    VTSS_VCAP_TYPE_IS1,
    VTSS_VCAP_TYPE_IS2,
    VTSS_VCAP_TYPE_ES0,
    VTSS_VCAP_TYPE_CLM_A,
    VTSS_VCAP_TYPE_CLM_B,
    VTSS_VCAP_TYPE_CLM_C,
    VTSS_VCAP_TYPE_LPM
} vtss_vcap_type_t;

/* VCAP ID */
typedef u64 vtss_vcap_id_t;

/* VCAP users in prioritized order */
typedef enum {
    /* IS0 users */
    VTSS_IS0_USER_EVC,         /* EVC (JR1) */
    VTSS_IS0_USER_MPLS_LL,     /* MPLS link layer (Serval) */
    VTSS_IS0_USER_MPLS_MLBS_3, /* MPLS label stack depth 3 (Serval) */
    VTSS_IS0_USER_MPLS_MLBS_2, /* MPLS label stack depth 2 (Serval) */
    VTSS_IS0_USER_MPLS_MLBS_1, /* MPLS label stack depth 1 (Serval) */

    /* IS1 users */
    VTSS_IS1_USER_TT_LOOP,  /* TT_LOOP (first lookup) */
    VTSS_IS1_USER_VCL,      /* VCL (first lookup) */
    VTSS_IS1_USER_VLAN,     /* VLAN translation (first lookup) */
    VTSS_IS1_USER_MEP,      /* MEP (first lookup, L26/SRVL) */
    VTSS_IS1_USER_EVC,      /* EVC (first lookup, L26/SRVL) */
    VTSS_IS1_USER_EFE,      /* EFE (first lookup, SRVL) */
    VTSS_IS1_USER_QOS,      /* QoS QCL (second lookup for L26/JR1, third lookup for SRVL) */
    VTSS_IS1_USER_ACL,      /* ACL SIP/SMAC (third lookup, L26) */
    VTSS_IS1_USER_SSM,      /* SSM (first lookup, L26) */

    /* IS2 users */
    VTSS_IS2_USER_IGMP,     /* IGMP rules (first lookup, JR1) */
    VTSS_IS2_USER_SSM,      /* SSM rules (first lookup, JR1) */
    VTSS_IS2_USER_IGMP_ANY, /* IGMP any rules (first lookup, JR1) */
    VTSS_IS2_USER_EEE,      /* EEE loopback port rules (second lookup, JR1) */
    VTSS_IS2_USER_ACL_PTP,  /* ACL PTP rules (second lookup, L26) */
    VTSS_IS2_USER_ACL,      /* ACL rules (first lookup for L26, second lookup for JR1) */
    VTSS_IS2_USER_ACL_SIP,  /* ACL SIP/SMAC rules (Serval) */

    /* ES0 users */
    VTSS_ES0_USER_TT_LOOP,  /* TT_LOOP (first lookup) */
    VTSS_ES0_USER_VLAN,     /* VLAN translation */
    VTSS_ES0_USER_MEP,      /* MEP rules */
    VTSS_ES0_USER_EVC,      /* EVC rules */
    VTSS_ES0_USER_EFE,      /* EFE rules */
    VTSS_ES0_USER_TX_TAG,   /* VLAN Tx tagging */
    VTSS_ES0_USER_MPLS,     /* MPLS (Serval) */

    /* LPM users */
    VTSS_LPM_USER_ACL,      /* ACL SIP/SMAC */
    VTSS_LPM_USER_L3,       /* L3 */
} vtss_vcap_user_t;

#if defined(VTSS_FEATURE_IS0)

#if defined(VTSS_ARCH_JAGUAR_1)

/* IS0 action */
typedef struct {
    BOOL       s1_dmac_ena;
    u8         vlan_pop_cnt;
    BOOL       vid_ena;
    BOOL       pcp_dei_ena;
    u8         pcp;
    BOOL       dei;
    u8         pag;
    u16        isdx;
    vtss_vid_t vid;
} vtss_is0_action_t;

typedef enum
{
    VTSS_IS0_TYPE_ISID,
    VTSS_IS0_TYPE_DBL_VID,
    VTSS_IS0_TYPE_MPLS,
    VTSS_IS0_TYPE_MAC_ADDR
} vtss_is0_type_t;

typedef struct {
    vtss_vcap_bit_t tagged;
    vtss_vcap_vid_t vid;
    vtss_vcap_u8_t  pcp;
    vtss_vcap_bit_t dei;
    vtss_vcap_bit_t s_tag;
} vtss_vcap_tag_t;

typedef enum {
    VTSS_IS0_PROTO_ANY,
    VTSS_IS0_PROTO_NON_IP,
    VTSS_IS0_PROTO_IPV4,
    VTSS_IS0_PROTO_IPV6
} vtss_is0_proto_t;

/* IS0 key */
typedef struct {
    vtss_port_no_t  port_no;
    vtss_is0_type_t type;

    union {
        struct {
            vtss_vcap_tag_t  outer_tag;
            vtss_vcap_tag_t  inner_tag;
            vtss_vcap_u8_t   dscp;
            vtss_is0_proto_t proto;
        } dbl_vid;

        struct {
            vtss_vcap_u48_t dmac;    /**< DMAC */
        } mac_addr;
    } data;
} vtss_is0_key_t;

/* IS0 entry */
typedef struct {
    vtss_is0_action_t action;
    vtss_is0_key_t    key;
} vtss_is0_entry_t;

/* IS0 data */
typedef struct {
    vtss_is0_entry_t *entry;
} vtss_is0_data_t;

#elif defined(VTSS_ARCH_SERVAL)

// In- and Out-segment encapsulation counts

#define VTSS_MPLS_IN_ENCAP_CNT          128     /* Number of HW entries */
#define VTSS_MPLS_IN_ENCAP_LABEL_CNT    3       /* Number of HW labels supported at ingress */
#define VTSS_MPLS_OUT_ENCAP_LABEL_CNT   3       /* Number of HW labels supported at egress */

typedef enum {
    VTSS_MLL_ETHERTYPE_DOWNSTREAM_ASSIGNED = 1,
    VTSS_MLL_ETHERTYPE_UPSTREAM_ASSIGNED   = 2
} vtss_mll_ethertype_t;

typedef enum {
    VTSS_IS0_MLBS_OAM_NONE    = 0,
    VTSS_IS0_MLBS_OAM_VCCV1   = 1,
    VTSS_IS0_MLBS_OAM_VCCV2   = 2,
    VTSS_IS0_MLBS_OAM_VCCV3   = 3,
    VTSS_IS0_MLBS_OAM_GAL_MEP = 4,
    VTSS_IS0_MLBS_OAM_GAL_MIP = 5
} vtss_is0_mlbs_oam_t;

typedef enum {
    VTSS_IS0_MLBS_POPCOUNT_0  = 1,
    VTSS_IS0_MLBS_POPCOUNT_14 = 2,
    VTSS_IS0_MLBS_POPCOUNT_18 = 3,
    VTSS_IS0_MLBS_POPCOUNT_22 = 4,
    VTSS_IS0_MLBS_POPCOUNT_26 = 5,
    VTSS_IS0_MLBS_POPCOUNT_30 = 6,
    VTSS_IS0_MLBS_POPCOUNT_34 = 7
} vtss_is0_mlbs_popcount_t;

typedef enum {
    VTSS_IS0_TYPE_MLL,
    VTSS_IS0_TYPE_MLBS
} vtss_is0_type_t;

typedef struct {
    BOOL                     physical[VTSS_PORT_ARRAY_SIZE]; /**< Physical port list */
    BOOL                     cpu;
} vtss_is0_b_portlist_t;

typedef enum {
    VTSS_IS0_TAGTYPE_UNTAGGED = 0,     /**< Frame is untagged */
    VTSS_IS0_TAGTYPE_CTAGGED  = 1,     /**< Frame is C-tagged */
    VTSS_IS0_TAGTYPE_STAGGED  = 2      /**< Frame is S-tagged */
} vtss_is0_tagtype_t;

typedef struct {
    vtss_port_no_t           ingress_port;
    vtss_is0_tagtype_t       tag_type;
    vtss_vid_t               b_vid;
    vtss_mac_t               dmac;
    vtss_mac_t               smac;
    vtss_mll_ethertype_t     ether_type;
    BOOL                     ingress_port_dontcare;     // TRUE = don't match field
    BOOL                     tag_type_dontcare;
    BOOL                     b_vid_dontcare;
    BOOL                     dmac_dontcare;
    BOOL                     smac_dontcare;
    BOOL                     ether_type_dontcare;
} vtss_is0_mll_key_t;

typedef struct {
    u8                       linklayer_index;
    BOOL                     mpls_forwarding;
    vtss_is0_b_portlist_t    b_portlist;
    u8                       cpu_queue;

    u16                      oam_isdx;
    BOOL                     oam_isdx_add_replace;

    // Below: Only used when mpls_forwarding == FALSE
    u16                      isdx;
    u8                       vprofile_index;
    vtss_vid_t               classified_vid;
    BOOL                     use_service_config;

    // Below: Only used when mpls_forwarding == FALSE && use_service_config == TRUE
    vtss_prio_t              qos;
    vtss_dp_level_t          dp;
} vtss_is0_mll_action_t;

typedef struct {
    u32                         linklayer_index;
    struct {
        u32                     value;                          /* Label value (20 bits) */
        u32                     value_mask;                     /* Label bits to match */
        u8                      tc;                             /* TC value (0-7) */
        u8                      tc_mask;                        /* TC bits to match */
    } label_stack[VTSS_MPLS_IN_ENCAP_LABEL_CNT];   // 0 is top-of-stack
} vtss_is0_mlbs_key_t;

typedef struct {
    u16                      isdx;
    u8                       cpu_queue;
    vtss_is0_b_portlist_t    b_portlist;
    vtss_is0_mlbs_oam_t      oam;
    BOOL                     oam_buried_mip;
    u8                       oam_reserved_label_value;
    BOOL                     oam_reserved_label_bottom_of_stack;
    u16                      oam_isdx;
    BOOL                     oam_isdx_add_replace;   /**< TRUE = replace; FALSE = add */

    BOOL                     cw_enable;
    BOOL                     terminate_pw;

    u8                       tc_label_index;
    u8                       ttl_label_index;
    u8                       swap_label_index;
    vtss_is0_mlbs_popcount_t pop_count; /**< 0, 14, 18, 22, 26, 30, 34 */

    BOOL                     e_lsp;
    u8                       tc_maptable_index;
    u8                       l_lsp_qos_class;
    BOOL                     add_tc_to_isdx;

    BOOL                     swap_is_bottom_of_stack;

    u8                       vprofile_index;
    vtss_vid_t               classified_vid;
    BOOL                     use_service_config;

    // Below: Only used when use_service_config == TRUE
    BOOL                     s_tag;          /**< FALSE = C-tag */
    vtss_tagprio_t           pcp;            /**< PCP value */
    vtss_dei_t               dei;            /**< DEI value */
} vtss_is0_mlbs_action_t;

typedef union {
    vtss_is0_mll_key_t       mll;
    vtss_is0_mlbs_key_t      mlbs;
} vtss_is0_key_t;

typedef union {
    vtss_is0_mll_action_t    mll;
    vtss_is0_mlbs_action_t   mlbs;
} vtss_is0_action_t;

/* IS0 entry */
typedef struct {
    vtss_is0_type_t          type;
    vtss_is0_key_t           key;
    vtss_is0_action_t        action;
} vtss_is0_entry_t;

/* IS0 data */
typedef struct {
    vtss_is0_entry_t *entry;
} vtss_is0_data_t;

#endif /* VTSS_ARCH_SERVAL_CE */

#endif /* VTSS_FEATURE_IS0 */

typedef enum {
    VTSS_FID_SEL_DEFAULT = 0,   /* Disabled: FID = classified VID. */
    VTSS_FID_SEL_SMAC,          /* Use FID_VAL for SMAC lookup in MAC table. */
    VTSS_FID_SEL_DMAC,          /* Use FID_VAL for DMAC lookup in MAC table. */
    VTSS_FID_SEL_BOTHMAC        /* Use FID_VAL for DMAC and SMAC lookup in MAC */
} vtss_fid_sel_t;

#if defined(VTSS_FEATURE_IS1) || defined(VTSS_FEATURE_CLM)
typedef struct {
    BOOL            dscp_enable;    /**< Enable DSCP classification */
    vtss_dscp_t     dscp;           /**< DSCP value */
    BOOL            dp_enable;      /**< Enable DP classification */
    vtss_dp_level_t dp;             /**< DP value */
    BOOL            prio_enable;    /**< Enable priority classification */
    vtss_prio_t     prio;           /**< Priority value */
    BOOL            vid_enable;     /**< VLAN ID enable */
    vtss_vid_t      vid;            /**< VLAN ID or VTSS_VID_NULL */
    vtss_fid_sel_t  fid_sel;        /**< FID Select */
    vtss_vid_t      fid_val;        /**< FID value */
    BOOL            pcp_dei_enable; /**< Enable PCP and DEI classification */
    vtss_tagprio_t  pcp;            /**< PCP value */
    vtss_dei_t      dei;            /**< DEI value */
    BOOL            host_match;     /**< Host match */
    BOOL            isdx_enable;    /**< ISDX enable */
    u16             isdx;           /**< ISDX value */
    BOOL            pag_enable;     /**< PAG enable */
    u8              pag;            /**< PAG value */
    BOOL            pop_enable;     /**< VLAN POP enable */
    u8              pop;            /**< VLAN POP count */
    BOOL            oam_enable;     /**< OAM detection enable */
#if defined(VTSS_ARCH_SERVAL_CE)
    vtss_mce_oam_detect_t oam_detect; /**< OAM detection */
#endif /* VTSS_ARCH_SERVAL_CE */
} vtss_is1_action_t;

typedef enum
{
    VTSS_IS1_TYPE_ANY,      /**< Any frame type */
    VTSS_IS1_TYPE_ETYPE,    /**< Ethernet Type */
    VTSS_IS1_TYPE_LLC,      /**< LLC */
    VTSS_IS1_TYPE_SNAP,     /**< SNAP */
    VTSS_IS1_TYPE_IPV4,     /**< IPv4 */
    VTSS_IS1_TYPE_IPV6,     /**< IPv6 */
    VTSS_IS1_TYPE_SMAC_SIP  /**< SMAC/SIP */
} vtss_is1_type_t;

typedef struct
{
    vtss_vcap_bit_t dmac_mc; /**< Multicast DMAC */
    vtss_vcap_bit_t dmac_bc; /**< Broadcast DMAC */
    vtss_vcap_u48_t smac;    /**< SMAC */
#if defined(VTSS_ARCH_SERVAL) || defined(VTSS_ARCH_JAGUAR_2)
    vtss_vcap_u48_t dmac;    /**< DMAC */
#endif /* VTSS_ARCH_SERVAL/JAGUAR_2 */
} vtss_is1_mac_t; /**< MAC header */

typedef struct
{
    vtss_vcap_vr_t  vid;        /**< VLAN ID (12 bit) */
    vtss_vcap_u8_t  pcp;        /**< PCP (3 bit) */
    vtss_vcap_bit_t dei;        /**< DEI */
    vtss_vcap_bit_t tagged;     /**< Tagged frame */
    vtss_vcap_bit_t s_tag;      /**< S-tag type */
} vtss_is1_tag_t; /**< VLAN Tag */

typedef struct {
    vtss_vcap_u16_t etype; /**< Ethernet Type value */
    vtss_vcap_u32_t data;  /**< MAC data */
} vtss_is1_frame_etype_t;

typedef struct {
    vtss_vcap_u48_t data; /**< Data */
} vtss_is1_frame_llc_t;

typedef struct {
    vtss_vcap_u48_t data; /**< Data */
} vtss_is1_frame_snap_t;

typedef struct {
    vtss_vcap_bit_t ip_mc;    /**< IP_MC field */
    vtss_vcap_bit_t fragment; /**< Fragment */
    vtss_vcap_bit_t options;  /**< Header options */
    vtss_vcap_vr_t  dscp;     /**< DSCP field (6 bit) */
    vtss_vcap_u8_t  proto;    /**< Protocol */
    vtss_vcap_ip_t  sip;      /**< Source IP address */
#if defined(VTSS_ARCH_SERVAL) || defined(VTSS_ARCH_JAGUAR_2)
    vtss_vcap_ip_t  dip;      /**< Destination IP address */
#endif /* VTSS_ARCH_SERVAL/JAGUAR_2 */
    vtss_vcap_vr_t  sport;    /**< UDP/TCP: Source port */
    vtss_vcap_vr_t  dport;    /**< UDP/TCP: Destination port */
} vtss_is1_frame_ipv4_t;

typedef struct {
    vtss_vcap_bit_t  ip_mc; /**< IP_MC field */
    vtss_vcap_vr_t   dscp;  /**< DSCP field (6 bit) */
    vtss_vcap_u8_t   proto; /**< Protocol */
    vtss_vcap_u128_t sip;   /**< Source IP address */
#if defined(VTSS_ARCH_SERVAL) || defined(VTSS_ARCH_JAGUAR_2)
    vtss_vcap_u128_t dip;   /**< Destination IP adddress */
#endif /* VTSS_ARCH_SERVAL/JAGUAR_2 */
    vtss_vcap_vr_t   sport; /**< UDP/TCP: Source port */
    vtss_vcap_vr_t   dport; /**< UDP/TCP: Destination port */
} vtss_is1_frame_ipv6_t;

typedef struct {
    vtss_port_no_t port_no; /**< Ingress port or VTSS_PORT_NO_NONE */
    vtss_mac_t     smac;    /**< SMAC */
    vtss_ip_t      sip;     /**< Source IP address */
} vtss_is1_frame_smac_sip_t;

typedef struct {
    vtss_is1_type_t      type;      /**< Frame type */

#if defined(VTSS_ARCH_SERVAL) || defined(VTSS_ARCH_JAGUAR_2)
    vtss_vcap_key_type_t key_type;  /**< Key type */
    vtss_vcap_u16_t      isdx;      /**< ISDX */
#endif /* VTSS_ARCH_SERVAL/JAGUAR_2 */

    BOOL                 port_list[VTSS_PORT_ARRAY_SIZE]; /**< Port list */
    vtss_is1_mac_t       mac;       /**< MAC header */
    vtss_is1_tag_t       tag;       /**< VLAN Tag */
    vtss_is1_tag_t       inner_tag; /**< Inner Tag, only 'tagged' field valid for L26/JR1 */

    union
    {
        /* VTSS_IS1_TYPE_ANY: No specific fields */
        vtss_is1_frame_etype_t    etype;    /**< VTSS_IS1_TYPE_ETYPE */
        vtss_is1_frame_llc_t      llc;      /**< VTSS_IS1_TYPE_LLC */
        vtss_is1_frame_snap_t     snap;     /**< VTSS_IS1_TYPE_SNAP */
        vtss_is1_frame_ipv4_t     ipv4;     /**< VTSS_IS1_TYPE_IPV4 */
        vtss_is1_frame_ipv6_t     ipv6;     /**< VTSS_IS1_TYPE_IPV6 */
        vtss_is1_frame_smac_sip_t smac_sip; /**< VTSS_IS1_TYPE_SMAC_SIP */
    } frame; /**< Frame type specific data */
} vtss_is1_key_t;

typedef struct {
    vtss_is1_action_t action;
    vtss_is1_key_t    key;
} vtss_is1_entry_t;

typedef struct {
    u8               lookup;      /* Lookup (Serval) or first flag (L26/JR) */
    u32              vid_range;   /* VID range */
    u32              dscp_range;  /* DSCP range */
    u32              sport_range; /* Source port range */
    u32              dport_range; /* Destination port range */
    vtss_is1_entry_t *entry;      /* IS1 entry */
} vtss_is1_data_t;
#endif /* VTSS_FEATURE_IS1/CLM */

#if defined(VTSS_FEATURE_LPM)
typedef enum {
    VTSS_LPM_KEY_SGL_IP4,
    VTSS_LPM_KEY_DBL_IP4,
    VTSS_LPM_KEY_SGL_IP6,
    VTSS_LPM_KEY_DBL_IP6,
} vtss_lpm_key_type_t;

typedef struct {
    BOOL           dst_ena;
    vtss_vcap_ip_t xip;
} vtss_lpm_sgl_ip4_t;

typedef struct {
    BOOL             dst_ena;
    vtss_vcap_u128_t xip;
} vtss_lpm_sgl_ip6_t;

typedef struct {
    vtss_lpm_key_type_t type;
    union {
        vtss_lpm_sgl_ip4_t sgl_ip4;
        vtss_lpm_sgl_ip6_t sgl_ip6;
    } data;
} vtss_lpm_key_t;

typedef struct {
    vtss_mac_t             mac;
    vtss_packet_rx_queue_t cpu_queue;
    BOOL                   secur_match_mac_ena;
    BOOL                   arp_ena;
} vtss_lpm_arp_entry_t;

typedef struct {
    u32 arp_ptr;
    u8  ecmp_cnt;
    u8  rgid;
} vtss_lpm_arp_ptr_t;

typedef struct {
    u8 type;
    union {
        vtss_lpm_arp_entry_t arp_entry;
        vtss_lpm_arp_ptr_t   arp_ptr;
    } data;
} vtss_lpm_action_t;

typedef struct {
    vtss_lpm_key_t    key;
    vtss_lpm_action_t action;
} vtss_lpm_entry_t;

typedef struct {
    vtss_lpm_entry_t *entry;
} vtss_lpm_data_t;
#endif /* VTSS_FEATURE_LPM */

#if defined(VTSS_FEATURE_IS2)
typedef struct {
    BOOL       first;        /* First or second lookup */
    BOOL       host_match;   /* Host match from IS1 */
    BOOL       udp_tcp_any;  /* Match UDP/TCP frames */
    BOOL       smac_sip6;    /* SMAC/SIPv6 */
#if defined(VTSS_ARCH_JAGUAR_1)
    u32        type;                /* Entry type */
    u32        type_mask;           /* Type mask */
    u32        action_ext;          /* Action extensions */
    u32        chip_port_mask[2];   /* Mask used when non-zero, one per possible chip */
    BOOL       include_int_ports;   /* Include internal ports for VTSS_PORT_NO_ANY */
    BOOL       include_stack_ports; /* Include stack ports for VTSS_PORT_NO_ANY */
#endif /* VTSS_ARCH_JAGUAR_1 */
    vtss_ace_t ace;          /* ACE structure */
} vtss_is2_entry_t;

typedef struct {
    u32              srange;       /* Source port range */
    u32              drange;       /* Destination port range */
#if defined(VTSS_ARCH_JAGUAR_2)
    u32              cnt_id;       /* Counter ID */
#endif /* VTSS_ARCH_JAGUAR_2 */
#if defined(VTSS_ARCH_LUTON26)
    u8               policer_type; /* Policer type */
    u8               policer;      /* Allocated policer index */
#endif /* VTSS_ARCH_LUTON26 */
    vtss_is2_entry_t *entry; /* ACE data */
} vtss_is2_data_t;
#endif /* VTSS_FEATURE_IS2 */

#if defined(VTSS_FEATURE_ES0)
/* ES0 tag */
typedef enum {
    VTSS_ES0_TAG_NONE, /* No ES0 tag */
    VTSS_ES0_TAG_ES0,  /* ES0 tag only */
    VTSS_ES0_TAG_PORT, /* ES0 tag and port tag, if enabled (not valid for Serval inner_tag) */
    VTSS_ES0_TAG_BOTH  /* ES0 and port tag (not valid for Serval) */
} vtss_es0_tag_t;

/* ES0 TPI */
typedef enum {
    VTSS_ES0_TPID_C,   /* C-tag */
    VTSS_ES0_TPID_S,   /* S-tag */
    VTSS_ES0_TPID_PORT /* Port tag type */
} vtss_es0_tpid_t;

/* ES0 QOS */
typedef enum {
    VTSS_ES0_QOS_CLASS,
    VTSS_ES0_QOS_ES0,
    VTSS_ES0_QOS_PORT,
    VTSS_ES0_QOS_MAPPED
} vtss_es0_qos_t;

#if defined(VTSS_ARCH_SERVAL) || defined(VTSS_ARCH_JAGUAR_2)

#define VTSS_MPLS_OUT_ENCAP_CNT         1023    /* Number of HW entries */

/* ES0 VID information */
typedef struct {
    BOOL       sel; /* Enable to select value (default classified) */
    vtss_vid_t val; /* VLAN ID value */
} vtss_es0_vid_t;

/* ES0 PCP selection */
typedef enum {
    VTSS_ES0_PCP_CLASS,  /* Classified PCP */
    VTSS_ES0_PCP_ES0,    /* PCP_VAL in ES0 */
    VTSS_ES0_PCP_MAPPED, /* Mapped PCP */
    VTSS_ES0_PCP_QOS     /* QoS class */
} vtss_es0_pcp_sel_t;

/* ES0 PCP information */
typedef struct {
    vtss_es0_pcp_sel_t sel; /* PCP selection */
    vtss_tagprio_t     val; /* PCP value */
} vtss_es0_pcp_t;

/* ES0 DEI selection */
typedef enum {
    VTSS_ES0_DEI_CLASS,  /* Classified DEI */
    VTSS_ES0_DEI_ES0,    /* DEI_VAL in ES0 */
    VTSS_ES0_DEI_MAPPED, /* Mapped DEI */
    VTSS_ES0_DEI_DP      /* DP value */
} vtss_es0_dei_sel_t;

/* ES0 DEI information */
typedef struct {
    vtss_es0_dei_sel_t sel; /* DEI selection */
    vtss_dei_t         val; /* DEI value */
} vtss_es0_dei_t;

/* ES0 tag information */
typedef struct {
    vtss_es0_tag_t  tag;  /* Tag selection */
    vtss_es0_tpid_t tpid; /* TPID selection */
    vtss_es0_vid_t  vid;  /* VLAN ID selection and value */
    vtss_es0_pcp_t  pcp;  /* PCP selection and value */
    vtss_es0_dei_t  dei;  /* DEI selection and value */
} vtss_es0_tag_conf_t;

typedef enum {
    VTSS_ES0_MPLS_ENCAP_LEN_NONE = 0,
    VTSS_ES0_MPLS_ENCAP_LEN_14   = 2,
    VTSS_ES0_MPLS_ENCAP_LEN_18   = 3,
    VTSS_ES0_MPLS_ENCAP_LEN_22   = 4,
    VTSS_ES0_MPLS_ENCAP_LEN_26   = 5,
    VTSS_ES0_MPLS_ENCAP_LEN_30   = 6,
    VTSS_ES0_MPLS_ENCAP_LEN_34   = 7
} vtss_es0_mpls_encap_len_t;
#endif /* VTSS_ARCH_SERVAL/JAGUAR_2 */

/* ES0 action */
typedef struct {
#if defined(VTSS_ARCH_SERVAL) || defined(VTSS_ARCH_JAGUAR_2)
    /* For Serval, the following two fields replace the tag related fields below */
    vtss_es0_tag_conf_t       outer_tag;
    vtss_es0_tag_conf_t       inner_tag;
    BOOL                      mep_idx_enable;
    u8                        mep_idx;
    u32                       mpls_encap_idx;
    vtss_es0_mpls_encap_len_t mpls_encap_len;
    vtss_es0_mpls_encap_len_t mpls_pop_len;
#endif /* VTSS_ARCH_SERVAL/JAGUAR_2 */

    vtss_es0_tag_t  tag;
    vtss_es0_tpid_t tpid;
    vtss_es0_qos_t  qos;
    vtss_vid_t      vid_a; /* Outer/port tag */
    vtss_vid_t      vid_b; /* Inner/ES0 tag */
    u8              pcp;
    BOOL            dei;

    u16             esdx;  /* Jaguar/Serval only */
} vtss_es0_action_t;

typedef enum
{
    VTSS_ES0_TYPE_VID,
    VTSS_ES0_TYPE_ISDX /* Jaguar/Serval only */
} vtss_es0_type_t;

/* ES0 key */
typedef struct {
    vtss_port_no_t  port_no;    /* Egress port number or VTSS_PORT_NO_NONE */
    vtss_es0_type_t type;       /* Jaguar/Serval only */
    vtss_vcap_bit_t isdx_neq0;  /* Jaguar/Serval only */
    vtss_port_no_t  rx_port_no; /* Ingress port number or VTSS_PORT_NO_NONE */
    BOOL            vid_any;    /* Serval only: Match any vid */

    union {
        struct {
            vtss_vid_t     vid;
            vtss_vcap_u8_t pcp;
        } vid;

        struct {
            u16            isdx;
            vtss_vcap_u8_t pcp;
        } isdx;
    } data;
} vtss_es0_key_t;

/* ES0 entry */
typedef struct {
    vtss_es0_action_t action;
    vtss_es0_key_t    key;
} vtss_es0_entry_t;

#define VTSS_ES0_FLAG_MASK_PORT 0x00ff /* Flags related to ES0 egress port */
#define VTSS_ES0_FLAG_MASK_NNI  0xff00 /* Flags related to ES0 NNI port */

#define VTSS_ES0_FLAG_TPID      0x0001 /* Use port TPID in outer tag */
#define VTSS_ES0_FLAG_QOS       0x0002 /* Use port QoS setup in outer tag */
#define VTSS_ES0_FLAG_PCP_MAP   0x0100 /* Use mapped PCP for ECE */

/* ES0 data */
typedef struct {
    vtss_es0_entry_t *entry;
    u16              flags; 
    u8               prio;    /* ECE priority */
    vtss_port_no_t   port_no; /* Egress port number */
    vtss_port_no_t   nni;     /* NNI port affecting ES0 entry for UNI port */
} vtss_es0_data_t;
#endif /* VTSS_FEATURE_ES0 */

/* VCAP data */
typedef struct {
    vtss_vcap_key_size_t key_size;   /* Key size */
    union {
#if defined(VTSS_FEATURE_IS0)
        vtss_is0_data_t is0;
#endif /* VTSS_FEATURE_IS0 */
#if defined(VTSS_FEATURE_IS1) || defined(VTSS_FEATURE_CLM)
        vtss_is1_data_t is1;
#endif /* VTSS_FEATURE_IS1/CLM */
#if defined(VTSS_FEATURE_LPM)
        vtss_lpm_data_t lpm;
#endif /* VTSS_FEATURE_LPM */
#if defined(VTSS_FEATURE_IS2)
        vtss_is2_data_t is2;
#endif /* VTSS_FEATURE_IS2 */
#if defined(VTSS_FEATURE_ES0)
        vtss_es0_data_t es0;
#endif /* VTSS_FEATURE_ES0 */
    } u;
} vtss_vcap_data_t;

/* VCAP entry */
typedef struct vtss_vcap_entry_t {
    struct vtss_vcap_entry_t *next; /* Next in list */
    vtss_vcap_user_t         user;  /* User */
    vtss_vcap_id_t           id;    /* Entry ID */
    vtss_vcap_data_t         data;  /* Entry data */
    void                     *copy; /* Entry copy. Points to a copy of entry key/action (or NULL if not needed). */
} vtss_vcap_entry_t;

/* VCAP rule index */
typedef struct {
    u32                  row;      /* TCAM row */
    u32                  col;      /* TCAM column */
    vtss_vcap_key_size_t key_size; /* Rule key size */
} vtss_vcap_idx_t;

typedef struct {
    u32 count;     /* Actual number */
    u32 max_count; /* Maximum number */
} vtss_vcap_count_t;

#if defined(VTSS_FEATURE_VCAP_SUPER)
/* VCAP super object */
typedef struct {
    vtss_vcap_type_t  block_type[VTSS_VCAP_SUPER_BLK_CNT];
    vtss_rc           (* block_map)(struct vtss_state_s *vtss_state, u32 block, vtss_vcap_type_t type);
    vtss_rc           (* block_move)(struct vtss_state_s *vtss_state, u32 block, BOOL up);
    u32               row_count; /* Number of rows in one block */
    vtss_vcap_count_t block;
    u32               max_rule_count; /* Maximum number of rules */
    u32               rule_count;     /* Actual number of rules */
    vtss_vcap_entry_t table[VTSS_VCAP_SUPER_RULE_CNT]; /* Table */
    vtss_vcap_entry_t *free;          /* Free entries */
} vtss_vcap_super_obj_t;
#endif /* VTSS_FEATURE_VCAP_SUPER */

/* VCAP object */
typedef struct {
    /* VCAP data */
    u32               max_count;      /* Maximum number of rows */
    u32               count;          /* Actual number of rows */
    u32               max_rule_count; /* Maximum number of rules */
    u32               rule_count;     /* Actual number of rules */
    u32               key_count[VTSS_VCAP_KEY_SIZE_MAX]; /* Actual number of rule per key */
    vtss_vcap_entry_t *used;          /* Used entries */
    vtss_vcap_entry_t *free;          /* Free entries */
    const char        *name;          /* VCAP name for debugging */
    vtss_vcap_type_t  type;           /* VCAP type */

    /* VCAP methods */
    vtss_rc (* entry_get)(struct vtss_state_s *vtss_state, vtss_vcap_idx_t *idx, u32 *counter, BOOL clear);
    vtss_rc (* entry_add)(struct vtss_state_s *vtss_state, vtss_vcap_idx_t *idx, vtss_vcap_data_t *data, u32 counter);
    vtss_rc (* entry_del)(struct vtss_state_s *vtss_state, vtss_vcap_idx_t *idx);
    vtss_rc (* entry_move)(struct vtss_state_s *vtss_state, vtss_vcap_idx_t *idx, u32 count, BOOL up);

#if defined(VTSS_FEATURE_VCAP_SUPER)
    vtss_vcap_super_obj_t *vcap_super;
#endif /* VTSS_FEATURE_VCAP_SUPER */
} vtss_vcap_obj_t;

/* Special VCAP ID used to add last in list */
#define VTSS_VCAP_ID_LAST 0

/* VCAP ranges */
#define VTSS_VCAP_RANGE_CHK_CNT 8
#define VTSS_VCAP_RANGE_CHK_NONE 0xffffffff

/* VCAP range checker type */
typedef enum {
    VTSS_VCAP_RANGE_TYPE_SPORT,  /* UDP/TCP source port */
    VTSS_VCAP_RANGE_TYPE_DPORT,  /* UDP/TCP destination port */
    VTSS_VCAP_RANGE_TYPE_SDPORT, /* UDP/TCP source/destination port */
    VTSS_VCAP_RANGE_TYPE_VID,    /* VLAN ID */
    VTSS_VCAP_RANGE_TYPE_DSCP    /* DSCP */
} vtss_vcap_range_chk_type_t;

/* VCAP range checker entry */
typedef struct {
    vtss_vcap_range_chk_type_t type;  /* Range type */
    u32                        count; /* Reference count */
    u32                        min;   /* Lower value of range */
    u32                        max;   /* Upper value of range */
} vtss_vcap_range_chk_t;

/* VCAP range checker table */
typedef struct {
    vtss_vcap_range_chk_t entry[VTSS_VCAP_RANGE_CHK_CNT];
} vtss_vcap_range_chk_table_t;

#if defined(VTSS_FEATURE_IS0)
/* Number of IS0 rules */
#define VTSS_JR1_IS0_CNT  4096
#define VTSS_SRVL_IS0_CNT 768  /* Half entries */

#if defined(VTSS_ARCH_JAGUAR_1)
#define VTSS_IS0_CNT      VTSS_JR1_IS0_CNT
#else
#define VTSS_IS0_CNT      VTSS_SRVL_IS0_CNT
#endif /* VTSS_JR1_IS0_CNT */

/* IS0 information */
typedef struct {
    vtss_vcap_obj_t   obj;                 /* Object */
    vtss_vcap_entry_t table[VTSS_IS0_CNT]; /* Table */
#if defined(VTSS_OPT_WARM_START)
    vtss_is0_entry_t  copy[VTSS_IS0_CNT];  /* Copy of entries */
#endif /* VTSS_OPT_WARM_START */
} vtss_is0_info_t;
#endif /* VTSS_FEATURE_IS0 */

#if defined(VTSS_FEATURE_IS1)
/* Number of IS1 rules */
#define VTSS_L26_IS1_CNT  256
#define VTSS_SRVL_IS1_CNT 1024 /* Quarter entries */
#define VTSS_JR1_IS1_CNT  512

#if defined(VTSS_ARCH_JAGUAR_1)
#define VTSS_IS1_CNT      VTSS_JR1_IS1_CNT
#elif defined(VTSS_ARCH_SERVAL)
#define VTSS_IS1_CNT      VTSS_SRVL_IS1_CNT
#else
#define VTSS_IS1_CNT      VTSS_L26_IS1_CNT
#endif

/* IS1 information */
typedef struct {
    vtss_vcap_obj_t   obj;                 /* Object */
    vtss_vcap_entry_t table[VTSS_IS1_CNT]; /* Table */
#if defined(VTSS_OPT_WARM_START) || defined(VTSS_ARCH_SERVAL)
    vtss_is1_entry_t  copy[VTSS_IS1_CNT];  /* Copy of entries */
#endif /* VTSS_OPT_WARM_START */
} vtss_is1_info_t;
#endif /* VTSS_FEATURE_IS1 */

#if defined(VTSS_FEATURE_CLM)
/* CLM information */
typedef struct {
    vtss_vcap_obj_t obj; /* Object */
} vtss_clm_info_t;
#endif /* VTSS_FEATURE_CLM */

#if defined(VTSS_FEATURE_LPM)
/* LPM information */
typedef struct {
    vtss_vcap_obj_t obj; /* Object */
} vtss_lpm_info_t;
#endif /* VTSS_FEATURE_LPM */

#if defined(VTSS_FEATURE_IS2)
/* Number of IS2 rules */
#define VTSS_L26_IS2_CNT  256
#if defined(VTSS_ARCH_SEVILLE)
#define VTSS_SRVL_IS2_CNT 4096 /* Quarter entries */
#else
#define VTSS_SRVL_IS2_CNT 1024 /* Quarter entries */
#endif /* VTSS_ARCH_SEVILLE */
#define VTSS_JR1_IS2_CNT  512

#if defined(VTSS_ARCH_SERVAL)
#define VTSS_IS2_CNT      VTSS_SRVL_IS2_CNT
#elif defined(VTSS_ARCH_JAGUAR_1)
#define VTSS_IS2_CNT      VTSS_JR1_IS2_CNT
#elif defined(VTSS_ARCH_LUTON26)
#define VTSS_IS2_CNT      VTSS_L26_IS2_CNT
#elif defined(VTSS_ARCH_JAGUAR_2)
#define VTSS_IS2_CNT      0 /* VCAP_SUPER is used */
#define VTSS_ACL_CNT_SIZE 4096
#endif /* VTSS_ARCH_SERVAL */

/* IS2 information */
typedef struct {
    vtss_vcap_obj_t   obj;                 /* Object */
    vtss_vcap_entry_t table[VTSS_IS2_CNT]; /* Table */
#if defined(VTSS_OPT_WARM_START)
    vtss_is2_entry_t  copy[VTSS_IS2_CNT];  /* Copy of entries */
#endif /* VTSS_OPT_WARM_START */
} vtss_is2_info_t;
#endif /* VTSS_FEATURE_IS2 */

#if defined(VTSS_FEATURE_ES0)
#define VTSS_L26_ES0_CNT  256
#define VTSS_SRVL_ES0_CNT 1024
#define VTSS_JR1_ES0_CNT  4096
#define VTSS_JR2_ES0_CNT  4096

#if defined(VTSS_ARCH_JAGUAR_1)
#define VTSS_ES0_CNT VTSS_JR1_ES0_CNT
#elif defined(VTSS_ARCH_JAGUAR_2)
#define VTSS_ES0_CNT VTSS_JR2_ES0_CNT
#elif defined(VTSS_ARCH_SERVAL)
#define VTSS_ES0_CNT VTSS_SRVL_ES0_CNT
#else
#define VTSS_ES0_CNT VTSS_L26_ES0_CNT
#endif /* VTSS_ARCH_JAGUAR_1 */

/* ES0 information */
typedef struct {
    vtss_vcap_obj_t   obj;                 /* Object */
    vtss_vcap_entry_t table[VTSS_ES0_CNT]; /* Table */
#if defined(VTSS_OPT_WARM_START)
    vtss_es0_entry_t  copy[VTSS_ES0_CNT];  /* Copy of entries */
#endif /* VTSS_OPT_WARM_START */
} vtss_es0_info_t;
#endif /* VTSS_FEATURE_ES0 */

#if defined(VTSS_ARCH_SERVAL)
typedef struct {
    BOOL dmac_dip[3]; /* Aggregated dmac_dip flag - one per IS1 lookup */
} vtss_dmac_dip_conf_t;
#endif /* VTSS_ARCH_SERVAL */

typedef struct {
    /* CIL function pointers */
    vtss_rc (* range_commit)(struct vtss_state_s *vtss_state);
#if defined(VTSS_FEATURE_ES0)
    vtss_rc (* es0_entry_update)(struct vtss_state_s *vtss_state,
                                 vtss_vcap_idx_t *idx, vtss_es0_data_t *es0);
#endif /* VTSS_FEATURE_ES0 */
    vtss_rc (* acl_policer_set)(struct vtss_state_s *vtss_state,
                                const vtss_acl_policer_no_t policer_no);
    vtss_rc (* acl_port_set)(struct vtss_state_s *vtss_state,
                             const vtss_port_no_t port_no);
    vtss_rc (* acl_port_counter_get)(struct vtss_state_s *vtss_state,
                                     const vtss_port_no_t     port_no,
                                     vtss_acl_port_counter_t  *const counter);
    vtss_rc (* acl_port_counter_clear)(struct vtss_state_s *vtss_state,
                                       const vtss_port_no_t     port_no);
    vtss_rc (* acl_ace_add)(struct vtss_state_s *vtss_state,
                            const vtss_ace_id_t  ace_id,
                            const vtss_ace_t     *const ace);
    vtss_rc (* acl_ace_del)(struct vtss_state_s *vtss_state,
                            const vtss_ace_id_t  ace_id);
    vtss_rc (* acl_ace_counter_get)(struct vtss_state_s *vtss_state,
                                    const vtss_ace_id_t  ace_id,
                                    vtss_ace_counter_t   *const counter);
    vtss_rc (* acl_ace_counter_clear)(struct vtss_state_s *vtss_state,
                                      const vtss_ace_id_t  ace_id);

    /* Configuration/state */
    vtss_vcap_range_chk_table_t   range;
    u32                           counter[2]; /* Multi-chip support */
#if defined(VTSS_FEATURE_VCAP_SUPER)
    vtss_vcap_super_obj_t         vcap_super;
#endif /* VTSS_FEATURE_VCAP_SUPER */
#if defined(VTSS_FEATURE_IS0)
    vtss_is0_info_t               is0;
#endif /* VTSS_FEATURE_IS0 */
#if defined(VTSS_FEATURE_IS1)
    vtss_is1_info_t               is1;
#endif /* VTSS_FEATURE_IS1 */
#if defined(VTSS_FEATURE_IS2)
    vtss_is2_info_t               is2;
#endif /* VTSS_FEATURE_IS2 */
#if defined(VTSS_FEATURE_ES0)
    vtss_es0_info_t               es0;
#endif /* VTSS_FEATURE_ES0 */
#if defined(VTSS_FEATURE_CLM)
    vtss_clm_info_t               clm_a;
    vtss_clm_info_t               clm_b;
    vtss_clm_info_t               clm_c;
#endif /* VTSS_FEATURE_CLM */
#if defined(VTSS_FEATURE_LPM)
    vtss_clm_info_t               lpm;
#endif /* VTSS_FEATURE_LPM */
    vtss_acl_policer_conf_t       acl_policer_conf[VTSS_ACL_POLICERS];
#if defined(VTSS_ARCH_LUTON26)
    vtss_policer_alloc_t          acl_policer_alloc[VTSS_ACL_POLICERS];
#endif /* VTSS_ARCH_LUTON26 */
    vtss_acl_port_conf_t          acl_old_port_conf;
    vtss_acl_port_conf_t          acl_port_conf[VTSS_PORT_ARRAY_SIZE];
#if defined(VTSS_ARCH_SERVAL)
    vtss_vcap_port_conf_t         port_conf[VTSS_PORT_ARRAY_SIZE];
    vtss_vcap_port_conf_t         port_conf_old;
    vtss_dmac_dip_conf_t          dmac_dip_conf[VTSS_PORT_ARRAY_SIZE];/* Aggregated dmac_dip flag - per port per IS1 lookup */
#endif /* VTSS_ARCH_SERVAL */
#if defined(VTSS_ARCH_JAGUAR_2)
    u8 acl_cnt_alloc[VTSS_BF_SIZE(VTSS_ACL_CNT_SIZE)];
#endif /* VTSS_ARCH_JAGUAR_2 */
} vtss_vcap_state_t;

vtss_rc vtss_vcap_inst_create(struct vtss_state_s *vtss_state);
vtss_rc vtss_vcap_restart_sync(struct vtss_state_s *vtss_state);

void vtss_cmn_res_init(vtss_res_t *res);
vtss_rc vtss_cmn_vcap_res_check(vtss_vcap_obj_t *obj, vtss_res_chg_t *chg);
vtss_rc vtss_cmn_res_check(struct vtss_state_s *vtss_state, vtss_res_t *res);

BOOL vtss_vcap_udp_tcp_rule(const vtss_vcap_u8_t *proto);
vtss_rc vtss_vcap_range_alloc(vtss_vcap_range_chk_table_t *range_chk_table,
                              u32 *range,
                              vtss_vcap_range_chk_type_t type,
                              u32 min,
                              u32 max);
vtss_rc vtss_vcap_range_free(vtss_vcap_range_chk_table_t *range_chk_table,
                             u32 range);
vtss_rc vtss_vcap_udp_tcp_range_alloc(vtss_vcap_range_chk_table_t *range_chk_table,
                                      u32 *range, 
                                      const vtss_vcap_udp_tcp_t *port, 
                                      BOOL sport);
BOOL vtss_vcap_vr_rng2vm(vtss_vcap_vr_t *vr);
vtss_rc vtss_vcap_vr_alloc(vtss_vcap_range_chk_table_t *range_chk_table,
                           u32 *range,
                           vtss_vcap_range_chk_type_t type,
                           vtss_vcap_vr_t *vr);
vtss_rc vtss_vcap_range_commit(struct vtss_state_s *vtss_state, vtss_vcap_range_chk_table_t *range_new);
u32 vtss_vcap_key_rule_count(vtss_vcap_key_size_t key_size);
char *vtss_vcap_id_txt(struct vtss_state_s *vtss_state, vtss_vcap_id_t id);
vtss_rc vtss_vcap_lookup(struct vtss_state_s *vtss_state,
                         vtss_vcap_obj_t *obj, int user, vtss_vcap_id_t id, 
                         vtss_vcap_data_t *data, vtss_vcap_idx_t *idx);
vtss_rc vtss_vcap_del(struct vtss_state_s *vtss_state, vtss_vcap_obj_t *obj, int user, vtss_vcap_id_t id);
vtss_rc vtss_vcap_add(struct vtss_state_s *vtss_state, vtss_vcap_obj_t *obj, int user, vtss_vcap_id_t id, 
                      vtss_vcap_id_t ins_id, vtss_vcap_data_t *data, BOOL dont_add);
vtss_rc vtss_vcap_get_next_id(vtss_vcap_obj_t *obj, int user1, int user2, 
                              vtss_vcap_id_t id, vtss_vcap_id_t *ins_id);
#if defined(VTSS_FEATURE_VCAP_SUPER)
const char *vtss_vcap_type_txt(vtss_vcap_type_t type);
#endif /* VTSS_FEATURE_VCAP_SUPER */
#if defined(VTSS_FEATURE_IS0)
void vtss_vcap_is0_init(vtss_vcap_data_t *data, vtss_is0_entry_t *entry);
void vtss_vcap_debug_print_is0(struct vtss_state_s *vtss_state,
                               const vtss_debug_printf_t pr,
                               const vtss_debug_info_t   *const info);
#endif /* VTSS_FEATURE_IS0 */
#if defined(VTSS_FEATURE_IS1) || defined(VTSS_FEATURE_CLM)
vtss_vcap_obj_t *vtss_vcap_is1_obj_get(struct vtss_state_s *vtss_state);
void vtss_vcap_is1_init(vtss_vcap_data_t *data, vtss_is1_entry_t *entry);
void vtss_vcap_debug_print_is1(struct vtss_state_s *vtss_state,
                               const vtss_debug_printf_t pr,
                               const vtss_debug_info_t   *const info);
#endif /* VTSS_FEATURE_IS1/CLM */
#if defined(VTSS_FEATURE_CLM)
void vtss_vcap_debug_print_clm_a(struct vtss_state_s *vtss_state,
                                 const vtss_debug_printf_t pr,
                                 const vtss_debug_info_t   *const info);
void vtss_vcap_debug_print_clm_b(struct vtss_state_s *vtss_state,
                                 const vtss_debug_printf_t pr,
                                 const vtss_debug_info_t   *const info);
void vtss_vcap_debug_print_clm_c(struct vtss_state_s *vtss_state,
                                 const vtss_debug_printf_t pr,
                                 const vtss_debug_info_t   *const info);
#endif /* VTSS_FEATURE_CLM */
#if defined(VTSS_FEATURE_IS2)
void vtss_vcap_is2_init(vtss_vcap_data_t *data, vtss_is2_entry_t *entry);
void vtss_vcap_debug_print_is2(struct vtss_state_s *vtss_state,
                               const vtss_debug_printf_t pr,
                               const vtss_debug_info_t   *const info);
#endif /* VTSS_FEATURE_IS2 */
#if defined(VTSS_FEATURE_ES0)
void vtss_vcap_es0_init(vtss_vcap_data_t *data, vtss_es0_entry_t *entry);
void vtss_cmn_es0_action_get(struct vtss_state_s *vtss_state, vtss_es0_data_t *es0);
vtss_rc vtss_vcap_es0_update(struct vtss_state_s *vtss_state,
                             const vtss_port_no_t port_no, u16 flags);
void vtss_vcap_debug_print_es0(struct vtss_state_s *vtss_state,
                               const vtss_debug_printf_t pr,
                               const vtss_debug_info_t   *const info);
#endif /* VTSS_FEATURE_ES0 */
const char *vtss_vcap_key_size2txt(vtss_vcap_key_size_t key_size);
#if defined(VTSS_ARCH_SERVAL)
const char *vtss_vcap_key_type2txt(vtss_vcap_key_type_t key_type);
vtss_vcap_key_size_t vtss_vcap_key_type2size(vtss_vcap_key_type_t key_type);
#endif /* defined(VTSS_ARCH_SERVAL) */
void vtss_vcap_debug_print_range_checkers(struct vtss_state_s *vtss_state,
                                          const vtss_debug_printf_t pr,
                                          const vtss_debug_info_t   *const info);

vtss_rc vtss_cmn_ace_add(struct vtss_state_s *vtss_state,
                         const vtss_ace_id_t ace_id, const vtss_ace_t *const ace);
vtss_rc vtss_cmn_ace_del(struct vtss_state_s *vtss_state, const vtss_ace_id_t ace_id);
vtss_rc vtss_cmn_ace_counter_get(struct vtss_state_s *vtss_state,
                                 const vtss_ace_id_t ace_id, vtss_ace_counter_t *const counter);
vtss_rc vtss_cmn_ace_counter_clear(struct vtss_state_s *vtss_state, const vtss_ace_id_t ace_id);
char *vtss_acl_policy_no_txt(vtss_acl_policy_no_t policy_no, char *buf);
void vtss_vcap_debug_print_acl(struct vtss_state_s *vtss_state,
                               const vtss_debug_printf_t pr,
                               const vtss_debug_info_t   *const info);

#endif /* VTSS_FEATURE_VCAP */

#endif /* _VTSS_VCAP_STATE_H_ */
