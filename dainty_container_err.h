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

#ifndef _DAINTY_CONTAINER_ERR_H_
#define _DAINTY_CONTAINER_ERR_H_

#include "dainty_oops.h"

namespace dainty
{
namespace container
{
  enum t_err_codes {
    E_INVALID_IX   = 1,
    E_NO_SPACE     = 2,
    E_NO_POP       = 3,
    E_INVALID_ID   = 4,
    E_UNUSED_ID    = 5,
    E_INVALID_INST = 6,
    E_NOT_UNIQUE   = 7,
    E_NO_ERASE     = 8
  };

  oops::t_def err_what(oops::t_id);

  using t_err = oops::t_oops<err_what, t_err_codes>;
}
}

#define T_ERR_GUARD_TAG(err, tagid) if DAINTY_OOPS_BLOCK_GUARD_TAG(err, tagid)
#define T_ERR_GUARD(err)            if DAINTY_OOPS_BLOCK_GUARD(err)

#endif
