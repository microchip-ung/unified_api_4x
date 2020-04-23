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
// Avoid "*.h not used in module port_custom_jr2.c"
/*lint --e{766} */
/*lint -sem(jr2_reset,          thread_protected) */
#include "port_custom_api.h"
#include "board_probe.h"
#if defined(VTSS_SW_OPTION_PHY)
#include "phy_api.h" // For PHY_INST
#else
#define PHY_INST NULL
#endif
#include "../base/phy/phy_1g/vtss_phy.h" // For PHY registers defines
#if defined(VTSS_ARCH_JAGUAR_2)
#define VTSS_RC(expr) { vtss_rc __rc__ = (expr); if (__rc__ < VTSS_RC_OK) return __rc__; }
#if 0
#include <cyg/infra/diag.h>
#define PRINTD(args) { diag_printf("%s: ", __FUNCTION__); diag_printf args; diag_printf("\n"); }
#else
#define PRINTD(args)
#endif
/*lint -esym(459, jr2_i2c_read) OK not to protect jr2_i2c_read with semaphore in this file */
static vtss_i2c_read_t    jr2_i2c_read;     /**< I2C read function */

/*lint -esym(459, jr2_i2c_write) OK not to protect jr2_i2c_write with semaphore in this file */
static vtss_i2c_write_t   jr2_i2c_write;    /**< I2C write function */

/*lint -esym(459, jr2_port_table) */
static port_custom_entry_t jr2_port_table[VTSS_PORT_ARRAY_SIZE];

typedef struct {
    vtss_port_counters_t cnt;
    vtss_port_status_t   status;
    BOOL                 activity;
} jr2_port_info_t;

/* 10G PHY detection data */
typedef struct  {
    vtss_port_no_t port_no;       /* Port number (debugging) */
    u8             miim_addr[2];  /* MIIM address */
    BOOL           miim_found[2]; /* MIIM address */
    port_cap_t     cap;           /* Port capability */
} jr2_10g_detect_t;

/*lint -esym(459, jr2_old_port_info) */
static jr2_port_info_t jr2_old_port_info[VTSS_PORT_ARRAY_SIZE];

/*lint -esym(459, jr2_board_type) */
static int jr2_board_type;

/* Jaguar NPI chip port */
#define JR2_NPI_CHIP_PORT 48

/* SGPIO LED mapping */
typedef struct {
    u8  port;
    u8  bit;
} sgpio_mapping_t;

/* LED colors */
typedef enum {
    LED_GREEN,
    LED_YELLOW,
    LED_MAX
} led_color_t;

typedef enum {
    SFP_DETECT,
    SFP_FAULT,
    SFP_LOS
} sfp_signal_t;

// Configure the DAC to use output voltage 1.5V -> 0ppm (default is -5ppm)
void dac_init()
{
    u8 i2c_data[4];
    i2c_data[0] = 0x38;
    i2c_data[1] = 0x00;
    i2c_data[2] = 0x01;
    (void)jr2_i2c_write(0, 0xf, i2c_data, 3, 14); // use internal ref voltage

    i2c_data[0] = 0x17;
    i2c_data[1] = 0x99;
    i2c_data[2] = 0x99;
    (void)jr2_i2c_write(0, 0xf, i2c_data, 3, 14); // set voltage to 1.5V
}

/* Jaguar-2 register targets and addresses */
#define JR2_TGT_GCB            0x01
#define JR2_ADDR_CHIP_ID       0x02
#define JR2_ADDR_GPIO_OUT      0x12
#define JR2_ADDR_GPIO_IN       0x14
#define JR2_ADDR_GPIO_OE       0x16
#define JR2_ADDR_GPIO_ALT_0    0x1E
#define JR2_ADDR_GPIO_ALT_1    0x1F
#define JR2_ADDR_GPIO_ALT1_0   0x20
#define JR2_ADDR_GPIO_ALT1_1   0x21

#define JR2_ADDR_MIIM_STATUS   0x32
#define JR2_ADDR_MIIM_CMD      0x34
#define JR2_ADDR_MIIM_DATA     0x35
#define JR2_ADDR_MIIM_CFG      0x36

/* Read from primary device */
static vtss_rc jr2_rd(u32 tgt, u32 addr, u32 *value, vtss_board_info_t *board_info)
{
    vtss_rc rc = VTSS_RC_ERROR;

    *value = 0xdeadbeef;
    if (board_info->reg_read != NULL) {
        rc = board_info->reg_read(0, (tgt << 22) + (tgt << 14) + addr, value);
        PRINTD(("tgt: 0x%x, addr: 0x%x, value: 0x%08x", tgt, (tgt << 22) + (tgt << 14) + addr, *value));
    }
    return rc;
}

/* Write to primary device */
static vtss_rc jr2_wr(u32 tgt, u32 addr, u32 value, vtss_board_info_t *board_info)
{
    vtss_rc rc = VTSS_RC_ERROR;

    if (board_info->reg_write != NULL) {
        PRINTD(("tgt: 0x%x, addr: 0x%x, combined: 0x%x value: 0x%08x", tgt, addr,((tgt << 14) + (tgt << 14) + addr), value));
        rc = board_info->reg_write(0, (tgt << 22) + (tgt << 14) + addr, value);
    }
    return rc;
}

#define PHY_CMD_ADDRESS  0 /* 10G: Address */
#define PHY_CMD_WRITE    1 /* 1G/10G: Write */
#define PHY_CMD_READ_INC 2 /* 1G: Read, 10G: Read and increment */
#define PHY_CMD_READ     3 /* 10G: Read */

/* MIIM bus command function for primary device */
static vtss_rc jr2_miim_cmd(u8 cmd, u8 sof, u8 ctrl, u8 miim_addr, u8 reg_addr, u16 *data,
                            vtss_board_info_t *board_info)
{
    vtss_rc rc;
    u32     value, offs = (ctrl * 9);
    int     i;

    PRINTD(("cmd: %u, sof: %u, ctrl: %u, miim_addr: %u, reg_addr: %u",  cmd, sof, ctrl, miim_addr, reg_addr));

    rc = jr2_wr(JR2_TGT_GCB, JR2_ADDR_MIIM_CFG + offs, (sof<<9) | (0x32<<0), board_info);


    if (rc != VTSS_RC_OK)
        return rc;

    rc = jr2_wr(JR2_TGT_GCB, JR2_ADDR_MIIM_CMD + offs,
               (1UL<<31) | (miim_addr<<25) | (reg_addr<<20) | ((*data)<<4) | (cmd<<1),
               board_info);


    if (rc != VTSS_RC_OK)
        return rc;

    for (i = 0; i < 100; i++) {
        rc = jr2_rd(JR2_TGT_GCB, JR2_ADDR_MIIM_STATUS + offs, &value, board_info);
        if (rc != VTSS_RC_OK)
            return rc;
        if (value != 0)
            continue;

        if (cmd == PHY_CMD_READ_INC || cmd == PHY_CMD_READ) {
            rc = jr2_rd(JR2_TGT_GCB, JR2_ADDR_MIIM_DATA + offs, &value, board_info);
            if (rc != VTSS_RC_OK)
                return rc;
            if (value & 0x30000)
                break;
            *data = (value & 0xffff);
        }
        return VTSS_RC_OK;
    }
    return VTSS_RC_ERROR;
}

/* Read PHY register on primary device */
static vtss_rc jr2_miim_rd(u8 ctrl, u8 miim_addr, u8 reg_addr, u16 *data,
                           vtss_board_info_t *board_info)
{
    return jr2_miim_cmd(PHY_CMD_READ_INC, 1, ctrl, miim_addr, reg_addr, data, board_info);
}

/* Detect Vitesse 10G PHY module on MIIM controller 0 */
static vtss_miim_controller_t jr2_phy_detect(u8 miim_addr, vtss_board_info_t *board_info)
{
    u16 tmp;
    u8 ctrl;
    for (ctrl = 0; ctrl < 2; ctrl++) {
        if (jr2_miim_rd(ctrl, miim_addr, 0, &tmp, board_info) == VTSS_RC_OK) {
            return (ctrl == 0 ? VTSS_MIIM_CONTROLLER_0 : VTSS_MIIM_CONTROLLER_1);
        }
    }
    PRINTD("Did not find an NPI Phy\n");
    return VTSS_MIIM_CONTROLLER_NONE; // Nothing found
}

/* Read MMD register on primary device */
static vtss_rc jr2_mmd_rd(u8 ctrl, u8 miim_addr, u8 mmd, u16 addr, u16 *value,
                          vtss_board_info_t *board_info)
{
    vtss_rc rc;

    rc = jr2_miim_cmd(PHY_CMD_ADDRESS, 0, ctrl, miim_addr, mmd, &addr, board_info);
    if (rc == VTSS_RC_OK)
        rc = jr2_miim_cmd(PHY_CMD_READ, 0, ctrl, miim_addr, mmd, value, board_info);
    return rc;
}


/* Detect Vitesse 10G PHY module on MIIM controller 0 */
static BOOL jr2_10g_detect(jr2_10g_detect_t *detect, vtss_board_info_t *board_info)
{
    u16 model, miim;
    u8 ctrl = 0;

    if (detect->miim_found[1]) {
        // Already found a VTSS PHY in previous round, just return the miim address
        detect->miim_addr[0] = detect->miim_addr[1];
        detect->cap = (PORT_CAP_VTSS_10G_PHY | PORT_CAP_10G_FDX | PORT_CAP_FLOW_CTRL | PORT_CAP_XAUI_LANE_FLIP | PORT_CAP_1G_FDX);
        return 1;
    }

    // Scan the miim bus for PHY targets, they come in pairs..
    for (miim = 0; miim < 32; miim++) {
        if (jr2_mmd_rd(ctrl, miim, 30, 0, &model, board_info) == VTSS_RC_OK) {
            if (model == 0x8484 || model == 0x8487 || model == 0x8488 ||
                model == 0x8489 || model == 0x8490 || model == 0x8491) {
                PRINTD(("Found VTSS PHY model 0x%x at port_no %u", model, detect->port_no));
                if (!detect->miim_found[0]) {
                    detect->miim_addr[1] = miim; // need to swap the channels to get them inline with JR2 ports
                    detect->miim_found[0] = 1;
                    detect->cap |= (PORT_CAP_VTSS_10G_PHY | PORT_CAP_1G_FDX | PORT_CAP_10G_FDX | PORT_CAP_FLOW_CTRL | PORT_CAP_XAUI_LANE_FLIP);
//                    detect->cap = (PORT_CAP_VTSS_10G_PHY | PORT_CAP_10G_FDX | PORT_CAP_FLOW_CTRL | PORT_CAP_XAUI_LANE_FLIP);
                } else {
                    detect->miim_addr[0] = miim;
                    detect->miim_found[1] = 1;
                    return 1;
                }
            }
        }
    }
    // VTSS Phy not found, look for X2 or Xenpak module at the given miim_addr
    if (jr2_mmd_rd(ctrl, detect->miim_addr[0], 1, 0, &model, board_info) == VTSS_RC_OK) {
        PRINTD(("Found XAUI device at at miim address:%d port_no %u", detect->miim_addr[0], detect->port_no));
        detect->cap = (PORT_CAP_10G_FDX | PORT_CAP_FLOW_CTRL | PORT_CAP_XAUI_LANE_FLIP);
        return 1;
    }
    return 0; // Nothing found
}



static void jr2_init_cu48(void)
{
    vtss_sgpio_conf_t conf;
    u32 port, gpio_no;

    // Configure GPIOs for MIIM/MDIO
    for (gpio_no = 56; gpio_no < 60; gpio_no++) {
        (void)vtss_gpio_mode_set(NULL, 0, gpio_no, VTSS_GPIO_ALT_0);
    }

    /* GPIO pins 0-3,4-5,30-33 are used for SGPIOs. */
    (void)vtss_gpio_mode_set(NULL, 0, 0, VTSS_GPIO_ALT_0);  // SGPIO Grp 0 / CLK
    (void)vtss_gpio_mode_set(NULL, 0, 1, VTSS_GPIO_ALT_0);  // SGPIO Grp 0 / DO
    (void)vtss_gpio_mode_set(NULL, 0, 2, VTSS_GPIO_ALT_0);  // SGPIO Grp 0 / DI
    (void)vtss_gpio_mode_set(NULL, 0, 3, VTSS_GPIO_ALT_0);  // SGPIO Grp 0 / LD
    (void)vtss_gpio_mode_set(NULL, 0, 4, VTSS_GPIO_ALT_0);  // SGPIO Grp 1 / CLK
    (void)vtss_gpio_mode_set(NULL, 0, 5, VTSS_GPIO_ALT_0);  // SGPIO Grp 1 / DO
    (void)vtss_gpio_mode_set(NULL, 0, 30, VTSS_GPIO_ALT_0); // SGPIO Grp 2 / Clk
    (void)vtss_gpio_mode_set(NULL, 0, 31, VTSS_GPIO_ALT_0); // SGPIO Grp 2 / LD
    (void)vtss_gpio_mode_set(NULL, 0, 32, VTSS_GPIO_ALT_0); // SGPIO Grp 2 / DO
    (void)vtss_gpio_mode_set(NULL, 0, 33, VTSS_GPIO_ALT_0); // SGPIO Grp 2 / DI
    (void)vtss_gpio_mode_set(NULL, 0, 54, VTSS_GPIO_IN_INT); // Enable 10G SFP Los Intr
    (void)vtss_gpio_mode_set(NULL, 0, 55, VTSS_GPIO_IN_INT); // Enable 10G SFP Los Intr

    /* Setup SGPIO group 0 */
    if (vtss_sgpio_conf_get(NULL, 0, 0, &conf) == VTSS_RC_OK) {
        /* The blink mode 0 is 5 HZ for link activity and collion in half duplex. */
        conf.bmode[0] = VTSS_SGPIO_BMODE_5;
        conf.bit_count = 2;
        for (port = 0; port < VTSS_SGPIO_PORTS; port++) {
            conf.port_conf[port].enabled = 1;
            conf.port_conf[port].mode[0] =  VTSS_SGPIO_MODE_ON;
            conf.port_conf[port].mode[1] =  VTSS_SGPIO_MODE_ON;
        }
        (void)vtss_sgpio_conf_set(NULL, 0, 0, &conf);
    }
    /* Setup SGPIO group 1 */
    if (vtss_sgpio_conf_get(NULL, 0, 1, &conf) == VTSS_RC_OK) {
        /* The blink mode 0 is 5 HZ for link activity and collion in half duplex. */
        conf.bmode[0] = VTSS_SGPIO_BMODE_5;
        conf.bit_count = 2;
        // QSGMII ports
        for (port = 0; port < 16; port++) {
            conf.port_conf[port].enabled = 1;
            conf.port_conf[port].mode[0] =  VTSS_SGPIO_MODE_ON;
            conf.port_conf[port].mode[1] =  VTSS_SGPIO_MODE_ON;
        }
        // SFP+ ports
        for (port = 17; port < 21; port++) {
            conf.port_conf[port].enabled = 1;
            conf.port_conf[port].mode[0] =  VTSS_SGPIO_MODE_ON;
            conf.port_conf[port].mode[1] =  VTSS_SGPIO_MODE_ON;
        }
        (void)vtss_sgpio_conf_set(NULL, 0, 1, &conf);
    }
    /* Setup SGPIO group 2 */
    if (vtss_sgpio_conf_get(NULL, 0, 2, &conf) == VTSS_RC_OK) {
        /* The blink mode 0 is 5 HZ for link activity and collion in half duplex. */
        conf.bmode[0] = VTSS_SGPIO_BMODE_5;
        conf.bit_count = 3;
        // 24:NPI, 25:X2 slot A, 26:X2 slot B, [31:27] TxEnable, Fan, SFP+ TxEnable, Rate select
        for (port = 24; port <32 ; port++) {
            conf.port_conf[port].enabled = 1;
        }
        /* X2 TXONOFF = ON */
        conf.port_conf[26].mode[2] = VTSS_SGPIO_MODE_ON;
        conf.port_conf[27].mode[0] = VTSS_SGPIO_MODE_ON;
        /* SFP Tx_disable = enabled */
        conf.port_conf[28].mode[0] = VTSS_SGPIO_MODE_ON;
        conf.port_conf[28].mode[1] = VTSS_SGPIO_MODE_ON;
        conf.port_conf[28].mode[2] = VTSS_SGPIO_MODE_ON;
        conf.port_conf[29].mode[0] = VTSS_SGPIO_MODE_ON;
        /* SFP RateSel = enabled    */
        conf.port_conf[29].mode[1] = VTSS_SGPIO_MODE_ON;
        conf.port_conf[29].mode[2] = VTSS_SGPIO_MODE_ON;
        conf.port_conf[30].mode[0] = VTSS_SGPIO_MODE_ON;
        conf.port_conf[30].mode[1] = VTSS_SGPIO_MODE_ON;
        conf.port_conf[30].mode[2] = VTSS_SGPIO_MODE_ON;
        conf.port_conf[31].mode[0] = VTSS_SGPIO_MODE_ON;
        conf.port_conf[31].mode[1] = VTSS_SGPIO_MODE_ON;
        conf.port_conf[31].mode[2] = VTSS_SGPIO_MODE_ON;

        (void)vtss_sgpio_conf_set(NULL, 0, 2, &conf);
    }

    dac_init();
}

static void jr2_init_sfp24(void)
{
    vtss_sgpio_conf_t conf;
    u32 port, gpio_no;

    // Configure GPIOs for MIIM/MDIO
    for (gpio_no = 56; gpio_no < 60; gpio_no++) {
        (void)vtss_gpio_mode_set(NULL, 0, gpio_no, VTSS_GPIO_ALT_0);
    }

    /* GPIO pins 0-3,4-5,30-33 are used for SGPIOs. */
    (void)vtss_gpio_mode_set(NULL, 0, 0, VTSS_GPIO_ALT_0);  // SGPIO Grp 0 / CLK
    (void)vtss_gpio_mode_set(NULL, 0, 1, VTSS_GPIO_ALT_0);  // SGPIO Grp 0 / DO
    (void)vtss_gpio_mode_set(NULL, 0, 2, VTSS_GPIO_ALT_0);  // SGPIO Grp 0 / DI
    (void)vtss_gpio_mode_set(NULL, 0, 3, VTSS_GPIO_ALT_0);  // SGPIO Grp 0 / LD
    (void)vtss_gpio_mode_set(NULL, 0, 4, VTSS_GPIO_ALT_0);  // SGPIO Grp 1 / CLK
    (void)vtss_gpio_mode_set(NULL, 0, 5, VTSS_GPIO_ALT_0);  // SGPIO Grp 1 / DO
    (void)vtss_gpio_mode_set(NULL, 0, 30, VTSS_GPIO_ALT_0); // SGPIO Grp 2 / Clk
    (void)vtss_gpio_mode_set(NULL, 0, 31, VTSS_GPIO_ALT_0); // SGPIO Grp 2 / LD
    (void)vtss_gpio_mode_set(NULL, 0, 32, VTSS_GPIO_ALT_0); // SGPIO Grp 2 / DO
    (void)vtss_gpio_mode_set(NULL, 0, 33, VTSS_GPIO_ALT_0); // SGPIO Grp 2 / DI

    (void)vtss_gpio_mode_set(NULL, 0, 28, VTSS_GPIO_OUT);
    (void)vtss_gpio_write(NULL, 0, 28, TRUE);
    (void)vtss_gpio_mode_set(NULL, 0, 29, VTSS_GPIO_OUT);
    (void)vtss_gpio_write(NULL, 0, 29, TRUE);
    (void)vtss_gpio_mode_set(NULL, 0, 49, VTSS_GPIO_OUT);
    (void)vtss_gpio_write(NULL, 0, 49, TRUE);
    (void)vtss_gpio_mode_set(NULL, 0, 51, VTSS_GPIO_OUT);
    (void)vtss_gpio_mode_set(NULL, 0, 52, VTSS_GPIO_OUT);
    (void)vtss_gpio_mode_set(NULL, 0, 53, VTSS_GPIO_OUT);


    /* Setup SGPIO group 0    */
    /* Controls Port LEDs 1-24*/
    if (vtss_sgpio_conf_get(NULL, 0, 0, &conf) == VTSS_RC_OK) {
        /* The blink mode 0 is 5 HZ for link activity and collion in half duplex. */
        conf.bmode[0] = VTSS_SGPIO_BMODE_5;
        conf.bit_count = 2;
        for (port = 0; port < 24; port++) {
            conf.port_conf[port].enabled = 1;
            conf.port_conf[port].mode[0] =  VTSS_SGPIO_MODE_ON;
            conf.port_conf[port].mode[1] =  VTSS_SGPIO_MODE_ON;
        }
        (void)vtss_sgpio_conf_set(NULL, 0, 0, &conf);
    }
    /* Setup SGPIO group 1 */
    /* Controls LEDs for SFP+ and the LED tower */
    if (vtss_sgpio_conf_get(NULL, 0, 1, &conf) == VTSS_RC_OK) {
        /* The blink mode 0 is 5 HZ for link activity and collion in half duplex. */
        conf.bmode[0] = VTSS_SGPIO_BMODE_5;
        conf.bit_count = 2;
        // LEDs for SFP+ and the LED tower
        for (port = 16; port < 24; port++) {
            conf.port_conf[port].enabled = 1;
        }
        (void)vtss_sgpio_conf_set(NULL, 0, 1, &conf);
    }
    /* Setup SGPIO group 2 */
    /* Controls LED for NPI, X2 LEDs and SFP signals */
    if (vtss_sgpio_conf_get(NULL, 0, 2, &conf) == VTSS_RC_OK) {
        /* The blink mode 0 is 5 HZ for link activity and collion in half duplex. */
        conf.bmode[0] = VTSS_SGPIO_BMODE_5;
        conf.bit_count = 3;
        for (port = 0; port < 16 ; port++) {
            conf.port_conf[port].enabled = 1; // SFPs
        }
        for (port = 24; port < 30 ; port++) {
            conf.port_conf[port].enabled = 1; // SFPs+
        }
        /* X2 TXONOFF = ON */
        conf.port_conf[10].mode[2] = VTSS_SGPIO_MODE_ON;
        conf.port_conf[11].mode[0] = VTSS_SGPIO_MODE_ON;
        /* SFP+ Tx_disable = enabled */
        conf.port_conf[12].mode[0] = VTSS_SGPIO_MODE_ON;
        conf.port_conf[12].mode[1] = VTSS_SGPIO_MODE_ON;
        conf.port_conf[12].mode[2] = VTSS_SGPIO_MODE_ON;
        conf.port_conf[13].mode[0] = VTSS_SGPIO_MODE_ON;
        /* SFP RateSel = enabled    */
        conf.port_conf[13].mode[1] = VTSS_SGPIO_MODE_ON;
        conf.port_conf[13].mode[2] = VTSS_SGPIO_MODE_ON;
        conf.port_conf[14].mode[0] = VTSS_SGPIO_MODE_ON;
        conf.port_conf[14].mode[1] = VTSS_SGPIO_MODE_ON;
        conf.port_conf[14].mode[2] = VTSS_SGPIO_MODE_ON;
        conf.port_conf[15].mode[0] = VTSS_SGPIO_MODE_ON;
        conf.port_conf[15].mode[1] = VTSS_SGPIO_MODE_ON;
        conf.port_conf[15].mode[2] = VTSS_SGPIO_MODE_ON;

        conf.port_conf[24].mode[0] = VTSS_SGPIO_MODE_ON; // NPI
        conf.port_conf[25].mode[1] = VTSS_SGPIO_MODE_ON; // X2 A
        conf.port_conf[26].mode[0] = VTSS_SGPIO_MODE_ON; // X2 B

        (void)vtss_sgpio_conf_set(NULL, 0, 2, &conf);
    }

    dac_init();
}

static void jr2_init_nid(void)
{
    vtss_sgpio_conf_t conf;
    u32               port, gpio;

    /* GPIO pins 0-3 are used for SGPIOs. */
    (void)vtss_gpio_mode_set(NULL, 0, 0, VTSS_GPIO_ALT_0);  // SGPIO Grp 0 / CLK
    (void)vtss_gpio_mode_set(NULL, 0, 1, VTSS_GPIO_ALT_0);  // SGPIO Grp 0 / DO
    (void)vtss_gpio_mode_set(NULL, 0, 2, VTSS_GPIO_ALT_0);  // SGPIO Grp 0 / DI
    (void)vtss_gpio_mode_set(NULL, 0, 3, VTSS_GPIO_ALT_0);  // SGPIO Grp 0 / LD

    (void)vtss_gpio_mode_set(NULL, 0, 30, VTSS_GPIO_ALT_0); // SGPIO Grp 2 / Clk
    (void)vtss_gpio_mode_set(NULL, 0, 31, VTSS_GPIO_ALT_0); // SGPIO Grp 2 / LD
    (void)vtss_gpio_mode_set(NULL, 0, 32, VTSS_GPIO_ALT_0); // SGPIO Grp 2 / DO
    (void)vtss_gpio_mode_set(NULL, 0, 33, VTSS_GPIO_ALT_0); // SGPIO Grp 2 / DI

    for (gpio = 44; gpio <= 59; gpio++) {
        if (gpio > 49 && gpio < 56) {
            continue;
        }
        (void)vtss_gpio_mode_set(NULL, 0, gpio, VTSS_GPIO_IN_INT); // Enable SFP Los interrupt
    }

    /* Setup SGPIO group 0 used for SFP signals */
    if (vtss_sgpio_conf_get(NULL, 0, 0, &conf) == VTSS_RC_OK) {
        conf.bmode[0] = VTSS_SGPIO_BMODE_5;
        conf.bit_count = 2;
        for (port = 0; port < 16; port++) {
            conf.port_conf[port].enabled = 1;
            conf.port_conf[port].mode[0] =  VTSS_SGPIO_MODE_ON;
            conf.port_conf[port].mode[1] =  VTSS_SGPIO_MODE_ON;
        }
        (void)vtss_sgpio_conf_set(NULL, 0, 0, &conf);
    }

    /* Setup SGPIO group 1 is not used */
    if (vtss_sgpio_conf_get(NULL, 0, 1, &conf) == VTSS_RC_OK) {
        conf.bmode[0] = VTSS_SGPIO_BMODE_5;
        (void)vtss_sgpio_conf_set(NULL, 0, 1, &conf);
    }

    /* Setup SGPIO group 2 used for LEDs */
    if (vtss_sgpio_conf_get(NULL, 0, 2, &conf) == VTSS_RC_OK) {
        /* The blink mode 0 is 5 HZ for link activity and collion in half duplex. */
        conf.bmode[0] = VTSS_SGPIO_BMODE_5;
        conf.bit_count = 2;
        // LEDs
        for (port = 0; port < 16 ; port++) {
            conf.port_conf[port].enabled = 1;
        }
        for (port = 21; port < 29 ; port++) {
            conf.port_conf[port].enabled = 1;
        }
        (void)vtss_sgpio_conf_set(NULL, 0, 2, &conf);
    }

}

static void jr2_init_aqr(void)
{
    vtss_sgpio_conf_t conf;
    u32 port, gpio_no;

    // Configure GPIOs for MIIM/MDIO
    for (gpio_no = 56; gpio_no < 60; gpio_no++) {
        (void)vtss_gpio_mode_set(NULL, 0, gpio_no, VTSS_GPIO_ALT_0);
    }

    (void)vtss_gpio_mode_set(NULL, 0, 21, VTSS_GPIO_OUT);  // SGPIO Grp 0 / DO
    (void)vtss_gpio_mode_set(NULL, 0, 23, VTSS_GPIO_OUT);  // SGPIO Grp 0 / DO

    (void)vtss_gpio_mode_set(NULL, 0, 30, VTSS_GPIO_ALT_0); // SGPIO Grp 2 / Clk
    (void)vtss_gpio_mode_set(NULL, 0, 31, VTSS_GPIO_ALT_0); // SGPIO Grp 2 / LD
    (void)vtss_gpio_mode_set(NULL, 0, 32, VTSS_GPIO_ALT_0); // SGPIO Grp 2 / DO
    (void)vtss_gpio_mode_set(NULL, 0, 33, VTSS_GPIO_ALT_0); // SGPIO Grp 2 / DI

    (void)vtss_gpio_write(NULL, 0, 21, 0);
    (void)vtss_gpio_write(NULL, 0, 23, 0);

    /* Setup SGPIO group 0     */
    /* Controls Port LEDs 1-26 */
    if (vtss_sgpio_conf_get(NULL, 0, 0, &conf) == VTSS_RC_OK) {
        /* The blink mode 0 is 5 HZ for link activity and collion in half duplex. */
        conf.bmode[0] = VTSS_SGPIO_BMODE_5;
        conf.bit_count = 2;
        for (port = 0; port < VTSS_PORTS && port < VTSS_SGPIO_PORTS; port++) {
            conf.port_conf[port].enabled = 1;
        }
        (void)vtss_sgpio_conf_set(NULL, 0, 0, &conf);
    }
}

static vtss_rc jr2_post_reset(void)
{
    if (jr2_port_table[0].map.miim_controller != VTSS_MIIM_CONTROLLER_NONE) {
        VTSS_RC(vtss_phy_post_reset(PHY_INST, 0));
    }
    return VTSS_RC_OK;
}

static vtss_rc jr2_post_reset_aqr(void)
{
    return VTSS_RC_OK;
}

static void jr2_pre_reset(void)
{
}

//**************************************************************************************
//* I2C
//**************************************************************************************

//
// Serval 2 nid board
//
// Getting the i2c clock for a specific port
static vtss_rc i2c_clk_sel_get(vtss_port_no_t port, u32 *i2c_clk_sel) {
    // See Table 263 in data sheet + schematic
    switch (port) {
    case 4: *i2c_clk_sel = 34; break; // Port 5 = SFP 0
    case 5: *i2c_clk_sel = 35; break;
    case 6: *i2c_clk_sel = 36; break;
    case 7: *i2c_clk_sel = 37; break;
    case 8: *i2c_clk_sel = 38; break;
    case 9: *i2c_clk_sel = 39; break;
    case 10:*i2c_clk_sel = 40; break;
    case 11:*i2c_clk_sel = 41; break;
    case 12:*i2c_clk_sel = 42; break;
    case 13:*i2c_clk_sel = 43; break;
    default :return VTSS_RC_ERROR;
    }
    return VTSS_RC_OK;
}

static vtss_rc srv2_nid_i2c_read(vtss_port_no_t port_no, u8 i2c_addr, u8 addr, u8 *const data, u8 cnt, BOOL word_access)
{
    port_custom_entry_t *entry = &jr2_port_table[port_no];
    u32                 i2c_clk_sel;

    PRINTD(("port:%d\n", port_no));

    if (entry->cap & PORT_CAP_DUAL_COPPER) {
        return vtss_phy_i2c_read(NULL, port_no, port_no, addr, i2c_addr, data, cnt, word_access);
    }

    VTSS_RC(i2c_clk_sel_get(port_no, &i2c_clk_sel));

    PRINTD(("i2c_clk_sel:%d\n", i2c_clk_sel));
    if (jr2_i2c_read != NULL) {
        PRINTD(("i2c_addr:0x%X, i2c_clk_sel:%d, cnd:%d, addr:%0xX\n", i2c_addr, i2c_clk_sel, cnt, addr));
        return jr2_i2c_read(port_no, i2c_addr, addr, data, cnt, i2c_clk_sel);
    }

    return VTSS_RC_OK;
}

static vtss_rc srv2_nid_i2c_write(vtss_port_no_t port_no, u8 i2c_addr, u8 addr, u8 *const data, BOOL word_access)
{
    port_custom_entry_t *entry = &jr2_port_table[port_no];
    u32                 i2c_clk_sel;
    u8                  i2c_data[3];

    if (entry->cap & PORT_CAP_DUAL_COPPER) {
        return vtss_phy_i2c_write(NULL, port_no, port_no, addr, i2c_addr, data, 2, word_access);
    }

    VTSS_RC(i2c_clk_sel_get(port_no, &i2c_clk_sel));

    if (jr2_i2c_write != NULL) {
        i2c_data[0] = addr;
        memcpy(&i2c_data[1], data, 2);
        return jr2_i2c_write(port_no, i2c_addr, i2c_data, 3, i2c_clk_sel);
    }
    return VTSS_RC_OK;
}

static BOOL get_sfp_status(vtss_port_no_t port_no, vtss_sgpio_port_data_t *data, sfp_signal_t sfp)
{
    u32 chip_port = jr2_port_table[port_no].map.chip_port;

    if (jr2_board_type == VTSS_BOARD_SERVAL2_NID_REF) {

        if (sfp == SFP_DETECT) {
            switch (chip_port) { // The Present signal is inverted
            case 13: return !data[0].value[0];
            case 12: return !data[0].value[1];
            case 15: return !data[1].value[0];
            case 14: return !data[1].value[1];
            case 17: return !data[2].value[0];
            case 16: return !data[2].value[1];
            case 19: return !data[3].value[0];
            case 18: return !data[3].value[1];
            case 50: return !data[8].value[0];
            case 49: return !data[8].value[1];
            }
            return 0;
        } else if (sfp == SFP_FAULT) {
            switch (chip_port) {
            case 13: return data[4].value[0];
            case 12: return data[4].value[1];
            case 15: return data[5].value[0];
            case 14: return data[5].value[1];
            case 17: return data[6].value[0];
            case 16: return data[6].value[1];
            case 19: return data[7].value[0];
            case 18: return data[7].value[1];
            case 50: return data[10].value[0];
            case 49: return data[10].value[1];
            }
            return 0;
        } else {
            BOOL val = 0;
            // SFP_LOS
            switch (chip_port) {
            case 13: (void)vtss_gpio_read(NULL, 0, 44, &val); break;
            case 12: (void)vtss_gpio_read(NULL, 0, 45, &val); break;
            case 15: (void)vtss_gpio_read(NULL, 0, 46, &val); break;
            case 14: (void)vtss_gpio_read(NULL, 0, 47, &val); break;
            case 17: (void)vtss_gpio_read(NULL, 0, 48, &val); break;
            case 16: (void)vtss_gpio_read(NULL, 0, 49, &val); break;
            case 19: (void)vtss_gpio_read(NULL, 0, 56, &val); break;
            case 18: (void)vtss_gpio_read(NULL, 0, 57, &val); break;
            case 50: (void)vtss_gpio_read(NULL, 0, 58, &val); break;
            case 49: (void)vtss_gpio_read(NULL, 0, 59, &val); break;
            }
            return val;
        }

    } else if (jr2_board_type == VTSS_BOARD_JAGUAR2_REF) {
        u32 indx = (sfp == SFP_DETECT) ? 1 : (sfp == SFP_FAULT) ? 2 : 0;
        if (chip_port >= 8 && chip_port <= 23) {
            if (sfp == SFP_DETECT) {
                return !data[chip_port - 8].value[1];
            } else {
                return data[chip_port - 8].value[indx];
            }
        } else if (chip_port >= 49 && chip_port <= 52) {
            if (sfp == SFP_DETECT) {
                return !data[chip_port - 24].value[indx];
            } else {
                return data[chip_port - 24].value[indx];
            }
        }
    } else {
        // VTSS_BOARD_JAGUAR2_CU48_REF || VTSS_BOARD_JAGUAR2_AQR_REF
        if (sfp == SFP_LOS) {
            BOOL val = 0;
            // SFP_LOS
            switch (chip_port) {
            case 49: (void)vtss_gpio_read(NULL, 0, 52, &val); break;
            case 50: (void)vtss_gpio_read(NULL, 0, 53, &val); break;
            case 51: (void)vtss_gpio_read(NULL, 0, 54, &val); break;
            case 52: (void)vtss_gpio_read(NULL, 0, 55, &val); break;
            }
            return val;
        } else {
            u32 indx = (sfp == SFP_DETECT) ? 1 : (sfp == SFP_FAULT) ? 2 : 0;
            if (chip_port >= 8 && chip_port <= 23) {
                if (sfp == SFP_DETECT) {
                    return !data[chip_port - 8].value[1];
                } else {
                    return data[chip_port - 8].value[indx];
                }
            } else if (chip_port >= 49 && chip_port <= 52) {
                if (sfp == SFP_DETECT) {
                    return !data[chip_port - 24].value[indx];
                } else {
                    return data[chip_port - 24].value[indx];
                }
            }
        }
    }
    return 0;
}

// Get module: Present / LOS / TxFault signals
static vtss_rc jr2_sfp_status(port_custom_sfp_status_t *sfp_status)
{
    vtss_sgpio_port_data_t data[VTSS_SGPIO_PORTS];
    vtss_port_no_t         port_no;
    port_custom_entry_t    *entry;
    u32 sgpio_group = (jr2_board_type == VTSS_BOARD_SERVAL2_NID_REF) ? 0 : 2;

    if (vtss_sgpio_read(NULL, 0, sgpio_group, data) != VTSS_RC_OK) {
        return VTSS_RC_ERROR;
    }
    for (port_no = VTSS_PORT_NO_START; port_no < VTSS_PORT_NO_END; port_no++) {
        entry = &jr2_port_table[port_no];
        if (entry->cap & PORT_CAP_SFP_DETECT) {
            sfp_status[port_no].present  = get_sfp_status(port_no, data, SFP_DETECT);
            sfp_status[port_no].tx_fault = get_sfp_status(port_no, data, SFP_FAULT);
            sfp_status[port_no].los      = get_sfp_status(port_no, data, SFP_LOS);
        } else {
            sfp_status[port_no].present  = 0;
            sfp_status[port_no].los      = 0;
            sfp_status[port_no].tx_fault = 0;
        }
    }
    return VTSS_RC_OK;
}

static vtss_rc srv2_nid_sfp_mod_detect(BOOL *detect_status)
{
    vtss_port_no_t port_no;
    vtss_sgpio_port_data_t data[VTSS_SGPIO_PORTS];

    if (vtss_sgpio_read(NULL, 0, 0, data) != VTSS_RC_OK) {
        return VTSS_RC_ERROR;
    }

    /* The status in 'data' is inverted i.e. '0' means detected */
    /* We return '1' when a module is detected */
    for (port_no = VTSS_PORT_NO_START; port_no < VTSS_PORT_NO_END; port_no++) {
        // iPort 4 = SFP 0
        switch (port_no) {
        case 4:  detect_status[port_no] = !data[0].value[0]; break; // Schematic U36
        case 5:  detect_status[port_no] = !data[0].value[1]; break; // Schematic U36
        case 6:  detect_status[port_no] = !data[1].value[0]; break; // Schematic U36
        case 7:  detect_status[port_no] = !data[1].value[1]; break; // Schematic U36
        case 8:  detect_status[port_no] = !data[2].value[0]; break; // Schematic U36
        case 9:  detect_status[port_no] = !data[2].value[1]; break; // Schematic U36
        case 10: detect_status[port_no] = !data[3].value[0]; break; // Schematic U36
        case 11: detect_status[port_no] = !data[3].value[1]; break; // Schematic U36
        case 12: detect_status[port_no] = !data[8].value[0]; break; // Schematic U28
        case 13: detect_status[port_no] = !data[8].value[1]; break; // Schematic U28
        }
        PRINTD(("port:%d, status:%d\n", port_no, detect_status[port_no]));
    }

    return VTSS_RC_OK;
}

static vtss_rc srv2_nid_i2c_enable(vtss_port_no_t port_no)
{
    return VTSS_RC_OK;

}

//
// SFP board
//
static BOOL jr2_sfp_accept(u8 *sfp_rom)
{
    return TRUE;
}

// Function for doing SFP i2c reads.
// In : port_no - The physical port number
//      i2c_addr - The address for the i2c device
//      addr     - The address with the i2c device to access.
//      cnt      - The number of data to read
//      word     - Not Used here
// In/Out  data     - The data read
static vtss_rc jr2_sfp_i2c_read(vtss_port_no_t port_no, u8 i2c_addr, u8 addr, u8 *const data, u8 cnt, BOOL word_access)
{
    u32 chip_port = jr2_port_table[port_no].map.chip_port;
    if (jr2_i2c_read != NULL) {
        /* Custom board SFP i2c enable function */
        if (board_sfp_i2c_enable(port_no) != VTSS_RC_OK) {
            return VTSS_RC_ERROR;
        }

        // (SFP+ port 1), GPIO18 (SFP+ port 2), GPIO20 (SFP+ port 3) og GPIO21 (SFP+ port 4).
        PRINTD(("i2c read, port:%d, i2c_addr:%d, cnt:%d", port_no, addr, cnt));
        if (chip_port >= 8 && chip_port <= 32) {
            PRINTD(("port number :%d, external mux", port_no));
            // External mux'es. See the enable function
            return jr2_i2c_read(port_no, i2c_addr, addr, data, cnt, VTSS_I2C_NO_MULTIPLEXER);
        } else if(chip_port >= 49 && chip_port <= 52) { // SFP+ ports
            // Internal mux.
            i8 gpio = 17;

            if (jr2_board_type == VTSS_BOARD_JAGUAR2_AQR_REF) {
                if (chip_port == 51) {
                    gpio = 17;
                } else if (chip_port == 52) {
                    gpio = 16;
                } else {
                    PRINTD(("i2c read - unknown port number :%d", port_no));
                    return VTSS_RC_ERROR;
                }
            } else {
                // See schematic for the GPIO numbers
                switch (chip_port) {
                case 49: gpio = 17; break;
                case 50: gpio = 18; break;
                case 51: gpio = 20; break;
                case 52: gpio = 21; break;
                }
            }
            PRINTD(("port number :%d, gpio:%d", port_no, gpio));
            return jr2_i2c_read(port_no, i2c_addr, addr, data, cnt, gpio);
        } else {
            PRINTD(("i2c read - unknown port number :%d", port_no));
            return VTSS_RC_ERROR;
        }
    }
    return VTSS_RC_OK;
}

// Function for doing SFP i2c writes
// In : port_no - The physical port number
//      i2c_addr - The address for the i2c device
//      addr     - The address with the i2c device to access.
//      cnt      - The number of data to read
//      word     - Not Used here
//      data     - The data to write
static vtss_rc jr2_sfp_i2c_write(vtss_port_no_t port_no, u8 i2c_addr, u8 addr, u8 *const data, BOOL word_access)
{
    u32 chip_port = jr2_port_table[port_no].map.chip_port;
    u8 i2c_data[3];
    if (jr2_i2c_write != NULL) {
        /* Custom board SFP i2c enable function */
        if (board_sfp_i2c_enable(port_no) != VTSS_RC_OK) {
            return VTSS_RC_ERROR;
        }

        i2c_data[0] = addr;
        memcpy(&i2c_data[1], data, 2);

        if (chip_port < 32) {
            PRINTD(("port number :%d, external mux", port_no));
            // External mux'es. See the enable function
            return jr2_i2c_write(port_no, i2c_addr, i2c_data, 3, VTSS_I2C_NO_MULTIPLEXER);
        } else if(chip_port >= 49 && chip_port <= 52) {
            // Internal mux.
            i8 gpio = 17;

            // See schematic for the GPIO numbers
            if (jr2_board_type == VTSS_BOARD_JAGUAR2_AQR_REF) {
                if (chip_port == 51) {
                    gpio = 17;
                } else if (chip_port == 52) {
                    gpio = 16;
                } else {
                    PRINTD(("i2c write - unknown port number :%d", port_no));
                    return VTSS_RC_ERROR;
                }
            } else {
                switch (chip_port) {
                case 49: gpio = 17; break;
                case 50: gpio = 18; break;
                case 51: gpio = 20; break;
                case 52: gpio = 21; break;
                }
            }
            PRINTD(("port number :%d, gpio:%d", port_no, gpio));
            return jr2_i2c_write(port_no, i2c_addr, i2c_data, 3, gpio);
        } else {
            PRINTD(("i2c write - unknown port number :%d", port_no));
            return VTSS_RC_ERROR;
        }
    }
    return VTSS_RC_OK;
}

static void jr2_sfp_update_if(vtss_port_no_t port_no, vtss_port_interface_t mac_if)
{
    jr2_port_table[port_no].mac_if = mac_if;
}

static vtss_rc jr2_sfp_mod_detect(BOOL *detect_status)
{
    vtss_port_no_t port_no;
    vtss_sgpio_port_data_t data[VTSS_SGPIO_PORTS];
    u32 chip_port;

    if (jr2_board_type == VTSS_BOARD_JAGUAR2_AQR_REF) {
        // Not use on Atlanta reference board.
        memset(detect_status, 0, sizeof(*detect_status));
        return VTSS_RC_OK;
    }

    if (vtss_sgpio_read(NULL, 0, 2, data) != VTSS_RC_OK) {
        return VTSS_RC_ERROR;
    }

    /* The status in 'data' is inverted i.e. '0' means detected */
    /* We return '1' when a module is detected */
    for (port_no = VTSS_PORT_NO_START; port_no < VTSS_PORT_NO_END; port_no++) {
        chip_port = jr2_port_table[port_no].map.chip_port;

        // SGPIO2 port/bit	Input	Output
        // 0b0	Port 9 (SFP) LOS (1=LOS from SFP)	Port 9 (SFP) TXEN (1=enable)
        // 0b1	Port 9 (SFP) MODDETn (0=SFP present)	Port 9 (SFP) RATESEL
        // 0b2	Port 9 (SFP) TXFAULT (1=TX fault)	Port 10 (SFP) TXEN (1=enable)
        // 1b0	Port 10 (SFP) LOS (1=LOS from SFP)	Port 10 (SFP) RATESEL
        // 1b1	Port 10 (SFP) MODDETn (0=SFP present)	Port 11 (SFP) TXEN (1=enable)
        // 1b2	Port 10 (SFP) TXFAULT (1=TX fault)	Port 11 (SFP) RATESEL
        // 2b0	Port 11 (SFP) LOS (1=LOS from SFP)	Port 12 (SFP) TXEN (1=enable)
        // 2b1	Port 11 (SFP) MODDETn (0=SFP present)	Port 12 (SFP) RATESEL
        // 2b2	Port 11 (SFP) TXFAULT (1=TX fault)	Port 13 (SFP) TXEN (1=enable)
        // 3b0	Port 12 (SFP) LOS (1=LOS from SFP)	Port 13 (SFP) RATESEL
        // 3b1	Port 12 (SFP) MODDETn (0=SFP present)	Port 14 (SFP) TXEN (1=enable)
        // 3b2	Port 12 (SFP) TXFAULT (1=TX fault)	Port 14 (SFP) RATESEL
        // 4b0	Port 13 (SFP) LOS (1=LOS from SFP)	Port 15 (SFP) TXEN (1=enable)
        // 4b1	Port 13 (SFP) MODDETn (0=SFP present)	Port 15 (SFP) RATESEL
        // 4b2	Port 13 (SFP) TXFAULT (1=TX fault)	Port 16 (SFP) TXEN (1=enable)
        // 5b0	Port 14 (SFP) LOS (1=LOS from SFP)	Port 16 (SFP) RATESEL
        // 5b1	Port 14 (SFP) MODDETn (0=SFP present)	Port 17 (SFP) TXEN (1=enable)
        // 5b2	Port 14 (SFP) TXFAULT (1=TX fault)	Port 17 (SFP) RATESEL
        // 6b0	Port 15 (SFP) LOS (1=LOS from SFP)	Port 18 (SFP) TXEN (1=enable)
        // 6b1	Port 15 (SFP) MODDETn (0=SFP present)	Port 18 (SFP) RATESEL
        // 6b2	Port 15 (SFP) TXFAULT (1=TX fault)	Port 19 (SFP) TXEN (1=enable)
        // 7b0	Port 16 (SFP) LOS (1=LOS from SFP)	Port 19 (SFP) RATESEL
        // 7b1	Port 16 (SFP) MODDETn (0=SFP present)	Port 20 (SFP) TXEN (1=enable)
        // 7b2	Port 16 (SFP) TXFAULT (1=TX fault)	Port 20 (SFP) RATESEL
        // 8b0	Port 17 (SFP) LOS (1=LOS from SFP)	Port 21 (SFP) TXEN (1=enable)
        // 8b1	Port 17 (SFP) MODDETn (0=SFP present)	Port 21 (SFP) RATESEL
        // 8b2	Port 17 (SFP) TXFAULT (1=TX fault)	Port 22 (SFP) TXEN (1=enable)
        // 9b0	Port 18 (SFP) LOS (1=LOS from SFP)	Port 22 (SFP) RATESEL
        // 9b1	Port 18 (SFP) MODDETn (0=SFP present)	Port 23 (SFP) TXEN (1=enable)
        // 9b2	Port 18 (SFP) TXFAULT (1=TX fault)	Port 23 (SFP) RATESEL
        // 10b0	Port 19 (SFP) LOS (1=LOS from SFP)	Port 24 (SFP) TXEN (1=enable)
        // 10b1	Port 19 (SFP) MODDETn (0=SFP present)	Port 24 (SFP) RATESEL
        // 10b2	Port 19 (SFP) TXFAULT (1=TX fault)	X2_TXONOFF_A
        // 11b0	Port 20 (SFP) LOS (1=LOS from SFP)	X2_TXONOFF_B
        // 11b1	Port 20 (SFP) MODDETn (0=SFP present)	FAN1_ENn (0=enable)
        // 11b2	Port 20 (SFP) TXFAULT (1=TX fault)	FAN2_ENn (0=enable)
        // 12b0	Port 21 (SFP) LOS (1=LOS from SFP)	SFP+ 1 TXEN (1=enable)
        // 12b1	Port 21 (SFP) MODDETn (0=SFP present)	SFP+ 2 TXEN (1=enable)
        // 12b2	Port 21 (SFP) TXFAULT (1=TX fault)	SFP+ 3 TXEN (1=enable)
        // 13b0	Port 22 (SFP) LOS (1=LOS from SFP)	SFP+ 4 TXEN (1=enable)
        // 13b1	Port 22 (SFP) MODDETn (0=SFP present)	SFP+ 1 RateSel0
        // 13b2	Port 22 (SFP) TXFAULT (1=TX fault)	SFP+ 1 RateSel1
        // 14b0	Port 23 (SFP) LOS (1=LOS from SFP)	SFP+ 2 RateSel0
        // 14b1	Port 23 (SFP) MODDETn (0=SFP present)	SFP+ 2 RateSel1
        // 14b2	Port 23 (SFP) TXFAULT (1=TX fault)	SFP+ 3 RateSel0
        // 15b0	Port 24 (SFP) LOS (1=LOS from SFP)	SFP+ 3 RateSel1
        // 15b1	Port 24 (SFP) MODDETn (0=SFP present)	SFP+ 4 RateSel0
        // 15b2	Port 24 (SFP) TXFAULT (1=TX fault)	SFP+ 4 RateSel1
        // 24b0	nINT_PHY1 (PHY for ports 1-4)	NPI LED, green (1=on)
        // 24b1	nINT_PHY2 (PHY for ports 5-8)	NPI LED, yellow (1=on)
        // 24b2	nMODULE_INT (interrupt for X2, Malibu, NPI, SyncE)	not used
        // 25b0	SFP+ 1 LOS (1=LOS from SFP+)	X2_A LED, green (1=on)
        // 25b1	SFP+ 1 MODDETn (0=SFP+ present)	X2_A LED, yellow (1=on)
        // 25b2	SFP+ 1 TXFAULT (1=TX fault)	not used
        // 26b0	SFP+ 2 LOS (1=LOS from SFP+)	X2_B LED, green (1=on)
        // 26b1	SFP+ 2 MODDETn (0=SFP+ present)	X2_B LED, yellow (1=on)
        // 26b2	SFP+ 2 TXFAULT (1=TX fault)	not used
        // 27b0	SFP+ 3 LOS (1=LOS from SFP+)	not used
        // 27b1	SFP+ 3 MODDETn (0=SFP+ present)	not used
        // 27b2	SFP+ 3 TXFAULT (1=TX fault)	not used
        // 28b0	SFP+ 4 LOS (1=LOS from SFP+)	not used
        // 28b1	SFP+ 4 MODDETn (0=SFP+ present)	not used
        // 28b2	SFP+ 4 TXFAULT (1=TX fault)	not used
        // 29b0	SyncE module PRESENT_n	not used
        // 29b1	Ext CPU module PRESENTn	not used
        // 29b2	not used	not used

        if (chip_port >= 8 && chip_port <= 23) {
            detect_status[port_no] = !data[chip_port - 8].value[1]; // Schematic U142
        } else if (chip_port >= 49 && chip_port <= 52) {
            detect_status[port_no] = !data[chip_port - 24].value[1];
        }
    }
    return VTSS_RC_OK;
}

static vtss_rc jr2_sfp_i2c_enable(vtss_port_no_t port_no)
{
    u32 chip_port = jr2_port_table[port_no].map.chip_port;

    // ***** See schematic PCB110 *****
    vtss_rc                rc = VTSS_RC_OK;
    const vtss_gpio_no_t i2cmux2_en_gpio  = 59;
    const vtss_gpio_no_t i2cmux1_en_gpio  = 58;
    const vtss_gpio_no_t i2cmux_sel2_gpio = 53;
    const vtss_gpio_no_t i2cmux_sel1_gpio = 52;
    const vtss_gpio_no_t i2cmux_sel0_gpio = 51;

    u8 pca_9545a_i2c_addr = 0x70; // At the X2 plug-in module (PCB080) there is a i2c chip, which is used to
                                  // select which SFP to access. The i2c address is automatically set depending upon module slot.
                                  // Look for PRTAD0 and PRTAD1 at the schematic.

    u8 pca_9545a_data = 0x0; // The Control register for selecting SFP. See pca9545a data-sheet table 4.

    // Start by disabling all external i2c muxes
    VTSS_RC(vtss_gpio_mode_set(NULL, 0, i2cmux2_en_gpio,  VTSS_GPIO_OUT));  // I2CMUX2_ENn
    VTSS_RC(vtss_gpio_mode_set(NULL, 0, i2cmux1_en_gpio,  VTSS_GPIO_OUT));  // I2CMUX1_ENn
    VTSS_RC(vtss_gpio_write(NULL, 0, i2cmux2_en_gpio,  1));
    VTSS_RC(vtss_gpio_write(NULL, 0, i2cmux1_en_gpio,  1));

    (void) jr2_i2c_write(port_no, 0x70, &pca_9545a_data, 1, VTSS_I2C_NO_MULTIPLEXER);
    (void) jr2_i2c_write(port_no, 0x71, &pca_9545a_data, 1, VTSS_I2C_NO_MULTIPLEXER);

    if (chip_port >= 8 && chip_port <= 23) {
        // I2C For the 16 front SFP ports goes through multiplexers. (like PCB083 (Jaguar1 SFP24))
        // First setup multiplexer though  MUX_SEL0 (GPIO51), MUX_SEL1 (GPIO52), MUX_SEL2 (GPIO53), I2CMUX1_ENn (GPIO58) og I2CMUX2_ENn (GPIO59):

        //                  I2CMUX2_ENn	I2CMUX1_ENn	MUX_SEL2	MUX_SEL1	MUX_SEL0
        BOOL table[16][5]={{1,	        0,	        0,	        0,	        0}, // SFP 9
                           {1,	        0,	        0,	        0,	        1}, // SFP 10
                           {1,	        0,	        0,	        1,	        0}, // SFP 11
                           {1,	        0,	        0,	        1,	        1}, // SFP 12
                           {1,	        0,	        1,	        0,	        0}, // SFP 13
                           {1,	        0,	        1,	        0,	        1}, // SFP 14
                           {1,	        0,	        1,	        1,	        0}, // SFP 15
                           {1,	        0,	        1,	        1,	        1}, // SFP 16
                           {0,	        1,	        0,	        0,	        0}, // SFP 17
                           {0,	        1,	        0,	        0,	        1}, // SFP 18
                           {0,	        1,	        0,	        1,	        0}, // SFP 19
                           {0,	        1,	        0,	        1,	        1}, // SFP 20
                           {0,	        1,	        1,	        0,	        0}, // SFP 21
                           {0,	        1,	        1,	        0,	        1}, // SFP 22
                           {0,	        1,	        1,	        1,	        0}, // SFP 23
                           {0,	        1,	        1,	        1,	        1}}; // SFP 24


        // The access the SFP you have addressed above, through the default i2c controller GPIO15 (SDA) og GPIO16 (SCL).

        // I2C for the  4 SFP+ ports goes through J2's multiplexed I2C controller, where SCL comes from GPIO17 (SFP+ port 1), GPIO18 (SFP+ port 2), GPIO20 (SFP+ port 3) og GPIO21 (SFP+ port 4).

        VTSS_RC(vtss_gpio_mode_set(NULL, 0, i2cmux2_en_gpio,  VTSS_GPIO_OUT));  // I2CMUX2_ENn
        VTSS_RC(vtss_gpio_mode_set(NULL, 0, i2cmux1_en_gpio,  VTSS_GPIO_OUT));  // I2CMUX1_ENn
        VTSS_RC(vtss_gpio_mode_set(NULL, 0, i2cmux_sel2_gpio, VTSS_GPIO_OUT)); // MUX_SEL2
        VTSS_RC(vtss_gpio_mode_set(NULL, 0, i2cmux_sel1_gpio, VTSS_GPIO_OUT)); // MUX_SEL1
        VTSS_RC(vtss_gpio_mode_set(NULL, 0, i2cmux_sel0_gpio, VTSS_GPIO_OUT)); // MUX_SEL0
        VTSS_RC(vtss_gpio_direction_set(NULL, 0, i2cmux2_en_gpio,  TRUE));
        VTSS_RC(vtss_gpio_direction_set(NULL, 0, i2cmux1_en_gpio,  TRUE));
        VTSS_RC(vtss_gpio_direction_set(NULL, 0, i2cmux_sel2_gpio, TRUE));
        VTSS_RC(vtss_gpio_direction_set(NULL, 0, i2cmux_sel1_gpio, TRUE));
        VTSS_RC(vtss_gpio_direction_set(NULL, 0, i2cmux_sel0_gpio, TRUE));

        u8 port_lookup = chip_port - 8; // Adjusting so we can lookup in the table above

        VTSS_RC(vtss_gpio_write(NULL, 0, i2cmux2_en_gpio,  table[port_lookup][0]));
        VTSS_RC(vtss_gpio_write(NULL, 0, i2cmux1_en_gpio,  table[port_lookup][1]));
        VTSS_RC(vtss_gpio_write(NULL, 0, i2cmux_sel2_gpio, table[port_lookup][2]));
        VTSS_RC(vtss_gpio_write(NULL, 0, i2cmux_sel1_gpio, table[port_lookup][3]));
        VTSS_RC(vtss_gpio_write(NULL, 0, i2cmux_sel0_gpio, table[port_lookup][4]));

        PRINTD(("port:%d, setup multiplexer. %d, %d, %d, %d, %d\n", port_no, table[port_lookup][0], table[port_lookup][1],
                table[port_lookup][2], table[port_lookup][3], table[port_lookup][4] ));

    } else if (chip_port >= 24 && chip_port <= 31) { // X2 plug-in module - port 24-31

        // The sfp select - See pca9545a data-sheet table 4(+ schematic for i2c address)
        switch (chip_port) {
        case 24:
            pca_9545a_i2c_addr = 0x70;
            pca_9545a_data     = 0x01;
            break;

        case 25:
            pca_9545a_i2c_addr = 0x70;
            pca_9545a_data     = 0x02;
            break;

        case 26:
            pca_9545a_i2c_addr = 0x70;
            pca_9545a_data     = 0x04;
            break;

        case 27:
            pca_9545a_i2c_addr = 0x70;
            pca_9545a_data     = 0x08;
            break;

        case 28:
            pca_9545a_i2c_addr = 0x71;
            pca_9545a_data     = 0x01;
            break;

        case 29:
            pca_9545a_i2c_addr = 0x71;
            pca_9545a_data     = 0x02;
            break;

        case 30:
            pca_9545a_i2c_addr = 0x71;
            pca_9545a_data     = 0x04;
            break;

        case 31:
            pca_9545a_i2c_addr = 0x71;
            pca_9545a_data     = 0x08;
            break;

        default: PRINTD(("Unexpected X2 module port number :%d", port_no));
            return VTSS_RC_ERROR;
        }

        // Setup which SFP to access
        VTSS_RC(jr2_i2c_write(port_no, pca_9545a_i2c_addr, &pca_9545a_data, 1, VTSS_I2C_NO_MULTIPLEXER));

    } else if (chip_port >= 49 &&  port_no <= chip_port) { // SFP+
         // No, enable. They are using the internal mux.
    } else {
        PRINTD(("Unexpected port number :%d (%d)", port_no, chip_port));
        return VTSS_RC_ERROR;
    }
    return rc;
}


static vtss_rc jr2_cu48_sfp_i2c_enable(vtss_port_no_t port_no)
{
    PRINTD(("Not Implemented"));
    return VTSS_RC_OK;
}

static vtss_rc jr2_aqr_sfp_i2c_enable(vtss_port_no_t port_no)
{
    // No, enable. They are using the internal mux.
    PRINTD(("Not Implemented"));
    return VTSS_RC_OK;
}


//**************************************************************************************
//* Add-on modules
//**************************************************************************************

//
// Malibu
//
// Function for detecting and configuring Malibu board
// Return VTSS_RC_OK if Malibu board was detected and configured correct, else error code
vtss_rc malibu_detect_and_conf(void) {
    u8 data;
    (void)board_sfp_i2c_lock(TRUE);
    if (jr2_sfp_i2c_read(VTSS_PORTS-2, 0x40, 0, &data, 1, FALSE) == VTSS_RC_OK) { // Try to read the i2c EEPROM (address 0x40 )at the Malibu board, SFP+ port 1 (port = 27)
        (void)board_sfp_i2c_lock(FALSE);

        PRINTD(("Malibu detected"));

        // Setup LREF to 156.25MHz
        // Setting LREF_SELx pins to 0x00 for 156.25 MHz, See U1B at PCB VSC8258-SFP-04 schematic
        // LREF_SEL0, and LREF_SEL1 goes to SFPplusD_RATESEL0 and SFPplusD_RATESEL1, See the two schematics.
        // SFPplusD_RATESEL0 goes to Serial GPIO p15b1,(group 2)
        // SFPplusD_RATESEL1 goes to Serial GPIO p15b2 (group 2)
        vtss_sgpio_conf_t conf;
        VTSS_RC(vtss_sgpio_conf_get(NULL, 0, 2, &conf));
        conf.port_conf[15].enabled = 1;
        conf.port_conf[15].mode[1] = VTSS_SGPIO_MODE_OFF;
        conf.port_conf[15].mode[2] = VTSS_SGPIO_MODE_OFF;
        VTSS_RC(vtss_sgpio_conf_set(NULL, 0, 2, &conf));

        // Setup "CML out -> DC coupling -> CML in" to channel o
        // Setting SELx pins to 0x00 for channel 0, See U4 at PCB VSC8258-SFP-04 schematic
        // SEL0, and SEL1 goes to SFPplusB_RATESEL0 and SFPplusB_RATESEL1, See the two schematics.
        // SFPplusB_RATESEL0 goes to Serial GPIO p14b0,(group 2)
        // SFPplusB_RATESEL1 goes to Serial GPIO p14b1 (group 2)
        VTSS_RC(vtss_sgpio_conf_get(NULL, 0, 2, &conf));
        conf.port_conf[14].enabled = 1;
        conf.port_conf[14].mode[0] = VTSS_SGPIO_MODE_OFF;
        conf.port_conf[14].mode[1] = VTSS_SGPIO_MODE_OFF;
        VTSS_RC(vtss_sgpio_conf_set(NULL, 0, 2, &conf));
        return VTSS_RC_OK;
    }
    (void)board_sfp_i2c_lock(FALSE);
    PRINTD(("Malibu NOT detected"));
    return VTSS_RC_ERROR;
}

static vtss_rc jr2_reset(void)
{
#if defined(VTSS_CHIP_10G_PHY) && (defined(VTSS_CHIP_JAGUAR_2) || defined(VTSS_CHIP_SPARX_IV_80) || defined(VTSS_CHIP_SPARX_IV_90))
    vtss_port_map_t    port_map[VTSS_PORT_ARRAY_SIZE];
    vtss_sgpio_conf_t conf;

    if (jr2_board_type != VTSS_BOARD_JAGUAR2_CU48_REF && jr2_board_type != VTSS_BOARD_JAGUAR2_AQR_REF && malibu_detect_and_conf() == VTSS_RC_OK) {

        if (VTSS_PORT_ARRAY_SIZE < 24) {
            return VTSS_RC_ERROR; // Shall never happen if Malibu is detected.
        }

        (void)vtss_port_map_get(NULL, port_map);
        for (vtss_port_no_t iport = 24; iport < VTSS_PORTS-1 && iport < VTSS_PORT_ARRAY_SIZE; iport++) {
            port_map[iport].miim_controller = VTSS_MIIM_CONTROLLER_0;
            jr2_port_table[iport].cap = (PORT_CAP_VTSS_10G_PHY | PORT_CAP_10G_FDX | PORT_CAP_FLOW_CTRL | PORT_CAP_XAUI_LANE_FLIP |PORT_CAP_1G_FDX);
#ifdef MALIBU_2_CHANNEL
            switch(iport) {
                case 24:
                port_map[24].miim_addr = 1;
                    break;
                case 25:
                port_map[25].miim_addr = 0;
                    break;
            }
#else
            switch(iport) {
                case 24:
                port_map[24].miim_addr = 3;
                    break;
                case 25:
                port_map[25].miim_addr = 2;
                    break;
                case 26:
                port_map[26].miim_addr = 1;
                    break;
                case 27:
                port_map[27].miim_addr = 0;
                    break;
            }
#endif
        }
        (void)vtss_port_map_set(NULL, port_map);
        // The PHY_RST goes to  SFPplusC_RATESEL0 (P14b2), See schematics
        VTSS_RC(vtss_sgpio_conf_get(NULL, 0, 2, &conf));
        conf.port_conf[14].enabled = 1;
        conf.port_conf[14].mode[2] = VTSS_SGPIO_MODE_ON;
        (void) (vtss_sgpio_conf_set(NULL, 0, 2, &conf));
    }
#endif
    if (jr2_port_table[0].map.miim_controller != VTSS_MIIM_CONTROLLER_NONE) {
        VTSS_RC(vtss_phy_pre_reset(PHY_INST, 0));
        if (jr2_board_type == VTSS_BOARD_JAGUAR2_REF) {
            VTSS_RC(vtss_phy_pre_reset(PHY_INST, 4));

        } else if (jr2_board_type == VTSS_BOARD_JAGUAR2_CU48_REF) { // This board has 4 Atom12s - See schematics
            VTSS_RC(vtss_phy_pre_reset(PHY_INST, 12));
            VTSS_RC(vtss_phy_pre_reset(PHY_INST, 24));
            VTSS_RC(vtss_phy_pre_reset(PHY_INST, 36));

        } else if( jr2_board_type == VTSS_BOARD_JAGUAR2_AQR_REF) {
            for (vtss_port_no_t iport = 0; iport < (VTSS_PORTS-2); iport++) { // reset only for 2.5G ports, 2 ports are 10G
                VTSS_RC(vtss_phy_pre_reset(PHY_INST, iport));
            }
        }
    }
    return VTSS_RC_OK;
}


//**************************************************************************************
//* LEDs
//**************************************************************************************

static vtss_rc jr2_led_init(void)
{
    // Enable enahanced LED control to disable the Phy PWM (LED intensity) signal (otherwise the LEDs are off).
    if (jr2_port_table[0].map.miim_controller == VTSS_MIIM_CONTROLLER_NONE) {
        return VTSS_RC_OK;
    }
    if (jr2_board_type != VTSS_BOARD_JAGUAR2_AQR_REF) {
        (void)vtss_phy_write(NULL, 0, 31, 0x0010);
        (void)vtss_phy_write(NULL, 0, 25, 0xc6b5);
        (void)vtss_phy_write(NULL, 0, 31, 0);
    }
    /* Update status LED using GPIO_12 and GPIO_13 */
    (void)vtss_gpio_mode_set(NULL, 0, 12, VTSS_GPIO_OUT);
    (void)vtss_gpio_mode_set(NULL, 0, 13, VTSS_GPIO_OUT);
    (void)vtss_gpio_write(NULL, 0, 12, 1); // Green LED ON
    (void)vtss_gpio_write(NULL, 0, 13, 0); // Red LED OFF
    return VTSS_RC_OK;
}

static BOOL port_activity(vtss_port_no_t port_no,
                          vtss_port_status_t *status,
                          vtss_port_counters_t *counters,
                          BOOL *cnt_activity)
{

    vtss_port_status_t *old_status = &jr2_old_port_info[port_no].status;
    vtss_port_counters_t *old_cnt = &jr2_old_port_info[port_no].cnt;
    BOOL link_activity = 0;

    if ((counters->rmon.rx_etherStatsPkts > (old_cnt->rmon.rx_etherStatsPkts + 2)) ||
        (counters->rmon.tx_etherStatsPkts > (old_cnt->rmon.tx_etherStatsPkts + 2))) {
        *cnt_activity = 1;
        *old_cnt = *counters;
    }
    if (old_status->link  != status->link  ||
        old_status->speed != status->speed ||
        old_status->fdx   != status->fdx) {
        link_activity = 1;
        *old_status = *status;
    }
    if (!*cnt_activity && !link_activity && !jr2_old_port_info[port_no].activity) {
        return 0; // No Activity
    }
    jr2_old_port_info[port_no].activity = link_activity || *cnt_activity;
    return 1; // Activity
}

/* Port traffic activity is auto updated */
static BOOL port_activity_auto(vtss_port_no_t port_no, vtss_port_status_t *status)
{

    vtss_port_status_t *old_status = &jr2_old_port_info[port_no].status;
    BOOL link_activity = 0;

    if (old_status->link  != status->link  ||
        old_status->speed != status->speed ||
        old_status->fdx   != status->fdx   ||
        old_status->fiber != status->fiber) {
        link_activity = 1;
        *old_status = *status;
    }
    if (!link_activity && !jr2_old_port_info[port_no].activity) {
        return 0; // No Activity
    }
    jr2_old_port_info[port_no].activity = link_activity;
    return 1; // Activity
}

static vtss_rc jr2_led_update_jr2(vtss_port_no_t port_no,
                                  vtss_port_status_t *status,
                                  vtss_port_counters_t *counters,
                                  port_custom_conf_t *port_conf)

{
    u32                 sgpio_group = 0, sgpio_port=0, chip_port = jr2_port_table[port_no].map.chip_port;
    vtss_sgpio_conf_t   conf;
    vtss_sgpio_mode_t   mode_green = VTSS_SGPIO_MODE_ON, mode_yellow = VTSS_SGPIO_MODE_ON;
    BOOL                update_led = port_activity_auto(port_no, status);

    if (!update_led) {
        return VTSS_RC_OK;
    }

    if (chip_port < 32) {
        sgpio_group = 0;
        sgpio_port = chip_port;
    } else if (chip_port == 48) {
        sgpio_group = 2;
        sgpio_port = 24; // NPI
        mode_green =  VTSS_SGPIO_MODE_OFF;
        mode_yellow = VTSS_SGPIO_MODE_OFF;
    } else if (chip_port > 48) {
        mode_green =  VTSS_SGPIO_MODE_OFF;
        mode_yellow = VTSS_SGPIO_MODE_OFF;
        if (jr2_port_table[port_no].mac_if == VTSS_PORT_INTERFACE_SFI) {
            sgpio_group = 1;
            sgpio_port = 16 + (chip_port - 49);  // SFP+
        } else { // XAUI port
            sgpio_group = 2;
            sgpio_port = chip_port == 49 ? 25 : 26;
        }
    }
    /* If link then auto update LED */
    if (status->link) {
        if (status->speed >= VTSS_SPEED_1G) {
            mode_green = (chip_port == JR2_NPI_CHIP_PORT || (chip_port > 48)) ? VTSS_SGPIO_MODE_0_ACTIVITY_INV : VTSS_SGPIO_MODE_0_ACTIVITY;
        } else {
            mode_yellow = (chip_port == JR2_NPI_CHIP_PORT || (chip_port > 48)) ? VTSS_SGPIO_MODE_0_ACTIVITY_INV : VTSS_SGPIO_MODE_0_ACTIVITY;
        }
    }

    VTSS_RC(vtss_sgpio_conf_get(NULL, 0, sgpio_group, &conf));

    // The Green/yellow LEDs are inverted for 1G ports
    conf.port_conf[sgpio_port].mode[LED_GREEN] = (chip_port > 48) ? mode_green : mode_yellow;
    conf.port_conf[sgpio_port].mode[LED_YELLOW] = (chip_port > 48) ? mode_yellow : mode_green;
    return vtss_sgpio_conf_set(NULL, 0, sgpio_group, &conf);
}


// TBD: enable LED update via the Phy
static vtss_rc jr2_led_update_cu48(vtss_port_no_t port_no,
                                   vtss_port_status_t *status,
                                   vtss_port_counters_t *counters,
                                   port_custom_conf_t *port_conf)
{
    u32                 sgpio_group=0, sgpio_port=0, chip_port = jr2_port_table[port_no].map.chip_port;
    vtss_sgpio_conf_t   conf;
    vtss_sgpio_mode_t   mode_green, mode_yellow;
    vtss_rc             rc;
    BOOL                update_led = port_activity_auto(port_no, status);

    if (!update_led) {
        return VTSS_RC_OK;
    }

    mode_green = chip_port == JR2_NPI_CHIP_PORT ? VTSS_SGPIO_MODE_OFF : VTSS_SGPIO_MODE_ON;
    mode_yellow = chip_port == JR2_NPI_CHIP_PORT ? VTSS_SGPIO_MODE_OFF : VTSS_SGPIO_MODE_ON;

    /* If link then auto update LED */
    if (status->link) {
        if (status->speed >= VTSS_SPEED_1G) {
            mode_green = (chip_port == JR2_NPI_CHIP_PORT) ? VTSS_SGPIO_MODE_0_ACTIVITY_INV : VTSS_SGPIO_MODE_0_ACTIVITY;
        } else {
            mode_yellow = (chip_port == JR2_NPI_CHIP_PORT) ? VTSS_SGPIO_MODE_0_ACTIVITY_INV : VTSS_SGPIO_MODE_0_ACTIVITY;
        }
    }

    if (chip_port < 32) {
        sgpio_port = chip_port;
        sgpio_group = 0;
    } else if (chip_port < 48) {
        sgpio_port = chip_port - 32;
        sgpio_group = 1;
    } else if (chip_port == 48) {
        sgpio_port = 24; //NPI
        sgpio_group = 2;
    } else {
        if ((jr2_port_table[port_no].mac_if == VTSS_PORT_INTERFACE_XAUI) && (chip_port == 49 || chip_port == 50)) {
            sgpio_group = 2;
            sgpio_port = (chip_port == 49) ? 25 : 26;
            mode_green = status->link ? VTSS_SGPIO_MODE_0_ACTIVITY_INV : VTSS_SGPIO_MODE_0_ACTIVITY;
            mode_yellow = VTSS_SGPIO_MODE_OFF;
        } else {
            // SFP+ ports
            switch (chip_port) {
            case 49:  sgpio_port = 17; break;
            case 50:  sgpio_port = 18; break;
            case 51:  sgpio_port = 19; break;
            case 52:  sgpio_port = 20; break;
            }
            sgpio_group = 1;
        }
    }
    if ((rc = vtss_sgpio_conf_get(NULL, 0, sgpio_group, &conf)) != VTSS_RC_OK) {
        return rc;
    }
    conf.port_conf[sgpio_port].mode[LED_GREEN] = mode_green;
    conf.port_conf[sgpio_port].mode[LED_YELLOW] = mode_yellow;
    return vtss_sgpio_conf_set(NULL, 0, sgpio_group, &conf);
}

// TBD: enable auto update
static vtss_rc srv2_led_update_nid(vtss_port_no_t port_no,
                                   vtss_port_status_t *status,
                                   vtss_port_counters_t *counters,
                                   port_custom_conf_t *port_conf)
{
    u8                   port_to_led_map[VTSS_PORTS] = {0,1,2,3,5,4,7,6,9,8,11,10,26,25}; // See UG1057, table 16 (The table doesn't say that bit 25 and 26 shall be swapped, but that has turned out to be the case)
    u32                  sgpio_port;
    vtss_sgpio_conf_t    conf;
    vtss_sgpio_mode_t    mode_green, mode_yellow;
    BOOL                 cnt_activity = 0, update_led = port_activity(port_no, status, counters, &cnt_activity);

    if (!update_led) {
        return VTSS_RC_OK;
    }

    VTSS_RC(vtss_sgpio_conf_get(NULL, 0, 2, &conf));

    mode_green = VTSS_SGPIO_MODE_OFF;
    mode_yellow = VTSS_SGPIO_MODE_OFF;

    if (status->link) {
        if (status->speed >= VTSS_SPEED_1G) {
            if (cnt_activity) {
                mode_green = VTSS_SGPIO_MODE_0;
            } else {
                mode_green = VTSS_SGPIO_MODE_ON;
            }
        } else {
            if (cnt_activity) {
                mode_yellow = VTSS_SGPIO_MODE_0;
            } else {
                mode_yellow = VTSS_SGPIO_MODE_ON;
            }
        }
    }
    sgpio_port = port_to_led_map[port_no];
    conf.port_conf[sgpio_port].mode[LED_GREEN] = mode_green;
    conf.port_conf[sgpio_port].mode[LED_YELLOW] = mode_yellow;
    return vtss_sgpio_conf_set(NULL, 0, 2, &conf);
}

static vtss_rc jr2_led_update_aqr(vtss_port_no_t port_no,
                                  vtss_port_status_t *status,
                                  vtss_port_counters_t *counters,
                                  port_custom_conf_t *port_conf)

{
    u32                 sgpio_port = 0, chip_port = jr2_port_table[port_no].map.chip_port;
    vtss_sgpio_conf_t   conf;
    vtss_sgpio_mode_t   mode_green = VTSS_SGPIO_MODE_OFF, mode_yellow = VTSS_SGPIO_MODE_OFF;
    vtss_rc             rc;
    BOOL                cnt_activity = 0, update_led = port_activity(port_no, status, counters, &cnt_activity);

    if (!update_led) {
        return VTSS_RC_OK;
    }

    if ((rc = vtss_sgpio_conf_get(NULL, 0, 0, &conf)) != VTSS_RC_OK) {
        return rc;
    }

    if (chip_port == 51) {
        sgpio_port = 24;
    } else if (chip_port == 52) {
        sgpio_port = 25;
    } else {
        sgpio_port = chip_port - 8;
        mode_green = VTSS_SGPIO_MODE_ON;
        mode_yellow = VTSS_SGPIO_MODE_ON;
    }

    if (status->link) {
        if (status->speed >= VTSS_SPEED_1G) {
            if (cnt_activity) {
                mode_green = VTSS_SGPIO_MODE_0;
            } else {
                mode_green = (chip_port > 48) ? VTSS_SGPIO_MODE_ON : VTSS_SGPIO_MODE_OFF;
            }
        } else {
            if (cnt_activity) {
                mode_yellow = VTSS_SGPIO_MODE_0;
            } else {
                mode_yellow = VTSS_SGPIO_MODE_OFF;
            }
        }
    }
    /* Note: For AQ PHY, colors green & yellow are swapped */
    conf.port_conf[sgpio_port].mode[LED_GREEN] = (chip_port > 48) ? mode_green : mode_yellow;
    conf.port_conf[sgpio_port].mode[LED_YELLOW] = (chip_port > 48) ? mode_yellow : mode_green;

    return vtss_sgpio_conf_set(NULL, 0, 0, &conf);
}

static void jr2_port_conf(vtss_port_no_t port_no,
                          port_custom_conf_t *port_conf,
                          vtss_port_status_t *port_status)
{
    u8 port_to_tx_disable_map_nid[VTSS_PORTS] = {0,0,0,0,8,9,10,11,4,5,6,7,12,13}; // TxDisable maps to SGPIO port bit 0
    u32 chip_port = jr2_port_table[port_no].map.chip_port;
    vtss_sgpio_conf_t  conf;
    vtss_sgpio_mode_t  sgpio_mode = (port_conf->enable ? VTSS_SGPIO_MODE_ON : VTSS_SGPIO_MODE_OFF);
#if defined(VTSS_CHIP_10G_PHY) && (defined(VTSS_CHIP_JAGUAR_2) || defined(VTSS_CHIP_SPARX_IV_80) || defined(VTSS_CHIP_SPARX_IV_90))
    vtss_gpio_10g_gpio_mode_t gpio_conf;
    vtss_gpio_10g_no_t gpio_no;
    u32 global_dev = 0x1e,lopc_reg = 0xf234,value = 0x13f;
#endif

    if (jr2_board_type == VTSS_BOARD_SERVAL2_NID_REF && jr2_port_table[port_no].map.miim_controller == -1) {
        (void)vtss_sgpio_conf_get(NULL, 0, 0, &conf);
        conf.port_conf[port_to_tx_disable_map_nid[port_no]].mode[0] = sgpio_mode;
        (void)vtss_sgpio_conf_set(NULL, 0, 0, &conf);
    } else if (jr2_board_type == VTSS_BOARD_JAGUAR2_REF && (jr2_port_table[port_no].map.miim_controller == -1) )  {
        (void)vtss_sgpio_conf_get(NULL, 0, 2, &conf);
        switch (chip_port) {
        case 8:  conf.port_conf[0].mode[0] = sgpio_mode; break;
        case 9:  conf.port_conf[0].mode[2] = sgpio_mode; break;
        case 10: conf.port_conf[1].mode[1] = sgpio_mode; break;
        case 11: conf.port_conf[2].mode[0] = sgpio_mode; break;
        case 12: conf.port_conf[2].mode[2] = sgpio_mode; break;
        case 13: conf.port_conf[3].mode[1] = sgpio_mode; break;
        case 14: conf.port_conf[4].mode[0] = sgpio_mode; break;
        case 15: conf.port_conf[4].mode[2] = sgpio_mode; break;
        case 16: conf.port_conf[5].mode[1] = sgpio_mode; break;
        case 17: conf.port_conf[6].mode[0] = sgpio_mode; break;
        case 18: conf.port_conf[6].mode[2] = sgpio_mode; break;
        case 19: conf.port_conf[7].mode[1] = sgpio_mode; break;
        case 20: conf.port_conf[8].mode[0] = sgpio_mode; break;
        case 21: conf.port_conf[8].mode[2] = sgpio_mode; break;
        case 22: conf.port_conf[9].mode[1] = sgpio_mode; break;
        case 23: conf.port_conf[10].mode[0] = sgpio_mode; break;
#if defined(VTSS_CHIP_10G_PHY) && (defined(VTSS_CHIP_JAGUAR_2) || defined(VTSS_CHIP_SPARX_IV_80) || defined(VTSS_CHIP_SPARX_IV_90))
        case 49: gpio_conf.mode = VTSS_10G_PHY_GPIO_DRIVE_LOW;
                 gpio_no = 28;
                 (void)vtss_phy_10g_gpio_mode_set(PHY_INST,port_no,gpio_no,&gpio_conf);
                 gpio_conf.mode = VTSS_10G_PHY_GPIO_AGG_INT_0;
                 gpio_conf.aggr_intrpt = (1<<VTSS_10G_GPIO_AGGR_INTRPT_CH3_INTR0_EN) | (1<<VTSS_10G_GPIO_AGGR_INTRPT_IP1588_0_INTR3_EN) | (1<<VTSS_10G_GPIO_AGGR_INTRPT_IP1588_1_INTR3_EN);
                 gpio_conf.c_intrpt = VTSS_10G_GPIO_INTRPT_HPMA;
                 gpio_no = 34;
                 (void)vtss_phy_10g_gpio_mode_set(PHY_INST,port_no,gpio_no,&gpio_conf);
                 break;
        case 50: gpio_conf.mode = VTSS_10G_PHY_GPIO_DRIVE_LOW;
                 gpio_no = 20;
                 (void)vtss_phy_10g_gpio_mode_set(PHY_INST,port_no,gpio_no,&gpio_conf);
                 gpio_conf.mode = VTSS_10G_PHY_GPIO_AGG_INT_0;
                 gpio_conf.aggr_intrpt = (1<<VTSS_10G_GPIO_AGGR_INTRPT_CH2_INTR0_EN) | (1<<VTSS_10G_GPIO_AGGR_INTRPT_IP1588_0_INTR2_EN) | (1<<VTSS_10G_GPIO_AGGR_INTRPT_IP1588_1_INTR2_EN);
                 gpio_conf.c_intrpt = VTSS_10G_GPIO_INTRPT_HPMA;
                 gpio_no = 34;
                 (void)vtss_phy_10g_gpio_mode_set(PHY_INST,port_no,gpio_no,&gpio_conf);
                 break;
        case 51: gpio_conf.mode = VTSS_10G_PHY_GPIO_DRIVE_LOW;
                 gpio_no = 12;
                 (void)vtss_phy_10g_gpio_mode_set(PHY_INST,port_no,gpio_no,&gpio_conf);
                 gpio_conf.mode = VTSS_10G_PHY_GPIO_AGG_INT_0;
                 gpio_conf.aggr_intrpt = (1<<VTSS_10G_GPIO_AGGR_INTRPT_CH1_INTR0_EN) | (1<<VTSS_10G_GPIO_AGGR_INTRPT_IP1588_0_INTR1_EN) | (1<<VTSS_10G_GPIO_AGGR_INTRPT_IP1588_1_INTR1_EN);
                 gpio_conf.c_intrpt = VTSS_10G_GPIO_INTRPT_HPMA;
                 gpio_no = 34;
                 (void)vtss_phy_10g_gpio_mode_set(PHY_INST,port_no,gpio_no,&gpio_conf);
                 break;
        case 52: gpio_conf.mode = VTSS_10G_PHY_GPIO_DRIVE_LOW;
                 gpio_no = 4;
                 (void)vtss_phy_10g_gpio_mode_set(PHY_INST,port_no,gpio_no,&gpio_conf);
                 gpio_conf.mode = VTSS_10G_PHY_GPIO_AGG_INT_0;
                 gpio_conf.aggr_intrpt = (1<<VTSS_10G_GPIO_AGGR_INTRPT_CH0_INTR0_EN) | (1<<VTSS_10G_GPIO_AGGR_INTRPT_IP1588_0_INTR0_EN) | (1<<VTSS_10G_GPIO_AGGR_INTRPT_IP1588_1_INTR0_EN);
                 gpio_conf.c_intrpt = VTSS_10G_GPIO_INTRPT_HPMA;
                 gpio_no = 34;
                 (void)vtss_phy_10g_gpio_mode_set(PHY_INST,port_no,gpio_no,&gpio_conf);
                 /* By default GPIO 34 is used as INPUT for LOPC on channel 0,Now as we are changing its default behaviour
                    Configuring registers accordingly */
                 vtss_phy_10g_csr_write(PHY_INST,port_no,global_dev,lopc_reg,value);
                 break;
#endif
        default:
            return;
        }
        (void)vtss_sgpio_conf_set(NULL, 0, 2, &conf);
    }
}

BOOL vtss_board_probe_jr2_cu48(vtss_board_t *board, vtss_board_info_t *board_info)
{
    vtss_port_no_t      port_no;
    port_custom_entry_t *entry;
    jr2_10g_detect_t detect;
    u32 gpio_no, alt_mask;
    jr2_i2c_read   = board_info->i2c_read;
    jr2_i2c_write  = board_info->i2c_write;

    // Configure GPIOs for MIIM/MDIO
    (void) jr2_rd(JR2_TGT_GCB, JR2_ADDR_GPIO_ALT1_0, &alt_mask, board_info);
    for (gpio_no = 56; gpio_no < 60; gpio_no++) {
        alt_mask = (1 << (gpio_no - 32)) | alt_mask;
    }
    (void) jr2_wr(JR2_TGT_GCB, JR2_ADDR_GPIO_ALT1_0, alt_mask, board_info);

    board->type = VTSS_BOARD_JAGUAR2_CU48_REF,
    jr2_board_type = board_info->board_type = board->type;
    board->name = "Jaguar-2 CU48 Reference";
    board->features = (VTSS_BOARD_FEATURE_LOS | VTSS_BOARD_FEATURE_1588_REF_CLK_SEL | VTSS_BOARD_FEATURE_1588_CLK_ADJ_DAC);
    board->custom_port_table = jr2_port_table;
    board->init = jr2_init_cu48;
    board->reset = jr2_reset;
    board->pre_reset = jr2_pre_reset;
    board->post_reset = jr2_post_reset;
    board->led_init = jr2_led_init;
    board->led_update = jr2_led_update_cu48;
    board->port_conf = jr2_port_conf;
    board->sfp_i2c_read = jr2_sfp_i2c_read;
    board->sfp_i2c_write = jr2_sfp_i2c_write;
    board->sfp_update_if = jr2_sfp_update_if;
    board->sfp_mod_detect = jr2_sfp_mod_detect;
    board->sfp_status = jr2_sfp_status;
    board->sfp_i2c_enable = jr2_cu48_sfp_i2c_enable;
    board->sfp_i2c_lock = 0;
    board->sfp_accept = jr2_sfp_accept;

#if defined(VTSS_CHIP_SPARX_IV_90)
    board->mux_mode = VTSS_PORTS > 51 ? VTSS_PORT_MUX_MODE_2 : VTSS_PORT_MUX_MODE_AUTO;
#else
    board->mux_mode = VTSS_PORT_MUX_MODE_AUTO;
#endif

    detect.miim_found[0] = 0;
    detect.miim_found[1] = 0;

    for (port_no = 0; port_no < VTSS_PORTS; port_no++) {
        entry = &jr2_port_table[port_no];
        if (port_no < 24) {
            /* Port 0-23: Copper ports */
            entry->map.chip_port = (port_no);
            entry->map.miim_controller = VTSS_MIIM_CONTROLLER_1;
            entry->map.miim_addr = (port_no);
            entry->mac_if = VTSS_PORT_INTERFACE_QSGMII;
            entry->cap = PORT_CAP_TRI_SPEED_COPPER;
        } else if (port_no < 48) {
            /* Port 24-47: Copper ports */
            entry->map.chip_port = (port_no);
            entry->map.miim_controller = VTSS_MIIM_CONTROLLER_2;
            entry->map.miim_addr = (port_no - 24);
            entry->mac_if = VTSS_PORT_INTERFACE_QSGMII;
            entry->cap = PORT_CAP_TRI_SPEED_COPPER;
#if defined(VTSS_CHIP_SPARX_IV_90)
        } else if (port_no < 50) {
            /* Port 48-49: 10G ports SFI or XAUI */
            /* Detect 10G PHY, can be a VTSS PHY or X2/Xenpak module */
            /* If a device is found then this XAUI port is activated and a  SFP+ port disabled */
            detect.port_no = port_no;
            detect.miim_addr[0] = (port_no - 24);
            detect.cap = 0;
            if (jr2_10g_detect(&detect, board_info)) {
                /* API Port 48-49: XAUI chip ports:49,50 possibly VTSS Phys */
                entry->map.chip_port = (port_no + 1);
                entry->map.miim_controller = VTSS_MIIM_CONTROLLER_0;
                entry->map.miim_addr = detect.miim_addr[0];
                entry->mac_if = VTSS_PORT_INTERFACE_XAUI;
                entry->cap = detect.cap;
            } else {
                /* API Port 48-49: SFP+ chip ports:51,52 */
                entry->map.chip_port = (port_no == 48 ? 50 : 49);
                entry->map.miim_controller = VTSS_MIIM_CONTROLLER_NONE;
                entry->mac_if = VTSS_PORT_INTERFACE_SFI;
                entry->cap = (PORT_CAP_10G_FDX | PORT_CAP_SFP_2_5G | PORT_CAP_FLOW_CTRL | PORT_CAP_SFP_SD_HIGH);
            }
        } else if (port_no < 52 && port_no != (VTSS_PORTS-1)) {
            /* API Port 50-51: 10G SFI ports */
            entry->map.chip_port = (port_no == 50 ? 52 : 51);
            entry->map.miim_controller = VTSS_MIIM_CONTROLLER_NONE;
            entry->mac_if = VTSS_PORT_INTERFACE_SFI;
            entry->cap = (PORT_CAP_10G_FDX | PORT_CAP_SFP_2_5G | PORT_CAP_FLOW_CTRL | PORT_CAP_SFP_SD_HIGH);
#else /* VTSS_CHIP_SPARX_IV_80 */
        } else if (port_no < 50) {
            /* Port 48-49: 10G ports SFI or XAUI */
            /* Detect 10G PHY, can be a VTSS PHY or X2/Xenpak module */
            /* If a device is found then this XAUI port is activated and a  SFP+ port disabled */
            detect.port_no = port_no;
            detect.miim_addr[0] = (port_no - 24);
            detect.cap = 0;
            if (jr2_10g_detect(&detect, board_info)) {
                /* API Port 48-49: XAUI chip ports:49,50 possibly VTSS Phys */
                entry->map.chip_port = (port_no + 1);
                entry->map.miim_controller = VTSS_MIIM_CONTROLLER_0;
                entry->map.miim_addr = detect.miim_addr[0];
                entry->mac_if = VTSS_PORT_INTERFACE_XAUI;
                entry->cap = detect.cap;
            } else {
                /* API Port 48-49: SFP+ chip ports:51,52 */
                entry->map.chip_port = (port_no == 48 ? 52 : 51);
                entry->map.miim_controller = VTSS_MIIM_CONTROLLER_NONE;
                entry->mac_if = VTSS_PORT_INTERFACE_SFI;
                entry->cap = (PORT_CAP_10G_FDX | PORT_CAP_SFP_2_5G | PORT_CAP_FLOW_CTRL | PORT_CAP_SFP_SD_HIGH);
            }
#endif /* VTSS_CHIP_SPARX_IV_90 */
        } else {
            /* Last port */
            entry->map.chip_port = 48;
            entry->map.miim_controller = jr2_phy_detect(28, board_info); // Autodetect controller 0 or 1
            entry->map.miim_addr = 28;
            entry->mac_if = VTSS_PORT_INTERFACE_SGMII;
            entry->cap = PORT_CAP_TRI_SPEED_COPPER;
        }
    }

    return TRUE;
}


BOOL vtss_board_probe_srv2_nid(vtss_board_t *board, vtss_board_info_t *board_info)
{
    vtss_port_no_t      port_no;
    port_custom_entry_t *entry;
    jr2_i2c_read   = board_info->i2c_read;
    jr2_i2c_write  = board_info->i2c_write;

    board->type = VTSS_BOARD_SERVAL2_NID_REF;
    jr2_board_type = board_info->board_type = board->type;
    board->name = "Serval-2";
    board->features = (VTSS_BOARD_FEATURE_LOS);
    board->custom_port_table = jr2_port_table;
    board->init = jr2_init_nid;
    board->reset = jr2_reset;
    board->pre_reset = jr2_pre_reset;
    board->post_reset = jr2_post_reset;
    board->led_init = jr2_led_init;
    board->led_update = srv2_led_update_nid;
    board->port_conf = jr2_port_conf;
    board->sfp_i2c_read = srv2_nid_i2c_read;
    board->sfp_i2c_write = srv2_nid_i2c_write;
    board->sfp_update_if =  jr2_sfp_update_if;
    board->sfp_mod_detect = srv2_nid_sfp_mod_detect;
    board->sfp_status = jr2_sfp_status;
    board->sfp_i2c_enable = srv2_nid_i2c_enable;
    board->sfp_i2c_lock = 0;
    board->sfp_accept = jr2_sfp_accept;
    board->mux_mode = VTSS_PORT_MUX_MODE_AUTO;

    /* Fill out port mapping table */
    for (port_no = 0; port_no < VTSS_PORTS; port_no++) {
        entry = &jr2_port_table[port_no];
        if (port_no < 4) {
            /* Port 0-3: Copper ports */
            entry->map.chip_port = (port_no + 8);
            entry->map.miim_controller = VTSS_MIIM_CONTROLLER_0;
            entry->map.miim_addr = (port_no + 16);
            entry->mac_if = VTSS_PORT_INTERFACE_SGMII;
            entry->cap = PORT_CAP_TRI_SPEED_COPPER;
#if (VTSS_PORTS > 12)
        } else if (port_no < 12) {
            /* Port 4-11: 1G SFP */
            entry->map.chip_port = (port_no + ((port_no & 1) ? 7 : 9));
            entry->map.miim_controller = VTSS_MIIM_CONTROLLER_NONE;
            entry->mac_if = VTSS_PORT_INTERFACE_SERDES;
            entry->cap = PORT_CAP_SFP_2_5G | PORT_CAP_SFP_SD_HIGH;
#endif /* VTSS_PORTS > 12 */
#if (VTSS_PORTS > 14)
        } else if (port_no < 14) {
            /* Port 12-13: SFP+ */
            entry->map.chip_port = (port_no == 13 ? 49 : 50);
            entry->map.miim_controller = VTSS_MIIM_CONTROLLER_NONE;
            /* TBD */
            entry->mac_if = VTSS_PORT_INTERFACE_SFI;
            entry->cap = (PORT_CAP_10G_FDX | PORT_CAP_SFP_2_5G | PORT_CAP_FLOW_CTRL | PORT_CAP_SFP_SD_HIGH);
#endif /* VTSS_PORTS > 14 */
        } else {
            /* Port 14: NPI port */
            entry->map.chip_port = 48;
            entry->map.miim_controller = VTSS_MIIM_CONTROLLER_0;
            entry->map.miim_addr = 28;
            entry->mac_if = VTSS_PORT_INTERFACE_SGMII;
            entry->cap = PORT_CAP_TRI_SPEED_COPPER;
        }
    }

    return TRUE;
}

BOOL vtss_board_probe_jr2_sfp24(vtss_board_t *board, vtss_board_info_t *board_info)
{
    vtss_port_no_t      port_no;
    port_custom_entry_t *entry;
    jr2_i2c_read   = board_info->i2c_read;
    jr2_i2c_write  = board_info->i2c_write;
    jr2_10g_detect_t detect;

    board->type = VTSS_BOARD_JAGUAR2_REF;
    jr2_board_type = board_info->board_type = board->type;
    board->name = "Jaguar-2";
    board->features = (VTSS_BOARD_FEATURE_LOS | VTSS_BOARD_FEATURE_1588_REF_CLK_SEL | VTSS_BOARD_FEATURE_1588_CLK_ADJ_DAC);
    board->custom_port_table = jr2_port_table;
    board->init = jr2_init_sfp24;
    board->reset = jr2_reset;
    board->pre_reset = jr2_pre_reset;
    board->post_reset = jr2_post_reset;
    board->led_init = jr2_led_init;
    board->led_update = jr2_led_update_jr2;
    board->port_conf = jr2_port_conf;
    board->sfp_i2c_read = jr2_sfp_i2c_read;
    board->sfp_i2c_write = jr2_sfp_i2c_write;
    board->sfp_update_if = jr2_sfp_update_if;
    board->sfp_mod_detect = jr2_sfp_mod_detect;
    board->sfp_status = jr2_sfp_status;
    board->sfp_i2c_enable = jr2_sfp_i2c_enable;
    board->sfp_i2c_lock = 0;
    board->sfp_accept = jr2_sfp_accept;
    board->mux_mode = VTSS_PORT_MUX_MODE_AUTO;

    detect.miim_found[0] = 0;
    detect.miim_found[1] = 0;

    if (board_info->port_cfg == VTSS_BOARD_CONF_20x1G_4x2G5_2xSFI_NPI ) {
        for (port_no = 0; port_no < VTSS_PORTS; port_no++) {
            entry = &jr2_port_table[port_no];
            board_info->port_count = 27;
            if (port_no < 8) {
                entry->map.chip_port = port_no;
                entry->map.miim_controller = VTSS_MIIM_CONTROLLER_1;
                entry->map.miim_addr = port_no;
                entry->mac_if = VTSS_PORT_INTERFACE_SGMII;
                entry->cap = PORT_CAP_TRI_SPEED_COPPER;
            } else if (port_no < 24) {
                entry->map.chip_port = port_no;
                entry->map.miim_controller = VTSS_MIIM_CONTROLLER_NONE;
                entry->mac_if = VTSS_PORT_INTERFACE_SERDES;
                if (port_no > 11) {
                    entry->map.max_bw = VTSS_BW_1G;
                    entry->cap = PORT_CAP_SFP_1G | PORT_CAP_SFP_SD_HIGH;
                } else {
                    entry->cap = PORT_CAP_SFP_2_5G | PORT_CAP_SFP_SD_HIGH;
                }
        } else if (port_no <26) {
            entry->map.chip_port = (port_no + 27);
            entry->map.miim_controller = VTSS_MIIM_CONTROLLER_NONE;
            entry->mac_if = VTSS_PORT_INTERFACE_SFI;
            entry->cap = (PORT_CAP_10G_FDX | PORT_CAP_SFP_2_5G | PORT_CAP_FLOW_CTRL | PORT_CAP_SFP_SD_HIGH);
        } else if (port_no == 26) {
                entry->map.chip_port = 48; // NPI port
                entry->map.miim_controller = jr2_phy_detect(28, board_info); // Autodetect controller 0 or 1
                entry->map.miim_addr = 28;
                entry->mac_if = VTSS_PORT_INTERFACE_SGMII;
                entry->cap = PORT_CAP_TRI_SPEED_COPPER;
            } else {
                entry->map.chip_port = CHIP_PORT_UNUSED;
            }
        }
 
    } else if (board_info->port_cfg == VTSS_BOARD_CONF_20x1G_4x2G5_4xSFI_NPI || board_info->port_cfg == VTSS_BOARD_CONF_DEFAULT_VENICE_1G_MODE) {
        for (port_no = 0; port_no < VTSS_PORTS; port_no++) {
            entry = &jr2_port_table[port_no];
            board_info->port_count = 29;
            if (port_no < 8) {
                entry->map.chip_port = port_no;
                entry->map.miim_controller = VTSS_MIIM_CONTROLLER_1;
                entry->map.miim_addr = port_no;
                entry->mac_if = VTSS_PORT_INTERFACE_SGMII;
                entry->cap = PORT_CAP_TRI_SPEED_COPPER;
            } else if (port_no < 24) {
                entry->map.chip_port = port_no;
                entry->map.miim_controller = VTSS_MIIM_CONTROLLER_NONE;
                entry->mac_if = VTSS_PORT_INTERFACE_SERDES;
                if (port_no > 11) {
                    entry->map.max_bw = VTSS_BW_1G;
                    entry->cap = PORT_CAP_SFP_1G | PORT_CAP_SFP_SD_HIGH;
                } else {
                    entry->cap = PORT_CAP_SFP_2_5G | PORT_CAP_SFP_SD_HIGH;
                }
            } else if (port_no < 28) {
                // Auto detect XAUI ports and replace the SFI ports if detected
                detect.port_no = port_no;
                detect.miim_addr[0] = (port_no == 24) ? 24 : 25;
                detect.cap = 0;
                entry->map.max_bw   = VTSS_BW_DEFAULT;
                if (((port_no == 24) || (port_no == 25)) && jr2_10g_detect(&detect, board_info)) {
                    /* API Port 26,27 = XAUI chip ports 49,50 - possibly VTSS PHYs */
                    entry->cap                 = detect.cap;
                    if (board_info->port_cfg == VTSS_BOARD_CONF_DEFAULT_VENICE_1G_MODE) {
                        /* chip ports 49-50 do not map to single XAUI lanes instead we use chip ports 24 and 28 */
                        entry->map.chip_port       = port_no == 24 ? 24 : 28;
                        entry->cap |= PORT_CAP_1G_FDX | PORT_CAP_AUTONEG;
                    } else {
                        entry->map.chip_port       = port_no == 24 ? 49 : 50;
                    }

                    /* API Port 26-27: XAUI chip ports:49,50 possibly VTSS Phys */
                    entry->map.miim_controller = VTSS_MIIM_CONTROLLER_0;
                    entry->map.miim_addr = detect.miim_addr[0];
                    entry->mac_if = VTSS_PORT_INTERFACE_XAUI;
                } else {
                    entry->map.chip_port = (port_no + 25);
                    entry->map.miim_controller = VTSS_MIIM_CONTROLLER_NONE;
                    entry->mac_if = VTSS_PORT_INTERFACE_SFI;
                    entry->cap = (PORT_CAP_10G_FDX | PORT_CAP_SFP_2_5G | PORT_CAP_FLOW_CTRL | PORT_CAP_SFP_SD_HIGH);
                }
            } else if (port_no == 28) {
                entry->map.chip_port = 48; // NPI port
                entry->map.miim_controller = jr2_phy_detect(28, board_info); // Autodetect controller 0 or 1
                entry->map.miim_addr = 28;
                entry->mac_if = VTSS_PORT_INTERFACE_SGMII;
                entry->cap = PORT_CAP_TRI_SPEED_COPPER;
            } else {
                entry->map.chip_port = CHIP_PORT_UNUSED;
            }
        }
    } else if (board_info->port_cfg == VTSS_BOARD_CONF_24x2G5_2xSFI_NPI) {
        board->mux_mode = VTSS_PORT_MUX_MODE_1;
        board_info->port_count = 27;
        for (port_no = 0; port_no < VTSS_PORTS; port_no++) {
            entry = &jr2_port_table[port_no];
            if (port_no < 24) {
                entry->map.chip_port = port_no + 8;
                entry->map.miim_controller = VTSS_MIIM_CONTROLLER_NONE;
                entry->mac_if = VTSS_PORT_INTERFACE_SERDES;
                entry->cap = PORT_CAP_SFP_2_5G | PORT_CAP_SFP_SD_HIGH;
            } else if (port_no < 26) {
                entry->map.chip_port = port_no + 25;
                entry->map.miim_controller = VTSS_MIIM_CONTROLLER_NONE;
                entry->mac_if = VTSS_PORT_INTERFACE_SFI;
                entry->cap = (PORT_CAP_10G_FDX | PORT_CAP_SFP_2_5G | PORT_CAP_FLOW_CTRL | PORT_CAP_SFP_SD_HIGH);
            } else if (port_no == 26) {
                entry->map.chip_port = 48; // NPI port
                entry->map.miim_controller = jr2_phy_detect(28, board_info); // Autodetect controller 0 or 1
                entry->map.miim_addr = 28;
                entry->mac_if = VTSS_PORT_INTERFACE_SGMII;
                entry->cap = PORT_CAP_TRI_SPEED_COPPER;
            } else {
                entry->map.chip_port = CHIP_PORT_UNUSED;
            }
        }
    } else if (board_info->port_cfg == VTSS_BOARD_CONF_16x2G5_4xSFI_NPI) {
        board->mux_mode = VTSS_PORT_MUX_MODE_0;
        board_info->port_count = 21;
        for (port_no = 0; port_no < VTSS_PORTS; port_no++) {
            entry = &jr2_port_table[port_no];
            if (port_no < 16) {
                entry->map.chip_port = port_no + 8;
                entry->map.miim_controller = VTSS_MIIM_CONTROLLER_NONE;
                entry->mac_if = VTSS_PORT_INTERFACE_SERDES;
                entry->cap = PORT_CAP_SFP_2_5G | PORT_CAP_SFP_SD_HIGH;
            } else if (port_no < 20) {
                entry->map.chip_port = port_no + 33;
                entry->map.miim_controller = VTSS_MIIM_CONTROLLER_NONE;
                entry->mac_if = VTSS_PORT_INTERFACE_SFI;
                entry->cap = (PORT_CAP_10G_FDX | PORT_CAP_SFP_2_5G | PORT_CAP_FLOW_CTRL | PORT_CAP_SFP_SD_HIGH);
            } else if (port_no == 20) {
                entry->map.chip_port = 48;// NPI port
                entry->map.miim_controller = jr2_phy_detect(28, board_info); // Autodetect controller 0 or 1
                entry->map.miim_addr = 28;
                entry->mac_if = VTSS_PORT_INTERFACE_SGMII;
                entry->cap = PORT_CAP_TRI_SPEED_COPPER;
            } else {
                entry->map.chip_port = CHIP_PORT_UNUSED;
            }
        }
    }

    return TRUE;
}

static void jr2_map_update(vtss_port_no_t         port_no,
                           i32                    chip_port,
                           u8                     miim_addr,
                           vtss_miim_controller_t miim_controller,
                           vtss_port_interface_t  mac_if,
                           port_cap_t             cap)
{
    port_custom_entry_t *entry;
    if (port_no < VTSS_PORTS) {
        entry = &jr2_port_table[port_no];
        entry->map.chip_port = chip_port;
        entry->map.miim_controller = miim_controller;
        entry->map.miim_addr = miim_addr;
        entry->mac_if = mac_if;
        entry->cap = cap;
    }
}
BOOL vtss_board_probe_jr2_aqr(vtss_board_t *board, vtss_board_info_t *board_info)
{
    jr2_i2c_read   = board_info->i2c_read;
    jr2_i2c_write  = board_info->i2c_write;

    board->type = VTSS_BOARD_JAGUAR2_AQR_REF;
    jr2_board_type = board_info->board_type = board->type;
    board->name = "Jaguar-2 Atlanta Reference";
    board->features = (VTSS_BOARD_FEATURE_LOS | VTSS_BOARD_FEATURE_1588_REF_CLK_SEL | VTSS_BOARD_FEATURE_1588_CLK_ADJ_DAC);
    board->custom_port_table = jr2_port_table;
    board->init = jr2_init_aqr;
    board->reset = jr2_reset;
    board->pre_reset = jr2_pre_reset;
    board->post_reset = jr2_post_reset_aqr;
    board->led_init = jr2_led_init;
    board->led_update = jr2_led_update_aqr;
    board->port_conf = jr2_port_conf;
    board->sfp_i2c_read = jr2_sfp_i2c_read;
    board->sfp_i2c_write = jr2_sfp_i2c_write;
    board->sfp_update_if = jr2_sfp_update_if;
    board->sfp_mod_detect = jr2_sfp_mod_detect;
    board->sfp_status = jr2_sfp_status;
    board->sfp_i2c_enable = jr2_aqr_sfp_i2c_enable;
    board->sfp_i2c_lock = 0;
    board->sfp_accept = jr2_sfp_accept;
    board->mux_mode = VTSS_PORT_MUX_MODE_1; // Ports muxed to Serdes blocks: 24x2G5, chip ports 8-23,51-52

    /* Port mapping
     * Internal Port   Chip Port   MIIM Address    MIIM Controller
     * -------------   ---------   ------------    ---------------
     * 0               11          0               0
     * 1               10          1               0
     * 2               9           2               0
     * 3               8           3               0
     * 4               15          6               0
     * 5               14          5               0
     * 6               13          4               0
     * 7               12          7               0

     * 8               19          8               1
     * 9               18          9               1
     * 10              17          10              1
     * 11              16          11              1
     * 12              23          12              1
     * 13              22          13              1
     * 14              21          14              1
     * 15              20          15              1

     * 16              27          16              2
     * 17              26          17              2
     * 18              25          18              2
     * 19              24          19              2
     * 20              31          20              2
     * 21              30          21              2
     * 22              29          22              2
     * 23              28          23              2

     * 24              51          -               -
     * 25              52          -               -
     */
    jr2_map_update(0, 11, 0, VTSS_MIIM_CONTROLLER_0, VTSS_PORT_INTERFACE_SGMII, PORT_CAP_2_5G_TRI_SPEED_COPPER);
    jr2_map_update(1, 10, 1, VTSS_MIIM_CONTROLLER_0, VTSS_PORT_INTERFACE_SGMII, PORT_CAP_2_5G_TRI_SPEED_COPPER);
    jr2_map_update(2, 9,  2, VTSS_MIIM_CONTROLLER_0, VTSS_PORT_INTERFACE_SGMII, PORT_CAP_2_5G_TRI_SPEED_COPPER);
    jr2_map_update(3, 8,  3, VTSS_MIIM_CONTROLLER_0, VTSS_PORT_INTERFACE_SGMII, PORT_CAP_2_5G_TRI_SPEED_COPPER);
    jr2_map_update(4, 15, 4, VTSS_MIIM_CONTROLLER_0, VTSS_PORT_INTERFACE_SGMII, PORT_CAP_2_5G_TRI_SPEED_COPPER);
    jr2_map_update(5, 14, 5, VTSS_MIIM_CONTROLLER_0, VTSS_PORT_INTERFACE_SGMII, PORT_CAP_2_5G_TRI_SPEED_COPPER);
    jr2_map_update(6, 13, 6, VTSS_MIIM_CONTROLLER_0, VTSS_PORT_INTERFACE_SGMII, PORT_CAP_2_5G_TRI_SPEED_COPPER);
    jr2_map_update(7, 12, 7, VTSS_MIIM_CONTROLLER_0, VTSS_PORT_INTERFACE_SGMII, PORT_CAP_2_5G_TRI_SPEED_COPPER);

    jr2_map_update(8, 19, 8, VTSS_MIIM_CONTROLLER_1, VTSS_PORT_INTERFACE_SGMII, PORT_CAP_2_5G_TRI_SPEED_COPPER);
    jr2_map_update(9, 18, 9, VTSS_MIIM_CONTROLLER_1, VTSS_PORT_INTERFACE_SGMII, PORT_CAP_2_5G_TRI_SPEED_COPPER);
    jr2_map_update(10, 17, 10, VTSS_MIIM_CONTROLLER_1, VTSS_PORT_INTERFACE_SGMII, PORT_CAP_2_5G_TRI_SPEED_COPPER);
    jr2_map_update(11, 16, 11, VTSS_MIIM_CONTROLLER_1, VTSS_PORT_INTERFACE_SGMII, PORT_CAP_2_5G_TRI_SPEED_COPPER);
    jr2_map_update(12, 23, 12, VTSS_MIIM_CONTROLLER_1, VTSS_PORT_INTERFACE_SGMII, PORT_CAP_2_5G_TRI_SPEED_COPPER);
    jr2_map_update(13, 22, 13, VTSS_MIIM_CONTROLLER_1, VTSS_PORT_INTERFACE_SGMII, PORT_CAP_2_5G_TRI_SPEED_COPPER);
    jr2_map_update(14, 21, 14, VTSS_MIIM_CONTROLLER_1, VTSS_PORT_INTERFACE_SGMII, PORT_CAP_2_5G_TRI_SPEED_COPPER);
    jr2_map_update(15, 20, 15, VTSS_MIIM_CONTROLLER_1, VTSS_PORT_INTERFACE_SGMII, PORT_CAP_2_5G_TRI_SPEED_COPPER);

    jr2_map_update(16, 27, 16, VTSS_MIIM_CONTROLLER_2, VTSS_PORT_INTERFACE_SGMII, PORT_CAP_2_5G_TRI_SPEED_COPPER);
    jr2_map_update(17, 26, 17, VTSS_MIIM_CONTROLLER_2, VTSS_PORT_INTERFACE_SGMII, PORT_CAP_2_5G_TRI_SPEED_COPPER);
    jr2_map_update(18, 25, 18, VTSS_MIIM_CONTROLLER_2, VTSS_PORT_INTERFACE_SGMII, PORT_CAP_2_5G_TRI_SPEED_COPPER);
    jr2_map_update(19, 24, 19, VTSS_MIIM_CONTROLLER_2, VTSS_PORT_INTERFACE_SGMII, PORT_CAP_2_5G_TRI_SPEED_COPPER);
    jr2_map_update(20, 31, 20, VTSS_MIIM_CONTROLLER_2, VTSS_PORT_INTERFACE_SGMII, PORT_CAP_2_5G_TRI_SPEED_COPPER);
    jr2_map_update(21, 30, 21, VTSS_MIIM_CONTROLLER_2, VTSS_PORT_INTERFACE_SGMII, PORT_CAP_2_5G_TRI_SPEED_COPPER);
    jr2_map_update(22, 29, 22, VTSS_MIIM_CONTROLLER_2, VTSS_PORT_INTERFACE_SGMII, PORT_CAP_2_5G_TRI_SPEED_COPPER);
    jr2_map_update(23, 28, 23, VTSS_MIIM_CONTROLLER_2, VTSS_PORT_INTERFACE_SGMII, PORT_CAP_2_5G_TRI_SPEED_COPPER);

    jr2_map_update(24, 51, 0, VTSS_MIIM_CONTROLLER_NONE, VTSS_PORT_INTERFACE_SFI, PORT_CAP_10G_FDX | PORT_CAP_SFP_2_5G | PORT_CAP_FLOW_CTRL | PORT_CAP_SFP_SD_HIGH);
    jr2_map_update(25, 52, 0, VTSS_MIIM_CONTROLLER_NONE, VTSS_PORT_INTERFACE_SFI, PORT_CAP_10G_FDX | PORT_CAP_SFP_2_5G | PORT_CAP_FLOW_CTRL | PORT_CAP_SFP_SD_HIGH);
    return TRUE;
}

BOOL vtss_board_probe_jr2(vtss_board_t *board, vtss_board_info_t *board_info)
{
    memset(board, 0, sizeof(*board));
    memset(jr2_old_port_info, 0, sizeof(jr2_old_port_info));

    if (VTSS_PORTS > 37) {
        return vtss_board_probe_jr2_cu48(board, board_info);
    } else if (VTSS_PORTS == 26) {
        return vtss_board_probe_jr2_aqr(board, board_info);
    } else if ((VTSS_PORTS < 38) && (VTSS_PORTS > 15)) {
        return vtss_board_probe_jr2_sfp24(board, board_info);
    } else {
        return vtss_board_probe_srv2_nid(board, board_info);
    }
}

char *board_port_id_txt(vtss_board_port_cfg_t id)
{
    char *txt;
    switch (id) {
    case VTSS_BOARD_CONF_20x1G_4x2G5_4xSFI_NPI:
        txt = "20x1G + 4x2G5 + 4xSFP+ + NPI (29 ports, 71Gb)";
        break;
    case VTSS_BOARD_CONF_24x2G5_2xSFI_NPI:
        txt = "24x2G5 + 2xSFP+ + NPI (27 ports, 81Gb)";
        break;
    case VTSS_BOARD_CONF_16x2G5_4xSFI_NPI:
        txt = "16x2G5 + 4xSFP+ + NPI (21 ports, 81Gb)";
        break;
    case VTSS_BOARD_CONF_20x1G_4x2G5_2xSFI_NPI:
        txt = "20x1G + 4x2G5 + 2xSFP+ + NPI (27 ports, 51Gb)";
        break;
    case VTSS_BOARD_CONF_DEFAULT_VENICE_1G_MODE:
        txt = "Default board config with Venice ports in 1G";
        break;
    default:
        txt = "?";
        break;
    }
    return txt;
}

#endif /* defined(VTSS_ARCH_JAGUAR_2) */

/****************************************************************************/
/*                                                                          */
/*  End of file.                                                            */
/*                                                                          */
/****************************************************************************/
