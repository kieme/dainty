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
#include "dainty_os_networking.h"

namespace dainty
{
namespace os
{
namespace networking
{
  using named::utility::reset;

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

  t_socket::t_socket(x_socket socket) noexcept : fd_{reset(socket.fd_)} {
  }

  t_socket::~t_socket() {
    call_close(fd_);
  }

  t_socket::operator t_validity() const noexcept {
    return fd_ == BAD_FD ? INVALID : VALID;
  }

  t_fd t_socket::get_fd() const noexcept {
    return fd_;
  }

  t_errn t_socket::create(t_socket_domain, t_socket_type,
                         t_socket_protocol) noexcept {
  }

  t_void t_socket::create(t_err, t_socket_domain, t_socket_type,
                         t_socket_protocol) noexcept {
  }

  t_errn t_socket::close() noexcept {
  }

  t_void t_socket::close(t_err) noexcept {
  }

  t_errn t_socket::bind(R_socket_address) noexcept {
  }

  t_void t_socket::bind(t_err, R_socket_address) noexcept {
  }

  t_errn t_socket::connect(R_socket_address) noexcept {
  }

  t_void t_socket::connect(t_err, R_socket_address) noexcept {
  }

  t_errn t_socket::listen(t_socket_backlog) noexcept {
  }

  t_void t_socket::listen(t_err, t_socket_backlog) noexcept {
  }

  t_verify<t_fd> t_socket::accept() noexcept {
  }

  t_fd t_socket::accept(t_err) noexcept {
  }

  t_verify<t_fd> t_socket::accept(r_socket_address) noexcept {
  }

  t_fd t_socket::accept(t_err, r_socket_address) noexcept {
  }

  t_errn t_socket::shutdown(t_socket_howto) noexcept {
  }

  t_void t_socket::shutdown(t_err, t_socket_howto) noexcept {
  }

  t_errn t_socket::getpeername(r_socket_address) const noexcept {
  }

  t_void t_socket::getpeername(t_err, r_socket_address) const noexcept {
  }

  t_errn t_socket::getsockname(r_socket_address) const noexcept {
  }

  t_void t_socket::getsockname(t_err, r_socket_address) const noexcept {
  }

  t_errn t_socket::getsockopt(t_socket_level level,
                              r_socket_option option) const noexcept {
  }

  t_void t_socket::getsockopt(t_err err, t_socket_level level,
                              r_socket_option option) const noexcept {
  }

  t_errn t_socket::setsockopt(t_socket_level level,
                              R_socket_option option) noexcept {
  }

  t_void t_socket::setsockopt(t_err err, t_socket_level level,
                              R_socket_option option) noexcept {
  }

  t_verify<t_n> t_socket::send(R_byte_crange, t_flags) noexcept {
  }

  t_n t_socket::send(t_err, R_byte_crange, t_flags) noexcept {
  }

  t_verify<t_n> t_socket::recv(r_byte_range, t_flags) noexcept {
  }

  t_n t_socket::recv(t_err, r_byte_range, t_flags) noexcept {
  }

  t_verify<t_n> t_socket::sendto(R_byte_crange, R_socket_address,
                                t_flags) noexcept {
  }

  t_n t_socket::sendto(t_err, R_byte_crange, R_socket_address,
                                t_flags) noexcept {
  }

  t_verify<t_n> t_socket::recvfrom(r_byte_range, r_socket_address,
                                  t_flags) noexcept {
  }

  t_n t_socket::recvfrom(t_err, r_byte_range, r_socket_address,
                                  t_flags) noexcept {
  }

  t_verify<t_n> t_socket::sendmsg(R_socket_msghdr, t_flags) noexcept {
  }

  t_n t_socket::sendmsg(t_err, R_socket_msghdr, t_flags) noexcept {
  }

  t_verify<t_n> t_socket::recvmsg(r_socket_msghdr, t_flags) noexcept {
  }

  t_n t_socket::recvmsg(t_err, r_socket_msghdr, t_flags) noexcept {
  }

  t_verify<t_n> t_socket::sendmmsg(R_socket_msghdr_crange, t_flags) noexcept {
  }

  t_n t_socket::sendmmsg(t_err, R_socket_msghdr_crange, t_flags) noexcept {
  }

  t_verify<t_n> t_socket::recvmmsg(r_socket_msghdr_range, t_flags) noexcept {
  }

  t_verify<t_n> t_socket::recvmmsg(r_socket_msghdr_range, t_flags,
                                  r_timespec) noexcept {
  }

  t_n t_socket::recvmmsg(t_err, r_socket_msghdr_range, t_flags) noexcept {
  }
  t_n t_socket::recvmmsg(t_err, r_socket_msghdr_range, t_flags,
                          r_timespec) noexcept {
  }

///////////////////////////////////////////////////////////////////////////////
}
}
}
