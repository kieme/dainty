#include <iostream>
#include "dainty_base_traits.h"

using namespace dainty::base::traits;

template<typename T, typename T1,
         t_if<t_and<t_is_unsigned_integral<T>,
                    t_is_unsigned_integral<T1>>> = YES>
constexpr
bool is_same_sign(T, T1) noexcept {
  return true;
}

template<typename T, typename T1,
         t_if<t_or<t_is_signed_integral<T>,
                   t_is_signed_integral<T1>>> = YES>
constexpr
bool is_same_sign(T value, T1 value1) noexcept {
  return (value < 0 && value1 < 0) || (value >= 0 && value1 >= 0);
}

int main() {
  std::cout << "same sign (-1,  -1LL) = " << is_same_sign(-1, -1LL)  << std::endl;
  std::cout << "same sign (1,   -1LL) = " << is_same_sign(1, -1LL)   << std::endl;
  std::cout << "same sign (1UL, -1LL) = " << is_same_sign(1UL, -1LL) << std::endl;
  std::cout << "same sign (1UL, 1ULL) = " << is_same_sign(1UL, 1ULL) << std::endl;
  return 0;
}
