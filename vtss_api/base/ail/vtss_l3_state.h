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

#ifndef _VTSS_L3_STATE_H_
#define _VTSS_L3_STATE_H_

#if defined(VTSS_FEATURE_LAYER3)

typedef enum {
    VTSS_L3_LPM_MATCH_BOTH,
    VTSS_L3_LPM_MATCH_LPM,
    VTSS_L3_LPM_MATCH_SECURITY,
} vtss_l3_lpm_match_type;

typedef enum {
    VTSS_L3_LPM_ENTRY_IPV4_UC,
    VTSS_L3_LPM_ENTRY_IPV6_UC,
    VTSS_L3_LPM_ENTRY_IPV4_MC,
} vtss_l3_lpm_entry_type;

typedef struct {
    BOOL                   valid;  /* current entry is valid  (book keeping only - not in hardware) */
    vtss_l3_lpm_match_type match;  /* lpm/security/both */
    vtss_l3_lpm_entry_type  type;  /* ipv4_uc/ipv6_uc/ipv4_mc */
    u32                     data;  /* ip address to match */
    u32                     mask;  /* mask to match */
    u32                     dest;  /* destination of the route (book keeping only - not in hardware) */
    BOOL                    ecmp;  /* current entry represents ecmp */
    BOOL               ptr_valid;  /* pointer valid (active in hardware) */
    u32                      ptr;  /* pointer to ARP/ICMP entry */
    u32                  vcap_id;  /* JR2: VCAP ID */
} vtss_l3_lpm_entry_t;

typedef struct {
    vtss_l3_lpm_entry_t e[VTSS_LPM_CNT];
} vtss_l3_lpm_table_t;

typedef struct {
    BOOL        valid;
    vtss_ipv4_t ip;
    u32         ptr;
} vtss_l3_arp_ipv4_entry_t;

typedef struct {
    BOOL        valid;
    vtss_ipv6_t ip;
    u32         ptr;
} vtss_l3_ndp_ipv6_entry_t;

typedef struct {
    BOOL              valid;       /* current entry is valid */
    vtss_l3_rleg_id_t egress_rleg; /* router leg used for transmitting frames */
    BOOL              secure_vmid; /* ? */
    BOOL              secure_mac;  /* ? */
    BOOL              arp_enable;  /* arp enable */
    vtss_mac_t        dmac;        /* destination mac address */
} vtss_l3_arp_hw_entry_t;

typedef struct {
    vtss_l3_arp_hw_entry_t   hw[VTSS_ARP_CNT];
    vtss_l3_arp_ipv4_entry_t ipv4[VTSS_ARP_IPV4_RELATIONS];
    vtss_l3_ndp_ipv6_entry_t ipv6[VTSS_ARP_IPV6_RELATIONS];
} vtss_l3_neighbour_cache_t;

typedef struct {
    vtss_l3_counters_t interface_shadow_counter[VTSS_RLEG_CNT];
    vtss_l3_counters_t interface_counter[VTSS_RLEG_CNT];
    u32                rleg; /* Next router leg to poll */
} vtss_l3_statistics_t;

typedef struct {
    /* CIL function pointers */
    vtss_rc (* rleg_counters_get)(struct vtss_state_s *vtss_state,
                                  const vtss_l3_rleg_id_t);
    vtss_rc (* rleg_counters_reset)(struct vtss_state_s *vtss_state);
    vtss_rc (* common_set)(struct vtss_state_s *vtss_state,
                              const vtss_l3_common_conf_t * const);
    vtss_rc (* rleg_set)(struct vtss_state_s *vtss_state,
                            const vtss_l3_rleg_id_t, const vtss_l3_rleg_conf_t * const);
    vtss_rc (* vlan_set)(struct vtss_state_s *vtss_state,
                            const vtss_l3_rleg_id_t, const vtss_vid_t, const BOOL);
    vtss_rc (* arp_set)(struct vtss_state_s *vtss_state,
                           const u32, const vtss_mac_t * const, const vtss_vid_t);
    vtss_rc (* arp_clear)(struct vtss_state_s *vtss_state, const u32);
    vtss_rc (* ipv4_uc_set)(struct vtss_state_s *vtss_state,
                               const u32, const u32, const BOOL, const u32, const u32);
    vtss_rc (* ipv6_uc_set)(struct vtss_state_s *vtss_state,
                               const u32, const u32, const BOOL, const u32, const u32, const u32, const u32, const u32, const u32, const u32, const u32);
    vtss_rc (* ipv4_uc_clear)(struct vtss_state_s *vtss_state, const u32);
    vtss_rc (* ipv4_mc_clear)(struct vtss_state_s *vtss_state, const u32);
    vtss_rc (* ipv6_uc_clear)(struct vtss_state_s *vtss_state, const u32);
    vtss_rc (* lpm_move)(struct vtss_state_s *vtss_state,
                            const u32, const u32, const u32, const BOOL);
    vtss_rc (* debug_sticky_clear)(struct vtss_state_s *vtss_state);

    /* Configuration/state */
    u32                        checksum;
    BOOL                       initialized;
    vtss_l3_common_conf_t      common;
    vtss_l3_rleg_conf_t        rleg_conf[VTSS_RLEG_CNT];
    vtss_l3_lpm_table_t        lpm;
    vtss_l3_neighbour_cache_t  neighbour_cache;
    vtss_l3_statistics_t       statistics;
} vtss_l3_state_t;

void vtss_l3_integrity_update(struct vtss_state_s *vtss_state);
void vtss_l3_integrity_check(const struct vtss_state_s *vtss_state, const char *file, unsigned line);

#endif /* VTSS_FEATURE_LAYER3 */

#endif /* _VTSS_L3_STATE_H_ */
