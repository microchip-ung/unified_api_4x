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
