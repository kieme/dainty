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

#ifndef _DAINTY_NAMED_STRING_IMPL_H_
#define _DAINTY_NAMED_STRING_IMPL_H_

#include <stdarg.h>
#include "dainty_named.h"
#include "dainty_named_utility.h"
#include "dainty_named_range.h"
#include "dainty_named_ptr.h"
#include "dainty_named_assert.h"

namespace dainty
{
namespace named
{
namespace string
{
  using named::t_bool;
  using named::t_void;
  using named::p_cstr_;
  using named::P_cstr_;
  using named::P_cstr;
  using named::t_n_;
  using named::t_n;
  using named::t_ix_;
  using named::t_ix;
  using named::t_char;
  using named::t_int;
  using named::t_int;

  enum  t_crange_tag_ {};
  using t_crange = range::t_crange<t_char, t_crange_tag_>;
  using R_crange = t_prefix<t_crange>::R_;

  enum t_fmt { FMT };

///////////////////////////////////////////////////////////////////////////////

  template<t_n_ N>
  inline t_crange mk_range(const t_char (&value)[N]) noexcept {
    return t_crange{value, t_n{N-1}};
  }

  template<t_n_ N>
  inline t_crange mk_range(const t_char (&value)[N], t_ix begin) noexcept {
    return range::mk_crange<t_crange_tag_>(t_crange{value, t_n{N-1}}, begin);
  }

  template<t_n_ N>
  inline t_crange mk_range(const t_char (&value)[N],
                           t_ix begin, t_ix end) noexcept {
    return range::mk_crange<t_crange_tag_>(t_crange{value, t_n{N-1}}, begin,
                                                    end);
  }

  inline t_crange mk_range(R_crange range, t_ix begin) noexcept {
    return range::mk_crange<t_crange_tag_>(range, begin);
  }

  inline t_crange mk_range(R_crange range, t_ix begin, t_ix end) noexcept {
    return range::mk_crange<t_crange_tag_>(range, begin, end);
  }

////////////////////////////////////////////////////////////////////////////////

  enum t_overflow { OVERFLOW_ASSERT, OVERFLOW_TRUNCATE, OVERFLOW_GROW };

////////////////////////////////////////////////////////////////////////////////

  struct t_block {
    t_char c   = '\0';
    t_n    max = t_n{0};

    t_block() = default;
    t_block(t_char _c, t_n _max) : c(_c), max(_max) { };
  };
  using R_block = named::t_prefix<t_block>::R_;

////////////////////////////////////////////////////////////////////////////////

  t_n_ build_try_     (p_cstr_, t_n_, P_cstr_, va_list) noexcept;
  t_n_ build_assert_  (p_cstr_, t_n_, P_cstr_, va_list) noexcept;
  t_n_ build_truncate_(p_cstr_, t_n_, P_cstr_, va_list) noexcept;
  t_n_ copy_assert_   (p_cstr_, t_n_, P_cstr_, t_n_)    noexcept;
  t_n_ copy_truncate_ (p_cstr_, t_n_, P_cstr_, t_n_)    noexcept;
  t_n_ copy_assert_   (p_cstr_, t_n_, P_cstr_)          noexcept;
  t_n_ copy_truncate_ (p_cstr_, t_n_, P_cstr_)          noexcept;
  t_n_ fill_assert_   (p_cstr_, t_n_, R_block)          noexcept;
  t_n_ fill_truncate_ (p_cstr_, t_n_, R_block)          noexcept;

  t_n_     calc_n_   (t_n_, t_n_)                   noexcept;
  p_cstr_  alloc_    (t_n_)                         noexcept;
  p_cstr_  sso_alloc_(p_cstr_, t_n_, t_n_&)          noexcept; //XXX r_n_;
  p_cstr_  sso_alloc_(p_cstr_, t_n_, p_cstr_, t_n_) noexcept;
  t_void   dealloc_  (p_cstr_)                      noexcept;
  p_cstr_  realloc_  (p_cstr_, t_n_)                noexcept;

  t_void   display_  (P_cstr_)                  noexcept;
  t_int    compare_  (P_cstr_, P_cstr_)         noexcept;
  t_bool   match_    (P_cstr_, P_cstr_ pattern) noexcept;
  t_n_     count_    (t_char,  P_cstr_)         noexcept;
  t_n_     length_   (P_cstr_)                  noexcept;
  t_n_     length_   (P_cstr_, va_list)         noexcept;

////////////////////////////////////////////////////////////////////////////////

  struct t_del_ {
    inline t_void operator()(p_cstr_ cstr) noexcept {
      dealloc_(cstr);
    }
  };
  using t_ptr_ = ptr::t_ptr<t_char, p_cstr_, t_del_>;

////////////////////////////////////////////////////////////////////////////////

  class t_string_impl_base_ {
  public:
    using t_char   = named::t_char;
    using R_block = string::R_block;

    inline t_string_impl_base_() noexcept : len_{0} {
    }

    inline t_string_impl_base_(t_n_ len) noexcept : len_{len} {
    }

    inline t_string_impl_base_(p_cstr_ str) noexcept : len_{0} {
      str[0] = '\0';
    }

    inline t_void clear(p_cstr_ str) noexcept {
      str[0] = '\0';
      len_ = 0;
    }

    inline t_void display(P_cstr_ str) const noexcept {
      if (len_)
        display_(str);
    }

    inline t_void display_then_clear(p_cstr_ str) noexcept {
      if (len_) {
        display_(str);
        clear(str);
      }
    }

    inline t_bool is_match(P_cstr_ str, P_cstr_ pattern) const noexcept {
      return match_(str, pattern);
    }

    inline t_bool is_empty() const noexcept {
      return len_ == 0;
    }

    inline P_cstr_ get_cstr(P_cstr_ str) const noexcept {
      return str;
    }

    inline t_n_ reset(t_n_ len = 0) noexcept {
      return utility::reset(len_, len);
    }

    inline t_n_ get_length() const noexcept {
      return len_;
    }

    inline t_n_ get_count(P_cstr_ str, t_char c) const noexcept {
      return count_(c, str);
    }

    inline t_char get_front(P_cstr_ str) const noexcept {
      return len_ ? str[0] : '\0';
    }

    inline t_char get_back(P_cstr_ str) const noexcept {
      return len_ ? str[len_ - 1] : '\0';
    }

    inline t_crange mk_range(P_cstr_ str) const noexcept {
      return t_crange{str, t_n{len_}};
    }

    inline t_crange mk_range(P_cstr_ str, t_ix begin) const noexcept {
      return range::mk_crange<t_crange_tag_>(t_crange{str, t_n{len_}},
                                             begin, t_ix{len_});
    }

    inline t_crange mk_range(P_cstr_ str, t_ix begin,
                             t_ix end) const noexcept {
      return range::mk_crange<t_crange_tag_>(t_crange{str, t_n{len_}},
                                             begin, end);
    }

    template<class F>
    inline t_void each(P_cstr_ str, F f) noexcept {
      for (t_n_ n = 0; n < len_; ++n)
        f(str[n]);
    }

    template<class F>
    inline t_void each(P_cstr_ str, F f) const noexcept {
      for (t_n_ n = 0; n < len_; ++n)
        f(str[n]);
    }

    inline t_void mod_(p_cstr_ str, t_ix_ pos, t_char ch) noexcept {
      if (pos < len_)
        str[pos] = ch;
      else
        assert_now(P_cstr{"not in range"});
    }

    inline t_n_ va_assign_try(p_cstr_ str, t_n_ max, P_cstr_ fmt,
                              va_list vars) noexcept {
      auto n = build_try_(str, max, fmt, vars);
      if (n < max)
        len_ = n;
      return n;
    }

    inline t_n_ va_append_try(p_cstr_ str, t_n_ max, P_cstr_ fmt,
                              va_list vars) noexcept {
      auto left = max - len_;
      auto n = build_try_(str + len_, left, fmt, vars);
      if (n < left)
        len_ += n;
      return n;
    }

  protected:
    t_n_ len_ = 0;
  };

///////////////////////////////////////////////////////////////////////////////

  template<t_overflow> class t_string_impl_;

  template<>
  class t_string_impl_<OVERFLOW_ASSERT> : public t_string_impl_base_ {
  public:
    using base_ = t_string_impl_base_;

    inline t_string_impl_(t_n_ len) noexcept : base_{len} {
    }

    inline t_string_impl_(p_cstr_ str) noexcept : base_{str} {
    }

    inline t_string_impl_(p_cstr_ str, t_n_ max, P_cstr_ src) noexcept
      : base_{copy_assert_(str, max, src)} {
    }

    inline t_string_impl_(p_cstr_ str, t_n_ max, P_cstr_ src,
                          t_n_ cnt) noexcept
      : base_{copy_assert_(str, max, src, cnt)} {
    }

    inline t_string_impl_(p_cstr_ str, t_n_ max, R_block block) noexcept
      : base_{fill_assert_(str, max, block)} {
    }

    inline t_void assign(p_cstr_ str, t_n_ max, P_cstr_ src) noexcept {
      len_ = copy_assert_(str, max, src);
    }

    inline t_void assign(p_cstr_ str, t_n_ max, P_cstr_ src,
                         t_n_ cnt) noexcept {
      len_ = copy_assert_(str, max, src, cnt);
    }

    inline t_void assign(p_cstr_ str, t_n_ max, R_block block) noexcept {
      len_ = fill_assert_(str, max, block);
    }

    inline t_void append(p_cstr_ str, t_n_ max, P_cstr_ src) noexcept {
      len_ += copy_assert_(str + len_, max - len_, src);
    }

    inline t_void append(p_cstr_ str, t_n_ max, P_cstr_ src,
                         t_n_ cnt) noexcept {
      len_ += copy_assert_(str + len_, max - len_, src, cnt);
    }

    inline t_void append(p_cstr_ str, t_n_ max, R_block block) noexcept {
      len_ += fill_assert_(str + len_, max - len_, block);
    }

    inline t_void va_assign(p_cstr_ str, t_n_ max, P_cstr_ fmt,
                            va_list vars) noexcept {
      len_ = build_assert_(str, max, fmt, vars);
    }

    inline t_void va_append(p_cstr_ str, t_n_ max, P_cstr_ fmt,
                            va_list vars) noexcept {
      len_ += build_assert_(str + len_, max - len_, fmt, vars);
    }
  };

///////////////////////////////////////////////////////////////////////////////

  template<>
  class t_string_impl_<OVERFLOW_TRUNCATE> : public t_string_impl_base_ {
  public:
    using base_ = t_string_impl_base_;

    inline t_string_impl_(t_n_ len) noexcept : base_{len} {
    }

    inline t_string_impl_(p_cstr_ str) noexcept : base_{str} {
    }

    inline t_string_impl_(p_cstr_ str, t_n_ max, P_cstr_ src) noexcept
      : base_{copy_truncate_(str, max, src)} {
    }

    inline t_string_impl_(p_cstr_ str, t_n_ max, P_cstr_ src,
                          t_n_ cnt) noexcept
      : base_{copy_truncate_(str, max, src, cnt)} {
    }

    inline t_string_impl_(p_cstr_ str, t_n_ max, R_block block) noexcept
      : base_{fill_truncate_(str, max, block)} {
    }

    inline t_void assign(p_cstr_ str, t_n_ max, P_cstr_ src) noexcept {
      len_ = copy_truncate_(str, max, src);
    }

    inline t_void assign(p_cstr_ str, t_n_ max, P_cstr_ src,
                         t_n_ cnt) noexcept {
      len_ = copy_truncate_(str, max, src, cnt);
    }

    inline t_void assign(p_cstr_ str, t_n_ max, R_block block) noexcept {
      len_ = fill_truncate_(str, max, block);
    }

    inline t_void append(p_cstr_ str, t_n_ max, P_cstr_ src) noexcept {
      len_ += copy_truncate_(str + len_, max - len_, src);
    }

    inline t_void append(p_cstr_ str, t_n_ max, P_cstr_ src,
                         t_n_ cnt) noexcept {
      len_ += copy_truncate_(str + len_, max - len_, src, cnt);
    }

    inline t_void append(p_cstr_ str, t_n_ max, R_block block) noexcept {
      len_ += fill_truncate_(str + len_, max - len_, block);
    }

    inline t_void va_assign(p_cstr_ str, t_n_ max, P_cstr_ fmt,
                            va_list vars) noexcept {
      len_ = build_truncate_(str, max, fmt, vars);
    }

    inline t_void va_append(p_cstr_ str, t_n_ max, P_cstr_ fmt,
                            va_list vars) noexcept {
      len_ += build_truncate_(str + len_, max - len_, fmt, vars);
    }
  };

///////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
