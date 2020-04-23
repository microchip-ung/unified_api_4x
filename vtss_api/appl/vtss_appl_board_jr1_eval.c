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

#include "vtss_api.h"
#include "vtss_appl.h"

#if defined(BOARD_JAGUAR1_EVAL)

/* ================================================================= *
 *  Register access
 * ================================================================= */

#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/vitgenio.h>

static int fd;

static vtss_rc board_rd_wr(const u32 addr, u32 *const value, int write)
{
    struct vitgenio_cpld_spi_readwrite spi_buf;
    char                               *buf = &spi_buf.buffer[0];
    u32                                val;

    /* Address is 22 bits: (tgt_id << 14) + reg_addr */
    val = (write ? *value : 0xffffffff);
    buf[0] = ((write << 7) | ((addr >> 16) & 0x3f));
    buf[1] = ((addr >> 8) & 0xff);
    buf[2] = ((addr >> 0) & 0xff);
    buf[3] = ((val >> 24) & 0xff);
    buf[4] = ((val >> 16) & 0xff);
    buf[5] = ((val >> 8) & 0xff);
    buf[6] = ((val >> 0) & 0xff);
    spi_buf.length = 7;
    if (ioctl(fd, VITGENIO_CPLD_SPI_READWRITE, &spi_buf) < 0) {
        T_E("SPI_READWRITE failed");
        return VTSS_RC_ERROR;
    }
    if (!write) {
        *value = ((buf[3] << 24) | (buf[4] << 16) | (buf[5] << 8) | (buf[6] << 0));
    }
    T_N("%s: addr 0x%08x (0x%02x:0x%04x), value: 0x%08x", 
        write ? "WR" : "RD", addr, (addr >> 14) & 0xff, addr & 0x3fff, *value);

    return VTSS_RC_OK;
}

static vtss_rc reg_read(const vtss_chip_no_t chip_no,
                        const u32            addr,
                        u32                  *const value)
{
    return board_rd_wr(addr, value, 0);
}

static vtss_rc reg_write(const vtss_chip_no_t chip_no,
                         const u32            addr,
                         const u32            value)
{
    u32 val = value;
    return board_rd_wr(addr, &val, 1);
}


static void board_io_init(void)
{
    struct vitgenio_cpld_spi_setup setup = {
        /* char ss_select; Which of the CPLD_GPIOs is used for Slave Select */
        VITGENIO_SPI_SS_CPLD_GPIO0,
        VITGENIO_SPI_SS_ACTIVE_LOW, /* char ss_activelow; Slave Select (Chip Select) active low: true, active high: false */
        VITGENIO_SPI_CPOL_0, /* char sck_activelow; CPOL=0: false, CPOL=1: true */
        VITGENIO_SPI_CPHA_0, /* char sck_phase_early; CPHA=0: false, CPHA=1: true */
        VITGENIO_SPI_MSBIT_FIRST, /* char bitorder_msbfirst; */
        0, /* char reserved1; currently unused, only here for alignment purposes */
        0, /* char reserved2; currently unused, only here for alignment purposes */
        0, /* char reserved3; currently unused, only here for alignment purposes */
        200 /* unsigned int ndelay; minimum delay in nanoseconds, two of these delays are used per clock cycle */
    };

    if ((fd = open("/dev/vitgenio", 0)) < 0) {
        T_E("open /dev/vitgenio failed");
        return;
    }
    
    /* SI */
    ioctl(fd, VITGENIO_ENABLE_CPLD_SPI);
    ioctl(fd, VITGENIO_CPLD_SPI_SETUP, &setup);

    /* CPLD0 */
    setup.ss_select = VITGENIO_SPI_SS_CPLD_GPIO1;
    ioctl(fd, VITGENIO_ENABLE_CPLD_SPI);
    ioctl(fd, VITGENIO_CPLD_SPI_SETUP, &setup);

    /* CPLD1 */
    setup.ss_select = VITGENIO_SPI_SS_CPLD_GPIO2;
    ioctl(fd, VITGENIO_ENABLE_CPLD_SPI);
    ioctl(fd, VITGENIO_CPLD_SPI_SETUP, &setup);


   /* Set DUT  reset (CPLD reg 3) */
    reg_write(0, 3, 0);
    VTSS_MSLEEP(500);
    /* Release DUT reset (CPLD reg 3) */
    reg_write(0, 3, 0x7);
    VTSS_MSLEEP(500);
       
    /* SI */
    setup.ss_select = VITGENIO_SPI_SS_CPLD_GPIO0;
    ioctl(fd, VITGENIO_ENABLE_CPLD_SPI);
    ioctl(fd, VITGENIO_CPLD_SPI_SETUP, &setup);  
}


/* Board port map */
static vtss_port_map_t port_map[VTSS_PORT_ARRAY_SIZE];

static vtss_port_interface_t port_interface(vtss_port_no_t port_no)
{
    vtss_port_map_t *map = &port_map[port_no];
    return (map->chip_port > 23 ? VTSS_PORT_INTERFACE_XAUI : VTSS_PORT_INTERFACE_SGMII);
}

/* ================================================================= *
 *  Board init.
 * ================================================================= */

static int board_init(int argc, const char **argv, vtss_appl_board_t *board)
{
    vtss_port_no_t  port_no;
    vtss_port_map_t *map;

    board_io_init();
    board->init.init_conf->reg_read = reg_read;
    board->init.init_conf->reg_write = reg_write;
    board->init.init_conf->mux_mode = VTSS_PORT_MUX_MODE_0;
    board->port_map = port_map;
    board->port_interface = port_interface;

    /* Setup port map and calculate port count */
    board->port_count = VTSS_PORTS;
    for (port_no = VTSS_PORT_NO_START; port_no < VTSS_PORT_NO_END; port_no++) {
        map = &port_map[port_no];
        if (port_no < 24) {
            /* 1G ports */
            map->chip_port = port_no;
            map->miim_controller = VTSS_MIIM_CONTROLLER_0;
            map->miim_addr = (port_no + 8);
        } else if (port_no < 28) {
            /* XAUI ports */
            map->chip_port = (port_no + 3);
            map->miim_controller = VTSS_MIIM_CONTROLLER_NONE;
            map->miim_addr = -1;
        } else {
            map->chip_port = CHIP_PORT_UNUSED;
            map->miim_controller = VTSS_MIIM_CONTROLLER_NONE;
            map->miim_addr = -1;
            if (port_no < board->port_count) {
                board->port_count = port_no;
            }
        }
    }
    return 0;
}

static void board_init_post(vtss_appl_board_t *board) 
{   

}

void vtss_board_jr1_eval_init(vtss_appl_board_t *board)
{
    board->descr = "Jaguar1";
    board->target = VTSS_TARGET_JAGUAR_1;
    board->feature.port_control = 1;
    board->feature.layer2 = 1;
    board->feature.packet = 0;
    board->board_init = board_init;
    board->board_init_post = board_init_post;
}
#endif /* BOARD_JAGUAR1_EVAL */
