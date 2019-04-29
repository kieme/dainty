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

#ifndef _DAINTY_NAMED_UTILITY_H_
#define _DAINTY_NAMED_UTILITY_H_

#include "dainty_named.h"

namespace dainty
{
namespace named
{
namespace utility
{
///////////////////////////////////////////////////////////////////////////////

  struct t_true  { constexpr static t_bool VALUE = true;  };
  struct t_false { constexpr static t_bool VALUE = false; };

  template<bool> struct t_bool_result       { using t_result = t_false; };
  template<>     struct t_bool_result<true> { using t_result = t_true;  };

///////////////////////////////////////////////////////////////////////////////

  template<typename RESULT, typename THEN> struct t_if_then;

  template<typename THEN>
  struct t_if_then<t_true, THEN>              { using t_result = THEN; };

  template<typename THEN>
  struct t_if_then<t_bool_result<true>, THEN> { using t_result = THEN; };

///////////////////////////////////////////////////////////////////////////////

  template<typename T> struct t_is_unsigned : t_bool_result<false> { };

  template<> struct t_is_unsigned<t_uchar>    : t_bool_result<true> { };
  template<> struct t_is_unsigned<t_ushort>   : t_bool_result<true> { };
  template<> struct t_is_unsigned<t_uint>     : t_bool_result<true> { };
  template<> struct t_is_unsigned<t_ulong>    : t_bool_result<true> { };
  template<> struct t_is_unsigned<t_ullong>   : t_bool_result<true> { };

  template<typename T, typename THEN = void>
  struct t_if_unsigned {
    using t_result = typename t_if_then<t_is_unsigned<T>, THEN>::t_result;
  };

///////////////////////////////////////////////////////////////////////////////

  template<typename T>
  struct t_remove_ref {
    using t_result = T;
  };

  template<typename T>
  struct t_remove_ref<T&> {
    using t_result = T;
  };

  template<typename T>
  struct t_remove_ref<T&&> {
    using t_result = T;
  };

///////////////////////////////////////////////////////////////////////////////

  constexpr t_bool reset(t_bool& t, t_bool value) {
    t_bool tmp = t;
    t = value;
    return tmp;
  }

  template<class T>
  constexpr T reset(T& t, T value) {
    T tmp = t;
    t = value;
    return tmp;
  }

  template<class T>
  constexpr T* reset(T*& t, T* value) {
    T* tmp = t;
    t = value;
    return tmp;
  }

  template<class T, class TAG>
  constexpr t_explicit<T, TAG>  reset(t_explicit<T, TAG>& t, T value) {
    t_explicit<T, TAG> tmp{t};
    t = t_explicit<T, TAG>{value};
    return tmp;
  }

  template<class T, class TAG>
  constexpr t_explicit<T, TAG> reset(t_explicit<T, TAG>& t,
                                     t_explicit<T, TAG> value) {
    t_explicit<T, TAG> tmp{t};
    t = value;
    return tmp;
  }

  template<class TAG>
  constexpr t_user<TAG> reset(t_user<TAG>& t, t_int64 value) {
    return t_user<TAG>{reset(t.id, value)};
  }

///////////////////////////////////////////////////////////////////////////////

  constexpr t_bool reset(t_bool& t) {
    t_bool tmp = t;
    t = false;
    return tmp;
  }

  template<class T>
  constexpr T reset(T& t) {
    T tmp = t;
    t = 0;
    return tmp;
  }

  template<class T>
  constexpr T* reset(T*& t) {
    T* tmp = t;
    t = nullptr;
    return tmp;
  }

  constexpr t_fd reset(t_fd& fd) {
    return reset(fd, BAD_FD);
  }

  template<class T, class TAG>
  constexpr t_explicit<T, TAG> reset(t_explicit<T, TAG>& t) {
    t_explicit<T, TAG> tmp{t};
    t = t_explicit<T, TAG>{0};
    return tmp;
  }

  template<class T, class TAG>
  constexpr t_explicit<T*, TAG> reset(t_explicit<T*, TAG>& t) {
    t_explicit<T*, TAG> tmp{t};
    t = t_explicit<T*, TAG>{nullptr};
    return tmp;
  }

  template<class TAG>
  constexpr t_explicit<t_bool, TAG> reset(t_explicit<t_bool, TAG>& t) {
    t_explicit<t_bool, TAG> tmp{t};
    t = t_explicit<t_bool, TAG>{false};
    return tmp;
  }

  template<class TAG>
  constexpr t_user<TAG> reset(t_user<TAG>& t) {
    return t_user<TAG>{reset(t.id)};
  }

///////////////////////////////////////////////////////////////////////////////

  template<typename T>
  constexpr auto x_cast(T&& in) -> typename t_remove_ref<T>::t_result&& {
    return static_cast<typename t_remove_ref<T>::t_result&&>(in);
  }

  template<typename T>
  constexpr T&& preserve(typename t_remove_ref<T>::t_result& in) {
    return static_cast<T&&>(in);
  }

  template<typename T>
  constexpr T&& preserve(typename t_remove_ref<T>::t_result&& in) {
    return x_cast(in);
  }

///////////////////////////////////////////////////////////////////////////////

  template<typename T>
  inline
  t_void swap(T& lh, T& rh) {
    T tmp {x_cast(lh)};
    lh = x_cast(rh);
    rh = x_cast(tmp);
  }

///////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
