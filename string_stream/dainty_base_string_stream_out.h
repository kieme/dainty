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

#ifndef _DAINTY_BASE_STRING_STREAM_OUT_H_
#define _DAINTY_BASE_STRING_STREAM_OUT_H_

#include "impl_/dainty_base_string_impl.h"

// stream out types (<<)
//
// 1. t_block
// ----------
// 1.1  SP - space " "
// 1.2  SP_2       "  "
// 1.3  SP_3       "   "
// 1.4  SP_4       "    "
// 1.5  SP_5       "     "
// 1.6  spaces<N>()
// 1.7  spaces(t_n)
// 1.8  NL - newline "\n"
// 1.9  NL_2         "\n\n"
// 1.10 NL_3         "\n\n\n"
// 1.11 NL_4         "\n\n\n\n"
// 1.12 NL_5         "\n\n\n\n\n"
// 1.13 newlines<N>()
// 1.14 newlines(t_n)
// 1.15 line_of(t_n, t_char ch)  t_n ch characters. e.g. 3 '.' == "..."
// 1.16 line(t_n)                t_n '-' characters. e.g. 4 '-' == "----"
// 1.17 double_line(t_n)         t_n '=' characters. e.g. 6 '=' == "======"
//
// 2. t_indent_v_ - indention
// --------------
// 2.1  indent(3)        use default: char = ' ', depth=2. result="      "
// 2.2  indent<4>(2)                  char = ' ', depth=4, result="        "
// 2.3  indent<3,'#'>(2)              char = '#', depth=3, result="######"
// 2.4  INDENT_0    ""
// 2.5  INDENT_1    + "    "
// 2.6  INDENT_2    + "    "
// 2.7  INDENT_3    + "    "
// 2.8  INDENT_4    + "    "
// 2.9  INDENT_5    ...
// 2.10 INDENT_6
// 2.11 INDENT_7
// 2.12 INDENT_8
// 2.13 INDENT_9
// 2.14 INDENT_10
// 2.15 INDENT_11
// 2.16 INDENT_12
// 2.17 INDENT_13
// 2.18 INDENT_14
// 2.19 INDENT_15 + "    ".  // 15*4 = 60 characters
//
// 3. t_fmt_v_ - value(e.g.integer,hex,ptr) formatting with width and alignment
// -----------
// 3.1 format<5>(integer(1))     width=5,align=right,value=1,result="    1"
// 3.1 format<5,ALIGN_LEFT>(...) width=5,align=left, value=1,result="1    "
//
// 4. t_int_v_ - represent an integer type - can be used with format
// -----------
// 4.1  integer(value) -> type deducation is used to construct t_int_v_
//
// 5. t_hex_v_ - represent a hexidecimal value - can be used with format
// -----------
// 5.1  hex(value) -> type deducation is used to construct t_hex_v_
//
// 6. t_ptr_v_ - represent a pointer value - can be used with format
// -----------
// 6.1  pointer(value) -> type deducation is used to construct t_ptr_v_
//

namespace dainty
{
namespace base
{
namespace string
{
  using impl_::t_int;
  using impl_::t_ullong;
  using impl_::t_uintptr;
  using impl_::p_cstr_;
  using impl_::t_buf_range;
  using impl_::OVERFLOW_ASSERT;
  using impl_::OVERFLOW_TRUNCATE;
  using impl_::OVERFLOW_GROW;

////////////////////////////////////////////////////////////////////////////////

  template<class TAG, t_n_ N, typename O> class t_string;

////////////////////////////////////////////////////////////////////////////////

  constexpr t_block SP  {' ', t_n{1}};
  constexpr t_block SP_2{' ', t_n{2}};
  constexpr t_block SP_3{' ', t_n{3}};
  constexpr t_block SP_4{' ', t_n{4}};
  constexpr t_block SP_5{' ', t_n{5}};

  template<t_n_ N>
  constexpr t_block spaces()      noexcept { return t_block{' ', t_n{N}}; }
  constexpr t_block spaces(t_n n) noexcept { return t_block{'\n', n}; }

////////////////////////////////////////////////////////////////////////////////

  constexpr t_block NL  {'\n', t_n{1}};
  constexpr t_block NL_2{'\n', t_n{2}};
  constexpr t_block NL_3{'\n', t_n{3}};
  constexpr t_block NL_4{'\n', t_n{4}};
  constexpr t_block NL_5{'\n', t_n{5}};

  template<t_n_ N>
  constexpr t_block newlines()      noexcept { return t_block{'\n', t_n{N}}; }
  constexpr t_block newlines(t_n n) noexcept { return t_block{'\n', n}; }

////////////////////////////////////////////////////////////////////////////////

  constexpr t_block line_of(t_n n, t_char ch) noexcept {
    return t_block{ch, n};
  }

  constexpr t_block line   (t_n n) noexcept     { return line_of(n, '-'); }
  constexpr t_block double_line(t_n n) noexcept { return line_of(n, '='); }

////////////////////////////////////////////////////////////////////////////////

  enum  t_indent_no_tag_ {};
  using t_indent_no_ = t_int;
  using t_indent_no  = specific::t_specific<t_indent_no_, t_indent_no_tag_>;

  template<t_n_ INDENT_DEPTH = 2, t_char INDENT_CHAR = ' '>
  struct t_indent_v_ {
    constexpr t_indent_v_(t_indent_no _value) noexcept : value(_value) { }
    t_indent_no value;
  };

  constexpr t_indent_v_<> indent(t_indent_no no) noexcept {
    return t_indent_v_<>(no);
  }

  template<t_n_ INDENT_DEPTH>
  constexpr t_indent_v_<INDENT_DEPTH> indent(t_indent_no no) noexcept {
    return t_indent_v_<INDENT_DEPTH>(no);
  }

  template<t_n_ INDENT_DEPTH , t_char INDENT_CHAR>
  constexpr t_indent_v_<INDENT_DEPTH, INDENT_CHAR>
      indent(t_indent_no no) noexcept {
    return t_indent_v_<INDENT_DEPTH, INDENT_CHAR>(no);
  }

  using t_indent_def_ = t_indent_v_<4, ' '>;
  constexpr t_indent_def_ INDENT_0 {t_indent_no{0}};
  constexpr t_indent_def_ INDENT_1 {t_indent_no{1}};
  constexpr t_indent_def_ INDENT_2 {t_indent_no{2}};
  constexpr t_indent_def_ INDENT_3 {t_indent_no{3}};
  constexpr t_indent_def_ INDENT_4 {t_indent_no{4}};
  constexpr t_indent_def_ INDENT_5 {t_indent_no{5}};
  constexpr t_indent_def_ INDENT_6 {t_indent_no{6}};
  constexpr t_indent_def_ INDENT_7 {t_indent_no{7}};
  constexpr t_indent_def_ INDENT_8 {t_indent_no{8}};
  constexpr t_indent_def_ INDENT_9 {t_indent_no{9}};
  constexpr t_indent_def_ INDENT_10{t_indent_no{10}};
  constexpr t_indent_def_ INDENT_11{t_indent_no{11}};
  constexpr t_indent_def_ INDENT_12{t_indent_no{12}};
  constexpr t_indent_def_ INDENT_13{t_indent_no{13}};
  constexpr t_indent_def_ INDENT_14{t_indent_no{14}};
  constexpr t_indent_def_ INDENT_15{t_indent_no{15}};

////////////////////////////////////////////////////////////////////////////////

  enum t_hex_v_tag_ {};
  template<typename T>
  using t_hex_v_ = specific::t_specific<T, t_hex_v_tag_>;

  enum t_int_v_tag_ {};
  template<typename T>
  using t_int_v_ = specific::t_specific<T, t_int_v_tag_>;

  enum t_ptr_v_tag_ {};
  template<typename T>
  using t_ptr_v_ = specific::t_specific<T, t_ptr_v_tag_>;

  template<typename T>
  constexpr
  t_hex_v_<T> hex(T value) noexcept {
    return t_hex_v_<T>{value};
  }

  template<typename T>
  constexpr
  t_int_v_<T> integer(T value) noexcept {
    return t_int_v_<T>{value};
  }

  template<typename T>
  constexpr
  t_ptr_v_<T*> pointer(T* value) noexcept {
    return t_ptr_v_<T*>{value};
  }

////////////////////////////////////////////////////////////////////////////////

  template<class TAG, t_n_ N, typename O>
  inline
  t_string<TAG, N, O>& operator<<(t_string<TAG, N, O>& lh, t_char rh) noexcept {
    return lh.append(t_block{rh, t_n{1}});
  }

  template<class TAG, t_n_ N, typename O>
  inline
  t_string<TAG, N, O>& operator<<(t_string<TAG, N, O>& lh, P_cstr rh) noexcept {
    return lh.append(rh);
  }

  template<class TAG, class TAG1, t_n_ N, t_n_ N1, typename O, typename O1>
  inline
  t_string<TAG, N, O>& operator<<(t_string<TAG, N, O>& lh,
                                  const t_string<TAG1, N1, O1>& rh) noexcept {
    return lh.append(rh);
  }

////////////////////////////////////////////////////////////////////////////////

  template<typename T>
  struct t_int_to_hex_str_helper_ {
    constexpr
    t_int_to_hex_str_helper_(const T& _value) noexcept : value{_value} {
    }

    template<typename O>
    inline
    t_n operator()(t_buf_range range, O) noexcept {
      return impl_::hex_to_str_(range, static_cast<t_ullong>(value));
    }

    T value;
  };

  template<class TAG, t_n_ N, typename O, typename T>
  inline
  t_string<TAG, N, O>& operator<<(t_string<TAG, N, O>& lh,
                                  t_hex_v_<T> value) noexcept {
    t_int_to_hex_str_helper_<T> helper{get(value)};
    return lh.custom_append_(helper);
  }

////////////////////////////////////////////////////////////////////////////////

  template<class TAG, t_n_ N, typename O, typename P>
  inline
  t_string<TAG, N, O>& operator<<(t_string<TAG, N, O>& lh,
                                  t_ptr_v_<P> value) noexcept {
    t_int_to_hex_str_helper_<t_uintptr>
      helper{static_cast<t_uintptr>(get(value))};
    return lh.custom_append_(helper);
  }

////////////////////////////////////////////////////////////////////////////////

  template<typename T, typename = traits::t_is_unsigned_integral<T>>
  struct t_int_to_str_helper_ {
    constexpr
    t_int_to_str_helper_(const T& _value) noexcept : value{_value} {
    }

    template<typename O>
    inline
    t_n operator()(t_buf_range range, O) noexcept {
      return impl_::uint_to_str_(range, value);
    }

    T value;
  };

  template<typename T>
  struct t_int_to_str_helper_<T, traits::t_false> {
    constexpr
    t_int_to_str_helper_(const T& _value) noexcept : value{_value} {
    }

    template<typename O>
    inline
    t_n operator()(t_buf_range range, O) noexcept {
      return impl_::int_to_str_(range, value);
    }

    T value;
  };

  template<class TAG, t_n_ N, typename O, typename T>
  inline
  t_string<TAG, N, O>& operator<<(t_string<TAG, N, O>& lh,
                                  t_int_v_<T> value) noexcept {
    t_int_to_str_helper_<T> helper{get(value)};
    return lh.custom_append_(helper);
  }

////////////////////////////////////////////////////////////////////////////////

  enum t_align { ALIGN_RIGHT, ALIGN_LEFT, ALIGN_CENTRE };

  enum  t_width_tag_ {};
  using t_width_ = t_int;
  using t_width  = specific::t_specific<t_width_, t_width_tag_>;

  template<typename T, t_width_ WIDTH, t_align ALIGN = ALIGN_RIGHT>
  struct t_fmt_v_;

////////////////////////////////////////////////////////////////////////////////

  template<typename H>
  inline
  t_n add_left_(t_buf_range range, H& helper, t_width width,
                t_overflow_assert) noexcept {
    t_n n{width};
    return n < range.n ?
      impl_::shift_left_(range, helper(range, OVERFLOW_ASSERT), width) : n;
  }

  template<typename H>
  inline
  t_n add_left_(t_buf_range range, H& helper, t_width width,
                t_overflow_truncate) noexcept {
    return impl_::shift_left_(range, helper(range, OVERFLOW_TRUNCATE), width);
  }

  template<typename H>
  inline
  t_n add_left_(t_buf_range range, H& helper, t_width width,
                t_overflow_grow) noexcept {
    t_n n{width};
    return n < range.n ?
      impl_::shift_left_(range, helper(range, OVERFLOW_GROW), width) : n;
  }

  template<typename H>
  inline
  t_n add_right_(t_buf_range range, H& helper, t_width width,
                 t_overflow_assert) {
    t_n n{width};
    return n < range.n ?
      impl_::shift_right_(range, helper(range, OVERFLOW_ASSERT), width) : n;
  }

  template<typename H>
  inline
  t_n add_right_(t_buf_range range, H& helper, t_width width,
                 t_overflow_truncate) {
    return impl_::shift_right_(range, helper(range, OVERFLOW_TRUNCATE), width);
  }

  template<typename H>
  inline
  t_n add_right_(t_buf_range range, H& helper, t_width width,
                 t_overflow_grow) {
    t_n n{width};
    return n < range.n ?
      impl_::shift_right_(range, helper(range, OVERFLOW_GROW), width) : n;
  }

  template<typename H>
  inline
  t_n add_centre_(t_buf_range range, H& helper, t_width width,
                  t_overflow_assert) {
    t_n n{width};
    return n < range.n ?
      impl_::shift_centre_(range, helper(range, OVERFLOW_ASSERT), width) : n;
  }

  template<typename H>
  inline
  t_n add_centre_(t_buf_range range, H& helper, t_width width,
                  t_overflow_truncate) {
    return impl_::shift_centre_(range, helper(range, OVERFLOW_TRUNCATE), width);
  }

  template<typename H>
  inline
  t_n add_centre_(t_buf_range range, H& helper, t_width width,
                  t_overflow_grow) {
    t_n n{width};
    return n < range.n ?
      shift_centre_(range, helper(range, OVERFLOW_GROW), width) : n;
  }

////////////////////////////////////////////////////////////////////////////////

  template<typename T, t_width_ WIDTH>
  struct t_fmt_v_<t_int_v_<T>, WIDTH, ALIGN_LEFT> {
    constexpr
    t_fmt_v_(t_int_v_<T> value) noexcept : helper_(get(value)) {
    }

    template<typename O>
    inline
     t_n operator()(t_buf_range range, O overflow) noexcept {
      return add_left_(range, helper_, t_width{WIDTH}, overflow);
    }

    t_int_to_str_helper_<T> helper_;
  };

  template<typename T, t_width_ WIDTH>
  struct t_fmt_v_<t_int_v_<T>, WIDTH, ALIGN_RIGHT> {
    constexpr
    t_fmt_v_(t_int_v_<T> value) noexcept : helper_(get(value)) {
    }

    template<typename O>
    inline
    t_n operator()(t_buf_range range, O overflow) noexcept {
      return add_right_(range, helper_, t_width{WIDTH}, overflow);
    }

    t_int_to_str_helper_<T> helper_;
  };

  template<typename T, t_width_ WIDTH>
  struct t_fmt_v_<t_int_v_<T>, WIDTH, ALIGN_CENTRE> {
    constexpr
    t_fmt_v_(t_int_v_<T> value) noexcept : helper_(get(value)) {
    }

    template<typename O>
    inline
    t_n operator()(t_buf_range range, O overflow) noexcept {
      return add_centre_(range, helper_, t_width{WIDTH}, overflow);
    }

    t_int_to_str_helper_<T> helper_;
  };

////////////////////////////////////////////////////////////////////////////////

  template<typename T, t_width_ WIDTH>
  struct t_fmt_v_<t_hex_v_<T>, WIDTH, ALIGN_LEFT> {
    constexpr
    t_fmt_v_(t_hex_v_<T> value) noexcept : helper_(get(value)) {
    }

    template<typename O>
    inline
    t_n operator()(t_buf_range range, O overflow) noexcept {
      return add_left_(range, helper_, t_width{WIDTH}, overflow);
    }

    t_int_to_hex_str_helper_<T> helper_;
  };

  template<typename T, t_width_ WIDTH>
  struct t_fmt_v_<t_hex_v_<T>, WIDTH, ALIGN_RIGHT> {
    constexpr
    t_fmt_v_(t_hex_v_<T> value) noexcept : helper_(get(value)) {
    }

    template<typename O>
    inline
    t_n operator()(t_buf_range range, O overflow) noexcept {
      return add_right_(range, helper_, t_width{WIDTH}, overflow);
    }

    t_int_to_hex_str_helper_<T> helper_;
  };

  template<typename T, t_width_ WIDTH>
  struct t_fmt_v_<t_hex_v_<T>, WIDTH, ALIGN_CENTRE> {
    constexpr
    t_fmt_v_(t_hex_v_<T> value) noexcept : helper_(get(value)) {
    }

    template<typename O>
    inline
    t_n operator()(t_buf_range range, O overflow) noexcept {
      return add_centre_(range, helper_, t_width{WIDTH}, overflow);
    }

    t_int_to_hex_str_helper_<T> helper_;
  };

////////////////////////////////////////////////////////////////////////////////

  template<typename T, t_width_ WIDTH>
  struct t_fmt_v_<t_ptr_v_<T>, WIDTH, ALIGN_LEFT> {
    constexpr
    t_fmt_v_(t_ptr_v_<T> value) noexcept
      : helper_{reinterpret_cast<t_uintptr>(get(value))} {
    }

    template<typename O>
    inline
    t_n operator()(t_buf_range range, O overflow) noexcept {
      return add_left_(range, helper_, t_width{WIDTH}, overflow);
    }

    t_int_to_hex_str_helper_<t_uintptr> helper_;
  };

  template<typename T, t_width_ WIDTH>
  struct t_fmt_v_<t_ptr_v_<T>, WIDTH, ALIGN_RIGHT> {
    constexpr
    t_fmt_v_(t_ptr_v_<T> value) noexcept
      : helper_{reinterpret_cast<t_uintptr>(get(value))} {
    }

    template<typename O>
    inline
    t_n operator()(t_buf_range range, O overflow) noexcept {
      return add_right_(range, helper_, t_width{WIDTH}, overflow);
    }

    t_int_to_hex_str_helper_<t_uintptr> helper_;
  };

  template<typename T, t_width_ WIDTH>
  struct t_fmt_v_<t_ptr_v_<T>, WIDTH, ALIGN_CENTRE> {
    constexpr
    t_fmt_v_(t_ptr_v_<T> value) noexcept
      : helper_{reinterpret_cast<t_uintptr>(get(value))} {
    }

    template<typename O>
    inline
    t_n operator()(t_buf_range range, O overflow) noexcept {
      return add_centre_(range, helper_, t_width{WIDTH}, overflow);
    }

    t_int_to_hex_str_helper_<t_uintptr> helper_;
  };

///////////////////////////////////////////////////////////////////////////////

  struct t_crange_to_str_helper_ {
    constexpr
    t_crange_to_str_helper_(t_crange _value) noexcept : value{_value} {
    }

    inline
    t_n operator()(t_buf_range range, t_overflow_assert) noexcept {
      return impl_::copy_assert_(range, value);
    }

    inline
    t_n operator()(t_buf_range range, t_overflow_truncate) noexcept {
      return impl_::copy_truncate_(range, value);
    }

    inline
    t_n operator()(t_buf_range range, t_overflow_grow) noexcept {
      return impl_::copy_truncate_(range, value);
    }

    t_crange value;
  };

  template<t_width_ WIDTH>
  struct t_fmt_v_<t_crange, WIDTH, ALIGN_LEFT> {
    constexpr
    t_fmt_v_(t_crange _value) noexcept : helper_(_value) {
    }

    template<typename O>
    inline
     t_n operator()(t_buf_range range, O overflow) noexcept {
      return add_left_(range, helper_, t_width{WIDTH}, overflow);
    }

    t_crange_to_str_helper_ helper_;
  };

  template<t_width_ WIDTH>
  struct t_fmt_v_<t_crange, WIDTH, ALIGN_RIGHT> {
    constexpr
    t_fmt_v_(t_crange _value) noexcept : helper_(_value) {
    }

    template<typename O>
    inline
    t_n operator()(t_buf_range range, O overflow) noexcept {
      return add_right_(range, helper_, t_width{WIDTH}, overflow);
    }

    t_crange_to_str_helper_ helper_;
  };

  template<t_width_ WIDTH>
  struct t_fmt_v_<t_crange, WIDTH, ALIGN_CENTRE> {
    constexpr
    t_fmt_v_(t_crange _value) noexcept : helper_(_value) {
    }

    template<typename O>
    inline
    t_n operator()(t_buf_range range, O overflow) noexcept {
      return add_centre_(range, helper_, t_width{WIDTH}, overflow);
    }

    t_crange_to_str_helper_ helper_;
  };

///////////////////////////////////////////////////////////////////////////////

  template<t_width_ WIDTH>
  constexpr
  t_fmt_v_<t_crange, WIDTH> format(t_crange value) noexcept {
    return t_fmt_v_<t_crange, WIDTH>{value};
  }

  template<t_width_ WIDTH, t_align ALIGN>
  constexpr
  t_fmt_v_<t_crange, WIDTH, ALIGN> format(t_crange value) noexcept {
    return t_fmt_v_<t_crange, WIDTH, ALIGN>{value};
  }

  template<t_width_ WIDTH, typename T>
  constexpr
  t_fmt_v_<t_int_v_<T>, WIDTH> format(t_int_v_<T> value) noexcept {
    return t_fmt_v_<t_int_v_<T>, WIDTH>{value};
  }

  template<t_width_ WIDTH, t_align ALIGN, typename T>
  constexpr
  t_fmt_v_<t_int_v_<T>, WIDTH, ALIGN> format(t_int_v_<T> value) noexcept {
    return t_fmt_v_<t_int_v_<T>, WIDTH, ALIGN>{value};
  }

  template<t_width_ WIDTH, typename T>
  constexpr
  t_fmt_v_<t_hex_v_<T>, WIDTH> format(t_hex_v_<T> value) noexcept {
    return t_fmt_v_<t_hex_v_<T>, WIDTH>{value};
  }

  template<t_width_ WIDTH, t_align ALIGN, typename T>
  constexpr
  t_fmt_v_<t_hex_v_<T>, WIDTH, ALIGN> format(t_hex_v_<T> value) noexcept {
    return t_fmt_v_<t_hex_v_<T>, WIDTH, ALIGN>{value};
  }

  template<t_width_ WIDTH, typename T>
  constexpr
  t_fmt_v_<t_ptr_v_<T>, WIDTH> format(t_ptr_v_<T> value) noexcept {
    return t_fmt_v_<t_ptr_v_<T>, WIDTH>{value};
  }

  template<t_width_ WIDTH, t_align ALIGN, typename T>
  constexpr
  t_fmt_v_<t_ptr_v_<T>, WIDTH, ALIGN> format(t_ptr_v_<T> value) noexcept {
    return t_fmt_v_<t_ptr_v_<T>, WIDTH, ALIGN>{value};
  }

////////////////////////////////////////////////////////////////////////////////

  template<class TAG, t_n_ N, typename O, t_width_ WIDTH, t_align ALIGN,
           typename T>
  inline
  t_string<TAG, N, O>& operator<<(t_string<TAG, N, O>& lh,
                                  t_fmt_v_<T, WIDTH, ALIGN> fmt) noexcept {
    return lh.custom_append_(fmt);
  }

////////////////////////////////////////////////////////////////////////////////

  template<class TAG, t_n_ N, typename O, t_n_ INDENT_DEPTH,
           t_char INDENT_CHAR>
  inline
  t_string<TAG, N, O>& operator<<(t_string<TAG, N, O>& lh,
                                  t_indent_v_<INDENT_DEPTH,
                                              INDENT_CHAR> value) noexcept {
    if (!lh.is_empty() && lh.get_back() != '\n')
      lh.append(t_block{'\n', t_n{1}});
    return lh.append(t_block{INDENT_CHAR, t_n{get(value.value)*INDENT_DEPTH}});
  }

////////////////////////////////////////////////////////////////////////////////

  template<class TAG, t_n_ N, typename O, class T>
  inline
  t_string<TAG, N, O>& operator<<(t_string<TAG, N, O>& lh,
                                  const T& rh) noexcept {
    return lh.append(rh);
  }

////////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
