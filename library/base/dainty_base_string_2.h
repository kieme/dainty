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

#ifndef _DAINTY_BASE_STRING_2_H_
#define _DAINTY_BASE_STRING_2_H_

#include "dainty_base_string_1.h"

namespace dainty
{
namespace base
{
namespace string
{
////////////////////////////////////////////////////////////////////////////////

  template<typename TAG, t_n_ N>
  class t_string<TAG, N, t_overflow_grow> {
    using t_impl_ = impl_::t_impl_<t_overflow_grow>;
  public:
    using t_n         = string::t_n;
    using t_crange    = string::t_crange;
    using t_char      = string::t_char;
    using t_block     = string::t_block;
    using r_string    = typename t_prefix<t_string>::r_;
    using R_string    = typename t_prefix<t_string>::R_;

    t_string()                            noexcept;
    t_string(t_n max)                     noexcept;
    t_string(         t_block)            noexcept;
    t_string(t_n max, t_block)            noexcept;
    t_string(         t_crange)           noexcept;
    t_string(t_n max, t_crange)           noexcept;
    t_string(         R_string)           noexcept;
    t_string(t_n max, R_string)           noexcept;
    t_string(         t_cstr_cptr_, ...)  noexcept
      __attribute__((format(printf, 2, 3)));
    t_string(t_n max, t_cstr_cptr_, ...)  noexcept
      __attribute__((format(printf, 3, 4)));

    template<t_n_ N1, typename O1>
    t_string(         const t_string<TAG, N1, O1>&) noexcept;
    template<t_n_ N1, typename O1>
    t_string(t_n max, const t_string<TAG, N1, O1>&) noexcept;

    template<typename O1>
    t_string(t_string<TAG, 0, O1>&&) noexcept;
    //template<t_n_ N1>
    //t_string(t_string<TAG, N1, t_overflow_grow>&&) noexcept; //XXX

    r_string operator=(t_block)     noexcept;
    r_string operator=(t_crange)    noexcept;
    r_string operator=(R_string)    noexcept;

    template<t_n_ N1, typename O1>
    r_string operator=(const t_string<TAG, N1, O1>&) noexcept;

    template<typename O1>
    r_string operator=(t_string<TAG, 0, O1>&&)       noexcept;
    //template<t_n_ N1>
    //r_string operator=(t_string<TAG, N1, t_overflow_grow>&&) noexcept; // XXX

    template<class TAG1, t_n_ N1, typename O1>
    r_string assign(const t_string<TAG1, N1, O1>&) noexcept;
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

    t_void clear()                      noexcept;
    t_bool remove(t_begin_ix, t_end_ix) noexcept;

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

    template<class F> void  each(F&&)       noexcept;
    template<class F> void  each(F&&) const noexcept;
    template<class F> void ceach(F&&) const noexcept;

    t_void mod_(t_ix pos, t_char) noexcept;

  public: // custom interface - your responsibility
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

    using t_store_ = buf::t_buf<t_char, N + 1, buf::t_size_dynamic>;

    t_store_ store_;
    t_impl_  impl_;
  };

///////////////////////////////////////////////////////////////////////////////

  template<class TAG, t_n_ N>
  inline
  t_string<TAG, N, t_overflow_grow>::t_string() noexcept : impl_{store_} {
  }

  template<class TAG, t_n_ N>
  inline
  t_string<TAG, N, t_overflow_grow>::t_string(t_n max) noexcept
    : store_{max}, impl_{store_} {
  }

  template<class TAG, t_n_ N>
  inline
  t_string<TAG, N, t_overflow_grow>::t_string(t_block block) noexcept
    : store_{t_n{get(block.n) + 1}}, impl_{store_, block} {
  }

  template<class TAG, t_n_ N>
  inline
  t_string<TAG, N, t_overflow_grow>::t_string(t_n max, t_block block) noexcept
      : store_{max}, impl_{store_, block} {
  }

  template<class TAG, t_n_ N>
  inline
  t_string<TAG, N, t_overflow_grow>::t_string(t_crange range) noexcept
    : store_{t_n{get(range.n) + 1}}, impl_{store_, range} {
  }

  template<class TAG, t_n_ N>
  inline
  t_string<TAG, N, t_overflow_grow>::t_string(t_n max, t_crange range) noexcept
    : store_{max}, impl_{store_, range} {
  }

  template<class TAG, t_n_ N>
  inline
  t_string<TAG, N, t_overflow_grow>::t_string(t_cstr_cptr_ fmt, ...) noexcept
      : impl_{store_} {
    va_list vars;
    va_start(vars, fmt);
    impl_.assign(store_, string::mk_range(fmt), vars);
    va_end(vars);
  }

  template<class TAG, t_n_ N>
  inline
  t_string<TAG, N, t_overflow_grow>::t_string(t_n max, t_cstr_cptr_ fmt,
                                              ...) noexcept
      : store_{max}, impl_{store_} {
    va_list vars;
    va_start(vars, fmt);
    impl_.assign(store_, string::mk_range(fmt), vars);
    va_end(vars);
  }

  template<class TAG, t_n_ N>
  inline
  t_string<TAG, N, t_overflow_grow>::t_string(R_string str) noexcept
    : store_{t_n{get(str.get_length()) + 1}}, impl_{store_, str.mk_range()} {
  }

  template<class TAG, t_n_ N>
  inline
  t_string<TAG, N, t_overflow_grow>::t_string(t_n max, R_string str) noexcept
    : store_{max}, impl_{store_, str.mk_range()} {
  }

  template<class TAG, t_n_ N>
  template<t_n_ N1, typename O1>
  inline
  t_string<TAG, N, t_overflow_grow>
    ::t_string(const t_string<TAG, N1, O1>& str) noexcept
      : store_{t_n{get(str.get_length()) + 1}}, impl_{store_, str.mk_range()} {
  }

  template<class TAG, t_n_ N>
  template<t_n_ N1, typename O1>
  inline
  t_string<TAG, N, t_overflow_grow>
    ::t_string(t_n max, const t_string<TAG, N1, O1>& str) noexcept
      : store_{max}, impl_{store_, str.mk_range()} {
  }

  template<class TAG, t_n_ N>
  template<typename O1>
  inline
  t_string<TAG, N, t_overflow_grow>
    ::t_string(t_string<TAG, 0, O1>&& str) noexcept //XXX - COPY 
      : store_{t_n{get(str.get_length()) + 1}}, impl_{store_, str.mk_range()} {
  }

  template<class TAG, t_n_ N>
  inline
  typename t_string<TAG, N, t_overflow_grow>::r_string
      t_string<TAG, N, t_overflow_grow>::operator=(t_block block) noexcept {
    impl_.assign(store_, block);
    return *this;
  }

  template<class TAG, t_n_ N>
  inline
  typename t_string<TAG, N, t_overflow_grow>::r_string
      t_string<TAG, N, t_overflow_grow>::operator=(t_crange range) noexcept {
    impl_.assign(store_, range);
    return *this;
  }

  template<class TAG, t_n_ N>
  inline
  typename t_string<TAG, N, t_overflow_grow>::r_string
      t_string<TAG, N, t_overflow_grow>::operator=(R_string str) noexcept {
    impl_.assign(store_, str.mk_range());
    return *this;
  }

  template<class TAG, t_n_ N>
  template<t_n_ N1, typename O1>
  inline
  typename t_string<TAG, N, t_overflow_grow>::r_string
    t_string<TAG, N, t_overflow_grow>
      ::operator=(const t_string<TAG, N1, O1>& str) noexcept {
    impl_.assign(store_, str.mk_range());
    return *this;
  }

  template<class TAG, t_n_ N>
  template<typename O1>
  inline
  typename t_string<TAG, N, t_overflow_grow>::r_string
    t_string<TAG, N, t_overflow_grow>
      ::operator=(t_string<TAG, 0, O1>&& str) noexcept { // XXX COPY
    impl_.assign(store_, str.mk_range());
    return *this;
  }

  template<class TAG, t_n_ N>
  inline
  typename t_string<TAG, N, t_overflow_grow>::r_string
    t_string<TAG, N, t_overflow_grow>
      ::assign(t_cstr_cptr_ fmt, ...) noexcept {
    va_list vars;
    va_start(vars, fmt);
    impl_.assign(store_, string::mk_range(fmt), vars);
    va_end(vars);
    return *this;
  }

  template<class TAG, t_n_ N>
  template<class TAG1, t_n_ N1, typename O1>
  inline
  typename t_string<TAG, N, t_overflow_grow>::r_string
    t_string<TAG, N, t_overflow_grow>
      ::assign(const t_string<TAG1, N1, O1>& str) noexcept {
    impl_.assign(store_, str.mk_range());
    return *this;
  }

  template<class TAG, t_n_ N>
  inline
  typename t_string<TAG, N, t_overflow_grow>::r_string
      t_string<TAG, N, t_overflow_grow>::append(t_block block) noexcept {
    impl_.append(store_, block);
    return *this;
  }

  template<class TAG, t_n_ N>
  inline
  typename t_string<TAG, N, t_overflow_grow>::r_string
      t_string<TAG, N, t_overflow_grow>::append(t_crange range) noexcept {
    impl_.append(store_, range);
    return *this;
  }

  template<class TAG, t_n_ N>
  inline
  typename t_string<TAG, N, t_overflow_grow>::r_string
    t_string<TAG, N, t_overflow_grow>
      ::append(t_cstr_cptr_ fmt, ...) noexcept {
    va_list vars;
    va_start(vars, fmt);
    impl_.append(store_, string::mk_range(fmt), vars);
    va_end(vars);
    return *this;
  }

  template<class TAG, t_n_ N>
  template<class TAG1, t_n_ N1, typename O1>
  inline
  typename t_string<TAG, N, t_overflow_grow>::r_string
    t_string<TAG, N, t_overflow_grow>
      ::append(const t_string<TAG1, N1, O1>& str) noexcept {
    impl_.append(store_, str.mk_range());
    return *this;
  }

  template<class TAG, t_n_ N>
  inline
  typename t_string<TAG, N, t_overflow_grow>::r_string
    t_string<TAG, N, t_overflow_grow>
      ::assign(t_crange fmt, va_list vars) noexcept {
    impl_.assign(store_, fmt, vars);
    return *this;
  }

  template<class TAG, t_n_ N>
  inline
  typename t_string<TAG, N, t_overflow_grow>::r_string
    t_string<TAG, N, t_overflow_grow>
      ::append(t_crange fmt, va_list vars) noexcept {
    impl_.append(store_, fmt, vars);
    return *this;
  }

  template<class TAG, t_n_ N>
  inline
  t_void t_string<TAG, N, t_overflow_grow>
      ::display(t_crange prefix, t_crange postfix) const noexcept {
    impl_.display(store_, prefix, postfix);
  }

  template<class TAG, t_n_ N>
  inline
  t_void t_string<TAG, N, t_overflow_grow>
      ::display_then_clear(t_crange prefix, t_crange postfix) noexcept {
    impl_.display_then_clear(store_, prefix, postfix);
  }

  template<class TAG, t_n_ N>
  inline
  t_bool t_string<TAG, N, t_overflow_grow>
      ::is_match(t_crange pattern) const noexcept {
    return impl_.is_match(store_, pattern);
  }

  template<class TAG, t_n_ N>
  inline
  t_void t_string<TAG, N, t_overflow_grow>::clear() noexcept {
    impl_.clear(store_);
  }

  template<class TAG, t_n_ N>
  inline
  t_bool t_string<TAG, N, t_overflow_grow>::remove(t_begin_ix begin,
                                                   t_end_ix   end) noexcept {
    return impl_.remove(store_, begin, end);
  }

  template<class TAG, t_n_ N>
  inline
  t_cstr_cptr t_string<TAG, N, t_overflow_grow>::get_cstr() const noexcept {
    return t_cstr_cptr{impl_.get_cstr(store_)};
  }

  template<class TAG, t_n_ N>
  inline
  t_n t_string<TAG, N, t_overflow_grow>::get_length() const noexcept {
    return t_n{impl_.get_length()};
  }

  template<class TAG, t_n_ N>
  inline
  t_n t_string<TAG, N, t_overflow_grow>::get_capacity() const noexcept {
    return t_n{get(store_.get_capacity()) - 1};
  }

  template<class TAG, t_n_ N>
  inline
  t_bool t_string<TAG, N, t_overflow_grow>::is_empty() const noexcept {
    return impl_.is_empty();
  }

  template<class TAG, t_n_ N>
  inline
  t_n t_string<TAG, N, t_overflow_grow>::get_count(t_char c) const noexcept {
    return impl_.get_count(store_, c);
  }

  template<class TAG, t_n_ N>
  inline
  typename t_string<TAG, N, t_overflow_grow>::t_char
      t_string<TAG, N, t_overflow_grow>::get_front() const noexcept {
    return impl_.get_front(store_);
  }

  template<class TAG, t_n_ N>
  inline
  typename t_string<TAG, N, t_overflow_grow>::t_char
      t_string<TAG, N, t_overflow_grow>::get_back() const noexcept {
    return impl_.get_back(store_);
  }

  template<class TAG, t_n_ N>
  inline
  t_crange t_string<TAG, N, t_overflow_grow>::mk_range() const noexcept {
    return impl_.mk_range(store_);
  }

  template<class TAG, t_n_ N>
  inline
  t_crange t_string<TAG, N, t_overflow_grow>
      ::mk_range(t_begin_ix begin) const noexcept {
    return impl_.mk_range(store_, begin);
  }

  template<class TAG, t_n_ N>
  inline
  t_crange t_string<TAG, N, t_overflow_grow>
      ::mk_range(t_begin_ix begin, t_end_ix end) const noexcept {
    return impl_.mk_range(store_, begin, end);
  }

  template<class TAG, t_n_ N>
  template<class F>
  inline
  t_void t_string<TAG, N, t_overflow_grow>::each(F&& func) noexcept {
    impl_.each(store_, base::preserve<F>(func));
  }

  template<class TAG, t_n_ N>
  template<class F>
  inline
  t_void t_string<TAG, N, t_overflow_grow>::each(F&& func) const noexcept {
    impl_.each(store_, base::preserve<F>(func));
  }

  template<class TAG, t_n_ N>
  template<class F>
  inline
  t_void t_string<TAG, N, t_overflow_grow>::ceach(F&& func) const noexcept {
    impl_.each(store_, base::preserve<F>(func));
  }

  template<class TAG, t_n_ N>
  inline
  t_void t_string<TAG, N, t_overflow_grow>::mod_(t_ix pos, t_char ch) noexcept {
    impl_.mod_(store_, get(pos), ch);
  }

  template<class TAG, t_n_ N>
  inline
  t_n t_string<TAG, N, t_overflow_grow>::scan(t_n n, t_cstr_cptr_ fmt,
                                              ...) noexcept {
    va_list vars;
    va_start(vars, fmt);
    auto found = impl_.scan(store_, n, fmt, vars);
    va_end(vars);
    return found;
  }

  template<class TAG, t_n_ N>
  inline
  t_n t_string<TAG, N, t_overflow_grow>::scan(t_n n, t_crange fmt,
                                              va_list vars) noexcept {
    return impl_.scan(store_, n, fmt, vars);
  }

///////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
