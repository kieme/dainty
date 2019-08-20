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

#ifndef _DAINTY_BASE_EXPLICIT_H_
#define _DAINTY_BASE_EXPLICIT_H_

#include "dainty_base_types.h"
#include "dainty_base_traits.h"

namespace dainty
{
namespace base
{
///////////////////////////////////////////////////////////////////////////////

  template<typename T>
  struct t_explicit_value;

  template<> struct t_explicit_value<t_bool>   { using t_value = t_bool;   };
  template<> struct t_explicit_value<t_char>   { using t_value = t_char;   };
  template<> struct t_explicit_value<t_uchar>  { using t_value = t_uchar;  };
  template<> struct t_explicit_value<t_int>    { using t_value = t_int;    };
  template<> struct t_explicit_value<t_uint>   { using t_value = t_uint;   };
  template<> struct t_explicit_value<t_short>  { using t_value = t_short;  };
  template<> struct t_explicit_value<t_ushort> { using t_value = t_ushort; };
  template<> struct t_explicit_value<t_long>   { using t_value = t_long;   };
  template<> struct t_explicit_value<t_ulong>  { using t_value = t_ulong;  };
  template<> struct t_explicit_value<t_llong>  { using t_value = t_llong;  };
  template<> struct t_explicit_value<t_ullong> { using t_value = t_ullong; };
  template<> struct t_explicit_value<t_double> { using t_value = t_double; };
  template<> struct t_explicit_value<t_void>   { using t_value = t_void;   };

  template<typename T>
  struct t_explicit_value<const T> {
    using t_value = const typename t_explicit_value<T>::t_value;
  };

  template<typename T>
  struct t_explicit_value<T*> {
    using t_value = typename t_explicit_value<T>::t_value*;
  };

  template<typename T> struct t_explicit_value<T&>;
  template<typename T> struct t_explicit_value<T**>;

///////////////////////////////////////////////////////////////////////////////

  template<class, class, class> class t_explicit;
  template<class T, class TAG, class CHECK>
  constexpr T get(t_explicit<T, TAG, CHECK>) noexcept;

  template<class T, class TAG>
  struct t_check {
    using t_value = typename t_explicit_value<T>::t_value;
    using t_tag   = TAG;
    constexpr static t_value test(t_value value) { return value; }
  };

///////////////////////////////////////////////////////////////////////////////

  template<typename... Ts>
  using t_tags = traits::t_pack<Ts...>;

///////////////////////////////////////////////////////////////////////////////

  template<class T, class TAG, class CHECK = void>
  class t_explicit {
  public:
    using t_value = typename t_explicit_value<T>::t_value;
    using t_tag   = t_tags<TAG>;
    using t_check = CHECK;

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
    using t_value = typename t_explicit_value<T>::t_value;
    using t_tag   = t_tags<TAG>;
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

  enum  t_byte_tag_ { };
  using t_byte  = t_explicit<t_byte_, t_byte_tag_>;
  using T_byte  = t_prefix<t_byte>::T_;
  using r_byte  = t_prefix<t_byte>::r_;
  using R_byte  = t_prefix<t_byte>::R_;
  using p_byte  = t_prefix<t_byte>::p_;
  using P_byte  = t_prefix<t_byte>::P_;

  enum  t_fd_tag_ {};
  using t_fd  = t_explicit<t_fd_, t_fd_tag_>;
  using T_fd  = t_prefix<t_fd>::T_;
  using r_fd  = t_prefix<t_fd>::r_;
  using R_fd  = t_prefix<t_fd>::R_;
  using p_fd  = t_prefix<t_fd>::p_;
  using P_fd  = t_prefix<t_fd>::P_;

  enum  t_cnt_tag_ {};
  using t_cnt  = t_explicit<t_cnt_, t_cnt_tag_>;
  using T_cnt  = t_prefix<t_cnt>::T_;
  using r_cnt  = t_prefix<t_cnt>::r_;
  using R_cnt  = t_prefix<t_cnt>::R_;
  using p_cnt  = t_prefix<t_cnt>::p_;
  using P_cnt  = t_prefix<t_cnt>::P_;

  enum  t_n_tag_ {};
  using t_n  = t_explicit<t_n_, t_n_tag_>;
  using T_n  = t_prefix<t_n>::T_;
  using r_n  = t_prefix<t_n>::r_;
  using R_n  = t_prefix<t_n>::R_;
  using p_n  = t_prefix<t_n>::p_;
  using P_n  = t_prefix<t_n>::P_;

  enum  t_ix_tag_ {};
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
  using t_percentage  = t_explicit<t_percentage_, t_percentage_tag_>;
  using T_percentage  = t_prefix<t_percentage>::T_;
  using r_percentage  = t_prefix<t_percentage>::r_;
  using R_percentage  = t_prefix<t_percentage>::R_;
  using p_percentage  = t_prefix<t_percentage>::p_;
  using P_percentage  = t_prefix<t_percentage>::P_;

  enum  t_seq_no_tag_ {};
  using t_seq_no  = t_explicit<t_seq_no_, t_seq_no_tag_>;
  using T_seq_no  = t_prefix<t_seq_no>::T_;
  using r_seq_no  = t_prefix<t_seq_no>::r_;
  using R_seq_no  = t_prefix<t_seq_no>::R_;
  using p_seq_no  = t_prefix<t_seq_no>::p_;
  using P_seq_no  = t_prefix<t_seq_no>::P_;

  enum  t_nsec_tag_ {};
  using t_nsec  = t_explicit<t_nsec_, t_nsec_tag_>;
  using T_nsec  = t_prefix<t_nsec>::T_;
  using r_nsec  = t_prefix<t_nsec>::r_;
  using p_nsec  = t_prefix<t_nsec>::p_;
  using P_nsec  = t_prefix<t_nsec>::P_;

  enum  t_usec_tag_ {};
  using t_usec  = t_explicit<t_usec_, t_usec_tag_>;
  using T_usec  = t_prefix<t_usec>::T_;
  using r_usec  = t_prefix<t_usec>::r_;
  using p_usec  = t_prefix<t_usec>::p_;
  using P_usec  = t_prefix<t_usec>::P_;

  enum  t_msec_tag_ {};
  using t_msec  = t_explicit<t_msec_, t_msec_tag_>;
  using T_msec  = t_prefix<t_msec>::T_;
  using r_msec  = t_prefix<t_msec>::r_;
  using p_msec  = t_prefix<t_msec>::p_;
  using P_msec  = t_prefix<t_msec>::P_;

  enum  t_sec_tag_ {};
  using t_sec  = t_explicit<t_sec_, t_sec_tag_>;
  using T_sec  = t_prefix<t_sec>::T_;
  using r_sec  = t_prefix<t_sec>::r_;
  using p_sec  = t_prefix<t_sec>::p_;
  using P_sec  = t_prefix<t_sec>::P_;

  enum  t_min_tag_ {};
  using t_min  = t_explicit<t_min_, t_min_tag_>;
  using T_min  = t_prefix<t_min>::T_;
  using r_min  = t_prefix<t_min>::r_;
  using p_min  = t_prefix<t_min>::p_;
  using P_min  = t_prefix<t_min>::P_;

  enum  t_ticks_tag_ {};
  using t_ticks  = t_explicit<t_ticks_, t_ticks_tag_>;
  using T_ticks  = t_prefix<t_ticks>::T_;
  using r_ticks  = t_prefix<t_ticks>::r_;
  using p_ticks  = t_prefix<t_ticks>::p_;
  using P_ticks  = t_prefix<t_ticks>::P_;

  // deprecated - should remove overtime - P_cstr and p_cstr
  using p_cstr = t_explicit<p_cstr_, t_cstr_tag_>;
  using P_cstr = t_explicit<P_cstr_, t_cstr_tag_>; // XXX - not sure?

///////////////////////////////////////////////////////////////////////////////

  constexpr t_validity    VALID{true};
  constexpr t_validity  INVALID{false};

  constexpr t_cstr_ptr  NO_CSTR_PTR {nullptr};
  constexpr t_cstr_cptr NO_CSTR_CPTR{nullptr};

  constexpr t_fd_       BAD_FD_ = -1;
  constexpr t_fd        BAD_FD {BAD_FD_};

///////////////////////////////////////////////////////////////////////////////

  enum  t_errn_tag_ {};
  using t_errn  = base::t_explicit<t_errn_, t_errn_tag_>;

  constexpr t_errn_ NO_ERRN_  =  0;
  constexpr t_errn  NO_ERRN {NO_ERRN_};

  constexpr t_errn_ BAD_ERRN_ = -1;
  constexpr t_errn  BAD_ERRN {BAD_ERRN_};

  constexpr t_bool operator==(t_errn errn, t_validity validity) noexcept {
    return errn == NO_ERRN && validity == VALID;
  }

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

  template<typename T, typename TAG>
  constexpr t_explicit<T, TAG> minimum(t_explicit<T, TAG> _value1,
                                       t_explicit<T, TAG> _value2) noexcept {
    T value1 = get(_value1), value2 = get(_value2);
    return t_explicit<T, TAG>{value1 < value2 ? value1 : value2};
  }

  template<typename T, typename TAG>
  constexpr T minimum(t_explicit<T, TAG> _value1, T value2) noexcept {
    T value1 = get(_value1);
    return value1 < value2 ? value1 : value2;
  }

  template<typename T, typename TAG>
  constexpr T minimum(T value1, t_explicit<T, TAG> _value2) noexcept {
    T value2 = get(_value2);
    return value1 < value2 ? value1 : value2;
  }

///////////////////////////////////////////////////////////////////////////////

  template<typename T, typename TAG>
  constexpr t_explicit<T, TAG> maximum(t_explicit<T, TAG> _value1,
                                       t_explicit<T, TAG> _value2) noexcept {
    T value1 = get(_value1), value2 = get(_value2);
    return t_explicit<T, TAG>{value1 > value2 ? value1 : value2};
  }

  template<typename T, typename TAG>
  constexpr T maximum(t_explicit<T, TAG> _value1, T value2) noexcept {
    T value1 = get(_value1);
    return value1 > value2 ? value1 : value2;
  }

  template<typename T, typename TAG>
  constexpr T maximum(T value1, t_explicit<T, TAG> _value2) noexcept {
    T value2 = get(_value2);
    return value1 > value2 ? value1 : value2;
  }

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
