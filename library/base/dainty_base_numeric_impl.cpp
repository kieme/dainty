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

#include "dainty_base_numeric_impl.h"

namespace dainty
{
namespace base
{
namespace numeric
{
namespace impl_
{
  /////////////////////////////////////////////////////////////////////////////

  using types::p_ullong;
  using types::P_ullong;

  // The decision is based on the idea that you don't care if it is negative
  // if you shift a value. You can argue binary doesnt know negative. But I
  // don't see a conflict for binary to know, when it is means to represent
  // negative numbers. You don't need to make it negative!
  //
  // 1. what happens if you shift left  a negative value?
  //  - it becomes a positive number. all bits move one or more left as they are
  // 2. what happens if you shift right a negative value?
  //  . it becomes a positive number, they all shift right.

  /////////////////////////////////////////////////////////////////////////////

  t_void copy_(p_ullong dst, P_ullong src, t_n n) noexcept {
    P_ullong end = src + get(n);
    while (src != end)
      *dst++ = *src++;
  }

  // IMPL_FUNC_1_1_
  r_store_ ones_compl_(r_store_ store) noexcept {
    t_ix_ last = get(store.size) - 1;
    for (t_ix_ ix = 0; ix < last; ++ix) {
      r_pvalue_ entry = store.ptr[ix];
      entry = ~entry & BITS_MASK_;
    }
    r_pvalue_ entry = store.ptr[last];
    entry = ~entry;
    return store;
  }

  // IMPL_FUNC_1_2_
  r_store_ twos_compl_(r_store_ store) noexcept {
    t_ix_ last = get(store.size) - 1;
    t_pvalue_ carry = 1;
    for (t_ix_ ix = 0; ix < last; ++ix) {
      r_pvalue_ entry = store.ptr[ix];
      entry = (~entry & BITS_MASK_) + carry;
      carry = (entry & BITS_MSB_) ? 1 : 0;
      entry &= BITS_MASK_;
    }
    r_pvalue_ entry = store.ptr[last];
    entry = ~entry + carry;
    return store;
  }

  // IMPL_FUNC_1_3_
  t_void assign_(r_store_ store, R_store_ value) noexcept {
    if (store.ensure(value.size)) {
      t_ix_ max = get(value.size);
      t_ix_ ix  = 0;
      for (; ix < max; ++ix)
        store.ptr[ix] = value.ptr[ix];
      t_bool neg = is_negative_(value);
      max = get(store.size);
      for (; ix < max; ++ix)
        store.ptr[ix] = neg ? BITS_ALL_ : BITS_ZERO_;
    }
  }

  // IMPL_FUNC_1_4_
  // XXX-1
  // note: another way might be to < and sign
  t_void add_(r_store_ store, R_store_ value) noexcept {
    t_bool negs[2] = { is_negative_(store), is_negative_(value) };
    if (get(store.size) == get(value.size)) {
      t_ix_ last = get(store.size) - 1;
      t_pvalue_ carry = 0;
      for (t_ix_ ix = 0; ix < last; ++ix) {
        store.ptr[ix] += carry;
        store.ptr[ix] += value.ptr[ix];
        if (BITS_MSB_ & store.ptr[ix]) {
          store.ptr[ix] = store.ptr[ix] & BITS_MASK_;
          carry = 1;
        }
      }
      store.ptr[last] += carry;
      store.ptr[last] += value.ptr[last];
      if (negs[0] == negs[1]) {
        if (negs[0]) {
          if (!(BITS_MSB_ & store.ptr[last])) {
            //must enlarge by one + 1111s
          }
        } else {
          if (BITS_MSB_ & store.ptr[last]) {
            //must enlarge by one + 1
          }
        }
      }
    } else if (get(store.size) > get(value.size)) {
      t_ix_ last = get(value.size) - 1, last1 = get(store.size);
      t_pvalue_ carry = 0;
      for (t_ix_ ix = 0; ix < last; ++ix) {
        store.ptr[ix] += carry; // XXX - not sure this is right 
        store.ptr[ix] += value.ptr[ix];
        if (BITS_MSB_ & store.ptr[ix]) {
          store.ptr[ix] = store.ptr[ix] & BITS_MASK_;
          carry = 1;
        }
      }
      if (negs[1]) {
        for (t_ix_ ix = last; ix < last1; ++ix) {
          store.ptr[ix] += carry;
          store.ptr[ix] += BITS_ALL_;
          if (BITS_MSB_ & store.ptr[ix]) {
            store.ptr[ix] = store.ptr[ix] & BITS_MASK_;
            carry = 1;
          }
        }
        store.ptr[last] += carry;
        store.ptr[last] += BITS_ALL_;
        if (negs[0] && !(BITS_MSB_ & store.ptr[last])) {
            //must enlarge by one + 1111s
        }
      } else {
        if (carry) {
          for (t_ix_ ix = last; ix < last1; ++ix) {
            store.ptr[ix] += carry;
            if (BITS_MSB_ & store.ptr[ix]) {
              store.ptr[ix] = store.ptr[ix] & BITS_MASK_;
              carry = 1;
            }
          }
          store.ptr[last] += carry;
          if (negs[0] && !(BITS_MSB_ & store.ptr[last])) {
              //must enlarge by one + 1111s
          }
        }
      }
    } else {
    }
  }

  // IMPL_FUNC_1_5_
  t_void minus_(r_store_ , R_store_) noexcept {
    // XXX-2
  }

  // IMPL_FUNC_1_6_
  t_void multiply_(r_store_ , R_store_) noexcept {
    // XXX-3
    /*
    t_binary k;
    if (i && j) {
      int sign = 0;
      if (is_negative_(i)) {
        sign = 1;
        twos_complement_(i);
      }
      if (is_negative_(j)) {
        sign += 3;
        twos_complement(j);
      }
      for (;j;j>>=1,i<<=1)
        if (j & 1)
          k += i;
      if (sign & 1)
        twos_complement_(k);
    }
    return k;
    */
  }

  // IMPL_FUNC_1_7_
  t_void divide_(r_store_ , R_store_) noexcept {
    // XXX-4
    /*
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
    */
  }

  // IMPL_FUNC_1_8_
  t_void and_(r_store_, R_store_) noexcept {
    // XXX-5
    /*
    store.ptr[0] &= value; // XXX - 64th bit can be in ptr[1]
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
    */
  }

  // IMPL_FUNC_1_9_
  t_void or_(r_store_, R_store_) noexcept {
    // XXX-6
    /*
    store.ptr[0] &= value; // XXX - 64th bit can be in ptr[1]
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
    */
  }

  // IMPL_FUNC_1_10_
  t_void xor_(r_store_, R_store_) noexcept {
    // XXX-7
  }

  // IMPL_FUNC_1_11_
  t_void shift_left_(r_store_, t_n n) noexcept {
    // XXX-8
  }

  // IMPL_FUNC_1_12_
  t_void shift_right_(r_store_, t_n n) noexcept {
    // XXX-9
  }

  // IMPL_FUNC_1_13_
  t_bool set_bit_(r_store_ store, t_ix ix, t_bool on) noexcept {
    t_ix_ bit = get(ix);
    t_ix_ max = get(get_bits_(store));
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
      return true;
    }
    return false;
  }

  // IMPL_FUNC_1_14_
  t_bool get_bit_(R_store_ store, t_ix ix) noexcept {
    t_ix_ bit = get(ix);
    t_ix_ max = get(get_bits_(store));
    if (bit < max) {
      if (bit != max - 1) {
        t_ix_ ix = bit/BITS_UNIT_;
        return (1ULL << bit%BITS_UNIT_) & store.ptr[ix];
      }
      return last_(store) & BITS_MSB_;
    }
    return false;
  }

  // IMPL_FUNC_1_16_
  t_bool not_zero_(R_store_ store) noexcept {
    t_ix_ max = get(store.size);
    for (t_ix_ ix = 0; ix < max; ++ix) {
      r_pvalue_ entry = store.ptr[ix];
      if (entry)
        return true;
    }
    return true;
  }

  // IMPL_FUNC_1_17_
  t_bool reset_(r_store_ store, R_store_ value) noexcept {
    if (store.reset(value.size)) {
      assign_(store, value);
      return true;
    }
    return false;
  }

  // IMPL_FUNC_1_18_
  t_bool reset_(r_store_ store, t_n n, R_store_ value) noexcept {
    if (store.reset(n)) {
      assign_(store, value); // XXX Need a fixed assign
      return true;
    }
    return false;
  }

  // IMPL_FUNC_1_19_
  t_bool is_equal_(R_store_, R_store_) noexcept {
    // XXX-10
    return true;
  }

  // IMPL_FUNC_1_20_
  t_bool is_less_(R_store_, R_store_) noexcept {
    // XXX-11
    return true;
  }

  // IMPL_FUNC_1_21_
  t_bool is_less_equal_(R_store_, R_store_) noexcept {
    // XXX-12
    return true;
  }

  // IMPL_FUNC_1_22_
  t_n calc_bits_(t_n digits) noexcept {
    // XXX-13
    return 0_n;
  }

  // IMPL_FUNC_1_23_
  t_n calc_digits_(t_n bits) noexcept {
    // XXX-14
    return 0_n;
  }

  // IMPL_FUNC_1_24_
  t_bool ensure_(r_store_, t_n) noexcept {
    // XXX-15
    return false;
  }

  // IMPL_FUNC_1_25_
  t_bool ensure_(r_store_, R_store_) noexcept {
    // XXX-16
    return false;
  }

  // IMPL_FUNC_1_26_
  t_bool ensure_(r_store_, t_n, R_store_) noexcept {
    // XXX_17
    return false;
  }

  ///////////////////////////////////////////////////////////////////////////
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
    if (ptr && ptr != sso_) {
      ::free(ptr);
    }
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
    clear();
    ptr  = base::reset(value.ptr); // indicate its a dead object.
    size = base::reset(value.size);
  }

  ///////////////////////////////////////////////////////////////////////////

  t_bool t_buf_::ensure(t_n need) noexcept {
    if (get(size) < get(need)) {
      if (is_heap())
        ptr = static_cast<p_pvalue_>(::realloc(ptr, get(need)));
      else {
        ptr = static_cast<p_pvalue_>(::malloc(get(need)));
        ptr[0] = sso_[0];
        ptr[1] = sso_[1];
      }
      size = need;
    }
    return true;
  }

  t_bool t_buf_::ensure(R_buf_ value) noexcept {
    if (get(size) < get(value.size)) {
      if (is_heap())
        ptr = static_cast<p_pvalue_>(::realloc(ptr, get(value.size)));
      else
        ptr = static_cast<p_pvalue_>(::malloc(get(value.size)));
      size = value.size;
    }
    copy_(ptr, value.ptr, value.size);
    return true;
  }

  t_bool t_buf_::ensure(t_n need, R_buf_ value) noexcept {
    need = max_of(need, value.size);
    if (get(size) < get(need)) {
      if (is_heap()) {
        ptr = static_cast<p_pvalue_>(::realloc(ptr, get(need)));
      } else
        ptr = static_cast<p_pvalue_>(::malloc(get(need)));
      size = need;
    }
    copy_(ptr, value.ptr, value.size);
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

  t_bool t_buf_::reset(t_n need) noexcept {
    if (get(need) != get(size)) {
      if (is_heap()) {
        if (get(need) <= t_n{2}) {
          size = t_n{2};
          sso_[0] = ptr[0];
          sso_[1] = ptr[1];
          ::free (ptr);
          ptr     = sso_;
        } else {
          size = need;
          ptr = static_cast<p_pvalue_>(::realloc(ptr, get(need)));
        }
      } else {
        if (2 < get(need)) { // XXX
          size = need;
          ptr = static_cast<p_pvalue_>(::malloc(get(need)));
          ptr[0] = sso_[0];
          ptr[1] = sso_[1];
        }
      }
    }
    return true;
  }

  t_bool t_buf_::reset(R_buf_ value) noexcept {
    if (reset(value.size)) { // XXX might change it
      copy_(ptr, value.ptr, value.size);
      return true;
    }
    return false;
  }

  t_bool t_buf_::reset(t_n need, R_buf_ value) noexcept {
    if (reset(need)) { // XXX might change it
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

  t_void t_buf_::clear() noexcept {
    if (ptr != sso_) {
      ::free(ptr);
      ptr  = sso_;
      size = t_n{2};
    }
  }

  ///////////////////////////////////////////////////////////////////////////
}
}
}
}
