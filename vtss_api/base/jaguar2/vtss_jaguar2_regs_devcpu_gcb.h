#ifndef _VTSS_JAGUAR2_REGS_DEVCPU_GCB_H_
#define _VTSS_JAGUAR2_REGS_DEVCPU_GCB_H_

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

#include "vtss_jaguar2_regs_common.h"

/*********************************************************************** 
 *
 * Target: \a DEVCPU_GCB
 *
 * General Configuration Block
 *
 ***********************************************************************/

/**
 * Register Group: \a DEVCPU_GCB:CHIP_REGS
 *
 * Not documented
 */


/** 
 * \brief Chip ID register
 *
 * \details
 * Register: \a DEVCPU_GCB:CHIP_REGS:CHIP_ID
 */
#define VTSS_DEVCPU_GCB_CHIP_REGS_CHIP_ID    VTSS_IOREG(VTSS_TO_DEVCPU_GCB,0x0)

/** 
 * \brief
 * Revision ID : Chip revision starting from 0.
 *
 * \details 
 * Field: ::VTSS_DEVCPU_GCB_CHIP_REGS_CHIP_ID . REV_ID
 */
#define  VTSS_F_DEVCPU_GCB_CHIP_REGS_CHIP_ID_REV_ID(x)  VTSS_ENCODE_BITFIELD(x,28,4)
#define  VTSS_M_DEVCPU_GCB_CHIP_REGS_CHIP_ID_REV_ID     VTSS_ENCODE_BITMASK(28,4)
#define  VTSS_X_DEVCPU_GCB_CHIP_REGS_CHIP_ID_REV_ID(x)  VTSS_EXTRACT_BITFIELD(x,28,4)

/** 
 * \brief
 * Part ID: Part ID for identification of Vitesse Chips.
 *
 * \details 
 * Field: ::VTSS_DEVCPU_GCB_CHIP_REGS_CHIP_ID . PART_ID
 */
#define  VTSS_F_DEVCPU_GCB_CHIP_REGS_CHIP_ID_PART_ID(x)  VTSS_ENCODE_BITFIELD(x,12,16)
#define  VTSS_M_DEVCPU_GCB_CHIP_REGS_CHIP_ID_PART_ID     VTSS_ENCODE_BITMASK(12,16)
#define  VTSS_X_DEVCPU_GCB_CHIP_REGS_CHIP_ID_PART_ID(x)  VTSS_EXTRACT_BITFIELD(x,12,16)

/** 
 * \brief
 * Manufacturer ID.
 *
 * \details 
 * Field: ::VTSS_DEVCPU_GCB_CHIP_REGS_CHIP_ID . MFG_ID
 */
#define  VTSS_F_DEVCPU_GCB_CHIP_REGS_CHIP_ID_MFG_ID(x)  VTSS_ENCODE_BITFIELD(x,1,11)
#define  VTSS_M_DEVCPU_GCB_CHIP_REGS_CHIP_ID_MFG_ID     VTSS_ENCODE_BITMASK(1,11)
#define  VTSS_X_DEVCPU_GCB_CHIP_REGS_CHIP_ID_MFG_ID(x)  VTSS_EXTRACT_BITFIELD(x,1,11)

/** 
 * \brief
 * Always 1.
 *
 * \details 
 * Field: ::VTSS_DEVCPU_GCB_CHIP_REGS_CHIP_ID . ONE
 */
#define  VTSS_F_DEVCPU_GCB_CHIP_REGS_CHIP_ID_ONE(x)  VTSS_ENCODE_BITFIELD(!!(x),0,1)
#define  VTSS_M_DEVCPU_GCB_CHIP_REGS_CHIP_ID_ONE  VTSS_BIT(0)
#define  VTSS_X_DEVCPU_GCB_CHIP_REGS_CHIP_ID_ONE(x)  VTSS_EXTRACT_BITFIELD(x,0,1)


/** 
 * \brief General purpose register
 *
 * \details
 * Register: \a DEVCPU_GCB:CHIP_REGS:GPR
 */
#define VTSS_DEVCPU_GCB_CHIP_REGS_GPR        VTSS_IOREG(VTSS_TO_DEVCPU_GCB,0x1)


/** 
 * \brief Reset control register
 *
 * \details
 * Register: \a DEVCPU_GCB:CHIP_REGS:SOFT_RST
 */
#define VTSS_DEVCPU_GCB_CHIP_REGS_SOFT_RST   VTSS_IOREG(VTSS_TO_DEVCPU_GCB,0x2)

/** 
 * \brief
 * Set this field to reset everything except the chip configuration. The
 * reset remains asserted until this field is cleared.
 *
 * \details 
 * Field: ::VTSS_DEVCPU_GCB_CHIP_REGS_SOFT_RST . SOFT_NON_CFG_RST
 */
#define  VTSS_F_DEVCPU_GCB_CHIP_REGS_SOFT_RST_SOFT_NON_CFG_RST(x)  VTSS_ENCODE_BITFIELD(!!(x),2,1)
#define  VTSS_M_DEVCPU_GCB_CHIP_REGS_SOFT_RST_SOFT_NON_CFG_RST  VTSS_BIT(2)
#define  VTSS_X_DEVCPU_GCB_CHIP_REGS_SOFT_RST_SOFT_NON_CFG_RST(x)  VTSS_EXTRACT_BITFIELD(x,2,1)

/** 
 * \brief
 * Set this field to reset the switch core (everything except the SERDES IO
 * and PLL blocks.) This field is automatically cleared by the reset.
 * Note: It is possible for the VCore to protect itself from this
 * soft-reset, for more info see ICPU_CFG::RESET.CORE_RST_PROTECT.
 *
 * \details 
 * Field: ::VTSS_DEVCPU_GCB_CHIP_REGS_SOFT_RST . SOFT_SWC_RST
 */
#define  VTSS_F_DEVCPU_GCB_CHIP_REGS_SOFT_RST_SOFT_SWC_RST(x)  VTSS_ENCODE_BITFIELD(!!(x),1,1)
#define  VTSS_M_DEVCPU_GCB_CHIP_REGS_SOFT_RST_SOFT_SWC_RST  VTSS_BIT(1)
#define  VTSS_X_DEVCPU_GCB_CHIP_REGS_SOFT_RST_SOFT_SWC_RST(x)  VTSS_EXTRACT_BITFIELD(x,1,1)

/** 
 * \brief
 * Set this field to reset the whole chip. This field is automatically
 * cleared by the reset.
 * Note: It is possible for the VCore to protect itself from this
 * soft-reset, for more info see ICPU_CFG::RESET.CORE_RST_PROTECT.
 *
 * \details 
 * Field: ::VTSS_DEVCPU_GCB_CHIP_REGS_SOFT_RST . SOFT_CHIP_RST
 */
#define  VTSS_F_DEVCPU_GCB_CHIP_REGS_SOFT_RST_SOFT_CHIP_RST(x)  VTSS_ENCODE_BITFIELD(!!(x),0,1)
#define  VTSS_M_DEVCPU_GCB_CHIP_REGS_SOFT_RST_SOFT_CHIP_RST  VTSS_BIT(0)
#define  VTSS_X_DEVCPU_GCB_CHIP_REGS_SOFT_RST_SOFT_CHIP_RST(x)  VTSS_EXTRACT_BITFIELD(x,0,1)


/** 
 * \brief Various configrations
 *
 * \details
 * Register: \a DEVCPU_GCB:CHIP_REGS:HW_CFG
 */
#define VTSS_DEVCPU_GCB_CHIP_REGS_HW_CFG     VTSS_IOREG(VTSS_TO_DEVCPU_GCB,0x3)

/** 
 * \brief
 * Set to enable the DFT STAToutput on the GPIOs.
 *
 * \details 
 * Field: ::VTSS_DEVCPU_GCB_CHIP_REGS_HW_CFG . DFT_STAT_ENA
 */
#define  VTSS_F_DEVCPU_GCB_CHIP_REGS_HW_CFG_DFT_STAT_ENA(x)  VTSS_ENCODE_BITFIELD(!!(x),1,1)
#define  VTSS_M_DEVCPU_GCB_CHIP_REGS_HW_CFG_DFT_STAT_ENA  VTSS_BIT(1)
#define  VTSS_X_DEVCPU_GCB_CHIP_REGS_HW_CFG_DFT_STAT_ENA(x)  VTSS_EXTRACT_BITFIELD(x,1,1)

/** 
 * \brief
 * Set to enable the DFT clock monitor feature on the GPIOs.
 *
 * \details 
 * Field: ::VTSS_DEVCPU_GCB_CHIP_REGS_HW_CFG . DFT_CLK_MON_ENA
 */
#define  VTSS_F_DEVCPU_GCB_CHIP_REGS_HW_CFG_DFT_CLK_MON_ENA(x)  VTSS_ENCODE_BITFIELD(!!(x),0,1)
#define  VTSS_M_DEVCPU_GCB_CHIP_REGS_HW_CFG_DFT_CLK_MON_ENA  VTSS_BIT(0)
#define  VTSS_X_DEVCPU_GCB_CHIP_REGS_HW_CFG_DFT_CLK_MON_ENA(x)  VTSS_EXTRACT_BITFIELD(x,0,1)


/** 
 * \brief Various status indications
 *
 * \details
 * Register: \a DEVCPU_GCB:CHIP_REGS:HW_STAT
 */
#define VTSS_DEVCPU_GCB_CHIP_REGS_HW_STAT    VTSS_IOREG(VTSS_TO_DEVCPU_GCB,0x4)

/** 
 * \brief
 * This field is set if a hardware fail has been detected in any of the
 * memories during startup-initialization of the chip. This field is valid
 * after release of reset.
 *
 * \details 
 * Field: ::VTSS_DEVCPU_GCB_CHIP_REGS_HW_STAT . MEM_FAIL
 */
#define  VTSS_F_DEVCPU_GCB_CHIP_REGS_HW_STAT_MEM_FAIL(x)  VTSS_ENCODE_BITFIELD(!!(x),0,1)
#define  VTSS_M_DEVCPU_GCB_CHIP_REGS_HW_STAT_MEM_FAIL  VTSS_BIT(0)
#define  VTSS_X_DEVCPU_GCB_CHIP_REGS_HW_STAT_MEM_FAIL(x)  VTSS_EXTRACT_BITFIELD(x,0,1)


/** 
 * \brief Additional configuration of SGPIO signal detect 
 *
 * \details
 * Register: \a DEVCPU_GCB:CHIP_REGS:HW_SGPIO_SD_CFG
 */
#define VTSS_DEVCPU_GCB_CHIP_REGS_HW_SGPIO_SD_CFG  VTSS_IOREG(VTSS_TO_DEVCPU_GCB,0x5)

/** 
 * \brief
 * Set bits in this field to use 2G5 and 10G signal detects from 3'rd SIO
 * controller instead of default positions inside 1'st and 2'nd
 * controllers. Bit 0 in this field corresponds to dev2g5_0, bit 1
 * corresponds ot dev2g5_1 and so on. Bit 24 corrsponds to NPI port. Bit 25
 * corresponds to dev10g_0, bit 26 corresponds to dev10g_1 and so on. For
 * the dev10g devices; signal detect is provided to both the 10g device and
 * the corresponding dev2g5 (see HSIO::HW_CFG.DEV10G_0_MODE for more
 * information.)
 *
 * \details 
 * Field: ::VTSS_DEVCPU_GCB_CHIP_REGS_HW_SGPIO_SD_CFG . SD_HIGH_ENA
 */
#define  VTSS_F_DEVCPU_GCB_CHIP_REGS_HW_SGPIO_SD_CFG_SD_HIGH_ENA(x)  VTSS_ENCODE_BITFIELD(x,0,29)
#define  VTSS_M_DEVCPU_GCB_CHIP_REGS_HW_SGPIO_SD_CFG_SD_HIGH_ENA     VTSS_ENCODE_BITMASK(0,29)
#define  VTSS_X_DEVCPU_GCB_CHIP_REGS_HW_SGPIO_SD_CFG_SD_HIGH_ENA(x)  VTSS_EXTRACT_BITFIELD(x,0,29)


/** 
 * \brief Encoded features
 *
 * \details
 * Register: \a DEVCPU_GCB:CHIP_REGS:FEA_STAT
 */
#define VTSS_DEVCPU_GCB_CHIP_REGS_FEA_STAT   VTSS_IOREG(VTSS_TO_DEVCPU_GCB,0x6)


/** 
 * \brief Disable of features
 *
 * \details
 * Register: \a DEVCPU_GCB:CHIP_REGS:FEA_DIS
 */
#define VTSS_DEVCPU_GCB_CHIP_REGS_FEA_DIS    VTSS_IOREG(VTSS_TO_DEVCPU_GCB,0x7)

/**
 * Register Group: \a DEVCPU_GCB:SW_REGS
 *
 * Registers for software/software interaction
 */


/** 
 * \brief Manually assert software interrupt
 *
 * \details
 * This register provides a simple interface for interrupting on either
 * sofware interrupt 0 or 1, without implementing semaphore support. Note:
 * setting this field causes a short pulse on the corresponding interrupt
 * connection, this kind of interrupt cannot be used in combination with
 * the SW1_INTR_CONFIG.SW1_INTR_BYPASS feature.
 *
 * Register: \a DEVCPU_GCB:SW_REGS:SW_INTR
 */
#define VTSS_DEVCPU_GCB_SW_REGS_SW_INTR      VTSS_IOREG(VTSS_TO_DEVCPU_GCB,0x8)

/** 
 * \brief
 * Set this field to inject software interrupt 1. This field is
 * automatically cleared after interrupt has been generated.
 *
 * \details 
 * Field: ::VTSS_DEVCPU_GCB_SW_REGS_SW_INTR . SW1_INTR
 */
#define  VTSS_F_DEVCPU_GCB_SW_REGS_SW_INTR_SW1_INTR(x)  VTSS_ENCODE_BITFIELD(!!(x),1,1)
#define  VTSS_M_DEVCPU_GCB_SW_REGS_SW_INTR_SW1_INTR  VTSS_BIT(1)
#define  VTSS_X_DEVCPU_GCB_SW_REGS_SW_INTR_SW1_INTR(x)  VTSS_EXTRACT_BITFIELD(x,1,1)

/** 
 * \brief
 * Set this field to assert software interrupt 0. This field is
 * automatically cleared after interrupt has been generated.
 *
 * \details 
 * Field: ::VTSS_DEVCPU_GCB_SW_REGS_SW_INTR . SW0_INTR
 */
#define  VTSS_F_DEVCPU_GCB_SW_REGS_SW_INTR_SW0_INTR(x)  VTSS_ENCODE_BITFIELD(!!(x),0,1)
#define  VTSS_M_DEVCPU_GCB_SW_REGS_SW_INTR_SW0_INTR  VTSS_BIT(0)
#define  VTSS_X_DEVCPU_GCB_SW_REGS_SW_INTR_SW0_INTR(x)  VTSS_EXTRACT_BITFIELD(x,0,1)

/**
 * Register Group: \a DEVCPU_GCB:VCORE_ACCESS
 *
 * Registers for accessing the VCore
 */


/** 
 * \brief Control register for VCore accesses
 *
 * \details
 * Register: \a DEVCPU_GCB:VCORE_ACCESS:VA_CTRL
 */
#define VTSS_DEVCPU_GCB_VCORE_ACCESS_VA_CTRL  VTSS_IOREG(VTSS_TO_DEVCPU_GCB,0x9)

/** 
 * \brief
 * Controls the size of the access inside VCore domain. It is possible to
 * do 32-bit, 16-bit and 8-bit accesses. For 8bit and 16bit write-accesses
 * data must be aligned appropriately inside the 32bit write-data word
 * (i.e. for a byte-write to address 0x20001003 data has to be placed in
 * [31:24]). Likewise for 8bit and 16bit read operations, here data is
 * alligned accordingly to address.
 *
 * \details 
 * 0: 32bit
 * 1: Reserved, do not use
 * 2: 8bit
 * 3: 16bit
 *
 * Field: ::VTSS_DEVCPU_GCB_VCORE_ACCESS_VA_CTRL . VA_SIZE
 */
#define  VTSS_F_DEVCPU_GCB_VCORE_ACCESS_VA_CTRL_VA_SIZE(x)  VTSS_ENCODE_BITFIELD(x,4,2)
#define  VTSS_M_DEVCPU_GCB_VCORE_ACCESS_VA_CTRL_VA_SIZE     VTSS_ENCODE_BITMASK(4,2)
#define  VTSS_X_DEVCPU_GCB_VCORE_ACCESS_VA_CTRL_VA_SIZE(x)  VTSS_EXTRACT_BITFIELD(x,4,2)

/** 
 * \brief
 * If the VCore access logic detects an error this field is set based on
 * the nature of the error. This is a read-only field which is cleared by
 * the VCore access logic when a new access is (sucessfully) accepted.
 *
 * \details 
 * 0: No errors detected.
 * 1: SBA not ready when accessed.
 * 2: SBA reported error.
 * 3: DATA or ADDR written during active access.
 *
 * Field: ::VTSS_DEVCPU_GCB_VCORE_ACCESS_VA_CTRL . VA_ERR
 */
#define  VTSS_F_DEVCPU_GCB_VCORE_ACCESS_VA_CTRL_VA_ERR(x)  VTSS_ENCODE_BITFIELD(x,2,2)
#define  VTSS_M_DEVCPU_GCB_VCORE_ACCESS_VA_CTRL_VA_ERR     VTSS_ENCODE_BITMASK(2,2)
#define  VTSS_X_DEVCPU_GCB_VCORE_ACCESS_VA_CTRL_VA_ERR(x)  VTSS_EXTRACT_BITFIELD(x,2,2)

/** 
 * \brief
 * This field is set to the value of VA_CTRL.VA_BUSY whenever one of the
 * data registers VA_DATA, VA_DATA_INCR, or VA_DATA_RO is read. By
 * examining this field it is possible to determine if VA_BUSY was set at
 * the time a read from one of these registers was performed.
 *
 * \details 
 * Field: ::VTSS_DEVCPU_GCB_VCORE_ACCESS_VA_CTRL . VA_BUSY_RD
 */
#define  VTSS_F_DEVCPU_GCB_VCORE_ACCESS_VA_CTRL_VA_BUSY_RD(x)  VTSS_ENCODE_BITFIELD(!!(x),1,1)
#define  VTSS_M_DEVCPU_GCB_VCORE_ACCESS_VA_CTRL_VA_BUSY_RD  VTSS_BIT(1)
#define  VTSS_X_DEVCPU_GCB_VCORE_ACCESS_VA_CTRL_VA_BUSY_RD(x)  VTSS_EXTRACT_BITFIELD(x,1,1)

/** 
 * \brief
 * This field is set by hardware when an access into VCore domain is
 * started, and cleared when the access is done.
 *
 * \details 
 * Field: ::VTSS_DEVCPU_GCB_VCORE_ACCESS_VA_CTRL . VA_BUSY
 */
#define  VTSS_F_DEVCPU_GCB_VCORE_ACCESS_VA_CTRL_VA_BUSY(x)  VTSS_ENCODE_BITFIELD(!!(x),0,1)
#define  VTSS_M_DEVCPU_GCB_VCORE_ACCESS_VA_CTRL_VA_BUSY  VTSS_BIT(0)
#define  VTSS_X_DEVCPU_GCB_VCORE_ACCESS_VA_CTRL_VA_BUSY(x)  VTSS_EXTRACT_BITFIELD(x,0,1)


/** 
 * \brief Address register for VCore accesses
 *
 * \details
 * Register: \a DEVCPU_GCB:VCORE_ACCESS:VA_ADDR
 */
#define VTSS_DEVCPU_GCB_VCORE_ACCESS_VA_ADDR  VTSS_IOREG(VTSS_TO_DEVCPU_GCB,0xa)


/** 
 * \brief Data register for VCore accesses
 *
 * \details
 * The VA_DATA, VA_DATA_INCR, and VA_DATA_INERT registers are used for
 * indirect access into the VCore domain. The functionality of the
 * VA_DATA_INCR and VA_DATA_INERT registers are similar to this register -
 * but with minor exceptions. These exceptions are fleshed out in the
 * description of the respective registers.
 *
 * Register: \a DEVCPU_GCB:VCORE_ACCESS:VA_DATA
 */
#define VTSS_DEVCPU_GCB_VCORE_ACCESS_VA_DATA  VTSS_IOREG(VTSS_TO_DEVCPU_GCB,0xb)


/** 
 * \brief Data register for VCore accesses (w. auto increment of address)
 *
 * \details
 * Register: \a DEVCPU_GCB:VCORE_ACCESS:VA_DATA_INCR
 */
#define VTSS_DEVCPU_GCB_VCORE_ACCESS_VA_DATA_INCR  VTSS_IOREG(VTSS_TO_DEVCPU_GCB,0xc)


/** 
 * \brief Data register for VCore accesses (will not initiate access)
 *
 * \details
 * Register: \a DEVCPU_GCB:VCORE_ACCESS:VA_DATA_INERT
 */
#define VTSS_DEVCPU_GCB_VCORE_ACCESS_VA_DATA_INERT  VTSS_IOREG(VTSS_TO_DEVCPU_GCB,0xd)

/**
 * Register Group: \a DEVCPU_GCB:GPIO
 *
 * Not documented
 */


/** 
 * \brief GPIO output set
 *
 * \details
 * Register: \a DEVCPU_GCB:GPIO:GPIO_OUT_SET
 */
#define VTSS_DEVCPU_GCB_GPIO_GPIO_OUT_SET    VTSS_IOREG(VTSS_TO_DEVCPU_GCB,0xe)


/** 
 * \brief GPIO output set
 *
 * \details
 * Register: \a DEVCPU_GCB:GPIO:GPIO_OUT_SET1
 */
#define VTSS_DEVCPU_GCB_GPIO_GPIO_OUT_SET1   VTSS_IOREG(VTSS_TO_DEVCPU_GCB,0xf)


/** 
 * \brief GPIO output clear
 *
 * \details
 * Register: \a DEVCPU_GCB:GPIO:GPIO_OUT_CLR
 */
#define VTSS_DEVCPU_GCB_GPIO_GPIO_OUT_CLR    VTSS_IOREG(VTSS_TO_DEVCPU_GCB,0x10)


/** 
 * \brief GPIO output clear
 *
 * \details
 * Register: \a DEVCPU_GCB:GPIO:GPIO_OUT_CLR1
 */
#define VTSS_DEVCPU_GCB_GPIO_GPIO_OUT_CLR1   VTSS_IOREG(VTSS_TO_DEVCPU_GCB,0x11)


/** 
 * \brief GPIO output
 *
 * \details
 * In a multi-threaded software environment using the registers
 * GPIO_OUT_SET and GPIO_OUT_CLR for modifying GPIO values removes the need
 * for software-locked access.
 *
 * Register: \a DEVCPU_GCB:GPIO:GPIO_OUT
 */
#define VTSS_DEVCPU_GCB_GPIO_GPIO_OUT        VTSS_IOREG(VTSS_TO_DEVCPU_GCB,0x12)


/** 
 * \brief GPIO output
 *
 * \details
 * In a multi-threaded software environment using the registers
 * GPIO_OUT_SET and GPIO_OUT_CLR for modifying GPIO values removes the need
 * for software-locked access.
 *
 * Register: \a DEVCPU_GCB:GPIO:GPIO_OUT1
 */
#define VTSS_DEVCPU_GCB_GPIO_GPIO_OUT1       VTSS_IOREG(VTSS_TO_DEVCPU_GCB,0x13)


/** 
 * \brief GPIO input
 *
 * \details
 * Register: \a DEVCPU_GCB:GPIO:GPIO_IN
 */
#define VTSS_DEVCPU_GCB_GPIO_GPIO_IN         VTSS_IOREG(VTSS_TO_DEVCPU_GCB,0x14)


/** 
 * \brief GPIO input
 *
 * \details
 * Register: \a DEVCPU_GCB:GPIO:GPIO_IN1
 */
#define VTSS_DEVCPU_GCB_GPIO_GPIO_IN1        VTSS_IOREG(VTSS_TO_DEVCPU_GCB,0x15)


/** 
 * \brief GPIO pin direction
 *
 * \details
 * Register: \a DEVCPU_GCB:GPIO:GPIO_OE
 */
#define VTSS_DEVCPU_GCB_GPIO_GPIO_OE         VTSS_IOREG(VTSS_TO_DEVCPU_GCB,0x16)


/** 
 * \brief GPIO pin direction
 *
 * \details
 * Register: \a DEVCPU_GCB:GPIO:GPIO_OE1
 */
#define VTSS_DEVCPU_GCB_GPIO_GPIO_OE1        VTSS_IOREG(VTSS_TO_DEVCPU_GCB,0x17)


/** 
 * \brief GPIO interrupt
 *
 * \details
 * Register: \a DEVCPU_GCB:GPIO:GPIO_INTR
 */
#define VTSS_DEVCPU_GCB_GPIO_GPIO_INTR       VTSS_IOREG(VTSS_TO_DEVCPU_GCB,0x18)


/** 
 * \brief GPIO interrupt
 *
 * \details
 * Register: \a DEVCPU_GCB:GPIO:GPIO_INTR1
 */
#define VTSS_DEVCPU_GCB_GPIO_GPIO_INTR1      VTSS_IOREG(VTSS_TO_DEVCPU_GCB,0x19)


/** 
 * \brief GPIO interrupt enable
 *
 * \details
 * Register: \a DEVCPU_GCB:GPIO:GPIO_INTR_ENA
 */
#define VTSS_DEVCPU_GCB_GPIO_GPIO_INTR_ENA   VTSS_IOREG(VTSS_TO_DEVCPU_GCB,0x1a)


/** 
 * \brief GPIO interrupt enable
 *
 * \details
 * Register: \a DEVCPU_GCB:GPIO:GPIO_INTR_ENA1
 */
#define VTSS_DEVCPU_GCB_GPIO_GPIO_INTR_ENA1  VTSS_IOREG(VTSS_TO_DEVCPU_GCB,0x1b)


/** 
 * \brief GPIO interrupt identity
 *
 * \details
 * Register: \a DEVCPU_GCB:GPIO:GPIO_INTR_IDENT
 */
#define VTSS_DEVCPU_GCB_GPIO_GPIO_INTR_IDENT  VTSS_IOREG(VTSS_TO_DEVCPU_GCB,0x1c)


/** 
 * \brief GPIO interrupt identity
 *
 * \details
 * Register: \a DEVCPU_GCB:GPIO:GPIO_INTR_IDENT1
 */
#define VTSS_DEVCPU_GCB_GPIO_GPIO_INTR_IDENT1  VTSS_IOREG(VTSS_TO_DEVCPU_GCB,0x1d)


/** 
 * \brief GPIO alternate functions
 *
 * \details
 * Register: \a DEVCPU_GCB:GPIO:GPIO_ALT
 *
 * @param ri Register: GPIO_ALT (??), 0-1
 */
#define VTSS_DEVCPU_GCB_GPIO_GPIO_ALT(ri)    VTSS_IOREG(VTSS_TO_DEVCPU_GCB,0x1e + (ri))


/** 
 * \brief GPIO alternate functions
 *
 * \details
 * Register: \a DEVCPU_GCB:GPIO:GPIO_ALT1
 *
 * @param ri Register: GPIO_ALT1 (??), 0-1
 */
#define VTSS_DEVCPU_GCB_GPIO_GPIO_ALT1(ri)   VTSS_IOREG(VTSS_TO_DEVCPU_GCB,0x20 + (ri))


/** 
 * \brief GPIO Signal Detect Mapping
 *
 * \details
 * Register: \a DEVCPU_GCB:GPIO:GPIO_SD_MAP
 *
 * @param ri Register: GPIO_SD_MAP (??), 0-15
 */
#define VTSS_DEVCPU_GCB_GPIO_GPIO_SD_MAP(ri)  VTSS_IOREG(VTSS_TO_DEVCPU_GCB,0x22 + (ri))

/** 
 * \brief
 * Set to map a specific GPIO mapped signal detect input to specific
 * front-port index. There is one replication for each GPIO mapped signal
 * detect input. If multiple signal detects are enabled and map to same
 * front-port index, then the higher replication index will take priority.
 * For example to map 3'rd signal detect input asif it was provided by 2'nd
 * SERDES; set DEVCPU_GCB::GPIO_SD_MAP[2].G_SD_MAP = 1 and enable SD2 via
 * DEVCPU_GCB::GPIO_ALT registers.
 *
 * \details 
 * Field: ::VTSS_DEVCPU_GCB_GPIO_GPIO_SD_MAP . G_SD_MAP
 */
#define  VTSS_F_DEVCPU_GCB_GPIO_GPIO_SD_MAP_G_SD_MAP(x)  VTSS_ENCODE_BITFIELD(x,0,6)
#define  VTSS_M_DEVCPU_GCB_GPIO_GPIO_SD_MAP_G_SD_MAP     VTSS_ENCODE_BITMASK(0,6)
#define  VTSS_X_DEVCPU_GCB_GPIO_GPIO_SD_MAP_G_SD_MAP(x)  VTSS_EXTRACT_BITFIELD(x,0,6)

/**
 * Register Group: \a DEVCPU_GCB:MIIM
 *
 * Not documented
 */


/** 
 * \brief MIIM Status
 *
 * \details
 * Register: \a DEVCPU_GCB:MIIM:MII_STATUS
 *
 * @param gi Replicator: x_FFL_DEVCPU_MIIM_CNT (??), 0-2
 */
#define VTSS_DEVCPU_GCB_MIIM_MII_STATUS(gi)  VTSS_IOREG_IX(VTSS_TO_DEVCPU_GCB,0x32,gi,9,0,0)

/** 
 * \brief
 * Signals if all PHYs have been scanned ( with auto scan ) at least once.
 *
 * \details 
 * 0 : Auto scan has not scanned all PHYs.
 * 1 : Auto scan has scanned all PHY at least once.
 *
 * Field: ::VTSS_DEVCPU_GCB_MIIM_MII_STATUS . MIIM_SCAN_COMPLETE
 */
#define  VTSS_F_DEVCPU_GCB_MIIM_MII_STATUS_MIIM_SCAN_COMPLETE(x)  VTSS_ENCODE_BITFIELD(!!(x),4,1)
#define  VTSS_M_DEVCPU_GCB_MIIM_MII_STATUS_MIIM_SCAN_COMPLETE  VTSS_BIT(4)
#define  VTSS_X_DEVCPU_GCB_MIIM_MII_STATUS_MIIM_SCAN_COMPLETE(x)  VTSS_EXTRACT_BITFIELD(x,4,1)

/** 
 * \brief
 * Indicates the current state of the MIIM controller. When read operations
 * are done (no longer busy), then read data is available via the
 * DEVCPU_GCB::MII_DATA register.
 *
 * \details 
 * 0: MIIM controller is in idle state
 * 1: MIIM controller is busy performing MIIM cmd (Either read or read
 * cmd).
 *
 * Field: ::VTSS_DEVCPU_GCB_MIIM_MII_STATUS . MIIM_STAT_BUSY
 */
#define  VTSS_F_DEVCPU_GCB_MIIM_MII_STATUS_MIIM_STAT_BUSY(x)  VTSS_ENCODE_BITFIELD(!!(x),3,1)
#define  VTSS_M_DEVCPU_GCB_MIIM_MII_STATUS_MIIM_STAT_BUSY  VTSS_BIT(3)
#define  VTSS_X_DEVCPU_GCB_MIIM_MII_STATUS_MIIM_STAT_BUSY(x)  VTSS_EXTRACT_BITFIELD(x,3,1)

/** 
 * \brief
 * The MIIM controller has a CMD fifo of depth one. When this field is 0,
 * then it is safe to write another MIIM command to the MIIM controller.
 *
 * \details 
 * 0 : Read or write not pending
 * 1 : Read or write pending.
 *
 * Field: ::VTSS_DEVCPU_GCB_MIIM_MII_STATUS . MIIM_STAT_OPR_PEND
 */
#define  VTSS_F_DEVCPU_GCB_MIIM_MII_STATUS_MIIM_STAT_OPR_PEND(x)  VTSS_ENCODE_BITFIELD(!!(x),2,1)
#define  VTSS_M_DEVCPU_GCB_MIIM_MII_STATUS_MIIM_STAT_OPR_PEND  VTSS_BIT(2)
#define  VTSS_X_DEVCPU_GCB_MIIM_MII_STATUS_MIIM_STAT_OPR_PEND(x)  VTSS_EXTRACT_BITFIELD(x,2,1)

/** 
 * \brief
 * Indicates whether a read operation via the MIIM interface is in progress
 * or not.
 *
 * \details 
 * 0 : Read not in progress
 * 1 : Read in progress.
 *
 * Field: ::VTSS_DEVCPU_GCB_MIIM_MII_STATUS . MIIM_STAT_PENDING_RD
 */
#define  VTSS_F_DEVCPU_GCB_MIIM_MII_STATUS_MIIM_STAT_PENDING_RD(x)  VTSS_ENCODE_BITFIELD(!!(x),1,1)
#define  VTSS_M_DEVCPU_GCB_MIIM_MII_STATUS_MIIM_STAT_PENDING_RD  VTSS_BIT(1)
#define  VTSS_X_DEVCPU_GCB_MIIM_MII_STATUS_MIIM_STAT_PENDING_RD(x)  VTSS_EXTRACT_BITFIELD(x,1,1)

/** 
 * \brief
 * Indicates whether a write operation via the MIIM interface is in
 * progress or not.
 *
 * \details 
 * 0 : Write not in progress
 * 1 : Write in progress.
 *
 * Field: ::VTSS_DEVCPU_GCB_MIIM_MII_STATUS . MIIM_STAT_PENDING_WR
 */
#define  VTSS_F_DEVCPU_GCB_MIIM_MII_STATUS_MIIM_STAT_PENDING_WR(x)  VTSS_ENCODE_BITFIELD(!!(x),0,1)
#define  VTSS_M_DEVCPU_GCB_MIIM_MII_STATUS_MIIM_STAT_PENDING_WR  VTSS_BIT(0)
#define  VTSS_X_DEVCPU_GCB_MIIM_MII_STATUS_MIIM_STAT_PENDING_WR(x)  VTSS_EXTRACT_BITFIELD(x,0,1)


/** 
 * \brief MIIM Configuration
 *
 * \details
 * Register: \a DEVCPU_GCB:MIIM:MII_CFG_7226
 *
 * @param gi Replicator: x_FFL_DEVCPU_MIIM_CNT (??), 0-2
 */
#define VTSS_DEVCPU_GCB_MIIM_MII_CFG_7226(gi)  VTSS_IOREG_IX(VTSS_TO_DEVCPU_GCB,0x32,gi,9,0,1)

/** 
 * \brief
 * Use of XAUI/XGMII translator device.
 *
 * \details 
 * 0 : Disable
 * 1 : Enable.
 *
 * Field: ::VTSS_DEVCPU_GCB_MIIM_MII_CFG_7226 . MIIM_7226_CFG_FIELD
 */
#define  VTSS_F_DEVCPU_GCB_MIIM_MII_CFG_7226_MIIM_7226_CFG_FIELD(x)  VTSS_ENCODE_BITFIELD(!!(x),9,1)
#define  VTSS_M_DEVCPU_GCB_MIIM_MII_CFG_7226_MIIM_7226_CFG_FIELD  VTSS_BIT(9)
#define  VTSS_X_DEVCPU_GCB_MIIM_MII_CFG_7226_MIIM_7226_CFG_FIELD(x)  VTSS_EXTRACT_BITFIELD(x,9,1)


/** 
 * \brief MIIM Command
 *
 * \details
 * Register: \a DEVCPU_GCB:MIIM:MII_CMD
 *
 * @param gi Replicator: x_FFL_DEVCPU_MIIM_CNT (??), 0-2
 */
#define VTSS_DEVCPU_GCB_MIIM_MII_CMD(gi)     VTSS_IOREG_IX(VTSS_TO_DEVCPU_GCB,0x32,gi,9,0,2)

/** 
 * \brief
 * Must be set for starting a new PHY access. This bit is automatically
 * cleared.
 *
 * \details 
 * 0 : Write to this register is ignored.
 * 1 : Write to this register is processed.
 *
 * Field: ::VTSS_DEVCPU_GCB_MIIM_MII_CMD . MIIM_CMD_VLD
 */
#define  VTSS_F_DEVCPU_GCB_MIIM_MII_CMD_MIIM_CMD_VLD(x)  VTSS_ENCODE_BITFIELD(!!(x),31,1)
#define  VTSS_M_DEVCPU_GCB_MIIM_MII_CMD_MIIM_CMD_VLD  VTSS_BIT(31)
#define  VTSS_X_DEVCPU_GCB_MIIM_MII_CMD_MIIM_CMD_VLD(x)  VTSS_EXTRACT_BITFIELD(x,31,1)

/** 
 * \brief
 * Indicates the addressed PHY number.
 *
 * \details 
 * Field: ::VTSS_DEVCPU_GCB_MIIM_MII_CMD . MIIM_CMD_PHYAD
 */
#define  VTSS_F_DEVCPU_GCB_MIIM_MII_CMD_MIIM_CMD_PHYAD(x)  VTSS_ENCODE_BITFIELD(x,25,5)
#define  VTSS_M_DEVCPU_GCB_MIIM_MII_CMD_MIIM_CMD_PHYAD     VTSS_ENCODE_BITMASK(25,5)
#define  VTSS_X_DEVCPU_GCB_MIIM_MII_CMD_MIIM_CMD_PHYAD(x)  VTSS_EXTRACT_BITFIELD(x,25,5)

/** 
 * \brief
 * Indicates the addressed of the register within the PHY that shall be
 * accessed.
 *
 * \details 
 * Field: ::VTSS_DEVCPU_GCB_MIIM_MII_CMD . MIIM_CMD_REGAD
 */
#define  VTSS_F_DEVCPU_GCB_MIIM_MII_CMD_MIIM_CMD_REGAD(x)  VTSS_ENCODE_BITFIELD(x,20,5)
#define  VTSS_M_DEVCPU_GCB_MIIM_MII_CMD_MIIM_CMD_REGAD     VTSS_ENCODE_BITMASK(20,5)
#define  VTSS_X_DEVCPU_GCB_MIIM_MII_CMD_MIIM_CMD_REGAD(x)  VTSS_EXTRACT_BITFIELD(x,20,5)

/** 
 * \brief
 * Data to be written in the PHY register.
 *
 * \details 
 * Field: ::VTSS_DEVCPU_GCB_MIIM_MII_CMD . MIIM_CMD_WRDATA
 */
#define  VTSS_F_DEVCPU_GCB_MIIM_MII_CMD_MIIM_CMD_WRDATA(x)  VTSS_ENCODE_BITFIELD(x,4,16)
#define  VTSS_M_DEVCPU_GCB_MIIM_MII_CMD_MIIM_CMD_WRDATA     VTSS_ENCODE_BITMASK(4,16)
#define  VTSS_X_DEVCPU_GCB_MIIM_MII_CMD_MIIM_CMD_WRDATA(x)  VTSS_EXTRACT_BITFIELD(x,4,16)

/** 
 * \brief
 * Select if scanning of the PHY shall be done once, or scanning should be
 * done continuously.
 *
 * \details 
 * 0 : Do continuously PHY scanning 
 * 1 : Stop once all PHY have been scanned.
 *
 * Field: ::VTSS_DEVCPU_GCB_MIIM_MII_CMD . MIIM_CMD_SINGLE_SCAN
 */
#define  VTSS_F_DEVCPU_GCB_MIIM_MII_CMD_MIIM_CMD_SINGLE_SCAN(x)  VTSS_ENCODE_BITFIELD(!!(x),3,1)
#define  VTSS_M_DEVCPU_GCB_MIIM_MII_CMD_MIIM_CMD_SINGLE_SCAN  VTSS_BIT(3)
#define  VTSS_X_DEVCPU_GCB_MIIM_MII_CMD_MIIM_CMD_SINGLE_SCAN(x)  VTSS_EXTRACT_BITFIELD(x,3,1)

/** 
 * \brief
 * Indicates type of operation.
 *
 * \details 
 * Clause 22:
 * 
 * 01 : Write
 * 10 : Read
 * 
 * Clause 45:
 * 
 * 00 : Address
 * 01 : Write
 * 10 : Read inc.
 * 11 : Read.
 *
 * Field: ::VTSS_DEVCPU_GCB_MIIM_MII_CMD . MIIM_CMD_OPR_FIELD
 */
#define  VTSS_F_DEVCPU_GCB_MIIM_MII_CMD_MIIM_CMD_OPR_FIELD(x)  VTSS_ENCODE_BITFIELD(x,1,2)
#define  VTSS_M_DEVCPU_GCB_MIIM_MII_CMD_MIIM_CMD_OPR_FIELD     VTSS_ENCODE_BITMASK(1,2)
#define  VTSS_X_DEVCPU_GCB_MIIM_MII_CMD_MIIM_CMD_OPR_FIELD(x)  VTSS_EXTRACT_BITFIELD(x,1,2)

/** 
 * \brief
 * Indicates whether automatic scanning of PHY registers is enabled. When
 * enabled, the PHY-number for each automatic read is continuously
 * round-robined from PHY_ADDR_LOW through PHY_ADDR_HIGH. This function is
 * started upon a read operation (ACCESS_TYPE).
 * Scan MUST be disabled when doing any configuration of the MIIM
 * controller.
 *
 * \details 
 * 0 : Disabled
 * 1 : Enabled.
 *
 * Field: ::VTSS_DEVCPU_GCB_MIIM_MII_CMD . MIIM_CMD_SCAN
 */
#define  VTSS_F_DEVCPU_GCB_MIIM_MII_CMD_MIIM_CMD_SCAN(x)  VTSS_ENCODE_BITFIELD(!!(x),0,1)
#define  VTSS_M_DEVCPU_GCB_MIIM_MII_CMD_MIIM_CMD_SCAN  VTSS_BIT(0)
#define  VTSS_X_DEVCPU_GCB_MIIM_MII_CMD_MIIM_CMD_SCAN(x)  VTSS_EXTRACT_BITFIELD(x,0,1)


/** 
 * \brief MIIM Reply Data
 *
 * \details
 * Register: \a DEVCPU_GCB:MIIM:MII_DATA
 *
 * @param gi Replicator: x_FFL_DEVCPU_MIIM_CNT (??), 0-2
 */
#define VTSS_DEVCPU_GCB_MIIM_MII_DATA(gi)    VTSS_IOREG_IX(VTSS_TO_DEVCPU_GCB,0x32,gi,9,0,3)

/** 
 * \brief
 * Indicates whether a read operation failed or succeeded.
 *
 * \details 
 * 00 : OK
 * 11 : Error
 *
 * Field: ::VTSS_DEVCPU_GCB_MIIM_MII_DATA . MIIM_DATA_SUCCESS
 */
#define  VTSS_F_DEVCPU_GCB_MIIM_MII_DATA_MIIM_DATA_SUCCESS(x)  VTSS_ENCODE_BITFIELD(x,16,2)
#define  VTSS_M_DEVCPU_GCB_MIIM_MII_DATA_MIIM_DATA_SUCCESS     VTSS_ENCODE_BITMASK(16,2)
#define  VTSS_X_DEVCPU_GCB_MIIM_MII_DATA_MIIM_DATA_SUCCESS(x)  VTSS_EXTRACT_BITFIELD(x,16,2)

/** 
 * \brief
 * Data read from PHY register.
 *
 * \details 
 * Field: ::VTSS_DEVCPU_GCB_MIIM_MII_DATA . MIIM_DATA_RDDATA
 */
#define  VTSS_F_DEVCPU_GCB_MIIM_MII_DATA_MIIM_DATA_RDDATA(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_DEVCPU_GCB_MIIM_MII_DATA_MIIM_DATA_RDDATA     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_DEVCPU_GCB_MIIM_MII_DATA_MIIM_DATA_RDDATA(x)  VTSS_EXTRACT_BITFIELD(x,0,16)


/** 
 * \brief MIIM Configuration
 *
 * \details
 * Register: \a DEVCPU_GCB:MIIM:MII_CFG
 *
 * @param gi Replicator: x_FFL_DEVCPU_MIIM_CNT (??), 0-2
 */
#define VTSS_DEVCPU_GCB_MIIM_MII_CFG(gi)     VTSS_IOREG_IX(VTSS_TO_DEVCPU_GCB,0x32,gi,9,0,4)

/** 
 * \brief
 * Option to disable deadlock fix if it causes any troubles.
 *
 * \details 
 * Field: ::VTSS_DEVCPU_GCB_MIIM_MII_CFG . DEADLOCK_FIX_DIS
 */
#define  VTSS_F_DEVCPU_GCB_MIIM_MII_CFG_DEADLOCK_FIX_DIS(x)  VTSS_ENCODE_BITFIELD(!!(x),11,1)
#define  VTSS_M_DEVCPU_GCB_MIIM_MII_CFG_DEADLOCK_FIX_DIS  VTSS_BIT(11)
#define  VTSS_X_DEVCPU_GCB_MIIM_MII_CFG_DEADLOCK_FIX_DIS(x)  VTSS_EXTRACT_BITFIELD(x,11,1)

/** 
 * \brief
 * The ST (start-of-frame) field of the MIIM frame format adopts the value
 * of this field. This must be configured for either clause 22 or 45 MIIM
 * operation.
 *
 * \details 
 * "01": Clause 22
 * "00": Clause 45
 * Other values are reserved.
 *
 * Field: ::VTSS_DEVCPU_GCB_MIIM_MII_CFG . MIIM_ST_CFG_FIELD
 */
#define  VTSS_F_DEVCPU_GCB_MIIM_MII_CFG_MIIM_ST_CFG_FIELD(x)  VTSS_ENCODE_BITFIELD(x,9,2)
#define  VTSS_M_DEVCPU_GCB_MIIM_MII_CFG_MIIM_ST_CFG_FIELD     VTSS_ENCODE_BITMASK(9,2)
#define  VTSS_X_DEVCPU_GCB_MIIM_MII_CFG_MIIM_ST_CFG_FIELD(x)  VTSS_EXTRACT_BITFIELD(x,9,2)

/** 
 * \brief
 * Selects the interpretation of the BUSY_OR_IN_PROGRESS field in
 * MII_STATUS.
 *
 * \details 
 * Field: ::VTSS_DEVCPU_GCB_MIIM_MII_CFG . MIIM_CFG_DBG
 */
#define  VTSS_F_DEVCPU_GCB_MIIM_MII_CFG_MIIM_CFG_DBG(x)  VTSS_ENCODE_BITFIELD(!!(x),8,1)
#define  VTSS_M_DEVCPU_GCB_MIIM_MII_CFG_MIIM_CFG_DBG  VTSS_BIT(8)
#define  VTSS_X_DEVCPU_GCB_MIIM_MII_CFG_MIIM_CFG_DBG(x)  VTSS_EXTRACT_BITFIELD(x,8,1)

/** 
 * \brief
 * Configures the MIIM clock frequency. This is computed as
 * system_clk/(2*(1+X)), where X is the value written to this register.
 * Note : Setting X to 0 is invalid and will result in the same frequency
 * as setting X to 1.
 *
 * \details 
 * Field: ::VTSS_DEVCPU_GCB_MIIM_MII_CFG . MIIM_CFG_PRESCALE
 */
#define  VTSS_F_DEVCPU_GCB_MIIM_MII_CFG_MIIM_CFG_PRESCALE(x)  VTSS_ENCODE_BITFIELD(x,0,8)
#define  VTSS_M_DEVCPU_GCB_MIIM_MII_CFG_MIIM_CFG_PRESCALE     VTSS_ENCODE_BITMASK(0,8)
#define  VTSS_X_DEVCPU_GCB_MIIM_MII_CFG_MIIM_CFG_PRESCALE(x)  VTSS_EXTRACT_BITFIELD(x,0,8)


/** 
 * \brief MIIM Scan 0
 *
 * \details
 * Register: \a DEVCPU_GCB:MIIM:MII_SCAN_0
 *
 * @param gi Replicator: x_FFL_DEVCPU_MIIM_CNT (??), 0-2
 */
#define VTSS_DEVCPU_GCB_MIIM_MII_SCAN_0(gi)  VTSS_IOREG_IX(VTSS_TO_DEVCPU_GCB,0x32,gi,9,0,5)

/** 
 * \brief
 * Indicates the high PHY number to scan during automatic scanning.
 *
 * \details 
 * Field: ::VTSS_DEVCPU_GCB_MIIM_MII_SCAN_0 . MIIM_SCAN_PHYADHI
 */
#define  VTSS_F_DEVCPU_GCB_MIIM_MII_SCAN_0_MIIM_SCAN_PHYADHI(x)  VTSS_ENCODE_BITFIELD(x,5,5)
#define  VTSS_M_DEVCPU_GCB_MIIM_MII_SCAN_0_MIIM_SCAN_PHYADHI     VTSS_ENCODE_BITMASK(5,5)
#define  VTSS_X_DEVCPU_GCB_MIIM_MII_SCAN_0_MIIM_SCAN_PHYADHI(x)  VTSS_EXTRACT_BITFIELD(x,5,5)

/** 
 * \brief
 * Indicates the low PHY number to scan during automatic scanning.
 *
 * \details 
 * Field: ::VTSS_DEVCPU_GCB_MIIM_MII_SCAN_0 . MIIM_SCAN_PHYADLO
 */
#define  VTSS_F_DEVCPU_GCB_MIIM_MII_SCAN_0_MIIM_SCAN_PHYADLO(x)  VTSS_ENCODE_BITFIELD(x,0,5)
#define  VTSS_M_DEVCPU_GCB_MIIM_MII_SCAN_0_MIIM_SCAN_PHYADLO     VTSS_ENCODE_BITMASK(0,5)
#define  VTSS_X_DEVCPU_GCB_MIIM_MII_SCAN_0_MIIM_SCAN_PHYADLO(x)  VTSS_EXTRACT_BITFIELD(x,0,5)


/** 
 * \brief MIIM Scan 1
 *
 * \details
 * Register: \a DEVCPU_GCB:MIIM:MII_SCAN_1
 *
 * @param gi Replicator: x_FFL_DEVCPU_MIIM_CNT (??), 0-2
 */
#define VTSS_DEVCPU_GCB_MIIM_MII_SCAN_1(gi)  VTSS_IOREG_IX(VTSS_TO_DEVCPU_GCB,0x32,gi,9,0,6)

/** 
 * \brief
 * Indicates the mask for comparing the PHY registers during automatic
 * scan.
 *
 * \details 
 * Field: ::VTSS_DEVCPU_GCB_MIIM_MII_SCAN_1 . MIIM_SCAN_MASK
 */
#define  VTSS_F_DEVCPU_GCB_MIIM_MII_SCAN_1_MIIM_SCAN_MASK(x)  VTSS_ENCODE_BITFIELD(x,16,16)
#define  VTSS_M_DEVCPU_GCB_MIIM_MII_SCAN_1_MIIM_SCAN_MASK     VTSS_ENCODE_BITMASK(16,16)
#define  VTSS_X_DEVCPU_GCB_MIIM_MII_SCAN_1_MIIM_SCAN_MASK(x)  VTSS_EXTRACT_BITFIELD(x,16,16)

/** 
 * \brief
 * Indicates the expected value for comparing the PHY registers during
 * automatic scan.
 *
 * \details 
 * Field: ::VTSS_DEVCPU_GCB_MIIM_MII_SCAN_1 . MIIM_SCAN_EXPECT
 */
#define  VTSS_F_DEVCPU_GCB_MIIM_MII_SCAN_1_MIIM_SCAN_EXPECT(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_DEVCPU_GCB_MIIM_MII_SCAN_1_MIIM_SCAN_EXPECT     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_DEVCPU_GCB_MIIM_MII_SCAN_1_MIIM_SCAN_EXPECT(x)  VTSS_EXTRACT_BITFIELD(x,0,16)


/** 
 * \brief MIIM Results
 *
 * \details
 * Register: \a DEVCPU_GCB:MIIM:MII_SCAN_LAST_RSLTS
 *
 * @param gi Replicator: x_FFL_DEVCPU_MIIM_CNT (??), 0-2
 */
#define VTSS_DEVCPU_GCB_MIIM_MII_SCAN_LAST_RSLTS(gi)  VTSS_IOREG_IX(VTSS_TO_DEVCPU_GCB,0x32,gi,9,0,7)


/** 
 * \brief MIIM Results
 *
 * \details
 * Register: \a DEVCPU_GCB:MIIM:MII_SCAN_LAST_RSLTS_VLD
 *
 * @param gi Replicator: x_FFL_DEVCPU_MIIM_CNT (??), 0-2
 */
#define VTSS_DEVCPU_GCB_MIIM_MII_SCAN_LAST_RSLTS_VLD(gi)  VTSS_IOREG_IX(VTSS_TO_DEVCPU_GCB,0x32,gi,9,0,8)

/**
 * Register Group: \a DEVCPU_GCB:MIIM_READ_SCAN
 *
 * Not documented
 */


/** 
 * \brief MIIM Results
 *
 * \details
 * Register: \a DEVCPU_GCB:MIIM_READ_SCAN:MII_SCAN_RSLTS_STICKY
 *
 * @param ri Replicator: x_FFL_DEVCPU_MIIM_CNT (??), 0-2
 */
#define VTSS_DEVCPU_GCB_MIIM_READ_SCAN_MII_SCAN_RSLTS_STICKY(ri)  VTSS_IOREG(VTSS_TO_DEVCPU_GCB,0x4d + (ri))

/**
 * Register Group: \a DEVCPU_GCB:TEMP_SENSOR
 *
 * Temperature sensor control
 */


/** 
 * \brief Temperature Sensor Control
 *
 * \details
 * Register: \a DEVCPU_GCB:TEMP_SENSOR:TEMP_SENSOR_CTRL
 */
#define VTSS_DEVCPU_GCB_TEMP_SENSOR_TEMP_SENSOR_CTRL  VTSS_IOREG(VTSS_TO_DEVCPU_GCB,0x50)

/** 
 * \brief
 * Set to force reading of temperature irregardless of when Temperature
 * sensor says done.
 *
 * \details 
 * Field: ::VTSS_DEVCPU_GCB_TEMP_SENSOR_TEMP_SENSOR_CTRL . FORCE_TEMP_RD
 */
#define  VTSS_F_DEVCPU_GCB_TEMP_SENSOR_TEMP_SENSOR_CTRL_FORCE_TEMP_RD(x)  VTSS_ENCODE_BITFIELD(!!(x),5,1)
#define  VTSS_M_DEVCPU_GCB_TEMP_SENSOR_TEMP_SENSOR_CTRL_FORCE_TEMP_RD  VTSS_BIT(5)
#define  VTSS_X_DEVCPU_GCB_TEMP_SENSOR_TEMP_SENSOR_CTRL_FORCE_TEMP_RD(x)  VTSS_EXTRACT_BITFIELD(x,5,1)

/** 
 * \brief
 * Set to force RUN signal towards temperature sensor. This field only
 * works when SAMPLE_ENA is cleared.
 *
 * \details 
 * Field: ::VTSS_DEVCPU_GCB_TEMP_SENSOR_TEMP_SENSOR_CTRL . FORCE_RUN
 */
#define  VTSS_F_DEVCPU_GCB_TEMP_SENSOR_TEMP_SENSOR_CTRL_FORCE_RUN(x)  VTSS_ENCODE_BITFIELD(!!(x),4,1)
#define  VTSS_M_DEVCPU_GCB_TEMP_SENSOR_TEMP_SENSOR_CTRL_FORCE_RUN  VTSS_BIT(4)
#define  VTSS_X_DEVCPU_GCB_TEMP_SENSOR_TEMP_SENSOR_CTRL_FORCE_RUN(x)  VTSS_EXTRACT_BITFIELD(x,4,1)

/** 
 * \brief
 * Set to force RESETN signal towards temperature sensor (release of
 * reset). This field only works when SAMPLE_ENA is cleared.
 *
 * \details 
 * Field: ::VTSS_DEVCPU_GCB_TEMP_SENSOR_TEMP_SENSOR_CTRL . FORCE_NO_RST
 */
#define  VTSS_F_DEVCPU_GCB_TEMP_SENSOR_TEMP_SENSOR_CTRL_FORCE_NO_RST(x)  VTSS_ENCODE_BITFIELD(!!(x),3,1)
#define  VTSS_M_DEVCPU_GCB_TEMP_SENSOR_TEMP_SENSOR_CTRL_FORCE_NO_RST  VTSS_BIT(3)
#define  VTSS_X_DEVCPU_GCB_TEMP_SENSOR_TEMP_SENSOR_CTRL_FORCE_NO_RST(x)  VTSS_EXTRACT_BITFIELD(x,3,1)

/** 
 * \brief
 * Set to force PDB signal towards temperature sensor. This field only
 * works when SAMPLE_ENA is cleared.
 *
 * \details 
 * Field: ::VTSS_DEVCPU_GCB_TEMP_SENSOR_TEMP_SENSOR_CTRL . FORCE_POWER_UP
 */
#define  VTSS_F_DEVCPU_GCB_TEMP_SENSOR_TEMP_SENSOR_CTRL_FORCE_POWER_UP(x)  VTSS_ENCODE_BITFIELD(!!(x),2,1)
#define  VTSS_M_DEVCPU_GCB_TEMP_SENSOR_TEMP_SENSOR_CTRL_FORCE_POWER_UP  VTSS_BIT(2)
#define  VTSS_X_DEVCPU_GCB_TEMP_SENSOR_TEMP_SENSOR_CTRL_FORCE_POWER_UP(x)  VTSS_EXTRACT_BITFIELD(x,2,1)

/** 
 * \brief
 * Set to force clock signal towards temperature sensor. This field only
 * works when SAMPLE_ENA is cleared.
 *
 * \details 
 * Field: ::VTSS_DEVCPU_GCB_TEMP_SENSOR_TEMP_SENSOR_CTRL . FORCE_CLK
 */
#define  VTSS_F_DEVCPU_GCB_TEMP_SENSOR_TEMP_SENSOR_CTRL_FORCE_CLK(x)  VTSS_ENCODE_BITFIELD(!!(x),1,1)
#define  VTSS_M_DEVCPU_GCB_TEMP_SENSOR_TEMP_SENSOR_CTRL_FORCE_CLK  VTSS_BIT(1)
#define  VTSS_X_DEVCPU_GCB_TEMP_SENSOR_TEMP_SENSOR_CTRL_FORCE_CLK(x)  VTSS_EXTRACT_BITFIELD(x,1,1)

/** 
 * \brief
 * Set this field to enable sampling of temperature. Approximately 500us
 * after setting this field DEVCPU_GCB::TEMP_SENSOR_STAT.TEMP_VALID will be
 * set together with a valid temperature value. After this the temperature
 * will be updated every 500us for as long as this field remains set. Clear
 * ths field to disable temperature sensor.
 *
 * \details 
 * Field: ::VTSS_DEVCPU_GCB_TEMP_SENSOR_TEMP_SENSOR_CTRL . SAMPLE_ENA
 */
#define  VTSS_F_DEVCPU_GCB_TEMP_SENSOR_TEMP_SENSOR_CTRL_SAMPLE_ENA(x)  VTSS_ENCODE_BITFIELD(!!(x),0,1)
#define  VTSS_M_DEVCPU_GCB_TEMP_SENSOR_TEMP_SENSOR_CTRL_SAMPLE_ENA  VTSS_BIT(0)
#define  VTSS_X_DEVCPU_GCB_TEMP_SENSOR_TEMP_SENSOR_CTRL_SAMPLE_ENA(x)  VTSS_EXTRACT_BITFIELD(x,0,1)


/** 
 * \brief Temperature sensor sample period
 *
 * \details
 * Register: \a DEVCPU_GCB:TEMP_SENSOR:TEMP_SENSOR_CFG
 */
#define VTSS_DEVCPU_GCB_TEMP_SENSOR_TEMP_SENSOR_CFG  VTSS_IOREG(VTSS_TO_DEVCPU_GCB,0x51)

/** 
 * \brief
 * The time to keep RUN asserted during a sample period, time is in 1024
 * SwC clock cycles.
 *
 * \details 
 * 0: Reserved
 * n: n * 1024 * clock-period between samples
 *
 * Field: ::VTSS_DEVCPU_GCB_TEMP_SENSOR_TEMP_SENSOR_CFG . RUN_WID
 */
#define  VTSS_F_DEVCPU_GCB_TEMP_SENSOR_TEMP_SENSOR_CFG_RUN_WID(x)  VTSS_ENCODE_BITFIELD(x,8,8)
#define  VTSS_M_DEVCPU_GCB_TEMP_SENSOR_TEMP_SENSOR_CFG_RUN_WID     VTSS_ENCODE_BITMASK(8,8)
#define  VTSS_X_DEVCPU_GCB_TEMP_SENSOR_TEMP_SENSOR_CFG_RUN_WID(x)  VTSS_EXTRACT_BITFIELD(x,8,8)

/** 
 * \brief
 * The delay between temperature sensor samples in 1024 SwC clock cycles.
 * The default value corresponds to 500us. The delay have to be big enough
 * to allow the temperature sensor to complete its sample.
 *
 * \details 
 * 0: Reserved
 * n: n * 1024 * clock-period between samples
 *
 * Field: ::VTSS_DEVCPU_GCB_TEMP_SENSOR_TEMP_SENSOR_CFG . SAMPLE_PER
 */
#define  VTSS_F_DEVCPU_GCB_TEMP_SENSOR_TEMP_SENSOR_CFG_SAMPLE_PER(x)  VTSS_ENCODE_BITFIELD(x,0,8)
#define  VTSS_M_DEVCPU_GCB_TEMP_SENSOR_TEMP_SENSOR_CFG_SAMPLE_PER     VTSS_ENCODE_BITMASK(0,8)
#define  VTSS_X_DEVCPU_GCB_TEMP_SENSOR_TEMP_SENSOR_CFG_SAMPLE_PER(x)  VTSS_EXTRACT_BITFIELD(x,0,8)


/** 
 * \brief Temperature Sensor Status
 *
 * \details
 * Register: \a DEVCPU_GCB:TEMP_SENSOR:TEMP_SENSOR_STAT
 */
#define VTSS_DEVCPU_GCB_TEMP_SENSOR_TEMP_SENSOR_STAT  VTSS_IOREG(VTSS_TO_DEVCPU_GCB,0x52)

/** 
 * \brief
 * This fied is set when valid temperature data is available in
 * DEVCPU_GCB::TEMP_SENSOR_STAT.TEMP.
 *
 * \details 
 * Field: ::VTSS_DEVCPU_GCB_TEMP_SENSOR_TEMP_SENSOR_STAT . TEMP_VALID
 */
#define  VTSS_F_DEVCPU_GCB_TEMP_SENSOR_TEMP_SENSOR_STAT_TEMP_VALID(x)  VTSS_ENCODE_BITFIELD(!!(x),8,1)
#define  VTSS_M_DEVCPU_GCB_TEMP_SENSOR_TEMP_SENSOR_STAT_TEMP_VALID  VTSS_BIT(8)
#define  VTSS_X_DEVCPU_GCB_TEMP_SENSOR_TEMP_SENSOR_STAT_TEMP_VALID(x)  VTSS_EXTRACT_BITFIELD(x,8,1)

/** 
 * \brief
 * Temperature data readout, this field is valid when
 * DEVCPU_GCB::TEMP_SENSOR_STAT.TEMP_VALID is set. This field is
 * continually updated (approximately every 500us) while the termperature
 * sensor is enabled, see DEVCPU_GCB::TEMP_SENSOR_CTRL.SAMPLE_ENA for more
 * information.
 *
 * \details 
 * Temperature(C)=177.4 - 0.8777*DATA
 *
 * Field: ::VTSS_DEVCPU_GCB_TEMP_SENSOR_TEMP_SENSOR_STAT . TEMP
 */
#define  VTSS_F_DEVCPU_GCB_TEMP_SENSOR_TEMP_SENSOR_STAT_TEMP(x)  VTSS_ENCODE_BITFIELD(x,0,8)
#define  VTSS_M_DEVCPU_GCB_TEMP_SENSOR_TEMP_SENSOR_STAT_TEMP     VTSS_ENCODE_BITMASK(0,8)
#define  VTSS_X_DEVCPU_GCB_TEMP_SENSOR_TEMP_SENSOR_STAT_TEMP(x)  VTSS_EXTRACT_BITFIELD(x,0,8)

/**
 * Register Group: \a DEVCPU_GCB:MIIM_SLAVE
 *
 * Not documented
 */


/** 
 * \brief MIIM Slave Spike Filter
 *
 * \details
 * Register: \a DEVCPU_GCB:MIIM_SLAVE:MIIM_SLAVE_CFG
 */
#define VTSS_DEVCPU_GCB_MIIM_SLAVE_MIIM_SLAVE_CFG  VTSS_IOREG(VTSS_TO_DEVCPU_GCB,0x53)

/** 
 * \brief
 * Configuration of the spike filter width on the MDC and MDIO inputs.
 * Filters spikes with a width of (SPIKE_FILTER_CFG+1)*SYSTEM_CLK or less.
 *
 * \details 
 * Field: ::VTSS_DEVCPU_GCB_MIIM_SLAVE_MIIM_SLAVE_CFG . SPIKE_FILTER_CFG
 */
#define  VTSS_F_DEVCPU_GCB_MIIM_SLAVE_MIIM_SLAVE_CFG_SPIKE_FILTER_CFG(x)  VTSS_ENCODE_BITFIELD(x,1,5)
#define  VTSS_M_DEVCPU_GCB_MIIM_SLAVE_MIIM_SLAVE_CFG_SPIKE_FILTER_CFG     VTSS_ENCODE_BITMASK(1,5)
#define  VTSS_X_DEVCPU_GCB_MIIM_SLAVE_MIIM_SLAVE_CFG_SPIKE_FILTER_CFG(x)  VTSS_EXTRACT_BITFIELD(x,1,5)

/** 
 * \brief
 * Set this field to enable the spike filter on the MDC and MDIO inputs.
 * When enabled the MIIM_SLAVE_CFG.SPIKE_FILTER_CFG field determines the
 * width of the spike filter.
 *
 * \details 
 * Field: ::VTSS_DEVCPU_GCB_MIIM_SLAVE_MIIM_SLAVE_CFG . SPIKE_FILTER_ENA
 */
#define  VTSS_F_DEVCPU_GCB_MIIM_SLAVE_MIIM_SLAVE_CFG_SPIKE_FILTER_ENA(x)  VTSS_ENCODE_BITFIELD(!!(x),0,1)
#define  VTSS_M_DEVCPU_GCB_MIIM_SLAVE_MIIM_SLAVE_CFG_SPIKE_FILTER_ENA  VTSS_BIT(0)
#define  VTSS_X_DEVCPU_GCB_MIIM_SLAVE_MIIM_SLAVE_CFG_SPIKE_FILTER_ENA(x)  VTSS_EXTRACT_BITFIELD(x,0,1)

/**
 * Register Group: \a DEVCPU_GCB:SIO_CTRL
 *
 * Serial IO control configuration
 */


/** 
 * \brief Last value at SGPIO pins
 *
 * \details
 * Register: \a DEVCPU_GCB:SIO_CTRL:SIO_INPUT_DATA
 *
 * @param gi Replicator: x_FFL_DEVCPU_SIO_CTRL_CNT (??), 0-2
 * @param ri Replicator: x_FFL_DEVCPU_SIO_IOS_PR_PORT (??), 0-3
 */
#define VTSS_DEVCPU_GCB_SIO_CTRL_SIO_INPUT_DATA(gi,ri)  VTSS_IOREG_IX(VTSS_TO_DEVCPU_GCB,0x54,gi,67,ri,0)


/** 
 * \brief General configurations
 *
 * \details
 * Register: \a DEVCPU_GCB:SIO_CTRL:SIO_CFG
 *
 * @param gi Replicator: x_FFL_DEVCPU_SIO_CTRL_CNT (??), 0-2
 */
#define VTSS_DEVCPU_GCB_SIO_CTRL_SIO_CFG(gi)  VTSS_IOREG_IX(VTSS_TO_DEVCPU_GCB,0x54,gi,67,0,4)

/** 
 * \brief
 * Configuration for blink mode 1.
 * Supports three different blink modes and a "burst toggle" mode in which
 * blink mode 1 will alternate for each burst.
 *
 * \details 
 * 0: Blink freq appr. 20Hz
 * 1: Blink freq appr. 10Hz
 * 2: Blink freq appr. 5Hz
 * 3: Burst toggle
 *
 * Field: ::VTSS_DEVCPU_GCB_SIO_CTRL_SIO_CFG . SIO_BMODE_1
 */
#define  VTSS_F_DEVCPU_GCB_SIO_CTRL_SIO_CFG_SIO_BMODE_1(x)  VTSS_ENCODE_BITFIELD(x,21,2)
#define  VTSS_M_DEVCPU_GCB_SIO_CTRL_SIO_CFG_SIO_BMODE_1     VTSS_ENCODE_BITMASK(21,2)
#define  VTSS_X_DEVCPU_GCB_SIO_CTRL_SIO_CFG_SIO_BMODE_1(x)  VTSS_EXTRACT_BITFIELD(x,21,2)

/** 
 * \brief
 * Configuration of blink mode 0. Supports four different blink modes.
 *
 * \details 
 * 0 : Blink freq appr. 5Hz.
 * 1 : Blink freq appr. 2.5Hz.
 * 2 : Blink freq appr. 1.25Hz.
 * 3 : Blink freq appr. 0.625Hz.
 *
 * Field: ::VTSS_DEVCPU_GCB_SIO_CTRL_SIO_CFG . SIO_BMODE_0
 */
#define  VTSS_F_DEVCPU_GCB_SIO_CTRL_SIO_CFG_SIO_BMODE_0(x)  VTSS_ENCODE_BITFIELD(x,19,2)
#define  VTSS_M_DEVCPU_GCB_SIO_CTRL_SIO_CFG_SIO_BMODE_0     VTSS_ENCODE_BITMASK(19,2)
#define  VTSS_X_DEVCPU_GCB_SIO_CTRL_SIO_CFG_SIO_BMODE_0(x)  VTSS_EXTRACT_BITFIELD(x,19,2)

/** 
 * \brief
 * Reset the blink counters. Used to synchronize the blink modes between
 * different chips.
 *
 * \details 
 * 0: Blink counter is running
 * 1: Blink counter is reset until sio_blink_reset is unset again
 *
 * Field: ::VTSS_DEVCPU_GCB_SIO_CTRL_SIO_CFG . SIO_BLINK_RESET
 */
#define  VTSS_F_DEVCPU_GCB_SIO_CTRL_SIO_CFG_SIO_BLINK_RESET(x)  VTSS_ENCODE_BITFIELD(!!(x),18,1)
#define  VTSS_M_DEVCPU_GCB_SIO_CTRL_SIO_CFG_SIO_BLINK_RESET  VTSS_BIT(18)
#define  VTSS_X_DEVCPU_GCB_SIO_CTRL_SIO_CFG_SIO_BLINK_RESET(x)  VTSS_EXTRACT_BITFIELD(x,18,1)

/** 
 * \brief
 * Set to disable burst gap.
 *
 * \details 
 * Field: ::VTSS_DEVCPU_GCB_SIO_CTRL_SIO_CFG . SIO_BURST_GAP_DIS
 */
#define  VTSS_F_DEVCPU_GCB_SIO_CTRL_SIO_CFG_SIO_BURST_GAP_DIS(x)  VTSS_ENCODE_BITFIELD(!!(x),17,1)
#define  VTSS_M_DEVCPU_GCB_SIO_CTRL_SIO_CFG_SIO_BURST_GAP_DIS  VTSS_BIT(17)
#define  VTSS_X_DEVCPU_GCB_SIO_CTRL_SIO_CFG_SIO_BURST_GAP_DIS(x)  VTSS_EXTRACT_BITFIELD(x,17,1)

/** 
 * \brief
 * Configures the length of burst gap in steps of approx. 1 ms. Burst gap
 * can be disabled by setting DEVCPU_GCB::SIO_CFG.SIO_BURST_GAP_DIS.
 *
 * \details 
 * 0: 1.05 ms burst gap
 * 1: 2.10 ms burst gap
 * ...
 * 31: 33.55 ms burst gap
 *
 * Field: ::VTSS_DEVCPU_GCB_SIO_CTRL_SIO_CFG . SIO_BURST_GAP
 */
#define  VTSS_F_DEVCPU_GCB_SIO_CTRL_SIO_CFG_SIO_BURST_GAP(x)  VTSS_ENCODE_BITFIELD(x,12,5)
#define  VTSS_M_DEVCPU_GCB_SIO_CTRL_SIO_CFG_SIO_BURST_GAP     VTSS_ENCODE_BITMASK(12,5)
#define  VTSS_X_DEVCPU_GCB_SIO_CTRL_SIO_CFG_SIO_BURST_GAP(x)  VTSS_EXTRACT_BITFIELD(x,12,5)

/** 
 * \brief
 * Use this to output a single burst. Will be cleared by hardware when the
 * burst has finished.
 *
 * \details 
 * Field: ::VTSS_DEVCPU_GCB_SIO_CTRL_SIO_CFG . SIO_SINGLE_SHOT
 */
#define  VTSS_F_DEVCPU_GCB_SIO_CTRL_SIO_CFG_SIO_SINGLE_SHOT(x)  VTSS_ENCODE_BITFIELD(!!(x),11,1)
#define  VTSS_M_DEVCPU_GCB_SIO_CTRL_SIO_CFG_SIO_SINGLE_SHOT  VTSS_BIT(11)
#define  VTSS_X_DEVCPU_GCB_SIO_CTRL_SIO_CFG_SIO_SINGLE_SHOT(x)  VTSS_EXTRACT_BITFIELD(x,11,1)

/** 
 * \brief
 * Use this to output repeated bursts interleaved with burst gaps. Must be
 * manually reset again to stop output of bursts.
 *
 * \details 
 * Field: ::VTSS_DEVCPU_GCB_SIO_CTRL_SIO_CFG . SIO_AUTO_REPEAT
 */
#define  VTSS_F_DEVCPU_GCB_SIO_CTRL_SIO_CFG_SIO_AUTO_REPEAT(x)  VTSS_ENCODE_BITFIELD(!!(x),10,1)
#define  VTSS_M_DEVCPU_GCB_SIO_CTRL_SIO_CFG_SIO_AUTO_REPEAT  VTSS_BIT(10)
#define  VTSS_X_DEVCPU_GCB_SIO_CTRL_SIO_CFG_SIO_AUTO_REPEAT(x)  VTSS_EXTRACT_BITFIELD(x,10,1)

/** 
 * \brief
 * Polarity of the "Ld" signal
 *
 * \details 
 * 0: load signal is active low
 * 1: load signal is active high
 *
 * Field: ::VTSS_DEVCPU_GCB_SIO_CTRL_SIO_CFG . SIO_LD_POLARITY
 */
#define  VTSS_F_DEVCPU_GCB_SIO_CTRL_SIO_CFG_SIO_LD_POLARITY(x)  VTSS_ENCODE_BITFIELD(!!(x),9,1)
#define  VTSS_M_DEVCPU_GCB_SIO_CTRL_SIO_CFG_SIO_LD_POLARITY  VTSS_BIT(9)
#define  VTSS_X_DEVCPU_GCB_SIO_CTRL_SIO_CFG_SIO_LD_POLARITY(x)  VTSS_EXTRACT_BITFIELD(x,9,1)

/** 
 * \brief
 * Number of SGPIOs pr. port.
 *
 * \details 
 * 0: 1 gpio pr. port
 * 1: 2 gpios pr. port
 * 2: 3 gpios pr. port
 * 3: 4 gpios pr. port
 *
 * Field: ::VTSS_DEVCPU_GCB_SIO_CTRL_SIO_CFG . SIO_PORT_WIDTH
 */
#define  VTSS_F_DEVCPU_GCB_SIO_CTRL_SIO_CFG_SIO_PORT_WIDTH(x)  VTSS_ENCODE_BITFIELD(x,7,2)
#define  VTSS_M_DEVCPU_GCB_SIO_CTRL_SIO_CFG_SIO_PORT_WIDTH     VTSS_ENCODE_BITMASK(7,2)
#define  VTSS_X_DEVCPU_GCB_SIO_CTRL_SIO_CFG_SIO_PORT_WIDTH(x)  VTSS_EXTRACT_BITFIELD(x,7,2)

/** 
 * \brief
 * Reverse the output bitstream.
 * The default order of the output bit stream is (displayed in transmitted
 * order):
 * (portN bit3, portN bit2, ...., port0 bit1, port0 bit0)
 * The reverse order of the output bit stream is (displayed in transmitted
 * order):
 * (port0 bit0, port0 bit1, ...., portN bit2, portN bit3)
 *
 * \details 
 * 0: Do not reverse
 * 1: Reverse
 *
 * Field: ::VTSS_DEVCPU_GCB_SIO_CTRL_SIO_CFG . SIO_REVERSE_OUTPUT
 */
#define  VTSS_F_DEVCPU_GCB_SIO_CTRL_SIO_CFG_SIO_REVERSE_OUTPUT(x)  VTSS_ENCODE_BITFIELD(!!(x),6,1)
#define  VTSS_M_DEVCPU_GCB_SIO_CTRL_SIO_CFG_SIO_REVERSE_OUTPUT  VTSS_BIT(6)
#define  VTSS_X_DEVCPU_GCB_SIO_CTRL_SIO_CFG_SIO_REVERSE_OUTPUT(x)  VTSS_EXTRACT_BITFIELD(x,6,1)

/** 
 * \brief
 * Reverse the input bitstream.
 * The default order of the input bit stream is (displayed in received
 * order):
 * (port0 bit0, port0 bit1, ...., portN bit2, portN bit3)
 * The reverse order of the input bit stream is (displayed in received
 * order):
 * (portN bit3, portN bit2, ...., port0 bit1, port0 bit0)
 *
 * \details 
 * 0: Do not reverse
 * 1: Reverse
 *
 * Field: ::VTSS_DEVCPU_GCB_SIO_CTRL_SIO_CFG . SIO_REVERSE_INPUT
 */
#define  VTSS_F_DEVCPU_GCB_SIO_CTRL_SIO_CFG_SIO_REVERSE_INPUT(x)  VTSS_ENCODE_BITFIELD(!!(x),5,1)
#define  VTSS_M_DEVCPU_GCB_SIO_CTRL_SIO_CFG_SIO_REVERSE_INPUT  VTSS_BIT(5)
#define  VTSS_X_DEVCPU_GCB_SIO_CTRL_SIO_CFG_SIO_REVERSE_INPUT(x)  VTSS_EXTRACT_BITFIELD(x,5,1)

/** 
 * \brief
 * Bit interrupt enable. Enables interrupts for the four gpios in a port.
 * Is applied to all ports.
 *
 * \details 
 * 0: Interrupt is disabled for bit n for all ports
 * 1: Interrupt is enabled for bit n for all ports
 *
 * Field: ::VTSS_DEVCPU_GCB_SIO_CTRL_SIO_CFG . SIO_GPIO_INTR_ENA
 */
#define  VTSS_F_DEVCPU_GCB_SIO_CTRL_SIO_CFG_SIO_GPIO_INTR_ENA(x)  VTSS_ENCODE_BITFIELD(x,1,4)
#define  VTSS_M_DEVCPU_GCB_SIO_CTRL_SIO_CFG_SIO_GPIO_INTR_ENA     VTSS_ENCODE_BITMASK(1,4)
#define  VTSS_X_DEVCPU_GCB_SIO_CTRL_SIO_CFG_SIO_GPIO_INTR_ENA(x)  VTSS_EXTRACT_BITFIELD(x,1,4)

/** 
 * \brief
 * Master interrupt enable. Enables interrupts. Is applied to all gpios on
 * all ports. This field only has effect on the interrupt output signal of
 * the sio_ctrl block. It has no effect on the interrupt registers.
 *
 * \details 
 * 0: Interrupt is disabled for all bits for all ports
 * 1: Interrupt is enabled for all bits for all ports
 *
 * Field: ::VTSS_DEVCPU_GCB_SIO_CTRL_SIO_CFG . SIO_MASTER_INTR_ENA
 */
#define  VTSS_F_DEVCPU_GCB_SIO_CTRL_SIO_CFG_SIO_MASTER_INTR_ENA(x)  VTSS_ENCODE_BITFIELD(!!(x),0,1)
#define  VTSS_M_DEVCPU_GCB_SIO_CTRL_SIO_CFG_SIO_MASTER_INTR_ENA  VTSS_BIT(0)
#define  VTSS_X_DEVCPU_GCB_SIO_CTRL_SIO_CFG_SIO_MASTER_INTR_ENA(x)  VTSS_EXTRACT_BITFIELD(x,0,1)


/** 
 * \brief SGPIO shift clock frequency
 *
 * \details
 * Register: \a DEVCPU_GCB:SIO_CTRL:SIO_CLOCK
 *
 * @param gi Replicator: x_FFL_DEVCPU_SIO_CTRL_CNT (??), 0-2
 */
#define VTSS_DEVCPU_GCB_SIO_CTRL_SIO_CLOCK(gi)  VTSS_IOREG_IX(VTSS_TO_DEVCPU_GCB,0x54,gi,67,0,5)

/** 
 * \brief
 * SIO controller clock frequency. Divides the 250 MHz system clk with the
 * value of this field. E.g. field is set to 10, the output frequency will
 * be 25 MHz.
 *
 * \details 
 * 0: Disable clock
 * 1: Reserved, do not use
 * Others : Clock divider value.
 *
 * Field: ::VTSS_DEVCPU_GCB_SIO_CTRL_SIO_CLOCK . SIO_CLK_FREQ
 */
#define  VTSS_F_DEVCPU_GCB_SIO_CTRL_SIO_CLOCK_SIO_CLK_FREQ(x)  VTSS_ENCODE_BITFIELD(x,0,12)
#define  VTSS_M_DEVCPU_GCB_SIO_CTRL_SIO_CLOCK_SIO_CLK_FREQ     VTSS_ENCODE_BITMASK(0,12)
#define  VTSS_X_DEVCPU_GCB_SIO_CTRL_SIO_CLOCK_SIO_CLK_FREQ(x)  VTSS_EXTRACT_BITFIELD(x,0,12)


/** 
 * \brief Output configuration
 *
 * \details
 * Register: \a DEVCPU_GCB:SIO_CTRL:SIO_PORT_CFG
 *
 * @param gi Replicator: x_FFL_DEVCPU_SIO_CTRL_CNT (??), 0-2
 * @param ri Replicator: x_FFL_DEVCPU_SIO_PORT_CNT (??), 0-31
 */
#define VTSS_DEVCPU_GCB_SIO_CTRL_SIO_PORT_CFG(gi,ri)  VTSS_IOREG_IX(VTSS_TO_DEVCPU_GCB,0x54,gi,67,ri,6)

/** 
 * \brief
 * Output source select for the four outputs from each port.The source
 * select is encoded using three bits for each output bit: Output bit0 is
 * defined by (2 downto 0), output bit1 is defined by (5 downto 3), output
 * bit2 is defined by (8 downto 6), and output bit3 is defined by (11
 * downto 9).
 *
 * \details 
 * 0: Forced 0
 * 1: Forced 1
 * 2: Blink mode 0
 * 3: Blink mode 1
 * 4: Link activity blink mode 0
 * 5: Link activity blink mode 1
 * 6: Reserved
 * 7: Reserved
 *
 * Field: ::VTSS_DEVCPU_GCB_SIO_CTRL_SIO_PORT_CFG . BIT_SOURCE
 */
#define  VTSS_F_DEVCPU_GCB_SIO_CTRL_SIO_PORT_CFG_BIT_SOURCE(x)  VTSS_ENCODE_BITFIELD(x,12,12)
#define  VTSS_M_DEVCPU_GCB_SIO_CTRL_SIO_PORT_CFG_BIT_SOURCE     VTSS_ENCODE_BITMASK(12,12)
#define  VTSS_X_DEVCPU_GCB_SIO_CTRL_SIO_PORT_CFG_BIT_SOURCE(x)  VTSS_EXTRACT_BITFIELD(x,12,12)

/** 
 * \brief
 * PWM source select for the four outputs from each port.The PWM source
 * select is encoded using two bits for each output bit: Output bit0 is
 * defined by (1 downto 0), output bit1 is defined by (3 downto 2), output
 * bit2 is defined by (5 downto 4), and output bit3 is defined by (7 downto
 * 6).
 *
 * \details 
 * 0: PWM disabled
 * 1: PWM 0 used
 * 2: PWM 1 used
 * 3: PWM 2 used
 *
 * Field: ::VTSS_DEVCPU_GCB_SIO_CTRL_SIO_PORT_CFG . PWM_SOURCE
 */
#define  VTSS_F_DEVCPU_GCB_SIO_CTRL_SIO_PORT_CFG_PWM_SOURCE(x)  VTSS_ENCODE_BITFIELD(x,4,8)
#define  VTSS_M_DEVCPU_GCB_SIO_CTRL_SIO_PORT_CFG_PWM_SOURCE     VTSS_ENCODE_BITMASK(4,8)
#define  VTSS_X_DEVCPU_GCB_SIO_CTRL_SIO_PORT_CFG_PWM_SOURCE(x)  VTSS_EXTRACT_BITFIELD(x,4,8)

/** 
 * \brief
 * Output polarity select for the four outputs from each port.The polarity
 * select is encoded using one bit for each output bit.
 *
 * \details 
 * 0: Normal polarity
 * 1: Inversed polarity
 *
 * Field: ::VTSS_DEVCPU_GCB_SIO_CTRL_SIO_PORT_CFG . BIT_POLARITY
 */
#define  VTSS_F_DEVCPU_GCB_SIO_CTRL_SIO_PORT_CFG_BIT_POLARITY(x)  VTSS_ENCODE_BITFIELD(x,0,4)
#define  VTSS_M_DEVCPU_GCB_SIO_CTRL_SIO_PORT_CFG_BIT_POLARITY     VTSS_ENCODE_BITMASK(0,4)
#define  VTSS_X_DEVCPU_GCB_SIO_CTRL_SIO_PORT_CFG_BIT_POLARITY(x)  VTSS_EXTRACT_BITFIELD(x,0,4)


/** 
 * \brief Port enable
 *
 * \details
 * Register: \a DEVCPU_GCB:SIO_CTRL:SIO_PORT_ENA
 *
 * @param gi Replicator: x_FFL_DEVCPU_SIO_CTRL_CNT (??), 0-2
 */
#define VTSS_DEVCPU_GCB_SIO_CTRL_SIO_PORT_ENA(gi)  VTSS_IOREG_IX(VTSS_TO_DEVCPU_GCB,0x54,gi,67,0,38)


/** 
 * \brief Pulse-width modulation configuration
 *
 * \details
 * Register: \a DEVCPU_GCB:SIO_CTRL:SIO_PWM_CFG
 *
 * @param gi Replicator: x_FFL_DEVCPU_SIO_CTRL_CNT (??), 0-2
 * @param ri Register: SIO_PWM_CFG (??), 0-2
 */
#define VTSS_DEVCPU_GCB_SIO_CTRL_SIO_PWM_CFG(gi,ri)  VTSS_IOREG_IX(VTSS_TO_DEVCPU_GCB,0x54,gi,67,ri,39)

/** 
 * \brief
 * Define the duty cycle for the PWMs.
 *
 * \details 
 * 0x00: Always "off"
 * 0xFF: Always "on"
 *
 * Field: ::VTSS_DEVCPU_GCB_SIO_CTRL_SIO_PWM_CFG . PWM_DUTY_CYCLE
 */
#define  VTSS_F_DEVCPU_GCB_SIO_CTRL_SIO_PWM_CFG_PWM_DUTY_CYCLE(x)  VTSS_ENCODE_BITFIELD(x,0,8)
#define  VTSS_M_DEVCPU_GCB_SIO_CTRL_SIO_PWM_CFG_PWM_DUTY_CYCLE     VTSS_ENCODE_BITMASK(0,8)
#define  VTSS_X_DEVCPU_GCB_SIO_CTRL_SIO_PWM_CFG_PWM_DUTY_CYCLE(x)  VTSS_EXTRACT_BITFIELD(x,0,8)


/** 
 * \brief SGPIO interrupt polarity
 *
 * \details
 * Register: \a DEVCPU_GCB:SIO_CTRL:SIO_INTR_POL
 *
 * @param gi Replicator: x_FFL_DEVCPU_SIO_CTRL_CNT (??), 0-2
 * @param ri Replicator: x_FFL_DEVCPU_SIO_IOS_PR_PORT (??), 0-3
 */
#define VTSS_DEVCPU_GCB_SIO_CTRL_SIO_INTR_POL(gi,ri)  VTSS_IOREG_IX(VTSS_TO_DEVCPU_GCB,0x54,gi,67,ri,42)


/** 
 * \brief Interrupt raw status
 *
 * \details
 * Register: \a DEVCPU_GCB:SIO_CTRL:SIO_INTR_RAW
 *
 * @param gi Replicator: x_FFL_DEVCPU_SIO_CTRL_CNT (??), 0-2
 * @param ri Replicator: x_FFL_DEVCPU_SIO_IOS_PR_PORT (??), 0-3
 */
#define VTSS_DEVCPU_GCB_SIO_CTRL_SIO_INTR_RAW(gi,ri)  VTSS_IOREG_IX(VTSS_TO_DEVCPU_GCB,0x54,gi,67,ri,46)


/** 
 * \brief Interrupt trigger mode 0
 *
 * \details
 * Register: \a DEVCPU_GCB:SIO_CTRL:SIO_INTR_TRIGGER0
 *
 * @param gi Replicator: x_FFL_DEVCPU_SIO_CTRL_CNT (??), 0-2
 * @param ri Replicator: x_FFL_DEVCPU_SIO_IOS_PR_PORT (??), 0-3
 */
#define VTSS_DEVCPU_GCB_SIO_CTRL_SIO_INTR_TRIGGER0(gi,ri)  VTSS_IOREG_IX(VTSS_TO_DEVCPU_GCB,0x54,gi,67,ri,50)


/** 
 * \brief Interrupt trigger mode 1
 *
 * \details
 * Register: \a DEVCPU_GCB:SIO_CTRL:SIO_INTR_TRIGGER1
 *
 * @param gi Replicator: x_FFL_DEVCPU_SIO_CTRL_CNT (??), 0-2
 * @param ri Replicator: x_FFL_DEVCPU_SIO_IOS_PR_PORT (??), 0-3
 */
#define VTSS_DEVCPU_GCB_SIO_CTRL_SIO_INTR_TRIGGER1(gi,ri)  VTSS_IOREG_IX(VTSS_TO_DEVCPU_GCB,0x54,gi,67,ri,54)


/** 
 * \brief Currently interrupting SGPIOs
 *
 * \details
 * Register: \a DEVCPU_GCB:SIO_CTRL:SIO_INTR
 *
 * @param gi Replicator: x_FFL_DEVCPU_SIO_CTRL_CNT (??), 0-2
 * @param ri Replicator: x_FFL_DEVCPU_SIO_IOS_PR_PORT (??), 0-3
 */
#define VTSS_DEVCPU_GCB_SIO_CTRL_SIO_INTR(gi,ri)  VTSS_IOREG_IX(VTSS_TO_DEVCPU_GCB,0x54,gi,67,ri,58)


/** 
 * \brief SGPIO interrupt enable per port
 *
 * \details
 * Register: \a DEVCPU_GCB:SIO_CTRL:SIO_INTR_ENA
 *
 * @param gi Replicator: x_FFL_DEVCPU_SIO_CTRL_CNT (??), 0-2
 */
#define VTSS_DEVCPU_GCB_SIO_CTRL_SIO_INTR_ENA(gi)  VTSS_IOREG_IX(VTSS_TO_DEVCPU_GCB,0x54,gi,67,0,62)


/** 
 * \brief Currently active interrupts
 *
 * \details
 * Register: \a DEVCPU_GCB:SIO_CTRL:SIO_INTR_IDENT
 *
 * @param gi Replicator: x_FFL_DEVCPU_SIO_CTRL_CNT (??), 0-2
 * @param ri Replicator: x_FFL_DEVCPU_SIO_IOS_PR_PORT (??), 0-3
 */
#define VTSS_DEVCPU_GCB_SIO_CTRL_SIO_INTR_IDENT(gi,ri)  VTSS_IOREG_IX(VTSS_TO_DEVCPU_GCB,0x54,gi,67,ri,63)

/**
 * Register Group: \a DEVCPU_GCB:FAN_CTRL
 *
 * Fan controller configuration and status
 */


/** 
 * \brief Fan controller configuration
 *
 * \details
 * Register: \a DEVCPU_GCB:FAN_CTRL:FAN_CFG
 */
#define VTSS_DEVCPU_GCB_FAN_CTRL_FAN_CFG     VTSS_IOREG(VTSS_TO_DEVCPU_GCB,0x11d)

/** 
 * \brief
 * Define the duty cycle
 *
 * \details 
 * 0x00: Always "off"
 * 0xFF: Always "on"
 *
 * Field: ::VTSS_DEVCPU_GCB_FAN_CTRL_FAN_CFG . DUTY_CYCLE
 */
#define  VTSS_F_DEVCPU_GCB_FAN_CTRL_FAN_CFG_DUTY_CYCLE(x)  VTSS_ENCODE_BITFIELD(x,16,8)
#define  VTSS_M_DEVCPU_GCB_FAN_CTRL_FAN_CFG_DUTY_CYCLE     VTSS_ENCODE_BITMASK(16,8)
#define  VTSS_X_DEVCPU_GCB_FAN_CTRL_FAN_CFG_DUTY_CYCLE(x)  VTSS_EXTRACT_BITFIELD(x,16,8)

/** 
 * \brief
 * Set the frequency of the PWM output 
 *
 * \details 
 * 
 * 0: 25 kHz
 * 1: 120 Hz
 * 2: 100 Hz
 * 3: 80 Hz
 * 4: 60 Hz
 * 5: 40 Hz
 * 6: 20 Hz
 * 7: 10 Hz
 *
 * Field: ::VTSS_DEVCPU_GCB_FAN_CTRL_FAN_CFG . PWM_FREQ
 */
#define  VTSS_F_DEVCPU_GCB_FAN_CTRL_FAN_CFG_PWM_FREQ(x)  VTSS_ENCODE_BITFIELD(x,4,3)
#define  VTSS_M_DEVCPU_GCB_FAN_CTRL_FAN_CFG_PWM_FREQ     VTSS_ENCODE_BITMASK(4,3)
#define  VTSS_X_DEVCPU_GCB_FAN_CTRL_FAN_CFG_PWM_FREQ(x)  VTSS_EXTRACT_BITFIELD(x,4,3)

/** 
 * \brief
 * Define the polarity of the PWM output.
 *
 * \details 
 * 0: PWM is logic 1 when "on"
 * 1: PWM is logic 0 when "on"
 *
 * Field: ::VTSS_DEVCPU_GCB_FAN_CTRL_FAN_CFG . INV_POL
 */
#define  VTSS_F_DEVCPU_GCB_FAN_CTRL_FAN_CFG_INV_POL(x)  VTSS_ENCODE_BITFIELD(!!(x),3,1)
#define  VTSS_M_DEVCPU_GCB_FAN_CTRL_FAN_CFG_INV_POL  VTSS_BIT(3)
#define  VTSS_X_DEVCPU_GCB_FAN_CTRL_FAN_CFG_INV_POL(x)  VTSS_EXTRACT_BITFIELD(x,3,1)

/** 
 * \brief
 * Enable gating of the TACH input by the PWM output so that only TACH
 * pulses received when PWM is "on" are counted.
 *
 * \details 
 * 0: Disabled
 * 1: Enabled
 *
 * Field: ::VTSS_DEVCPU_GCB_FAN_CTRL_FAN_CFG . GATE_ENA
 */
#define  VTSS_F_DEVCPU_GCB_FAN_CTRL_FAN_CFG_GATE_ENA(x)  VTSS_ENCODE_BITFIELD(!!(x),2,1)
#define  VTSS_M_DEVCPU_GCB_FAN_CTRL_FAN_CFG_GATE_ENA  VTSS_BIT(2)
#define  VTSS_X_DEVCPU_GCB_FAN_CTRL_FAN_CFG_GATE_ENA(x)  VTSS_EXTRACT_BITFIELD(x,2,1)

/** 
 * \brief
 * Configure the PWM output to be open collector
 *
 * \details 
 * Field: ::VTSS_DEVCPU_GCB_FAN_CTRL_FAN_CFG . PWM_OPEN_COL_ENA
 */
#define  VTSS_F_DEVCPU_GCB_FAN_CTRL_FAN_CFG_PWM_OPEN_COL_ENA(x)  VTSS_ENCODE_BITFIELD(!!(x),1,1)
#define  VTSS_M_DEVCPU_GCB_FAN_CTRL_FAN_CFG_PWM_OPEN_COL_ENA  VTSS_BIT(1)
#define  VTSS_X_DEVCPU_GCB_FAN_CTRL_FAN_CFG_PWM_OPEN_COL_ENA(x)  VTSS_EXTRACT_BITFIELD(x,1,1)

/** 
 * \brief
 * Configure behavior of TACH input tick counter, see DEVCPU_GCB::FAN_CNT
 * for more infromation.
 *
 * \details 
 * Field: ::VTSS_DEVCPU_GCB_FAN_CTRL_FAN_CFG . FAN_STAT_CFG
 */
#define  VTSS_F_DEVCPU_GCB_FAN_CTRL_FAN_CFG_FAN_STAT_CFG(x)  VTSS_ENCODE_BITFIELD(!!(x),0,1)
#define  VTSS_M_DEVCPU_GCB_FAN_CTRL_FAN_CFG_FAN_STAT_CFG  VTSS_BIT(0)
#define  VTSS_X_DEVCPU_GCB_FAN_CTRL_FAN_CFG_FAN_STAT_CFG(x)  VTSS_EXTRACT_BITFIELD(x,0,1)


/** 
 * \brief TACH counter
 *
 * \details
 * Register: \a DEVCPU_GCB:FAN_CTRL:FAN_CNT
 */
#define VTSS_DEVCPU_GCB_FAN_CTRL_FAN_CNT     VTSS_IOREG(VTSS_TO_DEVCPU_GCB,0x11e)

/** 
 * \brief
 * Counts the number of TACH input ticks. If
 * DEVCPU_GCB::FAN_CFG.FAN_STAT_CFG is set then this is a wrapping counter
 * that shows the total number of registered TACH ticks. If
 * DEVCPU_GCB::FAN_CFG.FAN_STAT_CFG is cleared then this counter is updated
 * once every second with the number of TACH ticks registered during the
 * last second.
 *
 * \details 
 * Field: ::VTSS_DEVCPU_GCB_FAN_CTRL_FAN_CNT . FAN_CNT
 */
#define  VTSS_F_DEVCPU_GCB_FAN_CTRL_FAN_CNT_FAN_CNT(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_DEVCPU_GCB_FAN_CTRL_FAN_CNT_FAN_CNT     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_DEVCPU_GCB_FAN_CTRL_FAN_CNT_FAN_CNT(x)  VTSS_EXTRACT_BITFIELD(x,0,16)

/**
 * Register Group: \a DEVCPU_GCB:MEMITGR
 *
 * Memory integrity monitor
 */


/** 
 * \brief Monitor control
 *
 * \details
 * Register: \a DEVCPU_GCB:MEMITGR:MEMITGR_CTRL
 */
#define VTSS_DEVCPU_GCB_MEMITGR_MEMITGR_CTRL  VTSS_IOREG(VTSS_TO_DEVCPU_GCB,0x11f)

/** 
 * \brief
 * Setting this field transitions the integrity monitor between operating
 * modes. Transitioning between modes takes time, this field remains set
 * until the new mode is reached. During this time the monitor also reports
 * busy DEVCPU_GCB::MEMITGR_STAT.MODE_BUSY is set).
 * From IDLE (DEVCPU_GCB::MEMITGR_STAT.MODE_IDLE is set) the monitor can
 * transition into either DETECT or LISTEN mode, the DETECT mode is entered
 * if a memory reports an indication - the LISTEN mode is entered if no
 * indications are reported. The first time after reset the monitor will
 * not detect indications, that is; it will transition directly from IDLE
 * to LISTEN mode.
 * From DETECT (DEVCPU_GCB::MEMITGR_STAT.MODE_DETECT is set) the monitor
 * can transition into either DETECT or LISTEN mode, the DETECT mode is
 * entered if more indications are reported - the LISTEN mode is entered if
 * no more indications are reported.
 * From LISTEN (DEVCPU_GCB::MEMITGR_STAT.MODE_LISTEN is set) the monitor
 * can transition into IDLE mode.
 * Software shall not set this field when the monitor is BUSY (when
 * DEVCPU_GCB::MEMITGR_STAT.MODE_BUSY is set.)
 *
 * \details 
 * Field: ::VTSS_DEVCPU_GCB_MEMITGR_MEMITGR_CTRL . ACTIVATE
 */
#define  VTSS_F_DEVCPU_GCB_MEMITGR_MEMITGR_CTRL_ACTIVATE(x)  VTSS_ENCODE_BITFIELD(!!(x),0,1)
#define  VTSS_M_DEVCPU_GCB_MEMITGR_MEMITGR_CTRL_ACTIVATE  VTSS_BIT(0)
#define  VTSS_X_DEVCPU_GCB_MEMITGR_MEMITGR_CTRL_ACTIVATE(x)  VTSS_EXTRACT_BITFIELD(x,0,1)


/** 
 * \brief Monitor status
 *
 * \details
 * Register: \a DEVCPU_GCB:MEMITGR:MEMITGR_STAT
 */
#define VTSS_DEVCPU_GCB_MEMITGR_MEMITGR_STAT  VTSS_IOREG(VTSS_TO_DEVCPU_GCB,0x120)

/** 
 * \brief
 * This field is set if there is an overflow when recording indications
 * from the memories. If this happens (setting of this field) the value of
 * the DEVCPU_GCB::MEMITGR_DIV.MEM_DIV field is set too low!
 *
 * \details 
 * Field: ::VTSS_DEVCPU_GCB_MEMITGR_MEMITGR_STAT . INDICATION_OVF
 */
#define  VTSS_F_DEVCPU_GCB_MEMITGR_MEMITGR_STAT_INDICATION_OVF(x)  VTSS_ENCODE_BITFIELD(!!(x),5,1)
#define  VTSS_M_DEVCPU_GCB_MEMITGR_MEMITGR_STAT_INDICATION_OVF  VTSS_BIT(5)
#define  VTSS_X_DEVCPU_GCB_MEMITGR_MEMITGR_STAT_INDICATION_OVF(x)  VTSS_EXTRACT_BITFIELD(x,5,1)

/** 
 * \brief
 * If this field is set then there is an indication from one of the
 * memories that needs to be analyzed. An indication is either a parity
 * detection or an error correction.
 * This field is only set when the monitor is in LISTEN mode
 * (DEVCPU_GCB::MEMITGR_STAT.MODE_LISTEN is set), in all other states
 * (including BUSY) this field returns 0.
 *
 * \details 
 * Field: ::VTSS_DEVCPU_GCB_MEMITGR_MEMITGR_STAT . INDICATION
 */
#define  VTSS_F_DEVCPU_GCB_MEMITGR_MEMITGR_STAT_INDICATION(x)  VTSS_ENCODE_BITFIELD(!!(x),4,1)
#define  VTSS_M_DEVCPU_GCB_MEMITGR_MEMITGR_STAT_INDICATION  VTSS_BIT(4)
#define  VTSS_X_DEVCPU_GCB_MEMITGR_MEMITGR_STAT_INDICATION(x)  VTSS_EXTRACT_BITFIELD(x,4,1)

/** 
 * \brief
 * This field is set when the monitor is in LISTEN mode, during listen mode
 * the monitor continually check for parity/correction indications from the
 * memories.
 *
 * \details 
 * Field: ::VTSS_DEVCPU_GCB_MEMITGR_MEMITGR_STAT . MODE_LISTEN
 */
#define  VTSS_F_DEVCPU_GCB_MEMITGR_MEMITGR_STAT_MODE_LISTEN(x)  VTSS_ENCODE_BITFIELD(!!(x),3,1)
#define  VTSS_M_DEVCPU_GCB_MEMITGR_MEMITGR_STAT_MODE_LISTEN  VTSS_BIT(3)
#define  VTSS_X_DEVCPU_GCB_MEMITGR_MEMITGR_STAT_MODE_LISTEN(x)  VTSS_EXTRACT_BITFIELD(x,3,1)

/** 
 * \brief
 * This field is set when the monitor is in DETECT mode, during detect mode
 * the DEVCPU_GCB::MEMITGR_INFO and DEVCPU_GCB::MEMITGR_IDX registers
 * contains valid information about one indication.
 *
 * \details 
 * Field: ::VTSS_DEVCPU_GCB_MEMITGR_MEMITGR_STAT . MODE_DETECT
 */
#define  VTSS_F_DEVCPU_GCB_MEMITGR_MEMITGR_STAT_MODE_DETECT(x)  VTSS_ENCODE_BITFIELD(!!(x),2,1)
#define  VTSS_M_DEVCPU_GCB_MEMITGR_MEMITGR_STAT_MODE_DETECT  VTSS_BIT(2)
#define  VTSS_X_DEVCPU_GCB_MEMITGR_MEMITGR_STAT_MODE_DETECT(x)  VTSS_EXTRACT_BITFIELD(x,2,1)

/** 
 * \brief
 * This field is set when the monitor is in IDLE mode.
 *
 * \details 
 * Field: ::VTSS_DEVCPU_GCB_MEMITGR_MEMITGR_STAT . MODE_IDLE
 */
#define  VTSS_F_DEVCPU_GCB_MEMITGR_MEMITGR_STAT_MODE_IDLE(x)  VTSS_ENCODE_BITFIELD(!!(x),1,1)
#define  VTSS_M_DEVCPU_GCB_MEMITGR_MEMITGR_STAT_MODE_IDLE  VTSS_BIT(1)
#define  VTSS_X_DEVCPU_GCB_MEMITGR_MEMITGR_STAT_MODE_IDLE(x)  VTSS_EXTRACT_BITFIELD(x,1,1)

/** 
 * \brief
 * The busy signal is a copy of the DEVCPU_GCB::MEMITGR_CTRL.ACTIVATE
 * field, see description of that field for more information about the
 * different states/modes of the monitor.
 *
 * \details 
 * Field: ::VTSS_DEVCPU_GCB_MEMITGR_MEMITGR_STAT . MODE_BUSY
 */
#define  VTSS_F_DEVCPU_GCB_MEMITGR_MEMITGR_STAT_MODE_BUSY(x)  VTSS_ENCODE_BITFIELD(!!(x),0,1)
#define  VTSS_M_DEVCPU_GCB_MEMITGR_MEMITGR_STAT_MODE_BUSY  VTSS_BIT(0)
#define  VTSS_X_DEVCPU_GCB_MEMITGR_MEMITGR_STAT_MODE_BUSY(x)  VTSS_EXTRACT_BITFIELD(x,0,1)


/** 
 * \brief Memory indication
 *
 * \details
 * This field is only valid when the monitor is in the DETECT
 * (DEVCPU_GCB::MEMITGR_MODE.MODE_DETECT is set) mode.
 *
 * Register: \a DEVCPU_GCB:MEMITGR:MEMITGR_INFO
 */
#define VTSS_DEVCPU_GCB_MEMITGR_MEMITGR_INFO  VTSS_IOREG(VTSS_TO_DEVCPU_GCB,0x121)

/** 
 * \brief
 * This field is set if the monitor has detected a parity indication (or an
 * unrecoverable correction).
 *
 * \details 
 * Field: ::VTSS_DEVCPU_GCB_MEMITGR_MEMITGR_INFO . MEM_ERR
 */
#define  VTSS_F_DEVCPU_GCB_MEMITGR_MEMITGR_INFO_MEM_ERR(x)  VTSS_ENCODE_BITFIELD(!!(x),31,1)
#define  VTSS_M_DEVCPU_GCB_MEMITGR_MEMITGR_INFO_MEM_ERR  VTSS_BIT(31)
#define  VTSS_X_DEVCPU_GCB_MEMITGR_MEMITGR_INFO_MEM_ERR(x)  VTSS_EXTRACT_BITFIELD(x,31,1)

/** 
 * \brief
 * This field is set if the monitor has detected a correction.
 *
 * \details 
 * Field: ::VTSS_DEVCPU_GCB_MEMITGR_MEMITGR_INFO . MEM_COR
 */
#define  VTSS_F_DEVCPU_GCB_MEMITGR_MEMITGR_INFO_MEM_COR(x)  VTSS_ENCODE_BITFIELD(!!(x),30,1)
#define  VTSS_M_DEVCPU_GCB_MEMITGR_MEMITGR_INFO_MEM_COR  VTSS_BIT(30)
#define  VTSS_X_DEVCPU_GCB_MEMITGR_MEMITGR_INFO_MEM_COR(x)  VTSS_EXTRACT_BITFIELD(x,30,1)

/** 
 * \brief
 * This field is set if the monitor has detected a parity indication (or an
 * unrecoverable correction) for which the address has not been recorded.
 * If DEVCPU_GCB::MEMITGR_INFO.MEM_ERR is set then there has been more than
 * one indication, then only the address of the newest indication has been
 * kept. 
 * If DEVCPU_GCB::MEMITGR_INFO.MEM_ERR is cleared then an indication has
 * occurred for which the address could not be stored, this is a very rare
 * situation that can only happen if an indication is detected just as the
 * memory is talking to the monitor.
 *
 * \details 
 * Field: ::VTSS_DEVCPU_GCB_MEMITGR_MEMITGR_INFO . MEM_ERR_OVF
 */
#define  VTSS_F_DEVCPU_GCB_MEMITGR_MEMITGR_INFO_MEM_ERR_OVF(x)  VTSS_ENCODE_BITFIELD(!!(x),29,1)
#define  VTSS_M_DEVCPU_GCB_MEMITGR_MEMITGR_INFO_MEM_ERR_OVF  VTSS_BIT(29)
#define  VTSS_X_DEVCPU_GCB_MEMITGR_MEMITGR_INFO_MEM_ERR_OVF(x)  VTSS_EXTRACT_BITFIELD(x,29,1)

/** 
 * \brief
 * This field is set if the monitor has correction indication for which the
 * address has not been recorded.
 * If DEVCPU_GCB::MEMITGR_INFO.MEM_ERR is set then there has also been a
 * parity indication (or an unrecoverable correction) which takes priority
 * over correction indications.
 * If DEVCPU_GCB::MEMITGR_INFO.MEM_ERR is cleared and
 * DEVCPU_GCB::MEMITGR_INFO.MEM_COR is set then there has been more than
 * one correction indication, then only the address of the newest
 * correction indication has been kept.
 * If DEVCPU_GCB::MEMITGR_INFO.MEM_ERR and DEVCPU_GCB::MEMITGR_INFO.MEM_COR
 * is both cleared then a correction indication has occurred for which the
 * address could not be stored, this is a very rare situation that can only
 * happen if an indication is detected just as the memory is talking to the
 * monitor.
 *
 * \details 
 * Field: ::VTSS_DEVCPU_GCB_MEMITGR_MEMITGR_INFO . MEM_COR_OVF
 */
#define  VTSS_F_DEVCPU_GCB_MEMITGR_MEMITGR_INFO_MEM_COR_OVF(x)  VTSS_ENCODE_BITFIELD(!!(x),28,1)
#define  VTSS_M_DEVCPU_GCB_MEMITGR_MEMITGR_INFO_MEM_COR_OVF  VTSS_BIT(28)
#define  VTSS_X_DEVCPU_GCB_MEMITGR_MEMITGR_INFO_MEM_COR_OVF(x)  VTSS_EXTRACT_BITFIELD(x,28,1)

/** 
 * \brief
 * This field is valid only when DEVCPU_GCB::MEMITGR_INFO.MEM_ERR or
 * DEVCPU_GCB::MEMITGR_INFO.MEM_COR is set.
 *
 * \details 
 * Field: ::VTSS_DEVCPU_GCB_MEMITGR_MEMITGR_INFO . MEM_ADDR
 */
#define  VTSS_F_DEVCPU_GCB_MEMITGR_MEMITGR_INFO_MEM_ADDR(x)  VTSS_ENCODE_BITFIELD(x,0,28)
#define  VTSS_M_DEVCPU_GCB_MEMITGR_MEMITGR_INFO_MEM_ADDR     VTSS_ENCODE_BITMASK(0,28)
#define  VTSS_X_DEVCPU_GCB_MEMITGR_MEMITGR_INFO_MEM_ADDR(x)  VTSS_EXTRACT_BITFIELD(x,0,28)


/** 
 * \brief Memory index
 *
 * \details
 * This field is only valid when the monitor is in the DETECT
 * (DEVCPU_GCB::MEMITGR_MODE.MODE_DETECT is set) mode.
 *
 * Register: \a DEVCPU_GCB:MEMITGR:MEMITGR_IDX
 */
#define VTSS_DEVCPU_GCB_MEMITGR_MEMITGR_IDX  VTSS_IOREG(VTSS_TO_DEVCPU_GCB,0x122)

/** 
 * \brief
 * This field contains a unique index for the memory for which info is
 * currently provided in DEVCPU_GCB::MEMITGR_INFO. Indexes are counted from
 * 1 (not 0).
 *
 * \details 
 * Field: ::VTSS_DEVCPU_GCB_MEMITGR_MEMITGR_IDX . MEM_IDX
 */
#define  VTSS_F_DEVCPU_GCB_MEMITGR_MEMITGR_IDX_MEM_IDX(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_DEVCPU_GCB_MEMITGR_MEMITGR_IDX_MEM_IDX     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_DEVCPU_GCB_MEMITGR_MEMITGR_IDX_MEM_IDX(x)  VTSS_EXTRACT_BITFIELD(x,0,16)


/** 
 * \brief Monitor speed
 *
 * \details
 * Register: \a DEVCPU_GCB:MEMITGR:MEMITGR_DIV
 */
#define VTSS_DEVCPU_GCB_MEMITGR_MEMITGR_DIV  VTSS_IOREG(VTSS_TO_DEVCPU_GCB,0x123)

/** 
 * \brief
 * Configure divider for generating the sync-pulse to memories (controls
 * the speed at which the monitor talks to the memories). The lower this is
 * set the faster indications can be read out of the memories.
 *
 * \details 
 * See datasheet for appropriate value.
 *
 * Field: ::VTSS_DEVCPU_GCB_MEMITGR_MEMITGR_DIV . MEM_DIV
 */
#define  VTSS_F_DEVCPU_GCB_MEMITGR_MEMITGR_DIV_MEM_DIV(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_DEVCPU_GCB_MEMITGR_MEMITGR_DIV_MEM_DIV     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_DEVCPU_GCB_MEMITGR_MEMITGR_DIV_MEM_DIV(x)  VTSS_EXTRACT_BITFIELD(x,0,16)


/** 
 * \brief Debug registers
 *
 * \details
 * Register: \a DEVCPU_GCB:MEMITGR:MEMITGR_DBG
 */
#define VTSS_DEVCPU_GCB_MEMITGR_MEMITGR_DBG  VTSS_IOREG(VTSS_TO_DEVCPU_GCB,0x124)

/** 
 * \brief
 * This field is used for test purposes.
 *
 * \details 
 * Field: ::VTSS_DEVCPU_GCB_MEMITGR_MEMITGR_DBG . MEM_DIV_SENSE
 */
#define  VTSS_F_DEVCPU_GCB_MEMITGR_MEMITGR_DBG_MEM_DIV_SENSE(x)  VTSS_ENCODE_BITFIELD(!!(x),9,1)
#define  VTSS_M_DEVCPU_GCB_MEMITGR_MEMITGR_DBG_MEM_DIV_SENSE  VTSS_BIT(9)
#define  VTSS_X_DEVCPU_GCB_MEMITGR_MEMITGR_DBG_MEM_DIV_SENSE(x)  VTSS_EXTRACT_BITFIELD(x,9,1)

/** 
 * \brief
 * Set to force the intr-field of the intgrity-ring out from the monitor.
 *
 * \details 
 * Field: ::VTSS_DEVCPU_GCB_MEMITGR_MEMITGR_DBG . FORCE_INTR
 */
#define  VTSS_F_DEVCPU_GCB_MEMITGR_MEMITGR_DBG_FORCE_INTR(x)  VTSS_ENCODE_BITFIELD(!!(x),8,1)
#define  VTSS_M_DEVCPU_GCB_MEMITGR_MEMITGR_DBG_FORCE_INTR  VTSS_BIT(8)
#define  VTSS_X_DEVCPU_GCB_MEMITGR_MEMITGR_DBG_FORCE_INTR(x)  VTSS_EXTRACT_BITFIELD(x,8,1)

/** 
 * \brief
 * Always shows the (resynced) value of the sync-field of the
 * integrity-ring into the monitor.
 *
 * \details 
 * Field: ::VTSS_DEVCPU_GCB_MEMITGR_MEMITGR_DBG . SYNC_IN
 */
#define  VTSS_F_DEVCPU_GCB_MEMITGR_MEMITGR_DBG_SYNC_IN(x)  VTSS_ENCODE_BITFIELD(!!(x),7,1)
#define  VTSS_M_DEVCPU_GCB_MEMITGR_MEMITGR_DBG_SYNC_IN  VTSS_BIT(7)
#define  VTSS_X_DEVCPU_GCB_MEMITGR_MEMITGR_DBG_SYNC_IN(x)  VTSS_EXTRACT_BITFIELD(x,7,1)

/** 
 * \brief
 * Always shows the (resynced) value of the ena-field of the integrity-ring
 * into the monitor.
 *
 * \details 
 * Field: ::VTSS_DEVCPU_GCB_MEMITGR_MEMITGR_DBG . ENA_IN
 */
#define  VTSS_F_DEVCPU_GCB_MEMITGR_MEMITGR_DBG_ENA_IN(x)  VTSS_ENCODE_BITFIELD(!!(x),6,1)
#define  VTSS_M_DEVCPU_GCB_MEMITGR_MEMITGR_DBG_ENA_IN  VTSS_BIT(6)
#define  VTSS_X_DEVCPU_GCB_MEMITGR_MEMITGR_DBG_ENA_IN(x)  VTSS_EXTRACT_BITFIELD(x,6,1)

/** 
 * \brief
 * Always shows the (resynced) value of the data-field of the
 * integrity-ring into the monitor.
 *
 * \details 
 * Field: ::VTSS_DEVCPU_GCB_MEMITGR_MEMITGR_DBG . DATA_IN
 */
#define  VTSS_F_DEVCPU_GCB_MEMITGR_MEMITGR_DBG_DATA_IN(x)  VTSS_ENCODE_BITFIELD(!!(x),5,1)
#define  VTSS_M_DEVCPU_GCB_MEMITGR_MEMITGR_DBG_DATA_IN  VTSS_BIT(5)
#define  VTSS_X_DEVCPU_GCB_MEMITGR_MEMITGR_DBG_DATA_IN(x)  VTSS_EXTRACT_BITFIELD(x,5,1)

/** 
 * \brief
 * Always shows the (resynced) value of the intr-field of the
 * integrity-ring into the monitor.
 *
 * \details 
 * Field: ::VTSS_DEVCPU_GCB_MEMITGR_MEMITGR_DBG . INTR_IN
 */
#define  VTSS_F_DEVCPU_GCB_MEMITGR_MEMITGR_DBG_INTR_IN(x)  VTSS_ENCODE_BITFIELD(!!(x),4,1)
#define  VTSS_M_DEVCPU_GCB_MEMITGR_MEMITGR_DBG_INTR_IN  VTSS_BIT(4)
#define  VTSS_X_DEVCPU_GCB_MEMITGR_MEMITGR_DBG_INTR_IN(x)  VTSS_EXTRACT_BITFIELD(x,4,1)

/** 
 * \brief
 * When DEVCPU_GCB::MEMITGR_DBG.FORCE_ENA is set, this value is driven to
 * sync-field of the intgrity-ring out from the monitor.
 *
 * \details 
 * Field: ::VTSS_DEVCPU_GCB_MEMITGR_MEMITGR_DBG . SYNC_OUT
 */
#define  VTSS_F_DEVCPU_GCB_MEMITGR_MEMITGR_DBG_SYNC_OUT(x)  VTSS_ENCODE_BITFIELD(!!(x),3,1)
#define  VTSS_M_DEVCPU_GCB_MEMITGR_MEMITGR_DBG_SYNC_OUT  VTSS_BIT(3)
#define  VTSS_X_DEVCPU_GCB_MEMITGR_MEMITGR_DBG_SYNC_OUT(x)  VTSS_EXTRACT_BITFIELD(x,3,1)

/** 
 * \brief
 * When DEVCPU_GCB::MEMITGR_DBG.FORCE_ENA is set, this value is driven to
 * ena-field of the intgrity-ring out from the monitor.
 *
 * \details 
 * Field: ::VTSS_DEVCPU_GCB_MEMITGR_MEMITGR_DBG . ENA_OUT
 */
#define  VTSS_F_DEVCPU_GCB_MEMITGR_MEMITGR_DBG_ENA_OUT(x)  VTSS_ENCODE_BITFIELD(!!(x),2,1)
#define  VTSS_M_DEVCPU_GCB_MEMITGR_MEMITGR_DBG_ENA_OUT  VTSS_BIT(2)
#define  VTSS_X_DEVCPU_GCB_MEMITGR_MEMITGR_DBG_ENA_OUT(x)  VTSS_EXTRACT_BITFIELD(x,2,1)

/** 
 * \brief
 * Set to overtake ena and sync outputs (bit banging on the integrity ring)
 * and drive these by DEVCPU_GCB::MEMITGR_DBG.ENA_OUT and
 * DEVCPU_GCB::MEMITGR_DBG.SYNC_OUT respectively.
 *
 * \details 
 * Field: ::VTSS_DEVCPU_GCB_MEMITGR_MEMITGR_DBG . FORCE_ENA
 */
#define  VTSS_F_DEVCPU_GCB_MEMITGR_MEMITGR_DBG_FORCE_ENA(x)  VTSS_ENCODE_BITFIELD(!!(x),1,1)
#define  VTSS_M_DEVCPU_GCB_MEMITGR_MEMITGR_DBG_FORCE_ENA  VTSS_BIT(1)
#define  VTSS_X_DEVCPU_GCB_MEMITGR_MEMITGR_DBG_FORCE_ENA(x)  VTSS_EXTRACT_BITFIELD(x,1,1)

/** 
 * \brief
 * Set this field to enable detection of parity detections and ecc
 * corrections.
 * The monitor must be initialized before first use (after reset), this is
 * done by making the first transtion from IDLE to LISTEN mode without
 * enabeling detections (this field is cleared) and then setting this field
 * once the monitor reaches the LISTEN mode.
 *
 * \details 
 * Field: ::VTSS_DEVCPU_GCB_MEMITGR_MEMITGR_DBG . DETECT_ENA
 */
#define  VTSS_F_DEVCPU_GCB_MEMITGR_MEMITGR_DBG_DETECT_ENA(x)  VTSS_ENCODE_BITFIELD(!!(x),0,1)
#define  VTSS_M_DEVCPU_GCB_MEMITGR_MEMITGR_DBG_DETECT_ENA  VTSS_BIT(0)
#define  VTSS_X_DEVCPU_GCB_MEMITGR_MEMITGR_DBG_DETECT_ENA(x)  VTSS_EXTRACT_BITFIELD(x,0,1)

/**
 * Register Group: \a DEVCPU_GCB:VRAP
 *
 * VRAP controller
 */


/** 
 * \brief VRAP events
 *
 * \details
 * Register: \a DEVCPU_GCB:VRAP:VRAP_ACCESS_STAT
 */
#define VTSS_DEVCPU_GCB_VRAP_VRAP_ACCESS_STAT  VTSS_IOREG(VTSS_TO_DEVCPU_GCB,0x125)

/** 
 * \brief
 * This field is set if a valid VRAP (Vitesse Register Access Protocol)
 * frame has been received.
 *
 * \details 
 * Field: ::VTSS_DEVCPU_GCB_VRAP_VRAP_ACCESS_STAT . FRM_RECV_STICKY
 */
#define  VTSS_F_DEVCPU_GCB_VRAP_VRAP_ACCESS_STAT_FRM_RECV_STICKY(x)  VTSS_ENCODE_BITFIELD(!!(x),3,1)
#define  VTSS_M_DEVCPU_GCB_VRAP_VRAP_ACCESS_STAT_FRM_RECV_STICKY  VTSS_BIT(3)
#define  VTSS_X_DEVCPU_GCB_VRAP_VRAP_ACCESS_STAT_FRM_RECV_STICKY(x)  VTSS_EXTRACT_BITFIELD(x,3,1)

/** 
 * \brief
 * This field is set if an invalid command inside a valid VRAP frame has
 * been received. The VRAP engine has ignored the command.
 *
 * \details 
 * Field: ::VTSS_DEVCPU_GCB_VRAP_VRAP_ACCESS_STAT . CMD_INVALID_STICKY
 */
#define  VTSS_F_DEVCPU_GCB_VRAP_VRAP_ACCESS_STAT_CMD_INVALID_STICKY(x)  VTSS_ENCODE_BITFIELD(!!(x),2,1)
#define  VTSS_M_DEVCPU_GCB_VRAP_VRAP_ACCESS_STAT_CMD_INVALID_STICKY  VTSS_BIT(2)
#define  VTSS_X_DEVCPU_GCB_VRAP_VRAP_ACCESS_STAT_CMD_INVALID_STICKY(x)  VTSS_EXTRACT_BITFIELD(x,2,1)

/** 
 * \brief
 * This field is set if an invalid VRAP frame has been received and
 * discarded by the VRAP-engine. Frames with a VRAP header different from
 * V1 are considered invalid.
 *
 * \details 
 * Field: ::VTSS_DEVCPU_GCB_VRAP_VRAP_ACCESS_STAT . FRM_INVALID_STICKY
 */
#define  VTSS_F_DEVCPU_GCB_VRAP_VRAP_ACCESS_STAT_FRM_INVALID_STICKY(x)  VTSS_ENCODE_BITFIELD(!!(x),1,1)
#define  VTSS_M_DEVCPU_GCB_VRAP_VRAP_ACCESS_STAT_FRM_INVALID_STICKY  VTSS_BIT(1)
#define  VTSS_X_DEVCPU_GCB_VRAP_VRAP_ACCESS_STAT_FRM_INVALID_STICKY(x)  VTSS_EXTRACT_BITFIELD(x,1,1)

/** 
 * \brief
 * This field is set if a VRAP reply frame has been aborted. This my happen
 * if a protocol violation is detected during VRAP request frame
 * processing.
 *
 * \details 
 * Field: ::VTSS_DEVCPU_GCB_VRAP_VRAP_ACCESS_STAT . REPLY_ABORT_STICKY
 */
#define  VTSS_F_DEVCPU_GCB_VRAP_VRAP_ACCESS_STAT_REPLY_ABORT_STICKY(x)  VTSS_ENCODE_BITFIELD(!!(x),0,1)
#define  VTSS_M_DEVCPU_GCB_VRAP_VRAP_ACCESS_STAT_REPLY_ABORT_STICKY  VTSS_BIT(0)
#define  VTSS_X_DEVCPU_GCB_VRAP_VRAP_ACCESS_STAT_REPLY_ABORT_STICKY(x)  VTSS_EXTRACT_BITFIELD(x,0,1)


#endif /* _VTSS_JAGUAR2_REGS_DEVCPU_GCB_H_ */
