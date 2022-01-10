#include <iostream>
#include "dainty_base_logical.h"

using namespace dainty::base::logical;

int main() {
  constexpr t_n n1 = t_n{1};
  constexpr t_n n2 = t_max_n{2};
  if (n1 < n2)
    std::cout << "seems to work" << std::endl;

  if (n2 > n1)
    std::cout << "seems to work" << std::endl;

  constexpr auto n3 = n1 + n2;

  if (n3 == t_min_n{0})
    std::cout << "seems to work" << std::endl;

  return 0;
}
