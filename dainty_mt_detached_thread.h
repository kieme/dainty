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
#include "dainty_mt_err.h"

namespace dainty
{
namespace mt
{
namespace detached_thread
{
  using named::t_void;
  using named::p_void;
  using named::t_bool;
  using named::P_cstr;
  using named::t_prefix;
  using named::t_validity;
  using named::VALID;
  using named::INVALID;
  using err::t_err;

///////////////////////////////////////////////////////////////////////////////

  class t_thread;
  using r_thread = t_prefix<t_thread>::r_;
  using x_thread = t_prefix<t_thread>::x_;
  using R_thread = t_prefix<t_thread>::R_;

  class t_thread {
  public:
    class t_logic {
    public:
      using t_err = oops::t_oops<>;

      virtual ~t_logic() { }
      virtual t_void update (t_err, os::r_pthread_attr) noexcept; //XXX
      virtual t_void prepare(t_err) noexcept;
      virtual t_void run    ()      noexcept = 0;
    };

    using p_logic = named::t_prefix<t_logic>::p_;

    t_thread(t_err, P_cstr name, p_logic, t_bool del_logic) noexcept;

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
