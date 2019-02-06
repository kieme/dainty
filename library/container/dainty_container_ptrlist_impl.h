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

#ifndef _DAINTY_CONTAINER_PTRLIST_IMPL_H_
#define _DAINTY_CONTAINER_PTRLIST_IMPL_H_

#include "dainty_named.h"
#include "dainty_container_err.h"

namespace dainty
{
namespace container
{
namespace ptrlist
{
  using err::t_err;
  using err::r_err;

  using named::t_bool;
  using named::t_void;
  using named::t_n;
  using named::t_n_;
  using named::t_ix_;

///////////////////////////////////////////////////////////////////////////////

  template<typename T>
  class t_ptrlist_impl_ {
  public:
    using p_value = T*;
    using P_value = const T*;
    using t_entry = p_value;
    using p_store = t_entry*;
    using P_store = const t_entry*;

    inline
    t_ptrlist_impl_() : next_(0) {
    }

    inline
    t_bool push_back(p_store store, t_n_ max, p_value value) {
      if (next_ < max) {
        store[next_++] = value;
        return true;
      }
      return false;
    }

    inline
    t_bool push_back(r_err err, p_store store, t_n_ max, p_value value) {
      ERR_GUARD(err) {
        if (store) {
          if (next_ < max) {
            store[next_++] = value;
            return true;
          }
          err =  err::E_NO_SPACE;
        } else
          err = err::E_INVALID_INST;
      }
      return false;
    }

    inline
    t_bool insert(p_store store, t_n_ max, t_ix_ ix, p_value value) {
      if (next_ < max) {
        if (ix < next_) {
          move_up_(store, ix, next_++);
          store[ix] = value;
          return true;
        } else if ((!next_ && !ix) || (ix == next_))
          return push_back(store, max, value);
      }
      return false;
    }

    inline
    t_bool insert(r_err err, p_store store, t_n_ max, t_ix_ ix,
                  p_value value) {
      ERR_GUARD(err) {
        if (store) {
          if (next_ < max) {
            if (ix < next_) {
              move_up_(store, ix, next_++);
              store[ix] = value;
              return true;
            } else if ((!next_ && !ix) || (ix == next_))
              return push_back(store, max, value);
            err = err::E_INVALID_IX;
          } else
            err = err::E_NO_SPACE;
        } else
          err = err::E_INVALID_INST;
      }
      return false;
    }

    inline
    t_bool pop_back(p_store store) {
      if (next_) {
        --next_;
        return true;
      }
      return false;
    }

    inline
    t_bool pop_back(r_err err, p_store store) {
      ERR_GUARD(err) {
        if (store) {
          if (next_) {
            --next_;
            return true;
          }
          err = err::E_NO_POP;
        } else
          err = err::E_INVALID_INST;
      }
      return false;
    }

    inline
    t_bool erase(p_store store, t_ix_ ix) {
      if (ix < next_) {
        move_down_(store, ix, --next_);
        return true;
      }
      return false;
    }

    inline
    t_bool erase(r_err err, p_store store, t_ix_ ix) {
      ERR_GUARD(err) {
        if (store) {
          if (ix < next_) {
            move_down_(store, ix, --next_);
            return true;
          }
          err = err::E_INVALID_IX;
        } else
          err = err::E_INVALID_INST;
      }
      return false;
    }

    inline
    t_void clear(p_store store) {
      if (next_)
        next_ = 0;
    }

    inline
    t_void clear(r_err err, p_store store) {
      ERR_GUARD(err) {
        if (store) {
          if (next_)
            next_ = 0;
        } else
          err = err::E_INVALID_INST;
      }
    }

    inline
    t_bool is_full(t_n_ max) const {
      return next_ == max;
    }

    inline
    t_bool is_empty() const {
      return next_ == 0;
    }

    inline
    t_n_ get_size() const {
      return next_;
    }

    inline
    p_value get(p_store store, t_ix_ ix) {
      if (ix < next_)
        return store[ix];
      return nullptr;
    }

    inline
    p_value get(r_err err, p_store store, t_ix_ ix) {
      ERR_GUARD(err) {
        if (store) {
          if (ix < next_)
            return store[ix];
          err = err::E_INVALID_IX;
        } else
          err = err::E_INVALID_INST;
      }
      return nullptr;
    }

    inline
    P_value get(P_store store, t_ix_ ix) const {
      if (ix < next_)
        return store[ix];
      return nullptr;
    }

    inline
    P_value get(r_err err, P_store store, t_ix_ ix) const {
      ERR_GUARD(err) {
        if (store) {
          if (ix < next_)
            return store[ix];
          err = err::E_INVALID_IX;
        } else
          err = err::E_INVALID_INST;
      }
      return nullptr;
    }

    template<typename F>
    inline
    t_void each(p_store store, F f) {
      for (t_ix_ ix = 0; ix < next_; ++ix)
        f(store[ix]);
    }

    template<typename F>
    inline
    t_void each(r_err err, p_store store, F f) {
      ERR_GUARD(err) {
        if (store) {
          for (t_ix_ ix = 0; ix < next_; ++ix)
            f(store[ix]);
        } else
          err = err::E_INVALID_INST;
      }
    }

    template<typename F>
    inline
    t_void each(P_store store, F f) const {
      for (t_ix_ ix = 0; ix < next_; ++ix)
        f(store[ix]);
    }

    template<typename F>
    inline
    t_void each(r_err err, P_store store, F f) const {
      ERR_GUARD(err) {
        if (store) {
          for (t_ix_ ix = 0; ix < next_; ++ix)
            f(store[ix]);
        } else
          err = err::E_INVALID_INST;
      }
    }

  private:
    inline
    t_void move_up_(p_store store, t_ix_ ix, t_ix_ max) {
      for (t_ix_ i = max; i > ix; /**/) {
        const t_ix_ x = i--;
        store[x] = store[i];
      }
    }

    inline
    t_void move_down_(p_store store, t_ix_ ix, t_ix_ max) {
      for (t_ix_ i = ix; i < max; /**/) {
        const t_ix_ x = i++;
        store[x] = store[i];
      }
    }

    t_n_ next_;
  };
}
}
}

#endif
