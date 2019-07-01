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

#ifndef _DAINTY_NAMED_BUF_IMPL_H_
#define _DAINTY_NAMED_BUF_IMPL_H_

#include "dainty_named_ptr.h"
#include "dainty_named_range.h"
#include "dainty_named_valuestore.h"

namespace dainty
{
namespace named
{
namespace buf
{
  using named::VALID;
  using named::INVALID;
  using named::t_void;
  using named::t_bool;
  using named::t_validity;
  using named::t_prefix;
  using named::t_emplace_it;
  using named::t_n;
  using named::t_n_;
  using named::t_ix;
  using named::t_ix_;
  using named::ptr::t_ptr;
  using named::range::mk_range;
  using named::range::mk_crange;
  using named::range::t_range;
  using named::range::t_crange;
  using named::valuestore::t_valuestore;

////////////////////////////////////////////////////////////////////////////////

  enum t_size_dynamic { };
  enum t_size_static  { };

  template<typename, t_n_, typename = t_size_dynamic>
  class t_buf;

////////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
