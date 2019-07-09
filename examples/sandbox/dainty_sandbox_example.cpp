#include "dainty_base_terminal.h"
#include "dainty_sandbox.h"

using t_err = dainty::oops::t_oops<>;
using dainty::base::t_int;
using dainty::base::string::mk_range;
using dainty::base::string::t_crange;
using namespace dainty::sandbox;

class juggle : public t_logic {
public:
  juggle(t_err err, t_crange name, t_crange other) : t_logic{err, name, {}} {
    //add_monitor(err, other);
  }

  virtual t_void process_wakeup(t_time elapsed, t_bool maxwait) override {
  }

  virtual t_void process_notify(t_messenger_state,
                                  R_messenger_name,
                                  R_messenger_key,
                                  t_messenger_prio,
                                  t_messenger_user) override {
  }

  virtual t_void process_user(x_message) override {
  }

  virtual t_void process_bad(x_message) override {
  }

  virtual t_void process_timeout(t_bool periodic,
                                 t_multiple_of_100ms,
                                 R_messenger_key,
                                 t_messenger_prio,
                                 t_messenger_user) override {
  }

  virtual t_void process_event(t_fd, R_label, t_messenger_user) override {
  }

private:
  t_messenger_key key_ = t_messenger_key{0};
};

t_int main() {
  t_err err;
  {
    start(err);

    auto juggle1 = mk_range("juggle1");
    auto juggle2 = mk_range("juggle2");
    auto juggle3 = mk_range("juggle3");
    auto juggle4 = mk_range("juggle4");

    t_sandbox sb1 {err, mk_range("sb1"),  new juggle{err, juggle1, juggle2}};
    t_sandbox sb2 {err, mk_range("sb2"),  new juggle{err, juggle2, juggle3}};
    t_sandbox sb3 {err, mk_range("sb3"),  new juggle{err, juggle3, juggle4}};
    t_main    main{err, mk_range("main"), new juggle{err, juggle4, juggle1}};
  }
  sleep(2);
  return 0;
}
