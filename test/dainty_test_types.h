/******************************************************************************

 MIT License

 Copyright (c) 2018 kieme, frits.germs@gmx.net

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

******************************************************************************/

#ifndef _DAINTY_TEST_IMPL_TYPES_H_
#define _DAINTY_TEST_IMPL_TYPES_H_

#include "dainty_types.h"

namespace dainty
{
namespace test
{
  using types::t_void;

  /////////////////////////////////////////////////////////////////////////////

  using P_addr  = types::P_void;

  enum t_class_method {
    METHOD_DEFAULT_CONSTRUCT,
    METHOD_COPY_CONSTRUCT,
    METHOD_MOVE_CONSTRUCT,
    METHOD_COPY_ASSIGNMENT,
    METHOD_MOVE_ASSIGNMENT,
    METHOD_DESTRUCT,
    METHOD_SET,
    METHOD_GET
  };

  struct t_class_sequence {
    struct t_entry {
      P_addr         self  = nullptr;
      P_addr         other = nullptr;
      t_class_method method;
    };
    int     ix = 0;
    t_entry history[100];
  };

  struct t_class_counters {
    int default_construct = 0;
    int move_construct    = 0;
    int copy_construct    = 0;
    int destruct          = 0;
    int copy_assign       = 0;
    int move_assign       = 0;
    int set               = 0;
    int Get               = 0;
  };

  struct t_class_info {
    int size_of  = 0;
    int align_of = 0;

    t_class_counters counters;
    t_class_sequence sequence;

    t_void reset  ();
    t_void display() const;
  };

  /////////////////////////////////////////////////////////////////////////////
}
}

#endif
