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

#ifndef _DAINTY_NAMED_STRING_2_H_
#define _DAINTY_NAMED_STRING_2_H_

#include "dainty_named_string_1.h"

namespace dainty
{
namespace named
{
namespace string
{
////////////////////////////////////////////////////////////////////////////////

  template<typename TAG, t_n_ N>
  class t_string<TAG, N, t_overflow_grow> {
    using t_impl_ = t_string_impl_<t_overflow_grow>;
  public:
    using t_n         = named::t_n;
    using t_cstr_cptr = named::t_cstr_cptr;
    using R_crange    = t_prefix<t_crange>::R_;
    using r_string    = typename t_prefix<t_string>::r_;
    using R_string    = typename t_prefix<t_string>::R_;
    using t_char      = typename t_impl_::t_char;
    using R_block     = typename t_impl_::R_block;

    t_string()                              noexcept;
    t_string(t_n max)                       noexcept;
    t_string(         t_cstr_cptr)          noexcept;
    t_string(t_n max, t_cstr_cptr)          noexcept;
    t_string(         R_block)              noexcept;
    t_string(t_n max, R_block)              noexcept;
    t_string(         R_crange)             noexcept;
    t_string(t_n max, R_crange)             noexcept;
    t_string(         R_string)             noexcept;
    t_string(t_n max, R_string)             noexcept;
    t_string(         t_fmt, t_cstr_cptr_, ...)  noexcept
      __attribute__((format(printf, 3, 4)));
    t_string(t_n max, t_fmt, t_cstr_cptr_, ...)  noexcept
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
    t_string(t_string<TAG, 0, O1>&&) noexcept;
    //template<t_n_ N1>
    //t_string(t_string<TAG, N1, t_overflow_grow>&&) noexcept; //XXX

   ~t_string();

    r_string operator=(t_cstr_cptr) noexcept;
    r_string operator=(R_block)     noexcept;
    r_string operator=(R_crange)    noexcept;
    r_string operator=(R_string)    noexcept;

    template<t_n_ N1>
    r_string operator=(const t_char (&)[N1])         noexcept;
    template<t_n_ N1, typename O1>
    r_string operator=(const t_string<TAG, N1, O1>&) noexcept;

    template<typename O1>
    r_string operator=(t_string<TAG, 0, O1>&&)       noexcept;
    //template<t_n_ N1>
    //r_string operator=(t_string<TAG, N1, t_overflow_grow>&&) noexcept; // XXX

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
    t_bool remove(t_ix begin, t_ix end) noexcept;

    t_void display           (R_crange prefix  = NO_RANGE,
                              R_crange postfix = NO_RANGE) const noexcept;
    t_void display_then_clear(R_crange prefix  = NO_RANGE,
                              R_crange postfix = NO_RANGE)        noexcept;

    t_void    scan(t_n, t_cstr_cptr_, ...)         noexcept
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
      if (assign_size_(impl_.custom_assign(store_, max_, func)))
        impl_.custom_assign(store_, max_, func);
      return *this;
    }

    template<typename F> r_string custom_append_(F& func) noexcept {
      if (append_size_(impl_.custom_append(store_, max_, func)))
        impl_.custom_append(store_, max_, func);
      return *this;
    }

  private:
    template<typename, t_n_, typename> friend class t_string;
    t_bool assign_size_(t_n_) noexcept;
    t_bool append_size_(t_n_) noexcept;

    constexpr static t_n_ MAX_ = N + 1;

    t_char  sso_[MAX_];
    t_n_    max_;
    p_cstr_ store_ = nullptr;
    t_impl_ impl_;
  };

///////////////////////////////////////////////////////////////////////////////

  template<class TAG, t_n_ N>
  inline
  t_string<TAG, N, t_overflow_grow>::t_string() noexcept
    : max_{MAX_}, store_{sso_}, impl_{store_} {
  }

  template<class TAG, t_n_ N>
  inline
  t_string<TAG, N, t_overflow_grow>::t_string(t_n max) noexcept
    : max_{get(max) + 1},
      store_{sso_alloc_(sso_, N, max_)}, impl_{store_} {
  }

  template<class TAG, t_n_ N>
  inline
  t_string<TAG, N, t_overflow_grow>::t_string(t_cstr_cptr str) noexcept
    : max_{length_(get(str) + 1)}, store_{sso_alloc_(sso_, MAX_, max_)},
      impl_{store_, max_, get(str)} {
  }

  template<class TAG, t_n_ N>
  inline
  t_string<TAG, N, t_overflow_grow>::t_string(t_n max, t_cstr_cptr str) noexcept
      : t_string{max} {
     operator=(str);
  }

  template<class TAG, t_n_ N>
  inline
  t_string<TAG, N, t_overflow_grow>::t_string(R_block block) noexcept
    : max_{get(block.max) + 1}, store_{sso_alloc_(sso_, MAX_, max_)},
      impl_{store_, max_, block} {
  }

  template<class TAG, t_n_ N>
  inline
  t_string<TAG, N, t_overflow_grow>::t_string(t_n max, R_block block) noexcept
      : t_string{max} {
     operator=(block);
  }

  template<class TAG, t_n_ N>
  inline
  t_string<TAG, N, t_overflow_grow>::t_string(R_crange range) noexcept
    : max_{get(range.n) + 1}, store_{sso_alloc_(sso_, N, max_)},
      impl_{store_, max_, range} {
  }

  template<class TAG, t_n_ N>
  inline
  t_string<TAG, N, t_overflow_grow>::t_string(t_n max, R_crange range) noexcept
      : t_string{max} {
    operator=(range);
  }

  template<class TAG, t_n_ N>
  inline
  t_string<TAG, N, t_overflow_grow>::t_string(t_fmt,
                                              t_cstr_cptr_ fmt, ...) noexcept {
    va_list vars;
    va_start(vars, fmt);
    va_assign(fmt, vars);
    va_end(vars);
  }

  template<class TAG, t_n_ N>
  inline
  t_string<TAG, N, t_overflow_grow>::t_string(t_n max, t_fmt, t_cstr_cptr_ fmt,
                                              ...) noexcept
      : t_string{max} {
    va_list vars;
    va_start(vars, fmt);
    va_assign(fmt, vars);
    va_end(vars);
  }

  template<class TAG, t_n_ N>
  inline
  t_string<TAG, N, t_overflow_grow>::t_string(R_string str) noexcept
    : max_{get(str.get_length()) + 1}, store_{sso_alloc_(sso_, MAX_, max_)},
      impl_{store_, max_, get(str.get_cstr())} {
  }

  template<class TAG, t_n_ N>
  inline
  t_string<TAG, N, t_overflow_grow>::t_string(t_n max, R_string str) noexcept
      : t_string{max} {
    operator=(str);
  }

  template<class TAG, t_n_ N>
  template<t_n_ N1>
  inline
  t_string<TAG, N, t_overflow_grow>::t_string(const t_char (&str)[N1]) noexcept
    : max_{N1}, store_{sso_alloc_(sso_, MAX_, max_)},
      impl_{store_, max_, str} {
  }

  template<class TAG, t_n_ N>
  template<t_n_ N1>
  inline
  t_string<TAG, N, t_overflow_grow>::t_string(t_n max,
                                              const t_char (&str)[N1]) noexcept
      : t_string{max} {
    operator=(t_crange{str, t_n{N1-1}});
  }

  template<class TAG, t_n_ N>
  template<t_n_ N1, typename O1>
  inline
  t_string<TAG, N, t_overflow_grow>
      ::t_string(const t_string<TAG, N1, O1>& str) noexcept
    : max_{get(str.get_length()) + 1}, store_{sso_alloc_(sso_, MAX_, max_)},
      impl_{store_, max_, get(str.get_cstr())} {
  }

  template<class TAG, t_n_ N>
  template<t_n_ N1, typename O1>
  inline
  t_string<TAG, N, t_overflow_grow>
      ::t_string(t_n max, const t_string<TAG, N1, O1>& str) noexcept
      : t_string{max} {
    operator=(str);
  }

  template<class TAG, t_n_ N>
  template<typename O1>
  inline
  t_string<TAG, N, t_overflow_grow>
      ::t_string(t_string<TAG, 0, O1>&& str) noexcept //XXX - incorrect
    : max_{named::reset(str.max_)}, store_{str.store_.release()},
      impl_{str.impl_.reset()} {
  }

  template<class TAG, t_n_ N>
  inline
  t_string<TAG, N, t_overflow_grow>::~t_string() {
    if (store_ != sso_)
      dealloc_(store_);
  }

  template<class TAG, t_n_ N>
  inline
  t_bool t_string<TAG, N, t_overflow_grow>::assign_size_(t_n_ need) noexcept {
    if (need >= max_) {
      if (store_ != sso_)
        dealloc_(store_);
      max_   = need + 1;
      store_ = sso_alloc_(sso_, MAX_, max_);
      return true;
    }
    return false;
  }

  template<class TAG, t_n_ N>
  inline
  t_bool t_string<TAG, N, t_overflow_grow>::append_size_(t_n_ need) noexcept {
    auto len = impl_.get_length(), left = max_ - len;
    if (need >= left) {
      max_   = len + need + 1;
      store_ = sso_alloc_(sso_, MAX_, store_, max_);
      return true;
    }
    return false;
  }

  template<class TAG, t_n_ N>
  inline
  typename t_string<TAG, N, t_overflow_grow>::r_string
      t_string<TAG, N, t_overflow_grow>::operator=(t_cstr_cptr str) noexcept {
    assign_size_(length_(get(str)));
    impl_.assign(store_, max_, get(str));
    return *this;
  }

  template<class TAG, t_n_ N>
  inline
  typename t_string<TAG, N, t_overflow_grow>::r_string
      t_string<TAG, N, t_overflow_grow>::operator=(R_block block) noexcept {
    assign_size_(get(block.max));
    impl_.assign(store_, max_, block);
    return *this;
  }

  template<class TAG, t_n_ N>
  inline
  typename t_string<TAG, N, t_overflow_grow>::r_string
      t_string<TAG, N, t_overflow_grow>::operator=(R_crange range) noexcept {
    assign_size_(get(range.n));
    impl_.assign(store_, max_, range);
    return *this;
  }

  template<class TAG, t_n_ N>
  inline
  typename t_string<TAG, N, t_overflow_grow>::r_string
      t_string<TAG, N, t_overflow_grow>::operator=(R_string str) noexcept {
    assign_size_(get(str.get_length()));
    impl_.assign(store_, max_, get(str.get_cstr()));
    return *this;
  }

  template<class TAG, t_n_ N>
  template<t_n_ N1>
  inline
  typename t_string<TAG, N, t_overflow_grow>::r_string
    t_string<TAG, N, t_overflow_grow>
      ::operator=(const t_char (&str)[N1]) noexcept {
    assign_size_(N1-1);
    impl_.assign(store_, max_, str);
    return *this;
  }

  template<class TAG, t_n_ N>
  template<t_n_ N1, typename O1>
  inline
  typename t_string<TAG, N, t_overflow_grow>::r_string
    t_string<TAG, N, t_overflow_grow>
      ::operator=(const t_string<TAG, N1, O1>& str) noexcept {
    assign_size_(get(str.get_length()));
    impl_.assign(store_, max_, get(str.get_cstr()));
    return *this;
  }

  template<class TAG, t_n_ N>
  template<typename O1>
  inline
  typename t_string<TAG, N, t_overflow_grow>::r_string
    t_string<TAG, N, t_overflow_grow>
      ::operator=(t_string<TAG, 0, O1>&& str) noexcept { // XXX incorrect
    if (store_ != sso_)
      dealloc_(store_);
    impl_.reset(str.impl_.reset());
    max_   = named::reset(str.max_);
    store_ = str.store_.release();
    return *this;
  }

  template<class TAG, t_n_ N>
  inline
  typename t_string<TAG, N, t_overflow_grow>::r_string
    t_string<TAG, N, t_overflow_grow>
      ::assign(t_fmt, t_cstr_cptr_ fmt, ...) noexcept {
    va_list vars;
    va_start(vars, fmt);
    va_assign(fmt, vars);
    va_end(vars);
    return *this;
  }

  template<class TAG, t_n_ N>
  template<class TAG1, t_n_ N1, typename O1>
  inline
  typename t_string<TAG, N, t_overflow_grow>::r_string
    t_string<TAG, N, t_overflow_grow>
      ::assign(const t_string<TAG1, N1, O1>& str) noexcept {
    assign_size_(get(str.get_length()));
    impl_.assign(store_, max_, get(str.get_cstr()));
    return *this;
  }

  template<class TAG, t_n_ N>
  inline
  typename t_string<TAG, N, t_overflow_grow>::r_string
      t_string<TAG, N, t_overflow_grow>::append(t_cstr_cptr str) noexcept {
    append_size_(length_(get(str)));
    impl_.append(store_, max_, get(str));
    return *this;
  }

  template<class TAG, t_n_ N>
  inline
  typename t_string<TAG, N, t_overflow_grow>::r_string
      t_string<TAG, N, t_overflow_grow>::append(R_block block) noexcept {
    append_size_(get(block.max));
    impl_.append(store_, max_, block);
    return *this;
  }

  template<class TAG, t_n_ N>
  inline
  typename t_string<TAG, N, t_overflow_grow>::r_string
      t_string<TAG, N, t_overflow_grow>::append(R_crange range) noexcept {
    append_size_(get(range.n));
    impl_.append(store_, max_, range);
    return *this;
  }

  template<class TAG, t_n_ N>
  inline
  typename t_string<TAG, N, t_overflow_grow>::r_string
    t_string<TAG, N, t_overflow_grow>
      ::append(t_fmt, t_cstr_cptr_ fmt, ...) noexcept {
    va_list vars;
    va_start(vars, fmt);
    va_append(fmt, vars);
    va_end(vars);
    return *this;
  }

  template<class TAG, t_n_ N>
  template<t_n_ N1>
  inline
  typename t_string<TAG, N, t_overflow_grow>::r_string
    t_string<TAG, N, t_overflow_grow>
      ::append(const t_char (&str)[N1]) noexcept {
    append_size_(N1-1);
    impl_.append(store_, max_, str);
    return *this;
  }

  template<class TAG, t_n_ N>
  template<class TAG1, t_n_ N1, typename O1>
  inline
  typename t_string<TAG, N, t_overflow_grow>::r_string
    t_string<TAG, N, t_overflow_grow>
      ::append(const t_string<TAG1, N1, O1>& str) noexcept {
    append_size_(get(str.get_length()));
    impl_.append(store_, max_, get(str.get_cstr()));
    return *this;
  }

  template<class TAG, t_n_ N>
  inline
  typename t_string<TAG, N, t_overflow_grow>::r_string
    t_string<TAG, N, t_overflow_grow>
      ::va_assign(t_cstr_cptr_ fmt, va_list vars) noexcept {
    if (assign_size_(impl_.va_assign_try(store_, max_, fmt, vars)))
      impl_.va_assign(store_, max_, fmt, vars);
    return *this;
  }

  template<class TAG, t_n_ N>
  inline
  typename t_string<TAG, N, t_overflow_grow>::r_string
    t_string<TAG, N, t_overflow_grow>
      ::va_append(t_cstr_cptr_ fmt, va_list vars) noexcept {
    if (append_size_(impl_.va_append_try(store_, max_, fmt, vars)))
      impl_.va_append(store_, max_, fmt, vars);
    return *this;
  }

  template<class TAG, t_n_ N>
  inline
  t_void t_string<TAG, N, t_overflow_grow>
      ::display(R_crange prefix, R_crange postfix) const noexcept {
    impl_.display(store_, prefix, postfix);
  }

  template<class TAG, t_n_ N>
  inline
  t_void t_string<TAG, N, t_overflow_grow>
      ::display_then_clear(R_crange prefix, R_crange postfix) noexcept {
    impl_.display_then_clear(store_, prefix, postfix);
  }

  template<class TAG, t_n_ N>
  inline
  t_bool t_string<TAG, N, t_overflow_grow>
      ::is_match(t_cstr_cptr pattern) const noexcept {
    return impl_.is_match(store_, get(pattern));
  }

  template<class TAG, t_n_ N>
  template<t_n_ N1>
  inline
  t_bool t_string<TAG, N, t_overflow_grow>
      ::is_match(const t_char (&pattern)[N1]) const noexcept {
    return impl_.is_match(store_, pattern);
  }

  template<class TAG, t_n_ N>
  template<class TAG1, t_n_ N1, typename O1>
  inline
  t_bool t_string<TAG, N, t_overflow_grow>
      ::is_match(const t_string<TAG1, N1, O1>& pattern) const noexcept {
    return impl_.is_match(store_, get(pattern.get_cstr()));
  }

  template<class TAG, t_n_ N>
  inline
  t_void t_string<TAG, N, t_overflow_grow>::clear() noexcept {
    impl_.clear(store_);
  }

  template<class TAG, t_n_ N>
  inline
  t_bool t_string<TAG, N, t_overflow_grow>::remove(t_ix begin,
                                                   t_ix end) noexcept {
    return impl_.remove(store_, get(begin), get(end));
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
    return t_n{max_ ? max_ - 1 : 0};
  }

  template<class TAG, t_n_ N>
  inline
  t_bool t_string<TAG, N, t_overflow_grow>::is_empty() const noexcept {
    return impl_.is_empty();
  }

  template<class TAG, t_n_ N>
  inline
  t_n t_string<TAG, N, t_overflow_grow>::get_count(t_char c) const noexcept {
    return t_n{impl_.get_count(store_, c)};
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
      ::mk_range(t_ix begin) const noexcept {
    return impl_.mk_range(store_, begin);
  }

  template<class TAG, t_n_ N>
  inline
  t_crange t_string<TAG, N, t_overflow_grow>
      ::mk_range(t_ix begin, t_ix end) const noexcept {
    return impl_.mk_range(store_, begin, end);
  }

  template<class TAG, t_n_ N>
  inline
  t_string<TAG, N, t_overflow_grow>::operator t_crange() const noexcept {
    return mk_range();
  }

  template<class TAG, t_n_ N>
  template<class F>
  inline
  t_void t_string<TAG, N, t_overflow_grow>::each(F f) noexcept {
    impl_.each(store_, f);
  }

  template<class TAG, t_n_ N>
  template<class F>
  inline
  t_void t_string<TAG, N, t_overflow_grow>::each(F f) const noexcept {
    impl_.each(store_, f);
  }

  template<class TAG, t_n_ N>
  template<class F>
  inline
  t_void t_string<TAG, N, t_overflow_grow>::ceach(F f) const noexcept {
    impl_.each(store_, f);
  }

  template<class TAG, t_n_ N>
  inline
  t_void t_string<TAG, N, t_overflow_grow>::mod_(t_ix pos, t_char ch) noexcept {
    impl_.mod_(store_, get(pos), ch);
  }


  template<class TAG, t_n_ N>
  inline
  t_void t_string<TAG, N, t_overflow_grow>::scan(t_n n, t_cstr_cptr_ fmt,
                                                 ...) noexcept {
    va_list vars;
    va_start(vars, fmt);
    impl_.va_scan(store_, get(n), fmt, vars);
    va_end(vars);
  }

  template<class TAG, t_n_ N>
  inline
  t_void t_string<TAG, N, t_overflow_grow>::va_scan(t_n n, t_cstr_cptr_ fmt,
                                                    va_list vars) noexcept {
    impl_.va_scan(store_, get(n), fmt, vars);
  }

///////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
