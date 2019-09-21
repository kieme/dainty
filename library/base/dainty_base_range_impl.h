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

#ifndef _DAINTY_BASE_RANGE_IMPL_H_
#define _DAINTY_BASE_RANGE_IMPL_H_

#include "dainty_base.h"
#include "dainty_base_assert.h"

namespace dainty
{
namespace base
{
namespace range
{
namespace impl_
{
///////////////////////////////////////////////////////////////////////////////

  using types::t_prefix;
  using types::t_bool;
  using types::t_void;
  using types::t_n_;
  using types::t_ix_;
  using types::t_byte_;
  using types::t_char;
  using types::P_void;

  using specific::P_cstr;
  using specific::t_n;
  using specific::T_n;
  using specific::t_ix;
  using specific::t_begin_ix;
  using specific::t_end_ix;
  using specific::t_validity;
  using specific::VALID;
  using specific::INVALID;
  using specific::operator""_begin_ix;
  using specific::operator""_end_ix;

  using assertion::assert_now;

///////////////////////////////////////////////////////////////////////////////

  constexpr
  t_void check_valid_(P_void item, t_n_ n) noexcept {
    if (!item)
      assert_now(P_cstr{"range: init error"});
  }

  constexpr
  t_void check_range_(t_n_ n, t_ix_ begin) noexcept {
    if (begin >= n)
      assert_now(P_cstr{"range: overflow 1"});
  }

  constexpr
  t_void check_range_(t_n_ n, t_ix_ begin, t_ix_ end) noexcept {
    if (end > n || begin >= end)
      assert_now(P_cstr{"range: overflow 2"});
  }

  constexpr
  t_void check_equal_(t_n_ n1, t_n_ n2) noexcept {
    if (n1 != n2)
      assert_now(P_cstr{"range: valid comparison"});
  }

///////////////////////////////////////////////////////////////////////////////

  template<typename T>
  t_n_ copy_(t_n _dst_max, T* dst_ptr, t_n _src_max, const T* src_ptr) {
    if (dst_ptr != src_ptr) { // can determine overlap - XXX
      auto dst_max = get(_dst_max), src_max = get(_src_max);
      auto max = dst_max < src_max ? dst_max : src_max;
      check_range_(dst_max, src_max);
      for (t_ix_ ix = 0; ix < max; ++ix)
        dst_ptr[ix] = src_ptr[ix];
    }
    return 0;
  }

  template<typename T>
  t_bool is_equal_(t_n _a1_max, T* a1_ptr, t_n _a2_max, const T* a2_ptr) {
    auto a1_max = get(_a1_max), a2_max = get(_a2_max);
    if (a1_max == a2_max) {
      if (a1_ptr != a2_ptr) {
        for (t_ix_ ix = 0; ix < a1_max; ++ix)
          if (a1_ptr[ix] != a2_ptr[ix])
            return false;
      }
      return true;
    }
    return false;
  }

// specialize copy for all the buildin types

///////////////////////////////////////////////////////////////////////////////
}
}
}
}

#endif
