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

#include <utility>
#include "dainty_named.h"

namespace dainty
{
namespace container
{
namespace holder
{
  using named::t_validity;
  using named::VALID;
  using named::INVALID;

  enum  t_user_tag_ { };
  using t_user = named::t_user<t_user_tag_>;

///////////////////////////////////////////////////////////////////////////////

  struct t_it_ { virtual ~t_it_() {} };

  template<class T>
  struct t_store_ final : t_it_ {
    T value_;

    template<class U> t_store_(U&& u) : value_(std::forward<U>(u)) { }
    t_store_()                           = delete;
    t_store_(const t_store_&)            = delete;
    t_store_& operator=(const t_store_&) = delete;
    t_store_(t_store_&&)                 = delete;
    t_store_& operator=(t_store_&&)      = delete;
  };

///////////////////////////////////////////////////////////////////////////////

  class t_holder final {
  public:
     t_holder() = default;
     t_holder(t_holder&&);
    ~t_holder();

    t_holder& operator=(t_holder&&);
    t_holder(const t_holder&)            = delete;
    t_holder& operator=(const t_holder&) = delete;

    operator t_validity() const;

    template<typename T, typename... Args>
    T& emplace(Args&&... args);

    template<class T> T&        ref();
    template<class T> const T&  ref() const;
    template<class T> const T& cref() const;

  private:
    t_it_* store_ = nullptr;
  };

////////////////////////////////////////////////////////////////////////////////

  inline
  t_holder::t_holder(t_holder&& holder) : store_{holder.store_} {
    holder.store_ = nullptr;
  }

  inline
  t_holder::~t_holder() {
    if (store_)
      delete store_;
  }

  inline
  t_holder& t_holder::operator=(t_holder&& holder) {
    if (store_)
      delete store_;
    store_ = holder.store_;
    holder.store_ = nullptr;
    return *this;
  }

  inline
  t_holder::operator t_validity () const {
    return store_ ? VALID : INVALID;
  }

  template<typename T, typename... Args>
  inline
  T& t_holder::emplace(Args&&... args) {
    if (store_)
      delete store_;
    store_ = new t_store_<T>(std::forward<Args>(args)...);
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
