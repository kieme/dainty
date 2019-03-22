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

  template<class TAG, t_overflow O>
  class t_string<TAG, 0, O> {
    using t_impl_ = t_string_impl_<O>;
  public:
    using t_n      = named::t_n;
    using P_cstr   = named::P_cstr;
    using R_crange = t_prefix<t_crange>::R_;
    using r_string = typename t_prefix<t_string>::r_;
    using R_string = typename t_prefix<t_string>::R_;
    using t_char   = typename t_impl_::t_char;
    using R_block  = typename t_impl_::R_block;

    t_string(t_n max = t_n{64});
    t_string(P_cstr);
    t_string(R_block);
    t_string(R_crange);
    t_string(t_fmt, P_cstr_, ...) __attribute__((format(printf, 3, 4)));
    t_string(R_string);

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

    t_n    get_capacity();
    t_n    get_count   (t_char) const;
    P_cstr get_cstr    () const;
    t_n    get_length  () const;
    t_bool is_empty    () const;
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
    t_n_    max_  = 0;
    t_ptr_  store_;
    t_impl_ impl_;
  };

///////////////////////////////////////////////////////////////////////////////

  /// <- below must be fixed

  template<class TAG, t_overflow O>
  inline
  t_string<TAG, 0, O>::t_string(t_n max)
    : max_{get(max)}, store_{alloc_(max_)}, impl_{store_} {
  }

  template<class TAG, t_overflow O>
  inline
  t_string<TAG, 0, O>::t_string(P_cstr str)
    : max_{length_(get(str))+1}, store_{alloc_(max_)},
      impl_{store_, max_+1, str} {
  }

  template<class TAG, t_overflow O>
  inline
  t_string<TAG, 0, O>::t_string(R_block block) : impl_{store_, max_+1, block} {
  }

  template<class TAG, t_overflow O>
  inline
  t_string<TAG, 0, O>::t_string(R_crange range)
    : impl_{store_, max_+1, begin(range), get(range.n)} {
  }

  template<class TAG, t_overflow O>
  inline
  t_string<TAG, 0, O>::t_string(t_fmt, P_cstr_ fmt, ...) {
    va_list vars;
    va_start(vars, fmt);
    impl_.va_assign(store_, max_+1, fmt, vars);
    va_end(vars);
  }

  template<class TAG, t_overflow O>
  inline
  t_string<TAG, 0, O>::t_string(R_string str)
    : impl_{store_, max_+1, get(str.get_cstr())} {
  }

  template<class TAG, t_overflow O>
  template<t_n_ N1>
  inline
  t_string<TAG, 0, O>::t_string(const t_char (&str)[N1])
    : impl_{store_, max_+1, str} {
  }

  template<class TAG, t_overflow O>
  template<t_n_ N1, t_overflow O1>
  inline
  t_string<TAG, 0, O>::t_string(const t_string<TAG, N1, O1>& str)
    : impl_{store_, max_+1, get(str.get_cstr())} {
  }

  /// <- abive must be fixed
  template<class TAG, t_overflow O>
  inline
  typename t_string<TAG, 0, O>::r_string
      t_string<TAG, 0, O>::operator=(P_cstr str) {
    impl_.assign(store_, max_+1, get(str));
    return *this;
  }

  template<class TAG, t_overflow O>
  inline
  typename t_string<TAG, 0, O>::r_string
      t_string<TAG, 0, O>::operator=(R_block block) {
    impl_.assign(store_, max_+1, block);
    return *this;
  }

  template<class TAG, t_overflow O>
  inline
  typename t_string<TAG, 0, O>::r_string
      t_string<TAG, 0, O>::operator=(R_crange range) {
    impl_.assign(store_, max_+1, begin(range), get(range.n));
    return *this;
  }

  template<class TAG, t_overflow O>
  inline
  typename t_string<TAG, 0, O>::r_string
      t_string<TAG, 0, O>::operator=(R_string str) {
    impl_.assign(store_, max_+1, get(str.get_cstr()));
    return *this;
  }

  template<class TAG, t_overflow O>
  template<t_n_ N1>
  inline
  typename t_string<TAG, 0, O>::r_string
      t_string<TAG, 0, O>::operator=(const t_char (&str)[N1]) {
    impl_.assign(store_, max_+1, str);
    return *this;
  }

  template<class TAG, t_overflow O>
  template<t_n_ N1, t_overflow O1>
  inline
  typename t_string<TAG, 0, O>::r_string
      t_string<TAG, 0, O>::operator=(const t_string<TAG, N1, O1>& str) {
    impl_.assign(store_, max_+1, get(str.get_cstr()));
    return *this;
  }

  template<class TAG, t_overflow O>
  inline
  typename t_string<TAG, 0, O>::r_string
      t_string<TAG, 0, O>::assign(t_fmt, P_cstr_ fmt, ...) {
    va_list vars;
    va_start(vars, fmt);
    impl_.va_assign(store_, max_+1, fmt, vars);
    va_end(vars);
    return *this;
  }

  template<class TAG, t_overflow O>
  template<class TAG1, t_n_ N1, t_overflow O1>
  inline
  typename t_string<TAG, 0, O>::r_string
      t_string<TAG, 0, O>::assign(const t_string<TAG1, N1, O1>& str) {
    impl_.assign(store_, N1, get(str.get_cstr()));
    return *this;
  }

  template<class TAG, t_overflow O>
  inline
  typename t_string<TAG, 0, O>::r_string
      t_string<TAG, 0, O>::append(P_cstr str) {
    impl_.append(store_, max_+1, get(str));
    return *this;
  }

  template<class TAG, t_overflow O>
  inline
  typename t_string<TAG, 0, O>::r_string
      t_string<TAG, 0, O>::append(R_block block) {
    impl_.append(store_, max_+1, block);
    return *this;
  }

  template<class TAG, t_overflow O>
  inline
  typename t_string<TAG, 0, O>::r_string
      t_string<TAG, 0, O>::append(R_crange range) {
    impl_.append(store_, max_+1, begin(range), get(range.n));
    return *this;
  }

  template<class TAG, t_overflow O>
  inline
  typename t_string<TAG, 0, O>::r_string
      t_string<TAG, 0, O>::append(t_fmt, P_cstr_ fmt, ...) {
    va_list vars;
    va_start(vars, fmt);
    impl_.va_append(store_, max_+1, fmt, vars);
    va_end(vars);
    return *this;
  }

  template<class TAG, t_overflow O>
  template<t_n_ N1>
  inline
  typename t_string<TAG, 0, O>::r_string
      t_string<TAG, 0, O>::append(const t_char (&str)[N1]) {
    impl_.append(store_, max_+1, str);
    return *this;
  }

  template<class TAG, t_overflow O>
  template<class TAG1, t_n_ N1, t_overflow O1>
  inline
  typename t_string<TAG, 0, O>::r_string
      t_string<TAG, 0, O>::append(const t_string<TAG1, N1, O1>& str) {
    impl_.append(store_, max_+1, get(str.get_cstr()));
    return *this;
  }

  template<class TAG, t_overflow O>
  inline
  typename t_string<TAG, 0, O>::r_string
      t_string<TAG, 0, O>::va_assign(P_cstr_ fmt, va_list vars) {
    impl_.va_assign(store_, max_+1, fmt, vars);
    return *this;
  }

  template<class TAG, t_overflow O>
  inline
  typename t_string<TAG, 0, O>::r_string
      t_string<TAG, 0, O>::va_append(P_cstr_ fmt, va_list vars) {
    impl_.va_append(store_, max_+1, fmt, vars);
    return *this;
  }

  template<class TAG, t_overflow O>
  inline
  t_void t_string<TAG, 0, O>::display() const {
    impl_.display(store_);
  }

  template<class TAG, t_overflow O>
  inline
  t_void t_string<TAG, 0, O>::display_then_clear() {
    impl_.display_then_clear(store_);
  }

  template<class TAG, t_overflow O>
  inline
  t_bool t_string<TAG, 0, O>::is_match(P_cstr pattern) const {
    return impl_.is_match(store_, get(pattern));
  }

  template<class TAG, t_overflow O>
  template<t_n_ N1>
  inline
  t_bool t_string<TAG, 0, O>::is_match(const t_char (&pattern)[N1]) const {
    return impl_.is_match(store_, pattern);
  }

  template<class TAG, t_overflow O>
  template<class TAG1, t_n_ N1, t_overflow O1>
  inline
  t_bool t_string<TAG, 0, O>
      ::is_match(const t_string<TAG1, N1, O1>& pattern) const {
    return impl_.is_match(store_, get(pattern.get_cstr()));
  }

  template<class TAG, t_overflow O>
  inline
  t_void t_string<TAG, 0, O>::clear() {
    return impl_.clear(store_);
  }

  template<class TAG, t_overflow O>
  inline
  P_cstr t_string<TAG, 0, O>::get_cstr() const {
    return P_cstr{impl_.get_cstr(store_)};
  }

  template<class TAG, t_overflow O>
  inline
  t_n t_string<TAG, 0, O>::get_length() const {
    return t_n{impl_.get_length()};
  }

  template<class TAG, t_overflow O>
  t_n t_string<TAG, 0, O>::get_capacity() {
    return t_n{max_};
  }

  template<class TAG, t_overflow O>
  inline
  t_bool t_string<TAG, 0, O>::is_empty() const {
    return impl_.is_empty();
  }

  template<class TAG, t_overflow O>
  inline
  t_n t_string<TAG, 0, O>::get_count(t_char c) const {
    return t_n{impl_.get_count(store_, c)};
  }

  template<class TAG, t_overflow O>
  inline
  typename t_string<TAG, 0, O>::t_char t_string<TAG, 0, O>::get_front() const {
    return impl_.get_front(store_);
  }

  template<class TAG, t_overflow O>
  inline
  typename t_string<TAG, 0, O>::t_char t_string<TAG, 0, O>::get_back() const {
    return impl_.get_back(store_);
  }

  template<class TAG, t_overflow O>
  inline
  t_crange t_string<TAG, 0, O>::mk_range() const {
    return impl_.mk_range(store_);
  }

  template<class TAG, t_overflow O>
  inline
  t_crange t_string<TAG, 0, O>::mk_range(t_ix begin) const {
    return impl_.mk_range(store_, begin);
  }

  template<class TAG, t_overflow O>
  inline
  t_crange t_string<TAG, 0, O>::mk_range(t_ix begin, t_ix end) const {
    return impl_.mk_range(store_, begin, end);
  }

  template<class TAG, t_overflow O>
  template<class F>
  inline
  t_void t_string<TAG, 0, O>::each(F f) {
    impl_.each(store_, f);
  }

  template<class TAG, t_overflow O>
  template<class F>
  inline
  t_void t_string<TAG, 0, O>::each(F f) const {
    impl_.each(store_, f);
  }

  template<class TAG, t_overflow O>
  template<class F>
  inline
  t_void t_string<TAG, 0, O>::ceach(F f) const {
    impl_.each(store_, f);
  }

  template<class TAG, t_overflow O>
  inline
  t_void t_string<TAG, 0, O>::mod_(t_ix pos, t_char ch) {
    impl_.mod_(store_, get(pos), ch);
  }

///////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
