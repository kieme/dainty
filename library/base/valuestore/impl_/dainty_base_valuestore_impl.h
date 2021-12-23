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

#ifndef _DAINTY_BASE_VALUESTORE_IMPL_H_
#define _DAINTY_BASE_VALUESTORE_IMPL_H_

#define DAINTY_BASE_INTERNAL_INCL_NEW
#include "dainty_base_internal_incl_.h"

#include "dainty_base_types.h"
#include "dainty_base_specific.h"
#include "dainty_base_logical.h"
#include "dainty_base_util.h"
#include "dainty_base_traits.h"

namespace dainty
{
namespace base
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
  using types::t_emplace;
  using types::t_void;
  using types::t_int;
  using types::t_n_;
  using types::t_byte_;
  using types::EMPLACE;

  using specific::t_n; // FIXME should be size

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

  using t_alignment = t_int;

  template<t_n_ N, t_alignment A>
  struct t_valuestore_traits {
    constexpr static t_n_        SIZEOF    = N;
    constexpr static t_alignment ALIGNMENT = A;

    using t_bytes = alignas(ALIGNMENT) t_byte_[N];

    t_valuestore_traits() = delete;
  };

  template<typename T, typename... Ts>
  struct t_store {
     using t_traits = t_valuestore_traits<traits::t_union<T, Ts...>::SIZEOF,
                                          traits::t_union<T, Ts...>::ALIGNMENT>;

     typename t_traits::t_bytes bytes;
  };

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename T1, typename... Ts>
  constexpr
  T* get_ptr(t_store<T1, Ts...>& store) noexcept {
    static_assert(traits::t_is_one_of<T, T1, Ts...>::VALUE,
                  "T not in the union (T1, Ts)");
    return reinterpret_cast<T*>(store.bytes);
  }

  template<typename T, typename T1, typename... Ts>
  constexpr
  const T* get_Ptr(const t_store<T1, Ts...>& store) noexcept {
    static_assert(traits::t_is_one_of<T, T1, Ts...>::VALUE,
                  "T not in the union (T1, Ts)");
    return reinterpret_cast<const T*>(store.bytes);
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
    return *new (obj) T{util::x_cast(value)};
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename A, t_if_builtin_<T> = traits::WELL_FORMED>
  constexpr
  T& emplace_construct(T* obj, A&& arg) noexcept {
    *obj = T{arg};
    return *obj;
  }

  template<typename T, typename... As, t_if_not_builtin_<T> = traits::WELL_FORMED>
  inline
  T& emplace_construct(T* obj, As&&... args) noexcept {
    return *new (obj) T{util::f_cast<As>(args)...};
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

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename T1, typename... Ts>
  constexpr
  T& get_ref(t_store<T1, Ts...>& store) noexcept {
    return *get_ptr<T>(store);
  }

  template<typename T, typename T1, typename... Ts>
  constexpr
  const T& get_Ref(const t_store<T1, Ts...>& store) noexcept {
    return *get_Ptr<T>(store);
  }

  template<typename T, typename T1, typename... Ts>
  inline
  T& default_construct(t_store<T1, Ts...>& store) noexcept {
    return default_construct(get_ptr<T>(store));
  }

  template<typename T, typename T1, typename... Ts>
  inline
  T& copy_construct(t_store<T1, Ts...>& store, const T& value) noexcept {
    return copy_construct(get_ptr<T>(store), value);
  }

  template<typename T, typename T1, typename... Ts>
  inline
  T& move_construct(t_store<T1, Ts...>& store, T&& value) noexcept {
    return move_construct(get_ptr<T>(store), util::x_cast(value));
  }

  template<typename T, typename T1, typename... Ts, typename... As>
  inline
  T& emplace_construct(t_store<T1, Ts...>& store, As&&... args) noexcept {
    return emplace_construct(get_ptr<T>(store), util::f_cast<As>(args)...);
  }

  template<typename T, typename T1, typename... Ts>
  inline
  t_void destruct(t_store<T1, Ts...>& store) noexcept {
    return destruct(get_ptr<T>(store));
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename T>
  class t_valuestore {
  public:
    static_assert(traits::t_all_is_true<T, traits::t_is_not_ptr,
                                           traits::t_is_not_ref>::VALUE,
                  "valuestore will not store references or pointers");

    using t_value      = t_of<T>;
    using r_value      = r_of<T>;
    using R_value      = R_of<T>;
    using p_value      = p_of<T>;
    using P_value      = P_of<T>;
    using x_value      = x_of<T>;

    using t_store      = impl_::t_store<t_value>;
    using r_store      = t_of<t_store>;
    using R_store      = R_of<t_store>;

    using r_valuestore = r_of<t_valuestore>;
    using R_valuestore = R_of<t_valuestore>;
    using x_valuestore = x_of<t_valuestore>;

    constexpr static auto SIZEOF    = t_store::t_traits::SIZEOF;
    constexpr static auto ALIGNMENT = t_store::t_traits::ALIGNMENT;

    constexpr
    t_valuestore() noexcept    = default;
    t_valuestore(R_valuestore) = delete;
    t_valuestore(x_valuestore) = delete;

    t_valuestore(R_value)            noexcept;
    t_valuestore(x_value)            noexcept;
    template<typename... As>
    t_valuestore(t_emplace, As&&...) noexcept;

    r_valuestore operator=(R_valuestore) = delete;
    r_valuestore operator=(x_valuestore) = delete;

    r_value      operator=(R_value) noexcept;
    r_value      operator=(x_value) noexcept;

    r_value      default_construct ()        noexcept;
    r_value      copy_construct    (R_value) noexcept;
    r_value      move_construct    (x_value) noexcept;
    template<typename... As>
    r_value      emplace_construct (As&&...) noexcept;

    r_valuestore destruct() noexcept;

    r_value      swap(r_value) noexcept;
    r_value      swap(x_value) noexcept;

    constexpr p_value get_ptr  ()       noexcept;
    constexpr P_value get_ptr  () const noexcept;
    constexpr P_value get_Ptr  () const noexcept;

    constexpr r_value get_ref  ()       noexcept;
    constexpr R_value get_ref  () const noexcept;
    constexpr R_value get_Ref  () const noexcept;

    constexpr r_store get_store ()       noexcept;
    constexpr R_store get_store () const noexcept;
    constexpr R_store get_Store () const noexcept;

  private:
    t_store store_;
  };

  /////////////////////////////////////////////////////////////////////////////

  template<typename T>
  inline
  t_valuestore<T>::t_valuestore(R_value value) noexcept {
    impl_::copy_construct<T>(store_, value);
  }

  template<typename T>
  inline
  t_valuestore<T>::t_valuestore(x_value value) noexcept {
    impl_::move_construct<T>(store_, util::x_cast(value));
  }

  template<typename T>
  template<typename... As>
  inline
  t_valuestore<T>::t_valuestore(t_emplace, As&&... args) noexcept {
    impl_::emplace_construct<T>(store_, util::f_cast<As>(args)...);
  }

  template<typename T>
  inline
  t_valuestore<T>::r_value
      t_valuestore<T>::operator=(R_value value) noexcept {
    return (get_ref() = value);
  }

  template<typename T>
  inline
  t_valuestore<T>::r_value
      t_valuestore<T>::operator=(x_value value) noexcept {
    return (get_ref() = util::x_cast(value));
  }

  template<typename T>
  inline
  typename t_valuestore<T>::r_value
      t_valuestore<T>::default_construct() noexcept {
    return impl_::default_construct<T>(store_);
  }

  template<typename T>
  inline
  typename t_valuestore<T>::r_value
      t_valuestore<T>::copy_construct(R_value value) noexcept {
    return impl_::copy_construct<T>(store_, value);
  }

  template<typename T>
  inline
  typename t_valuestore<T>::r_value
      t_valuestore<T>::move_construct(x_value value) noexcept {
    return impl_::move_construct<T>(store_, util::x_cast(value));
  }

  template<typename T>
  template<typename... As>
  inline
  typename t_valuestore<T>::r_value
      t_valuestore<T>::emplace_construct(As&&... args) noexcept {
    return impl_::emplace_construct<T>(store_, util::f_cast<As>(args)...);
  }

  template<typename T>
  inline
  typename t_valuestore<T>::r_valuestore t_valuestore<T>::destruct() noexcept {
    impl_::destruct<T>(store_);
    return *this;
  }

  template<typename T>
  constexpr
  t_valuestore<T>::p_value t_valuestore<T>::get_ptr() noexcept {
    return impl_::get_ptr<T>(store_);
  }

  template<typename T>
  constexpr
  t_valuestore<T>::P_value t_valuestore<T>::get_ptr() const noexcept {
    return impl_::get_Ptr<T>(store_);
  }

  template<typename T>
  constexpr
  t_valuestore<T>::P_value t_valuestore<T>::Ptr() const noexcept {
    return impl_::get_Ptr<T>(store_);
  }

  template<typename T>
  constexpr
  t_valuestore<T>::r_value t_valuestore<T>::get_ref() noexcept {
    return impl_::get_ref<T>(store_);
  }

  template<typename T>
  constexpr
  t_valuestore<T>::R_value t_valuestore<T>::get_ref() const noexcept {
    return impl_::get_Ref<T>(store_);
  }

  template<typename T>
  constexpr
  t_valuestore<T>::R_value t_valuestore<T>::get_Ref() const noexcept {
    return impl_::get_Ref<T>(store_);
  }

  template<typename T>
  constexpr
  t_valuestore<T>::r_store t_valuestore<T>::get_store() noexcept {
    return store_;
  }

  template<typename T>
  constexpr
  t_valuestore<T>::R_store t_valuestore<T>::get_store() const noexcept {
    return store_;
  }

  template<typename T>
  constexpr
  t_valuestore<T>::R_store t_valuestore<T>::get_Store() const noexcept {
    return store_;
  }

  /////////////////////////////////////////////////////////////////////////////
}
}
}
}

#endif
