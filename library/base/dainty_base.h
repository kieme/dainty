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

#ifndef _DAINTY_BASE_H_
#define _DAINTY_BASE_H_

#include "dainty_base_types.h"
#include "dainty_base_traits.h"
#include "dainty_base_explicit.h"
#include "dainty_base_logical.h"

// base: 'give a name to'
//
//   <base> name builtin types and specify a simple naming convention
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
//     N12. when using <dainty::base> builtin types on interfaces, use
//          specializations of t_explicit, to assign purpose to a type.
//     N13. use '_' as delimiter within long names to improve readability.
//     N14. use <dainty::base> builtin types instead of <std>.
//     N15. use qualified names or "using " when using <dainty::base>.
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
//   typedef base::t_explicit<base::t_uint, t_x_> t_xpoint;
//   typedef base::t_explicit<base::t_uint, t_y_> t_ypoint;
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
namespace base
{
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

  template<typename T, typename TAG, T INIT_VALUE,
                       typename U = base::t_uint>
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
