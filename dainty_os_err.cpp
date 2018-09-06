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

#include "dainty_os_err.h"

namespace dainty
{
namespace os
{
namespace err
{
  namespace
  {
    using namespace oops;

    const t_def err_tbl_[] = {
     /* CATEGORY           MESSAGE                NEXT CODE      */
      { IGNORE, P_cstr{"os: bad"},             E_TIMEOUT            },
      { IGNORE, P_cstr{"os::timeout"},         E_INVALID_INST       },
      { IGNORE, P_cstr{"os::invalid inst"},    E_VALID_INST         },
      { IGNORE, P_cstr{"os::existing inst"},   E_INIT_FAIL          },
      { IGNORE, P_cstr{"os::init failed"},     E_DESTROY_FAIL       },
      { IGNORE, P_cstr{"os::destroy failed"},  E_ATTR_NOT_RECURSIVE },
      { IGNORE, P_cstr{"os::not recursive"},   E_ATTR_NOT_MONOTONIC },
      { IGNORE, P_cstr{"os::not monotonic"},   E_XXX                },
      { IGNORE, P_cstr{"os::undefined error"}, 0                    }
    };
  }

  t_def err_what(t_id id) {
    return err_tbl_[id <= 0 ? id : 0]; // fix later
  }
}
}
}

