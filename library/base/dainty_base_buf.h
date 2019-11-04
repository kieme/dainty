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

#ifndef _DAINTY_BASE_BUF_H_
#define _DAINTY_BASE_BUF_H_

#include "dainty_base_buf_1.h"
#include "dainty_base_buf_2.h"
#include "dainty_base_buf_3.h"

namespace dainty
{
namespace base
{
namespace buf
{
///////////////////////////////////////////////////////////////////////////////

// offer a buffer thats uninitialized but correctly aligned
// it could be used by nearly all my container structures.
// it takes care where the memory comes from - maybe use an allocator
//
// template<typename T, t_n_ N> class t_buf<T, N, t_size_dynamic>
// template<typename T>         class t_buf<T, 0, t_size_dynamic>
// template<typename T, t_n_ N> class t_buf<T, N, t_size_static>
// template<typename T>         class t_buf<T, 1, t_size_static>

///////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
