#include <iostream>
#include <limits>
#include <exception>
#include "dainty_base_logical.h"

using namespace dainty::base::types;
using namespace dainty::base::logical;
using namespace dainty::base::traits;

enum t1_ { };
using t_hallo = t_logical<p_void, t1_>;


enum t2_ { };
using t_val1 = t_logical<t_int, t2_>;

enum t3_ { };
using t_val2 = t_logical<t_short, t3_>;

enum t4_ { };
using t_val3 = t_logical<t_double, t4_>;

int main() {
  t_hallo hallo{nullptr};
  t_val1 val1{0};
  t_val2 val2{-1};
  t_val3 val3{10.1};

  auto max = max_of(val1, val2, val3);

  return 0;
}
