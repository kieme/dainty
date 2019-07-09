export DAINTY_ROOT=../../../..
g++ -Wall -o dainty_dacli_example \
    $DAINTY_ROOT/examples/library/dacli/dainty_dacli_example.cpp \
    $DAINTY_ROOT/library/oops/dainty_oops.cpp \
    $DAINTY_ROOT/library/dacli/dainty_dacli.cpp \
    $DAINTY_ROOT/library/dacli/dainty_dacli_err.cpp \
    $DAINTY_ROOT/library/base/dainty_base_string_impl.cpp \
    $DAINTY_ROOT/library/base/dainty_base_assert.cpp \
    $DAINTY_ROOT/library/base/dainty_base_terminal.cpp \
  -I$DAINTY_ROOT/library/base \
  -I$DAINTY_ROOT/library/oops \
  -I$DAINTY_ROOT/library/dacli \
    -std=c++17 -lpthread -O0 -ggdb3 -DCOFFEE_DEBUG -DDAINTY_BASE_RANGE_CHECK
