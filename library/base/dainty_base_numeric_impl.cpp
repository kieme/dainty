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
#include "dainty_base_numeric_impl.h"

namespace dainty
{
namespace base
{
namespace numeric
{
namespace impl_
{
  // XXX_RULE_1_ = total bits are always uneven. e.g. 64 + (63*size)
  //               positive integers cannot use the last zero bit.
  // XXX_RULE_2_ = left shift will not result in a negative number

  // XXX_TODO - get BIN_FUNC_ numbering right

  /////////////////////////////////////////////////////////////////////////////

  using types::t_double;
  using specific::mk;

  /////////////////////////////////////////////////////////////////////////////

  t_void copy_(p_pvalue_ dst, P_pvalue_ src, t_n n) noexcept {
    P_pvalue_ end = src + get(n); // memcpy
    while (src != end)
      *dst++ = *src++;
  }

  t_void clear_(p_pvalue_ dst, t_n n) noexcept {
    P_pvalue_ end = dst + get(n); // memset
    while (dst != end)
      *dst++ = 0;
  }

  /////////////////////////////////////////////////////////////////////////////

  t_void display_bits_(t_pvalue_ value, t_bool msb) noexcept {
    if (msb)
      std::cout << ((value & BITS_MSB_) ? '1' : '0');
    value <<= 1;
    for (t_ix_ ix = 0; ix < BITS_UNIT_; ++ix) {
      std::cout << ((value & BITS_MSB_) ? '1' : '0');
      value <<=1;
    }
  }

  t_void display_bits_(t_ix ix, t_pvalue_ value, t_bool msb) noexcept {
    std::cout << "[" << get(ix) << ":";
    display_bits_(value, msb);
    std::cout << "]";
  }

  // IMPL_FUNC_1_1_
  t_void display_bits_(R_store_ value, t_bool index) noexcept {
    std::cout << "0b";
    P_pvalue_ p = value.end() - 1;
    if (!index) {
      display_bits_(*p, true);
      for (--p; p >= value.begin(); --p)
        display_bits_(*p, false);
    } else {
      t_ix ix = last_ix_(value);
      display_bits_(ix, *p, true);
      for (--p; p >= value.begin(); --p) {
        ix = t_ix{get(ix) - 1};
        display_bits_(ix, *p, false);
      }
    }
    std::cout << std::endl;
  }

  /////////////////////////////////////////////////////////////////////////////

  // IMPL_FUNC_1_2_
  t_binfo_ bits_info_(R_store_ value) noexcept {
    t_n   max_bits = max_bits_(value);
    t_ix  last_ix  = last_ix_(value);
    t_ix_ ix       = get(last_ix);
    for (; ix && value.ptr[ix] == BITS_ZERO_; --ix);

    if (!ix && value.ptr[0] == BITS_ZERO_)
      return {max_bits};

    t_pvalue_ val = value.ptr[ix];
    t_n_ p = 0;
    t_n_ n = 0;
    for (; val; val>>=1, ++p)
      if (val & 1)
        ++n;

    if (ix) {
      do {
        --ix;
        t_pvalue_ val = value.ptr[ix];
        for (; val; val>>=1)
          if (val & 1)
            ++n;
      } while (ix);
    }
    return {{last_ix, t_n{p}}, t_n{n}, max_bits};
  }

  // IMPL_FUNC_1_3_
  t_n free_bits_(R_store_ value) noexcept {
    if (!is_neg_(value)) {
      auto max = max_bits_(value);
      auto ix  = msb_on_bit_(value);
      return max - bits_(ix);
    }
    return 0_n;
  }

  // IMPL_FUNC_1_4_
  t_bpos_ msb_on_bit_(R_store_ value) noexcept {
    t_ix_ ix  = get(last_ix_(value));
    for (; ix && value.ptr[ix] == BITS_ZERO_; --ix);

    if (!ix && value.ptr[0] == BITS_ZERO_)
      return {};

    t_pvalue_ val = value.ptr[ix];
    t_n_ p = 0;
    for (; val; val>>=1)
      ++p;

    return {t_ix{ix}, t_n{p}};
  }

  // IMPL_FUNC_1_5_
  t_bpos_ msb_off_bit_(R_store_ value) noexcept {
    t_ix_ ix  = get(last_ix_(value));
    for (; ix && value.ptr[ix] == BITS_ALL_; --ix);

    if (!ix && value.ptr[0] == BITS_ALL_)
      return {};

    t_pvalue_ val = value.ptr[ix];
    t_n_ p = BITS_UNIT_;
    for (t_pvalue_ mask = BITS_LAST_; mask && (mask & val); mask>>=1)
      --p;

    return {t_ix{ix}, t_n{p}};
  }

  // IMPL_FUNC_1_6_
  t_void ones_compl_(r_store_ value) noexcept {
    const auto last = get(last_ix_(value.size));
    for (t_ix_ ix = 0; ix < last; ++ix) {
      r_pvalue_ entry = value.ptr[ix];
      entry = ~entry & BITS_MASK_;
    }
    r_pvalue_ entry = last_(value);
    entry = ~entry;
  }

  // IMPL_FUNC_1_7_
  t_void twos_compl_(r_store_ value) noexcept {
    auto last  = get(last_ix_(value.size));
    auto carry = BITS_ONE_;
    for (auto ix = get(0_ix); ix < last; ++ix) {
      r_pvalue_ entry = value.ptr[ix];
      entry = (~entry & BITS_MASK_) + carry;
      carry = entry >> BITS_UNIT_;
      entry &= BITS_MASK_;
    }
    r_pvalue_ entry = last_(value);
    entry = ~entry + carry;
  }

  // IMPL_FUNC_1_8_
  t_bool assign_(r_store_ store, R_store_ value) noexcept {
    if (store.ensure(value.size)) {
      t_ix_ max = get(value.size);
      t_ix_ ix  = 0;
      for (; ix < max; ++ix)
        store.ptr[ix] = value.ptr[ix];
      t_bool neg = is_neg_(value);
      max = get(store.size);
      for (; ix < max; ++ix)
        store.ptr[ix] = neg ? BITS_ALL_ : BITS_ZERO_;
      return true;
    }
    return false;
  }

  /////////////////////////////////////////////////////////////////////////////

  struct t_add_ctxt_ {
    t_ix_     last[2];
    t_bool    neg[2];
    t_n_      min      = 0;
    t_n_      max      = 0;
    t_pvalue_ add_loop = BITS_ZERO_;
    t_pvalue_ add_last = BITS_ZERO_;
    t_pvalue_ add_end  = BITS_ZERO_;
    P_pvalue_ longer   = nullptr;
    P_pvalue_ shorter  = nullptr;
  };
  using r_add_ctxt_ = types::t_prefix<t_add_ctxt_>::r_;

  inline
  t_add_ctxt_ add_ctxt_(R_store_ value, R_store_ value1) noexcept {
    t_add_ctxt_ ctxt;
    ctxt.last[0] = get(last_ix_(value));
    ctxt.last[1] = get(last_ix_(value1));
    ctxt.neg[0]  = is_neg_(value);
    ctxt.neg[1]  = is_neg_(value1);
    if (ctxt.last[0] == ctxt.last[1]) {
      ctxt.min = ctxt.last[0];
    } else if (ctxt.last[0] < ctxt.last[1]) {
      ctxt.min     = ctxt.last[0];
      ctxt.max     = ctxt.last[1];
      ctxt.longer  = value1.begin();
      ctxt.shorter = value.begin();
      if (ctxt.neg[0]) {
        ctxt.add_loop = BITS_MASK_;
        ctxt.add_last = BITS_ALL_;
      } else {
        ctxt.add_loop = BITS_ZERO_;
        ctxt.add_last = BITS_ZERO_;
      }
    } else {
      ctxt.min     = ctxt.last[1];
      ctxt.max     = ctxt.last[0];
      ctxt.longer  = value.begin();
      ctxt.shorter = value1.begin();
      if (ctxt.neg[1]) {
        ctxt.add_loop = BITS_MASK_;
        ctxt.add_last = BITS_ALL_;
      } else {
        ctxt.add_loop = BITS_ZERO_;
        ctxt.add_last = BITS_ZERO_;
      }
    }
    return ctxt;
  }

  t_n calc_add_(r_add_ctxt_ ctxt, R_store_ value, R_store_ value1) noexcept {
    auto carry = BITS_ZERO_;
    auto ix    = get(0_ix);
    auto tmp   = BITS_ZERO_;
    for (; ix < ctxt.min; ++ix) {
      tmp   = value.ptr[ix] + value1.ptr[ix] + carry;
      carry = tmp >> BITS_UNIT_;
    }
    if (ctxt.last[0] == ctxt.last[1])
      tmp = value.ptr[ix] + value1.ptr[ix] + carry;
    else {
      tmp = (ctxt.shorter[ix] & BITS_MASK_) + ctxt.longer[ix] + carry;
      carry = tmp >> BITS_UNIT_;
      for (++ix; ix < ctxt.max; ++ix) {
        tmp = ctxt.longer[ix] + ctxt.add_loop + carry;
        carry = tmp >> BITS_UNIT_;
      }
      tmp = ctxt.longer[ix] + ctxt.add_last + carry;
    }
    if (ctxt.neg[0] == ctxt.neg[1]) {
      if (ctxt.neg[0]) {
        if (!(tmp >> BITS_UNIT_)) {
          ++ix;
          ctxt.add_end  = ~BITS_ONE_;
        }
      } else if (tmp >> BITS_UNIT_) {
        ++ix;
        ctxt.add_end  = BITS_ONE_;
      }
    }
    return t_n{ix + 1};
  }

  // IMPL_FUNC_1_9_
  t_bool add_(r_store_ store, R_store_ value) noexcept {
    t_add_ctxt_ ctxt = add_ctxt_(store, value);
    if (store.ensure(calc_add_(ctxt, store, value))) {
      t_ix_ ix        = 0;
      t_pvalue_ carry = BITS_ZERO_;
      for (; ix < ctxt.min; ++ix) {
        store.ptr[ix] += value.ptr[ix] + carry;
        carry = store.ptr[ix] >> BITS_UNIT_;
        store.ptr[ix] &= BITS_MASK_;
      }
      if (ctxt.last[0] == ctxt.last[1])
        store.ptr[ix] += value.ptr[ix] + carry;
      else {
        store.ptr[ix] = (ctxt.shorter[ix] & BITS_MASK_) + ctxt.longer[ix] + carry;
        carry = store.ptr[ix] >> BITS_UNIT_;
        store.ptr[ix] &= BITS_MASK_;
        for (++ix; ix < ctxt.max; ++ix) {
          store.ptr[ix] = ctxt.longer[ix] + ctxt.add_loop + carry;
          carry = store.ptr[ix] >> BITS_UNIT_;
          store.ptr[ix] &= BITS_MASK_;
        }
        store.ptr[ix] = ctxt.longer[ix] + ctxt.add_last + carry;
      }
      if (ctxt.add_end) {
        store.ptr[ix++] &= BITS_MASK_;
        store.ptr[ix]    = ctxt.add_end;
      }
      return true;
    }
    return false;
  }

  /////////////////////////////////////////////////////////////////////////////

  // IMPL_FUNC_1_10_
  t_bool minus_(r_store_ store, R_store_ value) noexcept {
    t_store_ tmp{value};
    twos_compl_(tmp);
    return add_(store, tmp);
  }

  // IMPL_FUNC_1_11_
  t_bool multiply_(r_store_ store, R_store_ value) noexcept {
    if (!is_zero_(store) && !is_zero_(value)) {
      t_store_ a{x_cast(store)};
      t_store_ b{value};
      assign_(store, BITS_ZERO_);

      t_n_ sign = 0;
      if (is_neg_(a)) {
        sign = 1;
        twos_compl_(a);
      }

      if (is_neg_(b)) {
        sign += 3;
        twos_compl_(b);
      }

      for (; !is_zero_(a); shift_left_(b), shift_right_(a))
        if (lsb_(a))
          add_(store, b);

      if (sign & 1)
        twos_compl_(store);
    } else
      assign_(store, BITS_ZERO_);
    return true;
  }

  // IMPL_FUNC_1_12_
  t_bool divide_(r_store_ store, R_store_ value) noexcept {
    if (!is_zero_(store)) {
      t_store_ var1{x_cast(store)};
      t_store_ var2{value};
      assign_(store, BITS_ZERO_);

      t_n_ sign = 0;
      if (is_neg_(var1)) {
        sign = 1;
        twos_compl_(var1);
      }

      if (is_neg_(var2)) {
        sign += 3;
        twos_compl_(var2);
      }

      t_store_ var3{var2};
      for (; is_less_(var3, var1); shift_left_(var3));
      for (shift_right_(var3); is_less_equal_(var2, var3); shift_right_(var3)) {
        shift_right_(store); // XXX_FIXME_X_
        if (is_less_(var3, var1)) {
          minus_(var1, var3);
          add_(store, BITS_ONE_);
        }
      }

      if (is_equal_(var1, var3))
        add_(store, BITS_ONE_);
      if (sign & 1)
        twos_compl_(store);
    }
    return true;
  }

  // IMPL_FUNC_1_13_
  t_bool and_(r_store_ store, R_store_ value) noexcept {
    auto ix = get(0_ix), min = get(min_of(store.size, value.size));
    for (; ix < min; ++ix)
      store.ptr[ix] &= value.ptr[ix];
    if (get(store.size) > get(value.size)) {
      for (t_ix_ end = get(store.size); ix < end; ++ix)
        store.ptr[ix] = 0;
    } else {
      if (store.ensure(value.size)) {
        for (t_ix_ end = get(store.size); ix < end; ++ix)
          store.ptr[ix] = 0;
      } else
        return false;
    }
    return true;
  }

  // IMPL_FUNC_1_14_
  t_bool or_(r_store_ store, R_store_ value) noexcept {
    auto ix = get(0_ix), min = get(min_of(store.size, value.size));
    for (; ix < min; ++ix)
      store.ptr[ix] |= value.ptr[ix];
    if (get(store.size) < get(value.size)) {
      if (store.ensure(value.size)) {
        for (t_ix_ end = get(store.size); ix < end; ++ix)
          store.ptr[ix] |= value.ptr[ix];
      } else
        return false;
    }
    return true;
  }

  // IMPL_FUNC_1_15_
  t_bool xor_(r_store_ store, R_store_ value) noexcept {
    auto ix = get(0_ix), min = get(min_of(store.size, value.size));
    for (; ix < min; ++ix)
      store.ptr[ix] ^= value.ptr[ix];
    if (get(store.size) < get(value.size)) {
      if (store.ensure(value.size)) {
        for (t_ix_ end = get(store.size); ix < end; ++ix)
          store.ptr[ix] |= value.ptr[ix];
      } else
        return false;
    }
    return true;
  }

  // IMPL_FUNC_1_16_
  t_bool shift_left_(r_store_ store, t_n n) noexcept {
    auto bpos = msb_on_bit_(store);
    if (bpos && get(n) > 0) {
      auto need = calc_size_(bits_(bpos) + n);
      if (store.ensure(need)) {
        auto k     = get(n) % BITS_UNIT_;
        auto z     = BITS_UNIT_ - k;
        auto ix    = get(bpos.ix);
        auto to_ix = get(need) - 1;
        if (to_ix) {
          store.ptr[to_ix] = store.ptr[ix] >> z;
          if (store.ptr[to_ix])
            --to_ix;
          for (; ix; --ix)
            store.ptr[to_ix--] =
              BITS_MASK_ & (store.ptr[ix] << k | store.ptr[ix-1] >> z);
        }
        store.ptr[to_ix] = store.ptr[0] << k;
        for (ix = 0; ix < to_ix; ++ix)
          store.ptr[ix] = BITS_ZERO_;
        return true;
      }
    }
    return false;
  }

  // IMPL_FUNC_1_17_
  t_bool shift_right_(r_store_ store, t_n n) noexcept {
    if (!is_zero_(store) && (get(n) > 0)) {
      auto last_ix = get(last_ix_(store));
      auto k       = get(n) % BITS_UNIT_;
      auto z       = BITS_UNIT_ - k;
      auto ix      = get(calc_size_(n)) - 1;
      auto to_ix   = get(0_ix);
      for (; ix < last_ix; ++ix)
        store.ptr[to_ix++] =
          BITS_MASK_ & (store.ptr[ix] >> k | store.ptr[ix+1] << z);
      store.ptr[to_ix++] = store.ptr[ix] >> (k ? k : BITS_UNIT_);
      for (; to_ix <= last_ix; ++to_ix)
        store.ptr[to_ix] = BITS_ZERO_;
      return true;
    }
    return false;
  }

  // IMPL_FUNC_1_18_
  t_bool set_bit_(r_store_ store, t_ix ix, t_bool on) noexcept {
    auto bit = get(ix);
    auto max = get(max_bits_(store));
    if (bit < max) {
      if (bit != max - 1) {
        t_ix_ ix = bit/BITS_UNIT_;
        if (on)
          store.ptr[ix] |=  (BITS_ONE_ << bit%BITS_UNIT_);
        else
          store.ptr[ix] &= ~(BITS_ONE_ << bit%BITS_UNIT_);
      } else {
        if (on)
          last_(store) |=  BITS_MSB_;
        else
          last_(store) &= ~BITS_MSB_;
      }
      return true;
    }
    return false;
  }

  // IMPL_FUNC_1_19_
  t_bool get_bit_(R_store_ store, t_ix ix) noexcept {
    t_ix_ bit = get(ix);
    t_ix_ max = get(max_bits_(store));
    if (bit < max) {
      if (bit != max - 1) {
        t_ix_ ix = bit/BITS_UNIT_;
        return (BITS_ONE_ << bit%BITS_UNIT_) & store.ptr[ix];
      }
      return last_(store) & BITS_MSB_;
    }
    return false;
  }

  // IMPL_FUNC_1_20_
  t_bool is_zero_(R_store_ store) noexcept {
    t_ix_ max = get(store.size);
    for (t_ix_ ix = 0; ix < max; ++ix) {
      r_pvalue_ entry = store.ptr[ix];
      if (entry)
        return false;
    }
    return true;
  }

  // IMPL_FUNC_1_21_
  t_bool reset_(r_store_ store, R_store_ value) noexcept {
    if (store.reset(value.size)) {
      assign_(store, value); // XXX_FIXME_X_ need a truncate assign
      return true;
    }
    return false;
  }

  // IMPL_FUNC_1_22_
  t_bool reset_(r_store_ store, t_n n, R_store_ value) noexcept {
    if (store.reset(n)) {
      assign_(store, value); // XXX_FIXME_X_ need a truncate assign
      return true;
    }
    return false;
  }

  // IMPL_FUNC_1_23_
  t_bool is_equal_(R_store_ store, R_store_ value) noexcept {
    t_bool negs[2] = { is_neg_(store), is_neg_(value)};
    if (negs[0] == negs[1]) {
      t_bpos_ n;
      t_bpos_ n1;
      if (negs[0]) {
        n  = msb_off_bit_(store);
        n1 = msb_off_bit_(value);
      } else {
        n  = msb_on_bit_(store);
        n1 = msb_on_bit_(value);
      }
      if (!n && !n)
        return true;
      if (n == n1) {
        t_ix_ ix = get(n.ix);
        for (; ix && store.ptr[ix] == value.ptr[ix]; --ix);
        return (!ix && store.ptr[0] == value.ptr[0]);
      }
    }
    return false;
  }

  // IMPL_FUNC_1_24_
  t_bool is_less_(R_store_ store, R_store_ value) noexcept {
    t_bool negs[2] = { is_neg_(store), is_neg_(value)};
    if (negs[0] == negs[1]) {
      t_bpos_ n;
      t_bpos_ n1;
      if (negs[0]) {
        n  = msb_off_bit_(store);
        n1 = msb_off_bit_(value);
        if (n) {
          if (n1) {
            if (n > n1)
              return true;
            if (n < n1)
              return false;
            t_ix_ ix = get(n.ix);
            for (; ix && store.ptr[ix] == value.ptr[ix]; --ix);
            return store.ptr[ix] < value.ptr[ix];
          } else
            return true;
        } else
          return false;
      } else {
        n  = msb_on_bit_(store);
        n1 = msb_on_bit_(value);
        if (n) {
          if (n1) {
            if (n > n1)
              return false;
            if (n < n1)
              return true;
            t_ix_ ix = get(n.ix);
            for (; ix && store.ptr[ix] == value.ptr[ix]; --ix);
            return store.ptr[ix] < value.ptr[ix];
          } else
            return false;
        } else
          return n1;
      }
    } else if (negs[0] && !negs[1])
      return true;
    return false;
  }

  // IMPL_FUNC_1_25_
  t_bool is_less_equal_(R_store_ store, R_store_ value) noexcept {
    t_bool negs[2] = { is_neg_(store), is_neg_(value)};
    if (negs[0] == negs[1]) {
      t_bpos_ n;
      t_bpos_ n1;
      if (negs[0]) {
        n  = msb_off_bit_(store);
        n1 = msb_off_bit_(value);
        if (n) {
          if (n1) {
            if (n > n1)
              return true;
            if (n < n1)
              return false;
            t_ix_ ix = get(n.ix);
            for (; ix && store.ptr[ix] == value.ptr[ix]; --ix);
            return store.ptr[ix] <= value.ptr[ix];
          } else
            return true;
        } else
          return !n;
      } else {
        n  = msb_on_bit_(store);
        n1 = msb_on_bit_(value);
        if (n) {
          if (n1) {
            if (n > n1)
              return false;
            if (n < n1)
              return true;
            t_ix_ ix = get(n.ix);
            for (; ix && store.ptr[ix] == value.ptr[ix]; --ix);
            return store.ptr[ix] <= value.ptr[ix];
          } else
            return false;
        } else
          return n1;
      }
    } else if (negs[0] && !negs[1])
      return true;
    return false;
  }

  // IMPL_FUNC_1_26_
  t_n calc_bits_(t_n digits) noexcept {
    t_double bits = get(digits) / 0.301029995;
    return t_n{static_cast<t_n_>(bits) + 1};
  }

  // IMPL_FUNC_1_27_
  t_n calc_digits_(t_n bits) noexcept {
    t_double digits = get(bits) * 0.301029995;
    return t_n{static_cast<t_n_>(digits)};
  }

  // IMPL_FUNC_1_28_
  t_bool ensure_(r_store_ store, t_n n) noexcept {
    if (get(n) > get(store.size)) {
      t_bool neg = is_neg_(store);
      t_n_   old = get(store.size);
      if (store.ensure(n)) {
        for (t_ix_ ix = old; ix < get(n); ++ix)
          store.ptr[ix] = !neg ? BITS_ZERO_ : BITS_ALL_;
      } else
        return false;
    }
    return true;
  }

  // IMPL_FUNC_1_29_
  t_bool ensure_(r_store_ store, R_store_ value) noexcept {
    if (store.ensure(value))
      assign_(store, value);
    else
      return false;
    return true;
  }

  // IMPL_FUNC_1_30_
  t_bool ensure_(r_store_ store, t_n n, R_store_ value) noexcept {
    if (store.ensure(n, value))
      assign_(store, value);
    else
      return false;
    return true;
  }

  ///////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////

  template<typename T>
  inline
  T* malloc_(t_n n) noexcept {
    return static_cast<T*>(::malloc(sizeof(T) * get(n)));
  }

  template<typename T>
  inline
  T* realloc_(T* p, t_n n) noexcept {
    return static_cast<T*>(::realloc(p, sizeof(T) * get(n)));
  }

  template<typename T>
  inline
  t_void free_(T* ptr) noexcept {
    ::free(ptr);
  }

  ///////////////////////////////////////////////////////////////////////////

  t_buf_::t_buf_(t_n n) noexcept {
    ensure(n);
  }

  t_buf_::t_buf_(R_buf_ value) noexcept {
    ensure(value);
  }

  t_buf_::t_buf_(t_n n, R_buf_ value) noexcept {
    ensure(n, value);
  }

  t_buf_::t_buf_(x_buf_ value) noexcept {
    ensure(x_cast(value));
  }

  t_buf_::t_buf_(t_n n, x_buf_ value) noexcept {
    ensure(n, x_cast(value));
  }

  t_buf_::~t_buf_() noexcept {
    if (ptr && ptr != sso_)
      free_(ptr);
  }

  ///////////////////////////////////////////////////////////////////////////

  r_buf_ t_buf_::operator=(R_buf_ value) noexcept {
    ensure(value);
    return *this;
  }

  r_buf_ t_buf_::operator=(x_buf_ value) noexcept {
    ensure(x_cast(value));
    return *this;
  }


  t_void t_buf_::steal(r_buf_ value) noexcept {
    reset();
    ptr  = base::reset(value.ptr);
    size = base::reset(value.size);
  }

  ///////////////////////////////////////////////////////////////////////////

  t_bool t_buf_::ensure(t_n need) noexcept {
    if (size < need) {
      t_ix ix = mk<t_ix>(size);
      if (is_heap())
        ptr = realloc_(ptr, need);
      else {
        ptr = malloc_<t_pvalue_>(need);
        ptr[0] = sso_[0];
        ptr[1] = sso_[1];
      }
      size = need;
      clear(ix);
    }
    return true;
  }

  t_bool t_buf_::ensure(R_buf_ value) noexcept {
    if (size < value.size) {
      if (is_heap())
        ptr = realloc_(ptr, value.size);
      else
        ptr = malloc_<t_pvalue_>(value.size);
      size = value.size;
    } else
      clear(mk<t_ix>(value.size));
    copy_(ptr, value.ptr, value.size);
    return true;
  }

  t_bool t_buf_::ensure(t_n need, R_buf_ value) noexcept {
    need = max_of(need, value.size);
    if (size < need) {
      if (is_heap()) {
        ptr = realloc_(ptr, need);
      } else
        ptr = malloc_<t_pvalue_>(need);
      size = need;
    }
    copy_(ptr, value.ptr, value.size);
    clear(mk<t_ix>(value.size));
    return true;
  }

  t_bool t_buf_::ensure(x_buf_ value) noexcept {
    if (!value.is_heap() || get(value.size) < get(size))
      return ensure(value);
    steal(value);
    return true;
  }

  t_bool t_buf_::ensure(t_n need, x_buf_ value) noexcept {
    if (!value.is_heap() || get(size) < get(need) ||
                            get(value.size) < get(need))
      return ensure(need, value);
    steal(value);
    return true;
  }

  ///////////////////////////////////////////////////////////////////////////

  t_void t_buf_::reset() noexcept {
    if (is_heap()) {
      sso_[0] = ptr[0];
      sso_[1] = ptr[1];
      free_(ptr);
      ptr  = sso_;
      size = t_n{2};
    }
  }

  t_bool t_buf_::reset(t_n need) noexcept {
    if (need != size) {
      if (is_heap()) {
        if (get(need) <= t_n{2}) {
          size = t_n{2};
          sso_[0] = ptr[0];
          sso_[1] = ptr[1];
          free_(ptr);
          ptr     = sso_;
        } else {
          ptr = realloc_(ptr, need);
          t_ix ix = mk<t_ix>(size);
          size = need;
          if (size < need)
            clear(ix);
        }
      } else {
        if (2 < get(need)) { // XXX_REVISIT_5_
          size = need;
          ptr = malloc_<t_pvalue_>(need);
          ptr[0] = sso_[0];
          ptr[1] = sso_[1];
          clear(2_ix);
        }
      }
    }
    return true;
  }

  t_bool t_buf_::reset(R_buf_ value) noexcept {
    if (reset(value.size)) {
      copy_(ptr, value.ptr, value.size);
      return true;
    }
    return false;
  }

  t_bool t_buf_::reset(t_n need, R_buf_ value) noexcept {
    if (reset(need)) {
      copy_(ptr, value.ptr, min_of(value.size, need));
      return true;
    }
    return false;
  }

  t_bool t_buf_::reset(x_buf_ value) noexcept {
    if (!value.is_heap())
      return reset(value);
    steal(value);
    return true;
  }

  t_bool t_buf_::reset(t_n need, x_buf_ value) noexcept {
    if (get(need) == get(value.size))
      return reset(x_cast(value));
    return reset(need, value);
  }

  ///////////////////////////////////////////////////////////////////////////

  t_bool t_buf_::is_valid() const noexcept {
    return ptr;
  }

  t_bool t_buf_::is_heap() const noexcept {
    return ptr != sso_;
  }

  ///////////////////////////////////////////////////////////////////////////

  p_pvalue_ t_buf_::begin() noexcept {
    return ptr;
  }

  P_pvalue_ t_buf_::begin() const noexcept {
    return ptr;
  }

  p_pvalue_ t_buf_::end() noexcept {
    return ptr + get(size);
  }

  P_pvalue_ t_buf_::end() const noexcept {
    return ptr + get(size);
  }

  ///////////////////////////////////////////////////////////////////////////

  t_bool t_buf_::clear(t_ix pos) noexcept {
    t_ix_ ix = get(pos), max = get(size);
    if (ix < max) {
      clear_(ptr + ix, t_n{max - ix});
      return true;
    }
    return false;
  }

  t_bool t_buf_::fill(t_pvalue_ value, t_ix pos) noexcept {
    if (get(pos) < get(size)) {
      t_ix_ max = get(size);
      for (t_ix_ ix = get(pos); ix < max; ++ix)
        ptr[ix] = value;
      return true;
    }
    return false;
  }

  ///////////////////////////////////////////////////////////////////////////
}
}
}
}
