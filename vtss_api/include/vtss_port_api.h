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
 * \file vtss_port_api.h
 * \brief Port API
 */

#ifndef _VTSS_PORT_API_H_
#define _VTSS_PORT_API_H_

#include <vtss/api/port.h>

#ifdef __cplusplus
extern "C" {
#endif

/** \brief MII management controller */
typedef enum
{
#if defined(VTSS_ARCH_LUTON26) || defined(VTSS_ARCH_JAGUAR_1) || defined(VTSS_ARCH_SERVAL) || defined(VTSS_ARCH_JAGUAR_2)
    VTSS_MIIM_CONTROLLER_0    = 0,  /**< MIIM controller 0 */
    VTSS_MIIM_CONTROLLER_1    = 1,  /**< MIIM controller 1 */
#endif /* VTSS_ARCH_LUTON26/JAGUAR_1/SERVAL/JAGUAR2 */
#if defined(VTSS_ARCH_JAGUAR_2)
    VTSS_MIIM_CONTROLLER_2    = 2,  /**< MIIM controller 2 */
#endif /* VTSS_ARCH_JAGUAR_2 */
#if defined(VTSS_ARCH_DAYTONA)
    VTSS_MIIM_CONTROLLER_0    = 0,  /**< MIIM controller 0 */
#endif /* VTSS_ARCH_DAYTONA */
    VTSS_MIIM_CONTROLLERS,          /**< Number of MIIM controllers */
    VTSS_MIIM_CONTROLLER_NONE = -1  /**< Unassigned MIIM controller */
} vtss_miim_controller_t;

#if defined(VTSS_FEATURE_PORT_CONTROL)

/** \brief The internal bandwidth allocated for the port */
typedef enum
{
    VTSS_BW_DEFAULT,     /**< Default to max port speed */
    VTSS_BW_1G,          /**< Max 1G */
    VTSS_BW_2500M,       /**< Max 2.5G */
    VTSS_BW_UNDEFINED,   /**< Undefined */
} vtss_internal_bw_t;

#define CHIP_PORT_UNUSED -1 /**< Signifies an unused chip port */

/** \brief Port map structure */
typedef struct
{
    i32                    chip_port;        /**< Set to -1 if not used */
    vtss_chip_no_t         chip_no;          /**< Chip number, multi-chip targets */

#if defined(VTSS_ARCH_JAGUAR_2)
    vtss_internal_bw_t     max_bw;           /**< Max internal bandwidth reserved for the port */
#endif /* VTSS_ARCH_JAGUAR_2 */
    vtss_miim_controller_t miim_controller;  /**< MII management controller */
    u8                     miim_addr;        /**< PHY address, ignored for VTSS_MIIM_CONTROLLER_NONE */
    vtss_chip_no_t         miim_chip_no;     /**< MII management chip number, multi-chip targets */
} vtss_port_map_t;

/**
 * \brief Set port map.
 *
 * \param inst [IN]      Target instance reference.
 * \param port_map [IN]  Port map array.
 *
 * \return Return code.
 **/
 vtss_rc vtss_port_map_set(const vtss_inst_t      inst,
			   const vtss_port_map_t port_map[VTSS_PORT_ARRAY_SIZE]);

/**
 * \brief Get port map.
 *
 * \param inst [IN]       Target instance reference.
 * \param port_map [OUT]  Port map.
 *
 * \return Return code.
 **/
vtss_rc vtss_port_map_get(const vtss_inst_t  inst,
                          vtss_port_map_t    port_map[VTSS_PORT_ARRAY_SIZE]);

#if defined(VTSS_FEATURE_CLAUSE_37)
/**
 * Advertisement Word (Refer to IEEE 802.3 Clause 37):
 *  MSB                                                                         LSB
 *  D15  D14  D13  D12  D11  D10   D9   D8   D7   D6   D5   D4   D3   D2   D1   D0 
 * +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
 * | NP | Ack| RF2| RF1|rsvd|rsvd|rsvd| PS2| PS1| HD | FD |rsvd|rsvd|rsvd|rsvd|rsvd|
 * +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
 **/

/** \brief Auto-negotiation remote fault type */
typedef enum                      /* RF2      RF1 */
{
    VTSS_PORT_CLAUSE_37_RF_LINK_OK       = ((0<<1) | (0<<0)),   /**< Link OK */
    VTSS_PORT_CLAUSE_37_RF_OFFLINE       = ((1<<1) | (0<<0)),   /**< Off line */
    VTSS_PORT_CLAUSE_37_RF_LINK_FAILURE  = ((0<<1) | (1<<0)),   /**< Link failure */
    VTSS_PORT_CLAUSE_37_RF_AUTONEG_ERROR = ((1<<1) | (1<<0))    /**< Autoneg error */
} vtss_port_clause_37_remote_fault_t;

/** \brief Advertisement control data for Clause 37 aneg */
typedef struct
{
    BOOL                               fdx;               /**< (FD) */
    BOOL                               hdx;               /**< (HD) */
    BOOL                               symmetric_pause;   /**< (PS1) */
    BOOL                               asymmetric_pause;  /**< (PS2) */
    vtss_port_clause_37_remote_fault_t remote_fault;      /**< (RF1) + (RF2) */
    BOOL                               acknowledge;       /**< (Ack) */
    BOOL                               next_page;         /**< (NP) */
} vtss_port_clause_37_adv_t;

/** \brief Advertisement control data for SGMII aneg */
typedef struct
{
    BOOL                               link;              /**< LP link status               */
    BOOL                               fdx;               /**< FD                           */
    BOOL                               hdx;               /**< HD                           */
    BOOL                               speed_10M;         /**< speed 10 advertised          */
    BOOL                               speed_100M;        /**< speed 100 advertised         */
    BOOL                               speed_1G;          /**< speed 1G advertised          */
    BOOL                               aneg_complete;     /**< Aneg process completed       */
} vtss_port_sgmii_aneg_t;


/** \brief Auto-negotiation control parameter struct */
typedef struct
{
    BOOL                      enable;           /**< Enable of Autoneg */
    vtss_port_clause_37_adv_t advertisement;    /**< Clause 37 Advertisement control data */
} vtss_port_clause_37_control_t;

/**
 * \brief Get clause 37 auto-negotiation Control word.
 *
 * \param inst [IN]      Target instance reference.
 * \param port_no [IN]   Port number.
 * \param control [OUT]  Control structure.
 *
 * \return Return code.
 **/
vtss_rc vtss_port_clause_37_control_get(const vtss_inst_t              inst,
                                        const vtss_port_no_t           port_no,
                                        vtss_port_clause_37_control_t  *const control);



/**
 * \brief Set clause 37 auto-negotiation Control word.
 *
 * \param inst [IN]     Target instance reference.
 * \param port_no [IN]  Port number.
 * \param control [IN]  Control structure.
 *
 * \return Return code.
 **/
vtss_rc vtss_port_clause_37_control_set(const vtss_inst_t                    inst,
                                        const vtss_port_no_t                 port_no,
                                        const vtss_port_clause_37_control_t  *const control);
#endif /* VTSS_FEATURE_CLAUSE_37 */


/** \brief Flow control setup */
typedef struct
{
    BOOL       obey;            /**< TRUE if 802.3x PAUSE frames should be obeyed */
#if defined(VTSS_ARCH_DAYTONA)  
    BOOL       rx_drop;         /**< TRUE if 802.3x PAUSE frames needs to be dropped/obey*/
#endif /* VTSS_ARCH_DAYTONA */    
    BOOL       generate;        /**< TRUE if 802.3x PAUSE frames should generated */
    vtss_mac_t smac;            /**< Port MAC address used as SMAC in PAUSE frames */
#if defined(VTSS_FEATURE_QOS)
    BOOL       pfc[VTSS_PRIOS]; /**< TRUE if 802.1Qbb Priority Flow Control should be generated and obeyed.
                                     Cannot be enabled together with 802.3x Flowcontrol */
#endif
} vtss_port_flow_control_conf_t;

#define VTSS_FRAME_GAP_DEFAULT 0 /**< Default frame gap used */

/** \brief Inter frame gap structure */
typedef struct
{
    u32 hdx_gap_1;      /**< Half duplex: First part of Rx to Tx gap */
    u32 hdx_gap_2;      /**< Half duplex: Second part of Rx to Tx gap */
    u32 fdx_gap;        /**< Full duplex: Tx to Tx gap */
} vtss_port_frame_gaps_t;

/* A selection of max frame lengths */
#define VTSS_MAX_FRAME_LENGTH_STANDARD 1518  /**< IEEE 802.3 standard */
#define VTSS_MAX_FRAME_LENGTH_MAX      10240

#if defined(VTSS_ARCH_LUTON26)
#undef VTSS_MAX_FRAME_LENGTH_MAX
#define VTSS_MAX_FRAME_LENGTH_MAX      9600  /**< Maximum frame length supported */
#endif /* VTSS_ARCH_LUTON26 */

#if defined(VTSS_ARCH_SERVAL)
#undef VTSS_MAX_FRAME_LENGTH_MAX
#define VTSS_MAX_FRAME_LENGTH_MAX      10240 /**< Maximum frame length supported */
#endif /* VTSS_ARCH_SERVAL */

#if defined(VTSS_CHIP_LYNX_1) || defined(VTSS_CHIP_JAGUAR_1)
#undef VTSS_MAX_FRAME_LENGTH_MAX
#define VTSS_MAX_FRAME_LENGTH_MAX      4776  /**< Maximum frame length supported for CEService */
#elif defined(VTSS_ARCH_JAGUAR_1)
#undef VTSS_MAX_FRAME_LENGTH_MAX
#define VTSS_MAX_FRAME_LENGTH_MAX      10056 /**< Maximum frame length supported for Enterprise */
#endif

#if defined(VTSS_ARCH_JAGUAR_2)
#undef VTSS_MAX_FRAME_LENGTH_MAX
#define VTSS_MAX_FRAME_LENGTH_MAX      10240 /**< Maximum frame length supported */
#endif /* VTSS_ARCH_JAGUAR_2 */

#if defined(VTSS_ARCH_DAYTONA)
#undef VTSS_MAX_FRAME_LENGTH_MAX
#define VTSS_MAX_FRAME_LENGTH_MAX      10240 /**< Maximum frame length supported */
#endif /* VTSS_ARCH_DAYTONA */

/** \brief VLAN awareness for frame length check */
typedef enum
{
    VTSS_PORT_MAX_TAGS_NONE,  /**< No extra tags allowed */
    VTSS_PORT_MAX_TAGS_ONE,   /**< Single tag allowed */
    VTSS_PORT_MAX_TAGS_TWO    /**< Single and double tag allowed */
} vtss_port_max_tags_t;

/** \brief Port loop back configuration */
typedef enum
{
    VTSS_PORT_LOOP_DISABLE,   /**< No port loop */
    VTSS_PORT_LOOP_PCS_HOST,  /**< PCS host port loop */
} vtss_port_loop_t;

/** \brief SFI Serdes configuration */
typedef struct
{
    BOOL sfp_dac;             /**< Optical (0) or Cu cable (SFP+ DAC) (1)  */
} vtss_port_serdes_conf_t;


/** \brief Port configuration structure */
typedef struct
{
    vtss_port_interface_t         if_type;           /**< Interface type */
    BOOL                          sd_enable;         /**< Signal detect enable */
    BOOL                          sd_active_high;    /**< External signal detect polarity */
    BOOL                          sd_internal;       /**< Internal signal detect selection */
    vtss_port_frame_gaps_t        frame_gaps;        /**< Inter frame gaps */
    BOOL                          power_down;        /**< Disable and power down the port */
    vtss_port_speed_t             speed;             /**< Port speed */
    BOOL                          fdx;               /**< Full duplex mode */
    vtss_port_flow_control_conf_t flow_control;      /**< Flow control setup */
    u32                           max_frame_length;  /**< Maximum frame length */
    BOOL                          frame_length_chk;  /**< Enforce 802.3 frame length check (from ethertype field) */
    vtss_port_max_tags_t          max_tags;          /**< VLAN awareness for length check */
    BOOL                          exc_col_cont;      /**< Excessive collision continuation */
    BOOL                          xaui_rx_lane_flip; /**< Xaui Rx lane flip */
    BOOL                          xaui_tx_lane_flip; /**< Xaui Tx lane flip */
    vtss_port_loop_t              loop;              /**< Enable/disable of port loop back */
    vtss_port_serdes_conf_t       serdes;            /**< Serdes settings (for SFI interface) */
} vtss_port_conf_t;

/**
 * \brief Set port configuration.
 *  Note: If if_type in the vtss_port_conf_t/vtss_port_interface_t definition is set to VTSS_PORT_INTERFACE_QSGMII, the ports are mapped together in groups of four. If one of the four ports is used, all four ports in the group must always be configured, but the four ports doesn't need to configured with the same configuration. 
 * This is needed in order to achieve correct comma alignment at the QSGMII interface. Which ports that are mapped together can be found in the chip data-sheet.
 *
 * \param inst [IN]     Target instance reference.
 * \param port_no [IN]  Port number.
 * \param conf [IN]     Port setup structure.
 *
 * \return Return code.
 **/
vtss_rc vtss_port_conf_set(const vtss_inst_t       inst,
                           const vtss_port_no_t    port_no,
                           const vtss_port_conf_t  *const conf);



/**
 * \brief Get port setup.
 *
 * \param inst [IN]     Target instance reference.
 * \param port_no [IN]  Port number.
 * \param conf [OUT]    Port configuration.
 *
 * \return Return code.
 **/
vtss_rc vtss_port_conf_get(const vtss_inst_t     inst,
                           const vtss_port_no_t  port_no,
                           vtss_port_conf_t      *const conf);


/**
 * \brief Get port status.
 *
 * \param inst [IN]     Target instance reference.
 * \param port_no [IN]  Port number.
 * \param status [OUT]  Status structure.
 *
 * \return Return code.
 **/
vtss_rc vtss_port_status_get(const vtss_inst_t     inst,
                             const vtss_port_no_t  port_no,
                             vtss_port_status_t    *const status);



/**
 * \brief Update counters for port.
 *
 * \param inst [IN]     Target instance reference.
 * \param port_no [IN]  Port number.
 *
 * \return Return code.
 **/
vtss_rc vtss_port_counters_update(const vtss_inst_t     inst,
                                  const vtss_port_no_t  port_no);



/**
 * \brief Clear counters for port.
 *
 * \param inst [IN]     Target instance reference.
 * \param port_no [IN]  Port/aggregation number.
 *
 * \return Return code.
 **/
vtss_rc vtss_port_counters_clear(const vtss_inst_t     inst,
                                 const vtss_port_no_t  port_no);


#if defined(VTSS_FEATURE_QOS)
#endif /* VTSS_FEATURE_QOS */
/**
 * \brief Get counters for port.
 *
 * \param inst [IN]       Target instance reference.
 * \param port_no [IN]    Port/aggregation number.
 * \param counters [OUT]  Counter structure.
 *
 * \return Return code.
 **/
vtss_rc vtss_port_counters_get(const vtss_inst_t     inst,
                               const vtss_port_no_t  port_no,
                               vtss_port_counters_t  *const counters);


/** \brief Basic counters structure */
typedef struct
{
    u32 rx_frames; /**< Rx frames */
    u32 tx_frames; /**< Tx frames */
} vtss_basic_counters_t;

/**
 * \brief Get basic counters for port.
 *
 * \param inst [IN]       Target instance reference.
 * \param port_no [IN]    Port/aggregation number.
 * \param counters [OUT]  Counter structure.
 *
 * \return Return code.
 **/
vtss_rc vtss_port_basic_counters_get(const vtss_inst_t     inst,
                                     const vtss_port_no_t  port_no,
                                     vtss_basic_counters_t *const counters);


/** \brief Port forwarding state */
typedef enum
{
    VTSS_PORT_FORWARD_ENABLED,   /**< Forward in both directions */
    VTSS_PORT_FORWARD_DISABLED,  /**< Forwarding and learning disabled */
    VTSS_PORT_FORWARD_INGRESS,   /**< Forward frames from port only */
    VTSS_PORT_FORWARD_EGRESS     /**< Forward frames to port only (learning disabled) */
} vtss_port_forward_t;

/**
 * \brief Get port forwarding state.
 *
 * \param inst [IN]      Target instance reference.
 * \param port_no [IN]   Port number.
 * \param forward [OUT]  Forwarding state.
 *
 * \return Return code.
 **/
vtss_rc vtss_port_forward_state_get(const vtss_inst_t     inst,
                                    const vtss_port_no_t  port_no, 
                                    vtss_port_forward_t   *const forward);

/**
 * \brief Set port forwarding state.
 *
 * \param inst [IN]     Target instance reference.
 * \param port_no [IN]  Port number.
 * \param forward [IN]  Forwarding state.
 *
 * \return Return code.
 **/
vtss_rc vtss_port_forward_state_set(const vtss_inst_t          inst,
                                    const vtss_port_no_t       port_no, 
                                    const vtss_port_forward_t  forward);

#if defined(VTSS_FEATURE_PORT_IFH)
/** \brief Port Internal Frame Header structure */
typedef struct
{
#if defined(VTSS_ARCH_SERVAL)
    BOOL ena_inj_header; /**< At ingress expect long prefix followed by an internal frame header */
    BOOL ena_xtr_header; /**< At egress prepend long prefix followed by the internal frame header */
#endif /* VTSS_ARCH_SERVAL */

#if defined(VTSS_ARCH_JAGUAR_2)
    BOOL ena_inj_header; /**< At ingress expect short prefix: DMAC:SMAC:0x8880:0007
                              followed by an internal frame header and then the frame */
    BOOL ena_xtr_header; /**< At egress prepend short prefix: DMAC:SMAC:0x8880:0007  (DMAC:SMAC from frame)
                              followed by an internal frame header and then the frame */
    BOOL ena_ifh_header; /**< Same as ena_xtr_header (for compatability with JR1) */
#endif /* VTSS_ARCH_JAGUAR_2 */

#if defined(VTSS_ARCH_JAGUAR_1)
    BOOL ena_ifh_header; /**< At egress keep IFH */
#endif /* VTSS_ARCH_JAGUAR_1 */
} vtss_port_ifh_t;

/**
 * \brief Set port Internal Frame Header settings.
 *
 * \param inst [IN]     Target instance reference.
 * \param port_no [IN]  Port number.
 * \param conf [IN]     Port IFH structure.
 *
 * \return Return code.
 **/
vtss_rc vtss_port_ifh_conf_set(const vtss_inst_t       inst,
                               const vtss_port_no_t    port_no,
                               const vtss_port_ifh_t  *const conf);

/**
 * \brief Get port Internal Frame Header settings.
 *
 * \param inst [IN]     Target instance reference.
 * \param port_no [IN]  Port number.
 * \param conf [OUT]    Port IFH configuration.
 *
 * \return Return code.
 **/
vtss_rc vtss_port_ifh_conf_get(const vtss_inst_t     inst,
                               const vtss_port_no_t  port_no,
                               vtss_port_ifh_t      *const conf);
#endif /* VTSS_FEATURE_PORT_IFH) */

/**
 * \brief Direct MIIM read (bypassing port map)
 *
 * \param inst            [IN]  Target instance reference.
 * \param chip_no         [IN]  Chip number (if multi-chip instance).
 * \param miim_controller [IN]  MIIM Controller Instance
 * \param miim_addr       [IN]  MIIM Device Address
 * \param addr            [IN]  MIIM Register Address
 * \param value           [OUT] Register value read
 *
 * \return Return code.
 **/
vtss_rc vtss_miim_read(const vtss_inst_t            inst,
                       const vtss_chip_no_t         chip_no,
                       const vtss_miim_controller_t miim_controller,
                       const u8                     miim_addr,
                       const u8                     addr,
                       u16                          *const value);

/**
 * \brief Direct MIIM write (bypassing port map)
 *
 * \param inst            [IN]  Target instance reference.
 * \param chip_no         [IN]  Chip number (if multi-chip instance).
 * \param miim_controller [IN]  MIIM Controller Instance
 * \param miim_addr       [IN]  MIIM Device Address
 * \param addr            [IN]  MIIM Register Address
 * \param value           [IN]  Register value to write
 *
 * \return Return code.
 **/
vtss_rc vtss_miim_write(const vtss_inst_t            inst,
                        const vtss_chip_no_t         chip_no,
                        const vtss_miim_controller_t miim_controller,
                        const u8                     miim_addr,
                        const u8                     addr,
                        const u16                    value);
#endif /* VTSS_FEATURE_PORT_CONTROL */

#if defined(VTSS_FEATURE_10G) || defined(VTSS_CHIP_10G_PHY)
/**
 * \brief Read value from MMD register.
 *
 * \param inst    [IN]  Target instance reference.
 * \param port_no [IN]  Port number connected to MMD.
 * \param mmd     [IN]  MMD number.
 * \param addr    [IN]  PHY register address.
 * \param value   [OUT] PHY register value.
 *
 * \return Return code.
 **/
vtss_rc vtss_port_mmd_read(const vtss_inst_t     inst,
                           const vtss_port_no_t  port_no,
                           const u8              mmd,
                           const u16             addr,
                           u16                   *const value);
/**
 * \brief Read values (a number of 16 bit values) from MMD register.
 *
 * \param inst    [IN]  Target instance reference.
 * \param port_no [IN]  Port number connected to MMD.
 * \param mmd     [IN]  MMD number.
 * \param addr    [IN]  PHY register address.
 * \param buf     [OUT] PHY register values.
 * \param count   [IN]  number of values to read.
 *
 * \return Return code.
 **/
vtss_rc vtss_port_mmd_read_inc(const vtss_inst_t     inst,
                               const vtss_port_no_t  port_no,
                               const u8              mmd,
                               const u16             addr,
                               u16                   *const buf,
                               u8                    count);
                          
/**
 * \brief Write value to MMD register.
 *
 * \param inst    [IN]  Target instance reference.
 * \param port_no [IN]  Port number connected to MMD.
 * \param mmd     [IN]  MMD number.
 * \param addr    [IN]  PHY register address.
 * \param value   [IN]  PHY register value.
 *
 * \return Return code.
 **/
vtss_rc vtss_port_mmd_write(const vtss_inst_t     inst,
                            const vtss_port_no_t  port_no,
                            const u8              mmd,
                            const u16             addr,
                            const u16             value);


/**
 * \brief Read, modify and write value to MMD register.
 *
 * \param inst     [IN]  Target instance reference.
 * \param port_no  [IN]  Port number connected to MMD.
 * \param mmd      [IN]  MMD number.
 * \param addr     [IN]  PHY register address.
 * \param value    [IN]  PHY register value.
 * \param mask     [IN]  PHY register mask, only enabled bits are changed.
 *
 * \return Return code.
 **/
vtss_rc vtss_port_mmd_masked_write(const vtss_inst_t     inst,
                                   const vtss_port_no_t  port_no,
                                   const u8              mmd,
                                   const u16             addr,
                                   const u16             value,
                                   const u16             mask);

/**
 * \brief Direct MMD read (Clause 45, bypassing port map)
 *
 * \param inst            [IN]  Target instance reference.
 * \param chip_no         [IN]  Chip number (if multi-chip instance).
 * \param miim_controller [IN]  MIIM Controller Instance
 * \param miim_addr       [IN]  MIIM Device Address
 * \param mmd             [IN]  MMD number.
 * \param addr            [IN]  MIIM Register Address
 * \param value           [OUT] Register value read
 *
 * \return Return code.
 **/
vtss_rc vtss_mmd_read(const vtss_inst_t            inst,
                      const vtss_chip_no_t         chip_no,
                      const vtss_miim_controller_t miim_controller,
                      const u8                     miim_addr,
                      const u8                     mmd,
                      const u16                    addr,
                      u16                          *const value);

/**
 * \brief Direct MMD write (Clause 45, bypassing port map)
 *
 * \param inst            [IN]  Target instance reference.
 * \param chip_no         [IN]  Chip number (if multi-chip instance).
 * \param miim_controller [IN]  MIIM Controller Instance
 * \param miim_addr       [IN]  MIIM Device Address
 * \param mmd             [IN]  MMD number.
 * \param addr            [IN]  MIIM Register Address
 * \param value           [IN]  Register value to write
 *
 * \return Return code.
 **/
vtss_rc vtss_mmd_write(const vtss_inst_t            inst,
                       const vtss_chip_no_t         chip_no,
                       const vtss_miim_controller_t miim_controller,
                       const u8                     miim_addr,
                       const u8                     mmd,
                       const u16                    addr,
                       const u16                    value);
#endif /* VTSS_FEATURE_10G */

#ifdef __cplusplus
}
#endif
#endif /* _VTSS_PORT_API_H_ */
