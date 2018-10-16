#include <iostream>

namespace sandbox
{
  struct t_impl_;
  using  p_impl_ = t_impl_*;

  struct t_logic {
    // base api
    void do_1();

    // associated base callback
    virtual void callback_1() = 0;

  private:
    p_impl_ impl_;
  };
  using p_logic = t_logic*;
  using r_logic = t_logic&;

  struct t_sandbox {
     t_sandbox(p_logic);
    ~t_sandbox();
  };
}

namespace datashare
{
  struct t_impl_;
  using  p_impl_ = t_impl_*;

  struct t_logic {
    t_logic(sandbox::r_logic logic) : logic_{logic} {
    }

    // datashare api
    void do_2();

    // associated datashare callback
    virtual void callback_2() = 0;

  private:
    sandbox::r_logic logic_;
    p_impl_          impl_;
  };
  using p_logic = t_logic*;
}

namespace dacli
{
  struct t_impl_;
  using  p_impl_ = t_impl_*;

  struct t_logic {
    t_logic(sandbox::r_logic logic) : logic_{logic} {
    }

    // dacli api
    void do_3();

    // associated dacli callback
    virtual void callback_3() = 0;

  private:
    sandbox::r_logic logic_;
    p_impl_          impl_;
  };
  using p_logic = t_logic*;
}

struct my_logic : private sandbox::t_logic,
                  private datashare::t_logic,
                  private dacli::t_logic {
  // my_logic has access to all apis and provide all callbacks
  my_logic() : datashare::t_logic{static_cast<sandbox::t_logic&>(*this)},
               dacli::t_logic    {static_cast<sandbox::t_logic&>(*this)} {
  }
  virtual void callback_1() override {
    std::cout << "callback_1" << std::endl;
  }
  virtual void callback_2() override {
    std::cout << "callback_2" << std::endl;
  }
  virtual void callback_3() override {
    std::cout << "callback_3" << std::endl;
  }
};

int main() {
   my_logic logic;

  return 0;
}
