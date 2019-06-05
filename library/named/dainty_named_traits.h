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

#ifndef _DAINTY_NAMED_TRAITS_H_
#define _DAINTY_NAMED_TRAITS_H_

#include "dainty_named_types.h"

namespace dainty
{
namespace named
{
///////////////////////////////////////////////////////////////////////////////

  template<typename T> struct t_ident     { using t_identity = T; }; // identity_type
  template<typename T> struct t_result_is { using t_result   = T; };

///////////////////////////////////////////////////////////////////////////////

  struct t_true  : t_ident<t_true>  { constexpr static t_bool VALUE = true;  };
  struct t_false : t_ident<t_false> { constexpr static t_bool VALUE = false; };

///////////////////////////////////////////////////////////////////////////////

  template<typename> struct t_test;

  enum t_ok { };
  template<typename...> using t_test_well_formed = t_ok; // void_t

///////////////////////////////////////////////////////////////////////////////

  template<typename... Args>
  struct t_pack {
    constexpr static const t_size N = sizeof...(Args);

    template<template<typename...> class D>
    using t_into = D<Args...>;
  };

///////////////////////////////////////////////////////////////////////////////

  template<typename I, typename T, typename U>
  struct t_if_then_else : t_if_then_else<typename I::t_result, T, U> { };

  template<typename T, typename U>
  struct t_if_then_else<t_true,  T, U> : t_result_is<T> { };

  template<typename T, typename U>
  struct t_if_then_else<t_false, T, U> : t_result_is<U> { };

///////////////////////////////////////////////////////////////////////////////

  template<typename I, typename T>
  struct t_if_then : t_if_then<typename I::t_result, T> { };

  template<typename T> struct t_if_then<t_true, T> { using t_result = T; };

///////////////////////////////////////////////////////////////////////////////

  template<typename I> using t_if = t_if_then<I, t_ok>; // enable_if

///////////////////////////////////////////////////////////////////////////////

  template<t_bool> struct t_bool_result       : t_result_is<t_false> { };
  template<>       struct t_bool_result<true> : t_result_is<t_true>  { };

  using t_TRUE_  = t_bool_result<true>;  // shorthand
  using t_FALSE_ = t_bool_result<false>; // shorthand

///////////////////////////////////////////////////////////////////////////////

  template<typename T> struct t_not          : t_not<typename T::t_result> { };
  template<>           struct t_not<t_true>  : t_FALSE_ { };
  template<>           struct t_not<t_false> : t_TRUE_  { };

///////////////////////////////////////////////////////////////////////////////

  namespace impl_ {
    template<template<typename> class Op, typename T, typename U, typename E>
    struct t_is_there_ : t_ident<U>, t_FALSE_ { };

    template<template<typename> class Op, typename T, typename U>
    struct t_is_there_<Op, T, U, t_test_well_formed<Op<T>>>
      : t_ident<Op<T>>, t_TRUE_ { };
  }

  template<template<typename> class Op, typename T>
  using t_is_there = impl_::t_is_there_<Op, T, t_ok, t_ok>;

  template<template<typename> class Op, typename T, typename U>
  using t_there_or = typename impl_::t_is_there_<Op, T, U, t_ok>::t_identity;

  template<typename T> using t_result     = decltype(T::t_result);
  template<typename T> using t_has_result = t_is_there<t_result, T>;

  template<typename T> using t_identity     = decltype(T::t_identity);
  template<typename T> using t_has_identity = t_is_there<t_identity, T>;

///////////////////////////////////////////////////////////////////////////////

  template<typename...> struct t_and;

  template<typename T, typename... Ts>
  struct t_and<T, Ts...> : t_and<typename T::t_result, Ts...> { };

  template<typename... Ts> struct t_and<t_true,  Ts...> : t_and<Ts...> { };
  template<typename... Ts> struct t_and<t_false, Ts...> : t_FALSE_ { };

  template<> struct t_and<t_true>  : t_TRUE_  { };
  template<> struct t_and<t_false> : t_FALSE_ { };

///////////////////////////////////////////////////////////////////////////////

  template<typename...> struct t_or;

  template<typename T, typename... Ts>
  struct t_or<T, Ts...> : t_or<typename T::t_result, Ts...> { };

  template<typename... Ts> struct t_or<t_true,  Ts...> : t_TRUE_     { };
  template<typename... Ts> struct t_or<t_false, Ts...> : t_or<Ts...> { };

  template<> struct t_or<t_true>  : t_TRUE_  { };
  template<> struct t_or<t_false> : t_FALSE_ { };

///////////////////////////////////////////////////////////////////////////////

  template<typename, template<typename> class...> struct t_all_is_true;

  template<typename T, template<typename> class C,
                       template<typename> class... Cs>
  struct t_all_is_true<T, C, Cs...> : t_and<C<T>, Cs<T>...> { };

  template<typename T, template<typename> class C>
  struct t_all_is_true<T, C> : C<T> { };

  template<typename T> struct t_all_is_true<T>;

  template<typename T, template<typename> class... Cs>
  using t_all_is_not_true = t_not<t_all_is_true<T, Cs...>>;

  template<typename T, template<typename> class... Cs>
  using t_if_all_is_true = t_if<t_all_is_true<T, Cs...>>;

  template<typename T, template<typename> class... Cs>
  using t_if_all_is_not_true = t_if<t_all_is_not_true<T, Cs...>>;

///////////////////////////////////////////////////////////////////////////////

  template<typename, template<typename> class...> struct t_none_is_true;

  template<typename T, template<typename> class C,
                       template<typename> class... Cs>
  struct t_none_is_true<T, C, Cs...> : t_and<t_not<C<T>>, Cs<T>...> { };

  template<typename T, template<typename> class C>
  struct t_none_is_true<T, C> : t_not<C<T>> { };

  template<typename T> struct t_none_is_true<T>;

  template<typename T, template<typename> class... Cs>
  using t_if_none_is_true = t_if<t_none_is_true<T, Cs...>>;

///////////////////////////////////////////////////////////////////////////////

  template<typename, template<typename> class...> struct t_least_one_is_true;

  template<typename T, template<typename> class C,
                       template<typename> class... Cs>
  struct t_least_one_is_true<T, C, Cs...> : t_or<C<T>, Cs<T>...> { };

  template<typename T, template<typename> class C>
  struct t_least_one_is_true<T, C> : C<T> { };

  template<typename T> struct t_least_one_is_true<T>;

  template<typename T, template<typename> class... Cs>
  using t_if_least_one_is_true = t_if<t_least_one_is_true<T, Cs...>>;

///////////////////////////////////////////////////////////////////////////////

  template<typename T, typename U> struct t_is_same       : t_FALSE_  {  };
  template<typename T>             struct t_is_same<T, T> : t_TRUE_   { };

  template<typename T, typename T1>
  using t_is_not_same = t_not<t_is_same<T, T1>>;

  template<typename T, typename T1>
  using t_if_same = t_if<t_is_same<T, T1>>;

  template<typename T, typename T1>
  using t_if_not_same = t_if<t_is_not_same<T, T1>>;

///////////////////////////////////////////////////////////////////////////////

  namespace impl_ {
    template<typename T, typename>
    struct t_is_true_  : t_FALSE_ { };

    template<typename T>
    struct t_is_true_<T, t_test_well_formed<typename T::t_result>>
      : t_is_same<typename T::t_result, t_true> { };

    template<typename T, typename>
    struct t_is_false_  : t_FALSE_ { };

    template<typename T>
    struct t_is_false_<T, t_test_well_formed<typename T::t_result>>
      : t_is_same<typename T::t_result, t_false> { };
  }

  template<typename T> struct t_is_true  : impl_::t_is_true_ <T, t_ok> { };
  template<typename T> struct t_is_false : impl_::t_is_false_<T, t_ok> { };

///////////////////////////////////////////////////////////////////////////////

  template<typename T>
  using t_is_bool_result = t_least_one_is_true<T, t_is_true, t_is_false>;

  template<typename T> using t_is_not_bool_result = t_not<t_is_bool_result<T>>;
  template<typename T> using t_if_bool_result     = t_if<t_is_bool_result<T>>;
  template<typename T> using t_if_not_bool_result = t_if<t_is_not_bool_result<T>>;

///////////////////////////////////////////////////////////////////////////////

  template<typename, typename...> struct t_is_one_of : t_FALSE_ { };

  template<typename T, typename T1, typename... Ts>
  struct t_is_one_of<T, T1, Ts...>
    : t_if_then_else<t_is_one_of<T, T1>, t_TRUE_,
                     t_is_one_of<T, Ts...>>::t_result { };

  template<typename T, typename T1>
  struct t_is_one_of<T, T1> : t_is_same<T, T1> { };

  template<typename T, typename... Ts>
  using t_is_not_one_of = t_not<t_is_one_of<T, Ts...>>;

  template<typename T, typename... Ts>
  using t_if_one_of = t_if<t_is_one_of<T, Ts...>>;

  template<typename T, typename... Ts>
  using t_if_not_one_of = t_if<t_is_not_one_of<T, Ts...>>;

///////////////////////////////////////////////////////////////////////////////

  // primary type categories
  template<typename> struct t_is_bool         : t_FALSE_ { };
  template<>         struct t_is_bool<t_bool> : t_TRUE_  { };

  template<typename T> using t_is_not_bool = t_not<t_is_bool<T>>;
  template<typename T> using t_if_bool     = t_if<t_is_bool<T>>;
  template<typename T> using t_if_not_bool = t_if<t_is_not_bool<T>>;

///////////////////////////////////////////////////////////////////////////////

  template<typename> struct t_is_void         : t_FALSE_ { };
  template<>         struct t_is_void<t_void> : t_TRUE_  { };

  template<typename T> using t_is_not_void = t_not<t_is_void<T>>;
  template<typename T> using t_if_void     = t_if<t_is_void<T>>;
  template<typename T> using t_if_not_void = t_if<t_is_not_void<T>>;

///////////////////////////////////////////////////////////////////////////////

  template<typename> struct t_is_nullptr            : t_FALSE_ { };
  template<>         struct t_is_nullptr<t_nullptr> : t_TRUE_  { };

  template<typename T> using t_is_not_nullptr = t_not<t_is_nullptr<T>>;
  template<typename T> using t_if_nullptr     = t_if<t_is_nullptr<T>>;
  template<typename T> using t_if_not_nullptr = t_if<t_is_not_nullptr<T>>;

///////////////////////////////////////////////////////////////////////////////

  template<typename T> struct t_is_enum : t_bool_result<__is_enum(T)> { };

  template<typename T> using t_is_not_enum = t_not<t_is_enum<T>>;
  template<typename T> using t_if_enum     = t_if<t_is_enum<T>>;
  template<typename T> using t_if_not_enum = t_if<t_is_not_enum<T>>;

///////////////////////////////////////////////////////////////////////////////

  template<typename>             struct t_is_bounded_array       : t_FALSE_ { };
  template<typename T, t_size N> struct t_is_bounded_array<T[N]> : t_TRUE_  { };

  template<typename T> using t_is_not_bounded_array = t_not<t_is_bounded_array<T>>;
  template<typename T> using t_if_bounded_array     = t_if<t_is_bounded_array<T>>;
  template<typename T> using t_if_not_bounded_array = t_if<t_is_not_bounded_array<T>>;

///////////////////////////////////////////////////////////////////////////////

  template<typename>   struct t_is_unbounded_array      : t_FALSE_ { };
  template<typename T> struct t_is_unbounded_array<T[]> : t_TRUE_  { };

  template<typename T>
  using t_is_not_unbounded_array = t_not<t_is_unbounded_array<T>>;
  template<typename T>
  using t_if_unbounded_array     = t_if<t_is_unbounded_array<T>>;
  template<typename T>
  using t_if_not_unbounded_array = t_if<t_is_not_unbounded_array<T>>;

///////////////////////////////////////////////////////////////////////////////

  template<typename T>
  struct t_is_array : t_least_one_is_true<T, t_is_bounded_array,
                                             t_is_unbounded_array> { };

  template<typename T> using t_is_not_array = t_not<t_is_array<T>>;
  template<typename T> using t_if_array     = t_if<t_is_array<T>>;
  template<typename T> using t_if_not_array = t_if<t_is_not_array<T>>;

///////////////////////////////////////////////////////////////////////////////

  template<typename>   struct t_is_ptr     : t_FALSE_ { };
  template<typename T> struct t_is_ptr<T*> : t_TRUE_  { };

  template<typename T> using t_is_not_ptr = t_not<t_is_ptr<T>>;
  template<typename T> using t_if_ptr     = t_if<t_is_ptr<T>>;
  template<typename T> using t_if_not_ptr = t_if<t_is_not_ptr<T>>;

///////////////////////////////////////////////////////////////////////////////

  template<typename>   struct t_is_ref      : t_FALSE_ { };
  template<typename T> struct t_is_ref<T&>  : t_TRUE_  { };
  template<typename T> struct t_is_ref<T&&> : t_TRUE_  { };

  template<typename T> using t_is_not_ref = t_not<t_is_ref<T>>;
  template<typename T> using t_if_ref     = t_if<t_is_ref<T>>;
  template<typename T> using t_if_not_ref = t_if<t_is_not_ref<T>>;

///////////////////////////////////////////////////////////////////////////////

  template<typename>   struct t_is_lvalue_ref      : t_FALSE_ { };
  template<typename T> struct t_is_lvalue_ref<T&&> : t_FALSE_ { };
  template<typename T> struct t_is_lvalue_ref<T&>  : t_TRUE_  { };

  template<typename T> using t_is_not_lvalue_ref = t_not<t_is_lvalue_ref<T>>;
  template<typename T> using t_if_lvalue_ref     = t_if<t_is_lvalue_ref<T>>;
  template<typename T> using t_if_not_lvalue_ref = t_if<t_is_not_lvalue_ref<T>>;

///////////////////////////////////////////////////////////////////////////////

  template<typename>   struct t_is_rvalue_ref      : t_FALSE_ { };
  template<typename T> struct t_is_rvalue_ref<T&&> : t_TRUE_  { };

  template<typename T> using t_is_not_rvalue_ref = t_not<t_is_rvalue_ref<T>>;
  template<typename T> using t_if_rvalue_ref     = t_if<t_is_rvalue_ref<T>>;
  template<typename T> using t_if_not_rvalue_ref = t_if<t_is_not_rvalue_ref<T>>;

///////////////////////////////////////////////////////////////////////////////

  template<typename T>
  struct t_is_free_func : t_FALSE_ { };

  template<typename R, typename... As>
  struct t_is_free_func<R(As...)>      : t_TRUE_ { };

  template<typename R, typename... As>
  struct t_is_free_func<R(As..., ...)> : t_TRUE_ { };

  #if __cplusplus >= 201703L
  template<typename R, typename... As>
  struct t_is_free_func<R(As...)      noexcept> : t_TRUE_ { };

  template<typename R, typename... As>
  struct t_is_free_func<R(As..., ...) noexcept> : t_TRUE_ { };
  #endif

  template<typename T> using t_is_not_free_func = t_not<t_is_free_func<T>>;
  template<typename T> using t_if_free_func     = t_if<t_is_free_func<T>>;
  template<typename T> using t_if_not_free_func = t_if<t_is_not_free_func<T>>;

///////////////////////////////////////////////////////////////////////////////

namespace impl_ {

  template<typename, typename>
   struct t_is_func_ : t_FALSE_ { };

  template<typename T, typename R, typename... As>
  struct t_is_func_<T, t_test_well_formed<t_if_same<T, R(As...)>>> : t_TRUE_ { };

  /*
  struct t_is_func_<T, t_if_one_of<T,
                                   R(As...),
                                   R(As...) &,
                                   R(As...) &&,
                                   R(As...)    noexcept,
                                   R(As...) &  noexcept,
                                   R(As...) && noexcept,
                                   R(As...) const,
                                   R(As...) const &,
                                   R(As...) const &&,
                                   R(As...) const    noexcept,
                                   R(As...) const &  noexcept,
                                   R(As...) const && noexcept,
                                   R(As...) volatile,
                                   R(As...) volatile &,
                                   R(As...) volatile &&,
                                   R(As...) volatile    noexcept,
                                   R(As...) volatile &  noexcept,
                                   R(As...) volatile && noexcept,
                                   R(As...) const volatile,
                                   R(As...) const volatile &,
                                   R(As...) const volatile &&,
                                   R(As...) const volatile    noexcept,
                                   R(As...) const volatile &  noexcept,
                                   R(As...) const volatile && noexcept,
                                   R(As......),
                                   R(As......) &,
                                   R(As......) &&,
                                   R(As......)    noexcept,
                                   R(As......) &  noexcept,
                                   R(As......) && noexcept,
                                   R(As......) const,
                                   R(As......) const &,
                                   R(As......) const &&,
                                   R(As......) const    noexcept,
                                   R(As......) const &  noexcept,
                                   R(As......) const && noexcept,
                                   R(As......) volatile,
                                   R(As......) volatile &,
                                   R(As......) volatile &&,
                                   R(As......) volatile    noexcept,
                                   R(As......) volatile &  noexcept,
                                   R(As......) volatile && noexcept,
                                   R(As......) const volatile,
                                   R(As......) const volatile &,
                                   R(As......) const volatile &&,
                                   R(As......) const volatile    noexcept,
                                   R(As......) const volatile &  noexcept,
                                   R(As......) const volatile && noexcept>
                   > : t_TRUE_ { };
  */
}

template<typename T> struct t_is_func : impl_::t_is_func_<T, T> { };

///////////////////////////////////////////////////////////////////////////////

template<typename T>
struct t_is_object : t_none_is_true<T, t_is_func, t_is_ref, t_is_void> { };

template<typename T> using t_is_not_object = t_not<t_is_object<T>>;
template<typename T> using t_if_object     = t_if<t_is_object<T>>;
template<typename T> using t_if_not_object = t_if<t_is_not_object<T>>;

///////////////////////////////////////////////////////////////////////////////

template<typename T>
struct t_is_ref_able : t_least_one_is_true<T, t_is_object, t_is_ref> { };

template<typename R, typename... As>
struct t_is_ref_able<R(As...)>      : t_TRUE_ { };

template<typename R, typename... As>
struct t_is_ref_able<R(As..., ...)> : t_TRUE_ { };

#if __cplusplus >= 201703L
template<typename R, typename... As>
struct t_is_ref_able<R(As...)      noexcept> : t_TRUE_ { };

template<typename R, typename... As>
struct t_is_ref_able<R(As..., ...) noexcept> : t_TRUE_ { };
#endif

template<typename T> using t_is_not_ref_able = t_not<t_is_ref_able<T>>;
template<typename T> using t_if_ref_able     = t_if<t_is_ref_able<T>>;
template<typename T> using t_if_not_ref_able = t_if<t_is_not_ref_able<T>>;

///////////////////////////////////////////////////////////////////////////////

  namespace impl_ {
    template<typename T, typename = typename t_is_ref_able<T>::t_result>
    struct t_add_lvalue_ref_            { using t_result = T; };

    template<typename T>
    struct t_add_lvalue_ref_<T, t_true> { using t_result = T&; };

    template<typename T, typename = typename t_is_ref_able<T>::t_result>
    struct t_add_rvalue_ref_            { using t_result = T; };

    template<typename T>
    struct t_add_rvalue_ref_<T, t_true> { using t_result = T&&; };
  }

  template<typename T>
  using t_add_lvalue_ref = impl_::t_add_lvalue_ref_<T>;

  template<typename T>
  using t_add_rvalue_ref = impl_::t_add_rvalue_ref_<T>;

///////////////////////////////////////////////////////////////////////////////

  template<typename T>
  typename t_add_rvalue_ref<T>::t_result declval() noexcept;

///////////////////////////////////////////////////////////////////////////////

  template<typename T> struct t_remove_ref      : t_result_is<T> { };
  template<typename T> struct t_remove_ref<T&>  : t_result_is<T> { };
  template<typename T> struct t_remove_ref<T&&> : t_result_is<T> { };

  template<typename T> struct t_remove_const          : t_result_is<T> { };
  template<typename T> struct t_remove_const<const T> : t_result_is<T> { };

  template<typename T> struct t_remove_volatile             : t_result_is<T> { };
  template<typename T> struct t_remove_volatile<volatile T> : t_result_is<T> { };

  template<typename T> struct t_remove_cv
    : t_remove_const<typename t_remove_volatile<T>::t_result> { };

  template<typename T> struct t_add_const    : t_result_is<const T>    { };
  template<typename T> struct t_add_volatile : t_result_is<volatile T> { };
  template<typename T> struct t_add_cv :
    t_add_const<typename t_add_volatile<T>::t_result> { };

///////////////////////////////////////////////////////////////////////////////

  namespace impl_ {
    template<typename T>
    struct t_is_member_ptr_         : t_FALSE_ { };

    template<typename T, typename U>
    struct t_is_member_ptr_<T U::*> : t_TRUE_  { };
  }

  template<typename T>
  struct t_is_member_ptr
    : impl_::t_is_member_ptr_<typename t_remove_cv<T>::t_result> { };

  template<typename T> using t_is_not_member_ptr = t_not<t_is_member_ptr<T>>;
  template<typename T> using t_if_member_ptr     = t_if<t_is_member_ptr<T>>;
  template<typename T> using t_if_not_member_ptr = t_if<t_is_not_member_ptr<T>>;

///////////////////////////////////////////////////////////////////////////////

  template<typename T>
  struct t_is_integral : t_is_one_of<T, bool,
                                        char,      unsigned char, signed char,
                                        short,     unsigned short,
                                        int,       unsigned int,
                                        long,      unsigned long,
                                        long long, unsigned long long> { };

  template<typename T> using t_is_not_integral = t_not<t_is_integral<T>>;
  template<typename T> using t_if_integral     = t_if<t_is_integral<T>>;
  template<typename T> using t_if_not_integral = t_if<t_is_not_integral<T>>;

///////////////////////////////////////////////////////////////////////////////

  template<typename T>
  struct t_is_unsigned : t_is_one_of<T, unsigned char,
                                        unsigned short,
                                        unsigned int,
                                        unsigned long,
                                        unsigned long long> { };

  template<typename T> using t_is_not_unsigned = t_not<t_is_unsigned<T>>;
  template<typename T> using t_if_unsigned     = t_if<t_is_unsigned<T>>;
  template<typename T> using t_if_not_unsigned = t_if<t_is_not_unsigned<T>>;

///////////////////////////////////////////////////////////////////////////////

template<typename T>
struct t_is_precision : t_is_one_of<T, double, float, long double> { };

  template<typename T> using t_is_not_precision = t_not<t_is_precision<T>>;
  template<typename T> using t_if_precision     = t_if<t_is_precision<T>>;
  template<typename T> using t_if_not_precision = t_if<t_is_not_precision<T>>;

///////////////////////////////////////////////////////////////////////////////

  // composite type categories
  template<typename T>
  struct t_is_arithmetic : t_least_one_is_true<T, t_is_integral,
                                                  t_is_precision> { };

  template<typename T> using t_is_not_arithmetic = t_not<t_is_arithmetic<T>>;
  template<typename T> using t_if_arithmetic     = t_if<t_is_arithmetic<T>>;
  template<typename T> using t_if_not_arithmetic = t_if<t_is_not_arithmetic<T>>;

///////////////////////////////////////////////////////////////////////////////

  template<typename T>
  struct t_is_fundamental : t_least_one_is_true<T, t_is_arithmetic,
                                                   t_is_void,
                                                   t_is_nullptr> { };

  template<typename T> using t_is_not_fundamental = t_not<t_is_fundamental<T>>;
  template<typename T> using t_if_fundamental     = t_if<t_is_fundamental<T>>;
  template<typename T> using t_if_not_fundamental = t_if<t_is_not_fundamental<T>>;

///////////////////////////////////////////////////////////////////////////////

  template<typename T>
  struct t_is_scalar : t_least_one_is_true<T, t_is_arithmetic,
                                              t_is_enum,
                                              t_is_ptr,
                                              t_is_member_ptr,
                                              t_is_nullptr> { };

  template<typename T> using t_is_not_scalar = t_not<t_is_scalar<T>>;
  template<typename T> using t_if_scalar     = t_if<t_is_scalar<T>>;
  template<typename T> using t_if_not_scalar = t_if<t_is_not_scalar<T>>;

///////////////////////////////////////////////////////////////////////////////

  template<typename T>
  struct t_is_compound : t_none_is_true<T, t_is_fundamental> { };

  template<typename T> using t_is_not_compound = t_not<t_is_compound<T>>;
  template<typename T> using t_if_compound     = t_if<t_is_compound<T>>;
  template<typename T> using t_if_not_compound = t_if<t_is_not_compound<T>>;

///////////////////////////////////////////////////////////////////////////////

  template<typename T> struct t_is_const          : t_FALSE_ { };
  template<typename T> struct t_is_const<const T> : t_TRUE_  { };

  template<typename T> using t_is_not_const = t_not<t_is_const<T>>;
  template<typename T> using t_if_const     = t_if<t_is_const<T>>;
  template<typename T> using t_if_not_const = t_if<t_is_not_const<T>>;

///////////////////////////////////////////////////////////////////////////////

  template<typename T> struct t_is_volatile             : t_FALSE_ { };
  template<typename T> struct t_is_volatile<volatile T> : t_TRUE_  { };

  template<typename T> using t_is_not_volatile = t_not<t_is_volatile<T>>;
  template<typename T> using t_if_volatile     = t_if<t_is_volatile<T>>;
  template<typename T> using t_if_not_volatile = t_if<t_is_not_volatile<T>>;

///////////////////////////////////////////////////////////////////////////////

  template<typename T> struct t_is_signed : t_all_is_true<T, t_is_arithmetic,
                                                             t_is_not_unsigned,
                                                             t_is_not_bool> { };

  template<typename T> using t_is_not_signed = t_not<t_is_signed<T>>;
  template<typename T> using t_if_signed     = t_if<t_is_signed<T>>;
  template<typename T> using t_if_not_signed = t_if<t_is_not_signed<T>>;

///////////////////////////////////////////////////////////////////////////////

  template<typename T, typename... Ts>
  struct t_is_unique : t_if_then_else<t_is_one_of<T, Ts...>, t_FALSE_,
                                      t_is_unique<Ts...>> { };

  template<typename T>    struct t_is_unique<T> : t_TRUE_ { };
  template<typename... Ts> using t_if_unique = t_if<t_is_unique<Ts...>>;

  template<typename... Ts> using t_is_not_unique = t_not<t_is_unique<Ts...>>;
  template<typename... Ts> using t_if_not_unique = t_if<t_is_not_unique<Ts...>>;

///////////////////////////////////////////////////////////////////////////////

  namespace impl_ {
    template<typename...> struct t_mk_unique_;

    template<typename... Ts, typename U, typename... Us>
    struct t_mk_unique_<t_pack<Ts...>, t_pack<U, Us...>>
      : t_if_then_else<
          t_is_not_one_of<U, Ts...>,
          typename t_mk_unique_<t_pack<Ts..., U>, t_pack<Us...>>::t_result,
          typename t_mk_unique_<t_pack<Ts...>, t_pack<Us...>>::t_result> { };

    template<typename... Ts>
    struct t_mk_unique_<t_pack<Ts...>, t_pack<>>
      : t_result_is<t_pack<Ts...>> { };
  }

  template<typename T, typename... Ts>
  struct t_mk_unique : impl_::t_mk_unique_<t_pack<>, t_pack<T, Ts...>> { };

///////////////////////////////////////////////////////////////////////////////

  namespace impl_ {
    template<typename... Ts> struct t_flatten_tree_;

    template<typename... Ts, typename... Hs, typename... Us>
    struct t_flatten_tree_<t_pack<Ts...>, t_pack<t_pack<Hs...>, Us...>>
      : t_flatten_tree_<t_pack<Ts...>, t_pack<Hs..., Us...>> {
    };

    template<typename... Ts, typename U, typename... Us>
    struct t_flatten_tree_<t_pack<Ts...>, t_pack<U, Us...>>
      : t_if_then_else<
          t_is_one_of<U, Ts...>,
          typename t_flatten_tree_<t_pack<Ts...>,    t_pack<Us...>>::t_result,
          typename t_flatten_tree_<t_pack<Ts..., U>, t_pack<Us...>>::t_result> { };

    template<typename... Ts>
    struct t_flatten_tree_<t_pack<Ts...>, t_pack<>>
      : t_result_is<t_pack<Ts...>> { };
  }

  template<typename... Ts>
  struct t_flatten_tree : impl_::t_flatten_tree_<t_pack<>, t_pack<Ts...>> { };

///////////////////////////////////////////////////////////////////////////////

  namespace impl_ {
    template<t_size, typename...> struct t_is_subset_of_;

    template<t_size N, typename T, typename... Ts, typename... Us>
    struct t_is_subset_of_<N, t_pack<T, Ts...>, t_pack<Us...>>
      : t_if_then_else<t_is_one_of<T, Us...>,
                       t_is_subset_of_<N, t_pack<Ts...>, t_pack<Us...>>,
                       t_FALSE_>::t_result { };

    template<t_size N, typename... Us>
    struct t_is_subset_of_<N, t_pack<>, t_pack<Us...>> : t_TRUE_  { };

    template<t_size N, typename... Ts>
    struct t_is_subset_of_<N, t_pack<Ts...>, t_pack<>> : t_FALSE_ { };

    template<typename... Ts, typename... Us>
    struct t_is_subset_of_<0, t_pack<Ts...>, t_pack<Us...>>;
  }

  template<typename...> struct t_is_subset_of;

  template<typename... Ts, typename... Us>
  struct t_is_subset_of<t_pack<Ts...>, t_pack<Us...>> :
    impl_::t_is_subset_of_<sizeof...(Ts), t_pack<Ts...>, t_pack<Us...>> { };

  template<typename... Ts> using t_is_not_subset_of = t_not<t_is_subset_of<Ts...>>;
  template<typename... Ts> using t_if_subset_of     = t_if<t_is_subset_of<Ts...>>;
  template<typename... Ts> using t_if_not_subset_of = t_if<t_is_not_subset_of<Ts...>>;

///////////////////////////////////////////////////////////////////////////////
}
}

#endif
