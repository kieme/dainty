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

#include "dainty_base_util.h"
#include "impl_/dainty_base_range_impl.h"

namespace dainty
{
namespace base
{
namespace range
{
  /////////////////////////////////////////////////////////////////////////////

  using util::t_block;

  using types::R_prefix;
  using types::P_prefix;

  using impl_::t_bool;
  using impl_::t_void;
  using impl_::t_n_;
  using impl_::t_ix_;
  using impl_::t_byte_;
  using impl_::t_char;
  using impl_::P_void;
  using impl_::t_n;
  using impl_::T_n;
  using impl_::t_ix;
  using impl_::t_ix_begin;
  using impl_::t_ix_end;
  using impl_::t_validity;
  using impl_::VALID;
  using impl_::INVALID;
  using impl_::assert_now;

  /////////////////////////////////////////////////////////////////////////////

  enum t_skip_ { SKIP_ };

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename TAG> class t_range;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename TAG>
  class t_Range : private TAG {
  public:
    using t_tag  = TAG;
    using t_item = T;
    using R_item = R_prefix_of<T>;
    using P_item = P_prefix_of<T>;

    constexpr t_Range()                       noexcept = default;
    constexpr t_Range(const t_Range<T, TAG>&) noexcept = default;
    constexpr t_Range(P_item, t_n)            noexcept;
    constexpr t_Range(P_item, t_n, t_skip_)   noexcept;
    constexpr t_Range(const t_range<T, TAG>&) noexcept;

    constexpr t_Range& operator=(const t_Range<T, TAG>&) noexcept = default;

    constexpr operator t_validity() const noexcept;

    template<typename TAG1 = TAG>
    constexpr t_Range<T, TAG1> mk_Range()                     const noexcept;
    template<typename TAG1 = TAG>
    constexpr t_Range<T, TAG1> mk_Range(t_ix_begin)           const noexcept;
    template<typename TAG1 = TAG>
    constexpr t_Range<T, TAG1> mk_Range(t_ix_begin, t_ix_end) const noexcept;
    template<typename TAG1 = TAG>
    constexpr t_Range<T, TAG1> mk_Range(t_n)                  const noexcept;
    template<typename TAG1 = TAG>
    constexpr t_Range<T, TAG1> mk_Range(t_ix_begin, t_n)      const noexcept;

    constexpr R_item operator[](t_ix) const noexcept;
    constexpr P_item address   (t_ix) const noexcept;

    template<typename F> t_void each(F&&);
    template<typename F> t_void each(F&&) const;
    template<typename F> t_void Each(F&&) const;

    /* one_of             */
    /* any_of             */
    /* is_of              */
    /* first_ix           */
    /* last_ix            */
    /* is_ix              */
    /* first              */
    /* last               */
    /* find               */
    /* reverse_find       */
    /* find range         */
    /* reverse_find range */

    P_item const ptr = nullptr;
    T_n          n   = t_n{0};
  };

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename TAG>
  class t_range : private TAG {
  public:
    using t_tag  = TAG;
    using t_item = T;
    using r_item = r_prefix_of<T>;
    using R_item = R_prefix_of<T>;
    using p_item = p_prefix_of<T>;
    using P_item = P_prefix_of<T>;

    constexpr t_range()                          noexcept = default;
    constexpr t_range(const t_range&)            noexcept = default;
    constexpr t_range(p_item, t_n)               noexcept;
    constexpr t_range(p_item, t_n, t_skip_)      noexcept;

    constexpr t_range& operator=(const t_range&) noexcept = default;

    ///////////////////////////////////////////////////////////////////////////

    constexpr operator t_validity() const noexcept;

    ///////////////////////////////////////////////////////////////////////////

    template<typename TAG1 = TAG>
    constexpr t_range<T, TAG1> mk_range()                     const noexcept;
    template<typename TAG1 = TAG>
    constexpr t_range<T, TAG1> mk_range(t_ix_begin)           const noexcept;
    template<typename TAG1 = TAG>
    constexpr t_range<T, TAG1> mk_range(t_ix_begin, t_ix_end) const noexcept;
    template<typename TAG1 = TAG>
    constexpr t_range<T, TAG1> mk_range(t_n)                  const noexcept;
    template<typename TAG1 = TAG>
    constexpr t_range<T, TAG1> mk_range(t_ix_begin, t_n)      const noexcept;

    template<typename TAG1 = TAG>
    constexpr t_Range<T, TAG1> mk_Range()                     const noexcept;
    template<typename TAG1 = TAG>
    constexpr t_Range<T, TAG1> mk_Range(t_ix_begin)           const noexcept;
    template<typename TAG1 = TAG>
    constexpr t_Range<T, TAG1> mk_Range(t_ix_begin, t_ix_end) const noexcept;
    template<typename TAG1 = TAG>
    constexpr t_Range<T, TAG1> mk_Range(t_n)                  const noexcept;
    template<typename TAG1 = TAG>
    constexpr t_Range<T, TAG1> mk_Range(t_ix_begin, t_n)      const noexcept;

    ///////////////////////////////////////////////////////////////////////////

              r_item operator[](t_ix)       noexcept;
    constexpr R_item operator[](t_ix) const noexcept;

              p_item address(t_ix)       noexcept;
    constexpr P_item address(t_ix) const noexcept;

    ///////////////////////////////////////////////////////////////////////////

    r_range copy(const t_range<T, TAG>&) noexcept;
    r_range copy(const t_Range<T, TAG>&) noexcept;

    r_range copy(const t_range<T, TAG>&, t_ix_begin) noexcept;
    r_range copy(const t_Range<T, TAG>&, t_ix_begin) noexcept;

    r_range copy(const t_range<T, TAG>&, t_ix_begin, t_ix_end) noexcept;
    r_range copy(const t_Range<T, TAG>&, t_ix_begin, t_ix_end) noexcept;

    r_range copy(const t_range<T, TAG>&, t_n) noexcept;
    r_range copy(const t_Range<T, TAG>&, t_n) noexcept;

    r_range copy(const t_range<T, TAG>&, t_ix_begin, t_n) noexcept;
    r_range copy(const t_Range<T, TAG>&, t_ix_begin, t_n) noexcept;

    ///////////////////////////////////////////////////////////////////////////

    template<typename TAG1 = TAG>
    r_range copy(t_block<T, TAG1>) noexcept;

    template<typename TAG1 = TAG>
    r_range copy(t_block<T, TAG1>, t_ix_begin) noexcept;

    ///////////////////////////////////////////////////////////////////////////

    r_range move(t_range <T, TAG>&)                       noexcept;
    r_range move(t_range <T, TAG>&, t_ix_begin)           noexcept;
    r_range move(t_range <T, TAG>&, t_ix_begin, t_ix_end) noexcept;
    r_range move(t_range <T, TAG>&, t_n)                  noexcept;
    r_range move(t_range <T, TAG>&, t_ix_begin, t_n)      noexcept;

    ///////////////////////////////////////////////////////////////////////////

    r_range swap(t_range <T, TAG>&)                       noexcept;
    r_range swap(t_range <T, TAG>&, t_ix_begin)           noexcept;
    r_range swap(t_range <T, TAG>&, t_ix_begin, t_ix_end) noexcept;
    r_range swap(t_range <T, TAG>&, t_n)                  noexcept;
    r_range swap(t_range <T, TAG>&, t_ix_begin, t_n)      noexcept;

    ///////////////////////////////////////////////////////////////////////////

    template<typename F> t_void each(F&&);
    template<typename F> t_void each(F&&) const;
    template<typename F> t_void Each(F&&) const;

    ///////////////////////////////////////////////////////////////////////////

    /* one_of             */
    /* any_of             */
    /* is_of              */
    /* first_ix           */
    /* last_ix            */
    /* is_ix              */
    /* first              */
    /* last               */
    /* find               */
    /* reverse_find       */
    /* find range         */
    /* reverse_find range */

  private:

  public:
    p_item const ptr = nullptr;
    T_n          n   = t_n{0};
  };

  /* must be a function */
  template<typename T, typename TAG> class t_range<const T, TAG>;

  /////////////////////////////////////////////////////////////////////////////

  enum  t_byte_range_tag_ {};
  using t_byte_range  = t_range<t_byte_, t_byte_range_tag_>;
  using T_byte_range  = t_prefix<t_byte_range>::T_;
  using R_byte_range  = t_prefix<t_byte_range>::R_;
  using r_byte_range  = t_prefix<t_byte_range>::r_;
  using P_byte_range  = t_prefix<t_byte_range>::P_;
  using p_byte_range  = t_prefix<t_byte_range>::p_;

  using t_byte_crange = t_Range<t_byte_, t_byte_range_tag_>;
  using T_byte_crange = t_prefix<t_byte_crange>::T_;
  using R_byte_crange = t_prefix<t_byte_crange>::R_;
  using r_byte_crange = t_prefix<t_byte_crange>::r_;
  using P_byte_crange = t_prefix<t_byte_crange>::P_;
  using p_byte_crange = t_prefix<t_byte_crange>::p_;

  /////////////////////////////////////////////////////////////////////////////

  enum  t_cstr_range_tag_ {};
  using t_cstr_range  = t_range<t_char, t_cstr_range_tag_>;
  using T_cstr_range  = t_prefix<t_cstr_range>::T_;
  using R_cstr_range  = t_prefix<t_cstr_range>::R_;
  using r_cstr_range  = t_prefix<t_cstr_range>::r_;
  using P_cstr_range  = t_prefix<t_cstr_range>::P_;
  using p_cstr_range  = t_prefix<t_cstr_range>::p_;

  using t_cstr_crange = t_Range<t_char, t_cstr_range_tag_>;
  using T_cstr_crange = t_prefix<t_cstr_crange>::T_;
  using R_cstr_crange = t_prefix<t_cstr_crange>::R_;
  using r_cstr_crange = t_prefix<t_cstr_crange>::r_;
  using P_cstr_crange = t_prefix<t_cstr_crange>::P_;
  using p_cstr_crange = t_prefix<t_cstr_crange>::p_;

  /////////////////////////////////////////////////////////////////////////////

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

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename TAG>
  constexpr
  typename t_Range<T, TAG>::P_item begin(t_Range<T, TAG> range) noexcept {
    return range.ptr;
  }

  template<typename T, typename TAG>
  constexpr
  typename t_Range<T, TAG>::P_item end(t_Range<T, TAG> range) noexcept {
    return begin(range) + get(range.n);
  }

  template<typename T, typename TAG>
  constexpr
  typename t_Range<T, TAG>::P_item cbegin(t_Range<T, TAG> range) noexcept {
    return range.ptr;
  }

  template<typename T, typename TAG>
  constexpr
  typename t_Range<T, TAG>::P_item cend(t_Range<T, TAG> range) noexcept {
    return begin(range) + get(range.n);
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename TAG>
  constexpr
  t_bool operator==(t_Range<T, TAG> lh, t_Range<T, TAG> rh) noexcept {
    return impl_::is_equal_(lh.n, lh.ptr, rh.n, rh.ptr);
  }

  template<typename T, typename TAG>
  constexpr
  t_bool operator==(t_Range<T, TAG> lh, t_range<T, TAG> rh) noexcept {
    return lh == t_Range<T, TAG>{rh};
  }

  template<typename T, typename TAG>
  constexpr
  t_bool operator==(t_range<T, TAG> lh, t_Range<T, TAG> rh) noexcept {
    return t_Range<T, TAG>{lh} == rh;
  }

  template<typename T, typename TAG>
  constexpr
  t_bool operator==(t_range<T, TAG> lh, t_range<T, TAG> rh) noexcept {
    return t_Range<T, TAG>{lh} == t_Range<T, TAG>{rh};
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename TAG>
  constexpr
  t_bool operator!=(t_Range<T, TAG> lh, t_Range<T, TAG> rh) noexcept {
    return !(lh == rh);
  }

  template<typename T, typename TAG>
  constexpr
  t_bool operator!=(t_Range<T, TAG> lh, t_range<T, TAG> rh) noexcept {
    return !(lh == rh);
  }

  template<typename T, typename TAG>
  constexpr
  t_bool operator!=(t_range<T, TAG> lh, t_Range<T, TAG> rh) noexcept {
    return !(lh == rh);
  }

  template<typename T, typename TAG>
  constexpr
  t_bool operator!=(t_range<T, TAG> lh, t_range<T, TAG> rh) noexcept {
    return !(lh == rh);
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename TAG1, typename T, typename TAG>
  constexpr
  t_range<T, TAG1> mk_range(t_range<T, TAG> range) noexcept {
    return {range.ptr, range.n, SKIP_};
  }

  template<typename TAG1, typename T, typename TAG>
  constexpr
  t_Range<T, TAG1> mk_range(t_Range<T, TAG> range) noexcept {
    return {range.ptr, range.n, SKIP_};
  }

  template<typename TAG1, typename T, typename TAG>
  constexpr
  t_Range<T, TAG1> mk_Range(t_Range<T, TAG> range) noexcept {
    return {range.ptr, range.n, SKIP_};
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename TAG>
  constexpr
  t_range<T, TAG> mk_range(t_range<T, TAG> range, t_ix_begin _begin,
                           t_ix_end _end) noexcept {
    auto begin = get(_begin), end = get(_end), n = get(range.n);
    impl_::check_range_(n, begin, end);
    return {range.ptr + begin, t_n{end - begin}, SKIP_};
  }

  template<typename T, typename TAG>
  constexpr
  t_Range<T, TAG> mk_range(t_Range<T, TAG> range, t_ix_begin _begin,
                            t_ix_end _end) noexcept {
    auto begin = get(_begin), end = get(_end), n = get(range.n);
    impl_::check_range_(n, begin, end);
    return {range.ptr + begin, t_n{end - begin}, SKIP_};
  }

  template<typename T, typename TAG>
  constexpr
  t_Range<T, TAG> mk_Range(t_Range<T, TAG> range, t_ix_begin _begin,
                             t_ix_end _end) noexcept {
    auto begin = get(_begin), end = get(_end), n = get(range.n);
    impl_::check_range_(n, begin, end);
    return {range.ptr + begin, t_n{end - begin}, SKIP_};
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename TAG1, typename T, typename TAG>
  constexpr
  t_range<T, TAG1> mk_range(t_range<T, TAG> range, t_ix_begin _begin,
                            t_ix_end _end) noexcept {
    auto begin = get(_begin), end = get(_end), n = get(range.n);
    impl_::check_range_(n, begin, end);
    return {range.ptr + begin, t_n{end - begin}, SKIP_};
  }

  template<typename TAG1, typename T, typename TAG>
  constexpr
  t_Range<T, TAG1> mk_range(t_Range<T, TAG> range, t_ix_begin _begin,
                             t_ix_end _end) noexcept {
    auto begin = get(_begin), end = get(_end), n = get(range.n);
    impl_::check_range_(n, begin, end);
    return {range.ptr + begin, t_n{end - begin}, SKIP_};
  }

  template<typename TAG1, typename T, typename TAG>
  constexpr
  t_Range<T, TAG1> mk_Range(t_Range<T, TAG> range, t_ix_begin _begin,
                              t_ix_end _end) noexcept {
    auto begin = get(_begin), end = get(_end), n = get(range.n);
    impl_::check_range_(n, begin, end);
    return {range.ptr + begin, t_n{end - begin}, SKIP_};
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename TAG>
  constexpr
  t_range<T, TAG> mk_range(t_range<T, TAG> range, t_ix_begin begin) noexcept {
    return mk_range(range, begin, t_ix_end{get(range.n)});
  }

  template<typename T, typename TAG>
  constexpr
  t_Range<T, TAG> mk_range(t_Range<T, TAG> range,
                            t_ix_begin begin) noexcept {
    return mk_Range(range, begin, t_ix_end{get(range.n)});
  }

  template<typename T, typename TAG>
  constexpr
  t_Range<T, TAG> mk_Range(t_Range<T, TAG> range,
                             t_ix_begin begin) noexcept {
    return mk_Range(range, begin, t_ix_end{get(range.n)});
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename TAG1, typename T, typename TAG>
  constexpr
  t_range<T, TAG1> mk_range(t_range<T, TAG> range,
                            t_ix_begin begin) noexcept {
    return mk_range<TAG1>(range, begin, t_ix_end{get(range.n)});
  }

  template<typename TAG1, typename T, typename TAG>
  constexpr
  t_Range<T, TAG1> mk_range(t_Range<T, TAG> range,
                             t_ix_begin begin) noexcept {
    return mk_Range<TAG1>(range, begin, t_ix_end{get(range.n)});
  }

  template<typename TAG1, typename T, typename TAG>
  constexpr
  t_Range<T, TAG1> mk_Range(t_Range<T, TAG> range,
                              t_ix_begin begin) noexcept {
    return mk_Range<TAG1>(range, begin, t_ix_end{get(range.n)});
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename TAG>
  constexpr
  t_range<T, TAG> mk_range(t_range<T, TAG> range, t_n n) noexcept {
    return mk_range(range, 0_begin_ix, t_ix_end(get(n)));
  }

  template<typename T, typename TAG>
  constexpr
  t_Range<T, TAG> mk_range(t_Range<T, TAG> range, t_n n) noexcept {
    return mk_Range(range, 0_begin_ix, t_ix_end(get(n)));
  }

  template<typename T, typename TAG>
  constexpr
  t_Range<T, TAG> mk_Range(t_Range<T, TAG> range, t_n n) noexcept {
    return mk_Range(range, 0_begin_ix, t_ix_end(get(n)));
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename TAG1, typename T, typename TAG>
  constexpr
  t_range<T, TAG1> mk_range(t_range<T, TAG> range, t_n n) noexcept {
    return mk_range<TAG1>(range, 0_begin_ix, t_ix_end(get(n)));
  }

  template<typename TAG1, typename T, typename TAG>
  constexpr
  t_Range<T, TAG1> mk_range(t_Range<T, TAG> range, t_n n) noexcept {
    return mk_Range<TAG1>(range, 0_begin_ix, t_ix_end(get(n)));
  }

  template<typename TAG1, typename T, typename TAG>
  constexpr
  t_Range<T, TAG1> mk_Range(t_Range<T, TAG> range, t_n n) noexcept {
    return mk_Range<TAG1>(range, 0_begin_ix, t_ix_end(get(n)));
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename TAG>
  constexpr
  t_range<T, TAG> mk_range(t_range<T, TAG> range, t_ix_begin begin,
                           t_n n) noexcept {
    return mk_range(range, begin, t_ix_end(get(begin) + get(n)));
  }

  template<typename T, typename TAG>
  constexpr
  t_Range<T, TAG> mk_range(t_Range<T, TAG> range, t_ix_begin begin,
                            t_n n) noexcept {
    return mk_Range(range, begin, t_ix_end(get(begin) + get(n)));
  }

  template<typename T, typename TAG>
  constexpr
  t_Range<T, TAG> mk_Range(t_Range<T, TAG> range, t_ix_begin begin,
                             t_n n) noexcept {
    return mk_Range(range, begin, t_ix_end(get(begin) + get(n)));
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename TAG1, typename T, typename TAG>
  constexpr
  t_range<T, TAG1> mk_range(t_range<T, TAG> range, t_ix_begin begin,
                            t_n n) noexcept {
    return mk_range<TAG1>(range, begin, t_ix_end(get(begin) + get(n)));
  }

  template<typename TAG1, typename T, typename TAG>
  constexpr
  t_Range<T, TAG1> mk_range(t_Range<T, TAG> range, t_ix_begin begin,
                             t_n n) noexcept {
    return mk_Range<TAG1>(range, begin, t_ix_end(get(begin) + get(n)));
  }

  template<typename TAG1, typename T, typename TAG>
  constexpr
  t_Range<T, TAG1> mk_Range(t_Range<T, TAG> range, t_ix_begin begin,
                              t_n n) noexcept {
    return mk_Range<TAG1>(range, begin, t_ix_end(get(begin) + get(n)));
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename TAG, typename T, t_n_ N>
  constexpr
  t_range<T, TAG> mk_range(T (&arr)[N]) noexcept {
    return {arr, t_n{N}, SKIP_};
  }

  template<typename TAG, typename T, t_n_ N>
  constexpr
  t_Range<T, TAG> mk_Range(T (&arr)[N]) noexcept {
    return {arr, t_n{N}, SKIP_};
  }

  template<typename TAG, typename T, t_n_ N>
  constexpr
  t_Range<T, TAG> mk_range(const T (&arr)[N]) noexcept {
    return {arr, t_n{N}, SKIP_};
  }

  template<typename TAG, typename T, t_n_ N>
  constexpr
  t_Range<T, TAG> mk_Range(const T (&arr)[N]) noexcept {
    return {arr, t_n{N}, SKIP_};
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename TAG, typename T, t_n_ N>
  constexpr
  t_range<T, TAG> mk_range(T (&arr)[N], t_ix_begin begin) noexcept {
    return mk_range<TAG>(mk_range<TAG>(arr), begin);
  }

  template<typename TAG, typename T, t_n_ N>
  constexpr
  t_Range<T, TAG> mk_Range(T (&arr)[N], t_ix_begin begin) noexcept {
    return mk_Range<TAG>(mk_Range<TAG>(arr), begin);
  }

  template<typename TAG, typename T, t_n_ N>
  constexpr
  t_Range<T, TAG> mk_range(const T (&arr)[N], t_ix_begin begin) noexcept {
    return mk_Range<TAG>(mk_Range<TAG>(arr), begin);
  }

  template<typename TAG, typename T, t_n_ N>
  constexpr
  t_Range<T, TAG> mk_Range(const T (&arr)[N], t_ix_begin begin) noexcept {
    return mk_Range<TAG>(mk_Range<TAG>(arr), begin);
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename TAG, typename T, t_n_ N>
  constexpr
  t_range<T, TAG> mk_range(T (&arr)[N], t_ix_begin begin,
                                        t_ix_end   end) noexcept {
    return mk_range<TAG>(mk_range<TAG>(arr), begin, end);
  }

  template<typename TAG, typename T, t_n_ N>
  constexpr
  t_Range<T, TAG> mk_Range(T (&arr)[N], t_ix_begin begin,
                                          t_ix_end   end) noexcept {
    return mk_Range<TAG>(mk_Range<TAG>(arr), begin, end);
  }

  template<typename TAG, typename T, t_n_ N>
  constexpr
  t_Range<T, TAG> mk_range(const T (&arr)[N], t_ix_begin begin,
                                               t_ix_end   end) noexcept {
    return mk_Range<TAG>(mk_Range<TAG>(arr), begin, end);
  }

  template<typename TAG, typename T, t_n_ N>
  constexpr
  t_Range<T, TAG> mk_Range(const T (&arr)[N], t_ix_begin begin,
                                                t_ix_end   end) noexcept {
    return mk_Range<TAG>(mk_Range<TAG>(arr), begin, end);
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename TAG, typename T, t_n_ N>
  constexpr
  t_range<T, TAG> mk_range(T (&arr)[N], t_n n) noexcept {
    return mk_range<TAG>(mk_range<TAG>(arr), 0_begin_ix, t_ix_end(get(n)));
  }

  template<typename TAG, typename T, t_n_ N>
  constexpr
  t_Range<T, TAG> mk_Range(T (&arr)[N], t_n n) noexcept {
    return mk_Range<TAG>(mk_Range<TAG>(arr), 0_begin_ix, t_ix_end(get(n)));
  }

  template<typename TAG, typename T, t_n_ N>
  constexpr
  t_Range<T, TAG> mk_range(const T (&arr)[N], t_n n) noexcept {
    return mk_Range<TAG>(mk_Range<TAG>(arr), 0_begin_ix, t_ix_end(get(n)));
  }

  template<typename TAG, typename T, t_n_ N>
  constexpr
  t_Range<T, TAG> mk_Range(const T (&arr)[N], t_n n) noexcept {
    return mk_Range<TAG>(mk_Range<TAG>(arr), 0_begin_ix, t_ix_end(get(n)));
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename TAG, typename T, t_n_ N>
  constexpr
  t_range<T, TAG> mk_range(T (&arr)[N], t_ix_begin begin, t_n n) noexcept {
    return mk_range<TAG>(mk_range<TAG>(arr), begin,
                         t_ix_end(get(begin) + get(n)));
  }

  template<typename TAG, typename T, t_n_ N>
  constexpr
  t_Range<T, TAG> mk_Range(T (&arr)[N], t_ix_begin begin, t_n n) noexcept {
    return mk_Range<TAG>(mk_Range<TAG>(arr), begin,
                          t_ix_end(get(begin) + get(n)));
  }

  template<typename TAG, typename T, t_n_ N>
  constexpr
  t_Range<T, TAG> mk_range(const T (&arr)[N], t_ix_begin begin,
                                               t_n n) noexcept {
    return mk_Range<TAG>(mk_Range<TAG>(arr), begin,
                          t_ix_end(get(begin) + get(n)));
  }

  template<typename TAG, typename T, t_n_ N>
  constexpr
  t_Range<T, TAG> mk_Range(const T (&arr)[N], t_ix_begin begin,
                                                t_n n) noexcept {
    return mk_Range<TAG>(mk_Range<TAG>(arr), begin,
                          t_ix_end(get(begin) + get(n)));
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename TAG>
  constexpr
  t_range<T, TAG>::t_range(p_item _ptr, t_n _n) noexcept : ptr{_ptr}, n{_n} {
    impl_::check_valid_(ptr, get(n));
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
  constexpr
  t_range<T, TAG1> t_range<T, TAG>::mk_range() noexcept const {
    return range::mk_range<TAG1>(*this);
  }

  template<typename T, typename TAG>
  template<typename TAG1>
  constexpr
  t_range<T, TAG1> t_range<T, TAG>::mk_range(t_ix_begin begin) noexcept const {
    return range::mk_range<TAG1>(*this, begin);
  }

  template<typename T, typename TAG>
  template<typename TAG1>
  constexpr
  t_range<T, TAG1> t_range<T, TAG>::mk_range(t_ix_begin begin,
                                             t_ix_end end) const noexcept {
    return range::mk_range<TAG1>(*this, begin, end);
  }

  template<typename T, typename TAG>
  template<typename TAG1>
  constexpr
  t_range<T, TAG1> t_range<T, TAG>::mk_range(t_n n) const noexcept {
    return range::mk_range<TAG1>(*this, n);
  }

  template<typename T, typename TAG>
  template<typename TAG1>
  constexpr
  t_range<T, TAG1> t_range<T, TAG>::mk_range(t_ix_begin begin,
                                             t_n n) const noexcept {
    return range::mk_range<TAG1>(*this, begin, n);
  }

  template<typename T, typename TAG>
  template<typename TAG1>
  constexpr
  t_Range<T, TAG1> t_range<T, TAG>::mk_Range() const noexcept {
    return range::mk_Range<TAG1>(*this);
  }

  template<typename T, typename TAG>
  template<typename TAG1>
  constexpr
  t_Range<T, TAG1> t_range<T, TAG>:: mk_Range(t_ix_begin begin) const noexcept {
    return range::mk_Range<TAG1>(*this, begin);
  }

  template<typename T, typename TAG>
  template<typename TAG1>
  constexpr
  t_Range<T, TAG1> t_range<T, TAG>::mk_Range(t_ix_begin begin,
                                             t_ix_end end) const noexcept {
    return range::mk_range<TAG1>(*this, begin, end);
  }

  template<typename T, typename TAG>
  template<typename TAG1>
  constexpr
  t_Range<T, TAG1> t_range<T, TAG>::mk_Range(t_n n) const noexcept {
    return range::mk_Range<TAG1>(*this, n);
  }

  template<typename T, typename TAG>
  template<typename TAG1>
  constexpr
  t_Range<T, TAG1> t_range<T, TAG>::mk_Range(t_ix_begin begin,
                                             t_n n) const noexcept {
    return range::mk_Range<TAG1>(*this, begin, n);
  }

  template<typename T, typename TAG>
  inline
  t_range<T, TAG>&
      t_range<T, TAG>::copy(const t_range<T, TAG>& range) noexcept {
    impl_::copy_(n, ptr, range.n, range.ptr);
    return *this;
  }

  template<typename T, typename TAG>
  inline
  t_range<T, TAG>&
      t_range<T, TAG>::copy(const t_Range<T, TAG>& range) noexcept {
    impl_::copy_(n, ptr, range.n, range.ptr);
    return *this;
  }

  template<typename T, typename TAG>
  inline
  t_range<T, TAG>& t_range<T, TAG>::
      copy(const t_range <T, TAG>& range, t_ix_begin begin) noexcept {
    auto tmp = range.mk_Range(begin);
    impl_::copy_(n, ptr, tmp.n, tmp.ptr);
    return *this;
  }

  template<typename T, typename TAG>
  inline
  t_range<T, TAG>& t_range<T, TAG>::
      copy(const t_Range<T, TAG>& range, t_ix_begin begin) noexcept {
    auto tmp = range.mk_Range(begin);
    impl_::copy_(n, ptr, tmp.n, tmp.ptr);
    return *this;
  }

  template<typename T, typename TAG>
  inline
  t_range<T, TAG>& t_range<T, TAG>::
      copy(const t_range<T, TAG>& range, t_ix_begin begin,
                                         t_ix_end   end) noexcept {
    auto tmp = range.mk_Range(begin, end);
    impl_::copy_(n, ptr, tmp.n, tmp.ptr);
    return *this;
  }

  template<typename T, typename TAG>
  inline
  t_range<T, TAG>& t_range<T, TAG>::
      copy(const t_Range<T, TAG>& range, t_ix_begin begin,
                                          t_ix_end   end) noexcept {
    auto tmp = range.mk_Range(begin, end);
    impl_::copy_(n, ptr, tmp.n, tmp.ptr);
    return *this;
  }

  template<typename T, typename TAG>
  inline
  t_range<T, TAG>& t_range<T, TAG>::
      copy(const t_range<T, TAG>& range, t_n n) noexcept {
    auto tmp = range.mk_Range(n);
    impl_::copy_(n, ptr, tmp.n, tmp.ptr);
    return *this;
  }

  template<typename T, typename TAG>
  inline
  t_range<T, TAG>& t_range<T, TAG>::copy(const t_Range<T, TAG>& range,
                                         t_n n) noexcept {
    auto tmp = range.mk_Range(n);
    impl_::copy_(n, ptr, tmp.n, tmp.ptr);
    return *this;
  }

  template<typename T, typename TAG>
  inline
  t_range<T, TAG>& t_range<T, TAG>::copy(const t_range<T, TAG>& range,
                                         t_ix_begin begin, t_n n) noexcept {
    auto tmp = range.mk_Range(begin, n);
    impl_::copy_(n, ptr, tmp.n, tmp.ptr);
    return *this;
  }

  template<typename T, typename TAG>
  inline
  t_range<T, TAG>& t_range<T, TAG>::copy(const t_Range<T, TAG>& range,
                                         t_ix_begin begin, t_n n) noexcept {
    auto tmp = range.mk_Range(begin, n);
    impl_::copy_(n, ptr, tmp.n, tmp.ptr);
    return *this;
  }

  template<typename T, typename TAG>
  template<typename TAG1>
  inline
  t_range<T, TAG>& t_range<T, TAG>::copy(t_block<T, TAG1> block) noexcept {
    impl_::copy_block_(n, ptr, block.n, block.value);
    return *this;
  }

  template<typename T, typename TAG>
  inline
  typename t_range<T, TAG>::r_item
      t_range<T, TAG>::operator[](t_ix ix) noexcept {
    return *impl_::access_(n, ptr, ix);
  }

  template<typename T, typename TAG>
  constexpr
  typename t_range<T, TAG>::R_item
      t_range<T, TAG>::operator[](t_ix ix) const noexcept {
    return *impl_::access_(n, ptr, ix);
  }

  template<typename T, typename TAG>
  inline
  typename t_range<T, TAG>::p_item
      t_range<T, TAG>::address(t_ix ix) noexcept {
    return impl_::access_(n, ptr, ix);
  }

  template<typename T, typename TAG>
  constexpr
  typename t_range<T, TAG>::P_item
      t_range<T, TAG>::address(t_ix ix) const noexcept {
    return impl_::access_(n, ptr, ix);
  }

  template<typename T, typename TAG>
  template<typename F>
  inline
  t_void t_range<T, TAG>::each(F&& func) {
    for (auto&& i : *this)
      func(i);
  }

  template<typename T, typename TAG>
  template<typename F>
  inline
  t_void t_range<T, TAG>::each(F&& func) const {
    for (auto&& i : *this)
      func(i);
  }

  template<typename T, typename TAG>
  template<typename F>
  inline
  t_void t_range<T, TAG>::Each(F&& func) const {
    for (auto&& i : *this)
      func(i);
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename TAG>
  constexpr
  t_Range<T, TAG>::t_Range(P_item _ptr, t_n _n) noexcept : ptr{_ptr}, n{_n} {
    impl_::check_valid_(ptr, get(n));
  }

  template<typename T, typename TAG>
  constexpr
  t_Range<T, TAG>::t_Range(P_item _ptr, t_n _n, t_skip_) noexcept
    : ptr{_ptr}, n{_n} {
  }

  template<typename T, typename TAG>
  constexpr
  t_Range<T, TAG>::t_Range(const t_range<T, TAG>& range) noexcept
    : ptr{range.ptr}, n{range.n} {
  }

  template<typename T, typename TAG>
  constexpr
  t_Range<T, TAG>::operator t_validity() const noexcept {
    return ptr && get(n) ? VALID : INVALID;
  }

  template<typename T, typename TAG>
  template<typename TAG1>
  constexpr
  t_Range<T, TAG1> t_Range<T, TAG>::mk_Range() const noexcept {
    return range::mk_Range<TAG1>(*this);
  }

  template<typename T, typename TAG>
  template<typename TAG1>
  constexpr
  t_Range<T, TAG1> t_Range<T, TAG>::
      mk_Range(t_ix_begin begin) const noexcept {
    return range::mk_Range<TAG1>(*this, begin);
  }

  template<typename T, typename TAG>
  template<typename TAG1>
  constexpr
  t_Range<T, TAG1> t_Range<T, TAG>::
      mk_Range(t_ix_begin begin, t_ix_end end) const noexcept {
    return range::mk_range<TAG1>(*this, begin, end);
  }

  template<typename T, typename TAG>
  template<typename TAG1>
  constexpr
  t_Range<T, TAG1> t_Range<T, TAG>::
      mk_Range(t_n n) const noexcept {
    return range::mk_Range<TAG1>(*this, n);
  }

  template<typename T, typename TAG>
  template<typename TAG1>
  constexpr
  t_Range<T, TAG1> t_Range<T, TAG>::
      mk_Range(t_ix_begin begin, t_n n) const noexcept {
    return range::mk_Range<TAG1>(*this, begin, n);
  }

  template<typename T, typename TAG>
  constexpr
  typename t_Range<T, TAG>::R_item
      t_Range<T, TAG>::operator[](t_ix ix) const noexcept {
    return *impl_::access_(n, ptr, ix);
  }

  template<typename T, typename TAG>
  constexpr
  typename t_Range<T, TAG>::P_item
      t_Range<T, TAG>::address(t_ix ix) const noexcept {
    return impl_::access_(n, ptr, ix);
  }

  template<typename T, typename TAG>
  template<typename F>
  inline
  t_void t_Range<T, TAG>::each(F&& func) {
    for (auto&& i : *this)
      func(i);
  }

  template<typename T, typename TAG>
  template<typename F>
  inline
  t_void t_Range<T, TAG>::each(F&& func) const {
    for (auto&& i : *this)
      func(i);
  }

  template<typename T, typename TAG>
  template<typename F>
  inline
  t_void t_Range<T, TAG>::Each(F&& func) const {
    for (auto&& i : *this)
      func(i);
  }

  /////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
