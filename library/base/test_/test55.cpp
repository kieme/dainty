#include <iostream>
#include <type_traits>
#include "dainty_base_traits.h"

using namespace dainty::base::traits;

struct t_a {
};

struct t_b {
  using t_value = int;
  typedef int t_result;
  t_value check(t_value value) { return value; }
};

template<typename F>
using t_function = typename t_if_same<
  decltype(uneval<F>().check(typename F::t_value())),
  typename F::t_value
  >::t_value;

class A {
  enum t_e1_ { E1_ };
  enum t_e2_ { E2_ };

public:
  template<typename T,
           typename t_if_then<t_is_bool<T>, t_e1_>::t_value = E1_>
  A(T value) {
    std::cout << "hallo world" << std::endl;
  }

  template<typename T,
           typename t_if_then<t_is_same<T, char>, t_e2_>::t_value = E2_>
  A(T value) {
    std::cout << "bye world" << std::endl;
  }
};

int main() {
  static_assert(t_has_result<t_b>::VALUE == true,  "t_b has no result");
  static_assert(t_has_result<t_a>::VALUE == false, "t_a has a result");
  static_assert(t_is_there<t_function, t_b>::VALUE == true, "t_b does not have function");

  A a('a');
  A a1(true);

  return 0;
}
