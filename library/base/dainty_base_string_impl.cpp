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
////////////////////////////////////////////////////////////////////////////////

  using types::T_bool;
  using types::T_char;

////////////////////////////////////////////////////////////////////////////////

  t_void display(t_fmt, P_cstr_ fmt, ...) noexcept {
    va_list args;
    va_start(args, fmt);
    std::vprintf(fmt, args);
    va_end(args);
  }

  inline t_n_ multiple_of_64_(t_n_ n) noexcept {
    if (n%64)
      n = (n - n%64) + 64;
    return n;
  }

  t_n_ calc_n_(t_n_ chars, t_n_ blks) noexcept {
    return blks*64 + multiple_of_64_(chars + 1);
  }

  p_cstr_ alloc_(t_n_ n) noexcept {
    p_cstr_ str = (p_cstr_)std::malloc(n);
    if (!str)
      assertion::assert_now(P_cstr("malloc failed to allocate"));
    return str;
  }

  t_void dealloc_(p_cstr_ str) noexcept {
    if (str)
      std::free(str);
  }

  p_cstr_ realloc_(p_cstr_ str, t_n_ n) noexcept {
    str = (p_cstr_)std::realloc(str, n);
    if (!str)
      assertion::assert_now(P_cstr("realloc failed to allocate"));
    return str;
  }

  p_cstr_ sso_alloc_(p_cstr_ sso, t_n_ sso_max, t_n_& max) noexcept {
    if (max > sso_max)
      return alloc_(max);
    max = sso_max;
    return sso;
  }

  p_cstr_ sso_alloc_(p_cstr_ sso, t_n_ sso_max, p_cstr_ curr,
                     t_n_ max) noexcept {
    if (max > sso_max) {
      if (curr != sso)
        return realloc_(curr, max);
      p_cstr_ tmp = alloc_(max);
      memcpy(tmp, curr, max);
      return tmp;
    }
    return curr;
  }

  t_n_ build_(p_cstr_ dst, t_n_ max, P_cstr_ fmt, va_list vars,
              t_overflow_grow) noexcept {
    va_list args;
    va_copy(args, vars);
    t_n_ n = std::vsnprintf(dst, max, fmt, args);
    va_end(args);
    return n;
  }

  t_n_ build_(p_cstr_ dst, t_n_ max, P_cstr_ fmt, va_list vars,
              t_overflow_assert) noexcept {
    auto n = std::vsnprintf(dst, max, fmt, vars);
    assertion::assert_when_false(n > 0 && (t_n_)n < max,
      P_cstr("failed to build, buffer might be too small"));
    return n;
  }

  t_n_ build_(p_cstr_ dst, t_n_ max, P_cstr_ fmt, va_list vars,
              t_overflow_truncate) noexcept {
    auto n = std::vsnprintf(dst, max, fmt, vars);
    assertion::assert_when_false(n > 0,
      P_cstr("failed to build, std::vsnprintf failed"));
    if ((t_n_)n >= max)
      n = max - 1;
    return n;
  }

  t_n_ copy_(p_cstr_ dst, t_n_ max, P_cstr_ src, t_n_ n,
             t_overflow_assert) noexcept {
    t_n_ cnt = 0, min = max - 1 < n ? max - 1 : n;
    for (; cnt < min && src[cnt]; ++cnt)
      dst[cnt] = src[cnt];
    if (src[cnt] && cnt != n)
      assertion::assert_now(P_cstr("buffer not big enough"));
    dst[cnt] = '\0';
    return cnt;
  }

  t_n_ copy_(p_cstr_ dst, t_n_ max, P_cstr_ src, t_n_ n,
             t_overflow_truncate) noexcept {
    t_n_ cnt = 0, min = max - 1 < n ? max - 1 : n;
    for (; cnt < min && src[cnt]; ++cnt)
      dst[cnt] = src[cnt];
    dst[cnt] = '\0';
    return cnt;
  }

  t_n_ copy_(p_cstr_ dst, t_n_ max, P_cstr_ src,
             t_overflow_assert) noexcept {
    t_n_ cnt = 0, min = max - 1;
    for (; cnt < min && src[cnt]; ++cnt)
      dst[cnt] = src[cnt];
    if (src[cnt])
      assertion::assert_now(P_cstr("buffer not big enough"));
    dst[cnt] = '\0';
    return cnt;
  }

  t_n_ copy_(p_cstr_ dst, t_n_ max, P_cstr_ src,
             t_overflow_truncate) noexcept {
    t_n_ cnt = 0, min = max - 1;
    for (; cnt < min && src[cnt]; ++cnt)
      dst[cnt] = src[cnt];
    dst[cnt] = '\0';
    return cnt;
  }

  t_n_ fill_(p_cstr_ dst, t_n_ max, R_block block,
             t_overflow_assert) noexcept {
    auto bmax = get(block.max);
    if (bmax > max - 1)
      assertion::assert_now(P_cstr("buffer not big enough"));
    for (t_n_ cnt = 0; cnt < bmax; ++cnt)
      dst[cnt] = block.c;
    dst[bmax] = '\0';
    return bmax;
  }

  t_n_ fill_(p_cstr_ dst, t_n_ max, R_block block,
             t_overflow_truncate) noexcept {
    auto bmax = get(block.max);
    t_n_ min = max - 1 < bmax ? max - 1 : bmax;
    for (t_n_ cnt = 0; cnt < min; ++cnt)
      dst[cnt] = block.c;
    dst[min] = '\0';
    return min;
  }

  t_void display_(R_crange range, R_crange prefix, R_crange postfix) noexcept {
    terminal::t_out out;
    out << prefix << range << postfix;
  }

  t_void display_(P_cstr_ str, t_n_ len) noexcept {
    if (len && str[len - 1] == '\n')
      std::printf("%s", str);
    else
      std::printf("%s\n", str);
  }

  t_void display_n_(P_cstr_ str, t_n_ max) noexcept {
    if (max && str[max-1] == '\n')
      std::printf("%.*s", static_cast<t_int>(max), str);
    else
      std::printf("%.*s\n", static_cast<t_int>(max), str);
  }

  t_bool equal_(R_crange lh, R_crange rh) noexcept {
    auto len1 = get(lh.n), len2 = get(rh.n);
    if (len1 == len2)
      return lh.ptr != rh.ptr ? (std::strncmp(lh.ptr, rh.ptr, len1) == 0)
                              : true;
    return false;
  }

  t_bool less_(R_crange lh, R_crange rh) noexcept {
    auto len1 = get(lh.n), len2 = get(rh.n);
    if (lh.ptr != rh.ptr) {
      auto ret = std::strncmp(lh.ptr, rh.ptr, len1 < len2 ? len1 : len2);
      if (ret == 0)
        return len1 < len2;
      return ret < 0;
    }
    return false;
  }

  t_bool less_equal_(R_crange lh, R_crange rh) noexcept {
    auto len1 = get(lh.n), len2 = get(rh.n);
    if (lh.ptr != rh.ptr) {
      auto ret = std::strncmp(lh.ptr, rh.ptr, len1 < len2 ? len1 : len2);
      if (ret == 0)
        return len1 <= len2;
      return ret < 0;
    }
    return true;
  }

  t_n_ length_(P_cstr_ str) noexcept {
    return std::strlen(str);
  }

  t_n_ length_(P_cstr_ fmt, va_list vars) noexcept {
    va_list args;
    va_copy(args, vars);
    t_n_ require = std::vsnprintf(NULL, 0, fmt, args);
    va_end(args);
    return require;
  }

  t_bool match_(P_cstr_ str, P_cstr_ pattern) noexcept {
    P_cstr_ l = nullptr; // XXX not fully correct
    while (*pattern && *str) {
             if (*pattern == *str)    ++pattern;
        else if (*pattern == '*') l = ++pattern;
        else if (*pattern == '?') {   ++pattern;
      } else if (l) {
        if (pattern != l) {
          pattern = l;
          continue;
        }
      } else
        return false;
      ++str;
    }
    return *str ? (t_bool)l : !*pattern;
  }

  t_n_ count_(t_char c,  P_cstr_ str) noexcept {
    t_n_ cnt = 0;
    for (; *str; ++str)
      if (*str == c)
        ++cnt;
    return cnt;
  }

///////////////////////////////////////////////////////////////////////////////

  t_ullong to_uint_(t_n_& use, t_char first, t_char last, t_n_ max_n,
                    P_cstr_ str) noexcept {
    t_ullong value = 0;
    P_cstr_ p = str, max_p = str + max_n;
    for (; p < max_p && *p <= '9' && *p >= '0'; ++p);
    if (p != str) {
      if (p == max_p && (*str > first || (*str == first && p[-1] > last)))
        --p;
      use = p-- - str;
      for (t_ullong i = 1; p >= str; i *= 10)
        value += (*p-- - '0') * i;
    } else
      use = 0;
    return value;
  }

  t_llong to_sint_(t_n_& use, t_char first, t_char last_min,
                   t_char last_max, t_n_ max_n, P_cstr_ str) noexcept {
    const t_bool neg   = *str == '-';
    const t_char last  = neg ? last_min : last_max;
    P_cstr_      begin = str + (neg || *str == '+');
    return static_cast<t_llong>(
             to_uint_(use, first, last, max_n - (begin == str), begin))
           * (neg ? -1 : 1);
  }

  t_ullong hex_to_uint_(t_n_& use, t_n_ max_n, P_cstr_ str) noexcept {
    t_ullong value = 0;
    P_cstr_ p = str, max_p = str + max_n;
    for (; p < max_p &&
         ((*p <= '9' && *p >= '0') ||
          (*p <= 'f' && *p >= 'a') ||
          (*p <= 'F' && *p >= 'A')); ++p);
    if (p != str) {
      use = p-- - str;
      for (t_ullong i = 1; p >= str; i *= 16) {
        if (*p >= '0' && *p <= '9')
          value += (*p-- - '0') * i;
        else if (*p >= 'a' && *p <= 'f')
          value += (*p-- - 'a' + 10) * i;
        else
          value += (*p-- - 'A' + 10) * i;
      }
    } else
      use = 0;
    return value;
  }

///////////////////////////////////////////////////////////////////////////////

  t_n_ uint_to_str_(p_cstr_ dst, t_n_ max, t_ullong value) noexcept {
    t_n_ req = 0;
    if (max) {
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
    return req;
  }

  t_n_ int_to_str_(p_cstr_ dst, t_n_ max, t_llong value) noexcept {
    T_bool neg = value < 0;
    if (neg) {
      if (max > 2)
        *dst++ = '-';
    }
    return uint_to_str_(dst, max - neg, neg ? -value : value) + neg;
  }

  t_n_ hex_to_str_(p_cstr_ dst, t_n_ max, t_ullong value) noexcept {
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
    return req;
  }

///////////////////////////////////////////////////////////////////////////////

  t_void scan_(P_cstr_ str, t_n_ n, P_cstr_ fmt, va_list args) noexcept {
    auto cnt = std::vsscanf(str, fmt, args);
    if (cnt != static_cast<t_int>(n))
      assertion::assert_now(P_cstr("scanf could not find you value(s)"));
  }

  t_void scan_fmt_(P_cstr_ str, t_n_ n, P_cstr_ fmt, ...) noexcept {
    va_list args;
    va_start(args, fmt);
    scan_(str, n, fmt, args);
    va_end(args);
  }

///////////////////////////////////////////////////////////////////////////////

  t_n_ skip_(R_crange range, t_char ch) noexcept {
    t_n_ max = get(range.n);
    if (max >= 1 && range[t_ix{0}] == ch)
      return 1;

    assertion::assert_now(P_cstr("can't skip charater"));
    return 0;
  }

  t_n_ skip_(R_crange range, t_n_ n) noexcept {
    t_n_ max = get(range.n);
    if (n <= max)
      return n;

    assertion::assert_now(P_cstr("buffer not big enough"));
    return 0;
  }

  t_n_ skip_(R_crange range, R_crange src) noexcept {
    auto ix_begin = t_ix{0}, ix_end = t_ix{get(src.n)};
    auto tmp = mk_range(range, ix_begin, ix_end);
    if (tmp == src)
      return get(src.n);

    assertion::assert_now(P_cstr("range not the same"));
    return 0;
  }

  t_n_ skip_(R_crange range, R_block block) noexcept {
    auto max = get(range.n), n = get(block.max);
    if (n <= max) {
      t_ix_ ix = 0;
      for (; ix < n && range[t_ix{ix}] == block.c; ++ix);
      if (ix == n)
        return n;
    }

    assertion::assert_now(P_cstr("range not the same"));
    return 0;
  }

  t_n_ skip_until_(R_crange range, t_char ch, t_plus1_ plus1) noexcept {
    auto max = get(range.n);
    if (max) {
      t_ix_ ix = 0;
      for (; ix < max && range[t_ix{ix}] != ch; ++ix);
      if (ix < max)
        return ix + plus1;
    }

    assertion::assert_now(P_cstr("dont find char"));
    return 0;
  }

  t_n_ skip_until_(R_crange range, R_crange value, t_plus1_ plus1) noexcept {
    auto max = get(range.n), value_max = get(value.n);
    if (max && max > value_max) {
      t_ix_ ix = 0, k = 0;
      for (; ix < max && k < value_max; ++ix)
        k = range[t_ix{ix}] == value[t_ix{k}] ? k + 1 : 0;
      if (k == value_max)
        return plus1 == PLUS1 ? ix : ix - value_max;
    }

    assertion::assert_now(P_cstr("dont find substring"));
    return 0;
  }

  t_n_ skip_all_(R_crange range, t_char ch) noexcept {
    auto max = get(range.n);
    if (max) {
      t_ix_ ix = 0;
      for (; ix < max && range[t_ix{ix}] == ch; ++ix);
      if (ix <= max)
        return ix;
    }
    return 0;
  }

///////////////////////////////////////////////////////////////////////////////

  t_n_ snip_n_(R_crange range, p_snippet snip, t_n_ n) noexcept {
    auto max = get(range.n);
    if (max && max > n) {
      *snip = range;
      return max;
    }

    assertion::assert_now(P_cstr("not large enough"));
    return 0;
  }

  t_n_ snip_char_(R_crange range, p_snippet snip, t_char ch,
                  t_plus1_ plus1, t_incl_char_ incl_char) noexcept {
    auto max = get(range.n);
    if (max) {
      t_ix_ ix = 0;
      for (; ix < max && range[t_ix{ix}] != ch; ++ix);
      if (ix < max) {
        *snip = t_crange{range.ptr, t_n{ix + incl_char}};
        return ix + plus1;
      }
    }

    assertion::assert_now(P_cstr("not found"));
    return 0;
  }

  t_n_ snip_char_eol_(R_crange range, p_snippet snip, t_char ch,
                      t_plus1_ plus1, t_incl_char_ incl_char) noexcept {
    auto max = get(range.n);
    if (max) {
      t_ix_ ix = 0;
      for (; ix < max && range[t_ix{ix}] != ch; ++ix);
      if (ix < max) {
        *snip = t_crange{range.ptr, t_n{ix + incl_char}};
        return ix + plus1;
      }
      *snip = range;
      return max;
    }

    assertion::assert_now(P_cstr("not found"));
    return 0;
  }

  t_n_ snip_char_(R_crange range, p_snippet snip, p_char_select select,
                  t_plus1_ plus1, t_incl_char_ incl_char) noexcept {
    auto max = get(range.n);
    if (max) {
      t_ix_ ix = 0;
      for (; ix < max; ++ix) {
        for (t_char ch : select->range) {
          if (range[t_ix{ix}] == ch) {
            select->choice = ch;
            *snip = t_crange{range.ptr, t_n{ix + incl_char}};
            return ix + plus1;
          }
        }
      }
    }

    select->choice = EOL;
    return 0;
  }

  t_n_ snip_char_eol_(R_crange range, p_snippet snip, p_char_select select,
                      t_plus1_ plus1, t_incl_char_ incl_char) noexcept {
    auto max = get(range.n);
    if (max) {
      t_ix_ ix = 0;
      for (; ix < max; ++ix) {
        for (t_char ch : select->range) {
          if (range[t_ix{ix}] == ch) {
            select->choice = ch;
            *snip = t_crange{range.ptr, t_n{ix + incl_char}};
            return ix + plus1;
          }
        }
      }
      select->choice = EOL;
      *snip = range;
      return max;
    }

    select->choice = EOL;
    return 0;
  }

///////////////////////////////////////////////////////////////////////////////

  t_n_ shift_left_(p_cstr_ str, t_n_ max, t_n_ len, t_n_ width) noexcept {
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
    return n;
  }

  t_n_ shift_right_(p_cstr_ str, t_n_ max, t_n_ len, t_n_ width) noexcept {
    return len < max ? len : max - 1; // XXX do next
  }

  t_n_ shift_centre_(p_cstr_ str, t_n_ max, t_n_ len, t_n_ width) noexcept {
    return shift_left_(str, max, len, width);
  }

////////////////////////////////////////////////////////////////////////////////
}
}
}
