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

#ifndef _DAINTY_CONTAINER_H_
#define _DAINTY_CONTAINER_H_

#include "dainty_named.h"
#include "dainty_container_list.h"

namespace dainty
{
namespace container
{
namespace pushlist
{
  using namespace dainty::container::list;

///////////////////////////////////////////////////////////////////////////////

  template<typename T, t_n_ N = 0>
  class t_pushlist {
    using t_impl_ = t_list<T, N>;
  public:
    using t_value = typename t_impl_::t_value;
    using p_value = typename t_impl_::p_value;
    using P_value = typename t_impl_::P_value;
    using R_value = typename t_impl_::R_value;
    using x_value = typename t_impl_::x_value;

    t_pushlist();

    t_pushlist(const t_pushlist&) = delete;
    t_pushlist(t_pushlist&&) = delete;
    t_pushlist& operator=(const t_pushlist&) = delete;
    t_pushlist& operator=(t_pushlist&&) = delete;

    operator t_validity() const;

    p_value push_back();
    p_value push_back(t_err);
    p_value push_back(       R_value);
    p_value push_back(t_err, R_value);
    p_value push_back(       x_value);
    p_value push_back(t_err, x_value);

    t_bool is_full      () const;
    t_bool is_empty     () const;
    t_n    get_size     () const;
    t_n    get_capacity () const;

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
    t_impl_ impl_;
  };

///////////////////////////////////////////////////////////////////////////////

  template<typename T>
  class t_pushlist<T, 0> {
    using t_impl_ = t_list<T, 0>;
  public:
    using t_value = typename t_impl_::t_value;
    using p_value = typename t_impl_::p_value;
    using P_value = typename t_impl_::P_value;
    using R_value = typename t_impl_::R_value;
    using x_value = typename t_impl_::x_value;

    t_pushlist(       t_n max);
    t_pushlist(t_err, t_n max);

    t_pushlist(const t_pushlist&) = delete;
    t_pushlist(t_pushlist&&) = delete;
    t_pushlist& operator=(const t_pushlist&) = delete;
    t_pushlist& operator=(t_pushlist&&) = delete;

    operator t_validity() const;

    p_value push_back();
    p_value push_back(t_err);
    p_value push_back(       R_value);
    p_value push_back(t_err, R_value);
    p_value push_back(       x_value);
    p_value push_back(t_err, x_value);

    t_bool is_full      () const;
    t_bool is_empty     () const;
    t_n    get_size     () const;
    t_n    get_capacity () const;

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
    t_impl_ impl_;
  };

///////////////////////////////////////////////////////////////////////////////

  template<typename T, t_n_ N>
  inline
  t_pushlist<T, N>::t_pushlist() {
  }

  template<typename T, t_n_ N>
  inline
  t_pushlist<T, N>::operator t_validity() const {
    return impl_.operator t_validity();
  }

  template<typename T, t_n_ N>
  inline
  typename t_pushlist<T, N>::p_value t_pushlist<T, N>::push_back() {
    return impl_.push_back();
  }

  template<typename T, t_n_ N>
  inline
  typename t_pushlist<T, N>::p_value t_pushlist<T, N>::push_back(t_err err) {
    return impl_.push_back(err);
  }

  template<typename T, t_n_ N>
  inline
  typename t_pushlist<T, N>::p_value
    t_pushlist<T, N>::push_back(R_value value) {
    return impl_.push_back(value);
  }

  template<typename T, t_n_ N>
  inline
  typename t_pushlist<T, N>::p_value
    t_pushlist<T, N>::push_back(t_err err, R_value value) {
    return impl_.push_back(err, value);
  }

  template<typename T, t_n_ N>
  inline
  typename t_pushlist<T, N>::p_value
    t_pushlist<T, N>::push_back(x_value value) {
    return impl_.push_back(std::move(value));
  }

  template<typename T, t_n_ N>
  inline
  typename t_pushlist<T, N>::p_value
    t_pushlist<T, N>::push_back(t_err err, x_value value) {
    return impl_.push_back(err, std::move(value));
  }

  template<typename T, t_n_ N>
  inline
  t_bool t_pushlist<T, N>::is_full() const {
    return impl_.is_full();
  }

  template<typename T, t_n_ N>
  inline
  t_bool t_pushlist<T, N>::is_empty() const {
    return impl_.is_empty();
  }

  template<typename T, t_n_ N>
  inline
  t_n t_pushlist<T, N>::get_size() const {
    return impl_.get_size();
  }

  template<typename T, t_n_ N>
  inline
  t_n t_pushlist<T, N>::get_capacity() const {
    return impl_.get_capacity();
  }

  template<typename T, t_n_ N>
  inline
  typename t_pushlist<T, N>::p_value t_pushlist<T, N>::get(t_ix ix) {
    return impl_.get(ix);
  }

  template<typename T, t_n_ N>
  inline
  typename t_pushlist<T, N>::p_value t_pushlist<T, N>::get(t_err err,
                                                           t_ix ix) {
    return impl_.get(err, ix);
  }

  template<typename T, t_n_ N>
  inline
  typename t_pushlist<T, N>::P_value t_pushlist<T, N>::get(t_ix ix) const {
    return impl_.get(ix);
  }

  template<typename T, t_n_ N>
  inline
  typename t_pushlist<T, N>::P_value t_pushlist<T, N>::get(t_err err,
                                                            t_ix ix) const {
    return impl_.get(err, ix);
  }

  template<typename T, t_n_ N>
  inline
  typename t_pushlist<T, N>::P_value t_pushlist<T, N>::cget(t_ix ix) const {
    return impl_.cget(ix);
  }

  template<typename T, t_n_ N>
  inline
  typename t_pushlist<T, N>::P_value t_pushlist<T, N>::cget(t_err err,
                                                             t_ix ix) const {
    return impl_.cget(err, ix);
  }

  template<typename T, t_n_ N>
  template<typename F>
  inline
  t_void t_pushlist<T, N>::each(F f) {
    impl_.each(f);
  }

  template<typename T, t_n_ N>
  template<typename F>
  inline
  t_void t_pushlist<T, N>::each(t_err err, F f) {
    impl_.each(err, f);
  }

  template<typename T, t_n_ N>
  template<typename F>
  inline
  t_void t_pushlist<T, N>::each(F f) const {
    impl_.each(f);
  }

  template<typename T, t_n_ N>
  template<typename F>
  inline
  t_void t_pushlist<T, N>::each(t_err err, F f) const {
    impl_.each(err, f);
  }

  template<typename T, t_n_ N>
  template<typename F>
  inline
  t_void t_pushlist<T, N>::ceach(F f) const {
    impl_.each(f);
  }

  template<typename T, t_n_ N>
  template<typename F>
  inline
  t_void t_pushlist<T, N>::ceach(t_err err, F f) const {
    impl_.each(err, f);
  }

///////////////////////////////////////////////////////////////////////////////

  template<typename T>
  inline
  t_pushlist<T, 0>::t_pushlist(t_n max) : impl_{max} {
  }

  template<typename T>
  inline
  t_pushlist<T, 0>::t_pushlist(t_err err, t_n max) : impl_{err, max} {
  }

  template<typename T>
  inline
  t_pushlist<T, 0>::operator t_validity() const {
    return impl_.operator t_validity();
  }

  template<typename T>
  inline
  typename t_pushlist<T, 0>::p_value t_pushlist<T, 0>::push_back() {
    return impl_.push_back();
  }

  template<typename T>
  inline
  typename t_pushlist<T, 0>::p_value t_pushlist<T, 0>::push_back(t_err err) {
    return impl_.push_back(err);
  }

  template<typename T>
  inline
  typename t_pushlist<T, 0>::p_value
    t_pushlist<T, 0>::push_back(R_value value) {
    return impl_.push_back(value);
  }

  template<typename T>
  inline
  typename t_pushlist<T, 0>::p_value
    t_pushlist<T, 0>::push_back(t_err err, R_value value) {
    return impl_.push_back(err, value);
  }

  template<typename T>
  inline
  typename t_pushlist<T, 0>::p_value
    t_pushlist<T, 0>::push_back(x_value value) {
    return impl_.push_back(std::move(value));
  }

  template<typename T>
  inline
  typename t_pushlist<T, 0>::p_value
    t_pushlist<T, 0>::push_back(t_err err, x_value value) {
    return impl_.push_back(err, std::move(value));
  }

  template<typename T>
  inline
  t_bool t_pushlist<T, 0>::is_full() const {
    return impl_.is_full();
  }

  template<typename T>
  inline
  t_bool t_pushlist<T, 0>::is_empty() const {
    return impl_.is_empty();
  }

  template<typename T>
  inline
  t_n t_pushlist<T, 0>::get_size() const {
    return impl_.get_size();
  }

  template<typename T>
  inline
  t_n t_pushlist<T, 0>::get_capacity() const {
    return impl_.get_capacity();
  }

  template<typename T>
  inline
  typename t_pushlist<T, 0>::p_value t_pushlist<T, 0>::get(t_ix ix) {
    return impl_.get(ix);
  }

  template<typename T>
  inline
  typename t_pushlist<T, 0>::p_value t_pushlist<T, 0>::get(t_err err,
                                                           t_ix ix) {
    return impl_.get(err, ix);
  }

  template<typename T>
  inline
  typename t_pushlist<T, 0>::P_value t_pushlist<T, 0>::get(t_ix ix) const {
    return impl_.get(ix);
  }

  template<typename T>
  inline
  typename t_pushlist<T, 0>::P_value t_pushlist<T, 0>::get(t_err err,
                                                            t_ix ix) const {
    return impl_.get(err, ix);
  }

  template<typename T>
  inline
  typename t_pushlist<T, 0>::P_value t_pushlist<T, 0>::cget(t_ix ix) const {
    return impl_.cget(ix);
  }

  template<typename T>
  inline
  typename t_pushlist<T, 0>::P_value t_pushlist<T, 0>::cget(t_err err,
                                                             t_ix ix) const {
    return impl_.cget(err, ix);
  }

  template<typename T>
  template<typename F>
  inline
  t_void t_pushlist<T, 0>::each(F f) {
    impl_.each(f);
  }

  template<typename T>
  template<typename F>
  inline
  t_void t_pushlist<T, 0>::each(t_err err, F f) {
    impl_.each(err, f);
  }

  template<typename T>
  template<typename F>
  inline
  t_void t_pushlist<T, 0>::each(F f) const {
    impl_.each(f);
  }

  template<typename T>
  template<typename F>
  inline
  t_void t_pushlist<T, 0>::each(t_err err, F f) const {
    impl_.each(err, f);
  }

  template<typename T>
  template<typename F>
  inline
  t_void t_pushlist<T, 0>::ceach(F f) const {
    impl_.each(f);
  }

  template<typename T>
  template<typename F>
  inline
  t_void t_pushlist<T, 0>::ceach(t_err err, F f) const {
    impl_.each(err, f);
  }

///////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
