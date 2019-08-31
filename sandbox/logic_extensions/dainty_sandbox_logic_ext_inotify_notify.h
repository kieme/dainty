/******************************************************************************

MIT License

Copyright (c) 2018 frits.germs@gmx.net

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

#ifndef _DAINTY_SANDBOX_LOGIC_EXT_INOTIFY_NOTIFY_H_
#define _DAINTY_SANDBOX_LOGIC_EXT_INOTIFY_NOTIFY_H_

#include "dainty_base.h"
#include "dainty_sandbox_err.h"

namespace dainty
{
namespace sandbox
{
namespace logic_inotify_ext
{
///////////////////////////////////////////////////////////////////////////////

  using t_err = err::t_err;

  using base::types::t_void;

///////////////////////////////////////////////////////////////////////////////

  class t_logic_inotify_ext_notify {
  public:
    virtual ~t_logic_inotify_ext_notify() { }

    virtual t_void notify_inotify_start(t_err) noexcept = 0;
    virtual t_void notify_inotify_cleanup()    noexcept = 0;
  };

///////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
