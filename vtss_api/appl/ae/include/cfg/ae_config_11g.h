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


#include "ae.h"

/* The number of links being trained. This is only used in ae_main.c and ae_run.c.
	 You may prefer to use different mechanisms for training multiple links.
 */
#define LINKS             7

/* The peaking amp gain, 0 to 7. If AE_PEAK_AMP < 0, the peaking amp gain is
   left as is (unchanged).*/
#define AE_PEAK_AMP       7

/* The DFE mode, as given in the core databook. 0=DFE5, if AE_DFE_MODE < 0,
   the dfe mode is left as is.*/
#define AE_DFE_MODE       0

/* Set to 1 if the system uses the AEC macro (Adaptive Equalization Core for
	 Ethernet...)
   Will not work automatically with AE_USE_AEC = 0 and a local TX (loopback),
	 (ae_set_req() and ae_get_status() must use registers 0xE and 0xF)! 
*/
#define AE_USE_AEC        1

/* Modifications past this point will affect the adaptive equalization
	 algorithm. */
#define AE_INIT_METH      3
#define AE_CE_DECS        3
#define AE_EN_ZFE         1
#define AE_EN_H1T_EQ      1
#define AE_EN_AI_C1       1
#define AE_EN_MAX_ST      1
#define AE_EN_AMIN_TH     1
#define AE_EN_SD_TH       0
#define AE_EN_GAIN_TOG    1

#define AE_ACC_LIM        (1<< 8)
#define AE_CNV_LIM        (1<<12)
#define AE_TOG_LIM        4

#define AE_GAIN_TH        15
#define AE_AMIN_TH        20
#define AE_H1TEQ_GOAL     4

#define AE_EST_PRE        3
#define AE_EST_POST123    0
#define AE_EST_POST45     5
#define AE_EST_POST6      9
#define AE_EST_POST7      12

#define AE_SAMP_REQ_LIM   128

