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

#ifndef _DAINTY_CONTAINER_BITS_H_
#define _DAINTY_CONTAINER_BITS_H_

#include "dainty_named.h"
#include "dainty_container_bits_impl.h"

namespace dainty
{
namespace container
{
namespace bits
{
  using named::t_n;
  using named::t_validity;
  using named::t_bool;
  using named::VALID;
  using named::INVALID;

///////////////////////////////////////////////////////////////////////////////

  template<t_n_ N>
  class t_bits {
    using t_impl_ = t_bits_impl_;
  public:
    t_bits(t_bit_state = BIT_OFF)    noexcept;
    t_bits(const t_bits&)            noexcept = default;
    t_bits& operator=(const t_bits&) noexcept = default;

    operator t_validity() const noexcept;
    operator t_bool    () const noexcept;

    t_bool set(       t_bit, t_bit_state) noexcept;
    t_void set(t_err, t_bit, t_bit_state) noexcept;

    t_void clear(       t_bit_state = BIT_OFF) noexcept;
    t_void clear(t_err, t_bit_state = BIT_OFF) noexcept;

    t_bool operator[t_bit] const noexcept;
    t_bool is_full ()      const noexcept;
    t_bool is_empty()      const noexcept;
    t_n    get_size()      const noexcept;
    t_n    get_capacity()  const noexcept;

    template<typename F> t_void  each(       F) const noexcept;
    template<typename F> t_void  each(t_err, F) const noexcept;
    template<typename F> t_void ceach(       F) const noexcept;
    template<typename F> t_void ceach(t_err, F) const noexcept;

  private:
    typename t_impl_::t_value store_[calc_bytes_(N)];
    t_impl_                   impl_;
  };

///////////////////////////////////////////////////////////////////////////////

  template<>
  class t_bits<0> {
    using t_impl_ = t_bits_impl_;
  public:
  public:
    t_bits(       t_n max, t_bit_state = BIT_OFF) noexcept;
    t_bits(t_err, t_n max, t_bit_state = BIT_OFF) noexcept;
    t_bits(t_bits&&) noexcept;

    t_bits(const t_bits&)            = delete;
    t_bits& operator=(const t_bits&) = delete;
    t_bits& operator=(t_bits&&)      = delete;

    operator t_validity() const noexcept;
    operator t_bool    () const noexcept;

    t_bool set(       t_bit, t_bit_state) noexcept;
    t_void set(t_err, t_bit, t_bit_state) noexcept;

    t_void clear(       t_bit_state = BIT_OFF) noexcept;
    t_void clear(t_err, t_bit_state = BIT_OFF) noexcept;

    t_bit_result operator[t_bit] const noexcept;
    t_bool       is_full ()      const noexcept;
    t_bool       is_empty()      const noexcept;
    t_n          get_size()      const noexcept;
    t_n          get_capacity()  const noexcept;

    template<typename F> t_void  each(       F) const noexcept;
    template<typename F> t_void  each(t_err, F) const noexcept;
    template<typename F> t_void ceach(       F) const noexcept;
    template<typename F> t_void ceach(t_err, F) const noexcept;

  private:
    t_n_                      max_;
    typename t_impl_::t_value store_;
    t_impl_                   impl_;
  };

///////////////////////////////////////////////////////////////////////////////

  // ==
  // !=
  // &
  // |
  // ^

///////////////////////////////////////////////////////////////////////////////

  template<t_n_ N>
  inline
  t_bits<N>::t_bits(t_bit_state state) : impl_{N, store_, state} noexcept {
  }

  template<t_n_ N>
  inline
  t_bits<N>::operator t_validity() const noexcept {
    return VALID;
  }

  template<t_n_ N>
  inline
  t_bits<N>::operator t_bool() const noexcept {
    return impl_;
  }

  template<t_n_ N>
  inline
  t_bool t_bits<N>::set(t_bit bit, t_bit_state state) noexcept {
    return impl_.set(N, store_, bit, state);
  }

  template<t_n_ N>
  inline
  t_void t_bits<N>::set(t_err err, t_bit bit, t_bit_state state) noexcept {
    return impl_.set(err, N, store_, bit, state);
  }

  template<t_n_ N>
  inline
  t_void t_bits<N>::clear(t_bit_state state) noexcept {
    return impl_.clear(N, store_, state);
  }

  template<t_n_ N>
  inline
  t_void t_bits<N>::clear(t_err err, t_bit_state state) noexcept {
    return impl_.clear(err, N, store_, state);
  }

  template<t_n_ N>
  inline
  t_bit_result t_bits<N>::operator[t_bit bit] const noexcept {
    return impl_.get(N, store_, bit);
  }

  template<t_n_ N>
  inline
  t_bool t_bits<N>::is_full() const noexcept {
    return impl_.is_full(N);
  }

  template<t_n_ N>
  inline
  t_bool t_bits<N>::is_empty() const noexcept {
    return impl_.is_empty();
  }

  template<t_n_ N>
  inline
  t_n t_bits<N>::get_size() const noexcept {
    return t_n{impl_.get_size()};
  }

  template<t_n_ N>
  inline
  t_n t_bits<N>::get_capacity() const noexcept {
    return t_n{N};
  }

  template<t_n_ N>
  template<typename F>
  inline
  t_void t_bits<N>::each(F f) const noexcept {
    ceach(N, store_, f);
  }

  template<t_n_ N>
  template<typename F>
  inline
  t_void t_bits<N>::each(t_err err, F f) const noexcept {
    ceach(err, N, store_, f);
  }

  template<t_n_ N>
  template<typename F>
  inline
  t_void t_bits<N>::ceach(F f) const noexcept {
    impl_.ceach(N, store_, f);
  }

  template<t_n_ N>
  template<typename F>
  inline
  t_void t_bits<N>::ceach(t_err err, F f) const noexcept {
    impl_.ceach(err, N, store_, f);
  }

///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////

}
}
}
#endif
