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
 * \brief L3 routing API
 * \details This header file describes L3 IPv4/IPv6 hardware assisted routing
 * functions.
 */

#ifndef _VTSS_L3_API_H_
#define _VTSS_L3_API_H_

#include <vtss/api/types.h>

#if defined(VTSS_FEATURE_LAYER3)

/** \page layer3 Layer 3
 *
 * The Layer 3 functionality is build around some global configuration and three
 * tables: router-legs (RLEG), longest-prefix-match (LPM) and neighbour-table
 * (ARP). Following is the purpose of these configurations entities.
 *
 *
 * Configuration entities
 * ----------------------
 *
 * Global configuration: Required to enable routing and to configure the MAC
 * addressing schema to use for the router legs.
 *
 * Router legs: The purpose of L3 routing is to forward IP frames from one L3
 * interface to another. The router legs represents these L3 interfaces. A
 * router leg is associated to a L2 VLAN, it has a MAC-address and a set of
 * other attributes to enable/disable IPv4/IPv6 routing and VRRP settings. If a
 * IP frame is received on a VLAN which has an associated router leg, and the
 * destination MAC of the frame matches the MAC address of the router leg, then
 * the frame is candidate for being routed.
 *
 * NOTE: The router leg table should be synchronized with the interface table in
 * the operating systems IP stack. When a L3 interface is added in the operating
 * system, a corresponding router leg should be created. When an IP address is
 * assigned to a given L3 interface in the IP stack, then the corresponding
 * interface route should be installed in the LPM table.
 *
 * Longest prefix match: This is where the actual routing takes place, if a IP
 * frame qualifies for routing then the destination IP address is matched
 * against the LPM table. The best match (the longest prefix match) in the LPM
 * table is used to route the frame. If no match is found in the LPM table, then
 * the frame is forwarded to the CPU. The LPM table should include network
 * routes, host routes and interface routes. An interface route is a route
 * representing the configured IP range of the interface, and with the
 * destination configured to zero.
 *
 * NOTE: An LPM entry can only be routed in HW if the destination of the route
 * is installed in the neighbour table. If this is not the case the frame will
 * be forwarded to the CPU, which should perform the ARP/NDP resolution route
 * the packet in SW and install the neighbour in HW for future use.
 *
 * Neighbour table: This table is used to map IP address to (MAC addresses,
 * VLAN). When a packet is being routed in the LPM table, the output is a
 * "next-hop" IP address, this "next-hop" IP address must be translated to a
 * MAC-address and a VLAN before it can be L2 forwarded. If a LPM entry does not
 * have a corresponding neighbour entry, then the packet will be forwarded to
 * the CPU.
 *
 * The neighbour table should be synchronized with the ARP/NDP table of the
 * operating systems IP stack.
 *
 *
 * Mode of operation
 * -----------------
 *
 * When a frame is being routed and the best match in the LPM table is a
 * interface route, then the packet will be forwarded to the CPU. The CPU should
 * now start the ARP/NDP protocols in order to figure out what the MAC address
 * is of the destination host. The address resolution has completed the frame
 * can be routed. In order to enable hardware routing for directly connected
 * routes, the derived destination host must be installed as a host route in the
 * LPM table, and an associated neighbour entry should be installed.
 *
 * When a new network route is installed, the MAC address of the next hop route
 * may not be known in advance. If this is the case the first hit on that route
 * will also cause the frame to be forwarded to the CPU, and the CPU may
 * complete the neighbour discovery and install a neighbour entry. After the
 * neighbour entry has been installed the routing will be performed in HW.
 *
 */

#if defined(VTSS_ARCH_JAGUAR_1)

#define VTSS_JR1_LPM_CNT  (1024u)            /**< JR1 length of LPM table */
#define VTSS_LPM_CNT      VTSS_JR1_LPM_CNT   /**< Length of LPM table */

#define VTSS_JR1_ARP_CNT  (1024u)            /**< JR1 length of ARP table */
#define VTSS_ARP_CNT      VTSS_JR1_ARP_CNT   /**< Length of ARP table */

#define VTSS_JR1_RLEG_CNT  (128u)            /**< JR1 length of RLEG table */
#define VTSS_RLEG_CNT      VTSS_JR1_RLEG_CNT /**< Length of RLEG table */

#endif /* VTSS_ARCH_JAGUAR_1 */

#if defined(VTSS_ARCH_JAGUAR_2)
#if defined(VTSS_ARCH_SERVAL_T)
#define VTSS_RLEG_CNT 32   /**< Length of RLEG table */
#define VTSS_LPM_CNT  512  /**< Length of LPM table */
#define VTSS_ARP_CNT  256  /**< Length of ARP table */
#else
#define VTSS_RLEG_CNT 128  /**< Length of RLEG table */
#define VTSS_LPM_CNT  4096 /**< Length of LPM table */
#define VTSS_ARP_CNT  2048 /**< Length of ARP table */
#endif /* VTSS_ARCH_SERVAL_T */
#endif /* VTSS_ARCH_JAGUAR_2 */

/** Length of IPv4 ARP table */
#define VTSS_ARP_IPV4_RELATIONS VTSS_ARP_CNT

/** Length of IPv6 NDP table */
#define VTSS_ARP_IPV6_RELATIONS VTSS_ARP_CNT

#ifdef __cplusplus
extern "C" {
#endif

/** \brief Router leg ID */
typedef u32 vtss_l3_rleg_id_t;

/** \brief Virtual router identifier */
typedef u8  vtss_l3_vrid_t;

/** \brief MAC addressing mode for routing legs */
typedef enum
{
    /** The addressing mode has still not been configured */
    VTSS_ROUTING_RLEG_MAC_MODE_INVALID = 0,

    /** One common MAC address is used for all legs */
    VTSS_ROUTING_RLEG_MAC_MODE_SINGLE = 1,
} vtss_l3_rleg_common_mode_t;

/** \brief Common configurations for all routing legs */
typedef struct
{
    /** Common rleg-mode for all routing legs. */
    vtss_l3_rleg_common_mode_t rleg_mode;

    /** Base mac address used to derive addresses for all routing legs. */
    vtss_mac_t                 base_address;

    /** Globally enable/disable routing. */
    BOOL                       routing_enable;
} vtss_l3_common_conf_t;

/** \brief Router leg control structure */
typedef struct
{
    /** Enable IPv4 unicast routing */
    BOOL ipv4_unicast_enable;

    /** Enable IPv6 unicast routing */
    BOOL ipv6_unicast_enable;

    /** Enable IPv4 icmp redirect */
    BOOL ipv4_icmp_redirect_enable;

    /** Enable IPv6 icmp redirect */
    BOOL ipv6_icmp_redirect_enable;

    /** Vlan for which the router leg is instantiated */
    vtss_vid_t          vlan;

    /** Enable/disable VRRP for a given router leg.
     *
     * The hardware has support for enabling 0-2 VRID's for a given router leg.
     * This is activated by configured vrid0_enable and vrid1_enable. The actual
     * VRID the route is assigned to is configured in vrid0/vrid1.
     *
     * JR1-NOTE: When enabling vrrp for JR1-revb, the hardware will consider all
     * destination mac addresses matching 00-00-5E-00-XX-{VRID} as VRRP address.
     * For versions earlier than rev-B VRRP is only supported for IPv4
     * (00-00-5E-00-01-{VRID}).
     */
    BOOL                vrid0_enable;

    /** The VRID value assigned to this router leg. */
    vtss_l3_vrid_t      vrid0;

    /** Enable/disable vrid1 for this router leg. */
    BOOL                vrid1_enable;

    /** The VRID value assigned to this router leg. */
    vtss_l3_vrid_t      vrid1;
} vtss_l3_rleg_conf_t;

/** \brief Neighbour type */
typedef enum
{
    /** Invalid entry. */
    VTSS_L3_NEIGHBOUR_TYPE_INVALID = 0,

    /** IPv4 Neighbour entry (ARP). */
    VTSS_L3_NEIGHBOUR_TYPE_ARP = 1,

    /** IPv6 Neighbour entry (NDP). */
    VTSS_L3_NEIGHBOUR_TYPE_NDP = 2,
} vtss_l3_neighbour_type_t;

/** \brief Neighbour entry */
typedef struct
{
    /** MAC address of destination */
    vtss_mac_t         dmac;

    /** VLAN of destination */
    vtss_vid_t         vlan;

    /** IP address of destination */
    vtss_ip_addr_t     dip;
} vtss_l3_neighbour_t;

/**
 * \brief Flush all L3 configurations
 *
 * \param inst [IN]     Target instance reference.
 *
 * \return Return code.
 **/
vtss_rc vtss_l3_flush(const vtss_inst_t inst);


/**
 * \brief Get common router configuration.
 *
 * \param inst [IN]     Target instance reference.
 * \param conf [OUT]    Common routing configurations.
 *
 * \return Return code.
 **/
vtss_rc vtss_l3_common_get(const vtss_inst_t      inst,
                           vtss_l3_common_conf_t *const conf);

/**
 * \brief Set common router configuration.
 *
 * \param inst [IN]     Target instance reference.
 * \param conf [OUT]    Common routing configurations.
 *
 * \return Return code.
 **/
vtss_rc vtss_l3_common_set(const vtss_inst_t            inst,
                           const vtss_l3_common_conf_t *const conf);

/**
 * \brief Get all configured router leg
 *
 * \param inst [IN]     Target instance reference.
 * \param cnt [OUT]     Amount of entries copied to output buffer
 * \param buf [OUT]     Output buffer
 *
 * \return Return code.
 **/
vtss_rc vtss_l3_rleg_get(const vtss_inst_t     inst,
                         u32                  *cnt,
                         vtss_l3_rleg_conf_t   buf[VTSS_RLEG_CNT]);

/**
 * \brief Get a specific configured router leg
 *
 * \param inst [IN]     Target instance reference.
 * \param vid  [IN]     VLAN ID of the router leg to get
 * \param conf [OUT]    Output buffer where the configuration is written
 *
 * \return Return code.
 **/
vtss_rc vtss_l3_rleg_get_specific(const vtss_inst_t     inst,
                                  vtss_vid_t            vid,
                                  vtss_l3_rleg_conf_t  *conf);

/**
 * \brief Add a router leg on the given VLAN
 *
 * \param inst [IN] Target instance reference.
 * \param conf [IN] Routing leg configuration.
 *
 * \return Return code.
 **/
vtss_rc vtss_l3_rleg_add(const vtss_inst_t          inst,
                         const vtss_l3_rleg_conf_t *const conf);

/**
 * \brief Update an existing router leg.
 *
 * Will fail if an existing router leg with the same VLAN does not exists.
 *
 * \param inst [IN] Target instance reference.
 * \param conf [IN] Routing leg configuration.
 *
 * \return Return code.
 **/
vtss_rc vtss_l3_rleg_update(const vtss_inst_t          inst,
                            const vtss_l3_rleg_conf_t *const conf);

/**
 * \brief Delete a router leg associated with VLAN
 *
 * \param inst [IN]     Target instance reference.
 * \param vlan [IN]     VLAN to delete router leg from
 *
 * \return Return code.
 **/
vtss_rc vtss_l3_rleg_del(const vtss_inst_t         inst,
                         const vtss_vid_t          vlan);

/**
 * \brief Get all configured routes
 *
 * \param inst [IN]     Target instance reference.
 * \param cnt [OUT]     Amount of entries copied to output buffer
 * \param buf [OUT]     Output buffer
 *
 * \return Return code.
 **/
vtss_rc vtss_l3_route_get(const vtss_inst_t     inst,
                          u32                  *cnt,
                          vtss_routing_entry_t  buf[VTSS_LPM_CNT]);

/**
 * \brief Add an entry to the routing table
 *
 * \param inst [IN]     Target instance reference.
 * \param entry [IN]    Route to add
 *
 * \return Return code.
 **/
vtss_rc vtss_l3_route_add(const vtss_inst_t             inst,
                          const vtss_routing_entry_t    * const entry);

/**
 * \brief Delete an entry from the routing table
 *
 * \param inst [IN]     Target instance reference.
 * \param entry [IN]    Entry to delete.
 *
 * \return Return code.
 **/
vtss_rc vtss_l3_route_del(const vtss_inst_t             inst,
                          const vtss_routing_entry_t    *const entry);

/**
 * \brief Get all configured neighbours
 *
 * \param inst [IN]     Target instance reference.
 * \param cnt [OUT]     Amount of entries copied to output buffer
 * \param buf [OUT]     Output buffer
 *
 * \return Return code.
 **/
vtss_rc vtss_l3_neighbour_get(const vtss_inst_t    inst,
                              u32                 *cnt,
                              vtss_l3_neighbour_t  buf[VTSS_ARP_CNT]);

/**
 * \brief Add a new entry to the neighbour cache.
 *
 * \param inst [IN]     Target instance reference.
 * \param entry [IN]    Entry to add.
 *
 * \return Return code.
 **/
vtss_rc vtss_l3_neighbour_add(const vtss_inst_t         inst,
                              const vtss_l3_neighbour_t * const entry);

/**
 * \brief Delete an entry from the neighbour  cache.
 *
 * \param inst [IN]     Target instance reference.
 * \param entry [IN]    Entry to delete.
 *
 * \return Return code.
 **/
vtss_rc vtss_l3_neighbour_del(const vtss_inst_t         inst,
                              const vtss_l3_neighbour_t * const entry);

/**
 * \brief Reset all routing leg statistics counters
 *
 * \param inst [IN]     Target instance reference.
 *
 * \return Return code.
 **/
vtss_rc vtss_l3_counters_reset(const vtss_inst_t inst);

/**
 * \brief Get routing system counters
 *
 * \param inst [IN]      Target instance reference.
 * \param counters [OUT] Counters
 *
 * \return Return code.
 **/
vtss_rc vtss_l3_counters_system_get(const vtss_inst_t  inst,
                                    vtss_l3_counters_t * const counters);

/**
 * \brief Get routing legs counters
 *
 * \param inst [IN]      Target instance reference.
 * \param vlan [IN]      Routing leg
 * \param counters [OUT] Counters
 *
 * \return Return code.
 **/
vtss_rc vtss_l3_counters_rleg_get(const vtss_inst_t       inst,
                                  const vtss_vid_t        vlan,
                                  vtss_l3_counters_t      * const counters);

/**
 * \brief Clear routing legs counters
 *
 * \param inst [IN]      Target instance reference.
 * \param vlan [IN]      Routing leg
 *
 * \return Return code.
 **/
vtss_rc vtss_l3_counters_rleg_clear(const vtss_inst_t       inst,
                                    const vtss_vid_t        vlan);


#ifdef __cplusplus
}
#endif
#endif /* VTSS_FEATURE_LAYER3 */
#endif /* _VTSS_L3_API_H_ */
