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

namespace dainty
{
namespace os
{
namespace fdbased
{
  using named::t_void;

///////////////////////////////////////////////////////////////////////////////

  t_eventfd::t_eventfd(t_n cnt) noexcept : fd_{call_eventfd(cnt).value} {
  }

  t_eventfd::t_eventfd(t_err err, t_n cnt) noexcept
      : fd_{call_eventfd(err, cnt)} {
  }

  t_eventfd::t_eventfd(t_eventfd&& event) noexcept : fd_(event.fd_) {
    event.fd_ = BAD_FD;
  }

  t_eventfd::~t_eventfd() {
    close();
  }

  t_errn t_eventfd::create(t_n cnt) noexcept {
    t_errn errn{-1};
    if (fd_ == BAD_FD) {
      auto verify = call_eventfd(cnt);
      if (verify == VALID)
        fd_ = verify.value;
      errn = verify.errn;
    }
    return errn;
  }

  t_validity t_eventfd::create(t_err err, t_n cnt) noexcept {
    T_ERR_GUARD(err) {
      if (fd_ == BAD_FD) {
        fd_ = call_eventfd(err, cnt);
        if (fd_ != BAD_FD)
          return VALID;
      } else
        err = E_XXX;
    }
    return INVALID;
  }

  t_errn t_eventfd::close() noexcept {
    t_errn errn{-1};
    if (fd_ != BAD_FD) {
      errn = call_close(fd_);
      fd_  = BAD_FD;
    }
    return errn;
  }

  t_validity t_eventfd::close(t_err err) noexcept {
    T_ERR_GUARD(err) {
      if (close() == VALID)
        return VALID;
      err = E_XXX;
    }
    return INVALID;
  }

  t_errn t_eventfd::read(r_value value) noexcept {
    t_errn errn{-1};
    if (fd_ != BAD_FD) {
      auto verify = call_read(fd_, &value, t_n{sizeof(t_value)});
      if (get(verify.value) == sizeof(value))
        return verify.errn;
      set(errn) = -1;
    }
    return errn;
  }

  t_validity t_eventfd::read(t_err err, r_value value) noexcept {
    T_ERR_GUARD(err) {
      if (fd_ != BAD_FD) {
        auto cnt = call_read(err, fd_, &value, t_n{sizeof(t_value)});
        if (get(cnt) == sizeof(value))
          return VALID;
      } else
        err = E_XXX;
    }
    return INVALID;
  }

  t_errn t_eventfd::write(R_value value) noexcept {
    t_errn errn{-1};
    if (fd_ != BAD_FD) {
      auto verify = call_write(fd_, &value, t_n{sizeof(t_value)});
      if (get(verify.value) == sizeof(value))
        return verify.errn;
      set(errn) = -1;
    }
    return errn;
  }

  t_validity t_eventfd::write(t_err err, R_value value) noexcept {
    T_ERR_GUARD(err) {
      if (fd_ != BAD_FD) {
        auto cnt = call_write(err, fd_, &value, t_n{sizeof(t_value)});
        if (get(cnt) == sizeof(value))
          return VALID;
      } else
        err = E_XXX;
    }
    return INVALID;
  }

///////////////////////////////////////////////////////////////////////////////

  t_epoll::t_epoll() noexcept : fd_{call_epoll_create().value} {
  }

  t_epoll::t_epoll(t_err err) noexcept : fd_{call_epoll_create(err)} {
  }

  t_epoll::t_epoll(t_epoll&& epoll) noexcept : fd_{epoll.fd_} {
    epoll.fd_ = BAD_FD;
  }

  t_epoll::~t_epoll() {
    close();
  }

  t_errn t_epoll::create() noexcept {
    t_errn errn{-1};
    if (fd_ == BAD_FD) {
      auto verify = call_epoll_create();
      if (verify == VALID)
        fd_ = verify.value;
      errn = verify.errn;
    }
    return errn;
  }

  t_validity t_epoll::create(t_err err) noexcept {
    T_ERR_GUARD(err) {
      fd_ =  call_epoll_create(err);
      if (fd_ != BAD_FD)
        return VALID;
    }
    return INVALID;
  }

  t_errn t_epoll::close() noexcept {
    t_errn errn{-1};
    if (fd_ != BAD_FD) {
      errn = call_close(fd_);
      fd_ = BAD_FD;
    }
    return errn;
  }

  t_validity t_epoll::close(t_err err) noexcept {
    T_ERR_GUARD(err) {
      if (close() == VALID)
        return VALID;
      err = E_XXX;
    }
    return INVALID;
  }

  t_errn t_epoll::add_event(t_fd fd, t_event_mask mask,
                            t_event_data data) noexcept {
    t_event event{mask, data};
    return call_epoll_ctl_add(fd_, fd, event);
  }

  t_validity t_epoll::add_event(t_err err, t_fd fd, t_event_mask mask,
                                t_event_data data) noexcept {
    T_ERR_GUARD(err) {
      t_event event{mask, data};
      return call_epoll_ctl_add(err, fd_, fd, event);
    }
    return INVALID;
  }

  t_errn t_epoll::mod_event(t_fd fd, t_event_mask mask,
                            t_event_data data) noexcept {
    t_event event{mask, data};
    return call_epoll_ctl_mod(fd_, fd, event);
  }

  t_validity t_epoll::mod_event(t_err err, t_fd fd, t_event_mask mask,
                                t_event_data data) noexcept {
    T_ERR_GUARD(err) {
      t_event event{mask, data};
      return call_epoll_ctl_mod(err, fd_, fd, event);
    }
    return INVALID;
  }

  t_errn t_epoll::del_event(t_fd fd) noexcept {
    return call_epoll_ctl_del(fd_, fd);
  }

  t_validity t_epoll::del_event(t_err err, t_fd fd) noexcept {
    T_ERR_GUARD(err) {
      return call_epoll_ctl_del(err, fd_, fd);
    }
    return INVALID;
  }

  t_verify<t_n> t_epoll::wait(p_event event, t_n max) noexcept {
    return call_epoll_wait(fd_, event, max);
  }

  t_n t_epoll::wait(t_err err, p_event event, t_n max) noexcept {
    T_ERR_GUARD(err) {
      return call_epoll_wait(err, fd_, event, max);
    }
    return t_n(0);
  }

  t_verify<t_n> t_epoll::wait(p_event event, t_n max, t_usec usec) noexcept {
    return call_epoll_wait(fd_, event, max, usec);
  }

  t_n t_epoll::wait(t_err err, p_event event, t_n max, t_usec usec) noexcept {
    T_ERR_GUARD(err) {
      return call_epoll_wait(err, fd_, event, max, usec);
    }
    return t_n(0);
  }

///////////////////////////////////////////////////////////////////////////////

  t_timerfd::t_timerfd(t_flags flags) noexcept {
  }

  t_timerfd::t_timerfd(t_err err, t_flags flags) noexcept {
    T_ERR_GUARD(err) {
    }
  }

  t_timerfd::t_timerfd(t_timerfd&&) noexcept {
  }

  t_timerfd::~t_timerfd() {
  }

  t_errn t_timerfd::create(t_flags flags) noexcept {
    return t_errn{-1};
  }

  t_validity t_timerfd::create(t_err err, t_flags flags) noexcept {
    T_ERR_GUARD(err) {
    }
    return INVALID;
  }

  t_errn t_timerfd::close() noexcept {
    return t_errn{-1};
  }

  t_validity t_timerfd::close(t_err err) noexcept {
    T_ERR_GUARD(err) {
    }
    return INVALID;
  }

  t_errn t_timerfd::set_time(R_timerspec, t_flags flags) noexcept {
    return t_errn{-1};
  }

  t_validity t_timerfd::set_time(t_err err, R_timerspec timer,
                                 t_flags flags) noexcept {
    T_ERR_GUARD(err) {
    }
    return INVALID;
  }

  t_errn t_timerfd::set_time(R_timerspec, r_timerspec timer,
                            t_flags flags) noexcept {
    return t_errn{-1};
  }

  t_validity t_timerfd::set_time(t_err err, R_timerspec ntimer,
                                 r_timerspec otimer, t_flags flags) noexcept {
    T_ERR_GUARD(err) {
    }
    return INVALID;
  }

  t_errn t_timerfd::get_time(r_timerspec timer) noexcept {
    return t_errn{-1};
  }

  t_validity t_timerfd::get_time(t_err err, r_timerspec timer) noexcept {
    T_ERR_GUARD(err) {
    }
    return INVALID;
  }

  t_errn t_timerfd::read(r_data data) noexcept {
    return t_errn{-1};
  }

  t_validity t_timerfd::read(t_err err, r_data data) noexcept {
    T_ERR_GUARD(err) {
    }
    return INVALID;
  }

///////////////////////////////////////////////////////////////////////////////

}
}
}
