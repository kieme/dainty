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

#ifndef _DAINTY_BASE_NUMERIC_H_
#define _DAINTY_BASE_NUMERIC_H_

#include "dainty_base_numeric_impl.h"

namespace dainty
{
namespace base
{
namespace numeric
{
  using impl_::t_void;
  using impl_::t_bool;
  using impl_::t_n;
  using impl_::t_ix;

///////////////////////////////////////////////////////////////////////////////

  template<typename T> using t_if_neg = traits::t_if_signed_integral<T>;
  template<typename T> using t_if_pos = traits::t_if_unsigned_integral<T>;
  template<typename T> using t_if_int =
    traits::t_if<traits::t_or<traits::t_is_signed_integral<T>,
                              traits::t_is_unsigned_integral<T>>>;

///////////////////////////////////////////////////////////////////////////////

  class t_binary;
  using r_binary = types::t_prefix<t_binary>::r_;
  using x_binary = types::t_prefix<t_binary>::x_;
  using R_binary = types::t_prefix<t_binary>::R_;

  class t_binary {
  public:
    t_binary()    noexcept; // BIN_METHOD_1_
    t_binary(t_n) noexcept; // BIN_METHOD_2_

    template<typename T, t_if_neg<T> = traits::YES>
    t_binary(     T) noexcept; // BIN_METHOD_3_
    template<typename T, t_if_neg<T> = traits::YES>
    t_binary(t_n, T) noexcept; // BIN_METHOD_4_

    template<typename T, t_if_pos<T> = traits::YES>
    t_binary(     T) noexcept; // BIN_METHOD_5_
    template<typename T, t_if_pos<T> = traits::YES>
    t_binary(t_n, T) noexcept; // BIN_METHOD_6_

    t_binary(     R_binary) noexcept; // BIN_METHOD_7_
    t_binary(t_n, R_binary) noexcept; // BIN_METHOD_10_
    t_binary(     x_binary) noexcept; // BIN_METHOD_11_
    t_binary(t_n, x_binary) noexcept; // BIN_METHOD_12_

    ///////////////////////////////////////////////////////////////////////////

    template<typename T, t_if_neg<T> = traits::YES>
    r_binary operator=(T) noexcept; // BIN_METHOD_13_

    template<typename T, t_if_pos<T> = traits::YES>
    r_binary operator=(T) noexcept; // BIN_METHOD_14_

    r_binary operator=(R_binary) noexcept; // BIN_METHOD_16_
    r_binary operator=(x_binary) noexcept; // BIN_METHOD_17_

    ///////////////////////////////////////////////////////////////////////////

    template<typename T, t_if_neg<T> = traits::YES>
    r_binary operator+=(T) noexcept; // BIN_METHOD_18_

    template<typename T, t_if_pos<T> = traits::YES>
    r_binary operator+=(T) noexcept; // BIN_METHOD_19_

    r_binary operator+=(R_binary) noexcept; // BIN_METHOD_21_

    ///////////////////////////////////////////////////////////////////////////

    template<typename T, t_if_neg<T> = traits::YES>
    r_binary operator-=(T) noexcept; // BIN_METHOD_22_

    template<typename T, t_if_pos<T> = traits::YES>
    r_binary operator-=(T) noexcept; // BIN_METHOD_23_

    r_binary operator-=(R_binary) noexcept; // BIN_METHOD_25_

    ///////////////////////////////////////////////////////////////////////////

    template<typename T, t_if_neg<T> = traits::YES>
    r_binary operator*=(T) noexcept; // BIN_METHOD_26_

    template<typename T, t_if_pos<T> = traits::YES>
    r_binary operator*=(T) noexcept; // BIN_METHOD_27_

    r_binary operator*=(R_binary) noexcept; // BIN_METHOD_29_

    ///////////////////////////////////////////////////////////////////////////

    template<typename T, t_if_neg<T> = traits::YES>
    r_binary operator/=(T) noexcept; // BIN_METHOD_30_

    template<typename T, t_if_pos<T> = traits::YES>
    r_binary operator/=(T) noexcept; // BIN_METHOD_31_

    r_binary operator/=(R_binary) noexcept; // BIN_METHOD_33_

    ///////////////////////////////////////////////////////////////////////////

    template<typename T, t_if_int<T> = traits::YES>
    r_binary operator&=(T) noexcept; // BIN_METHOD_34_

    r_binary operator&=(R_binary) noexcept; // BIN_METHOD_37_

    ///////////////////////////////////////////////////////////////////////////

    template<typename T, t_if_int<T> = traits::YES>
    r_binary operator|=(T) noexcept; // BIN_METHOD_38_

    r_binary operator|=(R_binary) noexcept; // BIN_METHOD_41_

    ///////////////////////////////////////////////////////////////////////////

    template<typename T, t_if_int<T> = traits::YES>
    r_binary operator^=(T) noexcept; // BIN_METHOD_42_

    r_binary operator^=(R_binary) noexcept; // BIN_METHOD_45_

    ///////////////////////////////////////////////////////////////////////////

    t_binary operator-()      noexcept; // BIN_METHOD_46_
    t_binary operator~()      noexcept; // BIN_METHOD_47_
    r_binary operator>>=(t_n) noexcept; // BIN_METHOD_48_
    r_binary operator<<=(t_n) noexcept; // BIN_METHOD_49_

    ///////////////////////////////////////////////////////////////////////////

    r_binary ones_complement()     noexcept; // BIN_METHOD_50_
    r_binary twos_complement()     noexcept; // BIN_METHOD_51_
    r_binary shift_left(t_n)       noexcept; // BIN_METHOD_52_
    r_binary shift_right(t_n)      noexcept; // BIN_METHOD_53_
    r_binary set_bit(t_ix, t_bool) noexcept; // BIN_METHOD_54_
    r_binary set_bits(t_n)         noexcept; // BIN_METHOD_63_

    ///////////////////////////////////////////////////////////////////////////

    operator t_bool()       const noexcept; // BIN_METHOD_63_
    t_n    get_bits()       const noexcept; // BIN_METHOD_55_
    t_bool operator[](t_ix) const noexcept; // BIN_METHOD_56_
    t_bool is_negative()    const noexcept; // BIN_METHOD_57_

    ///////////////////////////////////////////////////////////////////////////

    r_binary reset()       noexcept; // BIN_METHOD_58_
    template<typename T, t_if_neg<T> = traits::YES>
    r_binary reset(     T) noexcept; // BIN_METHOD_59_
    template<typename T, t_if_neg<T> = traits::YES>
    r_binary reset(t_n, T) noexcept; // BIN_METHOD_60_
    template<typename T, t_if_pos<T> = traits::YES>
    r_binary reset(     T) noexcept; // BIN_METHOD_61_
    template<typename T, t_if_pos<T> = traits::YES>
    r_binary reset(t_n, T) noexcept; // BIN_METHOD_62_

    ///////////////////////////////////////////////////////////////////////////

    t_bool is_equal(R_binary) const noexcept; // BIN_METHOD_63_
    template<typename T, t_if_neg<T> = traits::YES>
    t_bool is_equal(T) const noexcept; // BIN_METHOD_64_
    template<typename T, t_if_pos<T> = traits::YES>
    t_bool is_equal(T) const noexcept; // BIN_METHOD_65_

    t_bool is_less(R_binary) const noexcept; // BIN_METHOD_66_
    template<typename T, t_if_neg<T> = traits::YES>
    t_bool is_less(T) const noexcept; // BIN_METHOD_67_
    template<typename T, t_if_pos<T> = traits::YES>
    t_bool is_less(T) const noexcept; // BIN_METHOD_68_

    t_bool is_less_equal(R_binary) const noexcept; // BIN_METHOD_69_
    template<typename T, t_if_neg<T> = traits::YES>
    t_bool is_less_equal(T) const noexcept; // BIN_METHOD_70_
    template<typename T, t_if_pos<T> = traits::YES>
    t_bool is_less_equal(T) const noexcept; // BIN_METHOD_71_

    ///////////////////////////////////////////////////////////////////////////

  private:
    using t_store_ = impl_::t_store_;
    using t_impl_  = impl_::t_binary_;

    t_store_ store_;
    t_impl_  impl_;
  };

///////////////////////////////////////////////////////////////////////////////

  // make_binary();

///////////////////////////////////////////////////////////////////////////////

  class t_integer {
  public:
  private:
    t_binary binary_;
  };

  class t_rational {
  public:
  private:
    t_integer numerator_;
    t_integer denominator_;
  };

  class t_precision {
  public:
  private:
  };

///////////////////////////////////////////////////////////////////////////////

  t_binary operator+(R_binary, R_binary) noexcept; // BIN_FUNC_1_
  t_binary operator+(x_binary, R_binary) noexcept; // BIN_FUNC_2_
  t_binary operator+(R_binary, x_binary) noexcept; // BIN_FUNC_3_
  t_binary operator+(x_binary, x_binary) noexcept; // BIN_FUNC_4_

  template<typename T, t_if_int<T> = traits::YES>
  t_binary operator+(R_binary, T) noexcept; // BIN_FUNC_5_
  template<typename T, t_if_int<T> = traits::YES>
  t_binary operator+(T, R_binary) noexcept; // BIN_FUNC_6_
  template<typename T, t_if_int<T> = traits::YES>
  t_binary operator+(x_binary, T) noexcept; // BIN_FUNC_7_
  template<typename T, t_if_int<T> = traits::YES>
  t_binary operator+(T, x_binary) noexcept; // BIN_FUNC_8_

  t_binary operator-(R_binary, R_binary) noexcept; // BIN_FUNC_9_
  t_binary operator-(x_binary, R_binary) noexcept; // BIN_FUNC_10_
  t_binary operator-(R_binary, x_binary) noexcept; // BIN_FUNC_11_
  t_binary operator-(x_binary, x_binary) noexcept; // BIN_FUNC_12_

  template<typename T, t_if_int<T> = traits::YES>
  t_binary operator-(R_binary, T) noexcept; // BIN_FUNC_13_
  template<typename T, t_if_int<T> = traits::YES>
  t_binary operator-(T, R_binary) noexcept; // BIN_FUNC_14_
  template<typename T, t_if_int<T> = traits::YES>
  t_binary operator-(x_binary, T) noexcept; // BIN_FUNC_15_
  template<typename T, t_if_int<T> = traits::YES>
  t_binary operator-(T, x_binary) noexcept; // BIN_FUNC_16_

  t_binary operator*(R_binary, R_binary) noexcept; // BIN_FUNC_17_
  t_binary operator*(x_binary, R_binary) noexcept; // BIN_FUNC_18_
  t_binary operator*(R_binary, x_binary) noexcept; // BIN_FUNC_19_
  t_binary operator*(x_binary, x_binary) noexcept; // BIN_FUNC_20_

  template<typename T, t_if_int<T> = traits::YES>
  t_binary operator*(R_binary, T) noexcept; // BIN_FUNC_21_
  template<typename T, t_if_int<T> = traits::YES>
  t_binary operator*(T, R_binary) noexcept; // BIN_FUNC_22_
  template<typename T, t_if_int<T> = traits::YES>
  t_binary operator*(x_binary, T) noexcept; // BIN_FUNC_23_
  template<typename T, t_if_int<T> = traits::YES>
  t_binary operator*(T, x_binary) noexcept; // BIN_FUNC_24_

  t_binary operator/(R_binary, R_binary) noexcept; // BIN_FUNC_25_
  t_binary operator/(x_binary, R_binary) noexcept; // BIN_FUNC_26_
  t_binary operator/(R_binary, x_binary) noexcept; // BIN_FUNC_27_
  t_binary operator/(x_binary, x_binary) noexcept; // BIN_FUNC_28_

  template<typename T, t_if_int<T> = traits::YES>
  t_binary operator/(R_binary, T) noexcept; // BIN_FUNC_29_
  template<typename T, t_if_int<T> = traits::YES>
  t_binary operator/(T, R_binary) noexcept; // BIN_FUNC_30_
  template<typename T, t_if_int<T> = traits::YES>
  t_binary operator/(x_binary, T) noexcept; // BIN_FUNC_31_
  template<typename T, t_if_int<T> = traits::YES>
  t_binary operator/(T, x_binary) noexcept; // BIN_FUNC_32_

  t_binary operator|(R_binary, R_binary) noexcept; // BIN_FUNC_33_
  t_binary operator|(x_binary, R_binary) noexcept; // BIN_FUNC_34_
  t_binary operator|(R_binary, x_binary) noexcept; // BIN_FUNC_35_
  t_binary operator|(x_binary, x_binary) noexcept; // BIN_FUNC_36_

  template<typename T, t_if_int<T> = traits::YES>
  t_binary operator|(R_binary, T) noexcept; // BIN_FUNC_37_
  template<typename T, t_if_int<T> = traits::YES>
  t_binary operator|(T, R_binary) noexcept; // BIN_FUNC_38_
  template<typename T, t_if_int<T> = traits::YES>
  t_binary operator|(x_binary, T) noexcept; // BIN_FUNC_39_
  template<typename T, t_if_int<T> = traits::YES>
  t_binary operator|(T, x_binary) noexcept; // BIN_FUNC_40_

  t_binary operator&(R_binary, R_binary) noexcept; // BIN_FUNC_41_
  t_binary operator&(x_binary, R_binary) noexcept; // BIN_FUNC_42_
  t_binary operator&(R_binary, x_binary) noexcept; // BIN_FUNC_43_
  t_binary operator&(x_binary, x_binary) noexcept; // BIN_FUNC_44_

  template<typename T, t_if_int<T> = traits::YES>
  t_binary operator&(R_binary, T) noexcept; // BIN_FUNC_45_
  template<typename T, t_if_int<T> = traits::YES>
  t_binary operator&(T, R_binary) noexcept; // BIN_FUNC_46_
  template<typename T, t_if_int<T> = traits::YES>
  t_binary operator&(x_binary, T) noexcept; // BIN_FUNC_47_
  template<typename T, t_if_int<T> = traits::YES>
  t_binary operator&(T, x_binary) noexcept; // BIN_FUNC_48_

  t_binary operator^(R_binary, R_binary) noexcept; // BIN_FUNC_49_
  t_binary operator^(x_binary, R_binary) noexcept; // BIN_FUNC_50_
  t_binary operator^(R_binary, x_binary) noexcept; // BIN_FUNC_51_
  t_binary operator^(x_binary, x_binary) noexcept; // BIN_FUNC_52_

  template<typename T, t_if_int<T> = traits::YES>
  t_binary operator^(R_binary, T) noexcept; // BIN_FUNC_53_
  template<typename T, t_if_int<T> = traits::YES>
  t_binary operator^(T, R_binary) noexcept; // BIN_FUNC_54_
  template<typename T, t_if_int<T> = traits::YES>
  t_binary operator^(x_binary, T) noexcept; // BIN_FUNC_55_
  template<typename T, t_if_int<T> = traits::YES>
  t_binary operator^(T, x_binary) noexcept; // BIN_FUNC_56_

  t_binary operator>>(t_binary, t_n) noexcept; // BIN_FUNC_57_
  t_binary operator<<(t_binary, t_n) noexcept; // BIN_FUNC_66_

  t_bool operator==(R_binary, R_binary) noexcept; // BIN_FUNC_67_
  template<typename T, t_if_int<T> = traits::YES>
  t_bool operator==(R_binary, T) noexcept; // BIN_FUNC_68_
  template<typename T, t_if_int<T> = traits::YES>
  t_bool operator==(T, R_binary) noexcept; // BIN_FUNC_69_

  t_bool operator!=(R_binary, R_binary) noexcept; // BIN_FUNC_70_
  template<typename T, t_if_int<T> = traits::YES>
  t_bool operator!=(R_binary, T) noexcept; // BIN_FUNC_71_
  template<typename T, t_if_int<T> = traits::YES>
  t_bool operator!=(T, R_binary) noexcept; // BIN_FUNC_72_

  t_bool operator<(R_binary, R_binary) noexcept; // BIN_FUNC_67_
  template<typename T, t_if_int<T> = traits::YES>
  t_bool operator<(R_binary, T) noexcept; // BIN_FUNC_73_
  template<typename T, t_if_int<T> = traits::YES>
  t_bool operator<(T, R_binary) noexcept; // BIN_FUNC_74_

  t_bool operator<=(R_binary, R_binary) noexcept; // BIN_FUNC_75_
  template<typename T, t_if_int<T> = traits::YES>
  t_bool operator<=(R_binary, T) noexcept; // BIN_FUNC_76_
  template<typename T, t_if_int<T> = traits::YES>
  t_bool operator<=(T, R_binary) noexcept; // BIN_FUNC_77_

  t_bool operator>(R_binary, R_binary) noexcept; // BIN_FUNC_78_
  template<typename T, t_if_int<T> = traits::YES>
  t_bool operator>(R_binary, T) noexcept; // BIN_FUNC_79_
  template<typename T, t_if_int<T> = traits::YES>
  t_bool operator>(T, R_binary) noexcept; // BIN_FUNC_80_

  t_bool operator>=(R_binary, R_binary) noexcept; // BIN_FUNC_81_
  template<typename T, t_if_int<T> = traits::YES>
  t_bool operator>=(R_binary, T) noexcept; // BIN_FUNC_82_
  template<typename T, t_if_int<T> = traits::YES>
  t_bool operator>=(T, R_binary) noexcept; // BIN_FUNC_83_

///////////////////////////////////////////////////////////////////////////////

  inline
  t_binary::t_binary() noexcept : impl_{store_} { // BIN_METHOD_1_
  }

  inline
  t_binary::t_binary(t_n n) noexcept
    : store_{n, 0}, impl_{store_} { // BIN_METHOD_2_
  }

  template<typename T, t_if_neg<T>>
  inline
  t_binary::t_binary(T value) noexcept
    : impl_{store_, static_cast<impl_::t_nvalue_>(value)} { // BIN_METHOD_3_
  }

  template<typename T, t_if_neg<T>>
  inline
  t_binary::t_binary(t_n n, T value) noexcept
    : store_{n, 0}, impl_{store_, static_cast<impl_::t_nvalue_>(value)} { // BIN_METHOD_4_
  }

  template<typename T, t_if_pos<T>>
  inline
  t_binary::t_binary(T value) noexcept
    : impl_{store_, static_cast<impl_::t_pvalue_>(value)} { // BIN_METHOD_5_
  }

  template<typename T, t_if_pos<T>>
  inline
  t_binary::t_binary(t_n n, T value) noexcept
    : store_{n, 0}, impl_{store_, static_cast<impl_::t_pvalue_>(value)} { // BIN_METHOD_6_
  }

  inline
  t_binary::t_binary(R_binary value) noexcept : impl_{store_, value.store_} { // BIN_METHOD_7_
  }

  inline
  t_binary::t_binary(t_n n, R_binary value) noexcept
    : store_{n, 0}, impl_{store_, value.store_} { // BIN_METHOD_10_
  }

  inline
  t_binary::t_binary(x_binary value) noexcept
    : impl_{store_, value.store_} { // BIN_METHOD_11_
  }

  inline
  t_binary::t_binary(t_n n, x_binary value) noexcept
    : store_{n, 0}, impl_{store_, value.store_} { // BIN_METHOD_12_
  }

  template<typename T, t_if_neg<T>>
  inline
  r_binary t_binary::operator=(T value) noexcept { // BIN_METHOD_13_
    impl_.assign(store_, static_cast<impl_::t_nvalue_>(value));
    return *this;
  }

  template<typename T, t_if_pos<T>>
  inline
  r_binary t_binary::operator=(T value) noexcept { // BIN_METHOD_14_
    impl_.assign(store_, static_cast<impl_::t_pvalue_>(value));
    return *this;
  }

  inline
  r_binary t_binary::operator=(R_binary value) noexcept { // BIN_METHOD_16_
    impl_.assign(store_, value.store_);
    return *this;
  }

  inline
  r_binary t_binary::operator=(x_binary value) noexcept { // BIN_METHOD_17_
    impl_.assign(store_, value.store_);
    return *this;
  }

  template<typename T, t_if_neg<T>>
  inline
  r_binary t_binary::operator+=(T value) noexcept { // BIN_METHOD_18_
    impl_.addition(store_, static_cast<impl_::t_nvalue_>(value));
    return *this;
  }

  template<typename T, t_if_pos<T>>
  inline
  r_binary t_binary::operator+=(T value) noexcept { // BIN_METHOD_19_
    impl_.addition(store_, static_cast<impl_::t_pvalue_>(value));
    return *this;
  }

  inline
  r_binary t_binary::operator+=(R_binary value) noexcept { // BIN_METHOD_21_
    impl_.addition(store_, value.store_);
    return *this;
  }

  template<typename T, t_if_neg<T>>
  inline
  r_binary t_binary::operator-=(T value) noexcept { // BIN_METHOD_22_
    impl_.subtraction(store_, static_cast<impl_::t_nvalue_>(value));
    return *this;
  }

  template<typename T, t_if_pos<T>>
  inline
  r_binary t_binary::operator-=(T value) noexcept { // BIN_METHOD_23_
    impl_.subtraction(store_, static_cast<impl_::t_pvalue_>(value));
    return *this;
  }

  inline
  r_binary t_binary::operator-=(R_binary value) noexcept { // BIN_METHOD_25_
    impl_.subtraction(store_, value.store_);
    return *this;
  }

  template<typename T, t_if_neg<T>>
  inline
  r_binary t_binary::operator*=(T value) noexcept { // BIN_METHOD_26_
    impl_.multiplication(store_, static_cast<impl_::t_nvalue_>(value));
    return *this;
  }

  template<typename T, t_if_pos<T>>
  inline
  r_binary t_binary::operator*=(T value) noexcept { // BIN_METHOD_27_
    impl_.multiplication(store_, static_cast<impl_::t_pvalue_>(value));
    return *this;
  }

  inline
  r_binary t_binary::operator*=(R_binary value) noexcept { // BIN_METHOD_29_
    impl_.multiplication(store_, value.store_);
    return *this;
  }

  template<typename T, t_if_neg<T>>
  inline
  r_binary t_binary::operator/=(T value) noexcept { // BIN_METHOD_30_
    impl_.division(store_, static_cast<impl_::t_nvalue_>(value));
    return *this;
  }

  template<typename T, t_if_pos<T>>
  inline
  r_binary t_binary::operator/=(T value) noexcept { // BIN_METHOD_31_
    impl_.division(store_, static_cast<impl_::t_pvalue_>(value));
    return *this;
  }

  inline
  r_binary t_binary::operator/=(R_binary value) noexcept { // BIN_METHOD_33_
    impl_.division(store_, value.store_);
    return *this;
  }

  template<typename T, t_if_int<T>>
  inline
  r_binary t_binary::operator&=(T value) noexcept { // BIN_METHOD_34_
    impl_.binary_and(store_, static_cast<impl_::t_pvalue_>(value));
    return *this;
  }

  inline
  r_binary t_binary::operator&=(R_binary value) noexcept { // BIN_METHOD_37_
    impl_.binary_and(store_, value.store_);
    return *this;
  }

  template<typename T, t_if_int<T>>
  inline
  r_binary t_binary::operator|=(T value) noexcept { // BIN_METHOD_38_
    impl_.binary_or(store_, static_cast<impl_::t_pvalue_>(value));
    return *this;
  }

  inline
  r_binary t_binary::operator|=(R_binary value) noexcept { // BIN_METHOD_41_
    impl_.binary_or(store_, value.store_);
    return *this;
  }

  template<typename T, t_if_int<T>>
  inline
  r_binary t_binary::operator^=(T value) noexcept { // BIN_METHOD_43_
    impl_.binary_xor(store_, static_cast<impl_::t_pvalue_>(value));
    return *this;
  }

  inline
  r_binary t_binary::operator^=(R_binary value) noexcept { // BIN_METHOD_45_
    impl_.binary_xor(store_, value.store_);
    return *this;
  }

  inline
  t_binary t_binary::operator-() noexcept { // BIN_METHOD_46_
    t_binary tmp(*this);
    tmp.twos_complement();
    return tmp;
  }

  inline
  t_binary t_binary::operator~() noexcept { // BIN_METHOD_47_
    t_binary tmp(*this);
    tmp.ones_complement();
    return tmp;
  }

  inline
  r_binary t_binary::operator>>=(t_n n) noexcept { // BIN_METHOD_48_
    impl_.shift_right(store_, n);
    return *this;
  }

  inline
  r_binary t_binary::operator<<=(t_n n) noexcept { // BIN_METHOD_49_
    impl_.shift_left(store_, n);
    return *this;
  }

  inline
  r_binary t_binary::ones_complement() noexcept { // BIN_METHOD_50_
    impl_.ones_complement(store_);
    return *this;
  }

  inline
  r_binary t_binary::twos_complement() noexcept { // BIN_METHOD_51_
    impl_.twos_complement(store_);
    return *this;
  }

  inline
  r_binary t_binary::shift_left(t_n n) noexcept { // BIN_METHOD_52_
    impl_.shift_left(store_, n);
    return *this;
  }

  inline
  r_binary t_binary::shift_right(t_n n) noexcept { // BIN_METHOD_53_
    impl_.shift_right(store_, n);
    return *this;
  }

  inline
  r_binary t_binary::set_bit(t_ix ix, t_bool on) noexcept { // BIN_METHOD_54_
    impl_.set_bit(store_, ix, on);
    return *this;
  }

  inline
  r_binary t_binary::set_bits(t_n n) noexcept { // BIN_METHOD_63_
    impl_.set_bits(store_, n);
    return *this;
  }

  inline
  t_n t_binary::get_bits() const noexcept { // BIN_METHOD_55_
    return impl_.get_bits(store_);
  }

  inline
  t_bool t_binary::operator[](t_ix ix) const noexcept { // BIN_METHOD_56_
    return impl_.get_bit(store_, ix);
  }

  inline
  t_bool t_binary::is_negative() const noexcept { // BIN_METHOD_57_
    return impl_.is_negative(store_);
  }

  inline
  r_binary t_binary::reset() noexcept { // BIN_METHOD_58_
    impl_.reset(store_, 0ULL);
    return *this;
  }

  template<typename T, t_if_neg<T>>
  inline
  r_binary t_binary::reset(T value) noexcept { // BIN_METHOD_59_
    impl_.reset(store_, static_cast<impl_::t_nvalue_>(value));
    return *this;
  }

  template<typename T, t_if_neg<T>>
  inline
  r_binary t_binary::reset(t_n n, T value) noexcept { // BIN_METHOD_60_
    impl_.reset(store_, n, static_cast<impl_::t_nvalue_>(value));
    return *this;
  }

  template<typename T, t_if_pos<T>>
  inline
  r_binary t_binary::reset(T value) noexcept { // BIN_METHOD_61_
    impl_.reset(store_, static_cast<impl_::t_pvalue_>(value));
    return *this;
  }

  template<typename T, t_if_pos<T>>
  inline
  r_binary t_binary::reset(t_n n, T value) noexcept { // BIN_METHOD_62_
    impl_.reset(store_, n, static_cast<impl_::t_pvalue_>(value));
    return *this;
  }

  inline
  t_binary::operator t_bool() const noexcept { // BIN_METHOD_63_
    return impl_.not_zero(store_);
  }

  inline
  t_bool t_binary::is_equal(R_binary value) const noexcept { // BIN_METHOD_63_
    return impl_.is_equal(store_, value.store_);
  }

  template<typename T, t_if_neg<T>>
  inline
  t_bool t_binary::is_equal(T value) const noexcept { // BIN_METHOD_64_
    return impl_.is_equal(store_, static_cast<impl_::t_nvalue_>(value));
  }

  template<typename T, t_if_pos<T>>
  inline
  t_bool t_binary::is_equal(T value) const noexcept { // BIN_METHOD_65_
    return impl_.is_equal(store_, static_cast<impl_::t_pvalue_>(value));
  }

  inline
  t_bool t_binary::is_less(R_binary value) const noexcept { // BIN_METHOD_66_
    return impl_.is_less(store_, value.store_);
  }

  template<typename T, t_if_neg<T>>
  inline
  t_bool t_binary::is_less(T value) const noexcept { // BIN_METHOD_67_
    return impl_.is_less(store_, static_cast<impl_::t_nvalue_>(value));
  }

  template<typename T, t_if_pos<T>>
  inline
  t_bool t_binary::is_less(T value) const noexcept { // BIN_METHOD_68_
    return impl_.is_less(store_, static_cast<impl_::t_pvalue_>(value));
  }

  inline
  t_bool t_binary::is_less_equal(R_binary value) const noexcept { // BIN_METHOD_69_
    return impl_.is_less_equal(store_, value.store_);
  }

  template<typename T, t_if_neg<T>>
  inline
  t_bool t_binary::is_less_equal(T value) const noexcept { // BIN_METHOD_70_
    return impl_.is_less_equal(store_, static_cast<impl_::t_nvalue_>(value));
  }

  template<typename T, t_if_pos<T>>
  inline
  t_bool t_binary::is_less_equal(T value) const noexcept { // BIN_METHOD_71_
    return impl_.is_less_equal(store_, static_cast<impl_::t_pvalue_>(value));
  }

///////////////////////////////////////////////////////////////////////////////

#define DAINTY_BASE_NUMERIC_OP_R_R_(op, lh, rh) \
    t_binary sum(lh); \
    sum op rh; \
    return sum;

#define DAINTY_BASE_NUMERIC_OP_X_R_(op, lh, rh) \
    lh op rh; \
    return x_cast(lh);

#define DAINTY_BASE_NUMERIC_OP_R_X_(op, lh, rh) \
    rh op lh; \
    return x_cast(rh);

#define DAINTY_BASE_NUMERIC_OP_X_X_(op, lh, rh) \
    if (get(lh.get_bits()) > get(rh.get_bits())) { \
      lh op rh; \
      return x_cast(lh); \
    } \
    rh op lh; \
    return x_cast(rh);

#define DAINTY_BASE_NUMERIC_OP_R_T_(op, lh, rh) \
    t_binary sum(lh); \
    sum op rh; \
    return sum;

#define DAINTY_BASE_NUMERIC_OP_T_R_(op, lh, rh) \
    t_binary sum(rh); \
    sum op lh; \
    return sum;

#define DAINTY_BASE_NUMERIC_OP_X_T_(op, lh, rh) \
    lh op rh; \
    return x_cast(lh);

#define DAINTY_BASE_NUMERIC_OP_T_X_(op, lh, rh) \
    rh op lh; \
    return x_cast(rh);

///////////////////////////////////////////////////////////////////////////////

  inline
  t_binary operator+(R_binary lh, R_binary rh) noexcept { // BIN_FUNC_1_
    DAINTY_BASE_NUMERIC_OP_R_R_(+=, lh, rh)
  }

  inline
  t_binary operator+(x_binary lh, R_binary rh) noexcept { // BIN_FUNC_2_
    DAINTY_BASE_NUMERIC_OP_X_R_(+=, lh, rh)
  }

  inline
  t_binary operator+(R_binary lh, x_binary rh) noexcept { // BIN_FUNC_3_
    DAINTY_BASE_NUMERIC_OP_R_X_(+=, lh, rh)
  }

  inline
  t_binary operator+(x_binary lh, x_binary rh) noexcept { // BIN_FUNC_4_
    DAINTY_BASE_NUMERIC_OP_X_X_(+=, lh, rh)
  }

  template<typename T, t_if_int<T>>
  inline
  t_binary operator+(R_binary lh, T rh) noexcept { // BIN_FUNC_5_
    DAINTY_BASE_NUMERIC_OP_R_T_(+=, lh, rh)
  }

  template<typename T, t_if_int<T>>
  inline
  t_binary operator+(T lh, R_binary rh) noexcept { // BIN_FUNC_6_
    DAINTY_BASE_NUMERIC_OP_T_R_(+=, lh, rh)
  }

  template<typename T, t_if_int<T>>
  inline
  t_binary operator+(x_binary lh, T rh) noexcept { // BIN_FUNC_7_
    DAINTY_BASE_NUMERIC_OP_X_T_(+=, lh, rh)
  }

  template<typename T, t_if_int<T>>
  inline
  t_binary operator+(T lh, x_binary rh) noexcept { // BIN_FUNC_8_
    DAINTY_BASE_NUMERIC_OP_T_X_(+=, lh, rh)
  }

///////////////////////////////////////////////////////////////////////////////

  inline
  t_binary operator-(R_binary lh, R_binary rh) noexcept { // BIN_FUNC_9_
    DAINTY_BASE_NUMERIC_OP_R_R_(-=, lh, rh)
  }

  inline
  t_binary operator-(x_binary lh, R_binary rh) noexcept { // BIN_FUNC_10_
    DAINTY_BASE_NUMERIC_OP_X_R_(-=, lh, rh)
  }

  inline
  t_binary operator-(R_binary lh, x_binary rh) noexcept { // BIN_FUNC_11_
    DAINTY_BASE_NUMERIC_OP_R_X_(-=, lh, rh)
  }

  inline
  t_binary operator-(x_binary lh, x_binary rh) noexcept { // BIN_FUNC_12_
    DAINTY_BASE_NUMERIC_OP_X_X_(-=, lh, rh)
  }

  template<typename T, t_if_int<T>>
  inline
  t_binary operator-(R_binary lh, T rh) noexcept { // BIN_FUNC_13_
    DAINTY_BASE_NUMERIC_OP_R_T_(-=, lh, rh)
  }

  template<typename T, t_if_int<T>>
  inline
  t_binary operator-(T lh, R_binary rh) noexcept { // BIN_FUNC_14_
    DAINTY_BASE_NUMERIC_OP_T_R_(-=, lh, rh)
  }

  template<typename T, t_if_int<T>>
  inline
  t_binary operator-(x_binary lh, T rh) noexcept { // BIN_FUNC_15_
    DAINTY_BASE_NUMERIC_OP_X_T_(-=, lh, rh)
  }

  template<typename T, t_if_int<T>>
  inline
  t_binary operator-(T lh, x_binary rh) noexcept { // BIN_FUNC_16_
    DAINTY_BASE_NUMERIC_OP_T_X_(-=, lh, rh)
  }

///////////////////////////////////////////////////////////////////////////////

  inline
  t_binary operator*(R_binary lh, R_binary rh) noexcept { // BIN_FUNC_17_
    DAINTY_BASE_NUMERIC_OP_R_R_(*=, lh, rh)
  }

  inline
  t_binary operator*(x_binary lh, R_binary rh) noexcept { // BIN_FUNC_18_
    DAINTY_BASE_NUMERIC_OP_X_R_(*=, lh, rh)
  }

  inline
  t_binary operator*(R_binary lh, x_binary rh) noexcept { // BIN_FUNC_19_
    DAINTY_BASE_NUMERIC_OP_R_X_(*=, lh, rh)
  }

  inline
  t_binary operator*(x_binary lh, x_binary rh) noexcept { // BIN_FUNC_20_
    DAINTY_BASE_NUMERIC_OP_X_X_(*=, lh, rh)
  }

  template<typename T, t_if_int<T>>
  inline
  t_binary operator*(R_binary lh, T rh) noexcept { // BIN_FUNC_21_
    DAINTY_BASE_NUMERIC_OP_R_T_(*=, lh, rh)
  }

  template<typename T, t_if_int<T>>
  inline
  t_binary operator*(T lh, R_binary rh) noexcept { // BIN_FUNC_22_
    DAINTY_BASE_NUMERIC_OP_T_R_(*=, lh, rh)
  }

  template<typename T, t_if_int<T>>
  inline
  t_binary operator*(x_binary lh, T rh) noexcept { // BIN_FUNC_23_
    DAINTY_BASE_NUMERIC_OP_X_T_(*=, lh, rh)
  }

  template<typename T, t_if_int<T>>
  inline
  t_binary operator*(T lh, x_binary rh) noexcept { // BIN_FUNC_24_
    DAINTY_BASE_NUMERIC_OP_T_X_(*=, lh, rh)
  }

///////////////////////////////////////////////////////////////////////////////

  inline
  t_binary operator/(R_binary lh, R_binary rh) noexcept { // BIN_FUNC_25_
    DAINTY_BASE_NUMERIC_OP_R_R_(/=, lh, rh)
  }

  inline
  t_binary operator/(x_binary lh, R_binary rh) noexcept { // BIN_FUNC_26_
    DAINTY_BASE_NUMERIC_OP_X_R_(/=, lh, rh)
  }

  inline
  t_binary operator/(R_binary lh, x_binary rh) noexcept { // BIN_FUNC_27_
    DAINTY_BASE_NUMERIC_OP_R_X_(/=, lh, rh)
  }

  inline
  t_binary operator/(x_binary lh, x_binary rh) noexcept { // BIN_FUNC_28_
    DAINTY_BASE_NUMERIC_OP_X_X_(/=, lh, rh)
  }

  template<typename T, t_if_int<T>>
  inline
  t_binary operator/(R_binary lh, T rh) noexcept { // BIN_FUNC_29_
    DAINTY_BASE_NUMERIC_OP_R_T_(/=, lh, rh)
  }

  template<typename T, t_if_int<T>>
  inline
  t_binary operator/(T lh, R_binary rh) noexcept { // BIN_FUNC_30_
    DAINTY_BASE_NUMERIC_OP_T_R_(/=, lh, rh)
  }

  template<typename T, t_if_int<T>>
  inline
  t_binary operator/(x_binary lh, T rh) noexcept { // BIN_FUNC_31_
    DAINTY_BASE_NUMERIC_OP_X_T_(/=, lh, rh)
  }

  template<typename T, t_if_int<T>>
  inline
  t_binary operator/(T lh, x_binary rh) noexcept { // BIN_FUNC_32_
    DAINTY_BASE_NUMERIC_OP_T_X_(/=, lh, rh)
  }

///////////////////////////////////////////////////////////////////////////////

  inline
  t_binary operator|(R_binary lh, R_binary rh) noexcept { // BIN_FUNC_33_
    DAINTY_BASE_NUMERIC_OP_R_R_(|=, lh, rh)
  }

  inline
  t_binary operator|(x_binary lh, R_binary rh) noexcept { // BIN_FUNC_34_
    DAINTY_BASE_NUMERIC_OP_X_R_(|=, lh, rh)
  }

  inline
  t_binary operator|(R_binary lh, x_binary rh) noexcept { // BIN_FUNC_35_
    DAINTY_BASE_NUMERIC_OP_R_X_(|=, lh, rh)
  }

  inline
  t_binary operator|(x_binary lh, x_binary rh) noexcept { // BIN_FUNC_36_
    DAINTY_BASE_NUMERIC_OP_X_X_(|=, lh, rh)
  }

  template<typename T, t_if_int<T>>
  inline
  t_binary operator|(R_binary lh, T rh) noexcept { // BIN_FUNC_37_
    DAINTY_BASE_NUMERIC_OP_R_T_(|=, lh, rh)
  }

  template<typename T, t_if_int<T>>
  inline
  t_binary operator|(T lh, R_binary rh) noexcept { // BIN_FUNC_38_
    DAINTY_BASE_NUMERIC_OP_T_R_(|=, lh, rh)
  }

  template<typename T, t_if_int<T>>
  inline
  t_binary operator|(x_binary lh, T rh) noexcept { // BIN_FUNC_39_
    DAINTY_BASE_NUMERIC_OP_X_T_(|=, lh, rh)
  }

  template<typename T, t_if_int<T>>
  inline
  t_binary operator|(T lh, x_binary rh) noexcept { // BIN_FUNC_40_
    DAINTY_BASE_NUMERIC_OP_T_X_(|=, lh, rh)
  }

///////////////////////////////////////////////////////////////////////////////

  inline
  t_binary operator&(R_binary lh, R_binary rh) noexcept { // BIN_FUNC_41_
    DAINTY_BASE_NUMERIC_OP_R_R_(&=, lh, rh)
  }

  inline
  t_binary operator&(x_binary lh, R_binary rh) noexcept { // BIN_FUNC_42_
    DAINTY_BASE_NUMERIC_OP_X_R_(&=, lh, rh)
  }

  inline
  t_binary operator&(R_binary lh, x_binary rh) noexcept { // BIN_FUNC_43_
    DAINTY_BASE_NUMERIC_OP_R_X_(&=, lh, rh)
  }

  inline
  t_binary operator&(x_binary lh, x_binary rh) noexcept { // BIN_FUNC_44_
    DAINTY_BASE_NUMERIC_OP_X_X_(&=, lh, rh)
  }

  template<typename T, t_if_int<T>>
  inline
  t_binary operator&(R_binary lh, T rh) noexcept { // BIN_FUNC_45_
    DAINTY_BASE_NUMERIC_OP_R_T_(&=, lh, rh)
  }

  template<typename T, t_if_int<T>>
  inline
  t_binary operator&(T lh, R_binary rh) noexcept { // BIN_FUNC_46_
    DAINTY_BASE_NUMERIC_OP_T_R_(&=, lh, rh)
  }

  template<typename T, t_if_int<T>>
  inline
  t_binary operator&(x_binary lh, T rh) noexcept { // BIN_FUNC_47_
    DAINTY_BASE_NUMERIC_OP_X_T_(&=, lh, rh)
  }

  template<typename T, t_if_int<T>>
  inline
  t_binary operator&(T lh, x_binary rh) noexcept { // BIN_FUNC_48_
    DAINTY_BASE_NUMERIC_OP_T_X_(&=, lh, rh)
  }

///////////////////////////////////////////////////////////////////////////////

  inline
  t_binary operator^(R_binary lh, R_binary rh) noexcept { // BIN_FUNC_49_
    DAINTY_BASE_NUMERIC_OP_R_R_(^=, lh, rh)
  }

  inline
  t_binary operator^(x_binary lh, R_binary rh) noexcept { // BIN_FUNC_50_
    DAINTY_BASE_NUMERIC_OP_X_R_(^=, lh, rh)
  }

  inline
  t_binary operator^(R_binary lh, x_binary rh) noexcept { // BIN_FUNC_51_
    DAINTY_BASE_NUMERIC_OP_R_X_(^=, lh, rh)
  }

  inline
  t_binary operator^(x_binary lh, x_binary rh) noexcept { // BIN_FUNC_52_
    DAINTY_BASE_NUMERIC_OP_X_X_(^=, lh, rh)
  }

  template<typename T, t_if_int<T>>
  inline
  t_binary operator^(R_binary lh, T rh) noexcept { // BIN_FUNC_53_
    DAINTY_BASE_NUMERIC_OP_R_T_(^=, lh, rh)
  }

  template<typename T, t_if_int<T>>
  inline
  t_binary operator^(T lh, R_binary rh) noexcept { // BIN_FUNC_54_
    DAINTY_BASE_NUMERIC_OP_T_R_(^=, lh, rh)
  }

  template<typename T, t_if_int<T>>
  inline
  t_binary operator^(x_binary lh, T rh) noexcept { // BIN_FUNC_55_
    DAINTY_BASE_NUMERIC_OP_X_T_(^=, lh, rh)
  }

  template<typename T, t_if_int<T>>
  inline
  t_binary operator^(T lh, x_binary rh) noexcept { // BIN_FUNC_56_
    DAINTY_BASE_NUMERIC_OP_T_X_(^=, lh, rh)
  }

///////////////////////////////////////////////////////////////////////////////

  inline
  t_binary operator>>(t_binary lh, t_n n) noexcept { // BIN_FUNC_57_
    DAINTY_BASE_NUMERIC_OP_X_T_(>>=, lh, n)
  }

///////////////////////////////////////////////////////////////////////////////

  inline
  t_binary operator<<(t_binary lh, t_n n) noexcept { // BIN_FUNC_66_
    DAINTY_BASE_NUMERIC_OP_X_T_(<<=, lh, n)
  }

///////////////////////////////////////////////////////////////////////////////

  inline
  t_bool operator==(R_binary lh, R_binary rh) noexcept { // BIN_FUNC_67_
    return lh.is_equal(rh);
  }

  template<typename T, t_if_int<T>>
  inline
  t_bool operator==(R_binary lh, T rh) noexcept { // BIN_FUNC_68_
    return lh.is_equal(rh);
  }

  template<typename T, t_if_int<T>>
  inline
  t_bool operator==(T lh, R_binary rh) noexcept { // BIN_FUNC_69_
    return rh.is_equal(lh);
  }

  inline
  t_bool operator!=(R_binary lh, R_binary rh) noexcept { // BIN_FUNC_70_
    return !lh.is_equal(rh);
  }

  template<typename T, t_if_int<T>>
  inline
  t_bool operator!=(R_binary lh, T rh) noexcept { // BIN_FUNC_71_
    return !lh.is_equal(rh);
  }

  template<typename T, t_if_int<T>>
  inline
  t_bool operator!=(T lh, R_binary rh) noexcept { // BIN_FUNC_72_
    return !rh.is_equal(lh);
  }

  inline
  t_bool operator<(R_binary lh, R_binary rh) noexcept { // BIN_FUNC_67_
    return lh.is_less(rh);
  }

  template<typename T, t_if_int<T>>
  inline
  t_bool operator<(R_binary lh, T rh) noexcept { // BIN_FUNC_73_
    return lh.is_less(rh);
  }

  template<typename T, t_if_int<T>>
  inline
  t_bool operator<(T lh, R_binary rh) noexcept { // BIN_FUNC_74_
    return !rh.is_less_equal(lh);
  }

  inline
  t_bool operator<=(R_binary lh, R_binary rh) noexcept { // BIN_FUNC_75_
    return lh.is_less_equal(rh);
  }

  template<typename T, t_if_int<T>>
  inline
  t_bool operator<=(R_binary lh, T rh) noexcept { // BIN_FUNC_76_
    return lh.is_less_equal(rh);
  }

  template<typename T, t_if_int<T>>
  inline
  t_bool operator<=(T lh, R_binary rh) noexcept { // BIN_FUNC_77_
    return !rh.is_less(lh);
  }

  inline
  t_bool operator>(R_binary lh, R_binary rh) noexcept { // BIN_FUNC_78_
    return !lh.is_less_equal(rh);
  }

  template<typename T, t_if_int<T>>
  inline
  t_bool operator>(R_binary lh, T rh) noexcept { // BIN_FUNC_79_
    return !lh.is_less_equal(rh);
  }

  template<typename T, t_if_int<T>>
  inline
  t_bool operator>(T lh, R_binary rh) noexcept { // BIN_FUNC_80_
    return rh.is_less(lh);
  }

  inline
  t_bool operator>=(R_binary lh, R_binary rh) noexcept { // BIN_FUNC_81_
    return rh.is_less_equal(lh);
  }

  template<typename T, t_if_int<T>>
  inline
  t_bool operator>=(R_binary lh, T rh) noexcept { // BIN_FUNC_82_
    return !lh.is_less(rh);
  }

  template<typename T, t_if_int<T>>
  inline
  t_bool operator>=(T lh, R_binary rh) noexcept { // BIN_FUNC_83_
    return rh.is_less_equal(lh);
  }

///////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
