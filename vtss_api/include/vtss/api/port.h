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

#ifndef _VTSS_PORT_TYPES_H_
#define _VTSS_PORT_TYPES_H_

#include <vtss/api/types.h>
#include <vtss/api/phy.h>

/**
 * \file
 * \brief Port Public API Header
 * \details This header file describes public port data-types
 */

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
/* Port capabilities                                                          */
/******************************************************************************/
#define PORT_CAP_NONE              0x00000000 /**< No capabilities */
#define PORT_CAP_AUTONEG           0x00000001 /**< Auto negotiation */
#define PORT_CAP_10M_HDX           0x00000002 /**< 10 Mbps, half duplex */
#define PORT_CAP_10M_FDX           0x00000004 /**< 10 Mbps, full duplex */
#define PORT_CAP_100M_HDX          0x00000008 /**< 100 Mbps, half duplex */
#define PORT_CAP_100M_FDX          0x00000010 /**< 100 Mbps, full duplex */
#define PORT_CAP_1G_FDX            0x00000020 /**< 1 Gbps, full duplex */
#define PORT_CAP_2_5G_FDX          0x00000040 /**< 2.5 Gbps, full duplex */
#define PORT_CAP_5G_FDX            0x00000080 /**< 5Gbps, full duplex */
#define PORT_CAP_10G_FDX           0x00000100 /**< 10Gbps, full duplex */
#define PORT_CAP_FLOW_CTRL         0x00001000 /**< Flow control */
#define PORT_CAP_COPPER            0x00002000 /**< Copper media */
#define PORT_CAP_FIBER             0x00004000 /**< Fiber media */
#define PORT_CAP_DUAL_COPPER       0x00008000 /**< Dual media, copper preferred */
#define PORT_CAP_DUAL_FIBER        0x00010000 /**< Dual media, fiber preferred */
#define PORT_CAP_SD_ENABLE         0x00020000 /**< Signal Detect enabled */
#define PORT_CAP_SD_HIGH           0x00040000 /**< Signal Detect active high */
#define PORT_CAP_SD_INTERNAL       0x00080000 /**< Signal Detect select internal */
#define PORT_CAP_DUAL_FIBER_100FX  0x00100000 /**< Dual media (Fiber = 100FX), fiber preferred */
#define PORT_CAP_XAUI_LANE_FLIP    0x00200000 /**< Flip the XAUI lanes */
#define PORT_CAP_VTSS_10G_PHY      0x00400000 /**< Connected to VTSS 10G PHY */
#define PORT_CAP_SFP_DETECT        0x00800000 /**< Auto detect the SFP module */
#define PORT_CAP_STACKING          0x01000000 /**< Stack port candidate */
#define PORT_CAP_DUAL_SFP_DETECT   0x02000000 /**< Auto detect the SFP module for dual media*/
#define PORT_CAP_SFP_ONLY          0x04000000 /**< SFP only port (not dual media)*/
#define PORT_CAP_DUAL_COPPER_100FX 0x08000000 /**< Dual media (Fiber = 100FX), copper preferred */


#define PORT_CAP_HDX (PORT_CAP_10M_HDX | PORT_CAP_100M_HDX) /**< Half duplex */
#define PORT_CAP_TRI_SPEED_FDX (PORT_CAP_AUTONEG | PORT_CAP_1G_FDX | PORT_CAP_100M_FDX | PORT_CAP_10M_FDX | PORT_CAP_FLOW_CTRL) /**< Tri-speed port full duplex only */
#define PORT_CAP_TRI_SPEED (PORT_CAP_TRI_SPEED_FDX | PORT_CAP_HDX) /**< Tri-speed port, both full and half duplex */
#define PORT_CAP_1G_PHY (PORT_CAP_COPPER | PORT_CAP_FIBER | PORT_CAP_DUAL_COPPER | PORT_CAP_DUAL_FIBER | PORT_CAP_DUAL_FIBER_100FX) /**< 1G PHY present */

#define PORT_CAP_TRI_SPEED_COPPER            (PORT_CAP_TRI_SPEED | PORT_CAP_COPPER)                                                                          /**< Tri-speed port copper only*/
#define PORT_CAP_TRI_SPEED_FIBER             (PORT_CAP_TRI_SPEED | PORT_CAP_FIBER)                                                                           /**< Tri-speed port fiber only*/
#define PORT_CAP_TRI_SPEED_DUAL_COPPER       (PORT_CAP_TRI_SPEED | PORT_CAP_DUAL_COPPER)                                                                     /**< Tri-speed port both fiber and copper. Copper prefered*/
#define PORT_CAP_TRI_SPEED_DUAL_FIBER        (PORT_CAP_TRI_SPEED | PORT_CAP_DUAL_FIBER)                                                                      /**< Tri-speed port both fiber and copper. Fiber prefered*/
#define PORT_CAP_TRI_SPEED_DUAL_FIBER_100FX  (PORT_CAP_TRI_SPEED | PORT_CAP_DUAL_FIBER_100FX)                                                                /**< Tri-speed port both fiber (100FX) and copper. Copper prefered*/
#define PORT_CAP_ANY_FIBER                   (PORT_CAP_FIBER | PORT_CAP_DUAL_FIBER_100FX | PORT_CAP_DUAL_FIBER | PORT_CAP_DUAL_COPPER | PORT_CAP_SFP_DETECT) /**< Any fiber mode */
#define PORT_CAP_SPEED_DUAL_ANY_FIBER_FIXED_SPEED       (PORT_CAP_DUAL_FIBER_100FX | PORT_CAP_DUAL_FIBER | PORT_CAP_DUAL_COPPER)                             /**< Any fiber mode, but auto detection not supported */
#define PORT_CAP_SPEED_DUAL_ANY_FIBER        (PORT_CAP_DUAL_COPPER | PORT_CAP_DUAL_FIBER| PORT_CAP_DUAL_FIBER_100FX | PORT_CAP_DUAL_SFP_DETECT)              /**< Any fiber mode, auto detection supported */
#define PORT_CAP_TRI_SPEED_DUAL_ANY_FIBER    (PORT_CAP_TRI_SPEED | PORT_CAP_SPEED_DUAL_ANY_FIBER)                                                            /**< Copper 5 Fiber mode, auto detection supported */
#define PORT_CAP_TRI_SPEED_DUAL_ANY_FIBER_FIXED_SFP_SPEED    (PORT_CAP_TRI_SPEED | PORT_CAP_SPEED_DUAL_ANY_FIBER_FIXED_SPEED)                                /**< Copper & Fiber mode, but SFP auto detection not supported */
#define PORT_CAP_DUAL_FIBER_1000X                 (PORT_CAP_DUAL_FIBER | PORT_CAP_DUAL_COPPER)                                                                    /**< 1000Base-X fiber mode */

#define PORT_CAP_SFP_1G   (PORT_CAP_AUTONEG | PORT_CAP_100M_FDX | PORT_CAP_1G_FDX | PORT_CAP_FLOW_CTRL | PORT_CAP_SFP_ONLY)                 /**< SFP fiber port 100FX/1G with auto negotiation and flow control */
#define PORT_CAP_SFP_2_5G (PORT_CAP_SFP_1G | PORT_CAP_2_5G_FDX)                                                                                              /**< SFP fiber port 100FX/1G/2.5G with auto negotiation and flow control */
#define PORT_CAP_SFP_SD_HIGH (PORT_CAP_SD_ENABLE | PORT_CAP_SD_HIGH | PORT_CAP_SD_INTERNAL | PORT_CAP_SFP_DETECT | PORT_CAP_SFP_ONLY)                        /**< SFP fiber port 100FX/1G/2.5G with auto negotiation and flow control, signal detect high */ 

#define PORT_CAP_2_5G_TRI_SPEED_FDX     (PORT_CAP_AUTONEG | PORT_CAP_2_5G_FDX | PORT_CAP_1G_FDX | PORT_CAP_100M_FDX | PORT_CAP_FLOW_CTRL) /**< 100M/1G/2.5G Tri-speed port full duplex only */
#define PORT_CAP_2_5G_TRI_SPEED         (PORT_CAP_2_5G_TRI_SPEED_FDX | PORT_CAP_100M_HDX)                                                 /**< 100M/1G/2.5G Tri-speed port, all full duplex and 100M half duplex */
#define PORT_CAP_2_5G_TRI_SPEED_COPPER  (PORT_CAP_2_5G_TRI_SPEED | PORT_CAP_COPPER)                                                        /**< 100M/1G/2.5G Tri-speed port copper only */

typedef u32 port_cap_t; /**< Bit-mask containing the port capabilities */

/******************************************************************************/
/* Port Counters                                                              */
/******************************************************************************/

/** \brief Counter type */
typedef u64 vtss_port_counter_t;

/** \brief RMON counter structure (RFC 2819) */
typedef struct {
    vtss_port_counter_t rx_etherStatsDropEvents;           /**< Rx drop events */
    vtss_port_counter_t rx_etherStatsOctets;               /**< Rx octets */
    vtss_port_counter_t rx_etherStatsPkts;                 /**< Rx packets */
    vtss_port_counter_t rx_etherStatsBroadcastPkts;        /**< Rx broadcasts */ 
    vtss_port_counter_t rx_etherStatsMulticastPkts;        /**< Rx multicasts */
    vtss_port_counter_t rx_etherStatsCRCAlignErrors;       /**< Rx CRC/alignment errors */
    vtss_port_counter_t rx_etherStatsUndersizePkts;        /**< Rx undersize packets */
    vtss_port_counter_t rx_etherStatsOversizePkts;         /**< Rx oversize packets */
    vtss_port_counter_t rx_etherStatsFragments;            /**< Rx fragments */
    vtss_port_counter_t rx_etherStatsJabbers;              /**< Rx jabbers */
    vtss_port_counter_t rx_etherStatsPkts64Octets;         /**< Rx 64 byte packets */
    vtss_port_counter_t rx_etherStatsPkts65to127Octets;    /**< Rx 65-127 byte packets */
    vtss_port_counter_t rx_etherStatsPkts128to255Octets;   /**< Rx 128-255 byte packets */
    vtss_port_counter_t rx_etherStatsPkts256to511Octets;   /**< Rx 256-511 byte packets */
    vtss_port_counter_t rx_etherStatsPkts512to1023Octets;  /**< Rx 512-1023 byte packet */
    vtss_port_counter_t rx_etherStatsPkts1024to1518Octets; /**< Rx 1024-1518 byte packets */
    vtss_port_counter_t rx_etherStatsPkts1519toMaxOctets;  /**< Rx 1519- byte packets */

    vtss_port_counter_t tx_etherStatsDropEvents;           /**< Tx drop events */
    vtss_port_counter_t tx_etherStatsOctets;               /**< Tx octets */
    vtss_port_counter_t tx_etherStatsPkts;                 /**< Tx packets */
    vtss_port_counter_t tx_etherStatsBroadcastPkts;        /**< Tx broadcasts */
    vtss_port_counter_t tx_etherStatsMulticastPkts;        /**< Tx multicasts */
    vtss_port_counter_t tx_etherStatsCollisions;           /**< Tx collisions */
    vtss_port_counter_t tx_etherStatsPkts64Octets;         /**< Tx 64 byte packets */
    vtss_port_counter_t tx_etherStatsPkts65to127Octets;    /**< Tx 65-127 byte packets */
    vtss_port_counter_t tx_etherStatsPkts128to255Octets;   /**< Tx 128-255 byte packets */
    vtss_port_counter_t tx_etherStatsPkts256to511Octets;   /**< Tx 256-511 byte packets */
    vtss_port_counter_t tx_etherStatsPkts512to1023Octets;  /**< Tx 512-1023 byte packet */
    vtss_port_counter_t tx_etherStatsPkts1024to1518Octets; /**< Tx 1024-1518 byte packets */
    vtss_port_counter_t tx_etherStatsPkts1519toMaxOctets;  /**< Tx 1519- byte packets */
} vtss_port_rmon_counters_t;

/** \brief Interfaces Group counter structure (RFC 2863) */
typedef struct {
    vtss_port_counter_t ifInOctets;          /**< Rx octets */
    vtss_port_counter_t ifInUcastPkts;       /**< Rx unicasts */
    vtss_port_counter_t ifInMulticastPkts;   /**< Rx multicasts */
    vtss_port_counter_t ifInBroadcastPkts;   /**< Rx broadcasts */
    vtss_port_counter_t ifInNUcastPkts;      /**< Rx non-unicasts */
    vtss_port_counter_t ifInDiscards;        /**< Rx discards */
    vtss_port_counter_t ifInErrors;          /**< Rx errors */
    
    vtss_port_counter_t ifOutOctets;         /**< Tx octets */
    vtss_port_counter_t ifOutUcastPkts;      /**< Tx unicasts */
    vtss_port_counter_t ifOutMulticastPkts;  /**< Tx multicasts */
    vtss_port_counter_t ifOutBroadcastPkts;  /**< Tx broadcasts */
    vtss_port_counter_t ifOutNUcastPkts;     /**< Tx non-unicasts */
    vtss_port_counter_t ifOutDiscards;       /**< Tx discards */
    vtss_port_counter_t ifOutErrors;         /**< Tx errors */
} vtss_port_if_group_counters_t;

/** \brief Ethernet-like Interface counter structure (RFC 3635) */
typedef struct {
#if defined(VTSS_FEATURE_PORT_CNT_ETHER_LIKE)
    vtss_port_counter_t dot3StatsAlignmentErrors;          /**< Rx alignment errors */
    vtss_port_counter_t dot3StatsFCSErrors;                /**< Rx FCS errors */
    vtss_port_counter_t dot3StatsFrameTooLongs;            /**< Rx too long */
    vtss_port_counter_t dot3StatsSymbolErrors;             /**< Rx symbol errors */
    vtss_port_counter_t dot3ControlInUnknownOpcodes;       /**< Rx unknown opcodes */
#endif /* VTSS_FEATURE_PORT_CNT_ETHER_LIKE */
    vtss_port_counter_t dot3InPauseFrames;                 /**< Rx pause */

#if defined(VTSS_FEATURE_PORT_CNT_ETHER_LIKE)
    vtss_port_counter_t dot3StatsSingleCollisionFrames;    /**< Tx single collisions */
    vtss_port_counter_t dot3StatsMultipleCollisionFrames;  /**< Tx multiple collisions */
    vtss_port_counter_t dot3StatsDeferredTransmissions;    /**< Tx deferred */
    vtss_port_counter_t dot3StatsLateCollisions;           /**< Tx late collisions */
    vtss_port_counter_t dot3StatsExcessiveCollisions;      /**< Tx excessive collisions */
    vtss_port_counter_t dot3StatsCarrierSenseErrors;       /**< Tx carrier sense errors */
#endif /* VTSS_FEATURE_PORT_CNT_ETHER_LIKE */
    vtss_port_counter_t dot3OutPauseFrames;                /**< Tx pause */
} vtss_port_ethernet_like_counters_t;

#if defined(VTSS_ARCH_CARACAL)
/** \brief EVC counters */
typedef struct
{
#if defined(VTSS_FEATURE_QOS)
    vtss_port_counter_t rx_green[VTSS_PRIOS];          /**< Rx green frames */
    vtss_port_counter_t rx_yellow[VTSS_PRIOS];         /**< Rx yellow frames */
    vtss_port_counter_t rx_red[VTSS_PRIOS];            /**< Rx red frames */
    vtss_port_counter_t rx_green_discard[VTSS_PRIOS];  /**< Rx green discarded frames */
    vtss_port_counter_t rx_yellow_discard[VTSS_PRIOS]; /**< Rx yellow discarded frames */
    vtss_port_counter_t tx_green[VTSS_PRIOS];          /**< Tx green frames */
    vtss_port_counter_t tx_yellow[VTSS_PRIOS];         /**< Tx yellow frames */
#endif
} vtss_port_evc_counters_t;
#endif /* VTSS_ARCH_CARACAL */

/** \brief Port bridge counter structure (RFC 4188) */
typedef struct
{
    vtss_port_counter_t dot1dTpPortInDiscards; /**< Rx bridge discards */
} vtss_port_bridge_counters_t;

/** \brief Port proprietary counter structure */
typedef struct
{
#if defined(VTSS_FEATURE_QOS)
    vtss_port_counter_t rx_prio[VTSS_PRIOS];        /**< Rx frames */
#endif
#if defined(VTSS_ARCH_LUTON26) || defined(VTSS_ARCH_JAGUAR_1) || defined(VTSS_ARCH_SERVAL) || defined(VTSS_ARCH_JAGUAR_2)
#if defined(VTSS_FEATURE_QOS)
    vtss_port_counter_t tx_prio[VTSS_PRIOS];        /**< Tx frames */
#endif
#endif /* VTSS_ARCH_LUTON26/JAGUAR_1/SERVAL/JAGUAR_2 */
} vtss_port_proprietary_counters_t;

/** \brief Port counter structure */
typedef struct
{
    vtss_port_rmon_counters_t          rmon;           /**< RMON counters  */
    vtss_port_if_group_counters_t      if_group;       /**< Interfaces Group counters  */
    vtss_port_ethernet_like_counters_t ethernet_like;  /**< Ethernet-like Interface counters */ 

#if defined(VTSS_FEATURE_PORT_CNT_BRIDGE)
    vtss_port_bridge_counters_t        bridge;         /**< Bridge counters */
#endif /* VTSS_FEATURE_PORT_CNT_BRIDGE */
    
    vtss_port_proprietary_counters_t   prop;           /**< Proprietary counters */

#if defined(VTSS_ARCH_CARACAL)
    vtss_port_evc_counters_t           evc;            /**< EVC counters */
#endif /* VTSS_ARCH_CARACAL */
} vtss_port_counters_t;

/******************************************************************************/
/* Port Speed                                                                 */
/******************************************************************************/

/** \brief Port speed */
typedef enum
{
    VTSS_SPEED_UNDEFINED,   /**< Undefined */
    VTSS_SPEED_10M,         /**< 10 M */
    VTSS_SPEED_100M,        /**< 100 M */
    VTSS_SPEED_1G,          /**< 1 G */
    VTSS_SPEED_2500M,       /**< 2.5G */
    VTSS_SPEED_5G,          /**< 5G or 2x2.5G */
    VTSS_SPEED_10G,         /**< 10 G */
    VTSS_SPEED_12G          /**< 12G */
} vtss_port_speed_t;

/** \brief Fiber Port speed */
typedef enum
{
    VTSS_SPEED_FIBER_NOT_SUPPORTED_OR_DISABLED = 0, /**< Fiber not supported/ Fiber port disabled */
    VTSS_SPEED_FIBER_100FX = 2,       /**< 100BASE-FX*/
    VTSS_SPEED_FIBER_1000X = 3,       /**< 1000BASE-X*/
    VTSS_SPEED_FIBER_AUTO = 4,        /**< Auto detection*/
    VTSS_SPEED_FIBER_DISABLED = 5,    /**< Obsolete - use VTSS_SPEED_FIBER_NOT_SUPPORTED_OR_DISABLED instead*/
} vtss_fiber_port_speed_t;

/******************************************************************************/
/* Port configuration                                                         */
/******************************************************************************/

/** \brief Port configuration */
typedef struct {
    BOOL                  enable;                  /**< Admin enable/disable */
    BOOL                  autoneg;                 /**< Auto negotiation */
    BOOL                  fdx;                     /**< Forced duplex mode */
    BOOL                  flow_control;            /**< Flow control (Standard 802.3x) */
#if defined(VTSS_FEATURE_QOS)
    BOOL                  pfc[VTSS_PRIOS];         /**< Priority Flow control (802.1Qbb)*/
#endif
    vtss_port_speed_t     speed;                   /**< Forced port speed */
    vtss_fiber_port_speed_t dual_media_fiber_speed;/**< Speed for dual media fiber ports*/
    unsigned int          max_length;              /**< Max frame length */
#ifdef VTSS_SW_OPTION_PHY_POWER_CONTROL
    vtss_phy_power_mode_t power_mode;              /**< PHY power mode */
#endif /* VTSS_SW_OPTION_PHY_POWER_CONTROL */
    BOOL                  exc_col_cont;            /**< Excessive collision continuation */
    u8                    adv_dis;                 /**< Auto neg advertisement disable */
    u8                    max_tags;                /**< Maximum number of tags */
    BOOL                  oper_up;                 /**< Force operational state up */
    BOOL                   frame_length_chk;       /**< True to do 802.3 frame length check for ethertypes below 0x0600*/
} port_custom_conf_t;

/******************************************************************************/
/* Port status                                                                */
/******************************************************************************/

/** \brief Port status parameter struct */
typedef struct
{
    vtss_event_t      link_down;       /**< Link down event occurred since last call */
    BOOL              link;            /**< Link is up. Remaining fields only valid if TRUE */
    vtss_port_speed_t speed;           /**< Speed */
    BOOL              fdx;             /**< Full duplex */
    BOOL              remote_fault;    /**< Remote fault signalled */
    BOOL              aneg_complete;   /**< Autoneg completed (for clause_37 and Cisco aneg) */
    BOOL              unidirectional_ability; /**<TRUE: PHY able to transmit from media independent interface regardless of whether the PHY has 
                                                 determined that a valid link has been established.FALSE: PHY able to transmit from media 
                                                 independent interface only when the PHY has determined that a valid link has been established. 
                                                 Note This bit is only applicable to 100BASE-FX and 1000BASE-X fiber media modes.*/
    vtss_aneg_t aneg;                  /**< Auto negotiation result */
    BOOL mdi_cross;                    /**< Indication of if Auto-MDIX crossover is performed */
    BOOL fiber;                        /**< Indication of if the link is a fiber link, TRUE if link is a fiber link. FALSE if link is cu link or No Media */
    BOOL copper;                       /**< Indication of if the link is a copper link, TRUE if link is a copper link. FALSE if link is fiber link or No Media */
} vtss_port_status_t;

#ifdef __cplusplus
}
#endif

#endif /* #define _VTSS_PORT_TYPES_H_ */
