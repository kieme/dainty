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
  using named::t_char;
  using named::t_int;

////////////////////////////////////////////////////////////////////////////////

  enum t_overflow_assert   { };
  enum t_overflow_truncate { };

////////////////////////////////////////////////////////////////////////////////

  struct t_block {
    t_char c   = '\0';
    t_n    max = t_n{0};

    t_block() = default;
    t_block(t_char _c, t_n _max) : c(_c), max(_max) { };
  };

  using R_block = named::t_prefix<t_block>::R_;

////////////////////////////////////////////////////////////////////////////////

  t_n_ build_(p_cstr_, t_n_, P_cstr_, va_list);
  t_n_ build_(p_cstr_, t_n_, P_cstr_, va_list, t_overflow_assert);
  t_n_ build_(p_cstr_, t_n_, P_cstr_, va_list, t_overflow_truncate);
  t_n_ copy_ (p_cstr_, t_n_, P_cstr_, t_n_,    t_overflow_assert);
  t_n_ copy_ (p_cstr_, t_n_, P_cstr_, t_n_,    t_overflow_truncate);
  t_n_ copy_ (p_cstr_, t_n_, P_cstr_,          t_overflow_assert);
  t_n_ copy_ (p_cstr_, t_n_, P_cstr_,          t_overflow_truncate);
  t_n_ fill_ (p_cstr_, t_n_, R_block,          t_overflow_assert);
  t_n_ fill_ (p_cstr_, t_n_, R_block,          t_overflow_truncate);

  t_n_    calc_chr_(t_n_, t_n_);
  p_cstr_ alloc_   (t_n_);
  t_void  dealloc_ (p_cstr_);
  p_cstr_ realloc_ (p_cstr_, t_n_);
  t_bool  prepare_ (p_cstr_&, t_n_, t_n_);

  t_void display_(P_cstr_);
  t_int  compare_(P_cstr_, P_cstr_);
  t_bool match_  (P_cstr_, P_cstr_ pattern);
  t_n_   count_  (t_char,  P_cstr_);
  t_n_   length_ (P_cstr_);
  t_n_   length_ (P_cstr_, va_list);

////////////////////////////////////////////////////////////////////////////////

  template<class I>
  class t_string_impl_ {
  public:
    using t_char   = named::t_char;
    using R_block = string::R_block;

    //inline
    //t_string_impl_() : len_{0} {
    //}

    inline
    t_string_impl_(t_n_ len) : len_{len} {
    }

    inline
    t_string_impl_(p_cstr_ str) : len_{0} {
      str[0] = '\0';
    }

    inline
    t_string_impl_(p_cstr_ str, t_n_ max, P_cstr_ src)
      : len_{copy_(str, max, src, I())} {
    }

    inline
    t_string_impl_(p_cstr_ str, t_n_ max, R_block block)
      : len_{fill_(str, max, block, I())} {
    }

    inline
    t_void assign(p_cstr_ str, t_n_ max, P_cstr_ src) {
      len_ = copy_(str, max, src, I());
    }

    inline
    t_void assign(p_cstr_ str, t_n_ max, R_block block) {
      len_ = fill_(str, max, block, I());
    }

    inline
    t_void append(p_cstr_ str, t_n_ max, P_cstr_ src) {
      len_ += copy_(str + len_, max - len_, src, I());
    }

    t_void append(p_cstr_ str, t_n_ max, R_block block) {
      len_ += fill_(str + len_, max - len_, block, I());
    }

    // XXX move to cpp file - begin

    inline
    t_n_ va_assign(p_cstr_ str, t_n_ max, P_cstr_ fmt, va_list vars) {
      t_n_ len = build_(str, max, fmt, vars, I());
      if (len <= max)
        len_ = reset(len);
      return len;
    }

    inline
    t_n_ va_append(p_cstr_ str, t_n_ max, P_cstr_ fmt, va_list vars) {
      t_n_ left = max - len_;
      t_n_ len = build_(str + len_, left, fmt, vars, I());
      if (len <= left) {
        len_ += len;
        return 0;
      }
      return len + len_;
    }

    inline
    t_n_ va_assign_(p_cstr_ str, t_n_ max, P_cstr_ fmt, va_list vars) {
      va_list args;
      va_copy(args, vars);
      t_n_ len = build_(str, max, fmt, args);
      va_end(args);
      if (len <= max)
        len_ = reset(len);
      return len;
    }

    inline
    t_n_ va_append_(p_cstr_ str, t_n_ max, P_cstr_ fmt, va_list vars) {
      va_list args;
      va_copy(args, vars);
      t_n_ left = max - len_;
      t_n_ len = build_(str + len_, left, fmt, args);
      va_end(args);
      if (len <= left) {
        len_ += len;
        return 0;
      }
      return len + len_;
    }

    // XXX move to cpp file - end

    inline
    t_void display(P_cstr_ str) const {
      if (len_)
        display_(str);
    }

    inline
    t_void display_then_clear(p_cstr_ str) {
      if (len_) {
        display_(str);
        str[0] = '\0';
      }
    }

    inline
    t_bool match(P_cstr_ str, P_cstr_ pattern) const {
      return match_(str, pattern);
    }

    inline
    t_void clear(p_cstr_ str) {
      str[0] = '\0';
    }

    inline
    P_cstr_ c_str(P_cstr_ str) const {
      return str;
    }

    inline
    t_n_ get_length() const {
      return len_;
    }

    inline
    t_n_ count(P_cstr_ str, t_char c) const {
      return count_(c, str);
    }

    inline
    t_char front(P_cstr_ str) const {
      return len_ ? str[0] : '\0';
    }

    inline
    t_char back(P_cstr_ str) const {
      return len_ ? str[len_ - 1] : '\0';
    }

    template<class F>
    inline
    t_void each(P_cstr_ str, F f) {
      for (t_n_ n = 0; n < len_; ++n)
        f(str[n]);
    }

    template<class F>
    inline
    t_void each(P_cstr_ str, F f) const {
      for (t_n_ n = 0; n < len_; ++n)
        f(str[n]);
    }

    t_n_ len_ = 0;
  };

///////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
