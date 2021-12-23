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

#ifndef _DAINTY_BASE_ARRAY_H_
#define _DAINTY_BASE_ARRAY_H_

#include <new>
#include "dainty_base_util.h"
#include "dainty_base_traits.h"

namespace dainty
{
namespace base
{
namespace array
{
  /////////////////////////////////////////////////////////////////////////////

  using types::t_void;
  using types::t_bool;

  using util::f_cast;

  enum t_init_list_ { INIT_LIST };
  enum t_init_none_ { INIT_NONE };

  /////////////////////////////////////////////////////////////////////////////

  template<typename U, typename T, typename P>
  constexpr
  U cast_(P store, int ix) {
    return reinterpret_cast<U>(store + (ix * sizeof(T)));
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename Arg,
           traits::t_opt_if_scalar<types::t_opt1, T> = types::OPT1>
  constexpr
  t_void construct_(T* value, Arg&& arg) {
    *value = arg;
  }

  template<typename T, typename Arg,
           traits::t_opt_if_not_scalar<types::t_opt2, T> = types::OPT2>
  inline
  t_void construct_(T* value, Arg&& arg) {
    new (value) T(f_cast<Arg>(arg));
  }

  /////////////////////////////////////////////////////////////////////////////

  template<int N, typename T,
           traits::t_opt_if_scalar<types::t_opt1, T> = types::OPT1>
  constexpr
  t_void destruct_(T (&ref)[N]) {
  }

  template<int N, typename T,
           traits::t_opt_if_not_scalar<types::t_opt2, T> = types::OPT2>
  inline
  t_void destruct_(T (&ref)[N]) {
    for (int ix = N - 1; ix; --ix)
      ref[ix].~T();
    ref[0].~T();
  }

  /////////////////////////////////////////////////////////////////////////////

  template<int N, typename T>
  class t_array {
    using r_array_ = t_array&;
    using R_array_ = const t_array&;

    template<typename Arg>
    inline
    int init_(int ix, Arg&& arg) {
      construct_(ptr(ix), f_cast<Arg>(arg));
      return ix + 1;
    }

    template<typename Arg, typename... Args>
    inline
    int init_(int ix, Arg&& arg, Args&&... args) {
      return init_(init_(ix, f_cast<Arg>(arg)), f_cast<Args>(args)...);
    }

  public:
    typedef T             t_value;
    typedef T&            r_value_;
    typedef const T&      R_value_;
    typedef T*            p_value_;
    typedef const T*      P_value_;
    typedef t_value       t_arr[N];
    typedef t_arr*        p_arr_;
    typedef const t_arr*  P_arr_;
    typedef t_arr&        r_arr_;
    typedef const t_arr&  R_arr_;

    constexpr
    static int capacity() { return N; }

    inline
    t_array() {
      for (int ix = 0; ix < N; ix = init_(ix, T()));
    }

    constexpr
    t_array(t_init_none_) {
    }

    template<typename U, typename... Us>
    inline
    t_array(t_init_list_, U&& u, Us&&... us) {
      using t_int_ = traits::t_opt_if<int, traits::t_is_truth<(sizeof...(Us) + 1 <= N)>>;
      for (t_int_ ix = init_(0, f_cast<U>(u), f_cast<Us>(us)...);
           ix < N; ix = init_(ix, T()));
    }

    inline
    t_array(R_array_ arr) {
      for (int ix = 0; ix < N; ix = init_(ix, arr.Get(ix)));
    }

    template<typename U>
    inline
    t_array(const t_array<N, U>& arr) {
      for (int ix = 0; ix < N; ix = init_(ix, arr.Get(ix)));
    }

    template<typename U>
    inline
    t_array(const U (&arr)[N]) {
      for (int ix = 0; ix < N; ix = init_(ix, arr[ix]));
    }

    inline
    r_array_ operator=(R_array_ arr) {
      for (int ix = 0; ix < N; ++ix)
        set(ix) = arr.Get(ix);
      return *this;
    }

    template<typename U>
    inline
    r_array_ operator=(const U (&arr)[N]) {
      for (int ix = 0; ix < N; ix = init_(ix, arr[ix]));
    }

    template<typename U>
    inline
    r_array_ operator=(const t_array<N, U>& arr) {
      for (int ix = 0; ix < N; ++ix)
        set(ix) = arr.Get(ix);
      return *this;
    }

    inline
    ~t_array() {
      array::destruct_(get());
    }

    template<typename U, typename... Us>
    inline
    r_array_ init(t_init_list_, U&& u, Us&&... us) {
      using t_int_ = traits::t_opt_if<int, traits::t_is_truth<(sizeof...(Us) + 1 <= N)>>;
      for (t_int_ ix = init_(0, f_cast<U>(u), f_cast<Us>(us)...);
           ix < N; ix = init_(ix, T()));
      return *this;
    }

    template<typename U>
    inline
    r_array_ init(const U (&arr)[N]) {
      for (int ix = 0; ix < N; ix = init_(ix, arr[ix]));
    }

    template<typename U>
    inline
    r_array_ init(const t_array<N, U>& arr) {
      for (int ix = 0; ix < N; ix = init_(ix, arr.Get(ix)));
      return *this;
    }

    inline
    r_array_ destruct() {
      array::destruct_(get());
      return *this;
    }

    inline
    p_value_ ptr(int ix = 0) {
      return cast_<p_value_, t_value>(store_, ix);
    }

    inline
    P_value_ Ptr(int ix = 0) const {
      return cast_<P_value_, t_value>(store_, ix);
    }

    inline
    R_value_ Get(int ix) const {
      return *Ptr(ix);
    }

    inline
    r_value_ set(int ix) {
      return *ptr(ix);
    }

    inline
    r_value_ operator[](int ix) {
      return set(ix);
    }

    inline
    R_value_ operator[](int ix) const {
      return Get(ix);
    }

    inline
    operator p_value_ () {
      return ptr();
    }

    inline
    operator P_value_ () const {
      return Ptr();
    }

    inline
    R_arr_ get() const {
      return *reinterpret_cast<P_arr_>(store_);
    }

    inline
    r_arr_ set() {
      return *reinterpret_cast<p_arr_>(store_);
    }

    template<typename U>
    inline
    t_bool is_equal(const t_array<N, U>& arr) const {
      int ix = 0;
      for (; ix < N && Get(ix) == arr[ix]; ++ix);
      return ix == N;
    }

    inline
    p_value_ begin() {
      return ptr();
    }

    inline
    p_value_ end() {
      return ptr() + N;
    }

    inline
    P_value_ Begin() const {
      return Ptr();
    }

    inline
    P_value_ End() const {
      return Ptr() + N;
    }

    inline
    p_value_ rbegin() {
      return ptr() + N - 1;
    }

    inline
    p_value_ rend() {
      return ptr() - 1;
    }

    inline
    P_value_ RBegin() const {
      return Ptr() + N - 1;
    }

    inline
    P_value_ REnd() const {
      return Ptr() - 1;
    }

  private:
    char store_[N * sizeof(t_value)]
      __attribute__((aligned(__alignof(t_value))));
  };

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename... Ts>
  inline
  t_array<sizeof...(Ts) + 1, T> make_array(T&& t, Ts&&... ts) {
    return t_array<sizeof...(Ts) + 1, T>(INIT_LIST, f_cast<T>(t), f_cast<Ts>(ts)...);
  }

  template<int N, typename T, typename... Ts>
  inline
  t_array<N, T> make_array(T&& t, Ts&&... ts) {
    return t_array<N, T>(INIT_LIST, f_cast<T>(t), f_cast<Ts>(ts)...);
  }

  template<int N, typename T>
  inline
  t_array<N, T> make_array(T (&arr)[N]) {
    return t_array<N, T>(arr);
  }

  template<int N, typename T>
  inline
  t_array<N, T> make_array(const T (&arr)[N]) {
    return t_array<N, T>(arr);
  }

  template<int N, typename T>
  inline
  t_array<N, T> make_array() {
    return t_array<N, T>();
  }

  template<int N, typename T>
  inline
  t_array<N, T> make_array_no_init() {
    return t_array<N, T>(INIT_NONE);
  }

  /////////////////////////////////////////////////////////////////////////////

  template<int N, typename T, typename U>
  inline
  t_bool operator==(const t_array<N, T>& lh, const t_array<N, U>& rh) {
    return lh.is_equal(rh);
  }

  template<int N, typename T, typename U>
  inline
  t_bool operator!=(const t_array<N, T>& lh, const t_array<N, U>& rh) {
    return !(lh == rh);
  }

  /////////////////////////////////////////////////////////////////////////////
}
}
}
#endif
