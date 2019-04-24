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

#ifndef _DAINTY_NAMED_H_
#define _DAINTY_NAMED_H_

// named: 'give a name to'
//
//   <named> name builtin types and specify a simple naming convention
//   that should be followed by projects that use it.
//
//   the focus was on the embedded world that often use c++03.
//   (reason for not using type traits, constexpr, etc.). The API will
//   be changed to support c++17 features.
//
//   basic naming convention:
//
//     N01. use lowercase characters except for a few cases mentioned below.
//     N02. functions, method names must be verbs.
//     N03. classes, struct, types names must be nouns.
//     N04. literal and constant values should be written in capital letters.
//     N05. all types names are prefixed. (t_prefix<xxx>)
//     N06. types must be prefixed with t_.
//          (avoid conflict with std:: convention).
//     N07. pointer definitions must be prefixed p_/P_.
//     N08. lvalue reference definitions must be prefixed r_/R_.
//     N09. rvalue reference definitions must be prefixed x_.
//     N10. a capital prefix indicates the content is const.
//     N11. internal functions, methods, variables, types must have an
//          postfix _. API users should never use these.
//     N12. when using <dainty::named> builtin types on interfaces, use
//          specializations of t_explicit, to assign purpose to a type.
//     N13. use '_' as delimiter within long names to improve readability.
//     N14. use <dainty::named> builtin types instead of <std>.
//     N15. use qualified names or "using " when using <dainty::named>.
//     N16. prefix a const "type" name with c but not the parameter itself.
//           e.g. t_id  id; -> non-const id.
//                t_cid id; -> const id. but const is not identifyable with
//                             the variable name.
//     N17. when writing classes/structs using the following naming convention
//          for function parameters, data memebers and types.
//          // -> t_param, param, param_
//          e.g class t_class {
//              public:
//                using t_param = ... ;
//                void set(const t_param& param) {
//                  param_ = param;
//                }
//              private:
//                t_param param_; // param_ is internal
//              };
//     N18.1 when a naming conflict exit, prefix a variable with _.
//           struct t_pair {
//             t_first  first;   // first is not internal
//             t_second second;
//             t_pair(t_first _first, t_second _second)
//               : first(_first), second{_second) {
//             }
//           };
//
//   predefined t_explicit types for interface use:
//
//      Z01: use type t_ix that is intended to index into something.
//      Z02: use type t_n that is intended to represent a number of elements.
//      Z03: use t_validity to express if something is valid.
//
//   file naming convention:
//
//      F01: dainty_<namespace>.h of a namespace located withint dainty.
//      F02: dainty_<namespace>_<purpose>.h a functionality within namespace.
//      F03: the cpp files names are the same as the associated header files.
//           dainty_oops.h and dainty_oops.cpp
//      F04: example cpp files use the name of the functionality.
//           dainty_oops.cpp then dainty_oops_example<n>.cpp
//      F05: test cpp files use the name of the functionality.
//           dainty_oops.cpp then dainty_oops_test<n>.cpp
//
//   design naming: <xxx> is a functionality.
//
//     data:
//
//      D01: t_<xxx>_params - this is the params of a
//      D02: t_<xxx>_stats  - counter statit
//      D03: t_<xxx>_info   - both runtime, params and other visible properties.
//      D04: t_<xxx>_ctxt   - embeds info and containers private runtime properties.
//      D05: t_<xxx>_data   - encapculates all internal data and provide methods
//                       to consistently manipulate that data.
//
//     operations:
//
//      D10: t_<xxx>_logic - where specific logic is located.
//                           it typically embeds t_<xxx>_data.
//

// should set be allowed. its meant only to transfer data through an interface.
//
// t_explicit:
//
//   note: with t_explicit<> assign a purpose to a builtin type.
//
//   e.g. the demonstation use the classic set_point(int x, int y) example.
//
//   enum t_x_ { };
//   enum t_y_ { };
//   typedef named::t_explicit<named::t_uint, t_x_> t_xpoint;
//   typedef named::t_explicit<named::t_uint, t_y_> t_ypoint;
//
//   void set_point(t_xpoint x, t_ypoint y);
//   void set_point(int x, int y);
//
//   void set_point(5, 10);
//   void set_point(xpoint(5), ypoint(10));
//
//  Why is such a feature not part of the cpp language?
//
//    e.g. using t_xpoint = explicit unsigned int; // make type t_xpoint
//         using t_ypoint = explicit unsigned int; // make type t_ypoint
//
//    t_xpoint and t_ypoint are different types.
//
//    unsigned int x = t_xpoint(1) // ok
//    t_xpoint x = 1;              // nok
//    t_xpoint x(1);               // ok
//    t_xpoint x{1};               // ok
//    t_xpoint x(-1);              // nok
//    t_xpoint x{-1)}              // nok

#include <cstdint>

#define OK  dainty::named::VALID
#define NOK dainty::named::INVALID

namespace dainty
{
namespace named
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
  using R_bool         = t_prefix<t_bool>::R_;
  using p_bool         = t_prefix<t_bool>::p_;
  using P_bool         = t_prefix<t_bool>::P_;

  using t_char         = t_prefix<char>::t_;
  using T_char         = t_prefix<t_char>::T_;
  using r_char         = t_prefix<t_char>::r_;
  using R_char         = t_prefix<t_char>::R_;
  using p_char         = t_prefix<t_char>::p_;
  using P_char         = t_prefix<t_char>::P_;

  using t_schar        = t_prefix<signed char>::t_;
  using T_schar        = t_prefix<t_schar>::T_;
  using r_schar        = t_prefix<t_schar>::r_;
  using R_schar        = t_prefix<t_schar>::R_;
  using p_schar        = t_prefix<t_schar>::p_;
  using P_schar        = t_prefix<t_schar>::P_;

  using t_uchar        = t_prefix<unsigned char>::t_;
  using T_uchar        = t_prefix<t_uchar>::T_;
  using r_uchar        = t_prefix<t_uchar>::r_;
  using R_uchar        = t_prefix<t_uchar>::R_;
  using p_uchar        = t_prefix<t_uchar>::p_;
  using P_uchar        = t_prefix<t_uchar>::P_;

  using t_int          = t_prefix<int>::t_;
  using T_int          = t_prefix<int>::T_;
  using r_int          = t_prefix<t_int>::r_;
  using R_int          = t_prefix<t_int>::R_;
  using p_int          = t_prefix<t_int>::p_;
  using P_int          = t_prefix<t_int>::P_;

  using t_uint         = t_prefix<unsigned int>::t_;
  using T_uint         = t_prefix<t_uint>::T_;
  using r_uint         = t_prefix<t_uint>::r_;
  using R_uint         = t_prefix<t_uint>::R_;
  using p_uint         = t_prefix<t_uint>::p_;
  using P_uint         = t_prefix<t_uint>::P_;

  using t_short        = t_prefix<short>::t_;
  using T_short        = t_prefix<t_short>::T_;
  using r_short        = t_prefix<t_short>::r_;
  using R_short        = t_prefix<t_short>::R_;
  using p_short        = t_prefix<t_short>::p_;
  using P_short        = t_prefix<t_short>::P_;

  using t_ushort       = t_prefix<unsigned short>::t_;
  using T_ushort       = t_prefix<t_ushort>::T_;
  using r_ushort       = t_prefix<t_ushort>::r_;
  using R_ushort       = t_prefix<t_ushort>::R_;
  using p_ushort       = t_prefix<t_ushort>::p_;
  using P_ushort       = t_prefix<t_ushort>::P_;

  using t_long         = t_prefix<long int>::t_;
  using T_long         = t_prefix<t_long>::T_;
  using r_long         = t_prefix<t_long>::r_;
  using R_long         = t_prefix<t_long>::R_;
  using p_long         = t_prefix<t_long>::p_;
  using P_long         = t_prefix<t_long>::P_;

  using t_ulong        = t_prefix<unsigned long int>::t_;
  using T_ulong        = t_prefix<t_ulong>::T_;
  using r_ulong        = t_prefix<t_ulong>::r_;
  using R_ulong        = t_prefix<t_ulong>::R_;
  using p_ulong        = t_prefix<t_ulong>::p_;
  using P_ulong        = t_prefix<t_ulong>::P_;

  using t_llong        = t_prefix<long long int>::t_;
  using T_llong        = t_prefix<t_llong>::T_;
  using r_llong        = t_prefix<t_llong>::r_;
  using R_llong        = t_prefix<t_llong>::R_;
  using p_llong        = t_prefix<t_llong>::p_;
  using P_llong        = t_prefix<t_llong>::P_;

  using t_ullong       = t_prefix<unsigned long long int>::t_;
  using T_ullong       = t_prefix<t_ullong>::T_;
  using r_ullong       = t_prefix<t_ullong>::r_;
  using R_ullong       = t_prefix<t_ullong>::R_;
  using p_ullong       = t_prefix<t_ullong>::p_;
  using P_ullong       = t_prefix<t_ullong>::P_;

  using t_double       = t_prefix<double>::t_;
  using T_double       = t_prefix<t_double>::T_;
  using r_double       = t_prefix<t_double>::r_;
  using R_double       = t_prefix<t_double>::R_;
  using p_double       = t_prefix<t_double>::p_;
  using P_double       = t_prefix<t_double>::P_;

  using t_void         = t_prefix<void>::t_;
  using p_void         = t_prefix<t_void>::p_;
  using P_void         = t_prefix<t_void>::P_;

  using t_int8         = t_prefix<std::int8_t>::t_;
  using T_int8         = t_prefix<t_int8>::T_;
  using r_int8         = t_prefix<t_int8>::r_;
  using R_int8         = t_prefix<t_int8>::R_;
  using p_int8         = t_prefix<t_int8>::p_;
  using P_int8         = t_prefix<t_int8>::P_;

  using t_int16        = t_prefix<std::int16_t>::t_;
  using T_int16        = t_prefix<t_int16>::T_;
  using r_int16        = t_prefix<t_int16>::r_;
  using R_int16        = t_prefix<t_int16>::R_;
  using p_int16        = t_prefix<t_int16>::p_;
  using P_int16        = t_prefix<t_int16>::P_;

  using t_int32        = t_prefix<std::int32_t>::t_;
  using T_int32        = t_prefix<t_int32>::T_;
  using r_int32        = t_prefix<t_int32>::r_;
  using R_int32        = t_prefix<t_int32>::R_;
  using p_int32        = t_prefix<t_int32>::p_;
  using P_int32        = t_prefix<t_int32>::P_;

  using t_int64        = t_prefix<std::int64_t>::t_;
  using T_int64        = t_prefix<t_int64>::T_;
  using r_int64        = t_prefix<t_int64>::r_;
  using R_int64        = t_prefix<t_int64>::R_;
  using p_int64        = t_prefix<t_int64>::p_;
  using P_int64        = t_prefix<t_int64>::P_;

  using t_uint8        = t_prefix<std::uint8_t>::t_;
  using T_uint8        = t_prefix<t_uint8>::T_;
  using r_uint8        = t_prefix<t_uint8>::r_;
  using R_uint8        = t_prefix<t_uint8>::R_;
  using p_uint8        = t_prefix<t_uint8>::p_;
  using P_uint8        = t_prefix<t_uint8>::P_;

  using t_uint16       = t_prefix<std::uint16_t>::t_;
  using T_uint16       = t_prefix<t_uint16>::T_;
  using r_uint16       = t_prefix<t_uint16>::r_;
  using R_uint16       = t_prefix<t_uint16>::R_;
  using p_uint16       = t_prefix<t_uint16>::p_;
  using P_uint16       = t_prefix<t_uint16>::P_;

  using t_uint32       = t_prefix<std::uint32_t>::t_;
  using T_uint32       = t_prefix<t_uint32>::T_;
  using r_uint32       = t_prefix<t_uint32>::r_;
  using R_uint32       = t_prefix<t_uint32>::R_;
  using p_uint32       = t_prefix<t_uint32>::p_;
  using P_uint32       = t_prefix<t_uint32>::P_;

  using t_uint64       = t_prefix<std::uint64_t>::t_;
  using T_uint64       = t_prefix<t_uint64>::T_;
  using r_uint64       = t_prefix<t_uint64>::r_;
  using R_uint64       = t_prefix<t_uint64>::R_;
  using p_uint64       = t_prefix<t_uint64>::p_;
  using P_uint64       = t_prefix<t_uint64>::P_;

  using t_int_least8   = t_prefix<std::int_least8_t>::t_;
  using T_int_least8   = t_prefix<t_int_least8>::T_;
  using r_int_least8   = t_prefix<t_int_least8>::r_;
  using R_int_least8   = t_prefix<t_int_least8>::R_;
  using p_int_least8   = t_prefix<t_int_least8>::p_;
  using P_int_least8   = t_prefix<t_int_least8>::P_;

  using t_int_least16  = t_prefix<std::int_least16_t>::t_;
  using T_int_least16  = t_prefix<t_int_least16>::T_;
  using r_int_least16  = t_prefix<t_int_least16>::r_;
  using R_int_least16  = t_prefix<t_int_least16>::R_;
  using p_int_least16  = t_prefix<t_int_least16>::p_;
  using P_int_least16  = t_prefix<t_int_least16>::P_;

  using t_int_least32  = t_prefix<std::int_least32_t>::t_;
  using T_int_least32  = t_prefix<t_int_least32>::T_;
  using r_int_least32  = t_prefix<t_int_least32>::r_;
  using R_int_least32  = t_prefix<t_int_least32>::R_;
  using p_int_least32  = t_prefix<t_int_least32>::p_;
  using P_int_least32  = t_prefix<t_int_least32>::P_;

  using t_int_least64  = t_prefix<std::int_least64_t>::t_;
  using T_int_least64  = t_prefix<t_int_least64>::T_;
  using r_int_least64  = t_prefix<t_int_least64>::r_;
  using R_int_least64  = t_prefix<t_int_least64>::R_;
  using p_int_least64  = t_prefix<t_int_least64>::p_;
  using P_int_least64  = t_prefix<t_int_least64>::P_;

  using t_uint_least8  = t_prefix<std::uint_least8_t>::t_;
  using T_uint_least8  = t_prefix<t_uint_least8>::T_;
  using r_uint_least8  = t_prefix<t_uint_least8>::r_;
  using R_uint_least8  = t_prefix<t_uint_least8>::R_;
  using p_uint_least8  = t_prefix<t_uint_least8>::p_;
  using P_uint_least8  = t_prefix<t_uint_least8>::P_;

  using t_uint_least16 = t_prefix<std::uint_least16_t>::t_;
  using T_uint_least16 = t_prefix<t_uint_least16>::T_;
  using r_uint_least16 = t_prefix<t_uint_least16>::r_;
  using R_uint_least16 = t_prefix<t_uint_least16>::R_;
  using p_uint_least16 = t_prefix<t_uint_least16>::p_;
  using P_uint_least16 = t_prefix<t_uint_least16>::P_;

  using t_uint_least32 = t_prefix<std::uint_least32_t>::t_;
  using T_uint_least32 = t_prefix<t_uint_least32>::T_;
  using r_uint_least32 = t_prefix<t_uint_least32>::r_;
  using R_uint_least32 = t_prefix<t_uint_least32>::R_;
  using p_uint_least32 = t_prefix<t_uint_least32>::p_;
  using P_uint_least32 = t_prefix<t_uint_least32>::P_;

  using t_uint_least64 = t_prefix<std::uint_least64_t>::t_;
  using T_uint_least64 = t_prefix<t_uint_least64>::T_;
  using r_uint_least64 = t_prefix<t_uint_least64>::r_;
  using R_uint_least64 = t_prefix<t_uint_least64>::R_;
  using p_uint_least64 = t_prefix<t_uint_least64>::p_;
  using P_uint_least64 = t_prefix<t_uint_least64>::P_;

  using t_int_fast8    = t_prefix<std::int_fast8_t>::t_;
  using T_int_fast8    = t_prefix<t_int_fast8>::T_;
  using r_int_fast8    = t_prefix<t_int_fast8>::r_;
  using R_int_fast8    = t_prefix<t_int_fast8>::R_;
  using p_int_fast8    = t_prefix<t_int_fast8>::p_;
  using P_int_fast8    = t_prefix<t_int_fast8>::P_;

  using t_int_fast16   = t_prefix<std::int_fast16_t>::t_;
  using T_int_fast16   = t_prefix<t_int_fast16>::T_;
  using r_int_fast16   = t_prefix<t_int_fast16>::r_;
  using R_int_fast16   = t_prefix<t_int_fast16>::R_;
  using p_int_fast16   = t_prefix<t_int_fast16>::p_;
  using P_int_fast16   = t_prefix<t_int_fast16>::P_;

  using t_int_fast32   = t_prefix<std::int_fast32_t>::t_;
  using T_int_fast32   = t_prefix<t_int_fast32>::T_;
  using r_int_fast32   = t_prefix<t_int_fast32>::r_;
  using R_int_fast32   = t_prefix<t_int_fast32>::R_;
  using p_int_fast32   = t_prefix<t_int_fast32>::p_;
  using P_int_fast32   = t_prefix<t_int_fast32>::P_;

  using t_int_fast64   = t_prefix<std::int_fast64_t>::t_;
  using T_int_fast64   = t_prefix<t_int_fast64>::T_;
  using r_int_fast64   = t_prefix<t_int_fast64>::r_;
  using R_int_fast64   = t_prefix<t_int_fast64>::R_;
  using p_int_fast64   = t_prefix<t_int_fast64>::p_;
  using P_int_fast64   = t_prefix<t_int_fast64>::P_;

  using t_uint_fast8   = t_prefix<std::uint_fast8_t>::t_;
  using T_uint_fast8   = t_prefix<t_uint_fast8>::T_;
  using r_uint_fast8   = t_prefix<t_uint_fast8>::r_;
  using R_uint_fast8   = t_prefix<t_uint_fast8>::R_;
  using p_uint_fast8   = t_prefix<t_uint_fast8>::p_;
  using P_uint_fast8   = t_prefix<t_uint_fast8>::P_;

  using t_uint_fast16  = t_prefix<std::uint_fast16_t>::t_;
  using T_uint_fast16  = t_prefix<t_uint_fast16>::T_;
  using r_uint_fast16  = t_prefix<t_uint_fast16>::r_;
  using R_uint_fast16  = t_prefix<t_uint_fast16>::R_;
  using p_uint_fast16  = t_prefix<t_uint_fast16>::p_;
  using P_uint_fast16  = t_prefix<t_uint_fast16>::P_;

  using t_uint_fast32  = t_prefix<std::uint_fast32_t>::t_;
  using T_uint_fast32  = t_prefix<t_uint_fast32>::T_;
  using r_uint_fast32  = t_prefix<t_uint_fast32>::r_;
  using R_uint_fast32  = t_prefix<t_uint_fast32>::R_;
  using p_uint_fast32  = t_prefix<t_uint_fast32>::p_;
  using P_uint_fast32  = t_prefix<t_uint_fast32>::P_;

  using t_uint_fast64  = t_prefix<std::uint_fast64_t>::t_;
  using T_uint_fast64  = t_prefix<t_uint_fast64>::T_;
  using r_uint_fast64  = t_prefix<t_uint_fast64>::r_;
  using R_uint_fast64  = t_prefix<t_uint_fast64>::R_;
  using p_uint_fast64  = t_prefix<t_uint_fast64>::p_;
  using P_uint_fast64  = t_prefix<t_uint_fast64>::P_;

  using t_uintptr      = t_prefix<std::uintptr_t>::t_;
  using T_uintptr      = t_prefix<t_uintptr>::T_;
  using r_uintptr      = t_prefix<t_uintptr>::r_;
  using R_uintptr      = t_prefix<t_uintptr>::R_;
  using p_uintptr      = t_prefix<t_uintptr>::p_;
  using P_uintptr      = t_prefix<t_uintptr>::P_;

///////////////////////////////////////////////////////////////////////////////

  template<typename T> struct t_explicit_ { };

  template<> struct t_explicit_<t_bool>   { using t_type_ = t_bool;   };
  template<> struct t_explicit_<t_char>   { using t_type_ = t_char;   };
  template<> struct t_explicit_<t_uchar>  { using t_type_ = t_uchar;  };
  template<> struct t_explicit_<t_int>    { using t_type_ = t_int;    };
  template<> struct t_explicit_<t_uint>   { using t_type_ = t_uint;   };
  template<> struct t_explicit_<t_short>  { using t_type_ = t_short;  };
  template<> struct t_explicit_<t_ushort> { using t_type_ = t_ushort; };
  template<> struct t_explicit_<t_long>   { using t_type_ = t_long;   };
  template<> struct t_explicit_<t_ulong>  { using t_type_ = t_ulong;  };
  template<> struct t_explicit_<t_llong>  { using t_type_ = t_llong;  };
  template<> struct t_explicit_<t_ullong> { using t_type_ = t_ullong; };
  template<> struct t_explicit_<t_double> { using t_type_ = t_double; };
  template<> struct t_explicit_<t_void>   { using t_type_ = t_void;   };

  template<typename T>
  struct t_explicit_<T*> {
    using t_type_ = typename t_explicit_<T>::t_type_*;
  };

  template<typename T>
  struct t_explicit_<const T*> {
    using t_type_ = const typename t_explicit_<T>::t_type_*;
  };

  template<typename T>
  struct t_explicit_<T* const> {
    using t_type_ = typename t_explicit_<T>::t_type_* const;
  };

  template<typename T>
  struct t_explicit_<const T* const> {
    using t_type_ = const typename t_explicit_<T>::t_type_* const;
  };

  template<typename T> struct t_explicit_<T**>;

///////////////////////////////////////////////////////////////////////////////

  template<class, class, class> class t_explicit;
  template<class T, class TAG, class CHECK>
  constexpr T get(t_explicit<T, TAG, CHECK>) noexcept;

  template<class T, class TAG>
  struct t_check {
    static constexpr T test(T t) { return t; }
  };

  template<class T, class TAG, class CHECK = void>
  class t_explicit {
  public:
    using t_value    = typename t_explicit_<T>::t_type_;
    using t_tag      = TAG;
    using t_check    = CHECK;

    constexpr
    explicit t_explicit(t_value value)
#ifndef DAINTY_EXPLICIT_CHECK
      : value_{value}
#else
      : value_{CHECK::test(value)}
#endif
    { }

    template<class T1, class CHECK1>
    constexpr
    explicit t_explicit(t_explicit<T1, TAG, CHECK1> value)
#ifndef DAINTY_EXPLICIT_CHECK
      : value_{value}
#else
      : value_{CHECK::test(value.value_)}
#endif
    { }

    template<class T1, class CHECK1>
    constexpr
    t_explicit& operator=(t_explicit<T1, TAG, CHECK1> value) {
#ifndef DAINTY_EXPLICIT_CHECK
      value_ = value;
#else
      value_ = CHECK::test(value.value_);
#endif
    }

    t_explicit() = delete;                              // for clarity
    t_explicit(const t_explicit&) = default;            // for clarity
    t_explicit& operator=(const t_explicit&) = default; // for clarity

  private:
    template<class T1, class TAG1, class CHECK1> friend class t_explicit;
    template<class T1, class TAG1, class CHECK1>
    friend constexpr T1  get(t_explicit<T1, TAG1, CHECK1>) noexcept;
    t_value value_;
  };

///////////////////////////////////////////////////////////////////////////////

  template<class T, class TAG>
  class t_explicit<T, TAG, void> {
  public:
    using t_value = typename t_explicit_<T>::t_type_;
    using t_tag   = TAG;
    using t_check = void;

    constexpr
    explicit t_explicit(t_value value) noexcept : value_{value} {
    }

    template<class T1, class CHECK1>
    constexpr
    explicit t_explicit(t_explicit<T1, TAG, CHECK1> value) noexcept
      : value_{value} {
    }

    template<class T1, class CHECK1>
    constexpr
    t_explicit& operator=(t_explicit<T1, TAG, CHECK1> value) noexcept {
      value_ = value;
      return *this;
    }

    t_explicit() = delete;                              // for clarity
    t_explicit(const t_explicit&) = default;            // for clarity
    t_explicit& operator=(const t_explicit&) = default; // for clarity

  private:
    template<class T1, class TAG1, class CHECK1> friend class t_explicit;
    template<class T1, class TAG1, class CHECK1>
    friend constexpr T1 get(t_explicit<T1, TAG1, CHECK1>) noexcept;
    t_value value_;
  };

///////////////////////////////////////////////////////////////////////////////

  template<class T, class TAG, class CHECK>
  constexpr
  T get(t_explicit<T, TAG, CHECK> value) noexcept { return value.value_; }

///////////////////////////////////////////////////////////////////////////////

  template<class T, class TAG, class CHECK>
  constexpr
  t_bool operator==(t_explicit<T, TAG, CHECK> lh,
                    t_explicit<T, TAG, CHECK> rh) noexcept {
    return get(lh) == get(rh);
  }

  template<class T, class TAG, class CHECK>
  constexpr
  t_bool operator!=(t_explicit<T, TAG, CHECK> lh,
                    t_explicit<T, TAG, CHECK> rh) noexcept {
    return get(lh) != get(rh);
  }

///////////////////////////////////////////////////////////////////////////////

  template<class> struct t_is_explicit_;

  template<class T, class TAG, class CHECK>
  struct t_is_explicit_<t_explicit<T, TAG, CHECK>> {
    using t_result_ = t_explicit<T, TAG, CHECK>;
  };

  template<class E, class T, class TAG, class CHECK>
  constexpr
  E transform(t_explicit<T, TAG, CHECK> src) {
    return typename t_is_explicit_<E>::t_result_{get(src)};
  }

///////////////////////////////////////////////////////////////////////////////

  enum t_byte_tag_ { };
  using t_byte_       = t_uchar;
  using t_byte        = t_explicit<t_byte_, t_byte_tag_>;
  using T_byte        = t_prefix<t_byte>::T_;
  using r_byte        = t_prefix<t_byte>::r_;
  using R_byte        = t_prefix<t_byte>::R_;
  using p_byte        = t_prefix<t_byte>::p_;
  using P_byte        = t_prefix<t_byte>::P_;

  enum t_fd_tag_ {};
  using t_fd_       = t_int32;
  using t_fd        = t_explicit<t_fd_, t_fd_tag_>;
  using T_fd        = t_prefix<t_fd>::T_;
  using r_fd        = t_prefix<t_fd>::r_;
  using R_fd        = t_prefix<t_fd>::R_;
  using p_fd        = t_prefix<t_fd>::p_;
  using P_fd        = t_prefix<t_fd>::P_;

  enum t_cnt_tag_ {};
  using t_cnt_      = t_ulong;
  using t_cnt       = t_explicit<t_cnt_, t_cnt_tag_>;
  using T_cnt       = t_prefix<t_cnt>::T_;
  using r_cnt       = t_prefix<t_cnt>::r_;
  using R_cnt       = t_prefix<t_cnt>::R_;
  using p_cnt       = t_prefix<t_cnt>::p_;
  using P_cnt       = t_prefix<t_cnt>::P_;

  enum t_n_tag_ {};
  using t_n_        = t_ulong;
  using t_n         = t_explicit<t_n_, t_n_tag_>;
  using T_n         = t_prefix<t_n>::T_;
  using r_n         = t_prefix<t_n>::r_;
  using R_n         = t_prefix<t_n>::R_;
  using p_n         = t_prefix<t_n>::p_;
  using P_n         = t_prefix<t_n>::P_;

  enum t_ix_tag_ {};
  using t_ix_       = t_n_;
  using t_ix        = t_explicit<t_ix_, t_ix_tag_>;
  using T_ix        = t_prefix<t_ix>::T_;
  using r_ix        = t_prefix<t_ix>::r_;
  using R_ix        = t_prefix<t_ix>::R_;
  using p_ix        = t_prefix<t_ix>::p_;
  using P_ix        = t_prefix<t_ix>::P_;

  enum t_bix_tag_ {};
  using t_bix       = t_explicit<t_ix_, t_bix_tag_>;
  using T_bix       = t_prefix<t_bix>::T_;
  using r_bix       = t_prefix<t_bix>::r_;
  using R_bix       = t_prefix<t_bix>::R_;
  using p_bix       = t_prefix<t_bix>::p_;
  using P_bix       = t_prefix<t_bix>::P_;

  enum t_eix_tag_ {};
  using t_eix       = t_explicit<t_ix_, t_eix_tag_>;
  using T_eix       = t_prefix<t_eix>::T_;
  using r_eix       = t_prefix<t_eix>::r_;
  using R_eix       = t_prefix<t_eix>::R_;
  using p_eix       = t_prefix<t_eix>::p_;
  using P_eix       = t_prefix<t_eix>::P_;

  enum t_validity_tag_ {};
  using t_validity_ = t_bool;
  using t_validity  = t_explicit<t_validity_, t_validity_tag_>;
  using T_validity  = t_prefix<t_validity>::T_;
  using r_validity  = t_prefix<t_validity>::r_;
  using R_validity  = t_prefix<t_validity>::R_;
  using p_validity  = t_prefix<t_validity>::p_;
  using P_validity  = t_prefix<t_validity>::P_;

  enum t_cstr_tag_ {};
  using p_cstr_     = t_prefix<char>::p_;
  using t_cstr_ptr  = t_explicit<p_cstr_, t_cstr_tag_>;
  using T_cstr_ptr  = t_prefix<t_cstr_ptr>::T_;
  using r_cstr_ptr  = t_prefix<t_cstr_ptr>::r_;
  using R_cstr_ptr  = t_prefix<t_cstr_ptr>::R_;
  using p_cstr_ptr  = t_prefix<t_cstr_ptr>::p_;
  using P_cstr_ptr  = t_prefix<t_cstr_ptr>::P_;

  using P_cstr_     = t_prefix<char>::P_;
  using t_cstr_cptr = t_explicit<P_cstr_, t_cstr_tag_>;
  using T_cstr_cptr = t_prefix<t_cstr_cptr>::T_;
  using r_cstr_cptr = t_prefix<t_cstr_cptr>::r_;
  using R_cstr_cptr = t_prefix<t_cstr_cptr>::R_;
  using p_cstr_cptr = t_prefix<t_cstr_cptr>::p_;
  using P_cstr_cptr = t_prefix<t_cstr_cptr>::P_;

  enum  t_percentage_tag_ {};
  using t_percentage_ = t_uchar; // units of precentage - no decimals
  using t_percentage  = t_explicit<t_percentage_, t_percentage_tag_>;
  using T_percentage  = t_prefix<t_percentage>::T_;
  using r_percentage  = t_prefix<t_percentage>::r_;
  using R_percentage  = t_prefix<t_percentage>::R_;
  using p_percentage  = t_prefix<t_percentage>::p_;
  using P_percentage  = t_prefix<t_percentage>::P_;

///////////////////////////////////////////////////////////////////////////////

  constexpr r_cnt operator++(r_cnt cnt) noexcept {
    return cnt = t_cnt{get(cnt) + 1};
  }

///////////////////////////////////////////////////////////////////////////////

  constexpr t_bool operator< (t_n lh, t_n rh) noexcept {
    return get(lh) < get(rh);
  }

  constexpr t_bool operator<=(t_n lh, t_n rh) noexcept {
    return get(lh) <= get(rh);
  }

  constexpr t_bool operator< (t_ix lh, t_ix rh) noexcept {
    return get(lh) < get(rh);
  }

  constexpr t_bool operator<=(t_ix lh, t_ix rh) noexcept {
    return get(lh) <= get(rh);
  }

  constexpr t_bool operator< (t_percentage lh, t_percentage rh) noexcept {
    return get(lh) < get(rh);
  }

  constexpr t_bool operator<=(t_percentage lh, t_percentage rh) noexcept {
    return get(lh) <= get(rh);
  }

///////////////////////////////////////////////////////////////////////////////

  constexpr t_validity   VALID{true};
  constexpr t_validity INVALID{false};

  constexpr t_fd_      BAD_FD_ = -1;
  constexpr t_fd       BAD_FD {BAD_FD_};

///////////////////////////////////////////////////////////////////////////////

  constexpr t_n   to_n  (t_ix ix)  noexcept  { return transform<t_n>  (ix); }
  constexpr t_bix to_bix(t_ix ix)  noexcept  { return transform<t_bix>(ix); }
  constexpr t_eix to_eix(t_ix ix)  noexcept  { return transform<t_eix>(ix); }
  constexpr t_ix  to_ix (t_n  n)   noexcept  { return transform<t_ix> (n);  }
  constexpr t_ix  to_ix (t_bix ix) noexcept  { return transform<t_ix> (ix); }
  constexpr t_ix  to_ix (t_eix ix) noexcept  { return transform<t_ix> (ix); }

  constexpr t_ix  operator"" _ix (t_ullong ix) noexcept { return t_ix(ix);  }
  constexpr t_bix operator"" _bix(t_ullong ix) noexcept { return t_bix(ix); }
  constexpr t_eix operator"" _eix(t_ullong ix) noexcept { return t_eix(ix); }
  constexpr t_n   operator"" _n  (t_ullong n)  noexcept { return t_n(n);    }

///////////////////////////////////////////////////////////////////////////////

  constexpr
  t_bool operator==(const t_validity& lh, const t_validity& rh) noexcept {
    return get(lh) == get(rh);
  }

///////////////////////////////////////////////////////////////////////////////

  template<int N>
  constexpr t_cstr_ptr mk_ptr(t_char (&str)[N]) noexcept {
    return t_cstr_ptr{str};
  }

  constexpr t_cstr_ptr mk_ptr(p_cstr_ str) noexcept {
    return t_cstr_ptr{str};
  }

  template<int N>
  constexpr t_cstr_cptr mk_cptr(T_char (&cstr)[N]) noexcept {
    return t_cstr_cptr{cstr};
  }

  constexpr t_cstr_cptr mk_cptr(P_cstr_ cstr) noexcept {
    return t_cstr_cptr{cstr};
  }

///////////////////////////////////////////////////////////////////////////////

  enum  t_errn_tag_ {};
  using t_errn_ = named::t_int;
  using t_errn  = named::t_explicit<t_errn_, t_errn_tag_>;

  constexpr t_errn_ NO_ERRN_  =  0;
  constexpr t_errn  NO_ERRN {NO_ERRN_};

  constexpr t_errn_ BAD_ERRN_ = -1;
  constexpr t_errn  BAD_ERRN {BAD_ERRN_};

  constexpr t_bool operator==(t_errn errn, t_validity validity) noexcept {
    return errn == NO_ERRN && validity == VALID;
  }

///////////////////////////////////////////////////////////////////////////////

  template<typename T>
  class t_verifiable;

  template<typename T, typename TAG, typename CHECK>
  class t_verifiable<t_explicit<T, TAG, CHECK>> {
  public:
    using t_value = t_explicit<T, TAG, CHECK>;
    using T_value = typename t_prefix<t_value>::T_;

    constexpr t_verifiable(t_value _value, t_errn _errn) noexcept
      : value{_value}, errn{_errn} {
    }

    constexpr operator t_validity() const noexcept {
      return errn == NO_ERRN ? VALID : INVALID;
    }

    constexpr operator t_value () const noexcept {
      return value;
    }

    T_value value;
    t_errn  errn;
  };

  template<typename T>
  constexpr const T& get(const t_verifiable<T>& verifiable) noexcept {
    return verifiable.value;
  }

  template<class T, class TAG, class CHECK>
  constexpr
  T get(const t_verifiable<t_explicit<T, TAG, CHECK>>& verifiable) noexcept {
    return get(verifiable.value);
  }

///////////////////////////////////////////////////////////////////////////////

  template<typename TAG>
  struct t_user {
    union {
      t_int64 id;
      p_void  ptr;
      P_void  cptr;
      t_char  buf[sizeof(t_int64)]; // 8 bytes
    };
    t_user()             noexcept : id  {0L}    { }
    t_user(t_int64  _id) noexcept : id  {_id}   { }
    t_user(p_void  _ptr) noexcept : ptr {_ptr}  { }
    t_user(P_void _cptr) noexcept : cptr{_cptr} { }
  };

  template<typename TAG>
  constexpr
  t_bool operator==(const t_user<TAG>& lh, const t_user<TAG>& rh) noexcept {
    return lh.id == rh.id;
  }

  template<typename TAG>
  constexpr
  t_bool operator!=(const t_user<TAG>& lh, const t_user<TAG>& rh) noexcept {
    return lh.id != rh.id;
  }

///////////////////////////////////////////////////////////////////////////////

  enum  t_nsec_tag_ {};
  using t_nsec_ = t_ullong;
  using t_nsec  = t_explicit<t_nsec_, t_nsec_tag_>;
  using T_nsec  = t_prefix<t_nsec>::T_;
  using r_nsec  = t_prefix<t_nsec>::r_;
  using p_nsec  = t_prefix<t_nsec>::p_;
  using P_nsec  = t_prefix<t_nsec>::P_;

  enum  t_usec_tag_ {};
  using t_usec_ = t_ulong;
  using t_usec  = t_explicit<t_usec_, t_usec_tag_>;
  using T_usec  = t_prefix<t_usec>::T_;
  using r_usec  = t_prefix<t_usec>::r_;
  using p_usec  = t_prefix<t_usec>::p_;
  using P_usec  = t_prefix<t_usec>::P_;

  enum  t_msec_tag_ {};
  using t_msec_ = t_uint;
  using t_msec  = t_explicit<t_msec_, t_msec_tag_>;
  using T_msec  = t_prefix<t_msec>::T_;
  using r_msec  = t_prefix<t_msec>::r_;
  using p_msec  = t_prefix<t_msec>::p_;
  using P_msec  = t_prefix<t_msec>::P_;

  enum  t_sec_tag_ {};
  using t_sec_ = t_uint;
  using t_sec  = t_explicit<t_sec_, t_sec_tag_>;
  using T_sec  = t_prefix<t_sec>::T_;
  using r_sec  = t_prefix<t_sec>::r_;
  using p_sec  = t_prefix<t_sec>::p_;
  using P_sec  = t_prefix<t_sec>::P_;

  enum  t_min_tag_ {};
  using t_min_ = t_ushort;
  using t_min  = t_explicit<t_min_, t_min_tag_>;
  using T_min  = t_prefix<t_min>::T_;
  using r_min  = t_prefix<t_min>::r_;
  using p_min  = t_prefix<t_min>::p_;
  using P_min  = t_prefix<t_min>::P_;

  enum  t_ticks_tag_ {};
  using t_ticks_ = t_uint64;
  using t_ticks  = t_explicit<t_ticks_, t_ticks_tag_>;
  using T_ticks  = t_prefix<t_ticks>::T_;
  using r_ticks  = t_prefix<t_ticks>::r_;
  using p_ticks  = t_prefix<t_ticks>::p_;
  using P_ticks  = t_prefix<t_ticks>::P_;

///////////////////////////////////////////////////////////////////////////////

  enum t_emplace_it { EMPLACE_IT };

///////////////////////////////////////////////////////////////////////////////

  template<t_n_ N, typename TAG>
  class t_multiple {
  public:
    constexpr static t_n of() noexcept {
      return t_n{N};
    }
    constexpr t_multiple(t_n_ _value) noexcept : value(_value) {
    }

    t_n value;
  };

  template<t_n_ N, typename TAG>
  constexpr t_n multiple_of(t_multiple<N, TAG> multiple) {
    return t_n{N*get(multiple.value)};
  }

///////////////////////////////////////////////////////////////////////////////

  template<typename TAG>
  class t_unknown_sized_ptr {
  public:
    using r_unknown_sized_ptr = typename t_prefix<t_unknown_sized_ptr<TAG>>::r_;

    constexpr t_unknown_sized_ptr() noexcept = default;

    constexpr
    t_unknown_sized_ptr(p_void _ptr, t_n _n) noexcept : ptr{_ptr}, n{_n} {
    }

    template<typename T>
    constexpr t_unknown_sized_ptr(T* _ptr) noexcept : ptr{_ptr}, n{sizeof(T)} {
    }

    template<typename T>
    constexpr r_unknown_sized_ptr operator=(T* _ptr) noexcept {
      ptr = _ptr;
      n   = t_n{sizeof(T)};
      return *this;
    }

    constexpr operator t_validity() const noexcept {
      return ptr ? VALID : INVALID;
    }

    p_void ptr = nullptr;
    t_n    n   = t_n{0};
  };

  template<typename TAG>
  class t_unknown_sized_cptr {
  public:
    using r_unknown_sized_cptr = typename t_prefix<t_unknown_sized_cptr<TAG>>::r_;
    using R_unknown_sized_ptr  = typename t_prefix<t_unknown_sized_ptr<TAG>>::R_;

    constexpr t_unknown_sized_cptr() noexcept = default;

    constexpr
    t_unknown_sized_cptr(P_void _ptr, t_n _n) noexcept : ptr{_ptr}, n{_n} {
    }

    template<typename T>
    constexpr
    t_unknown_sized_cptr(T* _ptr) noexcept : ptr{_ptr}, n{sizeof(T)} {
    }

    constexpr t_unknown_sized_cptr(R_unknown_sized_ptr _ptr) noexcept
      : ptr{_ptr.ptr}, n{_ptr.n} {
    }

    constexpr
    r_unknown_sized_cptr operator=(R_unknown_sized_ptr _ptr) noexcept {
      ptr = _ptr.ptr;
      n   = _ptr.n;
      return *this;
    }

    template<typename T>
    constexpr r_unknown_sized_cptr operator=(T* _ptr) noexcept {
      ptr = _ptr;
      n   = t_n{sizeof(T)};
      return *this;
    }

    constexpr operator t_validity() const noexcept {
      return ptr ? VALID : INVALID;
    }

    P_void ptr = nullptr;
    t_n    n   = t_n{0};
  };

///////////////////////////////////////////////////////////////////////////////

  // deprecated - should remove overtime - P_cstr and p_cstr

  using p_cstr = t_explicit<p_cstr_, t_cstr_tag_>;
  using P_cstr = t_explicit<P_cstr_, t_cstr_tag_>; // XXX - not sure?

  template<int N>
  constexpr p_cstr mk_str(char (&str)[N]) noexcept { return p_cstr{str}; }
  constexpr p_cstr mk_str(p_cstr_ str)    noexcept { return p_cstr{str}; }

  template<int N>
  constexpr P_cstr mk_cstr(const char (&cstr)[N]) noexcept {
    return P_cstr{cstr};
  }

  constexpr P_cstr mk_cstr(P_cstr_ cstr) noexcept {
     return P_cstr{cstr};
  }

///////////////////////////////////////////////////////////////////////////////
}
}

#endif
