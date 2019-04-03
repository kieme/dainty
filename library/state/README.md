# dainty_state
&lt;dainty::state> is a small c++ framework that supports development of statemachines

<dainty::state> offers and promote a pattern that can be used to design
and implement state machines of different complexities effectively. Originally
the intention was to model UML behaviour statemachine with most of its
features. This however did not translate well into C++ software. Repeatedly
the formalistic UML constructs only introduced overhead, both on the
processing side and the amount of source code required.

Another approach was adopted: define a framework that only support the major
statemachine elements that are actually used by the majority of implementations.

It was also decided that if the user of the statemachine could do something
more cogently then trying to solve it within the framework (this was also
tried a few times), then it becomes part of what the user should do and it
should be described as part of the pattern.

This lead to some of the same goals as "statecharts", where composite-states,
submachine states can be implemented as an additional embedded statemachines.


Summary of <dainty::state> goals:

  1. Use a pattern that is flexible and easily repeatable.
  2. The mechanics (inner working) should be a light as possible.
  3. Re-use parts to construct more complex structures when needed.
  4. The source code should reflect the design extermely closely
     (avoid concepts unrelated to the design)
  5. The source code must be easily readable and debugable.


<dainty::state> framework elements:

  1. t_traits          -> define states, define triggers, define state sharing
  2. t_state           -> abstract class that represent a specific state.
  3. t_statemachine    -> abstract class that represent all the states and
                          handles the transitioning.
  4. a debug interface -> can plugin to see the transitioning of the
                          the statemachine.


element: dainty::state::t_traits

  t_traits is a class template that describes the type of statemachine you
  about to define.

  // primary template
  template<typename ID, typename USER = t_no_user, typename IF = t_no_if>
  struct t_traits;

  The template has three template parameters that are associated with
  distinct characteristics of a statemachine.

  1. argument 1: ID   -> represent the state id type. suggest using an enum.
  2. argument 2: USER -> user specific reference accessable to all operations.
  3. argument 3: IF   -> abstract interface representing the triggers and their
                         associated responses.


  t_no_user (require no shared user reference) and t_no_if (no common trigger
  interface for all states) are specializations that allow the same pattern to
  be used but with different requirements. This will be explained in full
  later. Its likely that t_no_user, t_no_if will be very seldomly used.

  e.g app has:

          three states:         -> enum class t_state_id {
            state1                   STATE1_ID = 0, // explicit
            state2                   STATE2_ID,
            state3                   STATE3_ID,
                                     STOP_ID
                                   };

          four triggers:         -> struct t_trigger_if {
                                      virtual ~t_trigger_if() {}
            timeout_1                 virtual t_state_id timeout_1   () = 0;
            timeout_2                 virtual t_state_id timeout_2   () = 0;
            config_ev                 virtual t_state_id config_ev   () = 0;
            detection_ev              virtual t_state_id detection_ev() = 0;
                                    };

          will invoke actions:   -> struct t_action_if {
                                      virtual ~t_action_if() { }
            test                      virtual t_bool test() = 0;
            do1                       virtual t_bool do1 () = 0;
            do2                       virtual t_bool do2 () = 0;
                                    };

     using t_app_traits_      = t_traits<t_state_id, t_action_if, t_event_if>;
     using t_app_state        = t_app_traits_::t_state;
     using t_app_statemachine = t_app_traits_::t_statemachine;

     From this point you only need to use t_app_state and t_app_statemachine.


element: dainty::state::t_state

  The state class describes a particular state or set of behaviours associated
  with a specific moment.

  // primary template
  template<typename ID, typename USER = t_no_user, typename IF = t_no_if>
  class t_state;

  The class template parameters reflect that of the t_traits class. It is
  recommended to use the t_traits to get both the t_state and t_statemachine
  declarations (as shown in the t_app_state example above).

  The state class by itself does not do any transitioning, but it must request
  it. It supports a small number of functions.

    1. t_state(t_state_id id, r_user user)

      Has a single constructor that cannot be ignored. You must specify the
      state using the t_state_id that was defined. This should be a enum type.
      r_user is a modifiable reference to a user type instance that are shared
      by all the states and the statemachine. r_user is only available if
      a user type is given, otherwise the default type, t_no_user, will be
      used.

      A characteristic of t_state is that it has no contact with either
      the statemachine or any other states. This is different from most other
      state pattern implementations. This is intentional as to make sure the
      state can only "request" a transition and not force it. Another reason
      is that it should not have access to another states instance, where it
      could invoke anything it wants (or the programmer could). The same
      argument can be made if it had a reference to the statemachine instance.

      This means t_state is isolated from the statemachine instance and from
      the other state instances.


      t_state supports the following methods:

      1. const methods

        1.1. read the state_id of the state.

          t_state_id get_state_id() const noexcept { return state_id_; }

        1.2. access the shared user reference. different varities.

          r_user get_user ()       noexcept { return user_; }
          R_user get_user () const noexcept { return user_; }
          R_user get_cuser() const noexcept { return user_; }

        1.3 request (or not) a state transition

          request_transition request a transition. In part it is mostly
          documentation. It can be specialized to make certain it actually does
          request the proper states for transistion.

          virtual t_state_id request_transition(t_state_id id) const {
            return id;
          }

          t_state_id no_transition() const noexcept { return state_id_; }

          When no transition is required then use no_transition(). It returns
          the current state id and it acts as documentation.




