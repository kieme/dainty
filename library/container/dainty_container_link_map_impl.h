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
  struct t_result {
    using t_key   = K;
    using P_key   = typename t_prefix<t_key>::P_;
    using t_value = T;
    using p_value = typename t_prefix<t_value>::p_;

    t_id    id;
    P_key   key;
    p_value value;

    operator t_validity() const { return value ? VALID : INVALID; }

    t_result()
      : id{0}, key{nullptr}, value{nullptr} {
    }
    t_result(t_id _id, P_key _key, p_value _value)
      : id{_id}, key{_key}, value{_value} {
    }
  };

  template<typename K, typename T>
  struct t_cresult {
    using t_id     = link_map::t_id;
    using t_key    = K;
    using P_key    = typename t_prefix<t_key>::P_;
    using t_value  = T;
    using P_value  = typename t_prefix<t_value>::P_;
    using R_result = typename t_prefix<t_result<K, T>>::R_;

    t_id    id;
    P_key   key;
    P_value value;

    operator t_validity() const { return value ? VALID : INVALID; }

    t_cresult() : id{0}, key{nullptr}, value{nullptr} { }
    t_cresult(t_id _id, P_key _key, P_value _value)
      : id{_id}, key{_key}, value{_value} {
    }
    t_cresult(R_result result)
      : id{result.id}, key{result.key}, value{result.value} {
    }
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
    using t_result   = link_map::t_result <K, T>;
    using t_cresult  = link_map::t_cresult<K, T>;

    template<typename K1>
    t_result insert(K1&& key) {
      return {};
    }

    template<typename K1>
    t_result insert(t_err err, K1&& key) {
      return {};
    }

    template<typename K1, typename T1>
    t_result insert(K1&& key, T1&& value) {
      return {};
    }

    template<typename K1, typename T1>
    t_result insert(t_err err, K1&& key, T1&& value) {
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

    t_result find(R_key key) {
      return {};
    }

    t_result find(t_err err, R_key key) {
      return {};
    }

    t_cresult find(R_key key) const {
      return {};
    }

    t_cresult find(t_err err, R_key key) const {
      return {};
    }

    t_n get_size() const {
      return store_.get_size();
    }

    t_n get_capacity() const {
      return store_.get_capacity();
    }

    t_bool is_empty() const {
      return store_.is_empty();
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
    struct t_entry_ {
      P_key   key;
      t_value value;
    };
    using t_lk_ = std::map<K, T, C>;

    t_lk_    lk_;
  };

///////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
