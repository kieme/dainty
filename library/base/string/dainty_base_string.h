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

#ifndef _DAINTY_BASE_STRING_H_
#define _DAINTY_BASE_STRING_H_

#include "dainty_base_string_1.h"
#include "dainty_base_string_2.h"
#include "dainty_base_string_3.h"
#include "dainty_base_string_4.h"
#include "dainty_base_string_stream_in.h"
#include "dainty_base_string_stream_out.h"

namespace dainty
{
namespace base
{
namespace string
{
///////////////////////////////////////////////////////////////////////////////
//
// support 6 strings. with slightly different behaviour but same interfaces.
//
// difference of support:
//   when N > 0, then t_string default constructor is available.
//   when N==0,  then t_string constructor will determine size from its input.
//   when OVERFLOW_GROW, then it support move from another GROW.
//
// 1. t_string<TAG, 100, OVERFLOW_ASSERT>
//     * static buf of 100 bytes, assert when overflow.
//
// 2. t_string<TAG, 100, OVERFLOW_TRUNCATE>
//     * static buf of 100 bytes, truncate when overflow.
//
// 3. t_string<TAG, 0,   OVERFLOW_ASSERT>
//     * heap buf, assert when overflow.
//
// 4. t_string<TAG, 0,   OVERFLOW_TRUNCATE>
//     * heap buf, truncate when overflow.
//
// 5. t_string<TAG, 10,  OVERFLOW_GROW>
//     * use sso of 10, but can be heap buf, will grow as needed.
//
// 6. t_string<TAG, 0,   OVERFLOW_GROW>
//     * no sso, heap buf, will grow as needed.
//
///////////////////////////////////////////////////////////////////////////////

  template<t_n_ N>
  constexpr
  t_crange string_literal(const t_char (&value)[N]) noexcept {
    return mk_range(value);
  }

  constexpr
  t_crange operator""_SL (P_cstr_ cstr, t_n_ len) {
    return {cstr, t_n{len}};
  }

///////////////////////////////////////////////////////////////////////////////

  t_void display(t_cstr_cptr_, ...) noexcept
      __attribute__((format(printf, 1, 2)));

  t_void display(t_crange, va_list) noexcept;

  inline
  t_void display(t_crange range, t_crange prefix  = NO_RANGE,
                                 t_crange postfix = NO_RANGE) noexcept {
    impl_::display_(range, prefix, postfix);
  }

////////////////////////////////////////////////////////////////////////////////

  inline
  t_bool operator==(t_crange lh, t_crange rh) noexcept {
    return impl_::equal_(lh, rh);
  }

  inline
  t_bool operator!=(t_crange lh, t_crange rh) noexcept {
    return !impl_::equal_(lh, rh);
  }

  inline
  t_bool operator<(t_crange lh, t_crange rh) noexcept {
    return impl_::less_(lh, rh);
  }

  inline
  t_bool operator<=(t_crange lh, t_crange rh) noexcept {
    return impl_::less_equal_(lh, rh);
  }

  inline
  t_bool operator>(t_crange lh, t_crange rh) noexcept {
    return impl_::less_(rh, lh);
  }

  inline
  t_bool operator>=(t_crange lh, t_crange rh) noexcept {
    return impl_::less_equal_(rh, lh);
  }

///////////////////////////////////////////////////////////////////////////////

  template<class TAG, t_n_ N, typename O, t_n_ N1, typename O1>
  inline
  t_bool operator==(const t_string<TAG, N,  O>&  lh,
                    const t_string<TAG, N1, O1>& rh) noexcept {
    return impl_::equal_(lh.mk_range(), rh.mk_range());
  }

  template<class TAG, t_n_ N, typename O, t_n_ N1, typename O1>
  inline
  t_bool operator!=(const t_string<TAG, N,  O>&  lh,
                    const t_string<TAG, N1, O1>& rh) noexcept {
    return !impl_::equal_(lh.mk_range(), rh.mk_range());
  }

  template<class TAG, t_n_ N, typename O, t_n_ N1, typename O1>
  inline
  t_bool operator<(const t_string<TAG, N,  O>&  lh,
                   const t_string<TAG, N1, O1>& rh) noexcept {
    return impl_::less_(lh.mk_range(), rh.mk_range());
  }

  template<class TAG, t_n_ N, typename O, t_n_ N1, typename O1>
  inline
  t_bool operator<=(const t_string<TAG, N,  O>&  lh,
                    const t_string<TAG, N1, O1>& rh) noexcept {
    return impl_::less_equal_(lh.mk_range(), rh.mk_range());
  }

  template<class TAG, t_n_ N, typename O, t_n_ N1, typename O1>
  inline
  t_bool operator>(const t_string<TAG, N,  O>&  lh,
                   const t_string<TAG, N1, O1>& rh) noexcept {
    return impl_::less_(rh.mk_range(), lh.mk_range());
  }

  template<class TAG, t_n_ N, typename O, t_n_ N1, typename O1>
  inline
  t_bool operator>=(const t_string<TAG, N,  O>&  lh,
                    const t_string<TAG, N1, O1>& rh) noexcept {
    return impl_::less_equal_(rh.mk_range(), lh.mk_range());
  }

///////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
