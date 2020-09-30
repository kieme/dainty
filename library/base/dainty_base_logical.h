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

#include "dainty_base_types.h"
#include "dainty_base_traits.h"

namespace dainty
{
namespace base
{
namespace logical
{
  /////////////////////////////////////////////////////////////////////////////

  using types::t_prefix;
  using types::t_bool;
  using types::t_n_;
  using types::t_ix_;
  using types::t_int;

  using traits::t_pack;
  using traits::t_result_false;
  using traits::t_result_true;
  using traits::t_is_there;
  using traits::t_is_fundamental;
  using traits::t_is_ptr;
  using traits::t_least_one_is_true;
  using traits::t_flatten_pack;
  using traits::t_unique_flatten_pack;
  using traits::t_unique_pack;
  using traits::t_is_in_pack;
  using traits::t_is_subset_of_pack;
  using traits::t_largest_pack;
  using traits::t_if_then_else;
  using traits::t_is_same;
  using traits::t_and;
  using traits::t_or;
  using traits::t_none;

  // RULE TAG can have t_value
  // RULE TAG can have t_ops
  // RULE TAG can have check - static

  /////////////////////////////////////////////////////////////////////////////

  enum t_op_less_tag            { }; // X <
  enum t_op_less_equal_tag      { }; // X <=
  enum t_op_larger_tag          { }; // X >
  enum t_op_larger_equal_tag    { }; // X >=
  enum t_op_equal_tag           { }; // X ==
  enum t_op_not_equal_tag       { }; // X !=
  enum t_op_plus_tag            { }; // X + or + X
  enum t_op_minus_tag           { }; // X - or - X
  enum t_op_multiply_tag        { }; // X * or * X
  enum t_op_divide_tag          { }; // X / or / X
  enum t_op_mod_tag             { }; // X % or % X
  enum t_op_self_plus_tag       { }; // X +=
  enum t_op_self_minus_tag      { }; // X -=
  enum t_op_self_multiply_tag   { }; // X *=
  enum t_op_self_divide_tag     { }; // X /=
  enum t_op_self_mod_tag        { }; // X %=
  enum t_op_and_tag             { }; // X && or && X
  enum t_op_or_tag              { }; // X || or || X
  enum t_op_not_tag             { }; // !X -- FIXME NOT SURE
  enum t_op_negate_tag          { }; // -X
  enum t_op_incr_tag            { }; // ++X
  enum t_op_decr_tag            { }; // --X
  enum t_op_post_incr_tag       { }; // X++
  enum t_op_post_decr_tag       { }; // X--
  enum t_op_bin_xor_tag         { }; // X ^ or ^ X
  enum t_op_bin_and_tag         { }; // X & or & X
  enum t_op_bin_ior_tag         { }; // X | or | X
  enum t_op_bin_lshift_tag      { }; // X << or << X
  enum t_op_bin_rshift_tag      { }; // X >> or >> X
  enum t_op_bin_self_xor_tag    { }; // X ^=
  enum t_op_bin_self_and_tag    { }; // X &=
  enum t_op_bin_self_ior_tag    { }; // X |=
  enum t_op_bin_self_lshift_tag { }; // X <<= or <<= X
  enum t_op_bin_self_rshift_tag { }; // X >>= or >>= X

  using t_ops_compare_tag = t_pack<t_op_less_tag,
                                   t_op_less_equal_tag,
                                   t_op_larger_tag,
                                   t_op_larger_equal_tag,
                                   t_op_equal_tag,
                                   t_op_not_equal_tag>;

  using t_ops_arithmetic_tag = t_pack<t_op_plus_tag,
                                      t_op_minus_tag,
                                      t_op_multiply_tag,
                                      t_op_divide_tag,
                                      t_op_mod_tag>;

  using t_ops_arithmetic_self_tag = t_pack<t_op_self_plus_tag,
                                           t_op_self_minus_tag,
                                           t_op_self_multiply_tag,
                                           t_op_self_divide_tag,
                                           t_op_self_mod_tag>;

  using t_ops_or_and_tag = t_pack<t_op_and_tag,
                                  t_op_or_tag>;

  using t_ops_bin_tag = t_pack<t_op_bin_xor_tag,
                               t_op_bin_and_tag,
                               t_op_bin_ior_tag,
                               t_op_bin_lshift_tag,
                               t_op_bin_rshift_tag,
                               t_op_bin_self_xor_tag,
                               t_op_bin_self_and_tag,
                               t_op_bin_self_ior_tag,
                               t_op_bin_self_lshift_tag,
                               t_op_bin_self_rshift_tag>;

  using t_ops_value_tag = t_pack<t_ops_compare_tag,
                                 t_ops_arithmetic_tag,
                                 t_ops_arithmetic_self_tag,
                                 t_ops_bin_tag,
                                 t_op_negate_tag, // FIXME
                                 t_op_incr_tag,
                                 t_op_decr_tag,
                                 t_op_post_incr_tag,
                                 t_op_post_decr_tag>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename TAG, typename... Ls>
  class t_logical;

  /////////////////////////////////////////////////////////////////////////////

  namespace impl_
  {
    template<typename T, t_bool>
    struct t_build_ops_ {
      using t_value = t_pack<>;
    };

    template<typename T>
    struct t_build_ops_<T, true> {
      using t_value =
        typename t_unique_flatten_pack<typename T::t_ops>::t_value;
    };

    template<typename T>
    using t_has_ops_ = traits::t_if_pack<typename T::t_ops>;

    ///

    template<typename...>
    struct t_pack_tags_;

    template<typename T>
    struct t_pack_tags_<t_pack<T>> {
      using t_value = t_pack<T>;
    };

    template<typename... Ts, typename T1, typename TAG1>
    struct t_pack_tags_<t_pack<Ts...>, t_logical<T1, TAG1>> {
      using t_value = t_pack<Ts..., TAG1>;
    };

    template<typename... Ts, typename T1, typename TAG1, typename... Ls1>
    struct t_pack_tags_<t_pack<Ts...>, t_logical<T1, TAG1, Ls1...>> {
      using t_value = typename t_pack_tags_<t_pack<Ts..., TAG1>,
                                            Ls1...>::t_value;
    };

    template<typename... Ts, typename T1, typename TAG1, typename... Ls1,
             typename... Ls>
    struct t_pack_tags_<t_pack<Ts...>, t_logical<T1, TAG1, Ls1...>, Ls...> {
      using t_value =
        typename t_pack_tags_<
          typename t_pack_tags_<t_pack<Ts...>,
                                t_logical<T1, TAG1, Ls1...>>::t_value,
          Ls...>::t_value;
    };

    template<typename TAG, typename... Ls>
    struct t_build_tags_ {
      using t_value =
        typename t_unique_pack<
          typename impl_::t_pack_tags_<t_pack<TAG>, Ls...>::t_value>::t_value;
    };

    template<typename L, typename L1>
    struct t_largest_ {
      using t_value =
        typename t_if_then_else<
          t_is_same<typename t_largest_pack<typename L ::t_tags,
                                            typename L1::t_tags>::t_value,
                    typename L::t_tags>,
          L, L1>::t_value;
    };
  }

  template<typename TAG, typename... Ls>
  using t_build_tags_ = impl_::t_build_tags_<TAG, Ls...>;

  template<typename TAG>
  using t_build_ops_ =
    impl_::t_build_ops_<TAG, t_is_there< impl_::t_has_ops_, TAG>::VALUE>;

  template<typename TAG>
  constexpr
  typename TAG::t_value check_(typename TAG::t_value value) noexcept {
    return TAG::check(value);
  }

  template<typename TAG, typename... Ts>
  constexpr
  typename TAG::t_value check_(typename TAG::t_value value) noexcept {
    return check_<Ts...>(check_<TAG>(value));
  }

  template<typename T, typename T1>
  using t_is_subset_of = t_is_subset_of_pack<T, T1>;

  template<typename T, typename T1>
  using t_is_related_ = t_or<t_is_subset_of_pack<typename T ::t_tags,
                                                 typename T1::t_tags>,
                             t_is_subset_of_pack<typename T1::t_tags,
                                                 typename T ::t_tags>>;

  template<typename O, typename T, typename T1 = t_none>
  struct t_is_op_ : t_and<t_is_in_pack<O, typename T ::t_ops>,
                          t_is_in_pack<O, typename T1::t_ops>,
                          t_is_related_<T, T1>> {
  };

  template<typename O, typename T>
  struct t_is_op_<O, T, t_none> : t_is_in_pack<O, typename T::t_ops> {
  };

  /////////////////////////////////////////////////////////////////////////////

  template<class T, class TAG, class... Ls>
  constexpr
  T get(t_logical<T, TAG, Ls...>) noexcept;

  template<class T, class TAG, class... Ls>
  constexpr
  t_logical<T, TAG, Ls...>& set(t_logical<T, TAG, Ls...>&, T) noexcept;

  template<class T, class TAG, class... Ls, class T1, class TAG1, class... Ls1>
  constexpr
  t_logical<T, TAG, Ls...>& set(t_logical<T,  TAG,  Ls...>&,
                                t_logical<T1, TAG1, Ls1...>) noexcept;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename TAG, typename... Ls>
  class t_logical {
  public:
    static_assert(t_least_one_is_true<T,
                  t_is_fundamental, t_is_ptr>::VALUE, "wrong type");

    using t_value = T;
    using t_tag   = TAG;
    using t_ops   = typename t_build_ops_<TAG>::t_value;
    using t_tags  = typename t_build_tags_<TAG, Ls...>::t_value;

    constexpr
    explicit t_logical(t_value value) noexcept : value_{value} {
    }

    template<typename T1, typename... Ts>
    constexpr
    t_logical(t_logical<T1, Ts...> logical) noexcept
        : value_{logical.value_} {
      static_assert(t_is_subset_of<t_tags,
                      typename t_logical<T1, Ts...>::t_tags>::VALUE,
                     "cannot copy type. types don't give permission.");
    }

    template<typename T1, typename... Ts>
    constexpr
    t_logical& operator=(t_logical<T1, Ts...> logical) noexcept {
      static_assert(t_is_subset_of<t_tags,
                      typename t_logical<T1, Ts...>::t_tags>::VALUE,
                     "cannot copy type. types don't give permission.");
      value_ = logical.value_;
      return *this;
    }

    t_logical()                                     = delete;  // for clarity
    t_logical(const t_logical&)            noexcept = default; // for clarity
    t_logical& operator=(const t_logical&) noexcept = default; // for clarity

  private:
    template<typename, typename, typename...> friend class t_logical;

    template<typename T1, typename TAG1, typename... Ls1>
    friend constexpr
    T1 get(t_logical<T1, TAG1, Ls1...>) noexcept;

    template<typename T1, typename TAG1, typename... Ls1>
    friend constexpr
    t_logical<T1, TAG1, Ls1...>& set(t_logical<T1, TAG1, Ls1...>&, T1) noexcept;

    template<class T1, class TAG1, class... Ls1,
             class T2, class TAG2, class... Ls2>
    friend constexpr
    t_logical<T1, TAG1, Ls1...>& set(t_logical<T1, TAG1, Ls1...>&,
                                     t_logical<T2, TAG2, Ls2...>) noexcept;

    t_value value_;
  };

  /////////////////////////////////////////////////////////////////////////////

  template<class T, class TAG, class... Ls>
  constexpr
  T get(t_logical<T, TAG, Ls...> logical) noexcept {
    return logical.value_;
  }

  template<class T, class TAG, class... Ls>
  constexpr
  t_logical<T, TAG, Ls...>& set(t_logical<T, TAG, Ls...>& logical,
                                T value) noexcept {
    return (logical = t_logical<T, TAG, Ls...>{value});
  }

  template<class T,  class TAG,  class... Ls,
           class T1, class TAG1, class... Ls1>
  constexpr
  t_logical<T, TAG, Ls...>& set(t_logical<T,  TAG,  Ls...>& logical,
                                t_logical<T1, TAG1, Ls1...> value) noexcept {
    return (logical = t_logical<T, TAG, Ls...>{get(value)});
    // FIXME even if types are not exact - can reduce the checking
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename>
  struct t_is_logical : t_result_false {
  };

  template<typename T, typename TAG, typename... Ls>
  struct t_is_logical<t_logical<T, TAG, Ls...>> : t_result_true {
  };

  template<class T> using t_is_not_logical = traits::t_not<t_is_logical<T>>;
  template<class T> using t_if_logical     = traits::t_if<t_is_logical<T>>;
  template<class T> using t_if_not_logical = traits::t_if<t_is_not_logical<T>>;

  /////////////////////////////////////////////////////////////////////////////

  template<class T,  class... TAGS, class T1, class... TAGS1>
  constexpr
  t_bool operator==(t_logical<T,  TAGS...>  lh,
                    t_logical<T1, TAGS1...> rh) noexcept {
    static_assert(t_is_op_<t_op_equal_tag,
                           t_logical<T,  TAGS...>,
                           t_logical<T1, TAGS1...>>::VALUE,
                  "logical types not allowed to do equal operation");
    return get(lh) == get(rh);
  }

  template<class T,  class... TAGS, class T1, class... TAGS1>
  constexpr
  t_bool operator!=(t_logical<T,  TAGS...>  lh,
                    t_logical<T1, TAGS1...> rh) noexcept {
    static_assert(t_is_op_<t_op_not_equal_tag,
                           t_logical<T,  TAGS...>,
                           t_logical<T1, TAGS1...>>::VALUE,
                  "logical types not allowed to do not_equal operation");
    return get(lh) != get(rh);
  }

  template<class T,  class... TAGS, class T1, class... TAGS1>
  constexpr
  t_bool operator<(t_logical<T,  TAGS...>  lh,
                   t_logical<T1, TAGS1...> rh) noexcept {
    static_assert(t_is_op_<t_op_less_tag,
                           t_logical<T,  TAGS...>,
                           t_logical<T1, TAGS1...>>::VALUE,
                  "logical types not allowed to do less operation");
    return get(lh) < get(rh);
  }

  template<class T,  class... TAGS, class T1, class... TAGS1>
  constexpr
  t_bool operator>(t_logical<T,  TAGS...>  lh,
                   t_logical<T1, TAGS1...> rh) noexcept {
    static_assert(t_is_op_<t_op_larger_tag,
                           t_logical<T,  TAGS...>,
                           t_logical<T1, TAGS1...>>::VALUE,
                  "logical types not allowed to do larger operation");
    return get(lh) > get(rh);
  }

  template<class T,  class... TAGS, class T1, class... TAGS1>
  constexpr
  t_bool operator<=(t_logical<T,  TAGS...>  lh,
                    t_logical<T1, TAGS1...> rh) noexcept {
    static_assert(t_is_op_<t_op_less_equal_tag,
                           t_logical<T,  TAGS...>,
                           t_logical<T1, TAGS1...>>::VALUE,
                  "logical types not allowed to do less_equal operation");
    return get(lh) <= get(rh);
  }

  template<class T,  class... TAGS, class T1, class... TAGS1>
  constexpr
  t_bool operator>=(t_logical<T,  TAGS...>  lh,
                    t_logical<T1, TAGS1...> rh) noexcept {
    static_assert(t_is_op_<t_op_larger_equal_tag,
                           t_logical<T,  TAGS...>,
                           t_logical<T1, TAGS1...>>::VALUE,
                  "logical types not allowed to do larger_equal operation");
    return get(lh) >= get(rh);
  }

  /////////////////////////////////////////////////////////////////////////////

  template<class T,  class... TAGS, class T1, class... TAGS1>
  constexpr
  auto operator+(t_logical<T,  TAGS...>  lh,
                 t_logical<T1, TAGS1...> rh) noexcept
      -> typename impl_::t_largest_<t_logical<T,  TAGS...>,
                                    t_logical<T1, TAGS1...>>::t_value {
    static_assert(t_is_op_<t_op_plus_tag,
                           t_logical<T,  TAGS...>,
                           t_logical<T1, TAGS1...>>::VALUE,
                  "logical types not allowed to do plus operation");
    return typename impl_::t_largest_<t_logical<T,  TAGS...>,
                                      t_logical<T1, TAGS1...>>::t_value
      {(get(lh) + get(rh))};
  }

  template<class T,  class... TAGS>
  constexpr
  auto operator+(t_logical<T,  TAGS...> lh, T rh) noexcept
      -> t_logical<T, TAGS...> {
    static_assert(t_is_op_<t_op_plus_tag,
                           t_logical<T, TAGS...>,
                           t_logical<T, TAGS...>>::VALUE,
                  "logical types not allowed to do plus operation");
    return t_logical<T, TAGS...>(get(lh) + rh);
  }

  template<class T,  class... TAGS>
  constexpr
  auto operator+(T lh, t_logical<T,  TAGS...> rh) noexcept
      -> t_logical<T, TAGS...> {
    static_assert(t_is_op_<t_op_plus_tag,
                           t_logical<T, TAGS...>,
                           t_logical<T, TAGS...>>::VALUE,
                  "logical types not allowed to do plus operation");
    return t_logical<T, TAGS...>(lh + get(rh));
  }

  /////////////////////////////////////////////////////////////////////////////

  template<class T,  class... TAGS, class T1, class... TAGS1>
  constexpr
  auto operator-(t_logical<T,  TAGS...>  lh,
                 t_logical<T1, TAGS1...> rh) noexcept
      -> typename impl_::t_largest_<t_logical<T,  TAGS...>,
                                    t_logical<T1, TAGS1...>>::t_value {
    static_assert(t_is_op_<t_op_minus_tag,
                           t_logical<T,  TAGS...>,
                           t_logical<T1, TAGS1...>>::VALUE,
                  "logical types not allowed to do minus operation");
    return typename impl_::t_largest_<t_logical<T,  TAGS...>,
                                      t_logical<T1, TAGS1...>>::t_value
      {(get(lh) - get(rh))};
  }

  template<class T,  class... TAGS>
  constexpr
  auto operator-(t_logical<T,  TAGS...> lh, T rh) noexcept
      -> t_logical<T, TAGS...> {
    static_assert(t_is_op_<t_op_minus_tag,
                           t_logical<T, TAGS...>,
                           t_logical<T, TAGS...>>::VALUE,
                  "logical types not allowed to do minus operation");
    return t_logical<T, TAGS...>(get(lh) - rh);
  }

  template<class T,  class... TAGS>
  constexpr
  auto operator-(T lh, t_logical<T,  TAGS...> rh) noexcept
      -> t_logical<T, TAGS...> {
    static_assert(t_is_op_<t_op_minus_tag,
                           t_logical<T, TAGS...>,
                           t_logical<T, TAGS...>>::VALUE,
                  "logical types not allowed to do minus operation");
    return t_logical<T, TAGS...>(lh - get(rh));
  }

  /////////////////////////////////////////////////////////////////////////////

  template<class T,  class... TAGS, class T1, class... TAGS1>
  constexpr
  auto operator*(t_logical<T,  TAGS...>  lh,
                 t_logical<T1, TAGS1...> rh) noexcept
      -> typename impl_::t_largest_<t_logical<T,  TAGS...>,
                                    t_logical<T1, TAGS1...>>::t_value {
    static_assert(t_is_op_<t_op_multiply_tag,
                           t_logical<T,  TAGS...>,
                           t_logical<T1, TAGS1...>>::VALUE,
                  "logical types not allowed to do multiply operation");
    return typename impl_::t_largest_<t_logical<T,  TAGS...>,
                                      t_logical<T1, TAGS1...>>::t_value
      {(get(lh) * get(rh))};
  }

  template<class T,  class... TAGS>
  constexpr
  auto operator*(t_logical<T,  TAGS...> lh, T rh) noexcept
      -> t_logical<T, TAGS...> {
    static_assert(t_is_op_<t_op_multiply_tag,
                           t_logical<T, TAGS...>,
                           t_logical<T, TAGS...>>::VALUE,
                  "logical types not allowed to do multiply operation");
    return t_logical<T, TAGS...>(get(lh) * rh);
  }

  template<class T,  class... TAGS>
  constexpr
  auto operator*(T lh, t_logical<T,  TAGS...> rh) noexcept
      -> t_logical<T, TAGS...> {
    static_assert(t_is_op_<t_op_multiply_tag,
                           t_logical<T, TAGS...>,
                           t_logical<T, TAGS...>>::VALUE,
                  "logical types not allowed to do multiply operation");
    return t_logical<T, TAGS...>(lh * get(rh));
  }

  /////////////////////////////////////////////////////////////////////////////

  template<class T,  class... TAGS, class T1, class... TAGS1>
  constexpr
  auto operator/(t_logical<T,  TAGS...>  lh,
                 t_logical<T1, TAGS1...> rh) noexcept
      -> typename impl_::t_largest_<t_logical<T,  TAGS...>,
                                    t_logical<T1, TAGS1...>>::t_value {
    static_assert(t_is_op_<t_op_divide_tag,
                           t_logical<T,  TAGS...>,
                           t_logical<T1, TAGS1...>>::VALUE,
                  "logical types not allowed to do divide operation");
    return typename impl_::t_largest_<t_logical<T,  TAGS...>,
                                      t_logical<T1, TAGS1...>>::t_value
      {(get(lh) / get(rh))};
  }

  template<class T,  class... TAGS>
  constexpr
  auto operator/(t_logical<T,  TAGS...> lh, T rh) noexcept
      -> t_logical<T, TAGS...> {
    static_assert(t_is_op_<t_op_divide_tag,
                           t_logical<T, TAGS...>,
                           t_logical<T, TAGS...>>::VALUE,
                  "logical types not allowed to do divide operation");
    return t_logical<T, TAGS...>(get(lh) / rh);
  }

  template<class T,  class... TAGS>
  constexpr
  auto operator/(T lh, t_logical<T,  TAGS...> rh) noexcept
      -> t_logical<T, TAGS...> {
    static_assert(t_is_op_<t_op_divide_tag,
                           t_logical<T, TAGS...>,
                           t_logical<T, TAGS...>>::VALUE,
                  "logical types not allowed to do divide operation");
    return t_logical<T, TAGS...>(lh / get(rh));
  }

  /////////////////////////////////////////////////////////////////////////////

  template<class T,  class... TAGS, class T1, class... TAGS1>
  constexpr
  auto operator%(t_logical<T,  TAGS...>  lh,
                 t_logical<T1, TAGS1...> rh) noexcept
      -> typename impl_::t_largest_<t_logical<T,  TAGS...>,
                                    t_logical<T1, TAGS1...>>::t_value {
    static_assert(t_is_op_<t_op_mod_tag,
                           t_logical<T,  TAGS...>,
                           t_logical<T1, TAGS1...>>::VALUE,
                  "logical types not allowed to do mod operation");
    return typename impl_::t_largest_<t_logical<T,  TAGS...>,
                                      t_logical<T1, TAGS1...>>::t_value
      {(get(lh) % get(rh))};
  }

  template<class T,  class... TAGS>
  constexpr
  auto operator%(t_logical<T,  TAGS...> lh, T rh) noexcept
      -> t_logical<T, TAGS...> {
    static_assert(t_is_op_<t_op_mod_tag,
                           t_logical<T, TAGS...>,
                           t_logical<T, TAGS...>>::VALUE,
                  "logical types not allowed to do mod operation");
    return t_logical<T, TAGS...>(get(lh) % rh);
  }

  template<class T,  class... TAGS>
  constexpr
  auto operator%(T lh, t_logical<T,  TAGS...> rh) noexcept
      -> t_logical<T, TAGS...> {
    static_assert(t_is_op_<t_op_mod_tag,
                           t_logical<T, TAGS...>,
                           t_logical<T, TAGS...>>::VALUE,
                  "logical types not allowed to do mod operation");
    return t_logical<T, TAGS...>(lh % get(rh));
  }

  /////////////////////////////////////////////////////////////////////////////

  template<class T,  class... TAGS, class T1, class... TAGS1>
  constexpr
  auto operator+=(t_logical<T,  TAGS...>& lh,
                  t_logical<T1, TAGS1...> rh) noexcept
      -> t_logical<T,  TAGS...>& {
    static_assert(t_is_op_<t_op_self_plus_tag,
                           t_logical<T,  TAGS...>,
                           t_logical<T1, TAGS1...>>::VALUE,
                  "logical types not allowed to do self plus operation");
    return (lh = t_logical<T, TAGS...>(get(lh) + get(rh)));
  }

  template<class T,  class... TAGS>
  constexpr
  auto operator+=(t_logical<T,  TAGS...>& lh, T rh) noexcept
      -> t_logical<T, TAGS...>& {
    static_assert(t_is_op_<t_op_self_plus_tag,
                           t_logical<T, TAGS...>,
                           t_logical<T, TAGS...>>::VALUE,
                  "logical types not allowed to do self plus operation");
    return (lh = t_logical<T, TAGS...>(get(lh) + rh));
  }

  /////////////////////////////////////////////////////////////////////////////

  template<class T,  class... TAGS, class T1, class... TAGS1>
  constexpr
  auto operator-=(t_logical<T,  TAGS...>& lh,
                  t_logical<T1, TAGS1...> rh) noexcept
      -> t_logical<T,  TAGS...>& {
    static_assert(t_is_op_<t_op_self_minus_tag,
                           t_logical<T,  TAGS...>,
                           t_logical<T1, TAGS1...>>::VALUE,
                  "logical types not allowed to do self minus operation");
    return (lh = t_logical<T, TAGS...>(get(lh) - get(rh)));
  }

  template<class T,  class... TAGS>
  constexpr
  auto operator-=(t_logical<T,  TAGS...>& lh, T rh) noexcept
      -> t_logical<T, TAGS...>& {
    static_assert(t_is_op_<t_op_self_minus_tag,
                           t_logical<T, TAGS...>,
                           t_logical<T, TAGS...>>::VALUE,
                  "logical types not allowed to do self minus operation");
    return (lh = t_logical<T, TAGS...>(get(lh) - rh));
  }

  /////////////////////////////////////////////////////////////////////////////

  template<class T,  class... TAGS, class T1, class... TAGS1>
  constexpr
  auto operator*=(t_logical<T,  TAGS...>& lh,
                  t_logical<T1, TAGS1...> rh) noexcept
      -> t_logical<T,  TAGS...>& {
    static_assert(t_is_op_<t_op_self_multiply_tag,
                           t_logical<T,  TAGS...>,
                           t_logical<T1, TAGS1...>>::VALUE,
                  "logical types not allowed to do self multiply operation");
    return (lh = t_logical<T, TAGS...>(get(lh) * get(rh)));
  }

  template<class T,  class... TAGS>
  constexpr
  auto operator*=(t_logical<T,  TAGS...>& lh, T rh) noexcept
      -> t_logical<T, TAGS...>& {
    static_assert(t_is_op_<t_op_self_multiply_tag,
                           t_logical<T, TAGS...>,
                           t_logical<T, TAGS...>>::VALUE,
                  "logical types not allowed to do self multiply operation");
    return (lh = t_logical<T, TAGS...>(get(lh) * rh));
  }

  /////////////////////////////////////////////////////////////////////////////

  template<class T,  class... TAGS, class T1, class... TAGS1>
  constexpr
  auto operator/=(t_logical<T,  TAGS...>& lh,
                  t_logical<T1, TAGS1...> rh) noexcept
      -> t_logical<T,  TAGS...>& {
    static_assert(t_is_op_<t_op_self_divide_tag,
                           t_logical<T,  TAGS...>,
                           t_logical<T1, TAGS1...>>::VALUE,
                  "logical types not allowed to do self divide operation");
    return (lh = t_logical<T, TAGS...>(get(lh) / get(rh)));
  }

  template<class T,  class... TAGS>
  constexpr
  auto operator/=(t_logical<T,  TAGS...>& lh, T rh) noexcept
      -> t_logical<T, TAGS...>& {
    static_assert(t_is_op_<t_op_self_divide_tag,
                           t_logical<T, TAGS...>,
                           t_logical<T, TAGS...>>::VALUE,
                  "logical types not allowed to do self divide operation");
    return (lh = t_logical<T, TAGS...>(get(lh) / rh));
  }

  /////////////////////////////////////////////////////////////////////////////

  template<class T,  class... TAGS, class T1, class... TAGS1>
  constexpr
  auto operator%=(t_logical<T,  TAGS...>& lh,
                  t_logical<T1, TAGS1...> rh) noexcept
      -> t_logical<T,  TAGS...>& {
    static_assert(t_is_op_<t_op_self_mod_tag,
                           t_logical<T,  TAGS...>,
                           t_logical<T1, TAGS1...>>::VALUE,
                  "logical types not allowed to do self mod operation");
    return (lh = t_logical<T, TAGS...>(get(lh) % get(rh)));
  }

  template<class T,  class... TAGS>
  constexpr
  auto operator%=(t_logical<T,  TAGS...>& lh, T rh) noexcept
      -> t_logical<T, TAGS...>& {
    static_assert(t_is_op_<t_op_self_mod_tag,
                           t_logical<T, TAGS...>,
                           t_logical<T, TAGS...>>::VALUE,
                  "logical types not allowed to do self mod operation");
    return (lh = t_logical<T, TAGS...>(get(lh) % rh));
  }

  /////////////////////////////////////////////////////////////////////////////

  template<class T,  class... TAGS, class T1, class... TAGS1>
  constexpr
  t_bool operator&&(t_logical<T,  TAGS...>  lh,
                    t_logical<T1, TAGS1...> rh) noexcept {
    static_assert(t_is_op_<t_op_and_tag,
                           t_logical<T,  TAGS...>,
                           t_logical<T1, TAGS1...>>::VALUE,
                  "logical types not allowed to do and operation");
    return get(lh) && get(rh);
  }

  template<class T,  class... TAGS, class U>
  constexpr
  t_bool operator&&(t_logical<T,  TAGS...> lh, const U& rh) noexcept {
    static_assert(t_is_op_<t_op_and_tag,
                           t_logical<T, TAGS...>,
                           t_logical<T, TAGS...>>::VALUE,
                  "logical types not allowed to do and operation");
    return get(lh) && rh;
  }

  template<class T,  class... TAGS, class U>
  constexpr
  t_bool operator&&(const U& lh, t_logical<T,  TAGS...> rh) noexcept {
    static_assert(t_is_op_<t_op_and_tag,
                           t_logical<T, TAGS...>,
                           t_logical<T, TAGS...>>::VALUE,
                  "logical types not allowed to do and operation");
    return lh && get(rh);
  }

  /////////////////////////////////////////////////////////////////////////////

  template<class T,  class... TAGS, class T1, class... TAGS1>
  constexpr
  t_bool operator||(t_logical<T,  TAGS...>  lh,
                    t_logical<T1, TAGS1...> rh) noexcept {
    static_assert(t_is_op_<t_op_or_tag,
                           t_logical<T,  TAGS...>,
                           t_logical<T1, TAGS1...>>::VALUE,
                  "logical types not allowed to do or operation");
    return get(lh) && get(rh);
  }

  template<class T,  class... TAGS, class U>
  constexpr
  t_bool operator||(t_logical<T,  TAGS...> lh, const U& rh) noexcept {
    static_assert(t_is_op_<t_op_or_tag,
                           t_logical<T, TAGS...>,
                           t_logical<T, TAGS...>>::VALUE,
                  "logical types not allowed to do or operation");
    return get(lh) && rh;
  }

  template<class T,  class... TAGS, class U>
  constexpr
  t_bool operator||(const U& lh, t_logical<T,  TAGS...> rh) noexcept {
    static_assert(t_is_op_<t_op_or_tag,
                           t_logical<T, TAGS...>,
                           t_logical<T, TAGS...>>::VALUE,
                  "logical types not allowed to do or operation");
    return lh && get(rh);
  }

  /////////////////////////////////////////////////////////////////////////////

  template<class T,  class... TAGS>
  constexpr
  auto operator-(t_logical<T,  TAGS...> lh) noexcept
      -> t_logical<T,  TAGS...> {
    static_assert(t_is_op_<t_op_negate_tag, t_logical<T,  TAGS...>>::VALUE,
                  "logical type not allowed to negate operation");
    return t_logical<T,  TAGS...>{-get(lh)};
  }

  template<class T,  class... TAGS>
  constexpr
  auto operator++(t_logical<T,  TAGS...>& lh) noexcept
      -> t_logical<T,  TAGS...>& {
    static_assert(t_is_op_<t_op_incr_tag, t_logical<T,  TAGS...>>::VALUE,
                  "logical type not allowed increment operation");
    return (lh = t_logical<T,  TAGS...>{get(lh) + 1});
  }

  template<class T,  class... TAGS>
  constexpr
  auto operator--(t_logical<T,  TAGS...>& lh) noexcept
      -> t_logical<T,  TAGS...>& {
    static_assert(t_is_op_<t_op_decr_tag, t_logical<T,  TAGS...>>::VALUE,
                  "logical type not allowed to decrement operation");
    return (lh = t_logical<T,  TAGS...>{get(lh) - 1});
  }

  template<class T,  class... TAGS>
  constexpr
  auto operator++(t_logical<T,  TAGS...>& lh, t_int) noexcept
      -> t_logical<T,  TAGS...> {
    static_assert(t_is_op_<t_op_post_incr_tag, t_logical<T,  TAGS...>>::VALUE,
                  "logical type not allowed post increment operation");
    t_logical<T,  TAGS...> tmp = lh;
    lh = t_logical<T,  TAGS...>{get(lh) + 1};
    return tmp;
  }

  template<class T,  class... TAGS>
  constexpr
  auto operator--(t_logical<T,  TAGS...>& lh, t_int) noexcept
      -> t_logical<T,  TAGS...> {
    static_assert(t_is_op_<t_op_post_decr_tag, t_logical<T,  TAGS...>>::VALUE,
                  "logical type not allowed to post decrement operation");
    t_logical<T,  TAGS...> tmp = lh;
    lh = t_logical<T,  TAGS...>{get(lh) - 1};
    return tmp;
  }

  /////////////////////////////////////////////////////////////////////////////

  template<class T,  class... TAGS, class T1, class... TAGS1>
  constexpr
  auto operator^(t_logical<T,  TAGS...>  lh,
                 t_logical<T1, TAGS1...> rh) noexcept
      -> typename impl_::t_largest_<t_logical<T,  TAGS...>,
                                    t_logical<T1, TAGS1...>>::t_value {
    static_assert(t_is_op_<t_op_bin_xor_tag,
                           t_logical<T,  TAGS...>,
                           t_logical<T1, TAGS1...>>::VALUE,
                  "logical types not allowed to do xor operation");
    return typename impl_::t_largest_<t_logical<T,  TAGS...>,
                                      t_logical<T1, TAGS1...>>::t_value
      {(get(lh) ^ get(rh))};
  }

  template<class T,  class... TAGS>
  constexpr
  auto operator^(t_logical<T,  TAGS...> lh, T rh) noexcept
      -> t_logical<T, TAGS...> {
    static_assert(t_is_op_<t_op_bin_xor_tag,
                           t_logical<T, TAGS...>,
                           t_logical<T, TAGS...>>::VALUE,
                  "logical types not allowed to do xor operation");
    return t_logical<T, TAGS...>(get(lh) ^ rh);
  }

  template<class T,  class... TAGS>
  constexpr
  auto operator^(T lh, t_logical<T,  TAGS...> rh) noexcept
      -> t_logical<T, TAGS...> {
    static_assert(t_is_op_<t_op_bin_xor_tag,
                           t_logical<T, TAGS...>,
                           t_logical<T, TAGS...>>::VALUE,
                  "logical types not allowed to do xor operation");
    return t_logical<T, TAGS...>(lh ^ get(rh));
  }

  /////////////////////////////////////////////////////////////////////////////

  template<class T,  class... TAGS, class T1, class... TAGS1>
  constexpr
  auto operator&(t_logical<T,  TAGS...>  lh,
                 t_logical<T1, TAGS1...> rh) noexcept
      -> typename impl_::t_largest_<t_logical<T,  TAGS...>,
                                    t_logical<T1, TAGS1...>>::t_value {
    static_assert(t_is_op_<t_op_bin_and_tag,
                           t_logical<T,  TAGS...>,
                           t_logical<T1, TAGS1...>>::VALUE,
                  "logical types not allowed to do & operation");
    return typename impl_::t_largest_<t_logical<T,  TAGS...>,
                                      t_logical<T1, TAGS1...>>::t_value
      {(get(lh) & get(rh))};
  }

  template<class T,  class... TAGS>
  constexpr
  auto operator&(t_logical<T,  TAGS...> lh, T rh) noexcept
      -> t_logical<T, TAGS...> {
    static_assert(t_is_op_<t_op_bin_and_tag,
                           t_logical<T, TAGS...>,
                           t_logical<T, TAGS...>>::VALUE,
                  "logical types not allowed to do & operation");
    return t_logical<T, TAGS...>(get(lh) & rh);
  }

  template<class T,  class... TAGS>
  constexpr
  auto operator&(T lh, t_logical<T,  TAGS...> rh) noexcept
      -> t_logical<T, TAGS...> {
    static_assert(t_is_op_<t_op_bin_and_tag,
                           t_logical<T, TAGS...>,
                           t_logical<T, TAGS...>>::VALUE,
                  "logical types not allowed to do & operation");
    return t_logical<T, TAGS...>(lh & get(rh));
  }

  /////////////////////////////////////////////////////////////////////////////

  template<class T,  class... TAGS, class T1, class... TAGS1>
  constexpr
  auto operator|(t_logical<T,  TAGS...>  lh,
                 t_logical<T1, TAGS1...> rh) noexcept
      -> typename impl_::t_largest_<t_logical<T,  TAGS...>,
                                    t_logical<T1, TAGS1...>>::t_value {
    static_assert(t_is_op_<t_op_bin_ior_tag,
                           t_logical<T,  TAGS...>,
                           t_logical<T1, TAGS1...>>::VALUE,
                  "logical types not allowed to do | operation");
    return typename impl_::t_largest_<t_logical<T,  TAGS...>,
                                      t_logical<T1, TAGS1...>>::t_value
      {(get(lh) | get(rh))};
  }

  template<class T,  class... TAGS>
  constexpr
  auto operator|(t_logical<T,  TAGS...> lh, T rh) noexcept
      -> t_logical<T, TAGS...> {
    static_assert(t_is_op_<t_op_bin_ior_tag,
                           t_logical<T, TAGS...>,
                           t_logical<T, TAGS...>>::VALUE,
                  "logical types not allowed to do | operation");
    return t_logical<T, TAGS...>(get(lh) | rh);
  }

  template<class T,  class... TAGS>
  constexpr
  auto operator|(T lh, t_logical<T,  TAGS...> rh) noexcept
      -> t_logical<T, TAGS...> {
    static_assert(t_is_op_<t_op_bin_ior_tag,
                           t_logical<T, TAGS...>,
                           t_logical<T, TAGS...>>::VALUE,
                  "logical types not allowed to do | operation");
    return t_logical<T, TAGS...>(lh | get(rh));
  }

  /////////////////////////////////////////////////////////////////////////////

  template<class T,  class... TAGS, class T1, class... TAGS1>
  constexpr
  auto operator<<(t_logical<T,  TAGS...>  lh,
                  t_logical<T1, TAGS1...> rh) noexcept
      -> typename impl_::t_largest_<t_logical<T,  TAGS...>,
                                    t_logical<T1, TAGS1...>>::t_value {
    static_assert(t_is_op_<t_op_bin_lshift_tag,
                           t_logical<T,  TAGS...>,
                           t_logical<T1, TAGS1...>>::VALUE,
                  "logical types not allowed to do << operation");
    return typename impl_::t_largest_<t_logical<T,  TAGS...>,
                                      t_logical<T1, TAGS1...>>::t_value
      {(get(lh) << get(rh))};
  }

  template<class T,  class... TAGS>
  constexpr
  auto operator<<(t_logical<T,  TAGS...> lh, T rh) noexcept
      -> t_logical<T, TAGS...> {
    static_assert(t_is_op_<t_op_bin_lshift_tag,
                           t_logical<T, TAGS...>,
                           t_logical<T, TAGS...>>::VALUE,
                  "logical types not allowed to do << operation");
    return t_logical<T, TAGS...>(get(lh) << rh);
  }

  template<class T,  class... TAGS>
  constexpr
  auto operator<<(T lh, t_logical<T,  TAGS...> rh) noexcept
      -> t_logical<T, TAGS...> {
    static_assert(t_is_op_<t_op_bin_lshift_tag,
                           t_logical<T, TAGS...>,
                           t_logical<T, TAGS...>>::VALUE,
                  "logical types not allowed to do << operation");
    return t_logical<T, TAGS...>(lh << get(rh));
  }

  /////////////////////////////////////////////////////////////////////////////

  template<class T,  class... TAGS, class T1, class... TAGS1>
  constexpr
  auto operator>>(t_logical<T,  TAGS...>  lh,
                  t_logical<T1, TAGS1...> rh) noexcept
      -> typename impl_::t_largest_<t_logical<T,  TAGS...>,
                                    t_logical<T1, TAGS1...>>::t_value {
    static_assert(t_is_op_<t_op_bin_lshift_tag,
                           t_logical<T,  TAGS...>,
                           t_logical<T1, TAGS1...>>::VALUE,
                  "logical types not allowed to do >> operation");
    return typename impl_::t_largest_<t_logical<T,  TAGS...>,
                                      t_logical<T1, TAGS1...>>::t_value
      {(get(lh) >> get(rh))};
  }

  template<class T,  class... TAGS>
  constexpr
  auto operator>>(t_logical<T,  TAGS...> lh, T rh) noexcept
      -> t_logical<T, TAGS...> {
    static_assert(t_is_op_<t_op_bin_lshift_tag,
                           t_logical<T, TAGS...>,
                           t_logical<T, TAGS...>>::VALUE,
                  "logical types not allowed to do >> operation");
    return t_logical<T, TAGS...>(get(lh) >> rh);
  }

  template<class T,  class... TAGS>
  constexpr
  auto operator>>(T lh, t_logical<T,  TAGS...> rh) noexcept
      -> t_logical<T, TAGS...> {
    static_assert(t_is_op_<t_op_bin_lshift_tag,
                           t_logical<T, TAGS...>,
                           t_logical<T, TAGS...>>::VALUE,
                  "logical types not allowed to do >> operation");
    return t_logical<T, TAGS...>(lh >> get(rh));
  }

  /////////////////////////////////////////////////////////////////////////////

  template<class T,  class... TAGS, class T1, class... TAGS1>
  constexpr
  auto operator^=(t_logical<T,  TAGS...>& lh,
                  t_logical<T1, TAGS1...> rh) noexcept
      -> t_logical<T,  TAGS...>& {
    static_assert(t_is_op_<t_op_bin_self_xor_tag,
                           t_logical<T,  TAGS...>,
                           t_logical<T1, TAGS1...>>::VALUE,
                  "logical types not allowed to do ^= operation");
    return (lh = t_logical<T, TAGS...>(get(lh) ^ get(rh)));
  }

  template<class T,  class... TAGS>
  constexpr
  auto operator^=(t_logical<T,  TAGS...>& lh, T rh) noexcept
      -> t_logical<T, TAGS...>& {
    static_assert(t_is_op_<t_op_bin_self_xor_tag,
                           t_logical<T, TAGS...>>::VALUE,
                  "logical types not allowed to do ^= operation");
    return (lh = t_logical<T, TAGS...>(get(lh) ^ rh));
  }

  /////////////////////////////////////////////////////////////////////////////

  template<class T,  class... TAGS, class T1, class... TAGS1>
  constexpr
  auto operator&=(t_logical<T,  TAGS...>& lh,
                  t_logical<T1, TAGS1...> rh) noexcept
      -> t_logical<T,  TAGS...>& {
    static_assert(t_is_op_<t_op_bin_self_and_tag,
                           t_logical<T,  TAGS...>,
                           t_logical<T1, TAGS1...>>::VALUE,
                  "logical types not allowed to do &= operation");
    return (lh = t_logical<T, TAGS...>(get(lh) & get(rh)));
  }

  template<class T,  class... TAGS>
  constexpr
  auto operator&=(t_logical<T,  TAGS...>& lh, T rh) noexcept
      -> t_logical<T, TAGS...>& {
    static_assert(t_is_op_<t_op_bin_self_and_tag,
                           t_logical<T, TAGS...>>::VALUE,
                  "logical types not allowed to do &= operation");
    return (lh = t_logical<T, TAGS...>(get(lh) & rh));
  }

  /////////////////////////////////////////////////////////////////////////////

  template<class T,  class... TAGS, class T1, class... TAGS1>
  constexpr
  auto operator|=(t_logical<T,  TAGS...>& lh,
                  t_logical<T1, TAGS1...> rh) noexcept
      -> t_logical<T,  TAGS...>& {
    static_assert(t_is_op_<t_op_bin_self_ior_tag,
                           t_logical<T,  TAGS...>,
                           t_logical<T1, TAGS1...>>::VALUE,
                  "logical types not allowed to do |= operation");
    return (lh = t_logical<T, TAGS...>(get(lh) | get(rh)));
  }

  template<class T,  class... TAGS>
  constexpr
  auto operator|=(t_logical<T,  TAGS...>& lh, T rh) noexcept
      -> t_logical<T, TAGS...>& {
    static_assert(t_is_op_<t_op_bin_self_ior_tag,
                           t_logical<T, TAGS...>>::VALUE,
                  "logical types not allowed to do |= operation");
    return (lh = t_logical<T, TAGS...>(get(lh) | rh));
  }

  /////////////////////////////////////////////////////////////////////////////

  template<class T,  class... TAGS, class T1, class... TAGS1>
  constexpr
  auto operator<<=(t_logical<T,  TAGS...>& lh,
                  t_logical<T1, TAGS1...> rh) noexcept
      -> t_logical<T,  TAGS...>& {
    static_assert(t_is_op_<t_op_bin_self_lshift_tag,
                           t_logical<T,  TAGS...>,
                           t_logical<T1, TAGS1...>>::VALUE,
                  "logical types not allowed to do <<= operation");
    return (lh = t_logical<T, TAGS...>(get(lh) << get(rh)));
  }

  template<class T,  class... TAGS>
  constexpr
  auto operator<<=(t_logical<T,  TAGS...>& lh, T rh) noexcept
      -> t_logical<T, TAGS...>& {
    static_assert(t_is_op_<t_op_bin_self_lshift_tag,
                           t_logical<T, TAGS...>>::VALUE,
                  "logical types not allowed to do <<= operation");
    return (lh = t_logical<T, TAGS...>(get(lh) << rh));
  }

  /////////////////////////////////////////////////////////////////////////////

  template<class T,  class... TAGS, class T1, class... TAGS1>
  constexpr
  auto operator>>=(t_logical<T,  TAGS...>& lh,
                  t_logical<T1, TAGS1...> rh) noexcept
      -> t_logical<T,  TAGS...>& {
    static_assert(t_is_op_<t_op_bin_self_rshift_tag,
                           t_logical<T,  TAGS...>,
                           t_logical<T1, TAGS1...>>::VALUE,
                  "logical types not allowed to do >>= operation");
    return (lh = t_logical<T, TAGS...>(get(lh) >> get(rh)));
  }

  template<class T,  class... TAGS>
  constexpr
  auto operator>>=(t_logical<T,  TAGS...>& lh, T rh) noexcept
      -> t_logical<T, TAGS...>& {
    static_assert(t_is_op_<t_op_bin_self_rshift_tag,
                           t_logical<T, TAGS...>>::VALUE,
                  "logical types not allowed to do >>= operation");
    return (lh = t_logical<T, TAGS...>(get(lh) >> rh));
  }

  /////////////////////////////////////////////////////////////////////////////

  struct t_i_tag_ {
    using t_ops = t_pack<t_ops_value_tag>;
  };
  using t_i = t_logical<types::t_int64, t_i_tag_>;

  struct t_u_tag_ {
    using t_ops = t_ops_value_tag;
  };
  using t_u = t_logical<types::t_uint64, t_u_tag_>;

  /////////////////////////////////////////////////////////////////////////////

  struct t_n_tag_ : t_u_tag_ {
  };
  enum  t_max_n_tag_ {};
  enum  t_min_n_tag_ {};
  enum  t_by_n_tag_  {};

  using t_n     = t_logical<types::t_n_, t_n_tag_,     t_i>;
  using t_max_n = t_logical<types::t_n_, t_max_n_tag_, t_n>;
  using t_min_n = t_logical<types::t_n_, t_min_n_tag_, t_n>;
  using t_by_n  = t_logical<types::t_n_, t_by_n_tag_,  t_n>;

  /////////////////////////////////////////////////////////////////////////////

  struct t_ix_tag_ : t_n_tag_ {
  };
  enum t_begin_ix_tag_ {};
  enum t_end_ix_tag_   {};

  using t_ix       = t_logical<types::t_ix_,  t_ix_tag_,       t_n>;
  using t_begin_ix = t_logical<types::t_ix_,  t_begin_ix_tag_, t_ix>;
  using t_end_ix   = t_logical<types::t_ix_,  t_end_ix_tag_,   t_ix>;

  /////////////////////////////////////////////////////////////////////////////

  // max_of
  // min_of

  /////////////////////////////////////////////////////////////////////////////

  constexpr
  t_n operator""_n(unsigned long long value) noexcept {
    return t_n{value};
  }

  constexpr
  t_min_n operator""_min_n(unsigned long long value) noexcept {
    return t_min_n{value};
  }

  constexpr
  t_max_n operator""_max_n(unsigned long long value) noexcept {
    return t_max_n{value};
  }

  constexpr
  t_ix operator""_ix(unsigned long long value) noexcept {
    return t_ix{value};
  }

  constexpr
  t_begin_ix operator""_begin_ix(unsigned long long value) noexcept {
    return t_begin_ix{value};
  }

  constexpr
  t_end_ix operator""_end_ix(unsigned long long value) noexcept {
    return t_end_ix{value};
  }

  /////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
