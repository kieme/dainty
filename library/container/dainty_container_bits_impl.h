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

#ifndef _DAINTY_CONTAINER_BITS_IMPL_H_
#define _DAINTY_CONTAINER_BITS_IMPL_H_

#include "dainty_named.h"

namespace dainty
{
namespace container
{
namespace bits
{
  using named::t_n;
  using named::t_validity;
  using named::t_bool;

///////////////////////////////////////////////////////////////////////////////

  enum t_bit_state  : t_bool { BIT_OFF  = 0, BIT_ON  = 1 };
  enum t_bit_result : t_int  { BIT_OFF_ = 0, BIT_ON_ = 1, BIT_UNDEF = 2};

  constexpr t_bool operator==(t_bit_result result, t_bit_state state) noexcept {
    return result != t_bit_result::BIT_UNDEF && result == state;
  }

  constexpr t_bool operator!=(t_bit_result result, t_bit_state state) noexcept {
    return !(result == state);
  }

  constexpr t_bool operator==(t_bit_state state, t_bit_result result) noexcept {
    return result == state;
  }

  constexpr t_bool operator!=(t_bit_state state, t_bit_result result) noexcept {
    return !(result == state);
  }

///////////////////////////////////////////////////////////////////////////////

  constexpr t_n_ calc_bytes_(t_n_ bits) noexcept {
    t_n_ l = bits%8;
    if (l)
      bits += 8 - l;
    return bits/8;
  }

  enum  t_bit_tag_ { };
  using t_bit_ = t_uint;
  using t_bit  = t_logical<t_bit_, t_bit_tag_>;

///////////////////////////////////////////////////////////////////////////////

  inline
  t_void fill_(t_n_ max, p_char p, t_char value) noexcept { // range
    // memset
    for (p_char end = p + max; p < end; ++p)
      *p = value;
  }

///////////////////////////////////////////////////////////////////////////////

  class t_bits_impl_ {
  public:
    using t_value = t_byte;
    using p_store = t_prefix<t_value>::p_;

    constexpr t_bits_impl_(t_n_ max, p_store store,
                           t_bit_state state) noexcept {
      fill_(max, store, state == BIT_OFF ? 0 : 0xFF);
    }

    constexpr operator t_bool() const {
      return size_;
    }

    constexpr t_bool set(t_n_ max, p_store store, t_bit, t_bit_state) noexcept {
    }

    constexpr t_void set(t_err, t_n_ max, p_store store, t_bit bit,
                         t_bit_state) noexcept {
      ERR_GUARD(err) {
      }
    }

    constexpr t_void clear(t_n_ max, p_store store,
                           t_bit_state state) noexcept {
      fill_(max, store, state == BIT_OFF ? 0 : 0xFF);
    }

    constexpr t_void clear(r_err err, t_n_ max, p_store store,
                           t_bit_state state) noexcept {
      ERR_GUARD(err) {
        fill_(max, store, state == BIT_OFF ? 0 : 0xFF);
      }
    }

    constexpr t_bool get[t_n_ max, p_store store, t_bit_ bit] const noexcept {
    }

    constexpr t_bool is_full(t_n_ max) const noexcept {
      return size_ == max;
    }

    constexpr t_bool is_empty() const noexcept {
      return !size_;
    }

    constexpr t_n_ get_size() const noexcept {
      return size_;
    }

    template<typename F>
    constexpr t_void ceach(t_n_ max, p_store store, F f) const noexcept {
      for (t_n_ byte_n = 0; byte_n < max; ++byte_n)
        for (t_n_ bit_n = 0; bit_n < 8; ++bit_n)
          f(t_bit{byte_n*8 + n}, (*store & (1 << n)) ? BIT_ON : BIT_OFF);
    }

    template<typename F>
    constexpr t_void ceach(r_err err, t_n_ max, p_store store,
                           F f) const noexcept {
      ERR_GUARD(err) {
        for (t_n_ byte_n = 0; byte_n < max; ++byte_n)
          for (t_n_ bit_n = 0; bit_n < 8; ++bit_n)
            f(t_bit{byte_n*8 + n}, (*store & (1 << n)) ? BIT_ON : BIT_OFF);
      }
    }

  private:
    t_n_ size_;
  };

///////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
