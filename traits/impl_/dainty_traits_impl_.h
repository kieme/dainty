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

#ifndef _DAINTY_TRAITS_IMPL_H_
#define _DAINTY_TRAITS_IMPL_H_

#include "dainty_types.h"

namespace dainty
{
namespace traits
{
namespace impl_
{
  /////////////////////////////////////////////////////////////////////////////

  using types::t_well_formed;
  using types::t_expr;
  using types::t_dummy;
  using types::t_opt;
  using types::t_bool;
  using types::t_char;
  using types::t_schar;
  using types::t_uchar;
  using types::t_void;
  using types::t_nullptr;
  using types::t_pack;
  using types::t_empty_pack;
  using types::t_short;
  using types::t_ushort;
  using types::t_int;
  using types::t_uint;
  using types::t_long;
  using types::t_ulong;
  using types::t_llong;
  using types::t_ullong;
  using types::t_float;
  using types::t_double;
  using types::t_ldouble;
  using types::t_n_;
  using types::WELL_FORMED;
  using types::t_add_result;
  using types::t_add_value;
  using types::t_add_VALUE;
  using types::t_add_MAX;
  using types::t_add_MIN;
  using types::t_add_SIZEOF;
  using types::t_add_BITS;
  using types::t_constant;
  using types::t_identity_of;
  using types::t_result_of;
  using types::t_value_of;
  using types::MAX_of;

  /////////////////////////////////////////////////////////////////////////////

  struct t_true  : t_constant<t_bool, true,  t_true>  { };
  struct t_false : t_constant<t_bool, false, t_false> { };

  namespace help_ {
    template<t_bool> struct t_is_truth_       : t_add_result<t_false> { };
    template<>       struct t_is_truth_<true> : t_add_result<t_true>  { };
  }

  template<t_bool B> using t_is_truth = t_result_of<help_::t_is_truth_<B>>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<t_bool> struct t_set_result_       : t_add_result<t_false> { };
    template<>       struct t_set_result_<true> : t_add_result<t_true>  { };
  }

  template<t_bool B> using t_set_result = help_::t_set_result_<B>;

  using t_rtrue  = t_set_result<true>;
  using t_rfalse = t_set_result<false>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<template<typename...> class D, typename... As>
    struct t_pack_into_ {
      using t_value = D<As...>;
    };

    template<typename...> struct t_add_to_front_;

    template<typename... Ts, typename... As>
    struct t_add_to_front_<t_pack<Ts...>, As...> {
      using t_value = t_pack<As..., Ts...>;
    };

    template<typename... Ts, typename... As>
    struct t_add_to_front_<t_pack<Ts...>, t_pack<As...>> {
      using t_value = t_pack<As..., Ts...>;
    };

    template<typename...> struct t_add_to_back_;

    template<typename... Ts, typename... As>
    struct t_add_to_back_<t_pack<Ts...>, As...> {
      using t_value = t_pack<Ts..., As...>;
    };

    template<typename... Ts, typename... As>
    struct t_add_to_back_<t_pack<Ts...>, t_pack<As...>> {
      using t_value = t_pack<Ts..., As...>;
    };
  }

  template<template<typename...> class D, typename... As>
  using t_pack_into    = t_value_of<help_::t_pack_into_<D, As...>>;

  template<typename T, typename T1>
  using t_add_to_front = t_value_of<help_::t_add_to_front_<T, T1>>;

  template<typename T, typename T1>
  using t_add_to_back  = t_value_of<help_::t_add_to_back_<T, T1>>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename, typename, typename> struct t_if_then_else_;

    template<typename T, typename U>
    struct t_if_then_else_<t_true,  T, U> : t_add_value<T> { };

    template<typename T, typename U>
    struct t_if_then_else_<t_false, T, U> : t_add_value<U> { };
  }

  template<typename I, typename T, typename U>
  using t_if_then_else = t_value_of<help_::t_if_then_else_<I, T, U>>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename, typename> struct t_if_then_;

    template<typename T> struct t_if_then_<t_false, T> { };
    template<typename T> struct t_if_then_<t_true,  T> : t_add_value<T> { };
  }

  template<typename I, typename T>
  using t_if_then = t_value_of<help_::t_if_then_<I, T>>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename> struct t_not_;

    template<> struct t_not_<t_true>  : t_rfalse { };
    template<> struct t_not_<t_false> : t_rtrue  { };
  }

  template<typename T> using t_not = t_result_of<help_::t_not_<T>>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename O, typename I> using t_opt_if = t_if_then<I, O>;
  template<typename I>             using t_if     = t_opt_if<t_opt, I>;

  template<typename O, typename I> using t_opt_if_not = t_opt_if<O, t_not<I>>;
  template<typename I>             using t_if_not     = t_opt_if_not<t_opt, I>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<template<typename> class Op, typename T, typename U, typename E>
    struct t_is_there_ : t_add_value<U>, t_rfalse { };

    template<template<typename> class Op, typename T, typename U>
    struct t_is_there_<Op, T, U, t_expr<Op<T>>>
      : t_add_value<Op<T>>, t_rtrue {
    };
  }

  template<template<typename> class Op, typename T>
  using t_expr_of
    = t_value_of<help_::t_is_there_<Op, T, t_well_formed, t_well_formed>>;

  template<template<typename> class Op, typename T, typename U>
  using t_expr_of_or
    = t_value_of<help_::t_is_there_<Op, T, U, t_well_formed>>;

  template<template<typename> class Op, typename T>
  using t_is_there
    = t_result_of<help_::t_is_there_<Op, T, t_well_formed, t_well_formed>>;

  template<template<typename> class Op, typename T>
  using t_is_not_there
    = t_not<t_is_there<Op, T>>;

  template<typename O, template<typename> class Op, typename T>
  using t_opt_if_there
    = t_opt_if<O, t_is_there<Op, T>>;

  template<typename O, template<typename> class Op, typename T>
  using t_opt_if_not_there
    = t_opt_if<O, t_is_not_there<Op, T>>;

  template<template<typename> class Op, typename T>
  using t_if_there
    = t_opt_if_there<t_opt, Op, T>;

  template<template<typename> class Op, typename T>
  using t_if_not_there
    = t_opt_if_not_there<t_opt, Op, T>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T> using t_has_result   = t_is_there<t_result_of,   T>;
  template<typename T> using t_has_value    = t_is_there<t_value_of,    T>;
  template<typename T> using t_has_identity = t_is_there<t_identity_of, T>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename, typename...> struct t_and_;

    template<typename... Ts> struct t_and_<t_true,  Ts...> : t_and_<Ts...> { };
    template<typename... Ts> struct t_and_<t_false, Ts...> : t_rfalse { };
    template<>               struct t_and_<t_true>         : t_rtrue  { };
    template<>               struct t_and_<t_false>        : t_rfalse { };
  }

  template<typename T, typename... Ts>
  using t_and = t_result_of<help_::t_and_<T, Ts...>>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename, typename...> struct t_or_;

    template<typename... Ts> struct t_or_<t_true,  Ts...> : t_rtrue { };
    template<typename... Ts> struct t_or_<t_false, Ts...> : t_or_<Ts...>  { };
    template<>               struct t_or_<t_true>         : t_rtrue  { };
    template<>               struct t_or_<t_false>        : t_rfalse { };
  }

  template<typename T, typename... Ts>
  using t_or = t_result_of<help_::t_or_<T, Ts...>>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename T, template<typename, typename...> class C,
                         template<typename, typename...> class... Cs>
    struct t_all_is_true_ : t_add_result<t_and<C<T>, Cs<T>...>> { };

    template<typename T, template<typename, typename...> class C>
    struct t_all_is_true_<T, C> : t_add_result<C<T>> { };
  }

  template<typename T, template<typename, typename...> class C,
                       template<typename, typename...> class... Cs>
  using t_all_is_true     = t_result_of<help_::t_all_is_true_<T, C, Cs...>>;

  template<typename T, template<typename, typename...> class C,
                       template<typename, typename...> class... Cs>
  using t_all_is_not_true = t_not<t_all_is_true<T, C, Cs...>>;

  template<typename O, typename T, template<typename, typename...> class C,
                                   template<typename, typename...> class... Cs>
  using t_opt_if_all_true     = t_opt_if<O, t_all_is_true<T, C, Cs...>>;

  template<typename O, typename T, template<typename, typename...> class C,
                                   template<typename, typename...> class... Cs>
  using t_opt_if_all_not_true = t_opt_if<O, t_all_is_not_true<T, C, Cs...>>;

  template<typename T, template<typename, typename...> class C,
                       template<typename, typename...> class... Cs>
  using t_if_all_true         = t_opt_if_all_true<t_opt, T, C, Cs...>;

  template<typename T, template<typename, typename...> class C,
                       template<typename, typename...> class... Cs>
  using t_if_all_not_true     = t_opt_if_all_not_true<t_opt, T, C, Cs...>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename T, template<typename, typename...> class C,
                         template<typename, typename...> class... Cs>
    struct t_none_is_true_ : t_add_result<t_and<t_not<C<T>>,
                                                t_not<Cs<T>>...>> { };

    template<typename T, template<typename, typename...> class C>
    struct t_none_is_true_<T, C> : t_add_result<t_not<C<T>>> { };
  }

  template<typename T, template<typename, typename...> class C,
                       template<typename, typename...> class... Cs>
  using t_none_is_true  = t_result_of<help_::t_none_is_true_<T, C, Cs...>>;

  template<typename O, typename T, template<typename, typename...> class C,
                                   template<typename, typename...> class... Cs>
  using t_opt_if_none_true  = t_opt_if<O, t_none_is_true<T, C, Cs...>>;

  template<typename T, template<typename, typename...> class C,
                       template<typename, typename...> class... Cs>
  using t_if_none_true  = t_opt_if_none_true<t_opt, T, C, Cs...>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename T, template<typename, typename...> class C,
                         template<typename, typename...> class... Cs>
    struct t_least_one_is_true_ : t_add_result<t_or<C<T>, Cs<T>...>> { };

    template<typename T, template<typename, typename...> class C>
    struct t_least_one_is_true_<T, C> : t_add_result<C<T>> { };
  }

  template<typename T, template<typename, typename...> class C,
                       template<typename, typename...> class... Cs>
  using t_least_one_is_true =
    t_result_of<help_::t_least_one_is_true_<T, C, Cs...>>;

  template<typename O, typename T, template<typename, typename...> class C,
                                   template<typename, typename...> class... Cs>
  using t_opt_if_least_one_true = t_opt_if<O, t_least_one_is_true<T, C, Cs...>>;

  template<typename T, template<typename, typename...> class C,
                       template<typename, typename...> class... Cs>
  using t_if_least_one_true = t_opt_if_least_one_true<t_opt, T, C, Cs...>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<template<typename, typename...> class C,
             typename T, typename... Ts>
    struct t_each_is_true_ : t_add_result<t_and<C<T>, C<Ts>...>> { };

    template<template<typename, typename...> class C,
             typename T>
    struct t_each_is_true_<C, T> : t_add_result<C<T>> { };
  }

  template<template<typename, typename...> class C, typename T, typename...Ts>
  using t_each_is_true = t_result_of<help_::t_each_is_true_<C, T, Ts...>>;

  template<typename O, template<typename, typename...> class C,
                       typename T, typename... Ts>
  using t_opt_if_each_true = t_opt_if<O, t_each_is_true<C, T, Ts...>>;

  template<template<typename, typename...> class C, typename T, typename... Ts>
  using t_if_each_true     = t_opt_if_each_true<t_opt, C, T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<template<typename, typename...> class C,
             typename T, typename...Ts>
    struct t_each_is_false_ : t_add_result<t_and<t_not<C<T>>,
                                                 t_not<C<Ts>>...>> { };

    template<template<typename, typename...> class C, typename T>
    struct t_each_is_false_<C, T> : t_add_result<t_not<C<T>>> { };
  }

  template<template<typename, typename...> class C, typename T, typename...Ts>
  using t_each_is_false = t_result_of<help_::t_each_is_false_<C, T, Ts...>>;

  template<typename O, template<typename, typename...> class C,
                       typename T, typename...Ts>
  using t_opt_if_each_false = t_opt_if<O, t_each_is_false<C, T, Ts...>>;

  template<template<typename, typename...> class C, typename T, typename...Ts>
  using t_if_each_false     = t_opt_if_each_false<t_opt, C, T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename> struct t_is_true_help_ : t_rfalse { };
    template<> struct t_is_true_help_<t_true> : t_rtrue  { };

    template<typename T>
    using t_is_true_ = t_result_of<t_is_true_help_<T>>;
  }

  template<typename T, typename... Ts>
  using t_is_true     = t_each_is_true<help_::t_is_true_, T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_true = t_opt_if<O, t_is_true<T, Ts...>>;

  template<typename T, typename... Ts>
  using t_if_true     = t_opt_if_true<t_opt, T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename> struct t_is_false_help_ : t_rfalse { };
    template<> struct t_is_false_help_<t_true> : t_rtrue  { };

    template<typename T>
    using t_is_false_ = t_result_of<t_is_false_help_<T>>;
  }

  template<typename T, typename... Ts>
  using t_is_false     = t_each_is_true<help_::t_is_false_, T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_false = t_opt_if<O, t_is_false<T>>;

  template<typename T, typename... Ts>
  using t_if_false     = t_opt_if_false<t_opt, T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename T, typename T1, typename... Ts>
    struct t_is_same_ : t_add_result<t_and<t_is_same_<T, T1>,
                                           t_is_same_<T, Ts>...>> { };

    template<typename T, typename T1> struct t_is_same_<T, T1> : t_rfalse { };
    template<typename T>              struct t_is_same_<T, T>  : t_rtrue  { };
  }

  template<typename T, typename T1, typename... Ts>
  using t_is_same         = t_result_of<help_::t_is_same_<T, T1, Ts...>>;

  template<typename T, typename T1, typename... Ts>
  using t_is_not_same     = t_not<t_is_same<T, T1, Ts...>>;

  template<typename O, typename T, typename T1, typename... Ts>
  using t_opt_if_same     = t_opt_if<O, t_is_same<T, T1, Ts...>>;

  template<typename O, typename T, typename T1, typename... Ts>
  using t_opt_if_not_same = t_opt_if<O, t_is_not_same<T, T1, Ts...>>;

  template<typename T, typename T1, typename... Ts>
  using t_if_same         = t_opt_if_same<t_opt, T, T1, Ts...>;

  template<typename T, typename T1, typename... Ts>
  using t_if_not_same     = t_opt_if_not_same<t_opt, T, T1, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename, typename...> struct t_is_one_of_;
    template<typename, typename...> struct t_is_one_of_help_;

    template<typename... Ts>
    struct t_is_one_of_help_<t_true, Ts...> : t_rtrue { };

    template<typename T, typename... Ts>
    struct t_is_one_of_help_<t_false, T, Ts...> : t_is_one_of_<T, Ts...> { };

    template<typename T, typename T1, typename... Ts>
    struct t_is_one_of_<T, T1, Ts...>
      : t_is_one_of_help_<t_is_same<T, T1>, T, Ts...> { };

    template<typename T, typename T1>
    struct t_is_one_of_<T, T1> : t_add_result<t_is_same<T, T1>> { };

    template<typename T>
    struct t_is_one_of_<T> : t_rfalse { };
  }

  template<typename T, typename... Ts>
  using t_is_one_of         = t_result_of<help_::t_is_one_of_<T, Ts...>>;

  template<typename T, typename... Ts>
  using t_is_not_one_of     = t_not<t_is_one_of<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_one_of     = t_opt_if<O, t_is_one_of<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_one_of = t_opt_if<O, t_is_not_one_of<T, Ts...>>;

  template<typename T, typename... Ts>
  using t_if_one_of         = t_opt_if_one_of<t_opt, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_one_of     = t_opt_if_not_one_of<t_opt, T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    enum t_lvalue_  { LVALUE  = 0 };
    enum t_xvalue_  { XVALUE  = 1 };
    enum t_prvalue_ { PRVALUE = 2 };

    template<class T> struct t_category_      : t_add_value<t_prvalue_> { };
    template<class T> struct t_category_<T&>  : t_add_value<t_lvalue_>  { };
    template<class T> struct t_category_<T&&> : t_add_value<t_xvalue_>  { };
  }

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename T>
    using t_is_lvalue_
      = t_is_same<t_value_of<help_::t_category_<T>>, help_:: t_lvalue_>;
  }

  template<typename T, typename... Ts>
  using t_is_lvalue         = t_each_is_true<help_::t_is_lvalue_, T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_lvalue     = t_not<t_is_lvalue<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_lvalue     = t_opt_if<O, t_is_lvalue<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_lvalue = t_opt_if<O, t_is_not_lvalue<T, Ts...>>;

  template<typename T, typename... Ts>
  using t_if_lvalue         = t_opt_if_lvalue<t_opt, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_lvalue     = t_opt_if_not_lvalue<t_opt, T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename T>
    using t_is_xvalue_
      = t_is_same<t_value_of<help_::t_category_<T>>, help_:: t_xvalue_>;
  }

  template<typename T, typename... Ts>
  using t_is_xvalue          = t_each_is_true<help_::t_is_xvalue_, T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_xvalue      = t_not<t_is_xvalue<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_xvalue      = t_opt_if<O, t_is_xvalue<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_xvalue  = t_opt_if<O, t_is_not_xvalue<T, Ts...>>;

  template<typename T, typename... Ts>
  using t_if_xvalue          = t_opt_if_xvalue<t_opt, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_xvalue      = t_opt_if_not_xvalue<t_opt, T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename T>
    using t_is_prvalue_
      = t_is_same<t_value_of<help_::t_category_<T>>, help_:: t_prvalue_>;
  }

  template<typename T, typename... Ts>
  using t_is_prvalue          = t_each_is_true<help_::t_is_prvalue_, T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_prvalue      = t_not<t_is_prvalue<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_prvalue      = t_opt_if<O, t_is_prvalue<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_prvalue  = t_opt_if<O, t_is_not_prvalue<T, Ts...>>;

  template<typename T, typename... Ts>
  using t_if_prvalue          = t_opt_if_prvalue<t_opt, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_prvalue      = t_opt_if_not_prvalue<t_opt, T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename T>
    using t_is_result_ = t_least_one_is_true<T, t_is_true, t_is_false>;
  }

  template<typename T, typename... Ts>
  using t_is_result         = t_each_is_true<help_::t_is_result_, T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_result     = t_not<t_is_result<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_result     = t_opt_if<O, t_is_result<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_result = t_opt_if<O, t_is_not_result<T, Ts...>>;

  template<typename T, typename... Ts>
  using t_if_result         = t_opt_if_result<t_opt, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_result     = t_opt_if_not_result<t_opt, T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename> struct t_is_bool_help_         : t_rfalse { };
    template<>         struct t_is_bool_help_<t_bool> : t_rtrue  { };

    template<typename T>
    using t_is_bool_ = t_result_of<t_is_bool_help_<T>>;
  }

  template<typename T, typename... Ts>
  using t_is_bool         = t_each_is_true<help_::t_is_bool_, T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_bool     = t_not<t_is_bool<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_bool     = t_opt_if<O, t_is_bool<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_bool = t_opt_if<O, t_is_not_bool<T, Ts...>>;

  template<typename T, typename... Ts>
  using t_if_bool         = t_opt_if_bool<t_opt, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_bool     = t_opt_if_not_bool<t_opt, T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename> struct t_is_void_help_         : t_rfalse { };
    template<>         struct t_is_void_help_<t_void> : t_rtrue  { };

    template<typename T> using t_is_void_ = t_result_of<t_is_void_help_<T>>;
  }

  template<typename T, typename... Ts>
  using t_is_void         = t_each_is_true<help_::t_is_void_, T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_void     = t_not<t_is_void<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_void     = t_opt_if<O, t_is_void<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_void = t_opt_if<O, t_is_not_void<T, Ts...>>;

  template<typename T, typename... Ts>
  using t_if_void         = t_opt_if_void<t_opt, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_void     = t_opt_if_not_void<t_opt, T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename> struct t_is_nullptr_help_            : t_rfalse { };
    template<>         struct t_is_nullptr_help_<t_nullptr> : t_rtrue  { };

    template<typename T>
    using t_is_nullptr_ = t_result_of<t_is_nullptr_help_<T>>;
  }

  template<typename T, typename... Ts>
  using t_is_nullptr         = t_each_is_true<help_::t_is_nullptr_, T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_nullptr     = t_not<t_is_nullptr<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_nullptr     = t_opt_if<O, t_is_nullptr<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_nullptr = t_opt_if<O, t_is_not_nullptr<T, Ts...>>;

  template<typename T, typename... Ts>
  using t_if_nullptr         = t_opt_if_nullptr<t_opt, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_nullptr     = t_opt_if_not_nullptr<t_opt, T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename T> using t_is_enum_ = t_is_truth<__is_enum(T)>;
  }

  template<typename T, typename... Ts>
  using t_is_enum         = t_each_is_true<help_::t_is_enum_, T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_enum     = t_not<t_is_enum<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_enum     = t_opt_if<O, t_is_enum<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_enum = t_opt_if<O, t_is_not_enum<T, Ts...>>;

  template<typename T, typename... Ts>
  using t_if_enum         = t_opt_if_enum<t_opt, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_enum     = t_opt_if_not_enum<t_opt, T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename B, typename D>
    using t_is_base_of_ = t_is_truth<__is_base_of(B, D)>;
  }

  template<typename B, typename D, typename... Ds>
  using t_is_base_of = t_and<help_::t_is_base_of_<B, D>,
                             help_::t_is_base_of_<B, Ds>...>;

  template<typename B, typename D, typename... Ds>
  using t_is_not_base_of     = t_not<t_is_base_of<B, D, Ds...>>;

  template<typename O, typename B, typename D, typename... Ds>
  using t_opt_if_base_of     = t_opt_if<O, t_is_base_of<B, D, Ds...>>;

  template<typename O, typename B, typename D, typename... Ds>
  using t_opt_if_not_base_of = t_opt_if<O, t_is_not_base_of<B, D, Ds...>>;

  template<typename B, typename D, typename... Ds>
  using t_if_base_of         = t_opt_if_base_of<t_opt, B, D, Ds...>;

  template<typename B, typename D, typename... Ds>
  using t_if_not_base_of     = t_opt_if_not_base_of<t_opt, B, D, Ds...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename D, typename B, typename... Bs>
  using t_is_derived_from = t_and<help_::t_is_base_of_<B,  D>,
                                  help_::t_is_base_of_<Bs, D>...>;

  template<typename D, typename B, typename... Bs>
  using t_is_not_derived_from     = t_not<t_is_derived_from<D, B, Bs...>>;

  template<typename O, typename D, typename B, typename... Bs>
  using t_opt_if_derived_from     = t_opt_if<O, t_is_derived_from<D, B, Bs...>>;

  template<typename O, typename D, typename B, typename... Bs>
  using t_opt_if_not_derived_from
    = t_opt_if<O, t_is_not_derived_from<D, B, Bs...>>;

  template<typename D, typename B, typename... Bs>
  using t_if_derived_from         = t_opt_if_derived_from<t_opt, D, B, Bs...>;

  template<typename D, typename B, typename... Bs>
  using t_if_not_derived_from     = t_opt_if_not_derived_from<t_opt, D, B, Bs...>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename T> using t_is_union_ = t_is_truth<__is_union(T)>;
  }

  template<typename T, typename... Ts>
  using t_is_union         = t_each_is_true<help_::t_is_union_, T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_union     = t_not<t_is_union<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_union     = t_opt_if<O, t_is_union<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_union = t_opt_if<O, t_is_not_union<T, Ts...>>;

  template<typename T, typename... Ts>
  using t_if_union         = t_opt_if_union<t_opt, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_union     = t_opt_if_not_union<t_opt, T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename T> using t_is_class_ = t_is_truth<__is_class(T)>;
  }

  template<typename T, typename... Ts>
  using t_is_class         = t_each_is_true<help_::t_is_class_, T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_class     = t_not<t_is_class<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_class     = t_opt_if<O, t_is_class<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_class = t_opt_if<O, t_is_not_class<T, Ts...>>;

  template<typename T, typename... Ts>
  using t_if_class         = t_opt_if_class<t_opt, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_class     = t_opt_if_not_class<t_opt, T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename T> using t_is_trivial_ = t_is_truth<__is_trivial(T)>;
  }

  template<typename T, typename... Ts>
  using t_is_trivial         = t_each_is_true<help_::t_is_trivial_, T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_trivial     = t_not<t_is_trivial<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_trivial     = t_opt_if<O, t_is_trivial<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_trivial = t_opt_if<O, t_is_not_trivial<T, Ts...>>;

  template<typename T, typename... Ts>
  using t_if_trivial         = t_opt_if_trivial<t_opt, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_trivial     = t_opt_if_not_trivial<t_opt, T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename T> using t_is_pod_ = t_is_truth<__is_pod(T)>;
  }

  template<typename T, typename... Ts>
  using t_is_pod         = t_each_is_true<help_::t_is_pod_, T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_pod     = t_not<t_is_pod<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_pod     = t_opt_if<O, t_is_pod<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_pod = t_opt_if<O, t_is_not_pod<T, Ts...>>;

  template<typename T, typename... Ts>
  using t_if_pod         = t_opt_if_pod<t_opt, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_pod     = t_opt_if_not_pod<t_opt, T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename T>
    using t_is_literal_ = t_is_truth<__is_literal_type(T)>;
  }

  template<typename T, typename... Ts>
  using t_is_literal         = t_each_is_true<help_::t_is_literal_, T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_literal     = t_not<t_is_literal<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_literal     = t_opt_if<O, t_is_literal<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_literal = t_opt_if<O, t_is_not_literal<T, Ts...>>;

  template<typename T, typename... Ts>
  using t_if_literal         = t_opt_if_literal<t_opt, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_literal     = t_opt_if_not_literal<t_opt, T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename T> using t_is_empty_ = t_is_truth<__is_empty(T)>;
  }

  template<typename T, typename... Ts>
  using t_is_empty         = t_each_is_true<help_::t_is_empty_, T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_empty     = t_not<t_is_empty<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_empty     = t_opt_if<O, t_is_empty<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_empty = t_opt_if<O, t_is_not_empty<T, Ts...>>;

  template<typename T, typename... Ts>
  using t_if_empty         = t_opt_if_empty<t_opt, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_empty     = t_opt_if_not_empty<t_opt, T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename T>
    using t_is_polymorphic_ = t_is_truth<__is_polymorphic(T)>;
  }

  template<typename T, typename... Ts>
  using t_is_polymorphic = t_each_is_true<help_::t_is_polymorphic_, T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_polymorphic     = t_not<t_is_polymorphic<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_polymorphic     = t_opt_if<O, t_is_polymorphic<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_polymorphic = t_opt_if<O, t_is_not_polymorphic<T, Ts...>>;

  template<typename T, typename... Ts>
  using t_if_polymorphic         = t_opt_if_polymorphic<t_opt, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_polymorphic     = t_opt_if_not_polymorphic<t_opt, T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename T> using t_is_final_ = t_is_truth<__is_final(T)>;
  }

  template<typename T, typename... Ts>
  using t_is_final         = t_each_is_true<help_::t_is_final_, T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_final     = t_not<t_is_final<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_final     = t_opt_if<O, t_is_final<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_final = t_opt_if<O, t_is_not_final<T, Ts...>>;

  template<typename T, typename... Ts>
  using t_if_final         = t_opt_if_final<t_opt, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_final     = t_opt_if_not_final<t_opt, T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename T> using t_is_abstract_ = t_is_truth<__is_abstract(T)>;
  }

  template<typename T, typename... Ts>
  using t_is_abstract     = t_each_is_true<help_::t_is_abstract_, T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_abstract     = t_not<t_is_abstract<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_abstract     = t_opt_if<O, t_is_abstract<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_abstract = t_opt_if<O, t_is_not_abstract<T, Ts...>>;

  template<typename T, typename... Ts>
  using t_if_abstract         = t_opt_if_abstract<t_opt, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_abstract     = t_opt_if_not_abstract<t_opt, T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename T>
    using t_is_trivially_copyable_ = t_is_truth<__is_trivially_copyable(T)>;
  }

  template<typename T, typename... Ts>
  using t_is_trivially_copyable
    = t_each_is_true<help_::t_is_trivially_copyable_, T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_trivially_copyable
    = t_not<t_is_trivially_copyable<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_trivially_copyable
    = t_opt_if<O, t_is_trivially_copyable<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_trivially_copyable
    = t_opt_if<O, t_is_not_trivially_copyable<T, Ts...>>;

  template<typename T, typename... Ts>
  using t_if_trivially_copyable
    = t_opt_if_trivially_copyable<t_opt, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_trivially_copyable
    = t_opt_if_not_trivially_copyable<t_opt, T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename T, typename... Ts>
    using t_is_standard_layout_ = t_is_truth<__is_standard_layout(T)>;
  }

  template<typename T, typename... Ts>
  using t_is_standard_layout
    = t_each_is_true<help_::t_is_standard_layout_, T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_standard_layout
    = t_not<t_is_standard_layout<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_standard_layout
    = t_opt_if<O, t_is_standard_layout<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_standard_layout
    = t_opt_if<O, t_is_not_standard_layout<T, Ts...>>;

  template<typename T, typename... Ts>
  using t_if_standard_layout
    = t_opt_if_standard_layout<t_opt, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_standard_layout
    = t_opt_if_not_standard_layout<t_opt, T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename>
    struct t_is_bounded_array_help_ : t_rfalse { };

    template<typename T, t_n_ N>
    struct t_is_bounded_array_help_<T[N]> : t_rtrue { };

    template<typename T>
    using t_is_bounded_array_ = t_result_of<t_is_bounded_array_help_<T>>;
  }

  template<typename T, typename... Ts>
  using t_is_bounded_array
    = t_each_is_true<help_::t_is_bounded_array_, T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_bounded_array
    = t_not<t_is_bounded_array<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_bounded_array
    = t_opt_if<O, t_is_bounded_array<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_bounded_array
    = t_opt_if<O, t_is_not_bounded_array<T, Ts...>>;

  template<typename T, typename... Ts>
  using t_if_bounded_array
    = t_opt_if_bounded_array<t_opt, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_bounded_array
    = t_opt_if_not_bounded_array<t_opt, T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename>   struct t_is_unbounded_array_help_      : t_rfalse { };
    template<typename T> struct t_is_unbounded_array_help_<T[]> : t_rtrue  { };

    template<typename T>
    using t_is_unbounded_array = t_result_of<t_is_unbounded_array_help_<T>>;
  }

  template<typename T, typename... Ts>
  using t_is_unbounded_array
    = t_each_is_true<help_::t_is_unbounded_array, T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_unbounded_array
    = t_not<t_is_unbounded_array<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_unbounded_array
    = t_opt_if<O, t_is_unbounded_array<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_unbounded_array
    = t_opt_if<O, t_is_not_unbounded_array<T, Ts...>>;

  template<typename T, typename... Ts>
  using t_if_unbounded_array
    = t_opt_if_unbounded_array<t_opt, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_unbounded_array
    = t_opt_if_not_unbounded_array<t_opt, T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename T>
    using t_is_array_ = t_least_one_is_true<T, t_is_bounded_array,
                                               t_is_unbounded_array>;
  }

  template<typename T, typename... Ts>
  using t_is_array         = t_each_is_true<help_::t_is_array_, T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_array     = t_not<t_is_array<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_array     = t_opt_if<O, t_is_array<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_array = t_opt_if<O, t_is_not_array<T, Ts...>>;

  template<typename T, typename... Ts>
  using t_if_array         = t_opt_if_array<t_opt, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_array     = t_opt_if_not_array<t_opt, T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename>   struct t_is_ptr_help_            : t_rfalse { };
    template<typename T> struct t_is_ptr_help_<T*>        : t_rtrue  { };
    template<>           struct t_is_ptr_help_<t_nullptr> : t_rtrue  { };

    template<typename T> using t_is_ptr_ = t_result_of<t_is_ptr_help_<T>>;
  }

  template<typename T, typename... Ts>
  using t_is_ptr         = t_each_is_true<help_::t_is_ptr_, T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_ptr     = t_not<t_is_ptr<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_ptr     = t_opt_if<O, t_is_ptr<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_ptr = t_opt_if<O, t_is_not_ptr<T, Ts...>>;

  template<typename T, typename... Ts>
  using t_if_ptr         = t_opt_if_ptr<t_opt, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_ptr     = t_opt_if_not_ptr<t_opt, T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename>   struct t_is_ref_help_      : t_rfalse { };
    template<typename T> struct t_is_ref_help_<T&>  : t_rtrue  { };
    template<typename T> struct t_is_ref_help_<T&&> : t_rtrue  { };

    template<typename T> using t_is_ref_ = t_result_of<t_is_ref_help_<T>>;
  }

  template<typename T, typename... Ts>
  using t_is_ref         = t_each_is_true<help_::t_is_ref_, T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_ref     = t_not<t_is_ref<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_ref     = t_opt_if<O, t_is_ref<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_ref = t_opt_if<O, t_is_not_ref<T, Ts...>>;

  template<typename T, typename... Ts>
  using t_if_ref         = t_opt_if_ref<t_opt, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_ref     = t_opt_if_not_ref<t_opt, T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename>   struct t_is_lvalue_ref_help_      : t_rfalse { };
    template<typename T> struct t_is_lvalue_ref_help_<T&&> : t_rfalse { };
    template<typename T> struct t_is_lvalue_ref_help_<T&>  : t_rtrue  { };

    template<typename T>
    using t_is_lvalue_ref_ = t_result_of<t_is_lvalue_ref_help_<T>>;
  }

  template<typename T, typename... Ts>
  using t_is_lvalue_ref = t_each_is_true<help_::t_is_lvalue_ref_, T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_lvalue_ref     = t_not<t_is_lvalue_ref<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_lvalue_ref     = t_opt_if<O, t_is_lvalue_ref<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_lvalue_ref = t_opt_if<O, t_is_not_lvalue_ref<T, Ts...>>;

  template<typename T, typename... Ts>
  using t_if_lvalue_ref         = t_opt_if_lvalue_ref<t_opt, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_lvalue_ref     = t_opt_if_not_lvalue_ref<t_opt, T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename>   struct t_is_rvalue_ref_help_      : t_rfalse { };
    template<typename T> struct t_is_rvalue_ref_help_<T&&> : t_rtrue  { };

    template<typename T, typename... Ts>
    using t_is_rvalue_ref_ = t_result_of<t_is_rvalue_ref_help_<T>>;
  }

  template<typename T, typename... Ts>
  using t_is_rvalue_ref = t_each_is_true<help_::t_is_rvalue_ref_, T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_rvalue_ref      = t_not<t_is_rvalue_ref<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_rvalue_ref      = t_opt_if<O, t_is_rvalue_ref<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
   using t_opt_if_not_rvalue_ref = t_opt_if<O, t_is_not_rvalue_ref<T, Ts...>>;

  template<typename T, typename... Ts>
  using t_if_rvalue_ref          = t_opt_if_rvalue_ref<t_opt, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_rvalue_ref      = t_opt_if_not_rvalue_ref<t_opt, T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename T>
    struct t_is_free_func_help_ : t_rfalse { };

    template<typename R, typename... As>
    struct t_is_free_func_help_<R(As...)>      : t_rtrue { };

    template<typename R, typename... As>
    struct t_is_free_func_help_<R(As..., ...)> : t_rtrue { };

#if __cplusplus >= 201703L
    template<typename R, typename... As>
    struct t_is_free_func_help_<R(As...)      noexcept> : t_rtrue { };

    template<typename R, typename... As>
    struct t_is_free_func_help_<R(As..., ...) noexcept> : t_rtrue { };
#endif

    template<typename T>
    using t_is_free_func_ = t_result_of<help_::t_is_free_func_help_<T>>;
  }

  template<typename T, typename... Ts>
  using t_is_free_func = t_each_is_true<help_::t_is_free_func_, T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_free_func     = t_not<t_is_free_func<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_free_func     = t_opt_if<O, t_is_free_func<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_free_func = t_opt_if<O, t_is_not_free_func<T, Ts...>>;

  template<typename T, typename... Ts>
  using t_if_free_func         = t_opt_if_free_func<t_opt, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_free_func     = t_opt_if_not_free_func<t_opt, T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename T>
    struct t_member_func_;

    template<typename T, typename U>
    struct t_member_func_<T U::*> : t_add_value<T> { };
  }

  template<typename T>
  using t_member_func = t_value_of<help_::t_member_func_<T>>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<t_bool M,
             t_bool N,
             t_bool C,
             t_bool V,
             t_bool A,
             t_bool L,
             t_bool R,
             typename    Ret,
             typename... Args>
    struct t_add_func_traits_ {
      using t_return        = Ret;
      using t_args          = t_pack<Args...>;
      using t_is_member     = t_is_truth<M>;
      using t_is_noexcept   = t_is_truth<N>;
      using t_is_const      = t_is_truth<C>;
      using t_is_volatile   = t_is_truth<V>;
      using t_is_va_args    = t_is_truth<A>;
      using t_is_lvalue_ref = t_is_truth<L>;
      using t_is_rvalue_ref = t_is_truth<R>;
    };

    ///////////////////////////////////////////////////////////////////////////

    template<typename T> struct t_func_;

    template<typename T, typename U>
    struct t_func_<T U::*> : t_add_value<T> {
      using t_return        = typename t_func_<T>::t_return;
      using t_args          = typename t_func_<T>::t_args;
      using t_is_member     = t_rtrue;
      using t_is_noexcept   = typename t_func_<T>::t_is_noexcept;
      using t_is_const      = typename t_func_<T>::t_is_const;
      using t_is_volatile   = typename t_func_<T>::t_is_volatile;
      using t_is_va_args    = typename t_func_<T>::t_is_va_args;
      using t_is_lvalue_ref = typename t_func_<T>::t_is_lvalue_ref;
      using t_is_rvalue_ref = typename t_func_<T>::t_is_rvalue_ref;
    };

    template<typename R, typename... As>
    struct t_func_<R(As...)>
      : t_add_value<R(As...)>,
        t_add_func_traits_<false, // IS_MEMBER,
                           false, // IS_NOEXCEPT
                           false, // IS_CONST
                           false, // IS_VOLATILE
                           false, // IS_VA_ARGS
                           false, // IS_LVALUE_REF
                           false, // IS_RVALUE_REF
                           R, As...> { };

    template<typename R, typename... As>
    struct t_func_<R(As..., ...)>
      : t_add_value<R(As..., ...)>,
        t_add_func_traits_<false, // IS_MEMBER,
                           false, // IS_NOEXCEPT
                           false, // IS_CONST
                           false, // IS_VOLATILE
                           true,  // IS_VA_ARGS
                           false, // IS_LVALUE_REF
                           false, // IS_RVALUE_REF
                           R, As...> { };

#if __cplusplus >= 201703L
    template<typename R, typename... As>
    struct t_func_<R(As...) noexcept>
      : t_add_value<R(As...) noexcept>,
        t_add_func_traits_<false, // IS_MEMBER,
                           true,  // IS_NOEXCEPT
                           false, // IS_CONST
                           false, // IS_VOLATILE
                           false, // IS_VA_ARGS
                           false, // IS_LVALUE_REF
                           false, // IS_RVALUE_REF
                           R, As...> { };

    template<typename R, typename... As>
    struct t_func_<R(As..., ...) noexcept>
      : t_add_value<R(As..., ...) noexcept>,
        t_add_func_traits_<false, // IS_MEMBER,
                           true,  // IS_NOEXCEPT
                           false, // IS_CONST
                           false, // IS_VOLATILE
                           true,  // IS_VA_ARGS
                           false, // IS_LVALUE_REF
                           false, // IS_RVALUE_REF
                           R, As...> { };
#endif

    template<typename R, typename... As>
    struct t_func_<R(As...) &>
      : t_add_value<R(As...) &>,
        t_add_func_traits_<true,  // IS_MEMBER,
                           false, // IS_NOEXCEPT
                           false, // IS_CONST
                           false, // IS_VOLATILE
                           false, // IS_VA_ARGS
                           true,  // IS_LVALUE_REF
                           false, // IS_RVALUE_REF
                           R, As...> { };

    template<typename R, typename... As>
    struct t_func_<R(As..., ...) &>
      : t_add_value<R(As..., ...) &>,
        t_add_func_traits_<true,  // IS_MEMBER,
                           false, // IS_NOEXCEPT
                           false, // IS_CONST
                           false, // IS_VOLATILE
                           true,  // IS_VA_ARGS
                           true,  // IS_LVALUE_REF
                           false, // IS_RVALUE_REF
                           R, As...> { };

#if __cplusplus >= 201703L
    template<typename R, typename... As>
    struct t_func_<R(As...) & noexcept>
      : t_add_value<R(As...) & noexcept>,
        t_add_func_traits_<true,  // IS_MEMBER,
                           true,  // IS_NOEXCEPT
                           false, // IS_CONST
                           false, // IS_VOLATILE
                           false, // IS_VA_ARGS
                           true,  // IS_LVALUE_REF
                           false, // IS_RVALUE_REF
                           R, As...> { };

    template<typename R, typename... As>
    struct t_func_<R(As..., ...) & noexcept>
      : t_add_value<R(As..., ...) & noexcept>,
        t_add_func_traits_<true,  // IS_MEMBER,
                           true,  // IS_NOEXCEPT
                           false, // IS_CONST
                           false, // IS_VOLATILE
                           true,  // IS_VA_ARGS
                           true,  // IS_LVALUE_REF
                           false, // IS_RVALUE_REF
                           R, As...> { };
#endif

    template<typename R, typename... As>
    struct t_func_<R(As...) &&>
      : t_add_value<R(As...) &&>,
        t_add_func_traits_<true,  // IS_MEMBER,
                           false, // IS_NOEXCEPT
                           false, // IS_CONST
                           false, // IS_VOLATILE
                           false, // IS_VA_ARGS
                           false, // IS_LVALUE_REF
                           true,  // IS_RVALUE_REF
                           R, As...> { };

    template<typename R, typename... As>
    struct t_func_<R(As..., ...) &&>
      : t_add_value<R(As..., ...) &&>,
        t_add_func_traits_<true,  // IS_MEMBER,
                           false, // IS_NOEXCEPT
                           false, // IS_CONST
                           false, // IS_VOLATILE
                           true,  // IS_VA_ARGS
                           false, // IS_LVALUE_REF
                           true,  // IS_RVALUE_REF
                           R, As...> { };

#if __cplusplus >= 201703L
    template<typename R, typename... As>
    struct t_func_<R(As...) && noexcept>
      : t_add_value<R(As...) && noexcept>,
        t_add_func_traits_<true,  // IS_MEMBER,
                           true,  // IS_NOEXCEPT
                           false, // IS_CONST
                           false, // IS_VOLATILE
                           false, // IS_VA_ARGS
                           false, // IS_LVALUE_REF
                           true,  // IS_RVALUE_REF
                           R, As...> { };

    template<typename R, typename... As>
    struct t_func_<R(As..., ...) && noexcept>
      : t_add_value<R(As..., ...) && noexcept>,
        t_add_func_traits_<true,  // IS_MEMBER,
                           true,  // IS_NOEXCEPT
                           false, // IS_CONST
                           false, // IS_VOLATILE
                           true,  // IS_VA_ARGS
                           false, // IS_LVALUE_REF
                           true,  // IS_RVALUE_REF
                           R, As...> { };
#endif

    ///////////////////////////////////////////////////////////////////////////

    template<typename R, typename... As>
    struct t_func_<R(As...) const>
      : t_add_value<R(As...) const>,
        t_add_func_traits_<true,  // IS_MEMBER,
                           false, // IS_NOEXCEPT
                           true,  // IS_CONST
                           false, // IS_VOLATILE
                           false, // IS_VA_ARGS
                           false, // IS_LVALUE_REF
                           false, // IS_RVALUE_REF
                           R, As...> { };

    template<typename R, typename... As>
    struct t_func_<R(As..., ...) const>
      : t_add_value<R(As..., ...) const>,
        t_add_func_traits_<true,  // IS_MEMBER,
                           false, // IS_NOEXCEPT
                           true,  // IS_CONST
                           false, // IS_VOLATILE
                           true,  // IS_VA_ARGS
                           false, // IS_LVALUE_REF
                           false, // IS_RVALUE_REF
                           R, As...> { };

#if __cplusplus >= 201703L
    template<typename R, typename... As>
    struct t_func_<R(As...) const noexcept>
      : t_add_value<R(As...) const noexcept>,
        t_add_func_traits_<true,  // IS_MEMBER,
                           true,  // IS_NOEXCEPT
                           true,  // IS_CONST
                           false, // IS_VOLATILE
                           false, // IS_VA_ARGS
                           false, // IS_LVALUE_REF
                           false, // IS_RVALUE_REF
                           R, As...> { };

    template<typename R, typename... As>
    struct t_func_<R(As..., ...) const noexcept>
      : t_add_value<R(As..., ...) const noexcept>,
        t_add_func_traits_<true,  // IS_MEMBER,
                           true,  // IS_NOEXCEPT
                           true,  // IS_CONST
                           false, // IS_VOLATILE
                           true,  // IS_VA_ARGS
                           false, // IS_LVALUE_REF
                           false, // IS_RVALUE_REF
                           R, As...> { };
#endif

    template<typename R, typename... As>
    struct t_func_<R(As...) const &>
      : t_add_value<R(As...) const &>,
        t_add_func_traits_<true,  // IS_MEMBER,
                           false, // IS_NOEXCEPT
                           true,  // IS_CONST
                           false, // IS_VOLATILE
                           false, // IS_VA_ARGS
                           true,  // IS_LVALUE_REF
                           false, // IS_RVALUE_REF
                           R, As...> { };

    template<typename R, typename... As>
    struct t_func_<R(As..., ...) const &>
      : t_add_value<R(As..., ...) const &>,
        t_add_func_traits_<true,  // IS_MEMBER,
                           false, // IS_NOEXCEPT
                           true,  // IS_CONST
                           false, // IS_VOLATILE
                           true,  // IS_VA_ARGS
                           true,  // IS_LVALUE_REF
                           false, // IS_RVALUE_REF
                           R, As...
                         > { };

#if __cplusplus >= 201703L
    template<typename R, typename... As>
    struct t_func_<R(As...) const & noexcept>
      : t_add_value<R(As...) const & noexcept>,
        t_add_func_traits_<true,  // IS_MEMBER,
                           true,  // IS_NOEXCEPT
                           true,  // IS_CONST
                           false, // IS_VOLATILE
                           false, // IS_VA_ARGS
                           true,  // IS_LVALUE_REF
                           false, // IS_RVALUE_REF
                           R, As...> { };

    template<typename R, typename... As>
    struct t_func_<R(As..., ...) const & noexcept>
      : t_add_value<R(As..., ...) const & noexcept>,
        t_add_func_traits_<true,  // IS_MEMBER,
                           true,  // IS_NOEXCEPT
                           true,  // IS_CONST
                           false, // IS_VOLATILE
                           true,  // IS_VA_ARGS
                           true,  // IS_LVALUE_REF
                           false, // IS_RVALUE_REF
                           R, As...> { };
#endif

    template<typename R, typename... As>
    struct t_func_<R(As...) const &&>
      : t_add_value<R(As...) const &&>,
        t_add_func_traits_<true,  // IS_MEMBER,
                           false, // IS_NOEXCEPT
                           true,  // IS_CONST
                           false, // IS_VOLATILE
                           false, // IS_VA_ARGS
                           false, // IS_LVALUE_REF
                           true,  // IS_RVALUE_REF
                           R, As...> { };

    template<typename R, typename... As>
    struct t_func_<R(As..., ...) const &&>
      : t_add_value<R(As..., ...) const &&>,
        t_add_func_traits_<true,  // IS_MEMBER,
                           false, // IS_NOEXCEPT
                           true,  // IS_CONST
                           false, // IS_VOLATILE
                           true,  // IS_VA_ARGS
                           false, // IS_LVALUE_REF
                           true,  // IS_RVALUE_REF
                           R, As...> { };

#if __cplusplus >= 201703L
    template<typename R, typename... As>
    struct t_func_<R(As...) const && noexcept>
      : t_add_value<R(As...) const && noexcept>,
        t_add_func_traits_<true,  // IS_MEMBER,
                           true,  // IS_NOEXCEPT
                           true,  // IS_CONST
                           false, // IS_VOLATILE
                           false, // IS_VA_ARGS
                           false, // IS_LVALUE_REF
                           true,  // IS_RVALUE_REF
                           R, As...> { };

    template<typename R, typename... As>
    struct t_func_<R(As..., ...) const && noexcept>
      : t_add_value<R(As..., ...) const && noexcept>,
        t_add_func_traits_<true,  // IS_MEMBER,
                           true,  // IS_NOEXCEPT
                           true,  // IS_CONST
                           false, // IS_VOLATILE
                           true,  // IS_VA_ARGS
                           false, // IS_LVALUE_REF
                           true,  // IS_RVALUE_REF
                           R, As...> { };
#endif

    ///////////////////////////////////////////////////////////////////////////

    template<typename R, typename... As>
    struct t_func_<R(As...) volatile>
      : t_add_value<R(As...) volatile>,
        t_add_func_traits_<true,  // IS_MEMBER,
                           false, // IS_NOEXCEPT
                           false, // IS_CONST
                           true,  // IS_VOLATILE
                           false, // IS_VA_ARGS
                           false, // IS_LVALUE_REF
                           false, // IS_RVALUE_REF
                           R, As...> { };

    template<typename R, typename... As>
    struct t_func_<R(As..., ...) volatile>
      : t_add_value<R(As..., ...) volatile>,
        t_add_func_traits_<true,  // IS_MEMBER,
                           false, // IS_NOEXCEPT
                           false, // IS_CONST
                           true,  // IS_VOLATILE
                           true,  // IS_VA_ARGS
                           false, // IS_LVALUE_REF
                           false, // IS_RVALUE_REF
                           R, As...> { };

#if __cplusplus >= 201703L
    template<typename R, typename... As>
    struct t_func_<R(As...) volatile noexcept>
      : t_add_value<R(As...) volatile noexcept>,
        t_add_func_traits_<true,  // IS_MEMBER,
                           true,  // IS_NOEXCEPT
                           false, // IS_CONST
                           true,  // IS_VOLATILE
                           false, // IS_VA_ARGS
                           false, // IS_LVALUE_REF
                           false, // IS_RVALUE_REF
                           R, As...> { };

    template<typename R, typename... As>
    struct t_func_<R(As..., ...) volatile noexcept>
      : t_add_value<R(As..., ...) volatile noexcept>,
        t_add_func_traits_<true,  // IS_MEMBER,
                           true,  // IS_NOEXCEPT
                           false, // IS_CONST
                           true,  // IS_VOLATILE
                           true,  // IS_VA_ARGS
                           false, // IS_LVALUE_REF
                           false, // IS_RVALUE_REF
                           R, As...> { };
#endif

    template<typename R, typename... As>
    struct t_func_<R(As...) volatile &>
      : t_add_value<R(As...) volatile &>,
        t_add_func_traits_<true,  // IS_MEMBER,
                           false, // IS_NOEXCEPT
                           false, // IS_CONST
                           true,  // IS_VOLATILE
                           false, // IS_VA_ARGS
                           true,  // IS_LVALUE_REF
                           false, // IS_RVALUE_REF
                           R, As...> { };

    template<typename R, typename... As>
    struct t_func_<R(As..., ...) volatile &>
      : t_add_value<R(As..., ...) volatile &>,
        t_add_func_traits_<true,  // IS_MEMBER,
                          false, // IS_NOEXCEPT
                          false, // IS_CONST
                          true,  // IS_VOLATILE
                          true,  // IS_VA_ARGS
                          true,  // IS_LVALUE_REF
                          false, // IS_RVALUE_REF
                          R, As...> { };

#if __cplusplus >= 201703L
    template<typename R, typename... As>
    struct t_func_<R(As...) volatile & noexcept>
      : t_add_value<R(As...) volatile & noexcept>,
        t_add_func_traits_<true,  // IS_MEMBER,
                           true,  // IS_NOEXCEPT
                           false, // IS_CONST
                           true,  // IS_VOLATILE
                           false, // IS_VA_ARGS
                           true,  // IS_LVALUE_REF
                           false, // IS_RVALUE_REF
                           R, As...> { };

    template<typename R, typename... As>
    struct t_func_<R(As..., ...) volatile & noexcept>
      : t_add_value<R(As..., ...) volatile & noexcept>,
        t_add_func_traits_<true,  // IS_MEMBER,
                           true,  // IS_NOEXCEPT
                           false, // IS_CONST
                           true,  // IS_VOLATILE
                           true,  // IS_VA_ARGS
                           true,  // IS_LVALUE_REF
                           false, // IS_RVALUE_REF
                           R, As...> { };
#endif

    template<typename R, typename... As>
    struct t_func_<R(As...) volatile &&>
      : t_add_value<R(As...) volatile &&>,
        t_add_func_traits_<true,  // IS_MEMBER,
                           false, // IS_NOEXCEPT
                           false, // IS_CONST
                           true,  // IS_VOLATILE
                           false, // IS_VA_ARGS
                           false, // IS_LVALUE_REF
                           true,  // IS_RVALUE_REF
                           R, As...> { };

    template<typename R, typename... As>
    struct t_func_<R(As..., ...) volatile &&>
      : t_add_value<R(As..., ...) volatile &&>,
        t_add_func_traits_<true,  // IS_MEMBER,
                           false, // IS_NOEXCEPT
                           false, // IS_CONST
                           true,  // IS_VOLATILE
                           true,  // IS_VA_ARGS
                           false, // IS_LVALUE_REF
                           true,  // IS_RVALUE_REF
                           R, As...> { };

#if __cplusplus >= 201703L
    template<typename R, typename... As>
    struct t_func_<R(As...) volatile && noexcept>
      : t_add_value<R(As...) volatile && noexcept>,
        t_add_func_traits_<true,  // IS_MEMBER,
                           true,  // IS_NOEXCEPT
                           false, // IS_CONST
                           true,  // IS_VOLATILE
                           false, // IS_VA_ARGS
                           false, // IS_LVALUE_REF
                           true,  // IS_RVALUE_REF
                           R, As...> { };

    template<typename R, typename... As>
    struct t_func_<R(As..., ...) volatile && noexcept>
      : t_add_value<R(As..., ...) volatile && noexcept>,
        t_add_func_traits_<true,  // IS_MEMBER,
                           true,  // IS_NOEXCEPT
                           false, // IS_CONST
                           true,  // IS_VOLATILE
                           true,  // IS_VA_ARGS
                           false, // IS_LVALUE_REF
                           true,  // IS_RVALUE_REF
                           R, As...> { };
#endif

    ///////////////////////////////////////////////////////////////////////////

    template<typename R, typename... As>
    struct t_func_<R(As...) const volatile>
      : t_add_value<R(As...) const volatile>,
        t_add_func_traits_<true,  // IS_MEMBER,
                           false, // IS_NOEXCEPT
                           true,  // IS_CONST
                           true,  // IS_VOLATILE
                           false, // IS_VA_ARGS
                           false, // IS_LVALUE_REF
                           false, // IS_RVALUE_REF
                           R, As...
                         > { };

    template<typename R, typename... As>
    struct t_func_<R(As..., ...) const volatile>
      : t_add_value<R(As..., ...) const volatile>,
        t_add_func_traits_<true,  // IS_MEMBER,
                           false, // IS_NOEXCEPT
                           true,  // IS_CONST
                           true,  // IS_VOLATILE
                           true,  // IS_VA_ARGS
                           false, // IS_LVALUE_REF
                           false, // IS_RVALUE_REF
                           R, As...> { };

#if __cplusplus >= 201703L
    template<typename R, typename... As>
    struct t_func_<R(As...) const volatile noexcept>
      : t_add_value<R(As...) const volatile noexcept>,
        t_add_func_traits_<true,  // IS_MEMBER,
                           true,  // IS_NOEXCEPT
                           true,  // IS_CONST
                           true,  // IS_VOLATILE
                           false, // IS_VA_ARGS
                           false, // IS_LVALUE_REF
                           false, // IS_RVALUE_REF
                           R, As...> { };

    template<typename R, typename... As>
    struct t_func_<R(As..., ...) const volatile noexcept>
      : t_add_value<R(As..., ...) const volatile noexcept>,
        t_add_func_traits_<true,  // IS_MEMBER,
                           true,  // IS_NOEXCEPT
                           true,  // IS_CONST
                           true,  // IS_VOLATILE
                           true,  // IS_VA_ARGS
                           false, // IS_LVALUE_REF
                           false, // IS_RVALUE_REF
                           R, As...> { };
#endif

    template<typename R, typename... As>
    struct t_func_<R(As...) const volatile &>
      : t_add_value<R(As...) const volatile &>,
        t_add_func_traits_<true,  // IS_MEMBER,
                           false, // IS_NOEXCEPT
                           true,  // IS_CONST
                           true,  // IS_VOLATILE
                           false, // IS_VA_ARGS
                           true,  // IS_LVALUE_REF
                           false, // IS_RVALUE_REF
                           R, As...> { };

    template<typename R, typename... As>
    struct t_func_<R(As..., ...) const volatile &>
      : t_add_value<R(As..., ...) const volatile &>,
        t_add_func_traits_<true,  // IS_MEMBER,
                          false, // IS_NOEXCEPT
                          true,  // IS_CONST
                          true,  // IS_VOLATILE
                          true,  // IS_VA_ARGS
                          true,  // IS_LVALUE_REF
                          false, // IS_RVALUE_REF
                          R, As...> { };

#if __cplusplus >= 201703L
    template<typename R, typename... As>
    struct t_func_<R(As...) const volatile & noexcept>
      : t_add_value<R(As...) const volatile & noexcept>,
        t_add_func_traits_<true,  // IS_MEMBER,
                           true,  // IS_NOEXCEPT
                           true,  // IS_CONST
                           true,  // IS_VOLATILE
                           false, // IS_VA_ARGS
                           true,  // IS_LVALUE_REF
                           false, // IS_RVALUE_REF
                           R, As...> { };

    template<typename R, typename... As>
    struct t_func_<R(As..., ...) const volatile & noexcept>
      : t_add_value<R(As..., ...) const volatile & noexcept>,
        t_add_func_traits_<true,  // IS_MEMBER,
                           true,  // IS_NOEXCEPT
                           true,  // IS_CONST
                           true,  // IS_VOLATILE
                           true,  // IS_VA_ARGS
                           true,  // IS_LVALUE_REF
                           false, // IS_RVALUE_REF
                           R, As...> { };
#endif

    template<typename R, typename... As>
    struct t_func_<R(As...) const volatile &&>
      : t_add_value<R(As...) const volatile &&>,
        t_add_func_traits_<true,  // IS_MEMBER,
                           false, // IS_NOEXCEPT
                           true,  // IS_CONST
                           true,  // IS_VOLATILE
                           false, // IS_VA_ARGS
                           false, // IS_LVALUE_REF
                           true,  // IS_RVALUE_REF
                           R, As...> { };

    template<typename R, typename... As>
    struct t_func_<R(As..., ...) const volatile &&>
      : t_add_value<R(As..., ...) const volatile &&>,
        t_add_func_traits_<true,  // IS_MEMBER,
                           false, // IS_NOEXCEPT
                           true,  // IS_CONST
                           true,  // IS_VOLATILE
                           true,  // IS_VA_ARGS
                           false, // IS_LVALUE_REF
                           true,  // IS_RVALUE_REF
                           R, As...> { };

#if __cplusplus >= 201703L
    template<typename R, typename... As>
    struct t_func_<R(As...) const volatile && noexcept>
      : t_add_value<R(As...) const volatile && noexcept>,
        t_add_func_traits_<true,  // IS_MEMBER,
                           true,  // IS_NOEXCEPT
                           true,  // IS_CONST
                           true,  // IS_VOLATILE
                           false, // IS_VA_ARGS
                           false, // IS_LVALUE_REF
                           true,  // IS_RVALUE_REF
                           R, As...> { };

    template<typename R, typename... As>
    struct t_func_<R(As..., ...) const volatile && noexcept>
      : t_add_value<R(As..., ...) const volatile && noexcept>,
        t_add_func_traits_<true,  // IS_MEMBER,
                           true,  // IS_NOEXCEPT
                           true,  // IS_CONST
                           true,  // IS_VOLATILE
                           true,  // IS_VA_ARGS
                           false, // IS_LVALUE_REF
                           true,  // IS_RVALUE_REF
                           R, As...> { };
#endif
  }

  template<typename T> using t_func     = help_::t_func_<T>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename T> using t_has_func_ = t_value_of<t_func<T>>;
    template<typename T> using t_is_func_  = t_is_there<t_has_func_, T>;
  }

  template<typename T, typename... Ts>
  using t_is_func         = t_each_is_true<help_::t_is_func_, T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_func     = t_not<t_is_func<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_func     = t_opt_if<O, t_is_func<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_func = t_opt_if<O, t_is_not_func<T, Ts...>>;

  template<typename T, typename... Ts>
  using t_if_func         = t_opt_if_func<t_opt, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_func     = t_opt_if_not_func<t_opt, T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename T>
    using t_is_object_ = t_none_is_true<T, t_is_func, t_is_ref, t_is_void>;
  }

  template<typename T, typename... Ts>
  using t_is_object         = t_each_is_true<help_::t_is_object_, T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_object     = t_not<t_is_object<T>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_object     = t_opt_if<O, t_is_object<T>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_object = t_opt_if<O, t_is_not_object<T>>;

  template<typename T, typename... Ts>
  using t_if_object         = t_opt_if_object<t_opt, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_object     = t_opt_if_not_object<t_opt, T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename T>
    struct t_is_ref_able_help_
      : t_add_result<t_least_one_is_true<T, t_is_object, t_is_ref>> {
    };

    template<typename R, typename... As>
    struct t_is_ref_able_help_<R(As...)>      : t_rtrue { };

    template<typename R, typename... As>
    struct t_is_ref_able_help_<R(As..., ...)> : t_rtrue { };

#if __cplusplus >= 201703L
    template<typename R, typename... As>
    struct t_is_ref_able_help_<R(As...)      noexcept> : t_rtrue { };

    template<typename R, typename... As>
    struct t_is_ref_able_help_<R(As..., ...) noexcept> : t_rtrue { };
#endif

    template<typename T, typename... Ts>
    using t_is_ref_able_ = t_result_of<t_is_ref_able_help_<T>>;
  }

  template<typename T, typename... Ts>
  using t_is_ref_able  = t_each_is_true<help_::t_is_ref_able_, T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_ref_able     = t_not<t_is_ref_able<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_ref_able     = t_opt_if<O, t_is_ref_able<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_ref_able = t_opt_if<O, t_is_not_ref_able<T, Ts...>>;

  template<typename T, typename... Ts>
  using t_if_ref_able         = t_opt_if_ref_able<t_opt, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_ref_able     = t_opt_if_not_ref_able<t_opt, T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename T, typename = t_is_ref_able<T>>
    struct t_add_lvalue_ref_ : t_add_value<T> { };

    template<typename T>
    struct t_add_lvalue_ref_<T&,  t_true> : t_add_value<T&> { };

    template<typename T>
    struct t_add_lvalue_ref_<T&&, t_true> : t_add_value<T&> { };

    template<typename T>
    struct t_add_lvalue_ref_<T,   t_true> : t_add_value<T&> { };
  }

  template<typename T>
  using t_add_lvalue_ref = t_value_of<help_::t_add_lvalue_ref_<T>>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename T, typename = t_is_ref_able<T>>
    struct t_add_rvalue_ref_ : t_add_value<T> { };

    template<typename T>
    struct t_add_rvalue_ref_<T&&, t_true> : t_add_value<T&&> { };

    template<typename T>
    struct t_add_rvalue_ref_<T&,  t_true> : t_add_value<T&&> { };

    template<typename T>
    struct t_add_rvalue_ref_<T,   t_true> : t_add_value<T&&> { };
  }

  template<typename T>
  using t_add_rvalue_ref = t_value_of<help_::t_add_rvalue_ref_<T>>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T> t_add_rvalue_ref<T> uneval       () noexcept; // declval
  template<typename T> t_add_rvalue_ref<T> uneval_rvalue() noexcept;
  template<typename T> t_add_lvalue_ref<T> uneval_lvalue() noexcept;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename T> struct t_remove_ref_      : t_add_value<T> { };
    template<typename T> struct t_remove_ref_<T&>  : t_add_value<T> { };
    template<typename T> struct t_remove_ref_<T&&> : t_add_value<T> { };
  }

  template<typename T>
  using t_remove_ref = t_value_of<help_::t_remove_ref_<T>>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename T> struct t_remove_const_          : t_add_value<T> { };
    template<typename T> struct t_remove_const_<const T> : t_add_value<T> { };
  }

  template<typename T>
  using t_remove_const = t_value_of<help_::t_remove_const_<T>>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename T>
    struct t_remove_volatile_             : t_add_value<T> { };
    template<typename T>
    struct t_remove_volatile_<volatile T> : t_add_value<T> { };
  }

  template<typename T>
  using t_remove_volatile = t_value_of<help_::t_remove_volatile_<T>>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T> using t_remove_cv = t_remove_const<t_remove_volatile<T>>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T> using t_add_const    = const T;
  template<typename T> using t_add_volatile = volatile T;
  template<typename T> using t_add_cv       = t_add_const<t_add_volatile<T>>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename T>
    struct t_is_member_ptr_help_ : t_rfalse { };

    template<typename T, typename U>
    struct t_is_member_ptr_help_<T U::*> : t_rtrue  { };

    template<typename T>
    using t_is_member_ptr_ = t_result_of<t_is_member_ptr_help_<t_remove_cv<T>>>;
  }

  template<typename T, typename... Ts>
  using t_is_member_ptr = t_each_is_true<help_::t_is_member_ptr_, T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_member_ptr     = t_not<t_is_member_ptr<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_member_ptr     = t_opt_if<O, t_is_member_ptr<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_member_ptr = t_opt_if<O, t_is_not_member_ptr<T, Ts...>>;

  template<typename T, typename... Ts>
  using t_if_member_ptr         = t_opt_if_member_ptr<t_opt, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_member_ptr     = t_opt_if_not_member_ptr<t_opt, T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename T>
    using t_is_integer_ = t_is_one_of<T, t_short, t_ushort,
                                         t_int,   t_uint,
                                         t_long,  t_ulong,
                                         t_llong, t_ullong>;
  }

  template<typename T, typename... Ts>
  using t_is_integer         = t_each_is_true<help_::t_is_integer_, T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_integer     = t_not<t_is_integer<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_integer     = t_opt_if<O, t_is_integer<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_integer = t_opt_if<O, t_is_not_integer<T, Ts...>>;

  template<typename T, typename... Ts>
  using t_if_integer         = t_opt_if_integer<t_opt, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_integer     = t_opt_if_not_integer<t_opt, T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename T>
    using t_is_integral_ = t_is_one_of<T, t_bool,
                                          t_char,  t_uchar, t_schar,
                                          t_short, t_ushort,
                                          t_int,   t_uint,
                                          t_long,  t_ulong,
                                          t_llong, t_ullong>;
  }

  template<typename T, typename... Ts>
  using t_is_integral = t_each_is_true<help_::t_is_integral_, T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_integral     = t_not<t_is_integral<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_integral     = t_opt_if<O, t_is_integral<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_integral = t_opt_if<O, t_is_not_integral<T, Ts...>>;

  template<typename T, typename... Ts>
  using t_if_integral         = t_opt_if_integral<t_opt, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_integral     = t_opt_if_not_integral<t_opt, T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename T>
    using t_is_unsigned_integral_ = t_is_one_of<T, t_uchar,
                                                   t_ushort,
                                                   t_uint,
                                                   t_ulong,
                                                   t_ullong>;
  }

  template<typename T, typename... Ts>
  using t_is_unsigned_integral
    = t_each_is_true<help_::t_is_unsigned_integral_, T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_unsigned_integral
    = t_not<t_is_unsigned_integral<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_unsigned_integral
    = t_opt_if<O, t_is_unsigned_integral<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_unsigned_integral
    = t_opt_if<O, t_is_not_unsigned_integral<T, Ts...>>;

  template<typename T, typename... Ts>
  using t_if_unsigned_integral
    = t_opt_if_unsigned_integral<t_opt, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_unsigned_integral
    = t_opt_if_not_unsigned_integral<t_opt, T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename T>
    using t_is_signed_integral_ = t_is_one_of<T, t_char, t_schar,
                                                 t_short,
                                                 t_int,
                                                 t_long,
                                                 t_llong>;
  }

  template<typename T, typename... Ts>
  using t_is_signed_integral
    = t_each_is_true<help_::t_is_signed_integral_, T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_signed_integral
    = t_not<t_is_signed_integral<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_signed_integral
    = t_opt_if<O, t_is_signed_integral<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_signed_integral
    = t_opt_if<O, t_is_not_signed_integral<T, Ts...>>;

  template<typename T, typename... Ts>
  using t_if_signed_integral
    = t_opt_if_signed_integral<t_opt, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_signed_integral
    = t_opt_if_not_signed_integral<t_opt, T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename T>
    using t_is_precision_ = t_is_one_of<T, t_double, t_float, t_ldouble>;
  }

  template<typename T, typename... Ts>
  using t_is_precision = t_each_is_true<help_::t_is_precision_, T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_precision     = t_not<t_is_precision<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_precision     = t_opt_if<O, t_is_precision<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_precision = t_opt_if<O, t_is_not_precision<T, Ts...>>;

  template<typename T, typename... Ts>
  using t_if_precision         = t_opt_if_precision<t_opt, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_precision     = t_opt_if_not_precision<t_opt, T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename T>
    using t_is_arithmetic_ = t_least_one_is_true<T, t_is_integral,
                                                    t_is_precision>;
  }

  template<typename T, typename... Ts>
  using t_is_arithmetic = t_each_is_true<help_::t_is_arithmetic_, T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_arithmetic     = t_not<t_is_arithmetic<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_arithmetic     = t_opt_if<O, t_is_arithmetic<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_arithmetic = t_opt_if<O, t_is_not_arithmetic<T, Ts...>>;

  template<typename T, typename... Ts>
  using t_if_arithmetic         = t_opt_if_arithmetic<t_opt, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_arithmetic     = t_opt_if_not_arithmetic<t_opt, T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename T>
    using t_is_fundamental_ = t_least_one_is_true<T, t_is_arithmetic,
                                                     t_is_void,
                                                     t_is_nullptr>;
  }

  template<typename T, typename... Ts>
  using t_is_fundamental = t_each_is_true<help_::t_is_fundamental_, T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_fundamental     = t_not<t_is_fundamental<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_fundamental     = t_opt_if<O, t_is_fundamental<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_fundamental = t_opt_if<O, t_is_not_fundamental<T, Ts...>>;

  template<typename T, typename... Ts>
  using t_if_fundamental         = t_opt_if_fundamental<t_opt, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_fundamental     = t_opt_if_not_fundamental<t_opt, T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename T>
    using t_is_scalar_ = t_least_one_is_true<T, t_is_arithmetic,
                                                t_is_enum,
                                                t_is_ptr,
                                                t_is_member_ptr,
                                                t_is_nullptr>;
  }

  template<typename T, typename... Ts>
  using t_is_scalar         = t_each_is_true<help_::t_is_scalar_, T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_scalar     = t_not<t_is_scalar<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_scalar     = t_opt_if<O, t_is_scalar<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_scalar = t_opt_if<O, t_is_not_scalar<T, Ts...>>;

  template<typename T, typename... Ts>
  using t_if_scalar         = t_opt_if_scalar<t_opt, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_scalar     = t_opt_if_not_scalar<t_opt, T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename T>
    using t_is_compound_ = t_is_not_fundamental<T>;
  }

  template<typename T, typename... Ts>
  using t_is_compound = t_each_is_true<help_::t_is_compound_, T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_compound     = t_not<t_is_compound<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_compound     = t_opt_if<O, t_is_compound<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_compound = t_opt_if<O, t_is_not_compound<T, Ts...>>;

  template<typename T, typename... Ts>
  using t_if_compound         = t_opt_if_compound<t_opt, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_compound     = t_opt_if_not_compound<t_opt, T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename T> struct t_is_const_help_          : t_rfalse { };
    template<typename T> struct t_is_const_help_<const T> : t_rtrue  { };

    template<typename T>
    using t_is_const_ = t_result_of<help_::t_is_const_help_<T>>;
  }

  template<typename T, typename... Ts>
  using t_is_const         = t_each_is_true<help_::t_is_const_, T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_const     = t_not<t_is_const<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_const     = t_opt_if<O, t_is_const<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_const = t_opt_if<O, t_is_not_const<T, Ts...>>;

  template<typename T, typename... Ts>
  using t_if_const         = t_opt_if_const<t_opt, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_const     = t_opt_if_not_const<t_opt, T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename T> struct t_is_volatile_help_             : t_rfalse { };
    template<typename T> struct t_is_volatile_help_<volatile T> : t_rtrue  { };

    template<typename T>
    using t_is_volatile_ = t_result_of<help_::t_is_volatile_help_<T>>;
  }

  template<typename T, typename... Ts>
  using t_is_volatile = t_each_is_true<help_::t_is_volatile_, T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_volatile     = t_not<t_is_volatile<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_volatile     = t_opt_if<O, t_is_volatile<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_volatile = t_opt_if<O, t_is_not_volatile<T, Ts...>>;

  template<typename T, typename... Ts>
  using t_if_volatile         = t_opt_if_volatile<t_opt, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_volatile     = t_opt_if_not_volatile<t_opt, T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename T>
    using t_is_signed_ = t_all_is_true<T, t_is_arithmetic,
                                          t_is_not_unsigned_integral,
                                          t_is_not_bool>;
  }

  template<typename T, typename... Ts>
  using t_is_signed         = t_each_is_true<help_::t_is_signed_, T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_signed     = t_not<t_is_signed<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_signed     = t_opt_if<O, t_is_signed<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_signed = t_opt_if<O, t_is_not_signed<T, Ts...>>;

  template<typename T, typename... Ts>
  using t_if_signed         = t_opt_if_signed<t_opt, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_signed     = t_opt_if_not_signed<t_opt, T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename T, typename... Ts> struct t_is_unique_;
    template<typename... Ts>             struct t_is_unique_help_;

    template<typename... Ts>
    struct t_is_unique_help_<t_true, Ts...> : t_rfalse {
    };

    template<typename... Ts>
    struct t_is_unique_help_<t_false, Ts...> : t_is_unique_<Ts...> {
    };

    template<typename T, typename... Ts>
    struct t_is_unique_ : t_is_unique_help_<t_is_one_of<T, Ts...>, Ts...> {
    };

    template<typename T> struct t_is_unique_<T> : t_rtrue { };
  }

  template<typename T, typename... Ts>
  using t_is_unique         = t_result_of<help_::t_is_unique_<T, Ts...>>;

  template<typename T, typename... Ts>
  using t_is_not_unique     = t_not<t_is_unique<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_unique     = t_opt_if<O, t_is_unique<T, Ts...>>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_unique = t_opt_if<O, t_is_not_unique<T, Ts...>>;

  template<typename T, typename... Ts>
  using t_if_unique         = t_opt_if_unique<t_opt, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_unique     = t_opt_if_not_unique<t_opt, T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename>
    struct t_is_pack_help_ : t_rfalse { };

    template<typename... Ts>
    struct t_is_pack_help_<t_pack<Ts...>> : t_rtrue { };

    template<typename P>
    using t_is_pack_ = t_result_of<t_is_pack_help_<P>>;
  }

  template<typename P, typename... Ps>
  using t_is_pack         = t_each_is_true<help_::t_is_pack_, P, Ps...>;

  template<typename P, typename... Ps>
  using t_is_not_pack     = t_not<t_is_pack<P, Ps...>>;

  template<typename O, typename P, typename... Ps>
  using t_opt_if_pack     = t_opt_if<O, t_is_pack<P, Ps...>>;

  template<typename O, typename P, typename... Ps>
  using t_opt_if_not_pack = t_opt_if<O, t_is_not_pack<P, Ps...>>;

  template<typename P, typename... Ps>
  using t_if_pack         = t_opt_if_pack<t_opt, P, Ps...>;

  template<typename P, typename... Ps>
  using t_if_not_pack     = t_opt_if_not_pack<t_opt, P, Ps...>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename...> struct t_unique_pack_help_;
    template<typename...> struct t_unique_pack_help1_;

    template<typename... Ts, typename U, typename... Us>
    struct t_unique_pack_help1_<t_false, t_pack<Ts...>, t_pack<U, Us...>>
      : t_unique_pack_help_<t_pack<Ts..., U>, t_pack<Us...>> { };

    template<typename... Ts, typename U, typename... Us>
    struct t_unique_pack_help1_<t_true, t_pack<Ts...>, t_pack<U, Us...>>
      : t_unique_pack_help_<t_pack<Ts...>, t_pack<Us...>> { };

    template<typename... Ts, typename U, typename... Us>
    struct t_unique_pack_help_<t_pack<Ts...>, t_pack<U, Us...>>
      : t_unique_pack_help1_<t_is_one_of<U, Ts...>, t_pack<Ts...>,
                                                    t_pack<U, Us...>> { };

    template<typename... Ts>
    struct t_unique_pack_help_<t_pack<Ts...>, t_empty_pack>
      : t_add_value<t_pack<Ts...>> { };

    template<typename P, typename... Ps>
    struct t_unique_pack_ : t_unique_pack_help_<t_empty_pack, P, Ps...> {
      static_assert(t_is_pack<P, Ps...>::VALUE,
                     "template parameters must be of t_pack<...> type");
    };
  }

  template<typename P, typename... Ps>
  using t_unique_pack = t_value_of<help_::t_unique_pack_<P, Ps...>>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename, typename...> struct t_flatten_pack_help_;

    template<typename... Ts, typename... Hs, typename... Us>
    struct t_flatten_pack_help_<t_pack<Ts...>, t_pack<t_pack<Hs...>, Us...>>
      : t_flatten_pack_help_<t_pack<Ts...>, t_pack<Hs..., Us...>> { };

    template<typename... Ts, typename U, typename... Us>
    struct t_flatten_pack_help_<t_pack<Ts...>, t_pack<U, Us...>>
      : t_flatten_pack_help_<t_pack<Ts..., U>, t_pack<Us...>> { };

    template<typename... Ts>
    struct t_flatten_pack_help_<t_pack<Ts...>, t_empty_pack>
      : t_add_value<t_pack<Ts...>> { };

    template<typename P, typename... Ps>
    struct t_flatten_pack_ : t_flatten_pack_help_<t_empty_pack, P, Ps...> {
      static_assert(t_is_pack<P, Ps...>::VALUE,
                     "template parameters must be of t_pack<...> type");
    };
  }

  template<typename P, typename... Ps>
  using t_flatten_pack = t_value_of<help_::t_flatten_pack_<P, Ps...>>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename P, typename... Ps>
  using t_unique_flatten_pack = t_unique_pack<t_flatten_pack<P, Ps...>>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename...> struct t_is_subset_of_pack_help1_;
    template<typename...> struct t_is_subset_of_pack_help2_;

    template<typename... Ts, typename... Us>
    struct t_is_subset_of_pack_help2_<t_true, t_pack<Ts...>, t_pack<Us...>>
      : t_is_subset_of_pack_help1_<t_pack<Ts...>, t_pack<Us...>> { };

    template<typename... Ts>
    struct t_is_subset_of_pack_help2_<t_false, Ts...> : t_rfalse { };

    template<typename... Us>
    struct t_is_subset_of_pack_help2_<t_true, t_empty_pack, t_pack<Us...>>
      : t_rtrue { };

    template<typename T, typename... Ts, typename... Us>
    struct t_is_subset_of_pack_help1_<t_pack<T, Ts...>, t_pack<Us...>>
      : t_is_subset_of_pack_help2_<t_is_one_of<T, Us...>, t_pack<Ts...>,
                                                          t_pack<Us...>> { };

    template<typename... Ts>
    struct t_is_subset_of_pack_help1_<t_pack<Ts...>, t_empty_pack>
      : t_rfalse { };

    template<typename... Us>
    struct t_is_subset_of_pack_help1_<t_empty_pack, t_pack<Us...>>
      : t_rfalse { };

    template<typename P, typename... Ps>
    struct t_is_subset_of_pack_help_ : t_is_subset_of_pack_help1_<P, Ps...> {
      static_assert(t_is_pack<P, Ps...>::VALUE,
                     "template parameters must be of t_pack<...> type");
    };

    template<typename P, typename P1>
    using t_is_subset_of_pack_ = t_result_of<t_is_subset_of_pack_help_<P, P1>>;
  }

  template<typename P, typename P1, typename... Ps>
  using t_is_subset_of_pack = t_and<help_::t_is_subset_of_pack_<P, P1>,
                                    help_::t_is_subset_of_pack_<P, Ps>...>;

  template<typename P, typename P1, typename... Ps>
  using t_is_not_subset_of_pack
    = t_not<t_is_subset_of_pack<P, P1, Ps...>>;

  template<typename O, typename P, typename P1, typename... Ps>
  using t_opt_if_subset_of_pack
    = t_opt_if<O, t_is_subset_of_pack<P, P1, Ps...>>;

  template<typename O, typename P, typename P1, typename... Ps>
  using t_opt_if_not_subset_of_pack
    = t_opt_if<O, t_is_not_subset_of_pack<P, P1, Ps...>>;

  template<typename P, typename P1, typename... Ps>
  using t_if_subset_of_pack
    = t_opt_if_subset_of_pack<t_opt, P, P1, Ps...>;

  template<typename P, typename P1, typename... Ps>
  using t_if_not_subset_of_pack
    = t_opt_if_not_subset_of_pack<t_opt, P, P1, Ps...>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename, typename> struct t_is_in_pack_help1_;

    template<typename T, typename... Ts>
    struct t_is_in_pack_help1_<T, t_pack<Ts...>>
      : t_add_result<t_is_one_of<T, Ts...>> { };

    template<typename T, typename P>
    struct t_is_in_pack_help_ : t_is_in_pack_help1_<T, P> {
      static_assert(t_is_pack<P>::VALUE,
                    "template parameter P must be of t_pack<...> type");
    };

    template<typename T, typename P>
    using t_is_in_pack_ = t_result_of<help_::t_is_in_pack_help_<T, P>>;
  }

  template<typename T, typename P, typename... Ps>
  using t_is_in_pack = t_and<help_::t_is_in_pack_<T, P>,
                             help_::t_is_in_pack_<T, Ps>...>;


  template<typename T, typename P, typename... Ps>
  using t_is_not_in_pack     = t_not<t_is_in_pack<T, P, Ps...>>;

  template<typename O, typename T, typename P, typename... Ps>
  using t_opt_if_in_pack     = t_opt_if<O, t_is_in_pack<T, P, Ps...>>;

  template<typename O, typename T, typename P, typename... Ps>
  using t_opt_if_not_in_pack = t_opt_if<O, t_is_not_in_pack<T, P, Ps...>>;

  template<typename T, typename P, typename... Ps>
  using t_if_in_pack         = t_opt_if_in_pack<t_opt, T, P, Ps...>;

  template<typename T, typename P, typename... Ps>
  using t_if_not_in_pack     = t_opt_if_not_in_pack<t_opt, T, P, Ps...>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename...> struct t_del_from_pack_help_;
    template<typename...> struct t_del_from_pack_help1_;
    template<typename...> struct t_del_from_pack_help2_;

    template<typename... Us, typename... Ts, typename... T1s>
    struct t_del_from_pack_help2_<t_pack<Us...>, t_pack<Ts...>,
                                  t_pack<T1s...>>
      : t_del_from_pack_help_<t_pack<Us...>, t_pack<Ts...>, t_pack<T1s...>> { };

    template<typename... Us, typename... Ts>
    struct t_del_from_pack_help2_<t_pack<Us...>, t_pack<>, t_pack<Ts...>>
      : t_add_value<t_pack<Us...>> { };

    template<typename... Us, typename T, typename... Ts, typename... T1s>
    struct t_del_from_pack_help1_<t_true, t_pack<Us...>, t_pack<T, Ts...>,
                                  t_pack<T1s...>>
      : t_del_from_pack_help2_<t_pack<Us...>, t_pack<Ts...>,
                               t_pack<T1s...>> { };

    template<typename... Us, typename T, typename... Ts, typename... T1s>
    struct t_del_from_pack_help1_<t_false, t_pack<Us...>, t_pack<T, Ts...>,
                                  t_pack<T1s...>>
      : t_del_from_pack_help2_<t_pack<Us..., T>, t_pack<Ts...>,
                               t_pack<T1s...>> { };

    template<typename... Us, typename T, typename... Ts, typename... T1s>
    struct t_del_from_pack_help_<t_pack<Us...>, t_pack<T, Ts...>,
                                                t_pack<T1s...>>
      :  t_del_from_pack_help1_<t_is_in_pack<T, t_pack<T1s...>>, t_pack<Us...>,
                                t_pack<T, Ts...>, t_pack<T1s...>> { };

    template<typename... Us, typename... Ts>
    struct t_del_from_pack_help_<t_pack<Us...>, t_empty_pack, t_pack<Ts...>>
      :  t_add_value<t_empty_pack> { };

    template<typename... Us, typename... Ts>
    struct t_del_from_pack_help_<t_pack<Us...>, t_pack<Ts...>, t_empty_pack>
      :  t_add_value<t_pack<Ts...>> { };

    template<typename P, typename P1>
    struct t_del_from_pack_ : t_del_from_pack_help_<t_empty_pack, P, P1> {
      static_assert(t_is_pack<P, P1>::VALUE,
                    "template parameter P and P1 must be of t_pack<...> type");
    };
  }

  template<typename P, typename P1>
  using t_del_from_pack = t_value_of<help_::t_del_from_pack_<P, P1>>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename...> struct t_largest_pack_help_;
    template<typename...> struct t_largest_pack_help1_;

    template<typename P, typename P1, typename... Ps>
    struct t_largest_pack_help1_<t_true, P, P1, Ps...>
      : t_largest_pack_help_<P, Ps...> { };

    template<typename P, typename P1, typename... Ps>
    struct t_largest_pack_help1_<t_false, P, P1, Ps...>
      : t_largest_pack_help_<P1, Ps...> { };

    template<typename P, typename P1, typename... Ps>
    struct t_largest_pack_help_<P, P1, Ps...>
      : t_largest_pack_help1_<t_is_truth<(P::N > P1::N)>, P, P1, Ps...> { };

    template<typename P>
    struct t_largest_pack_help_<P> : t_add_value<P> { };

    template<typename P, typename... Ps>
    struct t_largest_pack_ : t_largest_pack_help_<P, Ps...> {
      static_assert(t_is_pack<P, Ps...>::VALUE,
                     "template parameters must be of t_pack<...> type");
    };
  }

  template<typename P, typename... Ps>
  using t_largest_pack = t_value_of<help_::t_largest_pack_<P, Ps...>>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename...> struct t_smallest_pack_help_;
    template<typename...> struct t_smallest_pack_help1_;

    template<typename P, typename P1, typename... Ps>
    struct t_smallest_pack_help1_<t_true, P, P1, Ps...>
      : t_smallest_pack_help_<P, Ps...> { };

    template<typename P, typename P1, typename... Ps>
    struct t_smallest_pack_help1_<t_false, P, P1, Ps...>
      : t_smallest_pack_help_<P1, Ps...> { };

    template<typename P, typename P1, typename... Ps>
    struct t_smallest_pack_help_<P, P1, Ps...>
      : t_smallest_pack_help1_<t_is_truth<(P::N < P1::N)>, P, P1, Ps...> { };

    template<typename P>
    struct t_smallest_pack_help_<P> : t_add_value<P> { };

    template<typename P, typename... Ps>
    struct t_smallest_pack_ : t_smallest_pack_help_<P, Ps...> {
      static_assert(t_is_pack<P, Ps...>::VALUE,
                     "template parameters must be of t_pack<...> type");
    };
  }

  template<typename P, typename... Ps>
  using t_smallest_pack = t_value_of<help_::t_smallest_pack_<P, Ps...>>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename U, typename P, typename... Ps>
    struct t_union_pack_help_;

    template<typename... Us, typename... Ps, typename... P1s>
    struct t_union_pack_help_<t_pack<Us...>, t_pack<Ps...>, P1s...>
      : t_union_pack_help_<t_pack<Us..., Ps...>, t_empty_pack, P1s...> { };

    template<typename... Us, typename... Ps>
    struct t_union_pack_help_<t_pack<Us...>, t_empty_pack, Ps...>
      : t_union_pack_help_<t_pack<Us...>, Ps...> { };

    template<typename... Us>
    struct t_union_pack_help_<t_pack<Us...>, t_empty_pack>
      : t_add_value<t_unique_pack<t_pack<Us...>>> { };

    template<typename P, typename... Ps>
    struct t_union_pack_ : t_union_pack_help_<t_empty_pack, P, Ps...> {
      static_assert(t_is_pack<P, Ps...>::VALUE,
                     "template parameters must be of t_pack<...> type");
    };
  }

  template<typename P, typename... Ps>
  using t_union_pack = t_value_of<help_::t_union_pack_<P, Ps...>>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename...> struct t_intersect_pack_help_;
    template<typename...> struct t_intersect_pack_help1_;
    template<typename...> struct t_intersect_pack_help2_;
    template<typename...> struct t_intersect_pack_help3_;

    template<typename... Us, typename... Ps, typename... P1s>
    struct t_intersect_pack_help3_<t_pack<Us...>, t_pack<Ps...>,
                                   t_pack<P1s...>>
      : t_intersect_pack_help1_<t_pack<Us...>, t_pack<Ps...>,
                                t_pack<P1s...>> { };

    template<typename... Us, typename... Ps>
    struct t_intersect_pack_help3_<t_pack<Us...>, t_empty_pack, t_pack<Ps...>>
      : t_add_value<t_pack<Us...>> { };

    template<typename... Us, typename P, typename... Ps, typename... P1s>
    struct t_intersect_pack_help2_<t_true, t_pack<Us...>, t_pack<P, Ps...>,
                                   t_pack<P1s...>>
      : t_intersect_pack_help3_<t_pack<Us..., P>, t_pack<Ps...>,
                                t_pack<P1s...>> { };

    template<typename... Us, typename P, typename... Ps, typename... P1s>
    struct t_intersect_pack_help2_<t_false, t_pack<Us...>, t_pack<P, Ps...>,
                                   t_pack<P1s...>>
      : t_intersect_pack_help3_<t_pack<Us...>, t_pack<Ps...>,
                                t_pack<P1s...>> { };

    template<typename... Us, typename P, typename... Ps, typename... P1s>
    struct t_intersect_pack_help1_<t_pack<Us...>, t_pack<P, Ps...>,
                                   t_pack<P1s...>>
      : t_intersect_pack_help2_<t_is_one_of<P, P1s...>, t_pack<Us...>,
                                t_pack<P, Ps...>, t_pack<P1s...>> { };


    template<typename... Ps, typename... P1s, typename... P2s>
    struct t_intersect_pack_help_<t_false, t_pack<Ps...>, t_pack<P1s...>,
                                  P2s...>
      : t_intersect_pack_help_<
          t_false,
          t_value_of<t_intersect_pack_help1_<t_empty_pack,
                                             t_pack<Ps...>, t_pack<P1s...>>>,
          P2s...> { };

    template<typename... Ps, typename... P1s>
    struct t_intersect_pack_help_<t_false, t_pack<Ps...>, t_pack<P1s...>>
      : t_intersect_pack_help1_<t_empty_pack, t_pack<Ps...>,
                                t_pack<P1s...>> { };

    template<typename... Ps>
    struct t_intersect_pack_help_<t_false, t_pack<Ps...>>
      : t_add_value<t_pack<Ps...>> { };

    template<typename... Ps>
    struct t_intersect_pack_help_<t_true, Ps...>
      : t_add_value<t_empty_pack> { };

    template<typename P, typename... Ps>
    struct t_intersect_pack_
      : t_intersect_pack_help_<t_is_one_of<t_empty_pack, P, Ps...>, P, Ps...> {
      static_assert(t_is_pack<P, Ps...>::VALUE,
                    "template parameters must be of t_pack<...> type");
    };
  }

  template<typename P, typename... Ps>
  using t_intersect_pack = t_value_of<help_::t_intersect_pack_<P, Ps...>>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename P, typename... Ps>
  using t_diff_pack
    = t_del_from_pack<t_union_pack<P, Ps...>, t_intersect_pack<P, Ps...>>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename T, typename... Ts>
    struct t_union_ {
      template<t_n_ N>
      struct t_sizeof_ {
       constexpr static t_n_ VALUE = t_union_<Ts...>::
         template t_sizeof_<(N > sizeof(T) ?  N : sizeof(T))>::VALUE;
      };
      constexpr static t_n_ SIZEOF_ =
        t_union_<Ts...>::template t_sizeof_<sizeof(T)>::VALUE;

      template<t_n_ N>
      struct t_align_  {
        constexpr static t_n_ VALUE = t_union_<Ts...>::
          template t_align_<(N > alignof(T) ?  N : alignof(T))>::VALUE;
      };
      constexpr static t_n_ ALIGNMENT_ =
        t_union_<Ts...>::template t_align_<alignof(T)>::VALUE;
    };

    template<typename T>
    struct t_union_<T> {
      template<t_n_ N>
      struct t_sizeof_ {
        constexpr static t_n_ VALUE = (N > sizeof(T) ? N : sizeof(T));
      };
      constexpr static t_n_ SIZEOF_ = sizeof(T);

      template<t_n_ N>
      struct t_align_ {
        constexpr static t_n_ VALUE = (N > alignof(T) ? N : alignof(T));
      };
      constexpr static t_n_ ALIGNMENT_ = alignof(T);
    };
  }

  template<typename... Ts>
  struct t_union {
    using t_types = t_pack<Ts...>;
    constexpr static t_n_ SIZEOF    = help_::t_union_<Ts...>::SIZEOF_;
    constexpr static t_n_ ALIGNMENT = help_::t_union_<Ts...>::ALIGNMENT_;
  };

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename> struct t_int_rank_help_;
    template<> struct t_int_rank_help_<t_bool>   : t_add_VALUE<t_uint, 1> { };
    template<> struct t_int_rank_help_<t_char>   : t_add_VALUE<t_uint, 2> { };
    template<> struct t_int_rank_help_<t_schar>  : t_add_VALUE<t_uint, 2> { };
    template<> struct t_int_rank_help_<t_uchar>  : t_add_VALUE<t_uint, 2> { };
    template<> struct t_int_rank_help_<t_short>  : t_add_VALUE<t_uint, 3> { };
    template<> struct t_int_rank_help_<t_ushort> : t_add_VALUE<t_uint, 3> { };
    template<> struct t_int_rank_help_<t_int>    : t_add_VALUE<t_uint, 4> { };
    template<> struct t_int_rank_help_<t_uint>   : t_add_VALUE<t_uint, 4> { };
    template<> struct t_int_rank_help_<t_long>   : t_add_VALUE<t_uint, 5> { };
    template<> struct t_int_rank_help_<t_ulong>  : t_add_VALUE<t_uint, 5> { };
    template<> struct t_int_rank_help_<t_llong>  : t_add_VALUE<t_uint, 6> { };
    template<> struct t_int_rank_help_<t_ullong> : t_add_VALUE<t_uint, 6> { };

    template<typename T>
    struct t_int_rank_ : t_int_rank_help_<T> {
      static_assert(t_is_integral<T>::VALUE,
                    "template parameter must be a integral type");
    };
  }

  template<typename T>
  using t_int_rank = help_::t_int_rank_<T>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename T, typename T1>
    using t_is_greater_int_rank_
      = t_is_truth<(t_int_rank<T>::VALUE > t_int_rank<T1>::VALUE)>;
  }

  template<typename T, typename T1, typename... Ts>
  using t_is_greater_int_rank
    = t_and<help_::t_is_greater_int_rank_<T, T1>,
            help_::t_is_greater_int_rank_<T, Ts>...>;

  template<typename T, typename T1, typename... Ts>
  using t_is_not_greater_int_rank
    = t_not<t_is_greater_int_rank<T, T1, Ts...>>;

  template<typename O, typename T, typename T1, typename... Ts>
  using t_opt_if_greater_int_rank
    = t_opt_if<O, t_is_greater_int_rank<T, T1, Ts...>>;

  template<typename O, typename T, typename T1, typename... Ts>
  using t_opt_if_not_greater_int_rank
    = t_opt_if<O, t_is_not_greater_int_rank<T, T1, Ts...>>;

  template<typename T, typename T1, typename... Ts>
  using t_if_greater_int_rank
    = t_opt_if_greater_int_rank<t_opt, T, T1, Ts...>;

  template<typename T, typename T1, typename... Ts>
  using t_if_not_greater_int_rank
    = t_opt_if_not_greater_int_rank<t_opt, T, T1, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename T, typename T1>
    using t_is_equal_int_rank_
      = t_is_truth<(t_int_rank<T>::VALUE == t_int_rank<T1>::VALUE)>;
  }

  template<typename T, typename T1, typename... Ts>
  using t_is_equal_int_rank
    = t_and<help_::t_is_equal_int_rank_<T, T1>,
            help_::t_is_equal_int_rank_<T, Ts>...>;

  template<typename T, typename T1, typename... Ts>
  using t_is_not_equal_int_rank
    = t_not<t_is_equal_int_rank<T, T1, Ts...>>;

  template<typename O, typename T, typename T1, typename... Ts>
  using t_opt_if_equal_int_rank
    = t_opt_if<O, t_is_equal_int_rank<T, T1, Ts...>>;

  template<typename O, typename T, typename T1, typename... Ts>
  using t_opt_if_not_equal_int_rank
    = t_opt_if<O, t_is_not_equal_int_rank<T, T1, Ts...>>;

  template<typename T, typename T1, typename... Ts>
  using t_if_equal_int_rank
    = t_opt_if_equal_int_rank<t_opt, T, T1, Ts...>;

  template<typename T, typename T1, typename... Ts>
  using t_if_not_equal_int_rank
    = t_opt_if_not_equal_int_rank<t_opt, T, T1, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename T1, typename... Ts>
  using t_is_greater_equal_int_rank = t_or<t_is_equal_int_rank<T, T1, Ts...>,
                                           t_is_greater_int_rank<T, T1, Ts...>>;

  template<typename T, typename T1, typename... Ts>
  using t_is_not_greater_equal_int_rank
    = t_not<t_is_greater_equal_int_rank<T, T1, Ts...>>;

  template<typename O, typename T, typename T1, typename... Ts>
  using t_opt_if_greater_equal_int_rank
    = t_opt_if<O, t_is_greater_equal_int_rank<T, T1, Ts...>>;

  template<typename O, typename T, typename T1, typename... Ts>
  using t_opt_if_not_greater_equal_int_rank
    = t_opt_if<O, t_is_not_greater_equal_int_rank<T, T1, Ts...>>;

  template<typename T, typename T1, typename... Ts>
  using t_if_greater_equal_int_rank
    = t_opt_if_greater_equal_int_rank<t_opt, T, T1, Ts...>;

  template<typename T, typename T1, typename... Ts>
  using t_if_not_greater_equal_int_rank
    = t_opt_if_not_greater_equal_int_rank<t_opt, T, T1, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename...> struct t_greatest_int_rank_help_;
    template<typename...> struct t_greatest_int_rank_help1_;

    template<typename T, typename T1, typename... Ts>
    struct t_greatest_int_rank_help1_<t_false, T, T1, Ts...>
      : t_greatest_int_rank_help_<T1, Ts...> { };

    template<typename T, typename T1, typename... Ts>
    struct t_greatest_int_rank_help1_<t_true, T, T1, Ts...>
      : t_greatest_int_rank_help_<T, Ts...> { };

    template<typename T, typename T1, typename... Ts>
    struct t_greatest_int_rank_help_<T, T1, Ts...>
      : t_greatest_int_rank_help1_<t_is_greater_int_rank<T, T1>,
                                   T, T1, Ts...> { };

    template<typename T>
    struct t_greatest_int_rank_help_<T> : t_add_value<T> { };

    template<typename T, typename... Ts>
    struct t_greatest_int_rank_ : t_greatest_int_rank_help_<T, Ts...> {
      static_assert(t_is_integral<T, Ts...>::VALUE,
                    "template parameters must be of integral type.");
    };
  }

  template<typename T, typename... Ts>
  using t_greatest_int_rank
    = t_value_of<help_::t_greatest_int_rank_<T, Ts...>>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename, typename...> struct t_smallest_int_rank_help_;
    template<typename...> struct t_smallest_int_rank_help1_;

    template<typename T, typename T1, typename... Ts>
    struct t_smallest_int_rank_help1_<t_false, T, T1, Ts...>
      : t_smallest_int_rank_help_<T1, Ts...> { };

    template<typename T, typename T1, typename... Ts>
    struct t_smallest_int_rank_help1_<t_true, T, T1, Ts...>
      : t_smallest_int_rank_help_<T, Ts...> { };

    template<typename T, typename T1, typename... Ts>
    struct t_smallest_int_rank_help_<T, T1, Ts...>
      : t_smallest_int_rank_help1_<t_is_not_greater_equal_int_rank<T, T1>,
                                   T, T1, Ts...> { };

    template<typename T>
    struct t_smallest_int_rank_help_<T> : t_add_value<T> { };

    template<typename T, typename... Ts>
    struct t_smallest_int_rank_ : t_smallest_int_rank_help_<T, Ts...> {
      static_assert(t_is_integral<T, Ts...>::VALUE,
                    "template parameters must be of integral type.");
    };
  }

  template<typename T, typename... Ts>
  using t_smallest_int_rank
    = t_value_of<help_::t_smallest_int_rank_<T, Ts...>>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename T, typename T1>
    struct t_is_same_integral_sign_help_
      : t_add_result<t_or<t_is_signed_integral<T, T1>,
                          t_is_unsigned_integral<T, T1>>> {
      static_assert(t_each_is_true<t_is_integral, T, T1>::VALUE,
                    "template parameter must be a integral type");
    };

    template<typename T, typename T1>
    using t_is_same_integral_sign_
      = t_result_of<t_is_same_integral_sign_help_<T, T1>>;
  }

  template<typename T, typename T1, typename... Ts>
  using t_is_same_integral_sign
    = t_and<help_::t_is_same_integral_sign_<T, T1>,
            help_::t_is_same_integral_sign_<T, Ts>...>;

  template<typename T, typename T1, typename... Ts>
  using t_is_not_same_integral_sign
    = t_not<t_is_same_integral_sign<T, T1, Ts...>>;

  template<typename O, typename T, typename T1, typename... Ts>
  using t_opt_if_same_integral_sign
    = t_opt_if<O, t_is_same_integral_sign<T, T1, Ts...>>;

  template<typename O, typename T, typename T1, typename... Ts>
  using t_opt_if_not_same_integral_sign
    = t_opt_if<O, t_is_not_same_integral_sign<T, T1, Ts...>>;

  template<typename T, typename T1, typename... Ts>
  using t_if_same_integral_sign
    = t_opt_if_same_integral_sign<t_opt, T, T1, Ts...>;

  template<typename T, typename T1, typename... Ts>
  using t_if_not_same_integral_sign
    = t_opt_if_not_same_integral_sign<t_opt, T, T1, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename T> struct t_property_;

    template<>
    struct t_property_<t_char>  : t_add_MAX<t_char, __SCHAR_MAX__> { };

    template<>
    struct t_property_<t_schar> : t_property_<t_char> { };

    template<>
    struct t_property_<t_short> : t_add_MAX<t_short, __SHRT_MAX__> { };

    template<>
    struct t_property_<t_int>   : t_add_MAX<t_int, __INT_MAX__> { };

    template<>
    struct t_property_<t_long>  : t_add_MAX<t_long, __LONG_MAX__> { };

    template<>
    struct t_property_<t_llong> : t_add_MAX<t_llong, __LONG_LONG_MAX__> { };

    template<typename T>
    constexpr auto CALC_MAX_ = MAX_of<help_::t_property_<T>>;

    template<typename T>
    constexpr auto CALC_MIN_  = -CALC_MAX_<T> - 1;

    template<typename T, typename T1>
    constexpr auto CALC_UMAX_ = (CALC_MAX_<T1> * (T)2) + 1;
  }

  template<typename T> struct t_property;

  template<> struct t_property<t_bool>
      : t_add_value <t_bool>,
        t_add_SIZEOF<t_bool>,
        t_add_BITS  <t_bool>,
        t_add_MIN   <t_bool, 0>,
        t_add_MAX   <t_bool, 1> { };

  template<> struct t_property<t_char>
      : t_add_value <t_char>,
        t_add_SIZEOF<t_char>,
        t_add_BITS  <t_char>,
        t_add_MIN   <t_char, help_::CALC_MIN_<t_char>>,
        t_add_MAX   <t_char, help_::CALC_MAX_<t_char>> { };

  template<> struct t_property<t_schar>
      : t_add_value <t_schar>,
        t_add_SIZEOF<t_schar>,
        t_add_BITS  <t_schar>,
        t_add_MIN   <t_schar, help_::CALC_MIN_<t_schar>>,
        t_add_MAX   <t_schar, help_::CALC_MAX_<t_schar>> { };

  template<> struct t_property<t_uchar>
      : t_add_value <t_uchar>,
        t_add_SIZEOF<t_uchar>,
        t_add_BITS  <t_uchar>,
        t_add_MIN   <t_uchar, 0>,
        t_add_MAX   <t_uchar, help_::CALC_UMAX_<t_uchar, t_schar>> { };

  template<> struct t_property<t_short>
      : t_add_value <t_short>,
        t_add_SIZEOF<t_short>,
        t_add_BITS  <t_short>,
        t_add_MIN   <t_short, help_::CALC_MIN_<t_short>>,
        t_add_MAX   <t_short, help_::CALC_MAX_<t_short>> { };

  template<> struct t_property<t_ushort>
      : t_add_value <t_ushort>,
        t_add_SIZEOF<t_ushort>,
        t_add_BITS  <t_ushort>,
        t_add_MIN   <t_ushort, 0>,
        t_add_MAX   <t_ushort, help_::CALC_UMAX_<t_ushort, t_short>> { };

  template<> struct t_property<t_int>
      : t_add_value <t_int>,
        t_add_SIZEOF<t_int>,
        t_add_BITS  <t_int>,
        t_add_MIN   <t_int, help_::CALC_MIN_<t_int>>,
        t_add_MAX   <t_int, help_::CALC_MAX_<t_int>> { };

  template<> struct t_property<t_uint>
      : t_add_value <t_uint>,
        t_add_SIZEOF<t_uint>,
        t_add_BITS  <t_uint>,
        t_add_MIN   <t_uint, 0>,
        t_add_MAX   <t_uint, help_::CALC_UMAX_<t_uint, t_int>> { };

  template<> struct t_property<t_long>
      : t_add_value <t_long>,
        t_add_SIZEOF<t_long>,
        t_add_BITS  <t_long>,
        t_add_MIN   <t_long, help_::CALC_MIN_<t_long>>,
        t_add_MAX   <t_long, help_::CALC_MAX_<t_long>> { };

  template<> struct t_property<t_ulong>
      : t_add_value <t_ulong>,
        t_add_SIZEOF<t_ulong>,
        t_add_BITS  <t_ulong>,
        t_add_MIN   <t_ulong, 0>,
        t_add_MAX   <t_ulong, help_::CALC_UMAX_<t_ulong, t_long>> { };

  template<> struct t_property<t_llong>
      : t_add_value <t_llong>,
        t_add_SIZEOF<t_llong>,
        t_add_BITS  <t_llong>,
        t_add_MIN   <t_llong, help_::CALC_MIN_<t_llong>>,
        t_add_MAX   <t_llong, help_::CALC_MAX_<t_llong>> { };

  template<> struct t_property<t_ullong>
      : t_add_value <t_ullong>,
        t_add_SIZEOF<t_ullong>,
        t_add_BITS  <t_ullong>,
        t_add_MIN   <t_ullong, 0>,
        t_add_MAX   <t_ullong, help_::CALC_UMAX_<t_ullong, t_llong>> { };

  template<> struct t_property<t_float>
      : t_add_value <t_float>,
        t_add_SIZEOF<t_float>,
        t_add_BITS  <t_float> {
    // TODO - other properties must be added
    constexpr static t_float MIN = __FLT_MIN__;
    constexpr static t_float MAX = __FLT_MAX__;
  };

  template<> struct t_property<t_double>
      : t_add_value <t_double>,
        t_add_SIZEOF<t_double>,
        t_add_BITS  <t_double> {
    // TODO - other properties must be added
    constexpr static t_double MIN = __DBL_MIN__;
    constexpr static t_double MAX = __DBL_MAX__;
  };

  template<> struct t_property<t_ldouble>
      : t_add_value <t_ldouble>,
        t_add_SIZEOF<t_ldouble>,
        t_add_BITS  <t_ldouble> {
    // TODO - other properties must be added
    constexpr static t_ldouble MIN = __LDBL_MIN__;
    constexpr static t_ldouble MAX = __LDBL_MAX__;
  };

  /////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
