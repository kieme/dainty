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

#ifndef _DAINTY_NAMED_STRING_4_H_
#define _DAINTY_NAMED_STRING_4_H_

#include "dainty_named_utility.h"
#include "dainty_named_string_1.h"

namespace dainty
{
namespace named
{
namespace string
{
////////////////////////////////////////////////////////////////////////////////

  template<class TAG>
  class t_string<TAG, 0, OVERFLOW_GROW> {
    using t_impl_ = t_string_impl_<OVERFLOW_ASSERT>;
  public:
    using t_n      = named::t_n;
    using P_cstr   = named::P_cstr;
    using R_crange = t_prefix<t_crange>::R_;
    using r_string = typename t_prefix<t_string>::r_;
    using R_string = typename t_prefix<t_string>::R_;
    using t_char   = typename t_impl_::t_char;
    using R_block  = typename t_impl_::R_block;

    t_string(t_n max = t_n{64}, t_n blks = t_n{1});
    t_string(P_cstr);
    t_string(R_block);
    t_string(R_crange);
    t_string(t_fmt, P_cstr_, ...) __attribute__((format(printf, 3, 4)));
    t_string(R_string);

    template<t_overflow O1>
    t_string(t_string<TAG, 0, O1>&&);

    template<t_n_ N1>
    t_string(const t_char (&)[N1]);
    template<t_n_ N1, t_overflow O1>
    t_string(const t_string<TAG, N1, O1>&);

    r_string operator=(P_cstr);
    r_string operator=(R_block);
    r_string operator=(R_crange);
    r_string operator=(R_string);
    template<t_n_ N1>
    r_string operator=(const t_char (&)[N1]);
    template<t_n_ N1, t_overflow O1>
    r_string operator=(const t_string<TAG, N1, O1>&);
    template<t_overflow O1>
    r_string operator=(t_string<TAG, 0, O1>&&);

    r_string assign(t_fmt, P_cstr_, ...) __attribute__((format(printf, 3, 4)));
    template<class TAG1, t_n_ N1, t_overflow O1>
    r_string assign(const t_string<TAG1, N1, O1>&);

    r_string append(P_cstr);
    r_string append(R_block);
    r_string append(R_crange);
    r_string append(t_fmt, P_cstr_, ...) __attribute__((format(printf, 3, 4)));
    template<t_n_ N1>
    r_string append(const t_char (&)[N1]);
    template<class TAG1, t_n_ N1, t_overflow O1>
    r_string append(const t_string<TAG1, N1, O1>&);

    r_string va_assign(P_cstr_ fmt, va_list vars);
    r_string va_append(P_cstr_ fmt, va_list vars);

    t_void clear();

    t_void display() const;
    t_void display_then_clear();

    t_bool is_match(P_cstr pattern) const;
    template<t_n_ N1>
    t_bool is_match(const t_char (&pattern)[N1]) const;
    template<class TAG1, t_n_ N1, t_overflow O1>
    t_bool is_match(const t_string<TAG1, N1, O1>& pattern) const;

    P_cstr get_cstr    () const;
    t_n    get_length  () const;
    t_n    get_capacity() const;
    t_bool is_empty    () const;
    t_n    get_count   (t_char) const;
    t_char get_front   () const;
    t_char get_back    () const;

    t_crange mk_range()           const;
    t_crange mk_range(t_ix)       const;
    t_crange mk_range(t_ix, t_ix) const;

    template<class F> void  each(F);
    template<class F> void  each(F) const;
    template<class F> void ceach(F) const;

    t_void mod_(t_ix pos, t_char);

  private:
    template<class, t_n_, t_overflow> friend class t_string;
    t_void maybe_adjust_  (t_n_);
    t_void maybe_readjust_(t_n_);

    t_n_    blks_ = 0;
    t_n_    max_  = 0;
    t_ptr_  store_;
    t_impl_ impl_;
  };

///////////////////////////////////////////////////////////////////////////////

  template<class TAG>
  inline
  t_string<TAG, 0, OVERFLOW_GROW>::t_string(t_n max, t_n blks)
    : blks_{get(blks)}, max_{calc_n_(get(max), blks_)}, store_{alloc_(max_)},
      impl_{store_.get()} {
  }

  template<class TAG>
  inline
  t_string<TAG, 0, OVERFLOW_GROW>::t_string(P_cstr str)
    : blks_{0}, max_{calc_n_(length_(get(str)), blks_)}, store_{alloc_(max_)},
      impl_{store_.get(), max_, get(str)} {
  }

  template<class TAG>
  inline
  t_string<TAG, 0, OVERFLOW_GROW>::t_string(R_block block)
    : blks_{0}, max_{calc_n_(get(block.max), blks_)}, store_{alloc_(max_)},
      impl_{store_.get(), max_, block} {
  }

  template<class TAG>
  inline
  t_string<TAG, 0, OVERFLOW_GROW>::t_string(R_crange range)
    : blks_{0}, max_{calc_n_(get(range.n), blks_)}, store_{alloc_(max_)},
      impl_{store_.get(), max_, begin(range), get(range.n)} {
  }

  template<class TAG>
  inline
  t_string<TAG, 0, OVERFLOW_GROW>::t_string(t_fmt, P_cstr_ fmt, ...) {
    va_list vars;
    va_start(vars, fmt);
    va_assign(fmt, vars);
    va_end(vars);
  }

  template<class TAG>
  inline
  t_string<TAG, 0, OVERFLOW_GROW>::t_string(R_string str)
    : blks_{0}, max_{calc_n_(get(str.get_length()), blks_)},
      store_{alloc_(max_)}, impl_{store_.get(), max_, get(str.get_cstr())} {
  }

  template<class TAG>
  template<t_n_ N1>
  inline
  t_string<TAG, 0, OVERFLOW_GROW>::t_string(const t_char (&str)[N1])
    : blks_{0}, max_{calc_n_(N1-1, blks_)}, store_{alloc_(max_)},
      impl_{store_.get(), max_, str} {
  }

  template<class TAG>
  template<t_n_ N1, t_overflow O1>
  inline
  t_string<TAG, 0, OVERFLOW_GROW>::t_string(const t_string<TAG, N1, O1>& str)
    : blks_{0}, max_{calc_n_(get(str.get_length()), blks_)},
      store_{alloc_(max_)}, impl_{store_.get(), max_, get(str.get_cstr())} {
  }

  template<class TAG>
  template<t_overflow O1>
  inline
  t_string<TAG, 0, OVERFLOW_GROW>::t_string(t_string<TAG, 0, O1>&& str)
    : blks_{str.blks_}, max_{utility::reset(str.max_)},
      store_{utility::x_cast(str.store_)}, impl_{str.impl_.reset()} {
  }

  template<class TAG>
  inline
  t_void t_string<TAG, 0, OVERFLOW_GROW>::maybe_adjust_(t_n_ need) {
    if (need > max_) {
      if (store_ == VALID)
        dealloc_(store_.release());
      max_   = calc_n_(need, blks_);
      store_ = alloc_ (max_);
    }
  }

  template<class TAG>
  inline
  t_void t_string<TAG, 0, OVERFLOW_GROW>::maybe_readjust_(t_n_ need) {
    auto len = impl_.get_length(), left = max_ - len;
    if (need > left) {
      max_   = calc_n_ (len + need, blks_);
      store_ = realloc_(store_.release(), max_);
    }
  }

  template<class TAG>
  inline
  typename t_string<TAG, 0, OVERFLOW_GROW>::r_string
      t_string<TAG, 0, OVERFLOW_GROW>::operator=(P_cstr str) {
    maybe_adjust_(length_(get(str)));
    impl_.assign(store_.get(), max_, get(str));
    return *this;
  }

  template<class TAG>
  inline
  typename t_string<TAG, 0, OVERFLOW_GROW>::r_string
      t_string<TAG, 0, OVERFLOW_GROW>::operator=(R_block block) {
    maybe_adjust_(get(block.max));
    impl_.assign(store_.get(), max_, block);
    return *this;
  }

  template<class TAG>
  inline
  typename t_string<TAG, 0, OVERFLOW_GROW>::r_string
      t_string<TAG, 0, OVERFLOW_GROW>::operator=(R_crange range) {
    maybe_adjust_(get(range.n));
    impl_.assign(store_.get(), max_, begin(range), get(range.n));
    return *this;
  }

  template<class TAG>
  inline
  typename t_string<TAG, 0, OVERFLOW_GROW>::r_string
      t_string<TAG, 0, OVERFLOW_GROW>::operator=(R_string str) {
    maybe_adjust_(get(str.get_length()));
    impl_.assign(store_.get(), max_, get(str.get_cstr()));
    return *this;
  }

  template<class TAG>
  template<t_n_ N1>
  inline
  typename t_string<TAG, 0, OVERFLOW_GROW>::r_string
      t_string<TAG, 0, OVERFLOW_GROW>::operator=(const t_char (&str)[N1]) {
    maybe_adjust_(N1-1);
    impl_.assign(store_.get(), max_, str);
    return *this;
  }

  template<class TAG>
  template<t_n_ N1, t_overflow O1>
  inline
  typename t_string<TAG, 0, OVERFLOW_GROW>::r_string
    t_string<TAG, 0, OVERFLOW_GROW>
      ::operator=(const t_string<TAG, N1, O1>& str) {
    maybe_adjust_(get(str.get_length()));
    impl_.assign(store_.get(), max_, get(str.get_cstr()));
    return *this;
  }

  template<class TAG>
  template<t_overflow O1>
  inline
  typename t_string<TAG, 0, OVERFLOW_GROW>::r_string
      t_string<TAG, 0, OVERFLOW_GROW>::operator=(t_string<TAG, 0, O1>&& str) {
    dealloc_(store_.release());
    impl_.reset(str.impl_.reset());
    max_   = utility::reset(str.max_);
    blks_  = str.blks_;
    store_ = utility::x_cast(str.store_);
    return *this;
  }

  template<class TAG>
  inline
  typename t_string<TAG, 0, OVERFLOW_GROW>::r_string
      t_string<TAG, 0, OVERFLOW_GROW>::assign(t_fmt, P_cstr_ fmt, ...) {
    va_list vars;
    va_start(vars, fmt);
    va_assign(fmt, vars);
    va_end(vars);
    return *this;
  }

  template<class TAG>
  template<class TAG1, t_n_ N1, t_overflow O1>
  inline
  typename t_string<TAG, 0, OVERFLOW_GROW>::r_string
    t_string<TAG, 0, OVERFLOW_GROW>
      ::assign(const t_string<TAG1, N1, O1>& str) {
    maybe_adjust_(get(str.get_length()));
    impl_.assign(store_.get(), max_, get(str.get_cstr()));
    return *this;
  }

  template<class TAG>
  inline
  typename t_string<TAG, 0, OVERFLOW_GROW>::r_string
      t_string<TAG, 0, OVERFLOW_GROW>::append(P_cstr str) {
    maybe_readjust_(length_(get(str)));
    impl_.append(store_.get(), max_, get(str));
    return *this;
  }

  template<class TAG>
  inline
  typename t_string<TAG, 0, OVERFLOW_GROW>::r_string
      t_string<TAG, 0, OVERFLOW_GROW>::append(R_block block) {
    maybe_readjust_(get(block.max));
    impl_.append(store_.get(), max_, block);
    return *this;
  }

  template<class TAG>
  inline
  typename t_string<TAG, 0, OVERFLOW_GROW>::r_string
      t_string<TAG, 0, OVERFLOW_GROW>::append(R_crange range) {
    maybe_readjust_(get(range.n));
    impl_.append(store_.get(), max_, begin(range), get(range.n));
    return *this;
  }

  template<class TAG>
  inline
  typename t_string<TAG, 0, OVERFLOW_GROW>::r_string
      t_string<TAG, 0, OVERFLOW_GROW>::append(t_fmt, P_cstr_ fmt, ...) {
    va_list vars;
    va_start(vars, fmt);
    va_append(fmt, vars);
    va_end(vars);
    return *this;
  }

  template<class TAG>
  template<t_n_ N1>
  inline
  typename t_string<TAG, 0, OVERFLOW_GROW>::r_string
      t_string<TAG, 0, OVERFLOW_GROW>::append(const t_char (&str)[N1]) {
    maybe_readjust_(N1-1);
    impl_.append(store_.get(), max_, str);
    return *this;
  }

  template<class TAG>
  template<class TAG1, t_n_ N1, t_overflow O1>
  inline
  typename t_string<TAG, 0, OVERFLOW_GROW>::r_string
    t_string<TAG, 0, OVERFLOW_GROW>
      ::append(const t_string<TAG1, N1, O1>& str) {
    maybe_readjust_(get(str.get_length()));
    impl_.append(store_.get(), max_, get(str.get_cstr()));
    return *this;
  }

  template<class TAG>
  inline
  typename t_string<TAG, 0, OVERFLOW_GROW>::r_string
      t_string<TAG, 0, OVERFLOW_GROW>::va_assign(P_cstr_ fmt, va_list vars) {
    maybe_adjust_(length_(fmt, vars));
    impl_.va_assign(store_.get(), max_, fmt, vars);
    return *this;
  }

  template<class TAG>
  inline
  typename t_string<TAG, 0, OVERFLOW_GROW>::r_string
      t_string<TAG, 0, OVERFLOW_GROW>::va_append(P_cstr_ fmt, va_list vars) {
    maybe_readjust_(length_(fmt, vars));
    impl_.va_append(store_.get(), max_, fmt, vars);
    return *this;
  }

  template<class TAG>
  inline
  t_void t_string<TAG, 0, OVERFLOW_GROW>::display() const {
    impl_.display(store_.get());
  }

  template<class TAG>
  inline
  t_void t_string<TAG, 0, OVERFLOW_GROW>::display_then_clear() {
    impl_.display_then_clear(store_.get());
  }

  template<class TAG>
  inline
  t_bool t_string<TAG, 0, OVERFLOW_GROW>::is_match(P_cstr pattern) const {
    return impl_.is_match(store_.get(), get(pattern));
  }

  template<class TAG>
  template<t_n_ N1>
  inline
  t_bool t_string<TAG, 0, OVERFLOW_GROW>
      ::is_match(const t_char (&pattern)[N1]) const {
    return impl_.is_match(store_.get(), pattern);
  }

  template<class TAG>
  template<class TAG1, t_n_ N1, t_overflow O1>
  inline
  t_bool t_string<TAG, 0, OVERFLOW_GROW>
      ::is_match(const t_string<TAG1, N1, O1>& pattern) const {
    return impl_.is_match(store_.get(), get(pattern.get_cstr()));
  }

  template<class TAG>
  inline
  t_void t_string<TAG, 0, OVERFLOW_GROW>::clear() {
    return impl_.clear(store_.get());
  }

  template<class TAG>
  inline
  P_cstr t_string<TAG, 0, OVERFLOW_GROW>::get_cstr() const {
    return P_cstr{impl_.get_cstr(store_.get())};
  }

  template<class TAG>
  inline
  t_n t_string<TAG, 0, OVERFLOW_GROW>::get_length() const {
    return t_n{impl_.get_length()};
  }

  template<class TAG>
  inline
  t_n t_string<TAG, 0, OVERFLOW_GROW>::get_capacity() const {
    return t_n{max_ ? max_ - 1 : 0};
  }

  template<class TAG>
  inline
  t_bool t_string<TAG, 0, OVERFLOW_GROW>::is_empty() const {
    return impl_.is_empty();
  }

  template<class TAG>
  inline
  t_n t_string<TAG, 0, OVERFLOW_GROW>::get_count(t_char c) const {
    return t_n{impl_.get_count(store_.get(), c)};
  }

  template<class TAG>
  inline
  typename t_string<TAG, 0, OVERFLOW_GROW>::t_char
      t_string<TAG, 0, OVERFLOW_GROW>::get_front() const {
    return impl_.get_front(store_.get());
  }

  template<class TAG>
  inline
  typename t_string<TAG, 0, OVERFLOW_GROW>::t_char
      t_string<TAG, 0, OVERFLOW_GROW>::get_back() const {
    return impl_.get_back(store_.get());
  }

  template<class TAG>
  inline
  t_crange t_string<TAG, 0, OVERFLOW_GROW>::mk_range() const {
    return impl_.mk_range(store_.get());
  }

  template<class TAG>
  inline
  t_crange t_string<TAG, 0, OVERFLOW_GROW>::mk_range(t_ix begin) const {
    return impl_.mk_range(store_.get(), begin);
  }

  template<class TAG>
  inline
  t_crange t_string<TAG, 0, OVERFLOW_GROW>::mk_range(t_ix begin,
                                                     t_ix end) const {
    return impl_.mk_range(store_.get(), begin, end);
  }

  template<class TAG>
  template<class F>
  inline
  t_void t_string<TAG, 0, OVERFLOW_GROW>::each(F f) {
    impl_.each(store_.get(), f);
  }

  template<class TAG>
  template<class F>
  inline
  t_void t_string<TAG, 0, OVERFLOW_GROW>::each(F f) const {
    impl_.each(store_.get(), f);
  }

  template<class TAG>
  template<class F>
  inline
  t_void t_string<TAG, 0, OVERFLOW_GROW>::ceach(F f) const {
    impl_.each(store_.get(), f);
  }

  template<class TAG>
  inline
  t_void t_string<TAG, 0, OVERFLOW_GROW>::mod_(t_ix pos, t_char ch) {
    impl_.mod_(store_.get(), get(pos), ch);
  }

///////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
