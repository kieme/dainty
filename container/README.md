# dainty_container
&lt;dainty::container> consists of a number of container types that are used by the &lt;dainty> framework

Historical context: The containers were initialy developed for a deterministic application. They are now transformed to a newer c++ version and nameing convention, which will most likely introduce bugs. This will be fixed when unit testing is introduced.

The dainty containers offers functionality not available in &lt;std> containers. All the containers have the following characterists:
1. incorporate &lt;oops>, the error handling mechanism used by the dainty framework.
2. heap and stack version avaiable for most of the containers.
3. small interfaces (compared to wide interfaces) to do essential work.
4. most containers are inplace containers that do not allow implicit copy or move.
5. no function method using t_err will crash.
6. container implementations are simple to understand and debug.

current containers:
1.  t_freelist
2.  t_list
3.  t_pushlist
4.  t_map
5.  t_valuestore - required to manage contruction & destruction
6.  t_any
7.  t_maybe
8.  t_passable_ptr
9.  t_holder
10. t_ptrlist

many containers to follow (they must still be ported from another framework)
1. t_linked_queue
2. t_linked_ring
3. t_bytebuf
4. t_set
5. t_uniquelist
6. t_array
7. t_string
8. t_texting
9. t_varbuf
10. ....
