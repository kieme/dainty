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
    return 0;
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

  t_void display_(P_cstr_ str) noexcept {
    std::printf("%s", str);
  }

  t_int compare_(P_cstr_ lh, P_cstr_ rh) noexcept {
    return std::strcmp(lh, rh);
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
    P_cstr_ l = 0; // XXX not fully correct
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

////////////////////////////////////////////////////////////////////////////////
}
}
}

