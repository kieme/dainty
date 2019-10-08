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

#ifndef _DAINTY_BASE_STRING_IMPL_H_
#define _DAINTY_BASE_STRING_IMPL_H_

#include <stdarg.h>
#include "dainty_base.h"
#include "dainty_base_range.h"
#include "dainty_base_buf.h"
#include "dainty_base_assert.h"

namespace dainty
{
namespace base
{
namespace string
{
namespace impl_
{
///////////////////////////////////////////////////////////////////////////////

  using types::t_prefix;
  using types::t_bool;
  using types::t_void;
  using types::t_char;
  using types::r_char;
  using types::t_uchar;
  using types::r_uchar;
  using types::t_schar;
  using types::r_schar;
  using types::t_short;
  using types::r_short;
  using types::t_ushort;
  using types::r_ushort;
  using types::t_int;
  using types::r_int;
  using types::t_uint;
  using types::r_uint;
  using types::t_long;
  using types::r_long;
  using types::t_ulong;
  using types::r_ulong;
  using types::t_llong;
  using types::r_llong;
  using types::t_ullong;
  using types::r_ullong;
  using types::t_uintptr;
  using types::p_cstr_;      // XXX
  using types::P_cstr_;      // XXX
  using types::t_cstr_cptr_; // XXX
  using types::t_n_;
  using types::t_ix_;

  using specific::operator""_ix;
  using specific::operator""_n;
  using specific::P_cstr;      // XXX
  using specific::t_cstr_cptr; // XXX
  using specific::t_n;
  using specific::r_n;
  using specific::t_ix;
  using specific::t_begin_ix;
  using specific::t_end_ix;
  using specific::t_validity;
  using specific::VALID;
  using specific::INVALID;

  using assertion::assert_now;

///////////////////////////////////////////////////////////////////////////////

  template<t_n_ N>
  using t_grow_buf   = buf::t_buf<t_char, N, buf::t_size_dynamic>;
  using t_buf_range  = buf::t_buf_range<t_char>;
  using t_buf_crange = buf::t_buf_crange<t_char>;

///////////////////////////////////////////////////////////////////////////////

  enum t_overflow_assert   { OVERFLOW_ASSERT   };
  enum t_overflow_truncate { OVERFLOW_TRUNCATE };
  enum t_overflow_grow     { OVERFLOW_GROW     };

  template<typename> class t_impl_;

///////////////////////////////////////////////////////////////////////////////

  enum t_plus1_     : t_n_   { NOT_PLUS1     = 0,     PLUS1     = 1    };
  enum t_incl_char_ : t_n_   { NOT_INCL_CHAR = 0,     INCL_CHAR = 1 };
  enum t_eol_       : t_bool { NOT_EOL_OK    = false, EOL_OK    = true };

  constexpr t_char EOL{'\0'};

///////////////////////////////////////////////////////////////////////////////

  using t_range_tag_ = range::t_cstr_range_tag_;
  using t_range      = range::t_cstr_range;
  using R_range      = range::R_cstr_range;
  using t_crange     = range::t_cstr_crange;
  using R_crange     = range::R_cstr_crange;

  constexpr t_crange NO_RANGE;

///////////////////////////////////////////////////////////////////////////////

  enum  t_block_tag_ { };
  using t_block_ = base::t_block<t_char, t_block_tag_>;

  struct t_block : t_block_ {
    constexpr
    t_block()                 noexcept : t_block_(EOL, 0_n) { }
    constexpr
    t_block(t_char ch, t_n n) noexcept : t_block_{ch, n}    { }
  };

///////////////////////////////////////////////////////////////////////////////

  struct t_char_select {
    template<t_n_ N>
    constexpr
    t_char_select(const t_char (&literal)[N]) noexcept
      : range{literal, t_n{N - 1}} {
    }

    constexpr
    t_char_select(R_crange _range) noexcept : range{_range} {
    }

    constexpr
    operator t_validity() const noexcept {
      return choice != EOL ? VALID : INVALID;
    }

    t_crange range;
    t_char   choice = EOL;
  };
  using r_char_select = t_prefix<t_char_select>::r_;
  using p_char_select = t_prefix<t_char_select>::p_;

////////////////////////////////////////////////////////////////////////////////

  class t_snippet;
  using r_snippet = t_prefix<t_snippet>::r_;
  using p_snippet = t_prefix<t_snippet>::p_;

  class t_snippet {
  public:
    constexpr
    t_snippet() noexcept = default;

    constexpr
    t_snippet(R_crange range) noexcept
      : ptr_{range.ptr}, n_{get(range.n)} {
    }

    constexpr
    operator t_crange() const noexcept {
      return t_crange{ptr_, t_n{n_}, range::SKIP_};
    }

    constexpr
    operator t_validity() const noexcept {
      return ptr_ && n_ ? VALID : INVALID;
    }

    constexpr
    r_snippet operator=(R_crange range) noexcept {
      ptr_ = range.ptr;
      n_   = get(range.n);
      return *this;
    }

  protected:
    P_cstr_ ptr_ = nullptr;
    t_n_    n_   = 0;
  };

///////////////////////////////////////////////////////////////////////////////

  t_void display_(t_crange)                     noexcept;
  t_void display_(t_crange, t_crange, t_crange) noexcept;

  t_n build_(t_buf_range, t_crange, va_list, t_overflow_grow)     noexcept;
  t_n build_(t_buf_range, t_crange, va_list, t_overflow_assert)   noexcept;
  t_n build_(t_buf_range, t_crange, va_list, t_overflow_truncate) noexcept;

  t_n copy_         (t_buf_range, t_crange, t_overflow_assert)   noexcept;
  t_n copy_         (t_buf_range, t_crange, t_overflow_truncate) noexcept;

  t_n fill_         (t_buf_range, t_block, t_overflow_assert)   noexcept;
  t_n fill_         (t_buf_range, t_block, t_overflow_truncate) noexcept;

  t_bool match_     (t_crange, t_crange) noexcept;
  t_n    count_     (t_crange, t_char)   noexcept;
  t_n    length_    (P_cstr)             noexcept;
  t_n    length_    (P_cstr,   va_list)  noexcept;

  t_bool equal_     (t_crange, t_crange)       noexcept;
  t_bool less_      (t_crange, t_crange)       noexcept;
  t_bool less_equal_(t_crange, t_crange)       noexcept;

  t_n shift_left_   (t_buf_range, t_n, t_n) noexcept;
  t_n shift_right_  (t_buf_range, t_n, t_n) noexcept;
  t_n shift_centre_ (t_buf_range, t_n, t_n) noexcept;

  t_ullong to_uint_    (r_n, t_char, t_char,         t_n, t_crange) noexcept;
  t_llong  to_sint_    (r_n, t_char, t_char, t_char, t_n, t_crange) noexcept;
  t_ullong hex_to_uint_(r_n,                         t_n, t_crange) noexcept;

  t_n uint_to_str_  (t_buf_range, t_ullong) noexcept;
  t_n int_to_str_   (t_buf_range, t_llong)  noexcept;
  t_n hex_to_str_   (t_buf_range, t_ullong) noexcept;

  t_n  scan_va_     (t_crange, t_n, t_crange, va_list) noexcept;
  t_n  scan_        (t_crange, t_n, t_crange, ...)     noexcept;

  t_n skip_         (t_crange, t_char)             noexcept;
  t_n skip_         (t_crange, t_n)                noexcept;
  t_n skip_         (t_crange, t_crange)           noexcept;
  t_n skip_         (t_crange, t_block)            noexcept;
  t_n skip_until_   (t_crange, t_char,   t_plus1_) noexcept;
  t_n skip_until_   (t_crange, t_crange, t_plus1_) noexcept;
  t_n skip_all_     (t_crange, t_char)             noexcept;

  t_n snip_n_       (t_crange, p_snippet, t_n)  noexcept;
  t_n snip_char_    (t_crange, p_snippet, t_char, t_plus1_,
                     t_incl_char_)              noexcept;
  t_n snip_char_eol_(t_crange, p_snippet, t_char, t_plus1_,
                     t_incl_char_)              noexcept;
  t_n snip_char_    (t_crange, p_snippet, p_char_select, t_plus1_,
                     t_incl_char_)              noexcept;
  t_n snip_char_eol_(t_crange, p_snippet, p_char_select, t_plus1_,
                     t_incl_char_)              noexcept;

////////////////////////////////////////////////////////////////////////////////

  inline
  t_crange mk_range(P_cstr str) noexcept { //XXX
    return t_crange{get(str), length_(str), range::SKIP_};
  }

  inline
  t_crange mk_range(t_cstr_cptr_ str) noexcept { // XXX
    return t_crange{str, length_(P_cstr(str)), range::SKIP_};
  }

  template<t_n_ N>
  constexpr
  t_crange mk_range(const t_char (&str)[N]) noexcept {
    return range::mk_crange<t_range_tag_>(str);
  }

  constexpr
  t_crange mk_range(R_crange range, t_begin_ix begin) noexcept {
    return range::mk_crange<t_range_tag_>(range, begin);
  }

  constexpr
  t_crange mk_range(R_crange range, t_begin_ix begin, t_end_ix end) noexcept {
    return range::mk_crange<t_range_tag_>(range, begin, end);
  }

////////////////////////////////////////////////////////////////////////////////

  class t_impl_base_ {
  public:
    t_impl_base_(t_n)         noexcept;
    t_impl_base_(t_buf_range) noexcept;

    t_void display           (t_buf_crange, t_crange, t_crange) const noexcept;
    t_void display_then_clear(t_buf_range,  t_crange, t_crange)       noexcept;

    t_bool   is_match  (t_buf_crange, t_crange) const noexcept;
    t_bool   is_empty  ()                       const noexcept;
    P_cstr_  get_cstr  (t_buf_crange)           const noexcept;
    t_n      get_length()                       const noexcept;
    t_n      get_count (t_buf_crange, t_char c) const noexcept;

    t_char   get_front (t_buf_crange) const noexcept;
    t_char   get_back  (t_buf_crange) const noexcept;

    t_crange mk_range  (t_buf_crange)                       const noexcept;
    t_crange mk_range  (t_buf_crange, t_begin_ix)           const noexcept;
    t_crange mk_range  (t_buf_crange, t_begin_ix, t_end_ix) const noexcept;

    t_bool   remove    (t_buf_range, t_begin_ix, t_end_ix) noexcept;
    t_void   mod_      (t_buf_range, t_ix,       t_char)   noexcept;
    t_n      reset     (t_n len = 0_n) noexcept;
    t_void   clear     (t_buf_range)   noexcept;

    t_n      scan      (t_buf_crange, t_n, t_crange, va_list) noexcept;

    template<class F>
    t_void each(t_buf_crange, F&&) const noexcept;

  protected:
    t_n_ len_ = 0;
  };

///////////////////////////////////////////////////////////////////////////////

  template<>
  class t_impl_<t_overflow_assert> : public t_impl_base_ {
  public:
    using base_ = t_impl_base_;
    using base_::base_;

    t_impl_(t_buf_range, t_crange) noexcept;
    t_impl_(t_buf_range, t_block)  noexcept;

    t_void assign(t_buf_range, t_crange)          noexcept;
    t_void assign(t_buf_range, t_block)           noexcept;
    t_void assign(t_buf_range, t_crange, va_list) noexcept;

    t_void append(t_buf_range, t_crange)          noexcept;
    t_void append(t_buf_range, t_block)           noexcept;
    t_void append(t_buf_range, t_crange, va_list) noexcept;

    template<typename F>
    t_void custom_assign(t_buf_range, F&&) noexcept;
    template<typename F>
    t_void custom_append(t_buf_range, F&&) noexcept;
  };

///////////////////////////////////////////////////////////////////////////////

  template<>
  class t_impl_<t_overflow_truncate> : public t_impl_base_ {
  public:
    using base_ = t_impl_base_;
    using base_::base_;

    t_impl_(t_buf_range, t_crange) noexcept;
    t_impl_(t_buf_range, t_block)  noexcept;

    t_void assign(t_buf_range, t_crange)          noexcept;
    t_void assign(t_buf_range, t_block)           noexcept;
    t_void assign(t_buf_range, t_crange, va_list) noexcept;

    t_void append(t_buf_range, t_crange)          noexcept;
    t_void append(t_buf_range, t_block)           noexcept;
    t_void append(t_buf_range, t_crange, va_list) noexcept;

    template<typename F>
    t_void custom_assign(t_buf_range, F&&) noexcept;
    template<typename F>
    t_void custom_append(t_buf_range, F&&) noexcept;
  };

///////////////////////////////////////////////////////////////////////////////

  template<>
  class t_impl_<t_overflow_grow> : public t_impl_base_ {
  public:
    using base_ = t_impl_base_;
    using base_::base_;

    template<t_n_ N>
    t_impl_(t_grow_buf<N>&, t_crange) noexcept;
    template<t_n_ N>
    t_impl_(t_grow_buf<N>&, t_block)  noexcept;

    template<t_n_ N>
    t_void assign(t_grow_buf<N>&, t_crange)          noexcept;
    template<t_n_ N>
    t_void assign(t_grow_buf<N>&, t_block)           noexcept;
    template<t_n_ N>
    t_void assign(t_grow_buf<N>&, t_crange, va_list) noexcept;

    template<t_n_ N>
    t_void append(t_grow_buf<N>&, t_crange)          noexcept;
    template<t_n_ N>
    t_void append(t_grow_buf<N>&, t_block)           noexcept;
    template<t_n_ N>
    t_void append(t_grow_buf<N>&, t_crange, va_list) noexcept;

    template<t_n_ N, typename F>
    t_void custom_assign(t_grow_buf<N>&, F&&) noexcept;
    template<t_n_ N, typename F>
    t_void custom_append(t_grow_buf<N>&, F&&) noexcept;
  };

///////////////////////////////////////////////////////////////////////////////

  inline
  t_impl_base_::t_impl_base_(t_n len) noexcept : len_{get(len)} {
  }

  inline
  t_impl_base_::t_impl_base_(t_buf_range store) noexcept : len_{0} {
    if (store == VALID)
      store[0_ix] = '\0';
  }

  inline
  t_void t_impl_base_::clear(t_buf_range store) noexcept {
    if (store == VALID)
      store[0_ix] = '\0';
    len_ = 0;
  }

  inline
  t_void t_impl_base_::display(t_buf_crange store, t_crange prefix,
                               t_crange postfix) const noexcept {
    if (len_)
      display_(mk_range(store), prefix, postfix);
  }

  inline
  t_void t_impl_base_::display_then_clear(t_buf_range store, t_crange prefix,
                                          t_crange postfix) noexcept {
    if (len_)
      display_(mk_range(store), prefix, postfix);
    clear(store);
  }

  inline
  t_bool t_impl_base_::is_match(t_buf_crange store,
                                t_crange     pattern) const noexcept {
    return match_(mk_range(store), pattern);
  }

  inline
  t_bool t_impl_base_::is_empty() const noexcept {
    return len_ == 0;
  }

  inline
  t_n t_impl_base_::get_length() const noexcept {
    return t_n{len_};
  }

  inline
  P_cstr_ t_impl_base_::get_cstr(t_buf_crange store) const noexcept {
    return store.ptr;
  }

  inline
  t_n t_impl_base_::reset(t_n len) noexcept {
    return t_n{base::reset(len_, get(len))};
  }

  inline
  t_n t_impl_base_::get_count(t_buf_crange store, t_char ch) const noexcept {
    return count_(mk_range(store), ch);
  }

  inline
  t_char t_impl_base_::get_front(t_buf_crange store) const noexcept {
    return len_ ? store[0_ix] : '\0';
  }

  inline
  t_char t_impl_base_::get_back(t_buf_crange store) const noexcept {
    return len_ ? store[t_ix{len_ - 1}] : '\0';
  }

  inline
  t_crange t_impl_base_::mk_range(t_buf_crange store) const noexcept {
    return store.mk_crange<t_range_tag_>(t_n{len_});
  }

  inline
  t_crange t_impl_base_::mk_range(t_buf_crange store,
                                  t_begin_ix begin) const noexcept {
    return mk_range(store).mk_crange(begin);
  }

  inline
  t_crange t_impl_base_::mk_range(t_buf_crange store, t_begin_ix begin,
                                  t_end_ix end) const noexcept {
    return mk_range(store).mk_crange(begin, end);
  }

  inline
  t_void t_impl_base_::mod_(t_buf_range store, t_ix _pos, t_char ch) noexcept {
    auto pos = get(_pos);
    if (pos < len_)
      store[_pos] = ch;
    else
      assertion::assert_now(P_cstr{"not in range"});
  }

  inline
  t_n t_impl_base_::scan(t_buf_crange store, t_n n, t_crange fmt,
                         va_list vars) noexcept {
    return scan_va_(mk_range(store), n, fmt, vars);
  }

  template<class F>
  inline
  t_void t_impl_base_::each(t_buf_crange store, F&& func) const noexcept {
    auto range = mk_range(store);
    for (auto ch : range)
      func(ch);
  }

///////////////////////////////////////////////////////////////////////////////

  inline
  t_impl_<t_overflow_assert>::t_impl_(t_buf_range store,
                                      t_crange range) noexcept
      : t_impl_base_{store} {
    assign(store, range);
  }

  inline
  t_impl_<t_overflow_assert>::t_impl_(t_buf_range store,
                                      t_block block) noexcept
      : t_impl_base_{store} {
    assign(store, block);
  }

  template<typename F>
  inline
  t_void t_impl_<t_overflow_assert>::custom_assign(t_buf_range store,
                                                   F&& func) noexcept {
  }

  template<typename F>
  inline
  t_void t_impl_<t_overflow_assert>::custom_append(t_buf_range store,
                                                   F&& func) noexcept {
  }

///////////////////////////////////////////////////////////////////////////////

  /*
  inline
  t_impl_<t_overflow_assert>::t_impl_(p_cstr_ str, t_n_ max,
                                      P_cstr_ src) noexcept
    : base_{copy_(str, max, src, length_(src), OVERFLOW_ASSERT)} {
  }

  inline
  t_impl_<t_overflow_assert>::t_impl_(p_cstr_ str, t_n_ max, P_cstr_ src, t_n_ len) noexcept
    : base_{copy_(str, max, src, len, OVERFLOW_ASSERT)} {
  }

  inline
  t_impl_<t_overflow_assert>::t_impl_(p_cstr_ str, t_n_ max, R_crange range) noexcept
    : base_{copy_(str, max, range.ptr, get(range.n), OVERFLOW_ASSERT)} {
  }

  inline
  t_impl_<t_overflow_assert>::t_impl_(p_cstr_ str, t_n_ max, R_block block) noexcept
    : base_{fill_(str, max, block, OVERFLOW_ASSERT)} {
  }

  inline
  t_void t_impl_<t_overflow_assert>::assign(p_cstr_ str, t_n_ max, P_cstr_ src) noexcept {
    len_ = copy_(str, max, src, length_(src), OVERFLOW_ASSERT);
  }

  inline
  t_void t_impl_<t_overflow_assert>::assign(p_cstr_ str, t_n_ max, P_cstr_ src,
                       t_n_ len) noexcept {
    len_ = copy_(str, max, src, len, OVERFLOW_ASSERT);
  }

  inline
  t_void t_impl_<t_overflow_assert>::assign(p_cstr_ str, t_n_ max, R_crange range) noexcept {
    len_ = copy_(str, max, range.ptr, get(range.n), OVERFLOW_ASSERT);
  }

  inline
  t_void t_impl_<t_overflow_assert>::assign(p_cstr_ str, t_n_ max, R_block block) noexcept {
    len_ = fill_(str, max, block, OVERFLOW_ASSERT);
  }

  inline
  t_void t_impl_<t_overflow_assert>::append(p_cstr_ str, t_n_ max, P_cstr_ src) noexcept {
    len_ += copy_(str + len_, max - len_, src, length_(src), OVERFLOW_ASSERT);
  }

  inline
  t_void t_impl_<t_overflow_assert>::append(p_cstr_ str, t_n_ max, P_cstr_ src,
                       t_n_ len) noexcept {
    len_ += copy_(str + len_, max - len_, src, len, OVERFLOW_ASSERT);
  }

  inline
  t_void t_impl_<t_overflow_assert>::append(p_cstr_ str, t_n_ max, R_crange range) noexcept {
    len_ += copy_(str + len_, max - len_, range.ptr, get(range.n),
                  OVERFLOW_ASSERT);
  }

  inline
  t_void t_impl_<t_overflow_assert>::append(p_cstr_ str, t_n_ max, R_block block) noexcept {
    len_ += fill_(str + len_, max - len_, block, OVERFLOW_ASSERT);
  }

  inline
  t_void t_impl_<t_overflow_assert>::assign(t_fmt_va, p_cstr_ str, t_n_ max, P_cstr_ fmt,
                       va_list vars) noexcept {
    len_ = build_(str, max, fmt, vars, OVERFLOW_ASSERT);
  }

  inline
  t_void t_impl_<t_overflow_assert>::append(t_fmt_va, p_cstr_ str, t_n_ max, P_cstr_ fmt,
                          va_list vars) noexcept {
    len_ += build_(str + len_, max - len_, fmt, vars, OVERFLOW_ASSERT);
  }

  template<typename F>
  inline
  t_void t_impl_<t_overflow_assert>::custom_assign(p_cstr_ str, t_n_ max, F& func) noexcept {
    auto n = func(str, max, OVERFLOW_ASSERT);
    if (n >= max)
      assertion::assert_now(P_cstr{"buffer overflow"});
    len_ = n;
  }

  template<typename F>
  inline
  t_void t_impl_<t_overflow_assert>::custom_append(p_cstr_ str, t_n_ max, F& func) noexcept {
    auto n = func(str + len_, max - len_, OVERFLOW_ASSERT);
    if (n + len_ >= max)
      assertion::assert_now(P_cstr{"buffer overflow"});
    len_ += n;
  }
  */

///////////////////////////////////////////////////////////////////////////////

  inline
  t_impl_<t_overflow_truncate>::t_impl_(t_buf_range store,
                                        t_crange range) noexcept
      : t_impl_base_{store} {
    assign(store, range);
  }

  inline
  t_impl_<t_overflow_truncate>::t_impl_(t_buf_range store,
                                        t_block block) noexcept
      : t_impl_base_{store} {
    assign(store, block);
  }

  template<typename F>
  inline
  t_void t_impl_<t_overflow_truncate>::custom_assign(t_buf_range store,
                                                     F&& func) noexcept {
  }

  template<typename F>
  inline
  t_void t_impl_<t_overflow_truncate>::custom_append(t_buf_range store,
                                                     F&& func) noexcept {
  }

///////////////////////////////////////////////////////////////////////////////

  /*
  template<>
  class t_impl_<t_overflow_truncate> : public t_impl_base_ {
  public:
    using base_ = t_impl_base_;

    using base_::base_;

    inline t_impl_(p_cstr_ str, t_n_ max, P_cstr_ src) noexcept
      : base_{copy_(str, max, src, length_(src), OVERFLOW_TRUNCATE)} {
    }

    inline t_impl_(p_cstr_ str, t_n_ max, P_cstr_ src, t_n_ len) noexcept
      : base_{copy_(str, max, src, len, OVERFLOW_TRUNCATE)} {
    }

    inline t_impl_(p_cstr_ str, t_n_ max, R_crange range) noexcept
      : base_{copy_(str, max, range.ptr, get(range.n), OVERFLOW_TRUNCATE)} {
    }

    inline t_impl_(p_cstr_ str, t_n_ max, R_block block) noexcept
      : base_{fill_(str, max, block, OVERFLOW_TRUNCATE)} {
    }

    inline t_void assign(p_cstr_ str, t_n_ max, P_cstr_ src) noexcept {
      len_ = copy_(str, max, src, length_(src), OVERFLOW_TRUNCATE);
    }

    inline t_void assign(p_cstr_ str, t_n_ max, P_cstr_ src,
                         t_n_ len) noexcept {
      len_ = copy_(str, max, src, len, OVERFLOW_TRUNCATE);
    }

    inline t_void assign(p_cstr_ str, t_n_ max, R_crange range) noexcept {
      len_ = copy_(str, max, range.ptr, get(range.n), OVERFLOW_TRUNCATE);
    }

    inline t_void assign(p_cstr_ str, t_n_ max, R_block block) noexcept {
      len_ = fill_(str, max, block, OVERFLOW_TRUNCATE);
    }

    inline t_void append(p_cstr_ str, t_n_ max, P_cstr_ src) noexcept {
      len_ += copy_(str + len_, max - len_, src, length_(src),
                    OVERFLOW_TRUNCATE);
    }

    inline t_void append(p_cstr_ str, t_n_ max, P_cstr_ src,
                         t_n_ len) noexcept {
      len_ += copy_(str + len_, max - len_, src, len, OVERFLOW_TRUNCATE);
    }

    inline t_void append(p_cstr_ str, t_n_ max, R_crange range) noexcept {
      len_ += copy_(str + len_, max - len_, range.ptr, get(range.n),
                    OVERFLOW_TRUNCATE);
    }

    inline t_void append(p_cstr_ str, t_n_ max, R_block block) noexcept {
      len_ += fill_(str + len_, max - len_, block, OVERFLOW_TRUNCATE);
    }

    inline t_void assign(t_fmt_va, p_cstr_ str, t_n_ max, P_cstr_ fmt,
                         va_list vars) noexcept {
      len_ = build_(str, max, fmt, vars, OVERFLOW_TRUNCATE);
    }

    inline t_void append(t_fmt_va, p_cstr_ str, t_n_ max, P_cstr_ fmt,
                         va_list vars) noexcept {
      len_ += build_(str + len_, max - len_, fmt, vars, OVERFLOW_TRUNCATE);
    }

    template<typename F>
    inline t_void custom_assign(p_cstr_ str, t_n_ max, F& func) noexcept {
      auto len = func(str, max, OVERFLOW_TRUNCATE);
      if (len < max)
        len_ = len;
      else {
        len_ = max - 1;
        str[len] = '\0';
      }
    }

    template<typename F>
    inline t_void custom_append(p_cstr_ str, t_n_ max, F& func) noexcept {
      auto len = func(str + len_, max - len_, OVERFLOW_TRUNCATE);
      if (len < max - len_)
        len_ += len;
      else {
        len_ = max - len_ - 1;
        str[len] = '\0';
      }
    }
  };
  */

///////////////////////////////////////////////////////////////////////////////

  template<t_n_ N>
  inline
  t_impl_<t_overflow_grow>::t_impl_(t_grow_buf<N>& store,
                                    t_crange range) noexcept
      : t_impl_base_{store} {
    assign(store, range);
  }

  template<t_n_ N>
  inline
  t_impl_<t_overflow_grow>::t_impl_(t_grow_buf<N>& store,
                                    t_block block) noexcept
      : t_impl_base_{store} {
    assign(store, block);
  }

  template<t_n_ N>
  inline
  t_void t_impl_<t_overflow_grow>::assign(t_grow_buf<N>&, t_crange) noexcept {
  }

  template<t_n_ N>
  inline
  t_void t_impl_<t_overflow_grow>::assign(t_grow_buf<N>&, t_block)  noexcept {
  }

  template<t_n_ N>
  inline
  t_void t_impl_<t_overflow_grow>::assign(t_grow_buf<N>&, t_crange, va_list) noexcept {
  }

  template<t_n_ N>
  inline
  t_void t_impl_<t_overflow_grow>::append(t_grow_buf<N>&, t_crange) noexcept {
  }

  template<t_n_ N>
  inline
  t_void t_impl_<t_overflow_grow>::append(t_grow_buf<N>&, t_block)  noexcept {
  }

  template<t_n_ N>
  inline
  t_void t_impl_<t_overflow_grow>::append(t_grow_buf<N>&, t_crange, va_list) noexcept {
  }

  template<t_n_ N, typename F>
  inline
  t_void t_impl_<t_overflow_grow>::custom_assign(t_grow_buf<N>&, F&&) noexcept {
  }

  template<t_n_ N, typename F>
  inline
  t_void t_impl_<t_overflow_grow>::custom_append(t_grow_buf<N>&, F&&) noexcept {
  }

///////////////////////////////////////////////////////////////////////////////

  /*
  template<>
  class t_impl_<t_overflow_grow> : public t_impl_base_ {
  public:
    using base_ = t_impl_base_;

    using base_::base_;

    inline t_impl_(p_cstr_ str, t_n_ max, P_cstr_ src) noexcept
      : base_{copy_(str, max, src, length_(src), OVERFLOW_TRUNCATE)} {
    }

    inline t_impl_(p_cstr_ str, t_n_ max, P_cstr_ src, t_n_ len) noexcept
      : base_{copy_(str, max, src, len, OVERFLOW_TRUNCATE)} {
    }

    inline t_impl_(p_cstr_ str, t_n_ max, R_crange range) noexcept
      : base_{copy_(str, max, range.ptr, get(range.n), OVERFLOW_TRUNCATE)} {
    }

    inline t_impl_(p_cstr_ str, t_n_ max, R_block block) noexcept
      : base_{fill_(str, max, block, OVERFLOW_TRUNCATE)} {
    }

    inline t_void assign(p_cstr_ str, t_n_ max, P_cstr_ src) noexcept {
      len_ = copy_(str, max, src, length_(src), OVERFLOW_TRUNCATE);
    }

    inline t_void assign(p_cstr_ str, t_n_ max, P_cstr_ src,
                         t_n_ len) noexcept {
      len_ = copy_(str, max, src, len, OVERFLOW_TRUNCATE);
    }

    inline t_void assign(p_cstr_ str, t_n_ max, R_crange range) noexcept {
      len_ = copy_(str, max, range.ptr, get(range.n), OVERFLOW_TRUNCATE);
    }

    inline t_void assign(p_cstr_ str, t_n_ max, R_block block) noexcept {
      len_ = fill_(str, max, block, OVERFLOW_TRUNCATE);
    }

    inline t_void append(p_cstr_ str, t_n_ max, P_cstr_ src) noexcept {
      len_ += copy_(str + len_, max - len_, src, length_(src),
                    OVERFLOW_TRUNCATE);
    }

    inline t_void append(p_cstr_ str, t_n_ max, P_cstr_ src,
                         t_n_ len) noexcept {
      len_ += copy_(str + len_, max - len_, src, len, OVERFLOW_TRUNCATE);
    }

    inline t_void append(p_cstr_ str, t_n_ max, R_crange range) noexcept {
      len_ += copy_(str + len_, max - len_, range.ptr, get(range.n),
                    OVERFLOW_TRUNCATE);
    }

    inline t_void append(p_cstr_ str, t_n_ max, R_block block) noexcept {
      len_ += fill_(str + len_, max - len_, block, OVERFLOW_TRUNCATE);
    }

    inline t_n_ assign_try(t_fmt_va, p_cstr_ str, t_n_ max, P_cstr_ fmt,
                           va_list vars) noexcept {
      auto n = build_(str, max, fmt, vars, OVERFLOW_GROW);
      if (n < max)
        len_ = n;
      return n;
    }

    inline t_n_ append_try(t_fmt_va, p_cstr_ str, t_n_ max, P_cstr_ fmt,
                           va_list vars) noexcept {
      auto left = max - len_;
      auto n = build_(str + len_, left, fmt, vars, OVERFLOW_GROW);
      if (n < left)
        len_ += n;
      return n;
    }

    inline t_void assign(t_fmt_va, p_cstr_ str, t_n_ max, P_cstr_ fmt,
                         va_list vars) noexcept {
      len_ = build_(str, max, fmt, vars, OVERFLOW_TRUNCATE);
    }

    inline t_void append(t_fmt_va, p_cstr_ str, t_n_ max, P_cstr_ fmt,
                         va_list vars) noexcept {
      len_ += build_(str + len_, max - len_, fmt, vars, OVERFLOW_TRUNCATE);
    }

    template<typename F>
    inline t_n_ custom_assign(p_cstr_ str, t_n_ max, F& func) noexcept {
      auto len = func(str, max, OVERFLOW_GROW);
      if (len < max)
        len_ = len;
      else
        str[len_] = '\0';
      return len;
    }

    template<typename F>
    inline t_n_ custom_append(p_cstr_ str, t_n_ max, F& func) noexcept {
      auto len = func(str + len_, max - len_, OVERFLOW_GROW);
      if (len < max - len_)
        len_ += len;
      else
        str[len_] = '\0';
      return len;
    }
  };
  */

///////////////////////////////////////////////////////////////////////////////

  inline
  t_n to_integer_(r_char value, t_crange str) {
    t_n use = 0_n;
    value = static_cast<t_char>(to_sint_(use, '1', '8', '7', 4_n, str));
    return use;
  }

  inline
  t_n to_integer_(r_schar value, t_crange str) {
    t_n use = 0_n;
    value = static_cast<t_schar>(to_sint_(use, '1', '8', '7', 4_n, str));
    return use;
  }

  inline t_n to_integer_(r_uchar value, t_crange str) {
    t_n use = 0_n;
    value = static_cast<t_uchar>(to_uint_(use, '2', '5', 3_n, str));
    return use;
  }

  inline
  t_n to_integer_(r_short value, t_crange str) {
    t_n use = 0_n;
    value = static_cast<t_short>(to_sint_(use, '3', '8', '7', 6_n, str));
    return use;
  }

  inline
  t_n to_integer_(r_ushort value, t_crange str) {
    t_n use = 0_n;
    value = static_cast<t_ushort>(to_uint_(use, '6', '5', 5_n, str));
    return use;
  }

  inline
  t_n to_integer_(r_int value, t_crange str) {
    t_n use = 0_n;
    value = static_cast<t_int>(to_sint_(use, '9', '8', '7', 20_n, str));
    return use;
  }

  inline
  t_n to_integer_(r_uint value, t_crange str) {
    t_n use = 0_n;
    value = static_cast<t_uint>(to_uint_(use, '1', '5', 20_n, str));
    return use;
  }

#if __LONG_WIDTH__ == 32 || __SIZEOF_LONG__ == 4
  inline
  t_n to_integer_(r_long value, t_crange str) {
    t_n use = 0_n;
    value = static_cast<t_long>(to_sint_(use, '9', '8', '7', 20_n, str));
    return use;
  }

  inline
  t_n to_integer_(r_ulong value, t_crange str) {
    t_n use = 0_n;
    value = static_cast<t_ulong>(to_uint_(use, '1', '5', 20_n, str));
    return use;
  }
#elif __LONG_WIDTH__ == 64 || __SIZEOF_LONG__ == 8
  inline
  t_n to_integer_(r_long value, t_crange str) {
    t_n use = 0_n;
    value = static_cast<t_long>(to_sint_(use, '9', '8', '7', 20_n, str));
    return use;
  }

  inline
  t_n to_integer_(r_ulong value, t_crange str) {
    t_n use = 0_n;
    value = static_cast<t_ulong>(to_uint_(use, '1', '5', 20_n, str));
    return use;
  }
#else
#error  compiler
#endif

  inline
  t_n to_integer_(r_llong value, t_crange str) {
    t_n use = 0_n;
    value = to_sint_(use, '9', '8', '7', 20_n, str);
    return use;
  }

  inline
  t_n to_integer_(r_ullong value, t_crange str) {
    t_n use = 0_n;
    value = to_uint_(use, '1', '5', 20_n, str);
    return use;
  }

///////////////////////////////////////////////////////////////////////////////

  inline
  t_n to_hexidecimal_(r_char value, t_crange str) {
    t_n use = 0_n;
    value = static_cast<t_char>(hex_to_uint_(use, 2_n, str));
    return use;
  }

  inline
  t_n to_hexidecimal_(r_schar value, t_crange str) {
    t_n use = 0_n;
    value = static_cast<t_schar>(hex_to_uint_(use, 2_n, str));
    return use;
  }

  inline
  t_n to_hexidecimal_(r_uchar value, t_crange str) {
    t_n use = 0_n;
    value = static_cast<t_uchar>(hex_to_uint_(use, 2_n, str));
    return use;
  }

  inline
  t_n to_hexidecimal_(r_short value, t_crange str) {
    t_n use = 0_n;
    value = static_cast<t_short>(hex_to_uint_(use, 4_n, str));
    return use;
  }

  inline
  t_n to_hexidecimal_(r_ushort value, t_crange str) {
    t_n use = 0_n;
    value = static_cast<t_ushort>(hex_to_uint_(use, 4_n, str));
    return use;
  }

  inline
  t_n to_hexidecimal_(r_int value, t_crange str) {
    t_n use = 0_n;
    value = static_cast<t_int>(hex_to_uint_(use, 8_n, str));
    return use;
  }

  inline
  t_n to_hexidecimal_(r_uint value, t_crange str) {
    t_n use = 0_n;
    value = static_cast<t_uint>(hex_to_uint_(use, 8_n, str));
    return use;
  }

#if __LONG_WIDTH__ == 32 || __SIZEOF_LONG__ == 4
  inline
  t_n to_hexidecimal_(r_long value, t_crange str) {
    t_n use = 0_n;
    value = static_cast<t_long>(hex_to_uint_(use, 8_n, str));
    return use;
  }

  inline
  t_n to_hexidecimal_(r_ulong value, t_crange str) {
    t_n use = 0_n;
    value = static_cast<t_ulong>(hex_to_uint_(use, 8_n, str));
    return use;
  }
#elif __LONG_WIDTH__ == 64 || __SIZEOF_LONG__ == 8
  inline
  t_n to_hexidecimal_(r_long value, t_crange str) {
    t_n use = 0_n;
    value = static_cast<t_long>(hex_to_uint_(use, 16_n, str));
    return use;
  }

  inline
  t_n to_hexidecimal_(r_ulong value, t_crange str) {
    t_n use = 0_n;
    value = static_cast<t_ulong>(hex_to_uint_(use, 16_n, str));
    return use;
  }
#else
#error unknown compiler
#endif

  inline
  t_n to_hexidecimal_(r_llong value, t_crange str) {
    t_n use = 0_n;
    value = hex_to_uint_(use, 16_n, str);
    return use;
  }

  inline
  t_n to_hexidecimal_(r_ullong value, t_crange str) {
    t_n use = 0_n;
    value = hex_to_uint_(use, 16_n, str);
    return use;
  }

///////////////////////////////////////////////////////////////////////////////

  template<typename T>
  inline
  t_n to_integer(T& value, t_crange str) {
    return to_integer_(value, str);
  }

///////////////////////////////////////////////////////////////////////////////

  template<typename T>
  inline
  t_n to_hexidecimal(T& value, t_crange str) {
    return to_hexidecimal_(value, str);
  }

///////////////////////////////////////////////////////////////////////////////
}
}
}
}

#endif
