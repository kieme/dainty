#include <utility>
#include "dainty_named_utility.h"
#include "dainty_named_terminal.h"

using namespace dainty::named::terminal;
using dainty::named::x_cast;
using dainty::named::preserve;

struct A {
  A()                     { t_out{"A()"}; }
  A(int)                  { t_out{"A(int)"}; }
  A(char&)                { t_out{"A(char&)"}; }
  A(const A&)             { t_out{"A(const A&)"}; }
  A(A&&)                  { t_out{"A(A&&)"}; }
  A& operator=(const A&)  { t_out{"A& operator=(const A&)"}; return *this; }
  A& operator=(A&&)       { t_out{"A& operator=(A&&)"};      return *this; }
  ~A()                    { t_out{"~A()"}; }
};

A __attribute__((noinline)) foo() {
  A a;
  t_out{"foo()"};
  return a;
}

struct B_1 {
  template<typename...Args>
  B_1(Args&&... args) : a_{std::forward<Args>(args)...} {
  }
  A a_;
};

struct B_2 {
  template<typename...Args>
  B_2(Args&&... args) : a_{preserve<Args>(args)...} {
  }
  A a_;
};

void check(int& i) {
  t_out{"check(int&)"};
  i = 1;
}
void check(int&&) {
  t_out{"check(int&&)"};
}

template<typename T>
void check_1(T&& value) {
  check(std::forward<T>(value));
}

template<typename T>
void check_2(T&& value) {
  check(preserve<T>(value));
}

int main() {

  t_out{"Block1"};
  {
    A a;
    A a1{a};
    A a2{foo()};

    a1 = std::move(5);
    a2 = a1;
  }

  t_out{"Block2"};
  {
    A a;
    A a1{a};
    A a2{foo()};

    a1 = x_cast(5);
    a2 = a1;
  }

  t_out{"Block3"};
  {
    B_1 b{};
    B_1 b1{1};
    B_1 b2{std::move(b1)};

    char ch;
    B_1 b3{ch};
  }

  t_out{"Block4"};
  {
    B_2 b{};
    B_2 b1{1};
    B_2 b2{x_cast(b1)};

    char ch;
    B_2 b3{ch};
  }

  {
    check_1(5);
    check_2(5);

    int i = 5;
    check_1(i);
    check_2(i);

    check(std::forward<int&>(i));
    check(preserve<int&>(i));
  }

  return 0;
}
