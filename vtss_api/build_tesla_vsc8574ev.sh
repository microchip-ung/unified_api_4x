#
#
# Copyright (c) 2002-2017 Microsemi Corporation "Microsemi". All Rights Reserved.
#
# Unpublished rights reserved under the copyright laws of the United States of
# America, other countries and international treaties. Permission to use, copy,
# store and modify, the software and its source code is granted but only in
# connection with products utilizing the Microsemi switch and PHY products.
# Permission is also granted for you to integrate into other products, disclose,
# transmit and distribute the software only in an absolute machine readable format
# (e.g. HEX file) and only in or with products utilizing the Microsemi switch and
# PHY products.  The source code of the software may not be disclosed, transmitted
# or distributed without the prior written permission of Microsemi.
#
# This copyright notice must appear in any copy, modification, disclosure,
# transmission or distribution of the software.  Microsemi retains all ownership,
# copyright, trade secret and proprietary rights in the software and its source code,
# including all modifications thereto.
#
# THIS SOFTWARE HAS BEEN PROVIDED "AS IS". MICROSEMI HEREBY DISCLAIMS ALL WARRANTIES
# OF ANY KIND WITH RESPECT TO THE SOFTWARE, WHETHER SUCH WARRANTIES ARE EXPRESS,
# IMPLIED, STATUTORY OR OTHERWISE INCLUDING, WITHOUT LIMITATION, WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR USE OR PURPOSE AND NON-INFRINGEMENT.
#
# Note: When compiling for an EVAL BOARD with a PHY ADDR that is NOT 0,
#       The VTSS_OPT_PORT_COUNT has to be compensated for the difference.
##
## -DTESLA_ING_TS_ERRFIX 

gcc -g -o vtss_api_tesla_vsc8574ev -I include -DVTSS_OPSYS_LINUX=1  -DVTSS_OPT_PORT_COUNT=4  -DVTSS_CHIP_CU_PHY -DTESLA_EVAL_BOARD -DVTSS_FEATURE_EEE -D_INCLUDE_DEBUG_TERM_PRINT_ -D_INCLUDE_DEBUG_FILE_PRINT_ -DVTSS_SW_OPTION_EEE -DVTSS_FEATURE_SERDES_MACRO_SETTINGS -DVTSS_FEATURE_PHY_TIMESTAMP  -DTESLA_ING_TS_ERRFIX  -DVTSS_TS_FIFO_SYNC -DVTSS_TS_FIFO_MEDIA_SWAP_SYNC appl/vtss_appl_board_tesla_eval.c appl/vtss_appl_cu_phy.c base/ail/vtss_api.c  base/ail/vtss_common.c  base/phy/common/vtss_phy_common.c base/phy/ts/vtss_phy_ts_api.c base/phy/ts/vtss_phy_ts_util.c  `find base/phy/phy_1g -name "*.c"`
##
## TIMESTAMPING ENABLED ON EVAL BOARD, DEBUG->Term, Tesla OOS code enabled, Run OSS from PHY Status call and from Media Swap, Board Addr = 0
##gcc -g -o vtss_api_tesla_vsc8574ev -I include -DVTSS_OPSYS_LINUX=1  -DVTSS_OPT_PORT_COUNT=4  -DVTSS_CHIP_CU_PHY -DTESLA_EVAL_BOARD  -D_INCLUDE_DEBUG_TERM_PRINT_  -DVTSS_FEATURE_EEE -DVTSS_SW_OPTION_EEE -DVTSS_FEATURE_SERDES_MACRO_SETTINGS -DVTSS_FEATURE_PHY_TIMESTAMP -DTESLA_ING_TS_ERRFIX -DVTSS_TS_FIFO_SYNC -DVTSS_TS_FIFO_MEDIA_SWAP_SYNC appl/vtss_appl_board_tesla_eval.c appl/vtss_appl_cu_phy.c base/ail/vtss_api.c  base/ail/vtss_common.c  base/phy/common/vtss_phy_common.c base/phy/ts/vtss_phy_ts_api.c base/phy/ts/vtss_phy_ts_util.c  `find base/phy/phy_1g -name "*.c"`
## TIMESTAMPING ENABLED ON EVAL BOARD, DEBUG->File, Tesla OOS code enabled, Run OSS from PHY Status call and from Media Swap, Board Addr = 0
##gcc -g -o vtss_api_tesla_vsc8574ev -I include -DVTSS_OPSYS_LINUX=1  -DVTSS_OPT_PORT_COUNT=4  -DVTSS_CHIP_CU_PHY -DTESLA_EVAL_BOARD  -D_INCLUDE_DEBUG_FILE_PRINT_  -DVTSS_FEATURE_EEE -DVTSS_SW_OPTION_EEE -DVTSS_FEATURE_SERDES_MACRO_SETTINGS -DVTSS_FEATURE_PHY_TIMESTAMP -DTESLA_ING_TS_ERRFIX -DVTSS_TS_FIFO_SYNC -DVTSS_TS_FIFO_MEDIA_SWAP_SYNC appl/vtss_appl_board_tesla_eval.c appl/vtss_appl_cu_phy.c base/ail/vtss_api.c  base/ail/vtss_common.c  base/phy/common/vtss_phy_common.c base/phy/ts/vtss_phy_ts_api.c base/phy/ts/vtss_phy_ts_util.c  `find base/phy/phy_1g -name "*.c"`
#
## TIMESTAMPING ENABLED ON EVAL BOARD, Board Addr = 0
##gcc -g -o vtss_api_tesla_vsc8574ev -I include -DVTSS_OPSYS_LINUX=1  -DVTSS_OPT_PORT_COUNT=4  -DVTSS_CHIP_CU_PHY -DTESLA_EVAL_BOARD  -D_INCLUDE_DEBUG_TERM_PRINT_  -DVTSS_FEATURE_EEE -DVTSS_SW_OPTION_EEE -DVTSS_FEATURE_SERDES_MACRO_SETTINGS -DVTSS_FEATURE_PHY_TIMESTAMP appl/vtss_appl_board_tesla_eval.c appl/vtss_appl_cu_phy.c base/ail/vtss_api.c  base/ail/vtss_common.c  base/phy/common/vtss_phy_common.c base/phy/ts/vtss_phy_ts_api.c  `find base/phy/phy_1g -name "*.c"`
## TIMESTAMPING ENABLED ON EVAL BOARD, Board Addr = 4
#3gcc -g -o vtss_api_tesla_vsc8574ev -I include -DVTSS_OPSYS_LINUX=1  -DVTSS_OPT_PORT_COUNT=8  -DVTSS_CHIP_CU_PHY -DTESLA_EVAL_BOARD -D_INCLUDE_DEBUG_TERM_PRINT_ -DVTSS_FEATURE_EEE -DVTSS_SW_OPTION_EEE -DVTSS_FEATURE_SERDES_MACRO_SETTINGS -DVTSS_FEATURE_PHY_TIMESTAMP appl/vtss_appl_board_tesla_eval.c appl/vtss_appl_cu_phy.c base/ail/vtss_api.c  base/ail/vtss_common.c  base/phy/common/vtss_phy_common.c base/phy/ts/vtss_phy_ts_api.c  `find base/phy/phy_1g -name "*.c"`
##
## NO TIMESTAMPING ENABLED ON EVAL BOARD
##gcc  -g -o vtss_api_tesla.exe  -I include -DVTSS_OPSYS_LINUX=1  -DVTSS_OPT_PORT_COUNT=4  -DVTSS_CHIP_CU_PHY -DTESLA_EVAL_BOARD -DVTSS_FEATURE_EEE -DVTSS_SW_OPTION_EEE -D_INCLUDE_DEBUG_TERM_PRINT_ appl/vtss_appl_board_tesla_eval.c appl/vtss_appl_cu_phy.c base/ail/vtss_api.c  base/ail/vtss_common.c  `find base/phy/phy_1g -name "*.c"` 
##
## Available Defines:
##  -DVTSS_API_INIT_SEQ_NORMAL /* Customer environment based PHY Init Seq */
##  -DVTSS_API_INIT_SEQ_WARM   /* Customer environment based Warmstart */
##  -DVTSS_TS_DEBUG_REG_DUMP   /* Only defined if TS Reg Dump Debug code included */
##  -DVTSS_OPSYS_LINUX=1
##  -DVTSS_OPT_PORT_COUNT=4
##  -DVTSS_CHIP_CU_PHY
##  -DTESLA_EVAL_BOARD
##  -DVTSS_FEATURE_EEE
##  -DVTSS_SW_OPTION_EEE
##  -D_INCLUDE_DEBUG_FILE_PRINT_
##  -D_INCLUDE_DEBUG_TERM_PRINT_
##  -DVTSS_PHY_OPT_CAP_FE_ONLY
##  -DVTSS_PHY_OPT_CAP_GE_ONLY
##  -DVTSS_FEATURE_SERDES_MACRO_SETTINGS
##  -DVTSS_FEATURE_PHY_TIMESTAMP
##  -DTESLA_ING_TS_ERRFIX
##  -DVTSS_TS_FIFO_SYNC
##  -DVTSS_TS_FIFO_MEDIA_SWAP_SYNC
