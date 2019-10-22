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

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include "dainty_base_assert.h"
#include "dainty_base_terminal.h"
#include "dainty_base_string_impl.h"

namespace dainty
{
namespace base
{
namespace string
{
namespace impl_
{
////////////////////////////////////////////////////////////////////////////////

  using types::T_bool;
  using types::T_char;

  using assertion::assert_now;

////////////////////////////////////////////////////////////////////////////////

  // consider when max is zero

  t_void display(P_cstr_ fmt, ...) noexcept {
    va_list args;
    va_start(args, fmt);
    std::vprintf(fmt, args);
    va_end(args);
  }

  t_void display_(t_crange range, t_crange prefix, t_crange postfix) noexcept {
    terminal::t_out out;
    out << prefix << range << postfix;
  }

  t_void display_(t_crange range) noexcept {
    if (range == VALID) {
      auto max = get(range.n);
      if (range[t_ix{max - 1}] == '\n')
        display("%.*s", max, range.ptr);
      else
        display("%.*s\n", max, range.ptr);
    }
  }

  t_n build_(t_buf_range store, t_crange fmt, va_list vars) noexcept {
    if (fmt == VALID && store == VALID) {
      auto n = std::vsnprintf(store.ptr, get(store.n), fmt.ptr, vars);
      return t_n(n);
    }
    return 0_n;
  }

  t_n try_build(t_buf_range store, t_crange fmt, va_list vars) noexcept {
    if (fmt == VALID) {
      va_list args;
      va_copy(args, vars);
      t_n_ n = std::vsnprintf(store.ptr, get(store.n), fmt.ptr, args);
      va_end(args);
      return t_n{n};
    }
    return 0_n;
  }

  t_n copy_(t_buf_range store, t_crange range, t_overflow_assert) noexcept {
    if (range == VALID) {
      if (store == VALID) {
        auto len = get(range.n), max = get(store.n);
        if (len < max) {
          store.copy(range.mk_crange<t_buf_range::t_tag>());
          store[t_ix{len}] = '\0';
          return t_n{len};
        }
      }
      assert_now(P_cstr("buffer not big enough"));
    }
    return 0_n;
  }

  t_n copy_(t_buf_range store, t_crange range, t_overflow_truncate) noexcept {
    if (range == VALID) {
      if (store == VALID) {
        auto len = get(range.n), max = get(store.n);
        if (len < max)
          store.copy(range.mk_crange<t_buf_range::t_tag>());
        else {
          len = max - 1;
          store.copy(range.mk_crange<t_buf_range::t_tag>(), t_n{len});
        }
        store[t_ix{len}] = '\0';
        return t_n{len};
      }
    }
    return 0_n;
  }

  t_n fill_(t_buf_range store, t_block block, t_overflow_assert) noexcept {
    auto len = get(block.n);
    if (len) {
      if (store == VALID) {
        auto max = get(store.n);
        if (len < max) {
          store.copy(block);
          store[t_ix{len}] = '\0';
          return t_n{len};
        }
      }
      assert_now(P_cstr("buffer not big enough"));
    }
    return 0_n;
  }

  t_n fill_(t_buf_range store, t_block block, t_overflow_truncate) noexcept {
    auto len = get(block.n);
    if (len) {
      if (store == VALID) {
        auto max = get(store.n);
        if (len < max)
          store.copy(block);
        else {
          len = max - 1;
          store.copy(t_block{block.value, t_n{len}});
        }
        store[t_ix{len}] = '\0';
        return t_n{len};
      }
    }
    return 0_n;
  }

  t_bool equal_(t_crange lh, t_crange rh) noexcept {
    auto len1 = get(lh.n), len2 = get(rh.n); // XXX
    if (len1 == len2)
      return lh.ptr != rh.ptr ? (std::strncmp(lh.ptr, rh.ptr, len1) == 0)
                              : true;
    return false;
  }

  t_bool less_(t_crange lh, t_crange rh) noexcept {
    auto len1 = get(lh.n), len2 = get(rh.n);
    if (lh.ptr != rh.ptr) {
      auto ret = std::strncmp(lh.ptr, rh.ptr, len1 < len2 ? len1 : len2);
      return !ret ? len1 < len2 : ret < 0;
    }
    return false;
  }

  t_bool less_equal_(t_crange lh, t_crange rh) noexcept {
    auto len1 = get(lh.n), len2 = get(rh.n);
    if (lh.ptr != rh.ptr) {
      auto ret = std::strncmp(lh.ptr, rh.ptr, len1 < len2 ? len1 : len2);
      return !ret ? len1 <= len2 : ret < 0; // XXX - ok?
    }
    return true;
  }

  t_n length_(P_cstr str) noexcept {
    return t_n{std::strlen(get(str))};
  }

  t_n length_(t_crange fmt, va_list vars) noexcept {
    if (fmt == VALID) {
      va_list args;
      va_copy(args, vars);
      t_n_ require = std::vsnprintf(NULL, 0, fmt.ptr, args);
      va_end(args);
      return t_n{require};
    }
    return 0_n;
  }

  t_bool match_(t_crange str, t_crange pattern) noexcept {
    if (str == VALID && pattern == VALID) {
      P_cstr_ l = nullptr, s = str.ptr, p = pattern.ptr; // XXX not fully correct
      while (*p && *s) {
               if (*p == *s)      ++p;
          else if (*p == '*') l = ++p;
          else if (*p == '?') {   ++p;
        } else if (l) {
          if (p != l) {
            p = l;
            continue;
          }
        } else
          return false;
        ++s;
      }
      return *s ? (t_bool)l : !*p;
    }
    return false;
  }

  t_n count_(t_crange range, t_char ch) noexcept {
    t_n_ cnt = 0;
    for (auto c : range)
      if (c == ch)
        ++cnt;
    return t_n{cnt};
  }

///////////////////////////////////////////////////////////////////////////////

  t_bool t_impl_base_::remove(t_buf_range store, t_begin_ix _begin,
                              t_end_ix _end) noexcept {
    auto begin = get(_begin), end = get(_end), len = get(len_);
    if (begin < end && end <= len) {
      t_n_ remove_n = end - begin;
      while (end != len)
        store[t_ix(begin++)] = store[t_ix(end++)];
      store[t_ix{begin}] = '\0';
      len_ = t_n{len - remove_n};
      return true;
    }
    return false;
  }

///////////////////////////////////////////////////////////////////////////////

  t_ullong to_uint_(r_n use, t_char first, t_char last, t_n max_n,
                    t_crange range) noexcept {
    t_ullong value = 0;
    P_cstr_ str = range.ptr, p = str, max_p = str + get(max_n);
    for (; p < max_p && *p <= '9' && *p >= '0'; ++p);
    if (p != str) {
      if (p == max_p && (*str > first || (*str == first && p[-1] > last)))
        --p;
      use = t_n(p-- - str);
      for (t_ullong i = 1; p >= str; i *= 10)
        value += (*p-- - '0') * i;
    } else
      use = 0_n;
    return value;
  }

  t_llong to_sint_(r_n use, t_char first, t_char last_min,
                   t_char last_max, t_n max_n, t_crange range) noexcept {
    P_cstr_      str   = range.ptr;
    const t_bool neg   = *str == '-';
    const t_char last  = neg ? last_min : last_max;
    P_cstr_      begin = str + (neg || *str == '+');
    return static_cast<t_llong>(
             to_uint_(use, first, last, t_n{get(max_n) - (begin == str)},
                      string::mk_range(begin))) * (neg ? -1 : 1);
  }

  t_ullong hex_to_uint_(r_n use, t_n max_n, t_crange range) noexcept {
    t_ullong value = 0;
    P_cstr_ str = range.ptr, p = str, max_p = str + get(max_n);
    for (; p < max_p &&
         ((*p <= '9' && *p >= '0') ||
          (*p <= 'f' && *p >= 'a') ||
          (*p <= 'F' && *p >= 'A')); ++p);
    if (p != str) {
      use = t_n(p-- - str);
      for (t_ullong i = 1; p >= str; i *= 16) {
        if (*p >= '0' && *p <= '9')
          value += (*p-- - '0') * i;
        else if (*p >= 'a' && *p <= 'f')
          value += (*p-- - 'a' + 10) * i;
        else
          value += (*p-- - 'A' + 10) * i;
      }
    } else
      use = 0_n;
    return value;
  }

///////////////////////////////////////////////////////////////////////////////

  t_n uint_to_str_(t_buf_range range, t_ullong value) noexcept {
    auto max = get(range.n);
    t_n_ req = 0;
    if (max) {
      p_cstr_ dst = range.ptr;
      if (!value) {
        if (max > 1)
          *dst++ = '0';
        *dst = '\0';
        req = 1;
      } else {
        t_char tmp[20];
        for (; value; value/=10)
          tmp[req++] = value%10 + '0';
        t_ix_ ix  = max - 1;
        t_ix_ end = (ix < req ? ix : req);
        ix = 0;
        for (t_ix_ last = req - 1; ix < end; ++ix)
          dst[ix] = tmp[last--];
        dst[ix] = '\0';
      }
    }
    return t_n{req};
  }

  t_n int_to_str_(t_buf_range range, t_llong value) noexcept {
    T_bool neg = value < 0; // XXX
    if (neg) {
      value = -value;
      if (get(range.n) > 2) {
        *range.ptr = '-';
        return t_n(get(uint_to_str_(range.mk_range(t_begin_ix{1}), value)) + 1);
      }
      // XXX
    }
    return uint_to_str_(range, value);
  }

  t_n hex_to_str_(p_cstr_ dst, t_n_ max, t_ullong value) noexcept {
    t_n_ req = 0;
    if (max) {
      if (!value) {
        if (max > 1)
          *dst++ = '0';
        *dst = '\0';
        req = 1;
      } else {
        T_char tbl[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                         'a', 'b', 'c', 'd', 'e', 'f'};
        t_char tmp[16];
        for (; value; value/=16)
          tmp[req++] = tbl[value%16];
        t_ix_ last = req - 1;
        t_ix_ end  = (max < req ? max - 1 : last);
        t_ix_ ix   = 0;
        for (; ix < end; ++ix)
          dst[ix] = tmp[last--];
        dst[ix] = '\0';
      }
    }
    return t_n{req};
  }

///////////////////////////////////////////////////////////////////////////////

  t_n scan_(t_crange str, t_n _n, t_crange fmt, va_list args) noexcept {
    if (str == VALID && fmt == VALID) {
      auto found = std::vsscanf(str.ptr, fmt.ptr, args);
      auto n = get(_n);
      if (n && static_cast<t_int>(n) != found)
        assert_now(P_cstr("scanf could not find you value(s)"));
      return t_n(found);
    }
    return 0_n;
  }

///////////////////////////////////////////////////////////////////////////////

  t_n skip_(t_crange range, t_char ch) noexcept {
    t_n_ max = get(range.n);
    if (max >= 1 && range[t_ix{0}] == ch)
      return 1_n;

    assertion::assert_now(P_cstr("can't skip charater"));
    return 0_n;
  }

  t_n skip_(t_crange range, t_n n) noexcept {
    t_n_ max = get(range.n);
    if (get(n) <= max)
      return n;

    assertion::assert_now(P_cstr("buffer not big enough"));
    return 0_n;
  }

  t_n skip_(t_crange range, t_crange src) noexcept {
    auto ix_begin = t_begin_ix{0};
    auto ix_end   = t_end_ix{get(src.n)};
    auto tmp      = mk_range(range, ix_begin, ix_end);
    if (tmp == src)
      return src.n;

    assertion::assert_now(P_cstr("range not the same"));
    return 0_n;
  }

  t_n skip_(t_crange range, t_block block) noexcept {
    auto max = get(range.n), n = get(block.n);
    if (n <= max) {
      t_ix_ ix = 0;
      for (; ix < n && range[t_ix{ix}] == block.value; ++ix);
      if (ix == n)
        return block.n; // check XXX
    }

    assertion::assert_now(P_cstr("range not the same"));
    return 0_n;
  }

  t_n skip_until_(t_crange range, t_char ch, t_plus1_ plus1) noexcept {
    auto max = get(range.n);
    if (max) {
      t_ix_ ix = 0;
      for (; ix < max && range[t_ix{ix}] != ch; ++ix);
      if (ix < max)
        return t_n{ix + plus1};
    }

    assertion::assert_now(P_cstr("dont find char"));
    return 0_n;
  }

  t_n skip_until_(t_crange range, t_crange value, t_plus1_ plus1) noexcept {
    auto max = get(range.n), value_max = get(value.n);
    if (max && max > value_max) {
      t_ix_ ix = 0, k = 0;
      for (; ix < max && k < value_max; ++ix)
        k = range[t_ix{ix}] == value[t_ix{k}] ? k + 1 : 0;
      if (k == value_max)
        return t_n{plus1 == PLUS1 ? ix : ix - value_max};
    }

    assertion::assert_now(P_cstr("dont find substring"));
    return 0_n;
  }

  t_n skip_all_(t_crange range, t_char ch) noexcept {
    auto max = get(range.n);
    if (max) {
      t_ix_ ix = 0;
      for (; ix < max && range[t_ix{ix}] == ch; ++ix);
      if (ix <= max)
        return t_n{ix};
    }
    return 0_n;
  }

///////////////////////////////////////////////////////////////////////////////

  t_n snip_n_(t_crange range, p_snippet snip, t_n _n) noexcept {
    auto max = get(range.n), n = get(_n);
    if (max && max > n) {
      *snip = range;
      return range.n;
    }

    assertion::assert_now(P_cstr("not large enough"));
    return 0_n;
  }

  t_n snip_char_(t_crange range, p_snippet snip, t_char ch,
                 t_plus1_ plus1, t_incl_char_ incl_char) noexcept {
    auto max = get(range.n);
    if (max) {
      t_ix_ ix = 0;
      for (; ix < max && range[t_ix{ix}] != ch; ++ix);
      if (ix < max) {
        *snip = t_crange{range.ptr, t_n{ix + incl_char}};
        return t_n{ix + plus1};
      }
    }

    assertion::assert_now(P_cstr("not found"));
    return 0_n;
  }

  t_n snip_char_eol_(t_crange range, p_snippet snip, t_char ch,
                     t_plus1_ plus1, t_incl_char_ incl_char) noexcept {
    auto max = get(range.n);
    if (max) {
      t_ix_ ix = 0;
      for (; ix < max && range[t_ix{ix}] != ch; ++ix);
      if (ix < max) {
        *snip = t_crange{range.ptr, t_n{ix + incl_char}};
        return t_n{ix + plus1};
      }
      *snip = range;
      return range.n;
    }

    assertion::assert_now(P_cstr("not found"));
    return 0_n;
  }

  t_n snip_char_(t_crange range, p_snippet snip, p_char_select select,
                 t_plus1_ plus1, t_incl_char_ incl_char) noexcept {
    auto max = get(range.n);
    if (max) {
      t_ix_ ix = 0;
      for (; ix < max; ++ix) {
        for (t_char ch : select->range) {
          if (range[t_ix{ix}] == ch) {
            select->choice = ch;
            *snip = t_crange{range.ptr, t_n{ix + incl_char}};
            return t_n{ix + plus1};
          }
        }
      }
    }

    select->choice = EOL;
    return 0_n;
  }

  t_n snip_char_eol_(t_crange range, p_snippet snip, p_char_select select,
                      t_plus1_ plus1, t_incl_char_ incl_char) noexcept {
    auto max = get(range.n);
    if (max) {
      t_ix_ ix = 0;
      for (; ix < max; ++ix) {
        for (t_char ch : select->range) {
          if (range[t_ix{ix}] == ch) {
            select->choice = ch;
            *snip = t_crange{range.ptr, t_n{ix + incl_char}};
            return t_n{ix + plus1};
          }
        }
      }
      select->choice = EOL;
      *snip = range;
      return range.n;
    }

    select->choice = EOL;
    return 0_n;
  }

///////////////////////////////////////////////////////////////////////////////

  t_n shift_left_(p_cstr_ str, t_n_ max, t_n_ len, t_n_ width) noexcept {
    t_n_ n = max - 1;
    if (len < max) {
      if (len <= width) {
        if (width < max)
          n = width;
        for (t_ix_ ix = len; ix < n; ++ix)
          str[ix] = ' ';
      } else
        n = width;
      str[n] = '\0';
    } else if (width < max) {
      str[width] = '\0';
      n = width;
    }
    return t_n{n};
  }

  t_n shift_right_(p_cstr_ str, t_n_ max, t_n_ len, t_n_ width) noexcept {
    return t_n{len < max ? len : max - 1}; // XXX - not implemented
  }

  t_n shift_centre_(p_cstr_ str, t_n_ max, t_n_ len, t_n_ width) noexcept {
    return shift_left_(str, max, len, width); // XXX - not implemented
  }

////////////////////////////////////////////////////////////////////////////////

  t_impl_<t_overflow_assert>::t_impl_(t_buf_range store,
                                      t_crange range) noexcept
      : t_impl_base_{store} {
    assign(store, range);
  }

  t_impl_<t_overflow_assert>::t_impl_(t_buf_range store,
                                      t_block block) noexcept
      : t_impl_base_{store} {
    assign(store, block);
  }

  t_void t_impl_<t_overflow_assert>::assign(t_buf_range store,
                                            t_crange range) noexcept {
    len_ = copy_(store, range, OVERFLOW_ASSERT);
  }

  t_void t_impl_<t_overflow_assert>::assign(t_buf_range store,
                                            t_block block) noexcept {
    len_ = fill_(store, range, OVERFLOW_ASSERT);
  }

  t_void t_impl_<t_overflow_assert>::assign(t_buf_range store, t_crange fmt,
                                            va_list vars) noexcept {
    len_ = build_(store, fmt, vars, OVERFLOW_ASSERT);
  }

  t_void t_impl_<t_overflow_assert>::append(t_buf_range store,
                                            t_crange range) noexcept {
    if (get(len_) + get(range.n) >= get(store.n))
      assert_now(P_cstr{"t_string: append range"});
    t_impl_base_::append(store, range);
  }

  t_void t_impl_<t_overflow_assert>::append(t_buf_range store,
                                            t_block block) noexcept {
    if (get(len_) + get(block.n) >= get(store.n))
      assert_now(P_cstr{"t_string: append block"});
    t_impl_base_::append(store, block);
  }

  t_void t_impl_<t_overflow_assert>::append(t_buf_range store, t_crange fmt,
                                            va_list vars) noexcept {
    len_ = build_(store.mk_range(t_begin_ix{get(len_)}), fmt, vars,
                                 OVERFLOW_ASSERT); // XXX
  }

////////////////////////////////////////////////////////////////////////////////

  t_impl_<t_overflow_truncate>::t_impl_(t_buf_range store,
                                        t_crange range) noexcept
      : t_impl_base_{store} {
    assign(store, range);
  }

  t_impl_<t_overflow_truncate>::t_impl_(t_buf_range store,
                                        t_block block) noexcept
      : t_impl_base_{store} {
    assign(store, block);
  }

  t_void t_impl_<t_overflow_truncate>::assign(t_buf_range store,
                                              t_crange range) noexcept {
    t_impl_base_::assign(store, range);
  }

  t_void t_impl_<t_overflow_truncate>::assign(t_buf_range store,
                                              t_block block) noexcept {
    t_impl_base_::assign(store, block);
  }

  t_void t_impl_<t_overflow_truncate>::assign(t_buf_range store,
                                              t_crange fmt,
                                              va_list vars) noexcept {
    len_ = build_(store, fmt, vars, OVERFLOW_TRUNCATE);
  }

  t_void t_impl_<t_overflow_truncate>::append(t_buf_range store,
                                              t_crange range) noexcept {
  }

  t_void t_impl_<t_overflow_truncate>::append(t_buf_range store,
                                              t_block block) noexcept {
  }

  t_void t_impl_<t_overflow_truncate>::append(t_buf_range store,
                                              t_crange range,
                                              va_list vars) noexcept {
  }

////////////////////////////////////////////////////////////////////////////////
}
}
}
}
