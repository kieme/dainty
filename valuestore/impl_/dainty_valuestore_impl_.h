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
  using types::t_pass;
  using types::FARGS;
  using types::pass;

  using util::x_cast;
  using util::f_cast;

  using traits::t_if;
  using traits::t_union;
  using traits::t_if_true;
  using traits::t_not;
  using traits::t_is_not_same;
  using traits::t_is_one_of;
  using traits::t_is_there;
  using traits::t_is_not_there;
  using traits::t_remove_ref;
  using traits::t_least_one_is_true;
  using traits::t_is_arithmetic;
  using traits::t_is_enum;
  using traits::uneval;
  using traits::uneval_lvalue;
  using traits::WELL_FORMED;

  //using specific::t_n;     // FIXME should be size
  //using specific::t_n_max; // FIXME should be size

  using t_n     = types::t_n_;
  using t_n_max = types::t_n_max_;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T>
  using t_is_builtin      = t_least_one_is_true<T, t_is_arithmetic,
                                                   t_is_enum,
                                                   logical::t_is_logical>;
  template<typename T>
  using t_is_not_builtin  = t_not<t_is_builtin<T>>;

  template<typename T>
  using t_if_builtin      = t_if<t_is_builtin<T>>;
  template<typename T>
  using t_if_not_builtin  = t_if<t_is_not_builtin<T>>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename U, typename T, typename...Ts>
  using t_if_other_ = t_if_true<t_is_not_same<t_remove_ref<U>, T>,
                                t_is_not_same<t_remove_ref<U>, t_pass<U>>,
                                t_is_not_same<t_remove_ref<U>, t_fargs>,
                                t_is_one_of<U, Ts...>>;

  template<typename T>
  using t_test_swap_ = decltype(uneval<T>().swap(uneval_lvalue<T>()));

  template<typename T>
  using t_if_swap_ = t_if_true<t_is_not_builtin<T>,
                               t_is_there<t_test_swap_, T>>;

  template<typename T>
  using t_if_no_swap_  = t_if_true<t_is_not_builtin<T>,
                                   t_is_not_there<t_test_swap_, T>>;

  /////////////////////////////////////////////////////////////////////////////

  // IMPL_STORE_1_
  template<typename T, typename... Ts>
  struct t_store {
    using t_union_ = t_union<T, Ts...>;

    constexpr static t_n_  SIZEOF    = t_union_::SIZEOF;
    constexpr static t_int ALIGNMENT = t_union_::ALIGNMENT; // TODO

    using t_bytes = t_byte_[SIZEOF];

    alignas(ALIGNMENT) t_bytes bytes;
  };

  /////////////////////////////////////////////////////////////////////////////

  // IMPL_PTR_1_
  template<typename T, typename T1, typename... Ts>
  constexpr
  T* ptr(t_store<T1, Ts...>& store) noexcept {
    static_assert(t_is_one_of<T, T1, Ts...>::VALUE,
                  "T not in the union (T1, Ts)");
    return reinterpret_cast<T*>(store.bytes);
  }

  // IMPL_PTR_2_
  template<typename T, typename T1, typename... Ts>
  constexpr
  const T* Ptr(const t_store<T1, Ts...>& store) noexcept {
    static_assert(t_is_one_of<T, T1, Ts...>::VALUE,
                  "T not in the union (T1, Ts)");
    return reinterpret_cast<const T*>(store.bytes);
  }

  /////////////////////////////////////////////////////////////////////////////

  // IMPL_REF_1_
  template<typename T, typename T1, typename... Ts>
  constexpr
  T& ref(t_store<T1, Ts...>& store) noexcept {
    return *ptr<T>(store);
  }

  // IMPL_REF_2_
  template<typename T, typename T1, typename... Ts>
  constexpr
  const T& Ref(const t_store<T1, Ts...>& store) noexcept {
    return *Ptr<T>(store);
  }

  /////////////////////////////////////////////////////////////////////////////

  // IMPL_DEFAULT_CONSTRUCT_1_1_
  template<typename T, t_if_builtin<T> = WELL_FORMED>
  constexpr
  T& default_construct(T* obj) noexcept {
    return (*obj = T{});
  }

  // IMPL_DEFAULT_CONSTRUCT_1_2_
  template<typename T, t_if_not_builtin<T> = WELL_FORMED>
  inline
  T& default_construct(T* obj) noexcept {
    return *new (obj) T{};
  }

  /////////////////////////////////////////////////////////////////////////////

  // IMPL_COPY_CONSTRUCT_1_1_
  template<typename T, t_if_builtin<T> = WELL_FORMED>
  constexpr
  T& copy_construct(T* obj, const T& value) noexcept {
    return (*obj = value);
  }

  // IMPL_COPY_CONSTRUCT_1_2_
  template<typename T, t_if_not_builtin<T> = WELL_FORMED>
  inline
  T& copy_construct(T* obj, const T& value) noexcept {
    return *new (obj) T{value};
  }

  /////////////////////////////////////////////////////////////////////////////

  // IMPL_MOVE_CONSTRUCT_1_1_
  template<typename T, t_if_builtin<T> = WELL_FORMED>
  constexpr
  T& move_construct(T* obj, T&& value) noexcept {
    return (*obj = value);
  }

  // IMPL_MOVE_CONSTRUCT_1_2_
  template<typename T, t_if_not_builtin<T> = WELL_FORMED>
  inline
  T& move_construct(T* obj, T&& value) noexcept {
    return *new (obj) T{x_cast(value)};
  }

  /////////////////////////////////////////////////////////////////////////////

  // IMPL_FARGS_CONSTRUCT_1_1_
  template<typename T, typename A, t_if_builtin<T> = WELL_FORMED>
  constexpr
  T& fargs_construct(T* obj, A&& a) noexcept {
    return (*obj = a);
  }

  // IMPL_FARGS_CONSTRUCT_1_2_
  template<typename T, typename... As, t_if_not_builtin<T> = WELL_FORMED>
  inline
  T& fargs_construct(T* obj, As&&... as) noexcept {
    return *new (obj) T{f_cast<As>(as)...};
  }

  /////////////////////////////////////////////////////////////////////////////

  // IMPL_CONSTRUCT_1_1_
  template<typename T>
  inline
  T& construct(T* obj) {
    return default_construct(obj);
  }

  // IMPL_CONSTRUCT_1_2_
  template<typename T>
  inline
  T& construct(T* obj, const T& value) {
    return copy_construct(obj, value);
  }

  // IMPL_CONSTRUCT_1_3_
  template<typename T>
  inline
  T& construct(T* obj, T&& value) {
    return move_construct(obj, x_cast(value));
  }

  // IMPL_CONSTRUCT_1_4_
  template<typename T, typename... As>
  inline
  T& construct(T* obj, t_fargs, As&&... as) {
    return fargs_construct(obj, f_cast<As>(as)...);
  }

  /////////////////////////////////////////////////////////////////////////////

  // IMPL_DESTRUCT_1_1_
  template<typename T, t_if_builtin<T> = WELL_FORMED>
  constexpr
  t_void destruct(T*) noexcept {
  }

  // IMPL_DESTRUCT_1_2_
  template<typename T, t_if_not_builtin<T> = WELL_FORMED>
  inline
  t_void destruct(T* obj) noexcept {
    obj->~T();
  }

  /////////////////////////////////////////////////////////////////////////////

  // IMPL_DEFAULT_CONSTRUCT_2_1_
  template<typename T, typename T1, typename... Ts>
  inline
  T& default_construct(t_store<T1, Ts...>& store) noexcept {
    return default_construct(ptr<T>(store));
  }

  // IMPL_COPY_CONSTRUCT_2_1_
  template<typename T, typename T1, typename... Ts>
  inline
  T& copy_construct(t_store<T1, Ts...>& store, const T& value) noexcept {
    return copy_construct(ptr<T>(store), value);
  }

  // IMPL_MOVE_CONSTRUCT_2_1_
  template<typename T, typename T1, typename... Ts>
  inline
  T& move_construct(t_store<T1, Ts...>& store, T&& value) noexcept {
    return move_construct(ptr<T>(store), x_cast(value));
  }

  // IMPL_FARGS_CONSTRUCT_2_1_
  template<typename T, typename T1, typename... Ts, typename... As>
  inline
  T& fargs_construct(t_store<T1, Ts...>& store, As&&... as) noexcept {
    return fargs_construct(ptr<T>(store), f_cast<As>(as)...);
  }

  /////////////////////////////////////////////////////////////////////////////

  // IMPL_CONSTRUCT_2_1_
  template<typename T, typename T1, typename... Ts>
  inline
  T& construct(t_store<T1, Ts...>& store) {
    return default_construct<T>(store);
  }

  // IMPL_CONSTRUCT_2_2_
  template<typename T, typename T1, typename... Ts>
  inline
  T& construct(t_store<T1, Ts...>& store, const T& value) {
    return copy_construct<T>(store, value);
  }

  // IMPL_CONSTRUCT_2_3_
  template<typename T, typename T1, typename... Ts>
  inline
  T& construct(t_store<T1, Ts...>& store, T&& value) {
    return move_construct<T>(store, x_cast(value));
  }

  // IMPL_CONSTRUCT_2_4_
  template<typename T, typename T1, typename... Ts, typename... As>
  inline
  T& construct(t_store<T1, Ts...>& store, t_fargs, As&&... as) {
    return fargs_construct(store, f_cast<As>(as)...);
  }

  /////////////////////////////////////////////////////////////////////////////

  // IMPL_DESTRUCT_2_1_
  template<typename T, typename T1, typename... Ts>
  inline
  t_void destruct(t_store<T1, Ts...>& store) noexcept {
    return destruct(ptr<T>(store));
  }

  /////////////////////////////////////////////////////////////////////////////

  // IMPL_SWAP_1_1_
  template<typename T, t_if_builtin<T> = WELL_FORMED>
  constexpr
  T& swap(T& ref, T& ref1) {
    T tmp = ref1;
    ref1 = ref;
    return (ref = tmp);
  }

  // IMPL_SWAP_1_2_
  template<typename T, t_if_swap_<T> = WELL_FORMED>
  T& swap(T& ref, T& ref1) {
    ref.swap(ref1);
    return ref;
  }

  // IMPL_SWAP_1_3_
  template<typename T, t_if_no_swap_<T> = WELL_FORMED>
  constexpr
  T& swap(T& ref, T& ref1) {
    T tmp = ref1;
    ref1 = ref;
    return (ref = tmp);
  }

  /////////////////////////////////////////////////////////////////////////////

  // IMPL_SWAP_2_1_
  template<typename T, typename T1, typename... Ts>
  inline
  T& swap(t_store<T1, Ts...>& store, T& value) {
    return impl_::swap(ref<T>(store), value);
  }

  /////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
