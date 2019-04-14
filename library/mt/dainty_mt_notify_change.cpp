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

#include "dainty_os_fdbased.h"
#include "dainty_os_threading.h"
#include "dainty_mt_notify_change.h"

namespace dainty
{
namespace mt
{
namespace notify_change
{
  using named::utility::x_cast;

  using err::r_err;
  using namespace os::threading;
  using namespace os::fdbased;

///////////////////////////////////////////////////////////////////////////////

  class t_impl_ {
  public:
    using r_logic = t_processor::r_logic;

    t_impl_(r_err err, t_any&& any) noexcept
      : lock_(err), eventfd_(err, t_n{0}), any_(x_cast(any)) {
      if (lock_ == VALID && eventfd_ == VALID)
        valid_ = VALID;
    }

    operator t_validity() const noexcept {
      return valid_;
    }

    t_void process(r_err err, r_logic logic, t_n max) noexcept {
      for (t_n_ n = get(max); !err && n; --n) {
        t_eventfd::t_value value = 0;
        eventfd_.read(err, value);
        if (!err) {
          t_any  any;
          t_user user;
          t_bool changed = false;
          <% auto scope = lock_.make_locked_scope(err);
            if (!err) {
              changed = changed_;
              if (changed) {
                user = user_;
                any  = any_;
                changed_ = false;
              }
            }
          %>
          if (!err && changed)
            logic.process(user, x_cast(any));
        }
      }
    }

    t_errn post(t_user user, t_any&& any) noexcept {
      <% auto scope = lock_.make_locked_scope();
        if (scope == VALID && any != any_) {
          user_    = user;
          any_     = x_cast(any);
          changed_ = true;

          t_eventfd::t_value value = 1;
          return eventfd_.write(value);
        }
      %>
      return t_errn{-1};
    }

    t_void post(r_err err, t_user user, t_any&& any) noexcept {
      <% auto scope = lock_.make_locked_scope(err);
        if (!err && any != any_) {
          user_    = user;
          any_     = x_cast(any);
          changed_ = true;

          t_eventfd::t_value value = 1;
          eventfd_.write(err, value);
        }
      %>
    }

    t_fd get_fd() const noexcept {
      return eventfd_.get_fd();
    }

    t_client make_client(t_user user) noexcept {
      // NOTE: future, we have information on clients.
      return {this, user};
    }

    t_client make_client(r_err, t_user user) noexcept {
      // NOTE: future, we have information on clients.
      return {this, user};
    }

  private:
    t_validity   valid_ = INVALID;
    t_mutex_lock lock_;
    t_eventfd    eventfd_;
    t_any        any_;
    t_user       user_    = t_user{0L};
    t_bool       changed_ = false;
  };

///////////////////////////////////////////////////////////////////////////////

  t_client::t_client(t_impl_user_ impl, t_user user) noexcept
    : impl_{impl}, user_{user} {
  }

  t_client::t_client(x_client client) noexcept
    : impl_{client.impl_.release()},
      user_{named::utility::reset(client.user_)} {
  }

  t_client::operator t_validity() const noexcept {
    return impl_ == VALID && *impl_ == VALID ? VALID: INVALID;
  }

  t_errn t_client::post(t_any&& any) noexcept {
    if (*this == VALID)
      return impl_->post(user_, x_cast(any));
    return t_errn{-1};
  }

  t_void t_client::post(t_err err, t_any&& any) noexcept {
    ERR_GUARD(err) {
      if (*this == VALID)
        impl_->post(err, user_, x_cast(any));
      else
        err = err::E_XXX;
    }
  }

///////////////////////////////////////////////////////////////////////////////

  t_processor::t_processor(t_err err, t_any&& any) noexcept {
    ERR_GUARD(err) {
      impl_ = new t_impl_(err, x_cast(any));
      if (impl_ == VALID) {
        if (err)
          impl_.clear();
      } else
        err = err::E_XXX;
    }
  }

  t_processor::t_processor(x_processor processor) noexcept
    : impl_{processor.impl_.release()} {
  }

  t_processor::~t_processor() {
    impl_.clear();
  }

  t_processor::operator t_validity() const noexcept {
    return impl_ == VALID && *impl_ == VALID ? VALID: INVALID;
  }

  t_client t_processor::make_client(t_user user) noexcept {
    if (*this == VALID)
      return impl_->make_client(user);
    return {};
  }

  t_client t_processor::make_client(t_err err, t_user user) noexcept {
    ERR_GUARD(err) {
      if (*this == VALID)
        return impl_->make_client(err, user);
      err = err::E_XXX;
    }
    return {};
  }

  t_void t_processor::process(t_err err, t_logic& logic, t_n max) noexcept {
    ERR_GUARD(err) {
      if (*this == VALID)
        impl_->process(err, logic, max);
      else
        err = err::E_XXX;
    }
  }

  t_fd t_processor::get_fd() const noexcept {
    if (*this == VALID)
      return impl_->get_fd();
    return BAD_FD;
  }

///////////////////////////////////////////////////////////////////////////////
}
}
}
