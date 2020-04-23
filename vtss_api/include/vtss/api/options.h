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
 * \brief Features and options
 * \details This header file describes target features and compile-time options
 */

#ifndef _VTSS_OPTIONS_H_
#define _VTSS_OPTIONS_H_

#ifdef VTSS_HAS_VTSS_API_CONFIG_H
#include "vtss_api_config.h"
#endif

/* ================================================================= *
 *  Features
 * ================================================================= */

#if defined(VTSS_CHIP_DAYTONA) || defined(VTSS_CHIP_TALLADEGA)
#define VTSS_ARCH_DAYTONA                   /**< Daytona chip architecture */
#define VTSS_FEATURE_WARM_START             /**< Warm start */
#define VTSS_FEATURE_MISC                   /**< Miscellaneous */
#define VTSS_FEATURE_PORT_CONTROL           /**< Port control (not used, but api depends on it) */
#define VTSS_FEATURE_CLAUSE_37              /**< IEEE 802.3 clause 37 auto-negotioation(not used, but api depends on it) */
#define VTSS_FEATURE_WIS                    /**< IEEE 802.3 clause 50 Wan Interface Sublayer (WIS) */
//#define VTSS_FEATURE_ANEG                   /**< IEEE 802.3 clause 73 auto-negotioation */
#define VTSS_FEATURE_XFI                    /**< XFI High speed Serdes interface */
#define VTSS_FEATURE_XAUI                   /**< XAUI interface function block */
#define VTSS_FEATURE_UPI                    /**< UPI High speed Serdes interface */
//#define VTSS_FEATURE_TFI5                   /**< TFI5 interface function block (TFI5 not yet included in Daytona release) */
//#define VTSS_FEATURE_SFI4                   /**< SFI4 interface function block  (SFI4 not yet included in Daytona release)*/
#define VTSS_FEATURE_GFP                    /**< GFP mapper function block (GFP not yet included in Daytona release)*/
#define VTSS_FEATURE_RAB                    /**< Rate Adaptation Buffer  */
//#define VTSS_FEATURE_AE                     /**< Adaptive Equalization algorithm (AE not yet included in Daytona release) */
#define VTSS_FEATURE_PCS_10GBASE_R          /**< IEEE 802.3 clause 49 PCS_10GBASE_R */
#define VTSS_FEATURE_MAC10G                 /**< MAC layer */
//#define VTSS_FEATURE_PORT_CNT_ETHER_LIKE  /**< Ethernet-like counters */
#define VTSS_FEATURE_10G                    /**< 10G ports */
//#define VTSS_FEATURE_INTERRUPTS           /**< Port Interrupt support */
#define VTSS_FEATURE_OTN                    /**< OTN mapper function block */
#define VTSS_DAYTONA_CHANNELS   2           /**< Number of channels in the chip */
#define VTSS_FEATURE_I2C                    /**< I2C controller driver support */
#define VTSS_FEATURE_PHY_TIMESTAMP          /**< 1588 timestamp feature (for PTP/OAM) */
#define VTSS_FEATURE_OHA                    /**< OH Add/Drop for OTN/WIS */
#define VTSS_DAYTONA_OOS_FIX                /**< OOS workaround for Daytona */
#endif /* VTSS_CHIP_DAYTONA */


#if defined(VTSS_CHIP_LYNX_1) || defined(VTSS_CHIP_JAGUAR_1)
/**< Jaguar-1 CE switches */
#define VTSS_ARCH_JAGUAR_1                     /**< Jaguar-1 architecture */
#define VTSS_ARCH_JAGUAR_1_CE_SWITCH           /**< Jaguar-1 CE switch family */
#define VTSS_FEATURE_EVC                       /**< Ethernet Virtual Connections */
#define VTSS_FEATURE_E_TREE                    /**< EVC E-Tree */
#define VTSS_FEATURE_TIMESTAMP                 /**< Packet timestamp feature (for PTP/OAM) */
#define VTSS_FEATURE_PHY_TIMESTAMP             /**< PHY timestamp feature (for PTP/OAM) */
#if defined(VTSS_CHIP_JAGUAR_1)
#define VTSS_FEATURE_VSTAX                     /**< VStaX stacking */
#define VTSS_FEATURE_AGGR_GLAG                 /**< Global link aggregations across stack */
#define VTSS_FEATURE_VSTAX_V2                  /**< VStaX stacking, as implemented on Jaguar1 (VStaX2/AF) */
//#define VTSS_FEATURE_PHY_TS_ONE_STEP_TXFIFO_OPTION  
#define VTSS_PHY_TS_SPI_CLK_THRU_PPS0          /**< Use 1588_PPS0 as New SPI_CLK, applicable only to 8574-15; old SPI_CLK pin will not be used anymore */
//#define VTSS_FEATURE_PTP_DELAY_COMP_ENGINE     /**< Delay compensation engine for 8574-10 device */
#define VTSS_FEATURE_FAN                       /**< Fan control */
#endif /* JAGUAR_1 */
#endif /* LYNX_1/JAGUAR_1 */

#if defined(VTSS_CHIP_E_STAX_III_48) || defined(VTSS_CHIP_E_STAX_III_68) || \
    defined(VTSS_CHIP_E_STAX_III_24_DUAL) || defined(VTSS_CHIP_E_STAX_III_68_DUAL)
/**< Jaguar-1 SME switches */
#define VTSS_ARCH_JAGUAR_1                     /**< Jaguar-1 architecture */
#define VTSS_ARCH_JAGUAR_1_SME_SWITCH          /**< Jaguar-1 SME switch family */
#define VTSS_FEATURE_VSTAX                     /**< VStaX stacking */
#define VTSS_FEATURE_AGGR_GLAG                 /**< Global link aggregations across stack */
#define VTSS_FEATURE_VSTAX_V2                  /**< VStaX stacking, as implemented on Jaguar1 (VStaX2/AF) */
#endif /* E_STAX_III_48/E_STAX_III_68 */

#if defined(VTSS_CHIP_LYNX_1) || defined(VTSS_CHIP_JAGUAR_1) || \
    defined(VTSS_CHIP_E_STAX_III_48) || defined(VTSS_CHIP_E_STAX_III_68)
/**< Jaguar-1 single chip switches */
#define VTSS_FEATURE_PVLAN                     /**< Private VLANs */
#endif /* LYNX_1/JAGUAR_1/E_STAX_III_48/E_STAX_III_68 */

#if defined(VTSS_CHIP_E_STAX_III_24_DUAL) || defined(VTSS_CHIP_E_STAX_III_68_DUAL)
/**< Jaguar-1 dual chip switches */
#define VTSS_ARCH_JAGUAR_1_DUAL                /**< Dual Jaguar-1 architecture */
#endif /* E_STAX_III_48_DUAL/E_STAX_III_68_DUAL */

#if defined(VTSS_CHIP_E_STAX_III_48) || defined(VTSS_CHIP_E_STAX_III_68)
/**< Jaguar-1 single chip SME switches */
#define VTSS_FEATURE_VLAN_COUNTERS             /**< VLAN counters are only supported in single chip Jaguar non-CE switches */
#endif /* defined(VTSS_CHIP_E_STAX_III_48) || defined(VTSS_CHIP_E_STAX_III_68) */

#if defined(VTSS_ARCH_JAGUAR_1)
/**< Jaguar-1 family */
#define VTSS_OPT_TS_SPI_FPGA
#define VTSS_CHIP_10G_PHY                      /**< 10Gb 848x Phy API  */
#define VTSS_FEATURE_MISC                      /**< Miscellaneous */
#define VTSS_FEATURE_SERIAL_GPIO               /**< Serial GPIO control */
#define VTSS_FEATURE_PORT_CONTROL              /**< Port control */
#define VTSS_FEATURE_PORT_IFH                  /**< Port IFH control */
#define VTSS_FEATURE_CLAUSE_37                 /**< IEEE 802.3 clause 37 auto-negotiation */
#define VTSS_FEATURE_10G                       /**< 10G ports */
#define VTSS_FEATURE_NPI                       /**< NPI port */
#define VTSS_FEATURE_EXC_COL_CONT              /**< Excessive collision continuation */
#define VTSS_FEATURE_PORT_CNT_ETHER_LIKE       /**< Ethernet-like counters */
#define VTSS_FEATURE_PORT_CNT_BRIDGE           /**< Bridge counters */
#define VTSS_FEATURE_QOS                       /**< QoS */
#define VTSS_FEATURE_QCL                       /**< QoS: QoS Control Lists */
#define VTSS_FEATURE_QCL_V2                    /**< QoS: QoS Control Lists, V2 features */
#define VTSS_FEATURE_QOS_PORT_POLICER_EXT      /**< QoS: Port Policer Extensions */
#define VTSS_FEATURE_QOS_PORT_POLICER_EXT_FPS  /**< QoS: Port Policer has frame rate support */
#define VTSS_FEATURE_QOS_PORT_POLICER_EXT_FC   /**< QoS: Port Policer has flow control support */
#define VTSS_FEATURE_QOS_PORT_POLICER_EXT_DPBL /**< QoS: Port Policer has Drop Precedence Bypas Level support */
#define VTSS_FEATURE_QOS_PORT_POLICER_EXT_TTM  /**< QoS: Port Policer has Traffic_Type Mask support */
#define VTSS_FEATURE_QOS_QUEUE_POLICER         /**< QoS: Has Ingress Queue Policers */
#define VTSS_FEATURE_QOS_QUEUE_TX              /**< QoS: Has TX Queue support */
#define VTSS_FEATURE_QOS_SCHEDULER_V2          /**< QoS: 2. version of scheduler */
#define VTSS_FEATURE_QOS_TAG_REMARK_V2         /**< QoS: 2. version of tag priority remarking */
#define VTSS_FEATURE_QOS_CLASSIFICATION_V2     /**< QoS: 2. version of classification */
#define VTSS_FEATURE_QOS_EGRESS_QUEUE_SHAPERS  /**< QoS: Has Egress Queue Shapers */
#define VTSS_FEATURE_QOS_EGRESS_QUEUE_SHAPERS_EB /**< QoS: Egress Queue Shapers has Excess Bandwidth support */
#define VTSS_FEATURE_QOS_DSCP_REMARK           /**< QoS: Has DSCP remarking */
#define VTSS_FEATURE_QOS_DSCP_REMARK_V2        /**< QoS: 2. version of DSCP remarking */
#define VTSS_FEATURE_QOS_WRED                  /**< QoS: WRED pr. port */
#define VTSS_FEATURE_PACKET                    /**< CPU Rx/Tx frame configuration */
#define VTSS_FEATURE_PACKET_TX                 /**< CPU Tx frame */
#define VTSS_FEATURE_PACKET_RX                 /**< CPU Rx frame */
#define VTSS_FEATURE_PACKET_GROUPING           /**< Extraction and injection occurs through extraction and injection groups rather than queues. */
#define VTSS_FEATURE_PACKET_PORT_REG           /**< Packet registration per port */
#define VTSS_FEATURE_LAYER2                    /**< Layer 2 (switching) */
#define VTSS_FEATURE_VLAN_PORT_V2              /**< VLAN port configuration, V2 features */
#define VTSS_FEATURE_VLAN_TX_TAG               /**< VLAN tagging per (VID, port) */
#define VTSS_FEATURE_VLAN_SVL                  /**< Shared VLAN Learning */
#define VTSS_FEATURE_MAC_AGE_AUTO              /**< Automatic MAC address ageing */
#define VTSS_FEATURE_MAC_CPU_QUEUE             /**< CPU queue per MAC address */
#define VTSS_FEATURE_LAYER3                    /**< Layer 3 (routing) */
#define VTSS_FEATURE_PORT_MUX                  /**< Port mux between serdes blocks and ports */
#define VTSS_FEATURE_VCAP                      /**< VCAP */
#define VTSS_FEATURE_ACL                       /**< Access Control Lists */
#define VTSS_FEATURE_ACL_V1                    /**< Access Control Lists, V2 features */
#define VTSS_FEATURE_VCL                       /**< VLAN Control Lists */
#define VTSS_FEATURE_SYNCE                     /**< SYNCE - L1 syncronization feature */
#define VTSS_FEATURE_FAN                       /**< Fan control */
#define VTSS_FEATURE_EEE                       /**< Energy Efficient Ethernet */
#define VTSS_FEATURE_SERDES_MACRO_SETTINGS     /**< Hooks for Serdes Macro configuration */
#define VTSS_FEATURE_LED_POW_REDUC             /**< LED power reduction */
#define VTSS_FEATURE_10GBASE_KR                /**< 10GBASE_KR transmit equalization support IEEE802.3 Clause 72.7 */
#define VTSS_FEATURE_IRQ_CONTROL               /**< General IRQ support */
#if !defined(VTSS_OPT_VCORE_III)
  #define VTSS_OPT_VCORE_III 1                 /**< Internal VCore-III (MIPS) CPU enabled by default */
#endif
#if VTSS_OPT_VCORE_III != 0
  #define VTSS_FEATURE_FDMA                    /**< Frame DMA */
  #if defined(VTSS_FEATURE_EVC)
    #define VTSS_FEATURE_AFI_FDMA              /**< FDMA-based Automatic Frame injector supported on EVC-enabled parts */
  #endif /* VTSS_FEATURE_EVC */
#endif /* VTSS_OPT_VCORE_III != 0 */
#define VTSS_FEATURE_VLAN_TRANSLATION          /**< VLAN Translation */
#define VTSS_FEATURE_SFLOW                     /**< Statistical flow sampling */
#define VTSS_PHY_10G_FIFO_SYNC                 /**< TSFIFO SYNC For 10G PHY */
#define VIPER_B_FIFO_RESET                     /**< Viper B 1588 FIFO sync */
#endif /* VTSS_ARCH_JAGUAR_1 */

#if defined(VTSS_CHIP_SPARX_IV_44) || defined(VTSS_CHIP_SPARX_IV_52) || defined(VTSS_CHIP_SPARX_IV_80)
#define VTSS_ARCH_JAGUAR_2                     /**< Jaguar-2 architecture */
#if defined(VTSS_OPT_JAGUAR_2_B) && VTSS_OPT_JAGUAR_2_B != 0
#define VTSS_ARCH_JAGUAR_2_B                   /**< Jaguar-2 revision B architecture */
#else
#define VTSS_ARCH_JAGUAR_2_C                   /**< Jaguar-2 revision C architecture */
#endif /* VTSS_OPT_JAGUAR_2_B */
#define VTSS_FEATURE_VLAN_COUNTERS             /**< VLAN counters are only supported for SMB devices */
#endif /* VTSS_CHIP_JAGUAR_2 */

#if defined(VTSS_CHIP_SPARX_IV_90)
#define VTSS_ARCH_JAGUAR_2                     /**< Jaguar-2 architecture */
#define VTSS_ARCH_JAGUAR_2_C                   /**< Jaguar-2 revision C architecture */
#define VTSS_FEATURE_VLAN_COUNTERS             /**< VLAN counters are only supported for SMB devices */
#endif /* VTSS_CHIP_SPARX_IV_90 */

#if defined(VTSS_CHIP_SERVAL_2) || defined(VTSS_CHIP_LYNX_2) || defined(VTSS_CHIP_JAGUAR_2)
#define VTSS_ARCH_JAGUAR_2                     /**< Jaguar-2 architecture */
#define VTSS_ARCH_JAGUAR_2_B                   /**< Jaguar-2 revision A/B architecture */
#endif /* VTSS_CHIP_JAGUAR_2 */

#if defined(VTSS_ARCH_JAGUAR_2)
//#define VTSS_FEATURE_WARM_START                  /**< Warm start */
#define VTSS_CHIP_10G_PHY                        /**< 10Gb 848x Phy API  */
#define VTSS_FEATURE_MISC                        /**< Miscellaneous */
#define VTSS_FEATURE_SERIAL_GPIO                 /**< Serial GPIO control */
#define VTSS_FEATURE_PORT_CONTROL                /**< Port control */
#define VTSS_FEATURE_PORT_IFH                    /**< Port IFH control */
#define VTSS_FEATURE_CLAUSE_37                   /**< IEEE 802.3 clause 37 auto-negotiation */
#define VTSS_FEATURE_EXC_COL_CONT                /**< Excessive collision continuation */
#define VTSS_FEATURE_PORT_CNT_ETHER_LIKE         /**< Ethernet-like counters */
#define VTSS_FEATURE_PORT_CNT_BRIDGE             /**< Bridge counters */
#define VTSS_FEATURE_QOS                         /**< QoS */
#define VTSS_FEATURE_VSTAX                       /**< VStaX stacking */
#define VTSS_FEATURE_VSTAX_V2                    /**< VStaX stacking, as implemented on Jaguar1 (VStaX2/AF) */
#define VTSS_FEATURE_AGGR_GLAG                   /**< Global link aggregations across stack */
#define VTSS_FEATURE_PORT_MUX                    /**< Port mux between serdes blocks and ports */
#define VTSS_FEATURE_PFC                         /**< 802.1Qbb Priority Flow Control */
#define VTSS_FEATURE_QCL                         /**< QoS: QoS Control Lists */
#define VTSS_FEATURE_QCL_V2                      /**< QoS: QoS Control Lists, V2 features */
#define VTSS_FEATURE_QCL_KEY_S_TAG               /**< QoS: QoS Control Lists has S tag support */
#define VTSS_FEATURE_QCL_KEY_INNER_TAG           /**< QoS: QoS Control Lists has inner tag */
#define VTSS_FEATURE_QCL_KEY_DMAC                /**< QoS: QoS Control Lists has destination MAC address */
#define VTSS_FEATURE_QCL_KEY_DIP                 /**< QoS: QoS Control Lists has destination IP address */
#define VTSS_FEATURE_QCL_PCP_DEI_ACTION          /**< QoS: QoS Control Lists has PCP and DEI action */
#define VTSS_FEATURE_QCL_POLICY_ACTION           /**< QoS: QoS Control Lists has policy action */
#define VTSS_FEATURE_QOS_POLICER_UC_SWITCH       /**< QoS: Unicast policer per switch */
#define VTSS_FEATURE_QOS_POLICER_MC_SWITCH       /**< QoS: Multicast policer per switch */
#define VTSS_FEATURE_QOS_POLICER_BC_SWITCH       /**< QoS: Broadcast policer per switch */
#define VTSS_FEATURE_QOS_PORT_POLICER_EXT        /**< QoS: Port Policer Extensions */
#define VTSS_FEATURE_QOS_PORT_POLICER_EXT_FPS    /**< QoS: Port Policer has frame rate support */
#define VTSS_FEATURE_QOS_PORT_POLICER_EXT_FC     /**< QoS: Port Policer has flow control support */
#define VTSS_FEATURE_QOS_PORT_POLICER_EXT_DPBL   /**< QoS: Port Policer has Drop Precedence Bypas Level support */
#define VTSS_FEATURE_QOS_PORT_POLICER_EXT_TTM_V2 /**< QoS: Port Policer has Traffic_Type Mask version 2 support */
#define VTSS_FEATURE_QOS_QUEUE_POLICER           /**< QoS: Has Ingress Queue Policers */
#define VTSS_FEATURE_QOS_QUEUE_TX                /**< QoS: Has TX Queue support */
#define VTSS_FEATURE_QOS_SCHEDULER_V2            /**< QoS: 2. version of scheduler */
#define VTSS_FEATURE_QOS_SCHEDULER_DWRR_CNT      /**< QoS: Scheduler supports variable number of DWRR inputs */
#define VTSS_FEATURE_QOS_TAG_REMARK_V2           /**< QoS: 2. version of tag priority remarking */
#define VTSS_FEATURE_QOS_CLASSIFICATION_V2       /**< QoS: 2. version of classification */
#define VTSS_FEATURE_QOS_EGRESS_QUEUE_SHAPERS    /**< QoS: Has Egress Queue Shapers */
#define VTSS_FEATURE_QOS_EGRESS_SHAPERS_DLB      /**< QoS: Egress shapers has DLB support */
#define VTSS_FEATURE_QOS_DSCP_CLASS_DP_AWARE     /**< QoS: DSCP classification is DP aware */
#define VTSS_FEATURE_QOS_DSCP_REMARK             /**< QoS: Has DSCP remarking */
#define VTSS_FEATURE_QOS_DSCP_REMARK_V2          /**< QoS: 2. version of DSCP remarking */
#define VTSS_FEATURE_QOS_WRED_V3                 /**< QoS: WRED global - per queue (0..7), per dpl (1..3), per group (0..2) */
#define VTSS_FEATURE_QOS_POLICER_DLB             /**< DLB policers */
#define VTSS_FEATURE_PACKET                    /**< CPU Rx/Tx frame configuration */
#define VTSS_FEATURE_PACKET_TX                 /**< CPU Tx frame */
#define VTSS_FEATURE_PACKET_RX                 /**< CPU Rx frame */
#define VTSS_FEATURE_PACKET_GROUPING             /**< Extraction and injection occurs through extraction and injection groups rather than queues. */
#define VTSS_FEATURE_PACKET_PORT_REG             /**< Packet registration per port */
#define VTSS_FEATURE_LAYER2                      /**< Layer 2 (switching) */
#define VTSS_FEATURE_PVLAN                       /**< Private VLANs */
#define VTSS_FEATURE_VLAN_PORT_V2                /**< VLAN port configuration, V2 features */
#define VTSS_FEATURE_VLAN_TX_TAG                 /**< VLAN tagging per (VID, port) */
#define VTSS_FEATURE_MAC_AGE_AUTO                /**< Automatic MAC address ageing */
#define VTSS_FEATURE_MAC_CPU_QUEUE               /**< CPU queue per MAC address */
//#define VTSS_FEATURE_LAYER3                      /**< Layer 3 (routing) */
#define VTSS_FEATURE_EEE                         /**< Energy Efficient Ethernet */
#define VTSS_FEATURE_FAN                         /**< Fan control */
#define VTSS_FEATURE_VCAP                        /**< VCAP */
#define VTSS_FEATURE_ACL                         /**< Access Control Lists */
#define VTSS_FEATURE_ACL_V2                      /**< Access Control Lists, V2 features */
#define VTSS_FEATURE_VCL                         /**< VLAN Control Lists */
#define VTSS_FEATURE_TIMESTAMP                   /**< Packet timestamp feature (for PTP and OAM) */
#define VTSS_FEATURE_PHY_TIMESTAMP               /**< PHY timestamp feature (for PTP/OAM) */
#define VTSS_FEATURE_TIMESTAMP_ONE_STEP          /**< ONESTEP timestamp hardware support */
#define VTSS_FEATURE_TIMESTAMP_LATENCY_COMP      /**< Ingress and egress latency compensation hardware support */
#define VTSS_FEATURE_TIMESTAMP_ORG_TIME        /**< OriginTimestamp update hardware support */
#define VTSS_FEATURE_TIMESTAMP_P2P_DELAY_COMP  /**< Peer-to-peer path delay compensation hardware support */
#define VTSS_FEATURE_TIMESTAMP_ASYMMETRY_COMP  /**< Path delay asymmetry compensation hardware support */
//#define VTSS_FEATURE_DELAY_REQ_AUTO_RESP        /**< Delay_Req/Resp processing supported in hardware */
#define VTSS_FEATURE_SYNCE                       /**< SYNCE - L1 syncronization feature */
#define VTSS_FEATURE_NPI                         /**< NPI port */
#define VTSS_FEATURE_LED_POW_REDUC               /**< LED power reduction */
#define VTSS_FEATURE_INTERRUPTS                  /**< Port Interrupt support */
#define VTSS_FEATURE_IRQ_CONTROL                 /**< General IRQ support */
#define VTSS_FEATURE_VLAN_TRANSLATION            /**< VLAN Translation */
#define VTSS_FEATURE_SFLOW                       /**< sFlow feature    */
//#define VTSS_FEATURE_MIRROR_CPU                  /**< CPU mirroring */
#define VTSS_FEATURE_SERDES_MACRO_SETTINGS       /**< Hooks for Serdes Macro configuration */
#define VTSS_FEATURE_10GBASE_KR                  /**< KR */
//#define VTSS_FEATURE_AFI_SWC                     /**< AFI */
//#define VTSS_AFI_V2                              /**< AFI API version 2 */
#define VTSS_FEATURE_10G                         /**< 10G ports */
#if !defined(VTSS_OPT_VCORE_III)
  #define VTSS_OPT_VCORE_III 1                   /**< Internal VCore-III (MIPS) CPU enabled by default */
#endif
#if VTSS_OPT_VCORE_III != 0
  #define VTSS_FEATURE_FDMA                      /**< Frame DMA */
  #ifndef VTSS_OPT_FDMA_VER
    #define VTSS_OPT_FDMA_VER 3                  /**< Use of VTSS_OPT_FDMA_VER is the preferred way to indicate which version of the FDMA API is required */
  #endif
  #if VTSS_OPT_FDMA_VER != 3
    #error "Jaguar2 only supports FDMA API v3"
  #endif
#endif /* VTSS_OPT_VCORE_III != 0 */
#if !defined(VTSS_OPT_PCIE_ACCESS) && !defined(VTSS_OPT_VRAP_ACCESS)
#define VTSS_OPT_PCIE_ACCESS                    /**< PCIe access from external CPU */
#endif
#define VTSS_PHY_10G_FIFO_SYNC
#define VIPER_B_FIFO_RESET                     /**< Viper B 1588 FIFO sync */
#endif /* VTSS_ARCH_JAGUAR_2 */

#if defined(VTSS_CHIP_SPARX_III_10_UM) || defined(VTSS_CHIP_SPARX_III_17_UM) || defined(VTSS_CHIP_SPARX_III_25_UM) ||\
    defined(VTSS_CHIP_SPARX_III_10) || defined(VTSS_CHIP_SPARX_III_18) || \
    defined(VTSS_CHIP_SPARX_III_24) || defined(VTSS_CHIP_SPARX_III_26) || \
    defined(VTSS_CHIP_SPARX_III_10_01)
#define VTSS_ARCH_SPARX_III                    /**< SparX-III SME switches */
#endif /* SPARX_III */

#if defined(VTSS_CHIP_CARACAL_1) || defined(VTSS_CHIP_CARACAL_2) || defined(VTSS_CHIP_CARACAL_LITE)
#define VTSS_ARCH_CARACAL                      /**< Caracal CE switches */
#define VTSS_FEATURE_EVC                       /**< Ethernet Virtual Connections */
#endif /* CARACAL */

#if defined(VTSS_FEATURE_EVC) || defined(VTSS_CHIP_SPARX_III_10_01)
#define VTSS_FEATURE_QOS_POLICER_DLB           /**< DLB policers */
#endif

#if defined(VTSS_ARCH_SPARX_III) || defined(VTSS_ARCH_CARACAL)
#define VTSS_ARCH_LUTON26                      /**< Luton26 architecture */
#define VTSS_FEATURE_MISC                      /**< Miscellaneous */
#define VTSS_FEATURE_SERIAL_GPIO               /**< Serial GPIO control */
#define VTSS_FEATURE_PORT_CONTROL              /**< Port control */
#define VTSS_FEATURE_CLAUSE_37                 /**< IEEE 802.3 clause 37 auto-negotiation */
#define VTSS_FEATURE_EXC_COL_CONT              /**< Excessive collision continuation */
#define VTSS_FEATURE_PORT_CNT_BRIDGE           /**< Bridge counters */
#define VTSS_FEATURE_QOS                       /**< QoS */
#define VTSS_FEATURE_QCL                       /**< QoS: QoS Control Lists */
#define VTSS_FEATURE_QCL_V2                    /**< QoS: QoS Control Lists, V2 features */
#define VTSS_FEATURE_QCL_DMAC_DIP              /**< QoS: QoS Control Lists, match on either SMAC/SIP or DMAC/DIP */
#define VTSS_FEATURE_QCL_KEY_S_TAG             /**< QoS: QoS Control Lists has S tag support */
#define VTSS_FEATURE_QCL_PCP_DEI_ACTION        /**< QoS: QoS Control Lists has PCP and DEI action */
#define VTSS_FEATURE_QCL_POLICY_ACTION         /**< QoS: QoS Control Lists has policy action */
#define VTSS_FEATURE_QOS_POLICER_UC_SWITCH     /**< QoS: Unicast policer per switch */
#define VTSS_FEATURE_QOS_POLICER_MC_SWITCH     /**< QoS: Multicast policer per switch */
#define VTSS_FEATURE_QOS_POLICER_BC_SWITCH     /**< QoS: Broadcast policer per switch */
#define VTSS_FEATURE_QOS_PORT_POLICER_EXT      /**< QoS: Port Policer Extensions */
#define VTSS_FEATURE_QOS_PORT_POLICER_EXT_FPS  /**< QoS: Port Policer has frame rate support */
#define VTSS_FEATURE_QOS_PORT_POLICER_EXT_FC   /**< QoS: Port Policer has flow control support */
#define VTSS_FEATURE_QOS_QUEUE_POLICER         /**< QoS: Has Ingress Queue Policers */
#define VTSS_FEATURE_QOS_QUEUE_TX              /**< QoS: Has TX Queue support */
#define VTSS_FEATURE_QOS_SCHEDULER_V2          /**< QoS: 2. version of scheduler */
#define VTSS_FEATURE_QOS_TAG_REMARK_V2         /**< QoS: 2. version of tag priority remarking */
#define VTSS_FEATURE_QOS_CLASSIFICATION_V2     /**< QoS: 2. version of classification */
#define VTSS_FEATURE_QOS_EGRESS_QUEUE_SHAPERS  /**< QoS: Has Egress Queue Shapers */
#define VTSS_FEATURE_QOS_EGRESS_QUEUE_SHAPERS_EB /**< QoS: Egress Queue Shapers has Excess Bandwidth support */
#define VTSS_FEATURE_QOS_DSCP_CLASS_DP_AWARE   /**< QoS: DSCP classification is DP aware */
#define VTSS_FEATURE_QOS_DSCP_REMARK           /**< QoS: Has DSCP remarking */
#define VTSS_FEATURE_QOS_DSCP_REMARK_V2        /**< QoS: 2. version of DSCP remarking */
#define VTSS_FEATURE_QOS_DSCP_REMARK_DP_AWARE  /**< QoS: DSCP remarking is DP aware */
#define VTSS_FEATURE_PACKET                    /**< CPU Rx/Tx frame configuration */
#define VTSS_FEATURE_PACKET_TX                 /**< CPU Tx frame */
#define VTSS_FEATURE_PACKET_RX                 /**< CPU Rx frame */
#define VTSS_FEATURE_PACKET_GROUPING           /**< Extraction and injection occurs through extraction and injection groups rather than queues. */
#define VTSS_FEATURE_PACKET_PORT_REG           /**< Packet registration per port */
#define VTSS_FEATURE_LAYER2                    /**< Layer 2 (switching) */
#define VTSS_FEATURE_PVLAN                     /**< Private VLANs */
#define VTSS_FEATURE_VLAN_PORT_V2              /**< VLAN port configuration, V2 features */
#define VTSS_FEATURE_VLAN_TX_TAG               /**< VLAN tagging per (VID, port) */
#define VTSS_FEATURE_IPV4_MC_SIP               /**< Source specific IPv4 multicast */
#define VTSS_FEATURE_IPV6_MC_SIP               /**< Source specific IPv6 multicast */
#define VTSS_FEATURE_MAC_AGE_AUTO              /**< Automatic MAC address ageing */
#define VTSS_FEATURE_MAC_CPU_QUEUE             /**< CPU queue per MAC address */
#define VTSS_FEATURE_EEE                       /**< Energy Efficient Ethernet */
#define VTSS_FEATURE_PORT_MUX                  /**< Port mux between serdes blocks and ports */
#define VTSS_FEATURE_FAN                       /**< Fan control */
#define VTSS_FEATURE_VCAP                      /**< VCAP */
#define VTSS_FEATURE_ACL                       /**< Access Control Lists */
#define VTSS_FEATURE_ACL_V2                    /**< Access Control Lists, V2 features */
#define VTSS_FEATURE_VCL                       /**< VLAN Control Lists */
#define VTSS_FEATURE_TIMESTAMP                 /**< Packet timestamp feature (for PTP and OAM) */
#define VTSS_FEATURE_TIMESTAMP_ONE_STEP        /**< ONESTEP timestamp hardware support */
#define VTSS_FEATURE_TIMESTAMP_LATENCY_COMP    /**< Ingress and egress latency compensation hardware support */
//#define VTSS_FEATURE_TIMESTAMP_P2P_DELAY_COMP  /**< Peer-to-peer path delay compensation hardware support */
//#define VTSS_FEATURE_TIMESTAMP_ASYMMETRY_COMP  /**< Path delay asymmetry compensation hardware support */
#define VTSS_FEATURE_SYNCE                     /**< SYNCE - L1 syncronization feature */
#define VTSS_FEATURE_NPI                       /**< NPI port */
#define VTSS_FEATURE_IRQ_CONTROL               /**< General IRQ support */
#if !defined(VTSS_OPT_VCORE_III)
  #define VTSS_OPT_VCORE_III 1                 /**< Internal VCore-III (MIPS) CPU enabled by default */
#endif
#if VTSS_OPT_VCORE_III != 0
  #define VTSS_FEATURE_FDMA                    /**< Frame DMA */
  #if defined(VTSS_FEATURE_EVC)
    #define VTSS_FEATURE_AFI_FDMA              /**< FDMA-based Automatic Frame injector supported on EVC-enabled parts */
  #endif /* VTSS_FEATURE_EVC */
#endif /* VTSS_OPT_VCORE_III != 0 */
#define VTSS_FEATURE_LED_POW_REDUC             /**< LED power reduction */
#define VTSS_FEATURE_INTERRUPTS                /**< Port Interrupt support */
#define VTSS_FEATURE_VLAN_TRANSLATION          /**< VLAN Translation */
#define VTSS_FEATURE_SFLOW                     /**< Statistical flow sampling */
#define VTSS_FEATURE_MIRROR_CPU                /**< CPU mirroring */
#define VTSS_FEATURE_SERDES_MACRO_SETTINGS     /**< Hooks for Serdes Macro configuration */
#endif /* SPARX_III/CARACAL */

#if defined(VTSS_CHIP_SPARX_III_11)
#define VTSS_ARCH_SERVAL                       /**< Serval architecture */
#define VTSS_ARCH_SERVAL_SME                   /**< Serval SME architecture */
#define VTSS_ARCH_SERVAL_CPU                   /**< Serval CPU system architecture */
#endif /* VTSS_CHIP_SPARX_III_11 */

#if defined(VTSS_CHIP_SERVAL) || defined(VTSS_CHIP_SERVAL_LITE)
#define VTSS_ARCH_SERVAL                       /**< Serval architecture */
#define VTSS_ARCH_SERVAL_CE                    /**< Serval CE architecture */
#define VTSS_ARCH_SERVAL_CPU                   /**< Serval CPU system architecture */
#endif /* VTSS_CHIP_SERVAL/SERVAL_SC */

#if defined(VTSS_CHIP_SEVILLE)
#define VTSS_ARCH_SERVAL                       /**< Serval architecture */
#define VTSS_ARCH_SEVILLE                      /**< Seville architecture */
#define VTSS_ARCH_SERVAL_SME                   /**< Serval SME architecture */
#endif /* VTSS_CHIP_SERVAL */

#if defined(VTSS_ARCH_SERVAL)
#define VTSS_FEATURE_WARM_START                /**< Warm start */
#define VTSS_FEATURE_MISC                      /**< Miscellaneous */
#define VTSS_FEATURE_PORT_CONTROL              /**< Port control */
#define VTSS_FEATURE_PORT_IFH                  /**< Port IFH control */
#define VTSS_FEATURE_CLAUSE_37                 /**< IEEE 802.3 clause 37 auto-negotiation */
#define VTSS_FEATURE_EXC_COL_CONT              /**< Excessive collision continuation */
#define VTSS_FEATURE_PORT_CNT_BRIDGE           /**< Bridge counters */
#define VTSS_FEATURE_PFC                       /**< 802.1Qbb Priority Flow Control */
#define VTSS_FEATURE_QOS                       /**< QoS */
#define VTSS_FEATURE_QCL                       /**< QoS: QoS Control Lists */
#define VTSS_FEATURE_QCL_V2                    /**< QoS: QoS Control Lists, V2 features */
#define VTSS_FEATURE_QCL_DMAC_DIP              /**< QoS: QoS Control Lists, match on either SMAC/SIP or DMAC/DIP */
#define VTSS_FEATURE_QCL_KEY_TYPE              /**< QoS: QoS Control Lists, different key types per port */
#define VTSS_FEATURE_QCL_KEY_S_TAG             /**< QoS: QoS Control Lists has S tag support */
#define VTSS_FEATURE_QCL_KEY_INNER_TAG         /**< QoS: QoS Control Lists has inner tag */
#define VTSS_FEATURE_QCL_KEY_DMAC              /**< QoS: QoS Control Lists has destination MAC address */
#define VTSS_FEATURE_QCL_KEY_DIP               /**< QoS: QoS Control Lists has destination IP address */
#define VTSS_FEATURE_QCL_PCP_DEI_ACTION        /**< QoS: QoS Control Lists has PCP and DEI action */
#define VTSS_FEATURE_QCL_POLICY_ACTION         /**< QoS: QoS Control Lists has policy action */
#define VTSS_FEATURE_QOS_POLICER_UC_SWITCH     /**< QoS: Unicast policer per switch */
#define VTSS_FEATURE_QOS_POLICER_MC_SWITCH     /**< QoS: Multicast policer per switch */
#define VTSS_FEATURE_QOS_POLICER_BC_SWITCH     /**< QoS: Broadcast policer per switch */
#define VTSS_FEATURE_QOS_PORT_POLICER_EXT      /**< QoS: Port Policer Extensions */
#define VTSS_FEATURE_QOS_PORT_POLICER_EXT_FPS  /**< QoS: Port Policer has frame rate support */
#define VTSS_FEATURE_QOS_PORT_POLICER_EXT_FC   /**< QoS: Port Policer has flow control support */
#define VTSS_FEATURE_QOS_QUEUE_TX              /**< QoS: Has TX Queue support */
#define VTSS_FEATURE_QOS_QUEUE_POLICER         /**< QoS: Has Ingress Queue Policers */
#define VTSS_FEATURE_QOS_SCHEDULER_V2          /**< QoS: 2. version of scheduler */
#define VTSS_FEATURE_QOS_SCHEDULER_DWRR_CNT    /**< QoS: Scheduler supports variable number of DWRR inputs */
#define VTSS_FEATURE_QOS_TAG_REMARK_V2         /**< QoS: 2. version of tag priority remarking */
#define VTSS_FEATURE_QOS_CLASSIFICATION_V2     /**< QoS: 2. version of classification */
#define VTSS_FEATURE_QOS_EGRESS_QUEUE_SHAPERS  /**< QoS: Has Egress Queue Shapers */
#define VTSS_FEATURE_QOS_EGRESS_QUEUE_SHAPERS_EB /**< QoS: Egress Queue Shapers has Excess Bandwidth support */
#define VTSS_FEATURE_QOS_EGRESS_SHAPERS_DLB    /**< QoS: Egress shapers has DLB support */
#define VTSS_FEATURE_QOS_EGRESS_SHAPERS_RT     /**< Qos: Egress shapers have rate type support - line or date rate */
#define VTSS_FEATURE_QOS_DSCP_CLASS_DP_AWARE   /**< QoS: DSCP classification is DP aware */
#define VTSS_FEATURE_QOS_DSCP_REMARK           /**< QoS: Has DSCP remarking */
#define VTSS_FEATURE_QOS_DSCP_REMARK_V2        /**< QoS: 2. version of DSCP remarking */
#define VTSS_FEATURE_QOS_DSCP_REMARK_DP_AWARE  /**< QoS: DSCP remarking is DP aware */
#define VTSS_FEATURE_QOS_WRED_V2               /**< QoS: WRED global - per queue (0..7), per dpl (0..1) */
#define VTSS_FEATURE_QOS_POLICER_DLB           /**< DLB policers */
#define VTSS_FEATURE_QOS_CPU_PORT_SHAPER       /**< QoS: Has CPU port shaper */
#define VTSS_FEATURE_PACKET                    /**< CPU Rx/Tx frame configuration */
#define VTSS_FEATURE_PACKET_TX                 /**< CPU Tx frame */
#define VTSS_FEATURE_PACKET_RX                 /**< CPU Rx frame */
#define VTSS_FEATURE_PACKET_GROUPING           /**< Extraction and injection occurs through extraction and injection groups rather than queues. */
#define VTSS_FEATURE_PACKET_PORT_REG           /**< Packet registration per port */
#define VTSS_FEATURE_LAYER2                    /**< Layer 2 (switching) */
#define VTSS_FEATURE_PVLAN                     /**< Private VLANs */
#define VTSS_FEATURE_VLAN_PORT_V2              /**< VLAN port configuration, V2 features */
#define VTSS_FEATURE_VLAN_TX_TAG               /**< VLAN tagging per (VID, port) */
#define VTSS_FEATURE_VLAN_SVL                  /**< Shared VLAN Learning */
#define VTSS_FEATURE_MAC_AGE_AUTO              /**< Automatic MAC address ageing */
#define VTSS_FEATURE_MAC_CPU_QUEUE             /**< CPU queue per MAC address */
#define VTSS_FEATURE_EEE                       /**< Energy Efficient Ethernet */
#define VTSS_FEATURE_VCAP                      /**< VCAP */
#define VTSS_FEATURE_ACL                       /**< Access Control Lists */
#define VTSS_FEATURE_ACL_V2                    /**< Access Control Lists, V2 features */
#define VTSS_FEATURE_VCL                       /**< VLAN Control Lists */
#define VTSS_FEATURE_NPI                       /**< NPI port */
#define VTSS_FEATURE_LED_POW_REDUC             /**< LED power reduction */
#define VTSS_FEATURE_VLAN_TRANSLATION          /**< VLAN Translation */
#define VTSS_FEATURE_SFLOW                     /**< sFlow feature    */
#define VTSS_FEATURE_MIRROR_CPU                /**< CPU mirroring */
#define VTSS_FEATURE_IRQ_CONTROL               /**< General IRQ support */
#define VTSS_FEATURE_SERDES_MACRO_SETTINGS     /**< Hooks for Serdes Macro configuration */
#define TESLA_ING_TS_ERRFIX                    /**< PHY Timestamp FIFO out of sync Support */
#define VIPER_B_FIFO_RESET                     /**< Viper B 1588 FIFO sync */
//#define VTSS_TS_FIFO_SYNC                      /**< PHY Timestamp FIFO OOS CHECK for correction of TSFIFO, in Link-Down Handler  */
#define VTSS_TS_FIFO_MEDIA_SWAP_SYNC           /**< PHY Timestamp FIFO OOS CHECK for correction of TSFIFO, in MEDIA i/f Swap case */
#define VTSS_PHY_TS_SPI_CLK_THRU_PPS0          /**< Use 1588_PPS0 as New SPI_CLK, applicable only to 8574-15; old SPI_CLK pin will not be used anymore */
#endif /* VTSS_ARCH_SERVAL */

#if defined(VTSS_ARCH_SERVAL_CPU)
#define VTSS_FEATURE_INTERRUPTS                /**< Port Interrupt support */
#define VTSS_FEATURE_SERDES_MACRO_SETTINGS     /**< Hooks for Serdes Macro configuration */
#define VTSS_FEATURE_SYNCE                     /**< SYNCE - L1 syncronization feature */
#define VTSS_FEATURE_SERIAL_GPIO               /**< Serial GPIO control */
#define VTSS_FEATURE_FAN                       /**< Fan control */
#define VTSS_FEATURE_PHY_TIMESTAMP             /**< PHY timestamp feature (for PTP/OAM) */
#define VTSS_FEATURE_TIMESTAMP                 /**< Packet timestamp feature (for PTP and OAM) */
#define VTSS_FEATURE_TIMESTAMP_ONE_STEP        /**< ONESTEP timestamp hardware support */
#define VTSS_FEATURE_TIMESTAMP_LATENCY_COMP    /**< Ingress and egress latency compensation hardwarce support */
#define VTSS_FEATURE_TIMESTAMP_ORG_TIME        /**< OriginTimestamp update hardware support */
#define VTSS_FEATURE_TIMESTAMP_P2P_DELAY_COMP  /**< Peer-to-peer path delay compensation hardware support */
#define VTSS_FEATURE_TIMESTAMP_ASYMMETRY_COMP  /**< Path delay asymmetry compensation hardware support */
#define VTSS_FEATURE_PTP_RS422                 /**< Support for the RS422 serial/1PPS interface */
#if !defined(VTSS_OPT_VCORE_III)
  #define VTSS_OPT_VCORE_III 1                 /**< Internal VCore-III (MIPS) CPU enabled by default */
#endif
#if VTSS_OPT_VCORE_III != 0
  #define VTSS_FEATURE_FDMA                    /**< Frame DMA */
#endif
#endif /* VTSS_ARCH_SERVAL_CPU */

#if defined(VTSS_ARCH_SERVAL_CE)
#define VTSS_FEATURE_EVC                       /**< Ethernet Virtual Connections */
#define VTSS_FEATURE_E_TREE                    /**< EVC E-Tree */
#define VTSS_FEATURE_OAM                       /**< Y.1731/IEEE802.1ag OAM */
#define VTSS_FEATURE_AFI_SWC                   /**< Switch-core-based Automatic Frame Injection */
#define VTSS_AFI_V1                            /**< AFI API version 1 */
#ifndef VTSS_OPT_AFI_OPTIMIZE_FOR_TIMERS
#define VTSS_OPT_AFI_OPTIMIZE_FOR_TIMERS 0     /**< Default to legacy SWC AFI mode, which is optimization for frame-slots. Set to 1 from app to optimize for timers */
#endif /* VTSS_OPT_AFI_OPTIMIZE_FOR_TIMERS */

#define VTSS_FEATURE_MPLS                      /**< MPLS / MPLS-TP */
#define VTSS_FEATURE_HQOS                      /**< Hierarchical Quality of Service */
#endif /* VTSS_ARCH_SERVAL_CE */

/* Cu PHY API always inclded for switch/MAC targets */
#if defined(VTSS_FEATURE_PORT_CONTROL)
#define VTSS_CHIP_CU_PHY                       /**< Cobber PHY chip */
#endif /* VTSS_FEATURE_PORT_CONTROL */

/* ================================================================= *
 *  Options
 * ================================================================= */

#ifndef VTSS_OPT_TRACE
#define VTSS_OPT_TRACE 1 /**< Trace enabled by default */
#endif /* VTSS_OPT_TRACE */

#ifdef VTSS_OPT_FDMA
  #if VTSS_OPT_FDMA && !defined(VTSS_FEATURE_FDMA)
    /* Application wants the FDMA to no avail */
    #error "FDMA is not available on this platform"
  #endif
#else
  /* Application hasn't set it, so by default we enable it whenever the
   * FDMA is supported. */
 #define VTSS_OPT_FDMA defined(VTSS_FEATURE_FDMA)
#endif
/* Now enable FDMA sub-options. */
#if VTSS_OPT_FDMA
  #ifndef VTSS_OPT_FDMA_IRQ_CONTEXT
    /* This option informs the FDMA code whether it is
     * called in interrupt context (1) or deferred interrupt
     * context (0). In the first case, it will call the
     * VTSS_OS_INTERRUPT_DISABLE()/RESTORE() functions
     * to ensure mututal exclusiveness between
     * interrupt handler and thread (user-level) code.
     * In the second case it will call the
     * VTSS_OS_SCHEDULER_LOCK()/UNLOCK() functions
     * instead.
     * Either way, the functions will only be called
     * from the thread code, not the interrupt handler
     * code (whether it's deferred or not).
     */
    #define VTSS_OPT_FDMA_IRQ_CONTEXT 0 /**< Deferred interrupt context by default */
  #endif

  /**< Use of VTSS_OPT_FDMA_VER is the preferred way to indicate which version of the FDMA API is required */
  #ifndef VTSS_OPT_FDMA_VER
    #define VTSS_OPT_FDMA_VER 2 /**< This is the default legacy mode */
  #endif
  #undef VTSS_OPT_FDMA_V2 /**< Make sure noone uses this one anymore */

  #ifndef VTSS_OPT_FDMA_DEBUG
    /* VTSS_OPT_FDMA_DEBUG:
     *   If non-zero, more run-time checks are added to the FDMA driver code.
     *   Specifically these are:
     *     a) when vtss_fdma_inj() is called, it is checked that the sum of
     *        all S/W DCB lengths match the total frame size as passed to the
     *        function.
     *     b) After a frame has been injected, it is tested that the FDMA engine
     *        has set the H/W DCB's DONE bit.
     *     c) On extraction, it is tested that the frame size indicated in the
     *        frame's IFH matches the total frame size as reported by the FDMA
     *        engine.
     */
    #define VTSS_OPT_FDMA_DEBUG 0 /**< FDMA debug disabled by default */
  #endif
#endif

/* VAUI equalization control, change value to 10 if PCB trace is more than 15 cm */
#if !defined(VTSS_OPT_VAUI_EQ_CTRL)
#define VTSS_OPT_VAUI_EQ_CTRL 6 /**< Default equalization control */
#endif /* VTSS_OPT_VAUI_EQ_CTRL */

#if !defined(VTSS_OPT_PORT_COUNT)
#define VTSS_OPT_PORT_COUNT 0 /**< Use all target ports by default */
#endif /* VTSS_OPT_PORT_COUNT */

#if !defined(VTSS_PHY_OPT_VERIPHY)
#define VTSS_PHY_OPT_VERIPHY 1 /**< VeriPHY enabled by default */
#endif /* VTSS_PHY_OPT_VERIPHY */

#if defined(VTSS_CHIP_CU_PHY)
#define VTSS_FEATURE_WARM_START                /**< Warm start */
#endif /* VTSS_CHIP_CU_PHY */

#if defined(VTSS_CHIP_10G_PHY)
#define VTSS_FEATURE_SYNCE_10G                 /**< SYNCE - L1 syncronization feature for 10G PHYs*/
#define VTSS_FEATURE_EDC_FW_LOAD               /**< 848x EDC firmware will get loaded at initilization */
#define VTSS_FEATURE_WIS                       /**< WAN interface sublayer functionality */
#define VTSS_FEATURE_WARM_START                /**< Warm start */
#define VTSS_ARCH_MALIBU                       /**< Used for Malibu-A PHY */
#define VTSS_ARCH_MALIBU_B                     /**< Used for Malibu-B PHY */
#define VTSS_ARCH_VENICE_C                     /**< Used for Venice-C PHY */
#endif /* VTSS_CHIP_10G_PHY */

// defining VTSS_10BASE_TE select 10BASE-Te settings. If not defined code will default to 10BASE-T . 
// 10BASE-Te settings select a reduced transmit amplitude that should be right in the middle of the spec. range.  
// The 10BASE-T settings will be right at the lower spec.-limit for 10BASE-T amplitude (higher than Te, but marginal to spec. the 2.2v spec.)
//#define VTSS_10BASE_TE


#if defined(VTSS_ARCH_JAGUAR_1) || defined(VTSS_ARCH_LUTON26) || defined(VTSS_ARCH_SERVAL)
#define VTSS_FEATURE_VCAP                      /**< VCAP */
#endif

#endif /* _VTSS_OPTIONS_H_ */

