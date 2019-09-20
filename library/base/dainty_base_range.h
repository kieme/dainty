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

#ifndef _DAINTY_BASE_RANGE_H_
#define _DAINTY_BASE_RANGE_H_

#include "dainty_base.h"
#include "dainty_base_range_impl.h"

namespace dainty
{
namespace base
{
namespace range
{
///////////////////////////////////////////////////////////////////////////////

  using types::t_prefix;
  using types::t_bool;
  using types::t_void;
  using types::t_n_;
  using types::t_ix_;
  using types::t_byte_;
  using types::t_char;
  using types::P_void;

  using specific::P_cstr;
  using specific::t_n;
  using specific::T_n;
  using specific::t_ix;
  using specific::t_begin_ix;
  using specific::t_end_ix;
  using specific::t_validity;
  using specific::VALID;
  using specific::INVALID;

  using assertion::assert_now;

  using impl_::check_range_;

///////////////////////////////////////////////////////////////////////////////

  enum t_skip_ { SKIP_ };

///////////////////////////////////////////////////////////////////////////////

  template<typename, typename>
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

    constexpr t_range()                     noexcept = default;
    constexpr t_range(p_item, t_n)          noexcept;
    constexpr t_range(p_item, t_n, t_skip_) noexcept;

    constexpr operator t_validity() const noexcept;

    template<typename TAG1 = TAG>
    constexpr t_range<T, TAG1> mk_range()                     noexcept;
    template<typename TAG1 = TAG>
    constexpr t_range<T, TAG1> mk_range(t_begin_ix)           noexcept;
    template<typename TAG1 = TAG>
    constexpr t_range<T, TAG1> mk_range(t_begin_ix, t_end_ix) noexcept;
    template<typename TAG1 = TAG>
    constexpr t_range<T, TAG1> mk_range(t_n)                  noexcept;
    template<typename TAG1 = TAG>
    constexpr t_range<T, TAG1> mk_range(t_begin_ix, t_n)      noexcept;

    template<typename TAG1 = TAG>
    constexpr t_crange<T, TAG1> mk_crange()                     const noexcept;
    template<typename TAG1 = TAG>
    constexpr t_crange<T, TAG1> mk_crange(t_begin_ix)           const noexcept;
    template<typename TAG1 = TAG>
    constexpr t_crange<T, TAG1> mk_crange(t_begin_ix, t_end_ix) const noexcept;
    template<typename TAG1 = TAG>
    constexpr t_crange<T, TAG1> mk_crange(t_n)                  const noexcept;
    template<typename TAG1 = TAG>
    constexpr t_crange<T, TAG1> mk_crange(t_begin_ix, t_n)      const noexcept;

              r_item operator[](t_ix)       noexcept;
    constexpr R_item operator[](t_ix) const noexcept;

              p_item address(t_ix)       noexcept;
    constexpr P_item address(t_ix) const noexcept;

    t_range& copy(const t_range <T, TAG>&) noexcept;
    t_range& copy(const t_crange<T, TAG>&) noexcept;

    t_range& copy(const t_range <T, TAG>&, t_begin_ix) noexcept;
    t_range& copy(const t_crange<T, TAG>&, t_begin_ix) noexcept;

    t_range& copy(const t_range <T, TAG>&, t_begin_ix, t_end_ix) noexcept;
    t_range& copy(const t_crange<T, TAG>&, t_begin_ix, t_end_ix) noexcept;

    t_range& copy(const t_range <T, TAG>&, t_n) noexcept;
    t_range& copy(const t_crange<T, TAG>&, t_n) noexcept;

    t_range& copy(const t_range <T, TAG>&, t_begin_ix, t_n) noexcept;
    t_range& copy(const t_crange<T, TAG>&, t_begin_ix, t_n) noexcept;

    template<typename F> t_void  each(F);
    template<typename F> t_void  each(F) const;
    template<typename F> t_void ceach(F) const;

    p_item const ptr = nullptr;
    T_n          n   = t_n{0};
  };

///////////////////////////////////////////////////////////////////////////////

  template<typename T, typename TAG>
  class t_crange {
  public:
    using t_tag  = TAG;
    using t_item = T;
    using R_item = typename t_prefix<T>::R_;
    using P_item = typename t_prefix<T>::P_;

    constexpr t_crange()                       noexcept = default;
    constexpr t_crange(P_item, t_n)            noexcept;
    constexpr t_crange(P_item, t_n, t_skip_)   noexcept;
    constexpr t_crange(const t_range<T, TAG>&) noexcept;

    constexpr operator t_validity() const noexcept;

    template<typename TAG1 = TAG>
    constexpr t_crange<T, TAG1> mk_crange()                     const noexcept;
    template<typename TAG1 = TAG>
    constexpr t_crange<T, TAG1> mk_crange(t_begin_ix)           const noexcept;
    template<typename TAG1 = TAG>
    constexpr t_crange<T, TAG1> mk_crange(t_begin_ix, t_end_ix) const noexcept;
    template<typename TAG1 = TAG>
    constexpr t_crange<T, TAG1> mk_crange(t_n)                  const noexcept;
    template<typename TAG1 = TAG>
    constexpr t_crange<T, TAG1> mk_crange(t_begin_ix, t_n)      const noexcept;

    constexpr R_item operator[](t_ix) const noexcept;
    constexpr P_item address   (t_ix) const noexcept;

    template<typename F> t_void  each(F);
    template<typename F> t_void  each(F) const;
    template<typename F> t_void ceach(F) const;

    P_item const ptr = nullptr;
    T_n          n   = t_n{0};
  };

  template<typename T, typename TAG> class t_range<const T, TAG>;

///////////////////////////////////////////////////////////////////////////////

  enum  t_byte_range_tag_ {};
  using t_byte_range  = t_range<t_byte_, t_byte_range_tag_>;
  using T_byte_range  = t_prefix<t_byte_range>::T_;
  using R_byte_range  = t_prefix<t_byte_range>::R_;
  using r_byte_range  = t_prefix<t_byte_range>::r_;
  using P_byte_range  = t_prefix<t_byte_range>::P_;
  using p_byte_range  = t_prefix<t_byte_range>::p_;

  using t_byte_crange = t_crange<t_byte_, t_byte_range_tag_>;
  using T_byte_crange = t_prefix<t_byte_crange>::T_;
  using R_byte_crange = t_prefix<t_byte_crange>::R_;
  using r_byte_crange = t_prefix<t_byte_crange>::r_;
  using P_byte_crange = t_prefix<t_byte_crange>::P_;
  using p_byte_crange = t_prefix<t_byte_crange>::p_;

///////////////////////////////////////////////////////////////////////////////

  enum  t_cstr_range_tag_ {};
  using t_cstr_range  = t_range<t_char, t_cstr_range_tag_>;
  using T_cstr_range  = t_prefix<t_cstr_range>::T_;
  using R_cstr_range  = t_prefix<t_cstr_range>::R_;
  using r_cstr_range  = t_prefix<t_cstr_range>::r_;
  using P_cstr_range  = t_prefix<t_cstr_range>::P_;
  using p_cstr_range  = t_prefix<t_cstr_range>::p_;

  using t_cstr_crange = t_crange<t_char, t_cstr_range_tag_>;
  using T_cstr_crange = t_prefix<t_cstr_crange>::T_;
  using R_cstr_crange = t_prefix<t_cstr_crange>::R_;
  using r_cstr_crange = t_prefix<t_cstr_crange>::r_;
  using P_cstr_crange = t_prefix<t_cstr_crange>::P_;
  using p_cstr_crange = t_prefix<t_cstr_crange>::p_;

///////////////////////////////////////////////////////////////////////////////

  template<typename T, typename TAG>
  constexpr
  typename t_range<T, TAG>::p_item begin(t_range<T, TAG> range) noexcept {
    return range.ptr;
  }

  template<typename T, typename TAG>
  constexpr
  typename t_range<T, TAG>::p_item end(t_range<T, TAG> range) noexcept {
    return begin(range) + get(range.n);
  }

  template<typename T, typename TAG>
  constexpr
  typename t_range<T, TAG>::P_item cbegin(t_range<T, TAG> range) noexcept {
    return range.ptr;
  }

  template<typename T, typename TAG>
  constexpr
  typename t_range<T, TAG>::P_item cend(t_range<T, TAG> range) noexcept {
    return begin(range) + get(range.n);
  }

///////////////////////////////////////////////////////////////////////////////

  template<typename T, typename TAG>
  constexpr
  typename t_crange<T, TAG>::P_item begin(t_crange<T, TAG> range) noexcept {
    return range.ptr;
  }

  template<typename T, typename TAG>
  constexpr
  typename t_crange<T, TAG>::P_item end(t_crange<T, TAG> range) noexcept {
    return begin(range) + get(range.n);
  }

  template<typename T, typename TAG>
  constexpr
  typename t_crange<T, TAG>::P_item cbegin(t_crange<T, TAG> range) noexcept {
    return range.ptr;
  }

  template<typename T, typename TAG>
  constexpr
  typename t_crange<T, TAG>::P_item cend(t_crange<T, TAG> range) noexcept {
    return begin(range) + get(range.n);
  }

///////////////////////////////////////////////////////////////////////////////

  template<typename T, typename TAG>
  constexpr
  t_bool operator==(t_crange<T, TAG> lh, t_crange<T, TAG> rh) noexcept {
    return is_equal_(lh.n, lh.ptr, rh.n, rh.ptr);
  }

  template<typename T, typename TAG>
  constexpr
  t_bool operator==(t_crange<T, TAG> lh, t_range<T, TAG> rh) noexcept {
    return lh == t_crange<T, TAG>{rh};
  }

  template<typename T, typename TAG>
  constexpr
  t_bool operator==(t_range<T, TAG> lh, t_crange<T, TAG> rh) noexcept {
    return t_crange<T, TAG>{lh} == rh;
  }

  template<typename T, typename TAG>
  constexpr
  t_bool operator==(t_range<T, TAG> lh, t_range<T, TAG> rh) noexcept {
    return t_crange<T, TAG>{lh} == t_crange<T, TAG>{rh};
  }

///////////////////////////////////////////////////////////////////////////////

  template<typename T, typename TAG>
  constexpr
  t_bool operator!=(t_crange<T, TAG> lh, t_crange<T, TAG> rh) noexcept {
    return !(lh == rh);
  }

  template<typename T, typename TAG>
  constexpr
  t_bool operator!=(t_crange<T, TAG> lh, t_range<T, TAG> rh) noexcept {
    return !(lh == rh);
  }

  template<typename T, typename TAG>
  constexpr
  t_bool operator!=(t_range<T, TAG> lh, t_crange<T, TAG> rh) noexcept {
    return !(lh == rh);
  }

  template<typename T, typename TAG>
  constexpr
  t_bool operator!=(t_range<T, TAG> lh, t_range<T, TAG> rh) noexcept {
    return !(lh == rh);
  }

///////////////////////////////////////////////////////////////////////////////

  template<typename TAG, typename T, t_n_ N>
  constexpr
  t_range<T, TAG> mk_range(T (&arr)[N]) noexcept {
    return {arr, t_n{N}, SKIP_};
  }

  template<typename TAG, typename T, t_n_ N>
  constexpr
  t_crange<T, TAG> mk_range(const T (&arr)[N]) noexcept {
    return {arr, t_n{N}, SKIP_};
  }

  template<typename TAG, typename T, t_n_ N>
  constexpr
  t_crange<T, TAG> mk_crange(T (&arr)[N]) noexcept {
    return {arr, t_n{N}, SKIP_};
  }

  template<typename TAG, typename T, t_n_ N>
  constexpr
  t_crange<T, TAG> mk_crange(const T (&arr)[N]) noexcept {
    return {arr, t_n{N}, SKIP_};
  }

  template<typename TAG, typename T, t_n_ N>
  constexpr
  t_range<T, TAG> mk_range(T (&arr)[N], t_begin_ix _begin) noexcept {
    auto begin = get(_begin);
    check_range_(N, begin);
    return {arr + begin, t_n{N - begin}, SKIP_};
  }

  template<typename TAG, typename T, t_n_ N>
  constexpr
  t_crange<T, TAG> mk_range(const T (&arr)[N], t_begin_ix _begin) noexcept {
    auto begin = get(_begin);
    check_range_(N, begin);
    return {arr + begin, t_n{N - begin}, SKIP_};
  }

  template<typename TAG, typename T, t_n_ N>
  constexpr
  t_crange<T, TAG> mk_crange(T (&arr)[N], t_begin_ix _begin) noexcept {
    auto begin = get(_begin);
    check_range_(N, begin);
    return {arr + begin, t_n{N - begin}, SKIP_};
  }

  template<typename TAG, typename T, t_n_ N>
  constexpr
  t_crange<T, TAG> mk_crange(const T (&arr)[N], t_begin_ix _begin) noexcept {
    auto begin = get(_begin);
    check_range_(N, begin);
    return {arr + begin, t_n{N - begin}, SKIP_};
  }

  template<typename TAG, typename T, t_n_ N>
  constexpr
  t_range<T, TAG> mk_range(T (&arr)[N], t_begin_ix _begin,
                                        t_end_ix   _end) noexcept {
    auto begin = get(_begin), end = get(_end);
    check_range_(N, begin, end);
    return {arr + begin, t_n{end - begin}, SKIP_};
  }

  template<typename TAG, typename T, t_n_ N>
  constexpr
  t_crange<T, TAG> mk_range(const T (&arr)[N], t_begin_ix _begin,
                                               t_end_ix   _end) noexcept {
    auto begin = get(_begin), end = get(_end);
    check_range_(N, begin, end);
    return {arr + begin, t_n{end - begin}, SKIP_};
  }

  template<typename TAG, typename T, t_n_ N>
  constexpr
  t_crange<T, TAG> mk_crange(T (&arr)[N], t_begin_ix _begin,
                                          t_end_ix   _end) noexcept {
    auto begin = get(_begin), end = get(_end);
    check_range_(N, begin, end);
    return {arr + begin, t_n{end - begin}, SKIP_};
  }

  template<typename TAG, typename T, t_n_ N>
  constexpr
  t_crange<T, TAG> mk_crange(const T (&arr)[N], t_begin_ix _begin,
                                                t_end_ix   _end) noexcept {
    auto begin = get(_begin), end = get(_end);
    check_range_(N, begin, end);
    return {arr + begin, t_n{end - begin}, SKIP_};
  }

  template<typename TAG, typename T, t_n_ N>
  constexpr
  t_crange<T, TAG> mk_crange(T (&arr)[N], t_n n) noexcept {
    // XXX check
    return {arr, n, SKIP_};
  }

  template<typename TAG, typename T, t_n_ N>
  constexpr
  t_crange<T, TAG> mk_crange(const T (&arr)[N], t_n n) noexcept {
    // XXX check
    return {arr, n, SKIP_};
  }

  template<typename TAG, typename T, t_n_ N>
  constexpr
  t_crange<T, TAG> mk_crange(T (&arr)[N], t_begin_ix _begin, t_n n) noexcept {
    auto begin = get(_begin);
    // XXX check
    return {arr + begin, n, SKIP_};
  }

  template<typename TAG, typename T, t_n_ N>
  constexpr
  t_crange<T, TAG> mk_crange(const T (&arr)[N], t_begin_ix _begin,
                                                t_n n) noexcept {
    auto begin = get(_begin);
    // XXX check
    return {arr + begin, n, SKIP_};
  }

///////////////////////////////////////////////////////////////////////////////

  template<typename TAG1, typename TAG, typename T>
  constexpr t_range<T, TAG1> mk_range(t_range<T, TAG> range) noexcept {
    return {range.ptr, range.n, SKIP_};
  }

  template<typename TAG1, typename TAG, typename T>
  constexpr t_crange<T, TAG1> mk_crange(t_crange<T, TAG> range) noexcept {
    return {range.ptr, range.n, SKIP_};
  }

  template<typename TAG1, typename TAG, typename T>
  constexpr
  t_range<T, TAG1> mk_range(t_range<T, TAG> range, t_begin_ix _begin) noexcept {
    auto begin = get(_begin), end = get(range.n);
    check_range_(end, begin);
    return {range.ptr + begin, t_n{end - begin}, SKIP_};
  }

  template<typename TAG1, typename TAG, typename T>
  constexpr
  t_crange<T, TAG1> mk_crange(t_crange<T, TAG> range,
                              t_begin_ix _begin) noexcept {
    auto begin = get(_begin), end = get(range.n);
    check_range_(end, begin);
    return {range.ptr + begin, t_n{end - begin}, SKIP_};
  }

  template<typename TAG1, typename TAG, typename T>
  constexpr
  t_range<T, TAG1> mk_range(t_range<T, TAG> range, t_begin_ix _begin,
                            t_end_ix _end) noexcept {
    auto begin = get(_begin), end = get(_end), n = get(range.n);
    check_range_(n, begin, end);
    return {range.ptr + begin, t_n{end - begin}, SKIP_};
  }

  template<typename TAG1, typename TAG, typename T>
  constexpr
  t_crange<T, TAG1> mk_crange(t_crange<T, TAG> range, t_begin_ix _begin,
                              t_end_ix _end) noexcept {
    auto begin = get(_begin), end = get(_end), n = get(range.n);
    check_range_(n, begin, end);
    return {range.ptr + begin, t_n{end - begin}, SKIP_};
  }

  template<typename TAG1, typename TAG, typename T>
  constexpr
  t_range<T, TAG1> mk_range(t_range<T, TAG> range, t_n n) noexcept {
    // XXX
     return {range.ptr, n};
  }

  template<typename TAG1, typename TAG, typename T>
  constexpr
  t_crange<T, TAG1> mk_crange(t_crange<T, TAG> range, t_n n) noexcept {
    // XXX
     return {range.ptr, n};
  }

  template<typename TAG1, typename TAG, typename T>
  constexpr
  t_range<T, TAG1> mk_range(t_range<T, TAG> range, t_begin_ix _begin,
                            t_n n) noexcept {
    auto begin = get(_begin);
    // XXX
     return {range.ptr + begin, n};
  }

  template<typename TAG1, typename TAG, typename T>
  constexpr
  t_crange<T, TAG1> mk_crange(t_crange<T, TAG> range, t_begin_ix _begin,
                              t_n n) noexcept {
    auto begin = get(_begin);
    // XXX
    return {range.ptr + begin, n};
  }

///////////////////////////////////////////////////////////////////////////////

  template<typename T, typename TAG>
  constexpr
  t_range<T, TAG>::t_range(p_item _ptr, t_n _n) noexcept : ptr{_ptr}, n{_n} {
    check_valid_(ptr, get(n));
  }

  template<typename T, typename TAG>
  constexpr
  t_range<T, TAG>::t_range(p_item _ptr, t_n _n, t_skip_) noexcept
    : ptr{_ptr}, n{_n} {
  }

  template<typename T, typename TAG>
  constexpr
  t_range<T, TAG>::operator t_validity() const noexcept {
    return ptr && get(n) ? VALID : INVALID;
  }

  template<typename T, typename TAG>
  template<typename TAG1>
  constexpr t_range<T, TAG1> t_range<T, TAG>::mk_range() noexcept {
    return range::mk_range<T, TAG1>(*this);
  }

  template<typename T, typename TAG>
  template<typename TAG1>
  constexpr t_range<T, TAG1> t_range<T, TAG>::
      mk_range(t_begin_ix begin) noexcept {
    return range::mk_range<T, TAG1>(*this, begin);
  }

  template<typename T, typename TAG>
  template<typename TAG1>
  constexpr t_range<T, TAG1> t_range<T, TAG>::
      mk_range(t_begin_ix begin, t_end_ix end) noexcept {
    return range::mk_range<T, TAG1>(*this, begin, end);
  }

  template<typename T, typename TAG>
  template<typename TAG1>
  constexpr t_range<T, TAG1> t_range<T, TAG>::mk_range(t_n n) noexcept {
    return range::mk_range<T, TAG1>(*this, n);
  }

  template<typename T, typename TAG>
  template<typename TAG1>
  constexpr t_range<T, TAG1> t_range<T, TAG>::mk_range(t_begin_ix begin,
                                                       t_n n) noexcept {
    return range::mk_range<T, TAG1>(*this, begin, n);
  }

  template<typename T, typename TAG>
  template<typename TAG1>
  constexpr t_crange<T, TAG1> t_range<T, TAG>::mk_crange() const noexcept {
    return range::mk_crange<T, TAG1>(*this);
  }

  template<typename T, typename TAG>
  template<typename TAG1>
  constexpr t_crange<T, TAG1> t_range<T, TAG>::
      mk_crange(t_begin_ix begin) const noexcept {
    return range::mk_crange<T, TAG1>(*this, begin);
  }

  template<typename T, typename TAG>
  template<typename TAG1>
  constexpr t_crange<T, TAG1> t_range<T, TAG>::
      mk_crange(t_begin_ix begin, t_end_ix end) const noexcept {
    return range::mk_range<T, TAG1>(*this, begin, end);
  }

  template<typename T, typename TAG>
  template<typename TAG1>
  constexpr t_crange<T, TAG1> t_range<T, TAG>::mk_crange(t_n n) const noexcept {
    return range::mk_crange<T, TAG1>(*this, n);
  }

  template<typename T, typename TAG>
  template<typename TAG1>
  constexpr t_crange<T, TAG1> t_range<T, TAG>::mk_crange(t_begin_ix begin,
                                                         t_n n) const noexcept {
    return range::mk_crange<T, TAG1>(*this, begin, n);
  }

  template<typename T, typename TAG>
  inline
  t_range<T, TAG>&
      t_range<T, TAG>::copy(const t_range<T, TAG>& range) noexcept {
    copy_(n, ptr, range.n, range.ptr);
    return *this;
  }

  template<typename T, typename TAG>
  inline
  t_range<T, TAG>&
      t_range<T, TAG>::copy(const t_crange<T, TAG>& range) noexcept {
    copy_(n, ptr, range.n, range.ptr);
    return *this;
  }

  template<typename T, typename TAG>
  inline
  t_range<T, TAG>& t_range<T, TAG>::
      copy(const t_range <T, TAG>& range, t_begin_ix begin) noexcept {
    auto tmp = range.mk_crange(begin);
    copy_(n, ptr, tmp.n, tmp.ptr);
    return *this;
  }

  template<typename T, typename TAG>
  inline
  t_range<T, TAG>& t_range<T, TAG>::
      copy(const t_crange<T, TAG>& range, t_begin_ix begin) noexcept {
    auto tmp = range.mk_crange(begin);
    copy_(n, ptr, tmp.n, tmp.ptr);
    return *this;
  }

  template<typename T, typename TAG>
  inline
  t_range<T, TAG>& t_range<T, TAG>::
      copy(const t_range<T, TAG>& range, t_begin_ix begin,
                                         t_end_ix   end) noexcept {
    auto tmp = range.mk_crange(begin, end);
    copy_(n, ptr, tmp.n, tmp.ptr);
    return *this;
  }

  template<typename T, typename TAG>
  inline
  t_range<T, TAG>& t_range<T, TAG>::
      copy(const t_crange<T, TAG>& range, t_begin_ix begin,
                                          t_end_ix   end) noexcept {
    auto tmp = range.mk_crange(begin, end);
    copy_(n, ptr, tmp.n, tmp.ptr);
    return *this;
  }

  template<typename T, typename TAG>
  inline
  t_range<T, TAG>& t_range<T, TAG>::
      copy(const t_range<T, TAG>& range, t_n n) noexcept {
    auto tmp = range.mk_crange(n);
    copy_(n, ptr, tmp.n, tmp.ptr);
    return *this;
  }

  template<typename T, typename TAG>
  inline
  t_range<T, TAG>& t_range<T, TAG>::
      copy(const t_crange<T, TAG>& range, t_n n) noexcept {
    auto tmp = range.mk_crange(n);
    copy_(n, ptr, tmp.n, tmp.ptr);
    return *this;
  }

  template<typename T, typename TAG>
  inline
  t_range<T, TAG>& t_range<T, TAG>::
      copy(const t_range<T, TAG>& range, t_begin_ix begin, t_n n) noexcept {
    auto tmp = range.mk_crange(begin, n);
    copy_(n, ptr, tmp.n, tmp.ptr);
    return *this;
  }

  template<typename T, typename TAG>
  inline
  t_range<T, TAG>& t_range<T, TAG>::
      copy(const t_crange<T, TAG>& range, t_begin_ix begin, t_n n) noexcept {
    auto tmp = range.mk_crange(begin, n);
    copy_(n, ptr, tmp.n, tmp.ptr);
    return *this;
  }

  template<typename T, typename TAG>
  inline
  typename t_range<T, TAG>::r_item
      t_range<T, TAG>::operator[](t_ix _ix) noexcept {
    auto curr_n = get(n), ix = get(_ix);
    check_range_(curr_n, ix);
    return ptr[ix];
  }

  template<typename T, typename TAG>
  constexpr
  typename t_range<T, TAG>::R_item
      t_range<T, TAG>::operator[](t_ix _ix) const noexcept {
    auto curr_n = get(n), ix = get(_ix);
    check_range_(curr_n, ix);
    return ptr[ix];
  }

  template<typename T, typename TAG>
  inline
  typename t_range<T, TAG>::p_item
      t_range<T, TAG>::address(t_ix _ix) noexcept {
    auto curr_n = get(n), ix = get(_ix);
    check_range_(curr_n, ix);
    return ptr + ix;
  }

  template<typename T, typename TAG>
  constexpr
  typename t_range<T, TAG>::P_item
      t_range<T, TAG>::address(t_ix _ix) const noexcept {
    auto curr_n = get(n), ix = get(_ix);
    check_range_(curr_n, ix);
    return ptr + ix;
  }

  template<typename T, typename TAG>
  template<typename F>
  inline
  t_void t_range<T, TAG>::each(F f) {
    for (auto&& i : *this)
      f(i);
  }

  template<typename T, typename TAG>
  template<typename F>
  inline
  t_void t_range<T, TAG>::each(F f) const {
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
  constexpr
  t_crange<T, TAG>::t_crange(P_item _ptr, t_n _n) noexcept : ptr{_ptr}, n{_n} {
    check_valid_(ptr, get(n));
  }

  template<typename T, typename TAG>
  constexpr
  t_crange<T, TAG>::t_crange(P_item _ptr, t_n _n, t_skip_) noexcept
    : ptr{_ptr}, n{_n} {
  }

  template<typename T, typename TAG>
  constexpr
  t_crange<T, TAG>::t_crange(const t_range<T, TAG>& range) noexcept
    : ptr{range.ptr}, n{range.n} {
  }

  template<typename T, typename TAG>
  constexpr
  t_crange<T, TAG>::operator t_validity() const noexcept {
    return ptr && get(n) ? VALID : INVALID;
  }

  template<typename T, typename TAG>
  template<typename TAG1>
  constexpr t_crange<T, TAG1> t_crange<T, TAG>::mk_crange() const noexcept {
    return range::mk_crange<T, TAG1>(*this);
  }

  template<typename T, typename TAG>
  template<typename TAG1>
  constexpr t_crange<T, TAG1> t_crange<T, TAG>::
      mk_crange(t_begin_ix begin) const noexcept {
    return range::mk_crange<T, TAG1>(*this, begin);
  }

  template<typename T, typename TAG>
  template<typename TAG1>
  constexpr t_crange<T, TAG1> t_crange<T, TAG>::
      mk_crange(t_begin_ix begin, t_end_ix end) const noexcept {
    return range::mk_range<T, TAG1>(*this, begin, end);
  }

  template<typename T, typename TAG>
  template<typename TAG1>
  constexpr t_crange<T, TAG1> t_crange<T, TAG>::
      mk_crange(t_n n) const noexcept {
    return range::mk_crange<T, TAG1>(*this, n);
  }

  template<typename T, typename TAG>
  template<typename TAG1>
  constexpr t_crange<T, TAG1> t_crange<T, TAG>::
      mk_crange(t_begin_ix begin, t_n n) const noexcept {
    return range::mk_crange<T, TAG1>(*this, begin, n);
  }

  template<typename T, typename TAG>
  constexpr
  typename t_crange<T, TAG>::R_item
      t_crange<T, TAG>::operator[](t_ix _ix) const noexcept {
    auto curr_n = get(n), ix = get(_ix);
    check_range_(curr_n, ix);
    return ptr[ix];
  }

  template<typename T, typename TAG>
  constexpr
  typename t_crange<T, TAG>::P_item
      t_crange<T, TAG>::address(t_ix _ix) const noexcept {
    auto curr_n = get(n), ix = get(_ix);
    check_range_(curr_n, ix);
    return ptr + ix;
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
