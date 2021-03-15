#include <iostream>
#include "dainty_base_valuestore.h"

using namespace dainty::base;
using namespace dainty::base::valuestore;

struct A_impl;
struct A {
  A();
  A(const A&);
  A(A&&);
  ~A();
  t_store<32, 1> impl_store_;
  A_impl*        impl_ = nullptr;
};

int main() {
  A a;
  A a1{a};
  A a2{x_cast(a)};

  return 0;
}

///////////////////////////////////////////////////////////////////////////////

struct A_impl {
  A_impl()              { std::cout << "A_impl::A_impl()" << std::endl; }
  A_impl(const A_impl&) { std::cout << "A_impl::A_impl(const A&)" << std::endl; }
  A_impl(A_impl&&     ) { std::cout << "A_impl::A_impl(A&&)" << std::endl; }
  ~A_impl()             { std::cout << "A_impl::~A_impl()" << std::endl; }
  alignas(1) char buf[32];
};

A::A() : impl_{default_construct<A_impl>(impl_store_)} {
}

A::A(const A& a) : impl_{copy_construct<A_impl>(impl_store_, *a.impl_)} {
}

A::A(A&& a) : impl_{move_construct<A_impl>(impl_store_, x_cast(*a.impl_))} {
}

A::~A() {
  destruct<A_impl>(impl_store_);
}

