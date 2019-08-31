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
////////////////////////////////////////////////////////////////////////////////

  using types::t_prefix;
  using types::t_bool;
  using types::t_n_;
  using types::t_n_;
  using types::p_void;
  using types::t_void;

  using specific::t_n;
  using specific::t_ix;
  using specific::t_validity;
  using specific::operator""_ix;
  using specific::operator""_n;

  using specific::VALID;
  using specific::INVALID;

  using ptr::t_ptr;
  using range::mk_range;
  using range::mk_crange;
  using range::t_range;
  using range::t_crange;
  using valuestore::t_valuestore;

////////////////////////////////////////////////////////////////////////////////

  enum t_size_dynamic { };
  enum t_size_static  { };

  enum t_buf_range_tag_ { };
  template<typename T> using t_buf_range  = t_range <T, t_buf_range_tag_>;
  template<typename T> using t_buf_crange = t_crange<T, t_buf_range_tag_>;

////////////////////////////////////////////////////////////////////////////////

  enum  t_ptr_tag_ { };
  template<typename T>
  using t_ptr_ = t_ptr<t_valuestore<T>[], t_ptr_tag_, ptr::t_no_deleter>;

////////////////////////////////////////////////////////////////////////////////

  template<typename, t_n_, typename = t_size_dynamic>
  class t_buf;

////////////////////////////////////////////////////////////////////////////////

  p_void mem_alloc_  (t_n)         noexcept;
  p_void mem_realloc_(p_void, t_n) noexcept;
  t_void mem_dealloc_(p_void)      noexcept;

////////////////////////////////////////////////////////////////////////////////

  template<typename T>
  inline T* alloc_(t_n n = 1_n) noexcept {
    return reinterpret_cast<T*>(mem_alloc_(t_n(sizeof(T) * get(n))));
  }

  template<typename T>
  inline T* realloc_(p_void obj, t_n n = 1_n) noexcept {
    return reinterpret_cast<T*>(mem_realloc_(obj, t_n(sizeof(T) * get(n))));
  }

  inline t_void dealloc_(p_void obj) noexcept {
    return mem_dealloc_(obj);
  }

////////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
