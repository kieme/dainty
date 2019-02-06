#include <unistd.h>
#include "dainty_sandbox.h"
#include "dainty_named_terminal.h"

using namespace dainty::sandbox;
using namespace dainty::named::terminal;

using dainty::named::P_cstr;

///////////////////////////////////////////////////////////////////////////////

class t_cli_logic_ext {
public:
  t_cli_logic_ext(t_err err, r_logic logic) noexcept
    : ext_{err, logic, *this} {
  }

private:
  virtual t_void cli_start(t_err) noexcept = 0;
  virtual t_void cli_cleanup()    noexcept = 0;

private:
  using r_callback = t_prefix<t_cli_logic_ext>::r_;

  class t_ext_ : public t_logic_ext_ {
  public:
    t_ext_(t_err err, r_logic logic, r_callback callback) noexcept
      : t_logic_ext_{"cli", logic}, callback_{callback} {
      ERR_GUARD(err) {
        register_(err, get_logic(), this);
      }
    }

    virtual t_void start(t_err err) noexcept override final {
      ERR_GUARD(err) {
        callback_.cli_start(err);
      }
    }

    virtual t_void cleanup() noexcept override final {
      callback_.cli_cleanup();
    }

  private:
    r_callback callback_;
  };

  t_ext_ ext_;
};

///////////////////////////////////////////////////////////////////////////////

class t_datashare_logic_ext {
public:
  t_datashare_logic_ext(t_err err, r_logic logic) noexcept
    : ext_{err, logic, *this} {
  }

private:
  virtual t_void datashare_start(t_err) noexcept = 0;
  virtual t_void datashare_cleanup()    noexcept = 0;

private:
  using r_callback = t_prefix<t_datashare_logic_ext>::r_;

  class t_ext_ : public t_logic_ext_ {
  public:
    t_ext_(t_err err, r_logic logic, r_callback callback) noexcept
      : t_logic_ext_{"datashare", logic}, callback_{callback} {
      ERR_GUARD(err) {
        register_(err, get_logic(), this);
      }
    }

    virtual t_void start(t_err err) noexcept override final {
      ERR_GUARD(err) {
        callback_.datashare_start(err);
      }
    }

    virtual t_void cleanup() noexcept override final {
      callback_.datashare_cleanup();
    }

  private:
    r_callback callback_;
  };

  t_ext_ ext_;
};

///////////////////////////////////////////////////////////////////////////////

class t_app_logic : public  t_logic,
                    private t_cli_logic_ext,
                    private t_datashare_logic_ext {
public:
  t_app_logic(t_err err) noexcept
    : t_logic              {err, "app_msngr"},
      t_cli_logic_ext      {err, *this},
      t_datashare_logic_ext{err, *this} {
  }

  virtual t_void datashare_start(t_err err) noexcept override final {
    ERR_GUARD(err) {
      t_out{"t_app_logic:datashare_start"};
    }
  }

  virtual t_void datashare_cleanup() noexcept override final {
    t_out{"t_app_logic:datashare_cleanup"};
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
  t_app(t_err err) : logic_ (err),
                     thread_(err, "app_thread", {&logic_, nullptr}) {
  }

private:
  t_app_logic logic_;
  t_main      thread_;
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
  return 0;
}
