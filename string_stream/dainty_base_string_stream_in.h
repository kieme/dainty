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

#ifndef _DAINTY_BASE_STRING_STREAM_IN_H_
#define _DAINTY_BASE_STRING_STREAM_IN_H_

#include "impl_/dainty_base_string_impl.h"

namespace dainty
{
namespace base
{
namespace string
{
  using impl_::t_plus1_;
  using impl_::NOT_PLUS1;
  using impl_::PLUS1;

  using impl_::t_incl_char_;
  using impl_::NOT_INCL_CHAR;
  using impl_::INCL_CHAR;

  using impl_::t_eol_;
  using impl_::NOT_EOL_OK;
  using impl_::EOL_OK;
  using impl_::EOL;

  using impl_::P_cstr_;
  using impl_::P_cstr;

  // 1. t_hex_p_ - store a pointer to a hex value
  // -----------
  // 1.1 read_hex(lvalue) use template dedecution to determine which type of hex.
  //
  // 2. t_int_p_ - store a pointer to an integer value
  // -----------
  // 2.1 read_integer(lvalue) use template dedecution to determine which type of
  //     integer.
  //
  // 3. t_snippet - a character range that can be valid or not
  // ------------
  // 3.1 read_snippet(lvalue, t_n)           a snippet of t_n characters
  // 3.1 read_snippet<true>(lvalue, ch)      a snippet until ch, plus one
  // 3.1 read_snippet<true,true>(lvalue, ch) a snippet until and including ch
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
  template<typename T>
  using t_hex_p_ = specific::t_specific<T*, t_hex_p_tag_>;

  enum t_int_p_tag_ {};
  template<typename T>
  using t_int_p_ = specific::t_specific<T*, t_int_p_tag_>;

  template<typename T>
  constexpr t_hex_p_<T> read_hex(T& value)  noexcept {
    return t_hex_p_<T>{&value};
  }

  template<typename T>
  constexpr t_int_p_<T> read_integer(T& value)  noexcept {
    return t_int_p_<T>{&value};
  }

////////////////////////////////////////////////////////////////////////////////

  struct t_snip_n_p_ {
    constexpr
    t_snip_n_p_(p_snippet _value, t_n _n) noexcept
      : value{_value}, n{_n} {
    }

    p_snippet value;
    t_n       n;
  };

  template<t_plus1_     = PLUS1,
           t_incl_char_ = NOT_INCL_CHAR,
           t_eol_       = NOT_EOL_OK>
  struct t_snip_char_p_ {
    constexpr
    t_snip_char_p_(p_snippet _value, t_char _ch) noexcept
      : value{_value}, ch{_ch} {
    }

    p_snippet value;
    t_char    ch;
  };

  template<t_plus1_     = PLUS1,
           t_incl_char_ = NOT_INCL_CHAR,
           t_eol_       = NOT_EOL_OK>
  struct t_snip_char_select_p_ {
    constexpr
    t_snip_char_select_p_(p_snippet _value, p_char_select _select) noexcept
      : value{_value}, select{_select} {
    }

    p_snippet     value;
    p_char_select select;
  };

  constexpr
  t_snip_n_p_ read_snippet_upto(r_snippet snip, t_n n) noexcept {
    return t_snip_n_p_{&snip, n};
  }

  template<t_plus1_     PLUS1_ = PLUS1,
           t_incl_char_ INCL_CHAR_ = INCL_CHAR,
           t_eol_       EOL_OK_ = NOT_EOL_OK>
  constexpr
  t_snip_char_p_<PLUS1_, INCL_CHAR_, EOL_OK_>
      read_snippet_upto(r_snippet snip, t_char ch) noexcept {
    return t_snip_char_p_<PLUS1_, INCL_CHAR_, EOL_OK_>{&snip, ch};
  }

  template<t_plus1_     PLUS1_     = PLUS1,
           t_incl_char_ INCL_CHAR_ = INCL_CHAR,
           t_eol_       EOL_OK_    = NOT_EOL_OK>
  constexpr
  t_snip_char_select_p_<PLUS1_, INCL_CHAR_, EOL_OK_>
      read_snippet_upto(r_snippet snip, r_char_select select) noexcept {
    return t_snip_char_select_p_<PLUS1_, INCL_CHAR_, EOL_OK_>{&snip, &select};
  }

////////////////////////////////////////////////////////////////////////////////

  template<typename T>
  struct t_skip_v_ {
    constexpr t_skip_v_(const T& _value) noexcept : value(_value) {
    }
    T value;
  };

  template<typename T, t_plus1_ = PLUS1>
  struct t_skip_until_v_ {
    constexpr t_skip_until_v_(const T& _value) noexcept : value{_value} {
    }
    T value;
  };

  enum  t_skip_n_v_tag_ {};
  using t_skip_n_v_ = specific::t_specific<t_n_, t_skip_n_v_tag_>;

  enum  t_skip_all_v_tag_ {};
  using t_skip_all_v_ = specific::t_specific<t_char, t_skip_all_v_tag_>;

////////////////////////////////////////////////////////////////////////////////

  template<typename T>
  constexpr t_skip_v_<T>               skip(const T& value) noexcept {
    return {value};
  }

  constexpr t_skip_n_v_                skip_n(t_n value) noexcept {
    return t_skip_n_v_{get(value)};
  }

  template<typename T>
  constexpr t_skip_until_v_<T>         skip_until(const T& value) noexcept {
    return t_skip_until_v_<T>{value};
  }

  template<t_plus1_ PLUS1_, typename T>
  constexpr t_skip_until_v_<T, PLUS1_> skip_until(const T& value) noexcept {
    return t_skip_until_v_<T, PLUS1_>{value};
  }

  constexpr t_skip_all_v_              skip_all(t_char value) noexcept {
    return t_skip_all_v_{value};
  }

  constexpr t_skip_all_v_              skip_spaces() noexcept {
    return t_skip_all_v_{' '};
  }

////////////////////////////////////////////////////////////////////////////////

  class t_slider;
  using r_slider = t_prefix<t_slider>::r_;

  class t_slider : public t_snippet {
  public:
    using t_snippet::t_snippet;
    using t_snippet::operator t_crange;
    using t_snippet::operator t_validity;
    using t_snippet::operator=;

    constexpr r_slider operator+=(t_n n) noexcept {
      auto add_n = get(n);
      if (add_n <= n_) {
        ptr_ += add_n;
        n_   -= add_n;
      } else
        assertion::assert_now(P_cstr{"move to far"});
      return *this;
    }

    constexpr P_cstr_ begin() const noexcept {
      return ptr_;
    }

    constexpr P_cstr_ end() const noexcept {
      return ptr_ + n_;
    }
  };

////////////////////////////////////////////////////////////////////////////////

  inline
  r_slider operator>>(r_slider lh, t_skip_v_<t_char> value) noexcept {
    t_n n{skip_(lh, value.value)};
    return lh += n;
  }

  inline
  r_slider operator>>(r_slider lh, t_skip_v_<t_block> value) noexcept {
    t_n n{skip_(lh, value.value)};
    return lh += n;
  }

  inline
  r_slider operator>>(r_slider lh, t_skip_v_<t_crange> value) noexcept {
    t_n n{skip_(lh, value.value)};
    return lh += n;
  }

////////////////////////////////////////////////////////////////////////////////

  inline
  r_slider operator>>(r_slider lh, t_skip_n_v_ value) noexcept {
    t_n n{skip_(lh, get(value))};
    return lh += n;
  }

  template<t_plus1_ PLUS1_>
  inline
  r_slider operator>>(r_slider lh,
                     t_skip_until_v_<t_char, PLUS1_> value) noexcept {
    t_n n{skip_until_(lh, value.value, PLUS1_)};
    return lh += n;
  }

  template<t_plus1_ PLUS1_>
  inline
  r_slider operator>>(r_slider lh,
                     t_skip_until_v_<t_crange, PLUS1_> value) noexcept {
    t_n n{skip_until_(lh, value.value, PLUS1_)};
    return lh += n;
  }

  inline
  r_slider operator>>(r_slider lh, t_skip_all_v_ value) noexcept {
    t_n n{skip_all_(lh, get(value))};
    return lh += n;
  }

////////////////////////////////////////////////////////////////////////////////

  inline
  r_slider operator>>(r_slider lh, t_snip_n_p_ value) noexcept {
    t_n n{snip_n_(lh, value.value, value.n)};
    return lh += n;
  }

  template<t_plus1_ PLUS1_, t_incl_char_ INCL_CHAR_>
  inline
  r_slider operator>>(r_slider lh,
                     const t_snip_char_p_<PLUS1_, INCL_CHAR_,
                                          NOT_EOL_OK>& value) noexcept {
    t_n n{snip_char_(lh, value.value, value.ch, PLUS1_, INCL_CHAR_)};
    return lh += n;
  }

  template<t_plus1_ PLUS1_, t_incl_char_ INCL_CHAR_>
  inline
  r_slider operator>>(r_slider lh,
                     const t_snip_char_p_<PLUS1_, INCL_CHAR_,
                                          EOL_OK>& value) noexcept {
    t_n n{snip_char_eol_(lh, value.value, value.ch, PLUS1_, INCL_CHAR_)};
    return lh += n;
  }

  template<t_plus1_ PLUS1_, t_incl_char_ INCL_CHAR_>
  inline
  r_slider operator>>(r_slider lh,
                     t_snip_char_select_p_<PLUS1_, INCL_CHAR_,
                                           NOT_EOL_OK> value) noexcept {
    t_n n{snip_char_(lh, value.value, value.select, PLUS1_, INCL_CHAR_)};
    return lh += n;
  }

  template<t_plus1_ PLUS1_, t_incl_char_ INCL_CHAR_>
  inline
  r_slider operator>>(r_slider lh,
                     t_snip_char_select_p_<PLUS1_, INCL_CHAR_,
                                           EOL_OK> value) noexcept {
    t_n n{snip_char_eol_(lh, value.value, value.select, PLUS1_, INCL_CHAR_)};
    return lh += n;
  }

////////////////////////////////////////////////////////////////////////////////

  template<typename T>
  inline
  r_slider operator>>(r_slider lh, t_hex_p_<T> value) noexcept {
    return lh += to_hexidecimal(*get(value), P_cstr(lh.begin()));
  }

////////////////////////////////////////////////////////////////////////////////

  template<typename T>
  inline
  r_slider operator>>(r_slider lh, t_int_p_<T> value) noexcept {
    return lh += to_integer(*get(value), P_cstr(lh.begin()));
  }

////////////////////////////////////////////////////////////////////////////////

//  template<typename, t_n_, typename> class t_string;
//
//  template<typename TAG, t_n_ N, typename O>
//  inline
//  t_slider mk_slider(const t_string<TAG, N, O>& lh) noexcept {
//    return {lh.mk_range()};
//  }

////////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
