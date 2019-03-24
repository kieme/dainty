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

  template<class TAG, t_n_ N>
  class t_string<TAG, N, OVERFLOW_GROW> {
    using t_impl_ = t_string_impl_<OVERFLOW_ASSERT>;
  public:
    using t_n      = named::t_n;
    using P_cstr   = named::P_cstr;
    using R_crange = t_prefix<t_crange>::R_;
    using r_string = typename t_prefix<t_string>::r_;
    using R_string = typename t_prefix<t_string>::R_;
    using t_char   = typename t_impl_::t_char;
    using R_block  = typename t_impl_::R_block;

    t_string()                    noexcept;
    t_string(t_n max)             noexcept;
    t_string(P_cstr)              noexcept;
    t_string(R_block)             noexcept;
    t_string(R_crange)            noexcept;
    t_string(R_string)            noexcept;
    t_string(t_fmt, P_cstr_, ...) noexcept
      __attribute__((format(printf, 3, 4)));

    template<t_overflow O1>
    t_string(t_string<TAG, 0, O1>&&) noexcept;

    template<t_n_ N1>
    t_string(const t_char (&)[N1])         noexcept;
    template<t_n_ N1, t_overflow O1>
    t_string(const t_string<TAG, N1, O1>&) noexcept;
   ~t_string();

    r_string operator=(P_cstr)   noexcept;
    r_string operator=(R_block)  noexcept;
    r_string operator=(R_crange) noexcept;
    r_string operator=(R_string) noexcept;

    template<t_n_ N1>
    r_string operator=(const t_char (&)[N1])         noexcept;
    template<t_n_ N1, t_overflow O1>
    r_string operator=(const t_string<TAG, N1, O1>&) noexcept;
    template<t_overflow O1>
    r_string operator=(t_string<TAG, 0, O1>&&)       noexcept;

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

    t_n    get_capacity()       const noexcept;
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
    t_void maybe_adjust_  (t_n_) noexcept;
    t_void maybe_readjust_(t_n_) noexcept;

    t_char  sso_[N+1];
    t_n_    max_;
    p_cstr_ store_ = nullptr;
    t_impl_ impl_;
  };

///////////////////////////////////////////////////////////////////////////////

  template<class TAG, t_n_ N>
  inline
  t_string<TAG, N, OVERFLOW_GROW>::t_string() noexcept
    : max_{N}, store_{sso_}, impl_{store_} {
  }

  template<class TAG, t_n_ N>
  inline
  t_string<TAG, N, OVERFLOW_GROW>::t_string(t_n max) noexcept
    : max_{get(max)},
      store_{sso_alloc_(sso, N, max_+1)}, impl_{store_} {
  }

  template<class TAG, t_n_ N>
  inline
  t_string<TAG, N, OVERFLOW_GROW>::t_string(P_cstr str) noexcept
    : max_{length_(get(str))}, store_{sso_alloc_(sso_, N+1, max_+1)},
      impl_{store_, max_, get(str)} {
  }

  template<class TAG, t_n_ N>
  inline
  t_string<TAG, N, OVERFLOW_GROW>::t_string(R_block block) noexcept
    : max_{get(block.max)}, store_{sso_alloc_(sso_, N+1, max_+1)},
      impl_{store_, max_, block} {
  }

  template<class TAG, t_n_ N>
  inline
  t_string<TAG, N, OVERFLOW_GROW>::t_string(R_crange range) noexcept
    : max_{get(range.n)}, store_{sso_alloc_(sso, N+1, max_+1)},
      impl_{store_, max_, begin(range), get(range.n)} {
  }

  template<class TAG, t_n_ N>
  inline
  t_string<TAG, N, OVERFLOW_GROW>::t_string(t_fmt, P_cstr_ fmt, ...) noexcept {
    va_list vars;
    va_start(vars, fmt);
    va_assign(fmt, vars);
    va_end(vars);
  }

  template<class TAG, t_n_ N>
  inline
  t_string<TAG, N, OVERFLOW_GROW>::t_string(R_string str) noexcept
    : max_{calc_n_(get(str.get_length()), 0)},
      store_{alloc_(max_)}, impl_{store_.get(), max_, get(str.get_cstr())} {
  }

  template<class TAG, t_n_ N>
  template<t_n_ N1>
  inline
  t_string<TAG, N, OVERFLOW_GROW>::t_string(const t_char (&str)[N1]) noexcept
    : max_{calc_n_(N1-1, 0)}, store_{alloc_(max_)},
      impl_{store_.get(), max_, str} {
  }

  template<class TAG, t_n_ N>
  template<t_n_ N1, t_overflow O1>
  inline
  t_string<TAG, N, OVERFLOW_GROW>
      ::t_string(const t_string<TAG, N1, O1>& str) noexcept
    : max_{calc_n_(get(str.get_length()), 0)},
      store_{alloc_(max_)}, impl_{store_.get(), max_, get(str.get_cstr())} {
  }

  template<class TAG, t_n_ N>
  template<t_overflow O1>
  inline
  t_string<TAG, N, OVERFLOW_GROW>
      ::t_string(t_string<TAG, 0, O1>&& str) noexcept
    : max_{utility::reset(str.max_)}, //XXX
      store_{utility::x_cast(str.store_)}, impl_{str.impl_.reset()} {
  }

  template<class TAG, t_n_ N>
  inline
  t_string<TAG, N, OVERFLOW_GROW>::~t_string() noexcept
    if (store_ != sso_)
      dealloc_(store_);
  }

  template<class TAG, t_n_ N>
  inline
  t_void t_string<TAG, N, OVERFLOW_GROW>::maybe_adjust_(t_n_ need) noexcept {
    if (need > max_) {
      if (store_ == VALID)
        dealloc_(store_.release());
      max_   = calc_n_(need, 0);
      store_ = alloc_ (max_);
    }
  }

  template<class TAG, t_n_ N>
  inline
  t_void t_string<TAG, N, OVERFLOW_GROW>::maybe_readjust_(t_n_ need) noexcept {
    auto len = impl_.get_length(), left = max_ - len;
    if (need > left) {
      max_   = calc_n_ (len + need, 0);
      store_ = realloc_(store_.release(), max_);
    }
  }

  template<class TAG, t_n_ N>
  inline
  typename t_string<TAG, N, OVERFLOW_GROW>::r_string
      t_string<TAG, N, OVERFLOW_GROW>::operator=(P_cstr str) noexcept {
    maybe_adjust_(length_(get(str)));
    impl_.assign(store_.get(), max_, get(str));
    return *this;
  }

  template<class TAG, t_n_ N>
  inline
  typename t_string<TAG, N, OVERFLOW_GROW>::r_string
      t_string<TAG, N, OVERFLOW_GROW>::operator=(R_block block) noexcept {
    maybe_adjust_(get(block.max));
    impl_.assign(store_.get(), max_, block);
    return *this;
  }

  template<class TAG, t_n_ N>
  inline
  typename t_string<TAG, N, OVERFLOW_GROW>::r_string
      t_string<TAG, N, OVERFLOW_GROW>::operator=(R_crange range) noexcept {
    maybe_adjust_(get(range.n));
    impl_.assign(store_.get(), max_, begin(range), get(range.n));
    return *this;
  }

  template<class TAG, t_n_ N>
  inline
  typename t_string<TAG, N, OVERFLOW_GROW>::r_string
      t_string<TAG, N, OVERFLOW_GROW>::operator=(R_string str) noexcept {
    maybe_adjust_(get(str.get_length()));
    impl_.assign(store_.get(), max_, get(str.get_cstr()));
    return *this;
  }

  template<class TAG, t_n_ N>
  template<t_n_ N1>
  inline
  typename t_string<TAG, N, OVERFLOW_GROW>::r_string
    t_string<TAG, N, OVERFLOW_GROW>
      ::operator=(const t_char (&str)[N1]) noexcept {
    maybe_adjust_(N1-1);
    impl_.assign(store_.get(), max_, str);
    return *this;
  }

  template<class TAG, t_n_ N>
  template<t_n_ N1, t_overflow O1>
  inline
  typename t_string<TAG, N, OVERFLOW_GROW>::r_string
    t_string<TAG, N, OVERFLOW_GROW>
      ::operator=(const t_string<TAG, N1, O1>& str) noexcept {
    maybe_adjust_(get(str.get_length()));
    impl_.assign(store_.get(), max_, get(str.get_cstr()));
    return *this;
  }

  template<class TAG, t_n_ N>
  template<t_overflow O1>
  inline
  typename t_string<TAG, N, OVERFLOW_GROW>::r_string
    t_string<TAG, N, OVERFLOW_GROW>
      ::operator=(t_string<TAG, 0, O1>&& str) noexcept {
    dealloc_(store_.release());
    impl_.reset(str.impl_.reset());
    max_   = utility::reset(str.max_);
    store_ = utility::x_cast(str.store_);
    return *this;
  }

  template<class TAG, t_n_ N>
  inline
  typename t_string<TAG, N, OVERFLOW_GROW>::r_string
    t_string<TAG, N, OVERFLOW_GROW>
      ::assign(t_fmt, P_cstr_ fmt, ...) noexcept {
    va_list vars;
    va_start(vars, fmt);
    va_assign(fmt, vars);
    va_end(vars);
    return *this;
  }

  template<class TAG, t_n_ N>
  template<class TAG1, t_n_ N1, t_overflow O1>
  inline
  typename t_string<TAG, N, OVERFLOW_GROW>::r_string
    t_string<TAG, N, OVERFLOW_GROW>
      ::assign(const t_string<TAG1, N1, O1>& str) noexcept {
    maybe_adjust_(get(str.get_length()));
    impl_.assign(store_.get(), max_, get(str.get_cstr()));
    return *this;
  }

  template<class TAG, t_n_ N>
  inline
  typename t_string<TAG, N, OVERFLOW_GROW>::r_string
      t_string<TAG, N, OVERFLOW_GROW>::append(P_cstr str) noexcept {
    maybe_readjust_(length_(get(str)));
    impl_.append(store_.get(), max_, get(str));
    return *this;
  }

  template<class TAG, t_n_ N>
  inline
  typename t_string<TAG, N, OVERFLOW_GROW>::r_string
      t_string<TAG, N, OVERFLOW_GROW>::append(R_block block) noexcept {
    maybe_readjust_(get(block.max));
    impl_.append(store_.get(), max_, block);
    return *this;
  }

  template<class TAG, t_n_ N>
  inline
  typename t_string<TAG, N, OVERFLOW_GROW>::r_string
      t_string<TAG, N, OVERFLOW_GROW>::append(R_crange range) noexcept {
    maybe_readjust_(get(range.n));
    impl_.append(store_.get(), max_, begin(range), get(range.n));
    return *this;
  }

  template<class TAG, t_n_ N>
  inline
  typename t_string<TAG, N, OVERFLOW_GROW>::r_string
    t_string<TAG, N, OVERFLOW_GROW>
      ::append(t_fmt, P_cstr_ fmt, ...) noexcept {
    va_list vars;
    va_start(vars, fmt);
    va_append(fmt, vars);
    va_end(vars);
    return *this;
  }

  template<class TAG, t_n_ N>
  template<t_n_ N1>
  inline
  typename t_string<TAG, N, OVERFLOW_GROW>::r_string
    t_string<TAG, N, OVERFLOW_GROW>
      ::append(const t_char (&str)[N1]) noexcept {
    maybe_readjust_(N1-1);
    impl_.append(store_.get(), max_, str);
    return *this;
  }

  template<class TAG, t_n_ N>
  template<class TAG1, t_n_ N1, t_overflow O1>
  inline
  typename t_string<TAG, N, OVERFLOW_GROW>::r_string
    t_string<TAG, N, OVERFLOW_GROW>
      ::append(const t_string<TAG1, N1, O1>& str) noexcept {
    maybe_readjust_(get(str.get_length()));
    impl_.append(store_.get(), max_, get(str.get_cstr()));
    return *this;
  }

  template<class TAG, t_n_ N>
  inline
  typename t_string<TAG, N, OVERFLOW_GROW>::r_string
    t_string<TAG, N, OVERFLOW_GROW>
      ::va_assign(P_cstr_ fmt, va_list vars) noexcept {
    maybe_adjust_(length_(fmt, vars));
    impl_.va_assign(store_.get(), max_, fmt, vars);
    return *this;
  }

  template<class TAG, t_n_ N>
  inline
  typename t_string<TAG, N, OVERFLOW_GROW>::r_string
    t_string<TAG, N, OVERFLOW_GROW>
      ::va_append(P_cstr_ fmt, va_list vars) noexcept {
    maybe_readjust_(length_(fmt, vars));
    impl_.va_append(store_.get(), max_, fmt, vars);
    return *this;
  }

  template<class TAG, t_n_ N>
  inline
  t_void t_string<TAG, N, OVERFLOW_GROW>::display() const noexcept {
    impl_.display(store_.get());
  }

  template<class TAG, t_n_ N>
  inline
  t_void t_string<TAG, N, OVERFLOW_GROW>::display_then_clear() noexcept {
    impl_.display_then_clear(store_.get());
  }

  template<class TAG, t_n_ N>
  inline
  t_bool t_string<TAG, N, OVERFLOW_GROW>
      ::is_match(P_cstr pattern) const noexcept {
    return impl_.is_match(store_.get(), get(pattern));
  }

  template<class TAG, t_n_ N>
  template<t_n_ N1>
  inline
  t_bool t_string<TAG, N, OVERFLOW_GROW>
      ::is_match(const t_char (&pattern)[N1]) const noexcept {
    return impl_.is_match(store_.get(), pattern);
  }

  template<class TAG, t_n_ N>
  template<class TAG1, t_n_ N1, t_overflow O1>
  inline
  t_bool t_string<TAG, N, OVERFLOW_GROW>
      ::is_match(const t_string<TAG1, N1, O1>& pattern) const noexcept {
    return impl_.is_match(store_.get(), get(pattern.get_cstr()));
  }

  template<class TAG, t_n_ N>
  inline
  t_void t_string<TAG, N, OVERFLOW_GROW>::clear() noexcept {
    return impl_.clear(store_.get());
  }

  template<class TAG, t_n_ N>
  inline
  P_cstr t_string<TAG, N, OVERFLOW_GROW>::get_cstr() const noexcept {
    return P_cstr{impl_.get_cstr(store_.get())};
  }

  template<class TAG, t_n_ N>
  inline
  t_n t_string<TAG, N, OVERFLOW_GROW>::get_length() const noexcept {
    return t_n{impl_.get_length()};
  }

  template<class TAG, t_n_ N>
  inline
  t_n t_string<TAG, N, OVERFLOW_GROW>::get_capacity() const noexcept {
    return t_n{max_ ? max_ - 1 : 0};
  }

  template<class TAG, t_n_ N>
  inline
  t_bool t_string<TAG, N, OVERFLOW_GROW>::is_empty() const noexcept {
    return impl_.is_empty();
  }

  template<class TAG, t_n_ N>
  inline
  t_n t_string<TAG, N, OVERFLOW_GROW>::get_count(t_char c) const noexcept {
    return t_n{impl_.get_count(store_.get(), c)};
  }

  template<class TAG, t_n_ N>
  inline
  typename t_string<TAG, N, OVERFLOW_GROW>::t_char
      t_string<TAG, N, OVERFLOW_GROW>::get_front() const noexcept {
    return impl_.get_front(store_.get());
  }

  template<class TAG, t_n_ N>
  inline
  typename t_string<TAG, N, OVERFLOW_GROW>::t_char
      t_string<TAG, N, OVERFLOW_GROW>::get_back() const noexcept {
    return impl_.get_back(store_.get());
  }

  template<class TAG, t_n_ N>
  inline
  t_crange t_string<TAG, N, OVERFLOW_GROW>::mk_range() const noexcept {
    return impl_.mk_range(store_.get());
  }

  template<class TAG, t_n_ N>
  inline
  t_crange t_string<TAG, N, OVERFLOW_GROW>
      ::mk_range(t_ix begin) const noexcept {
    return impl_.mk_range(store_.get(), begin);
  }

  template<class TAG, t_n_ N>
  inline
  t_crange t_string<TAG, N, OVERFLOW_GROW>
      ::mk_range(t_ix begin, t_ix end) const noexcept {
    return impl_.mk_range(store_.get(), begin, end);
  }

  template<class TAG, t_n_ N>
  template<class F>
  inline
  t_void t_string<TAG, N, OVERFLOW_GROW>::each(F f) noexcept {
    impl_.each(store_.get(), f);
  }

  template<class TAG, t_n_ N>
  template<class F>
  inline
  t_void t_string<TAG, N, OVERFLOW_GROW>::each(F f) const noexcept {
    impl_.each(store_.get(), f);
  }

  template<class TAG, t_n_ N>
  template<class F>
  inline
  t_void t_string<TAG, N, OVERFLOW_GROW>::ceach(F f) const noexcept {
    impl_.each(store_.get(), f);
  }

  template<class TAG, t_n_ N>
  inline
  t_void t_string<TAG, N, OVERFLOW_GROW>::mod_(t_ix pos, t_char ch) noexcept {
    impl_.mod_(store_.get(), get(pos), ch);
  }

///////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
