g++ -Wall -o run dainty_sandbox_example.cpp \
                 dainty_sandbox.cpp         \
                 dainty_sandbox_err.cpp     \
                 dainty_sandbox_impl.cpp    \
                 logic_extensions/dainty_sandbox_logic_ext_cli.cpp \
                 logic_extensions/dainty_sandbox_logic_ext_large_msg.cpp \
                 ../library/named/dainty_named_assert.cpp \
                 ../library/named/dainty_named_range.cpp  \
                 ../library/named/dainty_named_terminal.cpp \
                 ../library/named/dainty_named_string_impl.cpp \
                 ../library/oops/dainty_oops.cpp \
                 ../library/mt/dainty_mt_detached_thread.cpp \
                 ../library/mt/dainty_mt_event_dispatcher.cpp \
                 ../library/mt/dainty_mt_err.cpp  \
                 ../library/os/dainty_os_err.cpp \
                 ../library/os/dainty_os_call.cpp \
                 ../library/os/dainty_os_threading.cpp  \
                 ../library/os/dainty_os_fdbased.cpp \
                 ../library/os/dainty_os_clock.cpp \
                 ../library/container/dainty_container_err.cpp \
                 -I../library/named \
                 -I../library/mt \
                 -I../library/os \
                 -I../library/oops \
                 -I../library/container \
                 -I./logic_extensions \
                 -I./ \
                 -std=c++17 -lpthread -O0 -ggdb3
