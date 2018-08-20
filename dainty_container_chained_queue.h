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

#ifndef _DAINTY_CONTAINER_CHAINED_QUEUE_H_
#define _DAINTY_CONTAINER_CHAINED_QUEUE_H_

#include "dainty_named.h"
#include "dainty_container_freelist.h"
#include "dainty_container_chained_queue_impl.h"

namespace dainty
{
namespace container
{
namespace chained_queue
{
///////////////////////////////////////////////////////////////////////////////

  template<typename T, t_n_ N = 0>
  class t_chained_queue {
    using t_store_ = freelist::t_freelist<t_item<T>, N>;
    using t_impl_  = t_chained_queue_impl_<T, t_store_>;
  public:
    using t_n     = typename t_impl_::t_n;
    using t_chain = typename t_impl_::t_chain;
    using p_item  = typename t_impl_::p_item;

    t_chained_queue();

    t_chained_queue(const t_chained_queue&)            = delete;
    t_chained_queue(t_chained_queue&&)                 = delete;
    t_chained_queue& operator=(const t_chained_queue&) = delete;
    t_chained_queue& operator=(t_chained_queue&&)      = delete;

    operator t_validity() const;

    t_chain acquire(       t_n = t_n{1});
    t_chain acquire(t_err, t_n = t_n{1});
    t_void  release(       t_chain);
    t_void  release(t_err, t_chain);

    t_void  insert(       t_chain);
    t_void  insert(t_err, t_chain);
    t_chain remove(       t_n = t_n{0});
    t_chain remove(t_err, t_n = t_n{0});

    p_item get_tail();

    t_bool is_empty    () const;
    t_bool is_full     () const;

    t_n    get_capacity() const;
    t_n    get_size    () const;

  private:
    t_store_ store_;
    t_impl_  impl_;
  };

///////////////////////////////////////////////////////////////////////////////

  template<typename T>
  class t_chained_queue<T, 0> final {
    using t_store_ = freelist::t_freelist<t_item<T>, 0>;
    using t_impl_  = t_chained_queue_impl_<T, t_store_>;
  public:
    using t_n     = typename t_impl_::t_n;
    using t_chain = typename t_impl_::t_chain;
    using p_item  = typename t_impl_::p_item;

    t_chained_queue(t_n max);

    t_chained_queue(const t_chained_queue&)            = delete;
    t_chained_queue(t_chained_queue&&)                 = delete;
    t_chained_queue& operator=(const t_chained_queue&) = delete;
    t_chained_queue& operator=(t_chained_queue&&)      = delete;

    operator t_validity() const;

    t_chain acquire(       t_n = t_n{1});
    t_chain acquire(t_err, t_n = t_n{1});
    t_void  release(       t_chain);
    t_void  release(t_err, t_chain);

    t_void  insert(       t_chain);
    t_void  insert(t_err, t_chain);
    t_chain remove(       t_n = t_n{0});
    t_chain remove(t_err, t_n = t_n{0});

    p_item get_tail();

    t_bool is_empty    () const;
    t_bool is_full     () const;

    t_n    get_capacity() const;
    t_n    get_size    () const;

  private:
    t_store_ store_;
    t_impl_  impl_;
  };

///////////////////////////////////////////////////////////////////////////////

  template<typename T, t_n_ N>
  inline
  t_chained_queue<T, N>::t_chained_queue() {
  }

  template<typename T, t_n_ N>
  inline
  t_chained_queue<T, N>::operator t_validity() const {
    return VALID;
  }

  template<typename T, t_n_ N>
  inline
  typename t_chained_queue<T, N>::t_chain
      t_chained_queue<T, N>::acquire(t_n n) {
    return impl_.acquire(store_, n);
  }

  template<typename T, t_n_ N>
  inline
  typename t_chained_queue<T, N>::t_chain
      t_chained_queue<T, N>::acquire(t_err err, t_n n) {
    return impl_.acquire(err, store_, n);
  }

  template<typename T, t_n_ N>
  inline
  t_void t_chained_queue<T, N>::release(t_chain chain) {
    impl_.release(store_, chain);
  }

  template<typename T, t_n_ N>
  inline
  t_void t_chained_queue<T, N>::release(t_err err, t_chain chain) {
    impl_.release(err, store_, chain);
  }

  template<typename T, t_n_ N>
  inline
  t_void t_chained_queue<T, N>::insert(t_chain chain) {
    impl_.insert(store_, chain);
  }

  template<typename T, t_n_ N>
  inline
  t_void t_chained_queue<T, N>::insert(t_err err, t_chain chain) {
    impl_.insert(err, store_, chain);
  }

  template<typename T, t_n_ N>
  inline
  typename t_chained_queue<T, N>::t_chain
      t_chained_queue<T, N>::remove(t_n n) {
    return impl_.remove(store_, n);
  }

  template<typename T, t_n_ N>
  inline
  typename t_chained_queue<T, N>::t_chain
      t_chained_queue<T, N>::remove(t_err err, t_n n) {
    return impl_.remove(err, store_, n);
  }

  template<typename T, t_n_ N>
  inline
  typename t_chained_queue<T, N>::p_item t_chained_queue<T, N>::get_tail() {
    return impl_.get_tail(store_);
  }

  template<typename T, t_n_ N>
  inline
  t_bool t_chained_queue<T, N>::is_empty() const {
    return impl_.is_empty(store_);
  }

  template<typename T, t_n_ N>
  inline
  t_bool t_chained_queue<T, N>::is_full() const {
    return impl_.is_full(store_);
  }

  template<typename T, t_n_ N>
  inline
  typename t_chained_queue<T, N>::t_n
      t_chained_queue<T, N>::get_capacity() const {
    return impl_.get_capacity(store_);
  }

  template<typename T, t_n_ N>
  inline
  typename t_chained_queue<T, N>::t_n
      t_chained_queue<T, N>::get_size() const {
    return impl_.get_size(store_);
  }

///////////////////////////////////////////////////////////////////////////////

  template<typename T>
  inline
  t_chained_queue<T, 0>::t_chained_queue(t_n max) : store_{max} {
  }

  template<typename T>
  inline
  t_chained_queue<T, 0>::operator t_validity() const {
    return store_;
  }

  template<typename T>
  inline
  typename t_chained_queue<T, 0>::t_chain
      t_chained_queue<T, 0>::acquire(t_n n) {
    return impl_.acquire(store_, n);
  }

  template<typename T>
  inline
  typename t_chained_queue<T, 0>::t_chain
      t_chained_queue<T, 0>::acquire(t_err err, t_n n) {
    return impl_.acquire(err, store_, n);
  }

  template<typename T>
  inline
  t_void t_chained_queue<T, 0>::release(t_chain chain) {
    impl_.release(store_, chain);
  }

  template<typename T>
  inline
  t_void t_chained_queue<T, 0>::release(t_err err, t_chain chain) {
    impl_.release(err, store_, chain);
  }

  template<typename T>
  inline
  t_void t_chained_queue<T, 0>::insert(t_chain chain) {
    impl_.insert(store_, chain);
  }

  template<typename T>
  inline
  t_void t_chained_queue<T, 0>::insert(t_err err, t_chain chain) {
    impl_.insert(err, store_, chain);
  }

  template<typename T>
  inline
  typename t_chained_queue<T, 0>::t_chain
      t_chained_queue<T, 0>::remove(t_n n) {
    return impl_.remove(store_, n);
  }

  template<typename T>
  inline
  typename t_chained_queue<T, 0>::t_chain
      t_chained_queue<T, 0>::remove(t_err err, t_n n) {
    return impl_.remove(err, store_, n);
  }

  template<typename T>
  inline
  typename t_chained_queue<T, 0>::p_item t_chained_queue<T, 0>::get_tail() {
    return impl_.get_tail(store_);
  }

  template<typename T>
  inline
  t_bool t_chained_queue<T, 0>::is_empty() const {
    return impl_.is_empty(store_);
  }

  template<typename T>
  inline
  t_bool t_chained_queue<T, 0>::is_full() const {
    return impl_.is_full(store_);
  }

  template<typename T>
  inline
  typename t_chained_queue<T, 0>::t_n
      t_chained_queue<T, 0>::get_capacity() const {
    return impl_.get_capacity(store_);
  }

  template<typename T>
  inline
  typename t_chained_queue<T, 0>::t_n
      t_chained_queue<T, 0>::get_size() const {
    return impl_.get_size(store_);
  }

///////////////////////////////////////////////////////////////////////////////
}
}
}

#endif

