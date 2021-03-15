g++ -I/home/fgerms/Projects/catch2.git/include -Wall -Wpedantic -Wextra \
  -ggdb3 -std=c++2a -O2 -o run main.cpp \
   -Iinternal_/ \
   -Itypes/ \
   -Ibuf/ \
   -Ivaluestore/ \
   -Irange/ \
   -Itraits/ \
   -Istring/ \
   -Istring/segmented/ \
   -Istring/stream/ \
   -Ilogical/ \
   -Ispecific/ \
   -Iptr/ \
   -Inumeric/ \
   -Iassert/ \
   -Iterminal/ \
   -Iutil/ \
  ./string/segmented/dainty_base_string_segmented_impl.cpp \
  ./string/dainty_base_string_impl.cpp \
  ./numeric/dainty_base_numeric_impl.cpp \
  ./terminal/dainty_base_terminal.cpp \
  ./assert/dainty_base_assert.cpp \
  ./range/dainty_base_range_impl.cpp \
  ./buf/dainty_base_buf_impl.cpp
