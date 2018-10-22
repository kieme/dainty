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
#include "dainty_os_threading.h"
#include "dainty_mt_condvar_chained_queue.h"

namespace dainty
{
namespace mt
{
namespace condvar_chained_queue
{
  using err::r_err;
  using named::t_n_;
  using namespace os::threading;
  using t_queue = container::chained_queue::t_chained_queue<t_any>;

///////////////////////////////////////////////////////////////////////////////

  class t_impl_ {
  public:
    using t_chain = t_queue::t_chain;
    using r_logic = t_processor::r_logic;

    t_impl_(r_err err, t_n max) noexcept :
      queue_{err, max}, cond_{err}, lock1_{err}, lock2_{err} {
      if (queue_ == VALID && cond_ == VALID && lock1_ == VALID &&
          lock2_ == VALID)
        valid_ = VALID;
    }

    operator t_validity() const noexcept {
      return valid_;
    }

    t_void process(r_err err, r_logic logic, t_n max) noexcept {
      for (t_n_ n = get(max); !err && n; --n) {
        t_chain chain;
        <% auto scope = lock2_.make_locked_scope(err);
          cond_.wait(err, lock2_);
          chain = queue_.remove(err);
        %>

        if (get(chain.cnt)) {
          logic.async_process(chain);
          <% auto scope = lock1_.make_locked_scope(err);
            queue_.release(err, chain);
          %>
        }
      }
    }

    t_chain acquire(t_user, t_n n) noexcept {
      <% auto scope = lock1_.make_locked_scope();
        if (scope == VALID)
          return queue_.acquire(n);
      %>
      return {};
    }

    t_chain acquire(r_err err, t_user, t_n n) noexcept {
      <% auto scope = lock1_.make_locked_scope(err);
        return queue_.acquire(err, n);
      %>
      return {};
    }

    t_errn insert(t_user, t_chain& chain) noexcept {
      t_errn errn{-1};
      if (get(chain.cnt)) {
        t_bool send = false;
        <% auto scope = lock2_.make_locked_scope();
          if (scope == VALID) {
            send = queue_.is_empty();
            queue_.insert(chain);
            set(errn) = 0;
          }
        %>
        if (send && errn == VALID)
          errn = cond_.signal();
      }
      return errn;
    }

    t_void insert(r_err err, t_user, t_chain& chain) noexcept {
      if (get(chain.cnt)) {
        t_bool send = false;
        <% auto scope = lock2_.make_locked_scope(err);
          send = queue_.is_empty();
          queue_.insert(err, chain);
        %>
        if (send)
          cond_.signal(err);
      } else
        err = err::E_XXX;
    }

    t_client make_client(t_user user) noexcept {
      // NOTE: future, we have information on clients.
      return {this, user};
    }

    t_client make_client(r_err, t_user user) noexcept {
      // NOTE: future, we have information on clients.
      return {this, user};
    }

  private:
    t_validity   valid_ = INVALID;
    t_queue      queue_;
    t_cond_var   cond_;
    t_mutex_lock lock1_;
    t_mutex_lock lock2_;
  };

///////////////////////////////////////////////////////////////////////////////

  t_client::t_chain t_client::acquire(t_n cnt) noexcept {
    if (impl_ == VALID && *impl_ == VALID)
      return impl_->acquire(user_, cnt);
    return {};
  }

  t_client::t_chain t_client::acquire(t_err err, t_n cnt) noexcept {
    ERR_GUARD(err) {
      if (impl_ == VALID && *impl_ == VALID)
        return impl_->acquire(err, user_, cnt);
      err = err::E_XXX;
    }
    return {};
  }

  t_errn t_client::insert(t_chain chain) noexcept {
    if (impl_ == VALID && *impl_ == VALID)
      return impl_->insert(user_, chain);
    return t_errn{-1};
  }

  t_void t_client::insert(t_err err, t_chain chain) noexcept {
    ERR_GUARD(err) {
      if (impl_ == VALID && *impl_ == VALID)
        impl_->insert(err, user_, chain);
      else
        err = err::E_XXX;
    }
  }

///////////////////////////////////////////////////////////////////////////////

  t_processor::t_processor(t_err err, t_n max) noexcept {
    ERR_GUARD(err) {
      impl_ = new t_impl_(err, max);
      if (impl_ == VALID) {
        if (err)
          impl_.clear();
      } else
        err = err::E_XXX;
    }
  }

  t_processor::~t_processor() {
    impl_.clear();
  }

  t_client t_processor::make_client(t_user user) noexcept {
    if (impl_ == VALID && *impl_ == VALID)
      return impl_->make_client(user);
    return {};
  }

  t_client t_processor::make_client(t_err err, t_user user) noexcept {
    ERR_GUARD(err) {
      if (impl_ == VALID && *impl_ == VALID)
        return impl_->make_client(err, user);
      err = err::E_XXX;
    }
    return {};
  }

  t_void t_processor::process(t_err err, r_logic logic, t_n max) noexcept {
    ERR_GUARD(err) {
      if (impl_ == VALID && *impl_ == VALID)
        impl_->process(err, logic, max);
      else
        err = err::E_XXX;
    }
  }

///////////////////////////////////////////////////////////////////////////////
}
}
}
