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

#ifndef _DAINTY_CONTAINER_MAP_H_
#define _DAINTY_CONTAINER_MAP_H_

#include "dainty_named.h"
#include "dainty_container_map_impl.h"

namespace dainty
{
namespace container
{
namespace map
{
  //////////////////////////////////////////////////////////////////////////

  template<typename K>
  struct t_less_ {
    t_bool operator()(const K& lh, const K& rh) const {
      return lh < rh;
    }
  };

  //////////////////////////////////////////////////////////////////////////

  template<t_n_ N, typename K, typename T, typename C = t_less_<K> >
  class t_map {
    using t_impl_ = t_map_impl_<N, K, T, C, t_is_builtin_<K>::RESULT>;
  public:
    using t_compare  = typename t_impl_::t_compare;
    using t_keyvalue = typename t_impl_::t_keyvalue;
    using R_keyvalue = typename t_impl_::R_keyvalue;
    using p_keyvalue = typename t_impl_::p_keyvalue;
    using P_keyvalue = typename t_impl_::P_keyvalue;
    using x_keyvalue = typename t_impl_::x_keyvalue;
    using t_key      = typename t_impl_::t_key;
    using R_key      = typename t_impl_::R_key;
    using t_value    = typename t_impl_::t_value;
    using t_result   = typename t_impl_::t_result;
    using t_cresult  = typename t_impl_::t_cresult;

    t_result insert(       R_key);
    t_result insert(t_err, R_key);
    t_result insert(       R_keyvalue);
    t_result insert(t_err, R_keyvalue);
    t_result insert(       x_keyvalue);
    t_result insert(t_err, x_keyvalue);

    t_bool erase(       R_key);
    t_bool erase(t_err, R_key);
    t_bool erase(       t_ix);
    t_bool erase(t_err, t_ix);
    t_bool erase(       t_id);
    t_bool erase(t_err, t_id);

    t_void clear();
    t_void clear(t_err);

    t_result  find(       R_key);
    t_result  find(t_err, R_key);
    t_cresult find(       R_key) const;
    t_cresult find(t_err, R_key) const;

    t_n    get_size() const;
    t_n    get_capacity() const;
    t_bool is_empty() const;

    p_keyvalue  get(       t_ix);
    p_keyvalue  get(t_err, t_ix);
    P_keyvalue  get(       t_ix) const;
    P_keyvalue  get(t_err, t_ix) const;
    P_keyvalue cget(       t_ix) const;
    P_keyvalue cget(t_err, t_ix) const;

    p_keyvalue  get(       t_id);
    p_keyvalue  get(t_err, t_id);
    P_keyvalue  get(       t_id) const;
    P_keyvalue  get(t_err, t_id) const;
    P_keyvalue cget(       t_id) const;
    P_keyvalue cget(t_err, t_id) const;

    template<typename F> t_void  each(       F);
    template<typename F> t_void  each(t_err, F);
    template<typename F> t_void  each(       F) const;
    template<typename F> t_void  each(t_err, F) const;
    template<typename F> t_void ceach(       F) const;
    template<typename F> t_void ceach(t_err, F) const;

    template<typename F> t_void ordered_each (       F);
    template<typename F> t_void ordered_each (t_err, F);
    template<typename F> t_void ordered_each (       F) const;
    template<typename F> t_void ordered_each (t_err, F) const;
    template<typename F> t_void ordered_ceach(       F) const;
    template<typename F> t_void ordered_ceach(t_err, F) const;

  private:
    t_impl_ impl_;
  };

  //////////////////////////////////////////////////////////////////////////

  template<t_n_ N, typename K, typename T, typename C>
  inline
  typename t_map<N, K, T, C>::t_result t_map<N, K, T, C>::insert(R_key key) {
    return impl_.insert(key);
  }

  template<t_n_ N, typename K, typename T, typename C>
  inline
  typename t_map<N, K, T, C>::t_result t_map<N, K, T, C>::insert(t_err err,
                                                                 R_key key) {
    return impl_.insert(err, key);
  }

  template<t_n_ N, typename K, typename T, typename C>
  inline
  typename t_map<N, K, T, C>::t_result
    t_map<N, K, T, C>::insert(R_keyvalue keyvalue) {
    return impl_.insert(keyvalue);
  }

  template<t_n_ N, typename K, typename T, typename C>
  inline
  typename t_map<N, K, T, C>::t_result
    t_map<N, K, T, C>::insert(t_err err, R_keyvalue keyvalue) {
    return impl_.insert(err, keyvalue);
  }

  template<t_n_ N, typename K, typename T, typename C>
  inline
  typename t_map<N, K, T, C>::t_result
    t_map<N, K, T, C>::insert(x_keyvalue keyvalue) {
    return impl_.insert(std::move(keyvalue));
  }

  template<t_n_ N, typename K, typename T, typename C>
  inline
  typename t_map<N, K, T, C>::t_result
    t_map<N, K, T, C>::insert(t_err err, x_keyvalue keyvalue) {
    return impl_.insert(err, std::move(keyvalue));
  }

  template<t_n_ N, typename K, typename T, typename C>
  inline
  t_bool t_map<N, K, T, C>::erase(R_key key) {
    return impl_.erase(key);
  }

  template<t_n_ N, typename K, typename T, typename C>
  inline
  t_bool t_map<N, K, T, C>::erase(t_err err, R_key key) {
    return impl_.erase(err, key);
  }

  template<t_n_ N, typename K, typename T, typename C>
  inline
  t_bool t_map<N, K, T, C>::erase(t_ix ix) {
    return impl_.erase(ix);
  }

  template<t_n_ N, typename K, typename T, typename C>
  inline
  t_bool t_map<N, K, T, C>::erase(t_err err, t_ix ix) {
    return impl_.erase(err, ix);
  }

  template<t_n_ N, typename K, typename T, typename C>
  inline
  t_bool t_map<N, K, T, C>::erase(t_id id) {
    return impl_.erase(id);
  }

  template<t_n_ N, typename K, typename T, typename C>
  inline
  t_bool t_map<N, K, T, C>::erase(t_err err, t_id id) {
    return impl_.erase(err, id);
  }

  template<t_n_ N, typename K, typename T, typename C>
  inline
  t_void t_map<N, K, T, C>::clear() {
    impl_.clear();
  }

  template<t_n_ N, typename K, typename T, typename C>
  inline
  t_void t_map<N, K, T, C>::clear(t_err err) {
    impl_.clear(err);
  }

  template<t_n_ N, typename K, typename T, typename C>
  inline
  typename t_map<N, K, T, C>::t_result
    t_map<N, K, T, C>::find(R_key key) {
    return impl_.find(key);
  }

  template<t_n_ N, typename K, typename T, typename C>
  inline
  typename t_map<N, K, T, C>::t_result
    t_map<N, K, T, C>::find(t_err err, R_key key) {
    return impl_.find(err, key);
  }

  template<t_n_ N, typename K, typename T, typename C>
  inline
  typename t_map<N, K, T, C>::t_cresult
    t_map<N, K, T, C>::find(R_key key) const {
    return impl_.find(key);
  }

  template<t_n_ N, typename K, typename T, typename C>
  inline
  typename t_map<N, K, T, C>::t_cresult
    t_map<N, K, T, C>::find(t_err err, R_key key) const {
    return impl_.find(err, key);
  }

  template<t_n_ N, typename K, typename T, typename C>
  inline
  t_n t_map<N, K, T, C>::get_size() const {
    return impl_.get_size();
  }

  template<t_n_ N, typename K, typename T, typename C>
  inline
  t_n t_map<N, K, T, C>::get_capacity() const {
    return impl_.get_capacity();
  }

  template<t_n_ N, typename K, typename T, typename C>
  inline
  t_bool t_map<N, K, T, C>::is_empty() const {
    return impl_.is_empty();
  }

  template<t_n_ N, typename K, typename T, typename C>
  inline
  typename t_map<N, K, T, C>::p_keyvalue
    t_map<N, K, T, C>::get(t_ix ix) {
    return impl_.get(ix);
  }

  template<t_n_ N, typename K, typename T, typename C>
  inline
  typename t_map<N, K, T, C>::p_keyvalue
    t_map<N, K, T, C>::get(t_err err, t_ix ix) {
    return impl_.get(err, ix);
  }

  template<t_n_ N, typename K, typename T, typename C>
  inline
  typename t_map<N, K, T, C>::P_keyvalue
    t_map<N, K, T, C>::get(t_ix ix) const {
    return impl_.get(ix);
  }

  template<t_n_ N, typename K, typename T, typename C>
  inline
  typename t_map<N, K, T, C>::P_keyvalue
    t_map<N, K, T, C>::cget(t_ix ix) const {
    return impl_.get(ix);
  }

  template<t_n_ N, typename K, typename T, typename C>
  inline
  typename t_map<N, K, T, C>::P_keyvalue
    t_map<N, K, T, C>::cget(t_err err, t_ix ix) const {
    return impl_.get(err, ix);
  }

  template<t_n_ N, typename K, typename T, typename C>
  inline
  typename t_map<N, K, T, C>::p_keyvalue
    t_map<N, K, T, C>::get(t_id id) {
    return impl_.get(id);
  }

  template<t_n_ N, typename K, typename T, typename C>
  inline
  typename t_map<N, K, T, C>::p_keyvalue
    t_map<N, K, T, C>::get(t_err err, t_id id) {
    return impl_.get(err, id);
  }

  template<t_n_ N, typename K, typename T, typename C>
  inline
  typename t_map<N, K, T, C>::P_keyvalue
    t_map<N, K, T, C>::get(t_id id) const {
    return impl_.get(id);
  }

  template<t_n_ N, typename K, typename T, typename C>
  inline
  typename t_map<N, K, T, C>::P_keyvalue
    t_map<N, K, T, C>::get(t_err err, t_id id) const {
    return impl_.get(err, id);
  }

  template<t_n_ N, typename K, typename T, typename C>
  inline
  typename t_map<N, K, T, C>::P_keyvalue
    t_map<N, K, T, C>::cget(t_id id) const {
    return impl_.get(id);
  }

  template<t_n_ N, typename K, typename T, typename C>
  inline
  typename t_map<N, K, T, C>::P_keyvalue
    t_map<N, K, T, C>::cget(t_err err, t_id id) const {
    return impl_.get(err, id);
  }

  template<t_n_ N, typename K, typename T, typename C>
  template<typename F>
  inline
  t_void t_map<N, K, T, C>::each(F f) {
    impl_.each(f);
  }

  template<t_n_ N, typename K, typename T, typename C>
  template<typename F>
  inline
  t_void t_map<N, K, T, C>::each(t_err err, F f) {
    impl_.each(err, f);
  }

  template<t_n_ N, typename K, typename T, typename C>
  template<typename F>
  inline
  t_void t_map<N, K, T, C>::each(F f) const {
    impl_.each(f);
  }

  template<t_n_ N, typename K, typename T, typename C>
  template<typename F>
  inline
  t_void t_map<N, K, T, C>::each(t_err err, F f) const {
    impl_.each(err, f);
  }

  template<t_n_ N, typename K, typename T, typename C>
  template<typename F>
  inline
  t_void t_map<N, K, T, C>::ceach(F f) const {
    impl_.each(f);
  }

  template<t_n_ N, typename K, typename T, typename C>
  template<typename F>
  inline
  t_void t_map<N, K, T, C>::ceach(t_err err, F f) const {
    impl_.each(err, f);
  }

  template<t_n_ N, typename K, typename T, typename C>
  template<typename F>
  inline
  t_void t_map<N, K, T, C>::ordered_each(F f) {
    impl_.ordered_each(f);
  }

  template<t_n_ N, typename K, typename T, typename C>
  template<typename F>
  inline
  t_void t_map<N, K, T, C>::ordered_each(t_err err, F f) {
    impl_.ordered_each(err, f);
  }

  template<t_n_ N, typename K, typename T, typename C>
  template<typename F>
  inline
  t_void t_map<N, K, T, C>::ordered_each(F f) const {
    impl_.ordered_each(f);
  }

  template<t_n_ N, typename K, typename T, typename C>
  template<typename F>
  inline
  t_void t_map<N, K, T, C>::ordered_each(t_err err, F f) const {
    impl_.ordered_each(err, f);
  }

  template<t_n_ N, typename K, typename T, typename C>
  template<typename F>
  inline
  t_void t_map<N, K, T, C>::ordered_ceach(F f) const {
    impl_.ordered_each(f);
  }

  template<t_n_ N, typename K, typename T, typename C>
  template<typename F>
  inline
  t_void t_map<N, K, T, C>::ordered_ceach(t_err err, F f) const {
    impl_.ordered_each(err, f);
  }

///////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
