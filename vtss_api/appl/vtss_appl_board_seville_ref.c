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

#if defined(BOARD_SEVILLE_REF)

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

#include "vtss_appl.h"

/* Board port map */
static vtss_port_map_t port_map[VTSS_PORT_ARRAY_SIZE];

#define MMAP_SIZE   0x00290000
#define CHIPID_OFF (0x00070000 >> 2)
#define ENDIAN_OFF (0x00000000 >> 2)

#define HWSWAP_BE 0x81818181       /* Big-endian */
#define HWSWAP_LE 0x00000000       /* Little-endian */

int irq_fd;
#ifdef __BYTE_ORDER
#define IS_BIG_ENDIAN (__BYTE_ORDER == __BIG_ENDIAN)
#else
# error Unable to determine byteorder!
#endif

#if (__BYTE_ORDER == __BIG_ENDIAN)
#define CPU_TO_LE32(x) __cpu_to_le32(x)
#define LE32_TO_CPU(x) __le32_to_cpu(x)
#else
#define CPU_TO_LE32(x) (x)
#define LE32_TO_CPU(x) (x)
#endif

vtss_packet_tx_ifh_t tx_ifh;

static char iodev[64];
static volatile u32 *base_mem;

/* ================================================================= *
 *  Register access
 * ================================================================= */

#define PHY_FILE "/sys/bus/platform/devices/ffe800000.l2switch/phy_"

static vtss_rc reg_read(const vtss_chip_no_t chip_no,
                        const u32            addr,
                        u32                  *const value)
{
    *value = LE32_TO_CPU(base_mem[addr]);
    return VTSS_RC_OK;
}

static vtss_rc reg_write(const vtss_chip_no_t chip_no,
                         const u32            addr,
                         const u32            value)
{
    base_mem[addr] = CPU_TO_LE32(value);
    return VTSS_RC_OK;
}

int mdio_devfile(vtss_port_no_t port_no, int val)
{
    int fno = -1;
    if (port_no < VTSS_PORTS) {
        char fn[512];
        if (val) {
            snprintf(fn, sizeof(fn), "%s%d/phy_val", PHY_FILE, port_no);
            fno = open(fn, O_RDWR);
            if(fno < 0) {
                printf("open(%s) - fail: %s\n", fn, strerror(errno));
            }
        } else {
            snprintf(fn, sizeof(fn), "%s%d/phy_reg", PHY_FILE, port_no);
            fno = open(fn, O_RDWR);
            if(fno < 0) {
                printf("open(%s) - fail: %s\n", fn, strerror(errno));
            }
        }
    }
    return fno;
}

int mdio_reg(vtss_port_no_t port_no)
{
    return mdio_devfile(port_no, 0);
}

int mdio_val(vtss_port_no_t port_no)
{
    return mdio_devfile(port_no, 1);
}

static void read_val(int fno, int *value)
{
    char buf[256];
    int ct = read(fno, buf, sizeof(buf)-1);
    buf[ct] = '\0';
    *value = atoi(buf);
    close(fno);
}

static void write_val(int fno, int value)
{
    char buf[256];
    int ct = snprintf(buf, sizeof(buf), "%u\n", value);
    write(fno, buf, ct);
    close(fno);
}

static vtss_rc miim_read(const vtss_inst_t    inst,
                         const vtss_port_no_t port_no,
                         const u8             addr,
                         u16                  *const value)
{
    vtss_rc rc = VTSS_RC_ERROR;
    int phy_reg, phy_val;
    if ((phy_reg = mdio_reg(port_no)) >= 0) {
        write_val(phy_reg, addr);
        if ((phy_val = mdio_val(port_no)) >= 0) {
            int ival;
            read_val(phy_val, &ival);
            *value = (unsigned) ival;
            rc = VTSS_RC_OK;
        }
    }
    return rc;
}

static vtss_rc miim_write(const vtss_inst_t    inst,
                          const vtss_port_no_t port_no,
                          const u8             addr,
                          const u16            value)
{
    vtss_rc rc = VTSS_RC_ERROR;
    int phy_reg, phy_val;
    if ((phy_reg = mdio_reg(port_no)) >= 0) {
        write_val(phy_reg, addr);
        if ((phy_val = mdio_val(port_no)) >= 0) {
            write_val(phy_val, value);
            rc = VTSS_RC_OK;
        }
    }
    return rc;
}

static BOOL find_dev(const char *deviceName)
{
    const char *top = "/sys/class/uio";
    DIR *dir;
    struct dirent *dent;
    char fn[PATH_MAX], devname[128];
    FILE *fp;
    BOOL found = FALSE;

    if (!(dir = opendir (top))) {
        perror(top);
        exit (1);
    }

    while((dent = readdir(dir)) != NULL) {
        if (dent->d_name[0] != '.') {
            snprintf(fn, sizeof(fn), "%s/%s/name", top, dent->d_name);
            if ((fp = fopen(fn, "r"))) {
                fgets(devname, sizeof(devname), fp);
                fclose(fp);
                if (strstr(devname, deviceName)) {
                    snprintf(iodev, sizeof(iodev), "/dev/%s", dent->d_name);
                    found = TRUE;
                    break;
                }
            }
        }
    }

    closedir(dir);

    return found;
}

static void board_io_init(void)
{
    u32 value, value1;
    const int enable = 1;
    const char *deviceName = "Seville";

    if(!find_dev(deviceName)) {
        fprintf(stderr, "Unable to locate UIO device '%s', please check 'lspci -nn' and 'lsmod'\n",deviceName);
        exit(1);
    }
    /* Open the UIO device file */
    fprintf(stderr, "Using UIO: %s\n", iodev);
    irq_fd = open(iodev, O_RDWR);
    if (irq_fd < 1) {
        perror(iodev);
        exit(1);
    }

    /* mmap the UIO device */
    base_mem = mmap(NULL, MMAP_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, irq_fd, 0);
    if(base_mem != MAP_FAILED) {
        fprintf(stderr, "Mapped register memory @ %p\n", base_mem);
        value = HWSWAP_LE;
        fprintf(stderr, "UIO: Using normal PCIe byteorder value %08x\n", value);
        reg_write(0, ENDIAN_OFF, value);
        reg_read(0, ENDIAN_OFF, &value);
        reg_read(0, CHIPID_OFF, &value);
        fprintf(stderr, "Chipid: %08x\n", value);
        value = (value >> 12) & 0xffff;
        if (value != (value1 = 0x9953)) {
            fprintf(stderr, "Unexpected Chip ID, expected 0x%08x\n", value1);
            exit(1);
        }
    } else {
        perror("mmap");
    }

    if(write(irq_fd, &enable, sizeof(enable)) != sizeof(enable)) {
        perror("write() failed, unable to enable IRQs");
    }

}

static vtss_port_interface_t port_interface(vtss_port_no_t port_no)
{
    return (port_map[port_no].chip_port > 7 ? VTSS_PORT_INTERFACE_SERDES : VTSS_PORT_INTERFACE_QSGMII);
}

static BOOL port_poll(vtss_port_no_t port_no)
{
    return (port_map[port_no].chip_port > 9 ? 0 : 1);
}

static void rx_frames_register(vtss_appl_board_t *board)
{
    u8                      frame[2000];
    vtss_packet_rx_header_t header;
    vtss_packet_rx_queue_t  queue;
    u32                     total, frames;
                
    total = 0;
    do {
        frames = 0;
        queue = VTSS_PACKET_RX_QUEUE_START;
        if (vtss_packet_rx_frame_get(board->inst, queue, &header, frame, sizeof(frame)) == VTSS_RC_OK) {
            T_I("received frame on port_no: %u, queue_mask: 0x%08x, length: %u",
                header.port_no, header.queue_mask, header.length);
            total++, frames++;
        }
    } while(frames);
    T_I("Extracted %d frames", total);
}

static void irq_poll(vtss_appl_board_t *board)
{
    fd_set rfds;
    struct timeval tv;
    int ret, nfds = 0;

    FD_ZERO(&rfds);
    FD_SET(irq_fd, &rfds);
    nfds = MAX(nfds, irq_fd);
    tv.tv_sec = 0;
    tv.tv_usec = 1000;
    if((ret = select(nfds + 1, &rfds, NULL, NULL, &tv)) > 0) {
        if (FD_ISSET(irq_fd, &rfds)) {
            const int enable = 1;
            int n_irq;
            vtss_irq_status_t irqstat;
            read(irq_fd, &n_irq, sizeof(n_irq));
            if (vtss_irq_status_get_and_mask(NULL, &irqstat) == VTSS_RC_OK) {
                T_D("IRQ:%d active %x raw_ident %x raw_status %x raw_mask %x",
                    n_irq, irqstat.active, irqstat.raw_ident, irqstat.raw_status, irqstat. raw_mask);
                if (irqstat.active & (1 << VTSS_IRQ_XTR)) {
                    rx_frames_register(board);
                    vtss_irq_enable(NULL, VTSS_IRQ_XTR, TRUE);
                }
            }
            if(write(irq_fd, &enable, sizeof(enable)) != sizeof(enable)) {
                perror("write() failed, unable to enable IRQs");
            }
        }
    } else {
        if (ret < 0) {
            perror("select()");
        }
    }
}

/* ================================================================= *
 *  Board init.
 * ================================================================= */

static void board_init_post(vtss_appl_board_t *board)
{
    vtss_port_no_t port_no;

    for (port_no = 0; port_no < board->port_count; port_no++) {
        if (port_map[port_no].chip_port > 7) {
            /* Enable forwarding on CPU ports */
            vtss_port_state_set(board->inst, port_no, 1);
        }
    }
}

static void board_init_done(vtss_appl_board_t *board)
{
    vtss_phy_post_reset(NULL, 0);
    vtss_phy_post_reset(NULL, 4);
    vtss_irq_enable(NULL, VTSS_IRQ_XTR, TRUE);
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
    board->port_poll = port_poll;
    board->init.init_conf->reg_read = reg_read;
    board->init.init_conf->reg_write = reg_write;
    board->init.init_conf->miim_read = miim_read;
    board->init.init_conf->miim_write = miim_write;

    /* Setup port map, VTSS_PORTS determines the configuration:
       4 : 4 ports in slot 5 only
       6 : 4 ports in slot 5 and 2 CPU ports
       8 : 4 ports in slot 5 and 4 ports in slot 6
       10: 4 ports in slot 5, 4 ports in slot 6 and 2 CPU ports */

    for (port_no = 0; port_no < VTSS_PORTS; port_no++) {
        map = &port_map[port_no];

        map->miim_controller = VTSS_MIIM_CONTROLLER_NONE;
        if (port_no < 4) {
            /* Copper ports, slot 5, chip port 4-7  */
            map->chip_port = port_no;
        } else if (port_no < 8 && VTSS_PORTS > 6) {
            /* Copper ports, slot 6, chip port 0-3  */
            map->chip_port = port_no;
        } else {
            /* CPU ports, chip port 8-9 */
            map->chip_port = (8 + (port_no & 1));
        }
    }
    return 0;
}

static vtss_rc seville_post_reset(void)
{
    vtss_rc rc = VTSS_RC_OK;
    if (VTSS_PORTS >= 10) {
        BOOL members[VTSS_PORT_ARRAY_SIZE];

        /* Aggregate the 2 2.5G CPU ports */
        memset(members, 0, sizeof(members));
        members[8] = members[9] = TRUE;
        rc = vtss_aggr_port_members_set(NULL, VTSS_AGGR_NO_START, members);
    }
    return rc;
}

static void seville_pre_reset(void)
{
}

void vtss_board_seville_ref_init(vtss_appl_board_t *board)
{
    board->pre_reset  = seville_pre_reset;
    board->post_reset = seville_post_reset;
    board->port_count = VTSS_PORTS;
    board->descr = "T1040QDS";
    board->target = VTSS_TARGET_SEVILLE;
    board->feature.port_control = 1;
    board->feature.layer2 = 1;
    board->feature.packet = 1;
    board->board_init = board_init;
    board->board_periodic = irq_poll;
}
#endif /* BOARD_SEVILLE_REF */
