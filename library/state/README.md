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
                          th statemachine.


element: dainty::state::t_traits

  t_traits is a class template that describes the type of statemachine you
  about to define.

  // primary template
  template<typename I, typename U = t_no_user, typename IF = t_no_if>
  struct t_traits;

  The template has three template parameters that are associated with
  distinct characteristics of a statemachine.

  1. argument 1: typename I  -> represent the state id. suggest using an enum.
  2. argument 2: typename U  -> a user specific reference accessable to all
                                operations.
  3. argument 3: typename IF -> an abstract interface representing the triggers
                                and their associated responses.


  t_no_user (require no shared user reference) and t_no_if (no common trigger
  interface for all states) are specializations that allow the same pattern to
  be used but with different requirements. This will be explained in full
  later. Its likely that t_no_user, t_no_if will be very seldomly be used.

  e.g a functionality (app1) has:

                            Translation
                            -----------

          three states:         -> enum class t_state_id {
            state1                   STATE1_ID = 0, // explicit
            state2                   STATE2_ID,
            state3                   STATE3_ID,
                                     STOP_ID
                                   };

          four triggers:         -> struct t_event_if {
                                      virtual ~t_event_if() {}
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

     using t_app1_state_traits = t_traits<t_state_id, t_action_if, t_event_if>;
     using t_app1_state        = t_app1_state_traits::t_state;
     using t_app1_statemachine = t_app1_state_traits::t_statemachine;

     From this point you only need to use t_app1_state and t_app1_statemachine.






