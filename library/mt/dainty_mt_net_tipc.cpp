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

#include "dainty_mt_net_tipc.h"

namespace dainty
{
namespace mt
{
namespace net_tipc
{
  using os::t_socket_domain;
  using os::t_socket_type;
  using os::t_socket_protocol;
  using os::t_socket_backlog;

///////////////////////////////////////////////////////////////////////////////

  t_tipc_address::operator p_sockaddr() noexcept {
    return nullptr; // XXX
  }

  t_tipc_address::operator P_sockaddr() const noexcept {
    return nullptr; // XXX
  }

///////////////////////////////////////////////////////////////////////////////

  t_tipc_stream_client::t_tipc_stream_client(R_tipc_address server) noexcept
      : socket_{t_socket_domain  {0},   //XXX
                t_socket_type    {0},   //XXX
                t_socket_protocol{0}} { //XXX
    if (socket_.connect(server) == INVALID)
      socket_.close();
  }

  t_tipc_stream_client::t_tipc_stream_client(t_err err,
                                             R_tipc_address server) noexcept
      : socket_{err, t_socket_domain  {0},   //XXX
                     t_socket_type    {0},   //XXX
                     t_socket_protocol{0}} { //XXX
    ERR_GUARD(err) {
      socket_.connect(err, server);
      if (err)
        socket_.close();
    }
  }

  t_tipc_stream_client
    ::t_tipc_stream_client(x_tipc_stream_client client) noexcept
      : socket_{base::x_cast(client.socket_)} {
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

  t_tipc_stream_server::t_tipc_stream_server(R_params params,
                                             r_logic logic) noexcept
      : params_{params},
        socket_{t_socket_domain  {0}, //XXX
                t_socket_type    {0}, //XXX
                t_socket_protocol{0}}, logic_{logic},
        table_{params_.max_connects} {
    if (socket_ == VALID) {
      auto errn = socket_.bind(params_.server);
      if (errn == VALID)
        errn = socket_.listen(t_socket_backlog{0});
      if (errn == INVALID)
        socket_.close();
    }
  }

  t_tipc_stream_server::t_tipc_stream_server(t_err err, R_params params,
                                             r_logic logic) noexcept
      : params_{params},
        socket_{err, t_socket_domain  {0}, //XXX
                     t_socket_type    {0}, //XXX
                     t_socket_protocol{0}}, logic_{logic},
        table_{err, params_.max_connects} {
    ERR_GUARD(err) {
      socket_.bind  (err, params_.server);
      socket_.listen(err, t_socket_backlog{0});
      if (err)
        socket_.close();
    }
  }

  t_tipc_stream_server
    ::t_tipc_stream_server(x_tipc_stream_server server) noexcept
      : params_{server.params_},
        socket_{base::x_cast(server.socket_)}, logic_{server.logic_},
        table_ {base::x_cast(server.table_)} {
    // XXX move - don't forget logic_
  }

  t_tipc_stream_server::~t_tipc_stream_server() {
  }

  t_tipc_stream_server::operator t_validity() const noexcept {
    return socket_;
  }

  t_fd t_tipc_stream_server::get_fd() const noexcept {
    return socket_.get_fd();
  }

  t_connect_result t_tipc_stream_server::accept_connection() noexcept {
    t_tipc_address client;
    auto ret = socket_.accept(client);
    if (ret == VALID) {
      t_connect_user user;
      if (logic_.notify_tipc_stream_connect_accept(client, user)) {
        auto fd = get(ret).get_fd();
        auto id = table_.add_connect(ret.release(), user);
        if (id != BAD_CONNECT_ID) {
          auto info = table_.get_connect(id);
          logic_.notify_tipc_stream_connect_add(*info, info->user);
          return {id, fd};
        }
      }
    }
    return {BAD_CONNECT_ID, BAD_FD};
  }

  t_connect_result t_tipc_stream_server::accept_connection(t_err err) noexcept {
    ERR_GUARD(err) {
      t_tipc_address client;
      auto ret = socket_.accept(err, client);
      if (ret == VALID) {
        t_connect_user user;
        if (logic_.notify_tipc_stream_connect_accept(client, user)) {
          auto fd = get(ret).get_fd();
          auto id = table_.add_connect(err, ret.release(), user);
          if (id != BAD_CONNECT_ID) {
            auto info = table_.get_connect(id);
            logic_.notify_tipc_stream_connect_add(*info, info->user);
            return {id, fd};
          }
        }
      }
    }
    return {BAD_CONNECT_ID, BAD_FD};
  }

  t_bool t_tipc_stream_server::close_connection(t_connect_id id) noexcept {
    return table_.del_connect(id);
  }

  P_connect_info t_tipc_stream_server
      ::get_connection(t_connect_id id) const noexcept {
    return table_.get_connect(id);
  }

  t_void t_tipc_stream_server
      ::get_connection_ids(r_connect_ids ids) const noexcept {
    table_.get_connect_ids(ids);
  }

  t_errn t_tipc_stream_server
      ::getpeername(t_connect_id id, r_tipc_address addr) const noexcept {
    auto info = table_.get_connect(id);
    return info ? info->socket.getpeername(addr) : BAD_ERRN;
  }

  t_void t_tipc_stream_server
      ::getpeername(t_err err, t_connect_id id,
                    r_tipc_address addr) const noexcept {
    ERR_GUARD(err) {
      auto info = table_.get_connect(err, id);
      if (info)
        info->socket.getpeername(err, addr);
    }
  }

  t_errn t_tipc_stream_server
      ::getsockname(t_connect_id id, r_tipc_address addr) const noexcept {
    auto info = table_.get_connect(id);
    return info ? info->socket.getsockname(addr) : BAD_ERRN;
  }

  t_void t_tipc_stream_server
      ::getsockname(t_err err, t_connect_id id,
                    r_tipc_address addr) const noexcept {
    ERR_GUARD(err) {
      auto info = table_.get_connect(err, id);
      if (info)
        info->socket.getsockname(err, addr);
    }
  }

  t_verify<t_n> t_tipc_stream_server
      ::send(t_connect_id id, R_byte_crange bytes, t_flags flags) noexcept {
    auto info = table_.get_connect(id);
    if (info)
      return info->socket.send(bytes, flags);
    return {t_n{0}, BAD_ERRN};
  }

  t_n t_tipc_stream_server
      ::send(t_err err, t_connect_id id, R_byte_crange bytes,
             t_flags flags) noexcept {
    ERR_GUARD(err) {
      auto info = table_.get_connect(err, id);
      if (info)
        return info->socket.send(err, bytes, flags);
    }
    return t_n{0};
  }

  t_verify<t_n> t_tipc_stream_server
      ::recv(t_connect_id id, r_byte_range bytes, t_flags flags) noexcept {
    auto info = table_.get_connect(id);
    if (info)
      return info->socket.recv(bytes, flags);
    return {t_n{0}, BAD_ERRN};
  }

  t_n t_tipc_stream_server
      ::recv(t_err err, t_connect_id id, r_byte_range bytes,
             t_flags flags) noexcept {
    ERR_GUARD(err) {
      auto info = table_.get_connect(err, id);
      if (info)
        return info->socket.recv(err, bytes, flags);
    }
    return t_n{0};
  }

  t_verify<t_n> t_tipc_stream_server
      ::sendmsg(t_connect_id id, R_socket_msghdr msghdr,
                t_flags flags) noexcept {
    auto info = table_.get_connect(id);
    if (info)
      return info->socket.sendmsg(msghdr, flags);
    return {t_n{0}, BAD_ERRN};
  }

  t_n t_tipc_stream_server
      ::sendmsg(t_err err, t_connect_id id, R_socket_msghdr msghdr,
                t_flags flags) noexcept {
    ERR_GUARD(err) {
      auto info = table_.get_connect(err, id);
      if (info)
        return info->socket.sendmsg(err, msghdr, flags);
    }
    return t_n{0};
  }

  t_verify<t_n> t_tipc_stream_server
      ::recvmsg(t_connect_id id, r_socket_msghdr msghdr,
                t_flags flags) noexcept {
    auto info = table_.get_connect(id);
    if (info)
      return info->socket.recvmsg(msghdr, flags);
    return {t_n{0}, BAD_ERRN};
  }

  t_n t_tipc_stream_server
      ::recvmsg(t_err err, t_connect_id id, r_socket_msghdr msghdr,
                t_flags flags) noexcept {
    ERR_GUARD(err) {
      auto info = table_.get_connect(err, id);
      if (info)
        return info->socket.recvmsg(err, msghdr, flags);
    }
    return t_n{0};
  }

  t_errn t_tipc_stream_server
      ::getsockopt(t_connect_id id, t_socket_level level,
                   r_socket_option option) const noexcept {
    auto info = table_.get_connect(id);
    return info ? info->socket.getsockopt(level, option) : BAD_ERRN;
  }

  t_void t_tipc_stream_server
      ::getsockopt(t_err err, t_connect_id id, t_socket_level level,
                   r_socket_option option) const noexcept {
    ERR_GUARD(err) {
      auto info = table_.get_connect(err, id);
      if (info)
        info->socket.getsockopt(err, level, option);
    }
  }

  t_errn t_tipc_stream_server
      ::setsockopt(t_connect_id id, t_socket_level level,
                   R_socket_option option) noexcept {
    auto info = table_.get_connect(id);
    return info ? info->socket.setsockopt(level, option) : BAD_ERRN;
  }

  t_void t_tipc_stream_server
      ::setsockopt(t_err err, t_connect_id id, t_socket_level level,
                   R_socket_option option) noexcept {
    ERR_GUARD(err) {
      auto info = table_.get_connect(err, id);
      if (info)
        info->socket.setsockopt(err, level, option);
    }
  }

  t_errn t_tipc_stream_server::getpeername(r_tipc_address addr) const noexcept {
    return socket_.getpeername(addr);
  }

  t_void t_tipc_stream_server
      ::getpeername(t_err err, r_tipc_address addr) const noexcept {
    ERR_GUARD(err) {
      socket_.getpeername(err, addr);
    }
  }

  t_errn t_tipc_stream_server
      ::getsockname(r_tipc_address addr) const noexcept {
    return socket_.getsockname(addr);
  }

  t_void t_tipc_stream_server
      ::getsockname(t_err err, r_tipc_address addr) const noexcept {
    ERR_GUARD(err) {
      socket_.getsockname(err, addr);
    }
  }

  t_verify<t_n> t_tipc_stream_server
      ::send(R_byte_crange bytes, t_flags flags) noexcept {
    return socket_.send(bytes, flags);
  }

  t_n t_tipc_stream_server
      ::send(t_err err, R_byte_crange bytes, t_flags flags) noexcept {
    ERR_GUARD(err) {
      return socket_.send(bytes, flags);
    }
    return t_n{0};
  }

  t_verify<t_n> t_tipc_stream_server
      ::recv(r_byte_range bytes, t_flags flags) noexcept {
    return socket_.recv(bytes, flags);
  }

  t_n t_tipc_stream_server
      ::recv(t_err err, r_byte_range bytes, t_flags flags) noexcept {
    ERR_GUARD(err) {
      return socket_.recv(err, bytes, flags);
    }
    return t_n{0};
  }

  t_verify<t_n> t_tipc_stream_server
      ::sendmsg(R_socket_msghdr msghdr, t_flags flags) noexcept {
    return socket_.sendmsg(msghdr, flags);
  }

  t_n t_tipc_stream_server
      ::sendmsg(t_err err, R_socket_msghdr msghdr, t_flags flags) noexcept {
    ERR_GUARD(err) {
      return socket_.sendmsg(err, msghdr, flags);
    }
    return t_n{0};
  }

  t_verify<t_n> t_tipc_stream_server
      ::recvmsg(r_socket_msghdr msghdr, t_flags flags) noexcept {
    return socket_.recvmsg(msghdr, flags);
  }

  t_n t_tipc_stream_server
      ::recvmsg(t_err err, r_socket_msghdr msghdr, t_flags flags) noexcept {
    ERR_GUARD(err) {
      return socket_.recvmsg(err, msghdr, flags);
    }
    return t_n{0};
  }

  t_errn t_tipc_stream_server
      ::getsockopt(t_socket_level level,
                   r_socket_option option) const noexcept {
    return socket_.getsockopt(level, option);
  }

  t_void t_tipc_stream_server
      ::getsockopt(t_err err, t_socket_level level,
                   r_socket_option option) const noexcept {
    ERR_GUARD(err) {
      socket_.getsockopt(err, level, option);
    }
  }

  t_errn t_tipc_stream_server
      ::setsockopt(t_socket_level level, R_socket_option option) noexcept {
    return socket_.setsockopt(level, option);
  }

  t_void t_tipc_stream_server
      ::setsockopt(t_err err, t_socket_level level,
                   R_socket_option option) noexcept {
    ERR_GUARD(err) {
      socket_.setsockopt(err, level, option);
    }
  }

///////////////////////////////////////////////////////////////////////////////
}
}
}
