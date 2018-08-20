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

#include "dainty_named_assert.h"
#include "dainty_os_clock.h"

namespace dainty
{
namespace os
{
namespace clock
{
  t_time monotonic_now() {
    t_time time;
    if (call_clock_gettime_monotonic(to_(time)) == VALID)
      return time;
    assert_now(P_cstr("could not read the monotonic time"));
    return {};
  }

  t_time monotonic_now(t_err err) {
    if (!err) {
      t_time time;
      if (call_clock_gettime_monotonic(err, to_(time)) == VALID)
        return time;
      err = E_XXX;
    }
    return {};
  }

  t_time realtime_now() {
    t_time time;
    if (call_clock_gettime_realtime(to_(time)) == VALID)
      return time;
    assert_now(P_cstr("could not read the realtime time"));
    return {};
  }

  t_time realtime_now(t_err err) {
    if (!err) {
      t_time time;
      if (call_clock_gettime_realtime(err, to_(time)) == VALID)
        return time;
      err = E_XXX;
    }
    return {};
  }
}
}
}
