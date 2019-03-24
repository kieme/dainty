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

#ifndef _DAINTY_NAMED_STRING_1_H_
#define _DAINTY_NAMED_STRING_1_H_

#include "dainty_named_string_impl.h"

namespace dainty
{
namespace named
{
namespace string
{
////////////////////////////////////////////////////////////////////////////////

  template<class TAG, t_n_ N = 0, t_overflow O = OVERFLOW_GROW>
  class t_string {
    using t_impl_ = t_string_impl_<O>;
  public:
    using t_n      = named::t_n;
    using P_cstr   = named::P_cstr;
    using R_crange = t_prefix<t_crange>::R_;
    using r_string = typename t_prefix<t_string>::r_;
    using R_string = typename t_prefix<t_string>::R_;
    using t_char   = typename t_impl_::t_char;
    using R_block  = typename t_impl_::R_block;

    t_string()                    noexcept;
    t_string(P_cstr)              noexcept;
    t_string(R_block)             noexcept;
    t_string(R_crange)            noexcept;
    t_string(R_string)            noexcept;
    t_string(t_fmt, P_cstr_, ...) noexcept
      __attribute__((format(printf, 3, 4)));

    template<t_n_ N1>
    t_string(const t_char (&)[N1])         noexcept;
    template<t_n_ N1, t_overflow O1>
    t_string(const t_string<TAG, N1, O1>&) noexcept;

    r_string operator=(P_cstr)   noexcept;
    r_string operator=(R_block)  noexcept;
    r_string operator=(R_crange) noexcept;
    r_string operator=(R_string) noexcept;

    template<t_n_ N1>
    r_string operator=(const t_char (&)[N1])         noexcept;
    template<t_n_ N1, t_overflow O1>
    r_string operator=(const t_string<TAG, N1, O1>&) noexcept;

    template<class TAG1, t_n_ N1, t_overflow O1>
    r_string assign(const t_string<TAG1, N1, O1>&) noexcept;
    r_string assign(t_fmt, P_cstr_, ...)           noexcept
      __attribute__((format(printf, 3, 4)));

    r_string append(P_cstr)              noexcept;
    r_string append(R_block)             noexcept;
    r_string append(R_crange)            noexcept;
    r_string append(t_fmt, P_cstr_, ...) noexcept
      __attribute__((format(printf, 3, 4)));

    template<t_n_ N1>
    r_string append(const t_char (&)[N1])          noexcept;
    template<class TAG1, t_n_ N1, t_overflow O1>
    r_string append(const t_string<TAG1, N1, O1>&) noexcept;

    r_string va_assign(P_cstr_ fmt, va_list vars) noexcept;
    r_string va_append(P_cstr_ fmt, va_list vars) noexcept;

    t_void clear() noexcept;

    t_void display           () const noexcept;
    t_void display_then_clear()       noexcept;

    t_bool is_match(P_cstr pattern)                        const noexcept;
    template<t_n_ N1>
    t_bool is_match(const t_char (&pattern)[N1])           const noexcept;
    template<class TAG1, t_n_ N1, t_overflow O1>
    t_bool is_match(const t_string<TAG1, N1, O1>& pattern) const noexcept;

    constexpr static
    t_n    get_capacity()             noexcept;
    t_n    get_count   (t_char) const noexcept;
    P_cstr get_cstr    ()       const noexcept;
    t_n    get_length  ()       const noexcept;
    t_bool is_empty    ()       const noexcept;
    t_char get_front   ()       const noexcept;
    t_char get_back    ()       const noexcept;

    t_crange mk_range()           const noexcept;
    t_crange mk_range(t_ix)       const noexcept;
    t_crange mk_range(t_ix, t_ix) const noexcept;

    template<class F> void  each(F)       noexcept;
    template<class F> void  each(F) const noexcept;
    template<class F> void ceach(F) const noexcept;

    t_void mod_(t_ix pos, t_char) noexcept;

  private:
    template<class, t_n_, t_overflow> friend class t_string;
    t_char  store_[N+1];
    t_impl_ impl_;
  };

////////////////////////////////////////////////////////////////////////////////

  template<class TAG, t_n_ N, t_overflow O>
  inline
  t_string<TAG, N, O>::t_string() noexcept : impl_{store_} {
  }

  template<class TAG, t_n_ N, t_overflow O>
  inline
  t_string<TAG, N, O>::t_string(P_cstr str) noexcept
    : impl_{store_, N+1, get(str)} {
  }

  template<class TAG, t_n_ N, t_overflow O>
  inline
  t_string<TAG, N, O>::t_string(R_block block) noexcept
    : impl_{store_, N+1, block} {
  }

  template<class TAG, t_n_ N, t_overflow O>
  inline
  t_string<TAG, N, O>::t_string(R_crange range) noexcept
    : impl_{store_, N+1, begin(range), get(range.n)} {
  }

  template<class TAG, t_n_ N, t_overflow O>
  inline
  t_string<TAG, N, O>::t_string(t_fmt, P_cstr_ fmt, ...) noexcept {
    va_list vars;
    va_start(vars, fmt);
    impl_.va_assign(store_, N+1, fmt, vars);
    va_end(vars);
  }

  template<class TAG, t_n_ N, t_overflow O>
  inline
  t_string<TAG, N, O>::t_string(R_string str) noexcept
    : impl_{store_, N+1, get(str.get_cstr())} {
  }

  template<class TAG, t_n_ N, t_overflow O>
  template<t_n_ N1>
  inline
  t_string<TAG, N, O>::t_string(const t_char (&str)[N1]) noexcept
    : impl_{store_, N+1, str} {
  }

  template<class TAG, t_n_ N, t_overflow O>
  template<t_n_ N1, t_overflow O1>
  inline
  t_string<TAG, N, O>::t_string(const t_string<TAG, N1, O1>& str) noexcept
    : impl_{store_, N+1, get(str.get_cstr())} {
  }

  template<class TAG, t_n_ N, t_overflow O>
  inline
  typename t_string<TAG, N, O>::r_string t_string<TAG, N, O>
      ::operator=(P_cstr str) noexcept {
    impl_.assign(store_, N+1, get(str));
    return *this;
  }

  template<class TAG, t_n_ N, t_overflow O>
  inline
  typename t_string<TAG, N, O>::r_string t_string<TAG, N, O>
      ::operator=(R_block block) noexcept {
    impl_.assign(store_, N+1, block);
    return *this;
  }

  template<class TAG, t_n_ N, t_overflow O>
  inline
  typename t_string<TAG, N, O>::r_string t_string<TAG, N, O>
      ::operator=(R_crange range) noexcept {
    impl_.assign(store_, N+1, begin(range), get(range.n));
    return *this;
  }

  template<class TAG, t_n_ N, t_overflow O>
  inline
  typename t_string<TAG, N, O>::r_string t_string<TAG, N, O>
      ::operator=(R_string str) noexcept {
    impl_.assign(store_, N+1, get(str.get_cstr()));
    return *this;
  }

  template<class TAG, t_n_ N, t_overflow O>
  template<t_n_ N1>
  inline
  typename t_string<TAG, N, O>::r_string t_string<TAG, N, O>
      ::operator=(const t_char (&str)[N1]) noexcept {
    impl_.assign(store_, N+1, str);
    return *this;
  }

  template<class TAG, t_n_ N, t_overflow O>
  template<t_n_ N1, t_overflow O1>
  inline
  typename t_string<TAG, N, O>::r_string t_string<TAG, N, O>
      ::operator=(const t_string<TAG, N1, O1>& str) noexcept {
    impl_.assign(store_, N+1, get(str.get_cstr()));
    return *this;
  }

  template<class TAG, t_n_ N, t_overflow O>
  inline
  typename t_string<TAG, N, O>::r_string t_string<TAG, N, O>
      ::assign(t_fmt, P_cstr_ fmt, ...) noexcept {
    va_list vars;
    va_start(vars, fmt);
    impl_.va_assign(store_, N+1, fmt, vars);
    va_end(vars);
    return *this;
  }

  template<class TAG, t_n_ N, t_overflow O>
  template<class TAG1, t_n_ N1, t_overflow O1>
  inline
  typename t_string<TAG, N, O>::r_string t_string<TAG, N, O>
      ::assign(const t_string<TAG1, N1, O1>& str) noexcept {
    impl_.assign(store_, N1, get(str.get_cstr()));
    return *this;
  }

  template<class TAG, t_n_ N, t_overflow O>
  inline
  typename t_string<TAG, N, O>::r_string t_string<TAG, N, O>
      ::append(P_cstr str) noexcept {
    impl_.append(store_, N+1, get(str));
    return *this;
  }

  template<class TAG, t_n_ N, t_overflow O>
  inline
  typename t_string<TAG, N, O>::r_string t_string<TAG, N, O>
      ::append(R_block block) noexcept {
    impl_.append(store_, N+1, block);
    return *this;
  }

  template<class TAG, t_n_ N, t_overflow O>
  inline
  typename t_string<TAG, N, O>::r_string t_string<TAG, N, O>
      ::append(R_crange range) noexcept {
    impl_.append(store_, N+1, begin(range), get(range.n));
    return *this;
  }

  template<class TAG, t_n_ N, t_overflow O>
  inline
  typename t_string<TAG, N, O>::r_string t_string<TAG, N, O>
      ::append(t_fmt, P_cstr_ fmt, ...) noexcept {
    va_list vars;
    va_start(vars, fmt);
    impl_.va_append(store_, N+1, fmt, vars);
    va_end(vars);
    return *this;
  }

  template<class TAG, t_n_ N, t_overflow O>
  template<t_n_ N1>
  inline
  typename t_string<TAG, N, O>::r_string t_string<TAG, N, O>
      ::append(const t_char (&str)[N1]) noexcept {
    impl_.append(store_, N+1, str);
    return *this;
  }

  template<class TAG, t_n_ N, t_overflow O>
  template<class TAG1, t_n_ N1, t_overflow O1>
  inline
  typename t_string<TAG, N, O>::r_string t_string<TAG, N, O>
      ::append(const t_string<TAG1, N1, O1>& str) noexcept {
    impl_.append(store_, N+1, get(str.get_cstr()));
    return *this;
  }

  template<class TAG, t_n_ N, t_overflow O>
  inline
  typename t_string<TAG, N, O>::r_string t_string<TAG, N, O>
      ::va_assign(P_cstr_ fmt, va_list vars) noexcept {
    impl_.va_assign(store_, N+1, fmt, vars);
    return *this;
  }

  template<class TAG, t_n_ N, t_overflow O>
  inline
  typename t_string<TAG, N, O>::r_string t_string<TAG, N, O>
      ::va_append(P_cstr_ fmt, va_list vars) noexcept {
    impl_.va_append(store_, N+1, fmt, vars);
    return *this;
  }

  template<class TAG, t_n_ N, t_overflow O>
  inline
  t_void t_string<TAG, N, O>::display() const noexcept {
    impl_.display(store_);
  }

  template<class TAG, t_n_ N, t_overflow O>
  inline
  t_void t_string<TAG, N, O>::display_then_clear() noexcept {
    impl_.display_then_clear(store_);
  }

  template<class TAG, t_n_ N, t_overflow O>
  inline
  t_bool t_string<TAG, N, O>::is_match(P_cstr pattern) const noexcept {
    return impl_.is_match(store_, get(pattern));
  }

  template<class TAG, t_n_ N, t_overflow O>
  template<t_n_ N1>
  inline
  t_bool t_string<TAG, N, O>
      ::is_match(const t_char (&pattern)[N1]) const noexcept {
    return impl_.is_match(store_, pattern);
  }

  template<class TAG, t_n_ N, t_overflow O>
  template<class TAG1, t_n_ N1, t_overflow O1>
  inline
  t_bool t_string<TAG, N, O>
      ::is_match(const t_string<TAG1, N1, O1>& pattern) const noexcept {
    return impl_.is_match(store_, get(pattern.get_cstr()));
  }

  template<class TAG, t_n_ N, t_overflow O>
  inline
  t_void t_string<TAG, N, O>::clear() noexcept {
    return impl_.clear(store_);
  }

  template<class TAG, t_n_ N, t_overflow O>
  inline
  P_cstr t_string<TAG, N, O>::get_cstr() const noexcept {
    return P_cstr{impl_.get_cstr(store_)};
  }

  template<class TAG, t_n_ N, t_overflow O>
  inline
  t_n t_string<TAG, N, O>::get_length() const noexcept {
    return t_n{impl_.get_length()};
  }

  template<class TAG, t_n_ N, t_overflow O>
  constexpr
  t_n t_string<TAG, N, O>::get_capacity() noexcept {
    return t_n{N};
  }

  template<class TAG, t_n_ N, t_overflow O>
  inline
  t_bool t_string<TAG, N, O>::is_empty() const noexcept {
    return impl_.is_empty();
  }

  template<class TAG, t_n_ N, t_overflow O>
  inline
  t_n t_string<TAG, N, O>::get_count(t_char c) const noexcept {
    return t_n{impl_.get_count(store_, c)};
  }

  template<class TAG, t_n_ N, t_overflow O>
  inline
  typename t_string<TAG, N, O>::t_char t_string<TAG, N, O>
      ::get_front() const noexcept {
    return impl_.get_front(store_);
  }

  template<class TAG, t_n_ N, t_overflow O>
  inline
  typename t_string<TAG, N, O>::t_char t_string<TAG, N, O>
      ::get_back() const noexcept {
    return impl_.get_back(store_);
  }

  template<class TAG, t_n_ N, t_overflow O>
  inline
  t_crange t_string<TAG, N, O>::mk_range() const noexcept {
    return impl_.mk_range(store_);
  }

  template<class TAG, t_n_ N, t_overflow O>
  inline
  t_crange t_string<TAG, N, O>::mk_range(t_ix begin) const noexcept {
    return impl_.mk_range(store_, begin);
  }

  template<class TAG, t_n_ N, t_overflow O>
  inline
  t_crange t_string<TAG, N, O>::mk_range(t_ix begin, t_ix end) const noexcept {
    return impl_.mk_range(store_, begin, end);
  }

  template<class TAG, t_n_ N, t_overflow O>
  template<class F>
  inline
  t_void t_string<TAG, N, O>::each(F f) noexcept {
    impl_.each(store_, f);
  }

  template<class TAG, t_n_ N, t_overflow O>
  template<class F>
  inline
  t_void t_string<TAG, N, O>::each(F f) const noexcept {
    impl_.each(store_, f);
  }

  template<class TAG, t_n_ N, t_overflow O>
  template<class F>
  inline
  t_void t_string<TAG, N, O>::ceach(F f) const noexcept {
    impl_.each(store_, f);
  }

  template<class TAG, t_n_ N, t_overflow O>
  inline
  t_void t_string<TAG, N, O>::mod_(t_ix pos, t_char ch) noexcept {
    impl_.mod_(store_, get(pos), ch);
  }

///////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
