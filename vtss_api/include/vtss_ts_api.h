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
 * \brief TimeStamping API.
 * \details This header file describes PTP/OAM TimeStamping API functions and
 * associated types.
 */

#ifndef _VTSS_TS_API_H_
#define _VTSS_TS_API_H_

#include <vtss/api/types.h>

// ***************************************************************************
//
//  TimeStamping interface
//
// ***************************************************************************

#if defined(VTSS_FEATURE_TIMESTAMP)
#ifdef __cplusplus
extern "C" {
#endif

/** \brief Number of clock cycle counts pr sec. */
#if defined (VTSS_ARCH_SERVAL)
#define VTSS_HW_TIME_CNT_PR_SEC 1000000000 /* Serval counts ns instead of clock cycles */
/** \brief Number of nanoseconds pr clock count. */
#define VTSS_HW_TIME_NSEC_PR_CNT 1
#endif
#if defined (VTSS_ARCH_LUTON26)
#define VTSS_HW_TIME_CNT_PR_SEC 250000000 /* L26 counts clock cycles instead of ns */
/** \brief Number of nanoseconds pr clock count. */
#define VTSS_HW_TIME_NSEC_PR_CNT 4
#endif
#if defined (VTSS_ARCH_JAGUAR_1)
#define VTSS_HW_TIME_CNT_PR_SEC 1000000000
/** \brief Number of nanoseconds pr clock count. */
#define VTSS_HW_TIME_NSEC_PR_CNT 1
/** \brief Jaguar nanosecond time counter wrap around value (jaguar time counter wraps each second). */
#define VTSS_HW_TIME_WRAP_LIMIT  VTSS_HW_TIME_CNT_PR_SEC       /* time counter wrap around limit+1 */
#endif

#if defined (VTSS_ARCH_JAGUAR_2)
#define VTSS_HW_TIME_CNT_PR_SEC 1000000000
    /** \brief Number of nanoseconds pr clock count. */
#define VTSS_HW_TIME_NSEC_PR_CNT 1
    /** \brief Jaguar2 nanosecond time counter wrap around value (jaguar2 time counter wraps when 0xffffffff is reached). */
#define VTSS_HW_TIME_WRAP_LIMIT  0       /* time counter wrap around limit+1 */
#endif

#if defined (VTSS_ARCH_LUTON26) || defined (VTSS_ARCH_SERVAL)
/** \brief Caracal nanosecond time counter wrap around value (Caracal time counter wraps when 0xffffffff is reached). */
#define VTSS_HW_TIME_WRAP_LIMIT  0       /* time counter wrap around limit+1 (=0 if wrap at 0xffffffff) */
#endif

#if defined (VTSS_ARCH_LUTON26) || defined (VTSS_ARCH_JAGUAR_1)
/** \brief Jaguar/Luton26 minimum adjustment rate in units of 0,1 ppb. */
#define VTSS_HW_TIME_MIN_ADJ_RATE  40       /* 4 ppb */
#endif
#if defined (VTSS_ARCH_SERVAL)
    /** \brief Serval minimum adjustment rate in units of 0,1 ppb. */
#define VTSS_HW_TIME_MIN_ADJ_RATE  10       /* 1 ppb */
#endif

#if defined (VTSS_ARCH_JAGUAR_2)
    /** \brief Jaguar 2 minimum adjustment rate in units of 0,1 ppb. */
#define VTSS_HW_TIME_MIN_ADJ_RATE  10       /* 1 ppb */
#endif

/** \brief This is the max time offset adjustment that os done without setting ports in disabled state */
#define VTSS_HW_TIME_MAX_FINE_ADJ   25

/**
 * \brief Set the current time in a Timestamp format.
 * \param inst [IN]     handle to an API instance.
 * \param ts [IN]       pointer to a TimeStamp structure.
 *
 * \return Return code.
 */
vtss_rc vtss_ts_timeofday_set(const vtss_inst_t             inst,
                              const vtss_timestamp_t               *const ts);

/**
 * \brief Set delta the current time in a Timestamp format.
 * \param inst [IN]     handle to an API instance.
 * \param ts [IN]       pointer to a TimeStamp structure.
 * \param negative [IN] True if ts is subtracted from current time, else ts is added.
 *
 * \return Return code.
 */
vtss_rc vtss_ts_timeofday_set_delta(const vtss_inst_t       inst,
                                    const vtss_timestamp_t         *ts,
                                    BOOL                      negative);

/**
 * \brief Subtract offset from the current time.
 * \param inst [IN]     handle to an API instance.
 * \param offset [IN]   offset in ns.
 *
 * \return Return code.
 */
vtss_rc vtss_ts_timeofday_offset_set(const vtss_inst_t          inst,
                                     const i32                  offset);

/**
 * \brief Do the one sec administration in the Timestamp function.
 * \param inst [IN]     handle to an API instance.
 * \param ongoing_adjustment [OUT]  True if clock adjustment is ongoing
 *
 * \return Return code.
 *
 * Jr1    : Maintains the clock setting process
 * Caracal: Maintains the clock setting process
 * Serval1: Maintains the clock setting process
 * JR2    : it must only be called when the PPS output pin is low, therefore it shall be called at least 200 microseconds after the 1PPS interrupt

 */
vtss_rc vtss_ts_adjtimer_one_sec(const vtss_inst_t             inst,
                                 BOOL                           *const ongoing_adjustment);

/**
 * \brief Check if the clock adjustment is ongoing.
 * \param inst [IN]     handle to an API instance.
 * \param ongoing_adjustment [OUT]  True if clock adjustment is ongoing
 *
 * \return Return code.
 */
vtss_rc vtss_ts_ongoing_adjustment(const vtss_inst_t           inst,
                                   BOOL                        *const ongoing_adjustment);

/**
 * \brief Get the current time in a Timestamp format, and the corresponding
 *        time counter.
 * \param inst [IN]     handle to an API instance
 * \param ts [OUT]      pointer to a TimeStamp structure
 * \param tc [OUT]      pointer to a time counter (internal hw format)
 *                      Jaguar: tc = nanoseconds/nanosec_pr_clock_cycle (0..249999999)
 *                      Caracal:tc = free running clock cycle counter
 *                      Serval: tc = (nanoseconds + seconds*10**9) mod 2**32
 *
 * \return Return code.
 */
vtss_rc vtss_ts_timeofday_get(const vtss_inst_t             inst,
                              vtss_timestamp_t                     *const ts,
                              u32                           *const tc);

/**
 * \brief Get the time at the next 1PPS pulse edge in a Timestamp format.
 * \param inst [IN]     handle to an API instance
 * \param ts [OUT]      pointer to a TimeStamp structure
 *
 * \return Return code.
 */
vtss_rc vtss_ts_timeofday_next_pps_get(const vtss_inst_t             inst,
                              vtss_timestamp_t                     *const ts);

/**
 * \brief Adjust the clock timer ratio.
 * \param inst [IN]     handle to an API instance.
 * \param adj [IN]      Clock ratio frequency offset in units of 0,1 ppb (parts pr billion).
 *                      ratio > 0 => clock runs faster
 *
 * \return Return code.
 */
vtss_rc vtss_ts_adjtimer_set(const vtss_inst_t              inst,
                             const i32                      adj);

/**
* \brief get the clock timer ratio.
*
* \param inst [IN]     handle to an API instance.
* \param adj [OUT]     Clock ratio frequency offset in ppb (parts pr billion).
*                      ratio > 0 => clock runs faster
*
* \return Return code.
*/
vtss_rc vtss_ts_adjtimer_get(const vtss_inst_t              inst,
                             i32                            *const adj);

/**
* \brief get the clock internal timer frequency offset, compared to external clock input.
*
* \param inst [IN]     handle to an API instance.
* \param adj [OUT]     Clock ratio frequency offset in ppb (parts pr billion).
*                      ratio > 0 => internal clock runs faster than external clock
*
* \return Return code.
*/
vtss_rc vtss_ts_freq_offset_get(const vtss_inst_t           inst,
                             i32                            *const adj);

#if defined(VTSS_ARCH_SERVAL) || defined(VTSS_ARCH_JAGUAR_2)
/**
 * \brief parameter for setting the alternative  clock mode.
 */
/** \brief external clock output configuration. */
typedef struct vtss_ts_alt_clock_mode_t {
    BOOL one_pps_out;       /**< Enable 1pps output */
    BOOL one_pps_in;        /**< Enable 1pps input */
    BOOL save;              /**< Save actual time counter at next 1 PPS input */
    BOOL load;              /**< Load actual time counter with at next 1 PPS input */
} vtss_ts_alt_clock_mode_t;

/**
 * \brief Get the latest saved nanosec counter from the alternative clock.
 *
 * \param inst [IN]             handle to an API instance
 * \param saved [OUT]           latest saved value.
 *
 * \return Return code.
 */
vtss_rc vtss_ts_alt_clock_saved_get(
                                const vtss_inst_t           inst,
                                u32    *const               saved);

/**
 * \brief Get the alternative external clock mode.
 *
 * \param inst [IN]             handle to an API instance
 * \param alt_clock_mode [OUT]  alternative clock mode.
 *
 * \return Return code.
 */
vtss_rc vtss_ts_alt_clock_mode_get(
                                const vtss_inst_t              inst,
                                vtss_ts_alt_clock_mode_t       *const alt_clock_mode);

/**
 * \brief Set the alternative external clock mode.
 *  This function configures the 1PPS, L/S pin usage for pin set no 0 in Serval
 *
 * \param inst [IN]             handle to an API instance
 * \param alt_clock_mode [IN]   alternative clock mode.
 *
 * \return Return code.
 */
vtss_rc vtss_ts_alt_clock_mode_set(
                                const vtss_inst_t              inst,
                                const vtss_ts_alt_clock_mode_t *const alt_clock_mode);

/**
 * \brief Set the time at the next 1PPS pulse edge in a Timestamp format.
 * \param inst [IN]     handle to an API instance
 * \param ts [OUT]      pointer to a TimeStamp structure
 *
 * \return Return code.
 */
vtss_rc vtss_ts_timeofday_next_pps_set(const vtss_inst_t       inst,
                                const vtss_timestamp_t         *const ts);
#endif

/**
 * \brief parameter for setting the external clock mode.
 */
typedef enum  {
    TS_EXT_CLOCK_MODE_ONE_PPS_DISABLE,
    TS_EXT_CLOCK_MODE_ONE_PPS_OUTPUT,
    TS_EXT_CLOCK_MODE_ONE_PPS_INPUT,
    TS_EXT_CLOCK_MODE_ONE_PPS_OUTPUT_INPUT,
    TS_EXT_CLOCK_MODE_MAX
} vtss_ts_ext_clock_one_pps_mode_t;

/** \brief external clock output configuration. */
typedef struct vtss_ts_ext_clock_mode_t {
    vtss_ts_ext_clock_one_pps_mode_t   one_pps_mode;    
                            /**< Select 1pps ext clock mode:
                               input :  lock clock to 1pps input
                               output:  enable external sync pulse output
                               disable: disable 1 pps */
    BOOL enable;            /**< Select internal sync pulse (enable = false) 
                                or external sync pulse (enable = true) */
    u32  freq;              /**< clock output frequency (hz [1..25.000.000]). */
} vtss_ts_ext_clock_mode_t;


/**
 * \brief Get the external clock mode.
 *  The mode depends on the hardware capability, it may be:
 *          Enable/disable external synch pulse
 *          Set clock output frequency
 *
 * \param inst [IN]             handle to an API instance
 * \param ext_clock_mode [OUT]  external clock mode.
 *
 * \return Return code.
 */
vtss_rc vtss_ts_external_clock_mode_get(
                                const vtss_inst_t           inst,
                                vtss_ts_ext_clock_mode_t    *const ext_clock_mode);

/**
 * \brief Set the external clock mode.
 *  The mode depends on the hardware capability, it may be:
 *          Enable/disable external synch pulse
 *          Set clock output frequency
 *
 * \param inst [IN]             handle to an API instance
 * \param ext_clock_mode [IN]   external clock mode.
 *
 * \return Return code.
 */
vtss_rc vtss_ts_external_clock_mode_set(
                                const vtss_inst_t              inst,
                                const vtss_ts_ext_clock_mode_t *const ext_clock_mode);

/**
 * \brief Get the latest saved time counter in nanosec.
 *
 * \param inst [IN]             handle to an API instance
 * \param saved [OUT]           latest saved value. [0..999.999.999]
 *
 * \return Return code.
 */
vtss_rc vtss_ts_external_clock_saved_get(
                                const vtss_inst_t               inst,
                                u32                             *const saved);

/**
 * \brief Set the ingress latency.
 * \param inst [IN]             handle to an API instance
 * \param port_no [IN]          port number
 * \param ingress_latency [IN]  pointer to ingress latency
 *
 * \return Return code.
 */
vtss_rc vtss_ts_ingress_latency_set(const vtss_inst_t              inst,
                                    const vtss_port_no_t           port_no,
                                    const vtss_timeinterval_t             *const ingress_latency);

/**
 * \brief Get the ingress latency.
 * \param inst [IN]             handle to an API instance
 * \param port_no [IN]          port number
 * \param ingress_latency [OUT] pointer to ingress_latency
 *
 * \return Return code.
 */
vtss_rc vtss_ts_ingress_latency_get(const vtss_inst_t              inst,
                                    const vtss_port_no_t           port_no,
                                    vtss_timeinterval_t                   *const ingress_latency);

/**
 * \brief Set the P2P delay.
 * \param inst [IN]             handle to an API instance
 * \param port_no [IN]          port number
 * \param p2p_delay [IN]        peer-2-peer delay (measured)
 *
 * \return Return code.
 */
vtss_rc vtss_ts_p2p_delay_set(const vtss_inst_t                   inst,
                              const vtss_port_no_t                port_no,
                              const vtss_timeinterval_t                  *const p2p_delay);

/**
 * \brief Get the P2P delay.
 * \param inst [IN]             handle to an API instance
 * \param port_no [IN]          port number
 * \param p2p_delay [OUT]       pointer to peer-2-peer delay
 *
 * \return Return code.
 */
vtss_rc vtss_ts_p2p_delay_get(const vtss_inst_t              inst,
                              const vtss_port_no_t           port_no,
                              vtss_timeinterval_t                   *const p2p_delay);


/**
 * \brief Set the egress latency.
 * \param inst [IN]             handle to an API instance
 * \param port_no [IN]          port number
 * \param egress_latency [IN]   egress latency
 *
 * \return Return code.
 */
vtss_rc vtss_ts_egress_latency_set(const vtss_inst_t            inst,
                                   const vtss_port_no_t         port_no,
                                   const vtss_timeinterval_t    *const egress_latency);

/**
 * \brief Get the egress latency.
 * \param inst [IN]             handle to an API instance
 * \param port_no [IN]          port number
 * \param egress_latency [OUT]  pointer to  egress latency
 *
 * \return Return code.
 */
vtss_rc vtss_ts_egress_latency_get(const vtss_inst_t            inst,
                                   const vtss_port_no_t         port_no,
                                   vtss_timeinterval_t          *const egress_latency);

/**
 * \brief Set the delay asymmetry.
 * \param inst [IN]             handle to an API instance
 * \param port_no [IN]          port number
 * \param delay_asymmetry [IN]  delay asymmetry
 *
 * \return Return code.
 */
vtss_rc vtss_ts_delay_asymmetry_set(const vtss_inst_t           inst,
                                   const vtss_port_no_t         port_no,
                                   const vtss_timeinterval_t    *const delay_asymmetry);

/**
 * \brief Get the delay asymmetry.
 * \param inst [IN]             handle to an API instance
 * \param port_no [IN]          port number
 * \param delay_asymmetry [OUT] pointer to  delay asymmetry
 *
 * \return Return code.
 */
vtss_rc vtss_ts_delay_asymmetry_get(const vtss_inst_t           inst,
                                   const vtss_port_no_t         port_no,
                                   vtss_timeinterval_t          *const delay_asymmetry);

/**
 * \brief parameter for setting the timestamp operating mode
 */
typedef enum  {
    TS_MODE_NONE,
    TS_MODE_EXTERNAL,
    TS_MODE_INTERNAL,
    TX_MODE_MAX
} vtss_ts_mode_t;

/** \brief Timestamp operation */
typedef struct vtss_ts_operation_mode_t {
    vtss_ts_mode_t mode;                /**< Hardware Timestamping mode for a port(EXTERNAL or INTERNAL) */
} vtss_ts_operation_mode_t;


/**
 * \brief Set the timestamping operation mode for a port.
 * \param inst [IN]             handle to an API instance
 * \param port_no [IN]          port number
 * \param mode [IN]             pointer to a struct holding the operation mode
 *
 * \return Return code.
 *
 * Serval: Used to set backplane (INTERNAL) mode/normal(EXTERNAL) mode
 * Other : Not used
 */
vtss_rc vtss_ts_operation_mode_set(const vtss_inst_t              inst,
                                   const vtss_port_no_t           port_no,
                                   const vtss_ts_operation_mode_t *const mode);

/**
 * \brief Get the timestamping operation mode for a port
 * \param inst [IN]             handle to an API instance
 * \param port_no [IN]          port number
 * \param mode [OUT]            pointer to a struct holding the operation mode
 *
 * \return Return code.
 */
vtss_rc vtss_ts_operation_mode_get(const vtss_inst_t              inst,
                                   const vtss_port_no_t           port_no,
                                   vtss_ts_operation_mode_t       *const mode);
                                   
/**
 * \brief parameter for setting the internal timestamp format
 */
typedef enum  {
    TS_INTERNAL_FMT_NONE,
    TS_INTERNAL_FMT_RESERVED_LEN_30BIT,        /* Ts is stored in reservedField as 30 bit (ns only) */
    TS_INTERNAL_FMT_RESERVED_LEN_32BIT,        /* Ts is stored in reservedField as 32 bit (ns+sec*10^9) mod 2^32) */
    TS_INTERNAL_FMT_SUB_ADD_LEN_44BIT_CF62,    /* Ts is subtracted from cf at ingress as 44 bit (ns+sec*10^9) mod 2^44), MSB is saved in cf bit 62 */
    TS_INTERNAL_FMT_RESERVED_LEN_48BIT_CF_3_0, /* Ts is subtracted from cf at ingress as 48 bit (ns+sec*10^9) mod 2^48), MSBs are saved in cf bit 3-0 */
    TS_INTERNAL_FMT_RESERVED_LEN_48BIT_CF_0,   /* Ts is subtracted from cf at ingress as 48 bit (ns+sec*10^9) mod 2^48), MSB is saved in cf bit 0 */
    TS_INTERNAL_FMT_RESERVED_LEN_48BIT_CF,     /* Ts is subtracted from cf at ingress as 48 bit (ns+sec*10^9) mod 2^48) */
    TX_INTERNAL_FMT_MAX
} vtss_ts_internal_fmt_t;

/** \brief Hardware timestamping format mode for internal ports */
typedef struct vtss_ts_internal_mode_t {
    vtss_ts_internal_fmt_t int_fmt;    /**< Hardware Timestamping format mode for INTERNAL ports */
} vtss_ts_internal_mode_t;


/**
 * \brief Set the internal timestamping mode.
 * \param inst [IN]             handle to an API instance
 * \param mode [IN]             pointer to a struct holding the operation mode
 *
 * \return Return code.
 *
 * Serval: Used to set INTERNAL mode timestamping format
 * Other : Not used
 */
vtss_rc vtss_ts_internal_mode_set(const vtss_inst_t              inst,
                                   const vtss_ts_internal_mode_t *const mode);

/**
 * \brief Get the internal timestamping mode.
 * \param inst [IN]             handle to an API instance
 * \param mode [OUT]            pointer to a struct holding the operation mode
 *
 * \return Return code.
 */
vtss_rc vtss_ts_internal_mode_get(const vtss_inst_t              inst,
                                   vtss_ts_internal_mode_t       *const mode);

/** \brief Timestamp identifier */
typedef struct vtss_ts_id_t {
    u32                 ts_id;  /**< Timestamp identifier */
} vtss_ts_id_t;

/** \brief Timestamp structure */
typedef struct vtss_ts_timestamp_t {
    u32 ts;                     /**< Timestamp value */
    u32 id;                     /**< Timestamp identifier */
    void * context;             /**< Application specific context */
    BOOL ts_valid;              /**< Timestamp is valid (can be not valid if timestamp is not received */
} vtss_ts_timestamp_t;

/**
 * \brief Update the internal timestamp table, from HW
 * \param inst    [IN]          handle to an API instance
 *
 * \return Return code.
 */
vtss_rc vtss_tx_timestamp_update(const vtss_inst_t              inst);

/**
* \brief Get the rx FIFO timestamp for a {timestampId}
* \param inst    [IN]          handle to an API instance
* \param ts_id   [IN]          timestamp id
* \param ts     [OUT]          pointer to a struct holding the fifo timestamp
*
* \return Return code.
*/
vtss_rc vtss_rx_timestamp_get(const vtss_inst_t              inst,
                              const vtss_ts_id_t             *const ts_id,
                              vtss_ts_timestamp_t            *const ts);

/**
* \brief Release the FIFO rx timestamp id 
* \param inst    [IN]          handle to an API instance
* \param ts_id   [IN]          timestamp id
*
* \return Return code.
*/
vtss_rc vtss_rx_timestamp_id_release(const vtss_inst_t              inst,
                              const vtss_ts_id_t             *const ts_id);

/**
* \brief Get rx timestamp from a port (convert from slave time to the master time)
* \param inst    [IN]          handle to an API instance
* \param port_no [IN]          port number
* \param ts     [IN/OUT]       pointer to a struct holding the timestamp
*
* \return Return code.
*/
vtss_rc vtss_rx_master_timestamp_get(const vtss_inst_t              inst,
                                     const vtss_port_no_t           port_no,
                                     vtss_ts_timestamp_t            *const ts);

#if defined (VTSS_ARCH_SERVAL_CE)
/**
 * \brief parameter for requesting an oam timestamp
 */
typedef struct vtss_oam_ts_id_s {
    u32                voe_id;  /**< VOE instance (Timestamp) identifier */
    u32                voe_sq;  /**< VOE (Timestamp) sequence no */
} vtss_oam_ts_id_t;

/**
 * \brief parameter for returning an oam timestamp
 */
typedef struct vtss_oam_ts_timestamp_s {
    u32             ts;         /**< Timestamp value (ns + sec*10^9) mod 2^32 */
    vtss_port_no_t  port_no;    /**< port number */
    BOOL            ts_valid;   /**< Timestamp is valid (can be not valid if no timestamp is received for the requested {voe_id, voe_sq} */
} vtss_oam_ts_timestamp_t;

/**
 * \brief Get oam timestamp
 * \param inst    [IN]          handle to an API instance
 * \param id      [IN]          identifies the requested timestamp id
 * \param ts      [OUT]         pointer to a struct holding the timestamp
 *
 * \return Return code.
 */
vtss_rc vtss_oam_timestamp_get(const vtss_inst_t             inst,
                               const vtss_oam_ts_id_t        *const id,
                               vtss_oam_ts_timestamp_t       *const ts);
#endif /* VTSS_ARCH_SERVAL_CE */

/** \brief Timestamp allocation */
typedef struct vtss_ts_timestamp_alloc_t {
    u64 port_mask;              /**< Identify the ports that a timestamp id is allocated to */
    void * context;             /**< Application specific context used as parameter in the call-out */
    void (*cb)(void *context, u32 port_no, vtss_ts_timestamp_t *ts);
                                /**< Application call-out function called when the timestamp is available */

} vtss_ts_timestamp_alloc_t;
/**
 * \brief Allocate a timestamp id for a two step transmission
 * \param inst       [IN]          handle to an API instance
 * \param alloc_parm [IN]          pointer allocation parameters
 * \param ts_id      [OUT]         timestamp id
 *
 * \return Return code.
 */
vtss_rc vtss_tx_timestamp_idx_alloc(const vtss_inst_t               inst,
                                 const vtss_ts_timestamp_alloc_t    *const alloc_parm,
                                 vtss_ts_id_t                       *const ts_id);

/**
 * \brief Age the FIFO timestamps
 * \param inst    [IN]          handle to an API instance
 *
 * \return Return code.
 */
vtss_rc vtss_timestamp_age(const vtss_inst_t              inst);

/**
* \brief Signal port status change (used to detect and compensate for the internal ingress and egress latencies)
* \param inst    [IN]          handle to an API instance
* \param port_no [IN]          port number
*
* \return Return code.
*/
vtss_rc vtss_ts_status_change(const vtss_inst_t      inst,
                              const vtss_port_no_t   port_no);

#if defined (VTSS_FEATURE_DELAY_REQ_AUTO_RESP)
/**
 * \brief parameter for setting auto response behaviour pr port
 */
typedef struct vtss_ts_autoresp_port_cfg_s {
    u8              domain;         /**< ptp domain assigned to the port [0..2], this refers to one of the 3 timingdomains that exiets in the chip, and is not
                                         the same as domainNumber in the PTP protocol */
                                    /**< Note that the domain is set into both ANA_ACL:PORT:PTP_CFG.PTP_DOMAIN and DEVxxxx::PTP_CFG.PTP_DOM. And the central
                                         time counters in the DEVCPU block must be disabled before setting PTP_DOM, and enabled afterwards. This influences all
                                         timestamping within this domain. 
                                         I.e. the application cannot rely on the timestamps during this setting process. */ 
    u8              ptp_port_lsb;   /**< ptp port number least significant bits 5:0 value [0..63] */
} vtss_ts_autoresp_port_cfg_t;

/**
 * \brief Set auto response behaviour pr. port.
 * \param inst    [IN]          handle to an API instance
 * \param port_no [IN]          port number
 * \param cfg     [IN]          port configuration for autoresponse feature
 *
 * \return Return code.
 */
vtss_rc vtss_ts_autoresp_port_cfg_set(const vtss_inst_t                 inst,
                                      const vtss_port_no_t              port_no,
                                      const vtss_ts_autoresp_port_cfg_t *const cfg);

/**
 * \brief Get auto response behaviour pr. port.
 * \param inst    [IN]          handle to an API instance
 * \param port_no [IN]          port number
 * \param cfg     [OUT]         port configuration for autoresponse feature
 *
 * \return Return code.
 */
vtss_rc vtss_ts_autoresp_port_cfg_get(const vtss_inst_t                 inst,
                                      const vtss_port_no_t              port_no,
                                      vtss_ts_autoresp_port_cfg_t       *const cfg);

/**
 * \brief parameter for setting auto response behaviour pr domain
 */
typedef struct vtss_ts_autoresp_dom_cfg_s {
    BOOL                        ptp_port_individual;/**< TRUE => PortIdentity = ptp_port_msb || ptp_port_lsb, FALSE => PortIdentity = ptp_port_msb */
    u16                         ptp_port_msb;       /**< ptp port number most significant bits 15:6 */
    vtss_clock_identity         clock_identity;     /**< ptp clock identity */
    vtss_ace_u8_t               flag_field_update;  /**< flag field [0] update value and mask */

} vtss_ts_autoresp_dom_cfg_t;

/**
 * \brief Set auto response behaviour pr. port.
 * \param inst    [IN]          handle to an API instance
 * \param domain  [IN]          ptp domain number [0..2]
 * \param cfg     [IN]          domain configuration for autoresponse feature
 *
 * \return Return code.
 */
vtss_rc vtss_ts_autoresp_dom_cfg_set(const vtss_inst_t                  inst,
                                     const u8                           domain,
                                     const vtss_ts_autoresp_dom_cfg_t   *const cfg);

/**
 * \brief Get auto response behaviour pr. port.
 * \param inst    [IN]          handle to an API instance
 * \param domain  [IN]          ptp domain number [0..2]
 * \param cfg     [OUT]         domain configuration for autoresponse feature
 *
 * \return Return code.
 */
vtss_rc vtss_ts_autoresp_dom_cfg_get(const vtss_inst_t                  inst,
                                     const u8                           domain,
                                     vtss_ts_autoresp_dom_cfg_t         *const cfg);

#endif /* VTSS_FEATURE_DELAY_REQ_AUTO_RESP */

#ifdef __cplusplus
}
#endif
#endif /* VTSS_FEATURE_TIMESTAMP */
#endif // _VTSS_TS_API_H_

// ***************************************************************************
//
//  End of file.
//
// ***************************************************************************
