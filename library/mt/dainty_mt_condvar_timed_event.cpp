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
#include "dainty_mt_condvar_timed_event.h"

namespace dainty
{
namespace mt
{
namespace condvar_timed_event
{
  using named::t_n_;
  using namespace dainty::os::threading;

///////////////////////////////////////////////////////////////////////////////

  class t_impl_ {
  public:
    using t_logic = t_processor::t_logic;
    using r_logic = t_processor::r_logic;
    using R_time  = t_processor::R_time;

    t_impl_(t_err& err) noexcept : lock_{err}, cond_{err} {
    }

    operator t_validity() const noexcept {
      return (lock_ == VALID && cond_ == VALID) ?  VALID : INVALID;
    }

    t_cnt get_cnt(t_err& err) {
      t_cnt cnt{0};
      <% auto scope = lock_.make_locked_scope(err);
        set(cnt) = cnt_;
      %>
      return cnt;
    }

    t_validity process(t_err& err, r_logic logic, R_time time,
                       t_n max) noexcept {
      for (t_n_ n = get(max); !err && n; --n) {
        t_cnt cnt{0};
        <% auto scope = lock_.make_locked_scope(err);
          if (scope == VALID) {
            if (!cnt_) {
              do {
                cond_.wait_for(err, lock_, time);
              } while (!err && !cnt_);
            }
            set(cnt) = cnt_;
            cnt_ = 0;
          }
        %>
        if (!err)
          logic.async_process(cnt);
        else if (err.id() == os::E_TIMEOUT) {
          err.clear();
          logic.timeout_process(time);
        }
      }
      return !err ? VALID : INVALID;
    }

    t_validity reset_then_process(t_err err, r_logic logic, R_time time,
                                  t_n max) noexcept {
      for (t_n_ n = get(max); !err && n; --n) {
        t_cnt cnt{0};
        <% auto scope = lock_.make_locked_scope(err);
          if (scope == VALID) {
            cnt_ = 0;
            do {
              cond_.wait_for(err, lock_, time);
            } while (!err && !cnt_);
            set(cnt) = cnt_;
            cnt_ = 0;
          }
        %>
        if (!err)
          logic.async_process(cnt);
        else if (err.id() == os::E_TIMEOUT) {
          err.clear();
          logic.timeout_process(time);
        }
      }
      return !err ? VALID : INVALID;
    }

    t_validity post(t_user, t_cnt cnt) noexcept {
      t_validity validity = INVALID;
      <% auto scope = lock_.make_locked_scope();
        if (scope == VALID) {
          const t_bool signal = !cnt_;
          cnt_ += get(cnt);
          validity = VALID;
          if (signal && cond_.signal() == INVALID)
            validity = INVALID;
        }
      %>
      return validity;
    }

    t_validity post(t_err& err, t_user, t_cnt cnt) noexcept {
      <% auto scope = lock_.make_locked_scope(err);
        if (scope == VALID) {
          const t_bool signal = !cnt_;
          cnt_ += get(cnt);
          if (signal)
            cond_.signal(err);
        }
      %>
      return !err ? VALID : INVALID;
    }

    t_client make_client(t_user user) noexcept {
      // NOTE: future, we have information on clients.
      return {this, user};
    }

    t_client make_client(t_err&, t_user user) noexcept {
      // NOTE: future, we have information on clients.
      return {this, user};
    }

  private:
    t_mutex_lock         lock_;
    t_monotonic_cond_var cond_;
    t_cnt_               cnt_ = 0;
  };

///////////////////////////////////////////////////////////////////////////////

  t_validity t_client::post(t_cnt cnt) noexcept {
    if (impl_)
      return impl_->post(user_, cnt);
    return INVALID;
  }

  t_validity t_client::post(t_err err, t_cnt cnt) noexcept {
    T_ERR_GUARD(err) {
      if (impl_ && *impl_ == VALID)
        return impl_->post(err, user_, cnt);
      err = E_XXX;
    }
    return INVALID;
  }

///////////////////////////////////////////////////////////////////////////////

  t_processor::t_processor(t_err err) noexcept {
    T_ERR_GUARD(err) {
      impl_ = new t_impl_(err);
      if (impl_) {
        if (err) {
          delete impl_;
          impl_ = nullptr;
        }
      } else
        err = E_XXX;
    }
  }

  t_processor::~t_processor() {
    if (impl_) { // NOTE: can check to see if clients exists
      delete impl_;
    }
  }

  t_cnt t_processor::get_cnt(t_err err) {
    T_ERR_GUARD(err) {
      if (impl_ && *impl_ == VALID)
        return impl_->get_cnt(err);
      err = E_XXX;
    }
    return t_cnt{0};
  }

  t_client t_processor::make_client(t_user user) noexcept {
    if (impl_)
      return impl_->make_client(user);
    return {};
  }

  t_client t_processor::make_client(t_err err, t_user user) noexcept {
    T_ERR_GUARD(err) {
      if (impl_ && *impl_ == VALID)
        return impl_->make_client(err, user);
      err = E_XXX;
    }
    return {};
  }

  t_validity t_processor::process(t_err err, r_logic logic, R_time time,
                                  t_n max) noexcept {
    T_ERR_GUARD(err) {
      if (impl_ && *impl_ == VALID)
        return impl_->process(err, logic, time, max);
      err = E_XXX;
    }
    return INVALID;
  }

  t_validity t_processor::reset_then_process(t_err err, r_logic logic,
                                             R_time time, t_n max) noexcept {
    T_ERR_GUARD(err) {
      if (impl_ && *impl_ == VALID)
        return impl_->reset_then_process(err, logic, time, max);
      err = E_XXX;
    }
    return INVALID;
  }

///////////////////////////////////////////////////////////////////////////////
}
}
}
