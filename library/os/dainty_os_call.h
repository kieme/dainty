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

#include <sys/types.h>
#include <sys/timerfd.h>
#include <sys/eventfd.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include "dainty_base.h"
#include "dainty_base_range.h"
#include "dainty_os_err.h"

namespace dainty
{
namespace os
{
///////////////////////////////////////////////////////////////////////////////

  using err::t_err;

  using base::t_void_size_ptr;
  using base::t_verifiable;

  using base::types::t_prefix;
  using base::types::p_void;
  using base::types::P_void;
  using base::types::t_void;
  using base::types::t_bool;
  using base::types::t_int;
  using base::types::t_fd_;
  using base::types::t_n_;
  using base::types::t_size_;
  //using base::types::NO_ERRN_;
  //using base::types::BAD_ERRN_;

  using base::specific::t_specific;
  using base::specific::P_cstr;
  using base::specific::t_n;
  using base::specific::p_cstr;
  using base::specific::t_msec;
  using base::specific::t_fd;
  using base::specific::t_errn;
  using base::specific::t_validity;

  using base::specific::NO_ERRN;
  using base::specific::BAD_ERRN;
  using base::specific::VALID;
  using base::specific::INVALID;
  using base::specific::BAD_FD;

  using base::range::t_range;
  using base::range::t_crange;
  using base::range::t_byte_range;
  using base::range::R_byte_range;
  using base::range::r_byte_range;
  using base::range::t_byte_crange;
  using base::range::R_byte_crange;
  using base::range::r_byte_crange;

///////////////////////////////////////////////////////////////////////////////

  template<typename T>
  using t_verify = t_verifiable<T>;

  enum  t_flags_tag_ {};
  using t_flags_ = t_int;
  using t_flags  = t_specific<t_flags_, t_flags_tag_>;
  constexpr t_flags NO_FLAGS{0};

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

  enum  t_pthread_attr_stacksize_tag {};
  using t_pthread_attr_stacksize = t_specific<t_size_,
                                     t_pthread_attr_stacksize_tag>;

  enum  t_pthread_attr_guardsize_tag {};
  using t_pthread_attr_guardsize = t_specific<t_size_,
                                     t_pthread_attr_guardsize_tag>;

///////////////////////////////////////////////////////////////////////////////

  using t_socket_msghdr         = ::msghdr;
  using R_socket_msghdr         = t_prefix<t_socket_msghdr>::R_;
  using r_socket_msghdr         = t_prefix<t_socket_msghdr>::r_;

  using t_socket_mmsghdr        = ::mmsghdr;
  enum t_socket_msghdr_range_tag_ {};
  using t_socket_msghdr_range   = t_range<t_socket_msghdr,
                                          t_socket_msghdr_range_tag_>;
  using R_socket_msghdr_range   = t_prefix<t_socket_msghdr_range>::R_;
  using r_socket_msghdr_range   = t_prefix<t_socket_msghdr_range>::r_;

  using t_socket_msghdr_crange  = t_crange<t_socket_msghdr,
                                           t_socket_msghdr_range_tag_>;
  using R_socket_msghdr_crange  = t_prefix<t_socket_msghdr_crange>::R_;
  using r_socket_msghdr_crange  = t_prefix<t_socket_msghdr_crange>::r_;

  enum  t_socket_domain_tag_ {};
  using t_socket_domain_  = t_int;
  using t_socket_domain   = t_specific<t_socket_domain_, t_socket_domain_tag_>;

  enum  t_socket_type_tag_ {};
  using t_socket_type_ = t_int;
  using t_socket_type  = t_specific<t_socket_type_, t_socket_type_tag_>;

  enum  t_socket_protocol_tag_ {};
  using t_socket_protocol_ = t_int;
  using t_socket_protocol  = t_specific<t_socket_protocol_,
                                        t_socket_protocol_tag_>;

  enum  t_socket_backlog_tag_ {};
  using t_socket_backlog_ = t_int;
  using t_socket_backlog  = t_specific<t_socket_backlog_,
                                       t_socket_backlog_tag_>;

  enum  t_socket_howto_tag_ {};
  using t_socket_howto_ = t_int;
  using t_socket_howto  = t_specific<t_socket_howto_, t_socket_howto_tag_>;

  enum  t_socket_level_tag_ {};
  using t_socket_level_ = t_int;
  using t_socket_level  = t_specific<t_socket_level_, t_socket_level_tag_>;

///////////////////////////////////////////////////////////////////////////////

  enum  t_socket_option_name_tag_ {};
  using t_socket_option_name_ = t_int;
  using t_socket_option_name  = t_specific<t_socket_option_name_,
                                           t_socket_option_name_tag_>;

  enum  t_socket_option_len_tag_ {};
  using t_socket_option_len_ = ::socklen_t; //XXX
  using t_socket_option_len  = t_specific<t_socket_option_len_,
                                          t_socket_option_len_tag_>;

  class t_socket_option {
  public:
    t_socket_option(t_socket_option_name _name,
                    t_socket_option_len  _len) noexcept
      : name{_name}, len{_len} {
    }

    virtual ~t_socket_option() {}

    virtual operator p_void  ()       noexcept = 0;
    virtual operator P_void  () const noexcept = 0;

    t_socket_option_name name = t_socket_option_name{0};
    t_socket_option_len  len  = t_socket_option_len {0};
  };
  using r_socket_option = t_prefix<t_socket_option>::r_;
  using R_socket_option = t_prefix<t_socket_option>::R_;

///////////////////////////////////////////////////////////////////////////////

  using t_sockaddr = ::sockaddr;
  using p_sockaddr = t_prefix<t_sockaddr>::p_;
  using P_sockaddr = t_prefix<t_sockaddr>::P_;

  enum  t_socket_address_len_tag_ {};
  using t_socket_address_len_ = ::socklen_t; // XXX
  using t_socket_address_len  = t_specific<t_socket_address_len_,
                                           t_socket_address_len_tag_>;

  class t_socket_address {
  public:
    using p_sockaddr         = os::p_sockaddr;
    using P_sockaddr         = os::P_sockaddr;
    using t_sock_address_len = os::t_socket_address_len;

    t_socket_address(t_socket_address_len _len) noexcept : len{_len} {
    }

    virtual ~t_socket_address() {}

    virtual operator p_sockaddr()       noexcept = 0;
    virtual operator P_sockaddr() const noexcept = 0;

    t_socket_address_len len;
  };
  using r_socket_address = t_prefix<t_socket_address>::r_;
  using R_socket_address = t_prefix<t_socket_address>::R_;
  using p_socket_address = t_prefix<t_socket_address>::p_;
  using P_socket_address = t_prefix<t_socket_address>::P_;

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

  t_errn call_pthread_init(       r_pthread_attr) noexcept;
  t_void call_pthread_init(t_err, r_pthread_attr) noexcept;

  t_errn call_pthread_set_stacksize(       r_pthread_attr,
                                           t_pthread_attr_stacksize) noexcept;
  t_void call_pthread_set_stacksize(t_err, r_pthread_attr,
                                           t_pthread_attr_stacksize) noexcept;

  t_errn call_pthread_set_guardsize(       r_pthread_attr,
                                           t_pthread_attr_guardsize) noexcept;
  t_void call_pthread_set_guardsize(t_err, r_pthread_attr,
                                           t_pthread_attr_guardsize) noexcept;

  t_errn call_pthread_set_schedpolicy_other(       r_pthread_attr) noexcept;
  t_void call_pthread_set_schedpolicy_other(t_err, r_pthread_attr) noexcept;

  t_errn call_pthread_set_inheritsched_explicit(       r_pthread_attr) noexcept;
  t_void call_pthread_set_inheritsched_explicit(t_err, r_pthread_attr) noexcept;

  t_errn call_pthread_set_detach(       r_pthread_attr) noexcept;
  t_void call_pthread_set_detach(t_err, r_pthread_attr) noexcept;

  t_bool call_pthread_is_detach(       R_pthread_attr) noexcept;
  t_bool call_pthread_is_detach(t_err, R_pthread_attr) noexcept;

///////////////////////////////////////////////////////////////////////////////

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
                                       t_msec) noexcept;
  t_n           call_epoll_wait(t_err, t_fd, p_epoll_event, t_n max,
                                       t_msec) noexcept;

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

  t_verify<t_fd> call_socket(       t_socket_domain, t_socket_type,
                                    t_socket_protocol) noexcept;
  t_fd           call_socket(t_err, t_socket_domain, t_socket_type,
                                    t_socket_protocol) noexcept;

  t_errn call_bind(       t_fd, R_socket_address) noexcept;
  t_void call_bind(t_err, t_fd, R_socket_address) noexcept;

  t_errn call_connect(       t_fd, R_socket_address) noexcept;
  t_void call_connect(t_err, t_fd, R_socket_address) noexcept;

  t_errn call_listen(       t_fd, t_socket_backlog) noexcept;
  t_void call_listen(t_err, t_fd, t_socket_backlog) noexcept;

  t_verify<t_fd> call_accept(       t_fd) noexcept;
  t_fd           call_accept(t_err, t_fd) noexcept;

  t_verify<t_fd> call_accept(       t_fd, r_socket_address) noexcept;
  t_fd           call_accept(t_err, t_fd, r_socket_address) noexcept;

  t_errn call_shutdown(       t_fd, t_socket_howto) noexcept;
  t_void call_shutdown(t_err, t_fd, t_socket_howto) noexcept;

  t_errn call_getpeername(       t_fd, r_socket_address) noexcept;
  t_void call_getpeername(t_err, t_fd, r_socket_address) noexcept;

  t_errn call_getsockname(       t_fd, r_socket_address) noexcept;
  t_void call_getsockname(t_err, t_fd, r_socket_address) noexcept;

  t_errn call_getsockopt(       t_fd, t_socket_level, r_socket_option) noexcept;
  t_void call_getsockopt(t_err, t_fd, t_socket_level, r_socket_option) noexcept;

  t_errn call_setsockopt(       t_fd, t_socket_level, R_socket_option) noexcept;
  t_void call_setsockopt(t_err, t_fd, t_socket_level, R_socket_option) noexcept;

  t_verify<t_n> call_send(       t_fd, R_byte_crange, t_flags) noexcept;
  t_n           call_send(t_err, t_fd, R_byte_crange, t_flags) noexcept;

  t_verify<t_n> call_recv(       t_fd, r_byte_range, t_flags) noexcept;
  t_n           call_recv(t_err, t_fd, r_byte_range, t_flags) noexcept;

  t_verify<t_n> call_sendto(       t_fd, R_byte_crange, R_socket_address,
                                   t_flags) noexcept;
  t_n           call_sendto(t_err, t_fd, R_byte_crange, R_socket_address,
                                   t_flags) noexcept;

  t_verify<t_n> call_recvfrom(       t_fd, r_byte_range, r_socket_address,
                                     t_flags) noexcept;
  t_n           call_recvfrom(t_err, t_fd, r_byte_range, r_socket_address,
                                     t_flags) noexcept;

  t_verify<t_n> call_sendmsg(       t_fd, R_socket_msghdr, t_flags) noexcept;
  t_n           call_sendmsg(t_err, t_fd, R_socket_msghdr, t_flags) noexcept;

  t_verify<t_n> call_recvmsg(       t_fd, r_socket_msghdr, t_flags) noexcept;
  t_n           call_recvmsg(t_err, t_fd, r_socket_msghdr, t_flags) noexcept;

  t_verify<t_n> call_sendmmsg(       t_fd, R_socket_msghdr_crange, t_flags) noexcept;
  t_n           call_sendmmsg(t_err, t_fd, R_socket_msghdr_crange, t_flags) noexcept;

  t_verify<t_n> call_recvmmsg(       t_fd, r_socket_msghdr_range, t_flags) noexcept;
  t_n           call_recvmmsg(t_err, t_fd, r_socket_msghdr_range, t_flags) noexcept;

  t_verify<t_n> call_recvmmsg(       t_fd, r_socket_msghdr_range, t_flags,
                                     r_timespec) noexcept;
  t_n           call_recvmmsg(t_err, t_fd, r_socket_msghdr_range, t_flags,
                                     r_timespec) noexcept;

///////////////////////////////////////////////////////////////////////////////

  // pipe

///////////////////////////////////////////////////////////////////////////////

}
}

#endif
