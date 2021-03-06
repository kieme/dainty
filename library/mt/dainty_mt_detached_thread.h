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

#ifndef _DAINTY_MT_DETACHED_THREAD_H_
#define _DAINTY_MT_DETACHED_THREAD_H_

#include "dainty_os_threading.h"
#include "dainty_container_ptr.h"
#include "dainty_mt_err.h"

namespace dainty
{
namespace mt
{
namespace detached_thread
{
///////////////////////////////////////////////////////////////////////////////

  using err::t_err;

  using base::types::t_prefix;
  using base::types::t_void;
  using base::types::p_void;
  using base::types::t_bool;

  using base::specific::P_cstr;
  using base::specific::t_prefix;
  using base::specific::t_validity;
  using base::specific::VALID;
  using base::specific::INVALID;

  using container::ptr::t_passable_ptr;

///////////////////////////////////////////////////////////////////////////////

  class t_thread;
  using r_thread = t_prefix<t_thread>::r_;
  using x_thread = t_prefix<t_thread>::x_;
  using R_thread = t_prefix<t_thread>::R_;

  class t_thread {
  public:
    class t_logic {
    public:
      using t_err          = oops::t_oops<>;
      using r_pthread_attr = os::r_pthread_attr;

      virtual ~t_logic() { }
      virtual t_void update (t_err, r_pthread_attr) noexcept;
      virtual t_void prepare(t_err) noexcept;
      virtual t_void run    ()      noexcept = 0;
    };

    using t_logic_ptr = t_passable_ptr<t_logic>;
    using x_logic_ptr = t_prefix<t_logic_ptr>::x_;

    t_thread(t_err, P_cstr name, x_logic_ptr) noexcept;

    t_thread(R_thread)           = delete;
    t_thread(x_thread)           = delete;
    r_thread operator=(R_thread) = delete;
    r_thread operator=(x_thread) = delete;

  private:
    os::threading::t_thread thread_;
  };

///////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
