export DAINTY_ROOT=../../../..
g++ -Wall -o dainty_container_bits_example \
    $DAINTY_ROOT/examples/library/container/dainty_container_bits_example.cpp \
    $DAINTY_ROOT/library/named/dainty_named_string_impl.cpp \
    $DAINTY_ROOT/library/named/dainty_named_assert.cpp \
    $DAINTY_ROOT/library/named/dainty_named_terminal.cpp \
    $DAINTY_ROOT/library/container/dainty_container_err.cpp \
    $DAINTY_ROOT/library/oops/dainty_oops.cpp \
  -I$DAINTY_ROOT/library/named \
  -I$DAINTY_ROOT/library/state \
  -I$DAINTY_ROOT/library/oops \
  -I$DAINTY_ROOT/library/container \
    -std=c++17 -lpthread -O0 -ggdb3 -DDAINTY_NAMED_RANGE_CHECK
