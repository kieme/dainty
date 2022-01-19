#include <iostream>
#include "dainty_array.h"

using namespace dainty::array;

struct t_test {
  t_test() {
    std::cout << "t_test()" << std::endl;
  }

  t_test(int _value) : value (_value) {
    std::cout << "t_test(" << value << ")" << std::endl;
  }

  t_test(const t_test& test) {
    value = test.value;
    std::cout << "t_test(R_test-" << value << ")" << std::endl;
  }

  t_test(t_test&& test) {
    value = test.value;
    std::cout << "t_test(x_test-" << value << ")" << std::endl;
  }

  t_test& operator=(const t_test& test) {
    value = test.value;
    std::cout << "t_test=(R_test-" << value << ")" << std::endl;
    return *this;
  }

  t_test& operator=(t_test&& test) {
    value = test.value;
    std::cout << "t_test=(x_test-" << value << ")" << std::endl;
    return *this;
  }

  ~t_test() {
    std::cout << "~t_test(" << value << ")" << std::endl;
  }

  int value = 0;
};

void answer(const int (&ans)[5]) {
  for (int i = 0; i < 5; ++i)
    std::cout << "ans[" << i << "] = " << ans[i] << std::endl;
}

void __attribute__((noinline)) foo(int (&ref)[5], int (&ref1)[5]) {
  int ans[5];
  for (int i = 0; i < 5; ++i)
    ans[i] = ref[i] + ref1[i];
  answer(ans);
}

void __attribute__((noinline)) foo1(int (&ref)[5], int (&ref1)[5]) {
  answer(make_array(INIT_FARGS, ref[0] + ref1[0],
                               ref[1] + ref1[1],
                               ref[2] + ref1[2],
                               ref[3] + ref1[3],
                               ref[4] + ref1[4]).Get());
}

int main()
{

  auto arr0 = make_array<6, int>();
  auto arr  = make_array<6>(INIT_FARGS, 1,2,3);
  auto arr1 = make_array(INIT_FARGS, 1.1,2,3.1);

  arr1[2] = 3.3;

  for (auto a : arr)
    std::cout << "a = " << a << std::endl;

  for (auto a : arr1)
    std::cout << "a = " << a << std::endl;

  auto arr2 = arr;

  int buf[6] { 0, };

  if (arr2 == buf)
    std::cout << "same values" << std::endl;

  auto arr4 = make_array<10, int>(INIT_NONE);

  for (auto i = arr.RBegin(); i != arr.REnd(); --i) {
    std::cout << "i = " << *i << std::endl;
  }

  arr1 = make_array(INIT_FARGS, 1, 2, 3);

  for (auto i = arr1.Begin(); i != arr1.End(); ++i) {
    std::cout << "i = " << *i << std::endl;
  }

  char b[10] = { 'a', 'b', };
  auto arr5 = make_array(b);

  for (auto i = arr5.Begin(); i != arr5.End(); ++i) {
    std::cout << "i = " << *i << std::endl;
  }

  auto arr8 = make_array<3, int*>();

  arr8.init(INIT_FARGS, arr.ptr(0), arr.ptr(1), arr.ptr(2));

  for (auto i = arr8.Begin(); i != arr8.End(); ++i) {
    std::cout << "i = " << *i << std::endl;
  }

  t_array<10, int> arr9(INIT_CUSTOM, [](auto ix, auto ptr) {
        *ptr = ix;
      });

  for (auto i = arr9.Begin(); i != arr9.End(); ++i) {
    std::cout << "i = " << *i << std::endl;
  }

  arr9.Each([](auto ix, auto& val) {
    std::cout << ix << " = " << val << std::endl;
    });

  /*
  t_array<4, t_test> arr10;
  */
  /*
  t_array<4, t_test> arr11(INIT_CUSTOM, [](auto ix, auto ptr) {
      new (ptr) t_test(ix);
      });
  */

  //t_array<4, t_test> arr10(INIT_NONE);

  auto custom = [](auto ix, auto ptr) { new (ptr) t_test(ix); };

  t_array<4, t_test> arr11(INIT_CUSTOM, custom);
  auto arr13 = arr11;

  arr13 = arr11;

  arr11.destruct();
  arr11.init(INIT_FARGS, 1, 2, 3, 4);

  int i[5] = { 0, 1, 2, 3, 4};

  foo(i, i);
  foo1(i, i);

  t_array<10, t_array<10, t_array<2, char>>> arr12;
  arr12[2][1][0] = 'a';

  return 0;
}
