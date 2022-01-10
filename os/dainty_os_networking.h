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

#ifndef _DAINTY_OS_NETWORKING_H_
#define _DAINTY_OS_NETWORKING_H_

// description
// os: operating system functionality used by dainty
//
//  not a complete API but only the things used by dainty.

#include "dainty_base.h"
#include "dainty_oops.h"
#include "dainty_container_transfer.h"
#include "dainty_os_call.h"

namespace dainty
{
namespace os
{
namespace networking
{
  using os::BAD_FD;
  using container::transfer::t_transfer;
  using container::transfer::t_errn_transfer;

///////////////////////////////////////////////////////////////////////////////

  class t_connect_socket;
  using r_connect_socket = t_prefix<t_connect_socket>::r_;
  using x_connect_socket = t_prefix<t_connect_socket>::x_;
  using R_connect_socket = t_prefix<t_connect_socket>::R_;
  using p_connect_socket = t_prefix<t_connect_socket>::p_;
  using P_connect_socket = t_prefix<t_connect_socket>::P_;

  class t_connect_socket final {
  public:
    t_connect_socket(t_fd)             noexcept;
    t_connect_socket(x_connect_socket) noexcept;
   ~t_connect_socket();

    t_connect_socket()                           = delete;
    t_connect_socket(R_connect_socket)           = delete;
    r_connect_socket operator=(R_connect_socket) = delete;
    r_connect_socket operator=(x_connect_socket) = delete;

    operator t_validity() const noexcept;
    t_fd     get_fd    () const noexcept;

///////////////////////////////////////////////////////////////////////////////

    t_errn close()      noexcept;
    t_void close(t_err) noexcept;

///////////////////////////////////////////////////////////////////////////////

    t_errn getpeername(       r_socket_address) const noexcept;
    t_void getpeername(t_err, r_socket_address) const noexcept;

    t_errn getsockname(       r_socket_address) const noexcept;
    t_void getsockname(t_err, r_socket_address) const noexcept;

///////////////////////////////////////////////////////////////////////////////

    t_errn getsockopt(       t_socket_level, r_socket_option) const noexcept;
    t_void getsockopt(t_err, t_socket_level, r_socket_option) const noexcept;

    t_errn setsockopt(       t_socket_level, R_socket_option) noexcept;
    t_void setsockopt(t_err, t_socket_level, R_socket_option) noexcept;

///////////////////////////////////////////////////////////////////////////////

    t_verify<t_n> send(       R_byte_crange, t_flags) noexcept;
    t_n           send(t_err, R_byte_crange, t_flags) noexcept;

    t_verify<t_n> recv(       r_byte_range, t_flags) noexcept;
    t_n           recv(t_err, r_byte_range, t_flags) noexcept;

    t_verify<t_n> sendmsg(       R_socket_msghdr, t_flags) noexcept;
    t_n           sendmsg(t_err, R_socket_msghdr, t_flags) noexcept;

    t_verify<t_n> recvmsg(       r_socket_msghdr, t_flags) noexcept;
    t_n           recvmsg(t_err, r_socket_msghdr, t_flags) noexcept;

///////////////////////////////////////////////////////////////////////////////

    t_verify<t_n> sendmmsg(       R_socket_msghdr_crange, t_flags) noexcept;
    t_n           sendmmsg(t_err, R_socket_msghdr_crange, t_flags) noexcept;



    t_verify<t_n> recvmmsg(       r_socket_msghdr_range, t_flags) noexcept;
    t_n           recvmmsg(t_err, r_socket_msghdr_range, t_flags) noexcept;

    t_verify<t_n> recvmmsg(       r_socket_msghdr_range, t_flags,
                                  r_timespec) noexcept;
    t_n           recvmmsg(t_err, r_socket_msghdr_range, t_flags,
                                  r_timespec) noexcept;

    template<t_n_ N>
    t_verify<t_n> recvmmsg(       t_socket_msghdr (&hdr)[N], t_flags) noexcept;
    template<t_n_ N>
    t_n           recvmmsg(t_err, t_socket_msghdr (&hdr)[N], t_flags) noexcept;

    template<t_n_ N>
    t_verify<t_n> recvmmsg(       t_socket_msghdr (&hdr)[N], t_flags,
                                  r_timespec) noexcept;
    template<t_n_ N>
    t_n           recvmmsg(t_err, t_socket_msghdr (&hdr)[N], t_flags,
                                  r_timespec) noexcept;

  private:
    t_fd fd_;
  };

///////////////////////////////////////////////////////////////////////////////

  class t_socket;
  using r_socket = t_prefix<t_socket>::r_;
  using x_socket = t_prefix<t_socket>::x_;
  using R_socket = t_prefix<t_socket>::R_;
  using p_socket = t_prefix<t_socket>::p_;
  using P_socket = t_prefix<t_socket>::P_;

  class t_socket final {
  public:
    t_socket(       t_socket_domain, t_socket_type, t_socket_protocol) noexcept;
    t_socket(t_err, t_socket_domain, t_socket_type, t_socket_protocol) noexcept;
    t_socket()         noexcept;
    t_socket(x_socket) noexcept;
   ~t_socket();

    t_socket(R_socket)           = delete;
    r_socket operator=(R_socket) = delete;
    r_socket operator=(x_socket) = delete;

    operator t_validity() const noexcept;
    t_fd     get_fd    () const noexcept;

///////////////////////////////////////////////////////////////////////////////

    t_errn create(       t_socket_domain, t_socket_type,
                         t_socket_protocol) noexcept;
    t_void create(t_err, t_socket_domain, t_socket_type,
                         t_socket_protocol) noexcept;

    t_errn close()      noexcept;
    t_void close(t_err) noexcept;

///////////////////////////////////////////////////////////////////////////////

    t_errn bind(       R_socket_address) noexcept;
    t_void bind(t_err, R_socket_address) noexcept;

    t_errn connect(       R_socket_address) noexcept;
    t_void connect(t_err, R_socket_address) noexcept;

    t_errn listen(       t_socket_backlog) noexcept;
    t_void listen(t_err, t_socket_backlog) noexcept;

    t_errn_transfer<t_connect_socket> accept(     ) noexcept;
    t_transfer     <t_connect_socket> accept(t_err) noexcept;

    t_errn_transfer<t_connect_socket> accept(       r_socket_address) noexcept;
    t_transfer     <t_connect_socket> accept(t_err, r_socket_address) noexcept;

    t_errn shutdown(       t_socket_howto) noexcept;
    t_void shutdown(t_err, t_socket_howto) noexcept;

///////////////////////////////////////////////////////////////////////////////

    t_errn getpeername(       r_socket_address) const noexcept;
    t_void getpeername(t_err, r_socket_address) const noexcept;

    t_errn getsockname(       r_socket_address) const noexcept;
    t_void getsockname(t_err, r_socket_address) const noexcept;

///////////////////////////////////////////////////////////////////////////////

    t_errn getsockopt(       t_socket_level, r_socket_option) const noexcept;
    t_void getsockopt(t_err, t_socket_level, r_socket_option) const noexcept;

    t_errn setsockopt(       t_socket_level, R_socket_option) noexcept;
    t_void setsockopt(t_err, t_socket_level, R_socket_option) noexcept;

///////////////////////////////////////////////////////////////////////////////

    t_verify<t_n> send(       R_byte_crange, t_flags) noexcept;
    t_n           send(t_err, R_byte_crange, t_flags) noexcept;

    t_verify<t_n> recv(       r_byte_range, t_flags) noexcept;
    t_n           recv(t_err, r_byte_range, t_flags) noexcept;

    t_verify<t_n> sendto(       R_byte_crange, R_socket_address,
                                t_flags) noexcept;
    t_n           sendto(t_err, R_byte_crange, R_socket_address,
                                t_flags) noexcept;

    t_verify<t_n> recvfrom(       r_byte_range, r_socket_address,
                                  t_flags) noexcept;
    t_n           recvfrom(t_err, r_byte_range, r_socket_address,
                                  t_flags) noexcept;

    t_verify<t_n> sendmsg(       R_socket_msghdr, t_flags) noexcept;
    t_n           sendmsg(t_err, R_socket_msghdr, t_flags) noexcept;

    t_verify<t_n> recvmsg(       r_socket_msghdr, t_flags) noexcept;
    t_n           recvmsg(t_err, r_socket_msghdr, t_flags) noexcept;

///////////////////////////////////////////////////////////////////////////////

    t_verify<t_n> sendmmsg(       R_socket_msghdr_crange, t_flags) noexcept;
    t_n           sendmmsg(t_err, R_socket_msghdr_crange, t_flags) noexcept;



    t_verify<t_n> recvmmsg(       r_socket_msghdr_range, t_flags) noexcept;
    t_n           recvmmsg(t_err, r_socket_msghdr_range, t_flags) noexcept;

    t_verify<t_n> recvmmsg(       r_socket_msghdr_range, t_flags,
                                  r_timespec) noexcept;
    t_n           recvmmsg(t_err, r_socket_msghdr_range, t_flags,
                                  r_timespec) noexcept;

    template<t_n_ N>
    t_verify<t_n> recvmmsg(       t_socket_msghdr (&hdr)[N], t_flags) noexcept;
    template<t_n_ N>
    t_n           recvmmsg(t_err, t_socket_msghdr (&hdr)[N], t_flags) noexcept;

    template<t_n_ N>
    t_verify<t_n> recvmmsg(       t_socket_msghdr (&hdr)[N], t_flags,
                                  r_timespec) noexcept;
    template<t_n_ N>
    t_n           recvmmsg(t_err, t_socket_msghdr (&hdr)[N], t_flags,
                                  r_timespec) noexcept;

  private:
    t_fd fd_;
  };

///////////////////////////////////////////////////////////////////////////////

}
}
}

#endif
