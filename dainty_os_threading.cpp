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

#include "dainty_os_clock.h"
#include "dainty_os_threading.h"

namespace dainty
{
namespace os
{
namespace threading
{
  using clock::to_;

///////////////////////////////////////////////////////////////////////////////

  t_mutex_lock::t_mutex_lock() noexcept {
    if (call_pthread_mutex_init(mutex_) == VALID)
      valid_ = VALID;
  }

  t_mutex_lock::t_mutex_lock(t_err err) noexcept {
    ERR_GUARD(err) {
      call_pthread_mutex_init(err, mutex_);
      valid_ = !err ? VALID : INVALID;
    }
  }

  t_mutex_lock::t_mutex_lock(R_pthread_mutexattr attr) noexcept {
    valid_ = call_pthread_mutex_init(mutex_, attr) == VALID ? VALID : INVALID;
  }

  t_mutex_lock::t_mutex_lock(t_err err, R_pthread_mutexattr attr) noexcept {
    ERR_GUARD(err) {
      call_pthread_mutex_init(err, mutex_, attr);
      valid_ = !err ? VALID : INVALID;
    }
  }

  t_mutex_lock::~t_mutex_lock() {
    if (valid_ == VALID)
      call_pthread_mutex_destroy(mutex_);
  }

  t_mutex_lock::t_locked_scope t_mutex_lock::make_locked_scope() noexcept {
    if (valid_ == VALID && call_pthread_mutex_lock(mutex_) == VALID)
      return {this};
    return {nullptr};
  }

  t_mutex_lock::t_locked_scope
      t_mutex_lock::make_locked_scope(t_err err) noexcept {
    ERR_GUARD(err) {
      if (valid_ == VALID) {
        call_pthread_mutex_lock(err, mutex_);
        if (!err)
          return {this};
      } else
        err = err::E_INVALID_INST;
    }
    return {nullptr};
  }

  t_mutex_lock::t_locked_scope
      t_mutex_lock::make_locked_scope(t_err err, t_time time) noexcept {
    ERR_GUARD(err) {
      if (valid_ == VALID) {
        call_pthread_mutex_timedlock(err, mutex_, to_(time));
        if (!err)
          return {this};
      } else
        err = err::E_INVALID_INST;
    }
    return {nullptr};
  }

  t_mutex_lock::t_locked_scope
      t_mutex_lock::make_locked_scope(t_time time) noexcept {
    if (valid_ == VALID &&
        call_pthread_mutex_timedlock(mutex_, to_(time)) == VALID)
      return {this};
    return {nullptr};
  }

  t_mutex_lock::t_locked_scope
      t_mutex_lock::trymake_locked_scope(t_err err) noexcept {
    ERR_GUARD(err) {
      if (valid_ == VALID) {
        call_pthread_mutex_trylock(err, mutex_);
        if (!err)
          return {this};
      } else
        err = err::E_INVALID_INST;
    }
    return {nullptr};
  }

  t_mutex_lock::t_locked_scope t_mutex_lock::trymake_locked_scope() noexcept {
    if (valid_ == VALID && call_pthread_mutex_trylock(mutex_) == VALID)
      return {this};
    return {nullptr};
  }

  t_void t_mutex_lock::enter_scope_(t_locked_scope*) noexcept {
    // can  use for debugging
  }

  t_void t_mutex_lock::leave_scope_(t_locked_scope* scope) noexcept {
    if (*scope == VALID)
      call_pthread_mutex_unlock(mutex_);
    // can  use for debugging
  }

///////////////////////////////////////////////////////////////////////////////

  t_recursive_mutex_lock::t_recursive_mutex_lock() noexcept {
    ::pthread_mutexattr_t attr;
    if (call_pthread_init(attr) == VALID) {
      if (call_pthread_set_recursive(attr) == VALID &&
          call_pthread_mutex_init(mutex_, attr) == VALID)
        valid_ = VALID;
      call_pthread_destroy(attr);
    }
  }

  t_recursive_mutex_lock::t_recursive_mutex_lock(t_err err) noexcept {
    ERR_GUARD(err) {
      ::pthread_mutexattr_t attr;
      call_pthread_init(err, attr);
      call_pthread_set_recursive(err.tag(1), attr);
      call_pthread_mutex_init(err, mutex_, attr);
      if (err.tag() == 1)
        call_pthread_destroy(attr);
      valid_ = !err ? VALID : INVALID;
    }
  }

  t_recursive_mutex_lock::
      t_recursive_mutex_lock(R_pthread_mutexattr attr) noexcept {
    if (call_pthread_is_recursive(attr) &&
        call_pthread_mutex_init(mutex_, attr) == VALID)
      valid_ = VALID;
  }

  t_recursive_mutex_lock::
      t_recursive_mutex_lock(t_err err, R_pthread_mutexattr attr) noexcept {
    ERR_GUARD(err) {
      if (call_pthread_is_recursive(attr)) {
        call_pthread_mutex_init(err, mutex_, attr);
        valid_ = !err ? VALID : INVALID;
      } else
        err = err::E_ATTR_NOT_RECURSIVE;
    }
  }

  t_recursive_mutex_lock::~t_recursive_mutex_lock() {
    if (valid_ == VALID)
      call_pthread_mutex_destroy(mutex_);
  }

  t_recursive_mutex_lock::t_locked_scope
      t_recursive_mutex_lock::make_locked_scope(t_err err) noexcept {
    ERR_GUARD(err) {
      if (valid_ == VALID) {
        call_pthread_mutex_lock(err, mutex_);
        if (!err)
          return {this};
      } else
        err = err::E_INVALID_INST;
    }
    return {nullptr};
  }

  t_recursive_mutex_lock::t_locked_scope
      t_recursive_mutex_lock::make_locked_scope() noexcept {
    if (valid_ == VALID && call_pthread_mutex_lock(mutex_) == VALID)
      return {this};
    return {nullptr};
  }

  t_recursive_mutex_lock::t_locked_scope
      t_recursive_mutex_lock::make_locked_scope(t_err err,
                                                t_time time) noexcept {
    ERR_GUARD(err) {
      if (valid_ == VALID) {
        call_pthread_mutex_timedlock(err, mutex_, to_(time));
        if (!err)
          return {this};
      } else
        err = err::E_INVALID_INST;
    }
    return {nullptr};
  }

  t_recursive_mutex_lock::t_locked_scope
      t_recursive_mutex_lock::make_locked_scope(t_time time) noexcept {
    if (valid_ == VALID &&
        call_pthread_mutex_timedlock(mutex_, to_(time)) == VALID)
      return {this};
    return {nullptr};
  }

  t_recursive_mutex_lock::t_locked_scope
      t_recursive_mutex_lock::trymake_locked_scope(t_err err) noexcept {
    ERR_GUARD(err) {
      if (valid_ == VALID) {
        call_pthread_mutex_trylock(err, mutex_);
        if (!err)
          return {this};
      } else
        err = err::E_INVALID_INST;
    }
    return {nullptr};
  }

  t_recursive_mutex_lock::t_locked_scope
      t_recursive_mutex_lock::trymake_locked_scope() noexcept {
    if (valid_ == VALID && call_pthread_mutex_trylock(mutex_) == VALID)
      return {this};
    return {nullptr};
  }

  t_void t_recursive_mutex_lock::enter_scope_(t_locked_scope*) noexcept {
    // can  use for debugging
  }

  t_void t_recursive_mutex_lock::leave_scope_(t_locked_scope* scope) noexcept {
    if (*scope == VALID)
      call_pthread_mutex_unlock(mutex_);
    // can  use for debugging
  }

///////////////////////////////////////////////////////////////////////////////

  t_cond_var::t_cond_var() noexcept {
    if (call_pthread_cond_init(cond_) == VALID)
      valid_ = VALID;
  }

  t_cond_var::t_cond_var(t_err err) noexcept {
    ERR_GUARD(err) {
      call_pthread_cond_init(err, cond_);
      valid_ = !err ? VALID : INVALID;
    }
  }

  t_cond_var::t_cond_var(R_pthread_condattr attr) noexcept {
    valid_ = call_pthread_cond_init(cond_, attr) == VALID ? VALID : INVALID;
  }

  t_cond_var::t_cond_var(t_err err, R_pthread_condattr attr) noexcept {
    ERR_GUARD(err) {
      call_pthread_cond_init(err, cond_, attr);
      valid_ = !err ? VALID : INVALID;
    }
  }

  t_cond_var::~t_cond_var() {
    if (valid_ == VALID)
      call_pthread_cond_destroy(cond_);
  }

  t_errn t_cond_var::signal() noexcept {
    t_errn errn{-1};
    if (valid_ == VALID)
      errn = call_pthread_cond_signal(cond_);
    return errn;
  }

  t_void t_cond_var::signal(t_err err) noexcept {
    ERR_GUARD(err) {
      if (valid_ == VALID)
        call_pthread_cond_signal(err, cond_);
      else
        err = err::E_INVALID_INST;
    }
  }

  t_errn t_cond_var::broadcast() noexcept {
    t_errn errn{-1};
    if (valid_ == VALID)
      errn = call_pthread_cond_broadcast(cond_);
    return errn;
  }

  t_void t_cond_var::broadcast(t_err err) noexcept {
    ERR_GUARD(err) {
      if (valid_ == VALID)
        call_pthread_cond_broadcast(err, cond_);
      else
        err = err::E_INVALID_INST;
    }
  }

  t_errn t_cond_var::wait_(::pthread_mutex_t& mutex) noexcept {
    t_errn errn{-1};
    if (valid_ == VALID)
      errn = call_pthread_cond_wait(cond_, mutex);
    return errn;
  }

  t_void t_cond_var::wait_(t_err err, r_pthread_mutex mutex) noexcept {
    ERR_GUARD(err) {
      if (valid_ == VALID)
        call_pthread_cond_wait(err, cond_, mutex);
      else
        err = err::E_INVALID_INST;
    }
  }

  t_errn t_cond_var::wait_until_(r_pthread_mutex mutex, t_time time) noexcept {
    t_errn errn{-1};
    if (valid_ == VALID)
      errn = call_pthread_cond_timedwait(cond_, mutex, to_(time));
    return errn;
  }

  t_void t_cond_var::wait_until_(t_err err, r_pthread_mutex mutex,
                                 t_time time) noexcept {
    ERR_GUARD(err) {
      if (valid_ == VALID)
        call_pthread_cond_timedwait(err, cond_, mutex, to_(time));
      else
        err = err::E_INVALID_INST;
    }
  }

///////////////////////////////////////////////////////////////////////////////

  t_monotonic_cond_var::t_monotonic_cond_var() noexcept {
    ::pthread_condattr_t attr;
    if (call_pthread_init(attr) == VALID) {
      if (call_pthread_set_monotonic(attr) == VALID &&
          call_pthread_cond_init(cond_) == VALID)
        valid_ = VALID;
      call_pthread_destroy(attr);
    }
  }

  t_monotonic_cond_var::t_monotonic_cond_var(t_err err) noexcept {
    ERR_GUARD(err) {
      ::pthread_condattr_t attr;
      call_pthread_init(err, attr);
      call_pthread_set_monotonic(err.tag(1), attr);
      call_pthread_cond_init(err.tag(2), cond_, attr);
      if (err && err.tag() > 0)
        call_pthread_destroy(attr);
      valid_ = !err ? VALID : INVALID;
    }
  }

  t_monotonic_cond_var::
      t_monotonic_cond_var(R_pthread_condattr attr) noexcept {
    if (call_pthread_is_monotonic(attr) &&
        call_pthread_cond_init(cond_, attr) == VALID)
      valid_ = VALID;
  }

  t_monotonic_cond_var::
      t_monotonic_cond_var(t_err err,
                           R_pthread_condattr attr) noexcept {
    ERR_GUARD(err) {
      if (call_pthread_is_monotonic(err, attr))
        call_pthread_cond_init(err, cond_, attr);
      else
        err = err::E_ATTR_NOT_MONOTONIC;
      valid_ = !err ? VALID : INVALID;
    }
  }

  t_monotonic_cond_var::~t_monotonic_cond_var() {
    if (valid_ == VALID)
      call_pthread_cond_destroy(cond_);
  }

  t_errn t_monotonic_cond_var::signal() noexcept {
    t_errn errn{-1};
    if (valid_ == VALID)
      errn = call_pthread_cond_signal(cond_);
    return errn;
  }

  t_void t_monotonic_cond_var::signal(t_err err) noexcept {
    ERR_GUARD(err) {
      if (valid_ == VALID)
        call_pthread_cond_signal(err, cond_);
      else
        err = err::E_INVALID_INST;
    }
  }

  t_errn t_monotonic_cond_var::broadcast() noexcept {
    t_errn errn{-1};
    if (valid_ == VALID)
      errn = call_pthread_cond_broadcast(cond_);
    return errn;
  }

  t_void t_monotonic_cond_var::broadcast(t_err err) noexcept {
    ERR_GUARD(err) {
      if (valid_ == VALID)
        call_pthread_cond_broadcast(err, cond_);
      else
        err = err::E_INVALID_INST;
    }
  }

  t_errn t_monotonic_cond_var::wait_(r_pthread_mutex mutex) noexcept {
    t_errn errn{-1};
    if (valid_ == VALID)
      errn = call_pthread_cond_wait(cond_, mutex);
    return errn;
  }

  t_void t_monotonic_cond_var::wait_(t_err err,
                                     r_pthread_mutex mutex) noexcept {
    ERR_GUARD(err) {
      if (valid_ == VALID)
        call_pthread_cond_wait(err, cond_, mutex);
      else
        err = err::E_INVALID_INST;
    }
  }

  t_errn t_monotonic_cond_var::wait_for_(r_pthread_mutex mutex,
                                         t_time time) noexcept {
    t_errn errn{-1};
    if (valid_ == VALID) {
      time += clock::monotonic_now();
      errn = call_pthread_cond_timedwait(cond_, mutex, to_(time));
    }
    return errn;
  }

  t_void t_monotonic_cond_var::wait_for_(t_err err, r_pthread_mutex mutex,
                                         t_time time) noexcept {
    ERR_GUARD(err) {
      if (valid_ == VALID) {
        time += clock::monotonic_now(err);
        call_pthread_cond_timedwait(err, cond_, mutex, to_(time));
      } else
        err = err::E_INVALID_INST;
    }
  }

///////////////////////////////////////////////////////////////////////////////

  t_monotonic_lock::t_monotonic_lock() noexcept {
  }

  t_monotonic_lock::t_monotonic_lock(t_err err) noexcept
    : mutex_{err}, cond_{err} {
  }

  t_monotonic_lock::~t_monotonic_lock() {
    //knows all locked_scope instances - can be used for debugging
  }

  t_monotonic_lock::t_locked_scope
      t_monotonic_lock::make_locked_scope(t_err err) noexcept {
    ERR_GUARD(err) {
      if (*this == VALID) {
        ::pthread_t th = call_pthread_self();
        <% auto scope = mutex_.make_locked_scope(err);
          if (scope == VALID) {
            if (!cnt_) {
              owner_ = th;
              cnt_   = 1;
            } else if (owner_ == th)
              ++cnt_;
            else {
              do {
                cond_.wait(err,  mutex_);
              } while (!err && cnt_);
              if (!err) {
                owner_ = th;
                cnt_ = 1;
              }
            }
            if (!err)
              return {this};
          }
        %>
      } else
        err = err::E_INVALID_INST;
    }
    return {nullptr};
  }

  t_monotonic_lock::t_locked_scope
      t_monotonic_lock::make_locked_scope() noexcept {
    if (*this == VALID) {
      ::pthread_t th = call_pthread_self();
      <% auto scope = mutex_.make_locked_scope();
        if (scope == VALID) {
          t_errn errn{-1};
          if (!cnt_) {
            owner_ = th;
            cnt_   = 1;
          } else if (owner_ == th)
            ++cnt_;
          else {
            do {
              errn = cond_.wait(mutex_);
            } while (errn == VALID && cnt_);
            if (errn == VALID) {
              owner_ = th;
              cnt_ = 1;
            }
          }
          if (errn == VALID)
            return {this};
        }
      %>
    }
    return {nullptr};
  }

  t_monotonic_lock::t_locked_scope
      t_monotonic_lock::trymake_locked_scope(t_err err) noexcept {
    ERR_GUARD(err) {
      if (*this == VALID) {
        ::pthread_t th = call_pthread_self();
        <% auto scope = mutex_.make_locked_scope(err);
          if (scope == VALID) {
            if (!cnt_) {
              owner_ = th;
              cnt_   = 1;
            } else if (owner_ == th)
              ++cnt_;
            else
              return {nullptr};
            return {this};
          }
        %>
      } else
        err = err::E_INVALID_INST;
    }
    return {nullptr};
  }

  t_monotonic_lock::t_locked_scope
      t_monotonic_lock::trymake_locked_scope() noexcept {
    if (*this == VALID) {
      ::pthread_t th = call_pthread_self();
      <% auto scope = mutex_.make_locked_scope();
        if (scope == VALID) {
          if (!cnt_) {
            owner_ = th;
            cnt_   = 1;
          } else if (owner_ == th)
            ++cnt_;
          else
            return {nullptr};
          return {this};
        }
      %>
    }
    return {nullptr};
  }

  t_monotonic_lock::t_locked_scope
    t_monotonic_lock::make_locked_scope(t_err err, t_time time) noexcept {
    ERR_GUARD(err) {
      if (*this == VALID) {
        ::pthread_t th = call_pthread_self();
        <% auto scope = mutex_.make_locked_scope(err);
          if (scope == VALID) {
            if (!cnt_) {
              owner_ = th;
              cnt_   = 1;
            } else if (owner_ == th)
              ++cnt_;
            else {
              do {
                cond_.wait_for(err, mutex_, time);
              } while (!err && cnt_);
              if (!err) {
                owner_ = th;
                cnt_ = 1;
              }
            }
            if (!err)
              return {this};
          }
        %>
      } else
        err = err::E_INVALID_INST;
    }
    return {nullptr};
  }

  t_monotonic_lock::t_locked_scope
    t_monotonic_lock::make_locked_scope(t_time time) noexcept {
    if (*this == VALID) {
      ::pthread_t th = call_pthread_self();
      <% auto scope = mutex_.make_locked_scope();
        if (scope == VALID) {
          t_errn errn{-1};
          if (!cnt_) {
            owner_ = th;
            cnt_   = 1;
          } else if (owner_ == th)
            ++cnt_;
          else {
            do {
              errn = cond_.wait_for(mutex_, time);
            } while (errn == VALID && cnt_);
            if (errn == VALID) {
              owner_ = th;
              cnt_ = 1;
            }
          }
          if (errn == VALID)
            return {this};
        }
      %>
    }
    return {nullptr};
  }

  named::t_void t_monotonic_lock::enter_scope_(t_locked_scope*) noexcept {
    // XXX debugging
  }

  named::t_void t_monotonic_lock::leave_scope_(t_locked_scope* scope) noexcept {
    if (*scope == VALID && *this == VALID) {
      ::pthread_t th = call_pthread_self();
      <% auto scope = mutex_.make_locked_scope();
        if (scope == VALID) {
          if (cnt_ && call_pthread_equal(th, owner_)) {
            --cnt_;
            if (!cnt_)
              cond_.signal();
          }
        }
      %>
    }
  }

///////////////////////////////////////////////////////////////////////////////

  t_thread::t_thread() noexcept {
  }

  t_thread::t_thread(p_run run, p_void arg) noexcept {
    create(run, arg);
  }

  t_thread::t_thread(t_err err, p_run run, p_void arg) noexcept {
    create(err, run, arg);
  }

  t_thread::t_thread(p_run run, p_void arg, R_pthread_attr attr) noexcept {
    create(run, arg, attr);
  }

  t_thread::t_thread(t_err err, p_run run, p_void arg,
                     R_pthread_attr attr) noexcept {
    create(err, run, arg, attr);
  }

  t_thread::~t_thread() {
    if (valid_ == VALID && join_)
      join();
  }

  t_errn t_thread::create(p_run run, p_void arg) noexcept {
    t_errn errn{-1};
    if (valid_ == INVALID) {
      errn = call_pthread_create(thread_, run, arg);
      if (errn == VALID) {
        join_  = true;
        valid_ = VALID;
      }
    }
    return errn;
  }

  t_void t_thread::create(t_err err, p_run run, p_void arg) noexcept {
    ERR_GUARD(err) {
      if (valid_ == INVALID) {
        call_pthread_create(err, thread_, run, arg);
        if (!err) {
          join_  = true;
          valid_ = VALID;
        }
      } else
        err = err::E_VALID_INST;
    }
  }

  t_errn t_thread::create(p_run run, p_void arg,
                          R_pthread_attr attr) noexcept {
    t_errn errn{-1};
    if (valid_ == INVALID) {
      errn = call_pthread_create(thread_, attr, run, arg);
      if (errn == VALID) {
        join_ = !call_pthread_is_detach(attr);
        valid_ = VALID;
      }
    }
    return errn;
  }

  t_void t_thread::create(t_err err, p_run run, p_void arg,
                          R_pthread_attr attr) noexcept {
    ERR_GUARD(err) {
      if (valid_ == INVALID) {
        call_pthread_create(err, thread_, attr, run, arg);
        if (!err) {
          join_  = !call_pthread_is_detach(attr);
          valid_ = VALID;
        }
      } else
        err = err::E_INVALID_INST;
    }
  }

  t_errn t_thread::detach() noexcept {
    t_errn errn{-1};
    if (valid_ == VALID && join_) {
      errn = call_pthread_detach(thread_);
      if (errn == VALID)
        join_ = false;
    }
    return errn;
  }

  t_void t_thread::detach(t_err err) noexcept {
    ERR_GUARD(err) {
      if (valid_ == VALID && join_) {
        call_pthread_detach(err, thread_);
        if (!err)
          join_ = false;
      } else
        err = err::E_INVALID_INST;
    }
  }

  t_errn t_thread::join() noexcept {
    t_errn errn{-1};
    if (valid_ == VALID && join_) {
      errn = call_pthread_join(thread_);
      if (errn == VALID)
        valid_ = INVALID;
    }
    return errn;
  }

  t_void t_thread::join(t_err err) noexcept {
    ERR_GUARD(err) {
      if (valid_ == VALID && join_) {
        call_pthread_join(err, thread_);
        valid_ = !err ? INVALID : VALID;
      } else
        err = err::E_INVALID_INST;
    }
  }

  t_errn t_thread::join(p_void& arg) noexcept {
    t_errn errn{-1};
    if (valid_ == VALID && join_) {
      errn = call_pthread_join(thread_, arg);
      if (errn == VALID)
        valid_ = INVALID;
    }
    return errn;
  }

  t_void t_thread::join(t_err err, p_void& arg) noexcept {
    ERR_GUARD(err) {
      if (valid_ == VALID && join_) {
        call_pthread_join(err, thread_, arg);
        valid_ = !err ? INVALID : VALID;
      } else
        err = err::E_INVALID_INST;
    }
  }

  t_errn t_thread::cancel() noexcept {
    t_errn errn{-1};
    if (valid_ == VALID) {
      errn = call_pthread_cancel(thread_);
      if (errn == VALID)
        valid_ = INVALID;
    }
    return errn;
  }

  t_void t_thread::cancel(t_err err) noexcept {
    ERR_GUARD(err) {
      if (valid_ == VALID) {
        call_pthread_cancel(err, thread_);
        valid_ = !err ? INVALID : VALID;
      } else
        err = err::E_INVALID_INST;
    }
  }

  t_errn t_thread::set_name(P_cstr name) noexcept {
    t_errn errn{-1};
    if (valid_ == VALID)
      errn = set_name(thread_, name);
    return errn;
  }

  t_void t_thread::set_name(t_err err, P_cstr name) noexcept {
    ERR_GUARD(err) {
      if (valid_ == VALID)
        set_name(err, thread_, name);
      else
        err = err::E_INVALID_INST;
    }
  }

  t_errn t_thread::get_name(p_cstr name, t_n len) noexcept {
    t_errn errn{-1};
    if (valid_ == VALID)
      errn = get_name(thread_, name, len);
    return errn;
  }

  t_void t_thread::get_name(t_err err, p_cstr name, t_n len) noexcept {
    ERR_GUARD(err) {
      if (valid_ == VALID)
        get_name(err, thread_, name, len);
      else
        err = err::E_INVALID_INST;
    }
  }

  t_bool t_thread::is_equal(R_pthread pthread) noexcept {
    if (valid_ == VALID)
      return call_pthread_equal(thread_, pthread);
    return false;
  }

  t_bool t_thread::is_equal(t_err err, R_pthread pthread) noexcept {
    ERR_GUARD(err) {
      if (valid_ == VALID)
        return call_pthread_equal(thread_, pthread);
      err = err::E_INVALID_INST;
    }
    return false;
  }

  t_bool t_thread::is_equal(const t_thread& pthread) noexcept {
    return is_equal(pthread.thread_);
  }

  t_bool t_thread::is_equal(t_err err, const t_thread& pthread) noexcept {
    return is_equal(err, pthread.thread_);
  }

  t_pthread t_thread::get_self() noexcept {
    return call_pthread_self();
  }

  t_pthread t_thread::get_self(t_err err) noexcept {
    ERR_GUARD(err) {
      return call_pthread_self();
    }
    return {};
  }

  t_void t_thread::exit(p_void arg) noexcept {
    call_pthread_exit(arg);
  }

  t_void t_thread::exit(t_err err, p_void arg) noexcept {
    call_pthread_exit(err, arg);
  }

  t_errn t_thread::set_name(t_pthread thread, P_cstr name) noexcept {
    return call_pthread_setname_np(thread, name);
  }

  t_void t_thread::set_name(t_err err, t_pthread thread, P_cstr name) noexcept {
    call_pthread_setname_np(err, thread, name);
  }

  t_errn t_thread::get_name(t_pthread thread, p_cstr name, t_n len) noexcept {
    return call_pthread_getname_np(thread, name, len);
  }

  t_void t_thread::get_name(t_err err, t_pthread thread, p_cstr name,
                            t_n len) noexcept {
    call_pthread_getname_np(err, thread, name, len);
  }

///////////////////////////////////////////////////////////////////////////////
}
}
}
