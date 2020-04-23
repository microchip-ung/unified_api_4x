#ifndef _VTSS_DAYTONA_REGS_USED_DEBUG_H_
#define _VTSS_DAYTONA_REGS_USED_DEBUG_H_

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

/**
 * \file
 * \brief This file holds defines for fields that are currently used in the initialization tables genereted from 
 *        the AN1029 init configuration sheet, but are defined as [DBG] in the documentation.
 * \details In the final release this should be removed, either because the fields are removed from the initialization,
 *          or the fields are made non[DBG] in the documentation.
 */


#define  VTSS_F_MAC10G_DEV_CFG_STATUS_MAC_RST_CTRL_PMA_XGMII_RX_BYPASS_SEL(x)  VTSS_ENCODE_BITFIELD(x,16,1)
#define  VTSS_M_MAC10G_DEV_CFG_STATUS_MAC_RST_CTRL_PMA_XGMII_RX_BYPASS_SEL     VTSS_ENCODE_BITMASK(16,1)
#define  VTSS_X_MAC10G_DEV_CFG_STATUS_MAC_RST_CTRL_PMA_XGMII_RX_BYPASS_SEL(x)  VTSS_EXTRACT_BITFIELD(x,16,1)


#define  VTSS_F_SFI4_SFI4_CONFIGURATION_SFI4_CFG_SD_ENA(x)  VTSS_ENCODE_BITFIELD(x,24,1)
#define  VTSS_M_SFI4_SFI4_CONFIGURATION_SFI4_CFG_SD_ENA     VTSS_ENCODE_BITMASK(24,1)
#define  VTSS_X_SFI4_SFI4_CONFIGURATION_SFI4_CFG_SD_ENA(x)  VTSS_EXTRACT_BITFIELD(x,24,1)

#define  VTSS_F_SFI4_SFI4_CONFIGURATION_SFI4_CFG_SD_POL(x)  VTSS_ENCODE_BITFIELD(x,25,1)
#define  VTSS_M_SFI4_SFI4_CONFIGURATION_SFI4_CFG_SD_POL     VTSS_ENCODE_BITMASK(25,1)
#define  VTSS_X_SFI4_SFI4_CONFIGURATION_SFI4_CFG_SD_POL(x)  VTSS_EXTRACT_BITFIELD(x,25,1)

#define  VTSS_F_SFI4_SFI4_CONFIGURATION_SFI4_CFG_LOS_ENA(x)  VTSS_ENCODE_BITFIELD(x,28,1)
#define  VTSS_M_SFI4_SFI4_CONFIGURATION_SFI4_CFG_LOS_ENA     VTSS_ENCODE_BITMASK(28,1)
#define  VTSS_X_SFI4_SFI4_CONFIGURATION_SFI4_CFG_LOS_ENA(x)  VTSS_EXTRACT_BITFIELD(x,28,1)

#define  VTSS_F_SFI4_SFI4_CONFIGURATION_SFI4_CFG_LOS_POL(x)  VTSS_ENCODE_BITFIELD(x,29,1)
#define  VTSS_M_SFI4_SFI4_CONFIGURATION_SFI4_CFG_LOS_POL     VTSS_ENCODE_BITMASK(29,1)
#define  VTSS_X_SFI4_SFI4_CONFIGURATION_SFI4_CFG_LOS_POL(x)  VTSS_EXTRACT_BITFIELD(x,29,1)

#define  VTSS_F_SFI4_SFI4_CONFIGURATION_SFI4_CFG_FAST_SYNC_ENA(x)  VTSS_ENCODE_BITFIELD(x,30,1)
#define  VTSS_M_SFI4_SFI4_CONFIGURATION_SFI4_CFG_FAST_SYNC_ENA     VTSS_ENCODE_BITMASK(30,1)
#define  VTSS_X_SFI4_SFI4_CONFIGURATION_SFI4_CFG_FAST_SYNC_ENA(x)  VTSS_EXTRACT_BITFIELD(x,30,1)

#define  VTSS_F_SFI4_SFI4_CONFIGURATION_SFI4_CFG_SH_CL49_ENA(x)  VTSS_ENCODE_BITFIELD(x,31,1)
#define  VTSS_M_SFI4_SFI4_CONFIGURATION_SFI4_CFG_SH_CL49_ENA     VTSS_ENCODE_BITMASK(31,1)
#define  VTSS_X_SFI4_SFI4_CONFIGURATION_SFI4_CFG_SH_CL49_ENA(x)  VTSS_EXTRACT_BITFIELD(x,31,1)



#define VTSS_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_OTU_INTERRUPT_STATUS(target)  VTSS_IOREG(target,0x1c)

#define  VTSS_F_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_OTU_INTERRUPT_STATUS_OTUK_AIS_IRQ(x)  VTSS_ENCODE_BITFIELD(x,15,1)
#define  VTSS_M_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_OTU_INTERRUPT_STATUS_OTUK_AIS_IRQ     VTSS_ENCODE_BITMASK(15,1)
#define  VTSS_X_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_OTU_INTERRUPT_STATUS_OTUK_AIS_IRQ(x)  VTSS_EXTRACT_BITFIELD(x,15,1)

#define  VTSS_F_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_OTU_INTERRUPT_STATUS_SM_TIM_IRQ(x)  VTSS_ENCODE_BITFIELD(x,14,1)
#define  VTSS_M_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_OTU_INTERRUPT_STATUS_SM_TIM_IRQ     VTSS_ENCODE_BITMASK(14,1)
#define  VTSS_X_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_OTU_INTERRUPT_STATUS_SM_TIM_IRQ(x)  VTSS_EXTRACT_BITFIELD(x,14,1)

#define  VTSS_F_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_OTU_INTERRUPT_STATUS_SM_BDI_IRQ(x)  VTSS_ENCODE_BITFIELD(x,8,1)
#define  VTSS_M_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_OTU_INTERRUPT_STATUS_SM_BDI_IRQ     VTSS_ENCODE_BITMASK(8,1)
#define  VTSS_X_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_OTU_INTERRUPT_STATUS_SM_BDI_IRQ(x)  VTSS_EXTRACT_BITFIELD(x,8,1)

#define  VTSS_F_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_OTU_INTERRUPT_STATUS_SM_IAE_IRQ(x)  VTSS_ENCODE_BITFIELD(x,7,1)
#define  VTSS_M_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_OTU_INTERRUPT_STATUS_SM_IAE_IRQ     VTSS_ENCODE_BITMASK(7,1)
#define  VTSS_X_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_OTU_INTERRUPT_STATUS_SM_IAE_IRQ(x)  VTSS_EXTRACT_BITFIELD(x,7,1)

#define  VTSS_F_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_OTU_INTERRUPT_STATUS_SM_BIAE_IRQ(x)  VTSS_ENCODE_BITFIELD(x,9,1)
#define  VTSS_M_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_OTU_INTERRUPT_STATUS_SM_BIAE_IRQ     VTSS_ENCODE_BITMASK(9,1)
#define  VTSS_X_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_OTU_INTERRUPT_STATUS_SM_BIAE_IRQ(x)  VTSS_EXTRACT_BITFIELD(x,9,1)

#define  VTSS_F_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_OTU_INTERRUPT_STATUS_SM_TTI_INCONSISTENT_IRQ(x)  VTSS_ENCODE_BITFIELD(x,12,1)
#define  VTSS_M_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_OTU_INTERRUPT_STATUS_SM_TTI_INCONSISTENT_IRQ     VTSS_ENCODE_BITMASK(12,1)
#define  VTSS_X_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_OTU_INTERRUPT_STATUS_SM_TTI_INCONSISTENT_IRQ(x)  VTSS_EXTRACT_BITFIELD(x,12,1)

#define  VTSS_F_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_OTU_INTERRUPT_STATUS_RES_INCONSISTENT_IRQ(x)  VTSS_ENCODE_BITFIELD(x,1,1)
#define  VTSS_M_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_OTU_INTERRUPT_STATUS_RES_INCONSISTENT_IRQ     VTSS_ENCODE_BITMASK(1,1)
#define  VTSS_X_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_OTU_INTERRUPT_STATUS_RES_INCONSISTENT_IRQ(x)  VTSS_EXTRACT_BITFIELD(x,1,1)

#define  VTSS_F_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_OTU_INTERRUPT_STATUS_SM_RES_INCONSISTENT_IRQ(x)  VTSS_ENCODE_BITFIELD(x,5,1)
#define  VTSS_M_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_OTU_INTERRUPT_STATUS_SM_RES_INCONSISTENT_IRQ     VTSS_ENCODE_BITMASK(5,1)
#define  VTSS_X_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_OTU_INTERRUPT_STATUS_SM_RES_INCONSISTENT_IRQ(x)  VTSS_EXTRACT_BITFIELD(x,5,1)

#define  VTSS_F_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_OTU_INTERRUPT_STATUS_GCC0_INCONSISTENT_IRQ(x)  VTSS_ENCODE_BITFIELD(x,3,1)
#define  VTSS_M_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_OTU_INTERRUPT_STATUS_GCC0_INCONSISTENT_IRQ     VTSS_ENCODE_BITMASK(3,1)
#define  VTSS_X_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_OTU_INTERRUPT_STATUS_GCC0_INCONSISTENT_IRQ(x)  VTSS_EXTRACT_BITFIELD(x,3,1)



#define VTSS_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_ODU_INTERRUPT_STATUS(target)  VTSS_IOREG(target,0x1F)

#define  VTSS_F_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_ODU_INTERRUPT_STATUS_RES1_INCONSISTENT_IRQ(x)  VTSS_ENCODE_BITFIELD(x,14,1)
#define  VTSS_M_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_ODU_INTERRUPT_STATUS_RES1_INCONSISTENT_IRQ     VTSS_ENCODE_BITMASK(14,1)
#define  VTSS_X_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_ODU_INTERRUPT_STATUS_RES1_INCONSISTENT_IRQ(x)  VTSS_EXTRACT_BITFIELD(x,14,1)

#define  VTSS_F_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_ODU_INTERRUPT_STATUS_EXP_INCONSISTENT_IRQ(x)  VTSS_ENCODE_BITFIELD(x,8,1)
#define  VTSS_M_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_ODU_INTERRUPT_STATUS_EXP_INCONSISTENT_IRQ     VTSS_ENCODE_BITMASK(8,1)
#define  VTSS_X_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_ODU_INTERRUPT_STATUS_EXP_INCONSISTENT_IRQ(x)  VTSS_EXTRACT_BITFIELD(x,8,1)

#define  VTSS_F_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_ODU_INTERRUPT_STATUS_FTFL_INCONSISTENT_IRQ(x)  VTSS_ENCODE_BITFIELD(x,10,1)
#define  VTSS_M_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_ODU_INTERRUPT_STATUS_FTFL_INCONSISTENT_IRQ     VTSS_ENCODE_BITMASK(10,1)
#define  VTSS_X_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_ODU_INTERRUPT_STATUS_FTFL_INCONSISTENT_IRQ(x)  VTSS_EXTRACT_BITFIELD(x,10,1)



#define VTSS_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_ODU_PM_INTERRUPT_STATUS(target)  VTSS_IOREG(target,0x37)

#define  VTSS_F_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_ODU_PM_INTERRUPT_STATUS_PM_TIM_IRQ(x)  VTSS_ENCODE_BITFIELD(x,15,1)
#define  VTSS_M_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_ODU_PM_INTERRUPT_STATUS_PM_TIM_IRQ     VTSS_ENCODE_BITMASK(15,1)
#define  VTSS_X_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_ODU_PM_INTERRUPT_STATUS_PM_TIM_IRQ(x)  VTSS_EXTRACT_BITFIELD(x,15,1)

#define  VTSS_F_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_ODU_PM_INTERRUPT_STATUS_PM_TTI_INCONSISTENT_IRQ(x)  VTSS_ENCODE_BITFIELD(x,13,1)
#define  VTSS_M_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_ODU_PM_INTERRUPT_STATUS_PM_TTI_INCONSISTENT_IRQ     VTSS_ENCODE_BITMASK(13,1)
#define  VTSS_X_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_ODU_PM_INTERRUPT_STATUS_PM_TTI_INCONSISTENT_IRQ(x)  VTSS_EXTRACT_BITFIELD(x,13,1)

#define  VTSS_F_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_ODU_PM_INTERRUPT_STATUS_PM_AIS_IRQ(x)  VTSS_ENCODE_BITFIELD(x,2,1)
#define  VTSS_M_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_ODU_PM_INTERRUPT_STATUS_PM_AIS_IRQ     VTSS_ENCODE_BITMASK(2,1)
#define  VTSS_X_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_ODU_PM_INTERRUPT_STATUS_PM_AIS_IRQ(x)  VTSS_EXTRACT_BITFIELD(x,2,1)

#define  VTSS_F_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_ODU_PM_INTERRUPT_STATUS_PM_BDI_IRQ(x)  VTSS_ENCODE_BITFIELD(x,9,1)
#define  VTSS_M_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_ODU_PM_INTERRUPT_STATUS_PM_BDI_IRQ     VTSS_ENCODE_BITMASK(9,1)
#define  VTSS_X_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_ODU_PM_INTERRUPT_STATUS_PM_BDI_IRQ(x)  VTSS_EXTRACT_BITFIELD(x,9,1)

#define  VTSS_F_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_ODU_PM_INTERRUPT_STATUS_PM_OCI_IRQ(x)  VTSS_ENCODE_BITFIELD(x,3,1)
#define  VTSS_M_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_ODU_PM_INTERRUPT_STATUS_PM_OCI_IRQ     VTSS_ENCODE_BITMASK(3,1)
#define  VTSS_X_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_ODU_PM_INTERRUPT_STATUS_PM_OCI_IRQ(x)  VTSS_EXTRACT_BITFIELD(x,3,1)

#define  VTSS_F_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_ODU_PM_INTERRUPT_STATUS_PM_LCK_IRQ(x)  VTSS_ENCODE_BITFIELD(x,4,1)
#define  VTSS_M_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_ODU_PM_INTERRUPT_STATUS_PM_LCK_IRQ     VTSS_ENCODE_BITMASK(4,1)
#define  VTSS_X_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_ODU_PM_INTERRUPT_STATUS_PM_LCK_IRQ(x)  VTSS_EXTRACT_BITFIELD(x,4,1)



#define VTSS_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_ODU_TCMX_INTERRUPT_STATUS(target,ri)  VTSS_IOREG(target,0x25 + (ri))

#define  VTSS_F_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_ODU_TCMX_INTERRUPT_STATUS_RX_TCMI_IAE_IRQ(x)  VTSS_ENCODE_BITFIELD(x,1,1)
#define  VTSS_M_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_ODU_TCMX_INTERRUPT_STATUS_RX_TCMI_IAE_IRQ     VTSS_ENCODE_BITMASK(1,1)
#define  VTSS_X_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_ODU_TCMX_INTERRUPT_STATUS_RX_TCMI_IAE_IRQ(x)  VTSS_EXTRACT_BITFIELD(x,1,1)

#define  VTSS_F_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_ODU_TCMX_INTERRUPT_STATUS_RX_TCMI_AIS_IRQ(x)  VTSS_ENCODE_BITFIELD(x,2,1)
#define  VTSS_M_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_ODU_TCMX_INTERRUPT_STATUS_RX_TCMI_AIS_IRQ     VTSS_ENCODE_BITMASK(2,1)
#define  VTSS_X_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_ODU_TCMX_INTERRUPT_STATUS_RX_TCMI_AIS_IRQ(x)  VTSS_EXTRACT_BITFIELD(x,2,1)

#define  VTSS_F_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_ODU_TCMX_INTERRUPT_STATUS_RX_TCMI_OCI_IRQ(x)  VTSS_ENCODE_BITFIELD(x,3,1)
#define  VTSS_M_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_ODU_TCMX_INTERRUPT_STATUS_RX_TCMI_OCI_IRQ     VTSS_ENCODE_BITMASK(3,1)
#define  VTSS_X_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_ODU_TCMX_INTERRUPT_STATUS_RX_TCMI_OCI_IRQ(x)  VTSS_EXTRACT_BITFIELD(x,3,1)

#define  VTSS_F_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_ODU_TCMX_INTERRUPT_STATUS_RX_TCMI_LCK_IRQ(x)  VTSS_ENCODE_BITFIELD(x,4,1)
#define  VTSS_M_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_ODU_TCMX_INTERRUPT_STATUS_RX_TCMI_LCK_IRQ     VTSS_ENCODE_BITMASK(4,1)
#define  VTSS_X_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_ODU_TCMX_INTERRUPT_STATUS_RX_TCMI_LCK_IRQ(x)  VTSS_EXTRACT_BITFIELD(x,4,1)

#define  VTSS_F_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_ODU_TCMX_INTERRUPT_STATUS_RX_TCMI_LTC_IRQ(x)  VTSS_ENCODE_BITFIELD(x,5,1)
#define  VTSS_M_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_ODU_TCMX_INTERRUPT_STATUS_RX_TCMI_LTC_IRQ     VTSS_ENCODE_BITMASK(5,1)
#define  VTSS_X_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_ODU_TCMX_INTERRUPT_STATUS_RX_TCMI_LTC_IRQ(x)  VTSS_EXTRACT_BITFIELD(x,5,1)

#define  VTSS_F_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_ODU_TCMX_INTERRUPT_STATUS_RX_TCMI_STAT_INCONSISTENT_IRQ(x)  VTSS_ENCODE_BITFIELD(x,6,1)
#define  VTSS_M_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_ODU_TCMX_INTERRUPT_STATUS_RX_TCMI_STAT_INCONSISTENT_IRQ     VTSS_ENCODE_BITMASK(6,1)
#define  VTSS_X_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_ODU_TCMX_INTERRUPT_STATUS_RX_TCMI_STAT_INCONSISTENT_IRQ(x)  VTSS_EXTRACT_BITFIELD(x,6,1)

#define  VTSS_F_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_ODU_TCMX_INTERRUPT_STATUS_RX_TCMI_BDI_IRQ(x)  VTSS_ENCODE_BITFIELD(x,9,1)
#define  VTSS_M_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_ODU_TCMX_INTERRUPT_STATUS_RX_TCMI_BDI_IRQ     VTSS_ENCODE_BITMASK(9,1)
#define  VTSS_X_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_ODU_TCMX_INTERRUPT_STATUS_RX_TCMI_BDI_IRQ(x)  VTSS_EXTRACT_BITFIELD(x,9,1)

#define  VTSS_F_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_ODU_TCMX_INTERRUPT_STATUS_RX_TCMI_BIAE_IRQ(x)  VTSS_ENCODE_BITFIELD(x,10,1)
#define  VTSS_M_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_ODU_TCMX_INTERRUPT_STATUS_RX_TCMI_BIAE_IRQ     VTSS_ENCODE_BITMASK(10,1)
#define  VTSS_X_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_ODU_TCMX_INTERRUPT_STATUS_RX_TCMI_BIAE_IRQ(x)  VTSS_EXTRACT_BITFIELD(x,10,1)

#define  VTSS_F_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_ODU_TCMX_INTERRUPT_STATUS_RX_TCMI_TTI_INCONSISTENT_IRQ(x)  VTSS_ENCODE_BITFIELD(x,13,1)
#define  VTSS_M_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_ODU_TCMX_INTERRUPT_STATUS_RX_TCMI_TTI_INCONSISTENT_IRQ     VTSS_ENCODE_BITMASK(13,1)
#define  VTSS_X_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_ODU_TCMX_INTERRUPT_STATUS_RX_TCMI_TTI_INCONSISTENT_IRQ(x)  VTSS_EXTRACT_BITFIELD(x,13,1)

#define  VTSS_F_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_ODU_TCMX_INTERRUPT_STATUS_RX_TCMI_TIM_IRQ(x)  VTSS_ENCODE_BITFIELD(x,15,1)
#define  VTSS_M_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_ODU_TCMX_INTERRUPT_STATUS_RX_TCMI_TIM_IRQ     VTSS_ENCODE_BITMASK(15,1)
#define  VTSS_X_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_ODU_TCMX_INTERRUPT_STATUS_RX_TCMI_TIM_IRQ(x)  VTSS_EXTRACT_BITFIELD(x,15,1)



#define VTSS_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_ODU_APS_PCC_IRQ_STATUS(target)  VTSS_IOREG(target,0x22)

#define  VTSS_F_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_ODU_APS_PCC_IRQ_STATUS_APSPCC_MFAS0_INCONSISTENT_IRQ(x)  VTSS_ENCODE_BITFIELD(x,0,1)
#define  VTSS_M_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_ODU_APS_PCC_IRQ_STATUS_APSPCC_MFAS0_INCONSISTENT_IRQ     VTSS_ENCODE_BITMASK(0,1)
#define  VTSS_X_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_ODU_APS_PCC_IRQ_STATUS_APSPCC_MFAS0_INCONSISTENT_IRQ(x)  VTSS_EXTRACT_BITFIELD(x,0,1)

#define  VTSS_F_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_ODU_APS_PCC_IRQ_STATUS_APSPCC_MFAS1_INCONSISTENT_IRQ(x)  VTSS_ENCODE_BITFIELD(x,1,1)
#define  VTSS_M_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_ODU_APS_PCC_IRQ_STATUS_APSPCC_MFAS1_INCONSISTENT_IRQ     VTSS_ENCODE_BITMASK(1,1)
#define  VTSS_X_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_ODU_APS_PCC_IRQ_STATUS_APSPCC_MFAS1_INCONSISTENT_IRQ(x)  VTSS_EXTRACT_BITFIELD(x,1,1)



#define VTSS_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_OPU_INTERRUPT_STATUS(target)  VTSS_IOREG(target,0x3a)

#define  VTSS_F_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_OPU_INTERRUPT_STATUS_RX_PLM_IRQ(x)  VTSS_ENCODE_BITFIELD(x,15,1)
#define  VTSS_M_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_OPU_INTERRUPT_STATUS_RX_PLM_IRQ     VTSS_ENCODE_BITMASK(15,1)
#define  VTSS_X_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_OPU_INTERRUPT_STATUS_RX_PLM_IRQ(x)  VTSS_EXTRACT_BITFIELD(x,15,1)

#define  VTSS_F_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_OPU_INTERRUPT_STATUS_RX_PT_INCONSISTENT_IRQ(x)  VTSS_ENCODE_BITFIELD(x,13,1)
#define  VTSS_M_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_OPU_INTERRUPT_STATUS_RX_PT_INCONSISTENT_IRQ     VTSS_ENCODE_BITMASK(13,1)
#define  VTSS_X_OTN_WRAPPER_WRAPPER_GROUP_RX_OTN_OPU_INTERRUPT_STATUS_RX_PT_INCONSISTENT_IRQ(x)  VTSS_EXTRACT_BITFIELD(x,13,1)



#define  VTSS_F_OTN_WRAPPER_XCO2_TX_OTN_XCO2_TX_OTN_GLOBAL_CONTROL_TX_TERMINAL_LOOPBACK(x)  VTSS_ENCODE_BITFIELD(x,0,1)
#define  VTSS_M_OTN_WRAPPER_XCO2_TX_OTN_XCO2_TX_OTN_GLOBAL_CONTROL_TX_TERMINAL_LOOPBACK       VTSS_ENCODE_BITMASK(0,1)
#define  VTSS_X_OTN_WRAPPER_XCO2_TX_OTN_XCO2_TX_OTN_GLOBAL_CONTROL_TX_TERMINAL_LOOPBACK(x)  VTSS_EXTRACT_BITFIELD(x,0,1)


#define  VTSS_F_OTN_WRAPPER_XCO2_TX_OTN_XCO2_TX_OTN_GLOBAL_CONTROL_TX_FACILITY_LOOPBACK(x)  VTSS_ENCODE_BITFIELD(x,1,1)
#define  VTSS_M_OTN_WRAPPER_XCO2_TX_OTN_XCO2_TX_OTN_GLOBAL_CONTROL_TX_FACILITY_LOOPBACK     VTSS_ENCODE_BITMASK(1,1)
#define  VTSS_X_OTN_WRAPPER_XCO2_TX_OTN_XCO2_TX_OTN_GLOBAL_CONTROL_TX_FACILITY_LOOPBACK(x)  VTSS_EXTRACT_BITFIELD(x,1,1)

#define  VTSS_F_EWIS_RX_WIS_CTRL_MISC_LOH_CTRL_LINE_RX_FORCE_AIS_L(x)  VTSS_ENCODE_BITFIELD(x,25,1)
#define  VTSS_M_EWIS_RX_WIS_CTRL_MISC_LOH_CTRL_LINE_RX_FORCE_AIS_L     VTSS_ENCODE_BITMASK(25,1)
#define  VTSS_X_EWIS_RX_WIS_CTRL_MISC_LOH_CTRL_LINE_RX_FORCE_AIS_L(x)  VTSS_EXTRACT_BITFIELD(x,25,1)

#define  VTSS_F_EWIS_RX_WIS_CTRL_MISC_LOH_CTRL_LINE_RX_FORCE_RDI_L(x)  VTSS_ENCODE_BITFIELD(x,24,1)
#define  VTSS_M_EWIS_RX_WIS_CTRL_MISC_LOH_CTRL_LINE_RX_FORCE_RDI_L     VTSS_ENCODE_BITMASK(24,1)
#define  VTSS_X_EWIS_RX_WIS_CTRL_MISC_LOH_CTRL_LINE_RX_FORCE_RDI_L(x)  VTSS_EXTRACT_BITFIELD(x,24,1)

#define  VTSS_F_EWIS_RX_WIS_CTRL_MISC_LOH_CTRL_LINE_TX_FORCE_AIS_L(x)  VTSS_ENCODE_BITFIELD(x,9,1)
#define  VTSS_M_EWIS_RX_WIS_CTRL_MISC_LOH_CTRL_LINE_TX_FORCE_AIS_L     VTSS_ENCODE_BITMASK(9,1)
#define  VTSS_X_EWIS_RX_WIS_CTRL_MISC_LOH_CTRL_LINE_TX_FORCE_AIS_L(x)  VTSS_EXTRACT_BITFIELD(x,9,1)

#define  VTSS_F_EWIS_RX_WIS_CTRL_MISC_LOH_CTRL_LINE_TX_FORCE_RDI_L(x)  VTSS_ENCODE_BITFIELD(x,8,1)
#define  VTSS_M_EWIS_RX_WIS_CTRL_MISC_LOH_CTRL_LINE_TX_FORCE_RDI_L     VTSS_ENCODE_BITMASK(8,1)
#define  VTSS_X_EWIS_RX_WIS_CTRL_MISC_LOH_CTRL_LINE_TX_FORCE_RDI_L(x)  VTSS_EXTRACT_BITFIELD(x,8,1)

#define  VTSS_F_EWIS_RX_WIS_CTRL_MISC_SOH_CTRL_B1_PERF_MODE(x)     VTSS_ENCODE_BITFIELD(x,16,1)
#define  VTSS_M_EWIS_RX_WIS_CTRL_MISC_SOH_CTRL_B1_PERF_MODE_ENA    VTSS_ENCODE_BITMASK(16,1)
#define  VTSS_X_EWIS_RX_WIS_CTRL_MISC_SOH_CTRL_B1_PERF_MODE_ENA(x) VTSS_EXTRACT_BITFIELD(x,16,1)

#define  VTSS_F_EWIS_RX_WIS_CTRL_MISC_LOH_CTRL_B2_PERF_MODE(x)     VTSS_ENCODE_BITFIELD(x,0,1)
#define  VTSS_M_EWIS_RX_WIS_CTRL_MISC_LOH_CTRL_B2_PERF_MODE_ENA    VTSS_ENCODE_BITMASK(0,1)
#define  VTSS_X_EWIS_RX_WIS_CTRL_MISC_LOH_CTRL_B2_PERF_MODE_ENA(x) VTSS_EXTRACT_BITFIELD(x,0,1)

#define  VTSS_F_EWIS_RX_WIS_CTRL_MISC_LOH_CTRL_REIL_PERF_MODE(x)     VTSS_ENCODE_BITFIELD(x,4,1)
#define  VTSS_M_EWIS_RX_WIS_CTRL_MISC_LOH_CTRL_REIL_PERF_MODE_ENA    VTSS_ENCODE_BITMASK(4,1)
#define  VTSS_X_EWIS_RX_WIS_CTRL_MISC_LOH_CTRL_REIL_PERF_MODE_ENA(x) VTSS_EXTRACT_BITFIELD(x,4,1)

#define  VTSS_F_EWIS_RX_WIS_CTRL_MISC_POH_CTRL_B3_PERF_MODE(x)  VTSS_ENCODE_BITFIELD(x,8,1)
#define  VTSS_M_EWIS_RX_WIS_CTRL_MISC_POH_CTRL_B3_PERF_MODE_ENA     VTSS_ENCODE_BITMASK(8,1)
#define  VTSS_X_EWIS_RX_WIS_CTRL_MISC_POH_CTRL_B3_PERF_MODE(x)  VTSS_EXTRACT_BITFIELD(x,8,1)

#define  VTSS_F_EWIS_RX_WIS_CTRL_MISC_POH_CTRL_REIP_PERF_MODE(x)  VTSS_ENCODE_BITFIELD(x,16,1)
#define  VTSS_M_EWIS_RX_WIS_CTRL_MISC_POH_CTRL_REIP_PERF_MODE_ENA     VTSS_ENCODE_BITMASK(16,1)
#define  VTSS_X_EWIS_RX_WIS_CTRL_MISC_POH_CTRL_REIP_PERF_MODE(x)  VTSS_EXTRACT_BITFIELD(x,16,1)

/** 
 * \brief Tx Pause Frame Counter
 *
 * \details
 * Register: \a MAC10G::DEV_STATISTICS::TX_PAUSE_CNT
 *
 * @param target A \a ::vtss_target_MAC10G_e target
 */
#define VTSS_MAC10G_DEV_STATISTICS_TX_PAUSE_CNT(target)  VTSS_IOREG(target,0x96)

/** 
 * \brief
 * The number of pause control frames transmitted.
 *
 * \details 
 * Counter can be written by SW. Value will automatically wrap to zero.
 *
 * Field: ::VTSS_MAC10G_DEV_STATISTICS_TX_PAUSE_CNT . TX_PAUSE_CNT
 */
#define  VTSS_F_MAC10G_DEV_STATISTICS_TX_PAUSE_CNT_TX_PAUSE_CNT(x)  VTSS_ENCODE_BITFIELD(x,0,32)
#define  VTSS_M_MAC10G_DEV_STATISTICS_TX_PAUSE_CNT_TX_PAUSE_CNT     VTSS_ENCODE_BITMASK(0,32)
#define  VTSS_X_MAC10G_DEV_STATISTICS_TX_PAUSE_CNT_TX_PAUSE_CNT(x)  VTSS_EXTRACT_BITFIELD(x,0,32)

/**
 * \brief GFPM RX GFP Client Interrupt Status
 *
 * \details
 * Register: \a GFP::RX_GFP_CLIENT_INTERRUPT_STATUS
 *
 * @param target A \a ::vtss_target_GFP_e target
 */
#define VTSS_GFP_GFPM_RX_GFP_ISR_RX_GFP_CLIENT_INTERRUPT_STATUS(target)  VTSS_IOREG(target,0xe3)

/**
 * \brief
 * Client Management Inconsistent Status
 * The client mgmt accept status bit is set when an 
 * incoming client management inconsistent 
 * condition is detected.
 *
 * \details
 * Field: ::VTSS_GFP_GFPM_RX_GFP_ISR_RX_GFP_CLIENT_INTERRUPT_STATUS . CLIENT_MGMT_INCONSISTENT_STATUS
 */
#define  VTSS_F_GFP_GFPM_RX_GFP_ISR_RX_GFP_CLIENT_INTERRUPT_STATUS_CLIENT_MGMT_INCONSISTENT_STATUS(x)  VTSS_ENCODE_BITFIELD(x,1,1)
#define  VTSS_M_GFP_GFPM_RX_GFP_ISR_RX_GFP_CLIENT_INTERRUPT_STATUS_CLIENT_MGMT_INCONSISTENT_STATUS     VTSS_ENCODE_BITMASK(1,1)
#define  VTSS_X_GFP_GFPM_RX_GFP_ISR_RX_GFP_CLIENT_INTERRUPT_STATUS_CLIENT_MGMT_INCONSISTENT_STATUS(x)  VTSS_EXTRACT_BITFIELD(x,15,1)


/**
 * Register Group: \a DEVCPU_GCB::CHIP_REGS
 *
 * Miscellaneous Registers
 */


/**
 * \brief Contains the Mux Controls for Channel 0 MAC and PCS XGMII and RAB clocks
 *
 * \details
 * Register: \a DEVCPU_GCB::CHIP_MUX::MAC_CLK_MUX_0
 */

//#define VTSS_DEVCPU_GCB_CHIP_MUX_MAC_CLK_MUX_0    VTSS_IOREG(VTSS_TO_CFG_IF_GCB,0xD3)


/**
 * \brief
 * Mux control for PCS1 RX XGMII and MAC RX clock in Channel 0
 *
 * \details
 * 0 = Client TX clock is source.
 * 1 = Line RX clock is source.
 * 2 = Client TX gapped clock is source.
 * 3 = Line RX gapped clock is source.
 * 7:4 = RESERVED
 *
 * Field: ::VTSS_DEVCPU_GCB_CHIP_MUX_MAC_CLK_MUX_0 . MAC2_RX_CLK_MUX_0
 */
#define  VTSS_F_DEVCPU_GCB_CHIP_MUX_MAC_CLK_MUX_0_MAC2_RX_CLK_MUX_0(x)  VTSS_ENCODE_BITFIELD(x,16,3)
#define  VTSS_M_DEVCPU_GCB_CHIP_MUX_MAC_CLK_MUX_0_MAC2_RX_CLK_MUX_0     VTSS_ENCODE_BITMASK(16,3)
#define  VTSS_X_DEVCPU_GCB_CHIP_MUX_MAC_CLK_MUX_0_MAC2_RX_CLK_MUX_0(x)  VTSS_EXTRACT_BITFIELD(x,16,3)

/**
 * \brief
 * Mux control for PCS1 RX XGMII and MAC RX clock in Channel 0
 *
 * \details
 * 0 = Client RX clock is source.
 * 1 = Line TX clock is source.
 * 2 = Client RX gapped clock is source.
 * 3 = Line TX gapped clock is source.
 * 7:4 = RESERVED
 *
 * Field: ::VTSS_DEVCPU_GCB_CHIP_MUX_MAC_CLK_MUX_0 . MAC1_RX_CLK_MUX_0
 */
#define  VTSS_F_DEVCPU_GCB_CHIP_MUX_MAC_CLK_MUX_0_MAC1_RX_CLK_MUX_0(x)  VTSS_ENCODE_BITFIELD(x,0,3)
#define  VTSS_M_DEVCPU_GCB_CHIP_MUX_MAC_CLK_MUX_0_MAC1_RX_CLK_MUX_0     VTSS_ENCODE_BITMASK(0,3)
#define  VTSS_X_DEVCPU_GCB_CHIP_MUX_MAC_CLK_MUX_0_MAC1_RX_CLK_MUX_0(x)  VTSS_EXTRACT_BITFIELD(x,0,3)




/**
 * \brief Contains the Mux Controls for Channel 1 MAC and PCS XGMII and RAB clocks
 *
 * \details
 * Register: \a DEVCPU_GCB::CHIP_MUX::MAC_CLK_MUX_1
 */

//#define VTSS_DEVCPU_GCB_CHIP_MUX_MAC_CLK_MUX_1    VTSS_IOREG(VTSS_TO_CFG_IF_GCB,0xDC)


/**
 * \brief
 * Mux control for PCS1 RX XGMII and MAC RX clock in Channel 1
 *
 * \details
 * 0 = Client TX clock is source.
 * 1 = Line RX clock is source.
 * 2 = Client TX gapped clock is source.
 * 3 = Line RX gapped clock is source.
 * 7:4 = RESERVED
 *
 * Field: ::VTSS_DEVCPU_GCB_CHIP_MUX_MAC_CLK_MUX_1 . MAC2_RX_CLK_MUX_1
 */
#define  VTSS_F_DEVCPU_GCB_CHIP_MUX_MAC_CLK_MUX_1_MAC2_RX_CLK_MUX_1(x)  VTSS_ENCODE_BITFIELD(x,16,3)
#define  VTSS_M_DEVCPU_GCB_CHIP_MUX_MAC_CLK_MUX_1_MAC2_RX_CLK_MUX_1     VTSS_ENCODE_BITMASK(16,3)
#define  VTSS_X_DEVCPU_GCB_CHIP_MUX_MAC_CLK_MUX_1_MAC2_RX_CLK_MUX_1(x)  VTSS_EXTRACT_BITFIELD(x,16,3)

/**
 * \brief
 * Mux control for PCS1 RX XGMII and MAC RX clock in Channel 1
 *
 * \details
 * 0 = Client RX clock is source.
 * 1 = Line TX clock is source.
 * 2 = Client RX gapped clock is source.
 * 3 = Line TX gapped clock is source.
 * 7:4 = RESERVED
 *
 * Field: ::VTSS_DEVCPU_GCB_CHIP_MUX_MAC_CLK_MUX_1 . MAC1_RX_CLK_MUX_1
 */
#define  VTSS_F_DEVCPU_GCB_CHIP_MUX_MAC_CLK_MUX_1_MAC1_RX_CLK_MUX_1(x)  VTSS_ENCODE_BITFIELD(x,0,3)
#define  VTSS_M_DEVCPU_GCB_CHIP_MUX_MAC_CLK_MUX_1_MAC1_RX_CLK_MUX_1     VTSS_ENCODE_BITMASK(0,3)
#define  VTSS_X_DEVCPU_GCB_CHIP_MUX_MAC_CLK_MUX_1_MAC1_RX_CLK_MUX_1(x)  VTSS_EXTRACT_BITFIELD(x,0,3)


#endif /* _VTSS_DAYTONA_REGS_USED_DEBUG_H_ */
