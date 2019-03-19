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

///////////////////////////////////////////////////////////////////////////////

  t_tipc_stream_client::t_tipc_stream_client() noexcept :
    socket_{t_socket_domain{0}, t_socket_type{0}, t_socket_protocol{0}} {
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
