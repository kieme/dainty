/******************************************************************************

MIT License

Copyright (c) 2018 frits.germs@gmx.net

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

#ifndef _DAINTY_SANDBOX_IMPL_H_
#define _DAINTY_SANDBOX_IMPL_H_

#include "dainty_named.h"
#include "dainty_mt_detached_thread.h"
#include "dainty_mt_event_dispatcher.h"
#include "dainty_sandbox.h"

///////////////////////////////////////////////////////////////////////////////

namespace dainty
{
namespace sandbox
{
  using t_thread_           = mt::detached_thread::t_thread;
  using t_dispatcher_       = mt::event_dispatcher::t_dispatcher;
  using t_thread_logic_     = t_thread_::t_logic;
  using t_thread_logic_ptr_ = t_thread_::t_logic_ptr;

///////////////////////////////////////////////////////////////////////////////

  // each thread has a timefd shared by all timers and logics.
  // logics will store which timers belong to them, by index
  // the logic itself gets and id(index)

  class t_impl_;
  using R_impl_ = t_prefix<t_impl_>::R_;
  using r_impl_ = t_prefix<t_impl_>::r_;
  using x_impl_ = t_prefix<t_impl_>::x_;

  class t_impl_ : public t_thread_logic_, public t_dispatcher_::t_logic {
    using base1 = t_thread_logic_;
    using base2 = t_dispatcher_::t_logic;
  public:
    t_impl_(t_err, R_thread_name, t_n max_logics) noexcept;

    t_impl_(R_impl_) = delete;
    t_impl_(x_impl_) = delete;
    r_impl_ operator=(R_impl_) = delete;
    r_impl_ operator=(x_impl_) = delete;

    virtual t_void update(base1::t_err,
                          base1::r_pthread_attr) noexcept override final;
    virtual t_void prepare(base1::t_err) noexcept override final;
    virtual t_void run() noexcept override final;

    // t_logic api

    /*
    t_void add_fdevent(t_err, t_fd, t_fdevent_type, t_fdevent_user, callback); //name it
    t_void del_fdevent(t_err, t_fd, t_fdevent_type);

    t_void event_loop(t_err);      // wakeup
    t_void event_loop(t_err); // wakeup

    t_void add_wait(t_err, );
    t_void del_wait(t_err, );
    */

  protected:
    t_void register_logic(t_err, p_logic logic) noexcept;

  private:
    struct t_logic_entry_ {
      p_logic       logic = nullptr;
      t_logic_entry_(p_logic _logic) : logic(_logic) { }
    };
    using t_logics_ = container::list::t_list<t_logic_entry_>;

///////////////////////////////////////////////////////////////////////////////

    t_void start_     (t_err) noexcept;
    t_void cleanup_   (t_err) noexcept;
    t_void event_loop_() noexcept;
    t_void start_extensions_  (t_err, p_logic) noexcept;
    t_void cleanup_extensions_(p_logic) noexcept;

///////////////////////////////////////////////////////////////////////////////

    virtual t_void may_reorder_events(base2::r_event_infos) override final;
    virtual t_void notify_event_remove(base2::r_event_info) override final;
    virtual base2::t_quit notify_timeout(base2::t_usec) override final;
    virtual base2::t_quit notify_error(base2::t_errn) override final;
    virtual base2::t_quit notify_events_processed() override final;

///////////////////////////////////////////////////////////////////////////////

    t_thread_name name_;
    t_dispatcher_ dispatcher_;
    t_logics_     logics_;
    // spinning_;
    // fds_;
    // timers_;
  };

///////////////////////////////////////////////////////////////////////////////

  class t_single_impl_ : public t_impl_ {
  public:
    t_single_impl_(t_err, R_thread_name, x_logic_ptr) noexcept;

  private:
    t_logic_ptr ptr_;
  };

///////////////////////////////////////////////////////////////////////////////

  class t_shared_impl_ : public t_impl_ {
  public:
    t_shared_impl_(t_err, R_thread_name, x_logic_ptrlist) noexcept;

  private:
    t_logic_ptrlist ptrlist_;
  };

///////////////////////////////////////////////////////////////////////////////

}
}

#endif
