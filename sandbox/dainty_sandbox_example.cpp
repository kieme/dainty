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

  virtual t_void notify_large_msg_start(t_err err) noexcept override final {
    ERR_GUARD(err) {
      t_out{"t_app_logic:ext_large_msg_start"};
    }
  }

  virtual t_void notify_large_msg_cleanup() noexcept override final {
    t_out{"t_app_logic:large_msg_cleanup"};
  }

  virtual t_void notify_cli_start(t_err err) noexcept override final {
    ERR_GUARD(err) {
      t_out{"t_app_logic:cli_start"};
    }
  }

  virtual t_void notify_cli_cleanup() noexcept override final {
    t_out{"t_app_logic:cli_cleanup"};
  }

  virtual t_void notify_start(t_err err) noexcept override final {
    ERR_GUARD(err) {
      t_out{"t_app_logic:start"};
    }
  }

  virtual t_void notify_cleanup() noexcept override final {
    t_out{"t_app_logic:cleanup"};
  }

private:
};

///////////////////////////////////////////////////////////////////////////////

class t_app {
public:
  t_app(t_err err)
    : logic_  {err},
      sandbox_{err, IN_NEW_THREAD, "app_thread", {&logic_, nullptr}} {
  }
private:
  t_app_logic logic_;
  t_sandbox   sandbox_;
};

///////////////////////////////////////////////////////////////////////////////

int main() {
  {
    t_err err;
    t_app app1{err.tag(1)};
    t_app app2{err.tag(2)};
    t_app app3{err.tag(3)};

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
