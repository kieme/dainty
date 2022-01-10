#include <iostream>
#include <limits>
#include <exception>
#include "dainty_base_logical.h"

using namespace dainty::base::types;
using namespace dainty::base::logical;
using namespace dainty::base::traits;

static_assert(std::numeric_limits<t_uchar>::max() == t_property<t_uchar>::MAX);
static_assert(std::numeric_limits<t_schar>::max() == t_property<t_schar>::MAX);
static_assert(std::numeric_limits<t_char> ::max() == t_property<t_char> ::MAX);
static_assert(std::numeric_limits<t_short>::max() == t_property<t_short>::MAX);
static_assert(std::numeric_limits<t_int>  ::max() == t_property<t_int>  ::MAX);
static_assert(std::numeric_limits<t_long> ::max() == t_property<t_long> ::MAX);
static_assert(std::numeric_limits<t_ulong>::max() == t_property<t_ulong>::MAX);
static_assert(std::numeric_limits<t_llong>::max() == t_property<t_llong>::MAX);
static_assert(std::numeric_limits<t_uchar>::min() == t_property<t_uchar>::MIN);
static_assert(std::numeric_limits<t_schar>::min() == t_property<t_schar>::MIN);
static_assert(std::numeric_limits<t_char> ::min() == t_property<t_char> ::MIN);
static_assert(std::numeric_limits<t_short>::min() == t_property<t_short>::MIN);
static_assert(std::numeric_limits<t_int>  ::min() == t_property<t_int>  ::MIN);
static_assert(std::numeric_limits<t_long> ::min() == t_property<t_long> ::MIN);
static_assert(std::numeric_limits<t_ulong>::min() == t_property<t_ulong>::MIN);
static_assert(std::numeric_limits<t_llong>::min() == t_property<t_llong>::MIN);

static_assert(std::numeric_limits<t_float>  ::max() == t_property<t_float>  ::MAX);
static_assert(std::numeric_limits<t_double> ::max() == t_property<t_double> ::MAX);
static_assert(std::numeric_limits<t_ldouble>::max() == t_property<t_ldouble>::MAX);
static_assert(std::numeric_limits<t_float>  ::min() == t_property<t_float>  ::MIN);
static_assert(std::numeric_limits<t_double> ::min() == t_property<t_double> ::MIN);
static_assert(std::numeric_limits<t_ldouble>::min() == t_property<t_ldouble>::MIN);

struct t_display {
  t_display() { std::cout << "-------------------------------{" << std::endl; }
 ~t_display() { std::cout << "}-------------------------------" << std::endl; }
};

template<typename T,
         t_opt_if_same<t_opt1, T, int> = OPT1>
void loo(T value) noexcept {
}

template<typename T,
         t_opt_if_unsigned_integral<t_opt2, T> = OPT2>
void loo(T value) noexcept {
}

template<typename T,
         t_opt_if<t_opt2, t_and<t_is_signed_integral<T>,
                                t_is_not_same<T, int>>> = OPT2>
void loo(T value) noexcept {
}

void foo() noexcept {
}

int goo(int) noexcept {
  return 0;
}

static_assert(t_and<t_is_free_func<decltype(foo), decltype(goo)>,
                    t_is_func<decltype(foo)>>::VALUE,
              "its not a ...");

struct B1 { };
struct B2 { };
struct B3 { };

struct D1 : B1, B2 { };
struct D2 : B1, B2, B3 { };
struct D3 : B2, B3 { };

static_assert(t_is_derived_from<D2, B1, B2>::VALUE, "yes");
static_assert(t_is_base_of<B3, D3>::VALUE, "yes");


template<typename T, typename T1>
constexpr void test_range(T1 value) noexcept {
  if (value < t_property<T>::MIN && value > t_property<T>::MAX)
    undef_func();
}

template<typename T>
constexpr t_u8_ mk_8U(T value) noexcept {
  test_range<t_u8_>(value);
  return (t_u8_)value;
}

template<typename T>
constexpr t_i8_ mk_8I(T value) noexcept {
  test_range<t_i8_>(value);
  return (t_i8_)value;
}

template<typename T>
constexpr t_u16_ mk_16U(T value) noexcept {
  test_range<t_u16_>(value);
  return (t_u16_)value;
}

template<typename T>
constexpr t_i16_ mk_16I(T value) noexcept {
  test_range<t_i16_>(value);
  return (t_i16_)value;
}

template<typename T>
constexpr t_u64_ mk_64U(T value) noexcept {
  test_range<t_u64_>(value);
  return (t_u64_)value;
}

enum t_o_ { };

struct t_tag_ {
  using t_ops = t_pack<t_o_, t_pack<t_o_>, t_o_, t_pack<t_o_>>;
  using t_value = int;
  constexpr
  static t_value check(t_value value) noexcept {
    //std::cout << "check t_tag_!" << std::endl;
    return value;
  }
};

struct t_tag1_ : public t_tag_ {
  //constexpr
  static t_value check(t_value value) noexcept {
    //std::cout << "check t_tag1_!" << std::endl;
    return value;
  }
};

struct t_tag2_ : public t_tag1_ {
  //constexpr
  static t_value check(t_value value) noexcept {
    //std::cout << "check t_tag2_!" << std::endl;
    return value;
  }
};

struct t_tag3_ : public t_tag_ {
  //constexpr
  static t_value check(t_value value) noexcept {
    //std::cout << "check t_tag3_!" << std::endl;
    return value;
  }
};

struct t_tag4_ : public t_tag3_ {
  //constexpr
  static t_value check(t_value value) noexcept {
    //std::cout << "check t_tag4_!" << std::endl;
    return value;
  }
};

//t_undef<t_is_subset_of_pack<t_tags_of<t_i16>, t_tags_of<t_u8>>> t1;

enum t1_ { };
enum t2_ { };
enum t3_ { };
enum t4_ { };
enum t5_ { };
enum t6_ { };
enum t7_ { };
enum t8_ { };
using p1_ = t_pack<t1_, t2_, t8_>;
using p2_ = t_pack<t1_, t2_, t3_, t4_, t1_>;
using p3_ = t_pack<t1_, t3_, t5_>;
using p4_ = t_pack<t7_, t8_>;
using p5_ = t_empty_pack;

using t_test  = t_logical<int, t_tag_>;
using t_test1 = t_logical<int, t_tag1_, t_test>;
using t_test2 = t_logical<int, t_tag2_, t_test1>;
using t_test3 = t_logical<int, t_tag3_, t_test>;
using t_test4 = t_logical<int, t_tag4_, t_test3>;
using t_test5 = t_logical<int, t_tag4_, t_test2, t_test4>;

struct A : t_logical<t_void, t_tag_> { };
struct B : t_logical<t_void, t_tag_> { };

using t_hallo = t_logical<p_void, t1_>;
using t_d1   = t_logical<t_double, t1_>;
using t_f1   = t_logical<t_float, t1_>;

static_assert(t_is_logical_data<t_bool, t_bool>::VALUE,
              "very strange");

int main() {
  t_truth tr = t_u8{5};
  if (tr == TRUTH_TRUE)
    std::cout << "its true" << std::endl;

  t_logical<t_bool, t_tag1_> bb{true};

  t_hallo hallo{nullptr};
  t_d1 d1{0.1};
  t_f1 f1{0.1f};

  loo(-1);

  t_bool zt = true;
  d1 = d1;

  constexpr auto zz7 = t_i{1L};

  t_ix ix = t_ix{-3};
  constexpr t_test iz{10};
  t_test iz1{1};
  t_test iz2{2};
  t_test1 it3{8};

  { t_display display;
    t_test5 zi1{10};
  }

  t_test5 zi1{10};
  t_test2 zi2 = zi1;

  { t_display display;
    set(zi1, zi2);
  }

  set(it3, iz2);
  iz2 = it3;

  t_i i1{1L};
  t_i i2{2L};
  i2 /= 3L;

  constexpr t_u i3 = U_MAX;
  if (i1 < i2)
    t_ix{3UL};

  t_u i8 = t_ix_begin{8UL};

  t_u i9 = t_u{8UL} % t_u{9UL};
  i9 += t_u{8UL};
  i9 += 8UL;
  if (static_cast<bool>(i3) && static_cast<bool>(i8) && 5)
    std::cout << "hello" << std::endl;

  t_i t11 = -t_i{3};
  ++t11;
  t11++;
  --t11;
  t11--;

  auto t12 = t_u{8UL};
  t12 << 1ul;

  std::cout << get(t11) << std::endl;

  {
    t_u8  u1{256};
    t_u16 u2 = u1;

    t_i8  i1{mk_8I(9)};
    t_i16 i2 = i1;

    i2 = i1;
    i2 = u1;
  }

  return 0;
}
