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

#ifndef _DAINTY_BASE_BUF_IMPL_H_
#define _DAINTY_BASE_BUF_IMPL_H_

#include "dainty_base_ptr.h"
#include "dainty_base_range.h"
#include "dainty_base_valuestore.h"

namespace dainty
{
namespace base
{
namespace buf
{
  using base::VALID;
  using base::INVALID;
  using base::t_void;
  using base::t_bool;
  using base::t_validity;
  using base::t_prefix;
  using base::t_emplace_it;
  using base::t_n;
  using base::t_n_;
  using base::t_ix;
  using base::t_ix_;
  using base::ptr::t_ptr;
  using base::range::mk_range;
  using base::range::mk_crange;
  using base::range::t_range;
  using base::range::t_crange;
  using base::valuestore::t_valuestore;

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
