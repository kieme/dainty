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

#ifndef _DAINTY_NAMED_STRING_H_
#define _DAINTY_NAMED_STRING_H_

#include "dainty_named_string_1.h"
#include "dainty_named_string_2.h"
#include "dainty_named_string_3.h"
#include "dainty_named_string_4.h"

namespace dainty
{
namespace named
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

  template<class TAG, t_n_ N, t_n_ N1, t_overflow O, t_overflow O1>
  inline
  t_bool operator<(const t_string<TAG, N, O>&   lh,
                   const t_string<TAG, N1, O1>& rh) noexcept {
    return compare_(get(lh.get_cstr()), get(rh.get_cstr())) < 0;
  }

  template<class TAG, t_n_ N, t_n_ N1, t_overflow O, t_overflow O1>
  inline
  t_bool operator>(const t_string<TAG, N,  O>&  lh,
                   const t_string<TAG, N1, O1>& rh) noexcept {
    return compare_(get(lh.get_cstr()), get(rh.get_cstr())) > 0;
  }

////////////////////////////////////////////////////////////////////////////////

  template<class TAG, t_n_ N, t_overflow O>
  inline
  t_bool operator==(const t_string<TAG, N, O>& lh, P_cstr rh) noexcept {
    return compare_(get(lh.get_cstr()), get(rh)) == 0;
  }

  template<class TAG, t_n_ N, t_overflow O>
  inline
  t_bool operator!=(const t_string<TAG, N, O>& lh, P_cstr rh) noexcept {
    return !(lh == rh);
  }

////////////////////////////////////////////////////////////////////////////////

  template<class TAG, t_n_ N, t_overflow O>
  inline
  t_bool operator==(P_cstr lh, const t_string<TAG, N, O>& rh) noexcept {
    return rh == lh;
  }

  template<class TAG, t_n_ N, t_overflow O>
  inline
  t_bool operator!=(P_cstr lh, const t_string<TAG, N, O>& rh) noexcept {
    return !(lh == rh);
  }

////////////////////////////////////////////////////////////////////////////////

  template<class TAG, t_n_ N, t_n_ N1, t_overflow O, t_overflow O1>
  inline
  t_bool operator==(const t_string<TAG, N,  O>&  lh,
                    const t_string<TAG, N1, O1>& rh) noexcept {
    return get(lh.get_length()) == get(rh.get_length()) && lh == rh.get_cstr();
  }

  template<class TAG, t_n_ N, t_n_ N1, t_overflow O, t_overflow O1>
  inline
  t_bool operator!=(const t_string<TAG, N,  O>&  lh,
                    const t_string<TAG, N1, O1>& rh) noexcept {
    return !(lh == rh);
  }

////////////////////////////////////////////////////////////////////////////////

  template<class TAG, t_n_ N, t_overflow O>
  inline
  t_string<TAG, N, O>& operator+=(t_string<TAG, N, O>& lh,
                                  P_cstr rh) noexcept {
    return lh.append(rh);
  }

  template<class TAG, class TAG1, t_n_ N, t_n_ N1, t_overflow O, t_overflow O1>
  inline
  t_string<TAG, N, O>& operator+=(t_string<TAG, N, O>& lh,
                                  const t_string<TAG1, N1, O1>& rh) noexcept {
    return lh.append(rh);
  }

  template<class TAG, t_n_ N, t_overflow O, class T>
  inline
  t_string<TAG, N, O>& operator+=(t_string<TAG, N, O>& lh,
                                  const T& rh) noexcept {
    return lh.append(rh);
  }

////////////////////////////////////////////////////////////////////////////////

  template<class TAG, t_n_ N, t_overflow O>
  inline
  t_string<TAG, N, O>& operator,(t_string<TAG, N, O>& lh, P_cstr rh) noexcept {
    return (lh +=rh);
  }

  template<class TAG, class TAG1, t_n_ N, t_n_ N1, t_overflow O, t_overflow O1>
  inline
  t_string<TAG, N, O>& operator,(t_string<TAG, N, O>& lh,
                                 const t_string<TAG1, N1, O1>& rh) noexcept {
    return (lh +=rh);
  }

  template<class TAG, t_n_ N, t_overflow O, class T>
  inline
  t_string<TAG, N, O>& operator,(t_string<TAG, N, O>& lh,
                                 const T& rh) noexcept {
    return (lh += rh);
  }

////////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
