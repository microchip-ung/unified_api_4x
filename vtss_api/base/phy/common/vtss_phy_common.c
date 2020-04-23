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

// Avoid "XXX.h not used in module "this_file.c"
/*lint --e{766} */

#include "vtss_phy_common.h"
#include "../phy_10g/chips/venice/vtss_venice_regs.h"
#include "../phy_10g/chips/malibu/vtss_malibu_regs_line_pma.h"
#include "../phy_10g/chips/malibu/vtss_malibu_regs_line_mac.h"
#include "../phy_10g/chips/malibu/vtss_malibu_regs_host_mac.h"
#include "../phy_10g/chips/malibu/vtss_malibu_regs_ptp_2.h"
#include "../phy_10g/chips/malibu/vtss_malibu_regs_ptp_3.h"
#if defined(VTSS_IOREG)
#undef VTSS_IOREG
#endif
#if defined(VTSS_TRACE_GROUP)
#undef VTSS_TRACE_GROUP
#define VTSS_TRACE_GROUP VTSS_TRACE_GROUP_PHY
#endif
/*fix Daytona warnings */
vtss_port_no_t base_port(vtss_state_t *vtss_state, vtss_port_no_t port_no);
vtss_port_no_t pma_port_no(vtss_state_t *vtss_state, vtss_port_no_t port_no);
#if defined(VTSS_CHIP_CU_PHY)
vtss_rc  debug_vtss_phy_1g_spi_read_write_inst(vtss_inst_t inst,
                                       vtss_port_no_t port_no,
                                   u8             rng_sel,
                                   BOOL           read,
                                   u8             target_id,
                                   u16            offset,
                                   u32            *const data);
#endif

#define VTSS_IOREG(dev, is32, off)  _ioreg(&vtss_state->io_var, (dev), (is32), (off))
#if defined(VTSS_CHIP_CU_PHY)
vtss_rc  debug_vtss_phy_1g_spi_read_write_inst(vtss_inst_t inst,
                                       vtss_port_no_t port_no,
                                   u8             rng_sel,
                                   BOOL           read,
                                   u8             target_id,
                                   u16            offset,
                                   u32            *const data)
{
    vtss_state_t *vtss_state;
    vtss_rc rc = VTSS_RC_ERROR;
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK)
        return vtss_phy_1g_spi_read_write(vtss_state, port_no, rng_sel, read, target_id, offset, data);
    else
    return rc;
}

vtss_rc vtss_phy_1g_spi_read_write(vtss_state_t   *vtss_state, 
                                   vtss_port_no_t port_no, 
                                   u8             rng_sel, 
                                   BOOL           read, 
                                   u8             target_id, 
                                   u16            offset, 
                                   u32            *const data)
{
    u8   tx_data[MAX_VIPER_SPI_STREAM];//56 + 24* -bits *(3 bytes of padding needed by rx_data)
    vtss_rc rc = VTSS_RC_OK;
    vtss_spi_read_write_t spi_read_write = vtss_state->init_conf.spi_read_write;
    u8   ch_id = vtss_state->phy_state[port_no].type.channel_id;

    if (vtss_state->phy_state[port_no].family != VTSS_PHY_FAMILY_VIPER) {//This SPI read/write common func only support VIPER PHY
        VTSS_E("port_no:%u, current phy part number is: %u", port_no, vtss_state->phy_state[port_no].type.part_number);
        return VTSS_RC_ERROR;
    }

    if (vtss_state->init_conf.spi_read_write == NULL) {
        VTSS_E("SPI access not supported");
        return VTSS_RC_ERROR;
    }
    memset(tx_data, 0, MAX_VIPER_SPI_STREAM);

    if (!read) {
        tx_data[3] = ((*data & 0xff000000) >> 24);
        tx_data[4] = ((*data & 0x00ff0000) >> 16);
        tx_data[5] = ((*data & 0x0000ff00) >> 8);
        tx_data[6] = ( *data & 0x000000ff);
    } 

    if (target_id == 0xE || target_id == 0xF) { // Valid targets for 1588 are 0xE,0xF,0x4E,0x4F
        target_id = (target_id & 0x3f) | ((ch_id % 2) << 6);
    } else {
        target_id = (target_id & 0x3f) | ((ch_id & 0x03) << 6);
    }
    tx_data[0] = (~read << 7) | ((rng_sel & 0x03) << 4) | ((target_id & 0xf0) >> 4);
    if ((target_id & 0x38) != 0x38) {//check if bits target_id[5:3] r not '111'
        tx_data[1] = ((target_id & 0x0f) << 4);//Target ID [1:0] for most Targets (read PS1046->8.3)
    } else {
        tx_data[1] = ((target_id & 0x0c) << 4);/* tx_data[13:12] is 00, when target_id[5:3] r '111'(as per PS1046),
                                                  then target_id[1:0] hardcoded to '00' */
        tx_data[1] |= ((offset & 0x3000) >> 8);//CSR register address[13:12] for MACsec INGR/EGR Targets
    }
    tx_data[1] |= ((offset & 0x0f00) >> 8);
    tx_data[2] = (offset & 0xff);

    rc = spi_read_write(NULL, port_no, MAX_VIPER_SPI_STREAM, tx_data);

    *data = (tx_data[3 + MAX_CFG_PADDING_BYTES] << 24) |
            (tx_data[4 + MAX_CFG_PADDING_BYTES] << 16) |
            (tx_data[5 + MAX_CFG_PADDING_BYTES] << 8 ) |
            (tx_data[6 + MAX_CFG_PADDING_BYTES]);

    VTSS_N("port_no:%u, %s, %02x_%04x %s %08x\n", port_no, read?"Read":"Write", target_id, offset, read?"->":"<-", *data);
    return rc;
}
#endif // VTSS_CHIP_CU_PHY

#ifdef VTSS_CHIP_10G_PHY
static BOOL is_1588_reg(u32 reg,u8 dev) 
{
    if (dev == 0x1E) {
        if (reg >= 0x9000 && reg <= 0xCFFF ) {
            return TRUE;
        } else {
            return FALSE;
        }
    }
    return FALSE;
}
u32 get_port_from_channel_id(struct vtss_state_s    *vtss_state,
                             const  vtss_port_no_t  port_no,
                             const  u16             ch_id)
{
    u32 port, port_count = 0, base_port = 0;
    port_count = vtss_state->port_count;
    u32 port_10g = 0xFFFFFFFF;
    for (port=0; port<port_count; ++port) {      /* Check all 10G ports to find the port corresponding to the channel-id  */
        if (vtss_state->phy_10g_state[port].type != VTSS_PHY_TYPE_10G_NONE) {
            /* the two ports should be on the same Chip
               for this purpose we compare the phy_api_base_no of the port and port_no being passed to the function.
               if they are equal then the two ports belong to the same chip */
            base_port = vtss_state->phy_10g_state[port].phy_api_base_no;
            if((vtss_state->phy_10g_state[port].channel_id == ch_id) && (base_port == vtss_state->phy_10g_state[port_no].phy_api_base_no)){
                port_10g = port;
                break;
            }
        }
    }
    if(port_10g == 0xFFFFFFFF)
        VTSS_E("Port no found, port_no %u channel_id %u", port_no, ch_id);

    return port_10g;
}
vtss_rc vtss_phy_10g_spi_read_write(vtss_state_t   *vtss_state, 
                                    vtss_port_no_t port_no, 
                                    BOOL           read, 
                                    u8             dev, 
                                    u16            reg_num, 
                                    u32            *const data)
{
    u8 tx_data[MAX_VENICE_SPI_STREAM], rx_data[MAX_VENICE_SPI_STREAM];// 56-bits
    u8 channel_no = vtss_state->phy_10g_state[(dev == 0x1e) ? vtss_state->phy_10g_state[port_no].phy_api_base_no : port_no].channel_id; // bits[54:53].
    vtss_spi_read_write_t spi_read_write = vtss_state->init_conf.spi_read_write;
    vtss_rc rc = VTSS_RC_OK;

    if (vtss_state->phy_10g_state[port_no].family != VTSS_PHY_FAMILY_VENICE &&vtss_state->phy_10g_state[port_no].family != VTSS_PHY_FAMILY_MALIBU) {//This SPI read/write common func only support VENICE PHY
        VTSS_I("port_no:%u, current phy type is: %u", port_no, vtss_state->phy_10g_state[port_no].type);
    }

    if (vtss_state->init_conf.spi_read_write == NULL) {
        VTSS_E("SPI access not supported");
        return VTSS_RC_ERROR;
    }
    /* For PHYs with more than one 1588 instances */
    if (vtss_state->phy_10g_state[port_no].family == VTSS_PHY_FAMILY_MALIBU && is_1588_reg(reg_num,dev)) {
        if(vtss_state->phy_10g_state[port_no].channel_id > 1) {
            /*Assigning base channel for 2nd 1588 instance */
            channel_no = 2;
        }
    }
    memset(tx_data, 0, MAX_VENICE_SPI_STREAM);
    memset(rx_data, 0, MAX_VENICE_SPI_STREAM);

    if (!read) {
        tx_data[3] = ((*data & 0xff000000) >> 24);
        tx_data[4] = ((*data & 0x00ff0000) >> 16);
        tx_data[5] = ((*data & 0x0000ff00) >> 8);
        tx_data[6] = ( *data & 0x000000ff);
    }

    /* Venice 56-bit Slave SPI interface */
    tx_data[0] = (~read << 7) | ((channel_no & 0x3) << 5) | (dev & 0x1f);
    tx_data[1] = (reg_num >> 8);
    tx_data[2] = (reg_num & 0xff);

    VTSS_RC(spi_read_write(vtss_state, port_no, MAX_VENICE_SPI_STREAM, tx_data));

    if (read) {//Might be fixed in future PHY rev (i.e. next PHY may not read twice to get actual reg value)
        rc = spi_read_write(NULL, port_no, MAX_VENICE_SPI_STREAM, rx_data);//don't care about data 2nd time, i.e. last arg can be anything
        *data = (rx_data[3] << 24) |
                (rx_data[4] << 16) |
                (rx_data[5] << 8 ) |
                (rx_data[6]);
    }
    VTSS_N("port_no:%u, %s, %02x_%04x %s %08x\n", port_no, read?"Read":"Write", dev, reg_num, read?"->":"<-", *data);
    return rc;
}
#endif // VTSS_CHIP_10G_PHY

#if defined(VTSS_CHIP_10G_PHY) || defined(VTSS_FEATURE_MACSEC)
ioreg_blk *_ioreg(ioreg_blk *io, u16 dev, BOOL is32, u32 off)
{
    io->mmd = dev;
    io->is32 = is32;
    io->addr = off;
    return io;
}
#endif
vtss_rc phy_type_get(vtss_state_t *vtss_state,
                        const vtss_port_no_t port_no, BOOL *const clause45)
{

#ifdef VTSS_CHIP_CU_PHY
    if (vtss_state->phy_state[port_no].type.part_number != VTSS_PHY_TYPE_NONE) {
        *clause45 = 0;
        return VTSS_RC_OK;
    }
#endif

#ifdef VTSS_CHIP_10G_PHY
    if (vtss_state->phy_10g_state[port_no].type != VTSS_PHY_TYPE_10G_NONE) {
        *clause45 = 1;
        return VTSS_RC_OK;
    }
#endif
    return VTSS_RC_ERROR;
}

vtss_port_no_t base_port(vtss_state_t *vtss_state, vtss_port_no_t port_no)
{
#ifdef VTSS_CHIP_10G_PHY
    return vtss_state->phy_10g_state[port_no].phy_api_base_no;
#else
    return port_no;
#endif
}
vtss_port_no_t pma_port_no(vtss_state_t *vtss_state, vtss_port_no_t port_no) 
{
#ifdef VTSS_CHIP_10G_PHY
    /* Only one PMA in VSC8491 at channel 0  */
    if (vtss_state->phy_10g_state[port_no].type == VTSS_PHY_TYPE_8491 ||
            vtss_state->phy_10g_state[port_no].type == VTSS_PHY_TYPE_8487) {
        return vtss_state->phy_10g_state[port_no].phy_api_base_no;
    }
#endif
    return port_no;
}

vtss_rc get_base_adr(vtss_state_t *vtss_state,vtss_port_no_t port_no, u32 dev, u32 addr, u32 *phy10g_base, u32 *target_id, u32 *offset, BOOL *use_base_port)
{
    *target_id = 0;
    *phy10g_base = addr;
    *use_base_port = TRUE;
#ifdef VTSS_CHIP_10G_PHY
    switch (vtss_state->phy_10g_state[port_no].family) {
        case VTSS_PHY_FAMILY_MALIBU: 
            switch (dev) {
                case 1:
                    if ( (addr >=0x96 && addr <=0x9b) || 
                            (addr >=0x8201 && addr <=0x82ff ) ) {
                        *target_id = 0x7;
                    } else if ( (addr >=0xc000 && addr <=0xc00f ) ) {
                        *target_id = 0xf;
                    } else if ( (addr >=0xc010 && addr <=0xc01f )) {
                        *target_id = 0x9;
                    } else if ((addr >=0xf000 && addr <=0xf0ff ) || (addr >= 0xf100 && addr <= 0xf1ff)) {
                        *phy10g_base = 0xf000;
                        *target_id = 0x10;
                    } else {
                        *target_id = 0x1;
                    }
                    break;
                case 2:
                    *target_id = 0x2;
                    break;
                case 3:
                    if ((addr <=0x7fff ) || ((addr >=0x8000) && (addr <=0x8fff) )) {
                        *target_id = 0x3;
                    } else if ((addr >=0xe000 && addr <=0xe0ff )) {
                        *target_id = 0x5;
                    } else if ((addr >=0xe100 && addr <=0xe1ff )) {
                        *target_id = 0x6;
                    } else if ((addr >=0xf000 && addr <=0xf0ff )) {
                        *target_id = 0xC;
                        *phy10g_base = 0xf000;
                    } else if ((addr >=0xf100 && addr <=0xf1ff )) {
                        *phy10g_base = 0xf100;
                        *target_id = 0xD;
                    } else if ((addr >=0xf200 && addr <=0xf2ff )) {
                        *phy10g_base = 0xf200;
                        *target_id = 0xE;
                    }
                    break;
                case 4:
                    *target_id = 0x11;
                    break;
                case 7:
                    *target_id = 0x8;
                    break;
                case 9:
                    if ((addr >=0x96 && addr <=0x9b ) ||
                            (addr >=0x8201 && addr <=0x82ff )) {
                        *target_id = 0x47;
                    } else if ((addr >=0xf000 && addr <=0xf0ff ) || (addr >=0xf100 && addr <=0xf1ff )) {
                        *phy10g_base = 0xf000;
                        *target_id = 0x50;
                    } else {
                        *target_id = 0x41;
                    }
                    break;
                case 0xb:
                    *target_id = 0x43;
                    break;
                case 0xf:
                    *target_id = 0x48;
                    break;
                case 0x1f:
                    if (addr <=0x7fff) {
                        *target_id = 0xa;
                        *phy10g_base = 0;
                    } else if ((addr >=0x8000 && addr <=0xffff )) {
                        *phy10g_base = 0x8000;
                        *target_id = 0xb;
                    }
                    break;
                case 0x1e:
                    if ((addr <=0x6fff ) || (addr >=0x7000 && addr <=0x70ff ) || (addr >=0x7100 && addr <=0x71ff ) ) {
                        *target_id = 0x61;
                    } else if ((addr >=0x7200 && addr <=0x72ff )) {
                        *target_id = 0x70;
                    } else if ((addr >=0x8100 && addr <=0x81ff )) {
                        *target_id = 0x6b;
                    } else if ((addr >=0x8200 && addr <=0x82ff )) {
                        *target_id = 0x6c;
                    } else if ((addr >=0x9000 && addr <=0x91ff )) {
                        if (vtss_state->phy_10g_state[port_no].channel_id <= 1 ) {
                            *target_id = 0x62;
                        } else {
                            *target_id = 0x72;
                        }
                        *phy10g_base = 0x9000;
                        *use_base_port = FALSE;
                    } else if ((addr >=0x9200 && addr <=0x93ff )) {
                        if (vtss_state->phy_10g_state[port_no].channel_id <= 1 ) {
                            *target_id = 0x63;
                        } else {
                            *target_id = 0x73;
                        }
                        *phy10g_base = 0x9200;
                        *use_base_port = FALSE;
                    } else if ((addr >=0xA000 && addr <=0xA7FF )) {
                        *target_id = 0x64;
                        *phy10g_base = 0xa000;
                        *use_base_port = FALSE;
                    } else if ((addr >=0xA800 && addr <=0xAFFF )) {
                        *phy10g_base = 0xa800;
                        *target_id = 0x65;
                        *use_base_port = FALSE;
                    } else if ((addr >=0xB000 && addr <=0xB7FF )) {
                        *target_id = 0x66;
                        *phy10g_base = 0xb000;
                        *use_base_port = FALSE;
                    } else if ((addr >=0xB800 && addr <=0xBFFF )) {
                        *phy10g_base = 0xb800;
                        *target_id = 0x67;
                        *use_base_port = FALSE;
                    } else if ((addr >=0xC000 && addr <=0xC7FF )) {
                        *phy10g_base = 0xc000;
                        *target_id = 0x68;
                        *use_base_port = FALSE;
                    } else if ((addr >=0xC800 && addr <=0xCFFF )) {
                        *phy10g_base = 0xc800;
                        *target_id = 0x69;
                        *use_base_port = FALSE;
                    } else if ((addr >=0xEE00 && addr <=0xEEFF )) {
                        *target_id = 0x6f;
                    } else if ((addr >=0xEF00 && addr <=0xEF7F )) {
                        *phy10g_base = 0xEF00;
                        *target_id = 0x6a;
                    } else if ((addr >=0xF000 && addr <=0xF07F )) {
                        *phy10g_base = 0xF000;
                        *target_id = 0x6a;
                    } else if ((addr >=0xF100 && addr <=0xF17F )) {
                        *phy10g_base = 0xF100;
                        *target_id = 0x6d;
                    } else if ((addr >=0xF200 && addr <=0xF27F )) {
                        *phy10g_base = 0xF200;
                        *target_id = 0x6e;
                    }
                    break;
                default:
                    return VTSS_RC_ERROR;
            }
            break;
        default:
#endif
            switch(dev) {
                case 1:
                    if (addr >= 0xF000) {
                        *phy10g_base = 0xF000;
                    }
                    break;
                case 3:
                    if ((addr >= FC_BUFFER) && (addr < 0xF07F)) {
                        *phy10g_base = FC_BUFFER;
                        *target_id = 4;
                    } else if ((addr >= HOST_MAC) && (addr < 0xF17F)) {
                        *phy10g_base = HOST_MAC;
                        *target_id = 5;
                    } else if ((addr >= LINE_MAC) && (addr < 0xF27F)) {
                        *phy10g_base = LINE_MAC;
                        *target_id = 6;
                    }
                    break;
                case 30:
                    if ((addr >= PROC_ID_0) && (addr < 0x91FF)) {
                        *phy10g_base = PROC_ID_0;
                        *target_id = 0xe;
                    } else if ((addr >= PROC_ID_2) && (addr < 0x93FF)) {
                        *phy10g_base = PROC_ID_2;
                        *target_id = 0xf;
                    } else if ((addr >= 0xA000) && (addr < 0xA7FF)) {
                        *phy10g_base = 0xA000;
                    } else if ((addr >= 0xA800) && (addr < 0xAFFF)) {
                        *phy10g_base = 0xA800;
                    } else if ((addr >= 0xB000) && (addr < 0xB7FF)) {
                        *phy10g_base = 0xB000;
                    } else if ((addr >= 0xB800) && (addr < 0xBFFF)) {
                        *phy10g_base = 0xB800;
                    } else if ((addr >= 0xC000) && (addr < 0xC7FF)) {
                        *phy10g_base = 0xC000;
                    } else if ((addr >= 0xC800) && (addr < 0xCFFF)) {
                        *phy10g_base = 0xC800;
                    } else if ((addr >= 0xF000) && (addr < 0xF07F)) {
                        *phy10g_base = 0xF000;
                    }
                    break;
                case 31:
                    if (addr < MACSEC_EGR) {
                        *phy10g_base = MACSEC_INGR;
                        *target_id = 0x38;
                    } else {
                        *phy10g_base = MACSEC_EGR;
                        *target_id = 0x3c;
                    }
                    break;
                default:
                    return VTSS_RC_ERROR;
            }
#ifdef VTSS_CHIP_10G_PHY
            break;
    }
#endif
    *offset = addr - *phy10g_base;
    return VTSS_RC_OK;
}
vtss_rc csr_rd(vtss_state_t *vtss_state, vtss_port_no_t port_no, u16 mmd, BOOL is32, u32 addr, u32 *value)
{
    BOOL                       clause45 = FALSE;
    u32                        reg_addr = 0;
    u16                        reg_value[2];
    u32                        offset, target, base_addr;
    BOOL                       use_base_port = TRUE;
#ifdef VTSS_CHIP_10G_PHY
    vtss_phy_10g_family_t      phy_family = VTSS_PHY_FAMILY_10G_NONE;
    vtss_port_no_t port=0;

    phy_family = vtss_state->phy_10g_state[port_no].family;
#endif /* VTSS_CHIP_10G_PHY */
    VTSS_RC(phy_type_get(vtss_state, port_no, &clause45));
    /* Use the SPI access method if available */
    if ((vtss_state->init_conf.spi_read_write != NULL ||
        vtss_state->init_conf.spi_32bit_read_write != NULL)) {
        if (clause45) {
#ifdef VTSS_CHIP_10G_PHY
            if (phy_family == VTSS_PHY_FAMILY_VENICE || phy_family == VTSS_PHY_FAMILY_MALIBU) {
                if (vtss_state->init_conf.spi_32bit_read_write != NULL) {
                    port = (mmd == 0x1e) ? PHY_BASE_PORT(port_no) : port_no;
                    VTSS_RC(vtss_state->init_conf.spi_32bit_read_write(vtss_state, port, 1, (u8)mmd, (u16)addr, value));
                } else {
                    VTSS_RC(vtss_phy_10g_spi_read_write(vtss_state, port_no, 1, (u8)mmd, (u16)addr, value));
                }
            }   
            VTSS_I("SPI 10G: RD port %u is_32_bit %s : reg %0xX%0x = 0x%0x",port_no,is32?"TRUE":"FALSE",mmd,addr,*value);
#endif /* VTSS_CHIP_10G_PHY */
        } else {
#ifdef VTSS_CHIP_CU_PHY
            VTSS_RC(get_base_adr(vtss_state, port_no, mmd, addr, &base_addr, &target, &offset, &use_base_port));
            if (vtss_state->init_conf.spi_32bit_read_write != NULL) {
                VTSS_RC(vtss_state->init_conf.spi_32bit_read_write(vtss_state, port_no, 1, (u8)target, (u16)offset, value));
            } else {
                VTSS_RC(vtss_phy_1g_spi_read_write(vtss_state, port_no, 0, 1, (u8)target, (u16)offset, value));
            }
            VTSS_I("SPI 1G: RD port %u is_32_bit %s : reg %0xX%0x = 0x%0x",port_no,is32?"TRUE":"FALSE",mmd,addr,*value);
#endif /* VTSS_CHIP_10G_PHY */
        }
        return VTSS_RC_OK;
    }
    if (clause45) {
        vtss_mmd_read_t     mmd_read_func = vtss_state->init_conf.mmd_read;
        vtss_mmd_read_inc_t mmd_read_inc_func = vtss_state->init_conf.mmd_read_inc;
        vtss_port_no_t p = (mmd == 0x1e) ? PHY_BASE_PORT(port_no) : port_no;
        if (is32) {
            VTSS_RC(get_base_adr(vtss_state, port_no, mmd, addr, &base_addr, &target, &offset, &use_base_port));
            reg_addr = base_addr | (offset << 1);
            if (!use_base_port) {
                p = port_no;
            }
            VTSS_RC(mmd_read_inc_func(vtss_state, p, mmd, reg_addr, reg_value, 2));
            *value = reg_value[0] + (((u32)reg_value[1]) << 16);
        } else {
            VTSS_RC(mmd_read_func(vtss_state, p, mmd, addr, &reg_value[0]));
            *value = (u32)reg_value[0];
        }
        VTSS_D("MMD RD port %u is_32_bit %s : reg %0xX%0x = 0x%0x",port_no,is32?"TRUE":"FALSE",mmd,addr,*value);
    } else {
#ifdef VTSS_CHIP_CU_PHY
        VTSS_RC(get_base_adr(vtss_state, port_no, mmd, addr, &base_addr, &target, &offset, &use_base_port));
        VTSS_RC(vtss_phy_macsec_csr_rd_private(vtss_state, port_no, target, offset, value));
        VTSS_I("1G RD port %u is_32_bit %s : reg %0xX%0x = 0x%0x",port_no,is32?"TRUE":"FALSE",mmd,addr,*value);
#endif /* VTSS_CHIP_CU_PHY */
    }

    return VTSS_RC_OK;
}


vtss_rc csr_wr(vtss_state_t *vtss_state, vtss_port_no_t port_no, u16 mmd, BOOL is32, u32 addr, u32 value)
{

    u16               reg_value_upper, reg_value_lower;
    BOOL              clause45 = FALSE, use_base_port = TRUE;
    u32               offset, target, base_addr, reg_addr;
#ifdef VTSS_CHIP_10G_PHY
    vtss_phy_10g_family_t      phy_family = VTSS_PHY_FAMILY_10G_NONE;
    vtss_port_no_t port=0;

    phy_family = vtss_state->phy_10g_state[port_no].family;
#endif /* VTSS_CHIP_10G_PHY */
    VTSS_RC(phy_type_get(vtss_state, port_no, &clause45));
    /* Use the SPI access method if available */
    if (vtss_state->init_conf.spi_read_write != NULL ||
        vtss_state->init_conf.spi_32bit_read_write != NULL) {
        if (clause45) {
#ifdef VTSS_CHIP_10G_PHY
            if (phy_family == VTSS_PHY_FAMILY_VENICE || phy_family == VTSS_PHY_FAMILY_MALIBU ) {
                if (vtss_state->init_conf.spi_32bit_read_write != NULL) {
                    port = (mmd == 0x1e) ? PHY_BASE_PORT(port_no) : port_no;
                    VTSS_RC(vtss_state->init_conf.spi_32bit_read_write(vtss_state, port, 0, (u8)mmd, (u16)addr, &value));
                } else {
                    VTSS_RC(vtss_phy_10g_spi_read_write(vtss_state, port_no, 0, (u8)mmd, (u16)addr, &value));
                }
            }
            VTSS_I("SPI 10G WR port %u is_32_bit %s : reg %0xX%0x = 0x%0x",port_no,is32?"TRUE":"FALSE",mmd,addr,value);
#endif /* VTSS_CHIP_10G_PHY */
        } else {
#ifdef VTSS_CHIP_CU_PHY
            VTSS_RC(get_base_adr(vtss_state, port_no, mmd, addr, &base_addr, &target, &offset, &use_base_port));
            if (vtss_state->init_conf.spi_32bit_read_write != NULL) {
                VTSS_RC(vtss_state->init_conf.spi_32bit_read_write(vtss_state, port_no, 0, (u8)target, (u16)offset, &value));
            } else {
                VTSS_RC(vtss_phy_1g_spi_read_write(vtss_state, port_no, 0, 0, (u8)target, (u16)offset, &value));
            }
            VTSS_I("SPI 1G WR port %u is_32_bit %s : reg %0xX%0x = 0x%0x",port_no,is32?"TRUE":"FALSE",mmd,addr,value);
#endif /* VTSS_CHIP_CU_PHY */
        }
        return VTSS_RC_OK;
    }

    /* Divide the 32 bit value to [15..0] Bits & [31..16] Bits */
    reg_value_lower = (value & 0xffff);
    reg_value_upper = (value >> 16);

    if (clause45) {
        vtss_mmd_write_t  mmd_write_func = vtss_state->init_conf.mmd_write;
        vtss_port_no_t p = (mmd == 0x1e) ? PHY_BASE_PORT(port_no) : port_no;
        if (is32) {
            VTSS_RC(get_base_adr(vtss_state, port_no, mmd, addr, &base_addr, &target, &offset, &use_base_port));
            reg_addr = base_addr | (offset << 1);
            if (!use_base_port) {
                p = port_no;
            }
            /* Write the Upper 2 Bytes */
            VTSS_RC(mmd_write_func(vtss_state, p, mmd, (reg_addr | 1), reg_value_upper));
            /* Write the Lower 2 Bytes */
            VTSS_RC(mmd_write_func(vtss_state, p, mmd, reg_addr, reg_value_lower));
        } else {
            VTSS_RC(mmd_write_func(vtss_state, p, mmd, addr, reg_value_lower));
        }
        VTSS_D("MMD WR port %u is_32_bit %s : reg %0xX%0x = 0x%0x",port_no,is32?"TRUE":"FALSE",mmd,addr,value);
    } else {
        // 1G PHY access
#ifdef VTSS_CHIP_CU_PHY
        VTSS_RC(get_base_adr(vtss_state, port_no, mmd, addr, &base_addr, &target, &offset, &use_base_port));
        VTSS_RC(vtss_phy_macsec_csr_wr_private(vtss_state, port_no, target, offset, value));
        VTSS_I("MMD 1G WR port %u is_32_bit %s : reg %0xX%0x = 0x%0x",port_no,is32?"TRUE":"FALSE",mmd,addr,value);
#endif /* VTSS_CHIP_CU_PHY */
    }
    return VTSS_RC_OK;
}


vtss_rc csr_wrm(vtss_state_t *vtss_state, vtss_port_no_t port_no, u16 mmd, BOOL is32, u32 addr, u32 value, u32 mask)
{
    vtss_rc rc;
    u32     val;

    if ((rc = csr_rd(vtss_state, port_no, mmd, is32, addr, &val)) == VTSS_RC_OK) {
        val = ((val & ~mask) | (value & mask));
        rc = csr_wr(vtss_state, port_no, mmd, is32, addr, val);
    }
    return rc;
}

#if defined(VTSS_CHIP_10G_PHY) || defined(VTSS_FEATURE_MACSEC)
vtss_rc _csr_rd(vtss_state_t *vtss_state, vtss_port_no_t port_no, ioreg_blk *io, u32 *value)
{
    return csr_rd(vtss_state, port_no, io->mmd, io->is32, io->addr, value);
}

vtss_rc _csr_wr(vtss_state_t *vtss_state, vtss_port_no_t port_no, ioreg_blk *io, u32 value)
{
    return csr_wr(vtss_state, port_no, io->mmd, io->is32, io->addr, value);
}

vtss_rc _csr_wrm(vtss_state_t *vtss_state, vtss_port_no_t port_no, ioreg_blk *io, u32 value, u32 mask)
{
    return csr_wrm(vtss_state, port_no, io->mmd, io->is32, io->addr, value, mask);
}

vtss_rc _csr_warm_wrm(vtss_state_t *vtss_state, vtss_port_no_t port_no, ioreg_blk *io, u32 value,
                             u32 mask, u32 chk_mask, const char *function, const u16 line)
{
    u32  curr_val;
#if defined(VTSS_CHIP_10G_PHY)
    vtss_phy_10g_port_state_t *ps = &vtss_state->phy_10g_state[port_no];
#endif

    //vtss_macsec_internal_glb_t *macsec = &vtss_state->macsec_conf[port_no].glb;
    if (vtss_state->sync_calling_private) {
        /* Read the current register value and compare with requested */
        VTSS_RC(csr_rd(vtss_state, port_no, io->mmd, io->is32, io->addr, &curr_val));
        if ((curr_val ^ value) & mask & chk_mask) { /* Change in bit field */
            VTSS_E("Warm start synch. field changed: Port:%u MMD:%d Register:0x%X  \n", port_no, io->mmd, io->addr);
            VTSS_E("Mask:0x%X Chip value:0x%X API value:0x%X \n", mask, curr_val, value);
            VTSS_E("Function:%s, Line:%d (chk_mask:0x%X) \n", function, line, chk_mask);
#if defined(VTSS_CHIP_10G_PHY)
            VTSS_I("ps->warm_start_reg_changed is: true, on port: %d\n", port_no+1);
            ps->warm_start_reg_changed = TRUE; // Signaling that a register for this port has changed.
#endif
            VTSS_RC(csr_wrm(vtss_state, port_no, io->mmd, io->is32, io->addr, value, mask));
        }
    } else {
        /* Normal write */
        VTSS_RC(csr_wrm(vtss_state, port_no, io->mmd, io->is32, io->addr, value, mask));
    }
    return VTSS_RC_OK;
}

vtss_rc _csr_warm_wr(vtss_state_t *vtss_state, vtss_port_no_t port_no, ioreg_blk *io, u32 value, const char *function, const u16 line)
{
    return _csr_warm_wrm(vtss_state, port_no, io, value, 0xFFFFFFFF, 0xFFFFFFFF, function, line);
}

vtss_rc _csr_rd_64(vtss_state_t *vtss_state, vtss_port_no_t port_no, ioreg_blk *reg_low, ioreg_blk *reg_up, u64 *value)
{
    /*Assuming both the registers belong to same device and size */
    return csr_rd_64(vtss_state, port_no, reg_low->mmd, reg_low->is32, reg_low->addr, reg_up->addr, value);
}

vtss_rc _csr_wr_64(vtss_state_t *vtss_state, vtss_port_no_t port_no, ioreg_blk *reg_low, ioreg_blk *reg_up, u64 value)
{
    return csr_wr_64(vtss_state, port_no, reg_low->mmd, reg_low->is32, reg_low->addr, reg_up->addr, value);
}
#endif
vtss_rc csr_rd_64(vtss_state_t *vtss_state, vtss_port_no_t port_no, u16 mmd, BOOL is32, u32 addr1, u32 addr2, u64 *value)
{
    BOOL                       clause45 = FALSE;
    u32                        reg_addr = 0;
    u16                        reg_value[4];
    u32                        offset, target, base_addr;
    BOOL                       use_base_port = TRUE;
    u32                        value_low, value_hi;
#ifdef VTSS_CHIP_CU_PHY
    u64                        value_64;
#endif /* VTSS_CHIP_CU_PHY */
#ifdef VTSS_CHIP_10G_PHY
    vtss_phy_10g_family_t      phy_family = VTSS_PHY_FAMILY_10G_NONE;

    phy_family = vtss_state->phy_10g_state[port_no].family;
#endif /* VTSS_CHIP_10G_PHY */
    VTSS_RC(phy_type_get(vtss_state, port_no, &clause45));
    if (!is32) {
        VTSS_D("Addresses passed must be of size 32 bit");
        return VTSS_RC_ERROR;
    }
    /* Use the SPI access method if available */
    if (((vtss_state->init_conf.spi_64bit_read_write != NULL)
        || (vtss_state->init_conf.spi_32bit_read_write != NULL)
        || (vtss_state->init_conf.spi_read_write != NULL))
#ifdef VTSS_CHIP_10G_PHY
        && (phy_family == VTSS_PHY_FAMILY_VENICE || phy_family == VTSS_PHY_FAMILY_MALIBU)
#endif /* VTSS_CHIP_10G_PHY */
       ) {
        if (addr1 + 1 != addr2) {
            VTSS_D("Only consecutive addresses are allowed for 64-bit read");
            return VTSS_RC_ERROR;
        }
        if (clause45) {
#ifdef VTSS_CHIP_10G_PHY
            if (vtss_state->init_conf.spi_64bit_read_write != NULL) {
                VTSS_RC(vtss_state->init_conf.spi_64bit_read_write(vtss_state, port_no, TRUE, (u8)mmd, (u16)addr1, value));
                return VTSS_RC_OK;
            } else if (vtss_state->init_conf.spi_32bit_read_write != NULL) {
                VTSS_RC(vtss_state->init_conf.spi_32bit_read_write(vtss_state, port_no, TRUE, (u8)mmd, (u16)addr1, &value_low));
                VTSS_RC(vtss_state->init_conf.spi_32bit_read_write(vtss_state, port_no, TRUE, (u8)mmd, (u16)addr2, &value_hi));
            } else {
                VTSS_RC(vtss_phy_10g_spi_read_write(vtss_state, port_no, TRUE, (u8)mmd, (u16)addr1, &value_low));
                VTSS_RC(vtss_phy_10g_spi_read_write(vtss_state, port_no, TRUE, (u8)mmd, (u16)addr2, &value_hi));
            }
#endif /* VTSS_CHIP_10G_PHY */
        } else {
#ifdef VTSS_CHIP_CU_PHY
            VTSS_RC(get_base_adr(vtss_state, port_no, mmd, addr1, &base_addr, &target, &offset, &use_base_port));
            if (vtss_state->init_conf.spi_64bit_read_write != NULL) {
                VTSS_RC(vtss_state->init_conf.spi_64bit_read_write(vtss_state, port_no, 1, (u8)target, (u16)offset, value));
                return VTSS_RC_OK;
            } else {
                if (vtss_state->init_conf.spi_32bit_read_write != NULL) {
                    VTSS_RC(vtss_state->init_conf.spi_32bit_read_write(vtss_state, port_no, 1, (u8)target, (u16)offset, &value_low));
                } else {
                    VTSS_RC(vtss_phy_1g_spi_read_write(vtss_state, port_no, 0, 1, (u8)target, (u16)offset, &value_low));
                }
                VTSS_RC(get_base_adr(vtss_state, port_no, mmd, addr2, &base_addr, &target, &offset, &use_base_port));
                if (vtss_state->init_conf.spi_32bit_read_write != NULL) {
                    VTSS_RC(vtss_state->init_conf.spi_32bit_read_write(vtss_state, port_no, 1, (u8)target, (u16)offset, &value_hi));
                } else {
                    VTSS_RC(vtss_phy_1g_spi_read_write(vtss_state, port_no, 0, 1, (u8)target, (u16)offset, &value_hi));
                }
            }
#endif /* VTSS_CHIP_CU_PHY */
        }
        *value = value_hi;
        *value = (*value << 32) + value_low;
        return VTSS_RC_OK;
    }
    if (clause45) {
        vtss_mmd_read_inc_t mmd_read_inc_func = vtss_state->init_conf.mmd_read_inc;
        vtss_port_no_t p = (mmd == 0x1e) ? PHY_BASE_PORT(port_no) : port_no;
        VTSS_RC(get_base_adr(vtss_state, port_no, mmd, addr1, &base_addr, &target, &offset, &use_base_port));
        reg_addr = base_addr | (offset << 1);
        if (!use_base_port) {
            p = port_no;
        }
        VTSS_RC(mmd_read_inc_func(vtss_state, p, mmd, reg_addr, reg_value, 4));
        value_low = reg_value[0] + (((u32)reg_value[1]) << 16);
        value_hi = reg_value[2] + (((u32)reg_value[3]) << 16);
        VTSS_D("Port:%d, MMD:0x%02X, RegAddr:0x%04X, val:0x%08X_%08X", port_no, mmd, addr1, value_hi, value_low);
    } else {
#ifdef VTSS_CHIP_CU_PHY
        VTSS_RC(get_base_adr(vtss_state, port_no, mmd, addr1, &base_addr, &target, &offset, &use_base_port));
        VTSS_RC(vtss_phy_macsec_csr_rd_64_private(vtss_state, port_no, target, offset, &value_64));
        *value = value_64;
        return VTSS_RC_OK;
#endif /* VTSS_CHIP_CU_PHY */
    }
    *value = value_hi;
    *value = (*value << 32) + value_low;

    return VTSS_RC_OK;
}

vtss_rc csr_wr_64(vtss_state_t *vtss_state, vtss_port_no_t port_no, u16 mmd, BOOL is32, u32 addr1, u32 addr2, u64 value)
{
    BOOL                       clause45 = FALSE;
    u32                        reg_addr = 0;
    u16                        reg_val;
    u32                        offset, target, base_addr;
    BOOL                       use_base_port = TRUE;
    u32                        value_low, value_hi;
#ifdef VTSS_CHIP_10G_PHY
    vtss_phy_10g_family_t      phy_family = VTSS_PHY_FAMILY_10G_NONE;

    phy_family = vtss_state->phy_10g_state[port_no].family;
#endif /* VTSS_CHIP_10G_PHY */
    VTSS_RC(phy_type_get(vtss_state, port_no, &clause45));
    if (!is32) {
        VTSS_D("Addresses passed must be of size 32 bit");
        return VTSS_RC_ERROR;
    }
    value_low = value & 0xffffffff;
    value_hi = value >> 32;
    /* Use the SPI access method if available */
    if (((vtss_state->init_conf.spi_64bit_read_write != NULL)
        || (vtss_state->init_conf.spi_32bit_read_write != NULL)
        || (vtss_state->init_conf.spi_read_write != NULL))
#ifdef VTSS_CHIP_10G_PHY
        && ((phy_family == VTSS_PHY_FAMILY_VENICE) || (phy_family == VTSS_PHY_FAMILY_MALIBU))
#endif /* VTSS_CHIP_10G_PHY */
       ) {
        if (addr1 + 1 != addr2) {
            VTSS_D("Only consecutive addresses are allowed for 64-bit write");
            return VTSS_RC_ERROR;
        }
        if (clause45) {
#ifdef VTSS_CHIP_10G_PHY
            if (vtss_state->init_conf.spi_64bit_read_write != NULL) {
                VTSS_RC(vtss_state->init_conf.spi_64bit_read_write(vtss_state, port_no, FALSE, (u8)mmd, (u16)addr1, &value));
            } else if (vtss_state->init_conf.spi_32bit_read_write != NULL) {
                VTSS_RC(vtss_state->init_conf.spi_32bit_read_write(vtss_state, port_no, FALSE, (u8)mmd, (u16)addr1, &value_low));
                VTSS_RC(vtss_state->init_conf.spi_32bit_read_write(vtss_state, port_no, FALSE, (u8)mmd, (u16)addr2, &value_hi));
            } else {
                VTSS_RC(vtss_phy_10g_spi_read_write(vtss_state, port_no, FALSE, (u8)mmd, (u16)addr1, &value_low));
                VTSS_RC(vtss_phy_10g_spi_read_write(vtss_state, port_no, FALSE, (u8)mmd, (u16)addr2, &value_hi));
            }
#endif /* VTSS_CHIP_10G_PHY */
        } else {
#ifdef VTSS_CHIP_CU_PHY
            VTSS_RC(get_base_adr(vtss_state, port_no, mmd, addr1, &base_addr, &target, &offset, &use_base_port));
            if (vtss_state->init_conf.spi_64bit_read_write != NULL) {
                VTSS_RC(vtss_state->init_conf.spi_64bit_read_write(vtss_state, port_no, FALSE, (u8)target, (u16)offset, &value));
            } else {
                if (vtss_state->init_conf.spi_32bit_read_write != NULL) {
                    VTSS_RC(vtss_state->init_conf.spi_32bit_read_write(vtss_state, port_no, FALSE, (u8)target, (u16)offset, &value_low));
                } else {
                    VTSS_RC(vtss_phy_1g_spi_read_write(vtss_state, port_no, 0, FALSE, (u8)target, (u16)offset, &value_low));
                }
                VTSS_RC(get_base_adr(vtss_state, port_no, mmd, addr2, &base_addr, &target, &offset, &use_base_port));
                if (vtss_state->init_conf.spi_32bit_read_write != NULL) {
                    VTSS_RC(vtss_state->init_conf.spi_32bit_read_write(vtss_state, port_no, FALSE, (u8)target, (u16)offset, &value_hi));
                } else {
                    VTSS_RC(vtss_phy_1g_spi_read_write(vtss_state, port_no, 0, FALSE, (u8)target, (u16)offset, &value_hi));
                }
            }
#endif /* VTSS_CHIP_CU_PHY */
        }
        return VTSS_RC_OK;
    }

    if (clause45) {
        vtss_mmd_write_t  mmd_write_func = vtss_state->init_conf.mmd_write;
        vtss_port_no_t p = (mmd == 0x1e) ? PHY_BASE_PORT(port_no) : port_no;
        VTSS_RC(get_base_adr(vtss_state, port_no, mmd, addr1, &base_addr, &target, &offset, &use_base_port));
        reg_addr = base_addr | (offset << 1);
        if (!use_base_port) {
            p = port_no;
        }
        /* Start writing from the Lower 2 Bytes */
        reg_val = value_low & 0xffff;
        VTSS_RC(mmd_write_func(vtss_state, p, mmd, reg_addr, reg_val));
        reg_val = value_low >> 16;
        VTSS_RC(mmd_write_func(vtss_state, p, mmd, (reg_addr | 1), reg_val));
        VTSS_RC(get_base_adr(vtss_state, port_no, mmd, addr2, &base_addr, &target, &offset, &use_base_port));
        reg_addr = base_addr | (offset << 1);
        reg_val = value_hi & 0xffff;
        VTSS_RC(mmd_write_func(vtss_state, p, mmd, reg_addr, reg_val));
        reg_val = value_hi >> 16;
        VTSS_RC(mmd_write_func(vtss_state, p, mmd, (reg_addr | 1), reg_val));
    } else {
        // 1G PHY access
#ifdef VTSS_CHIP_CU_PHY
        VTSS_RC(get_base_adr(vtss_state, port_no, mmd, addr1, &base_addr, &target, &offset, &use_base_port));
        VTSS_RC(vtss_phy_macsec_csr_wr_private(vtss_state, port_no, target, offset, value_low));
        VTSS_RC(get_base_adr(vtss_state, port_no, mmd, addr2, &base_addr, &target, &offset, &use_base_port));
        VTSS_RC(vtss_phy_macsec_csr_wr_private(vtss_state, port_no, target, offset, value_hi));
#endif /* VTSS_CHIP_CU_PHY */
    }

    return VTSS_RC_OK;
}
#if defined(VTSS_FEATURE_MACSEC)
vtss_rc vtss_fc_buffer_frm_gap_set(vtss_state_t *vtss_state, vtss_port_no_t port_no, u8 frm_gap)
{
    CSR_WARM_WRM(port_no, VTSS_FC_BUFFER_CONFIG_TX_FRM_GAP_COMP,
                 VTSS_F_FC_BUFFER_CONFIG_TX_FRM_GAP_COMP_TX_FRM_GAP_COMP(frm_gap),
                 VTSS_M_FC_BUFFER_CONFIG_TX_FRM_GAP_COMP_TX_FRM_GAP_COMP);
    return VTSS_RC_OK;
}
#endif
vtss_rc phy_10g_macsec_clk_en(vtss_state_t *vtss_state, vtss_port_no_t port_no, BOOL enable)
{
#ifdef VTSS_CHIP_10G_PHY
    switch (vtss_state->phy_10g_state[port_no].type) {
        case VTSS_PHY_TYPE_8254:
        case VTSS_PHY_TYPE_8258:
             CSR_WARM_WRM(port_no, VTSS_LINE_PMA_MAC_ENABLE_MAC_ENA,
                          (enable ?VTSS_F_LINE_PMA_MAC_ENABLE_MAC_ENA_MACSEC_CLK_ENA : 0),
                          VTSS_F_LINE_PMA_MAC_ENABLE_MAC_ENA_MACSEC_CLK_ENA);
             break;
        case VTSS_PHY_TYPE_8489:
        case VTSS_PHY_TYPE_8490:
        case VTSS_PHY_TYPE_8491:
        default :
             CSR_WARM_WRM(port_no, VTSS_VENICE_DEV1_MAC_ENABLE_MAC_ENA,
                          (enable ?VTSS_F_VENICE_DEV1_MAC_ENABLE_MAC_ENA_MACSEC_CLK_ENA : 0),
                          VTSS_F_VENICE_DEV1_MAC_ENABLE_MAC_ENA_MACSEC_CLK_ENA);
             break;
    }
#endif
    return VTSS_RC_OK;
}


#if defined(VTSS_CHIP_10G_PHY) || defined(VTSS_FEATURE_MACSEC)
vtss_rc phy_10g_mac_tx_rx_ena(vtss_state_t *vtss_state,
                              vtss_port_no_t port_no,
                              BOOL enable)
{
    VTSS_I("phy_10g_mac_tx_rx_ena called with enable:%s\n",enable?"TRUE":"FALSE");
    /* Disable MAC Tx and Rx Traffic */
    if (!enable) {
        /* Disable Ingress Line MAC RX_ENA */
        CSR_WARM_WRM(port_no, VTSS_LINE_MAC_CONFIG_MAC_ENA_CFG,
                     0,
                     VTSS_F_LINE_MAC_CONFIG_MAC_ENA_CFG_RX_ENA);
        /* Disable Egress Host MAC RX_ENA */
        CSR_WARM_WRM(port_no, VTSS_HOST_MAC_CONFIG_MAC_ENA_CFG,
                     0,
                     VTSS_F_HOST_MAC_CONFIG_MAC_ENA_CFG_RX_ENA);
        /* Disable FC Buffer RX_ENA */
        CSR_WARM_WRM(port_no, VTSS_FC_BUFFER_CONFIG_FC_ENA_CFG,
                     0,
                     VTSS_F_FC_BUFFER_CONFIG_FC_ENA_CFG_RX_ENA);
        /* Disable Ingress Host MAC TX_ENA */
        CSR_WARM_WRM(port_no, VTSS_HOST_MAC_CONFIG_MAC_ENA_CFG,
                     0,
                     VTSS_F_HOST_MAC_CONFIG_MAC_ENA_CFG_TX_ENA);
        /* Disable FC Buffer TX_ENA */
        CSR_WARM_WRM(port_no, VTSS_FC_BUFFER_CONFIG_FC_ENA_CFG,
                     0,
                     VTSS_F_FC_BUFFER_CONFIG_FC_ENA_CFG_TX_ENA);
        /* Disable Egress Line MAC TX_ENA */
        CSR_WARM_WRM(port_no, VTSS_LINE_MAC_CONFIG_MAC_ENA_CFG,
                     0,
                     VTSS_F_LINE_MAC_CONFIG_MAC_ENA_CFG_TX_ENA);
    }

    /* Eanble MAC Tx and Rx Traffic */
    if (enable) {
        /* Enable Ingress Host MAC TX_ENA */
        CSR_WARM_WRM(port_no, VTSS_HOST_MAC_CONFIG_MAC_ENA_CFG,
                     VTSS_F_HOST_MAC_CONFIG_MAC_ENA_CFG_TX_ENA,
                     VTSS_F_HOST_MAC_CONFIG_MAC_ENA_CFG_TX_ENA);
        /* Enable FC Buffer RX_ENA */
        CSR_WARM_WRM(port_no, VTSS_FC_BUFFER_CONFIG_FC_ENA_CFG,
                     VTSS_F_FC_BUFFER_CONFIG_FC_ENA_CFG_RX_ENA,
                     VTSS_F_FC_BUFFER_CONFIG_FC_ENA_CFG_RX_ENA);
        /* Enable Egress Line MAC TX_ENA */
        CSR_WARM_WRM(port_no, VTSS_LINE_MAC_CONFIG_MAC_ENA_CFG,
                     VTSS_F_LINE_MAC_CONFIG_MAC_ENA_CFG_TX_ENA,
                     VTSS_F_LINE_MAC_CONFIG_MAC_ENA_CFG_TX_ENA);
        /* Enable FC Buffer TX_ENA */
        CSR_WARM_WRM(port_no, VTSS_FC_BUFFER_CONFIG_FC_ENA_CFG,
                     VTSS_F_FC_BUFFER_CONFIG_FC_ENA_CFG_TX_ENA,
                     VTSS_F_FC_BUFFER_CONFIG_FC_ENA_CFG_TX_ENA);
        /* Enable Ingress Line MAC RX_ENA */
        CSR_WARM_WRM(port_no, VTSS_LINE_MAC_CONFIG_MAC_ENA_CFG,
                     VTSS_F_LINE_MAC_CONFIG_MAC_ENA_CFG_RX_ENA,
                     VTSS_F_LINE_MAC_CONFIG_MAC_ENA_CFG_RX_ENA);
        /* Enable Egress Host MAC RX_ENA */
        CSR_WARM_WRM(port_no, VTSS_HOST_MAC_CONFIG_MAC_ENA_CFG,
                     VTSS_F_HOST_MAC_CONFIG_MAC_ENA_CFG_RX_ENA,
                     VTSS_F_HOST_MAC_CONFIG_MAC_ENA_CFG_RX_ENA);
    }

    return VTSS_RC_OK;
}
#endif

vtss_rc phy_mac_fc_buffer_reset(vtss_state_t *vtss_state,
                                vtss_port_no_t port_no)
{
    BOOL phy10g;

    /* In case of warmstart, there is no need to reset fc butter. Return silently.*/
    if (vtss_state->sync_calling_private) {
        return VTSS_RC_OK;
    }

    VTSS_RC(phy_type_get(vtss_state, port_no, &phy10g));
#if defined(VTSS_CHIP_10G_PHY)
    if (phy10g &&
        (vtss_state->phy_10g_state[port_no].mode.oper_mode == VTSS_PHY_WAN_MODE)) {
#if defined(VTSS_FEATURE_MACSEC)
        if (vtss_state->macsec_conf[port_no].glb.init.enable == TRUE) {
            CSR_WARM_WRM(port_no, VTSS_FC_BUFFER_CONFIG_FC_ENA_CFG, 0,
                         VTSS_F_FC_BUFFER_CONFIG_FC_ENA_CFG_RX_ENA);

            CSR_WARM_WRM(port_no, VTSS_FC_BUFFER_CONFIG_FC_ENA_CFG,
                         VTSS_F_FC_BUFFER_CONFIG_FC_ENA_CFG_RX_ENA,
                         VTSS_F_FC_BUFFER_CONFIG_FC_ENA_CFG_RX_ENA);
            VTSS_I("FC Buffer Rx Enable reset");
        }
#endif
    }
#endif

    return VTSS_RC_OK;
}

/* 'enable' is used for Mac enable or disable.
   'macsec_enable' signifies whether macsec was or is enabled or not.
   We disable mac before disabling macsec. So, 'vtss_state' does not 
   accurately reflect future disabling of macsec by the time of this call.  */
vtss_rc phy_10g_mac_conf(vtss_state_t *vtss_state, vtss_port_no_t port_no, BOOL enable, BOOL macsec_ena)
{
#if defined(VTSS_CHIP_10G_PHY) || defined(VTSS_FEATURE_MACSEC)
    BOOL phy10g;
    u32 ch_id = 0, rx_rd_thres = 5, disable_dic = 0, tx_rd_thres = 4;
    BOOL  dis_validate_length = FALSE;
    u16 mac_block_mtu = 10240;  /* Default MTU value */

    BOOL pkt_mode_enable = FALSE;

#if defined(VTSS_FEATURE_MACSEC)
    u32 value;

    if (enable && vtss_state->macsec_conf[port_no].glb.init.enable) {
        disable_dic = 0;
    } else if (enable) {
        disable_dic = 1;
    }

    if ((vtss_state->macsec_conf[port_no].glb.init.enable) && (vtss_state->macsec_conf[port_no].glb.mac_block_mtu != 0)) {
       /* MAC block MTU must not have '0' value, configure to default if initialised with '0' */
        mac_block_mtu = vtss_state->macsec_conf[port_no].glb.mac_block_mtu;
    }
#endif
    if (enable || macsec_ena) {
        pkt_mode_enable = TRUE;
    } else if (!enable) {
        pkt_mode_enable = FALSE;
    }

    /* In case of warmstart, there is no need to toggle macs. Return silently.*/
    if ((vtss_state->warm_start_cur) ||
       (vtss_state->sync_calling_private)) {
        return VTSS_RC_OK;
    }

    VTSS_RC(phy_type_get(vtss_state, port_no, &phy10g));
#ifdef VTSS_CHIP_10G_PHY
    if (phy10g) {
        if (vtss_state->phy_10g_state[port_no].mode.oper_mode == VTSS_PHY_WAN_MODE) {
            if (vtss_state->phy_10g_state[port_no].family == VTSS_PHY_FAMILY_MALIBU) {
                rx_rd_thres = 128; // For FC buffer
            } else {
                rx_rd_thres = 127;
            }
        }
        if (vtss_state->phy_10g_state[port_no].mode.oper_mode == VTSS_PHY_LAN_MODE) {
            if (vtss_state->phy_10g_state[port_no].family == VTSS_PHY_FAMILY_MALIBU) {
                tx_rd_thres = 5;
                rx_rd_thres = 6;
            }
        }
    }
#endif // VTSS_CHIP_10G_PHY

    if (phy10g) {
#ifdef VTSS_CHIP_10G_PHY
        ch_id =  vtss_state->phy_10g_state[port_no].channel_id;
#endif
    } else {
#if defined(VTSS_CHIP_CU_PHY)
        ch_id =  vtss_state->phy_state[port_no].type.channel_id;
#endif
    }

    if (phy10g) {
        /* Enable MAC in the datapath */
#if defined(VTSS_FEATURE_MACSEC)
        if (enable) {
            CSR_WARM_WRM(port_no, VTSS_VENICE_DEV1_MAC_ENABLE_MAC_ENA,
                         VTSS_F_VENICE_DEV1_MAC_ENABLE_MAC_ENA_MAC_ENA ,
                         VTSS_F_VENICE_DEV1_MAC_ENABLE_MAC_ENA_MAC_ENA);
        } else {
            /* Do not disable mac when macsec is enabled */
            CSR_RD(port_no, VTSS_VENICE_DEV1_MAC_ENABLE_MAC_ENA, &value);
            if (!(value & VTSS_F_LINE_PMA_MAC_ENABLE_MAC_ENA_MACSEC_CLK_ENA)) {
                CSR_WARM_WRM(port_no, VTSS_VENICE_DEV1_MAC_ENABLE_MAC_ENA, 0,
                             VTSS_F_VENICE_DEV1_MAC_ENABLE_MAC_ENA_MAC_ENA);
            }
        }
#else
        CSR_WARM_WRM(port_no, VTSS_VENICE_DEV1_MAC_ENABLE_MAC_ENA,
                     (enable ? VTSS_F_VENICE_DEV1_MAC_ENABLE_MAC_ENA_MAC_ENA : 0),
                     VTSS_F_VENICE_DEV1_MAC_ENABLE_MAC_ENA_MAC_ENA);
#endif
    }

    // Stop the traffic irrespective of enable / disable
    CSR_WRM(port_no, VTSS_LINE_MAC_CONFIG_MAC_ENA_CFG,
            0,
            VTSS_F_LINE_MAC_CONFIG_MAC_ENA_CFG_RX_ENA);
    CSR_WRM(port_no, VTSS_HOST_MAC_CONFIG_MAC_ENA_CFG,
            0,
            VTSS_F_HOST_MAC_CONFIG_MAC_ENA_CFG_RX_ENA);

    // Wait for 2 ms
    VTSS_MSLEEP(2);

    CSR_COLD_WRM(port_no, VTSS_LINE_MAC_CONFIG_MAC_MODE_CFG, disable_dic,
                 VTSS_F_LINE_MAC_CONFIG_MAC_MODE_CFG_DISABLE_DIC);

    CSR_COLD_WRM(port_no, VTSS_LINE_MAC_CONFIG_MAC_MODE_CFG,
                 VTSS_BIT(16), VTSS_BIT(16));

    CSR_COLD_WRM(port_no, VTSS_LINE_MAC_CONFIG_MAC_MODE_CFG,
                 0, VTSS_BIT(16));


    /* Set the 1588 block into packet mode */
    if ((phy10g && (ch_id == 0)) || (!phy10g && (ch_id < 2))) {
        CSR_COLD_WRM(port_no, VTSS_PTP_0_IP_1588_TOP_CFG_STAT_MODE_CTL,
                     (pkt_mode_enable ? VTSS_F_PTP_0_IP_1588_TOP_CFG_STAT_MODE_CTL_PROTOCOL_MODE(4) : 0),
                     VTSS_M_PTP_0_IP_1588_TOP_CFG_STAT_MODE_CTL_PROTOCOL_MODE); // 10G ch-0, 1G ch-0/1
    } else if ((phy10g && (ch_id == 1)) || (!phy10g && (ch_id < 4))) {
        CSR_COLD_WRM(port_no, VTSS_PTP_1_IP_1588_TOP_CFG_STAT_MODE_CTL,
                     (pkt_mode_enable ? VTSS_F_PTP_1_IP_1588_TOP_CFG_STAT_MODE_CTL_PROTOCOL_MODE(4) : 0),
                     VTSS_M_PTP_1_IP_1588_TOP_CFG_STAT_MODE_CTL_PROTOCOL_MODE); // 10G ch-0, 1G ch-0/1
    }
    if (phy10g && (ch_id == 2)) {
        CSR_COLD_WRM(port_no, VTSS_PTP_2_IP_1588_TOP_CFG_STAT_MODE_CTL,
                     (pkt_mode_enable ? VTSS_F_PTP_2_IP_1588_TOP_CFG_STAT_MODE_CTL_PROTOCOL_MODE(4) : 0),
                     VTSS_M_PTP_2_IP_1588_TOP_CFG_STAT_MODE_CTL_PROTOCOL_MODE); // 10G ch-2 for malibu
    } else if (phy10g && (ch_id == 3)) {
        CSR_COLD_WRM(port_no, VTSS_PTP_3_IP_1588_TOP_CFG_STAT_MODE_CTL,
                     (pkt_mode_enable ? VTSS_F_PTP_3_IP_1588_TOP_CFG_STAT_MODE_CTL_PROTOCOL_MODE(4) : 0),
                     VTSS_M_PTP_3_IP_1588_TOP_CFG_STAT_MODE_CTL_PROTOCOL_MODE); // 10G ch-3 for malibu
    }
    if (enable) {
        // Host MAC
        CSR_WARM_WRM(port_no, VTSS_HOST_MAC_PAUSE_CFG_PAUSE_TX_FRAME_CONTROL,
                     VTSS_F_HOST_MAC_PAUSE_CFG_PAUSE_TX_FRAME_CONTROL_MAC_TX_PAUSE_MODE(2) |
                     VTSS_F_HOST_MAC_PAUSE_CFG_PAUSE_TX_FRAME_CONTROL_MAC_TX_PAUSE_VALUE(0xffff),
                     VTSS_M_HOST_MAC_PAUSE_CFG_PAUSE_TX_FRAME_CONTROL_MAC_TX_PAUSE_MODE |
                     VTSS_M_HOST_MAC_PAUSE_CFG_PAUSE_TX_FRAME_CONTROL_MAC_TX_PAUSE_VALUE);

        CSR_WARM_WRM(port_no, VTSS_HOST_MAC_PAUSE_CFG_PAUSE_TX_FRAME_CONTROL_2,
                     VTSS_F_HOST_MAC_PAUSE_CFG_PAUSE_TX_FRAME_CONTROL_2_MAC_TX_PAUSE_INTERVAL(0xffff),
                     VTSS_M_HOST_MAC_PAUSE_CFG_PAUSE_TX_FRAME_CONTROL_2_MAC_TX_PAUSE_INTERVAL);

        CSR_WARM_WRM(port_no, VTSS_HOST_MAC_PAUSE_CFG_PAUSE_RX_FRAME_CONTROL,
                     VTSS_F_HOST_MAC_PAUSE_CFG_PAUSE_RX_FRAME_CONTROL_MAC_RX_PAUSE_TIMER_ENA |
                     VTSS_F_HOST_MAC_PAUSE_CFG_PAUSE_RX_FRAME_CONTROL_MAC_RX_PAUSE_FRAME_DROP_ENA,
                     VTSS_F_HOST_MAC_PAUSE_CFG_PAUSE_RX_FRAME_CONTROL_MAC_RX_PAUSE_TIMER_ENA |
                     VTSS_F_HOST_MAC_PAUSE_CFG_PAUSE_RX_FRAME_CONTROL_MAC_RX_PAUSE_FRAME_DROP_ENA);

        CSR_WARM_WR(port_no, VTSS_HOST_MAC_CONFIG_MAC_PKTINF_CFG,
                    VTSS_F_HOST_MAC_CONFIG_MAC_PKTINF_CFG_STRIP_FCS_ENA |
                    VTSS_F_HOST_MAC_CONFIG_MAC_PKTINF_CFG_INSERT_FCS_ENA |
                    (phy10g ? 0 : VTSS_F_HOST_MAC_CONFIG_MAC_PKTINF_CFG_LPI_RELAY_ENA) |
                    VTSS_F_HOST_MAC_CONFIG_MAC_PKTINF_CFG_STRIP_PREAMBLE_ENA |
                    VTSS_F_HOST_MAC_CONFIG_MAC_PKTINF_CFG_INSERT_PREAMBLE_ENA |
                    (phy10g ? VTSS_F_HOST_MAC_CONFIG_MAC_PKTINF_CFG_ENABLE_TX_PADDING : VTSS_BIT(28)));

        CSR_WARM_WRM(port_no, VTSS_HOST_MAC_CONFIG_MAC_MODE_CFG, 0,
                     VTSS_F_HOST_MAC_CONFIG_MAC_MODE_CFG_DISABLE_DIC);

        CSR_COLD_WRM(port_no, VTSS_HOST_MAC_CONFIG_MAC_MAXLEN_CFG,
                     VTSS_F_HOST_MAC_CONFIG_MAC_MAXLEN_CFG_MAX_LEN(mac_block_mtu),
                     VTSS_M_HOST_MAC_CONFIG_MAC_MAXLEN_CFG_MAX_LEN);

#if defined(VTSS_FEATURE_MACSEC)
        dis_validate_length = vtss_state->macsec_conf[port_no].glb.init.mac_conf.hmac.dis_length_validate;
#endif
        CSR_WARM_WR(port_no, VTSS_HOST_MAC_CONFIG_MAC_ADV_CHK_CFG,
                    (phy10g ? VTSS_F_HOST_MAC_CONFIG_MAC_ADV_CHK_CFG_EXT_EOP_CHK_ENA : 0) |
                    VTSS_F_HOST_MAC_CONFIG_MAC_ADV_CHK_CFG_SFD_CHK_ENA |
                    VTSS_F_HOST_MAC_CONFIG_MAC_ADV_CHK_CFG_PRM_CHK_ENA |
                    (dis_validate_length ? 0 : VTSS_F_HOST_MAC_CONFIG_MAC_ADV_CHK_CFG_OOR_ERR_ENA) |
                    (dis_validate_length ? 0 : VTSS_F_HOST_MAC_CONFIG_MAC_ADV_CHK_CFG_INR_ERR_ENA));

        CSR_WARM_WRM(port_no, VTSS_HOST_MAC_CONFIG_MAC_LFS_CFG,
                     0,
                     VTSS_F_HOST_MAC_CONFIG_MAC_LFS_CFG_LFS_MODE_ENA);

        // Line MAC
        CSR_WARM_WRM(port_no, VTSS_LINE_MAC_PAUSE_CFG_PAUSE_TX_FRAME_CONTROL,
                     VTSS_F_LINE_MAC_PAUSE_CFG_PAUSE_TX_FRAME_CONTROL_MAC_TX_USE_PAUSE_STALL_ENA |
                     VTSS_F_LINE_MAC_PAUSE_CFG_PAUSE_TX_FRAME_CONTROL_MAC_TX_PAUSE_VALUE(0xffff) |
                     VTSS_F_LINE_MAC_PAUSE_CFG_PAUSE_TX_FRAME_CONTROL_MAC_TX_PAUSE_MODE(2),
                     VTSS_F_LINE_MAC_PAUSE_CFG_PAUSE_TX_FRAME_CONTROL_MAC_TX_USE_PAUSE_STALL_ENA |
                     VTSS_M_LINE_MAC_PAUSE_CFG_PAUSE_TX_FRAME_CONTROL_MAC_TX_PAUSE_VALUE |
                     VTSS_M_LINE_MAC_PAUSE_CFG_PAUSE_TX_FRAME_CONTROL_MAC_TX_PAUSE_MODE);

        CSR_WARM_WRM(port_no, VTSS_LINE_MAC_PAUSE_CFG_PAUSE_TX_FRAME_CONTROL_2,
                     VTSS_F_LINE_MAC_PAUSE_CFG_PAUSE_TX_FRAME_CONTROL_2_MAC_TX_PAUSE_INTERVAL(0xffff),
                     VTSS_M_LINE_MAC_PAUSE_CFG_PAUSE_TX_FRAME_CONTROL_2_MAC_TX_PAUSE_INTERVAL);

        CSR_WARM_WRM(port_no, VTSS_LINE_MAC_PAUSE_CFG_PAUSE_RX_FRAME_CONTROL,
                     VTSS_F_LINE_MAC_PAUSE_CFG_PAUSE_RX_FRAME_CONTROL_MAC_TX_PAUSE_REACT_ENA |
                     VTSS_F_LINE_MAC_PAUSE_CFG_PAUSE_RX_FRAME_CONTROL_MAC_RX_PAUSE_FRAME_DROP_ENA |
                     VTSS_F_LINE_MAC_PAUSE_CFG_PAUSE_RX_FRAME_CONTROL_MAC_RX_PAUSE_MODE |
                     VTSS_F_LINE_MAC_PAUSE_CFG_PAUSE_RX_FRAME_CONTROL_MAC_RX_EARLY_PAUSE_DETECT_ENA,
                     VTSS_F_LINE_MAC_PAUSE_CFG_PAUSE_RX_FRAME_CONTROL_MAC_TX_PAUSE_REACT_ENA |
                     VTSS_F_LINE_MAC_PAUSE_CFG_PAUSE_RX_FRAME_CONTROL_MAC_RX_PAUSE_FRAME_DROP_ENA |
                     VTSS_F_LINE_MAC_PAUSE_CFG_PAUSE_RX_FRAME_CONTROL_MAC_RX_PAUSE_MODE |
                     VTSS_F_LINE_MAC_PAUSE_CFG_PAUSE_RX_FRAME_CONTROL_MAC_RX_EARLY_PAUSE_DETECT_ENA);

        CSR_WARM_WR(port_no, VTSS_LINE_MAC_CONFIG_MAC_PKTINF_CFG,
                    VTSS_F_LINE_MAC_CONFIG_MAC_PKTINF_CFG_STRIP_FCS_ENA |
                    VTSS_F_LINE_MAC_CONFIG_MAC_PKTINF_CFG_INSERT_FCS_ENA |
                    (phy10g ? 0 : VTSS_F_LINE_MAC_CONFIG_MAC_PKTINF_CFG_LPI_RELAY_ENA) |
                    (phy10g ? VTSS_F_LINE_MAC_CONFIG_MAC_PKTINF_CFG_LF_RELAY_ENA : 0) |
                    (phy10g ? VTSS_F_LINE_MAC_CONFIG_MAC_PKTINF_CFG_RF_RELAY_ENA : 0) |
                    VTSS_F_LINE_MAC_CONFIG_MAC_PKTINF_CFG_STRIP_PREAMBLE_ENA |
                    VTSS_F_LINE_MAC_CONFIG_MAC_PKTINF_CFG_INSERT_PREAMBLE_ENA);

        CSR_COLD_WRM(port_no, VTSS_LINE_MAC_CONFIG_MAC_MAXLEN_CFG,
                     VTSS_F_LINE_MAC_CONFIG_MAC_MAXLEN_CFG_MAX_LEN(mac_block_mtu),
                     VTSS_M_LINE_MAC_CONFIG_MAC_MAXLEN_CFG_MAX_LEN);

#if defined(VTSS_FEATURE_MACSEC)
        dis_validate_length = vtss_state->macsec_conf[port_no].glb.init.mac_conf.lmac.dis_length_validate;
#endif
        CSR_WARM_WR(port_no, VTSS_LINE_MAC_CONFIG_MAC_ADV_CHK_CFG,
                    (phy10g ? VTSS_F_LINE_MAC_CONFIG_MAC_ADV_CHK_CFG_EXT_EOP_CHK_ENA : 0) |
                    VTSS_F_LINE_MAC_CONFIG_MAC_ADV_CHK_CFG_SFD_CHK_ENA |
                    VTSS_F_LINE_MAC_CONFIG_MAC_ADV_CHK_CFG_PRM_CHK_ENA |
                    (dis_validate_length ? 0: VTSS_F_LINE_MAC_CONFIG_MAC_ADV_CHK_CFG_OOR_ERR_ENA) |
                    (dis_validate_length ? 0: VTSS_F_LINE_MAC_CONFIG_MAC_ADV_CHK_CFG_INR_ERR_ENA));

        /* Enabling LFS_MODE_ENA for Line MAC */
        CSR_WARM_WRM(port_no, VTSS_LINE_MAC_CONFIG_MAC_LFS_CFG,
                     phy10g ? 1 : 0,
                     VTSS_F_LINE_MAC_CONFIG_MAC_LFS_CFG_LFS_MODE_ENA);

        // FC buffer (Note: Setup of flow control is done as per UG1054 section 4.3.1.2)
        CSR_WARM_WR(port_no, VTSS_FC_BUFFER_CONFIG_FC_READ_THRESH_CFG,
                    VTSS_F_FC_BUFFER_CONFIG_FC_READ_THRESH_CFG_TX_READ_THRESH(tx_rd_thres) |
                    VTSS_F_FC_BUFFER_CONFIG_FC_READ_THRESH_CFG_RX_READ_THRESH(rx_rd_thres));

        CSR_WARM_WRM(port_no, VTSS_FC_BUFFER_CONFIG_FC_MODE_CFG,
                     VTSS_F_FC_BUFFER_CONFIG_FC_MODE_CFG_PAUSE_GEN_ENA |
                     VTSS_F_FC_BUFFER_CONFIG_FC_MODE_CFG_RX_PPM_RATE_ADAPT_ENA |
                     VTSS_F_FC_BUFFER_CONFIG_FC_MODE_CFG_TX_PPM_RATE_ADAPT_ENA,
                     VTSS_F_FC_BUFFER_CONFIG_FC_MODE_CFG_PAUSE_GEN_ENA |
                     VTSS_F_FC_BUFFER_CONFIG_FC_MODE_CFG_RX_PPM_RATE_ADAPT_ENA |
                     VTSS_F_FC_BUFFER_CONFIG_FC_MODE_CFG_TX_PPM_RATE_ADAPT_ENA);

#ifdef VTSS_CHIP_10G_PHY
        if (vtss_state->phy_10g_state[port_no].family == VTSS_PHY_FAMILY_MALIBU) {
            CSR_WARM_WR(port_no, VTSS_FC_BUFFER_CONFIG_PPM_RATE_ADAPT_THRESH_CFG,
                        VTSS_F_FC_BUFFER_CONFIG_PPM_RATE_ADAPT_THRESH_CFG_TX_PPM_RATE_ADAPT_THRESH(tx_rd_thres + 2) |
                        VTSS_F_FC_BUFFER_CONFIG_PPM_RATE_ADAPT_THRESH_CFG_RX_PPM_RATE_ADAPT_THRESH(rx_rd_thres + 2));
        } else {
#endif
            CSR_WARM_WR(port_no, VTSS_FC_BUFFER_CONFIG_PPM_RATE_ADAPT_THRESH_CFG,
                        VTSS_F_FC_BUFFER_CONFIG_PPM_RATE_ADAPT_THRESH_CFG_TX_PPM_RATE_ADAPT_THRESH(tx_rd_thres + 4) |
                        (phy10g ? VTSS_F_FC_BUFFER_CONFIG_PPM_RATE_ADAPT_THRESH_CFG_RX_PPM_RATE_ADAPT_THRESH(rx_rd_thres + 4) :
                         VTSS_ENCODE_BITFIELD(rx_rd_thres + 4, 16, 16)));
#ifdef VTSS_CHIP_10G_PHY
        }
#endif

        CSR_COLD_WRM(port_no, VTSS_FC_BUFFER_CONFIG_TX_DATA_QUEUE_CFG,
                     VTSS_F_FC_BUFFER_CONFIG_TX_DATA_QUEUE_CFG_TX_DATA_QUEUE_START(0) |
                     VTSS_F_FC_BUFFER_CONFIG_TX_DATA_QUEUE_CFG_TX_DATA_QUEUE_END(5119),
                     VTSS_M_FC_BUFFER_CONFIG_TX_DATA_QUEUE_CFG_TX_DATA_QUEUE_START |
                     VTSS_M_FC_BUFFER_CONFIG_TX_DATA_QUEUE_CFG_TX_DATA_QUEUE_END);

    }

    CSR_WARM_WRM(port_no, VTSS_FC_BUFFER_CONFIG_FC_ENA_CFG,
                 (enable ? (VTSS_F_FC_BUFFER_CONFIG_FC_ENA_CFG_TX_ENA |
                 VTSS_F_FC_BUFFER_CONFIG_FC_ENA_CFG_RX_ENA) : 0),
                 VTSS_F_FC_BUFFER_CONFIG_FC_ENA_CFG_TX_ENA |
                 VTSS_F_FC_BUFFER_CONFIG_FC_ENA_CFG_RX_ENA);

    /* Enable/disable MAC */
    if (enable) {
        // Enable clocks.
        CSR_WR(port_no, VTSS_LINE_MAC_CONFIG_MAC_ENA_CFG,
               VTSS_F_LINE_MAC_CONFIG_MAC_ENA_CFG_RX_CLK_ENA |
               VTSS_F_LINE_MAC_CONFIG_MAC_ENA_CFG_TX_CLK_ENA |
               VTSS_F_LINE_MAC_CONFIG_MAC_ENA_CFG_RX_SW_RST |
               VTSS_F_LINE_MAC_CONFIG_MAC_ENA_CFG_TX_SW_RST);

        CSR_WR(port_no, VTSS_HOST_MAC_CONFIG_MAC_ENA_CFG,
               VTSS_F_HOST_MAC_CONFIG_MAC_ENA_CFG_RX_CLK_ENA |
               VTSS_F_HOST_MAC_CONFIG_MAC_ENA_CFG_TX_CLK_ENA |
               VTSS_F_HOST_MAC_CONFIG_MAC_ENA_CFG_RX_SW_RST |
               VTSS_F_HOST_MAC_CONFIG_MAC_ENA_CFG_TX_SW_RST);

        // Take out of SW reset
        CSR_WR(port_no, VTSS_LINE_MAC_CONFIG_MAC_ENA_CFG,
               VTSS_F_LINE_MAC_CONFIG_MAC_ENA_CFG_RX_CLK_ENA |
               VTSS_F_LINE_MAC_CONFIG_MAC_ENA_CFG_TX_CLK_ENA); 

        CSR_WR(port_no, VTSS_HOST_MAC_CONFIG_MAC_ENA_CFG,
               VTSS_F_HOST_MAC_CONFIG_MAC_ENA_CFG_RX_CLK_ENA |
               VTSS_F_HOST_MAC_CONFIG_MAC_ENA_CFG_TX_CLK_ENA);

        // Enable TX
        CSR_WR(port_no, VTSS_LINE_MAC_CONFIG_MAC_ENA_CFG,
               VTSS_F_LINE_MAC_CONFIG_MAC_ENA_CFG_RX_CLK_ENA |
               VTSS_F_LINE_MAC_CONFIG_MAC_ENA_CFG_TX_CLK_ENA |
               VTSS_F_LINE_MAC_CONFIG_MAC_ENA_CFG_TX_ENA);

        CSR_WR(port_no, VTSS_HOST_MAC_CONFIG_MAC_ENA_CFG,
               VTSS_F_HOST_MAC_CONFIG_MAC_ENA_CFG_RX_CLK_ENA |
               VTSS_F_HOST_MAC_CONFIG_MAC_ENA_CFG_TX_CLK_ENA |
               VTSS_F_HOST_MAC_CONFIG_MAC_ENA_CFG_TX_ENA);

        // Enable RX
        CSR_WR(port_no, VTSS_LINE_MAC_CONFIG_MAC_ENA_CFG,
               VTSS_F_LINE_MAC_CONFIG_MAC_ENA_CFG_RX_CLK_ENA |
               VTSS_F_LINE_MAC_CONFIG_MAC_ENA_CFG_TX_CLK_ENA |
               VTSS_F_LINE_MAC_CONFIG_MAC_ENA_CFG_RX_ENA |
               VTSS_F_LINE_MAC_CONFIG_MAC_ENA_CFG_TX_ENA);

        CSR_WR(port_no, VTSS_HOST_MAC_CONFIG_MAC_ENA_CFG,
               VTSS_F_HOST_MAC_CONFIG_MAC_ENA_CFG_RX_CLK_ENA |
               VTSS_F_HOST_MAC_CONFIG_MAC_ENA_CFG_TX_CLK_ENA |
               VTSS_F_HOST_MAC_CONFIG_MAC_ENA_CFG_RX_ENA |
               VTSS_F_HOST_MAC_CONFIG_MAC_ENA_CFG_TX_ENA);

    } else {
        // Disable TX
        CSR_WR(port_no, VTSS_LINE_MAC_CONFIG_MAC_ENA_CFG,
               VTSS_F_LINE_MAC_CONFIG_MAC_ENA_CFG_RX_CLK_ENA |
               VTSS_F_LINE_MAC_CONFIG_MAC_ENA_CFG_TX_CLK_ENA); 

        CSR_WR(port_no, VTSS_HOST_MAC_CONFIG_MAC_ENA_CFG,
               VTSS_F_HOST_MAC_CONFIG_MAC_ENA_CFG_RX_CLK_ENA |
               VTSS_F_HOST_MAC_CONFIG_MAC_ENA_CFG_TX_CLK_ENA);

        // Assert resets
        CSR_WR(port_no, VTSS_LINE_MAC_CONFIG_MAC_ENA_CFG,
               VTSS_F_LINE_MAC_CONFIG_MAC_ENA_CFG_RX_CLK_ENA |
               VTSS_F_LINE_MAC_CONFIG_MAC_ENA_CFG_TX_CLK_ENA |
               VTSS_F_LINE_MAC_CONFIG_MAC_ENA_CFG_RX_SW_RST |
               VTSS_F_LINE_MAC_CONFIG_MAC_ENA_CFG_TX_SW_RST);

        CSR_WR(port_no, VTSS_HOST_MAC_CONFIG_MAC_ENA_CFG,
               VTSS_F_HOST_MAC_CONFIG_MAC_ENA_CFG_RX_CLK_ENA |
               VTSS_F_HOST_MAC_CONFIG_MAC_ENA_CFG_TX_CLK_ENA |
               VTSS_F_HOST_MAC_CONFIG_MAC_ENA_CFG_RX_SW_RST |
               VTSS_F_HOST_MAC_CONFIG_MAC_ENA_CFG_TX_SW_RST);

        // Disable clocks
        CSR_WR(port_no, VTSS_LINE_MAC_CONFIG_MAC_ENA_CFG,
               VTSS_F_LINE_MAC_CONFIG_MAC_ENA_CFG_RX_SW_RST |
               VTSS_F_LINE_MAC_CONFIG_MAC_ENA_CFG_TX_SW_RST);

        CSR_WR(port_no, VTSS_HOST_MAC_CONFIG_MAC_ENA_CFG,
               VTSS_F_HOST_MAC_CONFIG_MAC_ENA_CFG_RX_SW_RST |
               VTSS_F_HOST_MAC_CONFIG_MAC_ENA_CFG_TX_SW_RST);

    }

    if (enable) {
        CSR_COLD_WRM(port_no, VTSS_LINE_MAC_CONFIG_MAC_MODE_CFG,
                     VTSS_BIT(16), VTSS_BIT(16));

        CSR_COLD_WRM(port_no, VTSS_LINE_MAC_CONFIG_MAC_MODE_CFG,
                     0, VTSS_BIT(16));
    }

#endif

    return VTSS_RC_OK;
}

vtss_rc vtss_statistics_get(const vtss_inst_t inst,
                             const vtss_port_no_t  port_no,
                              vtss_statistic_t *stats)
{
    vtss_rc      rc = VTSS_RC_OK;
#if defined(VTSS_CHIP_CU_PHY)
    vtss_phy_type_t phy_1g_id;
    BOOL is_phy_1g  = (vtss_phy_id_get(inst, port_no, &phy_1g_id) == VTSS_RC_OK)  && (phy_1g_id.part_number != VTSS_PHY_TYPE_NONE);
#endif
    VTSS_ENTER();

#if defined(VTSS_CHIP_CU_PHY)
    if (is_phy_1g) {
    rc = vtss_phy_statistic_get(inst, port_no, &(stats->counters_1g));
    } else {
#endif /* VTSS_CHIP_CU_PHY */
#if defined(VTSS_CHIP_10G_PHY)
            rc = vtss_phy_10g_pkt_mon_counters_get(inst, port_no, &(stats->counters_10g));
#endif /* VTSS_CHIP_10G_PHY */
#if defined(VTSS_CHIP_CU_PHY)
    }
#endif
    VTSS_EXIT();

    return rc;
}

