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


#include "dainty_base_ptr.h"
#include "dainty_container_ptr.h"
#include "dainty_container_list.h"
#include "dainty_sandbox_logic.h"

///////////////////////////////////////////////////////////////////////////////

namespace dainty
{
namespace sandbox
{
///////////////////////////////////////////////////////////////////////////////

  using base::types::t_prefix;

  using base::specific::t_n;
  using base::specific::t_ix;
  using base::specific::t_validity;
  using base::specific::VALID;
  using base::specific::INVALID;

  using container::ptr::t_passable_ptr;

///////////////////////////////////////////////////////////////////////////////

  enum  t_thread_name_tag {};
  using t_thread_name    = t_string<t_thread_name_tag>;
  using R_thread_name    = t_prefix<t_thread_name>::R_;
  using T_thread_name    = t_prefix<t_thread_name>::T_;

  enum t_thread_control { IN_CURRENT_THREAD, IN_NEW_THREAD };

  class t_thread_params { }; // XXX
  using R_thread_params = t_prefix<t_thread_params>::R_;
  using T_thread_params = t_prefix<t_thread_params>::T_;

///////////////////////////////////////////////////////////////////////////////

  using t_logic_ptr = t_passable_ptr<t_logic>;
  using x_logic_ptr = t_prefix<t_logic_ptr>::x_;
  using p_logic_ptr = t_prefix<t_logic_ptr>::p_;
  using P_logic_ptr = t_prefix<t_logic_ptr>::P_;

///////////////////////////////////////////////////////////////////////////////

  class t_logic_ptrlist;
  using r_logic_ptrlist = t_prefix<t_logic_ptrlist>::r_;
  using R_logic_ptrlist = t_prefix<t_logic_ptrlist>::R_;
  using x_logic_ptrlist = t_prefix<t_logic_ptrlist>::x_;

  class t_logic_ptrlist {
  public:
    t_logic_ptrlist(t_err, t_n max);
    t_logic_ptrlist(x_logic_ptrlist);

    t_logic_ptrlist(R_logic_ptrlist) = delete;
    r_logic_ptrlist operator=(R_logic_ptrlist) = delete;
    r_logic_ptrlist operator=(x_logic_ptrlist) = delete;

    t_void      push_back(t_err, x_logic_ptr) noexcept;
    t_n         get_size() const noexcept;
    p_logic_ptr get(t_ix)        noexcept;
    P_logic_ptr get(t_ix)  const noexcept;

  private:
    using t_list_ = container::list::t_list<t_logic_ptr>;
    t_list_ list_;
  };

///////////////////////////////////////////////////////////////////////////////

  class t_thread;
  using T_thread = t_prefix<t_thread>::T_;
  using R_thread = t_prefix<t_thread>::R_;
  using r_thread = t_prefix<t_thread>::r_;
  using x_thread = t_prefix<t_thread>::x_;
  using p_thread = t_prefix<t_thread>::p_;
  using P_thread = t_prefix<t_thread>::P_;

  class t_thread {
  public:
    using R_name    = R_thread_name;
    using t_params  = t_thread_params;
    using R_params  = R_thread_params;
    using x_ptr     = x_logic_ptr;
    using x_ptrlist = x_logic_ptrlist;

    t_thread(t_err, R_name, x_ptr,     R_params = t_params()) noexcept;
    t_thread(t_err, R_name, x_ptrlist, R_params = t_params()) noexcept;

    t_thread(R_thread) = delete;
    t_thread(x_thread) = delete;

    r_thread operator=(R_thread) = delete;
    r_thread operator=(x_thread) = delete;
  };

///////////////////////////////////////////////////////////////////////////////

  class t_main;
  using T_main = t_prefix<t_main>::T_;
  using R_main = t_prefix<t_main>::R_;
  using r_main = t_prefix<t_main>::r_;
  using x_main = t_prefix<t_main>::x_;
  using p_main = t_prefix<t_main>::p_;
  using P_main = t_prefix<t_main>::P_;

  class t_main {
  public:
    using R_name    = R_thread_name;
    using t_params  = t_thread_params;
    using R_params  = R_thread_params;
    using x_ptr     = x_logic_ptr;
    using x_ptrlist = x_logic_ptrlist;

    t_main(t_err, R_name, x_ptr    , R_params = t_params()) noexcept;
    t_main(t_err, R_name, x_ptrlist, R_params = t_params()) noexcept;

    t_main(R_main) = delete;
    t_main(x_main) = delete;

    r_main operator=(R_main) = delete;
    r_main operator=(x_main) = delete;
  };

///////////////////////////////////////////////////////////////////////////////

  class t_thread_of_control_;
  using p_thread_of_control_ = t_prefix<t_thread_of_control_>::p_;

///////////////////////////////////////////////////////////////////////////////

  class t_sandbox;
  using T_sandbox = t_prefix<t_sandbox>::T_;
  using R_sandbox = t_prefix<t_sandbox>::R_;
  using r_sandbox = t_prefix<t_sandbox>::r_;
  using x_sandbox = t_prefix<t_sandbox>::x_;
  using p_sandbox = t_prefix<t_sandbox>::p_;
  using P_sandbox = t_prefix<t_sandbox>::P_;

  class t_sandbox {
  public:
    using R_name    = R_thread_name;
    using t_params  = t_thread_params;
    using R_params  = R_thread_params;
    using t_control = t_thread_control;
    using x_ptr     = x_logic_ptr;
    using x_ptrlist = x_logic_ptrlist;

    t_sandbox(t_err, R_name, x_ptr,     t_control = IN_CURRENT_THREAD,
                                        R_params  = t_params()) noexcept;
    t_sandbox(t_err, R_name, x_ptrlist, t_control = IN_CURRENT_THREAD,
                                        R_params  = t_params()) noexcept;
   ~t_sandbox();

    t_sandbox(R_sandbox) = delete;
    t_sandbox(x_sandbox) = delete;

    r_sandbox operator=(R_sandbox) = delete;
    r_sandbox operator=(x_sandbox) = delete;

  private:
    enum t_thread_of_control_tag_ { };
    base::ptr::t_ptr<t_thread_of_control_,
                      t_thread_of_control_tag_,
                      base::ptr::t_deleter> thread_of_control_;
  };

///////////////////////////////////////////////////////////////////////////////
}
}

#endif
