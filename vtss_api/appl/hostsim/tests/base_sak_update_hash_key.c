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

#include "vtss_api.h"
#include "vtss_macsec_api.h"
#include "vtss_macsec_test_base.h"
#include <openssl/aes.h>

vtss_rc sak_update_hash_key(vtss_macsec_sak_t * sak)
{
    AES_KEY aes_key;
    char null_data[] = {0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0};

    if (sak->len == 16) {
        AES_set_encrypt_key(sak->buf, 128, &aes_key);

    } else if (sak->len == 32) {
        AES_set_encrypt_key(sak->buf, 256, &aes_key);

    } else {
        return VTSS_RC_ERROR;

    }

    AES_ecb_encrypt((const unsigned char *)null_data,
                    (unsigned char *)sak->h_buf,
                    &aes_key,
                    AES_ENCRYPT);

    return VTSS_RC_OK;
}

