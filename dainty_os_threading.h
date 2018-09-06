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

#ifndef _DAINTY_OS_THREADING_H_
#define _DAINTY_OS_THREADING_H_

#include "dainty_os_call.h"
#include "dainty_os_clock.h"

namespace dainty
{
namespace os
{
namespace threading
{
  using named::p_void;
  using named::t_void;
  using named::t_bool;
  using named::t_int;
  using named::t_validity;
  using named::t_n;
  using named::p_cstr;
  using named::P_cstr;
  using named::VALID;
  using named::INVALID;
  using clock::t_time;

///////////////////////////////////////////////////////////////////////////////

  template<typename L>
  class t_locked_scope {
  public:
    using t_lock = L;

     t_locked_scope(t_locked_scope&&) noexcept;
    ~t_locked_scope();

    t_locked_scope()                                 = delete;
    t_locked_scope(const t_locked_scope&)            = delete;
    t_locked_scope& operator=(const t_locked_scope&) = delete;
    t_locked_scope& operator=(t_locked_scope&&)      = delete;

    operator t_validity() const noexcept;

  private:
    friend L;
    using p_lock = typename named::t_prefix<t_lock>::p_;
    t_locked_scope(p_lock) noexcept;
    p_lock swap_(p_lock)  noexcept;

    p_lock lock_;
  };

///////////////////////////////////////////////////////////////////////////////

  class t_mutex_lock {
  public:
    using t_locked_scope = threading::t_locked_scope<t_mutex_lock>;

    t_mutex_lock()          noexcept;
    t_mutex_lock(t_err err) noexcept;
    t_mutex_lock(           R_pthread_mutexattr) noexcept;
    t_mutex_lock(t_err err, R_pthread_mutexattr) noexcept;
    ~t_mutex_lock();

    t_mutex_lock(const t_mutex_lock&)            = delete;
    t_mutex_lock(t_mutex_lock&&)                 = delete;
    t_mutex_lock& operator=(const t_mutex_lock&) = delete;
    t_mutex_lock& operator=(t_mutex_lock&&)      = delete;

    operator t_validity() const noexcept;

    t_locked_scope make_locked_scope()      noexcept;
    t_locked_scope make_locked_scope(t_err) noexcept;

    t_locked_scope make_locked_scope(       t_time) noexcept;
    t_locked_scope make_locked_scope(t_err, t_time) noexcept;

    t_locked_scope trymake_locked_scope()      noexcept;
    t_locked_scope trymake_locked_scope(t_err) noexcept;

  private:
    template<typename> friend class threading::t_locked_scope;
    friend class t_cond_var;
    friend class t_monotonic_cond_var;
    t_void enter_scope_(t_locked_scope*) noexcept;
    t_void leave_scope_(t_locked_scope*) noexcept;

    t_pthread_mutex mutex_;
    t_validity      valid_ = INVALID;
  };

///////////////////////////////////////////////////////////////////////////////

  class t_recursive_mutex_lock {
  public:
    using t_locked_scope = threading::t_locked_scope<t_recursive_mutex_lock>;

     t_recursive_mutex_lock()          noexcept;
     t_recursive_mutex_lock(t_err err) noexcept;
     t_recursive_mutex_lock(           R_pthread_mutexattr) noexcept;
     t_recursive_mutex_lock(t_err err, R_pthread_mutexattr) noexcept;
    ~t_recursive_mutex_lock();

    t_recursive_mutex_lock(const t_recursive_mutex_lock&)            = delete;
    t_recursive_mutex_lock(t_recursive_mutex_lock&&)                 = delete;
    t_recursive_mutex_lock& operator=(const t_recursive_mutex_lock&) = delete;
    t_recursive_mutex_lock& operator=(t_recursive_mutex_lock&&)      = delete;

    operator t_validity() const noexcept;

    t_locked_scope make_locked_scope()      noexcept;
    t_locked_scope make_locked_scope(t_err) noexcept;

    t_locked_scope make_locked_scope(       t_time) noexcept;
    t_locked_scope make_locked_scope(t_err, t_time) noexcept;

    t_locked_scope trymake_locked_scope()      noexcept;
    t_locked_scope trymake_locked_scope(t_err) noexcept;

  private:
    template<typename> friend class threading::t_locked_scope;
    friend class t_cond_var;
    friend class t_monotonic_cond_var;
    t_void enter_scope_(t_locked_scope*) noexcept;
    t_void leave_scope_(t_locked_scope*) noexcept;

    t_pthread_mutex mutex_;
    t_validity      valid_ = INVALID;
  };

///////////////////////////////////////////////////////////////////////////////

  class t_cond_var {
  public:
     t_cond_var()          noexcept;
     t_cond_var(t_err err) noexcept;
     t_cond_var(const pthread_condattr_t&)            noexcept;
     t_cond_var(t_err err, const pthread_condattr_t&) noexcept;
    ~t_cond_var();

    t_cond_var(const t_cond_var&)            = delete;
    t_cond_var(t_cond_var&&)                 = delete;
    t_cond_var& operator=(const t_cond_var&) = delete;
    t_cond_var& operator=(t_cond_var&&)      = delete;

    operator t_validity() const noexcept;

    t_errn signal()      noexcept;
    t_void signal(t_err) noexcept;

    t_errn broadcast()      noexcept;
    t_void broadcast(t_err) noexcept;

    t_errn wait(       t_mutex_lock&) noexcept;
    t_void wait(t_err, t_mutex_lock&) noexcept;

    t_errn wait_until(       t_mutex_lock&, t_time) noexcept;
    t_void wait_until(t_err, t_mutex_lock&, t_time) noexcept;

    t_errn wait(       t_recursive_mutex_lock&) noexcept;
    t_void wait(t_err, t_recursive_mutex_lock&) noexcept;

    t_errn wait_until(       t_recursive_mutex_lock&, t_time) noexcept;
    t_void wait_until(t_err, t_recursive_mutex_lock&, t_time) noexcept;

  private:
    t_errn wait_(       r_pthread_mutex) noexcept;
    t_void wait_(t_err, r_pthread_mutex) noexcept;

    t_errn wait_until_(       r_pthread_mutex, t_time) noexcept;
    t_void wait_until_(t_err, r_pthread_mutex, t_time) noexcept;

    t_pthread_cond cond_;
    t_validity     valid_ = INVALID;
  };

///////////////////////////////////////////////////////////////////////////////

  class t_monotonic_cond_var {
  public:
     t_monotonic_cond_var()      noexcept;
     t_monotonic_cond_var(t_err) noexcept;
     t_monotonic_cond_var(       R_pthread_condattr) noexcept;
     t_monotonic_cond_var(t_err, R_pthread_condattr) noexcept;
    ~t_monotonic_cond_var();

    t_monotonic_cond_var(const t_monotonic_cond_var&)            = delete;
    t_monotonic_cond_var(t_monotonic_cond_var&&)                 = delete;
    t_monotonic_cond_var& operator=(const t_monotonic_cond_var&) = delete;
    t_monotonic_cond_var& operator=(t_monotonic_cond_var&&)      = delete;

    operator t_validity() const noexcept;

    t_errn signal()      noexcept;
    t_void signal(t_err) noexcept;

    t_errn broadcast()      noexcept;
    t_void broadcast(t_err) noexcept;

    t_errn wait(       t_mutex_lock&) noexcept;
    t_void wait(t_err, t_mutex_lock&) noexcept;

    t_errn wait_for(       t_mutex_lock&, t_time) noexcept;
    t_void wait_for(t_err, t_mutex_lock&, t_time) noexcept;

    t_errn wait(       t_recursive_mutex_lock&) noexcept;
    t_void wait(t_err, t_recursive_mutex_lock&) noexcept;

    t_errn wait_for(       t_recursive_mutex_lock&, t_time) noexcept;
    t_void wait_for(t_err, t_recursive_mutex_lock&, t_time) noexcept;

  private:
    t_errn wait_(       r_pthread_mutex) noexcept;
    t_void wait_(t_err, r_pthread_mutex) noexcept;

    t_errn wait_for_(       r_pthread_mutex, t_time) noexcept;
    t_void wait_for_(t_err, r_pthread_mutex, t_time) noexcept;

    t_pthread_cond cond_;
    t_validity     valid_ = INVALID;
  };

///////////////////////////////////////////////////////////////////////////////

  class t_monotonic_lock {
  public:
    using t_locked_scope = threading::t_locked_scope<t_monotonic_lock>;

     t_monotonic_lock()          noexcept;
     t_monotonic_lock(t_err err) noexcept;
    ~t_monotonic_lock();

    operator t_validity() const noexcept;

    t_locked_scope make_locked_scope()      noexcept;
    t_locked_scope make_locked_scope(t_err) noexcept;

    t_locked_scope trymake_locked_scope()      noexcept;
    t_locked_scope trymake_locked_scope(t_err) noexcept;

    t_locked_scope make_locked_scope(       t_time) noexcept;
    t_locked_scope make_locked_scope(t_err, t_time) noexcept;

  private:
    template<typename> friend class threading::t_locked_scope;
    t_void enter_scope_(t_locked_scope*) noexcept;
    t_void leave_scope_(t_locked_scope*) noexcept;

    named::t_uint        cnt_    = 0;
    ::pthread_t          owner_;
    t_mutex_lock         mutex_;
    t_monotonic_cond_var cond_;
  };

///////////////////////////////////////////////////////////////////////////////

  class t_thread {
  public:
     t_thread() noexcept;
     t_thread(       p_run, p_void) noexcept;
     t_thread(t_err, p_run, p_void) noexcept;
     t_thread(       p_run, p_void, R_pthread_attr) noexcept;
     t_thread(t_err, p_run, p_void, R_pthread_attr) noexcept;
    ~t_thread();

    t_thread(const t_thread&)            = delete;
    t_thread(t_thread&&)                 = delete;
    t_thread& operator=(const t_thread&) = delete;
    t_thread& operator=(t_thread&&)      = delete;

    operator t_validity() const noexcept;

    t_bool is_joinable() const noexcept;

    t_errn create(       p_run, p_void) noexcept;
    t_void create(t_err, p_run, p_void) noexcept;

    t_errn create(       p_run, p_void, R_pthread_attr) noexcept;
    t_void create(t_err, p_run, p_void, R_pthread_attr) noexcept;

    t_errn detach()      noexcept;
    t_void detach(t_err) noexcept;

    t_errn join()      noexcept;
    t_void join(t_err) noexcept;

    t_errn join(       p_void&) noexcept;
    t_void join(t_err, p_void&) noexcept;

    t_errn cancel()      noexcept;
    t_void cancel(t_err) noexcept;

    t_errn set_name(       P_cstr name) noexcept;
    t_void set_name(t_err, P_cstr name) noexcept;
    t_errn get_name(       p_cstr  name, t_n) noexcept;
    t_void get_name(t_err, p_cstr  name, t_n) noexcept;

    t_bool is_equal(       const t_thread&) noexcept;
    t_bool is_equal(t_err, const t_thread&) noexcept;

    t_bool is_equal(       R_pthread) noexcept;
    t_bool is_equal(t_err, R_pthread) noexcept;

    static t_pthread get_self()      noexcept;
    static t_pthread get_self(t_err) noexcept;

    static t_void exit(       p_void) noexcept;
    static t_void exit(t_err, p_void) noexcept;

    static t_errn set_name(       t_pthread, P_cstr) noexcept;
    static t_void set_name(t_err, t_pthread, P_cstr) noexcept;
    static t_errn get_name(       t_pthread, p_cstr, t_n) noexcept;
    static t_void get_name(t_err, t_pthread, p_cstr, t_n) noexcept;

  private:
    t_pthread   thread_;
    t_validity  valid_ = INVALID;
    t_bool      join_  = true;
  };

///////////////////////////////////////////////////////////////////////////////

  template<typename L>
  inline t_locked_scope<L>::operator t_validity() const noexcept {
    return lock_ ? VALID : INVALID;
  }

  template<typename L>
  inline
  t_locked_scope<L>::t_locked_scope(p_lock lock) noexcept
    : lock_(lock) {
  }

  template<typename L>
  inline
  typename t_locked_scope<L>::p_lock
    t_locked_scope<L>::swap_(p_lock lock) noexcept {
    p_lock tmp = lock_;
    lock_ = lock;
    return tmp;
  }

  template<typename L>
  inline
  t_locked_scope<L>::t_locked_scope(t_locked_scope&& scope) noexcept
    : lock_(scope.swap_(nullptr)) {
    if (lock_)
      lock_->enter_scope_(this);
  }

  template<typename L>
  inline
  t_locked_scope<L>::~t_locked_scope() {
    if (lock_)
      lock_->leave_scope_(this);
  }

///////////////////////////////////////////////////////////////////////////////

  inline
  t_mutex_lock::operator t_validity() const noexcept {
    return valid_;
  }

///////////////////////////////////////////////////////////////////////////////

  inline
  t_recursive_mutex_lock::operator t_validity() const noexcept {
    return valid_;
  }

///////////////////////////////////////////////////////////////////////////////

  inline
  t_monotonic_lock::operator t_validity() const noexcept {
    return mutex_ == VALID && cond_ == VALID ? VALID : INVALID;
  }

///////////////////////////////////////////////////////////////////////////////

  inline
  t_cond_var::operator t_validity() const noexcept {
    return valid_;
  }

  inline
  t_errn t_cond_var::wait(t_mutex_lock& lock) noexcept {
    return wait_(lock.mutex_);
  }

  inline
  t_void t_cond_var::wait(t_err err, t_mutex_lock& lock) noexcept {
    wait_(err, lock.mutex_);
  }

  inline
  t_errn t_cond_var::wait_until(t_mutex_lock& lock, t_time time) noexcept {
    return wait_until_(lock.mutex_, time);
  }

  inline
  t_void t_cond_var::wait_until(t_err err, t_mutex_lock& lock,
                                t_time time) noexcept {
    wait_until_(err, lock.mutex_, time);
  }

  inline
  t_errn t_cond_var::wait(t_recursive_mutex_lock& lock) noexcept {
    return wait_(lock.mutex_);
  }

  inline
  t_void t_cond_var::wait(t_err err, t_recursive_mutex_lock& lock) noexcept {
    wait_(err, lock.mutex_);
  }

  inline
  t_errn t_cond_var::wait_until(t_recursive_mutex_lock& lock,
                                t_time time) noexcept {
    return wait_until_(lock.mutex_, time);
  }

  inline
  t_void t_cond_var::wait_until(t_err err, t_recursive_mutex_lock& lock,
                                t_time time) noexcept {
    wait_until_(err, lock.mutex_, time);
  }

///////////////////////////////////////////////////////////////////////////////

  inline
  t_monotonic_cond_var::operator t_validity() const noexcept {
    return valid_;
  }

  inline
  t_errn t_monotonic_cond_var::wait(t_mutex_lock& lock) noexcept {
    return wait_(lock.mutex_);
  }

  inline
  t_void t_monotonic_cond_var::wait(t_err err, t_mutex_lock& lock) noexcept {
    wait_(err, lock.mutex_);
  }

  inline
  t_errn t_monotonic_cond_var::wait_for(t_mutex_lock& lock,
                                       t_time time) noexcept {
    return wait_for_(lock.mutex_, time);
  }

  inline
  t_void t_monotonic_cond_var::wait_for(t_err err, t_mutex_lock& lock,
                                        t_time time) noexcept {
    wait_for_(err, lock.mutex_, time);
  }

  inline
  t_errn t_monotonic_cond_var::wait(t_recursive_mutex_lock& lock) noexcept {
    return wait_(lock.mutex_);
  }

  inline
  t_void t_monotonic_cond_var::wait(t_err err,
                                    t_recursive_mutex_lock& lock) noexcept {
    wait_(err, lock.mutex_);
  }

  inline
  t_errn t_monotonic_cond_var::wait_for(t_recursive_mutex_lock& lock,
                                         t_time time) noexcept {
    return wait_for_(lock.mutex_, time);
  }

  inline
  t_void t_monotonic_cond_var::wait_for(t_err err,
                                        t_recursive_mutex_lock& lock,
                                        t_time time) noexcept {
    return wait_for_(err, lock.mutex_, time);
  }

///////////////////////////////////////////////////////////////////////////////

  inline
  t_thread::operator t_validity() const noexcept {
    return valid_;
  }

  inline
  t_bool t_thread::is_joinable() const noexcept {
    return join_;
  }

///////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
