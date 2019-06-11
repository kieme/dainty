#include <iostream>
#include "dainty_container_bits.h"

using namespace dainty::container::bits;

int main() {
  t_bits<32> bits{BIT_ON};

  bits.each([](auto bit, auto state) {
    std::cout << "at bit-" << get(bit) << " is state = " << std::endl;
  });

  return 0;
}
