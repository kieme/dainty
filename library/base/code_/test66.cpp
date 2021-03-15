#include "dainty_base_traits.h"

using namespace dainty::base::traits;

template<typename T, t_if_each_is_true<t_is_integer, T> = YES>
constexpr
unsigned long long int add(T value) {
  return value;
}

template<typename T, typename... Ts,
         t_if_each_is_true<t_is_integer, T, Ts...> = YES>
constexpr
unsigned long long int add(T value, Ts... values) {
  return value + add(values...);
}

template<typename T, t_if_each_is_true<t_is_precision, T> = YES>
constexpr
long double add(T value) {
  return value;
}

template<typename T, typename... Ts,
         t_if_each_is_true<t_is_precision, T, Ts...> = YES>
constexpr
long double add(T value, Ts... values) {
  return value + add(values...);
}

/*
template<typename T,
         t_if<t_and<t_each_is_false<t_is_precision, T>,
                    t_each_is_false<t_is_integer, T>>> = YES>
constexpr
char add(T value) {
  return value;
}

template<typename T, typename... Ts,
         t_if<t_and<t_each_is_false<t_is_precision, T, Ts...>,
                    t_each_is_false<t_is_integer,  T, Ts...>>> = YES>
constexpr
char add(T value, Ts... values) {
  return value + add(values...);
}
*/



template<typename T>
constexpr
char add(T value) {
  return value;
}

template<typename T, typename... Ts>
constexpr
char add(T value, Ts... values) {
  return value + add(values...);
}

template<typename T, int N, typename T1, int N1,
         t_if<t_and<t_bool_result<sizeof(T) == sizeof(T1)>,
                    t_bool_result<(N1 <= N)>>> = YES>
void cpy(T (&value)[N], T1 (&value1)[N1]) {
  for (auto i = 0; i < N1; ++i)
    value[i] = value1[i];
}

template<typename T, int N, typename T1, int N1,
         t_if<t_and<t_bool_result<sizeof(T) == sizeof(T1)>,
                    t_bool_result<(N1 > N)>>> = YES>
void cpy(T (&value)[N], T1 (&value1)[N1]) {
  for (auto i = 0; i < N; ++i)
    value[i] = value1[i];
}

int main() {
  constexpr auto sum1 = add(1, 20UL, -17LL);
  constexpr auto sum2 = add(2.1, 2.1, 3.78, 21.9f);
  constexpr auto sum3 = add('a', 'b', 'c');

  {
    char j[10];
    char k[10];
    cpy(j, k);
  }

  {
    char j[15];
    char k[10];
    cpy(j, k);
  }

  {
    char j[5];
    char k[10];
    cpy(j, k);
  }

  return 0;
}
