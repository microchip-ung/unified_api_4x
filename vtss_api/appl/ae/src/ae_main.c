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
