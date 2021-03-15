#include "dainty_base_range.h"

using namespace dainty::base::types;
using namespace dainty::base;
using namespace dainty::base::range;

enum tag1_ { };
enum tag2_ { };
enum tag3_ { };

t_int main() {
  t_char buf[100];
  auto range = mk_range<tag1_>(buf);
  range.copy(t_block<t_char, tag2_>{'-', t_n{100}});

  t_range<t_char, tag2_> range1 = mk_range<tag2_>(buf, t_n{10});
  if (range == range1.mk_range<tag1_>())
    return 1;

  return 0;
}
