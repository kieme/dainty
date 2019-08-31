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

#include <execinfo.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include "dainty_base_terminal.h"
#include "dainty_base_assert.h"

namespace dainty
{
namespace base
{
namespace assertion
{
  using namespace types;
  using namespace specific;
  using namespace terminal;

  t_void assert_now(P_cstr reason) noexcept {
    t_out{FMT, "assert: %s\n", get(reason)};

    p_void array[20];
    auto size = backtrace(array, 20);

    backtrace_symbols_fd(array, size, STDERR_FILENO);

    fflush(stdout);
    fflush(stderr);

    assert(0);
  }

  t_void assert_now(t_fmt_va, P_cstr_ fmt, va_list args) noexcept {
    t_out out{"assert:"};
    out.append(FMT_VA_IT, fmt, args);
    out.display();

    p_void array[20];
    auto size = backtrace(array, 20);

    backtrace_symbols_fd(array, size, STDERR_FILENO);

    fflush(stdout);
    fflush(stderr);

    assert(0);
  }

  t_void assert_now(t_fmt, P_cstr_ fmt, ...) noexcept {
    va_list args;
    va_start(args, fmt);
    assert_now(FMT_VA_IT, fmt, args);
    va_end(args);
  }
}
}
}

