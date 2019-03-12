export DAINTY_ROOT=../../../..
g++ -Wall -o coffee_machine \
    $DAINTY_ROOT/examples/library/state/dainty_state_coffee_machine.cpp \
  -I$DAINTY_ROOT/library/named \
  -I$DAINTY_ROOT/library/state \
    -std=c++17 -lpthread -O2 -ggdb3
