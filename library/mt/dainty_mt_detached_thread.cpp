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
    t_valid join();
    t_valid join(t_err);


 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.

******************************************************************************/

#include "dainty_mt_detached_thread.h"

namespace dainty
{
namespace mt
{
namespace detached_thread
{
  using err::r_err;
  using namespace dainty::os;
  using dainty::os::threading::t_mutex_lock;
  using dainty::os::threading::t_cond_var;
  using t_os_thread = dainty::os::threading::t_thread;

///////////////////////////////////////////////////////////////////////////////

  namespace
  {
    struct t_data_ {
      t_err        err_;
      P_cstr       name_;
      p_void       logic_;
      t_bool       del_logic_;
      t_bool       ready_;
      t_mutex_lock lock_;
      t_cond_var   cond_;

      t_data_(t_err err, P_cstr name, p_void logic, t_bool del_logic) noexcept
        : err_(err), name_(name), logic_(logic), del_logic_(del_logic),
          ready_(false) {
      }
    };
    using p_data_ = t_prefix<t_data_>::p_;

///////////////////////////////////////////////////////////////////////////////

    p_void start_(p_void arg) {
      using p_logic = t_thread::p_logic;
      p_data_ data = reinterpret_cast<p_data_>(arg);

      r_err   err       = data->err_;
      p_logic logic     = reinterpret_cast<p_logic>(data->logic_);
      t_bool  del_logic = data->del_logic_;

      t_os_thread::set_name(err, t_os_thread::get_self(), data->name_);
      <% auto scope = data->lock_.make_locked_scope(err);
        logic->prepare(err);
        data->ready_ = !err;
      %>

      if (!err) {
        data->cond_.signal(err);
        if (!err)
          logic->run();
      } else
        data->cond_.signal();

      /* note: signal will invalidate data */

      if (del_logic)
        delete logic;

      return nullptr;
    }
  }

///////////////////////////////////////////////////////////////////////////////

  t_void t_thread::t_logic::update(t_err err,
                                   ::pthread_attr_t& attr) noexcept {
    ERR_GUARD(err) {
      if ((::pthread_attr_setstacksize   (&attr, (128*1024)))             ||
          (::pthread_attr_setguardsize   (&attr, (4 * 1024)))             ||
          (::pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED)) ||
          (::pthread_attr_setschedpolicy (&attr, SCHED_OTHER)))
        err = err::E_XXX;
    }
  }

  t_void t_thread::t_logic::prepare(t_err err) noexcept {
    ERR_GUARD(err) {
    }
  }

  t_thread::t_thread(t_err err, P_cstr name, p_logic logic,
                     t_bool del_logic) noexcept {
    ERR_GUARD(err) {
      t_data_ data{err, name, logic, del_logic};
      if (get(name) && logic && data.cond_ == VALID && data.lock_ == VALID) {
        pthread_attr_t attr;
        if ((!::pthread_attr_init(&attr)) &&
            (!::pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED))) {
          logic->update(err, attr);
          thread_.create(err, start_, &data, attr);
          <% auto scope = data.lock_.make_locked_scope(err);
             while (!err && !data.ready_)
               data.cond_.wait(err, data.lock_);
          %>
        }
      }
    }
  }

///////////////////////////////////////////////////////////////////////////////
}
}
}

