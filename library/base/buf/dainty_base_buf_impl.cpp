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

#define DAINTY_BASE_INTERNAL_INCL_C_STDLIB

#include "dainty_base_internal_incl_.h"
#include "dainty_base_buf_impl.h"

namespace dainty
{
namespace base
{
namespace buf
{
namespace impl_
{
///////////////////////////////////////////////////////////////////////////////

  p_void mem_alloc_(t_n n)  noexcept {
    return ::malloc(get(n));
  }

  p_void mem_realloc_(p_void obj, t_n n) noexcept {
    return ::realloc(obj, get(n));
  }

  t_void mem_dealloc_(p_void obj) noexcept {
    return ::free(obj);
  }

///////////////////////////////////////////////////////////////////////////////
}
}
}
}

