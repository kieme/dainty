# private_dainty_mt
&lt;dainty::mt> is a multithreading library that is used by the dainty framework

*&lt;dainty::mt::thread>*
  
A number of classes that represent a higher abstraction of threads.
  
  1. t_thread (t_logic)
  2. t_detachedthread (t_logic)

&lt;dainty::mt::command>
  
  A number of classes that support the issuing and the processing of commands over threading boundaries.
  
  1. t_client
  2. t_fd_processor (t_logic)
  3. t_condvar_processor (t_logic)

many multithreading related services must still be ported and added to dainty::mt.

&lt;dainty::mt::linked_queue>
  1. t_client
  2. t_fd_processor (t_logic)
  3. t_condvar_processor (t_logic)

&lt;dainty::mt::event>
  1. t_client
  2. t_fd_processor
  3. t_condvar_processor

&lt;dainty::mt::notify_change>
  1. t_client
  2. t_fd_processor
  3. t_condvar_processor

&lt;dainty::mt::mqueue>
  1. t_mq_client
  2. t_mq_processor

etc...
