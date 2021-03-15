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

#include "impl_/dainty_base_traits_impl.h"

namespace dainty
{
namespace base
{
namespace traits
{
  // RULE t_result must be t_true or t_false (or equivalent)
  // RULE impl_ can easily be replaced by impl1_ and interface remains same.

  /////////////////////////////////////////////////////////////////////////////

  using impl_::t_bool;
  using impl_::t_dummy;
  using impl_::t_opt;
  using impl_::uneval;
  using impl_::WELL_FORMED;

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

  template<typename O, typename I>
  using t_opt_if     = impl_::t_opt_if<O, I>;
  template<typename O, typename I>
  using t_opt_if_not = impl_::t_opt_if_not<O, I>;

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
  using t_all_is_true         = impl_::t_all_is_true<T, C, Cs...>;

  template<typename T, template<typename, typename...> class C,
                       template<typename, typename...> class... Cs>
  using t_all_is_not_true     = impl_::t_all_is_not_true<T, C, Cs...>;

  template<typename O, typename T, template<typename, typename...> class C,
                                   template<typename, typename...> class... Cs>
  using t_opt_if_all_true     = impl_::t_opt_if_all_true<O, T, C, Cs...>;

  template<typename O, typename T, template<typename, typename...> class C,
                                   template<typename, typename...> class... Cs>
  using t_opt_if_all_not_true = impl_::t_opt_if_all_not_true<O, T, C, Cs...>;

  template<typename T, template<typename, typename...> class C,
                       template<typename, typename...> class... Cs>
  using t_if_all_true         = impl_::t_if_all_true<T, C, Cs...>;

  template<typename T, template<typename, typename...> class C,
                       template<typename, typename...> class... Cs>
  using t_if_all_not_true     = impl_::t_if_all_not_true<T, C, Cs...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, template<typename, typename...> class C,
                       template<typename, typename...> class... Cs>
  using t_none_is_true     = impl_::t_none_is_true<T, C, Cs...>;

  template<typename O, typename T, template<typename, typename...> class C,
                                   template<typename, typename...> class... Cs>
  using t_opt_if_none_true = impl_::t_opt_if_none_true<O, T, C, Cs...>;

  template<typename T, template<typename, typename...> class C,
                       template<typename, typename...> class... Cs>
  using t_if_none_true     = impl_::t_if_none_true<T, C, Cs...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, template<typename, typename...> class C,
                       template<typename, typename...> class... Cs>
  using t_least_one_is_true
    = impl_::t_least_one_is_true<T, C, Cs...>;

  template<typename O, typename T, template<typename, typename...> class C,
                                   template<typename, typename...> class... Cs>
  using t_opt_if_least_one_true
    = impl_::t_opt_if_least_one_true<O, T, C, Cs...>;

  template<typename T, template<typename, typename...> class C,
                       template<typename, typename...> class... Cs>
  using t_if_least_one_true
    = impl_::t_if_least_one_true<T, C, Cs...>;

  /////////////////////////////////////////////////////////////////////////////

  template<template<typename, typename...> class C, typename T, typename... Ts>
  using t_each_is_true     = impl_::t_each_is_true<C, T, Ts...>;

  template<typename O, template<typename, typename...> class C,
                       typename T, typename...Ts>
  using t_opt_if_each_true = impl_::t_opt_if_each_true<O, C, T, Ts...>;

  template<template<typename, typename...> class C, typename T, typename... Ts>
  using t_if_each_true     = impl_::t_if_each_true<C, T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  template<template<typename, typename...> class C, typename T, typename... Ts>
  using t_each_is_false     = impl_::t_each_is_false<C, T, Ts...>;

  template<typename O, template<typename, typename...> class C,
                       typename T, typename...Ts>
  using t_opt_if_each_false = impl_::t_opt_if_each_false<O, C, T, Ts...>;

  template<template<typename, typename...> class C, typename T, typename... Ts>
  using t_if_each_false     = impl_::t_if_each_false<C, T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename... Ts>
  using t_is_true     = impl_::t_is_true<T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_true = impl_::t_opt_if_true<O, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_true     = impl_::t_if_true<T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename... Ts>
  using t_is_false     = impl_::t_is_false<T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_false = impl_::t_opt_if_false<O, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_false     = impl_::t_if_false<T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename T1, typename... Ts>
  using t_is_same         = impl_::t_is_same<T, T1, Ts...>;

  template<typename T, typename T1, typename... Ts>
  using t_is_not_same     = impl_::t_is_not_same<T, T1, Ts...>;

  template<typename O, typename T, typename T1, typename... Ts>
  using t_opt_if_same     = impl_::t_opt_if_same<O, T, T1, Ts...>;

  template<typename O, typename T, typename T1, typename... Ts>
  using t_opt_if_not_same = impl_::t_opt_if_not_same<O, T, T1, Ts...>;

  template<typename T, typename T1, typename... Ts>
  using t_if_same         = impl_::t_if_same<T, T1, Ts...>;

  template<typename T, typename T1, typename... Ts>
  using t_if_not_same     = impl_::t_if_not_same<T, T1, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename... Ts>
  using t_is_one_of         = impl_::t_is_one_of<T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_one_of     = impl_::t_is_not_one_of<T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_one_of     = impl_::t_opt_if_one_of<O, T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_one_of = impl_::t_opt_if_not_one_of<O, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_one_of         = impl_::t_if_one_of<T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_one_of     = impl_::t_if_not_one_of<T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename... Ts>
  using t_is_lvalue         = impl_::t_is_lvalue<T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_lvalue     = impl_::t_is_not_lvalue<T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_lvalue     = impl_::t_opt_if_lvalue<O, T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_lvalue = impl_::t_opt_if_not_lvalue<O, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_lvalue         = impl_::t_if_lvalue<T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_lvalue     = impl_::t_if_not_lvalue<T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename... Ts>
  using t_is_xvalue         = impl_::t_is_xvalue<T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_xvalue     = impl_::t_is_not_xvalue<T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_xvalue     = impl_::t_opt_if_xvalue<O, T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_xvalue = impl_::t_opt_if_not_xvalue<O, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_xvalue         = impl_::t_if_xvalue<T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_xvalue     = impl_::t_if_not_xvalue<T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename... Ts>
  using t_is_prvalue         = impl_::t_is_prvalue<T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_prvalue     = impl_::t_is_not_prvalue<T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_prvalue     = impl_::t_opt_if_prvalue<O, T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_prvalue = impl_::t_opt_if_not_prvalue<O, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_prvalue         = impl_::t_if_prvalue<T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_prvalue     = impl_::t_if_not_prvalue<T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename... Ts>
  using t_is_result         = impl_::t_is_result<T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_result     = impl_::t_is_not_result<T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_result     = impl_::t_opt_if_result<O, T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_result = impl_::t_opt_if_not_result<O, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_result         = impl_::t_if_result<T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_result     = impl_::t_if_not_result<T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename... Ts>
  using t_is_bool         = impl_::t_is_bool<T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_bool     = impl_::t_is_not_bool<T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_bool     = impl_::t_opt_if_bool<O, T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_bool = impl_::t_opt_if_not_bool<O, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_bool         = impl_::t_if_bool<T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_bool     = impl_::t_if_not_bool<T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename... Ts>
  using t_is_void         = impl_::t_is_void<T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_void     = impl_::t_is_not_void<T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_void     = impl_::t_opt_if_void<O, T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_void = impl_::t_opt_if_not_void<O, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_void         = impl_::t_if_void<T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_void     = impl_::t_if_not_void<T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename... Ts>
  using t_is_nullptr         = impl_::t_is_nullptr<T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_nullptr     = impl_::t_is_not_nullptr<T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_nullptr     = impl_::t_opt_if_nullptr<O, T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_nullptr = impl_::t_opt_if_not_nullptr<O, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_nullptr         = impl_::t_if_nullptr<T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_nullptr     = impl_::t_if_not_nullptr<T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename... Ts>
  using t_is_enum         = impl_::t_is_enum<T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_enum     = impl_::t_is_not_enum<T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_enum     = impl_::t_opt_if_enum<O, T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_enum = impl_::t_opt_if_not_enum<O, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_enum         = impl_::t_if_enum<T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_enum     = impl_::t_if_not_enum<T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename B, typename D, typename... Ds>
  using t_is_base_of         = impl_::t_is_base_of<B, D, Ds...>;

  template<typename B, typename D, typename... Ds>
  using t_is_not_base_of     = impl_::t_is_not_base_of<B, D, Ds...>;

  template<typename O, typename B, typename D, typename... Ds>
  using t_opt_if_base_of     = impl_::t_opt_if_base_of<O, B, D, Ds...>;

  template<typename O, typename B, typename D, typename... Ds>
  using t_opt_if_not_base_of = impl_::t_opt_if_not_base_of<O, B, D, Ds...>;

  template<typename B, typename D, typename... Ds>
  using t_if_base_of         = impl_::t_if_base_of<B, D, Ds...>;

  template<typename B, typename D, typename... Ds>
  using t_if_not_base_of     = impl_::t_if_not_base_of<B, D, Ds...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename D, typename B, typename... Bs>
  using t_is_derived_from
    = impl_::t_is_derived_from<D, B, Bs...>;

  template<typename D, typename B, typename... Bs>
  using t_is_not_derived_from
   = impl_::t_is_not_derived_from<D, B, Bs...>;

  template<typename O, typename D, typename B, typename... Bs>
  using t_opt_if_derived_from
    = impl_::t_opt_if_derived_from<O, D, B, Bs...>;

  template<typename O, typename D, typename B, typename... Bs>
  using t_opt_if_not_derived_from
    = impl_::t_opt_if_not_derived_from<O, D, B, Bs...>;

  template<typename D, typename B, typename... Bs>
  using t_if_derived_from
    = impl_::t_if_derived_from<D, B, Bs...>;

  template<typename D, typename B, typename... Bs>
  using t_if_not_derived_from
    = impl_::t_if_not_derived_from<D, B, Bs...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename... Ts>
  using t_is_union         = impl_::t_is_union<T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_union     = impl_::t_is_not_union<T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_union     = impl_::t_opt_if_union<O, T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_union = impl_::t_opt_if_not_union<O, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_union         = impl_::t_if_union<T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_union     = impl_::t_if_not_union<T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename... Ts>
  using t_is_class         = impl_::t_is_class<T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_class     = impl_::t_is_not_class<T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_class     = impl_::t_opt_if_class<O, T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_class = impl_::t_opt_if_not_class<O, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_class         = impl_::t_if_class<T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_class     = impl_::t_if_not_class<T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename... Ts>
  using t_is_trivial         = impl_::t_is_trivial<T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_trivial     = impl_::t_is_not_trivial<T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_trivial     = impl_::t_opt_if_trivial<O, T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_trivial = impl_::t_opt_if_not_trivial<O, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_trivial         = impl_::t_if_trivial<T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_trivial     = impl_::t_if_not_trivial<T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename... Ts>
  using t_is_pod         = impl_::t_is_pod<T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_pod     = impl_::t_is_not_pod<T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_pod     = impl_::t_opt_if_pod<O, T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_pod = impl_::t_opt_if_not_pod<O, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_pod         = impl_::t_if_pod<T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_pod     = impl_::t_if_not_pod<T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename... Ts>
  using t_is_literal         = impl_::t_is_literal<T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_literal     = impl_::t_is_not_literal<T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_literal     = impl_::t_opt_if_literal<O, T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_literal = impl_::t_opt_if_not_literal<O, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_literal         = impl_::t_if_literal<T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_literal     = impl_::t_if_not_literal<T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename... Ts>
  using t_is_empty         = impl_::t_is_empty<T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_empty     = impl_::t_is_not_empty<T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_empty     = impl_::t_opt_if_empty<O, T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_empty = impl_::t_opt_if_not_empty<O, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_empty         = impl_::t_if_empty<T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_empty     = impl_::t_if_not_empty<T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename... Ts>
  using t_is_polymorphic         = impl_::t_is_polymorphic<T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_polymorphic     = impl_::t_is_not_polymorphic<T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_polymorphic     = impl_::t_opt_if_polymorphic<O, T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_polymorphic = impl_::t_opt_if_not_polymorphic<O, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_polymorphic         = impl_::t_if_polymorphic<T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_polymorphic     = impl_::t_if_not_polymorphic<T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename... Ts>
  using t_is_final         = impl_::t_is_final<T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_final     = impl_::t_is_not_final<T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_final     = impl_::t_opt_if_final<O, T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_final = impl_::t_opt_if_not_final<O, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_final         = impl_::t_if_final<T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_final     = impl_::t_if_not_final<T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename... Ts>
  using t_is_abstract         = impl_::t_is_abstract<T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_abstract     = impl_::t_is_not_abstract<T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_abstract     = impl_::t_opt_if_abstract<O, T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_abstract = impl_::t_opt_if_not_abstract<O, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_abstract         = impl_::t_if_abstract<T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_abstract     = impl_::t_if_not_abstract<T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename... Ts>
  using t_is_trivially_copyable
    = impl_::t_is_trivially_copyable<T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_trivially_copyable
    = impl_::t_is_not_trivially_copyable<T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_trivially_copyable
    = impl_::t_opt_if_trivially_copyable<O, T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_trivially_copyable
    = impl_::t_opt_if_not_trivially_copyable<O, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_trivially_copyable
    = impl_::t_if_trivially_copyable<T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_trivially_copyable
    = impl_::t_if_not_trivially_copyable<T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename... Ts>
  using t_is_standard_layout
    = impl_::t_is_standard_layout<T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_standard_layout
    = impl_::t_is_not_standard_layout<T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_standard_layout
    = impl_::t_opt_if_standard_layout<O, T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_standard_layout
    = impl_::t_opt_if_standard_layout<O, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_standard_layout
    = impl_::t_if_standard_layout<T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_standard_layout
    = impl_::t_if_standard_layout<T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename... Ts>
  using t_is_bounded_array
    = impl_::t_is_bounded_array<T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_bounded_array
    = impl_::t_is_not_bounded_array<T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_bounded_array
    = impl_::t_opt_if_bounded_array<O, T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_bounded_array
    = impl_::t_opt_if_not_bounded_array<O, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_bounded_array
    = impl_::t_if_bounded_array<T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_bounded_array
    = impl_::t_if_not_bounded_array<T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename... Ts>
  using t_is_unbounded_array
    = impl_::t_is_unbounded_array<T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_unbounded_array
    = impl_::t_is_not_unbounded_array<T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_unbounded_array
    = impl_::t_opt_if_unbounded_array<O, T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_unbounded_array
    = impl_::t_opt_if_not_unbounded_array<O, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_unbounded_array
    = impl_::t_if_unbounded_array<T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_unbounded_array
    = impl_::t_if_not_unbounded_array<T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename... Ts>
  using t_is_array         = impl_::t_is_array<T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_array     = impl_::t_is_not_array<T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_array     = impl_::t_opt_if_array<O, T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_array = impl_::t_opt_if_not_array<O, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_array         = impl_::t_if_array<T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_array     = impl_::t_if_not_array<T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename... Ts>
  using t_is_ptr        = impl_::t_is_ptr<T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_ptr    = impl_::t_is_not_ptr<T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_ptr     = impl_::t_opt_if_ptr<O, T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_ptr = impl_::t_opt_if_not_ptr<O, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_ptr         = impl_::t_if_ptr<T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_ptr     = impl_::t_if_not_ptr<T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename... Ts>
  using t_is_ref         = impl_::t_is_ref<T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_ref     = impl_::t_is_not_ref<T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_ref     = impl_::t_opt_if_ref<O, T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_ref = impl_::t_opt_if_not_ref<O, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_ref         = impl_::t_if_ref<T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_ref     = impl_::t_if_not_ref<T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename... Ts>
  using t_is_lvalue_ref         = impl_::t_is_lvalue_ref<T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_lvalue_ref     = impl_::t_is_not_lvalue_ref<T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_lvalue_ref     = impl_::t_opt_if_lvalue_ref<O, T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_lvalue_ref = impl_::t_opt_if_not_lvalue_ref<O, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_lvalue_ref         = impl_::t_if_lvalue_ref<T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_lvalue_ref     = impl_::t_if_not_lvalue_ref<T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename... Ts>
  using t_is_rvalue_ref         = impl_::t_is_rvalue_ref<T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_rvalue_ref     = impl_::t_is_not_rvalue_ref<T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_rvalue_ref     = impl_::t_opt_if_rvalue_ref<O, T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_rvalue_ref = impl_::t_opt_if_not_rvalue_ref<O, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_rvalue_ref         = impl_::t_if_rvalue_ref<T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_rvalue_ref     = impl_::t_if_not_rvalue_ref<T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename... Ts>
  using t_is_free_func         = impl_::t_is_free_func<T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_free_func     = impl_::t_is_not_free_func<T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_free_func     = impl_::t_opt_if_free_func<O, T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_free_func = impl_::t_opt_if_not_free_func<O, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_free_func         = impl_::t_if_free_func<T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_free_func     = impl_::t_if_not_free_func<T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T> using t_member_func = impl_::t_member_func<T>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename... Ts>
  using t_is_func         = impl_::t_is_func<T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_func     = impl_::t_is_not_func<T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_func     = impl_::t_opt_if_func<O, T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_func = impl_::t_opt_if_not_func<O, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_func         = impl_::t_if_func<T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_func     = impl_::t_if_not_func<T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename... Ts>
  using t_is_object         = impl_::t_is_object<T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_object     = impl_::t_is_not_object<T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_object     = impl_::t_opt_if_object<O, T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_object = impl_::t_opt_if_not_object<O, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_object         = impl_::t_if_object<T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_object     = impl_::t_if_not_object<T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename... Ts>
  using t_is_ref_able         = impl_::t_is_ref_able<T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_ref_able     = impl_::t_is_not_ref_able<T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_ref_able     = impl_::t_opt_if_ref_able<O, T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_ref_able = impl_::t_opt_if_not_ref_able<O, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_ref_able         = impl_::t_if_ref_able<T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_ref_able     = impl_::t_if_not_ref_able<T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T> using t_add_lvalue_ref = impl_::t_add_lvalue_ref<T>;
  template<typename T> using t_add_rvalue_ref = impl_::t_add_rvalue_ref<T>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T> using t_remove_ref      = impl_::t_remove_ref<T>;
  template<typename T> using t_remove_const    = impl_::t_remove_const<T>;
  template<typename T> using t_remove_volatile = impl_::t_remove_volatile<T>;
  template<typename T> using t_remove_cv       = impl_::t_remove_cv<T>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename... Ts>
  using t_is_member_ptr         = impl_::t_is_member_ptr<T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_member_ptr     = impl_::t_is_not_member_ptr<T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_member_ptr     = impl_::t_opt_if_member_ptr<O, T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_member_ptr = impl_::t_opt_if_not_member_ptr<O, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_member_ptr         = impl_::t_if_member_ptr<T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_member_ptr     = impl_::t_if_not_member_ptr<T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename... Ts>
  using t_is_integer         = impl_::t_is_integer<T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_integer     = impl_::t_is_not_integer<T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_integer     = impl_::t_opt_if_integer<O, T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_integer = impl_::t_opt_if_not_integer<O, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_integer         = impl_::t_if_integer<T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_integer     = impl_::t_if_not_integer<T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename... Ts>
  using t_is_integral         = impl_::t_is_integral<T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_integral     = impl_::t_is_not_integral<T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_integral     = impl_::t_opt_if_integral<O, T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_integral = impl_::t_opt_if_not_integral<O, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_integral         = impl_::t_if_integral<T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_integral     = impl_::t_if_not_integral<T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename... Ts>
  using t_is_unsigned_integral
    = impl_::t_is_unsigned_integral<T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_unsigned_integral
    = impl_::t_is_not_unsigned_integral<T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_unsigned_integral
    = impl_::t_opt_if_unsigned_integral<O, T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_unsigned_integral
    = impl_::t_opt_if_not_unsigned_integral<O, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_unsigned_integral
    = impl_::t_if_unsigned_integral<T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_unsigned_integral
    = impl_::t_if_not_unsigned_integral<T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename... Ts>
  using t_is_signed_integral
    = impl_::t_is_signed_integral<T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_signed_integral
    = impl_::t_is_not_signed_integral<T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_signed_integral
    = impl_::t_opt_if_signed_integral<O, T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_signed_integral
    = impl_::t_opt_if_not_signed_integral<O, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_signed_integral
    = impl_::t_if_signed_integral<T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_signed_integral
    = impl_::t_if_not_signed_integral<T, Ts...>;

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

  template<typename T, typename... Ts>
  using t_is_arithmetic         = impl_::t_is_arithmetic<T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_arithmetic     = impl_::t_is_not_arithmetic<T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_arithmetic     = impl_::t_opt_if_arithmetic<O, T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_arithmetic = impl_::t_opt_if_not_arithmetic<O, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_arithmetic         = impl_::t_if_arithmetic<T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_arithmetic     = impl_::t_if_not_arithmetic<T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename... Ts>
  using t_is_fundamental         = impl_::t_is_fundamental<T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_fundamental     = impl_::t_is_not_fundamental<T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_fundamental     = impl_::t_opt_if_fundamental<O, T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_fundamental = impl_::t_opt_if_not_fundamental<O, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_fundamental         = impl_::t_if_fundamental<T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_fundamental     = impl_::t_if_not_fundamental<T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename... Ts>
  using t_is_scalar         = impl_::t_is_scalar<T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_scalar     = impl_::t_is_not_scalar<T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_scalar     = impl_::t_opt_if_scalar<O, T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_scalar = impl_::t_opt_if_not_scalar<O, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_scalar         = impl_::t_if_scalar<T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_scalar     = impl_::t_if_not_scalar<T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename... Ts>
  using t_is_compound         = impl_::t_is_compound<T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_compound     = impl_::t_is_not_compound<T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_compound     = impl_::t_opt_if_compound<O, T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_compound = impl_::t_opt_if_not_compound<O, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_compound         = impl_::t_if_compound<T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_compound     = impl_::t_if_not_compound<T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename... Ts>
  using t_is_const         = impl_::t_is_const<T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_const     = impl_::t_is_not_const<T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_const     = impl_::t_opt_if_const<O, T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_const = impl_::t_opt_if_not_const<O, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_const         = impl_::t_if_const<T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_const     = impl_::t_if_not_const<T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename... Ts>
  using t_is_volatile         = impl_::t_is_volatile<T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_volatile     = impl_::t_is_not_volatile<T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_volatile     = impl_::t_opt_if_volatile<O, T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_volatile = impl_::t_opt_if_not_volatile<O, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_volatile         = impl_::t_if_volatile<T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_volatile     = impl_::t_if_not_volatile<T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename... Ts>
  using t_is_signed         = impl_::t_is_signed<T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_signed     = impl_::t_is_not_signed<T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_signed     = impl_::t_opt_if_signed<O, T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_signed = impl_::t_opt_if_not_signed<O, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_signed         = impl_::t_if_signed<T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_signed     = impl_::t_if_not_signed<T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename... Ts>
  using t_is_unique         = impl_::t_is_unique<T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_unique     = impl_::t_is_not_unique<T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_unique     = impl_::t_opt_if_unique<O, T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_unique = impl_::t_opt_if_not_unique<O, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_unique         = impl_::t_if_unique<T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_unique     = impl_::t_if_not_unique<T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename... Ts>
  using t_is_pack         = impl_::t_is_pack<T, Ts...>;

  template<typename T, typename... Ts>
  using t_is_not_pack     = impl_::t_is_not_pack<T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_pack     = impl_::t_opt_if_pack<O, T, Ts...>;

  template<typename O, typename T, typename... Ts>
  using t_opt_if_not_pack = impl_::t_opt_if_not_pack<O, T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_pack         = impl_::t_if_pack<T, Ts...>;

  template<typename T, typename... Ts>
  using t_if_not_pack     = impl_::t_if_not_pack<T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename P, typename... Ps>
  using t_unique_pack  = impl_::t_unique_pack<P, Ps...>;

  template<typename P, typename... Ps>
  using t_flatten_pack = impl_::t_flatten_pack<P, Ps...>;

  template<typename P, typename... Ps>
  using t_unique_flatten_pack = impl_::t_unique_flatten_pack<P, Ps...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename P, typename P1, typename... Ps>
  using t_is_subset_of_pack
    = impl_::t_is_subset_of_pack<P, P1, Ps...>;

  template<typename P, typename P1, typename... Ps>
  using t_is_not_subset_of_pack
    = impl_::t_is_not_subset_of_pack<P, P1, Ps...>;

  template<typename O, typename P, typename P1, typename... Ps>
  using t_opt_if_subset_of_pack
    = impl_::t_opt_if_subset_of_pack<O, P, P1, Ps...>;

  template<typename O, typename P, typename P1, typename... Ps>
  using t_opt_if_not_subset_of_pack
    = impl_::t_opt_if_not_subset_of_pack<O, P, P1, Ps...>;

  template<typename P, typename P1, typename... Ps>
  using t_if_subset_of_pack
    = impl_::t_if_subset_of_pack<P, P1, Ps...>;

  template<typename P, typename P1, typename... Ps>
  using t_if_not_subset_of_pack
    = impl_::t_if_not_subset_of_pack<P, P1, Ps...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename P, typename... Ps>
  using t_is_in_pack         = impl_::t_is_in_pack<T, P>;

  template<typename T, typename P, typename... Ps>
  using t_is_not_in_pack     = impl_::t_is_not_in_pack<T, P>;

  template<typename O, typename T, typename P, typename... Ps>
  using t_opt_if_in_pack     = impl_::t_opt_if_in_pack<O, T, P>;

  template<typename O, typename T, typename P, typename... Ps>
  using t_opt_if_not_in_pack = impl_::t_opt_if_not_in_pack<O, T, P>;

  template<typename T, typename P, typename... Ps>
  using t_if_in_pack         = impl_::t_if_in_pack<T, P>;

  template<typename T, typename P, typename... Ps>
  using t_if_not_in_pack     = impl_::t_if_not_in_pack<T, P>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename P, typename P1>
  using t_del_from_pack   = impl_::t_del_from_pack<P, P1>;

  template<typename P, typename... Ps>
  using t_largest_pack   = impl_::t_largest_pack<P, Ps...>;

  template<typename P, typename... Ps>
  using t_smallest_pack  = impl_::t_smallest_pack<P, Ps...>;

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

  template<typename T, typename... Ts>
  using t_greatest_int_rank = impl_::t_greatest_int_rank<T, Ts...>;

  template<typename T, typename... Ts>
  using t_smallest_int_rank  = impl_::t_smallest_int_rank<T, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename T1, typename... Ts>
  using t_is_equal_int_rank
    = impl_::t_is_equal_int_rank<T, T1, Ts...>;

  template<typename T, typename T1, typename... Ts>
  using t_is_not_equal_int_rank
    = impl_::t_is_not_equal_int_rank<T, T1, Ts...>;

  template<typename O, typename T, typename T1, typename... Ts>
  using t_opt_if_equal_int_rank
    = impl_::t_opt_if_equal_int_rank<O, T, T1, Ts...>;

  template<typename O, typename T, typename T1, typename... Ts>
  using t_opt_if_not_equal_int_rank
    = impl_::t_opt_if_not_equal_int_rank<O, T, T1, Ts...>;

  template<typename T, typename T1, typename... Ts>
  using t_if_equal_int_rank
    = impl_::t_if_equal_int_rank<T, T1, Ts...>;

  template<typename T, typename T1, typename... Ts>
  using t_if_not_equal_int_rank
    = impl_::t_if_not_equal_int_rank<T, T1, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename T1, typename... Ts>
  using t_is_greater_int_rank
    = impl_::t_is_greater_int_rank<T, T1, Ts...>;

  template<typename T, typename T1, typename... Ts>
  using t_is_not_greater_int_rank
    = impl_::t_is_not_greater_int_rank<T, T1, Ts...>;

  template<typename O, typename T, typename T1, typename... Ts>
  using t_opt_if_greater_int_rank
    = impl_::t_opt_if_greater_int_rank<O, T, T1, Ts...>;

  template<typename O, typename T, typename T1, typename... Ts>
  using t_opt_if_not_greater_int_rank
    = impl_::t_opt_if_not_greater_int_rank<O, T, T1, Ts...>;

  template<typename T, typename T1, typename... Ts>
  using t_if_greater_int_rank
    = impl_::t_if_greater_int_rank<T, T1, Ts...>;

  template<typename T, typename T1, typename... Ts>
  using t_if_not_greater_int_rank
    = impl_::t_if_not_greater_int_rank<T, T1, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename T1, typename... Ts>
  using t_is_greater_equal_int_rank
    = impl_::t_is_greater_equal_int_rank<T, T1, Ts...>;

  template<typename T, typename T1, typename... Ts>
  using t_is_not_greater_equal_int_rank
    = impl_::t_is_not_greater_equal_int_rank<T, T1, Ts...>;

  template<typename O, typename T, typename T1, typename... Ts>
  using t_opt_if_greater_equal_int_rank
    = impl_::t_opt_if_greater_equal_int_rank<O, T, T1, Ts...>;

  template<typename O, typename T, typename T1, typename... Ts>
  using t_opt_if_not_greater_equal_int_rank
    = impl_::t_opt_if_not_greater_equal_int_rank<O, T, T1, Ts...>;

  template<typename T, typename T1, typename... Ts>
  using t_if_greater_equal_int_rank
    = impl_::t_if_greater_equal_int_rank<T, T1, Ts...>;

  template<typename T, typename T1, typename... Ts>
  using t_if_not_greater_equal_int_rank
    = impl_::t_if_not_greater_equal_int_rank<T, T1, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename T1, typename... Ts>
  using t_is_same_integral_sign
    = impl_::t_is_same_integral_sign<T, T1, Ts...>;

  template<typename T, typename T1, typename... Ts>
  using t_is_not_same_integral_sign
    = impl_::t_is_not_same_integral_sign<T, T1, Ts...>;

  template<typename O, typename T, typename T1, typename... Ts>
  using t_opt_if_same_integral_sign
    = impl_::t_opt_if_same_integral_sign<O, T, T1, Ts...>;

  template<typename O, typename T, typename T1, typename... Ts>
  using t_opt_if_not_same_integral_sign
    = impl_::t_opt_if_not_same_integral_sign<O, T, T1, Ts...>;

  template<typename T, typename T1, typename... Ts>
  using t_if_same_integral_sign
    = impl_::t_if_same_integral_sign<T, T1, Ts...>;

  template<typename T, typename T1, typename... Ts>
  using t_if_not_same_integral_sign
    = impl_::t_if_not_same_integral_sign<T, T1, Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T> using t_property = impl_::t_property<T>;

  /////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
