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
#ifndef _VTSS_DAYTONA_REG_INIT_TFI_H
#define _VTSS_DAYTONA_REG_INIT_TFI_H

// Settings for mode SDR

#define  VTSS_TFI_5_GLOBAL_GLOBAL_CONTROL_RTFI_EN_SDR                                               VTSS_F_TFI_5_GLOBAL_GLOBAL_CONTROL_RTFI_EN(0x0)
#define  VTSS_TFI_5_GLOBAL_GLOBAL_CONTROL_LOOP_EN_SDR                                               VTSS_F_TFI_5_GLOBAL_GLOBAL_CONTROL_LOOP_EN(0x0)
#define  VTSS_TFI_5_RX_RX_CONTROL_SYNC_SEL_SDR                                                      VTSS_F_TFI_5_RX_RX_CONTROL_SYNC_SEL(0x0)


// Settings for mode DDR

#define  VTSS_TFI_5_GLOBAL_GLOBAL_CONTROL_RTFI_EN_DDR                                               VTSS_F_TFI_5_GLOBAL_GLOBAL_CONTROL_RTFI_EN(0x1)
#define  VTSS_TFI_5_GLOBAL_GLOBAL_CONTROL_LOOP_EN_DDR                                               VTSS_F_TFI_5_GLOBAL_GLOBAL_CONTROL_LOOP_EN(0x0)
#define  VTSS_TFI_5_RX_RX_CONTROL_SYNC_SEL_DDR                                                      VTSS_F_TFI_5_RX_RX_CONTROL_SYNC_SEL(0x0)


// Settings for mode UNUSED

#define  VTSS_TFI_5_GLOBAL_GLOBAL_CONTROL_RTFI_EN_UNUSED                                            VTSS_F_TFI_5_GLOBAL_GLOBAL_CONTROL_RTFI_EN(0x0)
#define  VTSS_TFI_5_GLOBAL_GLOBAL_CONTROL_LOOP_EN_UNUSED                                            VTSS_F_TFI_5_GLOBAL_GLOBAL_CONTROL_LOOP_EN(0x0)
#define  VTSS_TFI_5_RX_RX_CONTROL_SYNC_SEL_UNUSED                                                   VTSS_F_TFI_5_RX_RX_CONTROL_SYNC_SEL(0x0)




typedef enum {
    BM_TFI_SDR,
    BM_TFI_DDR,
    BM_TFI_UNUSED,
    BM_TFI_LAST
} block_tfi_mode_t;

#endif /* _VTSS_DAYTONA_REG_INIT_TFI_H */
