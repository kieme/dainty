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

#ifndef _DAINTY_SANDBOX_LOGIC_EXT_MESSENGER_NOTIFY_H_
#define _DAINTY_SANDBOX_LOGIC_EXT_MESSENGER_NOTIFY_H_

#include "dainty_named.h"
#include "dainty_sandbox_logic_ext_messenger_api.h"

namespace dainty
{
namespace sandbox
{
namespace logic_messenger_ext
{

///////////////////////////////////////////////////////////////////////////////

  class t_logic_messenger_ext_notify {
  public:
    using t_messenger_name  = logic_messenger_ext::t_messenger_name;
    using R_messenger_name  = logic_messenger_ext::R_messenger_name;
    using t_messenger_state = logic_messenger_ext::t_messenger_state;
    using t_messenger_prio  = logic_messenger_ext::t_messenger_prio;
    using t_messenger_user  = logic_messenger_ext::t_messenger_user;
    using t_messenger_msg   = logic_messenger_ext::t_messenger_msg;
    using x_messenger_msg   = logic_messenger_ext::x_messenger_msg;

    virtual ~t_logic_messenger_ext_notify() { }

    virtual t_void notify_messenger_start(t_err) noexcept = 0;
    virtual t_void notify_messenger_cleanup()    noexcept = 0;

    virtual t_void notify_messenger_state(t_messenger_state,
                                          R_messenger_name,
                                          t_messenger_key,
                                          t_messenger_prio,
                                          t_messenger_user)      noexcept = 0;
    virtual t_void notify_messenger_msg        (x_messenger_msg) noexcept = 0;
    virtual t_void notify_messenger_failed_send(x_messenger_msg) noexcept = 0;

  };

///////////////////////////////////////////////////////////////////////////////
}
}
}


#endif
