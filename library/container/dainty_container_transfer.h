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

#ifndef _DAINTY_CONTAINER_TRANSFER_H_
#define _DAINTY_CONTAINER_TRANSFER_H_

#include "dainty_named_utility.h"
#include "dainty_container_maybe.h"

namespace dainty
{
namespace container
{
namespace transfer
{
  using named::t_errn;
  using named::t_validity;
  using named::t_emplace_it;
  using named::EMPLACE_IT;
  using named::VALID;
  using named::INVALID;
  using named::NO_ERRN;
  using named::BAD_ERRN;
  using named::utility::x_cast;
  using named::utility::preserve;

///////////////////////////////////////////////////////////////////////////////

  template<typename T>
  class t_transfer {
  public:
    using t_value    = typename named::t_prefix<T>::t_;
    using x_value    = typename named::t_prefix<T>::x_;
    using r_transfer = typename named::t_prefix<t_transfer>::r_;
    using R_transfer = typename named::t_prefix<t_transfer>::R_;
    using x_transfer = typename named::t_prefix<t_transfer>::x_;

    t_transfer() = default;
    template<typename... Args>
    t_transfer(t_emplace_it, Args&&...);
    t_transfer(x_value);
    t_transfer(x_transfer);

    t_transfer(R_transfer)           = delete;
    r_transfer operator=(R_transfer) = delete;

    operator t_validity() const noexcept;

    T release() noexcept;

  private:
    template<typename T1> friend       T1& set(      t_transfer<T1>&) noexcept;
    template<typename T1> friend const T1& get(const t_transfer<T1>&) noexcept;

    maybe::t_maybe<T> maybe_;
  };

///////////////////////////////////////////////////////////////////////////////

  template<typename T>
  inline T& set(t_transfer<T>& transfer) noexcept {
     // XXX assert when invalid
    return set(transfer.maybe_);
  }

  template<typename T>
  inline const T& get(const t_transfer<T>& transfer) noexcept {
     // XXX assert when invalid
    return get(transfer.maybe_);
  }

///////////////////////////////////////////////////////////////////////////////

  template<typename T>
  class t_errn_transfer {
  public:
    using t_value         = typename named::t_prefix<T>::t_;
    using x_value         = typename named::t_prefix<T>::x_;
    using r_errn_transfer = typename named::t_prefix<t_errn_transfer>::r_;
    using R_errn_transfer = typename named::t_prefix<t_errn_transfer>::R_;
    using x_errn_transfer = typename named::t_prefix<t_errn_transfer>::x_;

    template<typename... Args>
    t_errn_transfer(t_emplace_it, Args&&...);
    t_errn_transfer(t_errn) noexcept;
    t_errn_transfer(x_value);
    t_errn_transfer(x_errn_transfer);

    t_errn_transfer(R_errn_transfer)           = delete;
    r_errn_transfer operator=(R_errn_transfer) = delete;

    operator t_errn() const noexcept;

    T release() noexcept;

  private:
    template<typename T1>
    friend       T1& set(      t_errn_transfer<T1>&) noexcept;
    template<typename T1>
    friend const T1& get(const t_errn_transfer<T1>&) noexcept;

    maybe::t_maybe<T> maybe_;
    t_errn            errn_;
  };

///////////////////////////////////////////////////////////////////////////////

  template<typename T>
  inline T& set(t_errn_transfer<T>& transfer) noexcept {
    return set(transfer.maybe_);
  }

  template<typename T>
  inline const T& get(const t_errn_transfer<T>& transfer) noexcept {
    return get(transfer.maybe_);
  }

///////////////////////////////////////////////////////////////////////////////

  template<typename T>
  template<typename... Args>
  inline
  t_transfer<T>::t_transfer(t_emplace_it, Args&&... args)
    : maybe_{EMPLACE_IT, preserve<Args>(args)...} {
  }

  template<typename T>
  inline
  t_transfer<T>::t_transfer(x_value value)
    : maybe_{x_cast(value)} {
  }

  template<typename T>
  inline
  t_transfer<T>::t_transfer(x_transfer transfer)
    : maybe_{x_cast(transfer.maybe_)} {
  }

  template<typename T>
  inline
  t_transfer<T>::operator t_validity() const noexcept {
    return maybe_;
  }

  template<typename T>
  inline
  T t_transfer<T>::release() noexcept {
    //if (maybe_ == INVALID)
    // assert

    T tmp(x_cast(set(maybe_)));
    maybe_.release();
    return tmp;
  }

///////////////////////////////////////////////////////////////////////////////

  template<typename T>
  template<typename... Args>
  inline
  t_errn_transfer<T>::t_errn_transfer(t_emplace_it, Args&&... args)
    : maybe_{EMPLACE_IT, preserve<Args>(args)...}, errn_{NO_ERRN} {
  }

  template<typename T>
  inline
  t_errn_transfer<T>::t_errn_transfer(t_errn errn) noexcept : errn_{errn} {
    if (errn_ == NO_ERRN) {
      // assert when zero!!!
      errn_ = BAD_ERRN; // make sure this object is not mistaken
    }
  }

  template<typename T>
  inline
  t_errn_transfer<T>::t_errn_transfer(x_value value)
    : maybe_{x_cast(value)}, errn_{NO_ERRN} {
  }

  template<typename T>
  inline
  t_errn_transfer<T>::t_errn_transfer(x_errn_transfer transfer)
      : errn_{transfer} {
    if (errn_ == NO_ERRN) {
      maybe_ = x_cast(transfer.maybe_);
      transfer.release();
    }
  }

  template<typename T>
  inline
  t_errn_transfer<T>::operator t_errn() const noexcept {
    return errn_;
  }

  template<typename T>
  inline T t_errn_transfer<T>::release() noexcept {
    //if (errn_ != NO_ERRN)
    // assert

    T tmp(x_cast(set(maybe_)));
    errn_ = BAD_ERRN;
    maybe_.release();
    return tmp;
  }

///////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
