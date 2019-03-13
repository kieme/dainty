export DAINTY_ROOT=../../../..
g++ -Wall -o example \
    $DAINTY_ROOT/examples/library/oops/dainty_oops_example.cpp \
    $DAINTY_ROOT/library/named/dainty_named_assert.cpp \
    $DAINTY_ROOT/library/named/dainty_named_terminal.cpp \
    $DAINTY_ROOT/library/named/dainty_named_string_impl.cpp \
    $DAINTY_ROOT/library/oops/dainty_oops.cpp \
  -I$DAINTY_ROOT/library/named \
  -I$DAINTY_ROOT/library/oops \
    -std=c++17 -lpthread -O2 -ggdb3
