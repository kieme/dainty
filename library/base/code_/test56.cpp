#include <string>
#include "dainty_base.h"

using namespace std::string_literals
using namespace dainty::base::utility;

int main() {
  int i = py_or(0, 8);

  std::string name = py_or(1, "hallo"s);
  return 0;
}
