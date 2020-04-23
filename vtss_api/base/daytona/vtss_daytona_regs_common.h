#ifndef _VTSS_DAYTONA_REGS_COMMON_H_
#define _VTSS_DAYTONA_REGS_COMMON_H_

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

#ifndef VTSS_BITOPS_DEFINED
#ifdef __ASSEMBLER__
#define VTSS_BIT(x)                   (1 << (x))
#define VTSS_BITMASK(x)               (~(0XFFFFFFFE << (x-1)))
#else
// -eVTSS_BITMASK(648)
#define VTSS_BIT(x)                   (1U << (x))
/*lint  -emacro(648,VTSS_BITMASK) */  /*... Overflow is intended in this macro */
#define VTSS_BITMASK(x)               (~(0XFFFFFFFEU << (x-1)))
#endif
#define VTSS_EXTRACT_BITFIELD(x,o,w)  (((x) >> (o)) & VTSS_BITMASK(w))
#define VTSS_ENCODE_BITFIELD(x,o,w)   (((x) & VTSS_BITMASK(w)) << (o))
#define VTSS_ENCODE_BITMASK(o,w)      (VTSS_BITMASK(w) << (o))
#define VTSS_BITOPS_DEFINED
#endif /* VTSS_BITOPS_DEFINED */

/* Main target address offsets */
#define VTSS_TO_CFG_IF_ORG 0x00000000 /*!< Base offset for target CFG_IF_ORG */
#define VTSS_TO_TFI_0      0x00100000 /*!< Base offset for target TFI_0 */
#define VTSS_TO_SFI_0      0x00110000 /*!< Base offset for target SFI_0 */
#define VTSS_TO_XAUI_0     0x00120000 /*!< Base offset for target XAUI_0 */
#define VTSS_TO_OTU_1_0    0x00130000 /*!< Base offset for target OTU_1_0 */
#define VTSS_TO_WIS_1_0    0x00140000 /*!< Base offset for target WIS_1_0 */
#define VTSS_TO_PCS_1_0    0x00150000 /*!< Base offset for target PCS_1_0 */
#define VTSS_TO_GFP_1_0    0x00160000 /*!< Base offset for target GFP_1_0 */
#define VTSS_TO_MAC_1_0    0x00170000 /*!< Base offset for target MAC_1_0 */
#define VTSS_TO_RAB_0      0x00180000 /*!< Base offset for target RAB_0 */
#define VTSS_TO_MAC_2_0    0x00190000 /*!< Base offset for target MAC_2_0 */
#define VTSS_TO_GFP_2_0    0x001a0000 /*!< Base offset for target GFP_2_0 */
#define VTSS_TO_PCS_2_0    0x001b0000 /*!< Base offset for target PCS_2_0 */
#define VTSS_TO_WIS_2_0    0x001c0000 /*!< Base offset for target WIS_2_0 */
#define VTSS_TO_OTU_2_0    0x001d0000 /*!< Base offset for target OTU_2_0 */
#define VTSS_TO_EFEC_0     0x001e0000 /*!< Base offset for target EFEC_0 */
#define VTSS_TO_EFECI7_0   0x001f0000 /*!< Base offset for target EFECI7_0 */
#define VTSS_TO_TFI_1      0x00200000 /*!< Base offset for target TFI_1 */
#define VTSS_TO_SFI_1      0x00210000 /*!< Base offset for target SFI_1 */
#define VTSS_TO_XAUI_1     0x00220000 /*!< Base offset for target XAUI_1 */
#define VTSS_TO_OTU_1_1    0x00230000 /*!< Base offset for target OTU_1_1 */
#define VTSS_TO_WIS_1_1    0x00240000 /*!< Base offset for target WIS_1_1 */
#define VTSS_TO_PCS_1_1    0x00250000 /*!< Base offset for target PCS_1_1 */
#define VTSS_TO_GFP_1_1    0x00260000 /*!< Base offset for target GFP_1_1 */
#define VTSS_TO_MAC_1_1    0x00270000 /*!< Base offset for target MAC_1_1 */
#define VTSS_TO_RAB_1      0x00280000 /*!< Base offset for target RAB_1 */
#define VTSS_TO_MAC_2_1    0x00290000 /*!< Base offset for target MAC_2_1 */
#define VTSS_TO_GFP_2_1    0x002a0000 /*!< Base offset for target GFP_2_1 */
#define VTSS_TO_PCS_2_1    0x002b0000 /*!< Base offset for target PCS_2_1 */
#define VTSS_TO_WIS_2_1    0x002c0000 /*!< Base offset for target WIS_2_1 */
#define VTSS_TO_OTU_2_1    0x002d0000 /*!< Base offset for target OTU_2_1 */
#define VTSS_TO_EFEC_1     0x002e0000 /*!< Base offset for target EFEC_1 */
#define VTSS_TO_EFECI7_1   0x002f0000 /*!< Base offset for target EFECI7_1 */
#define VTSS_TO_PTP_0      0x00300000 /*!< Base offset for target PTP_0 */
#define VTSS_TO_PTP_1      0x00310000 /*!< Base offset for target PTP_1 */
#define VTSS_TO_PTP_ANA0_INGR_CFG 0x00320000 /*!< Base offset for target PTP_ANA0_INGR_CFG */
#define VTSS_TO_PTP_ANA0_EGR_CFG 0x00330000 /*!< Base offset for target PTP_ANA0_EGR_CFG */
#define VTSS_TO_PTP_ANA1_INGR_CFG 0x00340000 /*!< Base offset for target PTP_ANA1_INGR_CFG */
#define VTSS_TO_PTP_ANA1_EGR_CFG 0x00350000 /*!< Base offset for target PTP_ANA1_EGR_CFG */
#define VTSS_TO_PTP_ANA2_INGR_CFG 0x00360000 /*!< Base offset for target PTP_ANA2_INGR_CFG */
#define VTSS_TO_PTP_ANA2_EGR_CFG 0x00370000 /*!< Base offset for target PTP_ANA2_EGR_CFG */
#define VTSS_TO_CFG_IF_FAST_REGS 0x003f0000 /*!< Base offset for target CFG_IF_FAST_REGS */
#define VTSS_TO_UPI_0      0x00080000 /*!< Base offset for target UPI_0 */
#define VTSS_TO_LXFI_0     0x00090000 /*!< Base offset for target LXFI_0 */
#define VTSS_TO_CFG_IF_GCB 0x00010000 /*!< Base offset for target CFG_IF_GCB */
#define VTSS_TO_CXFI_0     0x000a0000 /*!< Base offset for target CXFI_0 */
#define VTSS_TO_UPI_1      0x000c0000 /*!< Base offset for target UPI_1 */
#define VTSS_TO_LXFI_1     0x000d0000 /*!< Base offset for target LXFI_1 */
#define VTSS_TO_CXFI_1     0x000e0000 /*!< Base offset for target CXFI_1 */

/* IO address mapping macro - may be changed for platform */
#if !defined(VTSS_IOADDR)
#define VTSS_IOADDR(t,o)        (t + (4 * (o)))
#endif

/* IO register access macro - may be changed for platform */
#if !defined(VTSS_IOREG)
/** 
 * @param t - target base offset
 * @param o - subtarget offset
 */
#define VTSS_IOREG(t,o)      (VTSS_IOADDR(t,o))
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

#ifdef VTSS_HARDWARE_WANT_TARGET_ENUMS
/*
 * This section is primarily for documentation purposes.
 */

/**
  * \brief Target IP_1588_ANA_EGR target offset(s)
  */
enum vtss_target_IP_1588_ANA_EGR_e {
    VTSS_TOE_PTP_ANA0_EGR_CFG = VTSS_TO_PTP_ANA0_EGR_CFG,
    VTSS_TOE_PTP_ANA1_EGR_CFG = VTSS_TO_PTP_ANA1_EGR_CFG,
};

/**
  * \brief Target PCS_10GBASE_R target offset(s)
  */
enum vtss_target_PCS_10GBASE_R_e {
    VTSS_TOE_PCS_1_0         = VTSS_TO_PCS_1_0,
    VTSS_TOE_PCS_2_0         = VTSS_TO_PCS_2_0,
    VTSS_TOE_PCS_1_1         = VTSS_TO_PCS_1_1,
    VTSS_TOE_PCS_2_1         = VTSS_TO_PCS_2_1,
};

/**
  * \brief Target UPI target offset(s)
  */
enum vtss_target_UPI_e {
    VTSS_TOE_UPI_0           = VTSS_TO_UPI_0,
    VTSS_TOE_UPI_1           = VTSS_TO_UPI_1,
};

/**
  * \brief Target XFI target offset(s)
  */
enum vtss_target_XFI_e {
    VTSS_TOE_LXFI_0          = VTSS_TO_LXFI_0,
    VTSS_TOE_CXFI_0          = VTSS_TO_CXFI_0,
    VTSS_TOE_LXFI_1          = VTSS_TO_LXFI_1,
    VTSS_TOE_CXFI_1          = VTSS_TO_CXFI_1,
};

/**
  * \brief Target TFI_5 target offset(s)
  */
enum vtss_target_TFI_5_e {
    VTSS_TOE_TFI_0           = VTSS_TO_TFI_0,
    VTSS_TOE_TFI_1           = VTSS_TO_TFI_1,
};

/**
  * \brief Target EWIS target offset(s)
  */
enum vtss_target_EWIS_e {
    VTSS_TOE_WIS_1_0         = VTSS_TO_WIS_1_0,
    VTSS_TOE_WIS_2_0         = VTSS_TO_WIS_2_0,
    VTSS_TOE_WIS_1_1         = VTSS_TO_WIS_1_1,
    VTSS_TOE_WIS_2_1         = VTSS_TO_WIS_2_1,
};

/**
  * \brief Target PCS_XAUI target offset(s)
  */
enum vtss_target_PCS_XAUI_e {
    VTSS_TOE_XAUI_0          = VTSS_TO_XAUI_0,
    VTSS_TOE_XAUI_1          = VTSS_TO_XAUI_1,
};

/**
  * \brief Target IP_1588_ANA_INGR target offset(s)
  */
enum vtss_target_IP_1588_ANA_INGR_e {
    VTSS_TOE_PTP_ANA0_INGR_CFG = VTSS_TO_PTP_ANA0_INGR_CFG,
    VTSS_TOE_PTP_ANA1_INGR_CFG = VTSS_TO_PTP_ANA1_INGR_CFG,
};

/**
  * \brief Target IP_1588 target offset(s)
  */
enum vtss_target_IP_1588_e {
    VTSS_TOE_PTP_0           = VTSS_TO_PTP_0,
    VTSS_TOE_PTP_1           = VTSS_TO_PTP_1,
};

/**
  * \brief Target EFECI7 target offset(s)
  */
enum vtss_target_EFECI7_e {
    VTSS_TOE_EFECI7_0        = VTSS_TO_EFECI7_0,
    VTSS_TOE_EFECI7_1        = VTSS_TO_EFECI7_1,
};

/**
  * \brief Target RAB target offset(s)
  */
enum vtss_target_RAB_e {
    VTSS_TOE_RAB_0           = VTSS_TO_RAB_0,
    VTSS_TOE_RAB_1           = VTSS_TO_RAB_1,
};

/**
  * \brief Target OTN_WRAPPER target offset(s)
  */
enum vtss_target_OTN_WRAPPER_e {
    VTSS_TOE_OTU_1_0         = VTSS_TO_OTU_1_0,
    VTSS_TOE_OTU_2_0         = VTSS_TO_OTU_2_0,
    VTSS_TOE_OTU_1_1         = VTSS_TO_OTU_1_1,
    VTSS_TOE_OTU_2_1         = VTSS_TO_OTU_2_1,
};

/**
  * \brief Target GFP target offset(s)
  */
enum vtss_target_GFP_e {
    VTSS_TOE_GFP_1_0         = VTSS_TO_GFP_1_0,
    VTSS_TOE_GFP_2_0         = VTSS_TO_GFP_2_0,
    VTSS_TOE_GFP_1_1         = VTSS_TO_GFP_1_1,
    VTSS_TOE_GFP_2_1         = VTSS_TO_GFP_2_1,
};

/**
  * \brief Target SFI4 target offset(s)
  */
enum vtss_target_SFI4_e {
    VTSS_TOE_SFI_0           = VTSS_TO_SFI_0,
    VTSS_TOE_SFI_1           = VTSS_TO_SFI_1,
};

/**
  * \brief Target MAC10G target offset(s)
  */
enum vtss_target_MAC10G_e {
    VTSS_TOE_MAC_1_0         = VTSS_TO_MAC_1_0,
    VTSS_TOE_MAC_2_0         = VTSS_TO_MAC_2_0,
    VTSS_TOE_MAC_1_1         = VTSS_TO_MAC_1_1,
    VTSS_TOE_MAC_2_1         = VTSS_TO_MAC_2_1,
};

/**
  * \brief Target EFECI4 target offset(s)
  */
enum vtss_target_EFECI4_e {
    VTSS_TOE_EFEC_0          = VTSS_TO_EFEC_0,
    VTSS_TOE_EFEC_1          = VTSS_TO_EFEC_1,
};

#endif /* VTSS_HARDWARE_WANT_TARGET_ENUMS */


#endif /* _VTSS_DAYTONA_REGS_COMMON_H_ */
