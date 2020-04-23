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

#ifndef _VTSS_PACKET_STATE_H_
#define _VTSS_PACKET_STATE_H_

#if defined(VTSS_FEATURE_PACKET)
typedef struct {
    /* CIL function pointers */
    vtss_rc (*rx_conf_set)(struct vtss_state_s *vtss_state);
    vtss_rc (*rx_frame_get)(struct vtss_state_s *vtss_state,
                            const vtss_packet_rx_queue_t  queue_no,
                            vtss_packet_rx_header_t       *const header,
                            u8                            *const frame,
                            const u32                     length);
    vtss_rc (*rx_frame_get_raw)(struct vtss_state_s *vtss_state,
                                u8                  *const data,
                                const u32           buflen,
                                u32                 *const ifhlen,
                                u32                 *const frmlen);
    vtss_rc (*rx_frame_discard)(struct vtss_state_s *vtss_state,
                                const vtss_packet_rx_queue_t  queue_no);
    vtss_rc (*tx_frame_port)(struct vtss_state_s *vtss_state,
                             const vtss_port_no_t  port_no,
                             const u8              *const frame,
                             const u32             length,
                             const vtss_vid_t      vid);
    vtss_rc (*tx_frame_ifh)(struct vtss_state_s *vtss_state,
                            const vtss_packet_tx_ifh_t *const ifh,
                            const u8              *const frame,
                            const u32             length);
    vtss_rc (*rx_hdr_decode)(const struct vtss_state_s   *const state,
                             const vtss_packet_rx_meta_t *const meta,
                             const u8                     hdr[VTSS_PACKET_HDR_SIZE_BYTES],
                                   vtss_packet_rx_info_t *const info);
    vtss_rc (*tx_hdr_encode)(struct vtss_state_s   *const state,
                             const vtss_packet_tx_info_t *const info,
                                   u8                    *const bin_hdr,
                                   u32                   *const bin_hdr_len);
#if defined(VTSS_FEATURE_VSTAX)
    vtss_rc (*vstax_header2frame)(const struct vtss_state_s     *const state,
                                  const vtss_port_no_t          port_no,
                                  const vtss_vstax_tx_header_t  *const header,
                                  u8                            *const frame);
    vtss_rc (*vstax_frame2header)(const struct vtss_state_s     *const state,
                                  const u8                  *const frame,
                                  vtss_vstax_rx_header_t    *const header);
#endif /* VTSS_FEATURE_VSTAX */

#if defined(VTSS_FEATURE_NPI)
    vtss_rc (*npi_conf_set)(struct vtss_state_s *vtss_state, const vtss_npi_conf_t *const conf);
#if defined(VTSS_FEATURE_FDMA) && VTSS_OPT_FDMA
    // Only need to publish function if the FDMA is included in the build.
    // Will check if queue is currently being redirected to NPI port and if so just cache the desired chip_port.
    // Otherwise it will cache and set the chip_port to the chip.
    vtss_rc (*fdma_qu_redirect_set)(struct vtss_state_s *vtss_state, vtss_packet_rx_queue_t qu, vtss_phys_port_no_t chip_port);
#endif /* VTSS_FEATURE_FDMA  && VTSS_OPT_FDMA */
#endif /* VTSS_FEATURE_NPI */
    
    vtss_rc (*dma_conf_set)(struct vtss_state_s *vtss_state, const vtss_packet_dma_conf_t *const conf);
    vtss_rc (*dma_offset)(struct vtss_state_s *vtss_state, BOOL extraction, u32 *offset);

#if defined(VTSS_FEATURE_AFI_SWC)
    vtss_rc (*afi_alloc)(struct vtss_state_s *vtss_state, vtss_afi_frm_dscr_t *const dscr, vtss_afi_id_t *const id);
    vtss_rc (*afi_free)(struct vtss_state_s *vtss_state, vtss_afi_id_t id);
#endif /* defined(VTSS_FEATURE_AFI_SWC) */

    /* Configuration/state */
    u32                        rx_queue_count;
    vtss_packet_rx_conf_t      rx_conf;
#if defined(VTSS_FEATURE_PACKET_PORT_REG)
    vtss_packet_rx_port_conf_t rx_port_conf[VTSS_PORT_ARRAY_SIZE];
#endif /* VTSS_FEATURE_PACKET_PORT_REG */
#if defined(VTSS_FEATURE_NPI)
    vtss_npi_conf_t            npi_conf;
#if defined(VTSS_ARCH_JAGUAR_2)
    // Desired FDMA redirect port for queue a given Rx queue.
    vtss_phys_port_no_t        default_qu_redirect[VTSS_PACKET_RX_QUEUE_CNT];
#endif /* defined(VTSS_ARCH_JAGUAR_2) */
#endif /* VTSS_FEATURE_NPI */
    vtss_packet_dma_conf_t     dma_conf;

    /* RX IFH Size */
    unsigned int               rx_ifh_size;

} vtss_packet_state_t;

vtss_rc vtss_packet_inst_create(struct vtss_state_s *vtss_state);
vtss_rc vtss_cmn_logical_to_chip_port_mask(const struct vtss_state_s *const state,
                                                 u64                 logical_port_mask,
                                                 u64                 *chip_port_mask,
                                                 vtss_chip_no_t      *chip_no,
                                                 vtss_port_no_t      *stack_port_no,
                                                 u32                 *port_cnt,
                                                 vtss_port_no_t      *port_no);

vtss_port_no_t vtss_cmn_chip_to_logical_port(const struct vtss_state_s       *const state,
                                             const vtss_chip_no_t      chip_no,
                                             const vtss_phys_port_no_t chip_port);

vtss_rc vtss_cmn_packet_hints_update(const struct vtss_state_s          *const state,
                                     const vtss_trace_group_t           trc_grp,
                                     const vtss_etype_t                 etype,
                                           vtss_packet_rx_info_t *const info);

void vtss_packet_debug_print(struct vtss_state_s *vtss_state,
                             const vtss_debug_printf_t pr,
                             const vtss_debug_info_t   *const info);

#endif /* VTSS_FEATURE_PACKET */

#endif /* _VTSS_PACKET_STATE_H_ */
