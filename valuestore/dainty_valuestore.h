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

  using impl_::t_pass;
  using impl_::pass;
  using impl_::t_store;
  using impl_::t_fargs;
  using impl_::t_if_builtin;
  using impl_::t_if_not_builtin;
  using impl_::FARGS;
  using impl_::WELL_FORMED;

  using impl_::ptr;
  using impl_::Ptr;
  using impl_::ref;
  using impl_::Ref;

  using impl_::construct;
  using impl_::default_construct;
  using impl_::copy_construct;
  using impl_::move_construct;
  using impl_::fargs_construct;
  using impl_::destruct;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename... Ts>
  class t_valuestore {
    //  traits::t_is_not_ptr,
    static_assert(traits::t_all_is_true<T, traits::t_is_not_ref>::VALUE,
                    "valuestore will not store references or pointers");

    using t_value_      = types::t_of<T>;
    using r_value_      = types::r_of<t_value_>;
    using R_value_      = types::R_of<t_value_>;
    using p_value_      = types::p_of<t_value_>;
    using P_value_      = types::P_of<t_value_>;
    using x_value_      = types::x_of<t_value_>;

    using t_store_      = impl_::t_store<t_value_, Ts...>;
    using r_store_      = types::t_of<t_store_>;
    using R_store_      = types::R_of<t_store_>;

    using r_valuestore_ = types::r_of<t_valuestore>;
    using R_valuestore_ = types::R_of<t_valuestore>;
    using x_valuestore_ = types::x_of<t_valuestore>;

  public:
    ///////////////////////////////////////////////////////////////////////////

    using t_value = t_value_;

    constexpr
    static auto SIZEOF    = t_store_::SIZEOF;
    constexpr
    static auto ALIGNMENT = t_store_::ALIGNMENT;

    ///////////////////////////////////////////////////////////////////////////

    constexpr
    t_valuestore() noexcept     = default;               // VALUESTORE_1_1_1_
    t_valuestore(R_valuestore_) = delete;                // VALUESTORE_1_1_2_
    t_valuestore(x_valuestore_) = delete;                // VALUESTORE_1_1_3_

    t_valuestore(R_value_)         noexcept;             // VALUESTORE_1_1_4_
    t_valuestore(x_value_)         noexcept;             // VALUESTORE_1_1_5_

    template<typename... As>
    t_valuestore(t_fargs, As&&...) noexcept;             // VALUESTORE_1_1_6_

    template<typename U, typename... As>
    t_valuestore(types::t_pass<U>, As&&...) noexcept;    // VALUESTORE_1_1_7_

    template<typename U,
             impl_::t_if_other_<U, T, Ts...> = traits::WELL_FORMED>
    t_valuestore(U&&)              noexcept;             // VALUESTORE_1_1_8_

    ///////////////////////////////////////////////////////////////////////////

    r_valuestore_ operator=(R_valuestore_) = delete;     // VALUESTORE_1_2_1_
    r_valuestore_ operator=(x_valuestore_) = delete;     // VALUESTORE_1_2_2_

    r_value_      operator=(R_value_) noexcept;          // VALUESTORE_1_2_3_
    r_value_      operator=(x_value_) noexcept;          // VALUESTORE_1_2_4_

    template<typename U,
             impl_::t_if_other_<U, T, Ts...> = traits::WELL_FORMED>
    U&            operator=(U&&)      noexcept;          // VALUESTORE_1_2_5_

    ///////////////////////////////////////////////////////////////////////////

    r_value_      default_construct ()         noexcept; // VALUESTORE_1_3_1_

    template<typename U>
    U&            default_construct ()         noexcept; // VALUESTORE_1_3_2_

    ///////////////////////////////////////////////////////////////////////////

    r_value_      copy_construct    (R_value_) noexcept; // VALUESTORE_1_4_1_

    template<typename U>
    U&            copy_construct    (const U&) noexcept; // VALUESTORE_1_4_2_

    ///////////////////////////////////////////////////////////////////////////

    r_value_      move_construct    (x_value_) noexcept; // VALUESTORE_1_5_1_

    template<typename U>
    U&            move_construct    (U&&)      noexcept; // VALUESTORE_1_5_2_

    ///////////////////////////////////////////////////////////////////////////

    template<typename... As>
    r_value_      fargs_construct   (As&&...)  noexcept; // VALUESTORE_1_6_1_

    template<typename U, typename... As>
    U&            fargs_construct   (As&&...)  noexcept; // VALUESTORE_1_6_2_

    ///////////////////////////////////////////////////////////////////////////

    r_valuestore_ destruct() noexcept;                   // VALUESTORE_1_7_1_

    template<typename U>
    r_valuestore_ destruct() noexcept;                   // VALUESTORE_1_7_2_

    ///////////////////////////////////////////////////////////////////////////

    r_value_      swap(r_value_) noexcept;               // VALUESTORE_1_8_1_

    template<typename U, impl_::t_if_other_<U, T, Ts...> = traits::WELL_FORMED>
    U&            swap(U&)       noexcept;               // VALUESTORE_1_8_2_

    ///////////////////////////////////////////////////////////////////////////

    p_value_ ptr  ()       noexcept;                     // VALUESTORE_1_9_1_
    P_value_ ptr  () const noexcept;                     // VALUESTORE_1_9_2_
    P_value_ Ptr  () const noexcept;                     // VALUESTORE_1_9_3_

    template<typename U>
    U*       ptr  ()       noexcept;                     // VALUESTORE_1_9_4_
    template<typename U>
    const U* ptr  () const noexcept;                     // VALUESTORE_1_9_5_
    template<typename U>
    const U* Ptr  () const noexcept;                     // VALUESTORE_1_9_6_

    ///////////////////////////////////////////////////////////////////////////

    r_value_ ref  ()       noexcept;                     // VALUESTORE_1_10_1_
    R_value_ ref  () const noexcept;                     // VALUESTORE_1_10_2_
    R_value_ Ref  () const noexcept;                     // VALUESTORE_1_10_3_

    template<typename U>
    U&       ref  ()       noexcept;                     // VALUESTORE_1_10_4_
    template<typename U>
    const U& ref  () const noexcept;                     // VALUESTORE_1_10_5_
    template<typename U>
    const U& Ref  () const noexcept;                     // VALUESTORE_1_10_6_

    ///////////////////////////////////////////////////////////////////////////

    r_store_ store ()       noexcept;                    // VALUESTORE_1_11_1_
    R_store_ store () const noexcept;                    // VALUESTORE_1_11_2_
    R_store_ Store () const noexcept;                    // VALUESTORE_1_11_3_

    ///////////////////////////////////////////////////////////////////////////

  private:
    t_store_ store_;
  };

  /////////////////////////////////////////////////////////////////////////////

  // VALUESTORE_1_1_4_
  template<typename T, typename... Ts>
  inline
  t_valuestore<T, Ts...>::t_valuestore(R_value_ value) noexcept {
    impl_::copy_construct<T>(store_, value);
  }

  // VALUESTORE_1_1_5_
  template<typename T, typename... Ts>
  inline
  t_valuestore<T, Ts...>::t_valuestore(x_value_ value) noexcept {
    impl_::move_construct<T>(store_, util::x_cast(value));
  }

  // VALUESTORE_1_1_6_
  template<typename T, typename... Ts>
  template<typename... As>
  inline
  t_valuestore<T, Ts...>::t_valuestore(t_fargs, As&&... as) noexcept {
    impl_::fargs_construct<T>(store_, util::f_cast<As>(as)...);
  }

  // VALUESTORE_1_1_7_
  template<typename T, typename... Ts>
  template<typename U, typename... As>
  inline
  t_valuestore<T, Ts...>::t_valuestore(types::t_pass<U>, As&&... as) noexcept {
    impl_::fargs_construct<U>(store_, util::f_cast<As>(as)...);
  }

  // VALUESTORE_1_1_8_
  template<typename T, typename... Ts>
  template<typename U, impl_::t_if_other_<U, T, Ts...>>
  inline
  t_valuestore<T, Ts...>::t_valuestore(U&& u) noexcept {
    impl_::fargs_construct<U>(store_, util::f_cast<U>(u));
  }

  /////////////////////////////////////////////////////////////////////////////

  // VALUESTORE_1_2_3_
  template<typename T, typename... Ts>
  inline
  typename t_valuestore<T, Ts...>::r_value_
      t_valuestore<T, Ts...>::operator=(R_value_ value) noexcept {
    return (ref() = value);
  }

  // VALUESTORE_1_2_4_
  template<typename T, typename... Ts>
  inline
  typename t_valuestore<T, Ts...>::r_value_
      t_valuestore<T, Ts...>::operator=(x_value_ value) noexcept {
    return (ref() = util::x_cast(value));
  }

  // VALUESTORE_1_2_5_
  template<typename T, typename... Ts>
  template<typename U, impl_::t_if_other_<U, T, Ts...>>
  inline
  U& t_valuestore<T, Ts...>::operator=(U&& u) noexcept {
    return (ref<traits::t_remove_ref<U>>() = util::f_cast<U>(u));
  }

  /////////////////////////////////////////////////////////////////////////////

  // VALUESTORE_1_3_1_
  template<typename T, typename... Ts>
  inline
  typename t_valuestore<T, Ts...>::r_value_
      t_valuestore<T, Ts...>::default_construct() noexcept {
    return impl_::default_construct<T>(store_);
  }

  // VALUESTORE_1_3_2_
  template<typename T, typename... Ts>
  template<typename U>
  inline
  U& t_valuestore<T, Ts...>::default_construct() noexcept {
    return impl_::default_construct<U>(store_);
  }

  /////////////////////////////////////////////////////////////////////////////

  // VALUESTORE_1_4_1_
  template<typename T, typename... Ts>
  inline
  typename t_valuestore<T, Ts...>::r_value_
      t_valuestore<T, Ts...>::copy_construct(R_value_ value) noexcept {
    return impl_::copy_construct<T>(store_, value);
  }

  // VALUESTORE_1_4_2_
  template<typename T, typename... Ts>
  template<typename U>
  inline
  U& t_valuestore<T, Ts...>::copy_construct(const U& u) noexcept {
    return impl_::copy_construct<U>(store_, u);
  }

  /////////////////////////////////////////////////////////////////////////////

  // VALUESTORE_1_5_1_
  template<typename T, typename... Ts>
  inline
  typename t_valuestore<T, Ts...>::r_value_
      t_valuestore<T, Ts...>::move_construct(x_value_ value) noexcept {
    return impl_::move_construct<T>(store_, util::x_cast(value));
  }

  // VALUESTORE_1_5_2_
  template<typename T, typename... Ts>
  template<typename U>
  inline
  U& t_valuestore<T, Ts...>::move_construct(U&& value) noexcept {
    return impl_::move_construct<U>(store_, util::x_cast(value));
  }

  /////////////////////////////////////////////////////////////////////////////

  // VALUESTORE_1_6_1_
  template<typename T, typename... Ts>
  template<typename... As>
  inline
  typename t_valuestore<T, Ts...>::r_value_
      t_valuestore<T, Ts...>::fargs_construct(As&&... as) noexcept {
    return impl_::fargs_construct<T>(store_, util::f_cast<As>(as)...);
  }

  // VALUESTORE_1_6_2_
  template<typename T, typename... Ts>
  template<typename U, typename... As>
  inline
  U& t_valuestore<T, Ts...>::fargs_construct(As&&... as) noexcept {
    return impl_::fargs_construct<U>(store_, util::f_cast<As>(as)...);
  }

  /////////////////////////////////////////////////////////////////////////////

  // VALUESTORE_1_7_1_
  template<typename T, typename... Ts>
  inline
  typename t_valuestore<T, Ts...>::r_valuestore_
       t_valuestore<T, Ts...>::destruct() noexcept {
    impl_::destruct<T>(store_);
    return *this;
  }

  // VALUESTORE_1_7_2_
  template<typename T, typename... Ts>
  template<typename U>
  inline
  typename t_valuestore<T, Ts...>::r_valuestore_
       t_valuestore<T, Ts...>::destruct() noexcept {
    impl_::destruct<U>(store_);
    return *this;
  }

  /////////////////////////////////////////////////////////////////////////////

  // VALUESTORE_1_8_1_
  template<typename T, typename... Ts>
  typename t_valuestore<T, Ts...>::r_value_
      t_valuestore<T, Ts...>::swap(r_value_ value) noexcept {
    return impl_::swap<T>(store_, value);
  }

  // VALUESTORE_1_8_2_
  template<typename T, typename... Ts>
  template<typename U, impl_::t_if_other_<U, T, Ts...>>
  U& t_valuestore<T, Ts...>::swap(U& value) noexcept {
    return impl_::swap<U>(store_, value);
  }

  /////////////////////////////////////////////////////////////////////////////

  // VALUESTORE_1_9_1_
  template<typename T, typename... Ts>
  inline
  typename t_valuestore<T, Ts...>::p_value_
      t_valuestore<T, Ts...>::ptr() noexcept {
    return impl_::ptr<T>(store_);
  }

  // VALUESTORE_1_9_2_
  template<typename T, typename... Ts>
  inline
  typename t_valuestore<T, Ts...>::P_value_
      t_valuestore<T, Ts...>::ptr() const noexcept {
    return impl_::Ptr<T>(store_);
  }

  // VALUESTORE_1_9_3_
  template<typename T, typename... Ts>
  inline
  typename t_valuestore<T, Ts...>::P_value_
      t_valuestore<T, Ts...>::Ptr() const noexcept {
    return impl_::Ptr<T>(store_);
  }

  // VALUESTORE_1_9_4_
  template<typename T, typename... Ts>
  template<typename U>
  inline
  U* t_valuestore<T, Ts...>::ptr() noexcept {
    return impl_::ptr<U>(store_);
  }

  // VALUESTORE_1_9_5_
  template<typename T, typename... Ts>
  template<typename U>
  inline
  const U* t_valuestore<T, Ts...>::ptr() const noexcept {
    return impl_::Ptr<U>(store_);
  }

  // VALUESTORE_1_9_6_
  template<typename T, typename... Ts>
  template<typename U>
  inline
  const U* t_valuestore<T, Ts...>::Ptr() const noexcept {
    return impl_::Ptr<U>(store_);
  }

  /////////////////////////////////////////////////////////////////////////////

  // VALUESTORE_1_10_1_
  template<typename T, typename... Ts>
  inline
  typename t_valuestore<T, Ts...>::r_value_
      t_valuestore<T, Ts...>::ref() noexcept {
    return impl_::ref<T>(store_);
  }

  // VALUESTORE_1_10_2_
  template<typename T, typename... Ts>
  inline
  typename t_valuestore<T, Ts...>::R_value_
      t_valuestore<T, Ts...>::ref() const noexcept {
    return impl_::Ref<T>(store_);
  }

  // VALUESTORE_1_10_3_
  template<typename T, typename... Ts>
  inline
  typename t_valuestore<T, Ts...>::R_value_
      t_valuestore<T, Ts...>::Ref() const noexcept {
    return impl_::Ref<T>(store_);
  }

  // VALUESTORE_1_10_4_
  template<typename T, typename... Ts>
  template<typename U>
  inline
  U& t_valuestore<T, Ts...>::ref() noexcept {
    return impl_::ref<U>(store_);
  }

  // VALUESTORE_1_10_5_
  template<typename T, typename... Ts>
  template<typename U>
  inline
  const U& t_valuestore<T, Ts...>::ref() const noexcept {
    return impl_::ref<U>(store_);
  }

  // VALUESTORE_1_10_6_
  template<typename T, typename... Ts>
  template<typename U>
  inline
  const U& t_valuestore<T, Ts...>::Ref() const noexcept {
    return impl_::Ref<U>(store_);
  }

  /////////////////////////////////////////////////////////////////////////////

  // VALUESTORE_1_11_1_
  template<typename T, typename... Ts>
  inline
  typename t_valuestore<T, Ts...>::r_store_
      t_valuestore<T, Ts...>::store() noexcept {
    return store_;
  }

  // VALUESTORE_1_11_2_
  template<typename T, typename... Ts>
  inline
  typename t_valuestore<T, Ts...>::R_store_
      t_valuestore<T, Ts...>::store() const noexcept {
    return store_;
  }

  // VALUESTORE_1_11_3_
  template<typename T, typename... Ts>
  inline
  typename t_valuestore<T, Ts...>::R_store_
      t_valuestore<T, Ts...>::Store() const noexcept {
    return store_;
  }

  /////////////////////////////////////////////////////////////////////////////

  // DEFAULT_CONSTRUCT_1_
  template<typename T, typename... Ts>
  inline
  T& default_construct(t_valuestore<T, Ts...>& store) noexcept {
    return store.default_construct();
  }

  // DEFAULT_CONSTRUCT_2_
  template<typename U, typename T, typename... Ts>
  inline
  T& default_construct(t_valuestore<T, Ts...>& store) noexcept {
    return store.template default_construct<U>();
  }

  /////////////////////////////////////////////////////////////////////////////

  // COPY_CONSTRUCT_1_
  template<typename T, typename... Ts>
  inline
  T& copy_construct(t_valuestore<T, Ts...>& store, const T& value) noexcept {
    return store.copy_construct(value);
  }

  // COPY_CONSTRUCT_2_
  template<typename U, typename T, typename... Ts>
  inline
  T& copy_construct(t_valuestore<T, Ts...>& store, const T& value) noexcept {
    return store.template copy_construct<U>(value);
  }

  /////////////////////////////////////////////////////////////////////////////

  // MOVE_CONSTRUCT_1_
  template<typename T, typename... Ts>
  inline
  T& move_construct(t_valuestore<T, Ts...>& store, T&& value) noexcept {
    return store.move_construct(util::x_cast(value));
  }

  // MOVE_CONSTRUCT_2_
  template<typename U, typename T, typename... Ts>
  inline
  T& move_construct(t_valuestore<T, Ts...>& store, T&& value) noexcept {
    return store.template move_construct<U>(util::x_cast(value));
  }

  /////////////////////////////////////////////////////////////////////////////

  // FARGS_CONSTRUCT_1_
  template<typename T, typename... Ts, typename... As>
  inline
  T& fargs_construct(t_valuestore<T, Ts...>& store, As&&... as) noexcept {
    return store.fargs_construct(util::f_cast<As>(as)...);
  }

  // FARGS_CONSTRUCT_2_
  template<typename U, typename T, typename... Ts, typename... As>
  inline
  T& fargs_construct(t_valuestore<T, Ts...>& store, As&&... as) noexcept {
    return store.template fargs_construct<U>(util::f_cast<As>(as)...);
  }

  /////////////////////////////////////////////////////////////////////////////

  // DESTRUCT_1_
  template<typename T, typename... Ts>
  inline
  t_valuestore<T, Ts...>& destruct(t_valuestore<T, Ts...>& store) noexcept {
    return store.destruct();
  }

  // DESTRUCT_2_
  template<typename U, typename T, typename... Ts>
  inline
  t_valuestore<T, Ts...>& destruct(t_valuestore<T, Ts...>& store) noexcept {
    return store.template destruct<U>();
  }

  /////////////////////////////////////////////////////////////////////////////

  // SWAP_1_
  template<typename T, typename... Ts>
  inline
  T& swap(t_valuestore<T, Ts...>& store, T& value) noexcept {
    return store.swap(value);
  }

  // SWAP_2_
  template<typename U, typename T, typename... Ts>
  inline
  U& swap(t_valuestore<T, Ts...>& store, U& value) noexcept {
    return store.swap(util::f_cast<U>(value));
  }

  /////////////////////////////////////////////////////////////////////////////

  // PTR_1_
  template<typename T, typename... Ts>
  inline
  T* ptr(t_valuestore<T, Ts...>& store) noexcept {
    return store.ptr();
  }

  // PTR_2_
  template<typename T, typename... Ts>
  inline
  T* ptr(const t_valuestore<T, Ts...>& store) noexcept {
    return store.Ptr();
  }

  // PTR_3_
  template<typename T, typename... Ts>
  inline
  T* Ptr(const t_valuestore<T, Ts...>& store) noexcept {
    return store.Ptr();
  }

  // PTR_4_
  template<typename U, typename T, typename... Ts>
  inline
  U* ptr(t_valuestore<T, Ts...>& store) noexcept {
    return store.template ptr<U>();
  }

  // PTR_5_
  template<typename U, typename T, typename... Ts>
  inline
  const U* ptr(const t_valuestore<T, Ts...>& store) noexcept {
    return store.template Ptr<U>();
  }

  // PTR_6_
  template<typename U, typename T, typename... Ts>
  inline
  const U* Ptr(const t_valuestore<T, Ts...>& store) noexcept {
    return store.template Ptr<U>();
  }

  /////////////////////////////////////////////////////////////////////////////

  // REF_1_
  template<typename T, typename... Ts>
  inline
  T& ref(t_valuestore<T, Ts...>& store) noexcept {
    return store.ref();
  }

  // REF_2_
  template<typename T, typename... Ts>
  inline
  T& ref(const t_valuestore<T, Ts...>& store) noexcept {
    return store.ref();
  }

  // REF_3_
  template<typename T, typename... Ts>
  inline
  T& Ref(const t_valuestore<T, Ts...>& store) noexcept {
    return store.Ref();
  }

  // REF_4_
  template<typename U, typename T, typename... Ts>
  inline
  U& ref(t_valuestore<T, Ts...>& store) noexcept {
    return store.template ref<U>();
  }

  // REF_5_
  template<typename U, typename T, typename... Ts>
  inline
  const U& ref(const t_valuestore<T, Ts...>& store) noexcept {
    return store.template ref<U>();
  }

  // REF_6_
  template<typename U, typename T, typename... Ts>
  inline
  const U& Ref(const t_valuestore<T, Ts...>& store) noexcept {
    return store.template Ref<U>();
  }

  /////////////////////////////////////////////////////////////////////////////

  // STORE_1_
  template<typename T, typename... Ts>
  inline
  t_store<T>& store(t_valuestore<T, Ts...>& store) noexcept {
    return store.store();
  }

  // STORE_2_
  template<typename T, typename... Ts>
  inline
  t_store<T>& store(const t_valuestore<T, Ts...>& store) noexcept {
    return store.store();
  }

  // STORE_3_
  template<typename T, typename... Ts>
  inline
  t_store<T>& Store(const t_valuestore<T, Ts...>& store) noexcept {
    return store.Store();
  }

  /////////////////////////////////////////////////////////////////////////////
}
}

#endif
