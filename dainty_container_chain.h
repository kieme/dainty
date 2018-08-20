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

#ifndef _DAINTY_CONTAINER_CHAIN_H_
#define _DAINTY_CONTAINER_CHAIN_H_

#include "dainty_named.h"
#include "dainty_container_err.h"

namespace dainty
{
namespace container
{
namespace chain
{
  using named::t_void;
  using named::t_n;

  using named::t_validity;
  using named::VALID;
  using named::INVALID;

///////////////////////////////////////////////////////////////////////////////

  template<typename T>
  class t_item final {
  public:
    using t_value = typename named::t_prefix<T>::t_;
    using p_item  = typename named::t_prefix<t_item>::p_;
    using P_item  = typename named::t_prefix<t_item>::P_;

    t_item() = default;

    t_item(const t_item&)            = delete;
    t_item(t_item&&)                 = delete;
    t_item& operator=(const t_item&) = delete;
    t_item& operator=(t_item&&)      = delete;

    t_value value;
    p_item  next = nullptr;
  };

  template<typename T>
  class t_chain final {
  public:
    using t_n    = chain::t_n;
    using p_item = typename named::t_prefix<t_item<T> >::p_;
    using P_item = typename named::t_prefix<t_item<T> >::P_;

    operator t_validity() const;

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

  template<typename T>
  inline
  t_chain<T>::operator t_validity() const {
    return get(cnt) ? VALID : INVALID;
  }

  template<typename T>
  template<typename F>
  inline
  t_void t_chain<T>::each(F f) {
    for (p_item item = head; item; item = item->next)
      f(item->value);
  }

  template<typename T>
  template<typename F>
  inline
  t_void t_chain<T>::each(t_err err, F f) {
    T_ERR_GUARD(err) {
      for (p_item item = head; item; item = item->next)
        f(item->value);
    }
  }

  template<typename T>
  template<typename F>
  inline
  t_void t_chain<T>::each(F f) const {
    for (P_item item = head; item; item = item->next)
      f(item->value);
  }

  template<typename T>
  template<typename F>
  inline
  t_void t_chain<T>::each(t_err err, F f) const {
    T_ERR_GUARD(err) {
      for (P_item item = head; item; item = item->next)
        f(item->value);
    }
  }

  template<typename T>
  template<typename F>
  inline
  t_void t_chain<T>::ceach(F f) const {
    for (P_item item = head; item; item = item->next)
      f(item->value);
  }

  template<typename T, typename I>
  template<typename F>
  inline
  t_void t_chain<T>::ceach(t_err err, F f) const {
    T_ERR_GUARD(err) {
      for (P_item item = head; item; item = item->next)
        f(item->value);
    }
  }

///////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
