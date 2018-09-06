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

#ifndef _DAINTY_OS_CALL_H_
#define _DAINTY_OS_CALL_H_

#include <sys/timerfd.h>
#include <sys/eventfd.h>
#include <sys/epoll.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include "dainty_named.h"
#include "dainty_os_err.h"

namespace dainty
{
namespace os
{
  using named::p_void;
  using named::P_void;
  using named::t_void;
  using named::t_bool;
  using named::t_int;
  using named::t_usec;
  using named::t_fd_;
  using named::t_fd;
  using named::t_errn;
  using named::t_validity;
  using named::t_n;
  using named::p_cstr;
  using named::t_prefix;
  using named::P_cstr;
  using named::VALID;
  using named::INVALID;
  using named::BAD_FD;
  using err::t_err;

  template<typename T>
  using t_verify = named::t_verifiable<T>;

  enum  t_flags_tag_ {};
  using t_flags_ = named::t_int;
  using t_flags  = named::t_explicit<t_flags_, t_flags_tag_>;

  using t_pthread_mutexattr = t_prefix<::pthread_mutexattr_t>::t_;
  using r_pthread_mutexattr = t_prefix<::pthread_mutexattr_t>::r_;
  using R_pthread_mutexattr = t_prefix<::pthread_mutexattr_t>::R_;

  using t_pthread_mutex     = t_prefix<::pthread_mutex_t>::t_;
  using r_pthread_mutex     = t_prefix<::pthread_mutex_t>::r_;
  using R_pthread_mutex     = t_prefix<::pthread_mutex_t>::R_;

  using t_pthread_condattr  = t_prefix<::pthread_condattr_t>::t_;
  using r_pthread_condattr  = t_prefix<::pthread_condattr_t>::r_;
  using R_pthread_condattr  = t_prefix<::pthread_condattr_t>::R_;

  using t_pthread_cond      = t_prefix<::pthread_cond_t>::t_;
  using r_pthread_cond      = t_prefix<::pthread_cond_t>::r_;
  using R_pthread_cond      = t_prefix<::pthread_cond_t>::R_;

  using t_pthread_attr      = t_prefix<::pthread_attr_t>::t_;
  using r_pthread_attr      = t_prefix<::pthread_attr_t>::r_;
  using R_pthread_attr      = t_prefix<::pthread_attr_t>::R_;

  using t_pthread           = t_prefix<::pthread_t>::t_;
  using r_pthread           = t_prefix<::pthread_t>::r_;
  using R_pthread           = t_prefix<::pthread_t>::R_;

  using t_timespec          = t_prefix<::timespec>::t_;
  using r_timespec          = t_prefix<::timespec>::r_;
  using R_timespec          = t_prefix<::timespec>::R_;

  using t_clockid           = t_prefix<::clockid_t>::t_;

  using t_epoll_event       = t_prefix<::epoll_event>::t_;
  using r_epoll_event       = t_prefix<::epoll_event>::r_;
  using p_epoll_event       = t_prefix<::epoll_event>::p_;

  using t_itimerspec        = t_prefix<::itimerspec>::t_;
  using r_itimerspec        = t_prefix<::itimerspec>::r_;
  using R_itimerspec        = t_prefix<::itimerspec>::R_;

///////////////////////////////////////////////////////////////////////////////

  using p_run = p_void (*)(p_void);

///////////////////////////////////////////////////////////////////////////////

  t_errn call_pthread_init(       r_pthread_mutexattr) noexcept;
  t_void call_pthread_init(t_err, r_pthread_mutexattr) noexcept;

  t_errn call_pthread_destroy(       r_pthread_mutexattr) noexcept;
  t_void call_pthread_destroy(t_err, r_pthread_mutexattr) noexcept;

  t_errn call_pthread_set_recursive(       r_pthread_mutexattr) noexcept;
  t_void call_pthread_set_recursive(t_err, r_pthread_mutexattr) noexcept;

  t_bool call_pthread_is_recursive(       R_pthread_mutexattr) noexcept;
  t_bool call_pthread_is_recursive(t_err, R_pthread_mutexattr) noexcept;

///////////////////////////////////////////////////////////////////////////////

  t_errn call_pthread_mutex_init(       r_pthread_mutex) noexcept;
  t_void call_pthread_mutex_init(t_err, r_pthread_mutex) noexcept;

  t_errn call_pthread_mutex_init(       r_pthread_mutex,
                                        R_pthread_mutexattr) noexcept;
  t_void call_pthread_mutex_init(t_err, r_pthread_mutex,
                                        R_pthread_mutexattr) noexcept;

  t_errn call_pthread_mutex_destroy(       r_pthread_mutex) noexcept;
  t_void call_pthread_mutex_destroy(t_err, r_pthread_mutex) noexcept;

  t_errn call_pthread_mutex_lock(       r_pthread_mutex) noexcept;
  t_void call_pthread_mutex_lock(t_err, r_pthread_mutex) noexcept;

  t_errn call_pthread_mutex_timedlock(       r_pthread_mutex,
                                             R_timespec) noexcept;
  t_void call_pthread_mutex_timedlock(t_err, r_pthread_mutex,
                                             R_timespec) noexcept;

  t_errn call_pthread_mutex_trylock(       r_pthread_mutex) noexcept;
  t_void call_pthread_mutex_trylock(t_err, r_pthread_mutex) noexcept;

  t_errn call_pthread_mutex_unlock(       r_pthread_mutex) noexcept;
  t_void call_pthread_mutex_unlock(t_err, r_pthread_mutex) noexcept;

///////////////////////////////////////////////////////////////////////////////

  t_errn call_pthread_init(       r_pthread_condattr) noexcept;
  t_void call_pthread_init(t_err, r_pthread_condattr) noexcept;

  t_errn call_pthread_destroy(       r_pthread_condattr) noexcept;
  t_void call_pthread_destroy(t_err, r_pthread_condattr) noexcept;

  t_errn call_pthread_set_monotonic(       r_pthread_condattr) noexcept;
  t_void call_pthread_set_monotonic(t_err, r_pthread_condattr) noexcept;

  t_bool call_pthread_is_monotonic(       R_pthread_condattr) noexcept;
  t_bool call_pthread_is_monotonic(t_err, R_pthread_condattr) noexcept;

///////////////////////////////////////////////////////////////////////////////

  t_errn call_pthread_cond_init(       r_pthread_cond) noexcept;
  t_void call_pthread_cond_init(t_err, r_pthread_cond) noexcept;

  t_errn call_pthread_cond_init(       r_pthread_cond,
                                       R_pthread_condattr) noexcept;
  t_void call_pthread_cond_init(t_err, r_pthread_cond,
                                       R_pthread_condattr) noexcept;

  t_errn call_pthread_cond_destroy(       r_pthread_cond) noexcept;
  t_void call_pthread_cond_destroy(t_err, r_pthread_cond) noexcept;

  t_errn call_pthread_cond_signal(       r_pthread_cond) noexcept;
  t_void call_pthread_cond_signal(t_err, r_pthread_cond) noexcept;

  t_errn call_pthread_cond_broadcast(       r_pthread_cond) noexcept;
  t_void call_pthread_cond_broadcast(t_err, r_pthread_cond) noexcept;

  t_errn call_pthread_cond_wait(       r_pthread_cond,
                                       r_pthread_mutex) noexcept;
  t_void call_pthread_cond_wait(t_err, r_pthread_cond,
                                       r_pthread_mutex) noexcept;

  t_errn call_pthread_cond_timedwait(       r_pthread_cond, r_pthread_mutex,
                                            R_timespec) noexcept;
  t_void call_pthread_cond_timedwait(t_err, r_pthread_cond, r_pthread_mutex,
                                            R_timespec) noexcept;

///////////////////////////////////////////////////////////////////////////////

  t_errn call_pthread_set_detach(       r_pthread_attr) noexcept;
  t_void call_pthread_set_detach(t_err, r_pthread_attr) noexcept;

  t_bool call_pthread_is_detach(       R_pthread_attr) noexcept;
  t_bool call_pthread_is_detach(t_err, R_pthread_attr) noexcept;

  t_pthread call_pthread_self() noexcept;
  t_pthread call_pthread_self() noexcept;
  t_bool    call_pthread_equal(R_pthread, R_pthread) noexcept;

  t_errn call_pthread_create(       r_pthread, p_run, p_void) noexcept;
  t_void call_pthread_create(t_err, r_pthread, p_run, p_void) noexcept;

  t_errn call_pthread_create(       r_pthread, R_pthread_attr, p_run,
                                    p_void) noexcept;
  t_void call_pthread_create(t_err, r_pthread, R_pthread_attr, p_run,
                                    p_void) noexcept;

  t_errn call_pthread_detach(       r_pthread)      noexcept;
  t_void call_pthread_detach(t_err, r_pthread) noexcept;

  t_errn call_pthread_join(       r_pthread) noexcept;
  t_void call_pthread_join(t_err, r_pthread) noexcept;

  t_errn call_pthread_join(       r_pthread, p_void&) noexcept;
  t_void call_pthread_join(t_err, r_pthread, p_void&) noexcept;

  t_errn call_pthread_cancel(       r_pthread) noexcept;
  t_void call_pthread_cancel(t_err, r_pthread) noexcept;

  t_void call_pthread_exit(       p_void) noexcept;
  t_void call_pthread_exit(t_err, p_void) noexcept;

  t_errn call_pthread_setname_np(       t_pthread, P_cstr) noexcept;
  t_void call_pthread_setname_np(t_err, t_pthread, P_cstr) noexcept;

  t_errn call_pthread_getname_np(       t_pthread, p_cstr, t_n) noexcept;
  t_void call_pthread_getname_np(t_err, t_pthread, p_cstr, t_n) noexcept;

///////////////////////////////////////////////////////////////////////////////

  t_errn call_clock_gettime(       t_clockid, r_timespec) noexcept;
  t_void call_clock_gettime(t_err, t_clockid, r_timespec) noexcept;

  t_errn call_clock_gettime_monotonic(       r_timespec) noexcept;
  t_void call_clock_gettime_monotonic(t_err, r_timespec) noexcept;

  t_errn call_clock_gettime_realtime(       r_timespec) noexcept;
  t_void call_clock_gettime_realtime(t_err, r_timespec) noexcept;

///////////////////////////////////////////////////////////////////////////////

  t_verify<t_fd> call_epoll_create()      noexcept;
  t_fd           call_epoll_create(t_err) noexcept;

  t_errn call_epoll_ctl_add(       t_fd, t_fd, r_epoll_event) noexcept;
  t_void call_epoll_ctl_add(t_err, t_fd, t_fd, r_epoll_event) noexcept;

  t_errn call_epoll_ctl_mod(       t_fd, t_fd, r_epoll_event) noexcept;
  t_void call_epoll_ctl_mod(t_err, t_fd, t_fd, r_epoll_event) noexcept;

  t_errn call_epoll_ctl_del(       t_fd, t_fd) noexcept;
  t_void call_epoll_ctl_del(t_err, t_fd, t_fd) noexcept;

  t_verify<t_n> call_epoll_wait(       t_fd, p_epoll_event, t_n max) noexcept;
  t_n           call_epoll_wait(t_err, t_fd, p_epoll_event, t_n max) noexcept;

  t_verify<t_n> call_epoll_wait(       t_fd, p_epoll_event, t_n max,
                                       t_usec) noexcept;
  t_n           call_epoll_wait(t_err, t_fd, p_epoll_event, t_n max,
                                       t_usec) noexcept;

///////////////////////////////////////////////////////////////////////////////

  t_verify<t_fd> call_timerfd_create(       t_flags) noexcept;
  t_fd           call_timerfd_create(t_err, t_flags) noexcept;

  t_errn call_timerfd_settime(       t_fd, t_flags, R_itimerspec) noexcept;
  t_void call_timerfd_settime(t_err, t_fd, t_flags, R_itimerspec) noexcept;

  t_errn call_timerfd_settime(       t_fd, t_flags, R_itimerspec,
                                                    r_itimerspec) noexcept;
  t_void call_timerfd_settime(t_err, t_fd, t_flags, R_itimerspec,
                                                    r_itimerspec) noexcept;

  t_errn call_timerfd_gettime(       t_fd, r_itimerspec) noexcept;
  t_void call_timerfd_gettime(t_err, t_fd, r_itimerspec) noexcept;

///////////////////////////////////////////////////////////////////////////////

  t_verify<t_fd> call_eventfd(       t_n) noexcept;
  t_fd           call_eventfd(t_err, t_n) noexcept;

///////////////////////////////////////////////////////////////////////////////

  t_errn     call_close(       t_fd&) noexcept;
  t_void     call_close(t_err, t_fd&) noexcept;

///////////////////////////////////////////////////////////////////////////////

  t_verify<t_n> call_read(        t_fd, p_void, t_n count) noexcept;
  t_n           call_read(t_err,  t_fd, p_void, t_n count) noexcept;

  t_verify<t_n> call_write(       t_fd, P_void, t_n count) noexcept;
  t_n           call_write(t_err, t_fd, P_void, t_n count) noexcept;

  // readv, writev

///////////////////////////////////////////////////////////////////////////////

  // signal

///////////////////////////////////////////////////////////////////////////////

  // socket api

///////////////////////////////////////////////////////////////////////////////

  // pipe

///////////////////////////////////////////////////////////////////////////////

  // time

///////////////////////////////////////////////////////////////////////////////
}
}

#endif
