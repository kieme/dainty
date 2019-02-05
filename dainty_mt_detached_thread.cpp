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

#include "dainty_named_utility.h"
#include "dainty_mt_detached_thread.h"

namespace dainty
{
namespace mt
{
namespace detached_thread
{
  using err::r_err;
  using t_attr_       = os::t_pthread_attr;
  using t_stacksize_  = os::t_pthread_attr_stacksize;
  using t_guardsize_  = os::t_pthread_attr_guardsize;
  using t_mutex_lock_ = os::threading::t_mutex_lock;
  using t_cond_var_   = os::threading::t_cond_var;
  using t_thread_     = os::threading::t_thread;
  using t_logic_ptr   = t_thread::t_logic_ptr;
  using x_logic_ptr   = t_thread::x_logic_ptr;

  using named::utility::x_cast;
  using os::call_pthread_init;
  using os::call_pthread_set_detach;
  using os::call_pthread_set_stacksize;
  using os::call_pthread_set_guardsize;
  using os::call_pthread_set_inheritsched_explicit;
  using os::call_pthread_set_schedpolicy_other;

///////////////////////////////////////////////////////////////////////////////

  namespace
  {
    struct t_data_ {
      t_err         err_;
      P_cstr        name_;
      t_logic_ptr   logic_;
      t_bool        ready_;
      t_mutex_lock_ lock_;
      t_cond_var_   cond_;

      t_data_(t_err err, P_cstr name, x_logic_ptr logic) noexcept
        : err_(err), name_(name), logic_(x_cast(logic)), ready_(false) {
      }
    };
    using p_data_ = t_prefix<t_data_>::p_;

///////////////////////////////////////////////////////////////////////////////

    p_void start_(p_void arg) {
      p_data_ data = reinterpret_cast<p_data_>(arg);

      t_logic_ptr logic{x_cast(data->logic_)};
      r_err err = data->err_;

      t_thread_::set_name(err, t_thread_::get_self(), data->name_);
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

      return nullptr;
    }
  }

///////////////////////////////////////////////////////////////////////////////

  t_void t_thread::t_logic::update(t_err err, r_pthread_attr attr) noexcept {
    ERR_GUARD(err) {
      call_pthread_set_stacksize(err, attr, t_stacksize_{128*1024});
      call_pthread_set_guardsize(err, attr, t_guardsize_{4*1024});
      call_pthread_set_inheritsched_explicit(err, attr);
      call_pthread_set_schedpolicy_other(err, attr);
    }
  }

  t_void t_thread::t_logic::prepare(t_err err) noexcept {
    ERR_GUARD(err) {
    }
  }

  t_thread::t_thread(t_err err, P_cstr name, x_logic_ptr logic) noexcept {
    ERR_GUARD(err) {
      t_data_ data{err, name, x_cast(logic)};
      if (get(name) && data.logic_ && data.cond_ == VALID &&
          data.lock_ == VALID) {
        t_attr_ attr;
        call_pthread_init(err, attr);
        call_pthread_set_detach(err, attr);
        data.logic_->update(err, attr);
        thread_.create(err, start_, &data, attr);
        <% auto scope = data.lock_.make_locked_scope(err);
           while (!err && !data.ready_)
             data.cond_.wait(err, data.lock_);
        %>
      } else
        err = err::E_XXX;
    }
  }

///////////////////////////////////////////////////////////////////////////////
}
}
}

