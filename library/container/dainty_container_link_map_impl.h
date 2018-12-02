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

#ifndef _DAINTY_CONTAINER_LINK_MAP_IMPL_H_
#define _DAINTY_CONTAINER_LINK_MAP_IMPL_H_

#include <map>
#include "dainty_named_utility.h"
#include "dainty_container_err.h"

namespace dainty
{
namespace container
{
namespace link_map
{
  using err::t_err;
  using err::r_err;

  using named::t_bool;
  using named::t_void;
  using named::t_n;
  using named::t_validity;
  using named::VALID;
  using named::INVALID;
  using named::t_prefix;

  using named::utility::preserve;
  using named::utility::reset;
  using named::utility::x_cast;

  //////////////////////////////////////////////////////////////////////////

  template<typename K, typename T>
  struct t_entry_ {
    using P_key   = typename t_prefix<K>::P_;
    using t_value = T;
    using p_entry_ = typename t_prefix<t_entry_<K,T>>::p_;

    P_key    key_;
    t_value  value_;
    p_entry_ next_;
    p_entry_ prev_;
  };

  //////////////////////////////////////////////////////////////////////////

  template<typename K, typename T>
  class t_itr {
    using p_entry_ = typename t_prefix<t_entry_<K,T>>::p_;
  public:
    using t_key    = K;
    using R_key    = typename t_prefix<t_key>::R_;
    using t_value  = T;
    using r_value  = typename t_prefix<t_value>::r_;
    using R_value  = typename t_prefix<t_value>::r_;

    t_itr() = default;
    t_itr(p_entry_ entry) : entry_{entry}  { }

    operator t_validity() const { return entry_ ? VALID : INVALID; }

    R_key   get_key()           { return *(entry_->key_); }
    r_value get_value()         { return  entry_->value_; }
    R_value get_value()  const  { return  entry_->value_; }
    R_value get_cvalue() const  { return  entry_->value_; }

    t_itr& operator++()    { entry_ = entry_->next_; return *this;}
    t_itr  operator++(int) {
      auto tmp = entry_;
      entry_ = entry_->next_;
      return t_itr{tmp};
    }

    t_itr& operator--()    { entry_ = entry_->prev_; return *this;}
    t_itr  operator--(int) {
      auto tmp = entry_;
      entry_ = entry_->prev_;
      return t_itr{tmp};
    }

  private:
    p_entry_ entry_ = nullptr;
  };

  template<typename K, typename T>
  class t_citr {
    using P_entry_ = typename t_prefix<t_entry_<K, T>>::P_;
  public:
    using t_key    = K;
    using R_key    = typename t_prefix<t_key>::R_;
    using t_value  = T;
    using R_value  = typename t_prefix<t_value>::R_;
    using R_itr    = typename t_prefix<t_itr<K, T>>::R_;

    t_citr() = default;
    t_citr(R_itr itr) : entry_{itr.entry_} { }
    t_citr(P_entry_ entry) : entry_{entry} { }

    operator t_validity() const { return entry_ ? VALID : INVALID; }

    R_key   get_key()           { return *(entry_->key_); }
    R_value get_value()  const  { return  entry_->value_; }
    R_value get_cvalue() const  { return  entry_->value_; }

    t_citr& operator++()    { entry_ = entry_->next_; return *this;}
    t_citr  operator++(int) {
      auto tmp = entry_;
      entry_ = entry_->next_;
      return t_citr{tmp};
    }

    t_citr& operator--()    { entry_ = entry_->prev_; return *this;}
    t_citr  operator--(int) {
      auto tmp = entry_;
      entry_ = entry_->prev_;
      return t_citr{tmp};
    }

  private:
    P_entry_ entry_ = nullptr;
  };

  //////////////////////////////////////////////////////////////////////////

  template<typename K, typename T, typename C>
  class t_link_map_impl_ {
  public:
    using r_err      = err::r_err;
    using t_compare  = C;
    using t_key      = K;
    using P_key      = typename t_prefix<K>::P_;
    using R_key      = typename t_prefix<K>::R_;
    using t_value    = T;
    using t_itr   = link_map::t_itr <K, T>;
    using t_citr  = link_map::t_citr<K, T>;

    template<typename K1>
    t_itr insert(K1&& key) {
      return {};
    }

    template<typename K1>
    t_itr insert(t_err err, K1&& key) {
      return {};
    }

    template<typename K1, typename T1>
    t_itr insert(K1&& key, T1&& value) {
      return {};
    }

    template<typename K1, typename T1>
    t_itr insert(t_err err, K1&& key, T1&& value) {
      return {};
    }

    t_bool erase(R_key key) {
      return false;
    }

    t_bool erase(t_err err, R_key key) {
      return false;
    }

    t_void clear() {
    }

    t_void clear(t_err err) {
    }

    t_itr find(R_key key) {
      return {};
    }

    t_itr find(t_err err, R_key key) {
      return {};
    }

    t_citr find(R_key key) const {
      return {};
    }

    t_citr find(t_err err, R_key key) const {
      return {};
    }

    t_n get_size() const {
      return t_n{lk_.size()};
    }

    t_n get_capacity() const {
      return t_n{lk_.capacity()};
    }

    t_bool is_empty() const {
      return lk_.empty();
    }

    t_itr begin() {
      return {};
    }

    t_citr cbegin() const {
      return {};
    }

    t_itr rbegin() {
      return {};
    }

    t_citr crbegin() const {
      return {};
    }

    template<typename F>
    t_void each(F f) {
      for (auto& e : lk_)
        f(e.key, e.value);
    }

    template<typename F>
    t_void each(t_err err, F f) {
      ERR_GUARD(err) {
        for (auto& e : lk_)
          f(e.key, e.value);
      }
    }

    template<typename F>
    t_void ceach(F f) const {
      for (auto& e : lk_)
        f(e.key, e.value);
    }

    template<typename F>
    t_void ceach(t_err err, F f) const {
      ERR_GUARD(err) {
        for (auto& e : lk_)
          f(e.key, e.value);
      }
    }

  private:
    using t_entry_ = link_map::t_entry_<K, T>;
    using t_lk_    = std::map<K, t_entry_, C>;

    t_lk_ lk_;
  };

///////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
