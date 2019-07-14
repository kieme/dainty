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
#include "dainty_base_traits.h"
#include "dainty_base_valuestore.h"

namespace dainty
{
namespace base
{
namespace buf
{
  using base::ptr::t_ptr;
  using base::range::mk_range;
  using base::range::mk_crange;
  using base::range::t_range;
  using base::range::t_crange;
  using base::valuestore::t_valuestore;

////////////////////////////////////////////////////////////////////////////////

  enum t_size_dynamic { };
  enum t_size_static  { };

  enum t_buf_range_tag_ { };
  template<typename T> using t_buf_range  = t_range<T, t_buf_range_tag_>;
  template<typename T> using t_buf_crange = t_crange<T, t_buf_range_tag_>;

  template<typename, t_n_, typename = t_size_dynamic>
  class t_buf;

////////////////////////////////////////////////////////////////////////////////

  template<typename T>
  t_void transfer(t_buf_range<T> dst, t_buf_crange<T> src) noexcept {
  }

  template<typename T>
  t_buf_range<T> enlarge_by_(t_n, t_buf_range<T>) noexcept {
  }

  //template<typename T, typename = t_if<t_is_arithmetic<T>>
  //t_buf_range<T> enlarge_by_(t_n, t_buf_range<T>) noexcept {
  //}

////////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
