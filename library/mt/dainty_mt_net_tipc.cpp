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
#include "dainty_mt_net_tipc.h"

namespace dainty
{
namespace mt
{
namespace net_tipc
{
  using named::utility::reset;
  using named::utility::x_cast;

  using os::t_socket_domain;
  using os::t_socket_type;
  using os::t_socket_protocol;

///////////////////////////////////////////////////////////////////////////////

  t_tipc_stream_client::t_tipc_stream_client(R_tipc_address server) noexcept
      : socket_{t_socket_domain{0}, //XXX
                t_socket_type{0}, //XXX
                t_socket_protocol{0}} { //XXX
    if (socket_.connect(server) == INVALID)
      socket_.close();
  }

  t_tipc_stream_client::t_tipc_stream_client(t_err err,
                                             R_tipc_address server) noexcept
      : socket_{err, t_socket_domain{0}, //XXX
                     t_socket_type{0}, //XXX
                     t_socket_protocol{0}} { //XXX
    ERR_GUARD(err) {
      socket_.connect(err, server);
      if (err)
        socket_.close();
    }
  }

  t_tipc_stream_client
    ::t_tipc_stream_client(x_tipc_stream_client client) noexcept
      : socket_{x_cast(client.socket_)} {
  }

  t_tipc_stream_client::operator t_validity() const noexcept {
    return socket_;
  }

  t_fd t_tipc_stream_client::get_fd() const noexcept {
    return socket_.get_fd();
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

  t_tipc_stream_server::t_tipc_stream_server(R_tipc_address server,
                                             r_logic logic) noexcept
      : socket_{t_socket_domain{0}, //XXX
                t_socket_type{0},   //XXX
                t_socket_protocol{0}}, logic_{logic} {
    if (socket_ == VALID) {
      // bind
      // listen
    }
  }

  t_tipc_stream_server::t_tipc_stream_server(t_err err, R_tipc_address server,
                                             r_logic logic) noexcept
      : socket_{err, t_socket_domain{0}, //XXX
                     t_socket_type{0},   //XXX
                     t_socket_protocol{0}}, logic_{logic} {
    ERR_GUARD(err) {
      // bind
      // listen
    }
  }

  t_tipc_stream_server
    ::t_tipc_stream_server(x_tipc_stream_server server) noexcept
      : socket_{x_cast(server.socket_)}, logic_{server.logic_} {
    // XXX move - don't forget logic_
  }

  t_tipc_stream_server::~t_tipc_stream_server() {
    // no sure I need this
  }

  t_tipc_stream_server::operator t_validity() const noexcept {
    return socket_;
  }

  t_fd t_tipc_stream_server::get_fd() const noexcept {
    return socket_.get_fd();
  }

  t_connect_result t_tipc_stream_server::accept_connection() noexcept {
  }

  t_bool t_tipc_stream_server::close_connection(t_connect_id) noexcept {
  }

  R_connect_info t_tipc_stream_server
      ::get_connection(t_connect_id) const noexcept {
  }

  t_void t_tipc_stream_server
      ::get_connection_ids(r_connect_ids) const noexcept {
  }

  t_errn t_tipc_stream_server
      ::getpeername(t_connect_id, r_tipc_address) const noexcept {
  }

  t_void t_tipc_stream_server
      ::getpeername(t_err, t_connect_id, r_tipc_address) const noexcept {
  }

  t_errn t_tipc_stream_server
      ::getsockname(t_connect_id, r_tipc_address) const noexcept {
  }

  t_void t_tipc_stream_server
      ::getsockname(t_err, t_connect_id, r_tipc_address) const noexcept {
  }

  t_verify<t_n> t_tipc_stream_server
      ::send(t_connect_id, R_byte_crange, t_flags) noexcept {
  }

  t_n t_tipc_stream_server
      ::send(t_err, t_connect_id, R_byte_crange, t_flags) noexcept {
  }

  t_verify<t_n> t_tipc_stream_server
      ::recv(t_connect_id, r_byte_range, t_flags) noexcept {
  }

  t_n t_tipc_stream_server
      ::recv(t_err, t_connect_id, r_byte_range, t_flags) noexcept {
  }

  t_verify<t_n> t_tipc_stream_server
      ::sendmsg(t_connect_id, R_socket_msghdr, t_flags) noexcept {
  }

  t_n t_tipc_stream_server
      ::sendmsg(t_err, t_connect_id, R_socket_msghdr, t_flags) noexcept {
  }

  t_verify<t_n> t_tipc_stream_server
      ::recvmsg(t_connect_id, r_socket_msghdr, t_flags) noexcept {
  }

  t_n t_tipc_stream_server
      ::recvmsg(t_err, t_connect_id, r_socket_msghdr, t_flags) noexcept {
  }

  t_errn t_tipc_stream_server
      ::getsockopt(t_connect_id, t_socket_level,
                   r_socket_option) const noexcept {
  }

  t_void t_tipc_stream_server
      ::getsockopt(t_err, t_connect_id, t_socket_level,
                   r_socket_option) const noexcept {
  }

  t_errn t_tipc_stream_server
      ::setsockopt(t_connect_id, t_socket_level, R_socket_option) noexcept {
  }

  t_void t_tipc_stream_server
      ::setsockopt(t_err, t_connect_id, t_socket_level,
                   R_socket_option) noexcept {
  }

  t_errn t_tipc_stream_server::getpeername(r_tipc_address) const noexcept {
  }

  t_void t_tipc_stream_server
      ::getpeername(t_err, r_tipc_address) const noexcept {
  }

  t_errn t_tipc_stream_server
      ::getsockname(r_tipc_address) const noexcept {
  }

  t_void t_tipc_stream_server
      ::getsockname(t_err, r_tipc_address) const noexcept {
  }

  t_verify<t_n> t_tipc_stream_server
      ::send(R_byte_crange, t_flags) noexcept {
  }

  t_n t_tipc_stream_server
      ::send(t_err, R_byte_crange, t_flags) noexcept {
  }

  t_verify<t_n> t_tipc_stream_server
      ::recv(r_byte_range, t_flags) noexcept {
  }

  t_n t_tipc_stream_server
      ::recv(t_err, r_byte_range, t_flags) noexcept {
  }

  t_verify<t_n> t_tipc_stream_server
      ::sendmsg(R_socket_msghdr, t_flags) noexcept {
  }

  t_n t_tipc_stream_server
      ::sendmsg(t_err, R_socket_msghdr, t_flags) noexcept {
  }

  t_verify<t_n> t_tipc_stream_server
      ::recvmsg(r_socket_msghdr, t_flags) noexcept {
  }

  t_n t_tipc_stream_server
      ::recvmsg(t_err, r_socket_msghdr, t_flags) noexcept {
  }

  t_errn t_tipc_stream_server
      ::getsockopt(t_socket_level, r_socket_option) const noexcept {
  }

  t_void t_tipc_stream_server
      ::getsockopt(t_err, t_socket_level, r_socket_option) const noexcept {
  }

  t_errn t_tipc_stream_server
      ::setsockopt(t_socket_level, R_socket_option) noexcept {
  }

  t_void t_tipc_stream_server
      ::setsockopt(t_err, t_socket_level, R_socket_option) noexcept {
  }

///////////////////////////////////////////////////////////////////////////////
}
}
}
