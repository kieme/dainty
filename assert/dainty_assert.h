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

#ifndef _DAINTY_ASSERT_H_
#define _DAINTY_ASSERT_H_

#define DAINTY_INTERNAL_INCLUDE_C_STDARG
#include "dainty_internal_.h"
#include "dainty_types.h"
#include "dainty_specific.h"

namespace dainty
{
namespace assertion
{
  /////////////////////////////////////////////////////////////////////////////

  using types::t_bool;
  using types::t_void;
  using types::P_char;

  /////////////////////////////////////////////////////////////////////////////

  t_void assert_now(P_char fmt, ...) noexcept
    __attribute__((format(printf, 1, 2)));

  t_void assert_now(P_char fmt, va_list) noexcept;

  /////////////////////////////////////////////////////////////////////////////

  t_void assert_when_true (t_bool, P_char fmt, ...) noexcept;
  t_void assert_when_false(t_bool, P_char fmt, ...) noexcept;

  /////////////////////////////////////////////////////////////////////////////
}
}

#include "impl_/dainty_assert_.icpp"

#endif
