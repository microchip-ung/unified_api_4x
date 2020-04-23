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

#ifndef _VTSS_PORT_CUSTOM_H_
#define _VTSS_PORT_CUSTOM_H_


#include <vtss_api.h>
#include <vtss/api/port.h>

#ifdef __cplusplus
extern "C" {
#endif
/* ================================================================= *
 *  Port capabilities
 * ================================================================= */

typedef struct {
    vtss_port_map_t       map;    /* Port map */
    vtss_port_interface_t mac_if; /* MAC interface */
    port_cap_t            cap;    /* Port capabilities */
} port_custom_entry_t;

typedef struct {
    BOOL  tx_fault;    /* TxFault */
    BOOL  los;         /* Loss Of Signal  */
    BOOL  present;     /* SFP module present  */
} port_custom_sfp_status_t;

/* Set after board probing */
extern const port_custom_entry_t *port_custom_table;

/* Disable advertisement during auto negotiation */
#define PORT_ADV_DIS_HDX  0x01
#define PORT_ADV_DIS_FDX  0x02
#define PORT_ADV_DIS_1G_FDX   0x10 
#define PORT_ADV_UP_MEP_LOOP  0x20 
#define PORT_ADV_DIS_100M     0x40 /* Disable 100Mbit mode*/
#define PORT_ADV_DIS_10M      0x80 /* Disable 10Mbit mode*/
#define PORT_ADV_DIS_SPEED    (PORT_ADV_DIS_10M | PORT_ADV_DIS_100M | PORT_ADV_DIS_1G_FDX) /* All speed bits */
#define PORT_ADV_DIS_DUPLEX   (PORT_ADV_DIS_HDX | PORT_ADV_DIS_FDX) /* All duplex bits*/

/* Maximum number of tags */
#define PORT_MAX_TAGS_ONE  0 /* Backward compatible default is one tag */
#define PORT_MAX_TAGS_NONE 1
#define PORT_MAX_TAGS_TWO  2 

/* The default power mode value */
#define CONF_POWER_MODE_DEFAULT VTSS_PHY_POWER_NOMINAL

/* Initialize board */
void port_custom_init(void);

/* Release ports from reset */
vtss_rc port_custom_reset(void);


/* Post ports reset */
vtss_rc post_port_custom_reset(void);

/* Initialize port LEDs */
vtss_rc port_custom_led_init(void);

/* Update port LED */
vtss_rc port_custom_led_update(vtss_port_no_t port_no, 
                               vtss_port_status_t *status,
                               vtss_port_counters_t *counters,
                               port_custom_conf_t *port_conf);


// Function for doing special port configuration that depends upon the platform
// Forexample do the enzo board requires that if the stack ports uses SFPs, the SFPs must be turn on 
// using a special serialised GPIO system.
void port_custom_conf(vtss_port_no_t port, 
                      port_custom_conf_t *port_conf, 
                      vtss_port_status_t *port_status);

/* Called when system is reset. */
void port_custom_pre_reset(void);

/* Functions for handling SFP modules */
vtss_rc board_sfp_i2c_lock(BOOL lock);
vtss_rc board_sfp_i2c_enable(vtss_port_no_t port_no);
vtss_rc board_sfp_update_if(vtss_port_no_t port_no, vtss_port_interface_t mac_if);
vtss_rc board_sfp_i2c_read(vtss_port_no_t port_no, u8 i2c_addr, u8 addr, u8 *const data, u8 cnt, BOOL word_access);
vtss_rc board_sfp_i2c_write(vtss_port_no_t port_no, u8 i2c_addr, u8 addr, u8 *const data, BOOL word_access);
vtss_rc port_custom_sfp_mod_detect(BOOL *detect_status);
vtss_rc port_custom_sfp_status(port_custom_sfp_status_t *status); // Gets sfp present, los and tx_fault signals
BOOL port_custom_sfp_accept(u8 *sfp_rom);

typedef enum {
    VTSS_BOARD_UNKNOWN = 0,
    VTSS_BOARD_LUTON10_REF = 4 /* Don't change. 1-3 are obsolete */,
    VTSS_BOARD_LUTON26_REF,
    VTSS_BOARD_JAG_CU24_REF,
    VTSS_BOARD_JAG_SFP24_REF,
    VTSS_BOARD_JAG_PCB107_REF,
    VTSS_BOARD_UNUSED,          /* Vacant entry, used to be JAG_CU24_DUAL_REF (obsolete) */
    VTSS_BOARD_JAG_CU48_REF,
    VTSS_BOARD_SERVAL_REF,
    VTSS_BOARD_SERVAL_PCB106_REF,
    VTSS_BOARD_SEVILLE_T1040QDS,
    VTSS_BOARD_SERVAL2_NID_REF,
    VTSS_BOARD_JAGUAR2_CU48_REF,
    VTSS_BOARD_JAGUAR2_REF,
    VTSS_BOARD_JAGUAR2_AQR_REF,
} vtss_board_type_t;

enum vtss_board_feature_e {
    VTSS_BOARD_FEATURE_AMS           = (1 << 0), /**< TBD */
    VTSS_BOARD_FEATURE_LOS           = (1 << 1), /**< Loss of Signal detect */
    VTSS_BOARD_FEATURE_POE           = (1 << 2), /**< Power Over Ethernet */
    VTSS_BOARD_FEATURE_VCXO          = (1 << 3), /**< Voltage-controlled oscillator */
    VTSS_BOARD_FEATURE_STACKING      = (1 << 4), /**< Stacking support */
    VTSS_BOARD_FEATURE_ANEG_COMPLETE = (1 << 5), /**< PHY autoneg complete interrupt */
    VTSS_BOARD_FEATURE_1588_CLK_ADJ_DAC = (1 << 6), /**< AD5667 DAC used to Adjust the 1588 ref clock */
    VTSS_BOARD_FEATURE_1588_REF_CLK_SEL = (1 << 7), /**< Set 1588 ref clock to different frequencies */
};

/* JR2 Ref board port configurations  */
typedef enum {
    VTSS_BOARD_CONF_20x1G_4x2G5_4xSFI_NPI,
    VTSS_BOARD_CONF_24x2G5_2xSFI_NPI,
    VTSS_BOARD_CONF_16x2G5_4xSFI_NPI,
    VTSS_BOARD_CONF_20x1G_4x2G5_2xSFI_NPI,
    VTSS_BOARD_CONF_DEFAULT_VENICE_1G_MODE,
    VTSS_BOARD_CONF_LAST
} vtss_board_port_cfg_t;

/* Struct to store port config for JR2 Ref boards */
typedef struct {
    vtss_board_port_cfg_t  port_cfg;
} vtss_board_port_conf_t;

/* Board information for exchanging data between application and probe function */
typedef struct {
    int                      board_type;   /* Board type */
    vtss_target_type_t       target;       /* Target ID */
    u32                      port_count;   /* Number of ports */
    vtss_reg_read_t          reg_read;     /* Register read function */
    vtss_reg_write_t         reg_write;    /* Register write function */
    vtss_i2c_read_t          i2c_read;     /**< I2C read function */
    vtss_i2c_write_t         i2c_write;    /**< I2C write function */
    volatile u32             *base_addr_1; /* Second base address for access to secondary device */
#if defined(VTSS_ARCH_JAGUAR_1) || defined(VTSS_ARCH_LUTON26) || defined(VTSS_ARCH_SERVAL)
#if defined(VTSS_FEATURE_SERDES_MACRO_SETTINGS)
    vtss_serdes_macro_conf_t serdes;       /* Serdes settings for this board */
#endif
#endif
#if defined(VTSS_ARCH_JAGUAR_2)
    vtss_board_port_cfg_t     port_cfg;    /* Board port configuration id */    
#endif 
} vtss_board_info_t;

char *board_port_id_txt(vtss_board_port_cfg_t id);

int vtss_board_probe(vtss_board_info_t *board_info, const port_custom_entry_t **port_custom_table);

const char *vtss_board_name(void);

vtss_board_type_t vtss_board_type(void);

#if defined(VTSS_ARCH_JAGUAR_2)
vtss_port_mux_mode_t vtss_port_mux_mode(void);
#endif /* VTSS_ARCH_JAGUAR_2 */

u32 vtss_board_features(void);

u32 vtss_board_chipcount(void);

#if defined(VTSS_FEATURE_VSTAX)
u32 vtss_board_default_stackport(BOOL port_0);
#endif  /* VTSS_FEATURE_VSTAX */

port_cap_t vtss_board_port_cap(int board_type, vtss_port_no_t port_no);

void led_tower_update(void);

#ifdef __cplusplus
}
#endif

#endif /* _VTSS_PORT_CUSTOM_H_ */

/****************************************************************************/
/*                                                                          */
/*  End of file.                                                            */
/*                                                                          */
/****************************************************************************/
