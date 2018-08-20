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

#include "dainty_container_err.h"

namespace dainty
{
namespace container
{
  namespace
  {
    using namespace oops;
    using named::P_cstr;

    const t_def err_tbl_[] = {
      /* CATEGORY                 MESSAGE               NEXT CODE      */
      {v_category_unrecoverable, P_cstr("-"),            E_INVALID_IX},
      {v_category_ignore,        P_cstr("invalid ix"),   E_NO_SPACE},
      {v_category_ignore,        P_cstr("no space"),     E_NO_POP},
      {v_category_ignore,        P_cstr("no pop"),       E_INVALID_ID},
      {v_category_ignore,        P_cstr("invalid id"),   E_UNUSED_ID},
      {v_category_ignore,        P_cstr("unused id"),    E_INVALID_INST},
      {v_category_ignore,        P_cstr("invalid inst"), E_NOT_UNIQUE},
      {v_category_ignore,        P_cstr("not unique"),   E_NO_ERASE},
      {v_category_ignore,        P_cstr("no erase"),     0}
    };
  }

  t_def err_what(t_id id) {
    return err_tbl_[id <= E_NO_POP ? id : 0];
  }
}
}

