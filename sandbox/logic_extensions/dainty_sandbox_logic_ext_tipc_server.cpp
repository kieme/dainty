/******************************************************************************

MIT License

Copyright (c) 2018 frits.germs@gmx.net

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

#include "dainty_named_terminal.h"
#include "dainty_sandbox_logic_ext_tipc_server.h"

namespace dainty
{
namespace sandbox
{
namespace logic_tipc_server_ext
{
  using namespace named::terminal;

///////////////////////////////////////////////////////////////////////////////

  t_logic_tipc_server_ext::t_logic_tipc_server_ext(t_err err,
                                                 r_logic logic) noexcept
    : impl_{err, logic, *this} {
  }

///////////////////////////////////////////////////////////////////////////////

  t_logic_tipc_server_ext::t_impl_::t_impl_(t_err err, r_logic logic,
                                    r_ext_ ext) noexcept
      : t_logic_ext{"tipc_server", logic}, ext_{ext} {
    ERR_GUARD(err) {
      register_(err, logic, this);
    }
  }

  t_void t_logic_tipc_server_ext::t_impl_::notify_start(t_err err) noexcept {
    ERR_GUARD(err) {
      t_out{"t_logic_tipc_server_ext::t_impl_::notify_start"};
      ext_.notify_tipc_server_start(err);
    }
  }

  t_void t_logic_tipc_server_ext::t_impl_::notify_cleanup() noexcept {
    ext_.notify_tipc_server_cleanup();
    t_out{"t_logic_tipc_server_ext::t_impl_::notify_cleanup"};
  }

  t_void t_logic_tipc_server_ext::t_impl_::notify_wakeup(t_msec) noexcept {
    t_out{"t_logic_tipc_server_ext::t_impl_::notify_wakeup"};
  }

  t_void t_logic_tipc_server_ext::t_impl_::notify_complete() noexcept {
    t_out{"t_logic_tipc_server_ext::t_impl_::notify_complete"};
  }

  t_void t_logic_tipc_server_ext::t_impl_
      ::notify_timeout(t_timer_id, R_timer_params) noexcept {
    t_out{"t_logic_tipc_server_ext::t_impl_::notify_timeout"};
  }

  t_void t_logic_tipc_server_ext::t_impl_
      ::notify_fdevent(t_fdevent_id, R_fdevent_params) noexcept {
    t_out{"t_logic_tipc_server_ext::t_impl_::notify_fdevent"};
  }

///////////////////////////////////////////////////////////////////////////////
}
}
}
