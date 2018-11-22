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

#include <errno.h>
#include "dainty_named_utility.h"
#include "dainty_os_call.h"

namespace dainty
{
namespace os
{
///////////////////////////////////////////////////////////////////////////////

  t_errn call_pthread_init(r_pthread_mutexattr attr) noexcept {
    return t_errn{::pthread_mutexattr_init(&attr)};
  }

  t_void call_pthread_init(t_err err, r_pthread_mutexattr attr) noexcept {
    ERR_GUARD(err) {
      auto errn{call_pthread_init(attr)};
      if (errn == INVALID)
        err = err::E_INIT_FAIL;
    }
  }

///////////////////////////////////////////////////////////////////////////////

  t_errn call_pthread_destroy(r_pthread_mutexattr attr) noexcept {
    return t_errn{::pthread_mutexattr_destroy(&attr)};
  }

  t_void call_pthread_destroy(t_err err, r_pthread_mutexattr attr) noexcept {
    ERR_GUARD(err) {
      auto errn{call_pthread_destroy(attr)};
      if (errn == INVALID)
        err = err::E_DESTROY_FAIL;
    }
  }

///////////////////////////////////////////////////////////////////////////////

  t_errn call_pthread_set_recursive(r_pthread_mutexattr attr) noexcept {
    return t_errn{::pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE)};
  }

  t_void call_pthread_set_recursive(t_err err,
                                    r_pthread_mutexattr attr) noexcept {
    ERR_GUARD(err) {
      auto errn{call_pthread_set_recursive(attr)};
      if (errn == INVALID)
        err = err::E_XXX;
    }
  }

///////////////////////////////////////////////////////////////////////////////

  t_bool call_pthread_is_recursive(R_pthread_mutexattr attr) noexcept {
    int type = 0;
    return ::pthread_mutexattr_gettype(&attr, &type) == 0 &&
           type == PTHREAD_MUTEX_RECURSIVE;
  }

  t_bool call_pthread_is_recursive(t_err err,
                                   R_pthread_mutexattr attr) noexcept {
    ERR_GUARD(err) {
      int type = 0;
      if (::pthread_mutexattr_gettype(&attr, &type) == 0)
        return type == PTHREAD_MUTEX_RECURSIVE;
      err = err::E_XXX;
    }
    return false;
  }

///////////////////////////////////////////////////////////////////////////////

  t_errn call_pthread_mutex_init(r_pthread_mutex mutex) noexcept {
     return t_errn{::pthread_mutex_init(&mutex, NULL)};
  }

  t_void call_pthread_mutex_init(t_err err, r_pthread_mutex mutex) noexcept {
    ERR_GUARD(err) {
      auto errn{call_pthread_mutex_init(mutex)};
      if (errn == INVALID)
        err = err::E_XXX;
    }
  }

  t_errn call_pthread_mutex_init(r_pthread_mutex mutex,
                                 R_pthread_mutexattr attr) noexcept {
     return t_errn{::pthread_mutex_init(&mutex, &attr)};
  }

  t_void call_pthread_mutex_init(t_err err, r_pthread_mutex mutex,
                                 R_pthread_mutexattr attr) noexcept {
    ERR_GUARD(err) {
      auto errn{call_pthread_mutex_init(mutex, attr)};
      if (errn == INVALID)
        err = err::E_XXX;
    }
  }

  t_errn call_pthread_mutex_destroy(r_pthread_mutex mutex) noexcept {
    return t_errn{::pthread_mutex_destroy(&mutex)};
  }

  t_void call_pthread_mutex_destroy(t_err err, r_pthread_mutex mutex) noexcept {
    ERR_GUARD(err) {
      auto errn{call_pthread_mutex_destroy(mutex)};
      if (errn == INVALID)
        err = err::E_XXX;
    }
  }

  t_errn call_pthread_mutex_lock(r_pthread_mutex mutex) noexcept {
    return t_errn{::pthread_mutex_lock(&mutex)};
  }

  t_void call_pthread_mutex_lock(t_err err, r_pthread_mutex mutex) noexcept {
    ERR_GUARD(err) {
      auto errn{call_pthread_mutex_lock(mutex)};
      if (errn == INVALID)
        err = err::E_XXX;
    }
  }

  t_errn call_pthread_mutex_timedlock(r_pthread_mutex mutex,
                                      R_timespec spec) noexcept {
    return t_errn{::pthread_mutex_timedlock(&mutex, &spec)};
  }

  t_void call_pthread_mutex_timedlock(t_err err, r_pthread_mutex mutex,
                                      R_timespec spec) noexcept {
    ERR_GUARD(err) {
      auto errn{call_pthread_mutex_timedlock(mutex, spec)};
      if (errn == INVALID)
        err = err::E_XXX;
    }
  }

  t_errn call_pthread_mutex_trylock(r_pthread_mutex mutex) noexcept {
    return t_errn{::pthread_mutex_trylock(&mutex)};
  }

  t_void call_pthread_mutex_trylock(t_err err, r_pthread_mutex mutex) noexcept {
    ERR_GUARD(err) {
      auto errn{call_pthread_mutex_trylock(mutex)};
      if (errn == INVALID)
        err = err::E_XXX;
    }
  }

  t_errn call_pthread_mutex_unlock(r_pthread_mutex mutex) noexcept {
    return t_errn{::pthread_mutex_unlock(&mutex)};
  }

  t_void call_pthread_mutex_unlock(t_err err, r_pthread_mutex mutex) noexcept {
    ERR_GUARD(err) {
      auto errn{call_pthread_mutex_unlock(mutex)};
      if (errn == INVALID)
        err = err::E_XXX;
    }
  }

///////////////////////////////////////////////////////////////////////////////

  t_errn call_pthread_init(r_pthread_condattr attr) noexcept {
    return t_errn{::pthread_condattr_init(&attr)};
  }

  t_void call_pthread_init(t_err err, r_pthread_condattr attr) noexcept {
    ERR_GUARD(err) {
      auto errn{call_pthread_init(attr)};
      if (errn == INVALID)
        err = err::E_XXX;
    }
  }

///////////////////////////////////////////////////////////////////////////////

  t_errn call_pthread_set_monotonic(r_pthread_condattr attr) noexcept {
    return t_errn{::pthread_condattr_setclock(&attr, CLOCK_MONOTONIC)};
  }

  t_void call_pthread_set_monotonic(t_err err,
                                    r_pthread_condattr attr) noexcept {
    ERR_GUARD(err) {
      auto errn{call_pthread_set_monotonic(attr)};
      if (errn == INVALID)
        err = err::E_XXX;
    }
  }

///////////////////////////////////////////////////////////////////////////////

  t_errn call_pthread_destroy(r_pthread_condattr attr) noexcept {
    return t_errn{::pthread_condattr_destroy(&attr)};
  }

  t_void call_pthread_destroy(t_err err, r_pthread_condattr attr) noexcept {
    ERR_GUARD(err) {
      auto errn{call_pthread_destroy(attr)};
      if (errn == INVALID)
        err = err::E_XXX;
    }
  }

///////////////////////////////////////////////////////////////////////////////

  t_bool call_pthread_is_monotonic(R_pthread_condattr attr) noexcept {
    t_clockid clk;
    return ::pthread_condattr_getclock(&attr, &clk) == 0 &&
           clk == CLOCK_MONOTONIC;
  }

  t_bool call_pthread_is_monotonic(t_err err,
                                   R_pthread_condattr attr) noexcept {
    ERR_GUARD(err) {
      t_clockid clk;
      auto ret = ::pthread_condattr_getclock(&attr, &clk);
      if (ret == 0)
        return clk == CLOCK_MONOTONIC;
      err = err::E_XXX;
    }
    return false;
  }

///////////////////////////////////////////////////////////////////////////////

  t_errn call_pthread_cond_init(r_pthread_cond cond) noexcept {
    return t_errn{::pthread_cond_init(&cond, NULL)};
  }

  t_void call_pthread_cond_init(t_err err, r_pthread_cond cond) noexcept {
    ERR_GUARD(err) {
      auto errn{call_pthread_cond_init(cond)};
      if (errn == INVALID)
        err = err::E_XXX;
    }
  }

  t_errn call_pthread_cond_init(r_pthread_cond cond,
                                R_pthread_condattr attr) noexcept {
     return t_errn{::pthread_cond_init(&cond, &attr)};
  }

  t_void call_pthread_cond_init(t_err err, r_pthread_cond cond,
                                R_pthread_condattr attr) noexcept {
    ERR_GUARD(err) {
      auto errn{call_pthread_cond_init(cond, attr)};
      if (errn == INVALID)
        err = err::E_XXX;
    }
  }

  t_errn call_pthread_cond_destroy(r_pthread_cond cond) noexcept {
     return t_errn{::pthread_cond_destroy(&cond)};
  }

  t_void call_pthread_cond_destroy(t_err err, r_pthread_cond cond) noexcept {
    ERR_GUARD(err) {
      auto errn{call_pthread_cond_destroy(cond)};
      if (errn == INVALID)
        err = err::E_XXX;
    }
  }

  t_errn call_pthread_cond_signal(r_pthread_cond cond) noexcept {
     return t_errn{::pthread_cond_signal(&cond)};
  }

  t_void call_pthread_cond_signal(t_err err, r_pthread_cond cond) noexcept {
    ERR_GUARD(err) {
      auto errn{call_pthread_cond_signal(cond)};
      if (errn == INVALID)
        err = err::E_XXX;
    }
  }

  t_errn call_pthread_cond_broadcast(r_pthread_cond cond) noexcept {
     return t_errn{::pthread_cond_broadcast(&cond)};
  }

  t_void call_pthread_cond_broadcast(t_err err, r_pthread_cond cond) noexcept {
    ERR_GUARD(err) {
      auto errn{call_pthread_cond_broadcast(cond)};
      if (errn == INVALID)
        err = err::E_XXX;
    }
  }

  t_errn call_pthread_cond_wait(r_pthread_cond cond,
                                r_pthread_mutex mutex) noexcept {
     return t_errn{::pthread_cond_wait(&cond, &mutex)};
  }

  t_void call_pthread_cond_wait(t_err err, r_pthread_cond cond,
                                r_pthread_mutex mutex) noexcept {
    ERR_GUARD(err) {
      auto errn{call_pthread_cond_wait(cond, mutex)};
      if (errn == INVALID)
        err = err::E_XXX;
    }
  }

  t_errn call_pthread_cond_timedwait(r_pthread_cond cond,
                                     r_pthread_mutex mutex,
                                     R_timespec spec) noexcept {
     return t_errn{::pthread_cond_timedwait(&cond, &mutex, &spec)};
  }

  t_void call_pthread_cond_timedwait(t_err err, r_pthread_cond cond,
                                     r_pthread_mutex mutex,
                                     R_timespec spec) noexcept {
    ERR_GUARD(err) {
      auto errn{call_pthread_cond_timedwait(cond, mutex, spec)};
      if (errn == INVALID)
        err = err::E_XXX;
    }
  }

///////////////////////////////////////////////////////////////////////////////

  t_errn call_pthread_set_detach(r_pthread_attr attr) noexcept {
    return t_errn{::pthread_attr_setdetachstate(&attr,
                                                PTHREAD_CREATE_DETACHED)};
  }

  t_void call_pthread_set_detach(t_err err, r_pthread_attr attr) noexcept {
    ERR_GUARD(err) {
      auto errn{call_pthread_set_detach(attr)};
      if (errn == INVALID)
        err = err::E_XXX;
    }
  }

  t_bool call_pthread_is_detach(R_pthread_attr attr) noexcept {
    int state;
    if (::pthread_attr_getdetachstate(&attr, &state) == 0)
      return state == PTHREAD_CREATE_DETACHED;
    //assert();
    return false;
  }

  t_bool call_pthread_is_detach(t_err err, R_pthread_attr attr) noexcept {
    ERR_GUARD(err) {
      int state = 0;
      if (::pthread_attr_getdetachstate(&attr, &state) == 0)
        return state == PTHREAD_CREATE_DETACHED;
      err = err::E_XXX;
    }
    return false;
  }

  ::pthread_t call_pthread_self() noexcept {
     return ::pthread_self();
  }

  t_bool call_pthread_equal(R_pthread p1, R_pthread p2) noexcept {
    return ::pthread_equal(p1, p2);
  }

  t_errn call_pthread_create(r_pthread thread, p_run run,
                             p_void arg) noexcept {
    return t_errn{::pthread_create(&thread, NULL, run, arg)};
  }

  t_void call_pthread_create(t_err err, r_pthread thread, p_run run,
                             p_void arg) noexcept {
    ERR_GUARD(err) {
      auto errn{call_pthread_create(thread, run, arg)};
      if (errn == INVALID)
        err = err::E_XXX;
    }
  }

  t_errn call_pthread_create(r_pthread thread, R_pthread_attr attr,
                             p_run run, p_void arg) noexcept {
    return t_errn{::pthread_create(&thread, &attr, run, arg)};
  }

  t_void call_pthread_create(t_err err, r_pthread thread,
                             R_pthread_attr attr, p_run run,
                             p_void arg) noexcept {
    ERR_GUARD(err) {
      auto errn{call_pthread_create(thread, attr, run, arg)};
      if (errn == INVALID)
        err = err::E_XXX;
    }
  }

  t_errn call_pthread_detach(r_pthread thread) noexcept {
    return t_errn{::pthread_detach(thread)};
  }

  t_void call_pthread_detach(t_err err, r_pthread thread) noexcept {
    ERR_GUARD(err) {
      auto errn{call_pthread_detach(thread)};
      if (errn == INVALID)
        err = err::E_XXX;
    }
  }

  t_errn call_pthread_join(r_pthread thread) noexcept {
    return t_errn{::pthread_join(thread, NULL)};
  }

  t_void call_pthread_join(t_err err, r_pthread thread) noexcept {
    ERR_GUARD(err) {
      auto errn{call_pthread_join(thread)};
      if (errn == INVALID)
        err = err::E_XXX;
    }
  }

  t_errn call_pthread_join(r_pthread thread, p_void& arg) noexcept {
    return t_errn{::pthread_join(thread, &arg)};
  }

  t_void call_pthread_join(t_err err, r_pthread thread, p_void& arg) noexcept {
    ERR_GUARD(err) {
      auto errn{call_pthread_join(thread, arg)};
      if (errn == INVALID)
        err = err::E_XXX;
    }
  }

  t_errn call_pthread_cancel(r_pthread thread) noexcept {
    return t_errn{::pthread_cancel(thread)};
  }

  t_void call_pthread_cancel(t_err err, r_pthread thread) noexcept {
    ERR_GUARD(err) {
      auto errn{call_pthread_cancel(thread)};
      if (errn == INVALID)
        err = err::E_XXX;
    }
  }

  t_void call_pthread_exit(p_void arg) noexcept {
    ::pthread_exit(arg);
  }

  t_void call_pthread_exit(t_err err, p_void arg) noexcept {
    ERR_GUARD(err) {
      call_pthread_exit(arg);
    }
  }

  t_errn call_pthread_setname_np(t_pthread thread, P_cstr name) noexcept {
    return t_errn{::pthread_setname_np(thread, get(name))};
  }

  t_void call_pthread_setname_np(t_err err, t_pthread thread,
                                 P_cstr name) noexcept {
    ERR_GUARD(err) {
      auto errn{call_pthread_setname_np(thread, name)};
      if (errn == INVALID)
        err = err::E_XXX;
    }
  }

  t_errn call_pthread_getname_np(t_pthread thread, p_cstr name,
                                t_n len) noexcept {
    return t_errn{::pthread_getname_np(thread, get(name), get(len))};
  }

  t_void call_pthread_getname_np(t_err err, t_pthread thread, p_cstr name,
                                 t_n len) noexcept {
    ERR_GUARD(err) {
      auto errn{call_pthread_getname_np(thread, name, len)};
      if (errn == INVALID)
        err = err::E_XXX;
    }
  }

///////////////////////////////////////////////////////////////////////////////

  t_errn call_clock_gettime(t_clockid clk, r_timespec spec) noexcept {
    return t_errn{::clock_gettime(clk, &spec)};
  }

  t_void call_clock_gettime(t_err err, t_clockid clk,
                            r_timespec spec) noexcept {
    ERR_GUARD(err) {
      auto errn{call_clock_gettime(clk, spec)};
      if (errn == INVALID)
        err = err::E_XXX;
    }
  }

  t_errn call_clock_gettime_monotonic(r_timespec spec) noexcept {
    return t_errn{::clock_gettime(CLOCK_MONOTONIC, &spec)};
  }

  t_void call_clock_gettime_monotonic(t_err err, r_timespec spec) noexcept {
    ERR_GUARD(err) {
      auto errn{call_clock_gettime_monotonic(spec)};
      if (errn == INVALID)
        err = err::E_XXX;
    }
  }

  t_errn call_clock_gettime_realtime(r_timespec spec) noexcept {
    return t_errn{::clock_gettime(CLOCK_REALTIME, &spec)};
  }

  t_void call_clock_gettime_realtime(t_err err, r_timespec spec) noexcept {
    ERR_GUARD(err) {
      auto errn{call_clock_gettime_realtime(spec)};
      if (errn == INVALID)
        err = err::E_XXX;
    }
  }

///////////////////////////////////////////////////////////////////////////////

  t_verify<t_fd> call_epoll_create() noexcept {
    auto fd = ::epoll_create1(0);
    if (fd >= 0)
      return {t_fd(fd), t_errn{0}};
    return {BAD_FD, t_errn{fd}};
  }

  t_fd call_epoll_create(t_err err) noexcept {
    ERR_GUARD(err) {
      auto verify = call_epoll_create();
      if (verify == VALID)
        return verify.value;
      err = err::E_XXX;
    }
    return BAD_FD;
  }

  t_errn call_epoll_ctl_add(t_fd efd, t_fd fd, r_epoll_event event) noexcept {
    return t_errn{::epoll_ctl(get(efd), EPOLL_CTL_ADD, get(fd), &event)};
  }

  t_void call_epoll_ctl_add(t_err err, t_fd efd, t_fd fd,
                            r_epoll_event event) noexcept {
    ERR_GUARD(err) {
      auto errn{call_epoll_ctl_add(efd, fd, event)};
      if (errn == INVALID)
        err = err::E_XXX;
    }
  }

  t_errn call_epoll_ctl_mod(t_fd efd, t_fd fd, r_epoll_event event) noexcept {
    return t_errn{::epoll_ctl(get(efd), EPOLL_CTL_MOD, get(fd), &event)};
  }

  t_void call_epoll_ctl_mod(t_err err, t_fd efd, t_fd fd,
                            r_epoll_event event) noexcept {
    ERR_GUARD(err) {
      auto errn{call_epoll_ctl_mod(efd, fd, event)};
      if (errn == INVALID)
        err = err::E_XXX;
    }
  }

  t_errn call_epoll_ctl_del(t_fd efd, t_fd fd) noexcept {
    return t_errn{::epoll_ctl(get(efd), EPOLL_CTL_DEL, get(fd), NULL)};
  }

  t_void call_epoll_ctl_del(t_err err, t_fd efd, t_fd fd) noexcept {
    ERR_GUARD(err) {
      auto errn{call_epoll_ctl_del(efd, fd)};
      if (errn == INVALID)
        err = err::E_XXX;
    }
  }

  t_verify<t_n> call_epoll_wait(t_fd efd, p_epoll_event events, t_n max) noexcept {
    auto ret = ::epoll_wait(get(efd), events, get(max), -1);
    if (ret >= 0)
      return {t_n(ret), t_errn{0}};
    return {t_n{0}, t_errn{ret}};
  }

  t_n call_epoll_wait(t_err err, t_fd efd, p_epoll_event events,
                      t_n max) noexcept {
    ERR_GUARD(err) {
      auto verify = call_epoll_wait(efd, events, max);
      if (verify == VALID)
        return verify.value;
      err = err::E_XXX;
    }
    return t_n{0};
  }

  t_verify<t_n> call_epoll_wait(t_fd efd, p_epoll_event events, t_n max,
                                 t_usec usec) noexcept {
    auto ret = ::epoll_wait(get(efd), events, get(max), get(usec));
    if (ret >= 0)
      return {t_n(ret), t_errn{0}};
    return {t_n{0}, t_errn{ret}};
  }

  t_n call_epoll_wait(t_err err, t_fd efd, p_epoll_event events,
                      t_n max, t_usec usec) noexcept {
    ERR_GUARD(err) {
      auto verify = call_epoll_wait(efd, events, max, usec);
      if (verify == VALID)
        return verify.value;
      err = err::E_XXX;
    }
    return t_n{0};
  }

///////////////////////////////////////////////////////////////////////////////

  t_verify<t_fd> call_eventfd(t_n cnt) noexcept {
    auto fd = ::eventfd(get(cnt), 0);
    if (fd >= 0)
      return {t_fd{fd}, t_errn{0}};
    return {BAD_FD, t_errn{fd}};
  }

  t_fd call_eventfd(t_err err, t_n cnt) noexcept {
    ERR_GUARD(err) {
      auto verify = call_eventfd(cnt);
      if (verify == VALID)
        return verify.value;
      err = err::E_XXX;
    }
    return BAD_FD;
  }

///////////////////////////////////////////////////////////////////////////////

  t_errn call_close(t_fd& fd) noexcept {
    if (fd != BAD_FD)
      return t_errn{::close(get(named::utility::reset(fd, BAD_FD)))};
    return t_errn{-1};
  }

  t_void call_close(t_err err, t_fd& fd) noexcept {
    ERR_GUARD(err) {
      if (fd != BAD_FD)
        ::close(get(named::utility::reset(fd, BAD_FD)));
      else
        err = err::E_XXX;
    }
  }

///////////////////////////////////////////////////////////////////////////////

  t_verify<t_fd> call_timerfd_create(t_flags flags) noexcept {
    auto fd = ::timerfd_create(CLOCK_MONOTONIC, get(flags));
    if (fd >= 0)
      return {t_fd{fd}, t_errn{0}};
    return {BAD_FD, t_errn{fd}};
  }

  t_fd call_timerfd_create(t_err err, t_flags flags) noexcept {
    ERR_GUARD(err) {
      auto verify = call_timerfd_create(flags);
      if (verify == VALID)
        return verify.value;
      err = err::E_XXX;
    }
    return BAD_FD;
  }

  t_errn call_timerfd_settime(t_fd fd, t_flags flags,
                              R_itimerspec timer) noexcept {
    return t_errn{::timerfd_settime(get(fd), get(flags), &timer, NULL)};
  }

  t_void call_timerfd_settime(t_err err, t_fd fd, t_flags flags,
                              R_itimerspec timer) noexcept {
    ERR_GUARD(err) {
      auto ret = call_timerfd_settime(fd, flags, timer);
      if (ret == INVALID)
        err = err::E_XXX;
    }
  }

  t_errn call_timerfd_settime(t_fd fd, t_flags flags, R_itimerspec ntimer,
                              r_itimerspec otimer) noexcept {
    return t_errn{::timerfd_settime(get(fd), get(flags), &ntimer, &otimer)};
  }

  t_void call_timerfd_settime(t_err err, t_fd fd, t_flags flags,
                              R_itimerspec ntimer,
                              r_itimerspec otimer) noexcept {
    ERR_GUARD(err) {
      auto ret = call_timerfd_settime(fd, flags, ntimer, otimer);
      if (ret == INVALID)
        err = err::E_XXX;
    }
  }

  t_errn call_timerfd_gettime(t_fd fd, r_itimerspec timer) noexcept {
    return t_errn{::timerfd_gettime(get(fd),&timer)};
  }

  t_void call_timerfd_gettime(t_err err, t_fd fd,
                              r_itimerspec timer) noexcept {
    ERR_GUARD(err) {
      auto ret = call_timerfd_gettime(fd, timer);
      if (ret == INVALID)
        err = err::E_XXX;
    }
  }

///////////////////////////////////////////////////////////////////////////////

  t_verify<t_n> call_read(t_fd fd, p_void buf, t_n cnt) noexcept {
    auto ret = ::read(get(fd), buf, get(cnt));
    if (ret >= 0)
      return {t_n(ret), t_errn{0}};
    return {t_n{0}, t_errn(ret)};
  }

  t_n call_read(t_err err, t_fd fd, p_void buf, t_n cnt) noexcept {
    ERR_GUARD(err) {
      auto verify = call_read(fd, buf, cnt);
      if (verify == VALID)
        return verify.value;
      err = err::E_XXX; // assign err
    }
    return t_n{0};
  }

  t_verify<t_n> call_write(t_fd fd, P_void buf, t_n cnt) noexcept {
    auto ret = ::write(get(fd), buf, get(cnt));
    if (ret >= 0)
      return {t_n(ret), t_errn{0}};
    return {t_n{0}, t_errn(ret)};
  }

  t_n call_write(t_err err, t_fd fd, P_void buf, t_n cnt) noexcept {
    ERR_GUARD(err) {
      auto verify = call_write(fd, buf, cnt);
      if (verify == VALID)
        return verify.value;
      err = err::E_XXX; // assign err
    }
    return t_n{0};
  }

///////////////////////////////////////////////////////////////////////////////
}
}
