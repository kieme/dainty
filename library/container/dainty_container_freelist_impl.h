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

#ifndef _DAINTY_CONTAINTER_FREELIST_IMPL_H_
#define _DAINTY_CONTAINTER_FREELIST_IMPL_H_

#include <memory>
#include "dainty_container_err.h"
#include "dainty_container_valuestore.h"

namespace dainty
{
namespace container
{
namespace freelist
{
///////////////////////////////////////////////////////////////////////////////

  using err::t_err;
  using err::r_err;

  using base::types::t_prefix;
  using base::types::t_void;
  using base::types::t_bool;
  using base::types::t_uint64;
  using base::types::t_n_;

  using base::specific::t_specific;

///////////////////////////////////////////////////////////////////////////////

  enum  t_id_tag_ {};
  using t_id_ = t_n_;
  using t_id  = t_specific<t_id_, t_id_tag_>;

  constexpr t_id_ BAD_ID_{0};
  constexpr t_id  BAD_ID {BAD_ID_};

  template<typename T>
  struct t_result {
    using t_id    = freelist::t_id;
    using t_value = typename t_prefix<T>::t_;
    using r_value = typename t_prefix<T>::r_;
    using R_value = typename t_prefix<T>::R_;
    using p_value = typename t_prefix<T>::p_;
    using P_value = typename t_prefix<T>::P_;

    t_id    id;
    p_value ptr;

    inline t_result()                        : id(BAD_ID), ptr(nullptr) { }
    inline t_result(t_id_ _id, p_value _ptr) : id(_id),    ptr(_ptr)    { }

    inline operator t_bool() const                      { return  ptr; }
    inline r_value operator*()                          { return *ptr; }
    inline R_value operator*() const                    { return *ptr; }
    inline p_value operator->()                         { return  ptr; }
    inline P_value operator->() const                   { return  ptr; }
  };

///////////////////////////////////////////////////////////////////////////////

  template<typename T>
  struct t_entry {
    using t_store = valuestore::t_valuestore<T>;
    t_store store_;
    t_n_    free_;
  };

///////////////////////////////////////////////////////////////////////////////

  template<typename T, t_void (*CLEANUP)(T&)>
  class t_freelist_impl_ {
  public:
    using t_id     = freelist::t_id;
    using t_result = typename freelist::t_result<T>;
    using t_entry  = typename freelist::t_entry<T>;
    using p_entry  = typename t_prefix<t_entry>::p_;
    using P_entry  = typename t_prefix<t_entry>::P_;
    using r_entry  = typename t_prefix<t_entry>::r_;
    using R_entry  = typename t_prefix<t_entry>::R_;
    using t_value  = typename t_prefix<T>::t_;
    using p_value  = typename t_prefix<T>::p_;
    using P_value  = typename t_prefix<T>::P_;
    using r_value  = typename t_prefix<T>::r_;
    using R_value  = typename t_prefix<T>::R_;
    using x_value  = typename t_prefix<T>::x_;

    inline
    t_freelist_impl_(p_entry _entry, t_n_ max) : size_{0}, free_{0} {
      for (t_n_ i = 0; i < max; /**/) {
        r_entry entry = _entry[i++];
        entry.free_ = i;
      }
    }

    inline
    t_freelist_impl_(r_err err, p_entry _entry, t_n_ max)
      : size_{0}, free_{0} {
      ERR_GUARD(err) {
        if (_entry) {
          for (t_n_ i = 0; i < max; /**/) {
            r_entry entry = _entry[i++];
            entry.free_ = i;
          }
        } else
          err = err::E_INVALID_INST;
      }
    }

    inline
    t_result insert(p_entry _entry, t_n_ max) {
      if (free_ < max) {
        r_entry entry = _entry[free_];
        t_result tmp(free_ + 1, entry.store_.default_construct());
        free_ = entry.free_;
        entry.free_ = max + 1;
        ++size_;
        return tmp;
      }
      return {};
    }

    inline
    t_result insert(r_err err, p_entry _entry, t_n_ max) {
      ERR_GUARD(err) {
        if (_entry) {
          if (free_ < max) {
            r_entry entry = _entry[free_];
            t_result tmp(free_ + 1, entry.store_.default_construct());
            free_ = entry.free_;
            entry.free_ = max + 1;
            ++size_;
            return tmp;
          } else
            err = err::E_NO_SPACE;
        } else
          err = err::E_INVALID_INST;
      }
      return {};
    }

    inline
    t_result insert(p_entry _entry, t_n_ max, R_value value) {
      if (free_ < max) {
        r_entry entry = _entry[free_];
        t_result tmp(free_ + 1, entry.store_.copy_construct(value));
        free_ = entry.free_;
        entry.free_ = max + 1;
        ++size_;
        return tmp;
      }
      return {};
    }

    inline
    t_result insert(r_err err, p_entry _entry, t_n_ max, R_value value) {
      ERR_GUARD(err) {
        if (_entry) {
          if (free_ < max) {
            r_entry entry = _entry[free_];
            t_result tmp(free_ + 1, entry.store_.copy_construct(value));
            free_ = entry.free_;
            entry.free_ = max + 1;
            ++size_;
            return tmp;
          } else
            err = err::E_NO_SPACE;
        } else
          err = err::E_INVALID_INST;
      }
      return {};
    }

    inline
    t_result insert(p_entry _entry, t_n_ max, x_value value) {
      if (free_ < max) {
        r_entry entry = _entry[free_];
        t_result tmp(free_ + 1,
                     entry.store_.move_construct(base::x_cast(value)));
        free_ = entry.free_;
        entry.free_ = max + 1;
        ++size_;
        return tmp;
      }
      return {};
    }

    inline
    t_result insert(r_err err, p_entry _entry, t_n_ max, x_value value) {
      ERR_GUARD(err) {
        if (_entry) {
          if (free_ < max) {
            r_entry entry = _entry[free_];
            t_result tmp(free_ + 1,
                         entry.store_.move_construct(base::x_cast(value)));
            free_ = entry.free_;
            entry.free_ = max + 1;
            ++size_;
            return tmp;
          } else
            err = err::E_NO_SPACE;
        } else
          err = err::E_INVALID_INST;
      }
      return {};
    }

    inline
    t_bool erase(p_entry _entry, t_n_ max, t_id_ id) {
      if (id != BAD_ID_ && --id < max) {
        r_entry entry = _entry[id];
        if (entry.free_ == max + 1) {
          CLEANUP(entry.store_.ref());
          entry.store_.destruct();
          entry.free_  = free_;
          free_        = id;
          --size_;
          return true;
        }
      }
      return false;
    }

    inline
    t_bool erase(r_err err, p_entry _entry, t_n_ max, t_id_ id) {
      ERR_GUARD(err) {
        if (_entry) {
          if (id != BAD_ID_ && --id < max) {
            r_entry entry = _entry[id];
            if (entry.free_ == max + 1) {
              CLEANUP(entry.store_.ref());
              entry.store_.destruct();
              entry.free_  = free_;
              free_        = id;
              --size_;
              return true;
            } else
              err = err::E_UNUSED_ID;
          } else
            err = err::E_INVALID_ID;
        } else
          err = err::E_INVALID_INST;
      }
      return false;
    }

    inline
    t_bool erase(p_entry entry, t_n_ max, P_value p) {
      t_uint64 begin = (t_uint64)entry,
               end   = begin + (sizeof(t_entry)*max),
               pos   = (t_uint64)p;
      if (pos >= begin && pos < end && !(pos % alignof(t_value)))
        return erase((pos - begin)/sizeof(t_entry));
      return false;
    }

    inline
    t_bool erase(r_err err, p_entry entry, t_n_ max, P_value p) {
      ERR_GUARD(err) {
        t_uint64 begin = (t_uint64)entry,
                 end   = begin + (sizeof(t_entry)*max),
                 pos   = (t_uint64)p;
        if (pos >= begin && pos < end && !(pos % alignof(t_value)))
          return erase((pos - begin)/sizeof(t_entry));
      }
      return false;
    }

    inline
    t_void clear(p_entry _entry, t_n_ max) {
      for (t_n_ i = 0; i < max; /* none */ ) {
        r_entry entry = _entry[i++];
        if (entry.free_ == max + 1) {
          CLEANUP(entry.store_.ref());
          entry.store_.destruct();
        }
        entry.free_ = i;
      }
      size_ = 0;
      free_ = 0;
    }

    inline
    t_void clear(r_err err, p_entry _entry, t_n_ max) {
      ERR_GUARD(err) {
        if (_entry) {
          for (t_n_ i = 0; i < max; /* none */ ) {
            r_entry entry = _entry[i++];
            if (entry.free_ == max + 1) {
              CLEANUP(entry.store_.ref());
              entry.store_.destruct();
            }
            entry.free_ = i;
          }
          size_ = 0;
          free_ = 0;
        } else
          err = err::E_INVALID_INST;
      }
    }

    inline
    t_bool is_full(t_n_ max) const {
      return free_ == max;
    }

    inline
    t_bool is_empty() const {
      return size_ == 0;
    }

    inline
    t_n_ get_size() const {
      return size_;
    }

    inline
    p_value get(p_entry _entry, t_n_ max, t_id_ id) {
      if (id != BAD_ID_ && --id < max) {
        r_entry entry = _entry[id];
        if (entry.free_ == max + 1)
          return entry.store_.ptr();
      }
      return nullptr;
    }

    inline
    p_value get(r_err err, p_entry _entry, t_n_ max, t_id_ id) {
      ERR_GUARD(err) {
        if (_entry) {
          if (id != BAD_ID_ && --id < max) {
            r_entry entry = _entry[id];
            if (entry.free_ == max + 1)
              return entry.store_.ptr();
            err = err::E_UNUSED_ID;
          } else
            err = err::E_INVALID_ID;
        } else
          err = err::E_INVALID_INST;
      }
      return nullptr;
    }

    inline
    P_value get(P_entry _entry, t_n_ max, t_id_ id) const {
      if (id != BAD_ID_ && --id < max) {
        R_entry entry = _entry[id];
        if (entry.free_ == max + 1)
          return entry.store_.cptr();
      }
      return nullptr;
    }

    inline
    P_value get(r_err err, P_entry _entry, t_n_ max, t_id_ id) const {
      ERR_GUARD(err) {
        if (_entry) {
          if (id != BAD_ID_ && --id < max) {
            R_entry entry = _entry[id];
            if (entry.free_ == max + 1)
              return entry.store_.cptr();
            err = err::E_UNUSED_ID;
          } else
            err = err::E_INVALID_ID;
        } else
          err = err::E_INVALID_INST;
      }
      return nullptr;
    }

    template<typename F>
    inline
    t_void each(p_entry _entry, t_n_ max, F f) {
      for (t_id_ id = 0; id < max; ++id) {
        r_entry entry = _entry[id];
        if (entry.free_ == max + 1)
          f(t_id{id + 1}, entry.store_.ref());
      }
    }

    template<typename F>
    inline
    t_void each(r_err err, p_entry _entry, t_n_ max, F f) {
      ERR_GUARD(err) {
        if (_entry) {
          for (t_id_ id = 0; id < max; ++id) {
            r_entry entry = _entry[id];
            if (entry.free_ == max + 1)
              f(t_id{id + 1}, entry.store_.ref());
          }
        } else
          err = err::E_INVALID_INST;
      }
    }

    template<typename F>
    inline
    t_void each(P_entry _entry, t_n_ max, F f) const {
      for (t_id_ id = 0; id < max; ++id) {
        R_entry entry = _entry[id];
        if (entry.free_ == max + 1)
          f(t_id{id + 1}, entry.store_.cref());
      }
    }

    template<typename F>
    inline
    t_void each(r_err err, P_entry _entry, t_n_ max, F f) const {
      ERR_GUARD(err) {
        if (_entry) {
          for (t_id_ id = 0; id < max; ++id) {
            R_entry entry = _entry[id];
            if (entry.free_ == max + 1)
              f(t_id{id + 1}, entry.store_.cref());
          }
        } else
          err = err::E_INVALID_INST;
      }
    }

    template<typename F>
    inline
    t_id_ find_if(P_entry _entry, t_n_ max, F& f) const {
      for (t_id_ id = 0; id < max; ++id) {
        R_entry entry = _entry[id];
        if ((entry.free_ == max + 1) && f(entry.store_.cref()))
          return id + 1;
      }
      return BAD_ID_;
    }

    template<typename F>
    inline
    t_id_ find_if(r_err err, P_entry entry, t_n_ max, F& f) const {
      ERR_GUARD(err) {
        if (entry)
          return find_if(entry, max, f);
        err = err::E_INVALID_INST;
      }
      return BAD_ID_;
    }

  private:
    t_n_ size_;
    t_n_ free_;
  };

///////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
