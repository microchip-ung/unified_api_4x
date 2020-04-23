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
/**
 * \file
 * \brief OS Layer API
 * \details This header file includes the OS specific header file
 */

#ifndef _VTSS_OS_H_
#define _VTSS_OS_H_

#if defined(VTSS_OPSYS_ECOS)
 #include <vtss_os_ecos.h>
#elif defined(VTSS_OPSYS_LINUX)
 #include <vtss_os_linux.h>
#elif defined(VTSS_OS_CUSTOM)
 #include <vtss_os_custom.h>
#else
 #error "Operating system not supported".
#endif

/*
 * Don't add default VTSS_xxx() macro implementations here,
 * since that might lead to uncaught problems on new platforms.
 */

#endif /* _VTSS_OS_H_ */
