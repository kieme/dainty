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

#ifndef _DAINTY_CONTAINER_ANY_H_
#define _DAINTY_CONTAINER_ANY_H_

#include <utility>
#include "dainty_named.h"

namespace dainty
{
namespace container
{
namespace any
{
  using named::t_void;
  using named::t_bool;
  using named::t_validity;
  using named::VALID;
  using named::INVALID;

  enum  t_user_tag_ { };
  using t_user = named::t_user<t_user_tag_>;

///////////////////////////////////////////////////////////////////////////////

  struct t_it_ {
    virtual ~t_it_() {}
    virtual t_bool is_equal(const t_it_&) const = 0;
    virtual t_void copy(const t_it_&) = 0;
    virtual t_it_* clone() const = 0;
  };

  t_bool same_type_(const t_it_&, const t_it_& it);

  template<class T>
  struct t_store_ final : t_it_ {
    T value_;

    template<class... Args> t_store_(Args&&... args)
      : value_(std::forward<Args>(args)...) {
    }

    t_store_(const t_store_&)            = delete;
    t_store_& operator=(const t_store_&) = delete;
    t_store_(t_store_&&)                 = delete;
    t_store_& operator=(t_store_&&)      = delete;

    virtual t_bool is_equal(const t_it_& it) const override {
      return same_type_(*this, it) &&
             value_ == static_cast<const t_store_<T>&>(it).value_;
    };

    virtual t_void copy(const t_it_& it) {
      value_ = static_cast<const t_store_<T>&>(it).value_;
    };

    virtual t_it_* clone() const {
      return new t_store_<T>(value_);
    };
  };

///////////////////////////////////////////////////////////////////////////////

  class t_any final {
  public:
     t_any() = default;
     t_any(const t_any&);
     t_any(t_any&&);
     explicit t_any(t_user);
     template<typename T> t_any(t_user, T&&);
    ~t_any();

    t_any& operator=(const t_any&);
    t_any& operator=(t_any&&);

    t_any& assign(t_user);

    template<typename T, typename... Args>
    T& emplace(t_user, Args&&... args);

    operator t_validity() const;
    t_user   get_user  () const;
    t_bool   same_type (const t_any&) const;
    t_bool   is_equal  (const t_any&) const;

    template<class T> T&        ref();
    template<class T> const T&  ref() const;
    template<class T> const T& cref() const;

  private:
    t_user user_  = t_user{0L};
    t_it_* store_ = nullptr;
  };

////////////////////////////////////////////////////////////////////////////////

  inline
  t_bool operator==(const t_any& lh, const t_any& rh) {
    return lh.is_equal(rh);
  }

  inline
  t_bool operator!=(const t_any& lh, const t_any& rh) {
    return !lh.is_equal(rh);
  }

////////////////////////////////////////////////////////////////////////////////

  inline
  t_any::t_any(t_user user) : user_{user} {
  }

  template<typename T>
  inline
  t_any::t_any(t_user user, T&& value)
    : user_{user}, store_(new t_store_<T>(std::forward<T>(value))) {
  }

  inline
  t_any::t_any(const t_any& any) : user_{any.user_}, store_{any.store_} {
    if (store_)
      store_ = store_->clone();
  }

  inline
  t_any::t_any(t_any&& any) : user_{any.user_}, store_{any.store_} {
    any.store_   = nullptr;
    any.user_.id = 0;
  }

  inline
  t_any::operator t_validity () const {
    return store_ ? VALID : INVALID;
  }

  inline
  t_user t_any::get_user() const {
    return user_;
  }

  template<typename T, typename... Args>
  inline
  T& t_any::emplace(t_user user, Args&&... args) {
    if (store_)
      delete store_;
    user_  = user;
    store_ = new t_store_<T>(std::forward<Args>(args)...);
    return ref<T>();
  }

  template<typename T>
  inline
  T& t_any::ref() {
    return (static_cast<t_store_<T>*>(store_))->value_;
  }

  template<typename T>
  inline
  const T& t_any::ref() const {
    return (static_cast<const t_store_<T>*>(store_))->value_;
  }

  template<typename T>
  inline
  const T& t_any::cref() const {
    return (static_cast<const t_store_<T>*>(store_))->value_;
  }

////////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
