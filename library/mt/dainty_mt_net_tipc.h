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

#ifndef _DAINTY_MT_NET_TIPC_H_
#define _DAINTY_MT_NET_TIPC_H_

#include "dainty_named.h"
#include "dainty_container_list.h"
#include "dainty_container_maybe.h"
#include "dainty_os_networking.h"
#include "dainty_mt_err.h"

// stats for all. just counters. how many calls. how many bytes sent receive.
// connection ooriented - have virtual methods - accept authenticate
//                                               ask accepted
//                                               inform when added
//                                               inform when remove
// t_logic

namespace dainty
{
namespace mt
{
namespace net_tipc
{
  using named::t_validity;
  using named::VALID;
  using named::INVALID;
  using named::t_prefix;
  using named::t_explicit;
  using named::t_void;
  using named::t_bool;
  using named::t_n;
  using named::t_fd;
  using named::t_errn;

  using container::list::t_list;
  using container::maybe::t_maybe;

  using os::t_verify;
  using os::t_tipc_address;
  using os::r_tipc_address;
  using os::R_tipc_address;
  using os::t_socket_level;
  using os::t_socket_option;
  using os::r_socket_option;
  using os::R_socket_option;
  using os::R_byte_crange;
  using os::r_byte_range;
  using os::R_socket_msghdr;
  using os::r_socket_msghdr;
  using os::t_flags;
  using os::networking::t_socket;

  using err::t_err;

///////////////////////////////////////////////////////////////////////////////

  enum  t_connect_id_tag_ {};
  using t_connect_id_ = named::t_int;
  using t_connect_id  = t_explicit<t_connect_id_, t_connect_id_tag_>;

  using t_connect_ids = t_maybe<t_list<t_connect_id>>;
  using r_connect_ids = t_prefix<t_connect_ids>::r_;

  struct t_connect_stats {
  };

  struct t_connect_info {
    t_fd            fd;
    t_connect_stats stats;
    // peername
  };
  using R_connect_info = t_prefix<t_connect_info>::R_;

///////////////////////////////////////////////////////////////////////////////

  class t_tipc_stream_client;
  using r_tipc_stream_client = t_prefix<t_tipc_stream_client>::r_;
  using x_tipc_stream_client = t_prefix<t_tipc_stream_client>::x_;
  using R_tipc_stream_client = t_prefix<t_tipc_stream_client>::R_;
  using p_tipc_stream_client = t_prefix<t_tipc_stream_client>::p_;
  using P_tipc_stream_client = t_prefix<t_tipc_stream_client>::P_;

  class t_tipc_stream_client {
  public:
    t_tipc_stream_client(       R_tipc_address) noexcept;
    t_tipc_stream_client(t_err, R_tipc_address) noexcept;

    t_tipc_stream_client(x_tipc_stream_client)  noexcept;

    t_tipc_stream_client(R_tipc_stream_client)           = delete;
    r_tipc_stream_client operator=(R_tipc_stream_client) = delete;
    r_tipc_stream_client operator=(x_tipc_stream_client) = delete;

    operator t_validity() const noexcept;
    t_fd     get_fd    () const noexcept;

    t_errn getpeername(       r_tipc_address) const noexcept;
    t_void getpeername(t_err, r_tipc_address) const noexcept;

    t_errn getsockname(       r_tipc_address) const noexcept;
    t_void getsockname(t_err, r_tipc_address) const noexcept;

    t_verify<t_n> send(       R_byte_crange, t_flags) noexcept;
    t_n           send(t_err, R_byte_crange, t_flags) noexcept;

    t_verify<t_n> recv(       r_byte_range, t_flags) noexcept;
    t_n           recv(t_err, r_byte_range, t_flags) noexcept;

    t_verify<t_n> sendmsg(       R_socket_msghdr, t_flags) noexcept;
    t_n           sendmsg(t_err, R_socket_msghdr, t_flags) noexcept;

    t_verify<t_n> recvmsg(       r_socket_msghdr, t_flags) noexcept;
    t_n           recvmsg(t_err, r_socket_msghdr, t_flags) noexcept;

    t_errn getsockopt(       t_socket_level, r_socket_option) const noexcept;
    t_void getsockopt(t_err, t_socket_level, r_socket_option) const noexcept;

    t_errn setsockopt(       t_socket_level, R_socket_option) noexcept;
    t_void setsockopt(t_err, t_socket_level, R_socket_option) noexcept;

  private:
    t_socket socket_;
  };

///////////////////////////////////////////////////////////////////////////////

  class t_tipc_stream_server;
  using r_tipc_stream_server = t_prefix<t_tipc_stream_server>::r_;
  using x_tipc_stream_server = t_prefix<t_tipc_stream_server>::x_;
  using R_tipc_stream_server = t_prefix<t_tipc_stream_server>::R_;
  using p_tipc_stream_server = t_prefix<t_tipc_stream_server>::p_;
  using P_tipc_stream_server = t_prefix<t_tipc_stream_server>::P_;

  class t_tipc_stream_server {
  public:
    t_tipc_stream_server(       R_tipc_address server) noexcept;
    t_tipc_stream_server(t_err, R_tipc_address server) noexcept;

    t_tipc_stream_server(x_tipc_stream_server)         noexcept;
   ~t_tipc_stream_server();

    t_tipc_stream_server(R_tipc_stream_server)           = delete;
    r_tipc_stream_server operator=(R_tipc_stream_server) = delete;
    r_tipc_stream_server operator=(x_tipc_stream_server) = delete;

    operator t_validity() const noexcept;
    t_fd     get_fd    () const noexcept;

///////////////////////////////////////////////////////////////////////////////

    t_connect_id   accept_connection()            noexcept;
    t_bool         close_connection(t_connect_id) noexcept;

    R_connect_info get_connection(t_connect_id)      const noexcept;
    t_void         get_connection_ids(r_connect_ids) const noexcept;

///////////////////////////////////////////////////////////////////////////////

    // shutdown/close
    t_errn getpeername(       t_connect_id, r_tipc_address) const noexcept;
    t_void getpeername(t_err, t_connect_id, r_tipc_address) const noexcept;

    t_errn getsockname(       t_connect_id, r_tipc_address) const noexcept;
    t_void getsockname(t_err, t_connect_id, r_tipc_address) const noexcept;

    t_verify<t_n> send(       t_connect_id, R_byte_crange, t_flags) noexcept;
    t_n           send(t_err, t_connect_id, R_byte_crange, t_flags) noexcept;

    t_verify<t_n> recv(       t_connect_id, r_byte_range, t_flags) noexcept;
    t_n           recv(t_err, t_connect_id, r_byte_range, t_flags) noexcept;

    t_verify<t_n> sendmsg(       t_connect_id, R_socket_msghdr,
                                 t_flags) noexcept;
    t_n           sendmsg(t_err, t_connect_id, R_socket_msghdr,
                                 t_flags) noexcept;

    t_verify<t_n> recvmsg(       t_connect_id, r_socket_msghdr,
                                 t_flags) noexcept;
    t_n           recvmsg(t_err, t_connect_id, r_socket_msghdr,
                                 t_flags) noexcept;

    t_errn getsockopt(       t_connect_id, t_socket_level,
                             r_socket_option) const noexcept;
    t_void getsockopt(t_err, t_connect_id, t_socket_level,
                             r_socket_option) const noexcept;

    t_errn setsockopt(       t_connect_id, t_socket_level,
                             R_socket_option) noexcept;
    t_void setsockopt(t_err, t_connect_id, t_socket_level,
                             R_socket_option) noexcept;

///////////////////////////////////////////////////////////////////////////////

    t_errn getpeername(       r_tipc_address) const noexcept;
    t_void getpeername(t_err, r_tipc_address) const noexcept;

    t_errn getsockname(       r_tipc_address) const noexcept;
    t_void getsockname(t_err, r_tipc_address) const noexcept;

    t_verify<t_n> send(       R_byte_crange, t_flags) noexcept;
    t_n           send(t_err, R_byte_crange, t_flags) noexcept;

    t_verify<t_n> recv(       r_byte_range, t_flags) noexcept;
    t_n           recv(t_err, r_byte_range, t_flags) noexcept;

    t_verify<t_n> sendmsg(       R_socket_msghdr, t_flags) noexcept;
    t_n           sendmsg(t_err, R_socket_msghdr, t_flags) noexcept;

    t_verify<t_n> recvmsg(       r_socket_msghdr, t_flags) noexcept;
    t_n           recvmsg(t_err, r_socket_msghdr, t_flags) noexcept;

    t_errn getsockopt(       t_socket_level, r_socket_option) const noexcept;
    t_void getsockopt(t_err, t_socket_level, r_socket_option) const noexcept;

    t_errn setsockopt(       t_socket_level, R_socket_option) noexcept;
    t_void setsockopt(t_err, t_socket_level, R_socket_option) noexcept;

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
    t_tipc_dgram_client()                    noexcept;
    t_tipc_dgram_client(x_tipc_dgram_client) noexcept;

    t_tipc_dgram_client(R_tipc_dgram_client)           = delete;
    r_tipc_dgram_client operator=(R_tipc_dgram_client) = delete;
    r_tipc_dgram_client operator=(x_tipc_dgram_client) = delete;

    operator t_validity() const noexcept;
    t_fd     get_fd    () const noexcept;
    // XXX add stats

    t_errn getsockopt(       t_socket_level, r_socket_option) const noexcept;
    t_void getsockopt(t_err, t_socket_level, r_socket_option) const noexcept;

    t_errn setsockopt(       t_socket_level, R_socket_option) noexcept;
    t_void setsockopt(t_err, t_socket_level, R_socket_option) noexcept;

    t_verify<t_n> sendto(       R_byte_crange, R_tipc_address,
                                t_flags) noexcept;
    t_n           sendto(t_err, R_byte_crange, R_tipc_address,
                                t_flags) noexcept;

    t_verify<t_n> recvfrom(       r_byte_range, r_tipc_address,
                                  t_flags) noexcept;
    t_n           recvfrom(t_err, r_byte_range, r_tipc_address,
                                  t_flags) noexcept;

    t_verify<t_n> sendmsg(       R_socket_msghdr, t_flags) noexcept;
    t_n           sendmsg(t_err, R_socket_msghdr, t_flags) noexcept;

    t_verify<t_n> recvmsg(       r_socket_msghdr, t_flags) noexcept;
    t_n           recvmsg(t_err, r_socket_msghdr, t_flags) noexcept;

  private:
    t_socket socket_;
  };

///////////////////////////////////////////////////////////////////////////////

  class t_tipc_dgram_server;
  using r_tipc_dgram_server = t_prefix<t_tipc_dgram_server>::r_;
  using x_tipc_dgram_server = t_prefix<t_tipc_dgram_server>::x_;
  using R_tipc_dgram_server = t_prefix<t_tipc_dgram_server>::R_;
  using p_tipc_dgram_server = t_prefix<t_tipc_dgram_server>::p_;
  using P_tipc_dgram_server = t_prefix<t_tipc_dgram_server>::P_;

  class t_tipc_dgram_server {
  public:
    t_tipc_dgram_server(       R_tipc_address server) noexcept;
    t_tipc_dgram_server(t_err, R_tipc_address server) noexcept;

    t_tipc_dgram_server(x_tipc_dgram_server)          noexcept;
   ~t_tipc_dgram_server();

    t_tipc_dgram_server(R_tipc_dgram_server)           = delete;
    r_tipc_dgram_server operator=(R_tipc_dgram_server) = delete;
    r_tipc_dgram_server operator=(x_tipc_dgram_server) = delete;

    operator t_validity() const noexcept;
    t_fd     get_fd    () const noexcept;

    // XXX add stats

    t_errn getsockopt(       t_socket_level, r_socket_option) const noexcept;
    t_void getsockopt(t_err, t_socket_level, r_socket_option) const noexcept;

    t_errn setsockopt(       t_socket_level, R_socket_option) noexcept;
    t_void setsockopt(t_err, t_socket_level, R_socket_option) noexcept;

    t_verify<t_n> sendto(       R_byte_crange, R_tipc_address,
                                t_flags) noexcept;
    t_n           sendto(t_err, R_byte_crange, R_tipc_address,
                                t_flags) noexcept;

    t_verify<t_n> recvfrom(       r_byte_range, r_tipc_address,
                                  t_flags) noexcept;
    t_n           recvfrom(t_err, r_byte_range, r_tipc_address,
                                  t_flags) noexcept;

    t_verify<t_n> sendmsg(       R_socket_msghdr, t_flags) noexcept;
    t_n           sendmsg(t_err, R_socket_msghdr, t_flags) noexcept;

    t_verify<t_n> recvmsg(       r_socket_msghdr, t_flags) noexcept;
    t_n           recvmsg(t_err, r_socket_msghdr, t_flags) noexcept;

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
    t_tipc_rdm_client()                  noexcept;
    t_tipc_rdm_client(x_tipc_rdm_client) noexcept;
   ~t_tipc_rdm_client();

    t_tipc_rdm_client(R_tipc_rdm_client)           = delete;
    r_tipc_rdm_client operator=(R_tipc_rdm_client) = delete;
    r_tipc_rdm_client operator=(x_tipc_rdm_client) = delete;

    operator t_validity() const noexcept;
    t_fd     get_fd    () const noexcept;

    // XXX add stats

    t_errn getsockopt(       t_socket_level, r_socket_option) const noexcept;
    t_void getsockopt(t_err, t_socket_level, r_socket_option) const noexcept;

    t_errn setsockopt(       t_socket_level, R_socket_option) noexcept;
    t_void setsockopt(t_err, t_socket_level, R_socket_option) noexcept;

    t_verify<t_n> sendto(       R_byte_crange, R_tipc_address,
                                t_flags) noexcept;
    t_n           sendto(t_err, R_byte_crange, R_tipc_address,
                                t_flags) noexcept;

    t_verify<t_n> recvfrom(       r_byte_range, r_tipc_address,
                                  t_flags) noexcept;
    t_n           recvfrom(t_err, r_byte_range, r_tipc_address,
                                  t_flags) noexcept;

    t_verify<t_n> sendmsg(       R_socket_msghdr, t_flags) noexcept;
    t_n           sendmsg(t_err, R_socket_msghdr, t_flags) noexcept;

    t_verify<t_n> recvmsg(       r_socket_msghdr, t_flags) noexcept;
    t_n           recvmsg(t_err, r_socket_msghdr, t_flags) noexcept;

  private:
    t_socket socket_;
  };

///////////////////////////////////////////////////////////////////////////////

  class t_tipc_rdm_server;
  using r_tipc_rdm_server = t_prefix<t_tipc_rdm_server>::r_;
  using x_tipc_rdm_server = t_prefix<t_tipc_rdm_server>::x_;
  using R_tipc_rdm_server = t_prefix<t_tipc_rdm_server>::R_;
  using p_tipc_rdm_server = t_prefix<t_tipc_rdm_server>::p_;
  using P_tipc_rdm_server = t_prefix<t_tipc_rdm_server>::P_;

  class t_tipc_rdm_server {
  public:
    t_tipc_rdm_server(       R_tipc_address server) noexcept;
    t_tipc_rdm_server(t_err, R_tipc_address server) noexcept;

    t_tipc_rdm_server(x_tipc_rdm_server)            noexcept;
   ~t_tipc_rdm_server();

    t_tipc_rdm_server(R_tipc_rdm_server)           = delete;
    r_tipc_rdm_server operator=(R_tipc_rdm_server) = delete;
    r_tipc_rdm_server operator=(x_tipc_rdm_server) = delete;

    operator t_validity() const noexcept;
    t_fd     get_fd    () const noexcept;

    t_errn getsockopt(       t_socket_level, r_socket_option) const noexcept;
    t_void getsockopt(t_err, t_socket_level, r_socket_option) const noexcept;

    t_errn setsockopt(       t_socket_level, R_socket_option) noexcept;
    t_void setsockopt(t_err, t_socket_level, R_socket_option) noexcept;

    t_verify<t_n> sendto(       R_byte_crange, R_tipc_address,
                                t_flags) noexcept;
    t_n           sendto(t_err, R_byte_crange, R_tipc_address,
                                t_flags) noexcept;

    t_verify<t_n> recvfrom(       r_byte_range, r_tipc_address,
                                  t_flags) noexcept;
    t_n           recvfrom(t_err, r_byte_range, r_tipc_address,
                                  t_flags) noexcept;

    t_verify<t_n> sendmsg(       R_socket_msghdr, t_flags) noexcept;
    t_n           sendmsg(t_err, R_socket_msghdr, t_flags) noexcept;

    t_verify<t_n> recvmsg(       r_socket_msghdr, t_flags) noexcept;
    t_n           recvmsg(t_err, r_socket_msghdr, t_flags) noexcept;

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
    t_tipc_seqpacket_client(       R_tipc_address server) noexcept;
    t_tipc_seqpacket_client(t_err, R_tipc_address server) noexcept;

    t_tipc_seqpacket_client(x_tipc_seqpacket_client)      noexcept;
   ~t_tipc_seqpacket_client();

    t_tipc_seqpacket_client(R_tipc_seqpacket_client)           = delete;
    r_tipc_seqpacket_client operator=(R_tipc_seqpacket_client) = delete;
    r_tipc_seqpacket_client operator=(x_tipc_seqpacket_client) = delete;

    operator t_validity() const noexcept;
    t_fd     get_fd    () const noexcept;

    t_errn getpeername(       r_tipc_address) const noexcept;
    t_void getpeername(t_err, r_tipc_address) const noexcept;

    t_errn getsockname(       r_tipc_address) const noexcept;
    t_void getsockname(t_err, r_tipc_address) const noexcept;

    t_verify<t_n> send(       R_byte_crange, t_flags) noexcept;
    t_n           send(t_err, R_byte_crange, t_flags) noexcept;

    t_verify<t_n> recv(       r_byte_range, t_flags) noexcept;
    t_n           recv(t_err, r_byte_range, t_flags) noexcept;

    t_verify<t_n> sendmsg(       R_socket_msghdr, t_flags) noexcept;
    t_n           sendmsg(t_err, R_socket_msghdr, t_flags) noexcept;

    t_verify<t_n> recvmsg(       r_socket_msghdr, t_flags) noexcept;
    t_n           recvmsg(t_err, r_socket_msghdr, t_flags) noexcept;

    t_errn getsockopt(       t_socket_level, r_socket_option) const noexcept;
    t_void getsockopt(t_err, t_socket_level, r_socket_option) const noexcept;

    t_errn setsockopt(       t_socket_level, R_socket_option) noexcept;
    t_void setsockopt(t_err, t_socket_level, R_socket_option) noexcept;

  private:
    t_socket socket_;
  };

///////////////////////////////////////////////////////////////////////////////

  class t_tipc_seqpacket_server;
  using r_tipc_seqpacket_server = t_prefix<t_tipc_seqpacket_server>::r_;
  using x_tipc_seqpacket_server = t_prefix<t_tipc_seqpacket_server>::x_;
  using R_tipc_seqpacket_server = t_prefix<t_tipc_seqpacket_server>::R_;
  using p_tipc_seqpacket_server = t_prefix<t_tipc_seqpacket_server>::p_;
  using P_tipc_seqpacket_server = t_prefix<t_tipc_seqpacket_server>::P_;

  class t_tipc_seqpacket_server {
  public:
    t_tipc_seqpacket_server(       R_tipc_address)   noexcept;
    t_tipc_seqpacket_server(t_err, R_tipc_address)   noexcept;

    t_tipc_seqpacket_server(x_tipc_seqpacket_server) noexcept;
   ~t_tipc_seqpacket_server();

    t_tipc_seqpacket_server(R_tipc_seqpacket_server)           = delete;
    r_tipc_seqpacket_server operator=(R_tipc_seqpacket_server) = delete;
    r_tipc_seqpacket_server operator=(x_tipc_seqpacket_server) = delete;

    operator t_validity() const noexcept;
    t_fd     get_fd    () const noexcept;

///////////////////////////////////////////////////////////////////////////////

    t_connect_id   accept_connection()            noexcept;
    t_bool         close_connection(t_connect_id) noexcept;

    R_connect_info get_connection(t_connect_id)      const noexcept;
    t_void         get_connection_ids(r_connect_ids) const noexcept;

///////////////////////////////////////////////////////////////////////////////

    // shutdown/close
    t_errn getpeername(       t_connect_id, r_tipc_address) const noexcept;
    t_void getpeername(t_err, t_connect_id, r_tipc_address) const noexcept;

    t_errn getsockname(       t_connect_id, r_tipc_address) const noexcept;
    t_void getsockname(t_err, t_connect_id, r_tipc_address) const noexcept;

    t_verify<t_n> send(       t_connect_id, R_byte_crange, t_flags) noexcept;
    t_n           send(t_err, t_connect_id, R_byte_crange, t_flags) noexcept;

    t_verify<t_n> recv(       t_connect_id, r_byte_range, t_flags) noexcept;
    t_n           recv(t_err, t_connect_id, r_byte_range, t_flags) noexcept;

    t_verify<t_n> sendmsg(       t_connect_id, R_socket_msghdr,
                                 t_flags) noexcept;
    t_n           sendmsg(t_err, t_connect_id, R_socket_msghdr,
                                 t_flags) noexcept;

    t_verify<t_n> recvmsg(       t_connect_id, r_socket_msghdr,
                                 t_flags) noexcept;
    t_n           recvmsg(t_err, t_connect_id, r_socket_msghdr,
                                 t_flags) noexcept;

    t_errn getsockopt(       t_connect_id, t_socket_level,
                             r_socket_option) const noexcept;
    t_void getsockopt(t_err, t_connect_id, t_socket_level,
                             r_socket_option) const noexcept;

    t_errn setsockopt(       t_connect_id, t_socket_level,
                             R_socket_option) noexcept;
    t_void setsockopt(t_err, t_connect_id, t_socket_level,
                             R_socket_option) noexcept;

///////////////////////////////////////////////////////////////////////////////

    t_errn getpeername(       r_tipc_address) const noexcept;
    t_void getpeername(t_err, r_tipc_address) const noexcept;

    t_errn getsockname(       r_tipc_address) const noexcept;
    t_void getsockname(t_err, r_tipc_address) const noexcept;

    t_verify<t_n> send(       R_byte_crange, t_flags) noexcept;
    t_n           send(t_err, R_byte_crange, t_flags) noexcept;

    t_verify<t_n> recv(       r_byte_range, t_flags) noexcept;
    t_n           recv(t_err, r_byte_range, t_flags) noexcept;

    t_verify<t_n> sendmsg(       R_socket_msghdr, t_flags) noexcept;
    t_n           sendmsg(t_err, R_socket_msghdr, t_flags) noexcept;

    t_verify<t_n> recvmsg(       r_socket_msghdr, t_flags) noexcept;
    t_n           recvmsg(t_err, r_socket_msghdr, t_flags) noexcept;

    t_errn getsockopt(       t_socket_level, r_socket_option) const noexcept;
    t_void getsockopt(t_err, t_socket_level, r_socket_option) const noexcept;

    t_errn setsockopt(       t_socket_level, R_socket_option) noexcept;
    t_void setsockopt(t_err, t_socket_level, R_socket_option) noexcept;

  private:
    t_socket socket_;
  };

///////////////////////////////////////////////////////////////////////////////

}
}
}

#endif
