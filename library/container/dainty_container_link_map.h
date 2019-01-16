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

#ifndef _DAINTY_CONTAINER_LINK_MAP_H_
#define _DAINTY_CONTAINER_LINK_MAP_H_

#include "dainty_container_link_map_impl.h"

namespace dainty
{
namespace container
{
namespace link_map
{
  //////////////////////////////////////////////////////////////////////////

  template<typename K>
  struct t_less_ {
    t_bool operator()(const K& lh, const K& rh) const {
      return lh < rh;
    }
  };

  //////////////////////////////////////////////////////////////////////////

  template<typename K, typename T, typename C = t_less_<K> >
  class t_link_map {
    using t_impl_ = t_link_map_impl_<K, T, C>;
  public:
    using t_compare = typename t_impl_::t_compare;
    using t_key     = typename t_impl_::t_key;
    using R_key     = typename t_impl_::R_key;
    using t_value   = typename t_impl_::t_value;
    using t_itr     = typename t_impl_::t_itr;
    using t_citr    = typename t_impl_::t_citr;

    operator t_validity() const;

    template<typename K1>
    t_itr insert(       K1&&);
    template<typename K1>
    t_itr insert(t_err, K1&&);
    template<typename K1, typename T1>
    t_itr insert(       K1&&, T1&&);
    template<typename K1, typename T1>
    t_itr insert(t_err, K1&&, T1&&);

    t_bool erase(       R_key);
    t_bool erase(t_err, R_key);

    t_void clear();
    t_void clear(t_err);

    t_itr  find(       R_key);
    t_itr  find(t_err, R_key);
    t_citr find(       R_key) const;
    t_citr find(t_err, R_key) const;

    t_n    get_size() const;
    t_n    get_capacity() const;
    t_bool is_empty() const;

    t_itr   begin();
    t_itr   end  ();
    t_citr  begin() const;
    t_citr  end  () const;
    t_citr cbegin() const;
    t_citr cend  () const;

    t_itr   rbegin();
    t_itr   rend  ();
    t_citr  rbegin() const;
    t_citr  rend  () const;
    t_citr crbegin() const;
    t_citr crend  () const;

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

  template<typename K, typename T, typename C>
  inline
  t_link_map<K, T, C>::operator t_validity() const {
    return impl_;
  }

  template<typename K, typename T, typename C>
  template<typename K1>
  inline
  typename t_link_map<K, T, C>::t_itr
      t_link_map<K, T, C>::insert(K1&& key) {
    return impl_.insert(preserve<K1>(key));
  }

  template<typename K, typename T, typename C>
  template<typename K1>
  inline
  typename t_link_map<K, T, C>::t_itr
      t_link_map<K, T, C>::insert(t_err err, K1&& key) {
    return impl_.insert(err, preserve<K1>(key));
  }

  template<typename K, typename T, typename C>
  template<typename K1, typename T1>
  inline
  typename t_link_map<K, T, C>::t_itr
      t_link_map<K, T, C>::insert(K1&& key, T1&& value) {
    return impl_.insert(preserve<K1>(key), preserve<T1>(value));
  }

  template<typename K, typename T, typename C>
  template<typename K1, typename T1>
  inline
  typename t_link_map<K, T, C>::t_itr
      t_link_map<K, T, C>::insert(t_err err, K1&& key, T1&& value) {
    return impl_.insert(err, preserve<K1>(key), preserve<T1>(value));
  }

  template<typename K, typename T, typename C>
  inline
  t_bool t_link_map<K, T, C>::erase(R_key key) {
    return impl_.erase(key);
  }

  template<typename K, typename T, typename C>
  inline
  t_bool t_link_map<K, T, C>::erase(t_err err, R_key key) {
    return impl_.erase(err, key);
  }

  template<typename K, typename T, typename C>
  inline
  t_void t_link_map<K, T, C>::clear() {
    impl_.clear();
  }

  template<typename K, typename T, typename C>
  inline
  t_void t_link_map<K, T, C>::clear(t_err err) {
    impl_.clear(err);
  }

  template<typename K, typename T, typename C>
  inline
  typename t_link_map<K, T, C>::t_itr
      t_link_map<K, T, C>::find(R_key key) {
    return impl_.find(key);
  }

  template<typename K, typename T, typename C>
  inline
  typename t_link_map<K, T, C>::t_itr
      t_link_map<K, T, C>::find(t_err err, R_key key) {
    return impl_.find(err, key);
  }

  template<typename K, typename T, typename C>
  inline
  typename t_link_map<K, T, C>::t_citr
      t_link_map<K, T, C>::find(R_key key) const {
    return impl_.find(key);
  }

  template<typename K, typename T, typename C>
  inline
  typename t_link_map<K, T, C>::t_citr
      t_link_map<K, T, C>::find(t_err err, R_key key) const {
    return impl_.find(err, key);
  }

  template<typename K, typename T, typename C>
  inline
  t_n t_link_map<K, T, C>::get_size() const {
    return impl_.get_size();
  }

  template<typename K, typename T, typename C>
  inline
  t_n t_link_map<K, T, C>::get_capacity() const {
    return impl_.get_capacity();
  }

  template<typename K, typename T, typename C>
  inline
  t_bool t_link_map<K, T, C>::is_empty() const {
    return impl_.is_empty();
  }

  template<typename K, typename T, typename C>
  inline
  typename t_link_map<K, T, C>::t_itr t_link_map<K, T, C>::begin() {
    return impl_.begin();
  }

  template<typename K, typename T, typename C>
  inline
  typename t_link_map<K, T, C>::t_itr t_link_map<K, T, C>::end() {
    return impl_.end();
  }

  template<typename K, typename T, typename C>
  inline
  typename t_link_map<K, T, C>::t_citr t_link_map<K, T, C>::begin() const {
    return impl_.cbegin();
  }

  template<typename K, typename T, typename C>
  inline
  typename t_link_map<K, T, C>::t_citr t_link_map<K, T, C>::end() const {
    return impl_.cend();
  }

  template<typename K, typename T, typename C>
  inline
  typename t_link_map<K, T, C>::t_citr t_link_map<K, T, C>::cbegin() const {
    return impl_.cbegin();
  }

  template<typename K, typename T, typename C>
  inline
  typename t_link_map<K, T, C>::t_citr t_link_map<K, T, C>::cend() const {
    return impl_.cend();
  }

  template<typename K, typename T, typename C>
  inline
  typename t_link_map<K, T, C>::t_itr t_link_map<K, T, C>::rbegin() {
    return impl_.rbegin();
  }

  template<typename K, typename T, typename C>
  inline
  typename t_link_map<K, T, C>::t_itr t_link_map<K, T, C>::rend() {
    return impl_.rend();
  }

  template<typename K, typename T, typename C>
  inline
  typename t_link_map<K, T, C>::t_citr t_link_map<K, T, C>::rbegin() const {
    return impl_.crbegin();
  }

  template<typename K, typename T, typename C>
  inline
  typename t_link_map<K, T, C>::t_citr t_link_map<K, T, C>::rend() const {
    return impl_.crend();
  }

  template<typename K, typename T, typename C>
  inline
  typename t_link_map<K, T, C>::t_citr t_link_map<K, T, C>::crbegin() const {
    return impl_.crbegin();
  }

  template<typename K, typename T, typename C>
  inline
  typename t_link_map<K, T, C>::t_citr t_link_map<K, T, C>::crend() const {
    return impl_.crend();
  }

  template<typename K, typename T, typename C>
  template<typename F>
  inline
  t_void t_link_map<K, T, C>::each(F f) {
    impl_.each(f);
  }

  template<typename K, typename T, typename C>
  template<typename F>
  inline
  t_void t_link_map<K, T, C>::each(t_err err, F f) {
    impl_.each(err, f);
  }

  template<typename K, typename T, typename C>
  template<typename F>
  inline
  t_void t_link_map<K, T, C>::each(F f) const {
    impl_.ceach(f);
  }

  template<typename K, typename T, typename C>
  template<typename F>
  inline
  t_void t_link_map<K, T, C>::each(t_err err, F f) const {
    impl_.ceach(err, f);
  }

  template<typename K, typename T, typename C>
  template<typename F>
  inline
  t_void t_link_map<K, T, C>::ceach(F f) const {
    impl_.ceach(f);
  }

  template<typename K, typename T, typename C>
  template<typename F>
  inline
  t_void t_link_map<K, T, C>::ceach(t_err err, F f) const {
    impl_.ceach(err, f);
  }

///////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
