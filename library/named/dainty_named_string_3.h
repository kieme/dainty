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

#ifndef _DAINTY_NAMED_STRING_3_H_
#define _DAINTY_NAMED_STRING_3_H_

#include "dainty_named_string_1.h"

namespace dainty
{
namespace named
{
namespace string
{
////////////////////////////////////////////////////////////////////////////////

  template<class TAG, typename O>
  class t_string<TAG, 0, O> {
    using t_impl_ = t_string_impl_<O>;
  public:
    using t_n         = named::t_n;
    using t_cstr_cptr = named::t_cstr_cptr;
    using R_crange    = t_prefix<t_crange>::R_;
    using r_string    = typename t_prefix<t_string>::r_;
    using R_string    = typename t_prefix<t_string>::R_;
    using t_char      = typename t_impl_::t_char;
    using R_block     = typename t_impl_::R_block;

    t_string(t_n max)                      noexcept;
    t_string(         t_cstr_cptr)         noexcept;
    t_string(t_n max, t_cstr_cptr)         noexcept;
    t_string(         R_block)             noexcept;
    t_string(t_n max, R_block)             noexcept;
    t_string(         R_crange)            noexcept;
    t_string(t_n max, R_crange)            noexcept;
    t_string(         R_string)            noexcept;
    t_string(t_n max, R_string)            noexcept;
    t_string(         t_fmt, t_cstr_cptr_, ...) noexcept
      __attribute__((format(printf, 3, 4)));
    t_string(t_n max, t_fmt, t_cstr_cptr_, ...) noexcept
      __attribute__((format(printf, 4, 5)));

    template<t_n_ N1>
    t_string(         const t_char (&)[N1]) noexcept;
    template<t_n_ N1>
    t_string(t_n max, const t_char (&)[N1]) noexcept;
    template<t_n_ N1, typename O1>
    t_string(         const t_string<TAG, N1, O1>&) noexcept;
    template<t_n_ N1, typename O1>
    t_string(t_n max, const t_string<TAG, N1, O1>&) noexcept;

    template<typename O1>
    t_string(t_string<TAG, 0,  O1>&&) noexcept; //XXX - impl
    template<t_n_ N1>
    t_string(t_string<TAG, N1, t_overflow_grow>&&) noexcept; //XXX - impl

    r_string operator=(t_cstr_cptr) noexcept;
    r_string operator=(R_block)     noexcept;
    r_string operator=(R_crange)    noexcept;
    r_string operator=(R_string)    noexcept;

    template<t_n_ N1>
    r_string operator=(const t_char (&)[N1])         noexcept;
    template<t_n_ N1, typename O1>
    r_string operator=(const t_string<TAG, N1, O1>&) noexcept;

    template<typename O1>
    r_string operator=(t_string<TAG, 0, O1>&&)       noexcept; // XXX impl
    template<t_n_ N1>
    r_string operator=(t_string<TAG, N1, t_overflow_grow>&&) noexcept; // XXX impl

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

    r_string va_assign(t_cstr_cptr_ fmt, va_list vars) noexcept;
    r_string va_append(t_cstr_cptr_ fmt, va_list vars) noexcept;

    t_void clear() noexcept;

    t_void display           (R_crange prefix  = NO_RANGE,
                              R_crange postfix = NO_RANGE) const noexcept;
    t_void display_then_clear(R_crange prefix  = NO_RANGE,
                              R_crange postfix = NO_RANGE)        noexcept;

    t_void    scan(t_n, t_cstr_cptr_, ...) noexcept
      __attribute__((format(scanf, 3, 4)));
    t_void va_scan(t_n, t_cstr_cptr_ fmt, va_list) noexcept;

    t_bool is_match(t_cstr_cptr pattern)                   const noexcept;
    template<t_n_ N1>
    t_bool is_match(const t_char (&pattern)[N1])           const noexcept;
    template<class TAG1, t_n_ N1, typename O1>
    t_bool is_match(const t_string<TAG1, N1, O1>& pattern) const noexcept;

    t_n         get_capacity()       const noexcept;
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
      impl_.custom_assign(store_.get(), max_, func);
      return *this;
    }

    template<typename F> r_string custom_append_(F& func) noexcept {
      impl_.custom_append(store_.get(), max_, func);
      return *this;
    }

  private:
    template<typename, t_n_, typename> friend class t_string;
    t_n_    max_  = 0; // includes '\0'
    t_ptr_  store_;
    t_impl_ impl_;
  };

///////////////////////////////////////////////////////////////////////////////

  template<class TAG, typename O>
  inline
  t_string<TAG, 0, O>::t_string(t_n max) noexcept
    : max_{get(max) + 1}, store_{alloc_(max_)}, impl_{store_.get()} {
  }

  template<class TAG, typename O>
  inline
  t_string<TAG, 0, O>::t_string(t_cstr_cptr str) noexcept
    : max_{length_(get(str)+1)}, store_{alloc_(max_)},
      impl_{store_.get(), max_, str} {
  }

  template<class TAG, typename O>
  inline
  t_string<TAG, 0, O>::t_string(t_n max, t_cstr_cptr str) noexcept
      : t_string{max} {
    operator=(str);
  }

  template<class TAG, typename O>
  inline
  t_string<TAG, 0, O>::t_string(R_block block) noexcept
    : max_{get(block.n) + 1}, store_{alloc_(max_)},
      impl_{store_.get(), max_, block} {
  }

  template<class TAG, typename O>
  inline
  t_string<TAG, 0, O>::t_string(t_n max, R_block block) noexcept
    : t_string{max} {
    operator=(block);
  }

  template<class TAG, typename O>
  inline
  t_string<TAG, 0, O>::t_string(R_crange range) noexcept
    : max_{get(range.n) + 1}, store_{alloc_(max_)},
      impl_{store_.get(), max_, range} {
  }

  template<class TAG, typename O>
  inline
  t_string<TAG, 0, O>::t_string(t_n max, R_crange range) noexcept
    : t_string{max} {
    operator=(range);
  }

  template<class TAG, typename O>
  inline
  t_string<TAG, 0, O>::t_string(t_fmt, t_cstr_cptr_ fmt, ...) noexcept {
    va_list vars;
    va_start(vars, fmt);
    max_   = length_(fmt, vars) + 1;
    store_ = alloc_ (max_);
    impl_.va_assign(store_.get(), max_, fmt, vars);
    va_end(vars);
  }

  template<class TAG, typename O>
  inline
  t_string<TAG, 0, O>::t_string(t_n max, t_fmt, t_cstr_cptr_ fmt, ...) noexcept
      : t_string{max} {
    va_list vars;
    va_start(vars, fmt);
    va_assign(fmt, vars);
    va_end(vars);
  }

  template<class TAG, typename O>
  inline
  t_string<TAG, 0, O>::t_string(R_string str) noexcept
    : max_{get(str.get_length) + 1}, store_{alloc_(max_)},
      impl_{store_.get(), max_, get(str.get_cstr())} {
  }

  template<class TAG, typename O>
  inline
  t_string<TAG, 0, O>::t_string(t_n max, R_string str) noexcept
      : t_string{max} {
    operator=(str);
  }

  template<class TAG, typename O>
  template<t_n_ N1>
  inline
  t_string<TAG, 0, O>::t_string(const t_char (&str)[N1]) noexcept
    : max_{N1}, store_{alloc_(max_)}, impl_{store_.get(), max_, str} {
  }

  template<class TAG, typename O>
  template<t_n_ N1>
  inline
  t_string<TAG, 0, O>::t_string(t_n max, const t_char (&str)[N1]) noexcept
      : t_string{max} {
    operator=(t_crange{str, t_n{N1-1}});
  }

  template<class TAG, typename O>
  template<t_n_ N1, typename O1>
  inline
  t_string<TAG, 0, O>::t_string(const t_string<TAG, N1, O1>& str) noexcept
    : max_{get(str.get_length()) + 1}, store_{alloc_(max_)},
      impl_{store_.get(), max_, get(str.get_cstr())} {
  }

  template<class TAG, typename O>
  template<t_n_ N1, typename O1>
  inline
  t_string<TAG, 0, O>::t_string(t_n max,
                                const t_string<TAG, N1, O1>& str) noexcept
      : t_string{max} {
    operator=(str);
  }

  template<class TAG, typename O>
  inline
  typename t_string<TAG, 0, O>::r_string t_string<TAG, 0, O>
      ::operator=(t_cstr_cptr str) noexcept {
    impl_.assign(store_.get(), max_, get(str));
    return *this;
  }

  template<class TAG, typename O>
  inline
  typename t_string<TAG, 0, O>::r_string t_string<TAG, 0, O>
      ::operator=(R_block block) noexcept {
    impl_.assign(store_.get(), max_, block);
    return *this;
  }

  template<class TAG, typename O>
  inline
  typename t_string<TAG, 0, O>::r_string t_string<TAG, 0, O>
      ::operator=(R_crange range) noexcept {
    impl_.assign(store_.get(), max_, range);
    return *this;
  }

  template<class TAG, typename O>
  inline
  typename t_string<TAG, 0, O>::r_string t_string<TAG, 0, O>
      ::operator=(R_string str) noexcept {
    impl_.assign(store_.get(), max_, get(str.get_cstr()));
    return *this;
  }

  template<class TAG, typename O>
  template<t_n_ N1>
  inline
  typename t_string<TAG, 0, O>::r_string t_string<TAG, 0, O>
      ::operator=(const t_char (&str)[N1]) noexcept {
    impl_.assign(store_.get(), max_, str);
    return *this;
  }

  template<class TAG, typename O>
  template<t_n_ N1, typename O1>
  inline
  typename t_string<TAG, 0, O>::r_string t_string<TAG, 0, O>
      ::operator=(const t_string<TAG, N1, O1>& str) noexcept {
    impl_.assign(store_.get(), max_, get(str.get_cstr()));
    return *this;
  }

  template<class TAG, typename O>
  inline
  typename t_string<TAG, 0, O>::r_string t_string<TAG, 0, O>
      ::assign(t_fmt, t_cstr_cptr_ fmt, ...) noexcept {
    va_list vars;
    va_start(vars, fmt);
    impl_.va_assign(store_.get(), max_, fmt, vars);
    va_end(vars);
    return *this;
  }

  template<class TAG, typename O>
  template<class TAG1, t_n_ N1, typename O1>
  inline
  typename t_string<TAG, 0, O>::r_string t_string<TAG, 0, O>
      ::assign(const t_string<TAG1, N1, O1>& str) noexcept {
    impl_.assign(store_.get(), max_, get(str.get_cstr()));
    return *this;
  }

  template<class TAG, typename O>
  inline
  typename t_string<TAG, 0, O>::r_string t_string<TAG, 0, O>
      ::append(t_cstr_cptr str) noexcept {
    impl_.append(store_.get(), max_, get(str));
    return *this;
  }

  template<class TAG, typename O>
  inline
  typename t_string<TAG, 0, O>::r_string t_string<TAG, 0, O>
      ::append(R_block block) noexcept {
    impl_.append(store_.get(), max_, block);
    return *this;
  }

  template<class TAG, typename O>
  inline
  typename t_string<TAG, 0, O>::r_string t_string<TAG, 0, O>
      ::append(R_crange range) noexcept {
    impl_.append(store_.get(), max_, range);
    return *this;
  }

  template<class TAG, typename O>
  inline
  typename t_string<TAG, 0, O>::r_string t_string<TAG, 0, O>
      ::append(t_fmt, t_cstr_cptr_ fmt, ...) noexcept {
    va_list vars;
    va_start(vars, fmt);
    impl_.va_append(store_.get(), max_, fmt, vars);
    va_end(vars);
    return *this;
  }

  template<class TAG, typename O>
  template<t_n_ N1>
  inline
  typename t_string<TAG, 0, O>::r_string t_string<TAG, 0, O>
      ::append(const t_char (&str)[N1]) noexcept {
    impl_.append(store_.get(), max_, str);
    return *this;
  }

  template<class TAG, typename O>
  template<class TAG1, t_n_ N1, typename O1>
  inline
  typename t_string<TAG, 0, O>::r_string t_string<TAG, 0, O>
      ::append(const t_string<TAG1, N1, O1>& str) noexcept {
    impl_.append(store_.get(), max_, get(str.get_cstr()));
    return *this;
  }

  template<class TAG, typename O>
  inline
  typename t_string<TAG, 0, O>::r_string t_string<TAG, 0, O>
      ::va_assign(t_cstr_cptr_ fmt, va_list vars) noexcept {
    impl_.va_assign(store_.get(), max_, fmt, vars);
    return *this;
  }

  template<class TAG, typename O>
  inline
  typename t_string<TAG, 0, O>::r_string t_string<TAG, 0, O>
      ::va_append(t_cstr_cptr_ fmt, va_list vars) noexcept {
    impl_.va_append(store_.get(), max_, fmt, vars);
    return *this;
  }

  template<class TAG, typename O>
  inline
  t_void t_string<TAG, 0, O>::display(R_crange prefix,
                                      R_crange postfix) const noexcept {
    impl_.display(store_.get(), prefix, postfix);
  }

  template<class TAG, typename O>
  inline
  t_void t_string<TAG, 0, O>::display_then_clear(R_crange prefix,
                                                 R_crange postfix) noexcept {
    impl_.display_then_clear(store_.get(), prefix, postfix);
  }

  template<class TAG, typename O>
  inline
  t_bool t_string<TAG, 0, O>::is_match(t_cstr_cptr pattern) const noexcept {
    return impl_.is_match(store_.get(), get(pattern));
  }

  template<class TAG, typename O>
  template<t_n_ N1>
  inline
  t_bool t_string<TAG, 0, O>
      ::is_match(const t_char (&pattern)[N1]) const noexcept {
    return impl_.is_match(store_.get(), pattern);
  }

  template<class TAG, typename O>
  template<class TAG1, t_n_ N1, typename O1>
  inline
  t_bool t_string<TAG, 0, O>
      ::is_match(const t_string<TAG1, N1, O1>& pattern) const noexcept {
    return impl_.is_match(store_.get(), get(pattern.get_cstr()));
  }

  template<class TAG, typename O>
  inline
  t_void t_string<TAG, 0, O>::clear() noexcept {
    return impl_.clear(store_.get());
  }

  template<class TAG, typename O>
  inline
  t_cstr_cptr t_string<TAG, 0, O>::get_cstr() const noexcept {
    return t_cstr_cptr{impl_.get_cstr(store_.get())};
  }

  template<class TAG, typename O>
  inline
  t_n t_string<TAG, 0, O>::get_length() const noexcept {
    return t_n{impl_.get_length()};
  }

  template<class TAG, typename O>
  inline
  t_n t_string<TAG, 0, O>::get_capacity() const noexcept {
    return t_n{max_ - 1};
  }

  template<class TAG, typename O>
  inline
  t_bool t_string<TAG, 0, O>::is_empty() const noexcept {
    return impl_.is_empty();
  }

  template<class TAG, typename O>
  inline
  t_n t_string<TAG, 0, O>::get_count(t_char c) const noexcept {
    return t_n{impl_.get_count(store_.get(), c)};
  }

  template<class TAG, typename O>
  inline
  typename t_string<TAG, 0, O>::t_char t_string<TAG, 0, O>
      ::get_front() const noexcept {
    return impl_.get_front(store_.get());
  }

  template<class TAG, typename O>
  inline
  typename t_string<TAG, 0, O>::t_char t_string<TAG, 0, O>
      ::get_back() const noexcept {
    return impl_.get_back(store_.get());
  }

  template<class TAG, typename O>
  inline
  t_crange t_string<TAG, 0, O>::mk_range() const noexcept {
    return impl_.mk_range(store_.get());
  }

  template<class TAG, typename O>
  inline
  t_crange t_string<TAG, 0, O>::mk_range(t_ix begin) const noexcept {
    return impl_.mk_range(store_.get(), begin);
  }

  template<class TAG, typename O>
  inline
  t_crange t_string<TAG, 0, O>::mk_range(t_ix begin, t_ix end) const noexcept {
    return impl_.mk_range(store_.get(), begin, end);
  }

  template<class TAG, typename O>
  inline
  t_string<TAG, 0, O>::operator t_crange() const noexcept {
    return mk_range();
  }

  template<class TAG, typename O>
  template<class F>
  inline
  t_void t_string<TAG, 0, O>::each(F f) noexcept {
    impl_.each(store_.get(), f);
  }

  template<class TAG, typename O>
  template<class F>
  inline
  t_void t_string<TAG, 0, O>::each(F f) const noexcept {
    impl_.each(store_.get(), f);
  }

  template<class TAG, typename O>
  template<class F>
  inline
  t_void t_string<TAG, 0, O>::ceach(F f) const noexcept {
    impl_.each(store_.get(), f);
  }

  template<class TAG, typename O>
  inline
  t_void t_string<TAG, 0, O>::mod_(t_ix pos, t_char ch) noexcept {
    impl_.mod_(store_.get(), get(pos), ch);
  }

  template<class TAG, typename O>
  inline
  t_void t_string<TAG, 0, O>::scan(t_n n, t_cstr_cptr_ fmt, ...) noexcept {
    va_list vars;
    va_start(vars, fmt);
    impl_.va_scan(store_.get(), get(n), fmt, vars);
    va_end(vars);
  }

  template<class TAG, typename O>
  inline
  t_void t_string<TAG, 0, O>::va_scan(t_n n, t_cstr_cptr_ fmt,
                                      va_list vars) noexcept {
    impl_.va_scan(store_.get(), get(n), fmt, vars);
  }
///////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
