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

#ifndef _DAINTY_BASE_LOGICAL_IMPL_H_
#define _DAINTY_BASE_LOGICAL_IMPL_H_

#include "dainty_base_types.h"
#include "dainty_base_traits.h"

namespace dainty
{
namespace base
{
namespace logical
{
namespace impl_
{
  /////////////////////////////////////////////////////////////////////////////

  using types::t_void;
  using types::t_bool;
  using types::t_int;
  using types::t_pack;
  using types::t_empty_pack;

  using traits::t_opt1;
  using traits::t_opt2;
  using traits::t_opt3;
  using traits::t_undef;
  using traits::t_and;
  using traits::t_or;
  using traits::t_dummy;
  using traits::t_least_one_is_true;
  using traits::t_unique_flatten_pack;
  using traits::t_diff_pack;
  using traits::t_largest_pack;
  using traits::t_add_value;
  using traits::t_value_of;
  using traits::t_add_result;
  using traits::t_result_of;
  using traits::t_false;
  using traits::t_true;
  using traits::t_rfalse;
  using traits::t_rtrue;
  using traits::t_not;
  using traits::t_if_there;
  using traits::t_if;
  using traits::t_if_then_else;
  using traits::t_if_same;
  using traits::t_if_not_same;
  using traits::t_if_pack;
  using traits::t_if_not_there;
  using traits::t_if_subset_of_pack;
  using traits::t_is_integral;
  using traits::t_is_arithmetic;
  using traits::t_is_precision;
  using traits::t_is_ptr;
  using traits::t_is_signed;
  using traits::t_is_same;
  using traits::t_is_there;
  using traits::t_is_pack;
  using traits::t_is_in_pack;
  using traits::t_is_subset_of_pack;
  using traits::t_is_same_integral_sign;
  using traits::t_is_greater_equal_int_rank;
  using traits::t_is_greater_int_rank;
  using traits::t_is_truth;
  using traits::t_property;
  using traits::uneval;
  using traits::YES;
  using traits::OPT1;
  using traits::OPT2;
  using traits::OPT3;

  /////////////////////////////////////////////////////////////////////////////

  template<typename, typename, typename...> class t_logical;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename>
    struct t_is_logical_help_ : t_rfalse { };

    template<typename T, typename TAG, typename... Ls>
    struct t_is_logical_help_<t_logical<T, TAG, Ls...>> : t_rtrue { };

    template<typename L, typename... Ls>
    struct t_is_logical_
      : t_add_result<
          t_and<t_result_of<t_is_logical_help_<L>>,
                t_result_of<t_is_logical_help_<Ls>>...>> { };
  }

  template<typename L, typename... Ls>
  using t_is_logical = t_result_of<help_::t_is_logical_<L, Ls...>>;
  template<typename L, typename... Ls>
  using t_is_not_logical = t_not<t_is_logical<L, Ls...>>;
  template<typename L, typename... Ls>
  using t_if_logical     = t_if<t_is_logical<L, Ls...>>;
  template<typename L, typename... Ls>
  using t_if_not_logical = t_if<t_is_not_logical<L, Ls...>>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T> using t_ops_of = typename T::t_ops;
  template<typename T> using t_tag_of = typename T::t_tag;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename T>
    struct t_tags_of_ : t_add_value<typename T::t_tags> {
      static_assert(t_is_logical<T>::VALUE,
                    "template parameter must be of t_logical<...> type");
    };
  }

  template<typename T>
  using t_tags_of = t_value_of<help_::t_tags_of_<T>>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename...> struct t_logical_test_help_;

    template<typename L, typename... Ls>
    struct t_logical_test_help_<t_true, L, Ls...>
      : t_add_result<t_is_logical<L, Ls...>> { };

    template<>
    struct t_logical_test_help_<t_false> : t_rtrue { };

    template<typename... Ls>
    struct t_logical_test_
      : t_logical_test_help_<t_is_truth<(sizeof...(Ls) > 0)>, Ls...> { };
  }

  template<typename... Ls>
  using t_logical_test_ = t_result_of<help_::t_logical_test_<Ls...>>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename L, typename L1>
  using t_largest_ =
    t_if_then_else<t_is_same<t_largest_pack<t_tags_of<L>, t_tags_of<L1>>,
                             t_tags_of<L>>,
                   L, L1>;

  template<typename T, typename T1>
  using t_is_subset_of_ = t_is_subset_of_pack<T, T1>;

  template<typename T, typename T1>
  using t_is_related_ = t_or<t_is_subset_of_<t_tags_of<T>,  t_tags_of<T1>>,
                             t_is_subset_of_<t_tags_of<T1>, t_tags_of<T>>>;

  template<typename O, typename T, typename T1 = t_dummy>
  struct t_is_op_ : t_and<t_is_in_pack<O, t_ops_of<T>, t_ops_of<T1>>,
                          t_is_related_<T, T1>> { };

  template<typename O, typename T>
  struct t_is_op_<O, T, t_dummy> : t_is_in_pack<O, t_ops_of<T>> { };

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename T>
    using t_has_ops_ = t_if_pack<typename T::t_ops>;

    template<typename, typename>
    struct t_get_ops_help_ : t_add_value<t_empty_pack> { };

    template<typename T>
    struct t_get_ops_help_<T, t_true>
      : t_add_value<t_unique_flatten_pack<t_ops_of<T>>> {
    };

    template<typename T>
    struct t_get_ops_ : t_get_ops_help_<T, t_is_there<t_has_ops_, T>> { };
  }

  template<typename TAG>
  using t_get_ops_ = t_value_of<help_::t_get_ops_<TAG>>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename, typename...> struct t_get_tags_help_;

    template<typename T>
    struct t_get_tags_help_<t_pack<T>> : t_add_value<t_pack<T>> { };

    template<typename... Ts, typename T, typename TAG, typename... Ls>
    struct t_get_tags_help_<t_pack<Ts...>, t_logical<T, TAG, Ls...>>
      : t_add_value<t_pack<Ts...,
                           typename t_logical<T, TAG, Ls...>::t_tags>> { };

    template<typename... Ts, typename T, typename TAG, typename... Ls,
             typename... Ls1>
    struct t_get_tags_help_<t_pack<Ts...>, t_logical<T, TAG, Ls...>, Ls1...>
      : t_get_tags_help_<
         t_pack<Ts..., typename t_logical<T, TAG, Ls...>::t_tags>, Ls1...> { };

    template<typename TAG, typename... Ls>
    struct t_get_tags_
      : t_add_value<t_unique_flatten_pack<
          t_value_of<t_get_tags_help_<t_pack<TAG>, Ls...>>>> { };
  }

  template<typename TAG, typename... Ls>
  using t_get_tags_ = t_value_of<help_::t_get_tags_<TAG, Ls...>>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename...> struct t_is_logical_data_help_;
    template<typename...> struct t_is_logical_data_help1_;
    template<typename...> struct t_is_logical_data_help2_;

    template<typename T, typename T1>
    struct t_is_logical_data_help2_<t_false, T, T1> : t_rfalse { };

    template<typename T, typename T1>
    struct t_is_logical_data_help2_<t_true, T, T1>
      : t_add_result<t_is_truth<(t_property<T> ::SIZEOF >=
                                 t_property<T1>::SIZEOF)>> { };

    template<typename T, typename T1>
    struct t_is_logical_data_help1_<t_false, T, T1>
      : t_is_logical_data_help2_<t_is_precision<T, T1>, T, T1> { };

    template<typename T, typename T1>
    struct t_is_logical_data_help1_<t_true, T, T1> : t_rtrue { };

    template<typename T, typename T1>
    struct t_is_logical_data_help_<t_false, T, T1>
      : t_is_logical_data_help1_<t_is_ptr<T, T1>, T, T1> { };

    template<typename T, typename T1>
    struct t_is_logical_data_help_<t_true, T, T1>
      : t_add_result<t_or<t_and<t_is_same_integral_sign<T, T1>,
                                t_is_greater_equal_int_rank<T, T1>>,
                          t_and<t_is_signed<T>,
                                t_is_greater_int_rank<T, T1>>>> { };

    template<typename T, typename T1>
    struct t_is_logical_data_
      : t_is_logical_data_help_<t_is_integral<T, T1>, T, T1> { };
  }

  template<typename T, typename T1>
  using t_is_logical_data = t_result_of<help_::t_is_logical_data_<T, T1>>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename T1>
  constexpr T mk_(T1 value) noexcept {
    return (T)value;
  }
  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename T>
    using t_has_check_ =
      t_if_same<decltype(uneval<T>().check(typename T::t_value())),
                typename T::t_value>;

    template<typename TAG, typename T, t_if_there<t_has_check_, TAG> = YES>
    constexpr
    typename TAG::t_value check_(T value) noexcept {
      static_assert(t_is_logical_data<typename TAG::t_value, T>::VALUE,
                    "check type can overflow on value");
      return TAG::check(value);
    }

    template<typename TAG, typename T, t_if_not_there<t_has_check_, TAG> = YES>
    constexpr
    T check_(T value) noexcept {
      return value;
    }

    template<typename> struct t_check_help_;

    template<typename TAG, typename... TAGs>
    struct t_check_help_<t_pack<TAG, TAGs...>> {
      template<typename T>
      static constexpr T call(T value) noexcept {
        return t_check_help_<t_pack<TAGs...>>::call(check_<TAG>(value));
      }
    };

    template<typename TAG>
    struct t_check_help_<t_pack<TAG>> {
      template<typename T>
      static constexpr T call(T value) noexcept {
        return check_<TAG>(value);
      }
    };

    template<typename P>
    struct t_check_ : t_check_help_<P> {
      static_assert(t_is_pack<P>::VALUE,
                     "template parameter must be of t_pack<...> type");
    };
  }

  template<typename P> using t_check_ = help_::t_check_<P>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename TAG, typename... Ls>
  constexpr
  T get_(t_logical<T, TAG, Ls...>) noexcept;

  template<typename T, typename TAG, typename... Ls>
  constexpr
  t_logical<T, TAG, Ls...>& set_(t_logical<T, TAG, Ls...>&, T) noexcept;

  template<typename T,   typename TAG,  typename... Ls,
            typename T1, typename TAG1, typename... Ls1>
  constexpr
  t_logical<T, TAG, Ls...>& set_(t_logical<T,  TAG,  Ls...>&,
                                 t_logical<T1, TAG1, Ls1...>) noexcept;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename TAG, typename... Ls>
  class t_logical {
  public:
    static_assert(t_least_one_is_true<T, t_is_arithmetic, t_is_ptr>::VALUE,
                  "wrong data type");

    static_assert(t_logical_test_<Ls...>::VALUE,
                  "Ls... must be logical<...> types");

    using t_value = T;
    using t_tag   = TAG;
    using t_ops   = t_get_ops_<TAG>;
    using t_tags  = t_get_tags_<TAG, Ls...>;

    template<typename T1, t_if_same<T, T1> = YES>
    constexpr
    explicit t_logical(T1 value) noexcept
      : value_{t_check_<t_tags>::call(value)} {
    }

    template<typename T1, t_if_not_same<T, T1> = YES>
    constexpr
    explicit t_logical(T1 value) noexcept
      : value_{t_check_<t_tags>::call(mk_<T>(value))} {
  //    static_assert(t_is_logical_data<T, T1>::VALUE,
  //                  "cannot copy type. data too large or wrong sign");
    }

    template<typename T1, typename TAG1, typename... L1s>
    constexpr
    t_logical(t_logical<T1, TAG1, L1s...> logical) noexcept
        : value_{logical.value_} {
      static_assert(t_is_subset_of_<t_tags,
                      t_tags_of<t_logical<T1, TAG1, L1s...>>>::VALUE,
                    "cannot copy type. types don't give permission.");
      static_assert(t_is_logical_data<T, T1>::VALUE,
                    "cannot copy type. data too large or wrong sign");
    }

    template<typename T1, typename TAG1, typename... L1s>
    constexpr
    t_logical& operator=(t_logical<T1, TAG1, L1s...> logical) noexcept {
      static_assert(t_is_subset_of_<t_tags,
                      t_tags_of<t_logical<T1, TAG1, L1s...>>>::VALUE,
                     "cannot copy type. types don't give permission.");
      static_assert(t_is_logical_data<T, T1>::VALUE,
                    "cannot copy type. data too large or wrong sign");
      value_ = logical.value_;
      return *this;
    }

    t_logical()                                     = delete;  // for clarity
    t_logical(const t_logical&)            noexcept = default; // for clarity
    t_logical& operator=(const t_logical&) noexcept = default; // for clarity

    constexpr
    explicit operator t_bool() const noexcept { return value_; }

  private:
    template<typename, typename, typename...> friend class t_logical;

    template<typename T1, typename TAG1, typename... Ls1>
    friend constexpr
    T1 get_(t_logical<T1, TAG1, Ls1...>) noexcept;

    template<typename T1, typename TAG1, typename... Ls1>
    friend constexpr
    t_logical<T1, TAG1, Ls1...>& set_(t_logical<T1, TAG1, Ls1...>&,
                                      T1) noexcept;

    template<typename T1, typename TAG1, typename... Ls1,
             typename T2, typename TAG2, typename... Ls2>
    friend constexpr
    t_logical<T1, TAG1, Ls1...>& set_(t_logical<T1, TAG1, Ls1...>&,
                                      t_logical<T2, TAG2, Ls2...>) noexcept;

    t_value value_;
  };

  /////////////////////////////////////////////////////////////////////////////

  template<typename TAG, typename... Ls>
  class t_logical<t_void, TAG, Ls...> {
  public:
    static_assert(t_logical_test_<Ls...>::VALUE,
                  "Ls... must be logical<...> types");

    using t_value = t_void;
    using t_tag   = TAG;
    using t_ops   = t_get_ops_<TAG>;
    using t_tags  = t_get_tags_<TAG, Ls...>;

    constexpr t_logical() noexcept = default;

    template<typename TAG1, typename... L1s>
    constexpr
    t_logical(t_logical<t_void, TAG1, L1s...>) noexcept {
      static_assert(t_is_subset_of_<t_tags,
                      t_tags_of<t_logical<t_void, TAG1, L1s...>>>::VALUE,
                    "cannot copy type. types don't give permission.");
    }

    template<typename TAG1, typename... L1s>
    constexpr
    t_logical& operator=(t_logical<t_void, TAG1, L1s...>) noexcept {
      static_assert(t_is_subset_of_<t_tags,
                      t_tags_of<t_logical<t_void, TAG1, L1s...>>>::VALUE,
                     "cannot copy type. types don't give permission.");
      return *this;
    }

    t_logical(const t_logical&)            noexcept = default; // for clarity
    t_logical& operator=(const t_logical&) noexcept = default; // for clarity
  };

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename TAG, typename... Ls>
  constexpr
  T get_(t_logical<T, TAG, Ls...> logical) noexcept {
    return logical.value_;
  }

  template<typename T, typename TAG, typename... Ls>
  constexpr
  t_logical<T, TAG, Ls...>& set_(t_logical<T, TAG, Ls...>& logical,
                                 T value) noexcept {
    return (logical = t_logical<T, TAG, Ls...>{value});
  }

  template<typename T,  typename TAG,  typename... Ls,
           typename T1, typename TAG1, typename... Ls1>
  constexpr
  t_logical<T, TAG, Ls...>& set_(t_logical<T,  TAG,  Ls...>& logical,
                                 t_logical<T1, TAG1, Ls1...> value) noexcept {
    static_assert(!t_is_subset_of_pack<
                     t_tags_of<t_logical<T,  TAG,  Ls...>>,
                     t_tags_of<t_logical<T1, TAG1, Ls1...>>>::VALUE,
                  "Do not set, when you can just assign");
    static_assert(t_is_logical_data<T, T1>::VALUE,
                  "cannot copy type. data too large or wrong sign");
    logical.value_
      = t_check_<t_diff_pack<t_tags_of<t_logical<T,  TAG,  Ls...>>,
                             t_tags_of<t_logical<T1, TAG1, Ls1...>>>>
          ::call(get_(value));
    return logical;
  }

  /////////////////////////////////////////////////////////////////////////////
}
}
}
}

#endif
