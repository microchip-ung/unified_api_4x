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

#Compiles 1588-API, macsec API,10g,1G
gcc -Werror -Wall -Wextra -Wdeclaration-after-statement -Wunused-but-set-variable -I include -std=gnu89\
                                             appl/vtss_appl_board_viper_eval.c \
                                             appl/vtss_appl.c appl/vtss_appl_cli.c \
                                             base/ail/vtss_wis_api.c \
                                             base/ail/vtss_api.c \
                                             base/ail/vtss_port_api.c \
                                             base/ail/vtss_common.c  \
                                             base/phy/common/vtss_phy_common.c \
                                             appl/vtss_version.c \
                                             `find base/phy/phy_1g -name "*.c"` \
                                             `find base/phy/phy_10g -name "*.c"` \
                                             `find base/phy/ts -name "*.c"` \
                                             `find base/phy/macsec -name "*.c"` \
                                             -DVTSS_OPT_PORT_COUNT=4  \
                                             -DVTSS_OPSYS_LINUX=1\
                                             -DVTSS_CHIP_10G_PHY \
                                             -DVTSS_FEATURE_10G  \
                                             -DVTSS_FEATURE_10GBASE_KR \
                                             -DVTSS_FEATURE_PHY_TIMESTAMP \
                                             -DVTSS_USE_STDINT_H \
                                             -DVTSS_FEATURE_MACSEC \
                                             -DVIPER_EVAL_BOARD \
                                             -o vtss_api_10g
echo "############################################################################################################"
echo "Compiler version used"
gcc -v
echo "############################################################################################################"
