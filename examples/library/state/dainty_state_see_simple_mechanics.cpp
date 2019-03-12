#include <iostream>
#include <assert.h>

#include "dainty_state.h"

using namespace dainty;
using named::t_void;

////////////////////////// design phase ////////////////////////////////

// which states are required
enum t_states { STATE1 = 0, STATE2, STATE3, STATE4, STOP };

// what data must be shared among them and the statemachine
struct t_data { };

// what interface of functions are required and must be implemented
struct t_interface {
  virtual ~t_interface() { }
  virtual t_states foo() = 0;
};

//////////////// choose statemachine framework  ////////////////////////

using t_traits       = state::t_traits<t_states, t_data, t_interface>;
using t_state        = t_traits::t_state;
using t_statemachine = t_traits::t_statemachine;

//////////////////// implementation phase //////////////////////////////

struct t_state1 final : t_state {
  t_state1(t_data& d) : t_state(STATE1, d) { }
  virtual t_sid  entry_point() override;
  virtual t_void exit_point()  override;
  virtual t_sid  foo()         override;
};

struct t_state2 final : t_state {
  t_state2(t_data& d) : t_state(STATE2, d) { }
  virtual t_sid  entry_point() override;
  virtual t_void exit_point()  override;
  virtual t_sid  foo()         override;
};

struct t_state3 final : t_state {
  t_state3(t_data& d) : t_state(STATE3, d) { }
  virtual t_sid  entry_point() override;
  virtual t_void exit_point()  override;
  virtual t_sid  foo()         override;
};

struct t_state4 final : t_state {
  t_state4(t_data& d) : t_state(STATE4, d) { }
  virtual t_sid  entry_point() override;
  virtual t_void exit_point()  override;
  virtual t_sid  foo()         override;
};

struct t_sm : t_statemachine {
  t_sm(t_data& d) : t_statemachine(STOP, d), s1_(d), s2_(d), s3_(d), s4_(d) {
    states_[STATE1] = &s1_;
    states_[STATE2] = &s2_;
    states_[STATE3] = &s3_;
    states_[STATE4] = &s4_;
    assert(start(STATE1) == STATE1);
  }

  ~t_sm()        { stop(); }

  t_void reset() { assert(restart(STATE1) == STATE1); }

  virtual t_states foo          ()              override {
    return do_transition(get_current()->foo());
  }

  virtual t_states initial_point(t_states s)    override;
  virtual t_void   final_point  ()              override;

  virtual p_state  get_state    (t_sid i)       override { return states_[i]; }
  virtual P_state  get_state    (t_sid i) const override { return states_[i]; }

  t_state1 s1_;
  t_state2 s2_;
  t_state3 s3_;
  t_state4 s4_;
  t_state* states_[STOP];
};

// function uses the statemachine
class t_function {
public:
  t_function() : sm_(data_)   { }

  t_void foo()   { sm_.foo();   }
  t_void reset() { sm_.reset(); }

private:
  t_data data_;
  t_sm   sm_;
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

t_state1::t_sid t_state1::entry_point() {
  std::cout << "state1::entry_point" << std::endl;
  return no_transition();
}

t_state1::t_void t_state1::exit_point() {
  std::cout << "state1::exit_point"  << std::endl;
}

t_state1::t_sid t_state1::foo() {
  std::cout << "state1::foo" << std::endl;
  return request_transition(STATE2);
}

t_state2::t_sid t_state2::entry_point() {
  std::cout << "state2::entry_point" << std::endl;
  return no_transition();
}

t_state2::t_void t_state2::exit_point() {
  std::cout << "state2::exit_point"  << std::endl;
}

t_state2::t_sid t_state2::foo() {
  std::cout << "state2::foo" << std::endl;
  return request_transition(STATE3);
}

t_state3::t_sid t_state3::entry_point() {
  std::cout << "state3::entry_point" << std::endl;
  return no_transition();
}

t_state3::t_void t_state3::exit_point() {
  std::cout << "state3::exit_point" << std::endl;
}

t_state3::t_sid t_state3::foo() {
  std::cout << "state3::foo" << std::endl;
  return request_transition(STATE4);
}

t_state4::t_sid t_state4::entry_point() {
  std::cout << "state4::entry_point" << std::endl;
  return no_transition();
}

t_state4::t_void t_state4::exit_point() {
  std::cout << "state4::exit_point" << std::endl;
}

t_state4::t_sid t_state4::foo() {
  std::cout << "state4::foo" << std::endl;
  return request_transition(STATE1);
}

t_states t_sm::initial_point(t_states s) {
  std::cout << "initial action" << std::endl;
  return s;
}

t_sm::t_void t_sm::final_point() {
  std::cout << "final action" << std::endl;
}

///////////////////////////////////////////////////////////////////////////////
