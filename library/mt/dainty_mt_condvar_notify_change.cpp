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

#include "dainty_os_threading.h"
#include "dainty_mt_condvar_notify_change.h"

namespace dainty
{
namespace mt
{
namespace condvar_notify_change
{
  using named::t_n_;
  using err::r_err;
  using namespace dainty::os::threading;

///////////////////////////////////////////////////////////////////////////////

  class t_impl_ {
  public:
    using r_logic = t_processor::r_logic;

    t_impl_(r_err err, t_any&& any) noexcept
      : lock_(err), cond_(err), any_(std::move(any)) {
    }

    operator t_validity() const noexcept {
      return (lock_ == VALID && cond_ == VALID) ? VALID : INVALID;
    }

    t_void process(r_err err, r_logic logic, t_n max) noexcept {
      for (t_n_ n = get(max); !err && n; --n) {
        t_any  any;
        t_user user;
        <% auto scope = lock_.make_locked_scope(err);
          if (scope == VALID) {
            while (!changed_ && !err)
              cond_.wait(err, lock_);
            changed_ = false;
            if (!err) {
              user = user_;
              any  = any_;
            }
          }
        %>
        if (!err)
          logic.process(user, std::move(any));
      }
    }

    t_errn post(t_user user, t_any&& any) noexcept {
      t_errn errn{-1};
      <% auto scope = lock_.make_locked_scope();
        if (scope == VALID && any != any_) {
          errn = cond_.signal();
          if (errn == VALID) {
            user_    = user;
            any_     = std::move(any);
            changed_ = true;
          }
        }
      %>
      return errn;
    }

    t_void post(r_err err, t_user user, t_any&& any) noexcept {
      <% auto scope = lock_.make_locked_scope(err);
        if (!err && any != any_) {
          cond_.signal(err);
          if (!err) {
            user_    = user;
            any_     = std::move(any);
            changed_ = true;
          }
        }
      %>
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
    t_mutex_lock lock_;
    t_cond_var   cond_;
    t_any        any_;
    t_user       user_    = t_user{0L};
    t_bool       changed_ = false;
  };

///////////////////////////////////////////////////////////////////////////////

  t_errn t_client::post(t_any&& any) noexcept {
    if (impl_)
      return impl_->post(user_, std::move(any));
    return t_errn{-1};
  }

  t_void t_client::post(t_err err, t_any&& any) noexcept {
    ERR_GUARD(err) {
      if (impl_ && *impl_ == VALID)
        impl_->post(err, user_, std::move(any));
      else
        err = err::E_XXX;
    }
  }

///////////////////////////////////////////////////////////////////////////////

  t_processor::t_processor(t_err err, t_any&& any) noexcept {
    ERR_GUARD(err) {
      impl_ = new t_impl_(err, std::move(any));
      if (impl_) {
        if (err)
          delete named::reset(impl_);
      } else
        err = err::E_XXX;
    }
  }

  t_processor::~t_processor() {
    if (impl_) // NOTE: can check to see if clients exists
      delete impl_;
  }

  t_client t_processor::make_client(t_user user) noexcept {
    if (impl_)
      return impl_->make_client(user);
    return {};
  }

  t_client t_processor::make_client(t_err err, t_user user) noexcept {
    ERR_GUARD(err) {
      if (impl_ && *impl_ == VALID)
        return impl_->make_client(err, user);
      err = err::E_XXX;
    }
    return {};
  }

  t_void t_processor::process(t_err err, r_logic logic, t_n max) noexcept {
    ERR_GUARD(err) {
      if (impl_ && *impl_ == VALID)
        impl_->process(err, logic, max);
      else
        err = err::E_XXX;
    }
  }

///////////////////////////////////////////////////////////////////////////////
}
}
}
