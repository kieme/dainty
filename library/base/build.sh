g++ -I/home/fgerms/Projects/catch2.git/include -Wall -Wpedantic -Wextra \
  -ggdb3 -std=c++2a -O2 -o run test4.cpp \
   -Iinternal_/ \
   -Itypes/ \
   -Ibuf/ \
   -Ivaluestore/ \
   -Irange/ \
   -Itraits/ \
   -Istring/ \
   -Ilogical/ \
   -Ispecific/ \
   -Iptr/ \
   -Inumeric/ \
   -Iassert/ \
   -Iterminal/ \
   -Iutil

