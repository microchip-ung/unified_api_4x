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

#ifndef _VTSS_PORT_CUSTOM_JR1_API_H_
#define _VTSS_PORT_CUSTOM_JR1_API_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Jaguar NPI chip port */
#define JR1_NPI_CHIP_PORT 31

/* LED tower mode */
typedef enum {
    LED_TOWER_MODE_LINK_SPEED,  /* Green: 1G link/activity; Orange: 10/100 link/activity */
    LED_TOWER_MODE_DUPLEX,      /* Green: FDX; Orange: HDX + Collisions */
    LED_TOWER_MODE_LINK_STATUS, /* Green: Link/activity; Orange: port disabled/errors */
    LED_TOWER_MODE_POWER_SAVE,  /* Disabled to save power */
    LED_TOWER_MODE_CNT
} led_tower_mode_t;

extern led_tower_mode_t jr1_tower_mode;

void jr1_board_setup_ref(vtss_board_t *board, vtss_board_info_t *board_info);
void jr1_board_setup_cu48(vtss_board_t *board, vtss_board_info_t *board_info);

/* 10G PHY detection data */
typedef struct  {
    vtss_port_no_t port_no;      /* Port number (debugging) */
    u8             miim_addr_0;  /* Primary MIIM address */ 
    u8             miim_addr_1;  /* Secondary MIIM address */
    BOOL           old_found[2]; /* Only one 'old' module can be detected */
    port_cap_t     cap;          /* Port capability */
} jr1_10g_detect_t;

/* Detect Vitesse 10G PHY module on MIIM controller 1 */
void jr1_10g_detect(jr1_10g_detect_t *detect, vtss_board_info_t *board_info);
BOOL jr1_1g_phy_detect(u8 ctrl, u8 miim_addr, vtss_board_info_t *board_info);

u8 jr_telsa_phy_detect_ports(u8 ctrl, u8 miim_addr, vtss_board_info_t *board_info);
/* Port information for LED control */
typedef struct {
    BOOL                enabled;    /* Port enabled */
    vtss_port_status_t  status;     /* Port status */
    vtss_port_counter_t col_cnt;    /* Collision counter */
    vtss_port_counter_t err_cnt;    /* Error counter */
    vtss_port_counter_t pkt_cnt;    /* Rx and Tx packets (activity) */
    BOOL                collisions; /* Collisions detected */
    BOOL                errors;     /* Rx errors detected */
    BOOL                activity;   /* Activity detected */
} jr1_port_info_t;

extern jr1_port_info_t jr1_port_info_table[VTSS_PORTS];

extern port_custom_entry_t jr1_port_table[VTSS_PORTS];

#ifdef __cplusplus
}
#endif

#endif /* _VTSS_PORT_CUSTOM_JR1_API_H_ */

/****************************************************************************/
/*                                                                          */
/*  End of file.                                                            */
/*                                                                          */
/****************************************************************************/
