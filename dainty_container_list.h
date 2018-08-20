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

#ifndef _DAINTY_CONTAINER_LIST_H_
#define _DAINTY_CONTAINER_LIST_H_

#include "dainty_named.h"
#include "dainty_container_list_impl.h"

namespace dainty
{
namespace container
{
namespace list
{
  using named::t_n;
  using named::t_ix;
  using named::t_validity;
  using named::VALID;
  using named::INVALID;

///////////////////////////////////////////////////////////////////////////////

  template<typename T, t_n_ N = 0>
  class t_list final {
    using t_impl_ = t_list_impl_<T>;
  public:
    using t_value = typename t_impl_::t_value;
    using p_value = typename t_impl_::p_value;
    using P_value = typename t_impl_::P_value;
    using r_value = typename t_impl_::r_value;
    using R_value = typename t_impl_::R_value;
    using x_value = typename t_impl_::x_value;

     t_list();
    ~t_list();

    t_list(const t_list&)            = delete;
    t_list(t_list&&)                 = delete;
    t_list& operator=(const t_list&) = delete;
    t_list& operator=(t_list&&)      = delete;

    operator t_validity() const;

    p_value push_back();
    p_value push_back(t_err);
    p_value push_back(       R_value);
    p_value push_back(t_err, R_value);
    p_value push_back(       x_value);
    p_value push_back(t_err, x_value);

    p_value insert(       t_ix);
    p_value insert(t_err, t_ix);
    p_value insert(       t_ix, R_value);
    p_value insert(t_err, t_ix, R_value);
    p_value insert(       t_ix, x_value);
    p_value insert(t_err, t_ix, x_value);

    t_bool pop_back();
    t_bool pop_back(t_err);

    t_bool erase(       t_ix);
    t_bool erase(t_err, t_ix);

    t_void clear();
    t_void clear(t_err);

    t_bool is_full () const;
    t_bool is_empty() const;
    t_n    get_size() const;
    t_n    get_capacity() const;

    p_value  get(       t_ix);
    p_value  get(t_err, t_ix);
    P_value  get(       t_ix) const;
    P_value  get(t_err, t_ix) const;
    P_value cget(       t_ix) const;
    P_value cget(t_err, t_ix) const;

    template<typename F> t_void  each(       F);
    template<typename F> t_void  each(t_err, F);
    template<typename F> t_void  each(       F) const;
    template<typename F> t_void  each(t_err, F) const;
    template<typename F> t_void ceach(       F) const;
    template<typename F> t_void ceach(t_err, F) const;

  private:
    typename t_impl_::t_entry store_[N];
    t_impl_                   impl_;
  };

///////////////////////////////////////////////////////////////////////////////

  template<typename T>
  class t_list<T, 0> final {
    using t_impl_ = t_list_impl_<T>;
  public:
    using t_value = typename t_impl_::t_value;
    using p_value = typename t_impl_::p_value;
    using P_value = typename t_impl_::P_value;
    using r_value = typename t_impl_::r_value;
    using R_value = typename t_impl_::R_value;
    using x_value = typename t_impl_::x_value;

     t_list(       t_n max);
     t_list(t_err, t_n max);
    ~t_list();

    t_list(const t_list&)            = delete;
    t_list(t_list&&)                 = delete;
    t_list& operator=(const t_list&) = delete;
    t_list& operator=(t_list&&)      = delete;

    operator t_validity() const;

    p_value push_back();
    p_value push_back(t_err);
    p_value push_back(       R_value);
    p_value push_back(t_err, R_value);
    p_value push_back(       x_value);
    p_value push_back(t_err, x_value);

    p_value insert(       t_ix);
    p_value insert(t_err, t_ix);
    p_value insert(       t_ix, R_value);
    p_value insert(t_err, t_ix, R_value);
    p_value insert(       t_ix, x_value);
    p_value insert(t_err, t_ix, x_value);

    t_bool pop_back();
    t_bool pop_back(t_err);

    t_bool erase(       t_ix);
    t_bool erase(t_err, t_ix);

    t_void clear();
    t_void clear(t_err);

    t_bool is_full () const;
    t_bool is_empty() const;
    t_n    get_size() const;
    t_n    get_capacity() const;

    p_value  get(       t_ix);
    p_value  get(t_err, t_ix);
    P_value  get(       t_ix) const;
    P_value  get(t_err, t_ix) const;
    P_value cget(       t_ix) const;
    P_value cget(t_err, t_ix) const;

    template<typename F> t_void  each(       F);
    template<typename F> t_void  each(t_err, F);
    template<typename F> t_void  each(       F) const;
    template<typename F> t_void  each(t_err, F) const;
    template<typename F> t_void ceach(       F) const;
    template<typename F> t_void ceach(t_err, F) const;

  private:
    t_n_                      max_;
    typename t_impl_::p_store store_;
    t_impl_                   impl_;
  };

///////////////////////////////////////////////////////////////////////////////

  template<typename T, t_n_ N>
  inline
  t_list<T, N>::t_list() {
  }

  template<typename T, t_n_ N>
  inline
  t_list<T, N>::~t_list() {
    impl_.clear(store_);
  }

  template<typename T, t_n_ N>
  inline
  t_list<T, N>::operator t_validity() const {
    return VALID;
  }

  template<typename T, t_n_ N>
  inline
  typename t_list<T, N>::p_value t_list<T, N>::push_back() {
    return impl_.push_back(store_, N);
  }

  template<typename T, t_n_ N>
  inline
  typename t_list<T, N>::p_value t_list<T, N>::push_back(t_err err) {
    return impl_.push_back(err, store_, N);
  }

  template<typename T, t_n_ N>
  inline
  typename t_list<T, N>::p_value t_list<T, N>::push_back(R_value value) {
    return impl_.push_back(store_, N, value);
  }

  template<typename T, t_n_ N>
  inline
  typename t_list<T, N>::p_value t_list<T, N>::push_back(t_err err,
                                                         R_value value) {
    return impl_.push_back(err, store_, N, value);
  }

  template<typename T, t_n_ N>
  inline
  typename t_list<T, N>::p_value t_list<T, N>::push_back(x_value value) {
    return impl_.push_back(store_, N, std::move(value));
  }

  template<typename T, t_n_ N>
  inline
  typename t_list<T, N>::p_value t_list<T, N>::push_back(t_err err,
                                                         x_value value) {
    return impl_.push_back(err, store_, N, std::move(value));
  }

  template<typename T, t_n_ N>
  inline
  typename t_list<T, N>::p_value t_list<T, N>::insert(t_ix ix) {
    return impl_.insert(store_, N, named::get(ix));
  }

  template<typename T, t_n_ N>
  inline
  typename t_list<T, N>::p_value t_list<T, N>::insert(t_err err, t_ix ix) {
    return impl_.insert(err, store_, N, named::get(ix));
  }

  template<typename T, t_n_ N>
  inline
  typename t_list<T, N>::p_value t_list<T, N>::insert(t_ix ix, R_value value) {
    return impl_.insert(store_, N, named::get(ix), value);
  }

  template<typename T, t_n_ N>
  inline
  typename t_list<T, N>::p_value t_list<T, N>::insert(t_err err, t_ix ix,
                                                      R_value value) {
    return impl_.insert(err, store_, N, named::get(ix), value);
  }

  template<typename T, t_n_ N>
  inline
  typename t_list<T, N>::p_value t_list<T, N>::insert(t_ix ix, x_value value) {
    return impl_.insert(store_, N, named::get(ix), std::move(value));
  }

  template<typename T, t_n_ N>
  inline
  typename t_list<T, N>::p_value t_list<T, N>::insert(t_err err, t_ix ix,
                                                      x_value value) {
    return impl_.insert(err, store_, N, named::get(ix), std::move(value));
  }

  template<typename T, t_n_ N>
  inline
  t_bool t_list<T, N>::pop_back() {
    return impl_.pop_back(store_);
  }

  template<typename T, t_n_ N>
  inline
  t_bool t_list<T, N>::pop_back(t_err err) {
    return impl_.pop_back(err, store_);
  }

  template<typename T, t_n_ N>
  inline
  t_bool t_list<T, N>::erase(t_ix ix) {
    return impl_.erase(store_, named::get(ix));
  }

  template<typename T, t_n_ N>
  inline
  t_bool t_list<T, N>::erase(t_err err, t_ix ix) {
    return impl_.erase(err, store_, named::get(ix));
  }

  template<typename T, t_n_ N>
  inline
  t_void t_list<T, N>::clear() {
    impl_.clear(store_);
  }

  template<typename T, t_n_ N>
  inline
  t_void t_list<T, N>::clear(t_err err) {
    impl_.clear(err, store_);
  }

  template<typename T, t_n_ N>
  inline
  t_bool t_list<T, N>::is_full() const {
    return impl_.is_full(N);
  }

  template<typename T, t_n_ N>
  inline
  t_bool t_list<T, N>::is_empty() const {
    return impl_.is_empty();
  }

  template<typename T, t_n_ N>
  inline
  t_n t_list<T, N>::get_size() const {
    return t_n{impl_.get_size()};
  }

  template<typename T, t_n_ N>
  inline
  t_n t_list<T, N>::get_capacity() const {
    return t_n{N};
  }

  template<typename T, t_n_ N>
  inline
  typename t_list<T, N>::p_value t_list<T, N>::get(t_ix ix) {
    return impl_.get(store_, named::get(ix));
  }

  template<typename T, t_n_ N>
  inline
  typename t_list<T, N>::p_value t_list<T, N>::get(t_err err, t_ix ix) {
    return impl_.get(err, store_, named::get(ix));
  }

  template<typename T, t_n_ N>
  inline
  typename t_list<T, N>::P_value t_list<T, N>::get(t_ix ix) const {
    return impl_.get(store_, named::get(ix));
  }

  template<typename T, t_n_ N>
  inline
  typename t_list<T, N>::P_value t_list<T, N>::get(t_err err, t_ix ix) const {
    return impl_.get(err, store_, named::get(ix));
  }

  template<typename T, t_n_ N>
  inline
  typename t_list<T, N>::P_value t_list<T, N>::cget(t_ix ix) const {
    return impl_.get(store_, named::get(ix));
  }

  template<typename T, t_n_ N>
  inline
  typename t_list<T, N>::P_value t_list<T, N>::cget(t_err err, t_ix ix) const {
    return impl_.get(err, store_, named::get(ix));
  }

  template<typename T, t_n_ N>
  template<typename F>
  inline
  t_void t_list<T, N>::each(F f) {
    impl_.each(store_, f);
  }

  template<typename T, t_n_ N>
  template<typename F>
  inline
  t_void t_list<T, N>::each(t_err err, F f) {
    impl_.each(err, store_, f);
  }

  template<typename T, t_n_ N>
  template<typename F>
  inline
  t_void t_list<T, N>::each(F f) const {
    impl_.each(store_, f);
  }

  template<typename T, t_n_ N>
  template<typename F>
  inline
  t_void t_list<T, N>::each(t_err err, F f) const {
    impl_.each(err, store_, f);
  }

  template<typename T, t_n_ N>
  template<typename F>
  inline
  t_void t_list<T, N>::ceach(F f) const {
    impl_.each(store_, f);
  }

  template<typename T, t_n_ N>
  template<typename F>
  inline
  t_void t_list<T, N>::ceach(t_err err, F f) const {
    impl_.each(err, store_, f);
  }

///////////////////////////////////////////////////////////////////////////////

  template<typename T>
  inline
  t_list<T, 0>::t_list(t_n max)
    : max_  {named::get(max)},
      store_{new typename t_impl_::t_entry[max_]} {
  }

  template<typename T>
  inline
  t_list<T, 0>::t_list(t_err err, t_n max)
    : max_ {named::get(max)},
     store_{!err ? new typename t_impl_::t_entry[max_] : nullptr} {
  }

  template<typename T>
  inline
  t_list<T, 0>::~t_list() {
    if (store_) {
      impl_.clear(store_);
      delete [] store_;
    }
  }

  template<typename T>
  inline
  t_list<T, 0>::operator t_validity() const {
    return store_ ? VALID : INVALID;
  }

  template<typename T>
  inline
  typename t_list<T, 0>::p_value t_list<T, 0>::push_back() {
    return impl_.push_back(store_, max_);
  }

  template<typename T>
  inline
  typename t_list<T, 0>::p_value t_list<T, 0>::push_back(t_err err) {
    return impl_.push_back(err, store_, max_);
  }

  template<typename T>
  inline
  typename t_list<T, 0>::p_value t_list<T, 0>::push_back(R_value value) {
    return impl_.push_back(store_, max_, value);
  }

  template<typename T>
  inline
  typename t_list<T, 0>::p_value t_list<T, 0>::push_back(t_err err,
                                                         R_value value) {
    return impl_.push_back(err, store_, max_, value);
  }

  template<typename T>
  inline
  typename t_list<T, 0>::p_value t_list<T, 0>::push_back(x_value value) {
    return impl_.push_back(store_, max_, std::move(value));
  }

  template<typename T>
  inline
  typename t_list<T, 0>::p_value t_list<T, 0>::push_back(t_err err,
                                                         x_value value) {
    return impl_.push_back(err, store_, max_, std::move(value));
  }

  template<typename T>
  inline
  typename t_list<T, 0>::p_value t_list<T, 0>::insert(t_ix ix) {
    return impl_.insert(store_, max_, named::get(ix));
  }

  template<typename T>
  inline
  typename t_list<T, 0>::p_value t_list<T, 0>::insert(t_err err, t_ix ix) {
    return impl_.insert(err, store_, max_, named::get(ix));
  }

  template<typename T>
  inline
  typename t_list<T, 0>::p_value t_list<T, 0>::insert(t_ix ix, R_value value) {
    return impl_.insert(store_, max_, named::get(ix), value);
  }

  template<typename T>
  inline
  typename t_list<T, 0>::p_value t_list<T, 0>::insert(t_err err, t_ix ix,
                                                      R_value value) {
    return impl_.insert(err, store_, max_, named::get(ix), value);
  }

  template<typename T>
  inline
  typename t_list<T, 0>::p_value t_list<T, 0>::insert(t_ix ix, x_value value) {
    return impl_.insert(store_, max_, named::get(ix), std::move(value));
  }

  template<typename T>
  inline
  typename t_list<T, 0>::p_value t_list<T, 0>::insert(t_err err, t_ix ix,
                                                      x_value value) {
    return impl_.insert(err, store_, max_, named::get(ix), std::move(value));
  }

  template<typename T>
  inline
  t_bool t_list<T, 0>::pop_back() {
    return impl_.pop_back(store_);
  }

  template<typename T>
  inline
  t_bool t_list<T, 0>::pop_back(t_err err) {
    return impl_.pop_back(err, store_);
  }

  template<typename T>
  inline
  t_bool t_list<T, 0>::erase(t_ix ix) {
    return impl_.erase(store_, named::get(ix));
  }

  template<typename T>
  inline
  t_bool t_list<T, 0>::erase(t_err err, t_ix ix) {
    return impl_.erase(err, store_, named::get(ix));
  }

  template<typename T>
  inline
  t_void t_list<T, 0>::clear() {
    impl_.clear(store_);
  }

  template<typename T>
  inline
  t_void t_list<T, 0>::clear(t_err err) {
    impl_.clear(err, store_);
  }

  template<typename T>
  inline
  t_bool t_list<T, 0>::is_full() const {
    return impl_.is_full(max_);
  }

  template<typename T>
  inline
  t_bool t_list<T, 0>::is_empty() const {
    return impl_.is_empty();
  }

  template<typename T>
  inline
  t_n t_list<T, 0>::get_size() const {
    return t_n{impl_.get_size()};
  }

  template<typename T>
  inline
  t_n t_list<T, 0>::get_capacity() const {
    return t_n{max_};
  }

  template<typename T>
  inline
  typename t_list<T, 0>::p_value t_list<T, 0>::get(t_ix ix) {
    return impl_.get(store_, max_, named::get(ix));
  }

  template<typename T>
  inline
  typename t_list<T, 0>::p_value t_list<T, 0>::get(t_err err, t_ix ix) {
    return impl_.get(err, store_, max_, named::get(ix));
  }

  template<typename T>
  inline
  typename t_list<T, 0>::P_value t_list<T, 0>::get(t_ix ix) const {
    return impl_.get(store_, max_, named::get(ix));
  }

  template<typename T>
  inline
  typename t_list<T, 0>::P_value t_list<T, 0>::get(t_err err, t_ix ix) const {
    return impl_.get(err, store_, max_, named::get(ix));
  }

  template<typename T>
  inline
  typename t_list<T, 0>::P_value t_list<T, 0>::cget(t_ix ix) const {
    return impl_.get(store_, max_, named::get(ix));
  }

  template<typename T>
  inline
  typename t_list<T, 0>::P_value t_list<T, 0>::cget(t_err err, t_ix ix) const {
    return impl_.get(err, store_, ix, named::get(ix));
  }

  template<typename T>
  template<typename F>
  inline
  t_void t_list<T, 0>::each(F f) {
    impl_.each(store_, f);
  }

  template<typename T>
  template<typename F>
  inline
  t_void t_list<T, 0>::each(t_err err, F f) {
    impl_.each(err, store_, f);
  }

  template<typename T>
  template<typename F>
  inline
  t_void t_list<T, 0>::each(F f) const {
    impl_.each(store_, f);
  }

  template<typename T>
  template<typename F>
  inline
  t_void t_list<T, 0>::each(t_err err, F f) const {
    impl_.each(err, store_, f);
  }

  template<typename T>
  template<typename F>
  inline
  t_void t_list<T, 0>::ceach(F f) const {
    impl_.each(store_, f);
  }

  template<typename T>
  template<typename F>
  inline
  t_void t_list<T, 0>::ceach(t_err err, F f) const {
    impl_.each(err, store_, f);
  }

///////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
