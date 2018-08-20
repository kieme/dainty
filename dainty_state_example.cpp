#include <assert.h>
#include <iostream>
#include "dainty_state.h"

namespace coffee_vending_machine
{
  /////////////////////////////////////////////////////////////////////////////////////////

  enum timeout_id { POUR_SMALL_T_, POUR_MEDIUM_T_, POUR_LARGE_T_, TEST_TEMP_T_, EXPIRE_T_  };
  enum select_id  { SMALL_, MEDIUM_, LARGE_ };
  enum coin_id    { CENT10, CENT20, CENT50, EURO, BADCOIN_ };
  enum state_id   { PREPARE_, READY_, ORDER_, POUR_, CLEAN_, STOP_ };

  /////////////////////////////////////////////////////////////////////////////////////////

  const int        coin_value[]   = { 10, 20, 50, 100, 0 };

  /////////////////////////////////////////////////////////////////////////////////////////

  using text = std::ostream;

  inline text& operator,(text& txt, timeout_id id) {
    const char* names[] = { "test_temp", "pour small", "pour medium", "pour large", "inactive" };
    txt << names[id];
    return txt;
  }

  inline text& operator,(text& txt, state_id id) {
    const char* names[] = { "prepare", "ready", "order", "pour", "clean", "stop" };
    txt << names[id];
    return txt;
  }

  inline text& operator,(text& txt, select_id id) {
    const char* names[] = { "small", "medium", "large" };
    txt << names[id];
    return txt;
  }

  inline text& operator,(text& txt, coin_id id) {
    const char* names[] = { "10cent", "20cent", "50cent", "1uero", "uknown" };
    txt << names[id];
    return txt;
  }

  enum e_cr { CR };
  inline e_cr cr() { return CR; }

  inline text& operator,(text& txt, e_cr) {
    txt << std::endl;
    return txt;
  }

  template<typename T>
  inline text& operator,(text& txt, const T& value) {
    txt << value;
    return txt;
  }

  /////////////////////////////////////////////////////////////////////////////////////////

  class events {
  public:
     virtual ~events() { }
     virtual state_id off()               = 0;
     virtual state_id timeout(timeout_id) = 0;
     virtual state_id select (select_id)  = 0;
     virtual state_id insert (coin_id)    = 0;
     virtual state_id cancel ()           = 0;
  };

  /////////////////////////////////////////////////////////////////////////////////////////

  class actions {
  public:
    actions() : total_(0) { }

    void rinse           ()              { std::cout << "rinse",                  cr(); }
    void pour            ()              { std::cout << "pour",                   cr(); }
    void stop_pour       ()              { std::cout << "stop_pour",              cr(); }
    void fill            ()              { std::cout << "fill(beans&water)",      cr(); }
    void display         (state_id);
    void heatup          ()              { std::cout << "heatup",                 cr(); }
    void keep_heat       ()              { std::cout << "keep_heat",              cr(); }
    void stop_heat       ()              { std::cout << "stop_heat",              cr(); }
    bool accept_order    (select_id);
    void keep_coin       (coin_id id)    { std::cout << "keep_coin=",     id,     cr(); total_ += coin_value[id]; }
    bool valid_coin      (coin_id id)    { std::cout << "valid_coin=",    id,     cr(); return id != BADCOIN_; }
    void return_coin     (coin_id id)    { std::cout << "return coin=",   id,     cr(); }
    void return_change   ()              { std::cout << "return_change=", total_, cr(); total_ = 0; }
    void return_coins    ()              { std::cout << "return_coins=",  total_, cr(); total_ = 0; }
    void beep            ()              { std::cout << "beep",                   cr(); }
    void power_off       ()              { std::cout << "poweroff",               cr(); }
    bool is_temp_above_75()              { std::cout << "test temp",              cr(); return true; }
    void start_timer     (timeout_id id) { std::cout << "start_timer=",   id,     cr(); }
    void stop_timer      (timeout_id id) { std::cout << "stop_timer=",    id,     cr(); }

    timeout_id pour_time    () const { return pour_time_; }
    void       what_happened();

  private:
    int        total_;
    timeout_id pour_time_;
  };

  /////////////////////////////////////////////////////////////////////////////////////////

  using state_ = dainty::state::t_state<state_id, actions, events>;

  struct state : public state_ {
    inline state(state_id id, actions& oper) : state_(id, oper) { }

    inline       actions& action()          { return get_user();  }
    inline const actions& action() const    { return get_cuser(); }
  };

  /////////////////////////////////////////////////////////////////////////////////////////

  class prepare_state : public state {
  public:
     prepare_state(actions& oper) : state(PREPARE_, oper) { }

  private:
     t_sid entry_point()           { if (!action().is_temp_above_75()) {
                                       action().display(PREPARE_);
                                       action().fill();
                                       action().heatup();
                                       action().start_timer(TEST_TEMP_T_);
                                       return no_transition();
                                     }
                                     return request_transition(READY_);
                                   }

     void  exit_point()            { }

     state_id off()                { action().stop_timer(TEST_TEMP_T_);
                                     return request_transition(CLEAN_);
                                   }

     state_id timeout(timeout_id)  { if (action().is_temp_above_75()) {
                                       action().keep_heat();
                                       return request_transition(READY_);
                                     }
                                     action().start_timer(TEST_TEMP_T_);
                                     return no_transition();
                                   }

     state_id select (select_id)   { action().beep();
                                     return no_transition();
                                   }

     state_id insert (coin_id id)  { action().beep();
                                     action().return_coin(id);
                                     return no_transition();
                                   }

     state_id cancel ()            { action().beep();
                                     return no_transition();
                                   }
  };

  /////////////////////////////////////////////////////////////////////////////////////////

  class ready_state : public state {
  public:
     ready_state(actions& oper) : state(READY_, oper) { }

  private:
     t_sid entry_point()           { action().display(READY_);
                                     return no_transition();
                                   }

     void  exit_point()            { }

     state_id off()                { return request_transition(CLEAN_); }

     state_id timeout(timeout_id)  { return no_transition(); }

     state_id select (select_id)   { action().beep();
                                     return no_transition();
                                   }

     state_id insert (coin_id id)  { if (action().valid_coin(id)) {
                                       action().keep_coin(id);
                                       return request_transition(ORDER_);
                                     }
                                     action().beep();
                                     action().return_coin(id);
                                     return no_transition();
                                   }

     state_id cancel ()            { return no_transition(); }
  };

  /////////////////////////////////////////////////////////////////////////////////////////

  class order_state : public state {
  public:
     order_state(actions& oper) : state(ORDER_, oper) { }

  private:
     t_sid entry_point()            { action().display(ORDER_);
                                      action().start_timer(EXPIRE_T_);
                                      return no_transition();
                                    }

     void  exit_point()             { action().stop_timer(EXPIRE_T_); }

     state_id off()                 { action().return_coins();
                                      return request_transition(CLEAN_);
                                    }

     state_id timeout(timeout_id)   { action().return_coins();
                                      return request_transition(READY_);
                                    }

     state_id select (select_id id) { if (action().accept_order(id)) {
                                        action().return_change();
                                        return request_transition(POUR_);
                                      }
                                      action().beep();
                                      return no_transition();
                                    }

     state_id insert (coin_id id)   { if (action().valid_coin(id)) {
                                        action().keep_coin(id);
                                        action().display(ORDER_);
                                      } else {
                                        action().beep();
                                        action().return_coin(id);
                                      }
                                      return no_transition();
                                    }

     state_id cancel ()             { action().return_coins();
                                      return request_transition(READY_);
                                    }
  };

  /////////////////////////////////////////////////////////////////////////////////////////

  class pour_state : public state {
  public:
     pour_state(actions& oper) : state(POUR_, oper) { }

  private:
     t_sid entry_point()          { action().display(POUR_);
                                    action().start_timer(action().pour_time());
                                    action().pour();
                                    return no_transition();
                                  }

     void  exit_point()           { action().stop_pour();
                                    action().stop_timer(action().pour_time());
                                  }

     state_id off()               { return request_transition(CLEAN_);   }

     state_id timeout(timeout_id) { return request_transition(PREPARE_); }

     state_id select (select_id)  { action().beep();
                                    return no_transition();
                                  }

     state_id insert (coin_id id) { action().beep();
                                    action().return_coin(id);
                                    return no_transition();
                                  }

     state_id cancel ()           { return request_transition(PREPARE_); }
  };

  /////////////////////////////////////////////////////////////////////////////////////////

  class clean_state : public state {
  public:
     clean_state(actions& oper) : state(CLEAN_, oper) { }

  private:
     t_sid entry_point()           { action().display(CLEAN_);
                                     action().stop_heat();
                                     action().rinse();
                                     return request_transition(STOP_);
                                   }

     void  exit_point()            { /* unexpected */ }

     state_id off()                { return no_transition(); }
     state_id timeout(timeout_id)  { return no_transition(); }

     state_id select (select_id)   { action().beep();
                                     return no_transition(); }

     state_id insert (coin_id id)  { action().beep();
                                     action().return_coin(id);
                                     return no_transition(); }
     state_id cancel ()            { return no_transition(); }
  };

  /////////////////////////////////////////////////////////////////////////////////////////

  using statemachine_ = dainty::state::t_statemachine<state_id, actions, events>;

  class device_sm : public statemachine_ {
  public:
      device_sm(actions& oper) : statemachine_(STOP_, oper),
                                    prepare_(oper),
                                    ready_  (oper),
                                    order_  (oper),
                                    pour_   (oper),
                                    clean_  (oper) {
       states_[PREPARE_] = &prepare_;
       states_[READY_]   = &ready_;
       states_[ORDER_]   = &order_;
       states_[POUR_]    = &pour_;
       states_[CLEAN_]   = &clean_;

       assert(start(PREPARE_) == READY_);
     }

     ~device_sm() { stop(); } // never returns from stop

     state_id off()                  { return do_transition(get_current()->off());        } // never returns
     state_id timeout(timeout_id id) { return do_transition(get_current()->timeout(id));  }
     state_id select (select_id  id) { return do_transition(get_current()->select(id));   }
     state_id insert (coin_id    id) { return do_transition(get_current()->insert(id));   }
     state_id cancel ()              { return do_transition(get_current()->cancel());     }

  private:
    state_id initial_point(state_id id)                  { get_user().rinse(); return id; }
    void     final_point  ()                             { get_user().power_off();        }

    p_state get_state(t_sid s)                                       { return states_[s]; }
    P_state get_state(t_sid s) const                                 { return states_[s]; }

    prepare_state prepare_;
    ready_state   ready_;
    order_state   order_;
    pour_state    pour_;
    clean_state   clean_;
    state_*       states_[STOP_];
  };

  /////////////////////////////////////////////////////////////////////////////////////////

#ifdef COFFEE_DEBUG
  void debug(const statemachine_* sm, state_id current, state_id next)
  {
    std::cout <<  "sm(", sm, ") transition: ", current, " -> ", next, cr();
  }

  void debug_start(const statemachine_* sm, state_id start)
  {
    std::cout <<  "sm(", sm, ") started with = ", start, cr();
  }

  void debug_stop(const statemachine_* sm)
  {
    std::cout <<  "sm(", sm, ") stopped", cr();
  }
#endif

  /////////////////////////////////////////////////////////////////////////////////////////

  class device {
  public:
     device() : actions_(), sm_(actions_)   { }

     void off    ()              { sm_.off();       actions_.what_happened(); }
     void timeout(timeout_id id) { sm_.timeout(id); actions_.what_happened(); }
     void select (select_id  id) { sm_.select(id);  actions_.what_happened(); }
     void insert (coin_id    id) { sm_.insert(id);  actions_.what_happened(); }
     void cancel ()              { sm_.cancel();    actions_.what_happened(); }

  private:
    actions    actions_;
    device_sm  sm_;
  };
}

/////////////////////////////////////////////////////////////////////////////////////////

using namespace coffee_vending_machine;

int main()
{
  device dev;

  //dev.timeout(TEST_TEMP_T_);
  dev.insert (CENT20);
  dev.insert (CENT10);

  dev.select (SMALL_);

  dev.insert (CENT50);
  dev.select (SMALL_);
  dev.timeout(POUR_SMALL_T_);

  dev.off();

  return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////

namespace coffee_vending_machine
{
  void actions::display(state_id id) {
    switch (id) {
      case ORDER_:
        std::cout << "display=", id, " total=", total_, cr();
        break;

      default:
        std::cout << "display=", id, cr();
        break;
    }
  }

  bool actions::accept_order(select_id id) {
    std::cout << "select=", id, cr();
    bool enough = false;
    switch (id) {
      case SMALL_: if (total_ >= 50) {
                     std::cout << "accept order small", cr();
                     total_    -= 50;
                     pour_time_ = POUR_SMALL_T_;
                     enough     = true;
                   }
                   break;
      case MEDIUM_: if (total_ >= 100) {
                     std::cout << "accept order medium", cr();
                     total_    -= 100;
                     pour_time_ = POUR_MEDIUM_T_;
                     enough     = true;
                   }
                   break;
      case LARGE_: if (total_ >= 150) {
                     std::cout << "accept order large", cr();
                     total_    -= 150;
                     pour_time_ = POUR_LARGE_T_;
                     enough     = true;
                   }
                   break;
    }
    if (!enough)
      std::cout << "insufficient funds", cr();
    return enough;
  }

  void actions::what_happened() {
  }
}

/////////////////////////////////////////////////////////////////////////////////////////

