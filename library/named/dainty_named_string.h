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

#ifndef _DAINTY_NAMED_STRING_H_
#define _DAINTY_NAMED_STRING_H_

#include "dainty_named_string_impl.h"

namespace dainty
{
namespace named
{
namespace string
{
////////////////////////////////////////////////////////////////////////////////

  enum t_fmt { FMT };

////////////////////////////////////////////////////////////////////////////////

  template<class TAG, t_n_ N = 0, class I = t_overflow_assert>
  class t_string {
    using t_impl_ = t_string_impl_<I>;
  public:
    using t_n     = named::t_n;
    using P_cstr  = named::P_cstr;
    using t_char  = typename t_impl_::t_char;
    using R_block = typename t_impl_::R_block;

    t_string();
    t_string(P_cstr);
    t_string(R_block);
    t_string(t_fmt, P_cstr_, ...) __attribute__((format(printf, 3, 4)));
    t_string(const t_string&);

    template<t_n_ N1>
    t_string(const t_char (&)[N1]);
    template<t_n_ N1, class I1>
    t_string(const t_string<TAG, N1, I1>&);

    t_string& operator=(P_cstr);
    t_string& operator=(R_block);
    t_string& operator=(const t_string& str);
    template<t_n_ N1>
    t_string& operator=(const t_char (&)[N1]);
    template<t_n_ N1, class I1>
    t_string& operator=(const t_string<TAG, N1, I1>&);

    t_string& assign(t_fmt, P_cstr_, ...) __attribute__((format(printf, 3, 4)));
    template<class TAG1, t_n_ N1, class I1>
    t_string& assign(const t_string<TAG1, N1, I1>&);

    t_string& append(P_cstr);
    t_string& append(R_block);
    t_string& append(t_fmt, P_cstr_, ...) __attribute__((format(printf, 3, 4)));
    template<t_n_ N1> t_string& append(const t_char (&)[N1]);
    template<class TAG1, t_n_ N1, class I1>
    t_string& append(const t_string<TAG1, N1, I1>&);

    t_string& va_assign(P_cstr_ fmt, va_list vars);
    t_string& va_append(P_cstr_ fmt, va_list vars);

    t_void display() const;
    t_void display_then_clear();

    t_bool match(P_cstr pattern) const;
    template<t_n_ N1>
    t_bool match(const t_char (&pattern)[N1]) const;
    template<class TAG1, t_n_ N1, class I1>
    t_bool match(const t_string<TAG1, N1, I1>& pattern) const;

    t_void clear();
    P_cstr c_str () const;
    t_n    get_length  () const;
    t_n    get_capacity() const;
    t_n    count       (t_char) const;

    t_char front() const;
    t_char back () const;

    template<class F> void  each(F);
    template<class F> void  each(F) const;
    template<class F> void ceach(F) const;

  private:
    template<class, t_n_, class> friend class t_string;
    t_char  store_[N+1];
    t_impl_ impl_;
  };

///////////////////////////////////////////////////////////////////////////////

  template<class TAG, class I>
  class t_string<TAG, 0, I> {
    using t_impl_ = t_string_impl_<I>;
  public:
    using t_n     = named::t_n;
    using P_cstr  = named::P_cstr;
    using t_char  = typename t_impl_::t_char;
    using R_block = typename t_impl_::R_block;

     t_string(t_n max, t_n blks = t_n{0});
     t_string(P_cstr);
     t_string(R_block);
     t_string(t_fmt, P_cstr_, ...) __attribute__((format(printf, 3, 4)));
     t_string(const t_string&);

     template<class I1>
     t_string(t_string<TAG, 0, I1>&&);

     template<t_n_ N1>
     t_string(const t_char (&)[N1]);
     template<t_n_ N1, class I1>
     t_string(const t_string<TAG, N1, I1>&);
    ~t_string();

    t_string& operator=(P_cstr);
    t_string& operator=(R_block);
    t_string& operator=(const t_string& str);
    template<t_n_ N1>
    t_string& operator=(const t_char (&)[N1]);
    template<t_n_ N1, class I1>
    t_string& operator=(const t_string<TAG, N1, I1>&);
    template<class I1>
    t_string& operator=(t_string<TAG, 0, I1>&&);

    t_string& assign(t_fmt, P_cstr_, ...) __attribute__((format(printf, 3, 4)));
    template<class TAG1, t_n_ N1, class I1>
    t_string& assign(const t_string<TAG1, N1, I1>&);

    t_string& append(P_cstr);
    t_string& append(R_block);
    t_string& append(t_fmt, P_cstr_, ...) __attribute__((format(printf, 3, 4)));
    template<t_n_ N1> t_string& append(const t_char (&)[N1]);
    template<class TAG1, t_n_ N1, class I1>
    t_string& append(const t_string<TAG1, N1, I1>&);

    t_string& va_assign(P_cstr_ fmt, va_list vars);
    t_string& va_append(P_cstr_ fmt, va_list vars);

    t_void display() const;
    t_void display_then_clear();

    t_bool match(P_cstr pattern) const;
    template<t_n_ N1>
    t_bool match(const t_char (&pattern)[N1]) const;
    template<class TAG1, t_n_ N1, class I1>
    t_bool match(const t_string<TAG1, N1, I1>& pattern) const;

    t_void clear();
    P_cstr c_str () const;
    t_n    get_length  () const;
    t_n    get_capacity() const;
    t_n    count       (t_char) const;

    t_char front() const;
    t_char back () const;

    template<class F> void  each(F);
    template<class F> void  each(F) const;
    template<class F> void ceach(F) const;

  private:
    template<class, t_n_, class> friend class t_string;
    t_void  adjust_(t_n_);
    t_void readjust_(t_n_);

    t_n_    max_   = 0;
    t_n_    blks_  = 0;
    p_cstr_ store_ = nullptr;
    t_impl_ impl_;
  };

///////////////////////////////////////////////////////////////////////////////

  template<class TAG, t_n_ N, class I>
  inline
  t_string<TAG, N, I>::t_string() : impl_{store_} {
  }

  template<class TAG, t_n_ N, class I>
  inline
  t_string<TAG, N, I>::t_string(P_cstr str) : impl_{store_, N, get(str)} {
  }

  template<class TAG, t_n_ N, class I>
  inline
  t_string<TAG, N, I>::t_string(R_block block) : impl_{store_, N, block} {
  }

  template<class TAG, t_n_ N, class I>
  inline
  t_string<TAG, N, I>::t_string(t_fmt, P_cstr_ fmt, ...) : impl_{0U} {
    va_list vars;
    va_start(vars, fmt);
    impl_.va_assign(store_, N, fmt, vars);
    va_end(vars);
  }

  template<class TAG, t_n_ N, class I>
  inline
  t_string<TAG, N, I>::t_string(const t_string& str)
    : impl_{store_, N, get(str.c_str())} {
  }

  template<class TAG, t_n_ N, class I>
  template<t_n_ N1>
  inline
  t_string<TAG, N, I>::t_string(const t_char (&str)[N1])
    : impl_{store_, N, str} {
  }

  template<class TAG, t_n_ N, class I>
  template<t_n_ N1, class I1>
  inline
  t_string<TAG, N, I>::t_string(const t_string<TAG, N1, I1>& str)
    : impl_{store_, N, get(str.c_str())} {
  }

  template<class TAG, t_n_ N, class I>
  inline
  t_string<TAG, N, I>& t_string<TAG, N, I>::operator=(P_cstr str) {
    impl_.assign(store_, N, get(str));
    return *this;
  }

  template<class TAG, t_n_ N, class I>
  inline
  t_string<TAG, N, I>& t_string<TAG, N, I>::operator=(R_block block) {
    impl_.assign(store_, N, block);
    return *this;
  }

  template<class TAG, t_n_ N, class I>
  inline
  t_string<TAG, N, I>& t_string<TAG, N, I>::operator=(const t_string& str) {
    impl_.assign(store_, N, get(str.c_str()));
    return *this;
  }

  template<class TAG, t_n_ N, class I>
  template<t_n_ N1>
  inline
  t_string<TAG, N, I>&
      t_string<TAG, N, I>::operator=(const t_char (&str)[N1]) {
    impl_.assign(store_, N, str);
    return *this;
  }

  template<class TAG, t_n_ N, class I>
  template<t_n_ N1, class I1>
  inline
  t_string<TAG, N, I>&
      t_string<TAG, N, I>::operator=(const t_string<TAG, N1, I1>& str) {
    impl_.assign(store_, N, get(str.c_str()));
    return *this;
  }

  template<class TAG, t_n_ N, class I>
  inline
  t_string<TAG, N, I>& t_string<TAG, N, I>::assign(t_fmt, P_cstr_ fmt, ...) {
    va_list vars;
    va_start(vars, fmt);
    impl_.va_assign(store_, N, fmt, vars);
    va_end(vars);
    return *this;
  }

  template<class TAG, t_n_ N, class I>
  template<class TAG1, t_n_ N1, class I1>
  inline
  t_string<TAG, N, I>&
      t_string<TAG, N, I>::assign(const t_string<TAG1, N1, I1>& str) {
    impl_.assign(store_, N, get(str.c_str()));
    return *this;
  }

  template<class TAG, t_n_ N, class I>
  inline
  t_string<TAG, N, I>& t_string<TAG, N, I>::append(P_cstr str) {
    impl_.append(store_, N, get(str));
    return *this;
  }

  template<class TAG, t_n_ N, class I>
  inline
  t_string<TAG, N, I>& t_string<TAG, N, I>::append(R_block block) {
    impl_.append(store_, N, block);
    return *this;
  }

  template<class TAG, t_n_ N, class I>
  inline
  t_string<TAG, N, I>& t_string<TAG, N, I>::append(t_fmt, P_cstr_ fmt, ...) {
    va_list vars;
    va_start(vars, fmt);
    impl_.va_append(store_, N, fmt, vars);
    va_end(vars);
    return *this;
  }

  template<class TAG, t_n_ N, class I>
  template<t_n_ N1>
  inline
  t_string<TAG, N, I>& t_string<TAG, N, I>::append(const t_char (&str)[N1]) {
    impl_.append(store_, N, str);
    return *this;
  }

  template<class TAG,  t_n_ N,  class I>
  template<class TAG1, t_n_ N1, class I1>
  inline
  t_string<TAG, N, I>&
      t_string<TAG, N, I>::append(const t_string<TAG1, N1, I1>& str) {
    impl_.append(store_, N, get(str.c_str()));
    return *this;
  }

  template<class TAG, t_n_ N, class I>
  inline
  t_string<TAG, N, I>& t_string<TAG, N, I>::va_assign(P_cstr_ fmt,
                                                      va_list vars) {
    impl_.va_assign(store_, N, fmt, vars);
    return *this;
  }

  template<class TAG, t_n_ N, class I>
  inline
  t_string<TAG, N, I>& t_string<TAG, N, I>::va_append(P_cstr_ fmt,
                                                      va_list vars) {
    impl_.va_append(store_, N, fmt, vars);
    return *this;
  }

  template<class TAG, t_n_ N, class I>
  inline
  t_void t_string<TAG, N, I>::display() const {
    impl_.display(store_);
  }

  template<class TAG, t_n_ N, class I>
  inline
  t_void t_string<TAG, N, I>::display_then_clear() {
    impl_.display_then_clear(store_);
  }

  template<class TAG, t_n_ N, class I>
  inline
  t_bool t_string<TAG, N, I>::match(P_cstr pattern) const {
    return impl_.match(store_, get(pattern));
  }

  template<class TAG, t_n_ N, class I>
  template<t_n_ N1>
  inline
  t_bool t_string<TAG, N, I>::match(const t_char (&pattern)[N1]) const {
    return impl_.match(store_, pattern);
  }

  template<class TAG,  t_n_ N,  class I>
  template<class TAG1, t_n_ N1, class I1>
  inline
  t_bool
      t_string<TAG, N, I>::match(const t_string<TAG1, N1, I1>& pattern) const {
    return impl_.match(store_, get(pattern.c_str()));
  }

  template<class TAG, t_n_ N, class I>
  inline
  t_void t_string<TAG, N, I>::clear() {
    return impl_.clear(store_);
  }

  template<class TAG, t_n_ N, class I>
  inline
  P_cstr t_string<TAG, N, I>::c_str() const {
    return P_cstr{impl_.c_str(store_)};
  }

  template<class TAG, t_n_ N, class I>
  inline
  t_n t_string<TAG, N, I>::get_length() const {
    return t_n{impl_.get_length()};
  }

  template<class TAG, t_n_ N, class I>
  inline
  t_n t_string<TAG, N, I>::get_capacity() const {
    return t_n{N};
  }

  template<class TAG, t_n_ N, class I>
  inline
  t_n t_string<TAG, N, I>::count(t_char c) const {
    return t_n{impl_.count(store_, c)};
  }

  template<class TAG, t_n_ N, class I>
  inline
  typename t_string<TAG, N, I>::t_char t_string<TAG, N, I>::front() const {
    return impl_.front(store_);
  }

  template<class TAG, t_n_ N, class I>
  inline
  typename t_string<TAG, N, I>::t_char t_string<TAG, N, I>::back() const {
    return impl_.back(store_);
  }

  template<class TAG, t_n_ N, class I>
  template<class F>
  inline
  t_void t_string<TAG, N, I>::each(F f) {
    impl_.each(store_, f);
  }

  template<class TAG, t_n_ N, class I>
  template<class F>
  inline
  t_void t_string<TAG, N, I>::each(F f) const {
    impl_.each(store_, f);
  }

  template<class TAG, t_n_ N, class I>
  template<class F>
  inline
  t_void t_string<TAG, N, I>::ceach(F f) const {
    impl_.each(store_, f);
  }

///////////////////////////////////////////////////////////////////////////////

  template<class TAG, class I>
  inline
  t_string<TAG, 0, I>::t_string(t_n max, t_n blks)
    : max_{get(max)}, blks_{get(blks)}, store_{alloc_(max_)}, impl_{store_} {
  }

  template<class TAG, class I>
  inline
  t_string<TAG, 0, I>::t_string(P_cstr str)
    : max_{length_(get(str))}, blks_{0}, store_{alloc_(max_)},
      impl_{store_, max_, get(str)} {
  }

  template<class TAG, class I>
  inline
  t_string<TAG, 0, I>::t_string(R_block block)
    : max_{get(block.max)}, blks_{0}, store_{alloc_(max_)},
      impl_{store_, max_, block} {
  }

  template<class TAG, class I>
  inline
  t_string<TAG, 0, I>::t_string(t_fmt, P_cstr_ fmt, ...) : impl_{0U} {
    va_list vars;
    va_start(vars, fmt);
    va_assign(fmt, vars);
    va_end(vars);
  }

  template<class TAG, class I>
  inline
  t_string<TAG, 0, I>::t_string(const t_string& str)
    : max_{get(str.get_length())}, blks_{0}, store_{alloc_(max_)},
      impl_{store_, max_, get(str.c_str())} {
  }

  template<class TAG, class I>
  template<t_n_ N1>
  inline
  t_string<TAG, 0, I>::t_string(const t_char (&str)[N1])
    : max_{N1-1}, blks_{0}, store_{alloc_(max_)}, impl_{store_, max_, str} {
  }

  template<class TAG, class I>
  template<t_n_ N1, class I1>
  inline
  t_string<TAG, 0, I>::t_string(const t_string<TAG, N1, I1>& str)
    : max_{get(str.get_length())}, blks_{0}, store_{alloc_(max_)},
      impl_{store_, max_, get(str.c_str())} {
  }

  template<class TAG, class I>
  template<class I1>
  inline
  t_string<TAG, 0, I>::t_string(t_string<TAG, 0, I1>&& str)
      : max_{reset(str.max_)}, blks_{reset(str.blks_)},
        store_{reset(str.store_)}, impl_ {reset(str.impl_.len_)} {
  }

  template<class TAG, class I>
  inline
  t_string<TAG, 0, I>::~t_string() {
    if (store_)
      dealloc_(store_);
  }

  template<class TAG, class I>
  inline
  t_void t_string<TAG, 0, I>::adjust_(t_n_ need) {
    if (need > max_) {
      if (store_)
        dealloc_(store_);
      max_   = calc_chr_(need, blks_);
      store_ = alloc_(max_);
    }
  }

  template<class TAG, class I>
  inline
  t_void t_string<TAG, 0, I>::readjust_(t_n_ need) {
    auto len = impl_.get_length(), left = max_ - len;
    if (need > left) {
      max_   = calc_chr_(len + need, blks_);
      store_ = realloc_ (store_, max_);
    }
  }

  template<class TAG, class I>
  inline
  t_string<TAG, 0, I>& t_string<TAG, 0, I>::operator=(P_cstr str) {
    adjust_(length_(get(str)));
    impl_.assign(store_, max_, get(str));
    return *this;
  }

  template<class TAG, class I>
  inline
  t_string<TAG, 0, I>& t_string<TAG, 0, I>::operator=(R_block block) {
    adjust_(get(block.max));
    impl_.assign(store_, max_, block);
    return *this;
  }

  template<class TAG, class I>
  inline
  t_string<TAG, 0, I>& t_string<TAG, 0, I>::operator=(const t_string& str) {
    adjust_(get(str.get_length()));
    impl_.assign(store_, max_, get(str.c_str()));
    return *this;
  }

  template<class TAG, class I>
  template<t_n_ N1>
  inline
  t_string<TAG, 0, I>&
      t_string<TAG, 0, I>::operator=(const t_char (&str)[N1]) {
    adjust_(N1-1);
    impl_.assign(store_, max_, str);
    return *this;
  }

  template<class TAG, class I>
  template<t_n_ N1, class I1>
  inline
  t_string<TAG, 0, I>&
      t_string<TAG, 0, I>::operator=(const t_string<TAG, N1, I1>& str) {
    adjust_(get(str.get_length()));
    impl_.assign(store_, max_, get(str.c_str()));
    return *this;
  }

  template<class TAG, class I>
  template<class I1>
  inline
  t_string<TAG, 0, I>&
      t_string<TAG, 0, I>::operator=(t_string<TAG, 0, I1>&& str) {
    dealloc_(store_);
    max_       = reset(str.max_);
    blks_      = reset(str.blks_);
    store_     = reset(str.store_);
    impl_.len_ = reset(str.impl_.len_);
    return *this;
  }

  template<class TAG, class I>
  inline
  t_string<TAG, 0, I>& t_string<TAG, 0, I>::assign(t_fmt, P_cstr_ fmt, ...) {
    va_list vars;
    va_start(vars, fmt);
    va_assign(fmt, vars);
    va_end(vars);
    return *this;
  }

  template<class TAG, class I>
  template<class TAG1, t_n_ N1, class I1>
  inline
  t_string<TAG, 0, I>&
      t_string<TAG, 0, I>::assign(const t_string<TAG1, N1, I1>& str) {
    adjust_(get(str.get_length()));
    impl_.assign(store_, max_, get(str.c_str()));
    return *this;
  }

  template<class TAG, class I>
  inline
  t_string<TAG, 0, I>& t_string<TAG, 0, I>::append(P_cstr str) {
    readjust_(length_(get(str)));
    impl_.append(store_, max_, get(str));
    return *this;
  }

  template<class TAG, class I>
  inline
  t_string<TAG, 0, I>& t_string<TAG, 0, I>::append(R_block block) {
    readjust_(get(block.max));
    impl_.append(store_, max_, block);
    return *this;
  }

  template<class TAG, class I>
  inline
  t_string<TAG, 0, I>& t_string<TAG, 0, I>::append(t_fmt, P_cstr_ fmt, ...) {
    va_list vars;
    va_start(vars, fmt);
    va_append(fmt, vars);
    va_end(vars);
    return *this;
  }

  template<class TAG, class I>
  template<t_n_ N1>
  inline
  t_string<TAG, 0, I>& t_string<TAG, 0, I>::append(const t_char (&str)[N1]) {
    readjust_(N1-1);
    impl_.append(store_, max_, str);
    return *this;
  }

  template<class TAG,   class I>
  template<class TAG1, t_n_ N1, class I1>
  inline
  t_string<TAG, 0, I>&
      t_string<TAG, 0, I>::append(const t_string<TAG1, N1, I1>& str) {
    readjust_(get(str.get_length()));
    impl_.append(store_, max_, get(str.c_str()));
    return *this;
  }

  template<class TAG, class I>
  inline
  t_string<TAG, 0, I>& t_string<TAG, 0, I>::va_assign(P_cstr_ fmt,
                                                      va_list vars) {
#if 1
  adjust_(length_(fmt, vars));
  impl_.va_assign(store_, max_, fmt, vars);
#else
    t_n_ need = impl_.va_assign_(store_, max_, fmt, vars);
    if (need) {
      adjust_(need);
      impl_.va_assign(store_, max_, fmt, vars);
    }
#endif
    return *this;
  }

  template<class TAG, class I>
  inline
  t_string<TAG, 0, I>& t_string<TAG, 0, I>::va_append(P_cstr_ fmt,
                                                      va_list vars) {
#if 2
  readjust_(length_(fmt, vars));
  impl_.va_append(store_, max_, fmt, vars);
#else
    t_n_ need = impl_.va_append_(store_, max_, fmt, vars);
    if (need) {
      readjust_(need);
      impl_.va_append(store_, max_, fmt, vars);
    }
#endif
    return *this;
  }

  template<class TAG, class I>
  inline
  t_void t_string<TAG, 0, I>::display() const {
    impl_.display(store_);
  }

  template<class TAG, class I>
  inline
  t_void t_string<TAG, 0, I>::display_then_clear() {
    impl_.display_then_clear(store_);
  }

  template<class TAG, class I>
  inline
  t_bool t_string<TAG, 0, I>::match(P_cstr pattern) const {
    return impl_.match(store_, get(pattern));
  }

  template<class TAG, class I>
  template<t_n_ N1>
  inline
  t_bool t_string<TAG, 0, I>::match(const t_char (&pattern)[N1]) const {
    return impl_.match(store_, pattern);
  }

  template<class TAG,  class I>
  template<class TAG1, t_n_ N1, class I1>
  inline
  t_bool
      t_string<TAG, 0, I>::match(const t_string<TAG1, N1, I1>& pattern) const {
    return impl_.match(store_, get(pattern.c_str()));
  }

  template<class TAG, class I>
  inline
  t_void t_string<TAG, 0, I>::clear() {
    return impl_.clear(store_);
  }

  template<class TAG, class I>
  inline
  P_cstr t_string<TAG, 0, I>::c_str() const {
    return P_cstr{impl_.c_str(store_)};
  }

  template<class TAG, class I>
  inline
  t_n t_string<TAG, 0, I>::get_length() const {
    return t_n{impl_.get_length()};
  }

  template<class TAG, class I>
  inline
  t_n t_string<TAG, 0, I>::get_capacity() const {
    return t_n{max_};
  }

  template<class TAG, class I>
  inline
  t_n t_string<TAG, 0, I>::count(t_char c) const {
    return t_n{impl_.count(store_, c)};
  }

  template<class TAG, class I>
  inline
  typename t_string<TAG, 0, I>::t_char t_string<TAG, 0, I>::front() const {
    return impl_.front(store_);
  }

  template<class TAG, class I>
  inline
  typename t_string<TAG, 0, I>::t_char t_string<TAG, 0, I>::back() const {
    return impl_.back(store_);
  }

  template<class TAG, class I>
  template<class F>
  inline
  t_void t_string<TAG, 0, I>::each(F f) {
    impl_.each(store_, f);
  }

  template<class TAG, class I>
  template<class F>
  inline
  t_void t_string<TAG, 0, I>::each(F f) const {
    impl_.each(store_, f);
  }

  template<class TAG, class I>
  template<class F>
  inline
  t_void t_string<TAG, 0, I>::ceach(F f) const {
    impl_.each(store_, f);
  }

///////////////////////////////////////////////////////////////////////////////

  template<class TAG, t_n_ N, t_n_ N1, class I, class I1>
  inline
  t_bool operator<(const t_string<TAG, N, I>&   lh,
                   const t_string<TAG, N1, I1>& rh) {
    return compare_(get(lh.c_str()), get(rh.c_str())) < 0;
  }

  template<class TAG, t_n_ N, t_n_ N1, class I, class I1>
  inline
  t_bool operator>(const t_string<TAG, N,  I>&  lh,
                   const t_string<TAG, N1, I1>& rh) {
    return compare_(get(lh.c_str()), get(rh.c_str())) > 0;
  }

////////////////////////////////////////////////////////////////////////////////

  template<class TAG, t_n_ N, class I>
  inline
  t_bool operator==(const t_string<TAG, N, I>& lh, P_cstr rh) {
    return compare_(get(lh.c_str()), get(rh)) == 0;
  }

  template<class TAG, t_n_ N, class I>
  inline
  t_bool operator!=(const t_string<TAG, N, I>& lh, P_cstr rh) {
    return !(lh == rh);
  }

////////////////////////////////////////////////////////////////////////////////

  template<class TAG, t_n_ N, class I>
  inline
  t_bool operator==(P_cstr lh, const t_string<TAG, N, I>& rh) {
    return rh == lh;
  }

  template<class TAG, t_n_ N, class I>
  inline
  t_bool operator!=(P_cstr lh, const t_string<TAG, N, I>& rh) {
    return !(lh == rh);
  }

////////////////////////////////////////////////////////////////////////////////

  template<class TAG, t_n_ N, t_n_ N1, class I, class I1>
  inline
  t_bool operator==(const t_string<TAG, N,  I>&  lh,
                    const t_string<TAG, N1, I1>& rh) {
    return get(lh.get_length()) == get(rh.get_length()) &&
           lh == rh.c_str();
  }

  template<class TAG, t_n_ N, t_n_ N1, class I, class I1>
  inline
  t_bool operator!=(const t_string<TAG, N,  I>&  lh,
                    const t_string<TAG, N1, I1>& rh) {
    return !(lh == rh);
  }

////////////////////////////////////////////////////////////////////////////////

  template<class TAG, t_n_ N, class I>
  inline
  t_string<TAG, N, I>& operator+=(t_string<TAG, N, I>& lh, P_cstr rh) {
    return lh.append(rh);
  }

  template<class TAG, class TAG1, t_n_ N, t_n_ N1, class I, class I1>
  inline
  t_string<TAG, N, I>& operator+=(t_string<TAG, N, I>& lh,
                                  const t_string<TAG1, N1, I1>& rh) {
    return lh.append(rh);
  }

  template<class TAG, t_n_ N, class I, class T>
  inline
  t_string<TAG, N, I>& operator+=(t_string<TAG, N, I>& lh, const T& rh) {
    return lh.append(rh);
  }

////////////////////////////////////////////////////////////////////////////////

  template<class TAG, t_n_ N, class I>
  inline
  t_string<TAG, N, I>& operator,(t_string<TAG, N, I>& lh, P_cstr rh) {
    return (lh +=rh);
  }

  template<class TAG, class TAG1, t_n_ N, t_n_ N1, class I, class I1>
  inline
  t_string<TAG, N, I>& operator,(t_string<TAG, N, I>& lh,
                                 const t_string<TAG1, N1, I1>& rh) {
    return (lh +=rh);
  }

  template<class TAG, t_n_ N, class I, class T>
  inline
  t_string<TAG, N, I>& operator,(t_string<TAG, N, I>& lh, const T& rh) {
    return (lh += rh);
  }

////////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
