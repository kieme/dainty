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
///////////////////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////////////

  using t_bool         = t_prefix<bool>::t_;
  using T_bool         = t_prefix<t_bool>::T_;
  using r_bool         = t_prefix<t_bool>::r_;
  using p_bool         = t_prefix<t_bool>::p_;
  using P_bool         = t_prefix<t_bool>::P_;

  using t_char         = t_prefix<char>::t_;
  using T_char         = t_prefix<t_char>::T_;
  using r_char         = t_prefix<t_char>::r_;
  using p_char         = t_prefix<t_char>::p_;
  using P_char         = t_prefix<t_char>::P_;

  using t_schar        = t_prefix<signed char>::t_;
  using T_schar        = t_prefix<t_schar>::T_;
  using r_schar        = t_prefix<t_schar>::r_;
  using p_schar        = t_prefix<t_schar>::p_;
  using P_schar        = t_prefix<t_schar>::P_;

  using t_uchar        = t_prefix<unsigned char>::t_;
  using T_uchar        = t_prefix<t_uchar>::T_;
  using r_uchar        = t_prefix<t_uchar>::r_;
  using p_uchar        = t_prefix<t_uchar>::p_;
  using P_uchar        = t_prefix<t_uchar>::P_;

  using t_int          = t_prefix<int>::t_;
  using T_int          = t_prefix<int>::T_;
  using r_int          = t_prefix<t_int>::r_;
  using p_int          = t_prefix<t_int>::p_;
  using P_int          = t_prefix<t_int>::P_;

  using t_uint         = t_prefix<unsigned int>::t_;
  using T_uint         = t_prefix<t_uint>::T_;
  using r_uint         = t_prefix<t_uint>::r_;
  using p_uint         = t_prefix<t_uint>::p_;
  using P_uint         = t_prefix<t_uint>::P_;

  using t_short        = t_prefix<short>::t_;
  using T_short        = t_prefix<t_short>::T_;
  using r_short        = t_prefix<t_short>::r_;
  using p_short        = t_prefix<t_short>::p_;
  using P_short        = t_prefix<t_short>::P_;

  using t_ushort       = t_prefix<unsigned short>::t_;
  using T_ushort       = t_prefix<t_ushort>::T_;
  using r_ushort       = t_prefix<t_ushort>::r_;
  using p_ushort       = t_prefix<t_ushort>::p_;
  using P_ushort       = t_prefix<t_ushort>::P_;

  using t_long         = t_prefix<long int>::t_;
  using T_long         = t_prefix<t_long>::T_;
  using r_long         = t_prefix<t_long>::r_;
  using p_long         = t_prefix<t_long>::p_;
  using P_long         = t_prefix<t_long>::P_;

  using t_ulong        = t_prefix<unsigned long int>::t_;
  using T_ulong        = t_prefix<t_ulong>::T_;
  using r_ulong        = t_prefix<t_ulong>::r_;
  using p_ulong        = t_prefix<t_ulong>::p_;
  using P_ulong        = t_prefix<t_ulong>::P_;

  using t_llong        = t_prefix<long long int>::t_;
  using T_llong        = t_prefix<t_llong>::T_;
  using r_llong        = t_prefix<t_llong>::r_;
  using p_llong        = t_prefix<t_llong>::p_;
  using P_llong        = t_prefix<t_llong>::P_;

  using t_ullong       = t_prefix<unsigned long long int>::t_;
  using T_ullong       = t_prefix<t_ullong>::T_;
  using r_ullong       = t_prefix<t_ullong>::r_;
  using p_ullong       = t_prefix<t_ullong>::p_;
  using P_ullong       = t_prefix<t_ullong>::P_;

  using t_double       = t_prefix<double>::t_;
  using T_double       = t_prefix<t_double>::T_;
  using r_double       = t_prefix<t_double>::r_;
  using p_double       = t_prefix<t_double>::p_;
  using P_double       = t_prefix<t_double>::P_;

  using t_void         = t_prefix<void>::t_;
  using p_void         = t_prefix<t_void>::p_;
  using P_void         = t_prefix<t_void>::P_;

  using t_int8         = t_prefix<std::int8_t>::t_;
  using T_int8         = t_prefix<t_int8>::T_;
  using r_int8         = t_prefix<t_int8>::r_;
  using p_int8         = t_prefix<t_int8>::p_;
  using P_int8         = t_prefix<t_int8>::P_;

  using t_int16        = t_prefix<std::int16_t>::t_;
  using T_int16        = t_prefix<t_int16>::T_;
  using r_int16        = t_prefix<t_int16>::r_;
  using p_int16        = t_prefix<t_int16>::p_;
  using P_int16        = t_prefix<t_int16>::P_;

  using t_int32        = t_prefix<std::int32_t>::t_;
  using T_int32        = t_prefix<t_int32>::T_;
  using r_int32        = t_prefix<t_int32>::r_;
  using p_int32        = t_prefix<t_int32>::p_;
  using P_int32        = t_prefix<t_int32>::P_;

  using t_int64        = t_prefix<std::int64_t>::t_;
  using T_int64        = t_prefix<t_int64>::T_;
  using r_int64        = t_prefix<t_int64>::r_;
  using p_int64        = t_prefix<t_int64>::p_;
  using P_int64        = t_prefix<t_int64>::P_;

  using t_uint8        = t_prefix<std::uint8_t>::t_;
  using T_uint8        = t_prefix<t_uint8>::T_;
  using r_uint8        = t_prefix<t_uint8>::r_;
  using p_uint8        = t_prefix<t_uint8>::p_;
  using P_uint8        = t_prefix<t_uint8>::P_;

  using t_uint16       = t_prefix<std::uint16_t>::t_;
  using T_uint16       = t_prefix<t_uint16>::T_;
  using r_uint16       = t_prefix<t_uint16>::r_;
  using p_uint16       = t_prefix<t_uint16>::p_;
  using P_uint16       = t_prefix<t_uint16>::P_;

  using t_uint32       = t_prefix<std::uint32_t>::t_;
  using T_uint32       = t_prefix<t_uint32>::T_;
  using r_uint32       = t_prefix<t_uint32>::r_;
  using p_uint32       = t_prefix<t_uint32>::p_;
  using P_uint32       = t_prefix<t_uint32>::P_;

  using t_uint64       = t_prefix<std::uint64_t>::t_;
  using T_uint64       = t_prefix<t_uint64>::T_;
  using r_uint64       = t_prefix<t_uint64>::r_;
  using R_uint64       = t_prefix<t_uint64>::R_;
  using p_uint64       = t_prefix<t_uint64>::p_;
  using P_uint64       = t_prefix<t_uint64>::P_;

  using t_least8       = t_prefix<std::int_least8_t>::t_;
  using T_least8       = t_prefix<t_least8>::T_;
  using r_least8       = t_prefix<t_least8>::r_;
  using p_least8       = t_prefix<t_least8>::p_;
  using P_least8       = t_prefix<t_least8>::P_;

  using t_least16      = t_prefix<std::int_least16_t>::t_;
  using T_least16      = t_prefix<t_least16>::T_;
  using r_least16      = t_prefix<t_least16>::r_;
  using p_least16      = t_prefix<t_least16>::p_;
  using P_least16      = t_prefix<t_least16>::P_;

  using t_least32      = t_prefix<std::int_least32_t>::t_;
  using T_least32      = t_prefix<t_least32>::T_;
  using r_least32      = t_prefix<t_least32>::r_;
  using p_least32      = t_prefix<t_least32>::p_;
  using P_least32      = t_prefix<t_least32>::P_;

  using t_least64      = t_prefix<std::int_least64_t>::t_;
  using T_least64      = t_prefix<t_least64>::T_;
  using r_least64      = t_prefix<t_least64>::r_;
  using p_least64      = t_prefix<t_least64>::p_;
  using P_least64      = t_prefix<t_least64>::P_;

  using t_uleast8      = t_prefix<std::uint_least8_t>::t_;
  using T_uleast8      = t_prefix<t_uleast8>::T_;
  using r_uleast8      = t_prefix<t_uleast8>::r_;
  using p_uleast8      = t_prefix<t_uleast8>::p_;
  using P_uleast8      = t_prefix<t_uleast8>::P_;

  using t_uleast16     = t_prefix<std::uint_least16_t>::t_;
  using T_uleast16     = t_prefix<t_uleast16>::T_;
  using r_uleast16     = t_prefix<t_uleast16>::r_;
  using p_uleast16     = t_prefix<t_uleast16>::p_;
  using P_uleast16     = t_prefix<t_uleast16>::P_;

  using t_uleast32     = t_prefix<std::uint_least32_t>::t_;
  using T_uleast32     = t_prefix<t_uleast32>::T_;
  using r_uleast32     = t_prefix<t_uleast32>::r_;
  using p_uleast32     = t_prefix<t_uleast32>::p_;
  using P_uleast32     = t_prefix<t_uleast32>::P_;

  using t_uleast64     = t_prefix<std::uint_least64_t>::t_;
  using T_uleast64     = t_prefix<t_uleast64>::T_;
  using r_uleast64     = t_prefix<t_uleast64>::r_;
  using p_uleast64     = t_prefix<t_uleast64>::p_;
  using P_uleast64     = t_prefix<t_uleast64>::P_;

  using t_fast8        = t_prefix<std::int_fast8_t>::t_;
  using T_fast8        = t_prefix<t_fast8>::T_;
  using r_fast8        = t_prefix<t_fast8>::r_;
  using p_fast8        = t_prefix<t_fast8>::p_;
  using P_fast8        = t_prefix<t_fast8>::P_;

  using t_fast16       = t_prefix<std::int_fast16_t>::t_;
  using T_fast16       = t_prefix<t_fast16>::T_;
  using r_fast16       = t_prefix<t_fast16>::r_;
  using p_fast16       = t_prefix<t_fast16>::p_;
  using P_fast16       = t_prefix<t_fast16>::P_;

  using t_fast32       = t_prefix<std::int_fast32_t>::t_;
  using T_fast32       = t_prefix<t_fast32>::T_;
  using r_fast32       = t_prefix<t_fast32>::r_;
  using p_fast32       = t_prefix<t_fast32>::p_;
  using P_fast32       = t_prefix<t_fast32>::P_;

  using t_fast64       = t_prefix<std::int_fast64_t>::t_;
  using T_fast64       = t_prefix<t_fast64>::T_;
  using r_fast64       = t_prefix<t_fast64>::r_;
  using p_fast64       = t_prefix<t_fast64>::p_;
  using P_fast64       = t_prefix<t_fast64>::P_;

  using t_ufast8       = t_prefix<std::uint_fast8_t>::t_;
  using T_ufast8       = t_prefix<t_ufast8>::T_;
  using r_ufast8       = t_prefix<t_ufast8>::r_;
  using p_ufast8       = t_prefix<t_ufast8>::p_;
  using P_ufast8       = t_prefix<t_ufast8>::P_;

  using t_ufast16      = t_prefix<std::uint_fast16_t>::t_;
  using T_ufast16      = t_prefix<t_ufast16>::T_;
  using r_ufast16      = t_prefix<t_ufast16>::r_;
  using R_ufast16      = t_prefix<t_ufast16>::R_;
  using p_ufast16      = t_prefix<t_ufast16>::p_;
  using P_ufast16      = t_prefix<t_ufast16>::P_;

  using t_ufast32      = t_prefix<std::uint_fast32_t>::t_;
  using T_ufast32      = t_prefix<t_ufast32>::T_;
  using r_ufast32      = t_prefix<t_ufast32>::r_;
  using p_ufast32      = t_prefix<t_ufast32>::p_;
  using P_ufast32      = t_prefix<t_ufast32>::P_;

  using t_ufast64      = t_prefix<std::uint_fast64_t>::t_;
  using T_ufast64      = t_prefix<t_ufast64>::T_;
  using r_ufast64      = t_prefix<t_ufast64>::r_;
  using p_ufast64      = t_prefix<t_ufast64>::p_;
  using P_ufast64      = t_prefix<t_ufast64>::P_;

  using t_uintptr      = t_prefix<std::uintptr_t>::t_;
  using T_uintptr      = t_prefix<t_uintptr>::T_;
  using r_uintptr      = t_prefix<t_uintptr>::r_;
  using p_uintptr      = t_prefix<t_uintptr>::p_;
  using P_uintptr      = t_prefix<t_uintptr>::P_;

///////////////////////////////////////////////////////////////////////////////

  enum  t_size_tag_       {};
  enum  t_min_size_tag_   {};
  enum  t_max_size_tag_   {};
  enum  t_byte_tag_       {};
  enum  t_fd_tag_         {};
  enum  t_cnt_tag_        {};
  enum  t_n_tag_          {};
  enum  t_min_n_tag_      {};
  enum  t_max_n_tag_      {};
  enum  t_ix_tag_         {};
  enum  t_begin_ix_tag_   {};
  enum  t_end_ix_tag_     {};
  enum  t_validity_tag_   {};
  enum  t_cstr_tag_       {};
  enum  t_percentage_tag_ {};
  enum  t_seq_no_tag_     {};
  enum  t_nsec_tag_       {};
  enum  t_usec_tag_       {};
  enum  t_msec_tag_       {};
  enum  t_sec_tag_        {};
  enum  t_min_tag_        {};
  enum  t_ticks_tag_      {};
  enum  t_errn_tag_       {};

///////////////////////////////////////////////////////////////////////////////

  using t_size_        = __SIZE_TYPE__;
  using T_size_        = t_prefix<t_size_>::T_;
  using r_size_        = t_prefix<t_size_>::r_;
  using p_size_        = t_prefix<t_size_>::p_;
  using P_size_        = t_prefix<t_size_>::P_;

  using t_nullptr      = decltype(nullptr);

///////////////////////////////////////////////////////////////////////////////

  using t_byte_        = t_uchar;
  using T_byte_        = t_prefix<t_byte_>::T_;
  using r_byte_        = t_prefix<t_byte_>::r_;
  using p_byte_        = t_prefix<t_byte_>::p_;
  using P_byte_        = t_prefix<t_byte_>::P_;

  using t_fd_          = t_int;
  using T_fd_          = t_prefix<t_fd_>::T_;
  using r_fd_          = t_prefix<t_fd_>::r_;
  using p_fd_          = t_prefix<t_fd_>::p_;
  using P_fd_          = t_prefix<t_fd_>::P_;

  using t_cnt_         = t_ulong;
  using T_cnt_         = t_prefix<t_cnt_>::T_;
  using r_cnt_         = t_prefix<t_cnt_>::r_;
  using p_cnt_         = t_prefix<t_cnt_>::p_;
  using P_cnt_         = t_prefix<t_cnt_>::P_;

  using t_n_           = t_ulong;
  using T_n_           = t_prefix<t_n_>::T_;
  using r_n_           = t_prefix<t_n_>::r_;
  using p_n_           = t_prefix<t_n_>::p_;
  using P_n_           = t_prefix<t_n_>::P_;

  using t_ix_          = t_n_;
  using T_ix_          = t_prefix<t_ix_>::T_;
  using r_ix_          = t_prefix<t_ix_>::r_;
  using p_ix_          = t_prefix<t_ix_>::p_;
  using P_ix_          = t_prefix<t_ix_>::P_;

  using t_validity_    = t_bool;
  using T_validity_    = t_prefix<t_validity_>::T_;
  using r_validity_    = t_prefix<t_validity_>::r_;
  using p_validity_    = t_prefix<t_validity_>::p_;
  using P_validity_    = t_prefix<t_validity_>::P_;

  using t_percentage_  = t_uchar;
  using T_percentage_  = t_prefix<t_percentage_>::T_;
  using r_percentage_  = t_prefix<t_percentage_>::r_;
  using p_percentage_  = t_prefix<t_percentage_>::p_;
  using P_percentage_  = t_prefix<t_percentage_>::P_;

  using t_seq_no_      = t_uint;
  using T_seq_no_      = t_prefix<t_seq_no_>::T_;
  using r_seq_no_      = t_prefix<t_seq_no_>::r_;
  using p_seq_no_      = t_prefix<t_seq_no_>::p_;
  using P_seq_no_      = t_prefix<t_seq_no_>::P_;

  using t_errn_        = t_int;
  using T_errn_        = t_prefix<t_errn_>::T_;
  using r_errn_        = t_prefix<t_errn_>::r_;
  using p_errn_        = t_prefix<t_errn_>::p_;
  using P_errn_        = t_prefix<t_errn_>::P_;

  using t_nsec_        = t_ullong;
  using T_nsec_        = t_prefix<t_nsec_>::T_;
  using r_nsec_        = t_prefix<t_nsec_>::r_;
  using p_nsec_        = t_prefix<t_nsec_>::p_;
  using P_nsec_        = t_prefix<t_nsec_>::P_;

  using t_usec_        = t_ulong;
  using T_usec_        = t_prefix<t_usec_>::T_;
  using r_usec_        = t_prefix<t_usec_>::r_;
  using p_usec_        = t_prefix<t_usec_>::p_;
  using P_usec_        = t_prefix<t_usec_>::P_;

  using t_msec_        = t_uint;
  using T_msec_        = t_prefix<t_msec_>::T_;
  using r_msec_        = t_prefix<t_msec_>::r_;
  using p_msec_        = t_prefix<t_msec_>::p_;
  using P_msec_        = t_prefix<t_msec_>::P_;

  using t_sec_         = t_uint;
  using T_sec_         = t_prefix<t_sec_>::T_;
  using r_sec_         = t_prefix<t_sec_>::r_;
  using p_sec_         = t_prefix<t_sec_>::p_;
  using P_sec_         = t_prefix<t_sec_>::P_;

  using t_min_         = t_ushort;
  using T_min_         = t_prefix<t_min_>::T_;
  using r_min_         = t_prefix<t_min_>::r_;
  using p_min_         = t_prefix<t_min_>::p_;
  using P_min_         = t_prefix<t_min_>::P_;

  using t_ticks_       = t_uint64;
  using T_ticks_       = t_prefix<t_ticks_>::T_;
  using r_ticks_       = t_prefix<t_ticks_>::r_;
  using p_ticks_       = t_prefix<t_ticks_>::p_;
  using P_ticks_       = t_prefix<t_ticks_>::P_;

///////////////////////////////////////////////////////////////////////////////

  using p_cstr_        = p_char;
  using P_cstr_        = P_char;
  using t_cstr_ptr_    = p_cstr_;
  using t_cstr_cptr_   = P_cstr_;

///////////////////////////////////////////////////////////////////////////////

  constexpr t_validity_    VALID_ = true;
  constexpr t_validity_  INVALID_ = false;

  constexpr t_cstr_ptr_  NO_CSTR_PTR_  = nullptr;
  constexpr t_cstr_cptr_ NO_CSTR_CPTR_ = nullptr;

  constexpr t_fd_        BAD_FD_   = -1;
  constexpr t_errn_      NO_ERRN_  =  0;
  constexpr t_errn_      BAD_ERRN_ = -1;

///////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
