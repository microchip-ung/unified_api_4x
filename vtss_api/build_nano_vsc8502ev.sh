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
gcc -g -o vtss_api_nano_vsc8502ev -I include -DVTSS_OPSYS_LINUX=1  -DVTSS_OPT_PORT_COUNT=2  -DVTSS_CHIP_CU_PHY -DNANO_EVAL_BOARD -DVTSS_FEATURE_EEE -DVTSS_SW_OPTION_EEE -D_INCLUDE_DEBUG_TERM_PRINT_ appl/vtss_appl_board_nano_eval.c appl/vtss_appl_cu_phy.c base/ail/vtss_api.c  base/ail/vtss_common.c  `find base/phy/phy_1g -name "*.c"`
## Available Defines:
##  -DVTSS_OPSYS_LINUX=1  
##  -DVTSS_OPT_PORT_COUNT=2  
##  -DVTSS_CHIP_CU_PHY 
##  -DNANO_EVAL_BOARD 
##  -DVTSS_FEATURE_EEE
##  -DVTSS_SW_OPTION_EEE
##  -DVTSS_SW_OPTION_DEBUG // Turns on Register Checking
##  -D_INCLUDE_DEBUG_FILE_PRINT_
##  -D_INCLUDE_DEBUG_TERM_PRINT_

