#include <iostream>
#include <chrono>
#include "dainty_os_clock.h"

void foo() {
  using dainty::os::clock::t_timer;
  using dainty::os::clock::t_time;
  using dainty::os::clock::t_nsec;
  using dainty::os::clock::t_usec;

  t_timer timer(t_timer::START);
  std::cout << std::endl;
  t_time elapsed = timer.elapsed();

  std::cout << "nanoseconds  = " << get(elapsed.to<t_nsec>()) << std::endl;
  std::cout << "microseconds = " << get(elapsed.to<t_usec>()) << std::endl;
}

void boo() {
  auto start = std::chrono::system_clock::now();
  std::cout << std::endl;
  auto end = std::chrono::system_clock::now();

  auto elapsed = std::chrono::nanoseconds(end - start);
  std::cout << "elapsed nanoseconds = " << elapsed.count() << std::endl;
}

int main() {
  std::cout << "load dynamic symbols" << std::endl;
  boo();
  foo();
  return 0;
}
