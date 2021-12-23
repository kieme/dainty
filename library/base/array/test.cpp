#include <iostream>
#include "dainty_base_array.h"

template<typename T>
class t_test;

using namespace dainty::base::array;

int main()
{

  auto arr0 = make_array<6, int>();
  auto arr  = make_array<6>(1,2,3);
  auto arr1 = make_array(1.1,2,3.1);

  arr1[2] = 3.3;

  for (auto a : arr)
    std::cout << "a = " << a << std::endl;

  for (auto a : arr1)
    std::cout << "a = " << a << std::endl;

  auto arr2 = arr;

  auto arr4 = make_array_no_init<10, int>();

  for (auto i = arr.RBegin(); i != arr.REnd(); --i) {
    std::cout << "i = " << *i << std::endl;
  }

  arr1 = make_array(1, 2, 3);

  for (auto i = arr1.Begin(); i != arr1.End(); ++i) {
    std::cout << "i = " << *i << std::endl;
  }

  char b[10] = { 'a', 'b', };
  auto arr5 = make_array(b);

  for (auto i = arr5.Begin(); i != arr5.End(); ++i) {
    std::cout << "i = " << *i << std::endl;
  }

  auto arr8 = make_array<3, int*>();

  arr8.init(INIT_LIST, arr.ptr(0), arr.ptr(1), arr.ptr(2), arr.ptr(2));

  for (auto i = arr8.Begin(); i != arr8.End(); ++i) {
    std::cout << "i = " << *i << std::endl;
  }

  return 0;
}
