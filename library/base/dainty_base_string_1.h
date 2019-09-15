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

#ifndef _DAINTY_BASE_STRING_1_H_
#define _DAINTY_BASE_STRING_1_H_

#include "dainty_base_string_impl.h"

namespace dainty
{
namespace base
{
namespace string
{
////////////////////////////////////////////////////////////////////////////////

  // to do : replace P_cstr_ with t_crange
  // to_integer and to_hexidecimal

  using impl_::t_prefix;
  using impl_::t_bool;
  using impl_::t_n;
  using impl_::t_ix;
  using impl_::t_cstr_cptr;
  using impl_::t_crange;
  using impl_::R_crange;
  using impl_::t_block;
  using impl_::R_block;
  using impl_::t_fmt;
  using impl_::FMT;
  using impl_::FMT_IT;
  using impl_::FMT_VA_IT;
  using impl_::t_char;
  using impl_::t_validity;
  using impl_::VALID;
  using impl_::INVALID;
  using impl_::t_range;
  using impl_::R_range;
  using impl_::t_crange;
  using impl_::R_crange;
  using impl_::t_char_select;
  using impl_::r_char_select; // XXX
  using impl_::p_char_select; // XXX
  using impl_::NO_RANGE;
  using impl_::t_snippet;
  using impl_::r_snippet;
  using impl_::p_snippet;
  using impl_::mk_range;

  using impl_::NOT_PLUS1;
  using impl_::PLUS1;
  using impl_::NOT_INCL_CHAR;
  using impl_::INCL_CHAR;
  using impl_::NOT_EOL_OK;
  using impl_::EOL_OK;
  using impl_::EOL;

  using impl_::t_overflow_assert;
  using impl_::t_overflow_truncate;
  using impl_::t_overflow_grow;

////////////////////////////////////////////////////////////////////////////////

  template<class TAG, t_n_ N = 0, typename O = t_overflow_grow>
  class t_string {
    using t_impl_ = impl_::t_impl_<O>;
  public:
    using t_n         = string::t_n;
    using t_cstr_cptr = string::t_cstr_cptr;
    using R_crange    = t_prefix<t_crange>::R_;
    using r_string    = typename t_prefix<t_string>::r_;
    using R_string    = typename t_prefix<t_string>::R_;
    using t_char      = typename t_impl_::t_char;
    using R_block     = typename t_impl_::R_block;

    t_string()                         noexcept;
    t_string(t_cstr_cptr)              noexcept;
    t_string(R_block)                  noexcept;
    t_string(R_crange)                 noexcept;
    t_string(R_string)                 noexcept;
    t_string(t_fmt, t_cstr_cptr_, ...) noexcept
      __attribute__((format(printf, 3, 4)));

    template<t_n_ N1>
    t_string(const t_char (&)[N1])         noexcept;
    template<t_n_ N1, typename O1>
    t_string(const t_string<TAG, N1, O1>&) noexcept;

    r_string operator=(t_cstr_cptr) noexcept;
    r_string operator=(R_block)     noexcept;
    r_string operator=(R_crange)    noexcept;
    r_string operator=(R_string)    noexcept;

    template<t_n_ N1>
    r_string operator=(const t_char (&)[N1])         noexcept;
    template<t_n_ N1, typename O1>
    r_string operator=(const t_string<TAG, N1, O1>&) noexcept;

    template<class TAG1, t_n_ N1, typename O1>
    r_string assign(const t_string<TAG1, N1, O1>&) noexcept;
    r_string assign(t_fmt, t_cstr_cptr_, ...)      noexcept
      __attribute__((format(printf, 3, 4)));

    r_string append(t_cstr_cptr)              noexcept;
    r_string append(R_block)                  noexcept;
    r_string append(R_crange)                 noexcept;
    r_string append(t_fmt, t_cstr_cptr_, ...) noexcept
      __attribute__((format(printf, 3, 4)));

    template<t_n_ N1>
    r_string append(const t_char (&)[N1])          noexcept;

    template<class TAG1, t_n_ N1, typename O1>
    r_string append(const t_string<TAG1, N1, O1>&) noexcept;

    r_string assign(t_fmt_va, t_cstr_cptr_ fmt, va_list vars) noexcept;
    r_string append(t_fmt_va, t_cstr_cptr_ fmt, va_list vars) noexcept;

    t_void clear() noexcept;
    t_bool remove(t_ix begin, t_ix end) noexcept;

    t_void display           (R_crange prefix  = NO_RANGE,
                              R_crange postfix = NO_RANGE) const noexcept;
    t_void display_then_clear(R_crange prefix  = NO_RANGE,
                              R_crange postfix = NO_RANGE)        noexcept;

    t_void    scan(t_n, t_cstr_cptr_, ...) noexcept
      __attribute__((format(scanf, 3, 4)));
    t_void va_scan(t_n, t_cstr_cptr_ fmt, va_list vars) noexcept;

    t_bool is_match(t_cstr_cptr pattern)                   const noexcept;
    template<t_n_ N1>
    t_bool is_match(const t_char (&pattern)[N1])           const noexcept;
    template<class TAG1, t_n_ N1, typename O1>
    t_bool is_match(const t_string<TAG1, N1, O1>& pattern) const noexcept;

    constexpr static
    t_n         get_capacity()             noexcept;
    t_n         get_count   (t_char) const noexcept;
    t_cstr_cptr get_cstr    ()       const noexcept;
    t_n         get_length  ()       const noexcept;
    t_bool      is_empty    ()       const noexcept;
    t_char      get_front   ()       const noexcept;
    t_char      get_back    ()       const noexcept;

    t_crange mk_range()           const noexcept;
    t_crange mk_range(t_ix)       const noexcept;
    t_crange mk_range(t_ix, t_ix) const noexcept;

    operator t_crange() const noexcept;

    template<class F> void  each(F)       noexcept;
    template<class F> void  each(F) const noexcept;
    template<class F> void ceach(F) const noexcept;

    t_void mod_(t_ix pos, t_char) noexcept;

  public: // custom interface - your responsibility
    template<typename F> r_string custom_assign_(F& func) noexcept {
      impl_.custom_assign(store_, MAX_, func);
      return *this;
    }

    template<typename F> r_string custom_append_(F& func) noexcept {
      impl_.custom_append(store_, MAX_, func);
      return *this;
    }

  private:
    template<typename, t_n_, typename> friend class t_string;

    constexpr static t_n_ MAX_ = N + 1;

    using t_store_ = buf::t_buf<t_char, MAX_, buf::t_size_static>;

    t_store_ store_;
    t_impl_  impl_;
  };

////////////////////////////////////////////////////////////////////////////////

  template<class TAG, t_n_ N, typename O>
  inline
  t_string<TAG, N, O>::t_string() noexcept : impl_{store_} {
  }

  template<class TAG, t_n_ N, typename O>
  inline
  t_string<TAG, N, O>::t_string(t_cstr_cptr str) noexcept
    : impl_{store_, MAX_, get(str)} {
  }

  template<class TAG, t_n_ N, typename O>
  inline
  t_string<TAG, N, O>::t_string(R_block block) noexcept
    : impl_{store_, MAX_, block} {
  }

  template<class TAG, t_n_ N, typename O>
  inline
  t_string<TAG, N, O>::t_string(R_crange range) noexcept
    : impl_{store_, MAX_, range} {
  }

  template<class TAG, t_n_ N, typename O>
  inline
  t_string<TAG, N, O>::t_string(t_fmt, t_cstr_cptr_ fmt, ...) noexcept {
    va_list vars;
    va_start(vars, fmt);
    impl_.assign(FMT_VA_IT, store_, MAX_, fmt, vars);
    va_end(vars);
  }

  template<class TAG, t_n_ N, typename O>
  inline
  t_string<TAG, N, O>::t_string(R_string str) noexcept
    : impl_{store_, MAX_, get(str.get_cstr())} {
  }

  template<class TAG, t_n_ N, typename O>
  template<t_n_ N1>
  inline
  t_string<TAG, N, O>::t_string(const t_char (&str)[N1]) noexcept
    : impl_{store_, MAX_, str} {
  }

  template<class TAG, t_n_ N, typename O>
  template<t_n_ N1, typename O1>
  inline
  t_string<TAG, N, O>::t_string(const t_string<TAG, N1, O1>& str) noexcept
    : impl_{store_, MAX_, get(str.get_cstr())} {
  }

  template<class TAG, t_n_ N, typename O>
  inline
  typename t_string<TAG, N, O>::r_string t_string<TAG, N, O>
      ::operator=(t_cstr_cptr str) noexcept {
    impl_.assign(store_, MAX_, get(str));
    return *this;
  }

  template<class TAG, t_n_ N, typename O>
  inline
  typename t_string<TAG, N, O>::r_string t_string<TAG, N, O>
      ::operator=(R_block block) noexcept {
    impl_.assign(store_, MAX_, block);
    return *this;
  }

  template<class TAG, t_n_ N, typename O>
  inline
  typename t_string<TAG, N, O>::r_string t_string<TAG, N, O>
      ::operator=(R_crange range) noexcept {
    impl_.assign(store_, MAX_, range);
    return *this;
  }

  template<class TAG, t_n_ N, typename O>
  inline
  typename t_string<TAG, N, O>::r_string t_string<TAG, N, O>
      ::operator=(R_string str) noexcept {
    impl_.assign(store_, MAX_, get(str.get_cstr()));
    return *this;
  }

  template<class TAG, t_n_ N, typename O>
  template<t_n_ N1>
  inline
  typename t_string<TAG, N, O>::r_string t_string<TAG, N, O>
      ::operator=(const t_char (&str)[N1]) noexcept {
    impl_.assign(store_, MAX_, str);
    return *this;
  }

  template<class TAG, t_n_ N, typename O>
  template<t_n_ N1, typename O1>
  inline
  typename t_string<TAG, N, O>::r_string t_string<TAG, N, O>
      ::operator=(const t_string<TAG, N1, O1>& str) noexcept {
    impl_.assign(store_, MAX_, get(str.get_cstr()));
    return *this;
  }

  template<class TAG, t_n_ N, typename O>
  inline
  typename t_string<TAG, N, O>::r_string t_string<TAG, N, O>
      ::assign(t_fmt, t_cstr_cptr_ fmt, ...) noexcept {
    va_list vars;
    va_start(vars, fmt);
    impl_.assign(FMT_VA_IT, store_, MAX_, fmt, vars);
    va_end(vars);
    return *this;
  }

  template<class TAG, t_n_ N, typename O>
  template<class TAG1, t_n_ N1, typename O1>
  inline
  typename t_string<TAG, N, O>::r_string t_string<TAG, N, O>
      ::assign(const t_string<TAG1, N1, O1>& str) noexcept {
    impl_.assign(store_, N1, get(str.get_cstr()));
    return *this;
  }

  template<class TAG, t_n_ N, typename O>
  inline
  typename t_string<TAG, N, O>::r_string t_string<TAG, N, O>
      ::append(t_cstr_cptr str) noexcept {
    impl_.append(store_, MAX_, get(str));
    return *this;
  }

  template<class TAG, t_n_ N, typename O>
  inline
  typename t_string<TAG, N, O>::r_string t_string<TAG, N, O>
      ::append(R_block block) noexcept {
    impl_.append(store_, MAX_, block);
    return *this;
  }

  template<class TAG, t_n_ N, typename O>
  inline
  typename t_string<TAG, N, O>::r_string t_string<TAG, N, O>
      ::append(R_crange range) noexcept {
    impl_.append(store_, MAX_, range);
    return *this;
  }

  template<class TAG, t_n_ N, typename O>
  inline
  typename t_string<TAG, N, O>::r_string t_string<TAG, N, O>
      ::append(t_fmt, t_cstr_cptr_ fmt, ...) noexcept {
    va_list vars;
    va_start(vars, fmt);
    impl_.append(FMT_VA_IT, store_, MAX_, fmt, vars);
    va_end(vars);
    return *this;
  }

  template<class TAG, t_n_ N, typename O>
  template<t_n_ N1>
  inline
  typename t_string<TAG, N, O>::r_string t_string<TAG, N, O>
      ::append(const t_char (&str)[N1]) noexcept {
    impl_.append(store_, MAX_, str);
    return *this;
  }

  template<class TAG, t_n_ N, typename O>
  template<class TAG1, t_n_ N1, typename O1>
  inline
  typename t_string<TAG, N, O>::r_string t_string<TAG, N, O>
      ::append(const t_string<TAG1, N1, O1>& str) noexcept {
    impl_.append(store_, MAX_, get(str.get_cstr()));
    return *this;
  }

  template<class TAG, t_n_ N, typename O>
  inline
  typename t_string<TAG, N, O>::r_string t_string<TAG, N, O>
      ::assign(t_fmt_va, t_cstr_cptr_ fmt, va_list vars) noexcept {
    impl_.assign(FMT_VA_IT, store_, MAX_, fmt, vars);
    return *this;
  }

  template<class TAG, t_n_ N, typename O>
  inline
  typename t_string<TAG, N, O>::r_string t_string<TAG, N, O>
      ::append(t_fmt_va, t_cstr_cptr_ fmt, va_list vars) noexcept {
    impl_.append(FMT_VA_IT, store_, MAX_, fmt, vars);
    return *this;
  }

  template<class TAG, t_n_ N, typename O>
  inline
  t_void t_string<TAG, N, O>::display(R_crange prefix,
                                      R_crange postfix) const noexcept {
    impl_.display(store_, prefix, postfix);
  }

  template<class TAG, t_n_ N, typename O>
  inline
  t_void t_string<TAG, N, O>::display_then_clear(R_crange prefix,
                                                 R_crange postfix) noexcept {
    impl_.display_then_clear(store_, prefix, postfix);
  }

  template<class TAG, t_n_ N, typename O>
  inline
  t_bool t_string<TAG, N, O>::is_match(t_cstr_cptr pattern) const noexcept {
    return impl_.is_match(store_, get(pattern));
  }

  template<class TAG, t_n_ N, typename O>
  template<t_n_ N1>
  inline
  t_bool t_string<TAG, N, O>
      ::is_match(const t_char (&pattern)[N1]) const noexcept {
    return impl_.is_match(store_, pattern);
  }

  template<class TAG, t_n_ N, typename O>
  template<class TAG1, t_n_ N1, typename O1>
  inline
  t_bool t_string<TAG, N, O>
      ::is_match(const t_string<TAG1, N1, O1>& pattern) const noexcept {
    return impl_.is_match(store_, get(pattern.get_cstr()));
  }

  template<class TAG, t_n_ N, typename O>
  inline
  t_void t_string<TAG, N, O>::clear() noexcept {
    impl_.clear(store_);
  }

  template<class TAG, t_n_ N, typename O>
  inline
  t_bool t_string<TAG, N, O>::remove(t_ix begin, t_ix end) noexcept {
    return impl_.remove(store_, get(begin), get(end));
  }

  template<class TAG, t_n_ N, typename O>
  inline
  t_cstr_cptr t_string<TAG, N, O>::get_cstr() const noexcept {
    return t_cstr_cptr{impl_.get_cstr(store_)};
  }

  template<class TAG, t_n_ N, typename O>
  inline
  t_n t_string<TAG, N, O>::get_length() const noexcept {
    return t_n{impl_.get_length()};
  }

  template<class TAG, t_n_ N, typename O>
  constexpr
  t_n t_string<TAG, N, O>::get_capacity() noexcept {
    return t_n{N}; // NOT MAX_
  }

  template<class TAG, t_n_ N, typename O>
  inline
  t_bool t_string<TAG, N, O>::is_empty() const noexcept {
    return impl_.is_empty();
  }

  template<class TAG, t_n_ N, typename O>
  inline
  t_n t_string<TAG, N, O>::get_count(t_char c) const noexcept {
    return t_n{impl_.get_count(store_, c)};
  }

  template<class TAG, t_n_ N, typename O>
  inline
  typename t_string<TAG, N, O>::t_char t_string<TAG, N, O>
      ::get_front() const noexcept {
    return impl_.get_front(store_);
  }

  template<class TAG, t_n_ N, typename O>
  inline
  typename t_string<TAG, N, O>::t_char t_string<TAG, N, O>
      ::get_back() const noexcept {
    return impl_.get_back(store_);
  }

  template<class TAG, t_n_ N, typename O>
  inline
  t_crange t_string<TAG, N, O>::mk_range() const noexcept {
    return impl_.mk_range(store_);
  }

  template<class TAG, t_n_ N, typename O>
  inline
  t_crange t_string<TAG, N, O>::mk_range(t_ix begin) const noexcept {
    return impl_.mk_range(store_, begin);
  }

  template<class TAG, t_n_ N, typename O>
  inline
  t_crange t_string<TAG, N, O>::mk_range(t_ix begin, t_ix end) const noexcept {
    return impl_.mk_range(store_, begin, end);
  }

  template<class TAG, t_n_ N, typename O>
  inline
  t_string<TAG, N, O>::operator t_crange() const noexcept {
    return mk_range();
  }

  template<class TAG, t_n_ N, typename O>
  template<class F>
  inline
  t_void t_string<TAG, N, O>::each(F f) noexcept {
    impl_.each(store_, f);
  }

  template<class TAG, t_n_ N, typename O>
  template<class F>
  inline
  t_void t_string<TAG, N, O>::each(F f) const noexcept {
    impl_.each(store_, f);
  }

  template<class TAG, t_n_ N, typename O>
  template<class F>
  inline
  t_void t_string<TAG, N, O>::ceach(F f) const noexcept {
    impl_.each(store_, f);
  }

  template<class TAG, t_n_ N, typename O>
  inline
  t_void t_string<TAG, N, O>::mod_(t_ix pos, t_char ch) noexcept {
    impl_.mod_(store_, get(pos), ch);
  }

  template<class TAG, t_n_ N, typename O>
  inline
  t_void t_string<TAG, N, O>::scan(t_n n, t_cstr_cptr_ fmt, ...) noexcept {
    va_list vars;
    va_start(vars, fmt);
    impl_.va_scan(store_, get(n), fmt, vars);
    va_end(vars);
  }

  template<class TAG, t_n_ N, typename O>
  inline
  t_void t_string<TAG, N, O>::va_scan(t_n n, t_cstr_cptr_ fmt,
                                      va_list vars) noexcept {
    impl_.va_scan(store_, get(n), fmt, vars);
  }

///////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
