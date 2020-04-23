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
##
## TIMESTAMPING & MACSEC ENABLED ON EVAL BOARD - vtss_appl_cli DEMO code
gcc -g -o vtss_api_viper_cli.exe  -I include -I boards -DVTSS_OPSYS_LINUX=1  -DVTSS_OPT_PORT_COUNT=4  -DVTSS_CHIP_CU_PHY -DVIPER_EVAL_BOARD -DVTSS_FEATURE_EEE -DVTSS_SW_OPTION_EEE -DVTSS_FEATURE_PHY_TIMESTAMP -DVTSS_FEATURE_MACSEC appl/vtss_appl_board_viper_eval.c appl/vtss_version.c appl/vtss_appl.c appl/vtss_appl_cli.c base/ail/vtss_api.c  base/ail/vtss_common.c  base/phy/common/vtss_phy_common.c base/phy/ts/vtss_phy_ts_api.c base/phy/macsec/vtss_macsec_api.c `find base/phy/phy_1g -name "*.c"`
##
## TIMESTAMPING & MACSEC ENABLED ON EVAL BOARD - vtss_appl_cu_phy DEMO code
## gcc -g -o vtss_api_viper.exe  -I include -I boards -DVTSS_OPSYS_LINUX=1  -DVTSS_OPT_PORT_COUNT=4  -DVTSS_CHIP_CU_PHY -DVIPER_EVAL_BOARD -DVTSS_FEATURE_EEE -DVTSS_SW_OPTION_EEE -DVTSS_FEATURE_PHY_TIMESTAMP -DVTSS_FEATURE_MACSEC appl/vtss_appl_board_viper_eval.c appl/vtss_appl_cu_phy.c base/ail/vtss_api.c  base/ail/vtss_common.c  base/phy/common/vtss_phy_common.c base/phy/ts/vtss_phy_ts_api.c base/phy/macsec/vtss_macsec_api.c `find base/phy/phy_1g -name "*.c"`
##
## TIMESTAMPING & MACSEC NOT ENABLED ON EVAL BOARD
##gcc -g -o vtss_api_viper.exe  -I include -I base/phy/common -DVTSS_OPSYS_LINUX=1  -DVTSS_OPT_PORT_COUNT=4  -DVTSS_CHIP_CU_PHY -DVIPER_EVAL_BOARD -DVTSS_FEATURE_MACSEC -DVTSS_FEATURE_EEE -DVTSS_SW_OPTION_EEE appl/vtss_appl_board_viper_eval.c appl/vtss_appl_cu_phy.c base/ail/vtss_api.c  base/ail/vtss_common.c  base/phy/common/vtss_phy_common.c base/phy/macsec/vtss_macsec_api.c `find base/phy/phy_1g -name "*.c"` 
## Available Defines:
##  -DVTSS_OPSYS_LINUX=1
##  -DVTSS_OPT_PORT_COUNT=4
##  -DVTSS_CHIP_CU_PHY
##  -DVIPER_EVAL_BOARD
##  -DVTSS_FEATURE_PHY_TIMESTAMP
##  -DVTSS_FEATURE_MACSEC
##  -DVTSS_FEATURE_EEE
##  -DVTSS_SW_OPTION_EEE
##  -D_INCLUDE_DEBUG_FILE_PRINT_
##  -D_INCLUDE_DEBUG_TERM_PRINT_
##  -DVTSS_SW_OPTION_DEBUG
##  -DVTSS_PHY_OPT_CAP_FE_ONLY
##  -DVTSS_PHY_OPT_CAP_GE_ONLY

