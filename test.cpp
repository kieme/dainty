#include <iostream>
#include "dainty_base_traits.h"
#include "dainty_base_valuestore.h"

using namespace dainty::base;
using namespace dainty::base::valuestore;

t_union_store<long double, short> store_;

int main() {
  std::cout << "alignof = " << alignof(store_) << std::endl
            << "sizeof  = " << sizeof(store_)  << std::endl;

  copy_construct<bool>(store_, true);

  return 0;
}
