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

#ifndef _DAINTY_OS_ERR_H_
#define _DAINTY_OS_ERR_H_

#include "dainty_oops.h"

namespace dainty
{
namespace os
{
///////////////////////////////////////////////////////////////////////////////

  using named::t_bool;
  using named::t_validity;
  using named::VALID;
  using named::INVALID;
  using named::t_errn;

  template<typename T>
  using t_verify = named::t_verifiable<T>;

///////////////////////////////////////////////////////////////////////////////

  enum t_err_codes {
    E_TIMEOUT = 1,
    E_INVALID_INST,
    E_VALID_INST,
    E_INIT_FAIL,
    E_DESTROY_FAIL,
    E_ATTR_NOT_RECURSIVE,
    E_ATTR_NOT_MONOTONIC,
    E_XXX
  };

  oops::t_def err_what(oops::t_id);

  using t_err = oops::t_oops<err_what, t_err_codes>;

///////////////////////////////////////////////////////////////////////////////

}
}

#define T_ERR_GUARD_TAG(err, tagid) if DAINTY_OOPS_BLOCK_GUARD_TAG(err, tagid)
#define T_ERR_GUARD(err)            if DAINTY_OOPS_BLOCK_GUARD(err)

#endif
