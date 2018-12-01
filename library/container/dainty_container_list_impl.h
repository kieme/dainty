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

#ifndef _DAINTY_CONTAINER_LIST_IMPL_H_
#define _DAINTY_CONTAINER_LIST_IMPL_H_

#include "dainty_named.h"
#include "dainty_container_err.h"
#include "dainty_container_valuestore.h"

namespace dainty
{
namespace container
{
namespace list
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
  class t_list_impl_ {
  public:
    using t_entry = typename named::t_prefix<valuestore::t_valuestore<T>>::t_;
    using p_store = typename named::t_prefix<t_entry>::p_;
    using P_store = typename named::t_prefix<t_entry>::P_;
    using t_value = typename named::t_prefix<T>::t_;
    using p_value = typename named::t_prefix<T>::p_;
    using P_value = typename named::t_prefix<T>::P_;
    using r_value = typename named::t_prefix<T>::r_;
    using R_value = typename named::t_prefix<T>::R_;
    using x_value = typename named::t_prefix<T>::x_;

    inline
    t_list_impl_() : next_(0) {
    }

    inline
    p_value push_back(p_store store, t_n_ max) {
      if (next_ < max)
        return store[next_++].default_construct();
      return nullptr;
    }

    inline
    p_value push_back(r_err err, p_store store, t_n_ max) {
      ERR_GUARD(err) {
        if (store) {
          if (next_ < max)
            return store[next_++].default_construct();
          err =  E_NO_SPACE;
        } else
          err = E_INVALID_INST;
      }
      return nullptr;
    }

    inline
    p_value push_back(p_store store, t_n_ max, R_value value) {
      if (next_ < max)
        return store[next_++].copy_construct(value);
      return nullptr;
    }

    inline
    p_value push_back(r_err err, p_store store, t_n_ max, R_value value) {
      ERR_GUARD(err) {
        if (store) {
          if (next_ < max)
            return store[next_++].copy_construct(value);
          err =  E_NO_SPACE;
        } else
          err = E_INVALID_INST;
      }
      return nullptr;
    }

    inline
    p_value push_back(p_store store, t_n_ max, x_value value) {
      if (next_ < max)
        return store[next_++].move_construct(std::move(value));
      return nullptr;
    }

    inline
    p_value push_back(r_err err, p_store store, t_n_ max, x_value value) {
      ERR_GUARD(err) {
        if (store) {
          if (next_ < max)
            return store[next_++].move_construct(std::move(value));
          err =  E_NO_SPACE;
        } else
          err = E_INVALID_INST;
      }
      return nullptr;
    }

    inline
    p_value insert(p_store store, t_n_ max, t_ix_ ix) {
      if (next_ < max) {
        if (ix < next_) {
          move_up_(store, ix, next_++);
          return store[ix].default_construct();
        } else if ((!next_ && !ix) || (ix == next_))
          return push_back(store, max);
      }
      return nullptr;
    }

    inline
    p_value insert(r_err err, p_store store, t_n_ max, t_ix_ ix) {
      ERR_GUARD(err) {
        if (store) {
          if (next_ < max) {
            if (ix < next_) {
              move_up_(store, ix, next_++);
              return store[ix].default_construct();
            } else if ((!next_ && !ix) || (ix == next_))
              return push_back(store, max);
            err = E_INVALID_IX;
          } else
            err = E_NO_SPACE;
        } else
          err = E_INVALID_INST;
      }
      return nullptr;
    }

    inline
    p_value insert(p_store store, t_n_ max, t_ix_ ix, R_value value) {
      if (next_ < max) {
        if (ix < next_) {
          move_up_(store, ix, next_++);
          return store[ix].copy_construct(value);
        } else if ((!next_ && !ix) || (ix == next_))
          return push_back(store, max, value);
      }
      return nullptr;
    }

    inline
    p_value insert(r_err err, p_store store, t_n_ max, t_ix_ ix,
                   R_value value) {
      ERR_GUARD(err) {
        if (store) {
          if (next_ < max) {
            if (ix < next_) {
              move_up_(store, ix, next_++);
              return store[ix].copy_construct(value);
            } else if ((!next_ && !ix) || (ix == next_))
              return push_back(store, max, value);
            err = E_INVALID_IX;
          } else
            err = E_NO_SPACE;
        } else
          err = E_INVALID_INST;
      }
      return nullptr;
    }

    inline
    p_value insert(p_store store, t_n_ max, t_ix_ ix, x_value value) {
      if (next_ < max) {
        if (ix < next_) {
          move_up_(store, ix, next_++);
          return store[ix].move_construct(std::move(value));
        } else if ((!next_ && !ix) || (ix == next_))
          return push_back(store, max, std::move(value));
      }
      return nullptr;
    }

    inline
    p_value insert(r_err err, p_store store, t_n_ max, t_ix_ ix,
                   x_value value) {
      ERR_GUARD(err) {
        if (store) {
          if (next_ < max) {
            if (ix < next_) {
              move_up_(store, ix, next_++);
              return store[ix].move_construct(std::move(value));
            } else if ((!next_ && !ix) || (ix == next_))
              return push_back(store, max, std::move(value));
            err = E_INVALID_IX;
          } else
            err = E_NO_SPACE;
        } else
          err = E_INVALID_INST;
      }
      return nullptr;
    }

    inline
    t_bool pop_back(p_store store) {
      if (next_) {
        store[--next_].destruct();
        return true;
      }
      return false;
    }

    inline
    t_bool pop_back(r_err err, p_store store) {
      ERR_GUARD(err) {
        if (store) {
          if (next_) {
            store[--next_].destruct();
            return true;
          }
          err = E_NO_POP;
        } else
          err = E_INVALID_INST;
      }
      return false;
    }

    inline
    t_bool erase(p_store store, t_ix_ ix) {
      if (ix < next_) {
        store[ix].destruct();
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
            store[ix].destruct();
            move_down_(store, ix, --next_);
            return true;
          }
          err = E_INVALID_IX;
        } else
          err = E_INVALID_INST;
      }
      return false;
    }

    inline
    t_void clear(p_store store) {
      if (next_) {
        valuestore::destruct_(store, t_n{next_});
        next_ = 0;
      }
    }

    inline
    t_void clear(r_err err, p_store store) {
      ERR_GUARD(err) {
        if (store) {
          if (next_) {
            valuestore::destruct_(store, t_n{next_});
            next_ = 0;
          }
        } else
          err = E_INVALID_INST;
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
        return store[ix].ptr();
      return nullptr;
    }

    inline
    p_value get(r_err err, p_store store, t_ix_ ix) {
      ERR_GUARD(err) {
        if (store) {
          if (ix < next_)
            return store[ix].ptr();
          err = E_INVALID_IX;
        } else
          err = E_INVALID_INST;
      }
      return nullptr;
    }

    inline
    P_value get(P_store store, t_ix_ ix) const {
      if (ix < next_)
        return store[ix].cptr();
      return nullptr;
    }

    inline
    P_value get(r_err err, P_store store, t_ix_ ix) const {
      ERR_GUARD(err) {
        if (store) {
          if (ix < next_)
            return store[ix].cptr();
          err = E_INVALID_IX;
        } else
          err = E_INVALID_INST;
      }
      return nullptr;
    }

    template<typename F>
    inline
    t_void each(p_store store, F f) {
      for (t_ix_ ix = 0; ix < next_; ++ix)
        f(store[ix].ref());
    }

    template<typename F>
    inline
    t_void each(r_err err, p_store store, F f) {
      ERR_GUARD(err) {
        if (store) {
          for (t_ix_ ix = 0; ix < next_; ++ix)
            f(store[ix].ref());
        } else
          err = E_INVALID_INST;
      }
    }

    template<typename F>
    inline
    t_void each(P_store store, F f) const {
      for (t_ix_ ix = 0; ix < next_; ++ix)
        f(store[ix].cref());
    }

    template<typename F>
    inline
    t_void each(r_err err, P_store store, F f) const {
      ERR_GUARD(err) {
        if (store) {
          for (t_ix_ ix = 0; ix < next_; ++ix)
            f(store[ix].cref());
        } else
          err = E_INVALID_INST;
      }
    }

  private:
    inline
    t_void move_up_(p_store store, t_ix_ ix, t_ix_ max) {
      for (t_ix_ i = max; i > ix; /**/) {
        const t_ix_ x = i--;
        store[x].move_construct(std::move(store[i].ref()));
        store[i].destruct();
      }
    }

    inline
    t_void move_down_(p_store store, t_ix_ ix, t_ix_ max) {
      for (t_ix_ i = ix; i < max; /**/) {
        const t_ix_ x = i++;
        store[x].move_construct(std::move(store[i].ref()));
        store[i].destruct();
      }
    }

    t_n_ next_;
  };
}
}
}

#endif
