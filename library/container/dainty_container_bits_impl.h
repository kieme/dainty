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
#include "dainty_named_ptr.h"
#include "dainty_container_err.h"
#include <stdlib.h>

namespace dainty
{
namespace container
{
namespace bits
{
  using named::t_n_;
  using named::t_uint;
  using named::t_validity;
  using named::t_bool;
  using named::t_void;
  using named::t_logical;
  using named::t_prefix;
  using named::t_byte_;
  using named::ptr::t_ptr;

///////////////////////////////////////////////////////////////////////////////

  using r_byte_ = t_prefix<t_byte_>::r_;
  using p_byte_ = t_prefix<t_byte_>::p_;
  using P_byte_ = t_prefix<t_byte_>::P_;
  using r_err_  = container::err::r_err;

  struct t_free_del_ {
    t_void operator()(p_byte_ ptr) noexcept {
      ::free (ptr);
    }
  };
  enum t_store_ptr_tag_ {};
  using t_store_ptr_ = t_ptr<t_byte_[], t_store_ptr_tag_, t_free_del_>;

  enum  t_bit_tag_ { };
  using t_bit_ = t_uint;
  using t_bit  = t_logical<t_bit_, t_bit_tag_>;

///////////////////////////////////////////////////////////////////////////////

  enum t_bit_state  : t_bool { BIT_OFF  = 0, BIT_ON  = 1 };
  enum t_bit_result : t_uint { BIT_OFF_ = BIT_OFF, BIT_ON_ = BIT_ON,
                               BIT_BAD = 2 };

  constexpr t_bool operator==(t_bit_result result, t_bit_state state) noexcept {
    return result != BIT_BAD && result == state;
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

///////////////////////////////////////////////////////////////////////////////

  t_store_ptr_ alloc_(t_n_ bits) noexcept {
    return {(p_byte_)::malloc(calc_bytes_(bits))}; // not in a header
  }

  constexpr t_void fill_(t_n_ max, p_byte_ p, t_byte_ value) noexcept { // range
    for (p_byte_ end = p + max; p < end; ++p)
      *p = value;
  }

///////////////////////////////////////////////////////////////////////////////

  class t_bits_impl_ {
  public:
    using t_value = t_byte_;
    using p_store = p_byte_;
    using P_store = P_byte_;

    inline t_bits_impl_(t_n_ max, p_store store,
                        t_bit_state state) noexcept : size_{0} {
      clear(max, store, state);
    }

    inline t_bits_impl_(r_err_ err, t_n_ max, p_store store,
                        t_bit_state state) noexcept : size_{0} {
      ERR_GUARD(err) {
        clear(max, store, state);
      }
    }

    inline t_bits_impl_(t_n_ size) : size_{size} {
    }

    inline operator t_bool() const {
      return size_;
    }

    inline t_bool set(t_n_ max, p_store store, t_bit_ bit,
                      t_bit_state state) noexcept {
      if (bit < max) {
        r_byte_ byte = store[bit/8];
        if (state == BIT_ON)
          byte |= (1 << bit%8);
        else
          byte &= ~(1 << bit%8);
        return true;
      }
      return false;
    }

    inline t_void set(r_err_ err, t_n_ max, p_store store, t_bit_ bit,
                      t_bit_state state) noexcept {
      ERR_GUARD(err) {
        if (!set(max, store, bit, state))
          err = err::E_XXX;
      }
    }

    inline t_void clear(t_n_ max, p_store store,
                           t_bit_state state) noexcept {
      if (state == BIT_OFF) {
        fill_(max, store, 0);
        size_ = 0;
      } else {
        fill_(max, store, 0xFF);
        size_ = max;
      }
    }

    inline t_void clear(r_err_ err, t_n_ max, p_store store,
                        t_bit_state state) noexcept {
      ERR_GUARD(err) {
        clear(max, store, state);
      }
    }

    inline t_bit_result get(t_n_ max, P_store store, t_bit_ bit) const noexcept {
      if (bit < max) {
        t_byte_ value = store[bit/8];
        return value & (1 << bit%8) ? BIT_ON_ : BIT_OFF_;
      }
      return BIT_BAD;
    }

    inline t_bool is_full(t_n_ max) const noexcept {
      return size_ == max;
    }

    inline t_bool is_empty() const noexcept {
      return !size_;
    }

    inline t_n_ get_size() const noexcept {
      return size_;
    }

    template<typename F>
    inline t_void ceach(t_n_ max, P_store store, F f) const noexcept {
      t_n_ byte_max = max/8;
      for (t_n_ byte_n = 0; byte_n < byte_max; ++byte_n)
        for (t_n_ bit_n = 0; bit_n < 8; ++bit_n) // XXX check how many bits
          f(t_bit(byte_n*8 + bit_n), (*store & (1 << bit_n)) ? BIT_ON : BIT_OFF);
    }

    template<typename F>
    inline t_void ceach(r_err_ err, t_n_ max, P_store store, F f) const noexcept {
      ERR_GUARD(err) {
        t_n_ byte_max = max/8;
        for (t_n_ byte_n = 0; byte_n < byte_max; ++byte_n)
          for (t_n_ bit_n = 0; bit_n < 8; ++bit_n) // check how many bits
            f(t_bit(byte_n*8 + bit_n), (*store & (1 << bit_n)) ? BIT_ON : BIT_OFF);
      }
    }

    t_n_ size_;
  };

///////////////////////////////////////////////////////////////////////////////
}
}
}

#endif