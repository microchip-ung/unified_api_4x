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

#include "vtss_util.h"
#include "vtss_state.h"

#define  BPC	8               /* Bits per (unsigned) char */

/* The functions below operate on an array of u32 words of data.
   These data are stored according to the CPU endianness.
   The endianness affects the mapping from a given bit to corresponding byte offset. */
#ifdef VTSS_OS_BIG_ENDIAN
#define BYTE_OFFSET(offset) ((offset/BPC) + 3 - 2*((offset/BPC)%4))
#else
#define BYTE_OFFSET(offset) (offset/BPC)
#endif /* VTSS_OS_BIG_ENDIAN */

/*
 * Get bit
 */
u8 
vtss_bs_bit_get(const void *cptr,
                u32 offset)
{
    int boff = BYTE_OFFSET(offset);
    u8 mask = VTSS_BIT(offset % BPC);
    return ((const u8 *)cptr)[boff] & mask;
}

/*
 * Set bit
 */
void 
vtss_bs_bit_set(void *vptr,
                u32 offset,
                u8  value)
{
    int boff = BYTE_OFFSET(offset);
    u8 *cptr = vptr;
    u8  mask = VTSS_BIT(offset % BPC);
    if(value)
        cptr[boff] |= mask;
    else
        cptr[boff] &= ~mask;
}

/*
 * Set byte (or part of) at aligned byte offset
 */
static void 
bs_byte_set(u8  *cptr,
            u32 offset,
            u32 len,
            u8  value)
{
    int boff = BYTE_OFFSET(offset);
    u8 mask = VTSS_BITMASK(len);
    cptr[boff] = (cptr[boff] & ~mask) | (value & mask);
}

void 
vtss_bs_set(void *vptr,
            u32 offset,
            u32 len,
            u32 value)
{
    u8 *cptr = vptr;
    while(len > 0) {
        /* Get Easy case - byte aligned */
        if(len > 1 && (offset % BPC) == 0) {
            unsigned int niblen = MIN(BPC, len);
            bs_byte_set(cptr, offset, niblen, (u8)value);
            len -= niblen;
            offset += niblen;
            value >>= niblen;
        } else {
            /* Work bit by bit */
            vtss_bs_bit_set(cptr, offset, value & 1);
            offset++;
            len--;
            value >>= 1;
        }
    }
}

u32
vtss_bs_get(const void *vptr,
            u32 offset,
            u32 len)
{
    const u8 *cptr = vptr;
    u32  value = 0;
    offset += len;              /* We work backwards when extracting */
    while(len > 0) {
        /* Work bit by bit */
        value <<= 1;
        if(vtss_bs_bit_get(cptr, --offset))
            value++;
        len--;
    }
    return value;
}

