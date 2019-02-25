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

#include <unistd.h>
#include "dainty_named_terminal.h"
#include "dainty_mt_event.h"
#include "dainty_sandbox.h"
#include "dainty_mt_timers.h"

using namespace dainty::named;
using namespace dainty::named::utility;
using namespace dainty::sandbox;
using namespace dainty::named::terminal;

using t_event_client          = dainty::mt::event::t_client;
using x_event_client          = dainty::mt::event::x_client;
using t_event_processor       = dainty::mt::event::t_processor;
using x_event_processor       = dainty::mt::event::x_processor;
using t_event_user            = dainty::mt::event::t_user;
using t_event_cnt             = dainty::mt::event::t_cnt;
using t_event_processor_logic = t_event_processor::t_logic;

///////////////////////////////////////////////////////////////////////////////

class t_app_logic : public t_logic {
public:
  using r_app_logic = t_prefix<t_app_logic>::r_;

  t_app_logic(t_err err, t_bool start,
              x_event_client client, x_event_processor processor) noexcept
    : t_logic    {err, "app_msngr"},
      start_     {start},
      proxy_     {*this},
      client_    {x_cast(client)},
      processor_ {x_cast(processor)} {
  }

  t_void notify_start(t_err err) noexcept override final {
    ERR_GUARD(err) {
      t_out{"t_app_logic:start"};
      add_fdevent(err, "time1", {processor_.get_fd(), FDEVENT_READ});

      if (start_)
        enable_spin(err, t_spin_cnt{500});
    }
  }

  t_void notify_cleanup() noexcept override final {
    t_out{"t_app_logic:cleanup"};
  }

  t_void notify_spin(t_msec msec) noexcept override final {
    t_out{FMT, "t_app_logic:notify_spin - msec %u", get(msec)};
    client_.post();
  }

  t_void notify_timeout(t_timer_id, R_timer_params) noexcept override final {
    t_out{"t_app_logic:notify_timer_timout"};
  }

  t_void notify_fdevent(t_fdevent_id, R_fdevent_params) noexcept override final {
    t_out{"t_app_logic:notify_fdevent"};

    t_err err;
    processor_.process(err, proxy_);
    if (!start_)
      client_.post(err);

    if (err) {
      err.print();
      err.clear();
    }
  }

  t_void notify_async_cnt(t_event_cnt cnt) {
    t_out{FMT, "t_app_logic:cnt-%lu", get(cnt)};
  }

private:
  struct t_event_proxy_ : t_event_processor_logic {
    r_app_logic logic_;
    t_event_proxy_(r_app_logic logic) : logic_(logic) { }

    t_void async_process(t_cnt cnt) noexcept override final {
      logic_.notify_async_cnt(cnt);
    }
  };

  T_bool            start_;
  t_event_proxy_    proxy_;
  t_event_client    client_;
  t_event_processor processor_;
};
using p_app_logic = t_prefix<t_app_logic>::p_;

///////////////////////////////////////////////////////////////////////////////

class t_app0 {
public:
  t_app0(t_err err, t_bool start, x_event_client client,
         x_event_processor processor)
    : logic_  {err, start, x_cast(client), x_cast(processor)},
      sandbox_{err, "app_thread", {&logic_, nullptr}, IN_NEW_THREAD} {
  }
private:
  t_app_logic logic_;
  t_sandbox   sandbox_;
};

int main() {
  {
    t_err err;

    t_event_user      user1{1};
    t_event_user      user2{2};
    t_event_processor processor1{err.tag(1)};
    t_event_processor processor2{err.tag(2)};
    t_event_client    client1{processor1.make_client(err.tag(3), user1)};
    t_event_client    client2{processor2.make_client(err.tag(4), user2)};

    t_app0 app1{err.tag(5), false, x_cast(client2), x_cast(processor1)};
    t_app0 app2{err.tag(6), true,  x_cast(client1), x_cast(processor2)};

    t_n_ cnt = 0;
    for (; cnt < 20; ++cnt)
      sleep(5);

    sleep(1);

    if (err) {
      err.print();
      err.clear();
    }
  }

  sleep(2);

  t_out{"main is exiting"};

  return 0;
}

///////////////////////////////////////////////////////////////////////////////
