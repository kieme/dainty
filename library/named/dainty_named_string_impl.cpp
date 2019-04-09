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
#include "dainty_named_assert.h"
#include "dainty_named_string_impl.h"

namespace dainty
{
namespace named
{
namespace string
{
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
      assert_now(P_cstr("malloc failed to allocate"));
    return str;
  }

  t_void dealloc_(p_cstr_ str) noexcept {
    if (str)
      std::free(str);
  }

  p_cstr_ realloc_(p_cstr_ str, t_n_ n) noexcept {
    str = (p_cstr_)std::realloc(str, n);
    if (!str)
      assert_now(P_cstr("realloc failed to allocate"));
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

  t_n_ build_try_(p_cstr_ dst, t_n_ max, P_cstr_ fmt, va_list vars) noexcept {
    va_list args;
    va_copy(args, vars);
    t_n_ n = std::vsnprintf(dst, max, fmt, args);
    va_end(args);
    return n;
  }

  t_n_ build_assert_(p_cstr_ dst, t_n_ max, P_cstr_ fmt,
                     va_list vars) noexcept {
    auto n = std::vsnprintf(dst, max, fmt, vars);
    assert_if_false(n > 0 && (t_n_)n < max,
                    P_cstr("failed to build, buffer might be too small"));
    return n;
  }

  t_n_ build_truncate_(p_cstr_ dst, t_n_ max, P_cstr_ fmt,
                       va_list vars) noexcept {
    auto n = std::vsnprintf(dst, max, fmt, vars);
    assert_if_false(n > 0, P_cstr("failed to build, std::vsnprintf failed"));
    if ((t_n_)n >= max)
      n = max - 1;
    return n;
  }

  t_n_ copy_assert_(p_cstr_ dst, t_n_ max, P_cstr_ src, t_n_ n) noexcept {
    t_n_ cnt = 0, min = max - 1 < n ? max - 1 : n;
    for (; cnt < min && src[cnt]; ++cnt)
      dst[cnt] = src[cnt];
    if (src[cnt] && cnt != n)
      assert_now(P_cstr("buffer not big enough"));
    dst[cnt] = '\0';
    return cnt;
  }

  t_n_ copy_truncate_(p_cstr_ dst, t_n_ max, P_cstr_ src, t_n_ n) noexcept {
    t_n_ cnt = 0, min = max - 1 < n ? max - 1 : n;
    for (; cnt < min && src[cnt]; ++cnt)
      dst[cnt] = src[cnt];
    dst[cnt] = '\0';
    return cnt;
  }

  t_n_ copy_assert_(p_cstr_ dst, t_n_ max, P_cstr_ src) noexcept {
    t_n_ cnt = 0, min = max - 1;
    for (; cnt < min && src[cnt]; ++cnt)
      dst[cnt] = src[cnt];
    if (src[cnt])
      assert_now(P_cstr("buffer not big enough"));
    dst[cnt] = '\0';
    return cnt;
  }

  t_n_ copy_truncate_(p_cstr_ dst, t_n_ max, P_cstr_ src) noexcept {
    t_n_ cnt = 0, min = max - 1;
    for (; cnt < min && src[cnt]; ++cnt)
      dst[cnt] = src[cnt];
    dst[cnt] = '\0';
    return cnt;
  }

  t_n_ fill_assert_(p_cstr_ dst, t_n_ max, R_block block) noexcept {
    auto bmax = get(block.max);
    if (bmax > max - 1)
      assert_now(P_cstr("buffer not big enough"));
    for (t_n_ cnt = 0; cnt < bmax; ++cnt)
      dst[cnt] = block.c;
    dst[bmax] = '\0';
    return bmax;
  }

  t_n_ fill_truncate_(p_cstr_ dst, t_n_ max, R_block block) noexcept {
    auto bmax = get(block.max);
    t_n_ min = max - 1 < bmax ? max - 1 : bmax;
    for (t_n_ cnt = 0; cnt < min; ++cnt)
      dst[cnt] = block.c;
    dst[min] = '\0';
    return min;
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

  t_void scan_(R_crange range, t_n_ n, P_cstr_ fmt, va_list args) noexcept {
    auto cnt = std::vsscanf(range.ptr, fmt, args);
    if (cnt != static_cast<t_int>(n))
      assert_now(P_cstr("scanf could not find you value(s)"));
  }

  r_walk_ scan_(r_walk_ walk, t_n_& len, t_n_ n, P_cstr_ fmt, ...) noexcept {
    va_list args;
    va_start(args, fmt);
    auto cnt = std::vsscanf(walk.ptr, fmt, args);
    va_end(args);

    if (cnt != static_cast<t_int>(n))
      assert_now(P_cstr("scanf could not find you value(s)"));

    return jump_forward_(walk, len);
  }

  r_walk_ skip_(r_walk_ walk, t_char ch) noexcept {
    t_n_ max = get(walk.n);
    if (max >= 1 && walk.ptr[0] == ch)
      return jump_forward_(walk, 1);

    assert_now(P_cstr("can't skip charater"));
    return walk;
  }

  r_walk_ skip_(r_walk_ walk, t_n_ n) noexcept {
    t_n_ max = get(walk.n);
    if (n <= max)
      return jump_forward_(walk, n);

    assert_now(P_cstr("buffer not big enough"));
    return walk;
  }

  r_walk_ skip_(r_walk_ walk, R_crange src) noexcept {
    auto ix_begin = t_ix{0}, ix_end = t_ix{get(src.n)};
    auto tmp = mk_range(walk, ix_begin, ix_end);
    if (tmp == src)
      return jump_forward_(walk, get(src.n));

    assert_now(P_cstr("range not the same"));
    return walk;
  }

  r_walk_ skip_(r_walk_ walk, R_block block) noexcept {
    auto max = get(walk.n), n = get(block.max);
    if (n <= max) {
      t_ix_ ix = 0;
      for (; ix < n && walk.ptr[ix] == block.c; ++ix);
      if (ix == n)
        return jump_forward_(walk, n);
    }

    assert_now(P_cstr("range not the same"));
    return walk;
  }

  r_walk_ skip_until_(r_walk_ walk, t_char ch, t_bool plus1) noexcept {
    auto max = get(walk.n);
    if (max) {
      t_ix_ ix = 0;
      for (; ix < max && walk.ptr[ix] != ch; ++ix);
      if (ix < max)
        return jump_forward_(walk, ix + plus1);
    }

    assert_now(P_cstr("dont find char"));
    return walk;
  }

  r_walk_ skip_until_(r_walk_ walk, R_crange value, t_bool plus1) noexcept {
    auto max = get(walk.n), value_max = get(value.n);
    if (max && max > value_max) {
      t_ix_ ix = 0, k = 0;
      for (; ix < max && k < value_max; ++ix)
        k = walk.ptr[ix] == value[t_ix{k}] ? k + 1 : 0;
      if (k == value_max)
        return jump_forward_(walk, plus1 ? ix : ix - value_max);
    }

    assert_now(P_cstr("dont find substring"));
    return walk;
  }

  r_walk_ skip_all_(r_walk_ walk, t_char ch) noexcept {
    auto max = get(walk.n);
    if (max) {
      t_ix_ ix = 0;
      for (; ix < max && walk.ptr[ix] == ch; ++ix);
      if (ix <= max)
        return jump_forward_(walk, ix);
    }
    return walk;
  }

  r_walk_ snip_n_(r_walk_ walk, p_snippet snip, t_n_ n) noexcept {
    auto max = get(walk.n);
    if (max && max > n) {
      snip->ptr = walk.ptr;
      snip->n   = t_n{n};
      return jump_forward_(walk, n);
    }

    assert_now(P_cstr("not large enough"));
    return walk;
  }

  r_walk_ snip_char_(r_walk_ walk, p_snippet snip, t_char ch,
                      t_bool plus1, t_bool incl_char) noexcept {
    auto max = get(walk.n);
    if (max) {
      t_ix_ ix = 0;
      for (; ix < max && walk.ptr[ix] != ch; ++ix);
      if (ix < max) {
        snip->ptr = walk.ptr;
        snip->n   = t_n{incl_char ? ix + 1: ix};
        if (plus1)
          ++ix;
        return jump_forward_(walk, ix);
      }
    }

    assert_now(P_cstr("not found"));
    return walk;
  }

////////////////////////////////////////////////////////////////////////////////
}
}
}
