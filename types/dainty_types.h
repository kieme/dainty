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

#ifndef _DAINTY_TYPES_H_
#define _DAINTY_TYPES_H_

#define DAINTY_INTERNAL_INCLUDE_C_STDINT
#include "dainty_internal_.h"

namespace dainty
{
namespace types
{
  /////////////////////////////////////////////////////////////////////////////

  template<typename T>
  struct t_prefix {
    using T_  = const T;
    using t_  = T;
    using p_  = T*;
    using P_  = const T*;
    using r_  = T&;
    using R_  = const T&;
    using x_  = T&&;
    using X_  = const T&&;
    using q_  = T**;
    using Q_  = const T* const *;

    t_prefix() = delete;
  };

  template<typename T>
  struct t_prefix<const T> {
    using T_  = const T;
    using P_  = const T*;
    using R_  = const T&;
    using X_  = const T&&;
    using Q_  = const T* const *;

    t_prefix() = delete;
  };

  template<>
  struct t_prefix<void> {
    using t_  = void;
    using T_  = const void;
    using p_  = void*;
    using P_  = const void*;
    using q_  = void**;
    using Q_  = const void* const *;

    t_prefix() = delete;
  };

  template<>
  struct t_prefix<const void> {
    using T_  = const void;
    using P_  = const void*;
    using Q_  = const void* const *;

    t_prefix() = delete;
  };

  //template<typename T> struct t_prefix<T*>;
  template<typename T> struct t_prefix<T**>;
  template<typename T> struct t_prefix<T&>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T> using T_of = typename t_prefix<T>::T_;
  template<typename T> using t_of = typename t_prefix<T>::t_;
  template<typename T> using p_of = typename t_prefix<T>::p_;
  template<typename T> using P_of = typename t_prefix<T>::P_;
  template<typename T> using r_of = typename t_prefix<T>::r_;
  template<typename T> using R_of = typename t_prefix<T>::R_;
  template<typename T> using x_of = typename t_prefix<T>::x_;
  template<typename T> using X_of = typename t_prefix<T>::X_;
  template<typename T> using q_of = typename t_prefix<T>::q_;
  template<typename T> using Q_of = typename t_prefix<T>::Q_;

  /////////////////////////////////////////////////////////////////////////////

  using t_bool = t_of<bool>;
  using T_bool = T_of<t_bool>;
  using r_bool = r_of<t_bool>;
  using p_bool = p_of<t_bool>;
  using P_bool = P_of<t_bool>;

  /////////////////////////////////////////////////////////////////////////////

  using t_char   = t_of<char>;
  using T_char   = T_of<t_char>;
  using r_char   = r_of<t_char>;
  using p_char   = p_of<t_char>;
  using P_char   = P_of<t_char>;

  using t_schar  = t_of<signed char>;
  using T_schar  = T_of<t_schar>;
  using r_schar  = r_of<t_schar>;
  using p_schar  = p_of<t_schar>;
  using P_schar  = P_of<t_schar>;

  using t_uchar  = t_of<unsigned char>;
  using T_uchar  = T_of<t_uchar>;
  using r_uchar  = r_of<t_uchar>;
  using p_uchar  = p_of<t_uchar>;
  using P_uchar  = P_of<t_uchar>;

  using t_int    = t_of<int>;
  using T_int    = T_of<t_int>;
  using r_int    = r_of<t_int>;
  using p_int    = p_of<t_int>;
  using P_int    = P_of<t_int>;

  using t_uint   = t_of<unsigned int>;
  using T_uint   = T_of<t_uint>;
  using r_uint   = r_of<t_uint>;
  using p_uint   = p_of<t_uint>;
  using P_uint   = P_of<t_uint>;

  using t_short  = t_of<short>;
  using T_short  = T_of<t_short>;
  using r_short  = r_of<t_short>;
  using p_short  = p_of<t_short>;
  using P_short  = P_of<t_short>;

  using t_ushort = t_of<unsigned short>;
  using T_ushort = T_of<t_ushort>;
  using r_ushort = r_of<t_ushort>;
  using p_ushort = p_of<t_ushort>;
  using P_ushort = P_of<t_ushort>;

  using t_long   = t_of<long int>;
  using T_long   = T_of<t_long>;
  using r_long   = r_of<t_long>;
  using p_long   = p_of<t_long>;
  using P_long   = P_of<t_long>;

  using t_ulong  = t_of<unsigned long int>;
  using T_ulong  = T_of<t_ulong>;
  using r_ulong  = r_of<t_ulong>;
  using p_ulong  = p_of<t_ulong>;
  using P_ulong  = P_of<t_ulong>;

  using t_llong  = t_of<long long int>;
  using T_llong  = T_of<t_llong>;
  using r_llong  = r_of<t_llong>;
  using p_llong  = p_of<t_llong>;
  using P_llong  = P_of<t_llong>;

  using t_ullong = t_of<unsigned long long int>;
  using T_ullong = T_of<t_ullong>;
  using r_ullong = r_of<t_ullong>;
  using p_ullong = p_of<t_ullong>;
  using P_ullong = P_of<t_ullong>;

  /////////////////////////////////////////////////////////////////////////////

  using t_float   = t_of<float>;
  using T_float   = T_of<t_float>;
  using r_float   = r_of<t_float>;
  using p_float   = p_of<t_float>;
  using P_float   = P_of<t_float>;

  using t_double  = t_of<double>;
  using T_double  = T_of<t_double>;
  using r_double  = r_of<t_double>;
  using p_double  = p_of<t_double>;
  using P_double  = P_of<t_double>;

  using t_ldouble = t_of<long double>;
  using T_ldouble = T_of<t_ldouble>;
  using r_ldouble = r_of<t_ldouble>;
  using p_ldouble = p_of<t_ldouble>;
  using P_ldouble = P_of<t_ldouble>;

  /////////////////////////////////////////////////////////////////////////////

  using t_void = t_of<void>;
  using p_void = p_of<t_void>;
  using P_void = P_of<t_void>;

  /////////////////////////////////////////////////////////////////////////////

  using t_int8   = t_of<std::int8_t>;
  using T_int8   = T_of<t_int8>;
  using r_int8   = r_of<t_int8>;
  using p_int8   = p_of<t_int8>;
  using P_int8   = P_of<t_int8>;

  using t_int16  = t_of<std::int16_t>;
  using T_int16  = T_of<t_int16>;
  using r_int16  = r_of<t_int16>;
  using p_int16  = p_of<t_int16>;
  using P_int16  = P_of<t_int16>;

  using t_int32  = t_of<std::int32_t>;
  using T_int32  = T_of<t_int32>;
  using r_int32  = r_of<t_int32>;
  using p_int32  = p_of<t_int32>;
  using P_int32  = P_of<t_int32>;

  using t_int64  = t_of<std::int64_t>;
  using T_int64  = T_of<t_int64>;
  using r_int64  = r_of<t_int64>;
  using p_int64  = p_of<t_int64>;
  using P_int64  = P_of<t_int64>;

  using t_uint8  = t_of<std::uint8_t>;
  using T_uint8  = T_of<t_uint8>;
  using r_uint8  = r_of<t_uint8>;
  using p_uint8  = p_of<t_uint8>;
  using P_uint8  = P_of<t_uint8>;

  using t_uint16 = t_of<std::uint16_t>;
  using T_uint16 = T_of<t_uint16>;
  using r_uint16 = r_of<t_uint16>;
  using p_uint16 = p_of<t_uint16>;
  using P_uint16 = P_of<t_uint16>;

  using t_uint32 = t_of<std::uint32_t>;
  using T_uint32 = T_of<t_uint32>;
  using r_uint32 = r_of<t_uint32>;
  using p_uint32 = p_of<t_uint32>;
  using P_uint32 = P_of<t_uint32>;

  using t_uint64 = t_of<std::uint64_t>;
  using T_uint64 = T_of<t_uint64>;
  using r_uint64 = r_of<t_uint64>;
  using p_uint64 = p_of<t_uint64>;
  using P_uint64 = P_of<t_uint64>;

  /////////////////////////////////////////////////////////////////////////////

  using t_least8   = t_of<std::int_least8_t>;
  using T_least8   = T_of<t_least8>;
  using r_least8   = r_of<t_least8>;
  using p_least8   = p_of<t_least8>;
  using P_least8   = P_of<t_least8>;

  using t_least16  = t_of<std::int_least16_t>;
  using T_least16  = T_of<t_least16>;
  using r_least16  = r_of<t_least16>;
  using p_least16  = p_of<t_least16>;
  using P_least16  = P_of<t_least16>;

  using t_least32  = t_of<std::int_least32_t>;
  using T_least32  = T_of<t_least32>;
  using r_least32  = r_of<t_least32>;
  using p_least32  = p_of<t_least32>;
  using P_least32  = P_of<t_least32>;

  using t_least64  = t_of<std::int_least64_t>;
  using T_least64  = T_of<t_least64>;
  using r_least64  = r_of<t_least64>;
  using p_least64  = p_of<t_least64>;
  using P_least64  = P_of<t_least64>;

  using t_uleast8  = t_of<std::uint_least8_t>;
  using T_uleast8  = T_of<t_uleast8>;
  using r_uleast8  = r_of<t_uleast8>;
  using p_uleast8  = p_of<t_uleast8>;
  using P_uleast8  = P_of<t_uleast8>;

  using t_uleast16 = t_of<std::uint_least16_t>;
  using T_uleast16 = T_of<t_uleast16>;
  using r_uleast16 = r_of<t_uleast16>;
  using p_uleast16 = p_of<t_uleast16>;
  using P_uleast16 = P_of<t_uleast16>;

  using t_uleast32 = t_of<std::uint_least32_t>;
  using T_uleast32 = T_of<t_uleast32>;
  using r_uleast32 = r_of<t_uleast32>;
  using p_uleast32 = p_of<t_uleast32>;
  using P_uleast32 = P_of<t_uleast32>;

  using t_uleast64 = t_of<std::uint_least64_t>;
  using T_uleast64 = T_of<t_uleast64>;
  using r_uleast64 = r_of<t_uleast64>;
  using p_uleast64 = p_of<t_uleast64>;
  using P_uleast64 = P_of<t_uleast64>;

  /////////////////////////////////////////////////////////////////////////////

  using t_fast8   = t_of<std::int_fast8_t>;
  using T_fast8   = T_of<t_fast8>;
  using r_fast8   = r_of<t_fast8>;
  using p_fast8   = p_of<t_fast8>;
  using P_fast8   = P_of<t_fast8>;

  using t_fast16  = t_of<std::int_fast16_t>;
  using T_fast16  = T_of<t_fast16>;
  using r_fast16  = r_of<t_fast16>;
  using p_fast16  = p_of<t_fast16>;
  using P_fast16  = P_of<t_fast16>;

  using t_fast32  = t_of<std::int_fast32_t>;
  using T_fast32  = T_of<t_fast32>;
  using r_fast32  = r_of<t_fast32>;
  using p_fast32  = p_of<t_fast32>;
  using P_fast32  = P_of<t_fast32>;

  using t_fast64  = t_of<std::int_fast64_t>;
  using T_fast64  = T_of<t_fast64>;
  using r_fast64  = r_of<t_fast64>;
  using p_fast64  = p_of<t_fast64>;
  using P_fast64  = P_of<t_fast64>;

  using t_ufast8  = t_of<std::uint_fast8_t>;
  using T_ufast8  = T_of<t_ufast8>;
  using r_ufast8  = r_of<t_ufast8>;
  using p_ufast8  = p_of<t_ufast8>;
  using P_ufast8  = P_of<t_ufast8>;

  using t_ufast16 = t_of<std::uint_fast16_t>;
  using T_ufast16 = T_of<t_ufast16>;
  using r_ufast16 = r_of<t_ufast16>;
  using p_ufast16 = p_of<t_ufast16>;
  using P_ufast16 = P_of<t_ufast16>;

  using t_ufast32 = t_of<std::uint_fast32_t>;
  using T_ufast32 = T_of<t_ufast32>;
  using r_ufast32 = r_of<t_ufast32>;
  using p_ufast32 = p_of<t_ufast32>;
  using P_ufast32 = P_of<t_ufast32>;

  using t_ufast64 = t_of<std::uint_fast64_t>;
  using T_ufast64 = T_of<t_ufast64>;
  using r_ufast64 = r_of<t_ufast64>;
  using p_ufast64 = p_of<t_ufast64>;
  using P_ufast64 = P_of<t_ufast64>;

  /////////////////////////////////////////////////////////////////////////////

  using t_uintptr = t_of<std::uintptr_t>;
  using T_uintptr = T_of<t_uintptr>;
  using r_uintptr = r_of<t_uintptr>;
  using p_uintptr = p_of<t_uintptr>;
  using P_uintptr = P_of<t_uintptr>;

  /////////////////////////////////////////////////////////////////////////////

  using t_nullptr = decltype(nullptr);

  /////////////////////////////////////////////////////////////////////////////

  using t_truth_ = t_of<t_bool>;
  using T_truth_ = T_of<t_truth_>;
  using r_truth_ = r_of<t_truth_>;
  using R_truth_ = R_of<t_truth_>;
  using p_truth_ = p_of<t_truth_>;
  using P_truth_ = P_of<t_truth_>;

  constexpr t_truth_ TRUTHY_ = true;
  constexpr t_truth_ FALSEY_ = false;

  /////////////////////////////////////////////////////////////////////////////

  using t_validity_ = t_of<t_bool>;
  using T_validity_ = T_of<t_validity_>;
  using r_validity_ = r_of<t_validity_>;
  using p_validity_ = p_of<t_validity_>;
  using P_validity_ = P_of<t_validity_>;

  constexpr t_validity_   VALID_ = true;
  constexpr t_validity_ INVALID_ = false;

  /////////////////////////////////////////////////////////////////////////////

  /* _i_ -> native signed integer */
  using t_i1_ = t_of<t_int8>;
  using T_i1_ = T_of<t_i1_>;
  using r_i1_ = r_of<t_i1_>;
  using R_i1_ = R_of<t_i1_>;
  using p_i1_ = p_of<t_i1_>;
  using P_i1_ = P_of<t_i1_>;

  using t_i2_ = t_of<t_int16>;
  using T_i2_ = T_of<t_i2_>;
  using r_i2_ = r_of<t_i2_>;
  using R_i2_ = R_of<t_i2_>;
  using p_i2_ = p_of<t_i2_>;
  using P_i2_ = P_of<t_i2_>;

  using t_i4_ = t_of<t_int32>;
  using T_i4_ = T_of<t_i4_>;
  using r_i4_ = r_of<t_i4_>;
  using R_i4_ = R_of<t_i4_>;
  using p_i4_ = p_of<t_i4_>;
  using P_i4_ = P_of<t_i4_>;

  using t_i8_ = t_of<t_int64>;
  using T_i8_ = T_of<t_i8_>;
  using r_i8_ = r_of<t_i8_>;
  using R_i8_ = R_of<t_i8_>;
  using p_i8_ = p_of<t_i8_>;
  using P_i8_ = P_of<t_i8_>;

  using t_i_  = t_of<t_i8_>;
  using T_i_  = T_of<t_i_>;
  using r_i_  = r_of<t_i_>;
  using R_i_  = R_of<t_i_>;
  using p_i_  = p_of<t_i_>;
  using P_i_  = P_of<t_i_>;

  /////////////////////////////////////////////////////////////////////////////

  /* _u_ -> native unsigned integer */
  using t_u1_ = t_of<t_uint8>;
  using T_u1_ = T_of<t_u1_>;
  using r_u1_ = r_of<t_u1_>;
  using R_u1_ = R_of<t_u1_>;
  using p_u1_ = p_of<t_u1_>;
  using P_u1_ = P_of<t_u1_>;

  using t_u2_ = t_of<t_uint16>;
  using T_u2_ = T_of<t_u2_>;
  using r_u2_ = r_of<t_u2_>;
  using R_u2_ = R_of<t_u2_>;
  using p_u2_ = p_of<t_u2_>;
  using P_u2_ = P_of<t_u2_>;

  using t_u4_ = t_of<t_uint32>;
  using T_u4_ = T_of<t_u4_>;
  using r_u4_ = r_of<t_u4_>;
  using R_u4_ = R_of<t_u4_>;
  using p_u4_ = p_of<t_u4_>;
  using P_u4_ = P_of<t_u4_>;

  using t_u8_ = t_of<t_uint64>;
  using T_u8_ = T_of<t_u8_>;
  using r_u8_ = r_of<t_u8_>;
  using R_u8_ = R_of<t_u8_>;
  using p_u8_ = p_of<t_u8_>;
  using P_u8_ = P_of<t_u8_>;

  using t_u_  = t_of<t_u8_>;
  using T_u_  = T_of<t_u_>;
  using r_u_  = r_of<t_u_>;
  using R_u_  = R_of<t_u_>;
  using p_u_  = p_of<t_u_>;
  using P_u_  = P_of<t_u_>;

  /////////////////////////////////////////////////////////////////////////////

  /* _h_ -> unsigned hexadecimal integer */
  using t_h_    = t_of<t_u_>;
  using T_h_    = T_of<t_h_>;
  using r_h_    = r_of<t_h_>;
  using R_h_    = R_of<t_h_>;
  using p_h_    = p_of<t_h_>;
  using P_h_    = P_of<t_h_>;

  using t_u1_h_ = t_of<t_u1_>;
  using T_u1_h_ = T_of<t_u1_h_>;
  using r_u1_h_ = r_of<t_u1_h_>;
  using R_u1_h_ = R_of<t_u1_h_>;
  using p_u1_h_ = p_of<t_u1_h_>;
  using P_u1_h_ = P_of<t_u1_h_>;

  using t_u2_h_ = t_of<t_u2_>;
  using T_u2_h_ = T_of<t_u2_h_>;
  using r_u2_h_ = r_of<t_u2_h_>;
  using R_u2_h_ = R_of<t_u2_h_>;
  using p_u2_h_ = p_of<t_u2_h_>;
  using P_u2_h_ = P_of<t_u2_h_>;

  using t_u4_h_ = t_of<t_u4_>;
  using T_u4_h_ = T_of<t_u4_h_>;
  using r_u4_h_ = r_of<t_u4_h_>;
  using R_u4_h_ = R_of<t_u4_h_>;
  using p_u4_h_ = p_of<t_u4_h_>;
  using P_u4_h_ = P_of<t_u4_h_>;

  using t_u8_h_ = t_of<t_u8_>;
  using T_u8_h_ = T_of<t_u8_h_>;
  using r_u8_h_ = r_of<t_u8_h_>;
  using R_u8_h_ = R_of<t_u8_h_>;
  using p_u8_h_ = p_of<t_u8_h_>;
  using P_u8_h_ = P_of<t_u8_h_>;

  /////////////////////////////////////////////////////////////////////////////

  /* _b_ -> unsigned binary integer */
  using t_b_    = t_of<t_u_>;
  using T_b_    = T_of<t_b_>;
  using r_b_    = r_of<t_b_>;
  using R_b_    = R_of<t_b_>;
  using p_b_    = p_of<t_b_>;
  using P_b_    = P_of<t_b_>;

  using t_u1_b_ = t_of<t_u1_>;
  using T_u1_b_ = T_of<t_u1_b_>;
  using r_u1_b_ = r_of<t_u1_b_>;
  using R_u1_b_ = R_of<t_u1_b_>;
  using p_u1_b_ = p_of<t_u1_b_>;
  using P_u1_b_ = P_of<t_u1_b_>;

  using t_u2_b_ = t_of<t_u2_>;
  using T_u2_b_ = T_of<t_u2_b_>;
  using r_u2_b_ = r_of<t_u2_b_>;
  using R_u2_b_ = R_of<t_u2_b_>;
  using p_u2_b_ = p_of<t_u2_b_>;
  using P_u2_b_ = P_of<t_u2_b_>;

  using t_u4_b_ = t_of<t_u4_>;
  using T_u4_b_ = T_of<t_u4_b_>;
  using r_u4_b_ = r_of<t_u4_b_>;
  using R_u4_b_ = R_of<t_u4_b_>;
  using p_u4_b_ = p_of<t_u4_b_>;
  using P_u4_b_ = P_of<t_u4_b_>;

  using t_u8_b_ = t_of<t_u8_>;
  using T_u8_b_ = T_of<t_u8_b_>;
  using r_u8_b_ = r_of<t_u8_b_>;
  using R_u8_b_ = R_of<t_u8_b_>;
  using p_u8_b_ = p_of<t_u8_b_>;
  using P_u8_b_ = P_of<t_u8_b_>;

  /////////////////////////////////////////////////////////////////////////////

  /* _id_ -> id integer */
  using t_id_    = t_of<t_i_>;
  using T_id_    = T_of<t_id_>;
  using r_id_    = r_of<t_id_>;
  using R_id_    = R_of<t_id_>;
  using p_id_    = p_of<t_id_>;
  using P_id_    = P_of<t_id_>;

  using t_i_id_  = t_of<t_i_>;
  using T_i_id_  = T_of<t_i_id_>;
  using r_i_id_  = r_of<t_i_id_>;
  using R_i_id_  = R_of<t_i_id_>;
  using p_i_id_  = p_of<t_i_id_>;
  using P_i_id_  = P_of<t_i_id_>;

  using t_u_id_  = t_of<t_u_>;
  using T_u_id_  = T_of<t_u_id_>;
  using r_u_id_  = r_of<t_u_id_>;
  using R_u_id_  = R_of<t_u_id_>;
  using p_u_id_  = p_of<t_u_id_>;
  using P_u_id_  = P_of<t_u_id_>;

  using t_i1_id_ = t_of<t_i1_>;
  using T_i1_id_ = T_of<t_i1_id_>;
  using r_i1_id_ = r_of<t_i1_id_>;
  using R_i1_id_ = R_of<t_i1_id_>;
  using p_i1_id_ = p_of<t_i1_id_>;
  using P_i1_id_ = P_of<t_i1_id_>;

  using t_i2_id_ = t_of<t_i2_>;
  using T_i2_id_ = T_of<t_i2_id_>;
  using r_i2_id_ = r_of<t_i2_id_>;
  using R_i2_id_ = R_of<t_i2_id_>;
  using p_i2_id_ = p_of<t_i2_id_>;
  using P_i2_id_ = P_of<t_i2_id_>;

  using t_i4_id_ = t_of<t_i4_>;
  using T_i4_id_ = T_of<t_i4_id_>;
  using r_i4_id_ = r_of<t_i4_id_>;
  using R_i4_id_ = R_of<t_i4_id_>;
  using p_i4_id_ = p_of<t_i4_id_>;
  using P_i4_id_ = P_of<t_i4_id_>;

  using t_i8_id_ = t_of<t_i8_>;
  using T_i8_id_ = T_of<t_i8_id_>;
  using r_i8_id_ = r_of<t_i8_id_>;
  using R_i8_id_ = R_of<t_i8_id_>;
  using p_i8_id_ = p_of<t_i8_id_>;
  using P_i8_id_ = P_of<t_i8_id_>;

  using t_u1_id_ = t_of<t_u1_>;
  using T_u1_id_ = T_of<t_u1_id_>;
  using r_u1_id_ = r_of<t_u1_id_>;
  using R_u1_id_ = R_of<t_u1_id_>;
  using p_u1_id_ = p_of<t_u1_id_>;
  using P_u1_id_ = P_of<t_u1_id_>;

  using t_u2_id_ = t_of<t_u2_>;
  using T_u2_id_ = T_of<t_u2_id_>;
  using r_u2_id_ = r_of<t_u2_id_>;
  using R_u2_id_ = R_of<t_u2_id_>;
  using p_u2_id_ = p_of<t_u2_id_>;
  using P_u2_id_ = P_of<t_u2_id_>;

  using t_u4_id_ = t_of<t_u4_>;
  using T_u4_id_ = T_of<t_u4_id_>;
  using r_u4_id_ = r_of<t_u4_id_>;
  using R_u4_id_ = R_of<t_u4_id_>;
  using p_u4_id_ = p_of<t_u4_id_>;
  using P_u4_id_ = P_of<t_u4_id_>;

  using t_u8_id_ = t_of<t_u8_>;
  using T_u8_id_ = T_of<t_u8_id_>;
  using r_u8_id_ = r_of<t_u8_id_>;
  using R_u8_id_ = R_of<t_u8_id_>;
  using p_u8_id_ = p_of<t_u8_id_>;
  using P_u8_id_ = P_of<t_u8_id_>;

  /////////////////////////////////////////////////////////////////////////////

  /* _n_ -> number (quantity) integer */
  using t_n_    = t_of<t_i_>;
  using T_n_    = T_of<t_n_>;
  using r_n_    = r_of<t_n_>;
  using R_n_    = R_of<t_n_>;
  using p_n_    = p_of<t_n_>;
  using P_n_    = P_of<t_n_>;

  using t_i_n_  = t_of<t_i_>;
  using T_i_n_  = T_of<t_i_n_>;
  using r_i_n_  = r_of<t_i_n_>;
  using R_i_n_  = R_of<t_i_n_>;
  using p_i_n_  = p_of<t_i_n_>;
  using P_i_n_  = P_of<t_i_n_>;

  using t_u_n_  = t_of<t_u_>;
  using T_u_n_  = T_of<t_u_n_>;
  using r_u_n_  = r_of<t_u_n_>;
  using R_u_n_  = R_of<t_u_n_>;
  using p_u_n_  = p_of<t_u_n_>;
  using P_u_n_  = P_of<t_u_n_>;

  using t_i1_n_ = t_of<t_i1_>;
  using T_i1_n_ = T_of<t_i1_n_>;
  using r_i1_n_ = r_of<t_i1_n_>;
  using R_i1_n_ = R_of<t_i1_n_>;
  using p_i1_n_ = p_of<t_i1_n_>;
  using P_i1_n_ = P_of<t_i1_n_>;

  using t_i2_n_ = t_of<t_i2_>;
  using T_i2_n_ = T_of<t_i2_n_>;
  using r_i2_n_ = r_of<t_i2_n_>;
  using R_i2_n_ = R_of<t_i2_n_>;
  using p_i2_n_ = p_of<t_i2_n_>;
  using P_i2_n_ = P_of<t_i2_n_>;

  using t_i4_n_ = t_of<t_i4_>;
  using T_i4_n_ = T_of<t_i4_n_>;
  using r_i4_n_ = r_of<t_i4_n_>;
  using R_i4_n_ = R_of<t_i4_n_>;
  using p_i4_n_ = p_of<t_i4_n_>;
  using P_i4_n_ = P_of<t_i4_n_>;

  using t_i8_n_ = t_of<t_i8_>;
  using T_i8_n_ = T_of<t_i8_n_>;
  using r_i8_n_ = r_of<t_i8_n_>;
  using R_i8_n_ = R_of<t_i8_n_>;
  using p_i8_n_ = p_of<t_i8_n_>;
  using P_i8_n_ = P_of<t_i8_n_>;

  using t_u1_n_ = t_of<t_u1_>;
  using T_u1_n_ = T_of<t_u1_n_>;
  using r_u1_n_ = r_of<t_u1_n_>;
  using R_u1_n_ = R_of<t_u1_n_>;
  using p_u1_n_ = p_of<t_u1_n_>;
  using P_u1_n_ = P_of<t_u1_n_>;

  using t_u2_n_ = t_of<t_u2_>;
  using T_u2_n_ = T_of<t_u2_n_>;
  using r_u2_n_ = r_of<t_u2_n_>;
  using R_u2_n_ = R_of<t_u2_n_>;
  using p_u2_n_ = p_of<t_u2_n_>;
  using P_u2_n_ = P_of<t_u2_n_>;

  using t_u4_n_ = t_of<t_u4_>;
  using T_u4_n_ = T_of<t_u4_n_>;
  using r_u4_n_ = r_of<t_u4_n_>;
  using R_u4_n_ = R_of<t_u4_n_>;
  using p_u4_n_ = p_of<t_u4_n_>;
  using P_u4_n_ = P_of<t_u4_n_>;

  using t_u8_n_ = t_of<t_u8_>;
  using T_u8_n_ = T_of<t_u8_n_>;
  using r_u8_n_ = r_of<t_u8_n_>;
  using R_u8_n_ = R_of<t_u8_n_>;
  using p_u8_n_ = p_of<t_u8_n_>;
  using P_u8_n_ = P_of<t_u8_n_>;

  /////////////////////////////////////////////////////////////////////////////

  /* _n_min_ -> minimal numbered integer */
  using t_n_min_   = t_of<t_n_>;
  using T_n_min_   = T_of<t_n_min_>;
  using r_n_min_   = r_of<t_n_min_>;
  using R_n_min_   = R_of<t_n_min_>;
  using p_n_min_   = p_of<t_n_min_>;
  using P_n_min_   = P_of<t_n_min_>;

  using t_i_n_min_ = t_of<t_i_n_>;
  using T_i_n_min_ = T_of<t_i_n_min_>;
  using r_i_n_min_ = r_of<t_i_n_min_>;
  using R_i_n_min_ = R_of<t_i_n_min_>;
  using p_i_n_min_ = p_of<t_i_n_min_>;
  using P_i_n_min_ = P_of<t_i_n_min_>;

  using t_u_n_min_ = t_of<t_u_n_>;
  using T_u_n_min_ = T_of<t_u_n_min_>;
  using r_u_n_min_ = r_of<t_u_n_min_>;
  using R_u_n_min_ = R_of<t_u_n_min_>;
  using p_u_n_min_ = p_of<t_u_n_min_>;
  using P_u_n_min_ = P_of<t_u_n_min_>;

  using t_i1_n_min_ = t_of<t_i1_n_>;
  using T_i1_n_min_ = T_of<t_i1_n_min_>;
  using r_i1_n_min_ = r_of<t_i1_n_min_>;
  using R_i1_n_min_ = R_of<t_i1_n_min_>;
  using p_i1_n_min_ = p_of<t_i1_n_min_>;
  using P_i1_n_min_ = P_of<t_i1_n_min_>;

  using t_i2_n_min_ = t_of<t_i2_n_>;
  using T_i2_n_min_ = T_of<t_i2_n_min_>;
  using r_i2_n_min_ = r_of<t_i2_n_min_>;
  using R_i2_n_min_ = R_of<t_i2_n_min_>;
  using p_i2_n_min_ = p_of<t_i2_n_min_>;
  using P_i2_n_min_ = P_of<t_i2_n_min_>;

  using t_i4_n_min_ = t_of<t_i4_n_>;
  using T_i4_n_min_ = T_of<t_i4_n_min_>;
  using r_i4_n_min_ = r_of<t_i4_n_min_>;
  using R_i4_n_min_ = R_of<t_i4_n_min_>;
  using p_i4_n_min_ = p_of<t_i4_n_min_>;
  using P_i4_n_min_ = P_of<t_i4_n_min_>;

  using t_i8_n_min_ = t_of<t_i8_n_>;
  using T_i8_n_min_ = T_of<t_i8_n_min_>;
  using r_i8_n_min_ = r_of<t_i8_n_min_>;
  using R_i8_n_min_ = R_of<t_i8_n_min_>;
  using p_i8_n_min_ = p_of<t_i8_n_min_>;
  using P_i8_n_min_ = P_of<t_i8_n_min_>;

  using t_u1_n_min_ = t_of<t_u1_n_>;
  using T_u1_n_min_ = T_of<t_u1_n_min_>;
  using r_u1_n_min_ = r_of<t_u1_n_min_>;
  using R_u1_n_min_ = R_of<t_u1_n_min_>;
  using p_u1_n_min_ = p_of<t_u1_n_min_>;
  using P_u1_n_min_ = P_of<t_u1_n_min_>;

  using t_u2_n_min_ = t_of<t_u2_n_>;
  using T_u2_n_min_ = T_of<t_u2_n_min_>;
  using r_u2_n_min_ = r_of<t_u2_n_min_>;
  using R_u2_n_min_ = R_of<t_u2_n_min_>;
  using p_u2_n_min_ = p_of<t_u2_n_min_>;
  using P_u2_n_min_ = P_of<t_u2_n_min_>;

  using t_u4_n_min_ = t_of<t_u4_n_>;
  using T_u4_n_min_ = T_of<t_u4_n_min_>;
  using r_u4_n_min_ = r_of<t_u4_n_min_>;
  using R_u4_n_min_ = R_of<t_u4_n_min_>;
  using p_u4_n_min_ = p_of<t_u4_n_min_>;
  using P_u4_n_min_ = P_of<t_u4_n_min_>;

  using t_u8_n_min_ = t_of<t_u8_n_>;
  using T_u8_n_min_ = T_of<t_u8_n_min_>;
  using r_u8_n_min_ = r_of<t_u8_n_min_>;
  using R_u8_n_min_ = R_of<t_u8_n_min_>;
  using p_u8_n_min_ = p_of<t_u8_n_min_>;
  using P_u8_n_min_ = P_of<t_u8_n_min_>;

  /////////////////////////////////////////////////////////////////////////////

  /* _n_max_ -> maximal numbered integer */
  using t_n_max_   = t_of<t_n_>;
  using T_n_max_   = T_of<t_n_max_>;
  using r_n_max_   = r_of<t_n_max_>;
  using R_n_max_   = R_of<t_n_max_>;
  using p_n_max_   = p_of<t_n_max_>;
  using P_n_max_   = P_of<t_n_max_>;

  using t_i_n_max_ = t_of<t_i_n_>;
  using T_i_n_max_ = T_of<t_i_n_max_>;
  using r_i_n_max_ = r_of<t_i_n_max_>;
  using R_i_n_max_ = R_of<t_i_n_max_>;
  using p_i_n_max_ = p_of<t_i_n_max_>;
  using P_i_n_max_ = P_of<t_i_n_max_>;

  using t_u_n_max_ = t_of<t_u_n_>;
  using T_u_n_max_ = T_of<t_u_n_max_>;
  using r_u_n_max_ = r_of<t_u_n_max_>;
  using R_u_n_max_ = R_of<t_u_n_max_>;
  using p_u_n_max_ = p_of<t_u_n_max_>;
  using P_u_n_max_ = P_of<t_u_n_max_>;

  using t_i1_n_max_ = t_of<t_i1_n_>;
  using T_i1_n_max_ = T_of<t_i1_n_max_>;
  using r_i1_n_max_ = r_of<t_i1_n_max_>;
  using R_i1_n_max_ = R_of<t_i1_n_max_>;
  using p_i1_n_max_ = p_of<t_i1_n_max_>;
  using P_i1_n_max_ = P_of<t_i1_n_max_>;

  using t_i2_n_max_ = t_of<t_i2_n_>;
  using T_i2_n_max_ = T_of<t_i2_n_max_>;
  using r_i2_n_max_ = r_of<t_i2_n_max_>;
  using R_i2_n_max_ = R_of<t_i2_n_max_>;
  using p_i2_n_max_ = p_of<t_i2_n_max_>;
  using P_i2_n_max_ = P_of<t_i2_n_max_>;

  using t_i4_n_max_ = t_of<t_i4_n_>;
  using T_i4_n_max_ = T_of<t_i4_n_max_>;
  using r_i4_n_max_ = r_of<t_i4_n_max_>;
  using R_i4_n_max_ = R_of<t_i4_n_max_>;
  using p_i4_n_max_ = p_of<t_i4_n_max_>;
  using P_i4_n_max_ = P_of<t_i4_n_max_>;

  using t_i8_n_max_ = t_of<t_i8_n_>;
  using T_i8_n_max_ = T_of<t_i8_n_max_>;
  using r_i8_n_max_ = r_of<t_i8_n_max_>;
  using R_i8_n_max_ = R_of<t_i8_n_max_>;
  using p_i8_n_max_ = p_of<t_i8_n_max_>;
  using P_i8_n_max_ = P_of<t_i8_n_max_>;

  using t_u1_n_max_ = t_of<t_u1_n_>;
  using T_u1_n_max_ = T_of<t_u1_n_max_>;
  using r_u1_n_max_ = r_of<t_u1_n_max_>;
  using R_u1_n_max_ = R_of<t_u1_n_max_>;
  using p_u1_n_max_ = p_of<t_u1_n_max_>;
  using P_u1_n_max_ = P_of<t_u1_n_max_>;

  using t_u2_n_max_ = t_of<t_u2_n_>;
  using T_u2_n_max_ = T_of<t_u2_n_max_>;
  using r_u2_n_max_ = r_of<t_u2_n_max_>;
  using R_u2_n_max_ = R_of<t_u2_n_max_>;
  using p_u2_n_max_ = p_of<t_u2_n_max_>;
  using P_u2_n_max_ = P_of<t_u2_n_max_>;

  using t_u4_n_max_ = t_of<t_u4_n_>;
  using T_u4_n_max_ = T_of<t_u4_n_max_>;
  using r_u4_n_max_ = r_of<t_u4_n_max_>;
  using R_u4_n_max_ = R_of<t_u4_n_max_>;
  using p_u4_n_max_ = p_of<t_u4_n_max_>;
  using P_u4_n_max_ = P_of<t_u4_n_max_>;

  using t_u8_n_max_ = t_of<t_u8_n_>;
  using T_u8_n_max_ = T_of<t_u8_n_max_>;
  using r_u8_n_max_ = r_of<t_u8_n_max_>;
  using R_u8_n_max_ = R_of<t_u8_n_max_>;
  using p_u8_n_max_ = p_of<t_u8_n_max_>;
  using P_u8_n_max_ = P_of<t_u8_n_max_>;

  /////////////////////////////////////////////////////////////////////////////

  /* _ix_ ->  integer index */
  using t_ix_    = t_of<t_id_>;
  using T_ix_    = T_of<t_ix_>;
  using r_ix_    = r_of<t_ix_>;
  using R_ix_    = R_of<t_ix_>;
  using p_ix_    = p_of<t_ix_>;
  using P_ix_    = P_of<t_ix_>;

  using t_i_ix_  = t_of<t_i_id_>;
  using T_i_ix_  = T_of<t_i_ix_>;
  using r_i_ix_  = r_of<t_i_ix_>;
  using R_i_ix_  = R_of<t_i_ix_>;
  using p_i_ix_  = p_of<t_i_ix_>;
  using P_i_ix_  = P_of<t_i_ix_>;

  using t_u_ix_  = t_of<t_u_id_>;
  using T_u_ix_  = T_of<t_u_ix_>;
  using r_u_ix_  = r_of<t_u_ix_>;
  using R_u_ix_  = R_of<t_u_ix_>;
  using p_u_ix_  = p_of<t_u_ix_>;
  using P_u_ix_  = P_of<t_u_ix_>;

  using t_i1_ix_ = t_of<t_i1_id_>;
  using T_i1_ix_ = T_of<t_i1_ix_>;
  using r_i1_ix_ = r_of<t_i1_ix_>;
  using R_i1_ix_ = R_of<t_i1_ix_>;
  using p_i1_ix_ = p_of<t_i1_ix_>;
  using P_i1_ix_ = P_of<t_i1_ix_>;

  using t_i2_ix_ = t_of<t_i2_id_>;
  using T_i2_ix_ = T_of<t_i2_ix_>;
  using r_i2_ix_ = r_of<t_i2_ix_>;
  using R_i2_ix_ = R_of<t_i2_ix_>;
  using p_i2_ix_ = p_of<t_i2_ix_>;
  using P_i2_ix_ = P_of<t_i2_ix_>;

  using t_i4_ix_ = t_of<t_i4_id_>;
  using T_i4_ix_ = T_of<t_i4_ix_>;
  using r_i4_ix_ = r_of<t_i4_ix_>;
  using R_i4_ix_ = R_of<t_i4_ix_>;
  using p_i4_ix_ = p_of<t_i4_ix_>;
  using P_i4_ix_ = P_of<t_i4_ix_>;

  using t_i8_ix_ = t_of<t_i8_id_>;
  using T_i8_ix_ = T_of<t_i8_ix_>;
  using r_i8_ix_ = r_of<t_i8_ix_>;
  using R_i8_ix_ = R_of<t_i8_ix_>;
  using p_i8_ix_ = p_of<t_i8_ix_>;
  using P_i8_ix_ = P_of<t_i8_ix_>;

  using t_u1_ix_ = t_of<t_u1_id_>;
  using T_u1_ix_ = T_of<t_u1_ix_>;
  using r_u1_ix_ = r_of<t_u1_ix_>;
  using R_u1_ix_ = R_of<t_u1_ix_>;
  using p_u1_ix_ = p_of<t_u1_ix_>;
  using P_u1_ix_ = P_of<t_u1_ix_>;

  using t_u2_ix_ = t_of<t_u2_id_>;
  using T_u2_ix_ = T_of<t_u2_ix_>;
  using r_u2_ix_ = r_of<t_u2_ix_>;
  using R_u2_ix_ = R_of<t_u2_ix_>;
  using p_u2_ix_ = p_of<t_u2_ix_>;
  using P_u2_ix_ = P_of<t_u2_ix_>;

  using t_u4_ix_ = t_of<t_u4_id_>;
  using T_u4_ix_ = T_of<t_u4_ix_>;
  using r_u4_ix_ = r_of<t_u4_ix_>;
  using R_u4_ix_ = R_of<t_u4_ix_>;
  using p_u4_ix_ = p_of<t_u4_ix_>;
  using P_u4_ix_ = P_of<t_u4_ix_>;

  using t_u8_ix_ = t_of<t_u8_id_>;
  using T_u8_ix_ = T_of<t_u8_ix_>;
  using r_u8_ix_ = r_of<t_u8_ix_>;
  using R_u8_ix_ = R_of<t_u8_ix_>;
  using p_u8_ix_ = p_of<t_u8_ix_>;
  using P_u8_ix_ = P_of<t_u8_ix_>;

  /////////////////////////////////////////////////////////////////////////////

  /* _ix_begin_ ->  begin integer index */
  using t_ix_begin_    = t_of<t_ix_>;
  using T_ix_begin_    = T_of<t_ix_begin_>;
  using r_ix_begin_    = r_of<t_ix_begin_>;
  using R_ix_begin_    = R_of<t_ix_begin_>;
  using p_ix_begin_    = p_of<t_ix_begin_>;
  using P_ix_begin_    = P_of<t_ix_begin_>;

  using t_i_ix_begin_  = t_of<t_i_ix_>;
  using T_i_ix_begin_  = T_of<t_i_ix_begin_>;
  using r_i_ix_begin_  = r_of<t_i_ix_begin_>;
  using R_i_ix_begin_  = R_of<t_i_ix_begin_>;
  using p_i_ix_begin_  = p_of<t_i_ix_begin_>;
  using P_i_ix_begin_  = P_of<t_i_ix_begin_>;

  using t_u_ix_begin_  = t_of<t_u_ix_>;
  using T_u_ix_begin_  = T_of<t_u_ix_begin_>;
  using r_u_ix_begin_  = r_of<t_u_ix_begin_>;
  using R_u_ix_begin_  = R_of<t_u_ix_begin_>;
  using p_u_ix_begin_  = p_of<t_u_ix_begin_>;
  using P_u_ix_begin_  = P_of<t_u_ix_begin_>;

  using t_i1_ix_begin_ = t_of<t_i1_ix_>;
  using T_i1_ix_begin_ = T_of<t_i1_ix_begin_>;
  using r_i1_ix_begin_ = r_of<t_i1_ix_begin_>;
  using R_i1_ix_begin_ = R_of<t_i1_ix_begin_>;
  using p_i1_ix_begin_ = p_of<t_i1_ix_begin_>;
  using P_i1_ix_begin_ = P_of<t_i1_ix_begin_>;

  using t_i2_ix_begin_ = t_of<t_i2_ix_>;
  using T_i2_ix_begin_ = T_of<t_i2_ix_begin_>;
  using r_i2_ix_begin_ = r_of<t_i2_ix_begin_>;
  using R_i2_ix_begin_ = R_of<t_i2_ix_begin_>;
  using p_i2_ix_begin_ = p_of<t_i2_ix_begin_>;
  using P_i2_ix_begin_ = P_of<t_i2_ix_begin_>;

  using t_i4_ix_begin_ = t_of<t_i4_ix_>;
  using T_i4_ix_begin_ = T_of<t_i4_ix_begin_>;
  using r_i4_ix_begin_ = r_of<t_i4_ix_begin_>;
  using R_i4_ix_begin_ = R_of<t_i4_ix_begin_>;
  using p_i4_ix_begin_ = p_of<t_i4_ix_begin_>;
  using P_i4_ix_begin_ = P_of<t_i4_ix_begin_>;

  using t_i8_ix_begin_ = t_of<t_i8_ix_>;
  using T_i8_ix_begin_ = T_of<t_i8_ix_begin_>;
  using r_i8_ix_begin_ = r_of<t_i8_ix_begin_>;
  using R_i8_ix_begin_ = R_of<t_i8_ix_begin_>;
  using p_i8_ix_begin_ = p_of<t_i8_ix_begin_>;
  using P_i8_ix_begin_ = P_of<t_i8_ix_begin_>;

  using t_u1_ix_begin_ = t_of<t_u1_ix_>;
  using T_u1_ix_begin_ = T_of<t_u1_ix_begin_>;
  using r_u1_ix_begin_ = r_of<t_u1_ix_begin_>;
  using R_u1_ix_begin_ = R_of<t_u1_ix_begin_>;
  using p_u1_ix_begin_ = p_of<t_u1_ix_begin_>;
  using P_u1_ix_begin_ = P_of<t_u1_ix_begin_>;

  using t_u2_ix_begin_ = t_of<t_u2_ix_>;
  using T_u2_ix_begin_ = T_of<t_u2_ix_begin_>;
  using r_u2_ix_begin_ = r_of<t_u2_ix_begin_>;
  using R_u2_ix_begin_ = R_of<t_u2_ix_begin_>;
  using p_u2_ix_begin_ = p_of<t_u2_ix_begin_>;
  using P_u2_ix_begin_ = P_of<t_u2_ix_begin_>;

  using t_u4_ix_begin_ = t_of<t_u4_ix_>;
  using T_u4_ix_begin_ = T_of<t_u4_ix_begin_>;
  using r_u4_ix_begin_ = r_of<t_u4_ix_begin_>;
  using R_u4_ix_begin_ = R_of<t_u4_ix_begin_>;
  using p_u4_ix_begin_ = p_of<t_u4_ix_begin_>;
  using P_u4_ix_begin_ = P_of<t_u4_ix_begin_>;

  using t_u8_ix_begin_ = t_of<t_u8_ix_>;
  using T_u8_ix_begin_ = T_of<t_u8_ix_begin_>;
  using r_u8_ix_begin_ = r_of<t_u8_ix_begin_>;
  using R_u8_ix_begin_ = R_of<t_u8_ix_begin_>;
  using p_u8_ix_begin_ = p_of<t_u8_ix_begin_>;
  using P_u8_ix_begin_ = P_of<t_u8_ix_begin_>;

  /////////////////////////////////////////////////////////////////////////////

  /* _ix_end_ ->  begin integer index */
  using t_ix_end_    = t_of<t_ix_>;
  using T_ix_end_    = T_of<t_ix_end_>;
  using r_ix_end_    = r_of<t_ix_end_>;
  using R_ix_end_    = R_of<t_ix_end_>;
  using p_ix_end_    = p_of<t_ix_end_>;
  using P_ix_end_    = P_of<t_ix_end_>;

  using t_i_ix_end_  = t_of<t_i_ix_>;
  using T_i_ix_end_  = T_of<t_i_ix_end_>;
  using r_i_ix_end_  = r_of<t_i_ix_end_>;
  using R_i_ix_end_  = R_of<t_i_ix_end_>;
  using p_i_ix_end_  = p_of<t_i_ix_end_>;
  using P_i_ix_end_  = P_of<t_i_ix_end_>;

  using t_u_ix_end_  = t_of<t_u_ix_>;
  using T_u_ix_end_  = T_of<t_u_ix_end_>;
  using r_u_ix_end_  = r_of<t_u_ix_end_>;
  using R_u_ix_end_  = R_of<t_u_ix_end_>;
  using p_u_ix_end_  = p_of<t_u_ix_end_>;
  using P_u_ix_end_  = P_of<t_u_ix_end_>;

  using t_i1_ix_end_ = t_of<t_i1_ix_>;
  using T_i1_ix_end_ = T_of<t_i1_ix_end_>;
  using r_i1_ix_end_ = r_of<t_i1_ix_end_>;
  using R_i1_ix_end_ = R_of<t_i1_ix_end_>;
  using p_i1_ix_end_ = p_of<t_i1_ix_end_>;
  using P_i1_ix_end_ = P_of<t_i1_ix_end_>;

  using t_i2_ix_end_ = t_of<t_i2_ix_>;
  using T_i2_ix_end_ = T_of<t_i2_ix_end_>;
  using r_i2_ix_end_ = r_of<t_i2_ix_end_>;
  using R_i2_ix_end_ = R_of<t_i2_ix_end_>;
  using p_i2_ix_end_ = p_of<t_i2_ix_end_>;
  using P_i2_ix_end_ = P_of<t_i2_ix_end_>;

  using t_i4_ix_end_ = t_of<t_i4_ix_>;
  using T_i4_ix_end_ = T_of<t_i4_ix_end_>;
  using r_i4_ix_end_ = r_of<t_i4_ix_end_>;
  using R_i4_ix_end_ = R_of<t_i4_ix_end_>;
  using p_i4_ix_end_ = p_of<t_i4_ix_end_>;
  using P_i4_ix_end_ = P_of<t_i4_ix_end_>;

  using t_i8_ix_end_ = t_of<t_i8_ix_>;
  using T_i8_ix_end_ = T_of<t_i8_ix_end_>;
  using r_i8_ix_end_ = r_of<t_i8_ix_end_>;
  using R_i8_ix_end_ = R_of<t_i8_ix_end_>;
  using p_i8_ix_end_ = p_of<t_i8_ix_end_>;
  using P_i8_ix_end_ = P_of<t_i8_ix_end_>;

  using t_u1_ix_end_ = t_of<t_u1_ix_>;
  using T_u1_ix_end_ = T_of<t_u1_ix_end_>;
  using r_u1_ix_end_ = r_of<t_u1_ix_end_>;
  using R_u1_ix_end_ = R_of<t_u1_ix_end_>;
  using p_u1_ix_end_ = p_of<t_u1_ix_end_>;
  using P_u1_ix_end_ = P_of<t_u1_ix_end_>;

  using t_u2_ix_end_ = t_of<t_u2_ix_>;
  using T_u2_ix_end_ = T_of<t_u2_ix_end_>;
  using r_u2_ix_end_ = r_of<t_u2_ix_end_>;
  using R_u2_ix_end_ = R_of<t_u2_ix_end_>;
  using p_u2_ix_end_ = p_of<t_u2_ix_end_>;
  using P_u2_ix_end_ = P_of<t_u2_ix_end_>;

  using t_u4_ix_end_ = t_of<t_u4_ix_>;
  using T_u4_ix_end_ = T_of<t_u4_ix_end_>;
  using r_u4_ix_end_ = r_of<t_u4_ix_end_>;
  using R_u4_ix_end_ = R_of<t_u4_ix_end_>;
  using p_u4_ix_end_ = p_of<t_u4_ix_end_>;
  using P_u4_ix_end_ = P_of<t_u4_ix_end_>;

  using t_u8_ix_end_ = t_of<t_u8_ix_>;
  using T_u8_ix_end_ = T_of<t_u8_ix_end_>;
  using r_u8_ix_end_ = r_of<t_u8_ix_end_>;
  using R_u8_ix_end_ = R_of<t_u8_ix_end_>;
  using p_u8_ix_end_ = p_of<t_u8_ix_end_>;
  using P_u8_ix_end_ = P_of<t_u8_ix_end_>;

  /////////////////////////////////////////////////////////////////////////////

  using t_s_ = t_of<__SIZE_TYPE__>;
  using T_s_ = T_of<t_s_>;
  using r_s_ = r_of<t_s_>;
  using R_s_ = R_of<t_s_>;
  using p_s_ = p_of<t_s_>;
  using P_s_ = P_of<t_s_>;

  /////////////////////////////////////////////////////////////////////////////

  using t_s_min_ = t_of<t_s_>;
  using T_s_min_ = T_of<t_s_min_>;
  using r_s_min_ = r_of<t_s_min_>;
  using R_s_min_ = R_of<t_s_min_>;
  using p_s_min_ = p_of<t_s_min_>;
  using P_s_min_ = P_of<t_s_min_>;

  /////////////////////////////////////////////////////////////////////////////

  using t_s_max_ = t_of<t_s_>;
  using T_s_max_ = T_of<t_s_max_>;
  using r_s_max_ = r_of<t_s_max_>;
  using R_s_max_ = R_of<t_s_max_>;
  using p_s_max_ = p_of<t_s_max_>;
  using P_s_max_ = P_of<t_s_max_>;

  /////////////////////////////////////////////////////////////////////////////

  using t_cnt_    = t_of<t_n_>;
  using T_cnt_    = T_of<t_cnt_>;
  using r_cnt_    = r_of<t_cnt_>;
  using p_cnt_    = p_of<t_cnt_>;
  using P_cnt_    = P_of<t_cnt_>;

  using t_u1_cnt_ = t_of<t_u1_n_>;
  using T_u1_cnt_ = T_of<t_u1_cnt_>;
  using r_u1_cnt_ = r_of<t_u1_cnt_>;
  using p_u1_cnt_ = p_of<t_u1_cnt_>;
  using P_u1_cnt_ = P_of<t_u1_cnt_>;

  using t_u2_cnt_ = t_of<t_u2_n_>;
  using T_u2_cnt_ = T_of<t_u2_cnt_>;
  using r_u2_cnt_ = r_of<t_u2_cnt_>;
  using p_u2_cnt_ = p_of<t_u2_cnt_>;
  using P_u2_cnt_ = P_of<t_u2_cnt_>;

  using t_u4_cnt_ = t_of<t_u4_n_>;
  using T_u4_cnt_ = T_of<t_u4_cnt_>;
  using r_u4_cnt_ = r_of<t_u4_cnt_>;
  using p_u4_cnt_ = p_of<t_u4_cnt_>;
  using P_u4_cnt_ = P_of<t_u4_cnt_>;

  using t_u8_cnt_ = t_of<t_u8_n_>;
  using T_u8_cnt_ = T_of<t_u8_cnt_>;
  using r_u8_cnt_ = r_of<t_u8_cnt_>;
  using p_u8_cnt_ = p_of<t_u8_cnt_>;
  using P_u8_cnt_ = P_of<t_u8_cnt_>;

  /////////////////////////////////////////////////////////////////////////////

  using t_bytes_ = t_of<t_u_>;
  using T_bytes_ = T_of<t_bytes_>;
  using r_bytes_ = r_of<t_bytes_>;
  using p_bytes_ = p_of<t_bytes_>;
  using P_bytes_ = P_of<t_bytes_>;

  using t_u1_bytes_ = t_of<t_u1_>;
  using T_u1_bytes_ = T_of<t_u1_bytes_>;
  using r_u1_bytes_ = r_of<t_u1_bytes_>;
  using p_u1_bytes_ = p_of<t_u1_bytes_>;
  using P_u1_bytes_ = P_of<t_u1_bytes_>;

  using t_u2_bytes_ = t_of<t_u2_>;
  using T_u2_bytes_ = T_of<t_u2_bytes_>;
  using r_u2_bytes_ = r_of<t_u2_bytes_>;
  using p_u2_bytes_ = p_of<t_u2_bytes_>;
  using P_u2_bytes_ = P_of<t_u2_bytes_>;

  using t_u4_bytes_ = t_of<t_u4_>;
  using T_u4_bytes_ = T_of<t_u4_bytes_>;
  using r_u4_bytes_ = r_of<t_u4_bytes_>;
  using p_u4_bytes_ = p_of<t_u4_bytes_>;
  using P_u4_bytes_ = P_of<t_u4_bytes_>;

  using t_u8_bytes_ = t_of<t_u8_>;
  using T_u8_bytes_ = T_of<t_u8_bytes_>;
  using r_u8_bytes_ = r_of<t_u8_bytes_>;
  using p_u8_bytes_ = p_of<t_u8_bytes_>;
  using P_u8_bytes_ = P_of<t_u8_bytes_>;

  /////////////////////////////////////////////////////////////////////////////

  using t_byte_ = t_of<t_uchar>;
  using T_byte_ = T_of<t_byte_>;
  using r_byte_ = r_of<t_byte_>;
  using p_byte_ = p_of<t_byte_>;
  using P_byte_ = P_of<t_byte_>;

  /////////////////////////////////////////////////////////////////////////////

  using t_fd_ = t_of<t_i4_>;
  using T_fd_ = T_of<t_fd_>;
  using r_fd_ = r_of<t_fd_>;
  using p_fd_ = p_of<t_fd_>;
  using P_fd_ = P_of<t_fd_>;

  /////////////////////////////////////////////////////////////////////////////

  using t_percentage_ = t_of<t_u1_>;
  using T_percentage_ = T_of<t_percentage_>;
  using r_percentage_ = r_of<t_percentage_>;
  using p_percentage_ = p_of<t_percentage_>;
  using P_percentage_ = P_of<t_percentage_>;

  /////////////////////////////////////////////////////////////////////////////

  using t_seqno_ = t_of<t_u_>;
  using T_seqno_ = T_of<t_seqno_>;
  using r_seqno_ = r_of<t_seqno_>;
  using p_seqno_ = p_of<t_seqno_>;
  using P_seqno_ = P_of<t_seqno_>;

  /////////////////////////////////////////////////////////////////////////////

  using t_errn_ = t_of<t_i_>;
  using T_errn_ = T_of<t_errn_>;
  using r_errn_ = r_of<t_errn_>;
  using p_errn_ = p_of<t_errn_>;
  using P_errn_ = P_of<t_errn_>;

  /////////////////////////////////////////////////////////////////////////////

  using t_nsec_ = t_of<t_u8_>;
  using T_nsec_ = T_of<t_nsec_>;
  using r_nsec_ = r_of<t_nsec_>;
  using p_nsec_ = p_of<t_nsec_>;
  using P_nsec_ = P_of<t_nsec_>;

  /////////////////////////////////////////////////////////////////////////////

  using t_usec_ = t_of<t_u8_>;
  using T_usec_ = T_of<t_usec_>;
  using r_usec_ = r_of<t_usec_>;
  using p_usec_ = p_of<t_usec_>;
  using P_usec_ = P_of<t_usec_>;

  /////////////////////////////////////////////////////////////////////////////

  using t_msec_ = t_of<t_u8_>;
  using T_msec_ = T_of<t_msec_>;
  using r_msec_ = r_of<t_msec_>;
  using p_msec_ = p_of<t_msec_>;
  using P_msec_ = P_of<t_msec_>;

  /////////////////////////////////////////////////////////////////////////////

  using t_sec_ = t_of<t_u4_>;
  using T_sec_ = T_of<t_sec_>;
  using r_sec_ = r_of<t_sec_>;
  using p_sec_ = p_of<t_sec_>;
  using P_sec_ = P_of<t_sec_>;

  /////////////////////////////////////////////////////////////////////////////

  using t_min_ = t_of<t_u4_>;
  using T_min_ = T_of<t_min_>;
  using r_min_ = r_of<t_min_>;
  using p_min_ = p_of<t_min_>;
  using P_min_ = P_of<t_min_>;

  /////////////////////////////////////////////////////////////////////////////

  using t_ticks_ = t_of<t_u8_>;
  using T_ticks_ = T_of<t_ticks_>;
  using r_ticks_ = r_of<t_ticks_>;
  using p_ticks_ = p_of<t_ticks_>;
  using P_ticks_ = P_of<t_ticks_>;

  /////////////////////////////////////////////////////////////////////////////

  constexpr t_nullptr     NULLPTR = nullptr;

  constexpr t_fd_        BAD_FD_   = -1;
  constexpr t_errn_      NO_ERRN_  =  0;
  constexpr t_errn_      BAD_ERRN_ = -1;

  /////////////////////////////////////////////////////////////////////////////

  template<typename... As>
  struct t_pack {
    constexpr static t_n_ N = sizeof...(As);
  };

  using t_empty_pack = t_pack<>;

  /////////////////////////////////////////////////////////////////////////////

  enum class t_dummy { DUMMY };

  /////////////////////////////////////////////////////////////////////////////

  template<typename, typename...> struct t_undef;
  template<typename T, T>         struct t_undef_value;

  /////////////////////////////////////////////////////////////////////////////

  enum t_well_formed { WELL_FORMED };

  template<typename...> using t_expr = t_well_formed;

  /////////////////////////////////////////////////////////////////////////////

  enum t_emplace    { EMPLACE      };  // XXX TRY_EMPLACE
  enum t_emplace_it { EMPLACE_IT   };  // XXX TRY_EMPLACE
  enum t_fmt        { FMT, FMT_IT  };  // XXX FMT is deprecated
  enum t_fmt_va     { FMT_VA_IT    };
  enum t_fargs      { FARGS        };  // PASSING forwarding arguments

  enum t_init_fargs  { INIT_FARGS  };  // HOW TO INIT OBJECT
  enum t_init_none   { INIT_NONE   };
  enum t_init_custom { INIT_CUSTOM };

  /////////////////////////////////////////////////////////////////////////////

  using t_opt = t_well_formed;

  enum  t_opt1 { OPT1 };
  enum  t_opt2 { OPT2 };
  enum  t_opt3 { OPT3 };
  enum  t_opt4 { OPT4 };
  enum  t_opt5 { OPT5 };
  enum  t_opt6 { OPT6 };
  enum  t_opt7 { OPT7 };
  enum  t_opt8 { OPT8 };
  enum  t_opt9 { OPT9 };

  /////////////////////////////////////////////////////////////////////////////

  enum t_hidden  { HIDDEN };
  enum t_hidden1 { HIDDEN1 };
  enum t_hidden2 { HIDDEN2 };
  enum t_hidden3 { HIDDEN3 };
  enum t_hidden4 { HIDDEN4 };

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

  /////////////////////////////////////////////////////////////////////////////

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

  enum t_byte_tag_       { };
  enum t_fd_tag_         { };
  enum t_cnt_tag_        { };
  enum t_validity_tag_   { };
  enum t_bytes_tag_      { };
  enum t_percentage_tag_ { };
  enum t_seqno_tag_      { };
  enum t_nsec_tag_       { };
  enum t_usec_tag_       { };
  enum t_msec_tag_       { };
  enum t_sec_tag_        { };
  enum t_min_tag_        { };
  enum t_ticks_tag_      { };
  enum t_errn_tag_       { };

  /////////////////////////////////////////////////////////////////////////////

  struct t_truth_tag_ {
    using t_ops = t_pack<t_op_equal_tag, t_op_not_equal_tag>;
  };

  struct t_i_tag_ {
    using t_ops = t_ops_value_tag;
  };

  struct t_u_tag_ {
    using t_ops = t_ops_value_tag;
  };

  struct t_b_tag_ { // binary
    using t_ops = t_ops_value_tag;
  };

  struct t_h_tag_ { // hexadecimal
    using t_ops = t_ops_value_tag;
  };

  struct t_n_tag_ {
    using t_ops = t_ops_value_tag;
  };

  struct t_n_max_tag_ {
    using t_ops = t_ops_value_tag;
  };

  struct t_n_min_tag_ {
    using t_ops = t_ops_value_tag;
  };

  struct t_id_tag_ {
    using t_ops = t_ops_value_tag;
  };

  struct t_ix_tag_ {
    using t_ops = t_ops_value_tag;
  };

  struct t_ix_begin_tag_ {
    using t_ops = t_ops_value_tag;
  };

  struct t_ix_end_tag_ {
    using t_ops = t_ops_value_tag;
  };

  struct t_s_tag_ {
    using t_ops = t_ops_value_tag;
  };

  struct t_s_max_tag_ {
    using t_ops = t_ops_value_tag;
  };

  struct t_s_min_tag_ {
    using t_ops = t_ops_value_tag;
  };

  /////////////////////////////////////////////////////////////////////////////

  template<typename T> struct t_add_identity { using t_identity = T; };
  template<typename T> struct t_add_result   { using t_result   = T; };
  template<typename T> struct t_add_value    { using t_value    = T; };

  template<typename T> using t_identity_of = typename T::t_identity;
  template<typename T> using t_result_of   = typename T::t_result;
  template<typename T> using t_value_of    = typename T::t_value;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, T V> struct t_add_VALUE {
    constexpr static T VALUE = V;
  };

  template<typename T> constexpr auto VALUE_of = T::VALUE;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, T V> struct t_add_MAX {
    constexpr static T MAX = V;
  };

  template<typename T> constexpr auto MAX_of = T::MAX;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, T V> struct t_add_MIN {
    constexpr static T MIN = V;
  };

  template<typename T> constexpr auto MIN_of = T::MIN;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T>
  constexpr t_s_ SIZEOF = sizeof(T);

  template<typename T> struct t_add_SIZEOF {
    constexpr static t_s_ SIZEOF = SIZEOF<T>;
  };

  template<typename T> constexpr t_s_ SIZE_of = T::SIZEOF;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T> struct t_add_BITS {
    constexpr static t_n_ BITS = SIZEOF<T> * __CHAR_BIT__;
  };

  template<typename T> constexpr auto BITS_of = T::BITS;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, T V, typename H = t_dummy>
  struct t_constant : t_add_value<T>,
                      t_add_VALUE<T, V>,
                      t_add_identity<H> {
  };

  /////////////////////////////////////////////////////////////////////////////

  template<typename T>
  struct t_pass { };

  template<typename T>
  constexpr
  t_pass<T> pass() { return t_pass<T>(); }

///////////////////////////////////////////////////////////////////////////////
}
}

#endif
