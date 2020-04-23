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
