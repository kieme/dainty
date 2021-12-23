export DAINTY_ROOT=../../../..
g++ -Wall -o coffee_machine \
    $DAINTY_ROOT/examples/library/state/dainty_state_coffee_machine.cpp \
    $DAINTY_ROOT/library/base/dainty_base_string_impl.cpp \
    $DAINTY_ROOT/library/base/dainty_base_assert.cpp \
    $DAINTY_ROOT/library/base/dainty_base_terminal.cpp \
  -I$DAINTY_ROOT/library/base \
  -I$DAINTY_ROOT/library/state \
    -std=c++17 -lpthread -O0 -ggdb3 -DCOFFEE_DEBUG -DDAINTY_BASE_RANGE_CHECK
