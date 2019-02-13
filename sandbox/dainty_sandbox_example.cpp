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
#include "dainty_sandbox.h"
#include "dainty_sandbox_logic_ext_cli.h"
#include "dainty_sandbox_logic_ext_large_msg.h"

using namespace dainty::sandbox;
using namespace dainty::named::terminal;

///////////////////////////////////////////////////////////////////////////////

class t_app_logic : public  t_logic,
                    private t_logic_cli_ext,
                    private t_logic_large_msg_ext {
public:
  t_app_logic(t_err err) noexcept
    : t_logic              {err, "app_msngr"},
      t_logic_cli_ext      {err, *this},
      t_logic_large_msg_ext{err, *this} {
  }

  t_void notify_large_msg_start(t_err err) noexcept override final {
    ERR_GUARD(err) {
      t_out{"t_app_logic:ext_large_msg_start"};
    }
  }

  t_void notify_large_msg_cleanup() noexcept override final {
    t_out{"t_app_logic:large_msg_cleanup"};
  }

  t_void notify_cli_start(t_err err) noexcept override final {
    ERR_GUARD(err) {
      t_out{"t_app_logic:cli_start"};
    }
  }

  t_void notify_cli_cleanup() noexcept override final {
    t_out{"t_app_logic:cli_cleanup"};
  }

  t_void notify_start(t_err err) noexcept override final {
    ERR_GUARD(err) {
      t_out{"t_app_logic:start"};
    }
  }

  t_void notify_cleanup() noexcept override final {
    t_out{"t_app_logic:cleanup"};
  }

  t_void notify_spin(t_bool exipred, t_msec delta) noexcept override final {
    t_out{"t_app_logic:notify_spin"};
  }

  t_void notify_timer_timout(t_timer_id, R_timer_info) noexcept override final {
    t_out{"t_app_logic:notify_timer_timout"};
  }

private:
};
using p_app_logic = t_prefix<t_app_logic>::p_;

///////////////////////////////////////////////////////////////////////////////

class t_app1 {
public:
  t_app1(t_err err)
    : logic_  {err},
      sandbox_{err, IN_NEW_THREAD, "app_thread", {&logic_, nullptr}} {
  }
private:
  t_app_logic logic_;
  t_sandbox   sandbox_;
};

class t_app2 {
public:
  static t_logic_ptrlist mk_(t_err err, p_app_logic logic1,
                                        p_app_logic logic2,
                                        p_app_logic logic3) {
    t_logic_ptrlist ptrlist{err, t_n{3}};
    ERR_GUARD(err) {
      ptrlist.push_back(err, {logic1, nullptr});
      ptrlist.push_back(err, {logic2, nullptr});
      ptrlist.push_back(err, {logic3, nullptr});
    }
    return ptrlist;
  }
  t_app2(t_err err)
    : logic1_ {err},
      logic2_ {err},
      logic3_ {err},
      sandbox_{err, IN_NEW_THREAD, "app_thread",
                    mk_(err, &logic1_, &logic2_, &logic3_)} {
  }
private:
  t_app_logic logic1_;
  t_app_logic logic2_;
  t_app_logic logic3_;
  t_sandbox   sandbox_;
};

///////////////////////////////////////////////////////////////////////////////

int main1() {
  {
    t_err err;
    t_app1 app1{err.tag(1)};
    t_app1 app2{err.tag(2)};
    t_app1 app3{err.tag(3)};

    sleep(1);

    if (err) {
      switch (err.tag()) {
        case 1: { t_out{"app1 failed"}; break; }
        case 2: { t_out{"app2 failed"}; break; }
        case 3: { t_out{"app3 failed"}; break; }
      }
      err.print();
      err.clear();
    }
  }

  sleep(2);

  t_out{"main is exiting"};

  return 0;
}

int main2() {
  {
    t_err err;
    t_app2 app{err};

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

int main() {
  wait_services();
  main1();
  main2();
  return 0;
}

///////////////////////////////////////////////////////////////////////////////
