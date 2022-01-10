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

#ifndef _DAINTY_SPECIFIC_H_
#define _DAINTY_SPECIFIC_H_

#include "dainty_types.h"
#include "dainty_traits.h"

namespace dainty
{
namespace specific
{
  /////////////////////////////////////////////////////////////////////////////

  using types::t_prefix;
  using types::t_bool;
  using types::t_char;
  using types::T_char;
  using types::t_uchar;
  using types::t_int;
  using types::t_uint;
  using types::t_short;
  using types::t_ushort;
  using types::t_long;
  using types::t_ulong;
  using types::t_llong;
  using types::t_ullong;
  using types::t_double;
  using types::t_void;
  using types::t_s_;
  using types::t_s_tag_;
  using types::t_result_of;

  using types::t_byte_;
  using types::t_byte_tag_;
  using types::t_fd_;
  using types::t_fd_tag_;
  using types::t_cnt_;
  using types::t_cnt_tag_;
  using types::t_n_;
  using types::t_n_tag_;
  using types::t_n_min_tag_;
  using types::t_n_max_tag_;
  using types::t_ix_;
  using types::t_ix_tag_;
  using types::t_ix_;
  using types::t_ix_begin_tag_;
  using types::t_ix_;
  using types::t_ix_end_tag_;
  using types::t_validity_;
  using types::t_validity_tag_;
  using types::t_errn_;
  using types::t_errn_tag_;
  using types::t_percentage_;
  using types::t_percentage_tag_;
  using types::t_seqno_;
  using types::t_seqno_tag_;
  using types::t_nsec_;
  using types::t_nsec_tag_;
  using types::t_usec_;
  using types::t_usec_tag_;
  using types::t_msec_;
  using types::t_msec_tag_;
  using types::t_sec_;
  using types::t_sec_tag_;
  using types::t_min_;
  using types::t_min_tag_;
  using types::t_ticks_;
  using types::t_ticks_tag_;

  using types::VALID_;
  using types::INVALID_;
  using types::BAD_FD_;
  using types::NO_ERRN_;
  using types::BAD_ERRN_;
  using types::t_pack;

  using traits::t_if_then;
  using traits::t_is_one_of;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T>
  struct t_specific_value {
    using t_value = t_if_then<t_is_one_of<T, t_bool,
                                             t_char,
                                             t_uchar,
                                             t_int,
                                             t_uint,
                                             t_short,
                                             t_ushort,
                                             t_long,
                                             t_ulong,
                                             t_llong,
                                             t_ullong,
                                             t_double,
                                             t_void>, T>;
  };

  template<typename T>
  struct t_specific_value<const T> {
    using t_value = const typename t_specific_value<T>::t_value;
  };

  template<typename T>
  struct t_specific_value<T*> {
    using t_value = typename t_specific_value<T>::t_value*;
  };

  template<typename T> struct t_specific_value<T&>;
  template<typename T> struct t_specific_value<T**>;

  /////////////////////////////////////////////////////////////////////////////

  template<class, class, class> class t_specific;
  template<class T, class TAG, class CHECK>
  constexpr T get(t_specific<T, TAG, CHECK>) noexcept;

  template<class T, class TAG>
  struct t_check {
    using t_value = typename t_specific_value<T>::t_value;
    using t_tag   = TAG;
    constexpr static t_value test(t_value value) { return value; }
  };

  /////////////////////////////////////////////////////////////////////////////

  template<typename... Ts>
  using t_tags = t_pack<Ts...>;

  /////////////////////////////////////////////////////////////////////////////

  template<class T, class TAG, class CHECK = void>
  class t_specific {
  public:
    using t_value = typename t_specific_value<T>::t_value;
    using t_tag   = t_tags<TAG>;
    using t_check = CHECK;

    constexpr
    explicit t_specific(t_value value)
#ifndef DAINTY_EXPLICIT_CHECK
      : value_{value}
#else
      : value_{CHECK::test(value)}
#endif
    { }

    template<class T1, class CHECK1>
    constexpr
    explicit t_specific(t_specific<T1, TAG, CHECK1> value)
#ifndef DAINTY_EXPLICIT_CHECK
      : value_{value}
#else
      : value_{CHECK::test(value.value_)}
#endif
    { }

    template<class T1, class CHECK1>
    constexpr
    t_specific& operator=(t_specific<T1, TAG, CHECK1> value) {
#ifndef DAINTY_EXPLICIT_CHECK
      value_ = value;
#else
      value_ = CHECK::test(value.value_);
#endif
    }

    constexpr
    operator t_bool() const noexcept {
      return value_;
    }

    t_specific() = delete;                              // for clarity
    t_specific(const t_specific&) = default;            // for clarity
    t_specific& operator=(const t_specific&) = default; // for clarity

  private:
    template<class T1, class TAG1, class CHECK1> friend class t_specific;
    template<class T1, class TAG1, class CHECK1>
    friend constexpr T1  get(t_specific<T1, TAG1, CHECK1>) noexcept;
    t_value value_;
  };

  /////////////////////////////////////////////////////////////////////////////

  template<class T, class TAG>
  class t_specific<T, TAG, void> {
  public:
    using t_value = typename t_specific_value<T>::t_value;
    using t_tag   = t_tags<TAG>;
    using t_check = void;

    constexpr
    explicit t_specific(t_value value) noexcept : value_{value} {
    }

    template<class T1, class CHECK1>
    constexpr
    explicit t_specific(t_specific<T1, TAG, CHECK1> value) noexcept
      : value_{value} {
    }

    template<class T1, class CHECK1>
    constexpr
    t_specific& operator=(t_specific<T1, TAG, CHECK1> value) noexcept {
      value_ = value;
      return *this;
    }

    constexpr
    operator t_bool() const noexcept {
      return value_;
    }

    t_specific() = delete;                              // for clarity
    t_specific(const t_specific&) = default;            // for clarity
    t_specific& operator=(const t_specific&) = default; // for clarity

  private:
    template<class T1, class TAG1, class CHECK1> friend class t_specific;
    template<class T1, class TAG1, class CHECK1>
    friend constexpr T1 get(t_specific<T1, TAG1, CHECK1>) noexcept;
    t_value value_;
  };

  /////////////////////////////////////////////////////////////////////////////

  template<class T, class TAG, class CHECK>
  constexpr
  T get(t_specific<T, TAG, CHECK> value) noexcept { return value.value_; }

  /////////////////////////////////////////////////////////////////////////////

  template<class T, class TAG, class CHECK>
  constexpr
  t_bool operator==(t_specific<T, TAG, CHECK> lh,
                    t_specific<T, TAG, CHECK> rh) noexcept {
    return get(lh) == get(rh);
  }

  template<class T, class TAG, class CHECK>
  constexpr
  t_bool operator!=(t_specific<T, TAG, CHECK> lh,
                    t_specific<T, TAG, CHECK> rh) noexcept {
    return get(lh) != get(rh);
  }

  /////////////////////////////////////////////////////////////////////////////

  namespace help_ {
    template<typename>
    struct t_is_specific_ : traits::t_rfalse {
    };

    template<typename T, typename TAG, typename CHECK>
    struct t_is_specific_<t_specific<T, TAG, CHECK>> : traits::t_rtrue {
    };
  }

  template<typename T>
  using t_is_specific = t_result_of<help_::t_is_specific_<T>>;

  /////////////////////////////////////////////////////////////////////////////

  template<typename E, typename T, typename TAG, typename CHECK>
  constexpr
  E mk(t_specific<T, TAG, CHECK> src) noexcept {
    static_assert(t_is_specific<E>::VALUE, "not t_specific source");
    return E{get(src)};
  }

  /////////////////////////////////////////////////////////////////////////////

  using t_size  = t_specific<t_s_, t_s_tag_>;
  using T_size  = t_prefix<t_size>::T_;
  using r_size  = t_prefix<t_size>::r_;
  using R_size  = t_prefix<t_size>::R_;
  using p_size  = t_prefix<t_size>::p_;
  using P_size  = t_prefix<t_size>::P_;

  using t_byte  = t_specific<t_byte_, t_byte_tag_>;
  using T_byte  = t_prefix<t_byte>::T_;
  using r_byte  = t_prefix<t_byte>::r_;
  using R_byte  = t_prefix<t_byte>::R_;
  using p_byte  = t_prefix<t_byte>::p_;
  using P_byte  = t_prefix<t_byte>::P_;

  using t_fd  = t_specific<t_fd_, t_fd_tag_>;
  using T_fd  = t_prefix<t_fd>::T_;
  using r_fd  = t_prefix<t_fd>::r_;
  using R_fd  = t_prefix<t_fd>::R_;
  using p_fd  = t_prefix<t_fd>::p_;
  using P_fd  = t_prefix<t_fd>::P_;

  using t_cnt  = t_specific<t_cnt_, t_cnt_tag_>;
  using T_cnt  = t_prefix<t_cnt>::T_;
  using r_cnt  = t_prefix<t_cnt>::r_;
  using R_cnt  = t_prefix<t_cnt>::R_;
  using p_cnt  = t_prefix<t_cnt>::p_;
  using P_cnt  = t_prefix<t_cnt>::P_;

  using t_n  = t_specific<t_n_, t_n_tag_>;
  using T_n  = t_prefix<t_n>::T_;
  using r_n  = t_prefix<t_n>::r_;
  using R_n  = t_prefix<t_n>::R_;
  using p_n  = t_prefix<t_n>::p_;
  using P_n  = t_prefix<t_n>::P_;

  using t_n_min  = t_specific<t_n_, t_n_min_tag_>;
  using T_n_min  = t_prefix<t_n_min>::T_;
  using r_n_min  = t_prefix<t_n_min>::r_;
  using R_n_min  = t_prefix<t_n_min>::R_;
  using p_n_min  = t_prefix<t_n_min>::p_;
  using P_n_min  = t_prefix<t_n_min>::P_;

  using t_n_max  = t_specific<t_n_, t_n_max_tag_>;
  using T_n_max  = t_prefix<t_n_max>::T_;
  using r_n_max  = t_prefix<t_n_max>::r_;
  using R_n_max  = t_prefix<t_n_max>::R_;
  using p_n_max  = t_prefix<t_n_max>::p_;
  using P_n_max  = t_prefix<t_n_max>::P_;

  using t_ix  = t_specific<t_ix_, t_ix_tag_>;
  using T_ix  = t_prefix<t_ix>::T_;
  using r_ix  = t_prefix<t_ix>::r_;
  using R_ix  = t_prefix<t_ix>::R_;
  using p_ix  = t_prefix<t_ix>::p_;
  using P_ix  = t_prefix<t_ix>::P_;

  using t_ix_begin = t_specific<t_ix_, t_ix_begin_tag_>;
  using T_ix_begin = t_prefix<t_ix_begin>::T_;
  using r_ix_begin = t_prefix<t_ix_begin>::r_;
  using R_ix_begin = t_prefix<t_ix_begin>::R_;
  using p_ix_begin = t_prefix<t_ix_begin>::p_;
  using P_ix_begin = t_prefix<t_ix_begin>::P_;

  using t_ix_end = t_specific<t_ix_, t_ix_end_tag_>;
  using T_ix_end = t_prefix<t_ix_end>::T_;
  using r_ix_end = t_prefix<t_ix_end>::r_;
  using R_ix_end = t_prefix<t_ix_end>::R_;
  using p_ix_end = t_prefix<t_ix_end>::p_;
  using P_ix_end = t_prefix<t_ix_end>::P_;

  using t_validity  = t_specific<t_validity_, t_validity_tag_>;
  using T_validity  = t_prefix<t_validity>::T_;
  using r_validity  = t_prefix<t_validity>::r_;
  using R_validity  = t_prefix<t_validity>::R_;
  using p_validity  = t_prefix<t_validity>::p_;
  using P_validity  = t_prefix<t_validity>::P_;

  using t_errn  = t_specific<t_errn_, t_errn_tag_>;
  using T_errn  = t_prefix<t_errn>::T_;
  using r_errn  = t_prefix<t_errn>::r_;
  using R_errn  = t_prefix<t_errn>::R_;
  using p_errn  = t_prefix<t_errn>::p_;
  using P_errn  = t_prefix<t_errn>::P_;

  using t_percentage  = t_specific<t_percentage_, t_percentage_tag_>;
  using T_percentage  = t_prefix<t_percentage>::T_;
  using r_percentage  = t_prefix<t_percentage>::r_;
  using R_percentage  = t_prefix<t_percentage>::R_;
  using p_percentage  = t_prefix<t_percentage>::p_;
  using P_percentage  = t_prefix<t_percentage>::P_;

  using t_seqno  = t_specific<t_seqno_, t_seqno_tag_>;
  using T_seqno  = t_prefix<t_seqno>::T_;
  using r_seqno  = t_prefix<t_seqno>::r_;
  using R_seqno  = t_prefix<t_seqno>::R_;
  using p_seqno  = t_prefix<t_seqno>::p_;
  using P_seqno  = t_prefix<t_seqno>::P_;

  using t_nsec  = t_specific<t_nsec_, t_nsec_tag_>;
  using T_nsec  = t_prefix<t_nsec>::T_;
  using r_nsec  = t_prefix<t_nsec>::r_;
  using p_nsec  = t_prefix<t_nsec>::p_;
  using P_nsec  = t_prefix<t_nsec>::P_;

  using t_usec  = t_specific<t_usec_, t_usec_tag_>;
  using T_usec  = t_prefix<t_usec>::T_;
  using r_usec  = t_prefix<t_usec>::r_;
  using p_usec  = t_prefix<t_usec>::p_;
  using P_usec  = t_prefix<t_usec>::P_;

  using t_msec  = t_specific<t_msec_, t_msec_tag_>;
  using T_msec  = t_prefix<t_msec>::T_;
  using r_msec  = t_prefix<t_msec>::r_;
  using p_msec  = t_prefix<t_msec>::p_;
  using P_msec  = t_prefix<t_msec>::P_;

  using t_sec  = t_specific<t_sec_, t_sec_tag_>;
  using T_sec  = t_prefix<t_sec>::T_;
  using r_sec  = t_prefix<t_sec>::r_;
  using p_sec  = t_prefix<t_sec>::p_;
  using P_sec  = t_prefix<t_sec>::P_;

  using t_min  = t_specific<t_min_, t_min_tag_>;
  using T_min  = t_prefix<t_min>::T_;
  using r_min  = t_prefix<t_min>::r_;
  using p_min  = t_prefix<t_min>::p_;
  using P_min  = t_prefix<t_min>::P_;

  using t_ticks = t_specific<t_ticks_, t_ticks_tag_>;
  using T_ticks = t_prefix<t_ticks>::T_;
  using r_ticks = t_prefix<t_ticks>::r_;
  using p_ticks = t_prefix<t_ticks>::p_;
  using P_ticks = t_prefix<t_ticks>::P_;

  /////////////////////////////////////////////////////////////////////////////

  constexpr t_validity    VALID{VALID_};
  constexpr t_validity  INVALID{INVALID_};

  constexpr t_fd        BAD_FD {BAD_FD_};

  constexpr t_errn      NO_ERRN {NO_ERRN_};
  constexpr t_errn      BAD_ERRN{BAD_ERRN_};

  /////////////////////////////////////////////////////////////////////////////

  constexpr r_cnt operator++(r_cnt cnt) noexcept {
    return cnt = t_cnt{get(cnt) + 1};
  }

  /////////////////////////////////////////////////////////////////////////////

  constexpr t_bool operator==(t_errn errn,
                              t_validity validity) noexcept {
    return errn == NO_ERRN && validity == VALID;
  }

  /////////////////////////////////////////////////////////////////////////////

  constexpr t_bool operator<(t_n lh, t_n rh) noexcept {
    return get(lh) < get(rh);
  }

  constexpr t_bool operator<(t_n_ lh, t_n rh) noexcept {
    return lh < get(rh);
  }

  constexpr t_bool operator<(t_n lh, t_n_ rh) noexcept {
    return get(lh) < rh;
  }

  constexpr t_bool operator<=(t_n lh, t_n rh) noexcept {
    return get(lh) <= get(rh);
  }

  constexpr t_bool operator<=(t_n_ lh, t_n rh) noexcept {
    return lh <= get(rh);
  }

  constexpr t_bool operator<=(t_n lh, t_n_ rh) noexcept {
    return get(lh) <= rh;
  }

  constexpr t_n operator+(t_n lh, t_n rh) noexcept {
    return t_n(get(lh) + get(rh)); // can check for overflow - XXX
  }

  constexpr t_n operator+(t_n_ lh, t_n rh) noexcept {
    return t_n(lh + get(rh)); // can check for overflow - XXX
  }

  constexpr t_n operator+(t_n lh, t_n_ rh) noexcept {
    return t_n(get(lh) + rh); // can check for overflow - XXX
  }

  constexpr t_n operator-(t_n lh, t_n rh) noexcept {
    return t_n(get(lh) - get(rh)); // can check for overflow - XXX
  }

  constexpr t_n operator-(t_n_ lh, t_n rh) noexcept {
    return t_n(lh - get(rh)); // can check for overflow - XXX
  }

  constexpr t_n operator-(t_n lh, t_n_ rh) noexcept {
    return t_n(get(lh) - rh); // can check for overflow - XXX
  }

  constexpr r_n operator+=(r_n lh, t_n rh) noexcept {
    return lh = t_n(get(lh) + get(rh)); // can check for overflow - XXX
  }

  constexpr r_n operator+=(r_n lh, t_n_ rh) noexcept {
    return lh = t_n(get(lh) + rh); // can check for overflow - XXX
  }

  constexpr r_n operator-=(r_n lh, t_n rh) noexcept {
    return lh = t_n(get(lh) - get(rh)); // can check for overflow - XXX
  }

  constexpr r_n operator-=(r_n lh, t_n_ rh) noexcept {
    return lh = t_n(get(lh) - rh); // can check for overflow - XXX
  }

  /////////////////////////////////////////////////////////////////////////////

  constexpr r_ix operator--(r_ix ix) noexcept {
    return (ix = t_ix{get(ix) - 1});
  }

  constexpr r_ix operator++(r_ix ix) noexcept {
    return (ix = t_ix{get(ix) + 1});
  }

  constexpr t_bool operator< (t_ix lh, t_ix rh) noexcept {
    return get(lh) < get(rh);
  }

  constexpr t_bool operator<=(t_ix lh, t_ix rh) noexcept {
    return get(lh) <= get(rh);
  }

  constexpr t_bool operator<(t_percentage lh, t_percentage rh) noexcept {
    return get(lh) < get(rh);
  }

  constexpr t_bool operator<=(t_percentage lh, t_percentage rh) noexcept {
    return get(lh) <= get(rh);
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename TAG>
  constexpr t_specific<T, TAG> minimum(t_specific<T, TAG> _value1,
                                       t_specific<T, TAG> _value2) noexcept {
    T value1 = get(_value1), value2 = get(_value2);
    return t_specific<T, TAG>{value1 < value2 ? value1 : value2};
  }

  template<typename T, typename TAG>
  constexpr T minimum(t_specific<T, TAG> _value1, T value2) noexcept {
    T value1 = get(_value1);
    return value1 < value2 ? value1 : value2;
  }

  template<typename T, typename TAG>
  constexpr T minimum(T value1, t_specific<T, TAG> _value2) noexcept {
    T value2 = get(_value2);
    return value1 < value2 ? value1 : value2;
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename T, typename TAG>
  constexpr t_specific<T, TAG> maximum(t_specific<T, TAG> _value1,
                                       t_specific<T, TAG> _value2) noexcept {
    T value1 = get(_value1), value2 = get(_value2);
    return t_specific<T, TAG>{value1 > value2 ? value1 : value2};
  }

  template<typename T, typename TAG>
  constexpr T maximum(t_specific<T, TAG> _value1, T value2) noexcept {
    T value1 = get(_value1);
    return value1 > value2 ? value1 : value2;
  }

  template<typename T, typename TAG>
  constexpr T maximum(T value1, t_specific<T, TAG> _value2) noexcept {
    T value2 = get(_value2);
    return value1 > value2 ? value1 : value2;
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename TAG, typename T>
  constexpr
  t_specific<T, TAG> max_of(t_specific<T, TAG> value) noexcept {
    return value;
  }

  template<typename TAG, typename T, typename... Ts>
  constexpr
  t_specific<T, TAG> max_of(t_specific<T, TAG> value, Ts... values) noexcept {
    auto tmp = specific::max_of(values...);
    return get(value) > get(tmp) ? value : tmp;
  }

  /////////////////////////////////////////////////////////////////////////////

  template<typename TAG, typename T>
  constexpr
  t_specific<T, TAG> min_of(t_specific<T, TAG> value) noexcept {
    return value;
  }

  template<typename TAG, typename T, typename... Ts>
  constexpr
  t_specific<T, TAG> min_of(t_specific<T, TAG> value, Ts... values) noexcept {
    auto tmp = min_of(values...);
    return get(value) < get(tmp) ? value : tmp;
  }

  /////////////////////////////////////////////////////////////////////////////

  constexpr
  t_bool operator==(R_validity lh, R_validity rh) noexcept {
    return get(lh) == get(rh);
  }

  constexpr
  t_bool operator!=(R_validity lh, R_validity rh) noexcept {
    return get(lh) != get(rh);
  }

///////////////////////////////////////////////////////////////////////////////
}
}

#endif
