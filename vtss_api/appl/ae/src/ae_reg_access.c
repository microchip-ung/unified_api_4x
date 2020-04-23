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
#include <stdlib.h>
#include <stdio.h>

#include "ae.h"
#include "ae_cfg.h"
#include "ae_reg_access.h"
#include "vtss_ae_api.h"



/* TODO - API for this? */
#define VTSS_API_INST 0


/* 
	 This function must get the status read from the FAR END tx. If the AEC is used,
	 this should read AEC reg 1.
*/
int  ae_get_status(AE_LINK* link){    
	u32 status=0;
    if(vtss_ae_get_coeff_update_stat(VTSS_API_INST,link->id,&status) == VTSS_RC_OK)
        return status;    
    else{
        printf("Error vtss API\n");
        return 0;
    }
}

/*
	This function must set the request sent to the FAR END tx. If the AEC is used,
	this should write AEC reg 0.
*/	
void ae_set_req(AE_LINK* link, u16 request){    
    if(vtss_ae_set_coeff_update_req(VTSS_API_INST,link->id,request) != VTSS_RC_OK){
        printf("Error vtss API\n");
    }    
}

long long  time_usec(void){
	/* sleep and return current time */	
    return 0;
}



char ae_get_amin(AE_LINK* link){
    u32 retval=0;
    if(vtss_ae_get_eye_data(VTSS_API_INST,link->id, AMIN,&retval) == VTSS_RC_OK){
        return retval;
    }else{
        printf("Error vtss API\n");
        return 0;
    }
}

char ae_get_gain(AE_LINK* link){
    u32 retval=0;
    if(vtss_ae_get_eye_data(VTSS_API_INST,link->id, GAIN,&retval) == VTSS_RC_OK){
        return retval;
    }else{
        printf("Error vtss API\n");
        return 0;
    }
}

short ae_get_datas_edges(AE_LINK* link){
    u32 retval=0;
    if(vtss_ae_get_eye_data(VTSS_API_INST,link->id, DATA_EDGE,&retval) == VTSS_RC_OK){
        return retval;
    }else{
        printf("Error vtss API\n");
        return 0;
    }
}

char ae_get_h1t(AE_LINK* link){
    u32 retval=0;
    if(vtss_ae_get_eye_data(VTSS_API_INST,link->id, H1T,&retval) == VTSS_RC_OK){
        return retval;
    }else{
        printf("Error vtss API\n");
        return 0;
    }
}

char ae_req_dfe_sample(AE_LINK* link){
    u32 retval=0;
    if(vtss_ae_get_eye_data(VTSS_API_INST,link->id, DFE_SAMPLE,&retval) == VTSS_RC_OK){
        return retval;
    }else{
        printf("Error vtss API\n");
        return 0;
    }
   
}

void ae_init_rx(AE_LINK* link){

    if(vtss_ae_init_rx(VTSS_API_INST,link->id) != VTSS_RC_OK){        
        printf("Error vtss API\n");        
    }
    
}

void ae_return_rx(AE_LINK* link){
    if(vtss_ae_return_rx_cfg(VTSS_API_INST,link->id) != VTSS_RC_OK){        
        printf("Error vtss API\n");        
    }
}

void ae_init_tx(AE_LINK* link){
     if(vtss_ae_init_rx(VTSS_API_INST,link->id) != VTSS_RC_OK){        
        printf("Error vtss API\n");        
    }

}

