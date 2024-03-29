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

#include "dainty_base_util.h"
#include "impl_/dainty_base_numeric_impl.h"

namespace dainty
{
namespace base
{
namespace numeric
{
  /////////////////////////////////////////////////////////////////////////////

  using impl_::t_void;
  using impl_::t_bool;
  using impl_::t_bit;
  using impl_::t_info;
  using impl_::t_bits;
  using impl_::t_digits;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T> using t_if_neg = impl_::t_if_neg_<T>;
  template<typename T> using t_if_pos = impl_::t_if_pos_<T>;
  template<typename T> using t_if_int = impl_::t_if_int_<T>;

  /////////////////////////////////////////////////////////////////////////////

  class t_binary;
  using r_binary = types::r_prefix_of<t_binary>;
  using x_binary = types::x_prefix_of<t_binary>;
  using R_binary = types::R_prefix_of<t_binary>;

  class t_binary {
  public:
    t_binary()                 noexcept; // BIN_METHOD_1_1_
    t_binary(t_bits)           noexcept; // BIN_METHOD_1_2_

    t_binary(        R_binary) noexcept; // BIN_METHOD_1_3_
    t_binary(t_bits, R_binary) noexcept; // BIN_METHOD_1_4_
    t_binary(        x_binary) noexcept; // BIN_METHOD_1_5_
    t_binary(t_bits, x_binary) noexcept; // BIN_METHOD_1_6_

    template<typename T, t_if_neg<T> = traits::WELL_FORMED>
    t_binary(        T)        noexcept; // BIN_METHOD_1_7_
    template<typename T, t_if_neg<T> = traits::WELL_FORMED>
    t_binary(t_bits, T)        noexcept; // BIN_METHOD_1_8_

    template<typename T, t_if_pos<T> = traits::WELL_FORMED>
    t_binary(        T)        noexcept; // BIN_METHOD_1_9_
    template<typename T, t_if_pos<T> = traits::WELL_FORMED>
    t_binary(t_bits, T)        noexcept; // BIN_METHOD_1_10_

    ///////////////////////////////////////////////////////////////////////////

    r_binary operator=(R_binary) noexcept; // BIN_METHOD_2_1_
    r_binary operator=(x_binary) noexcept; // BIN_METHOD_2_2_

    template<typename T, t_if_neg<T> = traits::WELL_FORMED>
    r_binary operator=(T)        noexcept; // BIN_METHOD_2_3_
    template<typename T, t_if_pos<T> = traits::WELL_FORMED>
    r_binary operator=(T)        noexcept; // BIN_METHOD_2_4_

    ///////////////////////////////////////////////////////////////////////////

    r_binary operator+=(R_binary) noexcept; // BIN_METHOD_3_1_

    template<typename T, t_if_neg<T> = traits::WELL_FORMED>
    r_binary operator+=(T)        noexcept; // BIN_METHOD_3_2_
    template<typename T, t_if_pos<T> = traits::WELL_FORMED>
    r_binary operator+=(T)        noexcept; // BIN_METHOD_3_3_

    ///////////////////////////////////////////////////////////////////////////

    r_binary operator-=(R_binary) noexcept; // BIN_METHOD_4_1_

    template<typename T, t_if_neg<T> = traits::WELL_FORMED>
    r_binary operator-=(T)        noexcept; // BIN_METHOD_4_2_
    template<typename T, t_if_pos<T> = traits::WELL_FORMED>
    r_binary operator-=(T)        noexcept; // BIN_METHOD_4_3_

    ///////////////////////////////////////////////////////////////////////////

    r_binary operator*=(R_binary) noexcept; // BIN_METHOD_5_1_

    template<typename T, t_if_neg<T> = traits::WELL_FORMED>
    r_binary operator*=(T)        noexcept; // BIN_METHOD_5_2_
    template<typename T, t_if_pos<T> = traits::WELL_FORMED>
    r_binary operator*=(T)        noexcept; // BIN_METHOD_5_3_

    ///////////////////////////////////////////////////////////////////////////

    r_binary operator/=(R_binary) noexcept; // BIN_METHOD_6_1_

    template<typename T, t_if_neg<T> = traits::WELL_FORMED>
    r_binary operator/=(T)        noexcept; // BIN_METHOD_6_2_
    template<typename T, t_if_pos<T> = traits::WELL_FORMED>
    r_binary operator/=(T)        noexcept; // BIN_METHOD_6_3_

    ///////////////////////////////////////////////////////////////////////////

    r_binary operator&=(R_binary) noexcept; // BIN_METHOD_7_1_

    template<typename T, t_if_neg<T> = traits::WELL_FORMED>
    r_binary operator&=(T)        noexcept; // BIN_METHOD_7_2_
    template<typename T, t_if_pos<T> = traits::WELL_FORMED>
    r_binary operator&=(T)        noexcept; // BIN_METHOD_7_3_

    ///////////////////////////////////////////////////////////////////////////

    r_binary operator|=(R_binary) noexcept; // BIN_METHOD_8_1_

    template<typename T, t_if_neg<T> = traits::WELL_FORMED>
    r_binary operator|=(T)        noexcept; // BIN_METHOD_8_2_
    template<typename T, t_if_pos<T> = traits::WELL_FORMED>
    r_binary operator|=(T)        noexcept; // BIN_METHOD_8_3_

    ///////////////////////////////////////////////////////////////////////////

    r_binary operator^=(R_binary) noexcept; // BIN_METHOD_9_1_

    template<typename T, t_if_neg<T> = traits::WELL_FORMED>
    r_binary operator^=(T)        noexcept; // BIN_METHOD_9_2_
    template<typename T, t_if_pos<T> = traits::WELL_FORMED>
    r_binary operator^=(T)        noexcept; // BIN_METHOD_9_3_

    ///////////////////////////////////////////////////////////////////////////

    t_binary operator-()         const noexcept; // BIN_METHOD_10_
    t_binary operator~()         const noexcept; // BIN_METHOD_11_
    r_binary operator>>=(t_bits)       noexcept; // BIN_METHOD_12_
    r_binary operator<<=(t_bits)       noexcept; // BIN_METHOD_13_

    r_binary ones_complement()         noexcept; // BIN_METHOD_14_
    r_binary twos_complement()         noexcept; // BIN_METHOD_15_

    r_binary shift_left(t_bits)        noexcept; // BIN_METHOD_16_
    r_binary shift_right(t_bits)       noexcept; // BIN_METHOD_17_

    t_bool   set_bit(t_bit, t_bool)    noexcept; // BIN_METHOD_18_
    t_bool   ensure_bits(t_bits)       noexcept; // BIN_METHOD_19_

    operator t_bool      ()      const noexcept; // BIN_METHOD_20_
    t_bool   operator[]  (t_bit) const noexcept; // BIN_METHOD_21_
    t_bool   get_bit     (t_bit) const noexcept; // BIN_METHOD_22_
    t_bits   get_bits    ()      const noexcept; // BIN_METHOD_23_
    t_bool   is_negative ()      const noexcept; // BIN_METHOD_24_
    t_void   display     ()      const noexcept; // BIN_METHOD_25_
    t_bool   ms_bit      ()      const noexcept; // BIN_METHOD_26_
    t_bool   ls_bit      ()      const noexcept; // BIN_METHOD_27_
    t_bit    first_on_bit()      const noexcept; // BIN_METHOD_28_
    t_bit    last_on_bit ()      const noexcept; // BIN_METHOD_29_
    t_bits   on_bits     ()      const noexcept; // BIN_METHOD_30_
    t_info   get_info    ()      const noexcept; // BIN_METHOD_31_

    ///////////////////////////////////////////////////////////////////////////

    t_void reset()                 noexcept; // BIN_METHOD_32_1_
    t_bool reset(        R_binary) noexcept; // BIN_METHOD_32_2_
    t_bool reset(t_bits, R_binary) noexcept; // BIN_METHOD_32_3_

    template<typename T, t_if_neg<T> = traits::WELL_FORMED>
    t_void reset(        T)        noexcept; // BIN_METHOD_32_4_
    template<typename T, t_if_neg<T> = traits::WELL_FORMED>
    t_bool reset(t_bits, T)        noexcept; // BIN_METHOD_32_5_
    template<typename T, t_if_pos<T> = traits::WELL_FORMED>
    t_void reset(        T)        noexcept; // BIN_METHOD_32_6_
    template<typename T, t_if_pos<T> = traits::WELL_FORMED>
    t_bool reset(t_bits, T)        noexcept; // BIN_METHOD_32_7_

    ///////////////////////////////////////////////////////////////////////////

    t_bool is_equal(R_binary) const noexcept; // BIN_METHOD_33_1_

    template<typename T, t_if_neg<T> = traits::WELL_FORMED>
    t_bool is_equal(T)        const noexcept; // BIN_METHOD_33_2_
    template<typename T, t_if_pos<T> = traits::WELL_FORMED>
    t_bool is_equal(T)        const noexcept; // BIN_METHOD_33_3_

    ///////////////////////////////////////////////////////////////////////////

    t_bool is_less(R_binary) const noexcept; // BIN_METHOD_34_1_

    template<typename T, t_if_neg<T> = traits::WELL_FORMED>
    t_bool is_less(T)        const noexcept; // BIN_METHOD_34_2_
    template<typename T, t_if_pos<T> = traits::WELL_FORMED>
    t_bool is_less(T)        const noexcept; // BIN_METHOD_34_3_

    ///////////////////////////////////////////////////////////////////////////

    t_bool is_less_equal(R_binary) const noexcept; // BIN_METHOD_35_1_

    template<typename T, t_if_neg<T> = traits::WELL_FORMED>
    t_bool is_less_equal(T)        const noexcept; // BIN_METHOD_35_2_
    template<typename T, t_if_pos<T> = traits::WELL_FORMED>
    t_bool is_less_equal(T)        const noexcept; // BIN_METHOD_35_3_

    ///////////////////////////////////////////////////////////////////////////

  private:
    using t_store_ = impl_::t_store_;
    using t_impl_  = impl_::t_binary_;

    t_store_ store_;
    t_impl_  impl_;
  };

  /////////////////////////////////////////////////////////////////////////////

  // make_binary();

  /////////////////////////////////////////////////////////////////////////////

  class t_integer;
  using r_integer = types::r_prefix_of<t_integer>;
  using x_integer = types::x_prefix_of<t_integer>;
  using R_integer = types::R_prefix_of<t_integer>;

  class t_integer {
  public:
    t_integer()                    noexcept; // INT_METHOD_1_1_
    t_integer(t_digits)            noexcept; // INT_METHOD_1_2_

    t_integer(          R_integer) noexcept; // INT_METHOD_1_3_
    t_integer(t_digits, R_integer) noexcept; // INT_METHOD_1_4_
    t_integer(          x_integer) noexcept; // INT_METHOD_1_5_
    t_integer(t_digits, x_integer) noexcept; // INT_METHOD_1_6_

    t_integer(          R_binary)  noexcept; // INT_METHOD_1_7_
    t_integer(t_digits, R_binary)  noexcept; // INT_METHOD_1_8_
    t_integer(          x_binary)  noexcept; // INT_METHOD_1_9_
    t_integer(t_digits, x_binary)  noexcept; // INT_METHOD_1_10_

    template<typename T, t_if_int<T> = traits::WELL_FORMED>
    t_integer(          T)         noexcept; // INT_METHOD_1_11_
    template<typename T, t_if_int<T> = traits::WELL_FORMED>
    t_integer(t_digits, T)         noexcept; // INT_METHOD_1_12_

    ///////////////////////////////////////////////////////////////////////////

    r_integer operator=(R_integer) noexcept; // INT_METHOD_2_1_
    r_integer operator=(x_integer) noexcept; // INT_METHOD_2_2_

    r_integer operator=(R_binary)  noexcept; // INT_METHOD_2_3_
    r_integer operator=(x_binary)  noexcept; // INT_METHOD_2_4_

    template<typename T, t_if_int<T> = traits::WELL_FORMED>
    r_integer operator=(T)         noexcept; // INT_METHOD_2_5_

    ///////////////////////////////////////////////////////////////////////////

    r_integer operator+=(R_integer) noexcept; // INT_METHOD_3_1_
    r_integer operator+=(R_binary)  noexcept; // INT_METHOD_3_2_

    template<typename T, t_if_int<T> = traits::WELL_FORMED>
    r_integer operator+=(T)         noexcept; // INT_METHOD_3_3_

    ///////////////////////////////////////////////////////////////////////////

    r_integer operator-=(R_integer) noexcept; // INT_METHOD_4_1_
    r_integer operator-=(R_binary)  noexcept; // INT_METHOD_4_2_

    template<typename T, t_if_int<T> = traits::WELL_FORMED>
    r_integer operator-=(T)         noexcept; // INT_METHOD_4_3_

    ///////////////////////////////////////////////////////////////////////////

    r_integer operator*=(R_integer) noexcept; // INT_METHOD_5_1_
    r_integer operator*=(R_binary)  noexcept; // INT_METHOD_5_2_

    template<typename T, t_if_int<T> = traits::WELL_FORMED>
    r_integer operator*=(T)         noexcept; // INT_METHOD_5_3_

    ///////////////////////////////////////////////////////////////////////////

    r_integer operator/=(R_integer) noexcept; // INT_METHOD_6_1_
    r_integer operator/=(R_binary)  noexcept; // INT_METHOD_6_2_

    template<typename T, t_if_int<T> = traits::WELL_FORMED>
    r_integer operator/=(T)         noexcept; // INT_METHOD_6_3_

    ///////////////////////////////////////////////////////////////////////////

    t_bool    ensure_digits(t_digits)  noexcept; // INT_METHOD_7_
    t_integer operator-    ()    const noexcept; // INT_METHOD_8_
    t_digits  get_digits   ()    const noexcept; // INT_METHOD_9_
    t_bool    is_negative  ()    const noexcept; // INT_METHOD_10_

    ///////////////////////////////////////////////////////////////////////////

    operator x_binary() &&    noexcept; // INT_METHOD_11_1_
    operator R_binary() const noexcept; // INT_METHOD_11_2_
    operator t_bool()   const noexcept; // INT_METHOD_11_3_

    ///////////////////////////////////////////////////////////////////////////

    t_void reset()                    noexcept; // INT_METHOD_12_1_
    t_bool reset(          R_integer) noexcept; // INT_METHOD_12_2_
    t_bool reset(t_digits, R_integer) noexcept; // INT_METHOD_12_3_
    t_bool reset(          R_binary)  noexcept; // INT_METHOD_12_4_
    t_bool reset(t_digits, R_binary)  noexcept; // INT_METHOD_12_5_

    template<typename T, t_if_int<T> = traits::WELL_FORMED>
    t_void reset(          T)         noexcept; // INT_METHOD_12_6_
    template<typename T, t_if_int<T> = traits::WELL_FORMED>
    t_bool reset(t_digits, T)         noexcept; // INT_METHOD_12_7_

    ///////////////////////////////////////////////////////////////////////////

    t_bool is_equal(R_integer) const noexcept; // INT_METHOD_13_1_
    t_bool is_equal(R_binary)  const noexcept; // INT_METHOD_13_2_

    template<typename T, t_if_int<T> = traits::WELL_FORMED>
    t_bool is_equal(T)         const noexcept; // INT_METHOD_13_3_

    ///////////////////////////////////////////////////////////////////////////

    t_bool is_less(R_integer) const noexcept; // INT_METHOD_14_1_
    t_bool is_less(R_binary)  const noexcept; // INT_METHOD_14_2_

    template<typename T, t_if_int<T> = traits::WELL_FORMED>
    t_bool is_less(T)         const noexcept; // INT_METHOD_14_3_

    ///////////////////////////////////////////////////////////////////////////

    t_bool is_less_equal(R_integer) const noexcept; // INT_METHOD_15_1_
    t_bool is_less_equal(R_binary)  const noexcept; // INT_METHOD_15_2_

    template<typename T, t_if_int<T> = traits::WELL_FORMED>
    t_bool is_less_equal(T)         const noexcept; // INT_METHOD_15_3_

  private:
    t_binary bin_;
  };

  /////////////////////////////////////////////////////////////////////////////

  class t_fraction {
  public:
  private:
    t_integer numerator_;
    t_integer denominator_;
  };

  class t_precision {
  public:
  private:
  };

  /////////////////////////////////////////////////////////////////////////////

  t_binary operator+(R_binary, R_binary) noexcept; // BIN_FUNC_1_1_
  t_binary operator+(x_binary, R_binary) noexcept; // BIN_FUNC_1_2_
  t_binary operator+(R_binary, x_binary) noexcept; // BIN_FUNC_1_3_
  t_binary operator+(x_binary, x_binary) noexcept; // BIN_FUNC_1_4_

  template<typename T, t_if_int<T> = traits::WELL_FORMED>
  t_binary operator+(R_binary, T)        noexcept; // BIN_FUNC_1_5_
  template<typename T, t_if_int<T> = traits::WELL_FORMED>
  t_binary operator+(T, R_binary)        noexcept; // BIN_FUNC_1_6_
  template<typename T, t_if_int<T> = traits::WELL_FORMED>
  t_binary operator+(x_binary, T)        noexcept; // BIN_FUNC_1_7_
  template<typename T, t_if_int<T> = traits::WELL_FORMED>
  t_binary operator+(T, x_binary)        noexcept; // BIN_FUNC_1_8_

  /////////////////////////////////////////////////////////////////////////////

  t_binary operator-(R_binary, R_binary) noexcept; // BIN_FUNC_2_1_
  t_binary operator-(x_binary, R_binary) noexcept; // BIN_FUNC_2_2_
  t_binary operator-(R_binary, x_binary) noexcept; // BIN_FUNC_2_3_
  t_binary operator-(x_binary, x_binary) noexcept; // BIN_FUNC_2_4_

  template<typename T, t_if_int<T> = traits::WELL_FORMED>
  t_binary operator-(R_binary, T)        noexcept; // BIN_FUNC_2_5_
  template<typename T, t_if_int<T> = traits::WELL_FORMED>
  t_binary operator-(T, R_binary)        noexcept; // BIN_FUNC_2_6_
  template<typename T, t_if_int<T> = traits::WELL_FORMED>
  t_binary operator-(x_binary, T)        noexcept; // BIN_FUNC_2_7_
  template<typename T, t_if_int<T> = traits::WELL_FORMED>
  t_binary operator-(T, x_binary)        noexcept; // BIN_FUNC_2_8_

  /////////////////////////////////////////////////////////////////////////////

  t_binary operator*(R_binary, R_binary) noexcept; // BIN_FUNC_3_1_
  t_binary operator*(x_binary, R_binary) noexcept; // BIN_FUNC_3_2_
  t_binary operator*(R_binary, x_binary) noexcept; // BIN_FUNC_3_3_
  t_binary operator*(x_binary, x_binary) noexcept; // BIN_FUNC_3_4_

  template<typename T, t_if_int<T> = traits::WELL_FORMED>
  t_binary operator*(R_binary, T)        noexcept; // BIN_FUNC_3_5_
  template<typename T, t_if_int<T> = traits::WELL_FORMED>
  t_binary operator*(T, R_binary)        noexcept; // BIN_FUNC_3_6_
  template<typename T, t_if_int<T> = traits::WELL_FORMED>
  t_binary operator*(x_binary, T)        noexcept; // BIN_FUNC_3_7_
  template<typename T, t_if_int<T> = traits::WELL_FORMED>
  t_binary operator*(T, x_binary)        noexcept; // BIN_FUNC_3_8_

  /////////////////////////////////////////////////////////////////////////////

  t_binary operator/(R_binary, R_binary) noexcept; // BIN_FUNC_4_1_
  t_binary operator/(x_binary, R_binary) noexcept; // BIN_FUNC_4_2_
  t_binary operator/(R_binary, x_binary) noexcept; // BIN_FUNC_4_3_
  t_binary operator/(x_binary, x_binary) noexcept; // BIN_FUNC_4_4_

  template<typename T, t_if_int<T> = traits::WELL_FORMED>
  t_binary operator/(R_binary, T)        noexcept; // BIN_FUNC_4_5_
  template<typename T, t_if_int<T> = traits::WELL_FORMED>
  t_binary operator/(T, R_binary)        noexcept; // BIN_FUNC_4_6_
  template<typename T, t_if_int<T> = traits::WELL_FORMED>
  t_binary operator/(x_binary, T)        noexcept; // BIN_FUNC_4_7_
  template<typename T, t_if_int<T> = traits::WELL_FORMED>
  t_binary operator/(T, x_binary)        noexcept; // BIN_FUNC_4_8_

  /////////////////////////////////////////////////////////////////////////////

  t_binary operator|(R_binary, R_binary) noexcept; // BIN_FUNC_5_1_
  t_binary operator|(x_binary, R_binary) noexcept; // BIN_FUNC_5_2_
  t_binary operator|(R_binary, x_binary) noexcept; // BIN_FUNC_5_3_
  t_binary operator|(x_binary, x_binary) noexcept; // BIN_FUNC_5_4_

  template<typename T, t_if_int<T> = traits::WELL_FORMED>
  t_binary operator|(R_binary, T)        noexcept; // BIN_FUNC_5_5_
  template<typename T, t_if_int<T> = traits::WELL_FORMED>
  t_binary operator|(T, R_binary)        noexcept; // BIN_FUNC_5_6_
  template<typename T, t_if_int<T> = traits::WELL_FORMED>
  t_binary operator|(x_binary, T)        noexcept; // BIN_FUNC_5_7_
  template<typename T, t_if_int<T> = traits::WELL_FORMED>
  t_binary operator|(T, x_binary)        noexcept; // BIN_FUNC_5_8_

  /////////////////////////////////////////////////////////////////////////////

  t_binary operator&(R_binary, R_binary) noexcept; // BIN_FUNC_6_1_
  t_binary operator&(x_binary, R_binary) noexcept; // BIN_FUNC_6_2_
  t_binary operator&(R_binary, x_binary) noexcept; // BIN_FUNC_6_3_
  t_binary operator&(x_binary, x_binary) noexcept; // BIN_FUNC_6_4_

  template<typename T, t_if_int<T> = traits::WELL_FORMED>
  t_binary operator&(R_binary, T)        noexcept; // BIN_FUNC_6_5_
  template<typename T, t_if_int<T> = traits::WELL_FORMED>
  t_binary operator&(T, R_binary)        noexcept; // BIN_FUNC_6_6_
  template<typename T, t_if_int<T> = traits::WELL_FORMED>
  t_binary operator&(x_binary, T)        noexcept; // BIN_FUNC_6_7_
  template<typename T, t_if_int<T> = traits::WELL_FORMED>
  t_binary operator&(T, x_binary)        noexcept; // BIN_FUNC_6_8_

  /////////////////////////////////////////////////////////////////////////////

  t_binary operator^(R_binary, R_binary) noexcept; // BIN_FUNC_7_1_
  t_binary operator^(x_binary, R_binary) noexcept; // BIN_FUNC_7_2_
  t_binary operator^(R_binary, x_binary) noexcept; // BIN_FUNC_7_3_
  t_binary operator^(x_binary, x_binary) noexcept; // BIN_FUNC_7_4_

  template<typename T, t_if_int<T> = traits::WELL_FORMED>
  t_binary operator^(R_binary, T)        noexcept; // BIN_FUNC_7_5_
  template<typename T, t_if_int<T> = traits::WELL_FORMED>
  t_binary operator^(T, R_binary)        noexcept; // BIN_FUNC_7_6_
  template<typename T, t_if_int<T> = traits::WELL_FORMED>
  t_binary operator^(x_binary, T)        noexcept; // BIN_FUNC_7_7_
  template<typename T, t_if_int<T> = traits::WELL_FORMED>
  t_binary operator^(T, x_binary)        noexcept; // BIN_FUNC_7_8_

  /////////////////////////////////////////////////////////////////////////////

  t_binary operator>>(t_binary, t_bits) noexcept; // BIN_FUNC_8_
  t_binary operator<<(t_binary, t_bits) noexcept; // BIN_FUNC_9_

  /////////////////////////////////////////////////////////////////////////////

  t_bool operator==(R_binary, R_binary) noexcept; // BIN_FUNC_10_1_

  template<typename T, t_if_int<T> = traits::WELL_FORMED>
  t_bool operator==(R_binary, T)        noexcept; // BIN_FUNC_10_2_
  template<typename T, t_if_int<T> = traits::WELL_FORMED>
  t_bool operator==(T, R_binary)        noexcept; // BIN_FUNC_10_3_

  /////////////////////////////////////////////////////////////////////////////

  t_bool operator!=(R_binary, R_binary) noexcept; // BIN_FUNC_11_1_

  template<typename T, t_if_int<T> = traits::WELL_FORMED>
  t_bool operator!=(R_binary, T)        noexcept; // BIN_FUNC_11_2_
  template<typename T, t_if_int<T> = traits::WELL_FORMED>
  t_bool operator!=(T, R_binary)        noexcept; // BIN_FUNC_11_3_

  /////////////////////////////////////////////////////////////////////////////

  t_bool operator<(R_binary, R_binary) noexcept; // BIN_FUNC_12_1_

  template<typename T, t_if_int<T> = traits::WELL_FORMED>
  t_bool operator<(R_binary, T)        noexcept; // BIN_FUNC_12_2_
  template<typename T, t_if_int<T> = traits::WELL_FORMED>
  t_bool operator<(T, R_binary)        noexcept; // BIN_FUNC_12_3_

  /////////////////////////////////////////////////////////////////////////////

  t_bool operator<=(R_binary, R_binary) noexcept; // BIN_FUNC_13_1_

  template<typename T, t_if_int<T> = traits::WELL_FORMED>
  t_bool operator<=(R_binary, T)        noexcept; // BIN_FUNC_13_2_
  template<typename T, t_if_int<T> = traits::WELL_FORMED>
  t_bool operator<=(T, R_binary)        noexcept; // BIN_FUNC_13_3_

  /////////////////////////////////////////////////////////////////////////////

  t_bool operator>(R_binary, R_binary) noexcept; // BIN_FUNC_14_1_

  template<typename T, t_if_int<T> = traits::WELL_FORMED>
  t_bool operator>(R_binary, T)        noexcept; // BIN_FUNC_14_2_
  template<typename T, t_if_int<T> = traits::WELL_FORMED>
  t_bool operator>(T, R_binary)        noexcept; // BIN_FUNC_14_3_

  /////////////////////////////////////////////////////////////////////////////

  t_bool operator>=(R_binary, R_binary) noexcept; // BIN_FUNC_15_1_

  template<typename T, t_if_int<T> = traits::WELL_FORMED>
  t_bool operator>=(R_binary, T)        noexcept; // BIN_FUNC_15_2_
  template<typename T, t_if_int<T> = traits::WELL_FORMED>
  t_bool operator>=(T, R_binary)        noexcept; // BIN_FUNC_15_3_

  /////////////////////////////////////////////////////////////////////////////

  t_integer operator+(R_integer, R_integer) noexcept; // INT_FUNC_1_1_
  t_integer operator+(x_integer, R_integer) noexcept; // INT_FUNC_1_2_
  t_integer operator+(R_integer, x_integer) noexcept; // INT_FUNC_1_3_
  t_integer operator+(x_integer, x_integer) noexcept; // INT_FUNC_1_4_

  template<typename T, t_if_int<T> = traits::WELL_FORMED>
  t_integer operator+(R_integer, T)         noexcept; // INT_FUNC_1_5_
  template<typename T, t_if_int<T> = traits::WELL_FORMED>
  t_integer operator+(T, R_integer)         noexcept; // INT_FUNC_1_6_
  template<typename T, t_if_int<T> = traits::WELL_FORMED>
  t_integer operator+(x_integer, T)         noexcept; // INT_FUNC_1_7_
  template<typename T, t_if_int<T> = traits::WELL_FORMED>
  t_integer operator+(T, x_integer)         noexcept; // INT_FUNC_1_8_

  /////////////////////////////////////////////////////////////////////////////

  t_integer operator-(R_integer, R_integer) noexcept; // INT_FUNC_2_1_
  t_integer operator-(x_integer, R_integer) noexcept; // INT_FUNC_2_2_
  t_integer operator-(R_integer, x_integer) noexcept; // INT_FUNC_2_3_
  t_integer operator-(x_integer, x_integer) noexcept; // INT_FUNC_2_4_

  template<typename T, t_if_int<T> = traits::WELL_FORMED>
  t_integer operator-(R_integer, T)         noexcept; // INT_FUNC_2_5_
  template<typename T, t_if_int<T> = traits::WELL_FORMED>
  t_integer operator-(T, R_integer)         noexcept; // INT_FUNC_2_6_
  template<typename T, t_if_int<T> = traits::WELL_FORMED>
  t_integer operator-(x_integer, T)         noexcept; // INT_FUNC_2_7_
  template<typename T, t_if_int<T> = traits::WELL_FORMED>
  t_integer operator-(T, x_integer)         noexcept; // INT_FUNC_2_8_

  /////////////////////////////////////////////////////////////////////////////

  t_integer operator*(R_integer, R_integer) noexcept; // INT_FUNC_3_1_
  t_integer operator*(x_integer, R_integer) noexcept; // INT_FUNC_3_2_
  t_integer operator*(R_integer, x_integer) noexcept; // INT_FUNC_3_3_
  t_integer operator*(x_integer, x_integer) noexcept; // INT_FUNC_3_4_

  template<typename T, t_if_int<T> = traits::WELL_FORMED>
  t_integer operator*(R_integer, T)         noexcept; // INT_FUNC_3_5_
  template<typename T, t_if_int<T> = traits::WELL_FORMED>
  t_integer operator*(T, R_integer)         noexcept; // INT_FUNC_3_6_
  template<typename T, t_if_int<T> = traits::WELL_FORMED>
  t_integer operator*(x_integer, T)         noexcept; // INT_FUNC_3_7_
  template<typename T, t_if_int<T> = traits::WELL_FORMED>
  t_integer operator*(T, x_integer)         noexcept; // INT_FUNC_3_8_

  /////////////////////////////////////////////////////////////////////////////

  t_integer operator/(R_integer, R_integer) noexcept; // INT_FUNC_4_1_
  t_integer operator/(x_integer, R_integer) noexcept; // INT_FUNC_4_2_
  t_integer operator/(R_integer, x_integer) noexcept; // INT_FUNC_4_3_
  t_integer operator/(x_integer, x_integer) noexcept; // INT_FUNC_4_4_

  template<typename T, t_if_int<T> = traits::WELL_FORMED>
  t_integer operator/(R_integer, T)         noexcept; // INT_FUNC_4_5_
  template<typename T, t_if_int<T> = traits::WELL_FORMED>
  t_integer operator/(T, R_integer)         noexcept; // INT_FUNC_4_6_
  template<typename T, t_if_int<T> = traits::WELL_FORMED>
  t_integer operator/(x_integer, T)         noexcept; // INT_FUNC_4_7_
  template<typename T, t_if_int<T> = traits::WELL_FORMED>
  t_integer operator/(T, x_integer)         noexcept; // INT_FUNC_4_8_

  /////////////////////////////////////////////////////////////////////////////

  t_bool operator==(R_integer, R_integer) noexcept; // INT_FUNC_5_1_

  template<typename T, t_if_int<T> = traits::WELL_FORMED>
  t_bool operator==(R_integer, T)         noexcept; // INT_FUNC_5_2_
  template<typename T, t_if_int<T> = traits::WELL_FORMED>
  t_bool operator==(T, R_integer)         noexcept; // INT_FUNC_5_3_

  /////////////////////////////////////////////////////////////////////////////

  t_bool operator!=(R_integer, R_integer) noexcept; // INT_FUNC_6_1_

  template<typename T, t_if_int<T> = traits::WELL_FORMED>
  t_bool operator!=(R_integer, T)         noexcept; // INT_FUNC_6_2_
  template<typename T, t_if_int<T> = traits::WELL_FORMED>
  t_bool operator!=(T, R_integer)         noexcept; // INT_FUNC_6_3_

  /////////////////////////////////////////////////////////////////////////////

  t_bool operator<(R_integer, R_integer) noexcept; // INT_FUNC_7_1_

  template<typename T, t_if_int<T> = traits::WELL_FORMED>
  t_bool operator<(R_integer, T)         noexcept; // INT_FUNC_7_2_
  template<typename T, t_if_int<T> = traits::WELL_FORMED>
  t_bool operator<(T, R_integer)         noexcept; // INT_FUNC_7_3_

  /////////////////////////////////////////////////////////////////////////////

  t_bool operator<=(R_integer, R_integer) noexcept; // INT_FUNC_8_1_

  template<typename T, t_if_int<T> = traits::WELL_FORMED>
  t_bool operator<=(R_integer, T)         noexcept; // INT_FUNC_8_2_
  template<typename T, t_if_int<T> = traits::WELL_FORMED>
  t_bool operator<=(T, R_integer)         noexcept; // INT_FUNC_8_3_

  /////////////////////////////////////////////////////////////////////////////

  t_bool operator>(R_integer, R_integer) noexcept; // INT_FUNC_9_1_

  template<typename T, t_if_int<T> = traits::WELL_FORMED>
  t_bool operator>(R_integer, T)         noexcept; // INT_FUNC_9_2_
  template<typename T, t_if_int<T> = traits::WELL_FORMED>
  t_bool operator>(T, R_integer)         noexcept; // INT_FUNC_9_3_

  /////////////////////////////////////////////////////////////////////////////

  t_bool operator>=(R_integer, R_integer) noexcept; // INT_FUNC_10_1_

  template<typename T, t_if_int<T> = traits::WELL_FORMED>
  t_bool operator>=(R_integer, T)         noexcept; // INT_FUNC_10_2_
  template<typename T, t_if_int<T> = traits::WELL_FORMED>
  t_bool operator>=(T, R_integer)         noexcept; // INT_FUNC_10_3_

  /////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////

  // BIN_METHOD_1_1_
  inline
  t_binary::t_binary() noexcept : impl_{store_} {
  }

  // BIN_METHOD_1_2_
  inline
  t_binary::t_binary(t_bits max) noexcept
    : store_{impl_::calc_size_(max)}, impl_{store_} {
  }

  // BIN_METHOD_1_3_
  inline
  t_binary::t_binary(R_binary value) noexcept : store_{value.store_} {
  }

  // BIN_METHOD_1_4_
  inline
  t_binary::t_binary(t_bits max, R_binary value) noexcept
    : store_{max_of<impl_::t_n>(impl_::calc_size_(max), value.store_.size)},
      impl_{store_, value.store_} {
  }

  // BIN_METHOD_1_5_
  inline
  t_binary::t_binary(x_binary value) noexcept
    : store_{util::x_cast(value.store_)} {
  }

  // BIN_METHOD_1_6_
  inline
  t_binary::t_binary(t_bits max, x_binary value) noexcept
    : store_{impl_::calc_size_(max), util::x_cast(value.store_)},
      impl_{store_, value.store_} {
  }

  // BIN_METHOD_1_7_
  template<typename T, t_if_neg<T>>
  inline
  t_binary::t_binary(T value) noexcept
    : impl_{store_, static_cast<impl_::t_nvalue_>(value)} {
  }

  // BIN_METHOD_1_8_
  template<typename T, t_if_neg<T>>
  inline
  t_binary::t_binary(t_bits max, T value) noexcept
    : store_{impl_::calc_size_(max)},
      impl_{store_, static_cast<impl_::t_nvalue_>(value)} {
  }

  // BIN_METHOD_1_9_
  template<typename T, t_if_pos<T>>
  inline
  t_binary::t_binary(T value) noexcept
    : impl_{store_, static_cast<impl_::t_pvalue_>(value)} {
  }

  // BIN_METHOD_1_10_
  template<typename T, t_if_pos<T>>
  inline
  t_binary::t_binary(t_bits max, T value) noexcept
    : store_{impl_::calc_size_(max)},
      impl_{store_, static_cast<impl_::t_pvalue_>(value)} {
  }

  /////////////////////////////////////////////////////////////////////////////

  // BIN_METHOD_2_1_
  inline
  r_binary t_binary::operator=(R_binary value) noexcept {
    if (get(store_.size) <= get(value.store_.size))
      store_.ensure(value.store_);
    else
      impl_.ensure(store_, value.store_);
    return *this;
  }

  // BIN_METHOD_2_2_
  inline
  r_binary t_binary::operator=(x_binary value) noexcept {
    if (get(store_.size) <= get(value.store_.size))
      store_.ensure(util::x_cast(value.store_));
    else
      impl_.ensure(store_, value.store_);
    return *this;
  }

  // BIN_METHOD_2_3_
  template<typename T, t_if_neg<T>>
  inline
  r_binary t_binary::operator=(T value) noexcept {
    impl_.assign(store_, static_cast<impl_::t_nvalue_>(value));
    return *this;
  }

  // BIN_METHOD_2_4_
  template<typename T, t_if_pos<T>>
  inline
  r_binary t_binary::operator=(T value) noexcept {
    impl_.assign(store_, static_cast<impl_::t_pvalue_>(value));
    return *this;
  }

  /////////////////////////////////////////////////////////////////////////////

  // BIN_METHOD_3_1_
  inline
  r_binary t_binary::operator+=(R_binary value) noexcept {
    impl_.addition(store_, value.store_);
    return *this;
  }

  // BIN_METHOD_3_2_
  template<typename T, t_if_neg<T>>
  inline
  r_binary t_binary::operator+=(T value) noexcept {
    impl_.addition(store_, static_cast<impl_::t_nvalue_>(value));
    return *this;
  }

  // BIN_METHOD_3_3_
  template<typename T, t_if_pos<T>>
  inline
  r_binary t_binary::operator+=(T value) noexcept {
    impl_.addition(store_, static_cast<impl_::t_pvalue_>(value));
    return *this;
  }

  /////////////////////////////////////////////////////////////////////////////

  // BIN_METHOD_4_1_
  inline
  r_binary t_binary::operator-=(R_binary value) noexcept {
    impl_.subtraction(store_, value.store_);
    return *this;
  }

  // BIN_METHOD_4_2_
  template<typename T, t_if_neg<T>>
  inline
  r_binary t_binary::operator-=(T value) noexcept {
    impl_.subtraction(store_, static_cast<impl_::t_nvalue_>(value));
    return *this;
  }

  // BIN_METHOD_4_3_
  template<typename T, t_if_pos<T>>
  inline
  r_binary t_binary::operator-=(T value) noexcept {
    impl_.subtraction(store_, static_cast<impl_::t_pvalue_>(value));
    return *this;
  }

  /////////////////////////////////////////////////////////////////////////////

  // BIN_METHOD_5_1_
  inline
  r_binary t_binary::operator*=(R_binary value) noexcept {
    impl_.multiplication(store_, value.store_);
    return *this;
  }

  // BIN_METHOD_5_2_
  template<typename T, t_if_neg<T>>
  inline
  r_binary t_binary::operator*=(T value) noexcept {
    impl_.multiplication(store_, static_cast<impl_::t_nvalue_>(value));
    return *this;
  }

  // BIN_METHOD_5_3_
  template<typename T, t_if_pos<T>>
  inline
  r_binary t_binary::operator*=(T value) noexcept {
    impl_.multiplication(store_, static_cast<impl_::t_pvalue_>(value));
    return *this;
  }

  /////////////////////////////////////////////////////////////////////////////

  // BIN_METHOD_6_1_
  inline
  r_binary t_binary::operator/=(R_binary value) noexcept {
    impl_.division(store_, value.store_);
    return *this;
  }

  // BIN_METHOD_6_2_
  template<typename T, t_if_neg<T>>
  inline
  r_binary t_binary::operator/=(T value) noexcept {
    impl_.division(store_, static_cast<impl_::t_nvalue_>(value));
    return *this;
  }

  // BIN_METHOD_6_3_
  template<typename T, t_if_pos<T>>
  inline
  r_binary t_binary::operator/=(T value) noexcept {
    impl_.division(store_, static_cast<impl_::t_pvalue_>(value));
    return *this;
  }

  /////////////////////////////////////////////////////////////////////////////

  // BIN_METHOD_7_1_
  inline
  r_binary t_binary::operator&=(R_binary value) noexcept {
    impl_.binary_and(store_, value.store_);
    return *this;
  }

  // BIN_METHOD_7_2_
  template<typename T, t_if_neg<T>>
  inline
  r_binary t_binary::operator&=(T value) noexcept {
    impl_.binary_and(store_, static_cast<impl_::t_nvalue_>(value));
    return *this;
  }

  // BIN_METHOD_7_3_
  template<typename T, t_if_pos<T>>
  inline
  r_binary t_binary::operator&=(T value) noexcept {
    impl_.binary_and(store_, static_cast<impl_::t_pvalue_>(value));
    return *this;
  }

  /////////////////////////////////////////////////////////////////////////////

  // BIN_METHOD_8_1_
  inline
  r_binary t_binary::operator|=(R_binary value) noexcept {
    impl_.binary_or(store_, value.store_);
    return *this;
  }

  // BIN_METHOD_8_2_
  template<typename T, t_if_neg<T>>
  inline
  r_binary t_binary::operator|=(T value) noexcept {
    impl_.binary_or(store_, static_cast<impl_::t_nvalue_>(value));
    return *this;
  }

  // BIN_METHOD_8_3_
  template<typename T, t_if_pos<T>>
  inline
  r_binary t_binary::operator|=(T value) noexcept {
    impl_.binary_or(store_, static_cast<impl_::t_pvalue_>(value));
    return *this;
  }

  /////////////////////////////////////////////////////////////////////////////

  // BIN_METHOD_9_1_
  inline
  r_binary t_binary::operator^=(R_binary value) noexcept {
    impl_.binary_xor(store_, value.store_);
    return *this;
  }

  // BIN_METHOD_9_2_
  template<typename T, t_if_neg<T>>
  inline
  r_binary t_binary::operator^=(T value) noexcept {
    impl_.binary_xor(store_, static_cast<impl_::t_nvalue_>(value));
    return *this;
  }

  // BIN_METHOD_9_3_
  template<typename T, t_if_pos<T>>
  inline
  r_binary t_binary::operator^=(T value) noexcept {
    impl_.binary_xor(store_, static_cast<impl_::t_pvalue_>(value));
    return *this;
  }

  /////////////////////////////////////////////////////////////////////////////

  // BIN_METHOD_10_
  inline
  t_binary t_binary::operator-() const noexcept {
    t_binary tmp(*this);
    tmp.twos_complement();
    return tmp;
  }

  // BIN_METHOD_11_
  inline
  t_binary t_binary::operator~() const noexcept {
    t_binary tmp(*this);
    tmp.ones_complement();
    return tmp;
  }

  // BIN_METHOD_12_
  inline
  r_binary t_binary::operator>>=(t_bits n) noexcept {
    impl_.shift_right(store_, n);
    return *this;
  }

  // BIN_METHOD_13_
  inline
  r_binary t_binary::operator<<=(t_bits n) noexcept {
    impl_.shift_left(store_, n);
    return *this;
  }

  // BIN_METHOD_14_
  inline
  r_binary t_binary::ones_complement() noexcept {
    impl_.ones_complement(store_);
    return *this;
  }

  // BIN_METHOD_15_
  inline
  r_binary t_binary::twos_complement() noexcept {
    impl_.twos_complement(store_);
    return *this;
  }

  // BIN_METHOD_16_
  inline
  r_binary t_binary::shift_left(t_bits n) noexcept {
    impl_.shift_left(store_, n);
    return *this;
  }

  // BIN_METHOD_17_
  inline
  r_binary t_binary::shift_right(t_bits n) noexcept {
    impl_.shift_right(store_, n);
    return *this;
  }

  // BIN_METHOD_18_
  inline
  t_bool t_binary::set_bit(t_bit bit, t_bool on) noexcept {
    return impl_.set_bit(store_, bit, on);
  }

  // BIN_METHOD_19_
  inline
  t_bool t_binary::ensure_bits(t_bits max) noexcept {
    return impl_.ensure(store_, max);
  }

  // BIN_METHOD_20_
  inline
  t_binary::operator t_bool() const noexcept {
    return !impl_.is_zero(store_);
  }

  // BIN_METHOD_21_
  inline
  t_bool t_binary::operator[](t_bit bit) const noexcept {
    return impl_.get_bit(store_, bit);
  }

  // BIN_METHOD_22_
  inline
  t_bool t_binary::get_bit(t_bit bit) const noexcept {
    return impl_.get_bit(store_, bit);
  }

  // BIN_METHOD_23_
  inline
  t_bits t_binary::get_bits() const noexcept {
    return impl_.get_bits(store_);
  }

  // BIN_METHOD_24_
  inline
  t_bool t_binary::is_negative() const noexcept {
    return impl_.is_negative(store_);
  }

  // BIN_METHOD_25_
  inline
  t_void t_binary::display() const noexcept {
    impl_.display(store_);
  }

  // BIN_METHOD_26_
  inline
  t_bool t_binary::ms_bit() const noexcept {
    return impl_.ms_bit(store_);
  }

  // BIN_METHOD_27_
  inline
  t_bool t_binary::ls_bit() const noexcept {
    return impl_.ls_bit(store_);
  }

  // BIN_METHOD_28_
  inline
  t_bit t_binary::first_on_bit() const noexcept {
    return impl_.first_on_bit(store_);
  }

  // BIN_METHOD_29_
  inline
  t_bit t_binary::last_on_bit() const noexcept {
    return impl_.last_on_bit(store_);
  }

  // BIN_METHOD_30_
  inline
  t_bits t_binary::on_bits() const noexcept {
    return impl_.on_bits(store_);
  }

  // BIN_METHOD_31_
  inline
  t_info t_binary::get_info() const noexcept {
    return impl_.get_info(store_);
  }

  /////////////////////////////////////////////////////////////////////////////

  // BIN_METHOD_32_1_
  inline
  t_void t_binary::reset() noexcept {
    impl_.reset(store_);
  }

  // BIN_METHOD_32_2_
  inline
  t_bool t_binary::reset(R_binary value) noexcept {
    return store_.reset(value.store_);
  }

  // BIN_METHOD_32_3_
  inline
  t_bool t_binary::reset(t_bits max, R_binary value) noexcept {
    return store_.reset(impl_::calc_size_(max), value.store_);
  }

  // BIN_METHOD_32_4_
  template<typename T, t_if_neg<T>>
  inline
  t_void t_binary::reset(T value) noexcept {
    impl_.reset(store_, static_cast<impl_::t_nvalue_>(value));
  }

  // BIN_METHOD_32_5_
  template<typename T, t_if_neg<T>>
  inline
  t_bool t_binary::reset(t_bits max, T value) noexcept {
    return impl_.reset(store_, max, static_cast<impl_::t_nvalue_>(value));
  }

  // BIN_METHOD_32_6_
  template<typename T, t_if_pos<T>>
  inline
  t_void t_binary::reset(T value) noexcept {
    impl_.reset(store_, static_cast<impl_::t_pvalue_>(value));
  }

  // BIN_METHOD_32_7_
  template<typename T, t_if_pos<T>>
  inline
  t_bool t_binary::reset(t_bits max, T value) noexcept {
    return impl_.reset(store_, max, static_cast<impl_::t_pvalue_>(value));
  }

  /////////////////////////////////////////////////////////////////////////////

  // BIN_METHOD_33_1_
  inline
  t_bool t_binary::is_equal(R_binary value) const noexcept {
    return impl_.is_equal(store_, value.store_);
  }

  // BIN_METHOD_33_2_
  template<typename T, t_if_neg<T>>
  inline
  t_bool t_binary::is_equal(T value) const noexcept {
    return impl_.is_equal(store_, static_cast<impl_::t_nvalue_>(value));
  }

  // BIN_METHOD_33_3_
  template<typename T, t_if_pos<T>>
  inline
  t_bool t_binary::is_equal(T value) const noexcept {
    return impl_.is_equal(store_, static_cast<impl_::t_pvalue_>(value));
  }

  /////////////////////////////////////////////////////////////////////////////

  // BIN_METHOD_34_1_
  inline
  t_bool t_binary::is_less(R_binary value) const noexcept {
    return impl_.is_less(store_, value.store_);
  }

  // BIN_METHOD_34_2_
  template<typename T, t_if_neg<T>>
  inline
  t_bool t_binary::is_less(T value) const noexcept {
    return impl_.is_less(store_, static_cast<impl_::t_nvalue_>(value));
  }

  // BIN_METHOD_34_3_
  template<typename T, t_if_pos<T>>
  inline
  t_bool t_binary::is_less(T value) const noexcept {
    return impl_.is_less(store_, static_cast<impl_::t_pvalue_>(value));
  }

  /////////////////////////////////////////////////////////////////////////////

  // BIN_METHOD_35_1_
  inline
  t_bool t_binary::is_less_equal(R_binary value) const noexcept {
    return impl_.is_less_equal(store_, value.store_);
  }

  // BIN_METHOD_35_2_
  template<typename T, t_if_neg<T>>
  inline
  t_bool t_binary::is_less_equal(T value) const noexcept {
    return impl_.is_less_equal(store_, static_cast<impl_::t_nvalue_>(value));
  }

  // BIN_METHOD_35_3_
  template<typename T, t_if_pos<T>>
  inline
  t_bool t_binary::is_less_equal(T value) const noexcept {
    return impl_.is_less_equal(store_, static_cast<impl_::t_pvalue_>(value));
  }

  /////////////////////////////////////////////////////////////////////////////

  // INT_METHOD_1_1_
  inline
  t_integer::t_integer() noexcept {
  }

  // INT_METHOD_1_2_
  inline
  t_integer::t_integer(t_digits max) noexcept
    : bin_{impl_::calc_bits_(max)} {
  }

  // INT_METHOD_1_3__
  inline
  t_integer::t_integer(R_integer value) noexcept : bin_{value} {
  }

  // INT_METHOD_1_4_
  inline
  t_integer::t_integer(t_digits max, R_integer value) noexcept
    : bin_{impl_::calc_bits_(max), value} {
  }

  // INT_METHOD_1_5_
  inline
  t_integer::t_integer(x_integer value) noexcept
    : bin_{static_cast<x_binary>(util::x_cast(value))} {
  }

  // INT_METHOD_1_6_
  inline
  t_integer::t_integer(t_digits max, x_integer value) noexcept
    : bin_{impl_::calc_bits_(max),
           static_cast<x_binary>(util::x_cast(value))} {
  }

  // INT_METHOD_1_7__
  inline
  t_integer::t_integer(R_binary value) noexcept : bin_{value} {
  }

  // INT_METHOD_1_8_
  inline
  t_integer::t_integer(t_digits max, R_binary value) noexcept
    : bin_{impl_::calc_bits_(max), value} {
  }

  // INT_METHOD_1_9_
  inline
  t_integer::t_integer(x_binary value) noexcept
    : bin_{util::x_cast(value)} {
  }

  // INT_METHOD_1_10_
  inline
  t_integer::t_integer(t_digits max, x_binary value) noexcept
    : bin_{impl_::calc_bits_(max), util::x_cast(value)} {
  }

  // INT_METHOD_1_11_
  template<typename T, t_if_int<T>>
  inline
  t_integer::t_integer(T value) noexcept : bin_{value} {
  }

  // INT_METHOD_1_12_
  template<typename T, t_if_int<T>>
  inline
  t_integer::t_integer(t_digits max, T value) noexcept
    : bin_{impl_::calc_bits_(max), value} {
  }

  /////////////////////////////////////////////////////////////////////////////

  // INT_METHOD_2_1_
  inline
  r_integer t_integer::operator=(R_integer value) noexcept {
    bin_ = value;
    return *this;
  }

  // INT_METHOD_2_2_
  inline
  r_integer t_integer::operator=(x_integer value) noexcept {
    bin_ = static_cast<x_binary>(util::x_cast(value));
    return *this;
  }

  // INT_METHOD_2_3_
  inline
  r_integer t_integer::operator=(R_binary value) noexcept {
    bin_ = value;
    return *this;
  }

  // INT_METHOD_2_4_
  inline
  r_integer t_integer::operator=(x_binary value) noexcept {
    bin_ = util::x_cast(value);
    return *this;
  }

  // INT_METHOD_2_5_
  template<typename T, t_if_int<T>>
  inline
  r_integer t_integer::operator=(T value) noexcept {
    bin_ = value;
    return *this;
  }

  /////////////////////////////////////////////////////////////////////////////

  // INT_METHOD_3_1_
  inline
  r_integer t_integer::operator+=(R_integer value) noexcept {
    bin_ += value;
    return *this;
  }

  // INT_METHOD_3_2_
  inline
  r_integer t_integer::operator+=(R_binary value) noexcept {
    bin_ += value;
    return *this;
  }

  // INT_METHOD_3_3_
  template<typename T, t_if_int<T>>
  inline
  r_integer t_integer::operator+=(T value) noexcept {
    bin_ += value;
    return *this;
  }

  /////////////////////////////////////////////////////////////////////////////

  // INT_METHOD_4_1_
  inline
  r_integer t_integer::operator-=(R_integer value) noexcept {
    bin_ -= value;
    return *this;
  }

  // INT_METHOD_4_2_
  inline
  r_integer t_integer::operator-=(R_binary value) noexcept {
    bin_ -= value;
    return *this;
  }

  // INT_METHOD_4_3_
  template<typename T, t_if_int<T>>
  inline
  r_integer t_integer::operator-=(T value) noexcept {
    bin_ -= value;
    return *this;
  }

  /////////////////////////////////////////////////////////////////////////////

  // INT_METHOD_5_1_
  inline
  r_integer t_integer::operator*=(R_integer value) noexcept {
    bin_ *= value;
    return *this;
  }

  // INT_METHOD_5_2_
  inline
  r_integer t_integer::operator*=(R_binary value) noexcept {
    bin_ *= value;
    return *this;
  }

  // INT_METHOD_5_3_
  template<typename T, t_if_int<T>>
  inline
  r_integer t_integer::operator*=(T value) noexcept {
    bin_ *= value;
    return *this;
  }

  /////////////////////////////////////////////////////////////////////////////

  // INT_METHOD_6_1_
  inline
  r_integer t_integer::operator/=(R_integer value) noexcept {
    bin_ /= value;
    return *this;
  }

  // INT_METHOD_6_2_
  inline
  r_integer t_integer::operator/=(R_binary value) noexcept {
    bin_ /= value;
    return *this;
  }

  // INT_METHOD_6_3_
  template<typename T, t_if_int<T>>
  inline
  r_integer t_integer::operator/=(T value) noexcept {
    bin_ /= value;
    return *this;
  }

  /////////////////////////////////////////////////////////////////////////////

  // INT_METHOD_7_
  inline
  t_bool t_integer::ensure_digits(t_digits max) noexcept {
    return bin_.ensure_bits(impl_::calc_bits_(max));
  }

  // INT_METHOD_8_
  inline
  t_integer t_integer::operator-() const noexcept {
    return t_integer{-bin_};
  }

  // INT_METHOD_9_
  inline
  t_digits t_integer::get_digits() const noexcept {
    return impl_::calc_digits_(bin_.get_bits());
  }

  // INT_METHOD_10_
  inline
  t_bool t_integer::is_negative() const noexcept {
    return bin_.is_negative();
  }

  /////////////////////////////////////////////////////////////////////////////

  // INT_METHOD_11_1_
  inline
  t_integer::operator x_binary() && noexcept {
    return util::x_cast(bin_);
  }

  // INT_METHOD_11_2_
  inline
  t_integer::operator R_binary() const noexcept {
    return bin_;
  }

  // INT_METHOD_11_3_
  inline
  t_integer::operator t_bool() const noexcept {
    return bin_;
  }

  /////////////////////////////////////////////////////////////////////////////

  // INT_METHOD_12_1_
  inline
  t_void t_integer::reset() noexcept {
    bin_.reset();
  }

  // INT_METHOD_12_2_
  inline
  t_bool t_integer::reset(R_integer value) noexcept {
    return bin_.reset(value);
  }

  // INT_METHOD_12_3_
  inline
  t_bool t_integer::reset(t_digits max, R_integer value) noexcept {
    return bin_.reset(impl_::calc_bits_(max), value);
  }

  // INT_METHOD_12_4_
  inline
  t_bool t_integer::reset(R_binary value) noexcept {
    return bin_.reset(value);
  }

  // INT_METHOD_12_5_
  inline
  t_bool t_integer::reset(t_digits max, R_binary value) noexcept {
    return bin_.reset(impl_::calc_bits_(max), value);
  }

  // INT_METHOD_12_6_
  template<typename T, t_if_int<T>>
  inline
  t_void t_integer::reset(T value) noexcept {
    bin_.reset(value);
  }

  // INT_METHOD_12_7_
  template<typename T, t_if_int<T>>
  inline
  t_bool t_integer::reset(t_digits max, T value) noexcept {
    return bin_.reset(impl_::calc_bits_(max), value);
  }

  /////////////////////////////////////////////////////////////////////////////

  // INT_METHOD_13_1_
  inline
  t_bool t_integer::is_equal(R_integer value) const noexcept {
    return bin_.is_equal(value);
  }

  // INT_METHOD_13_2_
  inline
  t_bool t_integer::is_equal(R_binary value) const noexcept {
    return bin_.is_equal(value);
  }

  // INT_METHOD_13_3_
  template<typename T, t_if_int<T>>
  inline
  t_bool t_integer::is_equal(T value) const noexcept {
    return bin_.is_equal(value);
  }

  /////////////////////////////////////////////////////////////////////////////

  // INT_METHOD_14_1_
  inline
  t_bool t_integer::is_less(R_integer value) const noexcept {
    return bin_.is_less(value);
  }

  // INT_METHOD_14_2_
  inline
  t_bool t_integer::is_less(R_binary value) const noexcept {
    return bin_.is_less(value);
  }

  // INT_METHOD_14_3_
  template<typename T, t_if_int<T>>
  inline
  t_bool t_integer::is_less(T value) const noexcept {
    return bin_.is_less(value);
  }

  /////////////////////////////////////////////////////////////////////////////

  // INT_METHOD_15_1_
  inline
  t_bool t_integer::is_less_equal(R_integer value) const noexcept {
    return bin_.is_less_equal(value);
  }

  // INT_METHOD_15_2_
  inline
  t_bool t_integer::is_less_equal(R_binary value) const noexcept {
    return bin_.is_less_equal(value);
  }

  // INT_METHOD_15_3_
  template<typename T, t_if_int<T>>
  inline
  t_bool t_integer::is_less_equal(T value) const noexcept {
    return bin_.is_less_equal(value);
  }

  /////////////////////////////////////////////////////////////////////////////

  #define DAINTY_BASE_NUMERIC_OP_R_R_(type, op, lh, rh) \
      type result(lh); \
      result op rh; \
      return result;

  #define DAINTY_BASE_NUMERIC_OP_X_R_(op, lh, rh) \
      lh op rh; \
      return util::x_cast(lh);

  #define DAINTY_BASE_NUMERIC_OP_R_X_(type, op, lh, rh) \
      type result(lh); \
      result op rh; \
      return result;

  #define DAINTY_BASE_NUMERIC_OP_R_X_COMMUTATIVE_(op, lh, rh) \
      rh op lh; \
      return util::x_cast(rh);

  #define DAINTY_BASE_NUMERIC_OP_X_X_(op, lh, rh) \
      lh op rh; \
      return lh;

  #define DAINTY_BASE_NUMERIC_OP_X_X_COMMUTATIVE_(op, lh_larger, lh, rh) \
      if (lh_larger) { \
        lh op rh; \
        return util::x_cast(lh); \
      } \
      rh op lh; \
      return util::x_cast(rh);

  #define DAINTY_BASE_NUMERIC_OP_R_T_(type, op, lh, rh) \
      type result(lh); \
      result op rh; \
      return result;

  #define DAINTY_BASE_NUMERIC_OP_T_R_(type, op, lh, rh) \
      type result(lh); \
      result op rh; \
      return result;

  #define DAINTY_BASE_NUMERIC_OP_X_T_(op, lh, rh) \
      lh op rh; \
      return util::x_cast(lh);

  #define DAINTY_BASE_NUMERIC_OP_T_X_(type, op, lh, rh) \
      type result(lh); \
      result op rh; \
      return util::x_cast(rh);

  #define DAINTY_BASE_NUMERIC_OP_T_X_COMMUTATIVE_(op, lh, rh) \
      rh op lh; \
      return util::x_cast(rh);

  /////////////////////////////////////////////////////////////////////////////

  // BIN_FUNC_1_1
  inline
  t_binary operator+(R_binary lh, R_binary rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_R_R_(t_binary, +=, lh, rh)
  }

  // BIN_FUNC_1_2_
  inline
  t_binary operator+(x_binary lh, R_binary rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_X_R_(+=, lh, rh)
  }

  // BIN_FUNC_1_3_
  inline
  t_binary operator+(R_binary lh, x_binary rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_R_X_COMMUTATIVE_(+=, lh, rh)
  }

  // BIN_FUNC_1_4_
  inline
  t_binary operator+(x_binary lh, x_binary rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_X_X_COMMUTATIVE_(+=, get(lh.get_bits()) > get(rh.get_bits()), lh, rh)
  }

  // BIN_FUNC_1_5_
  template<typename T, t_if_int<T>>
  inline
  t_binary operator+(R_binary lh, T rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_R_T_(t_binary, +=, lh, rh)
  }

  // BIN_FUNC_1_6_
  template<typename T, t_if_int<T>>
  inline
  t_binary operator+(T lh, R_binary rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_T_R_(t_binary, +=, lh, rh)
  }

  // BIN_FUNC_1_7_
  template<typename T, t_if_int<T>>
  inline
  t_binary operator+(x_binary lh, T rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_X_T_(+=, lh, rh)
  }

  // BIN_FUNC_1_8_
  template<typename T, t_if_int<T>>
  inline
  t_binary operator+(T lh, x_binary rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_T_X_COMMUTATIVE_(+=, lh, rh)
  }

  /////////////////////////////////////////////////////////////////////////////

  // BIN_FUNC_2_1_
  inline
  t_binary operator-(R_binary lh, R_binary rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_R_R_(t_binary, -=, lh, rh)
  }

  // BIN_FUNC_2_2_
  inline
  t_binary operator-(x_binary lh, R_binary rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_X_R_(-=, lh, rh)
  }

  // BIN_FUNC_2_3_
  inline
  t_binary operator-(R_binary lh, x_binary rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_R_X_(t_binary, -=, lh, rh)
  }

  // BIN_FUNC_2_4_
  inline
  t_binary operator-(x_binary lh, x_binary rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_X_X_(-=, lh, rh)
  }

  // BIN_FUNC_2_5_
  template<typename T, t_if_int<T>>
  inline
  t_binary operator-(R_binary lh, T rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_R_T_(t_binary, -=, lh, rh)
  }

  // BIN_FUNC_2_6_
  template<typename T, t_if_int<T>>
  inline
  t_binary operator-(T lh, R_binary rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_T_R_(t_binary, -=, lh, rh)
  }

  // BIN_FUNC_2_7_
  template<typename T, t_if_int<T>>
  inline
  t_binary operator-(x_binary lh, T rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_X_T_(-=, lh, rh)
  }

  // BIN_FUNC_2_8_
  template<typename T, t_if_int<T>>
  inline
  t_binary operator-(T lh, x_binary rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_T_X_(t_binary, -=, lh, rh)
  }

  /////////////////////////////////////////////////////////////////////////////

  // BIN_FUNC_3_1_
  inline
  t_binary operator*(R_binary lh, R_binary rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_R_R_(t_binary, *=, lh, rh)
  }

  // BIN_FUNC_3_2_
  inline
  t_binary operator*(x_binary lh, R_binary rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_X_R_(*=, lh, rh)
  }

  // BIN_FUNC_3_3_
  inline
  t_binary operator*(R_binary lh, x_binary rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_R_X_COMMUTATIVE_(*=, lh, rh)
  }

  // BIN_FUNC_3_4_
  inline
  t_binary operator*(x_binary lh, x_binary rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_X_X_COMMUTATIVE_(*=, get(lh.get_bits()) > get(rh.get_bits()), lh, rh)
  }

  // BIN_FUNC_3_5_
  template<typename T, t_if_int<T>>
  inline
  t_binary operator*(R_binary lh, T rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_R_T_(t_binary, *=, lh, rh)
  }

  // BIN_FUNC_3_6_
  template<typename T, t_if_int<T>>
  inline
  t_binary operator*(T lh, R_binary rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_T_R_(t_binary, *=, lh, rh)
  }

  // BIN_FUNC_3_7_
  template<typename T, t_if_int<T>>
  inline
  t_binary operator*(x_binary lh, T rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_X_T_(*=, lh, rh)
  }

  // BIN_FUNC_3_8_
  template<typename T, t_if_int<T>>
  inline
  t_binary operator*(T lh, x_binary rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_T_X_COMMUTATIVE_(*=, lh, rh)
  }

  /////////////////////////////////////////////////////////////////////////////

  // BIN_FUNC_4_1_
  inline
  t_binary operator/(R_binary lh, R_binary rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_R_R_(t_binary, /=, lh, rh)
  }

  // BIN_FUNC_4_2_
  inline
  t_binary operator/(x_binary lh, R_binary rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_X_R_(/=, lh, rh)
  }

  // BIN_FUNC_4_3_
  inline
  t_binary operator/(R_binary lh, x_binary rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_R_X_(t_binary, /=, lh, rh)
  }

  // BIN_FUNC_4_4_
  inline
  t_binary operator/(x_binary lh, x_binary rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_X_X_(/=, lh, rh)
  }

  // BIN_FUNC_4_5_
  template<typename T, t_if_int<T>>
  inline
  t_binary operator/(R_binary lh, T rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_R_T_(t_binary, /=, lh, rh)
  }

  // BIN_FUNC_4_6_
  template<typename T, t_if_int<T>>
  inline
  t_binary operator/(T lh, R_binary rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_T_R_(t_binary, /=, lh, rh)
  }

  // BIN_FUNC_4_7_
  template<typename T, t_if_int<T>>
  inline
  t_binary operator/(x_binary lh, T rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_X_T_(/=, lh, rh)
  }

  // BIN_FUNC_4_8_
  template<typename T, t_if_int<T>>
  inline
  t_binary operator/(T lh, x_binary rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_T_X_(t_binary, /=, lh, rh)
  }

  /////////////////////////////////////////////////////////////////////////////

  // BIN_FUNC_5_1_
  inline
  t_binary operator|(R_binary lh, R_binary rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_R_R_(t_binary, |=, lh, rh)
  }

  // BIN_FUNC_5_2_
  inline
  t_binary operator|(x_binary lh, R_binary rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_X_R_(|=, lh, rh)
  }

  // BIN_FUNC_5_3_
  inline
  t_binary operator|(R_binary lh, x_binary rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_R_X_COMMUTATIVE_(|=, lh, rh)
  }

  // BIN_FUNC_5_4_
  inline
  t_binary operator|(x_binary lh, x_binary rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_X_X_COMMUTATIVE_(|=, get(lh.get_bits()) > get(rh.get_bits()), lh, rh)
  }

  // BIN_FUNC_5_5_
  template<typename T, t_if_int<T>>
  inline
  t_binary operator|(R_binary lh, T rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_R_T_(t_binary, |=, lh, rh)
  }

  // BIN_FUNC_5_6_
  template<typename T, t_if_int<T>>
  inline
  t_binary operator|(T lh, R_binary rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_T_R_(t_binary, |=, lh, rh)
  }

  // BIN_FUNC_5_7_
  template<typename T, t_if_int<T>>
  inline
  t_binary operator|(x_binary lh, T rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_X_T_(|=, lh, rh)
  }

  // BIN_FUNC_5_8_
  template<typename T, t_if_int<T>>
  inline
  t_binary operator|(T lh, x_binary rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_T_X_COMMUTATIVE_(|=, lh, rh)
  }

  /////////////////////////////////////////////////////////////////////////////

  // BIN_FUNC_6_1_
  inline
  t_binary operator&(R_binary lh, R_binary rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_R_R_(t_binary, &=, lh, rh)
  }

  // BIN_FUNC_6_2_
  inline
  t_binary operator&(x_binary lh, R_binary rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_X_R_(&=, lh, rh)
  }

  // BIN_FUNC_6_3_
  inline
  t_binary operator&(R_binary lh, x_binary rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_R_X_COMMUTATIVE_(&=, lh, rh)
  }

  // BIN_FUNC_6_4_
  inline
  t_binary operator&(x_binary lh, x_binary rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_X_X_COMMUTATIVE_(&=, get(lh.get_bits()) > get(rh.get_bits()), lh, rh)
  }

  // BIN_FUNC_6_5_
  template<typename T, t_if_int<T>>
  inline
  t_binary operator&(R_binary lh, T rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_R_T_(t_binary, &=, lh, rh)
  }

  // BIN_FUNC_6_6_
  template<typename T, t_if_int<T>>
  inline
  t_binary operator&(T lh, R_binary rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_T_R_(t_binary, &=, lh, rh)
  }

  // BIN_FUNC_6_7_
  template<typename T, t_if_int<T>>
  inline
  t_binary operator&(x_binary lh, T rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_X_T_(&=, lh, rh)
  }

  // BIN_FUNC_6_8_
  template<typename T, t_if_int<T>>
  inline
  t_binary operator&(T lh, x_binary rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_T_X_COMMUTATIVE_(&=, lh, rh)
  }

  /////////////////////////////////////////////////////////////////////////////

  // BIN_FUNC_7_1_
  inline
  t_binary operator^(R_binary lh, R_binary rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_R_R_(t_binary, ^=, lh, rh)
  }

  // BIN_FUNC_7_2_
  inline
  t_binary operator^(x_binary lh, R_binary rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_X_R_(^=, lh, rh)
  }

  // BIN_FUNC_7_3_
  inline
  t_binary operator^(R_binary lh, x_binary rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_R_X_COMMUTATIVE_(^=, lh, rh)
  }

  // BIN_FUNC_7_4_
  inline
  t_binary operator^(x_binary lh, x_binary rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_X_X_COMMUTATIVE_(^=, get(lh.get_bits()) > get(rh.get_bits()), lh, rh)
  }

  // BIN_FUNC_7_5_
  template<typename T, t_if_int<T>>
  inline
  t_binary operator^(R_binary lh, T rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_R_T_(t_binary, ^=, lh, rh)
  }

  // BIN_FUNC_7_6_
  template<typename T, t_if_int<T>>
  inline
  t_binary operator^(T lh, R_binary rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_T_R_(t_binary, ^=, lh, rh)
  }

  // BIN_FUNC_7_7_
  template<typename T, t_if_int<T>>
  inline
  t_binary operator^(x_binary lh, T rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_X_T_(^=, lh, rh)
  }

  // BIN_FUNC_7_8_
  template<typename T, t_if_int<T>>
  inline
  t_binary operator^(T lh, x_binary rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_T_X_COMMUTATIVE_(^=, lh, rh)
  }

  /////////////////////////////////////////////////////////////////////////////

  // BIN_FUNC_8_
  inline
  t_binary operator>>(t_binary lh, t_bits n) noexcept {
    DAINTY_BASE_NUMERIC_OP_X_T_(>>=, lh, n)
  }

  /////////////////////////////////////////////////////////////////////////////

  // BIN_FUNC_9_
  inline
  t_binary operator<<(t_binary lh, t_bits n) noexcept {
    DAINTY_BASE_NUMERIC_OP_X_T_(<<=, lh, n)
  }

  /////////////////////////////////////////////////////////////////////////////

  // BIN_FUNC_10_1_
  inline
  t_bool operator==(R_binary lh, R_binary rh) noexcept {
    return lh.is_equal(rh);
  }

  // BIN_FUNC_10_2_
  template<typename T, t_if_int<T>>
  inline
  t_bool operator==(R_binary lh, T rh) noexcept {
    return lh.is_equal(rh);
  }

  // BIN_FUNC_10_3_
  template<typename T, t_if_int<T>>
  inline
  t_bool operator==(T lh, R_binary rh) noexcept {
    return rh.is_equal(lh);
  }

  /////////////////////////////////////////////////////////////////////////////

  // BIN_FUNC_11_1_
  inline
  t_bool operator!=(R_binary lh, R_binary rh) noexcept {
    return !lh.is_equal(rh);
  }

  // BIN_FUNC_11_2_
  template<typename T, t_if_int<T>>
  inline
  t_bool operator!=(R_binary lh, T rh) noexcept {
    return !lh.is_equal(rh);
  }

  // BIN_FUNC_11_3_
  template<typename T, t_if_int<T>>
  inline
  t_bool operator!=(T lh, R_binary rh) noexcept {
    return !rh.is_equal(lh);
  }

  /////////////////////////////////////////////////////////////////////////////

  // BIN_FUNC_12_1_
  inline
  t_bool operator<(R_binary lh, R_binary rh) noexcept {
    return lh.is_less(rh);
  }

  // BIN_FUNC_12_2_
  template<typename T, t_if_int<T>>
  inline
  t_bool operator<(R_binary lh, T rh) noexcept {
    return lh.is_less(rh);
  }

  // BIN_FUNC_12_3_
  template<typename T, t_if_int<T>>
  inline
  t_bool operator<(T lh, R_binary rh) noexcept {
    return !rh.is_less_equal(lh);
  }

  /////////////////////////////////////////////////////////////////////////////

  // BIN_FUNC_13_1_
  inline
  t_bool operator<=(R_binary lh, R_binary rh) noexcept {
    return lh.is_less_equal(rh);
  }

  // BIN_FUNC_13_2_
  template<typename T, t_if_int<T>>
  inline
  t_bool operator<=(R_binary lh, T rh) noexcept {
    return lh.is_less_equal(rh);
  }

  // BIN_FUNC_13_3_
  template<typename T, t_if_int<T>>
  inline
  t_bool operator<=(T lh, R_binary rh) noexcept {
    return !rh.is_less(lh);
  }

  /////////////////////////////////////////////////////////////////////////////

  // BIN_FUNC_14_1_
  inline
  t_bool operator>(R_binary lh, R_binary rh) noexcept {
    return !lh.is_less_equal(rh);
  }

  // BIN_FUNC_14_2_
  template<typename T, t_if_int<T>>
  inline
  t_bool operator>(R_binary lh, T rh) noexcept {
    return !lh.is_less_equal(rh);
  }

  // BIN_FUNC_14_3_
  template<typename T, t_if_int<T>>
  inline
  t_bool operator>(T lh, R_binary rh) noexcept {
    return rh.is_less(lh);
  }

  /////////////////////////////////////////////////////////////////////////////

  // BIN_FUNC_15_1_
  inline
  t_bool operator>=(R_binary lh, R_binary rh) noexcept {
    return rh.is_less_equal(lh);
  }

  // BIN_FUNC_15_2_
  template<typename T, t_if_int<T>>
  inline
  t_bool operator>=(R_binary lh, T rh) noexcept {
    return !lh.is_less(rh);
  }

  // BIN_FUNC_15_3_
  template<typename T, t_if_int<T>>
  inline
  t_bool operator>=(T lh, R_binary rh) noexcept {
    return rh.is_less_equal(lh);
  }

  /////////////////////////////////////////////////////////////////////////////

  // INT_FUNC_1_1_
  inline
  t_integer operator+(R_integer lh, R_integer rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_R_R_(t_integer, +=, lh, rh)
  }

  // INT_FUNC_1_2_
  inline
  t_integer operator+(x_integer lh, R_integer rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_X_R_(+=, lh, rh)
  }

  // INT_FUNC_1_3_
  inline
  t_integer operator+(R_integer lh, x_integer rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_R_X_COMMUTATIVE_(+=, lh, rh)
  }

  // INT_FUNC_1_4_
  inline
  t_integer operator+(x_integer lh, x_integer rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_X_X_COMMUTATIVE_(+=, get(lh.get_digits()) > get(rh.get_digits()), lh, rh)
  }

  // INT_FUNC_1_5_
  template<typename T, t_if_int<T>>
  inline
  t_integer operator+(R_integer lh, T rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_R_T_(t_integer, +=, lh, rh)
  }

  // INT_FUNC_1_6_
  template<typename T, t_if_int<T>>
  inline
  t_integer operator+(T lh, R_integer rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_T_R_(t_integer, +=, lh, rh)
  }

  // INT_FUNC_1_7_
  template<typename T, t_if_int<T>>
  inline
  t_integer operator+(x_integer lh, T rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_X_T_(+=, lh, rh)
  }

  // INT_FUNC_1_8_
  template<typename T, t_if_int<T>>
  inline
  t_integer operator+(T lh, x_integer rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_T_X_COMMUTATIVE_(+=, lh, rh)
  }

  /////////////////////////////////////////////////////////////////////////////

  // INT_FUNC_2_1_
  inline
  t_integer operator-(R_integer lh, R_integer rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_R_R_(t_integer, -=, lh, rh)
  }

  // INT_FUNC_2_2_
  inline
  t_integer operator-(x_integer lh, R_integer rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_X_R_(-=, lh, rh)
  }

  // INT_FUNC_2_3_
  inline
  t_integer operator-(R_integer lh, x_integer rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_R_X_(t_integer, -=, lh, rh)
  }

  // INT_FUNC_2_4_
  inline
  t_integer operator-(x_integer lh, x_integer rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_X_X_(-=, lh, rh)
  }

  // INT_FUNC_2_5_
  template<typename T, t_if_int<T>>
  inline
  t_integer operator-(R_integer lh, T rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_R_T_(t_integer, -=, lh, rh)
  }

  // INT_FUNC_2_6_
  template<typename T, t_if_int<T>>
  inline
  t_integer operator-(T lh, R_integer rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_T_R_(t_integer, -=, lh, rh)
  }

  // INT_FUNC_2_7_
  template<typename T, t_if_int<T>>
  inline
  t_integer operator-(x_integer lh, T rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_X_T_(-=, lh, rh)
  }

  // INT_FUNC_2_8_
  template<typename T, t_if_int<T>>
  inline
  t_integer operator-(T lh, x_integer rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_T_X_(t_integer, -=, lh, rh)
  }

  /////////////////////////////////////////////////////////////////////////////

  // INT_FUNC_3_1_
  inline
  t_integer operator*(R_integer lh, R_integer rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_R_R_(t_integer, *=, lh, rh)
  }

  // INT_FUNC_3_2_
  inline
  t_integer operator*(x_integer lh, R_integer rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_X_R_(*=, lh, rh)
  }

  // INT_FUNC_3_3_
  inline
  t_integer operator*(R_integer lh, x_integer rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_R_X_COMMUTATIVE_(*=, lh, rh)
  }

  // INT_FUNC_3_4_
  inline
  t_integer operator*(x_integer lh, x_integer rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_X_X_COMMUTATIVE_(*=, get(lh.get_digits()) > get(rh.get_digits()), lh, rh)
  }

  // INT_FUNC_3_5_
  template<typename T, t_if_int<T>>
  inline
  t_integer operator*(R_integer lh, T rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_R_T_(t_integer, *=, lh, rh)
  }

  // INT_FUNC_3_6_
  template<typename T, t_if_int<T>>
  inline
  t_integer operator*(T lh, R_integer rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_T_R_(t_integer, *=, lh, rh)
  }

  // INT_FUNC_3_7_
  template<typename T, t_if_int<T>>
  inline
  t_integer operator*(x_integer lh, T rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_X_T_(*=, lh, rh)
  }

  // INT_FUNC_3_8_
  template<typename T, t_if_int<T>>
  inline
  t_integer operator*(T lh, x_integer rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_T_X_COMMUTATIVE_(*=, lh, rh)
  }

  /////////////////////////////////////////////////////////////////////////////

  // INT_FUNC_4_1_
  inline
  t_integer operator/(R_integer lh, R_integer rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_R_R_(t_integer, /=, lh, rh)
  }

  // INT_FUNC_4_2_
  inline
  t_integer operator/(x_integer lh, R_integer rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_X_R_(/=, lh, rh)
  }

  // INT_FUNC_4_3_
  inline
  t_integer operator/(R_integer lh, x_integer rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_R_X_(t_integer, /=, lh, rh)
  }

  // INT_FUNC_4_4_
  inline
  t_integer operator/(x_integer lh, x_integer rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_X_X_(/=, lh, rh)
  }

  // INT_FUNC_4_5_
  template<typename T, t_if_int<T>>
  inline
  t_integer operator/(R_integer lh, T rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_R_T_(t_integer, /=, lh, rh)
  }

  // INT_FUNC_4_6_
  template<typename T, t_if_int<T>>
  inline
  t_integer operator/(T lh, R_integer rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_T_R_(t_integer, /=, lh, rh)
  }

  // INT_FUNC_4_7_
  template<typename T, t_if_int<T>>
  inline
  t_integer operator/(x_integer lh, T rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_X_T_(/=, lh, rh)
  }

  // INT_FUNC_4_8_
  template<typename T, t_if_int<T>>
  inline
  t_integer operator/(T lh, x_integer rh) noexcept {
    DAINTY_BASE_NUMERIC_OP_T_X_(t_integer, /=, lh, rh)
  }

  /////////////////////////////////////////////////////////////////////////////

  // INT_FUNC_5_1_
  inline
  t_bool operator==(R_integer lh, R_integer rh) noexcept {
    return lh.is_equal(rh);
  }

  // INT_FUNC_5_2_
  template<typename T, t_if_int<T>>
  inline
  t_bool operator==(R_integer lh, T rh) noexcept {
    return lh.is_equal(rh);
  }

  // INT_FUNC_5_3_
  template<typename T, t_if_int<T>>
  inline
  t_bool operator==(T lh, R_integer rh) noexcept {
    return rh.is_equal(lh);
  }

  /////////////////////////////////////////////////////////////////////////////

  // INT_FUNC_6_1_
  inline
  t_bool operator!=(R_integer lh, R_integer rh) noexcept {
    return !lh.is_equal(rh);
  }

  // INT_FUNC_6_2_
  template<typename T, t_if_int<T>>
  inline
  t_bool operator!=(R_integer lh, T rh) noexcept {
    return !lh.is_equal(rh);
  }

  // INT_FUNC_6_3_
  template<typename T, t_if_int<T>>
  inline
  t_bool operator!=(T lh, R_integer rh) noexcept {
    return !rh.is_equal(lh);
  }

  /////////////////////////////////////////////////////////////////////////////

  // INT_FUNC_7_1_
  inline
  t_bool operator<(R_integer lh, R_integer rh) noexcept {
    return lh.is_less(rh);
  }

  // INT_FUNC_7_2_
  template<typename T, t_if_int<T>>
  inline
  t_bool operator<(R_integer lh, T rh) noexcept {
    return lh.is_less(rh);
  }

  // INT_FUNC_7_3_
  template<typename T, t_if_int<T>>
  inline
  t_bool operator<(T lh, R_integer rh) noexcept {
    return !rh.is_less_equal(lh);
  }

  /////////////////////////////////////////////////////////////////////////////

  // INT_FUNC_8_1_
  inline
  t_bool operator<=(R_integer lh, R_integer rh) noexcept {
    return lh.is_less_equal(rh);
  }

  // INT_FUNC_8_2_
  template<typename T, t_if_int<T>>
  inline
  t_bool operator<=(R_integer lh, T rh) noexcept {
    return lh.is_less_equal(rh);
  }

  // INT_FUNC_8_3_
  template<typename T, t_if_int<T>>
  inline
  t_bool operator<=(T lh, R_integer rh) noexcept {
    return !rh.is_less(lh);
  }

  /////////////////////////////////////////////////////////////////////////////

  // INT_FUNC_9_1_
  inline
  t_bool operator>(R_integer lh, R_integer rh) noexcept {
    return !lh.is_less_equal(rh);
  }

  // INT_FUNC_9_2_
  template<typename T, t_if_int<T>>
  inline
  t_bool operator>(R_integer lh, T rh) noexcept {
    return !lh.is_less_equal(rh);
  }

  // INT_FUNC_9_3_
  template<typename T, t_if_int<T>>
  inline
  t_bool operator>(T lh, R_integer rh) noexcept {
    return rh.is_less(lh);
  }

  /////////////////////////////////////////////////////////////////////////////

  // INT_FUNC_10_1_
  inline
  t_bool operator>=(R_integer lh, R_integer rh) noexcept {
    return rh.is_less_equal(lh);
  }

  // INT_FUNC_10_2_
  template<typename T, t_if_int<T>>
  inline
  t_bool operator>=(R_integer lh, T rh) noexcept {
    return !lh.is_less(rh);
  }

  // INT_FUNC_10_3_
  template<typename T, t_if_int<T>>
  inline
  t_bool operator>=(T lh, R_integer rh) noexcept {
    return rh.is_less_equal(lh);
  }

  /////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
