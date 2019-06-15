#include <iostream>
#include "dainty_container_bits.h"

using namespace dainty::container::bits;

int main() {
  t_bits<32> bits;

  bits.set(t_bit{3}, BIT_ON);
  if (bits[t_bit{3}] == BIT_ON)
    std::cout << "hello" << std::endl;

  bits.each_of(BIT_ON,
              [](auto bit, auto state) {
     std::cout << "at bit-" << get(bit) << " is state = " << state << std::endl;
    }
  );

  return 0;
}
