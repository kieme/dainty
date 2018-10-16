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
#include "dainty_named_range.h"

namespace dainty
{
namespace named
{
namespace range
{
///////////////////////////////////////////////////////////////////////////////

  t_void check_(t_n_ n, t_ix_ begin) {
    if (begin >= n)
      assert_now(P_cstr{"range: overflow 1"});
  }

  t_void check_(t_n_ n, t_ix_ begin, t_ix_ end) {
    if (begin >= n || begin >= end || end - begin >= n)
      assert_now(P_cstr{"range: overflow 2"});
  }

  t_void check_(P_void item, t_n_ n) {
    if (!item || !n)
      assert_now(P_cstr{"range: init error"});
  }

  t_void check_(P_void item, t_n_ n, t_ix_ begin) {
    if (!item)
      assert_now(P_cstr{"range: invalid range"});
    check_(n, begin);
  }

  t_void check_(P_void item, t_n_ n, t_ix_ begin, t_ix_ end) {
    if (!item)
      assert_now(P_cstr{"range: invalid range"});
    check_(n, begin, end);
  }

  t_void check_(P_void item1, t_n_ n1, P_void item2, t_n_ n2) {
    if (n1 != n2)
      assert_now(P_cstr{"range: not same size"});
    check_(item1, n1);
    check_(item2, n2);
  }

///////////////////////////////////////////////////////////////////////////////
}
}
}
