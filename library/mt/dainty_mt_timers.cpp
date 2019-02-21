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

#include "dainty_named_utility.h"
#include "dainty_mt_timers.h"

namespace dainty
{
namespace mt
{
namespace timers
{
  using named::t_cstr_cptr;
  using r_err   = t_prefix<t_err>::r_;
  using p_logic = t_timers::p_logic;


  class t_impl_ {
  public:
    t_impl_(R_params params) noexcept : params_{params} {
    }

    t_impl_(r_err err, R_params params) noexcept : params_{params} {
    }

    operator t_validity() const noexcept {
      return INVALID;
    }

    t_fd get_fd() const noexcept {
      return BAD_FD;
    }

    t_timer_id start_timer (R_timer_params params,
                            p_timer_logic logic) noexcept {
      return t_timer_id{0};
    }

    t_timer_id start_timer(r_err err, R_timer_params params,
                           p_timer_logic logic) noexcept {
      return t_timer_id{0};
    }

    t_errn restart_timer(t_timer_id id, R_timer_params params) noexcept {
      return t_errn{-1};
    }

    t_void restart_timer(r_err err, t_timer_id id,
                         R_timer_params params) noexcept {
    }

    t_errn restart_timer(t_timer_id id) noexcept {
      return t_errn{-1};
    }

    t_void restart_timer(r_err err, t_timer_id id) noexcept {
    }

    t_bool stop_timer(t_timer_id id) noexcept {
      return false;
    }

    p_timer_logic clear_timer(t_timer_id id) noexcept {
      return nullptr;
    }

    t_void clear_timers() noexcept {
    }

    t_bool is_timer_running(t_timer_id id) const noexcept {
      return false;
    }

    P_timer_info get_info(t_timer_id id) const noexcept {
      return nullptr;
    }

    P_timer_info get_info(r_err err, t_timer_id id) const noexcept {
      return nullptr;
    }

    t_void get_timer_ids(r_timer_ids ids) const noexcept {
    }

    t_errn notify_expiry(p_logic logic) noexcept {
      return t_errn{-1};
    }

    t_void notify_expiry(r_err err, p_logic logic) noexcept {
    }

    t_errn event_loop(p_logic logic) noexcept {
      return t_errn{-1};
    }

    t_void event_loop(r_err err, p_logic logic) noexcept {
    }

  private:
    T_params params_;
  };

///////////////////////////////////////////////////////////////////////////////

  t_n get_supported_services() noexcept {
    return t_n{1};
  }

  t_service_name get_supported_service(t_ix) noexcept {
    return "timerfd_service";
  }

///////////////////////////////////////////////////////////////////////////////

  t_void
      t_timers::t_logic::notify_timers_reorder(r_timer_infos) noexcept {
    // XXX provide default reorder - thats reorders on prio
  }

///////////////////////////////////////////////////////////////////////////////

  t_timers::t_timers(R_params params) noexcept {
    if (params.service_name == t_cstr_cptr{"timerfd_service"})
      impl_ = new t_impl_{params};
    else {
      // not yet supported - required for non-linux systems
    }
  }

  t_timers::t_timers(t_err err, R_params params) noexcept {
    ERR_GUARD(err) {
      if (params.service_name == t_cstr_cptr{"timerfd_service"})
        impl_ = new t_impl_{err, params};
      else {
        err = err::E_XXX;
        // not yet supported - required for non-linux systems
      }
    }
  }

  t_timers::t_timers(x_timers timers) noexcept
    : impl_{timers.impl_.release()} {
  }

  t_timers::~t_timers() {
    if (*this == VALID)
      impl_->clear_timers();
  }

  t_timers::operator t_validity() const noexcept {
    return impl_ == VALID && *impl_ == VALID ? VALID : INVALID;
  }

  t_fd t_timers::get_fd() const noexcept {
    if (*this == VALID)
      return impl_->get_fd();
    return BAD_FD;
  }

  t_timer_id t_timers::start_timer(R_timer_params params,
                                   p_timer_logic logic) noexcept {
    if (*this == VALID)
      return impl_->start_timer(params, logic);
    return BAD_TIMER_ID;
  }

  t_timer_id t_timers::start_timer(t_err err, R_timer_params params,
                                   p_timer_logic logic) noexcept {
    ERR_GUARD(err) {
      if (*this == VALID)
        return impl_->start_timer(err, params, logic);
      err = err::E_XXX;
    }
    return BAD_TIMER_ID;
  }

  t_errn t_timers::restart_timer(t_timer_id id,
                                 R_timer_params params) noexcept {
    if (*this == VALID)
      return impl_->restart_timer(id, params);
    return t_errn{-1};
  }

  t_void t_timers::restart_timer(t_err err, t_timer_id id,
                                 R_timer_params params) noexcept {
    ERR_GUARD(err) {
      if (*this == VALID)
        impl_->restart_timer(err, id, params);
      else
        err = err::E_XXX;
    }
  }

  t_errn t_timers::restart_timer(t_timer_id id) noexcept {
    if (*this == VALID)
      return impl_->restart_timer(id);
    return t_errn{-1};
  }

  t_void t_timers::restart_timer(t_err err, t_timer_id id) noexcept {
    ERR_GUARD(err) {
      if (*this == VALID)
        impl_->restart_timer(err, id);
      else
        err = err::E_XXX;
    }
  }

  t_bool t_timers::stop_timer(t_timer_id id) noexcept {
    if (*this == VALID)
      return impl_->stop_timer(id);
    return false;
  }

  p_timer_logic t_timers::clear_timer(t_timer_id id) noexcept {
    if (*this == VALID)
      return impl_->clear_timer(id);
    return nullptr;
  }

  t_void t_timers::clear_timers() noexcept {
    if (*this == VALID)
      impl_->clear_timers();
  }

  t_bool t_timers::is_timer_running(t_timer_id id) const noexcept {
    if (*this == VALID)
      return impl_->is_timer_running(id);
    return false;
  }

  P_timer_info t_timers::get_info(t_timer_id id) const noexcept {
    if (*this == VALID)
      return impl_->get_info(id);
    return nullptr;
  }

  P_timer_info t_timers::get_info(t_err err, t_timer_id id) const noexcept {
    ERR_GUARD(err) {
      if (*this == VALID)
        return impl_->get_info(err, id);
      err = err::E_XXX;
    }
    return nullptr;
  }

  t_void t_timers::get_timer_ids(r_timer_ids ids) const noexcept {
    if (*this == VALID)
      impl_->get_timer_ids(ids);
  }

  t_errn t_timers::notify_expiry(p_logic logic) noexcept {
    if (*this == VALID)
      return impl_->notify_expiry(logic);
    return t_errn{-1};
  }

  t_void t_timers::notify_expiry(t_err err, p_logic logic) noexcept {
    ERR_GUARD(err) {
      if (*this == VALID)
        return impl_->notify_expiry(err, logic);
      err = err::E_XXX;
    }
  }

  t_errn t_timers::event_loop(p_logic logic) noexcept {
    if (*this == VALID)
      return impl_->event_loop(logic);
    return t_errn{-1};
  }

  t_void t_timers::event_loop(t_err err, p_logic logic) noexcept {
    ERR_GUARD(err) {
      if (*this == VALID)
        return impl_->event_loop(err, logic);
      err = err::E_XXX;
    }
  }

///////////////////////////////////////////////////////////////////////////////
}
}
}
