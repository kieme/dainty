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

#include "dainty_os_threading.h"
#include "dainty_mt_chained_queue.h"

namespace dainty
{
namespace mt
{
namespace chained_queue
{
  using named::t_n_;
  using namespace dainty::os::threading;
  using namespace dainty::os::fdbased;

  using t_queue = container::chained_queue::t_chained_queue<t_any>;

///////////////////////////////////////////////////////////////////////////////

  class t_impl_ {
  public:
    using t_chain = t_queue::t_chain;
    using t_logic = t_processor::t_logic;

    t_impl_(t_err& err, t_n max) noexcept
      : queue_{max}, eventfd_(err, t_n{0}), lock1_{err}, lock2_{err} {
    }

    operator t_validity() const noexcept {
      return (queue_   == VALID &&
              eventfd_ == VALID &&
              lock1_   == VALID &&
              lock2_   == VALID) ? VALID : INVALID;
    }

    t_validity process(t_err& err, t_logic& logic, t_n max) noexcept {
      for (t_n_ n = get(max); !err && n; --n) {
        t_eventfd::t_value value = 0;
        if (eventfd_.read(err, value) == VALID) {
          t_chain chain;
          <% auto scope = lock2_.make_locked_scope(err);
            if (scope == VALID)
              chain = queue_.remove(err);
          %>
          if (get(chain.cnt)) {
            logic.async_process(chain);
            <% auto scope = lock1_.make_locked_scope(err);
              if (scope == VALID)
                queue_.release(err, chain);
            %>
          }
        }
      }
      return !err ? VALID : INVALID;
    }

    t_validity process_available(t_err& err, t_logic& logic) noexcept {
      t_chain chain;
      <% auto scope = lock2_.make_locked_scope(err);
        if (scope == VALID) {
          chain = queue_.remove(err);
          if (get(chain.cnt)) {
            t_eventfd::t_value value = 0;
            eventfd_.read(err, value);
          }
        }
      %>
      if (get(chain.cnt)) {
        logic.async_process(chain);
        <% auto scope = lock1_.make_locked_scope(err);
          if (scope == VALID)
            queue_.release(err, chain);
        %>
      }
      return !err ? VALID : INVALID;
    }

    t_chain acquire(t_user, t_n n) noexcept {
      <% auto scope = lock1_.make_locked_scope();
        if (scope == VALID)
          return queue_.acquire(n);
      %>
      return {};
    }

    t_chain acquire(t_err& err, t_user, t_n n) noexcept {
      <% auto scope = lock1_.make_locked_scope(err);
        return queue_.acquire(err, n);
      %>
      return {};
    }

    t_validity insert(t_user, t_chain& chain) noexcept {
      t_validity validity = INVALID;
      if (get(chain.cnt)) {
        t_bool send = false;
        <% auto scope = lock2_.make_locked_scope();
          if (scope == VALID) {
            send = queue_.is_empty();
            queue_.insert(chain);
          }
        %>
        if (send) {
          t_eventfd::t_value value = 1;
          validity = eventfd_.write(value) == VALID ? VALID : INVALID;
        }
      }
      return validity;
    }

    t_validity insert(t_err& err, t_user, t_chain& chain) noexcept {
      if (get(chain.cnt)) {
        t_bool send = false;
        <% auto scope = lock2_.make_locked_scope(err);
          send = queue_.is_empty();
          queue_.insert(err, chain);
        %>
        if (send) {
          t_eventfd::t_value value = 1;
          eventfd_.write(err, value);
        }
      } else
        err = E_XXX;
      return !err ? VALID : INVALID;
    }

    t_fd get_fd() const noexcept {
      return eventfd_.get_fd();
    }

    t_client make_client(t_user user) noexcept {
      // NOTE: future, we have information on clients.
      return {this, user};
    }

    t_client make_client(t_err& err, t_user user) noexcept {
      // NOTE: future, we have information on clients.
      return {this, user};
    }

  private:
    t_queue      queue_;
    t_eventfd    eventfd_;
    t_mutex_lock lock1_;
    t_mutex_lock lock2_;
  };

///////////////////////////////////////////////////////////////////////////////

  t_client::t_chain t_client::acquire(t_n cnt) noexcept {
    if (impl_)
      return impl_->acquire(user_, cnt);
    return {};
  }

  t_client::t_chain t_client::acquire(t_err err, t_n cnt) noexcept {
    T_ERR_GUARD(err) {
      if (impl_ && *impl_ == VALID)
        return impl_->acquire(err, user_, cnt);
      err = E_XXX;
    }
    return {};
  }

  t_validity t_client::insert(t_chain chain) noexcept {
    if (impl_)
      return impl_->insert(user_, chain);
    return INVALID;
  }

  t_validity t_client::insert(t_err err, t_chain chain) noexcept {
    T_ERR_GUARD(err) {
      if (impl_ && *impl_ == VALID)
        return impl_->insert(err, user_, chain);
      err = E_XXX;
    }
    return INVALID;
  }

///////////////////////////////////////////////////////////////////////////////

  t_processor::t_processor(t_err err, t_n max) noexcept {
    T_ERR_GUARD(err) {
      impl_ = new t_impl_(err, max);
      if (impl_) {
        if (err) {
          delete impl_;
          impl_ = nullptr;
        }
      } else
        err = E_XXX;
    }
  }

  t_processor::~t_processor() {
    if (impl_) {
      delete impl_;
    }
  }

  t_client t_processor::make_client(t_user user) noexcept {
    if (impl_)
      return impl_->make_client(user);
    return {};
  }

  t_client t_processor::make_client(t_err err, t_user user) noexcept {
    T_ERR_GUARD(err) {
      if (impl_ && *impl_ == VALID)
        return impl_->make_client(err, user);
      err = E_XXX;
    }
    return {};
  }

  t_validity t_processor::process(t_err err, t_logic& logic,
                                  t_n max) noexcept {
    T_ERR_GUARD(err) {
      if (impl_ && *impl_ == VALID)
        return impl_->process(err, logic, max);
      err = E_XXX;
    }
    return INVALID;
  }

  t_validity t_processor::process_available(t_err err,
                                            t_logic& logic) noexcept {
    T_ERR_GUARD(err) {
      if (impl_ && *impl_ == VALID)
        return impl_->process_available(err, logic);
      err = E_XXX;
    }
    return INVALID;
  }

  t_fd t_processor::get_fd() const noexcept {
    if (impl_)
      return impl_->get_fd();
    return BAD_FD;
  }

///////////////////////////////////////////////////////////////////////////////
}
}
}
