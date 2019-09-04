export DAINTY_ROOT=../..
g++ -Wall \
  -I$DAINTY_ROOT/sandbox \
  -I$DAINTY_ROOT/sandbox/logic_extensions \
  -I$DAINTY_ROOT/library/base \
  -I$DAINTY_ROOT/library/mt \
  -I$DAINTY_ROOT/library/os \
  -I$DAINTY_ROOT/library/oops \
  -I$DAINTY_ROOT/library/state \
  -I$DAINTY_ROOT/library/dacli \
  -I$DAINTY_ROOT/library/container \
  -I$DAINTY_ROOT/service/tracing \
  -I$DAINTY_ROOT/service/messaging \
  -std=c++17 -lpthread -O2 -fsanitize=address -pg -ggdb3 \
  -c dainty_foo.cpp
