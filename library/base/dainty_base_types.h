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

#ifndef _DAINTY_BASE_TYPES_H_
#define _DAINTY_BASE_TYPES_H_

#include <cstdint>

namespace dainty
{
namespace base
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

    t_prefix() = delete;
  };

  template<>
  struct t_prefix<void> {
    using t_  = void;
    using p_  = void*;
    using P_  = const void*;

    t_prefix() = delete;
  };

  template<typename T> struct t_prefix<T*>;
  template<typename T> struct t_prefix<T&>;
  template<typename T> struct t_prefix<const T>;

  template<typename T> using T_prefix_of = typename t_prefix<T>::T_;
  template<typename T> using t_prefix_of = typename t_prefix<T>::t_;
  template<typename T> using p_prefix_of = typename t_prefix<T>::p_;
  template<typename T> using P_prefix_of = typename t_prefix<T>::P_;
  template<typename T> using r_prefix_of = typename t_prefix<T>::r_;
  template<typename T> using R_prefix_of = typename t_prefix<T>::R_;
  template<typename T> using x_prefix_of = typename t_prefix<T>::x_;
  template<typename T> using X_prefix_of = typename t_prefix<T>::X_;

  /////////////////////////////////////////////////////////////////////////////

  using t_bool         = t_prefix_of<bool>;
  using T_bool         = T_prefix_of<t_bool>;
  using r_bool         = r_prefix_of<t_bool>;
  using p_bool         = p_prefix_of<t_bool>;
  using P_bool         = P_prefix_of<t_bool>;

  using t_char         = t_prefix_of<char>;
  using T_char         = T_prefix_of<t_char>;
  using r_char         = r_prefix_of<t_char>;
  using p_char         = p_prefix_of<t_char>;
  using P_char         = P_prefix_of<t_char>;

  using t_schar        = t_prefix_of<signed char>;
  using T_schar        = T_prefix_of<t_schar>;
  using r_schar        = r_prefix_of<t_schar>;
  using p_schar        = p_prefix_of<t_schar>;
  using P_schar        = P_prefix_of<t_schar>;

  using t_uchar        = t_prefix_of<unsigned char>;
  using T_uchar        = T_prefix_of<t_uchar>;
  using r_uchar        = r_prefix_of<t_uchar>;
  using p_uchar        = p_prefix_of<t_uchar>;
  using P_uchar        = P_prefix_of<t_uchar>;

  using t_int          = t_prefix_of<int>;
  using T_int          = T_prefix_of<t_int>;
  using r_int          = r_prefix_of<t_int>;
  using p_int          = p_prefix_of<t_int>;
  using P_int          = P_prefix_of<t_int>;

  using t_uint         = t_prefix_of<unsigned int>;
  using T_uint         = T_prefix_of<t_uint>;
  using r_uint         = r_prefix_of<t_uint>;
  using p_uint         = p_prefix_of<t_uint>;
  using P_uint         = P_prefix_of<t_uint>;

  using t_short        = t_prefix_of<short>;
  using T_short        = T_prefix_of<t_short>;
  using r_short        = r_prefix_of<t_short>;
  using p_short        = p_prefix_of<t_short>;
  using P_short        = P_prefix_of<t_short>;

  using t_ushort       = t_prefix_of<unsigned short>;
  using T_ushort       = T_prefix_of<t_ushort>;
  using r_ushort       = r_prefix_of<t_ushort>;
  using p_ushort       = p_prefix_of<t_ushort>;
  using P_ushort       = P_prefix_of<t_ushort>;

  using t_long         = t_prefix_of<long int>;
  using T_long         = T_prefix_of<t_long>;
  using r_long         = r_prefix_of<t_long>;
  using p_long         = p_prefix_of<t_long>;
  using P_long         = P_prefix_of<t_long>;

  using t_ulong        = t_prefix_of<unsigned long int>;
  using T_ulong        = T_prefix_of<t_ulong>;
  using r_ulong        = r_prefix_of<t_ulong>;
  using p_ulong        = p_prefix_of<t_ulong>;
  using P_ulong        = P_prefix_of<t_ulong>;

  using t_llong        = t_prefix_of<long long int>;
  using T_llong        = T_prefix_of<t_llong>;
  using r_llong        = r_prefix_of<t_llong>;
  using p_llong        = p_prefix_of<t_llong>;
  using P_llong        = P_prefix_of<t_llong>;

  using t_ullong       = t_prefix_of<unsigned long long int>;
  using T_ullong       = T_prefix_of<t_ullong>;
  using r_ullong       = r_prefix_of<t_ullong>;
  using p_ullong       = p_prefix_of<t_ullong>;
  using P_ullong       = P_prefix_of<t_ullong>;

  using t_float        = t_prefix_of<float>;
  using T_float        = T_prefix_of<t_float>;
  using r_float        = r_prefix_of<t_float>;
  using p_float        = p_prefix_of<t_float>;
  using P_float        = P_prefix_of<t_float>;

  using t_double       = t_prefix_of<double>;
  using T_double       = T_prefix_of<t_double>;
  using r_double       = r_prefix_of<t_double>;
  using p_double       = p_prefix_of<t_double>;
  using P_double       = P_prefix_of<t_double>;

  using t_ldouble      = t_prefix_of<long double>;
  using T_ldouble      = T_prefix_of<t_ldouble>;
  using r_ldouble      = r_prefix_of<t_ldouble>;
  using p_ldouble      = p_prefix_of<t_ldouble>;
  using P_ldouble      = P_prefix_of<t_ldouble>;

  using t_void         = t_prefix_of<void>;
  using p_void         = p_prefix_of<t_void>;
  using P_void         = P_prefix_of<t_void>;

  using t_int8         = t_prefix_of<std::int8_t>;
  using T_int8         = T_prefix_of<t_int8>;
  using r_int8         = r_prefix_of<t_int8>;
  using p_int8         = p_prefix_of<t_int8>;
  using P_int8         = P_prefix_of<t_int8>;

  using t_int16        = t_prefix_of<std::int16_t>;
  using T_int16        = T_prefix_of<t_int16>;
  using r_int16        = r_prefix_of<t_int16>;
  using p_int16        = p_prefix_of<t_int16>;
  using P_int16        = P_prefix_of<t_int16>;

  using t_int32        = t_prefix_of<std::int32_t>;
  using T_int32        = T_prefix_of<t_int32>;
  using r_int32        = r_prefix_of<t_int32>;
  using p_int32        = p_prefix_of<t_int32>;
  using P_int32        = P_prefix_of<t_int32>;

  using t_int64        = t_prefix_of<std::int64_t>;
  using T_int64        = T_prefix_of<t_int64>;
  using r_int64        = r_prefix_of<t_int64>;
  using p_int64        = p_prefix_of<t_int64>;
  using P_int64        = P_prefix_of<t_int64>;

  using t_uint8        = t_prefix_of<std::uint8_t>;
  using T_uint8        = T_prefix_of<t_uint8>;
  using r_uint8        = r_prefix_of<t_uint8>;
  using p_uint8        = p_prefix_of<t_uint8>;
  using P_uint8        = P_prefix_of<t_uint8>;

  using t_uint16       = t_prefix_of<std::uint16_t>;
  using T_uint16       = T_prefix_of<t_uint16>;
  using r_uint16       = r_prefix_of<t_uint16>;
  using p_uint16       = p_prefix_of<t_uint16>;
  using P_uint16       = P_prefix_of<t_uint16>;

  using t_uint32       = t_prefix_of<std::uint32_t>;
  using T_uint32       = T_prefix_of<t_uint32>;
  using r_uint32       = r_prefix_of<t_uint32>;
  using p_uint32       = p_prefix_of<t_uint32>;
  using P_uint32       = P_prefix_of<t_uint32>;

  using t_uint64       = t_prefix_of<std::uint64_t>;
  using T_uint64       = T_prefix_of<t_uint64>;
  using r_uint64       = r_prefix_of<t_uint64>;
  using p_uint64       = p_prefix_of<t_uint64>;
  using P_uint64       = P_prefix_of<t_uint64>;

  using t_least8       = t_prefix_of<std::int_least8_t>;
  using T_least8       = T_prefix_of<t_least8>;
  using r_least8       = r_prefix_of<t_least8>;
  using p_least8       = p_prefix_of<t_least8>;
  using P_least8       = P_prefix_of<t_least8>;

  using t_least16      = t_prefix_of<std::int_least16_t>;
  using T_least16      = T_prefix_of<t_least16>;
  using r_least16      = r_prefix_of<t_least16>;
  using p_least16      = p_prefix_of<t_least16>;
  using P_least16      = P_prefix_of<t_least16>;

  using t_least32      = t_prefix_of<std::int_least32_t>;
  using T_least32      = T_prefix_of<t_least32>;
  using r_least32      = r_prefix_of<t_least32>;
  using p_least32      = p_prefix_of<t_least32>;
  using P_least32      = P_prefix_of<t_least32>;

  using t_least64      = t_prefix_of<std::int_least64_t>;
  using T_least64      = T_prefix_of<t_least64>;
  using r_least64      = r_prefix_of<t_least64>;
  using p_least64      = p_prefix_of<t_least64>;
  using P_least64      = P_prefix_of<t_least64>;

  using t_uleast8      = t_prefix_of<std::uint_least8_t>;
  using T_uleast8      = T_prefix_of<t_uleast8>;
  using r_uleast8      = r_prefix_of<t_uleast8>;
  using p_uleast8      = p_prefix_of<t_uleast8>;
  using P_uleast8      = P_prefix_of<t_uleast8>;

  using t_uleast16     = t_prefix_of<std::uint_least16_t>;
  using T_uleast16     = T_prefix_of<t_uleast16>;
  using r_uleast16     = r_prefix_of<t_uleast16>;
  using p_uleast16     = p_prefix_of<t_uleast16>;
  using P_uleast16     = P_prefix_of<t_uleast16>;

  using t_uleast32     = t_prefix_of<std::uint_least32_t>;
  using T_uleast32     = T_prefix_of<t_uleast32>;
  using r_uleast32     = r_prefix_of<t_uleast32>;
  using p_uleast32     = p_prefix_of<t_uleast32>;
  using P_uleast32     = P_prefix_of<t_uleast32>;

  using t_uleast64     = t_prefix_of<std::uint_least64_t>;
  using T_uleast64     = T_prefix_of<t_uleast64>;
  using r_uleast64     = r_prefix_of<t_uleast64>;
  using p_uleast64     = p_prefix_of<t_uleast64>;
  using P_uleast64     = P_prefix_of<t_uleast64>;

  using t_fast8        = t_prefix_of<std::int_fast8_t>;
  using T_fast8        = T_prefix_of<t_fast8>;
  using r_fast8        = r_prefix_of<t_fast8>;
  using p_fast8        = p_prefix_of<t_fast8>;
  using P_fast8        = P_prefix_of<t_fast8>;

  using t_fast16       = t_prefix_of<std::int_fast16_t>;
  using T_fast16       = T_prefix_of<t_fast16>;
  using r_fast16       = r_prefix_of<t_fast16>;
  using p_fast16       = p_prefix_of<t_fast16>;
  using P_fast16       = P_prefix_of<t_fast16>;

  using t_fast32       = t_prefix_of<std::int_fast32_t>;
  using T_fast32       = T_prefix_of<t_fast32>;
  using r_fast32       = r_prefix_of<t_fast32>;
  using p_fast32       = p_prefix_of<t_fast32>;
  using P_fast32       = P_prefix_of<t_fast32>;

  using t_fast64       = t_prefix_of<std::int_fast64_t>;
  using T_fast64       = T_prefix_of<t_fast64>;
  using r_fast64       = r_prefix_of<t_fast64>;
  using p_fast64       = p_prefix_of<t_fast64>;
  using P_fast64       = P_prefix_of<t_fast64>;

  using t_ufast8       = t_prefix_of<std::uint_fast8_t>;
  using T_ufast8       = T_prefix_of<t_ufast8>;
  using r_ufast8       = r_prefix_of<t_ufast8>;
  using p_ufast8       = p_prefix_of<t_ufast8>;
  using P_ufast8       = P_prefix_of<t_ufast8>;

  using t_ufast16      = t_prefix_of<std::uint_fast16_t>;
  using T_ufast16      = T_prefix_of<t_ufast16>;
  using r_ufast16      = r_prefix_of<t_ufast16>;
  using p_ufast16      = p_prefix_of<t_ufast16>;
  using P_ufast16      = P_prefix_of<t_ufast16>;

  using t_ufast32      = t_prefix_of<std::uint_fast32_t>;
  using T_ufast32      = T_prefix_of<t_ufast32>;
  using r_ufast32      = r_prefix_of<t_ufast32>;
  using p_ufast32      = p_prefix_of<t_ufast32>;
  using P_ufast32      = P_prefix_of<t_ufast32>;

  using t_ufast64      = t_prefix_of<std::uint_fast64_t>;
  using T_ufast64      = T_prefix_of<t_ufast64>;
  using r_ufast64      = r_prefix_of<t_ufast64>;
  using p_ufast64      = p_prefix_of<t_ufast64>;
  using P_ufast64      = P_prefix_of<t_ufast64>;

  using t_uintptr      = t_prefix_of<std::uintptr_t>;
  using T_uintptr      = T_prefix_of<t_uintptr>;
  using r_uintptr      = r_prefix_of<t_uintptr>;
  using p_uintptr      = p_prefix_of<t_uintptr>;
  using P_uintptr      = P_prefix_of<t_uintptr>;

  /////////////////////////////////////////////////////////////////////////////

  using t_nullptr      = decltype(nullptr);

  /////////////////////////////////////////////////////////////////////////////

  using t_u8_          = t_prefix_of<t_uint8>;
  using t_u16_         = t_prefix_of<t_uint16>;
  using t_u32_         = t_prefix_of<t_uint32>;
  using t_u64_         = t_prefix_of<t_uint64>;
  using t_u_           = t_prefix_of<t_u64_>;

  using t_i8_          = t_prefix_of<t_int8>;
  using t_i16_         = t_prefix_of<t_int16>;
  using t_i32_         = t_prefix_of<t_int32>;
  using t_i64_         = t_prefix_of<t_int64>;
  using t_i_           = t_prefix_of<t_i64_>;

  /////////////////////////////////////////////////////////////////////////////

  using t_size_        = t_prefix_of<__SIZE_TYPE__>;
  using T_size_        = T_prefix_of<t_size_>;
  using r_size_        = r_prefix_of<t_size_>;
  using p_size_        = p_prefix_of<t_size_>;
  using P_size_        = P_prefix_of<t_size_>;

  using t_byte_        = t_prefix_of<t_uchar>;
  using T_byte_        = T_prefix_of<t_byte_>;
  using r_byte_        = r_prefix_of<t_byte_>;
  using p_byte_        = p_prefix_of<t_byte_>;
  using P_byte_        = P_prefix_of<t_byte_>;

  using t_fd_          = t_prefix_of<t_int>;
  using T_fd_          = T_prefix_of<t_fd_>;
  using r_fd_          = r_prefix_of<t_fd_>;
  using p_fd_          = p_prefix_of<t_fd_>;
  using P_fd_          = P_prefix_of<t_fd_>;

  using t_cnt_         = t_prefix_of<t_ulong>;
  using T_cnt_         = T_prefix_of<t_cnt_>;
  using r_cnt_         = r_prefix_of<t_cnt_>;
  using p_cnt_         = p_prefix_of<t_cnt_>;
  using P_cnt_         = P_prefix_of<t_cnt_>;

  using t_n_           = t_prefix_of<t_ulong>;
  using T_n_           = T_prefix_of<t_n_>;
  using r_n_           = r_prefix_of<t_n_>;
  using p_n_           = p_prefix_of<t_n_>;
  using P_n_           = P_prefix_of<t_n_>;

  using t_ix_          = t_prefix_of<t_n_>;
  using T_ix_          = T_prefix_of<t_ix_>;
  using r_ix_          = r_prefix_of<t_ix_>;
  using p_ix_          = p_prefix_of<t_ix_>;
  using P_ix_          = P_prefix_of<t_ix_>;

  using t_validity_    = t_prefix_of<t_bool>;
  using T_validity_    = T_prefix_of<t_validity_>;
  using r_validity_    = r_prefix_of<t_validity_>;
  using p_validity_    = p_prefix_of<t_validity_>;
  using P_validity_    = P_prefix_of<t_validity_>;

  using t_percentage_  = t_prefix_of<t_uchar>;
  using T_percentage_  = T_prefix_of<t_percentage_>;
  using r_percentage_  = r_prefix_of<t_percentage_>;
  using p_percentage_  = p_prefix_of<t_percentage_>;
  using P_percentage_  = P_prefix_of<t_percentage_>;

  using t_seq_no_      = t_prefix_of<t_uint>;
  using T_seq_no_      = T_prefix_of<t_seq_no_>;
  using r_seq_no_      = r_prefix_of<t_seq_no_>;
  using p_seq_no_      = p_prefix_of<t_seq_no_>;
  using P_seq_no_      = P_prefix_of<t_seq_no_>;

  using t_errn_        = t_prefix_of<t_int>;
  using T_errn_        = T_prefix_of<t_errn_>;
  using r_errn_        = r_prefix_of<t_errn_>;
  using p_errn_        = p_prefix_of<t_errn_>;
  using P_errn_        = P_prefix_of<t_errn_>;

  using t_nsec_        = t_prefix_of<t_ullong>;
  using T_nsec_        = T_prefix_of<t_nsec_>;
  using r_nsec_        = r_prefix_of<t_nsec_>;
  using p_nsec_        = p_prefix_of<t_nsec_>;
  using P_nsec_        = P_prefix_of<t_nsec_>;

  using t_usec_        = t_prefix_of<t_ulong>;
  using T_usec_        = T_prefix_of<t_usec_>;
  using r_usec_        = r_prefix_of<t_usec_>;
  using p_usec_        = p_prefix_of<t_usec_>;
  using P_usec_        = P_prefix_of<t_usec_>;

  using t_msec_        = t_prefix_of<t_uint>;
  using T_msec_        = T_prefix_of<t_msec_>;
  using r_msec_        = r_prefix_of<t_msec_>;
  using p_msec_        = p_prefix_of<t_msec_>;
  using P_msec_        = P_prefix_of<t_msec_>;

  using t_sec_         = t_prefix_of<t_uint>;
  using T_sec_         = T_prefix_of<t_sec_>;
  using r_sec_         = r_prefix_of<t_sec_>;
  using p_sec_         = p_prefix_of<t_sec_>;
  using P_sec_         = P_prefix_of<t_sec_>;

  using t_min_         = t_prefix_of<t_ushort>;
  using T_min_         = T_prefix_of<t_min_>;
  using r_min_         = r_prefix_of<t_min_>;
  using p_min_         = p_prefix_of<t_min_>;
  using P_min_         = P_prefix_of<t_min_>;

  using t_ticks_       = t_prefix_of<t_uint64>;
  using T_ticks_       = T_prefix_of<t_ticks_>;
  using r_ticks_       = r_prefix_of<t_ticks_>;
  using p_ticks_       = p_prefix_of<t_ticks_>;
  using P_ticks_       = P_prefix_of<t_ticks_>;

  /////////////////////////////////////////////////////////////////////////////

  using p_cstr_        = p_char;
  using P_cstr_        = P_char;
  using t_cstr_ptr_    = p_cstr_;
  using t_cstr_cptr_   = P_cstr_;

  /////////////////////////////////////////////////////////////////////////////

  constexpr t_nullptr     NULLPTR = nullptr;

  constexpr t_validity_    VALID_ = true;
  constexpr t_validity_  INVALID_ = false;

  constexpr t_cstr_ptr_  NO_CSTR_PTR_  = nullptr;
  constexpr t_cstr_cptr_ NO_CSTR_CPTR_ = nullptr;

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

  enum  t_size_tag_       { };
  enum  t_min_size_tag_   { };
  enum  t_max_size_tag_   { };
  enum  t_byte_tag_       { };
  enum  t_fd_tag_         { };
  enum  t_cnt_tag_        { };
  enum  t_n_tag_          { };
  enum  t_min_n_tag_      { };
  enum  t_max_n_tag_      { };
  enum  t_ix_tag_         { };
  enum  t_begin_ix_tag_   { };
  enum  t_end_ix_tag_     { };
  enum  t_validity_tag_   { };
  enum  t_cstr_tag_       { };
  enum  t_percentage_tag_ { };
  enum  t_seq_no_tag_     { };
  enum  t_nsec_tag_       { };
  enum  t_usec_tag_       { };
  enum  t_msec_tag_       { };
  enum  t_sec_tag_        { };
  enum  t_min_tag_        { };
  enum  t_ticks_tag_      { };
  enum  t_errn_tag_       { };

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

  template<typename T> struct t_add_SIZEOF {
    constexpr static t_size_ SIZEOF = sizeof(T);
  };

  template<typename T> constexpr auto SIZEOF_of = T::SIZEOF;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T> struct t_add_BITS {
    constexpr static t_size_ BITS = sizeof(T) * __CHAR_BIT__;
  };

  template<typename T> constexpr auto BITS_of = T::BITS;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, T V, typename H = t_dummy>
  struct t_constant : t_add_value<T>,
                      t_add_VALUE<T, V>,
                      t_add_identity<H> {
  };

  /////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
