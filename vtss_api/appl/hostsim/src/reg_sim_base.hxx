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
#ifndef __REG_SIM_BASE_HXX__
#define __REG_SIM_BASE_HXX__

#include <stdint.h>
#include <stdlib.h>

struct Chip
{
    virtual const char * name() const = 0;
};

struct Target
{
    virtual const char * name() const = 0;
    virtual uint32_t device_id() const = 0;
};

struct RegisterGroup
{
    virtual const char * name() const = 0;
    virtual uint32_t address() const = 0;
};

struct RegisterGroupSet
{
    virtual const char * name() const = 0;

    virtual uint32_t cnt() const = 0;
    virtual uint32_t address(uint32_t idx) const = 0;
};

struct RegisterDesc
{
    virtual const char * name() const = 0;
    virtual const uint32_t initial() const = 0;
    virtual const uint32_t read_mask() const = 0;
    virtual const uint32_t write_mask() const = 0;
    virtual const uint32_t max_width() const = 0;
    virtual uint32_t address() const = 0;
};

struct RegisterSetDesc
{
    virtual const char * name() const = 0;
    virtual const uint32_t initial() const = 0;
    virtual const uint32_t read_mask() const = 0;
    virtual const uint32_t write_mask() const = 0;
    virtual const uint32_t max_width() const = 0;
    virtual uint32_t cnt() const = 0;
    virtual uint32_t address(uint32_t idx) const = 0;
};

struct Register
{
    uint32_t data;
};

struct Walker
{
    virtual void exec(Chip *c, Target *t, RegisterGroup *g, RegisterDesc *r,
                      Register *reg) {};
    virtual void exec(Chip *c, Target *t, RegisterGroupSet *g,
                      uint32_t gidx, RegisterDesc *r, Register *reg) {};
    virtual void exec(Chip *c, Target *t, RegisterGroup *g,
                      RegisterSetDesc *r, uint32_t ridx, Register *reg) {};
    virtual void exec(Chip *c, Target *t, RegisterGroupSet *g, uint32_t gidx,
                      RegisterSetDesc *r, uint32_t ridx, Register *reg) {};
};


#endif //__REG_SIM_BASE_HXX__
