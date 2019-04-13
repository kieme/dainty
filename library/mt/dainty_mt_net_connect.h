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

#include "dainty_named.h"
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
  using named::t_prefix;
  using named::t_explicit;
  using named::t_fd;
  using named::BAD_FD;
  using named::t_user;
  using named::t_validity;
  using named::VALID;
  using named::INVALID;

  using container::list::t_list;
  using container::maybe::t_maybe;

///////////////////////////////////////////////////////////////////////////////

  enum  t_connect_user_tag_ {};
  using t_connect_user = t_user<t_connect_user_tag_>;
  using r_connect_user = t_prefix<t_connect_user>::r_;

  enum  t_connect_id_tag_ {};
  using t_connect_id_ = named::t_int;
  using t_connect_id  = t_explicit<t_connect_id_, t_connect_id_tag_>;

  constexpr t_connect_id BAD_CONNECT_ID{-1}; // BAD_ID - XXX

  using t_connect_ids = t_maybe<t_list<t_connect_id>>;
  using r_connect_ids = t_prefix<t_connect_ids>::r_;

  class t_connect_result {
  public:
    constexpr t_connect_result(t_connect_id _id, t_fd _fd) noexcept
      : id{_id}, fd{_fd} {
    }

    operator t_validity() const noexcept {
      return id == BAD_CONNECT_ID ? INVALID : VALID;
    }

    t_connect_id id = BAD_CONNECT_ID;
    t_fd         fd = BAD_FD;
  };

  struct t_connect_stats {
  };

  struct t_connect_info {
    t_fd            fd    = BAD_FD;
    t_connect_id    id    = BAD_CONNECT_ID;
    t_connect_user  user;
    t_connect_stats stats;
  };
  using R_connect_info = t_prefix<t_connect_info>::R_;

///////////////////////////////////////////////////////////////////////////////

}
}
}

#endif
