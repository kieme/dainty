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

#ifndef _DAINTY_ARRAY_IMPL_H_
#define _DAINTY_ARRAY_IMPL_H_

#define DAINTY_INTERNAL_INCLUDE_NEW
#define DAINTY_INTERNAL_C_STRING
#include "dainty_internal_.h"
#include "dainty_valuestore.h"
#include "dainty_alloc.h"
#include "dainty_util.h"
#include "dainty_traits.h"

namespace dainty
{
namespace array
{
namespace impl_
{
  /////////////////////////////////////////////////////////////////////////////

  using types::t_void;
  using types::t_bool;
  using types::FARGS;

  using valuestore::default_construct;
  using valuestore::fargs_construct;
  using valuestore::destruct;
  using valuestore::t_valuestore;

  using t_u1_     = types::t_u1_;
  using t_n       = types::t_i4_;
  using t_n_max   = t_n;
  using t_n_bytes = t_n;

  using util::f_cast;

  using alloc::alloc;
  using alloc::dealloc;

  /////////////////////////////////////////////////////////////////////////////

  enum t_init_list_   { INIT_LIST };
  enum t_init_none_   { INIT_NONE };
  enum t_init_custom_ { INIT_CUSTOM };

  /////////////////////////////////////////////////////////////////////////////

  template<typename T>
  constexpr
  T max_of(T t) {
    return t;
  }

  template<typename T, typename... Ts>
  constexpr
  T max_of(T t, Ts... ts) {
    T max = max_of(ts...);
    return t > max ? t : max;
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename U, typename T, typename P>
  constexpr
  U cast_(P store, int ix) {
    return reinterpret_cast<U>(store + (ix * sizeof(T)));
  }

  /////////////////////////////////////////////////////////////////////////////

  /*
  template<typename T,
           traits::t_opt_if_scalar<types::t_opt1, T> = types::OPT1>
  constexpr
  t_void construct_(T* value) {
    *value = T();
  }

  template<typename T, typename Arg,
           traits::t_opt_if_scalar<types::t_opt1, T> = types::OPT1>
  constexpr
  t_void construct_(T* value, Arg&& arg) {
    *value = arg;
  }

  template<typename T,
           traits::t_opt_if_not_scalar<types::t_opt2, T> = types::OPT2>
  inline
  t_void construct_(T* value) {
    new (value) T;
  }

  template<typename T, typename Arg,
           traits::t_opt_if_not_scalar<types::t_opt2, T> = types::OPT2>
  inline
  t_void construct_(T* value, Arg&& arg) {
    new (value) T(f_cast<Arg>(arg));
  }
  */

  /////////////////////////////////////////////////////////////////////////////

  /*
  template<typename T, traits::t_opt_if_scalar<types::t_opt1, T> = types::OPT1>
  constexpr
  t_void destruct_(t_n_max, T*) {
  }

  template<int N, typename T,
           traits::t_opt_if_scalar<types::t_opt1, T> = types::OPT1>
  constexpr
  t_void destruct_(T (&ref)[N]) {
  }

  template<typename T, traits::t_opt_if_not_scalar<types::t_opt2, T> = types::OPT2>
  inline
  t_void destruct_(t_n_max max, T* arr) {
    for (int ix = max - 1; ix; --ix)
      arr[ix].~T();
    arr[0].~T();
  }

  template<int N, typename T,
           traits::t_opt_if_not_scalar<types::t_opt2, T> = types::OPT2>
  inline
  t_void destruct_(T (&arr)[N]) {
    destruct_(N, &arr[0]);
  }
  */

  /////////////////////////////////////////////////////////////////////////////

  template<typename T>
  inline
  int init_val_(int ix, T* arr) {
    default_construct(arr + ix);
    return ix + 1;
  }

  template<typename T, typename U>
  inline
  int init_val_(int ix, T* arr, U&& u) {
    fargs_construct(arr + ix, f_cast<U>(u));
    return ix + 1;
  }

  namespace help_ {
    template<typename T>
    constexpr
    int init_(int ix, T*) {
      return ix;
    }

    template<typename T, typename U, typename... Us>
    inline
    int init_(int ix, T* arr, U&& u, Us&&... us) {
      return init_(init_val_(ix, arr, f_cast<U>(u)), arr, f_cast<Us>(us)...);
    }
  }

  template<typename T, typename U, typename... Us>
  inline
  t_void init_(t_n_max max, T* arr, t_init_list_, U&& u, Us&&... us) {
    int ix = help_::init_(0, arr, f_cast<U>(u), f_cast<Us>(us)...);
    for (; ix < max; ix = init_val_(ix, arr));
  }

  template<typename T, typename C>
  inline
  t_void init_(t_n_max max, T* arr, t_init_custom_, C&& custom) {
    for (int ix = 0; ix < max; ++ix)
      custom(ix, arr + ix);
  }

  template<typename T>
  inline
  t_void init_(t_n_max max, T* arr) {
    for (int ix = 0; ix < max; ix = init_val_(ix, arr));
  }

  template<typename T, typename U>
  inline
  t_void init_(t_n_max max, T* arr, const U* arr1) {
    for (int ix = 0; ix < max; ix = init_val_(ix, arr, arr1[ix]));
  }

  template<typename T, typename U>
  inline
  t_void init_(t_n_max max, T* arr, t_n_max max1, const U* arr1) {
    init_(max1, arr, arr1);
    for (int ix = max1; ix < max; ix = init_val_(ix, arr));
  }

  /////////////////////////////////////////////////////////////////////////////

  template<int N, typename T, typename U>
  inline
  t_void copy_(T (&arr)[N], const U (&arr1)[N]) {
    for (int ix = 0; ix < N; ++ix)
      arr[ix] = arr1[ix];
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename T>
  inline
  t_bool is_equal_(t_n_max max, const T* arr, t_n_max max1, const T* arr1) {
    if (max != max1)
      return false;
    for (int ix = 0; ix < max; ++ix)
      if (arr[ix] != arr1[ix])
        return false;
    return true;
  }

  template<int N, typename T>
  inline
  t_bool is_equal_(const T (&arr)[N], const T (&arr1)[N]) {
    for (int ix = 0; ix < N; ++ix)
      if (arr[ix] != arr1[ix])
        return false;
    return true;
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename F>
  inline
  t_void each_(t_n_max max, T* arr, F&& func) {
    for (int ix = 0; ix < max; ++ix)
      func(ix, arr[ix]);
  }

  template<typename T, typename F>
  inline
  t_void Each_(t_n_max max, const T* arr, F&& func) {
    for (int ix = 0; ix < max; ++ix)
      func(ix, arr[ix]);
  }

  template<int N, typename T, typename F>
  inline
  t_void each_(T (&arr)[N], F&& func) {
    each_(N, &arr[0], f_cast<F>(func));
  }

  template<int N, typename T, typename F>
  inline
  t_void Each_(const T (&arr)[N], F&& func) {
    each_(N, &arr[0], f_cast<F>(func));
  }

  /////////////////////////////////////////////////////////////////////////////
}
}
}
#endif
