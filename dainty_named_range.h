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

namespace dainty
{
namespace named
{
namespace range
{
  using named::t_void;
  using named::t_bool;
  using named::t_ix_;
  using named::t_ix;
  using named::t_n_;
  using named::t_n;
  using named::t_validity;
  using named::VALID;
  using named::INVALID;

///////////////////////////////////////////////////////////////////////////////

  template<typename I, typename TAG>
  class t_range {
  public:
    using t_tag  = TAG;
    using t_item = I;
    using p_item = typename t_prefix<I>::p_;

    t_range(p_item, t_n);

    operator t_validity() const;

    template<typename F> t_void  each(F);
    template<typename F> t_void  each(F) const;
    template<typename F> t_void ceach(F) const;

    p_item const item;
    const t_n    n;
  };

  template<typename I, typename TAG>
  class t_crange {
  public:
    using t_tag  = TAG;
    using t_item = I;
    using P_item = typename t_prefix<I>::P_;

    t_crange(const t_range<I, TAG>&);
    t_crange(P_item, t_n);

    operator t_validity() const;

    template<typename F> t_void  each(F);
    template<typename F> t_void  each(F) const;
    template<typename F> t_void ceach(F) const;

    P_item const item;
    const t_n    n;
  };

  template<typename I, typename TAG> class t_range<const I, TAG>;

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
    // check_()
    const auto n = N - get(begin);
    return {arr + get(begin), t_n{n}};
  }

  template<typename TAG, typename T, t_n_ N>
  inline
  t_crange<T, TAG> mk_crange(T (&arr)[N], t_ix begin) {
    // check_()
    const auto n = N - get(begin);
    return {arr + get(begin), t_n{n}};
  }

  template<typename TAG, typename T, t_n_ N>
  inline
  t_crange<T, TAG> mk_crange(const T (&arr)[N], t_ix begin) {
    // check_()
    const auto n = N - get(begin);
    return {arr + get(begin), t_n{n}};
  }

  template<typename TAG, typename T, t_n_ N>
  inline
  t_range<T, TAG> mk_range(T (&arr)[N], t_ix begin, t_ix end) {
    // check_()
    const auto n = get(end) - get(begin);
    return {arr + get(begin), t_n{n}};
  }

  template<typename TAG, typename T, t_n_ N>
  inline
  t_crange<T, TAG> mk_crange(T (&arr)[N], t_ix begin, t_ix end) {
    // check_()
    const auto n = get(end) - get(begin);
    return {arr + get(begin), t_n{n}};
  }

  template<typename TAG, typename T, t_n_ N>
  inline
  t_crange<T, TAG> mk_crange(const T (&arr)[N], t_ix begin, t_ix end) {
    // check_()
    const auto n = get(end) - get(begin);
    return {arr + get(begin), t_n{n}};
  }

///////////////////////////////////////////////////////////////////////////////

  template<typename TAG1, typename TAG, typename T>
  inline
  t_range<T, TAG1> mk_range(t_range<T, TAG> range) {
    return {range.item, range.n};
  }

  template<typename TAG1, typename TAG, typename T>
  inline
  t_crange<T, TAG1> mk_crange(t_crange<T, TAG> range) {
    return {range.item, range.n};
  }

  template<typename TAG1, typename TAG, typename T>
  inline
  t_range<T, TAG1> mk_range(t_range<T, TAG> range, t_ix begin) {
    // check_()
    const auto n = get(range.n) - get(begin);
    return {range.item + get(begin), t_n{n}};
  }

  template<typename TAG1, typename TAG, typename T>
  inline
  t_crange<T, TAG1> mk_crange(t_crange<T, TAG> range, t_ix begin) {
    // check_()
    const auto n = get(range.n) - get(begin);
    return {range.item + get(begin), t_n{n}};
  }

  template<typename TAG1, typename TAG, typename T>
  inline
  t_range<T, TAG1> mk_range(t_range<T, TAG> range, t_ix begin, t_ix end) {
    // check_()
    const auto n = get(end) - get(begin);
    return {range.item + get(begin), t_n{n}};
  }

  template<typename TAG1, typename TAG, typename T>
  inline
  t_crange<T, TAG1> mk_crange(t_crange<T, TAG> range, t_ix begin, t_ix end) {
    // check_()
    const auto n = get(end) - get(begin);
    return {range.item + get(begin), t_n{n}};
  }

///////////////////////////////////////////////////////////////////////////////

  template<typename I, typename TAG>
  inline
  typename t_range<I, TAG>::p_item begin(t_range<I, TAG> range) {
    return range.item;
  }

  template<typename I, typename TAG>
  inline
  typename t_range<I, TAG>::p_item end(t_range<I, TAG> range) {
    return begin(range) + get(range.n);
  }

  template<typename I, typename TAG>
  inline
  typename t_crange<I, TAG>::P_item cbegin(t_range<I, TAG> range) {
    return range.item;
  }

  template<typename I, typename TAG>
  inline
  typename t_crange<I, TAG>::P_item cend(t_range<I, TAG> range) {
    return begin(range) + get(range.n);
  }

///////////////////////////////////////////////////////////////////////////////

  template<typename I, typename TAG>
  inline
  typename t_crange<I, TAG>::P_item begin(t_crange<I, TAG> range) {
    return range.item;
  }

  template<typename I, typename TAG>
  inline
  typename t_crange<I, TAG>::P_item end(t_crange<I, TAG> range) {
    return begin(range) + get(range.n);
  }

  template<typename I, typename TAG>
  inline
  typename t_crange<I, TAG>::P_item cbegin(t_crange<I, TAG> range) {
    return range.item;
  }

  template<typename I, typename TAG>
  inline
  typename t_crange<I, TAG>::P_item cend(t_crange<I, TAG> range) {
    return begin(range) + get(range.n);
  }

///////////////////////////////////////////////////////////////////////////////

  template<typename I, typename TAG>
  inline
  t_range<I, TAG>::t_range(p_item _item, t_n _n) : item{_item}, n{_n} {
      // check_(_item);
  }

  template<typename I, typename TAG>
  inline
  t_range<I, TAG>::operator t_validity() const {
    return item ? VALID : INVALID;
  }

  template<typename I, typename TAG>
  template<typename F>
  inline
  t_void  t_range<I, TAG>::each(F f) {
    for (auto&& i : *this)
      f(i);
  }

  template<typename I, typename TAG>
  template<typename F>
  inline
  t_void  t_range<I, TAG>::each(F f) const {
    for (auto&& i : *this)
      f(i);
  }

  template<typename I, typename TAG>
  template<typename F>
  inline
  t_void t_range<I, TAG>::ceach(F f) const {
    for (auto&& i : *this)
      f(i);
  }

///////////////////////////////////////////////////////////////////////////////

  template<typename I, typename TAG>
  inline
  t_crange<I, TAG>::t_crange(const t_range<I, TAG>& range)
    : item{range.item}, n{range.n} {
  }

  template<typename I, typename TAG>
  inline
  t_crange<I, TAG>::t_crange(P_item _item, t_n _n) : item{_item}, n{_n} {
    // check_(_item);
  }

  template<typename I, typename TAG>
  inline
  t_crange<I, TAG>::operator t_validity() const {
    return item ? VALID : INVALID;
  }

  template<typename I, typename TAG>
  template<typename F>
  inline
  t_void t_crange<I, TAG>::each(F f) {
    for (auto&& i : *this)
      f(i);
  }

  template<typename I, typename TAG>
  template<typename F>
  inline
  t_void t_crange<I, TAG>::each(F f) const {
    for (auto&& i : *this)
      f(i);
  }

  template<typename I, typename TAG>
  template<typename F>
  inline
  t_void t_crange<I, TAG>::ceach(F f) const {
    for (auto&& i : *this)
      f(i);
  }

///////////////////////////////////////////////////////////////////////////////

  template<typename I, typename TAG>
  inline
  t_bool operator==(t_crange<I, TAG> lh, t_crange<I, TAG> rh) {
    if (lh.n == rh.n) {
      if (lh.item != rh.item) {
        t_n_ i = 0, n = get(lh.n);
        for (; i < n; ++i)
          if (lh.item[i] != rh.item[i])
            break;
        return i == n;
      }
      return true;
    }
    return false;
  }

  template<typename I, typename TAG>
  inline
  t_bool operator==(t_crange<I, TAG> lh, t_range<I, TAG> rh) {
    return lh == t_crange<I, TAG>{rh};
  }

  template<typename I, typename TAG>
  inline
  t_bool operator==(t_range<I, TAG> lh, t_crange<I, TAG> rh) {
    return t_crange<I, TAG>{lh} == rh;
  }

  template<typename I, typename TAG>
  inline
  t_bool operator==(t_range<I, TAG> lh, t_range<I, TAG> rh) {
    return t_crange<I, TAG>{lh} == t_crange<I, TAG>{rh};
  }

///////////////////////////////////////////////////////////////////////////////

  template<typename I, typename TAG>
  inline
  t_bool operator!=(t_crange<I, TAG> lh, t_crange<I, TAG> rh) {
    return !(lh == rh);
  }

  template<typename I, typename TAG>
  inline
  t_bool operator!=(t_crange<I, TAG> lh, t_range<I, TAG> rh) {
    return !(lh == rh);
  }

  template<typename I, typename TAG>
  inline
  t_bool operator!=(t_range<I, TAG> lh, t_crange<I, TAG> rh) {
    return !(lh == rh);
  }

  template<typename I, typename TAG>
  inline
  t_bool operator!=(t_range<I, TAG> lh, t_range<I, TAG> rh) {
    return !(lh == rh);
  }

///////////////////////////////////////////////////////////////////////////////

}
}
}

#endif
