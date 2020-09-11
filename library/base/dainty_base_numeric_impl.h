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

#include <stdlib.h>
#include "dainty_base_types.h"
#include "dainty_base_specific.h"
#include "dainty_base_traits.h"
#include "dainty_base.h"

namespace dainty
{
namespace base
{
namespace numeric
{
namespace impl_
{
  /////////////////////////////////////////////////////////////////////////////

  using types::t_void;
  using types::t_bool;
  using specific::mk;
  using specific::max_of;
  using specific::min_of;
  using specific::t_n;
  using specific::t_n_;
  using specific::t_ix;
  using specific::t_ix_;
  using specific::operator""_n;
  using specific::operator""_ix;

  /////////////////////////////////////////////////////////////////////////////

  using t_nvalue_ = types::t_schar;
  using t_pvalue_ = types::t_uchar;

  /*
  using t_nvalue_ = types::t_short;
  using t_pvalue_ = types::t_ushort;

  using t_nvalue_ = types::t_llong;
  using t_pvalue_ = types::t_ullong;
  */

  using p_pvalue_ = types::t_prefix<t_pvalue_>::p_;
  using P_pvalue_ = types::t_prefix<t_pvalue_>::P_;
  using r_pvalue_ = types::t_prefix<t_pvalue_>::r_;
  using R_pvalue_ = types::t_prefix<t_pvalue_>::R_;

  /////////////////////////////////////////////////////////////////////////////

  constexpr t_pvalue_ BITS_ZERO_  = (t_pvalue_)0;
  constexpr t_pvalue_ BITS_ONE_   = (t_pvalue_)1;
  constexpr t_pvalue_ BITS_ALL_   = (t_pvalue_)~BITS_ZERO_;
  constexpr t_pvalue_ BITS_BYTES_ = (t_pvalue_)sizeof(t_pvalue_);
  constexpr t_pvalue_ BITS_UNIT_  = (t_pvalue_)((BITS_BYTES_*8) - 1);
  constexpr t_pvalue_ BITS_MSB_   = (BITS_ONE_ << BITS_UNIT_);
  constexpr t_pvalue_ BITS_LAST_  = BITS_ONE_ << (BITS_UNIT_ - BITS_ONE_);
  constexpr t_pvalue_ BITS_MASK_  = (t_pvalue_)~BITS_MSB_;

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

  template<typename T>
  using t_is_sz_ = traits::t_bool_result<sizeof(T) <= sizeof(impl_::t_pvalue_)>;

  template<typename T> using t_if_neg_ =
    traits::t_if<
      traits::t_and<traits::t_is_signed_integral<T>, t_is_sz_<T>>>;

  template<typename T> using t_if_pos_ =
    traits::t_if<
      traits::t_and<traits::t_is_unsigned_integral<T>, t_is_sz_<T>>>;

  template<typename T> using t_if_int_ =
    traits::t_if<
      traits::t_and<
        traits::t_or<traits::t_is_signed_integral<T>,
                     traits::t_is_unsigned_integral<T>>, t_is_sz_<T>>>;

  /////////////////////////////////////////////////////////////////////////////

  struct t_buf_;
  using  r_buf_ = types::t_prefix<t_buf_>::r_;
  using  x_buf_ = types::t_prefix<t_buf_>::x_;
  using  p_buf_ = types::t_prefix<t_buf_>::p_;
  using  R_buf_ = types::t_prefix<t_buf_>::R_;

  // t_buf_ is actually t_buf
  struct t_buf_ {
    t_n       size    = 2_n; // support more than 64 bits - 127 bit
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

    t_bool clear(           t_ix = 0_ix) noexcept;
    t_bool fill (t_pvalue_, t_ix = 0_ix) noexcept;

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
  };

  using  t_store_ = t_buf_;
  using  r_store_ = r_buf_;
  using  p_store_ = p_buf_;
  using  x_store_ = x_buf_;
  using  R_store_ = R_buf_;

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

    t_void ones_complement(r_store_)               noexcept; // IMPL_METHOD_1_10_
    t_void twos_complement(r_store_)               noexcept; // IMPL_METHOD_1_11_
    t_void shift_left     (r_store_, t_n = 1_n)    noexcept; // IMPL_METHOD_1_12_
    t_void shift_right    (r_store_, t_n = 1_n)    noexcept; // IMPL_METHOD_1_13_
    t_bool set_bit        (r_store_, t_ix, t_bool) noexcept; // IMPL_METHOD_1_14_
    t_bool is_zero        (R_store_)         const noexcept; // IMPL_METHOD_1_15_
    t_n    get_bits       (R_store_)         const noexcept; // IMPL_METHOD_1_16_
    t_bool get_bit        (R_store_, t_ix)   const noexcept; // IMPL_METHOD_1_17_
    t_bool is_negative    (R_store_)         const noexcept; // IMPL_METHOD_1_18_

    ///////////////////////////////////////////////////////////////////////////

    t_bool ensure(r_store_, t_n)            noexcept; // IMPL_METHOD_1_19_1_
    t_bool ensure(r_store_,      R_store_)  noexcept; // IMPL_METHOD_1_19_2_
    t_bool ensure(r_store_, t_n, R_store_)  noexcept; // IMPL_METHOD_1_19_3_

    ///////////////////////////////////////////////////////////////////////////

    t_void reset(r_store_)                 noexcept; // IMPL_METHOD_1_20_1_
    t_void reset(r_store_,      t_nvalue_) noexcept; // IMPL_METHOD_1_20_2_
    t_bool reset(r_store_, t_n, t_nvalue_) noexcept; // IMPL_METHOD_1_20_3_
    t_void reset(r_store_,      t_pvalue_) noexcept; // IMPL_METHOD_1_20_4_
    t_bool reset(r_store_, t_n, t_pvalue_) noexcept; // IMPL_METHOD_1_20_5_
    t_bool reset(r_store_,      R_store_)  noexcept; // IMPL_METHOD_1_20_6_
    t_bool reset(r_store_, t_n, R_store_)  noexcept; // IMPL_METHOD_1_20_7_

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
    t_ix   first_on_bit(R_store_) const noexcept; // BIN_METHOD_1_27_
    t_ix   last_on_bit (R_store_) const noexcept; // BIN_METHOD_1_28_
    t_n    on_bits     (R_store_) const noexcept; // BIN_METHOD_1_29_

    ///////////////////////////////////////////////////////////////////////////
  };

  /////////////////////////////////////////////////////////////////////////////

  using t_binary_ = t_impl_base_; // Don't support variable grow

  /////////////////////////////////////////////////////////////////////////////

  struct t_bpos_;
  using  R_bpos_ = types::t_prefix<t_bpos_>::R_;

  struct t_bpos_ {
    t_ix ix  = 0_ix;
    t_n  bit = 0_n;

    constexpr // IMPL_METHOD_2_1_1_
    t_bpos_() noexcept = default;

    constexpr // IMPL_METHOD_2_1_2_
    t_bpos_(t_ix ix_, t_n bit_) noexcept;

    constexpr // IMPL_METHOD_2_2_
    operator t_bool() const noexcept;

    template<typename T> // IMPL_METHOD_2_3_
    constexpr
    t_ix bit_ix(T) const noexcept;
  };

  /////////////////////////////////////////////////////////////////////////////

  struct t_info_;
  using  R_info_ = types::t_prefix<t_info_>::R_;

  struct t_info_ {
    t_bpos_ msb_on   = {};
    t_n     on_bits  = 0_n;
    t_n     max_bits = 0_n;

    constexpr // IMPL_METHOD_3_1_1_
    t_info_() noexcept = default;

    constexpr // IMPL_METHOD_3_1_2_
    t_info_(t_n max_bits_) noexcept;

    constexpr // IMPL_METHOD_3_1_3_
    t_info_(R_bpos_ msb_on_, t_n on_bits_, t_n max_bits_) noexcept;

    constexpr // IMPL_METHOD_3_2_
    operator t_bool() const noexcept;

    constexpr // IMPL_METHOD_3_3_
    t_n free_bits() const noexcept;

    constexpr // IMPL_METHOD_3_4_
    t_bool is_neg() const noexcept;
  };

  /////////////////////////////////////////////////////////////////////////////

  t_void   display_bits_  (R_store_, t_bool)        noexcept; // IMPL_FUNC_1_1_
  t_info_  bits_info_     (R_store_)                noexcept; // IMPL_FUNC_1_2_
  t_n      free_bits_     (R_store_)                noexcept; // IMPL_FUNC_1_3_
  t_n      on_bits_       (R_store_)                noexcept; // IMPL_FUNC_1_4_
  t_bpos_  lsb_on_bit_    (R_store_)                noexcept; // IMPL_FUNC_1_5_
  t_bpos_  msb_on_bit_    (R_store_)                noexcept; // IMPL_FUNC_1_6_
  t_bpos_  msb_off_bit_   (R_store_)                noexcept; // IMPL_FUNC_1_7_
  t_void   ones_compl_    (r_store_)                noexcept; // IMPL_FUNC_1_8_
  t_void   twos_compl_    (r_store_)                noexcept; // IMPL_FUNC_1_9_
  t_bool   assign_        (r_store_, R_store_)      noexcept; // IMPL_FUNC_1_10_
  t_bool   add_           (r_store_, R_store_)      noexcept; // IMPL_FUNC_1_11_
  t_bool   minus_         (r_store_, R_store_)      noexcept; // IMPL_FUNC_1_12_
  t_bool   multiply_      (r_store_, R_store_)      noexcept; // IMPL_FUNC_1_13_
  t_bool   divide_        (r_store_, R_store_)      noexcept; // IMPL_FUNC_1_14_
  t_bool   and_           (r_store_, R_store_)      noexcept; // IMPL_FUNC_1_15_
  t_bool   or_            (r_store_, R_store_)      noexcept; // IMPL_FUNC_1_16_
  t_bool   xor_           (r_store_, R_store_)      noexcept; // IMPL_FUNC_1_17_
  t_bool   shift_left_    (r_store_, t_n = 1_n)     noexcept; // IMPL_FUNC_1_18_
  t_bool   shift_right_   (r_store_, t_n = 1_n)     noexcept; // IMPL_FUNC_1_19_
  t_n      shift_next_    (r_store_)                noexcept; // IMPL_FUNC_1_20_
  t_bool   set_bit_       (r_store_, t_ix, t_bool)  noexcept; // IMPL_FUNC_1_21_
  t_bool   get_bit_       (R_store_, t_ix)          noexcept; // IMPL_FUNC_1_22_
  t_bool   is_zero_       (R_store_)                noexcept; // IMPL_FUNC_1_23_
  t_bool   reset_         (r_store_, R_store_)      noexcept; // IMPL_FUNC_1_24_
  t_bool   reset_         (r_store_, t_n, R_store_) noexcept; // IMPL_FUNC_1_25_
  t_bool   is_equal_      (R_store_, R_store_)      noexcept; // IMPL_FUNC_1_26_
  t_bool   is_less_       (R_store_, R_store_)      noexcept; // IMPL_FUNC_1_27_
  t_bool   is_less_equal_ (R_store_, R_store_)      noexcept; // IMPL_FUNC_1_28_
  t_n      calc_bits_     (t_n)                     noexcept; // IMPL_FUNC_1_29_
  t_n      calc_digits_   (t_n)                     noexcept; // IMPL_FUNC_1_30_
  t_bool   ensure_        (r_store_, t_n)           noexcept; // IMPL_FUNC_1_31_
  t_bool   ensure_        (r_store_, R_store_)      noexcept; // IMPL_FUNC_1_32_
  t_bool   ensure_        (r_store_, t_n, R_store_) noexcept; // IMPL_FUNC_1_33_

  /////////////////////////////////////////////////////////////////////////////

  // IMPL_FUNC_2_1_
  constexpr
  t_n calc_size_(t_n bits) noexcept {
    return t_n((get(bits)/BITS_UNIT_) + ((get(bits) % BITS_UNIT_) != 0));
  }

  // IMPL_FUNC_2_2_
  constexpr
  t_n bits_(R_bpos_ pos) noexcept {
    return t_n{(BITS_UNIT_ * get(pos.ix)) + get(pos.bit)};
  }

  // IMPL_FUNC_2_3_
  constexpr
  t_ix last_ix_(R_store_ store) noexcept {
    return t_ix{get(store.size) - 1};
  }

  /////////////////////////////////////////////////////////////////////////////

  // IMPL_FUNC_2_4_1_
  constexpr
  r_pvalue_ last_(r_store_ store) noexcept {
    return store.ptr[get(last_ix_(store))];
  }

  // IMPL_FUNC_2_4_2_
  constexpr
  R_pvalue_ last_(R_store_ store) noexcept {
    return store.ptr[get(last_ix_(store))];
  }

  /////////////////////////////////////////////////////////////////////////////

  // IMPL_FUNC_2_5_
  constexpr
  t_bool lsb_(R_store_ store) noexcept {
    return store.ptr[0] & BITS_ONE_;
  }

  // IMPL_FUNC_2_6_
  constexpr
  t_bool msb_(R_store_ store) noexcept {
    return last_(store) & BITS_MSB_;
  }

  // IMPL_FUNC_2_7_
  constexpr
  t_bool is_neg_(R_store_ store) noexcept {
    return msb_(store);
  }

  // IMPL_FUNC_2_8_
  constexpr
  t_n max_bits_(R_store_ value) noexcept {
    return t_n{(get(value.size) * BITS_UNIT_) + 1};
  }

  // IMPL_FUNC_2_9_
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
  t_bool operator==(R_bpos_ lh, R_bpos_ rh) noexcept {
    return (lh.ix == rh.ix) && (lh.bit == rh.bit);
  }

  // IMPL_FUNC_3_2_
  constexpr
  t_bool operator<(R_bpos_ lh, R_bpos_ rh) noexcept {
    return lh.ix < rh.ix || (lh.ix == rh.ix && lh.bit < rh.bit);
  }

  // IMPL_FUNC_3_3_
  constexpr
  t_bool operator<=(R_bpos_ lh, R_bpos_ rh) noexcept {
    return  lh.ix < rh.ix || (lh.ix == rh.ix && lh.bit <= rh.bit);
  }

  // IMPL_FUNC_3_4_
  constexpr
  t_bool operator!=(R_bpos_ lh, R_bpos_ rh) noexcept {
    return !(lh == rh);
  }

  // IMPL_FUNC_3_5_
  constexpr
  t_bool operator>(R_bpos_ lh, R_bpos_ rh) noexcept {
    return rh < lh;
  }

  // IMPL_FUNC_3_6_
  constexpr
  t_bool operator>=(R_bpos_ lh, R_bpos_ rh) noexcept {
    return rh <= lh;
  }

  /////////////////////////////////////////////////////////////////////////////

  // IMPL_FUNC_4_1_
  inline
  t_void assign_(r_store_ store, t_pvalue_ value) noexcept {
    store.ptr[0] = value & BITS_MASK_;
    store.ptr[1] = value & BITS_MSB_ ? BITS_ONE_ : BITS_ZERO_;
    store.fill(BITS_ZERO_, 2_ix);
  }

  // IMPL_FUNC_4_2_
  inline
  t_void assign_(r_store_ store, t_nvalue_ value) noexcept {
    store.ptr[0] = value & BITS_MASK_;
    store.fill(value & BITS_MSB_ ? BITS_ALL_ : BITS_ZERO_, 1_ix);
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
  t_bool reset_(r_store_ store, t_n n, T value) noexcept {
    t_store_ value_store;
    assign_(value_store, value);
    return reset_(store, n, value_store);
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
  t_impl_base_::t_impl_base_(r_store_ store) noexcept {
  }

  // IMPL_METHOD_1_1_3_
  constexpr
  t_impl_base_::t_impl_base_(r_store_ store, t_nvalue_ value) noexcept {
    assign_(store, value);
  }

  // IMPL_METHOD_1_1_4_
  constexpr
  t_impl_base_::t_impl_base_(r_store_ store, t_pvalue_ value) noexcept {
    assign_(store, value);
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
  t_void t_impl_base_::shift_left(r_store_ store, t_n n) noexcept {
    shift_left_(store, n);
  }

  // IMPL_METHOD_1_13_
  inline
  t_void t_impl_base_::shift_right(r_store_ store, t_n n) noexcept {
    shift_right_(store, n);
  }

  // IMPL_METHOD_1_14_
  inline
  t_bool t_impl_base_::set_bit(r_store_ store, t_ix ix, t_bool on) noexcept {
    return set_bit_(store, ix, on);
  }

  // IMPL_METHOD_1_15_
  inline
  t_bool t_impl_base_::is_zero(R_store_ store) const noexcept {
    return is_zero_(store);
  }

  // IMPL_METHOD_1_16_
  inline
  t_n t_impl_base_::get_bits(R_store_ store) const noexcept {
    return max_bits_(store);
  }

  // IMPL_METHOD_1_17_
  inline
  t_bool t_impl_base_::get_bit(R_store_ store, t_ix ix) const noexcept {
    return get_bit_(store, ix);
  }

  // IMPL_METHOD_1_18_
  inline
  t_bool t_impl_base_::is_negative(R_store_ store) const noexcept {
    return is_neg_(store);
  }

  /////////////////////////////////////////////////////////////////////////////

  // IMPL_METHOD_1_19_1_
  inline
  t_bool t_impl_base_::ensure(r_store_ store, t_n n) noexcept {
    return ensure_(store, n);
  }

  // IMPL_METHOD_1_19_1_
  inline
  t_bool t_impl_base_::ensure(r_store_ store, R_store_ value) noexcept {
    return ensure_(store, value);
  }

  // IMPL_METHOD_1_19_2_
  inline
  t_bool t_impl_base_::ensure(r_store_ store, t_n n, R_store_ value) noexcept {
    return ensure_(store, n, value);
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
  t_bool t_impl_base_::reset(r_store_ store, t_n n, t_nvalue_ value) noexcept {
    return reset_(store, n, value);
  }

  // IMPL_METHOD_1_20_4_
  inline
  t_void t_impl_base_::reset(r_store_ store, t_pvalue_ value) noexcept {
    reset_(store, value);
  }

  // IMPL_METHOD_1_20_5_
  inline
  t_bool t_impl_base_::reset(r_store_ store, t_n n, t_pvalue_ value) noexcept {
    return reset_(store, n, value);
  }

  // IMPL_METHOD_1_20_6_
  inline
  t_bool t_impl_base_::reset(r_store_ store, R_store_ value) noexcept {
    return reset_(store, value);
  }

  // IMPL_METHOD_1_20_7_
  inline
  t_bool t_impl_base_::reset(r_store_ store, t_n n, R_store_ value) noexcept {
    return reset_(store, n, value);
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
  t_ix t_impl_base_::first_on_bit(R_store_ store) const noexcept {
    return lsb_on_bit_(store).bit_ix(get_bits(store));
  }

  // BIN_METHOD_1_28_
  inline
  t_ix t_impl_base_::last_on_bit(R_store_ store) const noexcept {
    return msb_on_bit_(store).bit_ix(get_bits(store));
  }

  // BIN_METHOD_1_29_
  inline
  t_n t_impl_base_::on_bits(R_store_ store) const noexcept {
    return on_bits_(store);
  }

  /////////////////////////////////////////////////////////////////////////////

  // IMPL_METHOD_2_1_2_
  constexpr
  t_bpos_::t_bpos_(t_ix ix_, t_n bit_) noexcept : ix{ix_}, bit{bit_} {
  }

  // IMPL_METHOD_2_2_
  constexpr
  t_bpos_::operator t_bool() const noexcept {
    return get(bit);
  };

  template<typename T> // IMPL_METHOD_2_3_
  constexpr
  t_ix t_bpos_::bit_ix(T value) const noexcept {
    return get(bit) ? t_ix{get(bits_(*this)) - 1} : mk<t_ix>(value);
  }

  /////////////////////////////////////////////////////////////////////////////

  // IMPL_METHOD_3_1_2_
  constexpr
  t_info_::t_info_(t_n max_bits_) noexcept : max_bits{max_bits_} {
  }

  // IMPL_METHOD_3_1_3_
  constexpr
  t_info_::t_info_(R_bpos_ msb_on_, t_n on_bits_, t_n max_bits_) noexcept
    : msb_on{msb_on_}, on_bits{on_bits_}, max_bits{max_bits_} {
  }

  /////////////////////////////////////////////////////////////////////////////

  // IMPL_METHOD_3_2_
  constexpr
  t_info_::operator t_bool() const noexcept {
    return get(on_bits);
  }

  // IMPL_METHOD_3_3_
  constexpr
  t_n t_info_::free_bits() const noexcept {
    return t_n{get(max_bits) - get(bits_(msb_on))};
  }

  // IMPL_METHOD_3_4_
  constexpr
  t_bool t_info_::is_neg() const noexcept {
    return get(max_bits) == ((get(msb_on.ix)*BITS_UNIT_) + get(msb_on.bit));
  }

  /////////////////////////////////////////////////////////////////////////////
}
}
}
}

#endif
