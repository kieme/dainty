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
///////////////////////////////////////////////////////////////////////////////

  using types::t_prefix;
  using types::t_bool;
  using types::t_n_;
  using types::t_n_tag_;
  using types::t_ix_;
  using types::t_ix_tag_;

  using traits::t_pack;
  using traits::t_least_one_is_true;
  using traits::t_is_fundamental;
  using traits::t_is_ptr;
  using traits::t_flatten_tree;
  using traits::t_is_subset_of;
  using traits::t_add_result;
  using traits::t_or;
  using traits::t_and;
  using traits::t_is_one_of;

  using traits::t_result_false;
  using traits::t_result_true;

///////////////////////////////////////////////////////////////////////////////

  template<class, class TAG, class...>
  class t_logical;

  template<class T, class TAG, class... TAGS>
  constexpr
  T get(t_logical<T, TAG, TAGS...>) noexcept;

///////////////////////////////////////////////////////////////////////////////

  template<class T, class TAG, class... TAGS>
  class t_logical {
  public:
    static_assert(t_least_one_is_true<T,
                    t_is_fundamental, t_is_ptr>::VALUE, "wrong type");

    using t_value   = T;
    using t_tags    = t_pack<TAG, TAGS...>;
    using t_flatten = typename t_flatten_tree<t_tags>::t_value;

    constexpr
    explicit t_logical(t_value value) noexcept : value_{value} {
      // XXX - tests
    }

    template<typename T1, typename... TAGS1>
    constexpr
    t_logical(t_logical<T1, TAGS1...> logical) noexcept
        : value_{logical.value_} {
      static_assert(t_is_subset_of<t_flatten,
                      typename t_logical<T1, TAGS1...>::t_flatten>::VALUE,
                     "cannot copy type. types don't give permission.");
    }

    template<typename T1, typename... TAGS1>
    constexpr
    t_logical& operator=(t_logical<T1, TAGS1...> logical) noexcept {
      static_assert(t_is_subset_of<t_flatten,
                      typename t_logical<T1, TAGS1...>::t_flatten>::VALUE,
                     "cannot copy type. types don't give permission.");
      value_ = logical.value_;
      return *this;
    }

    t_logical()                                     = delete;  // for clarity
    t_logical(const t_logical&)            noexcept = default; // for clarity
    t_logical& operator=(const t_logical&) noexcept = default; // for clarity

  private:
    template<class T1, class TAG1, class... TAGS1> friend class t_logical;
    template<class T1, class TAG1, class... TAGS1>
    friend constexpr T1 get(t_logical<T1, TAG1, TAGS1...>) noexcept;
    t_value value_;
  };

///////////////////////////////////////////////////////////////////////////////

  template<class T, class TAG, class... TAGS>
  constexpr
  T get(t_logical<T, TAG, TAGS...> logical) noexcept { return logical.value_; }

///////////////////////////////////////////////////////////////////////////////

  template<class>
  struct t_is_logical : t_result_false { };

  template<class T, class TAG, class... TAGS>
  struct t_is_logical<t_logical<T, TAG, TAGS...>> : t_result_true { };

///////////////////////////////////////////////////////////////////////////////

namespace impl_ {
  template<typename, typename>
  struct t_is_allowed_ : t_result_false { };

  template<class T,  class... TAGS, class T1, class... TAGS1>
  struct t_is_allowed_<t_logical<T,  TAGS...>, t_logical<T1, TAGS1...>>
    : t_add_result<
        t_or<t_is_subset_of<
          typename t_logical<T,  TAGS... >::t_flatten,
          typename t_logical<T1, TAGS1...>::t_flatten>,
        t_is_subset_of<
          typename t_logical<T1, TAGS1... >::t_flatten,
          typename t_logical<T,  TAGS...>::t_flatten>>> {
  };

  template<typename, typename, typename>
  struct t_is_op_allowed_ : t_result_false { };

  template<class... Cs, class T,  class... TAGS, class T1, class... TAGS1>
  struct t_is_op_allowed_<t_pack<Cs...>, t_logical<T,  TAGS...>,
                                         t_logical<T1, TAGS1...>> {
    using t_0_ = typename t_flatten_tree<Cs...>::t_value;
    using t_1_ = typename t_logical<T,  TAGS... >::t_flatten;
    using t_2_ = typename t_logical<T1, TAGS1...>::t_flatten;
    using t_result = t_and<
                       t_and<
                         t_or<
                           t_is_subset_of<t_1_, t_2_>,
                           t_is_subset_of<t_2_, t_1_>>,
                         t_is_subset_of<t_0_, t_1_>>,
                       t_is_subset_of<t_0_, t_2_>>;
  };
}

template<typename T, typename T1>
struct t_is_allowed : impl_::t_is_allowed_<T, T1>::t_result { };

template<typename C, typename T, typename T1>
struct t_is_op_allowed : impl_::t_is_op_allowed_<C, T, T1>::t_result { };

///////////////////////////////////////////////////////////////////////////////

  enum t_allow_op_less_tag         { };
  enum t_allow_op_less_equal_tag   { };
  enum t_allow_op_larger_tag       { };
  enum t_allow_op_larger_equal_tag { };
  enum t_allow_op_equal_tag        { };
  enum t_allow_op_not_equal_tag    { };

///////////////////////////////////////////////////////////////////////////////

  enum t_allow_op_plus_tag     { };
  enum t_allow_op_minus_tag    { };
  enum t_allow_op_multiply_tag { };
  enum t_allow_op_divide_tag   { };
  enum t_allow_op_mod_tag      { };

///////////////////////////////////////////////////////////////////////////////

  template<class T,  class... TAGS, class T1, class... TAGS1>
  constexpr
  t_bool operator==(t_logical<T,  TAGS...>  lh,
                    t_logical<T1, TAGS1...> rh) noexcept {
    static_assert(
      t_is_op_allowed<t_pack<t_allow_op_equal_tag>,
                      t_logical<T,  TAGS...>,
                      t_logical<T1, TAGS1...>>::VALUE,
           "logical types not allowed to do equal operation");
    return get(lh) == get(rh);
  }

  template<class T,  class... TAGS, class T1, class... TAGS1>
  constexpr
  t_bool operator!=(t_logical<T,  TAGS...>  lh,
                    t_logical<T1, TAGS1...> rh) noexcept {
    static_assert(
      t_is_op_allowed<t_pack<t_allow_op_not_equal_tag>,
                      t_logical<T,  TAGS...>,
                      t_logical<T1, TAGS1...>>::VALUE,
           "logical types not allowed to do not_equal operation");
    return get(lh) != get(rh);
  }

  template<class T,  class... TAGS, class T1, class... TAGS1>
  constexpr
  t_bool operator<(t_logical<T,  TAGS...>  lh,
                   t_logical<T1, TAGS1...> rh) noexcept {
    static_assert(
      t_is_op_allowed<t_pack<t_allow_op_less_tag>,
                      t_logical<T,  TAGS...>,
                      t_logical<T1, TAGS1...>>::VALUE,
           "logical types not allowed to do less operation");
    return get(lh) < get(rh);
  }

  template<class T,  class... TAGS, class T1, class... TAGS1>
  constexpr
  t_bool operator>(t_logical<T,  TAGS...>  lh,
                   t_logical<T1, TAGS1...> rh) noexcept {
    static_assert(
      t_is_op_allowed<t_pack<t_allow_op_larger_tag>,
                      t_logical<T,  TAGS...>,
                      t_logical<T1, TAGS1...>>::VALUE,
           "logical types not allowed to do larger operation");
    return get(lh) > get(rh);
  }

  template<class T,  class... TAGS, class T1, class... TAGS1>
  constexpr
  t_bool operator<=(t_logical<T,  TAGS...>  lh,
                    t_logical<T1, TAGS1...> rh) noexcept {
    static_assert(
      t_is_op_allowed<t_pack<t_allow_op_less_equal_tag>,
                      t_logical<T,  TAGS...>,
                      t_logical<T1, TAGS1...>>::VALUE,
           "logical types not allowed to do less_equal operation");
    return get(lh) <= get(rh);
  }

  template<class T,  class... TAGS, class T1, class... TAGS1>
  constexpr
  t_bool operator>=(t_logical<T,  TAGS...>  lh,
                    t_logical<T1, TAGS1...> rh) noexcept {
    static_assert(
      t_is_op_allowed<t_pack<t_allow_op_larger_equal_tag>,
                      t_logical<T,  TAGS...>,
                      t_logical<T1, TAGS1...>>::VALUE,
           "logical types not allowed to do larger_equal operation");
    return get(lh) >= get(rh);
  }

///////////////////////////////////////////////////////////////////////////////

  template<class T,  class... TAGS, class T1, class... TAGS1>
  constexpr
  auto operator+(t_logical<T,  TAGS...>  lh,
                 t_logical<T1, TAGS1...> rh) noexcept {
    static_assert(
      t_is_op_allowed<t_pack<t_allow_op_plus_tag>,
                      t_logical<T,  TAGS...>,
                      t_logical<T1, TAGS1...>>::VALUE,
           "logical types not allowed to do plus operation");
    using t_0_ = typename t_logical<T,  TAGS... >::t_flatten;
    using t_1_ = typename t_logical<T1, TAGS1...>::t_flatten;
    using t_result_ =
      typename traits::t_bool_result<(t_0_::N > t_1_::N)>::t_result;
    using t_value_ = typename traits::t_if_then_else<t_result_,
      t_logical<T,  TAGS...>,
      t_logical<T1, TAGS1...>>::t_value;
    return t_value_(get(lh) + get(rh));
  }

  template<class T,  class... TAGS>
  constexpr
  auto operator+(t_logical<T,  TAGS...> lh, T rh) noexcept
      -> t_logical<T, TAGS...> {
    static_assert(
      t_is_op_allowed<t_pack<t_allow_op_plus_tag>,
                      t_logical<T, TAGS...>,
                      t_logical<T, TAGS...>>::VALUE,
           "logical types not allowed to do plus operation");
    return t_logical<T, TAGS...>(get(lh) + rh);
  }

  template<class T,  class... TAGS>
  constexpr
  auto operator+(T lh, t_logical<T,  TAGS...> rh) noexcept
      -> t_logical<T, TAGS...> {
    static_assert(
      t_is_op_allowed<t_pack<t_allow_op_plus_tag>,
                      t_logical<T, TAGS...>,
                      t_logical<T, TAGS...>>::VALUE,
           "logical types not allowed to do plus operation");
    return t_logical<T, TAGS...>(lh + get(rh));
  }

///////////////////////////////////////////////////////////////////////////////

  template<class T,  class... TAGS, class T1, class... TAGS1>
  constexpr
  auto operator-(t_logical<T,  TAGS...>  lh,
                 t_logical<T1, TAGS1...> rh) noexcept {
    static_assert(
      t_is_op_allowed<t_pack<t_allow_op_minus_tag>,
                      t_logical<T,  TAGS...>,
                      t_logical<T1, TAGS1...>>::VALUE,
           "logical types not allowed to do minus operation");
    using t_0_ = typename t_logical<T,  TAGS... >::t_flatten;
    using t_1_ = typename t_logical<T1, TAGS1...>::t_flatten;
    using t_result_ =
      typename traits::t_bool_result<(t_0_::N > t_1_::N)>::t_result;
    using t_value_ = typename traits::t_if_then_else<t_result_,
      t_logical<T,  TAGS...>,
      t_logical<T1, TAGS1...>>::t_value;
    return t_value_(get(lh) - get(rh));
  }

  template<class T,  class... TAGS>
  constexpr
  auto operator-(t_logical<T,  TAGS...> lh, T rh) noexcept
      -> t_logical<T, TAGS...> {
    static_assert(
      t_is_op_allowed<t_pack<t_allow_op_minus_tag>,
                      t_logical<T, TAGS...>,
                      t_logical<T, TAGS...>>::VALUE,
           "logical types not allowed to do minus operation");
    return t_logical<T, TAGS...>(get(lh) - rh);
  }

  template<class T,  class... TAGS>
  constexpr
  auto operator-(T lh, t_logical<T,  TAGS...> rh) noexcept
      -> t_logical<T, TAGS...> {
    static_assert(
      t_is_op_allowed<t_pack<t_allow_op_minus_tag>,
                      t_logical<T, TAGS...>,
                      t_logical<T, TAGS...>>::VALUE,
           "logical types not allowed to do minus operation");
    return t_logical<T, TAGS...>(lh - get(rh));
  }

///////////////////////////////////////////////////////////////////////////////

  template<class T,  class... TAGS, class T1, class... TAGS1>
  constexpr
  auto operator*(t_logical<T,  TAGS...>  lh,
                 t_logical<T1, TAGS1...> rh) noexcept {
    static_assert(
      t_is_op_allowed<t_pack<t_allow_op_multiply_tag>,
                      t_logical<T,  TAGS...>,
                      t_logical<T1, TAGS1...>>::VALUE,
           "logical types not allowed to do multiply operation");
    using t_0_ = typename t_logical<T,  TAGS... >::t_flatten;
    using t_1_ = typename t_logical<T1, TAGS1...>::t_flatten;
    using t_result_ =
      typename traits::t_bool_result<(t_0_::N > t_1_::N)>::t_result;
    using t_value_ = typename traits::t_if_then_else<t_result_,
      t_logical<T,  TAGS...>,
      t_logical<T1, TAGS1...>>::t_value;
    return t_value_(get(lh) * get(rh));
  }

  template<class T,  class... TAGS>
  constexpr
  auto operator*(t_logical<T,  TAGS...> lh, T rh) noexcept
      -> t_logical<T, TAGS...> {
    static_assert(
      t_is_op_allowed<t_pack<t_allow_op_multiply_tag>,
                      t_logical<T, TAGS...>,
                      t_logical<T, TAGS...>>::VALUE,
           "logical types not allowed to do multiply operation");
    return t_logical<T, TAGS...>(get(lh) * rh);
  }

  template<class T,  class... TAGS>
  constexpr
  auto operator*(T lh, t_logical<T,  TAGS...> rh) noexcept
      -> t_logical<T, TAGS...> {
    static_assert(
      t_is_op_allowed<t_pack<t_allow_op_multiply_tag>,
                      t_logical<T, TAGS...>,
                      t_logical<T, TAGS...>>::VALUE,
           "logical types not allowed to do multiply operation");
    return t_logical<T, TAGS...>(lh * get(rh));
  }

///////////////////////////////////////////////////////////////////////////////

  template<class T,  class... TAGS, class T1, class... TAGS1>
  constexpr
  auto operator/(t_logical<T,  TAGS...>  lh,
                 t_logical<T1, TAGS1...> rh) noexcept {
    static_assert(
      t_is_op_allowed<t_pack<t_allow_op_divide_tag>,
                      t_logical<T,  TAGS...>,
                      t_logical<T1, TAGS1...>>::VALUE,
           "logical types not allowed to do divide operation");
    using t_0_ = typename t_logical<T,  TAGS... >::t_flatten;
    using t_1_ = typename t_logical<T1, TAGS1...>::t_flatten;
    using t_result_ =
      typename traits::t_bool_result<(t_0_::N > t_1_::N)>::t_result;
    using t_value_ = typename traits::t_if_then_else<t_result_,
      t_logical<T,  TAGS...>,
      t_logical<T1, TAGS1...>>::t_value;
    return t_value_(get(lh) / get(rh));
  }

  template<class T,  class... TAGS>
  constexpr
  auto operator/(t_logical<T,  TAGS...> lh, T rh) noexcept
      -> t_logical<T, TAGS...> {
    static_assert(
      t_is_op_allowed<t_pack<t_allow_op_divide_tag>,
                      t_logical<T, TAGS...>,
                      t_logical<T, TAGS...>>::VALUE,
           "logical types not allowed to do divide operation");
    return t_logical<T, TAGS...>(get(lh) / rh);
  }

  template<class T,  class... TAGS>
  constexpr
  auto operator/(T lh, t_logical<T,  TAGS...> rh) noexcept
      -> t_logical<T, TAGS...> {
    static_assert(
      t_is_op_allowed<t_pack<t_allow_op_divide_tag>,
                      t_logical<T, TAGS...>,
                      t_logical<T, TAGS...>>::VALUE,
           "logical types not allowed to do divide operation");
    return t_logical<T, TAGS...>(lh / get(rh));
  }

///////////////////////////////////////////////////////////////////////////////

  template<class T,  class... TAGS, class T1, class... TAGS1>
  constexpr
  auto operator%(t_logical<T,  TAGS...>  lh,
                 t_logical<T1, TAGS1...> rh) noexcept {
    static_assert(
      t_is_op_allowed<t_pack<t_allow_op_mod_tag>,
                      t_logical<T,  TAGS...>,
                      t_logical<T1, TAGS1...>>::VALUE,
           "logical types not allowed to do mod operation");
    using t_0_ = typename t_logical<T,  TAGS... >::t_flatten;
    using t_1_ = typename t_logical<T1, TAGS1...>::t_flatten;
    using t_result_ =
      typename traits::t_bool_result<(t_0_::N > t_1_::N)>::t_result;
    using t_value_ = typename traits::t_if_then_else<t_result_,
      t_logical<T,  TAGS...>,
      t_logical<T1, TAGS1...>>::t_value;
    return t_value_(get(lh) % get(rh));
  }

  template<class T,  class... TAGS>
  constexpr
  auto operator%(t_logical<T,  TAGS...> lh, T rh) noexcept
      -> t_logical<T, TAGS...> {
    static_assert(
      t_is_op_allowed<t_pack<t_allow_op_mod_tag>,
                      t_logical<T, TAGS...>,
                      t_logical<T, TAGS...>>::VALUE,
           "logical types not allowed to do mod operation");
    return t_logical<T, TAGS...>(get(lh) % rh);
  }

  template<class T,  class... TAGS>
  constexpr
  auto operator%(T lh, t_logical<T,  TAGS...> rh) noexcept
      -> t_logical<T, TAGS...> {
    static_assert(
      t_is_op_allowed<t_pack<t_allow_op_mod_tag>,
                      t_logical<T, TAGS...>,
                      t_logical<T, TAGS...>>::VALUE,
           "logical types not allowed to do mod operation");
    return t_logical<T, TAGS...>(lh % get(rh));
  }

///////////////////////////////////////////////////////////////////////////////

  using t_allow_ops_compare_tag = traits::t_pack<t_allow_op_less_tag,
                                                 t_allow_op_less_equal_tag,
                                                 t_allow_op_larger_tag,
                                                 t_allow_op_larger_equal_tag,
                                                 t_allow_op_equal_tag,
                                                 t_allow_op_not_equal_tag>;

  using t_allow_ops_arithmetic_tag = traits::t_pack<t_allow_op_plus_tag,
                                                    t_allow_op_minus_tag,
                                                    t_allow_op_multiply_tag,
                                                    t_allow_op_divide_tag,
                                                    t_allow_op_mod_tag>;

  using t_allow_ops_value_tag = traits::t_pack<t_allow_ops_compare_tag,
                                               t_allow_ops_arithmetic_tag>;

///////////////////////////////////////////////////////////////////////////////

  using t_n = t_logical<types::t_n_, types::t_n_tag_, t_allow_ops_value_tag>;

  enum  t_max_n_tag_ {};
  using t_max_n = t_logical<types::t_n_, t_n::t_tags, t_max_n_tag_>;

  enum  t_min_n_tag_ {};
  using t_min_n = t_logical<types::t_n_, t_n::t_tags, t_min_n_tag_>;

  enum  t_by_n_tag_ {};
  using t_by_n = t_logical<types::t_n_, t_n::t_tags,  t_by_n_tag_>;

///////////////////////////////////////////////////////////////////////////////

  using t_ix = t_logical<types::t_ix_, types::t_ix_tag_, t_allow_ops_value_tag>;

  enum  t_begin_ix_tag_ {};
  using t_begin_ix = t_logical<types::t_ix_, t_ix::t_tags, t_begin_ix_tag_>;

  enum  t_end_ix_tag_ {};
  using t_end_ix   = t_logical<types::t_ix_, t_ix::t_tags, t_end_ix_tag_>;

///////////////////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
