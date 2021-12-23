export DAINTY_ROOT=../../..
g++ -Wall -o run $DAINTY_ROOT/examples/sandbox/dainty_sandbox_example2.cpp \
                 $DAINTY_ROOT/sandbox/dainty_sandbox.cpp         \
                 $DAINTY_ROOT/sandbox/dainty_sandbox_err.cpp     \
                 $DAINTY_ROOT/sandbox/dainty_sandbox_impl.cpp    \
                 $DAINTY_ROOT/sandbox/logic_extensions/dainty_sandbox_logic_ext_inotify.cpp \
                 $DAINTY_ROOT/sandbox/logic_extensions/dainty_sandbox_logic_ext_mqueue_server.cpp \
                 $DAINTY_ROOT/sandbox/logic_extensions/dainty_sandbox_logic_ext_mqueue_client.cpp \
                 $DAINTY_ROOT/sandbox/logic_extensions/dainty_sandbox_logic_ext_tcp_server.cpp \
                 $DAINTY_ROOT/sandbox/logic_extensions/dainty_sandbox_logic_ext_tcp_client.cpp \
                 $DAINTY_ROOT/sandbox/logic_extensions/dainty_sandbox_logic_ext_udp_server.cpp \
                 $DAINTY_ROOT/sandbox/logic_extensions/dainty_sandbox_logic_ext_udp_client.cpp \
                 $DAINTY_ROOT/sandbox/logic_extensions/dainty_sandbox_logic_ext_file.cpp \
                 $DAINTY_ROOT/sandbox/logic_extensions/dainty_sandbox_logic_ext_messenger.cpp \
                 $DAINTY_ROOT/sandbox/logic_extensions/dainty_sandbox_logic_ext_cli.cpp \
                 $DAINTY_ROOT/sandbox/logic_extensions/dainty_sandbox_logic_ext_large_msg.cpp \
                 $DAINTY_ROOT/service/messaging/dainty_messaging_err.cpp \
                 $DAINTY_ROOT/service/messaging/dainty_messaging.cpp \
                 $DAINTY_ROOT/service/tracing/dainty_tracing_err.cpp \
                 $DAINTY_ROOT/service/tracing/dainty_tracing.cpp \
                 $DAINTY_ROOT/library/container/dainty_container_any.cpp \
                 $DAINTY_ROOT/library/container/dainty_container_bytebuf_impl.cpp \
                 $DAINTY_ROOT/library/container/dainty_container_err.cpp \
                 $DAINTY_ROOT/library/mt/dainty_mt_timers.cpp \
                 $DAINTY_ROOT/library/mt/dainty_mt_event.cpp \
                 $DAINTY_ROOT/library/mt/dainty_mt_condvar_chained_queue.cpp \
                 $DAINTY_ROOT/library/mt/dainty_mt_condvar_timed_event.cpp \
                 $DAINTY_ROOT/library/mt/dainty_mt_waitable_chained_queue.cpp \
                 $DAINTY_ROOT/library/mt/dainty_mt_detached_thread.cpp \
                 $DAINTY_ROOT/library/mt/dainty_mt_err.cpp \
                 $DAINTY_ROOT/library/mt/dainty_mt_command.cpp \
                 $DAINTY_ROOT/library/mt/dainty_mt_thread.cpp \
                 $DAINTY_ROOT/library/mt/dainty_mt_notify_change.cpp \
                 $DAINTY_ROOT/library/mt/dainty_mt_chained_queue.cpp \
                 $DAINTY_ROOT/library/mt/dainty_mt_condvar_event.cpp \
                 $DAINTY_ROOT/library/mt/dainty_mt_condvar_notify_change.cpp \
                 $DAINTY_ROOT/library/mt/dainty_mt_event_dispatcher.cpp \
                 $DAINTY_ROOT/library/mt/dainty_mt_condvar_command.cpp \
                 $DAINTY_ROOT/library/os/dainty_os_clock.cpp \
                 $DAINTY_ROOT/library/os/dainty_os_call.cpp \
                 $DAINTY_ROOT/library/os/dainty_os_threading.cpp \
                 $DAINTY_ROOT/library/os/dainty_os_err.cpp \
                 $DAINTY_ROOT/library/os/dainty_os_networking.cpp \
                 $DAINTY_ROOT/library/os/dainty_os_fdbased.cpp \
                 $DAINTY_ROOT/library/os/dainty_os_scheduling.cpp \
                 $DAINTY_ROOT/library/os/dainty_os.cpp \
                 $DAINTY_ROOT/library/oops/dainty_oops.cpp \
                 $DAINTY_ROOT/library/base/dainty_base_string_impl.cpp \
                 $DAINTY_ROOT/library/base/dainty_base_assert.cpp \
                 $DAINTY_ROOT/library/base/dainty_base_terminal.cpp \
                 $DAINTY_ROOT/library/base/dainty_base_range.cpp \
                 -I$DAINTY_ROOT/sandbox \
                 -I$DAINTY_ROOT/sandbox/logic_extensions \
                 -I$DAINTY_ROOT/library/base \
                 -I$DAINTY_ROOT/library/mt \
                 -I$DAINTY_ROOT/library/os \
                 -I$DAINTY_ROOT/library/oops \
                 -I$DAINTY_ROOT/library/container \
                 -I$DAINTY_ROOT/service/tracing \
                 -I$DAINTY_ROOT/service/messaging \
                 -std=c++17 -lpthread -O2
