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

#include "dainty_named_types.h"
#include "dainty_named_traits.h"
#include "dainty_named_explicit.h"
#include "dainty_named_logical.h"

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

namespace dainty
{
namespace named
{
///////////////////////////////////////////////////////////////////////////////

  enum  t_byte_tag_ { };
  using t_byte_ = t_uchar;
  using t_byte  = t_explicit<t_byte_, t_byte_tag_>;
  using T_byte  = t_prefix<t_byte>::T_;
  using r_byte  = t_prefix<t_byte>::r_;
  using R_byte  = t_prefix<t_byte>::R_;
  using p_byte  = t_prefix<t_byte>::p_;
  using P_byte  = t_prefix<t_byte>::P_;

  enum  t_fd_tag_ {};
  using t_fd_ = t_int32;
  using t_fd  = t_explicit<t_fd_, t_fd_tag_>;
  using T_fd  = t_prefix<t_fd>::T_;
  using r_fd  = t_prefix<t_fd>::r_;
  using R_fd  = t_prefix<t_fd>::R_;
  using p_fd  = t_prefix<t_fd>::p_;
  using P_fd  = t_prefix<t_fd>::P_;

  enum  t_cnt_tag_ {};
  using t_cnt_ = t_ulong;
  using t_cnt  = t_explicit<t_cnt_, t_cnt_tag_>;
  using T_cnt  = t_prefix<t_cnt>::T_;
  using r_cnt  = t_prefix<t_cnt>::r_;
  using R_cnt  = t_prefix<t_cnt>::R_;
  using p_cnt  = t_prefix<t_cnt>::p_;
  using P_cnt  = t_prefix<t_cnt>::P_;

  enum  t_n_tag_ {};
  using t_n_ = t_ulong;
  using t_n  = t_explicit<t_n_, t_n_tag_>;
  using T_n  = t_prefix<t_n>::T_;
  using r_n  = t_prefix<t_n>::r_;
  using R_n  = t_prefix<t_n>::R_;
  using p_n  = t_prefix<t_n>::p_;
  using P_n  = t_prefix<t_n>::P_;

  enum  t_ix_tag_ {};
  using t_ix_ = t_n_;
  using t_ix  = t_explicit<t_ix_, t_ix_tag_>;
  using T_ix  = t_prefix<t_ix>::T_;
  using r_ix  = t_prefix<t_ix>::r_;
  using R_ix  = t_prefix<t_ix>::R_;
  using p_ix  = t_prefix<t_ix>::p_;
  using P_ix  = t_prefix<t_ix>::P_;

  enum  t_bix_tag_ {};
  using t_bix = t_explicit<t_ix_, t_bix_tag_>;
  using T_bix = t_prefix<t_bix>::T_;
  using r_bix = t_prefix<t_bix>::r_;
  using R_bix = t_prefix<t_bix>::R_;
  using p_bix = t_prefix<t_bix>::p_;
  using P_bix = t_prefix<t_bix>::P_;

  enum  t_eix_tag_ {};
  using t_eix = t_explicit<t_ix_, t_eix_tag_>;
  using T_eix = t_prefix<t_eix>::T_;
  using r_eix = t_prefix<t_eix>::r_;
  using R_eix = t_prefix<t_eix>::R_;
  using p_eix = t_prefix<t_eix>::p_;
  using P_eix = t_prefix<t_eix>::P_;

  enum  t_validity_tag_ {};
  using t_validity_ = t_bool;
  using t_validity  = t_explicit<t_validity_, t_validity_tag_>;
  using T_validity  = t_prefix<t_validity>::T_;
  using r_validity  = t_prefix<t_validity>::r_;
  using R_validity  = t_prefix<t_validity>::R_;
  using p_validity  = t_prefix<t_validity>::p_;
  using P_validity  = t_prefix<t_validity>::P_;

  enum  t_cstr_tag_ {};
  using p_cstr_     = t_prefix<char>::p_;
  using t_cstr_ptr_ = p_cstr_;
  using t_cstr_ptr  = t_explicit<t_cstr_ptr_, t_cstr_tag_>;
  using T_cstr_ptr  = t_prefix<t_cstr_ptr>::T_;
  using r_cstr_ptr  = t_prefix<t_cstr_ptr>::r_;
  using R_cstr_ptr  = t_prefix<t_cstr_ptr>::R_;
  using p_cstr_ptr  = t_prefix<t_cstr_ptr>::p_;
  using P_cstr_ptr  = t_prefix<t_cstr_ptr>::P_;

  using P_cstr_      = t_prefix<char>::P_;
  using t_cstr_cptr_ = P_cstr_;
  using t_cstr_cptr  = t_explicit<t_cstr_cptr_, t_cstr_tag_>;
  using T_cstr_cptr  = t_prefix<t_cstr_cptr>::T_;
  using r_cstr_cptr  = t_prefix<t_cstr_cptr>::r_;
  using R_cstr_cptr  = t_prefix<t_cstr_cptr>::R_;
  using p_cstr_cptr  = t_prefix<t_cstr_cptr>::p_;
  using P_cstr_cptr  = t_prefix<t_cstr_cptr>::P_;

  enum  t_percentage_tag_ {};
  using t_percentage_ = t_uchar; // units of precentage - no decimals
  using t_percentage  = t_explicit<t_percentage_, t_percentage_tag_>;
  using T_percentage  = t_prefix<t_percentage>::T_;
  using r_percentage  = t_prefix<t_percentage>::r_;
  using R_percentage  = t_prefix<t_percentage>::R_;
  using p_percentage  = t_prefix<t_percentage>::p_;
  using P_percentage  = t_prefix<t_percentage>::P_;

  enum  t_seq_no_tag_ {};
  using t_seq_no_ = t_uint;
  using t_seq_no  = t_explicit<t_seq_no_, t_seq_no_tag_>;
  using T_seq_no  = t_prefix<t_seq_no>::T_;
  using r_seq_no  = t_prefix<t_seq_no>::r_;
  using R_seq_no  = t_prefix<t_seq_no>::R_;
  using p_seq_no  = t_prefix<t_seq_no>::p_;
  using P_seq_no  = t_prefix<t_seq_no>::P_;

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

  constexpr t_validity    VALID{true};
  constexpr t_validity  INVALID{false};

  constexpr t_cstr_ptr  NO_CSTR_PTR{nullptr};
  constexpr t_cstr_cptr NO_CSTR_CPTR{nullptr};

  constexpr t_fd_       BAD_FD_ = -1;
  constexpr t_fd        BAD_FD {BAD_FD_};

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

  template<typename TAG>
  struct t_user {
    union {
      t_int64 id;
      p_void  ptr;
      P_void  cptr;
      t_char  buf[sizeof(t_int64)]; // 8 bytes
    };
    constexpr t_user()             noexcept : id  {0L}    { }
    constexpr t_user(t_int64  _id) noexcept : id  {_id}   { }
    constexpr t_user(p_void  _ptr) noexcept : ptr {_ptr}  { }
    constexpr t_user(P_void _cptr) noexcept : cptr{_cptr} { }
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

  template<typename T, typename TAG, T INIT_VALUE,
                       typename U = named::t_uint>
  struct t_id_pair {
    using t_value   = t_explicit<T, TAG>;
    using t_user_no = t_explicit<U, TAG>;

    constexpr static t_user_no BAD_USER_NO{0};

    constexpr t_id_pair() noexcept : value{INIT_VALUE}, user_no{BAD_USER_NO} {
    }

    constexpr explicit t_id_pair(t_value _value, t_user_no _user_no) noexcept
      : value{_value}, user_no{_user_no} {
    }

    constexpr operator t_validity() const noexcept {
      return user_no != BAD_USER_NO ? VALID : INVALID;
    }

    constexpr t_id_pair release() {
      t_id_pair tmp = *this;
      value    = t_value {INIT_VALUE};
      user_no  = BAD_USER_NO;
      return tmp;
    }

    t_value   value;
    t_user_no user_no;
  };

  template<typename T, typename TAG, T BAD_VALUE>
  constexpr t_bool operator==(const t_id_pair<T, TAG, BAD_VALUE> lh,
                              const t_id_pair<T, TAG, BAD_VALUE> rh) {
    return get(lh.user_no) == get(rh.user_no) &&
           get(lh.value)   == get(rh.value);
  }

  template<typename T, typename TAG, T BAD_VALUE>
  constexpr t_bool operator!=(const t_id_pair<T, TAG, BAD_VALUE> lh,
                              const t_id_pair<T, TAG, BAD_VALUE> rh) {
    return !(lh == rh);
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

  enum t_emplace_it { EMPLACE_IT }; // XXX TRY_EMPLACE ?

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
  class t_void_size_ptr {
  public:
    using r_void_size_ptr = typename t_prefix<t_void_size_ptr<TAG>>::r_;

    constexpr t_void_size_ptr() noexcept = default;

    constexpr
    t_void_size_ptr(p_void _ptr, t_n _n) noexcept : ptr{_ptr}, n{_n} {
    }

    template<typename T>
    constexpr t_void_size_ptr(T* _ptr) noexcept : ptr{_ptr}, n{sizeof(T)} {
    }

    template<typename T>
    constexpr r_void_size_ptr operator=(T* _ptr) noexcept {
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
  class t_void_size_cptr { // t_void_sized_ptr ?
  public:
    using r_void_size_cptr = typename t_prefix<t_void_size_cptr<TAG>>::r_;
    using R_void_size_ptr  = typename t_prefix<t_void_size_ptr<TAG>>::R_;

    constexpr t_void_size_cptr() noexcept = default;

    constexpr
    t_void_size_cptr(P_void _ptr, t_n _n) noexcept : ptr{_ptr}, n{_n} {
    }

    template<typename T>
    constexpr
    t_void_size_cptr(T* _ptr) noexcept : ptr{_ptr}, n{sizeof(T)} {
    }

    constexpr t_void_size_cptr(R_void_size_ptr _ptr) noexcept
      : ptr{_ptr.ptr}, n{_ptr.n} {
    }

    constexpr
    r_void_size_cptr operator=(R_void_size_ptr _ptr) noexcept {
      ptr = _ptr.ptr;
      n   = _ptr.n;
      return *this;
    }

    template<typename T>
    constexpr r_void_size_cptr operator=(T* _ptr) noexcept {
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

  constexpr t_bool reset(t_bool& t, t_bool value) {
    t_bool tmp = t;
    t = value;
    return tmp;
  }

  template<class T>
  constexpr T reset(T& t, T value) {
    T tmp = t;
    t = value;
    return tmp;
  }

  template<class T>
  constexpr T* reset(T*& t, T* value) {
    T* tmp = t;
    t = value;
    return tmp;
  }

  template<class T, class TAG>
  constexpr t_explicit<T, TAG>  reset(t_explicit<T, TAG>& t, T value) {
    t_explicit<T, TAG> tmp{t};
    t = t_explicit<T, TAG>{value};
    return tmp;
  }

  template<class T, class TAG>
  constexpr t_explicit<T, TAG> reset(t_explicit<T, TAG>& t,
                                     t_explicit<T, TAG> value) {
    t_explicit<T, TAG> tmp{t};
    t = value;
    return tmp;
  }

  template<class TAG>
  constexpr t_user<TAG> reset(t_user<TAG>& t, t_int64 value) {
    return t_user<TAG>{reset(t.id, value)};
  }

///////////////////////////////////////////////////////////////////////////////

  constexpr t_bool reset(t_bool& t) {
    t_bool tmp = t;
    t = false;
    return tmp;
  }

  template<class T>
  constexpr T reset(T& t) {
    T tmp = t;
    t = 0;
    return tmp;
  }

  template<class T>
  constexpr T* reset(T*& t) {
    T* tmp = t;
    t = nullptr;
    return tmp;
  }

  constexpr t_fd reset(t_fd& fd) {
    return reset(fd, BAD_FD);
  }

  template<class T, class TAG>
  constexpr t_explicit<T, TAG> reset(t_explicit<T, TAG>& t) {
    t_explicit<T, TAG> tmp{t};
    t = t_explicit<T, TAG>{0};
    return tmp;
  }

  template<class T, class TAG>
  constexpr t_explicit<T*, TAG> reset(t_explicit<T*, TAG>& t) {
    t_explicit<T*, TAG> tmp{t};
    t = t_explicit<T*, TAG>{nullptr};
    return tmp;
  }

  template<class TAG>
  constexpr t_explicit<t_bool, TAG> reset(t_explicit<t_bool, TAG>& t) {
    t_explicit<t_bool, TAG> tmp{t};
    t = t_explicit<t_bool, TAG>{false};
    return tmp;
  }

  template<class TAG>
  constexpr t_user<TAG> reset(t_user<TAG>& t) {
    return t_user<TAG>{reset(t.id)};
  }

///////////////////////////////////////////////////////////////////////////////

  template<typename T>
  constexpr auto x_cast(T&& in) -> typename t_remove_ref<T>::t_result&& {
    return static_cast<typename t_remove_ref<T>::t_result&&>(in);
  }

  template<typename T>
  constexpr T&& preserve(typename t_remove_ref<T>::t_result& in) {
    return static_cast<T&&>(in);
  }

  template<typename T>
  constexpr T&& preserve(typename t_remove_ref<T>::t_result&& in) {
    return x_cast(in);
  }

///////////////////////////////////////////////////////////////////////////////

  template<typename T>
  inline
  t_void swap(T& lh, T& rh) {
    T tmp {x_cast(lh)};
    lh = x_cast(rh);
    rh = x_cast(tmp);
  }

///////////////////////////////////////////////////////////////////////////////
}
}

#endif
