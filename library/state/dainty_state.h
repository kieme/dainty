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

#include "dainty_named.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

#define DAINTY_SM_START                                          \
      if (curr_ == stop_) {                                      \
        debug_start(*this, sid);                                 \
        curr_ = initial_point(sid);                              \
        debug(*this, stop_, curr_);                              \
        return do_transition(get_state(curr_)->entry_point());   \
      }                                                          \
      return curr_;

#define DAINTY_SM_STOP                                           \
      if (curr_ != stop_) {                                      \
        get_state(curr_)->exit_point();                          \
        debug(*this, curr_, stop_);                              \
        curr_ = stop_;                                           \
        final_point();                                           \
        debug_stop(*this);                                       \
      }

#define DAINTY_SM_TRANSITION                                     \
      if (next != curr_) {                                       \
        if (next != stop_) {                                     \
          get_state(curr_)->exit_point();                        \
          debug(*this, curr_, next);                             \
          curr_ = next;                                          \
          next = do_transition(get_state(curr_)->entry_point()); \
        } else                                                   \
          stop();                                                \
      }                                                          \
      return curr_;

///////////////////////////////////////////////////////////////////////////////////////////////////

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

  using named::t_prefix;
  using named::t_void;

  ///////////////////////////////////////////////////////////////////////////

  enum t_no_user { };
  enum t_no_if   { };

  ///////////////////////////////////////////////////////////////////////////

  template<typename ID,   // enum type defining states
           typename USER, // user type which is used by the states
           typename IF>   // enforced interface
  class t_state;

  template<typename ID,   // enum type defining states
           typename USER, // user type which is used by the states
           typename IF>   // enforced interface
  class t_statemachine;

  ///////////////////////////////////////////////////////////////////////////

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

  ///////////////////////////////////////////////////////////////////////////

  template<typename ID, typename USER = t_no_user, typename IF = t_no_if>
  struct t_traits {
    t_traits() = delete;

    using t_state        = state::t_state       <ID, USER, IF>;
    using t_statemachine = state::t_statemachine<ID, USER, IF>;
    using t_state_id     = typename named::t_prefix<ID>::t_;
    using t_interface    = typename named::t_prefix<IF>::t_;
    using t_user         = typename named::t_prefix<USER>::t_;
    using r_user         = typename named::t_prefix<USER>::r_;
    using R_user         = typename named::t_prefix<USER>::R_;
    using p_state        = typename named::t_prefix<t_state>::p_;
    using P_state        = typename named::t_prefix<t_state>::P_;
  };

  template<typename ID, typename IF>
  struct t_traits<ID, t_no_user, IF> {
    t_traits() = delete;

    using t_state        = state::t_state       <ID, t_no_user, IF>;
    using t_statemachine = state::t_statemachine<ID, t_no_user, IF>;
    using t_state_id     = typename named::t_prefix<ID>::t_;
    using t_interface    = typename named::t_prefix<IF>::t_;
    using p_state        = typename named::t_prefix<t_state>::p_;
    using P_state        = typename named::t_prefix<t_state>::P_;
  };

  template<typename ID, typename USER>
  struct t_traits<ID, USER, t_no_if> {
    t_traits() = delete;

    using t_state        = state::t_state       <ID, USER, t_no_if>;
    using t_statemachine = state::t_statemachine<ID, USER, t_no_if>;
    using t_state_id     = typename named::t_prefix<ID>::t_;
    using t_user         = typename named::t_prefix<USER>::t_;
    using r_user         = typename named::t_prefix<USER>::r_;
    using R_user         = typename named::t_prefix<USER>::R_;
    using p_state        = typename named::t_prefix<t_state>::p_;
    using P_state        = typename named::t_prefix<t_state>::P_;
  };

  template<typename ID>
  struct t_traits<ID, t_no_user, t_no_if> {
    t_traits() = delete;

    using t_state        = state::t_state       <ID, t_no_user, t_no_if>;
    using t_statemachine = state::t_statemachine<ID, t_no_user, t_no_if>;
    using t_state_id     = typename named::t_prefix<ID>::t_;
    using p_state        = typename named::t_prefix<t_state>::p_;
    using P_state        = typename named::t_prefix<t_state>::P_;
  };

  ///////////////////////////////////////////////////////////////////////////

  template<typename ID, typename USER = t_no_user, typename IF = t_no_if>
  class t_state : public IF {
  public:
    using t_void   = state::t_void;
    using t_traits = state::t_traits<ID, USER, IF>;
    using t_sid    = typename t_traits::t_state_id;
    using r_user   = typename t_traits::r_user;
    using R_user   = typename t_traits::R_user;

    // identification of the state
    t_sid get_sid() const noexcept { return sid_; }

  protected:
    // state object must have an associated id and access to user
    t_state(t_sid sid, r_user user) noexcept : sid_{sid}, user_{user} { }
    virtual ~t_state()                                                { }

    // use to indicate that a state change is requested or not.
    virtual t_sid request_transition(t_sid sid) const          { return sid;  }
            t_sid no_transition     ()          const noexcept { return sid_; }

    r_user get_user ()       noexcept { return user_; }
    R_user get_user () const noexcept { return user_; }
    R_user get_cuser() const noexcept { return user_; }

  private:
    using T_sid = typename t_prefix<t_sid>::T_;
    template<typename, typename, typename> friend class t_statemachine;

    // generic actions. entry_point can request a state change.
    virtual t_sid  entry_point() { return no_transition(); }
    virtual t_void exit_point () { }

    T_sid  sid_;
    r_user user_;
  };

  template<typename ID, typename IF>
  class t_state<ID, t_no_user, IF> : public IF {
  public:
    using t_void   = state::t_void;
    using t_traits = state::t_traits<ID, t_no_user, IF>;
    using t_sid    = typename t_traits::t_state_id;

    // identification of the state
    t_sid get_sid() const noexcept { return sid_; }

  protected:
    // state object must have an associated id
    t_state(t_sid sid) noexcept : sid_{sid} { }
    virtual ~t_state()                      { }

    // use to indicate that a state change is requested or not.
    virtual t_sid request_transition(t_sid sid) const          { return sid;  }
            t_sid no_transition     ()          const noexcept { return sid_; }

  private:
    using T_sid = typename t_prefix<t_sid>::T_;
    template<typename, typename, typename> friend class t_statemachine;

    // generic actions. entry_point can request a state change.
    virtual t_sid  entry_point() { return no_transition(); }
    virtual t_void exit_point () { }

    T_sid sid_;
  };

  template<typename ID, typename USER>
  class t_state<ID, USER, t_no_if> {
  public:
    using t_void   = state::t_void;
    using t_traits = state::t_traits<ID, USER, t_no_if>;
    using t_sid    = typename t_traits::t_state_id;
    using r_user   = typename t_traits::r_user;
    using R_user   = typename t_traits::R_user;

    // identification of the state
    t_sid get_sid() const noexcept { return sid_; }

  protected:
    // state object must have an associated id and access to user
    t_state(t_sid sid, r_user user) noexcept : sid_{sid}, user_{user} { }
    virtual ~t_state()                                                { }

    // use to indicate that a state change is requested or not.
    virtual t_sid request_transition(t_sid sid) const          { return sid;  }
            t_sid no_transition     ()          const noexcept { return sid_; }

    r_user get_user ()       noexcept { return user_; }
    R_user get_user () const noexcept { return user_; }
    R_user get_cuser() const noexcept { return user_; }

  private:
    using T_sid = typename t_prefix<t_sid>::T_;
    template<typename, typename, typename> friend class t_statemachine;

    // generic actions. entry_point can request a state change.
    virtual t_sid  entry_point() { return no_transition(); }
    virtual t_void exit_point () { }

    T_sid  sid_;
    r_user user_;
  };

  template<typename ID>
  class t_state<ID, t_no_user, t_no_if> {
  public:
    using t_void = state::t_void;
    using t_traits = state::t_traits<ID, t_no_user, t_no_if>;
    using t_sid    = typename t_traits::t_state_id;

    // identification of the state
    t_sid get_sid() const noexcept { return sid_; }

  protected:
    // state object must have an associated id
    t_state(t_sid sid) noexcept : sid_{sid}  { }
    virtual ~t_state()                       { }

    // use to indicate that a state change is requested or not.
    virtual t_sid request_transition(t_sid sid) const          { return sid;  }
            t_sid no_transition     ()          const noexcept { return sid_; }

  private:
    using T_sid = typename t_prefix<t_sid>::T_;
    template<typename, typename, typename> friend class t_statemachine;

    // generic actions. entry_point can request a state change.
    virtual t_sid  entry_point() { return no_transition(); }
    virtual t_void exit_point () { }

    T_sid sid_;
  };

  ///////////////////////////////////////////////////////////////////////////

  template<typename ID, typename USER = t_no_user, typename IF = t_no_if>
  class t_statemachine : public IF {
  public:
    using t_void   = state::t_void;
    using t_traits = state::t_traits<ID, USER, IF>;
    using t_sid    = typename t_traits::t_state_id;
    using r_user   = typename t_traits::r_user;
    using R_user   = typename t_traits::R_user;
    using p_state  = typename t_traits::p_state;
    using P_state  = typename t_traits::P_state;

    t_sid get_current_sid() const noexcept { return curr_; }

    // access user
    r_user get_user ()       noexcept { return user_; }
    R_user get_user () const noexcept { return user_; }
    R_user get_cuser() const noexcept { return user_; }

  protected:
    // important. user is NOT owned but used. PRE should be an invalid state.
    t_statemachine(t_sid stop, r_user user) noexcept
      : stop_{stop}, curr_{stop_}, user_{user} {
    }

    // start the statemachine in the id state.
    t_sid start(t_sid sid) {
      DAINTY_SM_START
    }

    // start the statemachine in the id state.
    t_sid restart(t_sid sid) {
      stop();
      return start(sid);
    }

    // stop the statemachine in the id state. this is the terminating state.
    t_void stop() {
      DAINTY_SM_STOP
    }

    // control state changes if required. return indicate if state was changed
    t_sid do_transition(t_sid next) {
      DAINTY_SM_TRANSITION
    }

    // access to current state pointer
    p_state get_current()        noexcept { return get_state(curr_); }
    P_state get_current()  const noexcept { return get_state(curr_); }
    P_state get_ccurrent() const noexcept { return get_current(curr_); }

  private:
    using T_sid = typename t_prefix<t_sid>::T_;

    // provide methods that detect when the statemachine starts/stops
    virtual t_sid  initial_point(t_sid id) { return id; }
    virtual t_void final_point  ()                    { }

    // access to state pointer associated with their ids.
    virtual p_state get_state (t_sid)           noexcept = 0;
    virtual P_state get_state (t_sid)     const noexcept = 0;

    P_state get_cstate(t_sid sid) const {
      return get_state(sid);
    }

    T_sid  stop_;
    t_sid  curr_;
    r_user user_;
  };

  template<typename ID, typename IF>
  class t_statemachine<ID, t_no_user, IF> : public IF {
  public:
    using t_void   = state::t_void;
    using t_traits = state::t_traits<ID, t_no_user, IF>;
    using t_sid    = typename t_traits::t_state_id;
    using p_state  = typename t_traits::p_state;
    using P_state  = typename t_traits::P_state;

    t_sid get_current_sid() const noexcept { return curr_; }

  protected:
    // important. user is NOT owned but used. PRE should be an invalid state.
    t_statemachine(t_sid stop) noexcept : stop_{stop}, curr_{stop_} {
    }

    // start the statemachine in the id state.
    t_sid start(t_sid sid) {
      DAINTY_SM_START
    }

    // start the statemachine in the id state.
    t_sid restart(t_sid sid) {
      stop();
      return start(sid);
    }

    // stop the statemachine in the id state. this is the terminating state.
    t_void stop() {
      DAINTY_SM_STOP
    }

    // control state changes if required. return indicate if state was changed
    t_sid do_transition(t_sid next) {
      DAINTY_SM_TRANSITION
    }

    // access to current state pointer
    p_state get_current ()       noexcept { return get_state (curr_); }
    P_state get_current () const noexcept { return get_state (curr_); }
    P_state get_ccurrent() const noexcept { return get_cstate(curr_); }

  private:
    using T_sid = typename t_prefix<t_sid>::T_;

    // provide methods that detect when the statemachine starts/stops
    virtual t_sid  initial_point(t_sid id) { return id; }
    virtual t_void final_point  ()                    { }

    // access to state pointer associated with their ids.
    virtual p_state get_state(t_sid)       noexcept = 0;
    virtual P_state get_state(t_sid) const noexcept = 0;

    P_state get_cstate(t_sid sid) const noexcept {
      return get_state(sid);
    }

    T_sid stop_;
    t_sid curr_;
  };

  template<typename ID, typename USER>
  class t_statemachine<ID, USER, t_no_if> {
  public:
    using t_void   = state::t_void;
    using t_traits = state::t_traits<ID, USER, t_no_if>;
    using t_sid    = typename t_traits::t_state_id;
    using r_user   = typename t_traits::r_user;
    using R_user   = typename t_traits::R_user;
    using p_state  = typename t_traits::p_state;
    using P_state  = typename t_traits::P_state;

    t_sid get_current_sid() const noexcept { return curr_; }

    // access user
    r_user get_user ()       noexcept { return user_; }
    R_user get_user () const noexcept { return user_; }
    R_user get_cuser() const noexcept { return user_; }

  protected:
    // important. user is NOT owned but used. PRE should be an invalid state.
    t_statemachine(t_sid stop, r_user user) noexcept
      : stop_{stop}, curr_{stop_}, user_{user} {
    }

    // start the statemachine in the id state.
    t_sid start(t_sid sid) {
      DAINTY_SM_START
    }

    // start the statemachine in the id state.
    t_sid restart(t_sid sid) {
      stop();
      return start(sid);
    }

    // stop the statemachine in the id state. this is the terminating state.
    t_void stop() {
      DAINTY_SM_STOP
    }

    // control state changes if required. return indicate if state was changed
    t_sid do_transition(t_sid next) {
      DAINTY_SM_TRANSITION
    }

    // NOTE:     current() is not given because there is no interface.
    // NOTE use: get_state(get_current_sid()) to get current state from the
    //           the derived class.

    // access to state pointer associated with their ids.
    virtual p_state get_state(t_sid)       noexcept = 0;
    virtual P_state get_state(t_sid) const noexcept = 0;

    P_state get_cstate(t_sid sid) const noexcept {
      return get_state(sid);
    }

  private:
    using T_sid = typename t_prefix<t_sid>::T_;

    // provide methods that detect when the statemachine starts/stops
    virtual t_sid  initial_point(t_sid id) { return id; }
    virtual t_void final_point  ()                    { }

    T_sid  stop_;
    t_sid  curr_;
    r_user user_;
  };

  template<typename ID>
  class t_statemachine<ID, t_no_user, t_no_if> {
  public:
    using t_void   = state::t_void;
    using t_traits = state::t_traits<ID, t_no_user, t_no_if>;
    using t_sid    = typename t_traits::t_state_id;
    using p_state  = typename t_traits::p_state;
    using P_state  = typename t_traits::P_state;

    t_sid get_current_sid() const noexcept { return curr_; }

  protected:
    // important. user is NOT owned but used. PRE should be an invalid state.
    t_statemachine(t_sid stop) noexcept : stop_{stop}, curr_{stop_} {
    }

    // start the statemachine in the id state.
    t_sid start(t_sid sid) {
      DAINTY_SM_START
    }

    // start the statemachine in the id state.
    t_sid restart(t_sid sid) {
      stop();
      return start(sid);
    }

    // stop the statemachine in the id state. this is the terminating state.
    t_void stop() {
      DAINTY_SM_STOP
    }

    // control state changes if required. return indicate if state was changed
    t_sid do_transition(t_sid next) {
      DAINTY_SM_TRANSITION
    }

    // NOTE:     current() is not given because there is no interface.
    // NOTE use: get_state(get_current_sid()) to get current state from the
    //           the derived class.

    // access to state pointer associated with their ids.
    virtual p_state get_state(t_sid)       noexcept = 0;
    virtual P_state get_state(t_sid) const noexcept = 0;

    P_state get_cstate(t_sid sid) const noexcept {
      return get_state(sid);
    }

  private:
    using T_sid = typename t_prefix<t_sid>::T_;

    // provide methods that detect when the statemachine starts/stops
    virtual t_sid  initial_point(t_sid id) { return id; }
    virtual t_void final_point  ()                    { }

    T_sid stop_;
    t_sid curr_;
  };

///////////////////////////////////////////////////////////////////////////////
}
}

#endif

