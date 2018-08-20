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

#ifndef _DAINTY_CONTAINTER_FREELIST_H_
#define _DAINTY_CONTAINTER_FREELIST_H_

#include "dainty_named.h"
#include "dainty_container_freelist_impl.h"

namespace dainty
{
namespace container
{
namespace freelist
{
  using named::t_n;
  using named::t_validity;
  using named::VALID;
  using named::INVALID;

///////////////////////////////////////////////////////////////////////////////

  template<typename T> inline t_void cleanup_(T&) { }

  template<typename T, t_n_ N = 0, t_void (*CLEANUP)(T&) = cleanup_<T> >
  class t_freelist final {
    using t_impl_ = t_freelist_impl_<T, CLEANUP>;
  public:
    using t_id     = typename t_impl_::t_id;
    using t_result = typename t_impl_::t_result;
    using t_value  = typename t_impl_::t_value;
    using x_value  = typename t_impl_::x_value;
    using r_value  = typename t_impl_::r_value;
    using R_value  = typename t_impl_::R_value;
    using p_value  = typename t_impl_::p_value;
    using P_value  = typename t_impl_::P_value;

    t_freelist();
    ~t_freelist();

    t_freelist(const t_freelist&)            = delete;
    t_freelist(t_freelist&&)                 = delete;
    t_freelist& operator=(const t_freelist&) = delete;
    t_freelist& operator=(t_freelist&&)      = delete;

    operator t_validity() const;

    t_result insert();
    t_result insert(t_err);
    t_result insert(       R_value);
    t_result insert(t_err, R_value);
    t_result insert(       x_value);
    t_result insert(t_err, x_value);

    t_bool erase(       t_id);
    t_bool erase(t_err, t_id);
    t_bool erase(       P_value);
    t_bool erase(t_err, P_value);

    t_void clear();
    t_void clear(t_err);

    t_bool is_full() const;
    t_bool is_empty() const;

    t_n get_size() const;
    t_n get_capacity() const;

    p_value  get(       t_id);
    p_value  get(t_err, t_id);
    P_value  get(       t_id) const;
    P_value  get(t_err, t_id) const;
    P_value cget(       t_id) const;
    P_value cget(t_err, t_id) const;

    template<typename F> t_void  each(       F);
    template<typename F> t_void  each(t_err, F);
    template<typename F> t_void  each(       F) const;
    template<typename F> t_void  each(t_err, F) const;
    template<typename F> t_void ceach(       F) const;
    template<typename F> t_void ceach(t_err, F) const;

  private:
    typename t_impl_::t_entry store_[N];
    t_impl_ impl_;
  };

///////////////////////////////////////////////////////////////////////////////

  template<typename T, t_void (*CLEANUP)(T&)>
  class t_freelist<T, 0, CLEANUP> final {
    using t_impl_ = t_freelist_impl_<T, CLEANUP>;
  public:
    using t_id     = typename t_impl_::t_id;
    using t_result = typename t_impl_::t_result;
    using t_value  = typename t_impl_::t_value;
    using x_value  = typename t_impl_::x_value;
    using r_value  = typename t_impl_::r_value;
    using R_value  = typename t_impl_::R_value;
    using p_value  = typename t_impl_::p_value;
    using P_value  = typename t_impl_::P_value;

    t_freelist(       t_n max);
    t_freelist(t_err, t_n max);
    ~t_freelist();

    t_freelist(const t_freelist&)            = delete;
    t_freelist(t_freelist&&)                 = delete;
    t_freelist& operator=(const t_freelist&) = delete;
    t_freelist& operator=(t_freelist&&)      = delete;

    operator t_validity() const;

    t_result insert();
    t_result insert(t_err);
    t_result insert(       R_value);
    t_result insert(t_err, R_value);
    t_result insert(       x_value);
    t_result insert(t_err, x_value);

    t_bool erase(       t_id);
    t_bool erase(t_err, t_id);
    t_bool erase(       P_value);
    t_bool erase(t_err, P_value);

    t_void clear();
    t_void clear(t_err);

    t_bool is_full() const;
    t_bool is_empty() const;

    t_n get_size() const;
    t_n get_capacity() const;

    p_value  get(       t_id);
    p_value  get(t_err, t_id);
    P_value  get(       t_id) const;
    P_value  get(t_err, t_id) const;
    P_value cget(       t_id) const;
    P_value cget(t_err, t_id) const;

    template<typename F> t_void  each(       F);
    template<typename F> t_void  each(t_err, F);
    template<typename F> t_void  each(       F) const;
    template<typename F> t_void  each(t_err, F) const;
    template<typename F> t_void ceach(       F) const;
    template<typename F> t_void ceach(t_err, F) const;

  private:
    t_n_ max_;
    typename t_impl_::p_entry store_;
    t_impl_ impl_;
  };

///////////////////////////////////////////////////////////////////////////////

  template<typename T, t_n_ N, t_void (*CLEANUP)(T&)>
  inline
  t_freelist<T, N, CLEANUP>::t_freelist() : impl_(&store_[0], N) {
  }

  template<typename T, t_n_ N, t_void (*CLEANUP)(T&)>
  inline
  t_freelist<T, N, CLEANUP>::~t_freelist() {
    clear();
  }

  template<typename T, t_n_ N, t_void (*CLEANUP)(T&)>
  inline
  t_freelist<T, N, CLEANUP>::operator t_validity() const {
    return VALID;
  }

  template<typename T, t_n_ N, t_void (*CLEANUP)(T&)>
  inline
  typename t_freelist<T, N, CLEANUP>::t_result
    t_freelist<T, N, CLEANUP>::insert() {
    return impl_.insert(store_, N);
  }

  template<typename T, t_n_ N, t_void (*CLEANUP)(T&)>
  inline
  typename t_freelist<T, N, CLEANUP>::t_result
    t_freelist<T, N, CLEANUP>::insert(t_err err) {
    return impl_.insert(err, store_, N);
  }

  template<typename T, t_n_ N, t_void (*CLEANUP)(T&)>
  inline
  typename t_freelist<T, N, CLEANUP>::t_result
    t_freelist<T, N, CLEANUP>::insert(R_value value) {
    return impl_.insert(store_, N, value);
  }

  template<typename T, t_n_ N, t_void (*CLEANUP)(T&)>
  inline
  typename t_freelist<T, N, CLEANUP>::t_result
    t_freelist<T, N, CLEANUP>::insert(t_err err, R_value value) {
    return impl_.insert(err, store_, N, value);
  }

  template<typename T, t_n_ N, t_void (*CLEANUP)(T&)>
  inline
  typename t_freelist<T, N, CLEANUP>::t_result
    t_freelist<T, N, CLEANUP>::insert(x_value value) {
    return impl_.insert(store_, N, std::move(value));
  }

  template<typename T, t_n_ N, t_void (*CLEANUP)(T&)>
  inline
  typename t_freelist<T, N, CLEANUP>::t_result
    t_freelist<T, N, CLEANUP>::insert(t_err err, x_value value) {
    return impl_.insert(err, store_, N, std::move(value));
  }

  template<typename T, t_n_ N, t_void (*CLEANUP)(T&)>
  inline
  t_bool t_freelist<T, N, CLEANUP>::erase(t_id id) {
    return impl_.erase(store_, N, named::get(id));
  }

  template<typename T, t_n_ N, t_void (*CLEANUP)(T&)>
  inline
  t_bool t_freelist<T, N, CLEANUP>::erase(t_err err, t_id id) {
    return impl_.erase(err, store_, N, named::get(id));
  }

  template<typename T, t_n_ N, t_void (*CLEANUP)(T&)>
  inline
  t_bool t_freelist<T, N, CLEANUP>::erase(P_value p) {
    return impl_.erase(store_, N, p);
  }

  template<typename T, t_n_ N, t_void (*CLEANUP)(T&)>
  inline
  t_bool t_freelist<T, N, CLEANUP>::erase(t_err err, P_value p) {
    return impl_.erase(err, store_, N, p);
  }

  template<typename T, t_n_ N, t_void (*CLEANUP)(T&)>
  inline
  t_void t_freelist<T, N, CLEANUP>::clear() {
    impl_.clear(store_, N);
  }

  template<typename T, t_n_ N, t_void (*CLEANUP)(T&)>
  inline
  t_void t_freelist<T, N, CLEANUP>::clear(t_err err) {
    impl_.clear(err, store_, N);
  }

  template<typename T, t_n_ N, t_void (*CLEANUP)(T&)>
  inline
  t_bool t_freelist<T, N, CLEANUP>::is_full() const {
    return impl_.is_full(N);
  }

  template<typename T, t_n_ N, t_void (*CLEANUP)(T&)>
  inline
  t_bool t_freelist<T, N, CLEANUP>::is_empty() const {
    return impl_.is_empty();
  }

  template<typename T, t_n_ N, t_void (*CLEANUP)(T&)>
  inline
  t_n t_freelist<T, N, CLEANUP>::get_size() const {
    return t_n{impl_.get_size()};
  }

  template<typename T, t_n_ N, t_void (*CLEANUP)(T&)>
  inline
  t_n t_freelist<T, N, CLEANUP>::get_capacity() const {
    return t_n{N};
  }

  template<typename T, t_n_ N, t_void (*CLEANUP)(T&)>
  inline
  typename t_freelist<T, N, CLEANUP>::p_value
    t_freelist<T, N, CLEANUP>::get(t_id id) {
    return impl_.get(store_, N, named::get(id));
  }

  template<typename T, t_n_ N, t_void (*CLEANUP)(T&)>
  inline
  typename t_freelist<T, N, CLEANUP>::p_value
    t_freelist<T, N, CLEANUP>::get(t_err err, t_id id) {
    return impl_.get(err, store_, N, named::get(id));
  }

  template<typename T, t_n_ N, t_void (*CLEANUP)(T&)>
  inline
  typename t_freelist<T, N, CLEANUP>::P_value
    t_freelist<T, N, CLEANUP>::get(t_id id) const {
    return impl_.get(store_, N, named::get(id));
  }

  template<typename T, t_n_ N, t_void (*CLEANUP)(T&)>
  inline
  typename t_freelist<T, N, CLEANUP>::P_value
    t_freelist<T, N, CLEANUP>::get(t_err err, t_id id) const {
    return impl_.get(err, store_, N, named::get(id));
  }

  template<typename T, t_n_ N, t_void (*CLEANUP)(T&)>
  inline
  typename t_freelist<T, N, CLEANUP>::P_value
    t_freelist<T, N, CLEANUP>::cget(t_id id) const {
    return impl_.get(store_, N, named::get(id));
  }

  template<typename T, t_n_ N, t_void (*CLEANUP)(T&)>
  inline
  typename t_freelist<T, N, CLEANUP>::P_value
    t_freelist<T, N, CLEANUP>::cget(t_err err, t_id id) const {
    return impl_.get(err, store_, N, named::get(id));
  }

  template<typename T, t_n_ N, t_void (*CLEANUP)(T&)>
  template<typename F>
  inline
  t_void t_freelist<T, N, CLEANUP>::each(F f) {
    impl_.each(store_, N, f);
  }

  template<typename T, t_n_ N, t_void (*CLEANUP)(T&)>
  template<typename F>
  inline
  t_void t_freelist<T, N, CLEANUP>::each(t_err err, F f) {
    impl_.each(err, store_, N, f);
  }

  template<typename T, t_n_ N, t_void (*CLEANUP)(T&)>
  template<typename F>
  inline
  t_void t_freelist<T, N, CLEANUP>::each(F f) const {
    impl_.each(store_, N, f);
  }

  template<typename T, t_n_ N, t_void (*CLEANUP)(T&)>
  template<typename F>
  inline
  t_void t_freelist<T, N, CLEANUP>::each(t_err err, F f) const {
    impl_.each(err, store_, N, f);
  }

  template<typename T, t_n_ N, t_void (*CLEANUP)(T&)>
  template<typename F>
  inline
  t_void t_freelist<T, N, CLEANUP>::ceach(F f) const {
    impl_.each(store_, N, f);
  }

  template<typename T, t_n_ N, t_void (*CLEANUP)(T&)>
  template<typename F>
  inline
  t_void t_freelist<T, N, CLEANUP>::ceach(t_err err, F f) const {
    impl_.each(err, store_, N, f);
  }

///////////////////////////////////////////////////////////////////////////////

  template<typename T, t_void (*CLEANUP)(T&)>
  inline
  t_freelist<T, 0, CLEANUP>::t_freelist(t_n max)
    : max_{named::get(max)},
      store_{new typename t_impl_::t_entry[max_]},
      impl_{store_, max_} {
  }

  template<typename T, t_void (*CLEANUP)(T&)>
  inline
  t_freelist<T, 0, CLEANUP>::t_freelist(t_err err, t_n max)
    : max_{named::get(max)},
      store_{ !err ? new typename t_impl_::t_entry[max_] : nullptr},
      impl_{err, store_, max_} {
  }

  template<typename T, t_void (*CLEANUP)(T&)>
  inline
  t_freelist<T, 0, CLEANUP>::~t_freelist() {
    if (store_) {
      clear();
      delete [] store_;
    }
  }

  template<typename T, t_void (*CLEANUP)(T&)>
  inline
  t_freelist<T, 0, CLEANUP>::operator t_validity() const {
    return store_ ? VALID : INVALID;
  }

  template<typename T, t_void (*CLEANUP)(T&)>
  inline
  typename t_freelist<T, 0, CLEANUP>::t_result
    t_freelist<T, 0, CLEANUP>::insert() {
    return impl_.insert(store_, max_);
  }

  template<typename T, t_void (*CLEANUP)(T&)>
  inline
  typename t_freelist<T, 0, CLEANUP>::t_result
    t_freelist<T, 0, CLEANUP>::insert(t_err err) {
    return impl_.insert(err, store_, max_);
  }

  template<typename T, t_void (*CLEANUP)(T&)>
  inline
  typename t_freelist<T, 0, CLEANUP>::t_result
    t_freelist<T, 0, CLEANUP>::insert(R_value value) {
    return impl_.insert(store_, max_, value);
  }

  template<typename T, t_void (*CLEANUP)(T&)>
  inline
  typename t_freelist<T, 0, CLEANUP>::t_result
    t_freelist<T, 0, CLEANUP>::insert(t_err err, R_value value) {
    return impl_.insert(err, store_, max_, value);
  }

  template<typename T, t_void (*CLEANUP)(T&)>
  inline
  typename t_freelist<T, 0, CLEANUP>::t_result
    t_freelist<T, 0, CLEANUP>::insert(x_value value) {
    return impl_.insert(store_, max_, std::move(value));
  }

  template<typename T, t_void (*CLEANUP)(T&)>
  inline
  typename t_freelist<T, 0, CLEANUP>::t_result
    t_freelist<T, 0, CLEANUP>::insert(t_err err, x_value value) {
    return impl_.insert(err, store_, max_, std::move(value));
  }

  template<typename T, t_void (*CLEANUP)(T&)>
  inline
  t_bool t_freelist<T, 0, CLEANUP>::erase(t_id id) {
    return impl_.erase(store_, max_, named::get(id));
  }

  template<typename T, t_void (*CLEANUP)(T&)>
  inline
  t_bool t_freelist<T, 0, CLEANUP>::erase(t_err err, t_id id) {
    return impl_.erase(err, store_, max_, named::get(id));
  }

  template<typename T, t_void (*CLEANUP)(T&)>
  inline
  t_bool t_freelist<T, 0, CLEANUP>::erase(P_value p) {
    return impl_.erase(store_, max_, p);
  }

  template<typename T, t_void (*CLEANUP)(T&)>
  inline
  t_bool t_freelist<T, 0, CLEANUP>::erase(t_err err, P_value p) {
    return impl_.erase(err, store_, max_, p);
  }

  template<typename T, t_void (*CLEANUP)(T&)>
  inline
  t_void t_freelist<T, 0, CLEANUP>::clear() {
    impl_.clear(store_, max_);
  }

  template<typename T, t_void (*CLEANUP)(T&)>
  inline
  t_void t_freelist<T, 0, CLEANUP>::clear(t_err err) {
    impl_.clear(err, store_, max_);
  }

  template<typename T, t_void (*CLEANUP)(T&)>
  inline
  t_bool t_freelist<T, 0, CLEANUP>::is_full() const {
    return impl_.is_full(max_);
  }

  template<typename T, t_void (*CLEANUP)(T&)>
  inline
  t_bool t_freelist<T, 0, CLEANUP>::is_empty() const {
    return impl_.is_empty();
  }

  template<typename T, t_void (*CLEANUP)(T&)>
  inline
  t_n t_freelist<T, 0, CLEANUP>::get_size() const {
    return t_n{impl_.get_size()};
  }

  template<typename T, t_void (*CLEANUP)(T&)>
  inline
  t_n t_freelist<T, 0, CLEANUP>::get_capacity() const {
    return t_n{max_};
  }

  template<typename T, t_void (*CLEANUP)(T&)>
  inline
  typename t_freelist<T, 0, CLEANUP>::p_value
    t_freelist<T, 0, CLEANUP>::get(t_id id) {
    return impl_.get(store_, max_, named::get(id));
  }

  template<typename T, t_void (*CLEANUP)(T&)>
  inline
  typename t_freelist<T, 0, CLEANUP>::p_value
    t_freelist<T, 0, CLEANUP>::get(t_err err, t_id id) {
    return impl_.get(err, store_, max_, named::get(id));
  }

  template<typename T, t_void (*CLEANUP)(T&)>
  inline
  typename t_freelist<T, 0, CLEANUP>::P_value
    t_freelist<T, 0, CLEANUP>::get(t_id id) const {
    return impl_.get(store_, max_, named::get(id));
  }

  template<typename T, t_void (*CLEANUP)(T&)>
  inline
  typename t_freelist<T, 0, CLEANUP>::P_value
    t_freelist<T, 0, CLEANUP>::get(t_err err, t_id id) const {
    return impl_.get(err, store_, max_, named::get(id));
  }

  template<typename T, t_void (*CLEANUP)(T&)>
  inline
  typename t_freelist<T, 0, CLEANUP>::P_value
    t_freelist<T, 0, CLEANUP>::cget(t_id id) const {
    return impl_.get(store_, max_, named::get(id));
  }

  template<typename T, t_void (*CLEANUP)(T&)>
  inline
  typename t_freelist<T, 0, CLEANUP>::P_value
    t_freelist<T, 0, CLEANUP>::cget(t_err err, t_id id) const {
    return impl_.get(err, store_, max_, named::get(id));
  }

  template<typename T, t_void (*CLEANUP)(T&)>
  template<typename F>
  inline
  t_void t_freelist<T, 0, CLEANUP>::each(F f) {
    impl_.each(store_, max_, f);
  }

  template<typename T, t_void (*CLEANUP)(T&)>
  template<typename F>
  inline
  t_void t_freelist<T, 0, CLEANUP>::each(t_err err, F f) {
    impl_.each(err, store_, max_, f);
  }

  template<typename T, t_void (*CLEANUP)(T&)>
  template<typename F>
  inline
  t_void t_freelist<T, 0, CLEANUP>::each(F f) const {
    impl_.each(store_, max_, f);
  }

  template<typename T, t_void (*CLEANUP)(T&)>
  template<typename F>
  inline
  t_void t_freelist<T, 0, CLEANUP>::each(t_err err, F f) const {
    impl_.each(err, store_, max_, f);
  }

  template<typename T, t_void (*CLEANUP)(T&)>
  template<typename F>
  inline
  t_void t_freelist<T, 0, CLEANUP>::ceach(F f) const {
    impl_.each(store_, max_, f);
  }

  template<typename T, t_void (*CLEANUP)(T&)>
  template<typename F>
  inline
  t_void t_freelist<T, 0, CLEANUP>::ceach(t_err err, F f) const {
    impl_.each(err, store_, max_, f);
  }

///////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
