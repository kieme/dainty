/******************************************************************************

 MIT License

 Copyright (c) 2018 kieme, frits.germs@gmx.net

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.

******************************************************************************/

#include <iostream>
#include <array>
#include <vector>
#include <limits>
#include "dainty_base.h"

using namespace dainty;

///////////////////////////////////////////////////////////////////////////////

void set_point(unsigned short x, unsigned short y) {
  std::cout << "1. x:" << x << ", y:" << y << std::endl;
}

///////////////////////////////////////////////////////////////////////////////
// step1. create explicit types

enum t_x_ {};
using t_xaxis = base::specific::t_specific<base::t_ushort, t_x_>;

enum t_y_ {};
using t_yaxis = base::specific::t_specific<base::t_ushort, t_y_>;

// step2. overload operators for new types
constexpr t_xaxis operator "" _xaxis(unsigned long long int value) {
  // can throw if out of bounds - don't use braced initialization.
  return t_xaxis(value);
}

constexpr t_yaxis operator "" _yaxis(unsigned long long int value) {
  // can throw if out of bounds - don't use braced initialization.
  return t_yaxis(value);
}

inline std::ostream& operator<<(std::ostream& out, const t_xaxis& xaxis) {
  return (out << "x:" << get(xaxis));
}

inline std::ostream& operator<<(std::ostream& out, const t_yaxis& yaxis) {
  return (out << "y:" << get(yaxis));
}

// step3. rewrite set_point with new types
void set_point(t_xaxis x, t_yaxis y) {
  std::cout << "2. " << x << ", " << y << std::endl;
}

///////////////////////////////////////////////////////////////////////////////

using base::t_uint;
using base::t_ix;
using base::t_bix;
using base::t_eix;
using base::t_n;
using base::to_ix;
using base::to_bix;
using base::to_eix;
using base::to_n;

constexpr t_ix maxix{10};
constexpr t_ix minix{0};

using t_arr = std::array<int, get(to_n(maxix))>;

///////////////////////////////////////////////////////////////////////////////
// difference between indexing and number of elemen

template<typename T>
void print(t_bix begin, t_eix end, const T& arr) {
  for (auto ix = get(begin), eix = get(end); ix != eix; ++ix)
    std::cout << arr[ix];
  std::cout << std::endl;
}

template<typename T>
inline void print(t_ix pos, t_n n, const T& arr) {
  print(to_bix(pos), t_eix{get(pos) + get(n)}, arr);
}

///////////////////////////////////////////////////////////////////////////////

enum t_vlanid_tag_ { };
using t_vlanid_ = base::t_uint16;
struct t_validate_vlanid_ {
  static constexpr t_vlanid_ check(t_vlanid_ value) {
    if (value < 1 || value > 4089)
      throw std::range_error("vlanid range error");
    return value;
  }
};
using t_vlanid=base::specific::t_specific<t_vlanid_, t_vlanid_tag_, t_validate_vlanid_>;

constexpr t_vlanid operator"" _vlanid(base::t_ullong value) {
  return t_vlanid(value);
}

///////////////////////////////////////////////////////////////////////////////

base::P_cstr p{"hallo"};

///////////////////////////////////////////////////////////////////////////////

int main() {
  set_point(1,2);
  set_point(t_xaxis(1), t_yaxis(2));
  set_point(1_xaxis, 2_yaxis);

  std::cout << 7_xaxis << std::endl;

  //set_point(t_xaxis{-1}, t_yaxis{2}); // can detect narrowing
  set_point(t_xaxis{1}, t_yaxis{2});

  auto three = t_n{3};

  t_arr arr = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
  print(t_ix{1}, three, arr);

  std::vector<t_uint> vect{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  print(t_ix{0}, three, vect);

  t_ix ix = to_ix(three);

  constexpr auto vlan500{500_vlanid};

  std::cout << get(300_vlanid) << std::endl;

  t_ix uix{5};
  set(uix) = 10;

  constexpr auto ptr = base::mk_cstr("hallo");
  std::cout << "ptr = " << get(ptr) << std::endl;

  char buf[] = "hallo";
  base::p_cstr str1(buf);
  base::P_cstr str2(str1);

  return 0;
}
