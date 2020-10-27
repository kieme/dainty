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

#ifndef _DAINTY_BASE_LOGICAL_H_
#define _DAINTY_BASE_LOGICAL_H_

#include "dainty_base_logical_impl.h"

namespace dainty
{
namespace base
{
namespace logical
{
  /////////////////////////////////////////////////////////////////////////////

  using types::t_n_;
  using types::t_ix_;
  using types::t_8u_;
  using types::t_16u_;
  using types::t_32u_;
  using types::t_64u_;
  using types::t_u_;
  using types::t_8i_;
  using types::t_16i_;
  using types::t_32i_;
  using types::t_64i_;
  using types::t_i_;
  using types::t_int;
  using types::t_pack;
  using types::t_bool;

  using types::t_op_less_tag;
  using types::t_op_less_equal_tag;
  using types::t_op_larger_tag;
  using types::t_op_larger_equal_tag;
  using types::t_op_equal_tag;
  using types::t_op_not_equal_tag;
  using types::t_op_plus_tag;
  using types::t_op_minus_tag;
  using types::t_op_multiply_tag;
  using types::t_op_divide_tag;
  using types::t_op_mod_tag;
  using types::t_op_self_plus_tag;
  using types::t_op_self_minus_tag;
  using types::t_op_self_multiply_tag;
  using types::t_op_self_divide_tag;
  using types::t_op_self_mod_tag;
  using types::t_op_and_tag;
  using types::t_op_or_tag;
  using types::t_op_not_tag;
  using types::t_op_negate_tag;
  using types::t_op_incr_tag;
  using types::t_op_decr_tag;
  using types::t_op_post_incr_tag;
  using types::t_op_post_decr_tag;
  using types::t_op_bin_xor_tag;
  using types::t_op_bin_and_tag;
  using types::t_op_bin_ior_tag;
  using types::t_op_bin_lshift_tag;
  using types::t_op_bin_rshift_tag;
  using types::t_op_bin_self_xor_tag;
  using types::t_op_bin_self_and_tag;
  using types::t_op_bin_self_ior_tag;
  using types::t_op_bin_self_lshift_tag;
  using types::t_op_bin_self_rshift_tag;
  using types::t_ops_compare_tag;
  using types::t_ops_arithmetic_tag;
  using types::t_ops_arithmetic_self_tag;
  using types::t_ops_or_and_tag;
  using types::t_ops_bin_tag;
  using types::t_ops_value_tag;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename TAG, typename... Ls>
  using t_logical = impl_::t_logical<T, TAG, Ls...>;

  /////////////////////////////////////////////////////////////////////////////

  template<class T, class TAG, class... Ls>
  constexpr
  T get(t_logical<T, TAG, Ls...> logical) noexcept {
    return impl_::get_(logical);
  }

  template<class T, class TAG, class... Ls>
  constexpr
  t_logical<T, TAG, Ls...>& set(t_logical<T, TAG, Ls...>& logical,
                                T value) noexcept {
    return impl_::set_(logical, value);
  }

  template<class T, class TAG, class... Ls, class T1, class TAG1, class... Ls1>
  constexpr
  t_logical<T, TAG, Ls...>& set(t_logical<T,  TAG,  Ls...>& logical,
                                t_logical<T1, TAG1, Ls1...> value) noexcept {
    return impl_::set_(logical, value);
  }

  /////////////////////////////////////////////////////////////////////////////

  template<class T> using t_is_logical     = impl_::t_is_logical<T>;
  template<class T> using t_is_not_logical = impl_::t_is_not_logical<T>;
  template<class T> using t_if_logical     = impl_::t_if_logical<T>;
  template<class T> using t_if_not_logical = impl_::t_if_not_logical<T>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T> using t_ops_of  = impl_::t_ops_of<T>;
  template<typename T> using t_tag_of  = impl_::t_tag_of<T>;
  template<typename T> using t_tags_of = impl_::t_tags_of<T>;

  /////////////////////////////////////////////////////////////////////////////

  struct t_u_tag_ {
    using t_ops = t_ops_value_tag;
  };
  using t_8u  = t_logical<t_8u_,  t_u_tag_>;
  using t_16u = t_logical<t_16u_, t_u_tag_>;
  using t_32u = t_logical<t_32u_, t_u_tag_>;
  using t_64u = t_logical<t_64u_, t_u_tag_>;

  using t_u   = t_64u;

  /////////////////////////////////////////////////////////////////////////////

  struct t_i_tag_ {
    using t_ops = t_ops_value_tag;
  };
  using t_8i  = t_logical<t_8i_,  t_i_tag_>;
  using t_16i = t_logical<t_16i_, t_i_tag_, t_8u>;
  using t_32i = t_logical<t_32i_, t_i_tag_, t_16u>;
  using t_64i = t_logical<t_64i_, t_i_tag_, t_32u>;

  using t_i   = t_64i;

  /////////////////////////////////////////////////////////////////////////////

  struct t_n_tag_ {
    using t_ops = t_ops_value_tag;
  };
  using t_8u_n  = t_logical<t_8u_,  t_n_tag_, t_8u>;
  using t_16u_n = t_logical<t_16u_, t_n_tag_, t_16u>;
  using t_32u_n = t_logical<t_32u_, t_n_tag_, t_32u>;
  using t_64u_n = t_logical<t_64u_, t_n_tag_, t_64u>;

  using t_8i_n  = t_logical<t_8i_,  t_n_tag_, t_8i>;
  using t_16i_n = t_logical<t_16i_, t_n_tag_, t_16i>;
  using t_32i_n = t_logical<t_32i_, t_n_tag_, t_32i>;
  using t_64i_n = t_logical<t_64i_, t_n_tag_, t_64i>;

  using t_n     = t_64u_n;

  /////////////////////////////////////////////////////////////////////////////

  struct t_n_max_tag_ {
    using t_ops = t_ops_value_tag;
  };
  using t_8u_n_max  = t_logical<t_8u_,  t_n_max_tag_, t_8u_n>;
  using t_16u_n_max = t_logical<t_16u_, t_n_max_tag_, t_16u_n>;
  using t_32u_n_max = t_logical<t_32u_, t_n_max_tag_, t_32u_n>;
  using t_64u_n_max = t_logical<t_64u_, t_n_max_tag_, t_64u_n>;

  using t_8i_n_max  = t_logical<t_8i_,  t_n_max_tag_, t_8i_n>;
  using t_16i_n_max = t_logical<t_16i_, t_n_max_tag_, t_16i_n>;
  using t_32i_n_max = t_logical<t_32i_, t_n_max_tag_, t_32i_n>;
  using t_64i_n_max = t_logical<t_64i_, t_n_max_tag_, t_64i_n>;

  using t_n_max     = t_64u_n_max;

  /////////////////////////////////////////////////////////////////////////////

  struct t_n_min_tag_ {
    using t_ops = t_ops_value_tag;
  };
  using t_8u_n_min  = t_logical<t_8u_,  t_n_min_tag_, t_8u_n>;
  using t_16u_n_min = t_logical<t_16u_, t_n_min_tag_, t_16u_n>;
  using t_32u_n_min = t_logical<t_32u_, t_n_min_tag_, t_32u_n>;
  using t_64u_n_min = t_logical<t_64u_, t_n_min_tag_, t_64u_n>;

  using t_8i_n_min  = t_logical<t_8i_,  t_n_min_tag_, t_8i_n>;
  using t_16i_n_min = t_logical<t_16i_, t_n_min_tag_, t_16i_n>;
  using t_32i_n_min = t_logical<t_32i_, t_n_min_tag_, t_32i_n>;
  using t_64i_n_min = t_logical<t_64i_, t_n_min_tag_, t_64i_n>;

  using t_n_min     = t_64u_n_min;

  /////////////////////////////////////////////////////////////////////////////

  struct t_ix_tag_ {
    using t_ops = t_ops_value_tag;
  };
  using t_8u_ix  = t_logical<t_8u_,  t_ix_tag_, t_8u_n>;
  using t_16u_ix = t_logical<t_16u_, t_ix_tag_, t_16u_n>;
  using t_32u_ix = t_logical<t_32u_, t_ix_tag_, t_32u_n>;
  using t_64u_ix = t_logical<t_64u_, t_ix_tag_, t_64u_n>;

  using t_8i_ix  = t_logical<t_8i_,  t_ix_tag_, t_8i_n>;
  using t_16i_ix = t_logical<t_16i_, t_ix_tag_, t_16i_n>;
  using t_32i_ix = t_logical<t_32i_, t_ix_tag_, t_32i_n>;
  using t_64i_ix = t_logical<t_64i_, t_ix_tag_, t_64i_n>;

  using t_ix     = t_64u_ix;

  /////////////////////////////////////////////////////////////////////////////

  struct t_ix_begin_tag_ {
    using t_ops = t_ops_value_tag;
  };
  using t_8u_ix_begin  = t_logical<t_8u_,  t_ix_begin_tag_, t_8u_ix>;
  using t_16u_ix_begin = t_logical<t_16u_, t_ix_begin_tag_, t_16u_ix>;
  using t_32u_ix_begin = t_logical<t_32u_, t_ix_begin_tag_, t_32u_ix>;
  using t_64u_ix_begin = t_logical<t_64u_, t_ix_begin_tag_, t_64u_ix>;

  using t_8i_ix_begin  = t_logical<t_8i_,  t_ix_begin_tag_, t_8i_ix>;
  using t_16i_ix_begin = t_logical<t_16i_, t_ix_begin_tag_, t_16i_ix>;
  using t_32i_ix_begin = t_logical<t_32i_, t_ix_begin_tag_, t_32i_ix>;
  using t_64i_ix_begin = t_logical<t_64i_, t_ix_begin_tag_, t_64i_ix>;

  using t_ix_begin     = t_64u_ix_begin;

  /////////////////////////////////////////////////////////////////////////////

  struct t_ix_end_tag_ {
    using t_ops = t_ops_value_tag;
  };
  using t_8u_ix_end  = t_logical<t_8u_,  t_ix_end_tag_, t_8u_ix>;
  using t_16u_ix_end = t_logical<t_16u_, t_ix_end_tag_, t_16u_ix>;
  using t_32u_ix_end = t_logical<t_32u_, t_ix_end_tag_, t_32u_ix>;
  using t_64u_ix_end = t_logical<t_64u_, t_ix_end_tag_, t_64u_ix>;

  using t_8i_ix_end  = t_logical<t_8i_,  t_ix_end_tag_, t_8i_ix>;
  using t_16i_ix_end = t_logical<t_16i_, t_ix_end_tag_, t_16i_ix>;
  using t_32i_ix_end = t_logical<t_32i_, t_ix_end_tag_, t_32i_ix>;
  using t_64i_ix_end = t_logical<t_64i_, t_ix_end_tag_, t_64i_ix>;

  using t_ix_end     = t_64u_ix_end;

  /////////////////////////////////////////////////////////////////////////////

  // max_of
  // min_of
  // avg_of

  /////////////////////////////////////////////////////////////////////////////

  constexpr
  t_n operator""_n(unsigned long long value) noexcept {
    return t_n{value};
  }

  constexpr
  t_n_min operator""_n_min(unsigned long long value) noexcept {
    return t_n_min{value};
  }

  constexpr
  t_n_max operator""_n_max(unsigned long long value) noexcept {
    return t_n_max{value};
  }

  constexpr
  t_ix operator""_ix(unsigned long long value) noexcept {
    return t_ix{value};
  }

  constexpr
  t_ix_begin operator""_ix_begin(unsigned long long value) noexcept {
    return t_ix_begin{value};
  }

  constexpr
  t_ix_end operator""_ix_end(unsigned long long value) noexcept {
    return t_ix_end{value};
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename T,  typename TAG,  typename... Ls,
           typename T1, typename TAG1, typename... L1s>
  constexpr
  t_bool operator==(t_logical<T,  TAG,  Ls...>  lh,
                    t_logical<T1, TAG1, L1s...> rh) noexcept {
    static_assert(impl_::t_is_op_<t_op_equal_tag,
                                  t_logical<T,  TAG,  Ls...>,
                                  t_logical<T1, TAG1, L1s...>>::VALUE,
                  "logical types not allowed to do equal operation");
    return get(lh) == get(rh);
  }

  template<typename T,  typename TAG,  typename... Ls,
           typename T1, typename TAG1, typename... L1s>
  constexpr
  t_bool operator!=(t_logical<T,  TAG,  Ls...>  lh,
                    t_logical<T1, TAG1, L1s...> rh) noexcept {
    static_assert(impl_::t_is_op_<t_op_not_equal_tag,
                                  t_logical<T,  TAG,  Ls...>,
                                  t_logical<T1, TAG1, L1s...>>::VALUE,
                  "logical types not allowed to do not_equal operation");
    return get(lh) != get(rh);
  }

  template<typename T,  typename TAG,  typename... Ls,
           typename T1, typename TAG1, typename... L1s>
  constexpr
  t_bool operator<(t_logical<T,  TAG,  Ls...>  lh,
                   t_logical<T1, TAG1, L1s...> rh) noexcept {
    static_assert(impl_::t_is_op_<t_op_less_tag,
                                  t_logical<T,  TAG,  Ls...>,
                                  t_logical<T1, TAG1, L1s...>>::VALUE,
                  "logical types not allowed to do less operation");
    return get(lh) < get(rh);
  }

  template<typename T,  typename TAG,  typename... Ls,
           typename T1, typename TAG1, typename... L1s>
  constexpr
  t_bool operator>(t_logical<T,  TAG,  Ls...>  lh,
                   t_logical<T1, TAG1, L1s...> rh) noexcept {
    static_assert(impl_::t_is_op_<t_op_larger_tag,
                                  t_logical<T,  TAG,  Ls...>,
                                  t_logical<T1, TAG1, L1s...>>::VALUE,
                  "logical types not allowed to do larger operation");
    return get(lh) > get(rh);
  }

  template<typename T,  typename TAG,  typename... Ls,
           typename T1, typename TAG1, typename... L1s>
  constexpr
  t_bool operator<=(t_logical<T,  TAG,  Ls...>  lh,
                    t_logical<T1, TAG1, L1s...> rh) noexcept {
    static_assert(impl_::t_is_op_<t_op_less_equal_tag,
                                  t_logical<T,  TAG,  Ls...>,
                                  t_logical<T1, TAG1, L1s...>>::VALUE,
                  "logical types not allowed to do less_equal operation");
    return get(lh) <= get(rh);
  }

  template<typename T,  typename TAG,  typename... Ls,
           typename T1, typename TAG1, typename... L1s>
  constexpr
  t_bool operator>=(t_logical<T,  TAG,  Ls...>  lh,
                    t_logical<T1, TAG1, L1s...> rh) noexcept {
    static_assert(impl_::t_is_op_<t_op_larger_equal_tag,
                                  t_logical<T,  TAG,  Ls...>,
                                  t_logical<T1, TAG1, L1s...>>::VALUE,
                  "logical types not allowed to do larger_equal operation");
    return get(lh) >= get(rh);
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename T,  typename TAG,  typename... Ls,
           typename T1, typename TAG1, typename... L1s>
  constexpr
  auto operator+(t_logical<T,  TAG,  Ls...>  lh,
                 t_logical<T1, TAG1, L1s...> rh) noexcept
      -> impl_::t_largest_<t_logical<T,  TAG,  Ls...>,
                           t_logical<T1, TAG1, L1s...>> {
    static_assert(impl_::t_is_op_<t_op_plus_tag,
                                  t_logical<T,  TAG,  Ls...>,
                                  t_logical<T1, TAG1, L1s...>>::VALUE,
                  "logical types not allowed to do plus operation");
    return impl_::t_largest_<t_logical<T,  TAG,  Ls...>,
                             t_logical<T1, TAG1, L1s...>>
      {(get(lh) + get(rh))};
  }

  template<typename T,  typename TAG,  typename... Ls>
  constexpr
  auto operator+(t_logical<T,  TAG, Ls...> lh, T rh) noexcept
      -> t_logical<T, TAG, Ls...> {
    static_assert(impl_::t_is_op_<t_op_plus_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical types not allowed to do plus operation");
    return t_logical<T, TAG, Ls...>(get(lh) + rh);
  }

  template<typename T,  typename TAG,  typename... Ls>
  constexpr
  auto operator+(T lh, t_logical<T,  TAG, Ls...> rh) noexcept
      -> t_logical<T, TAG, Ls...> {
    static_assert(impl_::t_is_op_<t_op_plus_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical types not allowed to do plus operation");
    return t_logical<T, TAG, Ls...>(lh + get(rh));
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename T,  typename TAG,  typename... Ls,
           typename T1, typename TAG1, typename... L1s>
  constexpr
  auto operator-(t_logical<T,  TAG,  Ls...>  lh,
                 t_logical<T1, TAG1, L1s...> rh) noexcept
      -> impl_::t_largest_<t_logical<T,  TAG,  Ls...>,
                           t_logical<T1, TAG1, L1s...>> {
    static_assert(impl_::t_is_op_<t_op_minus_tag,
                                  t_logical<T,  TAG,  Ls...>,
                                  t_logical<T1, TAG1, L1s...>>::VALUE,
                  "logical types not allowed to do minus operation");
    return impl_::t_largest_<t_logical<T,  TAG,  Ls...>,
                             t_logical<T1, TAG1, L1s...>>
      {(get(lh) - get(rh))};
  }

  template<typename T,  typename TAG,  typename... Ls>
  constexpr
  auto operator-(t_logical<T,  TAG, Ls...> lh, T rh) noexcept
      -> t_logical<T, TAG, Ls...> {
    static_assert(impl_::t_is_op_<t_op_minus_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical types not allowed to do minus operation");
    return t_logical<T, TAG, Ls...>(get(lh) - rh);
  }

  template<typename T,  typename TAG,  typename... Ls>
  constexpr
  auto operator-(T lh, t_logical<T,  TAG, Ls...> rh) noexcept
      -> t_logical<T, TAG, Ls...> {
    static_assert(impl_::t_is_op_<t_op_minus_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical types not allowed to do minus operation");
    return t_logical<T, TAG, Ls...>(lh - get(rh));
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename T,  typename TAG,  typename... Ls,
           typename T1, typename TAG1, typename... L1s>
  constexpr
  auto operator*(t_logical<T,  TAG,  Ls...>  lh,
                 t_logical<T1, TAG1, L1s...> rh) noexcept
      -> impl_::t_largest_<t_logical<T,  TAG,  Ls...>,
                           t_logical<T1, TAG1, L1s...>> {
    static_assert(impl_::t_is_op_<t_op_multiply_tag,
                                  t_logical<T,  TAG,  Ls...>,
                                  t_logical<T1, TAG1, L1s...>>::VALUE,
                  "logical types not allowed to do multiply operation");
    return impl_::t_largest_<t_logical<T, TAG,  Ls...>,
                             t_logical<T1,TAG1, L1s...>>
      {(get(lh) * get(rh))};
  }

  template<typename T,  typename TAG,  typename... Ls>
  constexpr
  auto operator*(t_logical<T,  TAG, Ls...> lh, T rh) noexcept
      -> t_logical<T, TAG, Ls...> {
    static_assert(impl_::t_is_op_<t_op_multiply_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical types not allowed to do multiply operation");
    return t_logical<T, TAG, Ls...>(get(lh) * rh);
  }

  template<typename T,  typename TAG,  typename... Ls>
  constexpr
  auto operator*(T lh, t_logical<T,  TAG, Ls...> rh) noexcept
      -> t_logical<T, TAG, Ls...> {
    static_assert(impl_::t_is_op_<t_op_multiply_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical types not allowed to do multiply operation");
    return t_logical<T, TAG, Ls...>(lh * get(rh));
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename T,  typename TAG,  typename... Ls,
           typename T1, typename TAG1, typename... L1s>
  constexpr
  auto operator/(t_logical<T,  TAG,  Ls...>  lh,
                 t_logical<T1, TAG1, L1s...> rh) noexcept
      -> impl_::t_largest_<t_logical<T,  TAG,  Ls...>,
                           t_logical<T1, TAG1, L1s...>> {
    static_assert(impl_::t_is_op_<t_op_divide_tag,
                                  t_logical<T,  TAG,  Ls...>,
                                  t_logical<T1, TAG1, L1s...>>::VALUE,
                  "logical types not allowed to do divide operation");
    return impl_::t_largest_<t_logical<T, TAG,  Ls...>,
                             t_logical<T1,TAG1, L1s...>>
      {(get(lh) / get(rh))};
  }

  template<typename T,  typename TAG,  typename... Ls>
  constexpr
  auto operator/(t_logical<T, TAG, Ls...> lh, T rh) noexcept
      -> t_logical<T, TAG, Ls...> {
    static_assert(impl_::t_is_op_<t_op_divide_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical types not allowed to do divide operation");
    return t_logical<T, TAG, Ls...>(get(lh) / rh);
  }

  template<typename T,  typename TAG,  typename... Ls>
  constexpr
  auto operator/(T lh, t_logical<T, TAG, Ls...> rh) noexcept
      -> t_logical<T, TAG, Ls...> {
    static_assert(impl_::t_is_op_<t_op_divide_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical types not allowed to do divide operation");
    return t_logical<T, TAG, Ls...>(lh / get(rh));
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename T,  typename TAG,  typename... Ls,
           typename T1, typename TAG1, typename... L1s>
  constexpr
  auto operator%(t_logical<T,  TAG,  Ls...>  lh,
                 t_logical<T1, TAG1, L1s...> rh) noexcept
      -> impl_::t_largest_<t_logical<T,  TAG,  Ls...>,
                           t_logical<T1, TAG1, L1s...>> {
    static_assert(impl_::t_is_op_<t_op_mod_tag,
                                  t_logical<T,  TAG,  Ls...>,
                                  t_logical<T1, TAG1, L1s...>>::VALUE,
                  "logical types not allowed to do mod operation");
    return impl_::t_largest_<t_logical<T,  TAG,  Ls...>,
                             t_logical<T1, TAG1, L1s...>>
      {(get(lh) % get(rh))};
  }

  template<typename T,  typename TAG,  typename... Ls>
  constexpr
  auto operator%(t_logical<T,  TAG, Ls...> lh, T rh) noexcept
      -> t_logical<T, TAG, Ls...> {
    static_assert(impl_::t_is_op_<t_op_mod_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical types not allowed to do mod operation");
    return t_logical<T, TAG, Ls...>(get(lh) % rh);
  }

  template<typename T,  typename TAG,  typename... Ls>
  constexpr
  auto operator%(T lh, t_logical<T,  TAG, Ls...> rh) noexcept
      -> t_logical<T, TAG, Ls...> {
    static_assert(impl_::t_is_op_<t_op_mod_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical types not allowed to do mod operation");
    return t_logical<T, TAG, Ls...>(lh % get(rh));
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename T,  typename TAG,  typename... Ls,
           typename T1, typename TAG1, typename... L1s>
  constexpr
  auto operator+=(t_logical<T,  TAG,  Ls...>& lh,
                  t_logical<T1, TAG1, L1s...> rh) noexcept
      -> t_logical<T,  TAG, Ls...>& {
    static_assert(impl_::t_is_op_<t_op_self_plus_tag,
                                  t_logical<T,  TAG,  Ls...>,
                                  t_logical<T1, TAG1, L1s...>>::VALUE,
                  "logical types not allowed to do self plus operation");
    return (lh = t_logical<T, TAG, Ls...>(get(lh) + get(rh)));
  }

  template<typename T,  typename TAG,  typename... Ls>
  constexpr
  auto operator+=(t_logical<T,  TAG, Ls...>& lh, T rh) noexcept
      -> t_logical<T, TAG, Ls...>& {
    static_assert(impl_::t_is_op_<t_op_self_plus_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical types not allowed to do self plus operation");
    return (lh = t_logical<T, TAG, Ls...>(get(lh) + rh));
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename T,  typename TAG,  typename... Ls,
           typename T1, typename TAG1, typename... L1s>
  constexpr
  auto operator-=(t_logical<T,  TAG,  Ls...>& lh,
                  t_logical<T1, TAG1, L1s...> rh) noexcept
      -> t_logical<T,  TAG, Ls...>& {
    static_assert(impl_::t_is_op_<t_op_self_minus_tag,
                                  t_logical<T,  TAG,  Ls...>,
                                  t_logical<T1, TAG1, L1s...>>::VALUE,
                  "logical types not allowed to do self minus operation");
    return (lh = t_logical<T, TAG, Ls...>(get(lh) - get(rh)));
  }

  template<typename T,  typename TAG,  typename... Ls>
  constexpr
  auto operator-=(t_logical<T,  TAG, Ls...>& lh, T rh) noexcept
      -> t_logical<T, TAG, Ls...>& {
    static_assert(impl_::t_is_op_<t_op_self_minus_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical types not allowed to do self minus operation");
    return (lh = t_logical<T, TAG, Ls...>(get(lh) - rh));
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename T,  typename TAG,  typename... Ls,
           typename T1, typename TAG1, typename... L1s>
  constexpr
  auto operator*=(t_logical<T,  TAG,  Ls...>& lh,
                  t_logical<T1, TAG1, L1s...> rh) noexcept
      -> t_logical<T,  TAG, Ls...>& {
    static_assert(impl_::t_is_op_<t_op_self_multiply_tag,
                                  t_logical<T,  TAG,  Ls...>,
                                  t_logical<T1, TAG1, L1s...>>::VALUE,
                  "logical types not allowed to do self multiply operation");
    return (lh = t_logical<T, TAG, Ls...>(get(lh) * get(rh)));
  }

  template<typename T,  typename TAG,  typename... Ls>
  constexpr
  auto operator*=(t_logical<T,  TAG, Ls...>& lh, T rh) noexcept
      -> t_logical<T, TAG, Ls...>& {
    static_assert(impl_::t_is_op_<t_op_self_multiply_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical types not allowed to do self multiply operation");
    return (lh = t_logical<T, TAG, Ls...>(get(lh) * rh));
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename T,  typename TAG,  typename... Ls,
           typename T1, typename TAG1, typename... L1s>
  constexpr
  auto operator/=(t_logical<T,  TAG,  Ls...>& lh,
                  t_logical<T1, TAG1, L1s...> rh) noexcept
      -> t_logical<T,  TAG, Ls...>& {
    static_assert(impl_::t_is_op_<t_op_self_divide_tag,
                                  t_logical<T,  TAG,  Ls...>,
                                  t_logical<T1, TAG1, L1s...>>::VALUE,
                  "logical types not allowed to do self divide operation");
    return (lh = t_logical<T, TAG, Ls...>(get(lh) / get(rh)));
  }

  template<typename T,  typename TAG,  typename... Ls>
  constexpr
  auto operator/=(t_logical<T,  TAG, Ls...>& lh, T rh) noexcept
      -> t_logical<T, TAG, Ls...>& {
    static_assert(impl_::t_is_op_<t_op_self_divide_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical types not allowed to do self divide operation");
    return (lh = t_logical<T, TAG, Ls...>(get(lh) / rh));
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename T,  typename TAG,  typename... Ls,
           typename T1, typename TAG1, typename... L1s>
  constexpr
  auto operator%=(t_logical<T,  TAG,  Ls...>& lh,
                  t_logical<T1, TAG1, L1s...> rh) noexcept
      -> t_logical<T,  TAG, Ls...>& {
    static_assert(impl_::t_is_op_<t_op_self_mod_tag,
                                  t_logical<T,  TAG,  Ls...>,
                                  t_logical<T1, TAG1, L1s...>>::VALUE,
                  "logical types not allowed to do self mod operation");
    return (lh = t_logical<T, TAG, Ls...>(get(lh) % get(rh)));
  }

  template<typename T,  typename TAG,  typename... Ls>
  constexpr
  auto operator%=(t_logical<T,  TAG, Ls...>& lh, T rh) noexcept
      -> t_logical<T, TAG, Ls...>& {
    static_assert(impl_::t_is_op_<t_op_self_mod_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical types not allowed to do self mod operation");
    return (lh = t_logical<T, TAG, Ls...>(get(lh) % rh));
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename T,  typename TAG,  typename... Ls,
           typename T1, typename TAG1, typename... L1s>
  constexpr
  t_bool operator&&(t_logical<T,  TAG,  Ls...>  lh,
                    t_logical<T1, TAG1, L1s...> rh) noexcept {
    static_assert(impl_::t_is_op_<t_op_and_tag,
                                  t_logical<T,  TAG,  Ls...>,
                                  t_logical<T1, TAG1, L1s...>>::VALUE,
                  "logical types not allowed to do and operation");
    return get(lh) && get(rh);
  }

  template<typename T,  typename TAG,  typename... Ls, typename U>
  constexpr
  t_bool operator&&(t_logical<T, TAG, Ls...> lh, const U& rh) noexcept {
    static_assert(impl_::t_is_op_<t_op_and_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical types not allowed to do and operation");
    return get(lh) && rh;
  }

  template<typename T,  typename TAG,  typename... Ls, typename U>
  constexpr
  t_bool operator&&(const U& lh, t_logical<T, TAG, Ls...> rh) noexcept {
    static_assert(impl_::t_is_op_<t_op_and_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical types not allowed to do and operation");
    return lh && get(rh);
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename T,  typename TAG,  typename... Ls,
           typename T1, typename TAG1, typename... L1s>
  constexpr
  t_bool operator||(t_logical<T,  TAG,  Ls...>  lh,
                    t_logical<T1, TAG1, L1s...> rh) noexcept {
    static_assert(impl_::t_is_op_<t_op_or_tag,
                                  t_logical<T,  TAG,  Ls...>,
                                  t_logical<T1, TAG1, L1s...>>::VALUE,
                  "logical types not allowed to do or operation");
    return get(lh) && get(rh);
  }

  template<typename T,  typename TAG,  typename... Ls, typename U>
  constexpr
  t_bool operator||(t_logical<T, TAG, Ls...> lh, const U& rh) noexcept {
    static_assert(impl_::t_is_op_<t_op_or_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical types not allowed to do or operation");
    return get(lh) && rh;
  }

  template<typename T,  typename TAG,  typename... Ls, typename U>
  constexpr
  t_bool operator||(const U& lh, t_logical<T, TAG, Ls...> rh) noexcept {
    static_assert(impl_::t_is_op_<t_op_or_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical types not allowed to do or operation");
    return lh && get(rh);
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename T,  typename TAG,  typename... Ls>
  constexpr
  auto operator-(t_logical<T, TAG, Ls...> lh) noexcept
      -> t_logical<T, TAG, Ls...> {
    static_assert(impl_::t_is_op_<t_op_negate_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical type not allowed to negate operation");
    return t_logical<T, TAG, Ls...>{-get(lh)};
  }

  template<typename T,  typename TAG,  typename... Ls>
  constexpr
  auto operator++(t_logical<T, TAG, Ls...>& lh) noexcept
      -> t_logical<T, TAG, Ls...>& {
    static_assert(impl_::t_is_op_<t_op_incr_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical type not allowed increment operation");
    return (lh = t_logical<T, TAG, Ls...>{get(lh) + 1});
  }

  template<typename T,  typename TAG,  typename... Ls>
  constexpr
  auto operator--(t_logical<T, TAG, Ls...>& lh) noexcept
      -> t_logical<T, TAG, Ls...>& {
    static_assert(impl_::t_is_op_<t_op_decr_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical type not allowed to decrement operation");
    return (lh = t_logical<T, TAG, Ls...>{get(lh) - 1});
  }

  template<typename T,  typename TAG,  typename... Ls>
  constexpr
  auto operator++(t_logical<T, TAG, Ls...>& lh, t_int) noexcept
      -> t_logical<T, TAG, Ls...> {
    static_assert(impl_::t_is_op_<t_op_post_incr_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical type not allowed post increment operation");
    t_logical<T, TAG, Ls...> tmp = lh;
    lh = t_logical<T, TAG, Ls...>{get(lh) + 1};
    return tmp;
  }

  template<typename T,  typename TAG,  typename... Ls>
  constexpr
  auto operator--(t_logical<T, TAG, Ls...>& lh, t_int) noexcept
      -> t_logical<T, TAG, Ls...> {
    static_assert(impl_::t_is_op_<t_op_post_decr_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical type not allowed to post decrement operation");
    t_logical<T, TAG, Ls...> tmp = lh;
    lh = t_logical<T, TAG, Ls...>{get(lh) - 1};
    return tmp;
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename T,  typename TAG,  typename... Ls,
           typename T1, typename TAG1, typename... L1s>
  constexpr
  auto operator^(t_logical<T,  TAG,  Ls...>  lh,
                 t_logical<T1, TAG1, L1s...> rh) noexcept
      -> impl_::t_largest_<t_logical<T,  TAG,  Ls...>,
                           t_logical<T1, TAG1, L1s...>> {
    static_assert(impl_::t_is_op_<t_op_bin_xor_tag,
                                  t_logical<T,  TAG,  Ls...>,
                                  t_logical<T1, TAG1, L1s...>>::VALUE,
                  "logical types not allowed to do bin xor operation");
    return impl_::t_largest_<t_logical<T,  TAG,  Ls...>,
                             t_logical<T1, TAG1, L1s...>>
      {(get(lh) ^ get(rh))};
  }

  template<typename T, typename TAG, typename... Ls>
  constexpr
  auto operator^(t_logical<T, TAG, Ls...> lh, T rh) noexcept
      -> t_logical<T, TAG, Ls...> {
    static_assert(impl_::t_is_op_<t_op_bin_xor_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical types not allowed to do bin xor operation");
    return t_logical<T, TAG, Ls...>(get(lh) ^ rh);
  }

  template<typename T, typename TAG, typename... Ls>
  constexpr
  auto operator^(T lh, t_logical<T, TAG, Ls...> rh) noexcept
      -> t_logical<T, TAG, Ls...> {
    static_assert(impl_::t_is_op_<t_op_bin_xor_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical types not allowed to do bin xor operation");
    return t_logical<T, TAG, Ls...>(lh ^ get(rh));
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename T,  typename TAG,  typename... Ls,
           typename T1, typename TAG1, typename... L1s>
  constexpr
  auto operator&(t_logical<T,  TAG,  Ls...>  lh,
                 t_logical<T1, TAG1, L1s...> rh) noexcept
      -> impl_::t_largest_<t_logical<T,  TAG,  Ls...>,
                           t_logical<T1, TAG1, L1s...>> {
    static_assert(impl_::t_is_op_<t_op_bin_and_tag,
                                  t_logical<T,  TAG,  Ls...>,
                                  t_logical<T1, TAG1, L1s...>>::VALUE,
                  "logical types not allowed to do bin and operation");
    return impl_::t_largest_<t_logical<T,  TAG,  Ls...>,
                             t_logical<T1, TAG1, L1s...>>
      {(get(lh) & get(rh))};
  }

  template<typename T, typename TAG, typename... Ls>
  constexpr
  auto operator&(t_logical<T, TAG, Ls...> lh, T rh) noexcept
      -> t_logical<T, TAG, Ls...> {
    static_assert(impl_::t_is_op_<t_op_bin_and_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical types not allowed to do bin and operation");
    return t_logical<T, TAG, Ls...>(get(lh) & rh);
  }

  template<typename T, typename TAG, typename... Ls>
  constexpr
  auto operator&(T lh, t_logical<T, TAG, Ls...> rh) noexcept
      -> t_logical<T, TAG, Ls...> {
    static_assert(impl_::t_is_op_<t_op_bin_and_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical types not allowed to do bin and operation");
    return t_logical<T, TAG, Ls...>(lh & get(rh));
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename T,  typename TAG,  typename... Ls,
           typename T1, typename TAG1, typename... L1s>
  constexpr
  auto operator|(t_logical<T,  TAG,  Ls...>  lh,
                 t_logical<T1, TAG1, L1s...> rh) noexcept
      -> impl_::t_largest_<t_logical<T,  TAG,  Ls...>,
                           t_logical<T1, TAG1, L1s...>> {
    static_assert(impl_::t_is_op_<t_op_bin_ior_tag,
                                  t_logical<T,  TAG,  Ls...>,
                                  t_logical<T1, TAG1, L1s...>>::VALUE,
                  "logical types not allowed to do bin ior operation");
    return impl_::t_largest_<t_logical<T,  TAG,  Ls...>,
                             t_logical<T1, TAG1, L1s...>>
      {(get(lh) | get(rh))};
  }

  template<typename T, typename TAG, typename... Ls>
  constexpr
  auto operator|(t_logical<T, TAG, Ls...> lh, T rh) noexcept
      -> t_logical<T, TAG, Ls...> {
    static_assert(impl_::t_is_op_<t_op_bin_ior_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical types not allowed to do bin ior operation");
    return t_logical<T, TAG, Ls...>(get(lh) | rh);
  }

  template<typename T, typename TAG, typename... Ls>
  constexpr
  auto operator|(T lh, t_logical<T, TAG, Ls...> rh) noexcept
      -> t_logical<T, TAG, Ls...> {
    static_assert(impl_::t_is_op_<t_op_bin_ior_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical types not allowed to do bin ior operation");
    return t_logical<T, TAG, Ls...>(lh | get(rh));
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename T,  typename TAG,  typename... Ls,
           typename T1, typename TAG1, typename... L1s>
  constexpr
  auto operator<<(t_logical<T,  TAG,  Ls...>  lh,
                 t_logical<T1, TAG1, L1s...> rh) noexcept
      -> impl_::t_largest_<t_logical<T,  TAG,  Ls...>,
                           t_logical<T1, TAG1, L1s...>> {
    static_assert(impl_::t_is_op_<t_op_bin_lshift_tag,
                                  t_logical<T,  TAG,  Ls...>,
                                  t_logical<T1, TAG1, L1s...>>::VALUE,
                  "logical types not allowed to do bin lshift operation");
    return impl_::t_largest_<t_logical<T,  TAG,  Ls...>,
                             t_logical<T1, TAG1, L1s...>>
      {(get(lh) << get(rh))};
  }

  template<typename T, typename TAG, typename... Ls>
  constexpr
  auto operator<<(t_logical<T, TAG, Ls...> lh, T rh) noexcept
      -> t_logical<T, TAG, Ls...> {
    static_assert(impl_::t_is_op_<t_op_bin_lshift_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical types not allowed to do bin lshift operation");
    return t_logical<T, TAG, Ls...>(get(lh) << rh);
  }

  template<typename T, typename TAG, typename... Ls>
  constexpr
  auto operator<<(T lh, t_logical<T, TAG, Ls...> rh) noexcept
      -> t_logical<T, TAG, Ls...> {
    static_assert(impl_::t_is_op_<t_op_bin_lshift_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical types not allowed to do bin lshift operation");
    return t_logical<T, TAG, Ls...>(lh << get(rh));
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename T,  typename TAG,  typename... Ls,
           typename T1, typename TAG1, typename... L1s>
  constexpr
  auto operator>>(t_logical<T,  TAG,  Ls...>  lh,
                 t_logical<T1, TAG1, L1s...> rh) noexcept
      -> impl_::t_largest_<t_logical<T,  TAG,  Ls...>,
                           t_logical<T1, TAG1, L1s...>> {
    static_assert(impl_::t_is_op_<t_op_bin_rshift_tag,
                                  t_logical<T,  TAG,  Ls...>,
                                  t_logical<T1, TAG1, L1s...>>::VALUE,
                  "logical types not allowed to do bin rshift operation");
    return impl_::t_largest_<t_logical<T,  TAG,  Ls...>,
                             t_logical<T1, TAG1, L1s...>>
      {(get(lh) >> get(rh))};
  }

  template<typename T, typename TAG, typename... Ls>
  constexpr
  auto operator>>(t_logical<T, TAG, Ls...> lh, T rh) noexcept
      -> t_logical<T, TAG, Ls...> {
    static_assert(impl_::t_is_op_<t_op_bin_rshift_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical types not allowed to do bin rshift operation");
    return t_logical<T, TAG, Ls...>(get(lh) >> rh);
  }

  template<typename T, typename TAG, typename... Ls>
  constexpr
  auto operator>>(T lh, t_logical<T, TAG, Ls...> rh) noexcept
      -> t_logical<T, TAG, Ls...> {
    static_assert(impl_::t_is_op_<t_op_bin_rshift_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical types not allowed to do bin rshift operation");
    return t_logical<T, TAG, Ls...>(lh >> get(rh));
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename T,  typename TAG,  typename... Ls,
           typename T1, typename TAG1, typename... L1s>
  constexpr
  auto operator^=(t_logical<T,  TAG,  Ls...>& lh,
                  t_logical<T1, TAG1, L1s...> rh) noexcept
      -> t_logical<T,  TAG, Ls...>& {
    static_assert(impl_::t_is_op_<t_op_bin_self_xor_tag,
                                  t_logical<T,  TAG,  Ls...>,
                                  t_logical<T1, TAG1, L1s...>>::VALUE,
                  "logical types not allowed to do ^= operation");
    return (lh = t_logical<T, TAG, Ls...>(get(lh) ^ get(rh)));
  }

  template<typename T, typename TAG, typename... Ls>
  constexpr
  auto operator^=(t_logical<T, TAG, Ls...>& lh, T rh) noexcept
      -> t_logical<T, TAG, Ls...>& {
    static_assert(impl_::t_is_op_<t_op_bin_self_xor_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical types not allowed to do ^= operation");
    return (lh = t_logical<T, TAG, Ls...>(get(lh) ^ rh));
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename T,  typename TAG,  typename... Ls,
           typename T1, typename TAG1, typename... L1s>
  constexpr
  auto operator&=(t_logical<T,  TAG,  Ls...>& lh,
                  t_logical<T1, TAG1, L1s...> rh) noexcept
      -> t_logical<T,  TAG, Ls...>& {
    static_assert(impl_::t_is_op_<t_op_bin_self_and_tag,
                                  t_logical<T,  TAG,  Ls...>,
                                  t_logical<T1, TAG1, L1s...>>::VALUE,
                  "logical types not allowed to do ^= operation");
    return (lh = t_logical<T, TAG, Ls...>(get(lh) & get(rh)));
  }

  template<typename T, typename TAG, typename... Ls>
  constexpr
  auto operator&=(t_logical<T, TAG, Ls...>& lh, T rh) noexcept
      -> t_logical<T, TAG, Ls...>& {
    static_assert(impl_::t_is_op_<t_op_bin_self_and_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical types not allowed to do ^= operation");
    return (lh = t_logical<T, TAG, Ls...>(get(lh) & rh));
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename T,  typename TAG,  typename... Ls,
           typename T1, typename TAG1, typename... L1s>
  constexpr
  auto operator|=(t_logical<T,  TAG,  Ls...>& lh,
                  t_logical<T1, TAG1, L1s...> rh) noexcept
      -> t_logical<T,  TAG, Ls...>& {
    static_assert(impl_::t_is_op_<t_op_bin_self_ior_tag,
                                  t_logical<T,  TAG,  Ls...>,
                                  t_logical<T1, TAG1, L1s...>>::VALUE,
                  "logical types not allowed to do |= operation");
    return (lh = t_logical<T, TAG, Ls...>(get(lh) | get(rh)));
  }

  template<typename T, typename TAG, typename... Ls>
  constexpr
  auto operator|=(t_logical<T, TAG, Ls...>& lh, T rh) noexcept
      -> t_logical<T, TAG, Ls...>& {
    static_assert(impl_::t_is_op_<t_op_bin_self_ior_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical types not allowed to do |= operation");
    return (lh = t_logical<T, TAG, Ls...>(get(lh) | rh));
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename T,  typename TAG,  typename... Ls,
           typename T1, typename TAG1, typename... L1s>
  constexpr
  auto operator<<=(t_logical<T,  TAG,  Ls...>& lh,
                   t_logical<T1, TAG1, L1s...> rh) noexcept
      -> t_logical<T,  TAG, Ls...>& {
    static_assert(impl_::t_is_op_<t_op_bin_self_lshift_tag,
                                  t_logical<T,  TAG,  Ls...>,
                                  t_logical<T1, TAG1, L1s...>>::VALUE,
                  "logical types not allowed to do <<= operation");
    return (lh = t_logical<T, TAG, Ls...>(get(lh) << get(rh)));
  }

  template<typename T, typename TAG, typename... Ls>
  constexpr
  auto operator<<=(t_logical<T, TAG, Ls...>& lh, T rh) noexcept
      -> t_logical<T, TAG, Ls...>& {
    static_assert(impl_::t_is_op_<t_op_bin_self_lshift_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical types not allowed to do <<= operation");
    return (lh = t_logical<T, TAG, Ls...>(get(lh) << rh));
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename T,  typename TAG,  typename... Ls,
           typename T1, typename TAG1, typename... L1s>
  constexpr
  auto operator>>=(t_logical<T,  TAG,  Ls...>& lh,
                   t_logical<T1, TAG1, L1s...> rh) noexcept
      -> t_logical<T,  TAG, Ls...>& {
    static_assert(impl_::t_is_op_<t_op_bin_self_rshift_tag,
                                  t_logical<T,  TAG,  Ls...>,
                                  t_logical<T1, TAG1, L1s...>>::VALUE,
                  "logical types not allowed to do >>= operation");
    return (lh = t_logical<T, TAG, Ls...>(get(lh) >> get(rh)));
  }

  template<typename T, typename TAG, typename... Ls>
  constexpr
  auto operator>>=(t_logical<T, TAG, Ls...>& lh, T rh) noexcept
      -> t_logical<T, TAG, Ls...>& {
    static_assert(impl_::t_is_op_<t_op_bin_self_rshift_tag,
                                  t_logical<T, TAG, Ls...>>::VALUE,
                  "logical types not allowed to do >>= operation");
    return (lh = t_logical<T, TAG, Ls...>(get(lh) >> rh));
  }

  /////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
