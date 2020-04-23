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
#include <iostream>
#include <iomanip>
#include "venice.hxx"

struct Foo : public Walker
{
    void exec(Chip *c, Target *t, RegisterGroup *g, RegisterDesc *r)
    {
        std::cout << t->channel() << "," << t->device() << "@" <<
                     "0x" << std::hex << std::setfill ('0') << std::setw (4) <<
                     (r->address() + g->address()) << " " <<
                     c->name() << "::" <<
                     t->name() << "::" <<
                     g->name() << "::" <<
                     r->name() << std::endl;
    }

    void exec(Chip *c, Target *t, RegisterGroupSet *g, uint32_t gidx, RegisterDesc *r)
    {
        std::cout << t->channel() << "," << t->device() << "@" <<
                     "0x" << std::hex << std::setfill ('0') << std::setw (4) <<
                     (r->address() + g->address(gidx)) << " " <<
                     c->name() << "::" <<
                     t->name() << "::" <<
                     g->name() << "[" << gidx << "]" << "::" <<
                     r->name() << std::endl;
    }

    void exec(Chip *c, Target *t, RegisterGroup *g, RegisterSetDesc *r, uint32_t ridx)
    {
        std::cout << t->channel() << "," << t->device() << "@" <<
                     "0x" << std::hex << std::setfill ('0') << std::setw (4) <<
                     (r->address(ridx) + g->address()) << " " <<
                     c->name() << "::" <<
                     t->name() << "::" <<
                     g->name() << "::" <<
                     r->name() << "[" << ridx << "]" << std::endl;
    }

    void exec(Chip *c, Target *t, RegisterGroupSet *g, uint32_t gidx, RegisterSetDesc *r, uint32_t ridx)
    {
        std::cout << t->channel() << "," << t->device() << "@" <<
                     "0x" << std::hex << std::setfill ('0') << std::setw (4) <<
                     (r->address(ridx) + g->address(gidx)) << " " <<
                     c->name() << "::" <<
                     t->name() << "::" <<
                     g->name() << "[" << gidx << "]" << "::" <<
                     r->name() << "[" << ridx << "]" <<
                     std::endl;
    }
};

int main()
{
    std::cout << sizeof(Venice) << " ";
    std::cout << sizeof(Venice) / 1024.0 << "kb" << " ";
    std::cout << sizeof(Venice) / 1024.0 / 1024 << "mb" << std::endl;
    Foo foo;
    Venice venice;
    venice.walk(&foo);
    return 0;
}

