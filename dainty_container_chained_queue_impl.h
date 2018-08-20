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

#ifndef _DAINTY_CONTAINER_CHAINED_QUEUE_IMPL_H_
#define _DAINTY_CONTAINER_CHAINED_QUEUE_IMPL_H_

#include "dainty_named.h"
#include "dainty_container_err.h"

namespace dainty
{
namespace container
{
namespace chained_queue
{
  using named::t_void;
  using named::t_bool;
  using named::t_n_;
  using named::t_n;
  using freelist::t_id;

  using named::t_validity;
  using named::VALID;
  using named::INVALID;

///////////////////////////////////////////////////////////////////////////////

  template<typename T>
  class t_item final {
  public:
    using t_value  = typename named::t_prefix<T>::t_;
    using r_value  = typename named::t_prefix<T>::r_;
    using R_value  = typename named::t_prefix<T>::R_;
    using p_item   = typename named::t_prefix<t_item>::p_;
    using P_item   = typename named::t_prefix<t_item>::P_;

    t_item() = default;

    t_item(const t_item&)            = delete;
    t_item(t_item&&)                 = delete;
    t_item& operator=(const t_item&) = delete;
    t_item& operator=(t_item&&)      = delete;

    p_item  next()        { return next_;  }
    P_item  next() const  { return next_;  }
    P_item cnext() const  { return next_;  }

    r_value  ref()        { return value_; }
    R_value  ref() const  { return value_; }
    R_value cref() const  { return value_; }

  private:
    template<class>        friend class t_chain;
    template<class, class> friend class t_chained_queue_impl_;

    t_value value_;
    p_item  next_ = nullptr;
    t_id    id_   = t_id{0};
  };

  template<typename T>
  class t_chain final {
  public:
    using t_n    = chained_queue::t_n;
    using p_item = typename named::t_prefix<t_item<T> >::p_;
    using P_item = typename named::t_prefix<t_item<T> >::P_;

    t_n    cnt   = t_n{0};
    p_item head  = nullptr;
    p_item tail  = nullptr;

    template<typename F> t_void  each(       F);
    template<typename F> t_void  each(t_err, F);
    template<typename F> t_void  each(       F) const;
    template<typename F> t_void  each(t_err, F) const;
    template<typename F> t_void ceach(       F) const;
    template<typename F> t_void ceach(t_err, F) const;
  };

///////////////////////////////////////////////////////////////////////////////

  template<typename T, typename S>
  class t_chained_queue_impl_ {
  public:
    using t_n     = chained_queue::t_n;
    using t_store = typename named::t_prefix<S>::t_;
    using r_store = typename named::t_prefix<S>::r_;
    using R_store = typename named::t_prefix<S>::R_;
    using p_item  = typename named::t_prefix<t_item<T> >::p_;
    using t_chain = typename named::t_prefix<chained_queue::t_chain<T> >::t_;
    using r_chain = typename named::t_prefix<chained_queue::t_chain<T> >::r_;

    t_chain acquire(r_store store, t_n n) {
      if (store == VALID) {
        auto need = get(n);
        auto free = get(store.get_capacity()) - get(store.get_size());
        if (free >= need) {
          auto result = store.insert();
          p_item head = result.ptr;
          head->id_   = result.id;
          p_item tail = head;
          if (need > 1) {
            for (t_n_ cnt = 1; cnt < need; ++cnt) {
              result = store.insert();
              tail->next_ = result.ptr;
              tail        = tail->next_;
              tail->id_   = result.id;
            }
          }
          tail->next_ = nullptr;
          return {n, head, tail};
        }
      }
      return {};
    }

    t_chain acquire(t_err& err, r_store store, t_n n) {
      T_ERR_GUARD(err) {
        if (store == VALID) {
          auto need = get(n);
          auto free = get(store.get_capacity()) - get(store.get_size());
          if (free >= need) {
            auto result = store.insert();
            p_item head = result.ptr;
            head->id_   = result.id;
            p_item tail = head;
            if (need > 1) {
              for (t_n_ cnt = 1; cnt < need; ++cnt) {
                result = store.insert();
                tail->next_ = result.ptr;
                tail        = tail->next_;
                tail->id_   = result.id;
              }
            }
            tail->next_ = nullptr;
            return {n, head, tail};
          }
          err = E_NO_SPACE;
        } else
          err = E_INVALID_INST;
      }
      return {};
    }

    t_void release(r_store store, r_chain chain) {
      if (store == VALID) {
        for (p_item item = chain.head; item; item = item->next_)
          store.erase(item->id_);
      }
    }

    t_void release(t_err& err, r_store store, t_chain& chain) {
      T_ERR_GUARD(err) {
        if (store == VALID) {
          for (p_item item = chain.head; item; item = item->next_)
            store.erase(item->id_);
        }
      }
    }

    t_void insert(r_store store, r_chain chain) {
      if (store == VALID) {
        if (get(chain.cnt)) {
          if (get(chain_.cnt))
            chain_.tail->next_ = chain.head;
          else
            chain_.head = chain.head;
          chain_.tail =  chain.tail;
          set(chain_.cnt) += get(chain.cnt);
        }
      }
    }

    t_void insert(t_err& err, r_store store, r_chain chain) {
      T_ERR_GUARD(err) {
        if (store == VALID) {
          if (get(chain.cnt)) {
            if (get(chain_.cnt))
              chain_.tail->next_ = chain.head;
            else
              chain_.head = chain.head;
            chain_.tail =  chain.tail;
            set(chain_.cnt) += get(chain.cnt);
          }
        } else
          err = E_INVALID_INST;
      }
    }

    t_chain remove(r_store store, t_n n) {
      if (store == VALID) {
        auto has_cnt = get(chain_.cnt);
        if (has_cnt) {
          auto cnt = get(n);
          if (!cnt || cnt >= has_cnt) {
            t_chain tmp = chain_;
            set(chain_.cnt) = 0;
            return tmp;
          }
          p_item head = chain_.head;
          p_item tail = head;
          for (t_n_ i = 1; i < cnt ; ++i)
            tail = tail->next_;
          chain_.head =  tail->next_;
          tail->next_ = nullptr;
          set(chain_.cnt) -= cnt;
          return {n, head, tail};
        }
      }
      return {};
    }

    t_chain remove(t_err& err, r_store store, t_n n) {
      T_ERR_GUARD(err) {
        if (store == VALID) {
          auto has_cnt = get(chain_.cnt);
          if (has_cnt) {
            auto cnt = get(n);
            if (!cnt || cnt >= has_cnt) {
              t_chain tmp = chain_;
              set(chain_.cnt) = 0;
              return tmp;
            }
            p_item head = chain_.head;
            p_item tail = head;
            for (t_n_ i = 1; i < cnt ; ++i)
              tail = tail->next_;
            chain_.head =  tail->next_;
            tail->next_ = nullptr;
            set(chain_.cnt) -= cnt;
            return {n, head, tail};
          }
        } else
          err = E_INVALID_INST;
      }
      return {};
    }

    p_item get_tail(R_store) {
      return get(chain_.cnt) ? chain_.tail : nullptr;
    }

    t_bool is_empty(R_store) const {
      return !get(chain_.cnt);
    }

    t_bool is_full(R_store store) const {
      return get(store.get_capacity()) == get(chain_.cnt);
    }

    t_n get_capacity(R_store store) const {
      return store.get_capacity();
    }

    t_n get_size(R_store store) const {
      return chain_.cnt;
    }

  private:
    t_chain chain_;
  };

///////////////////////////////////////////////////////////////////////////////

  template<typename T>
  template<typename F>
  inline
  t_void t_chain<T>::each(F f) {
    for (p_item item = head; item; item = item->next_)
      f(item->ref());
  }

  template<typename T>
  template<typename F>
  inline
  t_void t_chain<T>::each(t_err err, F f) {
    T_ERR_GUARD(err) {
      for (p_item item = head; item; item = item->next_)
        f(item->ref());
    }
  }

  template<typename T>
  template<typename F>
  inline
  t_void t_chain<T>::each(F f) const {
    for (P_item item = head; item; item = item->next_)
      f(item->cref());
  }

  template<typename T>
  template<typename F>
  inline
  t_void t_chain<T>::each(t_err err, F f) const {
    T_ERR_GUARD(err) {
      for (P_item item = head; item; item = item->next_)
        f(item->cref());
    }
  }

  template<typename T>
  template<typename F>
  inline
  t_void t_chain<T>::ceach(F f) const {
    for (P_item item = head; item; item = item->next_)
      f(item->cref());
  }

  template<typename T>
  template<typename F>
  inline
  t_void t_chain<T>::ceach(t_err err, F f) const {
    T_ERR_GUARD(err) {
      for (P_item item = head; item; item = item->next_)
        f(item->cref());
    }
  }
}
}
}

#endif
