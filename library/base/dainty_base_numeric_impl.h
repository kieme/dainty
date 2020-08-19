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
  using specific::max_of;
  using specific::min_of;
  using specific::t_n;
  using specific::t_n_;
  using specific::t_ix;
  using specific::t_ix_;
  using specific::operator""_n;

  /////////////////////////////////////////////////////////////////////////////

  using t_nvalue_ = types::t_llong;
  using t_pvalue_ = types::t_ullong;
  using p_pvalue_ = types::t_prefix<t_pvalue_>::p_;
  using r_pvalue_ = types::t_prefix<t_pvalue_>::r_;
  using R_pvalue_ = types::t_prefix<t_pvalue_>::R_;

  /////////////////////////////////////////////////////////////////////////////

  constexpr t_pvalue_ BITS_UNIT_ = (t_pvalue_)((sizeof(p_pvalue_)*8) - 1);
  constexpr t_pvalue_ BITS_MSB_  = 1ULL << BITS_UNIT_;
  constexpr t_pvalue_ BITS_SIGN_ = BITS_MSB_;
  constexpr t_pvalue_ BITS_MASK_ = ~BITS_SIGN_;
  constexpr t_pvalue_ BITS_ONE_  = 1ULL;
  constexpr t_pvalue_ BITS_ZERO_ = 0ULL;
  constexpr t_pvalue_ BITS_ALL_  = ~BITS_ZERO_;

  static_assert(BITS_UNIT_ ==                 63ULL);
  static_assert(BITS_MSB_  == 0x8000000000000000ULL);
  static_assert(BITS_SIGN_ == 0x8000000000000000ULL);
  static_assert(BITS_ONE_  ==                  1ULL);
  static_assert(BITS_ZERO_ ==                  0ULL);
  static_assert(BITS_MASK_ == 0x7fffffffffffffffULL);
  static_assert(BITS_ALL_  == 0xffffffffffffffffULL);

  /////////////////////////////////////////////////////////////////////////////

  struct t_buf_;
  using  r_buf_ = types::t_prefix<t_buf_>::r_;
  using  x_buf_ = types::t_prefix<t_buf_>::x_;
  using  R_buf_ = types::t_prefix<t_buf_>::R_;

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

    t_bool ensure(t_n)         noexcept;
    t_bool ensure(     R_buf_) noexcept;
    t_bool ensure(t_n, R_buf_) noexcept;
    t_bool ensure(     x_buf_) noexcept;
    t_bool ensure(t_n, x_buf_) noexcept;

    ///////////////////////////////////////////////////////////////////////////

    t_bool reset(t_n)         noexcept;
    t_bool reset(     R_buf_) noexcept;
    t_bool reset(t_n, R_buf_) noexcept;
    t_bool reset(     x_buf_) noexcept;
    t_bool reset(t_n, x_buf_) noexcept;

    ///////////////////////////////////////////////////////////////////////////

    t_bool is_valid() const noexcept;
    t_bool is_heap () const noexcept;
    t_void clear   ()       noexcept;
    t_void steal   (r_buf_) noexcept;

    ///////////////////////////////////////////////////////////////////////////
  };

  using  t_store_ = t_buf_;
  using  r_store_ = r_buf_;
  using  x_store_ = x_buf_;
  using  R_store_ = R_buf_;

///////////////////////////////////////////////////////////////////////////////

  class t_impl_base_ {
  public:
    constexpr
    t_impl_base_()                    noexcept; // IMPL_METHOD_1_1_
    constexpr
    t_impl_base_(r_store_)            noexcept; // IMPL_METHOD_1_2_
    constexpr
    t_impl_base_(r_store_, t_nvalue_) noexcept; // IMPL_METHOD_1_3_
    constexpr
    t_impl_base_(r_store_, t_pvalue_) noexcept; // IMPL_METHOD_1_4_
    t_impl_base_(r_store_, R_store_)  noexcept; // IMPL_METHOD_1_5_

    ///////////////////////////////////////////////////////////////////////////

    t_void assign(r_store_, t_nvalue_) noexcept; // IMPL_METHOD_2_1_
    t_void assign(r_store_, t_pvalue_) noexcept; // IMPL_METHOD_2_2_
    t_void assign(r_store_, R_store_)  noexcept; // IMPL_METHOD_2_3_

    ///////////////////////////////////////////////////////////////////////////

    t_void addition(r_store_, t_nvalue_) noexcept; // IMPL_METHOD_3_1_
    t_void addition(r_store_, t_pvalue_) noexcept; // IMPL_METHOD_3_2_
    t_void addition(r_store_, R_store_)  noexcept; // IMPL_METHOD_3_3_

    ///////////////////////////////////////////////////////////////////////////

    t_void subtraction(r_store_, t_nvalue_) noexcept; // IMPL_METHOD_4_1_
    t_void subtraction(r_store_, t_pvalue_) noexcept; // IMPL_METHOD_4_2_
    t_void subtraction(r_store_, R_store_)  noexcept; // IMPL_METHOD_4_3_

    ///////////////////////////////////////////////////////////////////////////

    t_void multiplication(r_store_, t_nvalue_) noexcept; // IMPL_METHOD_5_1_
    t_void multiplication(r_store_, t_pvalue_) noexcept; // IMPL_METHOD_5_2_
    t_void multiplication(r_store_, R_store_)  noexcept; // IMPL_METHOD_5_3_

    ///////////////////////////////////////////////////////////////////////////

    t_void division(r_store_, t_nvalue_) noexcept; // IMPL_METHOD_6_1_
    t_void division(r_store_, t_pvalue_) noexcept; // IMPL_METHOD_6_2_
    t_void division(r_store_, R_store_)  noexcept; // IMPL_METHOD_6_3_

    ///////////////////////////////////////////////////////////////////////////

    t_void binary_and(r_store_, t_nvalue_) noexcept; // IMPL_METHOD_7_1_
    t_void binary_and(r_store_, t_pvalue_) noexcept; // IMPL_METHOD_7_2_
    t_void binary_and(r_store_, R_store_)  noexcept; // IMPL_METHOD_7_3_

    ///////////////////////////////////////////////////////////////////////////

    t_void binary_or(r_store_, t_nvalue_) noexcept; // IMPL_METHOD_8_1_
    t_void binary_or(r_store_, t_pvalue_) noexcept; // IMPL_METHOD_8_2_
    t_void binary_or(r_store_, R_store_)  noexcept; // IMPL_METHOD_8_3_

    ///////////////////////////////////////////////////////////////////////////

    t_void binary_xor(r_store_, t_nvalue_) noexcept; // IMPL_METHOD_9_1_
    t_void binary_xor(r_store_, t_pvalue_) noexcept; // IMPL_METHOD_9_2_
    t_void binary_xor(r_store_, R_store_)  noexcept; // IMPL_METHOD_9_3_

    ///////////////////////////////////////////////////////////////////////////

    t_void ones_complement(r_store_)               noexcept; // IMPL_METHOD_10_
    t_void twos_complement(r_store_)               noexcept; // IMPL_METHOD_11_
    t_void shift_left     (r_store_, t_n)          noexcept; // IMPL_METHOD_12_
    t_void shift_right    (r_store_, t_n)          noexcept; // IMPL_METHOD_13_
    t_bool set_bit        (r_store_, t_ix, t_bool) noexcept; // IMPL_METHOD_14_
    t_bool not_zero       (R_store_)         const noexcept; // IMPL_METHOD_15_
    t_n    get_bits       (R_store_)         const noexcept; // IMPL_METHOD_16_
    t_bool get_bit        (R_store_, t_ix)   const noexcept; // IMPL_METHOD_17_
    t_bool is_negative    (R_store_)         const noexcept; // IMPL_METHOD_18_

    ///////////////////////////////////////////////////////////////////////////

    t_bool ensure(r_store_, t_n)            noexcept; // IMPL_METHOD_19_1_
    t_bool ensure(r_store_,      R_store_)  noexcept; // IMPL_METHOD_19_2_
    t_bool ensure(r_store_, t_n, R_store_)  noexcept; // IMPL_METHOD_19_3_

    ///////////////////////////////////////////////////////////////////////////

    t_void reset(r_store_,      t_nvalue_) noexcept; // IMPL_METHOD_20_1_
    t_bool reset(r_store_, t_n, t_nvalue_) noexcept; // IMPL_METHOD_20_2_
    t_void reset(r_store_,      t_pvalue_) noexcept; // IMPL_METHOD_20_3_
    t_bool reset(r_store_, t_n, t_pvalue_) noexcept; // IMPL_METHOD_20_4_
    t_bool reset(r_store_,      R_store_)  noexcept; // IMPL_METHOD_20_5_
    t_bool reset(r_store_, t_n, R_store_)  noexcept; // IMPL_METHOD_20_6_

    ///////////////////////////////////////////////////////////////////////////

    t_bool is_equal(R_store_, R_store_)  const noexcept; // IMPL_METHOD_21_1_
    t_bool is_equal(R_store_, t_pvalue_) const noexcept; // IMPL_METHOD_21_2_
    t_bool is_equal(R_store_, t_nvalue_) const noexcept; // IMPL_METHOD_21_3_

    ///////////////////////////////////////////////////////////////////////////

    t_bool is_less(R_store_, R_store_)  const noexcept; // IMPL_METHOD_22_1_
    t_bool is_less(R_store_, t_pvalue_) const noexcept; // IMPL_METHOD_22_2_
    t_bool is_less(R_store_, t_nvalue_) const noexcept; // IMPL_METHOD_22_3_

    ///////////////////////////////////////////////////////////////////////////

    t_bool is_less_equal(R_store_, R_store_)  const noexcept; // IMPL_METHOD_23_1_
    t_bool is_less_equal(R_store_, t_pvalue_) const noexcept; // IMPL_METHOD_23_2_
    t_bool is_less_equal(R_store_, t_nvalue_) const noexcept; // IMPL_METHOD_23_3_

    ///////////////////////////////////////////////////////////////////////////
  };

  /////////////////////////////////////////////////////////////////////////////

  using t_binary_ = t_impl_base_; // Don't support variable grow

  /////////////////////////////////////////////////////////////////////////////

  r_store_ ones_compl_   (r_store_)                noexcept; // IMPL_FUNC_1_1_
  r_store_ twos_compl_   (r_store_)                noexcept; // IMPL_FUNC_1_2_
  t_void   assign_       (r_store_, R_store_)      noexcept; // IMPL_FUNC_1_3_
  t_void   add_          (r_store_, R_store_)      noexcept; // IMPL_FUNC_1_4_
  t_void   minus_        (r_store_, R_store_)      noexcept; // IMPL_FUNC_1_5_
  t_void   multiply_     (r_store_, R_store_)      noexcept; // IMPL_FUNC_1_6_
  t_void   divide_       (r_store_, R_store_)      noexcept; // IMPL_FUNC_1_7_
  t_void   and_          (r_store_, R_store_)      noexcept; // IMPL_FUNC_1_8_
  t_void   or_           (r_store_, R_store_)      noexcept; // IMPL_FUNC_1_9_
  t_void   xor_          (r_store_, R_store_)      noexcept; // IMPL_FUNC_1_10_
  t_void   shift_left_   (r_store_, t_n)           noexcept; // IMPL_FUNC_1_11_
  t_void   shift_right_  (r_store_, t_n)           noexcept; // IMPL_FUNC_1_12_
  t_bool   set_bit_      (r_store_, t_ix, t_bool)  noexcept; // IMPL_FUNC_1_13_
  t_bool   get_bit_      (R_store_, t_ix)          noexcept; // IMPL_FUNC_1_14_
  t_bool   not_zero_     (R_store_)                noexcept; // IMPL_FUNC_1_16_
  t_bool   reset_        (r_store_, R_store_)      noexcept; // IMPL_FUNC_1_17_
  t_bool   reset_        (r_store_, t_n, R_store_) noexcept; // IMPL_FUNC_1_18_
  t_bool   is_equal_     (R_store_, R_store_)      noexcept; // IMPL_FUNC_1_19_
  t_bool   is_less_      (R_store_, R_store_)      noexcept; // IMPL_FUNC_1_20_
  t_bool   is_less_equal_(R_store_, R_store_)      noexcept; // IMPL_FUNC_1_21_
  t_n      calc_bits_    (t_n)                     noexcept; // IMPL_FUNC_1_22_
  t_n      calc_digits_  (t_n)                     noexcept; // IMPL_FUNC_1_23_
  t_bool   ensure_       (r_store_, t_n)           noexcept; // IMPL_FUNC_1_24_
  t_bool   ensure_       (r_store_, R_store_)      noexcept; // IMPL_FUNC_1_25_
  t_bool   ensure_       (r_store_, t_n, R_store_) noexcept; // IMPL_FUNC_1_26_

  /////////////////////////////////////////////////////////////////////////////

  enum class t_sign_      : types::t_int { NEG = -1, POS = 1 };
  enum class t_same_sign_ : types::t_int { NEG = -1, NO  = 0, POS = 1 };

  using t_negs_ = t_bool[2];

  /////////////////////////////////////////////////////////////////////////////

  // IMPL_FUNC_2_1_
  constexpr
  t_n calc_(t_n bits) noexcept {
    return t_n((get(bits)/BITS_UNIT_) + ((get(bits) % BITS_UNIT_) != 0));
  }

  // IMPL_FUNC_2_2_
  constexpr
  r_pvalue_ last_(r_store_ store) noexcept {
    return store.ptr[get(store.size) - 1];
  }

  // IMPL_FUNC_2_3_
  constexpr
  R_pvalue_ last_(R_store_ store) noexcept {
    return store.ptr[get(store.size) - 1];
  }

  // IMPL_FUNC_2_4_
  constexpr
  t_bool is_negative_(R_store_ store) noexcept {
    return last_(store) & BITS_SIGN_;
  }

  // IMPL_FUNC_2_5_
  constexpr
  t_same_sign_ same_sign_(R_store_ store, t_pvalue_ value) noexcept {
    return is_negative_(store) ? t_same_sign_::NO : t_same_sign_::POS;
  }

  // IMPL_FUNC_2_6_
  constexpr
  t_same_sign_ same_sign_(R_store_ store, t_nvalue_ value) noexcept {
    t_negs_ negs = { is_negative_(store), value < 0L };
    if (negs[0] == negs[1])
      return negs[0] ? t_same_sign_::NEG : t_same_sign_::POS;
    return t_same_sign_::NO;
  }

  // IMPL_FUNC_2_7_
  constexpr
  t_same_sign_ same_sign_(R_store_ store, R_store_ value) noexcept {
    t_negs_ negs = { is_negative_(store), is_negative_(value) };
    if (negs[0] == negs[1])
      return negs[0] ? t_same_sign_::NEG : t_same_sign_::POS;
    return t_same_sign_::NO;
  }

  // IMPL_FUNC_2_8_
  constexpr
  t_n get_bits_(R_store_ store) noexcept {
    return t_n((get(store.size) * BITS_UNIT_) + 1);
  }

  // IMPL_FUNC_2_9_
  constexpr
  t_void assign_(r_store_ store, t_pvalue_ value) noexcept {
    store.ptr[0] = value & BITS_MASK_;
    store.ptr[1] = value & BITS_MSB_ ? BITS_ONE_ : BITS_ZERO_;
  }

  // IMPL_FUNC_2_10_
  constexpr
  t_void assign_(r_store_ store, t_nvalue_ value) noexcept {
    store.ptr[0] = value & BITS_MASK_;
    store.ptr[1] = value & BITS_SIGN_ ? BITS_ALL_ : BITS_ZERO_;
  }

  /////////////////////////////////////////////////////////////////////////////

  // IMPL_FUNC_2_11_
  template<typename T>
  inline
  t_void add_(r_store_ store, T value) noexcept {
    t_store_ value_store;
    assign_(value_store, value);
    add_(store, value_store);
  }

  // IMPL_FUNC_2_12_
  template<typename T>
  inline
  t_void minus_(r_store_ store, T value) noexcept {
    t_store_ value_store;
    assign_(value_store, value);
    minus_(store, value_store);
  }

  // IMPL_FUNC_2_13_
  template<typename T>
  inline
  t_void multiply_(r_store_ store, T value) noexcept {
    t_store_ value_store;
    assign_(value_store, value);
    multiply_(store, value_store);
  }

  // IMPL_FUNC_2_14_
  template<typename T>
  inline
  t_void divide_(r_store_ store, T value) noexcept {
    t_store_ value_store;
    assign_(value_store, value);
    divide_(store, value_store);
  }

  // IMPL_FUNC_2_15_
  template<typename T>
  inline
  t_void and_(r_store_ store, T value) noexcept {
    t_store_ value_store;
    assign_(value_store, value);
    and_(store, value_store);
  }

  // IMPL_FUNC_2_16_
  template<typename T>
  inline
  t_void or_(r_store_ store, T value) noexcept {
    t_store_ value_store;
    assign_(value_store, value);
    or_(store, value_store);
  }

  // IMPL_FUNC_2_17_
  template<typename T>
  inline
  t_void xor_(r_store_ store, T value) noexcept {
    t_store_ value_store;
    assign_(value_store, value);
    xor_(store, value_store);
  }

  /////////////////////////////////////////////////////////////////////////////

  // IMPL_FUNC_2_18_
  template<typename T>
  inline
  t_void reset_(r_store_ store, T value) noexcept {
    t_store_ value_store;
    assign_(value_store, value);
    reset_(store, value_store);
  }

  // IMPL_FUNC_2_19_
  template<typename T>
  inline
  t_bool reset_(r_store_ store, t_n n, T value) noexcept {
    t_store_ value_store;
    assign_(value_store, value);
    return reset_(store, n, value_store);
  }

  /////////////////////////////////////////////////////////////////////////////

  // IMPL_FUNC_2_20_
  template<typename T>
  inline
  t_bool is_equal_(R_store_ store, T value) noexcept {
    t_store_ value_store;
    assign_(value_store, value);
    return is_equal_(store, value_store);
  }

  // IMPL_FUNC_2_21_
  template<typename T>
  inline
  t_bool is_less_(R_store_ store, T value) noexcept {
    t_store_ value_store;
    assign_(value_store, value);
    return is_less_(store, value_store);
  }

  // IMPL_FUNC_2_22_
  template<typename T>
  inline
  t_bool is_less_equal_(R_store_ store, T value) noexcept {
    t_store_ value_store;
    assign_(value_store, value);
    return is_less_equal_(store, value_store);
  }

  /////////////////////////////////////////////////////////////////////////////

  // IMPL_METHOD_1_1_
  constexpr
  t_impl_base_::t_impl_base_() noexcept {
  }

  // IMPL_METHOD_1_2_
  constexpr
  t_impl_base_::t_impl_base_(r_store_ store) noexcept {
  }

  // IMPL_METHOD_1_3_
  constexpr
  t_impl_base_::t_impl_base_(r_store_ store, t_nvalue_ value) noexcept {
    assign_(store, value);
  }

  // IMPL_METHOD_1_4_
  constexpr
  t_impl_base_::t_impl_base_(r_store_ store, t_pvalue_ value) noexcept {
    assign_(store, value);
  }

  // IMPL_METHOD_1_5_
  inline
  t_impl_base_::t_impl_base_(r_store_ store, R_store_ value) noexcept {
    assign_(store, value);
  }

  /////////////////////////////////////////////////////////////////////////////

  // IMPL_METHOD_2_1_
  inline
  t_void t_impl_base_::assign(r_store_ store, t_nvalue_ value) noexcept {
    assign_(store, value);
  }

  // IMPL_METHOD_2_2_
  inline
  t_void t_impl_base_::assign(r_store_ store, t_pvalue_ value) noexcept {
    assign_(store, value);
  }

  // IMPL_METHOD_2_3_
  inline
  t_void t_impl_base_::assign(r_store_ store, R_store_ value) noexcept {
    assign_(store, value);
  }

  /////////////////////////////////////////////////////////////////////////////

  // IMPL_METHOD_3_1_
  inline
  t_void t_impl_base_::addition(r_store_ store, t_nvalue_ value) noexcept {
    add_(store, value);
  }

  // IMPL_METHOD_3_2_
  inline
  t_void t_impl_base_::addition(r_store_ store, t_pvalue_ value) noexcept {
    add_(store, value);
  }

  // IMPL_METHOD_3_3_
  inline
  t_void t_impl_base_::addition(r_store_ store, R_store_ value) noexcept {
    add_(store, value);
  }

  /////////////////////////////////////////////////////////////////////////////

  // IMPL_METHOD_4_1_
  inline
  t_void t_impl_base_::subtraction(r_store_ store, t_nvalue_ value) noexcept {
    minus_(store, value);
  }

  // IMPL_METHOD_4_2_
  inline
  t_void t_impl_base_::subtraction(r_store_ store, t_pvalue_ value) noexcept {
    minus_(store, value);
  }

  // IMPL_METHOD_4_3_
  inline
  t_void t_impl_base_::subtraction(r_store_ store, R_store_ value) noexcept {
    minus_(store, value);
  }

  /////////////////////////////////////////////////////////////////////////////

  // IMPL_METHOD_5_1_
  inline
  t_void t_impl_base_::multiplication(r_store_ store, t_nvalue_ value) noexcept {
    multiply_(store, value);
  }

  // IMPL_METHOD_5_2_
  inline
  t_void t_impl_base_::multiplication(r_store_ store, t_pvalue_ value) noexcept {
    multiply_(store, value);
  }

  // IMPL_METHOD_5_3_
  inline
  t_void t_impl_base_::multiplication(r_store_ store, R_store_ value) noexcept {
    multiply_(store, value);
  }

  /////////////////////////////////////////////////////////////////////////////

  // IMPL_METHOD_6_1_
  inline
  t_void t_impl_base_::division(r_store_ store, t_nvalue_ value) noexcept {
    divide_(store, value);
  }

  // IMPL_METHOD_6_2_
  inline
  t_void t_impl_base_::division(r_store_ store, t_pvalue_ value) noexcept {
    divide_(store, value);
  }

  // IMPL_METHOD_6_3_
  inline
  t_void t_impl_base_::division(r_store_ store, R_store_ value) noexcept {
    divide_(store, value);
  }

  /////////////////////////////////////////////////////////////////////////////

  // IMPL_METHOD_7_1_
  inline
  t_void t_impl_base_::binary_and(r_store_ store, t_nvalue_ value) noexcept {
    and_(store, value);
  }

  // IMPL_METHOD_7_2_
  inline
  t_void t_impl_base_::binary_and(r_store_ store, t_pvalue_ value) noexcept {
    and_(store, value);
  }

  // IMPL_METHOD_7_3_
  inline
  t_void t_impl_base_::binary_and(r_store_ store, R_store_ value) noexcept {
    and_(store, value);
  }

  /////////////////////////////////////////////////////////////////////////////

  // IMPL_METHOD_8_1_
  inline
  t_void t_impl_base_::binary_or(r_store_ store, t_nvalue_ value) noexcept {
    or_(store, value);
  }

  // IMPL_METHOD_8_2_
  inline
  t_void t_impl_base_::binary_or(r_store_ store, t_pvalue_ value) noexcept {
    or_(store, value);
  }

  // IMPL_METHOD_8_3_
  inline
  t_void t_impl_base_::binary_or(r_store_ store, R_store_ value)  noexcept {
    or_(store, value);
  }

  /////////////////////////////////////////////////////////////////////////////

  // IMPL_METHOD_9_1_
  inline
  t_void t_impl_base_::binary_xor(r_store_ store, t_nvalue_ value) noexcept {
    xor_(store, value);
  }

  // IMPL_METHOD_9_2_
  inline
  t_void t_impl_base_::binary_xor(r_store_ store, t_pvalue_ value) noexcept {
    xor_(store, value);
  }

  // IMPL_METHOD_9_3_
  inline
  t_void t_impl_base_::binary_xor(r_store_ store, R_store_ value) noexcept {
    xor_(store, value);
  }

  /////////////////////////////////////////////////////////////////////////////

  // IMPL_METHOD_10_
  inline
  t_void t_impl_base_::ones_complement(r_store_ store) noexcept {
    ones_compl_(store);
  }

  // IMPL_METHOD_11_
  inline
  t_void t_impl_base_::twos_complement(r_store_ store) noexcept {
    twos_compl_(store);
  }

  // IMPL_METHOD_12_
  inline
  t_void t_impl_base_::shift_left(r_store_ store, t_n n) noexcept {
    shift_left_(store, n);
  }

  // IMPL_METHOD_13_
  inline
  t_void t_impl_base_::shift_right(r_store_ store, t_n n) noexcept {
    shift_right_(store, n);
  }

  // IMPL_METHOD_14_
  inline
  t_bool t_impl_base_::set_bit(r_store_ store, t_ix ix, t_bool on) noexcept {
    return set_bit_(store, ix, on);
  }

  // IMPL_METHOD_15_
  inline
  t_bool t_impl_base_::not_zero(R_store_ store) const noexcept {
    return not_zero_(store);
  }

  // IMPL_METHOD_16_
  inline
  t_n t_impl_base_::get_bits(R_store_ store) const noexcept {
    return get_bits_(store);
  }

  // IMPL_METHOD_17_
  inline
  t_bool t_impl_base_::get_bit(R_store_ store, t_ix ix) const noexcept {
    return get_bit_(store, ix);
  }

  // IMPL_METHOD_18_
  inline
  t_bool t_impl_base_::is_negative(R_store_ store) const noexcept {
    return is_negative_(store);
  }

  /////////////////////////////////////////////////////////////////////////////

  // IMPL_METHOD_19_1_
  inline
  t_bool t_impl_base_::ensure(r_store_ store, t_n n) noexcept {
    return ensure_(store, n);
  }

  // IMPL_METHOD_19_1_
  inline
  t_bool t_impl_base_::ensure(r_store_ store, R_store_ value) noexcept {
    return ensure_(store, value);
  }

  // IMPL_METHOD_19_2_
  inline
  t_bool t_impl_base_::ensure(r_store_ store, t_n n, R_store_ value) noexcept {
    return ensure_(store, n, value);
  }

  /////////////////////////////////////////////////////////////////////////////

  // IMPL_METHOD_20_1_
  inline
  t_void t_impl_base_::reset(r_store_ store, t_nvalue_ value) noexcept {
    reset_(store, value);
  }

  // IMPL_METHOD_20_2_
  inline
  t_bool t_impl_base_::reset(r_store_ store, t_n n, t_nvalue_ value) noexcept {
    return reset_(store, n, value);
  }

  // IMPL_METHOD_20_3_
  inline
  t_void t_impl_base_::reset(r_store_ store, t_pvalue_ value) noexcept {
    reset_(store, value);
  }

  // IMPL_METHOD_20_4_
  inline
  t_bool t_impl_base_::reset(r_store_ store, t_n n, t_pvalue_ value) noexcept {
    return reset_(store, n, value);
  }

  // IMPL_METHOD_20_5_
  inline
  t_bool t_impl_base_::reset(r_store_ store, R_store_ value) noexcept {
    return reset_(store, value);
  }

  // IMPL_METHOD_20_6_
  inline
  t_bool t_impl_base_::reset(r_store_ store, t_n n, R_store_ value) noexcept {
    return reset_(store, n, value);
  }

  /////////////////////////////////////////////////////////////////////////////

  // IMPL_METHOD_21_1_
  inline
  t_bool t_impl_base_::is_equal(R_store_ store, R_store_ value) const noexcept {
    return is_equal_(store, value);
  }

  // IMPL_METHOD_21_2_
  inline
  t_bool t_impl_base_::is_equal(R_store_ store, t_pvalue_ value) const noexcept {
    return is_equal_(store, value);
  }

  // IMPL_METHOD_21_3_
  inline
  t_bool t_impl_base_::is_equal(R_store_ store, t_nvalue_ value) const noexcept {
    return is_equal_(store, value);
  }

  /////////////////////////////////////////////////////////////////////////////

  // IMPL_METHOD_22_1_
  inline
  t_bool t_impl_base_::is_less(R_store_ store, R_store_ value)  const noexcept {
    return is_less_(store, value);
  }

  // IMPL_METHOD_22_2_
  inline
  t_bool t_impl_base_::is_less(R_store_ store, t_pvalue_ value) const noexcept {
    return is_less_(store, value);
  }

  // IMPL_METHOD_22_3_
  inline
  t_bool t_impl_base_::is_less(R_store_ store, t_nvalue_ value) const noexcept {
    return is_less_(store, value);
  }

  /////////////////////////////////////////////////////////////////////////////

  // IMPL_METHOD_23_1_
  inline
  t_bool t_impl_base_::is_less_equal(R_store_ store, R_store_ value) const noexcept {
    return is_less_equal_(store, value);
  }

  // IMPL_METHOD_23_2_
  inline
  t_bool t_impl_base_::is_less_equal(R_store_ store, t_pvalue_ value) const noexcept {
    return is_less_equal_(store, value);
  }

  // IMPL_METHOD_23_3_
  inline
  t_bool t_impl_base_::is_less_equal(R_store_ store, t_nvalue_ value) const noexcept {
    return is_less_equal_(store, value);
  }

///////////////////////////////////////////////////////////////////////////////
}
}
}
}

#endif
