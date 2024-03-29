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

#define  DAINTY_INTERNAL_INCLUDE_C_STDLIB
#define  DAINTY_INTERNAL_INCLUDE_C_STRING
#include "dainty_internal_.h"
#include "dainty_alloc_impl_.h"

namespace dainty
{
namespace alloc
{
namespace impl_
{
  /////////////////////////////////////////////////////////////////////////////

  p_void alloc_(t_n_bytes bytes) {
    return static_cast<p_void>(::malloc(bytes));
  }

  p_void alloc_fill_(t_n_bytes bytes, t_u1 ch) {
    p_void mem = alloc_(bytes);
    ::memset(mem, ch, bytes);
    return mem;
  }

  t_void dealloc_(p_void mem) {
    ::free(mem);
  }

  /////////////////////////////////////////////////////////////////////////////
}
}
}
