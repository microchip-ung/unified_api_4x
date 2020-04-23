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

/*

	This is an example of how adaptive equalization can be called.
	
	Modification of ae_run() (ae_run.c) is not required unless the otherwise
	indicated for	your application.
	
	Before calling ae_run(), you must:
	  Initialize all the links using ae_init().
	  Initialize all the transmitters (this is separate from ae_init() in case the
	  transmitters are serviced by a different processor).
	
 */

#include "ae.h"
#include "ae_cfg.h" 
#include "ae_reg_access.h"
int ae_main(void); /* forward declaration to avoid compiler warning */

int ae_main(void){
	AE_LINK link[LINKS];	
	int i;
	
	for(i = 0; i < LINKS; i++){
        ae_init(&link[i],i);
        ae_init_tx(&link[i]);
	}
    	
	ae_run(link, LINKS);

	return 0;
}
