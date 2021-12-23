export DAINTY_ROOT=../../../..
g++ -Wall -o example \
    $DAINTY_ROOT/examples/library/oops/dainty_oops_example_except.cpp \
    $DAINTY_ROOT/library/base/dainty_base_assert.cpp \
    $DAINTY_ROOT/library/base/dainty_base_terminal.cpp \
    $DAINTY_ROOT/library/base/dainty_base_string_impl.cpp \
    $DAINTY_ROOT/library/oops/dainty_oops.cpp \
  -I$DAINTY_ROOT/library/base \
  -I$DAINTY_ROOT/library/oops \
    -std=c++17 -lpthread -O2 -ggdb3
