#include <iostream>

#include "dainty_base_types.h"
#include "dainty_base_ptr.h"
#include "dainty_base_string_segmented.h"
#include "dainty_base_string_stream_out.h"
#include "dainty_base_string_stream_in.h"
#include "dainty_base_string.h"
#include "dainty_base_numeric.h"
#include "dainty_base_terminal.h"
#include "dainty_base_assert.h"
#include "dainty_base_specific.h"
#include "dainty_base_range.h"
#include "dainty_base_util.h"
#include "dainty_base_buf.h"
#include "dainty_base_valuestore.h"
#include "dainty_base_traits.h"
#include "dainty_base_logical.h"
#include "dainty_base.h"

using namespace dainty::base::traits;
using namespace dainty::base::valuestore;
using namespace dainty::base::logical;

class A {
public:
  int value = 1;
};

class A1 {
public:
  A1()            : value{3}       { std::cout << "A1::A1()\n";     }
  A1(int i)       : value{i}       { std::cout << "A1::A1(int)\n";  }
  A1(const A1& a) : value{a.value} { std::cout << "A1::A1(R_A1)\n"; }
 ~A1()                             { std::cout << "A1::~A1()\n";    }

  A1& operator=(const A1& a) {
    value = a.value; std::cout << "A1::A1=(R_A1)\n";
    return *this;
  }

  int value = 1;
};

class A2 {
public:
  A2()            : value{3}       { std::cout << "A2::A2()\n";     }
  A2(int i)       : value{i}       { std::cout << "A2::A2(int)\n";  }
  A2(const A2& a) : value{a.value} { std::cout << "A2::A2(R_A2)\n"; }
  A2(A2&& a)      : value{a.value} { std::cout << "A2::A2(x_A2)\n"; }
 ~A2()                             { std::cout << "A2::~A2()\n";    }

  A2& operator=(const A2& a) {
    value = a.value; std::cout << "A1::A1=(R_A1)\n";
    return *this;
  }

  A2& operator=(A2&& a) {
    value = a.value; std::cout << "A1::A1=(x_A1)\n";
    return *this;
  }

  int value = 1;
};

enum t_tag1_ { };

struct C {
  using t_tag = t_logical_tag<t_tag1_>;
};

///////////////////////////////////////////////////////////////////////////////

template<typename TAG>
struct t_logical_base : private TAG {
  template<typename TAG1> friend struct t_logical_base;

  t_logical_base() : TAG {} { }

  template<typename TAG1>
  t_logical_base(const t_logical_base<TAG1>& d) : TAG(d) {
  }

  using t_logical = TAG;
};

///////////////////////////////////////////////////////////////////////////////

enum t_tag2_ { };
enum t_tag3_ { };
enum t_tag4_ { };

using D_logical_ = t_logical_base<t_tag2_>;
using D_base_    = t_logical_base<t_tag2_>;
using E_base_    = t_logical_base<t_tag3_, D_logical_>;
using F_base_    = t_logical_base<t_tag4_, E_logical_>;

///////////////////////////////////////////////////////////////////////////////

template<typename TAG>
struct D : t_logical_base<TAG> {
  D() = default;

  D(const D& d)                               : t_logical_base<TAG>(d) { }
  template<typename TAG1> D(const D<TAG1>& d) : t_logical_base<TAG>(d) { }
};

///////////////////////////////////////////////////////////////////////////////

/*
template<typename T>
using t_has_logical_tag = t_if_logical<typename T::t_tag>;

template<typename T>
using t_has_logical_ops = t_if_logical<typename T::t_ops>;

template<typename T, typename U,
         t_if<t_and<t_is_there<t_has_logical_tag, T>,
                    t_is_there<t_has_logical_ops, T>>> = WELL_FORMED>
T& operator<<(T& out, U) {
  std::cout << "This worked" << std::endl;
  return out;
}
*/

///////////////////////////////////////////////////////////////////////////////

int main() {
  C c;
  //c << 1;

  std::cout << "--------------------------------" << std::endl;

  {
    t_valuestore<t_ix> i;

    std::cout << "value of i = " << get(i.ref()) << std::endl;

    i = t_ix{3};

    std::cout << "value of i = " << get(i.ref()) << std::endl;

    i.emplace_construct(2);

    std::cout << "value of i = " << get(i.ref()) << std::endl;
  }

  std::cout << "--------------------------------" << std::endl;

  {
    t_valuestore<int> i;

    std::cout << i.default_construct()      << std::endl;

    std::cout << "value of i = " << i.ref() << std::endl;

    i = 3;

    std::cout << "value of i = " << i.ref() << std::endl;

    i.emplace_construct(2);

    std::cout << "value of i = " << i.ref() << std::endl;
  }

  std::cout << "--------------------------------" << std::endl;

  {
    t_valuestore<A> i;

    std::cout << i.default_construct().value      << std::endl;

    std::cout << "value of i = " << i.ref().value << std::endl;

  }

  std::cout << "--------------------------------" << std::endl;

  {
    t_valuestore<A1> i;

    std::cout << i.default_construct().value      << std::endl;

    std::cout << "value of i = " << i.ref().value << std::endl;

    i.destruct().copy_construct(A1{4});

    std::cout << "value of i = " << i.ref().value << std::endl;

    i.destruct().move_construct(A1{5});

    std::cout << "value of i = " << i.ref().value << std::endl;

    i.destruct().emplace_construct(7);

    std::cout << "value of i = " << i.ref().value << std::endl;
    i = A1{10};

    std::cout << "value of i = " << i.ref().value << std::endl;
    i.destruct();
  }

  std::cout << "--------------------------------" << std::endl;

  {
    t_valuestore<A2> i;

    std::cout << i.default_construct().value      << std::endl;

    std::cout << "value of i = " << i.ref().value << std::endl;

    i.destruct().copy_construct(A2{4});

    std::cout << "value of i = " << i.ref().value << std::endl;

    i.destruct().move_construct(A2{5});

    std::cout << "value of i = " << i.ref().value << std::endl;

    i.destruct().emplace_construct(7);

    std::cout << "value of i = " << i.ref().value << std::endl;
    i = A2{10};

    std::cout << "value of i = " << i.ref().value << std::endl;
    i.destruct();

    t_valuestore<A2> j(EMPLACE, 16);
    std::cout << "value of j = " << j.ref().value << std::endl;
    j.destruct();
  }

  t_valuestore<int> k =1;
  return 0;
}

