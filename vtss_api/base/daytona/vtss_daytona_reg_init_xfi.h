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
#ifndef _VTSS_DAYTONA_REG_INIT_XFI_H
#define _VTSS_DAYTONA_REG_INIT_XFI_H

// Settings for mode WORK

#define  VTSS_XFI_XFI_CONTROL_XFI_MODE_TX_AUTORESYNC_WORK                                           VTSS_F_XFI_XFI_CONTROL_XFI_MODE_TX_AUTORESYNC(0x0)


// Settings for mode PROT

#define  VTSS_XFI_XFI_CONTROL_XFI_MODE_TX_AUTORESYNC_PROT                                           VTSS_F_XFI_XFI_CONTROL_XFI_MODE_TX_AUTORESYNC(0x0)


// Settings for mode W_P

#define  VTSS_XFI_XFI_CONTROL_XFI_MODE_TX_AUTORESYNC_W_P                                            VTSS_F_XFI_XFI_CONTROL_XFI_MODE_TX_AUTORESYNC(0x0)


// Settings for mode UNUSED

#define  VTSS_XFI_XFI_CONTROL_XFI_MODE_TX_AUTORESYNC_UNUSED                                         VTSS_F_XFI_XFI_CONTROL_XFI_MODE_TX_AUTORESYNC(0x0)




typedef enum {
    BM_XFI_WORK,
    BM_XFI_PROT,
    BM_XFI_W_P,
    BM_XFI_UNUSED,
    BM_XFI_LAST
} block_xfi_mode_t;

#endif /* _VTSS_DAYTONA_REG_INIT_XFI_H */
