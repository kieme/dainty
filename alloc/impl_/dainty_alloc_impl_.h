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

#ifndef _DAINTY_ALLOC_IMPL_H_
#define _DAINTY_ALLOC_IMPL_H_

#include "dainty_types.h"

namespace dainty
{
namespace alloc
{
namespace impl_
{
  /////////////////////////////////////////////////////////////////////////////

  using types::t_void;
  using types::p_void;

  using t_n       = types::t_n_;    // TODO
  using t_u1      = types::t_u1_;
  using t_n_bytes = t_n;

  /////////////////////////////////////////////////////////////////////////////

  p_void alloc_     (t_n_bytes);
  p_void alloc_fill_(t_n_bytes, t_u1 = 0);
  t_void dealloc_   (p_void);

  /////////////////////////////////////////////////////////////////////////////

  struct t_alloc {
    template<typename T>
    constexpr
    static t_n_bytes bytes(t_n n = 1) {
      return types::SIZEOF<T> * n;
    }

    template<typename T>
    inline
    T* acquire(t_n n = 1) {
      return static_cast<T*>(alloc_(bytes<T>(n)));
    }

    template<typename T>
    inline
    T* acquire_fill(t_n n = 1, t_u1 ch = 0) {
      return static_cast<T*>(alloc_fill_(bytes<T>(n), ch));
    }

    template<typename T>
    inline
    t_void release(T* mem) {
      dealloc_(mem);
    }
  };

  extern t_alloc global_;

  /////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
