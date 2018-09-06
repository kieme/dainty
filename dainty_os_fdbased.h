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

#ifndef _DAINTY_OS_FDBASED_H_
#define _DAINTY_OS_FDBASED_H_

#include "dainty_named.h"
#include "dainty_oops.h"
#include "dainty_os_call.h"
#include "dainty_os_clock.h"

namespace dainty
{
namespace os
{
namespace fdbased
{
  using named::t_prefix;
  using named::t_fd;
  using named::t_void;
  using named::t_n_;
  using named::t_n;
  using named::t_fd;
  using named::BAD_FD;
  using err::t_err;

///////////////////////////////////////////////////////////////////////////////

  class t_eventfd;
  using r_eventfd = t_prefix<t_eventfd>::r_;
  using x_eventfd = t_prefix<t_eventfd>::x_;
  using R_eventfd = t_prefix<t_eventfd>::R_;

  class t_eventfd final {
  public:
    using t_fd    = fdbased::t_fd;
    using t_value = named::t_uint64;
    using r_value = t_prefix<t_value>::r_;
    using R_value = t_prefix<t_value>::R_;

     t_eventfd(t_n)        noexcept;
     t_eventfd(t_err, t_n) noexcept;
     t_eventfd(x_eventfd)  noexcept;
    ~t_eventfd();

    t_eventfd(R_eventfd)           = delete;
    r_eventfd operator=(R_eventfd) = delete;
    r_eventfd operator=(x_eventfd) = delete;

    t_fd     get_fd()     const noexcept;
    operator t_validity() const noexcept;

    t_errn create(       t_n cnt) noexcept;
    t_void create(t_err, t_n cnt) noexcept;

    t_errn close() noexcept;
    t_void close(t_err) noexcept;

    t_errn read(       r_value) noexcept;
    t_void read(t_err, r_value) noexcept;

    t_errn write(       R_value) noexcept;
    t_void write(t_err, R_value) noexcept;

  private:
    t_fd fd_ = BAD_FD;
  };

///////////////////////////////////////////////////////////////////////////////

  class t_epoll;
  using r_epoll = t_prefix<t_epoll>::r_;
  using x_epoll = t_prefix<t_epoll>::x_;
  using R_epoll = t_prefix<t_epoll>::R_;

  class t_epoll final {
  public:
    using t_n          = named::t_n;
    using t_usec       = named::t_usec;
    using t_fd         = fdbased::t_fd;
    using t_event_mask = ::uint32_t;
    using t_event_data = ::epoll_data;
    using t_event      = t_prefix<::epoll_event>::t_;
    using p_event      = t_prefix<::epoll_event>::p_;

     t_epoll()        noexcept;
     t_epoll(t_err)   noexcept;
     t_epoll(x_epoll) noexcept;
    ~t_epoll();

    t_epoll(R_epoll)           = delete;
    r_epoll operator=(R_epoll) = delete;
    r_epoll operator=(x_epoll) = delete;

    operator t_validity() const noexcept;
    t_fd     get_fd()     const noexcept;

    t_errn create()      noexcept;
    t_void create(t_err) noexcept;

    t_errn close()      noexcept;
    t_void close(t_err) noexcept;

    t_errn add_event(       t_fd, t_event_mask, t_event_data) noexcept;
    t_void add_event(t_err, t_fd, t_event_mask, t_event_data) noexcept;

    t_errn mod_event(       t_fd, t_event_mask, t_event_data) noexcept;
    t_void mod_event(t_err, t_fd, t_event_mask, t_event_data) noexcept;

    t_errn del_event(       t_fd) noexcept;
    t_void del_event(t_err, t_fd) noexcept;

    t_verify<t_n> wait(       p_event, t_n max) noexcept;
    t_n           wait(t_err, p_event, t_n max) noexcept;

    t_verify<t_n> wait(       p_event, t_n max, t_usec) noexcept;
    t_n           wait(t_err, p_event, t_n max, t_usec) noexcept;

    template<t_n_ N>
    inline
    t_verify<t_n> wait(t_event (&event)[N]) noexcept {
      return wait(event, t_n{N});
    }

    template<t_n_ N>
    inline
    t_n wait(t_err err, t_event (&event)[N]) noexcept {
      return wait(err, event, t_n{N});
    }

    template<t_n_ N>
    inline
    t_verify<t_n> wait(t_event (&event)[N], t_usec usec) noexcept {
      return wait(event, t_n{N}, usec);
    }

    template<t_n_ N>
    inline
    t_n wait(t_err err, t_event (&event)[N], t_usec usec) noexcept {
      return wait(err, event, t_n{N}, usec);
    }

  private:
    t_fd fd_ = BAD_FD;
  };

///////////////////////////////////////////////////////////////////////////////

  class t_timerfd;
  using r_timerfd = t_prefix<t_timerfd>::r_;
  using x_timerfd = t_prefix<t_timerfd>::x_;
  using R_timerfd = t_prefix<t_timerfd>::R_;

  class t_timerfd final {
  public:
    using t_time      = clock::t_time;
    using t_flags     = os::t_flags;
    using t_fd        = fdbased::t_fd;
    using t_timerspec = t_prefix<::itimerspec>::t_;
    using r_timerspec = t_prefix<t_timerspec>::r_;
    using R_timerspec = t_prefix<t_timerspec>::R_;
    using t_data      = named::t_uint64;
    using r_data      = t_prefix<t_data>::r_;

    static t_timerspec mk_timerspec(t_time value, t_time interval);

     t_timerfd(       t_flags) noexcept;
     t_timerfd(t_err, t_flags) noexcept;
     t_timerfd(x_timerfd)      noexcept;
    ~t_timerfd();

    t_timerfd(R_timerfd)           = delete;
    r_timerfd operator=(R_timerfd) = delete;
    r_timerfd operator=(x_timerfd) = delete;

    operator t_validity() const noexcept;
    t_fd     get_fd()     const noexcept;

    t_errn create(       t_flags) noexcept;
    t_void create(t_err, t_flags) noexcept;

    t_errn close()      noexcept;
    t_void close(t_err) noexcept;

    t_errn set_time(       t_flags, R_timerspec) noexcept;
    t_void set_time(t_err, t_flags, R_timerspec) noexcept;
    t_errn set_time(       t_flags, R_timerspec, r_timerspec) noexcept;
    t_void set_time(t_err, t_flags, R_timerspec, r_timerspec) noexcept;

    t_errn get_time(       r_timerspec) noexcept;
    t_void get_time(t_err, r_timerspec) noexcept;

    t_errn read(       r_data) noexcept;
    t_void read(t_err, r_data) noexcept;

  private:
    t_fd fd_ = BAD_FD;
  };

///////////////////////////////////////////////////////////////////////////////


  inline
  t_eventfd::operator t_validity() const noexcept {
    return fd_ != BAD_FD ? VALID : INVALID;
  }

  inline
  t_fd t_eventfd::get_fd() const noexcept {
    return fd_;
  }

  inline
  t_epoll::operator t_validity() const noexcept {
    return fd_ != BAD_FD ? VALID : INVALID;
  }

  inline
  t_fd t_epoll::get_fd() const noexcept {
    return fd_;
  }

  inline
  t_timerfd::operator t_validity() const noexcept {
    return fd_ != BAD_FD ? VALID : INVALID;
  }

  inline
  t_fd t_timerfd::get_fd() const noexcept {
    return fd_;
  }

///////////////////////////////////////////////////////////////////////////////

  inline
  t_timerfd::t_timerspec t_timerfd::mk_timerspec(t_time value,
                                                 t_time interval) {
    t_timerspec spec;
    spec.it_value    = clock::to_(value);
    spec.it_interval = clock::to_(interval);
    return spec;
  }

///////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
