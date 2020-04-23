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

#include "vtss_api.h"
#include "vtss_appl.h"

#if defined(BOARD_SERVAL_VSIM)

/* ================================================================= *
 *  Register access
 * ================================================================= */

#include <sys/ioctl.h>
#include <fcntl.h>

static FILE *f_vsim_cmd;
static FILE *f_vsim_rsp;

static char *fname_vsim_cmd = "/users/mot/own_tools/cosim/vsim_cmd_pipe";
static char *fname_vsim_rsp = "/users/mot/own_tools/cosim/vsim_rsp_pipe";

static vtss_rc board_rd_wr(const u32 addr, u32 *const value, int write)
{
    char s[100];
    int restarting;

    fprintf (f_vsim_cmd, "%d 0x%08X 0x%08X\n", write, addr, *value);
    fflush(f_vsim_cmd);
    restarting = addr==0xffffffff && write && (*value)==0;
    do {
            fgets(s,100,f_vsim_rsp);
            if (restarting) printf ("Restarting garbage: %s\n", s); 
    } while (memcmp(s,"OK",2) && restarting);

    if (!write) {
            *value = strtoul(s,0,0);
    }
    T_N("%s: addr 0x%08x (0x%02x:0x%04x), value: 0x%08x\n", 
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

void vsim_nsleep(u32 n) {
        reg_write(0,0xffffffff,n|1);
}

static void board_io_init(void)
{
  printf ("Waiting for vsim target\n");
  f_vsim_cmd = fopen(fname_vsim_cmd, "w");
  f_vsim_rsp = fopen(fname_vsim_rsp, "r");
  printf ("Connected\n");
  reg_write(0,0xffffffff,0);
}

/* Board port map */
static vtss_port_map_t port_map[VTSS_PORT_ARRAY_SIZE];

static vtss_port_interface_t port_interface(vtss_port_no_t port_no)
{
    return (port_no < 8 ? VTSS_PORT_INTERFACE_SGMII : VTSS_PORT_INTERFACE_SERDES);
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
    board->port_map = port_map;
    board->port_interface = port_interface;

    /* Setup port map and calculate port count */
    board->port_count = VTSS_PORTS;
    for (port_no = VTSS_PORT_NO_START; port_no < VTSS_PORT_NO_END; port_no++) {
        map = &port_map[port_no];

        if (port_no < 8) {
            /* Octal PHY module assumed */
            map->chip_port = (8 - port_no - 1);
            map->miim_controller = VTSS_MIIM_CONTROLLER_0;
            map->miim_addr = (8 + port_no);
        } else {
            map->chip_port = port_no;
            map->miim_controller = VTSS_MIIM_CONTROLLER_NONE;
        }
    }

    return 0;
}

static void board_init_post(vtss_appl_board_t *board) 
{
}

void vtss_board_serval_vsim_init(vtss_appl_board_t *board)
{
    board->descr = "Serval";
    board->target = VTSS_TARGET_SERVAL;
    board->feature.port_control = 1;
    board->feature.layer2 = 1;
    board->feature.packet = 1;
    board->board_init = board_init;
    board->board_init_post = board_init_post;
}
#endif /* BOARD_SERVAL_VSIM */
