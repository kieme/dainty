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

#ifndef _DAINTY_OS_NET_CONNECT_H_
#define _DAINTY_OS_NET_CONNECT_H_

// description
// os: operating system functionality used by dainty
//
//  not a complete API but only the things used by dainty.

#include "dainty_base.h"
#include "dainty_os_networking.h"
#include "dainty_container_list.h"
#include "dainty_container_maybe.h"
#include "dainty_mt_err.h"

namespace dainty
{
namespace mt
{
namespace net_connect
{
  using err::t_err;

  using base::t_prefix;
  using base::t_explicit;
  using base::t_fd;
  using base::t_n;
  using base::t_void;
  using base::t_bool;
  using base::BAD_FD;
  using base::t_user;
  using base::t_validity;
  using base::VALID;
  using base::INVALID;

  using container::list::t_list;
  using container::maybe::t_maybe;

  using os::networking::t_connect_socket;
  using os::networking::x_connect_socket;

///////////////////////////////////////////////////////////////////////////////

  enum  t_connect_user_tag_ {};
  using t_connect_user = t_user<t_connect_user_tag_>;
  using r_connect_user = t_prefix<t_connect_user>::r_;

///////////////////////////////////////////////////////////////////////////////

  enum  t_connect_id_tag_ {};
  using t_connect_id_ = base::t_int;
  using t_connect_id  = t_explicit<t_connect_id_, t_connect_id_tag_>;

  constexpr t_connect_id BAD_CONNECT_ID{-1}; // BAD_ID - XXX

  using t_connect_ids = t_maybe<t_list<t_connect_id>>;
  using r_connect_ids = t_prefix<t_connect_ids>::r_;

///////////////////////////////////////////////////////////////////////////////

  class t_connect_result {
  public:
    constexpr t_connect_result(t_connect_id _id, t_fd _fd) noexcept
      : id{_id}, fd{_fd} {
    }

    constexpr operator t_validity() const noexcept {
      return id == BAD_CONNECT_ID ? INVALID : VALID;
    }

    t_connect_id id = BAD_CONNECT_ID;
    t_fd         fd = BAD_FD;
  };

///////////////////////////////////////////////////////////////////////////////

  struct t_connect_stats {
  };

///////////////////////////////////////////////////////////////////////////////

  struct t_connect_info {
    t_connect_id     id    = BAD_CONNECT_ID;
    t_connect_socket socket;
    t_connect_user   user;
    t_connect_stats  stats;
  };
  using R_connect_info = t_prefix<t_connect_info>::R_;
  using p_connect_info = t_prefix<t_connect_info>::p_;
  using P_connect_info = t_prefix<t_connect_info>::P_;

///////////////////////////////////////////////////////////////////////////////

  class t_connect_table;
  using R_connect_table = t_prefix<t_connect_table>::R_;
  using x_connect_table = t_prefix<t_connect_table>::x_;

  class t_connect_table {
  public:
    t_connect_table(       t_n)    noexcept;
    t_connect_table(t_err, t_n)    noexcept;
    t_connect_table(x_connect_table) noexcept;
   ~t_connect_table();

    t_connect_table(R_connect_table)           = delete;
    R_connect_table operator=(R_connect_table) = delete;

    operator t_validity() const noexcept;

    t_connect_id   add_connect(       x_connect_socket, t_connect_user) noexcept;
    t_connect_id   add_connect(t_err, x_connect_socket, t_connect_user) noexcept;

    t_bool         del_connect(       t_connect_id) noexcept;
    t_bool         del_connect(t_err, t_connect_id) noexcept;

    p_connect_info get_connect(       t_connect_id)       noexcept;
    p_connect_info get_connect(t_err, t_connect_id)       noexcept;

    P_connect_info get_connect(       t_connect_id) const noexcept;
    P_connect_info get_connect(t_err, t_connect_id) const noexcept;

    t_n            get_size       () const noexcept;
    t_n            get_capacity   () const noexcept;
    t_void         get_connect_ids(r_connect_ids) const noexcept;

  private:
  };

///////////////////////////////////////////////////////////////////////////////

}
}
}

#endif
