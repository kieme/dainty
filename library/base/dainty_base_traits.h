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

#ifndef _DAINTY_BASE_TRAITS_H_
#define _DAINTY_BASE_TRAITS_H_

#include "dainty_base_traits_impl.h"

namespace dainty
{
namespace base
{
namespace traits
{
  // RULE t_result must be t_true or t_false (or equivalent)
  // RULE impl_ can easily be replaced by impl1_ and interface remains same.

  /////////////////////////////////////////////////////////////////////////////

  using impl_::t_pack;
  using impl_::t_bool;
  using impl_::t_dummy;
  using impl_::t_undef;
  using impl_::t_undef_value;
  using impl_::t_yes;
  using impl_::t_wellformed;
  using impl_::t_empty_pack;
  using impl_::t_opt1;
  using impl_::t_opt2;
  using impl_::t_opt3;
  using impl_::t_opt4;
  using impl_::t_opt5;
  using impl_::t_opt6;
  using impl_::t_opt7;
  using impl_::uneval;
  using impl_::YES;
  using impl_::OPT1;
  using impl_::OPT2;
  using impl_::OPT3;
  using impl_::OPT4;
  using impl_::OPT5;
  using impl_::OPT6;
  using impl_::OPT7;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T>      using t_add_identity = impl_::t_add_identity<T>;
  template<typename T>      using t_identity_of  = impl_::t_identity_of<T>;

  template<typename T>      using t_add_result   = impl_::t_add_result<T>;
  template<typename T>      using t_result_of    = impl_::t_result_of<T>;

  template<typename T>      using t_add_value    = impl_::t_add_value<T>;
  template<typename T>      using t_value_of     = impl_::t_value_of<T>;

  template<typename T, T V> using t_add_VALUE    = impl_::t_add_VALUE<T, V>;
  template<typename T>  constexpr auto VALUE_of  = impl_::VALUE_of<T>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, T V, typename H = t_dummy>
  using t_constant = impl_::t_constant<T, V, H>;

  /////////////////////////////////////////////////////////////////////////////

  using impl_::t_true;
  using impl_::t_false;

  template<t_bool B> using t_is_truth = impl_::t_is_truth<B>;

  /////////////////////////////////////////////////////////////////////////////

  template<t_bool B> using t_set_result = impl_::t_set_result<B>;

  using t_rtrue  = impl_::t_rtrue;
  using t_rfalse = impl_::t_rfalse;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T> using t_not = impl_::t_not<T>;

  /////////////////////////////////////////////////////////////////////////////

  template<template<typename...> class D, typename... As>
  using t_pack_into    = impl_::t_pack_into<D, As...>;
  template<typename T, typename T1>
  using t_add_to_front = impl_::t_add_to_front<T, T1>;
  template<typename T, typename T1>
  using t_add_to_back  = impl_::t_add_to_back<T, T1>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename I, typename T, typename U>
  using t_if_then_else = impl_::t_if_then_else<I, T, U>;
  template<typename I, typename T>
  using t_if_then      = impl_::t_if_then<I, T>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename I> using t_if     = impl_::t_if<I>;
  template<typename I> using t_if_not = impl_::t_if_not<I>;

  template<typename O, typename T>
  using t_opt_if     = impl_::t_opt_if<O, T>;
  template<typename O, typename T>
  using t_opt_if_not = impl_::t_opt_if_not<O, T>;

  /////////////////////////////////////////////////////////////////////////////

  template<template<typename> class Op, typename T>
  using t_expr_of          = impl_::t_expr_of<Op, T>;
  template<template<typename> class Op, typename T, typename U>
  using t_expr_of_or       = impl_::t_expr_of_or<Op, T, U>;
  template<template<typename> class Op, typename T>
  using t_is_there         = impl_::t_is_there<Op, T>;
  template<template<typename> class Op, typename T>
  using t_is_not_there     = impl_::t_is_not_there<Op, T>;
  template<typename O, template<typename> class Op, typename T>
  using t_opt_if_there     = impl_::t_opt_if_there<O, Op, T>;
  template<typename O, template<typename> class Op, typename T>
  using t_opt_if_not_there = impl_::t_opt_if_not_there<O, Op, T>;
  template<template<typename> class Op, typename T>
  using t_if_there         = impl_::t_if_there<Op, T>;
  template<template<typename> class Op, typename T>
  using t_if_not_there     = impl_::t_if_not_there<Op, T>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T> using t_has_result   = impl_::t_has_result<T>;
  template<typename T> using t_has_value    = impl_::t_has_value<T>;
  template<typename T> using t_has_identity = impl_::t_has_identity<T>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename... Ts> using t_and = impl_::t_and<T, Ts...>;
  template<typename T, typename... Ts> using t_or  = impl_::t_or<T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, template<typename, typename...> class C,
                       template<typename, typename...> class... Cs>
  using t_all_is_true     = impl_::t_all_is_true<T, C, Cs...>;

  template<typename T, template<typename, typename...> class C,
                       template<typename, typename...> class... Cs>
  using t_all_is_not_true = impl_::t_all_is_not_true<T, C, Cs...>;

  template<typename T, template<typename, typename...> class C,
                       template<typename, typename...> class... Cs>
  using t_if_all_true     = impl_::t_if_all_true<T, C, Cs...>;

  template<typename T, template<typename, typename...> class C,
                       template<typename, typename...> class... Cs>
  using t_if_all_not_true = impl_::t_if_all_not_true<T, C, Cs...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, template<typename, typename...> class C,
                       template<typename, typename...> class... Cs>
  using t_none_is_true = impl_::t_none_is_true<T, C, Cs...>;

  template<typename T, template<typename, typename...> class C,
                       template<typename, typename...> class... Cs>
  using t_if_none_true = impl_::t_if_none_true<T, C, Cs...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, template<typename, typename...> class C,
                       template<typename, typename...> class... Cs>
  using t_least_one_is_true = impl_::t_least_one_is_true<T, C, Cs...>;

  template<typename T, template<typename, typename...> class C,
                       template<typename, typename...> class... Cs>
  using t_if_least_one_true = impl_::t_if_least_one_true<T, C, Cs...>;

  /////////////////////////////////////////////////////////////////////////////

  template<template<typename, typename...> class C, typename T, typename... Ts>
  using t_each_is_true = impl_::t_each_is_true<C, T, Ts...>;

  template<template<typename, typename...> class C, typename T, typename...Ts>
  using t_if_each_true = impl_::t_if_each_true<C, T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  template<template<typename, typename...> class C, typename T, typename...Ts>
  using t_each_is_false = impl_::t_each_is_false<C, T, Ts...>;

  template<template<typename, typename...> class C, typename T, typename...Ts>
  using t_if_each_false = impl_::t_if_each_false<C, T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T> using t_is_pack     = impl_::t_is_pack<T>;
  template<typename T> using t_is_not_pack = impl_::t_is_not_pack<T>;
  template<typename T> using t_if_pack     = impl_::t_if_pack<T>;
  template<typename T> using t_if_not_pack = impl_::t_if_not_pack<T>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename T1, typename... Ts>
  using t_is_same     = impl_::t_is_same<T, T1, Ts...>;
  template<typename T, typename T1, typename... Ts>
  using t_is_not_same = impl_::t_is_not_same<T, T1, Ts...>;
  template<typename T, typename T1, typename... Ts>
  using t_if_same     = impl_::t_if_same<T, T1, Ts...>;
  template<typename T, typename T1, typename... Ts>
  using t_if_not_same = impl_::t_if_not_same<T, T1, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T> using t_is_lvalue      = impl_::t_is_lvalue<T>;
  template<typename T> using t_is_not_lvalue  = impl_::t_is_not_lvalue<T>;
  template<typename T> using t_if_lvalue      = impl_::t_if_lvalue<T>;
  template<typename T> using t_if_not_lvalue  = impl_::t_if_not_lvalue<T>;

  template<typename T> using t_is_xvalue      = impl_::t_is_xvalue<T>;
  template<typename T> using t_is_not_xvalue  = impl_::t_is_not_xvalue<T>;
  template<typename T> using t_if_xvalue      = impl_::t_if_xvalue<T>;
  template<typename T> using t_if_not_xvalue  = impl_::t_if_not_xvalue<T>;

  template<typename T> using t_is_prvalue     = impl_::t_is_prvalue<T>;
  template<typename T> using t_is_not_prvalue = impl_::t_is_not_prvalue<T>;
  template<typename T> using t_if_prvalue     = impl_::t_if_prvalue<T>;
  template<typename T> using t_if_not_prvalue = impl_::t_if_not_prvalue<T>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T> using t_is_true = impl_::t_is_true<T>;
  template<typename T> using t_if_true = impl_::t_if_true<T>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T> using t_is_false = impl_::t_is_false<T>;
  template<typename T> using t_if_false = impl_::t_if_false<T>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T> using t_is_result     = impl_::t_is_result<T>;
  template<typename T> using t_is_not_result = impl_::t_is_not_result<T>;
  template<typename T> using t_if_result     = impl_::t_if_result<T>;
  template<typename T> using t_if_not_result = impl_::t_if_not_result<T>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename... Ts>
  using t_is_one_of     = impl_::t_is_one_of<T, Ts...>;
  template<typename T, typename... Ts>
  using t_is_not_one_of = impl_::t_is_not_one_of<T, Ts...>;
  template<typename T, typename... Ts>
  using t_if_one_of     = impl_::t_if_one_of<T, Ts...>;
  template<typename T, typename... Ts>
  using t_if_not_one_of = impl_::t_if_not_one_of<T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T> using t_is_bool     = impl_::t_is_bool<T>;
  template<typename T> using t_is_not_bool = impl_::t_is_not_bool<T>;
  template<typename T> using t_if_bool     = impl_::t_if_bool<T>;
  template<typename T> using t_if_not_bool = impl_::t_if_not_bool<T>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T> using t_is_void     = impl_::t_is_void<T>;
  template<typename T> using t_is_not_void = impl_::t_is_not_void<T>;
  template<typename T> using t_if_void     = impl_::t_if_void<T>;
  template<typename T> using t_if_not_void = impl_::t_if_not_void<T>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T> using t_is_nullptr     = impl_::t_is_nullptr<T>;
  template<typename T> using t_is_not_nullptr = impl_::t_is_not_nullptr<T>;
  template<typename T> using t_if_nullptr     = impl_::t_if_nullptr<T>;
  template<typename T> using t_if_not_nullptr = impl_::t_if_not_nullptr<T>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T> using t_is_enum     = impl_::t_is_enum<T>;
  template<typename T> using t_is_not_enum = impl_::t_is_not_enum<T>;
  template<typename T> using t_if_enum     = impl_::t_if_enum<T>;
  template<typename T> using t_if_not_enum = impl_::t_if_not_enum<T>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename B, typename D, typename... Ds>
  using t_is_base_of     = impl_::t_is_base_of<B, D, Ds...>;
  template<typename B, typename D, typename... Ds>
  using t_is_not_base_of = impl_::t_is_not_base_of<B, D, Ds...>;
  template<typename B, typename D, typename... Ds>
  using t_if_base_of     = impl_::t_if_base_of<B, D, Ds...>;
  template<typename B, typename D, typename... Ds>
  using t_if_not_base_of = impl_::t_if_not_base_of<B, D, Ds...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename D, typename B, typename... Bs>
  using t_is_derived_from     = impl_::t_is_derived_from<D, B, Bs...>;
  template<typename D, typename B, typename... Bs>
  using t_is_not_derived_from = impl_::t_is_not_derived_from<D, B, Bs...>;
  template<typename D, typename B, typename... Bs>
  using t_if_derived_from     = impl_::t_if_derived_from<D, B, Bs...>;
  template<typename D, typename B, typename... Bs>
  using t_if_not_derived_from = impl_::t_if_not_derived_from<D, B, Bs...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T> using t_is_union     = impl_::t_is_union<T>;
  template<typename T> using t_is_not_union = impl_::t_is_not_union<T>;
  template<typename T> using t_if_union     = impl_::t_if_union<T>;
  template<typename T> using t_if_not_union = impl_::t_if_not_union<T>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T> using t_is_class     = impl_::t_is_class<T>;
  template<typename T> using t_is_not_class = impl_::t_is_not_class<T>;
  template<typename T> using t_if_class     = impl_::t_if_class<T>;
  template<typename T> using t_if_not_class = impl_::t_if_not_class<T>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T> using t_is_trivial     = impl_::t_is_trivial<T>;
  template<typename T> using t_is_not_trivial = impl_::t_is_not_trivial<T>;
  template<typename T> using t_if_trivial     = impl_::t_if_trivial<T>;
  template<typename T> using t_if_not_trivial = impl_::t_if_not_trivial<T>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T> using t_is_pod     = impl_::t_is_pod<T>;
  template<typename T> using t_is_not_pod = impl_::t_is_not_pod<T>;
  template<typename T> using t_if_pod     = impl_::t_if_pod<T>;
  template<typename T> using t_if_not_pod = impl_::t_if_not_pod<T>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T> using t_is_literal     = impl_::t_is_literal<T>;
  template<typename T> using t_is_not_literal = impl_::t_is_not_literal<T>;
  template<typename T> using t_if_literal     = impl_::t_if_literal<T>;
  template<typename T> using t_if_not_literal = impl_::t_if_not_literal<T>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T> using t_is_empty     = impl_::t_is_empty<T>;
  template<typename T> using t_is_not_empty = impl_::t_is_not_empty<T>;
  template<typename T> using t_if_empty     = impl_::t_if_empty<T>;
  template<typename T> using t_if_not_empty = impl_::t_if_not_empty<T>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T>
  using t_is_polymorphic     = impl_::t_is_polymorphic<T>;
  template<typename T>
  using t_is_not_polymorphic = impl_::t_is_not_polymorphic<T>;
  template<typename T>
  using t_if_polymorphic     = impl_::t_if_polymorphic<T>;
  template<typename T>
  using t_if_not_polymorphic = impl_::t_if_not_polymorphic<T>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T> using t_is_final     = impl_::t_is_final<T>;
  template<typename T> using t_is_not_final = impl_::t_is_not_final<T>;
  template<typename T> using t_if_final     = impl_::t_if_final<T>;
  template<typename T> using t_if_not_final = impl_::t_if_not_final<T>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T> using t_is_abstract     = impl_::t_is_abstract<T>;
  template<typename T> using t_is_not_abstract = impl_::t_is_not_abstract<T>;
  template<typename T> using t_if_abstract     = impl_::t_if_abstract<T>;
  template<typename T> using t_if_not_abstract = impl_::t_if_not_abstract<T>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T>
  using t_is_trivially_copyable     = impl_::t_is_trivially_copyable<T>;
  template<typename T>
  using t_is_not_trivially_copyable = impl_::t_is_not_trivially_copyable<T>;
  template<typename T>
  using t_if_trivially_copyable     = impl_::t_if_trivially_copyable<T>;
  template<typename T>
  using t_if_not_trivially_copyable = impl_::t_if_not_trivially_copyable<T>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T>
  using t_is_standard_layout     = impl_::t_is_standard_layout<T>;
  template<typename T>
  using t_is_not_standard_layout = impl_::t_is_not_standard_layout<T>;
  template<typename T>
  using t_if_standard_layout     = impl_::t_if_standard_layout<T>;
  template<typename T>
  using t_if_not_standard_layout = impl_::t_if_standard_layout<T>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T>
  using t_is_bounded_array     = impl_::t_is_bounded_array<T>;
  template<typename T>
  using t_is_not_bounded_array = impl_::t_is_not_bounded_array<T>;
  template<typename T>
  using t_if_bounded_array     = impl_::t_if_bounded_array<T>;
  template<typename T>
  using t_if_not_bounded_array = impl_::t_if_not_bounded_array<T>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T>
  using t_is_unbounded_array     = impl_::t_is_unbounded_array<T>;
  template<typename T>
  using t_is_not_unbounded_array = impl_::t_is_not_unbounded_array<T>;
  template<typename T>
  using t_if_unbounded_array     = impl_::t_if_unbounded_array<T>;
  template<typename T>
  using t_if_not_unbounded_array = impl_::t_if_not_unbounded_array<T>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T> using t_is_array     = impl_::t_is_array<T>;
  template<typename T> using t_is_not_array = impl_::t_is_not_array<T>;
  template<typename T> using t_if_array     = impl_::t_if_array<T>;
  template<typename T> using t_if_not_array = impl_::t_if_not_array<T>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename... Ts>
  using t_is_ptr     = impl_::t_is_ptr<T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_ptr = impl_::t_is_not_ptr<T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_ptr     = impl_::t_if_ptr<T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_ptr = impl_::t_if_not_ptr<T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T> using t_is_ref     = impl_::t_is_ref<T>;
  template<typename T> using t_is_not_ref = impl_::t_is_not_ref<T>;
  template<typename T> using t_if_ref     = impl_::t_if_ref<T>;
  template<typename T> using t_if_not_ref = impl_::t_if_not_ref<T>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T>
  using t_is_lvalue_ref     = impl_::t_is_lvalue_ref<T>;
  template<typename T>
  using t_is_not_lvalue_ref = impl_::t_is_not_lvalue_ref<T>;
  template<typename T>
  using t_if_lvalue_ref     = impl_::t_if_lvalue_ref<T>;
  template<typename T>
  using t_if_not_lvalue_ref = impl_::t_if_not_lvalue_ref<T>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T>
  using t_is_rvalue_ref     = impl_::t_is_rvalue_ref<T>;
  template<typename T>
  using t_is_not_rvalue_ref = impl_::t_is_not_rvalue_ref<T>;
  template<typename T>
  using t_if_rvalue_ref     = impl_::t_if_rvalue_ref<T>;
  template<typename T>
  using t_if_not_rvalue_ref = impl_::t_if_not_rvalue_ref<T>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T> using t_is_free_func     = impl_::t_is_free_func<T>;
  template<typename T> using t_is_not_free_func = impl_::t_is_not_free_func<T>;
  template<typename T> using t_if_free_func     = impl_::t_if_free_func<T>;
  template<typename T> using t_if_not_free_func = impl_::t_if_not_free_func<T>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T> using t_member_func = impl_::t_member_func<T>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T> using t_is_func     = impl_::t_is_func<T>;
  template<typename T> using t_is_not_func = impl_::t_is_not_func<T>;
  template<typename T> using t_if_func     = impl_::t_if_func<T>;
  template<typename T> using t_if_not_func = impl_::t_if_not_func<T>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T> using t_is_object     = impl_::t_is_object<T>;
  template<typename T> using t_is_not_object = impl_::t_is_not_object<T>;
  template<typename T> using t_if_object     = impl_::t_if_object<T>;
  template<typename T> using t_if_not_object = impl_::t_if_not_object<T>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T> using t_is_ref_able     = impl_::t_is_ref_able<T>;
  template<typename T> using t_is_not_ref_able = impl_::t_is_not_ref_able<T>;
  template<typename T> using t_if_ref_able     = impl_::t_if_ref_able<T>;
  template<typename T> using t_if_not_ref_able = impl_::t_if_not_ref_able<T>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T> using t_add_lvalue_ref = impl_::t_add_lvalue_ref<T>;
  template<typename T> using t_add_rvalue_ref = impl_::t_add_rvalue_ref<T>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T> using t_remove_ref      = impl_::t_remove_ref<T>;
  template<typename T> using t_remove_const    = impl_::t_remove_const<T>;
  template<typename T> using t_remove_volatile = impl_::t_remove_volatile<T>;
  template<typename T> using t_remove_cv       = impl_::t_remove_cv<T>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T>
  using t_is_member_ptr     = impl_::t_is_member_ptr<T>;
  template<typename T>
  using t_is_not_member_ptr = impl_::t_is_not_member_ptr<T>;
  template<typename T>
  using t_if_member_ptr     = impl_::t_if_member_ptr<T>;
  template<typename T>
  using t_if_not_member_ptr = impl_::t_if_not_member_ptr<T>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T> using t_is_integer     = impl_::t_is_integer<T>;
  template<typename T> using t_is_not_integer = impl_::t_is_not_integer<T>;
  template<typename T> using t_if_integer     = impl_::t_if_integer<T>;
  template<typename T> using t_if_not_integer = impl_::t_if_not_integer<T>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename... Ts>
  using t_is_integral     = impl_::t_is_integral<T, Ts...>;
  template<typename T, typename... Ts>
  using t_is_not_integral = impl_::t_is_not_integral<T, Ts...>;
  template<typename T, typename... Ts>
  using t_if_integral     = impl_::t_if_integral<T, Ts...>;
  template<typename T, typename... Ts>
  using t_if_not_integral = impl_::t_if_not_integral<T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T>
  using t_is_unsigned_integral     = impl_::t_is_unsigned_integral<T>;
  template<typename T>
  using t_is_not_unsigned_integral = impl_::t_is_not_unsigned_integral<T>;
  template<typename T>
  using t_if_unsigned_integral     = impl_::t_if_unsigned_integral<T>;
  template<typename T>
  using t_if_not_unsigned_integral = impl_::t_if_not_unsigned_integral<T>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T>
  using t_is_signed_integral     = impl_::t_is_signed_integral<T>;
  template<typename T>
  using t_is_not_signed_integral = impl_::t_is_not_signed_integral<T>;
  template<typename T>
  using t_if_signed_integral     = impl_::t_if_signed_integral<T>;
  template<typename T>
  using t_if_not_signed_integral = impl_::t_if_not_signed_integral<T>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename... Ts>
  using t_is_precision         = impl_::t_is_precision<T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_precision     = impl_::t_is_not_precision<T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_precision     = impl_::t_opt_if_precision<O, T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_precision = impl_::t_opt_if_not_precision<O, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_precision         = impl_::t_if_precision<T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_precision     = impl_::t_if_not_precision<T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T>
  using t_is_arithmetic     = impl_::t_is_arithmetic<T>;
  template<typename T>
  using t_is_not_arithmetic = impl_::t_is_not_arithmetic<T>;
  template<typename T>
  using t_if_arithmetic     = impl_::t_if_arithmetic<T>;
  template<typename T>
  using t_if_not_arithmetic = impl_::t_if_not_arithmetic<T>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T>
  using t_is_fundamental     = impl_::t_is_fundamental<T>;
  template<typename T>
  using t_is_not_fundamental = impl_::t_is_not_fundamental<T>;
  template<typename T>
  using t_if_fundamental     = impl_::t_if_fundamental<T>;
  template<typename T>
  using t_if_not_fundamental = impl_::t_if_not_fundamental<T>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T> using t_is_scalar     = impl_::t_is_scalar<T>;
  template<typename T> using t_is_not_scalar = impl_::t_is_not_scalar<T>;
  template<typename T> using t_if_scalar     = impl_::t_if_scalar<T>;
  template<typename T> using t_if_not_scalar = impl_::t_if_not_scalar<T>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T> using t_is_compound     = impl_::t_is_compound<T>;
  template<typename T> using t_is_not_compound = impl_::t_is_not_compound<T>;
  template<typename T> using t_if_compound     = impl_::t_if_compound<T>;
  template<typename T> using t_if_not_compound = impl_::t_if_not_compound<T>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T> using t_is_const     = impl_::t_is_const<T>;
  template<typename T> using t_is_not_const = impl_::t_is_not_const<T>;
  template<typename T> using t_if_const     = impl_::t_if_const<T>;
  template<typename T> using t_if_not_const = impl_::t_if_not_const<T>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T> using t_is_volatile     = impl_::t_is_volatile<T>;
  template<typename T> using t_is_not_volatile = impl_::t_is_not_volatile<T>;
  template<typename T> using t_if_volatile     = impl_::t_if_volatile<T>;
  template<typename T> using t_if_not_volatile = impl_::t_if_not_volatile<T>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T> using t_is_signed     = impl_::t_is_signed<T>;
  template<typename T> using t_is_not_signed = impl_::t_is_not_signed<T>;
  template<typename T> using t_if_signed     = impl_::t_if_signed<T>;
  template<typename T> using t_if_not_signed = impl_::t_if_not_signed<T>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T> using t_is_unique     = impl_::t_is_unique<T>;
  template<typename T> using t_is_not_unique = impl_::t_is_not_unique<T>;
  template<typename T> using t_if_unique     = impl_::t_if_unique<T>;
  template<typename T> using t_if_not_unique = impl_::t_if_not_unique<T>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename P, typename... Ps>
  using t_unique_pack  = impl_::t_unique_pack<P, Ps...>;
  template<typename P, typename... Ps>
  using t_flatten_pack = impl_::t_flatten_pack<P, Ps...>;
  template<typename P, typename... Ps>
  using t_unique_flatten_pack = impl_::t_unique_flatten_pack<P, Ps...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename P, typename P1>
  using t_is_subset_of_pack     = impl_::t_is_subset_of_pack<P, P1>;
  template<typename P, typename P1>
  using t_is_not_subset_of_pack = impl_::t_is_not_subset_of_pack<P, P1>;
  template<typename P, typename P1>
  using t_if_subset_of_pack     = impl_::t_if_subset_of_pack<P, P1>;
  template<typename P, typename P1>
  using t_if_not_subset_of_pack = impl_::t_if_not_subset_of_pack<P, P1>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename P, typename... Ps>
  using t_is_in_pack     = impl_::t_is_in_pack<T, P>;
  template<typename P, typename... Ps>
  using t_largest_pack   = impl_::t_largest_pack<P, Ps...>;
  template<typename P, typename... Ps>
  using t_union_pack     = impl_::t_union_pack<P, Ps...>;
  template<typename P, typename... Ps>
  using t_intersect_pack = impl_::t_intersect_pack<P, Ps...>;
  template<typename P, typename... Ps>
  using t_diff_pack     = impl_::t_diff_pack<P, Ps...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename... Ts> using t_union = impl_::t_union<Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T> using t_int_rank = impl_::t_int_rank<T>;

  template<typename T, typename T1, typename... Ts>
  using t_is_equal_int_rank = impl_::t_is_equal_int_rank<T, T1, Ts...>;
  template<typename T, typename T1, typename... Ts>
  using t_is_not_equal_int_rank = impl_::t_is_not_equal_int_rank<T, T1, Ts...>;
  template<typename T, typename T1, typename... Ts>
  using t_if_equal_int_rank = impl_::t_if_equal_int_rank<T, T1, Ts...>;
  template<typename T, typename T1, typename... Ts>
  using t_if_not_equal_int_rank = impl_::t_if_not_equal_int_rank<T, T1, Ts...>;

  template<typename T, typename T1, typename... Ts>
  using t_is_greater_int_rank
    = impl_::t_is_greater_int_rank<T, T1, Ts...>;
  template<typename T, typename T1, typename... Ts>
  using t_is_not_greater_int_rank
    = impl_::t_is_not_greater_int_rank<T, T1, Ts...>;
  template<typename T, typename T1, typename... Ts>
  using t_if_greater_int_rank
    = impl_::t_if_greater_int_rank<T, T1, Ts...>;
  template<typename T, typename T1, typename... Ts>
  using t_if_not_greater_int_rank
    = impl_::t_if_not_greater_int_rank<T, T1, Ts...>;

  template<typename T, typename T1, typename... Ts>
  using t_is_greater_equal_int_rank
    = impl_::t_is_greater_equal_int_rank<T, T1, Ts...>;
  template<typename T, typename T1, typename... Ts>
  using t_is_not_greater_equal_int_rank
    = impl_::t_is_not_greater_equal_int_rank<T, T1, Ts...>;
  template<typename T, typename T1, typename... Ts>
  using t_if_greater_equal_int_rank
    = impl_::t_if_greater_equal_int_rank<T, T1, Ts...>;
  template<typename T, typename T1, typename... Ts>
  using t_if_not_greater_equal_int_rank
    = impl_::t_if_not_greater_equal_int_rank<T, T1, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename T1, typename... Ts>
  using t_is_same_integral_sign = impl_::t_is_same_integral_sign<T, T1, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T> using t_property = impl_::t_property<T>;

  /////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
