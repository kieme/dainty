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

  virtual t_void large_msg_start(t_err err) noexcept override final {
    ERR_GUARD(err) {
      t_out{"t_app_logic:ext_large_msg_start"};
    }
  }

  virtual t_void large_msg_cleanup() noexcept override final {
    t_out{"t_app_logic:large_msg_cleanup"};
  }

  virtual t_void cli_start(t_err err) noexcept override final {
    ERR_GUARD(err) {
      t_out{"t_app_logic:cli_start"};
    }
  }

  virtual t_void cli_cleanup() noexcept override final {
    t_out{"t_app_logic:cli_cleanup"};
  }

  virtual t_void start(t_err err) noexcept override final {
    ERR_GUARD(err) {
      t_out{"t_app_logic:start"};
    }
  }

  virtual t_void cleanup() noexcept override final {
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
    t_app app{err};

    sleep(1);

    if (err) {
      err.print();
      err.clear();
    }
  }

  sleep(1);

  t_out{"main is existing"};

  return 0;
}
