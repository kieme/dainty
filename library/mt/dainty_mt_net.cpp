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

#include "dainty_os_networking.h"

namespace dainty
{
namespace os
{
namespace networking
{
///////////////////////////////////////////////////////////////////////////////

  t_socket::t_socket() noexcept : fd_{BAD_FD} {
  }

  t_socket::t_socket(t_socket_domain domain, t_socket_type type,
                     t_socket_protocol protocol) noexcept
    : fd_{call_socket(domain, type, protocol)} {
  }

  t_socket::t_socket(t_err err, t_socket_domain domain, t_socket_type type,
                     t_socket_protocol protocol) noexcept
    : fd_{call_socket(err, domain, type, protocol)} {
  }

  t_socket::t_socket(x_socket socket) noexcept : fd_{named::reset(socket.fd_)} {
  }

  t_socket::~t_socket() {
    if (fd_ != BAD_FD)
      call_close(fd_);
  }

  t_socket::operator t_validity() const noexcept {
    return fd_ == BAD_FD ? INVALID : VALID;
  }

  t_fd t_socket::get_fd() const noexcept {
    return fd_;
  }

  t_errn t_socket::create(t_socket_domain domain, t_socket_type type,
                          t_socket_protocol protocol) noexcept {
    if (fd_ == BAD_FD) {
      auto ret = call_socket(domain, type, protocol);
      if (ret == VALID)
         fd_ = ret;
      return ret.errn;
    }
    return BAD_ERRN;
  }

  t_void t_socket::create(t_err err, t_socket_domain domain,
                          t_socket_type type,
                          t_socket_protocol protocol) noexcept {
    ERR_GUARD(err) {
      if (fd_ == BAD_FD)
        fd_ = call_socket(err, domain, type, protocol);
      else
        err = err::E_XXX;
    }
  }

  t_errn t_socket::close() noexcept {
    if (fd_ != BAD_FD)
      return call_close(fd_);
    return BAD_ERRN;
  }

  t_void t_socket::close(t_err err) noexcept {
    ERR_GUARD(err) {
      if (fd_ != BAD_FD)
        call_close(err, fd_);
      else
        err = err::E_XXX;
    }
  }

  t_errn t_socket::bind(R_socket_address addr) noexcept {
    if (fd_ != BAD_FD)
      return call_bind(fd_, addr);
    return BAD_ERRN;
  }

  t_void t_socket::bind(t_err err, R_socket_address addr) noexcept {
    ERR_GUARD(err) {
      if (fd_ != BAD_FD)
        call_bind(err, fd_, addr);
      else
        err = err::E_XXX;
    }
  }

  t_errn t_socket::connect(R_socket_address addr) noexcept {
    if (fd_ != BAD_FD)
      return call_connect(fd_, addr);
    return BAD_ERRN;
  }

  t_void t_socket::connect(t_err err, R_socket_address addr) noexcept {
    ERR_GUARD(err) {
      if (fd_ != BAD_FD)
        call_connect(err, fd_, addr);
      else
        err = err::E_XXX;
    }
  }

  t_errn t_socket::listen(t_socket_backlog backlog) noexcept {
    if (fd_ != BAD_FD)
      return call_listen(fd_, backlog);
    return BAD_ERRN;
  }

  t_void t_socket::listen(t_err err, t_socket_backlog backlog) noexcept {
    ERR_GUARD(err) {
      if (fd_ != BAD_FD)
        call_listen(err, fd_, backlog);
      else
        err = err::E_XXX;
    }
  }

  t_verify<t_fd> t_socket::accept() noexcept {
    if (fd_ != BAD_FD)
      return call_accept(fd_);
    return {BAD_FD, BAD_ERRN};
  }

  t_fd t_socket::accept(t_err err) noexcept {
    ERR_GUARD(err) {
      if (fd_ != BAD_FD)
        return call_accept(err, fd_);
      err = err::E_XXX;
    }
    return BAD_FD;
  }

  t_verify<t_fd> t_socket::accept(r_socket_address addr) noexcept {
    if (fd_ != BAD_FD)
      return call_accept(fd_, addr);
    return {BAD_FD, BAD_ERRN};
  }

  t_fd t_socket::accept(t_err err, r_socket_address addr) noexcept {
    ERR_GUARD(err) {
      if (fd_ != BAD_FD)
        return call_accept(err, fd_, addr);
      err = err::E_XXX;
    }
    return BAD_FD;
  }

  t_errn t_socket::shutdown(t_socket_howto howto) noexcept {
    if (fd_ != BAD_FD)
      return call_shutdown(fd_, howto);
    return BAD_ERRN;
  }

  t_void t_socket::shutdown(t_err err, t_socket_howto howto) noexcept {
    ERR_GUARD(err) {
      if (fd_ != BAD_FD)
        call_shutdown(err, fd_, howto);
      else
        err = err::E_XXX;
    }
  }

  t_errn t_socket::getpeername(r_socket_address addr) const noexcept {
    if (fd_ != BAD_FD)
      return call_getpeername(fd_, addr);
    return BAD_ERRN;
  }

  t_void t_socket::getpeername(t_err err,
                               r_socket_address addr) const noexcept {
    ERR_GUARD(err) {
      if (fd_ != BAD_FD)
        call_getpeername(err, fd_, addr);
      else
        err = err::E_XXX;
    }
  }

  t_errn t_socket::getsockname(r_socket_address addr) const noexcept {
    if (fd_ != BAD_FD)
      return call_getsockname(fd_, addr);
    return BAD_ERRN;
  }

  t_void t_socket::getsockname(t_err err,
                               r_socket_address addr) const noexcept {
    ERR_GUARD(err) {
      if (fd_ != BAD_FD)
        call_getsockname(err, fd_, addr);
      else
        err = err::E_XXX;
    }
  }

  t_errn t_socket::getsockopt(t_socket_level level,
                              r_socket_option option) const noexcept {
    if (fd_ != BAD_FD)
      return call_getsockopt(fd_, level, option);
    return BAD_ERRN;
  }

  t_void t_socket::getsockopt(t_err err, t_socket_level level,
                              r_socket_option option) const noexcept {
    ERR_GUARD(err) {
      if (fd_ != BAD_FD)
        call_getsockopt(err, fd_, level, option);
      else
        err = err::E_XXX;
    }
  }

  t_errn t_socket::setsockopt(t_socket_level level,
                              R_socket_option option) noexcept {
    if (fd_ != BAD_FD)
      return call_setsockopt(fd_, level, option);
    return BAD_ERRN;
  }

  t_void t_socket::setsockopt(t_err err, t_socket_level level,
                              R_socket_option option) noexcept {
    ERR_GUARD(err) {
      if (fd_ != BAD_FD)
        call_setsockopt(err, fd_, level, option);
      else
        err = err::E_XXX;
    }
  }

  t_verify<t_n> t_socket::send(R_byte_crange range, t_flags flags) noexcept {
    if (fd_ != BAD_FD)
      return call_send(fd_, range, flags);
    return {t_n{0}, BAD_ERRN};
  }

  t_n t_socket::send(t_err err, R_byte_crange range, t_flags flags) noexcept {
    ERR_GUARD(err) {
      if (fd_ != BAD_FD)
        return call_send(err, fd_, range, flags);
      err = err::E_XXX;
    }
    return t_n{0};
  }

  t_verify<t_n> t_socket::recv(r_byte_range range, t_flags flags) noexcept {
    if (fd_ != BAD_FD)
      return call_recv(fd_, range, flags);
    return {t_n{0}, BAD_ERRN};
  }

  t_n t_socket::recv(t_err err, r_byte_range range, t_flags flags) noexcept {
    ERR_GUARD(err) {
      if (fd_ != BAD_FD)
        return call_recv(err, fd_, range, flags);
      err = err::E_XXX;
    }
    return t_n{0};
  }

  t_verify<t_n> t_socket::sendto(R_byte_crange range, R_socket_address addr,
                                 t_flags flags) noexcept {
    if (fd_ != BAD_FD)
      return call_sendto(fd_, range, addr, flags);
    return {t_n{0}, BAD_ERRN};
  }

  t_n t_socket::sendto(t_err err, R_byte_crange range, R_socket_address addr,
                       t_flags flags) noexcept {
    ERR_GUARD(err) {
      if (fd_ != BAD_FD)
        return call_sendto(err, fd_, range, addr, flags);
      err = err::E_XXX;
    }
    return t_n{0};
  }

  t_verify<t_n> t_socket::recvfrom(r_byte_range range, r_socket_address addr,
                                   t_flags flags) noexcept {
    if (fd_ != BAD_FD)
      return call_recvfrom(fd_, range, addr, flags);
    return {t_n{0}, BAD_ERRN};
  }

  t_n t_socket::recvfrom(t_err err, r_byte_range range, r_socket_address addr,
                         t_flags flags) noexcept {
    ERR_GUARD(err) {
      if (fd_ != BAD_FD)
        return call_recvfrom(err, fd_, range, addr, flags);
      err = err::E_XXX;
    }
    return t_n{0};
  }

  t_verify<t_n> t_socket::sendmsg(R_socket_msghdr msg,
                                  t_flags flags) noexcept {
    if (fd_ != BAD_FD)
      return call_sendmsg(fd_, msg, flags);
    return {t_n{0}, BAD_ERRN};
  }

  t_n t_socket::sendmsg(t_err err, R_socket_msghdr msg,
                        t_flags flags) noexcept {
    ERR_GUARD(err) {
      if (fd_ != BAD_FD)
        return call_sendmsg(err, fd_, msg, flags);
      err = err::E_XXX;
    }
    return t_n{0};
  }

  t_verify<t_n> t_socket::recvmsg(r_socket_msghdr msg,
                                  t_flags flags) noexcept {
    if (fd_ != BAD_FD)
      return call_recvmsg(fd_, msg, flags);
    return {t_n{0}, BAD_ERRN};
  }

  t_n t_socket::recvmsg(t_err err, r_socket_msghdr msg,
                        t_flags flags) noexcept {
    ERR_GUARD(err) {
      if (fd_ != BAD_FD)
        return call_recvmsg(err, fd_, msg, flags);
      err = err::E_XXX;
    }
    return t_n{0};
  }

  t_verify<t_n> t_socket::sendmmsg(R_socket_msghdr_crange range,
                                   t_flags flags) noexcept {
    if (fd_ != BAD_FD)
      return call_sendmmsg(fd_, range, flags);
    return {t_n{0}, BAD_ERRN};
  }

  t_n t_socket::sendmmsg(t_err err, R_socket_msghdr_crange range,
                         t_flags flags) noexcept {
    ERR_GUARD(err) {
      if (fd_ != BAD_FD)
        return call_sendmmsg(err, fd_, range, flags);
      err = err::E_XXX;
    }
    return t_n{0};
  }

  t_verify<t_n> t_socket::recvmmsg(r_socket_msghdr_range range,
                                   t_flags flags) noexcept {
    if (fd_ != BAD_FD)
      return call_recvmmsg(fd_, range, flags);
    return {t_n{0}, BAD_ERRN};
  }

  t_verify<t_n> t_socket::recvmmsg(r_socket_msghdr_range range, t_flags flags,
                                   r_timespec timespec) noexcept {
    if (fd_ != BAD_FD)
      return call_recvmmsg(fd_, range, flags, timespec);
    return {t_n{0}, BAD_ERRN};
  }

  t_n t_socket::recvmmsg(t_err err, r_socket_msghdr_range range,
                         t_flags flags) noexcept {
    ERR_GUARD(err) {
      if (fd_ != BAD_FD)
        return call_recvmmsg(err, fd_, range, flags);
      err = err::E_XXX;
    }
    return t_n{0};
  }

  t_n t_socket::recvmmsg(t_err err, r_socket_msghdr_range range,
                         t_flags flags, r_timespec timespec) noexcept {
    ERR_GUARD(err) {
      if (fd_ != BAD_FD)
        return call_recvmmsg(err, fd_, range, flags, timespec);
      err = err::E_XXX;
    }
    return t_n{0};
  }

///////////////////////////////////////////////////////////////////////////////

  t_tipc_stream_client::t_tipc_stream_client() noexcept :
    socket_{t_socket_domain{0}, t_socket_type{0}, t_socket_protocol{0}} {
  }

  t_tipc_stream_client
    ::t_tipc_stream_client(x_tipc_stream_client client) noexcept
      : socket_{named::x_cast(client.socket_)} {
  }

  t_tipc_stream_client::operator t_validity() const noexcept {
    return socket_;
  }

  t_fd t_tipc_stream_client::get_fd() const noexcept {
    return socket_.get_fd();
  }

  t_errn t_tipc_stream_client::connect(R_tipc_address addr) noexcept {
    return socket_.connect(addr);
  }

  t_void t_tipc_stream_client::connect(t_err err,
                                       R_tipc_address addr) noexcept {
    ERR_GUARD(err) {
      socket_.connect(err, addr);
    }
  }

  t_errn t_tipc_stream_client::getpeername(r_tipc_address addr) const noexcept {
    return socket_.getpeername(addr);
  }

  t_void t_tipc_stream_client::getpeername(t_err err,
                                           r_tipc_address addr) const noexcept {
    ERR_GUARD(err) {
      socket_.getpeername(err, addr);
    }
  }

  t_errn t_tipc_stream_client::getsockname(r_tipc_address addr) const noexcept {
    return socket_.getsockname(addr);
  }

  t_void t_tipc_stream_client::getsockname(t_err err,
                                           r_tipc_address addr) const noexcept {
    ERR_GUARD(err) {
      socket_.getsockname(err, addr);
    }
  }

  t_verify<t_n> t_tipc_stream_client::send(R_byte_crange range,
                                           t_flags flags) noexcept {
    return socket_.send(range, flags);
  }

  t_n t_tipc_stream_client::send(t_err err, R_byte_crange range,
                                 t_flags flags) noexcept {
    ERR_GUARD(err) {
      return socket_.send(err, range, flags);
    }
    return t_n{0};
  }

  t_verify<t_n> t_tipc_stream_client::recv(r_byte_range range,
                                           t_flags flags) noexcept {
    return socket_.recv(range, flags);
  }

  t_n t_tipc_stream_client::recv(t_err err, r_byte_range range,
                                 t_flags flags) noexcept {
    ERR_GUARD(err) {
      return socket_.recv(err, range, flags);
    }
    return t_n{0};
  }

  t_verify<t_n> t_tipc_stream_client::sendmsg(R_socket_msghdr msg,
                                              t_flags flags) noexcept {
    return socket_.sendmsg(msg, flags);
  }

  t_n t_tipc_stream_client::sendmsg(t_err err, R_socket_msghdr msg,
                                    t_flags flags) noexcept {
    ERR_GUARD(err) {
      return socket_.sendmsg(err, msg, flags);
    }
    return t_n{0};
  }

  t_verify<t_n> t_tipc_stream_client::recvmsg(r_socket_msghdr msg,
                                              t_flags flags) noexcept {
    return socket_.recvmsg(msg, flags);
  }

  t_n t_tipc_stream_client::recvmsg(t_err err, r_socket_msghdr msg,
                                    t_flags flags) noexcept {
    ERR_GUARD(err) {
      return socket_.recvmsg(err, msg, flags);
    }
    return t_n{0};
  }

  t_errn t_tipc_stream_client
      ::getsockopt(t_socket_level level,
                   r_socket_option option) const noexcept {
    return socket_.getsockopt(level, option);
  }

  t_void t_tipc_stream_client
      ::getsockopt(t_err err, t_socket_level level,
                   r_socket_option option) const noexcept {
    ERR_GUARD(err) {
      socket_.getsockopt(err, level, option);
    }
  }

  t_errn t_tipc_stream_client::setsockopt(t_socket_level level,
                                          R_socket_option option) noexcept {
    return socket_.setsockopt(level, option);
  }

  t_void t_tipc_stream_client::setsockopt(t_err err, t_socket_level level,
                                          R_socket_option option) noexcept {
    ERR_GUARD(err) {
      socket_.setsockopt(err, level, option);
    }
  }

///////////////////////////////////////////////////////////////////////////////

}
}
}
