export DAINTY_ROOT=../..
g++ -Wall \
  -I$DAINTY_ROOT/sandbox \
  -I$DAINTY_ROOT/sandbox/logic_extensions \
  -I$DAINTY_ROOT/library/base \
  -I$DAINTY_ROOT/library/mt \
  -I$DAINTY_ROOT/library/os \
  -I$DAINTY_ROOT/library/oops \
  -I$DAINTY_ROOT/library/container \
  -I$DAINTY_ROOT/service/tracing \
  -I$DAINTY_ROOT/service/messaging \
  -std=c++17 -lpthread -O2 -fsanitize=address -pg -ggdb3 -c \
  $DAINTY_ROOT/sandbox/dainty_sandbox.cpp \
  $DAINTY_ROOT/sandbox/dainty_sandbox_impl.cpp \
  $DAINTY_ROOT/sandbox/dainty_sandbox_err.cpp \
  $DAINTY_ROOT/sandbox/logic_extensions/dainty_sandbox_logic_ext_tipc_client.cpp \
  $DAINTY_ROOT/sandbox/logic_extensions/dainty_sandbox_logic_ext_file.cpp \
  $DAINTY_ROOT/sandbox/logic_extensions/dainty_sandbox_logic_ext_mqueue_server.cpp \
  $DAINTY_ROOT/sandbox/logic_extensions/dainty_sandbox_logic_ext_cli.cpp \
  $DAINTY_ROOT/sandbox/logic_extensions/dainty_sandbox_logic_ext_tipc_server.cpp \
  $DAINTY_ROOT/sandbox/logic_extensions/dainty_sandbox_logic_ext_inotify.cpp \
  $DAINTY_ROOT/sandbox/logic_extensions/dainty_sandbox_logic_ext_mqueue_client.cpp \
  $DAINTY_ROOT/sandbox/logic_extensions/dainty_sandbox_logic_ext_large_msg.cpp \
  $DAINTY_ROOT/sandbox/logic_extensions/dainty_sandbox_logic_ext_udp_client.cpp \
  $DAINTY_ROOT/sandbox/logic_extensions/dainty_sandbox_logic_ext_messenger.cpp \
  $DAINTY_ROOT/sandbox/logic_extensions/dainty_sandbox_logic_ext_tcp_server.cpp \
  $DAINTY_ROOT/sandbox/logic_extensions/dainty_sandbox_logic_ext_udp_server.cpp \
  $DAINTY_ROOT/sandbox/logic_extensions/dainty_sandbox_logic_ext_tcp_client.cpp \
  $DAINTY_ROOT/service/messaging/dainty_messaging.cpp \
  $DAINTY_ROOT/service/messaging/dainty_messaging_err.cpp \
  $DAINTY_ROOT/service/tracing/dainty_tracing_err.cpp \
  $DAINTY_ROOT/service/tracing/dainty_tracing.cpp \
  $DAINTY_ROOT/library/container/dainty_container_any.cpp \
  $DAINTY_ROOT/library/container/dainty_container_bytebuf_impl.cpp \
  $DAINTY_ROOT/library/container/dainty_container_err.cpp \
  $DAINTY_ROOT/library/mt/dainty_mt_event.cpp \
  $DAINTY_ROOT/library/mt/dainty_mt_net_connect.cpp \
  $DAINTY_ROOT/library/mt/dainty_mt_condvar_chained_queue.cpp \
  $DAINTY_ROOT/library/mt/dainty_mt_condvar_timed_event.cpp \
  $DAINTY_ROOT/library/mt/dainty_mt_waitable_chained_queue.cpp \
  $DAINTY_ROOT/library/mt/dainty_mt_detached_thread.cpp \
  $DAINTY_ROOT/library/mt/dainty_mt_err.cpp \
  $DAINTY_ROOT/library/mt/dainty_mt_command.cpp \
  $DAINTY_ROOT/library/mt/dainty_mt_thread.cpp \
  $DAINTY_ROOT/library/mt/dainty_mt_notify_change.cpp \
  $DAINTY_ROOT/library/mt/dainty_mt_net_tipc.cpp \
  $DAINTY_ROOT/library/mt/dainty_mt_net.cpp \
  $DAINTY_ROOT/library/mt/dainty_mt_chained_queue.cpp \
  $DAINTY_ROOT/library/mt/dainty_mt_timers.cpp \
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
  $DAINTY_ROOT/library/dacli/dainty_dacli.cpp \
  $DAINTY_ROOT/library/dacli/dainty_dacli_err.cpp \
  $DAINTY_ROOT/library/oops/dainty_oops.cpp \
  $DAINTY_ROOT/library/base/dainty_base_mem_impl.cpp \
  $DAINTY_ROOT/library/base/dainty_base_buf_impl.cpp \
  $DAINTY_ROOT/library/base/dainty_base_string_segmented_impl.cpp \
  $DAINTY_ROOT/library/base/dainty_base_terminal.cpp \
  $DAINTY_ROOT/library/base/dainty_base_assert.cpp \
  $DAINTY_ROOT/library/base/dainty_base_string_impl.cpp \

ar rcs libdainty.a \
  dainty_sandbox.o \
  dainty_sandbox_impl.o \
  dainty_sandbox_err.o \
  dainty_sandbox_logic_ext_tipc_client.o \
  dainty_sandbox_logic_ext_file.o \
  dainty_sandbox_logic_ext_mqueue_server.o \
  dainty_sandbox_logic_ext_cli.o \
  dainty_sandbox_logic_ext_tipc_server.o \
  dainty_sandbox_logic_ext_inotify.o \
  dainty_sandbox_logic_ext_mqueue_client.o \
  dainty_sandbox_logic_ext_large_msg.o \
  dainty_sandbox_logic_ext_udp_client.o \
  dainty_sandbox_logic_ext_messenger.o \
  dainty_sandbox_logic_ext_tcp_server.o \
  dainty_sandbox_logic_ext_udp_server.o \
  dainty_sandbox_logic_ext_tcp_client.o \
  dainty_messaging.o \
  dainty_messaging_err.o \
  dainty_tracing_err.o \
  dainty_tracing.o \
  dainty_container_any.o \
  dainty_container_bytebuf_impl.o \
  dainty_container_err.o \
  dainty_mt_event.o \
  dainty_mt_net_connect.o \
  dainty_mt_condvar_chained_queue.o \
  dainty_mt_condvar_timed_event.o \
  dainty_mt_waitable_chained_queue.o \
  dainty_mt_detached_thread.o \
  dainty_mt_err.o \
  dainty_mt_command.o \
  dainty_mt_net.o \
  dainty_mt_thread.o \
  dainty_mt_notify_change.o \
  dainty_mt_net_tipc.o \
  dainty_mt_chained_queue.o \
  dainty_mt_timers.o \
  dainty_mt_condvar_event.o \
  dainty_mt_condvar_notify_change.o \
  dainty_mt_event_dispatcher.o \
  dainty_mt_condvar_command.o \
  dainty_os_clock.o \
  dainty_os_call.o \
  dainty_os_threading.o \
  dainty_os_err.o \
  dainty_os_networking.o \
  dainty_os_fdbased.o \
  dainty_os_scheduling.o \
  dainty_os.o \
  dainty_dacli.o \
  dainty_dacli_err.o \
  dainty_oops.o \
  dainty_base_mem_impl.o \
  dainty_base_buf_impl.o \
  dainty_base_string_segmented_impl.o \
  dainty_base_terminal.o \
  dainty_base_assert.o \
  dainty_base_string_impl.o

rm *.o
