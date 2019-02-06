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

#ifndef _DAINTY_SANDBOX_IMPL_H_
#define _DAINTY_SANDBOX_IMPL_H_

#include "dainty_named.h"
#include "dainty_mt_detached_thread.h"
#include "dainty_sandbox.h"

///////////////////////////////////////////////////////////////////////////////

namespace dainty
{
namespace sandbox
{
  using t_thread_           = mt::detached_thread::t_thread;
  using t_thread_logic_     = t_thread_::t_logic;
  using t_thread_logic_ptr_ = t_thread_::t_logic_ptr;

///////////////////////////////////////////////////////////////////////////////

  class t_impl_ : public t_thread_logic_ {
    using base = t_thread_logic_;
  public:
    t_impl_(t_err, R_thread_name) noexcept;

    virtual t_void update(base::t_err,
                          base::r_pthread_attr) noexcept override final;

    virtual t_void prepare(base::t_err) noexcept override final;

    t_void start_extensions(t_err, p_logic) noexcept;
    t_void cleanup_extensions(p_logic) noexcept;

    // event_loop
  private:
    t_thread_name name_;
  };

///////////////////////////////////////////////////////////////////////////////

  class t_single_impl_ : public t_impl_ {
  public:
    t_single_impl_(t_err, R_thread_name, x_logic_ptr) noexcept;

    // api that is open for t_logic

    // api that allow extensions - might not need it
    virtual t_void run() noexcept override final;

  private:
    t_logic_ptr logic_;
  };

///////////////////////////////////////////////////////////////////////////////

  class t_shared_impl_ : public t_impl_ { // don't support now
  public:
    t_shared_impl_(t_err, R_thread_name, x_logic_ptrlist) noexcept;

    // api that allow extensions - might not need it
    virtual t_void run() noexcept override final;

  private:
    t_logic_ptrlist list_;
  };

///////////////////////////////////////////////////////////////////////////////

}
}

#endif
