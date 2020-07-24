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
  using impl_::t_char;
  using impl_::t_void;
  using impl_::t_n;
  using impl_::t_n_;
  using impl_::t_ix;
  using impl_::t_begin_ix;
  using impl_::t_end_ix;
  using impl_::t_cstr_cptr_; // XXX
  using impl_::t_cstr_cptr;  // XXX
  using impl_::t_crange;
  using impl_::t_block;
  using impl_::t_char;
  using impl_::t_validity;
  using impl_::VALID;
  using impl_::INVALID;
  using impl_::t_range;
  using impl_::t_crange;
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
    using t_n      = string::t_n;
    using t_crange = string::t_crange;
    using t_char   = string::t_char;
    using t_block  = string::t_block;
    using r_string = typename t_prefix<t_string>::r_;
    using R_string = typename t_prefix<t_string>::R_;

    t_string()                  noexcept;
    t_string(t_block)           noexcept;
    t_string(t_crange)          noexcept;
    t_string(R_string)          noexcept;
    t_string(t_cstr_cptr_, ...) noexcept
      __attribute__((format(printf, 2, 3)));
    template<t_n_ N1, typename O1>
    t_string(const t_string<TAG, N1, O1>&) noexcept;

    r_string operator=(t_block)  noexcept;
    r_string operator=(t_crange) noexcept;
    r_string operator=(R_string) noexcept;

    template<t_n_ N1, typename O1>
    r_string operator=(const t_string<TAG, N1, O1>&) noexcept;

    template<class TAG1, t_n_ N1, typename O1>
    r_string assign(const t_string<TAG1, N1, O1>&) noexcept;
    r_string assign(t_block)                       noexcept;
    r_string assign(t_crange)                      noexcept;
    r_string assign(t_crange fmt, va_list vars)    noexcept;
    r_string assign(t_cstr_cptr_, ...)             noexcept
      __attribute__((format(printf, 2, 3)));

    template<class TAG1, t_n_ N1, typename O1>
    r_string append(const t_string<TAG1, N1, O1>&) noexcept;
    r_string append(t_block)                       noexcept;
    r_string append(t_crange)                      noexcept;
    r_string append(t_crange fmt, va_list vars)    noexcept;
    r_string append(t_cstr_cptr_, ...)             noexcept
      __attribute__((format(printf, 2, 3)));

    t_bool remove  (t_begin_ix, t_end_ix) noexcept;
    t_void clear   ()    noexcept;
    t_void truncate(t_n) noexcept;
    t_void reverse ()    noexcept;

    t_void display           (t_crange prefix  = NO_RANGE,
                              t_crange postfix = NO_RANGE) const noexcept;
    t_void display_then_clear(t_crange prefix  = NO_RANGE,
                              t_crange postfix = NO_RANGE)       noexcept;

    t_n scan(t_n, t_cstr_cptr_, ...)          noexcept
      __attribute__((format(scanf, 3, 4)));
    t_n scan(t_n, t_crange fmt, va_list vars) noexcept;

    t_bool is_match(t_crange pattern) const noexcept;

    constexpr static
    t_n         get_capacity()             noexcept;
    t_n         get_count   (t_char) const noexcept;
    t_cstr_cptr get_cstr    ()       const noexcept;
    t_n         get_length  ()       const noexcept;
    t_bool      is_empty    ()       const noexcept;
    t_char      get_front   ()       const noexcept;
    t_char      get_back    ()       const noexcept;

    t_crange mk_range()                     const noexcept;
    t_crange mk_range(t_begin_ix)           const noexcept;
    t_crange mk_range(t_begin_ix, t_end_ix) const noexcept;
    t_crange mk_range(t_n)                  const noexcept;
    t_crange mk_range(t_begin_ix, t_n)      const noexcept;

    template<class F> void  each(F&&)       noexcept;
    template<class F> void  each(F&&) const noexcept;
    template<class F> void ceach(F&&) const noexcept;

    t_void mod_(t_ix pos, t_char) noexcept;

  public:
    template<typename F>
    inline
    r_string custom_assign_(F&& func) noexcept {
      impl_.custom_assign(store_, base::preserve<F>(func));
      return *this;
    }

    template<typename F>
    inline
    r_string custom_append_(F&& func) noexcept {
      impl_.custom_append(store_, base::preserve<F>(func));
      return *this;
    }

  private:
    template<typename, t_n_, typename> friend class t_string;

    using t_store_ = buf::t_buf<t_char, N + 1, buf::t_size_static>;

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
  t_string<TAG, N, O>::t_string(t_block block) noexcept
    : impl_{store_, block} {
  }

  template<class TAG, t_n_ N, typename O>
  inline
  t_string<TAG, N, O>::t_string(t_crange range) noexcept
    : impl_{store_, range} {
  }

  template<class TAG, t_n_ N, typename O>
  inline
  t_string<TAG, N, O>::t_string(t_cstr_cptr_ fmt, ...) noexcept {
    va_list vars;
    va_start(vars, fmt);
    impl_.assign(store_, string::mk_range(fmt), vars);
    va_end(vars);
  }

  template<class TAG, t_n_ N, typename O>
  inline
  t_string<TAG, N, O>::t_string(R_string str) noexcept
    : impl_{store_, str.mk_range()} {
  }

  template<class TAG, t_n_ N, typename O>
  template<t_n_ N1, typename O1>
  inline
  t_string<TAG, N, O>::t_string(const t_string<TAG, N1, O1>& str) noexcept
    : impl_{store_, str.mk_range()} {
  }

  template<class TAG, t_n_ N, typename O>
  inline
  typename t_string<TAG, N, O>::r_string t_string<TAG, N, O>
      ::operator=(t_block block) noexcept {
    impl_.assign(store_, block);
    return *this;
  }

  template<class TAG, t_n_ N, typename O>
  inline
  typename t_string<TAG, N, O>::r_string t_string<TAG, N, O>
      ::operator=(t_crange range) noexcept {
    impl_.assign(store_, range);
    return *this;
  }

  template<class TAG, t_n_ N, typename O>
  inline
  typename t_string<TAG, N, O>::r_string t_string<TAG, N, O>
      ::operator=(R_string str) noexcept {
    impl_.assign(store_, str.mk_range());
    return *this;
  }

  template<class TAG, t_n_ N, typename O>
  template<t_n_ N1, typename O1>
  inline
  typename t_string<TAG, N, O>::r_string t_string<TAG, N, O>
      ::operator=(const t_string<TAG, N1, O1>& str) noexcept {
    impl_.assign(store_, str.mk_range());
    return *this;
  }

  template<class TAG, t_n_ N, typename O>
  inline
  typename t_string<TAG, N, O>::r_string t_string<TAG, N, O>
      ::assign(t_cstr_cptr_ fmt, ...) noexcept {
    va_list vars;
    va_start(vars, fmt);
    impl_.assign(store_, string::mk_range(fmt), vars);
    va_end(vars);
    return *this;
  }

  template<class TAG, t_n_ N, typename O>
  template<class TAG1, t_n_ N1, typename O1>
  inline
  typename t_string<TAG, N, O>::r_string t_string<TAG, N, O>
      ::assign(const t_string<TAG1, N1, O1>& str) noexcept {
    impl_.assign(store_, str.mk_range());
    return *this;
  }

  template<class TAG, t_n_ N, typename O>
  inline
  typename t_string<TAG, N, O>::r_string t_string<TAG, N, O>
      ::assign(t_block block) noexcept {
    impl_.assign(store_, block);
    return *this;
  }

  template<class TAG, t_n_ N, typename O>
  inline
  typename t_string<TAG, N, O>::r_string t_string<TAG, N, O>
      ::append(t_block block) noexcept {
    impl_.append(store_, block);
    return *this;
  }

  template<class TAG, t_n_ N, typename O>
  inline
  typename t_string<TAG, N, O>::r_string t_string<TAG, N, O>
      ::append(t_crange range) noexcept {
    impl_.append(store_, range);
    return *this;
  }

  template<class TAG, t_n_ N, typename O>
  inline
  typename t_string<TAG, N, O>::r_string t_string<TAG, N, O>
      ::append(t_cstr_cptr_ fmt, ...) noexcept {
    va_list vars;
    va_start(vars, fmt);
    impl_.append(store_, string::mk_range(fmt), vars);
    va_end(vars);
    return *this;
  }

  template<class TAG, t_n_ N, typename O>
  template<class TAG1, t_n_ N1, typename O1>
  inline
  typename t_string<TAG, N, O>::r_string t_string<TAG, N, O>
      ::append(const t_string<TAG1, N1, O1>& str) noexcept {
    impl_.append(store_, str.mk_range());
    return *this;
  }

  template<class TAG, t_n_ N, typename O>
  inline
  typename t_string<TAG, N, O>::r_string t_string<TAG, N, O>
      ::assign(t_crange range) noexcept {
    impl_.assign(store_, range);
    return *this;
  }

  template<class TAG, t_n_ N, typename O>
  inline
  typename t_string<TAG, N, O>::r_string t_string<TAG, N, O>
      ::assign(t_crange fmt, va_list vars) noexcept {
    impl_.assign(store_, fmt, vars);
    return *this;
  }

  template<class TAG, t_n_ N, typename O>
  inline
  typename t_string<TAG, N, O>::r_string t_string<TAG, N, O>
      ::append(t_crange fmt, va_list vars) noexcept {
    impl_.append(store_, fmt, vars);
    return *this;
  }

  template<class TAG, t_n_ N, typename O>
  inline
  t_void t_string<TAG, N, O>::display(t_crange prefix,
                                      t_crange postfix) const noexcept {
    impl_.display(store_, prefix, postfix);
  }

  template<class TAG, t_n_ N, typename O>
  inline
  t_void t_string<TAG, N, O>::display_then_clear(t_crange prefix,
                                                 t_crange postfix) noexcept {
    impl_.display_then_clear(store_, prefix, postfix);
  }

  template<class TAG, t_n_ N, typename O>
  inline
  t_bool t_string<TAG, N, O>::is_match(t_crange pattern) const noexcept {
    return impl_.is_match(store_, pattern);
  }

  template<class TAG, t_n_ N, typename O>
  inline
  t_bool t_string<TAG, N, O>::remove(t_begin_ix begin,
                                     t_end_ix   end) noexcept {
    return impl_.remove(store_, begin, end);
  }

  template<class TAG, t_n_ N, typename O>
  inline
  t_void t_string<TAG, N, O>::clear() noexcept {
    impl_.clear(store_);
  }

  template<class TAG, t_n_ N, typename O>
  inline
  t_bool t_string<TAG, N, O>::truncate(t_n n) noexcept {
    return impl_.truncate(store_, n);
  }

  template<class TAG, t_n_ N, typename O>
  inline
  t_void t_string<TAG, N, O>::reverse() noexcept {
    impl_.reverse(store_);
  }

  template<class TAG, t_n_ N, typename O>
  inline
  t_cstr_cptr t_string<TAG, N, O>::get_cstr() const noexcept {
    return t_cstr_cptr{impl_.get_cstr(store_)}; // XXX
  }

  template<class TAG, t_n_ N, typename O>
  inline
  t_n t_string<TAG, N, O>::get_length() const noexcept {
    return impl_.get_length();
  }

  template<class TAG, t_n_ N, typename O>
  constexpr
  t_n t_string<TAG, N, O>::get_capacity() noexcept {
    return t_store_::get_capacity() - 1;
  }

  template<class TAG, t_n_ N, typename O>
  inline
  t_bool t_string<TAG, N, O>::is_empty() const noexcept {
    return impl_.is_empty();
  }

  template<class TAG, t_n_ N, typename O>
  inline
  t_n t_string<TAG, N, O>::get_count(t_char ch) const noexcept {
    return impl_.get_count(store_, ch);
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
  t_crange t_string<TAG, N, O>::mk_range(t_begin_ix begin) const noexcept {
    return impl_.mk_range(store_, begin);
  }

  template<class TAG, t_n_ N, typename O>
  inline
  t_crange t_string<TAG, N, O>::mk_range(t_begin_ix begin,
                                         t_end_ix   end) const noexcept {
    return impl_.mk_range(store_, begin, end);
  }

  template<class TAG, t_n_ N, typename O>
  inline
  t_crange t_string<TAG, N, O>::mk_range(t_n n) const noexcept {
    return impl_.mk_range(store_, n);
  }

  template<class TAG, t_n_ N, typename O>
  inline
  t_crange t_string<TAG, N, O>::mk_range(t_begin_ix begin,
                                         t_n n) const noexcept {
    return impl_.mk_range(store_, begin, n);
  }

  template<class TAG, t_n_ N, typename O>
  template<class F>
  inline
  t_void t_string<TAG, N, O>::each(F&& func) noexcept {
    impl_.each(store_, base::preserve<F>(func));
  }

  template<class TAG, t_n_ N, typename O>
  template<class F>
  inline
  t_void t_string<TAG, N, O>::each(F&& func) const noexcept {
    impl_.each(store_, base::preserve<F>(func));
  }

  template<class TAG, t_n_ N, typename O>
  template<class F>
  inline
  t_void t_string<TAG, N, O>::ceach(F&& func) const noexcept {
    impl_.each(store_, base::preserve<F>(func));
  }

  template<class TAG, t_n_ N, typename O>
  inline
  t_void t_string<TAG, N, O>::mod_(t_ix pos, t_char ch) noexcept {
    impl_.mod_(store_, pos, ch);
  }

  template<class TAG, t_n_ N, typename O>
  inline
  t_n t_string<TAG, N, O>::scan(t_n n, t_cstr_cptr_ fmt, ...) noexcept {
    va_list vars;
    va_start(vars, fmt);
    t_n found = impl_.scan(store_, n, fmt, vars);
    va_end(vars);
    return found;
  }

  template<class TAG, t_n_ N, typename O>
  inline
  t_n t_string<TAG, N, O>::scan(t_n n, t_crange fmt, va_list vars) noexcept {
    return impl_.scan(store_, n, fmt, vars);
  }

///////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
