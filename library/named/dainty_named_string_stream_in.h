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

#ifndef _DAINTY_NAMED_STRING_STREAM_IN_H_
#define _DAINTY_NAMED_STRING_STREAM_IN_H_

#include "dainty_named_string_impl.h"

namespace dainty
{
namespace named
{
namespace string
{
  // 1. t_hex_p_ - store a pointer to a hex value
  // -----------
  // 1.1 hex_in(lvalue) use template dedecution to determine which type of hex.
  //
  // 2. t_int_p_ - store a pointer to an integer value
  // -----------
  // 2.1 integer_in(lvalue) use template dedecution to determine which type of
  //     integer.
  //
  // 3. t_snippet - a character range that can be valid or not
  // ------------
  // 3.1 snippet_in(lvalue, t_n)           a snippet of t_n characters
  // 3.1 snippet_in<true>(lvalue, ch)      a snippet until ch, plus one
  // 3.1 snippet_in<true,true>(lvalue, ch) a snippet until and including ch
  //                                       plus one
  // 4. t_skip_ skip and something match characters
  // ----------
  // 4.1  skip_n(t_n)                 number of characters to skip
  // 4.2  skip(t_char)                must match character then skip it
  // 4.3  skip(t_block)               must match block then skip it
  // 4.4  skip(t_crange)              must match range then skip it
  // 4.5  skip_until(t_char)          skip characters until t_char, return next
  // 4.6  skip_until<false>(t_char)   skip characters until t_char, return char
  // 4.7  skip_until(t_crange)        skip characters until t_char, return next
  // 4.8  skip_until<false>(t_crange) skip characters until start of range.
  // 4.9  skip_all(t_char)            skip all t_char characters
  // 4.10 skip_spaces()               skip all whitespaces
  //

////////////////////////////////////////////////////////////////////////////////

  enum t_hex_p_tag_ {};
  template<typename T> using t_hex_p_ = t_explicit<T*, t_hex_p_tag_>;

  enum t_int_p_tag_ {};
  template<typename T> using t_int_p_ = t_explicit<T*, t_int_p_tag_>;

  template<typename T>
  constexpr t_hex_p_<T>  hex_in(T& value)  noexcept {
    return t_hex_p_<T>{&value};
  }

  template<typename T>
  constexpr t_int_p_<T>  integer_in(T& value)  noexcept {
    return t_int_p_<T>{&value};
  }

////////////////////////////////////////////////////////////////////////////////

  struct t_snip_n_p_ {
    t_snip_n_p_(p_snippet _value, t_n_ _n) noexcept
      : value{_value}, n{_n} {
    }

    p_snippet value;
    t_n_      n;
  };

  template<t_bool PLUS1 = true, t_bool INCL_CHAR = false>
  struct t_snip_char_p_ {
    t_snip_char_p_(p_snippet _value, t_char _ch) noexcept
      : value{_value}, ch{_ch} {
    }

    p_snippet value;
    t_char    ch;
  };

  inline
  t_snip_n_p_ snippet_in(r_snippet snip, t_n n) noexcept {
    return t_snip_n_p_{&snip, get(n)};
  }

  inline
  t_snip_char_p_<> snippet_in(r_snippet snip, t_char ch) noexcept {
    return t_snip_char_p_<>{&snip, ch};
  }

  template<t_bool PLUS1>
  inline
  t_snip_char_p_<PLUS1> snippet_in(r_snippet snip, t_char ch) noexcept {
    return t_snip_char_p_<PLUS1>{&snip, ch};
  }

  template<t_bool PLUS1, t_bool INCL_CHAR>
  inline
  t_snip_char_p_<PLUS1, INCL_CHAR> snippet_in(r_snippet snip,
                                              t_char ch) noexcept {
    return t_snip_char_p_<PLUS1, INCL_CHAR>{&snip, ch};
  }

////////////////////////////////////////////////////////////////////////////////

  template<typename T>
  struct t_skip_v_ {
    t_skip_v_(const T& _value) noexcept : value(_value) { }
    T value;
  };

  template<typename T, t_bool PLUS1 = true>
  struct t_skip_until_v_ {
    t_skip_until_v_(const T& _value) noexcept : value{_value} { }
    T value;
  };

  enum  t_skip_n_v_tag_ {};
  using t_skip_n_v_ = t_explicit<t_n_, t_skip_n_v_tag_>;

  enum  t_skip_all_v_tag_ {};
  using t_skip_all_v_ = t_explicit<t_char, t_skip_all_v_tag_>;

////////////////////////////////////////////////////////////////////////////////

  template<typename T>
  inline t_skip_v_<T>                skip(const T& value) noexcept {
    return {value};
  }

  inline t_skip_n_v_                 skip_n(t_n value) noexcept {
    return t_skip_n_v_{get(value)};
  }

  template<typename T>
  inline t_skip_until_v_<T>          skip_until(const T& value) noexcept {
    return t_skip_until_v_<T>{value};
  }

  template<t_bool PLUS1, typename T>
  inline t_skip_until_v_<T, PLUS1>   skip_until(const T& value) noexcept {
    return t_skip_until_v_<T, PLUS1>{value};
  }

  inline t_skip_all_v_               skip_all(t_char value) noexcept {
    return t_skip_all_v_{value};
  }

  inline t_skip_all_v_               skip_spaces() noexcept {
    return t_skip_all_v_{' '};
  }

////////////////////////////////////////////////////////////////////////////////


  inline
  r_walk_ operator>>(r_walk_ lh, t_skip_v_<t_char> value) noexcept {
    return skip_(lh, value.value);
  }

  inline
  r_walk_ operator>>(r_walk_ lh, t_skip_v_<t_block> value) noexcept {
    return skip_(lh, value.value);
  }

  inline
  r_walk_ operator>>(r_walk_ lh, t_skip_v_<t_crange> value) noexcept {
    return skip_(lh, value.value);
  }

////////////////////////////////////////////////////////////////////////////////

  inline
  r_walk_ operator>>(r_walk_ lh, t_skip_n_v_ value) noexcept {
    return skip_(lh, get(value));
  }

  template<t_bool PLUS1>
  inline
  r_walk_ operator>>(r_walk_ lh,
                     t_skip_until_v_<t_char, PLUS1> value) noexcept {
    return skip_until_(lh, value.value, PLUS1);
  }

  template<t_bool PLUS1>
  inline
  r_walk_ operator>>(r_walk_ lh,
                     t_skip_until_v_<t_crange, PLUS1> value) noexcept {
    return skip_until_(lh, value.value, PLUS1);
  }

  inline
  r_walk_ operator>>(r_walk_ lh, t_skip_all_v_ value) noexcept {
    return skip_all_(lh, get(value));
  }

////////////////////////////////////////////////////////////////////////////////

  inline
  r_walk_ operator>>(r_walk_ lh, t_snip_n_p_ value) noexcept {
    return snip_n_(lh, value.value, value.n);
  }

  template<t_bool PLUS1, t_bool INCL_CHAR>
  inline
  r_walk_ operator>>(r_walk_ lh,
                     const t_snip_char_p_<PLUS1, INCL_CHAR>& value) noexcept {
    return snip_char_(lh, value.value, value.ch, PLUS1, INCL_CHAR);
  }

////////////////////////////////////////////////////////////////////////////////

  inline
  r_walk_ operator>>(r_walk_ lh, t_hex_p_<t_char> value) noexcept {
    t_n_ n = 0;
    return scan_(lh, n, 1, "%hhx%n", get(value), &n);
  }

  inline
  r_walk_ operator>>(r_walk_ lh, t_hex_p_<t_uchar> value) noexcept {
    t_n_ n = 0;
    return scan_(lh, n, 1, "%hhx%n", get(value), &n);
  }

  inline
  r_walk_ operator>>(r_walk_ lh, t_hex_p_<t_short> value) noexcept {
    t_n_ n = 0;
    return scan_(lh, n, 1, "%hhx%n", get(value), &n);
  }

  inline
  r_walk_ operator>>(r_walk_ lh, t_hex_p_<t_ushort> value) noexcept {
    t_n_ n = 0;
    return scan_(lh, n, 1, "%hhx%n", get(value), &n);
  }

  inline
  r_walk_ operator>>(r_walk_ lh, t_hex_p_<t_int> value) noexcept {
    t_n_ n = 0;
    return scan_(lh, n, 1, "%x%n", get(value), &n);
  }

  inline
  r_walk_ operator>>(r_walk_ lh, t_hex_p_<t_uint> value) noexcept {
    t_n_ n = 0;
    return scan_(lh, n, 1, "%x%n", get(value), &n);
  }

  inline
  r_walk_ operator>>(r_walk_ lh, t_hex_p_<t_long> value) noexcept {
    t_n_ n = 0;
    return scan_(lh, n, 1, "%lx%n", get(value), &n);
  }

  inline
  r_walk_ operator>>(r_walk_ lh, t_hex_p_<t_ulong> value) noexcept {
    t_n_ n = 0;
    return scan_(lh, n, 1, "%lx%n", get(value), &n);
  }

  inline
  r_walk_ operator>>(r_walk_ lh, t_hex_p_<t_llong> value) noexcept {
    t_n_ n = 0;
    return scan_(lh, n, 1, "%llx%n", get(value), &n);
  }

  inline
  r_walk_ operator>>(r_walk_ lh, t_hex_p_<t_ullong> value) noexcept {
    t_n_ n = 0;
    return scan_(lh, n, 1, "%llx%n", get(value), &n);
  }

////////////////////////////////////////////////////////////////////////////////

  inline
  r_walk_ operator>>(r_walk_ lh, t_int_p_<t_char> value) noexcept {
    t_n_ n = 0;
    return scan_(lh, n, 1, "%hhd%n", get(value), &n);
  }

  inline
  r_walk_ operator>>(r_walk_ lh, t_int_p_<t_uchar> value) noexcept {
    t_n_ n = 0;
    return scan_(lh, n, 1, "%hhu%n", get(value), &n);
  }

  inline
  r_walk_ operator>>(r_walk_ lh, t_int_p_<t_short> value) noexcept {
    t_n_ n = 0;
    return scan_(lh, n, 1, "%hhd%n", get(value), &n);
  }

  inline
  r_walk_ operator>>(r_walk_ lh, t_int_p_<t_ushort> value) noexcept {
    t_n_ n = 0;
    return scan_(lh, n, 1, "%hhu%n", get(value), &n);
  }

  inline
  r_walk_ operator>>(r_walk_ lh, t_int_p_<t_int> value) noexcept {
    t_n_ n = 0;
    return scan_(lh, n, 1, "%d%n$", get(value), &n);
  }

  inline
  r_walk_ operator>>(r_walk_ lh, t_int_p_<t_uint> value) noexcept {
    t_n_ n = 0;
    return scan_(lh, n, 1, "%u%n", get(value), &n);
  }

  inline
  r_walk_ operator>>(r_walk_ lh, t_int_p_<t_long> value) noexcept {
    t_n_ n = 0;
    return scan_(lh, n, 1, "%ld%n", get(value), &n);
  }

  inline
  r_walk_ operator>>(r_walk_ lh, t_int_p_<t_ulong> value) noexcept {
    t_n_ n = 0;
    return scan_(lh, n, 1, "%lu%n", get(value), &n);
  }

  inline
  r_walk_ operator>>(r_walk_ lh, t_int_p_<t_llong> value) noexcept {
    t_n_ n = 0;
    return scan_(lh, n, 1, "%lld%n", get(value), &n);
  }

  inline
  r_walk_ operator>>(r_walk_ lh, t_int_p_<t_ullong> value) noexcept {
    t_n_ n = 0;
    return scan_(lh, n, 1, "%llu%n", get(value), &n);
  }

////////////////////////////////////////////////////////////////////////////////

  template<class, t_n_, t_overflow> class t_string;

  template<class TAG, t_n_ N, t_overflow O>
  inline
  t_walk mk_walk(const t_string<TAG, N, O>& lh) noexcept {
    return lh.mk_range();
  }

////////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
