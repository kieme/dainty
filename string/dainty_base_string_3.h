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

#ifndef _DAINTY_BASE_STRING_3_H_
#define _DAINTY_BASE_STRING_3_H_

#include "dainty_base_string_1.h"

namespace dainty
{
namespace base
{
namespace string
{
////////////////////////////////////////////////////////////////////////////////

  template<class TAG, typename O>
  class t_string<TAG, 0, O> {
    using t_impl_ = impl_::t_impl_<O>;
  public:
    using t_n      = string::t_n;
    using t_crange = string::t_crange;
    using t_char   = string::t_char;
    using t_block  = string::t_block;
    using r_string = typename t_prefix<t_string>::r_;
    using R_string = typename t_prefix<t_string>::R_;

    t_string(t_n max)                    noexcept;
    t_string(t_n max, t_block)           noexcept;
    t_string(t_n max, t_crange)          noexcept;
    t_string(t_n max, R_string)          noexcept;
    t_string(t_n max, t_cstr_cptr_, ...) noexcept
      __attribute__((format(printf, 3, 4)));

    template<t_n_ N1, typename O1>
    t_string(t_n max, const t_string<TAG, N1, O1>&) noexcept;

    template<typename O1>
    t_string(t_n max, t_string<TAG, 0,  O1>&&) noexcept; //XXX
    template<t_n_ N1>
    t_string(t_n max, t_string<TAG, N1, t_overflow_grow>&&) noexcept; //XXX

    r_string operator=(t_block)  noexcept;
    r_string operator=(t_crange) noexcept;
    r_string operator=(R_string) noexcept;

    template<t_n_ N1, typename O1>
    r_string operator=(const t_string<TAG, N1, O1>&) noexcept;
    template<typename O1>
    r_string operator=(t_string<TAG, 0, O1>&&)       noexcept; // XXX
    template<t_n_ N1>
    r_string operator=(t_string<TAG, N1, t_overflow_grow>&&) noexcept; // XXX

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
    t_bool truncate(t_n) noexcept;
    t_void reverse ()    noexcept;

    t_void display           (t_crange prefix  = NO_RANGE,
                              t_crange postfix = NO_RANGE) const noexcept;
    t_void display_then_clear(t_crange prefix  = NO_RANGE,
                              t_crange postfix = NO_RANGE)       noexcept;

    t_n scan(t_n, t_cstr_cptr_, ...)     noexcept
      __attribute__((format(scanf, 3, 4)));
    t_n scan(t_n, t_crange fmt, va_list) noexcept;

    t_bool is_match(t_crange pattern) const noexcept;

    t_n         get_capacity()       const noexcept;
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
      impl_.custom_assign(store_, util::f_cast<F>(func));
      return *this;
    }

    template<typename F>
    inline
    r_string custom_append_(F&& func) noexcept {
      impl_.custom_append(store_, util::f_cast<F>(func));
      return *this;
    }

  private:
    template<typename, t_n_, typename> friend class t_string;

    using t_store_ = buf::t_buf<t_char, 0, buf::t_size_dynamic>;

    t_store_ store_;
    t_impl_  impl_;
  };

///////////////////////////////////////////////////////////////////////////////

  template<class TAG, typename O>
  inline
  t_string<TAG, 0, O>::t_string(t_n max) noexcept : store_{max}, impl_{store_} {
  }

  template<class TAG, typename O>
  inline
  t_string<TAG, 0, O>::t_string(t_n max, t_block block) noexcept
    : store_{max}, impl_{store_, block} {
  }

  template<class TAG, typename O>
  inline
  t_string<TAG, 0, O>::t_string(t_n max, t_crange range) noexcept
    : store_{max}, impl_{store_, range} {
  }

  template<class TAG, typename O>
  inline
  t_string<TAG, 0, O>::t_string(t_n max, t_cstr_cptr_ fmt, ...) noexcept
      : store_{max}, impl_{store_} {
    va_list vars;
    va_start(vars, fmt);
    impl_.assign(store_, string::mk_range(fmt), vars);
    va_end(vars);
  }

  template<class TAG, typename O>
  inline
  t_string<TAG, 0, O>::t_string(t_n max, R_string str) noexcept
    : store_{max}, impl_{store_, str.mk_range()} {
  }

  template<class TAG, typename O>
  template<t_n_ N1, typename O1>
  inline
  t_string<TAG, 0, O>::
    t_string(t_n max, const t_string<TAG, N1, O1>& str) noexcept
      : store_{max}, impl_{store_, str.mk_range()} {
  }

  template<class TAG, typename O>
  inline
  typename t_string<TAG, 0, O>::r_string t_string<TAG, 0, O>
    ::operator=(t_block block) noexcept {
    impl_.assign(store_, block);
    return *this;
  }

  template<class TAG, typename O>
  inline
  typename t_string<TAG, 0, O>::r_string t_string<TAG, 0, O>
      ::operator=(t_crange range) noexcept {
    impl_.assign(store_, range);
    return *this;
  }

  template<class TAG, typename O>
  inline
  typename t_string<TAG, 0, O>::r_string t_string<TAG, 0, O>
      ::operator=(R_string str) noexcept {
    impl_.assign(store_, str.mk_range());
    return *this;
  }

  template<class TAG, typename O>
  template<t_n_ N1, typename O1>
  inline
  typename t_string<TAG, 0, O>::r_string t_string<TAG, 0, O>
      ::operator=(const t_string<TAG, N1, O1>& str) noexcept {
    impl_.assign(store_, str.mk_range());
    return *this;
  }

  template<class TAG, typename O>
  inline
  typename t_string<TAG, 0, O>::r_string t_string<TAG, 0, O>
      ::assign(t_cstr_cptr_ fmt, ...) noexcept {
    va_list vars;
    va_start(vars, fmt);
    impl_.assign(store_, string::mk_range(fmt), vars);
    va_end(vars);
    return *this;
  }

  template<class TAG, typename O>
  template<class TAG1, t_n_ N1, typename O1>
  inline
  typename t_string<TAG, 0, O>::r_string t_string<TAG, 0, O>
      ::assign(const t_string<TAG1, N1, O1>& str) noexcept {
    impl_.assign(store_, str.mk_range());
    return *this;
  }

  template<class TAG, typename O>
  inline
  typename t_string<TAG, 0, O>::r_string t_string<TAG, 0, O>
      ::assign(t_block block) noexcept {
    impl_.assign(store_, block);
    return *this;
  }

  template<class TAG, typename O>
  inline
  typename t_string<TAG, 0, O>::r_string t_string<TAG, 0, O>
      ::append(t_block block) noexcept {
    impl_.append(store_, block);
    return *this;
  }

  template<class TAG, typename O>
  inline
  typename t_string<TAG, 0, O>::r_string t_string<TAG, 0, O>
      ::assign(t_crange range) noexcept {
    impl_.assign(store_, range);
    return *this;
  }

  template<class TAG, typename O>
  inline
  typename t_string<TAG, 0, O>::r_string t_string<TAG, 0, O>
      ::append(t_crange range) noexcept {
    impl_.append(store_, range);
    return *this;
  }

  template<class TAG, typename O>
  inline
  typename t_string<TAG, 0, O>::r_string t_string<TAG, 0, O>
      ::append(t_cstr_cptr_ fmt, ...) noexcept {
    va_list vars;
    va_start(vars, fmt);
    impl_.append(store_, string::mk_range(fmt), vars);
    va_end(vars);
    return *this;
  }

  template<class TAG, typename O>
  template<class TAG1, t_n_ N1, typename O1>
  inline
  typename t_string<TAG, 0, O>::r_string t_string<TAG, 0, O>
      ::append(const t_string<TAG1, N1, O1>& str) noexcept {
    impl_.append(store_, str.mk_range());
    return *this;
  }

  template<class TAG, typename O>
  inline
  typename t_string<TAG, 0, O>::r_string t_string<TAG, 0, O>
      ::assign(t_crange fmt, va_list vars) noexcept {
    impl_.assign(store_, fmt, vars);
    return *this;
  }

  template<class TAG, typename O>
  inline
  typename t_string<TAG, 0, O>::r_string t_string<TAG, 0, O>
      ::append(t_crange fmt, va_list vars) noexcept {
    impl_.append(store_, fmt, vars);
    return *this;
  }

  template<class TAG, typename O>
  inline
  t_void t_string<TAG, 0, O>::display(t_crange prefix,
                                      t_crange postfix) const noexcept {
    impl_.display(store_, prefix, postfix);
  }

  template<class TAG, typename O>
  inline
  t_void t_string<TAG, 0, O>::display_then_clear(t_crange prefix,
                                                 t_crange postfix) noexcept {
    impl_.display_then_clear(store_, prefix, postfix);
  }

  template<class TAG, typename O>
  inline
  t_bool t_string<TAG, 0, O>::is_match(t_crange pattern) const noexcept {
    return impl_.is_match(store_, pattern);
  }

  template<class TAG, typename O>
  inline
  t_bool t_string<TAG, 0, O>::remove(t_begin_ix begin, t_end_ix end) noexcept {
    return impl_.remove(store_, begin, end);
  }

  template<class TAG, typename O>
  inline
  t_void t_string<TAG, 0, O>::clear() noexcept {
    impl_.clear(store_);
  }

  template<class TAG, typename O>
  inline
  t_bool t_string<TAG, 0, O>::truncate(t_n n) noexcept {
    return impl_.truncate(store_, n);
  }

  template<class TAG, typename O>
  inline
  t_void t_string<TAG, 0, O>::reverse() noexcept {
    impl_.reverse(store_);
  }

  template<class TAG, typename O>
  inline
  t_cstr_cptr t_string<TAG, 0, O>::get_cstr() const noexcept {
    return t_cstr_cptr{impl_.get_cstr(store_)};
  }

  template<class TAG, typename O>
  inline
  t_n t_string<TAG, 0, O>::get_length() const noexcept {
    return impl_.get_length();
  }

  template<class TAG, typename O>
  inline
  t_n t_string<TAG, 0, O>::get_capacity() const noexcept {
    return t_n{get(store_.get_capacity()) - 1};
  }

  template<class TAG, typename O>
  inline
  t_bool t_string<TAG, 0, O>::is_empty() const noexcept {
    return impl_.is_empty();
  }

  template<class TAG, typename O>
  inline
  t_n t_string<TAG, 0, O>::get_count(t_char c) const noexcept {
    return impl_.get_count(store_, c);
  }

  template<class TAG, typename O>
  inline
  typename t_string<TAG, 0, O>::t_char t_string<TAG, 0, O>
      ::get_front() const noexcept {
    return impl_.get_front(store_);
  }

  template<class TAG, typename O>
  inline
  typename t_string<TAG, 0, O>::t_char t_string<TAG, 0, O>
      ::get_back() const noexcept {
    return impl_.get_back(store_);
  }

  template<class TAG, typename O>
  inline
  t_crange t_string<TAG, 0, O>::mk_range() const noexcept {
    return impl_.mk_range(store_);
  }

  template<class TAG, typename O>
  inline
  t_crange t_string<TAG, 0, O>::mk_range(t_begin_ix begin) const noexcept {
    return impl_.mk_range(store_, begin);
  }

  template<class TAG, typename O>
  inline
  t_crange t_string<TAG, 0, O>::mk_range(t_begin_ix begin,
                                         t_end_ix   end) const noexcept {
    return impl_.mk_range(store_, begin, end);
  }

  template<class TAG, typename O>
  inline
  t_crange t_string<TAG, 0, O>::mk_range(t_n n) const noexcept {
    return impl_.mk_range(store_, n);
  }

  template<class TAG, typename O>
  inline
  t_crange t_string<TAG, 0, O>::mk_range(t_begin_ix begin,
                                         t_n n) const noexcept {
    return impl_.mk_range(store_, begin, n);
  }

  template<class TAG, typename O>
  template<class F>
  inline
  t_void t_string<TAG, 0, O>::each(F&& func) noexcept {
    impl_.each(store_, util::f_cast<F>(func));
  }

  template<class TAG, typename O>
  template<class F>
  inline
  t_void t_string<TAG, 0, O>::each(F&& func) const noexcept {
    impl_.each(store_, util::f_cast<F>(func));
  }

  template<class TAG, typename O>
  template<class F>
  inline
  t_void t_string<TAG, 0, O>::ceach(F&& func) const noexcept {
    impl_.each(store_, util::f_cast<F>(func));
  }

  template<class TAG, typename O>
  inline
  t_void t_string<TAG, 0, O>::mod_(t_ix pos, t_char ch) noexcept {
    impl_.mod_(store_, pos, ch);
  }

  template<class TAG, typename O>
  inline
  t_n t_string<TAG, 0, O>::scan(t_n n, t_cstr_cptr_ fmt, ...) noexcept {
    va_list vars;
    va_start(vars, fmt);
    auto found = impl_.scan(store_, n, string::mk_range(fmt), vars);
    va_end(vars);
    return found;
  }

  template<class TAG, typename O>
  inline
  t_n t_string<TAG, 0, O>::scan(t_n n, t_crange fmt, va_list vars) noexcept {
    return impl_.scan(store_, n, fmt, vars);
  }

///////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
