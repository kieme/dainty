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

#ifndef _DAINTY_MT_CONDVAR_NOTIFY_CHANGE_H_
#define _DAINTY_MT_CONDVAR_NOTIFY_CHANGE_H_

#include "dainty_container_any.h"
#include "dainty_mt_err.h"

namespace dainty
{
namespace mt
{
namespace condvar_notify_change
{
  using named::t_n;
  using named::t_void;
  using named::t_errn;
  using named::t_prefix;
  using named::t_validity;
  using named::VALID;
  using named::INVALID;

  using err::t_err;
  using container::any::t_any;

  enum  t_user_tag_ { };
  using t_user = named::t_user<t_user_tag_>;

  class t_impl_;
  using p_impl_ = t_prefix<t_impl_>::p_;

///////////////////////////////////////////////////////////////////////////////

  class t_client;
  using r_client = t_prefix<t_client>::r_;
  using x_client = t_prefix<t_client>::x_;
  using R_client = t_prefix<t_client>::R_;

  class t_client {
  public:
    t_client(x_client) noexcept;

    r_client operator=(R_client) = delete;
    r_client operator=(x_client) = delete;

    operator t_validity() const noexcept;

    t_errn post(       t_any&&) noexcept;
    t_void post(t_err, t_any&&) noexcept;

  private:
    friend class t_processor;
    friend class t_impl_;
    t_client() = default;
    t_client(p_impl_ impl, t_user user) noexcept : impl_(impl), user_(user) {
    }

    p_impl_ impl_ = nullptr;
    t_user  user_ = t_user{0L};
  };

///////////////////////////////////////////////////////////////////////////////

  class t_processor;
  using r_processor = t_prefix<t_processor>::r_;
  using x_processor = t_prefix<t_processor>::x_;
  using R_processor = t_prefix<t_processor>::R_;

  class t_processor {
  public:
    class t_logic {
    public:
      using t_user = condvar_notify_change::t_user;
      using t_any  = condvar_notify_change::t_any;

      virtual ~t_logic() { }
      virtual t_void process(t_user, t_any&&) noexcept = 0;
    };

    using r_logic = t_logic&;

     t_processor(t_err, t_any&&) noexcept;
     t_processor(x_processor)    noexcept;
    ~t_processor();

    t_processor(R_processor)           = delete;
    r_processor operator=(x_processor) = delete;
    r_processor operator=(R_processor) = delete;

    operator t_validity () const noexcept;

    t_void process(t_err, r_logic, t_n max = t_n{1}) noexcept;

    t_client make_client(       t_user) noexcept;
    t_client make_client(t_err, t_user) noexcept;

  private:
    p_impl_ impl_ = nullptr;
  };

///////////////////////////////////////////////////////////////////////////////

  inline
  t_client::t_client(x_client client) noexcept
    : impl_(named::reset(client.impl_)),
      user_(named::reset(client.user_, 0L)) {
  }

  inline
  t_client::operator t_validity() const noexcept {
    return impl_ ? VALID : INVALID;
  }

///////////////////////////////////////////////////////////////////////////////

  inline
  t_processor::t_processor(x_processor processor) noexcept
      : impl_(named::reset(processor.impl_)) {
  }

  inline
  t_processor::operator t_validity() const noexcept {
    return impl_ ? VALID : INVALID;
  }

///////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
