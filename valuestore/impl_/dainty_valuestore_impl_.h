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

#ifndef _DAINTY_VALUESTORE_IMPL_H_
#define _DAINTY_VALUESTORE_IMPL_H_

#define DAINTY_INTERNAL_INCLUDE_NEW
#include "dainty_internal_.h"
#include "dainty_valuestore.h"
#include "dainty_specific.h"
#include "dainty_logical.h"
#include "dainty_util.h"
#include "dainty_traits.h"

namespace dainty
{
namespace valuestore
{
namespace impl_
{
  /////////////////////////////////////////////////////////////////////////////

  using types::t_of;
  using types::r_of;
  using types::R_of;
  using types::p_of;
  using types::P_of;
  using types::x_of;
  using types::t_void;
  using types::t_int;
  using types::t_n_;
  using types::t_byte_;
  using types::t_fargs;
  using types::FARGS;

  using util::x_cast;
  using util::f_cast;

  //using specific::t_n;     // FIXME should be size
  //using specific::t_n_max; // FIXME should be size

  using t_n     = types::t_n_;
  using t_n_max = types::t_n_max_;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T>
  using t_is_builtin_ =
    traits::t_least_one_is_true<T, traits::t_is_arithmetic,
                                   traits::t_is_enum,
                                   logical::t_is_logical>;

  template<typename T>
  using t_if_builtin_     = traits::t_if    <t_is_builtin_<T>>;
  template<typename T>
  using t_if_not_builtin_ = traits::t_if_not<t_is_builtin_<T>>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename... Ts>
  struct t_store {
    using t_union = traits::t_union<T, Ts...>;

    constexpr static t_n_  SIZEOF    = t_union::SIZEOF;
    constexpr static t_int ALIGNMENT = t_union::ALIGNMENT; // TODO

    using t_bytes = alignas(ALIGNMENT) t_byte_[SIZEOF];

    t_bytes bytes;
  };

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename T1, typename... Ts>
  constexpr
  T* ptr(t_store<T1, Ts...>& store) noexcept {
    static_assert(traits::t_is_one_of<T, T1, Ts...>::VALUE,
                  "T not in the union (T1, Ts)");
    return reinterpret_cast<T*>(store.bytes);
  }

  template<typename T, typename T1, typename... Ts>
  constexpr
  const T* Ptr(const t_store<T1, Ts...>& store) noexcept {
    static_assert(traits::t_is_one_of<T, T1, Ts...>::VALUE,
                  "T not in the union (T1, Ts)");
    return reinterpret_cast<const T*>(store.bytes);
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename T1, typename... Ts>
  constexpr
  T& ref(t_store<T1, Ts...>& store) noexcept {
    return *ptr<T>(store);
  }

  template<typename T, typename T1, typename... Ts>
  constexpr
  const T& Ref(const t_store<T1, Ts...>& store) noexcept {
    return *Ptr<T>(store);
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, t_if_builtin_<T> = traits::WELL_FORMED>
  constexpr
  T& default_construct(T* obj) noexcept {
    return (*obj = T{});
  }

  template<typename T, t_if_not_builtin_<T> = traits::WELL_FORMED>
  inline
  T& default_construct(T* obj) noexcept {
    return *new (obj) T{};
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, t_if_builtin_<T> = traits::WELL_FORMED>
  constexpr
  T& copy_construct(T* obj, const T& value) noexcept {
    return (*obj = value);
  }

  template<typename T, t_if_not_builtin_<T> = traits::WELL_FORMED>
  inline
  T& copy_construct(T* obj, const T& value) noexcept {
    return *new (obj) T{value};
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, t_if_builtin_<T> = traits::WELL_FORMED>
  constexpr
  T& move_construct(T* obj, T&& value) noexcept {
    *obj = value;
    return *obj;
  }

  template<typename T, t_if_not_builtin_<T> = traits::WELL_FORMED>
  inline
  T& move_construct(T* obj, T&& value) noexcept {
    return *new (obj) T{x_cast(value)};
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename A, t_if_builtin_<T> = traits::WELL_FORMED>
  constexpr
  T& fargs_construct(T* obj, A&& a) noexcept {
    *obj = T{a};
    return *obj;
  }

  template<typename T, typename... As,
           t_if_not_builtin_<T> = traits::WELL_FORMED>
  inline
  T& fargs_construct(T* obj, As&&... as) noexcept {
    return *new (obj) T{f_cast<As>(as)...};
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename T>
  inline
  T& construct(T* obj) {
    return default_construct(obj);
  }

  template<typename T>
  inline
  T& construct(T* obj, const T& value) {
    return copy_construct(obj, value);
  }

  template<typename T>
  inline
  T& construct(T* obj, T&& value) {
    return move_construct(obj, x_cast(value));
  }

  template<typename T, typename A, typename... As>
  inline
  T& construct(T* obj, t_fargs, A&& a, As&&... as) {
    return fargs_construct(obj, f_cast<A>(a), f_cast<As>(as)...);
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, t_if_builtin_<T> = traits::WELL_FORMED>
  constexpr
  t_void destruct(T*) noexcept {
  }

  template<typename T, t_if_not_builtin_<T> = traits::WELL_FORMED>
  inline
  t_void destruct(T* obj) noexcept {
    obj->~T();
  }

  template<typename T, t_if_builtin_<T> = traits::WELL_FORMED>
  constexpr
  t_void destruct(t_n_max, T*) {
  }

  template<typename T, t_if_not_builtin_<T> = traits::WELL_FORMED>
  inline
  t_void destruct(t_n_max max, T* arr) {
    for (int ix = max - 1; ix; --ix) // TODO
      arr[ix].~T();
    arr[0].~T();
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename T1, typename... Ts>
  inline
  T& default_construct(t_store<T1, Ts...>& store) noexcept {
    return default_construct(ptr<T>(store));
  }

  template<typename T, typename T1, typename... Ts>
  inline
  T& copy_construct(t_store<T1, Ts...>& store, const T& value) noexcept {
    return copy_construct(ptr<T>(store), value);
  }

  template<typename T, typename T1, typename... Ts>
  inline
  T& move_construct(t_store<T1, Ts...>& store, T&& value) noexcept {
    return move_construct(ptr<T>(store), x_cast(value));
  }

  template<typename T, typename T1, typename... Ts, typename... As>
  inline
  T& fargs_construct(t_store<T1, Ts...>& store, As&&... as) noexcept {
    return frags_construct(ptr<T>(store), f_cast<As>(as)...);
  }

  template<typename T, typename T1, typename... Ts>
  inline
  t_void destruct(t_store<T1, Ts...>& store) noexcept {
    return destruct(ptr<T>(store));
  }

  /////////////////////////////////////////////////////////////////////////////

}
}
}

#endif
