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

#ifndef _DAINTY_NAMED_STRING_STREAM_H_
#define _DAINTY_NAMED_STRING_STREAM_H_

#include "dainty_named_string_impl.h"

namespace dainty
{
namespace named
{
namespace string
{
  template<class TAG, t_n_ N, t_overflow O> class t_string;

////////////////////////////////////////////////////////////////////////////////

  constexpr t_block SP  {' ', t_n{1}};
  constexpr t_block SP_2{' ', t_n{2}};
  constexpr t_block SP_3{' ', t_n{3}};
  constexpr t_block SP_4{' ', t_n{4}};
  constexpr t_block SP_5{' ', t_n{5}};

  template<t_n_ N>
  constexpr t_block spaces() noexcept { return t_block{' ', t_n{N}}; }

////////////////////////////////////////////////////////////////////////////////

  constexpr t_block NL  {'\n', t_n{1}};
  constexpr t_block NL_2{'\n', t_n{2}};
  constexpr t_block NL_3{'\n', t_n{3}};
  constexpr t_block NL_4{'\n', t_n{4}};
  constexpr t_block NL_5{'\n', t_n{5}};

  template<t_n_ N>
  constexpr t_block newlines() noexcept { return t_block{'\n', t_n{N}}; }

////////////////////////////////////////////////////////////////////////////////

  template<class TAG, t_n_ N, t_overflow O>
  inline
  t_string<TAG, N, O>& operator<<(t_string<TAG, N, O>& lh, P_cstr rh) noexcept {
    return lh.append(rh);
  }

  template<class TAG, class TAG1, t_n_ N, t_n_ N1, t_overflow O, t_overflow O1>
  inline
  t_string<TAG, N, O>& operator<<(t_string<TAG, N, O>& lh,
                                  const t_string<TAG1, N1, O1>& rh) noexcept {
    return lh.append(rh);
  }

  template<class TAG, t_n_ N, t_overflow O, class T>
  inline
  t_string<TAG, N, O>& operator<<(t_string<TAG, N, O>& lh,
                                  const T& rh) noexcept {
    return lh.append(rh);
  }

////////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
