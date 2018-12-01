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

#ifndef _DAINTY_CONTAINER_IX_MAP_IMPL_H_
#define _DAINTY_CONTAINER_IX_MAP_IMPL_H_

#include <map>
#include "dainty_named_utility.h"
#include "dainty_container_err.h"
#include "dainty_container_freelist.h"

namespace dainty
{
namespace container
{
namespace ix_map
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

  using freelist::t_id;

  //////////////////////////////////////////////////////////////////////////

  template<typename K, typename T>
  struct t_result {
    using t_id    = ix_map::t_id;
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
    using t_id     = ix_map::t_id;
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
  class t_ix_map_impl_ {
  public:
    using r_err      = err::r_err;
    using t_compare  = C;
    using t_key      = K;
    using P_key      = typename t_prefix<K>::P_;
    using R_key      = typename t_prefix<K>::R_;
    using t_value    = T;
    using t_id       = ix_map::t_id;
    using t_result   = ix_map::t_result <K, T>;
    using t_cresult  = ix_map::t_cresult<K, T>;

    inline
    t_ix_map_impl_(t_n max) : store_{max} {
    }

    inline
    t_ix_map_impl_(r_err err, t_n max) : store_{err, max} {
    }

    inline
    operator t_validity() const {
      return store_;
    }

    template<typename K1>
    inline
    t_result insert(K1&& key) {
      auto result = store_.insert();
      if (result == VALID) {
        auto pair = lk_.insert(t_lk_value_{preserve<K1>(key), result.id});
        if (pair.second) {
          result.ptr->key = &pair.first->first;
          return {result.id, result.ptr->key, result.ptr};
        }
        store_.erase(result.id);
      }
      return {};
    }

    template<typename K1>
    inline
    t_result insert(r_err err, K1&& key) {
      ERR_GUARD(err) {
        auto result = store_.insert(err);
        if (result == VALID) {
          auto pair = lk_.insert(t_lk_value_{preserve<K1>(key), result.id});
          if (pair.second) {
            result.ptr->key = &pair.first->first;
            return {result.id, result.ptr->key, result.ptr};
          }
          store_.erase(result.id);
        }
      }
      return {};
    }

    template<typename K1, typename T1>
    inline
    t_result insert(K1&& key, T1&& value) {
      auto result = store_.insert(preserve<T1>(value));
      if (result == VALID) {
        auto pair = lk_.insert(t_lk_value_{preserve<K1>(key), result.id});
        if (pair.second) {
          result.ptr->key = &pair.first->first;
          return {result.id, result.ptr->key, result.ptr};
        }
        store_.erase(result.id);
      }
      return {};
    }

    template<typename K1, typename T1>
    inline
    t_result insert(r_err err, K1&& key, T1&& value) {
      ERR_GUARD(err) {
        auto result = store_.insert(err, preserve<T1>(value));
        if (result == VALID) {
          auto pair = lk_.insert(t_lk_value_{preserve<K1>(key), result.id});
          if (pair.second) {
            result.ptr->key = &pair.first->first;
            return {result.id, result.ptr->key, result.ptr};
          }
          store_.erase(result.id);
        }
      }
      return {};
    }

    inline
    t_bool erase(R_key key) {
      auto itr = lk_.find(key);
      if (itr != lk_.end()) {
        store_.erase(itr->second);
        lk_.erase(itr);
        return true;
      }
      return false;
    }

    inline
    t_bool erase(r_err err, R_key key) {
      ERR_GUARD(err) {
        auto itr = lk_.find(key);
        if (itr != lk_.end()) {
          store_.erase(itr->second);
          lk_.erase(err, itr);
          return true;
        }
      }
      return false;
    }

    inline
    t_bool erase(t_id id) {
      auto entry = store_.get(id);
      if (entry) {
        lk_.erase(*entry->key);
        store_.erase(id);
        return true;
      }
      return false;
    }

    inline
    t_bool erase(r_err err, t_id id) {
      ERR_GUARD(err) {
        auto entry = store_.get(id);
        if (entry) {
          lk_.erase(*entry->key);
          store_.erase(err, id);
          return true;
        }
      }
      return false;
    }

    inline
    t_void clear() {
      lk_.clear();
      store_.clear();
    }

    inline
    t_void clear(r_err err) {
      ERR_GUARD(err) {
        lk_.clear();
        store_.clear(err);
      }
    }

    inline
    t_result find(R_key key) {
      auto itr = lk_.find(key);
      if (itr != lk_.end()) {
        auto entry = store_.get(itr->second);
        return {itr->second, entry->key, &entry->value};
      }
      return {};
    }

    inline
    t_result find(r_err err, R_key key) {
      ERR_GUARD(err) {
        auto itr = lk_.find(key);
        if (itr != lk_.end()) {
          auto entry = store_.get(itr->second);
          return {itr->second, entry->key, &entry->value};
        }
      }
      return {};
    }

    inline
    t_cresult find(R_key key) const {
      auto itr = lk_.find(key);
      if (itr != lk_.end()) {
        auto entry = store_.get(itr->second);
        return {itr->second, entry->key, &entry->value};
      }
      return {};
    }

    inline
    t_cresult find(r_err err, R_key key) const {
      ERR_GUARD(err) {
        auto itr = lk_.find(key);
        if (itr != lk_.end()) {
          auto entry = store_.get(itr->second);
          return {itr->second, entry->key, &entry->value};
        }
      }
      return {};
    }

    inline
    t_n get_size() const {
      return store_.get_size();
    }

    inline
    t_n get_capacity() const {
      return store_.get_capacity();
    }

    inline
    t_bool is_empty() const {
      return store_.is_empty();
    }

    inline
    t_result get(t_id id) {
      auto entry = store_.get(id);
      return entry ? t_result{id, entry->key, &entry->value} : t_result{};
    }

    inline
    t_result get(r_err err, t_id id) {
      ERR_GUARD(err) {
        auto entry = store_.get(id);
        return entry ? t_result{id, entry->key, &entry->value} : t_result{};
      }
      return {};
    }

    inline
    t_cresult cget(t_id id) const {
      auto entry = store_.get(id);
      return entry ? t_result{id, entry->key, &entry->value} : t_result{};
    }

    inline
    t_cresult cget(r_err err, t_id id) const {
      ERR_GUARD(err) {
        auto entry = store_.get(id);
        return entry ? t_result{id, entry->key, &entry->value} : t_result{};
      }
      return {};
    }

    template<typename F>
    inline
    t_void each(F f) {
      for (auto& e : lk_)
        f(e.key, e.value);
    }

    template<typename F>
    inline
    t_void each(r_err err, F f) {
      ERR_GUARD(err) {
        for (auto& e : lk_)
          f(e.key, e.value);
      }
    }

    template<typename F>
    inline
    t_void ceach(F f) const {
      for (auto& e : lk_)
        f(e.key, e.value);
    }

    template<typename F>
    inline
    t_void ceach(r_err err, F f) const {
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
    using t_store_    = freelist::t_freelist<t_entry_>;
    using t_lk_       = std::map<K, t_id, C>;
    using t_lk_value_ = typename t_lk_::value_type;

    t_store_ store_;
    t_lk_    lk_;
  };

///////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
