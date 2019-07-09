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

#include "dainty_base.h"
#include "dainty_os_clock.h"
#include "dainty_mt_detached_thread.h"
#include "dainty_mt_event_dispatcher.h"
#include "dainty_mt_timers.h"
#include "dainty_sandbox.h"

///////////////////////////////////////////////////////////////////////////////

namespace dainty
{
namespace sandbox
{
  using base::t_fd;
  using base::t_msec_;
  using container::list::t_list;
  using container::freelist::t_freelist;
  using mt::event_dispatcher::BAD_EVENT_ID;

  using t_freelist_id_value_ = container::freelist::t_id_;
  using t_thread_            = mt::detached_thread::t_thread;
  using t_dispatcher_        = mt::event_dispatcher::t_dispatcher;
  using t_tmrs_              = mt::timers::t_timers;
  using t_tmr_id_            = mt::timers::t_timer_id;
  using t_time               = os::clock::t_time;
  using R_time               = os::clock::R_time;
  using t_thread_logic_      = t_thread_::t_logic;
  using t_thread_logic_ptr_  = t_thread_::t_logic_ptr;

///////////////////////////////////////////////////////////////////////////////

  class t_impl_;
  using R_impl_ = t_prefix<t_impl_>::R_;
  using r_impl_ = t_prefix<t_impl_>::r_;
  using x_impl_ = t_prefix<t_impl_>::x_;

  class t_impl_ : public t_thread_logic_,
                  public t_dispatcher_::t_logic,
                  public t_tmrs_::t_logic {
    using base1_ = t_thread_logic_;
    using base2_ = t_dispatcher_::t_logic;
    using base3_ = t_tmrs_::t_logic;
  public:
    using t_errn = base::t_errn;

    t_impl_(t_err, R_thread_name, R_thread_params, t_n max_logics) noexcept;
    virtual ~t_impl_();

    t_impl_(R_impl_) = delete;
    t_impl_(x_impl_) = delete;
    r_impl_ operator=(R_impl_) = delete;
    r_impl_ operator=(x_impl_) = delete;

///////////////////////////////////////////////////////////////////////////////

    virtual t_void update (base1_::t_err,
                           base1_::r_pthread_attr) noexcept override final;
    virtual t_void prepare(base1_::t_err)          noexcept override final;
    virtual t_void run    ()                       noexcept override final;

///////////////////////////////////////////////////////////////////////////////

    t_void     enable_spin    (t_err, t_ix, t_spin_cnt) noexcept;
    t_void     disable_spin   (t_ix)         noexcept;
    t_spin_cnt get_spin_cnt   (t_ix)   const noexcept;
    t_msec     get_spin_period(t_ix)   const noexcept;

///////////////////////////////////////////////////////////////////////////////

    t_timer_id         start_timer  (t_err, t_ix, R_timer_name,
                                     R_timer_params)          noexcept;
    t_timer_id         start_timer  (t_err, t_ix, R_timer_name,
                                     R_timer_params,
                                     x_timer_notify_ptr)      noexcept;
    t_void             restart_timer(t_err, t_ix, t_timer_id) noexcept;
    t_void             restart_timer(t_err, t_ix, t_timer_id,
                                     R_timer_params)          noexcept;
    t_bool             stop_timer   (t_ix, t_timer_id)        noexcept;
    t_timer_notify_ptr clear_timer  (t_ix, t_timer_id)        noexcept;
    P_timer_params     get_timer    (t_ix, t_timer_id) const  noexcept;

///////////////////////////////////////////////////////////////////////////////

    t_fdevent_id         add_fdevent(t_err, t_ix, R_fdevent_name,
                                     R_fdevent_params)          noexcept;
    t_fdevent_id         add_fdevent(t_err, t_ix, R_fdevent_name,
                                     R_fdevent_params,
                                     t_fdevent_notify_ptr)      noexcept;
    t_fdevent_notify_ptr del_fdevent(t_ix, t_fdevent_id)        noexcept;
    P_fdevent_params     get_fdevent(t_ix, t_fdevent_id)  const noexcept;

///////////////////////////////////////////////////////////////////////////////

  protected:
    t_void register_logic(t_err, p_logic logic) noexcept;

  private:
    struct t_fdevent_entry_ {
      t_fdevent_name       name;
      t_fdevent_params     params;
      t_ix                 logic_ix = t_ix{0};
      t_event_id           ev_id    = BAD_EVENT_ID;
      t_fdevent_id         id       = t_fdevent_id{0};
      t_fdevent_notify_ptr notify_ptr;
    };
    using p_fdevent_entry_ = t_prefix<t_fdevent_entry_>::p_;
    using P_fdevent_entry_ = t_prefix<t_fdevent_entry_>::P_;
    using t_fdevents_      = t_freelist<t_fdevent_entry_, 200>;
    using t_fdevents_id_   = t_fdevents_::t_id;
    using t_fdevent_ids_   = t_list<t_fdevent_id, 100>;

    struct t_timer_entry_ {
      t_timer_name       name;
      t_timer_params     params;
      t_ix               logic_ix = t_ix{0};
      t_timer_id         id       = BAD_TIMER_ID;
      t_tmr_id_          tmr_id   = BAD_TIMER_ID;
      t_timer_notify_ptr notify_ptr;
    };
    using p_timer_entry_ = t_prefix<t_timer_entry_>::p_;
    using P_timer_entry_ = t_prefix<t_timer_entry_>::P_;
    using t_timers_      = t_freelist<t_timer_entry_, 200>;
    using t_timers_id_   = t_timers_::t_id;
    using t_timer_ids_   = t_list<t_timer_id, 100>;

    struct t_logic_entry_ {
      p_logic        logic        = nullptr;
      t_spin_cnt_    spin_cnt     = 0;
      t_spin_cnt_    spin_cnt_max = 0;
      t_fdevent_ids_ event_ids;
      t_timer_ids_   timer_ids;
    };
    using t_logics_ = t_list<t_logic_entry_>;

///////////////////////////////////////////////////////////////////////////////

    t_void start_             (t_err)          noexcept;
    t_void cleanup_           (t_err)          noexcept;
    t_void event_loop_        ()               noexcept;
    t_void start_extensions_  (t_err, p_logic) noexcept;
    t_void cleanup_extensions_(p_logic)        noexcept;

///////////////////////////////////////////////////////////////////////////////

    t_action notify_dispatcher_event  (t_event_id,
                                       r_event_params) noexcept override final;
    t_void notify_dispatcher_reorder  (r_event_infos)  noexcept override final;
    t_void notify_dispatcher_removed  (r_event_info)   noexcept override final;
    t_quit notify_dispatcher_timeout  (t_msec)         noexcept override final;
    t_quit notify_dispatcher_error    (t_errn)         noexcept override final;
    t_quit notify_dispatcher_processed(r_msec)         noexcept override final;
    t_void notify_timers_reorder      (r_timer_infos)  noexcept override final;
    t_void notify_timers_overrun      (t_timer_id,
                                       t_msec)         noexcept override final;
    t_void notify_timers_timeout      (t_timer_id,
                                       R_timer_params) noexcept override final;

///////////////////////////////////////////////////////////////////////////////

    t_freelist_id_value_ generate_fdevent_unique_id_() noexcept;
    p_fdevent_entry_     get_entry_      (t_fdevent_id, t_ix)       noexcept;
    P_fdevent_entry_     get_entry_      (t_fdevent_id, t_ix) const noexcept;
    t_fdevent_id         mk_fdevent_id_  (t_fdevents_id_,
                                          t_freelist_id_value_) const noexcept;
    t_fdevents_id_       get_fdevents_id_(t_fdevent_id) const noexcept;
    t_freelist_id_value_ get_unique_id_  (t_fdevent_id) const noexcept;

///////////////////////////////////////////////////////////////////////////////

    t_freelist_id_value_ generate_timer_unique_id_() noexcept;
    p_timer_entry_       get_entry_    (t_timer_id, t_ix)       noexcept;
    P_timer_entry_       get_entry_    (t_timer_id, t_ix) const noexcept;
    t_timer_id           mk_timer_id_  (t_timers_id_,
                                        t_freelist_id_value_) const noexcept;
    t_timers_id_         get_timers_id_(t_timer_id) const noexcept;
    t_freelist_id_value_ get_unique_id_(t_timer_id) const noexcept;

///////////////////////////////////////////////////////////////////////////////

    T_thread_name   name_;
    T_thread_params params_;
    t_tmrs_         tmrs_;
    t_dispatcher_   dispatcher_;
    t_logics_       logics_;
    t_fdevents_     fdevents_;
    t_timers_       timers_;
    t_event_id      tmrs_ev_id_  = BAD_EVENT_ID;
    t_spin_cnt_     spin_cnt_    = 0;
    t_msec_         spin_period_ = 10;
    t_time          last_;
  };

///////////////////////////////////////////////////////////////////////////////

  class t_single_impl_ : public t_impl_ {
  public:
    using R_name    = R_thread_name;
    using R_params  = R_thread_params;
    using x_ptr     = x_logic_ptr;
    using t_ptr     = t_logic_ptr;

    t_single_impl_(t_err, R_name, R_params, x_ptr) noexcept;

  private:
    t_ptr ptr_;
  };

///////////////////////////////////////////////////////////////////////////////

  class t_shared_impl_ : public t_impl_ {
  public:
    using R_name    = R_thread_name;
    using R_params  = R_thread_params;
    using x_ptrlist = x_logic_ptrlist;
    using t_ptrlist = t_logic_ptrlist;

    t_shared_impl_(t_err, R_name, R_params, x_ptrlist) noexcept;

  private:
    t_ptrlist ptrlist_;
  };

///////////////////////////////////////////////////////////////////////////////

}
}

#endif
