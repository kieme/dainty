export DAINTY_ROOT=../../../..
g++ -Wall -o see_simple_mechaniscs \
    $DAINTY_ROOT/examples/library/state/dainty_state_see_simple_mechanics.cpp \
  -I$DAINTY_ROOT/library/named \
  -I$DAINTY_ROOT/library/state \
    -std=c++17 -lpthread -O2 -ggdb3
