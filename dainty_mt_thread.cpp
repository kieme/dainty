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

#include <utility>
#include "dainty_mt_thread.h"

namespace dainty
{
namespace mt
{
namespace thread
{
  using namespace dainty::os;
  using threading::t_mutex_lock;
  using threading::t_cond_var;
  using t_os_thread = threading::t_thread;
  using t_logic_ptr = t_thread::t_logic_ptr;

///////////////////////////////////////////////////////////////////////////////

  namespace
  {
    struct t_data_ {
      t_err        err_;
      P_cstr       name_;
      t_logic_ptr  logic_;
      t_bool       ready_;
      t_mutex_lock lock_;
      t_cond_var   cond_;

      t_data_(t_err err, P_cstr name, t_logic_ptr&& logic) noexcept
        : err_(err), name_(name), logic_(std::move(logic)), ready_(false) {
      }
    };

///////////////////////////////////////////////////////////////////////////////

    p_void start_(p_void arg) {
      t_data_* data = reinterpret_cast<t_data_*>(arg);

      t_logic_ptr logic{std::move(data->logic_)};
      t_err& err = data->err_;

      t_os_thread::set_name(err, t_os_thread::get_self(), data->name_);
      <% auto scope = data->lock_.make_locked_scope(err);
        logic->prepare(err);
        data->ready_ = !err;
      %>

      p_void returnarg = nullptr;
      if (!err) {
        data->cond_.signal(err);
        returnarg = logic->run();
      } else
        data->cond_.signal();

      /* note: signal will invalidate data */

      return returnarg;
    }
  }

///////////////////////////////////////////////////////////////////////////////

  t_void t_thread::t_logic::update(t_err err,
                                   os::r_pthread_attr attr) noexcept {
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

  t_thread::t_thread(t_err err, P_cstr name, t_logic_ptr logic) noexcept {
    ERR_GUARD(err) {
      t_data_ data{err, name, std::move(logic)};
      if (get(name) && data.logic_ && data.cond_ == VALID &&
          data.lock_ == VALID) {
        os::t_pthread_attr attr;
        if ((!::pthread_attr_init(&attr))) { //XXX
          data.logic_->update(err, attr);
          thread_.create(err, start_, &data, attr);
          <% auto scope = data.lock_.make_locked_scope(err);
            while (!err && !data.ready_)
              data.cond_.wait(err, data.lock_);
          %>
        }
      }
    }
  }

  t_thread::operator t_validity() const noexcept {
    return thread_;
  }

  t_errn t_thread::join() noexcept {
    return thread_.join();
  }

  t_void t_thread::join(t_err err) noexcept {
    thread_.join(err);
  }

  t_errn t_thread::join(p_void& arg) noexcept {
    return thread_.join(arg);
  }

  t_void t_thread::join(t_err err, p_void& arg) noexcept {
    thread_.join(err, arg);
  }

///////////////////////////////////////////////////////////////////////////////
}
}
}

