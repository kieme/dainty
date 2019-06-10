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
  using named::t_ix;
  using named::t_validity;
  using named::t_bool;

  enum t_state { BIT_OFF, BIT_ON };

  constexpr t_n_ calc_bytes_(t_n_ bits) {
    t_n_ l = bits%8;
    if (l)
      bits += 8 - l;
    return bits/8;
  }

///////////////////////////////////////////////////////////////////////////////

  class t_bits_impl_ {
  public:
    using t_value = t_byte;
    using p_store = t_prefix<t_value>::p_;

    constexpr t_bits_impl_(t_n_ max, p_store store, t_state state) {
      fill_(max, store, state == BIT_OFF ? 0 : 0xFF);
    }

    constexpr operator t_bool() const {
      return size_;
    }

    constexpr t_bool set(t_n_ max, p_store store, t_ix, t_state) {
    }

    constexpr t_void set(t_err, t_n_ max, p_store store, t_ix, t_state) {
      ERR_GUARD(err) {
      }
    }

    constexpr t_void clear(t_n_ max, p_store store, t_state state) {
      fill_(max, store, state == BIT_OFF ? 0 : 0xFF);
    }

    constexpr t_void clear(r_err err, t_n_ max, p_store store, t_state state) {
      ERR_GUARD(err) {
        fill_(max, store, state == BIT_OFF ? 0 : 0xFF);
      }
    }

    constexpr t_bool get[t_n_ max, p_store store, t_ix ix] const {
    }

    constexpr t_bool is_full(t_n_ max) const {
      return size_ == max;
    }

    constexpr t_bool is_empty() const {
      return !size_;
    }

    constexpr t_n_ get_size() const {
      return size_;
    }

    template<typename F>
    constexpr t_void ceach(t_n_ max, p_store store, F f) const {
    }

    template<typename F>
    constexpr t_void ceach(r_err err, t_n_ max, p_store store, F f) const {
      ERR_GUARD(err) {
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
