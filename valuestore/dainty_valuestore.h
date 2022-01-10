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

  template<typename T>
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

    constexpr static auto SIZEOF    = t_store_::t_traits::SIZEOF;
    constexpr static auto ALIGNMENT = t_store_::t_traits::ALIGNMENT;

    constexpr
    t_valuestore() noexcept     = default;
    t_valuestore(R_valuestore_) = delete;
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

  template<typename T>
  inline
  t_valuestore<T>::t_valuestore(R_value_ value) noexcept {
    impl_::copy_construct<T>(store_, value);
  }

  template<typename T>
  inline
  t_valuestore<T>::t_valuestore(x_value_ value) noexcept {
    impl_::move_construct<T>(store_, util::x_cast(value));
  }

  template<typename T>
  template<typename... As>
  inline
  t_valuestore<T>::t_valuestore(t_fargs, As&&... as) noexcept {
    impl_::fargs_construct<T>(store_, util::f_cast<As>(as)...);
  }

  template<typename T>
  inline
  typename t_valuestore<T>::r_value_
      t_valuestore<T>::operator=(R_value_ value) noexcept {
    return (ref() = value);
  }

  template<typename T>
  inline
  typename t_valuestore<T>::r_value_
      t_valuestore<T>::operator=(x_value_ value) noexcept {
    return (ref() = util::x_cast(value));
  }

  template<typename T>
  inline
  typename t_valuestore<T>::r_value_
      t_valuestore<T>::default_construct() noexcept {
    return impl_::default_construct<T>(store_);
  }

  template<typename T>
  inline
  typename t_valuestore<T>::r_value_
      t_valuestore<T>::copy_construct(R_value_ value) noexcept {
    return impl_::copy_construct<T>(store_, value);
  }

  template<typename T>
  inline
  typename t_valuestore<T>::r_value_
      t_valuestore<T>::move_construct(x_value_ value) noexcept {
    return impl_::move_construct<T>(store_, util::x_cast(value));
  }

  template<typename T>
  template<typename... As>
  inline
  typename t_valuestore<T>::r_value_
      t_valuestore<T>::fargs_construct(As&&... as) noexcept {
    return impl_::fargs_construct<T>(store_, util::f_cast<As>(as)...);
  }

  template<typename T>
  inline
  typename t_valuestore<T>::r_valuestore_ t_valuestore<T>::destruct() noexcept {
    impl_::destruct<T>(store_);
    return *this;
  }

  template<typename T>
  constexpr
  typename t_valuestore<T>::p_value_ t_valuestore<T>::ptr() noexcept {
    return impl_::ptr<T>(store_);
  }

  template<typename T>
  constexpr
  typename t_valuestore<T>::P_value_ t_valuestore<T>::ptr() const noexcept {
    return impl_::Ptr<T>(store_);
  }

  template<typename T>
  constexpr
  typename t_valuestore<T>::P_value_ t_valuestore<T>::Ptr() const noexcept {
    return impl_::Ptr<T>(store_);
  }

  template<typename T>
  constexpr
  typename t_valuestore<T>::r_value_ t_valuestore<T>::ref() noexcept {
    return impl_::ref<T>(store_);
  }

  template<typename T>
  constexpr
  typename t_valuestore<T>::R_value_ t_valuestore<T>::ref() const noexcept {
    return impl_::Ref<T>(store_);
  }

  template<typename T>
  constexpr
  typename t_valuestore<T>::R_value_ t_valuestore<T>::Ref() const noexcept {
    return impl_::Ref<T>(store_);
  }

  template<typename T>
  constexpr
  typename t_valuestore<T>::r_store_ t_valuestore<T>::store() noexcept {
    return store_;
  }

  template<typename T>
  constexpr
  typename t_valuestore<T>::R_store_ t_valuestore<T>::store() const noexcept {
    return store_;
  }

  template<typename T>
  constexpr
  typename t_valuestore<T>::R_store_ t_valuestore<T>::Store() const noexcept {
    return store_;
  }

  /////////////////////////////////////////////////////////////////////////////
}
}

#endif
