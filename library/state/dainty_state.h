/******************************************************************************

 MIT License

 Copyright (c) 2018 kieme, frits.germs@gmx.net

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

#ifndef _DAINTY_STATE_
#define _DAINTY_STATE_

#include "dainty_base.h"

namespace dainty
{
namespace state
{
  //
  // state and statemachine works together to form a framework for building
  // statemachines.
  //
  // classes:
  //
  //   state        -> represents a specific state
  //   statemachine -> consists of states and manage their changes.
  //
  //   each class have four specializations:
  //
  //     1. accept a user reference and specify an interface.
  //     2. accept a user reference and no interface is enforced.
  //     3. has no user reference   and specify an interface.
  //     4. has no user reference   and no interface is enforced.
  //
  //  The user reference can be:
  //
  //    + shared data managed and shared between the states.
  //    + functionality to call to do work from the state.
  //
  //  With these four specializations any type of statemaschine can be
  //  constructed.
  //
  //  A template statemashine has been written that can be copied and pasted
  //  as a starting point and then be modified.

  using base::types::t_prefix;
  using base::types::t_void;

///////////////////////////////////////////////////////////////////////////////

  enum t_no_user { };
  enum t_no_if   { };

///////////////////////////////////////////////////////////////////////////////

  template<typename ID,   // enum type defining states
           typename USER, // user type which is used by the states
           typename IF>   // enforced interface
  class t_state;

  template<typename ID,   // enum type defining states
           typename USER, // user type which is used by the states
           typename IF>   // enforced interface
  class t_statemachine;

///////////////////////////////////////////////////////////////////////////////

  template<typename ID, typename USER = t_no_user, typename IF = t_no_if>
  struct t_traits {
    t_traits() = delete;

    using t_state        = state::t_state       <ID, USER, IF>;
    using t_statemachine = state::t_statemachine<ID, USER, IF>;
    using t_state_id     = typename t_prefix<ID>::t_;
    using t_interface    = typename t_prefix<IF>::t_;
    using t_user         = typename t_prefix<USER>::t_;
    using r_user         = typename t_prefix<USER>::r_;
    using R_user         = typename t_prefix<USER>::R_;
    using p_state        = typename t_prefix<t_state>::p_;
    using P_state        = typename t_prefix<t_state>::P_;
  };

  template<typename ID, typename IF>
  struct t_traits<ID, t_no_user, IF> {
    t_traits() = delete;

    using t_state        = state::t_state       <ID, t_no_user, IF>;
    using t_statemachine = state::t_statemachine<ID, t_no_user, IF>;
    using t_state_id     = typename t_prefix<ID>::t_;
    using t_interface    = typename t_prefix<IF>::t_;
    using p_state        = typename t_prefix<t_state>::p_;
    using P_state        = typename t_prefix<t_state>::P_;
  };

  template<typename ID, typename USER>
  struct t_traits<ID, USER, t_no_if> {
    t_traits() = delete;

    using t_state        = state::t_state       <ID, USER, t_no_if>;
    using t_statemachine = state::t_statemachine<ID, USER, t_no_if>;
    using t_state_id     = typename t_prefix<ID>::t_;
    using t_user         = typename t_prefix<USER>::t_;
    using r_user         = typename t_prefix<USER>::r_;
    using R_user         = typename t_prefix<USER>::R_;
    using p_state        = typename t_prefix<t_state>::p_;
    using P_state        = typename t_prefix<t_state>::P_;
  };

  template<typename ID>
  struct t_traits<ID, t_no_user, t_no_if> {
    t_traits() = delete;

    using t_state        = state::t_state       <ID, t_no_user, t_no_if>;
    using t_statemachine = state::t_statemachine<ID, t_no_user, t_no_if>;
    using t_state_id     = typename t_prefix<ID>::t_;
    using p_state        = typename t_prefix<t_state>::p_;
    using P_state        = typename t_prefix<t_state>::P_;
  };

///////////////////////////////////////////////////////////////////////////////

  template<typename ID, typename USER, typename IF>
  inline
  t_void debug_start(const t_statemachine<ID, USER, IF>&, ID start) {
  }

  template<typename ID, typename USER, typename IF>
  inline
  t_void debug_stop(const t_statemachine<ID, USER, IF>&) {
  }

  template<typename ID, typename USER, typename IF>
  inline
  t_void debug(const t_statemachine<ID, USER, IF>&, ID current, ID next) {
  }

///////////////////////////////////////////////////////////////////////////////

  template<typename ID, typename USER, typename IF>
  inline
  ID statemachine_transition_(t_statemachine<ID, USER, IF>& sm, ID next) {
    while (sm.curr_ != next) {
      if (next != sm.stop_) {
        sm.get_state(sm.curr_)->exit_point();
        debug(sm, sm.curr_, next);
        sm.curr_ = next;
        next = sm.get_state(sm.curr_)->entry_point();
      } else {
        sm.stop();
        break;
      }
    }
    return sm.curr_;
  }

  template<typename ID, typename USER, typename IF>
  inline
  ID statemachine_start_(t_statemachine<ID, USER, IF>& sm, ID start) {
    if (sm.curr_ == sm.stop_) {
      debug_start(sm, start);
      sm.curr_ = sm.initial_point(start);
      debug(sm, sm.stop_, sm.curr_);
      return sm.do_transition(sm.get_state(sm.curr_)->entry_point());
    }
    return sm.curr_;
  }

  template<typename ID, typename USER, typename IF>
  inline
  t_void statemachine_stop_(t_statemachine<ID, USER, IF>& sm) {
    if (sm.curr_ != sm.stop_) {
      sm.get_state(sm.curr_)->exit_point();
      debug(sm, sm.curr_, sm.stop_);
      sm.curr_ = sm.stop_;
      sm.final_point();
      debug_stop(sm);
    }
  }

///////////////////////////////////////////////////////////////////////////////

  template<typename ID, typename USER = t_no_user, typename IF = t_no_if>
  class t_state : public IF {
  public:
    using t_void     = state::t_void;
    using t_traits   = state::t_traits<ID, USER, IF>;
    using t_state_id = typename t_traits::t_state_id;
    using r_user     = typename t_traits::r_user;
    using R_user     = typename t_traits::R_user;

    // identification of the state
    t_state_id get_state_id() const noexcept { return state_id_; }

    r_user get_user ()       noexcept { return user_; }
    R_user get_user () const noexcept { return user_; }
    R_user get_cuser() const noexcept { return user_; }

  protected:
    // state object must have an associated id and access to user
    t_state(t_state_id id, r_user user) noexcept
      : state_id_{id}, user_{user} {
    }
    virtual ~t_state() { }

    // use to indicate that a state change is requested or not.
    virtual t_state_id request_transition(t_state_id id) const {
      return id;
    }

    t_state_id no_transition() const noexcept { return state_id_; }

  private:
    template<typename ID1, typename USER1, typename IF1>
    friend ID1 statemachine_transition_(t_statemachine<ID1, USER1, IF1>&, ID1);
    template<typename ID1, typename USER1, typename IF1>
    friend ID1 statemachine_start_(t_statemachine<ID1, USER1, IF1>&, ID1);
    template<typename ID1, typename USER1, typename IF1>
    friend t_void statemachine_stop_(t_statemachine<ID1, USER1, IF1>&);

    using T_state_id = typename t_prefix<t_state_id>::T_;

    // generic actions. entry_point can request a state change.
    virtual t_state_id entry_point() { return no_transition(); }
    virtual t_void     exit_point () { }

    T_state_id  state_id_;
    r_user      user_;
  };

  template<typename ID, typename IF>
  class t_state<ID, t_no_user, IF> : public IF {
  public:
    using t_void     = state::t_void;
    using t_traits   = state::t_traits<ID, t_no_user, IF>;
    using t_state_id = typename t_traits::t_state_id;

    // identification of the state
    t_state_id get_state_id() const noexcept { return state_id_; }

  protected:
    // state object must have an associated id
    t_state(t_state_id id) noexcept : state_id_{id} {
    }
    virtual ~t_state() { }

    // use to indicate that a state change is requested or not.
    virtual t_state_id request_transition(t_state_id id) const {
      return id;
    }

    t_state_id no_transition() const noexcept { return state_id_; }

  private:
    template<typename ID1, typename USER1, typename IF1>
    friend ID1 statemachine_transition_(t_statemachine<ID1, USER1, IF1>&, ID1);
    template<typename ID1, typename USER1, typename IF1>
    friend ID1 statemachine_start_(t_statemachine<ID1, USER1, IF1>&, ID1);
    template<typename ID1, typename USER1, typename IF1>
    friend t_void statemachine_stop_(t_statemachine<ID1, USER1, IF1>&);

    using T_state_id = typename t_prefix<t_state_id>::T_;

    // generic actions. entry_point can request a state change.
    virtual t_state_id entry_point() { return no_transition(); }
    virtual t_void     exit_point () { }

    T_state_id state_id_;
  };

  template<typename ID, typename USER>
  class t_state<ID, USER, t_no_if> {
  public:
    using t_void     = state::t_void;
    using t_traits   = state::t_traits<ID, USER, t_no_if>;
    using t_state_id = typename t_traits::t_state_id;
    using r_user     = typename t_traits::r_user;
    using R_user     = typename t_traits::R_user;

    // identification of the state
    t_state_id get_state_id() const noexcept { return state_id_; }

    r_user get_user ()       noexcept { return user_; }
    R_user get_user () const noexcept { return user_; }
    R_user get_cuser() const noexcept { return user_; }

  protected:
    // state object must have an associated id and access to user
    t_state(t_state_id id, r_user user) noexcept
      : state_id_{id}, user_{user} {
    }
    virtual ~t_state() { }

    // use to indicate that a state change is requested or not.
    virtual t_state_id request_transition(t_state_id id) const {
      return id;
    }

    t_state_id no_transition() const noexcept { return state_id_; }

  private:
    template<typename ID1, typename USER1, typename IF1>
    friend ID1 statemachine_transition_(t_statemachine<ID1, USER1, IF1>&, ID1);
    template<typename ID1, typename USER1, typename IF1>
    friend ID1 statemachine_start_(t_statemachine<ID1, USER1, IF1>&, ID1);
    template<typename ID1, typename USER1, typename IF1>
    friend t_void statemachine_stop_(t_statemachine<ID1, USER1, IF1>&);

    using T_state_id = typename t_prefix<t_state_id>::T_;

    // generic actions. entry_point can request a state change.
    virtual t_state_id entry_point() { return no_transition(); }
    virtual t_void     exit_point () { }

    T_state_id state_id_;
    r_user     user_;
  };

  template<typename ID>
  class t_state<ID, t_no_user, t_no_if> {
  public:
    using t_void     = state::t_void;
    using t_traits   = state::t_traits<ID, t_no_user, t_no_if>;
    using t_state_id = typename t_traits::t_state_id;

    // identification of the state
    t_state_id get_state_id() const noexcept { return state_id_; }

  protected:
    // state object must have an associated id
    t_state(t_state_id id) noexcept : state_id_{id} {
    }
    virtual ~t_state() { }

    // use to indicate that a state change is requested or not.
    virtual t_state_id request_transition(t_state_id id) const {
      return id;
    }

    t_state_id no_transition() const noexcept { return state_id_; }

  private:
    template<typename ID1, typename USER1, typename IF1>
    friend ID1 statemachine_transition_(t_statemachine<ID1, USER1, IF1>&, ID1);
    template<typename ID1, typename USER1, typename IF1>
    friend ID1 statemachine_start_(t_statemachine<ID1, USER1, IF1>&, ID1);
    template<typename ID1, typename USER1, typename IF1>
    friend t_void statemachine_stop_(t_statemachine<ID1, USER1, IF1>&);

    using T_state_id = typename t_prefix<t_state_id>::T_;

    // generic actions. entry_point can request a state change.
    virtual t_state_id entry_point() { return no_transition(); }
    virtual t_void     exit_point () { }

    T_state_id state_id_;
  };

///////////////////////////////////////////////////////////////////////////////

  template<typename ID, typename USER = t_no_user, typename IF = t_no_if>
  class t_statemachine : public IF {
  public:
    using t_void     = state::t_void;
    using t_traits   = state::t_traits<ID, USER, IF>;
    using t_state_id = typename t_traits::t_state_id;
    using r_user     = typename t_traits::r_user;
    using R_user     = typename t_traits::R_user;
    using p_state    = typename t_traits::p_state;
    using P_state    = typename t_traits::P_state;

    t_state_id get_current_state_id() const noexcept { return curr_; }

    // access user
    r_user get_user ()       noexcept { return user_; }
    R_user get_user () const noexcept { return user_; }
    R_user get_cuser() const noexcept { return user_; }

  protected:
    // important. user is NOT owned but used. PRE should be an invalid state.
    t_statemachine(t_state_id stop, r_user user) noexcept
      : stop_{stop}, curr_{stop_}, user_{user} {
    }

    // start the statemachine in the id state.
    t_state_id start(t_state_id next) {
      return statemachine_start_(*this, next);
    }

    // start the statemachine in the id state.
    t_state_id restart(t_state_id next) {
      stop();
      return start(next);
    }

    // stop the statemachine in the id state. this is the terminating state.
    t_void stop() {
      statemachine_stop_(*this);
    }

    // control state changes if required. return indicate if state was changed
    t_state_id do_transition(t_state_id next) {
      return statemachine_transition_(*this, next);
    }

    // access to current state pointer
    p_state get_current()        noexcept { return get_state(curr_); }
    P_state get_current()  const noexcept { return get_state(curr_); }
    P_state get_ccurrent() const noexcept { return get_current(curr_); }

  private:
    template<typename ID1, typename USER1, typename IF1>
    friend ID1 statemachine_transition_(t_statemachine<ID1, USER1, IF1>&, ID1);
    template<typename ID1, typename USER1, typename IF1>
    friend ID1 statemachine_start_(t_statemachine<ID1, USER1, IF1>&, ID1);
    template<typename ID1, typename USER1, typename IF1>
    friend t_void statemachine_stop_(t_statemachine<ID1, USER1, IF1>&);

    using T_state_id = typename t_prefix<t_state_id>::T_;

    // provide methods that detect when the statemachine starts/stops
    virtual t_state_id initial_point(t_state_id id) { return id; }
    virtual t_void     final_point  ()              { }

    // access to state pointer associated with their ids.
    virtual p_state get_state(t_state_id)       noexcept = 0;
    virtual P_state get_state(t_state_id) const noexcept = 0;

    P_state get_cstate(t_state_id id) const {
      return get_state(id);
    }

    T_state_id stop_;
    t_state_id curr_;
    r_user     user_;
  };

  template<typename ID, typename IF>
  class t_statemachine<ID, t_no_user, IF> : public IF {
  public:
    using t_void     = state::t_void;
    using t_traits   = state::t_traits<ID, t_no_user, IF>;
    using t_state_id = typename t_traits::t_state_id;
    using p_state    = typename t_traits::p_state;
    using P_state    = typename t_traits::P_state;

    t_state_id get_current_state_id() const noexcept { return curr_; }

  protected:
    // important. user is NOT owned but used. PRE should be an invalid state.
    t_statemachine(t_state_id stop) noexcept : stop_{stop}, curr_{stop_} {
    }

    // start the statemachine in the id state.
    t_state_id start(t_state_id next) {
      return statemachine_start_(*this, next);
    }

    // start the statemachine in the id state.
    t_state_id restart(t_state_id next) {
      stop();
      return start(next);
    }

    // stop the statemachine in the id state. this is the terminating state.
    t_void stop() {
      statemachine_stop_(*this);
    }

    // control state changes if required. return indicate if state was changed
    t_state_id do_transition(t_state_id next) {
      return statemachine_transition_(*this, next);
    }

    // access to current state pointer
    p_state get_current ()       noexcept { return get_state (curr_); }
    P_state get_current () const noexcept { return get_state (curr_); }
    P_state get_ccurrent() const noexcept { return get_cstate(curr_); }

  private:
    template<typename ID1, typename USER1, typename IF1>
    friend ID1 statemachine_transition_(t_statemachine<ID1, USER1, IF1>&, ID1);
    template<typename ID1, typename USER1, typename IF1>
    friend ID1 statemachine_start_(t_statemachine<ID1, USER1, IF1>&, ID1);
    template<typename ID1, typename USER1, typename IF1>
    friend t_void statemachine_stop_(t_statemachine<ID1, USER1, IF1>&);

    using T_state_id = typename t_prefix<t_state_id>::T_;

    // provide methods that detect when the statemachine starts/stops
    virtual t_state_id initial_point(t_state_id id) { return id; }
    virtual t_void     final_point  ()              { }

    // access to state pointer associated with their ids.
    virtual p_state get_state(t_state_id)       noexcept = 0;
    virtual P_state get_state(t_state_id) const noexcept = 0;

    P_state get_cstate(t_state_id id) const noexcept {
      return get_state(id);
    }

    T_state_id stop_;
    t_state_id curr_;
  };

  template<typename ID, typename USER>
  class t_statemachine<ID, USER, t_no_if> {
  public:
    using t_void     = state::t_void;
    using t_traits   = state::t_traits<ID, USER, t_no_if>;
    using t_state_id = typename t_traits::t_state_id;
    using r_user     = typename t_traits::r_user;
    using R_user     = typename t_traits::R_user;
    using p_state    = typename t_traits::p_state;
    using P_state    = typename t_traits::P_state;

    t_state_id get_current_state_id() const noexcept { return curr_; }

    // access user
    r_user get_user ()       noexcept { return user_; }
    R_user get_user () const noexcept { return user_; }
    R_user get_cuser() const noexcept { return user_; }

  protected:
    // important. user is NOT owned but used. PRE should be an invalid state.
    t_statemachine(t_state_id stop, r_user user) noexcept
      : stop_{stop}, curr_{stop_}, user_{user} {
    }

    // start the statemachine in the id state.
    t_state_id start(t_state_id next) {
      return statemachine_start_(*this, next);
    }

    // start the statemachine in the id state.
    t_state_id restart(t_state_id next) {
      stop();
      return start(next);
    }

    // stop the statemachine in the id state. this is the terminating state.
    t_void stop() {
      statemachine_stop_(*this);
    }

    // control state changes if required. return indicate if state was changed
    t_state_id do_transition(t_state_id next) {
      return statemachine_transition_(*this, next);
    }

    // NOTE:     current() is not given because there is no interface.
    // NOTE use: get_state(get_current_state_id()) to get current state from
    //           the derived class.

    // access to state pointer associated with their ids.
    virtual p_state get_state(t_state_id)       noexcept = 0;
    virtual P_state get_state(t_state_id) const noexcept = 0;

    P_state get_cstate(t_state_id id) const noexcept {
      return get_state(id);
    }

  private:
    template<typename ID1, typename USER1, typename IF1>
    friend ID1 statemachine_transition_(t_statemachine<ID1, USER1, IF1>&, ID1);
    template<typename ID1, typename USER1, typename IF1>
    friend ID1 statemachine_start_(t_statemachine<ID1, USER1, IF1>&, ID1);
    template<typename ID1, typename USER1, typename IF1>
    friend t_void statemachine_stop_(t_statemachine<ID1, USER1, IF1>&);

    using T_state_id = typename t_prefix<t_state_id>::T_;

    // provide methods that detect when the statemachine starts/stops
    virtual t_state_id initial_point(t_state_id id) { return id; }
    virtual t_void     final_point  ()              { }

    T_state_id stop_;
    t_state_id curr_;
    r_user     user_;
  };

  template<typename ID>
  class t_statemachine<ID, t_no_user, t_no_if> {
  public:
    using t_void     = state::t_void;
    using t_traits   = state::t_traits<ID, t_no_user, t_no_if>;
    using t_state_id = typename t_traits::t_state_id;
    using p_state    = typename t_traits::p_state;
    using P_state    = typename t_traits::P_state;

    t_state_id get_current_state_id() const noexcept { return curr_; }

  protected:
    // important. user is NOT owned but used. PRE should be an invalid state.
    t_statemachine(t_state_id stop) noexcept : stop_{stop}, curr_{stop_} {
    }

    // start the statemachine in the id state.
    t_state_id start(t_state_id next) {
      return statemachine_start_(*this, next);
    }

    // start the statemachine in the id state.
    t_state_id restart(t_state_id next) {
      stop();
      return start(next);
    }

    // stop the statemachine in the id state. this is the terminating state.
    t_void stop() {
      statemachine_stop_(*this);
    }

    // control state changes if required. return indicate if state was changed
    t_state_id do_transition(t_state_id next) {
      return statemachine_transition_(*this, next);
    }

    // NOTE:     current() is not given because there is no interface.
    // NOTE use: get_state(get_current_state_id()) to get current state from
    //           the derived class.

    // access to state pointer associated with their ids.
    virtual p_state get_state(t_state_id)       noexcept = 0;
    virtual P_state get_state(t_state_id) const noexcept = 0;

    P_state get_cstate(t_state_id id) const noexcept {
      return get_state(id);
    }

  private:
    template<typename ID1, typename USER1, typename IF1>
    friend ID1 statemachine_transition_(t_statemachine<ID1, USER1, IF1>&, ID1);
    template<typename ID1, typename USER1, typename IF1>
    friend ID1 statemachine_start_(t_statemachine<ID1, USER1, IF1>&, ID1);
    template<typename ID1, typename USER1, typename IF1>
    friend t_void statemachine_stop_(t_statemachine<ID1, USER1, IF1>&);

    using T_state_id = typename t_prefix<t_state_id>::T_;

    // provide methods that detect when the statemachine starts/stops
    virtual t_state_id initial_point(t_state_id id) { return id; }
    virtual t_void     final_point  ()              { }

    T_state_id stop_;
    t_state_id curr_;
  };

///////////////////////////////////////////////////////////////////////////////
}
}

#endif

