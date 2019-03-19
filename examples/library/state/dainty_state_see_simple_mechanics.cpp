#include <iostream>
#include <assert.h>

#include "dainty_state.h"

using dainty::named::t_void;
using dainty::named::t_prefix;

////////////////////////// design phase ////////////////////////////////

// which states are required
enum t_state_id { STATE1 = 0, STATE2, STATE3, STATE4, STOP };

// what data must be shared among them and the statemachine
struct t_data { };
using  r_data = t_prefix<t_data>::r_;

// what interface of functions are required and must be implemented
struct t_interface {
  virtual ~t_interface() { }
  virtual t_state_id foo() = 0;
};

//////////////// choose statemachine framework  ////////////////////////

using t_traits_       = dainty::state::t_traits<t_state_id, t_data, t_interface>;
using t_state_        = t_traits_::t_state;
using t_statemachine_ = t_traits_::t_statemachine;

//////////////////// implementation phase //////////////////////////////

struct t_state1 final : t_state_ {
  t_state1(r_data data) : t_state_{STATE1, data} { }

  t_state_id entry_point() override;
  t_void     exit_point () override;
  t_state_id foo        () override;
};

struct t_state2 final : t_state_ {
  t_state2(r_data data) : t_state_{STATE2, data} { }

  t_state_id entry_point() override;
  t_void     exit_point () override;
  t_state_id foo        () override;
};

struct t_state3 final : t_state_ {
  t_state3(r_data data) : t_state_{STATE3, data} { }
  t_state_id entry_point() override;
  t_void     exit_point () override;
  t_state_id foo        () override;
};

struct t_state4 final : t_state_ {
  t_state4(r_data data) : t_state_{STATE4, data} { }
  t_state_id entry_point() override;
  t_void     exit_point () override;
  t_state_id foo        () override;
};

class t_statemachine final : public t_statemachine_ {
public:
  t_statemachine(r_data data)
    : t_statemachine_{STOP, data}, s1_{data}, s2_{data}, s3_{data}, s4_{data} {
    states_[STATE1] = &s1_;
    states_[STATE2] = &s2_;
    states_[STATE3] = &s3_;
    states_[STATE4] = &s4_;

    assert(start(STATE1) == STATE1);
  }

 ~t_statemachine() {
    stop();
  }

  t_void restart() {
    assert(t_statemachine_::restart(STATE1) == STATE1);
  }

  t_state_id foo() override {
    return do_transition(get_current()->foo());
  }

private:
  t_state_id initial_point(t_state_id) override;
  t_void   final_point  ()             override;

  p_state get_state(t_state_id id)       noexcept override { return states_[id]; }
  P_state get_state(t_state_id id) const noexcept override { return states_[id]; }

  t_state1 s1_;
  t_state2 s2_;
  t_state3 s3_;
  t_state4 s4_;
  p_state states_[STOP];
};

// function uses the statemachine
class t_function {
public:
  t_function() : statemachine_(data_)     { }

  t_void foo()   { statemachine_.foo();     }
  t_void reset() { statemachine_.restart(); }

private:
  t_data         data_;
  t_statemachine statemachine_;
};

// execution
int main() {
  t_function f;
  f.foo();
  f.foo();
  f.reset();
  f.foo();
  f.foo();

  return 0;
}

///////////////////////////////////////////////////////////////////////////////

t_state1::t_state_id t_state1::entry_point() {
  std::cout << "state1::entry_point" << std::endl;
  return no_transition();
}

t_state1::t_void t_state1::exit_point() {
  std::cout << "state1::exit_point"  << std::endl;
}

t_state1::t_state_id t_state1::foo() {
  std::cout << "state1::foo" << std::endl;
  return request_transition(STATE2);
}

t_state2::t_state_id t_state2::entry_point() {
  std::cout << "state2::entry_point" << std::endl;
  return no_transition();
}

t_state2::t_void t_state2::exit_point() {
  std::cout << "state2::exit_point"  << std::endl;
}

t_state2::t_state_id t_state2::foo() {
  std::cout << "state2::foo" << std::endl;
  return request_transition(STATE3);
}

t_state3::t_state_id t_state3::entry_point() {
  std::cout << "state3::entry_point" << std::endl;
  return no_transition();
}

t_state3::t_void t_state3::exit_point() {
  std::cout << "state3::exit_point" << std::endl;
}

t_state3::t_state_id t_state3::foo() {
  std::cout << "state3::foo" << std::endl;
  return request_transition(STATE4);
}

t_state4::t_state_id t_state4::entry_point() {
  std::cout << "state4::entry_point" << std::endl;
  return no_transition();
}

t_state4::t_void t_state4::exit_point() {
  std::cout << "state4::exit_point" << std::endl;
}

t_state4::t_state_id t_state4::foo() {
  std::cout << "state4::foo" << std::endl;
  return request_transition(STATE1);
}

t_state_id t_statemachine::initial_point(t_state_id id) {
  std::cout << "initial action" << std::endl;
  return id;
}

t_void t_statemachine::final_point() {
  std::cout << "final action" << std::endl;
}

///////////////////////////////////////////////////////////////////////////////
