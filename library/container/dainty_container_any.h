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

#include "dainty_base.h"
#include "dainty_base_ptr.h"

namespace dainty
{
namespace container
{
namespace any
{
///////////////////////////////////////////////////////////////////////////////

  using base::types::t_prefix;
  using base::types::t_void;
  using base::types::t_bool;

  using base::specific::t_validity;
  using base::specific::VALID;
  using base::specific::INVALID;

///////////////////////////////////////////////////////////////////////////////

  enum  t_user_tag_ { };
  using t_user = base::t_user<t_user_tag_>;

///////////////////////////////////////////////////////////////////////////////

  class t_type_;
  using R_type_ = t_prefix<t_type_>::R_;
  using p_type_ = t_prefix<t_type_>::p_;

  class t_type_ {
  public:
    using t_ptr_ = base::ptr::t_ptr<t_type_, t_type_, base::ptr::t_deleter>;

    virtual ~t_type_() {}
    virtual t_bool is_equal(R_type_) const = 0;
    virtual t_void copy(R_type_) = 0;
    virtual t_ptr_ clone() const = 0;
  };

  t_bool same_type_(R_type_, R_type_);

  template<class T>
  struct t_store_ final : public t_type_ {
    using R_store_ = typename t_prefix<t_store_>::R_;
    using x_store_ = typename t_prefix<t_store_>::x_;
    using r_store_ = typename t_prefix<t_store_>::r_;

    T value_;

    template<typename... Args> t_store_(Args&&... args)
      : value_(base::f_cast<Args>(args)...) {
    }

    t_store_(R_store_)           = delete;
    r_store_ operator=(R_store_) = delete;
    t_store_(x_store_)           = delete;
    r_store_ operator=(x_store_) = delete;

    virtual t_bool is_equal(R_type_ it) const override {
      return same_type_(*this, it) &&
             value_ == static_cast<R_store_>(it).value_;
    }

    virtual t_void copy(R_type_ it) override {
      value_ = static_cast<R_store_>(it).value_;
    }

    virtual t_ptr_ clone() const override {
      return {new t_store_<T>(value_)};
    }
  };

///////////////////////////////////////////////////////////////////////////////

  class t_any;
  using r_any = t_prefix<t_any>::r_;
  using x_any = t_prefix<t_any>::x_;
  using R_any = t_prefix<t_any>::R_;

  class t_any final {
  public:
    template<typename T, typename... Args>
    static t_any construct(t_user, Args&&...);

    t_any() = default;
    t_any(R_any);
    t_any(x_any);
    t_any(t_user);
    template<typename T>
    t_any(t_user, T&&);

    r_any operator=(R_any);
    r_any operator=(x_any);

    r_any assign(t_user);

    template<typename T, typename... Args>
    T& emplace(t_user, Args&&... args);

    operator t_validity() const;
    t_user   get_user  () const;
    t_bool   same_type (R_any) const;
    t_bool   is_equal  (R_any) const;

    template<class T>       T&  ref();
    template<class T> const T&  ref() const;
    template<class T> const T& cref() const;

  private:
    t_any(t_user, p_type_ store);
    t_user          user_  = t_user{0L};
    t_type_::t_ptr_ store_;
  };

////////////////////////////////////////////////////////////////////////////////

  inline t_bool operator==(R_any lh, R_any rh) {
    return lh.is_equal(rh);
  }

  inline t_bool operator!=(R_any lh, R_any rh) {
    return !lh.is_equal(rh);
  }

////////////////////////////////////////////////////////////////////////////////

  template<typename T, typename... Args>
  inline
  t_any t_any::construct(t_user user, Args&&... args) {
    return {user,
            static_cast<p_type_>(new t_store_<T>(base::f_cast<Args>(args)...))};
  }

  inline
  t_any::t_any(t_user user) : user_{user} {
  }

  inline
  t_any::t_any(t_user user, p_type_ store) : user_{user}, store_{store} {
  }

  template<typename T>
  inline
  t_any::t_any(t_user user, T&& value)
    : user_{user}, store_{new t_store_<T>(base::f_cast<T>(value))} {
  }

  inline
  t_any::t_any(R_any any)
    : user_{any.user_},
      store_{any.store_ == VALID ? any.store_->clone() : nullptr} {
  }

  inline
  t_any::t_any(x_any any) : user_{any.user_}, store_{base::x_cast(any.store_)} {
    base::reset(any.user_);
  }

  inline
  t_any::operator t_validity () const {
    return store_;
  }

  inline
  t_user t_any::get_user() const {
    return user_;
  }

  template<typename T, typename... Args>
  inline
  T& t_any::emplace(t_user user, Args&&... args) {
    user_  = user;
    store_ = new t_store_<T>(base::f_cast<Args>(args)...);
    return ref<T>();
  }

  template<typename T>
  inline
  T& t_any::ref() {
    return (static_cast<t_store_<T>*>(store_.get()))->value_;
  }

  template<typename T>
  inline
  const T& t_any::ref() const {
    return (static_cast<const t_store_<T>*>(store_.get()))->value_;
  }

  template<typename T>
  inline
  const T& t_any::cref() const {
    return (static_cast<const t_store_<T>*>(store_.get()))->value_;
  }

////////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
