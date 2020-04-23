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

#include "vtss_api.h"  /* Defines BOARD_JAGUAR2_REF if applicable */
#include "vtss_appl.h"

#if defined(BOARD_JAGUAR2_VSIM)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <fcntl.h>

#include <dirent.h>
#include <endian.h>

#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

#include <linux/if_tun.h>
#include <linux/limits.h>

#include <asm/byteorder.h>

#define TUNDEV "/dev/net/tun"

/* ================================================================= *
 *  Register access
 * ================================================================= */

#include <sys/ioctl.h>
#include <fcntl.h>

static FILE *f_vsim_cmd;
static FILE *f_vsim_rsp;

static vtss_rc board_rd_wr(const u32 addr, u32 *const value, int write)
{
    static char s[100];
    u32 a, tgt_id, reg_addr, v=0;

    if (write) v=*value;
    fprintf (f_vsim_cmd, "%d 0x%08X 0x%08X\n", write, addr & 0x3fffff, v);
    fflush(f_vsim_cmd);

    a=(addr & 0x3fffff);
    if ((a>>21)==0) { tgt_id= (a>>14);    reg_addr=(a & 0x3FFF); }
    if ((a>>20)==2) { tgt_id= (a>>17)*8;  reg_addr=(a & 0x1FFFF); }
    if ((a>>20)==3) { tgt_id= (a>>18)*16; reg_addr=(a & 0x3FFFF); }

    if ((!write) || tgt_id==0)
        fgets(s,100,f_vsim_rsp);
        
    if (!write) 
        *value = strtoul(s,0,0);


    T_N("%s: addr 0x%08x (0x%02x:0x%04x), value: 0x%08x\n", 
        write ? "WR" : "RD", addr, tgt_id, reg_addr, *value);
  
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
        reg_write(0,1,n);
}

static void board_io_init(void)
{
  static char fn_cmd[100];
  static char fn_rsp[100];
  char s[100];
  sprintf (fn_cmd, "%s/vsim_cmd_for_%s", getenv("TMP"), getenv("USER"));
  sprintf (fn_rsp, "%s/vsim_rsp_for_%s", getenv("TMP"), getenv("USER"));

  printf ("Connecting to vsim target:\n   CommandPipe:  %s\n   ResponsePipe: %s\n", fn_cmd, fn_rsp);

  if ((f_vsim_cmd = fopen(fn_cmd, "w"))!=0) {
      fprintf (f_vsim_cmd, "1 0 0\n");
      fflush(f_vsim_cmd);
      sleep(1);
      f_vsim_rsp = fopen(fn_rsp, "r");
      fgets(s,20,f_vsim_rsp);
  }
  
  if (!(f_vsim_rsp && f_vsim_cmd) || memcmp(s,"0xDEADBEEF",10)) {
          printf ("Failed %s\n\n\n",s);
      exit(-1);
  }
  printf ("Pipes connected.\n");
}




/* Board port map */
static vtss_port_map_t port_map[VTSS_PORT_ARRAY_SIZE];

static vtss_port_interface_t port_interface(vtss_port_no_t port_no)
{
    return (port_map[port_no].miim_controller == VTSS_MIIM_CONTROLLER_NONE ? 
            VTSS_PORT_INTERFACE_SERDES : VTSS_PORT_INTERFACE_SGMII);
}


/* ================================================================= *
 *  Board init.
 * ================================================================= */

static void board_init_post(vtss_appl_board_t *board)
{
// JR2-TBD: Stub:
}

static void board_init_done(vtss_appl_board_t *board)
{
// JR2-TBD: Stub
}


static int board_init(int argc, const char **argv, vtss_appl_board_t *board)
{
    vtss_port_no_t  port_no;
    vtss_port_map_t *map;


    board_io_init();
    board->board_init_post = board_init_post;
    board->board_init_done = board_init_done;
    board->port_map = port_map;
    board->port_interface = port_interface;
    board->init.init_conf->reg_read = reg_read;
    board->init.init_conf->reg_write = reg_write;
    board->init.init_conf->mux_mode = VTSS_PORT_MUX_MODE_2;

    /* Setup port map and calculate port count */
    board->port_count = VTSS_PORTS;

    for (port_no = VTSS_PORT_NO_START; port_no < VTSS_PORT_NO_END; port_no++) {
        map = &port_map[port_no];
        if (port_no <= 5) {
            /* 1G ports */
            map->chip_port = (port_no==32)?48:port_no;
            map->miim_controller = VTSS_MIIM_CONTROLLER_NONE;
            map->miim_addr = -1;
        } else if (0 && port_no >= 33 && port_no <=35) {
            /* SD10G ports */
            map->chip_port = port_no-33+49;
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





void vtss_board_jr2_vsim_init(vtss_appl_board_t *board)
{
    board->descr = "Jaguar2";
    board->target = VTSS_TARGET_JAGUAR_2;
    board->feature.port_control = 1;
    board->feature.layer2 = 1;
    board->feature.packet = 1;
    board->board_init = board_init;
}
#endif /* BOARD_JAGUAR2_VSIM */
