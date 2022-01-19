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

#ifndef _DAINTY_ALLOC_H_
#define _DAINTY_ALLOC_H_

#include "impl_/dainty_alloc_impl_.h"

namespace dainty
{
namespace alloc
{
  /////////////////////////////////////////////////////////////////////////////

  using impl_::t_void;
  using impl_::t_n;
  using impl_::t_u1;
  using impl_::t_n_bytes;

  /////////////////////////////////////////////////////////////////////////////

  template<typename T>
  T* alloc(t_n n = 1);

  template<typename T>
  T* alloc_fill(t_n n = 1, t_u1 ch = 0);

  template<typename T>
  t_void dealloc(T* mem);

  /////////////////////////////////////////////////////////////////////////////

  struct t_alloc {
    template<typename T>
    constexpr
    static t_n_bytes bytes(t_n n = 1);

    template<typename T>
    T* acquire(t_n n = 1);

    template<typename T>
    T* acquire_fill(t_n n = 1, t_u1 ch = 0);

    template<typename T>
    t_void release(T* mem);
  };

  /////////////////////////////////////////////////////////////////////////////
}
}

#include "impl_/dainty_alloc_.icpp"

#endif
