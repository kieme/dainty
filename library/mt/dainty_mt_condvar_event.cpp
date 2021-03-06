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
#include "dainty_mt_condvar_event.h"

namespace dainty
{
namespace mt
{
namespace condvar_event
{
///////////////////////////////////////////////////////////////////////////////

  using namespace dainty::os::threading;

///////////////////////////////////////////////////////////////////////////////

  using err::r_err;

  using base::types::t_n_;

///////////////////////////////////////////////////////////////////////////////

  class t_impl_ {
  public:
    using r_logic = t_processor::r_logic;

    t_impl_(r_err err) noexcept : lock_{err}, cond_{err} {
      if (lock_ == VALID && cond_ == VALID)
        valid_ = VALID;
    }

    operator t_validity() const noexcept {
      return valid_;
    }

    t_cnt get_cnt(t_err err) {
      t_cnt_ cnt = 0;
      <% auto scope = lock_.make_locked_scope(err);
        cnt = cnt_;
      %>
      return t_cnt{cnt};
    }

    t_void process(r_err err, r_logic logic, t_n max) noexcept {
      for (t_n_ n = get(max); !err && n; --n) {
        t_cnt_ cnt = 0;
        <% auto scope = lock_.make_locked_scope(err);
          if (!err) {
            if (!cnt_) {
              do {
                cond_.wait(err, lock_);
              } while (!err && !cnt_);
            }
            cnt = base::reset(cnt_);
          }
        %>
        if (!err)
          logic.async_process(t_cnt{cnt});
      }
    }

    t_void reset_then_process(r_err err, r_logic logic, t_n max) noexcept {
      for (t_n_ n = get(max); !err && n; --n) {
        t_cnt_ cnt = 0;
        <% auto scope = lock_.make_locked_scope(err);
          if (!err) {
            base::reset(cnt_);
            do {
              cond_.wait(err, lock_);
            } while (!err && !cnt_);
            cnt = base::reset(cnt_);
          }
        %>
        if (!err)
          logic.async_process(t_cnt{cnt});
      }
    }

    t_errn post(t_user, t_cnt cnt) noexcept {
      t_errn errn{-1};
      <% auto scope = lock_.make_locked_scope();
        if (scope == VALID) {
          const t_bool signal = !cnt_;
          cnt_ += get(cnt);
          if (signal)
            errn = cond_.signal();
        }
      %>
      return errn;
    }

    t_void post(r_err err, t_user, t_cnt cnt) noexcept {
      <% auto scope = lock_.make_locked_scope(err);
        if (!err) {
          const t_bool signal = !cnt_;
          cnt_ += get(cnt);
          if (signal)
            cond_.signal(err);
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
    t_validity   valid_ = INVALID;
    t_mutex_lock lock_;
    t_cond_var   cond_;
    t_cnt_       cnt_ = 0;
  };

///////////////////////////////////////////////////////////////////////////////

  t_client::t_client(t_impl_user_ impl, t_user user) noexcept
    : impl_{impl}, user_{user} {
  }

  t_client::t_client(x_client client) noexcept
    : impl_{client.impl_.release()},
      user_{base::reset(client.user_)} {
  }

  t_client::operator t_validity() const noexcept {
    return impl_ == VALID && *impl_ == VALID ? VALID : INVALID;
  }

  t_errn t_client::post(t_cnt cnt) noexcept {
    if (*this == VALID)
      return impl_->post(user_, cnt);
    return t_errn{-1};
  }

  t_void t_client::post(t_err err, t_cnt cnt) noexcept {
    ERR_GUARD(err) {
      if (*this == VALID)
        impl_->post(err, user_, cnt);
      else
        err = err::E_XXX;
    }
  }

///////////////////////////////////////////////////////////////////////////////

  t_processor::t_processor(t_err err) noexcept {
    ERR_GUARD(err) {
      impl_ = new t_impl_(err);
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

  t_processor::operator t_validity() const noexcept {
    return impl_ == VALID && *impl_ == VALID ? VALID : INVALID;
  }

  t_processor::~t_processor() {
    impl_.clear();
  }

  t_cnt t_processor::get_cnt(t_err err) {
    ERR_GUARD(err) {
      if (*this == VALID)
        return impl_->get_cnt(err);
      err = err::E_XXX;
    }
    return t_cnt{0};
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

  t_void t_processor::process(t_err err, r_logic logic, t_n max) noexcept {
    ERR_GUARD(err) {
      if (*this == VALID)
        impl_->process(err, logic, max);
      else
        err = err::E_XXX;
    }
  }

  t_void t_processor::reset_then_process(t_err err, r_logic logic,
                                         t_n max) noexcept {
    ERR_GUARD(err) {
      if (*this == VALID)
        impl_->reset_then_process(err, logic, max);
      else
        err = err::E_XXX;
    }
  }

///////////////////////////////////////////////////////////////////////////////
}
}
}
