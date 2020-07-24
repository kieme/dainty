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
  using types::t_void;
  using types::t_bool;
  using specific::t_n;
  using specific::t_n_;
  using specific::t_ix;
  using specific::t_ix_;

///////////////////////////////////////////////////////////////////////////////

  using t_nvalue_ = types::t_llong;
  using t_pvalue_ = types::t_ullong;
  using p_pvalue_ = types::t_prefix<t_pvalue_>::p_;
  using r_pvalue_ = types::t_prefix<t_pvalue_>::r_;
  using R_pvalue_ = types::t_prefix<t_pvalue_>::R_;

///////////////////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////////////

  constexpr
  t_n calc_(t_n n) noexcept {
    return t_n((get(n)/BITS_UNIT_) + ((get(n) % BITS_UNIT_) ? 1ULL : 0ULL));
  }

  struct t_store_ {
    t_n       size    = t_n{2}; // why two - it holds the largest builtin integer
    t_pvalue_ sso_[2] = { 0, 0 };
    p_pvalue_ ptr     = sso_;

    ~t_store_() noexcept {
      if (ptr != sso_) {
        ::free(ptr);
      }
    }

    t_bool ensure_size(t_n n) noexcept {
      t_n need = calc_(n);
      if (get(size) < get(need)) {
        if (!is_heap()) {
          ptr = static_cast<p_pvalue_>(::malloc(get(need)));
          ptr[0] = sso_[0];
          ptr[1] = sso_[1];
        } else
          ptr = static_cast<p_pvalue_>(::realloc(ptr, get(need)));
        size = need;
      }
      return true;
    }

    t_void reset(t_n n) noexcept { // NOT OK - it must shrink
      t_n need = calc_(n);
      if (!is_heap()) {
        if (get(need) <= t_n{2}) {
          size = t_n{2};
          sso_[0] = ptr[0];
          sso_[1] = ptr[1];
          ::free (ptr);
        } else {
          size = need;
          ptr = static_cast<p_pvalue_>(::realloc(ptr, get(need)));
        }
      }
    }

    t_bool is_heap() const noexcept {
      return ptr != sso_;
    }

  };
  using r_store_ = types::t_prefix<t_store_>::r_;
  using x_store_ = types::t_prefix<t_store_>::x_;
  using R_store_ = types::t_prefix<t_store_>::R_;

///////////////////////////////////////////////////////////////////////////////

  class t_impl_base_ {
  public:
    ///////////////////////////////////////////////////////////////////////////

    t_impl_base_(r_store_)            noexcept; // BIN_METHOD_1_
    t_impl_base_(r_store_, t_nvalue_) noexcept; // BIN_METHOD_2_
    t_impl_base_(r_store_, t_pvalue_) noexcept; // BIN_METHOD_3_
    t_impl_base_(r_store_, R_store_)  noexcept; // BIN_METHOD_4_

    t_void assign(r_store_, t_nvalue_) noexcept; // BIN_METHOD_5_
    t_void assign(r_store_, t_pvalue_) noexcept; // BIN_METHOD_6_
    t_void assign(r_store_, R_store_)  noexcept; // BIN_METHOD_7_

    t_void addition(r_store_, t_nvalue_) noexcept; // BIN_METHOD_8_
    t_void addition(r_store_, t_pvalue_) noexcept; // BIN_METHOD_9_
    t_void addition(r_store_, R_store_)  noexcept; // BIN_METHOD_10_

    t_void subtraction(r_store_, t_nvalue_) noexcept; // BIN_METHOD_11_
    t_void subtraction(r_store_, t_pvalue_) noexcept; // BIN_METHOD_12_
    t_void subtraction(r_store_, R_store_)  noexcept; // BIN_METHOD_13_

    t_void multiplication(r_store_, t_nvalue_) noexcept; // BIN_METHOD_14_
    t_void multiplication(r_store_, t_pvalue_) noexcept; // BIN_METHOD_15_
    t_void multiplication(r_store_, R_store_)  noexcept; // BIN_METHOD_16_

    t_void division(r_store_, t_nvalue_) noexcept; // BIN_METHOD_17_
    t_void division(r_store_, t_pvalue_) noexcept; // BIN_METHOD_18_
    t_void division(r_store_, R_store_)  noexcept; // BIN_METHOD_19_

    t_void binary_and(r_store_, t_pvalue_) noexcept; // BIN_METHOD_21_
    t_void binary_and(r_store_, R_store_)  noexcept; // BIN_METHOD_22_

    t_void binary_or(r_store_, t_pvalue_) noexcept; // BIN_METHOD_24_
    t_void binary_or(r_store_, R_store_)  noexcept; // BIN_METHOD_25_

    t_void binary_xor(r_store_, t_pvalue_) noexcept; // BIN_METHOD_27_
    t_void binary_xor(r_store_, R_store_)  noexcept; // BIN_METHOD_28_

    t_void ones_complement(r_store_)       noexcept; // BIN_METHOD_29_
    t_void twos_complement(r_store_)       noexcept; // BIN_METHOD_30_
    t_void shift_left(r_store_, t_n)       noexcept; // BIN_METHOD_31_
    t_void shift_right(r_store_, t_n)      noexcept; // BIN_METHOD_32_
    t_void set_bit(r_store_, t_ix, t_bool) noexcept; // BIN_METHOD_33_
    t_void set_bits(r_store_, t_n)         noexcept; // BIN_METHOD_41_

    t_bool not_zero(R_store_)      const noexcept; // BIN_METHOD_41_
    t_n    get_bits(R_store_)      const noexcept; // BIN_METHOD_34_
    t_bool get_bit(R_store_, t_ix) const noexcept; // BIN_METHOD_35_
    t_bool is_negative(R_store_)   const noexcept; // BIN_METHOD_36_

    t_void reset(r_store_,      t_nvalue_) noexcept; // BIN_METHOD_37_
    t_void reset(r_store_, t_n, t_nvalue_) noexcept; // BIN_METHOD_38_
    t_void reset(r_store_,      t_pvalue_) noexcept; // BIN_METHOD_39_
    t_void reset(r_store_, t_n, t_pvalue_) noexcept; // BIN_METHOD_40_

    t_bool is_equal(R_store_, R_store_)  const noexcept; // BIN_METHOD_41_
    t_bool is_equal(R_store_, t_pvalue_) const noexcept; // BIN_METHOD_42_
    t_bool is_equal(R_store_, t_nvalue_) const noexcept; // BIN_METHOD_43_

    t_bool is_less(R_store_, R_store_)  const noexcept; // BIN_METHOD_44_
    t_bool is_less(R_store_, t_pvalue_) const noexcept; // BIN_METHOD_45_
    t_bool is_less(R_store_, t_nvalue_) const noexcept; // BIN_METHOD_46_

    t_bool is_less_equal(R_store_, R_store_)  const noexcept; // BIN_METHOD_47_
    t_bool is_less_equal(R_store_, t_pvalue_) const noexcept; // BIN_METHOD_48_
    t_bool is_less_equal(R_store_, t_nvalue_) const noexcept; // BIN_METHOD_49_

    ///////////////////////////////////////////////////////////////////////////
  };

  /////////////////////////////////////////////////////////////////////////////

  using t_binary_ = t_impl_base_; // Don't support variable grow

  /////////////////////////////////////////////////////////////////////////////

  constexpr
  r_pvalue_ last_(r_store_ store) noexcept {
    return store.ptr[get(store.size) - 1];
  }

  constexpr
  R_pvalue_ last_(R_store_ store) noexcept {
    return store.ptr[get(store.size) - 1];
  }

  constexpr
  t_bool is_negative_(R_store_ store) noexcept {
    return store.ptr[get(store.size) - 1] & BITS_SIGN_;
  }

  constexpr
  t_n_ get_bits_(R_store_ store) noexcept {
    return (get(store.size) * BITS_UNIT_) + 1;
  }

  inline
  r_store_ ones_complement_(r_store_ store) noexcept {
    t_ix_ last = get(store.size) - 1;
    for (t_ix_ ix = 0; ix < last; ++ix)
      store.ptr[ix] = ((~store.ptr[ix]) & BITS_MASK_);
    last_(store) = ~last_(store);
    return store;
  }

  inline
  t_void assign_(r_store_ store, R_store_ value) noexcept {
    if (store.ensure_size(value.size)) {
      t_ix_ max = get(value.size);
      for (t_ix_ ix = 0; ix < max; ++ix)
        store.ptr[ix] = value.ptr[ix];
    }
  }

  constexpr
  t_void assign_(r_store_ store, t_pvalue_ value) noexcept {
    store.ptr[0] = value & BITS_MASK_;
    store.ptr[1] = value & BITS_MSB_ ? BITS_ONE_ : BITS_ZERO_;
  }

  constexpr
  t_void assign_(r_store_ store, t_nvalue_ value) noexcept {
    store.ptr[0] = value & BITS_MASK_;
    store.ptr[1] = value & BITS_SIGN_ ? BITS_ALL_ : BITS_ZERO_;
  }

  /*
  #define HIGHEST_BIT(value)   ((value) & (1 << ((sizeof(value)*8) - 1)))

  #define IS_NEG(value)    (value < 0)
  #define TWOS_COMP(value) (-value)

  t_binary_base multiply_(t_binary_base i, t_binary_base j) {
    t_binary_base k;
    if (i && j) {
      int sign = 0;
      if (i.is_negative()) {
        sign = 1;
        i.twos_complement();
      }
      if (j.is_negative()) {
        sign += 3;
        j.twos_complement();
      }
      for (;j;j>>=1,i<<=1)
        if (j & 1)
          k += i;
      if (sign & 1)
        k.twos_complement();
    }
    return k;
  }

  // positive
  t_binary multiply_(t_binary_base i, t_binary_base j) {
    t_binary k;
    if (i && j) {
      for (;j;j>>=1,i<<=1)
        if (j & 1)
          k += i;
    }
    return k;
  }

  int multiply_(int i, int j) {
    int k = 0;
    if (i && j) {
      int sign = 0;
      if (IS_NEG(i)) {
        sign = 1;
        i = TWOS_COMP(i);
      }
      if (IS_NEG(j)) {
        sign += 3;
        j = TWOS_COMP(j);
      }
      for (;j;j>>=1,i<<=1)
        if (j & 1)
          k += i;
      if (sign & 1)
        k = TWOS_COMP(k);
    }
    return k;
  }

  int divide_(int i, int j) { // works for ints not for unsigned
    int l = 0;
    if (i) {
      int sign = 0;
      if (IS_NEG(i)) {
        sign = 1;
        i = TWOS_COMP(i);
      }
      if (IS_NEG(j)) {
        sign += 3;
        j = TWOS_COMP(j);
      }
      int k = j;
      for (; !HIGHEST_BIT(k) && k < i; k <<= 1); // NOT MAX PROOF
      for (k >>= 1; k >= j; k >>= 1) {
        l <<= 1; // DONT understand this
        if (k < i) {
          i += TWOS_COMP(k);
          l += 1;
        }
      }
      if (j == i)
        l += 1;
      if (sign & 1)
        l = TWOS_COMP(l);
    }
    return l;
  }
  */

  t_void add_(r_store_ store, R_store_ value) {
    // XXX_1_
    // must know if negative
    int last = get(store.size) - 1;
    t_pvalue_ carry = 0;
    for (int ix = 0; ix < last; ++ix) {
      store.ptr[ix] += value.ptr[ix] + carry;
      if (BITS_MSB_ & store.ptr[ix]) {
        store.ptr[ix] = store.ptr[ix] & BITS_MASK_;
        carry = 1;
      }
    }
  }

  t_void add_(r_store_ store, t_pvalue_ value) {
    // XXX_2_
  }

  t_void add_(r_store_ store, t_nvalue_ value) {
    // XXX_3_
  }

  inline
  r_store_ twos_complement_(r_store_ store) noexcept {
    ones_complement_(store);
    add_(store, 1ULL); // XXX_4_ expensive
    return store;
  }

  /////////////////////////////////////////////////////////////////////////////

  inline
  t_impl_base_::t_impl_base_(r_store_ store) noexcept { // BIN_METHOD_1_
  }

  inline
  t_impl_base_::t_impl_base_(r_store_ store, t_nvalue_ value) noexcept { // BIN_METHOD_2_
    assign_(store, value);
  }

  inline
  t_impl_base_::t_impl_base_(r_store_ store, t_pvalue_ value) noexcept { // BIN_METHOD_3_
    assign_(store, value);
  }

  inline
  t_impl_base_::t_impl_base_(r_store_ store, R_store_ value) noexcept { // BIN_METHOD_4_
    assign_(store, value);
  }

  inline
  t_void t_impl_base_::assign(r_store_ store, t_nvalue_ value) noexcept { // BIN_METHOD_5_
    assign_(store, value);
  }

  inline
  t_void t_impl_base_::assign(r_store_ store, t_pvalue_ value) noexcept { // BIN_METHOD_6_
    assign_(store, value);
  }

  inline
  t_void t_impl_base_::assign(r_store_ store, R_store_ value) noexcept { // BIN_METHOD_7_
    assign_(store, value);
  }

  inline
  t_void t_impl_base_::addition(r_store_ store, t_nvalue_ value) noexcept { // BIN_METHOD_8_
    add_(store, value);
  }

  inline
  t_void t_impl_base_::addition(r_store_ store, t_pvalue_ value) noexcept { // BIN_METHOD_9_
    add_(store, value);
  }

  inline
  t_void t_impl_base_::addition(r_store_ store, R_store_ value) noexcept { // BIN_METHOD_10_
    add_(store, value);
  }

  inline
  t_void t_impl_base_::subtraction(r_store_ store, t_nvalue_ value) noexcept { // BIN_METHOD_11_
    add_(store, -value); // XXX_5_ no sure
  }

  inline
  t_void t_impl_base_::subtraction(r_store_ store, t_pvalue_ value) noexcept { // BIN_METHOD_12_
    add_(store, -value); // XXX_6_ no sure
  }

  inline
  t_void t_impl_base_::subtraction(r_store_ store, R_store_ value) noexcept { // BIN_METHOD_13_
    //add_(store, twos_complement_(value));
    // XXX_7_
  }

  inline
  t_void t_impl_base_::multiplication(r_store_, t_nvalue_) noexcept { // BIN_METHOD_14_
    // XXX_8_
  }

  inline
  t_void t_impl_base_::multiplication(r_store_, t_pvalue_) noexcept { // BIN_METHOD_15_
    // XXX_9_
  }

  inline
  t_void t_impl_base_::multiplication(r_store_, R_store_) noexcept { // BIN_METHOD_16_
    // XXX_10_
  }

  inline
  t_void t_impl_base_::division(r_store_, t_nvalue_) noexcept { // BIN_METHOD_17_
    // XXX_11_
  }

  inline
  t_void t_impl_base_::division(r_store_, t_pvalue_) noexcept { // BIN_METHOD_18_
    // XXX_12_
  }

  inline
  t_void t_impl_base_::division(r_store_, R_store_) noexcept { // BIN_METHOD_19_
    // XXX_13_
  }

  inline
  t_void t_impl_base_::binary_and(r_store_ store, t_pvalue_ value) noexcept { // BIN_METHOD_21_
    // XXX_15_
    store.ptr[0] &= value; // XXX_14_ - 64th bit can be in ptr[1]
    if (value >= 0) {
      t_ix_ max = get(store.size);
      for (t_ix_ ix = 1; ix < max; ++ix)
        store.ptr[ix] = BITS_ZERO_;
    } else {
      t_ix_ last = get(store.size) - 1;
      for (t_ix_ ix = 1; ix < last; ++ix)
        store.ptr[ix] = BITS_MASK_;
      store.ptr[last] = BITS_ALL_;
    }
  }

  inline
  t_void t_impl_base_::binary_and(r_store_, R_store_) noexcept { // BIN_METHOD_22_
    // XXX_16_
  }

  inline
  t_void t_impl_base_::binary_or(r_store_, t_pvalue_) noexcept { // BIN_METHOD_24_
    // XXX_18_
  }

  inline
  t_void t_impl_base_::binary_or(r_store_, R_store_)  noexcept { // BIN_METHOD_25_
    // XXX_19_
  }

  inline
  t_void t_impl_base_::binary_xor(r_store_, t_pvalue_) noexcept { // BIN_METHOD_27_
    // XXX_21_
  }

  inline
  t_void t_impl_base_::binary_xor(r_store_, R_store_) noexcept { // BIN_METHOD_28_
    // XXX_22_
  }

  inline
  t_void t_impl_base_::ones_complement(r_store_ store) noexcept { // BIN_METHOD_29_
    ones_complement(store);
  }

  inline
  t_void t_impl_base_::twos_complement(r_store_ store) noexcept { // BIN_METHOD_30_
    twos_complement(store);
  }

  inline
  t_void t_impl_base_::shift_left(r_store_ store, t_n n) noexcept { // BIN_METHOD_31_
    // will not enlarge
    // XXX_23_
  }

  inline
  t_void t_impl_base_::shift_right(r_store_ store, t_n n) noexcept { // BIN_METHOD_32_
    // XXX_24_
    // calculate how many indexes will be removed
    // calculate how many bit will be removed
    // check first bit of last
    // if bit is set then it must be carried
    // then bit shiff last to right
    // check if first bit set, if yes remember
    // shift to right
    // if the previous had a zero bit on, then add it to 62.
  }

  inline
  t_void t_impl_base_::set_bit(r_store_ store, t_ix ix, t_bool on) noexcept { // BIN_METHOD_33_
    t_ix_ bit = get(ix);
    t_ix_ max = get_bits_(store);
    if (bit < max) {
      if (bit != max - 1) {
        t_ix_ ix = bit/BITS_UNIT_;
        if (on)
          store.ptr[ix] |=  (1ULL << bit%BITS_UNIT_);
        else
          store.ptr[ix] &= ~(1ULL << bit%BITS_UNIT_);
      } else {
        if (on)
          last_(store) |=  BITS_MSB_;
        else
          last_(store) &= ~BITS_MSB_;
      }
    }
  }

  inline
  t_n t_impl_base_::get_bits(R_store_ store) const noexcept { // BIN_METHOD_34_
    return t_n{get_bits_(store)};
  }

  inline
  t_bool t_impl_base_::get_bit(R_store_ store, t_ix ix) const noexcept { // BIN_METHOD_35_
    t_ix_ bit = get(ix);
    t_ix_ max = get_bits_(store);
    if (bit < max) {
      if (bit != max - 1) {
        t_ix_ ix = bit/BITS_UNIT_;
        return (1ULL << bit%BITS_UNIT_) & store.ptr[ix];
      }
      return last_(store) & BITS_MSB_;
    }
    return false;
  }

  inline
  t_bool t_impl_base_::is_negative(R_store_ store) const noexcept { // BIN_METHOD_36_
    return is_negative_(store);
  }

  inline
  t_void t_impl_base_::reset(r_store_, t_nvalue_) noexcept { // BIN_METHOD_37_
    // XXX_26_
  }

  inline
  t_void t_impl_base_::reset(r_store_, t_n, t_nvalue_) noexcept { // BIN_METHOD_38_
    // XXX_27_
  }

  inline
  t_void t_impl_base_::reset(r_store_, t_pvalue_) noexcept { // BIN_METHOD_39_
    // XXX_28_
  }

  inline
  t_void t_impl_base_::reset(r_store_, t_n, t_pvalue_) noexcept { // BIN_METHOD_40_
    // XXX_29_
  }

  inline
  t_void t_impl_base_::set_bits(r_store_, t_n) noexcept { // BIN_METHOD_41_
    // XXX_30_
  }

  inline
  t_bool t_impl_base_::not_zero(R_store_) const noexcept { // BIN_METHOD_41_
    // XXX_31_
    return false;
  }

  inline
  t_bool t_impl_base_::is_equal(R_store_, R_store_)  const noexcept { // BIN_METHOD_41_
    // XXX_32_
    return false;
  }

  inline
  t_bool t_impl_base_::is_equal(R_store_, t_pvalue_) const noexcept { // BIN_METHOD_42_
    // XXX_33_
    return false;
  }

  inline
  t_bool t_impl_base_::is_equal(R_store_, t_nvalue_) const noexcept { // BIN_METHOD_43_
    // XXX_34_
    return false;
  }

  inline
  t_bool t_impl_base_::is_less(R_store_, R_store_)  const noexcept { // BIN_METHOD_44_
    // XXX_35_
    return false;
  }

  inline
  t_bool t_impl_base_::is_less(R_store_, t_pvalue_) const noexcept { // BIN_METHOD_45_
    // XXX_36_
    return false;
  }

  inline
  t_bool t_impl_base_::is_less(R_store_, t_nvalue_) const noexcept { // BIN_METHOD_46_
    // XXX_37_
    return false;
  }

  inline
  t_bool t_impl_base_::is_less_equal(R_store_, R_store_)  const noexcept { // BIN_METHOD_47_
    // XXX_38_
    return false;
  }

  inline
  t_bool t_impl_base_::is_less_equal(R_store_, t_pvalue_) const noexcept { // BIN_METHOD_48_
    // XXX_39_
    return false;
  }

  inline
  t_bool t_impl_base_::is_less_equal(R_store_, t_nvalue_) const noexcept { // BIN_METHOD_49_
    // XXX_40_
    return false;
  }

///////////////////////////////////////////////////////////////////////////////
}
}
}
}

#endif
