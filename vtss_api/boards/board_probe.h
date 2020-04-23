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

#ifndef _VTSS_BOARD_PROBE_H_
#define _VTSS_BOARD_PROBE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "port_custom_api.h"

typedef struct {
    /*
     * Identification & Features
     */
    vtss_board_type_t type;
    const char *name;
    u32 features;
    int chipcount;
    const port_custom_entry_t *custom_port_table;

    /*
     * Stacking support
     */
    u32 default_stackport_a, default_stackport_b;

    /* 
     * Board specific functions
     */
    
    /* Initialize board */
    void (*init)(void);

    /* Release ports from reset */
    vtss_rc (*reset)(void);

    /* Post ports reset */
    vtss_rc (*post_reset)(void);

    /* Called when system is reset. */
    void (*pre_reset)(void);

    /* Initialize port LEDs */
    vtss_rc (*led_init)(void);

    /* Update port LED */
    vtss_rc (*led_update)(vtss_port_no_t port_no,
                          vtss_port_status_t *status,
                          vtss_port_counters_t *counters,
                          port_custom_conf_t *port_conf);
    
    /* Function for doing special port configuration that depends upon the platform */
    void (*port_conf)(vtss_port_no_t port, 
                      port_custom_conf_t *port_conf, 
                      vtss_port_status_t *port_status);

    /* Update port map with new SFP interface */
    void (*sfp_update_if)(vtss_port_no_t port_no, 
                          vtss_port_interface_t mac_if);

    /* Do SFP i2c reads */
    vtss_rc (*sfp_i2c_read)(vtss_port_no_t port_no, 
                            u8 i2c_addr, 
                            u8 addr, 
                            u8 *const data, 
                            u8 cnt,
                            BOOL word_access);

    vtss_rc (*sfp_i2c_write)(vtss_port_no_t port_no, 
                            u8 i2c_addr, 
                            u8 addr, 
                            u8 *const data,
                            BOOL word_access);

   /* Do i2c reads */
    vtss_rc (*i2c_read)(vtss_port_no_t port_no, 
                        u8 i2c_addr, 
                        u8 addr, 
                        u8 *const data, 
                        u8 cnt);

    /* SFP module detect */
    vtss_rc (*sfp_mod_detect)(BOOL *detect_status);

    /* SFP module status */
    vtss_rc (*sfp_status)(port_custom_sfp_status_t *status);

    /* SFP module accept/deny */
    BOOL (*sfp_accept)(u8 *sfp_rom);

    /* Enable i2c access to a SFP module connected to at certain port */
    vtss_rc (*sfp_i2c_enable)(vtss_port_no_t port_no);

    /* Lock the SFP access while accessing the SFP via I2C (avoid a thread enable another SFP while accesseing via I2C ) */
    vtss_rc (*board_sfp_i2c_lock)(BOOL lock);
    BOOL sfp_i2c_lock;

    /* Return capabilities for board and port */
    port_cap_t (*port_cap)(int board_type, vtss_port_no_t port_no);

#if defined(VTSS_ARCH_JAGUAR_2)
    /* Mux mode of the chip (if detectable) */
    vtss_port_mux_mode_t mux_mode;
#endif /*  VTSS_ARCH_JAGUAR_2 */
} vtss_board_t;

typedef BOOL (*vtss_board_probe_t)(vtss_board_t *, vtss_board_info_t *);

#if defined(VTSS_ARCH_LUTON26)
extern BOOL vtss_board_probe_lu26(vtss_board_t *board, vtss_board_info_t *board_info);
#endif /* VTSS_ARCH_LUTON26 */

#if defined(VTSS_ARCH_JAGUAR_1)
extern BOOL vtss_board_probe_jr1(vtss_board_t *board, vtss_board_info_t *board_info);
#endif /* VTSS_ARCH_JAGUAR_1 */

#if defined(VTSS_ARCH_SERVAL_CPU)
extern BOOL vtss_board_probe_serval(vtss_board_t *board, vtss_board_info_t *board_info);
#endif /* VTSS_ARCH_SERVAL_CPU */

#if defined(VTSS_ARCH_SEVILLE)
extern BOOL vtss_board_probe_seville(vtss_board_t *board, vtss_board_info_t *board_info);
#endif /* VTSS_ARCH_SEVILLE */

#if defined(VTSS_ARCH_JAGUAR_2)
extern BOOL vtss_board_probe_jr2(vtss_board_t *board, vtss_board_info_t *board_info);
#endif /* VTSS_ARCH_JAGUAR_2 */

#ifdef __cplusplus
}
#endif

#endif /* _VTSS_BOARD_PROBE_H_ */

/****************************************************************************/
/*                                                                          */
/*  End of file.                                                            */
/*                                                                          */
/****************************************************************************/
