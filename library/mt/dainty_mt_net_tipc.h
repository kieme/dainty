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

#include "dainty_base.h"
#include "dainty_mt_err.h"
#include "dainty_mt_net_connect.h"

// XXX - each class must get stats - count

namespace dainty
{
namespace mt
{
namespace net_tipc
{
///////////////////////////////////////////////////////////////////////////////

  using err::t_err;

  using base::types::t_prefix;
  using base::types::t_void;
  using base::types::t_bool;

  using base::specific::t_validity;
  using base::specific::VALID;
  using base::specific::INVALID;
  using base::specific::t_n;
  using base::specific::t_fd;
  using base::specific::t_errn;
  using base::specific::NO_ERRN;
  using base::specific::BAD_ERRN;

  using os::t_verify;
  using os::t_socket_address;
  using os::t_socket_address_len;
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
  using os::networking::BAD_FD;

  using net_connect::t_connect_user;
  using net_connect::r_connect_user;
  using net_connect::t_connect_id;
  using net_connect::t_connect_ids;
  using net_connect::r_connect_ids;
  using net_connect::t_connect_result;
  using net_connect::t_connect_stats;
  using net_connect::t_connect_info;
  using net_connect::R_connect_info;
  using net_connect::P_connect_info;
  using net_connect::t_connect_table;
  using net_connect::BAD_CONNECT_ID;

///////////////////////////////////////////////////////////////////////////////

  class t_tipc_address final : public t_socket_address {
  public:
    // constructor
    t_tipc_address() noexcept : t_socket_address{t_socket_address_len{0}} {
    }

    operator p_sockaddr()       noexcept override;
    operator P_sockaddr() const noexcept override;

    //XXX - must store address
  };
  using r_tipc_address = t_prefix<t_tipc_address>::r_;
  using R_tipc_address = t_prefix<t_tipc_address>::R_;
  using p_tipc_address = t_prefix<t_tipc_address>::p_;
  using P_tipc_address = t_prefix<t_tipc_address>::P_;

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
    class t_params {
    public:
      t_params(R_tipc_address _server, t_n _max_connects)
        : server(_server), max_connects(_max_connects) {
      }

      t_tipc_address server;
      t_n            max_connects;
    };
    using T_params = t_prefix<t_params>::T_;
    using R_params = t_prefix<t_params>::R_;

    class t_logic {
    public:
      using t_fd           = net_tipc::t_fd;
      using t_errn         = net_tipc::t_errn;
      using R_tipc_address = net_tipc::R_tipc_address;
      using R_connect_info = net_tipc::R_connect_info;
      using r_connect_user = net_tipc::r_connect_user;

      virtual ~t_logic() {}
      virtual t_bool notify_tipc_stream_connect_accept(R_tipc_address,
                                                       r_connect_user) noexcept = 0;
      virtual t_void notify_tipc_stream_connect_add   (R_connect_info,
                                                       r_connect_user) noexcept = 0;
      virtual t_void notify_tipc_stream_connect_del   (R_connect_info) noexcept = 0;
      virtual t_void notify_tipc_stream_connect_error (t_errn)         noexcept = 0;
    };
    using r_logic = t_prefix<t_logic>::r_;

    t_tipc_stream_server(       R_params, r_logic) noexcept;
    t_tipc_stream_server(t_err, R_params, r_logic) noexcept;

    t_tipc_stream_server(x_tipc_stream_server) noexcept;
   ~t_tipc_stream_server();

    t_tipc_stream_server(R_tipc_stream_server)           = delete;
    r_tipc_stream_server operator=(R_tipc_stream_server) = delete;
    r_tipc_stream_server operator=(x_tipc_stream_server) = delete;

    operator t_validity() const noexcept;
    t_fd     get_fd    () const noexcept;

///////////////////////////////////////////////////////////////////////////////

    t_connect_result accept_connection()             noexcept;
    t_connect_result accept_connection(t_err)        noexcept;
    t_bool           close_connection (t_connect_id) noexcept;

    P_connect_info get_connection    (t_connect_id)  const noexcept;
    t_void         get_connection_ids(r_connect_ids) const noexcept;

///////////////////////////////////////////////////////////////////////////////

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
    T_params        params_;
    t_socket        socket_;
    r_logic         logic_;
    t_connect_table table_;
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
    class t_logic {
    public:
      using t_fd           = net_tipc::t_fd;
      using t_errn         = net_tipc::t_errn;
      using R_tipc_address = net_tipc::R_tipc_address;
      using R_connect_info = net_tipc::R_connect_info;
      using r_connect_user = net_tipc::r_connect_user;

      virtual ~t_logic() {}
      virtual t_bool notify_tipc_seqpacket_connect_accept(t_fd, R_tipc_address,
                                                          r_connect_user) noexcept = 0;
      virtual t_void notify_tipc_seqpacket_connect_add   (R_connect_info,
                                                          r_connect_user) noexcept = 0;
      virtual t_void notify_tipc_seqpacket_connect_del   (R_connect_info) noexcept = 0;
      virtual t_void notify_tipc_seqpacket_connect_error (t_errn)         noexcept = 0;
    };
    using r_logic = t_prefix<t_logic>::r_;

    t_tipc_seqpacket_server(       R_tipc_address, r_logic) noexcept;
    t_tipc_seqpacket_server(t_err, R_tipc_address, r_logic) noexcept;

    t_tipc_seqpacket_server(x_tipc_seqpacket_server) noexcept;
   ~t_tipc_seqpacket_server();

    t_tipc_seqpacket_server(R_tipc_seqpacket_server)           = delete;
    r_tipc_seqpacket_server operator=(R_tipc_seqpacket_server) = delete;
    r_tipc_seqpacket_server operator=(x_tipc_seqpacket_server) = delete;

    operator t_validity() const noexcept;
    t_fd     get_fd    () const noexcept;

///////////////////////////////////////////////////////////////////////////////

    t_connect_result accept_connection()            noexcept;
    t_bool           close_connection(t_connect_id) noexcept;

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
