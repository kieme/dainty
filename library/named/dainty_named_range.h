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

#ifndef _DAINTY_NAMED_RANGE_H_
#define _DAINTY_NAMED_RANGE_H_

#include "dainty_named.h"

// DAINTY_NAMED_RANGE_CHECK

namespace dainty
{
namespace named
{
namespace range
{
  using named::t_void;
  using named::P_void;
  using named::t_bool;
  using named::t_ix_;
  using named::t_ix;
  using named::t_n_;
  using named::t_n;
  using named::t_validity;
  using named::t_byte;
  using named::VALID;
  using named::INVALID;

///////////////////////////////////////////////////////////////////////////////

  template<typename T, typename TAG>
  class t_crange;

  template<typename T, typename TAG>
  class t_range {
  public:
    using t_tag  = TAG;
    using t_item = T;
    using r_item = typename t_prefix<T>::r_;
    using R_item = typename t_prefix<T>::R_;
    using p_item = typename t_prefix<T>::p_;
    using P_item = typename t_prefix<T>::P_;

    t_range(p_item, t_n);

    t_range& operator=(const t_range<T, TAG>&);
    t_range& operator=(const t_crange<T, TAG>&); // XXX looks wrong

    operator t_validity() const;

    r_item operator[](t_ix);
    R_item operator[](t_ix) const;

    p_item get(t_ix);
    P_item get(t_ix) const;

    template<typename F> t_void  each(F);
    template<typename F> t_void  each(F) const;
    template<typename F> t_void ceach(F) const;

    p_item const ptr;
    t_n          n;
  };

  template<typename T, typename TAG>
  class t_crange {
  public:
    using t_tag  = TAG;
    using t_item = T;
    using R_item = typename t_prefix<T>::R_;
    using P_item = typename t_prefix<T>::P_;

    t_crange(P_item, t_n);
    t_crange(const t_range<T, TAG>&);

    operator t_validity() const;

    R_item operator[](t_ix) const;

    P_item get(t_ix) const;

    template<typename F> t_void  each(F);
    template<typename F> t_void  each(F) const;
    template<typename F> t_void ceach(F) const;

    P_item const ptr;
    t_n          n;
  };

  template<typename T, typename TAG> class t_range<const T, TAG>;

///////////////////////////////////////////////////////////////////////////////

  enum  t_byte_range_tag_ {};
  using t_byte_range  = t_range<t_byte, t_byte_range_tag_>;
  using T_byte_range  = t_prefix<t_byte_range>::T_;
  using R_byte_range  = t_prefix<t_byte_range>::R_;
  using r_byte_range  = t_prefix<t_byte_range>::r_;
  using P_byte_range  = t_prefix<t_byte_range>::P_;
  using p_byte_range  = t_prefix<t_byte_range>::p_;

  using t_byte_crange = t_crange<t_byte, t_byte_range_tag_>;
  using T_byte_crange = t_prefix<t_byte_crange>::T_;
  using R_byte_crange = t_prefix<t_byte_crange>::R_;
  using r_byte_crange = t_prefix<t_byte_crange>::r_;
  using P_byte_crange = t_prefix<t_byte_crange>::P_;
  using p_byte_crange = t_prefix<t_byte_crange>::p_;

///////////////////////////////////////////////////////////////////////////////

  template<typename T, typename TAG>
  inline
  typename t_range<T, TAG>::p_item begin(t_range<T, TAG> range) {
    return range.ptr;
  }

  template<typename T, typename TAG>
  inline
  typename t_range<T, TAG>::p_item end(t_range<T, TAG> range) {
    return begin(range) + get(range.n);
  }

  template<typename T, typename TAG>
  inline
  typename t_crange<T, TAG>::P_item cbegin(t_range<T, TAG> range) {
    return range.ptr;
  }

  template<typename T, typename TAG>
  inline
  typename t_crange<T, TAG>::P_item cend(t_range<T, TAG> range) {
    return begin(range) + get(range.n);
  }

///////////////////////////////////////////////////////////////////////////////

  template<typename T, typename TAG>
  inline
  typename t_crange<T, TAG>::P_item begin(t_crange<T, TAG> range) {
    return range.ptr;
  }

  template<typename T, typename TAG>
  inline
  typename t_crange<T, TAG>::P_item end(t_crange<T, TAG> range) {
    return begin(range) + get(range.n);
  }

  template<typename T, typename TAG>
  inline
  typename t_crange<T, TAG>::P_item cbegin(t_crange<T, TAG> range) {
    return range.ptr;
  }

  template<typename T, typename TAG>
  inline
  typename t_crange<T, TAG>::P_item cend(t_crange<T, TAG> range) {
    return begin(range) + get(range.n);
  }

///////////////////////////////////////////////////////////////////////////////

  template<typename T, typename TAG>
  inline
  t_bool operator==(t_crange<T, TAG> lh, t_crange<T, TAG> rh) {
    if (lh.n == rh.n) {
      if (lh.ptr != rh.ptr) {
        t_n_ i = 0, n = get(lh.n);
        for (; i < n; ++i)
          if (lh.ptr[i] != rh.ptr[i])
            break;
        return i == n;
      }
      return true;
    }
    return false;
  }

  template<typename T, typename TAG>
  inline
  t_bool operator==(t_crange<T, TAG> lh, t_range<T, TAG> rh) {
    return lh == t_crange<T, TAG>{rh};
  }

  template<typename T, typename TAG>
  inline
  t_bool operator==(t_range<T, TAG> lh, t_crange<T, TAG> rh) {
    return t_crange<T, TAG>{lh} == rh;
  }

  template<typename T, typename TAG>
  inline
  t_bool operator==(t_range<T, TAG> lh, t_range<T, TAG> rh) {
    return t_crange<T, TAG>{lh} == t_crange<T, TAG>{rh};
  }

///////////////////////////////////////////////////////////////////////////////

  template<typename T, typename TAG>
  inline
  t_bool operator!=(t_crange<T, TAG> lh, t_crange<T, TAG> rh) {
    return !(lh == rh);
  }

  template<typename T, typename TAG>
  inline
  t_bool operator!=(t_crange<T, TAG> lh, t_range<T, TAG> rh) {
    return !(lh == rh);
  }

  template<typename T, typename TAG>
  inline
  t_bool operator!=(t_range<T, TAG> lh, t_crange<T, TAG> rh) {
    return !(lh == rh);
  }

  template<typename T, typename TAG>
  inline
  t_bool operator!=(t_range<T, TAG> lh, t_range<T, TAG> rh) {
    return !(lh == rh);
  }

///////////////////////////////////////////////////////////////////////////////

  t_void check_(        t_n_, t_ix_);
  t_void check_(        t_n_, t_ix_, t_ix_);
  t_void check_(P_void, t_n_);
  t_void check_(P_void, t_n_, t_ix_);
  t_void check_(P_void, t_n_, t_ix_, t_ix_);
  t_void check_(P_void, t_n_, P_void, t_n_);

///////////////////////////////////////////////////////////////////////////////

  template<typename TAG, typename T, t_n_ N>
  inline
  t_range<T, TAG> mk_range(T (&arr)[N]) {
    return {arr, t_n{N}};
  }

  template<typename TAG, typename T, t_n_ N>
  inline
  t_crange<T, TAG> mk_crange(T (&arr)[N]) {
    return {arr, t_n{N}};
  }

  template<typename TAG, typename T, t_n_ N>
  inline
  t_crange<T, TAG> mk_crange(const T (&arr)[N]) {
    return {arr, t_n{N}};
  }

  template<typename TAG, typename T, t_n_ N>
  inline
  t_range<T, TAG> mk_range(T (&arr)[N], t_ix begin) {
#ifdef DAINTY_NAMED_RANGE_CHECK
    check_(N, get(begin));
#endif
    const auto n = N - get(begin);
    return {arr + get(begin), t_n{n}};
  }

  template<typename TAG, typename T, t_n_ N>
  inline
  t_crange<T, TAG> mk_crange(T (&arr)[N], t_ix begin) {
#ifdef DAINTY_NAMED_RANGE_CHECK
    check_(N, get(begin));
#endif
    const auto n = N - get(begin);
    return {arr + get(begin), t_n{n}};
  }

  template<typename TAG, typename T, t_n_ N>
  inline
  t_crange<T, TAG> mk_crange(const T (&arr)[N], t_ix begin) {
#ifdef DAINTY_NAMED_RANGE_CHECK
    check_(N, get(begin));
#endif
    const auto n = N - get(begin);
    return {arr + get(begin), t_n{n}};
  }

  template<typename TAG, typename T, t_n_ N>
  inline
  t_range<T, TAG> mk_range(T (&arr)[N], t_ix begin, t_ix end) {
#ifdef DAINTY_NAMED_RANGE_CHECK
    check_(N, get(begin), get(end));
#endif
    const auto n = get(end) - get(begin);
    return {arr + get(begin), t_n{n}};
  }

  template<typename TAG, typename T, t_n_ N>
  inline
  t_crange<T, TAG> mk_crange(T (&arr)[N], t_ix begin, t_ix end) {
#ifdef DAINTY_NAMED_RANGE_CHECK
    check_(N, get(begin), get(end));
#endif
    const auto n = get(end) - get(begin);
    return {arr + get(begin), t_n{n}};
  }

  template<typename TAG, typename T, t_n_ N>
  inline
  t_crange<T, TAG> mk_crange(const T (&arr)[N], t_ix begin, t_ix end) {
#ifdef DAINTY_NAMED_RANGE_CHECK
    check_(N, get(begin), get(end));
#endif
    const auto n = get(end) - get(begin);
    return {arr + get(begin), t_n{n}};
  }

///////////////////////////////////////////////////////////////////////////////

  template<typename TAG1, typename TAG, typename T>
  inline
  t_range<T, TAG1> mk_range(t_range<T, TAG> range) {
    return {range.ptr, range.n};
  }

  template<typename TAG1, typename TAG, typename T>
  inline
  t_crange<T, TAG1> mk_crange(t_crange<T, TAG> range) {
    return {range.ptr, range.n};
  }

  template<typename TAG1, typename TAG, typename T>
  inline
  t_range<T, TAG1> mk_range(t_range<T, TAG> range, t_ix begin) {
#ifdef DAINTY_NAMED_RANGE_CHECK
    check_(range.ptr, get(range.n), get(begin));
#endif
    const auto n = get(range.n) - get(begin);
    return {range.ptr + get(begin), t_n{n}};
  }

  template<typename TAG1, typename TAG, typename T>
  inline
  t_crange<T, TAG1> mk_crange(t_crange<T, TAG> range, t_ix begin) {
#ifdef DAINTY_NAMED_RANGE_CHECK
    check_(range.ptr, get(range.n), get(begin));
#endif
    const auto n = get(range.n) - get(begin);
    return {range.ptr + get(begin), t_n{n}};
  }

  template<typename TAG1, typename TAG, typename T>
  inline
  t_range<T, TAG1> mk_range(t_range<T, TAG> range, t_ix begin, t_ix end) {
#ifdef DAINTY_NAMED_RANGE_CHECK
    check_(range.ptr, get(range.n), get(begin), get(end));
#endif
    const auto n = get(end) - get(begin);
    return {range.ptr + get(begin), t_n{n}};
  }

  template<typename TAG1, typename TAG, typename T>
  inline
  t_crange<T, TAG1> mk_crange(t_crange<T, TAG> range, t_ix begin, t_ix end) {
#ifdef DAINTY_NAMED_RANGE_CHECK
    check_(range.ptr, get(range.n), get(begin), get(end));
#endif
    const auto n = get(end) - get(begin);
    return {range.ptr + get(begin), t_n{n}};
  }

///////////////////////////////////////////////////////////////////////////////

  template<typename T, typename TAG>
  inline
  t_range<T, TAG>::t_range(p_item _ptr, t_n _n) : ptr{_ptr}, n{_n} {
#ifdef DAINTY_NAMED_RANGE_CHECK
    check_(ptr, named::get(n));
#endif
  }

  template<typename T, typename TAG>
  inline
  t_range<T, TAG>& t_range<T, TAG>::operator=(const t_range<T, TAG>& range) {
#ifdef DAINTY_NAMED_RANGE_CHECK
    check_(ptr, named::get(n), range.ptr, named::get(range.n));
#endif
    auto n = named::get(range.n);
    for (t_n_ i = 0; i < n; ++i)
      ptr[i] = range.ptr[i];
    return *this;
  }

  template<typename T, typename TAG>
  inline
  t_range<T, TAG>& t_range<T, TAG>::operator=(const t_crange<T, TAG>& range) {
#ifdef DAINTY_NAMED_RANGE_CHECK
    check_(ptr, named::get(n), range.ptr, named::get(range.n));
#endif
    auto n = named::get(range.n);
    for (t_n_ i = 0; i < n; ++i)
      ptr[i] = range.ptr[i];
    return *this;
  }

  template<typename T, typename TAG>
  inline
  typename t_range<T, TAG>::r_item t_range<T, TAG>::operator[](t_ix ix) {
#ifdef DAINTY_NAMED_RANGE_CHECK
    check_(ptr, named::get(n), named::get(ix));
#endif
    return ptr[named::get(ix)];
  }

  template<typename T, typename TAG>
  inline
  typename t_range<T, TAG>::R_item t_range<T, TAG>::operator[](t_ix ix) const {
#ifdef DAINTY_NAMED_RANGE_CHECK
    check_(ptr, named::get(n), named::get(ix));
#endif
    return ptr[named::get(ix)];
  }

  template<typename T, typename TAG>
  inline
  typename t_range<T, TAG>::p_item t_range<T, TAG>::get(t_ix ix) {
#ifdef DAINTY_NAMED_RANGE_CHECK
    check_(ptr, named::get(n), named::get(ix));
#endif
    return ptr + named::get(ix);
  }

  template<typename T, typename TAG>
  inline
  typename t_range<T, TAG>::P_item t_range<T, TAG>::get(t_ix ix) const {
#ifdef DAINTY_NAMED_RANGE_CHECK
    check_(ptr, named::get(n), named::get(ix));
#endif
    return ptr + named::get(ix);
  }

  template<typename T, typename TAG>
  inline
  t_range<T, TAG>::operator t_validity() const {
    return ptr ? VALID : INVALID;
  }

  template<typename T, typename TAG>
  template<typename F>
  inline
  t_void  t_range<T, TAG>::each(F f) {
    for (auto&& i : *this)
      f(i);
  }

  template<typename T, typename TAG>
  template<typename F>
  inline
  t_void  t_range<T, TAG>::each(F f) const {
    for (auto&& i : *this)
      f(i);
  }

  template<typename T, typename TAG>
  template<typename F>
  inline
  t_void t_range<T, TAG>::ceach(F f) const {
    for (auto&& i : *this)
      f(i);
  }

///////////////////////////////////////////////////////////////////////////////

  template<typename T, typename TAG>
  inline
  t_crange<T, TAG>::t_crange(const t_range<T, TAG>& range)
    : ptr{range.ptr}, n{range.n} {
  }

  template<typename T, typename TAG>
  inline
  t_crange<T, TAG>::t_crange(P_item _ptr, t_n _n) : ptr{_ptr}, n{_n} {
#ifdef DAINTY_NAMED_RANGE_CHECK
    check_(_ptr, named::get(n));
#endif
  }

  template<typename T, typename TAG>
  inline
  t_crange<T, TAG>::operator t_validity() const {
    return ptr ? VALID : INVALID;
  }

  template<typename T, typename TAG>
  inline
  typename t_crange<T, TAG>::R_item
      t_crange<T, TAG>::operator[](t_ix ix) const {
#ifdef DAINTY_NAMED_RANGE_CHECK
    check_(ptr, named::get(n), named::get(ix));
#endif
    return ptr[named::get(ix)];
  }

  template<typename T, typename TAG>
  inline
  typename t_crange<T, TAG>::P_item t_crange<T, TAG>::get(t_ix ix) const {
#ifdef DAINTY_NAMED_RANGE_CHECK
    check_(ptr, named::get(n), named::get(ix));
#endif
    return ptr + named::get(ix);
  }

  template<typename T, typename TAG>
  template<typename F>
  inline
  t_void t_crange<T, TAG>::each(F f) {
    for (auto&& i : *this)
      f(i);
  }

  template<typename T, typename TAG>
  template<typename F>
  inline
  t_void t_crange<T, TAG>::each(F f) const {
    for (auto&& i : *this)
      f(i);
  }

  template<typename T, typename TAG>
  template<typename F>
  inline
  t_void t_crange<T, TAG>::ceach(F f) const {
    for (auto&& i : *this)
      f(i);
  }

///////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
