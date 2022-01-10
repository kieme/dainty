#include <iostream>
#include <limits>
#include <utility>
#include "dainty_base_numeric.h"
#include "dainty_base_traits.h"

using namespace dainty::base::numeric;

using dainty::base::logical::t_ix;
using dainty::base::logical::t_ix_;
using dainty::base::logical::t_n;
using dainty::base::logical::t_n_;
using dainty::base::logical::get;

void display(impl_::R_store_ buf) {
  auto info = impl_::bits_info_(buf);
  std::cout << "max_bits = "   << get(info.max_bits)
            << ", free bits = "  << get(info.free_bits())
            << ", msb on bit = " << get(info.msb_on.bits())
            << "    ";
  impl_::display_bits_(buf, false);
}

using dainty::base::traits::t_and;
using dainty::base::traits::t_if;
using dainty::base::traits::t_none_is_true;
using dainty::base::traits::t_is_void;
using dainty::base::traits::uneval;
using dainty::base::traits::t_if_there;
using dainty::base::traits::t_if_not_there;
using dainty::base::traits::WELL_FORMED;

struct A {
 using t_ch = t_and<t_none_is_true<A, t_is_void>>;
 using t_val = int;
 t_ch check(int);
 int i = 0;
};

struct A1 {
 int i = 0;
};

struct B {
  template<typename T>
  using has_check =
    decltype(uneval<T>().check(typename T::t_val()));

  template<typename T, t_if_there<has_check, T> = WELL_FORMED>
  B(T value) { std::cout << "Number 1" << value.i << std::endl; }

  template<typename T, t_if_not_there<has_check, T> = WELL_FORMED>
  B(T value) { std::cout << "Number 2" << value.i << std::endl; }
};

int main() {
  A a;
  B b(a);

  A1 a1;
  B b1(a1);

  t_binary bin;

  bin = (impl_::t_pvalue_)1;
  bin = (impl_::t_pvalue_)'a';
  bin = (impl_::t_nvalue_)-1;

  bool on  = bin[t_bit{63}];
  bool on1 = bin[t_bit{126}];

  bin.set_bit(t_bit{63},  false);
  bin.set_bit(t_bit{126}, false);

  t_ix_ max = get(bin.get_bits());
  for (t_ix_ ix = 0; ix < max; ++ix)
    std::cout << "bit[" << ix << "] = " << bin[t_bit(ix)] << std::endl;

  t_binary bin1((impl_::t_pvalue_)1);
  bin1 + (impl_::t_pvalue_)1;

  bin + bin1;

//  t_ix_ max = get(bin1.get_bits());
//  for (t_ix_ ix = 0; ix < max; ++ix)
//    std::cout << "bit[" << ix << "] = " << bin[t_ix(ix)] << std::endl;

  t_binary  bin2((impl_::t_pvalue_)0xFF10F); // XXX WHY
  t_binary  bin3 = ~bin2;
  t_binary  bin4 = -bin2;
  t_integer int1 = bin4;

  impl_::t_store_ buf;

  {
    auto value = 1;
    impl_::assign_(buf, static_cast<impl_::t_pvalue_>(value));
    display(buf);

    shift_left_(buf, t_bits{40});
    display(buf);

    shift_right_(buf, t_bits{1}); // 39
    display(buf);
    shift_right_(buf, t_bits{1}); // 38
    display(buf);
    shift_right_(buf, t_bits{1}); // 37
    display(buf);
    shift_right_(buf, t_bits{1}); // 36
    display(buf);
    shift_right_(buf, t_bits{1}); // 35
    display(buf);
    shift_right_(buf, t_bits{1}); // 34
    display(buf);
    shift_right_(buf, t_bits{1}); // 33
    display(buf);
    shift_right_(buf, t_bits{1}); // 32
    display(buf);
    shift_right_(buf, t_bits{1}); // 31
    display(buf);
    shift_right_(buf, t_bits{1}); // 30
    display(buf);
    shift_right_(buf, t_bits{1}); // 29
    display(buf);
    shift_right_(buf, t_bits{1}); // 28
    display(buf);
    shift_right_(buf, t_bits{1}); // 27
    display(buf);
    shift_right_(buf, t_bits{1}); // 26
    display(buf);
    shift_right_(buf, t_bits{1}); // 25
    display(buf);
    shift_right_(buf, t_bits{1}); // 24
    display(buf);
    shift_right_(buf, t_bits{1}); // 23
    display(buf);
    shift_right_(buf, t_bits{1}); // 22
    display(buf);
    shift_right_(buf, t_bits{1}); // 21
    display(buf);
    shift_right_(buf, t_bits{1}); // 20
    display(buf);
    shift_right_(buf, t_bits{1}); // 19
    display(buf);
    shift_right_(buf, t_bits{1}); // 18
    display(buf);
    shift_right_(buf, t_bits{1}); // 17
    display(buf);
    shift_right_(buf, t_bits{1}); // 16
    display(buf);
    shift_right_(buf, t_bits{1}); // 15
    display(buf);
    shift_right_(buf, t_bits{1}); // 14
    display(buf);
    shift_right_(buf, t_bits{1}); // 13
    display(buf);
    shift_right_(buf, t_bits{1}); // 12
    display(buf);
    shift_right_(buf, t_bits{1}); // 11
    display(buf);
    shift_right_(buf, t_bits{1}); // 10
    display(buf);
    shift_right_(buf, t_bits{1}); // 9
    display(buf);
    shift_right_(buf, t_bits{1}); // 8
    display(buf);
    shift_right_(buf, t_bits{1}); // 7
    display(buf);
    shift_right_(buf, t_bits{1}); // 6
    display(buf);
    shift_right_(buf, t_bits{1}); // 5
    display(buf);
    shift_right_(buf, t_bits{1}); // 4
    display(buf);
    shift_right_(buf, t_bits{1}); // 3
    display(buf);
    shift_right_(buf, t_bits{1}); // 2
    display(buf);
    shift_right_(buf, t_bits{1}); // 1
    display(buf);
    shift_right_(buf, t_bits{1}); // 0
    display(buf);

    shift_left_(buf, t_bits{1});
    display(buf);
    shift_left_(buf, t_bits{1});
    display(buf);
    shift_left_(buf, t_bits{1});
    display(buf);
    shift_left_(buf, t_bits{1});
    display(buf);
    shift_left_(buf, t_bits{1});
    display(buf);
    shift_left_(buf, t_bits{1});
    display(buf);
    shift_left_(buf, t_bits{1});
    display(buf);
    shift_left_(buf, t_bits{1});
    display(buf);
    shift_left_(buf, t_bits{1});
    display(buf);
    shift_left_(buf, t_bits{1});
    display(buf);
    shift_left_(buf, t_bits{1});
    display(buf);
    shift_left_(buf, t_bits{1});
    display(buf);
    shift_left_(buf, t_bits{1});
    display(buf);
    shift_left_(buf, t_bits{1});
    display(buf);
    shift_left_(buf, t_bits{1});
    display(buf);
    shift_left_(buf, t_bits{1});
    display(buf);
    shift_left_(buf, t_bits{1});
    display(buf);
    shift_left_(buf, t_bits{1});
    display(buf);
    shift_left_(buf, t_bits{1});
    display(buf);
    shift_left_(buf, t_bits{1});
    display(buf);
    shift_left_(buf, t_bits{1});
    display(buf);
    shift_left_(buf, t_bits{1});
    display(buf);
    shift_left_(buf, t_bits{1});
    display(buf);
    shift_left_(buf, t_bits{1});
    display(buf);
    shift_left_(buf, t_bits{1});
    display(buf);
    shift_left_(buf, t_bits{1});
    display(buf);
    shift_left_(buf, t_bits{1});
    display(buf);
    shift_left_(buf, t_bits{1});
    display(buf);
    shift_left_(buf, t_bits{1});
    display(buf);
    shift_left_(buf, t_bits{1});
    display(buf);
    shift_left_(buf, t_bits{1});
    display(buf);
    shift_left_(buf, t_bits{1});
    display(buf);
    shift_left_(buf, t_bits{1});
    display(buf);
    shift_left_(buf, t_bits{1});
    display(buf);
    shift_left_(buf, t_bits{1});
    display(buf);
    shift_left_(buf, t_bits{1});
    display(buf);
    shift_left_(buf, t_bits{1});
    display(buf);
    shift_left_(buf, t_bits{1});
    display(buf);
    shift_left_(buf, t_bits{1});
    display(buf);
    shift_left_(buf, t_bits{1});
    display(buf);
    shift_left_(buf, t_bits{1});
    display(buf);
    shift_left_(buf, t_bits{1});
    display(buf);
    shift_left_(buf, t_bits{1});
    display(buf);
    shift_left_(buf, t_bits{1});
    display(buf);
    shift_left_(buf, t_bits{1});
    display(buf);
    shift_left_(buf, t_bits{1});
    display(buf);
  }

  {
    auto value = 1;
    impl_::assign_(buf, static_cast<impl_::t_pvalue_>(value));

    auto b  = impl_::msb_on_(buf);
    auto bm = impl_::max_bits_(buf);
    auto bf = impl_::free_bits_(buf);
    auto bn = b.bits();

    std::cout << value << " := need bits(" << get(bn)
              << "), max_bits(" << get(bm)
              << "), free_bits(" << get(bf) << ")\n";
    display(buf);
  }

  {
    auto value = std::numeric_limits<impl_::t_pvalue_>::max();
    impl_::assign_(buf, static_cast<impl_::t_pvalue_>(value));

    auto b  = impl_::msb_on_(buf);
    auto bm = impl_::max_bits_(buf);
    auto bf = impl_::free_bits_(buf);
    auto bn = b.bits();

    std::cout << value << " := " << get(bn)
              << "), max_bits(" << get(bm)
              << "), free_bits(" << get(bf) << ")\n";
    display(buf);
  }

  {
    auto value =  99;
    impl_::assign_(buf, static_cast<impl_::t_pvalue_>(value));

    auto b  = impl_::msb_on_(buf);
    auto bm = impl_::max_bits_(buf);
    auto bf = impl_::free_bits_(buf);
    auto bn = b.bits();

    std::cout << value << " := " << get(bn)
              << "), max_bits(" << get(bm)
              << "), free_bits(" << get(bf) << ")\n";
    display(buf);
  }

  {
    auto value = 0;
    impl_::assign_(buf, static_cast<impl_::t_pvalue_>(value));

    auto b  = impl_::msb_on_(buf);
    auto bm = impl_::max_bits_(buf);
    auto bf = impl_::free_bits_(buf);
    auto bn = b.bits();

    std::cout << value << " := " << get(bn)
              << "), max_bits(" << get(bm)
              << "), free_bits(" << get(bf) << ")\n";
    display(buf);
  }

  {
    auto value = 1;
    impl_::assign_(buf, static_cast<impl_::t_pvalue_>(value));

    impl_::twos_compl_(buf);

    auto b  = impl_::msb_off_(buf);
    auto bn = b.bits();

    std::cout << "-1 := off_bit " << get(bn) << ")\n";
    display(buf);
  }

  {
    auto value = 7;
    impl_::assign_(buf, static_cast<impl_::t_pvalue_>(value));
    impl_::twos_compl_(buf);

    auto b  = impl_::msb_off_(buf);
    auto bn = b.bits();

    std::cout << "-7 := off_bit " << get(bn) << ")\n";
    display(buf);
  }

  {
    auto value = 253;
    impl_::assign_(buf, static_cast<impl_::t_pvalue_>(value));
    impl_::twos_compl_(buf);

    auto b  = impl_::msb_off_(buf);
    auto bn = b.bits();

    std::cout << "-253:= off_bit " << get(bn) << ")\n";
    display(buf);
  }

  {
    auto value = 255;
    impl_::assign_(buf, static_cast<impl_::t_pvalue_>(value));
    impl_::twos_compl_(buf);

    auto b  = impl_::msb_off_(buf);
    auto bn = b.bits();

    std::cout << "-255 := off_bit " << get(bn) << ")\n";
    display(buf);
  }

  {
    auto value = 0;
    impl_::assign_(buf, static_cast<impl_::t_pvalue_>(value));
    impl_::twos_compl_(buf);

    auto b  = impl_::msb_off_(buf);
    auto bn = b.bits();

    std::cout << "0 := off_bit " << get(bn) << ")\n";
    display(buf);
  }

  {
    auto value = 7;
    impl_::assign_(buf, static_cast<impl_::t_pvalue_>(value));

    display(buf);
    impl_::shift_left_(buf, t_bits{2});
    display(buf);

  }

  {
    auto value = 1;
    impl_::assign_(buf, static_cast<impl_::t_pvalue_>(value));

    display(buf);
    impl_::shift_left_(buf, t_bits{15});
    display(buf);
  }

  {
    auto value = 1;
    impl_::assign_(buf, static_cast<impl_::t_pvalue_>(value));

    display(buf);
    impl_::shift_left_(buf, t_bits{7});
    display(buf);
    impl_::shift_right_(buf, t_bits{7});
    display(buf);
  }

  {
    auto value = 6;
    impl_::assign_(buf, static_cast<impl_::t_pvalue_>(value));

    display(buf);
    impl_::shift_left_(buf, t_bits{33});
    display(buf);
  }

  {
    auto value = -1;
    impl_::assign_(buf, static_cast<impl_::t_nvalue_>(value));

    display(buf);
    impl_::shift_left_(buf, t_bits{15});
    display(buf);
  }

  {
    auto value = -1;
    impl_::assign_(buf, static_cast<impl_::t_nvalue_>(value));

    display(buf);
    impl_::shift_right_(buf, t_bits{3});
    display(buf);
  }

  {
    impl_::shift_right_(buf, t_bits{29});
    display(buf);
    impl_::shift_left_(buf,  t_bits{29});
    display(buf);
    impl_::shift_right_(buf, t_bits{29});
    display(buf);
    impl_::shift_left_(buf,  t_bits{29});
    display(buf);
  }

  {
    impl_::shift_left_(buf, t_bits{100});
    display(buf);
  }

  {
    impl_::t_store_ buf;
    auto value  = -1;
    impl_::assign_(buf, static_cast<impl_::t_nvalue_>(value));

    impl_::t_store_ buf1;
    auto value1 = -4;
    impl_::assign_(buf1, static_cast<impl_::t_nvalue_>(value1));

    impl_::add_(buf, buf1);

    display(buf);
  }

  {
    impl_::t_store_ buf;
    auto value  = -1;
    impl_::assign_(buf, static_cast<impl_::t_nvalue_>(value));
    display(buf);

    impl_::t_store_ buf1;
    impl_::assign_(buf1, static_cast<impl_::t_pvalue_>(1));
    impl_::shift_left_(buf1,  t_bits{14});
    display(buf1);

    impl_::add_(buf, buf1);
    display(buf);
  }

  {
    impl_::t_store_ buf;
    auto value  = -1;
    impl_::assign_(buf, static_cast<impl_::t_nvalue_>(value));
    display(buf);

    impl_::t_store_ buf1;
    impl_::assign_(buf1, static_cast<impl_::t_nvalue_>(-1));
    display(buf1);

    impl_::add_(buf, buf1);
    display(buf);
  }

  {
    impl_::t_store_ buf;
    impl_::assign_(buf, static_cast<impl_::t_pvalue_>(0));
    display(buf);

    impl_::t_store_ buf1;
    impl_::assign_(buf1, static_cast<impl_::t_nvalue_>(127));
    display(buf1);

    int i = 1;
    for (; i < 100000; ++i)
      impl_::add_(buf, buf1);

    impl_::add_(buf, buf1);
    std::cout << "[" << i++ << "] ";
    display(buf);

    impl_::add_(buf, buf1);
    std::cout << "[" << i++ << "] ";
    display(buf);

    impl_::add_(buf, buf1);
    std::cout << "[" << i++ << "] ";
    display(buf);

    impl_::add_(buf, buf1);
    std::cout << "[" << i++ << "] ";
    display(buf);

    impl_::assign_(buf, static_cast<impl_::t_pvalue_>(1));
    for (i = 0; i < 100000; ++i) {
      impl_::t_store_ tmp(buf);
      impl_::add_(buf, tmp);
    }
    display(buf);
  }

  {
    impl_::t_store_ buf;
    impl_::assign_(buf, static_cast<impl_::t_nvalue_>(-1));
    display(buf);

    impl_::t_store_ buf1;
    impl_::assign_(buf1, static_cast<impl_::t_nvalue_>(-1));
    display(buf1);

    int i = 1;
    for (; i < 10000000; ++i) {
      impl_::add_(buf, buf1);
    }

    impl_::add_(buf, buf1);
    std::cout << "[" << i++ << "] ";
    impl_::t_store_ tmp(buf);
    impl_::twos_compl_(tmp);
    display(tmp);

    impl_::add_(buf, buf1);
    std::cout << "[" << i++ << "] ";
    tmp = buf;
    impl_::twos_compl_(tmp);
    display(tmp);

    impl_::add_(buf, buf1);
    std::cout << "[" << i++ << "] ";
    tmp = buf;
    impl_::twos_compl_(tmp);
    display(tmp);

    impl_::add_(buf, buf1);
    std::cout << "[" << i++ << "] ";
    tmp = buf;
    impl_::twos_compl_(tmp);
    display(tmp);
  }

  {
    t_integer int1 = static_cast<impl_::t_pvalue_>(1);
    t_integer int2 = static_cast<impl_::t_nvalue_>(-100);
    auto int3 = int1 + int2;
    auto int4 = -int3;

    std::cout << "1 + (-100) = " << std::endl;
    static_cast<R_binary>(int4).display();
  }

  {
    t_integer int1 = static_cast<impl_::t_pvalue_>(5);
    t_integer int2 = static_cast<impl_::t_pvalue_>(4);
    auto int3 = int1 * int2;

    std::cout << "5 * 4 = " << std::endl;
    static_cast<R_binary>(int3).display();
  }

  {
    t_integer int1 = static_cast<impl_::t_pvalue_>(5);
    t_integer int2 = static_cast<impl_::t_nvalue_>(-4);
    auto int3 = int1 * int2;
    auto int4 = -int3;

    std::cout << "5 * -4 = " << std::endl;
    static_cast<R_binary>(int4).display();
  }

  {
    t_integer int1 = static_cast<impl_::t_nvalue_>(-5);
    t_integer int2 = static_cast<impl_::t_nvalue_>(-4);
    auto int3 = int1 * int2;

    std::cout << "-5 * -4 = " << std::endl;
    static_cast<R_binary>(int3).display();
  }

  {
    t_integer int1 = std::numeric_limits<impl_::t_nvalue_>::min();
    t_integer int2 = std::numeric_limits<impl_::t_pvalue_>::max();
    auto int3 = int1 * int2;
    auto int4 = -int3;

    std::cout << "-128 * 255 = " << std::endl;
    static_cast<R_binary>(int4).display();
  }

  {
    t_integer int1 = static_cast<impl_::t_pvalue_>(127);
    static_cast<R_binary>(int1).display();

    for (int i = 0; i < 10; ++i) {
     int1 *= static_cast<impl_::t_pvalue_>(127);
      std::cout << "i = " << i << std::endl;
      static_cast<R_binary>(int1).display();
    }

    std::cout << "127**10 = " << std::endl;
    static_cast<R_binary>(int1).display();
  }

  return 0;
}
