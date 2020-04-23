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

#ifndef _VTSS_MISC_STATE_H_
#define _VTSS_MISC_STATE_H_

#if defined(VTSS_FEATURE_MISC)

/* Number of chips per instance */
#define VTSS_CHIP_CNT 2

#if defined(VTSS_FEATURE_SERIAL_GPIO)
typedef struct {
    BOOL     enable[VTSS_SGPIO_PORTS][4]; /* each port has four bits */
} vtss_sgpio_event_enable_t;
#endif /* VTSS_FEATURE_SERIAL_GPIO */

typedef struct {
    /* CIL function pointers */
    vtss_rc (* reg_read)(struct vtss_state_s *vtss_state,
                         const vtss_chip_no_t chip_no, const u32 addr, u32 * const value);
    vtss_rc (* reg_write)(struct vtss_state_s *vtss_state,
                          const vtss_chip_no_t chip_no, const u32 addr, const u32 value);
    vtss_rc (* chip_id_get)(struct vtss_state_s *vtss_state, vtss_chip_id_t *const chip_id);
    vtss_rc (* intr_sticky_clear)(const struct vtss_state_s *const state, u32 ext);
    vtss_rc (* poll_1sec)(struct vtss_state_s *vtss_state);
    vtss_rc (* ptp_event_poll)(struct vtss_state_s *vtss_state,
                               vtss_ptp_event_type_t  *const ev_mask);
    vtss_rc (* ptp_event_enable)(struct vtss_state_s *vtss_state,
                                 vtss_ptp_event_type_t ev_mask, BOOL enable);
    vtss_rc (* dev_all_event_poll)(struct vtss_state_s *vtss_state,
                                   vtss_dev_all_event_poll_t poll_type,
                                   vtss_dev_all_event_type_t  *const ev_mask);
    vtss_rc (* dev_all_event_enable)(struct vtss_state_s *vtss_state,
                                     vtss_port_no_t port,
                                     vtss_dev_all_event_type_t ev_mask, BOOL enable);
#if defined(VTSS_FEATURE_SERIAL_LED)
    vtss_rc (* serial_led_set)(struct vtss_state_s *vtss_state,
                               const vtss_led_port_t  port,
                               const vtss_led_mode_t  mode[3]);

    vtss_rc (* serial_led_intensity_set)(struct vtss_state_s *vtss_state,
                                         const vtss_led_port_t  port,
                                         const i8               intensity);

    vtss_rc (* serial_led_intensity_get)(struct vtss_state_s *vtss_state,
                                         i8               *intensity);
#endif /* VTSS_FEATURE_SERIAL_LED */
#if defined(VTSS_FEATURE_SERIAL_GPIO)
    vtss_rc (* sgpio_conf_set)(struct vtss_state_s *vtss_state,
                               const vtss_chip_no_t     chip_no,
                               const vtss_sgpio_group_t group,
                               const vtss_sgpio_conf_t  *const conf);
    vtss_rc (* sgpio_read)(struct vtss_state_s *vtss_state,
                           const vtss_chip_no_t     chip_no,
                           const vtss_sgpio_group_t group,
                           vtss_sgpio_port_data_t   data[VTSS_SGPIO_PORTS]);
    vtss_rc (* sgpio_event_enable)(struct vtss_state_s *vtss_state,
                                   const vtss_chip_no_t     chip_no,
                                   const vtss_sgpio_group_t group,
                                   const vtss_port_no_t     port,
                                   const u32                bit,
                                   BOOL                     enable);
    vtss_rc (* sgpio_event_poll)(struct vtss_state_s *vtss_state,
                                 const vtss_chip_no_t     chip_no,
                                 const vtss_sgpio_group_t group,
                                 const u32                bit,
                                 BOOL                     *const events);
#endif /* VTSS_FEATURE_SERIAL_GPIO */
#if defined(VTSS_GPIOS)
    vtss_rc (* gpio_mode)(struct vtss_state_s *vtss_state,
                          const vtss_chip_no_t   chip_no,
                          const vtss_gpio_no_t   gpio_no,
                          const vtss_gpio_mode_t mode);
    vtss_rc (* gpio_read)(struct vtss_state_s *vtss_state,
                          const vtss_chip_no_t  chip_no,
                          const vtss_gpio_no_t  gpio_no,
                          BOOL                  *const value);
    vtss_rc (* gpio_write)(struct vtss_state_s *vtss_state,
                           const vtss_chip_no_t  chip_no,
                           const vtss_gpio_no_t  gpio_no,
                           const BOOL            value);
    vtss_rc (* gpio_event_poll)(struct vtss_state_s *vtss_state,
                                const vtss_chip_no_t  chip_no,
                                BOOL                  *const events);
    vtss_rc (* gpio_event_enable)(struct vtss_state_s *vtss_state,
                                  const vtss_chip_no_t  chip_no,
                                  const vtss_gpio_no_t  gpio_no,
                                  const BOOL            enable);
#endif /* GPIOS */
#if defined(VTSS_FEATURE_INTERRUPTS)
    vtss_rc (* intr_cfg)(struct vtss_state_s *vtss_state,
                         const u32 mask, const BOOL polarity, const BOOL enable);
    vtss_rc (* intr_pol_negation)(struct vtss_state_s *vtss_state);
    vtss_rc (* intr_status_get)(struct vtss_state_s *vtss_state,
                                vtss_intr_t *status);
    vtss_rc (* intr_mask_set)(struct vtss_state_s *vtss_state,
                              vtss_intr_t *mask);
#endif // VTSS_FEATURE_INTERRUPTS
#ifdef VTSS_FEATURE_IRQ_CONTROL
    vtss_rc (* irq_cfg)(struct vtss_state_s *vtss_state,
                        const vtss_irq_t irq,
                        const vtss_irq_conf_t *const conf);
    vtss_rc (* irq_status)(struct vtss_state_s *vtss_state, 
                           vtss_irq_status_t *status);
    vtss_rc (* irq_enable)(const vtss_inst_t inst, 
                           const vtss_irq_t irq,
                           BOOL enable);
#endif  /* VTSS_FEATURE_IRQ_CONTROL */
    /* Configuration/state */
    vtss_chip_id_t                chip_id;
    BOOL                          jr2_a; /* Jaguar-2 revision A */
    u32                           gpio_count;
#if defined(VTSS_FEATURE_SERIAL_GPIO)
    u32                           sgpio_group_count;
    vtss_sgpio_conf_t             sgpio_conf[VTSS_CHIP_CNT][VTSS_SGPIO_GROUPS];
    vtss_sgpio_event_enable_t     sgpio_event_enabled[VTSS_CHIP_CNT][VTSS_SGPIO_GROUPS];
#endif /* VTSS_FEATURE_SERIAL_GPIO */
#ifdef VTSS_FEATURE_IRQ_CONTROL
    vtss_irq_conf_t               irq_conf[VTSS_IRQ_MAX];
#endif
} vtss_misc_state_t;

vtss_rc vtss_misc_inst_create(struct vtss_state_s *vtss_state);
void vtss_misc_debug_print(struct vtss_state_s *vtss_state,
                           const vtss_debug_printf_t pr,
                           const vtss_debug_info_t   *const info);

#endif /* VTSS_FEATURE_MISC */

#endif /* _VTSS_MISC_STATE_H_ */
