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

#ifndef _DAINTY_MT_NET_IP_H_
#define _DAINTY_MT_NET_IP_H_

#include "dainty_named.h"
#include "dainty_mt_err.h"
#include "dainty_os_networking.h"

namespace dainty
{
namespace mt
{
namespace net_ip
{
  using named::t_n_;

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

///////////////////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////////////

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
