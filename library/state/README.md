# dainty_state
&lt;dainty::state> is a small c++ framework that supports development of statemachines

  <lt;dainty::state> offers and promote a pattern that can be used to design
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



