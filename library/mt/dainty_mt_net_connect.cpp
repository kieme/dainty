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

#include "dainty_mt_net_connect.h"

namespace dainty
{
namespace mt
{
namespace net_connect
{
///////////////////////////////////////////////////////////////////////////////

  t_connect_table::t_connect_table(t_n) noexcept {
  }

  t_connect_table::t_connect_table(t_err, t_n) noexcept {
  }

  t_connect_table::t_connect_table(x_connect_table) noexcept {
  }

  t_connect_table::~t_connect_table() {
  }

  t_connect_table::operator t_validity() const noexcept {
    return INVALID;
  }

  t_connect_id t_connect_table::add_connect(x_connect_socket,
                                            t_connect_user) noexcept {
    return BAD_CONNECT_ID;
  }

  t_connect_id t_connect_table::add_connect(t_err, x_connect_socket,
                                            t_connect_user) noexcept {
    return BAD_CONNECT_ID;
  }

  t_bool t_connect_table::del_connect(t_connect_id) noexcept {
    return false;
  }

  t_bool t_connect_table:: del_connect(t_err, t_connect_id) noexcept {
    return false;
  }

  p_connect_info t_connect_table::get_connect(t_connect_id) noexcept {
    return nullptr;
  }

  p_connect_info t_connect_table::get_connect(t_err, t_connect_id) noexcept {
    return nullptr;
  }

  P_connect_info t_connect_table::get_connect(t_connect_id) const noexcept {
    return nullptr;
  }

  P_connect_info t_connect_table::get_connect(t_err,
                                              t_connect_id) const noexcept {
    return nullptr;
  }

  t_n t_connect_table::get_size() const noexcept {
    return t_n{0};
  }

  t_n t_connect_table::get_capacity() const noexcept {
    return t_n{0};
  }

  t_void t_connect_table::get_connect_ids(r_connect_ids) const noexcept {
  }

///////////////////////////////////////////////////////////////////////////////
}
}
}

