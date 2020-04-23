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
 * \file vtss_afi_api.h
 * \brief AFI API
 * \details This header file describes Automatic Frame Injector functions.
 */

#ifndef _VTSS_AFI_API_H_
#define _VTSS_AFI_API_H_

#include <vtss/api/options.h>

#if defined(VTSS_FEATURE_AFI_SWC)

#include <vtss/api/types.h>

// Define checks
#if !defined(VTSS_AFI_V1) && !defined(VTSS_AFI_V2)
#error Neither VTSS_AFI_V1 nor VTSS_AFI_V2 defined
#endif

#if defined(VTSS_AFI_V1) && defined(VTSS_AFI_V2)
#error Both VTSS_AFI_V1 and VTSS_AFI_V2 defined
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if defined(VTSS_AFI_V2)

// Chip specific properties
#if defined(VTSS_ARCH_JAGUAR_2_B) || defined(VTSS_ARCH_JAGUAR_2_C)
#define VTSS_AFI_SLOW_INJ_CNT     4096           /**< Maximum number of simultaneous slow injections */
#define VTSS_AFI_FAST_INJ_BPS_MAX 10000000000ULL /**< Max rate in bits per second: 10Gbps (including IFG and preamble) */
#endif /* VTSS_ARCH_JAGUAR_2_B */

#if defined(VTSS_ARCH_SERVAL_T)
#define VTSS_AFI_SLOW_INJ_CNT     2048          /**< Maximum number of simultaneous slow injections */
#define VTSS_AFI_FAST_INJ_BPS_MAX 5000000000ULL /**< Max rate in bits per second: 5Gbps (including IFG and preamble) */
#endif /* VTSS_ARCH_SERVAL_T */

#define VTSS_AFI_SLOW_INJ_FPM_MAX (60 * 19230) /**< Maximum number of frames per minute supported by slow injections */

/** \page Automatic Frame Injector (AFI)
 *
 * Two types of AFI injections are supported:
 * a) Fast injections
 *    Injection of a sequence of frames consisting of one or more frames with rates of up to several Gbps.
 *    IFG proportional to frame size are inserted between frames.
 *    This is known as Delay-Triggered Injections (DTI).
 * b) Slow injections
 *    Typically used for injecting single frames with an interval of ~3.3ms or slower.
 *    This is known as Timer-Triggered Injections (TTI).
 *
 * Fast injections are used as follows:
 * 1) Allocate resources identified by a 'fastid' using vtss_afi_fast_inj_alloc().
 * 2) Transmit frame with IFH.AFI_INJ = 1.
 * 3) Call vtss_afi_fast_inj_frm_hijack() to configure the frame for injection.
 *    Repeat steps 2-3 to configure a sequence of frames for injection.
 * 4) Call vtss_afi_fast_inj_start() to select bandwidth and start injection.
 * 5) Call vtss_afi_fast_inj_stop() to stop injection.
 *    vtss_afi_fast_inj_seq_cnt_get() can be called to check the remaining number
 *    of injections.
 *    Repeat 4-5 as desired.
 *    Note that fast injections, which are started with seq_cnt set to a
 *    non-zero value must also be stopped with vtss_afi_fast_inj_stop() before
 *    it can be started again.
 * 6) Call vtss_afi_fast_inj_free() to free allocated resources.
 *
 * Slow injections are used as follows:
 * 1) Allocate resources identified by a 'slowid' using vtss_afi_slow_inj_alloc().
 * 2) Transmit frame with IFH.AFI_INJ=1.
 * 3) Call vtss_afi_slow_inj_frm_hijack() to configure the frame for injection.
 * 4) Call vtss_afi_slow_inj_start() to start injection.
 * 5) Call vtss_afi_slow_inj_stop() to stop injection.
 *    Repeat 4-5 as desired.
 * 6) Call vtss_afi_slow_inj_free() to free allocated resources.
 *
 * During link down, all injections (slow as well as fast) are automatically stopped
 * and upon link up, injections are restarted.
 *
 * Maximum one frame (with AFI_INJ=1) can be setup at a time. I.e. another frame
 * must not be injected with AFI_INJ=1 until the previous such frame has been hijacked
 * using vtss_afi_slow_inj_frm_hijack() or vtss_afi_fast_inj_frm_hijack().
 */

// TOETBD - Jaguar2a:
// Note that if port is disabled when freeing (slow or fast), then AFI
// API must do the following:
// a) Disable all other injections on same port.
// b) Enable injections for port (FRM_OUT_MAX>0)
// c) Configure removals.
// d) Wait for removal injections to complete.
// e) Disable injections for port (FRM_OUT_MAX=0)
// f) Reenable the injections, which were disable in a).
//
// With PORT_TBL:PORT_CFG.FRM_RM_ONLY this can be done simpler for ServalT.

// ========================================
// Fast injections
// ----------------------------------------

/**
 * Maximum number of frames in fast injection sequence.
 * The recommended default EMIX in ITU-T Y.1564 consists of 5 frames
 * of sizes 1518, 1518, 1024, 64, 64.
 */
#define VTSS_AFI_FAST_INJ_FRM_CNT_MAX 8

#if defined(VTSS_ARCH_JAGUAR_2_B) || defined(VTSS_ARCH_JAGUAR_2_C)
#define VTSS_AFI_FAST_INJ_CNT 32 /**< Maximum number of simultaneous fast injections */
#elif defined(VTSS_ARCH_SERVAL_T)
#define VTSS_AFI_FAST_INJ_CNT 16 /**< Maximum number of simultaneous fast injections */
#else
#error "Unsupported architecture"
#endif

typedef u32 vtss_afi_fastid_t; /**< Opaque type for IDs of fast injections */

/**
 * Structure defining properties of a fast injection.
 */
typedef struct {
    /**
     * [IN]
     * Port number onto which the frame sequence shall be transmitted.
     *
     * Set to VTSS_PORT_NO_NONE to use Virtual Device #1 (VD1) (for Up-MEPs).
     *
     * The reason that the destination port number must be conveyed
     * to the AFI in addition to conveying it in the IFH is that
     * once the AFI starts injection, it doesn't have access to the IFH.
     */
    vtss_port_no_t port_no;

    /**
     * [IN]
     * Priority on which the frame sequence shall be transmitted.
     *
     * Valid values are in the range [0; VTSS_PRIO_END] and
     * [VTSS_PRIO_SUPER; VTSS_PRIO_SUPER + 1].
     */
    vtss_prio_t prio;

    /**
     * [IN]
     * Number of frames in sequence, i.e. number of frames to be hijacked
     * using vtss_afi_fast_inj_frm_hijack().
     * Maximum is VTSS_AFI_FAST_INJ_FRM_CNT_MAX.
     */
    u32 frm_cnt;
} vtss_afi_fast_inj_alloc_cfg_t;

/**
 * \brief Allocate AFI fast injection resource
 *
 * \param inst   [IN]  Target instance reference.
 * \param cfg    [IN]  Injection descriptor.
 * \param fastid [OUT] ID used for referencing the allocated resource.
 *
 * \return Return code.
 **/
vtss_rc vtss_afi_fast_inj_alloc(const vtss_inst_t                          inst,
                                const vtss_afi_fast_inj_alloc_cfg_t *const cfg,
                                      vtss_afi_fastid_t             *const fastid);

/**
 * \brief Free AFI fast injection resource
 *
 * Before resources are freed, fast injection must be stopped.
 *
 * \param inst   [IN] Target instance reference.
 * \param fastid [IN] Fast injection ID.
 *
 * \return Return code.
 **/
vtss_rc vtss_afi_fast_inj_free(const vtss_inst_t       inst,
                                     vtss_afi_fastid_t fastid);

/**
 * Structure needed when hijacking frame.
 */
typedef struct {
    /**
     * [IN]
     * Frame size in bytes.
     * frm_size is only used for calculating bandwidth and need not match
     * the actual frame size. If IFG and preamble shall be included in the
     * bandwidth calculation, then this must be added to frm_size.
     */
    u32 frm_size;
} vtss_afi_fast_inj_frm_cfg_t;

/**
 * \brief Setup frame for fast injection.
 *
 * When a sequence of frames is configured, then the frame order
 * is the order in which frames are setup.
 *
 * \param inst   [IN] Target instance reference.
 * \param fastid [IN] Fast injection ID.
 * \param cfg    [IN] Frame setup configuration.
 *
 * \return Return code.
 **/
vtss_rc vtss_afi_fast_inj_frm_hijack(const vtss_inst_t                        inst,
                                           vtss_afi_fastid_t                  fastid,
                                     const vtss_afi_fast_inj_frm_cfg_t *const cfg);

/**
 * Structure defining properties of a fast injection.
 */
typedef struct {
    /**
     * [IN]
     * Requested injection rate. Bits per second.
     * Maximum value is VTSS_AFI_FAST_INJ_BPS_MAX.
     * bps = 0 => Injection at line speed (for ports with speed <= VTSS_AFI_FAST_INJ_BPS_MAX).
     */
    u64 bps;

    /**
     * [IN]
     * Number of sequences to inject. 0 => inject until stopped.
     * Maximum value is 2^31-1
     */
    u32 seq_cnt;

    /**
     * [OUT]
     * Actual injection rate. Bits per second.
     * bps_actual is always <= bps.
     * For rates close to VTSS_AFI_FAST_INJ_BPS_MAX precision is 1%, or better.
     * For lower rates precision is better than 0.1%.
     * If bps is set to 0, then bps_actual is also set to 0.
     */
    u64 bps_actual;
} vtss_afi_fast_inj_start_cfg_t;

/**
 * \brief Start fast injection.
 *
 * \param inst   [IN]    Target instance reference.
 * \param fastid [IN]    Fast injection ID.
 * \param cfg    [INOUT] Fast injection configuration.
 *
 * \return Return code.
 **/
vtss_rc vtss_afi_fast_inj_start(const vtss_inst_t                          inst,
                                      vtss_afi_fastid_t                    fastid,
                                      vtss_afi_fast_inj_start_cfg_t *const cfg);

/**
 * \brief Stop fast injection.
 *
 * \param inst   [IN] Target instance reference.
 * \param fastid [IN] Fast injection ID.
 *
 * \return Return code.
 **/
vtss_rc vtss_afi_fast_inj_stop(const vtss_inst_t       inst,
                                     vtss_afi_fastid_t fastid);

/**
 * \brief Get seq_cnt for fast injection.
 *        If seq_cnt is set to 0 in vtss_afi_fast_inj_start() then the value
 *        read will be the number of frames injected.
 *        Otherwise the value read will be the remaining number of sequences
 *        to inject.
 *
 * \param inst    [IN]  Target instance reference.
 * \param fastid  [IN]  Fast injection ID.
 * \param seq_cnt [OUT] Sequence/frame count read.
 *
 * \return Return code.
 **/
vtss_rc vtss_afi_fast_inj_seq_cnt_get(const vtss_inst_t              inst,
                                            vtss_afi_fastid_t        fastid,
                                            u32               *const seq_cnt);

// ========================================
// Slow injections
// ----------------------------------------
typedef u32 vtss_afi_slowid_t; /**< Opaque type for IDs of slow injections */

/**
 * Structure defining properties of a slow injection.
 */
typedef struct {
    /**
     * [IN]
     * Port number onto which the frame shall be transmitted periodically.
     *
     * Set to VTSS_PORT_NO_NONE to use Virtual Device #1 (VD1) (for Up-MEPs).
     */
    vtss_port_no_t port_no;

    /**
     * [IN]
     * Priority on which the frame sequence shall be transmitted.
     *
     * Valid values are in the range [0; VTSS_PRIO_END] and
     * [VTSS_PRIO_SUPER; VTSS_PRIO_SUPER + 1].
     */
    vtss_prio_t prio;
} vtss_afi_slow_inj_alloc_cfg_t;

/**
 * \brief Allocate AFI slow injection resource
 *
 * \param inst   [IN]  Target instance reference.
 * \param cfg    [IN]  Injection descriptor.
 * \param slowid [OUT] ID used for referencing the allocated resource.
 *
 * \return Return code.
 **/
vtss_rc vtss_afi_slow_inj_alloc(const vtss_inst_t                          inst,
                                const vtss_afi_slow_inj_alloc_cfg_t *const cfg,
                                      vtss_afi_slowid_t             *const slowid);

/**
 * \brief Free AFI slow injection resource
 *
 * Before resources are freed, slow injection must be stopped.
 *
 * \param inst   [IN] Target instance reference.
 * \param slowid [IN] Slow injection ID.
 *
 * \return Return code.
 **/
vtss_rc vtss_afi_slow_inj_free(const vtss_inst_t       inst,
                                     vtss_afi_slowid_t slowid);

/**
 * \brief Setup frame for slow injection.
 *
 * \param inst   [IN] Target instance reference.
 * \param slowid [IN] Slow injection ID.
 *
 * \return Return code.
 **/
vtss_rc vtss_afi_slow_inj_frm_hijack(const vtss_inst_t       inst,
                                           vtss_afi_slowid_t slowid);

/**
 * Structure defining properties of a slow injection.
 */
typedef struct {
    /**
     * [IN]
     * Frames per minute.
     */
    u64 fpm;

    /**
     * [IN]
     * 0: No injection time jitter.
     * 1: Injection delta time is randomized in interval [0.75*60/fpm; 60/fpm] secs
     * 2: Injection delta time is randomized in interval [0.50*60/fpm; 60/fpm] secs
     * 3: Injection delta time is randomized in interval [1 tick; 60/fpm secs]
     *
     * E.g. to be used for PTP Delay Request, ref IEEE1588-2008, 9.5.11.2.
     */
    u8 jitter_mode;
} vtss_afi_slow_inj_start_cfg_t;

/**
 * \brief Start slow injection.
 *
 * \param inst   [IN] Target instance reference.
 * \param slowid [IN] Slow injection ID.
 * \param cfg    [IN] Slow injection configuration.
 *
 * \return Return code.
 **/
vtss_rc vtss_afi_slow_inj_start(const vtss_inst_t                          inst,
                                      vtss_afi_slowid_t                    slowid,
                                const vtss_afi_slow_inj_start_cfg_t *const cfg);

/**
 * \brief Stop slow injection.
 *
 * \param inst   [IN] Target instance reference.
 * \param slowid [IN] Slow injection ID.
 *
 * \return Return code.
 **/
vtss_rc vtss_afi_slow_inj_stop(const vtss_inst_t       inst,
                                     vtss_afi_slowid_t slowid);

// ========================================
// Other functions
// ----------------------------------------

// Stopping and starting all injections for port.
// Note that during link down, injections are automatically stopped and
// (re)started upon link up.

/**
 * \brief (Re)start all injections for specific port
 *
 * \param inst    [IN] Target instance reference.
 * \param port_no [IN] Port number. VTSS_PORT_NO_NONE for VD1.
 *
 * \return Return code.
 **/
vtss_rc vtss_afi_port_start(const vtss_inst_t    inst,
                                  vtss_port_no_t port_no);

/**
 * \brief Stop all injections for specific port
 *
 * \param inst    [IN] Target instance reference.
 * \param port_no [IN] Port number. VTSS_PORT_NO_NONE for VD1.
 *
 * \return Return code.
 **/
vtss_rc vtss_afi_port_stop(const vtss_inst_t    inst,
                                 vtss_port_no_t port_no);

#endif // VTSS_AFI_V2

#ifdef __cplusplus
}
#endif

#endif // VTSS_FEATURE_AFI_SWC
#endif // _VTSS_AFI_API_H_
