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

#ifndef _DAINTY_VALUESTORE_H_
#define _DAINTY_VALUESTORE_H_

#include "impl_/dainty_valuestore_impl_.h"

namespace dainty
{
namespace valuestore
{
  /////////////////////////////////////////////////////////////////////////////

  using impl_::t_store;
  using impl_::t_fargs;
  using impl_::FARGS;

  using impl_::ptr;
  using impl_::Ptr;
  using impl_::ref;
  using impl_::Ref;

  using impl_::default_construct;
  using impl_::copy_construct;
  using impl_::move_construct;
  using impl_::fargs_construct;
  using impl_::construct;
  using impl_::destruct;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename... Ts>
  class t_valuestore {
  //  static_assert(traits::t_all_is_true<T, traits::t_is_not_ptr,
  //                                         traits::t_is_not_ref>::VALUE,
  static_assert(traits::t_all_is_true<T, traits::t_is_not_ref>::VALUE,
                  "valuestore will not store references or pointers");

    using t_value_      = types::t_of<T>;
    using r_value_      = types::r_of<t_value_>;
    using R_value_      = types::R_of<t_value_>;
    using p_value_      = types::p_of<t_value_>;
    using P_value_      = types::P_of<t_value_>;
    using x_value_      = types::x_of<t_value_>;

    using t_store_      = impl_::t_store<t_value_>;
    using r_store_      = types::t_of<t_store_>;
    using R_store_      = types::R_of<t_store_>;

    using r_valuestore_ = types::r_of<t_valuestore>;
    using R_valuestore_ = types::R_of<t_valuestore>;
    using x_valuestore_ = types::x_of<t_valuestore>;

  public:
    using t_value = t_value_;

    constexpr static auto SIZEOF    = t_store_::SIZEOF;
    constexpr static auto ALIGNMENT = t_store_::ALIGNMENT;

    constexpr
    t_valuestore() noexcept     = default;
    t_valuestore(R_valuestore_) = delete;  // TODO. raw copy
    t_valuestore(x_valuestore_) = delete;

    t_valuestore(R_value_)         noexcept;
    t_valuestore(x_value_)         noexcept;
    template<typename... As>
    t_valuestore(t_fargs, As&&...) noexcept;

    r_valuestore_ operator=(R_valuestore_) = delete;
    r_valuestore_ operator=(x_valuestore_) = delete;

    r_value_      operator=(R_value_) noexcept;
    r_value_      operator=(x_value_) noexcept;

    r_value_      default_construct ()         noexcept;
    r_value_      copy_construct    (R_value_) noexcept;
    r_value_      move_construct    (x_value_) noexcept;
    template<typename... As>
    r_value_      fargs_construct   (As&&...)  noexcept;

    r_valuestore_ destruct() noexcept;

    r_value_      swap(r_value_) noexcept;
    r_value_      swap(x_value_) noexcept;

    constexpr p_value_ ptr  ()       noexcept;
    constexpr P_value_ ptr  () const noexcept;
    constexpr P_value_ Ptr  () const noexcept;

    constexpr r_value_ ref  ()       noexcept;
    constexpr R_value_ ref  () const noexcept;
    constexpr R_value_ Ref  () const noexcept;

    constexpr r_store_ store ()       noexcept;
    constexpr R_store_ store () const noexcept;
    constexpr R_store_ Store () const noexcept;

  private:
    t_store_ store_;
  };

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename... Ts>
  inline
  t_valuestore<T, Ts...>::t_valuestore(R_value_ value) noexcept {
    impl_::copy_construct<T>(store_, value);
  }

  template<typename T, typename... Ts>
  inline
  t_valuestore<T, Ts...>::t_valuestore(x_value_ value) noexcept {
    impl_::move_construct<T>(store_, util::x_cast(value));
  }

  template<typename T, typename... Ts>
  template<typename... As>
  inline
  t_valuestore<T, Ts...>::t_valuestore(t_fargs, As&&... as) noexcept {
    impl_::fargs_construct<T>(store_, util::f_cast<As>(as)...);
  }

  template<typename T, typename... Ts>
  inline
  typename t_valuestore<T, Ts...>::r_value_
      t_valuestore<T, Ts...>::operator=(R_value_ value) noexcept {
    return (ref() = value);
  }

  template<typename T, typename... Ts>
  inline
  typename t_valuestore<T, Ts...>::r_value_
      t_valuestore<T, Ts...>::operator=(x_value_ value) noexcept {
    return (ref() = util::x_cast(value));
  }

  template<typename T, typename... Ts>
  inline
  typename t_valuestore<T, Ts...>::r_value_
      t_valuestore<T, Ts...>::default_construct() noexcept {
    return impl_::default_construct<T>(store_);
  }

  template<typename T, typename... Ts>
  inline
  typename t_valuestore<T, Ts...>::r_value_
      t_valuestore<T, Ts...>::copy_construct(R_value_ value) noexcept {
    return impl_::copy_construct<T>(store_, value);
  }

  template<typename T, typename... Ts>
  inline
  typename t_valuestore<T, Ts...>::r_value_
      t_valuestore<T, Ts...>::move_construct(x_value_ value) noexcept {
    return impl_::move_construct<T>(store_, util::x_cast(value));
  }

  template<typename T, typename... Ts>
  template<typename... As>
  inline
  typename t_valuestore<T, Ts...>::r_value_
      t_valuestore<T, Ts...>::fargs_construct(As&&... as) noexcept {
    return impl_::fargs_construct<T>(store_, util::f_cast<As>(as)...);
  }

  template<typename T, typename... Ts>
  inline
  typename t_valuestore<T, Ts...>::r_valuestore_
       t_valuestore<T, Ts...>::destruct() noexcept {
    impl_::destruct<T>(store_);
    return *this;
  }

  template<typename T, typename... Ts>
  constexpr
  typename t_valuestore<T, Ts...>::p_value_ t_valuestore<T, Ts...>::ptr() noexcept {
    return impl_::ptr<T>(store_);
  }

  template<typename T, typename... Ts>
  constexpr
  typename t_valuestore<T, Ts...>::P_value_ t_valuestore<T, Ts...>::ptr() const noexcept {
    return impl_::Ptr<T>(store_);
  }

  template<typename T, typename... Ts>
  constexpr
  typename t_valuestore<T, Ts...>::P_value_ t_valuestore<T, Ts...>::Ptr() const noexcept {
    return impl_::Ptr<T>(store_);
  }

  template<typename T, typename... Ts>
  constexpr
  typename t_valuestore<T, Ts...>::r_value_ t_valuestore<T, Ts...>::ref() noexcept {
    return impl_::ref<T>(store_);
  }

  template<typename T, typename... Ts>
  constexpr
  typename t_valuestore<T, Ts...>::R_value_ t_valuestore<T, Ts...>::ref() const noexcept {
    return impl_::Ref<T>(store_);
  }

  template<typename T, typename... Ts>
  constexpr
  typename t_valuestore<T, Ts...>::R_value_ t_valuestore<T, Ts...>::Ref() const noexcept {
    return impl_::Ref<T>(store_);
  }

  template<typename T, typename... Ts>
  constexpr
  typename t_valuestore<T, Ts...>::r_store_ t_valuestore<T, Ts...>::store() noexcept {
    return store_;
  }

  template<typename T, typename... Ts>
  constexpr
  typename t_valuestore<T, Ts...>::R_store_ t_valuestore<T, Ts...>::store() const noexcept {
    return store_;
  }

  template<typename T, typename... Ts>
  constexpr
  typename t_valuestore<T, Ts...>::R_store_ t_valuestore<T, Ts...>::Store() const noexcept {
    return store_;
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename... Ts>
  inline
  T& default_construct(t_valuestore<T, Ts...>& store) noexcept {
    return store.default_construct();
  }

  template<typename T, typename... Ts>
  inline
  T& copy_construct(t_valuestore<T, Ts...>& store, const T& value) noexcept {
    return store.copy_construct(value);
  }

  template<typename T, typename... Ts>
  inline
  T& move_construct(t_valuestore<T, Ts...>& store, T&& value) noexcept {
    return store.move_construct(util::x_cast(value));
  }

  template<typename T, typename... Ts, typename... As>
  inline
  T& fargs_construct(t_valuestore<T, Ts...>& store, As&&... as) noexcept {
    return store.fargs_construct(util::f_cast<As>(as)...);
  }

  template<typename T, typename... Ts>
  inline
  t_valuestore<T, Ts...>& destruct(t_valuestore<T, Ts...>& store) noexcept {
    return store.destruct();
  }

  template<typename T, typename... Ts>
  inline
  T& swap(t_valuestore<T, Ts...>& store, T& value) noexcept {
    return store.swap(value);
  }

  template<typename T, typename... Ts>
  inline
  T& swap(t_valuestore<T, Ts...>& store, T&& value) noexcept {
    return store.swap(util::x_cast(value));
  }

  template<typename T, typename... Ts>
  inline
  T* ptr(t_valuestore<T, Ts...>& store) noexcept {
    return store.ptr();
  }

  template<typename T, typename... Ts>
  inline
  T* ptr(const t_valuestore<T, Ts...>& store) noexcept {
    return store.Ptr();
  }

  template<typename T, typename... Ts>
  inline
  T* Ptr(const t_valuestore<T, Ts...>& store) noexcept {
    return store.Ptr();
  }

  template<typename T, typename... Ts>
  inline
  T& ref(t_valuestore<T, Ts...>& store) noexcept {
    return store.ref();
  }

  template<typename T, typename... Ts>
  inline
  T& ref(const t_valuestore<T, Ts...>& store) noexcept {
    return store.ref();
  }

  template<typename T, typename... Ts>
  inline
  T& Ref(const t_valuestore<T, Ts...>& store) noexcept {
    return store.Ref();
  }

  template<typename T, typename... Ts>
  inline
  t_store<T>& store(t_valuestore<T, Ts...>& store) noexcept {
    return store.store();
  }

  template<typename T, typename... Ts>
  inline
  t_store<T>& store(const t_valuestore<T, Ts...>& store) noexcept {
    return store.store();
  }

  template<typename T, typename... Ts>
  inline
  t_store<T>& Store(const t_valuestore<T, Ts...>& store) noexcept {
    return store.Store();
  }

  /////////////////////////////////////////////////////////////////////////////
}
}

#endif
