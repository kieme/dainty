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

#ifndef _DAINTY_BASE_NUMERIC_IMPL_H_
#define _DAINTY_BASE_NUMERIC_IMPL_H_

#define DAINTY_BASE_INTERNAL_INCL_STDLIB

#include "dainty_base_internal_incl_.h"
#include "dainty_base_types.h"
#include "dainty_base_logical.h"
#include "dainty_base_traits.h"
#include "dainty_base_util.h"

namespace dainty
{
namespace base
{
namespace numeric
{
namespace impl_
{
  /////////////////////////////////////////////////////////////////////////////

  using types::t_schar;
  using types::t_uchar;
  using types::t_llong;
  using types::t_ullong;
  using types::t_double;
  using types::t_void;
  using types::t_bool;
  using types::x_prefix_of;
  using types::R_prefix_of;
  using types::r_prefix_of;
  using types::p_prefix_of;
  using types::P_prefix_of;
  using types::t_opt1;
  using types::OPT1;

  using traits::t_if;
  using traits::t_and;
  using traits::t_or;
  using traits::t_is_truth;
  using traits::t_is_signed_integral;
  using traits::t_is_unsigned_integral;

  using logical::max_of;
  using logical::min_of;
  using logical::t_n;
  using logical::t_n_;
  using logical::t_ix;
  using logical::t_ix_;
  using logical::t_logical;

  /////////////////////////////////////////////////////////////////////////////

  enum  t_digits_tag_ { };
  using t_digits = t_logical<t_n_, t_digits_tag_, t_n>;

  enum  t_bits_tag_ { };
  using t_bits   = t_logical<t_n_, t_bits_tag_, t_n>;

  enum  t_bit_tag_ { };
  using t_bit    = t_logical<t_ix_, t_bit_tag_, t_ix>;

  /////////////////////////////////////////////////////////////////////////////

  constexpr t_bit  zero_bit {0};
  constexpr t_bits zero_bits{0};
  constexpr t_bits one_bits {1};

  /////////////////////////////////////////////////////////////////////////////


  using t_nvalue_ = t_schar;
  using t_pvalue_ = t_uchar;

  /*
  using t_nvalue_ = t_short;
  using t_pvalue_ = t_ushort;

  using t_nvalue_ = t_llong;
  using t_pvalue_ = t_ullong;
  */

  using p_pvalue_ = p_prefix_of<t_pvalue_>;
  using P_pvalue_ = P_prefix_of<t_pvalue_>;
  using r_pvalue_ = r_prefix_of<t_pvalue_>;
  using R_pvalue_ = R_prefix_of<t_pvalue_>;

  /////////////////////////////////////////////////////////////////////////////

  constexpr t_pvalue_ BITS_ZERO_    = (t_pvalue_)0;
  constexpr t_pvalue_ BITS_ONE_     = (t_pvalue_)1;
  constexpr t_pvalue_ BITS_ALL_     = (t_pvalue_)~BITS_ZERO_;
  constexpr t_pvalue_ BITS_BYTES_   = (t_pvalue_)sizeof(t_pvalue_);
  constexpr t_pvalue_ BITS_UNIT_    = (t_pvalue_)((BITS_BYTES_*8) - 1);
  constexpr t_pvalue_ BITS_MSB_     = (BITS_ONE_ << BITS_UNIT_);
  constexpr t_pvalue_ BITS_LAST_    = BITS_ONE_ << (BITS_UNIT_ - BITS_ONE_);
  constexpr t_pvalue_ BITS_MASK_    = (t_pvalue_)~BITS_MSB_;
  constexpr t_pvalue_ BITS_BUT_ONE_ = (t_pvalue_)~BITS_ONE_;

  /*
  static_assert(BITS_ZERO_  ==                  0ULL);
  static_assert(BITS_ONE_   ==                  1ULL);
  static_assert(BITS_ALL_   == 0xffffffffffffffffULL);
  static_assert(BITS_BYTES_ ==                  8ULL);
  static_assert(BITS_UNIT_  ==                 63ULL);
  static_assert(BITS_MSB_   == 0x8000000000000000ULL);
  static_assert(BITS_LAST_  == 0x4000000000000000ULL);
  static_assert(BITS_MASK_  == 0x7fffffffffffffffULL);

  static_assert(BITS_ZERO_  ==      0);
  static_assert(BITS_ONE_   ==      1);
  static_assert(BITS_ALL_   == 0xffff);
  static_assert(BITS_BYTES_ ==      2);
  static_assert(BITS_UNIT_  ==     15);
  static_assert(BITS_MSB_   == 0x8000);
  static_assert(BITS_LAST_  == 0x4000);
  static_assert(BITS_MASK_  == 0x7fff);
  */

  static_assert(BITS_ZERO_  ==    0);
  static_assert(BITS_ONE_   ==    1);
  static_assert(BITS_ALL_   == 0xff);
  static_assert(BITS_BYTES_ ==    1);
  static_assert(BITS_UNIT_  ==    7);
  static_assert(BITS_MSB_   == 0x80);
  static_assert(BITS_LAST_  == 0x40);
  static_assert(BITS_MASK_  == 0x7f);

  /////////////////////////////////////////////////////////////////////////////

  template<typename T> // TODO is_not_greate_int_rank
  using t_is_sz_ = t_is_truth<sizeof(T) <= sizeof(impl_::t_pvalue_)>;

  template<typename T> using t_if_neg_ =
    t_if<t_and<t_is_signed_integral<T>, t_is_sz_<T>>>;

  template<typename T> using t_if_pos_ =
    t_if<t_and<t_is_unsigned_integral<T>, t_is_sz_<T>>>;

  template<typename T> using t_if_int_ =
    t_if<t_and<t_or<t_is_signed_integral<T>, t_is_unsigned_integral<T>>,
               t_is_sz_<T>>>;

  /////////////////////////////////////////////////////////////////////////////

  struct t_buf_;
  using  r_buf_ = r_prefix_of<t_buf_>;
  using  x_buf_ = x_prefix_of<t_buf_>;
  using  p_buf_ = p_prefix_of<t_buf_>;
  using  R_buf_ = R_prefix_of<t_buf_>;

  // t_buf_ is actually t_buf
  struct t_buf_ {
    t_n       size    = t_n{2}; // support more than 64 bits - 127 bit
    t_pvalue_ sso_[2] = { 0, 0 };
    p_pvalue_ ptr     = sso_;

    constexpr
    t_buf_() noexcept {
    }

    t_buf_(t_n)         noexcept;
    t_buf_(     R_buf_) noexcept;
    t_buf_(t_n, R_buf_) noexcept;
    t_buf_(     x_buf_) noexcept;
    t_buf_(t_n, x_buf_) noexcept;
   ~t_buf_()            noexcept;

    ///////////////////////////////////////////////////////////////////////////

    r_buf_ operator=(R_buf_ value) noexcept;
    r_buf_ operator=(x_buf_ value) noexcept;

    ///////////////////////////////////////////////////////////////////////////

    t_bool clear(           t_ix = t_ix{0}) noexcept;
    t_bool fill (t_pvalue_, t_ix = t_ix{0}) noexcept;

    ///////////////////////////////////////////////////////////////////////////

    t_bool ensure(t_n)         noexcept;
    t_bool ensure(     R_buf_) noexcept;
    t_bool ensure(t_n, R_buf_) noexcept;
    t_bool ensure(     x_buf_) noexcept;
    t_bool ensure(t_n, x_buf_) noexcept;

    ///////////////////////////////////////////////////////////////////////////

    t_void reset()            noexcept;
    t_bool reset(t_n)         noexcept;
    t_bool reset(     R_buf_) noexcept;
    t_bool reset(t_n, R_buf_) noexcept;
    t_bool reset(     x_buf_) noexcept;
    t_bool reset(t_n, x_buf_) noexcept;

    ///////////////////////////////////////////////////////////////////////////

    t_bool is_valid() const noexcept;
    t_bool is_heap () const noexcept;
    t_void steal   (r_buf_) noexcept;

    ///////////////////////////////////////////////////////////////////////////

    p_pvalue_ begin()       noexcept;
    P_pvalue_ begin() const noexcept;

    p_pvalue_ end  ()       noexcept;
    P_pvalue_ end  () const noexcept;

    ///////////////////////////////////////////////////////////////////////////

    constexpr r_pvalue_  ref(t_ix)       noexcept;
    constexpr R_pvalue_ cref(t_ix) const noexcept;

    ///////////////////////////////////////////////////////////////////////////

    t_bool set(t_ix, t_pvalue_) noexcept;

    ///////////////////////////////////////////////////////////////////////////
  };

  using  t_store_ = t_buf_;
  using  r_store_ = r_buf_;
  using  p_store_ = p_buf_;
  using  x_store_ = x_buf_;
  using  R_store_ = R_buf_;

  /////////////////////////////////////////////////////////////////////////////


  template<typename T>
  T first_else_second_(T first, T second) noexcept {
    return first ? first : second;
  }

  /////////////////////////////////////////////////////////////////////////////

  struct t_pos;
  using  R_pos = R_prefix_of<t_pos>;
  using  r_pos = r_prefix_of<t_pos>;

  struct t_pos {
    t_ix  ix  = t_ix{0};
    t_bit bit = zero_bit;

    constexpr // IMPL_METHOD_2_1_1_
    t_pos() noexcept = default;

    constexpr // IMPL_METHOD_2_1_2_
    t_pos(t_ix, t_bit) noexcept;

    constexpr // IMPL_METHOD_2_2_
    operator t_bool() const noexcept;

    constexpr // IMPL_METHOD_2_3_
    t_bits bits() const noexcept;

    constexpr // IMPL_METHOD_2_4_
    t_bit bit_ix() const noexcept;
  };

  /////////////////////////////////////////////////////////////////////////////

  struct t_info;
  using  R_info = R_prefix_of<t_info>;

  struct t_info {
    t_pos  msb_on   = {};
    t_bits on_bits  = zero_bits;
    t_bits max_bits = zero_bits;

    constexpr // IMPL_METHOD_3_1_1_
    t_info() noexcept = default;

    constexpr // IMPL_METHOD_3_1_2_
    t_info(t_bits max_bits_) noexcept;

    constexpr // IMPL_METHOD_3_1_3_
    t_info(R_pos msb_on_, t_bits on_bits_, t_bits max_bits_) noexcept;

    constexpr // IMPL_METHOD_3_2_
    operator t_bool() const noexcept;

    constexpr // IMPL_METHOD_3_3_
    t_bits free_bits() const noexcept;

    constexpr // IMPL_METHOD_3_4_
    t_bool is_neg() const noexcept;
  };

  /////////////////////////////////////////////////////////////////////////////

  class t_impl_base_ {
  public:
    constexpr
    t_impl_base_()                    noexcept; // IMPL_METHOD_1_1_1_
    constexpr
    t_impl_base_(r_store_)            noexcept; // IMPL_METHOD_1_1_2_
    constexpr
    t_impl_base_(r_store_, t_nvalue_) noexcept; // IMPL_METHOD_1_1_3_
    constexpr
    t_impl_base_(r_store_, t_pvalue_) noexcept; // IMPL_METHOD_1_1_4_
    t_impl_base_(r_store_, R_store_)  noexcept; // IMPL_METHOD_1_1_5_

    ///////////////////////////////////////////////////////////////////////////

    t_void assign(r_store_, t_nvalue_) noexcept; // IMPL_METHOD_1_2_1_
    t_void assign(r_store_, t_pvalue_) noexcept; // IMPL_METHOD_1_2_2_
    t_void assign(r_store_, R_store_)  noexcept; // IMPL_METHOD_1_2_3_

    ///////////////////////////////////////////////////////////////////////////

    t_void addition(r_store_, t_nvalue_) noexcept; // IMPL_METHOD_1_3_1_
    t_void addition(r_store_, t_pvalue_) noexcept; // IMPL_METHOD_1_3_2_
    t_void addition(r_store_, R_store_)  noexcept; // IMPL_METHOD_1_3_3_

    ///////////////////////////////////////////////////////////////////////////

    t_void subtraction(r_store_, t_nvalue_) noexcept; // IMPL_METHOD_1_4_1_
    t_void subtraction(r_store_, t_pvalue_) noexcept; // IMPL_METHOD_1_4_2_
    t_void subtraction(r_store_, R_store_)  noexcept; // IMPL_METHOD_1_4_3_

    ///////////////////////////////////////////////////////////////////////////

    t_void multiplication(r_store_, t_nvalue_) noexcept; // IMPL_METHOD_1_5_1_
    t_void multiplication(r_store_, t_pvalue_) noexcept; // IMPL_METHOD_1_5_2_
    t_void multiplication(r_store_, R_store_)  noexcept; // IMPL_METHOD_1_5_3_

    ///////////////////////////////////////////////////////////////////////////

    t_void division(r_store_, t_nvalue_) noexcept; // IMPL_METHOD_1_6_1_
    t_void division(r_store_, t_pvalue_) noexcept; // IMPL_METHOD_1_6_2_
    t_void division(r_store_, R_store_)  noexcept; // IMPL_METHOD_1_6_3_

    ///////////////////////////////////////////////////////////////////////////

    t_void binary_and(r_store_, t_nvalue_) noexcept; // IMPL_METHOD_1_7_1_
    t_void binary_and(r_store_, t_pvalue_) noexcept; // IMPL_METHOD_1_7_2_
    t_void binary_and(r_store_, R_store_)  noexcept; // IMPL_METHOD_1_7_3_

    ///////////////////////////////////////////////////////////////////////////

    t_void binary_or(r_store_, t_nvalue_) noexcept; // IMPL_METHOD_1_8_1_
    t_void binary_or(r_store_, t_pvalue_) noexcept; // IMPL_METHOD_1_8_2_
    t_void binary_or(r_store_, R_store_)  noexcept; // IMPL_METHOD_1_8_3_

    ///////////////////////////////////////////////////////////////////////////

    t_void binary_xor(r_store_, t_nvalue_) noexcept; // IMPL_METHOD_1_9_1_
    t_void binary_xor(r_store_, t_pvalue_) noexcept; // IMPL_METHOD_1_9_2_
    t_void binary_xor(r_store_, R_store_)  noexcept; // IMPL_METHOD_1_9_3_

    ///////////////////////////////////////////////////////////////////////////

    t_void ones_complement(r_store_)                noexcept; // IMPL_METHOD_1_10_
    t_void twos_complement(r_store_)                noexcept; // IMPL_METHOD_1_11_
    t_void shift_left     (r_store_, t_bits)        noexcept; // IMPL_METHOD_1_12_
    t_void shift_right    (r_store_, t_bits)        noexcept; // IMPL_METHOD_1_13_
    t_bool set_bit        (r_store_, t_bit, t_bool) noexcept; // IMPL_METHOD_1_14_
    t_bool is_zero        (R_store_)          const noexcept; // IMPL_METHOD_1_15_
    t_bits get_bits       (R_store_)          const noexcept; // IMPL_METHOD_1_16_
    t_bool get_bit        (R_store_, t_bit)   const noexcept; // IMPL_METHOD_1_17_
    t_bool is_negative    (R_store_)          const noexcept; // IMPL_METHOD_1_18_

    ///////////////////////////////////////////////////////////////////////////

    t_bool ensure(r_store_, t_bits)            noexcept; // IMPL_METHOD_1_19_1_
    t_bool ensure(r_store_,         R_store_)  noexcept; // IMPL_METHOD_1_19_2_
    t_bool ensure(r_store_, t_bits, R_store_)  noexcept; // IMPL_METHOD_1_19_3_

    ///////////////////////////////////////////////////////////////////////////

    t_void reset(r_store_)                    noexcept; // IMPL_METHOD_1_20_1_
    t_void reset(r_store_,         t_nvalue_) noexcept; // IMPL_METHOD_1_20_2_
    t_bool reset(r_store_, t_bits, t_nvalue_) noexcept; // IMPL_METHOD_1_20_3_
    t_void reset(r_store_,         t_pvalue_) noexcept; // IMPL_METHOD_1_20_4_
    t_bool reset(r_store_, t_bits, t_pvalue_) noexcept; // IMPL_METHOD_1_20_5_
    t_bool reset(r_store_,         R_store_)  noexcept; // IMPL_METHOD_1_20_6_
    t_bool reset(r_store_, t_bits, R_store_)  noexcept; // IMPL_METHOD_1_20_7_

    ///////////////////////////////////////////////////////////////////////////

    t_bool is_equal(R_store_, R_store_)  const noexcept; // IMPL_METHOD_1_21_1_
    t_bool is_equal(R_store_, t_pvalue_) const noexcept; // IMPL_METHOD_1_21_2_
    t_bool is_equal(R_store_, t_nvalue_) const noexcept; // IMPL_METHOD_1_21_3_

    ///////////////////////////////////////////////////////////////////////////

    t_bool is_less(R_store_, R_store_)  const noexcept; // IMPL_METHOD_1_22_1_
    t_bool is_less(R_store_, t_pvalue_) const noexcept; // IMPL_METHOD_1_22_2_
    t_bool is_less(R_store_, t_nvalue_) const noexcept; // IMPL_METHOD_1_22_3_

    ///////////////////////////////////////////////////////////////////////////

    t_bool is_less_equal(R_store_, R_store_)  const noexcept; // IMPL_METHOD_1_23_1_
    t_bool is_less_equal(R_store_, t_pvalue_) const noexcept; // IMPL_METHOD_1_23_2_
    t_bool is_less_equal(R_store_, t_nvalue_) const noexcept; // IMPL_METHOD_1_23_3_

    ///////////////////////////////////////////////////////////////////////////

    t_bool ms_bit      (R_store_) const noexcept; // BIN_METHOD_1_24_
    t_bool ls_bit      (R_store_) const noexcept; // BIN_METHOD_1_25_
    t_void display     (R_store_) const noexcept; // BIN_METHOD_1_26_
    t_bit  first_on_bit(R_store_) const noexcept; // BIN_METHOD_1_27_
    t_bit  last_on_bit (R_store_) const noexcept; // BIN_METHOD_1_28_
    t_bits on_bits     (R_store_) const noexcept; // BIN_METHOD_1_29_
    t_info get_info    (R_store_) const noexcept; // BIN_METHOD_1_30_

    ///////////////////////////////////////////////////////////////////////////
  };

  /////////////////////////////////////////////////////////////////////////////

  using t_binary_ = t_impl_base_; // Don't support variable grow

  /////////////////////////////////////////////////////////////////////////////

  t_void display_bits_  (R_store_, t_bool)           noexcept; // IMPL_FUNC_1_1_
  t_info bits_info_     (R_store_)                   noexcept; // IMPL_FUNC_1_2_
  t_bits free_bits_     (R_store_)                   noexcept; // IMPL_FUNC_1_3_
  t_bits on_bits_       (R_store_)                   noexcept; // IMPL_FUNC_1_4_
  t_pos  lsb_on_        (R_store_)                   noexcept; // IMPL_FUNC_1_5_
  t_pos  msb_on_        (R_store_)                   noexcept; // IMPL_FUNC_1_6_
  t_pos  msb_off_       (R_store_)                   noexcept; // IMPL_FUNC_1_7_
  t_void ones_compl_    (r_store_)                   noexcept; // IMPL_FUNC_1_8_
  t_void twos_compl_    (r_store_)                   noexcept; // IMPL_FUNC_1_9_
  t_bool assign_        (r_store_, R_store_)         noexcept; // IMPL_FUNC_1_10_
  t_bool add_           (r_store_, R_store_)         noexcept; // IMPL_FUNC_1_11_
  t_bool minus_         (r_store_, R_store_)         noexcept; // IMPL_FUNC_1_12_
  t_bool multiply_      (r_store_, R_store_)         noexcept; // IMPL_FUNC_1_13_
  t_bool divide_        (r_store_, R_store_)         noexcept; // IMPL_FUNC_1_14_
  t_bool and_           (r_store_, R_store_)         noexcept; // IMPL_FUNC_1_15_
  t_bool or_            (r_store_, R_store_)         noexcept; // IMPL_FUNC_1_16_
  t_bool xor_           (r_store_, R_store_)         noexcept; // IMPL_FUNC_1_17_
  t_bool shift_left_    (r_store_, t_bits)           noexcept; // IMPL_FUNC_1_18_
  t_bool shift_right_   (r_store_, t_bits)           noexcept; // IMPL_FUNC_1_19_
  t_bits shift_next_    (r_store_)                   noexcept; // IMPL_FUNC_1_20_
  t_bool set_bit_       (r_store_, t_bit, t_bool)    noexcept; // IMPL_FUNC_1_21_
  t_bool get_bit_       (R_store_, t_bit)            noexcept; // IMPL_FUNC_1_22_
  t_bool is_zero_       (R_store_)                   noexcept; // IMPL_FUNC_1_23_
  t_bool reset_         (r_store_, R_store_)         noexcept; // IMPL_FUNC_1_24_
  t_bool reset_         (r_store_, t_bits, R_store_) noexcept; // IMPL_FUNC_1_25_
  t_bool is_equal_      (R_store_, R_store_)         noexcept; // IMPL_FUNC_1_26_
  t_bool is_less_       (R_store_, R_store_)         noexcept; // IMPL_FUNC_1_27_
  t_bool is_less_equal_ (R_store_, R_store_)         noexcept; // IMPL_FUNC_1_28_
  t_bool ensure_        (r_store_, t_bits)           noexcept; // IMPL_FUNC_1_29_
  t_bool ensure_        (r_store_, R_store_)         noexcept; // IMPL_FUNC_1_30_
  t_bool ensure_        (r_store_, t_bits, R_store_) noexcept; // IMPL_FUNC_1_31_

  /////////////////////////////////////////////////////////////////////////////

  // IMPL_FUNC_2_1_
  constexpr
  t_n calc_size_(t_bits bits) noexcept {
    return t_n((get(bits)/BITS_UNIT_) + ((get(bits) % BITS_UNIT_) != 0));
  }

  // IMPL_FUNC_2_2_
  constexpr
  t_bits calc_bits_(t_digits digits) noexcept {
    t_double bits = get(digits) / 0.301029995;
    return t_bits{static_cast<t_n_>(bits) + 1};
  }

  // IMPL_FUNC_2_3_
  constexpr
  t_digits calc_digits_(t_bits bits) noexcept {
    t_double digits = get(bits) * 0.301029995;
    return t_digits{static_cast<t_n_>(digits)};
  }

  // IMPL_FUNC_2_4_
  constexpr
  t_ix last_ix_(R_store_ store) noexcept {
    return t_ix{get(store.size) - 1};
  }

  /////////////////////////////////////////////////////////////////////////////

  // IMPL_FUNC_2_5_1_
  constexpr
  r_pvalue_ last_(r_store_ store) noexcept {
    return store.ref(last_ix_(store));
  }

  // IMPL_FUNC_2_5_2_
  constexpr
  t_pvalue_ last_(R_store_ store) noexcept {
    return store.cref(last_ix_(store));
  }

  /////////////////////////////////////////////////////////////////////////////

  // IMPL_FUNC_2_6_
  constexpr
  t_bool lsb_(R_store_ store) noexcept {
    return store.cref(t_ix{0}) & BITS_ONE_;
  }

  // IMPL_FUNC_2_7_
  constexpr
  t_bool msb_(R_store_ store) noexcept {
    return last_(store) & BITS_MSB_;
  }

  // IMPL_FUNC_2_8_
  constexpr
  t_bool is_neg_(R_store_ store) noexcept {
    return msb_(store);
  }

  // IMPL_FUNC_2_9_
  constexpr
  t_bits max_bits_(R_store_ value) noexcept {
    return t_bits{(get(value.size) * BITS_UNIT_) + 1};
  }

  // IMPL_FUNC_2_10_
  inline
  t_bool abs_(r_store_ value) noexcept {
    if (is_neg_(value)) {
      twos_compl_(value);
      return true;
    }
    return false;
  }

  /////////////////////////////////////////////////////////////////////////////

  // IMPL_FUNC_3_1_
  constexpr
  t_bool operator==(R_pos lh, R_pos rh) noexcept {
    return (lh.ix == rh.ix) && (get(lh.bit) == get(rh.bit));
  }

  // IMPL_FUNC_3_2_
  constexpr
  t_bool operator<(R_pos lh, R_pos rh) noexcept {
    return lh.ix < rh.ix || (lh.ix == rh.ix && get(lh.bit) < get(rh.bit));
  }

  // IMPL_FUNC_3_3_
  constexpr
  t_bool operator<=(R_pos lh, R_pos rh) noexcept {
    return  lh.ix < rh.ix || (lh.ix == rh.ix && get(lh.bit) <= get(rh.bit));
  }

  // IMPL_FUNC_3_4_
  constexpr
  t_bool operator!=(R_pos lh, R_pos rh) noexcept {
    return !(lh == rh);
  }

  // IMPL_FUNC_3_5_
  constexpr
  t_bool operator>(R_pos lh, R_pos rh) noexcept {
    return rh < lh;
  }

  // IMPL_FUNC_3_6_
  constexpr
  t_bool operator>=(R_pos lh, R_pos rh) noexcept {
    return rh <= lh;
  }

  template<typename T>
  struct t_divmod {
    T div;
    T mod;
  };

  template<typename T, typename T1>
  constexpr
  t_divmod<T> divmod(T value, T1 value1) {
    return t_divmod<T>{value/value1, value%value1};
  }

  // IMPL_METHOD_3_7_
  constexpr
  t_pos operator+(R_pos pos, t_bits n) noexcept {
    if (pos && 0 < get(n)) {
      t_n_ max = (get(pos.ix)*BITS_UNIT_) + get(pos.bit) + get(n) - 1;
      t_divmod<t_n_> dm = divmod(max, BITS_UNIT_);
      return {t_ix{dm.div}, t_bit{dm.mod}};
    }
    return {};
  }

  /////////////////////////////////////////////////////////////////////////////

  // IMPL_FUNC_4_1_
  inline
  t_void assign_(r_store_ store, t_pvalue_ value) noexcept {
    store.ref(t_ix{0}) = value & BITS_MASK_;
    store.ref(t_ix{1}) = value & BITS_MSB_ ? BITS_ONE_ : BITS_ZERO_;
    store.fill(BITS_ZERO_, t_ix{0});
  }

  // IMPL_FUNC_4_2_
  inline
  t_void assign_(r_store_ store, t_nvalue_ value) noexcept {
    store.ref(t_ix{0}) = value & BITS_MASK_;
    store.fill(value & BITS_MSB_ ? BITS_ALL_ : BITS_ZERO_, t_ix{1});
  }

  // IMPL_FUNC_4_3_
  constexpr
  t_void assign_(r_store_ store, t_pvalue_ value, t_opt1) noexcept {
    store.ref(t_ix{0}) = value & BITS_MASK_;
    store.ref(t_ix{1}) = value & BITS_MSB_ ? BITS_ONE_ : BITS_ZERO_;
  }

  // IMPL_FUNC_4_4_
  constexpr
  t_void assign_(r_store_ store, t_nvalue_ value, t_opt1) noexcept {
    store.ref(t_ix{0}) = value & BITS_MASK_;
    store.ref(t_ix{1}) = value & BITS_MSB_ ? BITS_ALL_ : BITS_ZERO_;
  }

  /////////////////////////////////////////////////////////////////////////////

  // IMPL_FUNC_5_1_
  template<typename T>
  inline
  t_bool add_(r_store_ store, T value) noexcept {
    t_store_ value_store;
    assign_(value_store, value);
    return add_(store, value_store);
  }

  // IMPL_FUNC_5_2_
  template<typename T>
  inline
  t_bool minus_(r_store_ store, T value) noexcept {
    t_store_ value_store;
    assign_(value_store, value);
    return minus_(store, value_store);
  }

  // IMPL_FUNC_5_3_
  template<typename T>
  inline
  t_bool multiply_(r_store_ store, T value) noexcept {
    t_store_ value_store;
    assign_(value_store, value);
    return multiply_(store, value_store);
  }

  // IMPL_FUNC_5_4_
  template<typename T>
  inline
  t_bool divide_(r_store_ store, T value) noexcept {
    t_store_ value_store;
    assign_(value_store, value);
    return divide_(store, value_store);
  }

  // IMPL_FUNC_5_5_
  template<typename T>
  inline
  t_void and_(r_store_ store, T value) noexcept {
    t_store_ value_store;
    assign_(value_store, value);
    and_(store, value_store);
  }

  // IMPL_FUNC_5_6_
  template<typename T>
  inline
  t_void or_(r_store_ store, T value) noexcept {
    t_store_ value_store;
    assign_(value_store, value);
    or_(store, value_store);
  }

  // IMPL_FUNC_5_7_
  template<typename T>
  inline
  t_void xor_(r_store_ store, T value) noexcept {
    t_store_ value_store;
    assign_(value_store, value);
    xor_(store, value_store);
  }

  /////////////////////////////////////////////////////////////////////////////

  // IMPL_FUNC_6_1_
  template<typename T>
  inline
  t_void reset_(r_store_ store, T value) noexcept {
    t_store_ value_store;
    assign_(value_store, value);
    reset_(store, value_store);
  }

  // IMPL_FUNC_6_2_
  template<typename T>
  inline
  t_bool reset_(r_store_ store, t_bits max, T value) noexcept {
    t_store_ value_store;
    assign_(value_store, value);
    return reset_(store, max, value_store);
  }

  /////////////////////////////////////////////////////////////////////////////

  // IMPL_FUNC_7_1_
  template<typename T>
  inline
  t_bool is_equal_(R_store_ store, T value) noexcept {
    t_store_ value_store;
    assign_(value_store, value);
    return is_equal_(store, value_store);
  }

  // IMPL_FUNC_7_2_
  template<typename T>
  inline
  t_bool is_less_(R_store_ store, T value) noexcept {
    t_store_ value_store;
    assign_(value_store, value);
    return is_less_(store, value_store);
  }

  // IMPL_FUNC_7_3_
  template<typename T>
  inline
  t_bool is_less_equal_(R_store_ store, T value) noexcept {
    t_store_ value_store;
    assign_(value_store, value);
    return is_less_equal_(store, value_store);
  }

  /////////////////////////////////////////////////////////////////////////////

  // IMPL_METHOD_1_1_1_
  constexpr
  t_impl_base_::t_impl_base_() noexcept {
  }

  // IMPL_METHOD_1_1_2_
  constexpr
  t_impl_base_::t_impl_base_(r_store_) noexcept {
  }

  // IMPL_METHOD_1_1_3_
  constexpr
  t_impl_base_::t_impl_base_(r_store_ store, t_nvalue_ value) noexcept {
    assign_(store, value, OPT1);
  }

  // IMPL_METHOD_1_1_4_
  constexpr
  t_impl_base_::t_impl_base_(r_store_ store, t_pvalue_ value) noexcept {
    assign_(store, value, OPT1);
  }

  // IMPL_METHOD_1_1_5_
  inline
  t_impl_base_::t_impl_base_(r_store_ store, R_store_ value) noexcept {
    assign_(store, value);
  }

  /////////////////////////////////////////////////////////////////////////////

  // IMPL_METHOD_1_2_1_
  inline
  t_void t_impl_base_::assign(r_store_ store, t_nvalue_ value) noexcept {
    assign_(store, value);
  }

  // IMPL_METHOD_1_2_2_
  inline
  t_void t_impl_base_::assign(r_store_ store, t_pvalue_ value) noexcept {
    assign_(store, value);
  }

  // IMPL_METHOD_1_2_3_
  inline
  t_void t_impl_base_::assign(r_store_ store, R_store_ value) noexcept {
    assign_(store, value);
  }

  /////////////////////////////////////////////////////////////////////////////

  // IMPL_METHOD_1_3_1_
  inline
  t_void t_impl_base_::addition(r_store_ store, t_nvalue_ value) noexcept {
    add_(store, value);
  }

  // IMPL_METHOD_1_3_2_
  inline
  t_void t_impl_base_::addition(r_store_ store, t_pvalue_ value) noexcept {
    add_(store, value);
  }

  // IMPL_METHOD_1_3_3_
  inline
  t_void t_impl_base_::addition(r_store_ store, R_store_ value) noexcept {
    add_(store, value);
  }

  /////////////////////////////////////////////////////////////////////////////

  // IMPL_METHOD_1_4_1_
  inline
  t_void t_impl_base_::subtraction(r_store_ store, t_nvalue_ value) noexcept {
    minus_(store, value);
  }

  // IMPL_METHOD_1_4_2_
  inline
  t_void t_impl_base_::subtraction(r_store_ store, t_pvalue_ value) noexcept {
    minus_(store, value);
  }

  // IMPL_METHOD_1_4_3_
  inline
  t_void t_impl_base_::subtraction(r_store_ store, R_store_ value) noexcept {
    minus_(store, value);
  }

  /////////////////////////////////////////////////////////////////////////////

  // IMPL_METHOD_1_5_1_
  inline
  t_void t_impl_base_::multiplication(r_store_ store, t_nvalue_ value) noexcept {
    multiply_(store, value);
  }

  // IMPL_METHOD_1_5_2_
  inline
  t_void t_impl_base_::multiplication(r_store_ store, t_pvalue_ value) noexcept {
    multiply_(store, value);
  }

  // IMPL_METHOD_1_5_3_
  inline
  t_void t_impl_base_::multiplication(r_store_ store, R_store_ value) noexcept {
    multiply_(store, value);
  }

  /////////////////////////////////////////////////////////////////////////////

  // IMPL_METHOD_1_6_1_
  inline
  t_void t_impl_base_::division(r_store_ store, t_nvalue_ value) noexcept {
    divide_(store, value);
  }

  // IMPL_METHOD_1_6_2_
  inline
  t_void t_impl_base_::division(r_store_ store, t_pvalue_ value) noexcept {
    divide_(store, value);
  }

  // IMPL_METHOD_1_6_3_
  inline
  t_void t_impl_base_::division(r_store_ store, R_store_ value) noexcept {
    divide_(store, value);
  }

  /////////////////////////////////////////////////////////////////////////////

  // IMPL_METHOD_1_7_1_
  inline
  t_void t_impl_base_::binary_and(r_store_ store, t_nvalue_ value) noexcept {
    and_(store, value);
  }

  // IMPL_METHOD_1_7_2_
  inline
  t_void t_impl_base_::binary_and(r_store_ store, t_pvalue_ value) noexcept {
    and_(store, value);
  }

  // IMPL_METHOD_1_7_3_
  inline
  t_void t_impl_base_::binary_and(r_store_ store, R_store_ value) noexcept {
    and_(store, value);
  }

  /////////////////////////////////////////////////////////////////////////////

  // IMPL_METHOD_1_8_1_
  inline
  t_void t_impl_base_::binary_or(r_store_ store, t_nvalue_ value) noexcept {
    or_(store, value);
  }

  // IMPL_METHOD_1_8_2_
  inline
  t_void t_impl_base_::binary_or(r_store_ store, t_pvalue_ value) noexcept {
    or_(store, value);
  }

  // IMPL_METHOD_1_8_3_
  inline
  t_void t_impl_base_::binary_or(r_store_ store, R_store_ value)  noexcept {
    or_(store, value);
  }

  /////////////////////////////////////////////////////////////////////////////

  // IMPL_METHOD_1_9_1_
  inline
  t_void t_impl_base_::binary_xor(r_store_ store, t_nvalue_ value) noexcept {
    xor_(store, value);
  }

  // IMPL_METHOD_1_9_2_
  inline
  t_void t_impl_base_::binary_xor(r_store_ store, t_pvalue_ value) noexcept {
    xor_(store, value);
  }

  // IMPL_METHOD_1_9_3_
  inline
  t_void t_impl_base_::binary_xor(r_store_ store, R_store_ value) noexcept {
    xor_(store, value);
  }

  /////////////////////////////////////////////////////////////////////////////

  // IMPL_METHOD_1_10_
  inline
  t_void t_impl_base_::ones_complement(r_store_ store) noexcept {
    ones_compl_(store);
  }

  // IMPL_METHOD_1_11_
  inline
  t_void t_impl_base_::twos_complement(r_store_ store) noexcept {
    twos_compl_(store);
  }

  // IMPL_METHOD_1_12_
  inline
  t_void t_impl_base_::shift_left(r_store_ store, t_bits n) noexcept {
    shift_left_(store, n);
  }

  // IMPL_METHOD_1_13_
  inline
  t_void t_impl_base_::shift_right(r_store_ store, t_bits n) noexcept {
    shift_right_(store, n);
  }

  // IMPL_METHOD_1_14_
  inline
  t_bool t_impl_base_::set_bit(r_store_ store, t_bit bit, t_bool on) noexcept {
    return set_bit_(store, bit, on);
  }

  // IMPL_METHOD_1_15_
  inline
  t_bool t_impl_base_::is_zero(R_store_ store) const noexcept {
    return is_zero_(store);
  }

  // IMPL_METHOD_1_16_
  inline
  t_bits t_impl_base_::get_bits(R_store_ store) const noexcept {
    return max_bits_(store);
  }

  // IMPL_METHOD_1_17_
  inline
  t_bool t_impl_base_::get_bit(R_store_ store, t_bit bit) const noexcept {
    return get_bit_(store, bit);
  }

  // IMPL_METHOD_1_18_
  inline
  t_bool t_impl_base_::is_negative(R_store_ store) const noexcept {
    return is_neg_(store);
  }

  /////////////////////////////////////////////////////////////////////////////

  // IMPL_METHOD_1_19_1_
  inline
  t_bool t_impl_base_::ensure(r_store_ store, t_bits max) noexcept {
    return ensure_(store, max);
  }

  // IMPL_METHOD_1_19_1_
  inline
  t_bool t_impl_base_::ensure(r_store_ store, R_store_ value) noexcept {
    return ensure_(store, value);
  }

  // IMPL_METHOD_1_19_2_
  inline
  t_bool t_impl_base_::ensure(r_store_ store, t_bits max, R_store_ value) noexcept {
    return ensure_(store, max, value);
  }

  /////////////////////////////////////////////////////////////////////////////

  // IMPL_METHOD_1_20_1_
  inline
  t_void t_impl_base_::reset(r_store_ store) noexcept {
    reset_(store, BITS_ZERO_);
  }

  // IMPL_METHOD_1_20_2_
  inline
  t_void t_impl_base_::reset(r_store_ store, t_nvalue_ value) noexcept {
    reset_(store, value);
  }

  // IMPL_METHOD_1_20_3_
  inline
  t_bool t_impl_base_::reset(r_store_ store, t_bits max, t_nvalue_ value) noexcept {
    return reset_(store, max, value);
  }

  // IMPL_METHOD_1_20_4_
  inline
  t_void t_impl_base_::reset(r_store_ store, t_pvalue_ value) noexcept {
    reset_(store, value);
  }

  // IMPL_METHOD_1_20_5_
  inline
  t_bool t_impl_base_::reset(r_store_ store, t_bits max, t_pvalue_ value) noexcept {
    return reset_(store, max, value);
  }

  // IMPL_METHOD_1_20_6_
  inline
  t_bool t_impl_base_::reset(r_store_ store, R_store_ value) noexcept {
    return reset_(store, value);
  }

  // IMPL_METHOD_1_20_7_
  inline
  t_bool t_impl_base_::reset(r_store_ store, t_bits max, R_store_ value) noexcept {
    return reset_(store, max, value);
  }

  /////////////////////////////////////////////////////////////////////////////

  // IMPL_METHOD_1_21_1_
  inline
  t_bool t_impl_base_::is_equal(R_store_ store, R_store_ value) const noexcept {
    return is_equal_(store, value);
  }

  // IMPL_METHOD_1_21_2_
  inline
  t_bool t_impl_base_::is_equal(R_store_ store, t_pvalue_ value) const noexcept {
    return is_equal_(store, value);
  }

  // IMPL_METHOD_1_21_3_
  inline
  t_bool t_impl_base_::is_equal(R_store_ store, t_nvalue_ value) const noexcept {
    return is_equal_(store, value);
  }

  /////////////////////////////////////////////////////////////////////////////

  // IMPL_METHOD_1_22_1_
  inline
  t_bool t_impl_base_::is_less(R_store_ store, R_store_ value)  const noexcept {
    return is_less_(store, value);
  }

  // IMPL_METHOD_1_22_2_
  inline
  t_bool t_impl_base_::is_less(R_store_ store, t_pvalue_ value) const noexcept {
    return is_less_(store, value);
  }

  // IMPL_METHOD_1_22_3_
  inline
  t_bool t_impl_base_::is_less(R_store_ store, t_nvalue_ value) const noexcept {
    return is_less_(store, value);
  }

  /////////////////////////////////////////////////////////////////////////////

  // IMPL_METHOD_1_23_1_
  inline
  t_bool t_impl_base_::is_less_equal(R_store_ store, R_store_ value) const noexcept {
    return is_less_equal_(store, value);
  }

  // IMPL_METHOD_1_23_2_
  inline
  t_bool t_impl_base_::is_less_equal(R_store_ store, t_pvalue_ value) const noexcept {
    return is_less_equal_(store, value);
  }

  // IMPL_METHOD_1_23_3_
  inline
  t_bool t_impl_base_::is_less_equal(R_store_ store, t_nvalue_ value) const noexcept {
    return is_less_equal_(store, value);
  }

  /////////////////////////////////////////////////////////////////////////////

  // BIN_METHOD_1_24_
  inline
  t_bool t_impl_base_::ms_bit(R_store_ store) const noexcept {
    return msb_(store);
  }

  // BIN_METHOD_1_25_
  inline
  t_bool t_impl_base_::ls_bit(R_store_ store) const noexcept {
    return lsb_(store);
  }

  // BIN_METHOD_1_26_
  inline
  t_void t_impl_base_::display(R_store_ store) const noexcept {
    display_bits_(store, false);
  }

  // BIN_METHOD_1_27_
  inline
  t_bit t_impl_base_::first_on_bit(R_store_ store) const noexcept {
    t_pos pos = lsb_on_(store);
    return pos ? pos.bit_ix() : mk<t_bit>(get_bits(store));
  }

  // BIN_METHOD_1_28_
  inline
  t_bit t_impl_base_::last_on_bit(R_store_ store) const noexcept {
    t_pos pos = msb_on_(store);
    return pos ? pos.bit_ix() : mk<t_bit>(get_bits(store));
  }

  // BIN_METHOD_1_29_
  inline
  t_bits t_impl_base_::on_bits(R_store_ store) const noexcept {
    return on_bits_(store);
  }

  // BIN_METHOD_1_30_
  inline
  t_info t_impl_base_::get_info(R_store_ store) const noexcept {
    return bits_info_(store);
  }

  /////////////////////////////////////////////////////////////////////////////

  // IMPL_METHOD_2_1_2_
  constexpr
  t_pos::t_pos(t_ix ix_, t_bit bit_) noexcept : ix{ix_}, bit{bit_} {
  }

  // IMPL_METHOD_2_2_
  constexpr
  t_pos::operator t_bool() const noexcept {
    return get(bit);
  }

  // IMPL_METHOD_2_3_
  constexpr
  t_bits t_pos::bits() const noexcept {
    return t_bits{(BITS_UNIT_ * get(ix)) + get(bit)};
  }

  // IMPL_METHOD_2_4_
  constexpr
  t_bit t_pos::bit_ix() const noexcept {
    return t_bit{get(bits()) - 1};
  }

  /////////////////////////////////////////////////////////////////////////////

  // IMPL_METHOD_3_1_2_
  constexpr
  t_info::t_info(t_bits max_bits_) noexcept : max_bits{max_bits_} {
  }

  // IMPL_METHOD_3_1_3_
  constexpr
  t_info::t_info(R_pos msb_on_, t_bits on_bits_, t_bits max_bits_) noexcept
    : msb_on{msb_on_}, on_bits{on_bits_}, max_bits{max_bits_} {
  }

  /////////////////////////////////////////////////////////////////////////////

  // IMPL_METHOD_3_2_
  constexpr
  t_info::operator t_bool() const noexcept {
    return get(on_bits);
  }

  // IMPL_METHOD_3_3_
  constexpr
  t_bits t_info::free_bits() const noexcept {
    return t_bits{get(max_bits) - get(msb_on.bits())};
  }

  // IMPL_METHOD_3_4_
  constexpr
  t_bool t_info::is_neg() const noexcept {
    return get(max_bits) == ((get(msb_on.ix)*BITS_UNIT_) + get(msb_on.bit));
  }

  /////////////////////////////////////////////////////////////////////////////

  constexpr
  R_pvalue_ t_buf_::cref(t_ix ix) const noexcept {
    return ptr[get(ix)];
  }

  constexpr
  r_pvalue_ t_buf_::ref(t_ix ix) noexcept {
    return ptr[get(ix)];
  }

  ///////////////////////////////////////////////////////////////////////////
}
}
}
}

#endif
