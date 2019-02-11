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

#ifndef _DAINTY_SANDBOX_H_
#define _DAINTY_SANDBOX_H_

#include "dainty_container_ptr.h"
#include "dainty_container_list.h"
#include "dainty_sandbox_logic.h"

///////////////////////////////////////////////////////////////////////////////

namespace dainty
{
namespace sandbox
{
  using named::t_n;
  using named::t_ix;
  using container::ptr::t_passable_ptr;

  enum  t_thread_name_tag {};
  using t_thread_name    = t_string<t_thread_name_tag>;
  using R_thread_name    = t_prefix<t_thread_name>::R_;

  enum t_thread_control { IN_CURRENT_THREAD, IN_NEW_THREAD };

///////////////////////////////////////////////////////////////////////////////

  using t_logic_ptr = t_passable_ptr<t_logic>;
  using x_logic_ptr = t_prefix<t_logic_ptr>::x_;
  using p_logic_ptr = t_prefix<t_logic_ptr>::p_;
  using P_logic_ptr = t_prefix<t_logic_ptr>::P_;

///////////////////////////////////////////////////////////////////////////////

  class t_logic_ptrlist;
  using x_logic_ptrlist = t_prefix<t_logic_ptrlist>::x_;

  class t_logic_ptrlist {
  public:
    t_logic_ptrlist(t_err, t_n max);
    t_logic_ptrlist(t_logic_ptrlist&&);

    t_void      push_back(t_err, x_logic_ptr) noexcept;
    t_n         get_size() const noexcept;
    p_logic_ptr get(t_ix)        noexcept;
    P_logic_ptr get(t_ix)  const noexcept;

  private:
    using t_list_ = container::list::t_list<t_logic_ptr>;
    t_list_ list_;
  };
  using x_logic_ptrlist = t_prefix<t_logic_ptrlist>::x_;

///////////////////////////////////////////////////////////////////////////////

  class t_thread {
  public:
    t_thread(t_err, R_thread_name, x_logic_ptr)     noexcept;
    t_thread(t_err, R_thread_name, x_logic_ptrlist) noexcept;

  private:
     // key
  };

  class t_main {
  public:
    t_main(t_err, R_thread_name, x_logic_ptr)     noexcept;
    t_main(t_err, R_thread_name, x_logic_ptrlist) noexcept;

  private:
     // key
  };

///////////////////////////////////////////////////////////////////////////////

  class t_thread_of_control_;
  using p_thread_of_control_ = t_prefix<t_thread_of_control_>::p_;

///////////////////////////////////////////////////////////////////////////////

  class t_sandbox {
  public:
    t_sandbox(t_err, t_thread_control, R_thread_name, x_logic_ptr)     noexcept;
    t_sandbox(t_err, t_thread_control, R_thread_name, x_logic_ptrlist) noexcept;
   ~t_sandbox();
  private:
    p_thread_of_control_ thread_of_control_ = nullptr;
  };

///////////////////////////////////////////////////////////////////////////////
}
}

#endif
