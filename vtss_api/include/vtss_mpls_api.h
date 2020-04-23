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
 * \brief MPLS API
 * \details This header file describes the MPLS functions
 */

#ifndef _VTSS_MPLS_API_NEW_H_
#define	_VTSS_MPLS_API_NEW_H_

#include <vtss/api/types.h>

#if defined(VTSS_FEATURE_MPLS)

#ifdef __cplusplus
extern "C" {
#endif

/** \page mpls MPLS API Overview
 *
 * The MPLS API is made up of the following main elements:
 *
 *   - Segments
 *   - Cross-connects
 *   - Layer-2 configuration
 *
 * Together, these elements allow for the creation of MPLS label-swap LSPs,
 * PWE3 pseudo-wire head and tail ends and LSP tunnel head and tail ends.
 *
 * The available facilities are oriented towards the transportation of MPLS-
 * encapsulated Ethernet frames, including Ethernet OAM, and MPLS-encapsulated
 * MPLS OAM frames. IP is not supported by current hardware capabilities, and
 * is therefore absent from the present API.
 *
 *
 * Segment & Layer-2
 * -----------------
 * The Segment is the fundamental MPLS entity, designating either an incoming
 * unidirectional LSP (in-segment) or an outgoing unidirectional LSP (out-
 * segment).
 *
 * A segment thus holds an MPLS label, TC information as well as other details
 * necessary to configure the LSP.
 *
 * Segments may be nested: One segment may be Server to other segments, Clients.
 * Each Client segment may itself be Server to other Clients, and so on; only
 * limited by the HW capabilities.
 *
 * In order to move MPLS packets in and out of a port, a Layer-2 configuration
 * must be created; it holds the relevant L2 info (port, peer MAC, tag type and
 * VID, own MAC) enabling RX and TX.
 *
 * A segment must therefore be attached to a L2 entry in order to be able to
 * process MPLS. Obviously, only the ultimate server segment ("outermost
 * segment") can have L2 attached.
 *
 * Finally, each segment must be attached to exactly one Cross-connect; see
 * below.
 *
 *
 * Cross-connect (XC)
 * ------------------
 * Every segment must be attached to a Cross Connect (XC). The XC serves these
 * purposes:
 *
 *   - Connect an in- and out-segment, enabling a Label Swap operation (LSR)
 *   - For initiating/terminating LSPs/PWs, identify the type.
 *
 *
 * Ethernet Processing
 * -------------------
 * Ethernet payload processing is handled by EVCs configured to use MPLS
 * pseudo-wires as server layer. Thus, a typical setup for the Ethernet service
 * ingress direction:
 *
 *   - An EVC, configured with one or more ECEs to accept certain traffic and
 *     add a service-delimiting S-tag to the payload Ethernet frame
 *   - A pseudo-wire out-segment attached to an XC. This adds an inner MPLS label
 *     and an optional PW control word
 *   - An LSP tunnel out-segment attach to an XC. This adds an outer MPLS label
 *     on top of the PW label. Then the PW out-segment is attached to the LSP
 *     tunnel out-segment; it has become server to the PW
 *   - The LSP out-segment is attached to an L2 entry which provides the link-
 *     layer information required to form a valid Ethernet frame: Destination
 *     MAC, source MAC, optional VLAN tag, MPLS Ethertype, customer Ethernet frame
 *     (which includes the S-tag). FCS for the complete frame is automatically
 *     calculated and appended to the outgoing frame.
 *
 * Ethernet service egress processing is largely symmetric to ingress. Instead of
 * the EVC and out-segments adding an S-tag and labels, the tag and labels are
 * removed by ECE rules and MPLS in-segments before the resulting payload frame
 * is output.
 */


/** \page mpls
 * \section MPLS API Usage
 *
 * HW allocation
 * -------------
 * A segment will attempt to acquire HW resources when it is sufficiently
 * configured. What constitutes "sufficient" depends on many different factors,
 * detailed later. This HW allocation may fail if the underlying resources are
 * unavailable/consumed; in that case the segment will stay in this state and
 * there will be no allocation retrying except when explicitly initiated by a
 * call to ::vtss_mpls_segment_set() or one of the attachment functions (attach
 * to L2, segment, XC).
 *
 * Segment states
 * --------------
 * A segment can either be UNCONFigured, CONFigured or UP.
 *
 * UNCONF means that the segment lacks configuration in order to operate.
 * 
 * CONF means that the segment has sufficient configuration to operate, but
 * that:
 *   - either the underlying server segment isn't UP,
 *   - or the segment hasn't been able to allocate hardware resources
 * 
 * UP indicates that:
 *   - the configuration is sufficient
 *   - the underlying server (if any) is UP
 *   - all necessary HW resources (if any) have been allocated
 *
 * Please note that UP does not indicate whether the ultimately underlying L2
 * port is up or not. The MPLS API considers it a resource, not a server layer.
 *
 */

/** \page mpls
 * \section mpls_stats MPLS Statistics
 *
 * MPLS statistics include green/yellow/red/discarded frames and bytes for the
 * following types of setups and traffic:
 *
 *   - LSR LSPs. The traffic is counted on the in- and out-segments attached to an XC
 *
 * The Serval-1 hardware does not support MPLS OAM injection/extraction, nor are
 * frames handled directly by the MPLS API but instead other parts of the chip
 * API. Therefore, OAM counting must be handled by the application code running
 * on the CPU.
 *
 * Ethernet payload traffic is counted via the endpoint EVCs/ECEs available in the
 * EVC API. Endpoint PWs only carry Ethernet frames and MPLS PW OAM, and tunnel LSPs
 * only carry MPLS OAM, so the application must manually accumulate HW-based EVC
 * counters and software-based MPLS OAM counters.
 *
 */

#define VTSS_MPLS_LABEL_VALUE_DONTCARE  0xffffffff      /**< Don't-care label value */
#define VTSS_MPLS_TC_VALUE_DONTCARE     0xff            /**< Don't-care TC value*/



// Various indices and ranges.

#define VTSS_MPLS_IDX_UNDEFINED         (-1)                                                /**< Value of an undefined MPLS index */
#define VTSS_MPLS_IDX_IS_UNDEF(idx)     ((idx) == VTSS_MPLS_IDX_UNDEFINED)                  /**< Test whether an MPLS index is undefined (TRUE) or not (FALSE) */
#define VTSS_MPLS_IDX_IS_DEF(idx)       ((idx) != VTSS_MPLS_IDX_UNDEFINED)                  /**< Test whether an MPLS index is defined (TRUE) or not (FALSE) */
#define VTSS_MPLS_IDX_UNDEF(idx)        do { (idx) = VTSS_MPLS_IDX_UNDEFINED; } while (0)   /**< Set an MPLS index to undefined */

typedef u32 vtss_mpls_label_value_t;    /**< [0..2^20[ or VTSS_MPLS_LABEL_VALUE_DONTCARE for don't care/undefined */
typedef u8  vtss_mpls_tc_t;             /**< [0..7] or VTSS_MPLS_TC_VALUE_DONTCARE for don't care/undefined */
typedef u8  vtss_mpls_cos_t;            /**< [0..7] */
typedef u8  vtss_mpls_qos_t;            /**< [0..7] */
typedef i16 vtss_mpls_segment_idx_t;    /**< Index into table of vtss_mpls_segment_t */
typedef i16 vtss_mpls_xc_idx_t;         /**< Index into table of vtss_mpls_xc_t */
typedef i16 vtss_mpls_l2_idx_t;         /**< Index into table of vtss_mpls_l2_t */



/** \brief Frame tagging. Used both for ingress and egress. */
typedef enum {
    VTSS_MPLS_TAGTYPE_UNTAGGED = 0,     /**< Frame is untagged */
    VTSS_MPLS_TAGTYPE_CTAGGED  = 1,     /**< Frame is C-tagged */
    VTSS_MPLS_TAGTYPE_STAGGED  = 2      /**< Frame is S-tagged */
} vtss_mll_tagtype_t;




/** \brief Segment state. This expresses whether a segment is sufficiently
 * configured to be able to operate, and in that case, whether it has been
 * able to allocate the necessary hardware resources.
 *
 * A segment may fail to acquire HW resources for two main reasons:
 *
 *   1. The underlying resource pool is shared with other components
 *   2. The underlying resource is dynamically partitioned, meaning that
 *      the allocation limit for one allocation type is dependent on the
 *      number of allocations of other types
 */
typedef enum {
  VTSS_MPLS_SEGMENT_STATE_UNCONF,       /**< Segment is not fully configured */
  VTSS_MPLS_SEGMENT_STATE_CONF,         /**< Segment is sufficiently configured, but has not acquired HW resources */
  VTSS_MPLS_SEGMENT_STATE_UP            /**< Segment has acquired HW resources */
} vtss_mpls_segment_state_t;

/** \brief Segment status. Contains various kinds of status/state information
 * for a segment. */
typedef struct {
    vtss_mpls_segment_state_t       state;          /**< Segement state */
    BOOL                            oam_active;    /**< TRUE if OAM is configured AND valid given the current label stack AND any HW has been successfully allocated */
} vtss_mpls_segment_status_t;


/** \brief Cross-connect (XC) types.
 *
 * Note: Once an XC is created, it cannot change type.
 */
typedef enum {
    VTSS_MPLS_XC_TYPE_LSR,              /**< Label swap. In- and out-segment */
    VTSS_MPLS_XC_TYPE_LER               /**< LSP init/term. In- or out-segment */
} vtss_mpls_xc_type_t;



/** \brief DiffServ tunnel modes for TC/TTL */
typedef enum {
    VTSS_MPLS_TUNNEL_MODE_PIPE,         /**< Pipe mode */
    VTSS_MPLS_TUNNEL_MODE_SHORT_PIPE,   /**< Short Pipe mode */
    VTSS_MPLS_TUNNEL_MODE_UNIFORM       /**< Uniform mode */
} vtss_mpls_tunnel_mode_t;



/** \brief MPLS Pseudo-wire + LSP OAM types */
typedef enum {
    VTSS_MPLS_OAM_TYPE_NONE,            /**< No OAM */
    VTSS_MPLS_OAM_TYPE_VCCV1,           /**< VCCV1 PW OAM */
    VTSS_MPLS_OAM_TYPE_VCCV2,           /**< VCCV2 PW OAM */
    VTSS_MPLS_OAM_TYPE_VCCV3,           /**< VCCV3 PW OAM */
    VTSS_MPLS_OAM_TYPE_VCCV4,           /**< VCCV4 PW OAM */
    VTSS_MPLS_OAM_TYPE_GAL_MEP,         /**< LSP MEP using GAL */
    VTSS_MPLS_OAM_TYPE_GAL_MIP          /**< LSP MIP using GAL */
} vtss_mpls_oam_type_t;



/** \brief MPLS label. The TTL value is only relevant for LER out-segments. */
typedef struct {
    vtss_mpls_label_value_t value;      /**< Label value */
    vtss_mpls_tc_t          tc;         /**< Used unless TC mapping is installed */
    u8                      ttl;        /**< Used when pushing label (not for swap) */
} vtss_mpls_label_t;



#define VTSS_MPLS_QOS_TO_TC_MAP_CNT     8       /**< 8 tables */
#define VTSS_MPLS_QOS_TO_TC_ENTRY_CNT   8       /**< 8 entries per table */

#define VTSS_MPLS_TC_TO_QOS_MAP_CNT     8       /**< 8 tables */
#define VTSS_MPLS_TC_TO_QOS_ENTRY_CNT   8       /**< 8 entries per table */

/** \brief QoS-to-TC map entry */
typedef struct {
    vtss_mpls_tc_t  dp0_tc;                     /**< TC value for DP == 0. Valid range is [0..7] */
    vtss_mpls_tc_t  dp1_tc;                     /**< TC value for DP == 1. Valid range is [0..7] */
} vtss_mpls_qos_to_tc_map_entry_t;

/** \brief TC-to-(QoS,DP) map entry */
typedef struct {
    vtss_mpls_qos_t qos;                        /**< QoS value, valid range [0..7] */
    u8              dp;                         /**< DP value, valid range [0..1] */
} vtss_mpls_tc_to_qos_map_entry_t;

/** \brief TC/QoS map configuration */
typedef struct {
    vtss_mpls_qos_to_tc_map_entry_t  qos_to_tc_map[VTSS_MPLS_QOS_TO_TC_MAP_CNT][VTSS_MPLS_QOS_TO_TC_ENTRY_CNT];     /**< (QoS, DP) => TC tables. Access with [map][qos].dp0_tc (or .dp1_tc) */
    vtss_mpls_tc_to_qos_map_entry_t  tc_to_qos_map[VTSS_MPLS_TC_TO_QOS_MAP_CNT][VTSS_MPLS_TC_TO_QOS_ENTRY_CNT];     /**< TC => (QoS, DP) tables. */
} vtss_mpls_tc_conf_t;



/** \brief OAM configuration. */
typedef struct {
    vtss_mpls_oam_type_t        type;           /**< OAM type */
} vtss_mpls_oam_conf_t;



/** \brief Layer-2 configuration. */
typedef struct {
    vtss_port_no_t              port;           /**< Port number */
    vtss_mac_t                  peer_mac;       /**< MAC address of peer MPLS entity */
    vtss_mac_t                  self_mac;       /**< MAC address of this MPLS entity */
    vtss_mll_tagtype_t          tag_type;       /**< Tag type */
    vtss_vid_t                  vid;            /**< VLAN ID, if tag_type isn't untagged */
    vtss_tagprio_t              pcp;            /**< VLAN PCP; only relevant for tagged egress */
    vtss_dei_t                  dei;            /**< VLAN DEI; only relevant for tagged egress */
    BOOL                        section_oam;    /**< TRUE => accept incoming segment OAM. Note: Only supports ethertype for downsteam-assigned labels */
} vtss_mpls_l2_t;



/** \brief Pseudo-wire configuration. */
typedef struct {
    BOOL                        is_pw;          /**< TRUE == this segment is used for a PW */
    BOOL                        process_cw;     /**< TRUE for in-segment: Check and pop CW. TRUE for out-segment: Add CW in cw */
    u32                         cw;             /**< Control Word to use for out-segments */
} vtss_mpls_pw_conf_t;



/** \brief Segment configuration.
 *
 * All indices are VTSS_MPLS_IDX_UNDEFINED if unused/uninitialized.
 */
typedef struct {
    BOOL                        is_in;               /**< [RO] TRUE == in-segment; FALSE == out-segment */
    BOOL                        e_lsp;               /**< [RW] TRUE == E-LSP; FALSE == L-LSP */
    vtss_mpls_cos_t             l_lsp_cos;           /**< [RW] Fixed COS: Ingress TC is mapped to this value for L-LSPs */
    i8                          tc_qos_map_idx;      /**< [RW] In-segment:  Ingress TC-to-(QoS,DP) map: L-LSP: Only DP is mapped; E-LSP: Both QoS and DP are mapped. VTSS_MPLS_IDX_UNDEF = no mapping.
                                                               Out-segment: Egress (QoS,DP)-to-TC map. VTSS_MPLS_IDX_UNDEF = no mapping */
    vtss_mpls_l2_idx_t          l2_idx;              /**< [RO] L2 index */
    vtss_mpls_label_t           label;               /**< [RW] Label value, TC, TTL */
    BOOL                        upstream;            /**< [RW] TRUE == Label is upstream-assigned */
#if defined(VTSS_FEATURE_HQOS)
    vtss_hqos_id_t              hqos_id;             /**< [RW] Hierarchical QoS ID or VTSS_HQOS_ID_NONE for no HQoS. Only valid for LSR in-segments */
#endif /* VTSS_FEATURE_HQOS */
    vtss_mpls_pw_conf_t         pw_conf;             /**< [RW] Pseudo-wire configuration */
    vtss_mpls_oam_conf_t        oam_conf;            /**< [RW] OAM configuration */
    vtss_mpls_xc_idx_t          xc_idx;              /**< [RO] Index of XC using this segment */
    vtss_mpls_segment_idx_t     server_idx;          /**< [RO] Server (outer) segment */
} vtss_mpls_segment_t;



/** \brief Cross Connect function for unidirectional LSP/PW.
 * \details
 * A transit (label-swapped) LSP has both an in- and out-segment
 * A terminating LSP/PW only has an in-segment.
 * An initiating LSP/PW only has an out-segment.
 *
 * A segment which isn't attached to an XC cannot transport traffic.
 *
 * Most fields should not be set directly; treat them as R/O and use the
 * appropriate vtss_mpls_...() functions for manipulation.
 *
 * All indices are VTSS_MPLS_IDX_UNDEFINED if unused/uninitialized.
 */
typedef struct {
    vtss_mpls_xc_type_t         type;                       /**< [RO] XC type*/
    vtss_mpls_tunnel_mode_t     tc_tunnel_mode;             /**< [RW] DiffServ tunneling mode for TC. Default: VTSS_MPLS_TUNNEL_MODE_PIPE */
    vtss_mpls_tunnel_mode_t     ttl_tunnel_mode;            /**< [RW] DiffServ tunneling mode for TTL. Default: VTSS_MPLS_TUNNEL_MODE_PIPE */
    vtss_mpls_segment_idx_t     in_seg_idx;                 /**< [RO] Index of in-segment */
    vtss_mpls_segment_idx_t     out_seg_idx;                /**< [RO] Index of out-segment */
    vtss_isdx_t                 flow_handle;                /**< [RO] Opaque and unique flow handle for MPLS frames extracted to CPU; only valid when in-seg is attached and state UP. Value is ignored when writing to the XC config. NOTE: Value may change if in-seg goes out of state UP and back. */
} vtss_mpls_xc_t;



/*----------------------------------------------------------------------------
 *  Layer-2 functions
 *--------------------------------------------------------------------------*/

/** \brief Allocate L2 entry.
 *
 * The entry contains default values after allocation: Indices are undefined
 * and all other values are zero.
 *
 * \param inst      [IN]     Instance.
 * \param idx       [OUT]    Index of newly allocated entry.
 *
 * \return VTSS_RC_OK when successful; VTSS_RC_ERROR if no entries available.
 */
vtss_rc vtss_mpls_l2_alloc(vtss_inst_t                inst,
                           vtss_mpls_l2_idx_t * const idx);



/** \brief Free L2 entry.
 *
 * \param inst      [IN]     Instance.
 * \param idx       [IN]     Index of entry to free.
 *
 * \return VTSS_RC_OK when successful; VTSS_RC_ERROR if entry is still in use.
 */
vtss_rc vtss_mpls_l2_free(vtss_inst_t              inst,
                          const vtss_mpls_l2_idx_t idx);



/** \brief Get L2 entry.
 *
 * Note that it is possible _but not recommended_ to retrieve entries that
 * haven't been allocated. In that case the contents are undefined.
 *
 * \param inst      [IN]     Instance.
 * \param idx       [IN]     Index of entry to retrieve.
 * \param l2        [OUT]    Entry.
 *
 * \return VTSS_RC_OK when successful; VTSS_RC_ERROR if index is out-of-range.
 */
vtss_rc vtss_mpls_l2_get(vtss_inst_t              inst,
                         const vtss_mpls_l2_idx_t idx,
                         vtss_mpls_l2_t * const   l2);



/** \brief Set L2 entry.
 *
 * Note that it is possible _but not recommended_ to set entries that
 * haven't been allocated. The results are undefined and may lead to
 * system malfunction.
 *
 * If segments are attached, they will be asked to refresh themselves. This
 * will not cause attempted allocation of new HW resources for segments in
 * state VTSS_MPLS_SEGMENT_STATE_CONF (i.e. segments which have not earlier on
 * been able acquire HW resources).
 *
 * \param inst      [IN]     Instance.
 * \param idx       [IN]     Index of entry to retrieve.
 * \param l2        [IN]    Entry.
 *
 * \return VTSS_RC_OK when successful; VTSS_RC_ERROR if index is out-of-range
 * or some of the new values are invalid.
 */
vtss_rc vtss_mpls_l2_set(vtss_inst_t                  inst,
                         const vtss_mpls_l2_idx_t     idx,
                         const vtss_mpls_l2_t * const l2);



/** \brief Attach segment to L2 entry.
 *
 * If the segment is in state VTSS_MPLS_SEGMENT_STATE_CONF after attachment, it
 * will try to go to state VTSS_MPLS_SEGMENT_STATE_UP, i.e. allocate HW
 * resources.
 *
 * If the segment reaches UP and is a server it will try to (recursively) bring
 * each client UP as well.
 *
 * HW allocation may succeed or fail; use ::vtss_mpls_segment_status_get() to
 * determine the outcome.
 *
 * \param inst      [IN]     Instance.
 * \param l2_idx    [IN]     L2 index.
 * \param seg_idx   [IN]     Segment index to attach.
 *
 * \return VTSS_RC_OK when successful; VTSS_RC_ERROR if parameters are invalid.
 */
vtss_rc vtss_mpls_l2_segment_attach(vtss_inst_t                   inst,
                                    const vtss_mpls_l2_idx_t      l2_idx,
                                    const vtss_mpls_segment_idx_t seg_idx);



/** \brief Detach segment from L2 entry.
 *
 * If the segment is a server it will (recursively) bring all clients out of
 * UP state (and usually back to CONF, but don't depend on that behavior.).
 *
 * \param inst      [IN]     Instance.
 * \param seg_idx   [IN]     Segment index to detach.
 *
 * \return VTSS_RC_OK when successful; VTSS_RC_ERROR if parameters are invalid.
 */
vtss_rc vtss_mpls_l2_segment_detach(vtss_inst_t                   inst,
                                    const vtss_mpls_segment_idx_t seg_idx);



/*----------------------------------------------------------------------------
 *  Segment functions
 *--------------------------------------------------------------------------*/

/** \brief Allocate Segment entry.
 *
 * The entry contains default values after allocation: Indices are undefined
 * and all other values except \em is_in are zero/FALSE.
 *
 * \param inst      [IN]     Instance.
 * \param is_in     [IN]     TRUE: Allocate in-segment; FALSE: out-segment.
 * \param idx       [OUT]    Index of newly allocated entry.
 *
 * \return VTSS_RC_OK when successful; VTSS_RC_ERROR if no entries available.
 */
vtss_rc vtss_mpls_segment_alloc(vtss_inst_t                     inst,
                                const BOOL                      is_in,
                                vtss_mpls_segment_idx_t * const idx);



/** \brief Free segment entry.
 *
 * \param inst      [IN]     Instance.
 * \param idx       [IN]     Index of entry to free.
 *
 * \return VTSS_RC_OK when successful; VTSS_RC_ERROR if entry is still in use
 * (attached to XC, attached to server, or has clients).
 */
vtss_rc vtss_mpls_segment_free(vtss_inst_t                   inst,
                               const vtss_mpls_segment_idx_t idx);



/** \brief Get segment entry.
 *
 * Note that it is possible _but not recommended_ to retrieve entries that
 * haven't been allocated. In that case the contents are undefined.
 *
 * \param inst      [IN]     Instance.
 * \param idx       [IN]     Index of entry to retrieve.
 * \param seg       [OUT]    Entry.
 *
 * \return VTSS_RC_OK when successful; VTSS_RC_ERROR if index is out-of-range.
 */
vtss_rc vtss_mpls_segment_get(vtss_inst_t                   inst,
                              const vtss_mpls_segment_idx_t idx,
                              vtss_mpls_segment_t * const   seg);



/** \brief Set segment entry.
 *
 * Note that it is possible _but not recommended_ to set entries that
 * haven't been allocated. The results are undefined and may lead to
 * system malfunction.
 *
 * Several values in a ::vtss_mpls_segment_t are write-once: Once set they
 * cannot be changed. RO/RW for others depend on the current state of the
 * segment, in particular whether it is attached to something or is a server.
 *
 * If the segment is sufficiently configured, but has been unable to acquire HW
 * resources, the allocation can be re-tried by calling set() again.
 *
 * \param inst      [IN]     Instance.
 * \param idx       [IN]     Index of entry to retrieve.
 * \param seg       [IN]    Entry.
 *
 * \return VTSS_RC_OK when successful; VTSS_RC_ERROR if index is out-of-range
 * or some of the new values are invalid. NOTE that success does not imply that
 * the segment is up. Use ::vtss_mpls_segment_status_get() to determine resulting
 * state.
 */
vtss_rc vtss_mpls_segment_set(vtss_inst_t                       inst,
                              const vtss_mpls_segment_idx_t     idx,
                              const vtss_mpls_segment_t * const seg);



/** \brief Get current segment status.
 *
 * \param inst      [IN]     Instance.
 * \param idx       [IN]     Index of segment to retrieve state for.
 * \param status    [OUT]    Current segment status.
 *
 * \return VTSS_RC_OK when successful; VTSS_RC_ERROR if index is out-of-range.
 */
vtss_rc vtss_mpls_segment_status_get(vtss_inst_t                        inst,
                                     const vtss_mpls_segment_idx_t      idx,
                                     vtss_mpls_segment_status_t * const status);



/** \brief Attach segment to server segment.
 *
 * If the segment is in state VTSS_MPLS_SEGMENT_STATE_CONF after attachment, it
 * will try to go to state VTSS_MPLS_SEGMENT_STATE_UP, i.e. allocate HW
 * resources. (This requires that the server is UP, too.)
 *
 * If the segment reaches UP and is a server it will try to (recursively) bring
 * each client UP as well.
 *
 * HW allocation may succeed or fail; use ::vtss_mpls_segment_status_get() to
 * determine the outcome.
 *
 * \param inst      [IN]     Instance.
 * \param idx       [IN]     Segment index to attach.
 * \param srv_idx   [IN]     Server segment index.
 *
 * \return VTSS_RC_OK when successful; VTSS_RC_ERROR if parameters are invalid.
 */
vtss_rc vtss_mpls_segment_server_attach(vtss_inst_t                   inst,
                                        const vtss_mpls_segment_idx_t idx,
                                        const vtss_mpls_segment_idx_t srv_idx);



/** \brief Detach segment from server segment.
 *
 * This will cause the segment to go out of UP state.
 *
 * If the segment is itself a server it will (recursively) bring all clients
 * out of UP state (and usually back to CONF, but don't depend on that
 * behavior.).
 *
 * \param inst      [IN]     Instance.
 * \param seg_idx   [IN]     Segment index to detach from server.
 *
 * \return VTSS_RC_OK when successful; VTSS_RC_ERROR if parameters are invalid.
 */
vtss_rc vtss_mpls_segment_server_detach(vtss_inst_t                   inst,
                                        const vtss_mpls_segment_idx_t seg_idx);



/*----------------------------------------------------------------------------
 *  XC functions
 *--------------------------------------------------------------------------*/

/** \brief Allocate XC entry.
 *
 * The entry contains default values after allocation: Indices are undefined
 * and all other values except \em type are zero/FALSE.
 *
 * \param inst      [IN]     Instance.
 * \param type      [IN]     XC type.
 * \param idx       [OUT]    Index of newly allocated entry.
 *
 * \return VTSS_RC_OK when successful; VTSS_RC_ERROR if no entries available.
 */
vtss_rc vtss_mpls_xc_alloc(vtss_inst_t                inst,
                           const vtss_mpls_xc_type_t  type,
                           vtss_mpls_xc_idx_t * const idx);



/** \brief Free XC entry.
 *
 * \param inst      [IN]     Instance.
 * \param idx       [IN]     Index of entry to free.
 *
 * \return VTSS_RC_OK when successful; VTSS_RC_ERROR if entry is still in use.
 */
vtss_rc vtss_mpls_xc_free(vtss_inst_t              inst,
                          const vtss_mpls_xc_idx_t idx);



/** \brief Get XC entry.
 *
 * Note that it is possible _but not recommended_ to retrieve entries that
 * haven't been allocated. In that case the contents are undefined.
 *
 * \param inst      [IN]     Instance.
 * \param idx       [IN]     Index of entry to retrieve.
 * \param xc        [OUT]    Entry.
 *
 * \return VTSS_RC_OK when successful; VTSS_RC_ERROR if index is out-of-range.
 */
vtss_rc vtss_mpls_xc_get(vtss_inst_t              inst,
                         const vtss_mpls_xc_idx_t idx,
                         vtss_mpls_xc_t * const   xc);



/** \brief Set XC entry.
 *
 * Note that it is possible _but not recommended_ to set entries that
 * haven't been allocated. The results are undefined and may lead to
 * system malfunction.
 *
 * \param inst      [IN]     Instance.
 * \param idx       [IN]     Index of entry to retrieve.
 * \param xc        [IN]    Entry.
 *
 * \return VTSS_RC_OK when successful; VTSS_RC_ERROR if index is out-of-range
 * or some of the new values are invalid.
 */
vtss_rc vtss_mpls_xc_set(vtss_inst_t                  inst,
                         const vtss_mpls_xc_idx_t     idx,
                         const vtss_mpls_xc_t * const xc);



/** \brief Attach segment to XC.
 *
 * Note: If the segment contains inconsistent configuration, the attach operation
 * will fail.
 *
 * If the segment is in state VTSS_MPLS_SEGMENT_STATE_CONF after attachment, it
 * will try to go to state VTSS_MPLS_SEGMENT_STATE_UP, i.e. allocate HW
 * resources.
 *
 * If the segment reaches UP and is a server it will try to (recursively) bring
 * each client UP as well.
 *
 * HW allocation may succeed or fail; use ::vtss_mpls_segment_status_get() to
 * determine the outcome.
 *
 * \param inst      [IN]     Instance.
 * \param xc_idx    [IN]     XC index.
 * \param seg_idx   [IN]     Segment index to attach.
 *
 * \return VTSS_RC_OK when successful; VTSS_RC_ERROR if parameters are invalid.
 */
vtss_rc vtss_mpls_xc_segment_attach(vtss_inst_t                   inst,
                                    const vtss_mpls_xc_idx_t      xc_idx,
                                    const vtss_mpls_segment_idx_t seg_idx);



/** \brief Detach segment from XC.
 *
 * This will cause the segment to go out of UP state.
 *
 * If the segment is itself a server it will (recursively) bring all clients
 * out of UP state (and usually back to CONF, but don't depend on that
 * behavior.).
 *
 * \param inst      [IN]     Instance.
 * \param seg_idx   [IN]     Segment index to detach from server.
 *
 * \return VTSS_RC_OK when successful; VTSS_RC_ERROR if parameters are invalid.
 */
vtss_rc vtss_mpls_xc_segment_detach(vtss_inst_t                   inst,
                                    const vtss_mpls_segment_idx_t seg_idx);



/*----------------------------------------------------------------------------
 *  TC config functions
 *--------------------------------------------------------------------------*/

/** \brief Retrieve current global TC config.
 *
 * \param inst      [IN]     Instance.
 * \param conf      [OUT]    Configuration.
 *
 * \return Return code.
 */
vtss_rc vtss_mpls_tc_conf_get(vtss_inst_t                 inst,
                              vtss_mpls_tc_conf_t * const conf);



/** \brief Set global TC config.
 *
 * NOTE: conf.map[0] is pre-configured and cannot be altered. It provides
 * an identity mapping.
 *
 * \param inst      [IN]     Instance.
 * \param conf      [IN]     Configuration.
 *
 * \return VTSS_RC_OK when successful; VTSS_RC_ERROR if parameters are invalid.
 */
vtss_rc vtss_mpls_tc_conf_set(vtss_inst_t                       inst,
                              const vtss_mpls_tc_conf_t * const conf);



/*----------------------------------------------------------------------------
 *  Statistics functions
 *--------------------------------------------------------------------------*/

/** \brief Retrieve XC counters.
 *
 * \param inst      [IN]     Instance.
 * \param xc_idx    [IN]     XC index.
 * \param counters  [OUT]    XC counters. Contents are undefined in case of error return code.
 *
 * \return Return code. VTSS_RC_ERROR if counters aren't available/valid.
 */
vtss_rc vtss_mpls_xc_counters_get(vtss_inst_t                     inst,
                                  vtss_mpls_xc_idx_t              xc_idx,
                                  vtss_mpls_xc_counters_t * const counters);

/**
 * \brief Clear XC counters.
 *
 * \param inst      [IN]     Instance.
 * \param xc_idx    [IN]     XC index.
 *
 * \return Return code. VTSS_RC_ERROR if counters aren't available/valid.
 **/
vtss_rc vtss_mpls_xc_counters_clear(vtss_inst_t        inst,
                                    vtss_mpls_xc_idx_t xc_idx);


#ifdef __cplusplus
}
#endif
#endif	/* VTSS_FEATURE_MPLS */
#endif	/* _VTSS_MPLS_API_NEW_H_ */
