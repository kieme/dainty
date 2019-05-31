/******************************************************************************

 MIT License

 Copyright (c) 2018 kieme, frits.germs@gmx.net

 Permission is hereby granted, free of charge, to holder person obtaining a copy
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

#ifndef _DAINTY_CONTAINER_HOLDER_H_
#define _DAINTY_CONTAINER_HOLDER_H_

#include "dainty_named.h"
#include "dainty_named_ptr.h"

namespace dainty
{
namespace container
{
namespace holder
{
  using named::t_validity;
  using named::VALID;
  using named::INVALID;

///////////////////////////////////////////////////////////////////////////////

  struct t_it_ {
    virtual ~t_it_() {}
  };
  using p_it_ = t_prefix<t_it_>::p_;

  using t_ptr_ = named::ptr::t_ptr<t_it_, t_it_, named::ptr::t_deleter>;

  template<class T>
  struct t_store_ final : t_it_ {
    using R_store_ = typename t_prefix<t_store_>::R_;
    using x_store_ = typename t_prefix<t_store_>::x_;
    using r_store_ = typename t_prefix<t_store_>::r_;

    T value_;

    template<class... Args> t_store_(Args&&... args)
      : value_(named::preserve<Args>(args)...) {
    }

    t_store_()                   = delete;
    t_store_(R_store_)           = delete;
    t_store_(x_store_)           = delete;
    r_store_ operator=(R_store_) = delete;
    r_store_ operator=(x_store_) = delete;
  };

///////////////////////////////////////////////////////////////////////////////

  class t_holder;
  using r_holder = t_prefix<t_holder>::r_;
  using x_holder = t_prefix<t_holder>::x_;
  using R_holder = t_prefix<t_holder>::R_;

  class t_holder final {
  public:
    template<typename T, typename... Args>
    t_holder construct(Args&&...);

    t_holder() = default;
    template<typename T>
    t_holder(T&&);
    t_holder(x_holder);

    r_holder operator=(x_holder);

    t_holder(R_holder)           = delete;
    r_holder operator=(R_holder) = delete;

    operator t_validity() const;

    template<typename T, typename... Args>
    T& emplace(Args&&... args);

    template<class T> T&        ref();
    template<class T> const T&  ref() const;
    template<class T> const T& cref() const;

  private:
    t_holder(p_it_);

    t_ptr_ store_;
  };

////////////////////////////////////////////////////////////////////////////////

  template<typename T, typename... Args>
  inline
  t_holder t_holder::construct(Args&&... args) {
    return {static_cast<p_it>(new t_store_<T>(named::preserve<Args>(args)...))};
  }

  inline
  t_holder::t_holder(p_it_ store) : store_{store} {
  }

  inline
  t_holder::t_holder(x_holder holder) : store_{reset(holder.store_)} {
  }

  template<typename T>
  inline
  t_holder::t_holder(T&& value)
    : store_{new t_store_<T>(named::preserve<T>(value))} {
  }

  inline
  r_holder t_holder::operator=(x_holder holder) {
    store_ = named::x_cast(holder.store_);
    return *this;
  }

  inline
  t_holder::operator t_validity () const {
    return store_ ? VALID : INVALID;
  }

  template<typename T, typename... Args>
  inline
  T& t_holder::emplace(Args&&... args) {
    store_ = new t_store_<T>(named::preserve<Args>(args)...);
    return ref<T>();
  }

  template<typename T>
  inline
  T& t_holder::ref() {
    return (static_cast<t_store_<T>*>(store_))->value_;
  }

  template<typename T>
  inline
  const T& t_holder::ref() const {
    return (static_cast<const t_store_<T>*>(store_))->value_;
  }

  template<typename T>
  inline
  const T& t_holder::cref() const {
    return (static_cast<const t_store_<T>*>(store_))->value_;
  }

////////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
