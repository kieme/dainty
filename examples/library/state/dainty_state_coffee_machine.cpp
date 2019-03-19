#include <assert.h>
#include "dainty_named_string.h"
#include "dainty_named_assert.h"
#include "dainty_state.h"

namespace coffee_machine
{
  using dainty::named::P_cstr;
  using dainty::named::t_prefix;
  using dainty::named::t_void;
  using dainty::named::t_bool;
  using dainty::named::r_bool;
  using dainty::named::t_int;
  using dainty::named::t_n_;
  using dainty::named::t_ix_;
  using dainty::named::string::FMT;
  using dainty::named::string::t_string;
  using dainty::named::string::t_crange;
  using dainty::named::string::mk_range;

  enum  t_tracker_tag_ {};
  using t_tracker = t_string<t_tracker_tag_>;
  using R_tracker = t_prefix<t_tracker>::R_;
  using r_tracker = t_prefix<t_tracker>::r_;

///////////////////////////////////////////////////////////////////////////////

  enum t_timeout_id : t_ix_ { TEST_TEMP = 0,
                              POUR_SMALL,
                              POUR_MEDIUM,
                              POUR_LARGE,
                              WAIT_EXPIRE  };

  const t_crange timeout_names[] = { mk_range("test_temp"),
                                     mk_range("pour small)"),
                                     mk_range("pour medium"),
                                     mk_range("pour large"),
                                     mk_range("inactive") };

///////////////////////////////////////////////////////////////////////////////

  enum t_selection_id : t_ix_ { SMALL = 0,
                                MEDIUM,
                                LARGE };

  const t_crange select_names[] = { mk_range("small"),
                                    mk_range("medium"),
                                    mk_range("large") };

///////////////////////////////////////////////////////////////////////////////

  enum t_state_id : t_ix_ { PREPARE = 0,
                            READY,
                            ORDER,
                            POUR,
                            CLEAN,
                            STOP };

  const t_crange state_names[] = { mk_range("prepare"),
                                   mk_range("ready"),
                                   mk_range("order"),
                                   mk_range("pour"),
                                   mk_range("clean"),
                                   mk_range("stop") };

///////////////////////////////////////////////////////////////////////////////

  enum t_coin_id : t_ix_ { CENT10 = 0,
                           CENT20,
                           CENT50,
                           EURO,
                           BADCOIN };

  const t_crange coin_names[] = { mk_range("10cent"),
                                  mk_range("20cent"),
                                  mk_range("50cent"),
                                  mk_range("1euro"),
                                  mk_range("uknown") };

///////////////////////////////////////////////////////////////////////////////

  enum e_cr { CR };
  enum e_bg { BG };

  inline r_tracker operator<<(r_tracker tracker, e_cr) {
    tracker.append("\n");
    return tracker;
  }

  inline r_tracker operator<<(r_tracker tracker, e_bg) {
    static t_n_ cnt = 0;
    tracker.append(FMT, "%03lu: ", ++cnt);
    return tracker;
  }

  inline r_tracker operator<<(r_tracker tracker, t_timeout_id id) {
    tracker.append(timeout_names[id]);
    return tracker;
  }

  inline r_tracker operator<<(r_tracker tracker, t_state_id id) {
    tracker.append(state_names[id]);
    return tracker;
  }

  inline r_tracker operator<<(r_tracker tracker, t_selection_id id) {
    tracker.append(select_names[id]);
    return tracker;
  }

  inline r_tracker operator<<(r_tracker tracker, t_coin_id id) {
    tracker.append(coin_names[id]);
    return tracker;
  }

  inline r_tracker operator<<(r_tracker tracker, t_crange string) {
    tracker.append(string);
    return tracker;
  }

  template<typename T>
  inline r_tracker operator<<(r_tracker tracker, const T& value) {
    tracker.append(value);
    return tracker;
  }

///////////////////////////////////////////////////////////////////////////////

  class t_events {
  public:
     virtual ~t_events() {}

     virtual t_state_id off    ()               noexcept = 0;
     virtual t_state_id timeout(t_timeout_id)   noexcept = 0;
     virtual t_state_id select (t_selection_id) noexcept = 0;
     virtual t_state_id insert (t_coin_id)      noexcept = 0;
     virtual t_state_id cancel ()               noexcept = 0;
  };

///////////////////////////////////////////////////////////////////////////////

  class t_actions {
  public:
    t_actions(r_bool temp_ok) : temp_ok_{temp_ok} {
    }

    t_void       rinse           ()               noexcept;
    t_void       start_pour      ()               noexcept;
    t_void       stop_pour       ()               noexcept;
    t_void       fill            ()               noexcept;
    t_void       display         (t_state_id)     noexcept;
    t_void       heatup          ()               noexcept;
    t_void       keep_heat       ()               noexcept;
    t_void       stop_heat       ()               noexcept;
    t_bool       accept_order    (t_selection_id) noexcept;
    t_void       keep_coin       (t_coin_id)      noexcept;
    t_bool       valid_coin      (t_coin_id)      noexcept;
    t_void       return_coin     (t_coin_id)      noexcept;
    t_void       return_change   ()               noexcept;
    t_void       return_coins    ()               noexcept;
    t_void       beep            ()               noexcept;
    t_void       power_off       ()               noexcept;
    t_bool       is_temp_above_75()               noexcept;
    t_void       start_timer     (t_timeout_id)   noexcept;
    t_void       stop_timer      (t_timeout_id)   noexcept;
    t_timeout_id pour_time       () const         noexcept;
    t_void       what_happened   () const         noexcept;

  public:
    mutable t_tracker tracker;

  private:
    r_bool       temp_ok_;
    t_n_         total_  = 0;
    t_timeout_id pour_time_;
  };
  using r_actions = t_prefix<t_actions>::r_;
  using R_actions = t_prefix<t_actions>::R_;

///////////////////////////////////////////////////////////////////////////////

  using t_traits_ = dainty::state::t_traits<t_state_id, t_actions, t_events>;
  using t_state_         = t_traits_::t_state;
  using t_statemachine_  = t_traits_::t_statemachine;
  using R_statemachine_  = t_prefix<t_statemachine_>::R_;

///////////////////////////////////////////////////////////////////////////////

  struct t_state : public t_state_ {
    t_state(t_state_id id, r_actions actions) noexcept : t_state_{id, actions} {
    }

    r_actions action()       noexcept { return get_user();  }
    R_actions action() const noexcept { return get_cuser(); }
  };

///////////////////////////////////////////////////////////////////////////////

  class t_prepare_state final : public t_state {
  public:
    t_prepare_state(r_actions actions) noexcept : t_state{PREPARE, actions} {
    }

  private:
    t_state_id entry_point() override {
      if (!action().is_temp_above_75()) {
        action().display(PREPARE);
        action().fill();
        action().heatup();
        action().start_timer(TEST_TEMP);
        return no_transition();
      }
      return request_transition(READY);
    }

    t_void exit_point() override {
    }

    t_state_id off() noexcept override {
      action().stop_timer(TEST_TEMP);
      return request_transition(CLEAN);
    }

    t_state_id timeout(t_timeout_id) noexcept override {
      if (action().is_temp_above_75()) {
        action().keep_heat();
        return request_transition(READY);
      }
      action().start_timer(TEST_TEMP);
      return no_transition();
    }

    t_state_id select(t_selection_id) noexcept override {
      action().beep();
      return no_transition();
    }

    t_state_id insert(t_coin_id id) noexcept override {
      action().beep();
      action().return_coin(id);
      return no_transition();
    }

    t_state_id cancel() noexcept override {
      action().beep();
      return no_transition();
    }
  };

///////////////////////////////////////////////////////////////////////////////

  class t_ready_state final : public t_state {
  public:
    t_ready_state(r_actions actions) noexcept : t_state{READY, actions} {
    }

  private:
    t_state_id entry_point() override {
      action().display(READY);
      return no_transition();
    }

    t_void exit_point() override {
    }

    t_state_id off() noexcept override {
      return request_transition(CLEAN);
    }

    t_state_id timeout(t_timeout_id) noexcept override {
      return no_transition();
    }

    t_state_id select (t_selection_id) noexcept override {
      action().beep();
      return no_transition();
    }

    t_state_id insert(t_coin_id id) noexcept override {
      if (action().valid_coin(id)) {
        action().keep_coin(id);
        return request_transition(ORDER);
      }
      action().beep();
      action().return_coin(id);
      return no_transition();
    }

     t_state_id cancel() noexcept override {
       return no_transition();
    }
  };

///////////////////////////////////////////////////////////////////////////////

  class t_order_state final : public t_state {
  public:
    t_order_state(r_actions actions) noexcept : t_state{ORDER, actions} {
    }

  private:
    t_state_id entry_point() override {
      action().display(ORDER);
      action().start_timer(WAIT_EXPIRE);
      return no_transition();
    }

    t_void exit_point() override {
      action().stop_timer(WAIT_EXPIRE);
    }

    t_state_id off() noexcept override {
      action().return_coins();
      return request_transition(CLEAN);
    }

    t_state_id timeout(t_timeout_id) noexcept override {
      action().return_coins();
      return request_transition(READY);
    }

    t_state_id select(t_selection_id id) noexcept override {
      if (action().accept_order(id)) {
        action().return_change();
        return request_transition(POUR);
      }
      action().beep();
      return no_transition();
    }

    t_state_id insert(t_coin_id id) noexcept override {
      if (action().valid_coin(id)) {
        action().keep_coin(id);
        action().display(ORDER);
      } else {
        action().beep();
        action().return_coin(id);
      }
      return no_transition();
    }

    t_state_id cancel() noexcept override {
      action().return_coins();
      return request_transition(READY);
    }
  };

///////////////////////////////////////////////////////////////////////////////

  class t_pour_state final : public t_state {
  public:
    t_pour_state(r_actions actions) noexcept : t_state{POUR, actions} {
    }

  private:
    t_state_id entry_point() override {
      action().display(POUR);
      action().start_timer(action().pour_time());
      action().start_pour();
      return no_transition();
    }

    t_void exit_point() override {
      action().stop_pour();
      action().stop_timer(action().pour_time());
    }

    t_state_id off() noexcept override {
      return request_transition(CLEAN);
    }

    t_state_id timeout(t_timeout_id) noexcept override {
      return request_transition(PREPARE);
    }

    t_state_id select(t_selection_id) noexcept override {
      action().beep();
      return no_transition();
    }

    t_state_id insert(t_coin_id id) noexcept override {
      action().beep();
      action().return_coin(id);
      return no_transition();
    }

    t_state_id cancel() noexcept override {
      return request_transition(PREPARE);
    }
  };

///////////////////////////////////////////////////////////////////////////////

  class t_clean_state final : public t_state {
  public:
     t_clean_state(r_actions actions) : t_state(CLEAN, actions) {
     }

  private:
     t_state_id entry_point() override {
       action().display(CLEAN);
       action().stop_heat();
       action().rinse();
       return request_transition(STOP);
     }

     t_void exit_point() override {
       /* unexpected */
       action().beep();
       action().beep();
       action().beep();
       action().beep();
       action().beep();
     }

     t_state_id off() noexcept override {
       return no_transition();
     }

     t_state_id timeout(t_timeout_id) noexcept override {
       return no_transition();
     }

     t_state_id select (t_selection_id) noexcept override {
       action().beep();
       return no_transition();
     }

     t_state_id insert(t_coin_id id) noexcept override {
       action().beep();
       action().return_coin(id);
       return no_transition();
     }

     t_state_id cancel() noexcept override {
       return no_transition();
     }
  };

///////////////////////////////////////////////////////////////////////////////

  class t_statemachine final : public t_statemachine_ {
  public:
    t_statemachine(r_actions actions) noexcept
      : t_statemachine_{STOP, actions},
        prepare_       {actions},
        ready_         {actions},
        order_         {actions},
        pour_          {actions},
        clean_         {actions} {
      states_[PREPARE] = &prepare_;
      states_[READY]   = &ready_;
      states_[ORDER]   = &order_;
      states_[POUR]    = &pour_;
      states_[CLEAN]   = &clean_;

      auto stateid = start(PREPARE);
      assert(stateid == READY || stateid == PREPARE);
    }

    ~t_statemachine() {
      stop();
    } // never returns from stop

    t_state_id off() noexcept override {
      return do_transition(get_current()->off());
    } // never returns

    t_state_id timeout(t_timeout_id id) noexcept override {
      return do_transition(get_current()->timeout(id));
    }

    t_state_id select(t_selection_id id) noexcept override {
      return do_transition(get_current()->select(id));
    }

    t_state_id insert(t_coin_id id) noexcept override {
      return do_transition(get_current()->insert(id));
    }

    t_state_id cancel() noexcept override {
      return do_transition(get_current()->cancel());
    }

  private:
    r_actions action()       noexcept { return get_user();  }
    R_actions action() const noexcept { return get_cuser(); }

    t_state_id initial_point(t_state_id id) override {
      action().rinse();
      return id;
    }

    t_void final_point() override {
      action().power_off();
    }

    p_state get_state(t_state_id s) noexcept override {
      return states_[s];
    }

    P_state get_state(t_state_id s) const noexcept override {
      return states_[s];
    }

    t_prepare_state prepare_;
    t_ready_state   ready_;
    t_order_state   order_;
    t_pour_state    pour_;
    t_clean_state   clean_;
    p_state         states_[STOP];
  };

///////////////////////////////////////////////////////////////////////////////

#ifdef COFFEE_DEBUG
  t_void debug(R_statemachine_ sm, t_state_id current, t_state_id next) {
    sm.get_user().tracker << BG << "statemachine transition from "
                                << current << " to " << next << CR;
  }

  t_void debug_start(R_statemachine_ sm, t_state_id start) {
    sm.get_user().tracker << BG << "statemachine started in " << start << CR;
  }

  t_void debug_stop(R_statemachine_ sm) {
    sm.get_user().tracker << BG << "statemachine stopped" << CR;
  }
#endif

///////////////////////////////////////////////////////////////////////////////

  class t_coffee_machine {
  public:
    t_coffee_machine(r_bool temp_ok) noexcept
      : actions_{temp_ok}, statemachine_{actions_} {
    }

    t_void off() noexcept {
      statemachine_.off();
      actions_.what_happened();
    }

    t_void timeout(t_timeout_id id) noexcept {
      statemachine_.timeout(id);
      actions_.what_happened();
    }

    t_void select(t_selection_id id) noexcept {
      statemachine_.select(id);
      actions_.what_happened();
    }

    t_void insert(t_coin_id id) noexcept {
      statemachine_.insert(id);
      actions_.what_happened();
    }

    t_void cancel() noexcept {
      statemachine_.cancel();
      actions_.what_happened();
    }

  private:
    t_actions      actions_;
    t_statemachine statemachine_;
  };
}

///////////////////////////////////////////////////////////////////////////////

using namespace coffee_machine;

t_int main() {
  t_bool temp_ok = false;            // water temp is below 75 deg

  t_coffee_machine coffee_machine(temp_ok);

  coffee_machine.timeout(TEST_TEMP); // simulate internal event
  coffee_machine.timeout(TEST_TEMP); // simulate internal event
  coffee_machine.timeout(TEST_TEMP); // simulate internal event

  temp_ok = true;                    // water temp is 75 deg

  coffee_machine.timeout(TEST_TEMP); // simulate internal event

  coffee_machine.insert (CENT20);
  coffee_machine.insert (CENT10);

  coffee_machine.select (SMALL);

  coffee_machine.insert (CENT50);
  coffee_machine.select (SMALL);

  coffee_machine.timeout(POUR_SMALL); // simulate internal event

  coffee_machine.off();
  return 0;
}

///////////////////////////////////////////////////////////////////////////////

namespace coffee_machine
{
  using namespace dainty::named;

///////////////////////////////////////////////////////////////////////////////

  t_void t_actions::rinse() noexcept {
    tracker << BG << "rinse" << CR;
  }

  t_void t_actions::start_pour() noexcept {
    tracker << BG << "pour" << CR;
  }

  t_void t_actions::stop_pour() noexcept {
    tracker << BG << "stop_pour" << CR;
  }

  t_void t_actions::fill() noexcept {
    tracker << BG << "fill(beans&water)" << CR;
  }

  t_void t_actions::heatup() noexcept {
    tracker << BG << "heatup" << CR;
  }

  t_void t_actions::keep_heat() noexcept {
    tracker << BG << "keep_heat" << CR;
  }

  t_void t_actions::stop_heat() noexcept {
    tracker << BG << "stop_heat" << CR;
  }

  t_void t_actions::keep_coin(t_coin_id id) noexcept {
    assert(id != BADCOIN);
    switch (id) {
      case CENT10:  total_ += 10;  break;
      case CENT20:  total_ += 20;  break;
      case CENT50:  total_ += 50;  break;
      case EURO:    total_ += 100; break;
      case BADCOIN:                break; // never happed due to assert
    }
    tracker << BG;
    tracker.append(FMT, "keep_coin, total is now = %lu\n", total_);
  }

  t_bool t_actions::valid_coin(t_coin_id id) noexcept {
    tracker << BG << "valid_coin=" << id << CR;
    return id != BADCOIN;
  }

  t_void t_actions::return_coin(t_coin_id id) noexcept {
    tracker << BG << "return coin=" << id << CR;
  }

  t_void t_actions::return_change() noexcept {
    tracker << BG;
    tracker.append(FMT,"return change in total of = %lu sent\n", total_);
    total_ = 0;
  }

  t_void t_actions::return_coins() noexcept {
    tracker << BG;
    tracker.append(FMT," return given coins in total of = %lu sent\n", total_);
    total_ = 0;
  }

  t_void t_actions::beep() noexcept {
    tracker << BG << "beep" << CR;
  }

  t_void t_actions::power_off() noexcept {
    tracker << BG << "poweroff" << CR;
  }

  t_bool t_actions::is_temp_above_75() noexcept {
    tracker << BG << "test temp" << CR;
    return temp_ok_;
  }

  t_void t_actions::start_timer(t_timeout_id id) noexcept {
    tracker << BG << "start_timer=" << id << CR;
  }

  t_void t_actions::stop_timer(t_timeout_id id) noexcept {
    tracker << BG << "stop_timer=" << id << CR;
  }

  t_timeout_id t_actions::pour_time() const noexcept {
    tracker << BG << "fetch_pour_time" << CR;
    return pour_time_;
  }

  t_void t_actions::display(t_state_id id) noexcept {
    switch (id) {
      case ORDER:
        tracker << BG << "display" << id;
        tracker.append(FMT," total = %lu\n", total_);
        break;

      default:
        tracker << BG << "display=" << id << CR;
        break;
    }
  }

  t_bool t_actions::accept_order(t_selection_id id) noexcept {
    tracker << BG << "selected " << id << CR;
    t_bool enough = false;
    switch (id) {
      case SMALL: if (total_ >= 50) {
                    tracker << BG << "accept order small" << CR;
                    total_    -= 50;
                    pour_time_ = POUR_SMALL;
                    enough     = true;
                  }
                  break;
      case MEDIUM: if (total_ >= 100) {
                    tracker << BG << "accept order medium" << CR;
                    total_    -= 100;
                    pour_time_ = POUR_MEDIUM;
                    enough     = true;
                  }
                  break;
      case LARGE: if (total_ >= 150) {
                    tracker << BG << "accept order large" << CR;
                    total_    -= 150;
                    pour_time_ = POUR_LARGE;
                    enough     = true;
                  }
                  break;
    }
    if (!enough)
      tracker << BG << "insufficient funds" << CR;
    return enough;
  }

  t_void t_actions::what_happened() const noexcept {
    tracker.display_then_clear();
  }
}

/////////////////////////////////////////////////////////////////////////////////////////

