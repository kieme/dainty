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

#include "dainty_named.h"
#include "dainty_oops.h"
#include "dainty_os_call.h"

namespace dainty
{
namespace os
{
namespace networking
{
  using named::t_n_;

///////////////////////////////////////////////////////////////////////////////

  class t_socket;
  using r_socket = t_prefix<t_socket>::r_;
  using x_socket = t_prefix<t_socket>::x_;
  using R_socket = t_prefix<t_socket>::R_;
  using p_socket = t_prefix<t_socket>::p_;
  using P_socket = t_prefix<t_socket>::P_;

  class t_socket final {
  public:
    t_socket() noexcept;
    t_socket(t_socket_domain, t_socket_type, t_socket_protocol) noexcept;
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

    t_verify<t_fd> accept(       p_socket_address) noexcept;
    t_fd           accept(t_err, p_socket_address) noexcept;

    t_errn shutdown(       t_socket_howto) noexcept;
    t_void shutdown(t_err, t_socket_howto) noexcept;

///////////////////////////////////////////////////////////////////////////////

    t_errn getpeername(       r_socket_address) const noexcept;
    t_void getpeername(t_err, r_socket_address) const noexcept;

    t_errn getsockname(       r_socket_address) const noexcept;
    t_void getsockname(t_err, r_socket_address) const noexcept;

///////////////////////////////////////////////////////////////////////////////

    t_errn getsockopt(       t_socket_level, t_socket_option_name,
                             r_socket_option_value) const noexcept;
    t_void getsockopt(t_err, t_socket_level, t_socket_option_name,
                             r_socket_option_value) const noexcept;

    t_errn setsockopt(       t_socket_level, t_socket_option_name,
                             R_socket_option_value) noexcept;
    t_void setsockopt(t_err, t_socket_level, t_socket_option_name,
                             R_socket_option_value) noexcept;

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

    t_verify<t_n> sendmmsg(       R_socket_msghdr_crange, t_flags) noexcept;
    t_n           sendmmsg(t_err, R_socket_msghdr_crange, t_flags) noexcept;

///////////////////////////////////////////////////////////////////////////////

    t_verify<t_n> recvmmsg(       r_socket_msghdr_range, t_flags) noexcept;
    t_verify<t_n> recvmmsg(       r_socket_msghdr_range, t_flags,
                                  r_timespec) noexcept;

    t_n           recvmmsg(t_err, r_socket_msghdr_range, t_flags) noexcept;
    t_n           recvmmsg(t_err, r_socket_msghdr_range, t_flags,
                                  r_timespec) noexcept;

    template<t_n_ N>
    t_verify<t_n> recvmmsg(       t_socket_msghdr (&hdr)[N], t_flags) noexcept;
    template<t_n_ N>
    t_verify<t_n> recvmmsg(       t_socket_msghdr (&hdr)[N], t_flags,
                                  r_timespec) noexcept;

    template<t_n_ N>
    t_n           recvmmsg(t_err, t_socket_msghdr (&hdr)[N], t_flags) noexcept;
    template<t_n_ N>
    t_n           recvmmsg(t_err, t_socket_msghdr (&hdr)[N], t_flags,
                                  r_timespec) noexcept;

  private:
    t_fd fd_;
  };

///////////////////////////////////////////////////////////////////////////////

  class t_tipc_stream_client;
  using r_tipc_stream_client = t_prefix<t_tipc_stream_client>::r_;
  using x_tipc_stream_client = t_prefix<t_tipc_stream_client>::x_;
  using R_tipc_stream_client = t_prefix<t_tipc_stream_client>::R_;
  using p_tipc_stream_client = t_prefix<t_tipc_stream_client>::p_;
  using P_tipc_stream_client = t_prefix<t_tipc_stream_client>::P_;

  class t_tipc_stream_client {
  public:
    t_tipc_stream_client() noexcept;
    t_tipc_stream_client(x_tipc_stream_client) noexcept;
   ~t_tipc_stream_client();

    t_tipc_stream_client(R_tipc_stream_client)           = delete;
    r_tipc_stream_client operator=(R_tipc_stream_client) = delete;
    r_tipc_stream_client operator=(x_tipc_stream_client) = delete;

    operator t_validity() const noexcept;
    t_fd     get_fd    () const noexcept;

    // connect
    // send
    // recv
    // setsockopt
    // getsockopt

  private:
    t_socket socket_;
  };

  class t_tipc_stream_server;
  using r_tipc_stream_server = t_prefix<t_tipc_stream_server>::r_;
  using x_tipc_stream_server = t_prefix<t_tipc_stream_server>::x_;
  using R_tipc_stream_server = t_prefix<t_tipc_stream_server>::R_;
  using p_tipc_stream_server = t_prefix<t_tipc_stream_server>::p_;
  using P_tipc_stream_server = t_prefix<t_tipc_stream_server>::P_;

  class t_tipc_stream_server {
  public:
    t_tipc_stream_server(int) noexcept; // family, protocol, flags
    t_tipc_stream_server(x_tipc_stream_server) noexcept;
   ~t_tipc_stream_server();

    t_tipc_stream_server(R_tipc_stream_server)           = delete;
    r_tipc_stream_server operator=(R_tipc_stream_server) = delete;
    r_tipc_stream_server operator=(x_tipc_stream_server) = delete;

    operator t_validity() const noexcept;
    t_fd     get_fd    () const noexcept;

    // listen
    // accept
    // send
    // recv
    // sendmsg
    // recvmsg
    // setsockopt
    // getsockopt

  private:
    t_socket socket_;
  };

///////////////////////////////////////////////////////////////////////////////

  class t_tipc_dgram_client;
  using r_tipc_dgram_client = t_prefix<t_tipc_dgram_client>::r_;
  using x_tipc_dgram_client = t_prefix<t_tipc_dgram_client>::x_;
  using R_tipc_dgram_client = t_prefix<t_tipc_dgram_client>::R_;
  using p_tipc_dgram_client = t_prefix<t_tipc_dgram_client>::p_;
  using P_tipc_dgram_client = t_prefix<t_tipc_dgram_client>::P_;

  class t_tipc_dgram_client {
  public:
    t_tipc_dgram_client() noexcept;
    t_tipc_dgram_client(x_tipc_dgram_client) noexcept;

    t_tipc_dgram_client(R_tipc_dgram_client)           = delete;
    r_tipc_dgram_client operator=(R_tipc_dgram_client) = delete;
    r_tipc_dgram_client operator=(x_tipc_dgram_client) = delete;

    operator t_validity() const noexcept;
    t_fd     get_fd    () const noexcept;

    // send
    // recv
    // setsockopt
    // getsockopt

  private:
    t_socket socket_;
  };

  class t_tipc_dgram_server;
  using r_tipc_dgram_server = t_prefix<t_tipc_dgram_server>::r_;
  using x_tipc_dgram_server = t_prefix<t_tipc_dgram_server>::x_;
  using R_tipc_dgram_server = t_prefix<t_tipc_dgram_server>::R_;
  using p_tipc_dgram_server = t_prefix<t_tipc_dgram_server>::p_;
  using P_tipc_dgram_server = t_prefix<t_tipc_dgram_server>::P_;

  class t_tipc_dgram_server {
  public:
    t_tipc_dgram_server(int) noexcept; // family, protocol, flags
    t_tipc_dgram_server(x_tipc_dgram_server) noexcept;
   ~t_tipc_dgram_server();

    t_tipc_dgram_server(R_tipc_dgram_server)           = delete;
    r_tipc_dgram_server operator=(R_tipc_dgram_server) = delete;
    r_tipc_dgram_server operator=(x_tipc_dgram_server) = delete;

    operator t_validity() const noexcept;
    t_fd     get_fd    () const noexcept;

    // send
    // recv
    // sendmsg
    // recvmsg
    // setsockopt
    // getsockopt

  private:
    t_socket socket_;
  };

///////////////////////////////////////////////////////////////////////////////

  class t_tipc_rdm_client;
  using r_tipc_rdm_client = t_prefix<t_tipc_rdm_client>::r_;
  using x_tipc_rdm_client = t_prefix<t_tipc_rdm_client>::x_;
  using R_tipc_rdm_client = t_prefix<t_tipc_rdm_client>::R_;
  using p_tipc_rdm_client = t_prefix<t_tipc_rdm_client>::p_;
  using P_tipc_rdm_client = t_prefix<t_tipc_rdm_client>::P_;

  class t_tipc_rdm_client {
  public:
    t_tipc_rdm_client() noexcept;
    t_tipc_rdm_client(x_tipc_rdm_client) noexcept;
   ~t_tipc_rdm_client();

    t_tipc_rdm_client(R_tipc_rdm_client)           = delete;
    r_tipc_rdm_client operator=(R_tipc_rdm_client) = delete;
    r_tipc_rdm_client operator=(x_tipc_rdm_client) = delete;

    operator t_validity() const noexcept;
    t_fd     get_fd    () const noexcept;

    // send
    // recv
    // setsockopt
    // getsockopt

  private:
    t_socket socket_;
  };

  class t_tipc_rdm_server;
  using r_tipc_rdm_server = t_prefix<t_tipc_rdm_server>::r_;
  using x_tipc_rdm_server = t_prefix<t_tipc_rdm_server>::x_;
  using R_tipc_rdm_server = t_prefix<t_tipc_rdm_server>::R_;
  using p_tipc_rdm_server = t_prefix<t_tipc_rdm_server>::p_;
  using P_tipc_rdm_server = t_prefix<t_tipc_rdm_server>::P_;

  class t_tipc_rdm_server {
  public:
    t_tipc_rdm_server(int) noexcept; // family, protocol, flags
    t_tipc_rdm_server(x_tipc_rdm_server) noexcept;
   ~t_tipc_rdm_server();

    t_tipc_rdm_server(R_tipc_rdm_server)           = delete;
    r_tipc_rdm_server operator=(R_tipc_rdm_server) = delete;
    r_tipc_rdm_server operator=(x_tipc_rdm_server) = delete;

    operator t_validity() const noexcept;
    t_fd     get_fd    () const noexcept;

    // send
    // recv
    // sendmsg
    // recvmsg
    // setsockopt
    // getsockopt

  private:
    t_socket socket_;
  };

///////////////////////////////////////////////////////////////////////////////

  class t_tipc_seqpacket_client;
  using r_tipc_seqpacket_client = t_prefix<t_tipc_seqpacket_client>::r_;
  using x_tipc_seqpacket_client = t_prefix<t_tipc_seqpacket_client>::x_;
  using R_tipc_seqpacket_client = t_prefix<t_tipc_seqpacket_client>::R_;
  using p_tipc_seqpacket_client = t_prefix<t_tipc_seqpacket_client>::p_;
  using P_tipc_seqpacket_client = t_prefix<t_tipc_seqpacket_client>::P_;

  class t_tipc_seqpacket_client {
  public:
    t_tipc_seqpacket_client() noexcept;
    t_tipc_seqpacket_client(x_tipc_seqpacket_client) noexcept;
   ~t_tipc_seqpacket_client();

    t_tipc_seqpacket_client(R_tipc_seqpacket_client)           = delete;
    r_tipc_seqpacket_client operator=(R_tipc_seqpacket_client) = delete;
    r_tipc_seqpacket_client operator=(x_tipc_seqpacket_client) = delete;

    operator t_validity() const noexcept;
    t_fd     get_fd    () const noexcept;

    // connect
    // send
    // recv
    // setsockopt
    // getsockopt

  private:
    t_socket socket_;
  };

  class t_tipc_seqpacket_server;
  using r_tipc_seqpacket_server = t_prefix<t_tipc_seqpacket_server>::r_;
  using x_tipc_seqpacket_server = t_prefix<t_tipc_seqpacket_server>::x_;
  using R_tipc_seqpacket_server = t_prefix<t_tipc_seqpacket_server>::R_;
  using p_tipc_seqpacket_server = t_prefix<t_tipc_seqpacket_server>::p_;
  using P_tipc_seqpacket_server = t_prefix<t_tipc_seqpacket_server>::P_;

  class t_tipc_seqpacket_server {
  public:
    t_tipc_seqpacket_server(int) noexcept; // family, protocol, flags
    t_tipc_seqpacket_server(x_tipc_seqpacket_server) noexcept;
   ~t_tipc_seqpacket_server();

    t_tipc_seqpacket_server(R_tipc_seqpacket_server)           = delete;
    r_tipc_seqpacket_server operator=(R_tipc_seqpacket_server) = delete;
    r_tipc_seqpacket_server operator=(x_tipc_seqpacket_server) = delete;

    operator t_validity() const noexcept;
    t_fd     get_fd    () const noexcept;

    // accept
    // send
    // recv
    // sendmsg
    // recvmsg
    // setsockopt
    // getsockopt

  private:
    t_socket socket_;
  };

///////////////////////////////////////////////////////////////////////////////

  class t_tcp_client;
  using r_tcp_client = t_prefix<t_tcp_client>::r_;
  using x_tcp_client = t_prefix<t_tcp_client>::x_;
  using R_tcp_client = t_prefix<t_tcp_client>::R_;
  using p_tcp_client = t_prefix<t_tcp_client>::p_;
  using P_tcp_client = t_prefix<t_tcp_client>::P_;

  class t_tcp_client {
  public:
    t_tcp_client() noexcept;
    t_tcp_client(x_tcp_client) noexcept;
   ~t_tcp_client();

    t_tcp_client(R_tcp_client)           = delete;
    r_tcp_client operator=(R_tcp_client) = delete;
    r_tcp_client operator=(x_tcp_client) = delete;

    operator t_validity() const noexcept;
    t_fd     get_fd    () const noexcept;

    // connect
    // send
    // recv
    // setsockopt
    // getsockopt

  private:
    t_socket socket_;
  };

  class t_tcp_server;
  using r_tcp_server = t_prefix<t_tcp_server>::r_;
  using x_tcp_server = t_prefix<t_tcp_server>::x_;
  using R_tcp_server = t_prefix<t_tcp_server>::R_;
  using p_tcp_server = t_prefix<t_tcp_server>::p_;
  using P_tcp_server = t_prefix<t_tcp_server>::P_;

  class t_tcp_server {
  public:
    t_tcp_server(int) noexcept; // family, protocol, flags
    t_tcp_server(x_tcp_server) noexcept;
   ~t_tcp_server();

    t_tcp_server(R_tcp_server)           = delete;
    r_tcp_server operator=(R_tcp_server) = delete;
    r_tcp_server operator=(x_tcp_server) = delete;

    operator t_validity() const noexcept;
    t_fd     get_fd    () const noexcept;

    // accept
    // send
    // recv
    // sendmsg
    // recvmsg
    // setsockopt
    // getsockopt

  private:
    t_socket socket_;
  };

///////////////////////////////////////////////////////////////////////////////

  class t_udp_client;
  using r_udp_client = t_prefix<t_udp_client>::r_;
  using x_udp_client = t_prefix<t_udp_client>::x_;
  using R_udp_client = t_prefix<t_udp_client>::R_;
  using p_udp_client = t_prefix<t_udp_client>::p_;
  using P_udp_client = t_prefix<t_udp_client>::P_;

  class t_udp_client {
  public:
    t_udp_client() noexcept;
    t_udp_client(x_udp_client) noexcept;
   ~t_udp_client();

    t_udp_client(R_udp_client)           = delete;
    r_udp_client operator=(R_udp_client) = delete;
    r_udp_client operator=(x_udp_client) = delete;

    operator t_validity() const noexcept;
    t_fd     get_fd    () const noexcept;

    // send
    // recv
    // setsockopt
    // getsockopt

  private:
    t_socket socket_;
  };

  class t_udp_server;
  using r_udp_server = t_prefix<t_udp_server>::r_;
  using x_udp_server = t_prefix<t_udp_server>::x_;
  using R_udp_server = t_prefix<t_udp_server>::R_;
  using p_udp_server = t_prefix<t_udp_server>::p_;
  using P_udp_server = t_prefix<t_udp_server>::P_;

  class t_udp_server {
  public:
    t_udp_server(int) noexcept; // family, protocol, flags
    t_udp_server(x_udp_server) noexcept;
   ~t_udp_server();

    t_udp_server(R_udp_server)           = delete;
    r_udp_server operator=(R_udp_server) = delete;
    r_udp_server operator=(x_udp_server) = delete;

    operator t_validity() const noexcept;
    t_fd     get_fd    () const noexcept;

    // send
    // recv
    // sendmsg
    // recvmsg
    // setsockopt
    // getsockopt

  private:
    t_socket socket_;
  };

///////////////////////////////////////////////////////////////////////////////

}
}
}

#endif
