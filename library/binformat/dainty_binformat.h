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

#ifndef _DAINTY_BINFORMAT_H_
#define _DAINTY_BINFORMAT_H_

#include "dainty_base.h"

namespace dainty
{
namespace binformat
{
/******************************************************************************/

  enum t_builtin_value_type {
    TYPE_P,
    TYPE_U,
    TYPE_I,
    TYPE_IX,
    TYPE_S,
    TYPE_B,
    TYPE_H,
    TYPE_DT,
    TYPE_D,
    TYPE_T
  };

  class t_header_attr {
  };

  class t_header_def {
  };

  class t_header {
    t_header(.... input how it looks);
    t_header(.... from file);
  };

  class t_content {
    t_content(t_header);

    // encode
    // decode
  };

/******************************************************************************/
}
}

#endif
