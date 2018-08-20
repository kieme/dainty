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
#include "dainty_mt_condvar_command.h"

namespace dainty
{
namespace mt
{
namespace condvar_command
{
  using named::t_n_;
  using namespace dainty::os::threading;

///////////////////////////////////////////////////////////////////////////////

  class t_impl_ {
  public:
    using t_logic = t_processor::t_logic;

    t_impl_(t_err& err) noexcept
      : cmdlock_(err), lock_(err), reqcond_(err), ackcond_(err) {
    }

    operator t_validity() const noexcept {
      return (cmdlock_ == VALID &&
              lock_    == VALID &&
              reqcond_ == VALID &&
              ackcond_ == VALID) ?  VALID : INVALID;
    }

    t_validity process(t_err& err, t_logic& logic, t_n max) noexcept {
      for (t_n_ n = get(max); !err && n; --n) {
        <% auto scope = lock_.make_locked_scope(err);
          if (scope == VALID && reqcond_.wait(err, lock_) == VALID) {
            t_command* cmd = cmd_;
            t_user     user = user_;
            if (async_) {
              cmd_ = nullptr;
              ackcond_.signal(err);
              logic.async_process(user, cmd);
            } else {
              if (wait_) {
                logic.process(err, user, *cmd);
                ackcond_.signal(); // err?
              }
              cmd_ = nullptr;
            }
          }
        %>
      }
      return !err ? VALID : INVALID;
    }

    t_validity request(t_user user, t_command& cmd) noexcept {
      t_validity validity = INVALID;
      <% auto scope = cmdlock_.make_locked_scope();
        if (scope == VALID) {
          <% auto scope = lock_.make_locked_scope();
            if (scope == VALID) {
              if (reqcond_.signal() == VALID) {
                user_  = user;
                cmd_   = &cmd;
                async_ = false;
                wait_  = true;
                do {
                  validity = INVALID;
                  if (ackcond_.wait(lock_) == VALID)
                    validity = VALID;
                } while (validity == VALID && cmd_);
                wait_ = false;
              }
            }
          %>
        }
      %>
      return validity;
    }

    t_validity request(t_err& err, t_user user, t_command& cmd) noexcept {
      <% auto scope = cmdlock_.make_locked_scope(err);
        if (scope == VALID) {
          <% auto scope = lock_.make_locked_scope(err);
            if (scope == VALID) {
              if (reqcond_.signal(err) == VALID) {
                user_  = user;
                cmd_   = &cmd;
                async_ = false;
                wait_  = true;
                while (ackcond_.wait(err, lock_) == VALID && cmd_);
                wait_  = false;
              }
            }
          %>
        }
      %>
      return !err ? VALID : INVALID;
    }

    t_validity async_request(t_user user, t_command* cmd) noexcept {
      t_validity validity = INVALID;
      <% auto scope = cmdlock_.make_locked_scope();
        if (scope == VALID) {
          <% auto scope = lock_.make_locked_scope();
            if (scope == VALID) {
              if (reqcond_.signal() == VALID) {
                user_  = user;
                cmd_   = cmd;
                async_ = true;
                do {
                  validity = INVALID;
                  if (ackcond_.wait(lock_) == VALID)
                    validity = VALID;
                } while (validity == VALID && cmd_);
              }
            }
          %>
        }
      %>
      return validity;
    }

    t_validity async_request(t_err& err, t_user user,
                             t_command* cmd) noexcept {
      <% auto scope = cmdlock_.make_locked_scope(err);
        if (scope == VALID) {
          <% auto scope = lock_.make_locked_scope(err);
            if (scope == VALID) {
              if (reqcond_.signal(err) == VALID) {
                user_  = user;
                cmd_   = cmd;
                async_ = true;
                while (ackcond_.wait(err, lock_) == VALID && cmd_);
              }
            }
          %>
        }
      %>
      return !err ? VALID : INVALID;
    }

    t_client make_client(t_user user) noexcept {
      // NOTE: future, we have information on clients.
      return {this, user};
    }

    t_client make_client(t_err&, t_user user) noexcept {
        // NOTE: future, we have information on clients.
      return {this, user};
    }

  private:
    t_mutex_lock cmdlock_;
    t_mutex_lock lock_;
    t_cond_var   reqcond_;
    t_cond_var   ackcond_;
    t_command*   cmd_     = nullptr;
    t_user       user_    = t_user{0L};
    t_bool       async_   = false;
    t_bool       wait_    = false;
  };

///////////////////////////////////////////////////////////////////////////////

  t_validity t_client::request(t_command& cmd) noexcept {
    if (impl_)
      return impl_->request(user_, cmd);
    return INVALID;
  }

  t_validity t_client::request(t_err err, t_command& cmd) noexcept {
    T_ERR_GUARD(err) {
      if (impl_ && *impl_ == VALID)
        return impl_->request(err, user_, cmd);
      err = E_XXX;
    }
    return INVALID;
  }

  t_validity t_client::async_request(t_command* cmd) noexcept {
    if (impl_)
      return impl_->async_request(user_, cmd);
    return INVALID;
  }

  t_validity t_client::async_request(t_err err, t_command* cmd) noexcept {
    T_ERR_GUARD(err) {
      if (impl_ && *impl_ == VALID)
        return impl_->async_request(err, user_, cmd);
      err = E_XXX;
    }
    return INVALID;
  }

///////////////////////////////////////////////////////////////////////////////

  t_processor::t_processor(t_err err) noexcept {
    T_ERR_GUARD(err) {
      impl_ = new t_impl_(err);
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
    if (impl_) { // NOTE: can check to see if clients exists
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

///////////////////////////////////////////////////////////////////////////////
}
}
}
