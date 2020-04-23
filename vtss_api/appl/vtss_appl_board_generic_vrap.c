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

#include <net/if.h>
#include <net/ethernet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <linux/if_packet.h>

#include "vtss_api.h"
#include "vtss_appl.h"
#include "board_probe.h"

#if !defined (IO_METHOD_UIO)

static int irq_fd;

/* Global frame buffers */
#define VRAP_LEN  28
#define FRAME_MAX 2000
static u8 req_frame[VRAP_LEN];
static u8 rep_frame[FRAME_MAX];

static struct sockaddr_ll sock_addr;

static void frame_dump(const char *name, u8 *frame, int len)
{
    u32  i;
    char buf[100], *p = &buf[0];
    
    T_D("%s:", name);
    for (i = 0; i < len; i++) {
        if ((i % 16) == 0) {
            p = &buf[0];
            p += sprintf(p, "%04x: ", i);
        }
        p += sprintf(p,"%02x%c", frame[i], ((i + 9) % 16) == 0 ? '-' : ' ');
        if (((i + 1) % 16) == 0 || (i + 1) == len) {
            T_D("%s", buf);
        }
    }
}

static vtss_rc board_rd_wr(const u32 addr, u32 *const value, int write)
{
    u32     val = (write ? *value : 0);
    ssize_t n;
    BOOL    dump = 0;

    /* Address */
    req_frame[20] = ((addr >> 22) & 0xff);
    req_frame[21] = ((addr >> 14) & 0xff);
    req_frame[22] = ((addr >> 6) & 0xff);
    req_frame[23] = (((addr << 2) & 0xff) | (write ? 2 : 1));
    
    //dump = (req_frame[21] == 7 ? 1 : 0); /* DEVCPU_GCB */
    
    if (dump)
        T_D("%s, addr: 0x%08x, value: 0x%08x", write ? "WR" : "RD", addr, val);

    /* Value, only needed for write operations */
    req_frame[24] = ((val >> 24) & 0xff);
    req_frame[25] = ((val >> 16) & 0xff);
    req_frame[26] = ((val >> 8) & 0xff);
    req_frame[27] = (val & 0xff);
    
    n = VRAP_LEN;
    if (sendto(irq_fd, req_frame, n, 0, (struct sockaddr *)&sock_addr, sizeof(sock_addr)) < 0) {
        T_E("sendto failed");
        return VTSS_RC_ERROR;
    }
    if (dump)
        frame_dump("VRAP Request", req_frame, n);

    if ((n = recvfrom(irq_fd, rep_frame, FRAME_MAX, 0, NULL, NULL)) < 0) {
        T_E("recvfrom failed");
        return VTSS_RC_ERROR;
    }
    if (dump)
        frame_dump("VRAP Reply", rep_frame, n);
    
    if (!write) {
        *value = ((rep_frame[20] << 24) | (rep_frame[21] << 16) | 
                  (rep_frame[22] << 8) | rep_frame[23]);
    }

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

void board_io_vrap_init(vtss_board_info_t *board_info,
                       struct vtss_appl_board_t *board)
{
    struct ifreq ifr;
    int    protocol = htons(0x8880);

    /* Open socket */
    if ((irq_fd = socket(AF_PACKET, SOCK_RAW, protocol)) < 0) {
        T_E("socket create failed");
        return;
    }

    /* Get ifIndex */
    strcpy(ifr.ifr_name, "eth0");
    if (ioctl(irq_fd, SIOCGIFINDEX, &ifr) < 0) {
        T_E("SIOCGIFINDEX failed");
        return;
    }

    /* Initialize socket address */
    memset(&sock_addr, 0, sizeof(sock_addr));
    sock_addr.sll_family = htons(AF_PACKET);
    sock_addr.sll_protocol = protocol;
    sock_addr.sll_halen = ETH_ALEN;
    sock_addr.sll_ifindex = ifr.ifr_ifindex;

    /* Get SMAC */
    if (ioctl(irq_fd, SIOCGIFHWADDR, &ifr) < 0) {
        T_E("SIOCGIFHWADDR failed");
        return;
    }

    /* Initialize request frame */
    req_frame[0] = 0x00;
    req_frame[1] = 0x01; /* DMAC 00-01-00-00-00-00 */
    req_frame[2] = 0x00;
    req_frame[3] = 0x00;
    req_frame[4] = 0x00;
    req_frame[5] = 0x00;
    memcpy(&req_frame[6], ifr.ifr_hwaddr.sa_data, ETH_ALEN);
    req_frame[12] = 0x88;
    req_frame[13] = 0x80; 
    req_frame[15] = 0x04; /* EPID 0x0004 */
    req_frame[16] = 0x10; /* VRAP request */
    
    /* Copy DMAC */
    memcpy(&sock_addr.sll_addr, req_frame, ETH_ALEN);

    board_info->reg_read = reg_read;
    board_info->reg_write = reg_write;
    board_info->target = VTSS_TARGET_SERVAL;
}

#endif    // IO_METHOD_UIO
