#ifndef _VTSS_SERVAL_REGS_COMMON_H_
#define _VTSS_SERVAL_REGS_COMMON_H_

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

#ifndef VTSS_BITOPS_DEFINED
#ifdef __ASSEMBLER__
#define VTSS_BIT(x)                   (1 << (x))
#define VTSS_BITMASK(x)               ((1 << (x)) - 1)
#else
#define VTSS_BIT(x)                   (1U << (x))
#define VTSS_BITMASK(x)               ((1U << (x)) - 1)
#endif
#define VTSS_EXTRACT_BITFIELD(x,o,w)  (((x) >> (o)) & VTSS_BITMASK(w))
#define VTSS_ENCODE_BITFIELD(x,o,w)   (((x) & VTSS_BITMASK(w)) << (o))
#define VTSS_ENCODE_BITMASK(o,w)      (VTSS_BITMASK(w) << (o))
#define VTSS_BITOPS_DEFINED
#endif /* VTSS_BITOPS_DEFINED */


/*
 * Target offset base(s)
 */
#define VTSS_IO_ORIGIN1_OFFSET 0x70000000
#define VTSS_IO_ORIGIN1_SIZE   0x00200000
#define VTSS_IO_ORIGIN2_OFFSET 0x71000000
#define VTSS_IO_ORIGIN2_SIZE   0x01000000
#ifndef VTSS_IO_OFFSET1
#define VTSS_IO_OFFSET1(offset) (VTSS_IO_ORIGIN1_OFFSET + offset)
#endif
#ifndef VTSS_IO_OFFSET2
#define VTSS_IO_OFFSET2(offset) (VTSS_IO_ORIGIN2_OFFSET + offset)
#endif

/* Main target address offsets */
#define VTSS_TO_CFG        VTSS_IO_OFFSET1(0x00000000) /*!< Base offset for target CFG */
#define VTSS_TO_UART       VTSS_IO_OFFSET1(0x00100000) /*!< Base offset for target UART */
#define VTSS_TO_TWI        VTSS_IO_OFFSET1(0x00100400) /*!< Base offset for target TWI */
#define VTSS_TO_UART2      VTSS_IO_OFFSET1(0x00100800) /*!< Base offset for target UART2 */
#define VTSS_TO_PCIE_EP    VTSS_IO_OFFSET1(0x00101000) /*!< Base offset for target PCIE_EP */
#define VTSS_TO_SBA        VTSS_IO_OFFSET1(0x00110000) /*!< Base offset for target SBA */
#define VTSS_TO_DEVCPU_ORG VTSS_IO_OFFSET2(0x00000000) /*!< Base offset for target DEVCPU_ORG */
#define VTSS_TO_SYS        VTSS_IO_OFFSET2(0x00010000) /*!< Base offset for target SYS */
#define VTSS_TO_REW        VTSS_IO_OFFSET2(0x00030000) /*!< Base offset for target REW */
#define VTSS_TO_ES0        VTSS_IO_OFFSET2(0x00040000) /*!< Base offset for target ES0 */
#define VTSS_TO_S1         VTSS_IO_OFFSET2(0x00050000) /*!< Base offset for target S1 */
#define VTSS_TO_S2         VTSS_IO_OFFSET2(0x00060000) /*!< Base offset for target S2 */
#define VTSS_TO_DEVCPU_GCB VTSS_IO_OFFSET2(0x00070000) /*!< Base offset for target DEVCPU_GCB */
#define VTSS_TO_DEVCPU_QS  VTSS_IO_OFFSET2(0x00080000) /*!< Base offset for target DEVCPU_QS */
#define VTSS_TO_HSIO       VTSS_IO_OFFSET2(0x000a0000) /*!< Base offset for target HSIO */
#define VTSS_TO_IS0        VTSS_IO_OFFSET2(0x000b0000) /*!< Base offset for target IS0 */
#define VTSS_TO_OAM_MEP    VTSS_IO_OFFSET2(0x000c0000) /*!< Base offset for target OAM_MEP */
#define VTSS_TO_DEV_0      VTSS_IO_OFFSET2(0x001e0000) /*!< Base offset for target DEV_0 */
#define VTSS_TO_DEV_1      VTSS_IO_OFFSET2(0x001f0000) /*!< Base offset for target DEV_1 */
#define VTSS_TO_DEV_2      VTSS_IO_OFFSET2(0x00200000) /*!< Base offset for target DEV_2 */
#define VTSS_TO_DEV_3      VTSS_IO_OFFSET2(0x00210000) /*!< Base offset for target DEV_3 */
#define VTSS_TO_DEV_4      VTSS_IO_OFFSET2(0x00220000) /*!< Base offset for target DEV_4 */
#define VTSS_TO_DEV_5      VTSS_IO_OFFSET2(0x00230000) /*!< Base offset for target DEV_5 */
#define VTSS_TO_DEV_6      VTSS_IO_OFFSET2(0x00240000) /*!< Base offset for target DEV_6 */
#define VTSS_TO_DEV_7      VTSS_IO_OFFSET2(0x00250000) /*!< Base offset for target DEV_7 */
#define VTSS_TO_DEV_8      VTSS_IO_OFFSET2(0x00260000) /*!< Base offset for target DEV_8 */
#define VTSS_TO_DEV_9      VTSS_IO_OFFSET2(0x00270000) /*!< Base offset for target DEV_9 */
#define VTSS_TO_DEV_10     VTSS_IO_OFFSET2(0x00280000) /*!< Base offset for target DEV_10 */
#define VTSS_TO_QSYS       VTSS_IO_OFFSET2(0x00800000) /*!< Base offset for target QSYS */
#define VTSS_TO_ANA        VTSS_IO_OFFSET2(0x00900000) /*!< Base offset for target ANA */

/* IO address mapping macro - may be changed for platform */
#if !defined(VTSS_IOADDR)
#define VTSS_IOADDR(t,o)        ((t) + ((o) << 2))
#endif

/* IO register access macro - may be changed for platform */
#if !defined(VTSS_IOREG)
/** 
 * @param t - target base offset
 * @param o - subtarget offset
 */
#define VTSS_IOREG(t,o)      (*((volatile unsigned long*)(VTSS_IOADDR(t,o))))
#endif

/* IO indexed register access macro - may be changed for platform */
#if !defined(VTSS_IOREG_IX)
/** 
 * @param t  - target base offset
 * @param o  - subtarget offset
 * @param g  - group instance, 
 * @param gw - group width
 * @param ro - register offset, 
 * @param r  - register (instance) number
 */
#define VTSS_IOREG_IX(t,o,g,gw,r,ro)   VTSS_IOREG(t,(o) + ((g) * (gw)) + (ro) + (r))
#endif

#ifdef VTSS_SERVAL_WANT_TARGET_ENUMS
/*
 * This section is primarily for documentation purposes.
 */

/**
  * \brief Target UART target offset(s)
  */
enum vtss_target_UART_e {
    VTSS_TOE_UART            = VTSS_TO_UART,
    VTSS_TOE_UART2           = VTSS_TO_UART2,
};

/**
  * \brief Target DEV target offset(s)
  */
enum vtss_target_DEV_e {
    VTSS_TOE_DEV_0           = VTSS_TO_DEV_0,
    VTSS_TOE_DEV_1           = VTSS_TO_DEV_1,
    VTSS_TOE_DEV_2           = VTSS_TO_DEV_2,
    VTSS_TOE_DEV_3           = VTSS_TO_DEV_3,
    VTSS_TOE_DEV_4           = VTSS_TO_DEV_4,
    VTSS_TOE_DEV_5           = VTSS_TO_DEV_5,
    VTSS_TOE_DEV_6           = VTSS_TO_DEV_6,
    VTSS_TOE_DEV_7           = VTSS_TO_DEV_7,
    VTSS_TOE_DEV_8           = VTSS_TO_DEV_8,
    VTSS_TOE_DEV_9           = VTSS_TO_DEV_9,
    VTSS_TOE_DEV_10          = VTSS_TO_DEV_10,
};

/**
  * \brief Target VCAP_CORE target offset(s)
  */
enum vtss_target_VCAP_CORE_e {
    VTSS_TOE_ES0             = VTSS_TO_ES0,
    VTSS_TOE_IS0             = VTSS_TO_IS0,
    VTSS_TOE_S1              = VTSS_TO_S1,
    VTSS_TOE_S2              = VTSS_TO_S2,
};

#endif /* VTSS_SERVAL_WANT_TARGET_ENUMS */


#endif /* _VTSS_SERVAL_REGS_COMMON_H_ */
